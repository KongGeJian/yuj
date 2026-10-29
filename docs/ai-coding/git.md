# Git 规范

## 分支模型

| 分支 | 用途 |
|---|---|
| `main` | 稳定线，随时可构建出可烧录固件 |
| `feature/<scope>-<desc>` | 功能开发，如 `feature/rx-fhss`、`feature/core-ui-menu` |
| `fix/<scope>-<desc>` | 修复 |

## Commit 格式

```text
type(scope): 一行描述（中文可）

type: feat / fix / refactor / docs / test / chore / hw(硬件设计)
scope: common / core / rx / docs / pcb
```

## PR 检查单（合并前逐项确认）

- [ ] 单测全绿，`-Werror` 无警告
- [ ] **动了 `yuj-common/` → core + rx 双端构建通过**（CI 强制）
- [ ] 动了帧格式 → 双端同改 + `PROTOCOL_VERSION` bump
- [ ] 动了硬件设计 → `docs/parts.md` / `pcb.md` / 模块文档同步更新
- [ ] 新增公共符号带 `yuj_` 前缀且在头文件有注释契约
- [ ] 不引入「铁律」违规（依赖方向 / HAL 穿透，见 [coding.md](coding.md)）

## 提交节奏

小步频繁提交：一个完整的最小可验证改动（测试+实现）一次 commit，不攒大包。
