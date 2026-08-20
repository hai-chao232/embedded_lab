# embedded-lab 学习路线与实验清单

> 活文档：随学习进度调整。编号只表示顺序；Level 记录在本表与各实验 README 中。

## Level 约定

| Level | 手段 | 说明 |
|:-----:|------|------|
| L1 | 寄存器 / CMSIS | 直接操作寄存器，理解硬件 |
| L2 | STM32 LL | 厂商底层抽象 |
| L3 | HAL | 快速工程开发 |

- 同一外设尽量按 L1 → L2 → L3 递进；一个实验可以混合层次，在 README 里写清哪部分用哪个。
- 例外：USB 直接从框架级开始（TinyUSB / ST USB Device），不手搓 Chapter 9。

## 第一阶段：MCU 底层机制（GPIO / 中断 / Timer / DMA / UART）

| 编号 | 实验 | Level | 依赖 | 硬件 | 状态 |
|:----:|------|:-----:|------|------|:----:|
| 001 | gpio_output 点灯 | L1（对比 L2/L3） | 平台就绪 | LED | ⬜ |
| 002 | gpio_input 按键轮询 | L1 | 001 | 按键 | ⬜ |
| 003 | exti 外部中断 | L1 | 002 | 按键 + LED | ⬜ |
| 004 | nvic 优先级与抢占 | L1 | 003 | 按键 + LED | ⬜ |
| 005 | timer 基本定时中断 | L1 | 004 | LED | ⬜ |
| 006 | timer PWM 呼吸灯 | L2 | 005 | LED + 逻辑分析仪 | ⬜ |
| 007 | timer 输入捕获测频 | L2 | 005 | 信号源 + 逻辑分析仪 | ⬜ |
| 008 | uart 寄存器级收发回环 | L1 | 004 | CH340 | ⬜ |
| 009 | uart DMA 收发 | L2 | 008 | CH340 | ⬜ |
| 010 | dma mem2mem 搬运 | L1 | 009 | 逻辑分析仪 | ⬜ |

## 第二阶段：通信能力（草案，编号可能调整）

| 编号 | 实验 | Level | 硬件 | 状态 |
|:----:|------|:-----:|------|:----:|
| 011 | i2c bit-bang 软件模拟 | L1 | 24LC256 + LA | ⬜ |
| 012 | i2c 硬件外设 | L2 | 24LC256 + LA | ⬜ |
| 013 | spi 寄存器级 | L1 | W25Q64 + LA | ⬜ |
| 014 | spi 硬件外设 + JEDEC ID | L2 | W25Q64 + LA | ⬜ |
| 015 | qspi 四线模式提速对比 | L3 | W25Q64 + LA | ⬜ |
| 016 | adc 电位器采样 | L2 | 电位器 | ⬜ |
| 017 | eeprom/flash 读写与故障注入 | L2/L3 | 24LC256 / W25Q64 | ⬜ |
| 018 | can loopback + 双板对发 | L2 | CAN 收发器（待购） | ⬜ |
| 019 | usb cdc 虚拟串口 | 框架级 | USB 线 | ⬜ |

## 第三阶段：软件架构（对应 frameworks/，规划中）

Super Loop（天然贯穿早期实验）→ Protothreads → EventOS Nano → QP-nano
→ FreeRTOS → ChibiOS → Zephyr。集成策略见 `frameworks/README.md`。

## 第四阶段：网络（规划中）

TCP/IP + MQTT，与 Linux 主机通信。F446 无以太网 MAC，需补网络模块
（W5500 SPI 以太网或 WiFi 模块），硬件待购。

## 第五阶段：综合项目（projects/，规划中）

用前四阶段能力做一个接近真实产品的完整嵌入式设备。

## 整体节奏

以动手实验为主线，穿插：软考（2027年2-4月冲刺）、408（2027年12月初试）、
PAL 平台抽象（暂停中，断点在 1.2）。
