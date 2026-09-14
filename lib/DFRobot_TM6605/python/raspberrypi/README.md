# DFRobot_TM6605

* [中文版](./README_CN.md)

TM6605 is a haptic feedback driver specifically designed to address common obstacles in driving linear resonant actuators (LRA) tactile feedback components. The TM6605 is also used to provide driving power for drivers commonly used in portable devices with low latency, extremely high efficiency, and strong drive intensity. LRAs typically have a narrow frequency band within which they exhibit sufficient tactile feedback response. This frequency window is usually around ±2.5Hz, making it a challenge to drive an LRA.

The TM6605 solves this issue by using automatic resonance tracking, automatically detecting and tracking the optimal commutation frequency. This means that the TM6605 will automatically generate the correct resonant output frequency. As an additional benefit, the TM6605 can execute an optimized braking algorithm to prevent LRA ringing, leaving users with a clear tactile feedback sensation. The TM6605 also features power correction functionality to ensure constant vibration intensity without power regulation, allowing connection to an efficient DC battery. This product performs well and is highly reliable.

![Product Image](../../resources/images/DRI0056.png)

## Product Link（https://www.dfrobot.com/)

DRI0056 Gravity: TM6605 Vibration Motor

## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

## Summary


This library provides a way to drive the TM6605 under the Raspberry Pi, with multiple vibration effects to choose from.

## Installation

To use this library, first download the library to your Raspberry Pi and then open the use cases folder. To perform a use case demox.py, enter the python demox.py on the command line.
This library depends on the SMBUS library, please ensure that SMBUS has been installed before using this library. Installation: pip install smbus2

## Methods
```python
  
    def begin(self)
        """
        @fn begin
        @brief Check whether the TM6605 device exists on the IIC bus.
        @return Whether the sensor device is found.
        @retval 0: Sensor device exists, -1: Sensor device does not exist.
        """
    def play(self)
        '''
        @fn play
        @brief Start effect playback
        '''
    def stop(self)
        '''
        @fn stop
        @brief Stop effect playback
        '''

    def select_effect(self,effect)
        '''
        @fn select_effect
        @brief Select vibration effect, ensuring the parameter is an Effect enum member or valid integer value
        @param effect - Must be an Effect enum member or its corresponding valid integer value
        @raise ValueError - Raised when an invalid value is provided
        @raise TypeError - Raised when an incorrect type is provided
        '''

```

## Compatibility

| MCU         | Work Well | Work Wrong  | Untested | Remarks |
| ------------ | :--: | :----: | :----: | :--: |
| RaspberryPi4 |  √   |        |        |      |

* Python version 

| Python  | Work Well | Work Wrong | Untested | Remarks |
| ------- | :--: | :----: | :----: | ---- |
| Python2 |  √   |        |        |      |
| Python3 |  √   |        |        |      |
## History 

- 2025/04/22 - Version 1.0.0 released.

## Credits

Written by fary(feng.yang@dfrobot.com), 2025.04.10 (Welcome to our [website](https://www.dfrobot.com/))



