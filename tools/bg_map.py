#!/usr/bin/env python3
"""Post-processing of grit's tile set + map output for the menu and title
backgrounds of graphics/menu_bg/ (docs/graphics.md, "Menu and title
backgrounds"). grit does the pixel conversion and the tile reduction;
this only renumbers or picks entries, so every tile and flip still comes
from grit.

    tools/bg_map.py first-use TILES MAP OUT_TILES OUT_MAP --bpp N
        TILES/MAP are grit's `-gt -m -mLf -ftb` output for a whole
        picture. grit always starts its reduced tile set with a blank
        tile, but the ROM's tile sets are in plain order of first use in
        the map (reading order). This renumbers grit's tiles that way and
        drops the ones the map doesn't use (grit's blank, unless the
        picture has one). Flip and palette bits are kept.

    tools/bg_map.py pick TILES MAP CHOICES OUT_MAP --bpp N
        For a picture whose tile set has identical tiles (built with grit
        `-fx` against its own tile-set strip): grit maps a cell to the first
        tile that matches, and CHOICES (JSON, {"cell": "entry", ...}, hex
        entries) gives the cells where the ROM uses another one. Each choice
        must draw the same pixels as grit's entry, so a choice for a cell
        that was redrawn fails with a message. Delete that line of the JSON
        to let grit pick.

    tools/bg_map.py extract [baserom.gba]
        Writes the graphics/menu_bg/ sources from the ROM: each package's
        whole picture (indexed, with the package's palette), and for a
        tile set with identical or unused tiles its tile-set strip. Then
        run grit on them as graphics.mk does and write CHOICES with:

    tools/bg_map.py extract-choices TILES MAP ROM_MAP OUT.json
        Writes CHOICES: the cells where ROM_MAP differs from grit's MAP.
"""
import argparse
import json
import struct
import sys
from pathlib import Path

# The packages (struct bg_package, read by LoadGraphicsPackage and
# TitleScreen::LoadBg): file name, palette, tiles, map, width and height
# in cells, bpp, and whether the map is the affine title map (entries
# are plain tile numbers; LoadBg keeps their low byte).
PACKAGES = [
    ("61c30c_menu_sky", 0x0861BADC, 0x0861C30C, 0x0862FB24, 32, 20, 4, False),
    ("61e5f8_pause_menu", 0x0861BB04, 0x0861E5F8, 0x0862FFF4, 30, 20, 8, False),
    ("62556c_level_select_page", 0x0861BD48, 0x0862556C, 0x0863053C, 32, 32, 8, False),
    ("628c50_continue_prompt_smoke", 0x0861BF30, 0x08628C50, 0x086308F0, 30, 20, 4, False),
    ("62a958_continue_prompt_glow", 0x0861BF58, 0x0862A958, 0x08630E1C, 30, 20, 4, False),
    ("62b34c_continue_prompt_uka_uka", 0x0861BF80, 0x0862B34C, 0x08631158, 30, 20, 8, False),
    ("62d0cc_universal_logo", 0x0861C1FC, 0x0862D0CC, 0x086315BC, 30, 20, 4, False),
    ("62e3b0_title_screen", 0x0861C224, 0x0862E3B0, 0x0863183C, 16, 16, 8, True),
]
OUT_DIR = Path("graphics/menu_bg")


def lz77_decompress(rom, addr):
    o = addr - 0x08000000
    if rom[o] != 0x10:
        sys.exit("bg_map: no LZ77 stream at %#x" % addr)
    size = int.from_bytes(rom[o + 1:o + 4], "little")
    o += 4
    out = bytearray()
    while len(out) < size:
        flags = rom[o]
        o += 1
        for b in range(8):
            if len(out) >= size:
                break
            if flags & 0x80 >> b:
                n, d = (rom[o] >> 4) + 3, ((rom[o] & 0xF) << 8 | rom[o + 1]) + 1
                o += 2
                for _ in range(n):
                    out.append(out[-d])
            else:
                out.append(rom[o])
                o += 1
    return bytes(out)


def extract(args):
    from PIL import Image

    rom = open(args.rom, "rb").read()
    OUT_DIR.mkdir(parents=True, exist_ok=True)
    for name, pal_a, tiles_a, map_a, w, h, bpp, affine in PACKAGES:
        raw = lz77_decompress(rom, pal_a)
        pal = struct.unpack("<%dH" % (len(raw) // 2), raw)
        rgb = [c for v in pal for c in ((v & 31) << 3, (v >> 5 & 31) << 3, (v >> 10 & 31) << 3)]
        tiles = split_tiles(lz77_decompress(rom, tiles_a), bpp)
        entries = struct.unpack("<%dH" % (w * h), lz77_decompress(rom, map_a))
        img = Image.new("P", (w * 8, h * 8))
        px = img.load()
        for i, e in enumerate(entries):
            draw = pixels(tiles[(e & 0xFF) if affine else (e & 0x3FF)], bpp, 0 if affine else e)
            for k, c in enumerate(draw):
                px[i % w * 8 + k % 8, i // w * 8 + k // 8] = c
        img.putpalette(rgb)
        img.save(OUT_DIR / (name + ".png"))
        used = {(e & 0xFF) if affine else (e & 0x3FF) for e in entries}
        if len(set(tiles)) != len(tiles) or len(used) != len(tiles):
            strip = Image.new("P", (8, 8 * len(tiles)))
            spx = strip.load()
            for t, tile in enumerate(tiles):
                for k, c in enumerate(pixels(tile, bpp, 0)):
                    spx[k % 8, t * 8 + k // 8] = c
            strip.putpalette(rgb)
            strip.save(OUT_DIR / (name + "_tiles.png"))


def read_u16(path):
    data = open(path, "rb").read()
    if len(data) % 2:
        sys.exit("bg_map: %s: odd size" % path)
    return list(struct.unpack("<%dH" % (len(data) // 2), data))


def write_u16(path, values):
    open(path, "wb").write(struct.pack("<%dH" % len(values), *values))


def split_tiles(data, bpp):
    size = 8 * bpp
    if len(data) % size:
        sys.exit("bg_map: tile data isn't a whole number of %dbpp tiles" % bpp)
    return [data[i:i + size] for i in range(0, len(data), size)]


def pixels(tile, bpp, entry):
    """The 64 colour indices `entry` draws (tile with its h/v flip)."""
    if bpp == 4:
        px = [tile[i // 2] >> 4 * (i & 1) & 0xF for i in range(64)]
    else:
        px = list(tile)
    hf, vf = entry >> 10 & 1, entry >> 11 & 1
    return [px[(7 - y if vf else y) * 8 + (7 - x if hf else x)] for y in range(8) for x in range(8)]


def first_use(args):
    tiles = split_tiles(open(args.tiles, "rb").read(), args.bpp)
    entries = read_u16(args.map)
    order = {}
    for e in entries:
        t = e & 0x3FF
        if t >= len(tiles):
            sys.exit("bg_map: %s: entry %#x is past the %d tiles" % (args.map, e, len(tiles)))
        order.setdefault(t, len(order))
    out_tiles = [None] * len(order)
    for t, n in order.items():
        out_tiles[n] = tiles[t]
    open(args.out_tiles, "wb").write(b"".join(out_tiles))
    write_u16(args.out_map, [e & ~0x3FF | order[e & 0x3FF] for e in entries])


def pick(args):
    tiles = split_tiles(open(args.tiles, "rb").read(), args.bpp)
    entries = read_u16(args.map)
    choices = json.load(open(args.choices))
    for cell, want in choices.items():
        i, want = int(cell), int(want, 16)
        got = entries[i]
        if (want & 0x3FF) >= len(tiles) or \
                pixels(tiles[want & 0x3FF], args.bpp, want) != pixels(tiles[got & 0x3FF], args.bpp, got) or \
                want >> 12 != got >> 12:
            sys.exit("bg_map: %s: cell %d (x %d, y %d) no longer matches entry %#06x; "
                     "remove it from the file to let grit pick" % (args.choices, i, i % args.width, i // args.width, want)
                     if args.width else
                     "bg_map: %s: cell %d no longer matches entry %#06x; remove it from the file" % (args.choices, i, want))
        entries[i] = want
    write_u16(args.out_map, entries)


def extract_choices(args):
    entries = read_u16(args.map)
    rom = read_u16(args.rom_map)
    lines = ['    "%d": "%04X"' % (i, r) for i, (g, r) in enumerate(zip(entries, rom)) if g != r]
    open(args.out_json, "w").write("{\n" + ",\n".join(lines) + "\n}\n")


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    sub = ap.add_subparsers(dest="mode", required=True)
    p = sub.add_parser("first-use")
    p.add_argument("tiles")
    p.add_argument("map")
    p.add_argument("out_tiles")
    p.add_argument("out_map")
    p.add_argument("--bpp", type=int, choices=(4, 8), required=True)
    p.set_defaults(fn=first_use)
    p = sub.add_parser("pick")
    p.add_argument("tiles")
    p.add_argument("map")
    p.add_argument("choices")
    p.add_argument("out_map")
    p.add_argument("--bpp", type=int, choices=(4, 8), required=True)
    p.add_argument("--width", type=int, help="map width in cells, for the error messages")
    p.set_defaults(fn=pick)
    p = sub.add_parser("extract")
    p.add_argument("rom", nargs="?", default="baserom.gba")
    p.set_defaults(fn=extract)
    p = sub.add_parser("extract-choices")
    p.add_argument("tiles")
    p.add_argument("map")
    p.add_argument("rom_map")
    p.add_argument("out_json")
    p.set_defaults(fn=extract_choices)
    args = ap.parse_args()
    args.fn(args)


if __name__ == "__main__":
    main()
