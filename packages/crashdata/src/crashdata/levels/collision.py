#!/usr/bin/env python3
"""The rooms' collision layers as images, at the BG's scale: a cell is one
8x8 tile, like a BG layer's cell, and is drawn as an 8x8 block of one colour.
A room's collision image is therefore exactly as large as its BG0 picture
(`tools/levels.py render ROOM bg0`) and lines up with it pixel for pixel.
Each cell value (terrain type, flag nibble and open bits,
docs/flags/terrain.md) has its own colour.

The colours are the legend in games/xs/data/levels/collision_legend.json (drawn in
docs/levels.md, "Collision layers"). An image is read back by colour, not by
palette index, so an editor that reorders or extends the palette, or saves
the image as true colour, doesn't change the cells. A colour that isn't in
the legend is an error, and so is a block that isn't a single colour; both
messages name the cell. To use a cell value no room has yet, add it to the
legend with a colour of its own.

    tools/collision_bmp.py to-image MAP.bin OUT.png --width CELLS
        Draws a decoded collision map (little-endian u16 cells, row-major,
        CELLS wide) as an 8bpp indexed image with the legend's palette,
        8x8 pixels per cell (PNG, or BMP if OUT ends in .bmp).

    tools/collision_bmp.py to-bin IMAGE OUT.bin
        The way back: the map's u16 cells, row-major.

tools/levels.py reads the images directly (load_map), so the build needs no
intermediate file; `to-bin` is there to get the binary layout by hand.
"""
import json
import sys
from pathlib import Path

from crashdata.common.game import ROOT

from crashdata import schemas

LEGEND_JSON = Path(ROOT).resolve() / "games/xs/data/levels/collision_legend.json"

# A cell's size in pixels, each way: one 8x8 tile, as in the BG layers.
CELL_PX = 8

# What a cell value is (docs/flags/terrain.md, engine/include/game/level/level.h):
# its terrain type in bits 0-7, a flag nibble in 8-11, the open bits in 12-15.
# The lookups (TileCache, engine/src/game/level/tile_cache.cpp) read type
# 1-0x23's heights and 0x24 and above's solid rows from gTerrainTypes, which
# has TERRAIN_TYPES of them (engine/src/game/level/data/terrain_types.cpp):
# a type past them reads its rows from the bytes after the table.
TERRAIN_TYPES = 51
TERRAIN_LAST_HEIGHTS = 0x23
OPEN_BITS = ((1, "TERRAIN_FLAG_OPEN_RIGHT"), (2, "TERRAIN_FLAG_OPEN_LEFT"), (4, "TERRAIN_FLAG_OPEN_DOWN"),
             (8, "TERRAIN_FLAG_OPEN_UP"))
# Most entries the legend has: an image's palette, less the index read_image
# keeps for a colour that isn't in it.
MAX_ENTRIES = 255


def describe(value):
    """A cell value's name as the legend spells them: its type
    (TERRAIN_TYPE_NONE, "heights 0xNN", "solid 0xNN"), its flag nibble, its
    open bits."""
    t, flags, hi = value & 0xFF, value >> 8 & 0xF, value >> 12
    parts = ["TERRAIN_TYPE_NONE" if t == 0 else ("heights 0x%02X" if t <= TERRAIN_LAST_HEIGHTS else "solid 0x%02X") % t]
    if flags:
        parts.append("flags 0x%X" % flags)
    if hi:
        parts.append("|".join(n for b, n in OPEN_BITS if hi & b))
    return ", ".join(parts)


def new_entry_errors(value, colour, entries):
    """Why the legend (`entries`, legend()'s) can't take a new value with a
    colour ((r, g, b)): [] when it can. A value is 0-0xFFFF, new, in a colour
    of its own; its terrain type one gTerrainTypes has, or one of the
    legend's values already has (the ROM's rooms use a few past the table)."""
    out = []
    if not isinstance(value, int) or isinstance(value, bool) or not 0 <= value <= 0xFFFF:
        return ["a cell value is 0-0xFFFF (16 bits: terrain type, flag nibble, open bits)"]
    if len(entries) >= MAX_ENTRIES:
        out.append("the legend has %d values, the most a collision image's palette holds" % MAX_ENTRIES)
    for v, c, n in entries:
        if v == value:
            out.append("0x%04X is in the legend already (%s)" % (value, n))
        if c == tuple(colour):
            out.append("#%02X%02X%02X is 0x%04X's colour (%s): each value has its own" % (c + (v, n)))
    t = value & 0xFF
    if t >= TERRAIN_TYPES and t not in {v & 0xFF for v, _, _ in entries}:
        out.append("terrain type 0x%02X: gTerrainTypes has types 0-0x%02X (engine/src/game/level/data/terrain_types.cpp); "
                   "a type past them reads its rows from the bytes after the table, so only those the legend's values "
                   "already have are taken" % (t, TERRAIN_TYPES - 1))
    return out


def legend():
    """The legend's entries as (value, (r, g, b), name), in palette order."""
    out = []
    for e in schemas.load(LEGEND_JSON)["cells"]:
        c = int(e["colour"], 16)
        out.append((int(e["value"], 16), (c >> 16, c >> 8 & 0xFF, c & 0xFF), e["name"]))
    values = [v for v, _, _ in out]
    colours = [c for _, c, _ in out]
    if len(set(values)) != len(values) or len(set(colours)) != len(colours):
        sys.exit("collision_bmp: %s: a value or a colour is listed twice" % LEGEND_JSON)
    if len(out) > MAX_ENTRIES:
        sys.exit("collision_bmp: %s: more than %d entries" % (LEGEND_JSON, MAX_ENTRIES))
    return out


def write_image(path, width, cells, fmt=None):
    """The cells (row-major, `width` cells wide) as an 8bpp indexed image,
    CELL_PX x CELL_PX pixels per cell; the format follows the suffix, or
    `fmt` (PIL's name: "PNG"), where `path` is a file object (the level
    editor's, crashdata.levels.room_edit)."""
    from PIL import Image

    entries = legend()
    index = {v: i for i, (v, _, _) in enumerate(entries)}
    missing = sorted(set(cells) - set(index))
    if missing:
        sys.exit("collision_bmp: %s: cell values not in the legend: %s"
                 % (path, ", ".join("0x%04X" % v for v in missing)))
    if len(cells) % width:
        sys.exit("collision_bmp: %s: %d cells is not a whole number of %d-cell rows" % (path, len(cells), width))
    height = len(cells) // width
    img = Image.new("P", (width, height))
    img.putdata([index[v] for v in cells])
    img = img.resize((width * CELL_PX, height * CELL_PX), Image.NEAREST)
    img.putpalette([ch for _, c, _ in entries for ch in c])
    img.save(path, format=fmt)


def _where(x, y):
    return "cell (%d, %d) (pixels %d-%d, %d-%d)" % (
        x, y, x * CELL_PX, x * CELL_PX + CELL_PX - 1, y * CELL_PX, y * CELL_PX + CELL_PX - 1)


def read_image(path):
    """(width, height, cells) of a collision image, in cells."""
    from PIL import Image

    entries = legend()
    by_colour = {c: i for i, (_, c, _) in enumerate(entries)}
    img = Image.open(path)
    pw, ph = img.size
    if pw % CELL_PX or ph % CELL_PX:
        sys.exit("collision_bmp: %s: %dx%d pixels is not a whole number of %dx%d cells"
                 % (path, pw, ph, CELL_PX, CELL_PX))
    w, h = pw // CELL_PX, ph // CELL_PX

    # Every pixel as one byte, its colour's legend index (by colour, so the
    # image's own palette order doesn't matter); 0xFF for a colour that
    # isn't in the legend.
    if img.mode == "P" and img.palette.mode == "RGB":
        pal = img.getpalette()
        table = bytearray(b"\xFF" * 256)
        for i in range(len(pal) // 3):
            table[i] = by_colour.get(tuple(pal[3 * i:3 * i + 3]), 0xFF)
        raw = img.tobytes()
        data = raw.translate(bytes(table))
    else:  # true colour (or anything else): read the RGB triplets
        pal = None
        raw = img.convert("RGB").tobytes()
        data = None

    cells = []
    for y in range(h):
        if data is not None:
            rows = [data[(y * CELL_PX + r) * pw:(y * CELL_PX + r + 1) * pw] for r in range(CELL_PX)]
            samples = rows[0][::CELL_PX]
            expect = bytes(b for b in samples for _ in range(CELL_PX))
            bp = 1
        else:
            rows = [raw[(y * CELL_PX + r) * pw * 3:(y * CELL_PX + r + 1) * pw * 3] for r in range(CELL_PX)]
            samples = [rows[0][x * CELL_PX * 3:x * CELL_PX * 3 + 3] for x in range(w)]
            expect = b"".join(s * CELL_PX for s in samples)
            bp = 3
        for r, row in enumerate(rows):
            if row != expect:
                for x in range(w):
                    a, b = x * CELL_PX * bp, (x + 1) * CELL_PX * bp
                    if row[a:b] != expect[a:b]:
                        sys.exit("collision_bmp: %s: %s isn't a single colour" % (path, _where(x, y)))
        for x in range(w):
            if data is not None:
                i = samples[x]
                k = raw[y * CELL_PX * pw + x * CELL_PX]
                colour = tuple(pal[3 * k:3 * k + 3]) if 3 * k + 3 <= len(pal) else (0, 0, 0)
            else:
                colour = tuple(samples[x])
                i = by_colour.get(colour, 0xFF)
            if i == 0xFF:
                sys.exit("collision_bmp: %s: %s is #%02X%02X%02X, a colour that isn't in the legend (%s)"
                         % ((path, _where(x, y)) + colour + (LEGEND_JSON.name,)))
            cells.append(entries[i][0])
    return w, h, cells
