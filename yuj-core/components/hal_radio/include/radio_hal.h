#pragma once

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * nRF24 射频 HAL（SPI3 专用）。上层不得直接 include 驱动头（铁律③）。
 * 换射频模块只改本 HAL 内部参数表。
 */
esp_err_t core_radio_init(void);

#ifdef __cplusplus
}
#endif
