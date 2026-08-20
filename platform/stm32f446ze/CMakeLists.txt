# platform/stm32f446ze：NUCLEO-F446ZE 板级粘合
# 现阶段为空壳 INTERFACE 库；待 CubeF4 接入后落地
# （clock / startup / irq / board / linker / printf / openocd）
add_library(platform_stm32f446ze INTERFACE)
target_include_directories(platform_stm32f446ze INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
