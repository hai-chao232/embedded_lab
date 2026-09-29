# 实验 001：gpio_output 点灯

> 上级：[experiments/](../README.md)
>
> 封口状态：**✅ L1 核心闭环完成（2026-09-29）**

## 目的

建立“寄存器控制 GPIO”的第一层完整认知：

```text
RCC 门控时钟
  ↓
GPIO 寄存器配置
  ↓
输出数据寄存器
  ↓
PB0 物理电平
  ↓
板载 LD1
```

本实验不仅要求“灯亮”，还要求能够解释并验证：

- `GPIOB` / `RCC` 为什么能用 C 结构体指针访问真实硬件寄存器；
- Memory-Mapped I/O、基地址、寄存器 offset、`volatile` 和 Read-Modify-Write 的关系；
- C 源码如何编译成 Cortex-M4 的 `LDR / STR / BIC / ORR`；
- ELF 如何经 OpenOCD、ST-LINK、SWD 写入 MCU Flash；
- GDB 如何经 OpenOCD / ST-LINK / SWD 在真机上断点、单步并观察 MMIO 变化；
- 程序运行后关键寄存器的真实值是否与源码意图一致。
- 如何通过可控故障注入改变 `GPIOB_ODR`，观察 LD1 响应并恢复正确状态。

对应能力阶梯：**我知道底层怎么工作 → 我能自己配置和驱动**。

## Level

**L1：寄存器 / CMSIS 直接读写（RCC、GPIOB）。**

001 的可执行实现保持纯 L1；LL / HAL 只在文末做抽象层次对照，不把驱动库混进本实验 ELF。

## 前置依赖

- `platform/stm32f446ze` 已具备 startup、linker script、CMSIS device headers 和 Cortex-M4F 编译链接选项。
- 当前系统时钟保持 `system_stm32f4xx.c` 默认 HSI 路径；HSE bypass → PLL → 180 MHz 留到后续时钟实验。
- OpenOCD / ST-LINK 烧录入口随本实验正式落地：
  - `platform/stm32f446ze/openocd.cfg`
  - `scripts/flash.sh`
  - `flash_exp001_gpio_output` CMake/Ninja target

## 硬件连接

本实验**不接外部 LED、面包板或杜邦线**，直接使用 NUCLEO-F446ZE 板载用户 LED。

板载 LED 为高电平点亮（来源：UM1974 §7.5，官方资料归档见 `docs/board/NUCLEO-F446ZE/user_manual/`）。

| MCU 引脚 | 方向 | 板载对象 | 有效电平 | 备注 |
|----------|------|----------|----------|------|
| PB0 | OUT | LD1（绿） | HIGH | 默认焊桥 SB120 ON / SB119 OFF；可切至 PA5（Arduino D13） |
| PB7 | OUT | LD2（蓝） | HIGH | 本实验不用 |
| PB14 | OUT | LD3（红） | HIGH | 本实验不用 |

001 只操作 **GPIOB / PB0 / LD1**。

## 最终寄存器配置

| 目标 | 寄存器 / field | 目标值 | 含义 |
|------|----------------|--------|------|
| 打开 GPIOB 时钟 | `RCC_AHB1ENR.GPIOBEN` | `1` | GPIOB 获得 AHB1 外设时钟 |
| PB0 模式 | `GPIOB_MODER[1:0]` | `01` | General-purpose output |
| PB0 输出类型 | `GPIOB_OTYPER[0]` | `0` | Push-pull |
| PB0 输出速度 | `GPIOB_OSPEEDR[1:0]` | `00` | Low speed |
| PB0 上下拉 | `GPIOB_PUPDR[1:0]` | `00` | No pull-up / no pull-down |
| PB0 输出值 | `GPIOB_ODR[0]` | `1` | HIGH → LD1 ON |

对应地址：

| 寄存器 | 地址 |
|--------|------|
| `RCC_AHB1ENR` | `0x40023830` |
| `GPIOB_MODER` | `0x40020400` |
| `GPIOB_OTYPER` | `0x40020404` |
| `GPIOB_OSPEEDR` | `0x40020408` |
| `GPIOB_PUPDR` | `0x4002040C` |
| `GPIOB_ODR` | `0x40020414` |

### 为什么只改 PB0 field

这些寄存器由整个 GPIOB port 共享。001 使用掩码做 Read-Modify-Write，只改 PB0 对应的 bit/field，不把 PB1~PB15 的既有配置整体覆盖掉。

实机读回也验证了这一点：`MODER / OSPEEDR / PUPDR` 的其他非零位仍然保留。

## 源码主线

`main.c` 的逻辑保持刻意简单：

```text
Enable GPIOB clock
  ↓
Read back RCC enable bit（保证外设时钟使能后的同步延迟）
  ↓
PB0 MODER = Output
  ↓
PB0 OTYPER = Push-pull
  ↓
PB0 OSPEEDR = Low
  ↓
PB0 PUPDR = No pull
  ↓
PB0 ODR = HIGH
  ↓
while (1)
```

这里使用 ODR 是为了让第一次 L1 GPIO 实验直接看到“输出数据寄存器 → 引脚电平”的关系；BSRR 的原子 set/reset 优势作为后续对比知识保留，不替换本实验基线。

## 构建与烧录

### 首次配置

```bash
cmake --preset stm32f446ze
```

### 只构建

```bash
ninja -C build exp001_gpio_output
```

生成：

```text
build/experiments/001_gpio_output/exp001_gpio_output.elf
```

### 日常：构建 + 烧录

```bash
ninja -C build flash_exp001_gpio_output
```

`flash_exp001_gpio_output` 依赖 `exp001_gpio_output`：源码有变化时 Ninja 会先重新编译/链接，再调用统一烧录脚本。

底层链路：

```text
exp001_gpio_output.elf
        ↓
scripts/flash.sh
        ↓
platform/stm32f446ze/openocd.cfg
        ↓
OpenOCD（主机软件）
        ↓ USB
板载 ST-LINK/V2.1（调试/下载探针）
        ↓ SWD
STM32F446ZE
        ↓
内部 Flash
        ↓ reset
Vector Table → Reset_Handler → main()
```

### 手工等价调用

正常开发不需要手敲 OpenOCD 长命令。排查烧录基础设施时可直接调用：

```bash
scripts/flash.sh \
  platform/stm32f446ze/openocd.cfg \
  build/experiments/001_gpio_output/exp001_gpio_output.elf
```

本平台 `openocd.cfg` 再委托 OpenOCD 自带的 `board/st_nucleo_f4.cfg` 处理 ST-LINK、SWD、STM32F4 target 与 reset 配置。

## OpenOCD / ST-LINK / SWD 分工

- **OpenOCD**：运行在开发机上的软件；解析命令、控制调试探针、执行 Flash program / verify / reset / memory read 等操作。
- **ST-LINK**：NUCLEO 板上的调试/下载硬件；通过 USB 接收主机命令，再通过 SWD 接触目标 MCU。
- **SWD**：ARM Cortex-M 常用调试接口；可用于 halt/resume、读写内存/寄存器、断点以及配合 Flash 编程。
- **STM32F446ZE**：真正保存并执行程序的目标 MCU。

因此“烧录”不是 GCC 的工作：GCC/linker 先生成 ELF，OpenOCD + ST-LINK 再负责把 ELF 对应内容写进 MCU Flash。

## 验证记录（2026-09-29）

### 1. 构建与链接

以下流程成功：

```bash
cmake --preset stm32f446ze
ninja -C build exp001_gpio_output
```

链接器出现 `_close / _lseek / _read / _write` 的 `nosys/newlib-nano` 警告；001 不使用这些系统调用，因此它们不影响 GPIO 功能。后续真正需要 `printf` / UART retarget 时再实现对应底层接口，不在 001 为“消 warning”提前扩张。

### 2. ELF / 启动链检查

使用 `arm-none-eabi-size`、`arm-none-eabi-nm` 检查过最终 ELF，确认：

- `Reset_Handler`、`main` 和 linker script 关键符号存在；
- `.data` 当前为空；
- `.bss` / SRAM 区域和 `_estack` 符合 STM32F446ZE 的链接布局；
- Flash 中函数排列顺序不等于调用顺序，真正启动顺序仍由 Vector Table → Reset_Handler → main 决定。

### 3. 反汇编检查

对 `main` 执行过：

```bash
arm-none-eabi-objdump -d -M reg-names-std \
  --disassemble=main \
  build/experiments/001_gpio_output/exp001_gpio_output.elf
```

确认了关键 C 操作的真实机器指令形态：

| C 意图 | Cortex-M4 指令核心 |
|--------|-------------------|
| 读取 volatile 外设寄存器 | `LDR` |
| 清目标 bit/field | `BIC` |
| 置目标 bit | `ORR` |
| 写回 volatile 外设寄存器 | `STR` |
| `while (1)` | branch 回自身 |

同时能直接看到 `GPIO_TypeDef` 成员 offset 落成 `[base, #offset]`：例如 MODER `#0`、OTYPER `#4`、OSPEEDR `#8`、PUPDR `#12`、ODR `#20`。

### 4. ST-LINK / SWD 链路

实测环境：

```text
OpenOCD              0.11.0
USB debug probe      STMicroelectronics ST-LINK/V2.1
VID:PID              0483:374B
Target voltage       ≈ 3.25 V
Target               stm32f4x.cpu / Cortex-M4
```

OpenOCD 能成功 `reset halt`，说明主机 → USB → ST-LINK → SWD → Cortex-M4 链路打通。

### 5. 烧录

手工烧录验证得到：

```text
** Programming Finished **
** Verify Started **
** Verified OK **
** Resetting Target **
```

复位后板载绿色 **LD1 成功保持点亮**。

最终自动烧录入口也完成实机验证：

```bash
cmake --preset stm32f446ze
ninja -C build flash_exp001_gpio_output
```

`flash_exp001_gpio_output` 成功走通平台 OpenOCD 配置、通用 `flash.sh`、Programming、Verify、Reset，最终 LD1 正常点亮。

### 6. 关键寄存器读回

程序运行后 halt，通过 OpenOCD 32-bit memory read 得到：

```text
0x40023830: 00000002   RCC_AHB1ENR
0x40020400: 00000281   GPIOB_MODER
0x40020404: 00000000   GPIOB_OTYPER
0x40020408: 000000c0   GPIOB_OSPEEDR
0x4002040c: 00000100   GPIOB_PUPDR
0x40020414: 00000001   GPIOB_ODR
```

只检查 PB0 / GPIOB 本实验负责的位：

| 项目 | 预期 | 实测解释 | 结果 |
|------|------|----------|:----:|
| `GPIOBEN` | bit1 = 1 | `AHB1ENR = 0x2` | ✅ |
| `MODER0` | `01` | `0x281` 的 bits[1:0] = `01` | ✅ |
| `OT0` | `0` | `OTYPER` bit0 = 0 | ✅ |
| `OSPEEDR0` | `00` | `0xC0` 的 bits[1:0] = `00` | ✅ |
| `PUPDR0` | `00` | `0x100` 的 bits[1:0] = `00` | ✅ |
| `ODR0` | `1` | `ODR = 0x1` | ✅ |
| 物理结果 | LD1 ON | LD1 实际点亮 | ✅ |

`MODER = 0x281`、`OSPEEDR = 0xC0`、`PUPDR = 0x100` 而不是简单的 `1/0/0`，反而证明了代码只修改 PB0 field，没有粗暴覆盖 GPIOB 其他 pin 的既有配置。

### 7. GDB / SWD 运行时调试

本次直接使用 `arm-none-eabi-gdb` 连接 OpenOCD 的 GDB server（`localhost:3333`），没有依赖 VS Code 图形界面。Cortex-Debug 后续只是把同一条 GDB → OpenOCD → ST-LINK → SWD 链路自动化，不作为 001 额外重复验收项。

已实机完成：

- `break main` 成功使用 Cortex-M4 hardware breakpoint；
- `continue`、`stepi`、CPU register 查看和 MMIO memory read 均正常；
- RCC 时钟使能前 `RCC_AHB1ENR = 0x00000000`；执行 `ORR #2` 后仅 CPU `r3 = 0x2`，外设寄存器仍为 `0`；执行随后的 `STR` 后 `RCC_AHB1ENR = 0x00000002`；
- `GPIOB_MODER` 初值 `0x00000280`，执行 PB0 mode 的 `ORR #1` 后 CPU 中先得到 `0x281`，直到 `STR` 后外设寄存器才变为 `0x00000281`；
- 在本次最终 ELF 中，ODR 写入前断在 `0x080002A6`：执行 `ORR #1` 后 `r3 = 1`、`GPIOB_ODR` 仍为 `0`；再执行 `STR` 后 `GPIOB_ODR = 1`，同时观察到 LD1 由灭变亮；
- `main()` 最终进入自身 branch 的无限循环，验证初始化代码已经完整执行。

这组结果把“CPU register 中算出新值”和“通过 `STR` 真正写入 Memory-Mapped 外设寄存器”明确区分开。

## 实机与可选仪器验证

### 本次封口已经完成

- 板载 LD1 肉眼状态：**亮**。
- ST-LINK / OpenOCD：烧录 verify 通过、CPU 可 halt、关键寄存器可读回并与预期一致。

### 可选的后续测量练习（本次未伪造记录）

如果以后想把“可测量”再做得更物理化，可继续补充：

- **万用表**：PB0 对 GND，亮灯时预期接近 3.3 V；记录真实读数后再写入本 README / artifacts。
- **示波器 / LA1010**：把程序改成闪烁版后测 PB0 方波；这已经超出“恒亮点灯”核心封口，不作为当前完成状态的虚构证据。

本次没有实际采集万用表读数或 LA/示波器波形，因此仓库不填写不存在的测量值或截图。

## 故障注入

### 已执行：直接改写 GPIOB_ODR

程序停在 `while (1)` 后，通过 GDB 直接改写正在工作的 MMIO 寄存器：

```gdb
x/wx 0x40020414
set {unsigned int}0x40020414 = 0x00000000
x/wx 0x40020414
set {unsigned int}0x40020414 = 0x00000001
x/wx 0x40020414
```

实测结果：

```text
GPIOB_ODR = 0x1  → LD1 亮
GPIOB_ODR = 0x0  → LD1 灭
GPIOB_ODR = 0x1  → LD1 重新亮
```

过程中没有重新编译、重新烧录或重新执行 `main()`；变化来自 GDB → OpenOCD → ST-LINK → SWD 对运行中硬件寄存器的直接改写。这构成 001 的最小“制造故障 → 观察 → 恢复”闭环。

### 其余可复现练习（本次未执行，不标成实测）

1. **不开 RCC 时钟**
   - 制造：注释 `GPIOBEN` 使能。
   - 预期：GPIOB 配置不能按预期生效，LD1 不亮。
   - 定位：读 `RCC_AHB1ENR`，确认 bit1。
   - 恢复：重新使能 GPIOB clock。

2. **把 PB0 配成 Input**
   - 制造：让 `MODER[1:0] = 00`，仍写 ODR。
   - 预期：ODR 可以保存值，但 PB0 不作为 push-pull GPIO 输出驱动 LD1。
   - 定位：读 `GPIOB_MODER[1:0]`。
   - 恢复：改回 `01`。

3. **整体赋值破坏共享寄存器（只做代码审查或谨慎实验）**
   - 错误示例：`GPIOB->MODER = 1U;`
   - 风险：除 PB0 外，PB3/PB4 等既有 field 也会被覆盖。
   - 001 的实机读回 `MODER = 0x281` 已直观说明为什么应使用掩码 RMW。

4. **板级连接变化（可选）**
   - 若实际改动 SB120/SB119 把 LD1 切到 PA5，PB0 软件保持不变时 LED 行为会改变。
   - 用于理解“MCU 引脚配置正确”不等于“PCB 板级连接一定匹配”。

## L1 / LL / HAL 层次对比

001 的 ELF 只使用 L1。下面只对照“相同意图在更高抽象层大致长什么样”，不在本实验中同时链接三套实现。

| 意图 | L1（本实验） | LL | HAL |
|------|--------------|----|-----|
| GPIOB 时钟 | `RCC->AHB1ENR ...` | `LL_AHB1_GRP1_EnableClock(...)` | `__HAL_RCC_GPIOB_CLK_ENABLE()` |
| PB0 输出模式 | 直接改 `MODER/OTYPER/OSPEEDR/PUPDR` | `LL_GPIO_SetPinMode/...` | `GPIO_InitTypeDef` + `HAL_GPIO_Init()` |
| 输出 HIGH | 直接改 `ODR`（后续可对比 BSRR） | `LL_GPIO_SetOutputPin()` | `HAL_GPIO_WritePin()` |

L2/L3 的价值不是改变 GPIO 硬件本质，而是把本实验手动完成的“地址、field、掩码、初始化组合”封装成更高层 API。

## 本实验形成的知识闭环

到 001 封口时，应能解释以下链条：

```text
stm32f4xx.h
 ↓
stm32f446xx.h
 ↓
GPIO_TypeDef / RCC_TypeDef
 ↓
GPIOB / RCC 指针宏
 ↓
Memory-Mapped I/O：base + offset
 ↓
volatile 寄存器访问
 ↓
LDR / BIC / ORR / STR
 ↓
AHB1 / GPIOB 硬件寄存器
 ↓
PB0 push-pull HIGH
 ↓
LD1 ON
```

以及下载链：

```text
C/C++ source
 ↓
CMake / Ninja
 ↓
arm-none-eabi-gcc + linker
 ↓
ELF
 ↓
OpenOCD
 ↓
ST-LINK
 ↓
SWD
 ↓
STM32F446ZE Flash
 ↓
Reset_Handler
 ↓
main()
```

## 封口结论

001 的 L1 核心验收已经通过：

- [x] 确认板载 LD1 实际连接为 PB0，高电平有效。
- [x] CMSIS 寄存器级代码完成，不依赖 HAL/LL。
- [x] GPIOB clock、PB0 mode/type/speed/pull/output 均有明确配置意图。
- [x] CMake/Ninja 构建和 ELF 链接成功。
- [x] 用 `nm/size/objdump` 把 ELF、启动链和机器指令与源码连接起来。
- [x] OpenOCD 能通过板载 ST-LINK/V2.1 + SWD 控制目标 MCU。
- [x] Flash program + verify + reset 成功。
- [x] LD1 实机点亮。
- [x] 关键寄存器实机读回与预期完全一致。
- [x] GDB 通过 OpenOCD 完成 hardware breakpoint、`continue`、`stepi`、CPU register / MMIO 观察。
- [x] 逐指令确认 `ORR` 只改变 CPU 中间值，最终 `STR` 才真正改变 RCC / GPIO 外设寄存器；ODR 写入后 LD1 同步点亮。
- [x] 已执行最小故障注入：GDB 直接改写 `GPIOB_ODR`，LD1 实测 `亮 → 灭 → 亮` 并恢复。
- [x] OpenOCD 烧录流程已从手工长命令沉淀为平台配置 + 通用脚本 + `flash_<target>`，最终自动入口已在真机验证。
- [x] 明确记录本次没有做的万用表/LA 扩展测量，不伪造证据。

至此实验 001 正式作为后续 GPIO input、EXTI、Timer 等实验的 **L1 GPIO 输出基线**。

## 关联

- 408 组成原理：Memory-Mapped I/O；STM32 外设寄存器位于统一地址空间。
- CMSIS：device header、`GPIO_TypeDef` / `RCC_TypeDef`、`__IO` / `volatile`。
- Cortex-M4：load/store、Thumb-2、函数栈帧、向量表与 Reset_Handler。
- OpenOCD / ST-LINK / SWD：烧录与调试链路。
- PAL / Driver：以后封装 GPIO Driver 时回到这里，对比“裸寄存器事实”与软件抽象。
