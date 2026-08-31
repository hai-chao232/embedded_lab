# embedded-lab 学习路线与实验清单

> 活文档：随学习进度调整。编号只表示学习顺序；Level 记录在本表与各实验 README 中。
> 本仓库只记录平台技术进展；个人年度学习计划（软考 / 408 / PAL）不在这里维护。

本路线同时推进两条能力线：

- **硬件机制线**：通过实验逐步理解 MCU、外设、总线、中断、DMA、通信协议和 RTOS。
- **工程能力线**：逐步完善构建、调试、产物分析、可复现环境、CI、静态分析和发布流程。

原则是：**工程能力在“有真实问题可解释时”引入，不为了基础设施本身暂停实验主线。**

---

## Level 约定

| Level | 手段 | 说明 |
|:-----:|------|------|
| L1 | 寄存器 / CMSIS | 直接操作寄存器，理解硬件 |
| L2 | STM32 LL | 厂商底层抽象 |
| L3 | HAL | 快速工程开发 |

- 同一外设尽量按 L1 → L2 → L3 递进；一个实验可以混合层次，在 README 里写清哪部分用哪个。
- 例外：USB 直接从框架级开始（TinyUSB / ST USB Device），不手搓 Chapter 9。

---

## P0：仓库与裸机构建基线 ✅

P0 的目标不是“做完整工程系统”，而是建立一个**最小、可理解、可继续扩展的裸机仓库基线**。

已完成：

- [x] 仓库目录骨架确定，不再做架构级调整。
- [x] CMake + Ninja 构建链路跑通。
- [x] GNU Arm Embedded Toolchain 可用于 `arm-none-eabi` 交叉编译。
- [x] STM32CubeF4 v1.28.3 通过 Git submodule 接入并锁定版本。
- [x] STM32F446ZE CMSIS / startup / `system_stm32f4xx.c` 接入。
- [x] STM32F446ZE linker script 接入。
- [x] Cortex-M4F 基础编译 / 链接参数生效。
- [x] `001_gpio_output` 最小程序可生成 ELF。
- [x] 已使用 `nm` / `size` 等工具初步分析最小 ELF。
- [x] 已建立第三方许可、实验模板和实验依据归档规则。

P0 **有意不包含**以下工程增强，它们被明确安排到 E1，而不是遗忘：

- `-Wall / -Wextra / -Wpedantic`
- linker `.map`
- post-build `.bin / .hex`
- 自动 `size`
- Debug / Release 构建模型
- toolchain 版本检查

完成本节后进入实验 001，不再继续扩建仓库骨架。

---

## 第一阶段：MCU 底层机制（14 次）

> 顺序设计：
>
> - 001 首次完成“真实 GPIO → 烧录 → SWD/GDB → 仪器/寄存器验证”，随后执行 E1。
> - 004 完成后、005 Timer 开始前，完成平台时钟 checkpoint。
> - DMA 先作为独立能力理解（008），再应用到 ADC / UART（010 / 014）。
> - UART 走“轮询 → 中断 → ring buffer → DMA”四步。
> - 007 输入捕获不用信号发生器：第二块 F446ZE 输出 PWM，第一块捕获，LA1010 同时测两块。

| 编号 | 实验 | Level | 依赖 | 硬件 | 状态 |
|:----:|------|:-----:|------|------|:----:|
| 001 | gpio_output 点灯 | L1（对比 L2/L3） | P0 | 板载 LED + 调试器 / 万用表或 LA | ⬜ |
| 002 | gpio_input 按键轮询 | L1 | 001 + E1 | 按键 | ⬜ |
| 003 | exti 外部中断 | L1 | 002 | 按键 + LED | ⬜ |
| 004 | nvic 优先级与抢占 | L1 | 003 | 按键 + LED | ⬜ |
| 005 | timer 基本定时中断 | L1 | 004 + Clock checkpoint | LED | ⬜ |
| 006 | timer PWM 呼吸灯 | L2 | 005 | LED + LA | ⬜ |
| 007 | timer 输入捕获测频 | L2 | 006 | 板2 PWM 输出 + LA | ⬜ |
| 008 | dma mem2mem 搬运 | L1 | 005 | 调试器 | ⬜ |
| 009 | adc 电位器采样 | L2 | 005 | 电位器 | ⬜ |
| 010 | adc + timer 触发 + 环形 DMA | L2/L3 | 008, 009 | 电位器 + LA | ⬜ |
| 011 | uart 轮询收发 | L1 | 004 | CH340 | ⬜ |
| 012 | uart 中断收发 | L1 | 011 | CH340 | ⬜ |
| 013 | uart ring buffer | L1/L2 | 012 | CH340 | ⬜ |
| 014 | uart DMA 收发 | L2 | 008, 013 | CH340 | ⬜ |

### 001 的阶段目标

001 不只是“点亮 LED”。它负责第一次完整跑通：

1. 根据 Datasheet / Reference Manual / 开发板手册确认 GPIO 与板载 LED。
2. 理解 RCC 和 GPIO 的必要寄存器。
3. 使用 CMSIS 寄存器方式完成 GPIO 输出。
4. 编译、链接并分析固件。
5. 使用 OpenOCD / ST-LINK 烧录。
6. 使用 SWD/GDB / Cortex-Debug 查看寄存器、单步和断点。
7. 用 LED、电平测量或逻辑分析仪验证真实硬件行为。
8. 主动制造至少一个 GPIO 配置故障并完成定位。
9. 在理解 L1 后，对比 LL / HAL 的实现方式与抽象代价。

---

## E1：工程基线 v1（001 完成后、002 开始前）

E1 是 P0 的工程化补全点。此时已经有真实可运行 firmware，每项增强都必须结合 001 的实际产物理解。

### E1.1 编译告警基线

先统一启用：

```text
-Wall
-Wextra
-Wpedantic
```

原则：

- 先理解并清理自己代码中的 warning。
- 暂不默认启用 `-Werror`。
- 后续更严格 warning 在 E2 再评估，不一次性堆满 GCC 选项。

### E1.2 固件产物与链接分析

构建后自动得到并理解：

- `.elf`：调试和符号信息的核心产物。
- `.map`：分析 section、symbol、object file 和链接来源。
- `.bin`：裸二进制部署格式。
- `.hex`：带地址信息的 Intel HEX 部署格式。
- `arm-none-eabi-size`：每次构建观察 Flash / RAM 占用。

重点把 `.map` 和已经学过的 ELF / linker script / `nm` 串起来，而不是只生成文件。

### E1.3 Debug / Release 构建模型

建立明确的 Debug / Release preset 或等价配置，理解：

- 调试信息 `-g`
- `-Og` 与调试体验
- `-O0 / -Og / -O2 / -Os` 对汇编、代码尺寸和调试行为的影响
- `NDEBUG` 等 Release 语义

### E1.4 Toolchain 环境检查

目标是**验证环境，而不是把本机 `/opt/...` 绝对路径硬编码进仓库**。

至少做到：

- configure 时确认 `arm-none-eabi-gcc` 可找到。
- 输出 / 检查编译器版本。
- README 记录仓库验证过的 GNU Arm Toolchain 版本（当前基线：15.3）。
- 对明显不符合预期的版本给出清晰 warning 或 fail-fast 策略。

真正的“精确版本可复现”放到 E2。

---

## Clock checkpoint：004 → 005

在 Timer 实验开始之前完成平台时钟基线：

- 理解 HSI / HSE / PLL / AHB / APB 时钟树。
- 明确 NUCLEO-F446ZE 当前外部时钟来源与板级连接。
- 配置目标系统时钟：HSE bypass → PLL → 180 MHz（如最终平台方案保持该目标）。
- 正确配置 Flash latency、总线分频和相关电源 / 时钟约束。
- 更新 `SystemCoreClock`。
- 用可观测手段验证实际频率，而不是只相信寄存器配置。
- 特别理解 APB prescaler 与 Timer clock 倍频规则。

Timer / PWM / Capture 之后都以此时钟基线为前提。

---

## E2：第一阶段工程收尾（014 完成后）

E2 的目标是从“在当前开发机上能稳定构建”升级到“仓库能够持续、可重复地验证”。

计划内容：

- 精确化 GNU Arm Toolchain 版本策略，研究可复现安装 / pinning。
- 新机器从零构建的 bootstrap 流程。
- CI 至少执行 configure + build，覆盖第一阶段实验。
- 评估在 CI 对自有代码启用 `-Werror`。
- 逐步评估更严格 warning，例如 `-Wshadow`、`-Wundef`、`-Wconversion` 等，而不是机械全开。
- 引入合适的静态分析（如 clang-tidy / cppcheck，具体方案届时根据代码结构决定）。
- 检查不同 Debug / Release 配置的构建稳定性。
- 形成第一版“环境、构建、调试、产物分析”可复现文档。

E2 完成后，第一阶段不仅外设知识闭环，工程基线也完成一次升级。

---

## 第二阶段：通信能力（草案，编号可能调整）

| 编号 | 实验 | Level | 硬件 | 状态 |
|:----:|------|:-----:|------|:----:|
| 015 | i2c bit-bang 软件模拟 | L1 | 24LC256 + LA | ⬜ |
| 016 | i2c 硬件外设 | L2 | 24LC256 + LA | ⬜ |
| 017 | spi 寄存器级 | L1 | W25Q64 + LA | ⬜ |
| 018 | spi 硬件外设 + JEDEC ID | L2 | W25Q64 + LA | ⬜ |
| 019 | qspi 四线模式提速对比 | L3 | W25Q64 + LA | ⬜ |
| 020 | eeprom / flash 读写与故障注入 | L2/L3 | 24LC256 / W25Q64 | ⬜ |
| 021 | can loopback + 双板对发 | L2 | CAN 收发器（待购） | ⬜ |
| 022 | usb cdc 虚拟串口 | 框架级 | USB 线 | ⬜ |

第二阶段同时继续完善：

- 自动化烧录 / 调试命令。
- 协议波形与关键测试证据归档。
- 通信错误与恢复路径的故障注入。
- 必要的 host-side 辅助脚本。

---

## 第三阶段：软件架构（对应 `frameworks/`，规划中）

Super Loop（天然贯穿早期实验）→ Protothreads → EventOS Nano → QP-nano
→ FreeRTOS → ChibiOS → Zephyr。

边界与集成策略见 `frameworks/README.md`。

这一阶段的工程能力重点逐步转向：

- 模块边界与接口。
- host-side test。
- 静态分析。
- RTOS 调试与运行时观测。
- 不同 framework 的构建隔离与可重复集成。

---

## 第四阶段：网络（规划中）

TCP/IP + MQTT，与 Linux 主机通信。

STM32F446 本身无以太网 MAC，需要补网络模块（例如 W5500 SPI 以太网或 Wi-Fi 模块），硬件届时确定。

重点加入：

- 网络抓包。
- 超时 / 断线 / 重连故障注入。
- 长时间运行稳定性测试。
- 主机端测试工具与日志分析。

---

## 第五阶段：综合项目（`projects/`，规划中）

用前四阶段能力完成一个接近真实产品的完整嵌入式设备。

最终工程能力目标包括：

- Debug / Release 正式发布流程。
- 固件版本信息。
- 构建产物归档。
- Flash / RAM size budget。
- 自动化构建与基本测试。
- 第三方依赖与许可证复核。
- 可复现的发布说明。
