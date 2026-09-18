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
  `sub_8002C84`, `sub_8002CE8`, `sub_8002CF4`, `sub_8002D0C`,
  `sub_8002D28`, `sub_8002EFC`,
  `sub_8002FCC`, `sub_8002FD4`, `sub_8002FD8`, `sub_800300C`,
  `sub_800306C`, `sub_800312C`, `sub_80031E4`, `sub_80032E8`,
  `sub_80033E8`, `sub_80034BC`, `sub_80035C0`, `sub_8003698`,
  `sub_800376C`, `sub_8003824`, `sub_80038D0`, `sub_800397C`,
  `sub_8003A60`. (`sub_8002D44`/`sub_8002E20` are NAKED transcriptions
  tracked as parked, not matched - see below.)
- `src/graphics/settings_menu2.c` (new file - the composite pause/
  options screen's BG-load helper and per-row stats gatherer/
  aggregator; see `docs/rom_map.md`'s `overlay_ui` section):
  `sub_80047F8`, `sub_8004860`, `sub_80048BC`, `sub_80048E0`
- `src/graphics/settings_menu3.c` (new file - the same screen's flag
  test, link-cancel-flag pair, six near-identical per-item wrappers,
  state jump-table dispatcher, and a final list-refresh trio):
  `sub_8004A50`, `sub_8004A64`, `sub_8004A80`, `sub_8004AA4`,
  `sub_8004ACC`, `sub_8004AFC`, `sub_8004B24`, `sub_8004B54`,
  `sub_8004B70`, `sub_8004BA0`, `sub_8004BD0`, `sub_8004C7C`
- `src/graphics/settings_menu4.c` (new file - confirm/cancel handler,
  BG0HOFS/DISPCNT save-restore, and the SIO-spinner teardown/construct
  pair): `sub_8004CB4`, `sub_8004CE8`, `sub_8004D20`, `sub_8004D4C`
- `src/graphics/settings_menu5.c` (new file - a wrap-increment/decrement
  counter pair on a settings-row sub-widget): `sub_8006084`,
  `sub_800609C`
- `src/graphics/settings_menu6.c` (new file - first of the composite
  screen's settings-row icon-widget constructors): `sub_8005A78`. See
  `docs/matching/issue-7-0x08004d74-overlay-ui.md`.
- `src/graphics/settings_menu8d.c` (new file - issue #4,
  0x08002A08-0x08002AA4): the settings-sync record's EEPROM-load-with-
  retry orchestrator, muting the music player across the transfer:
  `sub_8002A08`. See `docs/matching/issue-4-sio-settings-sync.md`.
- `src/graphics/settings_menu8e.c` (new file - issue #4,
  0x08002B44-0x08002C84): checksum compare/store, the `versionNibble`
  accessor, the EEPROM-save-with-retry orchestrator, and three per-row
  default-refresh/force-set/mark-selected helpers extending
  `struct settings_sync_record`: `sub_8002B44`, `sub_8002B70`,
  `sub_8002B94`, `sub_8002BA4`, `sub_8002C14`, `sub_8002C40`,
  `sub_8002C6C`. See `docs/matching/issue-4-sio-settings-sync.md`. (This
  file's `sub_8002AA4`, checksum validate + DMA-repair, is a NAKED
  transcription tracked as parked, not matched - see below.)
- `src/graphics/settings_menu9.c` (new file - issue #8,
  0x080060AC-0x08006124): the decimal `itoa` helper and a
  percentage-string formatter built on it: `sub_80060AC`, `sub_80060F8`.
- `src/graphics/settings_menu12.c` (new file - issue #8,
  0x08006250-0x080062A8): the composite screen's own top-level object's
  "apply BLDCNT/BLDY/DISPCNT" step: `sub_8006250`. See
  `docs/matching/issue-8-0x080060ac-overlay-ui.md`.
- `src/graphics/settings_menu13.c` (new file - issue #8,
  0x080063D8-0x08006518): the two-string dialog/message-box object
  constructor called by `sub_80062A8`: `sub_80063D8`. Matched only
  after heavy register pinning - see
  `docs/matching/issue-8-0x080060ac-overlay-ui.md`'s "Second pass"
  section for the full gotcha list, including a case where an isolated
  compile looked byte-identical but a full clean `make compare` still
  failed (the compiled function was 4 bytes short).
- `src/graphics/settings_menu15.c` (issue #7, 0x08004EC0-0x08005004):
  the composite screen's per-instance constructor (`sub_8004EC0`) and
  icon-field refresh/teardown step (`sub_8005004`); matched. See
  `docs/matching/issue-7-0x08004d74-overlay-ui.md`.
- `src/graphics/settings_menu17.c` (new file - issue #7,
  0x08005304-0x080053F4): the icon-group reveal/cycle animation plus
  the row-cursor icon's blink countdown: `sub_8005304`. See
  `docs/matching/issue-7-0x08004d74-overlay-ui.md`.
- `src/graphics/settings_menu18.c` (new file - issue #7,
  0x0800570C-0x080057E0): shows whichever `icons8c` row changed, or a
  fallback label if none did: `sub_800570C`. See
  `docs/matching/issue-7-0x08004d74-overlay-ui.md`.
- `src/graphics/settings_menu19.c` (new file - issue #7,
  0x0800599C-0x08005A78): the results sub-region constructor:
  `sub_800599C`. See `docs/matching/issue-7-0x08004d74-overlay-ui.md`.

See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.

## Parked - NAKED asm transcriptions (byte-correct, not decompiled C)

- **`sub_8002D44`**, **`sub_8002E20`** (`src/graphics/settings_menu8a2.c`)
  - the SIO send/receive pump's TX/RX drain-fill steps, both needing r7
  (and sb/r8) as genuine scratch across their fill loops. GitHub issue
  #5, see `docs/matching/issue-5-overlay-ui-sync.md`'s
  "NAKED-transcription pass" section.
- **`sub_8002AA4`** (`src/graphics/settings_menu8e.c`) - checksum
  validate/repair-via-DMA. GitHub issue #4, see
  `docs/matching/issue-4-sio-settings-sync.md`.
- **`sub_8003B40`** (SIO-handshake "connecting..." spinner dialog),
  **`sub_8003BDC`**, **`sub_8003C90`**, **`sub_8003D3C`**,
  **`sub_8003F30`** (per-row numeric display, issue #6),
  **`sub_80041BC`** (`src/graphics/settings_menu.c`, ROM
  `0x08003B40`-`0x080041BC`) - icon-manager centered-label/positioned-
  glyph draws plus a SIO-handshake spinner dialog, all built on the same
  primitive as `sub_8006600` (`docs/status/graphics.md`). Every
  load/store, branch, and call is semantically confirmed for all six;
  each hit the same class of gcc-2.9 scratch-register nondeterminism
  `sub_8006600` documents at length, so all six were converted to
  byte-verified NAKED asm transcriptions instead of plain C - byte-exact
  but not real decompiled C, so tracked here as parked, not matched.
  `sub_800450C` (this file's own init routine, ROM `0x0800450C`, right
  after `sub_80041BC`) stays fully raw/unattempted - see
  `docs/matching/issue-6-0x08003f30-overlay-ui.md`.
- **`sub_8004914`**, **`sub_80049CC`** (`src/graphics/settings_menu23.c`,
  new file, ROM `0x08004914`-`0x080049CC`, right after
  `settings_menu2.c` and before `settings_menu3.c`) - two more
  icon-manager centered-glyph/label draws, same NAKED-transcription
  class as `settings_menu.c` above.
- **`sub_8004D74`**, **`sub_8005100`**, **`sub_80053F4`**,
  **`sub_800556C`**, **`sub_80057E0`**, **`sub_80058C0`**,
  **`sub_8005AE8`**, **`sub_8005B80`**, **`sub_8005C58`**,
  **`sub_8005D44`**, **`sub_8005E5C`**, **`sub_8005EF4`**,
  **`sub_8005FBC`**, **`sub_8006124`**, **`sub_800619C`**,
  **`sub_80061E8`**, **`sub_80062A8`**, **`sub_8006518`** - all 18 are
  byte-exact via `NAKED` asm transcription (the whole function body is
  a hand-transcribed copy of the ROM's own disassembly, not real
  decompiled C), so they're tracked here as parked rather than matched
  - see `docs/matching/issue-7-0x08004d74-overlay-ui.md`/
  `docs/matching/issue-8-0x080060ac-overlay-ui.md`'s "Third pass"
  writeups for the technical detail on each.
