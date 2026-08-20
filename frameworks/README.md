# frameworks/ 软件架构框架

第三阶段（软件架构）的主战场。集成策略因框架而异：

| 框架 | 集成方式 | 状态 |
|------|----------|:----:|
| Super Loop | 无需集成（早期实验的天然形态） | — |
| Protothreads | 头文件即库，直接进 build | ⬜ |
| EventOS Nano | 源码库 | ⬜ |
| QP-nano | 源码库 | ⬜ |
| FreeRTOS | 用 STM32CubeF4 自带的 Middlewares/Third_Party/FreeRTOS | ⬜ |
| ChibiOS | 独立构建体系（整体替换，非库），到阶段单独评估 | ⬜ |
| Zephyr | 独立 west + devicetree 体系，需自己 SDK，计划放仓库外独立 workspace | ⬜ |

## 原则

框架实验与 `experiments/` 联动：实验代码是"应用层"，框架适配做在 platform/common，
避免每个实验复制一份框架。
