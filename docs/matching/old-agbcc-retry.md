# Retrying the issue #23/#25/#26 NAKED functions under old_agbcc

Issue #24 (docs/matching/issue-24-boss-actor.md) found that this ROM
region was built with the older compiler, `tools/agbcc/bin/old_agbcc`. The
neighbouring passes for issues #23, #25 and #26 had used the current
`agbcc` and parked 7 functions as NAKED, each with a near-miss C
reconstruction under `#if NON_MATCHING`. This pass recompiled each of
those reconstructions with old_agbcc (no `-fprologue-bugfix`) and
compared it against the ROM.

## Result: 3 of 7 closed

| function | file | result |
|---|---|---|
| `UpdateCortexBoss` | `actor_part_188d0.c` (#23) | **matched**, NON_MATCHING C unchanged |
| `sub_801961C` | `actor_part_188d0.c` (#23) | **matched**, rewritten with the `CreatePlatform` stack-argument idiom |
| `CreatePlatform` | `actor_part_1a878.c` (#25) | **matched**, after removing every register pin |
| `sub_801AB98` | `actor_part_1ab98.c` (#25) | still NAKED |
| `InitLevelSelect` | `actor_part_1b85c.c` (#26) | still NAKED here; real C since the #12/#24/#26 retry |
| `LoadLevelSelectRecord` | `actor_part_1b85c.c` (#26) | still NAKED |
| `LevelSelectLoop` | `actor_part_1b85c.c` (#26) | still NAKED |

Issue #23 has no NAKED/NON_MATCHING functions left. Issue #25 still has
`sub_801AB98`, and issue #26 still has all three.

## Whole-file switches, no splits

The five functions that share a file with matched functions would have
needed that file split around them (all of their boundaries are 4-byte
aligned: `0x08018A30`/`0x08018BDC`, `0x0801961C`/`0x0801964C`). That
turned out to be unnecessary. Every function in `actor_part_188d0.c` and
`actor_part_1b85c.c` also matches under old_agbcc. Most of them match with
their agbcc-era C unchanged; a few needed a workaround *removed*. So both
files moved to old_agbcc whole:

- `actor_part_188d0.c`: under old_agbcc as-is, only three functions
  differed. `sub_8018E4C` and `sub_80194E0` each had an `asm("ldrsh ...")`
  with a pinned zero index. Plain `CALL3` gives the ROM's register there
  now. `sub_8019324` had its `part` pinned to r8 and matches with the pin
  removed.
- `actor_part_1b85c.c`: only `sub_801B984` differed (2 bytes), and it
  matches once its zero/id pins are dropped.
- `actor_part_1a878.c` was already on its own.

`OLD_AGBCC_OBJS` in the Makefile now lists `actor_part_188d0.o`,
`actor_part_1a878.o` and `actor_part_1b85c.o` next to `actor_part_1967c.o`.
`ldscript.txt` is unchanged, since no object was added or renamed.

## Workarounds that turned out to be unnecessary

Each was checked by stripping it and recompiling. The ones listed here
are removed from the tree.

- **All register pins in `CreatePlatform`** (12 of them) and its
  `asm("" : "+r"(rec))` barrier. With them in, old_agbcc is still off.
  Without them, reload's rotation lands exactly on the ROM's. The
  spawn-record lookup became `*(u8 **)(lvl + 0xC) + offsets[index]`, and
  the flag set became `obj->flags |= 0x10`.
- **`asm("" : "+r"(c))` constant barriers** in the byte-RMW helpers `AndFlags`/`OrFlags` (#23). old_agbcc already puts
  the constant before the `ldrb`, but only when the constant arrives as
  an inline helper's `s32` parameter. The same statement written in place
  still loads first. Also removed: the barriers in `sub_8018D70`,
  `sub_8018E4C`, `sub_8019094`, `sub_8019214`, `sub_8018BDC` and
  `sub_8018978`.
- **Pins**: `SetTag`'s r0 pin, and those in `sub_80188FC`, `sub_8018978`
  (`ip`), `sub_8018BDC`, `sub_8019094`, `sub_80194E0`, `sub_8019324` (r8)
  and `sub_8018E4C`'s `steps` (r8) (#23). In #26: `UpdateCameraLead`,
  `sub_801B984`, `UpdateLevelSelect`, `DrawLevelSelectRecord` and `DrawLevelSelect`'s
  `asm volatile` self barrier.
- **The zero-index `ldrsh` asm** (#23, see above).

Still needed under old_agbcc: `SetFrameNibble`/`CopyFlipX`'s pinned
`mov/neg` helpers, `MARK_GONE`'s per-site registers, `SET_FRAME_R`,
`StepHeight`'s copy asm, `sub_801B984`'s `mov/neg` masks, the
`CreatePlatform` palette-nibble `0xF` barrier, and the `s32` mask local in
the same spot (a `~0xF & u8` narrows to `movs #0xF0`). Removing any of
them breaks the match.

## The stack-passed byte (`sub_801961C`)

Both compilers widen a `u8` argument passed on the stack to a word `str`.
The ROM `strb`s `CreatePlatformMover`'s fifth argument. `sub_801961C`'s packed
one-byte struct argument gives the `strb`, but under either compiler it
materializes the 0 before `mov r1, sp`, the reverse of the ROM. What
matches is the idiom `CreatePlatform` already used: `volatile` stores of both
stack slots into a `struct mover_stack_args` local (the whole frame, so
it is the outgoing-argument area), then a call through a 4-argument
function-pointer view. A constant QImode store to a stack slot
legitimizes the `sp` address first, which gives the ROM's order. The
struct, the `CreatePlatformMover` prototype and `MOVER_NEW` moved from
`include/gobj_1a794.h` into a new `include/mover_new.h`, which both files
include. This form matches under the current agbcc too.

## What didn't close, and what the old_agbcc diff looked like

None of these four matches under old_agbcc, so their NON_MATCHING C is
left unchanged. The closer variants found are recorded in the issue
write-ups, not applied.

- **`LevelSelectLoop`** (#26), 900 vs 908 bytes. The three `CommitDisplay`
  BLDY-register differences that parked it under agbcc are gone. Two
  things are left. First, the fade-in `evy--`: a packed
  `struct { u8 evy:5; u8 rest:3; }` view fixes it and brings the size to
  908. Second, the ROM's `adds r1, r2, #0` copy of the key word, made
  before the 0x80 test and used only by the 0x20 test. CSE eats the copy
  in every plain C placement. An `asm("" : "+r"(k.all))` barrier keeps it,
  but it either lands one block late (10 bytes off) or in the right place
  with `keys`/`k` in r3/r2 instead of r2/r1 (12 bytes off).
- **`LoadLevelSelectRecord`** (#26), 846 vs 868 bytes. Taking `SetAnim`'s index as
  `s32` (old_agbcc otherwise narrows the table load to `ldrb`),
  `*(u16 *)sv & 0xFFF8` for the saved-time test, and unsigned compares
  each fix part of it. Still left: the ROM's 12-byte frame (it spills
  `info`), and its `time << 16` held across compares with a fresh
  `>> 19` for each one.
- **`InitLevelSelect`** (#26), 1040 vs 1048 bytes. With `SetAnim(s32)` the
  whole sprite-setup middle matches. It is still off in the opening
  blend/BLDY/DISPCNT constant blocks (which register holds 0/1/2/0x10,
  and a separate `self + 0xA9` address), in the item loop's r4/r5 roles,
  and in the ROM hoisting the sprites' `0x80` into r8.
- **`sub_801AB98`** (#25), 1608 vs 1648 bytes, about 290 diff lines.
  The first divergence is structural: the ROM cross-jumps both branches'
  overlap arithmetic (`x + w - x' + 1`) into one shared tail. Stripping
  the pins makes it worse.

`SetAnim(s32)` keeps every matched function in `actor_part_1b85c.c`
matching, so it is safe to adopt whenever `LoadLevelSelectRecord`/`InitLevelSelect` are
picked up again.

## Tools

`oldcc/` in the worktree (uncommitted) held the helper scripts. They
compile one `.c` with either compiler, compare every function against
`baserom.gba` with relocations masked, and try pin/barrier-stripping
variants per function.
