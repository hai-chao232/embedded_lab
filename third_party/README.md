# third_party/ 第三方代码

第三方代码的唯一入口。不放网上散装的 startup / CMSIS / 头文件。

## STM32CubeF4（计划）

以 git submodule 引入 ST 官方仓库——HAL、LL、CMSIS、startup、FreeRTOS 全部来自这一个来源：

```bash
git submodule add https://github.com/STMicroelectronics/STM32CubeF4.git third_party/STM32CubeF4
```

- 仓库较大（数百 MB 历史），按需拉取；clone 主仓库时**不要** `--recursive`
- 网络不稳可改用 Gitee 镜像（`git submodule set-url` 切换）
- 附带提供 `Middlewares/Third_Party/FreeRTOS`，frameworks/freertos 直接用它
- 许可：BSD-3-Clause，与本仓库 MIT 兼容

## 状态

- [ ] 接入 STM32CubeF4 submodule
