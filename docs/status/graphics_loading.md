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

- **`LoadGraphicsPackage`**-**`sub_801E96C`** (`src/graphics/graphics_package_1e578.c`,
  `_1e640.c`, `_1e688.c`, `_1e8f8.c`, `_1e964.c`) - issue #30's BG
  loader and its `struct bg_setup` accessors (`include/graphics_package.h`)
  and the sprite-box fitter `sub_801E688`/`sub_801E788`. All built with
  old_agbcc (as is `graphics_loading_1e990.c`). The four that were NAKED
  under agbcc (the dropped-`r7` gap) are now plain C. See
  [issue-30-old-agbcc.md](../matching/issue-30-old-agbcc.md), and
  [issue-30-graphics-loading.md](../matching/issue-30-graphics-loading.md)
  for the earlier accessor passes (`sub_801E8F8`'s DMA-register load
  order, the trailing `asm(".align 2, 0")` zero-padding fix).

- **`LoadLevelGraphics`** (`src/graphics/level_graphics.c`) - the
  per-level setup entry point `UpdateGameFrame` calls; stashes the
  icon-manager pointer, resets the OAM shadow buffer, sets up blend/
  display registers, DMA3-copies three palette banks, calls
  `LoadBg2Background`/`LoadObjSpriteTiles`, runs the fade/audio-reset
  quartet, and starts song `0xb` - see
  [issue-65-graphics-loading.md](../matching/issue-65-graphics-loading.md).
- **`LoadObjSpriteTiles`** (`src/graphics/level_graphics.c`) - the
  OBJ-sprite tileset/palette loader (4-pass over `gUnknown_030008BC`),
  closed via a register-pinning + opaque-asm-island pass on top of the
  previously-parked semantically-faithful reconstruction - see
  [issue-65-graphics-loading.md](../matching/issue-65-graphics-loading.md)'s
  "Third pass".
- **`sub_8021BFC`**-**`sub_8021D04`** (`src/graphics/graphics_loading_21bfc.c`)
  - the `sub_800FF0C` entity-constructor trampoline family, types `0`-`7`,
  all matched (`sub_8021D04`, type `0`, closed via register-pinning the
  table-resolution chain to the ROM's own registers) - see
  [issue-33-0x08021bfc-graphics-loading.md](../matching/issue-33-0x08021bfc-graphics-loading.md).
- **`sub_801EA5C`**, **`sub_801EB04`**, **`sub_801EBF0`**,
  **`sub_801EC9C`**, **`sub_801ED6C`**, **`sub_801EE3C`**
  (`src/graphics/graphics_loading_1ea5c.c`) - issue #30: six "trigger
  effect type N" spawners (a `sub_8023404`/`gUnknown_030012C0+2`
  collected-bit test, then a `sub_8008434` part with a fixed bank
  offset, tag and type byte registered with `gUnknown_030012EC`; the
  last three hand over to `sub_8018D70` in level mode 1). All plain C
  once built with **old_agbcc** (`OLD_AGBCC_OBJS`), whose mask-before-
  `ldrb` order the ROM shows; the current agbcc misses all six, which is
  likely also what parked the `trigger_effect.c` siblings.
  Retires `asm/code_3_2_17_1e990.s`. Uses the new shared
  `include/gfx_part.h` (moved out of `actor_part_188d0.c`). See
  [issue-30-graphics-loading.md](../matching/issue-30-graphics-loading.md)'s
  "Tenth pass".
- **`sub_8020E84`**, **`sub_8020F7C`**, **`sub_802107C`**,
  **`sub_802117C`** (`src/graphics/trigger_effect.c`) - issue #31: the
  "trigger effect type N" spawners (4 of the 15-slot
  `gStaticData_0816C7D8` dispatch table's slots): sound-only-or-
  full-spawn effect triggers gated by a `gUnknown_030012C0+2` flag bit.
  Parked as NAKED for a long time; all four are plain C with no pins
  once built with **old_agbcc** (`OLD_AGBCC_OBJS`). See
  [issue-31-trigger-effect-type-n.md](../matching/issue-31-trigger-effect-type-n.md)'s
  "Old-compiler pass".
- **`sub_8022354`** (UNUSED), **`sub_8022468`**
  (`src/graphics/graphics_loading_22354.c`) - the gap between issues #33
  and #34 (no issue of its own): the game context's never-called
  destructor (tears down every singleton `sub_8022230` builds) and the
  per-level text-list pager with its palette blank/BG2-affine reset.
  Real C, current agbcc (both compilers match). Retires
  `asm/code_3_2_17_22354.s`. See
  [gap-22354-game-context.md](../matching/gap-22354-game-context.md).
- **`sub_801EF0C`**-**`sub_801FCB4`** (`src/graphics/graphics_loading_1ef0c.c`),
  **`sub_801FDEC`** (`graphics_loading_1fdec.c`), **`sub_801FEEC`**-
  **`sub_8020D4C`** except `sub_802062C` (`graphics_loading_1feec.c`),
  **`sub_8021280`**-**`sub_802155C`** (`graphics_loading_21280.c`) and
  **`sub_8021668`**-**`sub_8021BD8`** (`graphics_loading_21668.c`) - the
  "two-line text popup" spawners (issue #31) and the spawner-table
  entries that follow them. All five files are built with old_agbcc and
  share `include/text_popup.h`. 33 functions were rewritten as plain C with no pins or
  asm; 22 of them were NAKED under agbcc, parked on the "r7 in the
  callee-saved set" gap, which was really a compiler mismatch.
  `sub_801F170` keeps its agbcc-era pinned C (plain C is 5 halfwords
  off). `sub_8021280` was NAKED until the NAKED retry pass; it needs four
  register pins and an empty `asm` nudge (see
  [naked-retry-mid45.md](../matching/naked-retry-mid45.md)). See
  [issue-31-old-agbcc.md](../matching/issue-31-old-agbcc.md).
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
- **`sub_801E990`** (`src/graphics/graphics_loading_1e990.c`) - the
  sound-trigger dispatch/position writer at the end of the
  `LoadGraphicsPackage` cluster's scratch-buffer-style helper family
  (issue #30). Was a NAKED transcription for a long time (see "Parked"
  below for the general convention); the residual context-sensitive
  register-choice gap in the `+0x28` write closed by modeling the
  r3-pinned local as the *address of* `gUnknown_030012D8`
  (`struct actor **`) rather than its dereferenced value, so gcc
  dereferences directly into the same register the `+0x28` add needs,
  with no extra `mov` - see
  [docs/matching/naked-sub_801e990-matched.md](../matching/naked-sub_801e990-matched.md)
  for the full derivation.
- **`sub_8035D1C`**, **`sub_8036668`**, **`sub_8036CF4`**
  (`src/graphics/graphics_loading_35d1c.c`, issue #65) - the "cheat
  code" detector, the 20-slot updater and BG2's tilemap-remap loader,
  NAKED until the issue #64/#65 NAKED retry: the held/pressed pair read
  into a local struct first (the ROM's `0x100` mask built in r4 and
  copied), the drain loop's end pointer as its own local (computed ahead
  of the hoisted constants), and `*dest++` in both remap branches
  (doubles `dest`'s reference count, giving it r4). See
  [issue-64-65-naked-retry.md](../matching/issue-64-65-naked-retry.md).
- **`sub_80360C0`** (`src/graphics/graphics_loading_35d1c.c`) - a
  standalone one-shot rolling-hash update (rotate-left-1 then multiply
  by 521), the same primitive `sub_8035D1C` inlines for its "cheat code"
  detector. Matched as real C once the rotate was register-pinned
  (`hi`/`lo` to `r3`/`r2`) to stop agbcc folding the natural
  `(v << 1) | (v >> 31)` idiom into a single Thumb `ROR` instruction the
  ROM's own build never emits. See
  [issue-65-0x08035780-graphics-loading.md](../matching/issue-65-0x08035780-graphics-loading.md).
- **`sub_8035780`** (`src/graphics/graphics_loading_35780.c`) - the
  9-slot record-array per-frame updater documented under "Parked" below
  for its siblings; promoted to real C in a follow-up pass via the
  static-inline anti-CSE technique first demonstrated in
  `tile_slot_pool.c` (issue #43): every `self+CONST+i*0x34` field access
  gets its own tiny `static inline` accessor (13 of them, one per
  offset), which stops this compiler's inliner from hoisting a shared
  `self+i*0x34` slot-base register the way any single plain-C
  reconstruction otherwise does. See
  [issue-59-60-static-inline-cse-promotion.md](../matching/issue-59-60-static-inline-cse-promotion.md).
- **`LoadBg2Background`** (`src/graphics/level_graphics.c`), and
  **`sub_8035F9C`**, **`sub_8035FEC`**, **`sub_8036068`**,
  **`sub_8036154`**, **`sub_80361B0`**, **`sub_8036528`**,
  **`sub_8036E20`**, **`sub_8036EC4`**, **`sub_8036FBC`**
  (`src/graphics/graphics_loading_35d1c.c`, split off
  `graphics_loading_35780.c`) - promoted from NAKED to
  real C in the issue #65 retry pass. Both files turned out to be
  old_agbcc code (both are now on the Makefile's `OLD_AGBCC_OBJS`);
  `LoadBg2Background`'s long-documented "dead r7 in the push list" gap
  was simply the wrong compiler. The others needed the usual later
  techniques: `__divsi3`/`_call_via_rN` aliases, bitfield structs for
  the DISPCNT shadow/BGCNT, an inline `operator new` wrapper, and a few
  source-order details (a chained `REG_BG2PA = scale = ...`, a
  destination pointer taken before an allocation call, a nested block
  for the ROM's stack-slot order). See
  [issue-65-naked-retry.md](../matching/issue-65-naked-retry.md).
- **`sub_8036600`** (`src/graphics/graphics_loading_35d1c.c`) - the
  20-slot seeder. Real C once the object is built with
  `-fno-strength-reduce` (the Makefile's `NO_STRENGTH_REDUCE_OBJS`):
  with strength reduction on, gcc reverses the first loop into a
  count-down, the ROM keeps `i` counting up. The flag changes no other
  real-C function in the file; it is not a global property of the
  old_agbcc objects. See
  [per-file-flags-investigation.md](../matching/per-file-flags-investigation.md).

- **`sub_80358A8`** (`src/graphics/graphics_loading_35780.c`),
  **`sub_8035E14`** and **`sub_80360DC`**
  (`src/graphics/graphics_loading_35d1c.c`) - the 9-slot OAM builder,
  the intro sequencer and the 9-slot seeder, NAKED until the issues
  #64/#65 second NAKED retry. The seed loops are `goto` loops with
  empty-asm reference nudges for global-alloc priority. `sub_80358A8`
  needs strength reduction on (its up-counting inner loop gets
  reversed), so the old file was split at `sub_8035D1C`: the second
  half keeps `-fno-strength-reduce` for `sub_8036600`. See
  [issue-64-65-naked-retry-2.md](../matching/issue-64-65-naked-retry-2.md).

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

- **`sub_802062C`** (`src/graphics/graphics_loading_1feec.c`) - text
  popup, tag 0x17. Plain C under old_agbcc is 62 halfwords off: the ROM
  spills `part+0x28` to its one stack slot and keeps the constant 1 in
  r8, while the reconstruction spills the constant and
  `&gUnknown_030012B4` instead. The draft now sits under
  `#if NON_MATCHING`; register pins on `arg3` and the table address made
  it worse. See
  [issue-31-old-agbcc.md](../matching/issue-31-old-agbcc.md).
- **`sub_803686C`** (`src/graphics/graphics_loading_35d1c.c`) - the
  20-slot OAM builder, the same shape as `sub_80358A8`. The big NAKED
  retry left a full C draft under `#if NON_MATCHING` (329 halfwords off
  under old_agbcc). It needs strength reduction on, so closing it also
  means splitting the file at `0x0803686C`. The header and slot loops
  have the ROM's instructions; the row-copy loop and the registers
  around it do not. See
  [big-naked-retry.md](../matching/big-naked-retry.md) and
  [issue-64-65-naked-retry-2.md](../matching/issue-64-65-naked-retry-2.md).

See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.
