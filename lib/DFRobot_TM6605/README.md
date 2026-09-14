# DFRobot_TM6605

* [中文版](./README_CN.md)

TM6605 is a haptic feedback driver specifically designed to address common obstacles in driving linear resonant actuators (LRA) tactile feedback components. The TM6605 is also used to provide driving power for drivers commonly used in portable devices with low latency, extremely high efficiency, and strong drive intensity. LRAs typically have a narrow frequency band within which they exhibit sufficient tactile feedback response. This frequency window is usually around ±2.5Hz, making it a challenge to drive an LRA.

The TM6605 solves this issue by using automatic resonance tracking, automatically detecting and tracking the optimal commutation frequency. This means that the TM6605 will automatically generate the correct resonant output frequency. As an additional benefit, the TM6605 can execute an optimized braking algorithm to prevent LRA ringing, leaving users with a clear tactile feedback sensation. The TM6605 also features power correction functionality to ensure constant vibration intensity without power regulation, allowing connection to an efficient DC battery. This product performs well and is highly reliable.

   
   
![正反面svg效果图](./resources/images/DRI0056.png)

## Product Link（ https://www.dfrobot.com/product-3003.html )

DRI0056 Gravity: TM6605 Vibration Motor

## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)


## Summary


This library provides a way to drive the TM6605 under the Arduino IDE, with multiple vibration effects to choose from.


## Installation

1.To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo 
## Methods
```C++

  /**
   * @fn begin
   * @brief Check whether the TM6605 device exists on the IIC bus.
   * @return Whether the sensor device is found.
   * @retval 0: Sensor device exists, other values: Sensor device does not exist.
  */
  byte begin(void);
  /**
   * @fn play
   * @brief Start effect playback
  */
  void play(void);

  /**
   * @fn stop
   * @brief Stop effect playback
  */
  void stop(void);

  /**
   * @fn selectEffect
   * @brief Select vibration effect
  */
  void selectEffect(eEffect_t effect);


```

## Compatibility

MCU                | Work Well | Work Wrong  | Untested | Remarks |
------------------ | :----------: | :----------: | :---------: | -----
Arduino UNO        |      √       |              |             | 
Arduino MEGA2560        |      √       |              |             | 
Arduino Leonardo        |      √       |              |             | 
FireBeetle-ESP32        |      √       |              |             | 
FireBeetle-ESP8266        |      √       |              |             | 
FireBeetle-M0        |      √       |              |             | 
Micro:bit        |      √       |              |             |

## History

- Date 2025-04-10
- Version V1.0.0
## Credits
Written by fary(feng.yang@dfrobot.com), 2025.04.10 (Welcome to our [website](https://www.dfrobot.com/))
