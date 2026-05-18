# Contato (hardware)
[![pt-br](https://img.shields.io/badge/lang-pt--br-green.svg)](README.md)

Embedded firmware for the **Contato** device, developed by the Dance Course at Universidade Federal do Rio de Janeiro in partnership with the UFRJ Technological Park.

The device is built around an **ESP32 DEVKIT V1** and an **MPU6050 IMU** (6-axis gyroscope + accelerometer). It captures the user's motion and transmits **MIDI messages over Bluetooth Low Energy (BLE)**, enabling musical interaction driven by body movement and capacitive touch.

## How it works

- The gyroscope roll angle (±89°) selects a MIDI note from a configurable set of sections.
- Touching the capacitive sensor triggers Note-On/Note-Off for the currently selected note.
- Acceleration spikes on the X-axis trigger a percussion note (MIDI channel 8).
- All configuration (notes, sensitivity, calibration offsets) is persisted in the ESP32's non-volatile storage (NVS) and can be updated by a BLE client.

## 📁 Structure

```
contato_hardware/
├── arduino/          # legacy implementations (ESP-NOW P2P, no longer maintained)
└── platformio/       # active project
    ├── platformio.ini
    ├── src/
    │   └── main.cpp  # main firmware
    └── include/
        ├── config.h  # pins, BLE UUIDs, MIDI and timing constants
        └── types.h   # data structs (StatusPacket, MPUOffsets)
```
