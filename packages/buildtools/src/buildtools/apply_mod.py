"""tools/apply_mod.py: the command line of crashdata.mods (#1167)."""
import argparse
import os
import runpy
import shutil
import sys
from pathlib import Path

from crashdata import mods


def main():
    ap = argparse.ArgumentParser(description=mods.__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("mod", help="the mod's folder, e.g. mods/sample")
    ap.add_argument("dest", help="where to put the modded copy of the tree (must not exist)")
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


if __name__ == "__main__":
    main()
