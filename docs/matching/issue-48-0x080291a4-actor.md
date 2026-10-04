# Issue #48: 0x080291A4-0x08029E4C (actor)

25-function `decomp-chunk` covering `SetupActorVramPool`/`InitActorCategory`
(the category (re)selection/loading-screen entry point), the
`sub_effect_table` entry-counting helper `sub_802968C`, and an unrelated-
but-address-adjacent BG-tilemap double-buffer scroll-effect subsystem
(`sub_8029720`-`sub_8029E40`) that turns out to sit interleaved in the
same ROM region rather than being part of the category system itself.

## Matched (real C)

- `SetupActorVramPool` (`actor_part104.c`) - pins the current category's
  tile-cache slots (2 for type-0 sprite families, 5 for type-1/2) from
  two still-unnamed ROM-side sub-tables, then (re)builds the category's
  status-icon OAM row via `sub_802732C`.
- `sub_802968C` (`actor_part100.c`) - counts how many of the current
  category's `sub_effect_table` entries (`struct sub_effect_record`,
  `include/actor_anim.h`) match one of two fixed `variantA` byte sets,
  picked by the category's `type` field.
- `sub_8029720`/`sub_8029730`/`sub_802973C` (`actor_part105.c`) - trivial
  frame-tick counter accessors.
- `sub_8029748`/`sub_8029794` (`actor_part95.c`) - category tick
  re-basing and an active-instance-count threshold test.
- `nullsub_5`/`GetCellAnimFreeTile` (`actor_part106.c`), `FlipCellAnimPage`
  (`actor_part96.c`), `sub_8029B2C` (`actor_part90.c`), `AdvanceCellAnim`
  (`actor_part97.c`), `sub_8029B8C`/`sub_8029B98` (`actor_part91.c`),
  `SetCellAnimSpeed`/`sub_8029C30`/`sub_8029D8C` (`actor_part98.c`),
  `sub_8029E28`/`sub_8029E34`/`sub_8029E40` (`actor_part92.c`) - the
  rest of the BG-tilemap scroll-effect subsystem's small accessors,
  accumulator-advance, and BG2-affine scroll/zoom setup functions.

Several of these needed real register-pinning/instruction-ordering
work once verified against a full clean build (not just an isolated
compile) - see the "Debugging notes" section below for the concrete
patterns hit repeatedly across this whole chunk.

## Parked - NAKED transcription (byte-correct, not decompiled)

- **`InitActorCategory`** (`actor_part101.c`) - the category
  (re)initialization + per-VBlank loading-screen driver. Fully
  understood (stores the category argument, resets active-instance
  counters, decompresses the sprite sheet, rebuilds VRAM pool pins,
  hands off to `SelectActorCategory`, then runs a loading loop polling
  input/redrawing the selector icon/flushing VRAM DMA until the
  surrounding menu signals exit). Sustains four simultaneous
  high-register pins (`sb`/`sl`/`r8`/`ip`, each reused for 2-3
  completely different roles across the function's sections) plus a
  stack-spilled loop-state variable threaded through many non-adjacent
  gotos, on a ~230-instruction, 20+-call, four-state loop body - a much
  larger instance of the same "many-high-register-difficulty" gap this
  project has already hit and parked elsewhere (`sub_80091D4`/
  `sub_8009868`). Verified byte-for-byte identical to the original raw
  disassembly by assembling both independently and comparing the raw
  `.text` bytes directly (not just against `baserom.gba`).
- **`UploadCellAnimFrame`/`InitCellAnim`/`ResetCellAnimBg`** (`actor_part95.c`) - a
  DMA copy trigger for the "console"/text-plane cursor cell, the cell
  geometry (re)configuration entry point, and the VRAM tilemap
  double-buffer fill pair it calls. All three reproduce the ROM's exact
  instructions but never converge on its exact register-role
  permutation/register-reuse pattern for the address-computation
  prologues - the same categorical register-pressure/role gap as
  `InitActorCategory` above, just smaller. Verified byte-for-byte
  against `baserom.gba` directly (relocation-aware: every differing
  byte in the isolated compile falls inside a `bl`/`ABS32` relocation
  range).
- **`FillCellAnimTilemap`** (`actor_part98.c`) - a VRAM tilemap-fill nested loop
  sharing `ResetCellAnimBg`'s shape; same register-pressure wall.

## Debugging notes: recurring gcc-2.9 patterns hit across this whole chunk

A first integration pass (across several parallel sessions working this
chunk concurrently) produced code that compiled clean and even matched
in *isolated* per-function compiles, but failed a full clean
`make compare` - exactly the trap docs/workflow.md step 3 warns about.
Tracking down every regression via map-file address-diffing (build,
find the first symbol whose linked address stops matching its expected
ROM address, binary-search backward through the functions before it)
turned up the same handful of gcc-2.9 quirks over and over:

1. **Literal-pool placement inside NAKED transcriptions matters.** This
   compiler/assembler doesn't always flush its literal pool at the very
   end of a function - for a large function it flushes wherever the
   first "safe" unconditional branch lands once the pool's forward
   PC-relative reach is about to run out, which can be deep inside a
   loop. A NAKED transcription that groups all its `.4byte` pool words
   at the function's end (rather than exactly where the ROM's own build
   put them) still assembles and even sizes correctly, but every
   `ldr rX, =symbol` between the pool's real position and its assumed
   one encodes a different PC-relative offset - shifting every
   subsequent instruction's *content* without changing the function's
   total size. Hit this in both `ResetCellAnimBg` and `FillCellAnimTilemap`.
2. **"Materialize the destination address before computing the value"**
   shows up constantly. For a plain `REG_X = expr;`/`global = expr;`
   assignment, this compiler's natural order is address-of-`REG_X`
   computed lazily, right before the store. The ROM's own build
   frequently computes the destination address *first* (sometimes long
   before the value is ready), especially when the same destination
   register gets reused for an unrelated purpose afterward. Fixed by
   materializing an explicit pointer local (optionally register-pinned)
   ahead of the value computation, then storing through it. Hit
   repeatedly: `FlipCellAnimPage`, `AdvanceCellAnim`, `SetCellAnimSpeed`, `sub_8029C30`
   (three separate times within the same function), `sub_8029E50`.
3. **Fresh-register vs. in-place reuse for a "new" value.** When a
   local's old value is dead after producing a new one (e.g. `pos =
   prev + delta;` where `prev` is never read again), this compiler
   defaults to overwriting `prev`'s own register in place. The ROM's
   build sometimes keeps the new value in a genuinely different
   register instead. Fixed with register-pinned locals for both the old
   and new values. Hit in `AdvanceCellAnim` (twice) and `sub_8029C30`.
4. **Signed vs. unsigned shift-by-31 idiom.** `v >> 31` on a signed
   `s32` produces an implementation-defined result this compiler
   resolves as an arithmetic shift (`asrs`); the ROM's own
   "sign bit as 0/1" rounding idiom needs the *logical* shift (`lsrs`),
   requiring an explicit `(s32)((u32)v >> 31)` cast. Hit in
   `sub_8029C30`.
5. **A genuine semantic bug**, not a codegen quirk: `sub_8029794`
   compared the two operands as unsigned (`bcc`) where the ROM compares
   them signed (`blt`) - fixed with an explicit `(s32)` cast on the
   `u32`-typed struct field being compared. And `sub_8029C30`'s
   `REG_BG0VOFS` write turned out to be `gUnknown_030013F4` alone, not
   `gUnknown_030013F4 + (v >> 9)` as an earlier pass had guessed - the
   ROM's own instructions have no `adds` between the load and the store.

None of this changes any function's understood *behavior* - every fix
above is either a pure codegen-matching technique or a correction to an
earlier pass's misreading of the ROM's own instruction sequence,
confirmed against the raw disassembly bytes directly.

## Later pass: NAKED retry

`UploadCellAnimFrame`, `ResetCellAnimBg` and `FillCellAnimTilemap` are now plain C, matching
under both compilers. `FillCellAnimTilemap` needed `tile++` in each branch of
the column test, and `ResetCellAnimBg` inlines the same body twice.
`InitCellAnim` is still NAKED, with a 37-halfword draft under
`NON_MATCHING`. `InitActorCategory` was not attempted. See
[issue-48-49-52-aabb-naked-retry.md](issue-48-49-52-aabb-naked-retry.md).

## Later pass: second near-miss sweep

`InitCellAnim` is real C (both compilers). The ROM stores
`gCellAnimFrameSize` once and reloads it for the division through a copy
of its address taken before the `if`. `asm("" : "=r"(reload) : "0"(a4))`
makes that copy. Evaluation order does the rest: the flag goes through a
pointer to `gCellAnimHasBanks` loaded first, `area` is assigned inside
the `gCellAnimTileBytes` store, and `size` is read back from
`gCellAnimTileBytes` between taking the address and copying it. See
[near-miss-polish-2.md](near-miss-polish-2.md).

## Later pass: category driver retry

`InitActorCategory` is now plain C, matching under old_agbcc
(`actor_part101.o` is on `OLD_AGBCC_OBJS`; current agbcc is 3 halfwords
off). The high-register roles are loop.c's invariant hoisting, not
pins. See [category-driver-naked-retry.md](category-driver-naked-retry.md).
