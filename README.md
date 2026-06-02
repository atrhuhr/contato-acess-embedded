# Contato (hardware)
[![en](https://img.shields.io/badge/lang-en-red.svg)](README.en.md)

Código embarcado para o dispositivo **Contato**, desenvolvido pelo curso de Dança da Universidade Federal do Rio de Janeiro em parceria com o Parque Tecnológico UFRJ.

O dispositivo é baseado no módulo **Seeed XIAO nRF52840 Sense** com IMU **LSM6DS3** integrado (giroscópio + acelerômetro em 6 graus de liberdade). Ele captura o movimento do usuário e transmite mensagens **MIDI via Bluetooth Low Energy (BLE)**, permitindo interação musical a partir do movimento corporal.

## Como funciona

- No primeiro boot, o dispositivo realiza uma **calibração automática do IMU** (2,5s com o sensor parado), armazenando os offsets de bias do giroscópio e acelerômetro na memória interna (LittleFS). Boots subsequentes carregam os offsets salvos.
- O **ângulo de elevação** do eixo X é calculado por um **filtro complementar** (α = 0,90) combinando integração do giroscópio e ângulo derivado do acelerômetro via `atan2`, estável em toda a faixa de ±90°.
- O ângulo (±90°) determina qual seção de notas MIDI está ativa dentre as seções configuráveis.
- **Picos de aceleração** no eixo X acima de um limiar disparam uma nota de percussão (MIDI note 36, canal 8).
- Toda a configuração é persistida na memória não-volátil e pode ser alterada pelo cliente BLE.

## Organização

```
contato-acess-embedded/
├── platformio.ini
├── src/
│   └── main.cpp        # firmware principal
└── include/
    ├── config.h        # pinos, UUIDs BLE, constantes MIDI e de tempo
    └── types.h         # structs (StatusPacket, IMUOffsets)
```

## Características BLE

O dispositivo anuncia um serviço BLE principal com as seguintes características:

| Característica   | Operações       | Descrição                                              |
|------------------|-----------------|--------------------------------------------------------|
| `midiChar`       | Read/Write/Notify | Canal MIDI BLE (especificação Apple/MIDI Association) |
| `sectionsChar`   | Read/Write      | Lista de notas MIDI por seção (até 32 bytes)           |
| `accelSensChar`  | Read/Write      | Limiar de aceleração para percussão (int32)            |
| `dirChar`        | Read/Write      | Inversão de direção do ângulo (0 ou 1)                 |
| `statusChar`     | Notify          | Pacote de status: ângulo, aceleração, toque            |
| `calibrateChar`  | Write           | Escreva `0x01` para recalibrar o IMU                   |

## Plataforma

- **Board**: Seeed XIAO nRF52840 Sense
- **Framework**: Arduino (via PlatformIO)
- **IMU**: LSM6DS3 (I2C, 400 kHz)
- **Armazenamento**: LittleFS (memória flash interna)
