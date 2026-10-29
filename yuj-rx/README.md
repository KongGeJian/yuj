# yuj-rx — 接收机固件（STM32F103C8T6）

CMake + arm-none-eabi-gcc 裸机固件。链路从站（TDMA 应答方）、8 通道 PWM 输出、双模供电。

| 关联文档 | 内容 |
|---|---|
| [docs/hardware.md](docs/hardware.md) | 接收机硬件详设：双模电源、LED、输出接口 |
| [docs/ai-coding/law.md](docs/ai-coding/law.md) | 模块增补规范：裸机/中断/PWM/flash |
| [../docs/protocol.md](../docs/protocol.md) | 链路协议契约 |

## 状态

阶段一开发中。原型用 Blue Pill + E01 模块（先于整机板验证链路）。
