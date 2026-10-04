# Issues #9-#11: NAKED retry (0x08007634-0x0800D040, box builders / collision)

This pass retried the NAKED functions left in issues #9, #10 and #11
(21 of them) - the part-list/AABB/collision core, the `sub_800B8DC`
controller cluster and the physics subsystem's first functions. 13 are
real C now.

## What worked

Most of these came down to one or two source-shape details, found by
compiling many small variants of a draft (declaration order, statement
order, where a copy is taken, an occasional register pin) under both
compilers and diffing each against the ROM:

- **old_agbcc.** `actor_part2.o`, `actor_part110.o`, `actor_part123.o`
  and `game_loop42.o` now build with it (each whole file matches under
  it). Under old_agbcc `sub_800A0FC` no longer needs the register pins
  it had under the current compiler.
- **One frame struct for the stack boxes** (`sub_800891C`), as in
  actor-zone-naked-retry.md.
- **Where a copy is taken.** `sub_8008F20`/`sub_8009914`: a
  `fl = freeList` copy taken inside the `if (i < n)` guard, right
  before the `do`, is the ROM's `mov ip, sb`. Read directly from
  `m->freeListArray`, the loop optimizer hoists `m + 0x810` above the
  grid clear and the whole allocation shifts. `sub_800CF70`: a local
  copy of the `self` parameter makes the ROM copy r0 last.
  `sub_800A178`: the `self+0x24` pointer taken after its `& 0xc` test
  value.
- **A constant in a variable.** `sub_8007DBC`: the flag tests'
  constant 1 lives in r6 (pinned) and the `gone` OR reuses it. The
  spawned part's `mode = 1` goes through a `u32` local, which puts the
  1 before the `-4` mask.
- **Bitfield vs byte views of the same flags byte** (`sub_800CBF4`):
  set the gone bit through a bitfield view and test bit 3 through the
  byte view. The union has to be 4 bytes (ARM structs are word-sized);
  a packed 1-byte union compiles differently.
- **Register pins** (never r7/r8), where the allocator tie didn't
  move otherwise: `sub_800C5D4` (target r1), `sub_800C244` (`baseY`
  r1), `sub_800C8F8` (product r2), `sub_800A420` (flags r2, its copy
  r1), `sub_8009BE0` (tries pointer r6), `sub_8007DBC` (constant r6).

## Closed (13)

| Function | File | Compiler | Technique |
|---|---|---|---|
| `sub_8007DBC` | actor_part2.c | old | constant 1 in a variable pinned to r6 and assigned inside the first flag test (`& (one = 1)`), `gone` ORs it in; spawned part's `mode = 1` through a `u32` local; the player method call needs the `_call_via_r4` alias. File moved to old_agbcc. |
| `sub_800891C` | actor_part7.c | old | near/screen boxes in one frame struct; the screen box's w/h stored through a `&screen` pointer taken after its x/y stores (that pointer is the loop's r8). |
| `sub_8008F20` | actor_part11.c | both | grid clear as a plain indexed `for` (gcc reverses it); free-list loop reads the array through `fl = freeList` taken inside the guard. |
| `sub_8009914` | actor_part11i.c | both | same tail as `sub_8008F20`; the teardown loop is plain C. |
| `sub_8009BE0` | actor_part12b.c | both | pos + origY as one frame struct; loop increments through a `t2 = tries` copy and tests through `tries` (pinned r6); `pos.y` bumped through `&pos`; `u8` first-probe result in a local; the in-loop hit restores the flag and returns on its own (reloads the global from the pool; cross-jumping shares the `strb`). |
| `sub_800A178` | actor_part110.c | old | `s32` result set by `? 8 : result` (expands to `-(x != 0)` into the result then `&= 8`); `self+0x24` pointer taken after its test value; the two out-bytes for `sub_800A420` are separate `u8` locals. File moved to old_agbcc. |
| `sub_800A420` | actor_part110.c | old | first hit path: bit-1 test into its own local, then the copy `v = f`, with `f` pinned r2 and `v` r1. |
| `sub_800C244` | actor_part119.c | both | target held in a local after the two calls, `baseY` pinned to r1 for the store. |
| `sub_800C5D4` | actor_part120.c | both | the `kind == 0xB` prelude's target in a block local pinned to r1. |
| `sub_800C8F8` | actor_part116.c | both | product into a fresh `v` pinned to r2, then the target load. |
| `sub_800CBF4` | actor_part123.c | old | inline `MarkGone` with the do/while(0) `SET_ID_BIT`; gone bit via a bitfield view, bit 3 tested via the byte view (4-byte union). File moved to old_agbcc. |
| `sub_800CEAC` | game_loop42.c | both | the wide-mode x as `x += xOffset; x -= 2;`. |
| `sub_800CF70` | game_loop42.c | old | local copy of the `self` parameter. File moved to old_agbcc. |

`sub_800A0FC` (actor_part110.c, already real C) lost its register pins
when the file moved to old_agbcc; the only shape left is its
`self+0xc` clear through an `s32` local (so the mask stays the
SImode `-0x21`). The mask clears bit 5 only (`~0x20`), not `~0x21` as
the older notes said.

`include/box_part.h` gained the fields these functions read:
`moveAxes` (+0x24), `physMode` (+0x4D), `state` (+0x4E), `hitAxes`
(+0x68), `probeTries` (+0x69) and `hitMask` (+0x74).

## Not closed (8)

| Function | State |
|---|---|
| `sub_8009008` (actor_part11b.c) | not retried. After an unlink it sets the bucket index to `0x100` before the `count > 1` test, so the outer loop restarts at 255 - that has to be in the source somehow, not found. |
| `sub_80091D4` (actor_part11c.c) | not retried this pass. |
| `DrawPlayer` (actor_part111.c) | new old_agbcc draft under `NON_MATCHING`, not converged (624 bytes vs 636): `self` in r6 instead of r7 and one spill slot too many. The child repositioning goes through an inline whose argument order (x, y, child) gives the ROM's load order. |
| `sub_800B8DC` (actor_part112.c) | not retried (1132 bytes). |
| `sub_800BD48` (actor_part112.c) | draft unchanged, 21 hw (old). In states 1/21/22 the ROM loads the layer's 1 before the `-4` mask and reuses that register as the `gone` OR's operand and destination; a `u32` local gets the order but costs a copy (4 bytes over). The rest is reload scratch registers. |
| `sub_800C940` / `sub_800C97C` (actor_part116.c) | drafts unchanged (10 / 27 hw). The ROM saves a callee-saved register it never uses (r5, and r8 with r7 skipped). |
| `sub_800CD00` (actor_part109.c) | new old_agbcc draft under `NON_MATCHING`, 42 hw: the player box's address is held in r6 from its first build (CSE, including through `-fno-cse-follow-jumps`/`-fno-cse-skip-blocks`) where the ROM rematerializes it from sp until the first overlap test. |

The raw-asm functions `DrawAffineSpritePieces`, `sub_800A884` and `PlayerHandleEvent`
(still in `asm/*.s`) were not attempted.

## Tools

Scratch helpers (not committed): a per-function side-by-side diff that
re-disassembles the ROM bytes through the compiled object's own mapping
symbols (so literal pools line up), a whole-file checker for both
compilers, and a brute-force runner that compiles every combination of
a few source variants and ranks them by halfword diff.

`rm -rf build && make NON_MATCHING=1 report` and a clean `make compare`
both pass.
