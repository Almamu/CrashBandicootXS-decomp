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
  **`sub_8021388`**-**`sub_802155C`** (`graphics_loading_21280.c`) and
  **`sub_8021668`**-**`sub_8021BD8`** (`graphics_loading_21668.c`) - the
  "two-line text popup" spawners (issue #31) and the spawner-table
  entries that follow them. All five files are built with old_agbcc and
  share `include/text_popup.h`. 33 functions were rewritten as plain C with no pins or
  asm; 22 of them were NAKED under agbcc, parked on the "r7 in the
  callee-saved set" gap, which was really a compiler mismatch.
  `sub_801F170` keeps its agbcc-era pinned C (plain C is 5 halfwords
  off). See
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
- **`sub_80360C0`** (`src/graphics/graphics_loading_35780.c`) - a
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
- **`sub_801E788`** (`src/graphics/graphics_package_1e688.c`) -
  `LoadGraphicsPackage`'s viewport-centering helper (position math on
  one of 4 packed modes, then an unconditional shadow-OAM insert that
  also allocates and writes one affine-parameter group when centering
  is active). Every operation was already confirmed correct by an
  earlier plain-C reconstruction, but hits the exact same `r7` gap
  documented for `sub_801E688` above, via yet another mechanism: the
  ROM needs `self` in `r7` for the entire function (matching its
  4-register `push {r4-r7}`), but this compiler only folds
  `self[offset]` into a single `ldrb/ldrh/ldr rX,[r7,#imm]` when
  `self` is an ordinary (non-`register`) local - pinning `self` to
  `r7` via `register u8 *self asm("r7")` makes it stop folding offsets
  entirely, emitting a separate `add rX,rX,#imm` before every
  zero-offset dereference instead (confirmed with a minimal one-line
  repro). This is specifically an artifact of plain-C register-pin
  semantics, though, not something a `NAKED` transcription runs into
  at all (no addressing-mode-folding pass to fight when every
  instruction is written literally) - transcribed
  instruction-for-instruction and matched byte-identical on the first
  attempt. See
  [issue-30-graphics-loading.md](../matching/issue-30-graphics-loading.md)'s
  "Ninth pass".
- **`sub_8020E84`**, **`sub_8020F7C`**, **`sub_802107C`**,
  **`sub_802117C`** (`src/graphics/trigger_effect.c`) - the
  "trigger effect type N" twin family (4 of the 15-slot
  `gStaticData_0816C7D8` dispatch table's slots): sound-only-or-
  full-spawn effect triggers gated by a `gUnknown_030012C0+2` flag bit.
  See
  [issue-31-graphics-loading.md](../matching/issue-31-graphics-loading.md).
  Still parked/NAKED in the default build - follow-up passes got all
  four functions within a handful of bytes of byte-exact as real C
  (each kept in-tree under `#if NON_MATCHING`, see
  [issue-31-trigger-effect-type-n.md](../matching/issue-31-trigger-effect-type-n.md))
  but could not close the last few register-choice/scheduling gaps for
  any of them - `sub_8020F7C`/`sub_802107C` hit the exact same two
  residual gaps `sub_8020E84` did, and `sub_802117C` (a genuinely
  harder, distinctly-shaped register allocation) landed a bit further
  off, with one extra 4-byte stack-spill gap on top of those two.
- **`sub_802062C`** (`src/graphics/graphics_loading_1feec.c`) - text
  popup, tag 0x17. Plain C under old_agbcc is 62 halfwords off: the ROM
  spills `part+0x28` to its one stack slot and keeps the constant 1 in
  r8, while the reconstruction spills the constant and
  `&gUnknown_030012B4` instead. See
  [issue-31-old-agbcc.md](../matching/issue-31-old-agbcc.md).
- **`sub_8021280`** (`src/graphics/graphics_loading_21280.c`) - a
  three-way spawner gated by the `gStaticData_0816C86C` guard. Plain C
  under old_agbcc is 9 halfwords off: the ROM computes the
  `{x - 2, y - 0x1e}` point into fresh r2/r3, the reconstruction
  subtracts in place (the same gap as `sub_802209C`). See
  [issue-31-old-agbcc.md](../matching/issue-31-old-agbcc.md).
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
- **`LoadBg2Background`** (`src/graphics/level_graphics.c`) - BG2's
  palette/tileset/tilemap loader, remapping the tilemap's per-tile
  palette-select nibble into VRAM. Every operation and register in the
  body was already confirmed to match the ROM exactly via plain C
  (isolated compile, instruction-for-instruction), but the ROM's
  prologue/epilogue pushes/pops one extra dead callee-saved register
  (`r7`, via `mov r7, r8`/`push {r7}`) that the body never reads or
  writes - no plain-C phrasing reproduces it alongside the correct body
  registers at the same time, and an explicit dummy
  `register u32 r7dummy asm("r7")` referenced via an empty asm barrier
  (the same "real register variable, not just a clobber" fix that
  unblocks other registers in this project) made no difference either -
  the same gcc-2.9 allocator artifact documented for `sub_801E644` and
  `sub_801E688` above and `sub_80240E4` (`src/system/game_loop8.c`).
  Transcribed instruction-for-instruction instead. See
  [issue-65-graphics-loading.md](../matching/issue-65-graphics-loading.md).
- **`sub_80358A8`**, **`sub_8035D1C`**, **`sub_8035E14`**,
  **`sub_8035F9C`**, **`sub_8035FEC`**, **`sub_8036068`**, **`sub_80360DC`**,
  **`sub_8036154`**, **`sub_80361B0`**, **`sub_8036528`**, **`sub_8036600`**,
  **`sub_8036668`**, **`sub_803686C`**, **`sub_8036CF4`**, **`sub_8036E20`**,
  **`sub_8036EC4`**, **`sub_8036FBC`** (`src/graphics/graphics_loading_35780.c`)
  - the rest of issue #65's chunk: a 9-slot (later, 20-slot) position/
  velocity record-array updater family operating on the same 0x220-byte
  scratch object `LoadLevelGraphics` returns, a BG2 affine-scroll setup/
  flush pair, a 7-slot rolling-hash "cheat code" detector, the level-
  object subsystem's own init/run/teardown driver and its BG2 tileset/
  palette/tilemap-remap loaders, and (unrelated to the scratch object)
  an actor-part constructor/animation-state-machine/OAM-builder trio.
  Each was attempted as real C first; each hit a different flavor of
  this compiler's register-allocation or code-layout gaps (cross-jump/
  tail-merging collapsing the ROM's own duplicated address computations,
  a shared-base pointer the ROM never hoists, extensive `sb`/`sl`/`r8`/
  `ip` shuffling) that didn't converge within a reasonable number of
  passes, so these 17 are NAKED transcriptions instead - confirmed
  byte-identical via a direct assemble + `objcopy --only-section=.text`
  + byte comparison against `baserom.gba` before integrating.
  `sub_8035780` (the field-copy/accumulate updater) was promoted to real
  C in a later pass via the static-inline anti-CSE technique - see
  [issue-59-60-static-inline-cse-promotion.md](../matching/issue-59-60-static-inline-cse-promotion.md).
  `sub_8035D1C` was re-attempted with the same technique and did *not*
  close (its blocker is cross-jump/tail-merging of branch *bodies*, not
  repeated address arithmetic - same doc). See
  [issue-65-0x08035780-graphics-loading.md](../matching/issue-65-0x08035780-graphics-loading.md).

See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.
