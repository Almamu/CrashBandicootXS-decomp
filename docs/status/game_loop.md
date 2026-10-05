# Status: game_loop

The top-level per-frame game loop - `MainLoop`, `UpdateGameFrame`, and
the state/counter machinery they drive. Filed under `src/system/` on
disk, tracked as its own `game_loop` category since `docs/rom_map.md`
and the `decomp-chunk` issue generator both treat it as a distinct
system from "core" system startup/init code.

## Matched

- **Issue #9-#11 box/collision NAKED retry** ([docs/matching/issue-9-11-box-naked-retry.md](../matching/issue-9-11-box-naked-retry.md)):
  `UpdateEnemyHop` (`enemy_motion.c`), `UpdateEnemyTriggerBox` (`enemy_attack.c`),
  `UpdateEnemyOscillateX` (`enemy_ctrl.c`), `UpdateEffectCtrl` (`effect_ctrl.c`)
  and `sub_800CEAC`/`sub_800CF70` (`crate_hit.c`) are real C now; they
  were NAKED. `effect_ctrl.o` and `crate_hit.o` moved to old_agbcc
  (whole-file matches).

- `src/system/game_loop.c` (GitHub issue #34): `EndBonusRound`
  (level-start/checkpoint-restore progress-total updater) and
  `SetCheckpointAtPlayer` (its cached-state/snapshot helper) - see
  [docs/matching/issue-37-game-loop-234e8.md](../matching/issue-37-game-loop-234e8.md)
  for the register-allocation fixes that closed these two out.
- `src/system/main_loop.c` (new file, GitHub issue #45 - categorized
  `hud` by the chunk generator, but `MainLoop` itself is squarely
  `game_loop`): `MainLoop` - the game's actual top-level loop (called
  once from `AgbMain`, sets up the central per-level state object and
  the on-screen counter widget, then runs `UpdateGameFrame` forever) -
  and `GetUiText`, the UI string lookup in the current language
- `src/system/game_loop2.c`: `AddBrokenCrate`, `PressSwitchCrate`, `GetBonusPlatform`,
  `SetCrateAssistDeaths`, `SetMaskAssistDeaths`, `sub_8023120`, `GetCrateAssistDeaths`,
  `GetMaskAssistDeaths`, `sub_8023138`, `AddPendingSwitchCrates`, `sub_802314C`,
  `sub_8023158`, `ClearPowers`, `GiveTornadoSpin`, `GiveSuperBodySlam`,
  `GiveTurboRun`, `GiveDoubleJump`, `HasTornadoSpin`, `HasSuperBodySlam`,
  `HasTurboRun` (GitHub issue #34, `UpdateGameFrame`-`MainLoop` cluster -
  a `self+0x80`/`0x84`/`0x88`/`0xac`/`0xc0`/`+2`-flags accessor family
  plus the two frame-counter/limit tick functions), and (GitHub issues
  #35/#36) `HasDoubleJump` through `CheckAllCratesBroken` (49 more functions,
  formerly `asm/code_3_2_17_231cc.s`, now retired - a direct, fully
  contiguous continuation of the same `gLevelState`-pointed
  "level" object: more `self+2` flag bits, the `self+0x6c`/`0x70`/
  `0x74`/`0x78`/`0xbc` counter/threshold-pair family, the `self+0xa4`-
  `0xa9` busy-flag bank, `self+0x7c`/`0x8c`/`0x90`/`0x94`/`0x98`/`0xc4`/
  `0xc8`/`0x1c8` fields, five thin `LevelHasYellowGemEntity`-family forwarders, two
  `self+0xc4` "current index" dispatchers, and `CheckAllCratesBroken` (the
  counter-notification consumer `RunRoom`/`game_loop56.c` calls to
  lazily initialize this object - GitHub issue #37's last standing
  gap) - all matched as real C, no `NAKED` fallbacks needed. See
  [docs/matching/issue-35-36-0x080231cc-game-loop.md](../matching/issue-35-36-0x080231cc-game-loop.md)
- `src/cutscene/cutscene_player.c` (new file, GitHub issue #39): `InitSlideshow`-
  `StepBgLayerScroll` (25 functions) - extends `struct SoundChannelList`
  (slideshow.c/slideshow_display.c) with more fields, plus the "visual scrolling
  background streamer" family (docs/rom_map.md): a circular 4x4-block
  ring-buffer tilemap fed by the same custom RLE/delta token-stream
  decoder as the terrain-tile cache's `DecodeCollisionChunk` (`DecodeLayerChunk`), its
  per-frame axis-crossing driver (`ScrollBgStreamer`, matched as real C on
  the first attempt), its row/column incremental streamers
  (`StreamBgRow`/`StreamBgColumn`) and full "level load" seeder
  (`FillBgStreamer`), three wrapped-address helpers (`GetBgStreamerColumn`/
  `GetBgStreamerRow`/`GetBgStreamerCell`, matched as real C), and the object's
  construction/accessors (`SetBgStreamerSource`/`DestroyBgStreamer`/`InitBgStreamer`/
  `GetBgStreamerHeight`/`GetBgStreamerWidth`/`SetBgStreamerSizeVec`/`SetBgStreamerSize`/`DestroyBgLayerBase`/
  `InitBgLayerBase`/`ClampBgLayerScrollStep`/`ClampBgLayerScrollMax`/`ScaleBgLayerScroll`/`StepBgLayerScroll`,
  all matched as real C). All 25 are real C (built with old_agbcc, see
  [game-loop-old-agbcc.md](../matching/game-loop-old-agbcc.md)); `RunCutscenePlayer` and `DecodeLayerChunk`, the last two `NAKED` transcriptions, became real C last (see [strag1-naked-retry.md](../matching/strag1-naked-retry.md)). `asm/code_3_2_17_24810.s` is now fully retired. See
  [docs/matching/issue-39-0x08024810-game-loop.md](../matching/issue-39-0x08024810-game-loop.md)
- `src/system/game_loop3.c` (GitHub issue #40): `ScrollBgLayerBase`,
  `ResetBgLayerBase`, `SetBgLayerSource` (a viewport/parallax-scroll-layer object)
  and `IsBgLayerEnabled`/`GetBgLayerY`/`GetBgLayerX`/`GetBgLayerHeightTiles`/
  `GetBgLayerWidthTiles`/`GetBgLayerHeight`/`GetBgLayerWidth` (its field accessors), and
  the terrain tile cache's `GetCollisionChunk`/`GetTerrainHeights`/`GetSolidTerrainHeights`/
  `sub_8025228` (plain C, built with old_agbcc - see [game-loop-old-agbcc.md](../matching/game-loop-old-agbcc.md)),
  and `DecodeCollisionChunk`, the RLE/delta decoder (real C since the second
  near-miss sweep - see [near-miss-polish-2.md](../matching/near-miss-polish-2.md))
- `src/system/game_loop4.c` (GitHub issue #40): `DestroyTileCache`,
  `nullsub_4`, `GetTerrainType` (plain C, built with old_agbcc - see
  [game-loop-old-agbcc.md](../matching/game-loop-old-agbcc.md))
- `src/system/game_loop5.c` (GitHub issue #40): `GetCollisionCell`,
  `SetCollisionSource`, `SetBitmapBit`, `ClearBitmapBit`, `ClearBitmap`,
  `InitBitmap` - the terrain tile-record decode cache's constructor,
  a raw-cell-lookup variant, a floor-div-by-32 bitmap set/clear pair,
  and a `CpuSet`-based palette-bank zero-fill wrapper pair
- `src/system/game_loop10.c` (GitHub issue #37 - numbered `10` rather
  than `6` since issue #12's parallel PR independently claimed
  `crate_hit.c`/`crate_break.c` first): `SetGemPlatform`,
  `SetBonusPlatform`, `SetCrateGemPos`, `RequestGemPath`, `RequestBonusRound`,
  `RestoreCheckpoint`, `SetCheckpoint`, `EndGemPath`, `PlayNewGameCutscene`,
  `PlayIntroCutscene`, `ShowCompanyLogos`, `PlayBootCutscene`, `nullsub_24`,
  `UnpackSaveData`, `PackSaveData` - camera-position setters,
  checkpoint/level-transition snapshot helpers, the `PlayCutscene`
  mode-trampoline family, and a packed-bitfield unpacker/repacker pair
- `src/system/game_loop11.c` (GitHub issue #37): `GetLevelState` - lazily
  allocates and returns `gLevelStateSingleton`
- `src/system/game_loop8.c` (GitHub issue #37): `UpdateRoomFrame` - the
  DMA3/VRAM refresh pass gated on `self+0x0 <= 0x1000` - and
  `SetupRoomBlend` - the `REG_BLDCNT`/`REG_BLDALPHA` shadow-word rebuild
  (plain C, built with old_agbcc - see [game-loop-old-agbcc.md](../matching/game-loop-old-agbcc.md)). See
  [docs/matching/issue-37-game-loop-234e8.md](../matching/issue-37-game-loop-234e8.md)
  for the details.
- `src/system/game_loop39.c` (GitHub issue #37, follow-up pass):
  `PlayRoom` - the level-start dispatcher that allocates the
  per-level HUD widget set, the player actor, and the text-box
  singleton, then dispatches on a widget-kind field to construct one of
  three counter/ring-buffer widgets before handing off to `RunRoom`
  (now parked `NAKED`, see below). See
  [docs/matching/issue-37-game-loop-2375c.md](../matching/issue-37-game-loop-2375c.md)
  for the register-pinning/evaluation-order gotchas that closed this
  out.
- `src/system/game_loop9.c` (GitHub issue #37): `ClearRoomExit`,
  `RequestRoomExit`, `IsRoomExitRequested`, `ResumeRoomAfterPause`, `ResetObjBuffers` - a
  boolean flag clear/set/get trio, the level-end teardown, and the
  shared vram-upload-cursor/OAM-shadow flush tail
- `src/system/game_loop12.c` (GitHub issue #41): `CountCrateEntities`
  (group/item list counter with a 19-entry jump table - previously
  `NON_MATCHING`, now matched as real C by materializing the
  `item->type == 0x1a` four-load lookup chain as one opaque
  `asm volatile` block using only `r0`/`r1`, matching the ROM's own
  two-scratch-register reuse pattern, plus splitting the loop-bound
  `i` init into two statements so the count loads directly into `i`'s
  own register instead of a scratch register first - see
  [issue-41-game-loop-25894.md](../matching/issue-41-game-loop-25894.md)'s
  "closed" update), `sub_8025944`,
  `sub_8025968`, `sub_802599C` - the first two of three overlapping
  bit-grid accessors at `self+8`/`self+0x208`/`self+0x308`
- `src/system/game_loop13.c` (GitHub issue #41): `sub_80259D4`
  (sets a bit in both the `self+0x208` and `self+0x308` bit-grids at
  once - previously NAKED, now matched as real C via an
  inline-asm-materialized self-stash/n-copy pair plus a second local
  keeping the ROM's own untouched `n`-copy register alive for later
  reuse - see
  [naked-sub_80259d4-matched.md](../matching/naked-sub_80259d4-matched.md)),
  `sub_8025A0C`
  (third bit-grid setter), `sub_8025A3C` (Q8-to-int store),
  `DestroyEntityFlags` (conditional `OperatorDelete` forward), `InitEntityFlags`
  (zero two Q8 words)
- `src/system/game_loop14.c` (GitHub issue #41): `SpawnEntity`
  (table-indexed function-pointer dispatch via the interworking
  trampoline convention), `SetEntitySpawnerTable` (store two Q8 words),
  `sub_8025D54` (conditional `OperatorDelete` forward, dup of
  `DestroyEntityFlags`), `InitEntitySpawner` (zero two Q8 words, dup of
  `InitEntityFlags`)
- `src/system/game_loop15.c` (GitHub issue #41): `InitBgLayer`
  (BG-scroll-layer hardware-register/bitfield initializer - previously
  NAKED, now matched as real C via opaque inline-asm-materialized mask
  folds plus one function-owned literal pool for its three pointer-sized
  constants - see
  [naked-sub_8025d74-matched.md](../matching/naked-sub_8025d74-matched.md)),
  `GrowBgLayerRows`,
  `GrowBgLayerColumns` (streamed-tile-range growers firing a `self->0x30`-
  table trampoline per step), `ClipBgLayerColumns`, `ClipBgLayerRows` (their
  plain clamp-only counterparts)
- `src/system/game_loop16.c` (GitHub issue #41): `CommitBgLayerScroll` -
  truncates the Q8 position to a tile-scroll halfword pair and writes
  it through the `self+0x58` hardware-register pointer
- `src/system/game_loop17.c` (GitHub issue #38): `sub_802425C`,
  `nullsub_25`, `CountLevelCrates` - a bit-tested `OperatorDelete` teardown
  wrapper, an empty stub, and the medal-table per-level tally
- `src/system/game_loop18.c` (GitHub issue #38): `IsInGemPathRoom`,
  `IsInBonusRoom`, `LevelHasYellowGemEntity`, `LevelHasBlueGemEntity`, `LevelHasGreenGemEntity`,
  `LevelHasRedGemEntity`, `LevelHasGemPathGemEntity`, `CountRoomCrates`, `PlayRoomMusic`,
  `NextRoom`, `EnterGemPathRoom`, `EnterBonusRoom`, `SelectRoom` -
  medal-table entry/item-list field accessors, the sound-cue resolver,
  and the `LevelHasEntityType` constant wrappers (`LevelHasEntityType` itself is left
  raw, see below)
- `src/cutscene/slideshow_display.c` (GitHub issue #38): `SetSlideshowDispcnt` -
  trivial `gSlideshowDispcnt` setter
- `src/cutscene/slideshow_display.c` (GitHub issue #38): `DestroySlideshow`,
  `ResetSlideshow` - the `sub_802425C`-shaped teardown wrapper and a
  trivial constructor
- `src/crates/crate_reset.c` (GitHub issue #13 - numbered `22` rather
  than `17` since issue #38's PR above independently claimed
  `game_loop17.c`-`20.c` first): `ResetCrate` - resets
  `self`'s collision-response state/timer/neighbor-list-pointer block
  on reset
- `src/crates/crate.c` (GitHub issue #13, third pass for
  `IsCrateInsideRect`): `IsCrateInsideRect` (AABB-overlap test between `self`'s
  own table-driven half-width/half-height box and a caller-supplied
  box, prepended ahead of the rest since it's immediately
  ROM-adjacent), `ResolvePlayerCollisions` (viewport collision-box refresh),
  `GetCrateBelow`/`GetCrateAbove` (neighbor-list "get prev"/"get next"),
  `SetCrateBelow`/`SetCrateAbove` ("set prev"/"set next"), `GetCrateClassId`
  (UNUSED trivial constant). See
  [docs/matching/issue-13-fc70-second-continuation.md](../matching/issue-13-fc70-second-continuation.md)
  for `IsCrateInsideRect`'s register-pinning/toolchain-bug notes.
- `src/crates/crate_time_trial.c` (GitHub issue #13): `ConvertCratesForTimeTrial` (a
  state-3-countdown-expiry sweep over `gCrateList`),
  `OpenAkuAkuCrate` (viewport trampoline-pair/cue-1 firing)
- `src/crates/crate_stack.c` (GitHub issue #13): `IsCrateKindBreakable` -
  trivial `gCrateKindBreakable[idx]` lookup
- `src/crates/slot_crate.c` (GitHub issue #13): `GetSlotCrateStage` -
  `self+0x48` bits 6-7 sub-state extractor
- `src/system/game_loop29.c` (GitHub issue #13, second pass): `OpenLifeCrate`
  - cue-3 SFX plus a `gEntityFlags` bit-grid consume-if-clear and a
  `DropExtraLife` part-object spawn. See
  [docs/matching/issue-13-fc70-continuation.md](../matching/issue-13-fc70-continuation.md).
- `src/crates/crate_stack.c` (GitHub issue #13, second pass):
  `GetTopCrate`/`GetBottomCrate` (the "get prev"/"get next"
  neighbor-list-walk-and-filter helpers - previously NAKED, now matched
  as real C via source-order block placement matching the ROM's own
  layout plus an inline-asm-materialized mask check - see
  [naked-GetTopCrate-matched.md](../matching/naked-GetTopCrate-matched.md))
  and `CollideCrateWithPlayer` - a distance-gated `QueueCratePlayerCollision` dispatcher
  clearing `self+0xc` bit 3. See
  [docs/matching/issue-13-fc70-continuation.md](../matching/issue-13-fc70-continuation.md).
- `src/crates/crate.c` (GitHub issue #13, second pass): `DestroyCrate`/
  `InitCrate` (a part-object table-set/tail-call-`DestroySpriteObj` helper
  pair) and `FindLineCrossingYMajor`/`FindLineCrossingXMajor` (two fixed single-octant
  Bresenham-line-style step algorithms). See
  [docs/matching/issue-13-fc70-continuation.md](../matching/issue-13-fc70-continuation.md).
- `src/crates/crate_reset.c` (GitHub issue #13, third pass): `FindLineCrossing`
  - the full 4-octant Bresenham-line-style line-stepper `FindLineCrossingYMajor`/
  `FindLineCrossingXMajor` (crate.c) are fixed single-octant variants of -
  previously NAKED, now matched as real C via the same source-order
  block-placement technique as `GetTopCrate`/`GetBottomCrate` (a `goto`
  to a physically-earlier shared-return label) plus an opaque-asm
  materialization of the one octant case whose own return must stay
  physically separate from that shared tail (plain-C placement alone
  wasn't enough here - this compiler's cross-jump pass still unified
  it with the shared copy purely by instruction content) and per-case
  `diff`/`err` register pins (`r6`/`r0`, the ROM's own fixed roles) -
  see [naked-sub_800fdc8-matched.md](../matching/naked-sub_800fdc8-matched.md).
- `src/crates/crate_create.c` (GitHub issue #13, fourth pass, new file -
  replaces the trimmed `asm/code_3_2_17_e560_ff0c.s`, now deleted):
  `CreateCrate` - the `CreateCrate` entity-constructor trampoline
  family's own target function (two whole files, `graphics_loading_21bfc.c`/
  `graphics_loading_21668.c`, exist purely to call it with a fixed
  `type` constant). Allocates a 0x64-byte object, sets `self+0x18` to
  `&gCrateVtable` (a `+0x18` outlier of the usual `+0xC`
  table-pointer convention), then dispatches on `type` (0-0x12,
  externally confirmed by every trampoline caller) through two nested
  jump tables (15 and 19 cases) to tag `self+0x2d` and initialize
  per-type fields - including a confirmed call to `SolidifyOutlineCrate`
  (crate_break.c, issue #12) for `type == 5`. `NAKED` transcription
  (byte-correct, not real decompiled C): three extended registers
  (`r8`/`sb`/`sl`) live across the whole function, the same
  gcc-2.9-resistant shape already established for `QueueCratePlayerCollision`/
  `ApplyCrateCollision` (crate_break.c). See
  [docs/matching/issue-13-0x0800ff0c-graphics.md](../matching/issue-13-0x0800ff0c-graphics.md)
  for the full type-code-to-behavior table. *Later pass (size2 NAKED
  retry):* real C under old_agbcc (`crate_create.o` joined
  `OLD_AGBCC_OBJS`). The ROM's three placement-record pointer copies
  come from reading the record through an inline `Placement(slot)`
  (its return value is copied) in the 0xb pre-check, the `flagged`
  block and case 15. See
  [docs/matching/size2-naked-retry.md](../matching/size2-naked-retry.md).
- `src/crates/crate_draw.c` (GitHub issue #13, third pass, new file -
  it sits between `CreateCrate` (now `crate_create.c`) and `UpdateCrate`,
  so it can't join either neighbor's file): `DrawCrate` - a
  `self+0x4d`-gated reset of `self+0x30`/`self+0x38` via the
  `self+0x20`-pointer-to-manager/`self+0x2d`-tag/0x1c-stride
  hitbox-record convention `BreakCrateTouchedByPlayer` (crate_hit.c) also uses,
  then a tail call to `DrawSprite`. See
  [docs/matching/issue-13-fc70-second-continuation.md](../matching/issue-13-fc70-second-continuation.md).
- `src/system/game_loop18.c` (GitHub issue #38, follow-up pass): `LevelHasEntityType`
  (medal item-list per-flag nonzero scan) - prepended ahead of
  `IsInGemPathRoom`, contiguous with `game_loop17.c` in ROM. See
  [docs/matching/issue-38-sound-channel-family.md](../matching/issue-38-sound-channel-family.md)
  for the `ip`/r12 pin plus the pointer-arithmetic-canonicalization
  gotcha that closed this out.
- `src/cutscene/slideshow.c` (GitHub issue #38, follow-up pass):
  `RunSlideshow` (per-item sound-channel driver loop), `SkipSlides`
  (its "find next active item" index scanner), and - as of a second
  follow-up pass - `ShowSlidePicture` (VRAM-bank tile-asset streamer) too.
  `BeginSlide` (plain C once built with old_agbcc - see [game-loop-old-agbcc.md](../matching/game-loop-old-agbcc.md)) closes
  the file. See
  [docs/matching/issue-38-sound-channel-family.md](../matching/issue-38-sound-channel-family.md)
  and its second-pass addendum.
- `src/cutscene/slideshow_display.c` (GitHub issue #38, follow-up pass):
  `EndSlide` - the tail half of `RunSlideshow`'s per-item body, reused
  standalone. See
  [docs/matching/issue-38-sound-channel-family.md](../matching/issue-38-sound-channel-family.md).
- `src/crates/slot_crate.c` (GitHub issue #14, recategorized
  graphics->game_loop - a direct continuation of the same physics/
  collision subsystem file family): `DecrementSlotCrateStage`-`GetCrateTrialKind` (24
  functions) plus the unlabeled `IsCrateBusy` (the original
  disassembly never gave it its own symbol - it falls out of
  `GetCrateFallSpeed`'s trailing alignment padding) - a run of bit-field get/
  set/clear accessors and plain field accessors on the same
  "collision box" record `GetSlotCrateStage`/`ResetCrate` already operate
  on. See
  [docs/matching/issue-14-0x08010a0c-graphics.md](../matching/issue-14-0x08010a0c-graphics.md).
- **`ResolveCollisionCandidates`** (`src/objects/collision_queue.c`) - issue #14's last
  function, the collision-candidate scan/resolve helper `ResolvePlayerCollisions`
  calls once a frame. Plain C; it was NAKED. The "twelve running
  pointers" were gcc's own loop strength reduction of
  `self->records[i].field`. See
  [issues-14-53-60-last-naked.md](../matching/issues-14-53-60-last-naked.md).
- **`FreezeLevelClock`/`TickLevelClock`** (`src/system/game_loop2.c`, GitHub
  issue #34) - record 47's periodic-trigger setter/decrementer; closed
  with a targeted register-pinning recipe after the ROM's cross-call
  `r4`/`r7` register map was reproduced by pinning only the two values
  that need it (see the functions' own doc comments for the full
  recipe: an early `base` snapshot forcing `r0`, a single `off`
  register pin for the `0x234` field-offset constant, freshly-named
  locals for the second chase to stop register "stickiness", and an
  `addr`/`countdown`/`newCountdown` pin set plus true-branch-first
  digit-cascade rewrites for `TickLevelClock`'s front half and `else`
  branch). Real bytes formerly in `asm/code_3_2_17_22ea8.s` (now
  removed, folded into `src/system/game_loop2.o`).
- **`ProbeTerrain`** (`src/system/game_loop43.c`, new file - dedicated
  deep investigation) - independently flagged "still unexamined" from
  two other closed call sites this session (`sub_8009BE0`'s physics/
  collision step-probe and `PlayerHasRoomForAnim`'s input-action-check gate) and
  sketched in `docs/rom_map.md` as an umbrella dispatcher unifying
  `ProbeTerrainX`/`ProbeTerrainY` under one API. A small (148 B) 4-arm
  `switch` on `mode`, matched on the first isolated-compile attempt
  with no register pins needed - see
  [docs/matching/issue-9-10-41-0x08026628-game-loop.md](../matching/issue-9-10-41-0x08026628-game-loop.md).
- **`ProbeTerrainY`**/**`ProbeTerrainX`** (`src/system/game_loop46.c`) -
  `ProbeTerrain`'s Y-axis (floor/ceiling) and X-axis (wall) tile-scan
  resolvers, 208/216 B. Plain C, built with old_agbcc - see [game-loop-old-agbcc.md](../matching/game-loop-old-agbcc.md) and
  [docs/matching/issue-9-10-41-0x08026628-game-loop.md](../matching/issue-9-10-41-0x08026628-game-loop.md).
- **`DrawBgLayerRow`/`RedrawBgLayer`/`ResetBgLayer`/`LoadBgLayerTiles`/`LoadBgLayer`/`GetBgLayerScreenIndex`/`sub_802612C`/`sub_802613C`/`SetBgLayerScreenBase`/`SetBgLayerPriority`/`SetBgLayerColors256`/`GetBgLayerCharBase`/`SetBgLayerCharBase`/`WriteBgLayerOffsetRegs`/`WriteBgLayerCntReg`/`DestroyBgLayer`/`DrawPooledBgLayerColumn`/`ClampPooledBgLayerScrollStep`/`ReleasePooledBgLayerColumn`/`ReleasePooledBgLayerRow`/`ClipPooledBgLayerColumns`/`ClipPooledBgLayerRows`/`DrawPooledBgLayerRow`/`ResetPooledBgLayer`/`LoadPooledBgLayerTiles`/`nullsub_26`**
  (`src/system/bg_scroll_layer_25fc8.c`, new file - GitHub issue #42,
  compiled with **old_agbcc**) - the BG-scroll layer's methods (base
  table `gBgLayerVtable`: destroy, reset, load tiles, draw row,
  draw all visible rows), its BGnCNT-shadow setters/getters, the
  32-entry wrap helpers, and the tile-slot-pooled layer-0 overrides
  (table `gPooledBgLayerVtable`: draw row/column through the pool,
  release a row/column, shrink the resident range, reset, load tiles,
  clamp a scroll step). All plain C; `DrawBgLayerRow` needed a goto loop.
  `sub_802612C` was hidden in the old disassembly and `sub_802613E` was
  mislabelled (it starts at `0x0802613C`). `asm/code_3_2_17_25fc8.s`
  removed. See
  [docs/matching/issue-42-bg-scroll-layer.md](../matching/issue-42-bg-scroll-layer.md).
- **`DestroyPooledBgLayer`/`InitPooledBgLayer`/`sub_8026480`/`ResetTileSlotPool`/`AcquireTileSlot`/`ReleaseTileSlot`/`UploadTileSlot`/`SetTileSlotPoolSource`**
  (`src/system/tile_slot_pool.c`, new file - GitHub issue #43) - BG
  layer 0 of the level-layers singleton (constructor/destructor chaining
  to the `InitBgLayer` BG-scroll-layer base) and its reference-counted
  VRAM tile-slot pool (0x2000 source tiles onto 0x200 slots): reset,
  acquire, release, tile DMA, base setup. Static-inline push/slot-table helpers
  reproduce the ROM's uncached address recomputation; r1-pinned refcount
  temps; a narrow inline-asm `+0x34` bitfield update (same case as
  `InitBgLayer`); `AcquireTileSlot` (acquire) needed two more
  three-instruction asm anchors (constant-before-load, refcount update). See
  [docs/matching/issue-43-level-layers.md](../matching/issue-43-level-layers.md).
- **`LoadRoom`/`InitLevelLayers`/`DestroyLevelLayers`/`GetLevelLayers`/`SetLevelScroll`/`CommitLevelScroll`/`ScrollLevelLayers`/`ResetLevelLayers`/`sub_80269DC`/`sub_80269F8`/`sub_8026A14`**
  (`src/system/level_layers.c`, new file - GitHub issue #43) - the
  `gLevelLayersSingleton` level-layers singleton (also `gLevelLayers`):
  level load, constructor/get-or-create, destructor, the camera's
  scroll clamp (`SetLevelScroll`), per-layer method-table passes, and two
  identical predicates. All plain C; only `SetLevelScroll` needed separate
  per-axis temps. `asm/code_3_2_17_266bc.s` removed.
- **`GetTerrainFlagsAt`** (`src/system/game_loop44.c`, new file - dedicated
  deep investigation) - independently flagged "still raw" by two
  already-documented callers (`CollidePlayer`'s camera-probe tail and a
  jump-table dispatch context in `DrawAffineSpritePieces`'s own write-up). A
  56-byte wrapper around the already-matched terrain-tile-cache lookup
  `GetTerrainType` (`game_loop4.c`, GitHub issue #40): converts `(x, y)`
  to that cache's lookup units via a plain `>>3` clamped to `>= 0` on
  each axis independently, then calls `GetTerrainType` and returns only
  the flags byte it also returns directly (the `hi` out-param is
  discarded, unread by either known caller). Matched on the first
  isolated-compile attempt with no register pins needed - see
  [docs/matching/issue-9-10-0x0800a884-graphics.md](../matching/issue-9-10-0x0800a884-graphics.md).
- **`sub_8026BF8`/`sub_8026C3C`/`sub_8026C80`/`sub_8026C8C`**
  (`src/system/game_loop45.c`, new file - GitHub issue #9/#10, matching
  pass on functions already fully understood from
  `docs/matching/issue-9-0x0800a178-graphics.md`) - the single-point
  terrain-height ("floor") probes `ProbeGroundSpriteTerrain`/`ProbeGroundSpriteFloor`
  (`src/objects/ground_sprite_collide.c`) call. Both `s32 fn(void *player,
  struct probe_pos *pos, s32 *outValue)`: `sub_8026BF8` reads a signed
  height byte via the raw terrain streamer `GetTerrainHeights`; `sub_8026C3C`
  gets it via the CheckTerrainFlag-style `sub_8025228`. `sub_8026C3C`
  matched on the first isolated-compile attempt; `sub_8026BF8` needed a
  narrow register-pinned inline-asm materialization of `ldrsb` (this
  agbcc build never emits Thumb `LDRSB` from any C-level signed-byte
  array read - confirmed categorically with a minimal standalone test -
  always lowering to `ldrb`+shift instead). Bonus pass also closed the
  two tiny functions immediately following, `sub_8026C80`/`sub_8026C8C`
  - both UNUSED (no caller anywhere in the ROM), matched anyway per this
  project's usual practice. `asm/code_3_2_17_26bf8.s` trimmed to begin
  at `StepCameraDirectional`.
- **`StepCameraDirectional`/`StepCameraFacing`/`SnapCamera`/`UpdateCamera`/`OperatorDeleteArray`/`OperatorNewArray`/`OperatorDelete`/`OperatorNew`**
  (`src/system/camera_follow.c`, new file - GitHub issue #44) - the
  `gCamera` camera follower: Q8 position eased a quarter-step
  per frame toward `target + look-ahead`, published centered on screen
  (`- (120 << 8)`, `- (80 << 8)`) through `SetLevelScroll`'s level-bounds
  clamp. Mode 2 (`StepCameraDirectional`) steers the look-ahead from `target+0x24`
  direction bits, mode 1 (`StepCameraFacing`) from the `target+0x28` mirror
  flag; `SnapCamera` snaps, `UpdateCamera` is the per-frame dispatcher.
  Plus two `mem_free`/`mem_alloc(size, MEM_HEAP_EWRAM)` wrapper pairs.
  Needed r2/r3 pins on the target position, an r4-pinned easing temp, an
  empty `case 3` for the switch's decision-tree shape, and a trailing
  `asm(".align 2, 0")` - see
  [docs/matching/issue-44-camera-follow.md](../matching/issue-44-camera-follow.md).
  `asm/code_3_2_17_26bf8.s` removed.
- **Issue #12 NAKED retry (old_agbcc)**: `ClearCrateStackTouched`/`MarkCrateStackTouched`
  (`src/crates/crate_break.c`), `BounceWumpaCrate`/`OpenCheckpointCrate`/`BreakCrateInStack`/
  `OpenMysteryCrate`/`OpenSlotCrate` (`src/crates/crate_break.c`) and
  `ExplodeCrate`/`BlastNearbyCrates`/`UpdateCrates`/`DetonateNitroCrates`/`ActivateNitroSwitchCrate`/
  `ActivateIronSwitchCrate`/`SolidifyOutlineCrates`/`SolidifyOutlineCrate`/`BreakCratesInArea`/`FinishBrokenCrate`/
  `UpdateTntCountdown` (`src/crates/crate_break.c`) promoted from NAKED to real
  C - all three files moved to `OLD_AGBCC_OBJS`, the shared object
  layout named in `include/crate.h`. See
  [docs/matching/issue-12-physics-collision.md](../matching/issue-12-physics-collision.md)'s
  NAKED-retry section.
- **`LightTntCrate`** (`src/crates/crate_break.c`, new file
  - GitHub issue #12 Phase 2, lower-address half) - two of
  `QueueCratePlayerCollision`'s/`ApplyCrateCollision`'s per-edge jump-table dispatch
  targets, matched as real C: `LightTntCrate` (dispatch id `0xe`)
  switches `self` into hitbox tag `0x14`, rebuilds its hitbox record,
  and re-derives a low-nibble sub-animation value via the shared
  `+0x20`-table/`+0x2d`-tag convention's own `GetPaletteSlot` tile-asset-
  cache lookup - needing several register-pinned/inline-asm-anchored
  blocks for this compiler's usual operand-materialization-order and
  register-choice gaps in this shape (a field-store's immediate loaded
  before vs. after the field address, a byte-mask computed via runtime
  negation instead of a folded 8-bit AND immediate, and which of two
  operands' registers an OR's result lands in); `OpenSlotCrate`
  (dispatch id `0xf`) is a small `self+0x48 & 7` state switch
  forwarding to `OpenMysteryCrate`/`ExplodeCrate` or spawning a bonus object.
  See
  [docs/matching/issue-12-physics-collision.md](../matching/issue-12-physics-collision.md)'s
  Phase 2 writeup.

See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.

- **`StartTimeTrial`** (`game_loop40.c`), **`DropExtraLife`** (`game_loop29.c`),
  **`SpawnEffectPart`** (`game_loop14.c`), **`ScrollBgLayer`**/**`DrawBgLayerColumn`**
  (`game_loop16.c`), and the other functions above marked "built with
  old_agbcc" - 17 former `NAKED` transcriptions in 0x08022D50-0x08026BC0,
  now plain C. This ROM region was built with old_agbcc; see [game-loop-old-agbcc.md](../matching/game-loop-old-agbcc.md).
- **Issue #10 NAKED retry**: `UpdateEnemyHomingX`/`UpdateEnemyHomingY`
  (`enemy_motion.c`), `UpdateEnemyFlipCycle` (`enemy_motion.c`), `UpdateEnemyPatrol`
  (`enemy_patrol.c`, old_agbcc), `UpdateEnemyAttackCycle` (`enemy_attack.c`,
  old_agbcc) and `SetEnemyState` (`enemy_attack.c`, old_agbcc) promoted
  from NAKED to real C. `enemy_patrol.o`, `enemy_attack.o` moved to `OLD_AGBCC_OBJS`; under old_agbcc
  `enemy_attack.c`'s bounds setters (`SetEnemyRangeXSpeed`/`SetEnemyRangeYSpeed`/
  `SetEnemyRangeX`) no longer need register pins. The controller/target
  layout is in the new `include/part_ctrl.h`. See
  [docs/matching/issue-10-naked-retry.md](../matching/issue-10-naked-retry.md).
- **Issue #12 NAKED retry:** `DropCratesAbove` (`crate_break.c`, old_agbcc,
  the neighbor "impact spread" walk) promoted from NAKED to real C. See
  [docs/matching/issue-24-26-12-naked-retry.md](../matching/issue-24-26-12-naked-retry.md).
- **Issue #12/#13/#25 NAKED retry:** `BreakCrate` (`crate_break.c`,
  the shared "apply the collision response" landing point),
  `UpdateCrateFall` (`crate_break.c`, the position-wrap advance) and
  `UpdateCrate` (`crate_update.c`, the per-frame state tick) promoted
  from NAKED to real C, all under old_agbcc (`crate_break.o` and
  `crate_update.o` joined `OLD_AGBCC_OBJS`). `BreakCrateTouchedByPlayer`,
  `ApplyCrateCollision`, `UpdateSlotCrate` and `CreateCrate` stay NAKED with new
  C drafts under `#if NON_MATCHING`. See
  [docs/matching/issue-12-13-25-naked-retry.md](../matching/issue-12-13-25-naked-retry.md).
- **Big NAKED retry:** `UpdateGameFrame` (`game_loop55.c`, GitHub issue
  #34, ~730 instructions) promoted from NAKED to real C under old_agbcc
  (`game_loop55.o` joined `OLD_AGBCC_OBJS`). The level loop and the
  attempt loop are real `for (;;)` loops that gcc rotates, the restore
  step is a `goto` loop, and a `bitmap` pointer local set right before
  the attempt loop gives the ROM's `sb`. See
  [docs/matching/big-naked-retry.md](../matching/big-naked-retry.md).
- **Third near-miss sweep:** `UpdateSlotCrate` (`crate_break.c`, the
  position-wrap/edge-scan advance) and `UpdateExtraLife` (`extra_life.c`,
  the orbiting-hazard state machine) promoted from NAKED to real C, both
  under old_agbcc (the files' compiler). See
  [docs/matching/near-miss-polish-3.md](../matching/near-miss-polish-3.md).

- **Second big NAKED retry:** `UpdateEnemyCtrl` (`enemy_ctrl_update.c`, GitHub
  issue #10, 1132 bytes, the 18-state controller update) promoted from
  NAKED to real C under old_agbcc (`enemy_ctrl_update.o` joined
  `OLD_AGBCC_OBJS`). State 18's second `animDone` test reads the byte
  through a `vu8` so jump threading keeps the ROM's re-test; the rest
  was statement order and locals. See
  [docs/matching/big-naked-retry-2.md](../matching/big-naked-retry-2.md).

- **Late NAKED retry 3:** `HitEnemy` (`enemy_ctrl_update.c`, GitHub
  issue #10, 608 bytes, the 22-state dispatcher) promoted from NAKED to
  real C under old_agbcc. An r2 register variable held live (by empty
  asms only) across the first MarkGone's id compare puts r3 in reload's
  spill-register set, which fixes the whole reload rotation. States
  1/21/22 store the layer from an `s32` local and build the bitmap bit
  with the constant-init asm. `enemy_ctrl_update.c` has no NAKED functions
  left. See
  [docs/matching/late-naked-retry-3.md](../matching/late-naked-retry-3.md).

- **Third big NAKED retry:** `SpawnRoomEntities` (`game_loop41.c`, GitHub
  issue #40, 704 bytes, the collision-bitmap refresh + actor link pass)
  promoted from NAKED to real C under old_agbcc (`game_loop41.o` joined
  `OLD_AGBCC_OBJS`). Most of it was loop shape: old_agbcc's loop
  rotation takes a `break` inside a search loop as the loop's exit
  test, so the searches leave with `goto`. See
  [docs/matching/big-naked-retry-3.md](../matching/big-naked-retry-3.md).

- **Stack-box NAKED retry:** `BreakCrateTouchedByPlayer` (`crate_hit.c`, GitHub
  issue #12, the self/player box overlap dispatch) promoted from NAKED
  to real C under old_agbcc (`crate_hit.o` joined `OLD_AGBCC_OBJS`).
  Each use of the player box's address goes through an empty
  `asm("" : "+r")` copy so cse doesn't hold `sp+16` in a callee-saved
  register, and `px`/`py` are shared by both blocks. See
  [docs/matching/sp-box-retry.md](../matching/sp-box-retry.md).
- **Hard-register hold pass:** `RunRoom` (`game_loop56.c`, issue
  #37, the level-lifecycle state machine) promoted from NAKED to real C
  under old_agbcc (`game_loop56.o` joined `OLD_AGBCC_OBJS`). The
  one-byte `direction` stack argument of `AddPaletteCycle` is a struct with
  a zero-length array member, which makes it BLKmode, so the compound
  literal is stored straight into the outgoing slot (address first, as
  in the ROM). An r0/r1 hard-register hold puts the post-fade
  player-position copy's pointer in r2. See
  [docs/matching/hard-register-hold-retry.md](../matching/hard-register-hold-retry.md).
- **Size2 NAKED retry:** `CreateCrate` (`crate_create.c`, issue #13, the
  entity constructor behind the trampoline family) promoted from NAKED
  to real C under old_agbcc (`crate_create.o` joined `OLD_AGBCC_OBJS`).
  See [docs/matching/size2-naked-retry.md](../matching/size2-naked-retry.md).
- **Last-five NAKED retry:** `ApplyCrateCollision` (`crate_break.c`, issue #12,
  the second per-edge dispatcher) promoted from NAKED to real C under
  old_agbcc (`crate_break.o` joined `OLD_AGBCC_OBJS`; the NAKED
  `QueueCratePlayerCollision` beside it is compiler-independent). The first flag
  byte is a register union of a u32 and a one-byte struct, passed to
  `BreakCrateInStack` as that struct, so it goes in QImode and its spill slot
  is reloaded with `mov r5, sp; ldrb`. See
  [docs/matching/last5-naked-retry.md](../matching/last5-naked-retry.md).
- **Huge NAKED retry 3:** `QueueCratePlayerCollision` (`crate_break.c`, issue #12,
  the 3840-byte collision-response commit) promoted from NAKED to real
  C under old_agbcc, from the 33-halfword draft the second pass left.
  A `u8 *st = &self->state` local declared last fixes the swapped spill
  slots, the first code lookup passes the table as an inline argument,
  the first slope check ends in `else edge = dirX` (its dead reload moves
  reload's round-robin for the second check), and two small tweaks fix
  the last four branch targets. See
  [docs/matching/huge-naked-retry-3.md](../matching/huge-naked-retry-3.md).

## Parked - NAKED transcription (byte-correct, not decompiled)

- **Now matched as real C (hard-register hold pass, see Matched); entry kept for history.** **`RunRoom`** (`src/system/game_loop56.c`, new file - GitHub
  issue #37, ROM `0x08023A1C`-`0x0802400C`) - the ~650-instruction
  level-lifecycle state machine `PlayRoom` unconditionally hands off
  to (`game_loop39.c`). Its 6-case jump table (state `1`/`6` share one
  code block) fires `AddPaletteCycle` "fx queue" calls - a palette
  color-cycle animation (`(u16 *)0x05000000`, GBA palette RAM, passed
  as the queue's own `targets` argument) rather than the HUD-digit
  rotation that function's other call sites drive - against
  `gThemePaletteCycle2`/`0816C81E`/`0816C830`/`0816C842`/`0816C862`,
  now confirmed as plain, tightly-packed `u16[]` index-list arguments
  (each exactly `list_count * 2` bytes, back-to-back in ROM) rather
  than per-level records with their own shape - closing that open
  question. Past the dispatch, a shared tail rebuilds the player's OAM
  entry and re-derives its `+0x29` low nibble, then a wait loop polls
  `IsRoomExitRequested` (`gRoomExitRequested`, `game_loop9.c`) until ready before
  firing the fade (`FadePaletteToBlack`), and a post-fade tail counts
  `gCrateList` entries in physics state `0xA`
  (`gCrateHitResponse`'s own convention,
  [docs/matching/issue-12-physics-collision.md](../matching/issue-12-physics-collision.md))
  before flushing every hot IWRAM widget-manager global. Parked `NAKED`
  rather than real C: beyond the sheer instruction count, the state
  `1`/`6` and state `5` jump-table targets converge on one shared
  physical tail block *mid-case-body* (after state 1/6's second
  `AddPaletteCycle` call has already begun loading its own arguments) -
  compiler-internal cross-jump-table-target block sharing this
  project's `goto`-restructuring toolbox targets *within* one `switch`,
  not across two separate jump-table entries. See
  [docs/matching/issue-37-game-loop-2375c.md](../matching/issue-37-game-loop-2375c.md)
  for the full dispatch map. **This closes GitHub issue #37.**

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

- **Now matched as real C (stack-box NAKED retry, see Matched and docs/matching/sp-box-retry.md); entry kept for history.** **`BreakCrateTouchedByPlayer`** (`src/crates/crate_hit.c`, GitHub issue #12) -
  builds `self`'s and the player's AABB from the shared
  `+0x20`-table-pointer/`+0x2d`-tag hitbox-record convention
  (`GetSpriteBounds`/`GetSpriteHitbox` in `sprite.c`), dispatches to
  `ExplodeCrate`/`BreakCrateInStack` on overlap. See
  [docs/matching/issue-12-physics-collision.md](../matching/issue-12-physics-collision.md).
  The issue #12/#13/#25 retry left a near-miss C draft (old_agbcc: gcc
  keeps the player box's `sp+0x10` in a register), see
  [docs/matching/issue-12-13-25-naked-retry.md](../matching/issue-12-13-25-naked-retry.md).
- **`QueueCratePlayerCollision`/`ApplyCrateCollision`** (`src/crates/crate_break.c`, new
  file - GitHub issue #12 Phase 1) - the physics/collision subsystem's
  two largest, most tangled dispatchers, closed by NAKED transcription
  rather than real C: `QueueCratePlayerCollision` (~3840 B, not the ~1960 B this
  issue's original read-only pass estimated) is the subsystem's
  collision-response commit - three nested jump tables (a 7-case hitbox
  selector, the 6-case per-edge handler dispatch
  `docs/rom_map.md`/this issue's write-up already described, and a
  9-case post-processing dispatch), a 5-slot "recently touched" ring
  buffer inside `gPlayer`, and ~30 distinct callees.
  `ApplyCrateCollision` (1032 B) is a further 6-case jump-table dispatcher
  `QueueCratePlayerCollision` itself calls into, sharing the exact same per-edge
  handler family. Both verified byte-exact via a full clean
  `make compare`. `asm/code_3_2_17_d18c.s` is now gone entirely - see
  [docs/matching/issue-12-physics-collision.md](../matching/issue-12-physics-collision.md)'s
  Phase 1 appendix for the confirmed dispatch maps (the basis for this
  issue's Phase 2 parallel split of the remaining 18 leaf functions).
  `ApplyCrateCollision` is now real C (last-five NAKED retry, see Matched);
  `QueueCratePlayerCollision` is now real C too (huge NAKED retry 3, see Matched);
  entry kept for history.
- **Now matched as real C (issue #12/#13/#25 NAKED retry, see Matched); entry kept for history.** **`BreakCrate`** (`src/crates/crate_break.c`; `DropCratesAbove` was
  promoted by the issue #12/#24/#26 retry; the
  rest of this file was promoted to C by the NAKED-retry pass, see
  Matched) (originally with `BounceWumpaCrate`/`OpenCheckpointCrate`/`BreakCrateInStack`/
  `OpenMysteryCrate`,, new file -
  GitHub issue #12 Phase 2, lower-address half) - NAKED transcriptions
  of the direct dispatch targets both `QueueCratePlayerCollision`'s and
  `ApplyCrateCollision`'s per-edge jump tables call (`BounceWumpaCrate`,
  `OpenCheckpointCrate`, `BreakCrateInStack`) plus their own transitive callees
  (`BreakCrate`, called from `BreakCrateInStack`; `OpenMysteryCrate`/
  `DropCratesAbove`, called from `BreakCrate`) - jump-table-heavy
  (`BreakCrate`'s own 23-case table is the largest in this subsystem
  after `QueueCratePlayerCollision`'s three) and/or built on this subsystem's
  confirmed `r8`/`sb`/`sl`-triple-accumulator-resistant shape
  (`DropCratesAbove`). See
  [docs/matching/issue-12-physics-collision.md](../matching/issue-12-physics-collision.md)'s
  Phase 2 writeup.
- **Now all matched as real C (the last one, `UpdateSlotCrate`, in the third
  near-miss sweep; see Matched); entry kept for history.** **`ExplodeCrate` through `UpdateSlotCrate`** (12 functions:
  `ExplodeCrate`, `BlastNearbyCrates`, `UpdateCrates`, `DetonateNitroCrates`,
  `ActivateNitroSwitchCrate`, `ActivateIronSwitchCrate`, `SolidifyOutlineCrates`, `SolidifyOutlineCrate`,
  `BreakCratesInArea`, `FinishBrokenCrate`, `UpdateTntCountdown`, `UpdateSlotCrate` -
  `src/crates/crate_break.c`, new file - GitHub issue #12 Phase 2,
  higher-address half, sibling pass) - the direct/transitive callees of
  `QueueCratePlayerCollision`'s and `ApplyCrateCollision`'s per-edge jump table reachable
  from `ExplodeCrate` up through the end of this whole cluster
  (0x0800EEF0-0x0800FC70). All twelve closed by NAKED transcription for
  the same gcc-2.9-resistant register-shape reasons as Phase 1's two
  dispatchers - each re-triggers either the `+0x20`/`+0x2d`-hitbox-record
  AABB-build idiom or plain high-register (`r8`/`sb`/`sl`) cross-block
  reuse under -O2. Verified byte-exact via a full clean `make compare`.
  `asm/code_3_2_17_e560.s` is now gone entirely - both this half and
  the lower-address half above (`src/crates/crate_break.c`) are fully
  consumed. See
  [docs/matching/issue-12-physics-collision.md](../matching/issue-12-physics-collision.md)'s
  Phase 2 appendix for the confirmed per-function roles.
- **Now matched as real C (issue #9-#11 NAKED retry, see Matched and docs/matching/issue-9-11-box-naked-retry.md); entry kept for history.** **`sub_800CEAC`/`sub_800CF70`** (`src/crates/crate_hit.c`, new
  file - dedicated deep investigation) - the two functions formerly
  tracked as unexamined raw bytes between `PlayerAnimWouldTouchCrate` (issue #9/#10)
  and `BreakCrateTouchedByPlayer` (issue #12); recategorized `graphics` -> `game_loop`
  since both are called only from `QueueCratePlayerCollision`. `sub_800CF70` walks
  `self`'s `GetCrateAbove`/`GetCrateBelow` neighbor list, sets a caller
  out-param when either exists, and - for the "prev" neighbor, gated by
  the subsystem's own `self+0x4d&0x7f==1` exclusion - builds its AABB
  via the shared `+0x20`-table convention and tests it against a
  caller-supplied box, confirming and sharpening `docs/rom_map.md`'s
  existing partial note on this function. `sub_800CEAC` builds a hybrid
  AABB (the player's hitbox quad positioned at `self`'s location,
  optionally widened when player state byte `+0x90` is set) and tests
  it the same way. Both NAKED: the same single-inlined-AABB-build shape
  `BreakCrateTouchedByPlayer`/`PlayerAnimWouldTouchCrate` already document as gcc-2.9-resistant. See
  [docs/matching/issue-9-10-0x0800ceac-graphics.md](../matching/issue-9-10-0x0800ceac-graphics.md).
- **Now matched as real C (issue #12/#13/#25 NAKED retry, see Matched); entry kept for history.** **`UpdateCrateFall`** (`src/crates/crate_break.c`, GitHub issue #13,
  second pass) - a per-frame position-wrap advance keeping `sb`/`r8`
  live as two extra callee-saved accumulators throughout, the same gap
  as `BreakCrateTouchedByPlayer` above. See
  [docs/matching/issue-13-fc70-continuation.md](../matching/issue-13-fc70-continuation.md).
- **Now matched as real C (late NAKED retry 3, see docs/matching/late-naked-retry-3.md); entry kept for history.** **`HitEnemy`** (`src/enemies/enemy_ctrl_update.c`, new file - GitHub
  issue #9/#10, foundational investigation of the large still-raw
  `0x0800B8DC`-`0x0800D040` cluster). An entity-vtable slot of the same
  object type (`gEnemyCtrlVtable`) as its ROM neighbour
  `UpdateEnemyCtrl` (now real C, see Matched), never called by it. A
  22-case dispatcher on its own third argument - 17 of the 22 states
  are no-ops, the other two distinct paths are an
  ambient-sound-spawn-plus-reflag tail and a
  spawn-and-launch-a-child-object handler. (Issue #10 NAKED retry: a
  21-halfword old_agbcc draft under `NON_MATCHING`, off only in reload
  scratch registers - see
  [issue-10-naked-retry.md](../matching/issue-10-naked-retry.md).) See
  [docs/matching/issue-9-10-0x0800b8dc-graphics.md](../matching/issue-9-10-0x0800b8dc-graphics.md)
  for the 22-case dispatch map.
- **`SetEnemyMotionY`/`SetEnemyMotionX`/`SetEnemyAnimMode`** (`src/enemies/enemy_ctrl.c`,
  new file - GitHub issue #9/#10, Phase 2 of the `0x0800B8DC`-cluster
  investigation above, tackling the three `(self, mode)`-shaped trigger
  primitives that pass's own doc flagged as shared by nearly every
  dispatch state). All three real C, matched clean - much smaller and,
  unlike `UpdateEnemyCtrl`/`HitEnemy`, free of the `self`/`owner`
  register-allocation gap (straight-line, no branches). `SetEnemyMotionY`/
  `SetEnemyMotionX` cache `mode` into `self+0x7c`/`self+0x78` and delegate
  to the already-matched `StartCtrlTargetMotionYFromSet`/`StartCtrlTargetMotionXFromSet`
  (`ctrl.c`); `SetEnemyAnimMode` caches into `self+0x68` and fires
  `_call_via_r3` directly, indexing `self+0x84`'s own pointer array by
  `mode` rather than going through the shared `gCtrlMotionRecords`
  table the other two use. See
  [docs/matching/issue-9-10-0x0800b8dc-graphics.md](../matching/issue-9-10-0x0800b8dc-graphics.md)'s
  "Phase 2" section for the full writeup.
- **`UpdateEnemyOscillateX` now matched as real C (issue #9-#11 NAKED retry, see docs/matching/issue-9-11-box-naked-retry.md); `UpdateEnemyBob`/`UpdateEnemyOscillateY` are real C too since the issue #9/#10 raw-asm pass (docs/matching/issue-9-raw-asm-pass.md).** **`UpdateEnemyOscillateX`/`UpdateEnemyBob`/`UpdateEnemyOscillateY`** (still NAKED after the
  issue #10 NAKED retry, drafts under `NON_MATCHING`; `UpdateEnemyHomingX`/
  `UpdateEnemyHomingY`/`UpdateEnemyFlipCycle` are now real C, see Matched) - originally
  **`UpdateEnemyHomingX`/`UpdateEnemyHomingY`/`UpdateEnemyFlipCycle`/`UpdateEnemyOscillateX`/
  `UpdateEnemyBob`/`UpdateEnemyOscillateY`/`LaunchHarmfulEffectPart`/`CreateKnockedEnemyCtrl`**
  (`src/enemies/enemy_motion.c`-`enemy_ctrl.c`, new files - GitHub
  issue #9/#10, Phase 3 of the `0x0800B8DC`-cluster investigation, the
  "remaining leaves" the Phase 1 doc's priority list named). `UpdateEnemyHomingX`/
  `UpdateEnemyHomingY` are the X-axis/Y-axis "homing velocity-target setter"
  pair; `UpdateEnemyFlipCycle` is state 7's `self+0x68`-dispatched callee (a
  mirror-flag toggle plus a wrapping 0-3 counter advance);
  `UpdateEnemyOscillateX`/`UpdateEnemyBob`/`UpdateEnemyOscillateY` are a `gSineTable`
  sine-wave-oscillator family. All six matched as NAKED - each hit a
  *different* gcc-2.9/this-agbcc-build code-selection gap (branch-
  polarity/cross-jump-merging differences for the first pair, bit-
  toggle instruction-sequencing for the third, constant-materialization
  and register-copy-operand choices for the oscillator trio) despite
  isolated real-C attempts getting the full branch/dispatch structure
  and even established idioms like the `(s32)(x<<27)<0` mirror-flag
  test right - see the doc's own "Phase 3" section for the full
  per-function breakdown. `LaunchHarmfulEffectPart` (a thin `LaunchEffectPart` wrapper,
  state 18's floating-popup spawner) and `CreateKnockedEnemyCtrl` (`HitEnemy`
  states 19-20's child-object allocator, resolving `self+0xc` to the
  fixed `gKnockedEnemyCtrlVtable` table) both matched as real C. See
  [docs/matching/issue-9-10-0x0800b8dc-graphics.md](../matching/issue-9-10-0x0800b8dc-graphics.md)'s
  "Phase 3" section for the full writeup.
- **`SetEnemyState`/`SetEnemyRangeXSpeed`/`SetEnemyRangeYSpeed`/`SetEnemyRangeX`** (all
  real C since the issue #10 NAKED retry, see Matched; kept here for
  the history) (`src/enemies/enemy_attack.c`, new file - GitHub issue #9/#10, the
  last four functions of the old `asm/code_3_2_17_c6a8.s`, now fully
  retired). `SetEnemyState` is the `menu_ui` dialog-widget system's own
  18-state `self+0x74` update, called from all 31 confirmed `menu_ui`
  dispatch-table entries - despite the "menu_ui" framing it turns out to
  run on the exact same `self`/`owner`/`self+0xc`-anchor/`self+0x84`-table
  object shape as the rest of this cluster, and its case bodies manually
  re-inline `SetEnemyMotionY`/`SetEnemyMotionX`/`SetEnemyAnimMode`'s own `bl` targets
  rather than calling those three wrapper functions - confirming these
  are literal instances of the same object type, not merely a
  structurally-similar sibling. NAKED: several case groups compile the
  identical inlined `SetEnemyAnimMode(self,0)` sequence at deliberately
  separate, unmerged jump-table addresses, the exact tail-merging trap
  the Phase 3 entry above already documents this agbcc build hitting,
  combined with the same `self`/`owner` register-pressure shape the rest
  of this cluster's dispatchers share. `SetEnemyRangeXSpeed`/`SetEnemyRangeYSpeed`/
  `SetEnemyRangeX` (the `self+0x70`-relative X/Y homing-bound accessor
  triple) matched as real C on the first attempt, using a register-pinned
  local plus an empty `asm volatile` barrier to force the ROM's own
  "load owner field, then shift the radius" instruction order. See
  [docs/matching/issue-9-10-0x0800b8dc-graphics.md](../matching/issue-9-10-0x0800b8dc-graphics.md)'s
  "Phase 4" section for the full writeup.
- **`UpdateEffectCtrl` now matched as real C (issue #9-#11 NAKED retry, see Matched and docs/matching/issue-9-11-box-naked-retry.md); entry kept for history.** **`UpdateEffectCtrl`/`EffectCtrlHandleEvent`/`nullsub_3`/`DestroyEffectCtrl`/`InitEffectCtrl`**
  (`src/objects/effect_ctrl.c`, new file - GitHub issue #9/#10, the
  final piece of the `0x0800B8DC`-cluster investigation, closing out
  the entire 43-function cluster). `UpdateEffectCtrl` (NAKED) inlines the
  "flag active + bitmap-set" idiom (`actor_part27c.c`'s `UpdateOneShotAnimCtrl`)
  three times over, each independently gated (a `_call_via_r1` hit-probe
  reporting no hit, a flags-bit-3 test, and a `+0x38` byte test).
  `EffectCtrlHandleEvent`/`nullsub_3` are genuine empty stubs, matched as real C.
  `DestroyEffectCtrl`/`InitEffectCtrl` (both real C) are two more constructors in
  the `CreateStompedHopPadCtrl`/`DestroyStompedHopPadCtrl`/`CreateKnockedEnemyCtrl` family, both re-pointing
  `self+0xc` at `gEffectCtrlVtable`. `InitEffectCtrl` sits right at the
  physics/collision subsystem's own boundary
  ([docs/matching/issue-12-physics-collision.md](../matching/issue-12-physics-collision.md))
  but is confirmed to still be a plain entity constructor in this
  cluster, immediately followed with no gap by the already-matched
  `PlayerAnimWouldTouchCrate` (`crate_touch.c`). See
  [docs/matching/issue-9-10-0x0800b8dc-graphics.md](../matching/issue-9-10-0x0800b8dc-graphics.md)'s
  final section for the full writeup.
- **`UpdateEnemyShooter`** (`src/enemies/enemy_shooter.c`, new file - GitHub
  issue #9/#10) - the last raw function in the cluster's own
  `asm/code_3_2_17_bfa8.s` chunk, called only from `UpdateEnemyCtrl` state
  15. A small `self+0x68`-keyed 2-way dispatcher gated by a
  `__modsi3` "close enough" check against `gRoomFrameCount` (here
  read as a plain word, not the table-base-pointer role `UpdateEnemyAttackCycle`
  uses it in) plus `self->0x48`/`self->0x4c`; on pass, triggers
  `SetEnemyAnimMode(self,2)`/`SetEnemyAnimMode(self,7)` for `self->0x68==0`/`4`.
  On failure, re-dispatches through `owner` (`self->0x70`):
  `owner->0x38` set triggers `SetEnemyAnimMode(self,0)`/`SetEnemyAnimMode(self,4)`
  for `self->0x68==2`/`7`; `owner->0x38` clear instead gates a
  `LaunchHarmfulEffectPart(0xc,6,0,d,0x400,owner)` call (`d=-0xa` for mode 2,
  `d=8` for mode 7) behind an `owner->0x30`/`owner->0x34` magic-constant
  check, tagging the returned record's `+0xa` byte with `8` on success.
  Small enough to avoid this cluster's usual `self`/`owner`
  register-pressure trap - matched as real C, needing `self` pinned to
  `asm("r4")` (gcc's unforced allocator otherwise duplicates `self`
  into a spare `r5` just to re-read `self->0x68` a second time) and the
  `gRoomFrameCount` read hoisted into its own statement ahead of
  `self->0x48`'s (two independent loads gcc's scheduler otherwise
  reorders vs. the ROM). This fully consumes `asm/code_3_2_17_bfa8.s` -
  retired from `ldscript.txt` entirely. See
  [docs/matching/issue-9-10-0x0800b8dc-graphics.md](../matching/issue-9-10-0x0800b8dc-graphics.md)'s
  "`UpdateEnemyShooter`" entry.
- **Now matched as real C (see docs/matching/strag1-naked-retry.md); entry kept for history.** **`LaunchEffectPart`/`DropWumpa`** (`src/system/game_loop14.c`, GitHub
  issue #41) - two part-object spawn helpers. Under old_agbcc, plain C
  is 61 and 5 halfwords off (register allocation, and one constant
  load's scheduling); their sibling `SpawnEffectPart` is matched. See
  [game-loop-old-agbcc.md](../matching/game-loop-old-agbcc.md).
- **Now matched as real C (third big NAKED retry, see Matched and docs/matching/big-naked-retry-3.md); entry kept for history.** **`SpawnRoomEntities`** (`src/system/game_loop41.c`, GitHub issue #34/#40/
  #41) - `self` is `*gEntityFlags`: refreshes the collision
  bitmaps, spawns `list`'s unseen items through `SpawnEntity`, then
  links spawned actors by a `links` array. Plain C under old_agbcc is
  153 halfwords off: everything through the first link pass is
  byte-exact, but the ROM walks the second pass's actor-list searches
  with a strength-reduced pointer and no peeled first iteration. See
  [docs/matching/issue-34-game-loop-8022d50-80255d4.md](../matching/issue-34-game-loop-8022d50-80255d4.md)
  and [game-loop-old-agbcc.md](../matching/game-loop-old-agbcc.md).
- **Now matched as real C (big NAKED retry, see Matched and docs/matching/big-naked-retry.md); entry kept for history.** **`UpdateGameFrame`** (`src/system/game_loop55.c`, GitHub issue #34,
  ROM `0x080225A0`-`0x08022BF0`) - the main per-frame game-loop driver,
  called once a frame from `MainLoop` with `self` =
  `gLevelState`. Traced branch-by-branch: a level-load loop
  (`InitTitleScreen`/`RunTitleScreen`/`RunCredits`) that spins until the
  level finishes loading; a confirmed 5-case jump table on
  `self->0xc4` (doubling as both the literal player-state enum value
  *and* the retry-loop's `RunLevelSelect` seed) - gates
  `HasSuperBodySlam`/`HasDoubleJump`/`HasTornadoSpin`/`HasTurboRun` transition to
  `GiveSuperBodySlam`/`GiveDoubleJump`/`GiveTornadoSpin`/`GiveTurboRun` respectively
  (case 3's own gate additionally free-runs a `GetCompletionPercent` timeout
  that increments `self->0xc4` past 0x63 frames), case 4 has no gate/
  transition and is unconditional; states past this table's range
  (`self->0xc4 - 0x14 > 4`) instead OR-set a flag byte on
  `GetCurrentLevelFlags`'s object when `self->0xdc`'s level object is in state
  3. End-of-frame: a double-buffered `self+0xe4`/`self+0x14c` snapshot
  pair (each exactly `0x68` bytes, DMA3 fixed-source zero-filled at
  entry then restored/re-saved every retry-loop pass), an actor-
  category processing loop keyed on an `r8`-resident status flag (0 =
  keep going, 1 = check `GetLives` for early-out, 2 = done this
  frame) that ping-pongs the `gEntityFlags` collision-bitmap
  buffer between `self+0x1b4`'s two halves via `InitEntityFlags`/
  `OperatorNew(0x408)`, and refreshes the HUD icon (`SetHudCrateTotal`) each
  pass. The whole per-category loop, and even the outer state-dispatch
  block above it, can run several times within one call (loops back via
  `GetLives`/`RunContinuePrompt` gating) before the function actually
  returns to `MainLoop`. At ~730 instructions with three persistent
  cross-call registers (`r7` = `&self->0xc4`, `sl` = `&self->0xc8`,
  `r8`/`sb`) plus six SP-relative field-address slots (`&self->0xe0`,
  `&self->0xe4`, `&self->0xcc`, `&self->0xbc`, `&self->0xac`, and a
  saved `self->0x78` snapshot) all live simultaneously across this
  nested loop structure, a real C reconstruction was well past what
  this project's register-pin/`asm volatile`/`goto`-restructuring
  toolbox has closed in one pass on a function this size; closed
  instead as a byte-exact NAKED transcription (mechanically converted
  from the confirmed-traced ROM disassembly, formerly
  `asm/code_3_2_17_225a0.s`, now retired). See `docs/matching.md`'s
  entry for this function for the full trace.
- **Now matched as real C (issue #12/#13/#25 NAKED retry, see Matched); entry kept for history.** **`UpdateCrate`** (`src/crates/crate_update.c`, new file, GitHub
  issue #13) - a ~195-instruction per-frame state-machine dispatcher:
  throttles/re-triggers `UpdateTntCountdown`/`UpdateSlotCrate`/`SolidifyOutlineCrates` off
  `self+0x4e`'s settle-state byte, always calls `UpdateCrateFall`, then -
  gated on `self+0x4d`'s bit 7 and `self+0x38` - re-derives
  `self+0x30`'s index via the same `self+0x20`/`self+0x2d`-tag/
  0x1c-stride hitbox-record clamp `DrawCrate` (`crate_draw.c`)
  uses and settles state 6/3, or otherwise re-triggers `FinishBrokenCrate`;
  finally hands off to the `_call_via_r1` table-trampoline convention
  `CheckEntityPlayerContact`/`UpdateEntity` (`graphics.c`) establish. A plain-C
  attempt (the same register-pin-per-nested-scope technique that
  matched `DrawCrate`'s near-identical hitbox-record clamp) matched
  the first ~10 instructions but diverged once a *second* field
  address needed the same "computed once, copied to a callee-saved
  register, reused later" shape - this compiler's liveness tracking
  for register-`asm`-pinned locals produced an extra dead register
  copy the ROM never makes. Beyond that, field *addresses* thread
  through r0/r1/r6/r2/r5/r8/ip across many `bl` calls with an
  inconsistent reuse pattern (sometimes recomputed fresh a few
  instructions after an equivalent address was already live) - the
  same "which anonymous scratch register" gap as `BreakCrateTouchedByPlayer`/
  `ResolveCollisionCandidates` above, at a finer grain spread across the whole
  function rather than one isolated block. Replaces
  `asm/code_3_2_17_e560_104e4.o` in `ldscript.txt`, sitting between
  `src/crates/crate_draw.o` and `crate.o`. Filed as
  `crate_update.c`, not `collision_queue.c`, after a merge conflict with
  the concurrently-matched `AddCollisionCandidate` below, which took the
  `collision_queue.c` name first. See
  [docs/matching/issue-13-fc70-continuation.md](../matching/issue-13-fc70-continuation.md)'s
  "Update: `UpdateCrate` matched" section.
- **`AddCollisionCandidate`** (`src/objects/collision_queue.c`, new file - Phase 1 of
  the next still-unexamined chunk past issue #14's own range) - the
  physics/collision subsystem's **apply/commit step**, the call
  `QueueCratePlayerCollision` (`crate_break.c`) makes at the very end of its own
  per-edge dispatch. Appends one 0x24-byte "collision candidate" record
  to a per-entity queue at `self->candidates[self->count]` (`self` is
  the caller's own `entity+0x108` - the player's
  `gPlayer+0x108` at this specific call site - the same
  record shape `ResolveCollisionCandidates`/`collision_queue.c` already reads back, per
  its own doc comment calling `AddCollisionCandidate` its "mirror image"). A
  plain-C reconstruction reproduces the ROM's exact instruction *shape*
  (the same 8-way common-subexpression grouping for the repeated
  `self->count` index computation, sharing a computation between
  adjacent field writes in exactly the same places the ROM does) but
  gcc 2.9 -O2 picks a different scratch register for the "copy of
  `self` used to read `self->count`" step almost every time - not one
  isolated register letter to pin, so closed via NAKED transcription
  instead (no branches or literal pool in this function, so no label
  renumbering was needed). Matched, confirmed by a full clean `make
  compare` ("La suma coincide"). Phase 2's own "quick win" group later
  folded `DestroyCollisionQueue`/`ResetCollisionQueue` into this same file (both real C):
  `DestroyCollisionQueue` is byte-identical in shape to `graphics.c`'s already-
  matched `DestroyOamBuffer` (`if (arg1 & 1) OperatorDelete(arg0);`) - a VRAM-
  manager-refresh gate that happens to be called from `player_update.c`
  with `arg1 = 2` (bit 0 clear), so that particular call site is itself
  a no-op; `ResetCollisionQueue` is a trivial two-field queue reset
  (`count`/`unk4[0]`). `asm/code_3_2_17_e560_10d54.s` trimmed to begin
  at `CheckExtraLifePickup` - see
  [docs/matching/issue-14-0x08010d54-physics-apply.md](../matching/issue-14-0x08010d54-physics-apply.md)
  for the full semantic map and Phase 2 planning notes on the rest of
  the former 24-function tail. *Later pass (issue #15 NAKED retry):*
  `AddCollisionCandidate` is real C now - the two trailing byte arguments are read
  with `ldrb` from their stack words through an empty-asm-hidden address
  (the ROM's `add; add; ldrb; ldrb`), and the +0x04/+0x08 pair is a
  by-value struct copy. See
  [docs/matching/issue-15-16-naked-retry.md](../matching/issue-15-16-naked-retry.md).
- **`UpdateExtraLifeHop`-`CheckWumpaPickup`** (`src/pickups/extra_life.c`, new file
  - Phase 2's "accessor cluster" group) - 11 functions, a small
  "orbiting hazard" behavior family on a further still-unnamed "part"
  object distinct from `struct actor` and from `AddCollisionCandidate`'s own
  `struct collision_queue`: `SetExtraLifePos` seeds an orbit anchor+start
  position, `SetExtraLifeHop` (re)starts the orbit at a given mode/phase 0,
  `SetExtraLifeCounter` sets an adjacent still-unexamined byte, `UpdateExtraLifeHop` is
  the per-frame orbit-position update (two lookups into the shared sine
  table `gSineTable` at different strides, combined via the
  overflow-avoiding fixed-point multiply `FixedMul`), `CollideExtraLife`
  fires a `self->table`-driven hit trampoline once "spawned"
  (`self+0x48 == 0`) and a player flag is set, `DrawExtraLife` re-derives
  visibility from a `DrawSprite`/`self+0x38` gate, `DestroyExtraLife`/
  `InitExtraLife`/`ResetExtraLifePickup` are a small init/reset/table-repoint trio
  (same `InitSpriteObj`/table-swap shape as `moving_sprite.c`), and
  `CheckWumpaPickup` is the per-frame player-proximity/hit-resolve step
  (AABB-tests against the player, choosing primary vs. secondary AABB
  build depending on the player's own state, and on overlap tail-calls
  the despawn picker `PickUpWumpa`). All matched as real C except
  `UpdateExtraLifeHop` (NAKED transcription: gcc 2.9 persistently picks the
  opposite operand order for the shared-table pointer adds no matter how
  the C source phrases the addition - not one isolated register to pin).
  Confirmed by a full clean `make compare` ("La suma coincide").
  `asm/code_3_2_17_e560_10d54.s` further trimmed to end at
  `SendExtraLifeToHud` - see
  [docs/matching/issue-14-0x08010d54-physics-apply.md](../matching/issue-14-0x08010d54-physics-apply.md)'s
  Phase 2 findings for the full field map. *Later pass (issue #15
  NAKED retry):* `UpdateExtraLifeHop` is real C under old_agbcc (the file moved
  to `OLD_AGBCC_OBJS`); the object is now `struct orbit_part`
  (`include/orbit_part.h`). See
  [docs/matching/issue-15-16-naked-retry.md](../matching/issue-15-16-naked-retry.md).
- **`PickUpWumpa`/`UpdateWumpa`/`CreateWumpa`/`SendWumpaToHud`/`StartWumpaPayout`/
  `UpdateWumpaHop`** (`src/pickups/wumpa_update.c`, new file - Phase 2,
  a parallel slice of the same 24-function chunk) - the chunk's tail 6
  functions, contiguous through to the already-matched `wumpa.c`.
  `PickUpWumpa` is a randomized-position spawn/despawn picker;
  `UpdateWumpa` is an entity-vtable-dispatched velocity integrator
  (mode 0-3 on `self->0x48`, with a PlaySfx+`CollectWumpa`+
  collision-bitmap arrival tail and a `DropWumpa` mode-3 spawn);
  `CreateWumpa` is the achievement/unlock-icon spawn helper;
  `SendWumpaToHud` is `SendExtraLifeToHud`'s alternative; `StartWumpaPayout`/
  `UpdateWumpaHop` are a tiny mode setter and a `gSineTable`
  table helper. All but `StartWumpaPayout` (trivial, real C) closed as NAKED
  transcription - this neighborhood reconfirms the same gcc-2.9
  register-pressure hazards (r7/r8/sb) already documented at length for
  `BreakCrateTouchedByPlayer`/`QueueCratePlayerCollision`/`ResolveCollisionCandidates` and the already-NAKED
  `DropExtraLife`/`DropWumpa` wrappers. Matched, confirmed by a full
  clean `make compare`. `CheckExtraLifePickup`/`PickUpExtraLife`/`UpdateExtraLife`/
  `SendExtraLifeToHud` were also read and isolated-verified this pass but left
  un-integrated at the time - see the `extra_life.c` entry below for
  their eventual integration. *Later pass (issue #15 NAKED retry):*
  `PickUpWumpa`, `SendWumpaToHud` and `UpdateWumpaHop` are real C under
  old_agbcc (the file moved to `OLD_AGBCC_OBJS`); `UpdateWumpa` and
  `CreateWumpa` stay NAKED (`CreateWumpa` with a C draft under
  `NON_MATCHING`). See
  [docs/matching/issue-15-16-naked-retry.md](../matching/issue-15-16-naked-retry.md).
  The third big NAKED retry brought the `UpdateWumpa` draft to the
  ROM's size, 21 halfwords off (register allocation) - see
  [big-naked-retry-3.md](../matching/big-naked-retry-3.md).
  *gap4 pass:* `CreateWumpa` is real C (old_agbcc). `self` is pinned to r4
  up to the list join, `phase` is an opaqued 0, and the tag is stored
  through a pointer - see
  [gap4-naked-retry.md](../matching/gap4-naked-retry.md).
  *Mix NAKED retry 5:* `UpdateWumpa` is real C (old_agbcc) - two
  no-code references on each velocity, the spawn argument's address
  taken by an `asm`, and locals for the state-3 tail; see
  [mix-naked-retry-5.md](../matching/mix-naked-retry-5.md).
- **`CheckExtraLifePickup`/`PickUpExtraLife`/`UpdateExtraLife`/`CreateExtraLife`/
  `SendExtraLifeToHud`** (`src/pickups/extra_life.c`, new file - Phase 2
  mop-up, the chunk's final slice) - the last 5 functions of the former
  24-function tail, closing the entire `AddCollisionCandidate` chunk (issue
  #12/#14). `CheckExtraLifePickup` (real C) is a bounds-checked AABB gate that
  calls `PickUpExtraLife(self, 0)` on overlap, sharing its opening gate and
  bit-test idiom verbatim with `CheckWumpaPickup` (`extra_life.c`).
  `PickUpExtraLife`/`SendExtraLifeToHud` (both real C) are two more members of the
  "randomized/fixed `(dx,dy)` offset, `PlaySfx`, `WorldToScreen`,
  `-FixedDiv(...)` distance-pair" tail shape already documented for
  `PickUpWumpa`/`SendWumpaToHud` (`wumpa_update.c`) - unlike those two,
  both closed as real C this time, needing a handful of register-pinned/
  opaque-materialization fixes (forced constant-first evaluation order
  for `self->0xc |= mask` and `self->0x25 = 1`, and a two-register
  `r0`/`r1` pin to reproduce the ROM's own "shift into a different
  register, reuse the freed one" idiom for the `self->0x4a` offset
  nudge). `UpdateExtraLife` (392B, NAKED) is the mode-dispatched rotating/
  orbiting hazard state machine `docs/rom_map.md` already flagged,
  duplicating its own "PlaySfx+`AddLife`+collision-bitmap" tail per
  mode with different register survivors each time (same shape as
  `UpdateWumpa`). `CreateExtraLife` (164B, NAKED) is the part-object spawn
  helper extern-declared in `game_loop29.c`, needing the confirmed
  `r8`-sentinel-spill dance already documented for `CreateWumpa`/
  `UpdateWumpa`. Matched, confirmed by a full clean `make compare`
  ("La suma coincide"). `asm/code_3_2_17_e560_10d54.s` is now fully
  consumed and retired from `ldscript.txt` entirely - see
  [docs/matching/issue-14-0x08010d54-physics-apply.md](../matching/issue-14-0x08010d54-physics-apply.md)
  for the full write-up. **This closes the entire `0x08010D54`
  physics/collision-apply chunk (GitHub issue #12/#14).** *Later pass
  (issue #15 NAKED retry):* `CreateExtraLife` is real C under old_agbcc (the
  file moved to `OLD_AGBCC_OBJS`); `UpdateExtraLife` stayed NAKED with a C
  draft under `NON_MATCHING` until the third near-miss sweep made it
  real C ([docs/matching/near-miss-polish-3.md](../matching/near-miss-polish-3.md)). See
  [docs/matching/issue-15-16-naked-retry.md](../matching/issue-15-16-naked-retry.md).
