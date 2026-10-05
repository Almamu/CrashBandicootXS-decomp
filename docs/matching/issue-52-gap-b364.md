# The 0x0802B364-0x0802BC68 gap before issue #52 (actor)

A scoping investigation of the actor zone found this 2308-byte range -
the tail of `asm/code_3_2_20_8b7c_ac28.s` right before its own already-
matched literal tail (`DispensePolarWumpa` onward, `src/vehicle/
polar_player_states.c`, `docs/matching/issue-50-actor-bc68.md`) - still
completely raw. `tools/report_units.py`'s `(0x0802AC28, None, "actor")`
entry covers the still-raw `CreateActor`-`SpawnActor` run before this
gap; `docs/rom_map.md` had already flagged `UpdatePolarPlayer` itself (740 B,
vtable-dispatched at an untraced slot of `gPolarPlayerVtable`) from
disassembly alone.

11 functions total, all on the same `gUnknown_0300148x`-`gUnknown_
030014Bx` object/global cluster already established in `polar_player_states.c`
(a countdown-timer/respawn pair at `gPolarFinishTimer`/`gUnknown_
0300149C`, a "camera catch-up" budget at `gPolarPlayerVelY`, and the
shared reset/state-transition idiom: state at `self+0x28`, table-index
at `self+0xc`, an anim-frame halfword/byte pair at `self+0x10`/
`self+0x12`, an accumulator at `self+8`, a frame counter at `self+
0x44`).

## Matched (4 of 11 functions)

- **`PolarPlayerStateShocked`** (`src/vehicle/polar_player.c`) - frame-counter
  threshold DMA driver: past `0x2c` frames, DMAs a gauge-strip pair and
  resets `self` to state 6/table-index 5 (arming `gPolarPauseLocked`
  and kicking the mode transition, the same shared idiom `HurtPolarPlayer`
  below uses); otherwise DMAs one of two gauge-strip variants every 4th
  frame without touching any state. Needed the established "materialize
  both sibling constants (state `6` into `r0`, index `5` into `r1`)
  before either store" idiom - a naive `self->0x28 = 6; self->0x44 = 0;
  self->0xc = 5;` sequence lets this compiler re-materialize a fresh
  literal per store instead of reusing the two already-loaded registers
  the ROM keeps live across both.

- **`HurtPolarPlayer`**/**`ShockPolarPlayer`** (`src/vehicle/polar_player.c`) -
  the once-only spawn/reset trigger pair described below, promoted from
  NAKED using the **`goto`-shared-tail idiom**: instead of a plain
  `if (already_used) return 1; ... return 0;` guard clause (which this
  compiler inlines the early return into its own copy of the tail
  rather than jumping to one shared epilogue), the C uses a single
  `register s32 result asm("r0")` local, an explicit `goto end;` for
  the early-return case, and exactly one `return result;` at a shared
  `end:` label - forcing one physical epilogue both paths jump/fall
  into, matching the ROM's own layout. Two more gaps needed closing
  alongside that restructuring, both found by isolated-compile diffing
  against `expected/code_3.s` instruction-by-instruction:
  - The three addresses this function keeps alive throughout
    (`&gPolarInvulnTimer`, `&gPolarAkuAku`, `&gLevelState`)
    each need their own persistent pointer local, read once and reused
    from there (`register s32 *usedTimer asm("r1")`, `register void
    **effectAddr asm("r2")`/`asm("r4")` in the two functions
    respectively, and an unpinned `void **playerAddr` that the
    compiler's own liveness analysis happily put in `r7` on its own -
    no `r7` pin needed, consistent with this project's categorical
    "never pin r7 explicitly" rule). Reading `*playerAddr` a second
    time as its own fresh local (rather than keeping one `player`
    variable alive across both dereferences) was needed to reproduce
    the ROM's own two separate loads into two different registers.
  - `self` (`HurtPolarPlayer`) and the `effectAddr` pointer itself
    (`ShockPolarPlayer`) get reused for an unrelated constant once their own
    value is no longer needed on the "tier clear" path - modeled by
    declaring a *fresh* `register` variable pinned to the same
    register name in a nested scope (`register u8 one asm("r4") = 1;`
    / `register u8 zero asm("r4") = 0;`), the same reuse idiom already
    established for `PolarPlayerStateShocked` above, not a real aliasing hazard
    since the two lifetimes never overlap (mutually exclusive
    branches).
  - Order-of-evaluation gotcha: a plain `u8 *p = &g; register u8 one
    asm("r4") = 1; *p = one;` sequence compiles the register
    initializer (`mov r4, #1`) *before* the address load, but the ROM
    computes the address first. Splitting the address into its own
    plain (unregistered) local declared *before* the register variable
    forced the load-then-materialize order the ROM actually uses.

- **`PolarPlayerStateRun`** (`src/vehicle/polar_player.c`) - the state-0x12
  anim-frame edge reset described below, promoted using the same
  `goto`-shared-tail idiom but for an *interior* shared tail rather than
  the whole function's epilogue: explicit `goto tail;`/`goto gated;`/
  `goto join;` labels reproduce the ROM's exact three-way branch order
  (skip-reset fallthrough / `self+0xc != 0` reset inlined then jumping
  past / `RandRange`-gated pair sharing one physical anim-frame-
  refresh tail) instead of a nested if/else-if this compiler reordered
  differently. The two `gKeys`-gated one-shot transitions
  in the shared tail needed one more fix beyond the established
  "materialize sibling constants" idiom: `gKeys.pressed`'s
  bit-0 test had to read the struct's base address into its own local
  *before* materializing the `1` test-mask constant (`register struct
  held_pressed_pair *addr asm("r5") = &gKeys;` declared
  before `register s32 bit1 asm("r0") = 1;`), matching the ROM's
  `ldr r5, =gKeys` / `movs r0, #1` order - the reverse
  order compiles fine but swaps those two instructions. That same `r5`
  address local is then reused (as a plain `u32` read through it) for
  the second transition's `gKeys` bit-1 test, matching the
  ROM's own reload-free reuse of `r5` there.

## Parked - NAKED transcription (byte-correct, not decompiled)

The remaining 7 in `src/vehicle/polar_player.c` (`HurtPolarPlayer`,
`ShockPolarPlayer`, and `PolarPlayerStateRun` were promoted to the "Matched" section
above in a later pass). Every one was fully understood semantically;
each resisted a byte-exact plain-C reconstruction for a different
reason, transcribed instruction-for-instruction from the ROM
disassembly instead (this project's established escape hatch,
`docs/matching/issue-4-sio-settings-sync.md`'s "general strategy"). A
small scratch-only Python script (mechanical `_08XXXXXX:` label
renumbering to GNU-as local numeric labels, plus unified-to-plain
mnemonic translation) was used to avoid hand-transcription typos, the
same approach this project has used for other large NAKED batches.

- **`UpdatePolarPlayer`** - the countdown-timer/respawn state machine
  `docs/rom_map.md` already flagged. Tail-calls `DispensePolarWumpa` first,
  then drives a `gPolarPlayerStateFuncs` stride-8 keyframe-table lookup
  (the same categorical r7-hazard shape already NAKED-parked elsewhere,
  e.g. `RunPolarPlayerState`) feeding a `_call_via_r3` trampoline dispatch,
  followed by a `gKeys` input-gated position-easing block.
  `r5`/`r6`/`r7` each switch roles repeatedly across the whole function.
- **`DrawPolarPlayer`** - sprite-frame OAM queuing: computes a keyframe-
  table-driven position offset (with a per-frame interpolation variant
  on a fresh animation transition), applies a shape/priority/palette
  bitmask, arms a `_call_via_r2` trampoline the first time a new part-
  table instance is seen, and queues the final OAM entry via
  `QueueSpriteFrameOam`. `r8`/`sb`/`sl` are all simultaneously live
  across the whole function - the same three-high-register shape this
  codebase's other DMA/OAM functions are consistently NAKED-parked for
  (`docs/matching/issue-56-0x0802f0dc-actor.md`'s `LoadBgPicture`/
  `FillBgPictureMap` entry).
- **`AllocPolarPlayerTiles`** - allocates a pair of VRAM tile blocks
  (`gPolarPlayerTiles[0]`/`[1]`), each sized from the same keyframe-
  table byte-pair lookup (`self`'s part table, indexed by `self+0xc`,
  offset by `self+8`'s frame accumulator, into a *second* pointer array
  at `self+4`) already established for `AllocJetpackPlayerTiles`
  (`jetpack_run.c`, issue #56) - which itself needed heavy `asm
  volatile` register-order forcing for this exact "materialize the
  multiply result into one register, copy it to a second, then shift"
  idiom; transcribed directly here instead of re-chased at the C level.
- **`PolarPlayerStateMount`** - spawn-once trigger for a secondary effect object
  (`gRiderlessPolar`, via `CreateActor`), then drains the camera
  catch-up budget into `self+0x20` until it crosses `0x2800`, at which
  point it plays a cue, clamps `self+0x20`, resets `self` to state 9/
  table-index 9, clears `gPolarPlayerInactive`, fires the spawned object's
  own trampoline if still alive, clears `gRiderlessPolar`, and fires
  `SetCellAnimSpeed(0x19)`. The spawned-object pointer needed to stay live in
  its own ROM-chosen register (`r0`) across several stores rather than
  being copied to a fresh one, which this compiler did unprompted.
- **`PolarPlayerStateJump`**/**`PolarPlayerStateDash`** - camera catch-up accumulate/
  threshold-reset pair and a `gKeys`-gated one-shot
  transition pair, both sharing the same reset idiom as
  `HurtPolarPlayer`/`PolarPlayerStateShocked`.
- **`PolarPlayerStateCaught`** - on the `self+0x12` edge, resets
  `gPolarPlayerVelY`'s stall clamp, conditionally spawns a secondary
  effect object once `self+0x20` crosses `0x2000`, and resets `self` to
  state 8/table-index 7 (arming `gPolarPauseLocked`, kicking the mode
  transition - the same shared tail idiom as `HurtPolarPlayer` above). Hit
  the same "spawned/self pointer gets an extra register copy" gotcha as
  `PolarPlayerStateMount`.

## A note on two literal-pool placement bugs

The first NAKED-transcription attempt at `UpdatePolarPlayer` initially placed
two `.4byte` pool entries (`gPolarPlayerHalted` and `0xFFFFFC80`) too
early - grouped with the nearest preceding code block instead of the
ROM's own, more distant placement (each shared a single pool group with
a *later* code block's own literal, e.g. `gPolarPlayerHalted` sits in the
same aligned group as `gPolarPlayerStateFuncs`, separated from its own
first use site by an entire keyframe-lookup block). Since GNU-as
computes `ldr rD, [pc, #N]` distances automatically from wherever a
label is actually placed, this didn't produce an assembler error - just
a wrong (but plausible-looking) immediate encoding, caught only by the
full-link `make compare` byte diff against `baserom.gba`, never by
inspecting the assembly text alone. Fixed by moving each `.4byte` entry
to match the ROM's own pool grouping exactly (verified against
`expected/code_3.s`).

## Verification

Full clean rebuild confirmed (`rm -rf build/crashbandicootxs/src
build/crashbandicootxs/asm build/crashbandicootxs/data
crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map &&
make compare`): `crashbandicootxs.gba: La suma coincide`. (This
session's sandbox lacked a working Pillow install for the graphics
pipeline; the `build/crashbandicootxs/graphics`/`sound` binary outputs
were reused verbatim from another worktree after confirming byte-
identical `graphics/`/`sound/` source trees via `diff -rq` - every
`.c`/`.s` file that actually changed was still fully recompiled and
relinked from scratch.) `make NON_MATCHING=1 report` also compiled
clean, no warnings for `polar_player.c`.

A later pass promoted `HurtPolarPlayer`/`ShockPolarPlayer`/`PolarPlayerStateRun` from
that NAKED batch to real C using the `goto`-shared-tail idiom (see the
"Matched" section above); re-verified with a fresh `rm -rf build &&
make NON_MATCHING=1 report` (clean, no warnings for
`polar_player.c`) followed by `rm -rf build crashbandicootxs.elf
crashbandicootxs.gba crashbandicootxs.map && make compare`:
`crashbandicootxs.gba: OK`.

See [docs/status/actor.md](../status/actor.md) for the running
matched/parked list this entry feeds into.

## Later pass: the other 7 promoted (issue #51/#54 NAKED retry)

`UpdatePolarPlayer`, `DrawPolarPlayer`, `AllocPolarPlayerTiles`, `PolarPlayerStateMount`,
`PolarPlayerStateJump`, `PolarPlayerStateDash` and `PolarPlayerStateCaught` are now real C too, so
the whole gap is decompiled. None of the reasons recorded above held
up: `polar_player.c` is an old_agbcc file (`PolarPlayerStateDash` materializes
its `1` mask before the `ldrh`; `DrawPolarPlayer`/`AllocPolarPlayerTiles` differ only
in load/multiply operand order under the current agbcc), the "stride-8
keyframe lookup" in `UpdatePolarPlayer` is a C++ pointer-to-member call
(`ACTOR_PMF_CALL` on `gPolarPlayerStateFuncs`), and `DrawPolarPlayer` is the
same code as `jetpack_spawn.c`'s `DrawJetpackPlayer`. No register pins. See
[issue-51-54-naked-retry.md](issue-51-54-naked-retry.md).
