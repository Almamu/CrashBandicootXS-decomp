"""New, deleted and resized rooms (#1167, docs/editors.md "The level editor", "Rooms").

The level editor makes a room (a copy of a room, or an empty room laid out
like one), deletes a room nothing plays, and resizes a room. Each is a
plan of files to write and remove, which the backend
(buildtools.editor_server) stages, commits and checks like any save (all
or nothing). What each keeps consistent:

- a new room: its folder (room.json, its layer pictures), its own symbols
  (gRoomNN, gLevelRoomNN: the next free NN, docs/levels.md "Adding a
  room"), levels.json's "rooms" and, if asked, a level that plays it (its
  "rooms", "bonus" or "gem_path"; N-Tranced's "table" "records" too), and
  cxx_symbols.txt's room classes block (XS's, `levels.py symbols`);
- a deleted room: refused while a level plays it; otherwise its folder goes,
  and so do its entries in levels.json ("rooms", the ROM's "regions" and
  "assets" lists) and tileset_order.json, and its classes' symbols;
- a resized room: BG0 and the collision layer (which share a grid) grow
  or shrink at each edge by whole cells; their pictures move with it, the
  entities move with them (those outside go, with their links, and the
  entities are put back in id order, the links renumbered), and each BG1-3
  layer grows or shrinks by its parallax share of the same edges
  (`edge * scale / 256` cells), so it lines up with BG0 as before at every
  camera position. The scrollable sizes (size_tiles), the chunk grids
  (size_chunks) and so the camera's bounds, the entity groups and the
  layers' streams follow from them. What is cut is reported.

The limits checked (LIMITS) are the engine's: docs/levels.md, "Room sizes".
"""
import io
import re
import struct
from pathlib import Path

from crashdata import levels as L
from crashdata.levels import collision as collision_bmp
from crashdata.levels.room_edit import BG_LAYERS, CELL, EditError

SCREEN_W, SCREEN_H = 240, 160
# a room's size in cells, BG0's (and the collision layer's) scrollable size
# (docs/levels.md, "Room sizes"): at least a screen; at most what the
# chunk grid's u16 sizes and ids, and the asset's u16 offsets, take
LIMITS = {"min_cells": L.ROOM_MIN_CELLS, "max_cells": L.ROOM_MAX_CELLS}
SLOT_RE = re.compile(r"^[a-z0-9_]+/[a-z0-9_]+$")
SYMBOL_RE = re.compile(r"^g(Level)?Room(\d+)(?=[A-Z]|$)")


def _ceil_div(a, b):
    return -(-a // b)


def chunks_for(size_tiles):
    """A layer's chunk grid for its scrollable size."""
    return [_ceil_div(size_tiles[0], L.CHUNK_W), _ceil_div(size_tiles[1], L.CHUNK_H)]


def _levels():
    return L.read_json(L.LEVELS_JSON)


def _room_dir(key):
    return L.LEVELS_DIR / key


def _read_room(key):
    return L.read_json(_room_dir(key) / "room.json")


def room_number(room):
    """The NN of a room's symbols (gRoomNNDesc)."""
    m = SYMBOL_RE.match(room["symbols"]["desc"])
    if not m:
        raise EditError("%s isn't gRoomNNDesc: name its symbols by hand (docs/levels.md, \"Adding a room\")"
                        % room["symbols"]["desc"])
    return int(m.group(2))


def room_symbols(room):
    """Every symbol a room.json defines."""
    out = list(room["symbols"].values()) + [room["asset"]["symbol"]]
    out += [layer["grid_symbol"] for layer in room["layers"].values() if "grid_symbol" in layer]
    if room.get("record"):
        out.append(room["record"]["symbol"])
    return out


def renumber(room, nn):
    """The room with its symbols' gRoomOLD / gLevelRoomOLD made NN."""
    old = room_number(room)
    width = max(2, len(str(nn)))

    def sub(s):
        m = SYMBOL_RE.match(s)
        if not m or int(m.group(2)) != old:
            raise EditError("%s doesn't follow the room's gRoom%02d names: name its symbols by hand" % (s, old))
        return "g%sRoom%0*d%s" % (m.group(1) or "", width, nn, s[m.end():])

    room = dict(room)
    room["symbols"] = {k: sub(v) for k, v in room["symbols"].items()}
    room["asset"] = dict(room["asset"], symbol=sub(room["asset"]["symbol"]))
    room["layers"] = {k: dict(v, grid_symbol=sub(v["grid_symbol"])) if "grid_symbol" in v else v
                      for k, v in room["layers"].items()}
    if room.get("record"):
        room["record"] = dict(room["record"], symbol=sub(room["record"]["symbol"]))
    return room


def _all_symbols(levels):
    out = set()
    for r in levels["rooms"]:
        out |= set(room_symbols(_read_room(r)))
    return out


def next_number(levels):
    """A room number no room has: the room count if it's free (gRoomNN is the
    NNth room, docs/levels.md "Room folders"), else one past the highest."""
    used = {room_number(_read_room(r)) for r in levels["rooms"]}
    n = len(levels["rooms"])
    return n if n not in used else max(used) + 1


# ---------------------------------------------------------------------------
# Pictures

def _palette(path):
    from PIL import Image

    with Image.open(path) as img:
        return img.getpalette()


def _png(img):
    buf = io.BytesIO()
    img.save(buf, format="PNG", optimize=True)  # write_layer_png's
    return buf.getvalue()


def blank_layer(width_px, height_px, palette):
    """A BG layer picture of index 0 (BG0's transparent pixel; BG1-3's bank
    0, tile 0) with the room's palette."""
    from PIL import Image

    img = Image.new("P", (width_px, height_px), 0)
    img.putpalette(palette)
    return _png(img)


def blank_collision(cols, rows):
    buf = io.BytesIO()
    collision_bmp.write_image(buf, cols, [0] * (cols * rows), fmt="PNG")
    return buf.getvalue()


def _shift_cells(cells, cols, rows, dx, dy, ncols, nrows, blank):
    """A grid of cells (row-major) moved by (dx, dy) cells into a grid of
    ncols x nrows, blank where nothing moved in: (the new cells, how many
    cells that weren't blank fell outside)."""
    out = [blank] * (ncols * nrows)
    lost = 0
    for y in range(rows):
        ny = y + dy
        row = cells[y * cols:(y + 1) * cols]
        for x, v in enumerate(row):
            nx = x + dx
            if 0 <= nx < ncols and 0 <= ny < nrows:
                out[ny * ncols + nx] = v
            elif v != blank:
                lost += 1
    return out, lost


def shift_picture(path, dx, dy, ncols, nrows, bg0):
    """A BG layer picture moved by (dx, dy) cells into one of ncols x nrows
    cells: (its PNG bytes, how many cells that weren't blank were cut). The
    room's palette stays; a new cell is index 0 (blank)."""
    from PIL import Image

    with Image.open(path) as old:
        old.load()
        img = Image.new("P", (ncols * CELL, nrows * CELL), 0)
        img.putpalette(old.getpalette())
        img.paste(old, (dx * CELL, dy * CELL))
        cols, rows = old.size[0] // CELL, old.size[1] // CELL
        raw = old.tobytes()
    lost = 0
    W = cols * CELL
    for cy in range(rows):
        ny = cy + dy
        for cx in range(cols):
            nx = cx + dx
            if 0 <= nx < ncols and 0 <= ny < nrows:
                continue
            for r in range(CELL):
                o = (cy * CELL + r) * W + cx * CELL
                px = raw[o:o + CELL]
                if any(p if bg0 else p & 15 for p in px):
                    lost += 1
                    break
    return _png(img), lost


def shift_collision(path, dx, dy, ncols, nrows):
    try:
        w, h, cells = collision_bmp.read_image(path)
    except SystemExit as e:
        raise EditError(str(e))
    new, lost = _shift_cells(cells, w, h, dx, dy, ncols, nrows, 0)
    buf = io.BytesIO()
    collision_bmp.write_image(buf, ncols, new, fmt="PNG")
    return buf.getvalue(), lost


# ---------------------------------------------------------------------------
# Entities

def reorder_entities(room):
    """The room's entities put back in id order (the last 256-pixel group
    first, docs/levels.md "Entities"; stable within a group), and its links
    renumbered to follow."""
    ents = room["entities"]
    order = sorted(range(len(ents)), key=lambda i: -(ents[i]["x"] >> L.GROUP_SHIFT))
    new_id = {old: new for new, old in enumerate(order)}
    room["entities"] = [ents[i] for i in order]
    if room.get("links"):
        room["links"] = [[new_id[a], new_id[b]] for a, b in room["links"]]
    return room


def drop_entities(room, gone):
    """The room without the entities `gone` (their ids): their links go,
    the other links are renumbered. The parameter records stay."""
    gone = set(gone)
    keep = [i for i in range(len(room["entities"])) if i not in gone]
    new_id = {old: new for new, old in enumerate(keep)}
    room["entities"] = [room["entities"][i] for i in keep]
    if room.get("links") is not None:
        room["links"] = [[new_id[a], new_id[b]] for a, b in room["links"] if a in new_id and b in new_id]
    return room


def _type_name(t):
    return t if isinstance(t, str) else "type %d" % t


# ---------------------------------------------------------------------------
# Checks

def check_size(size_tiles, what="the room"):
    (minw, minh), (maxw, maxh) = LIMITS["min_cells"], LIMITS["max_cells"]
    w, h = size_tiles
    if not (minw <= w <= maxw and minh <= h <= maxh):
        raise EditError("%s would be %dx%d cells: a room is at least a screen, %dx%d cells (240x160 pixels), and at "
                        "most %dx%d (an entity's position is a u16 pixel)" % (what, w, h, minw, minh, maxw, maxh))
    cw, ch = chunks_for(size_tiles)
    if cw * ch > 0xFFFF:
        raise EditError("%s would be %dx%d chunks: a layer's chunk ids are u16" % (what, cw, ch))


def parallax_size(room_tiles, scale):
    """The cells a layer at `scale` (Q8) needs to cover the camera's range in
    a room of `room_tiles`: the screen, plus the camera's last position
    scaled (BgLayerBase::ScaleScroll)."""
    return [_ceil_div(max(0, (n * CELL - s) * k // 256) + s, CELL)
            for n, s, k in zip(room_tiles, (SCREEN_W, SCREEN_H), scale)]


# ---------------------------------------------------------------------------
# The plans

def _plan():
    return {"json": [], "bytes": [], "delete": [], "dirs": [], "report": {}}


def _symbols_plan(plan, rooms):
    """cxx_symbols.txt's room classes block for the rooms (XS's; another
    game's classes keep XS's C names, docs/n_tranced.md)."""
    if not L.GAME.is_xs:
        return
    head, _, tail = L.split_room_symbols()
    text = head + L.room_symbols_block(rooms) + tail
    plan["bytes"].append((Path(L.CXX_SYMBOLS), text.encode("utf-8")))


def _rooms_with(levels, changes):
    """Every room of levels.json's "rooms" (room.json, "name") with
    `changes` ({key: room or None}) applied: what the symbols block lists."""
    out = []
    for r in levels["rooms"]:
        room = changes[r] if r in changes else _read_room(r)
        out.append(dict(room, name=r))
    return out


def plays(levels, key):
    """The levels that play a room: [(constant, how)]."""
    out = []
    for lv in levels.get("levels", []):
        if key in lv.get("rooms", []):
            out.append((lv["constant"], "rooms"))
        for k in ("bonus", "gem_path"):
            if lv.get(k) == key:
                out.append((lv["constant"], k))
    return out


def new_room(src, dst, empty=False, size=None, attach=None):
    """A new room `dst` (<level>/<slot>) like `src`: a copy of it, or with
    `empty` its layers' layout (tile sets, scroll scales, priorities, the
    palette, the record) with blank pictures and collision, `size` cells
    (BG0's scrollable size; the source's by default) and only the source's
    start marker. `attach`: {"level": LEVEL_*, "as": "rooms" | "bonus" |
    "gem_path", "index": where in "rooms"}."""
    levels = _levels()
    if not isinstance(dst, str) or not SLOT_RE.match(dst):
        raise EditError("a new room's folder is <level>/<slot>, lower-case letters, digits and _: %r" % (dst,))
    if dst in levels["rooms"] or _room_dir(dst).exists():
        raise EditError("%s already exists" % dst)
    if src not in levels["rooms"]:
        raise EditError("%s isn't a room of levels.json's \"rooms\"" % src)
    room = _read_room(src)
    nn = next_number(levels)
    room = renumber(room, nn)
    taken = _all_symbols(levels) & set(room_symbols(room))
    if taken:
        raise EditError("the new room's symbols %s are taken" % ", ".join(sorted(taken)))
    plan = _plan()
    plan["dirs"].append(_room_dir(dst))
    report = {"room": dst, "number": nn, "symbols": room["symbols"]["desc"], "warnings": []}
    sdir = _room_dir(src)
    if not empty:
        for _, layer in L.room_layers(room):
            plan["bytes"].append((_room_dir(dst) / layer["map"], (sdir / layer["map"]).read_bytes()))
    else:
        size = list(size or room["layers"]["bg0" if "bg0" in room["layers"] else "collision"]["size_tiles"])
        if len(size) != 2 or not all(isinstance(v, int) for v in size):
            raise EditError("a room's size is [width, height] in cells")
        check_size(size)
        palette = None
        for lname, layer in L.room_layers(room):
            if lname != "collision":
                palette = _palette(sdir / layer["map"])
                break
        layers = {}
        for lname, layer in room["layers"].items():
            layer = {k: v for k, v in layer.items() if k != "pad_overrides"}
            st = list(size) if lname in ("bg0", "collision") or layer["scale"] == [256, 256] else \
                [max(a, b) for a, b in zip(parallax_size(size, layer["scale"]), LIMITS["min_cells"])]
            layer["size_tiles"] = st
            layer["size_chunks"] = chunks_for(st)
            cols, rows = layer["size_chunks"][0] * L.CHUNK_W, layer["size_chunks"][1] * L.CHUNK_H
            if lname == "collision":
                data = blank_collision(cols, rows)
            else:
                data = blank_layer(cols * CELL, rows * CELL, palette)
            plan["bytes"].append((_room_dir(dst) / layer["map"], data))
            layers[lname] = layer
        room["layers"] = layers
        # the start marker: the source's (its kind's), at the same place if it fits
        names = L.entity_type_names()
        marker = next((e for e in room["entities"] if isinstance(e["type"], str) and e["type"].endswith("START_MARKER")),
                      None)
        if marker is None:
            marker = next((e for e in room["entities"] if e["type"] == 0 or (0 < len(names) and e["type"] == names[0])),
                          None)
        params, ents = [], []
        if marker is not None:
            params.append(room["params"][marker["param"]])
            ents.append(dict(marker, x=min(marker["x"], size[0] * CELL - 1), y=min(marker["y"], size[1] * CELL - 1),
                             param=0))
        else:
            report["warnings"].append("%s has no start marker to copy: add one" % src)
            params.append([0])
        room["params"], room["entities"] = params, ents
        room["symbols"] = {k: v for k, v in room["symbols"].items() if k != "links"}
        room.pop("links", None)
    plan["json"].append((_room_dir(dst) / "room.json", room))
    levels = dict(levels, rooms=levels["rooms"] + [dst])
    if attach:
        levels = _attach(levels, dst, attach, report)
    plan["json"].append((L.LEVELS_JSON, levels))
    _symbols_plan(plan, _rooms_with(levels, {dst: room}))
    plan["report"] = report
    return plan


def _attach(levels, key, attach, report):
    constant, how = attach.get("level"), attach.get("as", "rooms")
    lvs = [dict(lv) for lv in levels.get("levels", [])]
    lv = next((x for x in lvs if x["constant"] == constant), None)
    if lv is None:
        raise EditError("no level %s" % constant)
    if how == "rooms":
        rooms = list(lv["rooms"])
        i = attach.get("index", len(rooms))
        if not isinstance(i, int) or not 0 <= i <= len(rooms):
            raise EditError("%s plays %d rooms: a new one goes at 0-%d" % (constant, len(rooms), len(rooms)))
        rooms.insert(i, key)
        lv["rooms"] = rooms
        table = levels.get("table")
        if table and table.get("records") is not None:
            # N-Tranced's records in their own order: the new room's last
            table = dict(table, records=table["records"] + [key])
            levels = dict(levels, table=table)
    elif how in ("bonus", "gem_path"):
        if lv.get(how):
            report["warnings"].append("%s's %s room was %s, which no level plays now" % (constant, how, lv[how]))
        lv[how] = key
    else:
        raise EditError("a level plays a room in its \"rooms\", or as its \"bonus\" or \"gem_path\" room")
    report["attached"] = {"level": constant, "as": how}
    return dict(levels, levels=lvs)


def delete_room(key):
    """Deletes a room no level plays: its folder, its entries in levels.json
    and tileset_order.json, its classes' symbols."""
    levels = _levels()
    if key not in levels["rooms"]:
        raise EditError("%s isn't a room of levels.json's \"rooms\"" % key)
    users = plays(levels, key)
    if users:
        raise EditError("%s is played by %s: take it out of the level first"
                        % (key, ", ".join("%s (%s)" % u for u in users)))
    if len(levels["rooms"]) == 1:
        raise EditError("%s is the game's last room" % key)
    plan = _plan()
    room = _read_room(key)
    d = _room_dir(key)
    plan["delete"] += [d / "room.json"] + [d / layer["map"] for _, layer in L.room_layers(room)]
    others = sorted(p.name for p in d.iterdir() if p.name not in {x.name for x in plan["delete"]}) if d.is_dir() else []
    if others:
        raise EditError("%s has other files (%s): move them first" % (key, ", ".join(others)))
    new = dict(levels, rooms=[r for r in levels["rooms"] if r != key])
    if key in L.rom_region_rooms(levels):
        new["regions"] = [dict(rg, rooms=[r for r in rg["rooms"] if r != key]) for rg in levels["regions"]]
        new["assets"] = dict(levels["assets"], rooms=[r for r in levels["assets"]["rooms"] if r != key])
    plan["json"].append((L.LEVELS_JSON, new))
    order = L.load_tileset_order()
    if any(key in v for v in order.values()):
        plan["json"].append((L.TILESET_ORDER_JSON, {k: [r for r in v if r != key] for k, v in order.items()
                                                    if [r for r in v if r != key]}))
    _symbols_plan(plan, _rooms_with(new, {}))
    plan["report"] = {"room": key, "deleted": [str(p.name) for p in plan["delete"]],
                      "rom_room": key in L.rom_region_rooms(levels)}
    return plan


def resize_room(key, edges):
    """Resizes a room by `edges` ({"left", "right", "top", "bottom"}: cells,
    positive to grow, negative to shrink) at BG0 and the collision layer's
    scale, each BG1-3 layer by its parallax share. The plan's report says
    what was cut: the entities (with their links) and each layer's cells
    that weren't blank."""
    levels = _levels()
    if key not in levels["rooms"]:
        raise EditError("%s isn't a room of levels.json's \"rooms\"" % key)
    try:
        el, er, et, eb = (int(edges.get(k, 0)) for k in ("left", "right", "top", "bottom"))
    except (TypeError, ValueError, AttributeError):
        raise EditError("the edges are {\"left\", \"right\", \"top\", \"bottom\"}: cells to add (or, negative, take)")
    room = _read_room(key)
    d = _room_dir(key)
    base = room["layers"]["collision"]["size_tiles"]
    new_size = [base[0] + el + er, base[1] + et + eb]
    check_size(new_size)
    plan = _plan()
    report = {"room": key, "size": [list(base), new_size], "layers": {}, "entities": [], "links": 0, "warnings": []}
    if not (el or er or et or eb):
        plan["report"] = report
        return plan
    layers = {}
    for lname, layer in room["layers"].items():
        layer = dict(layer)
        if lname in ("bg0", "collision"):
            dl, dr, dt, db = el, er, et, eb
        else:
            sx, sy = layer["scale"]
            dl, dr = round(el * sx / 256), round(er * sx / 256)
            dt, db = round(et * sy / 256), round(eb * sy / 256)
        old_st = layer["size_tiles"]
        st = [old_st[0] + dl + dr, old_st[1] + dt + db]
        if lname not in ("bg0", "collision"):
            st = [max(a, b) for a, b in zip(st, LIMITS["min_cells"])]
            check_size(st, "%s's %s" % (key, lname))
            # a layer the camera's range runs past repeats its last chunks (the
            # streamer skips what is past its grid): the ROM's rooms do it, so
            # it is only said, when the resize makes it so
            need, was = parallax_size(new_size, layer["scale"]), parallax_size(base, layer["scale"])
            for axis, (n, a, w, o) in enumerate(zip(need, st, was, old_st)):
                if a < n and o >= w:
                    report["warnings"].append("%s is %d cells %s, but the camera's range at its scale (%d/256) "
                                              "takes %d: past its end it repeats its last chunks"
                                              % (lname, a, "wide" if axis == 0 else "high", layer["scale"][axis], n))
        layer["size_tiles"] = st
        layer["size_chunks"] = chunks_for(st)
        layer.pop("pad_overrides", None)  # the ROM's bytes of chunks that move
        cols, rows = layer["size_chunks"][0] * L.CHUNK_W, layer["size_chunks"][1] * L.CHUNK_H
        path = d / layer["map"]
        if lname == "collision":
            data, lost = shift_collision(path, dl, dt, cols, rows)
        else:
            data, lost = shift_picture(path, dl, dt, cols, rows, lname == "bg0")
        plan["bytes"].append((path, data))
        report["layers"][lname] = {"size": [list(old_st), st], "shift": [dl, dt], "cut": lost}
        layers[lname] = layer
    room = dict(room, layers=layers)
    # the entities: moved with BG0, those outside the room go
    W, H = new_size[0] * CELL, new_size[1] * CELL
    ents = [dict(e, x=e["x"] + el * CELL, y=e["y"] + et * CELL) for e in room["entities"]]
    gone = [i for i, e in enumerate(ents) if not (0 <= e["x"] < W and 0 <= e["y"] < H)]
    nlinks = len(room.get("links") or [])
    room["entities"] = ents
    report["entities"] = [{"id": i, "type": _type_name(room["entities"][i]["type"]),
                           "x": room["entities"][i]["x"] - el * CELL, "y": room["entities"][i]["y"] - et * CELL}
                          for i in gone]
    room = drop_entities(room, gone)
    report["links"] = nlinks - len(room.get("links") or [])
    room = reorder_entities(room)
    for e in report["entities"]:
        if "EXIT" in e["type"] or "START_MARKER" in e["type"]:
            report["warnings"].append("the room's %s (%d, %d) was cut" % (e["type"], e["x"], e["y"]))
    plan["json"].append((d / "room.json", room))
    plan["report"] = report
    return plan


__all__ = ["LIMITS", "chunks_for", "check_size", "delete_room", "new_room", "next_number", "parallax_size",
           "plays", "renumber", "reorder_entities", "resize_room", "room_number"]
