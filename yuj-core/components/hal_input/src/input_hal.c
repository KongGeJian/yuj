#include "input_hal.h"

#include "esp_log.h"

static const char *TAG = "input_hal";

esp_err_t core_input_init(void)
{
    /* E 段实现：I2C0（ADS7828）+ I2C1（TCA9555 ×2）初始化 */
    ESP_LOGW(TAG, "input init (stub)");
    return ESP_OK;
}
