"""A room as the level editor sees it, and its edits written back (#1167, docs/editors.md "The level editor").

The level editor (apps/editor) draws a room from its sources the way the
game does and edits them; this module is what the backend
(buildtools.editor_server) gives it and how it writes the edits:

- room_view(room_dir): room.json, the room's palette (the layer pictures',
  crashdata.levels.room_palette), each layer's size, priority and scroll
  scale, the collision legend, the entity types and, per type, a sprite
  frame to draw it with (entity_sprites);
- layer_pixels / collision_cells: a BG layer picture's palette indices, the
  collision layer's cell values (collision.read_image);
- tileset(symbol, room, layer): a BG tile set's tiles as the build makes
  them (crashdata.levels.generate_tileset) and the layer's cells, for the
  tile picker;
- sprite_png(...): a sprite bank's frame as a PNG with index 0
  transparent, in one of its palettes;
- redraw_tile(symbol, tile, pixels): a tile of a set redrawn in every room
  that names it, as the pictures that change;
- legend_with(value, colour): collision_legend.json with a new cell value;
- patch_layer / patch_collision / with_palette: a layer picture with some
  8x8 cells replaced, the collision image with some cells set, a picture
  with another palette. Each gives back the file's own bytes when nothing
  changed (a cell painted with what it had, the palette it had), so a room
  saved unchanged is byte for byte the same; otherwise the picture is
  written as crashdata's writers write it (write_layer_png's settings,
  collision.write_image), which give back the tree's pictures byte for
  byte, so a change shows only where it is.

crashdata.levels keeps the game it works on in module state
(levels.use_game): the caller binds the project's game first, and holds a
lock around it (the backend's LEVELS_LOCK).
"""
import io
import struct
import zlib
from pathlib import Path

from crashdata import levels as L
from crashdata.common import jasc_pal
from crashdata.levels import collision as collision_bmp

CELL = 8
BG_LAYERS = ("bg0", "bg1", "bg2", "bg3")

# The sprite each entity type is drawn with in the editor, where the type's
# name, its enemy kind or its sprite bank's name don't say (entity_sprites):
# name -> (bank, frame, OBJ palette id or None for the frame's own). Picked
# by eye from games/<game>/graphics/sprites/; a type no rule finds is a
# labelled box. Nothing here is game data: it only draws the editor's view.
SPRITES = {
    "xs": {
        "START_MARKER": (0, 0, None), "UNDERWATER_START_MARKER": (1, 0, None), "HOVER_START_MARKER": (2, 0, None),
        "GEM_PATH_GEM": (32, 0, 50), "BLUE_GEM": (32, 0, 53), "RED_GEM": (32, 0, 52), "GREEN_GEM": (32, 8, 51),
        "YELLOW_GEM": (32, 0, 49),
        "TURBO_RUN_POWER": (38, 0, None), "DOUBLE_JUMP_POWER": (38, 0, None), "BODY_SLAM_POWER": (38, 0, None),
        "TORNADO_SPIN_POWER": (38, 0, None),
        "BASIC_CRATE": (31, 44, None), "CHECKPOINT_CRATE": (31, 59, None), "AKU_AKU_CRATE": (31, 45, None),
        "IRON_SWITCH_CRATE": (31, 94, None), "ARROW_CRATE": (31, 46, None), "OUTLINE_CRATE": (31, 43, None),
        "NITRO_SWITCH_CRATE": (31, 12, None), "IRON_CRATE": (31, 11, None), "IRON_ARROW_CRATE": (31, 10, None),
        "LIFE_CRATE": (31, 31, None), "NITRO_CRATE": (31, 1, None), "MYSTERY_CRATE": (31, 0, None),
        "BOUNCY_WUMPA_CRATE": (31, 54, None), "REINFORCED_CRATE": (31, 30, None), "TNT_CRATE": (31, 35, None),
        "SLOT_CRATE": (31, 29, None), "TIME_CRATE_1": (31, 36, None), "TIME_CRATE_2": (31, 37, None),
        "TIME_CRATE_3": (31, 38, None),
        "SAUCER_LAB_ASSISTANT_B": (29, 0, None), "SEA_MINE_B": (6, 0, None), "CORTEX_BOSS": (53, 0, None),
        "LARGE_PLATFORM": (39, 0, None), "SMALL_PLATFORM": (39, 1, None), "MEDIUM_PLATFORM": (39, 2, None),
        "RED_GEM_PLATFORM": (32, 16, 52), "YELLOW_GEM_PLATFORM": (32, 16, 49),
        "GREEN_GEM_PLATFORM": (32, 16, 51), "BLUE_GEM_PLATFORM": (32, 16, 53),
        "BONUS_PLATFORM": (39, 4, None), "FLIP_PLATFORM": (39, 6, None), "ROCK_PLATFORM": (39, 17, None),
    },
    "n2": {
        "MYSTERY_CRATE": (23, 0, None), "IRON_ARROW_CRATE": (23, 9, None), "IRON_CRATE": (23, 10, None),
        "NITRO_SWITCH_CRATE": (23, 11, None), "NITRO_CRATE": (23, 19, None), "SLOT_CRATE": (23, 28, None),
        "REINFORCED_CRATE": (23, 29, None), "LIFE_CRATE": (23, 30, None), "TNT_CRATE": (23, 34, None),
        "TIME_CRATE_1": (23, 35, None), "TIME_CRATE_2": (23, 36, None), "TIME_CRATE_3": (23, 37, None),
        "OUTLINE_CRATE": (23, 42, None), "BASIC_CRATE": (23, 43, None), "AKU_AKU_CRATE": (23, 44, None),
        "ARROW_CRATE": (23, 45, None), "BOUNCY_WUMPA_CRATE": (23, 53, None), "CHECKPOINT_CRATE": (23, 58, None),
        "IRON_SWITCH_CRATE": (23, 92, None),
    },
}


class EditError(Exception):
    """An edit the room can't take: the message says why."""


# ---------------------------------------------------------------------------
# Reading

def legend():
    """The collision legend: [{value, colour, name}] (colour "#RRGGBB")."""
    return [{"value": v, "colour": "#%02X%02X%02X" % c, "name": n} for v, c, n in collision_bmp.legend()]


def layer_info(room):
    """Each layer's picture and what the game does with it."""
    out = {}
    for lname, layer in L.room_layers(room):
        w, h = layer["size_chunks"]
        out[lname] = {
            "map": layer["map"],
            "width": w * L.CHUNK_W * CELL, "height": h * L.CHUNK_H * CELL,
            "cells": [w * L.CHUNK_W, h * L.CHUNK_H],
            "sizeTiles": layer["size_tiles"],
            "scale": layer["scale"],
            "priority": layer["cnt"] & 3,
            "tileset": layer["tileset"],
            "bpp": 8 if lname == "bg0" else 4 if lname in BG_LAYERS else None,
        }
    return out


def sprite_banks(graphics_dir):
    """{bank number: its folder} of games/<game>/graphics/sprites/."""
    out = {}
    d = Path(graphics_dir) / "sprites"
    for p in sorted(d.iterdir()) if d.is_dir() else ():
        if p.is_dir() and p.name.startswith("bank") and p.name[4:6].isdigit():
            out[int(p.name[4:6])] = p
    return out


def entity_sprites(game, types, banks):
    """{entity type: {bank, frame, palette, origin, size}} for the types a
    sprite is found for: SPRITES, else the enemy kind (an enemy's kind is
    its sprite bank, docs/levels.md "Entity types"), else the bank named
    after the type (bankNN_<name>), frame 0."""
    import json

    from PIL import Image

    by_suffix = {p.name[7:]: n for n, p in banks.items()}
    table = SPRITES.get(game, {})
    frames_of = {}
    out = {}
    for t in types:
        name = t.get("name")
        pick = None
        if name in table:
            pick = table[name]
        elif t.get("enemy_kind") in banks:
            pick = (t["enemy_kind"], 0, None)
        elif name and name.lower() in by_suffix:
            pick = (by_suffix[name.lower()], 0, None)
        if pick is None or pick[0] not in banks:
            continue
        bank, frame, palette = pick
        if bank not in frames_of:
            try:
                frames_of[bank] = json.loads((banks[bank] / "bank.json").read_text())["frames"]
            except (OSError, ValueError, KeyError):
                frames_of[bank] = []
        frames = frames_of[bank]
        png = banks[bank] / ("f%03d.png" % frame)
        if frame >= len(frames) or not png.is_file():
            continue
        with Image.open(png) as im:
            size = list(im.size)
        out[t["type"]] = {"bank": bank, "frame": frame, "palette": palette,
                          "origin": frames[frame].get("origin", [0, 0]), "size": size}
    return out


def room_view(room_dir, game, graphics_dir):
    """What the editor opens: see the module's doc."""
    from crashdata.common import diagnostics as D

    room_dir = Path(room_dir)
    room = D.read_json(room_dir / "room.json")
    L.read_json(room_dir / "room.json")  # the schema's check, as the build reads it
    types = L.read_json(L.ENTITY_TYPES_JSON)["types"] if L.ENTITY_TYPES_JSON.exists() else []
    return {
        "room": room,
        "palette": list(L.room_palette(room_dir, room)),
        "layers": layer_info(room),
        "legend": legend(),
        "entityTypes": types,
        "sprites": {str(k): v for k, v in entity_sprites(game, types, sprite_banks(graphics_dir)).items()},
    }


def _open_layer(path, info):
    from PIL import Image

    img = Image.open(path)
    img.load()
    if img.mode != "P":
        raise EditError("%s: an image of mode %s, not an 8bpp indexed PNG" % (path, img.mode))
    if list(img.size) != [info["width"], info["height"]]:
        raise EditError("%s: %dx%d pixels, expected %dx%d" % ((path,) + img.size + (info["width"], info["height"])))
    return img


def layer_pixels(path, info):
    """(width, height, the palette indices row by row) of a BG layer picture."""
    img = _open_layer(path, info)
    return img.size[0], img.size[1], img.tobytes()


def collision_cells(path):
    """(width, height in cells, the cells as little-endian u16)."""
    try:
        w, h, cells = collision_bmp.read_image(path)
    except SystemExit as e:
        raise EditError(str(e))
    return w, h, struct.pack("<%dH" % len(cells), *cells)


def tileset(symbol, room_key, lname):
    """A BG tile set as the build makes it from its rooms' pictures, and the
    layer's cells in it: (bpp, tiles: 64 bytes each, a pixel a byte (4bpp:
    0-15), banks: each tile's palette bank where it is first drawn, cells:
    the layer's cells as little-endian u16, BG0's tile | flip << 14, BG1-3's
    tile | flip << 10 | bank << 12, uses: each tile's cells in every room's
    layers that name the set, limit: the most tiles the engine holds)."""
    levels = L.read_json(L.LEVELS_JSON)
    users = L.tileset_users(levels)
    if symbol not in users:
        raise EditError("no room's layer names the tile set %s" % symbol)
    try:
        tiles, banks, maps = L.generate_tileset(symbol, levels, users)
    except SystemExit as e:
        raise EditError(str(e))
    data = b"".join(r.to_bytes(8, "little") for b in tiles for r in b)
    cells = maps.get((room_key, lname))
    if cells is None:
        raise EditError("%s isn't one of %s's rooms (levels.json's \"rooms\")" % (room_key, symbol))
    bpp = users[symbol]["bpp"]
    mask = 0x3FFF if bpp == 8 else 0x3FF
    uses = [0] * len(tiles)
    for layer_cells in maps.values():
        for v in layer_cells:
            uses[v & mask] += 1
    return bpp, data, banks, struct.pack("<%dH" % len(cells), *cells), uses, L.MAX_TILES[bpp]


def redraw_tile(symbol, tile, pixels, was=None):
    """Tile `tile` of a BG tile set redrawn in every room that names it
    (docs/editors.md, "Drawing tiles"): each cell of their layers that draws
    it, under any flip and in any bank, drawn with `pixels` (64 bytes; 4bpp:
    0-15) in that cell's flips and bank. `was`, the tile's 64 pixels as the
    caller saw them, must still be it. ([(picture path, its bytes)] of the
    pictures that change, {"room/layer": cells redrawn})."""
    levels = L.read_json(L.LEVELS_JSON)
    users = L.tileset_users(levels)
    if symbol not in users:
        raise EditError("no room's layer names the tile set %s" % symbol)
    try:
        tiles, _, maps = L.generate_tileset(symbol, levels, users)
    except SystemExit as e:
        raise EditError(str(e))
    bpp = users[symbol]["bpp"]
    if not isinstance(tile, int) or isinstance(tile, bool) or not 0 < tile < len(tiles):
        raise EditError("%s has tiles 1-%d to redraw (tile 0 is the blank tile every set starts with)"
                        % (symbol, len(tiles) - 1))
    pixels = bytes(pixels)
    if len(pixels) != 64 or (bpp == 4 and max(pixels) > 15):
        raise EditError("a tile is 64 pixels%s" % (" of 0-15 (a BG1-3 tile's, without its bank)" if bpp == 4 else ""))
    old = b"".join(r.to_bytes(8, "little") for r in tiles[tile])
    if was is not None and bytes(was) != old:
        raise EditError("tile %d of %s isn't the tile the editor shows any more: open the room again" % (tile, symbol))
    shift = 14 if bpp == 8 else 10
    mask = (1 << shift) - 1
    drawn = []
    for f in range(4):  # FLIPS' order: none, h, v, both
        rows = [pixels[y * CELL:(y + 1) * CELL] for y in range(CELL)]
        if f & 1:
            rows = [r[::-1] for r in rows]
        if f & 2:
            rows = rows[::-1]
        drawn.append(b"".join(rows))
    out, report = [], {}
    for (room, lname), cells in maps.items():
        hits = []
        layer = L.read_json(L.LEVELS_DIR / room / "room.json")["layers"][lname]
        W = layer["size_chunks"][0] * L.CHUNK_W
        for i, v in enumerate(cells):
            if v & mask != tile:
                continue
            add = (v >> 12) * 16 if bpp == 4 else 0
            hits.append([i % W, i // W, bytes(p + add for p in drawn[v >> shift & 3])])
        if not hits:
            continue
        room_dir = L.LEVELS_DIR / room
        info = layer_info(L.read_json(room_dir / "room.json"))[lname]
        path = room_dir / layer["map"]
        data = patch_layer(path, info, lname, hits)
        report["%s/%s" % (room, lname)] = len(hits)
        if data != path.read_bytes():
            out.append((path, data))
    return out, report


def tileset_errors(symbols):
    """What the engine can't take of the tile sets `symbols` as their rooms'
    pictures now make them (docs/levels.md, "Tile sets"): more tiles than it
    holds, a screen of BG0 with more tiles than the tile slot pool's."""
    levels = L.read_json(L.LEVELS_JSON)
    users = L.tileset_users(levels)
    out = []
    for symbol in sorted(symbols):
        if symbol not in users:
            continue
        try:
            _, _, maps = L.generate_tileset(symbol, levels, users)
        except SystemExit as e:
            out.append(str(e.code if e.code is not None else e))
            continue
        if users[symbol]["bpp"] == 8:
            out += L.bg0_window_errors(symbol, maps, levels)
    return out


def _nearest(colours, rgb):
    r, g, b = rgb
    best, bi = None, 0
    for i, (cr, cg, cb) in colours:
        d = (cr - r) ** 2 + (cg - g) ** 2 + (cb - b) ** 2
        if best is None or d < best:
            best, bi = d, i
            if not d:
                break
    return bi, best


def import_tiles(png, palette, bpp, set_tiles, banks=None):
    """A picture (PNG bytes, any mode) as a BG layer's cells, quantised to the
    room's palette (docs/editors.md, "Tiles"): its size in cells (rounded up;
    the rest is transparent), each cell's 64 palette indices (BG0: the
    nearest of entries 1-255, a transparent pixel 0; BG1-3: the bank of
    `banks` (all 16 by default) whose 15 colours draw the cell closest,
    `bank * 16 + p`, a transparent pixel p = 0), and what the cells are in
    the tile set `set_tiles` (64 bytes a tile): a tile of it, one of its
    tiles flipped, blank, or a new tile (counted once, flips included)."""
    from PIL import Image

    try:
        img = Image.open(io.BytesIO(png))
        img.load()
    except Exception as e:
        raise EditError("not a picture: %s" % e)
    img = img.convert("RGBA")
    w, h = img.size
    if w * h > 1024 * 1024:
        raise EditError("%dx%d pixels: import at most a megapixel at a time" % (w, h))
    cols, rows = -(-w // CELL), -(-h // CELL)
    rgb = [tuple(c) for c in (jasc_pal.to_rgb(v) for v in palette_entries(palette))]
    px = img.load()
    bank_list = list(range(16)) if not banks else sorted({int(b) for b in banks if 0 <= int(b) < 16})
    if bpp == 4 and not bank_list:
        raise EditError("no palette bank to import into")
    # a cell that looks like a tile of the set (flipped, in a bank) is that
    # tile: the room palette has entries of the same colour, so the nearest
    # colour alone could make another block of the same look
    looks = {}
    for t in range(len(set_tiles) // 64):
        for f, blk in enumerate(_flips(bytes(set_tiles[t * 64:(t + 1) * 64]))):
            for bank in ([0] if bpp == 8 else bank_list):
                add = bank * 16 if bpp == 4 else 0
                key = tuple(None if p == 0 else rgb[p + add] for p in blk)
                looks.setdefault(key, bytes(p + add for p in blk))
    cache = {}
    out = bytearray(cols * rows * 64)
    for cy in range(rows):
        for cx in range(cols):
            cell = []
            for y in range(CELL):
                for x in range(CELL):
                    X, Y = cx * CELL + x, cy * CELL + y
                    c = px[X, Y] if X < w and Y < h else (0, 0, 0, 0)
                    cell.append(None if c[3] < 128 else c[:3])
            o = (cy * cols + cx) * 64
            if tuple(cell) in looks:
                out[o:o + 64] = looks[tuple(cell)]
                continue
            if bpp == 8:
                block = []
                for c in cell:
                    if c is None:
                        block.append(0)
                        continue
                    if c not in cache:
                        cache[c] = _nearest([(i, rgb[i]) for i in range(1, 256)], c)[0]
                    block.append(cache[c])
            else:
                best = None
                for bank in bank_list:
                    err, block = 0, []
                    for c in cell:
                        if c is None:
                            block.append(bank * 16)
                            continue
                        key = (bank, c)
                        if key not in cache:
                            cache[key] = _nearest([(bank * 16 + i, rgb[bank * 16 + i]) for i in range(1, 16)], c)
                        i, d = cache[key]
                        block.append(i)
                        err += d
                    if best is None or err < best[0]:
                        best = (err, block)
                block = best[1]
            o = (cy * cols + cx) * 64
            out[o:o + 64] = bytes(block)
    # what the cells are in the set
    index = {}
    for t in range(len(set_tiles) // 64):
        blk = bytes(set_tiles[t * 64:(t + 1) * 64])
        for f, fb in enumerate(_flips(blk)):
            index.setdefault(fb, (t, f))
    stats = {"cells": cols * rows, "blank": 0, "tiles": 0, "flipped": 0, "new": 0}
    new = set()
    for k in range(cols * rows):
        blk = bytes(p & 15 if bpp == 4 else p for p in out[k * 64:(k + 1) * 64])
        hit = index.get(blk)
        if hit is not None:
            stats["blank" if hit[0] == 0 else "flipped" if hit[1] else "tiles"] += 1
        elif blk not in new:
            new.add(blk)
            for fb in _flips(blk):
                index.setdefault(fb, (-1, 0))
            stats["new"] += 1
        else:
            stats["tiles"] += 1
    return cols, rows, bytes(out), stats


def _flips(blk):
    """A 64-pixel block under its four flips (none, X, Y, both)."""
    rows = [blk[y * 8:(y + 1) * 8] for y in range(8)]
    h = [r[::-1] for r in rows]
    return [b"".join(rows), b"".join(h), b"".join(rows[::-1]), b"".join(h[::-1])]


def palette_entries(palette):
    if len(palette) != 256 or not all(isinstance(v, int) and 0 <= v <= 0xFFFF for v in palette):
        raise EditError("a room palette is 256 entries of 0-0xFFFF (BGR555, bit 15 kept)")
    return palette


def sprite_png(bank_dir, frame, palette_pal=None):
    """A sprite frame (bankNN_<name>/fKKK.png) as a PNG whose index 0 is
    transparent, drawn with its own palette or the JASC palette given."""
    from PIL import Image

    img = Image.open(Path(bank_dir) / ("f%03d.png" % frame))
    img.load()
    if palette_pal is not None:
        pal = img.getpalette() or []
        colours = [c for v in jasc_pal.read_jasc(palette_pal) for c in jasc_pal.to_rgb(v)]
        pal[:len(colours)] = colours
        img.putpalette(pal)
    buf = io.BytesIO()
    img.save(buf, format="PNG", transparency=0)
    return buf.getvalue()


# ---------------------------------------------------------------------------
# Writing

def palette_rgb(palette):
    if len(palette) != 256 or not all(isinstance(v, int) and 0 <= v <= 0xFFFF for v in palette):
        raise EditError("a room palette is 256 entries of 0-0xFFFF (BGR555, bit 15 kept)")
    return [c for v in palette for c in jasc_pal.to_rgb(v)]


def _chunks(data):
    if data[:8] != b"\x89PNG\r\n\x1a\n":
        raise EditError("not a PNG")
    i = 8
    while i < len(data):
        (n,) = struct.unpack(">I", data[i:i + 4])
        yield data[i + 4:i + 8], i, i + 12 + n
        i += 12 + n


def with_palette(data, palette):
    """An indexed PNG's bytes with its PLTE replaced by `palette` (256
    BGR555 entries, written as jasc_pal writes them); its pixels' bytes stay
    as they are. The same bytes when the palette is the one it has."""
    rgb = bytes(palette_rgb(palette))
    for kind, a, b in _chunks(data):
        if kind == b"PLTE":
            if b - a - 12 != len(rgb):
                raise EditError("the picture's palette has %d entries, not 256" % ((b - a - 12) // 3))
            chunk = struct.pack(">I", len(rgb)) + b"PLTE" + rgb + struct.pack(">I", zlib.crc32(b"PLTE" + rgb))
            return data[:a] + chunk + data[b:]
    raise EditError("the picture has no palette")


def _cell(v, n, what):
    if not isinstance(v, int) or not 0 <= v < n:
        raise EditError("%s %r is outside the layer (0-%d)" % (what, v, n - 1))
    return v


def patch_layer(path, info, lname, cells, palette=None):
    """A BG layer picture with `cells` ([cx, cy, 64 palette indices, row by
    row]) drawn over it, and the palette replaced if given: its bytes. A BG1-3
    cell's 64 pixels must be of one palette bank (docs/levels.md, "BG
    layers"). The file's own bytes if nothing changes."""
    old = Path(path).read_bytes()
    img = _open_layer(io.BytesIO(old), info)
    W = img.size[0]
    cw, ch = info["cells"]
    raw = bytearray(img.tobytes())
    before = bytes(raw)
    for item in cells:
        if not isinstance(item, (list, tuple)) or len(item) != 3:
            raise EditError("a cell is [cx, cy, its 64 pixels]")
        cx, cy, px = _cell(item[0], cw, "column"), _cell(item[1], ch, "row"), bytes(item[2])
        if len(px) != 64:
            raise EditError("cell (%d, %d): %d pixels, a cell has 64" % (cx, cy, len(px)))
        if lname != "bg0" and len({p >> 4 for p in px}) != 1:
            raise EditError("cell (%d, %d) mixes palette banks: a BG1-3 cell draws all 64 pixels from one bank"
                            % (cx, cy))
        for r in range(CELL):
            o = (cy * CELL + r) * W + cx * CELL
            raw[o:o + CELL] = px[r * CELL:(r + 1) * CELL]
    if bytes(raw) == before:
        return old if palette is None else with_palette(old, palette)
    from PIL import Image

    out = Image.frombytes("P", img.size, bytes(raw))
    out.putpalette(img.getpalette() if palette is None else palette_rgb(palette))
    buf = io.BytesIO()
    out.save(buf, format="PNG", optimize=True)  # write_layer_png's
    return buf.getvalue()


def legend_with(value, colour, name=None):
    """collision_legend.json's value with a new cell value added at its end
    (collision.new_entry_errors says what it takes): `colour` "#RRGGBB" or
    "RRGGBB", `name` collision.describe's by default. Its colour is the next
    entry of the collision images' palettes; the pictures already written
    keep theirs (an image is read by colour)."""
    if not isinstance(colour, str) or len(colour.lstrip("#")) != 6:
        raise EditError("a colour is \"#RRGGBB\"")
    try:
        c = int(colour.lstrip("#"), 16)
    except ValueError:
        raise EditError("a colour is \"#RRGGBB\"")
    rgb = (c >> 16, c >> 8 & 0xFF, c & 0xFF)
    if name is None or (isinstance(name, str) and not name.strip()):
        name = collision_bmp.describe(value) if isinstance(value, int) else ""
    if not isinstance(name, str):
        raise EditError("a value's name is text")
    try:
        entries = collision_bmp.legend()
    except SystemExit as e:
        raise EditError(str(e))
    errors = collision_bmp.new_entry_errors(value, rgb, entries)
    if errors:
        raise EditError("; ".join(errors))
    doc = L.read_json(collision_bmp.LEGEND_JSON)
    doc["cells"].append({"value": "%04X" % value, "colour": "%06X" % c, "name": name.strip()})
    return doc


def patch_collision(path, info, cells):
    """The collision image with `cells` ([cx, cy, value]) set: its bytes
    (collision.write_image's), or the file's own if nothing changes."""
    old = Path(path).read_bytes()
    try:
        w, h, values = collision_bmp.read_image(io.BytesIO(old))
    except SystemExit as e:
        raise EditError(str(e))
    if [w, h] != list(info["cells"]):
        raise EditError("%s: %dx%d cells, expected %dx%d" % (path, w, h, info["cells"][0], info["cells"][1]))
    known = {v for v, _, _ in collision_bmp.legend()}
    new = list(values)
    for item in cells:
        if not isinstance(item, (list, tuple)) or len(item) != 3:
            raise EditError("a collision cell is [cx, cy, value]")
        cx, cy, v = _cell(item[0], w, "column"), _cell(item[1], h, "row"), item[2]
        if v not in known:
            raise EditError("cell (%d, %d): %r isn't a value of the collision legend (collision_legend.json)"
                            % (cx, cy, v))
        new[cy * w + cx] = v
    if new == values:
        return old
    buf = io.BytesIO()
    collision_bmp.write_image(buf, w, new, fmt="PNG")
    return buf.getvalue()


__all__ = ["EditError", "SPRITES", "collision_cells", "entity_sprites", "import_tiles", "layer_info", "layer_pixels",
           "legend", "legend_with", "patch_collision", "patch_layer", "redraw_tile", "room_view", "sprite_banks",
           "sprite_png", "tileset", "tileset_errors", "with_palette"]
