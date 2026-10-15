# Screen size

The GBA's screen is 240x160. `platform/gba/include/gba/defines.h` has it as
`DISPLAY_WIDTH` x `DISPLAY_HEIGHT` (a port's own `gba/defines.h` can set
another size, a widescreen 426x240 say; docs/platforms.md). Every value the game derives
from the screen size is spelled with these, through the names in
`engine/include/screen.h`, so that changing the defines changes the game
consistently (#979). `core.h` includes `screen.h`.

## The macros (`engine/include/screen.h`)

| Macro | GBA value | What |
|---|---|---|
| `DISPLAY_CENTER_X`, `DISPLAY_CENTER_Y` | 120, 80 | half the screen: its centre |
| `DISPLAY_WIDTH_TILES`, `DISPLAY_HEIGHT_TILES` | 30, 20 | the screen in 8x8 tiles |
| `DISPLAY_RIGHT(w)`, `DISPLAY_BOTTOM(h)` | `240 - w`, `160 - h` | where something `w` wide (`h` high) touches the right (bottom) edge |
| `DISPLAY_LEFT(x)`, `DISPLAY_TOP(y)` | `x`, `y` | `x` (`y`) pixels from the left (top) edge: the corner-anchored layouts (the HUD) name their anchor |
| `CAMERA_MAX_X(w)`, `CAMERA_MAX_Y(h)` | `w - 240`, `h - 160` | the scroll limit of a room or BG `w` x `h` pixels |
| `ACTIVE_AREA_WIDTH`, `ACTIVE_AREA_HEIGHT` | 440, 280 | the activation window: the screen plus `ACTIVE_AREA_MARGIN_X/Y` (100, 60) on each side |
| `DISPLAY_ART_WIDTH`, `DISPLAY_ART_HEIGHT` | 240, 160 | the fixed size of the full-screen art |
| `DISPLAY_ART_X`, `DISPLAY_ART_Y` | 0, 0 | where that art's top-left corner goes: centred on a bigger screen |
| `DISPLAY_ART_CENTER_X`, `DISPLAY_ART_CENTER_Y` | 120, 80 | the art's centre in its own coordinates: what an affine picture is scaled around |
| `DISPLAY_ART_MARGIN_LEFT`, `_TOP`, `_RIGHT`, `_BOTTOM` | 0 | the screen between each of the art's borders and the screen's edge: what a logo piece flies in through (see "The logo fly-ins") |

`gba/defines.h` has the hardware's side, next to `OAM_ENTRY_COUNT`:

| Macro | GBA | A port with wider OAM fields | What |
|---|---|---|---|
| `OAM_X(x)` | `x & 0x1FF` | `x` | a screen X written into OAM attribute 1's 9-bit field |
| `OAM_Y(y)` | `y & 0xFF` | `y` | a screen Y written into OAM attribute 0's 8-bit field |

The game code never truncates a screen coordinate itself: no `& 0x1FF`
or `& 0xFF` on a coordinate, no `(u8)` cast or 9-bit wrap before it is
written. Where the ROM masks one before packing it into an OAM entry,
the mask is spelled with `OAM_X()`/`OAM_Y()`, so a platform library with
wider fields can make it a no-op. The registers and hardware structures
(OAM attributes, BG and window registers) are the platform's; a port's
library defines them with wider types.

### Matching

Every macro is a constant expression: the generated code doesn't change
as long as a site keeps its operation order. `x <= DISPLAY_WIDTH - 1` is
the ROM's `x <= 239`, `(DISPLAY_WIDTH - w) >> 1` its `(240 - w) >> 1`,
`INT_TO_Q8(DISPLAY_CENTER_X)` its `0x7800`. Don't rewrite a test into
another shape (`x < DISPLAY_WIDTH` for `x <= 239`) or move a constant to
another operand. Each conversion is checked like any other change: a full
clean `make compare`, and `make NON_MATCHING=1 report` at 100%.

## Layouts

A position in a 2D layout (a menu's text row, an icon, a HUD counter)
is measured from an anchor, never from the screen's (0,0):

- **Over the 240x160 art** (every menu, the title screen, the logos):
  from the art's origin, `DISPLAY_ART_X + x`, `DISPLAY_ART_Y + y`. The
  art is centred on a bigger screen, and the layout moves with it.
  `x` and `y` are the ROM's numbers, measured on the art.
- **Over the game's view** (the HUD): from the screen's edges or its
  centre, `DISPLAY_RIGHT(w)`, `DISPLAY_BOTTOM(h)`, `DISPLAY_CENTER_X`.
  A bigger screen shows more of the level, and the HUD stays in its
  corners.
- **Centred text** keeps `(DISPLAY_WIDTH - w) >> 1`: the art's centre is
  the screen's.
- **Relative positions** (`gPauseGemIconPos[0].x - 0x14`, `label1 + 0x2b`,
  `-panelSlideX + ...`) take their anchor from the position they are
  measured from, and stay as they are.

The position tables in engine/src/*/data (`gLevelSelect*Pos`, `gPause*IconPos`,
`gHudPartPositions`, the logo pieces' motions) are anchored the same
way, in their initializers. On the GBA every anchor is 0 or folds into
the ROM's constant (`DISPLAY_ART_Y + 0x87` is 0x87), so the code and the
data are unchanged. Keep the ROM's operation order: `item * 10 + 0x80`
becomes `item * 10 + (DISPLAY_ART_Y + 0x80)`, `0xa0 - w` becomes
`DISPLAY_ART_X + 0xa0 - w`.

### The logo fly-ins

The title logo's and the Vicarious Visions logo's pieces move through
`struct delta_record` steps (`gTitleLogoPieceMotionN` in
title_screen_gfx.cpp, `gVvLogoPieceMotionNN` in universal_vv_logo_motions.cpp).
A step sets the piece's absolute x and y on the art, then adds its
deltas (Q16.16 pixels a frame) for its hold - 1 frames; the next step
sets the position it has reached, so the ROM's deltas are
`(next x - x) * 0x10000 / hold`. A seed (`gTitleLogoPieceSeeds`,
`gVvLogoPieceSeeds`) delays each piece's first step.

Only the first step of 21 pieces lies outside the art: the VV logo's
pieces 1-19 start at x 248 (240 for piece 19), just past its right
border, and the title logo's piece 0 at y 160 and piece 1 at (-96, 160),
below and left of it. On the GBA that is just off screen; on a bigger
screen it is on it. So those steps come from `LOGO_FLY_IN_STEP`
(engine/include/game/ui/logo_fly_in.h), which lengthens the step backwards through the
screen's margin around the art (`DISPLAY_ART_MARGIN_LEFT/TOP/RIGHT/BOTTOM`,
screen.h):

- `LOGO_FLY_IN_FRAMES(margin, speed)` is the frames the step's own speed
  takes to cross the margin on the side the piece comes from, rounded up
  (both margins for the title's piece 1, the longer wins).
- The step starts that many frames earlier, that far back along its
  path: as far past the screen's edge as the ROM's start is past the
  art's border, or up to one frame's move further. Its deltas are the
  ROM's, so it passes the ROM's start on the ROM's frame and reaches the
  next step where the ROM does.
- The piece's seed is that many frames shorter (`0x5a -
  VV_LOGO_PIECE01_FLY_IN`), so every piece lands on the ROM's frame and
  the logo keeps its timing.

The tables keep their length, and `UpdateLogoPieces`/`UpdateVvLogoPieces`
walk them unchanged. The generalisation is constant expressions only:
plain `make` builds whatever the defines say, and on the GBA every margin
is 0, so every fly-in is 0 frames and the data is the ROM's. The seeds
(84 frames at least) bound how long a fly-in can be.

| Size | VV pieces 1-19: start x (frames added) | Title piece 0 | Title piece 1 |
|---|---|---|---|
| 240x160 | 248 (240 for 19), +0 | (152, 160), +0 | (-96, 160), +0 |
| 426x240 | 435-444, +6 to +32 | (268, 244), +3 | (-108, 288), +6 |

(screen coordinates; at 426x240 the art is at (93, 40).)

## The types

A value that holds a screen position, a screen-space offset or a size
derived from the screen is declared with one of `screen.h`'s types, in
struct fields, parameters, return types and locals:

| Type | GBA | What |
|---|---|---|
| `screen_x_t`, `screen_y_t` | `s32` | a pixel position on the screen (it can be off screen) |
| `screen_ux_t`, `screen_uy_t` | `u32` | the same, where the ROM keeps it unsigned (the font cursor) |
| `screen_dx_t`, `screen_dy_t` | `s32` | a signed offset or delta in pixels |
| `screen_w_t`, `screen_h_t` | `s32` | a width or height in pixels (measured text, the screen's size) |
| `screen_x_q8_t`, `screen_y_q8_t` | `s32` | a position in Q8 (24.8 fixed point, #916) |
| `screen_dx_q8_t`, `screen_dy_q8_t` | `s32` | an offset in Q8 |

Each type is the one the ROM uses, so the matching build is unchanged; a
port with a bigger screen can widen them in `screen.h` alone. A typedef
doesn't change a C++ mangled name either. Where the ROM keeps a screen
value narrower (a `u8` Y, an `s16` X), the declaration keeps its type and
says so in a comment: it can't hold every position of a bigger screen.
Retype one struct or function family at a time and rebuild the whole ROM:
a type reaches every reader, and a different type changes the extension
instructions and register allocation (#855).

### The narrow ones that stay

A survey of the `u8`/`s8`/`s16`/`u16` fields, parameters and locals in
the drawing code (engine/src/game/ui, engine/src/engine/save, engine/src/engine/text,
engine/src/engine/gfx, the sprite classes) finds these screen values
narrower than `s32`. Each keeps its type, because the ROM reads or
writes it with a byte or halfword access, and says so where it is
declared:

| Where | Type | Why it stays |
|---|---|---|
| `struct delta_record`'s `dPosA`/`dPosB` (frontend.hpp): the logo pieces' x and y | `u16` | ROM data read with `ldrh` and shifted into posA/posB's high halfword; a negative position is stored as a `u16` (`(u16)(DISPLAY_ART_Y - 1)`) |
| `LogoPiece`'s `posA.h.i`/`posB.h.i` (frontend.hpp) | `s16` | the integer half of a Q16.16 screen position, read as a halfword |
| `Font::DrawGlyph`'s read of `posY` (font_glyph.cpp) | `u8` | OAM's 8-bit Y, see "Platform-layer limits" |
| `union oam_shadow_entry`'s `attr0.y`, `struct oam_attrs`' `x:9`/`y:8` (gfx.h) | OAM fields | the hardware's format: `OAM_X`/`OAM_Y` |
| `struct bg_affine_src`'s `scrX`/`scrY` (agb_syscall.h), ZoomBg's affine centre | `s16` | the BIOS BgAffineSet call's format |

Not screen values, so not retyped: the sprite banks' piece offsets
(`s16`, the asset format), the hit boxes' offsets, and the `u16 x, y` of
the spawn functions (`Sprite::Create`, `Spawn*`), which are level
positions from the room records. Everything else that holds a screen
value in that code is an `s32`/`u32` and takes a `screen_*_t` type
(Font, the HUD, the culling draws, the starfield, the credits, ZoomBg,
the level select's slide and icon offsets, `Sprite::DrawWithOffset`,
`ScaledSprite`).

## The inventory: `tools/magic_numbers.py --screen`

`tools/magic_screen.py` scans the game code and its data tables (engine/src/,
engine/src/*/data/ included, and engine/include/; comments and strings blanked) for the
literals a 240x160 screen leaves behind and classifies each one with a
hand-made table (`CLASSES`, and `LAYOUT_CLASSES` for the layouts):

- **screen**: derived from the screen size. Convert it.
- **hardware**: fixed by the GBA: a register's field width, the 32x32 BG
  screen block, Mode 4's second frame at VRAM + 0xA000, a palette nibble.
  It stays, spelled through a `gba/` macro where the code masks
  (`OAM_X`, `OAM_Y`, `OAM_ENTRY_COUNT`).
- **coincidence**: the same number for something else: a timer of 120
  frames, a depth of 0xA000 in a 3D level, a struct size of 0x78. It
  stays, or gets a name for what it is.

```
tools/magic_numbers.py --screen [--show]           # every site: kind, value, category, reason
tools/magic_numbers.py --screen --report OUT       # Markdown: per kind and directory, the sites
tools/magic_numbers.py --screen --category screen  # what is left to convert
tools/magic_numbers.py --screen --json --path engine/src/game/ui
```

The kinds it looks for: 240/160 (`size`), 239/159 (`last`), 120/80
(`centre`), their Q8 forms (`q8`, including the negative words
`0xFFFF1000` and the few derived values found by hand, such as the
camera's look-ahead), the 440x280 activation window (`range`), 240x160
frames in bytes (`bitmap`), the screen in tiles next to a tile, row or
column (`tiles`), `& 0x1FF`/`& 0xFF` on a coordinate (`mask`), 512/256
added to or compared with a coordinate (`wrap`), 128 next to the OAM
(`oam`) and VCOUNT scanline counts (`scanline`). The `layout` kind is the
layouts' fixed positions (see "Layouts" above): in the UI code
(engine/src/game/ui, engine/src/engine/save, engine/src/engine/text, game_progress.cpp), an argument of a
position call (`SetPos`, `Move`, `SetPixelPos`, `SetEntityPixelPos`,
`SetAabbPos`, the save menu's `SET_ROW_OBJ_POS`/`DRAW_ROW`/`DrawSlotStats`),
a local `x`/`y` or a sprite's Q8 position that adds a literal to nothing
but measured sizes and counters; in engine/src/*/data, the `{ x, y }` entries of
the position tables and the logo pieces' motions. A converted site isn't a
literal any more, and a literal added to a screen.h anchor
(`DISPLAY_ART_Y + 0x87`, `DISPLAY_RIGHT(0x14)`) is an offset from it, so
the screen count goes down as subsystems are converted; the hardware and
coincidence sites stay listed with their reason.

Not every screen-dependent value is one of these literals: a value can
be a sum (the activation window's 100x60 margin) or a count (the crate
grid's columns). The table lists the ones found by hand; the list below
has the rest.

## Other screen-dependent things

1. **Buffers and loops sized by the screen.** The starfield's 4bpp frame
   (`tileBuffer`, 240x160/2 bytes) and its 30x20 tile map. The level BGs'
   32x32 screen blocks and the streamer's 64x32 ring are fixed sizes
   (see "Platform-layer limits" below). The game has no per-scanline
   (HBlank) table; its only VCOUNT use is the music's interrupt
   (engine/src/engine/audio/, out of scope).
2. **Coordinate wrapping and culling.** The game culls with explicit
   on-screen tests (`x <= DISPLAY_WIDTH - 1`, `x + w >= 0`) before it
   writes an OAM entry; nothing relies on the OBJ wrap at 512/256. The
   OAM masks are `OAM_X()`/`OAM_Y()`.
3. **Activation and spawn ranges.** Crates (`CrateList::Update`), parts
   (`PartList::Update`) and entities (`Entity::IsNearCamera`) are active
   in a 440x280 window around the camera: the screen plus 100 pixels on
   each side and 60 above and below. The window scales with the screen;
   the margins stay. The crate list files its crates in 256-pixel grid
   columns, and its passes visit the camera's column and the
   `CRATE_GRID_SPAN` to its right (crate_list.hpp): enough columns for
   the window's right edge, 2 on the GBA, 3 for a 426-pixel screen. Distances measured from the player (the periodic
   spawner's 0xA1-0x18F, the sound fall-off at 0xA0) are tuning, not
   the screen.
4. **Camera framing.** The camera centres its target
   (`DISPLAY_CENTER_X/Y`); its directional look-ahead reaches a sixth of
   the screen each way (0x2800 x 0x1AAA in Q8), and scales. The facing
   look-ahead (0x1276) and the vertical offset (-0x1000) are fixed.
5. **Fixed-size art.** Full-screen pictures (the cutscenes, the title
   screen, the menus' backgrounds) are 240x160 whatever the screen is;
   `DISPLAY_ART_X/Y` is their origin, and layouts drawn over them are
   relative to it (see "Layouts").
6. **Sprite count.** `OAM_ENTRY_COUNT` (128 on the GBA; a port's `gba/defines.h` can have more)
   sizes the OAM shadow buffer.
7. **A compile check.** `make screen-check` compiles every game object
   (engine/src/, and the engine/src/*/data/ tables that spell a `DISPLAY_` name or a
   logo fly-in's `_FLY_IN`: the layouts' position tables, the logo
   pieces' motions and their seeds) with another screen size,
   `SCREEN_CHECK_WIDTH` x `SCREEN_CHECK_HEIGHT` (426x240, a widescreen size, by
   default; `make screen-check SCREEN_CHECK_WIDTH=256
   SCREEN_CHECK_HEIGHT=192` for another), into build/screen-check, with
   the usual warnings as errors. Nothing is linked or compared. gba/
   defines.h takes `DISPLAY_WIDTH`/`DISPLAY_HEIGHT` from the command line
   when they are defined there. It catches a screen constant stored in a
   type or bitfield too narrow for the bigger size (agbcc's "large integer
   implicitly truncated") and compile-time sizes that stop holding; a
   narrow variable compared with a bigger constant isn't warned about.
   A literal written where the defines are meant still compiles, so CI
   also runs `tools/magic_numbers.py --screen --check`, which fails on a
   screen-size literal that no rule classifies.

## Platform-layer limits

Some sizes the game's code is written against are the GBA's formats,
not the screen's. They stay as they are; a port with a bigger screen
replaces what uses them in its platform layer. `screen.h` names them:

| Name | Value | Where | Limit |
|---|---|---|---|
| `BG_SCREEN_BLOCK_TILES` | 32 | a text BG's screen block: 32x32 map entries, 256x256 pixels. `BgLayer` (bg_layer.cpp) keeps a 32x32 window of the level map in one, wrapping modulo 32; the cell-anim maps (`FillTileMap`, cell_anim.cpp) put columns past 31 in the next block | the screen and one tile more must fit in 256 pixels each way: a wider or taller screen needs a bigger BG map |
| `BG_STREAM_RING_COLS` x `BG_STREAM_RING_ROWS` | 64 x 32 | `BgStreamer`'s ring (bg_streamer.cpp): 4x4 chunks of 16x8 tiles (128x64 pixels), the four from the chunk at the layer's scroll position, wrapped with `& (COLS - 1)` / `& (ROWS - 1)` | `DISPLAY_WIDTH` <= 512 - 127 and `DISPLAY_HEIGHT` <= 256 - 63: 240x160 fits, 426x240 doesn't |
| `OAM_X(x)`, `OAM_Y(y)` (gba/defines.h) | 9 bits, 8 bits | OAM attribute 1's X and attribute 0's Y | no-ops in a port whose platform headers give OAM wider fields (docs/platforms.md) |
| `Font::DrawGlyph`'s read of `posY` (font_glyph.cpp) | 8 bits | the ROM reads the font cursor's Y with a byte load: OAM's 8-bit Y, truncated by the load itself, which `OAM_Y()` can't spell | a screen taller than 256 pixels needs that read widened to the whole `posY` |
| Mode 4's second frame (slideshow.cpp) | `VRAM + 0xA000` | the cutscene's double-buffered 240x160 bitmap | the picture is fixed-size art; the frame size is the hardware's |

`make screen-check` compiles at 426x240 regardless: it checks that the
game code spells its screen values with the defines, not that a bigger
screen runs on these GBA formats.
