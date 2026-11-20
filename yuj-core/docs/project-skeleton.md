# 工程骨架 - project-skeleton.md

> 一句话：**先立骨架、后填模块**——构建方式/组件边界/配置基线/任务优先级一次定死，模块内部设计另文讨论。
> 环境安装见 [getting-started.md](getting-started.md)（单一真源，本文不重复）。

## 1. 版本基线（兼容性锁定）

| 组件 | 版本 | 依据 | 兼容性约束 |
|---|---|---|---|
| ESP-IDF | **v6.1.1** | 官方文档 `.../esp-idf/zh_CN/v6.1.1/esp32s3/` 存在；[datasheet §ESP32-S3](../../docs/datasheet/ESP32-S3资料.md) 已指向 | — |
| LVGL | **9.6.0** | 官方组件仓库最新稳定版（9.6.0~1 为补丁） | esp_lvgl_port 要求 `>=8,<10` ✅ |
| esp_lvgl_port | **2.9.0** | 仓库最新版 | 要求 IDF `>=5.2`、LVGL `>=8,<10`；**上界未声明** ⚠ |
| ST7789 panel driver | **vendor 官方示例驱动**（`components/hal_display/src/st7789/`） | 组件仓库只有第三方实现（jbrilha 1.0.2 等，均未声明 IDF 6.x 兼容）；屏为自定义板，BSP 不适用 | 换屏只改命令表 = HAL 隔离 |
| ESP32-S3 模组 | **N16R8**：16MB Flash / 8MB OPI PSRAM | [parts.md](../../docs/parts.md) 已到货 | LVGL 缓冲走 PSRAM；SPI DMA 对 PSRAM 的对齐/能力见 §5 ⚠ |

**已知风险与退路**

| 风险 | 退路 |
|---|---|
| esp_lvgl_port 2.9.0 在 IDF 6.1.1 下编不过（上界未声明） | 它只封装 task + `lv_lock` + tick；退化为自写适配（约 30 行），LVGL 版本不变 |
| LVGL 9.6 与 esp_lcd 颜色格式/字节序差异 | RGB565 直通；F 段用屏实测校正，不提前引入转换层 |

## 2. 工程结构

```text
yuj-core/
├── CMakeLists.txt              # project(yuj-core VERSION 0.1.0) + EXTRA_COMPONENT_DIRS=components
├── sdkconfig.defaults          # 配置基线（§5）
├── partitions.csv              # OTA×2 3MB / NVS 96K / assets 9M / coredump 64K
├── components/
│   ├── yuj-common/             # 3 行 shim：源码指到 ../../../yuj-common，不复制
│   ├── board/                  # include/board.h：引脚表唯一真源
│   ├── hal_radio/  hal_display/  hal_input/
│   ├── svc_link/   svc_storage/
│   └── app_ui/
├── main/                       # 只有 app_main.c：初始化顺序 + 创建任务
└── docs/
```

**一模块一 component（P0 只建上表 6 个 + shim），依赖靠 `REQUIRES` 编译期强制**——铁律①③从"评审靠人"变成"违者编译失败"。

| component | REQUIRES（传递） | PRIV_REQUIRES（不外泄） |
|---|---|---|
| `board` | — | `esp_driver_gpio` |
| `hal_radio` | `board` | `esp_driver_spi`, `esp_driver_gpio` |
| `hal_display` | `board` | `esp_lcd`, `esp_driver_spi` |
| `svc_link` | `board`, `hal_radio`, `yuj-common` | — |
| `svc_storage` | `yuj-common` | `nvs_flash` |
| `app_ui` | `svc_link`, `svc_storage` | `esp_lvgl_port` |

约定：每组件 `include/`（公共头，进 `INCLUDE_DIRS`）+ `src/`（私有，不进）→ **跨模块引私有头直接编译失败**。

## 3. 构建与 yuj-common 接线

顶层：

```cmake
set(EXTRA_COMPONENT_DIRS "${CMAKE_CURRENT_LIST_DIR}/components")
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(yuj-core VERSION 0.1.0)     # 模块版本独立演进（law §4）
```

`components/yuj-common/CMakeLists.txt`（shim，不复制源码，铁律⑤）：

```cmake
set(COMMON_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../yuj-common")
idf_component_register(
    SRCS "${COMMON_DIR}/src/"            # 空目录时 SRCS 置空
    INCLUDE_DIRS "${COMMON_DIR}/include")
```

`yuj-common/CMakeLists.txt` 自身做成双端通吃：

```cmake
if(ESP_PLATFORM)
    idf_component_register(SRCS src/... INCLUDE_DIRS include)   # core 经 shim 引用
else()
    add_library(yuj-common STATIC src/...)                       # rx / Host 单测
    target_include_directories(yuj-common PUBLIC include)
endif()
```

**协议版本双端断言**（铁律②）：common 定义 `YUJ_PROTOCOL_VERSION`；core 在 `svc_link` 内：

```c
#define CORE_EXPECTED_PROTOCOL_VERSION 1
_Static_assert(YUJ_PROTOCOL_VERSION == CORE_EXPECTED_PROTOCOL_VERSION,
               "yuj-common 协议版本已变更：双端同改后再 bump 本值");
```

## 4. 配置基线

### 4.1 `sdkconfig.defaults`

| 键 | 值 | 理由 |
|---|---|---|
| `CONFIG_IDF_TARGET` | `esp32s3` | 锁定目标 |
| `CONFIG_ESPTOOLPY_FLASHSIZE` | `16MB` | N16R8 |
| `CONFIG_SPIRAM` / `MODE_OCT` | `y` | 8MB OPI PSRAM |
| `CONFIG_FREERTOS_HZ` | `1000` | 4ms 链路节拍需 1ms 粒度 |
| `CONFIG_PARTITION_TABLE_CUSTOM` | `partitions.csv` | 见 4.2 |
| `CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG` | `y` | 板载原生 USB，无串口芯片 |
| `CONFIG_LOG_DEFAULT_LEVEL` | `INFO` | 实时路径代码内禁打印（core law §2） |
| `-Werror` | 顶层 `idf_build_set_property(COMPILE_OPTIONS "-Werror" APPEND)` | law §1.1 |

### 4.2 `partitions.csv`

```text
# Name,     Type, SubType,  Offset, Size,  Flags
nvs,        data, nvs,      ,       96K,
otadata,    data, ota,      ,       8K,
phy_init,   data, phy,      ,       4K,
ota_0,      app,  ota_0,    ,       3M,
ota_1,      app,  ota_1,    ,       3M,
assets,     data, spiffs,   ,       9M,          # LittleFS 挂 data/spiffs 子类型
coredump,   data, coredump, ,       64K,
```

合计 ≈15.2MB < 16MB ✅；与 [architecture §3.1](../../docs/architecture.md) 一致。

## 5. 启动顺序与任务骨架

```text
app_main: nvs_flash_init → board_init → hal_*_init（display 后置） 
          → svc_*_init → 创建任务 → esp_timer 起 4ms 节拍 → main 挂起
```

| 任务 | 优先级 | 周期/触发 | 栈/内存 |
|---|---|---|---|
| `link_sched` | **最高** | 4ms 硬节拍（业务任务 + `vTaskDelayUntil`，**不用 esp_timer 回调**——回调跑在 timer 任务，优先级不可控） | 内部 RAM |
| `input` | 中 | ≤4ms，与链路同拍（ADS7828/TCA9555 轮询） | 内部 RAM |
| `ui` | 低 | LVGL，持 `lv_lock` | PSRAM 缓冲 |
| 其余 | 最低 | 阶段二三（音频/WiFi/BLE） | — |

ISR 只置标志/写环形缓冲，处理放下半部（core law §2）。

## 6. HAL 契约（先定接口、后填实现）

| 头文件 | 职责 | 本阶段 |
|---|---|---|
| `radio_hal.h` | nRF24 收发/信道/IRQ 回调 | stub 返回 `ESP_ERR_NOT_SUPPORTED` |
| `display_hal.h` | esp_lcd panel + LVGL flush | stub |
| `input_hal.h` | 摇杆/旋钮/按键采样 | stub |
| `audio_hal.h` / `imu_hal.h` | 阶段二三 | 不建 |

规则：上层只 include `*_hal.h`；HAL 只 include `board.h`；驱动头一律 `PRIV_REQUIRES`（不穿透）。

## 7. 质量线

| 项 | 做法 |
|---|---|
| 格式化 | 仓库根补 `.clang-format`（C11 基线），提交前格式化 |
| Host 单测 | `yuj-common` 用 plain CMake + CTest，**不依赖 IDF**（CI 直接跑） |
| 模块单测 | IDF 按组件 `test/`；替身注入点在 HAL 边界 |
| CI | 动 `yuj-common` → core + rx 双端构建（law §3.2） |

## 8. 里程碑

| 段 | 内容 | 验收 |
|---|---|---|
| A 环境 | 按 [getting-started.md](getting-started.md) 装 EIM + IDF v6.1.1；补 `.clang-format` | `idf.py --version` 显示 v6.1.1 |
| B 骨架 | 目录/CMake/6 个 P0 组件 + common shim + 版本断言 | `idf.py build` 通过 |
| C 配置 | `sdkconfig.defaults` + `partitions.csv` | 编译产物分区表正确；PSRAM 初始化日志 |
| D 最小固件 | `app_main` + 心跳日志 | 能烧则烧；无板以编译+体积报告为准 |
| E 架构骨架 | HAL stub 六接口面（先 3 个）+ 任务骨架 | 编译通过；优先级表落地为常量 |
| F 可交互 | LVGL 9.6 + esp_lvgl_port + ST7789 点屏 | 屏出画面；LVGL 缓冲在 PSRAM 且刷新无撕裂 |
| G 质量线 | `.clang-format` + common Host 单测 + CI | 单测全绿 |

## 9. 待定 / 另文

| 项 | 说明 |
|---|---|
| 模块内部代码架构 | 用户后续重点讨论（结构体/接口/数据流），本文只锁边界 |
| LVGL 接入细节 | F 段落地：draw buffer 大小、双缓冲、PSRAM + SPI DMA 对齐实测 |
| `getting-started.md` | 待补：EIM 安装命令、构建/烧录/监视章节、版本核对步骤 |
