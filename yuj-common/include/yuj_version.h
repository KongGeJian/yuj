#pragma once

/**
 * 链路协议版本（契约见 docs/protocol.md §7）。
 *
 * 纪律：改动帧格式 → 双端同改 + bump 本值；yuj-core / yuj-rx 各自 pin
 * EXPECTED_PROTOCOL_VERSION 并 _Static_assert，不一致编译失败。
 */
#define YUJ_PROTOCOL_VERSION 1
