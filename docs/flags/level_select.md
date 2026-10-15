# `ZoomBg::state` and `LevelSelectCursor::state`: the level select (#843)

The level-select screen (engine/include/game/ui/level_select.hpp) has two small state
machines, each an `s32 state` that the class's `Update` switches on. The
names sit next to each class: `ZOOM_BG_STATE_*` and
`LEVEL_SELECT_CURSOR_STATE_*`.

## ZoomBg (+0x0C)

BG2, the zooming level picture (engine/src/game/ui/zoom_bg.cpp). Update zooms it in
or out by 8 per frame between `scale` 8 (smallest) and 0x100 (1:1).

| Value | Name | Meaning | Entered by |
|---|---|---|---|
| 0 | `ZOOM_IN` | zooms in to 1:1, then `SHOWN` | `WAIT`, once it has loaded the picture |
| 1 | `ZOOM_OUT` | zooms out, then `WAIT` | ClearPicture (SFX_ZOOM_BG_OUT; the page turn) |
| 2 | `WAIT` | waits for a picture (`image` 11 is none); loads the requested one (SFX_ZOOM_BG_IN), then `ZOOM_IN` | the constructor, `ZOOM_OUT` |
| 3 | `SHOWN` | shown at 1:1, wobbling on the sine table; the twinkles move with it | `ZOOM_IN` |
| 4 | `EXIT` | zooms out while turning (`affine.angle` += 0x100), BG2 at the front, then `GONE` | StartExit (leaving the screen) |
| 5 | `GONE` | zoomed away; Update does nothing more | `EXIT` |

The tests are IsZoomingOut, IsWaiting, IsShown, IsExiting and IsGone, one
per state, which the level select's loops wait on. Draw builds the
zoom-only matrix for `ZOOM_IN ... SHOWN` and the rotating one for
`EXIT ... GONE`; SetPicture takes the picture only in `ZOOM_OUT ... WAIT`.
These GNU case ranges keep the ROM's values.

## LevelSelectCursor (+0x30)

The cursor (engine/src/game/ui/level_select_cursor.cpp). States 0-3 are the idle
animation cycle, drawn by the sprite part; 4 and 5 are the affine zoom,
drawn by hand into the OAM shadow buffer (Draw's `GROW ... SHRINK`).

| Value | Name | Meaning | Entered by |
|---|---|---|---|
| 0 | `IDLE` | plays gLevelSelectCursorAnims[0] and counts `timer` down; at 0, on frame 0, `CYCLE1` | `CYCLE3`, `GROW` (both reset `timer`) |
| 1 | `CYCLE1` | gLevelSelectCursorAnims[1] to its end, then `CYCLE2` | `IDLE` |
| 2 | `CYCLE2` | gLevelSelectCursorAnims[2] to its end, then `CYCLE3` | `CYCLE1` |
| 3 | `CYCLE3` | gLevelSelectCursorAnims[3] to its end, then `IDLE` | `CYCLE2` |
| 4 | `GROW` | grows by `speed` to 1:1, then `IDLE` | the constructor |
| 5 | `SHRINK` | shrinks by `speed` down to 8, and stays | Hide |

Move resets the idle timer only in `IDLE`. IsHidden (`SHRINK`) and
IsGrowing (`GROW`) are UNUSED.

## Not named here

- `ZoomBg::image`'s 11 ("no picture").
- The `scale` limits (8, 0xFF/0x100) and ZoomBg's wobble `phase`.
