# 遥控终端固件（ESP32-S3）

ESP-IDF + CMake + LVGL。链路主站（TDMA 节拍发起方）、UI、模型管理、USB 双模式。

| 关联文档 | 内容 |
|---|---|
| [../docs/pcb.md](../docs/pcb.md) | 遥控终端板硬件详设（§2）：输入件、引脚分配、电源树、布局 |
| [docs/system-design.md](docs/system-design.md) | 模块系统详设：充电模块（电量 ADC、慢充/快充 UI、状态机） |
| [docs/ai-coding/law.md](docs/ai-coding/law.md) | 模块增补规范：任务优先级/LVGL 线程安全/内存 |
| [../docs/architecture.md](../docs/architecture.md) | 系统分层、UI 设计、菜单树、交互 |
| [../docs/protocol.md](../docs/protocol.md) | 链路协议契约 |

状态：阶段一开发中——整机 PCB + 固件框架。

## 环境清单

- ESP-IDF: v6.1.1
- LVGL: 9.6.0
- 主控芯片： ESP32-S3（N16R8：16MB Flash / 8MB OPI PSRAM）
- 构建工具：CMake + Ninja（IDF 自带）

## 开发环境 & 运行

> 环境安装（macOS / EIM）见 [快速开始](docs/getting-started.md)，本节只列日常命令。

激活环境（每个新终端执行一次）：

```sh
source ~/.espressif/tools/activate_idf_v6.1.1.sh
idf.py --version    # 应输出 ESP-IDF v6.1.1
```

常用命令（在 `yuj-core/` 目录下执行，强烈建议 VS Code 新起一个窗口定位到 yuj-core 目录）：
```sh
# 首次配置（选芯片，重建 sdkconfig）
idf.py set-target esp32s3

# 编译
idf.py build

# 烧录
idf.py -p /dev/cu.usbmodem* flash

# 监视日志（`Ctrl+]` 退出）
idf.py -p /dev/cu.usbmodem* monitor

# 烧录 + 监视一步到位
idf.py -p /dev/cu.usbmodem* flash monitor

# 清构建目录 / 清 Flash
idf.py fullclean
idf.py erase-flash
```

> 串口名见 `ls /dev/cu.*`（原生 USB 为 `cu.usbmodem*`）。
> 配置基线在 `sdkconfig.defaults`（分区表 `partitions.csv`）：**改配置改 defaults，不要只改 menuconfig**——`sdkconfig` 已被 gitignore。
