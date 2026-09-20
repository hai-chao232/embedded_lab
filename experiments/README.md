# experiments/ 实验目录

> 上级：[根 README](../README.md)

## 结构

每个实验一个目录，命名 `0XX_<topic>`（编号与清单见根目录 `ROADMAP.md`）。

```
STM32CubeF4 ──▶ platform/common ──▶ Experiment001 / 002 / 003 …
```

公共 MCU 基础设施只有一份（third_party + platform + common）。
**每个实验只保留真正属于自己的文件**：

```
0XX_topic/
├── README.md        # 模板见 docs/experiment_template.md
├── CMakeLists.txt   # 实验自己的构建配置
├── main.c
└── artifacts/       # 本实验的波形截图、串口日志——与实验待在一起
    ├── normal_400khz.png
    ├── serial.log
    └── raw/         # 大体积原始采样 / 仪器工程，.gitignore 忽略不入库
```

一个实验的"代码 + 接线 + 波形 + 故障 + 结论"永远待在一起；
通用协议知识笔记放 `docs/protocols/`，不属于任何单个实验。

## 规则

- 新建实验：先复制模板写 README——"目的"与"仪器验证步骤"先于代码
- README 的"仪器验证"与"故障注入"是必填项
- 顶层 CMakeLists 中显式登记新实验（不用 glob，长期维护可控）
- 实验里第二次出现的通用代码，提炼到 `common/`，不留在实验里复制
