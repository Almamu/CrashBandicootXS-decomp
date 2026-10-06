# Issue #23: 0x080188D0-0x0801967C (25 functions)

All 25 functions of the former `asm/code_3_2_17_188d0.s` now live in
`src/bosses/cortex.c` (file retired; `ldscript.txt` links
`cortex.o` in its place). **All 25 are real C**, 0 NAKED, 0
left raw. Full clean `make compare` passes; `make NON_MATCHING=1 report`
builds with no warnings for this file.

**Update (old_agbcc retry, docs/matching/archive/old-agbcc-retry.md):** this
region was built with the older compiler, `tools/agbcc/bin/old_agbcc`.
The whole file now builds with it (Makefile `OLD_AGBCC_OBJS`), which
closed the two functions that were NAKED under the current agbcc
(`UpdateCortexBoss`, `CreateCortexBossPlatformMover`) and made several of the workarounds below
unnecessary - they have been removed. The notes below describe the
original agbcc pass; where a workaround has since gone, it is marked.

## What the code is

The same C++-style object family as `input_ctrl.c`, `input_ctrl_queue.c`
and the Mega Mix files (`mega_mix.c`, `mega_mix_update.c`): small classes with a method table ("vtable") at
`self+0xc`, gcc 2.x `{s16 this-adjust, pad, fn}` method entries called
through the `_call_via_r2`/`AD84`/`AD88` call-via-register trampolines.
Every class has a constructor (base constructor `InitCtrl`,
`CreateBossCtrl` or `CreatePlatformMover`, then its own table pointer, returns
`self`) and a destructor (table pointer, then the base destructor).

The objects drive a "part": an on-screen object built by `CreateMovingSprite`
(`struct gfx_part` in the file): position at `+0`/`+4`, bitmap id `+8`,
flags byte `+0xc` (bit 0 gone, bit 2 hidden, bit 4 active), animation bank
`+0x20` (28-byte records, frame count at record `+0x16`), mirror bit
(`+0x28` bit 4), frame nibble (`+0x29` low 4 bits, from `GetSpriteAnimPaletteSlot`),
animation tag `+0x2d`, frame `+0x30`, "animation finished" `+0x38`,
controller `+0x44`.

- `CreateOneShotAnimCtrl`/`DestroyOneShotAnimCtrl` (`gOneShotAnimCtrlVtable`),
  `CreateUnusedOneShotAnimCtrl`/`DestroyUnusedOneShotAnimCtrl` (`gUnusedOneShotAnimCtrlVtable`),
  `CreateCortexBossGemCtrl`/`DestroyCortexBossGemCtrl` (`gCortexBossGemVtable`),
  `CreateCortexBossPlatformMover`/`DestroyCortexBossPlatformMover` (`gCortexBossPlatformMoverVtable`),
  `CreateCortexShotCtrl`/`DestroyCortexShotCtrl` (`gCortexShotVtable`),
  `CreateTiny`/`DestroyTiny` (`gTinyVtable`, frees/allocates a
  257-entry `i*i >> 8` squares table at `+0x48`): constructor/destructor
  pairs. `CreateUnusedOneShotAnimCtrl` has no caller anywhere (no `bl`, no Thumb pointer in
  the ROM) - **UNUSED**, matched anyway.
- `UpdateUnusedOneShotAnimCtrl`: once the part's animation finishes, the inlined "mark
  gone" sequence (`MarkEntityGone`'s flags bit 0 + `gEntityFlags+0x108`
  bitmap bit).
- `StartTinyHop`: mirror the part towards `self+0x30`, reset two counters
  to 26 and store the part's offset from `self+0x30/0x34`.
- `UpdateCortexBoss` (table method) with `SpawnCortexCannon`/`SpawnCortexTarget`: a
  two-part effect. State 0 spawns two child parts (tags 3 and 15, the
  second offset by `+0x2000,-0x4000`), state 1 sets the first child's tag
  from the second's height, mirrors both towards it and derives both
  frames from the horizontal distance (`5 - min(5, |dx|*12 / width)`),
  state 2 waits three ticks, state 3 sinks everything until it passes
  the level bottom and then signals `RequestRoomExit`.
- The "mover" (`SpawnCortexBossGem` spawner, `UpdateCortexTarget` per-frame update,
  `SetCortexTargetState` state setter, `FireCortexShot` hit-effect spawner):
  `SetCortexTargetDest` (issue #24's range) stores a target and step count,
  `UpdateCortexTarget` interpolates the part towards it
  (`target - delta*remaining/steps`), and the state setter bounces it
  across the level; the level config's `+0x10` index picks the height
  pattern and the per-config timings in
  `gCortexTargetHopSteps`/`35C`/`35F`/`362`.
- `UpdateCortexShot`: box-overlap hit test (`GetSpriteBodyBox`/`GetSpriteAttackBox`/
  `GetSpriteHitbox` boxes, `AabbOverlaps` overlap) of a part against the
  player (fires the player's `+0x68` method with code 9 unless it's busy)
  and against every entry of the `gCollidableList` list.
- `UpdateCortexBossPlatformMover`, `UpdateCortexBossGem`: two more per-frame methods (frame reset
  and loop; a delayed re-skin followed by "mark gone").

## Matching notes

The recurring theme: this code always materializes a read-modify-write's
constant or mask *before* loading the byte it applies to, and computes a
stored value before its address - the shape an inlined C++ setter's
argument evaluation leaves. Plain C bitfield/`|=`/`&=` code loads first.
Fixes, all plain C plus pins/barriers unless noted:

- **Byte RMW helpers** (`OrFlags`, `AndFlags`, `SetFrameNibble`,
  `CopyFlipX`, the two-case flip in `StartTinyHop`): the constant goes
  through an empty `asm("" : "+r"(c))` before the load. Bitfield clears
  must use the sign-extended QImode masks (`-5`, `-0x11`, `-0x10`), which
  is what a bitfield store produces. `SetFrameNibble` also needs the
  established `mov #0x10; neg` inline asm (as in
  `UPDATE_ICON_FRAME_NIBBLE`) since gcc otherwise folds `-0x10` out of
  the `0xf` it just used.
- **Spawner tag store**: the tag value is pinned to r0 and barriered
  before its address is formed. In `SpawnCortexTarget` the ROM also keeps the
  tag constant 15 in r5 across three calls and reuses it as the frame
  nibble mask - reproduced with an r5 pin (`SetFrameNibbleM`).
- **Param-order pinning so `self` lands in r7**: `FireCortexShot` and
  `UpdateCortexTarget` keep `self` in r7, which can never be pinned. Pinning the
  *other* parameters (`part` to r6, `kind`/`n` to r5) leaves r7 as the
  allocator's natural choice for `self`, with the parameter copies in the
  ROM's order.
- **Mark-gone bitmap**: `MARK_GONE` is the `InputCtrlStateDead` sequence
  (volatile id re-read, signed `/ 32`) with the per-site registers as
  macro parameters; `UpdateCortexShot` (part in r8) additionally holds `0x108`
  in r4.
- **Frame clamp** (`SET_FRAME_R`): the ROM loads the record table before
  the tag byte and puts the tag in a callee-saved register (r4/r5/r6)
  distinct from its address register; pinned per site.
- **`SetCortexTargetState`'s inner switch**: the ROM loads the config index into
  r0, copies it to r1, runs three compares on r0 and the `== 2` one on r1.
  No `switch` shape (local, inline-function parameter, pins) reproduced
  the copy, so the compare tree is written with `goto`s and the copy is a
  one-instruction `asm("mov %0, %1")`.
- **Zero `ldrsh` index**: gcc's reload picks r2 for the zero index of the
  method-table `ldrsh`; for one call in `UpdateCortexBossGem` (r3) and one in
  `UpdateCortexTarget` (r4) the ROM picked another register, so that single load
  was a narrow `asm("ldrsh %0, [%1, %2]")` with the index pinned.
  *Gone under old_agbcc*: both are plain `CALL3`s now.
- **Hoisted zero**: `SetCortexTargetState` case 1 keeps a 0 in r4 across two calls
  (loaded between the `animating` store's address and the store itself) -
  reproduced with an r4 pin. *Under old_agbcc* the pin is gone; the
  `zero` local itself is still needed.
- *Removed under old_agbcc* as well: the `SetTag` r0 pin/barrier, the
  `AndFlags`/`OrFlags` barriers (the inline helper's parameter is
  enough), the pins and barriers in `UpdateUnusedOneShotAnimCtrl`, `StartTinyHop`,
  `SpawnCortexCannon`, `SpawnCortexBossGem`, `SetCortexTargetState` and `UpdateCortexBossGem`, the
  barriers in `UpdateCortexTarget`/`FireCortexShot`, `UpdateCortexTarget`'s r8 pin and
  `UpdateCortexShot`'s r8 pin on `part`.
- Smaller ones: `x += 0x2000; y -= 0x4000` as separate statements
  (`SpawnCortexTarget`); `c->pos = part->pos` struct copy for the ldr/ldr/str/str
  order; `self->squares[i]` indexing instead of a walking pointer
  (`CreateTiny`); an explicit empty `case 10` to keep `UpdateCortexTarget`'s
  11-entry jump table; `if (stepsLeft) break; goto next;` for its
  branch-trampoline shape; the `_call_via_r4` call's function pointer
  loaded into r4 through a volatile read (the `player_collide.c` idiom);
  `part` pinned to `ip` in `StartTinyHop`.

## Formerly NAKED, matched under old_agbcc

- **`UpdateCortexBoss`**: under agbcc every operation and the 5-entry jump
  table were reproduced, but the ROM keeps `self`/`part` in r5/r6 and
  uses r7 as a short-lived scratch register three times (the flip byte,
  the first frame clamp's tag, the `0x4000` constant), which agbcc never
  did. The unchanged NON_MATCHING C matches byte-for-byte under
  old_agbcc; only `AndFlags`'s barrier was dropped afterwards.
- **`CreateCortexBossPlatformMover`**: `CreatePlatformMover`'s fifth argument is a byte the caller
  `strb`s into the outgoing stack slot. Both compilers widen a `u8`
  stack argument to a word `str`, and a packed one-byte struct always
  materializes the 0 before the `mov r1, sp` slot address (the reverse
  of the ROM) under either compiler. What matches is the idiom
  `CreatePlatform` uses (`include/mover_new.h`): write both stack slots
  through `volatile` stores into a `struct mover_stack_args` local (the
  only thing in the frame, so it *is* the outgoing-argument area) and
  call through a 4-argument function-pointer view (`MOVER_NEW`). A
  constant `strb` to a stack slot legitimizes the address first, which
  gives the ROM's order. (This form also matches under the current
  agbcc.)
