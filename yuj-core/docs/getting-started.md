# 快速上手 - getting-started.md

> 目标：让新人通过本文档，可快速运行起工程代码，快速上手。

> 适用范围：**macOS**（Windows / Linux 命令有差异，参考官方文档）。

> ESP32-S3 官方文档（v6.1.1）：https://docs.espressif.com/projects/esp-idf/zh_CN/v6.1.1/esp32s3

## 1. 安装环境

### 1.1. 硬件准备

- 一款 ESP32-S3 开发板（第三方购买或自焊均可），**必须能接原生 USB**（D+/D-，即 GPIO19/20）：
  本项目控制台配置为 `CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG=y`，**日志只从原生 USB 输出**。
  > 若用带 UART 桥（CH340 / CP210x）的第三方板：需把 `sdkconfig.defaults` 的控制台改为 `CONFIG_ESP_CONSOLE_UART_DEFAULT=y`，并用桥对应的串口烧录/监视。
- USB **数据线**（A 转 Type-C；纯充电线不行）
- 电脑：我用的是 macOS

### 1.2. 安装 ESP-IDF 及工具链

① 通过 Homebrew 安装所需的依赖包：
```sh
brew install libgcrypt glib pixman sdl2 libslirp dfu-util cmake python
```

② 通过 Homebrew 安装 EIM （这里选择 CLI 版本）
```sh
brew tap espressif/eim  # 将 EIM 仓库添加到 Homebrew 中
brew trust espressif/eim  # 受信这个仓库
brew install eim  # 安装EIM cli
```

③ 使用 EIM 安装 ESP-IDF （CLI在线安装）
```sh
eim install -i v6.1.1  # 安装指定版本IDF。默认安装路径在 ~/.espressif/
# 或
eim wizard  # 通过交互式安装
```
安装成功后，最后会打印以下信息
```txt
2026-11-20 11:48:48 - 11 - 11 - INFO - Required components successfully downloaded
Configuration saved successfully to config.toml
You have successfully installed ESP-IDF
for using the ESP-IDF tools inside the terminal, you will find activation scripts inside the base install folder
sourcing the activation script will setup environment in the current terminal session
============================================
to activate the environment, run the following command in your terminal:
       source "/Users/kong/.espressif/tools/activate_idf_v6.1.1.sh"
============================================
2026-11-20 11:48:48 - 11 - 11 - INFO - Wizard result: Ok
2026-11-20 11:48:48 - 11 - 11 - INFO - Successfully installed IDF
2026-11-20 11:48:48 - 11 - 11 - INFO - Now you can start using IDF tools
```

> 环境复现/使用已有配置文件在线安装。本项目用不到，完成上线 ①-③ 即可
> ```sh
> # 从其他人拷贝 eim_config.toml 文件。
> eim install --config eim_config.toml
> ```

### 1.3. IDE 配置

IDE：`VS Code` + `ESP-IDF` 拓展

1. 安装扩展后，首次启动按提示选择**使用已安装的 ESP-IDF**，路径指向 `~/.espressif/v6.1.1/esp-idf`
2. 打开目录必须是 **`yuj-core`**（不是仓库根）——扩展靠顶层 `CMakeLists.txt` 识别 ESP-IDF 工程，配套工具才可用
3. 状态栏出现 ESP-IDF 相关图标/按钮，即识别成功

## 2. 项目创建&配置

### 2.1. 进入项目工程

```sh
git clone <repo-url> yuj
cd yuj/yuj-core  # yuj-core 是仓库的子目录，不是独立仓库
```

> 工程已随仓库提供（顶层 `CMakeLists.txt`、`main/`、`components/`、`sdkconfig.defaults`、`partitions.csv`），**无需创建**。
> 不要跑 `idf.py create-project`，也不要往 `yuj-core/` 里复制 `hello_world` 样例——会生成游离工程，污染目录且被 gitignore 隐藏。

### 2.2. 在终端中激活 ESP-IDF 环境

后续所有 ESP-IDF 相关命令都需在已激活的终端中运行
```sh
# 路径以  §1.2.→③ 结束时的打印为准（脚本名带版本号，升级 IDF 后会变）
source ~/.espressif/tools/activate_idf_v6.1.1.sh

# 验证
idf.py --version   # 应输出 ESP-IDF v6.1.1
```
> 该脚本做三件事：设置 `IDF_PATH`、把工具链加入 `PATH`、激活 Python 虚拟环境。

### 2.3. 配置项目

```sh
# ① 指定目标芯片（首次必须；会先 fullclean 再重建 sdkconfig）
idf.py set-target esp32s3

# ② 【非必须项】以 UI 方式浏览/编辑配置，产物是 sdkconfig（也可直接改文件）
idf.py menuconfig
```
> `sdkconfig`（快照）内容 = `sdkconfig.defaults` + 目标芯片 Kconfig 默认值。

> ⚠ 两条纪律：
> 1. **修改 `sdkconfig.defaults` 后，直接重新 build 不生效**——必须删掉 `sdkconfig` 或重跑 `idf.py set-target esp32s3` 才会重新生成。
> 2. **只改 menuconfig = 只有你这台机器生效**。配置要入库必须回填 `sdkconfig.defaults`；`sdkconfig` 本身已被 gitignore（单一真源是 defaults）。

## 3. 连接设备

将 ESP32-S3 开发板连接到 PC，查看串口：

```sh
ls /dev/cu.*
```

| 设备名样式 | 含义 |
|---|---|
| `/dev/cu.usbmodem*` | **原生 USB**（本项目的控制台）✅ |
| `/dev/cu.usbserial*`、`/dev/cu.SLAB_USBtoUART*` | UART 桥（CH340 / CP210x），需另装驱动 |

> 记住串口名，后面 `-p` 要用到。

## 4. 编译&烧录&监视

可运行以下命令，一次性执行构建、烧录和监视过程： `idf.py -p PORT flash monitor`

### 4.1. 编译项目

```sh
idf.py build
```
输出日志（末尾）：
```txt
[19/21] Generating binary image from built executable
Successfully created ESP32-S3 image.
Generated /.../yuj-core/build/yuj-core.bin
yuj-core.bin binary size 0x2e990 bytes. Smallest app partition is 0x300000 bytes. 0x2d1670 bytes (94%) free.
Project build complete. To flash, run:
 idf.py flash
```

如果一切正常，编译完成后将生成以下产物（首次编译约数分钟）：
```text
build/bootloader/bootloader.bin
build/partition_table/partition-table.bin
build/yuj-core.bin          # 应用镜像（当前约 190 KB）
```

### 4.2. 烧录到设备

```sh
idf.py -p <PORT> flash  # 替换 <PORT> 成 §3 记住的串口名
```
输出日志
```txt
（待补：实机烧录后补充）
```

> flash 的任务定义里将 build 声明为前置依赖。如果配置变了，将自动进行增量编译，然后打包烧录。因此无需再运行 idf.py build。

常见失败：
- 串口被占用 → 关掉其它监视器 / 串口工具 / IDE 里占用端口的任务
- 未进下载模式 → 原生 USB 一般自动复位，无需按 BOOT；仍失败则按住 BOOT 再复位

### 4.3. 监视输出

```sh
idf.py -p <PORT> monitor  # 替换 <PORT>
```
输出日志
```txt
（待补：实机运行后补充）
```

使用快捷键 `Ctrl+]`，可退出 ESP-IDF 监视器。

> 其它常用：`idf.py fullclean`（配置大改后清构建目录）、`idf.py erase-flash`（清除 NVS 等残留数据）


---
