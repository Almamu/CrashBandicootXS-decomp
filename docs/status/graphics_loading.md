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
zero-padding fix) found along the way. (`sub_801E644`/`sub_801E950`,
also in these files, are NAKED transcriptions - see "Parked - NAKED
transcription" below.)

- **`LoadLevelGraphics`** (`src/graphics/level_graphics.c`) - the
  per-level setup entry point `UpdateGameFrame` calls; stashes the
  icon-manager pointer, resets the OAM shadow buffer, sets up blend/
  display registers, DMA3-copies three palette banks, calls
  `LoadBg2Background`/`LoadObjSpriteTiles`, runs the fade/audio-reset
  quartet, and starts song `0xb` - see
  [issue-65-graphics-loading.md](../matching/issue-65-graphics-loading.md).
- **`sub_8021BFC`**-**`sub_8021CE0`** (`src/graphics/graphics_loading_21bfc.c`)
  - the `sub_800FF0C` entity-constructor trampoline family, types `1`-`7`
  (`sub_8021D04`, type `0`, also in this file, is a NAKED
  transcription) - see
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
- **`sub_801E950`** (`src/graphics/graphics_package_1e8f8.c`) - packs
  a second bitfield into the same scratch-buffer byte `sub_801E8F8`
  writes. See
  [issue-30-graphics-loading.md](../matching/issue-30-graphics-loading.md).
- **`sub_8021D04`** (`src/graphics/graphics_loading_21bfc.c`) - a
  `sub_800FF0C` trampoline (type `0`) plus a per-record flags-byte
  lookup via `gUnknown_030012B4`. See
  [issue-33-0x08021bfc-graphics-loading.md](../matching/issue-33-0x08021bfc-graphics-loading.md).
- **`sub_8020E84`**, **`sub_8020F7C`**, **`sub_802107C`**,
  **`sub_802117C`** (`src/graphics/trigger_effect.c`) - the
  "trigger effect type N" twin family (4 of the 15-slot
  `gStaticData_0816C7D8` dispatch table's slots): sound-only-or-
  full-spawn effect triggers gated by a `gUnknown_030012C0+2` flag bit.
  See
  [issue-31-graphics-loading.md](../matching/issue-31-graphics-loading.md).

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
- **`LoadGraphicsPackage`** (`src/graphics/graphics_package_1e578.c`,
  real bytes guarded at the tail of `asm/code_3_2_17_188d0.s`) - the
  cluster's own namesake; the palette/tileset/tilemap loader itself,
  using the same `struct bg_package` (now in `include/graphics_package.h`,
  shared with `LoadBg2Background`/`LoadObjSpriteTiles` above). Matches
  the ROM instruction-for-instruction after heavy register pinning except
  one dropped callee-saved `r7` push/pop pair - the same
  first-pass-vs-second-pass register-pressure artifact as
  `LoadBg2Background` above - see
  [issue-30-graphics-loading.md](../matching/issue-30-graphics-loading.md)'s
  "Third pass".
See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.
