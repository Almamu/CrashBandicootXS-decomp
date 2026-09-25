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

## Phase 2, higher-address half: `sub_800EEF0`-`sub_800F990` closed

One of two parallel Phase 2 passes over this cluster's remaining
18-20-function tail, split by address range (see Phase 1's "grouping
hint" above). This pass covers the **higher-address half**: the twelve
functions from `sub_800EEF0` up through the end of the whole cluster,
`sub_800F990` (`0x0800EEF0`-`0x0800FC70`) - `sub_800EEF0`, `sub_800F06C`,
`sub_800F1B8`, `sub_800F258`, `sub_800F2BC`, `sub_800F368`,
`sub_800F4F4`, `sub_800F5B8`, `sub_800F6B8`, `sub_800F798`,
`sub_800F8E0`, `sub_800F990`. A sibling pass covers the lower-address
half (`sub_800E560` through `sub_800EDBC`) separately.

All twelve are now **matched via NAKED transcription**, confirmed by a
full clean `make compare` ("La suma coincide"), the same escape hatch
Phase 1 used for `sub_0800D18C`/`sub_800E08C`: every one of them
re-triggers either the `self+0x20`-table/`+0x2d`-tag/28-byte-stride
AABB-build register shape, or plain high-register (`r8`/`sb`/`sl`)
cross-block reuse under `-O2` this compiler's allocator doesn't
reproduce - and given this batch's size (12 functions, ~1730 lines of
disassembly), transcription was the reliable path to a byte-exact result
for all of them at once. New file `src/system/game_loop49.c`; real bytes
formerly the tail of `asm/code_3_2_17_e560.s` (from `sub_800EEF0`
onward - that file now ends right after `sub_800EDBC`, the sibling
pass's own territory).

**Mechanics note for future NAKED-transcription passes**: this pass's
transcription was done with a small Python script (mnemonic map +
per-function local-label renumbering, the same algorithm Phase 1
describes) rather than by hand, which is worth reusing for any future
batch this size. One real bug surfaced and fixed while writing it: a
`.4byte` literal-pool entry whose *value* is itself a same-function local
label (a jump-table base address referenced via `ldr rX, =tableLabel`
immediately followed by `tableLabel: @ jump table`, hit once in
`sub_800F5B8`) needs its value resolved to an `Nf`/`Nb` local reference
too, not just the pool entry's own defining label - naively emitting the
raw ROM label name there produces an undefined-symbol assembler error.
A second near-miss: a naive `_[0-9A-F]{8}` regex for "is this operand a
local label reference" also matches *inside* an unrelated global
symbol's name (e.g. `gUnknown_030012D8` contains `_030012D8`, which
satisfies the same 8-hex-digit pattern) - needs a
`(?<![A-Za-z0-9_])...(?![0-9A-F])` boundary guard or it corrupts global
symbol references in pool data.

**Isolated-compile verification caveat (confirmed again here)**: an
isolated `cpp`/`agbcc`/`as` + `objcopy`/`cmp` pass against the raw ROM
bytes at `0x0800EEF0` shows ~470 of 3456 bytes differing purely from
unresolved relocations (`bl` targets not yet linked, and literal-pool
words for global addresses reading as 0 in the unlinked object) - not a
real mismatch. Disassembling both sides and diffing confirms every
divergent byte is one of exactly these two categories; the authoritative
check is the full clean `make compare`, which passed outright.

### Per-function roles

- **`sub_800EEF0(self, u8 arg1)`** - the per-edge dispatch's shared
  **case 4 target** (both `sub_0800D18C`'s and `sub_800E08C`'s own case
  4: `self+0x4d & 0x7f == 0` gates a call with `arg1=1`). Also called
  directly by several siblings below (`arg1=0`) whenever their own
  overlap/state checks reach the same "commit an edge collision"
  outcome. Clears `self+0x4f`/`self+0x4d`'s low 7 bits, sets `self+0xc`
  bit `0x10`, re-adds `self` to `gUnknown_0300130C`'s active list
  (`sub_8009150`), sets `self+0x4d` bit `0x80` then ORs in `arg1` as the
  low bit. Tags `self+0x2d` (`0x21`, or a `self+0x4e-0x21`-offset
  rewind when already `0xa`) via the `sub_80087C0`/`sub_80087B4`/
  `sub_800872C` "set tag, refresh sprite/animation" triplet every
  state-transition function in this cluster shares. Marks a cell in
  `gUnknown_030012B4`'s 32x32 collision bitmap, plays a fixed sound
  (id 4), calls `sub_800EDBC(self)` (sibling pass's territory), and -
  gated on a combo/proximity check against `gUnknown_0300082C`/
  `gUnknown_030012D8+0x8c` - `sub_803AD88(self, 0, 4, 0)`. Forces
  `self+0x4e = 0x13` in the common case (see `sub_800F8E0` below).
- **`sub_800F06C(self, u32 arg1)`** - called only by `sub_800F798`
  below (`arg1` = `0x14` or `0x28`, a proximity radius). Two
  `gUnknown_0300130C` list-scan passes: settles every nearby object via
  `gStaticData_0816BBC4`/`gStaticData_0816BBAE`-driven dispatch to
  `sub_800E7A8`/`sub_800F368`/`sub_800F2BC`/`sub_800EEF0`, then a second
  pass over `gUnknown_030012EC` calling `sub_8011448`. Resets
  `self+0x48` to the `-1` sentinel at the end.
- **`sub_800F1B8(void)`** - no arguments. Calls `sub_800F258` first
  (flush pending case-`0xa` commits), then an up-to-twice
  `gUnknown_0300130C` list scan removing/re-classifying objects via
  `sub_8009AA0`/`sub_803AD80`/`sub_803AD7C`, driven by a
  `gUnknown_030012B0` one-shot re-scan flag.
- **`sub_800F258(void)`** - no arguments. Settles every
  `gUnknown_0300130C` object stuck at `+0x4e==0xa`/`+0x4d&0x7f==0` via
  `sub_800EEF0(other, 0)`. Called by both `sub_800F1B8` and
  `sub_800F2BC` as a "flush leftovers from last frame" first step.
- **`sub_800F2BC(self)`** - per-edge dispatch id-row-`6` target (shared
  by `sub_0800D18C`'s/`sub_800E08C`'s case 0/1). Tags `self+0x2d=0x23`,
  runs the tag/refresh triplet plus a `sub_8006DF8`-driven `self+0x29`
  nibble update, flushes via `sub_800F258`, bumps a combo counter
  (`sub_8028474`), plays sound id 4, arms `self+0x48=1`.
- **`sub_800F368(self)`** - per-edge dispatch id-row-`3` target (sibling
  of `sub_800F2BC`, same case). Tags `self+0x2d=0x22`, same triplet +
  nibble update, marks the collision bitmap (`sub_8025A0C`), then scans
  `gUnknown_0300130C` for up to 0x20 simultaneously-triggered
  same-`+0x50`-group neighbors, allocating (`sub_8026EC0`) a linked
  group list at `self+0x48` when any are found (`-1` sentinel
  otherwise). Seeds `self+0x4f` from `self+0x4c`.
- **`sub_800F4F4(self)`** - bumps `self+0x50` against a `self+0x51`
  cap; once reached, frees `self+0x48`'s group (`sub_8026EB4`) and
  resets to the `0x13`/`0x14`/`0x15` "settle" family via `self+0x4e=7`.
  While under the cap, recursively settles every other member of
  `self+0x48`'s triggered group via `sub_800F5B8`, playing one shared
  sound (id `0xf`) for the batch.
- **`sub_800F5B8(self)`** - the group-settle worker `sub_800F4F4`
  calls. Reinterprets `self+0x48` as a byte offset subtracted into
  `self+0x4e`, then a 0x13-entry jump table mapping each resulting
  sub-case to one of a small set of `self+0x2d` tag constants, each
  through the same tag/refresh triplet, converging on a
  `sub_800815C`-driven nibble update.
- **`sub_800F6B8(s32 x, s32 y, s32 arg2, s32 arg3)`** - the one function
  here taking a raw probe box instead of `self` (existing extern in
  `actor_part.c`: `sub_800F6B8(part->x>>8, part->y>>8, 0x40, 0x12)`).
  Scans `gUnknown_0300130C` for objects within `(arg2,arg3)` of the box,
  dispatching via `gStaticData_0816BBC4`/`gStaticData_0816BBAE` to
  `sub_800E6B0`/`sub_800E7A8`/`sub_800EEF0` - the same "settle nearby
  objects" shape as `sub_800F06C`/`sub_800F798`, box-driven instead of
  `self`-driven.
- **`sub_800F798(self)`** - the per-edge dispatch's re-entry point once
  `self+0x4d&0x7f==1` (commit already underway; called from the
  still-raw `0x080104E4` continuation, outside this issue's scope).
  Dispatches to `sub_800F06C` per `self+0x30`/`gStaticData_0816BBC4`,
  unlinks `self` from its neighbor list when `self+0x38` is set
  (`sub_8010710`/`sub_8010714`), and marks/clears
  `gUnknown_030012B0`/`gUnknown_030012D8+0x94`'s ring-buffer re-visit
  bookkeeping.
- **`sub_800F8E0(self)`** - the `0x13`/`0x14`/`0x15` "settle" family's
  own small state cycle, guarded by `self+0x4f`'s cooldown throttle.
  `0x14`->`0x13` and `0x15`->`0x14` each retag/replay the triplet, play
  a sound (id `0x11`), and set a ~1s cooldown; `0x13` (once
  `self+0x4d&0x7f==0`) calls `sub_800EEF0(self, 0)`, closing the loop
  back to the top of this list.
- **`sub_800F990(self)`** - the cluster's last and largest function
  (~736 B), called for `self+0x4e==0xf`. A per-frame position-wrap/
  edge-scan advance structurally similar to the already-parked
  `sub_800FC70` that immediately follows this whole cluster (see
  `docs/matching/issue-13-fc70-continuation.md`): clamps `self` within
  0x4f/0x3f px of the player into a packed `self+0x48` byte, then (only
  when `self+0x2d==8`) runs a 4-phase `self+0x48&7` state rotation
  calling `sub_8010A50` to decide whether each phase continues or
  commits. Self-contained (no calls out to any other function in this
  cluster) - a full branch-by-branch semantic write-up was not attempted
  for this pass, since the NAKED transcription's byte-exactness doesn't
  depend on it.

## Cross-references

- `docs/status/game_loop.md` - matched/parked/raw lists updated,
  including the `graphics` -> `game_loop` recategorization for this
  address span, Phase 1's `sub_0800D18C`/`sub_800E08C` closure, and
  Phase 2's `sub_800EEF0`-`sub_800F990` closure (higher-address half).
- `tools/report_units.py` - `UNITS` list split/recategorized for
  `0x0800D040`-`0x0800FC70`, Phase 1's entry updated to point at the
  new `src/system/game_loop47.o`, and Phase 2's `0x0800E560` entry split
  in two at `0x0800EEF0` to point the higher half at the new
  `src/system/game_loop49.o`.
- `docs/rom_map.md` - "Confirmed: a shared physics/collision
  subsystem, entered from multiple different entity types" and the
  preceding "Cross-checked the `UpdateGameFrame`-`MainLoop` cluster"
  section are the read-only reconnaissance this issue's matching work
  is based on.
