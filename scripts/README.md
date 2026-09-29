# scripts/ 辅助脚本

> 上级：[根 README](../README.md)

烧录、调试、仪器辅助的自动化入口。脚本不依赖实验具体内容：通过参数传 elf / 串口设备等，不写死实验路径。

## 规划内容

| 脚本 | 用途 | 状态 |
|------|------|:----:|
| flash.sh | OpenOCD 烧录指定 ELF（program + verify + reset） | ✅ |
| debug.sh / gdb 初始化 | 接 VS Code Cortex-Debug 调试 | ⬜ |
| 逻辑分析仪波形导出辅助 | LA1010 导出自动化 | ⬜ |
| 串口监听辅助 | 板载 ST-Link 虚拟串口（USART3 PD8/PD9） | ⬜ |

## flash.sh

通用调用形式：

```bash
scripts/flash.sh <openocd-config> <firmware.elf>
```

例如实验 001 的底层等价调用为：

```bash
scripts/flash.sh \
  platform/stm32f446ze/openocd.cfg \
  build/experiments/001_gpio_output/exp001_gpio_output.elf
```

日常不需要直接调用脚本。实验 CMake target 已把平台配置与 ELF 路径绑定好，优先使用：

```bash
ninja -C build flash_exp001_gpio_output
```

该 target 依赖对应 ELF，因此源码有变化时会先自动重新构建，再执行烧录。

## 状态（2026-09-29）

- ✅ `flash.sh`：随实验 001 落地。
- ✅ `platform/stm32f446ze/openocd.cfg`：板载 ST-LINK / SWD / STM32F4 OpenOCD 入口已落地。
- ⬜ 交互式 GDB / Cortex-Debug 自动化留到后续调试工作流，不在 001 中提前扩张。
