#include <inttypes.h>

#include "esp_chip_info.h"
#include "esp_idf_version.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "board.h"
#include "display_hal.h"
#include "input_hal.h"
#include "link_sched.h"
#include "radio_hal.h"
#include "storage.h"

static const char *TAG = "core";

void app_main(void)
{
    ESP_LOGI(TAG, "yuj-core boot, IDF %d.%d.%d", ESP_IDF_VERSION_MAJOR, ESP_IDF_VERSION_MINOR, ESP_IDF_VERSION_PATCH);

    esp_chip_info_t chip = {0};
    esp_chip_info(&chip);
    ESP_LOGI(TAG, "chip: %d core(s), rev %d", chip.cores, chip.revision);

    ESP_ERROR_CHECK(core_storage_init());
    ESP_ERROR_CHECK(core_board_init());
    ESP_ERROR_CHECK(core_radio_init());
    ESP_ERROR_CHECK(core_display_init());
    ESP_ERROR_CHECK(core_input_init());
    ESP_ERROR_CHECK(core_link_init());

    /* D 段最小固件：心跳日志。E 段替换为正式任务骨架（链路 4ms > 输入 > UI）。 */
    while (true) {
        ESP_LOGI(TAG, "heartbeat %" PRIu64 " ms", (uint64_t)(esp_timer_get_time() / 1000));
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
