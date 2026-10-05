# Status: hud

The on-screen HUD - the stat-counter widget, icon-slot draws, and the
icon-blink animation timer. Filed under `src/graphics/` on disk (all
draw code), tracked as its own `hud` category since `docs/rom_map.md`
and the `decomp-chunk` issue generator both treat it as a distinct
system from "core" graphics.

## Matched

- `src/graphics/hud_counter.c` (new file, contributed via PR #1 by
  @MiryamSanchez26 - `UpdateHudLives` is an isolated HUD counter update
  inside the raw HUD stat-widget region, so preserving its ROM address
  required a split of what's now `asm/code_3_2_17.s`/`asm/code_3_2_20.s`
  right around it): `UpdateHudLives`

- `src/graphics/hud_icon_slot.c` (new file, GitHub issue #45, non-
  adjacent to `hud_counter.c` since `InitHud`-`UpdateHudClock` sit raw
  between them): `TickPaletteCycles`/`AddPaletteCycle` (the fx ring-buffer's
  per-frame consumer/producer pair - see
  `docs/matching/issue-45-hud-stat-widget-dispatcher.md`'s "Third pass"
  section for the register-pinning/instruction-ordering gotchas this
  pair needed), `ClearPaletteCycles`, `DestroyPaletteCycles`, `InitPaletteCycles`,
  `DrawHudPart`, `sub_802710C` (UNUSED - no caller anywhere in the
  ROM), `InitHudPart` - a fixed 3-entry particle/effect queue's reset/
  constructor/teardown trio, a HUD digit-slot draw helper, and two
  `struct actor`-table-swap slot constructors; see `docs/matching.md`.
  Also added `include/hud.h`, moving `hud_counter.c`'s
  `hud_anim_record`/`hud_anim_data`/`hud_digit_part`/`hud_counter`
  structs there so this file could reuse them.

- `src/graphics/hud_blink.c` (new file, GitHub issue #45):
  `UpdateHudSlides`, `ShowHudCrates`, `ShowHudLives`, `ShowHudWumpa`,
  `ShowHudCounters`, `StepHudSlide` - a 3-slot icon-blink animation timer
  (per-frame tick, three per-slot triggers, and the generic single-slot
  advance they share); see `docs/matching.md` for two codegen gotchas
  hit along the way. Extended by GitHub issue #46 with `SetHudCrateTotal`/
  `IncHudCrateTotal` - a setter/increment pair on the same central-state
  object's `+0x28` field.

- `src/graphics/hud_icon_widget.c` (new file, GitHub issue #46):
  `DestroyHud` - a `struct hud_counter`'s `parts`-array destructor.

- `src/graphics/hud_icon_widget_85c4.c` (GitHub issue #46, second pass):
  `FontPutChar` - the per-character newline/space/glyph-dispatch
  trampoline.

- `src/graphics/hud_icon_widget2.c` (new file, GitHub issue #46):
  `FontDrawChars` - draws a fixed-count run of characters via the
  `struct icon_manager` widget's own record trampoline.

- `src/graphics/hud_icon_widget_8890.c` (GitHub issue #46, second pass):
  `FontDrawText`.

- `src/graphics/hud_icon_widget3.c` (new file, GitHub issue #46):
  `FontTextHeight` - total text-block-height helper.

- `src/graphics/hud_icon_widget4.c` (new file, GitHub issue #46):
  `FontUploadTiles`, `FontSetPalette`, `FontResetPalette` - glyph-sheet VRAM
  upload and two OAM-attribute-nibble setters.

- `src/graphics/hud_icon_widget_8a78.c` (GitHub issue #46, second pass):
  `InitFont` - the "no data tables of its own" widget constructor
  variant; now fully byte-exact, so this file (unlike the other two
  above) carries no `#if NON_MATCHING` guard at all any more.

- `src/graphics/hud_icon_widget5.c` (new file, GitHub issue #46):
  `FontHeightToLines`, `FontGetTileCount`, `FontSetPos`, `FontNewLineAt`,
  `FontGetMargin`, `FontSetMargin`, `FontGetY`, `FontGetX`,
  `FontSetTileBase` - trivial `struct icon_manager` getter/setter/
  trampoline-forwarder family. Also includes `DestroyFont`
  (`0x08028B7C`, no tracked issue - just the next function in ROM
  order): a minimal `record`-pointer constructor for the same `struct
  icon_manager`, matched byte-exact on the first try with plain struct
  field access - see
  [docs/matching/naked-DestroyFont.md](../matching/naked-DestroyFont.md)
  (misnomer aside - it's plain C, not a NAKED transcription; named to
  match this repo's existing untracked-function-writeup convention).

- `src/graphics/hud_stat_widget.c` (new file, GitHub issue #45's second
  pass): `UpdateHud` - the HUD stat-widget family's dispatcher.
  Extended `struct hud_counter` (`include/hud.h`) with `field_08`,
  `icon_flag`, and `sync_value_a`/`b`/`c` - fields this function (and
  the family's other callees) touch inside what was previously opaque
  padding.

- `src/graphics/hud_stat_widget2.c`, `hud_digit_array.c`,
  `hud_stat_widget3.c` (GitHub issue #45's dispatcher family):
  `UpdateHudBoss`, `UpdateHudClock`, `InitHud`, `ConfigureHudParts`,
  `UpdateHudCrates`, `UpdateHudWumpa`, `UpdateHudPercentCounters`. All plain C built with
  old_agbcc (`hud_stat_widget2.c`/`hud_stat_widget3.c` moved to it). Six
  of them were NAKED until the NAKED retry pass. The "r7 miscompile"
  that parked them turned out to be a compiler mismatch: under
  old_agbcc the plain clamp compiles byte for byte. See
  [naked-retry-mid45.md](../matching/naked-retry-mid45.md).

All 24 functions in the `0x08026EEC`-`0x08028568` chunk are byte-exact,
and all of them are real C.

See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.
GitHub issue #46's own write-up (icon/text-widget renderer, including
`include/icon_manager.h`'s newly-documented field layout) is
[docs/matching/issue-46-hud-icon-widget.md](../matching/issue-46-hud-icon-widget.md).
GitHub issue #45's second-pass write-up (the stat-widget dispatcher, and
why the rest of the family stayed raw) is
[docs/matching/issue-45-hud-stat-widget-dispatcher.md](../matching/issue-45-hud-stat-widget-dispatcher.md).

- **`FontDrawGlyph`**, **`InitSmallFont`**, **`InitLargeFont`**
  (`src/graphics/hud_icon_widget_85c4.c`), **`FontMeasureChars`**
  (`hud_icon_widget_8890.c`) and **`FontMeasureText`** (`hud_icon_widget_8994.c`)
  - GitHub issue #46's last five, the glyph writer, the two icon-manager
  constructors and the text walkers. Plain C, built with old_agbcc (all
  three files move to it); they were NAKED. See [old-agbcc-round5.md](../matching/old-agbcc-round5.md).

## Parked (`NON_MATCHING`, not yet byte-exact)

- GitHub issue #46: none `NON_MATCHING` - all 25 functions in the chunk
  are byte-exact. Five of them are NAKED transcriptions tracked as
  parked, not matched - see below. See
  [docs/matching/issue-46-hud-icon-widget.md](../matching/issue-46-hud-icon-widget.md)
  for the full history.

### NAKED transcription (byte-exact, but not real decompiled C)

These functions produce byte-exact ROM output, but only because the
entire function body is hand-transcribed disassembly wrapped in inline
`asm()` - the C-level matching attempt failed and the raw bytes got
embedded as asm instead. They're tracked as parked, not matched.

- None left in this category.
