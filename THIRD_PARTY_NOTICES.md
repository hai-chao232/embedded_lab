# 第三方组件声明

本仓库自有代码采用 MIT License（见 LICENSE）。

`third_party/` 下的第三方代码**不受**本仓库 MIT License 重新许可，
各组件遵循其上游项目自身的 LICENSE。

| 组件 | 来源 | 许可 | 备注 |
|------|------|------|------|
| STM32CubeF4 v1.28.3（submodule） | STMicroelectronics | 见其仓库 LICENSE；HAL/CMSIS 等为 BSD-3-Clause，部分 middleware 组件各有独立许可 | 当前平台仅用 CMSIS/startup/system；实际引用组件随平台落地逐一核对补录 |

**规则**：接入任何新第三方组件时，必须在本表登记许可信息。
