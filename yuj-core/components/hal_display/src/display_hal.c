#include "display_hal.h"

#include "esp_log.h"

static const char *TAG = "display_hal";

esp_err_t core_display_init(void)
{
    /* F 段实现：SPI2 + esp_lcd panel（ST7789）+ LVGL 接入 */
    ESP_LOGW(TAG, "display init (stub)");
    return ESP_OK;
}
