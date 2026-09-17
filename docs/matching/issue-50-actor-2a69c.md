# Issue #50: 0x0802A69C-0x0802AC28 (actor)

**Naming note:** these files are numbered `actor_part50`-`56` rather
than `actor_part39`-`45` (which would have matched their creation
order more naturally) because issues #16 and #56's parallel PRs
independently claimed `actor_part39.c` and `actor_part43.c`-`46.c`
first, before this PR merged - resolved as a rename on merge to avoid
add/add filename collisions. The whole seven-file family was
renumbered together (not just the four that literally collided) to
keep it visually contiguous.

25-function `decomp-chunk` covering the `InitActorPart`/`gUnknown_03000884`-
rooted "self" object family already documented for
`actor_part17.c`/`actor_part18.c`/`actor_part19.c`/`actor_part28.c`/
`actor_part32.c`: a "part table" pointer at `self+0` (copied from the
constructor's `part` argument's own `+4` field), a table-index/"kind"
field at `self+0xc`, an anim-frame halfword/byte pair at `self+0x10`/
`self+0x12`, an accumulator at `self+8`, state at `self+0x28`, a frame
counter at `self+0x44`, a `+0x50`-rooted event/trampoline table fed
through `sub_803AD80`, and the `+0x48`(next)/`+0x4c`(prev) circular
doubly-linked list rooted at the player-pointer global
`gUnknown_03000884`. This chunk additionally pins down `InitActorPart`
itself (the constructor every other `actor_part*.c` file already
forward-declares and calls) plus a handful of new fields it introduces:
the constructor's raw `part`/`b`/`c`/`d` arguments cached at
`self+0x30`/`self+0x1c`/`self+0x20`/`self+0x24`, a "movement" threshold
pair at `self+0x14`/`self+0x34` (an absolute-value/packed-bitfield
distance metric compared against `gUnknown_030013C0`/`gUnknown_030013C4`,
gating whether the object fires its `+0x50` table's slot-3 trampoline
instead of animating), a one-shot byte flag at `self+0x2c`, and a
12-byte little vector block at `self+0x38` (copied from `part+0x14..20`)
whose first three `s16` slots are a position `sub_802AA0C` integrates a
per-axis velocity into. A separate, unrelated `gUnknown_03001464`-gated
palette-cycle DMA cluster (`sub_802AB08`/`sub_802AB34`/`sub_802AB58`/
`sub_802ABC8`/`sub_802ABFC`) and a fixed 15-slot object registry
(`gUnknown_03001428`/`gUnknown_03000888`, searched/appended by
`sub_802AA80`/`sub_802AAB4`/`sub_802AAFC`) round out the chunk.

The raw source file `asm/code_3_2_20_8b7c.s` (6522 lines, spanning far
beyond this chunk - InitHudTextWidget through past ConstructActorPart)
has been split around each matched/parked function, following this
project's "cut at the boundary" convention - new fragments are named by
the lower 5 hex digits of their first function's address
(`..._a88c.s`, `..._aa0c.s`, `..._ab58.s`, and the tail continuation
`..._ac28.s`).

## Matched (22 of 25 functions)

- **`sub_802A69C`/`sub_802A6B0`/`sub_802A6C4`/`sub_802A6D8`**
  (`src/graphics/actor_part50.c`) - four trivial forwarders, the same
  shape as `sub_802C0A8` in `actor_part19.c`: each ignores its own
  argument and calls a different function with the player pointer
  (`gUnknown_03000884`), discarding the return value.
- **`sub_802A6EC`** (`src/graphics/actor_part50.c`) - passes its own
  `self` argument through to `sub_803AD7C`, alongside a function pointer
  read from `gUnknown_03001418`'s own `+0x24` field.
- **`InitActorPart`** (`src/graphics/actor_part50.c`) - the constructor.
  Matching it byte-exact needed several of this project's established
  idioms stacked together: `part`/`b`/`c` pinned to `r4`/`r5`/`r6` and
  `self` left as a *plain* local (letting the allocator land it in `r7`
  naturally - pinning it explicitly produced a spurious extra load
  instead), the three-word `part+0x14..0x20` copy expressed as a real
  struct assignment (`struct blob0xc { u32 w0,w1,w2; }`) to get the
  `ldm`/`stm` idiom instead of three separate word copies, the
  movement-threshold computation using a `r2`-pinned running value and a
  new branchless-abs macro (`ABS32`, matching the ROM's own
  `asrs`/`eors`/`subs`-on-sign-shift codegen for `abs()` rather than the
  branch this compiler emits for `?:`/`if`) written as in-place updates
  so the same register gets reused across the whole sequence, and the
  final list-insertion re-ordered to store `self` into the head node's
  `+0x4c` *before* reading its `+0x48` (matching the ROM's literal
  instruction order, not just its final result).
- **`sub_802A7B8`** (`src/graphics/actor_part50.c`) - the movement-
  threshold recompute/frame-advance pair, sharing `InitActorPart`'s
  `ABS32`/`r2`-running-value idiom. Two additional gaps: the trampoline-
  fire call's `self + offset` argument had to be computed into its own
  local *before* reading the trampoline's function pointer (matching the
  ROM's evaluation order, not just gcc's default left-to-right guess),
  and the closing keyframe-record lookup needed `idx` read before
  `table` (not the more natural `table` first) plus the resulting
  `table + idx*0xc` address pinned to `r1` to reproduce the ROM's
  register choice for the shared record pointer.
- **`sub_802A980`** (`src/graphics/actor_part56.c`) - the same
  movement-threshold recompute as `sub_802A7B8`, with no trampoline-
  fire/frame-advance tail.
- **`sub_802A9D4`** (`src/graphics/actor_part56.c`) - trivial getter:
  the first byte of `self`'s part-table pointer.
- **`sub_802A9DC`** (`src/graphics/actor_part56.c`) - the state/table-
  index/anim-frame reset idiom already documented for the boss
  cluster's `sub_8030530`/`sub_8030C98`.
- **`sub_802AA00`/`sub_802AA04`/`sub_802AA08`** (`src/graphics/actor_part56.c`)
  - trivial `self+0x24`/`0x20`/`0x1c` getters.
- **`sub_802AA4C`** (`src/graphics/actor_part52.c`) - trivial `self+0x2c`
  byte getter.
- **`sub_802AA54`** (`src/graphics/actor_part52.c`) - teardown: marks
  `self` "dead", unlinks it from the circular list (the same shape as
  `sub_802C19C`'s unlink sequence in `actor_part19.c`), and frees it
  when requested.
- **`sub_802AA80`/`sub_802AAB4`/`sub_802AAFC`** (`src/graphics/actor_part52.c`)
  - the fixed 15-slot `gUnknown_03001428`/`gUnknown_03000888` registry's
  search/append/clear trio. Both search loops needed the counter/array-
  pointer pinned to `r2`/`r1` (`sub_802AA80`) or `self` pinned to `r3`
  with the loop's final store re-reading `gUnknown_03000888` fresh
  instead of reusing the already-checked value (`sub_802AAB4`) - a
  register choice this compiler picked differently depending on how many
  *other* functions preceded it in the same translation unit, discovered
  only after the full link shifted these two functions by 4 bytes (see
  "A note on isolated-compile confidence" below).
- **`sub_802AB08`/`sub_802AB34`** (`src/graphics/actor_part52.c`) - the
  palette-cycle cursor/bound save/restore pair.
- **`sub_802ABC8`/`sub_802ABFC`** (`src/graphics/actor_part54.c`) - the
  palette-cycle cluster's remaining seed/arm-disarm pair, non-adjacent to
  `actor_part52.c` since the parked `sub_802AB58` sits raw between them.

## Parked (3 of 25 functions, `NON_MATCHING`)

- **`UpdateAnimatedActorPart`** (`asm/code_3_2_20_8b7c_a88c.s`, C in
  `src/graphics/actor_part55.c`) - the OAM draw/scale routine: computes
  an OBJ scale factor and on-screen X/Y from `self`'s movement-threshold
  metric, culls off-screen, and calls `SetupSpriteFrameOam`. Every one
  of this ~120-instruction function's bytes matches the ROM except one:
  the second half's `frame[1]` read (the mirror of the first half's
  `frame[0]` read, done through `r7`) is read by the ROM through `r0` -
  a leftover, never-reloaded copy of `GetAnimFrameData`'s own return
  value still sitting untouched in `r0` at that point, saving a register
  materialization the ROM's real compiler recognized was redundant. This
  agbcc build does not perform that specific redundant-load/value-reuse
  optimization: referencing `frame` a second time always re-derives it
  from its one canonical register, and pinning a second `register` alias
  onto `r0` emits an explicit (extra) copy instruction instead of
  reusing `r0`'s still-valid contents - the opposite of what's needed.
  Same shape and same family of gap as the already-parked `sub_802C2FC`
  in `actor_part19b.c` (issue #52), which hits an analogous dead-code-
  elimination difference in this compiler.
- **`sub_802AA0C`** (`asm/code_3_2_20_8b7c_aa0c.s`, C in
  `src/graphics/actor_part51.c`) - a 12-byte little-vector velocity
  integrator (the position block `InitActorPart` copies from
  `part+0x14..0x20`, see above). Semantics are fully understood and
  every load/store is confirmed correct (this compiler does emit the
  same `ldm`/`stm` 3-word block-copy idiom the ROM uses at both ends,
  confirming the shape); the residual gap is purely instruction
  *scheduling* around the three per-axis `>>8` shifts between the two
  block copies - this compiler's own list scheduler bunches the three
  loads/shifts together differently from the ROM's own strict
  load-shift/load-shift/load-shift order no matter how the source
  statements are grouped or which registers the per-axis values are
  pinned to.
- **`sub_802AB58`** (`asm/code_3_2_20_8b7c_ab58.s`, C in
  `src/graphics/actor_part53.c`) - the palette-cycle cursor-advance DMA
  step. Semantics fully understood and every load/store, branch and call
  is confirmed correct (the DMA setup and refresh-counter reset at the
  top match the ROM instruction-for-instruction); the residual gap is in
  the cursor-advance tail: no C phrasing tried (plain if/else-if/else,
  `goto`-linearized with an explicit `result` copy, cached-address
  locals, register-pinned address locals) stops this compiler from
  speculatively computing the decrement (`idx - 1`) ahead of the branch
  that decides whether it's needed, which shortens the branch-taken path
  by folding away a redundant unconditional jump the ROM's own build
  still has - a shorter, still-correct result, but not byte-identical.

## A note on isolated-compile confidence

`sub_802AA80`/`sub_802AAB4` both compiled to a register choice matching
the ROM when tested alone in an early combined file, then *silently*
picked a different register (still correct, still the right size, just
the wrong specific register) once later functions were split out of
that file into `actor_part52.c`/`actor_part54.c` - shifting every
subsequent function's address by 4 bytes, caught only by the full-link
`make compare` and the map-file address-shift diagnostic
`docs/workflow.md` describes. This is the same failure mode already
documented for `sub_8033BB8` (issue #62) and the `sub_8008C80`/
`sub_8008D30` pair, now confirmed a third and fourth time: which other
functions share a translation unit can change this compiler's own
register allocation for a function whose *C source* never changed,
so isolated-compile confidence never substitutes for the full-link
check even when nothing about the function itself was touched.

See [docs/status/actor.md](../status/actor.md) for the running
matched/parked list this entry feeds into.
