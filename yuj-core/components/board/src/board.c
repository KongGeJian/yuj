#include "board.h"

#include "esp_log.h"

static const char *TAG = "board";

esp_err_t core_board_init(void)
{
    /* B/D 段占位：引脚表在 E 段按 docs/pcb.md §2 录入 */
    ESP_LOGI(TAG, "board init (stub)");
    return ESP_OK;
}
