# experiments/ 实验目录

## 结构

每个实验一个目录，命名 `0XX_<topic>`（编号与清单见根目录 `ROADMAP.md`）。

```
STM32CubeF4 ──▶ platform/common ──▶ Experiment001 / 002 / 003 …
```

公共 MCU 基础设施只有一份（third_party + platform + common）。
**每个实验只保留真正属于自己的文件**：

- `main.c`
- 实验配置（CMakeLists.txt / 私有头文件）
- `README.md`（模板见 `docs/experiment_template.md`）
- 必要的私有源文件

## 规则

- 新建实验：先复制 `docs/experiment_template.md` 为 README.md，**先写"目的"与"仪器验证步骤"再写代码**
- README 的"仪器验证"与"故障注入"是必填项
- 顶层 CMakeLists 中显式登记新实验（不用 glob，长期维护可控）
- 实验里出现第二次的通用代码，提炼到 `common/`，不留在实验里复制
