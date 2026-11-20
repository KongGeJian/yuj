#include "link_sched.h"

#include "esp_log.h"

static const char *TAG = "link_sched";

esp_err_t core_link_init(void)
{
    /* E 段实现：链路调度任务（4ms 节拍）+ 绑定/failsafe 状态机 */
    ESP_LOGW(TAG, "link init (stub)");
    return ESP_OK;
}
