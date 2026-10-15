"""tools/jasc_pal.py: the command line of crashdata.common.jasc_pal (#1167)."""
import struct
import sys

from crashdata.common import jasc_pal


def main():
    args = sys.argv[1:]
    colors = None
    if len(args) == 5 and args[0] == "png-to-gba" and args[3] == "--colors":
        colors = int(args[4])
        args = args[:3]
    if len(args) != 3 or args[0] not in ("to-gba", "to-jasc", "png-to-gba", "gba-to-png"):
        sys.exit(jasc_pal.__doc__)
    mode, src, dst = args
    if mode in ("to-gba", "png-to-gba"):
        entries = jasc_pal.read_jasc(src) if mode == "to-gba" else jasc_pal.read_png(src)
        if colors is not None and len(entries) != colors:
            sys.exit("jasc_pal: %s: a palette of %d colours, expected %d (keep the PNG's palette at "
                     "%d entries, see docs/graphics.md \"Palettes\")" % (src, len(entries), colors, colors))
        open(dst, "wb").write(struct.pack("<%dH" % len(entries), *entries))
    elif mode == "to-jasc":
        jasc_pal.write_jasc(dst, jasc_pal.read_gba(src))
    else:
        jasc_pal.write_png(dst, jasc_pal.read_gba(src))


if __name__ == "__main__":
    main()
