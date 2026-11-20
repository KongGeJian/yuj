#pragma once

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 板级初始化：引脚表与板级常量的唯一真源（引脚来自 docs/pcb.md §2）。
 * 调用时机：app_main 最早阶段，早于任何 HAL。
 */
esp_err_t core_board_init(void);

#ifdef __cplusplus
}
#endif
