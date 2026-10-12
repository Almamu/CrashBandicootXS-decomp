#!/usr/bin/env python3
"""The rooms' collision layers as images: one pixel per cell, one colour per
cell value (terrain type, flag nibble and open bits, docs/flags/terrain.md).

The colours are the legend in data/levels/collision_legend.json (drawn in
docs/levels.md, "Collision layers"). An image is read back by colour, not by
palette index, so an editor that reorders or extends the palette doesn't
change the cells; a colour that isn't in the legend is an error. To use a
cell value no room has yet, add it to the legend with a colour of its own.

    tools/collision_bmp.py to-bmp MAP.bin OUT.bmp --width CELLS
        Draws a decoded collision map (little-endian u16 cells, row-major,
        CELLS wide) as an 8bpp indexed BMP with the legend's palette.

    tools/collision_bmp.py to-bin IMAGE OUT.bin
        The way back: the map's u16 cells, row-major.

tools/levels.py reads the BMPs directly (load_map), so the build needs no
intermediate file; `to-bin` is there to get the binary layout by hand.
"""
import argparse
import json
import struct
import sys
from pathlib import Path

LEGEND_JSON = Path(__file__).resolve().parent.parent / "data/levels/collision_legend.json"


def legend():
    """The legend's entries as (value, (r, g, b), name), in palette order."""
    out = []
    for e in json.loads(LEGEND_JSON.read_text())["cells"]:
        c = int(e["colour"], 16)
        out.append((int(e["value"], 16), (c >> 16, c >> 8 & 0xFF, c & 0xFF), e["name"]))
    values = [v for v, _, _ in out]
    colours = [c for _, c, _ in out]
    if len(set(values)) != len(values) or len(set(colours)) != len(colours):
        sys.exit("collision_bmp: %s: a value or a colour is listed twice" % LEGEND_JSON)
    if len(out) > 256:
        sys.exit("collision_bmp: %s: more than 256 entries" % LEGEND_JSON)
    return out


def write_bmp(path, width, cells):
    from PIL import Image

    entries = legend()
    index = {v: i for i, (v, _, _) in enumerate(entries)}
    missing = sorted(set(cells) - set(index))
    if missing:
        sys.exit("collision_bmp: %s: cell values not in the legend: %s"
                 % (path, ", ".join("0x%04X" % v for v in missing)))
    if len(cells) % width:
        sys.exit("collision_bmp: %s: %d cells is not a whole number of %d-cell rows" % (path, len(cells), width))
    img = Image.new("P", (width, len(cells) // width))
    img.putdata([index[v] for v in cells])
    pal = []
    for _, c, _ in entries:
        pal += c
    img.putpalette(pal)
    img.save(path, format="BMP")


def read_bmp(path):
    """(width, height, cells) of a collision image."""
    from PIL import Image

    by_colour = {c: v for v, c, _ in legend()}
    img = Image.open(path).convert("RGB")
    w, h = img.size
    cells = []
    bad = set()
    for px in img.getdata():
        v = by_colour.get(px)
        if v is None:
            bad.add(px)
            v = 0
        cells.append(v)
    if bad:
        sys.exit("collision_bmp: %s: colours not in the legend (%s): %s" % (
            path, LEGEND_JSON.name, ", ".join("#%02X%02X%02X" % c for c in sorted(bad)[:8])))
    return w, h, cells


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("mode", choices=("to-bmp", "to-bin"))
    ap.add_argument("input")
    ap.add_argument("output")
    ap.add_argument("--width", type=int, help="map width in cells (to-bmp)")
    args = ap.parse_args()
    if args.mode == "to-bmp":
        if not args.width:
            sys.exit("collision_bmp: to-bmp needs --width")
        data = Path(args.input).read_bytes()
        write_bmp(args.output, args.width, list(struct.unpack("<%dH" % (len(data) // 2), data)))
    else:
        _, _, cells = read_bmp(args.input)
        Path(args.output).write_bytes(struct.pack("<%dH" % len(cells), *cells))


if __name__ == "__main__":
    main()
