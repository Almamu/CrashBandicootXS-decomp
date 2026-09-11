#!/usr/bin/env python3
# Converts between a raw linear (non-tiled) indexed framebuffer, as used by
# GBA Mode 4 bitmaps, and a viewable indexed PNG. gbagfx always arranges
# pixel data into 8x8 tiles, which is wrong for these full-screen bitmaps,
# so this tool keeps scanline order intact instead.

import sys

from PIL import Image

GRAYSCALE_PALETTE = [v for i in range(256) for v in (i, i, i)]


def to_bin(png_path, bin_path):
    img = Image.open(png_path)
    if img.mode != 'P':
        raise SystemExit(f"{png_path}: expected an indexed (palette) PNG")
    with open(bin_path, 'wb') as f:
        f.write(bytes(img.getdata()))


def to_png(bin_path, png_path, width, height):
    with open(bin_path, 'rb') as f:
        raw = f.read()
    expected = width * height
    if len(raw) != expected:
        raise SystemExit(f"{bin_path}: expected {expected} bytes for {width}x{height}, got {len(raw)}")
    img = Image.new('P', (width, height))
    img.putpalette(GRAYSCALE_PALETTE)
    img.putdata(raw)
    img.save(png_path)


def main(argv):
    if len(argv) < 3:
        raise SystemExit(f"Usage: {argv[0]} to-bin IN.png OUT.bin\n"
                          f"       {argv[0]} to-png IN.bin OUT.png WIDTH HEIGHT")
    cmd = argv[1]
    if cmd == 'to-bin':
        to_bin(argv[2], argv[3])
    elif cmd == 'to-png':
        to_png(argv[2], argv[3], int(argv[4]), int(argv[5]))
    else:
        raise SystemExit(f"Unknown command: {cmd}")


if __name__ == '__main__':
    main(sys.argv)
