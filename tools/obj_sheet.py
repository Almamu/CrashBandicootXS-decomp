#!/usr/bin/env python3
"""OBJ sheets: a PNG that shows each OBJ (sprite) of a tile set whole,
laid out for viewing, <-> the tiles in the order the game uploads them.

With 1D OBJ mapping, an OBJ of W x H pixels is (W/8) * (H/8) consecutive
tiles, row by row. A tile set made of OBJs (a font's glyphs, the title
screen's letters) is those runs back to back, so drawn as a plain tile
sheet its OBJs come out cut into strips. These sheets are drawn with each
OBJ whole instead, and the build turns them back into the tiles' order
before grit converts them (graphics.mk, docs/graphics.md "OBJ sheets").

    tools/obj_sheet.py to-tiles SHEET.png OUT.png LAYOUT... [--width N]
        Writes OUT.png: the OBJs' tiles in order, row-major, N tiles wide
        (default 1), with SHEET's palette, for grit -gt. Every pixel of
        SHEET outside the OBJs must be 0, so drawing there fails.

    tools/obj_sheet.py from-tiles TILES.png SHEET.png LAYOUT...
        The reverse: lays out the tiles of TILES.png (in row-major order)
        as a sheet.

OBJs may overlap (--at). A pixel that several OBJs cover belongs to the
first of them, as on the GBA, where the OBJ with the lower OAM index is
drawn on top: the later ones are transparent (0) there. from-tiles
fails if they aren't.

LAYOUT:
    --obj WxH[*N]   the OBJs' sizes in pixels, in tile order (repeat it,
                    e.g. --obj 8x16*77 --obj 16x8*2)
    and either
    --columns C [--cell WxH]
                    a grid of C cells per row, each OBJ at the top left
                    of its own cell (default: the largest OBJ's size)
    or
    --at X,Y        each OBJ's position in pixels (once per OBJ)
"""
import argparse
import sys


def parse_size(text):
    w, h = (int(v) for v in text.lower().split("x"))
    if w <= 0 or h <= 0 or w % 8 or h % 8:
        sys.exit("obj_sheet: %s: not a whole number of tiles" % text)
    return w, h


def layout(args):
    """The OBJs as (x, y, w, h), and the sheet's size."""
    sizes = []
    for spec in args.obj:
        size, _, count = spec.partition("*")
        sizes += [parse_size(size)] * (int(count) if count else 1)
    if args.at:
        if args.columns or args.cell:
            sys.exit("obj_sheet: --at and --columns/--cell don't mix")
        if len(args.at) != len(sizes):
            sys.exit("obj_sheet: %d --at for %d OBJs" % (len(args.at), len(sizes)))
        pos = [tuple(int(v) for v in at.split(",")) for at in args.at]
        objs = [(x, y, w, h) for (x, y), (w, h) in zip(pos, sizes)]
        return objs, max(x + w for x, y, w, h in objs), max(y + h for x, y, w, h in objs)
    if not args.columns:
        sys.exit("obj_sheet: give --columns or --at")
    cw, ch = parse_size(args.cell) if args.cell else (max(w for w, h in sizes), max(h for w, h in sizes))
    if any(w > cw or h > ch for w, h in sizes):
        sys.exit("obj_sheet: an OBJ is larger than the --cell")
    objs = [(i % args.columns * cw, i // args.columns * ch, w, h) for i, (w, h) in enumerate(sizes)]
    rows = (len(sizes) + args.columns - 1) // args.columns
    return objs, args.columns * cw, rows * ch


def tile_cells(objs):
    """Each tile's top-left pixel in the sheet and its OBJ, in tile order."""
    return [(x + tx, y + ty, n) for n, (x, y, w, h) in enumerate(objs) for ty in range(0, h, 8) for tx in range(0, w, 8)]


def owners(objs):
    """The first OBJ that covers each pixel."""
    owner = {}
    for n, (x, y, w, h) in enumerate(objs):
        for j in range(h):
            for i in range(w):
                owner.setdefault((x + i, y + j), n)
    return owner


def to_tiles(args):
    from PIL import Image

    objs, width, height = layout(args)
    sheet = Image.open(args.src)
    if sheet.mode != "P":
        sys.exit("obj_sheet: %s: not an indexed PNG" % args.src)
    if sheet.size != (width, height):
        sys.exit("obj_sheet: %s: %dx%d, the layout is %dx%d" % ((args.src,) + sheet.size + (width, height)))
    px = sheet.load()
    owner = owners(objs)
    for y in range(height):
        for x in range(width):
            if (x, y) not in owner and px[x, y]:
                sys.exit("obj_sheet: %s: pixel (%d, %d) is outside every OBJ and isn't 0" % (args.src, x, y))
    cells = tile_cells(objs)
    if len(cells) % args.width:
        sys.exit("obj_sheet: %d tiles don't make rows of %d" % (len(cells), args.width))
    out = Image.new("P", (args.width * 8, len(cells) // args.width * 8))
    out.putpalette(sheet.getpalette())
    opx = out.load()
    for t, (x, y, n) in enumerate(cells):
        ox, oy = t % args.width * 8, t // args.width * 8
        for j in range(8):
            for i in range(8):
                if owner[x + i, y + j] == n:
                    opx[ox + i, oy + j] = px[x + i, y + j]
    out.save(args.dst)


def from_tiles(args):
    from PIL import Image

    objs, width, height = layout(args)
    tiles = Image.open(args.src)
    if tiles.mode != "P":
        sys.exit("obj_sheet: %s: not an indexed PNG" % args.src)
    cols = tiles.size[0] // 8
    cells = tile_cells(objs)
    if cols * (tiles.size[1] // 8) != len(cells):
        sys.exit("obj_sheet: %s has %d tiles, the layout %d" % (args.src, cols * (tiles.size[1] // 8), len(cells)))
    tpx = tiles.load()
    sheet = Image.new("P", (width, height))
    sheet.putpalette(tiles.getpalette())
    px = sheet.load()
    owner = owners(objs)
    for t, (x, y, n) in enumerate(cells):
        sx, sy = t % cols * 8, t // cols * 8
        for j in range(8):
            for i in range(8):
                c = tpx[sx + i, sy + j]
                if owner[x + i, y + j] == n:
                    px[x + i, y + j] = c
                elif c:
                    sys.exit("obj_sheet: OBJ %d isn't transparent at (%d, %d), which OBJ %d covers"
                             % (n, x + i, y + j, owner[x + i, y + j]))
    sheet.save(args.dst)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    sub = ap.add_subparsers(dest="mode", required=True)
    for name, fn in (("to-tiles", to_tiles), ("from-tiles", from_tiles)):
        p = sub.add_parser(name)
        p.add_argument("src")
        p.add_argument("dst")
        p.add_argument("--obj", action="append", required=True)
        p.add_argument("--columns", type=int)
        p.add_argument("--cell")
        p.add_argument("--at", action="append")
        if fn is to_tiles:
            p.add_argument("--width", type=int, default=1)
        p.set_defaults(fn=fn)
    args = ap.parse_args()
    args.fn(args)


if __name__ == "__main__":
    main()
