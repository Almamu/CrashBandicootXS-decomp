"""tools/sprite_banks.py: the command line of crashdata.graphics.sprite_banks (#1167)."""
import argparse

from crashdata.graphics import sprite_banks


def main():
    ap = argparse.ArgumentParser(description=sprite_banks.__doc__.split("\n\n")[0])
    ap.add_argument("--graphics-dir", default="games/xs/graphics")
    ap.add_argument("rom", nargs="?", default=str(sprite_banks.ROOT / "baserom.gba"))
    args = ap.parse_args()
    sprite_banks.extract(args.rom, args.graphics_dir)


if __name__ == "__main__":
    main()
