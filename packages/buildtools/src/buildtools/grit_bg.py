"""tools/grit_bg.py: the command line of crashdata.graphics.grit_bg (#1167)."""
import argparse
import sys

from crashdata.graphics import grit_bg


def main():
    ap = argparse.ArgumentParser(description=grit_bg.__doc__.split("\n\n")[0])
    ap.add_argument("mode", choices=("map", "banks", "frames"))
    ap.add_argument("input")
    ap.add_argument("output")
    ap.add_argument("--cells", type=int)
    ap.add_argument("--frames", type=int)
    ap.add_argument("--banks", dest="banks_map")
    args = ap.parse_args()

    if args.mode == "map":
        entries = grit_bg.read_map(args.input)
        lines = ["\t" + ", ".join(f"0x{e & 0x0FFF:04X}" for e in entries[i:i + 8]) + ","
                 for i in range(0, len(entries), 8)]
        grit_bg.write(args.output, args.input, lines)
    elif args.mode == "banks":
        entries = grit_bg.read_map(args.input)
        grit_bg.write(args.output, args.input, grit_bg.byte_lines(grit_bg.pack_nibbles(e >> 12 for e in entries)))
    else:
        tiles = open(args.input, "rb").read()
        if args.frames and not args.cells:
            if len(tiles) % (32 * args.frames):
                sys.exit(f"grit_bg: {args.input}: {len(tiles) // 32} tiles is not {args.frames} frames of a size")
            args.cells = len(tiles) // 32 // args.frames
        if not args.cells:
            sys.exit("grit_bg: frames needs --cells or --frames")
        frame_size = args.cells * 32
        if len(tiles) % frame_size:
            sys.exit(f"grit_bg: {args.input}: {len(tiles)} bytes is not a whole number of {args.cells}-tile frames")
        count = len(tiles) // frame_size
        banks = None
        if args.banks_map:
            entries = grit_bg.read_map(args.banks_map)
            if len(entries) != count * args.cells:
                sys.exit(f"grit_bg: {args.banks_map}: {len(entries)} map entries for {count} frames of {args.cells} cells")
            banks = [e >> 12 for e in entries]
        lines = []
        for f in range(count):
            lines.append(f"\t/* frame {f} */ {{")
            lines.append("\t\t{")
            lines += grit_bg.byte_lines(tiles[f * frame_size:(f + 1) * frame_size], "\t\t\t")
            lines.append("\t\t},")
            if banks is not None:
                side = grit_bg.pack_nibbles(banks[f * args.cells:(f + 1) * args.cells],
                                    (args.cells + 7) // 8 * 4)
                lines.append("\t\t{")
                lines += grit_bg.byte_lines(side, "\t\t\t")
                lines.append("\t\t},")
            lines.append("\t},")
        grit_bg.write(args.output, args.input, lines)


if __name__ == "__main__":
    main()
