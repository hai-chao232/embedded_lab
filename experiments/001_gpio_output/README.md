# 实验 001：gpio_output 点灯

> 上级：[experiments/](../README.md)

## 目的

建立"寄存器控制 GPIO"的第一层认知：**RCC 门控时钟 → 模式配置 → 输出电平**。
对应能力阶梯的"我知道底层怎么工作"，并作为后续 L2/L3 对比的 L1 基线。

## Level

L1 寄存器 / CMSIS 直接读写（RCC、GPIOB）。
本实验以 L1 为主；完成寄存器实现和验证后，再对比 LL / HAL 的实现方式与抽象层次。

## 前置依赖

- 平台就绪：platform/stm32f446ze 已落地（startup / linker / 编译选项，见 `platform/stm32f446ze/README.md`）
- 当前时钟：默认 HSI 16 MHz（HSE bypass → 180 MHz 留待后续实验）

## 硬件连接

板载用户 LED，高电平点亮（来源：UM1974 §7.5，`docs/board/NUCLEO-F446ZE/user_manual/`）。

| MCU 引脚 | 信号 | 外设 | 备注 |
|----------|------|------|------|
| PB0 | OUT | LD1（绿） | 默认焊桥 SB120 ON / SB119 OFF；可切至 PA5（Arduino D13） |
| PB7 | OUT | LD2（蓝） | 高电平点亮 |
| PB14 | OUT | LD3（红） | 高电平点亮 |

## 寄存器 / 外设关注点

- `RCC_AHB1ENR`：GPIOBEN（bit 1）——GPIOB 挂在 AHB1 总线上，**不先开时钟，GPIO 寄存器写不进去**（本实验第一个认知点）
- `GPIOB_MODER`：每引脚 2 bit；00=输入、01=输出、10=复用、11=模拟
- `GPIOB_ODR` / `GPIOB_BSRR`：L1 先用 ODR 直观理解；BSRR 的原子 set/reset（无读-改-写竞争）留作对比

## 构建与烧录

```bash
cmake --preset stm32f446ze
ninja -C build exp001_gpio_output
# 烧录命令（openocd 配置待落地）
```

## 仪器验证步骤

- **万用表**（DC 电压档）：红笔接 LED 靠 MCU 侧（或直接测 PB0），黑笔接 GND
  - 灯亮：≈ 3.3 V；灯灭：≈ 0 V
- **示波器 / LA1010**（闪烁版，推荐）：接 PB0，预期方波，周期 = 代码延时 × 2
- 实测截图、串口日志存入本实验 `artifacts/`

## 故障注入

1. **不开 RCC 时钟**（新手第一大坑）：注释掉 AHB1ENR 使能 → 写 MODER/ODR 无效，灯不亮。
   定位：读回 `RCC_AHB1ENR` 确认 bit1 是否为 1。恢复：补上使能。
2. **模式配错**：MODER 配成 00（输入）却写 ODR → 灯不亮，写入"成功"但电平出不来。
   定位：读回 `GPIOB_MODER` 确认位域值。
3. （可选）焊桥 SB120 切至 PA5 → 观察 PA5 与 PB0 行为差异，理解板级硬件配置对软件的影响。

## 实验记录

（待实验完成后填写：波形截图、实测电压值、遇到的问题与结论）

## 关联

- 408 组成原理：内存映射 I/O（STM32 外设寄存器即内存映射）vs 端口映射 I/O
- PAL：后续实验用 Driver 层封装 GPIO 时，回到这里对比 L1 直写与抽象层
