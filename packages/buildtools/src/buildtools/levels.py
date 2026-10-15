"""tools/levels.py: the command line of crashdata.levels (#1167)."""
import json
import sys
from pathlib import Path

from crashdata import levels as L


def main():
    args = sys.argv[1:]
    if not args or args[0] in ("-h", "--help"):
        print(L.__doc__)
        return
    if "--levels-dir" in args:
        i = args.index("--levels-dir")
        L.use_levels_dir(args[i + 1])
        del args[i:i + 2]
    cmd = args[0]
    build_dir = None
    if "--build" in args:
        i = args.index("--build")
        build_dir = args[i + 1]
        del args[i:i + 2]
    if cmd == "extract":
        L.extract(args[1] if len(args) > 1 else "baserom.gba")
    elif cmd == "extract-rooms":
        L.extract_rooms_from_table(args[1], int(args[2], 16))
    elif cmd == "rooms":
        # levels.json's rooms (their folders); with --packed the ones whose
        # asset is LZ77-packed, with --new the ones in no ROM region
        levels = L.read_json(L.LEVELS_JSON)
        rooms = L.new_rooms(levels) if "--new" in args else levels["rooms"]
        if "--packed" in args:
            rooms = [r for r in rooms if L.read_json(L.LEVELS_DIR / r / "room.json")["asset"]["packed"]]
        print(" ".join(rooms))
    elif cmd == "tileset":
        L.tileset_build(args[1], build_dir)
    elif cmd == "tilesets-mk":
        L.tilesets_mk(args[1], build_dir)
    elif cmd == "table":
        if len(args) != 3 or args[1] not in ("levels", "records", "pictures"):
            sys.exit("usage: levels.py table levels|records|pictures OUT")
        L.table_c(args[1], args[2])
    elif cmd == "asset":
        asset, offsets, grids = L.build_asset(args[1], build_dir)
        Path(args[2]).write_bytes(asset)
        layout = Path(args[2]).with_name("asset.layout.json")
        layout.write_text(json.dumps({"offsets": offsets, "grids": grids}) + "\n")
    elif cmd == "c":
        L.region_c(args[1], args[2], build_dir)
    elif cmd == "assets":
        L.assets_c(args[1])
    elif cmd == "classes":
        L.write_if_changed(args[1], L.rooms_hpp().encode())
    elif cmd == "symbols":
        L.room_symbols("--check" in args)
    elif cmd == "render":
        L.render(args[1], args[2], args[3], build_dir)
    elif cmd == "constants":
        L.write_if_changed(args[2], L.constants_h(args[1]).encode())
    elif cmd == "palette-cycles":
        L.write_if_changed(args[1], L.theme_palette_cycles_h(L.read_json(L.LEVELS_JSON)).encode())
    elif cmd == "worlds":
        L.write_if_changed(args[1], L.level_select_worlds_h(L.read_json(L.LEVELS_JSON)).encode())
    elif cmd == "bosses":
        L.write_if_changed(args[1], L.level_bosses_h(L.read_json(L.LEVELS_JSON)).encode())
    elif cmd == "category-cycles":
        L.write_if_changed(args[1], L.category_palette_cycles_h(L.read_json(L.LEVELS_JSON)).encode())
    elif cmd == "on-clear":
        L.write_if_changed(args[1], L.level_on_clear_h(L.read_json(L.LEVELS_JSON)).encode())
    elif cmd == "spawns":
        Path(args[2]).parent.mkdir(parents=True, exist_ok=True)
        Path(args[2]).write_text(L.spawns_inc(args[1]))
    elif cmd == "categories":
        Path(args[1]).parent.mkdir(parents=True, exist_ok=True)
        Path(args[1]).write_text(L.actor_categories_inc(L.read_json(L.LEVELS_JSON)))
        L.category_palette_warnings(L.read_json(L.LEVELS_JSON))
    elif cmd == "anim-table":
        Path(args[2]).parent.mkdir(parents=True, exist_ok=True)
        Path(args[2]).write_text(L.anim_table_inc(args[1]))
    else:
        sys.exit("unknown command %s" % cmd)


if __name__ == "__main__":
    main()
