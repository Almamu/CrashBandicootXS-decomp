#!/usr/bin/env python3
"""Extracts the level BG tile sets from baserom.gba, the editable sources
graphics.mk rebuilds (see docs/data.md, "Resources (grit-style)"):

- graphics/level_tilesets/tilesetN_<addr>.png: the five tag-0x00 raw level
  BG0 tile sets (8bpp, inside gLanguageSelectPalette3, 0x0817E78C-
  0x084A5600); the 4-byte {size << 8} header is written by the C. Each
  carries the 256-colour BG palette of the first room (in ROM order) whose
  layers use it, found through the room records.
- graphics/level_bg_tiles/<addr>_<name>.png: the six LZ77 4bpp tile sets of
  the BG1-3 layers. Each carries the palette of the first room (in room
  table order) that uses it, and every tile is drawn in the palette bank
  the rooms' cells use it with most (the lowest of a tie): index = bank *
  16 + 4bpp pixel.

Each PNG is 16 tiles wide and padded to whole rows with blank (index 0)
tiles; graphics.mk passes the real byte count to `tools/bin2c.py --size`,
which trims that padding again. tools/gfx_manifest.py extract runs this.

    tools/tile_pools.py [baserom.gba]     writes the PNGs, prints the sizes
"""
import struct
import sys
from collections import Counter
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import levels  # noqa: E402

from PIL import Image

ROM_BASE = 0x08000000
TILESETS = [0x0817E7AC, 0x081E6330, 0x08200DF4, 0x08270F08, 0x08299DCC]
BG_TILESETS = [(0x0863D4C0, "snow"), (0x08640EF8, "jungle"), (0x08644A88, "temple"),
               (0x08649418, "sewer"), (0x0864BB5C, "space"), (0x0864D7E8, "underwater")]
ROOM_BLOCKS = [(0x0824B638, 0x08270F08), (0x082B91D0, 0x082BF120)]


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


def bg_layer_cells(rom):
    """Every BG1-3 layer of the 41 rooms, in room table order: (room
    palette address, tile set address, its cells)."""
    out = []
    for pal, desc in levels.find_rooms(rom):
        asset_addr, packed = rom.w(desc + 0x14), rom.b(desc + 0x18)
        if packed:
            asset = levels.lz77_decompress(rom.bytes(asset_addr, 0x20000))
        else:
            asset = rom.bytes(asset_addr, levels.RAW_ASSETS[asset_addr])
        for slot in range(3):
            p = rom.w(desc + 4 * slot)
            if not p:
                continue
            grid_addr, off, tiles = rom.w(p), rom.w(p + 4), rom.w(p + 8)
            cw, ch = rom.h(p + 0x16), rom.h(p + 0x18)
            grid = [rom.h(grid_addr + 2 * i) for i in range(cw * ch)]
            table = struct.unpack_from("<%dH" % (max(grid) + 1), asset, off)
            chunks = [levels.decode_chunk(asset, off + 4 * o)[0] for o in table]
            out.append((pal, tiles, [c for g in grid for c in chunks[g]]))
    return out


def bg_tiles(rom, addr, layers):
    """A BG1-3 tile set's tiles, each 64 pixels drawn in its most used bank."""
    data = levels.lz77_decompress(rom.bytes(addr, 0x10000))
    use = {}
    for _, tiles, cells in layers:
        if tiles == addr:
            for c in cells:
                use.setdefault(c & 0x3FF, Counter())[c >> 12] += 1
    out = bytearray()
    for t in range(len(data) // 32):
        cnt = use.get(t, Counter({0: 1}))
        top = max(cnt.values())
        bank = min(b for b, n in cnt.items() if n == top)
        tile = data[t * 32:(t + 1) * 32]
        out += bytes(bank * 16 + (tile[i // 2] >> 4 * (i & 1) & 0xF) for i in range(64))
    return bytes(out), len(data)


def main():
    rom = Rom(sys.argv[1] if len(sys.argv) > 1 else "baserom.gba")
    sizes = []
    layers = bg_layer_cells(levels.Rom(sys.argv[1] if len(sys.argv) > 1 else "baserom.gba"))
    for addr, name in BG_TILESETS:
        pal = next(p for p, tiles, _ in layers if tiles == addr)
        px, n = bg_tiles(rom, addr, layers)
        rgb = [((c >> s) & 31) << 3 for c in struct.unpack("<256H", rom.bytes(pal, 0x200)) for s in (0, 5, 10)]
        name = f"{addr & 0xFFFFFF:06x}_{name}"
        write_png(Path("graphics/level_bg_tiles") / f"{name}.png", px, 8, 16, list(zip(*[iter(rgb)] * 3)))
        sizes.append(("LEVEL_BG_TILE_BYTES_" + name, n))
    for k, addr in enumerate(TILESETS, 1):
        header = rom.w(addr)
        assert header & 0xFF == 0
        name = f"tileset{k}_{addr & 0xFFFFFF:06x}"
        n = write_png(Path("graphics/level_tilesets") / f"{name}.png",
                      rom.bytes(addr + 4, header >> 8), 8, 16,
                      bgr555(room_palette(rom, addr)))
        sizes.append(("TILE_BYTES_" + name, n))
    for name, n in sizes:
        print(f"{name} := {n:#x}")


if __name__ == "__main__":
    main()
