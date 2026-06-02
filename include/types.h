#pragma once

#include <cstdint>

struct __attribute__((packed)) StatusPacket {
    int16_t gyro_x;   // elevation angle em graus (limitado a ±GYRO_MAX_DEG)
    int16_t accel_x;  // aceleração linear no eixo X (g * 1000)
    uint8_t touch;    // reservado
};

struct IMUOffsets {
    float ax, ay, az;
    float gx, gy, gz;
};
