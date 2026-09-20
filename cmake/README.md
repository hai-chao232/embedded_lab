# cmake/ 工具链配置

> 上级：[根 README](../README.md)

交叉编译与构建模型的定义入口，只放**工具链级** CMake 配置；业务构建逻辑在各目录自己的 CMakeLists 中。

## 与构建入口的分工

| 文件 | 职责 |
|------|------|
| `CMakeLists.txt`（根） | 构建根入口：登记实验列表、组织 common / platform / experiments |
| `CMakePresets.json` | preset 入口：VS Code CMake Tools 与命令行共用同一套构建逻辑 |
| `cmake/arm-none-eabi.cmake` | 交叉编译工具链文件：编译器 / binutils 定位、裸机 System Name、交叉查找路径规则 |

MCU 宏、FPU 与链接选项等**板级**参数在 `platform/<board>/` 内定义，不进工具链文件。

## 规则

- 新增工具链 / 平台时：在 `cmake/` 增加对应 toolchain 文件，并在 `CMakePresets.json` 配套 preset
- 不把本机 `/opt/...` 绝对路径写进仓库（E1.4 会做 toolchain 版本检查，见 `ROADMAP.md`）

## 状态

- ✅ arm-none-eabi.cmake：Cortex-M4F 编译链接链路已跑通
- ⬜ E1 的构建增强（Debug / Release preset、.map / .bin / .hex 产物、自动 size、toolchain 版本检查）——落地位置就是本目录与根 CMakeLists，见 `ROADMAP.md` E1
