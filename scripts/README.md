# scripts/ 辅助脚本

> 上级：[根 README](../README.md)

烧录、调试、仪器辅助的自动化入口。脚本不依赖实验具体内容：通过参数传 elf / 串口设备等，不写死路径。

## 规划内容

| 脚本 | 用途 | 状态 |
|------|------|:----:|
| flash.sh | openocd 烧录指定实验的 elf | ⬜ |
| debug.sh / gdb 初始化 | 接 VS Code Cortex-Debug 调试 | ⬜ |
| 逻辑分析仪波形导出辅助 | LA1010 导出自动化 | ⬜ |
| 串口监听辅助 | 板载 ST-Link 虚拟串口（USART3 PD8/PD9） | ⬜ |

## 状态

`platform/stm32f446ze` 已落地（2026-08-31）。烧录脚本（openocd 适配 + flash.sh）是实验 001
阶段目标"OpenOCD / ST-LINK 烧录"的前置，随 001 一起落地。
