# Status: graphics_loading

Asset/graphics-package loading and the "trigger effect type N" dispatch
family. Filed under `src/graphics/` on disk, tracked as its own
`graphics_loading` category since `docs/rom_map.md` and the
`decomp-chunk` issue generator both treat it as a distinct system from
"core" graphics.

## Matched

- **`InitObjTileFreeList`**, **`FreeVramTileBlock`** (`src/graphics/sprite_frame_pool.c`,
  the OBJ-tile VRAM free-list allocator's init/free pair) - matched.
- **`AllocVramTileBlock`** (`src/graphics/sprite_frame_queue.c`) - matched.
  The `mem_alloc`-shaped next-fit search over that same free list, closed
  via one continuous `asm volatile` island spanning the search loop
  through the free-list split - see
  [issue-47-graphics-loading.md](../matching/issue-47-graphics-loading.md)
  for the full writeup of the gcc-2.9 cross-jump/tail-merging gap this
  closed and the technique used.
- **`sub_8028D6C`**, **`sub_8028D94`** (`src/graphics/sprite_frame_queue.c`) -
  matched, both `UNUSED` (no caller anywhere in the ROM). `sub_8028D94`
  never had its own `thumb_func_start` in the original disassembly - see
  `expected/corrections.txt`'s `split 0x08028D94` entry.
- **`FreeObjTileFreeList`**, **`QueueSpriteFrameOam`**, **`FreeSpriteFrameOamQueue`**,
  **`FlushSpriteFrameOamQueue`**, **`InitSpriteFrameOamQueue`**,
  **`LoadSpriteFrameTiles`**, **`SetupSpriteFrameOam`**, **`FreeSpriteFrameCache`**,
  **`AgeSpriteFrameCache`**, **`InitSpriteFrameCache`**, **`GetSpriteShapeSizeBits`**,
  **`FreeCategorySpriteSheet`**, **`DecompressCategorySpriteSheet`**
  (`src/graphics/sprite_frame_queue.c`) - the per-frame overflow OAM/affine
  queue and the sprite-frame VRAM cache built on top of the allocator
  above; matched. See
  [issue-47-graphics-loading.md](../matching/issue-47-graphics-loading.md)
  for the full write-up (issue #47).

- **`sub_801E640`** (`src/graphics/graphics_package_1e640.c`)
- **`sub_801E8F8`** (`src/graphics/graphics_package_1e8f8.c`) - DMA3
  fills one VRAM tile with a solid color
- **`sub_801E950`** (`src/graphics/graphics_package_1e8f8.c`) - packs a
  second bitfield into the same scratch-buffer byte `sub_801E8F8`
  writes; closed via the same `mov #N; neg` opaque-asm negative-mask
  idiom as `UPDATE_ICON_FRAME_NIBBLE` (src/graphics/settings_menu6.c)
- **`sub_801E964`**, **`sub_801E96C`**
  (`src/graphics/graphics_package_1e964.c`)

All five are accessors on the same 0x10-byte `LoadGraphicsPackage`
scratch buffer - see
[issue-30-graphics-loading.md](../matching/issue-30-graphics-loading.md)
for the full write-up, including two real compiler-codegen gotchas
(a DMA-register load-order fix, and a trailing `asm(".align 2, 0")`
zero-padding fix) found along the way. (`sub_801E644`, also in these
files, is a NAKED transcription - see "Parked - NAKED transcription"
below.)

- **`LoadLevelGraphics`** (`src/graphics/level_graphics.c`) - the
  per-level setup entry point `UpdateGameFrame` calls; stashes the
  icon-manager pointer, resets the OAM shadow buffer, sets up blend/
  display registers, DMA3-copies three palette banks, calls
  `LoadBg2Background`/`LoadObjSpriteTiles`, runs the fade/audio-reset
  quartet, and starts song `0xb` - see
  [issue-65-graphics-loading.md](../matching/issue-65-graphics-loading.md).
- **`sub_8021BFC`**-**`sub_8021D04`** (`src/graphics/graphics_loading_21bfc.c`)
  - the `sub_800FF0C` entity-constructor trampoline family, types `0`-`7`,
  all matched (`sub_8021D04`, type `0`, closed via register-pinning the
  table-resolution chain to the ROM's own registers) - see
  [issue-33-0x08021bfc-graphics-loading.md](../matching/issue-33-0x08021bfc-graphics-loading.md).
- **`sub_801FDEC`** (`src/graphics/graphics_loading_1fdec.c`) - one
  instance of the "two-line text popup" spawner family (issue #31,
  second pass); spawns a part-object via `sub_8009ED0`, fires a
  `sub_803AD80` animation-table trampoline twice, packs two
  "collected" bits from a `gUnknown_030012B4`-rooted table into its
  `+0x28` bitfield, and registers itself into `gUnknown_030012F0`'s
  manager - see
  [issue-31-graphics-loading.md](../matching/issue-31-graphics-loading.md)
  for the three compiler-codegen quirks (all fixed with small
  `asm volatile` blocks) needed to close this one byte-exact. ~20
  more instances of the same family remain raw in
  `asm/code_3_2_17_1e990.s`/`asm/code_3_2_17_1feec.s` - same doc has
  the full list and what's known about each one's tail-shape
  variant.
- **`sub_8021668`**/**`sub_8021748`**/**`sub_80217D0`**/**`sub_802183C`**/
  **`sub_80218C4`**/**`sub_80218E8`**/**`sub_8021974`**/**`sub_8021998`**/
  **`sub_80219BC`**/**`sub_80219E0`**/**`nullsub_21`**/**`sub_8021A00`**/
  **`sub_8021A4C`**/**`sub_8021A70`**/**`sub_8021A94`**/**`sub_8021AB8`**/
  **`sub_8021ADC`**/**`sub_8021B00`**/**`sub_8021B24`**/**`sub_8021B48`**/
  **`sub_8021B6C`**/**`sub_8021B90`**/**`sub_8021BB4`**/**`sub_8021BD8`**
  (`src/graphics/graphics_loading_21668.o`) - issue #31, fourth pass:
  the last "two-line text popup" sibling (OAM-trio tail variant), the
  `gStaticData_084A5600`-record spawner family registering into
  `gUnknown_030012F8`, plain `sub_801A878`/`sub_801B984` trampolines, a
  `sub_800CB40`-based constructor, and 12 more plain `sub_800FF0C`
  entity-constructor trampolines (types `0x12`-`7`) - matched. This runs
  through to the end of what used to be `asm/code_3_2_17_21280.s`.
  `sub_802190C`, interleaved between two matched
  ranges of this same file, is NAKED - see "Parked - NAKED
  transcription" below. See
  [issue-31-graphics-loading.md](../matching/issue-31-graphics-loading.md)'s
  "Fourth pass".
- **`sub_8021388`**, **`sub_802155C`** (`src/graphics/graphics_loading_21280.c`)
  - issue #31, fifth pass: two more "two-line text popup" siblings
  (`sub_8021388` builds its header via `sub_801A838`; `sub_802155C` is
  the OAM-trio tail variant, registering into `gUnknown_030012F4`) -
  matched. This retires `asm/code_3_2_17_21280.s` entirely - the file no
  longer exists, replaced by this new object at the same point in
  `ldscript.txt`. `sub_8021280`/`sub_8021480`, interleaved between/around
  these two matched functions in the same file, are NAKED - see "Parked -
  NAKED transcription" below. See
  [issue-31-graphics-loading.md](../matching/issue-31-graphics-loading.md)'s
  "Fifth pass".
- **`sub_8021D80`**, **`sub_8021DFC`**, **`sub_8021E78`**, **`sub_8021EF4`**,
  **`sub_8021F70`**, **`sub_802200C`**, **`sub_80220C4`**, **`sub_802209C`**,
  **`sub_8022158`**, **`nullsub_22`**, **`sub_802218C`**, **`sub_80221A4`**,
  **`sub_80221BC`**, **`sub_80221D4`**, **`nullsub_23`**, **`sub_80221F0`**,
  **`sub_8022208`**, **`sub_8022230`** (`src/graphics/graphics_loading_21d80.c`)
  - the `gStaticData_084A5600` record-indexed OAM-trio spawner family, the
  `sub_801E990` trampolines, the `gUnknown_030012D8` position writers, the
  `{table_base, count}` descriptor pair, and `sub_8022230` itself - the
  "origin point" that constructs nearly every hot IWRAM global this ROM
  region references. See
  [issue-33-0x08021bfc-graphics-loading.md](../matching/issue-33-0x08021bfc-graphics-loading.md).

## Parked - NAKED transcription (byte-correct, not decompiled)

These are byte-exact (confirmed by a full clean `make compare`), but
as `NAKED` functions whose body is the ROM's own disassembly
transcribed instruction-for-instruction rather than real decompiled C,
they don't count as "matched" for this project's tracking - the goal
is readable C, and an asm blob wrapped in a C function signature
doesn't advance that even when byte-correct. See
[docs/workflow.md](../workflow.md)'s NAKED-transcription escape hatch
(`sub_8001CB8`/`sub_8001DB4` in `src/system/link_cable.c`) for the
established convention, and each entry's linked write-up for why
plain C didn't converge.

- **`sub_801E644`** (`src/graphics/graphics_package_1e640.c`) - a
  five-field constructor on the same scratch buffer as `sub_801E640`.
  See [issue-30-graphics-loading.md](../matching/issue-30-graphics-loading.md).
- **`sub_801E688`** (`src/graphics/graphics_package_1e688.c`) -
  `LoadGraphicsPackage`'s tile-cell-selection helper (best-fit box
  search over the shared `gStaticData_0816C644`/`674` preset table,
  plus Q8.8 scale-factor computation). Every operation was already
  confirmed correct by an earlier plain-C reconstruction, but this
  ~110-instruction, register-starved function hits the exact same `r7`
  gap as its sibling `sub_801E788` below, just via a new mechanism: the
  ROM needs `r7` in its callee-save push/pop list, and this compiler's
  prologue-generation pass never adds `r7` to that list from any
  inline-asm-based hint (confirmed two ways: a bare clobber, and a
  dummy `register ... asm("r7")` output operand - both produced a
  body byte-identical to the ROM but silently dropped `r7` from both
  the push and pop). Transcribed instruction-for-instruction instead.
  See [issue-30-graphics-loading.md](../matching/issue-30-graphics-loading.md)'s
  "Seventh pass".
- **`sub_8020E84`**, **`sub_8020F7C`**, **`sub_802107C`**,
  **`sub_802117C`** (`src/graphics/trigger_effect.c`) - the
  "trigger effect type N" twin family (4 of the 15-slot
  `gStaticData_0816C7D8` dispatch table's slots): sound-only-or-
  full-spawn effect triggers gated by a `gUnknown_030012C0+2` flag bit.
  See
  [issue-31-graphics-loading.md](../matching/issue-31-graphics-loading.md).
- **`sub_802190C`** (`src/graphics/graphics_loading_21668.o`) - a
  gated `sub_801A878`/`sub_80234F4` dispatcher, same OR-gated id-choice
  shape as the twin family above. See
  [issue-31-graphics-loading.md](../matching/issue-31-graphics-loading.md)'s
  "Fourth pass".
- **`sub_8021280`** (`src/graphics/graphics_loading_21280.c`) - a
  three-way dispatcher (not part of the "two-line text popup" family)
  gated by a `gStaticData_0816C86C`-indexed guard check.
- **`sub_8021480`** (`src/graphics/graphics_loading_21280.c`) - one more
  "two-line text popup" sibling.

Both hit the same confirmed `r7`-pin gap as `sub_8007114`
(src/graphics/graphics.c) and `sub_802190C` above - see
[issue-31-graphics-loading.md](../matching/issue-31-graphics-loading.md)'s
"Fifth pass".
- **`LoadGraphicsPackage`** (`src/graphics/graphics_package_1e578.c`) -
  the cluster's own namesake; the palette/tileset/tilemap loader itself,
  using the shared `struct bg_package` (`include/graphics_package.h`).
  Every operation and register choice was already confirmed correct
  against the ROM by an earlier plain-C reconstruction (heavy register
  pinning across every one of r0-r8/sb/sl/ip), but hit the same dropped-
  `r7`-push/pop gap documented for `sub_801E644`/`sub_801E688` above and
  `LoadBg2Background` below: reusing r6 for one more scratch temp (to
  match the ROM's own mid-loop `ldrh r6,...`) makes this compiler stop
  treating `src`'s r7 as needing a callee-save push/pop at all, even
  though the function body still writes and reads it afterwards.
  Transcribed instruction-for-instruction instead. See
  [issue-30-graphics-loading.md](../matching/issue-30-graphics-loading.md)'s
  "Eighth pass".
- **`sub_801E990`** (`src/graphics/graphics_loading_1e990.c`) - the
  sound-trigger dispatch/position writer at the end of the
  `LoadGraphicsPackage` cluster's scratch-buffer-style helper family
  (issue #30). A 96.8%-matching C reconstruction (closing the
  `movs r0,#1`/`subs r0,#0x12` mask derivation and the `ldrsh`
  register-offset form, both via the same opaque-asm techniques used
  for `sub_8021D04`) is kept in-tree under `#if NON_MATCHING` - see
  [docs/matching/naked-sub_801e990-matched.md](../matching/naked-sub_801e990-matched.md)
  for the derivation and the residual context-sensitive register-choice
  gap.

## Parked (`NON_MATCHING`, not yet byte-exact)

- **`LoadBg2Background`** (real bytes in
  `asm/code_3_2_20_28568_c99c_31784_33ef4_355e0.s` under a
  `.if NON_MATCHING == 0` guard, C in `src/graphics/level_graphics.c`) -
  BG2's palette/tileset/tilemap loader; every operation and register in
  the body matches the ROM exactly after register-pinning, but the ROM's
  prologue/epilogue pushes/pops one extra dead callee-saved register
  (`r7`) that no reachable C phrasing reproduces alongside the correct
  body registers at the same time - the same gcc-2.9 allocator artifact
  documented for `sub_801E644` above - see
  [issue-65-graphics-loading.md](../matching/issue-65-graphics-loading.md).
- **`LoadObjSpriteTiles`** (real bytes in the same new asm file, C in
  the same file) - the OBJ-sprite tileset/palette loader (4-pass over
  `gUnknown_030008BC`); semantically faithful but not yet
  register-tuned - see
  [issue-65-graphics-loading.md](../matching/issue-65-graphics-loading.md).
- **`sub_801E788`** (`src/graphics/graphics_package_1e688.c`, real bytes
  guarded in `asm/code_3_2_17_1e644.s`) - `LoadGraphicsPackage`'s
  viewport-centering helper (position math on one of 4 packed modes,
  then an unconditional shadow-OAM insert that also allocates and
  writes one affine-parameter group when centering is active). Fully
  understood, including resolving a prior pass's flagged concern about
  its sibling `sub_801E688`'s `gUnknown_03001300` writes not fitting
  the shadow buffer's 8-byte hardware-OAM stride - they do
  (`field_08 * 0x20 + 0x12` decomposes into 4 consecutive entries'
  filler halfword, the real hardware's OBJ affine-parameter overlay) -
  but not byte-exact: needs `self` in `r7` (matching the ROM's
  `push {r4-r7}`), which is only reachable through an explicit
  `register u8 *self asm("r7")` pin, and that pin itself defeats this
  compiler's immediate-offset address folding for every `self[...]`
  access (confirmed with a minimal repro) - a gcc-2.9 register-
  allocation gotcha now also confirmed (via a different mechanism -
  `r7` never entering the callee-save push/pop list at all, regardless
  of how it's referenced from inline asm) to affect its sibling
  `sub_801E688` too, which is why that one is now parked as a NAKED
  transcription instead - see "Parked - NAKED transcription" above. See
  [issue-30-graphics-loading.md](../matching/issue-30-graphics-loading.md)'s
  "Fourth pass" and "Seventh pass".
See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.
