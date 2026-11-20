#pragma once

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * UI 应用层（LVGL）。所有 lv_* 调用只在 UI 任务或持 lv_lock 时进行（yuj-core/law §2）。
 */
esp_err_t core_ui_init(void);

#ifdef __cplusplus
}
#endif
