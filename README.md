# embedded-lab

一个长期、统一、可逐步扩展的个人嵌入式实验平台。

> 不是"买几块开发板、学几个协议"，而是用同一套硬件体系系统练习
> MCU 底层机制、通信协议、事件驱动框架、RTOS、网络协议与调试分析能力，
> 最终独立完成一个接近真实产品形态的嵌入式系统。
>
> 板子可以换，但实验体系、学习方法、代码结构、测试思路一直保留。

## 能力阶梯

我知道它为什么存在 → 我知道底层怎么工作 → 我能自己配置和驱动
→ 我能用仪器验证它 → 我能制造故障并定位 → 我能把它放进不同的软件架构
→ 我知道什么时候应该用它

## 实验四原则

所有实验尽量做到：**可观察、可测量、可故障注入、可复现**。

"程序跑通"只是及格线。重点是：为什么成功？逻辑分析仪看到什么？
寄存器是什么状态？中断什么时候发生？异常时怎么应对？

## 开发层次纪律

| Level | 手段 | 目的 |
|:-----:|------|------|
| L1 | 寄存器 / CMSIS | 理解硬件 |
| L2 | STM32 LL | 理解厂商驱动抽象 |
| L3 | HAL | 快速工程开发 |
| — | RTOS / Framework | 架构能力 |

同一外设从寄存器到 HAL 逐层递进，这样以后看到 HAL，你知道它下面在干什么。
例外：USB 直接从框架级开始（TinyUSB / ST USB Device），不手搓 Chapter 9。

## 目录结构

```
embedded-lab/
├── cmake/            # 交叉编译工具链定义
├── docs/             # 板卡 / MCU / Cortex-M 手册与笔记、协议波形、实验模板
├── third_party/      # 第三方代码唯一入口（STM32CubeF4，submodule）
├── platform/         # 板级粘合层（每块板只有一份：时钟、linker、printf 重定向…）
├── experiments/      # 实验（每个只含 main.c、配置、README、必要源文件）
├── frameworks/       # 软件架构框架集成（protothreads … zephyr）
├── common/           # 与硬件无关的可复用件（环形缓冲、CLI、日志…）
├── scripts/          # 烧录、调试、仪器辅助脚本
└── projects/         # 综合项目（区别于单点实验）
```

核心原则：**公共 MCU 基础设施只有一份**
（STM32CubeF4 → platform/common → experiments），
每个实验只保留真正属于自己的文件，杜绝几十个实验各复制一坨 HAL。

## 快速开始

工具链：arm-none-eabi-gcc（ARM GNU Toolchain 15.3）、cmake ≥ 3.20、ninja、openocd + ST-Link。

```bash
cmake -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi.cmake
ninja -C build <experiment_target>   # 例如 exp001_gpio_output
```

## 路线与状态

- 学习路线与实验清单：`ROADMAP.md`
- 每个实验的 README 模板：`docs/experiment_template.md`
- 立项背景文档：`docs/planning/`
- 当前状态：仓库骨架已建；STM32CubeF4 submodule 待接入（见 `third_party/README.md`）
