cmake_minimum_required(VERSION 3.20)

project(embedded-lab C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# 公共设施与板级平台：所有实验共享，只有一份
add_subdirectory(common)
add_subdirectory(platform)

# 实验登记：显式列表，不用 glob；新增实验在这里加一行
set(EXPERIMENTS
  # 001_gpio_output
  # 002_gpio_input
)

foreach(exp IN LISTS EXPERIMENTS)
  add_subdirectory(experiments/${exp})
endforeach()
