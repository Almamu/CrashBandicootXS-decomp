"""The cutscene editor's backend (docs/editors.md, "The cutscene editor"):
the cutscene API of buildtools.editor_server over crashdata.cutscenes.edit,
started on a free port of 127.0.0.1. Reading and saving unchanged run
against this tree (nothing is written); an edit runs in a scratch copy of
it (crashdata.mods.copy_tree), which CRASH_EDITOR_TEST_BUILD=1 also builds
(a plain `make`: the toolchain is needed).

    python3 -m unittest discover -s packages/buildtools/tests
"""
import base64
import gzip
import io
import json
import os
import shutil
import struct
import subprocess
import sys
import tempfile
import threading
import unittest
import urllib.error
import urllib.request
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
for _p in ("crashdata", "buildtools"):
    sys.path.insert(0, str(ROOT / "packages" / _p / "src"))

from buildtools import editor_server as E  # noqa: E402
from test_editor_server import Backend  # noqa: E402

LANGS = ("en", "fr", "de", "es", "it", "nl")


def strings(page):
    return [{c: s[c] for c in LANGS} for s in page]


class Cutscenes(Backend):
    def get_bytes(self, path):
        req = urllib.request.Request("http://127.0.0.1:%d%s" % (self.port, path), headers={"Accept-Encoding": "gzip"})
        try:
            with urllib.request.urlopen(req, timeout=600) as r:
                data = r.read()
                if r.headers.get("Content-Encoding") == "gzip":
                    data = gzip.decompress(data)
                return r.status, data
        except urllib.error.HTTPError as e:
            with e:
                return e.code, e.read()

    def picture(self, path, game="xs"):
        st, data = self.get_bytes("/api/projects/%s/cutscenes/picture?path=%s" % (game, path))
        self.assertEqual(st, 200, data[:300])
        w, h = struct.unpack_from("<II", data)
        return w, h, list(struct.unpack_from("<256H", data, 8)), data[8 + 512:]

    def view(self, game="xs"):
        st, v = self.call("GET", "/api/projects/%s/cutscenes" % game)
        self.assertEqual(st, 200, v)
        return v


class Reading(Cutscenes):
    def test_the_cutscenes(self):
        v = self.view()
        data = json.loads((ROOT / "games/xs/data/cutscenes.json").read_text())
        self.assertEqual(v["cutscenes"], data)
        self.assertEqual(len(v["pictures"]), 24)
        self.assertEqual(v["box"], {"x": 7, "y": 126, "w": 228, "h": 30})
        self.assertEqual(v["pages"]["cutscene01.page0"][0]["en"],
                         "on a mysterious space station orbiting high above the earth ...")
        self.assertTrue(v["pages"]["cutscene01.page0"][0]["fr"].startswith("a bord d'une"))
        self.assertEqual([lang["code"] for lang in v["languages"]], list(LANGS))
        self.assertIn("SONG_INTRO", [s["name"] for s in v["songs"]])
        self.assertEqual(v["sfx"][-1]["name"], "SFX_NONE")
        font = v["font"]
        self.assertEqual(len(base64.b64decode(font["glyphs"])), len(font["widths"]) * 256)
        self.assertEqual((len(font["widths"]), len(font["palette"]), font["chars"][0]), (79, 16, ord("a")))
        self.assertIn("engine/src/game/level/level_state.cpp", v["references"]["CUTSCENE_INTRO"])
        self.assertTrue(any("on_clear" in r for r in v["references"]["CUTSCENE_DINGODILE_CLEARED"]))
        self.assertEqual(v["references"]["CUTSCENE_CORTEX_WARNING"], [])

    def test_every_picture(self):
        from PIL import Image

        for pic in self.view()["pictures"]:
            w, h, pal, px = self.picture(pic["path"])
            img = Image.open(ROOT / "games/xs/graphics" / pic["path"])
            self.assertEqual(((w, h), px), (img.size, img.tobytes()))
            self.assertEqual(len(pal), 256)

    def test_n_tranced_has_none(self):
        st, r = self.call("GET", "/api/projects/n2/cutscenes")
        self.assertEqual(st, 404, r)
        self.assertIn("aren't data yet", r["error"])

    def test_import(self):
        from PIL import Image

        src = Image.open(ROOT / "games/xs/graphics/cutscenes/03_cortex_hologram.png").convert("RGB")
        buf = io.BytesIO()
        src.save(buf, format="PNG")
        png = base64.b64encode(buf.getvalue()).decode()
        st, r = self.call("POST", "/api/projects/xs/cutscenes/import", {"png": png})
        self.assertEqual(st, 200, r)
        self.assertEqual((r["width"], r["height"], len(r["palette"])), (240, 160, 256))
        self.assertTrue(r["exact"], r["how"])  # its 15-bit colours, at most 256 of them
        px = base64.b64decode(r["pixels"])
        rgb = [tuple(c >> 3 for c in src.getpixel((x, y))) for y in (0, 80, 159) for x in (0, 120, 239)]
        mine = [r["palette"][px[y * 240 + x]] for y in (0, 80, 159) for x in (0, 120, 239)]
        self.assertEqual(rgb, [(v & 31, v >> 5 & 31, v >> 10 & 31) for v in mine])
        # mapped to another picture's palette
        _, _, pal, _ = self.picture("cutscenes/11_cortex_on_screen.png")
        st, r = self.call("POST", "/api/projects/xs/cutscenes/import", {"png": png, "palette": pal})
        self.assertEqual((st, r["palette"]), (200, pal))
        # a picture of another size
        buf = io.BytesIO()
        src.resize((100, 100)).save(buf, format="PNG")
        st, r = self.call("POST", "/api/projects/xs/cutscenes/import", {"png": base64.b64encode(buf.getvalue()).decode()})
        self.assertEqual(st, 422, r)
        self.assertIn("240x160", r["error"])


class Saving(Cutscenes):
    def unchanged(self, v):
        body = {"cutscenes": v["cutscenes"], "pages": {k: strings(s) for k, s in v["pages"].items()},
                "pictures": {}}
        for pic in v["pictures"]:
            _, _, pal, px = self.picture(pic["path"])
            body["pictures"][pic["path"]] = {"palette": pal, "pixels": base64.b64encode(px).decode()}
        return body

    def test_every_cutscene_saved_unchanged_is_not_written(self):
        """Every cutscene, every page in the six languages and every picture
        through the API and back, unchanged: nothing is written."""
        tracked = ["games/xs/data", "games/xs/text", "games/xs/graphics", "cxx_symbols.txt"]
        before = {p: p.read_bytes() for d in tracked for p in ([ROOT / d] if (ROOT / d).is_file()
                                                              else (ROOT / d).rglob("*")) if p.is_file()}
        st, r = self.call("POST", "/api/projects/xs/cutscenes/save", self.unchanged(self.view()))
        self.assertEqual((st, r.get("saved")), (200, []), r)
        for p, b in before.items():
            self.assertEqual(p.read_bytes(), b, p)
        if (ROOT / ".git").exists() and shutil.which("git"):
            r = subprocess.run(["git", "diff", "--exit-code", "--stat", "--"] + tracked, cwd=ROOT,
                               capture_output=True, text=True)
            self.assertEqual(r.returncode, 0, r.stdout)

    def test_refused(self):
        """What the checks refuse, with where; nothing is written."""
        v = self.view()
        files = {p: p.read_bytes() for p in [ROOT / "games/xs/data/cutscenes.json"]
                 + sorted((ROOT / "games/xs/text").glob("*.po*"))}
        bad_song = json.loads(json.dumps(v["cutscenes"]))
        bad_song["slides"][0]["song"] = "SONG_NOTHING"
        dangling = json.loads(json.dumps(v["cutscenes"]))
        dangling["cutscenes"][3]["slides"].append({"slide": "no_such_record"})
        page = strings(v["pages"]["cutscene01.page0"])
        page[0]["de"] = "Quatsch"  # capitals the font doesn't have
        empty = strings(v["pages"]["cutscene01.page0"])
        empty[0]["it"] = ""
        black = {"palette": [0] * 256, "pixels": base64.b64encode(bytes(240 * 160)).decode()}
        for body, status, where in [
            ({"cutscenes": bad_song}, 422, "/slides/0/song"),
            ({"cutscenes": dangling}, 422, "/cutscenes/3/slides/1/slide"),
            ({"pages": {"cutscene01.page0": page}}, 422, "/cutscene01.page0/0/de"),
            ({"pages": {"cutscene01.page0": empty}}, 422, "/cutscene01.page0/0/it"),
            ({"pages": {"cutscene01.page0": None}}, 422, None),  # a slide still shows it
            ({"cutscenes": {"cutscenes": []}}, 422, None),          # the schema
            ({"newPictures": [dict(black, path="cutscenes/bad name.png")]}, 422, None),
            ({"newPictures": [dict(black, path="cutscenes/03_again.png")]}, 422, None),
            ({"pictures": {"cutscenes/99_none.png": black}}, 422, None),
            ({"pictures": {"cutscenes/00_space_station.png": {"palette": [0] * 3, "pixels": ""}}}, 422, None),
        ]:
            st, r = self.call("POST", "/api/projects/xs/cutscenes/save", body)
            self.assertEqual(st, status, (body.keys(), r))
            if where:
                self.assertIn(where, [d["pointer"] for d in r.get("diagnostics") or []], r)
        for p, b in files.items():
            self.assertEqual(p.read_bytes(), b, p)
        self.assertEqual(sorted(p.name for p in (ROOT / "games/xs").rglob(".*.tmp")), [])


class Scratch(unittest.TestCase):
    """An edit in a scratch copy of the tree: a page's text changed in two
    languages, a string added to a page, a slide's timing, a new picture
    imported and shown by a new slide with its own page, another picture
    replaced; data-check and the text check pass, and (with
    CRASH_EDITOR_TEST_BUILD=1) the ROM builds."""

    @classmethod
    def setUpClass(cls):
        from crashdata import mods

        cls.tmp = Path(tempfile.mkdtemp(prefix="cutscene-editor-"))
        cls.tree = cls.tmp / "tree"
        mods.copy_tree(cls.tree)
        cls.app = E.Server(cls.tree)
        cls.httpd = E.serve(cls.app, 0)
        cls.port = cls.httpd.server_address[1]
        threading.Thread(target=cls.httpd.serve_forever, daemon=True).start()

    @classmethod
    def tearDownClass(cls):
        cls.httpd.shutdown()
        cls.httpd.server_close()
        shutil.rmtree(cls.tmp, ignore_errors=True)

    token = None
    call = Backend.call
    get_bytes = Cutscenes.get_bytes
    picture = Cutscenes.picture
    view = Cutscenes.view

    def test_an_edit(self):
        from PIL import Image, ImageOps

        v = self.view()
        data = json.loads(json.dumps(v["cutscenes"]))
        pages = {}
        # the text: the intro's first string, in English and French
        p = strings(v["pages"]["cutscene01.page0"])
        p[0]["en"] = "on a mysterious space station, far above the earth ..."
        p[0]["fr"] = "sur une mystérieuse station spatiale, loin au-dessus de la terre ..."
        pages["cutscene01.page0"] = p
        # a second string on N. Gin's page (the same in every language)
        p = strings(v["pages"]["cutscene05.page0"])
        p.append({c: "coco: and one more thing ..." for c in LANGS})
        pages["cutscene05.page0"] = p
        # timing: coco_on_screen waits 90 frames, then no fade
        rec = next(s for s in data["slides"] if s["name"] == "coco_on_screen")
        rec["wait"], rec["fade_after"] = 90, -1
        # a new picture: picture 03 mirrored, imported from an RGB PNG
        src = ImageOps.mirror(Image.open(self.tree / "games/xs/graphics/cutscenes/03_cortex_hologram.png").convert("RGB"))
        buf = io.BytesIO()
        src.save(buf, format="PNG")
        st, imp = self.call("POST", "/api/projects/xs/cutscenes/import", {"png": base64.b64encode(buf.getvalue()).decode()})
        self.assertEqual(st, 200, imp)
        new_pic = {"path": "cutscenes/24_mirror.png", "about": "picture 03, mirrored", "palette": imp["palette"],
                   "pixels": imp["pixels"]}
        data["slides"].append(dict(rec, name="mirror", picture="cutscenes/24_mirror.png", wait=0, fade_after=0))
        data["cutscenes"][5]["slides"].append({"slide": "mirror", "text": "cutscene05.page1"})
        pages["cutscene05.page1"] = [{c: "cortex: a mirror? what is this?" for c in LANGS}]
        # picture 12 replaced: itself, upside down, on its own palette
        w, h, pal, px = self.picture("cutscenes/12_coco_on_screen.png")
        flipped = b"".join(px[(h - 1 - y) * w:(h - y) * w] for y in range(h))
        body = {"cutscenes": data, "pages": pages, "newPictures": [new_pic],
                "pictures": {"cutscenes/12_coco_on_screen.png": {"palette": pal,
                                                                  "pixels": base64.b64encode(flipped).decode()}}}
        st, r = self.call("POST", "/api/projects/xs/cutscenes/save", body)
        self.assertEqual(st, 200, r)
        self.assertFalse([d for d in r["diagnostics"] if d["severity"] == "error"], r)
        self.assertEqual(sorted(r["saved"]), sorted([
            "cxx_symbols.txt", "games/xs/data/cutscenes.json", "games/xs/graphics/manifest.json",
            "games/xs/graphics/cutscenes/24_mirror.png", "games/xs/graphics/cutscenes/12_coco_on_screen.png"]
            + ["games/xs/text/%s" % n for n in ("crashxs.pot", "fr.po", "de.po", "es.po", "it.po", "nl.po")]))
        # read back
        v2 = self.view()
        self.assertEqual(v2["cutscenes"], data)
        self.assertEqual(strings(v2["pages"]["cutscene01.page0"]), pages["cutscene01.page0"])
        self.assertEqual(strings(v2["pages"]["cutscene05.page1"]), pages["cutscene05.page1"])
        self.assertEqual(v2["pageOrder"][-1], "cutscene05.page1")
        self.assertEqual(self.picture("cutscenes/12_coco_on_screen.png")[3], flipped)
        self.assertEqual(self.picture("cutscenes/24_mirror.png")[3], base64.b64decode(imp["pixels"]))
        # only the changed entries are written again (the others' lines are as they were)
        diff = subprocess.run(["diff", str(ROOT / "games/xs/text/de.po"), str(self.tree / "games/xs/text/de.po")],
                              capture_output=True, text=True).stdout
        self.assertLess(len(diff.splitlines()), 40, diff)
        # every check, the text's too
        st, rep = self.call("GET", "/api/projects/xs/check?text=1")
        self.assertEqual(st, 200)
        self.assertTrue(rep["ok"], rep["diagnostics"])
        if os.environ.get("CRASH_EDITOR_TEST_BUILD") == "1":
            b = subprocess.run(["make", "-j%d" % (os.cpu_count() or 1)], cwd=self.tree, capture_output=True,
                               text=True)
            self.assertEqual(b.returncode, 0, b.stdout[-3000:] + b.stderr[-3000:])
            self.assertTrue((self.tree / "crashbandicootxs.gba").is_file())


if __name__ == "__main__":
    unittest.main()
