# platform/stm32f446ze：NUCLEO-F446ZE 板级粘合

# ---------------------------------------------------------------------------
# Library
# 语法：add_Library(<name> [STATIC|SHARED|MODULE|INTERFACE])
# 创建一个叫 platform_stm32f446ze 的库，类型是 INTERFACE：
#                                          1.不会编译生成.a或.so文件。 
#                                          2.只是一个属性合集，包含include路径、宏定义、编译选项等。 
#                                          3.任何依赖它的target都会继承这些属性
#
# 语法：set(<variable> <value> ...)
# 用来定义变量，后面可以通过 ${<variable>} 引用该变量
# ---------------------------------------------------------------------------
add_library(platform_stm32f446ze INTERFACE)
 
set(STM32CUBE_F4_DIR
    ${PROJECT_SOURCE_DIR}/third_party/STM32CubeF4
)

set(STM32F4_CMSIS_DEVICE_DIR
    ${STM32CUBE_F4_DIR}/Drivers/CMSIS/Device/ST/STM32F4xx
)

# ---------------------------------------------------------------------------
# CMSIS / device headers
# 语法：target_include_directories(<target> [INTERFACE|PUBLIC|PRIVATE] <dirs> ...)
# 给目标添加头文件路径。INTERFACE（表示这些路径只会传递给依赖它的目标，因为库本身不编译）
# ---------------------------------------------------------------------------

target_include_directories(
    platform_stm32f446ze
    INTERFACE

    ${CMAKE_CURRENT_SOURCE_DIR}

    ${STM32CUBE_F4_DIR}/Drivers/CMSIS/Include
    ${STM32F4_CMSIS_DEVICE_DIR}/Include
)

# ---------------------------------------------------------------------------
# MCU selection / board clock facts
# 语法：target_compile_definitions(<target> [INTERFACE|PUBLIC|PRIVATE] <definitions> ...)
# 给目标添加编译宏，相当于 -D。INTERFACE（同理）
# ---------------------------------------------------------------------------

target_compile_definitions(
    platform_stm32f446ze
    INTERFACE

    STM32F446xx
    HSE_VALUE=8000000U
)

# ---------------------------------------------------------------------------
# Startup / CMSIS system source
# 语法：target_sources(<target> [INTERFACE|PUBLIC|PRIVATE] <sources> ...)
# 给目标添加源码文件。INTERFACE（同理）
# ---------------------------------------------------------------------------

target_sources(
    platform_stm32f446ze
    INTERFACE

    ${STM32F4_CMSIS_DEVICE_DIR}/Source/Templates/system_stm32f4xx.c
    ${STM32F4_CMSIS_DEVICE_DIR}/Source/Templates/gcc/startup_stm32f446xx.s
)

# ---------------------------------------------------------------------------
# Cortex-M4F architecture
# 语法：target_compile_options(<target> [INTERFACE|PUBLIC|PRIVATE] <options>...)
# 给目标添加编译器选项。INTERFACE（同理）
#                        
# 语法：target_link_options(<target> [INTERFACE|PUBLIC|PRIVATE] <options>...)
# 给目标添加链接器选项。INTERFACE（同理）
# ---------------------------------------------------------------------------

target_compile_options(
    platform_stm32f446ze
    INTERFACE

    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard

    -ffunction-sections
    -fdata-sections
)

target_link_options(
    platform_stm32f446ze
    INTERFACE

    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard

    --specs=nano.specs
    --specs=nosys.specs

    -Wl,--gc-sections
)

# ---------------------------------------------------------------------------
# Linker script
# ---------------------------------------------------------------------------

set(STM32F446ZE_LINKER_SCRIPT
    ${CMAKE_CURRENT_SOURCE_DIR}/linker/STM32F446ZE.ld
)

target_link_options(
    platform_stm32f446ze
    INTERFACE

    -T${STM32F446ZE_LINKER_SCRIPT}
)
