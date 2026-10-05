# Second big NAKED retry: `UpdateEnemyCtrl` and `ActionCtrlHandleEvent`

Two large NAKED functions that had never had a C draft kept in the
tree. Both closed as real C under old_agbcc.

| Function | File | Issue | Size | Compiler | Result |
|---|---|---|---|---|---|
| `UpdateEnemyCtrl` | `src/graphics/actor_part112.c` | #10 | 1132 bytes | old_agbcc (object added to `OLD_AGBCC_OBJS`) | matched |
| `ActionCtrlHandleEvent` | `src/graphics/actor_part82.c` | #16 | 1420 bytes | old_agbcc (object added to `OLD_AGBCC_OBJS`) | matched |

Under the current agbcc the final C is 43 halfwords off for
`UpdateEnemyCtrl` and 36 for `ActionCtrlHandleEvent`. The other function in
`actor_part112.c`, `HitEnemy`, is still NAKED, so the whole file
builds the same under old_agbcc. `actor_part82.c` holds only
`ActionCtrlHandleEvent`.

Neither function needed loop work: both are a single `switch`. Both
first drafts were written from the ROM disassembly, with the case
bodies in the ROM's block order and the fields named through
`include/part_ctrl.h` and `include/action_obj.h`. Each first draft
already had most instructions right. The rest was found with the
`brute2.py` variant runner, a few options per run, mostly one statement
at a time.

## `UpdateEnemyCtrl` (313 -> 0 halfwords)

The first draft was 16 bytes short and 313 halfwords off.

- **The re-test in state 18.** The ROM tests
  `target->animDone && mode == 3`, and in the else branch reloads the
  target and tests `animDone` again before `mode == 5`. gcc's jump
  threading folds the second test into the first. Spelling it
  `(animDone << 24) != 0` kept the re-test, fixed the size and moved
  `self` to the ROM's `r5` (313 -> 118), but added an `lsl`. Reading
  the byte through a `vu8` instead keeps the re-test without the extra
  instruction (part of the next step).
- **Values read into locals before stores.** State 5's height tests
  read `t->y` into a local first (the ROM loads it before the camera
  pointer). The second test goes through its own `t2`. State 9's global
  stores go `x = target->x; gHomingEnemyX = x;`, which gives the
  ROM's value-then-address order. State 18's volume is its own local.
  118 -> 27.
- **The stack byte first.** State 18 passes a packed one-byte struct
  by value to `PlayAmbientSfx`, as `actor_part128.c` does. Storing
  `zero.v = 0` before the distance math, instead of right before the
  call, gave the ROM's register choice for the `0x100` constant (27 -> 0).
- Smaller fixes along the way:
  - The hit bit is `(flags >> 3) & 1` through `struct box_part`, so the
    test is `lsrs #3; ands #1` rather than a `movs #8; ands` bit test.
  - The distance clamp is `d = d < 0x20 ? 0x20 : d` (keeps the ROM's
    `cmp #0x20; bge`).
  - State 17's position reset goes through a `SetPos(t, x, y)` inline,
    so both values are loaded before either store.
  - The mode-6 keyframe rewind uses a fresh local for the reloaded target.

States 1 and 12 are explicit empty cases (the table is indexed by
`state - 1`). `MarkGone`/`SetVelX`/`SetVelY` moved to the top of the
file, and `HitEnemy`'s kept draft now uses the shared copies. That
draft is still 21 halfwords off.

## `ActionCtrlHandleEvent` (516 -> 0 halfwords)

The first draft was 16 bytes short and 516 halfwords off. It found one
thing the old doc comment had wrong. The function takes a fourth
argument: case 12 ORs `r3` into the part's contact byte and tests
`r3 & 3`, and nothing in the function sets `r3`. The gate is also the
other way round: the function returns *when* `self->state == 0x1D`.

- **The nested keyframe lookup is a macro.** Case 23 uses
  `GetSpriteFrameAnchor`'s nibble switch twice. As an inline function, the return
  value came back in `r0` and was then copied into `r7`. As a
  `PART_OFFSET(dst, part)` macro that assigns `dst` in each case, as
  `GetSpriteFrameAnchor` itself assigns `result`, the ROM writes `r7` directly.
  516 -> 328.
- **Fire-button cases (13/14/25).** The ROM materializes the trio's
  `1` (`flag30`) in `r5`, then a separate `1` for the `& 1` fire test.
  With plain constants, CSE shares one `1` and copies it. The trio's
  `1` is `asm("" : "=r"(one) : "0"(1))` (brief item 10) and is stored
  through an `ActTrio28(self, 0, one, next)` inline. 13 and 14 also read
  the input through a second local (`held = in`), which gives the ROM's
  `adds r2, r0, #0` copy. 328 -> 2.
- **The part's Y is read into a local before the subtraction**
  (`y = part->y; part->y = y - (d << 8)`). This puts the `lsl #8` after
  the load. 2 -> 0.
- **A dead load.** In case 12, the ROM loads `self->state` into `r0`
  and never uses it (at `0x08011E1E`, just before the `unk_2B = 3`
  store). Without a matching read, the case is laid out differently
  (352 halfwords, 4 bytes short). A `*(vs32 *)&self->state;` statement
  reproduces it. I didn't find a source form that leaves a dead
  non-volatile load.
- Smaller fixes:
  - `hanging` and `bumped` are stored through inline setters, so the
    value is materialized before the offset.
  - The player's `flags0C &= 0x7f` goes through a `u8 *`. As a struct
    member store, the zero from the dead `& 0` gets reused for the later
    trio stores (the problem `ACT_PART_FLAGS0D` works around).
  - Case 12's two mirror tests are spelled differently:
    `(s8)(flags28 << 3) < 0` and `(u32)(flags28 << 27) >> 31`. The ROM
    has a sign test in one and an extract in the other.
  - The zero in cases 1/4/6 is a local assigned before the `slippery`
    test.

`include/action_obj.h`'s unused `unk_54[0xC]` became three named `s32`s
(now `rampYStart`/`rampYStep`/`rampYTarget`) for the velocity triple. No other file
used it.

## Tools

The scratch helpers were a copy of `polish2/brute2.py` with two
additions:

- jump-table masking (`JT=26,8,8` zeroes each table after a
  `mov pc, r0` so the diff lines up);
- `SORT=score`, which sorts by disassembly diff score instead of
  halfword count. While a draft's size is wrong, every halfword after
  the first difference counts as off, so the halfword count doesn't
  show progress.

Nothing is committed.

## Verification

- `rm -rf build && make NON_MATCHING=1 report`: no warnings from the
  touched files.
- `rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare`:
  `crashbandicootxs.gba: OK`.
