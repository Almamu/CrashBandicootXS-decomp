"""tools/text.py: the command line of crashdata.text (#1167)."""
import argparse
import os

from crashdata import text


def main():
    ap = argparse.ArgumentParser(description=text.__doc__.split("\n\n")[0])
    ap.add_argument("--game", choices=["xs", "n2"], default="xs")
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("cpp")
    p.add_argument("table", choices=["ui_text", "cutscene_text_english", "cutscene_text_translations",
                                     "credits_text"])
    p.add_argument("out")
    p.add_argument("--mo", default=None, help="the directory of the compiled <lang>.mo files")
    sub.add_parser("check")
    p = sub.add_parser("extract")
    p.add_argument("rom", nargs="?", default=None)
    p.add_argument("--check", action="store_true")
    args = ap.parse_args()
    text.use_game(args.game)
    if args.cmd == "extract" and args.rom is None:
        args.rom = os.path.join(text.ROOT, "baserom.gba" if text.GAME == "xs" else "baserom_n2.gba")
    if args.cmd == "cpp" and args.table not in text.TABLES:
        text.fail("%s has no table %s" % (text.GAME_TITLE, args.table))
    if args.cmd == "cpp":
        if args.table not in ("cutscene_text_english", "credits_text") and not args.mo:
            text.fail("cpp %s needs --mo" % args.table)
        text.write_lines(args.out, text.generate(args.table, args.mo))
    elif args.cmd == "check":
        text.check()
    else:
        text.extract(args.rom, args.check)


if __name__ == "__main__":
    main()
