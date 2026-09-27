# Issue #9: NAKED retry (0x08007634-0x0800B3F0)

This pass retried the 21 NAKED functions in issue #9's range
(`src/graphics/`, the early actor/collision core). 11 are now real C.

## The main finding: this code was built with old_agbcc

Every function closed here matches under `old_agbcc`. The only
exception is `sub_800AAEC`, which matches under both compilers. For
each file we checked every function under old_agbcc. They all matched,
including the ones that were already matched with register pins under
the current compiler. So each touched object moved whole into
`OLD_AGBCC_OBJS`: `actor_part.o`, `actor_part3.o`, `actor_part7.o`,
`actor_part7b.o`, `actor_part11d.o`, `actor_part11e.o`,
`actor_part11f.o` and `actor_part12.o`. No files had to be split.

Many of the old NAKED notes blamed register-allocation problems that
were really caused by building with the wrong compiler.

## Closed (11)

| Function | File | Technique |
|---|---|---|
| `sub_8007B00` | actor_part.c | old_agbcc; box **returned by value** (the hidden return pointer is the ROM's `r8`/`r0`); mirror bits as `u32` bitfields (`lsl #27` + sign test - a `u8` container gives `movs #0x10; ands`) |
| `sub_8007B98` | actor_part.c | same C as `sub_8007B00` with the box at record+4 |
| `sub_8008044` | actor_part3.c | old_agbcc; `part` in `ip` falls out of the plain shape; second half reads `tick` then `*keyframes` into a local; `animDone` stored from a `u8` local (constant before the address) |
| `sub_8008A40` | actor_part7.c | old_agbcc; box **by value** in and out; the ROM copies it into one shared temporary with `sub_800014C` (memcpy) before each call - an explicit `sub_800014C(&tmp, &box, 16)` reproduces it (struct assignment copies inline with ldm/stm; calling `memcpy` by name gets inlined as a builtin) |
| `sub_8008AD8` | actor_part7.c | old_agbcc; box by value; empty `case 0:` for the `cmp #1; beq; cmp #1; ble; cmp #2` switch; player hit-flag OR through a `u8 *`; push-out reads `part->x` into a local first |
| `sub_8008D80` | actor_part7b.c | old_agbcc; box by value (the "boxH left in its incoming stack slot" blocker is just the by-value ABI) |
| `sub_80099F0` | actor_part12.c | `sub_8008D80`'s twin, same C |
| `sub_80096C0` | actor_part11e.c | `sub_8008AD8`'s twin, same C |
| `sub_8009528` | actor_part11f.c | old_agbcc; box by value + `sub_800014C` copy as in `sub_8008A40`; the duplicated per-node test as a `static inline` helper; `heads = m->gridHead` then `last = &m->gridHead[255]` computed up front |
| `sub_8009868` | actor_part11d.c | old_agbcc; plain C, camera x read before the `>> 8` |
| `sub_800AAEC` | actor_part108.c | guarded do-while list walk (the "per-iteration literal reload"); position read as a struct copy; `rec += 4` as its own statement (the ROM's `adds r1, #4; adds r4, r1, #0`) |

The shared layout of these objects is in the new `include/box_part.h`:
`struct box_part` (the collision/animation view of a part object),
`struct keyframe`, `struct part_box`, `struct part_aabb`,
`struct part_list`, and the `PART_METHOD` method-table accessor.

## Not closed (10)

| Function | State |
|---|---|
| `sub_8007DBC` (actor_part2.c) | old_agbcc C draft under `NON_MATCHING`, 42 hw off. Everything else is right, including r7 for the cached player global. Two spots are off. The gone-bit OR should use the r6 constant 1 left over from the flag tests (`ldrb; orrs r0, r6`), but this C materializes a fresh `movs #1` and reuses r6 for the bitmap's `1 << bit` instead. The spawned part's `mode = 1` should materialize its 1 before the `-4` mask. Tried nested ifs, an inline MarkGone, variables, bitfields, and u32 containers. |
| `sub_800891C` (actor_part7.c) | old_agbcc draft, 18 hw off. The ROM stores the screen box's x/y sp-relative and only then puts `&screen` in r0 for the w/h stores (kept in r8). This C gets the pointer in r2 before the x/y stores, which swaps r1/r2 for `count` and `i*4` in the loop. |
| `sub_8008F20` (actor_part11.c) | old_agbcc draft, 15 hw off, same size and shape as the ROM. The tail is an inline helper (`sub_8009914` has the same tail), the grid clear is a goto loop, and the free-list loop is a guarded do-while. The ROM keeps the count in r3 and copies `&freeListArray` to ip from sb. This C keeps it live in r1 through the grid clear, which pushes the count to r6. |
| `sub_8009914` (actor_part11i.c) | not retried separately: its tail is `sub_8008F20`'s and has the same gap. |
| `sub_800A420` (actor_part110.c) | old_agbcc draft, 15 hw off. All of it is register choice in the first probe's hit path (masked y in r3 and the flags copy made after the bit test in the ROM). |
| `sub_800A178` (actor_part110.c) | not retried (0x2a8 bytes, calls `sub_800A420`). |
| `sub_8009BE0` (actor_part12b.c) | old_agbcc attempt 61 hw off and 8 bytes short. The ROM recomputes `&origY` in the loop and keeps two copies of the tries pointer and `&pos`. Inline helpers and pointer locals didn't reproduce this. No draft kept. |
| `sub_80091D4` (actor_part11c.c) | tried inline versions of `sub_8009150`'s link and `sub_8009A30`'s remove. The shape is close, but the frame and spill slots differ (~200 hw). No draft kept. |
| `sub_8009008` (actor_part11b.c) | not retried (two-phase unlink with an odd `0x100` reload). |
| `sub_800AFF4` (actor_part111.c) | first C attempt, ~170 hw: `self` in r6 instead of r7, and the orbit tail's address caching differs. No draft kept. |

`make compare` was run from clean and passes.
