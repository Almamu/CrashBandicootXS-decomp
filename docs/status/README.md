# Status

Per-category matching status, one page per category from
`docs/rom_map.md`'s whole-ROM categorization (the same 9 categories
`tools/report_units.py`/`tools/chunk_remaining_work.py` use for
decomp.dev progress buckets and `decomp-chunk` issue labels) - finer
than `src/`'s own directory layout, since several categories
(`actor`/`hud`/`overlay_ui`/`graphics_loading`/`game_loop`) share a
`src/` directory with another category on disk but are tracked
separately here specifically so two unrelated categories' parallel PRs
never have to touch the same status page. Each page lists that
category's matched functions by file, its parked (`NON_MATCHING`) ones
with a one-line summary of what's left, and any other file-specific
note. For *how* a function gets from one list to the other, see
[docs/workflow.md](../workflow.md); for the detailed per-function log
behind every entry here, see [docs/matching.md](../matching.md).

- [graphics.md](./graphics.md) - core OAM/sprite rendering, screen
  fades, palette blending, per-actor animation frames, text layout
- [actor.md](./actor.md) - the per-instance actor "self" object family
  (`src/graphics/actor_part*.c`)
- [hud.md](./hud.md) - the on-screen HUD stat-counter/icon widgets
  (`src/graphics/hud_*.c`)
- [overlay_ui.md](./overlay_ui.md) - the pause/options screen and its
  widgets (`src/graphics/settings_menu*.c`)
- [graphics_loading.md](./graphics_loading.md) - asset/graphics-package
  loading and the trigger-effect dispatch family
- [game_loop.md](./game_loop.md) - the top-level per-frame game loop
  (`MainLoop`, `UpdateGameFrame`) - filed under `src/system/` on disk
- [system.md](./system.md) - core startup, memory allocator, interrupts,
  input polling, tagged-asset loading, BIOS wrappers
- [util.md](./util.md) - math, string/printf, RNG, line-drawing, time
  formatting helpers
- [audio.md](./audio.md) - the Shin'en GAX2 sound engine's own code
  (mostly not started) and the wrapper layer around it (music/SFX
  triggering, fading/ducking - matched)

Two `docs/rom_map.md` categories - `menu_ui` and `fx` - don't get their
own page: both are individual functions scattered inside another
category's span rather than a separate block of their own (same reason
`tools/report_units.py` folds them into their containing category's
address range - see that file's comments). File whatever gets matched
there under the containing category's page instead.
