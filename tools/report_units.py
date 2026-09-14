#!/usr/bin/env python3
"""Builds one objdiff unit per matched src/*.c file (instead of one giant
merged unit) and writes objdiff.json with each tagged by category, so
decomp.dev can show per-category progress (Graphics/Util/System - see
docs/decomp_dev.md for why category units have to be this fine-grained:
merging non-adjacent functions into one target reintroduces the same
"hidden function" boundary bug fixed for mem_collect, since ld -r-merged
objects lose the original per-file section boundaries objdiff's size
inference relies on).

Addresses below come from a clean `make compare` (NON_MATCHING=0) build's
crashbandicootxs.map - the only reliable source, since NON_MATCHING=1
addresses drift downstream of any parked function (its imperfect
reconstruction is a different byte count than the real ROM). Four files
contain a parked function and so have a wider range here than their
NON_MATCHING=0 object alone shows - the parked function's real bytes
currently live in the neighboring raw asm/*.s chunk instead, but
expected/code_3.s already has it labelled at its true address (it was
never extracted, only parked), so slicing still works unmodified.

Still-raw regions (nothing here) become uncategorized units: their
target is the frozen source slice for that range, with no base_path, so
they count toward the overall total but not any specific category -
see docs/decomp_dev.md for what's deliberately not categorized yet.
"""
import json
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
BUILD_DIR = ROOT / "build" / "expected" / "units"
CODE3 = ROOT / "expected" / "code_3.s"
LEGACY = ROOT / "expected" / "legacy.s"
CORRECTIONS = ROOT / "expected" / "corrections.txt"
CODE3_START = 0x080006A8  # legacy.s covers up to here (exclusive)

AS = ["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork"]
LD = ["arm-none-eabi-ld", "-r"]

# (start_address, base_object_relpath_or_None, category_or_None)
# base_object is relative to build/crashbandicootxs/, matching the
# NON_MATCHING=1 report build (see Makefile's `report` target) - that's
# the object already compiled with parked functions included as C.
# Sorted by address; each entry's range runs to the next entry's start.
UNITS = [
    (0x08000170, "src/system/main.o", "system"),
    (0x080001CC, "src/system/memory.o", "system"),
    (0x08000544, "src/system/irq.o", "system"),
    (0x080007EC, None, None),  # code_3_1.o (sub_80007EC), still raw
    (0x080008B4, "src/util/math_util.o", "util"),
    (0x0800094C, "src/util/string_util.o", "util"),
    (0x08000AA8, "src/util/printf_util.o", "util"),  # incl. parked sub_8000CBC
    (0x08000D68, "src/util/string_util2.o", "util"),
    (0x08000E10, "src/util/rand_util.o", "util"),
    (0x08000E6C, "src/util/line_util.o", "util"),
    (0x08000EE4, "src/graphics/text_layout.o", "graphics"),  # entirely parked
    (0x0800106C, "src/util/time_util.o", "util"),
    (0x080010E0, "src/system/input_util.o", "system"),  # entirely parked
    (0x08001174, "src/system/asset_util.o", "system"),
    (0x080011F4, "src/util/word_util.o", "util"),
    (0x08001254, "src/util/line_util2.o", "util"),
    (0x080012AC, "src/graphics/fade_util.o", "graphics"),
    (0x080013FC, "src/graphics/palette_blend.o", "graphics"),
    (0x080014A4, None, None),  # code_3_1_7.o, still raw (up to sub_8006600)
    (0x08006600, "src/graphics/oam_count.o", "graphics"),  # incl. parked sub_8006600
    (0x0800697C, "src/graphics/graphics.o", "graphics"),
    (0x08006C00, None, None),  # code_3_2.o, still raw (includes the audio engine - not carved out yet, see docs/decomp_dev.md)
    (0x0803B058, "src/graphics/actor_anim.o", "graphics"),
    (0x0803B060, None, None),  # code_3_3.o, still raw
    (0x0803B8B0, None, None),  # sentinel end address, not a real unit
]


def run(cmd, **kwargs):
    subprocess.run(cmd, check=True, **kwargs)


def slice_source(source, start, end):
    args = ["python3", str(ROOT / "tools" / "slice_expected.py"), str(source), f"{start:x}"]
    if end is not None:
        args.append(f"{end:x}")
    return subprocess.run(args, capture_output=True, text=True, check=True).stdout


def build_target(name, start, end):
    """Assembles the frozen-source slice(s) covering [start, end) into
    build/expected/units/<name>_target.o, applying corrections.txt."""
    out_o = BUILD_DIR / f"{name}_target.o"
    if start < CODE3_START and (end is None or end <= CODE3_START):
        text = slice_source(LEGACY, start, end)
        out_s = BUILD_DIR / f"{name}_target.s"
        out_s.write_text(text)
        run(AS + ["-o", str(out_o), str(out_s)])
    elif start >= CODE3_START:
        text = slice_source(CODE3, start, end)
        out_s = BUILD_DIR / f"{name}_target.s"
        out_s.write_text(text)
        run(AS + ["-o", str(out_o), str(out_s)])
    else:
        # Straddles both frozen sources (only irq.c does) - slice each
        # half separately, then merge.
        legacy_text = slice_source(LEGACY, start, CODE3_START)
        code3_text = slice_source(CODE3, CODE3_START, end)
        legacy_s = BUILD_DIR / f"{name}_legacy.s"
        code3_s = BUILD_DIR / f"{name}_code3.s"
        legacy_s.write_text(legacy_text)
        code3_s.write_text(code3_text)
        legacy_o = BUILD_DIR / f"{name}_legacy.o"
        code3_o = BUILD_DIR / f"{name}_code3.o"
        run(AS + ["-o", str(legacy_o), str(legacy_s)])
        run(AS + ["-o", str(code3_o), str(code3_s)])
        run(LD + ["-o", str(out_o), str(legacy_o), str(code3_o)])

    run(["python3", str(ROOT / "tools" / "patch_expected_target.py"), str(CORRECTIONS), str(out_o)])
    return out_o


def main():
    BUILD_DIR.mkdir(parents=True, exist_ok=True)
    categories = {}
    units = []

    for i in range(len(UNITS) - 1):
        start, base_rel, category = UNITS[i]
        end = UNITS[i + 1][0]
        if base_rel is None:
            name = f"raw_{start:08X}"
        else:
            name = Path(base_rel).stem

        target_o = build_target(name, start, end)
        unit = {
            "name": name,
            "target_path": str(target_o.relative_to(ROOT)),
        }
        if base_rel is not None:
            unit["base_path"] = f"build/crashbandicootxs/{base_rel}"
        if category is not None:
            unit["metadata"] = {"progress_categories": [category]}
            categories[category] = category.capitalize()
        units.append(unit)

    objdiff = {
        "$schema": "https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json",
        "build_target": False,
        "build_base": False,
        "progress_categories": [{"id": cid, "name": name} for cid, name in categories.items()],
        "units": units,
    }
    (ROOT / "objdiff.json").write_text(json.dumps(objdiff, indent=4) + "\n")
    print(f"Wrote objdiff.json with {len(units)} units, {len(categories)} categories")


if __name__ == "__main__":
    main()
