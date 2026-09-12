#!/usr/bin/env python3
# Converts between a raw "framed" OBJ sprite sheet (as used by the
# category-descriptor animation system in code_3.s - see docs/graphics.md,
# "Found the real per-actor animation-frame system") and a viewable/editable
# indexed PNG. Each sheet is a back-to-back sequence of frames:
#
#   4-byte header {w_tiles, h_tiles, 0x10, 0x00} + w_tiles*h_tiles*32 bytes
#   of standard swizzled 4bpp tile data (row-major within the frame)
#
# gbagfx can't touch this directly (the header bytes aren't pixels, and
# frames can vary in size within one sheet), so - like linear_gfx.py for
# Mode 4 bitmaps - this bypasses gbagfx and does the conversion directly.
#
# Frame sizes are stored in a sidecar ".frames" text file next to the PNG:
# first line is the grid column count, then one "w h" line (in tiles) per
# frame, in original file order. Frames are laid out left-to-right/top-to-
# bottom in a grid of cells sized to the largest frame in the sheet, each
# frame's real pixels anchored at the cell's top-left corner - the rest of
# an under-sized cell is filled with palette index 0 (every sheet uses
# index 0 as its transparent/background color already).

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
    """Split a raw sheet into a list of (w_tiles, h_tiles, tile_data) frames."""
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


def to_png(bin_path, png_path, frames_path, pal_path, cols=None):
    with open(bin_path, 'rb') as f:
        raw = f.read()
    frames = parse_frames(raw)

    max_w = max(w for w, h, _ in frames)
    max_h = max(h for w, h, _ in frames)
    if cols is None:
        cols = min(len(frames), 8)
    rows = (len(frames) + cols - 1) // cols

    cell_w = max_w * 8
    cell_h = max_h * 8
    img = Image.new('P', (cell_w * cols, cell_h * rows), 0)
    pixels = img.load()

    for idx, (w, h, tile_data) in enumerate(frames):
        gx = (idx % cols) * cell_w
        gy = (idx // cols) * cell_h
        for t in range(w * h):
            tile = decode_tile(tile_data, t * 32)
            tx = t % w
            ty = t // w
            for y in range(8):
                for x in range(8):
                    pixels[gx + tx * 8 + x, gy + ty * 8 + y] = tile[y][x]

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
    img.putpalette(palette)
    img.save(png_path)

    with open(frames_path, 'w') as f:
        f.write(f"{cols}\n")
        for w, h, _ in frames:
            f.write(f"{w} {h}\n")


def to_bin(png_path, frames_path, bin_path):
    img = Image.open(png_path)
    if img.mode != 'P':
        raise SystemExit(f"{png_path}: expected an indexed (palette) PNG")
    width, height = img.size

    with open(frames_path) as f:
        lines = [line.split() for line in f if line.strip()]
    cols = int(lines[0][0])
    frame_sizes = [(int(w), int(h)) for w, h in lines[1:]]

    max_w = max(w for w, h in frame_sizes)
    max_h = max(h for w, h in frame_sizes)
    cell_w = max_w * 8
    cell_h = max_h * 8
    expected_cols_w = cell_w * cols
    rows = (len(frame_sizes) + cols - 1) // cols
    if width != expected_cols_w or height != cell_h * rows:
        raise SystemExit(
            f"{png_path}: size {width}x{height} doesn't match expected "
            f"{expected_cols_w}x{cell_h * rows} for {len(frame_sizes)} frames, "
            f"{cols} cols, cell {cell_w}x{cell_h}")

    pixels = img.load()
    out = bytearray()
    for idx, (w, h) in enumerate(frame_sizes):
        gx = (idx % cols) * cell_w
        gy = (idx // cols) * cell_h
        out += bytes([w, h, PAD2, PAD3])
        for t in range(w * h):
            tx = t % w
            ty = t // w
            tile = [[0] * 8 for _ in range(8)]
            for y in range(8):
                for x in range(8):
                    tile[y][x] = pixels[gx + tx * 8 + x, gy + ty * 8 + y] & 0xF
            out += encode_tile(tile)

    with open(bin_path, 'wb') as f:
        f.write(out)


def main(argv):
    if len(argv) < 2:
        raise SystemExit(
            f"Usage: {argv[0]} to-png IN.bin OUT.png OUT.frames PAL.bin [COLS]\n"
            f"       {argv[0]} to-bin IN.png IN.frames OUT.bin")
    cmd = argv[1]
    if cmd == 'to-png':
        cols = int(argv[6]) if len(argv) > 6 else None
        to_png(argv[2], argv[3], argv[4], argv[5], cols)
    elif cmd == 'to-bin':
        to_bin(argv[2], argv[3], argv[4])
    else:
        raise SystemExit(f"Unknown command: {cmd}")


if __name__ == '__main__':
    main(sys.argv)
