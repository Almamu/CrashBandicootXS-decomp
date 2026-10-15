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
    tile | flip << 10 | bank << 12)."""
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
    return users[symbol]["bpp"], data, banks, struct.pack("<%dH" % len(cells), *cells)


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


__all__ = ["EditError", "SPRITES", "collision_cells", "entity_sprites", "layer_info", "layer_pixels", "legend",
           "patch_collision", "patch_layer", "room_view", "sprite_banks", "sprite_png", "tileset", "with_palette"]
