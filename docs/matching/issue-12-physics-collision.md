# Issue #12: 0x0800D040-0x0800FC70 - the physics/collision subsystem

GitHub issue #12 (`decomp-chunk`, labeled `graphics` by the chunk
generator) listed 25 raw functions in `asm/code_3_2_17.s`. This is the
write-up for the work done against that list.

## Category correction: `graphics` -> `game_loop`

The chunk generator's `graphics` label is a stale placeholder - the
`tools/report_units.py` unit covering this whole address neighborhood
(`0x0800B8DC` onward) was never individually examined and just
inherited the category of the already-matched `graphics.c` code sitting
right before it. `docs/rom_map.md`'s "Confirmed: a shared
physics/collision subsystem, entered from multiple different entity
types" section directly reads `sub_0800D18C` and traces its entry point
(`sub_0800D18C` <- `sub_80109A4` <- `sub_8009868` <- `sub_800AB9C`,
itself vtable-dispatched from a *different* entity-vtable family than
any previously-documented one) to conclude this is **shared
infrastructure inside the confirmed `game_loop` zone**
(`0x08006C00`-`0x0801E578`), not part of any single entity type's own
behavior. `tools/report_units.py`'s `UNITS` list has been split and
recategorized `game_loop` for exactly this chunk's address span
(`0x0800D040`-`0x0800FC70`); everything past `0x0800FC70` is left as
the pre-existing `graphics` placeholder pending its own examination -
out of scope for this issue.

## What this cluster turned out to be

Per `docs/rom_map.md`, this whole neighborhood (`~0x0800D000`-
`0x08010D54`, of which this issue's 25-function/11KB list is the first
slice) is one cohesive **physics/collision subsystem**: 45 of ~75
functions there form a single connected component, entered from
multiple different entity-type vtables as shared infrastructure (edge
detection, collision response, position commit), not specific to one
entity's own behavior.

- **`sub_0800D18C`** (~1960 B, one of the largest functions in the
  entire `game_loop` zone) is the subsystem's **collision-response
  commit**: walks a linked list of nearby objects
  (`sub_8010708`/`sub_801070C`, "get next"/"get prev"), accumulates an
  edge-code value (left/right/top/bottom, presumably from
  `sub_801AB98`), then dispatches a 6-case jump table to per-edge
  handlers (`sub_800F2BC`, `sub_800F368`, `sub_800E7A8`, `sub_800EEF0`,
  `sub_800E6B0`, and a 6th case). Along the way it maintains a 5-slot
  "recently touched" object ring buffer *inside* `gUnknown_030012D8`
  itself (`+0x94` counter, `+0x98`+ array), reads the
  `gStaticData_0816BC98` 22-row/28-byte-stride per-state table, and
  ends by handing an ~8-argument packed position/rect off to
  `sub_8010D54` (the apply/commit step).
- **`sub_800D040`** (matched to this issue's understanding, parked
  under `NON_MATCHING`) is a **self-vs-player AABB overlap check** that
  reuses the exact same `+0x20`-pointer-to-table/`+0x2d`-tag/28-byte-
  stride hitbox-record convention `sub_8007B00`/`sub_8007B98` in
  `src/graphics/actor_part.c` already established (just with the
  `{s16 xOff, s16 yOff, u8 w, u8 h}` quad at record `+4`/`+6`/`+8`/`+9`
  instead of `+0xc`/`+0xe`/`+0x10`/`+0x11` - the same "differently laid
  out" variance those two functions' own doc comments already flag).
  On overlap it looks up `self`'s state id in `gStaticData_0816BBC4`
  and dispatches to either `sub_800EEF0(self, 1)` or
  `sub_800E7A8(self, 0, 0, 0)`.
- **`sub_800E494`/`sub_800E4E4`** (matched to this issue's
  understanding, parked under `NON_MATCHING`) are **bidirectional
  neighbor-list walkers** built on the same `sub_801070C`/
  `sub_8010708` "get next"/"get prev" pair `sub_0800D18C` itself uses,
  clearing (`sub_800E494`) or setting (`sub_800E4E4`, plus a
  budget-redistribution side effect on a caller-supplied `ctx`
  pointer's `+4`/`+0xc` fields) each visited neighbor's `+0x58` flag
  byte whenever its own `+0x4d & 0x7f` state byte reads 0.
- **`sub_800E08C`** (1032 B) and the 18 functions from `sub_800E560`
  through `sub_800F990` are the rest of `sub_0800D18C`'s jump-table
  targets and their own further sub-dispatches - moderately-sized
  (80-750 B) state-machine functions, each reading/writing several of
  the same `self+0x4d`/`+0x4e`/`+0x50`/`+0x64`/`+0x74`/`gUnknown_030012D8`
  fields `sub_0800D18C` and `sub_800D040` already touch, calling
  `PlaySfx`, `sub_803AD88` (one of the `bx rN` BLX-emulation
  trampolines - see `docs/rom_map.md`'s trampoline-table correction),
  and each other. Left completely raw for this pass - see "Left raw"
  below.

## Parked (`NON_MATCHING`) - 3 functions

All three are fully understood (semantics, field offsets, every branch
and call confirmed against the ROM disassembly) but don't yet produce
byte-identical output from `tools/agbcc`.

- **`sub_800D040`** (`src/system/game_loop6.c`; real bytes in
  `asm/code_3_2_17_d040.s`) - this is the *same* AABB-build primitive
  `sub_8007B98` (`src/graphics/actor_part.c`) is already parked for,
  just inlined twice in a row (once for `self`, once for the player)
  instead of called as a subroutine, plus an overlap-dispatch tail.
  `sub_8007B98`'s own doc comment documents this shape resisting
  byte-exact register allocation even in isolation ("about 10 of ~73
  instructions... which anonymous scratch register" gaps); doing it
  twice compounds rather than cancels the problem. Concretely: the ROM
  keeps exactly two extra callee-saved registers live across both
  builds (`r8` and `sb`, the latter caching `&gUnknown_030012D8` so the
  player pointer survives the `sub_803AFE4`/`sub_803AFDC` calls'
  clobber), reusing `r7`/`r8` for the X/Y "shift" values across *both*
  blocks. Every variant tried here (explicit `xShift`/`yShift` locals
  shared across both blocks; a `vu8` volatile cast on the second
  `self+0x28` bit-test in each block, mirroring the CSE-blocking trick
  `sub_8007B98` needed; hoisting/flattening the player-box locals in
  and out of a nested scope) lands on *three* extra callee-saved
  registers (`r8`/`r9`/`sl`) instead of the ROM's two, and/or moves
  `self` itself out of `r6` into `r8`. Parked rather than keep chasing
  individual register letters.
- **`sub_800E494`/`sub_800E4E4`** (`src/system/game_loop7.c`; real
  bytes in `asm/code_3_2_17_e494.s`) - every operation, operand and
  branch matches the ROM one-for-one, including gcc naturally finding
  the same "reuse the just-computed `ands` result register as the
  literal 0/1 being stored" trick the ROM uses, and (for
  `sub_800E4E4`) hoisting the literal `1` into a register kept live
  across each loop, exactly like the ROM's `r7`/`r6`. The one remaining
  gap, 4 occurrences across both functions: the ROM materializes the
  `0x7f` mask immediate *before* the `ldrb` byte load
  (`movs r2,#0x7f; ldrb r0,[r0]; ands r2,r0`), while this compiler
  always schedules the load first regardless of operand order in the
  source (`mask & *cur` vs `*cur & mask`, `!(...)` vs `(...) == 0` -
  all tried, same scheduling). Parked rather than chase a
  scheduler-internal tie-break.

## Left raw (untouched) - 20 functions

- **`sub_0800D18C`** (`asm/code_3_2_17_d18c.s`, ROM `0x0800D18C`,
  ~1960 B) and **`sub_800E08C`** (same file, ROM `0x0800E08C`,
  1032 B) - both read at a high level via `docs/rom_map.md` (see
  above), but not understood branch-by-branch with the precision a
  byte-exact reconstruction needs. `sub_0800D18C` in particular calls
  27 other functions in this same still-raw neighborhood and was
  already flagged there as needing "a dedicated pass" of its own,
  larger in scope than this single chunk issue.
- **`sub_800E560`, `sub_800E620`, `sub_800E6B0`, `sub_800E7A8`,
  `sub_800E888`, `sub_800EAFC`, `sub_800ED08`, `sub_800EDBC`,
  `sub_800EEF0`, `sub_800F06C`, `sub_800F1B8`, `sub_800F258`,
  `sub_800F2BC`, `sub_800F368`, `sub_800F4F4`, `sub_800F5B8`,
  `sub_800F6B8`, `sub_800F798`, `sub_800F8E0`, `sub_800F990`**
  (`asm/code_3_2_17_e560.s`, ROM `0x0800E560`-`0x0800FC70`) - the rest
  of `sub_0800D18C`'s jump-table targets and their own further
  sub-dispatches (see "What this cluster turned out to be" above);
  each individually readable but not attempted here given the size of
  the remaining list and this subsystem's documented difficulty - left
  untouched rather than force a low-confidence match or park.

## Real gotchas found along the way (useful beyond this issue)

1. **A parked function's raw bytes don't need to move.** Unlike a
   truly-matched function (which must be physically cut out of its
   `asm/*.s` file into the target `.c` file, with `ldscript.txt`
   updated), a *parked* function's real bytes can stay exactly where
   they are, wrapped in `.if NON_MATCHING == 0` / `.endif` - see
   `asm/code_3_2_2.s`'s existing `sub_8007B00`/`sub_8007B98` for the
   established pattern this issue's `code_3_2_17_d040.s`/
   `code_3_2_17_e494.s` follow. The `#if NON_MATCHING` C reconstruction
   in the target `.c` file only ever gets linked for
   `make NON_MATCHING=1 report` (which compiles every `src/*/*.c` file
   standalone via a wildcard, never through `ldscript.txt`); the real
   `make compare` build excludes it and falls back to the guarded raw
   bytes at the same address, so ROM layout is unaffected either way.
   Splitting `asm/code_3_2_17.s` into `code_3_2_17.s` (head) /
   `code_3_2_17_d040.s` (guarded) / `code_3_2_17_d18c.s` (plain raw,
   untouched functions) / `code_3_2_17_e494.s` (guarded) /
   `code_3_2_17_e560.s` (tail) was still necessary here only because
   two *non-contiguous* islands (`sub_800D040` alone, then
   `sub_800E494`/`sub_800E4E4` after the untouched
   `sub_0800D18C`/`sub_800E08C` block) needed their own `.c` files per
   `docs/workflow.md`'s "one `.c` file per contiguous ROM region" rule
   - each new `.c` file still needs `ldscript.txt` to place it (even
   though it compiles empty in the real build), so its guarded raw
   companion has to sit at a matching, separately-nameable position.
2. **The `sub_8007B00`/`sub_8007B98` AABB-build shape is a known,
   confirmed-resistant register-allocation case**, now hit a second
   time (`sub_800D040`, inlined twice). Anyone matching another
   function built on the same `+0x20`/`+0x2d`/28-or-0x1c-byte-stride
   hitbox-record convention should expect the same "which anonymous
   scratch register" gap rather than re-deriving it from scratch.
3. **A `0x7f`-mask-immediate-before-`ldrb`-load instruction order is
   not controllable from the C source** in this compiler, at least for
   the `(mask & byteLoad) == 0` shape hit 4 times across
   `sub_800E494`/`sub_800E4E4` - every operand-order/negation phrasing
   tried produced the same (load-first) schedule. Worth checking
   against other still-open parked functions with the same shape
   before spending more time on it.

## Second pass: `sub_800D040`/`sub_800E494`/`sub_800E4E4` matched via NAKED transcription

All three functions parked above are now byte-exact matched, confirmed
by a full clean `make compare` ("La suma coincide"). Rather than keep
chasing gcc 2.9's register-allocation/instruction-scheduling gaps
documented above, each was converted to `NAKED` and its ROM
disassembly transcribed instruction-for-instruction (mnemonics
translated from the disassembler's unified syntax to the plain/divided
syntax this project's other `NAKED` functions use -
`adds`->`add`, `movs`->`mov`, `ands`->`and`, `lsls`/`lsrs`->`lsl`/`lsr`,
`asrs`->`asr`, `orrs`->`orr`, `rsbs rX,rX,#0`->`neg rX,rX` - with the
original `_08XXXXXX:` labels renumbered to GNU-as local numeric
labels), the same escape hatch this project already established for
`sub_8001CB8`/`sub_8001DB4` (`src/system/link_cable.c`, see
`docs/matching/issue-4-sio-settings-sync.md`'s "The general strategy
for the rest" section). The semantics-understanding paragraphs in each
function's doc comment were kept; the now-obsolete "here's exactly
what doesn't match" paragraphs were trimmed to a one-line pointer at
this gotcha instead.

Since both `asm/code_3_2_17_d040.s` and `asm/code_3_2_17_e494.s` held
nothing but their one/two guarded functions, both files are now empty
and were deleted, with their `ldscript.txt` lines dropped (their
`.c` files - `src/system/game_loop6.c`/`src/system/game_loop7.c` -
were already correctly positioned in `ldscript.txt` from the original
parked pass, so no other `ldscript.txt` changes were needed for these
two).

## Cross-references

- `docs/status/game_loop.md` - matched/parked/raw lists updated,
  including the `graphics` -> `game_loop` recategorization for this
  address span.
- `tools/report_units.py` - `UNITS` list split/recategorized for
  `0x0800D040`-`0x0800FC70`.
- `docs/rom_map.md` - "Confirmed: a shared physics/collision
  subsystem, entered from multiple different entity types" and the
  preceding "Cross-checked the `UpdateGameFrame`-`MainLoop` cluster"
  section are the read-only reconnaissance this issue's matching work
  is based on.
