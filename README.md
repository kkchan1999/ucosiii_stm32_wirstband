# ucosiii_stm32_wirstband
基于STM32F407和μC/OS-III的智能手环

具体要实现的功能：

- 基于MPU6050的计步功能
- ~~时间显示功能~~
- 表盘切换
- 翻腕唤醒
- 自动休眠
- ~~血氧测量~~
- 蓝牙时间同步



模块的选用：

- GEC-M4 STM32F407开发板

- MPU6050三轴加速度&陀螺仪模块
- MAX30102
- IIC 0.96寸的LED显示屏
- HC-05串口蓝牙模块
- MFRC522 RFID模块（暂时没用到）