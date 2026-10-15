"""tools/obj_sheet.py: the command line of crashdata.graphics.obj_sheet (#1167)."""
import argparse

from crashdata.graphics import obj_sheet


def main():
    ap = argparse.ArgumentParser(description=obj_sheet.__doc__.split("\n\n")[0])
    sub = ap.add_subparsers(dest="mode", required=True)
    for name, fn in (("to-tiles", obj_sheet.to_tiles), ("from-tiles", obj_sheet.from_tiles)):
        p = sub.add_parser(name)
        p.add_argument("src")
        p.add_argument("dst")
        p.add_argument("--obj", action="append", required=True)
        p.add_argument("--columns", type=int)
        p.add_argument("--cell")
        p.add_argument("--at", action="append")
        if fn is obj_sheet.to_tiles:
            p.add_argument("--width", type=int, default=1)
        p.set_defaults(fn=fn)
    args = ap.parse_args()
    args.fn(args)


if __name__ == "__main__":
    main()
