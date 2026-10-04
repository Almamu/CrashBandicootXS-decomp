# Status: overlay_ui

Pause menu and settings/options screens - the composite pause/options
screen's setup, per-row widgets, and the SIO-handshake spinner dialog.
Filed under `src/graphics/` on disk (all draw/BG-setup code), tracked
as its own `overlay_ui` category since `docs/rom_map.md` and the
`decomp-chunk` issue generator both treat it as a distinct system from
"core" graphics.

## Matched

- `src/graphics/settings_menu8.c`/`settings_menu8a2.c`/`settings_menu8a3.c`/
  `settings_menu8b.c`/`settings_menu8c.c` (`settings_menu8a3.c` new in the
  second pass - issue #5, 0x08002C84-0x08003B40): the settings-sync
  record's init/flag/checksum accessors (`struct settings_sync_record`,
  `include/settings_sync.h`), the SIO send/receive pump's handle
  accessors and per-frame poll step, the spinner dialog's blocking
  modal loop and shared field_8c/field_90 constructor/destructor
  (also used by the composite screen itself), the per-frame input
  dispatcher, all 13 of its per-state handlers, the shared "commit or
  refresh row" step, and the state-select label list draw; matched. See
  `docs/matching/issue-5-overlay-ui-sync.md` for the full write-up:
  `ResetSaveData`, `IsSaveSlotEmpty`, `TestSaveFlags`, `ClearSaveFlags`,
  `SetSaveFlags`, `sub_8002EFC`,
  `sub_8002FCC`, `sub_8002FD4`, `sub_8002FD8`, `RunSaveMenu`,
  `InitSaveMenu`, `DestroySaveMenu`, `SaveMenuInput`, `SaveMenuMainInput`,
  `SaveMenuMoveCursor`, `SaveMenuLoadInput`, `SaveMenuLinkInput`, `SaveGameToSlot`,
  `SaveMenuOverwriteInput`, `SaveMenuSaveInput`, `SaveMenuDeleteInput`, `SaveMenuConfirmDeleteInput`,
  `DrawSaveMenuMain`, and `sub_8002E20` (was NAKED, see below).
- `src/graphics/settings_menu2.c` (new file - the composite pause/
  options screen's BG-load helper and per-row stats gatherer/
  aggregator; see `docs/rom_map.md`'s `overlay_ui` section):
  `sub_80047F8`, `RefreshSaveSlotSummaries`, `LoadSaveMenuData`, `SummarizeProgress`
- `src/graphics/settings_menu3.c` (new file - the same screen's flag
  test, link-cancel-flag pair, six near-identical per-item wrappers,
  state jump-table dispatcher, and a final list-refresh trio):
  `sub_8004A50`, `sub_8004A64`, `sub_8004A80`, `DrawSaveMenuConfirmDelete`,
  `DrawSaveMenuDelete`, `DrawSaveMenuOverwrite`, `DrawSaveMenuSave`, `DrawSaveMenuMessage`,
  `DrawSaveMenuLoadLink`, `DrawSaveMenuLoad`, `DrawSaveMenu`, `DeleteSaveSlot`
- `src/graphics/settings_menu4.c` (new file - confirm/cancel handler,
  BG0HOFS/DISPCNT save-restore, and the SIO-spinner teardown/construct
  pair): `SaveMenuMessageInput`, `CommitSaveMenuFrame`, `CloseSaveMenu`, `OpenSaveMenu`
- `src/graphics/settings_menu5.c` (new file - a wrap-increment/decrement
  counter pair on a settings-row sub-widget): `PauseMenuCursorDown`,
  `PauseMenuCursorUp`
- `src/graphics/settings_menu6.c` (new file - first of the composite
  screen's settings-row icon-widget constructors): `InitPauseCrystalsPage`, plus
  (issue #7 retry, old_agbcc) `InitPausePowersPage`, `InitPauseGemsPage`,
  `InitPauseRelicsPage`, `InitPauseTimeTrialPage`. See
  `docs/matching/issue-7-0x08004d74-overlay-ui.md` and
  `docs/matching/issue-7-naked-retry.md`.
- `src/graphics/settings_menu7.c` (issue #7 retry): the per-row
  percentage dec/inc pair `PauseMenuVolumeDown`, `PauseMenuVolumeUp`.
- `src/graphics/settings_menu21.c` (issue #7 retry): the per-row list
  renderer `DrawPauseMenuRows`, and (hard-register hold pass) the per-frame
  row draw step `DrawPauseMenu` - plain C, was NAKED. See
  [hard-register-hold-retry.md](../matching/hard-register-hold-retry.md).
- `src/graphics/settings_menu22.c` (issue #7 retry, old_agbcc): the
  icon-row fraction readouts `DrawPauseGemsPage`, `DrawPauseRelicsPage`.
- `src/graphics/settings_menu8d.c` (new file - issue #4,
  0x08002A08-0x08002AA4): the settings-sync record's EEPROM-load-with-
  retry orchestrator, muting the music player across the transfer:
  `LoadSaveData`. See `docs/matching/issue-4-sio-settings-sync.md`.
- `src/graphics/settings_menu8e.c` (new file - issue #4,
  0x08002B44-0x08002C84): checksum compare/store, the `versionNibble`
  accessor, the EEPROM-save-with-retry orchestrator, and three per-row
  default-refresh/force-set/mark-selected helpers extending
  `struct settings_sync_record`: `CheckSaveChecksum`, `UpdateSaveChecksum`,
  `sub_8002B94`, `StoreSaveData`, `ReadSaveSlot`, `WriteSaveSlot`,
  `EraseSaveSlot`. See `docs/matching/issue-4-sio-settings-sync.md`. The
  file's `ValidateSaveData` (checksum validate + DMA-repair) is real C too
  since the near-miss polish pass - see
  [near-miss-polish.md](../matching/near-miss-polish.md).
- `src/graphics/settings_menu9.c` (new file - issue #8,
  0x080060AC-0x08006124): the decimal `itoa` helper and a
  percentage-string formatter built on it: `FormatDecimal`, `FormatVolumePercent`.
- `src/graphics/settings_menu12.c` (new file - issue #8,
  0x08006250-0x080062A8): the composite screen's own top-level object's
  "apply BLDCNT/BLDY/DISPCNT" step: `CommitPauseMenuFrame`. See
  `docs/matching/issue-8-0x080060ac-overlay-ui.md`.
- `src/graphics/settings_menu13.c` (new file - issue #8,
  0x080063D8-0x08006518): the two-string dialog/message-box object
  constructor called by `ShowPowerDialog`: `InitPowerDialog`. Matched only
  after heavy register pinning - see
  `docs/matching/issue-8-0x080060ac-overlay-ui.md`'s "Second pass"
  section for the full gotcha list, including a case where an isolated
  compile looked byte-identical but a full clean `make compare` still
  failed (the compiled function was 4 bytes short).
- `src/graphics/settings_menu15.c` (issue #7, 0x08004EC0-0x08005004):
  the composite screen's per-instance constructor (`InitPauseMenu`) and
  icon-field refresh/teardown step (`DestroyPauseMenu`); matched. See
  `docs/matching/issue-7-0x08004d74-overlay-ui.md`.
- `src/graphics/settings_menu17.c` (new file - issue #7,
  0x08005304-0x080053F4): the icon-group reveal/cycle animation plus
  the row-cursor icon's blink countdown: `AnimatePauseMenu`. See
  `docs/matching/issue-7-0x08004d74-overlay-ui.md`.
- `src/graphics/settings_menu18.c` (new file - issue #7,
  0x0800570C-0x080057E0): shows whichever `icons8c` row changed, or a
  fallback label if none did: `DrawPausePowersPage`. See
  `docs/matching/issue-7-0x08004d74-overlay-ui.md`.
- `src/graphics/settings_menu19.c` (new file - issue #7,
  0x0800599C-0x08005A78): the results sub-region constructor:
  `InitPauseMenuInfo`. See `docs/matching/issue-7-0x08004d74-overlay-ui.md`.
- `src/graphics/settings_menu.c` (issue #6 retry, old_agbcc): the
  "connecting..." spinner dialog `sub_8003B40`, the centered-label draws
  `sub_8003BDC`/`sub_8003C90`, the per-row stat renderer `sub_8003F30`
  and its 4-row driver `sub_80041BC` - all plain C, were NAKED. See
  [issue-4-6-8-naked-retry.md](../matching/issue-4-6-8-naked-retry.md).
  The value-label/pair draw `sub_8003D3C` followed in the early-ROM
  NAKED retry ([early-rom-naked-retry.md](../matching/early-rom-naked-retry.md)),
  and the screen's init routine `sub_800450C` (was raw asm in the now
  retired `asm/code_3_1_10_4.s`) in the hard-register hold pass
  ([hard-register-hold-retry.md](../matching/hard-register-hold-retry.md)).
- `src/graphics/settings_menu23.c` (issue #6 retry): `sub_8004914`,
  `sub_80049CC` - plain C, were NAKED.
- `src/graphics/settings_menu10.c` (issue #8 retry, old_agbcc):
  `PowerDialogLoop`; `settings_menu11.c`: `DrawPauseTimeTrialPage`, `DrawPauseCrystalsPage`,
  `DrawPauseMenuPageTitle`; `settings_menu14.c`: `ShowPowerDialog` - plain C, were
  NAKED.
- `src/graphics/settings_menu15.c` (second near-miss sweep):
  `RunPauseMenu`, the composite pause/options screen driver - plain C,
  was NAKED. See [near-miss-polish-2.md](../matching/near-miss-polish-2.md).
- `src/graphics/settings_menu20.c` (early-ROM NAKED retry 2, old_agbcc):
  `PauseMenuLoop`, the blocking cursor/confirm/cancel driver - plain C,
  was NAKED. See [early-rom-naked-retry-2.md](../matching/early-rom-naked-retry-2.md).

See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.

- **`sub_8002D44`** (`src/graphics/settings_menu8a2.c`) - the SIO pump's
  TX fill step (issue #5). Plain C; it was NAKED. The old "r7 can never be
  pushed" note was wrong - plain C gives the r7/r8/sb prologue. See [old-agbcc-round5.md](../matching/old-agbcc-round5.md).
- **`sub_8002E20`** (`src/graphics/settings_menu8a2.c`) - the SIO pump's
  RX drain step (issue #5). Plain C; it was NAKED. The channel pointer
  comes out of an asm with a plain `"r"` input (no copy preference for
  r2) and the wrap loop's count pointer is pinned to r1. See
  [last-eight-naked-retry.md](../matching/last-eight-naked-retry.md).
- **`DrawPauseFraction`** (`src/graphics/settings_menu16.c`) - the results
  icons' "N/M" fraction readout draw (issue #7). Plain C; it was NAKED.
  Plain posX/posY accesses let reload build the 0x110/0x114 offsets in
  r7 as the ROM does; the second reposition reads through inline
  getters. See [last-ten-naked-retry.md](../matching/last-ten-naked-retry.md).

## Parked - NAKED asm transcriptions (byte-correct, not decompiled C)

- No functions are parked here now. `DrawPauseFraction` became real C in
  the last-ten retry - see Matched.
  `DrawPauseMenu` and the raw `sub_800450C` are now real C (hard-register
  hold pass). Eleven former members of
  this list (`RunPauseMenu`, `DrawPauseMenuRows`, `DrawPauseGemsPage`, `DrawPauseRelicsPage`,
  `InitPausePowersPage`, `InitPauseGemsPage`, `InitPauseRelicsPage`, `InitPauseTimeTrialPage`,
  `PauseMenuVolumeDown`, `PauseMenuVolumeUp`, and since the early-ROM NAKED retry 2
  `PauseMenuLoop`) are now real C, and so are issue #8's
  `DrawPauseTimeTrialPage`, `DrawPauseCrystalsPage`, `DrawPauseMenuPageTitle`, `ShowPowerDialog`,
  `PowerDialogLoop` - see Matched.
