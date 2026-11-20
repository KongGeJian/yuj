#include "app_ui.h"

#include "esp_log.h"

static const char *TAG = "app_ui";

esp_err_t core_ui_init(void)
{
    /* F 段实现：LVGL 初始化 + 双主界面 */
    ESP_LOGW(TAG, "ui init (stub)");
    return ESP_OK;
}
