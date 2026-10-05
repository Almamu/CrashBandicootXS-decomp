# Status: graphics

`src/graphics/` (core rendering only) - OAM/sprite rendering, screen
fades, palette blending, per-actor animation frames, text layout. The
per-instance actor object family, the HUD, pause/options overlay UI,
and asset-loading/trigger-effect code also live under `src/graphics/`
on disk but are tracked in their own category pages - see
[actor.md](./actor.md), [hud.md](./hud.md), [overlay_ui.md](./overlay_ui.md),
and [graphics_loading.md](./graphics_loading.md).

## Matched

- `src/gfx/graphics.c`: `AllocVramDmaQueue`, `QueueVramDmaTransfer`,
  `FreeVramDmaQueue`, `FlushVramDmaQueue`, `InitOamBuffer`, `DestroyOamBuffer`,
  `AddOamEntry`, `CommitOamBuffer`, `RewindOamBuffer`, `MarkOamBufferBase`, `ResetOamBuffer`,
  `HideUnusedOamEntries`, `AppendOamEntries`, `SetOamAffineScales`, `GetCompletionPercent`,
  `RewindObjVram`, `MarkObjVram`, `GetObjVramFreeBytes`, `GetObjVramTile`, `ResetObjVram`,
  `ReserveObjVram`, `UploadObjVram`, `DestroyObjVramCursor`, `InitObjVramCursor`, `LoadPaletteSlot`,
  `BindPaletteSlot`, `ClaimPaletteSlot`, `UnlockPalette`, `LockPalette`, `UploadPaletteSlot`,
  `UploadPaletteCache`, `GetPaletteSlot`, `FreePaletteSlot`, `FreeUnlockedPaletteSlots`, `SetPaletteCacheSource`,
  `ClearPaletteCache`, `DestroyPaletteCache`, `InitPaletteCache`, `DestroySpriteBankSet`, `nullsub_1`,
  `IsEntityNearCamera`, `CheckEntityPlayerContact`, `DrawEntity`, `UpdateEntity`, `GetEntityBounds`,
  `SetEntitySize`, `EntityOverlapsRect`, `IsEntityOnScreen`, `IsEntityInsideRect`,
  `WorldToScreen`, `WorldPosToScreen`, `nullsub_12`, `CreateEntity`,
  `GetEntityClassId`, `ResetEntity`, `InitEntity`, `ClearEntityAlwaysActive`,
  `SetEntityAlwaysActive`, `IsEntityAlwaysActive`, `ClearEntityTouched`, `SetEntityTouched`,
  `IsEntityTouched`, `IsEntityGone`, `ClearEntityGone`, `MarkEntityGone`,
  `IsEntityContactEnabled`, `DisableEntityContact`, `EnableEntityContact`, `GetEntityFlag1`,
  `ClearEntityFlag1`, `SetEntityFlag1`, `GetEntityPixelY`, `GetEntityPixelX`,
  `GetEntityY`, `GetEntityX`, `SetEntityPixelPos`, `SetEntityPixelPosVec`,
  `SetEntityPos`, `SetEntityPosVec`, `SetEntityKind`, `GetEntityKind`,
  `GetEntityId`, `DestroyEntity`
- `src/menus/power_dialog_draw.c`: `AnimatePowerDialog`, `CommitPowerDialogFrame`, `DestroyPowerDialog`,
  `ShowTurboRunDialog`, `ShowTornadoSpinDialog`, `ShowDoubleJumpDialog`, `ShowSuperBodySlamDialog`, `GetProgressLives`,
  `CountPlatinumRelics`, `CountGoldRelics`, `CountSapphireRelics`, `CountRelics`, `CountGems`,
  `CountClearGems`, `CountCrystals`
- `src/gfx/fade.c`: `StepBrightnessFade`, `FadeBrightness`
- `src/gfx/fade.c`: `DarkenPalette`
- `src/graphics/actor_anim.c`: `GetAnimFrameBaseOffset`
- `src/gfx/fade_to_black.c` (new file - `FadePaletteToBlack`,
  `IsBrightnessFadeActive`) and
  `src/gfx/display.c` (new file - `SetDispcntMode`,
  `HideBg3`,
  `HideBg2`, `HideBg1`, `HideBg0`, `HideObj`,
  `ShowBg3`, `ShowBg2`, `ShowBg1`, `ShowBg0`,
  `ShowObj`, `SetObjMapping2D`, `SetObjMapping1D`, `CommitDispcnt`): the
  fade/screen-mode utility cluster documented in `docs/rom_map.md` - see
  `docs/matching.md`. `SetDispcntMode` was previously NAKED, now matched
  as real C via an inline-asm-materialized mask constant opaque to the
  compiler's value-propagation fold - see
  [naked-SetDispcntMode-matched.md](../matching/naked-SetDispcntMode-matched.md).
  `FadePaletteToBlack` (also in this cluster) was previously NAKED, now
  matched as real C via register-pinned locals matching the ROM's own
  register roles plus inline-asm-materialized DMA-field writes for the
  fields the ROM recomputes fresh every loop iteration - see
  [naked-sub_80014a4-matched.md](../matching/naked-sub_80014a4-matched.md).
- `src/text/wrapped_text.c`: `DrawWrappedText` (word-wrap text
  renderer) - was NAKED, now matched as real C under old_agbcc (the
  object joined `OLD_AGBCC_OBJS`); see
  [strag3-naked-retry.md](../matching/strag3-naked-retry.md).

- `src/util/aabb.c` (new file): `CommitBlendRegs` (BLDCNT/
  BLDALPHA/BLDY shadow commit - was previously NAKED, now matched as
  real C via an inline-asm-materialized store-and-increment pair
  opaque to the peephole fusion that otherwise always combines it into
  a `stmia` writeback, plus the ROM's own shift-based mask idiom - see
  [naked-CommitBlendRegs-matched.md](../matching/naked-CommitBlendRegs-matched.md)),
  `AabbOverlapsInclusiveX`, `AabbOverlaps`, `IwramFree`, `IwramAlloc` - two AABB
  overlap tests (one already referenced by name from `actor.md`'s
  `player_update.c`) plus `mem_free`/`mem_alloc` wrappers.

- `src/gfx/bitmap_screen.c` (new file, replacing `asm/code_3_1.s` -
  boot-adjacent but not part of `src/system/boot.c` since
  `main.c`/`memory.c`/`irq.c` sit between them in ROM order):
  `ShowBitmapScreen` - BG2 affine setup for a full-screen intro image; see
  `docs/matching.md` for the statement-ordering gotchas.

- `src/player/swim_ctrl.c` (new file - GitHub issue #20, plus
  issue #19's last raw function `CheckPlayerCtrlTurn`): `CheckPlayerCtrlTurn`-
  `SetPlayerCtrlMotionXPending` (26 functions), all real C - the player-input controller
  class (method table `gPlayerCtrlVtable`, struct in
  `include/player_ctrl.h`): per-frame update `UpdatePlayerCtrl` (D-pad
  auto-repeat level stepping, animation re-apply, pointer-to-member state
  dispatch through `gPlayerCtrlStateFuncs` to the eight state handlers
  `PlayerCtrlStateIdle`...`PlayerCtrlStateDead`), message handler `PlayerCtrlHandleEvent`, mode/
  animation setter `SetPlayerCtrlState`, player record writer `SetPlayerSwimDriftX`,
  constructor/destructor `InitPlayerCtrl`/`DestroyPlayerCtrl`. Nine UNUSED
  (`ApplyPlayerCtrlMotion`, `StartPlayerCtrlMotionYFromSet`, `StartPlayerCtrlMotionXFromSet`, `sub_8017330`,
  `ApplyPlayerCtrlTilt`, `StartPlayerCtrlSwim`, `sub_801750C`, `SetPlayerCtrlMotionYPending`,
  `SetPlayerCtrlMotionXPending`). Built with `tools/agbcc/bin/old_agbcc`; register pins
  only in `ApplyPlayerCtrlMotion`. See
  [docs/matching/issue-20-player-ctrl.md](../matching/issue-20-player-ctrl.md).

- `src/player/input_ctrl.c` (new file - GitHub issue #21):
  `ClearPlayerCtrlMotionYPending`-`IsInputCtrlMotionYPending` (25 functions) - six byte accessors, then a
  D-pad-driven actor-part subclass (method table `gInputCtrlVtable`):
  per-frame animation/speed selection from the held keys, a gcc 2.x
  pointer-to-member state dispatch (`gInputCtrlStateFuncs`), and the
  inlined "mark actor gone" bitmap sequence matched without inline asm.
  Built with old_agbcc since a later pass, which dropped all of its
  register pins and barriers.
  See [docs/matching/issue-21-input-ctrl.md](../matching/issue-21-input-ctrl.md).

- `src/graphics/actor_part_188d0.c` (new file - GitHub issue #23):
  all 25 functions in `CreateOneShotAnimCtrl`-`CreateCortexShotCtrl` as real C -
  method-table ("vtable" at `self+0xc`) constructor/destructor pairs
  (`CreateUnusedOneShotAnimCtrl` is UNUSED; `CreateCortexBossPlatformMover` base-constructs through
  `CreatePlatformMover`), a part-gone bitmap setter, the squares-table
  constructor `CreateTiny`, the two-part effect (state machine
  `UpdateCortexBoss`, child spawners `SpawnCortexCannon`/`SpawnCortexTarget`), the "mover"
  object (`SpawnCortexBossGem` spawner, `UpdateCortexTarget` per-frame update,
  `SetCortexTargetState` state setter, `FireCortexShot` hit-effect spawner), the part
  hit test `UpdateCortexShot`, and two more per-frame methods (`UpdateCortexBossPlatformMover`,
  `UpdateCortexBossGem`). Built with `tools/agbcc/bin/old_agbcc`, under which
  `UpdateCortexBoss` and `CreateCortexBossPlatformMover` (NAKED under agbcc) closed. See
  [docs/matching/issue-23-graphics.md](../matching/issue-23-graphics.md)
  and [docs/matching/old-agbcc-retry.md](../matching/old-agbcc-retry.md).
- `src/menus/level_select.c` (new file - GitHub issue #26):
  `sub_801B85C`-`GetCameraLeadOffset` (the player-follow child `InputCtrlStateStart`
  spawns), `SpawnLaunchPad`-`InitLaunchPad` (a 0x78-byte sprite subclass),
  `RunLevelSelect` (the modal level-select screen), `DestroyLevelSelect`,
  `UpdateLevelSelect`, `UpdateLevelSelectPageArrows`, `DrawLevelSelectRecord`, `DrawLevelSelectTime`,
  `DrawLevelSelect`, `SettleLevelSelectPage`, `LevelSelectCursorLeft`, `LevelSelectCursorRight` (its
  destructor, per-frame update/draw, record panel and cursor moves) -
  22 of the chunk's 25 functions as plain C; the other three are parked
  below. Built with `tools/agbcc/bin/old_agbcc`. See
  [docs/matching/issue-26-level-select-menu.md](../matching/issue-26-level-select-menu.md).
- `src/menus/level_select_pages.c` (new file - GitHub issue #27, shared
  structs in `include/level_menu.h`): all 25 functions of
  `LevelSelectTurnPage`-`InitZoomBg` as plain C - the rest of the level-select
  screen: the page-turn animation `LevelSelectTurnPage` and its Down/Up handlers
  `LevelSelectPrevWorld`/`LevelSelectNextWorld`, the A/Start exit loops `LevelSelectConfirm`/
  `LevelSelectExit`, the page-entry refresh (`PlaceLevelSelectEntries`/`LoadLevelSelectEntries`/
  `SetLevelSelectEntryBoxes`), the BG1 page strip (`GetLevelSelectPageBgScroll`-`CreateLevelSelectPageBg`) and
  the BG2 icon layer's constructor `InitZoomBg`. `CommitLevelSelectFrame` is
  UNUSED. Compiled with `old_agbcc`. See
  [docs/matching/issue-27-level-select-pages.md](../matching/issue-27-level-select-pages.md).
- `src/graphics/actor_part_1967c.c` (new file - GitHub issue #24):
  `sub_801967C`-`DestroyDingodileShieldCtrl` except the two NAKED ones below (23 of 25
  functions) - six small C++ actor-part controller classes (method
  tables `gCortexTargetVtable`/`476C`/`47D4`/`483C`/`48A4`/`490C`:
  constructors, destructors and per-frame updates), plus the
  `087E4974` boss-like state machine `UpdateDingodile`, its state-entry
  dispatcher `SetDingodileState` and the part spawners `SpawnDingodileShieldOrRocket`/
  `SpawnDingodileStalactite`. First file compiled with `tools/agbcc/bin/old_agbcc`.
  `sub_8019718` and `GetDingodileHits` are UNUSED (no caller or pointer
  anywhere in the ROM). See
  [docs/matching/issue-24-boss-actor.md](../matching/issue-24-boss-actor.md).
- GitHub issue #25 (0x0801A794-0x0801B85C, shared structs in
  `include/gobj_1a794.h`): `src/graphics/actor_part_1a794.c`
  (`CreateDingodileShieldCtrl`-`SetDingodileNextState`), `src/objects/platform_contact.c`
  (`CheckPlatformContact`) and `src/objects/platform.c`
  (`UpdatePlatform`-`ClearPlatformMoverActive`) - 23 functions: the level-object class
  (`gPlatformVtable`) and its oscillating-platform mover
  (`gPlatformMoverVtable`) - plus `src/objects/platform_create.c`
  (`CreatePlatform`, the level-object spawner, built with
  `tools/agbcc/bin/old_agbcc`; NAKED under agbcc, see
  [docs/matching/old-agbcc-retry.md](../matching/old-agbcc-retry.md)).
  `ResolvePlatformCollision` from the same range was parked, now matched (last-five
  NAKED retry, below). See
  [docs/matching/issue-25-level-objects.md](../matching/issue-25-level-objects.md).
- GitHub issues #28/#29 (0x0801DA38-0x0801E578, shared structs in
  `include/level_select_parts.h`, both files built with `old_agbcc`):
  `src/menus/level_select_widgets.c` (`DestroyZoomBg`-`DestroyLevelSelectEntry`, all
  25) - the level-select screen's zooming BG2 picture (`struct
  zoom_bg`: destructor, state machine, affine draw/commit, state
  queries, twinkle sprites) and the level entry's methods (`struct
  level_item`, method table `gLevelSelectEntryVtable`);
  `src/menus/level_select_widgets.c` (`CreateLevelSelectEntry`-`DestroyLevelSelectCursor`, all
  16) - the level entry's constructor and the cursor panel (`struct
  cursor_panel`: Bresenham glide, idle animation cycle, affine OBJ
  grow/shrink). `IsLevelSelectCursorHidden`, `IsLevelSelectCursorGrowing` and `MoveLevelSelectCursorTo` are
  UNUSED. All real C. See
  [docs/matching/issue-28-29-level-select-parts.md](../matching/issue-28-29-level-select-parts.md).
- **Near-miss polish pass:** `LevelSelectLoop` (`level_select.c`,
  level-select main loop) promoted from NAKED to real C under old_agbcc.
  See [near-miss-polish.md](../matching/near-miss-polish.md).
- **Issue #24/#26 NAKED retry:** `SpawnDingodileShark` (`actor_part_1967c.c`,
  floor-part spawner) and `InitLevelSelect` (`level_select.c`,
  level-select constructor) promoted from NAKED to real C, both under
  old_agbcc. See
  [docs/matching/issue-24-26-12-naked-retry.md](../matching/issue-24-26-12-naked-retry.md).
- **Third near-miss sweep:** `LoadLevelSelectRecord` (`level_select.c`,
  level-select record loader) promoted from NAKED to real C under
  old_agbcc: the ROM's stack-spilled second copy of the record pointer
  is a separate local that `info` copies. See
  [docs/matching/near-miss-polish-3.md](../matching/near-miss-polish-3.md).
- **Hard-register hold pass:** `UpdateDingodileShield` (`actor_part_1967c.c`,
  issue #24, the `gDingodileShieldVtable` controller's per-frame update)
  promoted from NAKED to real C under old_agbcc. r5/r6 held live across
  the box builders make global-alloc start the long-lived values at r7,
  as in the ROM; the state-0 BLDCNT accumulator is a block-scoped r5
  variable set through the constant-init asm. See
  [docs/matching/hard-register-hold-retry.md](../matching/hard-register-hold-retry.md).
- **Last-five NAKED retry:** `ResolvePlatformCollision` (`platform_collide.c`, issue
  #25, player-vs-object collision resolver) promoted from NAKED to real
  C under old_agbcc (object added to `OLD_AGBCC_OBJS`). An r8 hold gives
  `result` r8 and `self` sb; the FindLineCrossing calls pass a reassigned
  `px`; the player's position is read through a `PosPtr` inline instead
  of a `pp` pointer; the vtable call is an inline through the method's
  function pointer. See
  [docs/matching/last5-naked-retry.md](../matching/last5-naked-retry.md).

See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.

## Parked - NAKED asm transcriptions (byte-correct, not decompiled C)

- **`DrawWrappedText` is now matched as real C (old_agbcc; see docs/matching/strag3-naked-retry.md); entry kept for history.** **`DrawWrappedText`** (`src/text/wrapped_text.c`) - word-wrap text
  renderer. A full C reconstruction matched the ROM instruction-for-
  instruction except ~8 bytes from two small codegen details
  (incoming-argument spill ordering, and two loop-bound comparisons
  compiling one instruction shorter than the ROM's). Converted to NAKED.
  A much closer (99.86% instruction match) C reconstruction is kept
  in-tree under `#if NON_MATCHING` - see
  [naked-sub_8000ee4-progress.md](../matching/naked-sub_8000ee4-progress.md)
  for the full derivation and the two small residuals still open.

This is byte-exact against the ROM but is a NAKED asm transcription,
not decompiled C, so it's tracked here as parked rather than matched
(`SetDispcntMode`/`CommitBlendRegs`/`FadePaletteToBlack`, formerly also in this
list, are now matched as real C - see the Matched section above) -
see
`docs/matching/naked-transcription-parked-functions.md` for the full
derivation of each, and `docs/matching.md`'s original entries ("The
`0x080014A4`-`0x08001624` fade/screen-mode cluster" and "Parked, not
matched: `DrawWrappedText`") for the pre-NAKED gap analysis.

- **`DrawSpritePieces` is now matched as real C (split into `src/gfx/sprite_pieces.c`, old_agbcc; see docs/matching/strag1-naked-retry.md); `DrawPowerDialog` is now matched as real C too (see docs/matching/strag3-naked-retry.md); entry kept for history.** **`DrawPowerDialog`** (`src/menus/power_dialog_draw.c`) and **`DrawSpritePieces`**
  (`src/gfx/graphics.c`) - this project's original reference cases
  for the register-allocation-gap class documented above (several
  `overlay_ui`/`actor` functions elsewhere still hit the same class,
  see [overlay_ui.md](./overlay_ui.md) and [actor.md](./actor.md)).
  Both are now `NAKED` functions whose bodies are a literal
  instruction-for-instruction transcription of the ROM's own assembly
  (byte-exact, confirmed via a full clean `make compare`), rather than
  a derived C reconstruction - see
  `docs/matching/naked-oam-actor-part-batch.md`. Per project policy, a
  NAKED transcription standing in for a substantial function's
  register-allocation gap doesn't count as "matched" the way real
  decompiled C does, so both stay filed here rather than in "Matched"
  above, and `tools/report_units.py` tracks their address ranges as
  unmatched (`base_object: None`).
