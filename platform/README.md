# platform/ 板级粘合层

> 上级：[根 README](../README.md)

vendor 代码（`third_party/`）与实验之间的板级粘合。**每块板只有一份**，所有实验共享：
时钟、linker、板级封装、printf 重定向、openocd 适配。

## 三层边界（各管各的）

- `common/`：**既不认识 MCU，也不认识 RTOS**
- `platform/<board>/`：**只负责硬件平台，不知道应用用哪个 RTOS**（RTOS 移植放 `frameworks/<fw>/port/`）
- `frameworks/`：框架专属配置与移植

## 平台列表

| 目录 | 板卡 | 状态 |
|------|------|:----:|
| `stm32f446ze/` | NUCLEO-F446ZE | 🟡 基础已落地（startup / linker / 编译选项），详见其 README |

## 规则

- 新增板卡：`platform/<board>/` 新目录 + 顶层 `CMakeLists.txt` 登记
- 板级信息（引脚、时钟来源）以 `docs/board/` 官方资料为准
