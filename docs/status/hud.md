# Status: hud

The on-screen HUD - the stat-counter widget, icon-slot draws, and the
icon-blink animation timer. Filed under `src/graphics/` on disk (all
draw code), tracked as its own `hud` category since `docs/rom_map.md`
and the `decomp-chunk` issue generator both treat it as a distinct
system from "core" graphics.

## Matched

- `src/graphics/hud_counter.c` (new file, contributed via PR #1 by
  @MiryamSanchez26 - `sub_8027838` is an isolated HUD counter update
  inside the raw HUD stat-widget region, so preserving its ROM address
  required a split of what's now `asm/code_3_2_17.s`/`asm/code_3_2_20.s`
  right around it): `sub_8027838`

- `src/graphics/hud_icon_slot.c` (new file, GitHub issue #45, non-
  adjacent to `hud_counter.c` since `sub_8027138`-`sub_802763C` sit raw
  between them): `sub_8026F54`/`sub_8027018` (the fx ring-buffer's
  per-frame consumer/producer pair - see
  `docs/matching/issue-45-hud-stat-widget-dispatcher.md`'s "Third pass"
  section for the register-pinning/instruction-ordering gotchas this
  pair needed), `sub_8027088`, `sub_80270A8`, `sub_80270C0`,
  `sub_80270E0`, `sub_802710C` (UNUSED - no caller anywhere in the
  ROM), `sub_8027120` - a fixed 3-entry particle/effect queue's reset/
  constructor/teardown trio, a HUD digit-slot draw helper, and two
  `struct actor`-table-swap slot constructors; see `docs/matching.md`.
  Also added `include/hud.h`, moving `hud_counter.c`'s
  `hud_anim_record`/`hud_anim_data`/`hud_digit_part`/`hud_counter`
  structs there so this file could reuse them.

- `src/graphics/hud_blink.c` (new file, GitHub issue #45):
  `sub_8028400`, `sub_8028474`, `sub_80284A4`, `sub_80284D4`,
  `sub_8028504`, `sub_8028520` - a 3-slot icon-blink animation timer
  (per-frame tick, three per-slot triggers, and the generic single-slot
  advance they share); see `docs/matching.md` for two codegen gotchas
  hit along the way. Extended by GitHub issue #46 with `sub_8028568`/
  `sub_802856C` - a setter/increment pair on the same central-state
  object's `+0x28` field.

- `src/graphics/hud_icon_widget.c` (new file, GitHub issue #46):
  `sub_8028574` - a `struct hud_counter`'s `parts`-array destructor.

- `src/graphics/hud_icon_widget2.c` (new file, GitHub issue #46):
  `sub_8028860` - draws a fixed-count run of characters via the
  `struct icon_manager` widget's own record trampoline.

- `src/graphics/hud_icon_widget3.c` (new file, GitHub issue #46):
  `sub_8028968` - total text-block-height helper.

- `src/graphics/hud_icon_widget4.c` (new file, GitHub issue #46):
  `UploadHudTile`, `sub_8028A30`, `sub_8028A40` - glyph-sheet VRAM
  upload and two OAM-attribute-nibble setters.

- `src/graphics/hud_icon_widget5.c` (new file, GitHub issue #46):
  `sub_8028AC4`, `sub_8028ADC`, `sub_8028AE8`, `sub_8028B04`,
  `sub_8028B28`, `sub_8028B34`, `sub_8028B40`, `sub_8028B4C`,
  `sub_8028B58` - trivial `struct icon_manager` getter/setter/
  trampoline-forwarder family.

- `src/graphics/hud_stat_widget.c` (new file, GitHub issue #45's second
  pass): `sub_80274EC` - the HUD stat-widget family's dispatcher.
  Non-adjacent to `hud_counter.o`/other `hud` files since the rest of
  the family (`sub_8027138`/`sub_802732C`/`sub_802757C`/`sub_802763C`/
  `sub_8027940`/`sub_8027D5C`/`sub_8027E88`) still sits raw around it.
  Extended `struct hud_counter` (`include/hud.h`) with `field_08`,
  `icon_flag`, and `sync_value_a`/`b`/`c` - fields this function (and
  the still-raw callees around it) touch inside what was previously
  opaque padding.

See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.
GitHub issue #46's own write-up (icon/text-widget renderer, including
`include/icon_manager.h`'s newly-documented field layout) is
[docs/matching/issue-46-hud-icon-widget.md](../matching/issue-46-hud-icon-widget.md).
GitHub issue #45's second-pass write-up (the stat-widget dispatcher, and
why the rest of the family stayed raw) is
[docs/matching/issue-45-hud-stat-widget-dispatcher.md](../matching/issue-45-hud-stat-widget-dispatcher.md).

## Parked (`NON_MATCHING`, not yet byte-exact)

- GitHub issue #46 (see
  [docs/matching/issue-46-hud-icon-widget.md](../matching/issue-46-hud-icon-widget.md)
  for what was tried on each): `sub_80285C4`, `InitHudIconWidgetA`,
  `InitHudIconWidgetB`, `sub_8028808` (real bytes in
  `asm/code_3_2_20_85c4.s`, reconstructions in
  `src/graphics/hud_icon_widget_85c4.c`); `sub_8028890`, `sub_8028900`
  (real bytes in `asm/code_3_2_20_8890.s`, reconstructions in
  `src/graphics/hud_icon_widget_8890.c`); `MeasureText` (real bytes in
  `asm/code_3_2_20_8994.s`, reconstruction in
  `src/graphics/hud_icon_widget_8994.c`); `sub_8028A78` (real bytes in
  `asm/code_3_2_20_8a78.s`, reconstruction in
  `src/graphics/hud_icon_widget_8a78.c`).
