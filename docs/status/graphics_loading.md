# Status: graphics_loading

Asset/graphics-package loading and the "trigger effect type N" dispatch
family. Filed under `src/graphics/` on disk, tracked as its own
`graphics_loading` category since `docs/rom_map.md` and the
`decomp-chunk` issue generator both treat it as a distinct system from
"core" graphics.

## Matched

- **`sub_801E640`** (`src/graphics/graphics_package_1e640.c`)
- **`sub_801E8F8`** (`src/graphics/graphics_package_1e8f8.c`) - DMA3
  fills one VRAM tile with a solid color
- **`sub_801E964`**, **`sub_801E96C`**
  (`src/graphics/graphics_package_1e964.c`)

All four are accessors on the same 0x10-byte `LoadGraphicsPackage`
scratch buffer - see
[issue-30-graphics-loading.md](../matching/issue-30-graphics-loading.md)
for the full write-up, including two real compiler-codegen gotchas
(a DMA-register load-order fix, and a trailing `asm(".align 2, 0")`
zero-padding fix) found along the way.

- **`LoadLevelGraphics`** (`src/graphics/level_graphics.c`) - the
  per-level setup entry point `UpdateGameFrame` calls; stashes the
  icon-manager pointer, resets the OAM shadow buffer, sets up blend/
  display registers, DMA3-copies three palette banks, calls
  `LoadBg2Background`/`LoadObjSpriteTiles`, runs the fade/audio-reset
  quartet, and starts song `0xb` - see
  [issue-65-graphics-loading.md](../matching/issue-65-graphics-loading.md).
- **`sub_8021BFC`**-**`sub_8021CE0`** (`src/graphics/graphics_loading_21bfc.c`)
  - the `sub_800FF0C` entity-constructor trampoline family, types `1`-`7`.
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

## Parked (`NON_MATCHING`, not yet byte-exact)

- **`sub_8020E84`**, **`sub_8020F7C`**, **`sub_802107C`**,
  **`sub_802117C`** (real bytes in `asm/code_3_2_17_14674.s` under a
  `.if NON_MATCHING == 0` guard, C in
  `src/graphics/trigger_effect.c`) - the "trigger effect type N" twin
  family (4 of the 15-slot `gStaticData_0816C7D8` dispatch table's
  slots): sound-only-or-full-spawn effect triggers gated by a
  `gUnknown_030012C0+2` flag bit. The spawn-branch tail is
  instruction-for-instruction identical to the ROM; parked on two
  register-allocation gaps (the four parameters' register rotation,
  and the entry bit-test/`sub_8023278` call's register choice) - see
  `docs/matching.md`, "`graphics_loading` chunk `0x0801FA3C`-
  `0x08021668` (issue #31)", for what was tried.
- **`sub_801E644`** (real bytes in `asm/code_3_2_17_1e644.s` under a
  `.if NON_MATCHING == 0` guard, C in
  `src/graphics/graphics_package_1e640.c`) - a five-field constructor
  on the same scratch buffer as `sub_801E640`; every instruction's
  operation matches but this compiler collapses two of the ROM's
  register-copy instructions away and pushes one fewer callee-saved
  register in every phrasing tried - see
  [issue-30-graphics-loading.md](../matching/issue-30-graphics-loading.md).
- **`sub_801E950`** (real bytes in `asm/code_3_2_17_1e950.s` under a
  `.if NON_MATCHING == 0` guard, C in
  `src/graphics/graphics_package_1e8f8.c`) - matches in full shape
  except one instruction where this compiler rematerializes a mask
  constant from a still-live register instead of the ROM's fresh
  reload - see
  [issue-30-graphics-loading.md](../matching/issue-30-graphics-loading.md).
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
- **`sub_8021D04`** (real bytes in `asm/code_3_2_17_21d04.s`, C in
  `src/graphics/graphics_loading_21bfc.c`) - a `sub_800FF0C` trampoline
  plus a per-record flags-byte lookup via `gUnknown_030012B4`; every
  field/mask/branch confirmed correct, but the middle "resolve the
  flags byte address" section is 4 bytes short of the ROM's register
  allocation - see
  [issue-33-0x08021bfc-graphics-loading.md](../matching/issue-33-0x08021bfc-graphics-loading.md).

See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.
