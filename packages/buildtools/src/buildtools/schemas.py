"""tools/schemas.py: the data files' JSON Schemas, from crashdata (#1167).

    tools/schemas.py write [DIR]      write them to DIR (packages/crashdata/schemas/)
    tools/schemas.py --check [DIR]    fail if DIR's differ from what the model gives
    tools/schemas.py validate [--game xs|n2] [FILE ...]
                                      validate the game's data files (or FILEs)
                                      against them

Each takes --json for one JSON report instead of text (docs/editors.md).
"""
import sys
from pathlib import Path

from crashdata import schemas
from crashdata.common import diagnostics as D
from crashdata.common.game import Game


def run():
    args = sys.argv[1:]
    if not args or args[0] in ("-h", "--help"):
        print(__doc__)
        return 0
    cmd = args.pop(0)
    if cmd == "write":
        out = Path(args[0]) if args else schemas.SCHEMAS_DIR
        names = schemas.write(out)
        print("schemas: wrote %d files to %s" % (len(names), out))
        return 0
    if cmd == "--check":
        diags = schemas.check(Path(args[0]) if args else schemas.SCHEMAS_DIR)
        if diags:
            raise D.DataError(diags)
        print("schemas: OK")
        return 0
    if cmd == "validate":
        game = "xs"
        if "--game" in args:
            i = args.index("--game")
            game = args[i + 1]
            del args[i:i + 2]
        diags = []
        if args:
            for f in args:
                name = schemas.schema_for(f)
                if name is None:
                    diags.append(D.Diagnostic("no schema describes this file", f, code="no-schema"))
                else:
                    diags += schemas.validate_file(f, name)
        else:
            diags = schemas.validate_game(Game(game))
        if diags:
            raise D.DataError(diags)
        print("schemas: %s valid" % (", ".join(args) if args else game))
        return 0
    sys.exit("usage: see tools/schemas.py --help")


def main():
    return D.json_cli("schemas", run)


if __name__ == "__main__":
    sys.exit(main())
