# embedded-lab 学习路线与实验清单

> 活文档：随学习进度调整。编号只表示顺序；Level 记录在本表与各实验 README 中。
> 本仓库只记录平台技术进展；个人年度学习计划（软考/408/PAL）不在这里维护。

## Level 约定

| Level | 手段 | 说明 |
|:-----:|------|------|
| L1 | 寄存器 / CMSIS | 直接操作寄存器，理解硬件 |
| L2 | STM32 LL | 厂商底层抽象 |
| L3 | HAL | 快速工程开发 |

- 同一外设尽量按 L1 → L2 → L3 递进；一个实验可以混合层次，在 README 里写清哪部分用哪个。
- 例外：USB 直接从框架级开始（TinyUSB / ST USB Device），不手搓 Chapter 9。

## 第一阶段：MCU 底层机制（14 次）

> 顺序设计：DMA 先作为独立能力理解（008），再应用到 ADC/UART（010/014）；
> UART 走 轮询 → 中断 → ring buffer → DMA 四步。
> 007 输入捕获不用信号发生器：第二块 F446ZE 输出 PWM，第一块捕获，LA1010 同时测两块。

| 编号 | 实验 | Level | 依赖 | 硬件 | 状态 |
|:----:|------|:-----:|------|------|:----:|
| 001 | gpio_output 点灯 | L1（对比 L2/L3） | 平台就绪 | LED | ⬜ |
| 002 | gpio_input 按键轮询 | L1 | 001 | 按键 | ⬜ |
| 003 | exti 外部中断 | L1 | 002 | 按键 + LED | ⬜ |
| 004 | nvic 优先级与抢占 | L1 | 003 | 按键 + LED | ⬜ |
| 005 | timer 基本定时中断 | L1 | 004 | LED | ⬜ |
| 006 | timer PWM 呼吸灯 | L2 | 005 | LED + LA | ⬜ |
| 007 | timer 输入捕获测频 | L2 | 006 | 板2 PWM 输出 + LA | ⬜ |
| 008 | dma mem2mem 搬运 | L1 | 005 | LA | ⬜ |
| 009 | adc 电位器采样 | L2 | 005 | 电位器 | ⬜ |
| 010 | adc + timer 触发 + 环形 DMA | L2/L3 | 008, 009 | 电位器 + LA | ⬜ |
| 011 | uart 轮询收发 | L1 | 004 | CH340 | ⬜ |
| 012 | uart 中断收发 | L1 | 011 | CH340 | ⬜ |
| 013 | uart ring buffer | L1/L2 | 012 | CH340 | ⬜ |
| 014 | uart DMA 收发 | L2 | 008, 013 | CH340 | ⬜ |

## 第二阶段：通信能力（草案，编号可能调整）

| 编号 | 实验 | Level | 硬件 | 状态 |
|:----:|------|:-----:|------|:----:|
| 015 | i2c bit-bang 软件模拟 | L1 | 24LC256 + LA | ⬜ |
| 016 | i2c 硬件外设 | L2 | 24LC256 + LA | ⬜ |
| 017 | spi 寄存器级 | L1 | W25Q64 + LA | ⬜ |
| 018 | spi 硬件外设 + JEDEC ID | L2 | W25Q64 + LA | ⬜ |
| 019 | qspi 四线模式提速对比 | L3 | W25Q64 + LA | ⬜ |
| 020 | eeprom/flash 读写与故障注入 | L2/L3 | 24LC256 / W25Q64 | ⬜ |
| 021 | can loopback + 双板对发 | L2 | CAN 收发器（待购） | ⬜ |
| 022 | usb cdc 虚拟串口 | 框架级 | USB 线 | ⬜ |

## 第三阶段：软件架构（对应 frameworks/，规划中）

Super Loop（天然贯穿早期实验）→ Protothreads → EventOS Nano → QP-nano
→ FreeRTOS → ChibiOS → Zephyr。边界与集成策略见 `frameworks/README.md`。

## 第四阶段：网络（规划中）

TCP/IP + MQTT，与 Linux 主机通信。F446 无以太网 MAC，需补网络模块
（W5500 SPI 以太网或 WiFi 模块），硬件待购。

## 第五阶段：综合项目（projects/，规划中）

用前四阶段能力做一个接近真实产品的完整嵌入式设备。
