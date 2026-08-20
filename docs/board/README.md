# docs/board/ 板卡资料

```
NUCLEO-F446ZE/
├── schematic/     # 原理图
├── user_manual/   # 用户手册
└── notes/         # 学习笔记（可链 Obsidian）
```

板级关键信息速记（001 实验前按原理图核对）：

- MCU：STM32F446ZET6，Cortex-M4F @180MHz，512KB Flash / 128KB SRAM
- 板载 ST-Link V2-1：调试 + 虚拟串口（USART3，PD8/PD9）
- 板载 LED：LD1（PB0）/ LD2（PB7）/ LD3（PB14）；按键：B1（PC13，USER）
