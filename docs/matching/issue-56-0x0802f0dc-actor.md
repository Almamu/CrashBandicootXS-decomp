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

## Matched (17 of 25 functions)

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

## Parked (4 of 25 functions, `NON_MATCHING`, not yet byte-exact)

- **`sub_802F338`** (`src/graphics/actor_part43b.c`) - computes two
  keyframe-driven tile-cache sizes (`byte0*byte1`, scaled by 32) via
  `sub_8028CD4`, storing them into the `gUnknown_03001518` pair.
  Semantics fully understood and every load/store, branch and call
  confirmed correct - both keyframe-size sub-blocks are literally
  identical computations, matching the ROM's own duplication; parked
  on a residual "materialize the multiply result into one register,
  copy it to a second, *then* shift" gap (the ROM computes
  `byte0*byte1` into one register, copies it to a second, then shifts:
  `adds r2,r3,#0; muls r2,r1,r2; adds r0,r2,#0; lsls r0,r0,#5`) that
  this compiler's dead-store elimination always collapses into a
  shorter compute-and-shift-in-place sequence - separate locals, a
  `register`-pinned intermediate, and an `asm("" :: "r"(...))` barrier
  were all tried and none reproduced the extra copy without either
  eliminating it differently or collapsing the copy-then-shift into a
  single differently-encoded shift-with-distinct-registers instruction.
- **`sub_802F748`** (`src/graphics/actor_part44b.c`) - a
  `gStaticData_0817C1C0` stride-8 trampoline-record dispatcher -
  exactly the same shape as the already-parked `sub_802C208`
  (`src/graphics/actor_part19e.c`, issue #52): `{s16 baseOff; s16
  count; s16 subOffset}` records indexed by `self+0x28`'s state; when
  `count > 0`, indexes a per-instance list pointer at
  `self+subOffset` and reads its last entry's `{s32 delta; void *fn}`
  pair; otherwise falls back to the record's own inline `{..; void
  *fn}` pair. Fires `sub_803AD84(self+addr, baseOff, count, fn)`.
  Every load/store, branch and call confirmed correct; parked on the
  same register-allocation/instruction-scheduling gap around the two
  `record = base + state*8` re-derivations documented for
  `sub_802C208`.
- **`sub_802F97C`** (`src/graphics/actor_part45b.c`) - a physics-step-
  and-collision-react updater: advances `self`'s position by its
  velocity pair plus a fixed gravity-like Y offset and a fixed Z step,
  then reacts to a `sub_802A3AC` collision probe - firing a trampoline
  on the hit object if any, else checking `sub_8031378` (an AABB
  overlap test) and a `self+0x34` depth threshold before firing
  `self`'s own `self+0x50`-table trampoline (index 8) or falling back
  to `sub_802A7B8`. Every load/store, branch and call confirmed
  correct, including the ROM's exact duplicate-but-differently-
  scheduled `self+0x50`-table lookup reached from two different arms
  (a real "shared tail, two entry paths" shape reproduced here with an
  explicit `goto tail;`); parked on a residual register choice (`r2`
  vs `r3`) for the `8` immediate in those two lookups that this
  compiler allocates the opposite way round from the ROM, and did not
  budge under explicit `register asm("r2")`/`register asm("r3")` pins
  at each site.
- **`sub_802FA04`** (`src/graphics/actor_part45c.c`) - an
  `InitActorPart`-based constructor for this cluster's `self` object:
  forwards its first three real arguments plus one stack argument
  straight to `InitActorPart`, then marks `self+0x54` = 1, sets
  `self+0x50`'s event/trampoline table, and stashes its remaining two
  stack arguments into `self+0x58`/`self+0x5c`. Every load/store and
  call confirmed correct - the same 7-argument `InitActorPart`-wrapper
  shape already left raw as `sub_80305F8`
  (`docs/matching/issue-58-0x08030334-actor.md`); parked because this
  compiler always pushes only as many high registers (`r4`-`r7`) as it
  independently decides it needs for its own constant/stack-argument
  evaluation order, never matching the ROM's specific
  `r4=self,r5=1,r6=e,r7=f` assignment (and its 4-register push/pop)
  without either an incorrect extra `r8` push/pop (a relay attempt)
  or losing the stack argument's value outright to a register
  collision with an explicit `r7` pin.

## Left raw (4 of 25 functions, not attempted this pass)

- **`sub_802F164`** (`asm/code_3_2_20_28568_c99c_2f164.s`) - a
  ~160-instruction state-machine update for the singleton, including a
  `mov pc, r0` computed-goto 5-case jump table on
  `gUnknown_030014F0`. Semantics are largely readable (state
  transitions gated on `self+0x28`, timing checks via `sub_802A4D4`
  against `gUnknown_030014EC`, and per-case accumulator/threshold
  manipulation of `gUnknown_030014FC`/`gUnknown_030014F8`/
  `self+0x54`), but the size and the jump-table reconstruction were
  out of scope for this pass; left untouched in the raw `.s` file.
- **`sub_802F7B0`**/**`sub_802F8E8`**
  (`asm/code_3_2_20_28568_c99c_2f7b0.s`) - a pair of ~130-170-
  instruction VRAM tile-remap loops (4-bit palette-index repacking
  into a `0x0600D000`-based tile buffer via `REG_DMA`-adjacent hardware
  writes), each with heavy `sb`/`sl`/`r8` register pressure across a
  nested loop. Not attempted this pass given the size of the chunk.
- **`sub_802FA38`** (`asm/code_3_2_20_28568_c99c_2fa38.s`) - a
  ~150-instruction function combining a position update (via
  `self+0x60`/`0x64`/`0x68` velocity-like fields), a `self+0x2c`
  threshold flag, and a player-distance/push-out damage calculation
  (`sub_803ADB4`-scaled deltas feeding `sub_802E674`) plus a
  `self+0x7c`-gated `sub_802A6EC`/`sub_803AD80` trampoline pair - heavy
  `sb`/`r8` register pressure throughout. Not attempted this pass.

See [docs/status/actor.md](../status/actor.md) for the running
matched/parked list this entry feeds into.
