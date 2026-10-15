"""tools/cutscenes.py: the command line of crashdata.cutscenes (#1167)."""
import sys

from crashdata import cutscenes as C
from crashdata.common import diagnostics as D


def run():
    args = sys.argv[1:]
    if not args or args[0] in ("-h", "--help"):
        print(C.__doc__)
        return
    cutscenes, slides, slide_index, pages, orders, pictures = C.load()
    if args[0] == "cpp" and len(args) == 3 and args[1] in ("cutscenes", "cutscene_slides"):
        if args[1] == "cutscenes":
            lines = C.cutscenes_cpp(cutscenes, slides, slide_index, pages, orders)
        else:
            lines = C.cutscene_slides_cpp(cutscenes, slides, slide_index, pages, orders, pictures)
        C.text.write_lines(args[2], lines)
    elif args[0] == "constants" and len(args) == 2:
        C.write_if_changed(args[1], C.constants_h(cutscenes))
    elif args[0] == "check" and len(args) == 1:
        print("cutscenes: OK")
    else:
        C.fail("usage: see tools/cutscenes.py --help")


def main():
    """run(), or with --json its errors and warnings as one JSON report
    (crashdata.common.diagnostics, docs/editors.md)."""
    return D.json_cli('cutscenes', run)


if __name__ == "__main__":
    main()
