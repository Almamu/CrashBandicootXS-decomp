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


class NewInstruments(unittest.TestCase):
    """New pool instruments, instruments taken out, a pool instrument added to a song's XM, a song's settings and
    the sound-effect set's instruments (crashdata.audio.edit, pool)."""

    game = Game("xs", ROOT)

    def test_the_templates(self):
        # the empty instrument is instrument 0's record; a blank one is the build's instrument from a sample
        self.assertEqual(P.empty_instrument(), MANIFEST["instruments"]["0"])
        spec = {"sample": b"\x00" * 10, "loop": "none", "loop_start": 0, "loop_end": 0, "volume": 64,
                "tune": G.tune_for_rate(G.XM_C4_RATE, False), "fixed_pitch": False, "envelope": None,
                "vibrato": (0, 0, 0)}
        self.assertEqual(P.blank_instrument(5), G.instrument_entry(spec, 5, self.fail))

    def test_new_instruments(self):
        top = max(int(k) for k in MANIFEST["instruments"])
        copy = json.loads(json.dumps(MANIFEST["instruments"]["7"]))
        copy["samples"][0]["pitch"] += 32
        body = {"instruments": {str(top + 1): copy, str(top + 2): P.blank_instrument(3)}}
        m, _, _, _ = E.apply_pool_edits(E.Files(self.game), MANIFEST, body)
        self.assertEqual((m["instruments"][str(top + 1)], m["instruments"][str(top + 2)]["sample_indices"]),
                         (copy, [3, 0, 0, 0]))
        # not the next number; a field the engine doesn't read; a loop past the sample
        for bad in ({str(top + 2): copy}, {str(top + 1): dict(copy, unknown12=[0] * 12)},
                    {str(top + 1): dict(copy, samples=[dict(copy["samples"][0], loop_end=10 ** 7)]
                                        + copy["samples"][1:])}):
            self.assertRaises(E.EditError, E.apply_pool_edits, E.Files(self.game), MANIFEST, {"instruments": bad})
        pool = E.player_pool(self.game, body)
        self.assertEqual(len(pool["music"]["instruments"]), top + 3)
        self.assertEqual(pool["music"]["instruments"][top + 1]["rows"][0]["tune"], copy["samples"][0]["pitch"])

    def test_instruments_taken_out(self):
        files = E.Files(self.game)
        keys = [t["song"] for t in MANIFEST["song_table"]]
        named = E.named_instruments(files, keys, {})
        unused = sorted(set(map(int, MANIFEST["instruments"])) - set(named) - {0})
        self.assertEqual(unused, [26, 34, 51])
        top = max(map(int, MANIFEST["instruments"]))
        # one in the middle becomes the empty instrument; the last one leaves the pool
        m, _, _, _ = E.apply_pool_edits(files, MANIFEST, {"deleteInstruments": [26, top]}, lambda: named)
        self.assertEqual(m["instruments"]["26"], P.empty_instrument())
        self.assertNotIn(str(top), m["instruments"])
        self.assertRaises(E.EditError, E.apply_pool_edits, files, MANIFEST, {"deleteInstruments": [7]}, lambda: named)
        self.assertRaises(E.EditError, E.apply_pool_edits, files, MANIFEST, {"deleteInstruments": [0]}, lambda: named)
        # a song given the instrument names it then
        self.assertRaises(E.EditError, E.player_pool, self.game, {"songInstruments": {"intro": [26]},
                                                                  "deleteInstruments": [26]})

    def test_a_pool_instrument_added_to_a_song(self):
        buf = (SOUND / "songs" / "intro.xm").read_bytes()
        names = E.xm_instrument_names(buf)
        out = E.xm_with_instruments(buf, [26, 51])
        self.assertEqual(E.xm_instrument_names(out), names + ["Instrument 26", "Instrument 51"])
        xm = G.read_xm("intro.xm", out)
        self.assertEqual(xm["gax_instrument_indices"][-2:], [26, 51])
        self.assertEqual(E.structure(out), E.structure(buf))
        self.assertRaises(E.EditError, E.xm_with_instruments, buf, [int(names[0].split()[1])])
        # a cell playing it: the song built with it plays pool instrument 26 there
        n = len(names) + 1
        body = {"songInstruments": {"intro": [26]},
                "patterns": {"intro": [{"pattern": 0, "row": 0, "channel": 0, "cell": [49, n, 0, 0, 0]}]}}
        song = E.player_song(self.game, "intro", unsaved=body)
        streams = base64.b64decode(song["patterns"])
        self.assertIn(bytes([0x80 | 49, 26]), streams)
        self.assertRaises(E.EditError, E.player_song, self.game, "intro", unsaved={"songInstruments": {"intro": [99]}})

    def test_song_settings(self):
        files = E.Files(self.game)
        keys = [t["song"] for t in MANIFEST["song_table"]]
        st = {"sample_rate": 26760, "volume": 300, "half_rate_fx": 1,
              "mixer_data_bytes": list(struct.pack("<7I", 3, 40, 10, 15, 6, 0, 0))}
        m, new, writes = E.apply_settings(files, MANIFEST, keys, {}, {"songSettings": {"warp_room": st}})
        self.assertEqual(({f: m["songs"]["warp_room"][f] for f in st}, new, writes), (st, {}, []))
        # a ROM song keeps its scaffold
        self.assertTrue(G.scaffolded(m["songs"]["warp_room"]))
        song = E.player_song(self.game, "warp_room", unsaved={"songSettings": {"warp_room": st}})
        self.assertEqual((song["mixRate"], song["volume"], song["halfRateFx"], song["dsp"]),
                         (26760, 300, 1, [3, 40, 10, 15, 6, 0, 0]))
        # unchanged: the manifest as it was
        same = {f: MANIFEST["songs"]["warp_room"][f] for f in E.ROM_SETTINGS}
        self.assertEqual(E.apply_settings(files, MANIFEST, keys, {}, {"songSettings": {"warp_room": same}})[0],
                         MANIFEST)
        bad = [dict(st, sample_rate=22050), dict(st, volume=-1), dict(st, half_rate_fx=2),
               dict(st, mixer_data_bytes=list(struct.pack("<7I", 9, 40, 10, 0, 0, 0, 0))),
               dict(st, mixer_data_bytes=list(struct.pack("<7I", 3, 40, 300, 0, 0, 0, 0))),
               dict(st, sample_rate=42049, mixer_data_bytes=list(struct.pack("<7I", 3, 400, 10, 0, 0, 0, 0))),
               dict(st, title="a ROM song's title is its bytes")]
        for b in bad:
            self.assertRaises(E.EditError, E.apply_settings, files, MANIFEST, keys, {}, {"songSettings": {"warp_room": b}})

    def test_a_new_songs_settings(self):
        xm = base64.b64encode((SOUND / "songs" / "drums.xm").read_bytes()).decode()
        st = dict(G.NEW_SONG_DEFAULTS, sample_rate=21025, volume=200, title="test", artist=None)
        st = {f: st[f] for f in E.NEW_SETTINGS}
        body = {"newSongs": [{"key": "test_song", "xm": xm}], "songSettings": {"test_song": st}}
        song = E.player_song(self.game, "test_song", unsaved=body)
        self.assertEqual((song["mixRate"], song["volume"]), (21025, 200))
        e = E.unsaved_songs(E.Files(self.game), MANIFEST, body)
        self.assertEqual(e.settings["test_song"]["sample_rate"], 21025)

    def test_the_sound_effect_sets_instruments(self):
        sfx = json.loads((SOUND / "gax_sfx_manifest.json").read_text())
        rec = json.loads(json.dumps(sfx["instruments"]["9"]))
        rec["samples"][0]["pitch"] += 100
        rec["envelope"] = {"sustain": None, "loop_start": None, "loop_end": None,
                           "points": [{"x": 0, "y": 255}, {"x": 20, "y": 0}]}
        out, _, _, _ = E.apply_sfx_edits(E.Files(self.game), {"sfxInstruments": {"9": rec}})
        self.assertEqual(out["instruments"]["9"]["samples"][0]["pitch"], rec["samples"][0]["pitch"])
        self.assertEqual(out["instruments"]["10"], sfx["instruments"]["10"])
        pool = E.player_pool(self.game, {"sfxInstruments": {"9": rec}})
        self.assertEqual(pool["sfx"]["instruments"][9]["rows"][0]["tune"], rec["samples"][0]["pitch"])
        # every record of the set through the edit unchanged is itself
        self.assertEqual(E.apply_sfx_edits(E.Files(self.game), {"sfxInstruments": {
            k: v for k, v in sfx["instruments"].items() if k != "0"}})[0], sfx)
        for bad in ({"0": rec}, {"999": rec}, {"9": dict(rec, sample_indices=[200, 0, 0, 0])}):
            self.assertRaises(E.EditError, E.apply_sfx_edits, E.Files(self.game), {"sfxInstruments": bad})


class NewSfxInstruments(unittest.TestCase):
    """New instruments and samples of the sound-effect set, and ones taken out (crashdata.audio.edit.apply_sfx_edits,
    pool.blank_sfx_instrument)."""

    game = Game("xs", ROOT)
    sfx = json.loads((SOUND / "gax_sfx_manifest.json").read_text())
    table = json.loads((SOUND / "sfx_table.json").read_text())

    def test_the_blank_one_is_the_roms(self):
        # instrument 40 is a ROM sound effect as the set's are: a fixed pitch at the set's one rate
        self.assertEqual(P.blank_sfx_instrument(40), self.sfx["instruments"]["40"])

    def test_new_instruments_and_samples(self):
        files = E.Files(self.game)
        top = max(int(k) for k in self.sfx["instruments"])
        nxt = max(int(k) for k in self.sfx["samples"]) + 1
        wav = E.wav_bytes([(i % 20) * 6 - 60 for i in range(1500)], 8003)
        copy = json.loads(json.dumps(self.sfx["instruments"]["9"]))
        copy["samples"][0]["pitch"] -= 64
        body = {"newSfxSamples": {str(nxt): base64.b64encode(wav).decode()},
                "sfxInstruments": {str(top + 1): P.blank_sfx_instrument(nxt), str(top + 2): copy}}
        m, data, writes, deletes = E.apply_sfx_edits(files, body)
        self.assertEqual((m["instruments"][str(top + 1)]["sample_indices"], m["samples"][str(nxt)]),
                         ([nxt, 0, 0, 0], {"has_data": True}))
        self.assertEqual((len(data[nxt]), list(writes), deletes),
                         (1500, ["games/xs/sound/sfx_samples/%02d.wav" % nxt], []))
        pool = E.player_pool(self.game, body)
        self.assertEqual(len(pool["sfx"]["instruments"]), top + 3)
        self.assertEqual(pool["sfx"]["waves"][nxt][1], 1500)
        self.assertEqual(pool["sfx"]["instruments"][top + 2]["rows"][0]["tune"], copy["samples"][0]["pitch"])
        # a sound effect plays the new slot, saved with it
        table = json.loads(json.dumps(self.table))
        table["entries"].append({"slot_id": top + 2, "pitch_offset": 0, "volume": 256, "name": "TEST", "comment": ""})
        plan = E.plan_save(self.game, dict(body, sfxTable=table))
        self.assertEqual(sorted(r for r, _ in plan["json"]), ["games/xs/sound/gax_sfx_manifest.json",
                                                             "games/xs/sound/sfx_table.json"])
        # not the next numbers; a wave row past its new sample
        for bad in ({"sfxInstruments": {str(top + 2): copy}}, {"newSfxSamples": {str(nxt + 1): body["newSfxSamples"][
                str(nxt)]}}, dict(body, sfxInstruments={str(top + 1): dict(P.blank_sfx_instrument(nxt), samples=[
                    dict(P.blank_sfx_instrument()["samples"][0], loop_start=0, loop_end=2000)]
                    + P.blank_sfx_instrument()["samples"][1:])})):
            self.assertRaises(E.EditError, E.apply_sfx_edits, files, bad)
        # a slot past the set's
        table["entries"][-1]["slot_id"] = top + 3
        self.assertRaises(E.EditError, E.plan_save, self.game, dict(body, sfxTable=table))

    def test_taken_out(self):
        files = E.Files(self.game)
        played = {e["slot_id"] for e in self.table["entries"] if "slot_id" in e}
        unused = sorted(set(map(int, self.sfx["instruments"])) - played - {0})
        self.assertEqual(unused, [19, 86])
        m, _, _, _ = E.apply_sfx_edits(files, {"deleteSfxInstruments": [19]}, self.table["entries"])
        self.assertEqual(m["instruments"]["19"], P.empty_instrument())
        # its sample, which no instrument plays then, taken out too: the file goes
        m, data, _, deletes = E.apply_sfx_edits(files, {"deleteSfxInstruments": [19], "deleteSfxSamples": [19]},
                                                self.table["entries"])
        self.assertEqual((str(19) in m["samples"], data, deletes),
                         (False, {19: None}, ["games/xs/sound/sfx_samples/19.wav"]))
        crate = next(e["slot_id"] for e in self.table["entries"] if e.get("name") == "CRATE_BREAK")
        for bad in ({"deleteSfxInstruments": [crate]}, {"deleteSfxInstruments": [0]}, {"deleteSfxSamples": [crate]}):
            self.assertRaises(E.EditError, E.apply_sfx_edits, files, bad, self.table["entries"])


def own_xm(volume=48):
    """drums.xm with an instrument of its own (not `Instrument N`: a sample, a loop, an envelope with a sustain
    point) its first cell plays. Returns (the XM's bytes, the instrument's number)."""
    buf = (SOUND / "songs" / "drums.xm").read_bytes()
    count = len(E.xm_instrument_names(buf))
    wave_ = bytes(round(56 * math.sin(2 * math.pi * i / 64)) & 0xFF for i in range(256))
    head = struct.pack("<I22sBHI", 263, b"own lead".ljust(22, b"\0"), 0, 1, 40) + bytes(96)
    head += b"".join(struct.pack("<HH", x, y) for x, y in [(0, 64), (6, 44), (12, 40), (24, 0)]).ljust(48, b"\0")
    head += bytes(48) + bytes([4, 0, 2, 0, 0, 0, 0, 0, 1 | 2, 0, 0, 0, 0, 0])
    sample = struct.pack("<IIIBbBBbB22s", 256, 0, 256, volume, 0, 1, 0x80, 0, 0, b"sine")
    out = bytearray(buf + head.ljust(263, b"\0") + sample + bytes((v - p) & 0xFF for v, p in zip(wave_, b"\0" + wave_)))
    struct.pack_into("<H", out, 72, count + 1)
    out, _ = E.xm_with_cells(bytes(out), [(0, 0, 0, bytes([49, count + 1, 0, 0, 0]))])
    return out, count + 1


class OwnInstruments(unittest.TestCase):
    """A new song's own XM instruments, edited in its XM (crashdata.audio.edit.xm_with_own_instrument): read as the
    build reads them, written back changing only what the edit changes, played as the build makes them."""

    game = Game("xs", ROOT)

    def test_read(self):
        buf, n = own_xm()
        own = E.own_instruments(E.Files(self.game), G.read_xm("own.xm", buf))
        self.assertEqual(list(own), [n])
        self.assertEqual(own[n]["spec"], {
            "sample": 0, "volume": 48, "relative_note": 0, "finetune": 0, "loop": "forward", "loop_start": 0,
            "loop_end": 256, "envelope": {"points": [[0, 64], [6, 44], [12, 40], [24, 0]], "sustain": 2,
                                          "loop_start": None, "loop_end": None},
            "vibrato": {"depth": 0, "rate": 0}})
        self.assertEqual((own[n]["length"], own[n]["bits"], own[n]["plays"]), (256, 8, [0]))
        # the ROM songs have none: their XMs' instruments are the pool's
        for p in sorted((SOUND / "songs").glob("*.xm")):
            self.assertEqual(E.own_instruments(E.Files(self.game), G.read_xm(str(p))), {}, p.name)

    def test_unchanged_is_the_same_bytes(self):
        buf, n = own_xm(volume=70)  # over 64 (read as 64): an edit that keeps it keeps its byte
        own = E.own_instruments(E.Files(self.game), G.read_xm("own.xm", buf))[n]
        self.assertEqual(own["spec"]["volume"], 64)
        self.assertEqual(E.xm_with_own_instrument(E.Files(self.game), buf, n, own["spec"]), buf)

    def test_an_edit(self):
        files = E.Files(self.game)
        buf, n = own_xm()
        spec = E.own_instruments(files, G.read_xm("own.xm", buf))[n]["spec"]
        edit = dict(spec, volume=32, relative_note=12, finetune=-20, loop="pingpong", loop_start=10, loop_end=200,
                    envelope={"points": [[0, 64], [10, 30], [20, 0]], "sustain": None, "loop_start": 0, "loop_end": 1},
                    vibrato={"depth": 8, "rate": 18})
        out = E.xm_with_own_instrument(files, buf, n, edit)
        self.assertEqual(len(out), len(buf))
        xm = G.read_xm("own.xm", out)
        self.assertEqual(E.own_instruments(files, xm)[n]["spec"], edit)
        # the patterns and the other instruments keep their bytes
        self.assertEqual(E.structure(out), E.structure(buf))
        spans = E.xm_instrument_spans(buf)
        self.assertEqual(out[:spans[n - 1][0]], buf[:spans[n - 1][0]])
        # the build's instrument from it: its tune, loop, volume, envelope (4 times XM's), vibrato (a quarter rate)
        body = {"newSongs": [{"key": "own_song", "xm": base64.b64encode(buf).decode()}],
                "xmInstruments": {"own_song": {str(n): edit}}}
        pool = E.player_pool(self.game, body)
        ins = pool["music"]["instruments"][pool["xmPool"]["own_song"][n - 1]]
        tune = round(384 * math.log2(G.XM_C4_RATE / G.PERIOD_0_HZ) + 32 * 12 - 20 / 4) - (G.XM_C4 - 2) * 32
        self.assertEqual((ins["rows"][0]["tune"], ins["rows"][0]["pingPong"], ins["rows"][0]["sweepMin"],
                          ins["rows"][0]["sweepMax"]), (tune, 1, 10, 200))
        self.assertEqual(ins["seq"][0]["fx"][0], 12 << 8 | 128)
        self.assertEqual([p["value"] for p in ins["envelope"]["points"]], [255, 120, 0])
        self.assertEqual((ins["envelope"]["loopStart"], ins["envelope"]["loopEnd"]), (0, 1))
        self.assertEqual((ins["vibratoDepth"], ins["vibratoSpeed"]), (8, 4))
        # the envelope off; a vibrato on an instrument whose waveform isn't a sine: the save makes it one
        out = E.xm_with_own_instrument(files, buf, n, dict(spec, envelope=None))
        self.assertIsNone(E.own_instruments(files, G.read_xm("own.xm", out))[n]["spec"]["envelope"])
        start = E.xm_instrument_spans(buf)[n - 1][0]
        square = bytearray(buf)
        square[start + 235] = 1
        out = E.xm_with_own_instrument(files, bytes(square), n, dict(spec, vibrato={"depth": 4, "rate": 8}))
        self.assertEqual(out[start + 235:start + 239], bytes([0, 0, 4, 8]))

    def test_a_new_sound(self):
        files = E.Files(self.game)
        buf, n = own_xm()
        spec = E.own_instruments(files, G.read_xm("own.xm", buf))[n]["spec"]
        sound = [((i // 10) % 2) * 80 - 40 for i in range(1000)]
        wav = base64.b64encode(E.wav_bytes(sound, 16000)).decode()
        out = E.xm_with_own_instrument(files, buf, n, dict(spec, loop_end=900, wav=wav))
        own = E.own_instruments(files, G.read_xm("own.xm", out))[n]
        self.assertEqual((own["length"], own["spec"]["loop_end"]), (1000, 900))
        self.assertEqual(base64.b64decode(own["data"]), bytes(s & 0xFF for s in sound))
        # a loop past the new sound's end
        self.assertRaises(E.EditError, E.xm_with_own_instrument, files, buf, n, dict(spec, loop_end=1200, wav=wav))

    def test_refused(self):
        files = E.Files(self.game)
        buf, n = own_xm()
        spec = E.own_instruments(files, G.read_xm("own.xm", buf))[n]["spec"]
        env = dict(spec["envelope"])
        for i, s in [(n, dict(spec, volume=65)), (n, dict(spec, finetune=200)), (n, dict(spec, loop_end=300)),
                     (n, dict(spec, envelope=dict(env, points=[[k * 2, 10] for k in range(13)]))),
                     (n, dict(spec, envelope=dict(env, points=[[0, 64], [0, 10]]))),
                     (n, dict(spec, envelope=dict(env, loop_start=1, loop_end=None))),
                     (n, dict(spec, pan=1)), (n, dict(spec, sample=1)), (n + 1, spec),
                     # the pool's `Instrument N`, not the module's own
                     (1, spec)]:
            self.assertRaises(E.EditError, E.xm_with_own_instrument, files, buf, i, s)
        # a ROM song's are the pool's
        self.assertRaises(E.EditError, E.plan_save, self.game, {"xmInstruments": {"drums": {"1": spec}}})


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
