"""tools/data_check.py: `make data-check`, every check of the data without building (#1167).

    tools/data_check.py [--game xs|n2] [--no-text] [--json]

Checks the game's data in this tree (crashdata.datacheck, docs/editors.md):
every file against its JSON Schema, then the model's checks. Prints each
problem as `FILE: POINTER: MESSAGE` and `data-check: <game> OK` (or the
error count), or with --json one report (docs/editors.md, "Errors").
Exit status 1 on an error.
"""
import argparse
import json
import os
import sys

from crashdata import datacheck
from crashdata.common import diagnostics as D
from crashdata.common.game import ROOT


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--game", choices=["xs", "n2"], default="xs")
    ap.add_argument("--no-text", action="store_true", help="skip the text check (it needs gettext)")
    ap.add_argument("--json", action="store_true", help="print one JSON report instead of text")
    args = ap.parse_args()
    os.chdir(ROOT)
    tool = "data-check"
    # the checks' own output (msgfmt's statistics, ...) isn't the report's
    with D.capture_fd_stdout() as printed:
        diags = datacheck.check_game(args.game, ".", text=not args.no_text)
    errors = [d for d in diags if d.severity == D.ERROR]
    if args.json:
        print(json.dumps(D.report(tool, diags, printed.text), indent=1))
    else:
        for d in diags:
            print(d.text(), file=sys.stderr if d.severity == D.ERROR else sys.stdout)
        if errors:
            print("data-check: %s: %d error%s" % (args.game, len(errors), "" if len(errors) == 1 else "s"),
                  file=sys.stderr)
        else:
            print("data-check: %s OK" % args.game)
    return 1 if errors else 0


if __name__ == "__main__":
    sys.exit(main())
