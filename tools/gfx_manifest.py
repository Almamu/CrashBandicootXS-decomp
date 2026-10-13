#!/usr/bin/env python3
"""graphics/manifest.json: every source under graphics/, what it is, and
where baserom.gba has it (docs/graphics.md, "The manifest").

The build reads the manifest only for the graphics classes (below): the
ROM's layout comes from ldscript.txt and the order of the definitions in
each src/data/*.cpp, and every reference is a symbol. The ROM addresses
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
    tools/gfx_manifest.py objects
        The graphics classes (docs/graphics.md, "Graphics classes"): the
        header declaring them (graphics.hpp), the definitions of one
        object's members in ROM order (the .inc its src/data/OBJECT.cpp
        #includes), cxx_symbols.txt's block mapping each member to its C
        name, and the objects that have definitions.

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
    obj_palettes  the OBJ palettes of gObjPalettes, one JASC .pal per
                  paletteId, named by `names` (NNN_<owner>: the paletteId and
                  what uses it)
and, extracted by their own tools, which find them through the ROM's
tables: sprite_banks (tools/sprite_frames.py extract) and framed_sheet
(tools/split_entities.py, then tools/dump_entities.py; a framed_sheet's
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
src/data objects whose graphics are all class members: every part in an
object's ROM range is defined, in ROM order, in the .inc `defs` writes
for it. The rooms are classes too, of tools/levels.py (rooms.hpp).
"""
import argparse
import json
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
    roms = [addr(e["rom"]) for e in entries]
    if roms != sorted(roms):
        errors.append("the manifest isn't sorted by ROM address")
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
        if e["kind"] in EXTRACT:
            EXTRACT[e["kind"]](rom, e, root / "graphics", by_symbol)
        elif e["kind"] not in FAMILY_TOOLS:
            sys.exit("gfx_manifest: %s: unknown kind %s" % (e["symbol"], e["kind"]))
    done = []
    for kind, cmds in FAMILY_TOOLS.items():
        if any(e["kind"] == kind for e in entries):
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
        # the actor categories, which name the level table's stages
        (root / "include/constants").mkdir(parents=True)
        shutil.copy("include/constants/categories.h", root / "include/constants")
        (root / "graphics").mkdir()
        shutil.copy(MANIFEST, root / MANIFEST)
        (root / "baserom.gba").symlink_to(rom_path)
        extract("baserom.gba", root)
        errors = check()
        errors += compare(root / "graphics", Path("graphics"))
        errors += compare(root / "data/levels", Path("data/levels"))
        errors += compare(root / "docs/entities", Path("docs/entities"))
    return errors


# ---------------------------------------------------------------------------
# classes

GENERATED = "Generated by tools/gfx_manifest.py from graphics/manifest.json - do not edit."
SYMBOLS = Path("cxx_symbols.txt")
SYMBOLS_BEGIN = "# graphics classes: generated by tools/gfx_manifest.py symbols, do not edit"
SYMBOLS_END = "# end of graphics classes"


class Part:
    """A class member: `member` of `cls`, C name `symbol` at `rom`, a
    `ctype` array of `bound` entries (empty: sized by its initializer),
    initialised by the bytes `inc` (graphics.mk), described by `what`."""

    def __init__(self, cls, member, sub, ctype, inc, what, bound="", aligned=True):
        self.cls, self.member, self.symbol, self.rom = cls, member, sub["symbol"], addr(sub["rom"])
        self.ctype, self.inc, self.what, self.bound, self.aligned = ctype, inc, what, bound, aligned

    def mangled(self):
        return "_%d%s.%s" % (len(self.cls), self.cls, self.member)

    def definition(self):
        attr = " __attribute__((aligned(4)))" if self.aligned else ""
        return "const %s %s::%s[%s]%s = {\n#include \"%s\"\n};\n" % (self.ctype, self.cls, self.member, self.bound,
                                                                    attr, self.inc)


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
    return [e for e in (entries or load()) if "class" in e]


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
           " * parts static const data members. Each is defined in the src/data",
           " * object of its ROM range (gfx_manifest.py defs) and keeps its C name",
           " * through cxx_symbols.txt. */",
           "#ifndef GUARD_GRAPHICS_HPP", "#define GUARD_GRAPHICS_HPP", "",
           "extern \"C\" {", "#include \"gba/types.h\"", "}"]
    for e in class_entries():
        parts = class_parts(e)
        decls = ["    static const %s %s[%s];" % (p.ctype, p.member, p.bound) for p in parts]
        width = max(len(d) for d in decls)
        out += ["", class_comment(e), "class %s" % e["class"], "{", "public:"]
        out += ["%s // %s - %s" % (d.ljust(width), p.symbol, p.what) for d, p in zip(decls, parts)]
        out.append("};")
    out += ["", "#endif // GUARD_GRAPHICS_HPP", ""]
    return "\n".join(out)


def object_parts(name):
    """The members defined by the object `name`, in ROM order."""
    objs = {o["name"]: o for o in load_objects()}
    if name not in objs:
        sys.exit("gfx_manifest: no object %s in the manifest" % name)
    lo, hi = addr(objs[name]["rom"]), addr(objs[name]["end"])
    parts = [p for e in class_entries() for p in class_parts(e) if lo <= p.rom < hi]
    return sorted(parts, key=lambda p: p.rom)


def defs(name):
    out = ["/* %s */" % GENERATED]
    for p in object_parts(name):
        out += ["", "/* 0x%08X: %s::%s (%s), %s */" % (p.rom, p.cls, p.member, p.symbol, p.what), p.definition()[:-1]]
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
# table

def table():
    rows = ["| ROM | symbol | kind | source (graphics/) |", "|---|---|---|---|"]
    for e in load():
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
    sub.add_parser("objects")
    args = ap.parse_args()
    if args.mode in ("header", "defs", "symbols", "objects"):
        if args.mode == "header":
            write_if_changed(args.out, header())
        elif args.mode == "defs":
            Path(args.out).parent.mkdir(parents=True, exist_ok=True)
            Path(args.out).write_text(defs(args.object))
        elif args.mode == "symbols":
            write_symbols()
        else:
            print(" ".join(o["name"] for o in load_objects()))
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
