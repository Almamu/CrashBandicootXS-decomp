"""crashdata.levels.metatiles (docs/editors.md, "Meta-tiles"): the blocks
detect() finds in the real tile sets are where it says, in the flips and
banks it says, and are maximal; a meta-tile stamps the cells it names;
the files' text reads back as their value.

    python3 -m unittest discover -s packages/crashdata/tests
"""
import json
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "packages" / "crashdata" / "src"))

from crashdata import levels as L  # noqa: E402
from crashdata import schemas  # noqa: E402
from crashdata.common.game import Game  # noqa: E402
from crashdata.levels import metatiles as M  # noqa: E402


def built(game, symbol):
    """A set's tiles (64 pixel bytes each), bpp and {room/layer: (width, cells)}."""
    L.use_game(Game(game, ROOT))
    levels = L.read_json(L.LEVELS_JSON)
    users = L.tileset_users(levels)
    tiles, _, maps = L.generate_tileset(symbol, levels, users)
    out = {}
    for (room, lname), cells in maps.items():
        W = L.read_json(L.LEVELS_DIR / room / "room.json")["layers"][lname]["size_chunks"][0] * L.CHUNK_W
        out["%s/%s" % (room, lname)] = (W, cells)
    return [M.block_bytes(t) for t in tiles], users[symbol]["bpp"], out


def drawn(tiles, bpp, layer, x, y):
    """A layer's cell as the picture draws it (64 indices), or None blank."""
    W, cells = layer
    v = cells[y * W + x]
    shift = 14 if bpp == 8 else 10
    t, f = v & ((1 << shift) - 1), v >> shift & 3
    if not t:
        return None
    add = (v >> 12) * 16 if bpp == 4 else 0
    return bytes(p + add for p in M.flipped(tiles[t], f))


class Detection(unittest.TestCase):
    def check_set(self, game, symbol):
        tiles, bpp, layers = built(game, symbol)
        r = M.detect(symbol)
        self.assertEqual((r["tileset"], r["bpp"]), (symbol, bpp))
        self.assertTrue(r["candidates"], symbol)
        doc = {"tiles": [M.tile_hex(t, bpp) for t in tiles], "metatiles": []}
        scores = [c["score"] for c in r["candidates"]]
        self.assertEqual(scores, sorted(scores, reverse=True))
        for k, c in enumerate(r["candidates"]):
            w, h = c["size"]
            self.assertEqual(len(c["cells"]), w * h)
            self.assertGreaterEqual(c["count"], 2)
            self.assertEqual(sum(c["rooms"].values()), c["count"])
            self.assertEqual(len(c["at"]), min(c["count"], 200))
            self.assertGreater(len({(t, f) for t, f, _ in c["cells"] if t}), 1, "a fill isn't offered")
            doc["metatiles"].append({"name": "c%d" % k, "size": c["size"], "cells": c["cells"]})
            sides = [set(), set(), set(), set()]
            for name, x, y, flip, shift in c["at"]:
                # the repeat is the block, flipped as a whole by `flip`, its banks shifted by `shift`
                for i, j, px in M.stamp(doc, k, flip, shift):
                    got = drawn(tiles, bpp, layers[name], x + i, y + j)
                    if got is None:
                        self.assertFalse(any(p & 15 if bpp == 4 else p for p in px), (symbol, k, name, x, y))
                    else:
                        self.assertEqual(got, px, (symbol, k, name, x, y, i, j))
                # what is next to it, in the block's own orientation
                W, cells = layers[name]
                H = len(cells) // W
                for side, (dx, dy, n) in enumerate([(w, 0, h), (0, h, w), (-1, 0, h), (0, -1, w)]):
                    strip = []
                    for q in range(n):
                        i, j = (dx, q) if side in (0, 2) else (q, dy)
                        i = w - 1 - i if flip & 1 else i
                        j = h - 1 - j if flip & 2 else j
                        X, Y = x + i, y + j
                        strip.append(drawn(tiles, bpp, layers[name], X, Y) if 0 <= X < W and 0 <= Y < H else "out")
                    sides[side].add(tuple(strip))
            if c["count"] == len(c["at"]):
                for side, seen in enumerate(sides):
                    # maximal: its repeats don't all go on the same way on a side
                    # (a side of blanks, or outside, ends a block)
                    only = next(iter(seen))
                    self.assertTrue(len(seen) > 1 or "out" in only or all(p is None for p in only),
                                    (symbol, k, side))
        return r

    def test_xs_sets(self):
        r = self.check_set("xs", "gUnderwaterBgTiles")
        self.assertTrue(any(c["size"][0] * c["size"][1] >= 16 for c in r["candidates"]))
        r = self.check_set("xs", "gSnowBgTiles")
        self.assertTrue(any(c["flipped"] for c in r["candidates"]), "flipped repeats are the same meta-tile")

    def test_an_n_tranced_set(self):
        self.check_set("n2", "gIslandBgTiles")

    def test_a_room(self):
        L.use_game(Game("xs", ROOT))
        r = M.detect("gUnderwaterBgTiles", rooms={"air_supply/main"})
        self.assertTrue(r["candidates"])
        for c in r["candidates"]:
            self.assertEqual({n.rsplit("/", 1)[0] for n in c["rooms"]}, {"air_supply/main"})
        with self.assertRaises(SystemExit):
            M.detect("gUnderwaterBgTiles", rooms={"snow_crash/part1"})


class Files(unittest.TestCase):
    def doc(self):
        return {"metatiles": [{"name": "ledge", "size": [2, 1], "cells": [[1, 0, 3], [1, 1, 3]]},
                              {"name": "post", "size": [1, 2], "cells": [[0, 0, 2], [1, 2, 2]]}],
                "tiles": ["0" * 64, "".join("%x" % (i % 16) for i in range(64))]}

    def test_text_and_schema(self):
        doc = self.doc()
        text = M.text(doc)
        self.assertEqual(json.loads(text), doc)
        self.assertEqual(json.loads(M.text({"metatiles": [], "tiles": []})), {"metatiles": [], "tiles": []})
        rel = "games/xs/data/levels/metatiles/gSnowBgTiles.json"
        self.assertEqual(schemas.schema_for(rel), "metatiles.schema.json")
        from crashdata.common import schema as S
        self.assertEqual(S.validate(schemas.get("metatiles.schema.json"), doc), [])
        bad = self.doc()
        bad["metatiles"][0]["size"] = [0, 1]  # a meta-tile is a cell at least
        self.assertTrue(S.validate(schemas.get("metatiles.schema.json"), bad))

    def test_stamp(self):
        doc = self.doc()
        px = bytes(i % 16 for i in range(64))
        cells = M.stamp(doc, 0)
        self.assertEqual(cells, [(0, 0, bytes(p + 48 for p in px)), (1, 0, bytes(p + 48 for p in M.flipped(px, 1)))])
        # flipped as a whole: the cells swap places and each flips too; shifted: every bank
        cells = M.stamp(doc, 0, flip=1, shift=2)
        self.assertEqual(cells, [(1, 0, bytes(p + 80 for p in M.flipped(px, 1))), (0, 0, bytes(p + 80 for p in px))])
        for _, _, block in M.stamp(doc, 1, flip=2, shift=-2):
            self.assertEqual({p >> 4 for p in block}, {0}, "a cell's 64 pixels are of one bank")
        with self.assertRaises(ValueError):
            M.stamp(doc, 0, shift=13)


if __name__ == "__main__":
    unittest.main()
