"""tools/boss_pictures.py: the command line of crashdata.graphics.boss_pictures (#1167)."""
import argparse

from crashdata.graphics import boss_pictures


def main():
    ap = argparse.ArgumentParser(description=boss_pictures.__doc__.split("\n\n")[0])
    sub = ap.add_subparsers(dest="mode", required=True)
    pk = sub.add_parser("pack")
    pk.add_argument("output")
    pk.add_argument("header")
    pk.add_argument("--name", required=True)
    pk.add_argument("--cols", type=int, required=True)
    pk.add_argument("tiles", nargs="+")
    args = ap.parse_args()
    boss_pictures.pack(args)


if __name__ == "__main__":
    main()
