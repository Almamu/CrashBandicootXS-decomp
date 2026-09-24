# Issue #63: 0x08033EF4-0x08034AA4 (actor)

**Naming note:** these files are numbered `actor_part63`-`73` rather
than `actor_part57`-`67` (which would have matched their creation order
more naturally) because issues #19 and #54's parallel PRs independently
claimed `actor_part57.c`-`62.c` first, both before this PR merged -
resolved as a rename on merge to avoid a three-way add/add filename
collision. The whole eleven-file family was renumbered together (not
just the six files that literally collided) to keep it visually
contiguous.

25-function `decomp-chunk` immediately following issue #62's cluster
(`0x08033804`-`0x08033EF4`, see
[docs/matching/issue-62-0x08033804-actor.md](issue-62-0x08033804-actor.md)).
The raw source file (`asm/code_3_2_20_28568_c99c_31784_33ef4.s`, itself
the tail continuation left after issue #62's own cuts) has been split
around each matched/parked/left-raw run, following this project's
"cut at the boundary" convention - new fragments are named by the lower
5 hex digits of their first function's address appended to the current
filename (`..._33fe4.s`, `..._34058.s`, `..._34270.s`, `..._34314.s`,
`..._34374.s`, `..._345b0.s`, `..._3472c.s`, and the tail continuation
`..._34aa4.s`, which keeps the remaining still-raw span up to
`LoadLevelGraphics` at `0x080354E0`).

Three distinct `InitActorPart`-rooted per-instance "self" object kinds
are constructed in this chunk, all sharing the family's usual layout
(`self+0` part table, `self+0xc` table-index/"kind", `self+0x10`/`0x12`
anim-frame halfword/byte, `self+8` accumulator, `self+0x28` state,
`self+0x44` frame counter, `self+0x50` event/trampoline table):

- **Kind 1** (`sub_8033EF4`, vtable `gStaticData_087E551C`): health at
  `+0x54`, caches its own `b`/`c` constructor args at `+0x58`/`+0x5c`,
  a death flag at `+0x6c`.
- **Kind 2** (`sub_8034058`, vtable `gStaticData_087E5554`): health at
  `+0x54` (`0x10` or `0x18` depending on whether the
  `gUnknown_030015AC` singleton is already constructed), a death flag
  at `+0x58`, a second one-shot flag at `+0x2c`, the constructor's 6th
  (stack-passed byte) argument cached at `+0x59`, and a little
  "spawn/orbit" record at `+0x5c`/`+0x60`/`+0x64`/`+0x68`/`+0x6c`
  driving `sub_8034188`'s position-plus-effect-spawn step.
- **Kind 3** (`sub_80342D4`, vtable `gStaticData_087E558C`): a much
  smaller object reusing `+0x58` as a plain one-shot flag rather than a
  health countdown.

## Matched (19 of 25 functions)

- **`sub_8033EF4`/`sub_8033F48`/`sub_8033F74`** (`src/graphics/actor_part63.c`)
  - Kind 1's constructor, its trampoline-fire helper (same shape as
  `sub_8033BFC`, actor_part32.c), and a position-sync/state-1-transition
  helper gated on the singleton's lifetime counter and animation "kind".
  The constructor needed the established two-distinct-zero-register
  reset idiom (`zero`/`zero2`, see below) plus an explicit `d`-parameter
  register pin (`r0`) placed *after* the `b`/`c` pins so this compiler
  fetches the 5th (stack) constructor argument in the same position the
  ROM's own build does, rather than up front with the others.
- **`sub_8034050`** (`src/graphics/actor_part65.c`) - trivial Kind 1
  death-flag getter (`self+0x6c`).
- **`sub_8034110`/`sub_8034188`/`sub_80341F8`/`sub_8034264`/`nullsub_38`**
  (`src/graphics/actor_part67.c`) - Kind 2's damage/death handler (same
  `sub_8033AE0` shape, register-pinned `zero`/`one` reused across the
  `self+0x58`/`+0x2c`/`+0x28`/`+0x44`/`+8` stores and the gate-byte read
  at `self+0x59` - reachable only via a pointer-offset walk from
  `self+0x58`, since `0x2d` doesn't fit `ldrb`'s 5-bit immediate range
  and the ROM's own build visibly re-derives the address instead of
  indexing), a position-sync/orbit-effect updater (`sub_8034188`, three
  register-pinned locals - `field`/`z` for the Z-axis sum, `origCounter`/
  `result` for the orbit-counter decision - matching the ROM's exact
  `r6`-stays-immutable/`r0`-carries-the-final-value split), a near-twin
  (`sub_80341F8`) that turns out *not* to call `sub_802A7B8` first
  (initially miscopied as a byte-identical twin - the map-file address-
  shift diagnostic caught the missing 4-byte call), a trivial death-flag
  getter, and a no-op stub.
- **`sub_80342D4`** (`src/graphics/actor_part69.c`) - Kind 3's
  constructor, same two-distinct-zero-register reset idiom as
  `sub_8033EF4`.
- **`sub_803436C`** (`src/graphics/actor_part71.c`) - trivial Kind 3
  one-shot-flag getter (`self+0x58`).
- **`sub_8034688`/`sub_80346C8`/`sub_80346FC`** (`src/graphics/actor_part73.c`)
  - a particle-spawn-budget driver (calls `sub_8034480`, then spawns up
  to 8 particles via `sub_80345B0`; needed the incoming-`idx`-value
  register pinned through `r0` then copied to `r1` for the call,
  matching the ROM's own redundant-looking but real two-step move), an
  input-poll busy-wait built from a `goto`-based `while`-with-shared-
  check shape (matching the ROM's single physical check block reached
  both by the initial skip-into-loop jump and the loop-back fallthrough,
  with the poll flags' base address kept separate from the `+2`
  immediate offset so this compiler doesn't fold the offset into the
  constant-pool address), and a buffer-release/teardown helper.

### A recurring gotcha: the "two distinct zero registers" reset idiom

Every one of this chunk's constructors resets `self+0x28`/`+0x44`/`+0xc`/
`+8` to 0 *and* `self+0x12` (the anim-frame byte) to 0 in the same
breath. The ROM's own build never reuses one already-live zero register
for both: it materializes a second, freshly-loaded zero specifically for
the `self+0x12` store (and reuses *that* second register for anything
immediately after it, like `sub_8033EF4`'s `self+0x6c` store). Writing
the obvious single-`register zero`-reused-everywhere C consistently
compiles 2-4 bytes *shorter* than the ROM - an isolated compile still
"looks right" (same mnemonics, same operands, just one binding fewer),
and it takes the real map-file address-shift check (function boundaries
landing 4 bytes early) to catch it. Fixed throughout this chunk with a
nested block introducing a second `register ... zero2` immediately
before the `self+0x12` store, mirroring the established idiom already
in `sub_8033AE0`/`sub_803390C` (actor_part28.c/actor_part30.c).

### A note on isolated-compile confidence (again)

This chunk repeated the exact class of gap `docs/workflow.md` and
`docs/matching/issue-62-0x08033804-actor.md` already warn about twice:
an isolated per-function compile that reads instruction-for-instruction
identical to the ROM disassembly can still be wrong. Three separate
instances surfaced only once the whole chunk was linked and the ROM
diffed byte-for-byte against `baserom.gba`:

1. The missing-second-zero-register gap above (`sub_8033EF4`,
   `sub_8033F74`, `sub_80342D4`) - each shrank its own function by 2-4
   bytes, which a from-scratch isolated compile has nothing to compare
   its *size* against.
2. `sub_8033EF4`'s `d` constructor argument being fetched from the stack
   in the wrong position relative to `b`/`c` - same total instruction
   count and mnemonics, just reordered, so it produced a real 6-byte
   content mismatch without shifting any function's address at all.
3. `sub_80341F8` being copied as a "byte-identical twin" of `sub_8034188`
   when it's actually missing the leading `sub_802A7B8(self)` call - a
   genuine 4-byte size difference that happened to exactly cancel the
   4-byte deficit inherited from the upstream `sub_8033EF4` bug, so the
   *next* function (`sub_8034264`) landed back at its correct absolute
   address by coincidence and briefly looked like proof nothing was
   wrong.

All three were only caught by generating the real ROM (`make compare`),
diffing it byte-for-byte against `baserom.gba`, and cross-referencing
every differing byte range against `crashbandicootxs.map`'s function
boundaries - not by re-reading the isolated compiles more carefully.

- **`sub_8034374`** (`src/graphics/actor_part85.c`) - constructs the
  particle-trail BG0 object (see that file's header comment for the
  full `struct particle_bg` field layout: a `tileVramBase`/`mapVramBase`
  pair of fixed VRAM constants, a 128-slot particle array, an active
  count, and a 240x160 4-bit-per-pixel shadow `tileBuffer`). The
  apparent "read of an uninitialized local" this function was
  previously left raw over turned out to be the same negative-constant
  bit-clear idiom already established for `LoadBg2Background`'s
  `bg2cnt` (`src/graphics/level_graphics.c`, issue #65) - an
  intentionally uninitialized `u32` ANDed against `0xFFFF0000` before
  every bit the final halfword write actually reads gets ORed in, not a
  real bug. Now fully matched as real C: the ROM builds the tilemap-
  fill loop's palette-bank mask (`0xFFFFF000`) by loading the 32-bit
  literal into `r1` first and then copying it into `r5` (`ldr
  r1,=0xFFFFF000; adds r5,r1,#0`), rather than the single direct
  `ldr r5,=...` a plain `mask = -0x1000;` compiles to - closed by
  pinning an intermediate local to `r1` (its initializer has to stay a
  plain C constant, not an inline-asm-embedded immediate: an
  asm-embedded `=0xFFFFF000` immediate gets pooled by the assembler as
  a *second*, separately-placed literal appended after the compiler's
  own pool, landing 16 bytes past where the ROM actually puts it,
  whereas a plain C initializer lets the compiler place the same
  literal in its own pool at the ROM's real position) and then forcing
  the `r1`->`r5` copy via `asm volatile("add %0, %1, #0" ...)`. Closing
  that surfaced two further gaps: the `mapBase + (row << 6)` addition's
  operand order needed the same pin-and-force treatment (`add r1, r0,
  r7`, not gcc's default `r7, r0`), and the `col = 0x1d` initializer
  needed moving to *after* that computation in the C source - a trivial
  immediate move that gcc otherwise schedules ahead of a nearby
  pinned-register `asm volatile` block purely by its original textual
  position, once that block acts as a hard scheduling barrier. Retired
  the multi-function raw `asm/code_3_2_20_28568_c99c_31784_33ef4_34374.s`,
  split at the time into `asm/code_3_2_20_28568_c99c_31784_33ef4_34480.s`
  (real bytes for the twin `sub_8034480`) - that fragment is also retired
  now that `sub_8034480` itself is matched (see below), so
  `actor_part85.c` is fully matched, closing the whole file.
- **`sub_8034058`** (`src/graphics/actor_part66.c`) - Kind 2's
  constructor. Now fully matched as real C, closing two gaps: the 6th
  (stack-passed, byte-sized) constructor argument needs the same
  stack-slot-address-then-`ldrb` `asm volatile` anchor already
  established for other trailing byte arguments (see `sub_8003A60` in
  `issue-5-overlay-ui-sync.md`), materialized into a `register u32
  asm("r9")` pin mirroring the ROM's own `sb` cache (needed since it
  survives the following `sub_80338DC()` call) - the same
  trailing-byte-stack-argument gap already closed elsewhere, not the
  unrelated `sub_8025A64` mask-constant-folding gap this entry used to
  be compared against. The `self+0x5c` spawn-record ternary
  (`(self[0x59] != 0) ? 0xFFFFBF00 : 0x8400`) needed a second, separate
  fix: the ROM computes it as a genuine two-way branch diamond (a
  forward `beq`/`ldr`/`b` skipping a computed false branch, with
  `gStaticData_087E5554`'s pending literal and `0xFFFFBF00` pooled
  together right after the skip branch), but a plain ternary or if/else
  always collapses this compiler's output to an eager "compute one
  value, conditionally overwrite" shape instead (4 bytes short); and
  once the diamond is forced through any register-pinned C-level
  if/else, this compiler's own parameter-homing pass reorders `self`'s
  prologue copy relative to `part`/`b`/`cParam` (still the right
  register, just the wrong instruction position) - a discrepancy that
  didn't respond to any combination of pinning, `goto`-linearizing, or
  precomputing the store address tried. Closed by moving the whole
  diamond into one opaque `asm volatile` block that references `self`'s
  known `r5` home directly by name rather than as an operand (passing
  it as an operand reintroduces an extra, ROM-absent register copy),
  with a real `ldr r0, =0xFFFFBF00` assembler pseudo-op and a manual
  `.pool` directive right after the skip branch - and moving the
  preceding `gStaticData_087E5554` store into its own tiny `asm
  volatile` island too, since a real, respected `.pool` split only
  works for symbols whose literal load is itself opaque assembler text
  (this compiler's own C-driven pool placement for a plain `extern`
  global access always defers to the function's very end and ignores
  an `asm(".pool")` marker around it), the same gap already documented
  for `sub_802AB58` in `actor_part53.c`. Retires the raw
  `asm/code_3_2_20_28568_c99c_31784_33ef4_34058.s`.

- **`sub_80345B0`/`sub_8034634`** (`src/graphics/actor_part72.c`) - a
  128-slot particle spawner (rolls two `sub_8000E1C` random values
  against the 256-entry `gStaticData_0816A820` direction table to seed a
  position/velocity record) and a 4-bit-per-cell tilemap nibble writer;
  both now fully matched as real C, retiring
  `asm/code_3_2_20_28568_c99c_31784_33ef4_345b0.s` entirely.

  `sub_80345B0`'s previously-parked register-allocation gap for the
  final multiply/shift needed no register pins or opaque asm at all,
  once properly diagnosed: this compiler's codegen for `dest = a * b`
  always materializes/copies the *left* operand into the destination
  register before the `muls` (`adds r3, r1, #0; muls r3, r0` in the ROM,
  copying the table lookup), while the original C wrote the
  multiplication as `speed * table[...]` (copying `speed` instead, since
  it was the left operand there). Simply reordering to
  `table[...] * speed` - mathematically identical, since multiplication
  is commutative - matched immediately.

  `sub_8034634`'s residual `addr`/`blockY` register-role gap turned out
  to be three separate, independently-found issues, not one:
  1. `x`'s bounds check (`x <= 0xef`) wants an unsigned comparison (the
     ROM's `bhi`), but `x >> 3` wants a *signed* arithmetic shift
     (`asrs`, not `lsrs`) - i.e. the ROM's own source treated `x` as
     signed for the shift while still using an unsigned-style bounds
     check. Modeled with an explicit `((s32)x >> 3) << 6` cast rather
     than a plain unsigned `x >> 3`.
  2. `addr`'s two halves (the x-derived `<< 6` term and the
     blockY-derived `<< 7` term) needed splitting into two separate C
     statements (`addr = ...; addr += ...;`), not one combined `a + b`
     additive expression - gcc doesn't evaluate `+`'s operands
     left-to-right, so the combined-expression form let it pick
     blockY's half first (opposite the ROM's x-half-first order);
     splitting into statements pins the evaluation order to match.
  3. The temporary `mask` (`0xf << shift`) needed to stay a plain
     32-bit type (`u32`, not `u16`) - declaring it `u16` makes this
     compiler insert a defensive 32-bit-to-16-bit truncation sequence
     around the shift (`movs r0,#0xf0; lsls r0,r0,#12; lsls r0,r2;
     lsrs r0,r0,#16` - 4 instructions computing `0xf<<16<<shift>>16`,
     algebraically equal to `0xf<<shift` but the long way round), which
     the ROM's own build never has, since `mask`'s upper 16 bits never
     actually matter (only `bics`/`orrs` read it, against a cell already
     zero-extended by `ldrh`). Reverting `mask` to a plain 32-bit type
     let the ROM's own two-instruction `movs`/`lsls` fall out on its
     own.

  The final residual gap - the ROM's own "materialize `cell` into `r4`
  via `bics`, then copy it back into `r0` before `orrs`/`strh`" idiom,
  the same class of redundant-copy-after-a-binary-op gcc-2.9 quirk
  already seen for `sub_802F338`'s multiply-copy gap - is closed with
  one opaque `asm volatile` block emitting that exact instruction
  sequence verbatim, taking `shift` and `tileMapEntry` (itself pinned to
  `r2` via a nested `register ... asm("r2")` local initialized from a
  `register s32 off asm("r0")` intermediate, matching the ROM's own
  `asrs`/`lsls`/`ldr`/`adds` sequence for computing the tilemap entry
  address) as inputs, and `val` (already pinned to `r3` for the leaf-
  function register-spill fix) as an in/out operand.

- **`sub_8034480`** (`src/graphics/actor_part85.c`) - the particle-trail
  BG0 object's per-frame updater (`sub_8034374`'s companion, same file);
  now fully matched as real C, closing the file entirely. Commits last
  frame's `tileBuffer` to tile VRAM, clears it back to zero, then for
  each active particle inlines the same nibble-address formula as
  `sub_8034634` above *twice* (nibble `1` at the pre-move position,
  nibble `2` at the post-move position), applying `dx`/`dy` and
  respawning via `sub_80345B0` in between. Needed a mix of
  `sub_8034634`'s own three fixes plus two more ordering fixes this
  larger, twice-inlined function surfaces on its own:
  1. Both bounds checks that compare a pixel/position value against a
     hex constant (`x <= 0xef`, and separately `newX > 0xEFFF` after
     the move) want an unsigned comparison (the ROM's `bhi`), modeled
     with `(u32)` casts, while the `>> 11` block-index shifts stay plain
     signed arithmetic shifts on the already-`s32` raw values.
  2. `addr`'s two halves need computing as separate statements, not one
     combined `a + b` expression - same as `sub_8034634`.
  3. The tail - `cell &= ~mask; cell |= val << shift; *entry = cell;` -
     closes with one opaque `asm volatile` reproducing the ROM's exact
     sequence, simpler here than `sub_8034634`'s own tail since this
     function's ROM build never needs the extra `r4`-materialize-then-
     copy-back step: `mask` computed straight into `r0`, `cell` loaded
     straight into `r2` and `bic`'d in place, then `r0` reused to shift
     `val` in before the final `orr`/`strh`.
  4. Two purely scheduling-order fixes this compiler doesn't infer from
     a natural top-of-block declaration group: `x`'s raw value and its
     `>>8` pixel value must be computed immediately, before `y` is even
     loaded (and likewise `blockX`'s `<<6` term fully computed before
     `blockY` is loaded), and the post-move `slot->x = newX` store must
     happen immediately after computing `newX`, before `newY` is even
     loaded - not batched together the way the source's natural
     top-to-bottom order would suggest.
  5. The second inlined copy's `oldVal` (the second nibble value,
     always 2, pinned to `ip`) must be assigned by a plain statement
     *after* the position reload, not as its `register` declaration's
     own initializer - an initializer schedules the `movs #2`/`mov ip`
     pair too early, ahead of the ROM's own position.

  Retires `asm/code_3_2_20_28568_c99c_31784_33ef4_34480.s` entirely,
  closing `actor_part85.c` (both `sub_8034374` and `sub_8034480`, its
  only two functions) as fully matched.

## NAKED transcription (byte-correct, not counted as matched)

- **`sub_8033FE4`** (`src/graphics/actor_part64.c`) - a
  `gStaticData_0817C4F8` stride-8 trampoline-record dispatcher
  returning a 0/1 result instead of tail-calling. Same `{s16 baseOff;
  s16 count; void *fn}` record shape as `sub_8033B44`/`sub_8033C84`/
  `sub_8033E80` (issue #62) and `sub_802C208` (issue #52) - all hit
  the same confirmed categorical gcc-2.9 r7-pin bug (the ROM keeps the
  table's base address alive in `r7` for the whole function; an
  explicit `register T x asm("r7")` compiles correct instructions but
  never makes it into the prologue/epilogue push/pop list, and this
  compiler's own unforced allocator never reaches r7 here either) and
  are transcribed the same way - see
  docs/matching/issue-52-0x0802bed8-actor.md's `sub_802C208` entry for
  the full account. Every instruction is byte-verified against the ROM
  disassembly, so the built ROM is byte-identical here, but per this
  project's current tracking policy a NAKED transcription of a
  substantial function doesn't count as "matched" -
  `tools/report_units.py` keeps this address's `base_object` as `None`.

## Parked (2 of 25 functions, `NON_MATCHING`)

- **`sub_8034270`** (`asm/code_3_2_20_28568_c99c_31784_33ef4_34270.s`, C
  in `src/graphics/actor_part68.c`) - position-sync/flag/trampoline
  updater for Kind 1. Semantics fully understood and every field/call
  confirmed correct; parked because the ROM computes a "should animate"
  0/1 value into a register and re-checks it against zero before
  deciding whether to call `sub_802A7B8`, even though the value is a
  compile-time constant on each path - this compiler's dead-branch
  elimination always collapses that redundant compute-then-recheck
  step, the same class of gap already documented for `sub_802C2FC`
  (issue #52) and the dead `| 0` term in `sub_803B46C` (issue #71).
- **`sub_8034314`** (`asm/code_3_2_20_28568_c99c_31784_33ef4_34314.s`, C
  in `src/graphics/actor_part70.c`) - `sub_8034270`'s boolean-returning
  twin, parked on the identical gap.

## Left raw (3 of 25 functions, not attempted)

- **`sub_803472C`/`sub_803487C`/`sub_8034994`**
  (`asm/code_3_2_20_28568_c99c_31784_33ef4_3472c.s`) - a graphics-package
  loading setup (BG/window register packing, three `LoadGraphicsPackage`
  calls, heavy `sb`/`r8` register pressure), a larger orchestration
  routine spanning several subsystems, and a ~140-instruction state-
  machine/input-poll loop with `sb`/`sl`/`r8` all live simultaneously;
  left raw, out of scope for this pass.

See [docs/status/actor.md](../status/actor.md) for the running
matched/parked list this entry feeds into.
