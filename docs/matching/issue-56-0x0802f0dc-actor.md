# Issue #56: 0x0802F0DC-0x0802FBF0 (actor)

25-function `decomp-chunk` covering a second boss-weapon "spawn/pre-
attack" singleton and its "self" object - the same large per-instance
object family already documented in `actor_part17.c`/`actor_part18.c`/
`actor_part19.c`/`actor_part20.c` (state at `self+0x28`, a table-index
at `self+0xc`, an anim-frame halfword/byte pair at `self+0x10`/
`self+0x12`, an accumulator at `self+8`, a "part table" pointer at
`self+0`, and here also an event/trampoline table pointer at
`self+0x50`), but driving a *different* singleton than issue #58's
`gUnknown_03001534` cluster and issue #62's `gUnknown_030015AC`
cluster - this one's flags/counters live at `gUnknown_030014E0`-
`gUnknown_03001518`.

The chunk generator's listed source file (`asm/code_3_2_17.s`) was
stale by the time this issue was picked up - the real raw bytes for
this address range lived in `asm/code_3_2_20_28568_c99c.s` (itself a
split-off fragment of `code_3_2_20_28568.s`, the same file issue #58's
boss-weapon chunk continues from). That file has now been split
further at each matched/parked/raw boundary in this chunk, following
this project's usual "cut at the boundary" convention - the fragments
are named by the lower 5 hex digits of their first function's address
(`code_3_2_20_28568_c99c_2f164.s`, `..._2f338.s`, `..._2f748.s`,
`..._2f7b0.s`, `..._2f97c.s`, `..._2fa04.s`, `..._2fa38.s`,
`..._2fbf0.s`).

## Matched (22 of 25 functions)

- **`sub_802F338`** (`src/graphics/actor_part43b.c`) - computes two
  keyframe-driven tile-cache sizes (`byte0*byte1`, scaled by 32) via
  `AllocVramTileBlock`, storing them into the `gUnknown_03001518` pair;
  now fully matched as real C. The ROM's "materialize the multiply
  result into one register, copy it to a second, *then* shift" idiom
  (`adds r2,r3,#0; muls r2,r1,r2; adds r0,r2,#0; lsls r0,r0,#5`) closes
  via an opaque `asm volatile` forcing the exact register-to-register
  copy this compiler's dead-store elimination always collapsed.
  Closing that gap surfaced a further chain of register-role
  mismatches in the shared index/address computation: `table` needed
  an explicit early load into `r3` (a plain unpinned local loaded it
  too late), the `+2` index constant needed to be materialized via an
  opaque `mov #2` immediately before the `ldrsh` (a `register`-pinned
  local alone has no effect here since gcc constant-folds the literal
  and freely picks its own register for it), the `table + idx*3*4`
  addition needed its operand order pinned via `asm volatile("add %0,
  %0, %1" ...)`, and the two blocks' final byte-load pairs
  (`rec[0]`/`rec[1]`) needed per-block register pins matching the
  ROM's own `ldrb` register choices (which differ between the two
  otherwise-identical blocks). The old raw
  `asm/code_3_2_20_28568_c99c_2f338.s` is retired.
- **`sub_802F164`** (`src/graphics/actor_part43.c`) - state-machine
  update for the same singleton: while `self+0x28` is one of the
  "active" states (1/6/2/3), resets `self`'s table index/anim to the
  idle frame if it wasn't already, latches the target position at
  `self+0x1c`/`self+0x20` from the two arguments, and re-arms state 6
  (playing a cue only on the *first* transition into it). While the
  current game-mode flag at `gUnknown_030012C0+0x8c` is clear and the
  `gUnknown_030014EC` frame-timer has advanced far enough, drives a
  5-case round-robin (`gUnknown_030014F0`) once every >0xbe-frame
  window via a real `switch` on a dense 0-4 case set - the switch's
  own generated bounds check turned out to be exactly the ROM's own
  `cmp r0, #4; bls ...; b ...` pair, so no separate guard `if` was
  needed (an explicit `if (v <= 4) switch (v) {...}` duplicated that
  bounds check into two, since a bare global read isn't CSE'd with a
  switch's own re-read of the same global). The reset block's
  "materialize a fresh anim halfword/zero-byte/zero-word triple" idiom
  and two of the trailing global zero-stores needed the same
  register-pinned-block idiom already established throughout
  `actor_part43.c`/`actor_part44.c` (explicit `r0`/`r1`/`r2`/`r3`/`r4`
  pins matching the ROM's own scratch-register choices, including a
  pinned pointer-typed pair - `r2`=`&gUnknown_03001508`,
  `r1`=`&gUnknown_0300150C` - to get the ROM's specific "load both
  addresses before either store" scheduling instead of this compiler's
  default "reload the same register per store" pattern); the case-3
  accumulator-clamp arm needed its cap value re-read through the same
  pinned pointer *after* the `sub_803ADB4` call (a fresh `register s32
  cap asm("r4") = *maxPtr;` declared after the store, not before) to
  match the ROM's own redundant post-call reload rather than reusing
  the pre-call value.
- **`sub_802F0DC`** (`src/graphics/actor_part43.c`) - constructor/
  reset: while the singleton flag (`gUnknown_03001506`) is off, resets
  `self` to state 5/table-index 4, plays a cue, and conditionally
  fires an extra one-shot effect via `sub_8022EA8`. Matched with the
  established "cache the known-zero value in a register, front-load
  sibling constants before either store, reset the anim frame via a
  `register`-pinned halfword/byte pair" idioms already used throughout
  `actor_part20.c`'s family - no new gotchas.
- **`sub_802F3BC`** (`src/graphics/actor_part44.c`) - accumulator-
  drain/reward-dispenser for the `gUnknown_030014FC` accumulator
  `sub_802F540` fills: while the singleton flag is set, fully drains
  it via repeated `sub_8023430` calls; otherwise, once a
  `gUnknown_030014F8` cooldown elapses, dispenses one of four tiers of
  reward sized by the accumulator's own magnitude. Needed `self`
  explicitly pinned to `r1` - this compiler's default allocation put a
  redundant `self`-into-`r5` copy at function entry (for the one
  tier-4 case only) that the ROM never has, since the ROM keeps `self`
  in `r1` uniformly across all four tiers.
- **`sub_802F46C`** (`src/graphics/actor_part44.c`) - trivial
  pre-increment counter accessor (`return ++gUnknown_030014E0;`).
- **`sub_802F47C`** (`src/graphics/actor_part44.c`) - threshold check
  on `self+0x54`'s accumulator against `gUnknown_030014E4`'s cap.
  Needed the two-condition guard folded into one `if (r == 0 && v > 0)
  r = 1; return r;` rather than two early `return`s - the naive
  two-`return` form makes this compiler synthesize a spurious
  `mov r0, #0` for the "value already zero" fallthrough case that the
  ROM never emits (it just falls through with `r0` already zero).
- **`sub_802F4AC`** (`src/graphics/actor_part44.c`) - forwards
  `self+0x24` (z position) plus a fixed offset to `sub_8029748`,
  discarding the result; declared `void` (not `s32`) so this compiler
  reuses `r0` for the `pop {r0}; bx r0` epilogue instead of preserving
  a return value the ROM itself discards the same way.
- **`sub_802F4C0`** (`src/graphics/actor_part44.c`) - trivial byte
  getter for `gUnknown_030014E8`.
- **`sub_802F4CC`** (`src/graphics/actor_part44.c`) - countdown timer
  (`gUnknown_030014F4`) driving a palette-strip animation refresh,
  ping-ponging the frame index via `sub_803ADB4` the same way
  `sub_8031744` (actor_part26.c) does for its own strip.
- **`sub_802F50C`** (`src/graphics/actor_part44.c`) - advances
  `self+0x54`'s accumulator by a scaled `delta`, clamped to
  `gUnknown_030014E4`'s cap. Needed the multiply written as
  `delta * max` (not `max * delta`) - the classic "which operand goes
  first" gap - to get this compiler to pre-load `delta` into the
  multiply's destination register the way the ROM does.
- **`sub_802F540`** (`src/graphics/actor_part44.c`) - feeds `delta`
  into the `gUnknown_030014FC` reward accumulator, arming its
  `gUnknown_030014F8` cooldown the first time it goes from zero. Its
  own `self` parameter is genuinely unused (dead) in the ROM.
- **`sub_802F570`**/**`sub_802F69C`** (`src/graphics/actor_part44.c`)
  - twin "if a threshold/flag trips, reset `self` to an idle
    state-1/table-index-0 transition and arm the singleton's flags"
  idioms, matched with `self` pinned `r2` and the established
  register-pinned reset-block idiom.
- **`sub_802F5AC`** (`src/graphics/actor_part44.c`) - two independent
  one-shot transitions on `self`: a table-index-5 idle reset gated on
  `self+0x12`, and a separate `self+0x44`-counter-driven state-1
  transition.
- **`sub_802F5E4`** (`src/graphics/actor_part44.c`) - advances
  `gUnknown_03001508`'s bounded oscillator by 9 (clamped to +0x140 by
  absolute value via the standard `sign = v>>31; v ^= sign; v -=
  sign;` idiom, reusing `v`/`sign` in place rather than a separate
  `abs` local), then fires two one-shot threshold effects on
  `self+0x20`.
- **`sub_802F640`** (`src/graphics/actor_part44.c`) - advances
  `self+0x24` by a fixed step, derives `self+0x34` (a camera-relative
  depth) via `sub_8029B2C`, and fires the same one-shot threshold pair
  as `sub_802F5E4`. Needed `self+0x24` re-read fresh from memory after
  the `sub_8029B2C()` call (rather than keeping the pre-call value in
  a local) to match the ROM's own redundant reload.
- **`sub_802F6DC`** (`src/graphics/actor_part44.c`) - teardown/
  destructor: marks `self` "dying", drains the reward accumulator,
  frees the two keyframe-size tile allocations `sub_802F338` made,
  marks `self` fully "dead", unlinks it from its doubly-linked list
  (via two independent double-dereference statements, not cached
  `prev`/`next` locals - the ROM redundantly reloads `self+0x48`/
  `self+0x4c` a second time rather than reusing the first load), and
  optionally frees it. Needed `self`/`flags` each given an explicit
  plain local copy (`u8 *self = selfArg; s32 flagsReg = flags;`, no
  `register asm` pins) to get this compiler to home `r0` before `r1`
  at entry *and* to keep `flagsReg` in a properly saved/restored `r7`
  - an explicit `register ... asm("r7")` pin for the same variable
  compiles but silently drops `r7` from the prologue's `push`/`pop`
  list entirely (this compiler doesn't treat a plain low-register pin
  as needing callee-save unless another high register is also live in
  the function, the same quirk documented in `actor_part35.c` for
  `sub_8033CF8`) - a real correctness gap this plain-local-copy form
  avoids by letting the compiler make its own (correct) callee-save
  decision.
- **`sub_802F7A4`** (`src/graphics/actor_part45.c`) - trivial byte
  getter for the singleton's own flag.
- **`sub_802FA34`** (`src/graphics/actor_part46.c`) - trivial
  constant-true predicate.
- **`sub_802F97C`** (`src/graphics/actor_part45b.c`) - a physics-step-
  and-collision-react updater: advances `self`'s position by its
  velocity pair plus a fixed gravity-like Y offset and a fixed Z step,
  then reacts to a `sub_802A3AC` collision probe - firing a trampoline
  on the hit object if any, else checking `sub_8031378` (an AABB
  overlap test) and a `self+0x34` depth threshold before firing
  `self`'s own `self+0x50`-table trampoline (index 8, no NULL-guard on
  that specific call) or, once past the threshold, falling back to
  `sub_802A7B8` (also no NULL-guard). Two closing fixes over the prior
  parked attempt: (1) the threshold check needed to be a plain
  `if (cond) {...} else {sub_802A7B8(...);}`, with the shared tail
  reached by `goto`s landing on a `merge:` label *inside* the `if`
  body, rather than an early-returning `else if` - this compiler places
  an `if`'s `else` body last in program order but an early-returning
  `else if` chain's next statement first, silently relocating the
  `sub_802A7B8` call relative to the shared tail even though every
  individual instruction already matched; (2) the ROM's inconsistent
  (`r2` vs `r3`) scratch-register choice for the repeated `8` immediate
  needed a small inline-asm anchor per site (`asm volatile("mov rN, #8\n\tldrsh
  %0, [%1, rN]" : "=r"(off) : "r"(table))`) - critically with **no**
  clobber list (adding one, even naming the register the asm text
  already hardcodes, was enough extra register pressure to make this
  compiler spill a second copy of `self` into `r5`, which the earlier
  attempt's plain `register asm("r2")`/`register asm("r3")` pins likely
  triggered too). A related, non-obvious discovery: with the two fixes
  above in place, this compiler *still* produced 4 extra bytes (a
  spilled `r5` copy of `self`) as long as `self` was a separately
  declared local (`u8 *self = selfArg;`, this file's usual pattern) -
  only typing the parameter itself as `u8 *self` (dropping the
  `void *selfArg` indirection entirely) got the byte-exact result. Since
  nothing calls `sub_802F97C` by name (only indirectly through a
  `void *`-typed function-pointer table entry), the parameter's own
  type here doesn't need to match the project's usual convention.
- **`sub_802FA04`** (`src/graphics/actor_part45c.c`) - an
  `InitActorPart`-based constructor for this cluster's `self` object:
  forwards its first three real arguments plus one stack argument
  straight to `InitActorPart`, then marks `self+0x54` = 1, sets
  `self+0x50`'s event/trampoline table to `gStaticData_087E517C`, and
  stashes its remaining two stack arguments into `self+0x58`/`self+0x5c`;
  now fully matched as real C, closing the gap the same 7-argument
  `InitActorPart`-wrapper shape is still parked on for `sub_80305F8`
  (`docs/matching/issue-58-0x08030334-actor.md`). Explicitly pinning `e`/
  `f` to their ROM registers (`r6`/`r7`) either adds a spurious extra
  `r8` push/pop (a relay attempt) or - for `r7` specifically - drops that
  register from the compiler's own prologue push/pop list outright (a
  genuine agbcc/gcc 2.9 Thumb-prologue bug, not just a missed
  optimization). The fix: pin only the constant `1` to `r5`; leave
  `self`, the stack argument `d`, and both `e`/`f` completely unpinned
  (`self` as a plain `u8 *` local, `d` used directly as `InitActorPart`'s
  stack argument, `e`/`f` as plain `register` locals with no explicit
  hardware register). With that much natural register pressure, this
  compiler's own allocator picks `r4`/`r6`/`r7` for `self`/`e`/`f` on its
  own - correctly including all of `r4`-`r7` in the push/pop list - and,
  in the declaration order `self`, then the pinned constant, then `e`,
  then `f`, schedules the loads in the ROM's own self/d/e/f/constant
  order. Retires the old raw `asm/code_3_2_20_28568_c99c_2fa04.s`.

## NAKED transcription (byte-correct, not counted as matched)

- **`sub_802F748`** (`src/graphics/actor_part44b.c`) - a
  `gStaticData_0817C1C0` stride-8 trampoline-record dispatcher, same
  shape as `sub_802C208` (issue #52). Hits the same confirmed
  categorical gcc-2.9 r7-pin bug and is transcribed the same way - see
  docs/matching/issue-52-0x0802bed8-actor.md for the full account. The
  built ROM is byte-identical at this address, but a NAKED
  transcription of a substantial function doesn't count as "matched"
  under this project's current tracking policy, so
  `tools/report_units.py` keeps this address's `base_object` as `None`.
- **`sub_802FA38`** (`src/graphics/actor_part46b.c`) - a
  ~150-instruction function combining a position update (via
  `self+0x60`/`0x64`/`0x68` velocity-like fields), a `self+0x2c`
  threshold flag, a `gStaticData_0817C260` stride-8 keyframe-table
  lookup/`sub_803AD84` dispatch, and a player-distance/push-out damage
  calculation (`sub_803ADB4`-scaled deltas feeding `sub_802E674`) plus
  a `self+0x7c`-gated `sub_802A6EC`/`sub_803AD80` trampoline pair.
  Fully understood and every load/store, branch and call transcribed
  is confirmed correct; parked because the keyframe-table lookup is
  the exact same categorical r7-hazard shape as `sub_802C208`/
  `sub_802F748`/`sub_8030574` (the ROM keeps the table's base address
  alive in `r7` for the whole function - see those entries), and the
  damage-calculation block that follows compounds this with `r8`/`sb`
  register pressure held live across two `sub_803ADB4` calls and a
  `sub_802E674` call - no C-level technique (register-variable pins,
  local-copy barriers, splitting into helper calls) reached this exact
  allocation without either losing the ROM's registers or
  reintroducing the r7 hazard.
- **`sub_802F7B0`**/**`sub_802F8E8`** (`src/graphics/actor_part45d.c`)
  - a pair of ~130-170-instruction VRAM tile-remap loops (4-bit
  palette-index repacking into a `0x0600D000`-based tile buffer via
  raw `REG_DMA3SAD`/`DAD`/`CNT` pokes at `0x040000D4`), each with three
  high registers (`r8`, `sb`, `sl`) simultaneously live across a nested
  loop. Fully understood and every load/store, branch and call
  transcribed is confirmed correct; parked because every other
  DMA3-setup function in this codebase with the same
  `0x040000D4`/`0x0600D000` literal-pool shape (`actor_part26b.c`,
  `actor_part74.c`, `actor_part75.c`, `fade_screen_mode.c`,
  `hud_digit_array.c`, `settings_menu8e.c`, `timer_util_aa90.c`) is
  NAKED too - this compiler's register allocator never reproduces the
  ROM's specific three-high-register nested-loop allocation for this
  shape, and the two calls this project's usual register-pin idioms
  rely on (`sub_8029AC4`, called mid-loop-setup) leave no slack to pin
  three high registers across the loop body without the compiler
  spilling or reordering something else.

## Left raw (0 of 25 functions)

All 25 functions in this chunk are now either matched or parked
(`NON_MATCHING` or NAKED); none are left raw.

See [docs/status/actor.md](../status/actor.md) for the running
matched/parked list this entry feeds into.
