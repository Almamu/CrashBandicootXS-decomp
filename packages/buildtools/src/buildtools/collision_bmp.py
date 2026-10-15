"""tools/collision_bmp.py: the command line of crashdata.levels.collision (#1167)."""
import argparse
import struct
import sys
from pathlib import Path

from crashdata.levels import collision


def main():
    ap = argparse.ArgumentParser(description=collision.__doc__.split("\n\n")[0])
    ap.add_argument("mode", choices=("to-image", "to-bin"))
    ap.add_argument("input")
    ap.add_argument("output")
    ap.add_argument("--width", type=int, help="map width in cells (to-image)")
    args = ap.parse_args()
    if args.mode == "to-image":
        if not args.width:
            sys.exit("collision_bmp: to-image needs --width")
        data = Path(args.input).read_bytes()
        collision.write_image(args.output, args.width, list(struct.unpack("<%dH" % (len(data) // 2), data)))
    else:
        _, _, cells = collision.read_image(args.input)
        Path(args.output).write_bytes(struct.pack("<%dH" % len(cells), *cells))


if __name__ == "__main__":
    main()
