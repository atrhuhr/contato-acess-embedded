#include <Arduino.h>
#include <bluefruit.h>
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>
#include "LSM6DS3.h"
#include <Wire.h>
#include "config.h"
#include "types.h"

using namespace Adafruit_LittleFS_Namespace; // Para usar a classe InternalFS: Sistema de arquivos interno permanente

// -- BLE objects --  
// https://en.wikipedia.org/wiki/Bluetooth_Low_Energy
BLEService        mainSvc(MAIN_SERVICE_UUID);
BLECharacteristic midiChar(MIDI_CHAR_UUID);
BLECharacteristic sectionsChar(SECTIONS_CHAR_UUID);
BLECharacteristic accelSensChar(ACCEL_SENS_CHAR_UUID);
BLECharacteristic dirChar(DIR_CHAR_UUID);
BLECharacteristic statusChar(STATUS_CHAR_UUID);
BLECharacteristic calibrateChar(CALIBRATE_CHAR_UUID);

// -- IMU --
// https://wiki.seeedstudio.com/XIAO-BLE-Sense-IMU-Usage/
LSM6DS3 imu(I2C_MODE, 0x6A);

// -- State --
StatusPacket statusPkt;
IMUOffsets    imuOffsets     = {};

static float          elevationAngle    = 0.0f;
static const float    CF_ALPHA          = 0.96f;
static bool           calibrationPending = false;

int32_t       accelThreshold = DEFAULT_ACCEL_THRESHOLD;
uint8_t       flipDir        = 1;
uint8_t       notesBuf[32]   = {};
uint16_t      notesLen       = 0;
unsigned long lastSent       = 0; 
unsigned long lastAccel      = 0;
unsigned long lastPrint      = 0;
bool          accelFlag      = false;

// -- Helpers --
static float clamp(float v, float hi, float lo) {
    if (v > hi) return hi;
    if (v < lo) return lo;
    return v;
}

static void saveFile(const char *path, const void *data, size_t len) {
    File f = InternalFS.open(path, FILE_O_WRITE);
    if (f) { f.write((const uint8_t *)data, len); f.close(); }
}

static bool loadFile(const char *path, void *data, size_t len) {
    File f = InternalFS.open(path, FILE_O_READ);
    if (!f) return false;
    f.read((uint8_t *)data, len);
    f.close();
    return true;
}

// -- Calibração do IMU --
static void calibrateIMU() {
    Serial.println("Calibrating IMU — hold still...");
    double sumAx = 0, sumAy = 0, sumAz = 0;
    double sumGx = 0, sumGy = 0, sumGz = 0;
    int n = 0;
    unsigned long start = millis();
    while (millis() - start < CALIB_DURATION_MS) {
        sumAx += imu.readFloatAccelX();
        sumAy += imu.readFloatAccelY();
        sumAz += imu.readFloatAccelZ();
        sumGx += imu.readFloatGyroX();
        sumGy += imu.readFloatGyroY();
        sumGz += imu.readFloatGyroZ();
        n++;
        delay(4);
    }
    imuOffsets.ax = sumAx / n;
    imuOffsets.ay = sumAy / n;
    imuOffsets.az = sumAz / n;
    imuOffsets.gx = sumGx / n;
    imuOffsets.gy = sumGy / n;
    imuOffsets.gz = sumGz / n;
    elevationAngle = 0.0f;
    saveFile(FILE_IMU_OFFSETS, &imuOffsets, sizeof(imuOffsets));
    Serial.printf("Calibração feita (%d samples). ax=%.4f ay=%.4f az=%.4f gx=%.4f gy=%.4f gz=%.4f\n",
                  n, imuOffsets.ax, imuOffsets.ay, imuOffsets.az,
                  imuOffsets.gx, imuOffsets.gy, imuOffsets.gz);
}

// -- MIDI helpers --
// https://midi.org/summary-of-midi-1-0-messages
// https://www.geeksforgeeks.org/cpp/bitmasking-in-cpp/
static void sendMidi(uint8_t status, uint8_t d1, uint8_t d2) {
    uint8_t pkt[5] = {0x80, 0x80, status, d1, d2};
    midiChar.notify(pkt, 5); 
}

static void playNote(uint8_t note, uint8_t channel) {
    sendMidi(0x90 | (channel & 0x0F), note, 100);
}

static void stopNote(uint8_t note, uint8_t channel) {
    sendMidi(0x80 | (channel & 0x0F), note, 0);
}

// -- BLE callbacks --
static void onSectionsWrite(uint16_t /*conn*/, BLECharacteristic *chr,
                             uint8_t *data, uint16_t len) {
    if (len == 0 || len > 32) return;
    memcpy(notesBuf, data, len);
    notesLen = len;
    saveFile(FILE_SECTIONS, notesBuf, notesLen);
}

static void onAccelSensWrite(uint16_t /*conn*/, BLECharacteristic *chr,
                              uint8_t *data, uint16_t len) {
    if (len < 4) return;
    int32_t v;
    memcpy(&v, data, 4);
    v = constrain(v, MIN_ACCEL_THRESHOLD, MAX_ACCEL_THRESHOLD);
    accelThreshold = v;
    saveFile(FILE_SENS, &accelThreshold, sizeof(accelThreshold));
}

static void onDirWrite(uint16_t /*conn*/, BLECharacteristic *chr,
                        uint8_t *data, uint16_t len) {
    if (len < 1) return;
    flipDir = data[0] ? 1 : 0;
    saveFile(FILE_DIR, &flipDir, sizeof(flipDir));
}

static void onCalibrateWrite(uint16_t /*conn*/, BLECharacteristic *chr,
                              uint8_t *data, uint16_t len) {
    if (len < 1 || data[0] != 1) return;
    calibrationPending = true;
}

static void onConnect(uint16_t /*conn*/) {
    digitalWrite(LED_BUILTIN, LOW);
}

static void onDisconnect(uint16_t /*conn*/, uint8_t /*reason*/) {
    digitalWrite(LED_BUILTIN, HIGH);
}

// -- Setup --
void setup() {
    Serial.begin(115200);
    unsigned long _t = millis();
    while (!Serial && millis() - _t < 3000);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    pinMode(PIN_LSM6DS3TR_C_POWER, OUTPUT);
    digitalWrite(PIN_LSM6DS3TR_C_POWER, HIGH);
    delay(10);

    Wire.setClock(I2C_CLOCK_HZ);
    if (imu.begin() != 0)
        Serial.println("IMU error");

    InternalFS.begin(); 

    // Calibração IMU
    if (!loadFile(FILE_IMU_OFFSETS, &imuOffsets, sizeof(imuOffsets)))
        calibrateIMU();
    else
        Serial.printf("Offsets loaded: ax=%.4f ay=%.4f az=%.4f gx=%.4f gy=%.4f gz=%.4f\n",
                      imuOffsets.ax, imuOffsets.ay, imuOffsets.az,
                      imuOffsets.gx, imuOffsets.gy, imuOffsets.gz);

    // Inicializa configurações já atribuídas em boots anteriores
    uint8_t tmpNotes[32];
    uint16_t tmpLen = 0;
    if (loadFile(FILE_SECTIONS, tmpNotes, sizeof(tmpNotes))) {
        // determine actual len by reading file size
        File f = InternalFS.open(FILE_SECTIONS, FILE_O_READ);
        if (f) { tmpLen = f.size(); f.close(); }
        if (tmpLen > 0 && tmpLen <= 32) {
            memcpy(notesBuf, tmpNotes, tmpLen);
            notesLen = tmpLen;
        }
    }
    loadFile(FILE_SENS, &accelThreshold, sizeof(accelThreshold));
    loadFile(FILE_DIR,  &flipDir,        sizeof(flipDir));
    accelThreshold = constrain(accelThreshold, MIN_ACCEL_THRESHOLD, MAX_ACCEL_THRESHOLD);

    // BLE init
    Bluefruit.begin();
    Bluefruit.setName(DEVICE_NAME);
    Bluefruit.Periph.setConnectCallback(onConnect);
    Bluefruit.Periph.setDisconnectCallback(onDisconnect);

    // Service
    mainSvc.begin();

    // MIDI characteristic
    midiChar.setProperties(CHR_PROPS_READ | CHR_PROPS_WRITE_WO_RESP | CHR_PROPS_NOTIFY);
    midiChar.begin();

    // Sections characteristic: Seções (Notas)
    sectionsChar.setProperties(CHR_PROPS_READ | CHR_PROPS_WRITE);
    sectionsChar.setMaxLen(32);
    sectionsChar.setWriteCallback(onSectionsWrite);
    sectionsChar.begin();
    if (notesLen > 0)
        sectionsChar.write(notesBuf, notesLen);

    // Accel sensitivity characteristic: Sensibilidade acelerômetro
    accelSensChar.setProperties(CHR_PROPS_READ | CHR_PROPS_WRITE);
    accelSensChar.setWriteCallback(onAccelSensWrite);
    accelSensChar.begin();
    accelSensChar.write32((int)accelThreshold);

    // Direction flip characteristic: Orientação esquerda/direita 
    dirChar.setProperties(CHR_PROPS_READ | CHR_PROPS_WRITE);
    dirChar.setWriteCallback(onDirWrite);
    dirChar.begin();
    dirChar.write8(flipDir);

    // Status notify characteristic
    statusChar.setProperties(CHR_PROPS_NOTIFY);
    statusChar.begin();

    // Calibrate characteristic: Write-only que inicializa rotina de calibragem remotamente
    calibrateChar.setProperties(CHR_PROPS_WRITE);
    calibrateChar.setWriteCallback(onCalibrateWrite);
    calibrateChar.begin();

    // Advertising
    Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
    Bluefruit.Advertising.addService(mainSvc);
    Bluefruit.ScanResponse.addName();
    Bluefruit.Advertising.restartOnDisconnect(true);
    Bluefruit.Advertising.start(0);

    Serial.println("BLE started");
}

// -- Loop --
void loop() {
    unsigned long now = millis();
    if (now - lastSent < STATUS_INTERVAL_MS) return;
    float dt = (now - lastSent) / 1000.0f;
    lastSent = now;

    if (calibrationPending) {
        calibrationPending = false;
        calibrateIMU();
    }

    float ay_raw = imu.readFloatAccelY();
    float az_raw = imu.readFloatAccelZ();
    float ax     = imu.readFloatAccelX() - imuOffsets.ax;
    float gyY    = imu.readFloatGyroY()  - imuOffsets.gy;

    // gyro: ângulo de elevação, normalmente vai de valores entre ~ +85 até -85
    float accelElevation = atan2f(-ax, sqrtf(ay_raw*ay_raw + az_raw*az_raw)) * RAD_TO_DEG;
    elevationAngle = CF_ALPHA * (elevationAngle + gyY * dt)
                   + (1.0f - CF_ALPHA) * accelElevation;

    int gyro = (int)clamp(elevationAngle, GYRO_MAX_DEG, -GYRO_MAX_DEG);
    if (flipDir) gyro = -gyro;

    // accel: normalmente flutua entre -200 e 200 em repouso
    float total_g = sqrtf(ax*ax + ay_raw*ay_raw + az_raw*az_raw);
    int accel     = (int)((total_g - 1.0f) * 1000.0f);

    // Seção (Nota) MIDI atual
    int section = (int)((-gyro + GYRO_MAX_DEG) / (2.0f * GYRO_MAX_DEG) * notesLen);
    if (section >= (int)notesLen) section = (int)notesLen - 1;
    if (section < 0)              section = 0;

    if (now - lastPrint >= 30) {
        Serial.printf("elev=%d  accel=%d  thr=%d\n", (int)elevationAngle, accel, (int)accelThreshold);
        lastPrint = now;
    }

    // Gatilho acelerômetro
    if (!accelFlag && abs(accel) > accelThreshold
        && (now - lastAccel) >= ACCEL_DEBOUNCE_MS) {
        Serial.printf("TRIGGER accel=%d\n", accel);
        if (Bluefruit.Periph.connected()) playNote(PERC_NOTE, PERC_CHANNEL);
        accelFlag = true;
        lastAccel = now;
    }
    if (accelFlag && (now - lastAccel) >= ACCEL_DEBOUNCE_MS) {
        if (Bluefruit.Periph.connected()) stopNote(PERC_NOTE, PERC_CHANNEL);
        accelFlag = false;
    }

    if (Bluefruit.Periph.connected()) {
        statusPkt.gyro  = (int16_t)gyro;
        statusPkt.accel = (int16_t)accel;
        statusPkt.touch = 0;
        statusChar.notify((uint8_t *)&statusPkt, sizeof(StatusPacket));
    }
}
