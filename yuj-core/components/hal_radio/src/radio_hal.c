#include "radio_hal.h"

#include "esp_log.h"

static const char *TAG = "radio_hal";

esp_err_t core_radio_init(void)
{
    /* E 段实现：SPI3 初始化 + nRF24 上电自检 */
    ESP_LOGW(TAG, "radio init (stub)");
    return ESP_OK;
}
