# Contato (hardware)
[![pt](https://img.shields.io/badge/lang-pt-green.svg)](README.md)

Embedded firmware for the **Contato** device, developed by the Dance department of the Federal University of Rio de Janeiro in partnership with the UFRJ Technology Park.

The device is based on the **Seeed XIAO nRF52840 Sense** module with its onboard **LSM6DS3** IMU (6-DOF gyroscope + accelerometer). It captures the user's movement and transmits **MIDI messages over Bluetooth Low Energy (BLE)**, enabling musical interaction through body movement.

## How it works

- On first boot, the device runs an **automatic IMU calibration** (2.5s while held still), storing gyroscope and accelerometer bias offsets in internal flash (LittleFS). Subsequent boots load the saved offsets.
- The **elevation angle** of the X axis is computed by a **complementary filter** (α = 0.90) combining gyroscope integration and accelerometer-derived angle via `atan2`, stable across the full ±90° range.
- The angle (±90°) selects which MIDI note section is active from the configurable section list.
- **Acceleration spikes** on the X axis above a threshold trigger a percussion note (MIDI note 36, channel 8).
- All configuration is persisted to non-volatile memory and can be changed by a BLE client.

## Project structure

```
contato-acess-embedded/
├── platformio.ini
├── src/
│   └── main.cpp        # main firmware
└── include/
    ├── config.h        # pins, BLE UUIDs, MIDI and timing constants
    └── types.h         # structs (StatusPacket, IMUOffsets)
```

## BLE characteristics

The device advertises a main BLE service with the following characteristics:

| Characteristic   | Operations      | Description                                              |
|------------------|-----------------|----------------------------------------------------------|
| `midiChar`       | Read/Write/Notify | BLE MIDI channel (Apple/MIDI Association spec)         |
| `sectionsChar`   | Read/Write      | MIDI note list per section (up to 32 bytes)              |
| `accelSensChar`  | Read/Write      | Acceleration threshold for percussion (int32)            |
| `dirChar`        | Read/Write      | Angle direction flip (0 or 1)                            |
| `statusChar`     | Notify          | Status packet: angle, acceleration, touch                |
| `calibrateChar`  | Write           | Write `0x01` to recalibrate the IMU                      |

## Platform

- **Board**: Seeed XIAO nRF52840 Sense
- **Framework**: Arduino (via PlatformIO)
- **IMU**: LSM6DS3 (I2C, 400 kHz)
- **Storage**: LittleFS (internal flash)
