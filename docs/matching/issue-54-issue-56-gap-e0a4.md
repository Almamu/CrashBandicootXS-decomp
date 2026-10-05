# The 0x0802E0A4-0x0802F0DC gap between issues #54 and #56 (actor)

A scoping investigation of the actor zone found this 4152-byte range -
the whole of `asm/code_3_2_20_28568_c99c_e0a4.s` - sitting between
issue #54's chunk (`actor_part61.c`, ending at `nullsub_27`/
`sub_802E0A0`) and issue #56's chunk (`actor_part43.c`, starting at
`sub_802F0DC`) - still completely raw. `docs/rom_map.md` had already
partly read this range from disassembly alone: `CreateJetpackActor` is "a
31-case jump table paired with a new stride-40 RAM table,
`gJetpackAnimTable`, fetching a position-offset pair per case".

25 functions total. `CreateJetpackActor` is the dispatcher itself; the run of
near-identical `mem_alloc`-plus-forwarding-call constructors
(`SpawnJetpackCollectedWumpa`, `SpawnHovercraftCannonFlash`, `SpawnJetpackBalloon`, `SpawnHovercraftSideGun`,
`SpawnHovercraftLauncher`, `SpawnHovercraftCannon`) are `CreateJetpackActor`'s own per-"kind" case
bodies, each allocating a fixed-size struct and forwarding to a
different kind-specific initializer, indexed into
`gJetpackAnimTable`'s array of per-kind data tables by a fixed byte
offset. The rest of the range is a family of position-offset/physics
helpers and hazard-timer functions on the usual `self` object (state/
table-index/anim-frame fields at the established offsets) and a couple
of no-argument helpers reading a fixed global object (`gUnknown_
03001507`/`gKeys`).

## Matched (4 of 25 functions)

All four are the lowest-register-pressure members of the
`CreateJetpackActor` constructor family - just `r4`-`r6`, no `r8`/`sb`/`sl` -
in `src/graphics/actor_part128.c`:

- **`sub_802E0A4`** - state-3 anim-frame edge reset: while `self`
  (`gYeti`)'s table-index isn't already 3 and its anim-
  frame flag is set, resets it to table-index 3 (anim frame from the
  part table's `+0x24` field) - the same reset idiom used throughout
  this whole actor zone.
- **`SpawnJetpackCollectedWumpa`** - `CreateJetpackActor`'s kind-0 case body: allocates a
  0x64-byte struct (`mem_alloc(0x64, 0x80000000)`) and forwards to
  `CreateJetpackCollectedWumpa` with the kind's own data-table entry
  (`gJetpackAnimTable + 0x6E0`) plus the three incoming position
  arguments; the call's own return value is discarded (matching the
  `void` signature already established for this function elsewhere in
  the codebase, e.g. `actor_part44.c`).
- **`SpawnHovercraftCannonFlash`** - same shape, 0x5c-byte struct, table offset
  0x230, forwards to `CreateHovercraftCannonFlash`.
- **`SpawnHovercraftLauncher`**/**`SpawnHovercraftCannon`** - same shape, 0x70-byte struct,
  table offsets 0x1E0/0x1B8, forward to `CreateHovercraftLauncher`/`CreateHovercraftCannon`.

All four needed the `mem_alloc(size, flags)` call's two arguments
pinned to `r0`/`r1` explicitly (`register u32 size asm("r0") = ...;
register s32 flags asm("r1") = 0x80000000;`) - a plain
`mem_alloc(0x64, 0x80000000)` call let this compiler materialize the
`0x80000000` flags constant (`r1`) before the size (`r0`), the
opposite of the ROM's own instruction order. Declaring these `void`
(not `s32`, despite the callee itself returning a value) was also
required: with an `s32` return type, this compiler correctly keeps the
forwarded call's result live in `r0` through the epilogue, producing a
`pop {r1}; bx r1` trampoline (freeing `r1` to hold the popped return
address instead of clobbering the live `r0` return value) - but the
ROM's own build uses the plain `pop {r0}; bx r0` idiom this whole
codebase's `void` functions use everywhere else, confirming the
original source never actually returned the forwarded call's result
here (matching the already-established `void SpawnJetpackCollectedWumpa(s32 x, s32
y, s32 amount);` extern declaration in `actor_part44.c`).

## Parked - NAKED transcription (byte-correct, not decompiled)

The other 21 functions, all in `src/graphics/actor_part128.c`. Given
how heavily this whole neighborhood (issues #50/#52/#56, see their own
matching docs) has already needed the NAKED-transcription escape hatch
for gcc-2.9 register-pressure/branch-layout gaps, and the size of this
particular gap, these were transcribed directly rather than chased at
the C level - using the same small scratch-only Python script
(mechanical `_08XXXXXX:` label renumbering to GNU-as local numeric
labels, plus unified-to-plain mnemonic translation) this project has
used for other large NAKED batches, run once per function against the
raw disassembly.

- **`SpawnJetpackActor`** - kind-classification/spawn dispatch helper:
  reads a "kind" byte from one of `self`'s own three leading bytes
  (chosen by the current game-mode pause flag and a caller-supplied
  gate), then tail-calls `SpawnAirship` (kinds `0x10`-`0x12`),
  `CreateJetpackActor` (most other kinds - the 31-case dispatcher below), or
  `SpawnHovercraft` (kind `0xa`) with a position computed from `self`'s own
  `+4`/`+8`/`+0xc` fields.
- **`CreateJetpackActor`** - the 31-case jump-table dispatcher `docs/rom_map.md`
  already flagged: indexes `gJetpackAnimTable` (a stride-40 per-"kind"
  data table) by `kind`, then forwards to one of the constructors
  matched above (reached only via `bl`, not inlined) or a small set of
  shared default cases. Parked outright rather than attempted as a
  `switch` - a 31-case dispatch's exact jump-table layout and bounds-
  check codegen carries too much risk of a subtly-wrong-but-plausible
  reconstruction for this gap's scope.
- **`CreateJetpackCheckpointText`**, **`CreateJetpackExplosion`** - small helpers in the same
  family (established externs already exist for both:
  `void CreateJetpackCheckpointText(void);` and
  `s32 CreateJetpackExplosion(s32 x, s32 y, s32 z, s32 kind);`, `actor_part21f.c`).
- **`SpawnJetpackBalloon`**, **`SpawnHovercraftSideGun`**, **`SpawnHovercraftFireball`** (2-arg),
  **`SpawnAirshipFireball`**, **`SpawnJetpackCannonball`**, **`SpawnJetpackShot`** - more of
  `CreateJetpackActor`'s own constructor-family case bodies and position-
  offset helpers, each with `r8`/`sb` (and for `DrawJetpackPlayer` below,
  `sl` too) simultaneously live across the whole function - the same
  register-pressure family this codebase's DMA/OAM functions are
  consistently NAKED-parked for. Established externs already exist for
  `SpawnHovercraftFireball`, `SpawnAirshipFireball`, and `SpawnJetpackCannonball` (`actor_part67.c`,
  `actor_part21d.c`, `actor_part21e.c`/`actor_part29.c`/
  `actor_part46b.c`).
- **`CreateJetpackPlayer`** - stashes its first argument into
  `gJetpackAnimTable` then allocates and forwards to `sub_8032ADC` with
  a position derived from the player object (`gActorList`).
- **`InitJetpackPlayer`** - a `self`-object physics/collision-react step.
- **`UpdateJetpackPlayer`**, **`DrawJetpackPlayer`**, **`DamageJetpackPlayer`** - larger
  `self`-object state-machine steps (frame-counter/hazard-timer driven
  transitions); `DrawJetpackPlayer` additionally has `r8`/`sb`/`sl` all
  simultaneously live.
- **`SteerJetpackPlayerY`**, **`SteerJetpackPlayerX`** - no-argument helpers reading
  `gUnknown_03001507`/`gKeys` directly.
- **`JetpackPlayerStateFly`**, **`JetpackPlayerStateRollLeft`**, **`JetpackPlayerStateRollRight`** - `self`-
  object frame-counter-threshold steps, each starting with a tail-call
  into `SteerJetpackPlayerY`/`SteerJetpackPlayerX` above.

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
clean, no warnings for `actor_part128.c`.

See [docs/status/actor.md](../status/actor.md) for the running
matched/parked list this entry feeds into.

## Later pass (issue #55 retry)

All 21 NAKED functions above were promoted to real C, and the whole
file now builds with old_agbcc (`DrawJetpackPlayer` needs it). The reasons
given for parking them didn't hold up. `CreateJetpackActor` is a plain
`switch`. The "r8/sb pressure" constructors only needed the
`AllocActor` inline wrapper, or an inline base constructor that takes
the hit-point value as an argument. See
[issue-55-naked-retry.md](issue-55-naked-retry.md).
