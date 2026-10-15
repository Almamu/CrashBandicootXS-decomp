#!/usr/bin/env python3
"""graphics/manifest.json: every source under graphics/, what it is, and
where baserom.gba has it (docs/graphics.md, "The manifest").

The build reads the manifest only for the graphics classes (below): the
ROM's layout comes from ldscript.txt and the order of the definitions in
each src/.../data/*.cpp, and every reference is a symbol. The ROM addresses
are kept so the sources can be extracted from baserom.gba again, and to
put a class's members in ROM order.

    tools/gfx_manifest.py check
        Every file under graphics/ belongs to one manifest entry, every
        entry's files exist, every part in an object's range has a class
        and cxx_symbols.txt's graphics and room classes blocks are up to
        date.

    tools/gfx_manifest.py extract [baserom.gba] [--check]
        Writes every source under graphics/ and data/levels/ from the ROM
        (the room data through tools/levels.py extract), and the framed
        sheets' record dumps, docs/entities/. With --check, it
        writes them into a temporary tree instead and compares that with
        the sources: PNGs by size, mode, pixels and palette, the other
        files byte for byte. The tree must come out the same.

    tools/gfx_manifest.py table
        Rewrites docs/data_map.md's "Graphics assets" table (address,
        symbol, kind, source) from the manifest.

    tools/gfx_manifest.py header OUT
    tools/gfx_manifest.py defs OBJECT OUT
    tools/gfx_manifest.py symbols
        The graphics classes (docs/graphics.md, "Graphics classes"): the
        header declaring them (graphics.hpp), the definitions of one
        object's members in ROM order, then its new graphics' (the .inc its
        src/.../data/OBJECT.cpp #includes), and cxx_symbols.txt's block
        mapping each member to its C name.

    tools/gfx_manifest.py make OUT
        graphics.mk's lists that follow the manifest (graphics_manifest.mk):
        the tile sheets by bpp, the BG packages with their bpp, the
        16-colour PNG palettes, the category graphics, the class objects
        and each one's prerequisites (the .inc files of its members).

An entry has its `path` (a file, or a directory ending in `/` that holds
everything under it not claimed by a longer entry) or its `paths`, its
`kind`, its `symbol` and `rom` address, and the kind's parameters:

    bitmap        a Mode 4 picture: 240x160 8bpp, LZ77; `palette` is its
                  uncompressed u16[256] palette
    tiles         a tile sheet, LZ77: `bpp`, `width` (tiles per row), and
                  `obj_sheet` (tools/obj_sheet.py's layout) for a sheet of
                  whole OBJs; `palette` is an LZ77 palette ({symbol, rom,
                  colors}) or, for a sheet whose palette OAM picks at run
                  time, {"obj_palette": paletteId}: the gObjPalettes entry
                  the PNG is drawn with
    bg_package    a struct bg_package picture (tools/bg_map.py): `rom` is
                  its LZ77 tiles; its LZ77 `palette` and `map`, `size` in
                  cells, `bpp`, `affine`, and `obj_sheet` for the title's
                  OBJ packages
    rle_frames    a zero-run-compressed frame set (tools/rle_sprites.py),
                  `rom` to `end`, drawn with the 16 colours at `palette`
    boss_picture  a boss picture's frames (tools/boss_pictures.py), `rom`
                  to `end`, with the 16 colours at `palette`; `paths` are
                  the frames in order
    cell_anim     a category BG0 cell animation: `frames`, and `banks` when
                  each frame carries its cells' palette banks
    bg_picture    a category BG1 picture; `paths` are the picture and its
                  tile set strip
    category_palette
                  a category's 256-colour OBJ palette, a JASC .pal (`colors`:
                  256). Only new ones: the ROM's three are C arrays with other
                  data
    palette_cycle a polar category's BG palette cycle: `path` is a directory
                  of `frames` (32) JASC .pal files, 00.pal to 31.pal, each of
                  BG palettes 0-13 (224 colours), back to back in the ROM
    obj_palettes  the OBJ palettes of gObjPalettes, one JASC .pal per
                  paletteId, named by `names` (NNN_<owner>: the paletteId and
                  what uses it)
and, extracted by their own tools, which find them through the ROM's
tables: sprite_banks (tools/sprite_frames.py extract) and framed_sheet
(tools/split_entities.py, then tools/dump_entities.py; a framed_sheet's
`prefix` names its frames' offsets, FRAMED_<prefix>_<FOLDER>_<NN>, and its
`folder_banks`, {folder: bank}, draws a folder in another of its records'
banks than the first's). The level BG tile
sets have no source: the build generates them from the rooms' pictures,
which tools/levels.py extract writes (docs/levels.md, "Tile sets").

An entry with a `class` is a graphic whose parts are the static const data
members of that class (`TitleScreenBg::palette`), each keeping its C name
(the part's `symbol`) through cxx_symbols.txt. The members are named by
their role: the entry's own symbol is `tiles` (bg_package, tiles),
`bitmap` or `frames` (rle_frames), its `palette` is `palette` and its
`map` is `map`. A palette OAM picks at run time ({"obj_palette": n})
isn't one, nor an rle_frames' palette (the category's, defined with
other data). The manifest's `objects` ({name, rom, end}) are the
src/.../data objects whose graphics are all class members: every part in an
object's ROM range is defined, in ROM order, in the .inc `defs` writes
for it. The rooms are classes too, of tools/levels.py (rooms.hpp).

A new graphic, one the ROM doesn't have, is an entry with no `rom` (nor
its `palette` or `map`): a class, of one of NEW_KINDS, with the `object`
that defines it. Its members come after the ROM's in that object, in
manifest order, so the linker places them and moves what follows
(docs/shifting.md); the ROM's graphics keep their addresses. extract and
table leave it out: the ROM has nothing to extract.

A new category graphic (CATEGORY_KINDS: a cell_anim, bg_picture,
category_palette, framed_sheet or palette_cycle with no `rom`) is no
class and names no object: an actor category of levels.json that names it
(its "cell_anim", "sky", "palette", "sprite_sheet" or "palette_cycle") has
it defined by tools/levels.py categories, before ActorCategory::descriptors
(docs/levels.md, "Category graphics"). graphics_manifest.mk gives its build
rules (CELL_ANIMS, BG_PICTURES, CATEGORY_PALETTES, FRAMED_SHEETS,
PALETTE_CYCLES) as for the ROM's.
"""
import argparse
import json
import re
import shlex
import shutil
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))
import bg_map  # noqa: E402
import boss_pictures  # noqa: E402
import jasc_pal  # noqa: E402
import obj_sheet  # noqa: E402
import rle_sprites  # noqa: E402

ROM_BASE = 0x08000000
MANIFEST = Path("graphics/manifest.json")
DATA_MAP = Path("docs/data_map.md")
TABLE_BEGIN = "<!-- graphics assets: generated by tools/gfx_manifest.py table, do not edit -->"
TABLE_END = "<!-- end of graphics assets -->"

# The kinds another tool extracts (all of a kind in one run), in run order.
FAMILY_TOOLS = {
    "sprite_banks": [["sprite_frames.py", "extract", "{rom}"]],
    "framed_sheet": [["split_entities.py"], ["dump_entities.py"]],
}


def load():
    return json.loads(MANIFEST.read_text())["assets"]


def load_objects():
    return json.loads(MANIFEST.read_text()).get("objects", [])


def entry_paths(e):
    return e["paths"] if "paths" in e else [e["path"]]


def addr(v):
    return int(v, 16)


# The kinds a new graphic (an entry with no `rom`) can be: those whose
# build rules graphics.mk takes from the manifest (graphics_manifest.mk),
# and a cutscene picture (graphics.mk builds every graphics/cutscenes/
# PNG; a slide of data/cutscenes.json shows it). An OBJ sheet's layout is
# code's.
NEW_KINDS = ("tiles", "bg_package", "bitmap")
# Where a new bitmap goes: a cutscene picture, defined with the ROM's.
NEW_BITMAP_DIR, NEW_BITMAP_OBJECT = "cutscenes/", "cutscene_pictures"
# The kinds a new category graphic can be: an actor category names it, and
# tools/levels.py categories defines it (no class, no object).
CATEGORY_KINDS = ("cell_anim", "bg_picture", "category_palette", "framed_sheet",
                  "palette_cycle")
# A palette cycle (ActorPaletteCycle::frames): its frame count (the cycles'
# ends, startFrames/targetFrames, reach frame 31) and the colours of a frame
# (BG palettes 0-13).
PALETTE_CYCLE_FRAMES = 32
PALETTE_CYCLE_COLORS = 14 * 16


def is_new(e):
    """A graphic the ROM doesn't have: no `rom` address."""
    return "rom" not in e


def new_entry_errors(e, objects):
    """What's wrong with a new graphic's entry, if anything."""
    what = "%s (graphics/%s)" % (e.get("symbol", "?"), ", ".join(entry_paths(e)))
    errors = []
    if e["kind"] in CATEGORY_KINDS:
        return category_entry_errors(e, what)
    if e["kind"] not in NEW_KINDS or "obj_sheet" in e:
        errors.append("%s: has no `rom`, but a new graphic is a %s without `obj_sheet`, or a category's %s"
                      % (what, " or ".join(NEW_KINDS), " or ".join(CATEGORY_KINDS)))
    if "class" not in e:
        errors.append("%s: has no `rom`, so it is a new graphic, which needs a `class`" % what)
    if e.get("object") not in objects:
        errors.append("%s: a new graphic names the `object` that defines it, one of the manifest's `objects` (%s)"
                      % (what, ", ".join(objects)))
    for k in ("palette", "map"):
        sub = e.get(k, {})
        if "rom" in sub:
            errors.append("%s: a new graphic, but its %s has a `rom`" % (what, k))
        if k == "palette" and "symbol" in sub and "colors" not in sub:
            errors.append("%s: its palette has no `colors`" % what)
    if e["kind"] == "bg_package" and "symbol" not in e.get("map", {}):
        errors.append("%s: a bg_package has a `map` with its `symbol`" % what)
    if e["kind"] == "bitmap":
        if not e.get("path", "").startswith(NEW_BITMAP_DIR) or e.get("object") != NEW_BITMAP_OBJECT:
            errors.append("%s: a new bitmap is a cutscene picture: a PNG in graphics/%s, defined by %s"
                          % (what, NEW_BITMAP_DIR, NEW_BITMAP_OBJECT))
        pal = e.get("palette", {})
        if "symbol" not in pal or pal.get("colors") != 256:
            errors.append("%s: a cutscene picture has a `palette` with its `symbol` and 256 `colors` (ShowSlidePicture "
                          "finds the bitmap right after it)" % what)
    return errors


def category_entry_errors(e, what):
    """What's wrong with a new category graphic's entry, if anything."""
    errors = []
    for k in ("class", "object", "palette", "map"):
        if k in e:
            errors.append("%s: a new category graphic has no `%s`: the category that names it has it defined "
                          "(levels.py categories)" % (what, k))
    paths = entry_paths(e)
    if e["kind"] == "cell_anim":
        if len(paths) != 1 or not paths[0].endswith(".png"):
            errors.append("%s: a cell_anim is one PNG, its frames stacked top to bottom" % what)
        if not isinstance(e.get("frames"), int) or e["frames"] < 1 or not isinstance(e.get("banks"), bool):
            errors.append("%s: a cell_anim has its `frames` (a count) and `banks` (true or false)" % what)
    elif e["kind"] == "bg_picture":
        if len(paths) != 2 or not all(p.endswith(".png") for p in paths):
            errors.append("%s: a bg_picture's `paths` are its picture and its tile set strip, two PNGs" % what)
    elif e["kind"] == "framed_sheet":
        if len(paths) != 1 or not paths[0].endswith("/"):
            errors.append("%s: a framed_sheet is a directory (its path ends in /) of frame folders and its "
                          "sheet.json" % what)
    elif e["kind"] == "palette_cycle":
        errors += palette_cycle_errors(e, what)
    elif len(paths) != 1 or not paths[0].endswith(".pal") or e.get("colors") != 256:
        errors.append("%s: a category_palette is a JASC .pal of `colors` 256" % what)
    return errors


def palette_cycle_frames(e):
    """A palette_cycle's frame files, under graphics/: 00.pal, 01.pal, ..."""
    return [e["path"] + "%02d.pal" % k for k in range(e.get("frames", 0))]


def palette_cycle_errors(e, what, root=Path(".")):
    """What's wrong with a palette_cycle entry: a directory, `frames` 32,
    and each frame a JASC .pal of 224 colours."""
    if len(entry_paths(e)) != 1 or not e["path"].endswith("/") or e.get("frames") != PALETTE_CYCLE_FRAMES:
        return ["%s: a palette_cycle is a directory (`path` ending in /) of `frames` %d JASC .pal files"
                % (what, PALETTE_CYCLE_FRAMES)]
    errors = []
    for p in palette_cycle_frames(e):
        f = root / "graphics" / p
        if not f.exists():
            errors.append("%s: graphics/%s doesn't exist" % (what, p))
        elif len(jasc_pal.read_jasc(f)) != PALETTE_CYCLE_COLORS:
            errors.append("%s: graphics/%s has %d colours, a frame %d (BG palettes 0-13)"
                          % (what, p, len(jasc_pal.read_jasc(f)), PALETTE_CYCLE_COLORS))
        if errors:
            break  # the first frame that's wrong
    return errors


def category_graphics():
    """The manifest's category graphics (CATEGORY_KINDS), the ROM's and the
    new ones: path (the first) -> entry."""
    return {entry_paths(e)[0]: e for e in load() if e["kind"] in CATEGORY_KINDS}


def category_stem(e):
    """A category graphic's built name: its (first) path without the
    extension, or a framed sheet's or palette cycle's directory without
    the /."""
    if e["kind"] == "framed_sheet":
        return entry_paths(e)[0].rstrip("/")
    if e["kind"] == "palette_cycle":
        return e["path"].rstrip("/")
    return entry_paths(e)[0].rsplit(".", 1)[0]


def category_incs(e):
    """The .inc files a category graphic's definition #includes."""
    stem = category_stem(e)
    if e["kind"] == "cell_anim":
        return [stem + ".pal.inc", stem + "_frames.inc"]
    if e["kind"] == "bg_picture":
        return [stem + ".pal.inc", stem + "_map.inc", stem + "_tiles.img.inc", stem + "_banks.inc"]
    if e["kind"] == "framed_sheet":
        return [stem + ".bin.lz.inc", stem + "_frames.h"]
    if e["kind"] == "palette_cycle":
        return [stem + ".bin.inc"]
    return [stem + ".gbapal.inc"]


# ---------------------------------------------------------------------------
# check

def owner(entries, rel):
    """The entry a file under graphics/ belongs to: the one naming it, or
    the longest directory entry holding it."""
    best = None
    for e in entries:
        for p in entry_paths(e):
            if p == rel:
                return e
            if p.endswith("/") and rel.startswith(p) and (best is None or len(p) > len(best[0])):
                best = (p, e)
    return best[1] if best else None


def check(root=Path(".")):
    entries = load()
    errors = []
    gfx = root / "graphics"
    files = sorted(p.relative_to(gfx).as_posix() for p in gfx.rglob("*") if p.is_file())
    for rel in files:
        if rel != MANIFEST.name and owner(entries, rel) is None:
            errors.append("graphics/%s: in no manifest entry" % rel)
    for e in entries:
        for p in entry_paths(e):
            if not (gfx / p).exists():
                errors.append("%s: graphics/%s doesn't exist" % (e["symbol"], p))
    roms = [addr(e["rom"]) for e in entries if not is_new(e)]
    if roms != sorted(roms):
        errors.append("the manifest isn't sorted by ROM address")
    prefixes = [e.get("prefix") for e in entries if e["kind"] == "framed_sheet"]
    for e in entries:
        if e["kind"] == "framed_sheet" and not (isinstance(e.get("prefix"), str) and re.fullmatch(
                r"[A-Z][A-Z0-9_]*", e["prefix"]) and prefixes.count(e["prefix"]) == 1):
            errors.append("%s: a framed_sheet has its own `prefix`, FRAMED_<prefix>_<FOLDER>_<NN> (A-Z, 0-9, _)"
                          % e["symbol"])
    objects = [o["name"] for o in load_objects()]
    for e in entries:
        if is_new(e):
            errors += new_entry_errors(e, objects)
        elif e["kind"] == "palette_cycle":
            errors += palette_cycle_errors(e, e["symbol"], root)
        elif "object" in e:
            errors.append("%s: has a `rom`, so its object is the one whose range holds it, not `object`"
                          % e["symbol"])
    return errors + check_classes()


# ---------------------------------------------------------------------------
# extract

class Rom:
    def __init__(self, path):
        self.data = Path(path).read_bytes()

    def bytes(self, a, n):
        return self.data[a - ROM_BASE:a - ROM_BASE + n]

    def u16s(self, a, n):
        return list(struct.unpack_from("<%dH" % n, self.data, a - ROM_BASE))

    def lz77(self, a):
        return bg_map.lz77_decompress(self.data, a)


def jasc_rgb(entries):
    return [c for v in entries for c in jasc_pal.to_rgb(v)]


def grit_rgb(entries):
    """grit's 5- to 8-bit expansion (c << 3 | c >> 2), the category
    backgrounds' PNG palettes."""
    return [(c << 3 | c >> 2) for v in entries for c in (v & 31, v >> 5 & 31, v >> 10 & 31)]


def tile_pixels(tile, bpp):
    if bpp == 4:
        return [tile[i // 2] >> 4 * (i & 1) & 0xF for i in range(64)]
    return list(tile)


def tiles_image(data, bpp, width, rgb):
    """Tiles (bpp-bit, 8x8) row-major, `width` tiles per row."""
    from PIL import Image

    size = 8 * bpp
    n = len(data) // size
    img = Image.new("P", (width * 8, -(-n // width) * 8), 0)
    px = img.load()
    for t in range(n):
        tx, ty = t % width * 8, t // width * 8
        for i, c in enumerate(tile_pixels(data[t * size:(t + 1) * size], bpp)):
            px[tx + i % 8, ty + i // 8] = c
    img.putpalette(rgb)
    return img


def save(img, path, bpp=8):
    path.parent.mkdir(parents=True, exist_ok=True)
    if bpp == 4:
        img.save(path, bits=4)
    else:
        img.save(path)


def to_obj_sheet(img, layout, path):
    """A tile picture (its tiles in row-major order) laid out as an OBJ
    sheet (tools/obj_sheet.py from-tiles)."""
    with tempfile.TemporaryDirectory() as tmp:
        src = Path(tmp) / "tiles.png"
        img.save(src)
        ap = argparse.ArgumentParser()
        ap.add_argument("--obj", action="append")
        ap.add_argument("--columns", type=int)
        ap.add_argument("--cell")
        ap.add_argument("--at", action="append")
        args = ap.parse_args(shlex.split(layout))
        args.src, args.dst = str(src), str(path)
        path.parent.mkdir(parents=True, exist_ok=True)
        obj_sheet.from_tiles(args)


def palette_of(rom, e, by_symbol):
    pal = e["palette"]
    if "obj_palette" in pal:
        base = addr(by_symbol["gObjPalettes"]["rom"])
        return rom.u16s(base + 32 * pal["obj_palette"], 16)
    a = addr(pal["rom"])
    if pal.get("lz77"):
        raw = rom.lz77(a)
        entries = list(struct.unpack("<%dH" % (len(raw) // 2), raw))
    else:
        entries = rom.u16s(a, pal["colors"])
    if len(entries) != pal["colors"]:
        sys.exit("gfx_manifest: %s: %d colours, the manifest says %d" % (pal["symbol"], len(entries), pal["colors"]))
    return entries


def extract_bitmap(rom, e, out, by_symbol):
    from PIL import Image

    data = rom.lz77(addr(e["rom"]))
    img = Image.frombytes("P", (240, 160), data)
    img.putpalette(jasc_rgb(palette_of(rom, e, by_symbol)))
    save(img, out / e["path"])


def extract_tiles(rom, e, out, by_symbol):
    data = rom.lz77(addr(e["rom"]))
    rgb = jasc_rgb(palette_of(rom, e, by_symbol))
    if "obj_sheet" in e:
        to_obj_sheet(tiles_image(data, e["bpp"], 1, rgb), e["obj_sheet"], out / e["path"])
        return
    save(tiles_image(data, e["bpp"], e["width"], rgb), out / e["path"], e["bpp"])


def extract_bg_package(rom, e, out, by_symbol):
    w, h = e["size"]
    img, high = bg_map.render(rom.data, palette_of(rom, e, by_symbol), addr(e["rom"]), addr(e["map"]["rom"]),
                              w, h, e["bpp"], e["affine"])
    png = [p for p in entry_paths(e) if p.endswith(".png")][0]
    if "obj_sheet" in e:
        to_obj_sheet(img, e["obj_sheet"], out / png)
    else:
        save(img, out / png)
    hj = [p for p in entry_paths(e) if p.endswith(".json")]
    if bool(hj) != bool(high):
        sys.exit("gfx_manifest: %s: the map's high bytes and the manifest disagree" % e["symbol"])
    if high:
        lines = ['    "%d": "%02X"' % (i, b) for i, b in high.items()]
        (out / hj[0]).write_text("{\n" + ",\n".join(lines) + "\n}\n")


def extract_rle(rom, e, out, by_symbol):
    img = rle_sprites.render(rom.data, addr(e["rom"]), addr(e["end"]), addr(e["palette"]["rom"]))
    save(img, out / e["path"], 4)


def extract_boss(rom, e, out, by_symbol):
    imgs = boss_pictures.render(rom.data, addr(e["rom"]), addr(e["end"]), addr(e["palette"]["rom"]))
    if len(imgs) != len(e["paths"]):
        sys.exit("gfx_manifest: %s: %d frames, the manifest lists %d" % (e["symbol"], len(imgs), len(e["paths"])))
    for img, p in zip(imgs, e["paths"]):
        save(img, out / p, 4)


def nibbles(data, n):
    return [data[i // 2] >> 4 * (i & 1) & 0xF for i in range(n)]


def extract_cell_anim(rom, e, out, by_symbol):
    """All frames stacked top to bottom; a pixel is bank * 16 + the 4bpp
    pixel, the bank being the cell's (bank 0 without per-cell banks)."""
    from PIL import Image

    a = addr(e["rom"])
    pal = rom.u16s(a, 256)
    cols, rows = struct.unpack("<hh", rom.bytes(a + 0x200, 4))
    cells = cols * rows
    bank_bytes = (cells + 7) // 8 * 4 if e["banks"] else 0
    img = Image.new("P", (cols * 8, rows * 8 * e["frames"]), 0)
    px = img.load()
    pos = a + 0x204
    for f in range(e["frames"]):
        tiles = rom.bytes(pos, cells * 32)
        banks = nibbles(rom.bytes(pos + cells * 32, bank_bytes), cells) if bank_bytes else [0] * cells
        for c in range(cells):
            x0, y0 = c % cols * 8, (f * rows + c // cols) * 8
            for i, p in enumerate(tile_pixels(tiles[c * 32:(c + 1) * 32], 4)):
                px[x0 + i % 8, y0 + i // 8] = banks[c] * 16 + p
        pos += cells * 32 + bank_bytes
    img.putpalette(grit_rgb(pal))
    save(img, out / e["path"])


def extract_bg_picture(rom, e, out, by_symbol):
    """The picture (each cell its tile in the cell's bank) and its tile set
    as an 8-pixel-wide strip, each tile in the bank of its first cell."""
    from PIL import Image

    a = addr(e["rom"])
    pal = rom.u16s(a, 256)
    cols, rows, ntiles = struct.unpack("<hhI", rom.bytes(a + 0x200, 8))
    cells = cols * rows
    mp = rom.u16s(a + 0x208, cells)
    tpos = a + 0x208 + 2 * (cells + (cells & 1))
    tiles = [tile_pixels(rom.bytes(tpos + 32 * t, 32), 4) for t in range(ntiles)]
    banks = nibbles(rom.bytes(tpos + 32 * ntiles, (cells + 1) // 2), cells)
    pic = Image.new("P", (cols * 8, rows * 8), 0)
    px = pic.load()
    tile_bank = {}
    for c in range(cells):
        t = mp[c]
        tile_bank.setdefault(t, banks[c])
        for i, p in enumerate(tiles[t]):
            px[c % cols * 8 + i % 8, c // cols * 8 + i // 8] = banks[c] * 16 + p
    strip = Image.new("P", (8, ntiles * 8), 0)
    sx = strip.load()
    for t in range(ntiles):
        for i, p in enumerate(tiles[t]):
            sx[i % 8, t * 8 + i // 8] = tile_bank.get(t, 0) * 16 + p
    for img, p in zip((pic, strip), e["paths"]):
        img.putpalette(grit_rgb(pal))
        save(img, out / p)


def extract_palette_cycle(rom, e, out, by_symbol):
    d = out / e["path"]
    d.mkdir(parents=True, exist_ok=True)
    for k, p in enumerate(palette_cycle_frames(e)):
        jasc_pal.write_jasc(out / p, rom.u16s(addr(e["rom"]) + 2 * PALETTE_CYCLE_COLORS * k, PALETTE_CYCLE_COLORS))


def extract_obj_palettes(rom, e, out, by_symbol):
    d = out / e["path"]
    d.mkdir(parents=True, exist_ok=True)
    for k, name in enumerate(e["names"]):
        jasc_pal.write_jasc(d / (name + ".pal"), rom.u16s(addr(e["rom"]) + 32 * k, 16))


EXTRACT = {
    "bitmap": extract_bitmap,
    "tiles": extract_tiles,
    "bg_package": extract_bg_package,
    "rle_frames": extract_rle,
    "boss_picture": extract_boss,
    "cell_anim": extract_cell_anim,
    "bg_picture": extract_bg_picture,
    "obj_palettes": extract_obj_palettes,
    "palette_cycle": extract_palette_cycle,
}


def run_tool(root, rom_path, cmd):
    argv = [sys.executable, "tools/" + cmd[0]] + [a.format(rom=rom_path) for a in cmd[1:]]
    r = subprocess.run(argv, cwd=root, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    if r.returncode:
        sys.exit("gfx_manifest: %s failed:\n%s" % (" ".join(argv[1:]), r.stdout))


def extract(rom_path, root):
    """Writes graphics/ and data/levels/ under `root` from the ROM."""
    entries = load()
    by_symbol = {e["symbol"]: e for e in entries}
    rom = Rom(rom_path)
    for e in entries:
        if is_new(e):
            continue
        if e["kind"] in EXTRACT:
            EXTRACT[e["kind"]](rom, e, root / "graphics", by_symbol)
        elif e["kind"] not in FAMILY_TOOLS:
            sys.exit("gfx_manifest: %s: unknown kind %s" % (e["symbol"], e["kind"]))
    done = []
    for kind, cmds in FAMILY_TOOLS.items():
        if any(e["kind"] == kind and not is_new(e) for e in entries):
            for cmd in cmds:
                if cmd not in done:
                    run_tool(root, rom_path, cmd)
                    done.append(cmd)
    run_tool(root, rom_path, ["levels.py", "extract", "{rom}"])


def same_file(a, b):
    if a.suffix == ".png":
        from PIL import Image

        ia, ib = Image.open(a), Image.open(b)
        return (ia.size == ib.size and ia.mode == ib.mode and ia.tobytes() == ib.tobytes()
                and (ia.mode != "P" or ia.getpalette() == ib.getpalette()))
    return a.read_bytes() == b.read_bytes()


def compare(got, want, skip=()):
    fa = {p.relative_to(got).as_posix() for p in got.rglob("*") if p.is_file()}
    fb = {p.relative_to(want).as_posix() for p in want.rglob("*") if p.is_file()} - set(skip)
    errors = ["%s/%s: not extracted" % (want, p) for p in sorted(fb - fa)]
    errors += ["%s/%s: extracted, but not in the tree" % (want, p) for p in sorted(fa - fb)]
    errors += ["%s/%s: differs from the ROM's" % (want, p) for p in sorted(fa & fb) if not same_file(got / p, want / p)]
    print("gfx_manifest: %s: %d files compared" % (want, len(fa & fb)))
    return errors


def extract_check(rom_path):
    rom_path = Path(rom_path).resolve()
    with tempfile.TemporaryDirectory(prefix="gfx_manifest_") as tmp:
        root = Path(tmp)
        # the tools, and the hand-written level lists levels.py extract keeps
        shutil.copytree(TOOLS, root / "tools", ignore=shutil.ignore_patterns("agbcc", "gbagfx", "grit", "permuter",
                                                                              "__pycache__", "*.c", "*.h"))
        (root / "data/levels").mkdir(parents=True)
        for f in Path("data/levels").glob("*.json"):
            shutil.copy(f, root / "data/levels" / f.name)
        # and the animation tables, whose arrays (their symbols, lengths and
        # order) levels.py extract keeps
        (root / "data/levels/anim_tables").mkdir()
        for f in Path("data/levels/anim_tables").glob("*.json"):
            shutil.copy(f, root / "data/levels/anim_tables" / f.name)
        # the actor kinds, which name the actor categories' spawns
        (root / "include/constants").mkdir(parents=True)
        shutil.copy("include/constants/actor_kinds.h", root / "include/constants")
        (root / "graphics").mkdir()
        shutil.copy(MANIFEST, root / MANIFEST)
        # the songs, which name the level themes' music (levels.json "themes")
        (root / "sound").mkdir()
        shutil.copy("sound/gax_manifest.json", root / "sound")
        (root / "baserom.gba").symlink_to(rom_path)
        extract("baserom.gba", root)
        errors = check()
        # the new graphics aren't the ROM's
        new = [p for e in load() if is_new(e) for p in entry_paths(e)]
        errors += compare(root / "graphics", Path("graphics"), skip=new)
        errors += compare(root / "data/levels", Path("data/levels"))
        errors += compare(root / "docs/entities", Path("docs/entities"))
    return errors


# ---------------------------------------------------------------------------
# classes

GENERATED = "Generated by tools/gfx_manifest.py from graphics/manifest.json - do not edit."
SYMBOLS = Path("cxx_symbols.txt")
SYMBOLS_BEGIN = "# graphics classes: generated by tools/gfx_manifest.py symbols, do not edit"
SYMBOLS_END = "# end of graphics classes"
# The classes' namespace (#1033, docs/cplusplus.md "Namespaces").
NAMESPACE = ("Engine", "Gfx")


class Part:
    """A class member: `member` of `cls`, C name `symbol` at `rom`, a
    `ctype` array of `bound` entries (empty: sized by its initializer),
    initialised by the bytes `inc` (graphics.mk), described by `what`. A new
    graphic's parts have no `rom` (None)."""

    def __init__(self, cls, member, sub, ctype, inc, what, bound="", aligned=True):
        self.cls, self.member, self.symbol = cls, member, sub["symbol"]
        self.rom = addr(sub["rom"]) if "rom" in sub else None
        self.ctype, self.inc, self.what, self.bound, self.aligned = ctype, inc, what, bound, aligned

    def mangled(self):
        """`_Q36Engine3Gfx14SmallFontSheet.tiles`: g++ 2.9's name of the
        static data member, its class qualified by NAMESPACE."""
        names = NAMESPACE + (self.cls,)
        return "_Q%d%s.%s" % (len(names), "".join("%d%s" % (len(n), n) for n in names), self.member)

    def definition(self):
        attr = " __attribute__((aligned(4)))" if self.aligned else ""
        return "const %s %s::%s::%s[%s]%s = {\n#include \"%s\"\n};\n" % (self.ctype, "::".join(NAMESPACE), self.cls,
                                                                        self.member, self.bound, attr, self.inc)


def picture(e):
    """The entry's PNG, without graphics/ and .png."""
    return [p for p in entry_paths(e) if p.endswith(".png")][0][:-4]


def class_parts(e):
    """The members of an entry's class, in declaration order."""
    cls, kind, stem = e["class"], e["kind"], picture(e)
    pal = e.get("palette", {})
    parts = []
    if "symbol" in pal and kind != "rle_frames":
        if kind == "bitmap":
            parts.append(Part(cls, "palette", pal, "u16", stem + ".pal.inc", "%d colours" % pal["colors"],
                              bound=str(pal["colors"])))
        else:
            parts.append(Part(cls, "palette", pal, "u8", stem + ".gbapal.lz.inc", "%d colours, LZ77" % pal["colors"]))
    if kind == "bg_package":
        parts.append(Part(cls, "tiles", e, "u8", stem + ".img.bin.lz.inc", "%dbpp tiles, LZ77" % e["bpp"]))
        what = "OBJ tile map" if "obj_sheet" in e else "affine BG map" if e["affine"] else "text BG map"
        parts.append(Part(cls, "map", e["map"], "u8", stem + ".map.bin.lz.inc", what + ", LZ77"))
    elif kind == "tiles":
        parts.append(Part(cls, "tiles", e, "u8", stem + ".img.bin.lz.inc",
                          "%dbpp %s, LZ77" % (e["bpp"], "OBJ tiles" if "obj_sheet" in e else "tiles")))
    elif kind == "bitmap":
        # no aligned(4): it is right after its word-aligned 512-byte palette
        parts.append(Part(cls, "bitmap", e, "u8", stem + ".img.bin.lz.inc", "Mode 4 bitmap, 240x160 8bpp, LZ77",
                          aligned=False))
    elif kind == "rle_frames":
        # its palette, the category's (or Yeti::palette), is defined with other data
        parts.append(Part(cls, "frames", e, "u8", stem + ".inc", "zero-run-compressed OBJ frames", aligned=False))
    else:
        sys.exit("gfx_manifest: %s: a %s can't have a class" % (e["symbol"], kind))
    return parts


def class_entries(entries=None):
    # a new category graphic can't be a class (category_entry_errors says so)
    return [e for e in (entries or load()) if "class" in e and not (is_new(e) and e["kind"] in CATEGORY_KINDS)]


def wrap(text, first, rest, width=78):
    """A comment's lines: `first` before the first, `rest` before the others."""
    lines, line = [], first
    for word in text.split():
        if len(line) + 1 + len(word) > width and line != first:
            lines.append(line)
            line = rest + " " + word
        else:
            line += " " + word
    lines.append(line)
    return lines


def class_comment(e):
    """The class's comment: its PNG, what it is (`about`) and its kind."""
    pal = e.get("palette", {})
    if e["kind"] == "bg_package":
        what = "an OBJ tile map" if "obj_sheet" in e else "an affine BG map" if e["affine"] else "a text BG map"
        kind = "A struct bg_package picture: %dx%d cells, %dbpp, %s" % (e["size"][0], e["size"][1], e["bpp"], what)
    elif e["kind"] == "tiles":
        kind = "A%s %dbpp tile sheet%s" % ("n" if e["bpp"] == 8 else "", e["bpp"], " of whole OBJs" if "obj_sheet" in e else "")
        if "obj_palette" in pal:
            kind += "; OAM picks its palette at run time"
    elif e["kind"] == "rle_frames":
        kind = "A zero-run-compressed OBJ frame set (tools/rle_sprites.py), the frames back to back"
    else:
        kind = "A Mode 4 bitmap right after its palette (a slide points at the palette)"
    text = "graphics/%s.png: %s. %s." % (picture(e), e["about"], kind) if "about" in e else \
        "graphics/%s.png. %s." % (picture(e), kind)
    lines = wrap(text, "/*", " *", 75)
    return "\n".join(lines[:-1] + [lines[-1] + " */"])


def header():
    out = ["/* %s */" % GENERATED, "",
           "/* The graphics classes (docs/graphics.md, \"Graphics classes\"): one",
           " * class per graphic of graphics/manifest.json that has a `class`, its",
           " * parts static const data members. Each is defined in the src/.../data",
           " * object of its ROM range (gfx_manifest.py defs) and keeps its C name",
           " * through cxx_symbols.txt. They are in the namespace %s (#1033," % "::".join(NAMESPACE),
           " * docs/cplusplus.md \"Namespaces\"). */",
           "#ifndef GUARD_GRAPHICS_HPP", "#define GUARD_GRAPHICS_HPP", "",
           "extern \"C\" {", "#include \"gba/types.h\"", "}", ""]
    for n in NAMESPACE:
        out += ["namespace " + n, "{"]
    for e in class_entries():
        parts = class_parts(e)
        decls = ["    static const %s %s[%s];" % (p.ctype, p.member, p.bound) for p in parts]
        width = max(len(d) for d in decls)
        out += ["", class_comment(e), "class %s" % e["class"], "{", "public:"]
        out += ["%s // %s - %s" % (d.ljust(width), p.symbol, p.what) for d, p in zip(decls, parts)]
        out.append("};")
    out += [""] + ["} // namespace " + n for n in reversed(NAMESPACE)]
    out += ["", "#endif // GUARD_GRAPHICS_HPP", ""]
    return "\n".join(out)


def object_parts(name):
    """The members defined by the object `name`: the ROM's in its range, in
    ROM order, then the new graphics' that name it, in manifest order."""
    objs = {o["name"]: o for o in load_objects()}
    if name not in objs:
        sys.exit("gfx_manifest: no object %s in the manifest" % name)
    lo, hi = addr(objs[name]["rom"]), addr(objs[name]["end"])
    entries = class_entries()
    parts = [p for e in entries if not is_new(e) for p in class_parts(e) if lo <= p.rom < hi]
    new = [p for e in entries if is_new(e) and e.get("object") == name for p in class_parts(e)]
    return sorted(parts, key=lambda p: p.rom) + new


def defs(name):
    out = ["/* %s */" % GENERATED]
    for p in object_parts(name):
        where = "0x%08X" % p.rom if p.rom is not None else "new, not in the ROM"
        out += ["", "/* %s: %s::%s (%s), %s */" % (where, p.cls, p.member, p.symbol, p.what), p.definition()[:-1]]
    return "\n".join(out) + "\n"


def symbols_block():
    lines = [SYMBOLS_BEGIN,
             "# The graphics classes (graphics/manifest.json, docs/graphics.md \"Graphics",
             "# classes\"): each part keeps its C name."]
    lines += ["%s %s" % (p.mangled(), p.symbol) for e in class_entries() for p in class_parts(e)]
    return "\n".join(lines + [SYMBOLS_END])


def split_symbols():
    text = SYMBOLS.read_text()
    if SYMBOLS_BEGIN not in text or SYMBOLS_END not in text:
        sys.exit("gfx_manifest: %s has no graphics classes markers" % SYMBOLS)
    head, rest = text.split(SYMBOLS_BEGIN, 1)
    old, tail = rest.split(SYMBOLS_END, 1)
    return head, SYMBOLS_BEGIN + old + SYMBOLS_END, tail


def write_symbols():
    head, _, tail = split_symbols()
    SYMBOLS.write_text(head + symbols_block() + tail)


def check_classes():
    """The classes' names, the objects' ranges and cxx_symbols.txt's block."""
    errors = []
    entries = class_entries()
    names = [e["class"] for e in entries]
    errors += ["class %s: more than one graphic" % n for n in sorted(set(n for n in names if names.count(n) > 1))]
    in_objects = set()
    for o in load_objects():
        lo, hi = addr(o["rom"]), addr(o["end"])
        for e in load():
            subs = [e] + [e[k] for k in ("palette", "map") if "symbol" in e.get(k, {})]
            if is_new(e):
                if e.get("object") == o["name"] and "class" in e:
                    in_objects.update(s["symbol"] for s in subs)
                continue
            for s in subs:
                if lo <= addr(s["rom"]) < hi:
                    if "class" not in e:
                        errors.append("%s: in object %s, but its graphic has no class" % (s["symbol"], o["name"]))
                    in_objects.add(s["symbol"])
    for e in entries:
        for p in class_parts(e):
            if p.symbol not in in_objects:
                errors.append("%s::%s (%s): in no object's range" % (p.cls, p.member, p.symbol))
    if split_symbols()[1] != symbols_block():
        errors.append("%s: the graphics classes block is out of date (tools/gfx_manifest.py symbols)" % SYMBOLS)
    # the rooms' classes (tools/levels.py), whose block sits next to it
    import levels
    if levels.room_symbols_error():
        errors.append(levels.room_symbols_error())
    return errors


def write_if_changed(path, text):
    path = Path(path)
    if not path.exists() or path.read_text() != text:
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text)


# ---------------------------------------------------------------------------
# graphics_manifest.mk

def make_fragment():
    """graphics.mk's lists that follow the manifest: the tile sheets
    (TILE_SHEETS_4BPP, _8BPP: the `tiles` that aren't OBJ sheets), the BG
    packages (BG_PACKAGES, BG_PACKAGE_BPP_<package>, BG_PACKAGES_AFFINE),
    the PNG palettes of 16 colours (PALETTE_COLORS_16), the category
    graphics (CELL_ANIMS, CELL_ANIM_FRAMES_<anim>, CELL_ANIM_BANKS_<anim>,
    BG_PICTURES, CATEGORY_PALETTES, FRAMED_SHEETS with FRAMED_NAME_<sheet>,
    PALETTE_CYCLES with PALETTE_CYCLE_FRAMES_<cycle>, and the new ones'
    sources and .inc files, CATEGORY_GRAPHICS_NEW,
    CATEGORY_GRAPHICS_NEW_INCS), the class objects
    (GRAPHICS_CLASS_OBJECTS) and each object's prerequisites: the .inc
    files its members' definitions #include."""
    entries = load()
    out = ["# Generated by tools/gfx_manifest.py make from graphics/manifest.json - do not edit.", ""]

    def var(name, items):
        out.append("%s := %s" % (name, " ".join(items)))

    stems = [(e, picture(e)) for e in entries if any(p.endswith(".png") for p in entry_paths(e))]
    for bpp in (4, 8):
        var("TILE_SHEETS_%dBPP" % bpp, [s for e, s in stems if e["kind"] == "tiles" and "obj_sheet" not in e
                                        and e["bpp"] == bpp])
    packages = [(e, s) for e, s in stems if e["kind"] == "bg_package"]
    var("BG_PACKAGES", [s for e, s in packages])
    var("BG_PACKAGES_AFFINE", [s for e, s in packages if e["affine"]])
    out += ["BG_PACKAGE_BPP_%s := %d" % (s, e["bpp"]) for e, s in packages]
    var("PALETTE_COLORS_16", [s for e, s in stems if e["kind"] in ("tiles", "bg_package")
                              and e.get("palette", {}).get("colors") == 16])
    # the category graphics (docs/levels.md, "Category graphics"): the cell
    # animations with their frame counts and whether they carry banks, the
    # BG1 pictures and the new OBJ palettes, and the new ones' sources and
    # .inc files, which levels.mk makes actor_categories' prerequisites
    cells = [e for e in entries if e["kind"] == "cell_anim"]
    var("CELL_ANIMS", [category_stem(e) for e in cells])
    for e in cells:
        out.append("CELL_ANIM_FRAMES_%s := %d" % (category_stem(e), e["frames"]))
        out.append("CELL_ANIM_BANKS_%s :=%s" % (category_stem(e), " banks" if e["banks"] else ""))
    var("BG_PICTURES", [category_stem(e) for e in entries if e["kind"] == "bg_picture"])
    var("CATEGORY_PALETTES", [category_stem(e) for e in entries if e["kind"] == "category_palette"])
    # the framed sprite sheets, by their directory, with their frames'
    # FRAMED_<prefix> (graphics.mk)
    sheets = [e for e in entries if e["kind"] == "framed_sheet"]
    var("FRAMED_SHEETS", [category_stem(e) for e in sheets])
    out += ["FRAMED_NAME_%s := %s" % (category_stem(e), e["prefix"]) for e in sheets]
    # the palette cycles with their frames' sources
    cycles = [e for e in entries if e["kind"] == "palette_cycle"]
    var("PALETTE_CYCLES", [category_stem(e) for e in cycles])
    for e in cycles:
        out.append("PALETTE_CYCLE_FRAMES_%s := %s" % (category_stem(e), " ".join(
            "graphics/" + p for p in palette_cycle_frames(e))))
    new = [e for e in entries if e["kind"] in CATEGORY_KINDS and is_new(e)]

    def new_sources(e):
        if e["kind"] == "palette_cycle":
            return ["graphics/" + p for p in palette_cycle_frames(e)]
        return ["graphics/" + p if not p.endswith("/") else
                "graphics/%ssheet.json $(wildcard graphics/%s*/*.png)" % (p, p) for p in entry_paths(e)]
    var("CATEGORY_GRAPHICS_NEW", [s for e in new for s in new_sources(e)])
    var("CATEGORY_GRAPHICS_NEW_INCS", ["$(GRAPHICS_BUILDDIR)/" + i for e in new for i in category_incs(e)])
    objects = [o["name"] for o in load_objects()]
    var("GRAPHICS_CLASS_OBJECTS", objects)
    for name in objects:
        incs = []
        for p in object_parts(name):
            if p.inc not in incs:
                incs.append(p.inc)
        out.append("$(call data_obj,%s): %s" % (name, " ".join("$(GRAPHICS_BUILDDIR)/" + i for i in incs)))
    return "\n".join(out) + "\n"


# ---------------------------------------------------------------------------
# table

def table():
    rows = ["| ROM | symbol | kind | source (graphics/) |", "|---|---|---|---|"]
    for e in load():
        if is_new(e):
            continue
        paths = entry_paths(e)
        src = ", ".join("`%s`" % p for p in paths[:2]) + (" ... (%d files)" % len(paths) if len(paths) > 2 else "")
        rows.append("| `%s` | `%s` | %s | %s |" % (e["rom"][2:], e["symbol"], e["kind"], src))
        for part in ("palette", "map"):
            sub = e.get(part, {})
            if "symbol" in sub and "rom" in sub:
                rows.append("| `%s` | `%s` | %s | %s (its %s) |" % (sub["rom"][2:], sub["symbol"], part, src, part))
    rows.sort(key=lambda r: r.split("|")[1] if r.startswith("| `") else "")
    text = DATA_MAP.read_text()
    if TABLE_BEGIN not in text or TABLE_END not in text:
        sys.exit("gfx_manifest: %s has no graphics assets markers" % DATA_MAP)
    head, rest = text.split(TABLE_BEGIN, 1)
    _, tail = rest.split(TABLE_END, 1)
    DATA_MAP.write_text(head + TABLE_BEGIN + "\n" + "\n".join(rows) + "\n" + TABLE_END + tail)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    sub = ap.add_subparsers(dest="mode", required=True)
    sub.add_parser("check")
    p = sub.add_parser("extract")
    p.add_argument("rom", nargs="?", default="baserom.gba")
    p.add_argument("--check", action="store_true")
    sub.add_parser("table")
    p = sub.add_parser("header")
    p.add_argument("out")
    p = sub.add_parser("defs")
    p.add_argument("object")
    p.add_argument("out")
    sub.add_parser("symbols")
    p = sub.add_parser("make")
    p.add_argument("out")
    args = ap.parse_args()
    if args.mode in ("header", "defs", "symbols", "make"):
        if args.mode == "header":
            write_if_changed(args.out, header())
        elif args.mode == "make":
            Path(args.out).parent.mkdir(parents=True, exist_ok=True)
            Path(args.out).write_text(make_fragment())
        elif args.mode == "defs":
            Path(args.out).parent.mkdir(parents=True, exist_ok=True)
            Path(args.out).write_text(defs(args.object))
        else:
            write_symbols()
        return
    if args.mode == "check":
        errors = check()
    elif args.mode == "extract" and args.check:
        errors = extract_check(args.rom)
    elif args.mode == "extract":
        extract(args.rom, Path("."))
        errors = []
    else:
        table()
        errors = []
    if errors:
        sys.exit("\n".join(errors[:50] + (["... and %d more" % (len(errors) - 50)] if len(errors) > 50 else [])))
    if args.mode != "table":
        print("gfx_manifest: OK")


if __name__ == "__main__":
    main()
