# DFRobot_TM6605

* [English Version](./README.md)

TM6605 是一款专为解决在驱动线性振动马达(LRA)触觉反馈元件中常见障碍的触觉反馈驱动器。TM6605 还被用于为具有低延迟、极高的效率、以及大驱动强度的便携式器件中常用驱动器提供驱动力量。LRA驱动器通常有一个窄频带，在该频带内它们有充分的触觉反馈响应。这个频率窗口通常在±2.5Hz左右，所以对驱动一个LRA驱动器来说是一个挑战。

TM6605 通过采用自动谐振跟踪解决了这个问题，它会自动检测并跟踪最佳的换向频率。这意味着TM6605 会自动产生正确的谐振输出频率。作为一个额外的好处就是TM6605 能够执行优化的制动算法，以此阻止LRA振铃，留给用户一个清晰的触觉反馈感觉。TM6605 还有电源校正功能来确保无电源调节时的恒定振动强度，从而允许与一个高效直流电池连接。本产品性能优良，质量可靠。

![Product Image](../../resources/images/DRI0056.png)


## 产品链接（https://www.dfrobot.com.cn/)

DRI0056 Gravity: TM6605振动电机

## 目录

* [简介](#简介)
* [安装](#安装)
* [方法](#方法)
* [兼容性](#兼容性)
* [历史](#历史)
* [贡献者](#贡献者)

## 简介

本库提供了在树莓派下驱动 TM6605 的办法，可选择多种振动效果。

## 安装

要使用这个库，首先将库下载到Raspberry Pi，然后打开用例文件夹。要执行一个用例demox.py，请在命令行中输入python demox.py。
本库依赖smbus库，使用本库前，请保证已经安装了smbus。安装方式：pip install smbus2

## 方法
```python
    def begin(self)
        """
        @fn begin
        @brief 查找IIC总线上是否存在TM6605设备
        @return 是否发现传感器设备
        @retval 0:存在传感器设备，-1：不存在传感器设备
        """
    def play(self)
        '''
        @fn play
        @brief 开始效果播放
        '''
    def stop(self)
        '''
        @fn stop
        @brief 停止效果播放
        '''

    def select_effect(self,effect)
        '''
        @fn select_effect
        @brief Select vibration effect，确保参数是 Effect 枚举成员或有效整数值
        @param effect - 必须是 Effect 枚举成员或对应的有效整数值
        @raise ValueError - 当值无效时抛出
        @raise TypeError - 当类型错误时抛出
        '''
```

## 兼容性

| MCU         | 表现良好	|表现异常	|未测试	|备注 |
| ------------ | :--: | :----: | :----: | :--: |
| RaspberryPi4 |  √   |        |        |      |

* Python 版本 

| Python  | 表现良好	|表现异常	|未测试	|备注 |
| ------- | :--: | :----: | :----: | ---- |
| Python2 |  √   |        |        |      |
| Python3 |  √   |        |        |      |
## 历史 

- 2025/04/22 - Version 1.0.0 released.

## 贡献者

Written by fary(feng.yang@dfrobot.com), 2025.04.10 (Welcome to our [website](https://www.dfrobot.com.cn/))





