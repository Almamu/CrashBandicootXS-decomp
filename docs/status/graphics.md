# Status: graphics

`src/graphics/` (core rendering only) - OAM/sprite rendering, screen
fades, palette blending, per-actor animation frames, text layout. The
per-instance actor object family, the HUD, pause/options overlay UI,
and asset-loading/trigger-effect code also live under `src/graphics/`
on disk but are tracked in their own category pages - see
[actor.md](./actor.md), [hud.md](./hud.md), [overlay_ui.md](./overlay_ui.md),
and [graphics_loading.md](./graphics_loading.md).

## Matched

- `src/graphics/graphics.c`: `AllocVramDmaQueue`, `QueueVramDmaTransfer`,
  `FreeVramDmaQueue`, `FlushVramDmaQueue`, `InitOamBuffer`, `DestroyOamBuffer`,
  `AddOamEntry`, `CommitOamBuffer`, `RewindOamBuffer`, `MarkOamBufferBase`, `ResetOamBuffer`,
  `HideUnusedOamEntries`, `AppendOamEntries`, `SetOamAffineScales`, `GetCompletionPercent`,
  `RewindObjVram`, `MarkObjVram`, `GetObjVramFreeBytes`, `GetObjVramTile`, `ResetObjVram`,
  `ReserveObjVram`, `UploadObjVram`, `DestroyObjVramCursor`, `InitObjVramCursor`, `LoadPaletteSlot`,
  `BindPaletteSlot`, `ClaimPaletteSlot`, `UnlockPalette`, `LockPalette`, `UploadPaletteSlot`,
  `UploadPaletteCache`, `GetPaletteSlot`, `FreePaletteSlot`, `FreeUnlockedPaletteSlots`, `SetPaletteCacheSource`,
  `ClearPaletteCache`, `DestroyPaletteCache`, `InitPaletteCache`, `sub_8006FC8`, `nullsub_1`,
  `sub_8006FE4`, `sub_8007048`, `nullsub_11`, `sub_80070D4`, `sub_80070E8`,
  `sub_80070EC`, `sub_800710C`, `sub_8007110`, `sub_8007114`,
  `sub_8007174`, `sub_800719C`, `nullsub_12`, `sub_80071E4`,
  `sub_800722C`, `sub_8007230`, `sub_800725C`, `sub_8007278`,
  `sub_8007284`, `sub_8007290`, `sub_800729C`, `sub_80072A8`,
  `sub_80072B4`, `sub_80072C0`, `sub_80072CC`, `sub_80072D8`,
  `sub_800731C`, `sub_8007328`, `sub_8007334`, `sub_8007340`,
  `sub_800734C`, `sub_8007358`, `sub_8007364`, `sub_800736C`,
  `sub_8007374`, `sub_8007378`, `sub_800737C`, `sub_8007388`,
  `sub_8007398`, `sub_80073A0`, `sub_80073B0`, `sub_80073B4`,
  `sub_80073B8`, `sub_80073BC`
- `src/graphics/oam_count.c`: `AnimatePowerDialog`, `CommitPowerDialogFrame`, `DestroyPowerDialog`,
  `ShowTurboRunDialog`, `ShowTornadoSpinDialog`, `ShowDoubleJumpDialog`, `ShowSuperBodySlamDialog`, `GetProgressLives`,
  `CountPlatinumRelics`, `CountGoldRelics`, `CountSapphireRelics`, `CountRelics`, `CountGems`,
  `CountClearGems`, `CountCrystals`
- `src/graphics/fade_util.c`: `StepBrightnessFade`, `FadeBrightness`
- `src/graphics/palette_blend.c`: `DarkenPalette`
- `src/graphics/actor_anim.c`: `GetAnimFrameBaseOffset`
- `src/graphics/fade_screen_mode.c` (new file - `FadePaletteToBlack`,
  `IsBrightnessFadeActive`) and
  `src/graphics/fade_screen_mode2.c` (new file - `SetDispcntMode`,
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
- `src/graphics/text_layout.c`: `sub_8000EE4` (word-wrap text
  renderer) - was NAKED, now matched as real C under old_agbcc (the
  object joined `OLD_AGBCC_OBJS`); see
  [strag3-naked-retry.md](../matching/strag3-naked-retry.md).

- `src/graphics/aabb_util.c` (new file): `CommitBlendRegs` (BLDCNT/
  BLDALPHA/BLDY shadow commit - was previously NAKED, now matched as
  real C via an inline-asm-materialized store-and-increment pair
  opaque to the peephole fusion that otherwise always combines it into
  a `stmia` writeback, plus the ROM's own shift-based mask idiom - see
  [naked-CommitBlendRegs-matched.md](../matching/naked-CommitBlendRegs-matched.md)),
  `sub_8001640`, `sub_8001688`, `sub_80016D0`, `sub_80016DC` - two AABB
  overlap tests (one already referenced by name from `actor.md`'s
  `actor_part15.c`) plus `mem_free`/`mem_alloc` wrappers.

- `src/graphics/intro_screen.c` (new file, replacing `asm/code_3_1.s` -
  boot-adjacent but not part of `src/system/boot_util.c` since
  `main.c`/`memory.c`/`irq.c` sit between them in ROM order):
  `ShowBitmapScreen` - BG2 affine setup for a full-screen intro image; see
  `docs/matching.md` for the statement-ordering gotchas.

- `src/graphics/actor_part_16048.c` (new file - GitHub issue #20, plus
  issue #19's last raw function `sub_8016048`): `sub_8016048`-
  `sub_801751C` (26 functions), all real C - the player-input controller
  class (method table `gPlayerCtrlVtable`, struct in
  `include/player_ctrl.h`): per-frame update `sub_8016288` (D-pad
  auto-repeat level stepping, animation re-apply, pointer-to-member state
  dispatch through `gStaticData_0816C250` to the eight state handlers
  `sub_8016B1C`...`sub_8017184`), message handler `sub_8016128`, mode/
  animation setter `sub_8017264`, player record writer `sub_80172D0`,
  constructor/destructor `sub_80174EC`/`sub_80174D8`. Nine UNUSED
  (`sub_8016AB0`, `sub_801721C`, `sub_8017240`, `sub_8017330`,
  `sub_8017348`, `sub_80174BC`, `sub_801750C`, `sub_8017514`,
  `sub_801751C`). Built with `tools/agbcc/bin/old_agbcc`; register pins
  only in `sub_8016AB0`. See
  [docs/matching/issue-20-player-ctrl.md](../matching/issue-20-player-ctrl.md).

- `src/graphics/actor_part_17524.c` (new file - GitHub issue #21):
  `sub_8017524`-`sub_8017A40` (25 functions) - six byte accessors, then a
  D-pad-driven actor-part subclass (method table `gInputCtrlVtable`):
  per-frame animation/speed selection from the held keys, a gcc 2.x
  pointer-to-member state dispatch (`gStaticData_0816C290`), and the
  inlined "mark actor gone" bitmap sequence matched without inline asm.
  Built with old_agbcc since a later pass, which dropped all of its
  register pins and barriers.
  See [docs/matching/issue-21-input-ctrl.md](../matching/issue-21-input-ctrl.md).

- `src/graphics/actor_part_188d0.c` (new file - GitHub issue #23):
  all 25 functions in `sub_80188D0`-`sub_8019660` as real C -
  method-table ("vtable" at `self+0xc`) constructor/destructor pairs
  (`sub_8018948` is UNUSED; `sub_801961C` base-constructs through
  `CreatePlatformMover`), a part-gone bitmap setter, the squares-table
  constructor `CreateTiny`, the two-part effect (state machine
  `sub_8018A30`, child spawners `sub_8018BDC`/`sub_8018CB0`), the "mover"
  object (`sub_8018D70` spawner, `sub_8018E4C` per-frame update,
  `sub_8019094` state setter, `sub_8019214` hit-effect spawner), the part
  hit test `sub_8019324`, and two more per-frame methods (`sub_8019464`,
  `sub_80194E0`). Built with `tools/agbcc/bin/old_agbcc`, under which
  `sub_8018A30` and `sub_801961C` (NAKED under agbcc) closed. See
  [docs/matching/issue-23-graphics.md](../matching/issue-23-graphics.md)
  and [docs/matching/old-agbcc-retry.md](../matching/old-agbcc-retry.md).
- `src/graphics/actor_part_1b85c.c` (new file - GitHub issue #26):
  `sub_801B85C`-`sub_801B980` (the player-follow child `sub_8017600`
  spawns), `sub_801B984`-`sub_801BAD0` (a 0x78-byte sprite subclass),
  `RunLevelSelect` (the modal level-select screen), `DestroyLevelSelect`,
  `UpdateLevelSelect`, `sub_801C2B0`, `sub_801C364`, `sub_801C3E8`,
  `sub_801C51C`, `sub_801CCF8`, `LevelSelectCursorLeft`, `LevelSelectCursorRight` (its
  destructor, per-frame update/draw, record panel and cursor moves) -
  22 of the chunk's 25 functions as plain C; the other three are parked
  below. Built with `tools/agbcc/bin/old_agbcc`. See
  [docs/matching/issue-26-level-select-menu.md](../matching/issue-26-level-select-menu.md).
- `src/graphics/actor_part_1cee0.c` (new file - GitHub issue #27, shared
  structs in `include/level_menu.h`): all 25 functions of
  `LevelSelectTurnPage`-`InitZoomBg` as plain C - the rest of the level-select
  screen: the page-turn animation `LevelSelectTurnPage` and its Down/Up handlers
  `LevelSelectPrevWorld`/`LevelSelectNextWorld`, the A/Start exit loops `LevelSelectConfirm`/
  `LevelSelectExit`, the page-entry refresh (`sub_801D5CC`/`sub_801D638`/
  `sub_801D668`), the BG1 page strip (`sub_801D77C`-`sub_801D7F8`) and
  the BG2 icon layer's constructor `InitZoomBg`. `sub_801D698` is
  UNUSED. Compiled with `old_agbcc`. See
  [docs/matching/issue-27-level-select-pages.md](../matching/issue-27-level-select-pages.md).
- `src/graphics/actor_part_1967c.c` (new file - GitHub issue #24):
  `sub_801967C`-`sub_801A780` except the two NAKED ones below (23 of 25
  functions) - six small C++ actor-part controller classes (method
  tables `gStaticData_087E4704`/`476C`/`47D4`/`483C`/`48A4`/`490C`:
  constructors, destructors and per-frame updates), plus the
  `087E4974` boss-like state machine `UpdateDingodile`, its state-entry
  dispatcher `SetDingodileState` and the part spawners `sub_8019EBC`/
  `sub_801A584`. First file compiled with `tools/agbcc/bin/old_agbcc`.
  `sub_8019718` and `sub_80197F4` are UNUSED (no caller or pointer
  anywhere in the ROM). See
  [docs/matching/issue-24-boss-actor.md](../matching/issue-24-boss-actor.md).
- GitHub issue #25 (0x0801A794-0x0801B85C, shared structs in
  `include/gobj_1a794.h`): `src/graphics/actor_part_1a794.c`
  (`sub_801A794`-`sub_801A874`), `src/graphics/actor_part_1ab34.c`
  (`CheckPlatformContact`) and `src/graphics/actor_part_1b208.c`
  (`UpdatePlatform`-`sub_801B854`) - 23 functions: the level-object class
  (`gPlatformVtable`) and its oscillating-platform mover
  (`gPlatformMoverVtable`) - plus `src/graphics/actor_part_1a878.c`
  (`CreatePlatform`, the level-object spawner, built with
  `tools/agbcc/bin/old_agbcc`; NAKED under agbcc, see
  [docs/matching/old-agbcc-retry.md](../matching/old-agbcc-retry.md)).
  `sub_801AB98` from the same range was parked, now matched (last-five
  NAKED retry, below). See
  [docs/matching/issue-25-level-objects.md](../matching/issue-25-level-objects.md).
- GitHub issues #28/#29 (0x0801DA38-0x0801E578, shared structs in
  `include/level_select_parts.h`, both files built with `old_agbcc`):
  `src/graphics/actor_part_1da38.c` (`DestroyZoomBg`-`sub_801DF98`, all
  25) - the level-select screen's zooming BG2 picture (`struct
  zoom_bg`: destructor, state machine, affine draw/commit, state
  queries, twinkle sprites) and the level entry's methods (`struct
  level_item`, method table `gStaticData_087E4BAC`);
  `src/graphics/actor_part_1dfec.c` (`sub_801DFEC`-`sub_801E524`, all
  16) - the level entry's constructor and the cursor panel (`struct
  cursor_panel`: Bresenham glide, idle animation cycle, affine OBJ
  grow/shrink). `sub_801E3D4`, `sub_801E3E4` and `sub_801E4E4` are
  UNUSED. All real C. See
  [docs/matching/issue-28-29-level-select-parts.md](../matching/issue-28-29-level-select-parts.md).
- **Near-miss polish pass:** `LevelSelectLoop` (`actor_part_1b85c.c`,
  level-select main loop) promoted from NAKED to real C under old_agbcc.
  See [near-miss-polish.md](../matching/near-miss-polish.md).
- **Issue #24/#26 NAKED retry:** `sub_801A03C` (`actor_part_1967c.c`,
  floor-part spawner) and `InitLevelSelect` (`actor_part_1b85c.c`,
  level-select constructor) promoted from NAKED to real C, both under
  old_agbcc. See
  [docs/matching/issue-24-26-12-naked-retry.md](../matching/issue-24-26-12-naked-retry.md).
- **Third near-miss sweep:** `sub_801C608` (`actor_part_1b85c.c`,
  level-select record loader) promoted from NAKED to real C under
  old_agbcc: the ROM's stack-spilled second copy of the record pointer
  is a separate local that `info` copies. See
  [docs/matching/near-miss-polish-3.md](../matching/near-miss-polish-3.md).
- **Hard-register hold pass:** `sub_801A114` (`actor_part_1967c.c`,
  issue #24, the `gStaticData_087E490C` controller's per-frame update)
  promoted from NAKED to real C under old_agbcc. r5/r6 held live across
  the box builders make global-alloc start the long-lived values at r7,
  as in the ROM; the state-0 BLDCNT accumulator is a block-scoped r5
  variable set through the constant-init asm. See
  [docs/matching/hard-register-hold-retry.md](../matching/hard-register-hold-retry.md).
- **Last-five NAKED retry:** `sub_801AB98` (`actor_part_1ab98.c`, issue
  #25, player-vs-object collision resolver) promoted from NAKED to real
  C under old_agbcc (object added to `OLD_AGBCC_OBJS`). An r8 hold gives
  `result` r8 and `self` sb; the sub_800FDC8 calls pass a reassigned
  `px`; the player's position is read through a `PosPtr` inline instead
  of a `pp` pointer; the vtable call is an inline through the method's
  function pointer. See
  [docs/matching/last5-naked-retry.md](../matching/last5-naked-retry.md).

See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.

## Parked - NAKED asm transcriptions (byte-correct, not decompiled C)

- **`sub_8000EE4` is now matched as real C (old_agbcc; see docs/matching/strag3-naked-retry.md); entry kept for history.** **`sub_8000EE4`** (`src/graphics/text_layout.c`) - word-wrap text
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
matched: `sub_8000EE4`") for the pre-NAKED gap analysis.

- **`DrawSpritePieces` is now matched as real C (split into `src/graphics/graphics_73dc.c`, old_agbcc; see docs/matching/strag1-naked-retry.md); `DrawPowerDialog` is now matched as real C too (see docs/matching/strag3-naked-retry.md); entry kept for history.** **`DrawPowerDialog`** (`src/graphics/oam_count.c`) and **`DrawSpritePieces`**
  (`src/graphics/graphics.c`) - this project's original reference cases
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
