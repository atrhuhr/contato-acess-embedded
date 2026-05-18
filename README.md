# Contato (hardware)
[![en](https://img.shields.io/badge/lang-en-red.svg)](README.en.md)

Código embarcado para o dispositivo **Contato**, desenvolvido pelo curso de Dança da Universidade Federal do Rio de Janeiro em parceria com o Parque Tecnológico UFRJ.

O dispositivo é baseado em um módulo **ESP32 DEVKIT V1** com um **IMU MPU6050** (giroscópio + acelerômetro em 6 graus de liberdade). Ele captura o movimento do usuário e transmite mensagens **MIDI via Bluetooth Low Energy (BLE)**, permitindo interação musical a partir do movimento corporal e do toque capacitivo.

## Como funciona

- O ângulo de rolagem do giroscópio (±89°) determina qual nota MIDI está selecionada dentre as seções configuráveis.
- O toque no sensor capacitivo dispara Note-On/Note-Off da nota selecionada.
- Picos de aceleração no eixo X disparam uma nota de percussão (canal MIDI 8).
- Toda a configuração (notas, sensibilidade, calibração) é persistida na memória não-volátil do ESP32 (NVS) e pode ser alterada pelo cliente BLE.

## 📁 Organização

```
contato_hardware/
├── platformio.ini
├── src/
│   └── main.cpp  # firmware principal
└── include/
    ├── config.h  # pinos, UUIDs BLE, constantes MIDI e de tempo
    └── types.h   # structs de dados (StatusPacket, MPUOffsets)
```


