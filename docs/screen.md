# Screen size

The GBA's screen is 240x160. `include/gba/defines.h` has it as
`DISPLAY_WIDTH` x `DISPLAY_HEIGHT` (and 426x240 under `PORTABLE`, the
scaffold for a native or widescreen port). Every value the game derives
from the screen size is spelled with these, through the names in
`include/screen.h`, so that changing the defines changes the game
consistently (#979). `core.h` includes `screen.h`.

## The macros (`include/screen.h`)

| Macro | GBA value | What |
|---|---|---|
| `DISPLAY_CENTER_X`, `DISPLAY_CENTER_Y` | 120, 80 | half the screen: its centre |
| `DISPLAY_WIDTH_TILES`, `DISPLAY_HEIGHT_TILES` | 30, 20 | the screen in 8x8 tiles |
| `DISPLAY_RIGHT(w)`, `DISPLAY_BOTTOM(h)` | `240 - w`, `160 - h` | where something `w` wide (`h` high) touches the right (bottom) edge |
| `CAMERA_MAX_X(w)`, `CAMERA_MAX_Y(h)` | `w - 240`, `h - 160` | the scroll limit of a room or BG `w` x `h` pixels |
| `ACTIVE_AREA_WIDTH`, `ACTIVE_AREA_HEIGHT` | 440, 280 | the activation window: the screen plus `ACTIVE_AREA_MARGIN_X/Y` (100, 60) on each side |
| `DISPLAY_ART_WIDTH`, `DISPLAY_ART_HEIGHT` | 240, 160 | the fixed size of the full-screen art |
| `DISPLAY_ART_X`, `DISPLAY_ART_Y` | 0, 0 | where that art's top-left corner goes: centred on a bigger screen |
| `DISPLAY_ART_CENTER_X`, `DISPLAY_ART_CENTER_Y` | 120, 80 | the art's centre in its own coordinates: what an affine picture is scaled around |

`gba/defines.h` has the hardware's side, next to `OAM_ENTRY_COUNT`:

| Macro | GBA | `PORTABLE` | What |
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

## The inventory: `tools/magic_numbers.py --screen`

`tools/magic_screen.py` scans the game code (src/ without src/data/, and
include/; comments and strings blanked) for the literals a 240x160 screen
leaves behind and classifies each one with a hand-made table (`CLASSES`):

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
tools/magic_numbers.py --screen --json --path src/menus
```

The kinds it looks for: 240/160 (`size`), 239/159 (`last`), 120/80
(`centre`), their Q8 forms (`q8`, including the negative words
`0xFFFF1000` and the few derived values found by hand, such as the
camera's look-ahead), the 440x280 activation window (`range`), 240x160
frames in bytes (`bitmap`), the screen in tiles next to a tile, row or
column (`tiles`), `& 0x1FF`/`& 0xFF` on a coordinate (`mask`), 512/256
added to or compared with a coordinate (`wrap`), 128 next to the OAM
(`oam`) and VCOUNT scanline counts (`scanline`). A converted site isn't a
literal any more, so the screen count goes down as subsystems are
converted; the hardware and coincidence sites stay listed with their
reason.

Not every screen-dependent value is one of these literals: a value can
be a sum (the activation window's 100x60 margin), a count (the crate
grid's `lo + 2` columns) or a layout position on the 240x160 menu art.
The table lists the ones found by hand; the list below has the rest.

## Other screen-dependent things

1. **Buffers and loops sized by the screen.** The starfield's 4bpp frame
   (`tileBuffer`, 240x160/2 bytes) and its 30x20 tile map. The level BGs'
   32x32 screen blocks and the streamer's 64x32 ring are the hardware's
   BG map sizes: a port with a screen wider than 256 pixels needs a
   bigger BG map from its platform layer. The game has no per-scanline
   (HBlank) table; its only VCOUNT use is the music's interrupt
   (src/audio/, out of scope).
2. **Coordinate wrapping and culling.** The game culls with explicit
   on-screen tests (`x <= DISPLAY_WIDTH - 1`, `x + w >= 0`) before it
   writes an OAM entry; nothing relies on the OBJ wrap at 512/256. The
   OAM masks are `OAM_X()`/`OAM_Y()`.
3. **Activation and spawn ranges.** Crates (`CrateList::Update`), parts
   (`PartList::Update`) and entities (`Entity::IsNearCamera`) are active
   in a 440x280 window around the camera: the screen plus 100 pixels on
   each side and 60 above and below. The window scales with the screen;
   the margins stay. Distances measured from the player (the periodic
   spawner's 0xA1-0x18F, the sound fall-off at 0xA0) are tuning, not
   the screen.
4. **Camera framing.** The camera centres its target
   (`DISPLAY_CENTER_X/Y`); its directional look-ahead reaches a sixth of
   the screen each way (0x2800 x 0x1AAA in Q8), and scales. The facing
   look-ahead (0x1276) and the vertical offset (-0x1000) are fixed.
5. **Fixed-size art.** Full-screen pictures (the cutscenes, the title
   screen, the menus' backgrounds) are 240x160 whatever the screen is;
   `DISPLAY_ART_X/Y` is their origin, and layouts drawn over them are
   relative to it.
6. **Sprite count.** `OAM_ENTRY_COUNT` (128, more under `EXTENDED_OAM`)
   sizes the OAM shadow buffer.
7. **A compile check.** `make screen-check` compiles every game object
   (src/, not the src/data/ tables) with another screen size,
   `SCREEN_CHECK_WIDTH` x `SCREEN_CHECK_HEIGHT` (PORTABLE's 426x240 by
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
