"""tools/rle_sprites.py: the command line of crashdata.graphics.rle_sprites (#1167)."""
import argparse

from crashdata.graphics import rle_sprites


def main():
    ap = argparse.ArgumentParser(description=rle_sprites.__doc__.split("\n\n")[0])
    sub = ap.add_subparsers(dest="mode", required=True)
    pk = sub.add_parser("pack")
    pk.add_argument("input")
    pk.add_argument("output")
    pk.add_argument("header")
    pk.add_argument("--frame", required=True)
    pk.add_argument("--name", required=True)
    args = ap.parse_args()
    rle_sprites.pack(args)


if __name__ == "__main__":
    main()
