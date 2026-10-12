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

The editable sources live in data/levels/: levels.json (the two ROM
regions and which rooms they hold, in ROM order, and the ROM order of the
level assets) and one directory per
room with room.json, one `<layer>.png` per BG layer, the layer's whole
picture (8bpp indexed with the room's palette, 8x8 pixels per cell; a
BG1-3 pixel is `bank * 16 + pixel`, see "BG layer pictures" below), and
`collision.png`, the collision layer as an image the size of the BG0
picture (one 8x8 block per cell, coloured by
data/levels/collision_legend.json, see tools/collision_bmp.py). The
cells, the chunk set, the chunk grid, the asset offsets, the entity
groups, the parameter offsets and the per-type counts are all derived
from those again.

    tools/levels.py extract [baserom.gba]     write data/levels/ from the ROM
    tools/levels.py from-png ROOMDIR LAYER OUT
                                              a BG layer's picture -> its cells
                                              (little-endian u16, row-major), and
                                              OUT.d, make's dependency on the
                                              layer's tile set PNG
    tools/levels.py to-png ROOMDIR LAYER MAP OUT
                                              the way back: cells -> picture
    tools/levels.py asset ROOMDIR OUT [--build DIR]
                                              build a room's level asset, and
                                              asset.layout.json next to it (the
                                              layers' offsets and chunk grids);
                                              with --build, the BG cells are
                                              DIR/<room>/<layer>.map.bin (from-png)
    tools/levels.py c REGION OUT [--build DIR]
                                              C definitions of one room region
                                              (REGION = its name in levels.json);
                                              with --build, from the rooms'
                                              DIR/<room>/asset.layout.json
    tools/levels.py assets OUT                C definitions of the 41 level assets,
                                              in levels.json's "assets" order; each
                                              #includes the room's asset bytes
                                              (bin2c output, see levels.mk)
    tools/levels.py render ROOMDIR LAYER OUT  one layer's picture: a BG layer's
                                              PNG drawn again from its cells, or
                                              the collision image
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
import struct
import sys
import zlib
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import collision_bmp  # noqa: E402

ROM_BASE = 0x08000000
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
    """A layer's cells, row-major. A BG layer's come from its picture
    (read_layer_png), or with `build_dir` from the map `from-png` decoded it
    to, `<build_dir>/<room>/<layer>.map.bin` (the build's copy)."""
    path = Path(room_dir) / layer["map"]
    w, h = layer["size_chunks"]
    if lname != "collision":
        if build_dir is None:
            return read_layer_png(path, lname, layer)
        path = Path(build_dir) / Path(room_dir).name / ("%s.map.bin" % lname)
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
    room = read_json(Path(room_dir) / "room.json")
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
    room = read_json(Path(room_dir) / "room.json")
    name = room["name"]
    sym = room["symbols"]
    if build_dir is None:
        _, offsets, grids = build_asset(room_dir)
    else:
        layout = read_json(Path(build_dir) / Path(room_dir).name / "asset.layout.json")
        offsets, grids = layout["offsets"], layout["grids"]
    out = []
    externs = set()

    def ext(s):
        if s:
            externs.add(s)
        return s or "0"

    by_name = {n: t for t, n in enumerate(entity_type_names()) if n}
    types = [entity_type(name, e["type"], by_name) for e in room["entities"]]
    out.append("/* %s: level_desc %s, %d entities (data/levels/%s) */"
               % (name, sym["desc"], len(room["entities"]), Path(room_dir).name))
    # forward declarations for the pointers to objects defined further down
    out.append("extern const struct level_entity_list %s;" % sym["entities"])
    out.append("extern const struct level_layer_desc %s[];" % sym["layers"])
    out.append("extern const struct level_entity_group %s[];" % sym["groups"])
    out.append("extern const struct level_entity %s[];\n" % sym["items"])

    # Spawn parameter records.
    recs = room["params"]
    stype = "level_params_%s" % name
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

    # BG palette.
    pal = [int(c, 16) for c in room["palette"]]
    if len(pal) != 256:
        sys.exit("%s: the palette needs 256 colours" % name)
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
    if sorted(rooms) != sorted(levels["rooms"]):
        sys.exit("levels.json: the \"assets\" order must list every room once")
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
# BG layer pictures (data/levels/<room>/bg0-3.png)
#
# A BG layer is kept as its whole picture: 8x8 pixels per cell, 8bpp
# indexed with the room's palette. A cell's block is its tile, flipped as
# the cell says. BG0's tiles are 8bpp, so a pixel is the tile's colour
# index. A BG1-3 pixel is `bank * 16 + p`, p the 4bpp tile's pixel, for all
# 64 pixels, the transparent ones (p = 0) too: that keeps the cell's
# palette bank even in a blank cell, so every cell comes back exactly.
#
# Reading a picture back looks each block up among the tile set's tiles
# under the four flips. Several tiles (or flips of one tile) can draw the
# same block; the cell is then the lowest tile index, and for that tile the
# first flip of none, h, v, hv. That reproduces every cell of the ROM's
# 131 BG layers.

# The flips a block is looked up under, in order of preference: (h, v).
FLIPS = ((0, 0), (1, 0), (0, 1), (1, 1))
# One 8-pixel row of a block as a little-endian u64 (pixel 0 in the low
# byte): a block is a tuple of 8 of these, top row first.
_NIBBLES = 0x0F0F0F0F0F0F0F0F
_HIGH_NIBBLES = 0xF0F0F0F0F0F0F0F0
_BYTES = 0x0101010101010101


def tileset_png(symbol):
    """The source PNG of a layer's tile set: graphics/level_tilesets/ (BG0,
    8bpp) or graphics/level_bg_tiles/ (BG1-3, 4bpp)."""
    named = {n: a for a, n in TILESET_SYMBOLS.items()}
    addr = "%06x" % (named[symbol] & 0xFFFFFF) if symbol in named else symbol[-6:].lower()
    for pat in ("graphics/level_tilesets/*_%s.png", "graphics/level_bg_tiles/%s_*.png"):
        hits = sorted(Path(".").glob(pat % addr))
        if hits:
            return hits[0]
    sys.exit("no tile set source for %s" % symbol)


def tileset_rows(symbol):
    """A layer's tile set, read from its source PNG (tileset_png), as a
    list of tiles, each 8 rows of 8 colour indices (bytes). The BG1-3 sets
    are drawn in palette banks (graphics/level_bg_tiles/), so only the low
    nibble of an index is the pixel."""
    from PIL import Image

    path = tileset_png(symbol)
    im = Image.open(path)
    raw = im.tobytes()
    if "level_bg_tiles" in path.parts:
        raw = raw.translate(bytes(i & 0x0F for i in range(256)))
    pw = im.size[0]
    tw = pw // 8
    tiles = []
    for t in range(tw * (im.size[1] // 8)):
        o = t // tw * 8 * pw + t % tw * 8
        tiles.append([raw[o + y * pw:o + y * pw + 8] for y in range(8)])
    return tiles


def find_tileset(symbol):
    """A layer's tile set as a list of 64-pixel tiles (colour indices)."""
    return [list(b"".join(rows)) for rows in tileset_rows(symbol)]


class LayerTiles:
    """A BG layer's tile set, both ways: cell -> its 8x8 block, and block ->
    cell. bg0: the cells are tile-slot pool ids (tile in bits 0-13, h/v
    flip in 14-15) of an 8bpp set; else BG screen entries (tile 0-9, h/v
    flip 10-11, palette bank 12-15) of a 4bpp set."""

    _cache = {}

    @classmethod
    def get(cls, symbol, bg0):
        key = (symbol, bg0)
        if key not in cls._cache:
            cls._cache[key] = cls(symbol, bg0)
        return cls._cache[key]

    def __init__(self, symbol, bg0):
        self.symbol = symbol
        self.bg0 = bg0
        # per tile, its block under each flip of FLIPS
        self.flipped = []
        for rows in tileset_rows(symbol):
            plain = tuple(int.from_bytes(r, "little") for r in rows)
            mirrored = tuple(int.from_bytes(r, "big") for r in rows)  # h flip
            self.flipped.append((plain, mirrored, plain[::-1], mirrored[::-1]))
        self.blocks = {}  # cell value -> block
        self.cells = {}  # block -> cell value; for BG1-3 filled per bank as met
        self.base = {}  # BG1-3: bank-0 block -> cell value without the bank
        table = self.cells if bg0 else self.base
        shift = 14 if bg0 else 10
        for t, blocks in enumerate(self.flipped):
            for (hf, vf), block in zip(FLIPS, blocks):
                table.setdefault(block, t | hf << shift | vf << shift + 1)

    def block(self, v):
        """The 8x8 block a cell draws, as 8 u64 rows (None for a tile past
        the end of the set)."""
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

    def cell(self, block):
        """The cell that draws `block`: (value, None), or (None, why not)."""
        v = self.cells.get(block)
        if v is not None:
            return v, None
        if self.bg0:
            return None, "isn't a tile of %s under any flip" % self.symbol
        bank = block[0] >> 4 & 0xF
        if any(r & _HIGH_NIBBLES != bank * 16 * _BYTES for r in block):
            banks = sorted({p >> 4 for r in block for p in r.to_bytes(8, "little")})
            return None, ("mixes palette banks %s: a BG1-3 cell draws all 64 pixels from one bank "
                          "(indices bank * 16 to bank * 16 + 15)" % ", ".join(map(str, banks)))
        v = self.base.get(tuple(r & _NIBBLES for r in block))
        if v is None:
            return None, ("isn't a tile of %s under any flip (in palette bank %d, indices %d-%d)"
                          % (self.symbol, bank, bank * 16, bank * 16 + 15))
        v |= bank << 12
        self.cells[block] = v
        return v, None


def layer_tiles(lname, layer):
    return LayerTiles.get(layer["tileset"], lname == "bg0")


def room_palette_rgb(room):
    pal = []
    for c in room["palette"]:
        v = int(c, 16)
        pal += [(v & 31) << 3, (v >> 5 & 31) << 3, (v >> 10 & 31) << 3]
    return pal


def write_layer_png(path, room, lname, cells):
    """A BG layer's cells as its picture (see above), with the room's palette."""
    from PIL import Image

    layer = room["layers"][lname]
    tiles = layer_tiles(lname, layer)
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
                         % (path, x, y, v, layer["tileset"]))
            blocks.append(b)
        out += [pack(*row) for row in zip(*blocks)]
    img = Image.frombytes("P", (W * 8, h * CHUNK_H * 8), b"".join(out))
    img.putpalette(room_palette_rgb(room))
    img.save(path, optimize=True)


def _pixels(x, y):
    return "(pixels %d-%d, %d-%d)" % (x * 8, x * 8 + 7, y * 8, y * 8 + 7)


def read_layer_png(path, lname, layer):
    """A BG layer's picture -> its cells (row-major)."""
    from PIL import Image

    tiles = layer_tiles(lname, layer)
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
    pw = W * 8
    lookup = tiles.cells.get
    cells = []
    errors = []
    for y in range(H):
        blocks = list(zip(*[unpack(raw, (y * 8 + r) * pw) for r in range(8)]))
        row = list(map(lookup, blocks))
        if None in row:
            for x, v in enumerate(row):
                if v is None:
                    v, why = tiles.cell(blocks[x])
                    if why:
                        errors.append("%s: cell (%d, %d) %s %s" % (path, x, y, _pixels(x, y), why))
                    row[x] = v
        cells += row
    if errors:
        more = len(errors) - 20
        sys.exit("\n".join(errors[:20] + (["... and %d more cells" % more] if more > 0 else [])))
    return cells


def render(room_dir, lname, out_path):
    """A layer's picture: the BG layer's PNG drawn again from its cells, or
    the collision image."""
    room = read_json(Path(room_dir) / "room.json")
    layer = room["layers"][lname]
    cells = load_map(room_dir, lname, layer)
    if lname == "collision":
        # the collision image itself: the legend's colours, 8x8 pixels per cell
        collision_bmp.write_image(out_path, layer["size_chunks"][0] * CHUNK_W, cells)
        return
    write_layer_png(out_path, room, lname, cells)


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


def find_rooms(rom):
    """Room records of the level table region from ROM_TABLE, in table order: [(pal, desc)]."""
    out = []
    for a in range(ROOM_TABLE, ROOM_TABLE + ROOM_TABLE_SIZE, 4):
        if in_regions(rom.w(a)) and in_regions(rom.w(a + 4)) and not in_regions(rom.w(a - 4)):
            out.append((rom.w(a), rom.w(a + 4)))
    return out


def extract(rom_path):
    rom = Rom(rom_path)
    rooms = find_rooms(rom)
    assert len(rooms) == 41, len(rooms)
    LEVELS_DIR.mkdir(parents=True, exist_ok=True)
    pieces = []  # (addr, room name) for the ROM order of each region
    assets = []  # (asset addr, room name) for the ROM order of the assets
    type_names = entity_type_names()
    names = []
    for idx, (pal, desc) in enumerate(rooms):
        name = "room%02d_%06x" % (idx, desc - ROM_BASE)
        pre = "gRoom%02d" % idx
        names.append(name)
        room_dir = LEVELS_DIR / name
        room_dir.mkdir(exist_ok=True)
        L = [rom.w(desc + 4 * k) for k in range(9)]
        assert rom.bytes(desc + 0x19, 3) == b"\0\0\0" and not any(rom.bytes(desc + 0x24, 12))
        packed = rom.b(desc + 0x18)
        asset_addr = L[5]
        assets.append((asset_addr, name))
        if packed:
            # size from the LZ77 header; the stream length doesn't matter here
            asset = lz77_decompress(rom.bytes(asset_addr, 0x20000))
        else:
            asset = rom.bytes(asset_addr, RAW_ASSETS[asset_addr])
        ent_list = L[7]
        room = {
            "name": name,
            "palette": ["%04X" % rom.h(pal + 2 * i) for i in range(256)],
            "asset": {"symbol": pre + "Asset", "packed": bool(packed)},
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
        pieces.append((desc, name))
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
                write_layer_png(room_dir / fname, room, lname, cells)
        room["symbols"]["layers"] = pre + "Layers"
        assert sec_starts == sorted(sec_starts) and sec_starts[0] == 0
        # the pad bytes the encoder model can't predict (encoder heap garbage)
        for i, (lname, layer) in enumerate(room_layers(room)):
            off = sec_starts[i]
            end = sec_starts[i + 1] if i + 1 < len(sec_starts) else len(asset)
            # read back from the written sources, which checks their round trip
            chunks, _ = split_chunks(load_map(room_dir, lname, layer), *layer["size_chunks"])
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
        pieces.append((params, name))
    # regions, rooms in ROM order
    regions = []
    for rname, lo, hi in REGIONS:
        order = []
        for a, n in sorted(pieces):
            if lo <= a < hi and n not in order:
                order.append(n)
        regions.append({"name": rname, "start": "0x%08X" % lo, "end": "0x%08X" % hi, "rooms": order})
    aname, lo, hi = ASSET_REGION
    assert all(lo <= a < hi for a, _ in assets)
    levels = {"rooms": names, "regions": regions,
              "assets": {"name": aname, "start": "0x%08X" % lo, "end": "0x%08X" % hi,
                         "rooms": [n for _, n in sorted(assets)]}}
    old = LEVELS_DIR / "levels.json"
    if old.exists():
        # the level list isn't extracted: keep the hand-written one
        levels["levels"] = read_json(old).get("levels", [])
    (LEVELS_DIR / "levels.json").write_text(json.dumps(levels, indent=2) + "\n")
    return names


def write_room_json(path, room):
    """room.json with one entity/record/link per line."""
    def dump(v):
        return json.dumps(v, separators=(", ", ": "))

    lines = ["{"]
    keys = ["name", "asset", "symbols", "layers", "palette", "params", "links", "entities"]
    for i, k in enumerate(keys):
        comma = "," if i + 1 < len(keys) else ""
        v = room[k]
        if k == "layers":
            lines.append('  "layers": {')
            items = list(v.items())
            for j, (ln, lv) in enumerate(items):
                lines.append('    "%s": %s%s' % (ln, dump(lv), "," if j + 1 < len(items) else ""))
            lines.append("  }" + comma)
        elif k == "palette":
            lines.append('  "palette": [')
            for j in range(0, 256, 16):
                lines.append("    " + ", ".join('"%s"' % c for c in v[j:j + 16]) + ("," if j + 16 < 256 else ""))
            lines.append("  ]" + comma)
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
    elif cmd == "asset":
        asset, offsets, grids = build_asset(args[1], build_dir)
        Path(args[2]).write_bytes(asset)
        layout = Path(args[2]).with_name("asset.layout.json")
        layout.write_text(json.dumps({"offsets": offsets, "grids": grids}) + "\n")
    elif cmd == "c":
        region_c(args[1], args[2], build_dir)
    elif cmd == "assets":
        assets_c(args[1])
    elif cmd in ("from-png", "to-png"):
        room_dir, lname = args[1], args[2]
        room = read_json(Path(room_dir) / "room.json")
        layer = room["layers"].get(lname)
        if lname not in ("bg0", "bg1", "bg2", "bg3") or layer is None:
            sys.exit("%s has no BG layer %s" % (room_dir, lname))
        if cmd == "to-png":
            data = Path(args[3]).read_bytes()
            write_layer_png(args[4], room, lname, list(struct.unpack("<%dH" % (len(data) // 2), data)))
            return
        cells = read_layer_png(Path(room_dir) / layer["map"], lname, layer)
        out = Path(args[3])
        out.write_bytes(struct.pack("<%dH" % len(cells), *cells))
        # make's dependencies on the picture's tile set
        tiles = tileset_png(layer["tileset"]).as_posix()
        Path(str(out) + ".d").write_text("%s: %s\n%s:\n" % (out.as_posix(), tiles, tiles))
    elif cmd == "render":
        render(args[1], args[2], args[3])
    elif cmd == "constants":
        Path(args[2]).write_text(constants_h(args[1]))
    else:
        sys.exit("unknown command %s" % cmd)


if __name__ == "__main__":
    main()
