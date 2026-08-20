# platform/stm32f446ze/ 板级粘合层

**每块板只有一份**，所有实验共享。定位：vendor 代码（third_party）与实验之间的粘合。
**只负责硬件平台，不知道应用用哪个 RTOS**（RTOS 专属移植放 `frameworks/<fw>/port/`）。

## 规划内容

| 模块 | 说明 |
|------|------|
| clock | ST-LINK MCO 8 MHz → **HSE bypass** → PLL → SYSCLK 180 MHz（板载无独立 HSE 晶振） |
| startup | 从 CubeF4 选 startup_stm32f446xx.s（gcc 版） |
| irq | NVIC 配置、中断向量管理（内核部分知识归 `docs/cortex_m/`） |
| board | LED/按键引脚定义等板级封装 |
| linker script | STM32F446ZE 内存布局（512K Flash / 128K SRAM） |
| printf 重定向 | retarget 到板载 ST-Link 虚拟串口（USART3，PD8/PD9） |
| openocd | st_nucleo_f4 适配、烧录脚本对接 |

> 时钟来源从第一天记正确：以后研究 RCC 时会碰到 **HSE crystal 与 HSE bypass 的区别**。

## 状态

待 STM32CubeF4 submodule 接入后落地。
