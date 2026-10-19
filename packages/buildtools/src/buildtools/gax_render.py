"""tools/gax_render.py: songs and sound effects as the PC port's GAX2 mixes them (#1167, docs/editors.md "The music editor").

    tools/gax_render.py render SONG FRAMES OUT.wav [--music-volume V] [--fx FRAME:SLOT:VOICE:PRIO:VOL ...]
    tools/gax_render.py check [REFERENCE] [--make ARG ...]   the reference's renders, against their SHA-1s
    tools/gax_render.py write [REFERENCE] [--make ARG ...]   the same, writing the SHA-1s

(the edited songs' renders build the PC port in a copy of the tree with
the editor's edits, `make PLATFORM=pc ARG...`: give the arguments the PC
port was built with)

A render is a GAX2 trace written for it (platform/pc/src/gax_trace.c:
`init` with the song's offset in the music block, as AudioContext::StartSong
starts it, 3 sound-effect voices; `music_vol`; per frame the `fx_ex` and
`fx_vol` calls PlaySfx would make, then `play` and `irq`) replayed by
`build/pc/crashbandicootxs_pc --gax-trace TRACE --wav OUT`, which writes each
block GAX_play mixes: 8-bit mono at the song's mix rate. The PC port's GAX2
is lib/gax's matched C with the ARM routines in C, block for block the
GBA's (docs/platforms.md, "The sound").

The reference (apps/editor/test/gax_reference.json) is what the music
editor's own GAX2 (apps/editor/src/lib/music/gax.ts) is checked against
(apps/editor/test/music.test.ts): each song's first frames, a few
sound-effect sequences and a few songs the editor changed ("edited",
edited_body), by the SHA-1 of their samples as signed bytes. CI
(host.yml) renders them again with the PC port and checks the SHA-1s, so
the reference is the PC port's.
"""
import argparse
import base64
import hashlib
import json
import os
import re
import shutil
import struct
import subprocess
import sys
import tempfile
import wave
from pathlib import Path

PC = os.path.join("build", "pc", "crashbandicootxs_pc")
SONGS_H = os.path.join("build", "pc", "crashbandicootxs", "sound", "gax_songs.h")
REFERENCE = os.path.join("apps", "editor", "test", "gax_reference.json")


def song_offsets(path=SONGS_H):
    """GAX_SONG_<KEY> of the PC build's gax_songs.h: {key: offset in gGaxMusicData}."""
    with open(path) as f:
        return {m.group(1).lower(): int(m.group(2), 16)
                for m in re.finditer(r"#define GAX_SONG_(\w+) (0x[0-9a-fA-F]+)", f.read())}


def trace(offset, frames, music_volume=None, events=()):
    lines = ["init %d 0 65535 0 0 3 65535 0" % offset]
    if music_volume is not None:
        lines.append("music_vol -1 %d" % music_volume)
    by_frame = {}
    for e in events:
        by_frame.setdefault(e[0], []).append(e)
    for f in range(frames):
        for _, slot, voice, prio, vol in by_frame.get(f, ()):
            lines.append("fx_ex %d %d %d -1" % (slot, voice, prio))
            if vol >= 0:
                lines.append("fx_vol %d %d" % (voice, vol))
        lines += ["play 255 0", "irq"]
    return "\n".join(lines) + "\n"


def render(song, frames, out, music_volume=None, events=(), exe=PC, offsets=None):
    """The PC port's render of `song` into the WAV `out`: (its rate, its samples as signed bytes)."""
    offsets = offsets or song_offsets()
    if song not in offsets:
        sys.exit("gax_render: %s isn't a song of %s" % (song, SONGS_H))
    with tempfile.NamedTemporaryFile("w", suffix=".trace", delete=False) as t:
        t.write(trace(offsets[song], frames, music_volume, events))
    try:
        r = subprocess.run([exe, "--headless", "--quiet", "--wav", out, "--gax-trace", t.name], capture_output=True,
                           text=True)
    finally:
        os.unlink(t.name)
    if r.returncode:
        sys.exit("gax_render: %s failed (%d): %s%s" % (exe, r.returncode, r.stdout[-2000:], r.stderr[-2000:]))
    with wave.open(out) as w:
        return w.getframerate(), bytes((b - 128) & 0xFF for b in w.readframes(w.getnframes()))


def sha1(data):
    return hashlib.sha1(data).hexdigest()


# -- edited songs ---------------------------------------------------------------------
#
# The reference's "edited" renders are songs the music editor changed: the
# edits below, saved by the editor's backend (buildtools.editor_server's
# music save, crashdata.audio.edit.plan_save) in a copy of the tree, whose
# PC port (built there from the copied build, so only the sound is built
# again) renders them. music.test.ts plays the same edits unsaved
# (crashdata.audio.edit.player_song with them), as the editor plays them
# before they are saved.

def _cells(edits, buf, layout, pi):
    from crashdata.audio import edit as E

    cells = bytearray(E.unpack_cells(buf, layout, pi))
    for row, ch, cell in edits:
        k = (row * layout.channels + ch) * E.CELL
        cells[k:k + E.CELL] = bytes(cell)
    return cells


# "Play the note" (the Instruments tab): a one-channel song with a note on a pool instrument and a key off after
# HOLD rows, ROWS rows at speed 6 (apps/editor's model.ts instrumentSong). The reference has it as a new song the
# backend builds from an XM of that note (note_xm), which the PC port renders.
NOTE, NOTE_HOLD, NOTE_ROWS = 49, 16, 40
NOTE_SONG = "instrument_note"


def note_xm(instrument, note=NOTE, hold=NOTE_HOLD, rows=NOTE_ROWS):
    """An XM of one channel and one pattern: `note` on the pool's `Instrument N` on its first row, a key off on row
    `hold`, `rows` rows, speed 6, looping to its start."""
    from crashdata.audio import edit as E

    cells = bytearray(rows * E.CELL)
    cells[0:2] = bytes([note, 1])
    cells[hold * E.CELL] = E.XM_KEY_OFF
    packed = E.pack_cells(bytes(cells))
    head = b"Extended Module: " + NOTE_SONG.encode().ljust(20, b"\0") + b"\x1a" + b"the music editor".ljust(20)
    head += struct.pack("<HI", 0x104, 276) + struct.pack("<8H", 1, 0, 1, 1, 1, 1, 6, 125) + bytes(256)
    pattern = struct.pack("<IBHH", 9, 0, rows, len(packed)) + packed
    ins = struct.pack("<I22sBH", 29, b"Instrument %d" % instrument, 0, 0)
    return head + pattern + ins


def edited_body(root):
    """The music editor's edits of the reference's edited songs, as a save
    sends them (the tree at root's XMs, unedited, are their base):

    - drums (a ROM song) restructured: a copy of its pattern 1 played after
      its first order position, with GAX2's alternating speeds (7xy: 5
      ticks, then 3) on its first row and GAX2's own volume slide down by
      FF (volume column 01) on row 8; the loop point kept; speed 4;
    - jungle's cells: its first row's alternating speeds 6 and 4 (`764`;
      the ROM's 9 and 6), made in the order positions sharing the
      pattern, and a slide down by 0x40 (volume column 01, `B40`);
    - main_menu_japan a channel more, with a note on it;
    - the music pool, heard in jungle: instrument 43 (its drum) with a new
      volume envelope and tuned 2 semitones up; a new sample (a buzz,
      looped forward) for instrument 28, with a vibrato; the sample 28
      played before, which no other instrument plays, taken out;
    - a new pool instrument (the pool's next: sample 6 looped forward, a
      vibrato and a decaying envelope) played by cutscenes, which has it
      added to its XM's instruments, on its first row; and by
      instrument_note, a new song of that one note (note_xm: what "Play
      the note" plays);
    - warp_room's settings: mix rate 26760 Hz, volume 300, its sound
      effects at half rate and longer echo taps;
    - the sound-effect set's instrument 31 retuned with a new envelope
      (EDITED_SFX plays it)."""
    from crashdata.audio import edit as E

    songs = os.path.join(root, "games", "xs", "sound", "songs")

    def read(key):
        with open(os.path.join(songs, "%s.xm" % key), "rb") as f:
            return f.read()

    buf = read("drums")
    st = E.structure(buf)
    layout = E.XmLayout(buf)
    copy = _cells([(0, 0, [0, 0, 0, 7, 0x53]), (8, 0, [0, 0, 1, 11, 0xFF])], buf, layout, 1)
    st["patterns"].append({"rows": layout.patterns[1][2], "cells": E._b64(copy)})
    st["order"].insert(1, len(st["patterns"]) - 1)
    st["restart"] += 1
    st["speed"] = 4
    japan = E.structure(read("main_menu_japan"))
    n = japan["channels"]
    for p in japan["patterns"]:
        c = base64.b64decode(p["cells"])
        rows = [c[r * n * E.CELL:(r + 1) * n * E.CELL] + bytes(E.CELL) for r in range(p["rows"])]
        rows[0] = rows[0][:n * E.CELL] + bytes([61, 1, 0, 0, 0])
        p["cells"] = E._b64(b"".join(rows))
    japan["channels"] = n + 1
    # the music pool: instrument 43 (jungle's drum) with a new envelope and tuned 2 semitones up; a new
    # sample, a buzz looped forward, for instrument 28 (one of jungle's), with a vibrato; sample 18, which
    # only instrument 28 played, taken out
    with open(os.path.join(root, "games", "xs", "sound", "gax_manifest.json")) as f:
        manifest = json.load(f)
    drum = json.loads(json.dumps(manifest["instruments"]["43"]))
    drum["envelope"] = {"sustain": None, "loop_start": None, "loop_end": None,
                        "points": [{"x": 0, "y": 142}, {"x": 4, "y": 220}, {"x": 30, "y": 0}]}
    drum["samples"][0]["pitch"] += 64
    nxt = max(int(k) for k in manifest["samples"]) + 1
    buzz = bytes((128 + (i % 40) * 4 - 80 + (30 if i % 80 < 40 else -30)) & 0xFF for i in range(2000))
    lead = json.loads(json.dumps(manifest["instruments"]["28"]))
    gone = lead["sample_indices"][0]
    lead["sample_indices"][0] = nxt
    lead["samples"][0].update(byte_02=0, is_bidirectional=False, start_position=0, loop_start=200, loop_end=1800)
    lead["vibrato_depth"], lead["vibrato_speed"] = 30, 10
    # a new pool instrument: sample 6 (instrument 7's) looped forward, a vibrato, a decaying envelope
    new = max(int(k) for k in manifest["instruments"]) + 1
    from crashdata.audio import pool as P

    ins = P.blank_instrument(6)
    ins["samples"][0].update(loop_start=7282, loop_end=11523)
    ins["vibrato_delay"], ins["vibrato_depth"], ins["vibrato_speed"] = 6, 24, 6
    ins["envelope"] = {"sustain": 1, "loop_start": None, "loop_end": None,
                       "points": [{"x": 0, "y": 255}, {"x": 20, "y": 180}, {"x": 60, "y": 0}]}
    cutscenes = E.xm_instrument_names(read("cutscenes"))
    # the sound-effect set's instrument 31: tuned up 3 semitones, a new envelope
    with open(os.path.join(root, "games", "xs", "sound", "gax_sfx_manifest.json")) as f:
        sfx = json.load(f)
    s31 = json.loads(json.dumps(sfx["instruments"]["31"]))
    s31["samples"][0]["pitch"] += 96
    s31["envelope"] = {"sustain": None, "loop_start": None, "loop_end": None,
                       "points": [{"x": 0, "y": 255}, {"x": 12, "y": 60}, {"x": 40, "y": 0}]}
    return {
        "structures": {"drums": st, "main_menu_japan": japan},
        "patterns": {"jungle": [{"pattern": 0, "row": 0, "channel": 0, "cell": [14, 1, 0, 7, 0x64]},
                                {"pattern": 2, "row": 6, "channel": 1, "cell": [0, 0, 1, 11, 0x40]}],
                     "cutscenes": [{"pattern": 0, "row": 0, "channel": 0, "cell": [49, len(cutscenes) + 1, 0, 0, 0]}]},
        "instruments": {"43": drum, "28": lead, str(new): ins},
        "newSamples": {str(nxt): E._b64(E.wav_bytes([b - 256 if b > 127 else b for b in buzz], 16000))},
        "deleteSamples": [gone],
        "songInstruments": {"cutscenes": [new]},
        "newSongs": [{"key": NOTE_SONG, "xm": E._b64(note_xm(new)), "comment": "the music editor's note test"}],
        "songSettings": {"warp_room": {"sample_rate": 26760, "volume": 300, "half_rate_fx": 1,
                                       "mixer_data_bytes": list(struct.pack("<7I", 4, 45, 9, 25, 5, 30, 0))}},
        "sfxInstruments": {"31": s31},
    }


# the edited tree's sound effects: instrument 31 (edited) and 9 (as it was) over jungle, its music muted, and over
# warp_room (edited settings) with its music
EDITED_SFX = [
    {"song": "jungle", "musicVolume": 0, "frames": 300, "events": [[10, 31, 0, 0, 255], [150, 9, 1, 0, 255]]},
    {"song": "warp_room", "frames": 400, "events": [[30, 31, 0, 0, 200], [200, 31, 1, 0, 255]]},
]


def edited_tree(dest, make_args=()):
    """A copy of this tree at dest with edited_body() saved by the editor's
    backend and its PC port built (from a copy of build/pc, so only the
    sound is built again). Returns the executable."""
    from buildtools import editor_server
    from crashdata import mods

    mods.copy_tree(Path(dest))
    shutil.copytree(os.path.join("build", "pc"), os.path.join(dest, "build", "pc"), symlinks=True)
    include = os.path.join("build", "include")
    if os.path.isdir(include):
        shutil.copytree(include, os.path.join(dest, include), symlinks=True)
    app = editor_server.Server(dest)
    try:
        r = app.save_music(app.project("xs"), edited_body(dest))
    except editor_server.HttpError as e:
        sys.exit("gax_render: the edits don't save: %s %s" % (e.message, json.dumps(e.diagnostics)[:2000]))
    if not r["saved"]:
        sys.exit("gax_render: the edits saved nothing")
    b = subprocess.run(["make", "-j%d" % (os.cpu_count() or 1), "PLATFORM=pc"] + list(make_args), cwd=dest,
                       capture_output=True, text=True)
    if b.returncode:
        sys.exit("gax_render: the edited tree's PC port doesn't build: %s%s" % (b.stdout[-3000:], b.stderr[-3000:]))
    return os.path.join(dest, PC)


def reference_text(ref):
    """The reference as written: a song a line, a sequence's events on one."""
    out = ["{", ' "about": %s,' % json.dumps(ref["about"]), ' "frames": %d,' % ref["frames"], ' "songs": {']
    songs = list(ref["songs"].items())
    out += ['  %s: %s%s' % (json.dumps(k), json.dumps(v), "," if i < len(songs) - 1 else "")
            for i, (k, v) in enumerate(songs)]
    out += [" },", ' "sfx": [']
    for i, s in enumerate(ref["sfx"]):
        head = ", ".join("%s: %s" % (json.dumps(k), json.dumps(v)) for k, v in s.items() if k != "events")
        out.append('  {%s,\n   "events": [%s]}%s' % (head, ", ".join(json.dumps(e) for e in s["events"]),
                                                     "," if i < len(ref["sfx"]) - 1 else ""))
    out.append(" ]," if ref.get("edited") else " ]")
    if ref.get("edited"):
        e = ref["edited"]
        songs = list(e["songs"].items())
        out += [' "edited": {', '  "about": %s,' % json.dumps(e["about"]), '  "songs": {']
        out += ['   %s: %s%s' % (json.dumps(k), json.dumps(v), "," if i < len(songs) - 1 else "")
                for i, (k, v) in enumerate(songs)]
        if e.get("sfx"):
            out += ["  },", '  "sfx": [']
            for i, s in enumerate(e["sfx"]):
                head = ", ".join("%s: %s" % (json.dumps(k), json.dumps(v)) for k, v in s.items() if k != "events")
                out.append('   {%s,\n    "events": [%s]}%s' % (head, ", ".join(json.dumps(x) for x in s["events"]),
                                                              "," if i < len(e["sfx"]) - 1 else ""))
            out += ["  ]", " }"]
        else:
            out += ["  }", " }"]
    return "\n".join(out + ["}"]) + "\n"


def renders(ref):
    """Each render the reference names: (name, song, frames, music volume, events, its SHA-1)."""
    for song, digest in ref["songs"].items():
        yield song, song, ref["frames"], None, (), digest
    for i, s in enumerate(ref["sfx"]):
        yield "sfx %d (%s)" % (i, s["song"]), s["song"], s["frames"], s.get("musicVolume"), \
            [tuple(e) for e in s["events"]], s.get("sha1")


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    sub = ap.add_subparsers(dest="cmd", required=True)
    r = sub.add_parser("render")
    r.add_argument("song")
    r.add_argument("frames", type=int)
    r.add_argument("out")
    r.add_argument("--music-volume", type=int)
    r.add_argument("--fx", nargs="*", default=[], help="FRAME:SLOT:VOICE:PRIO:VOL")
    for name in ("check", "write"):
        c = sub.add_parser(name)
        c.add_argument("reference", nargs="?", default=REFERENCE)
        c.add_argument("--make", action="append", default=[], metavar="ARG",
                       help="an argument of the edited tree's `make PLATFORM=pc` (the build's own, e.g. HOST_SDL=1)")
    a = ap.parse_args(argv)
    if a.cmd == "render":
        events = [tuple(int(x) for x in e.split(":")) for e in a.fx]
        rate, data = render(a.song, a.frames, a.out, a.music_volume, events)
        print("%s: %d samples at %d Hz, sha1 %s" % (a.out, len(data), rate, sha1(data)))
        return 0
    with open(a.reference) as f:
        ref = json.load(f)
    offsets = song_offsets()
    bad = 0
    with tempfile.TemporaryDirectory() as tmp:
        for k, (name, song, frames, vol, events, digest) in enumerate(renders(ref)):
            _, data = render(song, frames, os.path.join(tmp, "%d.wav" % k), vol, events, offsets=offsets)
            got = sha1(data)
            if a.cmd == "write":
                if name == song and not events:
                    ref["songs"][song] = got
                else:
                    ref["sfx"][int(name.split()[1])]["sha1"] = got
            elif got != digest:
                bad += 1
                print("gax_render: %s: the PC port's %d frames are %s, the reference says %s" % (name, frames, got,
                                                                                                  digest))
        if ref.get("edited"):
            tree = os.path.join(tmp, "edited")
            exe = edited_tree(tree, a.make)
            edited_offsets = song_offsets(os.path.join(tree, SONGS_H))
            for song, digest in ref["edited"]["songs"].items():
                _, data = render(song, ref["frames"], os.path.join(tmp, "edited_%s.wav" % song), exe=exe,
                                 offsets=edited_offsets)
                got = sha1(data)
                if a.cmd == "write":
                    ref["edited"]["songs"][song] = got
                elif got != digest:
                    bad += 1
                    print("gax_render: edited %s: the PC port's %d frames are %s, the reference says %s"
                          % (song, ref["frames"], got, digest))
            # the edited tree's sound effects (an edited sound-effect instrument, a song with edited settings)
            if a.cmd == "write":
                ref["edited"]["sfx"] = [dict(s) for s in EDITED_SFX]
            for i, s in enumerate(ref["edited"].get("sfx", [])):
                _, data = render(s["song"], s["frames"], os.path.join(tmp, "edited_sfx_%d.wav" % i),
                                 s.get("musicVolume"), [tuple(e) for e in s["events"]], exe=exe,
                                 offsets=edited_offsets)
                got = sha1(data)
                if a.cmd == "write":
                    s["sha1"] = got
                elif got != s["sha1"]:
                    bad += 1
                    print("gax_render: edited sfx %d (%s): the PC port's %d frames are %s, the reference says %s"
                          % (i, s["song"], s["frames"], got, s["sha1"]))
    if a.cmd == "write":
        with open(a.reference, "w") as f:
            f.write(reference_text(ref))
        print("wrote %s" % a.reference)
        return 0
    n = len(ref["songs"]) + len(ref["sfx"]) + len(ref.get("edited", {}).get("songs", {})) + \
        len(ref.get("edited", {}).get("sfx", []))
    print("gax_render: %d of %d renders as the reference" % (n - bad, n))
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
