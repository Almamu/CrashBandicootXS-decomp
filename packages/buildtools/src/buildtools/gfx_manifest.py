"""tools/gfx_manifest.py: the command line of crashdata.graphics.manifest (#1167)."""
import argparse
import sys
from pathlib import Path

from crashdata.graphics import manifest


def main():
    ap = argparse.ArgumentParser(description=manifest.__doc__.split("\n\n")[0])
    ap.add_argument("--graphics-dir", default=str(manifest.XS_GRAPHICS),
                    help="the game's graphics directory (XS's games/xs/graphics/ by default)")
    sub = ap.add_subparsers(dest="mode", required=True)
    sub.add_parser("check")
    p = sub.add_parser("extract")
    p.add_argument("rom", nargs="?", default="baserom.gba")
    p.add_argument("--check", action="store_true")
    sub.add_parser("table")
    p = sub.add_parser("header")
    p.add_argument("out")
    p = sub.add_parser("defs")
    p.add_argument("object")
    p.add_argument("out")
    sub.add_parser("symbols")
    p = sub.add_parser("make")
    p.add_argument("out")
    args = ap.parse_args()
    manifest.use_graphics_dir(args.graphics_dir)
    if not manifest.is_xs() and args.mode in ("table", "symbols"):
        sys.exit("gfx_manifest: %s is XS's only (docs/data_map.md, cxx_symbols.txt)" % args.mode)
    if args.mode in ("header", "defs", "symbols", "make"):
        if args.mode == "header":
            manifest.write_if_changed(args.out, manifest.header())
        elif args.mode == "make":
            Path(args.out).parent.mkdir(parents=True, exist_ok=True)
            Path(args.out).write_text(manifest.make_fragment())
        elif args.mode == "defs":
            Path(args.out).parent.mkdir(parents=True, exist_ok=True)
            Path(args.out).write_text(manifest.defs(args.object))
        else:
            manifest.write_symbols()
        return
    if args.mode == "check":
        errors = manifest.check()
    elif args.mode == "extract" and not manifest.is_xs():
        errors = manifest.extract_game(args.rom, args.check)
    elif args.mode == "extract" and args.check:
        errors = manifest.extract_check(args.rom)
    elif args.mode == "extract":
        manifest.extract(args.rom, Path("."))
        errors = []
    else:
        manifest.table()
        errors = []
    if errors:
        sys.exit("\n".join(errors[:50] + (["... and %d more" % (len(errors) - 50)] if len(errors) > 50 else [])))
    if args.mode != "table":
        print("gfx_manifest: OK")


if __name__ == "__main__":
    main()
