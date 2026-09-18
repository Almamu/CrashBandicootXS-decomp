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
  `sub_8002D28`, `sub_8002EFC`, `sub_8002FCC`, `sub_8002FD4`,
  `sub_8002FD8`, `sub_800300C`, `sub_800306C`, `sub_800312C`,
  `sub_80031E4`, `sub_80032E8`, `sub_80033E8`, `sub_80034BC`,
  `sub_80035C0`, `sub_8003698`, `sub_800376C`, `sub_8003824`,
  `sub_80038D0`, `sub_800397C`, `sub_8003A60`
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
  screen's five settings-row icon-widget constructors; see
  `docs/matching.md`'s issue #7 writeup for the rest, parked in the same
  file): `sub_8005A78`
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
  `sub_8002C6C`. See `docs/matching/issue-4-sio-settings-sync.md`.

See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.

## Parked (`NON_MATCHING`, not yet byte-exact)

- **`sub_8002D44`**, **`sub_8002E20`** (`asm/code_3_1_10_3_2d44.s`, C in
  `src/graphics/settings_menu8a2.c`) - the SIO settings-sync
  send/receive pump's TX/RX drain-fill steps (`struct
  settings_sync_pump`, `include/settings_sync.h`); fully understood,
  but both genuinely need `r7` as scratch (matching the ROM's own
  `sendLen`/sentinel usage), and this exact agbcc build never includes
  `r7` in a function's automatic callee-save push/pop - confirmed by
  direct reproduction (a minimal function that only ever touches r7,
  whether via a plain asm clobber, an explicit `register T x
  asm("r7")` pin used across a real call, or a real C-level variable
  forced into r7 under heavy register pressure, never gets it back in
  the push/pop list). The third function of this trio, `sub_8002EFC`
  (which doesn't touch r7), is unaffected and has been matched - see
  `docs/matching/issue-5-overlay-ui-sync.md`, issue #5.
- **`sub_8003B40`**, **`sub_8003BDC`**, **`sub_8003C90`**,
  **`sub_8003D3C`**, **`sub_80041BC`**, **`sub_8004914`**,
  **`sub_80049CC`** (`asm/code_3_1_10_3.s`/`asm/code_3_1_10_4.s`/
  `asm/code_3_1_10_5.s`, C in `src/graphics/settings_menu.c`) - the
  composite pause/options screen's icon-manager centered-label draws
  (`sub_80049CC`/`sub_8003C90`/`sub_8003BDC`/`sub_8003D3C`/
  `sub_80041BC`/`sub_8004914`, all built on the same primitive
  `sub_8006600` in `docs/status/graphics.md` uses) plus a SIO-handshake
  spinner dialog (`sub_8003B40`). Every load/store, branch, and call is
  semantically confirmed for all seven; each hits the same class of
  gcc-2.9 scratch-register nondeterminism `sub_8006600` documents at
  length (`sub_80049CC`/`sub_8003C90` come within one or two
  register-letter choices; `sub_8003BDC` additionally spills a constant
  through `ip`, which plain C can't request) - see `docs/matching.md`,
  "Match 0x08003B40-0x08004CB4", for what was tried on each.
- **`sub_8005AE8`**, **`sub_8005B80`**, **`sub_8005C58`**,
  **`sub_8005D44`** (`asm/code_3_1_10_8.s`, C in
  `src/graphics/settings_menu6.c`) - four more settings-row icon-widget
  constructors on the composite pause/options screen (4/5/3-element
  arrays plus the medal/rank-award icon). Each one's `field_29`
  frame-index update matches byte-for-byte (shared with the matched
  `sub_8005A78` above), but the surrounding loop/branch address
  computation never lands the loop/self pointer in `r8`/`sb` the way
  the ROM's does, the same class `sub_8006600` documents - see
  `docs/matching.md`, issue #7.
- **`sub_8005EF4`**, **`sub_8005FBC`** (`asm/code_3_1_10_10.s`, C in
  `src/graphics/settings_menu7.c`) - a matched inc/dec pair for a
  per-row percentage counter, formatting a `" <NN%>"`-shaped scratch
  string and pushing it through the matching `AudioContext` setter.
  Fully understood; off by several register-letter choices in the
  digit-formatting tail, the same unresolved class `sub_80049CC` above
  documents - see `docs/matching.md`, issue #7.
- **`sub_8002AA4`** (`asm/code_3_1_10_3_2aa4.s`, C in
  `src/graphics/settings_menu8e.c`) - checksum validate + DMA-repair
  for the settings-sync record. Fully understood; the loop body and
  post-loop field writes match exactly with explicit register pins
  matching the ROM's cached field addresses, but the prologue's
  push-list still differs (this compiler doesn't protect `r7` across
  `sub_8002C6C`'s calls here, unlike the ROM). See
  `docs/matching/issue-4-sio-settings-sync.md`, issue #4.
