#!/usr/bin/env python3
"""Post-processing of grit's tile set + map output for the struct
bg_package pictures (graphics.mk's BG_PACKAGES: the menu and title
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

The packages' sources are extracted from baserom.gba by
tools/gfx_manifest.py extract (render() below), which has their addresses.
"""
import argparse
import json
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import jasc_pal  # noqa: E402


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


def render(rom, palette, tiles_a, map_a, w, h, bpp, affine):
    """A package's whole picture from the ROM (tools/gfx_manifest.py
    extract): indexed, w x h cells, with `palette` (the package's entries,
    kept as tools/jasc_pal.py writes them, bit 15 too). Returns the image
    and, for an affine map, its nonzero high bytes ({cell: byte})."""
    from PIL import Image

    tiles = split_tiles(lz77_decompress(rom, tiles_a), bpp)
    entries = struct.unpack("<%dH" % (w * h), lz77_decompress(rom, map_a))
    img = Image.new("P", (w * 8, h * 8))
    px = img.load()
    for i, e in enumerate(entries):
        draw = pixels(tiles[(e & 0xFF) if affine else (e & 0x3FF)], bpp, 0 if affine else e)
        for k, c in enumerate(draw):
            px[i % w * 8 + k % 8, i // w * 8 + k // 8] = c
    img.putpalette([c for v in palette for c in jasc_pal.to_rgb(v)])
    used = {(e & 0xFF) if affine else (e & 0x3FF) for e in entries}
    if len(set(tiles)) != len(tiles) or len(used) != len(tiles):
        sys.exit("bg_map: %#x: the tile set has identical or unused tiles, "
                 "which one picture can't rebuild" % tiles_a)
    return img, {i: e >> 8 for i, e in enumerate(entries) if affine and e >> 8}


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
    args = ap.parse_args()
    args.fn(args)


if __name__ == "__main__":
    main()
