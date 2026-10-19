"""crashdata.audio.edit's pieces (docs/editors.md, "The music editor"):
the XM patterns read as cells and packed back as the songs' XMs pack
them, a cell edit that writes only its pattern, the pattern sharing of
the ROM's songs, a song's patterns as the build lays them out, and WAVs
made into GAX2 samples.

    python3 -m unittest discover -s packages/crashdata/tests
"""
import base64
import hashlib
import io
import json
import math
import struct
import sys
import unittest
import wave
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "packages" / "crashdata" / "src"))

from crashdata.audio import edit as E  # noqa: E402
from crashdata.audio import gax as G  # noqa: E402
from crashdata.audio import pool as P  # noqa: E402
from crashdata.common.game import Game  # noqa: E402

SOUND = ROOT / "games" / "xs" / "sound"
MANIFEST = json.loads((SOUND / "gax_manifest.json").read_text())


class Patterns(unittest.TestCase):
    def test_every_pattern_packs_back_to_its_bytes(self):
        for p in sorted((SOUND / "songs").glob("*.xm")):
            buf = p.read_bytes()
            layout = E.XmLayout(buf)
            for i, (start, hlen, _, packed) in enumerate(layout.patterns):
                self.assertEqual(E.pack_cells(E.unpack_cells(buf, layout, i)), buf[start + hlen:start + hlen + packed],
                                 (p.name, i))

    def test_a_cell_edit_writes_its_pattern_only(self):
        buf = (SOUND / "songs" / "drums.xm").read_bytes()
        layout = E.XmLayout(buf)
        same, changed = E.xm_with_cells(buf, [(3, 5, 1, bytes(E.unpack_cells(buf, layout, 3)[(5 * 3 + 1) * 5:][:5]))])
        self.assertEqual((same, changed), (buf, []))
        new, changed = E.xm_with_cells(buf, [(3, 5, 1, bytes([49, 1, 0x30, 0xC, 0x20]))])
        self.assertEqual(changed, [3])
        after = E.XmLayout(new)
        cells = E.unpack_cells(new, after, 3)
        self.assertEqual(bytes(cells[(5 * 3 + 1) * 5:][:5]), bytes([49, 1, 0x30, 0xC, 0x20]))
        for i in range(len(layout.patterns)):
            if i != 3:
                self.assertEqual(E.unpack_cells(new, after, i), E.unpack_cells(buf, layout, i), i)
        # the instruments after the patterns are as they were
        self.assertEqual(new[after.end:], buf[layout.end:])
        self.assertEqual(G.read_xm("drums.xm", new)["instruments"], G.read_xm("drums.xm", buf)["instruments"])

    def test_an_edit_reaches_the_patterns_it_shares_bytes_with(self):
        sm = MANIFEST["songs"]["jungle"]
        shared = E.shared_cells(sm, 0, 0)
        self.assertIn((0, 0, 0), shared)
        self.assertGreater(len(shared), 1)
        edits = E.expand_edits(MANIFEST, "jungle", [{"pattern": 0, "row": 2, "channel": 0, "cell": [50, 1, 0, 0, 0]}], 14)
        self.assertEqual(sorted((p, r, c) for p, r, c, _ in edits), sorted((p, 2, c) for p, c, _ in shared))
        self.assertRaises(E.EditError, E.expand_edits, MANIFEST, "jungle",
                          [{"pattern": 0, "row": 0, "channel": 0, "cell": [50, 99, 0, 0, 0]}], 14)

    def test_every_rom_pattern_packs_back_to_the_roms_bytes(self):
        """Every pattern of every ROM song, its cells packed again as an
        edit packs them (E.pack_cells) and encoded as the build encodes
        an edited pattern, gives back the ROM's bytes: rom_patterns.json
        has the SHA-1 of each of the songs' pattern groups as the ROM
        stores them. Every order position and channel of a group, not only
        the one the build packs, so an edit of any of them (made in all,
        E.expand_edits) starts from the ROM's bytes."""
        rom = json.loads((Path(__file__).parent / "rom_patterns.json").read_text())
        self.assertEqual(sorted(rom), sorted(MANIFEST["songs"]))
        pool, _, _ = G.load_songs(MANIFEST, str(SOUND / "songs"), str(SOUND / "samples"))
        for key, sm in MANIFEST["songs"].items():
            buf = (SOUND / "songs" / ("%s.xm" % key)).read_bytes()
            layout = E.XmLayout(buf)
            out = bytearray(buf[:layout.patterns[0][0]])
            for i, (start, hlen, _, _) in enumerate(layout.patterns):
                data = E.pack_cells(E.unpack_cells(buf, layout, i))
                head = bytearray(buf[start:start + hlen])
                struct.pack_into("<H", head, 7, len(data))
                out += head + data
            out += buf[layout.end:]
            song = G.rom_song(sm, G.read_xm(key, bytes(out)), pool.instruments)
            for ci, ch in enumerate(song.channels):
                for pi, pat in enumerate(ch.patterns):
                    gid = ch.pattern_groups[pi]
                    self.assertEqual(hashlib.sha1(pat.pack()).hexdigest(), rom[key][gid], (key, ci, pi, gid))

    def test_the_effects_the_xm_says(self):
        """What a cell's effect and volume column are in GAX2
        (gax._xm_effect_to_gax): the ROM's alternating speeds, its finer
        and coarser volume slides and its volumes are all written."""
        for (e, p, v), want in [
            ((7, 0x96, 0), (7, 0x96)),          # 9 ticks, then 6
            ((10, 0x20, 0x92), (10, 2)),        # XM's A20: up 2
            ((10, 0x01, 0x81), (11, 1)),        # A01: down 1
            ((10, 0x11, 0), (0, 0)),            # both ways: no slide
            ((12, 0x18, 0), (12, 0x60)),        # C18: 24 * 4
            ((12, 0x40, 0), (12, 0xFF)),
            ((12, 0x62, 0), (12, 0x62)),        # over 0x40: GAX2's own
            ((11, 0xFF, 1), (11, 0xFF)),        # volume column 01: the command as it is
            ((10, 0x40, 1), (10, 0x40)),
            ((0, 0, 0x94), (10, 0x40)),
            ((15, 0x80, 0x30), (12, 0x80)),     # a BPM: the volume column's
        ]:
            self.assertEqual(G._xm_effect_to_gax(e, p, v), want, (e, p, v))

    def test_a_structure_written_back_unchanged_is_the_xm(self):
        for p in sorted((SOUND / "songs").glob("*.xm")):
            buf = p.read_bytes()
            self.assertEqual(E.xm_with_structure(buf, E.structure(buf)), buf, p.name)

    def test_a_structure(self):
        """A new structure: an order position, a shorter pattern, a channel,
        the speed. The XM reads back as it says; the other patterns and the
        instruments keep their bytes."""
        buf = (SOUND / "songs" / "drums.xm").read_bytes()
        st = E.structure(buf)
        ch = st["channels"]
        cells = base64.b64decode(st["patterns"][1]["cells"])
        st["patterns"].append({"rows": 8, "cells": base64.b64encode(cells[:8 * ch * 5]).decode()})
        st["order"] = [0, len(st["patterns"]) - 1] + st["order"][1:]
        st["restart"], st["speed"] = 2, 5
        new = E.xm_with_structure(buf, st)
        xm = G.read_xm("drums.xm", new)
        self.assertEqual((xm["order"], xm["restart"], xm["tempo"]), (st["order"], 2, 5))
        self.assertEqual(len(xm["patterns"][-1]), 8)
        old, after = E.XmLayout(buf), E.XmLayout(new)
        for i in range(len(old.patterns)):
            self.assertEqual(E.unpack_cells(new, after, i), E.unpack_cells(buf, old, i), i)
        self.assertEqual(new[after.end:], buf[old.end:])
        # a channel more: every pattern's cells widen
        wide = dict(st, channels=ch + 1, patterns=[
            {"rows": p["rows"], "cells": base64.b64encode(b"".join(
                base64.b64decode(p["cells"])[r * ch * 5:(r + 1) * ch * 5] + bytes(5) for r in range(p["rows"]))).decode()}
            for p in st["patterns"]])
        self.assertEqual(G.read_xm("drums.xm", E.xm_with_structure(buf, wide))["num_channels"], ch + 1)
        for bad in (dict(st, order=[]), dict(st, order=[99]), dict(st, restart=99), dict(st, speed=0),
                    dict(st, channels=ch + 1), dict(st, patterns=[{"rows": 0, "cells": ""}])):
            self.assertRaises(E.EditError, E.xm_with_structure, buf, bad)

    def test_a_rom_song_without_its_scaffold(self):
        """A ROM song whose structure changes loses its scaffold
        (descaffolded) and is built from its XM like a new song, with its
        settings; one that keeps it must fit it."""
        game = Game("xs", ROOT)
        buf = (SOUND / "songs" / "jungle.xm").read_bytes()
        st = E.structure(buf)
        st["order"].insert(3, 0)
        song = E.player_song(game, "jungle", unsaved={"structures": {"jungle": st}})
        before = E.player_song(game, "jungle")
        self.assertEqual((song["orderCount"], before["orderCount"]), (29, 28))
        self.assertEqual((song["mixRate"], song["volume"], song["dsp"]), (before["mixRate"], before["volume"],
                                                                           before["dsp"]))
        m = E.descaffolded(MANIFEST, "jungle")
        self.assertFalse(G.scaffolded(m["songs"]["jungle"]))
        self.assertTrue(G.scaffolded(MANIFEST["songs"]["jungle"]))
        self.assertIsNone(G.scaffold_misfit(MANIFEST["songs"]["jungle"], G.read_xm("jungle", buf)))
        self.assertIn("order list", G.scaffold_misfit(MANIFEST["songs"]["jungle"],
                                                      G.read_xm("jungle", E.xm_with_structure(buf, st))))
        # every ROM song builds from its XM without its scaffold
        for key in MANIFEST["songs"]:
            m = E.descaffolded(m, key)
        pool, keys, songs = G.load_songs(m, str(SOUND / "songs"), str(SOUND / "samples"))
        for key, s in zip(keys, songs):
            self.assertEqual(s.info.num_patterns_per_channel, MANIFEST["songs"][key]["num_patterns_per_channel"], key)
            self.assertEqual(s.info.loop_point, MANIFEST["songs"][key]["loop_point"], key)

    def test_an_unsaved_new_song(self):
        game = Game("xs", ROOT)
        xm = base64.b64encode((SOUND / "songs" / "drums.xm").read_bytes()).decode()
        unsaved = {"newSongs": [{"key": "a_new_one", "xm": xm}]}
        song = E.player_song(game, "a_new_one", unsaved=unsaved)
        self.assertEqual((song["id"], song["channelCount"]), (len(MANIFEST["song_table"]), 3))
        p = E.patterns(game, "a_new_one", xm)
        self.assertEqual((p["rom"], p["channels"]), (False, 3))
        self.assertRaises(E.EditError, E.player_song, game, "jungle", None, {"newSongs": [{"key": "jungle", "xm": xm}]})

    def test_the_songs_patterns_are_the_builds(self):
        pool, keys, songs = G.load_songs(MANIFEST, str(SOUND / "songs"), str(SOUND / "samples"))
        _, prefix = G.build_sfx(str(SOUND / "gax_sfx_manifest.json"), str(SOUND / "sfx_samples"))
        alloc = G.link(pool.instruments, songs, pool.samples, prefix)
        for key, song in zip(keys, songs):
            streams, orders = G.song_sequence(song, pool.instruments)
            start = song.info.sequence_data_ptr.off
            self.assertEqual(bytes(alloc.buf[start:start + len(streams)]), streams, key)
            self.assertEqual(len(orders), song.info.num_channels)

    def test_the_player_data(self):
        game = Game("xs", ROOT)
        song = E.player_song(game, "main_menu_europe")
        self.assertEqual((song["id"], song["channelCount"], song["dsp"][:3]), (11, 7, [6, 25, 8]))
        t = E.tables(E.Files(game))
        self.assertEqual((len(t["period"]), t["vibrato"][16], t["mixRates"][4]), (0xEF4, 127, 15769))


class PoolEdits(unittest.TestCase):
    """The music pool's instruments and samples edited (crashdata.audio.pool, edit.apply_pool_edits)."""

    def lengths(self):
        return {int(k): len(G.load_sample_wav(str(SOUND / "samples" / ("%02d.wav" % int(k)))))
                for k, v in MANIFEST["samples"].items() if v["has_data"]}

    def test_every_instrument_edited_unchanged_is_itself(self):
        lengths = self.lengths()
        for n, rec in MANIFEST["instruments"].items():
            self.assertEqual(P.edited_instrument(rec, json.loads(json.dumps(rec)), lengths, n), rec, n)
            # the envelope as the editor sends it (points' ticks and volumes): the recorded slopes kept
            if rec["envelope"]:
                env = {k: rec["envelope"][k] for k in ("sustain", "loop_start", "loop_end")}
                env["points"] = [{"x": p["x"], "y": p["y"]} for p in rec["envelope"]["points"]]
                self.assertEqual(P.edited_envelope(rec["envelope"], env, n), rec["envelope"], n)

    def test_an_instrument(self):
        lengths = self.lengths()
        rec = MANIFEST["instruments"]["7"]
        new = json.loads(json.dumps(rec))
        new["envelope"]["points"][1]["y"] = 100
        new["samples"][0].update(loop_start=10, loop_end=20, is_bidirectional=False)
        new["vibrato_depth"] = 9
        out = P.edited_instrument(rec, new, lengths, "7")
        pts = out["envelope"]["points"]
        # the moved point and the one after it get their slopes; the first keeps 0
        self.assertEqual([p["interpolation"] for p in pts], [0, P.slope(pts[0], pts[1]), P.slope(pts[1], pts[2])])
        self.assertEqual((out["samples"][0]["loop_end"], out["vibrato_depth"], out["unknown12"]),
                         (20, 9, rec["unknown12"]))
        for bad in (dict(new, unknown12=[0] * 12), dict(new, nothing=1),
                    dict(new, samples=[dict(new["samples"][0], loop_end=10 ** 7)] + new["samples"][1:]),
                    dict(new, sample_indices=[99, 0, 0, 0]), dict(new, rows=[]),
                    dict(new, envelope=dict(new["envelope"], points=[{"x": 3, "y": 0}]))):
            self.assertRaises(P.PoolError, P.edited_instrument, rec, bad, lengths, "7")

    def test_samples_added_and_taken_out(self):
        files = E.Files(Game("xs", ROOT))
        wav = base64.b64encode(E.wav_bytes([0, 40, 80, 40, 0, -40, -80, -40] * 100, 16000)).decode()
        nxt = max(int(k) for k in MANIFEST["samples"]) + 1
        m, data, writes, deletes = E.apply_pool_edits(files, MANIFEST, {"newSamples": {str(nxt): wav}})
        self.assertEqual((len(data[nxt]), list(writes), deletes), (800, ["games/xs/sound/samples/%02d.wav" % nxt], []))
        self.assertIn(str(nxt), m["samples"])
        # one that isn't next, one an instrument plays
        self.assertRaises(E.EditError, E.apply_pool_edits, files, MANIFEST, {"newSamples": {str(nxt + 1): wav}})
        self.assertRaises(E.EditError, E.apply_pool_edits, files, MANIFEST, {"deleteSamples": [1]})
        # instrument 28 given the new sample: its old one (18, only 28's) can go
        rec = json.loads(json.dumps(MANIFEST["instruments"]["28"]))
        old = rec["sample_indices"][0]
        rec["sample_indices"][0] = nxt
        rec["samples"][0].update(loop_start=0, loop_end=800, byte_02=0, start_position=0)
        m, data, writes, deletes = E.apply_pool_edits(files, MANIFEST, {"newSamples": {str(nxt): wav},
                                                                        "instruments": {"28": rec},
                                                                        "deleteSamples": [old]})
        self.assertEqual((deletes, data[old]), (["games/xs/sound/samples/%02d.wav" % old], None))
        self.assertNotIn(str(old), m["samples"])
        pool = E.player_pool(Game("xs", ROOT), {"newSamples": {str(nxt): wav}, "instruments": {"28": rec},
                                                "deleteSamples": [old]})
        self.assertEqual(pool["music"]["instruments"][28]["waveIdx"][0], nxt)
        self.assertNotIn(old, pool["music"]["waves"])
        self.assertEqual(pool["music"]["waves"][nxt][1], 800)


def pcm(frames, rate, width, channels):
    buf = io.BytesIO()
    with wave.open(buf, "wb") as w:
        w.setnchannels(channels)
        w.setsampwidth(width)
        w.setframerate(rate)
        w.writeframes(frames)
    return buf.getvalue()


class Wavs(unittest.TestCase):
    def test_import(self):
        tone = [0.75 * math.sin(2 * math.pi * i / 50) for i in range(1000)]
        b16 = b"".join(struct.pack("<h", int(v * 32767)) for v in tone)
        r = E.import_wav(pcm(b16, 10000, 2, 1))
        self.assertEqual((r["rate"], r["length"], r["clipped"]), (10000, 1000, 0))
        self.assertEqual(r["peak"], 96)
        data, rate = E.sample_wav(__import__("base64").b64decode(r["wav"]), "x")
        self.assertEqual((rate, len(data)), (10000, 1000))
        self.assertEqual(r["how"], "16-bit mono at 10000 Hz")
        # 32-bit float, stereo, resampled: mixed to mono
        f32 = b"".join(struct.pack("<ff", v, -v) for v in tone)
        fmt = struct.pack("<HHIIHH", 3, 2, 10000, 80000, 8, 32)
        riff = b"WAVE" + b"fmt " + struct.pack("<I", len(fmt)) + fmt + b"data" + struct.pack("<I", len(f32)) + f32
        r = E.import_wav(b"RIFF" + struct.pack("<I", len(riff)) + riff, 5000)
        self.assertEqual((r["rate"], r["length"], r["peak"]), (5000, 500, 0))
        self.assertIn("32-bit float 2 channels mixed to mono", r["how"])
        for bad in (b"RIFF0000WAVEnope", pcm(b"", 8000, 1, 1)):
            self.assertRaises(E.EditError, E.import_wav, bad)
        self.assertRaises(E.EditError, E.sample_wav, pcm(b16, 10000, 2, 1), "a 16-bit sample")


if __name__ == "__main__":
    unittest.main()
