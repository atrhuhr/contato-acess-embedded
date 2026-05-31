#include <Arduino.h>
#include "LSM6DS3.h"
#include "Wire.h"

#define IMU_PWR_PIN PIN_LSM6DS3TR_C_POWER

LSM6DS3 imu(I2C_MODE, 0x6A);

void setup() {
    Serial.begin(115200);
    while (!Serial);

    pinMode(IMU_PWR_PIN, OUTPUT);
    digitalWrite(IMU_PWR_PIN, HIGH);
    delay(10);

    if (imu.begin() != 0)
        Serial.println("IMU error");
    else
        Serial.println("IMU OK");
}

void loop() {
    Serial.print("Accel X:"); Serial.print(imu.readFloatAccelX(), 4);
    Serial.print(" Y:");      Serial.print(imu.readFloatAccelY(), 4);
    Serial.print(" Z:");      Serial.print(imu.readFloatAccelZ(), 4);
    Serial.print("  Gyro X:"); Serial.print(imu.readFloatGyroX(), 4);
    Serial.print(" Y:");       Serial.print(imu.readFloatGyroY(), 4);
    Serial.print(" Z:");       Serial.println(imu.readFloatGyroZ(), 4);
    delay(100);
}
