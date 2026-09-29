#!/usr/bin/env python3
"""The 41 rooms' level data: extract to editable sources, build back.

See docs/levels.md for the format. In short, a room is

- a `struct level_desc` (level_layers.c's view): up to five layers (BG1-3,
  the pooled BG0 and the collision layer), the level asset, the entity list
  and the entity links;
- per layer a `struct level_layer_desc` (bg_scroll_layer_25fc8.c's
  `struct bg_layer_desc`) and a grid of 16x8-cell chunk ids;
- the level asset: per layer a table of chunk offsets and one RLE/delta
  token stream per chunk (decoded by sub_8024960/sub_8025334). 34 rooms
  keep it LZ77-packed next to the intro graphics, 7 keep it raw;
- a 256-colour BG palette, the entity list (`{type, x, y, param}` spawn
  records in 256-px columns), the spawn parameter records and the
  per-type entity counts.

The editable sources live in data/levels/: levels.json (the two ROM
regions and which rooms they hold, in ROM order) and one directory per
room with room.json and one `<layer>.map.bin` per layer, the layer's
whole decoded tilemap (little-endian u16 cells, row-major, grit's flat
`-mLf` map layout). The chunk set, the chunk grid, the asset offsets, the
entity groups, the parameter offsets and the per-type counts are all
derived from those again.

    tools/levels.py extract [baserom.gba]     write data/levels/ from the ROM
    tools/levels.py asset ROOMDIR OUT         build a room's level asset
    tools/levels.py c REGION OUT              C definitions of one room region
                                              (REGION = its name in levels.json)
    tools/levels.py render ROOMDIR LAYER OUT  preview PNG of one layer, drawn with
                                              the room palette and the built tile
                                              sets (run make first)
"""
import json
import struct
import sys
import zlib
from pathlib import Path

ROM_BASE = 0x08000000
ROOM_TABLE = 0x0816CD80
ROOM_TABLE_SIZE = 0x474
REGIONS = [
    ("level_rooms_24b638", 0x0824B638, 0x08270F08),
    ("level_rooms_2b91d0", 0x082B91D0, 0x082BF120),
]
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


# ---------------------------------------------------------------------------
# Chunk streams (sub_8024960 / sub_8025334)

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


def load_map(room_dir, layer):
    data = (Path(room_dir) / layer["map"]).read_bytes()
    w, h = layer["size_chunks"]
    if len(data) != w * h * CHUNK_CELLS * 2:
        sys.exit("%s/%s: %d bytes, expected %d x %d chunks" % (room_dir, layer["map"], len(data), w, h))
    return list(struct.unpack("<%dH" % (len(data) // 2), data))


def room_layers(room):
    return [(name, room["layers"][name]) for name in LAYERS if name in room["layers"]]


def build_asset(room_dir):
    """The room's level asset (uncompressed) and each layer's offset in it.
    Sections are in slot order: BG1, BG2, BG3, BG0, collision."""
    room = read_json(Path(room_dir) / "room.json")
    asset = bytearray()
    offsets, grids = {}, {}
    for name, layer in room_layers(room):
        w, h = layer["size_chunks"]
        chunks, grid = split_chunks(load_map(room_dir, layer), w, h)
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
    column to the first (sub_80255D4's walk), which is also the order the
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


def room_c(room_dir):
    room = read_json(Path(room_dir) / "room.json")
    name = room["name"]
    sym = room["symbols"]
    asset, offsets, grids = build_asset(room_dir)
    out = []
    externs = set()

    def ext(s):
        if s:
            externs.add(s)
        return s or "NULL"

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
    out.append("const struct %s %s =\n{" % (stype, sym["params"]))
    for r in recs:
        out.append("    { " + ", ".join("%#x" % v for v in r) + " },")
    out.append("};\n")
    out.append("const u16 %s[%d] =\n{" % (sym["param_offsets"], len(recs)))
    for i in range(len(recs)):
        out.append("    LEVEL_PARAM_OFFSET(struct %s, r%d)," % (stype, i))
    out.append("};\n")

    # Chunk grids, one chunk row per line.
    for lname, layer in room_layers(room):
        g = grids[lname]
        w, h = layer["size_chunks"]
        out.append("/* %s chunk grid, %d x %d */" % (lname, w, h))
        out.append("const u16 %s[%d] =\n{" % (layer["grid_symbol"], len(g)))
        out.append(c_array(g, "%d", w))
        out.append("};\n")

    # BG palette.
    pal = [int(c, 16) for c in room["palette"]]
    if len(pal) != 256:
        sys.exit("%s: the palette needs 256 colours" % name)
    out.append("const u16 %s[256] =\n{" % sym["palette"])
    out.append(c_array(pal, "0x%04X", 8))
    out.append("};\n")

    # Per-type entity counts.
    counts = [0] * ENTITY_TYPES
    for e in room["entities"]:
        if not 0 <= e["type"] < ENTITY_TYPES:
            sys.exit("%s: entity type %d out of range" % (name, e["type"]))
        counts[e["type"]] += 1
    out.append("const u16 %s[%d] =\n{" % (sym["type_counts"], ENTITY_TYPES))
    out.append(c_array(counts, "%d", 16))
    out.append("};\n")

    # Entity links.
    links = room.get("links")
    if links is not None:
        out.append("const LEVEL_LINKS(%d) %s =\n{\n    %d,\n    {" % (len(links), sym["links"], len(links)))
        for i in range(0, len(links), 6):
            out.append("        " + " ".join("{ %d, %d }," % tuple(l) for l in links[i:i + 6]))
        out.append("    },\n};\n")

    # The level descriptor.
    layer_idx = {n: i for i, (n, _) in enumerate(room_layers(room))}

    def lref(n):
        return "&%s[%d]" % (sym["layers"], layer_idx[n]) if n in layer_idx else "NULL"

    a = room["asset"]
    out.append("const struct level_desc %s =\n{" % sym["desc"])
    out.append("    { %s, %s, %s }," % (lref("bg1"), lref("bg2"), lref("bg3")))
    out.append("    %s," % lref("bg0"))
    out.append("    %s," % lref("collision"))
    out.append("    %s," % ext(a["symbol"]))
    out.append("    %d," % (1 if a["packed"] else 0))
    out.append("    { 0, 0, 0 },")
    out.append("    &%s," % sym["entities"])
    out.append("    %s," % ("(const struct level_link_list *)&%s" % sym["links"] if links is not None else "NULL"))
    out.append("};\n")

    # Entity list header.
    groups = entity_groups(room)
    out.append("const struct level_entity_list %s =\n{" % sym["entities"])
    out.append("    %d, %d, %s, %s, (const u32 *)&%s, %s,"
               % (len(room["entities"]), len(groups), sym["groups"], sym["param_offsets"],
                  sym["params"], sym["type_counts"]))
    out.append("};\n")

    # Layer descriptors.
    out.append("const struct level_layer_desc %s[%d] =\n{" % (sym["layers"], len(layer_idx)))
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
    out.append("const struct level_entity_group %s[%d] =\n{" % (sym["groups"], len(groups)))
    for first, count, stored in groups:
        out.append("    { %d, %d, &%s[%d] }," % (first, count, sym["items"], stored))
    out.append("};\n")
    ents = room["entities"]
    out.append("/* In id order: type, x, y, param */")
    out.append("const struct level_entity %s[%d] =\n{" % (sym["items"], len(ents)))
    for i, e in enumerate(ents):
        out.append("    { %d, %d, %d, %d }, /* %d */" % (e["type"], e["x"], e["y"], e["param"], i))
    out.append("};\n")
    return externs, "\n".join(out) + "\n"


def region_c(region_name, out_path):
    levels = read_json(LEVELS_DIR / "levels.json")
    region = next((r for r in levels["regions"] if r["name"] == region_name), None)
    if region is None:
        sys.exit("levels.json has no region %s" % region_name)
    externs, bodies = set(), []
    for rname in region["rooms"]:
        e, body = room_c(LEVELS_DIR / rname)
        externs |= e
        bodies.append(body)
    text = ["/* Generated by tools/levels.py from data/levels/ - do not edit. */", ""]
    for s in sorted(externs):
        text.append("extern const u8 %s[];" % s)
    text.append("")
    text += bodies
    Path(out_path).write_text("\n".join(text))


# ---------------------------------------------------------------------------
# Preview rendering (not part of the build)

def find_tileset(symbol):
    """A layer's tile set as a list of 64-pixel tiles (colour indices),
    read from its source: graphics/level_tilesets/*.png (8bpp, indexed),
    graphics/tileset1/21-22 (4bpp PNGs, gbagfx grayscale: index = 15 -
    gray / 17) or graphics/tileset1/23-26 (raw 4bpp tiles)."""
    from PIL import Image

    addr = symbol[-6:].lower()
    for pat in ("graphics/level_tilesets/*_%s.png", "graphics/tileset1/*_%s_tiles.png"):
        hits = sorted(Path(".").glob(pat % addr))
        if hits:
            im = Image.open(hits[0])
            px = im.load()
            gray = im.mode == "L"
            tw = im.size[0] // 8
            tiles = []
            for t in range(tw * (im.size[1] // 8)):
                tx, ty = t % tw * 8, t // tw * 8
                tiles.append([15 - px[tx + x, ty + y] // 17 if gray else px[tx + x, ty + y]
                              for y in range(8) for x in range(8)])
            return tiles
    hits = sorted(Path(".").glob("graphics/tileset1/*_%s.bin" % addr))
    if hits:
        data = hits[0].read_bytes()
        return [[data[t * 32 + i // 2] >> 4 * (i & 1) & 0xF for i in range(64)]
                for t in range(len(data) // 32)]
    sys.exit("no tile set source for %s" % symbol)


def render(room_dir, lname, out_path):
    from PIL import Image

    room = read_json(Path(room_dir) / "room.json")
    layer = room["layers"][lname]
    cells = load_map(room_dir, layer)
    w, h = layer["size_chunks"]
    W, H = w * CHUNK_W, h * CHUNK_H
    img = Image.new("P", (W * 8, H * 8))
    pal = []
    for c in room["palette"]:
        v = int(c, 16)
        pal += [(v & 31) << 3, (v >> 5 & 31) << 3, (v >> 10 & 31) << 3]
    if lname == "collision":
        # terrain type (low byte) as a colour, types 1-0x23 are the
        # non-solid ones (sub_80250BC), the rest solid shapes (sub_8025130)
        pal = []
        for i in range(256):
            pal += [0, 0, 0] if i == 0 else ([64 + i * 5 % 192, 160, 64] if i <= 0x23 else
                                             [200, 64 + i * 7 % 192, 64 + i * 13 % 192])
        px = img.load()
        for y in range(H):
            for x in range(W):
                t = cells[y * W + x] & 0xFF
                for yy in range(8):
                    for xx in range(8):
                        px[x * 8 + xx, y * 8 + yy] = t
    else:
        tiles = find_tileset(layer["tileset"])
        px = img.load()
        for y in range(H):
            for x in range(W):
                v = cells[y * W + x]
                if lname == "bg0":  # tile-slot pool entry: 14-bit 8bpp tile, flips in bits 14-15
                    t, hf, vf, bank = v & 0x3FFF, v >> 14 & 1, v >> 15 & 1, None
                else:  # 4bpp BG screen entry
                    t, hf, vf, bank = v & 0x3FF, v >> 10 & 1, v >> 11 & 1, v >> 12
                if t >= len(tiles):
                    continue
                data = tiles[t]
                for yy in range(8):
                    for xx in range(8):
                        c = data[yy * 8 + xx]
                        if bank is not None and c:
                            c += bank * 16
                        px[x * 8 + (7 - xx if hf else xx), y * 8 + (7 - yy if vf else yy)] = c
    img.putpalette(pal)
    img.save(out_path)


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


def find_rooms(rom):
    """Room records of gStaticData_0816CD80, in table order: [(pal, desc)]."""
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
    names = []
    for idx, (pal, desc) in enumerate(rooms):
        name = "room%02d_%06x" % (idx, desc - ROM_BASE)
        names.append(name)
        room_dir = LEVELS_DIR / name
        room_dir.mkdir(exist_ok=True)
        L = [rom.w(desc + 4 * k) for k in range(9)]
        assert rom.bytes(desc + 0x19, 3) == b"\0\0\0" and not any(rom.bytes(desc + 0x24, 12))
        packed = rom.b(desc + 0x18)
        asset_addr = L[5]
        if packed:
            # size from the LZ77 header; the stream length doesn't matter here
            asset = lz77_decompress(rom.bytes(asset_addr, 0x20000))
        else:
            asset = rom.bytes(asset_addr, RAW_ASSETS[asset_addr])
        ent_list = L[7]
        room = {
            "name": name,
            "palette": ["%04X" % rom.h(pal + 2 * i) for i in range(256)],
            "asset": {"symbol": sym(asset_addr), "packed": bool(packed)},
            "layers": {},
            "entities": [],
            "params": [],
            "links": None,
            "symbols": {
                "desc": sym(desc), "palette": sym(pal), "entities": sym(ent_list),
                "groups": sym(rom.w(ent_list + 4)), "param_offsets": sym(rom.w(ent_list + 8)),
                "params": sym(rom.w(ent_list + 12)), "type_counts": sym(rom.w(ent_list + 16)),
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
            fname = "%s.map.bin" % lname
            (room_dir / fname).write_bytes(struct.pack("<%dH" % len(cells), *cells))
            layer = {
                "map": fname,
                "size_chunks": [cw, ch],
                "size_tiles": [rom.h(p + 0x1A), rom.h(p + 0x1C)],
                "tileset": sym(tiles) if tiles else None,
                "scale": [struct.unpack_from("<i", rom.data, p + 0xC - ROM_BASE)[0],
                          struct.unpack_from("<i", rom.data, p + 0x10 - ROM_BASE)[0]],
                "cnt": rom.h(p + 0x14),
                "unk_1E": rom.h(p + 0x1E),
                "grid_symbol": sym(grid_addr),
            }
            room["layers"][lname] = layer
        room["symbols"]["layers"] = sym(first_ld)
        assert sec_starts == sorted(sec_starts) and sec_starts[0] == 0
        # the pad bytes the encoder model can't predict (encoder heap garbage)
        for i, (lname, layer) in enumerate(room_layers(room)):
            off = sec_starts[i]
            end = sec_starts[i + 1] if i + 1 < len(sec_starts) else len(asset)
            chunks, _ = split_chunks(load_map(room_dir, layer), *layer["size_chunks"])
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
                room["entities"].append({"type": t, "x": x, "y": y, "param": prm})
        room["symbols"]["items"] = sym(min(rom.w(groups + 8 * g + 4) for g in range(ngroups)))
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
            room["symbols"]["links"] = sym(L[8])
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
    levels = {"rooms": names, "regions": regions}
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


def main():
    args = sys.argv[1:]
    if not args or args[0] in ("-h", "--help"):
        print(__doc__)
        return
    cmd = args[0]
    if cmd == "extract":
        extract(args[1] if len(args) > 1 else "baserom.gba")
    elif cmd == "asset":
        asset, _, _ = build_asset(args[1])
        Path(args[2]).write_bytes(asset)
    elif cmd == "c":
        region_c(args[1], args[2])
    elif cmd == "render":
        render(args[1], args[2], args[3])
    else:
        sys.exit("unknown command %s" % cmd)


if __name__ == "__main__":
    main()
