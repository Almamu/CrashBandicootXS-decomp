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

## Phase 1: `sub_0800D18C`/`sub_800E08C` closed - the Phase 2 dispatch map

Foundational pass for a second staged effort over this subsystem's
remaining 20-function tail (`0x0800D18C`-`0x0800FC70`,
`asm/code_3_2_17_d18c.s`/`asm/code_3_2_17_e560.s`), mirroring the
dispatcher-first/parallel-leaves approach already used to close the
`0x0800B8DC`-`0x0800D040` cluster (issue #9/#10). Both of the two
"left raw" dispatchers this issue's first pass flagged above
(`sub_0800D18C`, `sub_800E08C`) are now **matched via NAKED
transcription**, confirmed by a full clean `make compare`
("La suma coincide"). Real bytes formerly in `asm/code_3_2_17_d18c.s`
(now deleted, fully consumed - both functions folded into the new
`src/system/game_loop47.c`, inserted in `ldscript.txt` between
`game_loop6.o` and `game_loop7.o`, exactly where the raw file used to
sit).

**Size correction**: `sub_0800D18C` is **~3840 B**
(`0x0800D18C`-`0x0800E08C`), not the ~1960 B this issue's original
read-only pass (and `docs/rom_map.md`) estimated - it's nearly twice
the size, one of the largest single functions in the whole ROM.
`sub_800E08C` (`0x0800E08C`-`0x0800E490`, 1032 B) was already sized
correctly.

**Why NAKED, not real C**: three nested jump tables, ~30 distinct
callees, and (confirmed again here) the same gcc-2.9-resistant
AABB-build register shape `sub_800D040`'s own doc comment above
already documents (`r7`/`r8`/`sb` cross-block reuse) - twice more,
inside a function roughly four times the size of any other function
this project has attempted as real C. Transcribed
instruction-for-instruction from the ROM disassembly instead (mnemonics
translated `adds`->`add`, `movs`->`mov`, `ands`->`and`,
`lsls`/`lsrs`->`lsl`/`lsr`, `asrs`->`asr`, `orrs`->`orr`; original
`_08XXXXXX:` labels renumbered to GNU-as local numeric labels assigned
in first-definition order, each reference emitted with the correct
`f`/`b` suffix by comparing source position), the same escape hatch
already established for `sub_800D040`/`sub_800E494`/`sub_800E4E4`
above. Verified in two stages: an isolated `cpp`/`agbcc`/`as` +
`objcopy`/`cmp` pass against `baserom.gba` first (bytes matched up to
the first `bl` - the expected relocation-site divergence for an
unlinked object; both jump tables' own internal `.4byte` pool words
also diverge in this isolated check purely because the `.text`
section isn't yet relocated to its real `0x0800D18C` VMA, another
expected false-positive at this stage, not a real bug), then the
authoritative full clean `make compare`, which passed outright.

### `sub_0800D18C`'s three jump tables

`sub_0800D18C(void *self, u32 arg1)` runs three separate jump-table
dispatches in sequence, not the single 6-case table the original
read-only pass described (that one is real, it's the *second* of the
three):

1. **Hitbox-source selector**, ROM `0x0800D2AC`, 7 cases (0-6), keyed
   by `(self's hitbox-record byte at +4) >> 4`, `bhi`-gated at 6:
   - Cases 0, 4: use `self`'s own just-built AABB (`r2+0x1c`).
   - Cases 1, 2, 3, 5, 6: fall back to a fixed box,
     `gStaticData_0816B2F8`.
   The selected box is tested for a zero width/height
   (`gStaticData_0816B2F8+4`/`+5` both 0 => treated as "no box", early
   return) then overlap-tested against the player's own hitbox record
   (`sub_803AFE4`/`sub_803AFDC` + `sub_8001688`). No overlap => early
   return before either of the other two tables is reached.
2. **Per-edge handler dispatch**, ROM `0x0800D454`, 6 cases (0-5),
   keyed by an accumulated edge-code value (`r6`, built from
   `self+0x28`/hitbox-record comparisons and `gStaticData_0816BC98`
   lookups above) - **this is the 6-case table the original read-only
   pass already described**, now confirmed byte-for-byte:
   - **Case 0** (`0800D46C`): reads a dispatch-id byte from
     `gStaticData_0816BC98` (cached in `sb`/`r8+0x4e`'s row); if it's
     `6`, calls **`sub_800F2BC(self)`**; if `3`, calls
     **`sub_800F368(self)`**; otherwise no call.
   - **Case 1** (`0800D46C`): identical target to case 0 (shared code).
   - **Case 2** (`0800D48A`): no callee - just sets `self+0x4d` bit
     `0x80` and `gUnknown_030012D8+0x80 = 1`.
   - **Case 3** (`0800D4A8`): the most complex case - calls
     **`sub_800E7A8(self, 0, 0, 0)`**, then (unless
     `gUnknown_030012D8+0x94 != 0`) rebuilds `self`'s hitbox pointer,
     and if the dispatch id (`sp+0x78`) isn't 3, calls
     **`sub_800CEAC`** (already matched, `game_loop42.c`) to test a
     player-sized box at that spot; on overlap, walks to the "prev"
     neighbor (`sub_801070C`) and, if that neighbor's own `+0x4d&0x7f`
     isn't 1, looks up *its* dispatch id in `gStaticData_0816BC98` and
     calls **`sub_800E7A8(neighbor, 0, 0, 0)`** again (id 3),
     `strb`-sets a flag and **`sub_800EEF0(neighbor, 1)`** (id 4), or
     nothing (other ids).
   - **Case 4** (`0800D684`): if `self+0x4d & 0x7f == 0`, calls
     **`sub_800EEF0(self, 1)`**.
   - **Case 5** (`0800D6A2`): calls **`sub_800E6B0(self)`**.
   All six cases converge on a shared tail (`0800D6AC`) that adjusts
   the dispatch id (`sp+0x78`) for two special cases (id 5 + a
   `gUnknown_030012D8+0x24==4` gate rewrites to id 2; id 3 rewrites to
   id 1), then - unless `self+0x58` was set and `self+0x44` is 0 (early
   return) - calls **`sub_800E4E4(self, &localAABB)`** (already
   NAKED-matched, `game_loop7.c`) when the id isn't 6, before falling
   into the third table.
3. **Post-processing dispatch**, ROM `0x0800DD70`, 9 cases (0-8), keyed
   by the same adjusted dispatch id (`sp+0x7c`), `bls`-gated at 8:
   - **Case 0, 3, 5, 6, 7**: all share the same target, `0800E00C` -
     effectively a no-op fast path straight to the function's shared
     tail (apply-offset + `sub_8010D54` call).
   - **Case 1, 2** (`0800DEAC`): overlap-test `self`'s and a
     recomputed box (`sub_8001640`); on **no** overlap, calls
     **`sub_800E494(self)`** (already NAKED-matched, `game_loop7.c`)
     and clears the "apply offset" flag; on overlap, runs a large block
     of edge-distance comparisons that (depending on direction/gap)
     calls **`sub_800B324`** (external, unread) and/or
     **`sub_800FDC8`** (already matched, `game_loop33.c`, the
     Bresenham line-stepper) to decide a final offset direction.
   - **Case 4** (`0800DD94`): calls **`sub_801095C`** (already matched,
     `game_loop30.c`) to reselect `self`, re-reads its
     `gStaticData_0816BC98` row, and - if not filtered out - calls
     **`sub_803AD88`** (the `bx r4` trampoline; a sound/particle-effect
     function pointer loaded from `self+0x18+0x68`/`+4`) with a fixed
     arg pattern (`0, 0xc, 4`).
   - **Case 8** (`0800DE14`): calls **`sub_8010914`** (already matched,
     `game_loop30.c`) to reselect `self`, similarly re-reads its
     `gStaticData_0816BC98` row, optionally resets
     `gUnknown_030012D8`'s `+0x64`/`+0x54`/`+0x58`/`+0x5c` fields, and
     calls **`sub_8007398`** (already matched, `graphics.c` - applies
     the computed position offset).
   All paths converge on the shared tail at `0800E00C`, which
   conditionally calls **`sub_800E620`** (already-matched-elsewhere
   leaf; gated on `gUnknown_030012D8+0x88==1`, `self+0x4e==0xe`, and a
   re-overlap test), then **`sub_8007398`** (apply the final offset)
   and, if a sound/effect id was set, **`sub_803AD88`** again, then
   ends by calling **`sub_8010D54`** (already matched) with ~8 packed
   arguments - the actual apply/commit step.

### `sub_800E08C`'s jump table

`sub_800E08C(void *self, u32 arg1, u32 arg2, u32 arg3, u8 arg4, u8 arg5, u8 arg6)`
runs a **single** 6-case jump table, ROM `0x0800E27C`, keyed by `r7`
(itself derived from `arg1`/`arg2` plus a `gStaticData_0816BC98`-driven
special-case rewrite for id 1, and a `gUnknown_030012D8+0x92`-gated
rewrite to id 1 that also mutates `self+0x48`/state byte `+0x4e`):

- **Case 0, 1** (`0800E294`): identical to `sub_0800D18C`'s own case
  0/1 - reads `self+0x4e`; `6` => **`sub_800F2BC(self)`**, `3` =>
  **`sub_800F368(self)`**, else nothing.
- **Case 2** (`0800E342`): reads `self+0x4e`; `0xe` =>
  **`sub_800E620(self)`**, `0xc` => **`sub_800E560(self)`**, else no
  callee - just sets `self+0x4d` bit `0x80` and
  `gUnknown_030012D8+0x80 = 1` (same as `sub_0800D18C`'s own case 2,
  `sub_800E620`/`sub_800E560` are new here).
- **Case 3** (`0800E37C`): a multi-way gate on `arg1`/`self+0x44`/
  `arg2`/`arg3` that always ends in one **`sub_800E7A8(self, 0, ...)`**
  call with a different 3rd/4th argument per branch (`0`, `4`,
  `byte[sp]`+`arg3`, or `0`+`arg3`), optionally also recording `self`
  into `gUnknown_030012D8`'s ring buffer and bumping its `+0x94`
  counter.
- **Case 4** (`0800E41C`): if `self+0x4d & 0x7f == 0`, calls
  **`sub_800EEF0(self, 1)`**.
- **Case 5** (`0800E434`): calls **`sub_800E6B0(self)`**.
- Shared tail (`0800E43C`): calls `sub_8007398` (apply the accumulated
  offset, gated on `gUnknown_030012D8+0x1084`'s `+4` byte) and, if a
  sound/effect id was set (`sp+0x38`), `sub_803AD88`.

This confirms `sub_0800D18C`'s and `sub_800E08C`'s per-edge dispatch
tables really are the same shared handler family (identical case
ordering, identical targets for cases 0/1/4/5, case 3 always landing on
`sub_800E7A8`) - `sub_800E08C` is a second, narrower entry point into
the same six leaf handlers, called only from `sub_0800D18C`'s own
9-case table (cases 1/2/4, per that table's targets `0800DEAC`/
`0800DD94`/`0800DE14` above - none of those actually call
`sub_800E08C` directly by name in the disassembly read here; note for
Phase 2 to double-check the exact call site if this matters for a leaf
function's own understanding).

### Phase 2 grouping hint

Of this issue's remaining 18-20-function tail
(`sub_800E560`-`sub_800F990`), exactly **7 are direct callees** of the
two now-matched dispatchers: `sub_800E560`, `sub_800E620`,
`sub_800E6B0`, `sub_800E7A8`, `sub_800EEF0`, `sub_800F2BC`,
`sub_800F368`. The other ~12 (`sub_800E888`, `sub_800EAFC`,
`sub_800ED08`, `sub_800EDBC`, `sub_800F06C`, `sub_800F1B8`,
`sub_800F258`, `sub_800F4F4`, `sub_800F5B8`, `sub_800F6B8`,
`sub_800F798`, `sub_800F8E0`, `sub_800F990`) were not seen called from
either dispatcher directly in this pass - they're presumably called
transitively by one or more of the 7 direct entry points (each
individually still unread). A reasonable parallel-safe split: one
group per direct entry point (7 groups), each agent reading its entry
point first to discover which of the ~12 remaining leaves it pulls in,
rather than guessing the sub-call graph up front from this doc alone.

## Phase 2 (lower-address half): sub_800E560-sub_800EDBC closed

Closes 8 of the ~18-20 remaining leaf functions from Phase 1's grouping
hint: the lower-address group of direct dispatch targets
(`sub_800E560`, `sub_800E620`, `sub_800E6B0`, `sub_800E7A8`) and their
own transitive callees, everything up to but not including
`sub_800EEF0` (a sibling parallel pass's own territory, covering
`sub_800EEF0`-`sub_800F990`). All 8 verified byte-exact by a full clean
`make compare` ("La suma coincide"). New file `src/system/game_loop48.c`,
inserted in `ldscript.txt` between `game_loop7.o` and (the now-trimmed)
`code_3_2_17_e560.o`. `asm/code_3_2_17_e560.s` trimmed to begin at
`sub_800EEF0` (its own header directives kept, since the sibling pass
still needs the rest of the file).

**Transitive closure derivation**: reading each of the four direct
targets' own bodies (not just the dispatch map) found the actual
sub-call graph is narrower than "each dispatcher owns one of the 7
direct-callee groups" - `sub_800E560`/`sub_800E620`/`sub_800E6B0` call
nothing else in this still-raw neighborhood (only already-matched
siblings, `PlaySfx`, `rand`, or each other within the direct-target
set), while `sub_800E7A8` calls `sub_800E888` (both times it needs a
"dispatch id" leaf handler, cases 3/22 of `sub_800E7A8`'s own logic),
and `sub_800E888` in turn calls `sub_800EDBC`, `sub_800EAFC`, and
`sub_800ED08` (the latter two also reachable directly from
`sub_800E888`'s own 23-case jump table) - plus `sub_800EEF0`, left for
the sibling pass since it's out of this range. That's exactly 8
functions, no more, no fewer, in this half.

### What each function does

- **`sub_800E560`** - dispatch-id-5 handler (both dispatchers' case 5).
  Arms `self`'s `+0x48` frame-countdown to `0x168` the first time it's
  seen at its sentinel value, clearing a `+0x51` retry counter
  alongside it. While that countdown runs and `self+0x50` is zero,
  bumps `+0x51` each call; past 4 retries (or once `+0x48` itself
  expires), hands off to `sub_800E7A8(self, 0, 0, 0)`. Otherwise arms
  `self+0x4d` bit `0x80`, `gUnknown_030012D8+0x80 = 1`, a fresh
  `+0x4f = 6` sub-timer, and spawns a pair of `sub_8025CA4` particle
  effects (kind `0xe`) at `self`'s position, offset `-6`/`+3` pixels on
  Y/X.
- **`sub_800E620`** - case-2 handler (dispatch id `0xe`, both
  dispatchers). Switches `self` into hitbox tag `0x14`, rebuilds its
  hitbox record (the `sub_80087C0`/`sub_80087B4`/`sub_800872C` trio
  every hitbox-rebuild call in this subsystem uses), registers it with
  the object-pool grid (`sub_8009150`), re-derives a low-nibble
  sub-animation value from the freshly selected hitbox record's `+0x14`
  byte via `sub_8006DF8`'s tile-asset-cache lookup, plays SFX `0x11`,
  arms a `+0x4f = 0x3c` (60-frame) countdown.
- **`sub_800E6B0`** - dispatch-id-5's own sibling case (both
  dispatchers' case 5, same table slot `sub_800E560` covers on the
  *other* dispatcher; the two are not actually the same handler despite
  sharing a case index - each dispatcher's 6-case table independently
  selects its own target per row). Spawns a particle-effect object
  (`sub_8025BAC`, kind `0x2a`) at `self`'s position (minus 10 pixels on
  X), initializes its trajectory fields, switches `self` itself into
  hitbox tag `0x1b`, rebuilds its hitbox record, plays SFX `0x17`,
  notifies `sub_80259D4` unless `self+8` is the sentinel `0xffff`,
  conditionally reactivates the viewport, tells `sub_8022CA0` whether
  `self+0x50` is nonzero, and resets `self+0x4d` to `1`.
- **`sub_800E7A8(self, edgeFlag, walkFlag, dir)`** - case-3 handler
  (both dispatchers). Counts `self` into `gUnknown_030012D8+0x91`'s
  "objects handled this frame" tally (gated on `walkFlag`), then walks
  `self`'s neighbor chain (`dir==4` "get prev", `dir==8` "get next")
  past every node whose `+0x4d & 0x7f` state is already `1`, stopping
  at the first node that isn't (or the last reachable node if the
  whole chain is state `1`; neither `dir` value falls back to `self`
  itself). Unless `gStaticData_0816BBDA[target+0x4e]` is nonzero,
  dispatches to `sub_800E888(target, edgeFlag)`.
- **`sub_800E888(self, edgeFlag)`** - `sub_800E7A8`'s shared tail.
  Early-outs if `self+0x4d & 0x7f == 1`. Otherwise registers `self`
  with the object-pool grid, resets `self+0x4d` to `0x81`, switches
  `self` into hitbox tag `0x1d` and rebuilds its record, re-derives its
  `+0x29` sub-animation value and clamps `+0x30`'s index to the newly
  selected record's own `+0x16` count, conditionally reactivates the
  viewport, flips one bit of `gUnknown_030012B4`'s bit-grid keyed by
  `self+8`, calls `sub_800EDBC` (neighbor "impact spread"
  propagation), then dispatches its own 23-case jump table on `self`'s
  freshly-cached `+0x4e` state id to one of
  `sub_801085C`/`sub_801089C`/`sub_800F368`/`sub_800F2BC`/
  `sub_800EAFC`/`sub_800ED08`/`sub_800EEF0`/`sub_8022EA8`/a
  SFX-3-plus-particle-spawn fallback (`sub_8025CA4`) - the largest
  jump table in this subsystem after `sub_0800D18C`'s own three.
- **`sub_800EAFC(self, walkFlag)`** - case-11 handler of
  `sub_800E888`'s table (dispatch id `0xb`). Plays SFX 3, then (the
  first time `self+0x51` is exactly `9`) rolls a random "escalation
  level" (`1`/`4`/`7`/`8`) into that byte. Dispatches its own 10-case
  jump table on `(self+0x51 - 1)`: cases 5 down through 0 deliberately
  cascade-fall-through into each other (an escalating "more debris"
  particle burst, `sub_8025CA4`, at slightly different offsets the
  further the level counted down); case 6 fires a screen-shake
  (`sub_803AD88`) plus SFX; case 7 spawns a `sub_8025A64` bonus object;
  case 9 spawns one final small puff.
- **`sub_800ED08(self, walkFlag)`** - case-15 handler of
  `sub_800E888`'s table (dispatch id `0xf`). Plays SFX 3, then
  switches on `self+0x48 & 7`: `1` plays SFX 3 again, notifies
  `sub_80259D4`, and spawns a `sub_8025A64` bonus object 3 pixels below
  `self`; `2` forwards to `sub_800EAFC`; `3` clears `self+0x4d` bit
  `0x80` and calls `sub_800EEF0(self, 1)`; any other value does
  nothing further.
- **`sub_800EDBC(self, walkFlag)`** - neighbor "impact spread"
  propagation, called once from `sub_800E888`'s own body (not through
  its jump table). Derives a base spread budget from `self`'s hitbox
  record's own `+9` byte, then walks `self`'s "get next" neighbor chain
  redistributing that budget across each visited node's `+0x40`/`+0x44`
  "remaining spread" fields, nudging each node's `+0x4c` byte toward 0,
  re-registering it with the object-pool grid, and - for any node past
  a `gStaticData_0816BBC4`/`+0x48`/`+0x44` threshold gate - "graduating"
  it into state `+0x48 = 1`.

### Matching notes

- **`sub_800E620`/`sub_800ED08` matched as real C** - both are fairly
  linear (no loops, `sub_800ED08` a small `switch` rather than a
  computed jump table), unlike this half's other 6 functions. Three
  distinct gcc-2.9 gaps needed register-pinned/inline-asm anchoring in
  `sub_800E620` alone, all confirmed by direct byte comparison against
  a fresh `objdump` disassembly of `baserom.gba` (not just the
  isolated-compile eyeball check, which this pass got wrong twice
  before catching it against the real linked ROM bytes - see
  "Gotchas" below):
  1. `self[0xc] |= 0x10;` - the ROM materializes the `0x10` immediate
     *before* loading `self[0xc]`, not after; a plain C statement (in
     either operand order) always loads the field first. Anchored with
     a 2-instruction inline-asm block taking `self` as an input operand
     (so the compiler substitutes whichever register it lands in,
     rather than hardcoding `r4`).
  2. The `self+0x20`-pointer-to-table/`+0x2d`-tag/28-byte-stride record
     lookup (the same convention `sub_800D040`'s "AABB1" shape uses) -
     plain C picks a different register pair than the ROM for the
     table-pointer/tag values even when the source statement order
     already matches ROM's load order; closed with explicit
     `register T x asm("rN")` pins for both, plus a small inline-asm
     block for the `tag*28 + table` address arithmetic itself (matching
     ROM's specific dest-register choice for the running sum).
  3. `self[0x29] = (self[0x29] & ~0xf) | (lo & 0xf);` - the ROM
     computes `&self[0x29]` *before* masking `lo` down to its low
     nibble (a plain C statement here always masks first regardless of
     source statement order), and materializes the `~0xf` clear-mask at
     runtime (`movs r1,#0x10; rsbs r1,r1,#0`, the negative-constant
     register-pinned mask idiom already established for
     `sub_8010480`/game_loop35.c) rather than folding it into an 8-bit
     AND immediate. Anchored as one inline-asm block covering the whole
     sequence, taking the freshly-extracted `lo` value as an in-out
     operand.
- **The other 6 (`sub_800E560`/`sub_800E6B0`/`sub_800E7A8`/
  `sub_800E888`/`sub_800EAFC`/`sub_800EDBC`) closed as NAKED
  transcriptions**, the same escape hatch Phase 1's two dispatchers
  used - jump-table density (`sub_800E888`'s 23 cases, `sub_800EAFC`'s
  10, both with cascading-fallthrough or heavily-reused pool constants)
  and/or this subsystem's confirmed `r8`/`sb`/`sl`-triple-accumulator
  shape (`sub_800EDBC`, matching `sub_0800D18C`'s own AABB-build
  register reuse) made a plain-C attempt not worth chasing given this
  project's established precedent for the same shapes elsewhere in
  this subsystem.

### Gotchas found along the way (useful beyond this issue)

1. **An isolated-compile "eyeball match" against the wrong copy of the
   disassembly is not proof, even when it looks byte-identical
   line-for-line.** This pass initially cross-checked its C
   reconstruction and its NAKED transcriptions against the raw text
   already read out of `asm/code_3_2_17_e560.s` earlier in the same
   session - and still shipped two real bugs anyway:
   - `sub_800E620`'s `self[0x29]` mask-then-address vs.
     address-then-mask ordering (see "Matching notes" above) - a
     misreading of which operation the ROM actually does first,
     caught only once the *first* full clean `make compare` attempt
     failed and the mismatch's exact byte offset was traced back with
     a fresh `objdump -D -b binary -m arm --adjust-vma=0x08000000
     -M force-thumb baserom.gba` disassembly of `baserom.gba` itself
     (not the project's own pre-split `asm/*.s`, and not the isolated
     `agbcc` output) at that precise address.
   - `sub_800E6B0`'s constant-pool placement - all 8 of this function's
     pool words genuinely sit together at the very end of the function
     (right after its own `bx r0`) in the ROM, since nothing forces
     earlier emission in a function this short with only one internal
     branch; this pass's first NAKED draft instead invented an
     interspersed placement (a pool block after each first use,
     matching the *general* pattern several of this subsystem's other,
     longer/more-branchy functions do need) without rereading the
     specific function's own already-correctly-transcribed pool
     positions before writing the `asm()` block. Since a NAKED
     function's `ldr rX, label` forward-references resolve correctly
     regardless of where `label` physically sits in the source, this
     produces working, self-consistent code that even disassembles
     plausibly - it just isn't byte-identical to the ROM's own
     instruction stream, because moving a pool word earlier shifts
     every subsequent PC-relative `ldr`'s displacement.
   - Practical upshot: for any NAKED transcription, copy the
     pool-word `.align`/`.4byte` placement from the source disassembly
     *verbatim*, in its exact original position relative to the
     surrounding code, rather than reconstructing "where a pool
     probably goes" from a general pattern seen elsewhere - and after
     a full clean `make compare` failure, drill into the exact
     mismatching address range with a *fresh* disassembly of
     `baserom.gba` itself before re-editing anything, since that's the
     only source immune to a prior transcription mistake being carried
     forward into the "reference" text being checked against.
2. **`objdump -h`/`-j` needs the ELF's actual section name, not the
   assumed `.text`.** This project's `ldscript.txt` names the ROM
   output section `ROM` (see its own `SECTIONS` block), not `.text` -
   `arm-none-eabi-objdump -d --start-address=<addr> --stop-address=<addr>
   -j .text crashbandicootxs.elf` silently reports "section '.text'
   ... not found" (exit 0, easy to miss); `-j ROM` is what actually
   filters to the right address range for pulling out one function's
   real linked bytes/disassembly for inspection.

## Cross-references

- `docs/status/game_loop.md` - matched/parked/raw lists updated,
  including the `graphics` -> `game_loop` recategorization for this
  address span, Phase 1's `sub_0800D18C`/`sub_800E08C` closure, and
  Phase 2's (lower-address half) 8-function closure.
- `tools/report_units.py` - `UNITS` list split/recategorized for
  `0x0800D040`-`0x0800FC70`, Phase 1's entry updated to point at the
  new `src/system/game_loop47.o`, and Phase 2's `0x0800E560` entry
  split into a matched `src/system/game_loop48.o` unit and a
  still-raw `0x0800EEF0` unit for the sibling pass's own range.
- `docs/rom_map.md` - "Confirmed: a shared physics/collision
  subsystem, entered from multiple different entity types" and the
  preceding "Cross-checked the `UpdateGameFrame`-`MainLoop` cluster"
  section are the read-only reconnaissance this issue's matching work
  is based on.
