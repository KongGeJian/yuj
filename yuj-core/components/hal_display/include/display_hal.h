#pragma once

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 显示 HAL（ST7789 / 240×320 / 4-Line SPI，走 esp_lcd）。
 * 换屏只改 HAL 内部命令表与配置，上层不感知（铁律③）。
 */
esp_err_t core_display_init(void);

#ifdef __cplusplus
}
#endif
