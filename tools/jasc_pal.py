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
        r, g, b = (c >> 3 for c in rgb)
        bit15 = (rgb[0] ^ standard(r)) & 1
        out.append(bit15 << 15 | b << 10 | g << 5 | r)
    return out


def write_jasc(path, entries):
    lines = ["JASC-PAL", "0100", str(len(entries))]
    for v in entries:
        r, g, b = v & 31, v >> 5 & 31, v >> 10 & 31
        lines.append("%d %d %d" % (standard(r) ^ (v >> 15), standard(g), standard(b)))
    open(path, "wb").write(("\r\n".join(lines) + "\r\n").encode("ascii"))


def main():
    if len(sys.argv) != 4 or sys.argv[1] not in ("to-gba", "to-jasc"):
        sys.exit(__doc__)
    mode, src, dst = sys.argv[1:]
    if mode == "to-gba":
        entries = read_jasc(src)
        open(dst, "wb").write(struct.pack("<%dH" % len(entries), *entries))
    else:
        data = open(src, "rb").read()
        if len(data) % 2:
            sys.exit("jasc_pal: %s: odd size" % src)
        write_jasc(dst, struct.unpack("<%dH" % (len(data) // 2), data))


if __name__ == "__main__":
    main()
