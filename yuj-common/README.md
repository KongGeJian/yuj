# 链路协议公共库（yuj-common）

两端固件（yuj-core / yuj-rx）共享的协议实现：帧定义与编解码、FHSS 跳频序列、通道字段压缩、`PROTOCOL_VERSION`。**协议契约单一真源：[docs/protocol.md](../docs/protocol.md)**。

| 项 | 约定 |
|---|---|
| 形态 | 纯源码库（头文件 + 少量 .c），无独立构建产物 |
| 引用 | core/rx 源码级相对路径引入各自 CMake/IDF 工程 |
| 平台 | 零平台依赖 → 桌面 Host 可编译跑单测 |
| 版本 | `PROTOCOL_VERSION` 宏，两端构建期断言一致 |

增补规范：[docs/ai-coding/law.md](docs/ai-coding/law.md)。
