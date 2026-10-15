"""tools/apply_mod.py: the command line of crashdata.mods (#1167)."""
import argparse
import os
import runpy
import shutil
import sys
from pathlib import Path

from crashdata import mods
from crashdata.common import diagnostics as D


def run():
    ap = argparse.ArgumentParser(description=mods.__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("mod", help="the mod's folder, e.g. mods/sample")
    ap.add_argument("dest", help="where to put the modded copy of the tree (must not exist)")
    ap.add_argument("--json", action="store_true",
                    help="print the errors and warnings as one JSON report (docs/editors.md)")
    args = ap.parse_args()
    mod, dest = Path(args.mod).resolve(), Path(args.dest).resolve()
    if not (mod / "mod.py").exists():
        sys.exit("apply_mod: %s has no mod.py" % mod)
    mods.copy_tree(dest)
    files = mod / "files"
    if files.is_dir():
        shutil.copytree(files, dest, dirs_exist_ok=True)
    # the mod runs in the copy, with the copy's tools importable
    os.chdir(dest)
    sys.path.insert(0, str(dest / "tools"))
    runpy.run_path(str(mod / "mod.py"))["apply"](mods.Tree(dest, mod))
    print("apply_mod: %s applied to %s; build it there with `make`" % (mod.name, dest))


def main():
    """run(), or with --json its errors and warnings as one JSON report
    (crashdata.common.diagnostics, docs/editors.md)."""
    return D.json_cli('apply_mod', run)


if __name__ == "__main__":
    main()
