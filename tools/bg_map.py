#!/usr/bin/env python3
"""Post-processing of grit's tile set + map output for the struct
bg_package pictures of graphics/bg_package/ (the menu and title
backgrounds and the title's OBJ sprites; docs/graphics.md, "Menu and title
pictures"). grit does the pixel conversion and the tile reduction;
this only renumbers or picks entries, so every tile and flip still comes
from grit.

    tools/bg_map.py first-use TILES MAP OUT_TILES OUT_MAP --bpp N
        TILES/MAP are grit's `-gt -m -mLf -ftb` output for a whole
        picture. grit always starts its reduced tile set with a blank
        tile, but the ROM's tile sets are in plain order of first use in
        the map (reading order). This renumbers grit's tiles that way and
        drops the ones the map doesn't use (grit's blank, unless the
        picture has one). Flip and palette bits are kept.

        --high JSON: for an affine map, whose game code keeps only the
        entries' low byte: JSON ({"cell": "byte", ...}, hex) gives the
        cells whose ROM entry has a nonzero high byte anyway, a leftover
        of the original tool that nothing reads. It is put back as is.

    tools/bg_map.py extract [baserom.gba]
        Writes the graphics/bg_package/ sources from the ROM: each package's
        whole picture (indexed, with the package's palette) and, for an
        affine map with nonzero high bytes, the --high JSON
        (<name>_high.json).
"""
import argparse
import json
import struct
import sys
from pathlib import Path

# The packages (struct bg_package, read by BgSetup::Load,
# TitleScreen::LoadBg and TitleScreen::LoadObjTiles): file name, palette,
# tiles, map, width and height in cells, bpp, and whether the map is the
# affine title map (entries are plain tile numbers; LoadBg keeps their
# low byte).
PACKAGES = [
    ("61c30c_menu_sky", 0x0861BADC, 0x0861C30C, 0x0862FB24, 32, 20, 4, False),
    ("61e5f8_pause_menu", 0x0861BB04, 0x0861E5F8, 0x0862FFF4, 30, 20, 8, False),
    ("62556c_level_select_page", 0x0861BD48, 0x0862556C, 0x0863053C, 32, 32, 8, False),
    ("628c50_continue_prompt_smoke", 0x0861BF30, 0x08628C50, 0x086308F0, 30, 20, 4, False),
    ("62a958_continue_prompt_glow", 0x0861BF58, 0x0862A958, 0x08630E1C, 30, 20, 4, False),
    ("62b34c_continue_prompt_uka_uka", 0x0861BF80, 0x0862B34C, 0x08631158, 30, 20, 8, False),
    # CompanyLogos::LoadUniversalLogoBg loads it as an 8bpp affine BG2
    # (mode 1), keeping each map entry's low byte; 5 entries have a stray
    # high byte (62d0cc_universal_logo_high.json).
    ("62d0cc_universal_logo", 0x0861C1FC, 0x0862D0CC, 0x086315BC, 30, 20, 8, True),
    ("62e3b0_title_screen", 0x0861C224, 0x0862E3B0, 0x0863183C, 16, 16, 8, True),
    # The title's OBJ sprites (TitleScreen::LoadObjTiles copies one tile
    # per map entry to OBJ VRAM, by the entry's low byte; it ignores the
    # flip bits the maps have on a few symmetric-looking cells).
    ("62c2c8_title_bandicoot_obj", 0x0861C15C, 0x0862C2C8, 0x08631374, 4, 6, 4, False),
    ("62c4b0_title_crash_obj", 0x0861C184, 0x0862C4B0, 0x086313B0, 8, 40, 4, False),
    ("62cc3c_title_arrow1_obj", 0x0861C1AC, 0x0862CC3C, 0x086314F0, 4, 16, 4, False),
    ("62ce70_title_arrow2_obj", 0x0861C1D4, 0x0862CE70, 0x08631558, 4, 16, 4, False),
]
OUT_DIR = Path("graphics/bg_package")


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
            sys.exit("bg_map: %s: the tile set has identical or unused tiles, "
                     "which one picture can't rebuild" % name)
        high = {i: e >> 8 for i, e in enumerate(entries) if affine and e >> 8}
        if high:
            lines = ['    "%d": "%02X"' % (i, b) for i, b in high.items()]
            open(OUT_DIR / (name + "_high.json"), "w").write("{\n" + ",\n".join(lines) + "\n}\n")


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
    entries = [e & ~0x3FF | order[e & 0x3FF] for e in entries]
    if args.high:
        for cell, byte in json.load(open(args.high)).items():
            i, byte = int(cell), int(byte, 16)
            if not 0 <= i < len(entries) or entries[i] >> 8 or not 0 < byte <= 0xFF:
                sys.exit("bg_map: %s: can't give cell %s the high byte %#x"
                         % (args.high, cell, byte))
            entries[i] |= byte << 8
    write_u16(args.out_map, entries)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    sub = ap.add_subparsers(dest="mode", required=True)
    p = sub.add_parser("first-use")
    p.add_argument("tiles")
    p.add_argument("map")
    p.add_argument("out_tiles")
    p.add_argument("out_map")
    p.add_argument("--bpp", type=int, choices=(4, 8), required=True)
    p.add_argument("--high", help="JSON of an affine map's nonzero high bytes")
    p.set_defaults(fn=first_use)
    p = sub.add_parser("extract")
    p.add_argument("rom", nargs="?", default="baserom.gba")
    p.set_defaults(fn=extract)
    args = ap.parse_args()
    args.fn(args)


if __name__ == "__main__":
    main()
