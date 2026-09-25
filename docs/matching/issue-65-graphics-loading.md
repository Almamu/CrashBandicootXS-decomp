# Issue #65: 0x080354E0-0x08037110 - `LoadLevelGraphics`/`LoadBg2Background`/`LoadObjSpriteTiles`

GitHub issue #65 (`decomp-chunk`, category `graphics_loading`) listed 22
raw functions in `asm/code_3_2_20_28568_c99c_31784_33ef4.s`, sitting
right at the boundary between the 40.4 KB actor-per-type-behavior zone
(`docs/rom_map.md`'s `0x0802B348`-`0x080354E0` entry) and the GAX2 audio
range (which starts at `0x08037110`, confirmed both by `docs/rom_map.md`
and by this chunk's own object-file neighbor in `ldscript.txt`:
`src/audio/counter_selector.o` picks up immediately after this chunk's
last function). This is the write-up for the work done against that
list.

## What this pass covered

`docs/rom_map.md`'s table already named the first three functions with
high confidence (`0x080354E0`-`0x08035780`ish: `LoadLevelGraphics`,
`LoadBg2Background`, `LoadObjSpriteTiles`) - level-start asset loading,
not actor code, despite sitting immediately after 40 KB of actor
behavior. This pass worked those three:

- **`LoadLevelGraphics`**: the per-level setup entry point (`UpdateGameFrame`,
  `asm/code_3_2_17_225a0.s`, calls it with a freshly-allocated 0x220-byte
  scratch object). Stashes `gUnknown_030012DC` (the `struct icon_manager *`
  already established by GitHub issue #46's chunk, `include/icon_manager.h`)
  into the scratch object's `+0xc` field; resets the OAM shadow buffer
  (`gUnknown_03001300`, `struct oam_shadow_buffer *`) via
  `sub_8006A90`/`sub_8006A48`/`sub_80006A8`/`sub_8006AAC`; clears
  `REG_BLDCNT`/`REG_BLDALPHA` (one 32-bit store), sets `REG_BLDY` to
  `0x10`, and clears `REG_DISPCNT`; calls `sub_8028A30` (the icon-manager
  accessor from issue #46, `src/graphics/hud_icon_widget4.c`) with `0xe`;
  sets the icon manager's `field_108` to `0x200` and fires its
  `record->slots[6]` trampoline via `sub_803AD7C` (the same
  `(u8 *)obj + slot->offset, slot->ptr` pattern already established
  throughout `src/graphics/actor_part*.c`); DMA3-copies three 0x20-byte
  palette banks (`gStaticData_0817D034`/`_054`/`_074`) into palette RAM
  at `0x050003A0`/`_C0`/`_E0`; calls `LoadBg2Background`/
  `LoadObjSpriteTiles`; allocates and constructs a 0x14-byte object via
  `sub_8034374(sub_8026EDC(0x14))` (the exact same allocate-then-construct
  pairing already confirmed in `src/audio/counter_selector_setup.c`'s
  `self->field_10 = sub_8034374(sub_8026EDC(0x14))`) into the scratch
  object's `+0x208` field; runs a fixed fade/audio-reset sequence
  (`sub_8001604`/`sub_80015E0`/`sub_8001524(1)`/`sub_8001614` - the same
  quartet already matched in `src/graphics/fade_screen_mode2.c`); zeroes
  the scratch object's first two words; and starts song `0xb` via
  `sub_80017BC(gUnknown_030012BC, 0xb)` (`gUnknown_030012BC` is the
  `struct AudioContext *` from `include/audio.h`/`src/audio/music_player.c`).
  Returns the same scratch object pointer it was given.

  The 0x220-byte scratch object stays a raw `u32 *` rather than a named
  struct here - most of its fields are read/written only by this chunk's
  still-raw neighbors (`sub_8035780`, `sub_8035E14`, `sub_8036154`, all
  called on the same object from `UpdateGameFrame` right around
  `LoadLevelGraphics`), so naming it properly belongs with whichever pass
  works through those, not this one (see `matching_decomp_prefer_structs`:
  a fully-opaque scratch buffer with only a few fields understood stays
  raw-offset with a comment, same precedent as
  `graphics_package_1e640.c`'s scratch buffer).

  Two ordering-sensitive compiler quirks came up matching this one, both
  now confirmed via the full clean `make compare`, not just an isolated
  compile:
  - `iconManager->field_108 = 0x200;` has to be split into a named
    `fieldValue = 0x200;` local assigned *before* `iconManager` is
    re-read from `self[3]` (the ROM re-loads `self[3]` fresh here rather
    than reusing the register from the earlier `sub_8028A30` call) -
    otherwise the compiler computes the field's address first and
    derives `0x200` from the address offset via a cheap `ADD` instead of
    materializing it independently, producing a different (if
    value-equivalent) instruction sequence than the ROM's.
  - `self[0x82] = (u32)sub_8034374(sub_8026EDC(0x14));` has to be split
    into a named `u32 *dest = &self[0x82];` computed *before* the two
    calls, then `*dest = ...;` after - the ROM computes this destination
    address first and holds it live across both calls (`r4`), while the
    natural "assign to `self[0x82]` " phrasing computes the address only
    at the store, after the calls. This was the one real correctness
    catch from a full rebuild here: the isolated per-function compile
    already looked identical byte-for-byte in this section, but a
    genuinely different instruction was hiding one call layer up, code
    at a different address than shown by the isolated `.s` diff - a
    concrete instance of the documented "isolated compile is a
    diagnostic tool, never proof" rule (`docs/workflow.md` step 3).

## Matched (1 function, full clean `make compare` passing - "La suma coincide")

- **`LoadLevelGraphics`** (`src/graphics/level_graphics.c`)

## Parked (`NON_MATCHING`, not yet byte-exact)

- **`LoadBg2Background`** (`src/graphics/level_graphics.c`, real bytes in
  `asm/code_3_2_20_28568_c99c_31784_33ef4_355e0.s`) - loads BG2's
  palette/tileset/tilemap from the 5-field `gStaticData_0817D0E4`
  package (`struct bg_package`: `width`/`height`/`paletteAsset`/
  `tileAsset`/`mapAsset` - the same field layout `LoadObjSpriteTiles`'s
  `gUnknown_030008BC` array uses, confirmed by identical offsets),
  remapping the tilemap's per-tile palette-select nibble into VRAM at
  `0x0600F000`, then sets `REG_BG2CNT`. Every operation and, after
  register-pinning `mapBuf`/`dest`/`count`/`mask` to `r8`/`r6`/`ip`/`r4`
  respectively (`matching_decomp_register_pinning`), every register in
  the function body now matches the ROM exactly, confirmed
  instruction-for-instruction via isolated compile. What resists
  matching is purely the prologue/epilogue: the ROM pushes/pops **one
  extra callee-saved register** (`r7`, via the `mov r7, r8`/`push {r7}`
  entry shuttle and its matching exit) that the function body never
  actually reads or writes once `mapBuf` is pinned to `r8` - every
  phrasing tried either reproduces this exact dead `r7` slot (by leaving
  `mapBuf` unpinned, which lets the allocator naturally shuttle through
  `r7` again) at the cost of `dest`/`count` landing in the wrong
  registers instead, or gets `dest`/`count` in their correct ROM
  registers (by pinning `mapBuf`) at the cost of the shuttle register
  collapsing to `r6` and `r7` dropping out of the push list entirely.
  This is the same category of gcc-2.9-allocator artifact already
  documented for `sub_801E644`
  (`docs/matching/issue-30-graphics-loading.md`) - a live range the
  register allocator's first (pressure-counting) pass reserves that ends
  up unused by the time its second (assignment) pass actually runs.
  The `REG_BG2CNT` value itself is also built via a genuinely
  uninitialized local ANDed with `-0x10000` before the four `orrs` that
  fill in every bit the store actually reads - the ROM's own compiled
  behavior, not a permuter shortcut (see the negative-constant
  bit-clear idiom in `docs/matching.md`), reproduced faithfully rather
  than "cleaned up" into an initialized local.
- **`LoadObjSpriteTiles`** (`src/graphics/level_graphics.c`, real bytes
  in the same new asm file) - uploads the 4 `struct bg_package` entries
  in `gUnknown_030008BC` into OBJ VRAM (`0x06010000` on) and OBJ palette
  RAM (`0x05000200` on, one 16-color bank per package), remapping each
  package's tilemap into a straight tile copy the same way
  `LoadBg2Background` remaps BG2's. The overall shape (4-pass loop,
  palette DMA then tile-buffer DMA then per-tile remap DMA) is confirmed
  against the ROM and this reconstruction is semantically faithful, but
  it hasn't had the same per-register tuning pass `LoadBg2Background`
  got - isolated compiles put several locals (the `struct bg_package **`
  array-walk pointer, the per-pass palette/tile-VRAM cursors) in
  different registers than the ROM's own `sl`/`sb`/`r8` allocation.
  Left for a follow-up pass rather than force a low-confidence match.

## Left raw (19 functions)

`sub_8035780` through `sub_8036FBC` (the remainder of the chunk, real
bytes unconditionally following the two parked functions' guarded block
in the same new `asm/code_3_2_20_28568_c99c_31784_33ef4_355e0.s`) were
not examined this pass - the first of them (`sub_8035780`) appears to
operate on the same 0x220-byte scratch object `LoadLevelGraphics`
returns (stride-0x34 array of records with position/velocity-shaped
fields), suggesting the rest of the chunk is level-state/animation
bookkeeping tied to that object rather than more asset loading, but this
wasn't confirmed function-by-function. Left for a follow-up pass.

## File structure

`asm/code_3_2_20_28568_c99c_31784_33ef4.s` truncated to end right before
`LoadLevelGraphics` (still holds the actor-region functions before this
chunk's range, out of scope here). New `src/graphics/level_graphics.c`
holds `LoadLevelGraphics` (matched, unconditional) plus
`LoadBg2Background`/`LoadObjSpriteTiles` (guarded `#if NON_MATCHING`).
New `asm/code_3_2_20_28568_c99c_31784_33ef4_355e0.s` holds those same two
functions' real bytes (guarded `.if NON_MATCHING == 0`) followed
unconditionally by the chunk's remaining 19 raw functions. `ldscript.txt`
and `tools/report_units.py`'s `UNITS` list both updated to match. Verified
via a full clean `make compare` (`La suma coincide`) and
`make NON_MATCHING=1 report`.

See [docs/status/graphics_loading.md](../status/graphics_loading.md) for
the running matched/parked list.

## Second pass: `LoadBg2Background` matched via NAKED transcription

Picked up `LoadBg2Background`, the one function of this pair flagged
above as "every operation and every register in the body already
matches the ROM exactly". Re-confirmed that with a fresh isolated
compile of the exact `#if NON_MATCHING` body (register-pinning `mapBuf`
to `r8`, `count` to `ip`, `mask` to `r4`, per
`matching_decomp_register_pinning`): every single instruction in the
function body, including the `width`/`height` load order and the remap
loop's register choices, came out byte-identical to the ROM. The one gap
was exactly as documented - the ROM's prologue/epilogue push/pop one
extra dead callee-saved register (`r7`, via a `mov r7, r8`/`push {r7}`
shuttle at entry that's never read back except by its own matching pop)
that the compiler's own prologue-generation pass drops whenever `mapBuf`
is pinned to `r8` (the shuttle register collapses to `r6` and doubles as
`dest` instead, `r7` never entering the push/pop list).

Tried the same "force a live register variable" technique already ruled
out for `sub_801E688`'s identical-shaped gap, adapted for this function's
different flavor (a genuinely *unused* r7, not a used-but-dropped one, as
the task description flagged as worth re-checking): an explicit
`register u32 r7dummy asm("r7")` local, kept live across the whole
function body via an empty `asm volatile("" : "+r"(r7dummy))` barrier
right after its declaration. Compiled cleanly, but made no difference -
`r7` still never entered either the push or pop list (confirmed by
grepping the isolated `.s` output for `push`/`pop`: still
`push {r4, r5, r6, lr}` / `pop {r4, r5, r6}`, no `r7`). This reconfirms
the conclusion already reached for `sub_801E688`
(issue-30-graphics-loading.md's "Seventh pass") and `sub_80240E4`
(`src/system/game_loop8.c`): this compiler's callee-save prologue list is
built from a first (pressure-counting) allocator pass that can reserve a
register slot never used by the time the second (assignment) pass
actually runs, and no inline-asm-based hint - bare clobber or dummy
register-variable operand - routes around it once the slot's use has
already been optimized away entirely.

Converted to `NAKED` and transcribed instruction-for-instruction from the
ROM disassembly instead (the exact same instruction sequence the
plain-C body already compiled to, just with a hand-written
`push`/`mov`-shuttle/`push` prologue and matching `pop`/`mov`-shuttle/
`pop`/`bx` epilogue in place of the compiler-synthesized one) - confirmed
byte-identical via a direct `arm-none-eabi-objcopy --only-section=.text`
+ byte comparison against `baserom.gba` at `0x080355E0` before
integrating (every byte matched except the four `bl` call-site offsets
and the `gStaticData_0817D0E4` literal-pool word, both inherent
relocation artifacts of comparing an unlinked, standalone isolated
object rather than a real correctness gap). Cut `LoadBg2Background`'s
guarded block out of
`asm/code_3_2_20_28568_c99c_31784_33ef4_355e0.s` (it sat at the very
start of the file, so - like the precedent cuts for `sub_801E644`/
`sub_801E688` - no mid-file split was needed, just dropping the leading
block and re-opening the `.if NON_MATCHING == 0` guard right before
`LoadObjSpriteTiles`, which is now the file's only guarded function).

`tools/report_units.py`'s single combined entry for this pair's address
range was split into three: `(0x080354E0, "src/graphics/level_graphics.o", ...)`
for `LoadLevelGraphics` (unchanged), `(0x080355E0, None, ...)` for
`LoadBg2Background` (matching the `sub_801E644`/`sub_801E688` precedent -
a NAKED transcription doesn't count as "matched" for this project's
per-file tracking, even though it's byte-correct), and
`(0x08035684, "src/graphics/level_graphics.o", ...)` for
`LoadObjSpriteTiles` (unchanged treatment, still pointing at the `.c`
file's `#if NON_MATCHING` reconstruction for its NON_MATCHING=1 diffable
percentage).

Verified via a full clean `rm -rf build && make NON_MATCHING=1 report`
(clean compile, no warnings for the file) + `objdiff-cli report generate`
(`LoadBg2Background` correctly excluded from the diffable-percentage
report as a `raw_080355E0` unit with no target, same as `sub_801E644`/
`sub_801E688`; `LoadLevelGraphics` still 100%, `LoadObjSpriteTiles`
still its pre-existing fuzzy percentage) and a full clean `rm -rf build
crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make
compare` (`La suma coincide`).

## Third pass: `LoadObjSpriteTiles` matched via register-pinning + opaque asm islands

Picked up `LoadObjSpriteTiles`, the last function this chunk's earlier
passes left parked. Its semantics were already confirmed (4-pass loop:
palette DMA, tile-buffer DMA, per-tile remap DMA); this pass was purely
about reproducing the ROM's exact register allocation, worked out by
mapping every ROM register's role block-by-block against the raw
disassembly (still in `asm/code_3_2_20_28568_c99c_31784_33ef4_355e0.s`
at the time, guarded `.if NON_MATCHING == 0`) and iterating with an
isolated `cpp`+`agbcc`+`arm-none-eabi-as` compile/assemble/byte-diff
loop against `baserom.gba` directly (not just the disassembly text).

Register roles, once mapped: the `struct bg_package **` array-walk
pointer lives in `r7` for most of each pass, gets saved to `r8`
(`pkgPtrStash` here) right before the per-tile remap loop reuses `r7`
as a scratch temp, and gets restored from `r8` right before the
loop-condition check at the bottom - a save/reuse/restore shuttle
around one physical register serving two unrelated roles at different
points in the same iteration. `sb`(`r9`) holds the pass counter,
incremented mid-body (right after the map-asset load, before the
remap loop) rather than at a conventional for-loop's tail. `sl`(r10)
holds the tile-VRAM cursor throughout. `r4`/`r5`/`r6` hold
`paletteBuf`/`mapBuf`/`tileBuf` respectively (with `r4` reused for
`count` and, inside the remap loop, as pure scratch for the loaded
map-entry halfword and the `0x20` tile-cursor increment).

Three techniques closed the gaps a plain, unpinned reconstruction left:

- **Register-pinned locals** (`register TYPE name asm("rN")`) for
  `pkgPtr`(`r7`)/`pass`(`r9`)/`pkgPtrStash`(`r8`) and, inside the
  per-tile remap block, `dma2`(`r3`)/`src`(`r2`)/`i`(`r1`) - per
  `matching_decomp_register_pinning`. One correctness trap surfaced
  early: pinning `pkgPtr` to `r7` while an *unrelated*, unpinned
  temporary (the DMA-register base address, materialized fresh via
  `(struct dma_regs *)REG_ADDR_DMA3SAD`) was free to land in any
  register let the compiler's allocator opportunistically reuse `r7`
  for that temporary *before* `pkgPtr`'s last real use in the same
  block - a genuine miscompile (silently reading through a clobbered
  pointer), not just a missed optimization, caught by re-deriving the
  isolated `.s` output's actual register flow rather than trusting
  that "pinned" means "reserved everywhere." Giving that DMA-address
  temporary (and the `0x80000010` DMA_CNT constant, which hit the same
  issue) its own explicit local with a register pin to a genuinely
  free, caller-saved register (`r0`) - forcing fresh materialization
  at each of its two use sites instead of one shared, hoisted,
  CSE'd value living across calls - fixed it.
- **Declaration order controlling otherwise-untied locals' register
  order**: the remap loop's five setup-preheader values (`dma2`,
  `mask`, the hoisted `dmaCnt2` constant, `src`, `i`) needed to
  materialize in exactly the ROM's own order for an exact byte match,
  and this compiler's own hoisting/allocation order followed the
  *textual* declaration order of the C locals, not any semantic
  grouping - reordering the declarations (rather than re-pinning
  registers, which reliably triggered ripple-effect regressions
  elsewhere in the function, confirming this project's documented
  "always re-diff the full function after each change" caution) closed
  this cleanly once the right order was found by trial.
- **Opaque `asm volatile` islands** (`matching_decomp_register_pinning`'s
  "continuous asm island" pattern, `AllocVramTileBlock`'s precedent)
  for three spots no plain-C phrasing reproduced no matter how it was
  written:
  - the `(*pkgPtr)->mapAsset` load, where the ROM emits a single
    `ldm r7!, {r0}` (load-and-post-increment in one instruction) that
    agbcc's `*ptr++` idiom recognition never triggers once the loaded
    pointer is immediately dereferenced again in the same expression -
    materialized directly via `asm("ldm %1!, {%0}" : "=r"(pkg),
    "+r"(pkgPtr))`.
  - the per-tile mask/shift/tileBuf-add/store sequence, where plain C
    (`(mask & *(u16 *)src) << 5`, tried in both operand orders)
    canonicalizes the load-then-AND into the opposite register roles
    than the ROM's `mov r0,ip`-first ordering every time.
  - the `dma->cnt` readback immediately followed by the tile-VRAM
    cursor's `+= 0x20`, where plain C reuses the readback's
    just-freed register for the `0x20` constant instead of the ROM's
    separate `r4`.

  Each island's operands (`dma2`, `src`, `mask`, `tileBuf`, `tileDest`)
  were passed as real GCC asm operands (`"r"(...)`/`"+r"(...)`) rather
  than referencing physical registers by bare name in the asm text -
  the one time a physical register (`r9`, for the final
  `while (pass <= 3)` comparison's scratch copy) was referenced by
  bare name without declaring `pass` as a real input, the compiler
  concluded nothing actually read `pass` and dead-code-eliminated the
  `pass++` increment entirely, freeing `r9` for `mask` to clobber - a
  genuine correctness bug caught by re-running the isolated
  byte-diff after the change, not by inspection. Declaring `pass` as
  a proper `"r"(pass)` input operand fixed it.

**A costly process mistake worth recording**: the isolated per-function
byte-diff against `baserom.gba` was (correctly) treated as authoritative
for the function's *own* bytes, but the offset used for that diff was
computed from a stale `@ 0x08035684` label comment left over in the
raw `asm/*.s` file from before `LoadBg2Background` was converted to a
`NAKED` transcription earlier the same day. Converting `LoadBg2Background`
to hand-written `asm(...)` text without an explicit `.pool` directive
left its 5-word literal pool (`gStaticData_0817D0E4`/`0x06008000`/
`0x0600F000`/`0xFFFF0000`/`0x0400000C`) un-pooled at the end of the
`asm()` block, so the assembler deferred emitting those 20 bytes to
later in the translation unit instead of immediately after the
function body - which the *isolated, single-function* compile of
`LoadObjSpriteTiles` alone could never reveal, since it doesn't include
`LoadBg2Background` at all. Only the full clean `make compare` (step 6,
run after integrating) caught the resulting 20-byte address shift,
manifesting as a total checksum mismatch traced via the exact method
`docs/workflow.md` prescribes: reading `crashbandicootxs.map` for the
actual linked address of the functions on either side of the change
(`LoadBg2Background` at `0x080355E0`, `LoadObjSpriteTiles` linking 20
bytes earlier than the stale comment implied, at `0x08035670` instead
of `0x08035684`), rather than guessing. Fixed by adding an explicit
`.pool` directive at the end of `LoadBg2Background`'s `asm(...)` text,
forcing its literal pool to emit immediately - a one-line fix, but a
concrete reminder that `LoadBg2Background`'s own doc-comment/status
label addresses are downstream of this fix too, not just
`LoadObjSpriteTiles`'s. A second, narrower bug of the same flavor (an
isolated-compile-only "match" that wasn't) also surfaced during this
pass's own before-integration verification: the `count = pkg->width *
pkg->height` multiplication's operand order matters for which of
`width`/`height` loads into which scratch register, and an earlier
"instruction-for-instruction" manual read-through of the two disassembly
listings side-by-side missed that the immediate offsets (`[r0]` vs
`[r0, #4]`) differed between the two loads even though the surrounding
instructions lined up - only a real byte-level diff against the ROM
(cross-checked against the object file's actual relocation table, not
assumed) caught it. Both reinforce `docs/workflow.md` step 3's
"isolated compile is a diagnostic tool, never proof" rule, and argue
for preferring an automated byte/relocation-table diff over manual
instruction-list comparison even when the latter looks thorough.

Matched, confirmed via a full clean `rm -rf build && make NON_MATCHING=1
report` (clean compile, no warnings; `objdiff-cli report generate`
shows `LoadObjSpriteTiles` at 100% fuzzy-match) and a full clean
`rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` (`La suma coincide`).
`tools/report_units.py`'s entry for `LoadObjSpriteTiles` updated to
describe it as matched rather than parked.

## Fourth pass: `LoadBg2Background`'s r7 gap narrowed further, still not closed

Revisited the still-`NAKED` `LoadBg2Background`, specifically trying the
"raise register pressure via genuinely separate, *unpinned* plain C
locals" technique documented for `sub_8006600`'s prologue fix (rather
than an explicit dummy-register pin, already ruled out for this
function in the second pass above) and the "explicit pin gets silently
dropped, but the natural allocator's own choice survives" pattern from
`sub_803AA08` (`docs/matching/issue-69-eeprom-timer.md`). All work this
pass was isolated-compile only (`cpp`+`agbcc`+`arm-none-eabi-as`,
diffed byte-for-byte against a raw `baserom.gba` extract at
`0x080355E0` via `objdump -d -M force-thumb`); nothing was integrated
into the tree, since the gap did not close. Two genuinely new, isolated
results came out of this:

1. **The dead `r7` shuttle in the ROM's prologue is not actually a
   "dead" register in the strict sense** - re-reading the raw ROM
   disassembly directly (rather than relying on the second pass's
   prose summary) shows the ROM's remap loop itself reads/writes real
   `r7` once per iteration, as scratch for the second (`+2`) halfword
   load (`ldrh r7, [r2, #2]`). The "extra push/pop" is `r7` being
   preserved as a genuine (if transient, single-instruction-lifetime)
   scratch register the loop body itself clobbers - not a pressure-
   counting-pass artifact with no runtime use at all, as the earlier
   prose characterized it. This matters because it reopens the
   "unforced natural allocation" avenue: if some C-level value
   legitimately needs a register at that point in the loop and the
   compiler's own unforced allocator reaches for `r7` on its own, both
   the loop instruction *and* the prologue/epilogue push/pop should
   follow, per the `sub_8006600`/`sub_800132C` precedent.
2. **This does happen, but not for the same variable the ROM uses.**
   Restructuring the remap loop into the ROM's actual instruction shape
   (a `mask` copy into a fresh scratch *before* each raw halfword load,
   not an in-place `mask & src[n]`, matching the operand-order
   principle from `sub_803AA08`/`sub_803AA90`) plus explicit
   (non-`r7`) pins for `i`→`r3` and the walk pointer `src`→`r2`
   (matching the ROM's own choice for those, confirmed safe since
   neither survives a call) reproduces the ROM's prologue/epilogue
   **exactly**, byte-for-byte: `push {r4,r5,r6,r7,lr}` / `mov r7,r8` /
   `push {r7}` ... `pop {r3}` / `mov r8,r3` / `pop {r4,r5,r6,r7}` /
   `pop {r0}` / `bx r0`. Every other instruction in the function
   (both `width*height` multiplications, including their ROM-matching
   reversed operand order between the alloc-size calc and the loop-
   bound calc; the literal-pool layout and order) also came out
   byte-identical. But the register-lettering **inside** the loop and
   the trailing `REG_BG2CNT` setup rotates relative to the ROM: this
   reconstruction's unforced allocator puts the accumulator in `r5`
   (ROM: `r1`), the second raw halfword load in `r1` (ROM: `r7`), and
   the uninitialized `REG_BG2CNT` scratch in `r7` (ROM: `r5`) - a
   clean 3-cycle permutation (`r1→r5→r7→r1`) of the same three
   registers the ROM itself uses, just assigned to different roles.

Extensive follow-up on that 3-cycle, none of which closed it:

- **Declaration order has zero effect here**, contradicting the
  `LoadObjSpriteTiles`/`sub_8006600` precedent that textual declaration
  order controls otherwise-untied locals' register order. Moving the
  `REG_BG2CNT` scratch's declaration earlier or later in the function,
  or reversing the four loop-local declarations' textual order, produced
  byte-identical output every time - this specific rotation is
  apparently driven by something other than declaration order (possibly
  total live pseudo-register count at each point, or an internal
  pseudo-creation-order counter not reflected in the C source's textual
  layout).
- **Explicitly pinning any one of the three rotating values (to `r1` or
  `r5` - never `r7`, per the categorical ban) does not just fail
  silently, it actively regresses the already-correct prologue.**
  Pinning the accumulator to `r1` and/or the `REG_BG2CNT` scratch to
  `r5` both independently triggered the same unexpected pathology: an
  otherwise-unrelated value (the loop accumulator) got promoted to a
  genuinely new **high** register (`r9`) with its own extra
  `mov r9,r4`/`push{r5,r6}`-shuttle prologue construct, entirely
  unlike anything the ROM does, and the carefully-won exact-match
  prologue was lost. Plausible mechanism: an explicit
  `register T x asm("rN")` block-scoped local is, in this compiler,
  effectively a *whole-function* global register reservation (matching
  this project's existing understanding of why an `r7` pin breaks
  things), and `r1` in particular is also used as an argument register
  by this function's own `LoadTaggedAsset` calls earlier in the body -
  pinning a loop-local value to `r1` appears to force the compiler to
  also treat `r1` as needing preservation around those *earlier* calls,
  cascading into unrelated register reassignment elsewhere.
- **An opaque `asm volatile` island for the loop body** (the technique
  that closed three similar gaps in `LoadObjSpriteTiles`, third pass
  above), hand-transcribing the ROM's exact
  `add`/`ldrh`/`and`/`add`/`ldrh`/`and`/`lsl`/`orr`/`strh` sequence with
  bare `r0`/`r1`/`r7` register names in the asm text, **does** reproduce
  the loop body byte-for-byte (including the real `r7` use) and, as a
  side effect, the `REG_BG2CNT` scratch's *own* natural allocation
  shifts to the correct `r5` (apparently because the opaque block no
  longer creates competing pseudo-registers for the loop's own values,
  changing how many "extra" pseudos exist by the time `REG_BG2CNT`'s
  scratch is allocated). But this technique has the **opposite**
  problem: asm-text-only (bare-name or clobber-list) uses of `r7` are
  invisible to whatever pass in this compiler decides the function's
  push/pop set - the prologue reverts to a 3-register shuttle via `r6`
  (`push {r4,r5,r6,lr}` / `mov r6,r8` / `push {r6}`), silently dropping
  `r7` from both push and pop, exactly the "explicit/opaque register
  use doesn't count" failure mode already documented for the dummy-pin
  attempt in the second pass, just triggered by a clobber list instead
  of a `register` declaration this time. Tested with `r7` both present
  and absent from the clobber list - no difference to the emitted
  push/pop set either way (confirming this is really about what the
  prologue-generation pass scans, not a correctness-vs-optimizer
  question).
- **Trying to force a *third*, genuinely fresh, unforced low-register
  pseudo to exist after the (opaque-asm) loop**, hoping it would
  consume `r5` and bump `REG_BG2CNT`'s own natural choice up to `r7`
  (reproducing the ROM's registers **and** getting `r7` back into the
  push/pop set for free), consistently failed differently than hoped:
  a trivial copy (`spacer = i;`) got optimized away entirely (no new
  register consumed, since the constraint was satisfiable by reusing
  `i`'s own register); a genuinely-materialized fresh value (an
  output-only `asm volatile` operand, with or without a real
  instruction in the template) was instead assigned a **new high
  register** (`r9`, with its own unwanted shuttle prologue) rather than
  either of the two free low registers (`r5`/`r7`) - suggesting this
  compiler's fallback for an unconstrained fresh pseudo with no
  register-copy/coalescing hint prefers a high register over a "free"
  low one, unlike the `REG_BG2CNT` scratch (which does inherit some
  low-register-preferring coloring hint from being the destination of a
  real `&=`/`|=` chain feeding a real store, not just an opaque
  asm operand).

Net result: two mutually exclusive near-misses (exact prologue/epilogue
with wrong loop-body register letters, vs. exact loop body with wrong
prologue/epilogue), and no combination of the pinning/declaration-
order/opaque-asm/pressure-raising techniques tried managed to get both
at once - each fix for one side consistently regressed the other, via
mechanisms (the `r9`-promotion pathology, the asm-clobber/push-pop
blind spot) that aren't fully understood. This is the same flavor of
stubborn, non-monotonic register-letter permutation already documented
as unresolved for `sub_8006600`'s second half and `sub_803AAD4`
(`docs/matching/issue-69-eeprom-timer.md`) - manual C-level
restructuring hit a wall in the same way. **Left as-is**: `LoadBg2Background`
remains the `NAKED` transcription from the second pass (byte-correct,
tracked as parked in `tools/report_units.py`, `base_object = None`); no
tree changes from this pass, since nothing closed. If revisited, the
most promising untried angle (per the `sub_8006600` write-up's own
suggestion for its analogous unresolved half) is a permuter search
scoped narrowly to just the loop-body-register-letters vs.
prologue-shuttle-register tradeoff, rather than further manual
C-level probing - four qualitatively different manual techniques have
now each independently failed to reconcile the two sides.
