"""The graphics' JSON Schemas (#1167, docs/editors.md).

The graphics manifest (games/<game>/graphics/manifest.json, its kinds and
each kind's keys: crashdata.graphics.manifest), a sprite bank's bank.json
(crashdata.graphics.sprite_frames), a framed sheet's sheet.json
(crashdata.graphics.framed_gfx) and logos/universal_high.json. The
manifest's own checks (`tools/gfx_manifest.py check`: every file has an
entry, the classes, cxx_symbols.txt) come after the schema's.
"""
from crashdata.common import schema as S

ADDRESS = S.string(r"^0x0[89][0-9A-Fa-f]{6}$", description="a ROM address, 0x08......")
PATH = S.string(r"^[^/].*$", description="a path under the graphics directory (a directory ends in /)")

# Each kind's own keys (besides "kind", "symbol", "rom" and "path" or
# "paths"), from what crashdata.graphics reads for it (manifest.py's
# docstring has what each means).
PALETTE = {"symbol": S.string(S.C_NAME), "rom": ADDRESS, "colors": S.integer(1, 256),
           "lz77": S.boolean("the palette is LZ77-packed"),
           "obj_palette": S.integer(S.U16, description="a sheet whose palette OAM picks: the gObjPalettes entry "
                                                       "the PNG is drawn with")}
OBJ_SHEET = S.string(r"^--", description="a sheet of whole OBJs: tools/obj_sheet.py's layout options")
CLASS = {"class": S.string(S.C_NAME, description="the graphics class whose static members its parts are"),
         "about": S.string(), "object": S.string(S.C_NAME, description="a new graphic: the object defining it")}


def kind_keys():
    """{kind: {key: schema}}: each kind's own keys."""
    palette = S.obj(PALETTE)
    return {
        "bitmap": dict(CLASS, palette=palette),
        "tiles": dict(CLASS, bpp=S.enum([4, 8]), width=S.integer(1, 1024, description="tiles per row"),
                      obj_sheet=OBJ_SHEET, palette=palette,
                      packed=S.boolean("false: the raw tiles, `bytes` long (LZ77-packed by default)"),
                      bytes=S.integer(1, None, description="a raw sheet's length in bytes")),
        "bg_package": dict(CLASS, palette=palette, map=S.obj({"symbol": S.string(S.C_NAME), "rom": ADDRESS}),
                           size=S.array(S.integer(1, 1024), length=2, description="[width, height] in cells"),
                           bpp=S.enum([4, 8]), affine=S.boolean(),
                           obj_sheet=OBJ_SHEET),
        "rle_frames": dict(CLASS, end=ADDRESS, palette=palette),
        "boss_picture": {"end": ADDRESS, "palette": palette},
        "cell_anim": {"frames": S.integer(1, 0xFFFF), "banks": S.boolean("each frame carries its palette banks")},
        "bg_picture": {},
        "category_palette": {"colors": S.const(256)},
        "palette_cycle": {"frames": S.integer(1, 0xFF)},
        "obj_palettes": {"names": S.array(S.string(r"^[0-9]{3}_[a-z0-9_]+$"))},
        "sprite_banks": {"names": S.array(S.string()), "piece_sizes": S.string()},
        "framed_sheet": {"prefix": S.string(r"^[A-Z0-9_]+$", description="FRAMED_<prefix>_<FOLDER>_<NN>"),
                         "folder_banks": S.obj(additional=S.integer(S.U8),
                                               description="{folder: bank}: a folder drawn in another bank"),
                         "view_palette": S.any_of(S.string(S.C_NAME), ADDRESS,
                                                  description="the palette the frames' PNGs are drawn in")},
    }


def manifest():
    entries = []
    for kind, keys in kind_keys().items():
        props = {"kind": S.const(kind), "symbol": S.string(S.C_NAME), "rom": ADDRESS, "path": PATH,
                 "paths": S.array(PATH, min_items=1)}
        props.update(keys)
        entries.append(S.any_of(S.obj(props, required=["kind", "symbol", "path"]),
                                S.obj(props, required=["kind", "symbol", "paths"]),
                                description="a %s" % kind))
    root = S.obj({
        "assets": S.array(S.ref("entry"), description="every source under the graphics directory"),
        "objects": S.array(S.obj({"name": S.string(S.C_NAME), "rom": ADDRESS, "end": ADDRESS}, required="all"),
                           description="the data objects whose graphics are all class members"),
    }, required=["assets"])
    entry = {"anyOf": entries}
    return S.document("graphics_manifest.schema.json", "The graphics manifest (manifest.json)", root,
                      {"entry": entry}, "games/<game>/graphics/manifest.json (docs/graphics.md, \"The manifest\")")


def sprite_bank():
    from crashdata.graphics import sprite_frames as F
    shapes = sorted("%dx%d" % s for s in F.SHAPES)
    piece = S.array(prefix=[S.enum(shapes, description="its size in pixels, WxH"), S.integer(-0x8000, 0x7FFF),
                            S.integer(-0x8000, 0x7FFF), S.const("own_png")], items=False, min_items=3, max_items=4,
                    description="[\"WxH\", x, y] from the sprite's position, or with \"own_png\"")
    tiles = S.any_of(S.const("own"), S.obj({"frame": S.integer(0, 0xFFFF), "tile": S.integer(0, 0xFFFF),
                                            "bank": S.integer(S.U8)}, required=["frame", "tile"]),
                     description="where the frame's tile run starts in the pool")
    frame = S.obj({
        "origin": S.array(S.integer(-0x8000, 0x7FFF), length=2),
        "pieces": S.array(piece),
        "layout": S.enum(sorted(F.LAYOUT_STRUCTS), description="the layout type, which struct the frame is"),
        "tiles": tiles,
        "palettes": S.array(S.integer(S.U16), description="the paletteIds of the animations that show it"),
        "boxes": S.array(S.array(S.integer(-0x8000, 0x7FFF), length=4), description="a new frame's boxes, "
                                                                                     "[x, y, w, h]"),
        "anchor": S.array(S.integer(-0x8000, 0x7FFF), length=2, description="a new frame's anchor"),
    }, required=["pieces", "tiles"])
    root = S.obj({
        "frames": S.array(frame, description="in frame order"),
        "new_steps": S.obj(pattern_properties={r"^[0-9]+$": S.array(S.integer(0, 0xFFFF))},
                           description="{animation: [frame, ...]}: frames appended to its sequence"),
    }, required=["frames"])
    return S.document("sprite_bank.schema.json", "A sprite bank's frames (bank.json)", root,
                      description="games/<game>/graphics/sprites/bankNN_<name>/bank.json (docs/graphics.md, "
                                  "\"Sprite banks\")")


def sprite_sheet():
    root = S.obj({
        "sheet_rom_address": ADDRESS,
        "folders": S.array(S.obj({
            "folder": S.string(r"^[a-z0-9_]+$"), "frames": S.integer(1, 0xFFFF),
            "records": S.array(S.integer(S.U8), min_items=1, description="the animation records drawing it"),
            "palette": S.string(S.C_NAME), "bank": S.integer(S.U8),
            "other_banks": S.array(S.integer(S.U8))}, required=["folder", "frames"]),
            description="the entity folders, in the sheet's byte order"),
    }, required=["folders"])
    return S.document("sprite_sheet.schema.json", "A framed sprite sheet's folders (sheet.json)", root,
                      description="games/<game>/graphics/**/sprite_sheet/sheet.json (docs/graphics.md)")


def palette_high_bits():
    root = S.obj(pattern_properties={r"^[0-9]+$": S.string(r"^[0-9A-Fa-f]{2}$")},
                 description="{palette entry: the high byte}: the bits a PNG palette can't hold")
    return S.document("palette_high_bits.schema.json", "A palette's extra bits (universal_high.json)", root,
                      description="games/xs/graphics/logos/universal_high.json")


# schema name -> (builder, the files' globs under the game's graphics directory)
SCHEMAS = {
    "graphics_manifest": (manifest, ["manifest.json"]),
    "sprite_bank": (sprite_bank, ["sprites/*/bank.json"]),
    "sprite_sheet": (sprite_sheet, ["**/sprite_sheet/sheet.json"]),
    "palette_high_bits": (palette_high_bits, ["logos/*_high.json"]),
}
