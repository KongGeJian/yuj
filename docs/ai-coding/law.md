# 项目总规范 - law.md

> 各模块增补规范见 `yuj-<x>/docs/ai-coding/law.md`（只写增补，不重复本文）。

## 1. 代码规范

### 1.1 语言与工具

| 项 | 规定 |
|---|---|
| 语言 | C11（两端统一） |
| 格式化 | clang-format 统一，仓库根 `.clang-format`，提交前格式化 |
| 编译 | `-Wall -Wextra -Werror`，警告即错误 |

### 1.2 命名

| 对象 | 约定 | 示例 |
|---|---|---|
| yuj-common 公共符号 | `yuj_` 前缀 + snake_case | `yuj_encode_ctrl_frame()` |
| yuj-core / yuj-rx 符号 | `core_` / `rx_` 前缀 | `core_radio_send()` |
| 类型 | `_t` 后缀 | `yuj_frame_type_t` |
| 宏/常量 | 全大写 | `YUJ_PROTOCOL_VERSION` |
| 文件 | 与模块职责同名 | `radio_hal.c` / `link_sched.c` |

### 1.3 错误处理

| 环境 | 风格 |
|---|---|
| yuj-core（ESP-IDF） | `esp_err_t` + `ESP_RETURN_ON_ERROR` |
| yuj-rx（裸机） | `RX_OK(0)` / `RX_ERR_xxx(-n)` |
| 中断上下文 | 永不阻塞、不打印，只置标志/写环形缓冲 |

### 1.4 测试纪律

| 层 | 要求 |
|---|---|
| yuj-common | 纯逻辑 Host 桌面单测（帧编解码/跳频/CRC），零平台依赖 |
| 模块固件 | 关键服务（链路调度/failsafe/校准）单测用可注入替身；private 方法不放宽可见性（反射/接缝） |
| 提交前 | 相关单测全绿才可 commit |

## 2. 架构铁律（违者 PR 拒收）

1. **依赖单向**：`yuj-common ← yuj-core / yuj-rx`；common 禁止 include 任何模块文件与平台头（esp_*/stm32f1xx_*）。
2. **协议双端同步**：帧格式改动必须双端同改并 bump `PROTOCOL_VERSION`，两端构建期断言一致。
3. **HAL 不穿透**：射频/显示/ADC/IO/音频/IMU 皆有 `*_hal.h` 抽象，上层不得直接 include 驱动头或操作寄存器。
4. **扩展不侵入**：游戏一模块一 .c 注册制；换射频模块只改 Radio HAL 参数表，不动遥控核心逻辑。
5. **共享方式**：core/rx 以源码级相对路径引用 `../yuj-common`，不复制、不包管理。

## 3. Git 规范

### 3.1 分支与 Commit

| 项 | 规定 |
|---|---|
| 分支 | `main`（稳定可烧录）/ `feature/<scope>-<desc>` / `fix/<scope>-<desc>` |
| Commit | `type(scope): 一行描述`；type = feat/fix/refactor/docs/test/chore/hw(硬件)；scope = common/core/rx/docs/pcb |
| 节奏 | 一个最小可验证改动（测试+实现）一次 commit，不攒大包 |

### 3.2 PR 检查单

- [ ] 单测全绿，`-Werror` 无警告
- [ ] 动 `yuj-common/` → core + rx 双端构建通过（CI 强制）
- [ ] 动帧格式 → 双端同改 + `PROTOCOL_VERSION` bump
- [ ] 动硬件设计 → `docs/parts.md` / `docs/pcb.md` / 模块文档同步
- [ ] 新公共符号带 `yuj_` 前缀且头文件有注释契约
- [ ] 无铁律违规（依赖方向 / HAL 穿透）

## 4. 版本规范

| 版本 | 归属 | 规则 |
|---|---|---|
| `PROTOCOL_VERSION` | 链路协议（yuj-common 定义） | 独立演进，双端构建期断言一致；纪律见 [protocol.md](../protocol.md) §7 |
| 模块版本 | yuj-core / yuj-common / yuj-rx **各自独立** | **语义化 x.y.z**：不兼容改动 x+1，新增功能 y+1，修复 z+1 |

- **协议版本与模块版本互不绑定**：协议不常改，模块迭代频繁，各自独立 bump。
- 模块版本定义于各自构建工程（IDF / CMake project VERSION），系统信息页展示。

## 5. 文档规范

### 5.1 写作原则

| 原则 | 要求 |
|---|---|
| 章节服务决策 | 只写决策信息（背景/口径/改动/依赖/风险），能从代码推导的不写 |
| 手段适配 | 对比/清单→表格，并列→列表，层级→缩进，流程→图，接口/命令→代码片段；文字是最后手段 |
| 重点凸显 | 触发条件、零改动、新增项、⚠ 待核对用**加粗**标出，其余克制 |
| 篇幅克制 | 10 秒抓住全貌：开头一句话+表格；每多一段话，注意力稀释一分 |

### 5.2 统一格式

| 项 | 规定 |
|---|---|
| 标题编号 | 数字分级：`1.` → `1.1` → `1.1.1`（H2 起） |
| 防冗余 | **一个事实只在一处定义（单一真源）**，他处链接指向；README/index 等总领文档例外 |
| 信息密度 | 不失准确前提下尽量压缩；结构化表达优先于长句 |

### 5.3 文档结构与变更同步

| 位置 | 放什么 |
|---|---|
| 根 `README.md` / `CLAUDE.md` | 对外介绍 / 统筹入口（总领例外） |
| `docs/` | 跨模块：架构/UI、协议、选型、PCB、todo、datasheet、总规范 |
| `yuj-<x>/docs/` | 模块独有设计 + `ai-coding/law.md` 增补规范 |

| 改动 | 必须同步 |
|---|---|
| 协议帧格式 | `docs/protocol.md` + 双端代码 + 版本 bump（同一 PR） |
| 元件/模块选型 | `docs/parts.md`（数据手册入 `docs/datasheet/`） |
| PCB 决策 | `docs/pcb.md`（单一真源，含两板详设） |
| 新增待办 | `docs/todo.md`（完成勾选，不删行） |
