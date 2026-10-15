#!/usr/bin/env python3
"""JASC-PAL palettes <-> GBA palettes (RGB555), keeping bit 15.

A GBA palette entry is 15 bits of colour; the hardware ignores bit 15.
Some of the ROM's palettes have it set on random entries anyway (leftovers
of the original tools), so to rebuild them byte for byte a palette source
has to carry it. A JASC .pal has 8 bits per channel: a channel's 5-bit
value is its 8-bit value / 8 (as gbagfx reads it), and gbagfx writes
c * 255 / 31. This tool reads and writes the same, plus one rule:

    bit 15 is set when the red value's lowest bit differs from that of
    the standard value (c * 255 / 31) of its 5-bit red.

So a palette without bit 15 is exactly what gbagfx writes and reads, and
an entry with bit 15 shows the same colour, its red one step off (an
invisible 1/255). Colours picked freely in an editor may toggle bit 15,
which the hardware ignores. See docs/graphics.md, "Palettes".

    tools/jasc_pal.py to-gba IN.pal OUT.gbapal
    tools/jasc_pal.py to-jasc IN.gbapal OUT.pal

The same rule applies to the palette of an indexed PNG, for an asset whose
palette lives in its picture (the cutscene, menu, title, logo and
level-select pictures):

    tools/jasc_pal.py png-to-gba IN.png OUT.gbapal [--colors N]
        writes the PNG's palette (all of its entries) as a raw palette.
        With --colors, the palette must have exactly N entries: the
        palette's size in the ROM, which an editor that pads or trims
        the PNG's palette would otherwise change.
    tools/jasc_pal.py gba-to-png IN.gbapal PNG
        replaces the palette of the indexed PNG with IN, keeping its
        pixels.
"""
import struct
import sys


def standard(c):
    return c * 255 // 31


def read_jasc(path):
    lines = open(path, "rb").read().decode("ascii").split("\r\n")
    if lines[:2] != ["JASC-PAL", "0100"]:
        sys.exit("jasc_pal: %s: not a JASC-PAL file (CRLF line endings)" % path)
    n = int(lines[2])
    if not 1 <= n <= 256:
        sys.exit("jasc_pal: %s: %d colours" % (path, n))
    rows = lines[3:3 + n]
    if len(rows) != n or any(lines[3 + n:]):
        sys.exit("jasc_pal: %s: expected exactly %d colour lines" % (path, n))
    out = []
    for i, row in enumerate(rows):
        rgb = [int(x) for x in row.split(" ")]
        if len(rgb) != 3 or not all(0 <= c <= 255 for c in rgb):
            sys.exit("jasc_pal: %s: colour %d: %r" % (path, i, row))
        out.append(from_rgb(rgb))
    return out


def from_rgb(rgb):
    r, g, b = (c >> 3 for c in rgb)
    bit15 = (rgb[0] ^ standard(r)) & 1
    return bit15 << 15 | b << 10 | g << 5 | r


def to_rgb(v):
    r, g, b = v & 31, v >> 5 & 31, v >> 10 & 31
    return standard(r) ^ (v >> 15), standard(g), standard(b)


def write_jasc(path, entries):
    lines = ["JASC-PAL", "0100", str(len(entries))]
    for v in entries:
        lines.append("%d %d %d" % to_rgb(v))
    open(path, "wb").write(("\r\n".join(lines) + "\r\n").encode("ascii"))


def read_gba(path):
    data = open(path, "rb").read()
    if len(data) % 2:
        sys.exit("jasc_pal: %s: odd size" % path)
    return struct.unpack("<%dH" % (len(data) // 2), data)


def read_png(path):
    from PIL import Image

    img = Image.open(path)
    if img.mode != "P":
        sys.exit("jasc_pal: %s: not an indexed PNG" % path)
    rgb = img.getpalette("RGB")
    return [from_rgb(rgb[i:i + 3]) for i in range(0, len(rgb), 3)]


def write_png(path, entries):
    from PIL import Image

    img = Image.open(path)
    if img.mode != "P":
        sys.exit("jasc_pal: %s: not an indexed PNG" % path)
    img.load()
    img.putpalette([c for v in entries for c in to_rgb(v)])
    img.save(path)
