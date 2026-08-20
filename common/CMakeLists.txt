# common：与硬件、RTOS 均无关的公共件
# 现阶段为空壳 INTERFACE 库；组件随实验需求逐步上提
add_library(common INTERFACE)
target_include_directories(common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
