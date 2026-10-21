#!/usr/bin/env python3
"""N-Tranced's progress, as a decompilati.onl or objdiff progress report.

N-Tranced has no objdiff report yet: its code is a disassembly
(games/n2/asm/) with the shared sources of games/n2/objects.txt linked in
place of their part of it, and its data is the original's bytes. So its
progress is what the build says (docs/n_tranced.md, "Progress"), in the
same terms as XS's report:

  code.total         the code's bytes, 0x08000000 to the end of the code
  code.matching      the shared sources' (built from engine/src/, byte-identical)
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

With --objdiff it prints the same progress as an objdiff report instead
(objdiff's Report JSON, as `objdiff-cli report generate` writes it), the
ntranced_europe_report artifact that the progress sites' GitHub Apps
import like XS's: the areas are its categories, and its units are the
shared sources (one per source and category, complete), the disassembly
(one per category), the data objects (complete) and the incbin'd data.

Usage:
  games/n2/progress.py [--version NAME] [--commit SHA] [--message TEXT]
  games/n2/progress.py --objdiff
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


def measure():
    """What the build says: (functions, data objects, code size, data size).
    A function is (address, name, size, category id or None, the
    objects.txt source it's built from or None for the disassembly); a data
    object is (start, end, source)."""
    shared = shared_ranges()
    code_end = CODE_END
    funcs = sorted((a, n) for n, a in functions(ELF).items() if CODE_START <= a < code_end)
    cats = xs_categories()
    starts = [s for s, _, _ in shared]
    out = []
    for i, (addr, name) in enumerate(funcs):
        size = (funcs[i + 1][0] if i + 1 < len(funcs) else code_end) - addr
        k = bisect.bisect_right(starts, addr) - 1
        src = shared[k][2] if k >= 0 and addr < shared[k][1] else None
        out.append((addr, name, size, cats.get(name), src))
    code_total = code_end - CODE_START
    data_total = os.path.getsize(ROM) - code_total
    data_objs = [(a, e, src) for a, e, src in shared if a >= code_end]
    return out, data_objs, code_total, data_total


def category_name(cat):
    if cat is None:
        return UNIDENTIFIED
    return report_units.CATEGORY_NAMES.get(cat, cat.capitalize())


def webhook(args, funcs, data_objs, code_total, data_total):
    """decompilati.onl's webhook document."""
    areas = collections.OrderedDict()
    total_matching = matched_funcs = 0
    for _, _, size, cat, src in funcs:
        a = areas.setdefault(category_name(cat), {"code": collections.Counter(), "functions": collections.Counter()})
        a["code"]["total"] += size
        a["functions"]["total"] += 1
        if src:
            a["code"]["matching"] += size
            a["functions"]["matching"] += 1
            total_matching += size
            matched_funcs += 1

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
    return doc


def percent(part, total):
    return 100.0 * part / total if total else 100.0


class Measures:
    """objdiff's Measures (objdiff-core/protos/report.proto). What matches
    is linked from its source and the rest is disassembly or incbin'd, so
    matched is complete (linked) and nothing matches in part."""

    def __init__(self, code=0, matched_code=0, data=0, matched_data=0, funcs=0, matched_funcs=0,
                 units=0, complete_units=0):
        self.code, self.matched_code = code, matched_code
        self.data, self.matched_data = data, matched_data
        self.funcs, self.matched_funcs = funcs, matched_funcs
        self.units, self.complete_units = units, complete_units

    def add(self, other):
        for k in vars(self):
            setattr(self, k, getattr(self, k) + getattr(other, k))

    def json(self):
        # proto3's JSON mapping, as objdiff-cli writes it: 64-bit integers
        # as strings, zero values left out
        m = {"fuzzy_match_percent": percent(self.matched_code, self.code),
             "total_code": str(self.code), "matched_code": str(self.matched_code),
             "matched_code_percent": percent(self.matched_code, self.code),
             "total_data": str(self.data), "matched_data": str(self.matched_data),
             "matched_data_percent": percent(self.matched_data, self.data),
             "total_functions": self.funcs, "matched_functions": self.matched_funcs,
             "matched_functions_percent": percent(self.matched_funcs, self.funcs),
             "complete_code": str(self.matched_code),
             "complete_code_percent": percent(self.matched_code, self.code),
             "complete_data": str(self.matched_data),
             "complete_data_percent": percent(self.matched_data, self.data),
             "total_units": self.units, "complete_units": self.complete_units}
        return {k: v for k, v in m.items() if v not in (0, "0")}


# the categories after XS's, in this order
LAST_CATEGORIES = [("unidentified", UNIDENTIFIED), ("data_sources", "Data built from sources"),
                   ("rom_data", "ROM data")]


def objdiff(funcs, data_objs, code_total, data_total):
    """The objdiff report. Its categories are the webhook's areas, and its
    units are: each shared source (per category, if its functions are in
    several), the disassembly of each category, each data object and the
    incbin'd data."""
    groups = collections.OrderedDict()  # (category id, source) -> its functions
    for f in funcs:
        groups.setdefault((f[3] or "unidentified", f[4]), []).append(f)
    xs_cats = sorted({c for c, _ in groups} - {"unidentified"}, key=category_name)
    names = collections.OrderedDict((c, category_name(c)) for c in xs_cats)
    names.update(LAST_CATEGORIES)
    cats = {c: Measures() for c in names}
    sources = collections.Counter(src for _, src in groups if src)
    total = Measures()
    units = []

    def unit(name, cat, m, sections, functions=None, source=None):
        m.units, m.complete_units = 1, int(m.matched_code + m.matched_data == m.code + m.data)
        cats[cat].add(m)
        total.add(m)
        u = {"name": name, "measures": m.json(), "sections": sections}
        if functions:
            u["functions"] = functions
        u["metadata"] = {"complete": bool(m.complete_units), "progress_categories": [cat]}
        if source:
            u["metadata"]["source_path"] = source
        units.append(u)

    def item(name, size, done, addr):
        it = {"name": name, "size": str(size)}
        if done:
            it["fuzzy_match_percent"] = 100.0
        it["metadata"] = {"virtual_address": str(addr)}
        return it

    for (cat, src), fs in sorted(groups.items(), key=lambda kv: kv[1][0][0]):
        size = sum(f[2] for f in fs)
        if src:
            name = os.path.splitext(src)[0]
            if sources[src] > 1:
                name += " (%s)" % names[cat]
            m = Measures(code=size, matched_code=size, funcs=len(fs), matched_funcs=len(fs))
        else:
            name = "asm/" + cat
            m = Measures(code=size, funcs=len(fs))
        unit(name, cat, m, [item(".text", size, src, fs[0][0])],
             [item(n, s, src, a) for a, n, s, _, _ in fs], src)
    for a, e, src in data_objs:
        unit(os.path.splitext(src)[0], "data_sources",
             Measures(data=e - a, matched_data=e - a), [item(".rodata", e - a, True, a)], source=src)
    rest = data_total - sum(e - a for a, e, _ in data_objs)
    unit("baserom_n2.gba", "rom_data", Measures(data=rest),
         [item(".rodata", rest, False, CODE_START + code_total)])
    assert (total.code, total.data) == (code_total, data_total)
    return {"measures": total.json(), "units": units, "version": 2,
            "categories": [{"id": c, "name": n, "measures": cats[c].json()} for c, n in names.items()]}


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--version", default="europe", help="the game version (default: europe)")
    ap.add_argument("--commit", help="commit hash shown as the last change")
    ap.add_argument("--message", help="commit message shown as the last change")
    ap.add_argument("--objdiff", action="store_true",
                    help="print an objdiff report (Report JSON) instead of the webhook document")
    args = ap.parse_args()
    for path in (ROM, ELF):
        if not os.path.exists(path):
            sys.exit("progress.py: %s is missing; build N-Tranced first (make GAME=n2)" % path)

    measured = measure()
    doc = objdiff(*measured) if args.objdiff else webhook(args, *measured)
    json.dump(doc, sys.stdout, indent=1)
    print()
    return 0


if __name__ == "__main__":
    sys.exit(main())
