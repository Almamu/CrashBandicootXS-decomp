#!/usr/bin/env python3
"""The 41 rooms' level data: extract to editable sources, build back.

See docs/levels.md for the format. In short, a room is

- a `struct level_desc` (level_layers.cpp's view): up to five layers (BG1-3,
  the pooled BG0 and the collision layer), the level asset, the entity list
  and the entity links;
- per layer a `struct level_layer_desc` (the BG layers' source,
  include/game/level/bg_layer.hpp) and a grid of 16x8-cell chunk ids;
- the level asset: per layer a table of chunk offsets and one RLE/delta
  token stream per chunk (decoded by DecodeLayerChunk/DecodeCollisionChunk). 34 rooms
  keep it LZ77-packed next to the intro graphics, 7 keep it raw;
- a 256-colour BG palette, the entity list (`{type, x, y, param}` spawn
  records in 256-px columns), the spawn parameter records and the
  per-type entity counts.

The editable sources live in data/levels/: levels.json (the rooms, the
two ROM regions and which rooms they hold, in ROM order, the ROM order
of the level assets, the ROM layout of the level table, and the levels:
their gLevelTable fields and the rooms each plays, see "The level table"
below, and the level themes: their songs, pictures and colour cycles, see
"The level themes" below), tileset_order.json (the room order of 8 of the ROM's
BG tile sets, see "BG tile sets" below) and one directory per
room with room.json, one `<layer>.png` per BG layer, the layer's whole
picture (8bpp indexed with the room's palette, 8x8 pixels per cell; a
BG1-3 pixel is `bank * 16 + pixel`, see "BG layer pictures" below), and
`collision.png`, the collision layer as an image the size of the BG0
picture (one 8x8 block per cell, coloured by
data/levels/collision_legend.json, see tools/collision_bmp.py). The BG
tile sets, the cells, the chunk set, the chunk grid, the asset offsets,
the entity groups, the parameter offsets and the per-type counts are all
derived from those again.

    tools/levels.py extract [baserom.gba]     write data/levels/ from the ROM
    tools/levels.py rooms [--packed] [--new]  levels.json's rooms (with --packed,
                                              those with an LZ77-packed asset;
                                              with --new, those in no ROM region)
    tools/levels.py tileset SYMBOL --build DIR
                                              generate a BG tile set from its
                                              rooms' pictures: DIR/tilesets/
                                              SYMBOL.bin (.h, .png), and each of
                                              its layers' cells, DIR/<room>/
                                              <layer>.map.bin (little-endian u16,
                                              row-major); each written only if it
                                              changed
    tools/levels.py tilesets-mk OUT --build DIR
                                              levels.mk's rules for the tile sets
    tools/levels.py asset ROOMDIR OUT --build DIR
                                              build a room's level asset, and
                                              asset.layout.json next to it (the
                                              layers' offsets and chunk grids);
                                              the BG cells are
                                              DIR/<room>/<layer>.map.bin
    tools/levels.py c REGION OUT [--build DIR]
                                              C definitions of one room region
                                              (REGION = its name in levels.json);
                                              with --build, from the rooms'
                                              DIR/<room>/asset.layout.json. The
                                              last region also has the new tile
                                              sets and the new rooms (in no region)
    tools/levels.py assets OUT                C definitions of the level assets, in
                                              levels.json's "assets" order, then
                                              the new rooms'; each #includes the
                                              room's asset bytes (bin2c output,
                                              see levels.mk)
    tools/levels.py table levels|records OUT  the level table of levels.json and the
                                              rooms' room.json: the colour cycles'
                                              lists, gLevelTable, gLevelRoomLists
                                              and gThemeMusicCues (levels), or the
                                              room records and the played-room
                                              arrays (records), #included by
                                              src/game/level/data/level_table.cpp
    tools/levels.py table pictures OUT        gLevelSelectPictures, a level-select
                                              picture per theme, #included by
                                              src/game/ui/data/
                                              level_select_picture_table.cpp
    tools/levels.py palette-cycles OUT        theme_palette_cycles.h: RunRoom's
                                              colour-cycle cases per theme
    tools/levels.py worlds OUT                level_select_worlds.h: the level
                                              select's per-world tables,
                                              IsNextWorldOpen's cases and the
                                              page macros (partial pages),
                                              from levels.json's "worlds"
    tools/levels.py spawns SYMBOL OUT         the ROM's spawn list SYMBOL
                                              (gCategoryNSpawnTable), from its
                                              category's data/levels/spawns/ file,
                                              #included where the ROM has it
    tools/levels.py categories OUT            actor_categories.inc:
                                              ActorCategory::descriptors, from
                                              levels.json's "categories", and the
                                              spawn lists that aren't the ROM's
    tools/levels.py classes OUT               rooms.hpp: every room's class (its BG
                                              palette, chunk grids and level asset
                                              as static members), see "Room
                                              classes" below
    tools/levels.py symbols [--check]         write cxx_symbols.txt's room classes
                                              block (with --check, fail if it is
                                              out of date)
    tools/levels.py render ROOMDIR LAYER OUT [--build DIR]
                                              one layer's picture: the collision
                                              image, or a BG layer drawn again
                                              from the build's cells and tile set
    tools/levels.py constants entities OUT    constants/entities.h (ENTITY_*,
                                              ENEMY_KIND_*) from entity_types.json
    tools/levels.py constants crates OUT      constants/crates.h (CRATE_KIND_*)
                                              from crate_kinds.json
    tools/levels.py constants levels OUT      constants/levels.h (LEVEL_*, and the
                                              counts the code takes from the
                                              levels) from levels.json's "levels"
    tools/levels.py constants themes OUT      constants/themes.h (THEME_*, the
                                              pictures' count) from levels.json's
                                              "themes"
    tools/levels.py constants categories OUT  constants/categories.h (CATEGORY_*,
                                              CATEGORY_COUNT) from levels.json's
                                              "categories"
    tools/levels.py constants save OUT       constants/save.h: the save's EEPROM
                                              and record size, from the level
                                              count, and its version and checksum
                                              seed, from a CRC-32 of the level data

The entity types are named in data/levels/entity_types.json (one record
per gEntitySpawnFuncs slot) and room.json gives a placed entity's type by
that name; the crate kinds are named in data/levels/crate_kinds.json.
"""
import json
import re
import struct
import sys
import zlib
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import collision_bmp  # noqa: E402
import jasc_pal  # noqa: E402

ROM_BASE = 0x08000000
# gLevelRoomLists: per level (levels.json's "levels" order) a struct
# level_room_list {count, rooms, extra1, extra2}, which gives each room its
# folder (room_folders).
LEVEL_ROOM_LISTS = 0x0816CBF0
ROOM_TABLE = 0x0816CD80
ROOM_TABLE_SIZE = 0x474
REGIONS = [
    ("level_rooms_main_bonus", 0x0824B638, 0x08270F08),
    ("level_rooms_bosses_gem_paths", 0x082B91D0, 0x082BF120),
]
# The level assets, all 41 back to back (src/game/level/data/level_assets.cpp).
ASSET_REGION = ("level_assets", 0x0864F830, 0x087E3BEC)
# The seven raw (assetPacked = 0) assets and their sizes; the packed ones
# are LZ77 streams whose header gives the size.
RAW_ASSETS = {
    0x086C127C: 0x18A30, 0x086ECCD4: 0x1B484, 0x08708158: 0x2F568,
    0x087376C0: 0x1B684, 0x08752D44: 0x38064, 0x0878ADA8: 0x31394,
    0x087BC13C: 0x27AB0,
}
# level_desc slot order: layerData[0..2] (BG1-3), layer0Data (BG0),
# tileData (the collision layer, read by the terrain cache).
LAYERS = ["bg1", "bg2", "bg3", "bg0", "collision"]
# Chunk size in cells.
CHUNK_W, CHUNK_H = 16, 8
CHUNK_CELLS = CHUNK_W * CHUNK_H
# Number of entity types (the length of the per-type count table): the
# spawn-function table indexed by level_entity.type.
ENTITY_TYPES = 93
# Entities are grouped into 256-px columns (x >> 8).
GROUP_SHIFT = 8

LEVELS_DIR = Path("data/levels")
ENTITY_TYPES_JSON = LEVELS_DIR / "entity_types.json"
CRATE_KINDS_JSON = LEVELS_DIR / "crate_kinds.json"
LEVELS_JSON = LEVELS_DIR / "levels.json"


# ---------------------------------------------------------------------------
# Chunk streams (DecodeLayerChunk / DecodeCollisionChunk)

def decode_chunk(buf, o):
    """Decodes the token stream at buf[o:]. Returns (cells, end offset)."""
    out = []
    budget = 0x7F
    while budget >= 0:
        tok = struct.unpack_from("<H", buf, o)[0]
        n = tok & 0xFF
        o += 2
        budget -= n
        if tok & 0x8000:
            v = struct.unpack_from("<H", buf, o)[0]
            o += 2
            out += [v] * n
        elif tok & 0x4000:
            acc = struct.unpack_from("<H", buf, o)[0]
            o += 2
            out.append(acc)
            m = n - 1
            while True:
                p = struct.unpack_from("<H", buf, o)[0]
                o += 2
                for d in (p & 0xFF, p >> 8):
                    acc = (acc + d - (d >> 7 << 8)) & 0xFFFF
                    out.append(acc)
                m -= 2
                if m <= 1:
                    break
            if m:
                d = buf[o]
                o += 2
                acc = (acc + d - (d >> 7 << 8)) & 0xFFFF
                out.append(acc)
        else:
            out += struct.unpack_from("<%dH" % n, buf, o)
            o += 2 * n
    if len(out) != CHUNK_CELLS:
        raise ValueError("chunk stream at %#x decodes to %d cells" % (o, len(out)))
    return out, o


def _delta_ok(a, b):
    d = (b - a) & 0xFFFF
    if d >= 0x8000:
        d -= 0x10000
    return -127 <= d <= 127  # the encoder never uses -128


def _fill_run(c, i):
    j = i
    while j < len(c) and c[j] == c[i]:
        j += 1
    return j - i


def _delta_run(c, i):
    j = i + 1
    while j < len(c) and _delta_ok(c[j - 1], c[j]):
        j += 1
    return j - i


def chunk_tokens(cells):
    """The original encoder's token choice (reproduces all 16,323 chunks):
    a run of 4+ equal cells is a fill; any other stretch up to the next
    such run is one copy, unless a delta run at its start saves space (the
    encoder counts a delta run as 2 + (n - 1) / 2 halfwords, rounding
    down, and a stretch at the end of the chunk always as if a copy
    followed)."""
    n = len(cells)
    toks = []
    i = 0
    while i < n:
        r = _fill_run(cells, i)
        if r >= 4:
            toks.append(("fill", r, 0))
            i += r
            continue
        e = i
        while e < n and _fill_run(cells, e) < 4:
            e += 1
        c = e - i
        d = min(_delta_run(cells, i), c)
        delta = 0
        if d >= 3:
            cost = 2 + (d - 1) // 2
            if c > d or e == n:
                cost += 1 + c - d
            if cost < 1 + c:
                delta = d
        toks.append(("copy", c, delta))
        i = e
    return toks


def encode_chunk(cells):
    """Encodes 128 cells as the original tool did, including its leftover
    bytes: it wrote each stretch as a copy followed by the next fill, then
    rewrote a stretch that is better as a delta run (+ copy of the rest) in
    place and moved the fill down. That leaves the copy's cell bytes under
    a delta run's odd last halfword (whose high byte the decoder ignores)
    and stale bytes past the end, which the word alignment keeps.
    Returns (stream, pad)."""
    toks = chunk_tokens(cells)
    buf = bytearray(4 * CHUNK_CELLS)

    def put(o, v):
        struct.pack_into("<H", buf, o, v & 0xFFFF)

    p = i = 0
    for t, (kind, n, d) in enumerate(toks):
        if kind == "fill":
            put(p, 0x8000 | n)
            put(p + 2, cells[i])
            p += 4
            i += n
            continue
        put(p, n)
        for k in range(n):
            put(p + 2 + 2 * k, cells[i + k])
        after = p + 2 + 2 * n
        if t + 1 < len(toks):
            put(after, 0x8000 | toks[t + 1][1])
            put(after + 2, cells[i + n])
        if not d:
            p = after
            i += n
            continue
        put(p, 0x4000 | d)
        put(p + 2, cells[i])
        q = p + 4
        m, k = d - 1, i + 1
        while True:
            put(q, ((cells[k] - cells[k - 1]) & 0xFF) | ((cells[k + 1] - cells[k]) & 0xFF) << 8)
            q += 2
            k += 2
            m -= 2
            if m <= 1:
                break
        if m:
            buf[q] = (cells[k] - cells[k - 1]) & 0xFF
            q += 2
        if n > d:
            put(q, n - d)
            for r in range(n - d):
                put(q + 2 + 2 * r, cells[i + d + r])
            q += 2 + 2 * (n - d)
        p = q
        i += n
    return bytes(buf[:p]), bytes(buf[p:p + (-p) % 4])


def split_chunks(cells, w, h):
    """A layer's tilemap (w x h chunks) -> (unique chunks in order of first
    appearance, row-major chunk id grid)."""
    width = w * CHUNK_W
    chunks, index, grid = [], {}, []
    for gy in range(h):
        for gx in range(w):
            ch = tuple(cells[(gy * CHUNK_H + y) * width + gx * CHUNK_W + x]
                       for y in range(CHUNK_H) for x in range(CHUNK_W))
            if ch not in index:
                index[ch] = len(chunks)
                chunks.append(ch)
            grid.append(index[ch])
    return chunks, grid


def chunk_crc(ch):
    return "%08x" % zlib.crc32(struct.pack("<128H", *ch))


def encode_section(chunks, pad_overrides=None):
    """One layer's part of the asset: u16 chunk offsets (in words from the
    section start; a zero entry pads an odd count), then the word-aligned
    chunk streams."""
    pad_overrides = pad_overrides or {}
    n = len(chunks)
    table_len = 2 * (n + (n & 1))
    body = bytearray()
    offsets = []
    for k, ch in enumerate(chunks):
        offsets.append((table_len + len(body)) // 4)
        stream, pad = encode_chunk(list(ch))
        ov = pad_overrides.get(str(k))
        if ov is not None and ov["crc"] == chunk_crc(ch):
            pad = bytes.fromhex(ov["pad"])
        body += stream + pad
    if max(offsets, default=0) > 0xFFFF:
        raise ValueError("layer too large for the u16 chunk offset table")
    table = struct.pack("<%dH" % n, *offsets) + b"\0" * (table_len - 2 * n)
    return table + bytes(body)


# ---------------------------------------------------------------------------
# Room sources

def read_json(path):
    return json.loads(Path(path).read_text())


def room_key(room_dir):
    """A room's folder under data/levels/, e.g. "jungle_jam/main": its name
    in levels.json and under the build directory."""
    return Path(room_dir).relative_to(LEVELS_DIR).as_posix()


def load_room(room_dir):
    """A room's room.json, with its "name" (its folder, room_key) added."""
    room = read_json(Path(room_dir) / "room.json")
    room["name"] = room_key(room_dir)
    return room


def room_palette(room_dir, room):
    """The room's 256-colour BG palette: the palette of its layer pictures
    (bg0-bg3.png), read as tools/jasc_pal.py reads a PNG's palette (bit 15
    in the red value's lowest bit). Every layer picture must carry all 256
    entries, the same in each."""
    pal, first = None, None
    for lname, layer in room_layers(room):
        if lname == "collision":
            continue
        path = Path(room_dir) / layer["map"]
        p = jasc_pal.read_png(path)
        if len(p) != 256:
            sys.exit("%s: a palette of %d colours; a room's layer pictures keep all 256 of the "
                     "room's palette, unused ones too (docs/levels.md, \"BG layers\")" % (path, len(p)))
        if pal is None:
            pal, first = p, path
        elif p != pal:
            i = next(k for k in range(256) if p[k] != pal[k])
            sys.exit("%s: its palette differs from %s's (entry %d is %04X there, %04X here): every "
                     "layer picture of a room carries the room's palette" % (path, first, i, pal[i], p[i]))
    if pal is None:
        sys.exit("%s: no BG layer picture to take the palette from" % room_dir)
    return pal


def entity_type_names():
    """type -> name (None for the unnamed types), from entity_types.json."""
    types = read_json(ENTITY_TYPES_JSON)["types"]
    assert [t["type"] for t in types] == list(range(len(types))), ENTITY_TYPES_JSON
    return [t["name"] for t in types]


def entity_type(room_name, t, by_name):
    """A room.json entity type (a name from entity_types.json, or a number
    for an unnamed type) -> its number."""
    if isinstance(t, str):
        if t not in by_name:
            sys.exit("%s: unknown entity type %s (see %s)" % (room_name, t, ENTITY_TYPES_JSON))
        t = by_name[t]
    if not 0 <= t < ENTITY_TYPES:
        sys.exit("%s: entity type %d out of range" % (room_name, t))
    return t


def load_map(room_dir, lname, layer, build_dir=None):
    """A layer's cells, row-major. A BG layer's are the ones the tile set
    generator gave its picture (`levels.py tileset`),
    `<build_dir>/<room>/<layer>.map.bin`; the collision layer's come from
    its image."""
    path = Path(room_dir) / layer["map"]
    w, h = layer["size_chunks"]
    if lname != "collision":
        if build_dir is None:
            sys.exit("%s: a BG layer's cells come from its tile set (levels.py tileset): give --build" % path)
        path = Path(build_dir) / room_key(room_dir) / ("%s.map.bin" % lname)
    elif path.suffix in (".png", ".bmp"):
        # the collision layer: an image of its cells, 8x8 pixels each, the
        # size of the BG0 picture (tools/collision_bmp.py)
        px = collision_bmp.CELL_PX
        iw, ih, cells = collision_bmp.read_image(path)
        if (iw, ih) != (w * CHUNK_W, h * CHUNK_H):
            sys.exit("%s: %dx%d pixels, expected %dx%d (%dx%d cells of %dx%d pixels, %d x %d chunks)"
                     % (path, iw * px, ih * px, w * CHUNK_W * px, h * CHUNK_H * px,
                        w * CHUNK_W, h * CHUNK_H, px, px, w, h))
        return cells
    data = path.read_bytes()
    if len(data) != w * h * CHUNK_CELLS * 2:
        sys.exit("%s: %d bytes, expected %d x %d chunks" % (path, len(data), w, h))
    return list(struct.unpack("<%dH" % (len(data) // 2), data))


def room_layers(room):
    return [(name, room["layers"][name]) for name in LAYERS if name in room["layers"]]


def build_asset(room_dir, build_dir=None):
    """The room's level asset (uncompressed) and each layer's offset in it.
    Sections are in slot order: BG1, BG2, BG3, BG0, collision."""
    room = load_room(room_dir)
    asset = bytearray()
    offsets, grids = {}, {}
    for name, layer in room_layers(room):
        w, h = layer["size_chunks"]
        chunks, grid = split_chunks(load_map(room_dir, name, layer, build_dir), w, h)
        offsets[name] = len(asset)
        grids[name] = grid
        asset += encode_section(chunks, layer.get("pad_overrides"))
    return bytes(asset), offsets, grids


# ---------------------------------------------------------------------------
# Room classes
#
# Each room is a class (docs/levels.md, "Room classes"), like a graphic of
# graphics/manifest.json (docs/graphics.md, "Graphics classes"): its BG
# palette, its layers' chunk grids and its level asset are static const
# data members (Room37::palette, ::bg1Grid, ::asset), which keep their
# room.json symbols as their C names through cxx_symbols.txt. The class is
# named after the room's level_desc symbol, as its params struct is
# (gRoom37Desc: Room37). The tile sets stay free: the rooms of a theme
# share them.

ROOM_CLASSES_BEGIN = "# room classes: generated by tools/levels.py symbols, do not edit"
ROOM_CLASSES_END = "# end of room classes"
CXX_SYMBOLS = Path("cxx_symbols.txt")
# The room classes' namespace (#1033, docs/cplusplus.md "Namespaces").
NAMESPACE = ("Game", "Level")
LAYER_MEMBERS = {"bg0": "bg0Grid", "bg1": "bg1Grid", "bg2": "bg2Grid", "bg3": "bg3Grid", "collision": "collisionGrid"}


def room_class(room):
    """The room's class: its level_desc symbol without the g and Desc."""
    return room["symbols"]["desc"][1:].removesuffix("Desc")


def room_members(room):
    """The room's members, in declaration order: (member, C name, type,
    bound, what)."""
    out = [("palette", room["symbols"]["palette"], "u16", "256", "the BG palette")]
    for lname, layer in room_layers(room):
        out.append((LAYER_MEMBERS[lname], layer["grid_symbol"], "u16", "", "%s's chunk grid" % lname))
    a = room["asset"]
    out.append(("asset", a["symbol"], "u8", "", "the layers' chunk streams, %s" % ("LZ77" if a["packed"] else "raw")))
    return out


def member(room, name):
    """A member of the room's class, as the C++ names it: qualified by
    NAMESPACE (`Game::Level::Room37::palette`)."""
    return "%s::%s::%s" % ("::".join(NAMESPACE), room_class(room), name)


def mangled(room, name):
    """`_Q34Game5Level6Room37.palette`: g++ 2.9's name of the static data
    member, its class qualified by NAMESPACE."""
    names = NAMESPACE + (room_class(room),)
    return "_Q%d%s.%s" % (len(names), "".join("%d%s" % (len(n), n) for n in names), name)


def all_rooms():
    return [read_json(LEVELS_DIR / r / "room.json") | {"name": r} for r in read_json(LEVELS_JSON)["rooms"]]


def rooms_hpp():
    """rooms.hpp: every room's class, in levels.json's order."""
    out = ["/* Generated by tools/levels.py from data/levels/ - do not edit. */", "",
           "/* The room classes (docs/levels.md, \"Room classes\"): each room's BG",
           " * palette, chunk grids and level asset as static const data members,",
           " * defined by the level data (level_rooms_*.cpp, level_assets.cpp)",
           " * and keeping their room.json symbols through cxx_symbols.txt. They are",
           " * in the namespace %s (#1033, docs/cplusplus.md \"Namespaces\"). */" % "::".join(NAMESPACE),
           "#ifndef GUARD_ROOMS_HPP", "#define GUARD_ROOMS_HPP", "",
           "extern \"C\" {", "#include \"gba/types.h\"", "}", ""]
    for n in NAMESPACE:
        out += ["namespace " + n, "{"]
    for room in all_rooms():
        decls = ["    static const %s %s[%s];" % (t, m, b) for m, _, t, b, _ in room_members(room)]
        width = max(len(d) for d in decls)
        out += ["", "/* data/levels/%s (%s) */" % (room["name"], room["symbols"]["desc"]),
                "class %s" % room_class(room), "{", "public:"]
        out += ["%s // %s - %s" % (d.ljust(width), sym, what) for d, (_, sym, _, _, what) in zip(decls, room_members(room))]
        out.append("};")
    out += [""] + ["} // namespace " + n for n in reversed(NAMESPACE)]
    out += ["", "#endif // GUARD_ROOMS_HPP", ""]
    return "\n".join(out)


def room_symbols_block():
    lines = [ROOM_CLASSES_BEGIN,
             "# The room classes (data/levels/, docs/levels.md \"Room classes\"): each",
             "# member keeps its room.json symbol."]
    for room in all_rooms():
        lines += ["%s %s" % (mangled(room, m), sym) for m, sym, _, _, _ in room_members(room)]
    return "\n".join(lines + [ROOM_CLASSES_END])


def split_room_symbols():
    """cxx_symbols.txt: (before, the room classes block, after)."""
    text = CXX_SYMBOLS.read_text()
    if ROOM_CLASSES_BEGIN not in text or ROOM_CLASSES_END not in text:
        sys.exit("%s has no room classes markers" % CXX_SYMBOLS)
    head, rest = text.split(ROOM_CLASSES_BEGIN, 1)
    old, tail = rest.split(ROOM_CLASSES_END, 1)
    return head, ROOM_CLASSES_BEGIN + old + ROOM_CLASSES_END, tail


def room_symbols_error():
    """Why cxx_symbols.txt's room classes block is out of date, or None."""
    if split_room_symbols()[1] != room_symbols_block():
        return "%s: the room classes block is out of date (tools/levels.py symbols)" % CXX_SYMBOLS
    return None


def room_symbols(check):
    """Writes cxx_symbols.txt's room classes block, or with `check` fails
    if it is out of date."""
    if check:
        err = room_symbols_error()
        if err:
            sys.exit(err)
        return
    head, _, tail = split_room_symbols()
    CXX_SYMBOLS.write_text(head + room_symbols_block() + tail)


# ---------------------------------------------------------------------------
# C generation

def c_array(values, fmt, per_line):
    lines = []
    for i in range(0, len(values), per_line):
        lines.append("    " + ", ".join(fmt % v for v in values[i:i + per_line]) + ",")
    return "\n".join(lines)


def entity_groups(room):
    """Entities (in id order) -> group records. Ids run from the last
    column to the first (SpawnRoomEntities's walk), which is also the order the
    entities are stored in; a group's `first` counts from the first
    column."""
    ents = room["entities"]
    width_px = room["layers"]["collision"]["size_tiles"][0] * 8
    ngroups = (width_px >> GROUP_SHIFT) + 1
    members = [[] for _ in range(ngroups)]
    prev = ngroups
    for i, e in enumerate(ents):
        g = e["x"] >> GROUP_SHIFT
        if g >= ngroups:
            sys.exit("%s: entity %d at x=%d is outside the room" % (room["name"], i, e["x"]))
        if g > prev:
            sys.exit("%s: entity %d (column %d) is out of id order: ids go from the last column "
                     "to the first" % (room["name"], i, g))
        prev = g
        members[g].append(i)
    groups, first = [], 0
    stored = 0
    starts = [0] * ngroups
    for g in range(ngroups - 1, -1, -1):
        starts[g] = stored
        stored += len(members[g])
    for g in range(ngroups):
        groups.append((first, len(members[g]), starts[g]))
        first += len(members[g])
    return groups


def room_c(room_dir, build_dir=None):
    """The room's C definitions. The layers' asset offsets and chunk grids
    come from the asset: built here, or with `build_dir` read from the
    layout `asset` wrote next to it (`<build_dir>/<room>/asset.layout.json`)."""
    room = load_room(room_dir)
    name = room["name"]
    sym = room["symbols"]
    if build_dir is None:
        _, offsets, grids = build_asset(room_dir)
    else:
        layout = read_json(Path(build_dir) / room_key(room_dir) / "asset.layout.json")
        offsets, grids = layout["offsets"], layout["grids"]
    out = []
    externs = set()

    def ext(s):
        if s:
            externs.add(s)
        return s or "0"

    by_name = {n: t for t, n in enumerate(entity_type_names()) if n}
    types = [entity_type(name, e["type"], by_name) for e in room["entities"]]
    out.append("/* data/levels/%s: level_desc %s, %d entities */" % (name, sym["desc"], len(room["entities"])))
    # forward declarations for the pointers to objects defined further down
    out.append("extern const struct level_entity_list %s;" % sym["entities"])
    out.append("extern const struct level_layer_desc %s[];" % sym["layers"])
    out.append("extern const struct level_entity_group %s[];" % sym["groups"])
    out.append("extern const struct level_entity %s[];\n" % sym["items"])

    # Spawn parameter records.
    recs = room["params"]
    stype = "level_params_%s" % sym["desc"][1:].removesuffix("Desc").lower()
    out.append("struct %s\n{" % stype)
    for i, r in enumerate(recs):
        out.append("    u32 r%d[%d];" % (i, len(r)))
    out.append("};\n")
    out.append("extern const struct %s %s =\n{" % (stype, sym["params"]))
    for r in recs:
        out.append("    { " + ", ".join("%#x" % v for v in r) + " },")
    out.append("};\n")
    out.append("extern const u16 %s[%d] =\n{" % (sym["param_offsets"], len(recs)))
    for i in range(len(recs)):
        out.append("    LEVEL_PARAM_OFFSET(struct %s, r%d)," % (stype, i))
    out.append("};\n")

    # Chunk grids, one chunk row per line.
    for lname, layer in room_layers(room):
        g = grids[lname]
        w, h = layer["size_chunks"]
        out.append("/* %s chunk grid (%s), %d x %d */" % (lname, layer["grid_symbol"], w, h))
        out.append("const u16 %s[%d] =\n{" % (member(room, LAYER_MEMBERS[lname]), len(g)))
        out.append(c_array(g, "%d", w))
        out.append("};\n")

    # BG palette, from the layer pictures.
    pal = room_palette(room_dir, room)
    out.append("/* %s */" % sym["palette"])
    out.append("const u16 %s[256] =\n{" % member(room, "palette"))
    out.append(c_array(pal, "0x%04X", 8))
    out.append("};\n")

    # Per-type entity counts.
    counts = [0] * ENTITY_TYPES
    for t in types:
        counts[t] += 1
    out.append("extern const u16 %s[%d] =\n{" % (sym["type_counts"], ENTITY_TYPES))
    out.append(c_array(counts, "%d", 16))
    out.append("};\n")

    # Entity links.
    links = room.get("links")
    if links is not None:
        out.append("extern const LEVEL_LINKS(%d) %s =\n{\n    %d,\n    {" % (len(links), sym["links"], len(links)))
        for i in range(0, len(links), 6):
            out.append("        " + " ".join("{ %d, %d }," % tuple(l) for l in links[i:i + 6]))
        out.append("    },\n};\n")

    # The level descriptor.
    layer_idx = {n: i for i, (n, _) in enumerate(room_layers(room))}

    def lref(n):
        return "&%s[%d]" % (sym["layers"], layer_idx[n]) if n in layer_idx else "0"

    a = room["asset"]
    out.append("extern const struct level_desc %s =\n{" % sym["desc"])
    out.append("    { %s, %s, %s }," % (lref("bg1"), lref("bg2"), lref("bg3")))
    out.append("    %s," % lref("bg0"))
    out.append("    %s," % lref("collision"))
    out.append("    %s," % member(room, "asset"))
    out.append("    %d," % (1 if a["packed"] else 0))
    out.append("    &%s," % sym["entities"])
    out.append("    %s," % ("(const struct level_link_list *)&%s" % sym["links"] if links is not None else "0"))
    out.append("};\n")

    # Entity list header.
    groups = entity_groups(room)
    out.append("extern const struct level_entity_list %s =\n{" % sym["entities"])
    out.append("    %d, %d, %s, %s, (const u8 *)&%s, %s,"
               % (len(room["entities"]), len(groups), sym["groups"], sym["param_offsets"],
                  sym["params"], sym["type_counts"]))
    out.append("};\n")

    # Layer descriptors.
    out.append("extern const struct level_layer_desc %s[%d] =\n{" % (sym["layers"], len(layer_idx)))
    for lname, layer in room_layers(room):
        w, h = layer["size_chunks"]
        out.append("    { /* %s */" % lname)
        out.append("        %s, %#x, %s," % (member(room, LAYER_MEMBERS[lname]), offsets[lname], ext(layer["tileset"])))
        out.append("        %#x, %#x, %d, %d, %d, %d, %d, %d,"
                   % (layer["scale"][0] & 0xFFFFFFFF, layer["scale"][1] & 0xFFFFFFFF, layer["cnt"],
                      w, h, layer["size_tiles"][0], layer["size_tiles"][1], layer["unk_1E"]))
        out.append("    },")
    out.append("};\n")

    # Entity groups and the entities.
    out.append("extern const struct level_entity_group %s[%d] =\n{" % (sym["groups"], len(groups)))
    for first, count, stored in groups:
        out.append("    { %d, %d, &%s[%d] }," % (first, count, sym["items"], stored))
    out.append("};\n")
    ents = room["entities"]
    out.append("/* In id order: type, x, y, param */")
    out.append("extern const struct level_entity %s[%d] =\n{" % (sym["items"], len(ents)))
    for i, e in enumerate(ents):
        out.append("    { %d, %d, %d, %d }, /* %d */" % (types[i], e["x"], e["y"], e["param"], i))
    out.append("};\n")
    return externs, "\n".join(out) + "\n"


def region_c(region_name, out_path, build_dir=None):
    """A room region's C: its rooms, in levels.json's ROM order. The last
    region (level_rooms_bosses_gem_paths) goes on with the level data that isn't the
    ROM's (docs/levels.md, "Adding a room"): the new tile sets (used by
    no room of a region), then the new rooms (in no region), in
    levels.json order. With the original data there are none."""
    levels = read_json(LEVELS_DIR / "levels.json")
    region = next((r for r in levels["regions"] if r["name"] == region_name), None)
    if region is None:
        sys.exit("levels.json has no region %s" % region_name)
    rooms = list(region["rooms"])
    last = region is levels["regions"][-1]
    if last:
        rooms += new_rooms(levels)
    externs, bodies = set(), []
    for rname in rooms:
        e, body = room_c(LEVELS_DIR / rname, build_dir)
        externs |= e
        bodies.append(body)
    if last:
        # after the region's own rooms
        bodies[len(region["rooms"]):len(region["rooms"])] = new_tilesets_c(levels, build_dir)
        externs -= {sym for sym, _ in new_tilesets(levels)}
    text = ["/* Generated by tools/levels.py from data/levels/ - do not edit. */", ""]
    for s in sorted(externs):
        text.append("extern const u8 %s[];" % s)
    text.append("")
    text += bodies
    Path(out_path).write_text("\n".join(text))


def assets_c(out_path):
    """The definitions of the level assets, in ROM order, then the new
    rooms' (in no region) in levels.json order: one `u8` array per room,
    named by room.json's asset symbol. The bytes are levels.mk's bin2c
    output of the built asset (`asset.bin.lz.inc` for a packed one,
    `asset.bin.inc` for a raw one). Every asset starts on a word boundary
    (`aligned(4)`); the 0-3 zero bytes after a packed one are that
    alignment."""
    levels = read_json(LEVELS_JSON)
    rooms = levels["assets"]["rooms"]
    if sorted(rooms) != sorted(rom_region_rooms(levels)):
        sys.exit("levels.json: the \"assets\" order must list every room of the ROM's regions once "
                 "(a new room's asset comes after them)")
    rooms = rooms + new_rooms(levels)
    text = ["/* Generated by tools/levels.py from data/levels/ - do not edit. */", ""]
    for rname in rooms:
        room = read_json(LEVELS_DIR / rname / "room.json")
        asset = room["asset"]
        packed = asset["packed"]
        text.append("/* %s (%s): %s */" % (rname, asset["symbol"], "LZ77-packed" if packed else "raw"))
        text.append("const u8 %s[] __attribute__((aligned(4))) = {" % member(room, "asset"))
        text.append('#include "%s/asset.bin%s.inc"' % (rname, ".lz" if packed else ""))
        text.append("};")
        text.append("")
    Path(out_path).write_text("\n".join(text))


# ---------------------------------------------------------------------------
# BG layer pictures (data/levels/<level>/<slot>/bg0-3.png) and BG tile sets
#
# A BG layer is kept as its whole picture: 8x8 pixels per cell, 8bpp
# indexed with the room's palette. A cell's block is its tile, flipped as
# the cell says. BG0's tiles are 8bpp, so a pixel is the tile's colour
# index. A BG1-3 pixel is `bank * 16 + p`, p the 4bpp tile's pixel, for all
# 64 pixels, the transparent ones (p = 0) too: that keeps the cell's
# palette bank even in a blank cell, so every cell comes back exactly.
#
# The BG tile sets aren't sources: each one is generated from the pictures
# of the layers that name it in room.json (docs/levels.md, "Tile sets"):
# - tile 0 is the blank tile;
# - then the rooms that use the set, one after another: first the ones
#   data/levels/tileset_order.json lists for it, then the other rooms of the
#   ROM's regions in levels.json order, then the new rooms (in no region);
# - inside a room bg1, bg2, bg3 (a BG0 set: bg0), each picture in chunk
#   order (the 16x8-cell chunks row by row, a chunk's cells row by row);
# - a block that isn't a tile yet under any of the four flips becomes the
#   next tile, in the orientation it is first seen in. A cell is its
#   block's tile and flip (and, BG1-3, its bank).
# That gives back the ROM's 11 sets and every cell of its 131 BG layers.

# The flips of a cell, in its flip bits' order: (h, v).
FLIPS = ((0, 0), (1, 0), (0, 1), (1, 1))
# One 8-pixel row of a block as a little-endian u64 (pixel 0 in the low
# byte): a block is a tuple of 8 of these, top row first.
_NIBBLES = 0x0F0F0F0F0F0F0F0F
_HIGH_NIBBLES = 0xF0F0F0F0F0F0F0F0
_BYTES = 0x0101010101010101

TILESET_ORDER_JSON = LEVELS_DIR / "tileset_order.json"
# The most tiles a cell can name: BG0's 14-bit tile-slot pool ids, BG1-3's
# 10-bit screen entries.
MAX_TILES = {8: 0x4000, 4: 0x400}


def tile_blocks(data, bpp):
    """Tile bytes (8bpp, or 4bpp low nibble first) -> one block per tile."""
    size = 8 * bpp
    out = []
    for t in range(len(data) // size):
        tile = data[t * size:(t + 1) * size]
        if bpp == 4:
            tile = bytes(tile[i // 2] >> 4 * (i & 1) & 0xF for i in range(64))
        out.append(tuple(int.from_bytes(tile[y * 8:y * 8 + 8], "little") for y in range(8)))
    return out


def flips(b):
    """A block under the four flips of FLIPS."""
    m = tuple(int.from_bytes(r.to_bytes(8, "little"), "big") for r in b)  # h flip
    return b, m, b[::-1], m[::-1]


class LayerTiles:
    """A BG layer's tile set, cell -> its 8x8 block. bg0: the cells are
    tile-slot pool ids (tile in bits 0-13, h/v flip in 14-15) of an 8bpp
    set; else BG screen entries (tile 0-9, h/v flip 10-11, palette bank
    12-15) of a 4bpp set."""

    def __init__(self, symbol, bg0, blocks):
        self.symbol = symbol
        self.bg0 = bg0
        self.flipped = [flips(b) for b in blocks]
        self.blocks = {}

    def block(self, v):
        """The 8x8 block a cell draws (None for a tile past the end of the
        set)."""
        b = self.blocks.get(v)
        if b is None:
            if self.bg0:
                t, flip, bank = v & 0x3FFF, v >> 14 & 3, 0
            else:
                t, flip, bank = v & 0x3FF, v >> 10 & 3, v >> 12
            if t >= len(self.flipped):
                return None
            b = self.flipped[t][FLIPS.index((flip & 1, flip >> 1))]
            if bank:
                b = tuple(r | bank * 16 * _BYTES for r in b)
            self.blocks[v] = b
        return b


def write_layer_png(path, layer, tiles, cells, palette):
    """A BG layer's cells as its picture (see above), drawn with `tiles` (a
    LayerTiles) and the room's palette (256 entries, written as
    tools/jasc_pal.py writes a PNG's palette)."""
    from PIL import Image

    w, h = layer["size_chunks"]
    W = w * CHUNK_W
    if len(cells) != W * h * CHUNK_H:
        sys.exit("%s: %d cells, expected %d x %d chunks" % (path, len(cells), w, h))
    pack = struct.Struct("<%dQ" % W).pack
    out = []
    for y in range(h * CHUNK_H):
        blocks = []
        for x, v in enumerate(cells[y * W:(y + 1) * W]):
            b = tiles.block(v)
            if b is None:
                sys.exit("%s: cell (%d, %d) is %#06x, a tile past the end of %s"
                         % (path, x, y, v, tiles.symbol))
            blocks.append(b)
        out += [pack(*row) for row in zip(*blocks)]
    img = Image.frombytes("P", (W * 8, h * CHUNK_H * 8), b"".join(out))
    img.putpalette([c for v in palette for c in jasc_pal.to_rgb(v)])
    img.save(path, optimize=True)


def _pixels(x, y):
    return "(pixels %d-%d, %d-%d)" % (x * 8, x * 8 + 7, y * 8, y * 8 + 7)


def rom_region_rooms(levels):
    """The rooms of the ROM's regions: their data is the ROM's."""
    return [r for region in levels["regions"] for r in region["rooms"]]


def new_rooms(levels):
    """The rooms in no ROM region (docs/levels.md, "Adding a room"), in
    levels.json order: the last region's C has them after its own."""
    rom = set(rom_region_rooms(levels))
    return [r for r in levels["rooms"] if r not in rom]


def tileset_users(levels):
    """Every BG tile set the rooms' layers name, which makes the set:
    {symbol: {"bpp": 8 for a BG0 set, 4 for a BG1-3 one, "rooms": {room:
    [its layers naming the set]}}}, rooms in levels.json order."""
    sets = {}
    for r in levels["rooms"]:
        room = read_json(LEVELS_DIR / r / "room.json")
        for lname in ("bg0", "bg1", "bg2", "bg3"):
            layer = room["layers"].get(lname)
            if layer is None:
                continue
            bpp = 8 if lname == "bg0" else 4
            s = sets.setdefault(layer["tileset"], {"bpp": bpp, "rooms": {}})
            if s["bpp"] != bpp:
                sys.exit("data/levels/%s: %s names %s, which other rooms use for %s: a BG0 tile set "
                         "(8bpp) can't be a BG1-3 one (4bpp)" % (r, lname, layer["tileset"],
                                                                  "BG0" if s["bpp"] == 8 else "BG1-3"))
            s["rooms"].setdefault(r, []).append(lname)
    return sets


def load_tileset_order():
    return read_json(TILESET_ORDER_JSON) if TILESET_ORDER_JSON.exists() else {}


def is_rom_tileset(spec, levels):
    """A tile set the ROM's rooms use (the ROM's own, defined by the
    hand-written src/.../data files); the others are new ones."""
    rom = set(rom_region_rooms(levels))
    return any(r in rom for r in spec["rooms"])


def tileset_rooms(symbol, spec, levels, order):
    """The rooms of a tile set in the order its tiles are made: [(room,
    [layers])]. See above."""
    listed = order.get(symbol, [])
    for r in listed:
        if r not in spec["rooms"]:
            sys.exit("%s: %s lists %s, which doesn't use it" % (TILESET_ORDER_JSON, symbol, r))
    rom = set(rom_region_rooms(levels))
    users = list(spec["rooms"])
    seq = listed + [r for r in users if r in rom and r not in listed] + \
        [r for r in users if r not in rom and r not in listed]
    return [(r, spec["rooms"][r]) for r in seq]


def layer_blocks(path, layer):
    """A BG layer picture's 8x8 blocks in chunk order: (cell index, block)."""
    from PIL import Image

    w, h = layer["size_chunks"]
    W, H = w * CHUNK_W, h * CHUNK_H
    img = Image.open(path)
    if img.mode != "P":
        sys.exit("%s: an image of mode %s; a BG layer is an 8bpp indexed PNG (its pixels are palette "
                 "indices, see docs/levels.md)" % (path, img.mode))
    if img.size != (W * 8, H * 8):
        sys.exit("%s: %dx%d pixels, expected %dx%d (%d x %d chunks of 128x64 pixels)"
                 % ((path,) + img.size + (W * 8, H * 8, w, h)))
    raw = img.tobytes()
    unpack = struct.Struct("<%dQ" % W).unpack_from
    rows = [unpack(raw, y * W * 8) for y in range(H * 8)]
    for gy in range(h):
        for gx in range(w):
            for cy in range(CHUNK_H):
                y = gy * CHUNK_H + cy
                band = rows[y * 8:y * 8 + 8]
                for cx in range(CHUNK_W):
                    x = gx * CHUNK_W + cx
                    yield y * W + x, tuple(r[x] for r in band)


def generate_tileset(symbol, levels, users=None, order=None):
    """A BG tile set from its rooms' pictures (see above): (its tiles'
    blocks, each tile's palette bank where first drawn, {(room, layer):
    cells}). A new room's new tiles go at the end of the set, after the
    ROM's rooms' (the ROM data after the set moves, docs/shifting.md)."""
    users = users if users is not None else tileset_users(levels)
    order = order if order is not None else load_tileset_order()
    if symbol not in users:
        sys.exit("no room's layer names the tile set %s" % symbol)
    spec = users[symbol]
    bg0 = spec["bpp"] == 8
    shift = 14 if bg0 else 10
    tiles, banks, index = [], [], {}

    def add(b, bank):
        t = len(tiles)
        tiles.append(b)
        banks.append(bank)
        for f, blk in enumerate(flips(b)):
            index.setdefault(blk, t | (f & 1) << shift | (f >> 1) << shift + 1)

    add((0,) * 8, 0)
    maps, errors = {}, []
    for room, lays in tileset_rooms(symbol, spec, levels, order):
        layers = read_json(LEVELS_DIR / room / "room.json")["layers"]
        for lname in lays:
            layer = layers[lname]
            path = LEVELS_DIR / room / layer["map"]
            W = layer["size_chunks"][0] * CHUNK_W
            cells = [0] * (W * layer["size_chunks"][1] * CHUNK_H)
            for i, b in layer_blocks(path, layer):
                x, y = i % W, i // W
                bank = 0
                if not bg0:
                    bank = b[0] >> 4 & 0xF
                    if any(r & _HIGH_NIBBLES != bank * 16 * _BYTES for r in b):
                        bs = sorted({p >> 4 for r in b for p in r.to_bytes(8, "little")})
                        errors.append("%s: cell (%d, %d) %s mixes palette banks %s: a BG1-3 cell draws all 64 "
                                      "pixels from one bank (indices bank * 16 to bank * 16 + 15)"
                                      % (path, x, y, _pixels(x, y), ", ".join(map(str, bs))))
                        continue
                    b = tuple(r & _NIBBLES for r in b)
                v = index.get(b)
                if v is None:
                    v = len(tiles)
                    add(b, bank)
                cells[i] = v | bank << 12
            maps[(room, lname)] = cells
    if errors:
        more = len(errors) - 20
        sys.exit("\n".join(errors[:20] + (["... and %d more cells" % more] if more > 0 else [])))
    if len(tiles) > MAX_TILES[spec["bpp"]]:
        sys.exit("%s: %d tiles, more than a cell can name (%d)" % (symbol, len(tiles), MAX_TILES[spec["bpp"]]))
    return tiles, banks, maps


def tileset_bytes(tiles, bpp):
    out = bytearray()
    for b in tiles:
        for r in b:
            row = r.to_bytes(8, "little")
            out += row if bpp == 8 else bytes(row[i] | row[i + 1] << 4 for i in range(0, 8, 2))
    return bytes(out)


def write_if_changed(path, data):
    """Writes `data` unless the file already holds it, so make sees an
    unchanged output as unchanged (levels.mk's tile set rules)."""
    path = Path(path)
    if path.exists() and path.read_bytes() == data:
        return
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(data)


def tileset_png(tiles, banks, bpp, palette):
    """A viewing picture of a tile set: 16 tiles a row, a BG1-3 tile drawn in
    the bank of its first cell, with the palette of its first room."""
    import io

    from PIL import Image

    rows = -(-len(tiles) // 16)
    img = Image.new("P", (128, rows * 8), 0)
    px = img.load()
    for t, b in enumerate(tiles):
        add = 0 if bpp == 8 else banks[t] * 16
        for y, r in enumerate(b):
            for x, p in enumerate(r.to_bytes(8, "little")):
                px[t % 16 * 8 + x, t // 16 * 8 + y] = p + add
    img.putpalette([c for v in palette for c in jasc_pal.to_rgb(v)])
    buf = io.BytesIO()
    img.save(buf, format="PNG")
    return buf.getvalue()


def tileset_build(symbol, build_dir):
    """`levels.py tileset`: the set's tiles (BUILD/tilesets/SYMBOL.bin), its
    size (SYMBOL.h), a viewing picture (SYMBOL.png) and its layers' cells
    (BUILD/<room>/<layer>.map.bin), each written only if it changed."""
    levels = read_json(LEVELS_JSON)
    users = tileset_users(levels)
    tiles, banks, maps = generate_tileset(symbol, levels, users)
    bpp = users[symbol]["bpp"]
    data = tileset_bytes(tiles, bpp)
    out = Path(build_dir) / "tilesets"
    write_if_changed(out / (symbol + ".bin"), data)
    write_if_changed(out / (symbol + ".h"), ("/* Generated by tools/levels.py tileset - do not edit. */\n"
                                             "#define TILES_SIZE_%s %#x\n" % (symbol, len(data))).encode())
    first = next(iter(maps))[0]
    room_dir = LEVELS_DIR / first
    write_if_changed(out / (symbol + ".png"),
                     tileset_png(tiles, banks, bpp, room_palette(room_dir, load_room(room_dir))))
    for (room, lname), cells in maps.items():
        write_if_changed(Path(build_dir) / room / ("%s.map.bin" % lname), struct.pack("<%dH" % len(cells), *cells))


def tilesets_mk(out_path, build_dir):
    """`levels.py tilesets-mk`: levels.mk's rules for the tile sets, one per
    set the rooms name. A set's stamp depends on its rooms' room.json and
    layer pictures; its outputs depend on the stamp with an empty recipe,
    so an output tileset_build left as it was doesn't rebuild what uses it."""
    levels = read_json(LEVELS_JSON)
    users = tileset_users(levels)
    order = load_tileset_order()
    order_dep = " " + TILESET_ORDER_JSON.as_posix() if TILESET_ORDER_JSON.exists() else ""
    b = build_dir
    new = [s for s, spec in users.items() if not is_rom_tileset(spec, levels)]
    lines = ["# Generated by tools/levels.py tilesets-mk from data/levels/ - do not edit.",
             "LEVEL_TILESETS := " + " ".join(users),
             "LEVEL_NEW_TILESETS_8BPP := " + " ".join(s for s in new if users[s]["bpp"] == 8),
             "LEVEL_NEW_TILESETS_4BPP := " + " ".join(s for s in new if users[s]["bpp"] == 4)]
    for symbol, spec in users.items():
        rooms = tileset_rooms(symbol, spec, levels, order)
        deps = []
        for r, lays in rooms:
            layers = read_json(LEVELS_DIR / r / "room.json")["layers"]
            deps.append("data/levels/%s/room.json" % r)
            deps += ["data/levels/%s/%s" % (r, layers[lname]["map"]) for lname in lays]
        stamp = "%s/tilesets/%s.stamp" % (b, symbol)
        lines += ["%s: %s%s tools/levels.py" % (stamp, " ".join(deps), order_dep),
                  "\t@mkdir -p $(dir $@)",
                  "\tpython3 tools/levels.py tileset %s --build %s" % (symbol, b),
                  "\t@touch $@",
                  "%s/tilesets/%s.bin %s/tilesets/%s.h: %s ;" % (b, symbol, b, symbol, stamp)]
        lines += ["%s/%s/%s.map.bin: %s ;" % (b, r, lname, stamp) for r, lays in rooms for lname in lays]
    Path(out_path).parent.mkdir(parents=True, exist_ok=True)
    Path(out_path).write_text("\n".join(lines) + "\n")


def new_tilesets(levels):
    """The new tile sets (used by no room of a ROM region): [(symbol, spec)]."""
    return [(symbol, spec) for symbol, spec in tileset_users(levels).items() if not is_rom_tileset(spec, levels)]


def new_tilesets_c(levels, build_dir):
    """The C of the new tile sets, which the last room region's C starts
    its new data with: an 8bpp (BG0) set as a tag-0x00 raw tagged asset
    (`{size << 8}`, then the tiles; its size from the built
    `tilesets/<set>.bin`), a 4bpp (BG1-3) set as its LZ77 stream, like
    the ROM's (src/game/level/data/level_tilesets_*.cpp, level_bg_tiles.cpp)."""
    defs = []
    for symbol, spec in new_tilesets(levels):
        defs.append("/* new tile set, %dbpp */" % spec["bpp"])
        defs.append("extern const u8 %s[] __attribute__((aligned(4))) = {" % symbol)
        if spec["bpp"] == 8:
            n = (Path(build_dir) / "tilesets" / (symbol + ".bin")).stat().st_size
            defs.append("    0x00, 0x%02X, 0x%02X, 0x%02X," % (n & 0xFF, n >> 8 & 0xFF, n >> 16 & 0xFF))
            defs.append('#include "tilesets/%s.bin.inc"' % symbol)
        else:
            defs.append('#include "tilesets/%s.bin.lz.inc"' % symbol)
        defs += ["};", ""]
    return defs


# ---------------------------------------------------------------------------
# The level table (src/game/level/data/level_table.cpp, docs/levels.md, "The
# level table")
#
# - gLevelTable: per level of levels.json's "levels" (in id order) a
#   struct level_info, from the level's "name_text", "theme", "times",
#   "mask_assist_deaths", "crate_assist_deaths" and "boss";
# - gLevelRoomLists: per level a struct level_room_list, from its "rooms"
#   (played in order: room folders, and {"stage": CATEGORY} for a stage
#   played in an actor category), "bonus" (extra1) and "gem_path" (extra2);
# - the room records: per room a struct level_room (room.json's "record":
#   its symbol, "kind" and "blend"), and the stage records gLevelStageN,
#   N the stage's actor category.
#
# levels.json's "table" is the ROM's layout of the records and lists:
# "lists", the ROM's played-room arrays (gLevelNNRooms, NN the level id)
# in ROM order. The arrays are defined in that order (a new level's after
# them, in id order), each with the rooms its level plays now; the order
# of the records follows: first the rooms no array plays (the extra
# rooms: bonus and gem-path rooms, in "rooms" order), then the records of
# the rooms and stages the arrays play, each the first time one does.
# With the ROM's levels that is the ROM's layout; anything else grows or
# shrinks the table in place, and the ROM data after it moves
# (docs/shifting.md).

# The record kinds (ROOM_KIND_*, include/constants/rooms.h); a room's
# "kind" is one of the first three.
ROOM_KINDS = ["ON_FOOT", "UNDERWATER", "HOVER", "CATEGORY"]
# gLevelTable (struct level_info, 0x24 bytes) and the ROM's room table.
LEVEL_TABLE = 0x0816C86C
LEVEL_INFO_SIZE = 0x24
LEVEL_ROOM_SIZE = 0x14
# The keys every level of levels.json's "levels" has ("bonus", "gem_path"
# and "boss" are optional).
LEVEL_KEYS = ["constant", "name", "name_text", "theme", "times", "mask_assist_deaths", "crate_assist_deaths",
              "rooms"]


def stage_symbol(number):
    return "gLevelStage%d" % number


def room_list_symbol(level_id):
    return "gLevel%02dRooms" % level_id


class LevelTable:
    """The level table of levels.json and the rooms' room.json
    (`level_table_c`, `records_c`)."""

    def __init__(self, levels):
        self.levels = levels["levels"]
        self.ids = {lv["constant"]: i for i, lv in enumerate(self.levels)}
        table = levels.get("table")
        if table is None:
            sys.exit("%s: no \"table\" (the ROM's level table layout, written by levels.py extract)" % LEVELS_JSON)
        listed = [lst["level"] for lst in table["lists"]]
        if len(set(listed)) != len(listed):
            sys.exit("%s: \"table\" lists a level twice" % LEVELS_JSON)
        self.rooms = levels["rooms"]
        self.rom_rooms = set(rom_region_rooms(levels))
        self.cats = categories()
        self.records = {r: read_json(LEVELS_DIR / r / "room.json") for r in self.rooms}
        # the arrays' order: the ROM's (the levels "table" lists, which a
        # level that is gone leaves), then the new levels' in id order
        self.order = [c for c in listed if c in self.ids] + [lv["constant"] for lv in self.levels
                                                             if lv["constant"] not in listed]
        for lv in self.levels:
            missing = [k for k in LEVEL_KEYS if k not in lv]
            if missing:
                sys.exit("%s: %s has no %s" % (LEVELS_JSON, lv["constant"], ", ".join('"%s"' % k for k in missing)))
            if len(lv["times"]) != 3:
                sys.exit("%s: %s: \"times\" is three times, loosest first" % (LEVELS_JSON, lv["constant"]))
            if not lv["rooms"]:
                sys.exit("%s: %s plays no room (\"rooms\")" % (LEVELS_JSON, lv["constant"]))
            for k in ("bonus", "gem_path"):
                if k in lv and not isinstance(lv[k], str):
                    sys.exit("%s: %s: \"%s\" is a room folder" % (LEVELS_JSON, lv["constant"], k))
            for e in lv["rooms"] + [lv[k] for k in ("bonus", "gem_path") if lv.get(k)]:
                self.ref(e, lv["constant"])

    def ref(self, e, where):
        """A room-list entry's record symbol."""
        if isinstance(e, dict):
            if set(e) != {"stage"} or e["stage"] not in self.cats:
                sys.exit("%s: %s: %s isn't a room folder or {\"stage\": <a category of \"categories\", its "
                         "constant without CATEGORY_>}" % (LEVELS_JSON, where, json.dumps(e)))
            return stage_symbol(self.cats[e["stage"]])
        if e not in self.records:
            sys.exit("%s: %s plays %s, which isn't in \"rooms\"" % (LEVELS_JSON, where, e))
        rec = self.records[e].get("record")
        if rec is None:
            sys.exit("data/levels/%s/room.json: no \"record\" (the room's struct level_room: its symbol, "
                     "\"kind\" and \"blend\")" % e)
        return rec["symbol"]

    def array(self, constant):
        """The symbol and entries of the array a level plays."""
        i = self.ids[constant]
        return room_list_symbol(i), self.levels[i]["rooms"]

    def room_record(self, r):
        rec = self.records[r]["record"]
        sym = self.records[r]["symbols"]
        if rec.get("kind") not in ROOM_KINDS[:3]:
            sys.exit("data/levels/%s/room.json: \"record\" \"kind\" is one of %s" % (r, ", ".join(ROOM_KINDS[:3])))
        b = rec.get("blend", {})
        blend = [b.get(k, 0) for k in ("effect", "eva", "evb")]
        if not (0 <= blend[0] <= 0xFFFF and 0 <= blend[1] <= 0xFF and 0 <= blend[2] <= 0xFF):
            sys.exit("data/levels/%s/room.json: \"blend\" is a u16 \"effect\" and u8 \"eva\", \"evb\"" % r)
        return ["extern const struct level_room %s = {" % rec["symbol"],
                "    %s, &%s, ROOM_KIND_%s, 0, { { %d, %d, %d } }" % (member(self.records[r], "palette"), sym["desc"],
                                                                     rec["kind"], *blend),
                "}; /* data/levels/%s */" % r]

    @staticmethod
    def stage_record(name, number):
        return ["extern const struct level_room %s = {" % stage_symbol(number),
                "    0, 0, ROOM_KIND_CATEGORY, 0, { { /* catIndex */ CATEGORY_%s } }" % name,
                "};"]

    def array_c(self, symbol, entries, comment):
        return ["extern const struct level_room *const %s[%d] = { %s }; /* %s */"
                % (symbol, len(entries), ", ".join("&" + self.ref(e, symbol) for e in entries), comment)]

    def externs(self, rooms=(), arrays=(), records=()):
        out = []
        for r in rooms:
            sym = self.records[r]["symbols"]
            out += ["extern const struct level_desc %s;" % sym["desc"]]
        out += ["extern const struct level_room *const %s[%d];" % a for a in arrays]
        out += ["extern const struct level_room %s;" % s for s in records]
        return out

    def all_arrays(self):
        """(symbol, length) of every played-room array, in definition order."""
        return [(self.array(c)[0], len(self.array(c)[1])) for c in self.order]

    def all_records(self):
        return [self.records[r]["record"]["symbol"] for r in self.rooms] + \
            [stage_symbol(n) for n in sorted(self.cats.values())]

    def level_table_c(self, data):
        """The colour cycles' lists, gLevelTable, gLevelRoomLists and
        gThemeMusicCues (level_table.inc)."""
        out = ["/* Generated by tools/levels.py from data/levels/levels.json - do not edit. */", ""]
        out += palette_cycles_c(data) + [""]
        out += ["extern const struct level_room_list gLevelRoomLists[LEVEL_COUNT];"]
        out += self.externs(arrays=self.all_arrays(), records=self.all_records()) + [""]
        out.append("extern const struct level_info gLevelTable[LEVEL_COUNT] = {")
        for lv in self.levels:
            c = lv["constant"]
            out.append("    { %d, %d, { %s }, %d, %d, %d, &gLevelRoomLists[%s] }, /* %s */"
                       % (lv["name_text"], lv["theme"], ", ".join("%d" % t for t in lv["times"]),
                          lv["mask_assist_deaths"], lv["crate_assist_deaths"], 1 if lv.get("boss") else 0, c, c))
        out += ["};", ""]
        out.append("extern const struct level_room_list gLevelRoomLists[LEVEL_COUNT] = {")
        for lv in self.levels:
            sym, _ = self.array(lv["constant"])
            extra = ["&" + self.ref(lv[k], lv["constant"]) if lv.get(k) else "0" for k in ("bonus", "gem_path")]
            out.append("    { ARRAY_COUNT(%s), %s, %s, %s }, /* %s */" % (sym, sym, extra[0], extra[1], lv["constant"]))
        out += ["};", ""]
        out += music_cues_c(data) + [""]
        return "\n".join(out)

    def records_c(self):
        """The room records and played-room arrays, in ROM order
        (level_table_16c814_rooms.inc): the extra rooms' records, the
        arrays, then the records of the rooms and stages they play."""
        arrays = [self.array(c) for c in self.order]
        in_lists = {e for _, entries in arrays for e in entries if isinstance(e, str)}
        extras = [r for r in self.rooms if r not in in_lists]
        played, seen = [], set()
        for _, entries in arrays:
            for e in entries:
                key = json.dumps(e)
                if key not in seen:
                    seen.add(key)
                    played.append(e)
        out = ["/* Generated by tools/levels.py from data/levels/ - do not edit. */", ""]
        out += self.externs(rooms=extras + [e for e in played if isinstance(e, str)],
                            records=[self.ref(e, "table") for e in played]) + [""]
        out.append("/* The extra rooms' records. */")
        for r in extras:
            out += self.room_record(r)
        out += ["", "/* The rooms each level plays, in order. */"]
        for c, (sym, entries) in zip(self.order, arrays):
            out += self.array_c(sym, entries, c)
        out += ["", "/* The records of the rooms and stages they play. */"]
        for e in played:
            out += self.room_record(e) if isinstance(e, str) else self.stage_record(e["stage"], self.cats[e["stage"]])
        return "\n".join(out) + "\n"


def table_c(which, out_path):
    """`levels.py table levels|records|pictures OUT`: the in-place level
    table, or the level select's pictures."""
    data = read_json(LEVELS_JSON)
    if which == "pictures":
        text = level_select_pictures_c(data)
    else:
        t = LevelTable(data)
        text = t.level_table_c(data) if which == "levels" else t.records_c()
    Path(out_path).parent.mkdir(parents=True, exist_ok=True)
    Path(out_path).write_text(text)


# ---------------------------------------------------------------------------
# The level themes (docs/levels.md, "Themes")
#
# A level's "theme" is the index of levels.json's "themes". Everything the
# game picks by theme is a field of the theme:
#
# - "song": its music, the song (sound/gax_manifest.json's song_table) of
#   its entry of gThemeMusicCues (PlayRoomMusic);
# - "picture": its level-select picture (graphics/manifest.json's entry,
#   by path), its entry of gLevelSelectPictures (ZoomBg::SetPicture). A
#   theme without one has no entry: its levels may not be on the level
#   select's grid (mega-mix's);
# - "palette_cycle": the colour cycle RunRoom starts at the level's start,
#   by name, one of levels.json's "palette_cycles". A theme without one
#   stops the cycles (`gPaletteCycles->active = 0`);
# - "first_room_exit_zone": true when the exit of a level's first room is
#   a plain zone, not the exit pad (SpawnRoomExit; the snow levels', whose
#   first room leads into the polar ride).
#
# "palette_cycles" are the colour cycles, in ROM order (definition order =
# the order of RunRoom's cases): each a name and its lists, each list a
# palette-entry array (its "symbol" and "entries", the BG palette entries
# it cycles) and the AddPaletteCycle "rate" and "direction" of it.
#
# With the ROM's themes the generated tables and RunRoom's cases are the
# ROM's; `extract` reads the songs, the pictures and the cycles' entries
# back from the ROM.

GAX_MANIFEST = Path("sound/gax_manifest.json")
GFX_MANIFEST = Path("graphics/manifest.json")
THEME_KEYS = ["constant", "name", "song"]
THEME_OPTIONAL_KEYS = ["picture", "palette_cycle", "first_room_exit_zone"]
# The ROM's theme tables: the colour cycles' entry lists (back to back,
# up to gLevelTable), gLevelSelectPictures (10 {palette, tiles} pairs:
# mega-mix, the last theme, has none) and gThemeMusicCues (a byte per
# theme, after gLevelRoomLists).
PALETTE_CYCLES = 0x0816C814
LEVEL_SELECT_PICTURES = 0x0816C5A0
ROM_LEVEL_SELECT_PICTURE_COUNT = 10
THEME_MUSIC_CUES = 0x0816CD80


def song_names():
    """The songs, by name, in song id order (SONG_<NAME>, constants/songs.h)."""
    return [s["song"] for s in read_json(GAX_MANIFEST)["song_table"]]


def level_select_picture_classes():
    """graphics/manifest.json's level-select pictures: path -> its entry."""
    assets = read_json(GFX_MANIFEST)["assets"]
    return {e["path"]: e for e in assets
            if e.get("path", "").startswith("level_select/pictures/") and "class" in e}


def level_themes(data):
    """levels.json's "themes", in theme order, checked: every level's
    "theme" is one of them, and each theme's song, picture and colour
    cycle exist. A level on the level select's grid (one with a "world")
    needs a theme with a picture."""
    themes = data.get("themes")
    if not themes:
        sys.exit("%s: no \"themes\" (each level theme's constant, name and song)" % LEVELS_JSON)

    def fail(msg):
        sys.exit("%s: %s" % (LEVELS_JSON, msg))

    songs = song_names()
    pictures = level_select_picture_classes()
    cycles = [c["name"] for c in data.get("palette_cycles", [])]
    for t in themes:
        missing = [k for k in THEME_KEYS if k not in t]
        if missing:
            fail("theme %s has no %s" % (t.get("constant", "?"), ", ".join('"%s"' % k for k in missing)))
        unknown = [k for k in t if k not in THEME_KEYS + THEME_OPTIONAL_KEYS]
        if unknown:
            fail("%s: unknown %s" % (t["constant"], ", ".join('"%s"' % k for k in unknown)))
        if t["song"] not in songs:
            fail("%s: \"song\" %s isn't a song of %s's song_table (%s)" % (t["constant"], t["song"], GAX_MANIFEST,
                                                                          ", ".join(songs)))
        if "picture" in t and t["picture"] not in pictures:
            fail("%s: \"picture\" %s isn't a level-select picture of %s (level_select/pictures/*.png with a "
                 "\"class\")" % (t["constant"], t["picture"], GFX_MANIFEST))
        if "palette_cycle" in t and t["palette_cycle"] not in cycles:
            fail("%s: \"palette_cycle\" %s isn't one of \"palette_cycles\" (%s)" % (t["constant"], t["palette_cycle"],
                                                                                   ", ".join(cycles)))
        if not isinstance(t.get("first_room_exit_zone", False), bool):
            fail("%s: \"first_room_exit_zone\" is true or false" % t["constant"])
    names = [c["name"] for c in data.get("palette_cycles", [])]
    if len(set(names)) != len(names):
        fail("two \"palette_cycles\" have the same name")
    for c in data.get("palette_cycles", []):
        if not c.get("lists"):
            fail("palette cycle %s has no \"lists\"" % c["name"])
        for lst in c["lists"]:
            if not lst.get("entries") or not all(0 <= e <= 0xFF for e in lst["entries"]):
                fail("palette cycle %s: %s's \"entries\" are BG palette entries, 0-255" % (c["name"], lst["symbol"]))
            if not 0 < lst["rate"] <= 0xFF or lst["direction"] not in (0, 1):
                fail("palette cycle %s: %s: \"rate\" is 1-255 frames, \"direction\" 0 or 1" % (c["name"],
                                                                                              lst["symbol"]))
    for lv in data["levels"]:
        if not 0 <= lv["theme"] < len(themes):
            fail("%s's theme %d isn't one of the %d \"themes\"" % (lv["constant"], lv["theme"], len(themes)))
        if lv.get("world") is not None and "picture" not in themes[lv["theme"]]:
            fail("%s is on the level select's grid, but its theme %s has no \"picture\" (ZoomBg::SetPicture "
                 "shows its theme's)" % (lv["constant"], themes[lv["theme"]]["constant"]))
    return themes


def level_select_picture_count(themes):
    """gLevelSelectPictures' length: up to the last theme with a picture."""
    return max([i + 1 for i, t in enumerate(themes) if "picture" in t] + [0])


def palette_cycles_c(data):
    """The colour cycles' entry lists (level_table.inc, before gLevelTable)."""
    level_themes(data)
    out = ["/* The colour cycles' palette-entry lists (levels.json's \"palette_cycles\"): each",
           " * is the `lists` argument of an AddPaletteCycle call of RunRoom on BG palette RAM",
           " * (theme_palette_cycles.h). */"]
    for c in data.get("palette_cycles", []):
        users = [t["constant"] for t in data["themes"] if t.get("palette_cycle") == c["name"]]
        for lst in c["lists"]:
            out.append("extern const u16 %s[%d] = { %s }; /* %s: %s */"
                       % (lst["symbol"], len(lst["entries"]), ", ".join("0x%02x" % e for e in lst["entries"]),
                          c["name"], ", ".join(users) or "no theme"))
    return out


def music_cues_c(data):
    """gThemeMusicCues (level_table.inc, after gLevelRoomLists)."""
    themes = level_themes(data)
    out = ["/* The music cue of each level theme (`struct level_info.theme`, levels.json's",
           " * \"themes\" \"song\"), read by PlayRoomMusic (room_select.cpp). */",
           "extern const u8 gThemeMusicCues[THEME_COUNT] = {"]
    out += ["    SONG_%s, /* %s */" % (t["song"].upper(), t["constant"]) for t in themes]
    return out + ["};"]


def level_select_pictures_c(data):
    """gLevelSelectPictures (level_select_pictures.inc): per theme up to the
    last one with a picture, its picture's graphics class members. A theme
    before it without a picture gets an empty pair, which nothing reads:
    its levels are off the level select's grid (level_themes)."""
    themes = level_themes(data)
    classes = level_select_picture_classes()
    out = ["/* Generated by tools/levels.py from data/levels/levels.json - do not edit. */", "",
           "/* Per level theme its {palette, tiles} pair (levels.json's \"themes\" \"picture\"):",
           " * ZoomBg::Update (zoom_bg.cpp) loads both through LoadTaggedAsset. */",
           "extern const struct image_pair gLevelSelectPictures[LEVEL_SELECT_PICTURE_COUNT] = {"]
    for t in themes[:level_select_picture_count(themes)]:
        if "picture" in t:
            cls = "Engine::Gfx::" + classes[t["picture"]]["class"]
            out.append("    { %s::palette, %s::tiles }, /* %s: graphics/%s */" % (cls, cls, t["constant"],
                                                                                 t["picture"]))
        else:
            out.append("    { 0, 0 }, /* %s: no picture (off the grid) */" % t["constant"])
    return "\n".join(out + ["};", ""])


def theme_palette_cycles_h(data):
    """theme_palette_cycles.h: the colour cycles' lists, and RunRoom's
    switch cases on the level's theme (THEME_PALETTE_CYCLE_CASES), one per
    cycle in "palette_cycles" order, each labelled with the themes that
    name it. A theme with no cycle takes RunRoom's default case."""
    themes = level_themes(data)
    out = ["/* Generated by tools/levels.py from data/levels/levels.json. Do not edit. */",
           "#ifndef GUARD_THEME_PALETTE_CYCLES_H", "#define GUARD_THEME_PALETTE_CYCLES_H", "",
           "/*",
           " * The level themes' colour cycles (levels.json's \"palette_cycles\", docs/levels.md",
           " * \"Themes\"): the palette-entry lists (src/game/level/data/level_table.cpp), and",
           " * RunRoom's cases (play_room.cpp), each starting the cycles of a theme",
           " * (levels.json's \"themes\" \"palette_cycle\") on BG palette RAM through FX_CYCLE",
           " * (AddPaletteCycle: lists, rate, count, direction).",
           " */", ""]
    for c in data.get("palette_cycles", []):
        out += ["extern const u16 %s[%d];" % (lst["symbol"], len(lst["entries"])) for lst in c["lists"]]
    lines = []
    for c in data.get("palette_cycles", []):
        lines += ["case %s:" % t["constant"] for t in themes if t.get("palette_cycle") == c["name"]]
        lines += ["    gPaletteCycles->Clear();"]
        lines += ["    FX_CYCLE(%s, %d, %d, %d);" % (lst["symbol"], lst["rate"], len(lst["entries"]), lst["direction"])
                  for lst in c["lists"]]
        lines += ["    break;"]
    width = max(len(s) for s in lines) if lines else 0
    out += ["", "#define THEME_PALETTE_CYCLE_CASES%s" % (" \\" if lines else "")]
    out += ["    %s%s" % (s.ljust(width) + " \\" if k + 1 < len(lines) else s, "") for k, s in enumerate(lines)]
    out += ["", "#endif /* GUARD_THEME_PALETTE_CYCLES_H */", ""]
    return "\n".join(out)


def themes_from_rom(rom, kept):
    """extract: levels.json's "themes" and "palette_cycles". The constants,
    names, the cycle each theme picks and the cycles' names, symbols,
    lengths, rates and directions (RunRoom's code) are kept; the songs
    (gThemeMusicCues), the pictures (gLevelSelectPictures) and the cycles'
    entries come from the ROM."""
    songs = song_names()
    by_rom = {(int(e["palette"]["rom"], 16), int(e["rom"], 16)): path
              for path, e in level_select_picture_classes().items() if "rom" in e}
    themes = []
    for i, k in enumerate(kept["themes"]):
        t = {"constant": k["constant"], "name": k["name"], "song": songs[rom.b(THEME_MUSIC_CUES + i)]}
        if i < ROM_LEVEL_SELECT_PICTURE_COUNT:
            a = LEVEL_SELECT_PICTURES + 8 * i
            t["picture"] = by_rom[(rom.w(a), rom.w(a + 4))]
        t.update({key: k[key] for key in THEME_OPTIONAL_KEYS[1:] if key in k})
        themes.append(t)
    cycles, a = [], PALETTE_CYCLES
    for c in kept["palette_cycles"]:
        lists = []
        for lst in c["lists"]:
            n = len(lst["entries"])
            lists.append({"symbol": lst["symbol"], "rate": lst["rate"], "direction": lst["direction"],
                          "entries": [rom.h(a + 2 * k) for k in range(n)]})
            a += 2 * n
        cycles.append({"name": c["name"], "lists": lists})
    assert a == LEVEL_TABLE, "the colour cycles' lists don't run up to gLevelTable"
    return themes, cycles


# ---------------------------------------------------------------------------
# The actor categories (docs/levels.md, "Actor categories")
#
# A stage ({"stage": NAME} in a level's "rooms", a ROOM_KIND_CATEGORY
# record) runs an actor category: ActorCategory::Init(catIndex) plays
# ActorCategory::descriptors[catIndex] (struct category_descriptor,
# actor_anim.h). levels.json's "categories" has one entry per category, in
# CATEGORY_* order:
#
# - "constant", "name": its CATEGORY_* constant (constants/categories.h)
#   and what it is; a stage names it without the prefix;
# - "type": POLAR, JETPACK or HOVERCRAFT, the index of
#   ActorCategory::vtables (the player, the boss, the spawner: code). The
#   type also picks the BG0 cell animation, the OBJ palette, the
#   animation table and the sprite sheet (CATEGORY_TYPES: the ROM's
#   categories of a type all share them);
# - "sky" (optional): the BG1 picture (bgPicture), one of the jetpack
#   skies (CATEGORY_SKIES); none for the polar stages;
# - "spawns": its spawn list, data/levels/spawns/<name>.json;
# - "mask_assist_deaths", "bonus_kind_deaths", "retry_boss_deaths",
#   "boss_level", "retry_boss_level": the descriptor's numbers.
#
# A spawn list (struct actor_spawn_table, actor.h) is a JSON file:
# "symbol" (its C name), "family" (POLAR or JETPACK: the kinds' numbering,
# POLAR_KIND_* or JETPACK_KIND_*, constants/actor_kinds.h; a category's
# type picks it), the course "length", the "spawns" in depth order (each
# {"kind", "x", "y", "z"}, plus "alt" and "bonus" when the time-trial and
# bonus kinds differ from "kind", "unk_03" and "next" when not 0 and -1)
# and the 4 "end" bytes nothing reads, when not 0. The ROM's seven are
# generated in place, where their C files #include them (ROM_SPAWN_TABLES);
# any other is defined with the descriptors (actor_categories.inc). A new
# category, or a new mix of the type's existing actor kinds, is data:
# nothing in the code switches on the category (docs/levels.md).

CATEGORY_KEYS = ["constant", "name", "type", "spawns", "mask_assist_deaths", "bonus_kind_deaths",
                 "retry_boss_deaths", "boss_level", "retry_boss_level"]
SPAWNS_DIR = LEVELS_DIR / "spawns"
ACTOR_KINDS_H = Path("include/constants/actor_kinds.h")
# Per type: the family of its actor kinds, the BG0 cell animation and its
# size, the OBJ palette, the animation table, the sprite sheet, and the
# boss levels its boss has (Yeti::chargeParams[6]; Airship::attacks[6],
# `attacks[kind] + level` with kinds 0-2; Hovercraft::attacks[2]).
CATEGORY_TYPES = {
    "POLAR": {"family": "POLAR", "cell_anim": "gCategoryFamily0CellAnim", "cell_anim_size": 0x75B94,
              "palette": "gPolarCategoryPalette", "anim_table": "gCategoryFamily0AnimTable",
              "sprite_sheet": "gPolarSpriteSheet", "boss_levels": 6},
    "JETPACK": {"family": "JETPACK", "cell_anim": "gCategoryFamily1CellAnim", "cell_anim_size": 0x3E784,
                "palette": "gAirshipCategoryPalette", "anim_table": "gCategoryFamily1AnimTable",
                "sprite_sheet": "gJetpackSpriteSheet", "boss_levels": 4},
    "HOVERCRAFT": {"family": "JETPACK", "cell_anim": "gCategoryFamily1CellAnim", "cell_anim_size": 0x3E784,
                   "palette": "gHovercraftCategoryPalette", "anim_table": "gCategoryFamily1AnimTable",
                   "sprite_sheet": "gJetpackSpriteSheet", "boss_levels": 2},
}
CATEGORY_TYPE_NAMES = ["POLAR", "JETPACK", "HOVERCRAFT"]  # CATEGORY_TYPE_*, the vtables' order
# The BG1 skies (graphics/vehicles/jetpack/skies/<name>.png).
CATEGORY_SKIES = {"rocket_racket": "gCategory3BgPicture", "blimp_bonanza": "gCategory4BgPicture",
                  "no_fly_zone": "gCategory5BgPicture"}
# The kinds only one type's spawner can spawn: the airship needs
# Airship::Create (the jetpack type's boss), the hovercraft is the
# hovercraft type's boss (JetpackPlayer::SpawnActor, jetpack_spawn.cpp).
TYPE_ONLY_KINDS = {"AIRSHIP_0": "JETPACK", "AIRSHIP_1": "JETPACK", "AIRSHIP_2": "JETPACK",
                   "HOVERCRAFT": "HOVERCRAFT"}
# The ROM's spawn lists, in their C files' #include slots, and where they
# are in the ROM.
ROM_SPAWN_TABLES = {
    "gCategory0SpawnTable": 0x080B1444, "gCategory1SpawnTable": 0x080C0C38, "gCategory2SpawnTable": 0x080C19F0,
    "gCategory3SpawnTable": 0x08140DCC, "gCategory4SpawnTable": 0x0815405C, "gCategory5SpawnTable": 0x08158918,
    "gCategory6SpawnTable": 0x0815A030,
}
# ActorCategory::descriptors (gActorCategories, 0x34 bytes each) and the
# ROM addresses of what the descriptors name, for extract.
ACTOR_CATEGORIES = 0x08175558
CATEGORY_DESCRIPTOR_SIZE = 0x34
ROM_CATEGORY_SYMBOLS = {
    0x0803B8B0: "gCategoryFamily0CellAnim", 0x080FF1B0: "gCategoryFamily1CellAnim",
    0x08178F80: "gPolarCategoryPalette", 0x0817AAA4: "gAirshipCategoryPalette",
    0x0817AEA4: "gHovercraftCategoryPalette", 0x081796CC: "gCategoryFamily0AnimTable",
    0x0817B2A4: "gCategoryFamily1AnimTable", 0x080B2120: "gPolarSpriteSheet", 0x0814174C: "gJetpackSpriteSheet",
    0x0813D934: "gCategory3BgPicture", 0x08151AC4: "gCategory4BgPicture", 0x08155260: "gCategory5BgPicture",
}


def actor_kinds():
    """{family: {name without the prefix: kind}} of constants/actor_kinds.h."""
    out = {"POLAR": {}, "JETPACK": {}}
    for m in re.finditer(r"^#define (POLAR|JETPACK)_KIND_(\w+) (\d+|\(ACTOR_KIND_PATH_POINT \+ (\d+)\))",
                         ACTOR_KINDS_H.read_text(), re.M):
        out[m.group(1)][m.group(2)] = 0x20 + int(m.group(4)) if m.group(4) else int(m.group(3))
    return out


def level_categories(data):
    """levels.json's "categories", checked."""
    cats = data.get("categories")

    def fail(msg):
        sys.exit("%s: %s" % (LEVELS_JSON, msg))

    if not isinstance(cats, list) or not cats:
        fail("\"categories\" lists the actor categories (docs/levels.md, \"Actor categories\")")
    names = [c.get("constant") for c in cats]
    if len(set(names)) != len(names) or not all(isinstance(n, str) and re.fullmatch(r"CATEGORY_[A-Z0-9_]+", n)
                                                and not n.startswith("CATEGORY_TYPE_") and
                                                not n.startswith("CATEGORY_EXIT_") and n != "CATEGORY_COUNT"
                                                for n in names):
        fail("each category has a unique \"constant\", CATEGORY_<NAME> (not a CATEGORY_TYPE_/EXIT_ one)")
    for c in cats:
        missing = [k for k in CATEGORY_KEYS if k not in c]
        if missing:
            fail("%s has no %s" % (c["constant"], ", ".join('"%s"' % k for k in missing)))
        unknown = [k for k in c if k not in CATEGORY_KEYS + ["sky"]]
        if unknown:
            fail("%s: unknown %s" % (c["constant"], ", ".join('"%s"' % k for k in unknown)))
        if c["type"] not in CATEGORY_TYPES:
            fail("%s: \"type\" is one of %s" % (c["constant"], ", ".join(CATEGORY_TYPE_NAMES)))
        if "sky" in c and (c["type"] == "POLAR" or c["sky"] not in CATEGORY_SKIES):
            fail("%s: \"sky\" is one of %s, for a jetpack or hovercraft category"
                 % (c["constant"], ", ".join(CATEGORY_SKIES)))
        for k in CATEGORY_KEYS[4:]:
            if not isinstance(c[k], int) or not 0 <= c[k] < 0x10000:
                fail("%s: \"%s\" is a number" % (c["constant"], k))
        top = CATEGORY_TYPES[c["type"]]["boss_levels"]
        for k in ("boss_level", "retry_boss_level"):
            if c[k] >= top:
                fail("%s: \"%s\" is 0-%d (the %s boss's levels)" % (c["constant"], k, top - 1, c["type"].lower()))
        spawns = spawn_list(c["spawns"])
        if spawns["family"] != CATEGORY_TYPES[c["type"]]["family"]:
            fail("%s: its spawn list %s has %s kinds, its type %s's are %s"
                 % (c["constant"], c["spawns"], spawns["family"], c["type"], CATEGORY_TYPES[c["type"]]["family"]))
        for e in spawns["spawns"]:
            for k in ("kind", "alt", "bonus"):
                only = TYPE_ONLY_KINDS.get(e.get(k))
                if only and only != c["type"]:
                    fail("%s: its spawn list %s has %s_KIND_%s, which only a %s category spawns"
                         % (c["constant"], c["spawns"], spawns["family"], e[k], only))
    symbols = [spawn_list(c["spawns"])["symbol"] for c in cats]
    for c, s in zip(cats, symbols):
        clash = [c2["spawns"] for c2, s2 in zip(cats, symbols) if s2 == s and c2["spawns"] != c["spawns"]]
        if clash:
            fail("the spawn lists %s and %s are both %s" % (c["spawns"], clash[0], s))
    return cats


def categories():
    """The actor categories, levels.json's "categories": name without the
    CATEGORY_ prefix -> number."""
    return {c["constant"][len("CATEGORY_"):]: i for i, c in enumerate(level_categories(read_json(LEVELS_JSON)))}


_spawn_lists = {}


def spawn_list(name):
    """data/levels/spawns/<name>.json, checked."""
    if name in _spawn_lists:
        return _spawn_lists[name]
    path = SPAWNS_DIR / ("%s.json" % name)

    def fail(msg):
        sys.exit("%s: %s" % (path, msg))

    if not isinstance(name, str) or not path.exists():
        sys.exit("%s: \"spawns\" %s isn't a spawn list of %s" % (LEVELS_JSON, json.dumps(name), SPAWNS_DIR))
    s = read_json(path)
    unknown = [k for k in s if k not in ("symbol", "family", "length", "spawns", "end")]
    if unknown:
        fail("unknown %s" % ", ".join('"%s"' % k for k in unknown))
    if not re.fullmatch(r"g[A-Za-z0-9_]+", s.get("symbol", "")):
        fail("\"symbol\" is its C name, gSomething")
    kinds = actor_kinds()
    if s.get("family") not in kinds:
        fail("\"family\" is POLAR or JETPACK (the kinds of constants/actor_kinds.h it names)")
    if not isinstance(s.get("length"), int) or not 0 < s["length"] < 0x80000000:
        fail("\"length\" is the course length")
    if not isinstance(s.get("spawns"), list) or not s["spawns"]:
        fail("\"spawns\" lists the spawns")
    end = s.get("end", [0, 0, 0, 0])
    if not (isinstance(end, list) and len(end) == 4 and all(isinstance(b, int) and 0 <= b < 256 for b in end)):
        fail("\"end\" is the table's 4 last bytes")
    known = kinds[s["family"]]
    n = len(s["spawns"])
    last_z = None
    for i, e in enumerate(s["spawns"]):
        bad = [k for k in e if k not in ("kind", "alt", "bonus", "unk_03", "x", "y", "z", "next")]
        if bad or not all(k in e for k in ("kind", "x", "y", "z")):
            fail("spawn %d is {\"kind\", \"x\", \"y\", \"z\"}, with optional \"alt\", \"bonus\", \"unk_03\", "
                 "\"next\"" % i)
        for k in ("kind", "alt", "bonus"):
            v = e.get(k, 0)
            if not (v in known or (isinstance(v, int) and 0 <= v < 256)):
                fail("spawn %d: \"%s\" %s isn't a %s_KIND_* name or a byte" % (i, k, json.dumps(v), s["family"]))
        if not (isinstance(e.get("unk_03", 0), int) and 0 <= e.get("unk_03", 0) < 256):
            fail("spawn %d: \"unk_03\" is a byte" % i)
        for k in ("x", "y", "z"):
            if not isinstance(e[k], int) or not -0x80000000 <= e[k] < 0x80000000:
                fail("spawn %d: \"%s\" is a number" % (i, k))
        if "next" in e and not (isinstance(e["next"], int) and -1 <= e["next"] < n):
            fail("spawn %d: \"next\" is a spawn's index, or -1" % i)
        if last_z is not None and e["z"] < last_z:
            fail("spawn %d: the spawns go in depth order (\"z\"); %d comes after %d" % (i, e["z"], last_z))
        last_z = e["z"]
    _spawn_lists[name] = s
    return s


def kind_c(family, v):
    return "%s_KIND_%s" % (family, v) if isinstance(v, str) else "%d" % v


def spawn_table_c(name):
    """A spawn list's C definition (ACTOR_SPAWN_TABLE, actor.h)."""
    s = spawn_list(name)
    f = s["family"]
    out = ["/* data/levels/spawns/%s.json */" % name,
           "extern const ACTOR_SPAWN_TABLE(%d) %s = {" % (len(s["spawns"]), s["symbol"]),
           "    %d," % s["length"], "    %d," % len(s["spawns"]), "    {"]
    for e in s["spawns"]:
        k = e["kind"]
        out.append("        { { %s, %s, %s, %d, %d, %d, %d }, %d },"
                   % (kind_c(f, k), kind_c(f, e.get("alt", k)), kind_c(f, e.get("bonus", k)), e.get("unk_03", 0),
                      e["x"], e["y"], e["z"], e.get("next", -1)))
    out += ["    },", "    { %s }," % ", ".join("%d" % b for b in s.get("end", [0, 0, 0, 0])), "};"]
    return out


def spawns_inc(slot):
    """`levels.py spawns SLOT OUT`: what the C file of the ROM's spawn list
    SLOT (ROM_SPAWN_TABLES) #includes: the spawn list of that symbol, or
    nothing if no category has it."""
    if slot not in ROM_SPAWN_TABLES:
        sys.exit("levels.py spawns: %s isn't one of %s" % (slot, ", ".join(ROM_SPAWN_TABLES)))
    out = ["/* Generated by tools/levels.py from data/levels/ - do not edit. */"]
    lists = {spawn_list(c["spawns"])["symbol"]: c["spawns"] for c in level_categories(read_json(LEVELS_JSON))}
    if slot in lists:
        out += spawn_table_c(lists[slot])
    return "\n".join(out) + "\n"


def actor_categories_inc(data):
    """`levels.py categories OUT`: actor_categories.cpp's ActorCategory::
    descriptors, one per category, and the spawn lists that aren't the
    ROM's, before them."""
    cats = level_categories(data)
    names = list(dict.fromkeys(c["spawns"] for c in cats))
    rom = [n for n in names if spawn_list(n)["symbol"] in ROM_SPAWN_TABLES]
    new = [n for n in names if n not in rom]
    out = ["/* Generated by tools/levels.py from data/levels/levels.json - do not edit. */", ""]
    out += ["extern const u8 %s[];" % spawn_list(n)["symbol"] for n in rom]
    if new:
        out += ["", "/* The spawn lists that aren't the ROM's. */"]
        for n in new:
            out += spawn_table_c(n)
    out += ["", "/* One descriptor per actor category (levels.json's \"categories\"). */",
            "const struct category_descriptor ActorCategory::descriptors[CATEGORY_COUNT] = {"]
    for c in cats:
        t = CATEGORY_TYPES[c["type"]]
        out += ["    /* %s */" % c["constant"], "    {",
                "        CATEGORY_TYPE_%s," % c["type"],
                "        (void *)%s," % t["cell_anim"],
                "        0x%x," % t["cell_anim_size"],
                "        %s," % (CATEGORY_SKIES[c["sky"]] if "sky" in c else "0"),
                "        %s," % t["palette"],
                "        (struct actor_spawn_table *)%s%s," % ("&" if c["spawns"] in new else "",
                                                               spawn_list(c["spawns"])["symbol"]),
                "        (struct anim_table_record *)%s," % t["anim_table"],
                "        %s," % t["sprite_sheet"]]
        out += ["        %d," % c[k] for k in CATEGORY_KEYS[4:]]
        out.append("    },")
    out += ["};", ""]
    return "\n".join(out)


def spawns_from_rom(rom, a, symbol, family):
    """extract: a spawn list from the ROM."""
    # the kinds by name, the first of a value in actor_kinds.h
    first = {}
    for k, v in actor_kinds()[family].items():
        first.setdefault(v, k)
    length, count = rom.w(a), rom.w(a + 4)
    spawns = []
    for i in range(count):
        e = a + 8 + 0x14 * i
        kinds = [first.get(rom.b(e + k), rom.b(e + k)) for k in range(3)]
        sp = {"kind": kinds[0]}
        if kinds[1] != kinds[0]:
            sp["alt"] = kinds[1]
        if kinds[2] != kinds[0]:
            sp["bonus"] = kinds[2]
        if rom.b(e + 3):
            sp["unk_03"] = rom.b(e + 3)
        sp.update({k: struct.unpack_from("<i", rom.data, e + 4 * n - ROM_BASE)[0]
                   for n, k in ((1, "x"), (2, "y"), (3, "z"))})
        nxt = struct.unpack_from("<i", rom.data, e + 0x10 - ROM_BASE)[0]
        if nxt != -1:
            sp["next"] = nxt
        spawns.append(sp)
    s = {"symbol": symbol, "family": family, "length": length, "spawns": spawns}
    end = list(rom.bytes(a + 8 + 0x14 * count, 4))
    if any(end):
        s["end"] = end
    return s


def write_spawn_list(path, s):
    """A spawn list's JSON: one spawn per line."""
    head = ",\n".join('  "%s": %s' % (k, json.dumps(v)) for k, v in s.items() if k != "spawns" and k != "end")
    body = ",\n".join("    " + json.dumps(e) for e in s["spawns"])
    text = "{\n" + head + ',\n  "spawns": [\n' + body + "\n  ]"
    if "end" in s:
        text += ',\n  "end": ' + json.dumps(s["end"])
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text + "\n}\n")


def categories_from_rom(rom, kept):
    """extract: levels.json's "categories" (the kept constants, names and
    spawn list names; the rest from ActorCategory::descriptors) and the
    spawn lists, which it writes."""
    by_addr = {a: s for s, a in ROM_SPAWN_TABLES.items()}
    skies = {v: k for k, v in CATEGORY_SKIES.items()}
    out = []
    for i, k in enumerate(kept["categories"]):
        d = ACTOR_CATEGORIES + CATEGORY_DESCRIPTOR_SIZE * i
        f = [rom.w(d + 4 * n) for n in range(CATEGORY_DESCRIPTOR_SIZE // 4)]
        t = CATEGORY_TYPE_NAMES[f[0]]
        want = CATEGORY_TYPES[t]
        got = [ROM_CATEGORY_SYMBOLS.get(f[n]) for n in (1, 4, 6, 7)]
        assert got == [want["cell_anim"], want["palette"], want["anim_table"], want["sprite_sheet"]] \
            and f[2] == want["cell_anim_size"], "%s: the type's graphics aren't the ROM's: %s" % (k["constant"], got)
        c = {"constant": k["constant"], "name": k["name"], "type": t}
        if f[3]:
            c["sky"] = skies[ROM_CATEGORY_SYMBOLS[f[3]]]
        c["spawns"] = k["spawns"]
        c.update(zip(CATEGORY_KEYS[4:], f[8:13]))
        write_spawn_list(SPAWNS_DIR / ("%s.json" % k["spawns"]),
                         spawns_from_rom(rom, f[5], by_addr[f[5]], want["family"]))
        out.append(c)
    return out


# ---------------------------------------------------------------------------
# The level select's worlds (docs/levels.md, "Level-select pages")
#
# A world is a page of the level select: its numbered levels (the levels
# with that "world"), then its boss. levels.json's "worlds" has one entry
# per world, in world order, with what its page shows and what opens it:
#
# - "page_title": the page title sprite's animation (bank 47),
#   gLevelSelectWorldAnims;
# - "entry_box": the entries' box colour, an index of
#   gLevelSelectEntryBoxAnims (bank 49), gLevelSelectWorldEntryBoxAnims;
# - "boss_icon": the boss entry's icon animation (bank 51),
#   gLevelSelectEntryWorldAnims;
# - "opened_by" (every world but the first): what opens the page, the
#   case of LevelSelect::IsNextWorldOpen for the world before it:
#   {"power": NAME}, a power of GameProgress::flags (PROGRESS_BIT_<NAME>),
#   or {"levels_cleared": true}, every numbered level of the world before
#   cleared (LevelSelect::IsWorldCleared).
#
# The ROM's worlds 1-3 open with the power of the boss before them. With
# the ROM's worlds the three tables and IsNextWorldOpen's cases are the
# ROM's; `extract` reads the tables back from the ROM and keeps the rules,
# which are code.

WORLD_KEYS = ["page_title", "entry_box", "boss_icon"]
WORLD_OPEN_POWERS = ["TURBO_RUN", "SUPER_BODY_SLAM", "TORNADO_SPIN", "DOUBLE_JUMP"]
# The ROM's per-world tables (WORLD_COUNT u32s each).
LEVEL_SELECT_WORLD_ANIMS = 0x0816C548
LEVEL_SELECT_WORLD_ENTRY_BOX_ANIMS = 0x0816C538
LEVEL_SELECT_ENTRY_WORLD_ANIMS = 0x0816C624
# IsNextWorldOpen's cases in the ROM: worlds 1-3 open with the power of
# the boss before them.
ROM_WORLD_RULES = [None, {"power": "SUPER_BODY_SLAM"}, {"power": "DOUBLE_JUMP"}, {"power": "TORNADO_SPIN"}]
# What the values can name: bank 47's 13 animations, the 5 entries of
# gLevelSelectEntryBoxAnims and bank 51's 5 animations (src/engine/gfx/
# data/sprite_banks_39_55.cpp, src/game/ui/data/level_select_anims.cpp).
WORLD_KEY_LIMITS = {"page_title": 13, "entry_box": 5, "boss_icon": 5}
# A numbered level's entry shows step `level id` of bank 51's animation 0
# (LevelSelectEntry::SetLevel): the ROM's 20 number icons, then the steps
# its bank.json's "new_steps" adds (docs/graphics.md, "New frames").
LEVEL_ICONS_BANK = Path("graphics/sprites/bank51_level_select_icons/bank.json")
LEVEL_ICONS_ROM_STEPS = 20


def level_select_worlds(data):
    """levels.json's "worlds", checked: one per world of the levels, each
    with its page's animations and, past the first, the rule that opens it;
    and a number icon for every numbered level."""
    worlds = data.get("worlds")
    c = level_counts(data)

    def fail(msg):
        sys.exit("%s: %s" % (LEVELS_JSON, msg))

    if not isinstance(worlds, list) or len(worlds) != c["WORLD_COUNT"]:
        fail("\"worlds\" has one entry per world of the levels (%d): each world's page title, entry box and "
             "boss icon, and the rule that opens it" % c["WORLD_COUNT"])
    pages = world_pages(data)
    for w, e in enumerate(worlds):
        missing = [k for k in WORLD_KEYS if k not in e and not (k == "boss_icon" and pages[w]["boss"] is None)]
        if missing:
            fail("world %d has no %s" % (w, ", ".join('"%s"' % k for k in missing)))
        unknown = [k for k in e if k not in WORLD_KEYS + ["opened_by"]]
        if unknown:
            fail("world %d: unknown %s" % (w, ", ".join('"%s"' % k for k in unknown)))
        for k in WORLD_KEYS:
            if k not in e:
                continue
            if not isinstance(e[k], int) or not 0 <= e[k] < WORLD_KEY_LIMITS[k]:
                fail("world %d: \"%s\" is 0-%d" % (w, k, WORLD_KEY_LIMITS[k] - 1))
        rule = e.get("opened_by")
        if w == 0:
            if rule is not None:
                fail("world 0 is open from the start: it has no \"opened_by\"")
        elif rule == {"levels_cleared": True}:
            pass
        elif not (isinstance(rule, dict) and list(rule) == ["power"] and rule["power"] in WORLD_OPEN_POWERS):
            fail("world %d: \"opened_by\" is {\"power\": one of %s} or {\"levels_cleared\": true}"
                 % (w, ", ".join(WORLD_OPEN_POWERS)))
    icons = LEVEL_ICONS_ROM_STEPS + len(read_json(LEVEL_ICONS_BANK).get("new_steps", {}).get("0", []))
    if c["LEVEL_NUMBERED_COUNT"] > icons:
        fail("%d numbered levels, but %s's animation 0 has %d number icons: levels %d-%d need new frames "
             "(docs/graphics.md, \"New frames\")" % (c["LEVEL_NUMBERED_COUNT"], LEVEL_ICONS_BANK.parent, icons,
                                                     icons + 1, c["LEVEL_NUMBERED_COUNT"]))
    return worlds


# A partial page (#966): a world with fewer than LEVELS_PER_WORLD numbered
# levels, or no boss. The ROM's pages are all full, and the code finds a
# page's levels by arithmetic: `world * LEVELS_PER_WORLD + index`, the boss
# `LEVEL_FIRST_BOSS + world`, the boss entry once LEVELS_PER_WORLD levels
# are cleared, the two ROM layouts. level_select_worlds.h spells each of
# those as a macro (LEVEL_SELECT_WORLD_FIRST, ...): with full pages only,
# the ROM's arithmetic, so the code is the ROM's; with a partial page, a
# lookup in the per-world tables LEVEL_SELECT_PAGE_TABLES defines
# (level_select_layout.cpp), whose layouts put the page's entries on the
# ROM's arc. No `#if`: the generator writes one or the other.
PAGE_ARC_X, PAGE_ARC_Y, PAGE_ARC_STEP = 120, 136, 50   # the arc's middle and an entry's spacing
PAGE_ARC_DROP, PAGE_ARC_HALF = 16, 90                  # y 16 higher 90 px off the middle (the ROM's ends)
LEVEL_SELECT_NO_BOSS = 0xFF                             # gLevelSelectWorldBosses of a world with none


def world_pages(data):
    """Per world: its first numbered level's id, its numbered levels' count
    and its boss's id (None for none)."""
    levels = data["levels"]
    pages = []
    for w in range(level_counts(data)["WORLD_COUNT"]):
        ids = [i for i, lv in enumerate(levels) if not lv.get("boss") and lv["world"] == w]
        boss = [i for i, lv in enumerate(levels) if lv.get("boss") and lv.get("world") == w]
        pages.append({"first": ids[0], "count": len(ids), "boss": boss[0] if boss else None})
    return pages


def pages_full(data):
    """Whether every page is full (LEVELS_PER_WORLD levels and a boss), as
    the ROM's: the code then finds the pages' levels by its arithmetic."""
    return all(p["count"] == PAGE_LEVELS and p["boss"] is not None for p in world_pages(data))


def page_layout(k):
    """A page's entry positions on the menu art for `k` entries, k < 5 (5
    and 6 are the ROM's tables): evenly spaced on the ROM's arc."""
    out = []
    for i in range(k):
        dx = PAGE_ARC_STEP * (2 * i - (k - 1)) // 2
        out.append((PAGE_ARC_X + dx, PAGE_ARC_Y - (PAGE_ARC_DROP * dx * dx + PAGE_ARC_HALF ** 2 // 2)
                    // PAGE_ARC_HALF ** 2))
    return out


def layout_symbol(k):
    """The positions table for `k` entries shown."""
    return {PAGE_LEVELS: "gLevelSelectEntryPositions",
            PAGE_LEVELS + 1: "gLevelSelectEntryPositionsAllCleared"}.get(k, "gLevelSelectEntryPositions%d" % k)


def level_select_pages_h(data):
    """level_select_worlds.h's page macros: the ROM's arithmetic with full
    pages, else lookups in the tables of LEVEL_SELECT_PAGE_TABLES."""
    c = level_counts(data)
    head = ["", "/*",
            " * A page's levels (docs/levels.md \"Partial pages\"): world `world`'s first numbered",
            " * level, its count, its boss, whether the boss entry shows with `cleared` levels",
            " * cleared, the world and the entry of a level, and the entries' positions before",
            " * and after the boss entry shows.%s" % (" Every page is full, so these are the ROM's"
                                                     " arithmetic." if pages_full(data) else ""),
            " */"]
    if pages_full(data):
        return head + [
            "#define LEVEL_SELECT_WORLD_FIRST(world) ((world) * LEVELS_PER_WORLD)",
            "#define LEVEL_SELECT_WORLD_LEVELS(world) LEVELS_PER_WORLD",
            "#define LEVEL_SELECT_WORLD_BOSS(world) ((world) + LEVEL_FIRST_BOSS)",
            "#define LEVEL_SELECT_BOSS_SHOWN(world, cleared) ((cleared) == LEVELS_PER_WORLD)",
            "#define LEVEL_SELECT_WORLD_OF_LEVEL(level) __divsi3(level, LEVELS_PER_WORLD)",
            "#define LEVEL_SELECT_INDEX_OF_LEVEL(level, world) __modsi3(level, LEVELS_PER_WORLD)",
            "#define LEVEL_SELECT_WORLD_OF_BOSS(level) ((level) - LEVEL_FIRST_BOSS)",
            "#define LEVEL_SELECT_POSITIONS(world) gLevelSelectEntryPositions",
            "#define LEVEL_SELECT_POSITIONS_ALL_CLEARED(world) gLevelSelectEntryPositionsAllCleared",
            "", "/* No tables: the pages are the ROM's. */",
            "#define LEVEL_SELECT_PAGE_TABLES"]
    pages = world_pages(data)
    grid = c["LEVEL_COUNT"] - sum(1 for lv in data["levels"] if lv.get("boss") and lv.get("world") is None)
    level_worlds = [lv["world"] for lv in data["levels"][:grid]]
    shown = sorted({p["count"] for p in pages} | {p["count"] + 1 for p in pages if p["boss"] is not None})
    new_layouts = [k for k in shown if k < PAGE_LEVELS]

    def u8s(values):
        return ", ".join("%d" % v for v in values)

    tables = ["const u8 gLevelSelectWorldFirstLevels[WORLD_COUNT] = { %s };" % u8s(p["first"] for p in pages),
              "const u8 gLevelSelectWorldLevelCounts[WORLD_COUNT] = { %s };" % u8s(p["count"] for p in pages),
              "const u8 gLevelSelectWorldBosses[WORLD_COUNT] = { %s };"
              % ", ".join("%d" % p["boss"] if p["boss"] is not None else "LEVEL_SELECT_NO_BOSS" for p in pages),
              "const u8 gLevelSelectLevelWorlds[LEVEL_SELECT_GRID_LEVELS] = { %s };" % u8s(level_worlds)]
    for k in new_layouts:
        pos = ["{ DISPLAY_ART_X + %d, DISPLAY_ART_Y + %d }" % xy for xy in page_layout(k)]
        pos += ["{ 0, 0 }"] * (c["LEVEL_SELECT_ENTRIES"] - k)
        tables.append("const struct vec2 %s[LEVEL_SELECT_ENTRIES] = { %s };" % (layout_symbol(k), ", ".join(pos)))
    tables.append("const struct vec2 *const gLevelSelectWorldEntryPositions[WORLD_COUNT] = { %s };"
                  % ", ".join(layout_symbol(p["count"]) for p in pages))
    tables.append("const struct vec2 *const gLevelSelectWorldEntryPositionsAllCleared[WORLD_COUNT] = { %s };"
                  % ", ".join(layout_symbol(p["count"] + (p["boss"] is not None)) for p in pages))
    decls = ["extern " + t.split(" = ")[0] + ";" for t in tables]
    width = max(len(t) for t in tables)
    return head + [
        "#define LEVEL_SELECT_WORLD_FIRST(world) gLevelSelectWorldFirstLevels[world]",
        "#define LEVEL_SELECT_WORLD_LEVELS(world) gLevelSelectWorldLevelCounts[world]",
        "#define LEVEL_SELECT_WORLD_BOSS(world) gLevelSelectWorldBosses[world]",
        "#define LEVEL_SELECT_BOSS_SHOWN(world, cleared) \\",
        "    ((cleared) == gLevelSelectWorldLevelCounts[world] && gLevelSelectWorldBosses[world] != LEVEL_SELECT_NO_BOSS)",
        "#define LEVEL_SELECT_WORLD_OF_LEVEL(level) gLevelSelectLevelWorlds[level]",
        "#define LEVEL_SELECT_INDEX_OF_LEVEL(level, world) ((level) - gLevelSelectWorldFirstLevels[world])",
        "#define LEVEL_SELECT_WORLD_OF_BOSS(level) gLevelSelectLevelWorlds[level]",
        "#define LEVEL_SELECT_POSITIONS(world) gLevelSelectWorldEntryPositions[world]",
        "#define LEVEL_SELECT_POSITIONS_ALL_CLEARED(world) gLevelSelectWorldEntryPositionsAllCleared[world]",
        "", "#define LEVEL_SELECT_NO_BOSS 0x%02X /* gLevelSelectWorldBosses: the world has none */"
        % LEVEL_SELECT_NO_BOSS,
        "#define LEVEL_SELECT_GRID_LEVELS %d /* the levels on the grid: the numbered levels and the bosses */"
        % grid,
        "", "/* The page tables (level_select_layout.cpp defines them): per world its first",
        " * level, its count, its boss, the world of each level on the grid, and the",
        " * entries' positions (the ROM's two layouts for 5 and 6 entries, the arc's",
        " * spacing for fewer). */"] + decls + [
        "", "#define LEVEL_SELECT_PAGE_TABLES \\"] + [
        "    " + (t.ljust(width) + " \\" if i + 1 < len(tables) else t) for i, t in enumerate(tables)]


def level_select_worlds_h(data):
    """level_select_worlds.h: the per-world tables' initializers and
    IsNextWorldOpen's cases (LEVEL_SELECT_NEXT_WORLD_CASES), one per world
    after the first, under the case of the world before it."""
    worlds = level_select_worlds(data)
    out = ["/* Generated by tools/levels.py from data/levels/levels.json. Do not edit. */",
           "#ifndef GUARD_LEVEL_SELECT_WORLDS_H", "#define GUARD_LEVEL_SELECT_WORLDS_H", "",
           "/*",
           " * The level select's worlds (levels.json's \"worlds\", docs/levels.md \"Level-select",
           " * pages\"): per world, the page title's animation (bank 47, gLevelSelectWorldAnims),",
           " * the entries' box (gLevelSelectWorldEntryBoxAnims) and the boss icon's animation",
           " * (bank 51, gLevelSelectEntryWorldAnims), src/game/ui/data/level_select_layout.cpp and",
           " * level_select_anims.cpp; and LevelSelect::IsNextWorldOpen's cases",
           " * (level_select_pages.cpp): on page `world`, whether page `world + 1` is open.",
           " */", ""]
    for name, k in (("LEVEL_SELECT_WORLD_ANIMS", "page_title"),
                    ("LEVEL_SELECT_WORLD_ENTRY_BOX_ANIMS", "entry_box"),
                    ("LEVEL_SELECT_ENTRY_WORLD_ANIMS", "boss_icon")):
        out.append("#define %s %s" % (name, ", ".join("%d" % e.get(k, 0) for e in worlds)))
    lines = []
    for w, e in enumerate(worlds[1:], 1):
        rule = e["opened_by"]
        lines.append("case %d:" % (w - 1))
        if "power" in rule:
            lines.append("    r = TEST_BIT_SHR(save->flags, PROGRESS_BIT_%s);" % rule["power"])
        else:
            lines.append("    r = IsWorldCleared(%d);" % (w - 1))
        lines.append("    break;")
    width = max(len(s) for s in lines) if lines else 0
    out += ["", "#define LEVEL_SELECT_NEXT_WORLD_CASES%s" % (" \\" if lines else "")]
    out += ["    " + (s.ljust(width) + " \\" if k + 1 < len(lines) else s) for k, s in enumerate(lines)]
    out += level_select_pages_h(data)
    out += ["", "#endif /* GUARD_LEVEL_SELECT_WORLDS_H */", ""]
    return "\n".join(out)


def worlds_from_rom(rom, kept):
    """extract: levels.json's "worlds". The rules that open them are code
    (IsNextWorldOpen) and are kept; the page title, box and boss icon come
    from the ROM's tables."""
    worlds = []
    for w, k in enumerate(kept["worlds"]):
        e = {"page_title": rom.w(LEVEL_SELECT_WORLD_ANIMS + 4 * w),
             "entry_box": rom.w(LEVEL_SELECT_WORLD_ENTRY_BOX_ANIMS + 4 * w),
             "boss_icon": rom.w(LEVEL_SELECT_ENTRY_WORLD_ANIMS + 4 * w)}
        if "opened_by" in k:
            e["opened_by"] = k["opened_by"]
        worlds.append(e)
    return worlds


def tileset_order_lists(sets, levels):
    """For extract: each tile set's room order, the shortest list of rooms
    that, put before the set's other rooms (in levels.json order), gives
    the ROM's tile order. `sets`: {symbol: (the ROM's tile blocks, {room:
    [the blocks of its layers using the set, in chunk order]})}, the
    rooms in levels.json order. Every tile set of the ROM is a run of whole
    rooms in some order, which this finds first."""

    def simulate(seq, blocks):
        seen, out = set(flips((0,) * 8)), [(0,) * 8]
        for r in seq:
            for b in blocks[r]:
                if b not in seen:
                    out.append(b)
                    seen.update(flips(b))
        return out

    out = {}
    for symbol, (want, blocks) in sets.items():
        default = list(blocks)
        # the rooms in the ROM's order: each one's new tiles are the ROM's next
        seen, pos, order, left = set(flips((0,) * 8)), 1, [], list(default)
        while pos < len(want):
            for r in left:
                new = []
                s = set(seen)
                for b in blocks[r]:
                    if b not in s:
                        new.append(b)
                        s.update(flips(b))
                if new and want[pos:pos + len(new)] == new:
                    order.append(r)
                    left.remove(r)
                    seen, pos = s, pos + len(new)
                    break
            else:
                sys.exit("%s isn't a run of whole rooms in any order" % symbol)
        for k in range(len(order) + 1):
            seq = order[:k] + [r for r in default if r not in order[:k]]
            if simulate(seq, blocks) == want:
                if k:
                    out[symbol] = order[:k]
                break
    return out


def write_tileset_order(lists):
    lines = ["{"]
    items = list(lists.items())
    for i, (symbol, rooms) in enumerate(items):
        lines.append('  "%s": %s%s' % (symbol, json.dumps(rooms), "," if i + 1 < len(items) else ""))
    lines.append("}")
    TILESET_ORDER_JSON.write_text("\n".join(lines) + "\n")


def render(room_dir, lname, out_path, build_dir=None):
    """A layer's picture: the collision image, or a BG layer's picture drawn
    again from the build's cells and generated tile set (with --build)."""
    room = load_room(room_dir)
    layer = room["layers"][lname]
    if lname == "collision":
        # the collision image itself: the legend's colours, 8x8 pixels per cell
        cells = load_map(room_dir, lname, layer)
        collision_bmp.write_image(out_path, layer["size_chunks"][0] * CHUNK_W, cells)
        return
    if build_dir is None:
        sys.exit("render: a BG layer is drawn from the build's cells and tile set: give --build")
    cells = load_map(room_dir, lname, layer, build_dir)
    bpp = 8 if lname == "bg0" else 4
    data = (Path(build_dir) / "tilesets" / (layer["tileset"] + ".bin")).read_bytes()
    tiles = LayerTiles(layer["tileset"], lname == "bg0", tile_blocks(data, bpp))
    write_layer_png(out_path, layer, tiles, cells, room_palette(room_dir, room))


# ---------------------------------------------------------------------------
# Extraction from the ROM

class Rom:
    def __init__(self, path):
        self.data = Path(path).read_bytes()

    def w(self, a):
        return struct.unpack_from("<I", self.data, a - ROM_BASE)[0]

    def h(self, a):
        return struct.unpack_from("<H", self.data, a - ROM_BASE)[0]

    def b(self, a):
        return self.data[a - ROM_BASE]

    def bytes(self, a, n):
        return self.data[a - ROM_BASE:a - ROM_BASE + n]


def lz77_decompress(data):
    assert data[0] == 0x10
    size = int.from_bytes(data[1:4], "little")
    out = bytearray()
    pos = 4
    while len(out) < size:
        flags = data[pos]
        pos += 1
        for bit in range(7, -1, -1):
            if len(out) >= size:
                break
            if flags >> bit & 1:
                b1, b2 = data[pos], data[pos + 1]
                pos += 2
                n = (b1 >> 4) + 3
                disp = ((b1 & 0xF) << 8 | b2) + 1
                for _ in range(n):
                    out.append(out[-disp])
            else:
                out.append(data[pos])
                pos += 1
    return bytes(out)


def in_regions(a):
    return any(lo <= a < hi for _, lo, hi in REGIONS)


def sym(a):
    return "gStaticData_%08X" % a

# The level tile sets' symbol names (see docs/levels.md).
TILESET_SYMBOLS = {
    0x0817E7AC: "gJungleBg0Tiles",
    0x081E6330: "gSnowBg0Tiles",
    0x08200DF4: "gUnderwaterBg0Tiles",
    0x08270F08: "gSewerBg0Tiles",
    0x08299DCC: "gSpaceBg0Tiles",
    0x0863D4C0: "gSnowBgTiles",
    0x08640EF8: "gJungleBgTiles",
    0x08644A88: "gTempleBgTiles",
    0x08649418: "gSewerBgTiles",
    0x0864BB5C: "gSpaceBgTiles",
    0x0864D7E8: "gUnderwaterBgTiles",
}


# The room objects' symbol names: gRoomNN + what the object is.
LAYER_SYMBOLS = {"bg0": "Bg0", "bg1": "Bg1", "bg2": "Bg2", "bg3": "Bg3", "collision": "Collision"}


def room_records(rom):
    """The room records (struct level_room) of the level table region from
    ROM_TABLE, in table order: their addresses."""
    out = []
    for a in range(ROOM_TABLE, ROOM_TABLE + ROOM_TABLE_SIZE, 4):
        if in_regions(rom.w(a)) and in_regions(rom.w(a + 4)) and not in_regions(rom.w(a - 4)):
            out.append(a)
    return out


def find_rooms(rom):
    """The room records' (palette, level_desc), in table order."""
    return [(rom.w(a), rom.w(a + 4)) for a in room_records(rom)]


def slug(name):
    return re.sub(r"[^a-z0-9]+", "_", name.lower()).strip("_")


def room_folders(rom, levels):
    """Room record address -> its folder, `<level>/<slot>`: the level is the
    one whose gLevelRoomLists entry lists the room (named after levels.json's
    name), the slot `main` for a level's one played room, `part1`, `part2`
    for the played rooms around a stage (Frostbite Cavern, Snow Crash and
    Snow Job: rooms[0] and rooms[2], rooms[1] being the polar ride), `bonus`
    for extra1 (EnterBonusRoom) and `gem_path` for extra2
    (EnterGemPathRoom)."""
    out = {}
    for i, lv in enumerate(levels):
        a = LEVEL_ROOM_LISTS + 16 * i
        count, played, extra1, extra2 = rom.w(a), rom.w(a + 4), rom.w(a + 8), rom.w(a + 12)
        rooms = [r for r in (rom.w(played + 4 * k) for k in range(count)) if rom.w(r + 4)]
        slots = {r: "main" for r in rooms} if len(rooms) == 1 else \
            {r: "part%d" % (k + 1) for k, r in enumerate(rooms)}
        for r, slot in ((extra1, "bonus"), (extra2, "gem_path")):
            if r:
                slots[r] = slot
        for r, slot in slots.items():
            assert r not in out, "room record %#x is in two levels" % r
            out[r] = "%s/%s" % (slug(lv["name"]), slot)
    return out


def room_record(rom, a, symbol):
    """room.json's "record": the room's struct level_room at `a`."""
    kind, unk_0c = rom.w(a + 8), rom.w(a + 12)
    assert kind < 3 and unk_0c == 0, hex(a)
    return {"symbol": symbol, "kind": ROOM_KINDS[kind],
            "blend": {"effect": rom.h(a + 0x10), "eva": rom.b(a + 0x12), "evb": rom.b(a + 0x13)}}


# The level fields levels.py extract keeps from the hand-written levels.json
# (the names); it writes the others from the ROM ("boss" must agree). Of
# the "themes" and "palette_cycles" it keeps what the code says
# (themes_from_rom).
LEVEL_KEYS_KEPT = ["constant", "name", "world"]


def level_table_from_rom(rom, level_list, folders):
    """levels.json's "levels" (the kept names, the rest from gLevelTable and
    gLevelRoomLists) and "table" (the ROM's played-room arrays, in ROM
    order)."""
    cat_names = {n: c["constant"][len("CATEGORY_"):] for n, c in enumerate(read_json(LEVELS_JSON)["categories"])}

    def entry(r):
        if r in folders:
            return folders[r]
        assert rom.w(r) == 0 and rom.w(r + 4) == 0 and rom.w(r + 8) == ROOM_KINDS.index("CATEGORY") \
            and rom.w(r + 12) == 0 and rom.h(r + 0x12) == 0, hex(r)
        n = rom.h(r + 0x10)
        assert r == stage_addr.setdefault(n, r), "two records of stage %d" % n
        return {"stage": cat_names[n]}

    stage_addr = {}
    out, arrays = [], []
    for i, kept in enumerate(level_list):
        a = LEVEL_TABLE + LEVEL_INFO_SIZE * i
        lst = LEVEL_ROOM_LISTS + 16 * i
        assert rom.w(a + 0x20) == lst and rom.bytes(a + 0x1D, 3) == b"\0\0\0", hex(a)
        is_boss = rom.b(a + 0x1C)
        assert is_boss in (0, 1) and bool(is_boss) == bool(kept.get("boss")), \
            "%s: \"boss\" isn't the ROM's isBoss" % kept["constant"]
        count, played, extra1, extra2 = rom.w(lst), rom.w(lst + 4), rom.w(lst + 8), rom.w(lst + 12)
        rooms = [entry(rom.w(played + 4 * k)) for k in range(count)]
        arrays.append((played, i, {"level": kept["constant"], "rooms": rooms}))
        lv = {k: kept[k] for k in LEVEL_KEYS_KEPT if k in kept}
        if is_boss:
            lv["boss"] = True
        lv.update({"name_text": struct.unpack_from("<i", rom.data, a - ROM_BASE)[0], "theme": rom.w(a + 4),
                   "times": [rom.w(a + 8 + 4 * k) for k in range(3)],
                   "mask_assist_deaths": struct.unpack_from("<i", rom.data, a + 0x14 - ROM_BASE)[0],
                   "crate_assist_deaths": struct.unpack_from("<i", rom.data, a + 0x18 - ROM_BASE)[0],
                   "rooms": rooms})
        for key, r in (("bonus", extra1), ("gem_path", extra2)):
            if r:
                lv[key] = folders[r]
        out.append(lv)
    arrays.sort(key=lambda x: x[0])
    return out, {"lists": [x for _, _, x in arrays]}, arrays, stage_addr


def write_levels_json(levels):
    """levels.json: two-space indentation, a level's or a list's values on
    one line each, and the long room lists one room per line."""
    def dump(v):
        return json.dumps(v, ensure_ascii=False)

    def block(v, ind, inline):
        pad = "  " * (ind + 1)
        if isinstance(v, dict):
            items = ['%s"%s": %s' % (pad, k, inline(k, x, ind + 1)) for k, x in v.items()]
            return "{\n" + ",\n".join(items) + "\n" + "  " * ind + "}"
        return "[\n" + ",\n".join(pad + inline(None, x, ind + 1) for x in v) + "\n" + "  " * ind + "]"

    def top(k, v, ind):
        if k in ("rooms", "regions", "assets"):
            # as json.dumps(indent=2) writes them
            return json.dumps(v, indent=2).replace("\n", "\n" + "  " * ind)
        if k == "table":
            return block(v, ind, lambda k2, x, i2: block(x, i2, lambda _, y, __: dump(y)) if k2 == "lists"
                         else dump(x))
        if k == "levels":
            return block(v, ind, lambda _, lv, i2: block(lv, i2, lambda __, y, ___: dump(y)))
        if k in ("themes", "worlds", "categories"):
            return block(v, ind, lambda _, t, __: dump(t))
        if k == "palette_cycles":
            return block(v, ind, lambda _, c, i2: block(c, i2, lambda k2, y, i3: block(y, i3, lambda _, z, __: dump(z))
                                                         if k2 == "lists" else dump(y)))
        return dump(v)

    LEVELS_JSON.write_text(block(levels, 0, top) + "\n")


def extract(rom_path):
    rom = Rom(rom_path)
    records = room_records(rom)
    rooms = find_rooms(rom)
    assert len(rooms) == 41, len(rooms)
    # the level names aren't extracted: the hand-written ones name the
    # folders (LEVEL_KEYS_KEPT)
    kept = read_json(LEVELS_JSON)
    level_list = kept["levels"]
    folders = room_folders(rom, level_list)
    assert sorted(folders) == sorted(records), "rooms in no level's room list"
    LEVELS_DIR.mkdir(parents=True, exist_ok=True)
    pieces = []  # (addr, room name) for the ROM order of each region
    assets = []  # (asset addr, room name) for the ROM order of the assets
    type_names = entity_type_names()
    names = []
    rom_tiles = {}  # tile set address -> LayerTiles of the ROM's tiles
    decoded = {}  # (room, layer) -> its cells
    set_blocks = {}  # tile set symbol -> {room: its blocks in chunk order}
    for idx, (pal, desc) in enumerate(rooms):
        name = folder = folders[records[idx]]
        pre = "gRoom%02d" % idx
        names.append(folder)
        room_dir = LEVELS_DIR / folder
        room_dir.mkdir(parents=True, exist_ok=True)
        L = [rom.w(desc + 4 * k) for k in range(9)]
        assert rom.bytes(desc + 0x19, 3) == b"\0\0\0" and not any(rom.bytes(desc + 0x24, 12))
        packed = rom.b(desc + 0x18)
        asset_addr = L[5]
        assets.append((asset_addr, folder))
        if packed:
            # size from the LZ77 header; the stream length doesn't matter here
            asset = lz77_decompress(rom.bytes(asset_addr, 0x20000))
        else:
            asset = rom.bytes(asset_addr, RAW_ASSETS[asset_addr])
        ent_list = L[7]
        palette = [rom.h(pal + 2 * i) for i in range(256)]
        room = {
            "asset": {"symbol": pre + "Asset", "packed": bool(packed)},
            "record": room_record(rom, records[idx], "gLevelRoom%02d" % idx),
            "layers": {},
            "entities": [],
            "params": [],
            "links": None,
            "symbols": {
                "desc": pre + "Desc", "palette": pre + "Palette", "entities": pre + "EntityList",
                "groups": pre + "EntityGroups", "param_offsets": pre + "ParamOffsets",
                "params": pre + "Params", "type_counts": pre + "TypeCounts",
            },
        }
        pieces.append((desc, folder))
        # layers
        first_ld = None
        slots = [L[0], L[1], L[2], L[3], L[4]]
        sec_starts = []
        for lname, p in zip(LAYERS, slots):
            if not p:
                continue
            if first_ld is None:
                first_ld = p
            assert p == first_ld + 0x20 * len(room["layers"]), "layer descs not consecutive"
            grid_addr, off, tiles = rom.w(p), rom.w(p + 4), rom.w(p + 8)
            cw, ch = rom.h(p + 0x16), rom.h(p + 0x18)
            grid = [rom.h(grid_addr + 2 * i) for i in range(cw * ch)]
            sec_starts.append(off)
            # decode the section's chunks
            nchunks = max(grid) + 1
            table = struct.unpack_from("<%dH" % nchunks, asset, off)
            chunks = []
            for k in range(nchunks):
                cells, _ = decode_chunk(asset, off + 4 * table[k])
                chunks.append(cells)
            width = cw * CHUNK_W
            cells = [0] * (width * ch * CHUNK_H)
            for gy in range(ch):
                for gx in range(cw):
                    c = chunks[grid[gy * cw + gx]]
                    for y in range(CHUNK_H):
                        cells[(gy * CHUNK_H + y) * width + gx * CHUNK_W:
                              (gy * CHUNK_H + y) * width + gx * CHUNK_W + CHUNK_W] = c[y * CHUNK_W:(y + 1) * CHUNK_W]
            re_chunks, re_grid = split_chunks(cells, cw, ch)
            assert re_grid == grid and [list(c) for c in re_chunks] == chunks, name + " " + lname
            fname = "%s.png" % lname
            layer = {
                "map": fname,
                "size_chunks": [cw, ch],
                "size_tiles": [rom.h(p + 0x1A), rom.h(p + 0x1C)],
                "tileset": TILESET_SYMBOLS.get(tiles, sym(tiles)) if tiles else None,
                "scale": [struct.unpack_from("<i", rom.data, p + 0xC - ROM_BASE)[0],
                          struct.unpack_from("<i", rom.data, p + 0x10 - ROM_BASE)[0]],
                "cnt": rom.h(p + 0x14),
                "unk_1E": rom.h(p + 0x1E),
                "grid_symbol": pre + LAYER_SYMBOLS[lname] + "Grid",
            }
            room["layers"][lname] = layer
            if lname == "collision":
                collision_bmp.write_image(room_dir / fname, width, cells)
            else:
                if tiles not in rom_tiles:
                    if lname == "bg0":  # a tag-0x00 raw tagged asset: {size << 8}, then 8bpp tiles
                        data = rom.bytes(tiles + 4, rom.w(tiles) >> 8)
                    else:  # LZ77, 4bpp tiles
                        data = lz77_decompress(rom.bytes(tiles, 0x10000))
                    rom_tiles[tiles] = LayerTiles(layer["tileset"], lname == "bg0",
                                                  tile_blocks(data, 8 if lname == "bg0" else 4))
                lt = rom_tiles[tiles]
                write_layer_png(room_dir / fname, layer, lt, cells, palette)
                decoded[(folder, lname)] = cells
                mask = 0xFFFF if lname == "bg0" else 0x0FFF
                set_blocks.setdefault(layer["tileset"], {}).setdefault(folder, []).extend(
                    lt.block(cells[k] & mask) for k in chunk_order(cw, ch))
        room["symbols"]["layers"] = pre + "Layers"
        assert sec_starts == sorted(sec_starts) and sec_starts[0] == 0
        # the pad bytes the encoder model can't predict (encoder heap garbage)
        for i, (lname, layer) in enumerate(room_layers(room)):
            off = sec_starts[i]
            end = sec_starts[i + 1] if i + 1 < len(sec_starts) else len(asset)
            cells = decoded[(folder, lname)] if lname != "collision" else load_map(room_dir, lname, layer)
            chunks, _ = split_chunks(cells, *layer["size_chunks"])
            want = asset[off:end]
            got = encode_section(chunks)
            if got != want:
                overrides = {}
                table = struct.unpack_from("<%dH" % len(chunks), want, 0)
                for k, ch in enumerate(chunks):
                    stream, pad = encode_chunk(list(ch))
                    o = 4 * table[k]
                    assert want[o:o + len(stream)] == stream, (name, lname, k)
                    real = want[o + len(stream):o + len(stream) + len(pad)]
                    if real != pad:
                        overrides[str(k)] = {"crc": chunk_crc(ch), "pad": real.hex()}
                layer["pad_overrides"] = overrides
                assert encode_section(chunks, overrides) == want, (name, lname)
        # entities, in id order (last column first)
        ngroups, groups = rom.h(ent_list + 2), rom.w(ent_list + 4)
        for g in range(ngroups - 1, -1, -1):
            count, items = rom.h(groups + 8 * g + 2), rom.w(groups + 8 * g + 4)
            for m in range(count):
                t, x, y, prm = struct.unpack_from("<4H", rom.data, items + 8 * m - ROM_BASE)
                if t < len(type_names) and type_names[t]:
                    t = type_names[t]
                room["entities"].append({"type": t, "x": x, "y": y, "param": prm})
        room["symbols"]["items"] = pre + "Entities"
        # parameter records
        params, offs_addr = rom.w(ent_list + 12), rom.w(ent_list + 8)
        size = offs_addr - params
        # the offsets run up to the first chunk grid
        grids_start = min(rom.w(p) for p in slots if p)
        offs = [rom.h(offs_addr + 2 * i) for i in range((grids_start - offs_addr) // 2)]
        assert offs[0] == 0 and offs == sorted(set(offs)) and offs[-1] < size, name
        for i, o in enumerate(offs):
            end = offs[i + 1] if i + 1 < len(offs) else size
            assert (end - o) % 4 == 0
            room["params"].append([rom.w(params + o + 4 * k) for k in range(0, (end - o) // 4)])
        # links
        if L[8]:
            n = rom.w(L[8])
            room["links"] = [[rom.w(L[8] + 4 + 8 * k), rom.w(L[8] + 8 + 8 * k)] for k in range(n)]
            room["symbols"]["links"] = pre + "Links"
        write_room_json(room_dir / "room.json", room)
        pieces.append((params, folder))
    # regions, rooms in ROM order
    regions = []
    for rname, lo, hi in REGIONS:
        order = []
        for a, n in sorted(pieces):
            if lo <= a < hi and n not in order:
                order.append(n)
        regions.append({"name": rname, "rooms": order})
    aname, lo, hi = ASSET_REGION
    assert all(lo <= a < hi for a, _ in assets)
    level_info, table, arrays, stages = level_table_from_rom(rom, level_list, folders)
    levels = {"rooms": names, "regions": regions,
              "assets": {"name": aname, "rooms": [n for _, n in sorted(assets)]}, "table": table,
              "levels": level_info, "worlds": worlds_from_rom(rom, kept)}
    levels["themes"], levels["palette_cycles"] = themes_from_rom(rom, kept)
    levels["categories"] = categories_from_rom(rom, kept)
    # the derived counts (constants/levels.h, themes.h) are the ROM's
    check_rom_counts(levels)
    write_levels_json(levels)
    # the save version's CRC (constants/save.h) is the ROM's
    assert level_data_crc(levels) == ROM_LEVEL_DATA_CRC, \
        "the level data's CRC-32 is %#010x, not ROM_LEVEL_DATA_CRC" % level_data_crc(levels)
    # the generator's layout must be the ROM's: its objects back to back
    # in ROM order
    addr = {"gLevelRoom%02d" % k: a for k, a in enumerate(records)}
    addr.update({stage_symbol(n): a for n, a in stages.items()})
    addr.update({room_list_symbol(i): a for a, i, _ in arrays})
    sizes = {room_list_symbol(i): 4 * len(t["rooms"]) for _, i, t in arrays}
    order = re.findall(r"^extern const struct level_room (?:\*const )?(\w+)(?:\[\d+\])? = ",
                       LevelTable(levels).records_c(), re.M)
    assert sorted(order) == sorted(addr), "the level table's objects aren't the ROM's"
    for s, s2 in zip(order, order[1:]):
        assert addr[s] + sizes.get(s, LEVEL_ROOM_SIZE) == addr[s2], "%s, %s: not in ROM order" % (s, s2)
    # The tile sets aren't extracted: the build makes them from the layer
    # pictures. What their order needs beyond levels.json goes into
    # tileset_order.json; then the generator must give back the ROM's tiles
    # and every layer's cells.
    by_symbol = {lt.symbol: lt for lt in rom_tiles.values()}
    sets = {}
    for symbol in TILESET_SYMBOLS.values():
        users = set_blocks[symbol]
        sets[symbol] = ([f[0] for f in by_symbol[symbol].flipped], {r: users[r] for r in names if r in users})
    write_tileset_order(tileset_order_lists(sets, levels))
    order = load_tileset_order()
    users = tileset_users(levels)
    for symbol, lt in by_symbol.items():
        tiles, _, maps = generate_tileset(symbol, levels, users, order)
        assert tiles == [f[0] for f in lt.flipped], "%s: the generated tile set isn't the ROM's" % symbol
        for key, cells in maps.items():
            assert cells == decoded[key], "%s %s: the generated cells aren't the ROM's" % key
    return names


def chunk_order(w, h):
    """A layer's cell indices (row-major) in chunk order (see "BG layer
    pictures")."""
    W = w * CHUNK_W
    return [(gy * CHUNK_H + cy) * W + gx * CHUNK_W + cx
            for gy in range(h) for gx in range(w) for cy in range(CHUNK_H) for cx in range(CHUNK_W)]


def write_room_json(path, room):
    """room.json with one entity/record/link per line."""
    def dump(v):
        return json.dumps(v, separators=(", ", ": "))

    lines = ["{"]
    keys = ["asset", "record", "symbols", "layers", "params", "links", "entities"]
    for i, k in enumerate(keys):
        comma = "," if i + 1 < len(keys) else ""
        v = room[k]
        if k == "layers":
            lines.append('  "layers": {')
            items = list(v.items())
            for j, (ln, lv) in enumerate(items):
                lines.append('    "%s": %s%s' % (ln, dump(lv), "," if j + 1 < len(items) else ""))
            lines.append("  }" + comma)
        elif k in ("params", "links", "entities") and v is not None:
            lines.append('  "%s": [' % k)
            for j, e in enumerate(v):
                lines.append("    " + dump(e) + ("," if j + 1 < len(v) else ""))
            lines.append("  ]" + comma)
        else:
            lines.append('  "%s": %s%s' % (k, dump(v), comma))
    lines.append("}")
    Path(path).write_text("\n".join(lines) + "\n")


# ---------------------------------------------------------------------------
# Constants headers (#655): the names of the entity types and crate kinds

def define_lines(rows):
    """[(name, value, comment)] -> aligned #define lines."""
    width = max(len(n) + len(v) for n, v, _ in rows)
    out = []
    for n, v, c in rows:
        line = "#define %s %s" % (n, v)
        if c:
            line += " " * (width - len(n) - len(v) + 1) + "// " + c
        out.append(line)
    return out


# The ROM's counts, which `extract` checks the rules of level_counts give
# with the extracted levels: the level table's 25 entries (gLevelTable to
# gLevelRoomLists), the level select's 4 pages of 5 levels and a boss
# (LevelSelect::items[6], `world <= 2`), the `i < 20` loops and the
# pause menu's totals (20 crystals, 28 gems, 20 relics) of
# GameProgress, GetCompletionPercent's divisor 72 and the 70 strings of
# each gUiText table (ui_text.cpp, text/). And the 11 level themes
# (gThemeMusicCues), of which ZoomBg's "no picture" 11 is one past the last, and
# the 10 level-select pictures (gLevelSelectPictures).
ROM_COUNTS = {
    "LEVEL_COUNT": 25, "WORLD_COUNT": 4, "LEVELS_PER_WORLD": 5, "LEVEL_NUMBERED_COUNT": 20,
    "LEVEL_LAST_NUMBERED": "LEVEL_FINAL_COUNTDOWN", "LEVEL_FIRST_BOSS": "LEVEL_DINGODILE",
    "LEVEL_LAST_ON_GRID": "LEVEL_NEO_CORTEX", "LEVEL_FINALE": "LEVEL_MEGA_MIX", "LEVEL_SELECT_ENTRIES": 6,
    "CRYSTAL_TOTAL": 20, "GEM_TOTAL": 28, "RELIC_TOTAL": 20, "COMPLETION_TOTAL": 72, "UI_TEXT_COUNT": 70,
    "THEME_COUNT": 11, "LEVEL_SELECT_PICTURE_COUNT": ROM_LEVEL_SELECT_PICTURE_COUNT,
}
# The UI text tables' strings in the ROM (ids 0-69): level names are 1-25.
UI_TEXT_ROM_COUNT = 70
# The coloured gems (PROGRESS_FLAG_*_GEM) and the powers, which the
# completion percentage counts: four each.
COLOURED_GEMS = 4
POWERS = 4
# The save record (SaveData, save_data.hpp; docs/levels.md, "The save"):
# four slots, each a GameProgress (4 bytes, 4 a level, then the extra
# progress flags) and 8 bytes more (the level and the two volumes), then a
# 12-byte trailer (the slots' empty flags, the two markers, the flag bytes
# and the checksum). The ROM's is the whole 4 Kbit EEPROM, 0x200 bytes,
# which holds 28 levels. Past that the build takes the 64 Kbit EEPROM and
# the record grows to the slots' size, in the EEPROM's 8-byte blocks, up
# to SAVE_64KBIT_MAX_SIZE: SaveData::Read and Write copy the record
# through a stack buffer, and main's stack reserve grows with it.
SAVE_SLOTS = 4
SAVE_SLOT_TAIL = 8
SAVE_TRAILER = 12
SAVE_4KBIT_SIZE = 0x200
SAVE_64KBIT_MAX_SIZE = 0x400
EEPROM_4KBIT, EEPROM_64KBIT = 4, 0x40  # EEPROMConfigure's chip codes


# A level-select page holds up to 5 numbered levels and a boss (the ROM's
# gLevelSelectEntryPositions, 5 levels and a hidden sixth slot, and
# gLevelSelectEntryPositionsAllCleared, 6): LEVELS_PER_WORLD, the most a
# world has.
PAGE_LEVELS = 5


def level_counts(data):
    """The counts the code takes from the levels (constants/levels.h). The
    levels are in the order the code's arithmetic needs: the numbered
    levels world by world, 1 to LEVELS_PER_WORLD in each, then the bosses,
    at most one per world, in world order (`"boss": true` with a "world"),
    then the levels off the level select's grid (`"boss": true`, no
    "world"), mega-mix first. Each count is a rule, not a field, so it
    follows the data. A world with fewer levels or no boss is a partial
    page (level_select_pages)."""
    levels = data["levels"]

    def fail(msg):
        sys.exit("%s: %s" % (LEVELS_JSON, msg))

    numbered = [lv for lv in levels if not lv.get("boss")]
    bosses = [lv for lv in levels if lv.get("boss") and lv.get("world") is not None]
    off_grid = [lv for lv in levels if lv.get("boss") and lv.get("world") is None]
    if levels != numbered + bosses + off_grid:
        fail("the levels go numbered levels first, then the bosses (\"boss\": true and a \"world\"), then the "
             "levels off the level select's grid (\"boss\": true, no \"world\"): the code finds them by id")
    if not numbered or not bosses or not off_grid:
        fail("a numbered level, a boss and a level off the grid (mega-mix) are needed")
    worlds = sorted({lv["world"] for lv in numbered})
    if worlds != list(range(len(worlds))):
        fail("the numbered levels' worlds are 0 to %d" % (len(worlds) - 1))
    per_world = [sum(1 for lv in numbered if lv["world"] == w) for w in worlds]
    if max(per_world) != PAGE_LEVELS or min(per_world) < 1:
        fail("a world has 1 to %d numbered levels, and the largest %d (the level select's page holds %d "
             "levels and a boss, gLevelSelectEntryPositions); they have %s" % (PAGE_LEVELS, PAGE_LEVELS,
                                                                             PAGE_LEVELS, per_world))
    if [lv["world"] for lv in numbered] != sorted(lv["world"] for lv in numbered):
        fail("the numbered levels go world by world")
    boss_worlds = [lv["world"] for lv in bosses]
    if boss_worlds != sorted(set(boss_worlds)) or not set(boss_worlds) <= set(worlds):
        fail("a world has one boss at most, the bosses in world order, each of a world of the numbered "
             "levels: their worlds are %s" % boss_worlds)
    gem_levels = numbered + off_grid[:1]  # what GameProgress::CountGems counts
    gem_paths = 0
    for lv in gem_levels:
        for r in [e for e in lv["rooms"] if isinstance(e, str)] + [lv[k] for k in ("bonus", "gem_path") if lv.get(k)]:
            room = read_json(LEVELS_DIR / r / "room.json")
            gem_paths += sum(1 for e in room["entities"] if e["type"] in ("GEM_PATH_GEM", 8))
    c = {
        "LEVEL_COUNT": len(levels),
        "WORLD_COUNT": len(worlds),
        "LEVELS_PER_WORLD": max(per_world),
        "LEVEL_NUMBERED_COUNT": len(numbered),
        "LEVEL_LAST_NUMBERED": numbered[-1]["constant"],
        "LEVEL_FIRST_BOSS": bosses[0]["constant"],
        "LEVEL_LAST_ON_GRID": bosses[-1]["constant"],
        "LEVEL_FINALE": off_grid[0]["constant"],
        "LEVEL_SELECT_ENTRIES": PAGE_LEVELS + 1,
        "CRYSTAL_TOTAL": len(numbered),
        # a crate gem per numbered level (bosses have none: RunRoom skips
        # CheckAllCratesBroken), the gem-path gems of the levels CountGems
        # counts, and the coloured gems
        "GEM_TOTAL": len(numbered) + gem_paths + COLOURED_GEMS,
        "RELIC_TOTAL": len(numbered),
        "UI_TEXT_COUNT": max([UI_TEXT_ROM_COUNT] + [lv["name_text"] + 1 for lv in levels]),
    }
    c["COMPLETION_TOTAL"] = c["CRYSTAL_TOTAL"] + c["GEM_TOTAL"] + c["RELIC_TOTAL"] + POWERS
    for k in ("CRYSTAL_TOTAL", "GEM_TOTAL", "RELIC_TOTAL"):
        if c[k] > 99:
            fail("%s is %d: the pause menu prints it in two digits (PauseMenu's u8 [3] buffers)" % (k, c[k]))
    c["PROGRESS_EXTRA_FLAG_BYTES"] = progress_extra_flag_bytes(data)
    save_layout(data)  # stops past what the 64 Kbit save holds
    return c


def extra_progress_flags(data):
    """levels.json's "extra_progress_flags": the progress flags past the
    ROM's 8 (GameProgress::flags), each a bit of GameProgress::extraFlags
    in list order. None in the ROM."""
    flags = data.get("extra_progress_flags", [])
    names = [f["name"] for f in flags]
    if len(set(names)) != len(names) or not all(re.fullmatch(r"[A-Z][A-Z0-9_]*", n) for n in names):
        sys.exit("%s: \"extra_progress_flags\" names are unique, in upper case: %s" % (LEVELS_JSON, names))
    return flags


def progress_extra_flag_bytes(data):
    """GameProgress::extraFlags' length: a word per 32 extra flags (the
    block stays a whole number of words, which MemCopy32 copies)."""
    return 4 * ((len(extra_progress_flags(data)) + 31) // 32)


def save_layout(data):
    """The save record's layout for the level data: (EEPROMConfigure's
    chip code, the record's size, a slot's size). The 4 Kbit EEPROM's
    0x200 bytes while the four slots fit (28 levels with no extra progress
    flags), else the 64 Kbit EEPROM and a record of the slots' size in
    8-byte blocks; stops past SAVE_64KBIT_MAX_SIZE."""
    levels = len(data["levels"])
    slot = 4 + 4 * levels + progress_extra_flag_bytes(data) + SAVE_SLOT_TAIL
    need = SAVE_SLOTS * slot + SAVE_TRAILER
    if need <= SAVE_4KBIT_SIZE:
        return EEPROM_4KBIT, SAVE_4KBIT_SIZE, slot
    size = (need + 7) & ~7
    if size > SAVE_64KBIT_MAX_SIZE:
        most = ((SAVE_64KBIT_MAX_SIZE - SAVE_TRAILER) // SAVE_SLOTS - SAVE_SLOT_TAIL - 4
                - progress_extra_flag_bytes(data)) // 4
        sys.exit("%s: %d levels: the save record would be %#x bytes, past the 64 Kbit save's %#x "
                 "(%d levels at most%s; docs/levels.md, \"The save\")"
                 % (LEVELS_JSON, levels, size, SAVE_64KBIT_MAX_SIZE, most,
                    " with these extra progress flags" if extra_progress_flags(data) else ""))
    return EEPROM_64KBIT, size, slot


def check_rom_counts(data):
    """extract: the counts' rules give the ROM's numbers with the ROM's levels."""
    c = level_counts(data)
    c["THEME_COUNT"] = len(level_themes(data))
    c["LEVEL_SELECT_PICTURE_COUNT"] = level_select_picture_count(data["themes"])
    for k, v in ROM_COUNTS.items():
        assert c[k] == v, "%s: the rule gives %s, the ROM has %s" % (k, c[k], v)
    assert c["PROGRESS_EXTRA_FLAG_BYTES"] == 0
    assert [w.get("opened_by") for w in level_select_worlds(data)] == ROM_WORLD_RULES, \
        "the worlds' rules aren't the ROM's"
    assert save_layout(data) == (EEPROM_4KBIT, SAVE_4KBIT_SIZE, 0x70), "the save isn't the ROM's"
    assert c["LEVEL_COUNT"] == (LEVEL_ROOM_LISTS - LEVEL_TABLE) // LEVEL_INFO_SIZE


# The save data's compatibility (save_data.hpp): a save made with other level
# data than the build's must not load. The level data's CRC-32
# (level_data_crc) picks the save record's version byte and the start
# value of its checksum; with the ROM's level data (ROM_LEVEL_DATA_CRC,
# which extract checks) they are the ROM's, 0x12 and 0.
ROM_LEVEL_DATA_CRC = 0x7ED35075
# versionNibble: the high nibble is the game (1, Crash XS; the link cable's
# Crash 2/3 bonus reads it, SaveMenu::LinkInput), the low nibble the ROM's 2.
SAVE_GAME_ID = 1
ROM_SAVE_VERSION = 0x12


def level_data_crc(levels):
    """The CRC-32 of the level data a save depends on: the canonical JSON
    (sorted keys, no spaces, UTF-8) of levels.json's "levels" and of each
    room's room.json, in "rooms" order, then of "extra_progress_flags"
    if there are any. The ROM layout residuals ("regions", "assets",
    "table") and the pictures don't count: they don't change what a save's
    records mean."""
    def canon(v):
        return json.dumps(v, sort_keys=True, separators=(",", ":"), ensure_ascii=False).encode()

    crc = zlib.crc32(canon(levels["levels"]))
    for r in levels["rooms"]:
        crc = zlib.crc32(canon([r, read_json(LEVELS_DIR / r / "room.json")]), crc)
    if extra_progress_flags(levels):
        crc = zlib.crc32(canon(extra_progress_flags(levels)), crc)
    return crc


def save_version(crc):
    """(versionNibble, checksum seed) for level data of CRC-32 `crc`: the
    ROM's with the ROM's data. Otherwise the high nibble stays the game's
    and the low one is one of the other 15, picked by the CRC, and the
    seed is the CRC xor the ROM's: never 0, so a save of the ROM's (or of
    any other level data with another CRC) fails the checksum."""
    if crc == ROM_LEVEL_DATA_CRC:
        return ROM_SAVE_VERSION, 0
    others = [n for n in range(16) if n != ROM_SAVE_VERSION & 0xF]
    return SAVE_GAME_ID << 4 | others[crc % len(others)], crc ^ ROM_LEVEL_DATA_CRC


def constants_h(which):
    if which == "entities":
        src, guard = ENTITY_TYPES_JSON, "GUARD_CONSTANTS_ENTITIES_H"
        types = read_json(src)["types"]
        doc = """/*
 * Entity types: the `type` of a room's placed entities (docs/levels.md,
 * "Entity types"), the index into gEntitySpawnFuncs and into a room's
 * `typeCounts` (LevelHasEntityType). Each is named after its spawner,
 * which docs/levels.md identifies from the sprite bank and animation it
 * sets up. The types whose spawner is SpawnNoEntity have no name: no
 * level places them.
 *
 * Enemy kinds: `part_ctrl.kind`, the kind an enemy spawner gives the
 * shared enemy controller (CreateEnemyCtrl, UpdateEnemyCtrl). It is the
 * number of the enemy's sprite bank (graphics/sprites/).
 */"""
        ents = [("ENTITY_" + t["name"], "0x%02X" % t["type"],
                 t["spawner"] + ("; " + t["comment"] if "comment" in t else ""))
                for t in types if t["name"]]
        ents.append(("ENTITY_COUNT", "0x%02X" % len(types), "the length of gEntitySpawnFuncs"))
        kinds = sorted((t["enemy_kind"], "ENEMY_KIND_" + t.get("enemy_kind_name", t["name"]), t["spawner"])
                       for t in types if "enemy_kind" in t)
        body = define_lines(ents) + [""] + define_lines([(n, "0x%02X" % v, c) for v, n, c in kinds])
    elif which == "crates":
        src, guard = CRATE_KINDS_JSON, "GUARD_CONSTANTS_CRATES_H"
        kinds = read_json(src)["kinds"]
        assert [k["kind"] for k in kinds] == list(range(len(kinds))), src
        doc = """/*
 * Crate kinds: `crate.kind`, the type CreateCrate builds and the index
 * into the gCrateKind* tables and gCrateHitResponse. Kinds 0-18 are what
 * the crate spawners (SpawnBasicCrate .. SpawnTimeCrate3) pass to
 * CreateCrate, in the order of their entity types: a crate's entity type
 * is ENTITY_BASIC_CRATE + its kind. Kinds 19-21 are a lit TNT crate
 * counting down (LightTntCrate, UpdateTntCountdown).
 */"""
        rows = [("CRATE_KIND_" + k["name"], "%d" % k["kind"], k.get("comment")) for k in kinds]
        rows.append(("CRATE_KIND_COUNT", "%d" % len(kinds), "the length of the gCrateKind* tables"))
        body = define_lines(rows)
    elif which == "levels":
        src, guard = LEVELS_JSON, "GUARD_CONSTANTS_LEVELS_H"
        data = read_json(src)
        levels = data["levels"]
        c = level_counts(data)
        doc = """/*
 * Level ids: the index of gLevelTable, gLevelRoomLists and
 * level_state.levelFlags, and the value of level_state.level. Each is
 * named after the level's name text (gLevelTable's nameText). The
 * numbered levels come first, world by world, up to LEVELS_PER_WORLD
 * per world (the level select's LEVEL_SELECT_WORLD_FIRST(world) + index,
 * level_select_worlds.h; the pause menu's "level N" label), then the
 * boss levels, at most one per world, in world order, then the levels
 * off the level select's grid, mega-mix (LEVEL_FINALE) first.
 *
 * The counts below are derived from the levels too (tools/levels.py,
 * level_counts); with the ROM's levels each is the ROM's number, which
 * `levels.py extract` checks (ROM_COUNTS).
 */"""
        rows = [(lv["constant"], "%d" % i, lv["name"]) for i, lv in enumerate(levels)]
        rows.append(("LEVEL_COUNT", "%d" % len(levels), "the length of gLevelTable"))
        body = define_lines(rows) + [""] + define_lines([
            ("WORLD_COUNT", "%d" % c["WORLD_COUNT"], "the level select's pages"),
            ("LEVELS_PER_WORLD", "%d" % c["LEVELS_PER_WORLD"], "the most numbered levels of a page"),
            ("LEVEL_NUMBERED_COUNT", "%d" % c["LEVEL_NUMBERED_COUNT"], "the worlds' numbered levels"),
            ("LEVEL_LAST_NUMBERED", c["LEVEL_LAST_NUMBERED"], None),
            ("LEVEL_FIRST_BOSS", c["LEVEL_FIRST_BOSS"], "the first page's boss"),
            ("LEVEL_LAST_ON_GRID", c["LEVEL_LAST_ON_GRID"], "the last page's boss"),
            ("LEVEL_FINALE", c["LEVEL_FINALE"], "the first level off the grid"),
            ("LEVEL_SELECT_ENTRIES", "%d" % c["LEVEL_SELECT_ENTRIES"], "a full page's levels and its boss"),
        ]) + ["", "/* The pause menu's totals and the completion percentage's divisor. */"] + define_lines([
            ("CRYSTAL_TOTAL", "%d" % c["CRYSTAL_TOTAL"], "a crystal per numbered level"),
            ("GEM_TOTAL", "%d" % c["GEM_TOTAL"], "crate gems, gem-path gems and the 4 coloured gems"),
            ("RELIC_TOTAL", "%d" % c["RELIC_TOTAL"], "a relic per numbered level"),
            ("COMPLETION_TOTAL", "%d" % c["COMPLETION_TOTAL"], "the three totals and the 4 powers"),
        ]) + ["", "/* The length of the UI text tables (gUiTextEnglish, ...): the ROM's 70 strings,",
              " * or more when a level's name_text is past them. */"] + define_lines([
            ("UI_TEXT_COUNT", "%d" % c["UI_TEXT_COUNT"], None),
        ]) + ["", "/* The progress flags past the ROM's 8 (levels.json's \"extra_progress_flags\"):",
              " * bit N is bit N % 8 of GameProgress::extraFlags[N / 8], a word per 32 of them",
              " * (none in the ROM). */"] + define_lines([
            ("PROGRESS_EXTRA_FLAG_BYTES", "%d" % c["PROGRESS_EXTRA_FLAG_BYTES"], "GameProgress::extraFlags"),
        ] + [("PROGRESS_EXTRA_BIT_" + f["name"], "%d" % i, f.get("comment"))
             for i, f in enumerate(extra_progress_flags(data))])
    elif which == "themes":
        src, guard = LEVELS_JSON, "GUARD_CONSTANTS_THEMES_H"
        data = read_json(src)
        themes = level_themes(data)
        doc = """/*
 * Level themes: `struct level_info.theme`, the index of gThemeMusicCues
 * (PlayRoomMusic) and of the level select's pictures (ZoomBg::SetPicture),
 * and the case of RunRoom's colour cycles (theme_palette_cycles.h). Each
 * is a theme of levels.json's "themes", which gives its song, picture
 * and colour cycle (docs/levels.md, "Themes").
 */"""
        rows = [(t["constant"], "%d" % i, t["name"]) for i, t in enumerate(themes)]
        rows.append(("THEME_COUNT", "%d" % len(themes), "the length of gThemeMusicCues"))
        exits = [t["constant"] for t in themes if t.get("first_room_exit_zone")]
        body = define_lines(rows) + [""] + define_lines([
            ("ZOOM_BG_NO_PICTURE", "THEME_COUNT", "ZoomBg::image when no picture is wanted"),
            ("LEVEL_SELECT_PICTURE_COUNT", "%d" % level_select_picture_count(themes),
             "gLevelSelectPictures: up to the last theme with a picture"),
        ]) + ["", "/* Whether the exit of a level's first room is a plain zone, not the exit pad",
              " * (SpawnRoomExit): the themes with \"first_room_exit_zone\". */",
              "#define THEME_FIRST_ROOM_EXIT_ZONE(theme) (%s)"
              % (" || ".join("(theme) == %s" % c for c in exits) or "0")]
    elif which == "categories":
        src, guard = LEVELS_JSON, "GUARD_CONSTANTS_CATEGORIES_H"
        cats = level_categories(read_json(src))
        doc = """#include "constants/category_types.h"

/*
 * Actor categories: the index of ActorCategory::descriptors, the `catIndex` of a
 * ROOM_KIND_CATEGORY stage and ActorCategory::Init's argument, levels.json's
 * "categories" (docs/levels.md, "Actor categories"). The ROM's are the stages
 * of its levels: the polar rides of frostbite cavern, snow crash and snow job,
 * the jetpack levels, and n. gin's fight. The types and the exit statuses are
 * constants/category_types.h.
 */"""
        rows = [(c["constant"], "%d" % i, c["name"]) for i, c in enumerate(cats)]
        rows.append(("CATEGORY_COUNT", "%d" % len(cats), "the length of ActorCategory::descriptors"))
        body = define_lines(rows)
    elif which == "save":
        src, guard = LEVELS_JSON, "GUARD_CONSTANTS_SAVE_H"
        data = read_json(src)
        crc = level_data_crc(data)
        version, seed = save_version(crc)
        chip, size, _ = save_layout(data)
        doc = """/*
 * The save data (SaveData, save_data.hpp; docs/levels.md, "The save"),
 * from the level data.
 *
 * The EEPROM and the record's size (tools/levels.py, save_layout): the
 * ROM's 4 Kbit EEPROM, whose 0x200 bytes are the record, while its four
 * slots fit; else the 64 Kbit EEPROM, with a record of the slots' size.
 *
 * The version and the checksum seed, from the CRC-32 of the level data
 * (level_data_crc): a save made with other levels than the build's
 * doesn't load. With the ROM's level data they are the ROM's: version
 * 0x12 and seed 0. Else the version's low nibble is another one picked
 * by the CRC (the high one stays Crash XS's game id), and the seed is
 * the CRC xor the ROM's, never 0, so the checksum of a save of other
 * level data fails.
 */"""
        body = define_lines([
            ("SAVE_EEPROM_TYPE", {EEPROM_4KBIT: "EEPROM_CHIP_4KBIT", EEPROM_64KBIT: "EEPROM_CHIP_64KBIT"}[chip],
             "EEPROMConfigure's chip (<agb_eeprom.h>)"),
            ("SAVE_DATA_SIZE", "0x%03X" % size, "sizeof(SaveData), a multiple of the EEPROM's 8-byte blocks"),
        ]) + ["", "/* the level data's CRC-32: 0x%08X (the ROM's: 0x%08X) */" % (crc, ROM_LEVEL_DATA_CRC)] + define_lines([
            ("SAVE_VERSION", "0x%02X" % version, "versionNibble"),
            ("SAVE_CHECKSUM_SEED", "0x%08X" % seed if seed else "0", "the checksum's start value"),
        ])
    else:
        sys.exit("unknown constants %s" % which)
    return "\n".join(["/* Generated by tools/levels.py from %s. Do not edit. */" % src.as_posix(),
                      "#ifndef " + guard, "#define " + guard, "", doc, ""] + body
                     + ["", "#endif /* %s */" % guard, ""])


def main():
    args = sys.argv[1:]
    if not args or args[0] in ("-h", "--help"):
        print(__doc__)
        return
    cmd = args[0]
    build_dir = None
    if "--build" in args:
        i = args.index("--build")
        build_dir = args[i + 1]
        del args[i:i + 2]
    if cmd == "extract":
        extract(args[1] if len(args) > 1 else "baserom.gba")
    elif cmd == "rooms":
        # levels.json's rooms (their folders); with --packed the ones whose
        # asset is LZ77-packed, with --new the ones in no ROM region
        levels = read_json(LEVELS_JSON)
        rooms = new_rooms(levels) if "--new" in args else levels["rooms"]
        if "--packed" in args:
            rooms = [r for r in rooms if read_json(LEVELS_DIR / r / "room.json")["asset"]["packed"]]
        print(" ".join(rooms))
    elif cmd == "tileset":
        tileset_build(args[1], build_dir)
    elif cmd == "tilesets-mk":
        tilesets_mk(args[1], build_dir)
    elif cmd == "table":
        if len(args) != 3 or args[1] not in ("levels", "records", "pictures"):
            sys.exit("usage: levels.py table levels|records|pictures OUT")
        table_c(args[1], args[2])
    elif cmd == "asset":
        asset, offsets, grids = build_asset(args[1], build_dir)
        Path(args[2]).write_bytes(asset)
        layout = Path(args[2]).with_name("asset.layout.json")
        layout.write_text(json.dumps({"offsets": offsets, "grids": grids}) + "\n")
    elif cmd == "c":
        region_c(args[1], args[2], build_dir)
    elif cmd == "assets":
        assets_c(args[1])
    elif cmd == "classes":
        write_if_changed(args[1], rooms_hpp().encode())
    elif cmd == "symbols":
        room_symbols("--check" in args)
    elif cmd == "render":
        render(args[1], args[2], args[3], build_dir)
    elif cmd == "constants":
        write_if_changed(args[2], constants_h(args[1]).encode())
    elif cmd == "palette-cycles":
        write_if_changed(args[1], theme_palette_cycles_h(read_json(LEVELS_JSON)).encode())
    elif cmd == "worlds":
        write_if_changed(args[1], level_select_worlds_h(read_json(LEVELS_JSON)).encode())
    elif cmd == "spawns":
        Path(args[2]).parent.mkdir(parents=True, exist_ok=True)
        Path(args[2]).write_text(spawns_inc(args[1]))
    elif cmd == "categories":
        Path(args[1]).parent.mkdir(parents=True, exist_ok=True)
        Path(args[1]).write_text(actor_categories_inc(read_json(LEVELS_JSON)))
    else:
        sys.exit("unknown command %s" % cmd)


if __name__ == "__main__":
    main()
