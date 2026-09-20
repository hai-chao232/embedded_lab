# third_party/ 第三方代码唯一入口

> 上级：[根 README](../README.md)

所有第三方代码都经这里接入，**不放网上散装的 startup / CMSIS / 头文件**。
许可证声明见根目录 `THIRD_PARTY_NOTICES.md`。

## 为什么用 Git submodule

STM32CubeF4 体积大且上游独立维护；submodule 让本仓库只记录"用哪个版本"，
不把几万行 vendor 代码混进自己的提交历史。

## STM32CubeF4（已接入，锁定 v1.28.3）

以 git submodule 引入 ST 官方仓库——HAL、LL、CMSIS、startup、FreeRTOS 全部来自这一个来源：

```bash
git submodule add https://github.com/STMicroelectronics/STM32CubeF4.git third_party/STM32CubeF4
git submodule update --init --recursive third_party/STM32CubeF4
git -C third_party/STM32CubeF4 checkout v1.28.3   # 锁定 release tag
```

- **STM32CubeF4 官方仓库本身由多个嵌套 submodule 组成**，初始化必须 `--recursive`，
  否则 HAL/CMSIS/FreeRTOS 等嵌套组件不完整
- clone 本仓库时无需 `--recursive`；只有初始化 CubeF4 这一步需要递归
- **锁定 release tag**：接入时选定一个验证过的 tag 并 `git checkout`，不追 master——
  保证半年后重新 clone 环境仍可复现
- 网络不稳可改用 Gitee 镜像（`git submodule set-url` 切换）
- 许可：各组件遵循上游自身 LICENSE，详见根目录 `THIRD_PARTY_NOTICES.md`

## 升级流程

切换 tag → 全量构建验证 → 复查上游 LICENSE 并更新 `THIRD_PARTY_NOTICES.md` → 提交

## 规则

- 不直接修改 submodule 内文件；确需补丁时先讨论策略

## 状态

✅ 已接入 v1.28.3（2026-08）
