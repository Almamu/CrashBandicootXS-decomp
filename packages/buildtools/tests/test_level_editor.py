"""The level editor's backend (docs/editors.md, "The level editor"): the
room API of buildtools.editor_server over crashdata.levels.room_edit,
started on a free port of 127.0.0.1 against this tree. A test that
changes a file puts it back.

    python3 -m unittest discover -s packages/buildtools/tests
"""
import base64
import gzip
import io
import json
import struct
import sys
import unittest
import urllib.error
import urllib.request
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
for _p in ("crashdata", "buildtools"):
    sys.path.insert(0, str(ROOT / "packages" / _p / "src"))

from test_editor_server import Backend  # noqa: E402

ROOM = "games/xs/data/levels/jungle_jam/main"


def rooms(game):
    levels = json.loads((ROOT / "games" / game / "data/levels/levels.json").read_text())
    return ["games/%s/data/levels/%s" % (game, r) for r in levels["rooms"]]


class LevelEditor(Backend):
    def get_bytes(self, path, gz=True):
        h = {"Accept-Encoding": "gzip"} if gz else {}
        req = urllib.request.Request("http://127.0.0.1:%d%s" % (self.port, path), headers=h)
        try:
            with urllib.request.urlopen(req, timeout=600) as r:
                data = r.read()
                if r.headers.get("Content-Encoding") == "gzip":
                    data = gzip.decompress(data)
                return r.status, r.headers.get("Content-Type"), data
        except urllib.error.HTTPError as e:
            with e:
                return e.code, e.headers.get("Content-Type"), e.read()

    def layer(self, game, room, layer):
        st, _, data = self.get_bytes("/api/projects/%s/room/layer?path=%s&layer=%s" % (game, room, layer))
        self.assertEqual(st, 200, data[:300])
        w, h = struct.unpack_from("<II", data)
        return w, h, data[8:]


class Reading(LevelEditor):
    def test_a_room(self):
        st, v = self.call("GET", "/api/projects/xs/room?path=" + ROOM)
        self.assertEqual(st, 200, v)
        self.assertEqual(v["room"], json.loads((ROOT / ROOM / "room.json").read_text()))
        self.assertEqual(len(v["palette"]), 256)
        self.assertEqual(sorted(v["layers"]), ["bg0", "bg1", "bg2", "bg3", "collision"])
        bg1 = v["layers"]["bg1"]
        self.assertEqual((bg1["width"], bg1["height"], bg1["priority"], bg1["scale"], bg1["bpp"]),
                         (26 * 128, 8 * 64, 3, [128, 128], 4))
        self.assertIn({"value": 0x2B, "colour": "#C8C8C8", "name": "solid 0x2B"}, v["legend"])
        crate = next(t["type"] for t in v["entityTypes"] if t["name"] == "BASIC_CRATE")
        self.assertEqual(v["sprites"][str(crate)]["bank"], 31)
        lizard = next(t["type"] for t in v["entityTypes"] if t["name"] == "LIZARD")
        self.assertEqual(v["sprites"][str(lizard)]["bank"], 13)  # its enemy kind

    def test_an_n_tranced_room(self):
        st, v = self.call("GET", "/api/projects/n2/room?path=games/n2/data/levels/island_intro/main")
        self.assertEqual(st, 200, v)
        self.assertEqual(sorted(v["layers"]), ["bg0", "bg1", "bg3", "collision"])
        scarab = next(t["type"] for t in v["entityTypes"] if t["name"] == "SCARAB")
        self.assertEqual(v["sprites"][str(scarab)]["bank"], 13)  # bank13_scarab, by name
        # its records are named by its own views (docs/n_tranced.md, "Parameter records")
        layouts = {e["type"]: v["room"]["params"][e["param"]].get("layout") for e in v["room"]["entities"]
                   if isinstance(v["room"]["params"][e["param"]], dict)}
        self.assertEqual(layouts.get("POWERS"), "hint_text")
        self.assertEqual(layouts.get("BASIC_CRATE"), "n2_crate")

    def test_layers_and_cells(self):
        _, v = self.call("GET", "/api/projects/xs/room?path=" + ROOM)
        from PIL import Image

        w, h, px = self.layer("xs", ROOM, "bg1")
        img = Image.open(ROOT / ROOM / "bg1.png")
        self.assertEqual(((w, h), px), (img.size, img.tobytes()))
        cw, ch, cells = self.layer("xs", ROOM, "collision")
        self.assertEqual([cw, ch], v["layers"]["collision"]["cells"])
        self.assertEqual(len(cells), cw * ch * 2)
        values = set(struct.unpack("<%dH" % (cw * ch), cells))
        self.assertLessEqual(values, {e["value"] for e in v["legend"]})

    def test_the_tile_set_draws_the_layer(self):
        """A cell of the tile set's answer is the picture's block: the tile,
        flipped as the cell says, in the cell's bank (what the editor's tile
        painting and its eyedropper rely on)."""
        w, h, px = self.layer("xs", ROOM, "bg1")
        st, t = self.call("GET", "/api/projects/xs/room/tiles?path=%s&layer=bg1" % ROOM)
        self.assertEqual(st, 200, t)
        self.assertEqual((t["tileset"], t["bpp"]), ("gJungleBgTiles", 4))
        tiles = base64.b64decode(t["tiles"])
        self.assertEqual(len(tiles), t["count"] * 64)
        cw = w // 8
        cells = struct.unpack("<%dH" % (len(base64.b64decode(t["cells"])) // 2), base64.b64decode(t["cells"]))
        self.assertEqual(len(cells), cw * (h // 8))
        for i in range(0, len(cells), 97):
            v = cells[i]
            tile, hf, vf, bank = v & 0x3FF, v >> 10 & 1, v >> 11 & 1, v >> 12
            cx, cy = i % cw, i // cw
            for y in range(8):
                for x in range(8):
                    sx, sy = (7 - x if hf else x), (7 - y if vf else y)
                    want = tiles[tile * 64 + sy * 8 + sx] + bank * 16
                    self.assertEqual(px[(cy * 8 + y) * w + cx * 8 + x], want, (cx, cy))

    def test_a_sprite(self):
        st, ctype, data = self.get_bytes("/api/projects/xs/sprite?bank=32&frame=0&palette=53")
        self.assertEqual((st, ctype), (200, "image/png"))
        from PIL import Image

        img = Image.open(io.BytesIO(data))
        self.assertEqual(img.info.get("transparency"), 0)
        self.assertEqual(self.get_bytes("/api/projects/xs/sprite?bank=32&frame=999")[0], 404)
        self.assertEqual(self.get_bytes("/api/projects/xs/sprite?bank=x&frame=0")[0], 400)

    def test_not_a_room(self):
        self.assertEqual(self.call("GET", "/api/projects/xs/room?path=games/xs/data/levels")[0], 403)
        self.assertEqual(self.call("GET", "/api/projects/xs/room?path=games/xs/data/levels/no/such")[0], 404)
        self.assertEqual(self.call("GET", "/api/projects/xs/room?path=games/xs/../../x")[0], 400)
        self.assertEqual(self.get_bytes("/api/projects/xs/room/layer?path=%s&layer=bg9" % ROOM)[0], 404)


def same_cell(px, w, cx, cy):
    """A cell's 64 pixels, as the editor sends them."""
    return base64.b64encode(b"".join(px[(cy * 8 + y) * w + cx * 8:(cy * 8 + y) * w + cx * 8 + 8]
                                     for y in range(8))).decode()


class Saving(LevelEditor):
    def test_every_room_saved_unchanged_is_not_written(self):
        """Every room of both games through the API and back, unchanged: a
        cell of each layer painted with what it has, the palette it has,
        room.json as it is. Nothing is written."""
        for game in ("xs", "n2"):
            for room in rooms(game):
                st, v = self.call("GET", "/api/projects/%s/room?path=%s" % (game, room))
                self.assertEqual(st, 200, (room, v))
                body = {"path": room, "room": v["room"], "palette": v["palette"], "layers": {}}
                for lname in v["layers"]:
                    w, h, data = self.layer(game, room, lname)
                    if lname == "collision":
                        body["collision"] = [[w - 1, h - 1, struct.unpack_from("<H", data, len(data) - 2)[0]]]
                    else:
                        body["layers"][lname] = [[0, 0, same_cell(data, w, 0, 0)],
                                                 [w // 8 - 1, h // 8 - 1, same_cell(data, w, w // 8 - 1, h // 8 - 1)]]
                before = {p: p.read_bytes() for p in (ROOT / room).iterdir()}
                st, r = self.call("POST", "/api/projects/%s/room/save" % game, body)
                self.assertEqual((st, r.get("saved")), (200, []), (room, r))
                for p, b in before.items():
                    self.assertEqual(p.read_bytes(), b, p)

    def test_an_edit_then_put_back(self):
        """An entity moved, a collision cell and a BG cell painted, a palette
        entry changed: saved together (the model's checks pass), then saved
        back."""
        from PIL import Image

        files = {p: p.read_bytes() for p in (ROOT / ROOM).iterdir()}
        try:
            _, v = self.call("GET", "/api/projects/xs/room?path=" + ROOM)
            room = json.loads(json.dumps(v["room"]))
            room["entities"][1]["y"] -= 8
            cw, ch, cells = self.layer("xs", ROOM, "collision")
            old_value = struct.unpack_from("<H", cells, 2 * (10 * cw + 5))[0]
            value = 0x2B if old_value != 0x2B else 0
            w, h, px = self.layer("xs", ROOM, "bg1")
            palette = list(v["palette"])
            palette[17] ^= 0x1F
            body = {"path": ROOM, "room": room, "collision": [[5, 10, value]], "palette": palette,
                    "layers": {"bg1": [[0, 0, same_cell(px, w, 1, 2)]]}}
            st, r = self.call("POST", "/api/projects/xs/room/save", body)
            self.assertEqual(st, 200, r)
            self.assertEqual(sorted(r["saved"]), sorted(ROOM + "/" + n for n in
                                                        ("room.json", "collision.png", "bg0.png", "bg1.png",
                                                         "bg2.png", "bg3.png")))
            self.assertFalse([d for d in r["diagnostics"] if d["severity"] == "error"])
            c2 = self.layer("xs", ROOM, "collision")[2]
            self.assertEqual(struct.unpack_from("<H", c2, 2 * (10 * cw + 5))[0], value)
            _, v2 = self.call("GET", "/api/projects/xs/room?path=" + ROOM)
            self.assertEqual((v2["palette"], v2["room"]), (palette, room))
            # back: what the API wrote gives back the pictures' pixels and colours
            body = {"path": ROOM, "room": v["room"], "collision": [[5, 10, old_value]], "palette": v["palette"],
                    "layers": {"bg1": [[0, 0, same_cell(px, w, 0, 0)]]}}
            st, r = self.call("POST", "/api/projects/xs/room/save", body)
            self.assertEqual(st, 200, r)
            for p, b in files.items():
                if p.suffix == ".png":
                    a, o = Image.open(p), Image.open(io.BytesIO(b))
                    self.assertEqual((a.tobytes(), a.getpalette()), (o.tobytes(), o.getpalette()), p)
                else:
                    self.assertEqual(p.read_bytes(), b, p)
        finally:
            for p, b in files.items():
                p.write_bytes(b)

    def test_refused(self):
        files = {p: p.read_bytes() for p in (ROOT / ROOM).iterdir()}
        try:
            mixed = base64.b64encode(bytes([16] * 32 + [32] * 32)).decode()
            for body, status in [
                ({"path": ROOM, "layers": {"bg1": [[0, 0, mixed]]}}, 422),          # two palette banks
                ({"path": ROOM, "layers": {"bg1": [[9999, 0, mixed]]}}, 422),       # outside
                ({"path": ROOM, "layers": {"bg1": [[0, 0, "AAAA"]]}}, 422),         # not 64 pixels
                ({"path": ROOM, "layers": {"bg9": []}}, 400),
                ({"path": ROOM, "collision": [[0, 0, 0x7777]]}, 422),               # not in the legend
                ({"path": ROOM, "palette": [0] * 255}, 422),
                ({"path": ROOM, "room": {"bogus": 1}}, 422),                        # the schema
                ({"path": "games/xs/data/levels"}, 403),
            ]:
                st, r = self.call("POST", "/api/projects/xs/room/save", body)
                self.assertEqual(st, status, (body, r))
            # the model: an entity in another column than its id order allows
            _, v = self.call("GET", "/api/projects/xs/room?path=" + ROOM)
            room = json.loads(json.dumps(v["room"]))
            room["entities"][0]["x"] = 0
            st, r = self.call("POST", "/api/projects/xs/room/save", {"path": ROOM, "room": room,
                                                                      "collision": [[0, 0, 0x2B]]})
            self.assertEqual(st, 422, r)
            # a blend SetupRoomBlend can't store (BLDCNT's effect is two bits, BLDALPHA's weights five)
            for blend in ({"effect": 4, "eva": 4, "evb": 16}, {"effect": 1, "eva": 32, "evb": 16}):
                room = json.loads(json.dumps(v["room"]))
                room["record"]["blend"] = blend
                st, r = self.call("POST", "/api/projects/xs/room/save", {"path": ROOM, "room": room})
                self.assertEqual(st, 422, (blend, r))
            for p, b in files.items():
                self.assertEqual(p.read_bytes(), b, p)
            self.assertEqual(sorted(p.name for p in (ROOT / ROOM).glob(".*.tmp")), [])
        finally:
            for p, b in files.items():
                p.write_bytes(b)


class Rooms(LevelEditor):
    """New, resized and deleted rooms (crashdata.levels.room_ops): each a
    save, checked, all or nothing. The tree is put back after."""

    LEVELS = "games/xs/data/levels/"

    def setUp(self):
        self.kept = {p: p.read_bytes() for p in [ROOT / self.LEVELS / "levels.json", ROOT / "cxx_symbols.txt",
                                                 ROOT / self.LEVELS / "tileset_order.json"]}
        self.made = [ROOT / self.LEVELS / "shipwrecked/extra", ROOT / self.LEVELS / "shipwrecked/blank"]

    def tearDown(self):
        import shutil
        for p, b in self.kept.items():
            p.write_bytes(b)
        for d in self.made:
            if d.exists():
                shutil.rmtree(d)

    def levels(self):
        return json.loads((ROOT / self.LEVELS / "levels.json").read_text())

    def test_new_resize_delete(self):
        # a copy of jungle jam's room, shipwrecked's bonus room
        st, r = self.call("POST", "/api/projects/xs/room/new", {
            "from": "jungle_jam/main", "folder": "shipwrecked/extra",
            "attach": {"level": "LEVEL_SHIPWRECKED", "as": "bonus"}})
        self.assertEqual(st, 200, r)
        self.assertEqual(r["report"]["number"], 41)
        room = json.loads((ROOT / self.LEVELS / "shipwrecked/extra/room.json").read_text())
        self.assertEqual(room["symbols"]["desc"], "gRoom41Desc")
        self.assertEqual(room["record"]["symbol"], "gLevelRoom41")
        lv = self.levels()
        self.assertEqual(lv["rooms"][-1], "shipwrecked/extra")
        self.assertEqual(next(x for x in lv["levels"] if x["constant"] == "LEVEL_SHIPWRECKED")["bonus"],
                         "shipwrecked/extra")
        self.assertIn("_Q34Game5Level6Room41.palette gRoom41Palette", (ROOT / "cxx_symbols.txt").read_text())
        # the folder is taken now
        st, r = self.call("POST", "/api/projects/xs/room/new", {"from": "jungle_jam/main", "folder": "shipwrecked/extra"})
        self.assertEqual(st, 422, r)

        # resized: 4 cells more on the left, 10 fewer on the right; first what it would do
        path = self.LEVELS + "shipwrecked/extra"
        before = json.loads(json.dumps(room))
        edges = {"left": 4, "right": -10, "top": 0, "bottom": 2}
        st, r = self.call("POST", "/api/projects/xs/room/resize", {"path": path, "edges": edges, "check": True})
        self.assertEqual((st, r["saved"]), (200, []), r)
        self.assertEqual(r["report"]["size"], [[502, 60], [496, 62]])
        self.assertEqual(r["report"]["layers"]["bg1"]["shift"], [2, 0])  # its scale is 128: half the cells
        self.assertEqual(json.loads((ROOT / path / "room.json").read_text()), before)  # nothing written
        st, r = self.call("POST", "/api/projects/xs/room/resize", {"path": path, "edges": edges})
        self.assertEqual(st, 200, r)
        room = json.loads((ROOT / path / "room.json").read_text())
        self.assertEqual(room["layers"]["bg0"]["size_tiles"], [496, 62])
        self.assertEqual(room["layers"]["bg0"]["size_chunks"], [31, 8])
        self.assertEqual(room["layers"]["collision"]["size_tiles"], [496, 62])
        self.assertEqual(len(room["entities"]), len(before["entities"]) - len(r["report"]["entities"]))
        # every entity moved 32 pixels right (none was in the cut columns at the right? then it's gone)
        xs = sorted(e["x"] for e in room["entities"])
        kept = sorted(e["x"] + 32 for e in before["entities"] if e["x"] + 32 < 496 * 8)
        self.assertEqual(xs, kept)
        from PIL import Image
        with Image.open(ROOT / path / "bg0.png") as im:
            self.assertEqual(im.size, (31 * 128, 8 * 64))
            with Image.open(ROOT / self.LEVELS / "jungle_jam/main/bg0.png") as src:
                # the source's cell (0, 0) is now cell (4, 0)
                self.assertEqual(im.crop((32, 0, 40, 8)).tobytes(), src.crop((0, 0, 8, 8)).tobytes())
        # too small: refused, nothing written
        st, r = self.call("POST", "/api/projects/xs/room/resize", {"path": path, "edges": {"right": -480}})
        self.assertEqual(st, 422, r)
        self.assertEqual(json.loads((ROOT / path / "room.json").read_text()), room)

        # played: not deleted
        st, r = self.call("POST", "/api/projects/xs/room/delete", {"path": path})
        self.assertEqual(st, 422, r)

        # an empty room like ace of space's, played by no level, then deleted
        st, r = self.call("POST", "/api/projects/xs/room/new", {
            "from": "ace_of_space/main", "folder": "shipwrecked/blank", "empty": True, "size": [60, 30]})
        self.assertEqual(st, 200, r)
        self.assertEqual(r["report"]["number"], 42)
        blank = json.loads((ROOT / self.LEVELS / "shipwrecked/blank/room.json").read_text())
        self.assertEqual(blank["layers"]["bg0"]["size_tiles"], [60, 30])
        self.assertEqual([e["type"] for e in blank["entities"]], ["START_MARKER"])
        st, r = self.call("POST", "/api/projects/xs/room/delete", {"path": self.LEVELS + "shipwrecked/blank"})
        self.assertEqual(st, 200, r)
        self.assertFalse((ROOT / self.LEVELS / "shipwrecked/blank").exists())
        self.assertNotIn("shipwrecked/blank", self.levels()["rooms"])
        self.assertNotIn("gRoom42Palette", (ROOT / "cxx_symbols.txt").read_text())

    def test_an_n_tranced_room(self):
        # played after the island intro's room: its record goes last in "table" "records"
        n2 = ROOT / "games/n2/data/levels"
        self.kept[n2 / "levels.json"] = (n2 / "levels.json").read_bytes()
        self.made.append(n2 / "island_intro/extra")
        st, r = self.call("POST", "/api/projects/n2/room/new", {
            "from": "island_intro/main", "folder": "island_intro/extra", "empty": True,
            "attach": {"level": "LEVEL_ISLAND_INTRO", "as": "rooms", "index": 1}})
        self.assertEqual(st, 200, r)
        lv = json.loads((n2 / "levels.json").read_text())
        self.assertEqual(lv["levels"][0]["rooms"], ["island_intro/main", "island_intro/extra"])
        self.assertEqual(lv["table"]["records"][-1], "island_intro/extra")
        self.assertEqual(r["report"]["number"], 36)


class Tiles(LevelEditor):
    """Drawing tiles: a picture imported as cells, the tile sets' limits on a save."""

    def test_import(self):
        from PIL import Image
        # two cells of jungle jam's BG1 as true colour, one of them flipped: the set's tiles, nothing new
        with Image.open(ROOT / ROOM / "bg1.png") as im:
            rgb = im.convert("RGBA").crop((64, 64, 80, 72))
        a, b = rgb.crop((0, 0, 8, 8)), rgb.crop((8, 0, 16, 8)).transpose(Image.FLIP_LEFT_RIGHT)
        pic = Image.new("RGBA", (24, 8), (0, 0, 0, 0))
        pic.paste(a, (0, 0))
        pic.paste(b, (8, 0))
        buf = io.BytesIO()
        pic.save(buf, format="PNG")
        st, r = self.call("POST", "/api/projects/xs/room/import", {
            "path": ROOM, "layer": "bg1", "png": base64.b64encode(buf.getvalue()).decode()})
        self.assertEqual(st, 200, r)
        self.assertEqual((r["cols"], r["rows"]), (3, 1))
        s = r["stats"]
        self.assertEqual((s["cells"], s["new"], s["blank"], s["limit"]), (3, 0, 1, 768))
        self.assertEqual(s["tiles"] + s["flipped"], 2)
        cells = base64.b64decode(r["cells"])
        with Image.open(ROOT / ROOM / "bg1.png") as im:
            self.assertEqual(cells[:64], im.crop((64, 64, 72, 72)).tobytes())
        # a colour no entry has: the nearest, in one bank per cell
        pic = Image.new("RGB", (8, 8), (250, 10, 240))
        buf = io.BytesIO()
        pic.save(buf, format="PNG")
        st, r = self.call("POST", "/api/projects/xs/room/import", {
            "path": ROOM, "layer": "bg1", "png": base64.b64encode(buf.getvalue()).decode(), "banks": [2]})
        self.assertEqual(st, 200, r)
        self.assertTrue(all(p >> 4 == 2 and p & 15 for p in base64.b64decode(r["cells"])))
        self.assertEqual(r["stats"]["new"], 1)

    def test_too_many_tiles_on_a_screen(self):
        # 600 cells of new tiles in one screen of BG0: more than the tile slot pool's 512
        import random
        rnd = random.Random(1)
        cells = [[cx, cy, base64.b64encode(bytes(rnd.randrange(1, 256) for _ in range(64))).decode()]
                 for cy in range(20) for cx in range(30)]
        files = {p: p.read_bytes() for p in (ROOT / ROOM).glob("*")}
        try:
            st, r = self.call("POST", "/api/projects/xs/room/save", {"path": ROOM, "layers": {"bg0": cells}})
            self.assertEqual(st, 422, r)
            self.assertEqual({d["code"] for d in r["diagnostics"]}, {"tileset"}, r)
            self.assertIn("tile slot pool", r["diagnostics"][0]["message"])
            for p, b in files.items():
                self.assertEqual(p.read_bytes(), b, p)
        finally:
            for p, b in files.items():
                p.write_bytes(b)


EXTRA = "games/xs/data/levels/jungle_jam/extra"


def scratch_edits(call):
    """The edit test's edits, through the API (`call(method, path, body)`):
    a new room (a copy of jungle jam's, played before it), resized, its blend
    changed, a new tile drawn into BG0 and a picture imported into BG1.
    Returns the answers, for checking."""
    from PIL import Image

    out = {}
    st, out["new"] = call("POST", "/api/projects/xs/room/new", {
        "from": "jungle_jam/main", "folder": "jungle_jam/extra",
        "attach": {"level": "LEVEL_JUNGLE_JAM", "as": "rooms", "index": 0}})
    assert st == 200, out["new"]
    st, out["resize"] = call("POST", "/api/projects/xs/room/resize", {
        "path": EXTRA, "edges": {"left": 4, "right": -10, "top": 0, "bottom": 2}})
    assert st == 200, out["resize"]
    st, v = call("GET", "/api/projects/xs/room?path=" + EXTRA)
    room = v["room"]
    room["record"]["blend"] = {"effect": 1, "eva": 8, "evb": 12}
    start = next(e for e in room["entities"] if e["type"] == "START_MARKER")
    cx, cy = start["x"] // 8, start["y"] // 8
    # a new tile: a checker of two colours, in a row of BG0 cells above the start
    tile = bytes((34 if (x // 2 + y // 2) % 2 else 200) for y in range(8) for x in range(8))
    b64 = base64.b64encode(tile).decode()
    bg0 = [[cx + k, cy - 6, b64] for k in range(-4, 12)]
    # a picture imported into BG1 (scale 128: its cells near the start's, halved): a gradient
    pic = Image.new("RGB", (48, 24))
    pic.putdata([(x * 5, y * 10, 255 - x * 5) for y in range(24) for x in range(48)])
    buf = io.BytesIO()
    pic.save(buf, format="PNG")
    st, out["import"] = call("POST", "/api/projects/xs/room/import", {
        "path": EXTRA, "layer": "bg1", "png": base64.b64encode(buf.getvalue()).decode(), "banks": [0, 1, 2, 3]})
    assert st == 200, out["import"]
    imp = out["import"]
    cells = base64.b64decode(imp["cells"])
    bx, by = cx // 2 + 4, max(0, cy // 2 - 8)
    bg1 = [[bx + i % imp["cols"], by + i // imp["cols"], base64.b64encode(cells[i * 64:(i + 1) * 64]).decode()]
           for i in range(imp["cols"] * imp["rows"])]
    st, out["save"] = call("POST", "/api/projects/xs/room/save", {
        "path": EXTRA, "room": room, "layers": {"bg0": bg0, "bg1": bg1}})
    assert st == 200, out["save"]
    out["cells"] = {"bg0": bg0, "bg1": bg1}
    return out


class Scratch(unittest.TestCase):
    """The edit test in a scratch copy of the tree (crashdata.mods.copy_tree):
    a new room, resized, a new tile and an imported picture, a blend;
    data-check passes, and with CRASH_EDITOR_TEST_BUILD=1 the ROM builds."""

    @classmethod
    def setUpClass(cls):
        import tempfile
        import threading

        from crashdata import mods
        from buildtools import editor_server as E

        cls.tmp = Path(tempfile.mkdtemp(prefix="level-editor-"))
        cls.tree = cls.tmp / "tree"
        mods.copy_tree(cls.tree)
        cls.app = E.Server(cls.tree)
        cls.httpd = E.serve(cls.app, 0)
        cls.port = cls.httpd.server_address[1]
        threading.Thread(target=cls.httpd.serve_forever, daemon=True).start()

    @classmethod
    def tearDownClass(cls):
        import shutil

        cls.httpd.shutdown()
        cls.httpd.server_close()
        shutil.rmtree(cls.tmp, ignore_errors=True)

    token = None
    call = LevelEditor.call

    def test_an_edit(self):
        import os
        import subprocess

        out = scratch_edits(self.call)
        self.assertEqual(sorted(out["save"]["saved"]), [EXTRA + "/bg0.png", EXTRA + "/bg1.png", EXTRA + "/room.json"])
        self.assertGreaterEqual(out["import"]["stats"]["new"], 1)
        st, v = self.call("GET", "/api/projects/xs/room?path=" + EXTRA)
        self.assertEqual(v["room"]["record"]["blend"], {"effect": 1, "eva": 8, "evb": 12})
        self.assertEqual(v["room"]["layers"]["bg0"]["size_tiles"], [496, 62])
        # the new tile is the set's, at its end
        st, t = self.call("GET", "/api/projects/xs/room/tiles?path=%s&layer=bg0" % EXTRA)
        self.assertEqual(st, 200, t)
        self.assertGreater(t["count"], 6638)
        st, rep = self.call("GET", "/api/projects/xs/check?text=0")
        self.assertTrue(rep["ok"], rep["diagnostics"])
        if os.environ.get("CRASH_EDITOR_TEST_BUILD") == "1":
            b = subprocess.run(["make", "-j%d" % (os.cpu_count() or 1)], cwd=self.tree, capture_output=True, text=True)
            self.assertEqual(b.returncode, 0, b.stdout[-3000:] + b.stderr[-3000:])
            self.assertTrue((self.tree / "crashbandicootxs.gba").is_file())


class PreviewRooms(unittest.TestCase):
    """tools/preview.py's LEVEL and ROOM: a level's bonus and gem-path rooms
    by name or folder (the PC port's --room bonus|gem-path)."""

    def test_extra_rooms(self):
        from buildtools import preview
        from crashdata import levels as L
        from crashdata.common.game import Game

        L.use_game(Game("xs", ROOT))
        jj = next(i for i, lv in enumerate(preview.levels()) if lv["name"] == "jungle jam")
        self.assertEqual(preview.resolve("jungle_jam/bonus")[:2], (jj, "bonus"))
        self.assertEqual(preview.resolve("jungle jam", "bonus")[:2], (jj, "bonus"))
        self.assertEqual(preview.resolve("jungle_jam", "jungle_jam/bonus")[1], "bonus")
        self.assertEqual(preview.resolve("snow_crash", "gem_path")[1:], ("gem_path", "LEVEL_SNOW_CRASH",
                                                                       "snow_crash/gem_path"))
        self.assertEqual(preview.resolve("snow_crash", "gem-path")[1], "gem_path")
        self.assertEqual(preview.resolve("snow_crash/part2")[1], 2)
        with self.assertRaises(SystemExit):
            preview.resolve("jungle_jam", "gem_path")  # jungle jam has no gem path


if __name__ == "__main__":
    unittest.main()
