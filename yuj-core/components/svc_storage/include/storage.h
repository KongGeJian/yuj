#pragma once

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 存储服务：NVS 初始化与命名空间访问。
 * 模型/绑定/设置均带 schema 版本号 + 迁移函数（yuj-core/law §5）。
 */
esp_err_t core_storage_init(void);

#ifdef __cplusplus
}
#endif
