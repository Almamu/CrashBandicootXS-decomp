#!/usr/bin/env python3
"""N-Tranced's progress, as a decompilati.onl progress report.

N-Tranced has no objdiff report yet: its code is a disassembly
(games/n2/asm/) with the shared sources of games/n2/objects.txt linked in
place of their part of it, and its data is the original's bytes. So its
progress is what the build says (docs/n_tranced.md, "Progress"):

  code.total     the code's bytes: the .text of every code object
  code.matching  the shared sources' .text: built from src/, byte-identical
  data.total     the rest of the ROM (ntranced.gba's size less the code)
  data.done      0: all of it is still incbin'd from baserom_n2.gba

The areas are the shared sources (one each) and the disassembly (one per
games/n2/asm/ file). Run it after `make GAME=n2`; it prints the JSON
document decompilati.onl's webhook takes (https://decompilati.onl/docs).

Usage:
  games/n2/progress.py [--version NAME] [--commit SHA] [--message TEXT]
"""

import argparse
import json
import os
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
N2_DIR = os.path.join(ROOT, "games", "n2")
OBJ_DIR = os.path.join(ROOT, "build", "ntranced")
ROM = os.path.join(ROOT, "ntranced.gba")
SIZE = os.environ.get("SIZE", "arm-none-eabi-size")


def text_size(obj):
    """The .text size of an object, from `size -A`."""
    out = subprocess.run([SIZE, "-A", obj], check=True, capture_output=True, text=True).stdout
    for line in out.splitlines():
        parts = line.split()
        if parts and parts[0] == ".text":
            return int(parts[1])
    return 0


def shared_sources():
    """(source, its object) for each line of objects.txt, in ROM order."""
    out = []
    with open(os.path.join(N2_DIR, "objects.txt")) as f:
        for line in f:
            fields = line.split("#", 1)[0].split()
            if fields:
                src = fields[2]
                out.append((src, os.path.join(OBJ_DIR, os.path.splitext(src)[0] + ".o")))
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--version", default="europe", help="the game version (default: europe)")
    ap.add_argument("--commit", help="commit hash shown as the last change")
    ap.add_argument("--message", help="commit message shown as the last change")
    args = ap.parse_args()

    for path in [ROM] + [o for _, o in shared_sources()]:
        if not os.path.exists(path):
            sys.exit("progress.py: %s is missing; build N-Tranced first (make GAME=n2)" % path)

    shared = []
    for src, obj in shared_sources():
        size = text_size(obj)
        shared.append({"name": os.path.basename(src), "description": src,
                       "code": {"total": size, "matching": size, "non_matching": 0}})
    asm_dir = os.path.join(N2_DIR, "asm")
    disasm = []
    for name in sorted(os.listdir(asm_dir)):
        if name.endswith(".s"):
            size = text_size(os.path.join(OBJ_DIR, "games", "n2", "asm", name[:-2] + ".o"))
            disasm.append({"name": name, "description": "games/n2/asm/" + name,
                           "code": {"total": size, "matching": 0, "non_matching": 0}})

    def total(areas, key):
        return sum(a["code"][key] for a in areas)

    code = {k: total(shared, k) + total(disasm, k) for k in ("total", "matching", "non_matching")}
    data_total = os.path.getsize(ROM) - code["total"]
    areas = [
        {"name": "Shared with Crash XS", "description": "games/n2/objects.txt: sources built from src/",
         "code": {k: total(shared, k) for k in code}, "children": shared},
        {"name": "Disassembly", "description": "games/n2/asm/: the code still to decompile",
         "code": {k: total(disasm, k) for k in code}, "children": disasm},
        # the map sizes tiles by code, and the data has none: draw it as big
        # as all the code (it is 20 times bigger)
        {"name": "ROM data", "description": "incbin'd from baserom_n2.gba",
         "data": {"total": data_total, "done": 0}, "weight": code["total"]},
    ]
    doc = {"version": args.version, "unit": "bytes", "code": code,
           "data": {"total": data_total, "done": 0}, "layout": "treemap", "areas": areas}
    if args.commit:
        doc["commit"] = args.commit
    if args.message:
        doc["message"] = args.message
    json.dump(doc, sys.stdout, indent=1)
    print()
    return 0


if __name__ == "__main__":
    sys.exit(main())
