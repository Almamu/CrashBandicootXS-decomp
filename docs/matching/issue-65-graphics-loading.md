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
