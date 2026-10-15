#!/usr/bin/env python3
"""Turn the progress report into a decompilati.onl progress report.

decompilati.onl (https://decompilati.onl/docs) takes a JSON document
POSTed to its webhook: code and data totals in bytes, and an optional
tree of areas for the project's map. This reads the objdiff progress
report (`make NON_MATCHING=1 report`, then `objdiff-cli report generate
-o report.json`, docs/progress_report.md) and the objdiff.json `make
report` wrote, and prints that document.

The areas follow the source tree: Engine and Game by namespace
(src/engine/<ns>/, src/game/<ns>/), the libraries under lib/, and the
sound data. A code unit is placed by the object tools/report_units.py
builds its base from; a data unit by the source its name was made from.

  code.matching      objdiff's matched_code
  code.non_matching  code with C that doesn't match: the bytes of the
                     units with a base object, less what matches (the
                     same split as tools/badges.py)
  data.done          objdiff's matched_data

Usage:
  tools/progress_api.py [--report report.json] [--objdiff objdiff.json]
                        [--commit SHA] [--message TEXT]
"""

import argparse
import json
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import report_units  # noqa: E402

TOP_NAMES = {"engine": "Engine", "game": "Game", "lib": "Libraries", "sound": "Sound data",
             "asm": "Assembly"}
AREA_NAMES = {
    # src/engine/
    "sys": "System", "gfx": "Graphics", "actor": "Actors", "audio": "Audio",
    "save": "Save", "link": "Link cable", "text": "Text", "util": "Util",
    # src/game/
    "crash": "Crash", "entities": "Entities", "enemies": "Enemies", "bosses": "Bosses",
    "vehicles": "Vehicles", "crates": "Crates", "objects": "Objects", "level": "Level",
    "ui": "UI",
    # lib/
    "gax": "GAX2 sound engine", "agb_eeprom": "AgbEeprom", "libgcc": "libgcc",
    "libagbsyscall": "BIOS calls",
}
DESCRIPTIONS = {
    "Engine": "src/engine/: the engine, by namespace",
    "Game": "src/game/: the game, by namespace",
    "Libraries": "lib/: the third-party and SDK code linked into the ROM",
    "Sound data": "sound/: the GAX2 music and sound-effect data",
    "Assembly": "asm/: itoa_arm, assembly by owner decision (#662)",
}


def code_paths():
    """Report unit name -> the object its base is built from."""
    paths = {}
    for start, base_rel, category in report_units.UNITS + report_units.IWRAM_UNITS:
        if base_rel and base_rel != report_units.HANDWRITTEN:
            paths[os.path.splitext(os.path.basename(base_rel))[0]] = base_rel
    return paths


def area_path(parts):
    """(top, area) for a source path split on '/'."""
    if parts[0] == "src" and len(parts) > 2:
        return TOP_NAMES[parts[1]], AREA_NAMES.get(parts[2], parts[2])
    if parts[0] == "lib" and len(parts) > 1:
        return TOP_NAMES["lib"], AREA_NAMES.get(parts[1], parts[1])
    if parts[0] in TOP_NAMES:
        return TOP_NAMES[parts[0]], None
    raise SystemExit(f"progress_api.py: no area for {'/'.join(parts)}")


def data_parts(name):
    """The source path tokens of a data unit's name (data_<group>_<ADDR>,
    the group with '/' turned into '_', report_units.data_units()). The
    namespace names are single words, so the first tokens are exact."""
    tokens = name.split("_")[1:-1]
    if tokens[:1] == ["src"]:
        return tokens[:3]
    if tokens[:1] == ["lib"]:
        rest = "_".join(tokens[1:])
        for lib in sorted(AREA_NAMES, key=len, reverse=True):
            if rest == lib or rest.startswith(lib + "_"):
                return ["lib", lib]
    return tokens[:1]


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--report", default=os.path.join(ROOT, "report.json"))
    ap.add_argument("--objdiff", default=os.path.join(ROOT, "objdiff.json"))
    ap.add_argument("--commit", help="commit hash shown as the last change")
    ap.add_argument("--message", help="commit message shown as the last change")
    args = ap.parse_args()

    with open(args.report) as f:
        report = json.load(f)
    with open(args.objdiff) as f:
        has_base = {u["name"] for u in json.load(f)["units"] if u.get("base_path")}
    paths = code_paths()

    # top -> area -> {"code": [total, matching, non_matching], "data": [total, done]}
    tree = {}
    for unit in report["units"]:
        m = unit["measures"]
        name = unit["name"]
        if "data" in unit.get("metadata", {}).get("progress_categories", []):
            parts = data_parts(name)
        elif name in paths:
            parts = paths[name].split("/")
        else:
            raise SystemExit(f"progress_api.py: report unit {name} has no source")
        top, area = area_path(parts)
        node = tree.setdefault(top, {}).setdefault(area, {"code": [0, 0, 0], "data": [0, 0]})
        total, matched = int(m.get("total_code", 0)), int(m.get("matched_code", 0))
        node["code"][0] += total
        node["code"][1] += matched
        if name in has_base:
            node["code"][2] += total - matched
        node["data"][0] += int(m.get("total_data", 0))
        node["data"][1] += int(m.get("matched_data", 0))

    def measures(code, data):
        out = {}
        if code[0]:
            out["code"] = {"total": code[0], "matching": code[1], "non_matching": code[2]}
        if data[0]:
            out["data"] = {"total": data[0], "done": data[1]}
        return out

    totals = report["measures"]
    total_code, total_data = int(totals.get("total_code", 0)), int(totals.get("total_data", 0))

    def weighted(entry, code, data):
        # The map sizes a tile by its code; give a data-only area (the
        # sound data) its share of the data, at the code's scale.
        if not code[0] and data[0] and total_data:
            entry["weight"] = max(1, data[0] * total_code // total_data)
        return entry

    areas = []
    for top in sorted(tree, key=list(TOP_NAMES.values()).index):
        children = []
        code, data = [0, 0, 0], [0, 0]
        for area, node in sorted(tree[top].items(), key=lambda kv: kv[0] or ""):
            code = [a + b for a, b in zip(code, node["code"])]
            data = [a + b for a, b in zip(data, node["data"])]
            if area is not None:
                children.append(weighted({"name": area, **measures(node["code"], node["data"])},
                                         node["code"], node["data"]))
        entry = weighted({"name": top, "description": DESCRIPTIONS[top], **measures(code, data)},
                         code, data)
        if children:
            entry["children"] = children
        areas.append(entry)

    code = [total_code, int(totals.get("matched_code", 0))]
    code.append(sum(sum(n["code"][2] for n in t.values()) for t in tree.values()))
    data = [total_data, int(totals.get("matched_data", 0))]
    for i, (want, have) in enumerate(((code[0], sum(a.get("code", {}).get("total", 0) for a in areas)),
                                      (data[0], sum(a.get("data", {}).get("total", 0) for a in areas)))):
        if want != have:
            raise SystemExit(f"progress_api.py: the areas' {'code' if i == 0 else 'data'} "
                             f"({have}) doesn't add up to the report's ({want})")

    doc = {"unit": "bytes", **measures(code, data), "layout": "treemap", "areas": areas}
    if args.commit:
        doc["commit"] = args.commit
    if args.message:
        doc["message"] = args.message
    json.dump(doc, sys.stdout, indent=1)
    print()
    return 0


if __name__ == "__main__":
    sys.exit(main())
