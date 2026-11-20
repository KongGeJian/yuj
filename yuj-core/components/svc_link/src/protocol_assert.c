#include "yuj_version.h"

/* 铁律②：协议版本双端同步。
 * yuj-common 的 YUJ_PROTOCOL_VERSION 变更时，yuj-core 与 yuj-rx 必须同改并各自 bump 本值，
 * 否则构建期断言失败。 */
#define CORE_EXPECTED_PROTOCOL_VERSION 1

_Static_assert(YUJ_PROTOCOL_VERSION == CORE_EXPECTED_PROTOCOL_VERSION,
               "yuj-common PROTOCOL_VERSION 已变更：双端同改后再 bump core/rx 的期望值");
