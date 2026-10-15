"""tools/framed_gfx.py: the command line of crashdata.graphics.framed_gfx (#1167)."""
import sys

from crashdata.graphics import framed_gfx


def main(argv=None):
    argv = sys.argv if argv is None else argv
    if len(argv) < 2:
        raise SystemExit(
            f"Usage: {argv[0]} to-frames IN.bin OUT_DIR PAL.bin\n"
            f"       {argv[0]} to-bin-folder IN_DIR OUT.bin [--any-size]\n"
            f"       {argv[0]} header SHEET_DIR OUT.h --name SHEET\n"
            f"       {argv[0]} order SHEET_DIR")
    cmd = argv[1]
    if cmd == 'to-frames':
        framed_gfx.to_frames(argv[2], argv[3], argv[4])
    elif cmd == 'to-bin-folder':
        framed_gfx.to_bin_folder(argv[2], argv[3], argv[4:5] == ['--any-size'])
    elif cmd == 'header':
        if argv[4:5] != ['--name'] or len(argv) != 6:
            raise SystemExit(f"Usage: {argv[0]} header SHEET_DIR OUT.h --name SHEET")
        framed_gfx.write_header(argv[2], argv[3], argv[5])
    elif cmd == 'order':
        print(" ".join(framed_gfx.entity_order(argv[2])))
    else:
        raise SystemExit(f"Unknown command: {cmd}")


if __name__ == '__main__':
    main()
