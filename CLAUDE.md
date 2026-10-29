# CLAUDE.md — YUJ（驭界）项目统筹入口

**YUJ** = DIY 多用途无线收发控制系统：多功能多载具控制终端（ESP32-S3）+ 接收机（STM32F103），2.4GHz nRF24 架构 TDMA 跳频链路。三阶段演进：基础遥控 → 电脑模拟 → 图传。

## 1. 阶段规划

| 阶段 | 交付 | 硬件 |
|---|---|---|
| 一 | 整机 PCB + 固件框架：链路（跳频/failsafe/绑定）、双主界面、模型管理、校准、全按键交互、震动+音频反馈 | 首版整机板 |
| 二 | USB 双模式（HID + CDC 上位机）、蓝牙手柄、游戏大厅首 1–2 个 + 音效 | 无新硬件 |
| 三 | 图传（独立子系统，连接器预留）、手机 Wi-Fi 显示、天气/NTP | 图传模块 |

## 2. 目录结构

```text
yuj/
├── CLAUDE.md            # 本文件：统筹入口
├── README.md            # 项目对外介绍
├── docs/
│   ├── index.md         # 文档地图
│   ├── todo.md          # 待办事项
│   ├── architecture.md  # 系统架构 + UI/交互设计
│   ├── protocol.md      # 链路协议契约（两端共同）
│   ├── parts.md         # 元器件与成品模块选型库
│   ├── pcb.md           # PCB 设计要点
│   ├── datasheet/       # 元件数据手册/规格书（文件名=型号）
│   └── ai-coding/law.md # 项目总规范
├── yuj-common/          # 链路协议公共库（纯源码，两固件共享）
│   └── docs/ai-coding/law.md
├── yuj-core/            # 遥控终端固件（ESP-IDF + LVGL）
│   └── docs/            # 硬件详设 + ai-coding/law.md
└── yuj-rx/              # 接收机固件（STM32F103, CMake）
    └── docs/            # 硬件详设 + ai-coding/law.md
```

## 3. 规范体系（动手前必读）

| 规范 | 位置 |
|---|---|
| 项目总规范（代码/架构铁律/Git/文档） | [docs/ai-coding/law.md](docs/ai-coding/law.md) |
| 协议实现纪律 | [yuj-common/docs/ai-coding/law.md](yuj-common/docs/ai-coding/law.md) |
| 遥控端增补（任务优先级/LVGL 线程安全/内存） | [yuj-core/docs/ai-coding/law.md](yuj-core/docs/ai-coding/law.md) |
| 接收机增补（裸机/中断/PWM/flash） | [yuj-rx/docs/ai-coding/law.md](yuj-rx/docs/ai-coding/law.md) |

**架构铁律（全文见总规范 §2）**：① 依赖单向 `common ← core/rx`；② 协议改动双端同步 + 版本 bump；③ HAL 不穿透。
