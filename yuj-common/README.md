# yuj-common — 链路协议公共库

两端固件（yuj-core / yuj-rx）共享的协议实现：帧定义与编解码、FHSS 跳频序列、通道字段压缩、`PROTOCOL_VERSION`。**协议契约见 [docs/protocol.md](../docs/protocol.md)**。

## 性质与使用

| 项 | 约定 |
|---|---|
| 形态 | 纯源码库（头文件 + 少量 .c），无独立构建产物 |
| 引用 | core/rx 以源码级相对路径引入各自 CMake/IDF 工程 |
| 平台 | **零平台依赖**：禁止 include esp/stm32 任何头 → 可在桌面 Host 编译跑单测 |
| 版本 | `PROTOCOL_VERSION` 宏，两端构建期断言一致 |

## 增补规范

见 [docs/ai-coding/law.md](docs/ai-coding/law.md)。
