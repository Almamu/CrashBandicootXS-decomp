"""tools/gax_render.py: songs and sound effects as the PC port's GAX2 mixes them (#1167, docs/editors.md "The music editor").

    tools/gax_render.py render SONG FRAMES OUT.wav [--music-volume V] [--fx FRAME:SLOT:VOICE:PRIO:VOL ...]
    tools/gax_render.py check [REFERENCE]   the reference's renders, against their SHA-1s
    tools/gax_render.py write [REFERENCE]   the same, writing the SHA-1s

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
(apps/editor/test/music.test.ts): each song's first frames and a few
sound-effect sequences, by the SHA-1 of their samples as signed bytes. CI
(host.yml) renders them again with the PC port and checks the SHA-1s, so
the reference is the PC port's.
"""
import argparse
import hashlib
import json
import os
import re
import subprocess
import sys
import tempfile
import wave

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
    return "\n".join(out + [" ]", "}"]) + "\n"


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
    if a.cmd == "write":
        with open(a.reference, "w") as f:
            f.write(reference_text(ref))
        print("wrote %s" % a.reference)
        return 0
    n = len(ref["songs"]) + len(ref["sfx"])
    print("gax_render: %d of %d renders as the reference" % (n - bad, n))
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
