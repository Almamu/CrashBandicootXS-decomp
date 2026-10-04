# Fresh NAKED retry (issues #4, #7, #9, #10, #15, #16)

This pass covered NAKED functions that earlier passes had skipped or barely
touched: `sub_8001DB4` and `sub_8002114` (#4), `DrawPauseFraction` (#7),
`sub_8009008` and `sub_80091D4` (#9), `UpdateEnemyCtrl` (#10), `UpdateExtraLife`,
`UpdateWumpa` and `CreateWumpa` (#15), and `sub_8012420` and `sub_8012AF4`
(#16).

## Closed (1)

- **`sub_8009008`** (`src/graphics/actor_part11b.c`) builds with old_agbcc,
  and the object is now on `OLD_AGBCC_OBJS`. There was no earlier C draft;
  a first straightforward draft landed 44 halfwords off under agbcc and 6
  off under old_agbcc. Three steps took it to a match:
  - The parking note said the ROM "discards" the bucket index before the
    `count > 1` test. It doesn't: it sets the index to `0x100`, so the
    outer `i--` restarts the scan at bucket 255. The C writes `i = 0x100;`.
  - Phase 1 iterates with the same variable (`found`) that phase 2 sets on
    a match, which gives both the r5 register the ROM uses.
  - The free-list push loads the head before `node->wrap`, and the head
    goes in r1. A `register ... asm("r1")` pin on the head (in the
    `POOL_FREE_NODE` macro) reproduces this. Extra-reference nudges,
    `"+r"` copies, declaration order and an inline helper each fixed only
    the order or only the registers.

## Improved drafts (left NAKED, draft under `#if NON_MATCHING`)

| Function | Before | Now | What's left |
|---|---|---|---|
| `UpdateExtraLife` (game_loop54.c) | 163 hw, 4 B short | 22 hw, same size | An `asm("" : "+r"(vx))` after the x store makes mode 1 recompute `x + velX`. An extra reference to `vx` inside the hit branch and a volatile id compare in mode 2 fix more registers. Still off: mode 1's id compare uses r6 (the ROM uses r2), and mode 2 loads x and velX into swapped registers, which also changes the registers in its fire tail. |
| `sub_80091D4` (actor_part11c.c) | no draft | 215 hw, same size | The removal path is `sub_8009A30`'s body inlined, and `"+r"` copies keep the separate copies of `part` for the destroy and search targets. The ROM keeps `manager` in r7 and `node` in r8; this C puts them in r8 and sb, which shifts every other register. |

## Not closed

- `DrawPauseFraction` (settings_menu16.c): tried reading/writing the position
  through a pointer, reordering the x/y reads, and direct stores, under both
  compilers. The best was 57 hw off with the size fixed, so the draft was not
  updated. The ROM holds 0x110 in r7, re-materializes 0x114 in the first
  reposition and derives it as `r7 + 4` (with an r7-to-r6 copy) in the
  second. No source shape tried reproduced that.
- `sub_8001DB4`, `sub_8002114` (link_cable.c), `UpdateEnemyCtrl`
  (actor_part112.c), `UpdateWumpa`, `CreateWumpa` (game_loop53.c),
  `sub_8012420` (actor_part84.c), `sub_8012AF4` (actor_part83.c): not
  reached this pass. Their existing notes and drafts are unchanged.
