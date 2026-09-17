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
  between them): `sub_8027088`, `sub_80270A8`, `sub_80270C0`,
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
  hit along the way.

See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.

## Parked (`NON_MATCHING`, not yet byte-exact)

None yet.
