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
import math
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


# A new song's own XM instrument (docs/audio.md, "From the XM"): own_note, a song of one note on an XM instrument
# with its own sample (own_xm_instrument), which the editor then edits (its sound, loop, tune, envelope, vibrato).
OWN_SONG = "own_note"


def own_xm_instrument(name=b"own lead", length=400):
    """An XM instrument with its own sample, as a tracker writes one: 8-bit, `length` samples of a buzz looped
    forward over its second half, volume 48, a volume envelope of 4 points with a sustain point, no vibrato."""
    wave_ = bytes(round(60 * math.sin(2 * math.pi * i / 50) + (20 if i % 50 < 25 else -20)) & 0xFF
                  for i in range(length))
    deltas = bytes((v - p) & 0xFF for v, p in zip(wave_, b"\0" + wave_))
    env = [(0, 64), (4, 48), (16, 36), (40, 0)]
    head = struct.pack("<I22sBHI", 263, name.ljust(22, b"\0"), 0, 1, 40) + bytes(96)
    head += b"".join(struct.pack("<HH", x, y) for x, y in env).ljust(48, b"\0") + bytes(48)
    head += bytes([len(env), 0, 2, 0, 0, 0, 0, 0, 1 | 2, 0, 0, 0, 0, 0]) + bytes(4)
    sample = struct.pack("<IIIBbBBbB22s", length, length // 2, length // 2, 48, 0, 1, 0x80, 0, 0, b"buzz")
    return head.ljust(263, b"\0") + sample + deltas


def own_note_xm(note=NOTE, hold=NOTE_HOLD, rows=NOTE_ROWS, song=OWN_SONG, instrument=None):
    """note_xm's song, its note on its own instrument (own_xm_instrument, or `instrument`'s bytes) instead of the
    pool's."""
    buf = note_xm(0, note, hold, rows)
    head = bytearray(buf[:buf.index(b"Instrument 0") - 4])  # the module and its pattern, before the instrument
    head[17:37] = song.encode().ljust(20, b"\0")
    return bytes(head) + (own_xm_instrument() if instrument is None else instrument)


# A new song's own instrument with two samples and its key map (docs/audio.md, "From the XM": a GAX2 instrument
# has one sample, the one the key map plays on the song's notes), renamed; a new own instrument from a WAV and an
# empty one, around a pool instrument the song is given too; and json_note, a new song of one note on a new
# instruments/<name>.json instrument, all added by the editor.
KEYS_SONG, JSON_SONG, JSON_INSTRUMENT = "own_keys", "json_note", "json_lead"


def two_sample_instrument(name=b"two keys"):
    """own_xm_instrument with a second sample (a sine of 64 samples, 6 periods, unlooped, volume 64): its key map
    plays the first on every note, as a tracker's new instrument does."""
    one = own_xm_instrument(name)
    sine = bytes(round(70 * math.sin(2 * math.pi * i / 64)) & 0xFF for i in range(384))
    deltas = bytes((v - p) & 0xFF for v, p in zip(sine, b"\0" + sine))
    head = bytearray(one[:263])
    struct.pack_into("<H", head, 27, 2)
    first = one[263:263 + 40]
    second = struct.pack("<IIIBbBBbB22s", len(sine), 0, 0, 64, 0, 0, 0x80, 0, 0, b"sine")
    return bytes(head) + first + second + one[263 + 40:] + deltas


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
      (EDITED_SFX plays it);
    - own_note, a new song of one note on its own XM instrument
      (own_note_xm), the instrument edited: a new sound (a square at
      12000 Hz, C-4 tuned to play it there) looped ping-pong, its volume,
      a new envelope with a loop, and an auto-vibrato;
    - own_keys, a new song on its own XM instrument of two samples
      (two_sample_instrument), renamed, its key map playing the second on
      the song's notes, that one looped and its volume set, the first a
      new sound no note plays; a new own instrument from a WAV with an
      envelope, pool instrument 7 and a new empty one added around it, and
      cells playing the first two;
    - json_note, a new song of one note on json_lead, a new
      instruments/<name>.json instrument with its WAV, looped, with an
      envelope and a vibrato;
    - the sound-effect set's new instruments: 88, a blank one (the set's
      one rate) on a new sample 88 (a decaying chirp) with an envelope,
      and 89, a copy of instrument 9 tuned down 2 semitones (EDITED_SFX
      plays both)."""
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
    # own_note's own instrument: a square at 12000 Hz (C-4 plays it there: relative note 6, finetune 32),
    # ping-pong looped, at volume 40, a new envelope looping between its points 1 and 2, and an auto-vibrato
    square = [(40 if (i // 15) % 2 else -40) for i in range(1200)]
    own = {"samples": [{"volume": 40, "relative_note": 6, "finetune": 32, "loop": "pingpong", "loop_start": 300,
                        "loop_end": 900, "wav": E._b64(E.wav_bytes(square, 12000))}],
           "envelope": {"points": [[0, 64], [6, 40], [18, 56], [48, 0]], "sustain": 2, "loop_start": 1, "loop_end": 2},
           "vibrato": {"depth": 10, "rate": 22}}
    # own_keys's instrument 1 renamed, its key map playing its second sample from C-4 up (the song's notes), that
    # sample looped forward at volume 50, its first one a new sound (which no note plays); a new instrument 2 from
    # a WAV (a saw at 16000 Hz, its tune for C-4 to play it there: relative note 11, finetune 77) with an envelope;
    # pool instrument 7 at 3 (songInstruments); a new empty instrument 4, named, which no cell plays
    saw = [round(-60 + 120 * (i % 40) / 40) for i in range(1600)]
    keys = {"1": {"name": "split lead", "keymap": [0] * 48 + [1] * 48,
                  "samples": [{"loop": "none", "wav": E._b64(E.wav_bytes(square[:200], 8363))},
                              {"volume": 50, "loop": "forward", "loop_start": 64, "loop_end": 384}]},
            "2": {"name": "saw", "samples": [{"relative_note": 11, "finetune": 77, "loop": "forward",
                                              "loop_start": 400, "loop_end": 1600,
                                              "wav": E._b64(E.wav_bytes(saw, 16000))}],
                  "keymap": [0] * 96, "envelope": {"points": [[0, 64], [10, 30], [40, 0]], "sustain": 1,
                                                   "loop_start": None, "loop_end": None}},
            "4": {"name": "not yet"}}
    # json_lead, a new instruments/<name>.json instrument with its WAV (a decaying pulse at 11025 Hz, looped)
    pulse = [round((50 if (i // 20) % 2 else -50) * (1 if i < 1100 else 0.6)) for i in range(2200)]
    named = {"comment": "the music editor's JSON instrument", "sample": "samples/%s.wav" % JSON_INSTRUMENT,
            "loop": "forward", "loop_start": 1100, "loop_end": 2200, "volume": 48,
            "envelope": {"points": [[0, 64], [8, 40], [30, 20]], "sustain": 2},
            "vibrato": {"delay": 4, "depth": 12, "speed": 5}}
    # the sound-effect set's new sample 88 (a decaying chirp at the set's rate) and new instruments 88 (blank, on
    # it, with an envelope) and 89 (instrument 9 tuned down 2 semitones)
    nxt_sfx = max(int(k) for k in sfx["samples"]) + 1
    chirp = [round(90 * (1 - i / 3000) * math.sin(2 * math.pi * (300 + i / 10) * i / 8000)) for i in range(3000)]
    s88 = P.blank_sfx_instrument(nxt_sfx)
    s88["envelope"] = {"sustain": None, "loop_start": None, "loop_end": None,
                       "points": [{"x": 0, "y": 200}, {"x": 30, "y": 255}, {"x": 90, "y": 0}]}
    s89 = json.loads(json.dumps(sfx["instruments"]["9"]))
    s89["samples"][0]["pitch"] -= 64
    top_sfx = max(int(k) for k in sfx["instruments"])
    return {
        "structures": {"drums": st, "main_menu_japan": japan},
        "patterns": {"jungle": [{"pattern": 0, "row": 0, "channel": 0, "cell": [14, 1, 0, 7, 0x64]},
                                {"pattern": 2, "row": 6, "channel": 1, "cell": [0, 0, 1, 11, 0x40]}],
                     "cutscenes": [{"pattern": 0, "row": 0, "channel": 0, "cell": [49, len(cutscenes) + 1, 0, 0, 0]}],
                     KEYS_SONG: [{"pattern": 0, "row": 10, "channel": 0, "cell": [53, 2, 0, 0, 0]},
                                 {"pattern": 0, "row": 20, "channel": 0, "cell": [56, 3, 0, 0, 0]},
                                 {"pattern": 0, "row": 30, "channel": 0, "cell": [E.XM_KEY_OFF, 0, 0, 0, 0]}]},
        "instruments": {"43": drum, "28": lead, str(new): ins},
        "newSamples": {str(nxt): E._b64(E.wav_bytes([b - 256 if b > 127 else b for b in buzz], 16000))},
        "deleteSamples": [gone],
        "songInstruments": {"cutscenes": [new], KEYS_SONG: [7]},
        "newSongs": [{"key": NOTE_SONG, "xm": E._b64(note_xm(new)), "comment": "the music editor's note test"},
                     {"key": OWN_SONG, "xm": E._b64(own_note_xm()), "comment": "the music editor's own instrument"},
                     {"key": KEYS_SONG, "xm": E._b64(own_note_xm(hold=36, song=KEYS_SONG,
                                                                 instrument=two_sample_instrument())),
                      "comment": "the music editor's key map"},
                     {"key": JSON_SONG, "xm": E._b64(own_note_xm(song=JSON_SONG, instrument=struct.pack(
                         "<I22sBH", 29, JSON_INSTRUMENT.encode(), 0, 0))),
                      "comment": "the music editor's JSON instrument"}],
        "songSettings": {"warp_room": {"sample_rate": 26760, "volume": 300, "half_rate_fx": 1,
                                       "mixer_data_bytes": list(struct.pack("<7I", 4, 45, 9, 25, 5, 30, 0))}},
        "xmInstruments": {OWN_SONG: {"1": own}, KEYS_SONG: keys},
        "jsonInstruments": {JSON_INSTRUMENT: {"json": named, "wav": E._b64(E.wav_bytes(pulse, 11025))}},
        "sfxInstruments": {"31": s31, str(top_sfx + 1): s88, str(top_sfx + 2): s89},
        "newSfxSamples": {str(nxt_sfx): E._b64(E.wav_bytes(chirp, 8003))},
    }


# the edited tree's sound effects: instrument 31 (edited) and 9 (as it was) over jungle, its music muted, and over
# warp_room (edited settings) with its music; the set's new instruments 88 and 89 over jungle, muted
EDITED_SFX = [
    {"song": "jungle", "musicVolume": 0, "frames": 300, "events": [[10, 31, 0, 0, 255], [150, 9, 1, 0, 255]]},
    {"song": "warp_room", "frames": 400, "events": [[30, 31, 0, 0, 200], [200, 31, 1, 0, 255]]},
    {"song": "jungle", "musicVolume": 0, "frames": 300, "events": [[10, 88, 0, 0, 255], [160, 89, 1, 0, 230]]},
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
