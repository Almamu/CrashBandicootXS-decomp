#!/usr/bin/env python3
"""N-Tranced's progress, as a decompilati.onl progress report.

N-Tranced has no objdiff report yet: its code is a disassembly
(games/n2/asm/) with the shared sources of games/n2/objects.txt linked in
place of their part of it, and its data is the original's bytes. So its
progress is what the build says (docs/n_tranced.md, "Progress"), in the
same terms as XS's report:

  code.total         the code's bytes, 0x08000000 to the end of the code
  code.matching      the shared sources' (built from src/, byte-identical)
  functions.total    every function in ntranced.elf
  functions.matching the shared sources' functions
  data.total         the rest of the ROM (ntranced.gba's size less the code)
  data.done          the data objects' (objects.txt's entries after the
                     code: data built from sources, such as the level
                     assets); the rest is still incbin'd from baserom_n2.gba

The areas are XS's report categories (System, Graphics, Actor, ...,
tools/report_units.py). A function gets its category from the XS function
of the same name: the disassembly names a function after XS's where the
XS/N-Tranced comparison found it (games/n2/gbadisasm.cfg). The rest,
N-Tranced's own code and what isn't identified yet, are "Not identified
yet". This needs the XS build too (crashbandicootxs.elf, for the XS
functions' addresses); without it every function is in that one area.
The data is two areas: what is built from sources (one child per data
object) and the incbin'd rest.
Run it after `make GAME=n2`; it prints the JSON document decompilati.onl's
webhook takes (https://decompilati.onl/docs).

Usage:
  games/n2/progress.py [--version NAME] [--commit SHA] [--message TEXT]
"""

import argparse
import bisect
import collections
import json
import os
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
N2_DIR = os.path.join(ROOT, "games", "n2")
ROM = os.path.join(ROOT, "ntranced.gba")
ELF = os.path.join(ROOT, "ntranced.elf")
XS_ELF = os.path.join(ROOT, "crashbandicootxs.elf")
READELF = os.environ.get("READELF", "arm-none-eabi-readelf")
CODE_START = 0x08000000
UNIDENTIFIED = "Not identified yet"

sys.path.insert(0, os.path.join(ROOT, "tools"))
sys.path.insert(0, N2_DIR)
import report_units  # noqa: E402  (XS's report categories)
from disasm import CODE_END  # noqa: E402  (where the incbin'd data starts)


def functions(elf):
    """{name: address} of the ELF's function symbols (STT_FUNC)."""
    out = subprocess.run([READELF, "-sW", elf], check=True, capture_output=True, text=True).stdout
    funcs = {}
    for line in out.splitlines():
        parts = line.split()
        if len(parts) >= 8 and parts[3] == "FUNC" and parts[6] != "UND":
            funcs.setdefault(parts[7], int(parts[1], 16) & ~1)
    return funcs


def shared_ranges():
    """(start, end, source) of each line of objects.txt."""
    out = []
    with open(os.path.join(N2_DIR, "objects.txt")) as f:
        for line in f:
            fields = line.split("#", 1)[0].split()
            if fields:
                start, size = int(fields[0], 16), int(fields[1], 16)
                out.append((start, start + size, fields[2]))
    return out


def xs_categories():
    """XS function name -> its report category id, or {} without the XS build."""
    if not os.path.exists(XS_ELF):
        return {}
    units = [(a, c) for a, _, c in report_units.UNITS if a >= CODE_START]
    starts = [a for a, _ in units]
    out = {}
    for name, addr in functions(XS_ELF).items():
        i = bisect.bisect_right(starts, addr) - 1
        if i >= 0 and units[i][1]:
            out[name] = units[i][1]
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--version", default="europe", help="the game version (default: europe)")
    ap.add_argument("--commit", help="commit hash shown as the last change")
    ap.add_argument("--message", help="commit message shown as the last change")
    args = ap.parse_args()
    for path in (ROM, ELF):
        if not os.path.exists(path):
            sys.exit("progress.py: %s is missing; build N-Tranced first (make GAME=n2)" % path)

    shared = shared_ranges()
    code_end = CODE_END
    funcs = sorted((a, n) for n, a in functions(ELF).items() if CODE_START <= a < code_end)

    cats = xs_categories()
    starts = [s for s, _, _ in shared]
    areas = collections.OrderedDict()
    total_matching = matched_funcs = 0
    for i, (addr, name) in enumerate(funcs):
        size = (funcs[i + 1][0] if i + 1 < len(funcs) else code_end) - addr
        k = bisect.bisect_right(starts, addr) - 1
        matching = k >= 0 and addr < shared[k][1]
        cat = report_units.CATEGORY_NAMES.get(cats[name], cats[name].capitalize()) if name in cats else UNIDENTIFIED
        a = areas.setdefault(cat, {"code": collections.Counter(), "functions": collections.Counter()})
        a["code"]["total"] += size
        a["functions"]["total"] += 1
        if matching:
            a["code"]["matching"] += size
            a["functions"]["matching"] += 1
            total_matching += size
            matched_funcs += 1

    code_total = code_end - CODE_START
    data_total = os.path.getsize(ROM) - code_total
    data_objs = [(a, e, src) for a, e, src in shared if a >= code_end]
    data_done = sum(e - a for a, e, _ in data_objs)
    area_list = []
    for name in sorted(areas, key=lambda n: (n == UNIDENTIFIED, n)):
        c = areas[name]["code"]
        f = areas[name]["functions"]
        area_list.append({"name": name,
                          "description": "%d of %d functions matching" % (f["matching"], f["total"]),
                          "code": {"total": c["total"], "matching": c["matching"], "non_matching": 0}})
    # the map sizes tiles by code, and the data has none: draw it as big as
    # all the code (it is 20 times bigger)
    if data_objs:
        area_list.append({"name": "Data built from sources",
                          "description": "games/n2/objects.txt's data objects",
                          "data": {"total": data_done, "done": data_done},
                          "children": [{"name": os.path.basename(src), "description": src,
                                        "data": {"total": e - a, "done": e - a}} for a, e, src in data_objs]})
    area_list.append({"name": "ROM data", "description": "incbin'd from baserom_n2.gba",
                      "data": {"total": data_total - data_done, "done": 0}, "weight": code_total})
    doc = {"version": args.version, "unit": "bytes",
           "code": {"total": code_total, "matching": total_matching, "non_matching": 0},
           "data": {"total": data_total, "done": data_done},
           "functions": {"total": len(funcs), "matching": matched_funcs},
           "layout": "treemap", "areas": area_list}
    if args.commit:
        doc["commit"] = args.commit
    if args.message:
        doc["message"] = args.message
    json.dump(doc, sys.stdout, indent=1)
    print()
    return 0


if __name__ == "__main__":
    sys.exit(main())
