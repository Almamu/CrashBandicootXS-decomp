# Huge NAKED retry 2: `QueueCratePlayerCollision`

| Function | File | Issue | Before | After |
|---|---|---|---|---|
| `QueueCratePlayerCollision` | `src/system/game_loop47.c` | #12 | 938 hw, size-exact | 33 hw, size-exact, still NAKED |

All numbers are for old_agbcc (`game_loop47.o` is on `OLD_AGBCC_OBJS`).
The function doesn't match yet, so it stays NAKED. The improved draft is
kept under `#if NON_MATCHING`. Under current agbcc the draft is 1399
halfwords off and 8 bytes short.

## What fixed it

These are in the order they were applied. Each step was checked against
the ROM one region at a time and kept the exact size, except where noted.

1. **`goto edge_x` for the `dy > 2 || (dx <= 3 && sub_800B324())` arm,
   and `BOX_ADDR` on the rebuilt player box** (both from the last-five
   pass). With the steps below the 4 extra bytes go away.
2. **`f.c = f.a` through a pointer local** (`struct aabb *pc = &f.c;
   *pc = f.a;`). The block copy then takes a copy of the pointer
   (`add r2, sp, #0x2c; adds r1, r2, #0`), as in the ROM. (938 -> 750.)
3. **No `pp` for `pos` and `p2`.** Their y is written and read through
   `D18C_PosPtr(&f.pos)->y` (an identity inline, as in `sub_801AB98`),
   after reading the player's y into a local. gcse then makes the ROM's
   `add r0, sp, #N; str r1, [r0, #4]; adds r2, r0, #0`. `p1` keeps `pp`.
   `p3` does both: it writes y through `D18C_PosPtr` and then sets
   `pp = &f.p3`. `pp` must stay shared between `p1` and `p3`, or `p1`
   loses r2.
4. **`for (e = GetCrateBelow(self); e; e = GetCrateBelow(e))`** for the
   neighbour walk. The first call is cross-jumped into the loop's call, so
   the ROM enters the loop with `mov r0, sl`.
5. **`FindLineCrossing` called in each `dirX` arm** of both slope checks. In
   the ROM, the stack argument is stored before the join (`str r6,
   [sp]`) and px is passed from its register (`adds r2, r3, #0`), not
   reloaded. That only happens if each arm has its own call and
   cross-jumping merges the tails. (4 bytes short until step 7.)
6. **The `ay` nudge (`asm("" : : "r"(ay))`) removed.** With step 5, ay
   gets enough references by itself. The nudge now put ay ahead of dx,
   which swapped r5/r7. (Register-level diff 166 -> 85 instructions.)
7. **`asm("" : : "r"(ax), "r"(px))` after `GetSpritePrevX` in the
   `fallDistance` path.** This replaces the dead `side` code. The ROM reloads px
   into r1 there and never uses it (a leftover of a compare deleted after
   reload). An empty asm that uses both values gives the same reload.
   (1035 -> 209.)
8. **One function-level `r`** for both slope checks. Separate block
   locals gave the first one r1; shared, both get r2 as in the ROM, and
   the `edge = 8` / `edge = dirX` tails cross-jump like the ROM's. (209
   -> 72.)
9. **`D18C_Code(row, k)` inline for the code lookups** at the first
   response and in `case 8`. It moves `&obj->kind` / `tgt->kind` ahead of
   the table address, as in the ROM. (72 -> 67.)
10. **`if (dy <= 2) { edge = 4; ... } else edge = dirX;`** in the `ax ==
    px` arm, instead of `goto edge_x`. Cross-jumping merges the `else`
    into edge_x, but it is still in the RTL between that arm and the next
    one when reload runs. Its reload of dirX takes r0 and moves reload's
    round-robin on by one register, so the next arm reloads dy into r1
    as the ROM does. (67 -> 60.)
11. **`D18C_RingCount()` (an `s32` inline) for the first `ringCount != 0`
    test.** Written in place, `shorten_compare` narrows the test to a
    QImode load (`ldrb` + shifts in RTL). The loop test loads with
    `zero_extendqisi2`, so cse can't reuse the value and the ROM's
    `cmp r3, r0` became a reload. Through the inline both tests use the
    same load. (60 -> 33.)

## What's left (33 halfwords)

- **Spill slots 0x94/0x98.** The `&self->state` and `kind * 4` gcse temps
  are swapped. gcse creates its temps in expression-hash order (buckets
  0 to size - 1). `kind * 4` hashes to 452 and `self + 77` to 511. Both
  are below the table size (835), so no instruction-count padding can
  swap them. Moving the declarations of variables that don't spill
  changes nothing. `asm("")` padding at the top has no effect up to 37
  statements; from 38 on, the output changes a lot and is 8 bytes short.
- **The first code lookup** loads the table address after `&obj->kind`.
  The ROM loads it first. Passing the table as an inline argument fixes
  that order, but then the address sum is added in a different order.
- **The second slope check** (`ay += q->yOff`, ROM `151`-`153`): here
  local-alloc gives the `ldrsh` result r0, where the ROM has r1. The next
  ~20 instructions then take the wrong reload registers. An r0 hold
  across the result's lifetime gets r1, but then reload spills the value
  to the stack (4 bytes long). Holds around the first slope check's
  `edge` assignments break cross-jumping.

## Tools

The helpers are in the scratch area `huge2/`. They are copies of `huge/`
with the worktree path updated, plus:

- `st.py`: the official halfword count and both asmdiff scores.
- `s*.py`: bf.py variant specs.

`asmdiff.py` now treats `r9`/`r10`/`r12` as `sb`/`sl`/`ip`.

## Verification

- `rm -rf build && make NON_MATCHING=1 report`: no warnings from
  `game_loop47.c`.
- `rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare`:
  `crashbandicootxs.gba: OK`.

## Later

The third pass closed the last 33 halfwords; `QueueCratePlayerCollision` is real C.
See [huge-naked-retry-3.md](huge-naked-retry-3.md).
