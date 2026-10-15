"""tools/bg_map.py: the command line of crashdata.graphics.bg_map (#1167)."""
import argparse

from crashdata.graphics import bg_map


def main():
    ap = argparse.ArgumentParser(description=bg_map.__doc__.split("\n\n")[0])
    sub = ap.add_subparsers(dest="mode", required=True)
    p = sub.add_parser("first-use")
    p.add_argument("tiles")
    p.add_argument("map")
    p.add_argument("out_tiles")
    p.add_argument("out_map")
    p.add_argument("--bpp", type=int, choices=(4, 8), required=True)
    p.add_argument("--high", help="JSON of an affine map's nonzero high bytes")
    p.set_defaults(fn=bg_map.first_use)
    args = ap.parse_args()
    args.fn(args)


if __name__ == "__main__":
    main()
