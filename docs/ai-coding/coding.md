# 代码规范与架构铁律

## 一、语言与工具

| 项 | 规定 |
|---|---|
| 语言 | **C11**（两端统一；不用 C++ 例外） |
| 格式化 | **clang-format 统一**，仓库根放 `.clang-format`，提交前格式化 |
| 静态检查 | 编译开启 `-Wall -Wextra -Werror`（警告即错误） |

## 二、命名约定

| 对象 | 约定 | 示例 |
|---|---|---|
| yuj-common 公共符号 | `yuj_` 前缀 + snake_case | `yuj_encode_ctrl_frame()` |
| yuj-core 符号 | `core_` 前缀 | `core_radio_send()` |
| yuj-rx 符号 | `rx_` 前缀 | `rx_pwm_update()` |
| 类型 | `_t` 后缀 | `yuj_frame_type_t` |
| 宏/常量 | 全大写 | `YUJ_PROTOCOL_VERSION` |
| 文件 | 与模块职责同名 | `radio_hal.c` / `link_sched.c` |

## 三、错误处理

| 环境 | 风格 |
|---|---|
| yuj-core（ESP-IDF） | `esp_err_t` 返回值 + `ESP_RETURN_ON_ERROR` 宏 |
| yuj-rx（裸机） | 负数 errno 风格返回值，`RX_OK(0) / RX_ERR_xxx(-1..)` |
| 中断上下文 | **永不阻塞、不打印**，只置标志/写环形缓冲 |

## 四、架构铁律（违者 PR 拒收）

1. **依赖单向**：`yuj-common ← yuj-core / yuj-rx`。common **不得** include 任何模块文件、任何平台头（esp_xx.h / stm32f1xx.h 都禁止）。
2. **协议双端同步**：帧格式改动必须双端同改并 bump `PROTOCOL_VERSION`；两端构建期断言版本一致。
3. **HAL 不穿透**：射频/显示/ADC/IO/音频/IMU 皆有抽象层（`*_hal.h` 接口）。上层应用/服务**不得**直接 include 驱动头或操作外设寄存器。
4. **扩展不侵入**：游戏一模块一 .c 注册制；新射频模块只改 Radio HAL 参数表——均不得改动遥控核心逻辑。
5. **共享方式**：core/rx 以源码级相对路径引用 `../yuj-common`，不复制、不包管理。

## 五、测试纪律

| 层 | 要求 |
|---|---|
| yuj-common | **纯逻辑可 Host 测试**（帧编解码/跳频序列/CRC），无平台依赖，桌面编译跑单测 |
| 模块固件 | 关键服务（链路调度/failsafe/校准）写可注入替身的单测；private 方法不放宽可见性，用反射/接缝 |
| 提交前 | 相关单测全绿才可 commit |
