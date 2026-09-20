# docs/ 文档与资料

> 上级：[根 README](../README.md)

手册 PDF、知识笔记、实验模板、立项文档的唯一归档地。**知识放这里，实验产物放各实验自己的 `artifacts/`。**

## 子目录

| 目录 | 内容 | 归档边界 |
|------|------|----------|
| `board/` | 板卡资料：原理图、用户手册、板级速记 | 板载外设、引脚、跳线 |
| `mcu/` | MCU 资料：Datasheet、Reference Manual（RM0390） | 外设归 MCU |
| `cortex_m/` | 内核资料：用户指南、编程手册 | NVIC / SysTick 归内核 |
| `protocols/` | 协议知识笔记（i2c / spi / can…） | 通用知识，不绑定实验 |
| `planning/` | 立项文档（历史档案，不再更新） | — |

其他文件：

- `experiment_template.md` —— 新建实验的 README 模板（"仪器验证"与"故障注入"为必填项）

## 归档规则

- 手册 PDF 保留官方原文件名（如 RM0390、UM1974、PM0214），不重命名
- 每个手册目录配 `notes/` 放学习笔记（可链 Obsidian），手册本身不动
- `notes/` 等占位目录用 `.gitkeep` 保留，内容随实验进度填充
- 板卡与 MCU 新增资料照 `board/`、`mcu/` 的子目录模式归档
