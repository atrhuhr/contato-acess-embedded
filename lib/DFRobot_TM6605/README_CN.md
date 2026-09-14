# DFRobot_TM6605

* [English Version](./README.md)

TM6605 是一款专为解决在驱动线性振动马达(LRA)触觉反馈元件中常见障碍的触觉反馈驱动器。TM6605 还被用于为具有低延迟、极高的效率、以及大驱动强度的便携式器件中常用驱动器提供驱动力量。LRA驱动器通常有一个窄频带，在该频带内它们有充分的触觉反馈响应。这个频率窗口通常在±2.5Hz左右，所以对驱动一个LRA驱动器来说是一个挑战。

TM6605 通过采用自动谐振跟踪解决了这个问题，它会自动检测并跟踪最佳的换向频率。这意味着TM6605 会自动产生正确的谐振输出频率。作为一个额外的好处就是TM6605 能够执行优化的制动算法，以此阻止LRA振铃，留给用户一个清晰的触觉反馈感觉。TM6605 还有电源校正功能来确保无电源调节时的恒定振动强度，从而允许与一个高效直流电池连接。本产品性能优良，质量可靠。

![正反面svg效果图](./resources/images/DRI0056.png)

## 产品链接（ https://www.dfrobot.com.cn/goods-4226.html ）

DRI0056 Gravity: TM6605振动电机

## 目录

* [简介](#简介)
* [安装](#安装)
* [方法](#方法)
* [兼容性](#兼容性)
* [历史](#历史)
* [贡献者](#贡献者)

## 简介

本库提供了在Arduino IDE下驱动TM6605的办法，可选择多种振动效果。

## 安装

1.要使用此库，首先下载库文件，将其粘贴到 `\Arduino\libraries` 目录中，然后打开示例文件夹并运行其中的示例。

## 方法
```c++
  /**
   * @fn begin
   * @brief 查找IIC总线上是否存在TM6605设备
   * @return 是否发现传感器设备
   * @retval 0:存在传感器设备，其他值：不存在传感器设备
  */ 
  byte begin(void);
  /**
   * @fn startHaptic
   * @brief 开始效果播放
   * @return null
  */
  void play(void);

  /**
   * @fn stopHaptic
   * @brief 停止效果播放
   * @return null
  */
  void stop(void);

  /**
   * @fn selectEffect
   * @brief 选择振动效果
   * @return null
  */
  void selectEffect(eEffect_t effect);

```


## 兼容性

MCU                | 表现良好	|表现异常	|未测试	|备注 |
------------------ | :----------: | :----------: | :---------: | -----
Arduino UNO        |      √       |              |             | 
Arduino MEGA2560        |      √       |              |             | 
Arduino Leonardo        |      √       |              |             | 
FireBeetle-ESP32        |      √       |              |             | 
FireBeetle-ESP8266        |      √       |              |             | 
FireBeetle-M0        |      √       |              |             | 
Micro:bit        |      √       |              |             |


## 历史

- Date 2025-04-10
- Version V1.0.0


## 贡献者

Written by fary(feng.yang@dfrobot.com), 2025.04.10 (Welcome to our [website](https://www.dfrobot.com.cn/))
