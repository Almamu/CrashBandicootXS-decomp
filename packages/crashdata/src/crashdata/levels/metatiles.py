"""Meta-tiles: reusable blocks of a BG tile set's cells, for the level editor (#1167, docs/editors.md "Meta-tiles").

A meta-tile is a block of W x H cells of one BG tile set, each cell a tile,
its flips and (BG1-3) its palette bank: a brush the level editor stamps
onto a room's BG layer. The game has no such thing: a layer's cells are
copied straight into the screen block (BgLayer::DrawRow), and the only
block the level data has is the chunk, 16x8 cells that the build cuts
every layer into and keeps once per layer (docs/levels.md, "The level
asset"), which no other layer or room shares. So the meta-tiles are the
editor's own data, a file per tile set,

    games/<game>/data/levels/metatiles/<tile set>.json

(metatiles.schema.json), which nothing of the build reads: the ROM is the
same with or without them, and the save version's CRC (level_data_crc)
doesn't hash them. A stamped meta-tile is cells of the layer's picture,
which the build reads back like any other.

The file is
    {"tiles": [64 pixels as hex, ...],
     "metatiles": [{"name", "size": [w, h], "cells": [[tile, flip, bank], ...]}, ...]}
- "tiles": the pixels of the tiles the meta-tiles draw, as the set has them
  (unflipped): a hex digit a pixel for a BG1-3 set (0-15, without the
  bank), two for a BG0 set. A meta-tile names tiles by their pixels, not
  by their number in the set, which a new tile in an earlier room changes
  (docs/levels.md, "Tile sets"); the editor finds each one's number again;
- a meta-tile's "cells", row by row: [an index into "tiles", the flip (1
  X, 2 Y, as a cell's bits), the palette bank (0 on a BG0 set)].

detect() finds the blocks that recur in the set's rooms (see there).
"""
import heapq
from collections import Counter

from crashdata import levels as L

DIR_NAME = "metatiles"
# the largest meta-tile a file takes, and the largest detect() looks for
MAX_SIZE = 32
DETECT_MAX = 16
DETECT_LIMIT = 48


def path_for(symbol):
    """The meta-tiles file of a tile set."""
    return L.LEVELS_DIR / DIR_NAME / ("%s.json" % symbol)


def tile_hex(block, bpp):
    """A tile's 64 pixels (bytes; 4bpp: 0-15) as the file's hex."""
    return "".join(("%x" if bpp == 4 else "%02x") % p for p in block)


def hex_tile(text):
    """The file's hex back to 64 pixels: (bytes, bpp)."""
    if len(text) == 64:
        return bytes(int(c, 16) for c in text), 4
    return bytes.fromhex(text), 8


def block_bytes(tile):
    """A generate_tileset tile (8 row ints) as 64 pixel bytes."""
    return b"".join(r.to_bytes(8, "little") for r in tile)


def flipped(px, f):
    """64 pixels under flip f (1 X, 2 Y)."""
    rows = [px[y * 8:(y + 1) * 8] for y in range(8)]
    if f & 1:
        rows = [r[::-1] for r in rows]
    if f & 2:
        rows = rows[::-1]
    return b"".join(rows)


def stamp(doc, k, flip=0, shift=0):
    """Meta-tile `k` of a meta-tiles file as the cells it draws: [(x, y, 64
    palette indices)], the meta-tile flipped as a whole by `flip` and its
    banks shifted by `shift` (what the editor's stamp writes into the
    layer's picture: BG1-3's pixels `bank * 16 + p`, all 64 of a cell from
    its one bank)."""
    m = doc["metatiles"][k]
    w, h = m["size"]
    out = []
    for j in range(h):
        for i in range(w):
            t, f, bank = m["cells"][j * w + i]
            px, bpp = hex_tile(doc["tiles"][t])
            bank += shift
            if bpp == 4 and not 0 <= bank < 16:
                raise ValueError("bank %d: a cell's bank is 0-15" % bank)
            add = bank * 16 if bpp == 4 else 0
            x = w - 1 - i if flip & 1 else i
            y = h - 1 - j if flip & 2 else j
            out.append((x, y, bytes(p + add for p in flipped(px, f ^ flip))))
    return out


# ---------------------------------------------------------------------------
# The file

def text(doc):
    """A meta-tiles file's text: a meta-tile a few lines, its cells a row a
    line, a tile a line."""
    import json

    def j(v):
        return json.dumps(v, ensure_ascii=False)

    lines = ["{"]
    lines.append(' "metatiles": [' if doc.get("metatiles") else ' "metatiles": [],')
    for n, m in enumerate(doc.get("metatiles") or []):
        w = m["size"][0]
        head = ["%s: %s" % (j(key), j(m[key])) for key in m if key != "cells"]
        lines.append("  {%s, \"cells\": [" % ", ".join(head))
        rows = [m["cells"][y:y + w] for y in range(0, len(m["cells"]), w)]
        for r, row in enumerate(rows):
            lines.append("   " + ", ".join(j(c) for c in row) + ("," if r < len(rows) - 1 else ""))
        lines.append("  ]}" + ("," if n < len(doc["metatiles"]) - 1 else ""))
    if doc.get("metatiles"):
        lines.append(" ],")
    tiles = doc.get("tiles") or []
    lines.append(' "tiles": [' if tiles else ' "tiles": []')
    for n, t in enumerate(tiles):
        lines.append("  %s%s" % (j(t), "," if n < len(tiles) - 1 else ""))
    if tiles:
        lines.append(" ]")
    lines.append("}")
    return "\n".join(lines) + "\n"


def check(sets=None):
    """data-check's: each meta-tiles file names a tile set the rooms use,
    its tiles are of that set's depth, its meta-tiles' cells are their
    size, name its tiles and have a flip of 0-3 and a bank of 0-15 (0 in a
    BG0 set); the schema has the rest. `sets`: tileset_users' (read from levels.json if not
    given)."""
    d = L.LEVELS_DIR / DIR_NAME
    if not d.is_dir():
        return
    if sets is None:
        sets = L.tileset_users(L.read_json(L.LEVELS_JSON))
    errors = []
    for p in sorted(d.glob("*.json")):
        doc = L.read_json(p)
        symbol = p.stem
        if symbol not in sets:
            errors.append("%s: no room's layer names the tile set %s" % (p, symbol))
            continue
        bpp = sets[symbol]["bpp"]
        for n, t in enumerate(doc["tiles"]):
            if len(t) != (64 if bpp == 4 else 128):
                errors.append("%s: /tiles/%d: %s is a %dbpp set: a tile is %d hex digits"
                              % (p, n, symbol, bpp, 64 if bpp == 4 else 128))
        for k, m in enumerate(doc["metatiles"]):
            w, h = m["size"]
            if len(m["cells"]) != w * h:
                errors.append("%s: /metatiles/%d: %d cells, a %dx%d meta-tile has %d" % (p, k, len(m["cells"]),
                                                                                         w, h, w * h))
            for c, (t, flip, bank) in enumerate(m["cells"]):
                if flip > 3 or bank > 15:
                    errors.append("%s: /metatiles/%d/cells/%d: flip %d, bank %d: a flip is 0-3 (1 X, 2 Y), a bank "
                                  "0-15" % (p, k, c, flip, bank))
                if t >= len(doc["tiles"]):
                    errors.append("%s: /metatiles/%d/cells/%d: tile %d, the file has %d" % (p, k, c, t,
                                                                                           len(doc["tiles"])))
                if bpp == 8 and bank:
                    errors.append("%s: /metatiles/%d/cells/%d: bank %d, a BG0 cell has none (0)" % (p, k, c, bank))
    if errors:
        raise SystemExit("\n".join(errors))


# ---------------------------------------------------------------------------
# Detection

def detect(symbol, rooms=None, max_size=DETECT_MAX, limit=DETECT_LIMIT, levels=None):
    """The blocks of cells, up to max_size x max_size, that recur in the
    rooms whose layers name the tile set `symbol` (or in those of `rooms`,
    room folders), as the build reads their pictures: the meta-tiles to
    offer, best first.

    - A block is found again flipped as a whole (each of its cells'
      flips too), and with all of its banks shifted by one amount: a
      repeat in another orientation or in other banks is the same
      meta-tile, stamped flipped or in other banks.
    - Only maximal blocks: one whose every repeat goes on with the same
      cells on a side is part of a bigger block, which is offered instead.
      A block's edges aren't blank strips, and one that is mostly a fill
      of one tile isn't offered. The search takes shortcuts (see the
      loop below), so it is a good many of them, not every one.
    - Layers that are the same picture (a room copied) are searched once;
      their repeats count in each room.
    - Ranked by the cells its repeats draw beyond its first one (its
      repeats that don't overlap, times its cells that aren't blank), then
      by size; a block whose repeats mostly lie on a better one's is left
      out; `limit` at most.

    {"tileset", "bpp", "candidates": [{"size": [w, h], "cells": [[tile,
    flip, bank], ...] (row by row, the set's tile numbers: the block as its
    most common orientation and banks draw it), "count": its repeats,
    "flipped", "shifted": how many of them are flipped, in other banks,
    "rooms": {"<room>/<layer>": repeats}, "at": [["<room>/<layer>", x, y,
    flip, bank shift], ...] (the repeats' top left cells, up to 200),
    "score"}]}."""
    levels = levels if levels is not None else L.read_json(L.LEVELS_JSON)
    users = L.tileset_users(levels)
    if symbol not in users:
        raise SystemExit("no room's layer names the tile set %s" % symbol)
    tiles, _, maps = L.generate_tileset(symbol, levels, users)
    if rooms is not None and not any(r in rooms for r, _ in maps):
        raise SystemExit("none of %s names the tile set %s" % (", ".join(sorted(rooms)), symbol))
    bpp = users[symbol]["bpp"]
    bg0 = bpp == 8
    shift = 14 if bg0 else 10
    mask = (1 << shift) - 1

    # each tile's flips that draw it the same (a symmetric tile's cells are
    # read unflipped), and each flip's canonical form for the tile
    sym = []
    for t in tiles:
        fl = L.flips(t)
        sym.append([g for g in range(4) if fl[g] == t])
    canon = [[min(f ^ g for g in s) for f in range(4)] for s in sym]

    # the layers, each picture once: names, width, height, cells (tile,
    # flip, bank), and the cells as codes (0 blank, else tile << 2 | the
    # flip) in the four frames, each the layer flipped by f: rows[f][y] and
    # cols[f][x], tuples, so a block's edge is a slice
    layers, same = [], {}
    for (room, lname), cells in maps.items():
        if rooms is not None and room not in rooms:
            continue
        layer = L.read_json(L.LEVELS_DIR / room / "room.json")["layers"][lname]
        W = layer["size_chunks"][0] * L.CHUNK_W
        key = (W, tuple(cells))
        if key in same:
            layers[same[key]]["names"].append("%s/%s" % (room, lname))
            continue
        same[key] = len(layers)
        H = len(cells) // W
        tl = [v & mask for v in cells]
        fl = [v >> shift & 3 for v in cells]
        bk = [-1 if not t else 0 if bg0 else v >> 12 for t, v in zip(tl, cells)]
        rows, cols, banks = [], [], []
        for f in range(4):
            code = [(t << 2 | canon[t][x ^ f]) if t else 0 for t, x in zip(tl, fl)]
            r = [tuple(code[y * W:(y + 1) * W]) for y in range(H)]
            b = [tuple(bk[y * W:(y + 1) * W]) for y in range(H)]
            if f & 1:
                r = [x[::-1] for x in r]
                b = [x[::-1] for x in b]
            if f & 2:
                r = r[::-1]
                b = b[::-1]
            rows.append(r)
            cols.append(list(zip(*r)))
            banks.append(b)
        layers.append({"names": ["%s/%s" % (room, lname)], "W": W, "H": H, "cells": cells, "tl": tl, "fl": fl,
                       "rows": rows, "cols": cols, "banks": banks})

    def rect(o, w, h):
        """A repeat's (layer, x, y): its top left cell in the layer."""
        g, f, fx, fy = o
        lay = layers[g]
        return g, (lay["W"] - fx - w if f & 1 else fx), (lay["H"] - fy - h if f & 2 else fy)

    def edge(o, w, h, side):
        """The cells next to a repeat (layer, frame, x, y in the frame) of a
        w x h block, on a side of it in its frame (0 right, 1 down, 2 left,
        3 up): a tuple of codes, or None outside the layer."""
        g, f, fx, fy = o
        lay = layers[g]
        if side == 0:
            return lay["cols"][f][fx + w][fy:fy + h] if fx + w < lay["W"] else None
        if side == 1:
            return lay["rows"][f][fy + h][fx:fx + w] if fy + h < lay["H"] else None
        if side == 2:
            return lay["cols"][f][fx - 1][fy:fy + h] if fx > 0 else None
        return lay["rows"][f][fy - 1][fx:fx + w] if fy > 0 else None

    def grow(occs, w, h, side):
        """{the cells a block grows by on a side (0 a column, 1 a row): its
        repeats that grow so}, blank ones left out."""
        out = {}
        for o in occs:
            s = edge(o, w, h, side)
            if s is not None and any(s):
                out.setdefault(s, []).append(o)
        return out

    def recurs(occs, w, h):
        """Whether a block's repeats are at two places at least."""
        first = None
        for o in occs:
            r = rect(o, w, h)
            if first is None:
                first = r
            elif r != first:
                return True
        return False

    def apart(items, w, h):
        """The repeats ((rect, ...) items) that don't overlap, layer by
        layer in reading order. A kept repeat's top left cell is the only
        one in its w x h bucket of the layer, so a repeat can only overlap
        those of the 3 x 3 buckets about its own."""
        keep, kept = [], {}
        for item in sorted(items, key=lambda k: (k[0][0], k[0][2], k[0][1])):
            g, x0, y0 = item[0]
            bx, by = x0 // w, y0 // h
            for nx in (bx - 1, bx, bx + 1):
                for ny in (by - 1, by, by + 1):
                    k = kept.get((g, nx, ny))
                    if k is not None and abs(k[0] - x0) < w and abs(k[1] - y0) < h:
                        break
                else:
                    continue
                break
            else:
                kept[(g, bx, by)] = (x0, y0)
                keep.append(item)
        return keep

    def same_side(uniq, w, h, side):
        """Whether every repeat goes on with the same cells (not all blank)
        on a side."""
        first = edge(uniq[0][1], w, h, side)
        if first is None or not any(first):
            return False
        return all(edge(o, w, h, side) == first for _, o in uniq[1:])

    found, best = {}, []
    # the best scores so far: a few times the blocks offered, as some are
    # left out for lying on better ones
    BEST = limit * 20

    def consider(uniq, w, h, block):
        """A block whose repeats are `uniq` ([(rect, repeat)], each place
        once) and go on the same way on no side, as a candidate unless it
        is a cell or mostly a fill of one tile (a quarter of its cells, or
        all but one, are another's)."""
        if w * h < 2:
            return
        tally = Counter(c for c in block if c)
        filled = sum(tally.values())
        if len(tally) < 2 or filled - max(tally.values()) < max(1, w * h // 4):
            return
        # its score is at most its repeats' (overlapping ones too): one that
        # can't beat the blocks kept so far isn't looked at further
        if len(best) == BEST and (sum(len(layers[r[0]]["names"]) for r, _ in uniq) - 1) * filled <= best[0]:
            return
        # by their banks, shifted to the first cell that isn't blank's
        by_banks = {}
        for r, o in uniq:
            g, f, fx, fy = o
            b = layers[g]["banks"][f]
            raw = [x for j in range(h) for x in b[fy + j][fx:fx + w]]
            d = next(x for x in raw if x >= 0)
            by_banks.setdefault(tuple(x - d if x >= 0 else -1 for x in raw), []).append((r, o, d))
        for group in by_banks.values():
            keep = apart(group, w, h)
            if len(keep) < 2:
                continue
            if any(same_side([(r, o) for r, o, _ in keep], w, h, side) for side in range(4)):
                continue  # in these banks, or apart, it is part of a bigger block
            count = sum(len(layers[r[0]]["names"]) for r, _, _ in keep)
            key = (w, h, frozenset(r for r, _, _ in keep))
            if key not in found:  # the same block from another corner
                found[key] = (keep, w, h, filled, count)
                score = (count - 1) * filled
                if len(best) < BEST:
                    heapq.heappush(best, score)
                elif score > best[0]:
                    heapq.heapreplace(best, score)

    # a block grows from a cell (a tile, under any flip, in any bank) to the
    # right while it is a row tall, then down: each block in each frame is
    # reached once. A block whose repeats mostly overlap each other is a
    # piece of a pattern that repeats (a wall, a sky): the blocks it grows
    # into repeat less, each piece of the pattern is offered, not the bigger
    # blocks of it
    seeds = {}
    for g, lay in enumerate(layers):
        W, H = lay["W"], lay["H"]
        for i, t in enumerate(lay["tl"]):
            if t:
                x, y = i % W, i // W
                for s in sym[t]:
                    f = lay["fl"][i] ^ s
                    seeds.setdefault(t, []).append((g, f, W - 1 - x if f & 1 else x, H - 1 - y if f & 2 else y))
    stack = [(occs, 1, 1, (t << 2,)) for t, occs in seeds.items() if recurs(occs, 1, 1)]
    while stack:
        occs, w, h, block = stack.pop()
        seen, uniq = set(), []
        for o in occs:
            r = rect(o, w, h)
            if r not in seen:
                seen.add(r)
                uniq.append((r, o))
        # a block whose every repeat has the same cells above it grows down
        # into blocks that all do too, and to the right into rows that do:
        # none of them is a candidate. One whose every repeat has the same
        # cells left of it would mostly grow into such blocks too, found
        # from the bigger block's own corner: it isn't grown (a heuristic:
        # a block whose top part always has the same cells on its left and
        # its bottom part doesn't is found only in another orientation)
        if same_side(uniq, w, h, 3) or same_side(uniq, w, h, 2):
            continue
        right = same_side(uniq, w, h, 0)
        if not right and not same_side(uniq, w, h, 1):
            consider(uniq, w, h, block)
        sample = uniq[:256]  # in reading order: enough to see a pattern
        if len(apart(sample, w, h)) * 5 < len(sample) * 3:
            continue
        if h == 1 and w < max_size:
            for s, more in grow(occs, w, h, 0).items():
                if recurs(more, w + 1, h):
                    stack.append((more, w + 1, 1, block + s))
        # a row whose every repeat goes on the same way to the right grows
        # right, not down: the wider row grows down
        if h < max_size and not (h == 1 and right and w < max_size):
            for s, more in grow(occs, w, h, 1).items():
                if recurs(more, w, h + 1):
                    stack.append((more, w, h + 1, block + s))

    ranked = sorted(found.values(), key=lambda x: (-(x[4] - 1) * x[3], -x[1] * x[2], -x[4]))
    out, covered = [], set()
    for keep, w, h, filled, count in ranked:
        cells = set()
        for (g, x0, y0), _, _ in keep:
            cells.update((g, x0 + i, y0 + j) for j in range(h) for i in range(w))
        if len(cells - covered) * 2 < len(cells):
            continue  # mostly on a better block's repeats
        covered |= cells
        # the block as its most common orientation and banks draw it
        frames = Counter((o[1], d) for _, o, d in keep)
        (f0, d0), _ = frames.most_common(1)[0]
        (g, x0, y0), _, _ = next(k for k in keep if (k[1][1], k[2]) == (f0, d0))
        lay = layers[g]
        W = lay["W"]
        block = [[lay["tl"][i], lay["fl"][i], 0 if bg0 else lay["cells"][i] >> 12]
                 for i in ((y0 + j) * W + x0 + c for j in range(h) for c in range(w))]
        rooms, at = Counter(), []
        for (g, x, y), o, d in keep:
            for name in layers[g]["names"]:
                rooms[name] += 1
                if len(at) < 200:
                    at.append([name, x, y, o[1] ^ f0, d - d0])
        out.append({
            "size": [w, h], "cells": block, "count": count,
            "flipped": sum(len(layers[k[0][0]]["names"]) for k in keep if k[1][1] != f0),
            "shifted": sum(len(layers[k[0][0]]["names"]) for k in keep if k[2] != d0),
            "rooms": dict(rooms), "at": at, "score": (count - 1) * filled,
        })
        if len(out) >= limit:
            break
    return {"tileset": symbol, "bpp": bpp, "candidates": out}


__all__ = ["DETECT_LIMIT", "DETECT_MAX", "DIR_NAME", "MAX_SIZE", "block_bytes", "check", "detect", "flipped",
           "hex_tile", "path_for", "stamp", "text", "tile_hex"]
