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
types" section directly reads `QueueCratePlayerCollision` and traces its entry point
(`QueueCratePlayerCollision` <- `CollideCrateWithPlayer` <- `CollidePlayerWithCrates` <- `CollidePlayerWithObjects`,
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

- **`QueueCratePlayerCollision`** (~1960 B, one of the largest functions in the
  entire `game_loop` zone) is the subsystem's **collision-response
  commit**: walks a linked list of nearby objects
  (`GetCrateBelow`/`GetCrateAbove`, "get next"/"get prev"), accumulates an
  edge-code value (left/right/top/bottom, presumably from
  `ResolvePlatformCollision`), then dispatches a 6-case jump table to per-edge
  handlers (`ActivateNitroSwitchCrate`, `ActivateIronSwitchCrate`, `BreakCrateInStack`, `ExplodeCrate`,
  `OpenCheckpointCrate`, and a 6th case). Along the way it maintains a 5-slot
  "recently touched" object ring buffer *inside* `gPlayer`
  itself (`+0x94` counter, `+0x98`+ array), reads the
  `gCrateHitResponse` 22-row/28-byte-stride per-state table, and
  ends by handing an ~8-argument packed position/rect off to
  `AddCollisionCandidate` (the apply/commit step).
- **`BreakCrateTouchedByPlayer`** (matched to this issue's understanding, parked
  under `NON_MATCHING`) is a **self-vs-player AABB overlap check** that
  reuses the exact same `+0x20`-pointer-to-table/`+0x2d`-tag/28-byte-
  stride hitbox-record convention `GetSpriteBounds`/`GetSpriteHitbox` in
  `src/objects/sprite.c` already established (just with the
  `{s16 xOff, s16 yOff, u8 w, u8 h}` quad at record `+4`/`+6`/`+8`/`+9`
  instead of `+0xc`/`+0xe`/`+0x10`/`+0x11` - the same "differently laid
  out" variance those two functions' own doc comments already flag).
  On overlap it looks up `self`'s state id in `gCrateKindExplosive`
  and dispatches to either `ExplodeCrate(self, 1)` or
  `BreakCrateInStack(self, 0, 0, 0)`.
- **`ClearCrateStackTouched`/`MarkCrateStackTouched`** (matched to this issue's
  understanding, parked under `NON_MATCHING`) are **bidirectional
  neighbor-list walkers** built on the same `GetCrateAbove`/
  `GetCrateBelow` "get next"/"get prev" pair `QueueCratePlayerCollision` itself uses,
  clearing (`ClearCrateStackTouched`) or setting (`MarkCrateStackTouched`, plus a
  budget-redistribution side effect on a caller-supplied `ctx`
  pointer's `+4`/`+0xc` fields) each visited neighbor's `+0x58` flag
  byte whenever its own `+0x4d & 0x7f` state byte reads 0.
- **`ApplyCrateCollision`** (1032 B) and the 18 functions from `BounceWumpaCrate`
  through `UpdateSlotCrate` are the rest of `QueueCratePlayerCollision`'s jump-table
  targets and their own further sub-dispatches - moderately-sized
  (80-750 B) state-machine functions, each reading/writing several of
  the same `self+0x4d`/`+0x4e`/`+0x50`/`+0x64`/`+0x74`/`gPlayer`
  fields `QueueCratePlayerCollision` and `BreakCrateTouchedByPlayer` already touch, calling
  `PlaySfx`, `_call_via_r4` (one of the `bx rN` BLX-emulation
  trampolines - see `docs/rom_map.md`'s trampoline-table correction),
  and each other. Left completely raw for this pass - see "Left raw"
  below.

## Parked (`NON_MATCHING`) - 3 functions

All three are fully understood (semantics, field offsets, every branch
and call confirmed against the ROM disassembly) but don't yet produce
byte-identical output from `tools/agbcc`.

- **`BreakCrateTouchedByPlayer`** (`src/crates/crate_hit.c`; real bytes in
  `asm/code_3_2_17_d040.s`) - this is the *same* AABB-build primitive
  `GetSpriteHitbox` (`src/objects/sprite.c`) is already parked for,
  just inlined twice in a row (once for `self`, once for the player)
  instead of called as a subroutine, plus an overlap-dispatch tail.
  `GetSpriteHitbox`'s own doc comment documents this shape resisting
  byte-exact register allocation even in isolation ("about 10 of ~73
  instructions... which anonymous scratch register" gaps); doing it
  twice compounds rather than cancels the problem. Concretely: the ROM
  keeps exactly two extra callee-saved registers live across both
  builds (`r8` and `sb`, the latter caching `&gPlayer` so the
  player pointer survives the `SetAabbPos`/`SetAabbSize` calls'
  clobber), reusing `r7`/`r8` for the X/Y "shift" values across *both*
  blocks. Every variant tried here (explicit `xShift`/`yShift` locals
  shared across both blocks; a `vu8` volatile cast on the second
  `self+0x28` bit-test in each block, mirroring the CSE-blocking trick
  `GetSpriteHitbox` needed; hoisting/flattening the player-box locals in
  and out of a nested scope) lands on *three* extra callee-saved
  registers (`r8`/`r9`/`sl`) instead of the ROM's two, and/or moves
  `self` itself out of `r6` into `r8`. Parked rather than keep chasing
  individual register letters.
- **`ClearCrateStackTouched`/`MarkCrateStackTouched`** (`src/crates/crate_break.c`; real
  bytes in `asm/code_3_2_17_e494.s`) - every operation, operand and
  branch matches the ROM one-for-one, including gcc naturally finding
  the same "reuse the just-computed `ands` result register as the
  literal 0/1 being stored" trick the ROM uses, and (for
  `MarkCrateStackTouched`) hoisting the literal `1` into a register kept live
  across each loop, exactly like the ROM's `r7`/`r6`. The one remaining
  gap, 4 occurrences across both functions: the ROM materializes the
  `0x7f` mask immediate *before* the `ldrb` byte load
  (`movs r2,#0x7f; ldrb r0,[r0]; ands r2,r0`), while this compiler
  always schedules the load first regardless of operand order in the
  source (`mask & *cur` vs `*cur & mask`, `!(...)` vs `(...) == 0` -
  all tried, same scheduling). Parked rather than chase a
  scheduler-internal tie-break.

## Left raw (untouched) - 20 functions

- **`QueueCratePlayerCollision`** (`asm/code_3_2_17_d18c.s`, ROM `0x0800D18C`,
  ~1960 B) and **`ApplyCrateCollision`** (same file, ROM `0x0800E08C`,
  1032 B) - both read at a high level via `docs/rom_map.md` (see
  above), but not understood branch-by-branch with the precision a
  byte-exact reconstruction needs. `QueueCratePlayerCollision` in particular calls
  27 other functions in this same still-raw neighborhood and was
  already flagged there as needing "a dedicated pass" of its own,
  larger in scope than this single chunk issue.
- **`BounceWumpaCrate`, `LightTntCrate`, `OpenCheckpointCrate`, `BreakCrateInStack`,
  `BreakCrate`, `OpenMysteryCrate`, `OpenSlotCrate`, `DropCratesAbove`,
  `ExplodeCrate`, `BlastNearbyCrates`, `UpdateCrates`, `DetonateNitroCrates`,
  `ActivateNitroSwitchCrate`, `ActivateIronSwitchCrate`, `SolidifyOutlineCrates`, `SolidifyOutlineCrate`,
  `BreakCratesInArea`, `FinishBrokenCrate`, `UpdateTntCountdown`, `UpdateSlotCrate`**
  (`asm/code_3_2_17_e560.s`, ROM `0x0800E560`-`0x0800FC70`) - the rest
  of `QueueCratePlayerCollision`'s jump-table targets and their own further
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
   `asm/code_3_2_2.s`'s existing `GetSpriteBounds`/`GetSpriteHitbox` for the
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
   two *non-contiguous* islands (`BreakCrateTouchedByPlayer` alone, then
   `ClearCrateStackTouched`/`MarkCrateStackTouched` after the untouched
   `QueueCratePlayerCollision`/`ApplyCrateCollision` block) needed their own `.c` files per
   `docs/workflow.md`'s "one `.c` file per contiguous ROM region" rule
   - each new `.c` file still needs `ldscript.txt` to place it (even
   though it compiles empty in the real build), so its guarded raw
   companion has to sit at a matching, separately-nameable position.
2. **The `GetSpriteBounds`/`GetSpriteHitbox` AABB-build shape is a known,
   confirmed-resistant register-allocation case**, now hit a second
   time (`BreakCrateTouchedByPlayer`, inlined twice). Anyone matching another
   function built on the same `+0x20`/`+0x2d`/28-or-0x1c-byte-stride
   hitbox-record convention should expect the same "which anonymous
   scratch register" gap rather than re-deriving it from scratch.
3. **A `0x7f`-mask-immediate-before-`ldrb`-load instruction order is
   not controllable from the C source** in this compiler, at least for
   the `(mask & byteLoad) == 0` shape hit 4 times across
   `ClearCrateStackTouched`/`MarkCrateStackTouched` - every operand-order/negation phrasing
   tried produced the same (load-first) schedule. Worth checking
   against other still-open parked functions with the same shape
   before spending more time on it.

## Second pass: `BreakCrateTouchedByPlayer`/`ClearCrateStackTouched`/`MarkCrateStackTouched` matched via NAKED transcription

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
`MakeLinkHandshakeId`/`ResetLinkSessionState` (`src/link/link_handshake.c`, see
`docs/matching/archive/issue-4-sio-settings-sync.md`'s "The general strategy
for the rest" section). The semantics-understanding paragraphs in each
function's doc comment were kept; the now-obsolete "here's exactly
what doesn't match" paragraphs were trimmed to a one-line pointer at
this gotcha instead.

Since both `asm/code_3_2_17_d040.s` and `asm/code_3_2_17_e494.s` held
nothing but their one/two guarded functions, both files are now empty
and were deleted, with their `ldscript.txt` lines dropped (their
`.c` files - `src/crates/crate_hit.c`/`src/crates/crate_break.c` -
were already correctly positioned in `ldscript.txt` from the original
parked pass, so no other `ldscript.txt` changes were needed for these
two).

## Phase 1: `QueueCratePlayerCollision`/`ApplyCrateCollision` closed - the Phase 2 dispatch map

Foundational pass for a second staged effort over this subsystem's
remaining 20-function tail (`0x0800D18C`-`0x0800FC70`,
`asm/code_3_2_17_d18c.s`/`asm/code_3_2_17_e560.s`), mirroring the
dispatcher-first/parallel-leaves approach already used to close the
`0x0800B8DC`-`0x0800D040` cluster (issue #9/#10). Both of the two
"left raw" dispatchers this issue's first pass flagged above
(`QueueCratePlayerCollision`, `ApplyCrateCollision`) are now **matched via NAKED
transcription**, confirmed by a full clean `make compare`
("La suma coincide"). Real bytes formerly in `asm/code_3_2_17_d18c.s`
(now deleted, fully consumed - both functions folded into the new
`src/crates/crate_break.c`, inserted in `ldscript.txt` between
`crate_hit.o` and `crate_break.o`, exactly where the raw file used to
sit).

**Size correction**: `QueueCratePlayerCollision` is **~3840 B**
(`0x0800D18C`-`0x0800E08C`), not the ~1960 B this issue's original
read-only pass (and `docs/rom_map.md`) estimated - it's nearly twice
the size, one of the largest single functions in the whole ROM.
`ApplyCrateCollision` (`0x0800E08C`-`0x0800E490`, 1032 B) was already sized
correctly.

**Why NAKED, not real C**: three nested jump tables, ~30 distinct
callees, and (confirmed again here) the same gcc-2.9-resistant
AABB-build register shape `BreakCrateTouchedByPlayer`'s own doc comment above
already documents (`r7`/`r8`/`sb` cross-block reuse) - twice more,
inside a function roughly four times the size of any other function
this project has attempted as real C. Transcribed
instruction-for-instruction from the ROM disassembly instead (mnemonics
translated `adds`->`add`, `movs`->`mov`, `ands`->`and`,
`lsls`/`lsrs`->`lsl`/`lsr`, `asrs`->`asr`, `orrs`->`orr`; original
`_08XXXXXX:` labels renumbered to GNU-as local numeric labels assigned
in first-definition order, each reference emitted with the correct
`f`/`b` suffix by comparing source position), the same escape hatch
already established for `BreakCrateTouchedByPlayer`/`ClearCrateStackTouched`/`MarkCrateStackTouched`
above. Verified in two stages: an isolated `cpp`/`agbcc`/`as` +
`objcopy`/`cmp` pass against `baserom.gba` first (bytes matched up to
the first `bl` - the expected relocation-site divergence for an
unlinked object; both jump tables' own internal `.4byte` pool words
also diverge in this isolated check purely because the `.text`
section isn't yet relocated to its real `0x0800D18C` VMA, another
expected false-positive at this stage, not a real bug), then the
authoritative full clean `make compare`, which passed outright.

### `QueueCratePlayerCollision`'s three jump tables

`QueueCratePlayerCollision(void *self, u32 arg1)` runs three separate jump-table
dispatches in sequence, not the single 6-case table the original
read-only pass described (that one is real, it's the *second* of the
three):

1. **Hitbox-source selector**, ROM `0x0800D2AC`, 7 cases (0-6), keyed
   by `(self's hitbox-record byte at +4) >> 4`, `bhi`-gated at 6:
   - Cases 0, 4: use `self`'s own just-built AABB (`r2+0x1c`).
   - Cases 1, 2, 3, 5, 6: fall back to a fixed box,
     `gEmptySpriteBox`.
   The selected box is tested for a zero width/height
   (`gEmptySpriteBox+4`/`+5` both 0 => treated as "no box", early
   return) then overlap-tested against the player's own hitbox record
   (`SetAabbPos`/`SetAabbSize` + `AabbOverlaps`). No overlap => early
   return before either of the other two tables is reached.
2. **Per-edge handler dispatch**, ROM `0x0800D454`, 6 cases (0-5),
   keyed by an accumulated edge-code value (`r6`, built from
   `self+0x28`/hitbox-record comparisons and `gCrateHitResponse`
   lookups above) - **this is the 6-case table the original read-only
   pass already described**, now confirmed byte-for-byte:
   - **Case 0** (`0800D46C`): reads a dispatch-id byte from
     `gCrateHitResponse` (cached in `sb`/`r8+0x4e`'s row); if it's
     `6`, calls **`ActivateNitroSwitchCrate(self)`**; if `3`, calls
     **`ActivateIronSwitchCrate(self)`**; otherwise no call.
   - **Case 1** (`0800D46C`): identical target to case 0 (shared code).
   - **Case 2** (`0800D48A`): no callee - just sets `self+0x4d` bit
     `0x80` and `gPlayer+0x80 = 1`.
   - **Case 3** (`0800D4A8`): the most complex case - calls
     **`BreakCrateInStack(self, 0, 0, 0)`**, then (unless
     `gPlayer+0x94 != 0`) rebuilds `self`'s hitbox pointer,
     and if the dispatch id (`sp+0x78`) isn't 3, calls
     **`PlayerHitboxOverlapsAt`** (already matched, `crate_hit.c`) to test a
     player-sized box at that spot; on overlap, walks to the "prev"
     neighbor (`GetCrateAbove`) and, if that neighbor's own `+0x4d&0x7f`
     isn't 1, looks up *its* dispatch id in `gCrateHitResponse` and
     calls **`BreakCrateInStack(neighbor, 0, 0, 0)`** again (id 3),
     `strb`-sets a flag and **`ExplodeCrate(neighbor, 1)`** (id 4), or
     nothing (other ids).
   - **Case 4** (`0800D684`): if `self+0x4d & 0x7f == 0`, calls
     **`ExplodeCrate(self, 1)`**.
   - **Case 5** (`0800D6A2`): calls **`OpenCheckpointCrate(self)`**.
   All six cases converge on a shared tail (`0800D6AC`) that adjusts
   the dispatch id (`sp+0x78`) for two special cases (id 5 + a
   `gPlayer+0x24==4` gate rewrites to id 2; id 3 rewrites to
   id 1), then - unless `self+0x58` was set and `self+0x44` is 0 (early
   return) - calls **`MarkCrateStackTouched(self, &localAABB)`** (already
   NAKED-matched, `crate_break.c`) when the id isn't 6, before falling
   into the third table.
3. **Post-processing dispatch**, ROM `0x0800DD70`, 9 cases (0-8), keyed
   by the same adjusted dispatch id (`sp+0x7c`), `bls`-gated at 8:
   - **Case 0, 3, 5, 6, 7**: all share the same target, `0800E00C` -
     effectively a no-op fast path straight to the function's shared
     tail (apply-offset + `AddCollisionCandidate` call).
   - **Case 1, 2** (`0800DEAC`): overlap-test `self`'s and a
     recomputed box (`AabbOverlapsInclusiveX`); on **no** overlap, calls
     **`ClearCrateStackTouched(self)`** (already NAKED-matched, `crate_break.c`)
     and clears the "apply offset" flag; on overlap, runs a large block
     of edge-distance comparisons that (depending on direction/gap)
     calls **`HasPlayerRampYTarget`** (external, unread) and/or
     **`FindLineCrossing`** (already matched, `crate_reset.c`, the
     Bresenham line-stepper) to decide a final offset direction.
   - **Case 4** (`0800DD94`): calls **`GetBottomCrate`** (already matched,
     `crate_stack.c`) to reselect `self`, re-reads its
     `gCrateHitResponse` row, and - if not filtered out - calls
     **`_call_via_r4`** (the `bx r4` trampoline; a sound/particle-effect
     function pointer loaded from `self+0x18+0x68`/`+4`) with a fixed
     arg pattern (`0, 0xc, 4`).
   - **Case 8** (`0800DE14`): calls **`GetTopCrate`** (already matched,
     `crate_stack.c`) to reselect `self`, similarly re-reads its
     `gCrateHitResponse` row, optionally resets
     `gPlayer`'s `+0x64`/`+0x54`/`+0x58`/`+0x5c` fields, and
     calls **`SetEntityPos`** (already matched, `graphics.c` - applies
     the computed position offset).
   All paths converge on the shared tail at `0800E00C`, which
   conditionally calls **`LightTntCrate`** (already-matched-elsewhere
   leaf; gated on `gPlayer+0x88==1`, `self+0x4e==0xe`, and a
   re-overlap test), then **`SetEntityPos`** (apply the final offset)
   and, if a sound/effect id was set, **`_call_via_r4`** again, then
   ends by calling **`AddCollisionCandidate`** (already matched) with ~8 packed
   arguments - the actual apply/commit step.

### `ApplyCrateCollision`'s jump table

`ApplyCrateCollision(void *self, u32 arg1, u32 arg2, u32 arg3, u8 arg4, u8 arg5, u8 arg6)`
runs a **single** 6-case jump table, ROM `0x0800E27C`, keyed by `r7`
(itself derived from `arg1`/`arg2` plus a `gCrateHitResponse`-driven
special-case rewrite for id 1, and a `gPlayer+0x92`-gated
rewrite to id 1 that also mutates `self+0x48`/state byte `+0x4e`):

- **Case 0, 1** (`0800E294`): identical to `QueueCratePlayerCollision`'s own case
  0/1 - reads `self+0x4e`; `6` => **`ActivateNitroSwitchCrate(self)`**, `3` =>
  **`ActivateIronSwitchCrate(self)`**, else nothing.
- **Case 2** (`0800E342`): reads `self+0x4e`; `0xe` =>
  **`LightTntCrate(self)`**, `0xc` => **`BounceWumpaCrate(self)`**, else no
  callee - just sets `self+0x4d` bit `0x80` and
  `gPlayer+0x80 = 1` (same as `QueueCratePlayerCollision`'s own case 2,
  `LightTntCrate`/`BounceWumpaCrate` are new here).
- **Case 3** (`0800E37C`): a multi-way gate on `arg1`/`self+0x44`/
  `arg2`/`arg3` that always ends in one **`BreakCrateInStack(self, 0, ...)`**
  call with a different 3rd/4th argument per branch (`0`, `4`,
  `byte[sp]`+`arg3`, or `0`+`arg3`), optionally also recording `self`
  into `gPlayer`'s ring buffer and bumping its `+0x94`
  counter.
- **Case 4** (`0800E41C`): if `self+0x4d & 0x7f == 0`, calls
  **`ExplodeCrate(self, 1)`**.
- **Case 5** (`0800E434`): calls **`OpenCheckpointCrate(self)`**.
- Shared tail (`0800E43C`): calls `SetEntityPos` (apply the accumulated
  offset, gated on `gPlayer+0x1084`'s `+4` byte) and, if a
  sound/effect id was set (`sp+0x38`), `_call_via_r4`.

This confirms `QueueCratePlayerCollision`'s and `ApplyCrateCollision`'s per-edge dispatch
tables really are the same shared handler family (identical case
ordering, identical targets for cases 0/1/4/5, case 3 always landing on
`BreakCrateInStack`) - `ApplyCrateCollision` is a second, narrower entry point into
the same six leaf handlers, called only from `QueueCratePlayerCollision`'s own
9-case table (cases 1/2/4, per that table's targets `0800DEAC`/
`0800DD94`/`0800DE14` above - none of those actually call
`ApplyCrateCollision` directly by name in the disassembly read here; note for
Phase 2 to double-check the exact call site if this matters for a leaf
function's own understanding).

### Phase 2 grouping hint

Of this issue's remaining 18-20-function tail
(`BounceWumpaCrate`-`UpdateSlotCrate`), exactly **7 are direct callees** of the
two now-matched dispatchers: `BounceWumpaCrate`, `LightTntCrate`,
`OpenCheckpointCrate`, `BreakCrateInStack`, `ExplodeCrate`, `ActivateNitroSwitchCrate`,
`ActivateIronSwitchCrate`. The other ~12 (`BreakCrate`, `OpenMysteryCrate`,
`OpenSlotCrate`, `DropCratesAbove`, `BlastNearbyCrates`, `UpdateCrates`,
`DetonateNitroCrates`, `SolidifyOutlineCrates`, `SolidifyOutlineCrate`, `BreakCratesInArea`,
`FinishBrokenCrate`, `UpdateTntCountdown`, `UpdateSlotCrate`) were not seen called from
either dispatcher directly in this pass - they're presumably called
transitively by one or more of the 7 direct entry points (each
individually still unread). A reasonable parallel-safe split: one
group per direct entry point (7 groups), each agent reading its entry
point first to discover which of the ~12 remaining leaves it pulls in,
rather than guessing the sub-call graph up front from this doc alone.

## Phase 2 (lower-address half): BounceWumpaCrate-DropCratesAbove closed

Closes 8 of the ~18-20 remaining leaf functions from Phase 1's grouping
hint: the lower-address group of direct dispatch targets
(`BounceWumpaCrate`, `LightTntCrate`, `OpenCheckpointCrate`, `BreakCrateInStack`) and their
own transitive callees, everything up to but not including
`ExplodeCrate` (a sibling parallel pass's own territory, covering
`ExplodeCrate`-`UpdateSlotCrate`). All 8 verified byte-exact by a full clean
`make compare` ("La suma coincide"). New file `src/crates/crate_break.c`,
inserted in `ldscript.txt` between `crate_break.o` and (the now-trimmed)
`code_3_2_17_e560.o`. `asm/code_3_2_17_e560.s` trimmed to begin at
`ExplodeCrate` (its own header directives kept, since the sibling pass
still needs the rest of the file).

**Transitive closure derivation**: reading each of the four direct
targets' own bodies (not just the dispatch map) found the actual
sub-call graph is narrower than "each dispatcher owns one of the 7
direct-callee groups" - `BounceWumpaCrate`/`LightTntCrate`/`OpenCheckpointCrate` call
nothing else in this still-raw neighborhood (only already-matched
siblings, `PlaySfx`, `rand`, or each other within the direct-target
set), while `BreakCrateInStack` calls `BreakCrate` (both times it needs a
"dispatch id" leaf handler, cases 3/22 of `BreakCrateInStack`'s own logic),
and `BreakCrate` in turn calls `DropCratesAbove`, `OpenMysteryCrate`, and
`OpenSlotCrate` (the latter two also reachable directly from
`BreakCrate`'s own 23-case jump table) - plus `ExplodeCrate`, left for
the sibling pass since it's out of this range. That's exactly 8
functions, no more, no fewer, in this half.

### What each function does

- **`BounceWumpaCrate`** - dispatch-id-5 handler (both dispatchers' case 5).
  Arms `self`'s `+0x48` frame-countdown to `0x168` the first time it's
  seen at its sentinel value, clearing a `+0x51` retry counter
  alongside it. While that countdown runs and `self+0x50` is zero,
  bumps `+0x51` each call; past 4 retries (or once `+0x48` itself
  expires), hands off to `BreakCrateInStack(self, 0, 0, 0)`. Otherwise arms
  `self+0x4d` bit `0x80`, `gPlayer+0x80 = 1`, a fresh
  `+0x4f = 6` sub-timer, and spawns a pair of `DropWumpa` particle
  effects (kind `0xe`) at `self`'s position, offset `-6`/`+3` pixels on
  Y/X.
- **`LightTntCrate`** - case-2 handler (dispatch id `0xe`, both
  dispatchers). Switches `self` into hitbox tag `0x14`, rebuilds its
  hitbox record (the `ResetSpriteFrameTimer`/`ResetSpriteFrameIndex`/`SetSpriteAnimDone` trio
  every hitbox-rebuild call in this subsystem uses), registers it with
  the object-pool grid (`LinkCrateToActiveBucket`), re-derives a low-nibble
  sub-animation value from the freshly selected hitbox record's `+0x14`
  byte via `GetPaletteSlot`'s tile-asset-cache lookup, plays SFX `0x11`,
  arms a `+0x4f = 0x3c` (60-frame) countdown.
- **`OpenCheckpointCrate`** - dispatch-id-5's own sibling case (both
  dispatchers' case 5, same table slot `BounceWumpaCrate` covers on the
  *other* dispatcher; the two are not actually the same handler despite
  sharing a case index - each dispatcher's 6-case table independently
  selects its own target per row). Spawns a particle-effect object
  (`SpawnEffectPart`, kind `0x2a`) at `self`'s position (minus 10 pixels on
  X), initializes its trajectory fields, switches `self` itself into
  hitbox tag `0x1b`, rebuilds its hitbox record, plays SFX `0x17`,
  notifies `SetEntityIdActivated` unless `self+8` is the sentinel `0xffff`,
  conditionally reactivates the viewport, tells `SetCheckpointAtPlayer` whether
  `self+0x50` is nonzero, and resets `self+0x4d` to `1`.
- **`BreakCrateInStack(self, edgeFlag, walkFlag, dir)`** - case-3 handler
  (both dispatchers). Counts `self` into `gPlayer+0x91`'s
  "objects handled this frame" tally (gated on `walkFlag`), then walks
  `self`'s neighbor chain (`dir==4` "get prev", `dir==8` "get next")
  past every node whose `+0x4d & 0x7f` state is already `1`, stopping
  at the first node that isn't (or the last reachable node if the
  whole chain is state `1`; neither `dir` value falls back to `self`
  itself). Unless `gCrateKindUnbreakable[target+0x4e]` is nonzero,
  dispatches to `BreakCrate(target, edgeFlag)`.
- **`BreakCrate(self, edgeFlag)`** - `BreakCrateInStack`'s shared tail.
  Early-outs if `self+0x4d & 0x7f == 1`. Otherwise registers `self`
  with the object-pool grid, resets `self+0x4d` to `0x81`, switches
  `self` into hitbox tag `0x1d` and rebuilds its record, re-derives its
  `+0x29` sub-animation value and clamps `+0x30`'s index to the newly
  selected record's own `+0x16` count, conditionally reactivates the
  viewport, flips one bit of `gEntityFlags`'s bit-grid keyed by
  `self+8`, calls `DropCratesAbove` (neighbor "impact spread"
  propagation), then dispatches its own 23-case jump table on `self`'s
  freshly-cached `+0x4e` state id to one of
  `OpenAkuAkuCrate`/`OpenLifeCrate`/`ActivateIronSwitchCrate`/`ActivateNitroSwitchCrate`/
  `OpenMysteryCrate`/`OpenSlotCrate`/`ExplodeCrate`/`FreezeLevelClock`/a
  SFX-3-plus-particle-spawn fallback (`DropWumpa`) - the largest
  jump table in this subsystem after `QueueCratePlayerCollision`'s own three.
- **`OpenMysteryCrate(self, walkFlag)`** - case-11 handler of
  `BreakCrate`'s table (dispatch id `0xb`). Plays SFX 3, then (the
  first time `self+0x51` is exactly `9`) rolls a random "escalation
  level" (`1`/`4`/`7`/`8`) into that byte. Dispatches its own 10-case
  jump table on `(self+0x51 - 1)`: cases 5 down through 0 deliberately
  cascade-fall-through into each other (an escalating "more debris"
  particle burst, `DropWumpa`, at slightly different offsets the
  further the level counted down); case 6 fires a screen-shake
  (`_call_via_r4`) plus SFX; case 7 spawns a `DropExtraLife` bonus object;
  case 9 spawns one final small puff.
- **`OpenSlotCrate(self, walkFlag)`** - case-15 handler of
  `BreakCrate`'s table (dispatch id `0xf`). Plays SFX 3, then
  switches on `self+0x48 & 7`: `1` plays SFX 3 again, notifies
  `SetEntityIdActivated`, and spawns a `DropExtraLife` bonus object 3 pixels below
  `self`; `2` forwards to `OpenMysteryCrate`; `3` clears `self+0x4d` bit
  `0x80` and calls `ExplodeCrate(self, 1)`; any other value does
  nothing further.
- **`DropCratesAbove(self, walkFlag)`** - neighbor "impact spread"
  propagation, called once from `BreakCrate`'s own body (not through
  its jump table). Derives a base spread budget from `self`'s hitbox
  record's own `+9` byte, then walks `self`'s "get next" neighbor chain
  redistributing that budget across each visited node's `+0x40`/`+0x44`
  "remaining spread" fields, nudging each node's `+0x4c` byte toward 0,
  re-registering it with the object-pool grid, and - for any node past
  a `gCrateKindExplosive`/`+0x48`/`+0x44` threshold gate - "graduating"
  it into state `+0x48 = 1`.

### Matching notes

- **`LightTntCrate`/`OpenSlotCrate` matched as real C** - both are fairly
  linear (no loops, `OpenSlotCrate` a small `switch` rather than a
  computed jump table), unlike this half's other 6 functions. Three
  distinct gcc-2.9 gaps needed register-pinned/inline-asm anchoring in
  `LightTntCrate` alone, all confirmed by direct byte comparison against
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
     lookup (the same convention `BreakCrateTouchedByPlayer`'s "AABB1" shape uses) -
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
     `DrawCrate` in crate_draw.c) rather than folding it into an 8-bit
     AND immediate. Anchored as one inline-asm block covering the whole
     sequence, taking the freshly-extracted `lo` value as an in-out
     operand.
- **The other 6 (`BounceWumpaCrate`/`OpenCheckpointCrate`/`BreakCrateInStack`/
  `BreakCrate`/`OpenMysteryCrate`/`DropCratesAbove`) closed as NAKED
  transcriptions**, the same escape hatch Phase 1's two dispatchers
  used - jump-table density (`BreakCrate`'s 23 cases, `OpenMysteryCrate`'s
  10, both with cascading-fallthrough or heavily-reused pool constants)
  and/or this subsystem's confirmed `r8`/`sb`/`sl`-triple-accumulator
  shape (`DropCratesAbove`, matching `QueueCratePlayerCollision`'s own AABB-build
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
   - `LightTntCrate`'s `self[0x29]` mask-then-address vs.
     address-then-mask ordering (see "Matching notes" above) - a
     misreading of which operation the ROM actually does first,
     caught only once the *first* full clean `make compare` attempt
     failed and the mismatch's exact byte offset was traced back with
     a fresh `objdump -D -b binary -m arm --adjust-vma=0x08000000
     -M force-thumb baserom.gba` disassembly of `baserom.gba` itself
     (not the project's own pre-split `asm/*.s`, and not the isolated
     `agbcc` output) at that precise address.
   - `OpenCheckpointCrate`'s constant-pool placement - all 8 of this function's
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

## Phase 2, higher-address half: `ExplodeCrate`-`UpdateSlotCrate` closed

One of two parallel Phase 2 passes over this cluster's remaining
18-20-function tail, split by address range (see Phase 1's "grouping
hint" above). This pass covers the **higher-address half**: the twelve
functions from `ExplodeCrate` up through the end of the whole cluster,
`UpdateSlotCrate` (`0x0800EEF0`-`0x0800FC70`) - `ExplodeCrate`, `BlastNearbyCrates`,
`UpdateCrates`, `DetonateNitroCrates`, `ActivateNitroSwitchCrate`, `ActivateIronSwitchCrate`,
`SolidifyOutlineCrates`, `SolidifyOutlineCrate`, `BreakCratesInArea`, `FinishBrokenCrate`,
`UpdateTntCountdown`, `UpdateSlotCrate`. A sibling pass covers the lower-address
half (`BounceWumpaCrate` through `DropCratesAbove`) separately.

All twelve are now **matched via NAKED transcription**, confirmed by a
full clean `make compare` ("La suma coincide"), the same escape hatch
Phase 1 used for `QueueCratePlayerCollision`/`ApplyCrateCollision`: every one of them
re-triggers either the `self+0x20`-table/`+0x2d`-tag/28-byte-stride
AABB-build register shape, or plain high-register (`r8`/`sb`/`sl`)
cross-block reuse under `-O2` this compiler's allocator doesn't
reproduce - and given this batch's size (12 functions, ~1730 lines of
disassembly), transcription was the reliable path to a byte-exact result
for all of them at once. New file `src/crates/crate_break.c`; real bytes
formerly the tail of `asm/code_3_2_17_e560.s` (from `ExplodeCrate`
onward - that file now ends right after `DropCratesAbove`, the sibling
pass's own territory).

**Mechanics note for future NAKED-transcription passes**: this pass's
transcription was done with a small Python script (mnemonic map +
per-function local-label renumbering, the same algorithm Phase 1
describes) rather than by hand, which is worth reusing for any future
batch this size. One real bug surfaced and fixed while writing it: a
`.4byte` literal-pool entry whose *value* is itself a same-function local
label (a jump-table base address referenced via `ldr rX, =tableLabel`
immediately followed by `tableLabel: @ jump table`, hit once in
`SolidifyOutlineCrate`) needs its value resolved to an `Nf`/`Nb` local reference
too, not just the pool entry's own defining label - naively emitting the
raw ROM label name there produces an undefined-symbol assembler error.
A second near-miss: a naive `_[0-9A-F]{8}` regex for "is this operand a
local label reference" also matches *inside* an unrelated global
symbol's name (e.g. `gPlayer` contains `_030012D8`, which
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

- **`ExplodeCrate(self, u8 arg1)`** - the per-edge dispatch's shared
  **case 4 target** (both `QueueCratePlayerCollision`'s and `ApplyCrateCollision`'s own case
  4: `self+0x4d & 0x7f == 0` gates a call with `arg1=1`). Also called
  directly by several siblings below (`arg1=0`) whenever their own
  overlap/state checks reach the same "commit an edge collision"
  outcome. Clears `self+0x4f`/`self+0x4d`'s low 7 bits, sets `self+0xc`
  bit `0x10`, re-adds `self` to `gCrateList`'s active list
  (`LinkCrateToActiveBucket`), sets `self+0x4d` bit `0x80` then ORs in `arg1` as the
  low bit. Tags `self+0x2d` (`0x21`, or a `self+0x4e-0x21`-offset
  rewind when already `0xa`) via the `ResetSpriteFrameTimer`/`ResetSpriteFrameIndex`/
  `SetSpriteAnimDone` "set tag, refresh sprite/animation" triplet every
  state-transition function in this cluster shares. Marks a cell in
  `gEntityFlags`'s 32x32 collision bitmap, plays a fixed sound
  (id 4), calls `DropCratesAbove(self)` (sibling pass's territory), and -
  gated on a combo/proximity check against `gRoomFrameCount`/
  `gPlayer+0x8c` - `_call_via_r4(self, 0, 4, 0)`. Forces
  `self+0x4e = 0x13` in the common case (see `UpdateTntCountdown` below).
- **`BlastNearbyCrates(self, u32 arg1)`** - called only by `FinishBrokenCrate`
  below (`arg1` = `0x14` or `0x28`, a proximity radius). Two
  `gCrateList` list-scan passes: settles every nearby object via
  `gCrateKindExplosive`/`gCrateKindBreakable`-driven dispatch to
  `BreakCrateInStack`/`ActivateIronSwitchCrate`/`ActivateNitroSwitchCrate`/`ExplodeCrate`, then a second
  pass over `gTouchableList` calling `PickUpWumpa`. Resets
  `self+0x48` to the `-1` sentinel at the end.
- **`UpdateCrates(void)`** - no arguments. Calls `DetonateNitroCrates` first
  (flush pending case-`0xa` commits), then an up-to-twice
  `gCrateList` list scan removing/re-classifying objects via
  `RemoveCrateListAt`/`_call_via_r2`/`_call_via_r1`, driven by a
  `gCrateListChanged` one-shot re-scan flag.
- **`DetonateNitroCrates(void)`** - no arguments. Settles every
  `gCrateList` object stuck at `+0x4e==0xa`/`+0x4d&0x7f==0` via
  `ExplodeCrate(other, 0)`. Called by both `UpdateCrates` and
  `ActivateNitroSwitchCrate` as a "flush leftovers from last frame" first step.
- **`ActivateNitroSwitchCrate(self)`** - per-edge dispatch id-row-`6` target (shared
  by `QueueCratePlayerCollision`'s/`ApplyCrateCollision`'s case 0/1). Tags `self+0x2d=0x23`,
  runs the tag/refresh triplet plus a `GetPaletteSlot`-driven `self+0x29`
  nibble update, flushes via `DetonateNitroCrates`, bumps a combo counter
  (`ShowHudCrates`), plays sound id 4, arms `self+0x48=1`.
- **`ActivateIronSwitchCrate(self)`** - per-edge dispatch id-row-`3` target (sibling
  of `ActivateNitroSwitchCrate`, same case). Tags `self+0x2d=0x22`, same triplet +
  nibble update, marks the collision bitmap (`MarkEntityIdActivated`), then scans
  `gCrateList` for up to 0x20 simultaneously-triggered
  same-`+0x50`-group neighbors, allocating (`OperatorNewArray`) a linked
  group list at `self+0x48` when any are found (`-1` sentinel
  otherwise). Seeds `self+0x4f` from `self+0x4c`.
- **`SolidifyOutlineCrates(self)`** - bumps `self+0x50` against a `self+0x51`
  cap; once reached, frees `self+0x48`'s group (`OperatorDeleteArray`) and
  resets to the `0x13`/`0x14`/`0x15` "settle" family via `self+0x4e=7`.
  While under the cap, recursively settles every other member of
  `self+0x48`'s triggered group via `SolidifyOutlineCrate`, playing one shared
  sound (id `0xf`) for the batch.
- **`SolidifyOutlineCrate(self)`** - the group-settle worker `SolidifyOutlineCrates`
  calls. Reinterprets `self+0x48` as a byte offset subtracted into
  `self+0x4e`, then a 0x13-entry jump table mapping each resulting
  sub-case to one of a small set of `self+0x2d` tag constants, each
  through the same tag/refresh triplet, converging on a
  `GetSpriteAnimPaletteSlot`-driven nibble update.
- **`BreakCratesInArea(s32 x, s32 y, s32 arg2, s32 arg3)`** - the one function
  here taking a raw probe box instead of `self` (existing extern in
  `sprite.c`: `BreakCratesInArea(part->x>>8, part->y>>8, 0x40, 0x12)`).
  Scans `gCrateList` for objects within `(arg2,arg3)` of the box,
  dispatching via `gCrateKindExplosive`/`gCrateKindBreakable` to
  `OpenCheckpointCrate`/`BreakCrateInStack`/`ExplodeCrate` - the same "settle nearby
  objects" shape as `BlastNearbyCrates`/`FinishBrokenCrate`, box-driven instead of
  `self`-driven.
- **`FinishBrokenCrate(self)`** - the per-edge dispatch's re-entry point once
  `self+0x4d&0x7f==1` (commit already underway; called from the
  still-raw `0x080104E4` continuation, outside this issue's scope).
  Dispatches to `BlastNearbyCrates` per `self+0x30`/`gCrateKindExplosive`,
  unlinks `self` from its neighbor list when `self+0x38` is set
  (`SetCrateBelow`/`SetCrateAbove`), and marks/clears
  `gCrateListChanged`/`gPlayer+0x94`'s ring-buffer re-visit
  bookkeeping.
- **`UpdateTntCountdown(self)`** - the `0x13`/`0x14`/`0x15` "settle" family's
  own small state cycle, guarded by `self+0x4f`'s cooldown throttle.
  `0x14`->`0x13` and `0x15`->`0x14` each retag/replay the triplet, play
  a sound (id `0x11`), and set a ~1s cooldown; `0x13` (once
  `self+0x4d&0x7f==0`) calls `ExplodeCrate(self, 0)`, closing the loop
  back to the top of this list.
- **`UpdateSlotCrate(self)`** - the cluster's last and largest function
  (~736 B), called for `self+0x4e==0xf`. A per-frame position-wrap/
  edge-scan advance structurally similar to the already-parked
  `UpdateCrateFall` that immediately follows this whole cluster (see
  `docs/matching/archive/issue-13-fc70-continuation.md`): clamps `self` within
  0x4f/0x3f px of the player into a packed `self+0x48` byte, then (only
  when `self+0x2d==8`) runs a 4-phase `self+0x48&7` state rotation
  calling `GetSlotCrateSpins` to decide whether each phase continues or
  commits. Self-contained (no calls out to any other function in this
  cluster) - a full branch-by-branch semantic write-up was not attempted
  for this pass, since the NAKED transcription's byte-exactness doesn't
  depend on it.

## NAKED retry: 18 of 24 promoted to real C under old_agbcc

A later pass went back over all 24 NAKED functions in this cluster.
This cluster was built with the **older compiler**
(`tools/agbcc/bin/old_agbcc`). The "`0x7f` mask immediate loaded before
the `ldrb`" gap documented above is that compiler's usual instruction
order, not a scheduling quirk. `crate_break.c` now sits on the Makefile's `OLD_AGBCC_OBJS`.
`LightTntCrate`, the one function already in C, matches under both
compilers. The object layout is named in `include/crate.h`
(`struct crate`, `crate_list`, `phys_player`, the
`PHYS_CALL`/`PhysSetTag`/`PHYS_SET_ID_BIT` helpers).

**Closed (18):**

- `ClearCrateStackTouched`/`MarkCrateStackTouched` (crate_break.c): old_agbcc as-is. The
  first function needs a goto-into-`do` loop so the loop enters at the
  call. The second needs a per-loop `u8 one = 1` local, which makes gcc
  hoist the constant into r7/r6.
- `DetonateNitroCrates`/`UpdateCrates`/`BreakCratesInArea`/`BlastNearbyCrates`/`ActivateIronSwitchCrate`:
  these are list scans. The vtable `+0x48` class query is written as
  `PHYS_CALL` (`_call_via_r1`). Two things were needed:
  - Byte tests written with `&&` got combined into one word compare
    (`ldr [o,#0x4c]` masked against `0x7fff00`). Nested `if`s split
    them.
  - Where the ROM hoists `gCrateKindExplosive` into a register, a
    `u32 commit = (u32)table` local indexed as `*(u8 *)(kind + commit)`
    reproduces it, including the `kind + table` operand order.
- `ActivateIronSwitchCrate`: `n++; n &= 0x1f;` (not `n = (n + 1) & 0x1f`) keeps
  the ROM's signed `ble` loop pre-test. The copy loop is
  `((struct crate **)g)[i + 1] = found[i]`.
- `ActivateNitroSwitchCrate`/`ExplodeCrate`/`OpenCheckpointCrate`/`SolidifyOutlineCrates`: a small `u8`
  local holding a constant (`one`, `kind = 7`) gives the ROM's order
  and its reuse of that register. Tag changes go through the
  `PhysSetTag` inline, whose parameter puts the constant first.
- `ExplodeCrate`/`FinishBrokenCrate`: the bitmap setter is the
  do/while(0) `PHYS_SET_ID_BIT`, the same as swim_ctrl.c. The
  "gone" flag is a `u8 gone:1` bitfield view.
- `SolidifyOutlineCrate`/`UpdateTntCountdown`: plain switches over `PhysSetTag`.
  `GetSpriteAnimPaletteSlot` returns `s32`.
- `BreakCrateInStack`: the neighbor walks are written as the ROM's goto loops.
  One variable (`q`) holds both the walk candidate and the final
  target.
- `OpenSlotCrate`: an empty `case 0:` gives the ROM's compare order.
- `BounceWumpaCrate`/`OpenMysteryCrate`/`OpenSlotCrate`: these call
  `DropWumpa`/`DropExtraLife`. Both take a stack word plus a stack
  *byte* that the ROM stores with `strb`, and this compiler widens a
  stack byte to `str`. The workaround:
  - The caller declares two locals first (`s32 argP4; u32 argP5;`, at
    sp+0/sp+4) and calls through a 4-argument view (`SPAWN_CALL`).
  - The stores sit inside the last argument, and x/y are bound first
    (the `PHYS_SPAWN` macro), so the order matches the ROM.
  - Where the byte is a constant, the ROM computes the slot address
    before the constant. That needs `register ... asm("r4"/"r5")`
    pins, never r7.
  - In `OpenMysteryCrate`, the inlined `OpenAkuAkuCrate`/`OpenLifeCrate` SFX calls
    go through a `PhysSfx(id)` inline so the id loads before the volume.

**Not closed (6):**

- `BreakCrateTouchedByPlayer` (crate_hit.c): old_agbcc gets the push list, but gcc
  keeps the player box's address `sp+16` in a callee-saved register,
  where the ROM recomputes `add r0, sp, #16` at each use. Separate
  structs, an array, and a static inline accessor all gave the same
  result.
- `BreakCrate`: every block is right, but `self`/`chained` land in
  r5/r8 instead of r4/r7 (~120 halfwords).
- `DropCratesAbove`: register allocation, with `self` in r8 and the delta
  byte spilled to `[sp]` in the ROM.
- `UpdateSlotCrate`: the `+0x48` phase/count/direction byte is updated with
  word loads and stores and 8-bit masks. Neither u8/u32 bitfield views
  nor explicit masks reproduce the ROM's unfolded `(n & 7) & 4` test
  and its register set (~280 halfwords).
- `QueueCratePlayerCollision` (~3840 B) and `ApplyCrateCollision` (1032 B): too large for
  this pass. These are the next candidates for a C draft under
  old_agbcc; the helpers in `include/crate.h` cover most of their
  callee patterns.

## Cross-references

- `docs/status/game_loop.md` - matched/parked/raw lists updated,
  including the `graphics` -> `game_loop` recategorization for this
  address span, Phase 1's `QueueCratePlayerCollision`/`ApplyCrateCollision` closure, and
  Phase 2's lower-address-half (`BounceWumpaCrate`-`DropCratesAbove`) and
  higher-address-half (`ExplodeCrate`-`UpdateSlotCrate`) closures.
- `tools/report_units.py` - `UNITS` list split/recategorized for
  `0x0800D040`-`0x0800FC70`, Phase 1's entry updated to point at the
  new `src/crates/crate_break.o`, and Phase 2's `0x0800E560` entry
  split in two at `0x0800EEF0` to point the lower half at the new
  `src/crates/crate_break.o` and the higher half at the new
  `src/crates/crate_break.o`.
- `docs/rom_map.md` - "Confirmed: a shared physics/collision
  subsystem, entered from multiple different entity types" and the
  preceding "Cross-checked the `UpdateGameFrame`-`MainLoop` cluster"
  section are the read-only reconnaissance this issue's matching work
  is based on.

## Later pass (issue #12/#24/#26 NAKED retry)

`DropCratesAbove` is real C under old_agbcc. The "triple accumulator" shape
was not the blocker; `spread = n->fallTargetY; spread -= n->y;` (two steps)
puts `self`/`spread`/`carry`/`base` in r8/r7/r9/r10 like the ROM. See
[issue-24-26-12-naked-retry.md](issue-24-26-12-naked-retry.md) for the
remaining source-shape details.

### Later pass: issue #12/#13/#25 NAKED retry

`BreakCrate` is real C under old_agbcc. The "r8/sb accumulators" were
ordinary: `sb` is the CSE'd `&self->kind` and `r8` a local `one = 1`
that the state store and the bitmap shift share. What mattered was
switching the tag through the `PhysSetTag` inline, clamping the frame
through the new `PhysSetFrame(self, 3)` inline, setting `flags` bit 4
as a bitfield, and writing the switch cases in the ROM's block order.
`BreakCrateTouchedByPlayer`, `ApplyCrateCollision` and `UpdateSlotCrate` now have near-miss drafts
and `QueueCratePlayerCollision` is still untouched. See
[issue-12-13-25-naked-retry.md](issue-12-13-25-naked-retry.md).

## Later pass: third near-miss sweep

`UpdateSlotCrate` is real C under old_agbcc, the last of the twelve in
`crate_break.c`. The count update is written as separate in-place
steps on a fresh local (`t = (r - 1) << 24; cw &= 0xc7; t >>= 21;
cw |= t`), which ties each result to the ROM's register. See
[near-miss-polish-3.md](near-miss-polish-3.md).

## Later pass: first C draft of `QueueCratePlayerCollision`

`QueueCratePlayerCollision` now has a C draft under `#if NON_MATCHING`. It is the
ROM's exact size under old_agbcc and 968 halfwords off; the function
stays NAKED. The draft fixes the function's structure: the frame layout,
the three jump tables, the `goto` into a shared `edge = dirX` block,
and the case bodies in ROM order. What is left is low-register
allocation. See [huge-naked-retry.md](huge-naked-retry.md).

## Later pass (stack-box NAKED retry)

`BreakCrateTouchedByPlayer` is now real C under old_agbcc (`crate_hit.o` joined
`OLD_AGBCC_OBJS`). Every use of the player box's address goes through
an empty `asm("" : "+r")` copy, so cse doesn't hold `sp+16` in a
callee-saved register across the builder calls, and `px`/`py` are
shared by both blocks, as in the ROM's r7/r8. `QueueCratePlayerCollision` (938
halfwords off, was 968) and `ApplyCrateCollision` (49) stay NAKED. See
[sp-box-retry.md](sp-box-retry.md).

## Later pass: ApplyCrateCollision matched (last-five NAKED retry)

`ApplyCrateCollision` is now real C under old_agbcc (`crate_break.o` joined
`OLD_AGBCC_OBJS`). The first flag byte is a register union of a u32 and
a one-byte struct, passed to `BreakCrateInStack` as that struct (QImode), which
gives the ROM's `mov r5, sp; ldrb` reload. `QueueCratePlayerCollision` stays NAKED;
see [last5-naked-retry.md](last5-naked-retry.md) for what the pass found.

## Later pass: QueueCratePlayerCollision matched (huge NAKED retry 3)

`QueueCratePlayerCollision` is now real C under old_agbcc, which closes the last
function in this issue's range. The second pass had left it size-exact
and 33 halfwords off; this pass fixed the swapped spill slots, the table
load order at the first code lookup, the reload round-robin in the second
slope check, and four branch targets. See
[huge-naked-retry-3.md](huge-naked-retry-3.md).
