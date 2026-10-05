# Issues #15/#16: NAKED retry (0x08010D54-0x08012FBC)

This pass retried the 15 NAKED functions in issues #15
(`0x08010D54`-`0x080119A8`, the orbiting-part objects) and #16
(`0x080119A8`-`0x08012FBC`, the `gActionCtrlStateTable` action-table
helpers). 8 are now real C.

## Compiler

As on both sides (issue #12 below, issue #17 above), this code was
built with old_agbcc. Seven of the eight closures only match under it,
so `game_loop52.o`, `game_loop53.o`, `game_loop54.o`, `actor_part83.o`
and `actor_part84.o` moved to `OLD_AGBCC_OBJS`. Every other function in
those files (real C or NAKED) compiles the same under either compiler.
`game_loop50.c` stays on the current compiler (`AddCollisionCandidate` matches
under both).

## Structs

- New `include/orbit_part.h`: `struct orbit_part`, the 0x54-byte part
  object `CreateExtraLife`/`CreateWumpa` spawn and
  `UpdateExtraLife`/`UpdateWumpa`/`UpdateExtraLifeHop`/`UpdateWumpaHop` drive
  (`struct actor` head, animation bank/tag/frame, timer, velocity,
  state/counter/mode/phase bytes, orbit anchor). The x/y head and the
  anchor are both `struct orbit_vec`, so the spawners' paired
  `ldr; ldr; str; str` anchor copy is a struct copy (`ORBIT_POS`).
- `include/action_obj.h` gained named fields only: `act.anims`
  (+0x04), the method slots `m10`/`m28`..`m48`, `act_part` fields
  (`unk_48`/`unk_4C`/`unk_50`/`unk_60`/`unk_8C`/`unk_90`/`unk_94`/
  `unk_100`/`unk_102`/`unk_103`) and `act_anim_bank.unk_0A`.

## Closed (8)

| Function | File | Compiler | Technique |
|---|---|---|---|
| `AddCollisionCandidate` | game_loop50.c | both | The +0x04/+0x08 pair is one by-value `struct pos_pair` argument (the ROM copies it with `ldr; ldr; str; str`), and the two pointer fields are `s32`. The trailing byte arguments are read with `ldrb` straight from their stack words, both addresses first (`add r0, sp, #0x30; add r4, sp, #0x34; ldrb; ldrb`): a `u8` parameter always loads the whole word, so each slot's address goes through an empty `asm("" : "=r"(p) : "0"(&arg))`, with the second pinned to r4. |
| `UpdateExtraLifeHop` | game_loop52.c | old | One reused sine local `sn` (`sn = table[..]; sn = FixedMul(sn, ..)`), which old_agbcc keeps in r2 across both calls; the old "operand order of the pointer add" note was the compiler, not the source. |
| `UpdateWumpaHop` | game_loop53.c | old | Same as `UpdateExtraLifeHop` (0x3000 scale, `gWumpaHopWidths`), plus a trailing `asm(".align 2, 0")`. |
| `PickUpWumpa` | game_loop53.c | old | Plain C; the tag lands in r7 on its own under old_agbcc (the old r7-hazard note). The `+0x25 = 1` store goes through a `u8` local so the 1 is materialized before the field address. |
| `SendWumpaToHud` | game_loop53.c | old | The fixed `-0x1000` offsets go through `OrbitOffset(pos, off)`, an inline taking the offset as a parameter: that makes old_agbcc reload `0xFFFFF000` from the pool for each axis (into r1, then r6) while `0x1400` stays shared in r4, as in the ROM. |
| `CreateExtraLife` | game_loop54.c | old | Plain C: the r8 "zero sentinel" is just a `u8 zero` local stored to +0x49..+0x4B, the anchor copy is `ORBIT_POS`, and the +0x28 bit clears are `s32` 1-bit fields (the ROM's `-17`/`-33` masks). |
| `sub_8012D24` | actor_part83.c | old | The pad object is loaded before `in` is spilled (`void *pad = gInput` first). The held-0x100 result is what `+0x29` is cleared with. The D-pad part sits after the first `UpdatePlayerFacing` tail, reached by a `goto`, matching the ROM layout. The 3..8 range `case` comes before `case 2`. `part->unk_100` is read through `PartByte(part, 0x100)`, an inline with the offset as a parameter, so the 0x100 is rematerialized instead of reused from the input test. |
| `sub_801283C` | actor_part84.c | old | Each distance test is two separate `if`s (a `\|\|` gets folded into one compare). The part/type chain is an `if` chain with a shared `goto hit`. The +0x0D bit-0 set in the charge path is a plain `\|= 1` (the `ActOrFlags0D` inline's 1 would be reused for the trio). The trios mix literal stores and parameter-passing inlines exactly where the ROM materializes constants early (`ActTrio28` for the second trio, `ActQueue27(self, 0, 0)` for the idle reset). |

## Not closed (7)

| Function | State |
|---|---|
| `UpdateExtraLife` (game_loop54.c) | Old_agbcc draft under `NON_MATCHING`. Mode 2's timer compare needs a volatile reload to get the ROM's `ldrh` + signed compare. Left: mode 1 keeps `x + velX` in a register where the ROM recomputes it for the bound check (inline/variable/operand-order variants made no difference), and the collision-bitmap tails use r1/r2/r6 where the ROM uses r5/r6. |
| `UpdateWumpa` (game_loop53.c) | No draft; it has `UpdateExtraLife`'s mode 1/2 blocks verbatim, so it shares the same two gaps. |
| `CreateWumpa` (game_loop53.c) | Old_agbcc draft under `NON_MATCHING`, same instructions. The ROM keeps `id` in r8, `special` in sb and `mode` in r7, and its +0x4B store reuses the zero the frame clamp compares against (an SImode pseudo; +0x49 gets a fresh `movs`). Moving/retyping the `mode`/`phase` locals didn't reproduce that. |
| `ActionCtrlHandleEvent` (actor_part82.c) | Not attempted (1420 B, nested jump tables). |
| `sub_8012AF4` (actor_part83.c) | Old_agbcc draft under `NON_MATCHING`, same instructions and flow (the 0x102/0x103 tests have to be nested ifs, or gcc merges them into one `ldrh`). Register allocation differs throughout: the ROM puts short-lived constants in r5/r6 and keeps the +0x27 slot in r8. |
| `UpdateActionCtrl` (actor_part84.c) | Old_agbcc draft under `NON_MATCHING`, with the gcc 2.x pointer-to-member call through `gActionCtrlStateTable` written out like `ACTOR_PMF_CALL`. The ROM keeps the method record in an 8-byte stack slot and pushes an r7 it never uses (a DImode pair spilled after allocation); the draft keeps it in r6:r7. |
| `sub_8012694` (actor_part84.c) | Old_agbcc draft under `NON_MATCHING`, off only in registers: the ROM loads `self->part` into r0, keeps a copy in r2 and recomputes the +0x2D address for each tag test; old_agbcc keeps the part in r1 and reuses the address. Per-branch `PlaySfx` calls (merged by cross-jumping) and `PartBytePtr` for the player's +0x100 byte already get everything else. |

## Techniques worth reusing

- **Inline parameters decide constant CSE under old_agbcc.** Passing a
  constant through a `static inline` parameter (`OrbitOffset`,
  `PartByte`, `PartBytePtr`) makes it get materialized fresh at that use
  instead of reusing a register that already holds the same value. This
  closed `SendWumpaToHud` and `sub_8012D24` and fixed parts of three drafts.
- **`u8` stack arguments read with `ldrb`.** agbcc always loads a `u8`
  parameter's whole stack word. `*(u8 *)&arg` gives the `ldrb` (as
  `DropExtraLife` already did); to get both addresses formed before the
  loads, hide them behind an empty asm (`AddCollisionCandidate`).

## Tools

Scratch copies of `triage_naked.py`: a per-function disassembly diff
against the ROM under either compiler, and a template-variant runner.
Nothing is committed.

`rm -rf build && make NON_MATCHING=1 report` and a clean `make compare`
both pass.

## Later pass

The second retry (docs/matching/issue-15-16-17-naked-retry-2.md) closed
`sub_8012694`: the tag tests read `self->part` each time instead of
through a local (old_agbcc's GCSE turns those reloads into the ROM's r2
copy), and `pressed & 1` folds into `pressed`'s assignment. The other
functions listed as not closed above are still NAKED.

A third pass (docs/matching/issue-15-16-naked-retry-3.md) closed
`UpdateActionCtrl` and `sub_8012AF4` as real C under old_agbcc, and left new
or updated drafts for `UpdateWumpa` and `CreateWumpa`.
