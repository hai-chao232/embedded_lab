# 第三方组件声明

本仓库自有代码采用 MIT License（见 `LICENSE`）。

`third_party/` 下的第三方代码**不受**本仓库 MIT License 重新许可；
各组件遵循其上游项目自身的 LICENSE。

## 当前第三方依赖

STM32CubeF4 以 Git submodule 方式接入并锁定版本。该软件包内部不同组件采用不同许可证，
不能把整个 STM32CubeF4 简化描述为单一许可证。

| 组件 | 来源 | 许可 | 当前使用情况 |
|------|------|------|--------------|
| STM32CubeF4 v1.28.3（submodule） | STMicroelectronics | 混合许可，具体见 `third_party/STM32CubeF4/LICENSE.md` | 已接入并锁定版本 |
| CMSIS Core | Arm | Apache-2.0 | 已使用 |
| STM32F4 CMSIS Device | Arm / STMicroelectronics | Apache-2.0 | 已使用；包含器件头文件、startup、`system_stm32f4xx.c` 等 |
| STM32F4 HAL / LL | STMicroelectronics | BSD-3-Clause | 后续 L2 / L3 实验按需使用 |
| STM32CubeF4 middleware | 各上游项目 / STMicroelectronics | 各组件独立许可 | 当前未使用；接入时逐项核对并补录 |

## 维护规则

1. 接入任何新的第三方组件前，先确认其上游 LICENSE。
2. 实际开始引用某个 STM32CubeF4 middleware 时，在本文件补充对应组件、版本和许可证。
3. 不把第三方代码的许可证笼统归并为本仓库的 MIT License。
4. submodule 升级版本时，重新检查上游 LICENSE / NOTICE 是否发生变化。
