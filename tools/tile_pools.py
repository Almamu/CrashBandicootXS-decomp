#!/usr/bin/env python3
"""Extracts the raw tile pools inside gStaticData_0817E78C (0x0817E78C-0x084A5600)
from baserom.gba as indexed PNGs, the editable sources that graphics.mk
rebuilds with grit (see docs/data.md, "Resources (grit-style)"):

- graphics/sprites/bankNN_<addr>.png: the 56 sprite banks' OBJ tiles (4bpp).
  The gStaticData_084A5600 header's tileBase (0x082BF120) is the start of one
  headerless pool; each bank's frames address it as tileBase +
  (frame.packed & 0xFFFFFF), and the banks own disjoint, back-to-back ranges
  in bank order (banks 42 and 47 also reuse one frame of bank 0's). A bank's
  range runs from its lowest own frame offset to its highest frame end.
- graphics/sprites/tile_pool_4a4660.png: the 125 fixed 4bpp tiles
  (header +0x08, the sub_8004D74 tile-asset cache).
- graphics/level_tilesets/tilesetN_<addr>.png: the five tag-0x00 raw level
  BG tile sets (8bpp); the 4-byte {size << 8} header is written by the C.

Each PNG is `width` tiles wide and padded to whole rows with blank (index 0)
tiles; graphics.mk passes the real byte count to `tools/bin2c.py --size`,
which trims that padding again. Sprite banks are drawn as wide as their most
common OBJ piece (by tile area, at least 4 tiles), so with the 1D OBJ tile
mapping those pieces read correctly. Their palette is a grayscale placeholder:
the palette is picked per actor at run time (OAM attribute 2), nothing ties a
bank to one. The tile sets carry the 256-colour BG palette of the first room
(in ROM order) whose layers use them, found through the room records.

    tools/tile_pools.py [baserom.gba]     writes the PNGs, prints the sizes
"""
import struct
import sys
from collections import Counter
from pathlib import Path

from PIL import Image

ROM_BASE = 0x08000000
BANK_TABLE = 0x084A5600
FIXED_POOL = 0x084A4660
TILESETS = [0x0817E7AC, 0x081E6330, 0x08200DF4, 0x08270F08, 0x08299DCC]
ROOM_BLOCKS = [(0x0824B638, 0x08270F08), (0x082B91D0, 0x082BF120)]
OBJ_W = 0x0816B2E0  # u8[12] piece widths (px) by shape/size index
OBJ_H = 0x0816B2EC  # u8[12] piece heights


class Rom:
    def __init__(self, path):
        self.data = Path(path).read_bytes()

    def w(self, a):
        return struct.unpack_from("<I", self.data, a - ROM_BASE)[0]

    def h(self, a):
        return struct.unpack_from("<H", self.data, a - ROM_BASE)[0]

    def b(self, a):
        return self.data[a - ROM_BASE]

    def bytes(self, a, n):
        return self.data[a - ROM_BASE:a - ROM_BASE + n]


def sprite_banks(rom):
    """[(start, end, piece width in tiles)] per bank, as pool offsets."""
    banks, tile_base = rom.w(BANK_TABLE), rom.w(BANK_TABLE + 4)
    nbanks = rom.h(BANK_TABLE + 12)
    assert tile_base == 0x082BF120 and nbanks == 56
    widths = rom.bytes(OBJ_W, 12)
    heights = rom.bytes(OBJ_H, 12)
    out = []
    prev_end = 0
    for i in range(nbanks):
        frames = rom.w(banks + 12 * i + 4)
        # frame_desc *frames[], directly followed by the first frame_desc
        ptrs, a, first = [], frames, None
        while a != first:
            p = rom.w(a)
            first = p if first is None else min(first, p)
            ptrs.append(p)
            a += 4
        start, end, area = None, 0, Counter()
        for p in ptrs:
            packed = rom.w(p + 8)
            off, count, ids = packed & 0xFFFFFF, packed >> 24, rom.w(p + 4)
            if off < prev_end:
                continue  # a frame reused from an earlier bank's range
            size = 0
            for k in range(count):
                shape = rom.b(ids + k) & 0xF
                tiles = widths[shape] * heights[shape] // 64
                area[widths[shape] // 8] += tiles
                size += tiles * 32
            start = off if start is None else min(start, off)
            end = max(end, off + size)
        assert start == prev_end, f"bank {i}: gap before {start:#x}"
        out.append((start, end, max(4, area.most_common(1)[0][0])))
        prev_end = end
    assert prev_end == FIXED_POOL - tile_base
    return out


def room_palette(rom, tileset):
    """The palette of the first room whose level_desc uses `tileset`:
    bg_layer_desc.tileData (+0x08) -> level_desc (layer0Data, +0x0C) ->
    room record {palette, level_desc, kind}."""
    words = {}
    for lo, hi in [(0x0816CD80, 0x0816D1F4)] + ROOM_BLOCKS:
        for a in range(lo, hi, 4):
            words.setdefault(rom.w(a), []).append(a)
    descs = []
    for x in words.get(tileset, []):
        for p in words.get(x - 8, []):
            desc = p - 0x0C
            if words.get(desc):
                descs.append((desc, words[desc][0] - 4))
    desc, record = min(descs)
    return rom.bytes(rom.w(record), 0x200)


def write_png(path, data, bpp, width, palette_rgb):
    tile_bytes = 8 * bpp
    ntiles = len(data) // tile_bytes
    assert ntiles * tile_bytes == len(data)
    rows = -(-ntiles // width)
    img = Image.new("P", (width * 8, rows * 8), 0)
    px = img.load()
    for t in range(ntiles):
        tile = data[t * tile_bytes:(t + 1) * tile_bytes]
        tx, ty = (t % width) * 8, (t // width) * 8
        for i in range(64):
            if bpp == 4:
                v = tile[i // 2] >> (4 * (i & 1)) & 0xF
            else:
                v = tile[i]
            px[tx + i % 8, ty + i // 8] = v
    flat = [c for rgb in palette_rgb for c in rgb]
    img.putpalette(flat + [0] * (768 - len(flat)))
    path.parent.mkdir(parents=True, exist_ok=True)
    img.save(path, bits=bpp) if bpp == 4 else img.save(path)
    return len(data)


def bgr555(raw):
    out = []
    for i in range(0, len(raw), 2):
        c = raw[i] | raw[i + 1] << 8
        out.append(tuple(((c >> s) & 31) * 255 // 31 for s in (0, 5, 10)))
    return out


def main():
    rom = Rom(sys.argv[1] if len(sys.argv) > 1 else "baserom.gba")
    gray = [(i * 17,) * 3 for i in range(16)]
    sizes = []
    tile_base = rom.w(BANK_TABLE + 4)
    for i, (start, end, width) in enumerate(sprite_banks(rom)):
        name = f"bank{i:02}_{(tile_base + start) & 0xFFFFFF:06x}"
        n = write_png(Path("graphics/sprites") / f"{name}.png",
                      rom.bytes(tile_base + start, end - start), 4, width, gray)
        sizes.append((name, n))
    n = write_png(Path("graphics/sprites/tile_pool_4a4660.png"),
                  rom.bytes(FIXED_POOL, 125 * 32), 4, 25, gray)
    sizes.append(("tile_pool_4a4660", n))
    for k, addr in enumerate(TILESETS, 1):
        header = rom.w(addr)
        assert header & 0xFF == 0
        name = f"tileset{k}_{addr & 0xFFFFFF:06x}"
        n = write_png(Path("graphics/level_tilesets") / f"{name}.png",
                      rom.bytes(addr + 4, header >> 8), 8, 16,
                      bgr555(room_palette(rom, addr)))
        sizes.append((name, n))
    for name, n in sizes:
        print(f"TILE_BYTES_{name} := {n:#x}")


if __name__ == "__main__":
    main()
