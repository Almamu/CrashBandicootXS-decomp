"""The music editor's backend (docs/editors.md, "The music editor"): the
music API of buildtools.editor_server over crashdata.audio.edit, started
on a free port of 127.0.0.1. Reading and saving unchanged run against
this tree (nothing is written) and against the sample mod's (applied to a
scratch folder); an edit runs in a scratch copy of the tree
(crashdata.mods.copy_tree), which CRASH_EDITOR_TEST_BUILD=1 also builds
(a plain `make`: the toolchain is needed).

    python3 -m unittest discover -s packages/buildtools/tests
"""
import base64
import gzip
import io
import json
import math
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
import wave
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
for _p in ("crashdata", "buildtools"):
    sys.path.insert(0, str(ROOT / "packages" / _p / "src"))

from buildtools import editor_server as E  # noqa: E402
from test_editor_server import Backend  # noqa: E402

SOUND = "games/xs/sound"
TRACKED = [SOUND, "games/xs/data/levels/levels.json"]


def wav(samples, rate, width=1, channels=1):
    """A PCM WAV: samples are -1..1 floats (frames of `channels`)."""
    buf = io.BytesIO()
    with wave.open(buf, "wb") as w:
        w.setnchannels(channels)
        w.setsampwidth(width)
        w.setframerate(rate)
        out = bytearray()
        for v in samples:
            if width == 1:
                out.append(max(0, min(255, int(round(v * 127)) + 128)))
            else:
                out += int(round(v * 32767)).to_bytes(2, "little", signed=True)
        w.writeframes(bytes(out))
    return buf.getvalue()


def b64(data):
    return base64.b64encode(data).decode()


def snapshot(root):
    return {p: p.read_bytes() for d in TRACKED for p in ([root / d] if (root / d).is_file() else (root / d).rglob("*"))
            if p.is_file()}


class Music(Backend):
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

    def view(self, game="xs"):
        st, v = self.call("GET", "/api/projects/%s/music" % game)
        self.assertEqual(st, 200, v)
        return v

    def patterns(self, key, game="xs"):
        st, p = self.call("GET", "/api/projects/%s/music/patterns?key=%s" % (game, key))
        self.assertEqual(st, 200, p)
        return p

    def unchanged(self, v, game="xs", root=ROOT):
        """Everything the editor saves, as it is: the tables, the songs'
        every cell, every sample."""
        sound = root / SOUND
        body = {"sfxTable": json.loads((sound / "sfx_table.json").read_text()),
                "music": json.loads((sound / "music.json").read_text()),
                "themes": {t["constant"]: t["song"] for t in v["themes"]},
                "songComments": {s["key"]: s["comment"] for s in v["songs"]},
                "patterns": {}, "samples": {}}
        for s in v["songs"]:
            p = self.patterns(s["key"], game)
            edits = []
            for pi, cells in enumerate(p["cells"]):
                raw = base64.b64decode(cells)
                for k in range(0, len(raw), 5):
                    edits.append({"pattern": pi, "row": k // 5 // p["channels"], "channel": k // 5 % p["channels"],
                                  "cell": list(raw[k:k + 5])})
            body["patterns"][s["key"]] = edits
        for smp in v["samples"]:
            body["samples"][smp["path"]] = b64((sound / smp["path"]).read_bytes())
        for slot in v["sfxSlots"]:
            body["samples"][slot["path"]] = b64((sound / slot["path"]).read_bytes())
        return body


class Reading(Music):
    def test_the_music(self):
        v = self.view()
        manifest = json.loads((ROOT / SOUND / "gax_manifest.json").read_text())
        self.assertEqual([s["key"] for s in v["songs"]], [t["song"] for t in manifest["song_table"]])
        songs = {s["key"]: s for s in v["songs"]}
        self.assertEqual((songs["jungle"]["id"], songs["jungle"]["constant"], songs["jungle"]["mixRate"]),
                         (0, "SONG_JUNGLE", 18158))
        self.assertEqual(songs["jungle"]["xm"]["channels"], 4)
        self.assertIn({"kind": "theme", "name": "THEME_JUNGLE", "comment": "jungle"}, songs["jungle"]["uses"])
        self.assertEqual(sorted(u["name"] for u in songs["warp_room"]["uses"]), ["level_select", "save_menu"])
        self.assertIn("cutscene", {u["kind"] for u in songs["intro"]["uses"]})
        self.assertEqual(songs["drums"]["uses"][0]["name"], "invincible")
        self.assertEqual(len(v["sfx"]), 99)
        crate = next(e for e in v["sfx"] if e.get("name") == "CRATE_BREAK")
        self.assertEqual((crate["slot_id"], crate["volume"]), (9, 200))
        self.assertTrue(any(u.startswith("engine/") for u in crate["uses"]), crate["uses"])
        self.assertEqual(len(v["sfxSlots"]), 87)
        # every sound effect plays its sample at gGaxPeriodTable[1511] / 2048 Hz
        self.assertEqual({s["rate"] for s in v["sfxSlots"]}, {8002.6})
        self.assertEqual(len(v["samples"]), 29)
        self.assertEqual(v["samples"][0]["minLength"], 2718)  # instrument 1's loop end

    def test_the_player_data(self):
        st, raw = self.get_bytes("/api/projects/xs/music/pool")
        self.assertEqual(st, 200)
        pool = json.loads(raw)
        self.assertEqual((len(pool["tables"]["period"]), pool["tables"]["period"][0]), (0xEF4, 0x105A02))
        self.assertEqual(len(pool["tables"]["vibrato"]), 64)
        self.assertEqual(len(pool["music"]["instruments"]), 52)
        self.assertEqual(len(pool["sfx"]["instruments"]), 88)
        self.assertEqual(len(base64.b64decode(pool["sfx"]["samples"])),
                         sum(n for _, n in pool["sfx"]["waves"].values()))
        st, raw = self.get_bytes("/api/projects/xs/music/song?key=jungle")
        self.assertEqual(st, 200)
        song = json.loads(raw)
        self.assertEqual((song["channelCount"], song["orderCount"], song["mixRate"], song["halfRateFx"]),
                         (4, 28, 18158, 1))
        self.assertEqual([len(o) for o in song["orders"]], [28] * 4)
        # the patterns are the built block's, at the song's GaxSongData.patterns
        from crashdata.audio import gax as G

        manifest = G.schemas.load(ROOT / SOUND / "gax_manifest.json")
        pool_, keys, songs = G.load_songs(manifest, str(ROOT / SOUND / "songs"), str(ROOT / SOUND / "samples"))
        _, prefix = G.build_sfx(str(ROOT / SOUND / "gax_sfx_manifest.json"), str(ROOT / SOUND / "sfx_samples"))
        alloc = G.link(pool_.instruments, songs, pool_.samples, prefix)
        start = songs[keys.index("jungle")].info.sequence_data_ptr.off
        streams = base64.b64decode(song["patterns"])
        self.assertEqual(bytes(alloc.buf[start:start + len(streams)]), streams)
        st, r = self.call("GET", "/api/projects/xs/music/song?key=nothing")
        self.assertEqual(st, 422, r)

    def test_patterns(self):
        p = self.patterns("jungle")
        self.assertEqual((p["channels"], len(p["rows"]), p["rom"]), (4, 28, True))
        self.assertEqual(p["playOrder"], list(range(28)))
        self.assertEqual(len(base64.b64decode(p["cells"][0])), 16 * 4 * 5)
        self.assertEqual(p["instruments"][0], {"name": "Instrument 43", "pool": 43})
        self.assertEqual(len(p["groups"]), 4)

    def test_n_tranced_has_none(self):
        for what in ("music", "music/pool", "music/song?key=jungle", "music/patterns?key=jungle"):
            st, r = self.call("GET", "/api/projects/n2/" + what)
            self.assertEqual(st, 404, r)
            self.assertIn("isn't data yet", r["error"])

    def test_import(self):
        # a 16-bit stereo WAV at 22050 Hz: a second of a 440 Hz tone, left only
        n = 22050
        frames = [v for i in range(n) for v in (0.5 * math.sin(2 * math.pi * 440 * i / n), 0.0)]
        st, r = self.call("POST", "/api/projects/xs/music/import", {"wav": b64(wav(frames, n, 2, 2)), "rate": 8000})
        self.assertEqual(st, 200, r)
        self.assertEqual((r["rate"], r["length"], r["source"]), (8000, 8000, {"channels": 2, "bits": 16,
                                                                              "rate": 22050, "float": False}))
        with wave.open(io.BytesIO(base64.b64decode(r["wav"]))) as w:
            self.assertEqual((w.getnchannels(), w.getsampwidth(), w.getframerate(), w.getnframes()), (1, 1, 8000, 8000))
        self.assertIn(r["peak"], range(30, 34))  # half of the left channel: a quarter of full scale
        for bad in ({"wav": b64(b"not a wav")}, {"wav": b64(wav([0.0], 8000)), "rate": 10}, {"rate": 8000}):
            st, r = self.call("POST", "/api/projects/xs/music/import", bad)
            self.assertIn(st, (400, 422), r)

    def test_everything_saved_unchanged_is_not_written(self):
        """Every table, every cell of every song and every sample through
        the API and back, unchanged: nothing is written."""
        before = snapshot(ROOT)
        st, r = self.call("POST", "/api/projects/xs/music/save", self.unchanged(self.view()))
        self.assertEqual((st, r.get("saved")), (200, []), r)
        for p, b in before.items():
            self.assertEqual(p.read_bytes(), b, p)
        if (ROOT / ".git").exists() and shutil.which("git"):
            r = subprocess.run(["git", "diff", "--exit-code", "--stat", "--"] + TRACKED, cwd=ROOT,
                               capture_output=True, text=True)
            self.assertEqual(r.returncode, 0, r.stdout)

    def test_refused(self):
        """What the checks refuse, with where; nothing is written."""
        v = self.view()
        before = snapshot(ROOT)
        table = json.loads((ROOT / SOUND / "sfx_table.json").read_text())
        music = json.loads((ROOT / SOUND / "music.json").read_text())

        def sfx(change):
            t = json.loads(json.dumps(table))
            change(t["entries"])
            return {"sfxTable": t}

        def screens(change):
            m = json.loads(json.dumps(music))
            change(m["screen_music"])
            return {"music": m}

        crate = next(i for i, e in enumerate(table["entries"]) if e.get("name") == "CRATE_BREAK")
        short = wav([0.0] * 100, 15769)
        for body, where in [
            (sfx(lambda e: e[1].update(name="AKU_AKU_LOSE")), "/entries/1/name"),         # two of a name
            (sfx(lambda e: e[crate].update(name="CRATE_SMASH")), "/entries"),              # the code names it
            (sfx(lambda e: e[crate].update(slot_id=0)), "/entries/%d/slot_id" % crate),
            (sfx(lambda e: e[crate].update(slot_id=200)), "/entries/%d/slot_id" % crate),
            (sfx(lambda e: e.append({"instrument": "nothing", "pitch_offset": 0, "volume": 256})),
             "/entries/%d/instrument" % len(table["entries"])),
            (sfx(lambda e: e[crate].update(volume=-1)), "/entries/%d/volume" % crate),
            (screens(lambda s: s.pop()), "/screen_music"),
            (screens(lambda s: s[0].update(song="nothing")), "/screen_music/0/song"),
            ({"themes": {"THEME_JUNGLE": "nothing"}}, None),
            ({"removeSongs": ["jungle"]}, "/song_table/0"),
            ({"newSongs": [{"key": "Bad Key", "xm": b64(b"x")}]}, "/song_table"),
            ({"newSongs": [{"key": "jungle", "xm": b64(b"x")}]}, "/song_table"),
            ({"newSongs": [{"key": "fine", "xm": b64(b"not an xm")}]}, None),
            ({"samples": {"samples/01.wav": b64(short)}}, None),                             # instrument 1 loops to 2718
            ({"samples": {"samples/01.wav": b64(wav([0.0] * 3000, 15769, 2))}}, None),       # 16-bit
            ({"samples": {"samples/99.wav": b64(short)}}, None),
            ({"patterns": {"jungle": [{"pattern": 0, "row": 0, "channel": 0, "cell": [300, 0, 0, 0, 0]}]}}, None),
            ({"patterns": {"jungle": [{"pattern": 99, "row": 0, "channel": 0, "cell": [0, 0, 0, 0, 0]}]}}, None),
            ({"patterns": {"jungle": [{"pattern": 0, "row": 0, "channel": 0, "cell": [49, 99, 0, 0, 0]}]}}, None),
            ({"newInstruments": [{"name": "Bad", "wav": b64(short)}]}, None),
        ]:
            st, r = self.call("POST", "/api/projects/xs/music/save", body)
            self.assertEqual(st, 422, (list(body), r))
            if where:
                self.assertIn(where, [d["pointer"] for d in r.get("diagnostics") or []], r)
        for p, b in before.items():
            self.assertEqual(p.read_bytes(), b, p)
        self.assertFalse((ROOT / SOUND / "instruments").exists())
        self.assertEqual(sorted(p.name for p in (ROOT / "games/xs").rglob(".*.tmp")), [])
        del v


class Mod(unittest.TestCase):
    """The sample mod's tree (a new song with its own instrument, a new
    theme playing it): it opens, its new song plays from the data, and
    everything saved back unchanged writes nothing."""

    @classmethod
    def setUpClass(cls):
        cls.tmp = Path(tempfile.mkdtemp(prefix="music-editor-mod-"))
        cls.tree = cls.tmp / "tree"
        r = subprocess.run([sys.executable, str(ROOT / "tools/apply_mod.py"), str(ROOT / "mods/sample"), str(cls.tree)],
                           cwd=ROOT, capture_output=True, text=True)
        if r.returncode:
            raise RuntimeError(r.stdout + r.stderr)
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
    get_bytes = Music.get_bytes
    view = Music.view
    patterns = Music.patterns
    unchanged = Music.unchanged

    def test_the_sample_mod(self):
        v = self.view()
        song = next(s for s in v["songs"] if s["key"] == "sample_tune")
        self.assertFalse(song["rom"])
        self.assertIn("THEME_SAMPLE", [u["name"] for u in song["uses"]])
        st, raw = self.get_bytes("/api/projects/xs/music/song?key=sample_tune")
        self.assertEqual(st, 200, raw[:300])
        self.assertEqual(json.loads(raw)["orderCount"], len(self.patterns("sample_tune")["playOrder"]))
        # its new instrument is in the music pool, after the ROM's 52
        st, raw = self.get_bytes("/api/projects/xs/music/pool")
        self.assertGreater(len(json.loads(raw)["music"]["instruments"]), 52)
        before = snapshot(self.tree)
        st, r = self.call("POST", "/api/projects/xs/music/save", self.unchanged(v, root=self.tree))
        self.assertEqual((st, r.get("saved")), (200, []), r)
        for p, b in before.items():
            self.assertEqual(p.read_bytes(), b, p)


class Scratch(unittest.TestCase):
    """An edit in a scratch copy of the tree: a sound effect's volume and
    priority, a new sound effect with its own sample imported from a WAV,
    a sound-effect sample replaced, the title screen's and a theme's song,
    a note of a shared ROM pattern; data-check passes, the player plays
    the edit, and (with CRASH_EDITOR_TEST_BUILD=1) the ROM builds."""

    @classmethod
    def setUpClass(cls):
        from crashdata import mods

        cls.tmp = Path(tempfile.mkdtemp(prefix="music-editor-"))
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
    get_bytes = Music.get_bytes
    view = Music.view
    patterns = Music.patterns

    def song(self, key, edits=None):
        if edits is None:
            st, raw = self.get_bytes("/api/projects/xs/music/song?key=%s" % key)
            self.assertEqual(st, 200, raw[:300])
            return json.loads(raw)
        st, r = self.call("POST", "/api/projects/xs/music/song", {"key": key, "edits": edits})
        self.assertEqual(st, 200, r)
        return r

    def test_an_edit(self):
        v = self.view()
        sound = self.tree / SOUND
        table = json.loads((sound / "sfx_table.json").read_text())
        crate = next(e for e in table["entries"] if e.get("name") == "CRATE_BREAK")
        crate["volume"], crate["pitch_offset"] = 160, 5
        # a new sound effect: a decaying square at 8000 Hz, its own instrument
        beep = wav([(1 if (i // 4) % 2 else -1) * 0.8 * (1 - i / 2400) for i in range(2400)], 8000, 2)
        st, imp = self.call("POST", "/api/projects/xs/music/import", {"wav": b64(beep)})
        self.assertEqual((st, imp["rate"], imp["length"]), (200, 8000, 2400), imp)
        table["entries"].append({"instrument": "test_beep", "pitch_offset": 0, "volume": 256, "name": "TEST_BEEP",
                                 "comment": "the music editor's test"})
        # sound-effect sample 09 (CRATE_BREAK's) replaced by a sine, at its slot's rate
        slot = next(s for s in v["sfxSlots"] if s["slot"] == 9)
        sine = wav([0.6 * math.sin(2 * math.pi * 300 * i / 8000) for i in range(4000)], 8000, 2)
        st, imp2 = self.call("POST", "/api/projects/xs/music/import", {"wav": b64(sine), "rate": round(slot["rate"])})
        self.assertEqual(st, 200, imp2)
        music = json.loads((sound / "music.json").read_text())
        music["screen_music"][0]["song"] = "credits"
        # jungle's first note, an octave up: its pattern is stored once for order positions 0 and 8
        p = self.patterns("jungle")
        cells = base64.b64decode(p["cells"][0])
        cell = list(cells[:5])
        shared = [pi for pi, g in enumerate(p["groups"][0]) if g == p["groups"][0][0]]
        self.assertGreater(len(shared), 1)
        cell[0] += 12
        edits = [{"pattern": 0, "row": 0, "channel": 0, "cell": cell}]
        before = self.song("jungle")
        preview = self.song("jungle", edits)
        self.assertNotEqual(preview["patterns"], before["patterns"])
        body = {"sfxTable": table, "music": music, "themes": {"THEME_SNOW": "jungle"},
                "patterns": {"jungle": edits}, "songComments": {"jungle": "the jungle theme's, edited"},
                "samples": {"sfx_samples/09.wav": imp2["wav"]},
                "newInstruments": [{"name": "test_beep", "wav": imp["wav"]}]}
        st, r = self.call("POST", "/api/projects/xs/music/save", body)
        self.assertEqual(st, 200, r)
        self.assertFalse([d for d in r["diagnostics"] if d["severity"] == "error"], r)
        self.assertEqual(sorted(r["saved"]), sorted([
            "games/xs/data/levels/levels.json", SOUND + "/gax_manifest.json", SOUND + "/instruments/test_beep.json",
            SOUND + "/music.json", SOUND + "/sfx_samples/09.wav", SOUND + "/sfx_samples/test_beep.wav",
            SOUND + "/sfx_table.json", SOUND + "/songs/jungle.xm"]))
        # read back: the player plays the saved edit, the shared positions too
        self.assertEqual(self.song("jungle")["patterns"], preview["patterns"])
        p2 = self.patterns("jungle")
        for pi in shared:
            self.assertEqual(base64.b64decode(p2["cells"][pi])[0], cells[0] + 12 - p["transposes"][0][0]
                             + p["transposes"][0][pi])
        v2 = self.view()
        self.assertEqual(v2["screens"][0]["song"], "credits")
        self.assertEqual(next(t for t in v2["themes"] if t["constant"] == "THEME_SNOW")["song"], "jungle")
        self.assertEqual(v2["sfx"][-1]["constant"], "SFX_TEST_BEEP")
        st, raw = self.get_bytes("/api/projects/xs/music/pool")
        self.assertEqual(json.loads(raw)["sfxSlots"], {"test_beep": 88})
        # only the edited group lost its recorded bytes
        m0 = json.loads((ROOT / SOUND / "gax_manifest.json").read_text())
        m1 = json.loads((sound / "gax_manifest.json").read_text())
        g0 = m0["songs"]["jungle"]["pattern_group_bytes"]
        self.assertEqual(set(g0) - set(m1["songs"]["jungle"]["pattern_group_bytes"]), {str(p["groups"][0][0])})
        st, rep = self.call("GET", "/api/projects/xs/check?text=0")
        self.assertEqual(st, 200)
        self.assertTrue(rep["ok"], rep["diagnostics"])
        if os.environ.get("CRASH_EDITOR_TEST_BUILD") == "1":
            b = subprocess.run(["make", "-j%d" % (os.cpu_count() or 1)], cwd=self.tree, capture_output=True,
                               text=True)
            self.assertEqual(b.returncode, 0, b.stdout[-3000:] + b.stderr[-3000:])
            self.assertTrue((self.tree / "crashbandicootxs.gba").is_file())
            # the new sound effect's id (0x63) and the table's new length
            sfx_h = (self.tree / "build/include/constants/sfx.h").read_text()
            self.assertIn("#define SFX_TEST_BEEP 0x63", sfx_h)
            self.assertIn("#define SFX_COUNT 100", sfx_h)


if __name__ == "__main__":
    unittest.main()
