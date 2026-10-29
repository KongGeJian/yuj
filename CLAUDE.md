# CLAUDE.md — YUJ（驭界）项目统筹入口

**YUJ** = DIY 多用途无线收发控制系统：一个多功能多载具控制终端（遥控器，ESP32-S3）+ 一个接收机（STM32F103），2.4GHz nRF24 架构 TDMA 跳频链路。三阶段演进：基础遥控 → 电脑模拟 → 图传。

## 阶段规划

| 阶段 | 交付 | 硬件 |
|---|---|---|
| 一 | 整机 PCB + 固件框架：链路（跳频/failsafe/绑定）、双主界面、模型管理、校准、全按键交互、震动+音频反馈 | 首版整机板（全部已定器件） |
| 二 | USB 双模式（HID 手柄 + CDC 上位机）、蓝牙手柄、游戏大厅首 1–2 个游戏 + 音效 | 无新硬件 |
| 三 | 图传（独立子系统，结构/连接器预留）、手机 Wi-Fi 显示、天气/NTP | 图传模块 + 连接器 |

## 目录结构

```text
yuj/
├── CLAUDE.md            # 本文件：统筹入口（概览 + 规范索引）
├── README.md            # 项目对外介绍
├── docs/                # 跨模块文档
│   ├── index.md         # 文档地图
│   ├── todo.md          # 待办事项
│   ├── architecture.md  # 系统架构 + UI/交互设计
│   ├── protocol.md      # 链路协议契约（两端共同）
│   ├── parts.md         # 元器件与成品模块选型库
│   ├── pcb.md           # PCB 设计要点
│   └── ai-coding/       # 项目总规范（coding/git/docs/hardware）
├── yuj-common/          # 链路协议公共库（纯源码，两固件共享）
│   └── docs/ai-coding/law.md
├── yuj-core/            # 遥控终端固件（ESP-IDF + LVGL）
│   └── docs/            # 遥控端独有设计 + ai-coding/law.md
└── yuj-rx/              # 接收机固件（STM32F103, CMake）
    └── docs/            # 接收机独有设计 + ai-coding/law.md
```

## 规范体系（动手前必读）

| 规范 | 位置 | 管什么 |
|---|---|---|
| 代码规范 + 架构铁律 | [docs/ai-coding/coding.md](docs/ai-coding/coding.md) | C11/命名/格式化/依赖方向/HAL 纪律 |
| Git 规范 | [docs/ai-coding/git.md](docs/ai-coding/git.md) | 分支/commit/PR 检查单 |
| 文档规范 | [docs/ai-coding/docs.md](docs/ai-coding/docs.md) | 写作风格与文档结构 |
| 硬件规范 | [docs/ai-coding/hardware.md](docs/ai-coding/hardware.md) | 采购渠道/封装核对/安全 |
| 协议实现纪律 | [yuj-common/docs/ai-coding/law.md](yuj-common/docs/ai-coding/law.md) | 公共库增补 |
| 遥控端增补 | [yuj-core/docs/ai-coding/law.md](yuj-core/docs/ai-coding/law.md) | 任务优先级/LVGL 线程安全/内存 |
| 接收机增补 | [yuj-rx/docs/ai-coding/law.md](yuj-rx/docs/ai-coding/law.md) | 裸机/中断/PWM/flash 保存 |

## 架构铁律（摘要，全文见 coding.md）

1. **依赖单向**：`yuj-common ← yuj-core / yuj-rx`，common 不得反向引用任何模块文件。
2. **协议双端同步**：帧格式任何改动必须双端同改并 bump `PROTOCOL_VERSION`；构建期断言版本一致。
3. **HAL 不穿透**：射频/显示/ADC/IO 皆有抽象层，上层应用不得直接操作外设寄存器/驱动句柄。
