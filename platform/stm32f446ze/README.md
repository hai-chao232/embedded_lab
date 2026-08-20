# platform/stm32f446ze/ 板级粘合层

**每块板只有一份**，所有实验共享。定位：vendor 代码（third_party）与实验之间的粘合。

## 规划内容

| 模块 | 说明 |
|------|------|
| linker script | STM32F446ZE 内存布局（512K Flash / 128K SRAM） |
| startup 选择 | 从 CubeF4 选 startup_stm32f446xx.s（gcc 版） |
| SystemClock 配置 | 180MHz HSE/PLL 初始化，时钟树文档化 |
| printf 重定向 | retarget 到板载 ST-Link 虚拟串口（USART3, PD8/PD9） |
| board init | LED/按键引脚定义等板级封装 |
| openocd 配置 | st_nucleo_f4 适配、烧录脚本对接 |

## 状态

待 STM32CubeF4 submodule 接入后落地。
