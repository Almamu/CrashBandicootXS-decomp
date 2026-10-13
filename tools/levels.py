#!/usr/bin/env python3
"""The 41 rooms' level data: extract to editable sources, build back.

See docs/levels.md for the format. In short, a room is

- a `struct level_desc` (level_layers.cpp's view): up to five layers (BG1-3,
  the pooled BG0 and the collision layer), the level asset, the entity list
  and the entity links;
- per layer a `struct level_layer_desc` (the BG layers' source,
  include/bg_layer.hpp) and a grid of 16x8-cell chunk ids;
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
below), tileset_order.json (the room order of 8 of the ROM's
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
                                              DIR/<room>/asset.layout.json
    tools/levels.py assets OUT                C definitions of the ROM's level assets,
                                              in levels.json's "assets" order; each
                                              #includes the room's asset bytes
                                              (bin2c output, see levels.mk)
    tools/levels.py appended OUT --build DIR  C definitions of the level data that
                                              isn't the ROM's: the new tile sets,
                                              the new rooms, their records and
                                              room lists, and their assets
                                              (src/data/level_appended.cpp)
    tools/levels.py table levels|records OUT  the level table of levels.json and the
                                              rooms' room.json, in place: gLevelTable
                                              and gLevelRoomLists (levels), or the
                                              room records and the played-room
                                              arrays (records), #included by
                                              src/data/level_table_16c814.cpp
    tools/levels.py render ROOMDIR LAYER OUT [--build DIR]
                                              one layer's picture: the collision
                                              image, or a BG layer drawn again
                                              from the build's cells and tile set
    tools/levels.py constants entities OUT    constants/entities.h (ENTITY_*,
                                              ENEMY_KIND_*) from entity_types.json
    tools/levels.py constants crates OUT      constants/crates.h (CRATE_KIND_*)
                                              from crate_kinds.json
    tools/levels.py constants levels OUT      constants/levels.h (LEVEL_*) from
                                              levels.json's "levels" list

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
    ("level_rooms_24b638", 0x0824B638, 0x08270F08),
    ("level_rooms_2b91d0", 0x082B91D0, 0x082BF120),
]
# The level assets, all 41 back to back (src/data/level_assets_64f830.cpp).
ASSET_REGION = ("level_assets_64f830", 0x0864F830, 0x087E3BEC)
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
        out.append("/* %s chunk grid, %d x %d */" % (lname, w, h))
        out.append("extern const u16 %s[%d] =\n{" % (layer["grid_symbol"], len(g)))
        out.append(c_array(g, "%d", w))
        out.append("};\n")

    # BG palette, from the layer pictures.
    pal = room_palette(room_dir, room)
    out.append("extern const u16 %s[256] =\n{" % sym["palette"])
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
    out.append("    %s," % ext(a["symbol"]))
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
        out.append("        %s, %#x, %s," % (layer["grid_symbol"], offsets[lname], ext(layer["tileset"])))
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
    levels = read_json(LEVELS_DIR / "levels.json")
    region = next((r for r in levels["regions"] if r["name"] == region_name), None)
    if region is None:
        sys.exit("levels.json has no region %s" % region_name)
    externs, bodies = set(), []
    for rname in region["rooms"]:
        e, body = room_c(LEVELS_DIR / rname, build_dir)
        externs |= e
        bodies.append(body)
    text = ["/* Generated by tools/levels.py from data/levels/ - do not edit. */", ""]
    for s in sorted(externs):
        text.append("extern const u8 %s[];" % s)
    text.append("")
    text += bodies
    Path(out_path).write_text("\n".join(text))


def assets_c(out_path):
    """The definitions of the level assets, in ROM order: one `u8` array
    per room, named by room.json's asset symbol. The bytes are levels.mk's
    bin2c output of the built asset (`asset.bin.lz.inc` for a packed one,
    `asset.bin.inc` for a raw one). Every asset starts on a word boundary
    (`aligned(4)`); the 0-3 zero bytes after a packed one are that
    alignment."""
    levels = read_json(LEVELS_JSON)
    rooms = levels["assets"]["rooms"]
    if sorted(rooms) != sorted(rom_region_rooms(levels)):
        sys.exit("levels.json: the \"assets\" order must list every room of the ROM's regions once "
                 "(a new room's asset goes with the room, after the IWRAM image)")
    text = ["/* Generated by tools/levels.py from data/levels/ - do not edit. */", ""]
    for rname in rooms:
        asset = read_json(LEVELS_DIR / rname / "room.json")["asset"]
        packed = asset["packed"]
        text.append("/* %s: %s */" % (rname, "LZ77-packed" if packed else "raw"))
        text.append("extern const u8 %s[] __attribute__((aligned(4))) = {" % asset["symbol"])
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
    levels.json order: src/data/level_appended.cpp holds them."""
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
    hand-written src/data files); the others are new ones."""
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
    cells}). A new room may only use the tiles a set of the ROM's rooms
    has: its tiles would move later ROM data."""
    users = users if users is not None else tileset_users(levels)
    order = order if order is not None else load_tileset_order()
    if symbol not in users:
        sys.exit("no room's layer names the tile set %s" % symbol)
    spec = users[symbol]
    bg0 = spec["bpp"] == 8
    shift = 14 if bg0 else 10
    rom = set(rom_region_rooms(levels))
    rom_set = is_rom_tileset(spec, levels)
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
        new = room not in rom
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
                    if new and rom_set:
                        errors.append("%s: cell (%d, %d) %s isn't a tile of %s under any flip, and %s is a tile "
                                      "set of the ROM's rooms: a new room can only use its tiles. Name a new tile "
                                      "set in the layer's \"tileset\" (room.json) to have one made from the "
                                      "room's pictures" % (path, x, y, _pixels(x, y), symbol, symbol))
                        continue
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


def appended_c(out_path, build_dir):
    """`levels.py appended`: the C of src/data/level_appended.cpp, the level
    data that isn't the ROM's: the new tile sets (used by no room of a ROM
    region), the new rooms' data, the level table's new records and room
    lists (LevelTable.appended_c), then the new rooms' assets. Empty with
    the original data."""
    levels = read_json(LEVELS_JSON)
    users = tileset_users(levels)
    text = ["/* Generated by tools/levels.py from data/levels/ - do not edit. */", ""]
    defs, externs = [], set()
    for symbol, spec in users.items():
        if is_rom_tileset(spec, levels):
            continue
        defs.append("/* new tile set, %dbpp */" % spec["bpp"])
        defs.append("extern const u8 %s[] __attribute__((aligned(4))) = {" % symbol)
        if spec["bpp"] == 8:
            # a tag-0x00 raw tagged asset: {size << 8}, then the tiles
            n = (Path(build_dir) / "tilesets" / (symbol + ".bin")).stat().st_size
            defs.append("    0x00, 0x%02X, 0x%02X, 0x%02X," % (n & 0xFF, n >> 8 & 0xFF, n >> 16 & 0xFF))
            defs.append('#include "tilesets/%s.bin.inc"' % symbol)
        else:
            defs.append('#include "tilesets/%s.bin.lz.inc"' % symbol)
        defs += ["};", ""]
    rooms = new_rooms(levels)
    for r in rooms:
        e, body = room_c(LEVELS_DIR / r, build_dir)
        externs |= e
        defs.append(body)
    defs.append(LevelTable(levels).appended_c())
    for r in rooms:
        asset = read_json(LEVELS_DIR / r / "room.json")["asset"]
        defs.append("/* %s's asset: %s */" % (r, "LZ77-packed" if asset["packed"] else "raw"))
        defs.append("extern const u8 %s[] __attribute__((aligned(4))) = {" % asset["symbol"])
        defs.append('#include "%s/asset.bin%s.inc"' % (r, ".lz" if asset["packed"] else ""))
        defs += ["};", ""]
    text += ["extern const u8 %s[];" % s for s in sorted(externs)] + [""] + defs
    Path(out_path).write_text("\n".join(text))


# ---------------------------------------------------------------------------
# The level table (src/data/level_table_16c814.cpp, docs/levels.md, "The
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
# in ROM order, which also gives the order of the records of the rooms
# they play; the other rooms of the ROM's regions are the extra rooms,
# whose records come first, in "rooms" order. A level whose "rooms" isn't
# its ROM array any more gets a new array after the IWRAM image (with the
# new rooms' records), and its gLevelRoomLists entry points at it: the
# ROM's tables are only changed in place, never moved.

# The record kinds (ROOM_KIND_*, include/constants/rooms.h); a room's
# "kind" is one of the first three.
ROOM_KINDS = ["ON_FOOT", "UNDERWATER", "HOVER", "CATEGORY"]
CATEGORIES_H = Path("include/constants/categories.h")
# gLevelTable (struct level_info, 0x24 bytes) and the ROM's room table.
LEVEL_TABLE = 0x0816C86C
LEVEL_INFO_SIZE = 0x24
LEVEL_ROOM_SIZE = 0x14
# The keys every level of levels.json's "levels" has ("bonus", "gem_path"
# and "boss" are optional).
LEVEL_KEYS = ["constant", "name", "name_text", "theme", "times", "mask_assist_deaths", "crate_assist_deaths",
              "rooms"]


def categories():
    """The actor categories (CATEGORY_* of include/constants/categories.h,
    up to CATEGORY_COUNT): name without the prefix -> number."""
    out = {}
    for m in re.finditer(r"^#define CATEGORY_(\w+) (\d+)", CATEGORIES_H.read_text(), re.M):
        if m.group(1) == "COUNT":
            break
        out[m.group(1)] = int(m.group(2))
    return out


def stage_symbol(number):
    return "gLevelStage%d" % number


def room_list_symbol(level_id, new=False):
    return "gLevel%02d%sRooms" % (level_id, "New" if new else "")


class LevelTable:
    """The level table of levels.json and the rooms' room.json: what goes
    in place (`level_table_c`, `records_c`) and what after the IWRAM
    image (`appended_c`)."""

    def __init__(self, levels):
        self.levels = levels["levels"]
        self.ids = {lv["constant"]: i for i, lv in enumerate(self.levels)}
        table = levels.get("table")
        if table is None:
            sys.exit("%s: no \"table\" (the ROM's level table layout, written by levels.py extract)" % LEVELS_JSON)
        self.rom_lists = table["lists"]
        self.rooms = levels["rooms"]
        self.rom_rooms = set(rom_region_rooms(levels))
        self.cats = categories()
        self.records = {r: read_json(LEVELS_DIR / r / "room.json") for r in self.rooms}
        listed = [lst["level"] for lst in self.rom_lists]
        for lv in self.levels:
            if lv["constant"] not in listed:
                sys.exit("%s: %s isn't one of the ROM's levels: gLevelTable has room for the ROM's %d "
                         "levels only (new levels are #966)" % (LEVELS_JSON, lv["constant"], len(listed)))
        if sorted(listed) != sorted(self.ids):
            sys.exit("%s: \"table\" must list each of the ROM's levels once" % LEVELS_JSON)
        for lst in self.rom_lists:
            for e in lst["rooms"]:
                if isinstance(e, str) and e not in self.rom_rooms:
                    sys.exit("%s: \"table\" lists %s, which isn't a room of the ROM's regions" % (LEVELS_JSON, e))
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
        # the played-room arrays: the ROM's, and a new one for a level whose
        # rooms aren't its ROM array any more
        self.rom_array = {lst["level"]: lst["rooms"] for lst in self.rom_lists}
        self.new_arrays = [lv["constant"] for lv in self.levels if lv["rooms"] != self.rom_array[lv["constant"]]]

    def ref(self, e, where):
        """A room-list entry's record symbol."""
        if isinstance(e, dict):
            if set(e) != {"stage"} or e["stage"] not in self.cats:
                sys.exit("%s: %s: %s isn't a room folder or {\"stage\": <a CATEGORY_* name of %s, without "
                         "the prefix>}" % (LEVELS_JSON, where, json.dumps(e), CATEGORIES_H))
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
        if constant in self.new_arrays:
            return room_list_symbol(i, True), self.levels[i]["rooms"]
        return room_list_symbol(i), self.rom_array[constant]

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
                "    %s, &%s, ROOM_KIND_%s, 0, { { %d, %d, %d } }" % (sym["palette"], sym["desc"], rec["kind"],
                                                                     *blend),
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
            out += ["extern const u16 %s[];" % sym["palette"], "extern const struct level_desc %s;" % sym["desc"]]
        out += ["extern const struct level_room *const %s[%d];" % a for a in arrays]
        out += ["extern const struct level_room %s;" % s for s in records]
        return out

    def all_arrays(self):
        """(symbol, length) of every played-room array, ROM and new."""
        out = [(room_list_symbol(self.ids[lst["level"]]), len(lst["rooms"])) for lst in self.rom_lists]
        return out + [(room_list_symbol(self.ids[c], True), len(self.levels[self.ids[c]]["rooms"]))
                      for c in self.new_arrays]

    def all_records(self):
        return [self.records[r]["record"]["symbol"] for r in self.rooms] + \
            [stage_symbol(n) for n in sorted(self.cats.values())]

    def level_table_c(self):
        """gLevelTable and gLevelRoomLists (level_table_16c814.inc)."""
        out = ["/* Generated by tools/levels.py from data/levels/levels.json - do not edit. */", ""]
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
        return "\n".join(out)

    def records_c(self):
        """The ROM's room records and played-room arrays, in ROM order
        (level_table_16c814_rooms.inc): the extra rooms' records, the
        arrays, then the records of the rooms and stages they play."""
        in_lists = {e for lst in self.rom_lists for e in lst["rooms"] if isinstance(e, str)}
        extras = [r for r in self.rooms if r in self.rom_rooms and r not in in_lists]
        played, seen = [], set()
        for lst in self.rom_lists:
            for e in lst["rooms"]:
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
        for lst in self.rom_lists:
            c = lst["level"]
            note = c if c not in self.new_arrays else "%s's in the ROM; it plays %s now" % (
                c, room_list_symbol(self.ids[c], True))
            out += self.array_c(room_list_symbol(self.ids[c]), lst["rooms"], note)
        out += ["", "/* The records of the rooms and stages they play. */"]
        for e in played:
            out += self.room_record(e) if isinstance(e, str) else self.stage_record(e["stage"], self.cats[e["stage"]])
        return "\n".join(out) + "\n"

    def appended_c(self):
        """The new rooms' records and the new played-room arrays, for
        level_appended.inc. Empty with the original data."""
        new = [r for r in self.rooms if r not in self.rom_rooms]
        if not new and not self.new_arrays:
            return ""
        out = ["/* The level table's new records and room lists (levels.json, \"levels\") */"]
        new_syms = {self.ref(r, r) for r in new}
        # the ROM's records the new arrays play (the new rooms' palettes
        # and descriptors are defined above)
        used = {self.ref(e, c) for c in self.new_arrays for e in self.array(c)[1]}
        out += self.externs(records=[s for s in self.all_records() if s in used and s not in new_syms]) + [""]
        for r in new:
            out += self.room_record(r)
        for c in self.new_arrays:
            sym, entries = self.array(c)
            out += self.array_c(sym, entries, c)
        return "\n".join(out) + "\n"


def table_c(which, out_path):
    """`levels.py table levels|records OUT`: the in-place level table."""
    t = LevelTable(read_json(LEVELS_JSON))
    Path(out_path).parent.mkdir(parents=True, exist_ok=True)
    Path(out_path).write_text(t.level_table_c() if which == "levels" else t.records_c())


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
# (the names); it writes the others from the ROM ("boss" must agree).
LEVEL_KEYS_KEPT = ["constant", "name", "world"]


def level_table_from_rom(rom, level_list, folders):
    """levels.json's "levels" (the kept names, the rest from gLevelTable and
    gLevelRoomLists) and "table" (the ROM's played-room arrays, in ROM
    order)."""
    cat_names = {n: name for name, n in categories().items()}

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
        return dump(v)

    LEVELS_JSON.write_text(block(levels, 0, top) + "\n")


def extract(rom_path):
    rom = Rom(rom_path)
    records = room_records(rom)
    rooms = find_rooms(rom)
    assert len(rooms) == 41, len(rooms)
    # the level names aren't extracted: the hand-written ones name the
    # folders (LEVEL_KEYS_KEPT)
    level_list = read_json(LEVELS_JSON)["levels"]
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
              "levels": level_info}
    write_levels_json(levels)
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
        levels = read_json(src)["levels"]
        numbered = [lv for lv in levels if not lv.get("boss")]
        bosses = [lv for lv in levels if lv.get("boss")]
        per_world = {}
        for lv in numbered:
            per_world[lv["world"]] = per_world.get(lv["world"], 0) + 1
        assert len(set(per_world.values())) == 1, per_world
        assert levels.index(numbered[-1]) + 1 == levels.index(bosses[0]), src
        doc = """/*
 * Level ids: the index of gLevelTable, gLevelRoomLists and
 * level_state.levelFlags, and the value of level_state.level. Each is
 * named after the level's name text (gLevelTable's nameText). The
 * numbered levels come first, LEVELS_PER_WORLD per world (the level
 * select's `world * LEVELS_PER_WORLD + index`, the pause menu's
 * "level N" label), then the boss levels: one per world
 * (`LEVEL_FIRST_BOSS + world`) and mega-mix.
 */"""
        rows = [(lv["constant"], "%d" % i, lv["name"]) for i, lv in enumerate(levels)]
        rows.append(("LEVEL_COUNT", "%d" % len(levels), "the length of gLevelTable"))
        body = define_lines(rows) + [""] + define_lines([
            ("LEVELS_PER_WORLD", "%d" % next(iter(per_world.values())), None),
            ("LEVEL_LAST_NUMBERED", numbered[-1]["constant"], None),
            ("LEVEL_FIRST_BOSS", bosses[0]["constant"], None),
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
    elif cmd == "appended":
        appended_c(args[1], build_dir)
    elif cmd == "table":
        if len(args) != 3 or args[1] not in ("levels", "records"):
            sys.exit("usage: levels.py table levels|records OUT")
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
    elif cmd == "render":
        render(args[1], args[2], args[3], build_dir)
    elif cmd == "constants":
        Path(args[2]).write_text(constants_h(args[1]))
    else:
        sys.exit("unknown command %s" % cmd)


if __name__ == "__main__":
    main()
