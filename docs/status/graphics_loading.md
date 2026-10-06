# Status: graphics_loading

Asset/graphics-package loading and the "trigger effect type N" dispatch
family. Now in `src/gfx/` and `src/level/` (formerly `src/graphics/`), tracked as its own
`graphics_loading` category since `docs/rom_map.md` and the
`decomp-chunk` issue generator both treat it as a distinct system from
"core" graphics.

## Matched

- **`InitObjTileFreeList`**, **`FreeVramTileBlock`** (`src/gfx/sprite_frame.c`,
  the OBJ-tile VRAM free-list allocator's init/free pair) - matched.
- **`AllocVramTileBlock`** (`src/gfx/sprite_frame.c`) - matched.
  The `mem_alloc`-shaped next-fit search over that same free list, closed
  via one continuous `asm volatile` island spanning the search loop
  through the free-list split - see
  [issue-47-graphics-loading.md](../matching/archive/issue-47-graphics-loading.md)
  for the full writeup of the gcc-2.9 cross-jump/tail-merging gap this
  closed and the technique used.
- **`sub_8028D6C`**, **`GetFreeVramTileBytes`** (`src/gfx/sprite_frame.c`) -
  matched, both `UNUSED` (no caller anywhere in the ROM). `GetFreeVramTileBytes`
  never had its own `thumb_func_start` in the original disassembly - see
  `expected/corrections.txt`'s `split 0x08028D94` entry.
- **`FreeObjTileFreeList`**, **`QueueSpriteFrameOam`**, **`FreeSpriteFrameOamQueue`**,
  **`FlushSpriteFrameOamQueue`**, **`InitSpriteFrameOamQueue`**,
  **`LoadSpriteFrameTiles`**, **`SetupSpriteFrameOam`**, **`FreeSpriteFrameCache`**,
  **`AgeSpriteFrameCache`**, **`InitSpriteFrameCache`**, **`GetSpriteShapeSizeBits`**,
  **`FreeCategorySpriteSheet`**, **`DecompressCategorySpriteSheet`**
  (`src/gfx/sprite_frame.c`) - the per-frame overflow OAM/affine
  queue and the sprite-frame VRAM cache built on top of the allocator
  above; matched. See
  [issue-47-graphics-loading.md](../matching/archive/issue-47-graphics-loading.md)
  for the full write-up (issue #47).

- **`LoadGraphicsPackage`**-**`sub_801E96C`** (`src/gfx/graphics_package.c`) -
  issue #30's BG
  loader and its `struct bg_setup` accessors (`include/graphics_package.h`)
  and the sprite-box fitter `FitScaledSprite`/`DrawScaledSprite`. All built with
  old_agbcc (as is `spawn_start_marker.c`). The four that were NAKED
  under agbcc (the dropped-`r7` gap) are now plain C. See
  [issue-30-old-agbcc.md](../matching/archive/issue-30-old-agbcc.md), and
  [issue-30-graphics-loading.md](../matching/archive/issue-30-graphics-loading.md)
  for the earlier accessor passes (`sub_801E8F8`'s DMA-register load
  order, the trailing `asm(".align 2, 0")` zero-padding fix).

- **`InitTitleScreen`** (`src/frontend/title_screen_init.c`) - the
  title screen's constructor (`UpdateGameFrame` runs the title screen
  before the level loop: `RunTitleScreen`, then `DestroyTitleScreen`); stashes the
  icon-manager pointer, resets the OAM shadow buffer, sets up blend/
  display registers, DMA3-copies three palette banks, calls
  `LoadTitleScreenBg`/`LoadTitleScreenObjTiles`, runs the fade/audio-reset
  quartet, and starts song `0xb` - see
  [issue-65-graphics-loading.md](../matching/archive/issue-65-graphics-loading.md).
- **`LoadTitleScreenObjTiles`** (`src/frontend/title_screen_init.c`) - the
  OBJ-sprite tileset/palette loader (4-pass over `gTitleObjPackages`),
  closed via a register-pinning + opaque-asm-island pass on top of the
  previously-parked semantically-faithful reconstruction - see
  [issue-65-graphics-loading.md](../matching/archive/issue-65-graphics-loading.md)'s
  "Third pass".
- **`SpawnNitroSwitchCrate`**-**`SpawnBasicCrate`** (`src/level/spawn_crates.c`)
  - the `CreateCrate` entity-constructor trampoline family, types `0`-`7`,
  all matched (`SpawnBasicCrate`, type `0`, closed via register-pinning the
  table-resolution chain to the ROM's own registers) - see
  [issue-33-0x08021bfc-graphics-loading.md](../matching/archive/issue-33-0x08021bfc-graphics-loading.md).
- **`SpawnCrystal`**, **`SpawnCrateGem`**, **`SpawnGemPathGem`**,
  **`SpawnRedGem`**, **`SpawnGreenGem`**, **`SpawnYellowGem`**
  (`src/level/spawn_gems.c`) - issue #30: six "trigger
  effect type N" spawners (a `GetCurrentLevelFlags`/`gLevelState+2`
  collected-bit test, then a `CreateSpriteObj` part with a fixed bank
  offset, tag and type byte registered with `gTouchableList`; the
  last three hand over to `SpawnCortexBossGem` in level mode 1). All plain C
  once built with **old_agbcc** (`OLD_AGBCC_OBJS`), whose mask-before-
  `ldrb` order the ROM shows; the current agbcc misses all six, which is
  likely also what parked the `spawn_gem_platforms.c` siblings.
  Retires `asm/code_3_2_17_1e990.s`. Uses the new shared
  `include/gfx_part.h` (moved out of `cortex.c`). See
  [issue-30-graphics-loading.md](../matching/archive/issue-30-graphics-loading.md)'s
  "Tenth pass".
- **`SpawnRedGemPlatform`**, **`SpawnYellowGemPlatform`**, **`SpawnGreenGemPlatform`**,
  **`SpawnBlueGemPlatform`** (`src/level/spawn_gem_platforms.c`) - issue #31: the
  "trigger effect type N" spawners (4 of the 15-slot
  `gStaticData_0816C7D8` dispatch table's slots): sound-only-or-
  full-spawn effect triggers gated by a `gLevelState+2` flag bit.
  Parked as NAKED for a long time; all four are plain C with no pins
  once built with **old_agbcc** (`OLD_AGBCC_OBJS`). See
  [issue-31-trigger-effect-type-n.md](../matching/archive/issue-31-trigger-effect-type-n.md)'s
  "Old-compiler pass".
- **`DestroyLevelState`** (UNUSED), **`PlayCutscene`**
  (`src/level/level_cutscene.c`) - the gap between issues #33
  and #34 (no issue of its own): the game context's never-called
  destructor (tears down every singleton `InitLevelState` builds) and the
  per-level text-list pager with its palette blank/BG2-affine reset.
  Real C, current agbcc (both compilers match). Retires
  `asm/code_3_2_17_22354.s`. See
  [gap-22354-game-context.md](../matching/archive/gap-22354-game-context.md).
- **`SpawnLizard`**-**`SpawnElectricEel`** (`src/level/spawn_enemies.c`),
  **`SpawnSquid`** (`spawn_enemies.c`), **`SpawnJellyfish`**-
  **`SpawnWoodenCrusher`** (`spawn_enemies.c`; `SpawnFlamethrowerLabAssistant` closed in
  [last-eleven-naked-retry.md](../matching/archive/last-eleven-naked-retry.md)),
  **`SpawnRoomExit`**-**`SpawnCortexBoss`** (`spawn_bosses.c`) and
  **`SpawnMegaMix`**-**`SpawnIronCrate`** (`spawn_objects.c`) - the
  "two-line text popup" spawners (issue #31) and the spawner-table
  entries that follow them. All five files are built with old_agbcc and
  share `include/text_popup.h`. 33 functions were rewritten as plain C with no pins or
  asm; 22 of them were NAKED under agbcc, parked on the "r7 in the
  callee-saved set" gap, which was really a compiler mismatch.
  `SpawnVenusFlytrap` keeps its agbcc-era pinned C (plain C is 5 halfwords
  off). `SpawnRoomExit` was NAKED until the NAKED retry pass; it needs four
  register pins and an empty `asm` nudge (see
  [naked-retry-mid45.md](../matching/archive/naked-retry-mid45.md)). See
  [issue-31-old-agbcc.md](../matching/archive/issue-31-old-agbcc.md).
- **`SpawnBodySlamPower`**, **`SpawnTornadoSpinPower`**, **`SpawnDoubleJumpPower`**, **`SpawnTurboRunPower`**,
  **`SpawnStopwatch`**, **`SpawnBlueGem`**, **`CreateTouchableSprite`**, **`SpawnCrateGemMarker`**,
  **`SpawnWumpa`**, **`SpawnHoverPlayerPosition`**, **`SpawnHoverStartMarker`**, **`SpawnUnderwaterPlayerPosition`**,
  **`SpawnUnderwaterStartMarker`**, **`SpawnPlayerPosition`**, **`nullsub_23`**, **`DestroyEntitySpawner`**,
  **`CreateEntitySpawner`**, **`InitLevelState`** (`src/level/spawn_pickups.c`)
  - the `gSpriteBankTable` record-indexed OAM-trio spawner family, the
  `SpawnStartMarker` trampolines, the `gPlayer` position writers, the
  `{table_base, count}` descriptor pair, and `InitLevelState` itself - the
  "origin point" that constructs nearly every hot IWRAM global this ROM
  region references. See
  [issue-33-0x08021bfc-graphics-loading.md](../matching/archive/issue-33-0x08021bfc-graphics-loading.md).
- **`SpawnStartMarker`** (`src/level/spawn_start_marker.c`) - the
  sound-trigger dispatch/position writer at the end of the
  `LoadGraphicsPackage` cluster's scratch-buffer-style helper family
  (issue #30). Was a NAKED transcription for a long time (see "Parked"
  below for the general convention); the residual context-sensitive
  register-choice gap in the `+0x28` write closed by modeling the
  r3-pinned local as the *address of* `gPlayer`
  (`struct actor **`) rather than its dereferenced value, so gcc
  dereferences directly into the same register the `+0x28` add needs,
  with no extra `mov` - see
  [docs/matching/archive/naked-sub_801e990-matched.md](../matching/archive/naked-sub_801e990-matched.md)
  for the full derivation.
- **`TitleScreenCheatInput`**, **`UpdateVvLogoPieces`**, **`LoadUniversalLogoBg`**
  (`src/frontend/title_screen.c`, issue #65) - the "cheat
  code" detector, the 20-slot updater and BG2's tilemap-remap loader,
  NAKED until the issue #64/#65 NAKED retry: the held/pressed pair read
  into a local struct first (the ROM's `0x100` mask built in r4 and
  copied), the drain loop's end pointer as its own local (computed ahead
  of the hoisted constants), and `*dest++` in both remap branches
  (doubles `dest`'s reference count, giving it r4). See
  [issue-64-65-naked-retry.md](../matching/archive/issue-64-65-naked-retry.md).
- **`HashTitleCheatInput`** (`src/frontend/title_screen.c`) - a
  standalone one-shot rolling-hash update (rotate-left-1 then multiply
  by 521), the same primitive `TitleScreenCheatInput` inlines for its "cheat code"
  detector. Matched as real C once the rotate was register-pinned
  (`hi`/`lo` to `r3`/`r2`) to stop agbcc folding the natural
  `(v << 1) | (v >> 31)` idiom into a single Thumb `ROR` instruction the
  ROM's own build never emits. See
  [issue-65-0x08035780-graphics-loading.md](../matching/archive/issue-65-0x08035780-graphics-loading.md).
- **`UpdateTitleLogoPieces`** (`src/frontend/title_screen_init.c`) - the
  9-slot record-array per-frame updater documented under "Parked" below
  for its siblings; promoted to real C in a follow-up pass via the
  static-inline anti-CSE technique first demonstrated in
  `tile_slot_pool.c` (issue #43): every `self+CONST+i*0x34` field access
  gets its own tiny `static inline` accessor (13 of them, one per
  offset), which stops this compiler's inliner from hoisting a shared
  `self+i*0x34` slot-base register the way any single plain-C
  reconstruction otherwise does. See
  [issue-59-60-static-inline-cse-promotion.md](../matching/archive/issue-59-60-static-inline-cse-promotion.md).
- **`LoadTitleScreenBg`** (`src/frontend/title_screen_init.c`), and
  **`CommitTitleScreenFrame`**, **`DrawTitleMenuItem`**, **`DrawTitleScreen`**,
  **`DestroyTitleScreen`**, **`RunCompanyLogos`**, **`LoadVvLogoGraphics`**,
  **`InitLogoActor`**, **`UpdateLogoActor`**, **`DrawLogoActor`**
  (`src/frontend/title_screen.c`, split off
  `title_screen_init.c`) - promoted from NAKED to
  real C in the issue #65 retry pass. Both files turned out to be
  old_agbcc code (both are now on the Makefile's `OLD_AGBCC_OBJS`);
  `LoadTitleScreenBg`'s long-documented "dead r7 in the push list" gap
  was simply the wrong compiler. The others needed the usual later
  techniques: `__divsi3`/`_call_via_rN` aliases, bitfield structs for
  the DISPCNT shadow/BGCNT, an inline `operator new` wrapper, and a few
  source-order details (a chained `REG_BG2PA = scale = ...`, a
  destination pointer taken before an allocation call, a nested block
  for the ROM's stack-slot order). See
  [issue-65-naked-retry.md](../matching/archive/issue-65-naked-retry.md).
- **`InitVvLogoPieces`** (`src/frontend/title_screen.c`) - the
  20-slot seeder. Real C once the object is built with
  `-fno-strength-reduce` (the Makefile's `NO_STRENGTH_REDUCE_OBJS`):
  with strength reduction on, gcc reverses the first loop into a
  count-down, the ROM keeps `i` counting up. The flag changes no other
  real-C function in the file; it is not a global property of the
  old_agbcc objects. See
  [per-file-flags-investigation.md](../matching/per-file-flags-investigation.md).

- **`DrawTitleLogoPieces`** (`src/frontend/title_screen_init.c`),
  **`RunTitleScreen`** and **`ResetTitleLogoPieces`**
  (`src/frontend/title_screen.c`) - the 9-slot OAM builder,
  the intro sequencer and the 9-slot seeder, NAKED until the issues
  #64/#65 second NAKED retry. The seed loops are `goto` loops with
  empty-asm reference nudges for global-alloc priority. `DrawTitleLogoPieces`
  needs strength reduction on (its up-counting inner loop gets
  reversed), so the old file was split at `TitleScreenCheatInput`: the second
  half keeps `-fno-strength-reduce` for `InitVvLogoPieces`. See
  [issue-64-65-naked-retry-2.md](../matching/archive/issue-64-65-naked-retry-2.md).
- **`DrawVvLogoPieces`** (`src/frontend/company_logos.c`) - the
  20-slot OAM builder. NAKED until the #65 strength-reduction retry. It
  needs strength reduction on, so `title_screen.c` was split
  at `0x0803686C`; the new file (with `LoadUniversalLogoBg`..`DrawLogoActor`) is
  old_agbcc without `-fno-strength-reduce`. See
  [sr65-naked-retry.md](../matching/archive/sr65-naked-retry.md).

## Parked - NAKED transcription (byte-correct, not decompiled)

These are byte-exact (confirmed by a full clean `make compare`), but
as `NAKED` functions whose body is the ROM's own disassembly
transcribed instruction-for-instruction rather than real decompiled C,
they don't count as "matched" for this project's tracking - the goal
is readable C, and an asm blob wrapped in a C function signature
doesn't advance that even when byte-correct. See
[docs/workflow.md](../workflow.md)'s NAKED-transcription escape hatch
(`MakeLinkHandshakeId`/`ResetLinkSessionState` in `src/link/link_handshake.c`) for the
established convention, and each entry's linked write-up for why
plain C didn't converge.

_(none left in this range)_

See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.
