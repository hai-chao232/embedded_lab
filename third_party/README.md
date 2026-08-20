# third_party/ 第三方代码

第三方代码的唯一入口。不放网上散装的 startup / CMSIS / 头文件。

## STM32CubeF4（计划）

以 git submodule 引入 ST 官方仓库——HAL、LL、CMSIS、startup、FreeRTOS 全部来自这一个来源：

```bash
git submodule add https://github.com/STMicroelectronics/STM32CubeF4.git third_party/STM32CubeF4
git submodule update --init --recursive third_party/STM32CubeF4
```

- **STM32CubeF4 官方仓库本身由多个嵌套 submodule 组成**，初始化必须 `--recursive`，
  否则 HAL/CMSIS/FreeRTOS 等嵌套组件不完整
- clone 本仓库时仍无需 `--recursive`；只有初始化 CubeF4 这一步需要递归
- **锁定 release tag**：接入时选定一个验证过的 tag 并 `git checkout`，不追 master——
  保证半年后重新 clone 环境仍可复现
- 网络不稳可改用 Gitee 镜像（`git submodule set-url` 切换）
- 许可：各组件遵循上游自身 LICENSE，详见根目录 `THIRD_PARTY_NOTICES.md`

## 状态

- [ ] 接入 STM32CubeF4 submodule（选定并锁定 release tag，验证 HAL/LL/CMSIS 完整）
