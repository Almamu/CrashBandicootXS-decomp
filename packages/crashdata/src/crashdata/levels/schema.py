"""The level data's JSON Schemas (#1167, docs/editors.md), from crashdata.levels' own field lists.

levels.json, the rooms' room.json, the spawn lists, the animation tables,
entity_types.json, crate_kinds.json, collision_legend.json and
tileset_order.json. Each is built from what crashdata.levels reads (its
*_KEYS lists, its kinds and limits), and crashdata.levels.read_json
validates each file with it before the model's own checks.

The schemas describe both games' files (XS's games/xs/data/levels/ and
N-Tranced's games/n2/data/levels/), which the same code reads: a key only
one game has is optional. Names that come from other data (an entity
type's name, a room folder, a song) are strings here: the model checks
that they exist.
"""
from crashdata.common import schema as S


def _levels():
    from crashdata import levels as L
    return L


ROOM_FOLDER = S.string(r"^[a-z0-9_]+/[a-z0-9_]+$", description="a room's folder under the level data, <level>/<slot>")
CONSTANT = S.string(r"^[A-Z][A-Z0-9_]*$")


def level_constant(prefix):
    return S.string(r"^%s[A-Z0-9_]+$" % prefix)


# ---------------------------------------------------------------------------
# room.json

def layer_schema():
    return S.obj({
        "map": S.string(min_length=1, description="the layer's picture (bg0-3.png) or collision image, in the "
                                                  "room's folder"),
        "size_chunks": S.array(S.integer(1, 0xFFFF), length=2, description="[width, height] in 16x8-cell chunks"),
        "size_tiles": S.array(S.integer(U16[0], U16[1]), length=2, description="[width, height] in cells"),
        "tileset": S.nullable(S.string(S.C_NAME, description="its BG tile set's symbol")),
        "scale": S.array(S.integer(-0x80000000, 0xFFFFFFFF), length=2,
                         description="the parallax scale, [x, y] (256: the camera's speed)"),
        "cnt": S.integer(S.U8),
        "unk_1E": S.integer(S.U16),
        "grid_symbol": S.string(S.C_NAME),
        "pad_overrides": S.obj(pattern_properties={r"^[0-9]+$": S.obj(
            {"crc": S.string(r"^[0-9a-f]{8}$"), "pad": S.string(r"^[0-9a-f]*$")}, required="all")},
            description="ROM rooms only: the junk bytes the ROM's chunk streams pad with, by chunk"),
    }, required=["map", "size_chunks", "size_tiles", "tileset", "scale", "cnt", "unk_1E", "grid_symbol"])


U16 = S.U16


def param_record_schema():
    """A parameter record: its raw words, or a view's named fields
    (crashdata.levels.params)."""
    from crashdata.levels import params as P
    raw = S.array(S.integer(S.U32), min_items=1, description="the record's 32-bit words (the raw form)")
    return P.record_schema(S, raw)


def room():
    L = _levels()
    symbols = ["desc", "palette", "entities", "groups", "param_offsets", "params", "type_counts", "layers", "items"]
    root = S.obj({
        "asset": S.obj({"symbol": S.string(S.C_NAME), "packed": S.boolean("LZ77-packed (34 of XS's rooms)")},
                       required="all", description="the level asset: the layers' chunk streams"),
        "record": S.obj({
            "symbol": S.string(S.C_NAME),
            "kind": S.enum(L.ROOM_KINDS[:3], description="ROOM_KIND_*"),
            "blend": S.obj({"effect": S.integer(S.U16), "eva": S.integer(S.U8), "evb": S.integer(S.U8)},
                           description="SetupRoomBlend's colour effect"),
            "unk_0C": S.integer(S.S32, description="N-Tranced's extra rooms: the record's +0x0C when not 0"),
        }, required=["symbol", "kind"], description="the room's struct level_room in the level table"),
        "symbols": S.obj(dict({k: S.string(S.C_NAME) for k in symbols}, links=S.string(S.C_NAME)),
                         required=symbols, description="the C names of the room's objects"),
        "layers": S.obj({name: S.ref("layer") for name in L.LAYERS}, required=["collision"],
                        description="BG1-3, the pooled BG0 and the collision layer"),
        "params": S.array(S.ref("param_record"), description="the entities' parameter records (an entity's "
                                                             "\"param\" is an index)"),
        "links": S.nullable(S.array(S.array(S.integer(S.S32), length=2),
                                    description="[from, to] entity ids: a crate and the crate on top of it")),
        "entities": S.array(S.ref("entity"), description="in id order: the last 256-px column first"),
    }, required=["asset", "symbols", "layers", "params", "entities"])
    defs = {
        "layer": layer_schema(),
        "param_record": param_record_schema(),
        "entity": S.obj({
            "type": S.any_of(S.string(S.C_NAME, description="its name in entity_types.json"),
                             S.integer(S.U16, description="an unnamed type's number")),
            "x": S.integer(S.U16), "y": S.integer(S.U16),
            "param": S.integer(S.U16, description="its parameter record, an index of \"params\""),
        }, required="all"),
    }
    from crashdata.levels import params as P
    defs.update(P.record_schemas(S))
    return S.document("room.schema.json", "A room (room.json)", root, defs,
                      "games/<game>/data/levels/<level>/<slot>/room.json (docs/levels.md, \"Sources and build\")")


# ---------------------------------------------------------------------------
# levels.json

def levels():
    L = _levels()
    entry = S.any_of(ROOM_FOLDER,
                     S.obj({"stage": CONSTANT}, required="all",
                           description="a stage played in an actor category, its constant without CATEGORY_"),
                     S.obj({"kind": S.integer(3, 0xFFFF), "index": S.integer(S.U16)}, required="all",
                           description="N-Tranced: a record with no room, of kind K (3: ROOM_KIND_CATEGORY, "
                                       "a stage of category N)"))
    room_list = S.array(entry)
    cutscene = S.any_of(S.integer(0, 0xFF), S.string(r"^CUTSCENE_[A-Z0-9_]+$"),
                        description="a cutscene of cutscenes.json, its index or constant")
    on_clear = S.obj({
        "power": S.enum(L.WORLD_OPEN_POWERS),
        "open_world": S.boolean(),
        "cutscene": cutscene,
        "mark_cleared": S.boolean(),
        "next_level": S.obj({"if_completion_over": S.integer(0, 99), "cutscene": cutscene,
                             "level": level_constant("LEVEL_")}, required=["if_completion_over", "level"]),
        "credits": S.obj({"cutscene": cutscene}),
    }, description="what clearing the level does (docs/levels.md, \"What clearing a level does\")")
    on_clear["properties"] = S.with_keys(on_clear["properties"], L.ON_CLEAR_KEYS)
    level_props = {
        "constant": level_constant("LEVEL_"),
        "name": S.string(min_length=1),
        "name_text": S.integer(S.S32, description="its name's text id (GetUiText)"),
        "theme": S.integer(0, 0xFF, description="an index of \"themes\""),
        "times": S.array(S.integer(S.U32), length=3, description="time-trial times, centiseconds, loosest first"),
        "mask_assist_deaths": S.integer(S.S32),
        "crate_assist_deaths": S.integer(S.S32),
        "rooms": dict(room_list, minItems=1),
        "bonus": ROOM_FOLDER,
        "gem_path": ROOM_FOLDER,
        "world": S.nullable(S.integer(0, 0xFF, description="its level-select page, or none")),
        "boss": S.boolean("struct level_info.isBoss"),
        "boss_kind": S.enum(L.BOSS_KINDS),
        "on_clear": S.ref("on_clear"),
    }
    level_props = S.with_keys(level_props, L.LEVEL_KEYS)
    theme_props = {
        "constant": level_constant("THEME_"),
        "name": S.string(min_length=1),
        "song": S.any_of(S.string(S.C_NAME, description="a song of gax_manifest.json's song_table"),
                         S.integer(S.U8, description="a song id (N-Tranced's)")),
        "picture": S.string(description="a level-select picture of the graphics manifest"),
        "palette_cycle": S.string(description="one of \"palette_cycles\""),
        "first_room_exit_zone": S.boolean(),
        "palette_glow": S.string(description="one of \"palette_glows\""),
    }
    theme_props = S.with_keys(theme_props, L.THEME_KEYS + L.THEME_OPTIONAL_KEYS)
    category_props = {
        "constant": level_constant("CATEGORY_"),
        "name": S.string(min_length=1),
        "type": S.enum(L.CATEGORY_TYPE_NAMES),
        "spawns": S.string(r"^[a-z0-9_]+$", description="its spawn list, spawns/<name>.json"),
        "anim_table": S.string(r"^[a-z0-9_]+$", description="its animation table, anim_tables/<name>.json"),
    }
    category_props.update({k: S.integer(S.U16) for k in L.CATEGORY_KEYS[4:]})
    category_props.update({k: S.string(min_length=1, description="a %s of the graphics manifest, by path" % kind)
                           for k, kind in L.CATEGORY_GRAPHIC_KEYS.items()})
    world_props = {k: S.integer(0, L.WORLD_KEY_LIMITS[k] - 1) for k in L.WORLD_KEYS}
    world_props["opened_by"] = S.any_of(S.obj({"power": S.enum(L.WORLD_OPEN_POWERS)}, required="all"),
                                        S.obj({"levels_cleared": S.const(True)}, required="all"))
    root = S.obj({
        "rooms": S.array(ROOM_FOLDER, description="the rooms, in room-table order (gRoomNN is the NNth)"),
        "regions": S.array(S.obj({"name": S.string(S.C_NAME), "rooms": S.array(ROOM_FOLDER)}, required="all"),
                           description="the ROM's room regions: each one's C file and its rooms in ROM order"),
        "assets": S.obj({"name": S.string(S.C_NAME), "rooms": S.array(ROOM_FOLDER)}, required="all",
                        description="the ROM order of the level assets"),
        "table": S.obj({"lists": S.array(S.obj({"level": level_constant("LEVEL_"), "rooms": room_list},
                                               required="all")),
                        "records": room_list, "room_lists": S.array(level_constant("LEVEL_"))},
                       required=["lists"], description="the ROM's layout of the level table"),
        "levels": S.array(S.obj(level_props, required=L.LEVEL_KEYS), description="the levels, in id order"),
        "worlds": S.array(S.obj(world_props), description="the level-select pages, in world order"),
        "themes": S.array(S.obj(theme_props, required=L.THEME_KEYS), min_items=1),
        "palette_cycles": S.array(S.obj({"name": S.string(min_length=1), "lists": S.array(S.obj({
            "symbol": S.string(S.C_NAME), "rate": S.integer(1, 0xFF), "direction": S.integer(0, 1),
            "entries": S.array(S.integer(S.U8), min_items=1)}, required="all"), min_items=1)}, required="all")),
        "palette_glows": S.array(S.obj({
            "name": S.string(min_length=1), "rate": S.integer(1, 0xFF),
            "entries": S.obj({"symbol": S.string(S.C_NAME), "values": S.array(S.integer(S.U8), min_items=1)},
                             required="all", description="the BG palette entries it glows"),
            "colors": S.array(S.obj({"symbol": S.string(S.C_NAME),
                                     "values": S.array(S.integer(0, 0x7FFF), min_items=1)}, required="all"),
                              length=2, description="the two 15-bit colours of each entry it goes between")},
            required="all"), description="N-Tranced: the themes' palette glows"),
        "categories": S.array(S.obj(category_props, required=L.CATEGORY_KEYS), min_items=1),
        # N-Tranced's rooms-only levels.json (docs/n_tranced.md)
        "level_names": S.array(S.string(), description="N-Tranced: its levels' folder names, in id order"),
        "tileset_names": S.obj(pattern_properties={r"^0x[0-9A-Fa-f]{8}$": S.string(S.C_NAME)},
                               description="N-Tranced: its tile sets' names by ROM address"),
        "room_layout": S.array(S.enum(L.ROOM_LAYOUT), description="the order of a room's parts in its region"),
        "entity_types": S.integer(1, 0xFFFF, description="the number of entity types (the type counts' length)"),
    }, required=["rooms", "regions", "assets"])
    return S.document("levels.schema.json", "The levels (levels.json)", root, {"on_clear": on_clear},
                      "games/<game>/data/levels/levels.json (docs/levels.md)")


# ---------------------------------------------------------------------------
# The other level files

def spawn_list():
    kind = S.any_of(S.string(S.C_NAME, description="a <FAMILY>_KIND_* name without the prefix"),
                    S.integer(S.U8))
    root = S.obj({
        "symbol": S.string(S.G_SYMBOL),
        "family": S.enum(["POLAR", "JETPACK"]),
        "length": S.integer(1, 0x7FFFFFFF, description="the course length"),
        "spawns": S.array(S.obj({
            "kind": kind, "alt": kind, "bonus": kind, "unk_03": S.integer(S.U8),
            "x": S.integer(S.S32), "y": S.integer(S.S32), "z": S.integer(S.S32, description="depth"),
            "next": S.integer(-1, 0x7FFFFFFF, description="a spawn's index, or -1"),
        }, required=["kind", "x", "y", "z"]), min_items=1, description="in depth order"),
        "end": S.array(S.integer(S.U8), length=4, description="the table's last 4 bytes"),
    }, required=["symbol", "family", "length", "spawns"])
    return S.document("spawn_list.schema.json", "An actor category's spawn list", root,
                      description="games/xs/data/levels/spawns/<name>.json (docs/levels.md, \"Actor categories\")")


def anim_table():
    L = _levels()
    record = {
        "index": S.integer(S.U8, description="the record's first byte, when not its kind"),
        "keyframes": S.string(S.G_SYMBOL), "frames": S.string(S.G_SYMBOL),
        "palette": S.integer(0, 15, description="the OBJ palette bank"),
        "depth": S.integer(0, 0x7FFFFFFF, description="the depth at which it draws unscaled"),
        "box": S.array(S.integer(S.S16), length=6, description="[x, y, z, w, h, d]"),
        "spawn_x": S.integer(S.S32), "spawn_y": S.integer(S.S32),
    }
    record = S.with_keys(record, L.ANIM_RECORD_KEYS)
    comment = S.string()
    array = S.any_of(
        S.const("table"),
        S.obj({"keyframes": S.string(S.G_SYMBOL), "comment": comment,
               "list": S.array(S.array(S.integer(-0x8000, 0xFFFF), length=5,
                                       description="[duration, frame, loop end, loop start, OAM attribute]"),
                               min_items=1)}, required=["keyframes", "list"]),
        S.obj({"frames": S.string(S.G_SYMBOL), "comment": comment,
               "list": S.array(S.string(r"^[a-z0-9_]+/[0-9]+$", description="a frame of the sheet, entity/NN"),
                               min_items=1)}, required=["frames", "list"]),
        S.obj({"player_frames": S.string(S.G_SYMBOL), "comment": comment,
               "list": S.array(S.integer(0, 0xFFFF), min_items=1)}, required=["player_frames", "list"]))
    root = S.obj({
        "symbol": S.string(S.G_SYMBOL),
        "family": S.enum(list(L.ANIM_FAMILIES)),
        "sheet": S.string(min_length=1, description="a framed_sheet of the graphics manifest, by path"),
        "records": S.array(S.obj(record), description="one per actor kind; {} for a kind with none"),
        "arrays": S.array(array, description="the keyframes and frames, with \"table\" where the table goes"),
    }, required=["symbol", "family", "records", "arrays"])
    return S.document("anim_table.schema.json", "An actor family's animation table", root,
                      description="games/xs/data/levels/anim_tables/<name>.json (docs/levels.md, \"Animation tables\")")


def entity_types():
    L = _levels()
    t = {
        "type": S.integer(S.U16),
        "name": S.nullable(S.string(r"^[A-Z][A-Z0-9_]*$", description="ENTITY_<name>")),
        "spawner": S.string(S.C_NAME),
        "comment": S.string(),
        "enemy_kind": S.integer(S.U16),
        "enemy_kind_name": S.string(r"^[A-Z][A-Z0-9_]*$"),
    }
    from crashdata.levels import params as P
    t["params"] = S.enum(list(P.LAYOUTS), description="the view of its parameter records its spawner reads "
                                                      "(crashdata.levels.params)")
    root = S.obj({"types": S.array(S.obj(t, required=["type", "name", "spawner"]),
                                   description="one per gEntitySpawnFuncs slot, in type order")},
                 required="all")
    return S.document("entity_types.schema.json", "The entity types (entity_types.json)", root,
                      description="games/<game>/data/levels/entity_types.json (docs/levels.md, \"Entity types\")")


def crate_kinds():
    root = S.obj({"kinds": S.array(S.obj({"kind": S.integer(S.U8), "name": S.string(r"^[A-Z][A-Z0-9_]*$"),
                                          "comment": S.string()}, required=["kind", "name"]))}, required="all")
    return S.document("crate_kinds.schema.json", "The crate kinds (crate_kinds.json)", root,
                      description="games/xs/data/levels/crate_kinds.json: CRATE_KIND_*")


def collision_legend():
    root = S.obj({"cells": S.array(S.obj({
        "value": S.string(r"^[0-9A-Fa-f]{4}$", description="the cell value, 4 hex digits"),
        "colour": S.string(r"^[0-9A-Fa-f]{6}$", description="its colour in the collision images, RRGGBB"),
        "name": S.string()}, required="all"), max_items=255)}, required="all")
    return S.document("collision_legend.schema.json", "The collision images' legend", root,
                      description="games/<game>/data/levels/collision_legend.json (docs/levels.md, "
                                  "\"Collision layers\")")


def tileset_order():
    root = S.obj(pattern_properties={S.C_NAME: S.array(ROOM_FOLDER)},
                 description="per BG tile set, the order of the rooms whose tiles it holds")
    return S.document("tileset_order.schema.json", "The BG tile sets' room order", root,
                      description="games/<game>/data/levels/tileset_order.json (docs/levels.md, \"Tile sets\")")


# The files of the level data: schema name -> (builder, the files' globs
# under the game's levels directory)
SCHEMAS = {
    "levels": (levels, ["levels.json"]),
    "room": (room, ["*/*/room.json"]),
    "spawn_list": (spawn_list, ["spawns/*.json"]),
    "anim_table": (anim_table, ["anim_tables/*.json"]),
    "entity_types": (entity_types, ["entity_types.json"]),
    "crate_kinds": (crate_kinds, ["crate_kinds.json"]),
    "collision_legend": (collision_legend, ["collision_legend.json"]),
    "tileset_order": (tileset_order, ["tileset_order.json"]),
}
