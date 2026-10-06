# Issue #16, final remainder: 0x08011BD4-0x08012FBC (6 functions)

Continuation of
[issue-16-actor-12160.md](./issue-16-actor-12160.md), which matched
four more members of the `gActionCtrlStateTable` 42-slot action-dispatch
table and left the hardest six of that pass's ten-function remainder
untouched: `ActionCtrlHandleEvent`, `UpdateActionCtrl`, `TryActionCtrlDoubleJump`, `HandleActionCtrlAirInput`,
`ApplyActionCtrlMotion`, `ActionCtrlStateIdle`. This pass closes out all six - but as
NAKED transcriptions, not real decompiled C (see "Why NAKED" below).

## New files

`asm/code_3_2_17_11bd4.s` (just `ActionCtrlHandleEvent`, 0x08011BD4-0x08012160)
is removed entirely - its function moves to a new
`src/player/action_ctrl_event.c`. `asm/code_3_2_17_12420.s`
(`UpdateActionCtrl`/`TryActionCtrlDoubleJump`/`HandleActionCtrlAirInput`, 0x08012420-0x08012A7C) is
also removed entirely, moving to a new `src/player/action_ctrl_update.c`.
`asm/code_3_2_17_12af4.s` is trimmed to drop its leading
`ApplyActionCtrlMotion`/`ActionCtrlStateIdle` span (0x08012AF4-0x08012FBC, which moves to
a new `src/player/action_ctrl_idle.c`), now starting at `ActionCtrlStateRun`
(0x08012FBC) - the rest of that file was already out of this issue's
scope per the prior pass's writeup. `ldscript.txt` and
`tools/report_units.py`'s `UNITS` list were updated to place all three
new files in their correct link order (`action_ctrl_event.c` before the
already-matched `kill_player.c`, `action_ctrl_update.c` between
`kill_player.c` and `action_ctrl_left_ground.c`, `action_ctrl_idle.c` right after
`action_ctrl_left_ground.c` and before the trimmed `code_3_2_17_12af4.s`).

## Semantics (all six)

- **`ActionCtrlHandleEvent`** (1420 B, `action_ctrl_event.c`) - bails unless
  `self+8 == 0x1d` (the same type gate `UpdatePlayerCtrl`, still raw,
  checks). Otherwise dispatches its third argument (`arg2 - 1`, range
  `0..0x18`) through a 25-case jump table: several cases are thin
  `KillPlayer` wrappers with a fixed id; case 22 and case 23 each run
  a shared 7-case inner dispatch on `GetSpriteFrame(part)`'s nibble result
  to compute a Q8 position delta from a `gEmptySpritePoint` record
  (or the object's own `+0x24`/`+0x14` fields as a nibble-1-5
  fallback), then reset the state/flag/table-index trio via
  `SetActionCtrlModeAnim`; case 24 plays sound(s) gated on `gKeys`
  bits and either resets three `self+0x22..0x24` bytes plus calls
  `ApplyActionCtrlMotion`, or chains through `FadeOutMusic`/a
  `LoadPaletteSlot`-fed 28-byte-record lookup; case 11 resets a child
  object and pool-releases it via one of two dereference-chain-computed
  slots depending on the player's D-pad remap state; cases 9/10 gate
  `StartActionCtrlMaskHitJump` behind `gPlayer+0x68`/`PlayerHasRoomForAnim` checks.
- **`UpdateActionCtrl`** (628 B, `action_ctrl_update.c`) - a `part`-visibility/OAM-
  priority housekeeping pass: re-runs `UpdateActionCtrlSkidAnim` on an activity-flag
  change, resets velocity/target fields past two `gLevelLayers`-
  anchored screen-space thresholds (the far one also firing
  `SetMaskLevel`/`_call_via_r4`), ticks a couple of counters, looks up
  `self+8`'s type in `gActionCtrlStateTable` to fire one `_call_via_r3`
  trampoline call, then writes a small fixed value into `part+0xa` from
  a second, 22-case jump table on the same type.
- **`TryActionCtrlDoubleJump`** (424 B, `action_ctrl_update.c`) - a helper of
  `HandleActionCtrlAirInput`: gated on the D-pad snapshot's bit 1, `self+0x18`'s
  counter being 0, `gLevelState` passing `HasDoubleJump`, and a
  sub-object type of `6`/`0xb`/`0xc` (each with its own `+0x30 >= 0`
  distance-style gate), bumps `self+0x18`, fires the `+0x50`/`+0x54`
  and `+0x20`/`+0x24` trampoline pairs with type-keyed ids, resets the
  trio to a type-keyed value, plays a fixed sound, and returns 1;
  otherwise returns 0.
- **`HandleActionCtrlAirInput`** (576 B, `action_ctrl_update.c`) - a proximity-triggered
  indicator: dispatches `self+8`'s type (`7`/`9`/`0xb`/`0xe`) against
  per-type distance thresholds on `part->field_0x64` (falling back to
  `TryActionCtrlDoubleJump` first when within a `0x27f` threshold), setting
  `part+0xd` bit 0 and firing the `+0x20`/`+0x24` trampoline (id
  `0x1a`), or tail-calling `StartActionCtrlTornadoFall` for type `0xe`. Then, unless
  the type is one of the five gate values, reads the D-pad and remaps
  `self+0x27`'s table-index byte through a further small dispatch
  before a shared tail arming `self+0x31` when the player's `+0x100`
  flag is set.
- **`ApplyActionCtrlMotion`** (560 B, `action_ctrl_idle.c`) - an OAM-visibility/
  priority pass: nudges the player's saved-position word by a fixed
  delta and calls `SetSpritePrevPos` when `part+0x68` is busy and a flag
  just changed; plays a sound and fires the `+0x50`/`+0x54` trampoline
  (id `0x12`) when `self+8==0`; then, keyed on `self+0x2f`, looks up a
  per-tag `gCtrlMotionRecords` record (`(*(self+4))[tag]`), copies 12
  bytes of it to the stack, optionally rescales two fields via
  `FixedMul` (busy part + active player), special-cases tag `0x1e`,
  fires one of two `_call_via_r3` trampoline calls with the stack record
  as payload, and clears the flag; repeats a near-identical sequence
  keyed on `self+0x30`/`self+0x28` against the same table.
- **`ActionCtrlStateIdle`** (664 B, `action_ctrl_idle.c`) - a further sibling:
  reads the D-pad and ticks `self+0x25` down on release; fires the
  `+0x50`/`+0x54` trampoline (id `0x12`) and clears `part+0x33` when
  `part+0x38` is set; bumps `self+0x1c`'s frame counter and, while the
  player's type is `0x12` and `+0x30==0`, fires escalating-id trampoline
  calls once the counter crosses one of two thresholds; bails early if
  `CheckActionCtrlLeftGround(self)` reports busy; otherwise dispatches the input
  snapshot's low bits (sound + two trampoline pairs, or a
  `StartActionCtrlSpin` tail-call, or a `HasTurboRun`-gated trampoline call) -
  every path converging on `UpdatePlayerFacing`.

## Why NAKED, not real C

A first plain-C attempt at `UpdateActionCtrl` (the smallest, best-documented
of the six) compiled logically-equivalent code - every load, store,
branch, and call matched the ROM's own - but diverged at the prologue:
the ROM reserves an unused 8-byte stack slot and pushes a 5th callee-
saved register (`r7`) that the straightforward C translation never
needed (4 registers, no stack). This is the same unexplained-frame-
shape/register-budget gap this exact table family (`gActionCtrlStateTable`)
already hits repeatedly throughout `docs/status/actor.md`'s "Parked -
NAKED transcription" section (`ActionCtrlStateCrawl`, `ActionCtrlStateLand`,
`StartActionCtrlTornadoSpin`, `EndActionCtrlSpin`, `SteerActionCtrlSpin`, `ActionCtrlStateBodySlamStart`,
`ActionCtrlSetTargetAnim`, and the whole `UpdateAirshipFireball`-`ConvertAirshipTiles` boss-weapon
cluster) - a confirmed categorical difficulty for this class of
function, not a one-off. Given all six of this remainder's functions
share the same field-offset/trampoline conventions and jump-table-heavy
dispatch shape as those already-NAKED siblings (and `ActionCtrlHandleEvent` in
particular is the single widest jump-table dispatcher attempted in this
codebase so far - 25 outer cases plus two independent 7-case inner
tables), every one was transcribed instruction-for-instruction from the
ROM disassembly instead, the same escape hatch used for
`MakeLinkHandshakeId`/`ResetLinkSessionState` (`src/link/link_handshake.c`) and
`ActionCtrlStateCrawl` (`action_ctrl_states.c`).

To keep a function this size transcription-error-free, `ActionCtrlHandleEvent`
was transcribed mechanically (a small Python pass converting each ROM
instruction's unified-syntax mnemonic to its divided-syntax form -
`adds`/`movs`/`subs`/`ands`/`orrs`/`lsls`/`lsrs`/`asrs` drop the
trailing `s`, `rsbs Rd, Rs, #0` becomes `neg Rd, Rs` - and wrapping each
line for the `asm()` string) rather than hand-renumbered local labels;
the ROM's own `_0XXXXXXX` hex-address label names were kept verbatim as
plain, file-local asm symbols (safe here since each NAKED function's
`asm()` block assembles into its own object, so these never collide
with any other translation unit) instead of GNU-as numeric local
labels, eliminating the forward/backward-reference bookkeeping a
hand-renumbering pass of this size would otherwise risk getting wrong.

Every function was verified byte-for-byte against `baserom.gba` via an
isolated `arm-none-eabi-as` assemble + `objcopy`/`objdump` comparison
before being wired into the real build (all differences before linking
were relocation placeholders - `bl` targets and literal-pool addresses
showing as zero/section-relative offsets, resolving correctly once
linked - exactly the expected shape for an unlinked single-object
test), and a full clean `make compare` (`rm -rf build && make
NON_MATCHING=1 report`, then `rm -rf build crashbandicootxs.elf
crashbandicootxs.gba crashbandicootxs.map && make compare`) confirms
`La suma coincide` against the real ROM.

## Status

All six of this remainder's functions are now byte-exact, but as NAKED
transcriptions - parked, not matched, per project policy. Every
function GitHub issue #16 originally scoped (this remainder plus the
four matched in `issue-16-actor-12160.md` and the earlier
`KillPlayer`/`UpdateActionCtrlSkidAnim`/`UpdatePlayerFacing`/`CheckActionCtrlLeftGround` matches) is now
either real C or a verified NAKED transcription - nothing from this
issue's original scope is left raw - but since six functions are NAKED
rather than real decompiled C, the issue itself stays open per this
project's "NAKED doesn't count toward closing" convention (see
CONTRIBUTING.md's "Opening the PR").

## Later pass: issue #16 NAKED retry

`ActionCtrlStateIdle` (action_ctrl_idle.c) and `HandleActionCtrlAirInput` (action_ctrl_update.c) are
real C now, under old_agbcc (both files moved to `OLD_AGBCC_OBJS`), on
the `struct act` player/action object from `include/action_obj.h`.
`ApplyActionCtrlMotion`, `UpdateActionCtrl` and `TryActionCtrlDoubleJump` have old_agbcc drafts
under `NON_MATCHING`; `ActionCtrlHandleEvent` wasn't attempted. See
[issue-15-16-naked-retry.md](issue-15-16-naked-retry.md).


## Later pass (third issue #15/#16 NAKED retry)

`UpdateActionCtrl` and `ApplyActionCtrlMotion` are real C under old_agbcc now. See
[issue-15-16-naked-retry-3.md](issue-15-16-naked-retry-3.md).

## Later pass (second big NAKED retry)

`ActionCtrlHandleEvent` is real C under old_agbcc now (`action_ctrl_event.o` joined
`OLD_AGBCC_OBJS`). See [big-naked-retry-2.md](big-naked-retry-2.md).
