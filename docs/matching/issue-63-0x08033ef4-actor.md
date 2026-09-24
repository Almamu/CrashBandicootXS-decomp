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

## Matched (15 of 25 functions)

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
  position, once that block acts as a hard scheduling barrier. Retires
  the multi-function raw `asm/code_3_2_20_28568_c99c_31784_33ef4_34374.s`,
  split into the new `asm/code_3_2_20_28568_c99c_31784_33ef4_34480.s`
  (real bytes for the twin `sub_8034480`, still parked - see below).

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

## Parked (6 of 25 functions, `NON_MATCHING`)

- **`sub_8034058`** (`asm/code_3_2_20_28568_c99c_31784_33ef4_34058.s`, C
  in `src/graphics/actor_part66.c`) - Kind 2's constructor. Semantics
  fully understood and every field/call confirmed correct; parked
  because this compiler reads the 6th (stack-passed, byte-sized)
  constructor argument as a full word shifted/masked down to its low
  byte, where the ROM's own build addresses that stack slot directly
  with a plain `ldrb` - the same trailing-byte-stack-argument gap
  already parked for `sub_8025A64` in `game_loop14.c` (issue #41).
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
- **`sub_80345B0`/`sub_8034634`** (`asm/code_3_2_20_28568_c99c_31784_33ef4_345b0.s`,
  C in `src/graphics/actor_part72.c`) - a 128-slot particle spawner
  (rolls two `sub_8000E1C` random values against the 256-entry
  `gStaticData_0816A820` direction table to seed a position/velocity
  record) and a 4-bit-per-cell tilemap nibble writer. Semantics fully
  understood and every field/shift confirmed correct; parked on this
  compiler choosing different register allocations for the 16-bit table
  lookups/final multiply-shift (`sub_80345B0`) and unconditionally
  spilling all four leaf-function parameters into callee-saved registers
  even though `val` is never touched until the function's tail with no
  intervening call (`sub_8034634`) - the ROM's own build simply leaves
  `val` in `r3` the whole time.
- **`sub_8034480`** (`asm/code_3_2_20_28568_c99c_31784_33ef4_34480.s`,
  C in `src/graphics/actor_part85.c`) - the particle-trail BG0 object's
  per-frame updater (see that file's header comment for the full
  `struct particle_bg` field layout: a `tileVramBase`/`mapVramBase`
  pair of fixed VRAM constants, a 128-slot particle array, an active
  count, and a 240x160 4-bit-per-pixel shadow `tileBuffer`). Semantics
  fully understood and confirmed field-by-field/instruction-by-
  instruction against the ROM disassembly; matches the ROM exactly
  through both particle in-bounds checks and the respawn call, but hits
  the exact same categorical gap already accepted as unclosable for
  `sub_8034634` just above (its own inlined nibble-write logic,
  duplicated twice per particle instead of calling `sub_8034634`): this
  compiler computes the `addr & 3` shift amount and the `0xf <<
  shift`/`cell` values into the opposite register pair from the ROM's
  own build. Left parked (real C, `NON_MATCHING`) rather than fall back
  to a NAKED transcription, per this project's current policy of
  preferring real C whenever the semantics are this well understood -
  it's also too large (~150 instructions, `sb`/`r8`/`ip` all live) for
  a NAKED transcription to be a reasonable substitute anyway. Its twin
  `sub_8034374` (this same file) is now matched - see below.

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
