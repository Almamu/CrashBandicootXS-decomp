"""crashdata.audio.edit's pieces (docs/editors.md, "The music editor"):
the XM patterns read as cells and packed back as the songs' XMs pack
them, a cell edit that writes only its pattern, the pattern sharing of
the ROM's songs, a song's patterns as the build lays them out, and WAVs
made into GAX2 samples.

    python3 -m unittest discover -s packages/crashdata/tests
"""
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
