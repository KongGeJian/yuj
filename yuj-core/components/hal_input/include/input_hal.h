#pragma once

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 输入 HAL：摇杆/旋钮（ADS7828，I2C0）与按键（TCA9555 ×2，I2C1）轮询。
 * 上层只拿归一化后的通道值，不碰总线细节（铁律③）。
 */
esp_err_t core_input_init(void);

#ifdef __cplusplus
}
#endif
