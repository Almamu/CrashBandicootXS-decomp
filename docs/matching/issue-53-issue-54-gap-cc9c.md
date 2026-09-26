# The 0x0802CC9C-0x0802D3A8 gap between issues #53 and #54 (actor)

A scoping investigation of the actor zone found this whole 1804-byte
gap - the remainder of `asm/code_3_2_20_28568_c99c_cc9c.s`, right after
issue #53's own matched batch (`sub_802CC78`, `src/graphics/
actor_part19i.c`) and right before issue #54's chunk (`sub_802D3A8`,
`src/graphics/actor_part62.c`) - still completely raw.
`docs/matching/issue-53-actor-c7a8.md`'s "What's left" section had
already flagged `sub_802CC9C` onward as "a larger,
`sub_802DD9C`/`sub_802A6EC`/`sub_802B7E0`-calling state machine ... not
attempted this pass".

13 functions total, all on the same `InitActorPart`/`gUnknown_03000884`-
rooted "self" object family documented throughout `actor_part17.c`-
`actor_part19i.c` (a "part table" pointer at `self+0`, a table-index/
"kind" field at `self+0xc`, an anim-frame halfword/byte pair at
`self+0x10`/`self+0x12`, an accumulator at `self+8`, state at
`self+0x28`, a frame counter at `self+0x44`, the position triple at
`self+0x1c`/`self+0x20`/`self+0x24`, and a `self+0x50`-rooted event/
trampoline table).

## Parked - NAKED transcription (byte-correct, not decompiled)

- **`sub_802CC9C`** (`src/graphics/actor_part126.c`) - a per-frame
  hazard/proximity state machine: latches `self+0x2c` once `self+0x34`
  (a cached depth) exceeds `0x15FF`. If not already "used" (`self+0xc
  == 0`), snapshots the owning part table's own `+0x14` 12-byte record
  into `self+0x38` and runs `sub_802DD9C`'s player-overlap test against
  it, transitioning to "used" (index 1) on a hit; then, regardless,
  re-snapshots one of three static 12-byte `gStaticData_0817A7xx`
  records into `self+0x38` and probes `sub_802A6EC` against each in
  turn (the first via `sub_802B7E0` on the player object, the other two
  plain proximity), each on a hit also transitioning to "used". Once
  already "used" (`self+0xc != 0`), skips all of that and just fires the
  `self+0x50` trampoline (index 3) while `self+0x12` is set, or falls
  back to `sub_802A7B8` - the exact same "trampoline-or-`sub_802A7B8`"
  tail idiom `sub_802C4C8` (`actor_part19g.c`) uses for the sibling
  object family, just inlined here directly instead of shared via a
  helper call (this object's own "already used" sentinel is a plain `1`,
  not the `0x12` value `sub_802C4C8`'s family uses, so the two can't
  literally share code).

  Every one of the three 12-byte record snapshots reuses the same
  `self+0x38` scratch pointer (kept live in `r6`) across an intervening
  `sub_802A6EC`/`sub_802DD9C`/`sub_802B7E0` call, with `r5`/`r7` each
  also switching roles (old state, then a "confirmed zero" reused for
  every reset block's `self+0x44`/`self+8` clear) mid-function - the
  exact heavy register-reuse family this project has already NAKED-
  parked for `sub_802D7B0`/`sub_802DD9C`
  (`docs/matching/issue-54-actor-d3a8.md`) and `sub_802C7A8`
  (`docs/matching/issue-53-actor-c7a8.md`). Semantics are fully
  understood; transcribed instruction-for-instruction from the ROM
  disassembly rather than chased further at the C level, per this
  project's established escape hatch for this exact register-pressure
  shape. Every original address label was renumbered to a unique GNU-as
  local numeric label in strict order of appearance - every reference in
  this function turned out to be a forward reference (`Nf`), since every
  label used here is either a forward branch target or a literal-pool
  entry placed after the code that loads it.

## Matched (12 of 13 functions)

All in `src/graphics/actor_part126.c`.

- **`sub_802CDE4`** - `InitActorPart`-based constructor: forwards
  `a`/`b`/`c`/`d` straight through, installs `self+0x50 =
  gStaticData_087E4FB4`, and clears the `self+0x2c` one-shot flag.
- **`sub_802CE10`** - on the `sub_802A6EC` trampoline-fire edge,
  forwards to `sub_802B730(gUnknown_03000884)` (the player object),
  discarding its result; always tail-calls `sub_802A7B8`. Needed
  `sub_802B730`'s extern declared as returning `u8` (not `s32`) even
  though the result is discarded here - the ROM's own boolean check
  at this call site (`lsls r0,r0,0x18; cmp r0,#0`, no accompanying
  `lsrs`) only appears when the callee's return type itself is
  byte-sized, matching `sub_802CF30`'s identical call site below.
- **`sub_802CE38`** - same `InitActorPart`-based constructor shape as
  `sub_802CDE4`, minus the `self+0x2c` clear, `self+0x50 =
  gStaticData_087E4FD4`.
- **`sub_802CE5C`** - 3-way `self+0x28` state dispatch, written with
  explicit `goto`s to a `case0`/`case1`/`done` label set matching the
  ROM's own three-way `beq`/`beq`/`b` dispatch at the top (an
  `if`/`else if` chain instead compiles to an inverted `bne`/`bne`
  pair, a different byte sequence even though behaviorally identical).
  State 0: on `sub_802A6EC`'s fire edge, calls
  `sub_802C14C(gUnknown_03000884)`, plays a cue, and transitions to
  state 1/table-index 1; otherwise, on `sub_802DD9C`'s overlap test,
  transitions the same way. State 1: once `self+0x12` fires, dispatches
  the `self+0x50` trampoline (index 3) instead of the usual
  `sub_802A7B8` fallback. `state` pinned to `r5` and the
  `sub_802A6EC`/`sub_802DD9C`-fired boolean pinned to `r6`, each reused
  directly as the "confirmed zero" for that branch's own `self+0x44`/
  `self+8` stores (matching the ROM's own register reuse) - without
  these pins, both branches' otherwise-identical `PlaySfx`+reset
  sequences get cross-jump-merged by this compiler into one shared
  tail the ROM's own build never has (the ROM duplicates the whole
  sequence twice, once per branch, each with its own register).
- **`sub_802CF0C`** - same `InitActorPart`-based constructor shape as
  `sub_802CE38`, `self+0x50 = gStaticData_087E4FF4`.
- **`sub_802CF30`** - applies `self`'s own velocity
  (`self+0x54`/`0x58`/`0x5c`) to its position; while idle (`self+0x28 ==
  0`), counts down `self+0x60`, re-deriving a fresh velocity/homing
  target via `sub_802D044` once it expires, then probes
  `sub_802A6EC`+`sub_802B730` or `sub_802DD9C` - either hit re-arms a
  fixed outward X velocity (biased by `self+0x1c`'s sign), a random
  negative Y kick, bumps `self+0x5c`, plays a cue, and transitions to
  state 1/table-index 0. `state` (`self+0x28`'s old value) pinned to
  `r6`, the `sub_802A6EC` result pinned to `r5`, each reused as the
  "confirmed zero" the same way as `sub_802CE5C` (and for the same
  cross-jump-merge reason). The Y-kick computation needed
  `-(s32)(u16)sub_8000E1C(0x300)` (an explicit 16-bit zero-extend
  before negation) to reproduce the ROM's `lsls #0x10; lsrs #0x10;
  negs` triple - a plain `-sub_8000E1C(0x300)` silently drops the
  zero-extend since `sub_8000E1C` already returns `s32`.
- **`sub_802D044`** - homing-velocity (re)initializer: with a negative
  `target` index, arms a fixed slow downward drift (constants into
  `self+0x54/0x58/0x5c/0x60`); otherwise derives a per-frame speed
  factor (`sub_803ADB4` of `target`'s own "speed" record,
  `gUnknown_0300088C[sub_802A570(target)]`, against the remaining Z
  distance) and scales the X/Y deltas toward `target`'s tracked
  position by that factor. Two gotchas: (1) `gUnknown_0300088C[idx]`
  needed `idx` computed as its own statement (`s32 idx =
  sub_802A570(target);`) *before* the array index expression, or this
  compiler hoists the array's base-address load ahead of the
  `sub_802A570` call instead of after it, matching the ROM's own
  call-then-load order; (2) the `sub_803ADB4` distance argument needed
  to be re-read fresh from `self+0x5c` (`*(s32 *)(self + 0x5c)`) rather
  than reusing the `speed` local already holding the same value - the
  ROM redundantly reloads it from memory instead of reusing the
  cached register.
- **`sub_802D0C8`** - `InitActorPart`-based constructor forwarding
  `a`/`b`/`c`/`d` plus a 6th argument `e` (a pointer): installs
  `self+0x50 = gStaticData_087E5014`, then calls `sub_802D044(self,
  e->0x10)`.
- **`sub_802D0F4`** - on the trampoline-fire edge, forwards to
  `sub_802B730` on the player object; then, gated on `self+0x34`'s
  cached depth crossing one of two thresholds paired with `self+0x28`'s
  current tier, advances `self+0xc`'s table index and, once
  `GetAnimFrameBaseOffset` reaches the new record's own threshold,
  clears `self+8` (deep-tier variant also bumps `self+0x28`). Both
  advance blocks converge on one shared `self+0x28 += 1` tail reached
  by `goto` - written as `goto increment;`/`goto tail;` labels rather
  than duplicating the increment, matching the ROM's own single shared
  copy. The depth-threshold comparisons needed the constant
  materialized into a register *before* the depth load
  (`register s32 threshold1 asm("r0") = 0x6400;` declared and assigned
  ahead of `depth`) and written as `depth > threshold1` (not `threshold1
  < depth`) to get both this compiler's instruction-scheduling order
  and its `cmp`/branch-polarity choice to match the ROM's `movs+lsls`-
  then-`ldr`-then-`cmp r1,r0; ble` sequence. The post-call idx/table
  reload for the threshold check needed its own `idx2`/`table2` pair
  pinned to `r2`/`r3` (distinct from the first use's `r1`-pinned
  `idx`), matching the ROM's own register split between the two
  otherwise-identical address computations.
- **`sub_802D1B8`** - `InitActorPart`-based constructor: forwards
  `self`/`c`/`d`/`e` straight through plus `b` (a `u8 *`, passed to
  `InitActorPart` as a bare `s32` via cast - the same generic-argument
  overload already established for other constructors in this family);
  classifies a "kind" (`self+0xc`) from `*b` (bumped by 1 if `c > 0`),
  scaled `*4 - 0x40`. The pointer dereference (`*b`) had to happen
  *after* the `InitActorPart` call (not cached into a local beforehand)
  to match the ROM's own deferred-dereference order, and `kind` needed
  pinning to `r1` to match the ROM's specific register choice for the
  table-address multiply.
- **`sub_802D204`** - VRAM-gauge/state-transition driver for a
  `gUnknown_030014B8`-counted effect: while the current hazard tier
  (`gUnknown_030012C0->0x78`) and the `retrigger` argument are both
  zero, just clears `self+0x2c`; otherwise DMAs one of four
  `gStaticData_0817A798`-indexed gauge strips and resets `self`'s table
  index/anim, arming `self+0x2c`. Then: tier 3 arms a long
  `gUnknown_030014B8` countdown and transitions to state 1; tier 0 with
  `retrigger` set transitions to state 2/table-index 1 instead; any
  other combination clears `gUnknown_030014B8` and, if `self+0x28` was
  already non-zero, resets `self` back to state 0/table-index 0 - the
  tier-3 branch and this last "reset" branch converge on one real
  shared tail block (not duplicated C - both fall through to the same
  trailing statements, and this compiler naturally cross-jump-merges
  the resulting identical code back into one copy, matching the ROM's
  own single shared block). `retrigger` needed to stay a *plain* local
  (no explicit `register asm("r7")` pin) - explicitly pinning it hit
  this project's confirmed "r7 cannot be pinned in this toolchain, ever"
  bug (spilling it to the stack via `mov r1, sp` instead of keeping it
  in r7, and dropping r7 from the prologue's push/pop list); left
  unpinned, the natural register-pressure from `self`/`tier`/`zero`
  already occupying `r4`/`r5`/`r6` pushes this compiler's own allocator
  into `r7` for it anyway, correctly included in the push/pop list.
  Several small constant-materialization-order gotchas throughout
  (address-then-value vs value-then-address for simple global stores,
  and a two-constant "state store" needing both registers materialized
  before either store) needed matching one at a time against the ROM
  disassembly.
- **`sub_802D2DC`** - drives `gUnknown_030014B8`'s countdown, DMAing one
  of two gauge strips per frame and, once it expires, resetting the
  hazard tier via `sub_80231EC(gUnknown_030012C0, 2)` then
  `sub_802D204(self, 0)`; independently re-fires `sub_802D204` once
  state 2's own `self+0x12` edge trips; always advances `self`'s own
  anim frame (`sub_802A980`, frame-counter bump, and the usual
  wrap-around `GetAnimFrameBaseOffset` check). The final table-address
  computation (`idx*0xc + table`) needed an opaque
  `asm("add %0, %0, %1")` to pin the addition's operand order - even
  with `idx`/`table` each pinned to their own ROM-matching registers,
  this compiler still canonicalized the commutative add with the
  operands in its own preferred order, one byte-identical but
  differently-encoded instruction (`adds r1,r3,r1` instead of the
  ROM's `adds r1,r1,r3`).

## Verification

Full clean rebuild confirmed (`rm -rf build crashbandicootxs.elf
crashbandicootxs.gba crashbandicootxs.map && make compare`):
`crashbandicootxs.gba: La suma coincide`. `rm -rf build && make
NON_MATCHING=1 report` also ran clean, no warnings for
`actor_part126.c`.

See [docs/status/actor.md](../status/actor.md) for the running
matched/parked list this entry feeds into.
