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

Still-raw regions with no `base_object` can still carry a `category` -
docs/rom_map.md's whole-ROM reconnaissance pass split most of the ROM's
remaining raw stretches into game_loop/actor/graphics_loading/audio/hud/
overlay_ui by address, high-confidence enough (individually-read
functions, confirmed landmarks, or a dominant connected component) to be
worth a decomp.dev progress category even at 0% matched - these units
get `metadata.progress_categories` but no `base_path`, so they count
toward that category's total (all unmatched) without affecting per-file
match percentages. Two categories rom_map.md found - `menu_ui` and `fx` -
don't get their own address boundary: both are individual functions
scattered *inside* another category's contiguous range rather than a
separate block of their own (menu_ui's dispatch-table functions sit
inside graphics_loading's `LoadGraphicsPackage` cluster; fx's two-function
particle-queue pair sits inside the hud gap after `MainLoop`), so
splitting them out here would require a fake, unjustified address cut -
see docs/rom_map.md for exactly which functions are which; nothing here
should be read as more precise than "the dominant category in this
range." A handful of ranges have a similar, smaller-scale mix (a
confirmed SIO/link-cable subsystem inside `overlay_ui`'s span; a couple
of generic division-routine false positives inside the GAX2 span) -
flagged inline below, folded into the dominant category regardless.
Genuinely untouched regions (nothing in rom_map.md, too small to matter)
stay `None`/uncategorized, same as before.
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
    (0x080014A4, None, "graphics"),  # fade/screen-mode utility cluster (13 fns/332B), calls matched palette_blend.c - docs/rom_map.md "A fourth thing in this file"
    (0x080015E0, None, "overlay_ui"),  # pause-menu/dialog dominant component (118 fns/19.2KB); a smaller SIO/link-cable subsystem (sub_8001F50/sub_8001DB4/...) is interleaved throughout this same span, not separable by address - folded into overlay_ui regardless, see docs/rom_map.md
    (0x08006600, "src/graphics/oam_count.o", "graphics"),  # incl. parked sub_8006600
    (0x0800697C, "src/graphics/graphics.o", "graphics"),
    (0x08006C00, None, "game_loop"),  # 94.4 KB main zone, confirmed one cohesive system - docs/rom_map.md "Two big unnamed systems"
    (0x0801E578, None, "graphics_loading"),  # LoadGraphicsPackage cluster (16.2KB); menu_ui's ~9.1KB dispatch-table functions and the trigger-effect spawner family are interleaved inside this same range, not a separate block - see docs/rom_map.md "Major correction: there is no second table"
    (0x080225A0, None, "game_loop"),  # UpdateGameFrame-MainLoop cluster (18,764B exact), confirmed same system/signature as the 0x08006C00 zone, not a separate island
    (0x08026EEC, None, "hud"),  # ~4.8 of 5.9KB is HUD stat-widgets; ~0.3KB (fx's particle/trajectory-queue pair) and ~0.8KB unlabeled remainder are interleaved inside this same span - docs/rom_map.md "fx wasn't right either"
    (0x0802866C, None, "hud"),  # InitHudIconWidgetA/B, MeasureText, UploadHudTile, InitHudTextWidget
    (0x08028BA0, None, "graphics_loading"),  # InitObjTileFreeList, LoadSpriteFrameTiles, SetupSpriteFrameOam, DecompressCategorySpriteSheet
    (0x080291A4, None, "actor"),  # SetupActorVramPool, InitActorCategory, SelectActorCategory, InitActorPart, UpdateAnimatedActorPart, ConstructAnimTableState, ConstructActorPart
    (0x0802B364, None, "actor"),  # 40.4 KB actor zone (docs/rom_map.md cites 0x0802B348, 28B before the nearest real function start - snapped forward since 0x0802B348 itself falls mid-function) - category/part/vtable system, boss-candidate + singleton object clusters
    (0x080354E0, None, "graphics_loading"),  # LoadLevelGraphics, LoadBg2Background, LoadObjSpriteTiles
    (0x08037110, None, "audio"),  # Shin'en GAX2 engine, boundary narrowed this session to end at 0x0803A944; a couple of generic 64-bit-division helpers are confirmed interleaved false positives - see docs/audio.md
    (0x0803A944, None, "system"),  # BIOS svc wrapper stubs + LZ77UnCompWrapper/RLUnCompWrapper, confirmed non-audio via matched asset_util.c callers
    (0x0803B058, "src/graphics/actor_anim.o", "graphics"),
    (0x0803B060, None, "actor"),  # GetAnimFrameData + 43 unnamed neighbors, medium confidence
    (0x0803B8B0, None, None),  # sentinel end address, not a real unit
]

# Display names for progress_categories - report_units.py-only categories
# (game_loop/actor/... don't mirror a src/ directory the way graphics/util/
# system do, so `category.capitalize()` alone would read oddly).
CATEGORY_NAMES = {
    "system": "System",
    "util": "Util",
    "graphics": "Graphics",
    "game_loop": "Game Loop",
    "actor": "Actor",
    "graphics_loading": "Graphics Loading",
    "audio": "Audio (GAX2)",
    "hud": "HUD",
    "overlay_ui": "Overlay UI",
}


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
            categories[category] = CATEGORY_NAMES.get(category, category.capitalize())
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
