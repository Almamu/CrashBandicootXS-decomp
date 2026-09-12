#!/usr/bin/env python3
# Converts between a raw "framed" OBJ sprite sheet entity (as used by the
# category-descriptor animation system in code_3.s - see docs/graphics.md,
# "Found the real per-actor animation-frame system") and a folder of
# individual, per-frame indexed PNGs - one file per frame, no grid, no
# sidecar metadata file.
#
# Each frame in the raw entity is:
#   4-byte header {w_tiles, h_tiles, 0x10, 0x00} + w_tiles*h_tiles*32 bytes
#   of standard swizzled 4bpp tile data (row-major within the frame)
#
# gbagfx can't be used directly (the header bytes aren't pixels), so -
# like linear_gfx.py for Mode 4 bitmaps - this bypasses gbagfx and does
# the conversion directly. A frame's own PNG is sized to exactly
# w_tiles*8 x h_tiles*8 pixels, so its tile dimensions are always
# self-describing (png.width // 8, png.height // 8) - no sidecar needed.
# Frame order is filename sort order (00.png, 01.png, ...), which is why
# every frame folder uses zero-padded numeric names.

import glob
import os
import sys

from PIL import Image

PAD2 = 0x10
PAD3 = 0x00


def decode_tile(data, offset):
    tile = [[0] * 8 for _ in range(8)]
    for row in range(8):
        for col_byte in range(4):
            b = data[offset + row * 4 + col_byte]
            tile[row][col_byte * 2] = b & 0xF
            tile[row][col_byte * 2 + 1] = (b >> 4) & 0xF
    return tile


def encode_tile(pixels):
    out = bytearray(32)
    for row in range(8):
        for col_byte in range(4):
            lo = pixels[row][col_byte * 2] & 0xF
            hi = pixels[row][col_byte * 2 + 1] & 0xF
            out[row * 4 + col_byte] = lo | (hi << 4)
    return bytes(out)


def parse_frames(raw):
    """Split a raw entity into a list of (w_tiles, h_tiles, tile_data) frames."""
    frames = []
    pos = 0
    n = len(raw)
    while pos < n:
        if pos + 4 > n:
            raise SystemExit(f"truncated header at offset {pos}")
        w, h, pad2, pad3 = raw[pos], raw[pos + 1], raw[pos + 2], raw[pos + 3]
        if pad2 != PAD2 or pad3 != PAD3:
            raise SystemExit(f"unexpected pad bytes {pad2:#x},{pad3:#x} at offset {pos}")
        size = w * h * 32
        pos += 4
        if pos + size > n:
            raise SystemExit(f"frame at offset {pos - 4} overruns buffer end")
        frames.append((w, h, raw[pos:pos + size]))
        pos += size
    return frames


def load_palette(pal_path):
    with open(pal_path, 'rb') as f:
        pal_raw = f.read()
    palette = []
    for i in range(16):
        lo = pal_raw[i * 2]
        hi = pal_raw[i * 2 + 1]
        c = lo | (hi << 8)
        r = (c & 0x1F) * 255 // 31
        g = ((c >> 5) & 0x1F) * 255 // 31
        b = ((c >> 10) & 0x1F) * 255 // 31
        palette += [r, g, b]
    return palette


def to_frames(bin_path, out_dir, pal_path):
    with open(bin_path, 'rb') as f:
        raw = f.read()
    frames = parse_frames(raw)
    palette = load_palette(pal_path)

    os.makedirs(out_dir, exist_ok=True)
    digits = max(2, len(str(len(frames) - 1)))
    for idx, (w, h, tile_data) in enumerate(frames):
        img = Image.new('P', (w * 8, h * 8))
        img.putpalette(palette)
        pixels = img.load()
        for t in range(w * h):
            tile = decode_tile(tile_data, t * 32)
            tx = t % w
            ty = t // w
            for y in range(8):
                for x in range(8):
                    pixels[tx * 8 + x, ty * 8 + y] = tile[y][x]
        img.save(os.path.join(out_dir, f'{idx:0{digits}d}.png'))


def to_bin_folder(in_dir, bin_path):
    paths = sorted(glob.glob(os.path.join(in_dir, '*.png')))
    if not paths:
        raise SystemExit(f"{in_dir}: no .png files found")

    out = bytearray()
    for path in paths:
        img = Image.open(path)
        if img.mode != 'P':
            raise SystemExit(f"{path}: expected an indexed (palette) PNG")
        width, height = img.size
        if width % 8 or height % 8:
            raise SystemExit(f"{path}: {width}x{height} isn't a multiple of 8x8")
        w, h = width // 8, height // 8
        if w not in (1, 2, 4, 8) or h not in (1, 2, 4, 8):
            raise SystemExit(f"{path}: {w}x{h} tiles isn't a valid frame size")

        pixels = img.load()
        out += bytes([w, h, PAD2, PAD3])
        for t in range(w * h):
            tx = t % w
            ty = t // w
            tile = [[0] * 8 for _ in range(8)]
            for y in range(8):
                for x in range(8):
                    tile[y][x] = pixels[tx * 8 + x, ty * 8 + y] & 0xF
            out += encode_tile(tile)

    with open(bin_path, 'wb') as f:
        f.write(out)


def main(argv):
    if len(argv) < 2:
        raise SystemExit(
            f"Usage: {argv[0]} to-frames IN.bin OUT_DIR PAL.bin\n"
            f"       {argv[0]} to-bin-folder IN_DIR OUT.bin")
    cmd = argv[1]
    if cmd == 'to-frames':
        to_frames(argv[2], argv[3], argv[4])
    elif cmd == 'to-bin-folder':
        to_bin_folder(argv[2], argv[3])
    else:
        raise SystemExit(f"Unknown command: {cmd}")


if __name__ == '__main__':
    main(sys.argv)
