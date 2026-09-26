# The 0x0802B364-0x0802BC68 gap before issue #52 (actor)

A scoping investigation of the actor zone found this 2308-byte range -
the tail of `asm/code_3_2_20_8b7c_ac28.s` right before its own already-
matched literal tail (`sub_802BC68` onward, `src/graphics/
actor_part107.c`, `docs/matching/issue-50-actor-bc68.md`) - still
completely raw. `tools/report_units.py`'s `(0x0802AC28, None, "actor")`
entry covers the still-raw `sub_802AC28`-`sub_802B218` run before this
gap; `docs/rom_map.md` had already flagged `sub_802B364` itself (740 B,
vtable-dispatched at an untraced slot of `gStaticData_087E4E54`) from
disassembly alone.

11 functions total, all on the same `gUnknown_0300148x`-`gUnknown_
030014Bx` object/global cluster already established in `actor_part107.c`
(a countdown-timer/respawn pair at `gUnknown_0300148C`/`gUnknown_
0300149C`, a "camera catch-up" budget at `gUnknown_030014A4`, and the
shared reset/state-transition idiom: state at `self+0x28`, table-index
at `self+0xc`, an anim-frame halfword/byte pair at `self+0x10`/
`self+0x12`, an accumulator at `self+8`, a frame counter at `self+
0x44`).

## Matched (1 of 11 functions)

- **`sub_802BB4C`** (`src/graphics/actor_part127.c`) - frame-counter
  threshold DMA driver: past `0x2c` frames, DMAs a gauge-strip pair and
  resets `self` to state 6/table-index 5 (arming `gUnknown_03001480`
  and kicking the mode transition, the same shared idiom `sub_802B730`
  below uses); otherwise DMAs one of two gauge-strip variants every 4th
  frame without touching any state. Needed the established "materialize
  both sibling constants (state `6` into `r0`, index `5` into `r1`)
  before either store" idiom - a naive `self->0x28 = 6; self->0x44 = 0;
  self->0xc = 5;` sequence lets this compiler re-materialize a fresh
  literal per store instead of reusing the two already-loaded registers
  the ROM keeps live across both.

## Parked - NAKED transcription (byte-correct, not decompiled)

All 10 in `src/graphics/actor_part127.c`. Every one was fully
understood semantically; each resisted a byte-exact plain-C
reconstruction for a different reason, transcribed instruction-for-
instruction from the ROM disassembly instead (this project's
established escape hatch, `docs/matching/issue-4-sio-settings-
sync.md`'s "general strategy"). A small scratch-only Python script
(mechanical `_08XXXXXX:` label renumbering to GNU-as local numeric
labels, plus unified-to-plain mnemonic translation) was used to avoid
hand-transcription typos, the same approach this project has used for
other large NAKED batches.

- **`sub_802B364`** - the countdown-timer/respawn state machine
  `docs/rom_map.md` already flagged. Tail-calls `sub_802BC68` first,
  then drives a `gStaticData_0817A6B8` stride-8 keyframe-table lookup
  (the same categorical r7-hazard shape already NAKED-parked elsewhere,
  e.g. `sub_802C208`) feeding a `sub_803AD84` trampoline dispatch,
  followed by a `gUnknown_030007E0` input-gated position-easing block.
  `r5`/`r6`/`r7` each switch roles repeatedly across the whole function.
- **`sub_802B5B4`** - sprite-frame OAM queuing: computes a keyframe-
  table-driven position offset (with a per-frame interpolation variant
  on a fresh animation transition), applies a shape/priority/palette
  bitmask, arms a `sub_803AD80` trampoline the first time a new part-
  table instance is seen, and queues the final OAM entry via
  `QueueSpriteFrameOam`. `r8`/`sb`/`sl` are all simultaneously live
  across the whole function - the same three-high-register shape this
  codebase's other DMA/OAM functions are consistently NAKED-parked for
  (`docs/matching/issue-56-0x0802f0dc-actor.md`'s `sub_802F7B0`/
  `sub_802F8E8` entry).
- **`sub_802B730`**/**`sub_802B7E0`** - a once-only spawn/reset trigger
  pair (different reset targets - state 6/table-index 5 vs state 0xc/
  table-index 0xb): if the shared "used" respawn timer
  (`gUnknown_0300149C`) is already counting down, report "still used"
  without doing anything; otherwise, while the current hazard tier is
  clear, play a cue, DMA a gauge strip (`sub_802B730` only), reset
  `self`, arm `gUnknown_03001480`, kick the mode transition, and arm
  `gUnknown_030014A0`/clear `gUnknown_030014A3` - or, while a tier is
  already active, arm a fixed `gUnknown_0300149C` countdown and forward
  to `sub_802D4B0` (`actor_part58.c`). Both functions reuse the
  incoming `self` register for an unrelated `1` constant partway
  through (once `self`'s own fields are no longer needed) - the ROM's
  real branch layout keeps the "already used" early-return case sharing
  the exact same epilogue as the main "not yet used" fallthrough case,
  which this compiler's own scheduling would not reproduce as a plain
  `if (...) return 1;` guard clause (it inlines the early-return
  differently instead of jumping to the shared tail).
- **`sub_802B864`** - allocates a pair of VRAM tile blocks
  (`gUnknown_030014B0[0]`/`[1]`), each sized from the same keyframe-
  table byte-pair lookup (`self`'s part table, indexed by `self+0xc`,
  offset by `self+8`'s frame accumulator, into a *second* pointer array
  at `self+4`) already established for `sub_802F338`
  (`actor_part43b.c`, issue #56) - which itself needed heavy `asm
  volatile` register-order forcing for this exact "materialize the
  multiply result into one register, copy it to a second, then shift"
  idiom; transcribed directly here instead of re-chased at the C level.
- **`sub_802B8E8`** - spawn-once trigger for a secondary effect object
  (`gUnknown_03001490`, via `sub_802AC28`), then drains the camera
  catch-up budget into `self+0x20` until it crosses `0x2800`, at which
  point it plays a cue, clamps `self+0x20`, resets `self` to state 9/
  table-index 9, clears `gUnknown_030014A0`, fires the spawned object's
  own trampoline if still alive, clears `gUnknown_03001490`, and fires
  `sub_8029BAC(0x19)`. The spawned-object pointer needed to stay live in
  its own ROM-chosen register (`r0`) across several stores rather than
  being copied to a fresh one, which this compiler did unprompted.
- **`sub_802B990`** - on the state-0x12 anim-frame edge, plays a
  "confirm" cue then either resets `self` to idle (table-index 0) or,
  gated on `sub_8000E1C(3)`'s own result, restores or transitions
  `self+0xc`; independently, on the `gUnknown_030014A3` edge, fires up
  to two more `gUnknown_030007E0`-gated one-shot transitions. The ROM's
  three-way branch structure (skip-reset / sub_8000E1C-gated branch /
  shared tail) didn't survive translation into an equivalent nested
  if/else-if in C - this compiler reordered the reset-vs-check branches
  relative to the `sub_8029BAC(0x24)` call.
- **`sub_802BA5C`**/**`sub_802BAD0`** - camera catch-up accumulate/
  threshold-reset pair and a `gUnknown_030007E0`-gated one-shot
  transition pair, both sharing the same reset idiom as
  `sub_802B730`/`sub_802BB4C`.
- **`sub_802BBE4`** - on the `self+0x12` edge, resets
  `gUnknown_030014A4`'s stall clamp, conditionally spawns a secondary
  effect object once `self+0x20` crosses `0x2000`, and resets `self` to
  state 8/table-index 7 (arming `gUnknown_03001480`, kicking the mode
  transition - the same shared tail idiom as `sub_802B730` above). Hit
  the same "spawned/self pointer gets an extra register copy" gotcha as
  `sub_802B8E8`.

## A note on two literal-pool placement bugs

The first NAKED-transcription attempt at `sub_802B364` initially placed
two `.4byte` pool entries (`gUnknown_030014A1` and `0xFFFFFC80`) too
early - grouped with the nearest preceding code block instead of the
ROM's own, more distant placement (each shared a single pool group with
a *later* code block's own literal, e.g. `gUnknown_030014A1` sits in the
same aligned group as `gStaticData_0817A6B8`, separated from its own
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
clean, no warnings for `actor_part127.c`.

See [docs/status/actor.md](../status/actor.md) for the running
matched/parked list this entry feeds into.
