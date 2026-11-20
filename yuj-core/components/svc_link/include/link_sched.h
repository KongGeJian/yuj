#pragma once

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 链路调度服务：主站 TDMA 4ms 节拍、failsafe、绑定状态机（契约见 docs/protocol.md）。
 * E 段实现；调度任务优先级最高（yuj-core/law §1）。
 */
esp_err_t core_link_init(void);

#ifdef __cplusplus
}
#endif
