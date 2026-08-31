add_executable(
    exp001_gpio_output 
    main.c
)

target_link_libraries(
    exp001_gpio_output 
    PRIVATE 
    platform_stm32f446ze
)

set_target_properties(
    exp001_gpio_output 
    PROPERTIES 
    SUFFIX ".elf"
)