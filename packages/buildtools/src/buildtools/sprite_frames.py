"""tools/sprite_frames.py: the command line of crashdata.graphics.sprite_frames (#1167)."""
import argparse

from crashdata.graphics import sprite_frames


def main():
    ap = argparse.ArgumentParser(description=sprite_frames.__doc__.split("\n\n")[0])
    ap.add_argument("--graphics-dir", default=str(sprite_frames.XS_GRAPHICS))
    sub = ap.add_subparsers(dest="mode", required=True)
    p = sub.add_parser("extract")
    p.add_argument("rom", nargs="?", default="baserom.gba")
    p = sub.add_parser("pack")
    p.add_argument("outdir")
    args = ap.parse_args()
    sprite_frames.use_graphics_dir(args.graphics_dir)
    if args.mode == "extract":
        sprite_frames.extract(args.rom)
    else:
        sprite_frames.write_pack(sprite_frames.pack(sprite_frames.bank_dirs()), args.outdir)


if __name__ == "__main__":
    main()
