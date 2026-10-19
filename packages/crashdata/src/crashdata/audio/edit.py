"""The music and sound effects as the music editor sees them, and its edits written back (#1167, docs/editors.md "The music editor").

The music editor (apps/editor) lists the songs and sound effects, plays
them as GAX2 does (its own GAX2 player, apps/editor/src/lib/music/gax.ts,
on what this module gives it) and edits them; this module is what the
backend (buildtools.editor_server) gives it and how it writes the
edits. It works on a crashdata.common.game.Game, any tree's (XS's, a
mod's), binding crashdata.audio.gax to the tree (gax.use_game):

- view(game): the song table, each song's XM in brief and what plays it
  (the level themes, the screens, the cutscene slides), the screens'
  music, the sound-effect table and what plays each entry (code, the
  cutscene slides), the samples and the new instruments (instruments/<name>.json, with their sound);
- player_pool(game): what the player needs besides a song: GAX2's tables
  (lib/gax/data/gax_tables.c), the music pool's and the sound-effect
  set's instruments and samples, the song header of the voices;
- player_song(game, key, xm=None): one song as GAX2_init sees it (its
  GaxSongData, its channels' order lists, the packed patterns, the
  mixer's echo taps), as tools/gax_audio.py builds it: from the XM file,
  or from unsaved XM bytes;
- patterns(game, key): the XM's patterns as cells, its order list and,
  for a ROM song, which order positions share a pattern;
- import_wav(data, rate=None): a WAV made into a GAX2 sample (8-bit
  mono, resampled to `rate`);
- plan_save(game, body): the files a save changes, after its checks.

N-Tranced's sound isn't data yet (it has no games/n2/sound/): available(game)
says so.
"""
import base64
import contextlib
import io
import json
import math
import re
import struct
import threading
import wave
from pathlib import Path

from crashdata.audio import gax as G
from crashdata.audio import pool as P
from crashdata.common import diagnostics as D
from crashdata.common.game import Game

# gax.use_game binds module state: one tree at a time
LOCK = threading.RLock()


class EditError(Exception):
    def __init__(self, message, diagnostics=None):
        super().__init__(message)
        self.diagnostics = diagnostics


class Files:
    """Where a game's sound data is in its tree."""

    def __init__(self, game):
        self.game = game
        self.root = Path(game.root)
        self.sound = game.sound_dir
        self.manifest = self.sound / "gax_manifest.json"
        self.sfx_manifest = self.sound / "gax_sfx_manifest.json"
        self.music = self.sound / "music.json"
        self.sfx_table = self.sound / "sfx_table.json"
        self.songs = self.sound / "songs"
        self.samples = self.sound / "samples"
        self.sfx_samples = self.sound / "sfx_samples"
        self.instruments = self.sound / "instruments"
        self.levels = game.levels_dir / "levels.json"
        self.cutscenes = game.data_dir / "cutscenes.json"
        self.tables = self.root / "lib" / "gax" / "data" / "gax_tables.c"

    def rel(self, p):
        return Path(p).resolve().relative_to(self.root.resolve()).as_posix()

    def sound_rel(self, p):
        """A path from games/<game>/sound/."""
        return Path(p).resolve().relative_to(self.sound.resolve()).as_posix()


def available(game):
    return game.is_xs and Files(game).manifest.is_file()


@contextlib.contextmanager
def bound(game):
    """crashdata.audio.gax bound to the game's tree, under LOCK."""
    with LOCK:
        saved = G.ROOT
        G.use_game(game)
        try:
            yield
        finally:
            G.use_game(Game(game.name, saved))


def _diag(message, file=None, pointer=None, code="music"):
    return {"file": file, "pointer": pointer, "line": None, "col": None, "severity": D.ERROR, "code": code,
            "message": message}


def _b64(data):
    return base64.b64encode(bytes(data)).decode("ascii")


# ---------------------------------------------------------------------------
# GAX2's tables (lib/gax/data/gax_tables.c)

def _c_array(src, name):
    m = re.search(r"\b%s\[[^\]]*\]\s*=\s*\{(.*?)\};" % re.escape(name), src, re.S)
    if not m:
        raise EditError("lib/gax/data/gax_tables.c has no %s" % name)
    return [int(v.rstrip("uU"), 0) for v in re.findall(r"-?(?:0x[0-9A-Fa-f]+|\d+)[uU]?", m.group(1))]


def tables(files):
    """GAX2's period table (gGaxPeriodTable: Hz * 2048 per 1/32
    semitone), vibrato table and mix rates ({rate, timer})."""
    src = files.tables.read_text(encoding="latin-1")
    rates = _c_array(src, "gGaxMixRates")
    return {"period": _c_array(src, "gGaxPeriodTable"), "vibrato": _c_array(src, "gGaxVibratoTable"),
            "mixRates": [rates[i] for i in range(0, len(rates), 2)]}


# ---------------------------------------------------------------------------
# The player's data: the engine's structures (lib/gax/src/gax_internal.h)
# with their C names, from the manifest's records (crashdata.audio.gax)

def _s32(v):
    v &= 0xFFFFFFFF
    return v - (1 << 32) if v & 0x80000000 else v


def instrument_struct(ins):
    """A gax.Instrument as struct GaxChannelInstrument."""
    env = ins.envelope
    return {
        "empty": ins.byte_00,
        "waveIdx": list(ins.sample_indices),
        "vibratoDelay": ins.vibrato_delay, "vibratoDepth": ins.vibrato_depth, "vibratoSpeed": ins.vibrato_speed,
        # struct GaxInstrumentRow, from InstrumentSample.pack's fields
        "rows": [{"sweep": s.byte_02, "pingPong": 1 if s.is_bidirectional else 0, "start": _s32(s.start_position),
                  "sweepMin": _s32(s.loop_start), "sweepMax": _s32(s.loop_end), "sweepLen": _s32(s.int_10),
                  "sweepStep": _s32(s.ushort_14), "sweepRate": s.ushort_16 & 0xFFFF, "tune": s.pitch}
                 for s in ins.samples],
        "envelope": None if env is None else {
            "sustain": 0xFF if env.sustain is None else env.sustain,
            "loopStart": 0xFF if env.loop_start is None else env.loop_start,
            "loopEnd": 0xFF if env.loop_end is None else env.loop_end,
            "points": [{"pos": p.x, "slope": p.interpolation, "value": p.y} for p in env.points]},
        "seqSpeed": ins.row_speed,
        # struct GaxInstrumentSeqEntry, from InstrumentRow.pack's
        "seq": [{"note": r.relative_note_number & 0xFF, "fixedPitch": 1 if r.dont_use_note_pitch else 0,
                 "wave": r.sample_index, "fx": [(e.type & 0xFF) << 8 | (e.param & 0xFF) for e in r.effects]}
                for r in ins.rows],
    }


def _pool_struct(instruments, samples, dedup):
    """A pool as the player takes it: the instruments by number (None
    where there is none), and the samples, {number: [offset, length]} in
    one base64 block of signed 8-bit samples, laid out as the data set
    lays them out (gax.link_instruments: in number order, the music's
    stored once by content), since GAX2 can read a few samples past a
    sample's end."""
    n = max(instruments) + 1
    blob, waves, seen = bytearray(), {}, {}
    for idx in sorted(samples):
        data = samples[idx]
        if not data:
            continue
        if dedup and data in seen:
            waves[idx] = [seen[data], len(data)]
            continue
        seen.setdefault(data, len(blob))
        waves[idx] = [len(blob), len(data)]
        blob += data
    return {"instruments": [instrument_struct(instruments[i]) if i in instruments else None for i in range(n)],
            "waves": waves, "samples": _b64(blob)}


def player_pool(game, unsaved=None):
    """GAX2's tables, the music pool (with the new songs' new
    instruments) and the sound-effect set (with the new sound effects'),
    and the voices' song header: what the player needs besides a song.
    unsaved: the songs' unsaved edits (unsaved_songs), whose new
    instruments are in the pool as the build would add them."""
    files = Files(game)
    with bound(game):
        e = unsaved_songs(files, D.read_json(files.manifest), unsaved or {})
        xms = {k: _read_xm_bytes(files.songs / ("%s.xm" % k), v) for k, v in e.xms.items()}
        pool, order, songs = G.load_songs(e.manifest, str(files.songs), str(files.samples), xms, e.sample_data,
                                          e.settings, e.named)
        sfx, slots = G.sfx_pool(manifest=e.sfx_manifest, sample_data=e.sfx_sample_data, named=e.named)
        sfx_manifest = e.sfx_manifest
    info = sfx_manifest["song_info"]
    return {
        "tables": tables(files),
        "music": _pool_struct(pool.instruments, pool.samples, True),
        "sfx": dict(_pool_struct(sfx.instruments, sfx.samples, False), volume=info["volume"],
                    voiceTypes=sfx_manifest["voice_types"]),
        "sfxSlots": slots,
        # the pool instrument each XM instrument of a song built from its XM is (None: no cell plays it, so the
        # build doesn't add it): a song's own instrument is played by it
        "xmPool": {k: song.instrument_indices for k, song in zip(order, songs)
                   if hasattr(song, "instrument_indices")},
    }


def song_struct(song, instruments):
    """A gax.Song as GAX2_init sees it."""
    streams, orders = G.song_sequence(song, instruments)
    info = song.info
    return {
        "channelCount": info.num_channels, "patternRows": info.num_rows_per_pattern,
        "orderCount": info.num_patterns_per_channel, "loopOrder": info.loop_point, "volume": info.volume,
        "mixRate": info.sample_rate, "numSfx": info.num_fx_channels, "halfRateFx": info.half_rate_fx,
        # the mixer's data (struct GaxDspParams): the channels mixed before
        # the echo, then the three taps' {delay ms, gain}
        "dsp": list(struct.unpack("<7I", song.mixer_data_bytes)),
        "patterns": _b64(streams),
        "orders": [[list(o) for o in ch] for ch in orders],
    }


def player_song(game, key, edits=None, unsaved=None):
    """Song `key` (gax_manifest.json's song_table, or an unsaved new
    song's) as tools/gax_audio.py builds it; edits: unsaved cell edits of
    its XM (plan_save's "patterns"); unsaved: the songs' unsaved edits
    (unsaved_songs), played as the build would build them."""
    files = Files(game)
    unsaved = dict(unsaved or {})
    if edits:
        unsaved["patterns"] = dict(unsaved.get("patterns") or {}, **{key: edits})
    with bound(game):
        e = unsaved_songs(files, D.read_json(files.manifest), unsaved)
        keys = [t["song"] for t in e.manifest["song_table"]]
        if key not in keys:
            raise EditError("%s isn't a song of the song table" % key)
        xms = {k: _read_xm_bytes(files.songs / ("%s.xm" % k), v) for k, v in e.xms.items()}
        pool, order, songs = G.load_songs(e.manifest, str(files.songs), str(files.samples), xms, e.sample_data,
                                          e.settings, e.named)
        song = songs[order.index(key)]
        out = song_struct(song, pool.instruments)
    out["key"] = key
    out["id"] = keys.index(key)
    return out


def unsaved_songs(files, manifest, body):
    """The sound as unsaved edits make it (an Edited). body, each part
    optional, as plan_save's: "newSongs" [{key, xm}] (at the song table's
    end), "songInstruments", "xmInstruments", "structures" {key:
    structure()} (a ROM song whose structure changes loses its scaffold:
    descaffolded), "patterns" {key: cell edits}, "songSettings", the music
    pool's edits, the sound-effect set's and "jsonInstruments"."""
    manifest = json.loads(json.dumps(manifest))
    keys = [t["song"] for t in manifest["song_table"]]
    xms = {}
    for i, ns in enumerate(body.get("newSongs") or []):
        where = "newSongs/%d" % i
        key = ns.get("key") if isinstance(ns, dict) else None
        if not isinstance(key, str) or not NAME.match(key) or key in keys or key in manifest["songs"]:
            raise EditError("%s: %r isn't a new song's key (lower case letters, digits and _, a key no song "
                            "has)" % (where, key))
        data = _b64decode(ns.get("xm"), where)
        _read_xm_bytes(files.songs / ("%s.xm" % key), data)
        manifest["song_table"].append({"song": key, "comment": ns.get("comment") or ""})
        keys.append(key)
        xms[key] = data
    manifest, xms = apply_song_edits(files, manifest, keys, xms, body)
    manifest, settings, _ = apply_settings(files, manifest, keys, xms, body)
    manifest, sample_data, _, _ = apply_pool_edits(files, manifest, body, lambda: named_instruments(files, keys, xms))
    check_song_instruments(manifest, body)
    sfx_manifest, sfx_data, _, _ = apply_sfx_edits(files, body)
    named, _, _ = apply_json_instruments(files, body)
    return Edited(manifest, xms, sample_data, settings, sfx_manifest, sfx_data, named)


class Edited:
    """The sound as unsaved edits make it (unsaved_songs): the manifest, {key: XM bytes} of the songs they
    change, {sample: its signed bytes} of the music samples they change, {key: settings} of the new songs whose
    settings they change, the sound-effect manifest, {sample: its signed bytes} of the set's samples they
    change and {name: (instruments/<name>.json's value, its WAV's bytes or None)} of the JSON instruments they
    change or add."""

    def __init__(self, manifest, xms, sample_data, settings, sfx_manifest, sfx_sample_data, named=None):
        self.manifest, self.xms, self.sample_data = manifest, xms, sample_data
        self.settings, self.sfx_manifest, self.sfx_sample_data = settings, sfx_manifest, sfx_sample_data
        self.named = named or {}


def check_song_instruments(manifest, body):
    """The pool instruments body's "songInstruments" add to songs' XMs are instruments of the pool after the
    edits, not empty ones."""
    for key, ns in (body.get("songInstruments") or {}).items():
        for n in ns:
            rec = manifest["instruments"].get(str(n))
            if rec is None or rec["byte_00"]:
                raise EditError("songInstruments/%s: the music pool has no instrument %s%s"
                                % (key, n, " (it is the empty one)" if rec else ""))


def _pool_sample(files, idx):
    """The music pool's sample idx as the build reads it (gax.load_sample_wav), or None."""
    p = files.samples / ("%02d.wav" % idx)
    return G.load_sample_wav(str(p)) if p.is_file() else None


def apply_pool_edits(files, manifest, body, named=None):
    """body's edits of the music pool (each optional):

    - "samples": {"samples/NN.wav": base64 WAV}, a sample's new sound (8-bit mono);
    - "newSamples": {"NN": base64 WAV}: new samples (samples/NN.wav, the
      manifest's `samples`), numbered after the pool's;
    - "deleteSamples": [NN]: samples no instrument plays, taken out;
    - "instruments": {"N": record}: instrument N's manifest record (its
      editable fields, crashdata.audio.pool.EDITABLE); an N after the
      pool's (the next numbers) is a new instrument (pool.new_instrument:
      blank, a copy of another, from a sample);
    - "deleteInstruments": [N]: instruments no song's XM names (named():
      named_instruments' answer) taken out (pool.taken_out: emptied, the
      pool's last ones removed).

    Returns (the manifest, {sample: its signed bytes, None when it is taken
    out}, {rel: bytes to write}, [rel to delete])."""
    manifest = json.loads(json.dumps(manifest))
    data, writes, deletes = {}, {}, []
    rel = files.rel
    for path, b64 in (body.get("samples") or {}).items():
        m = SAMPLE_PATH.match(path or "")
        if m and m.group(1) == "samples" and (files.sound / path).is_file():
            data[int(m.group(2))] = sample_wav(_b64decode(b64, "samples/" + path), path)[0]
    have = sorted(int(k) for k in manifest["samples"])
    for i, (k, b64) in enumerate(sorted((body.get("newSamples") or {}).items(), key=lambda kv: int(kv[0])
                                        if str(kv[0]).isdigit() else -1)):
        where = "newSamples/%s" % k
        want = max(have, default=0) + 1
        if not str(k).isdigit() or int(k) != want:
            raise EditError("%s: a new sample is the pool's next, %d" % (where, want))
        if want > G.MAX_INDEX:
            raise EditError("%s: an instrument names its samples by a byte (at most %d)" % (where, G.MAX_INDEX))
        raw = _b64decode(b64, where)
        pcm, _ = sample_wav(raw, where)
        if len(pcm) > G.MAX_SAMPLE_LENGTH:
            raise EditError("%s: %d samples (at most %d)" % (where, len(pcm), G.MAX_SAMPLE_LENGTH))
        p = files.samples / ("%02d.wav" % want)
        if p.exists():
            raise EditError("%s: %s is there already" % (where, rel(p)))
        manifest["samples"][str(want)] = {"has_data": True}
        data[want] = pcm
        writes[rel(p)] = raw
        have.append(want)
    lengths = {}
    for k in manifest["samples"]:
        d = data[int(k)] if int(k) in data else _pool_sample(files, int(k))
        if d is not None:
            lengths[int(k)] = len(d)
    edits = body.get("instruments") or {}
    if not isinstance(edits, dict):
        raise EditError("instruments: {N: its manifest record}")
    # new instruments: the pool's next numbers
    top = max(int(k) for k in manifest["instruments"])
    new = sorted(int(n) for n in edits if str(n).isdigit() and str(int(n)) not in manifest["instruments"])
    if new and new != list(range(top + 1, top + 1 + len(new))):
        raise EditError("instruments: new instruments are the pool's next numbers, from %d (not %s)"
                        % (top + 1, ", ".join(map(str, new))))
    if new and new[-1] > G.MAX_INDEX:
        raise EditError("instruments/%d: a pattern row names an instrument by a byte (at most %d)"
                        % (new[-1], G.MAX_INDEX))
    for n, rec in sorted(edits.items(), key=lambda kv: int(kv[0]) if str(kv[0]).isdigit() else -1):
        where = "instruments/%s" % n
        if not str(n).isdigit() or str(int(n)) != str(n):
            raise EditError("%s: an instrument is a number" % where)
        try:
            if int(n) in new:
                manifest["instruments"][str(n)] = P.new_instrument(rec, lengths, where)
            else:
                manifest["instruments"][str(n)] = P.edited_instrument(manifest["instruments"][str(n)], rec, lengths,
                                                                      where)
        except P.PoolError as e:
            raise EditError(str(e))
    # instruments taken out: no song's XM may name them
    gone = body.get("deleteInstruments") or []
    if not isinstance(gone, list):
        raise EditError("deleteInstruments: [N, ...]")
    if gone:
        users_of = named() if named else {}
        for i, n in enumerate(gone):
            where = "deleteInstruments/%d" % i
            if isinstance(n, bool) or not isinstance(n, int) or str(n) not in manifest["instruments"] or n == 0:
                raise EditError("%s: %r isn't one of the music pool's instruments (1-%d)" % (where, n, top))
            if str(n) in edits:
                raise EditError("%s: instrument %d is edited by the same save" % (where, n))
            if users_of.get(n):
                raise EditError("%s: %s name Instrument %d" % (where, ", ".join(users_of[n]), n))
        manifest["instruments"] = P.taken_out(manifest["instruments"], gone)
    for idx in body.get("deleteSamples") or []:
        where = "deleteSamples/%s" % idx
        if isinstance(idx, bool) or not isinstance(idx, int) or str(idx) not in manifest["samples"]:
            raise EditError("%s: not one of the music pool's samples" % where)
        who = P.users(manifest["instruments"], idx)
        if who:
            raise EditError("%s: instruments %s play it" % (where, ", ".join(map(str, who))))
        del manifest["samples"][str(idx)]
        data[idx] = None
        p = files.samples / ("%02d.wav" % idx)
        writes.pop(rel(p), None)
        if p.is_file():
            deletes.append(rel(p))
    return manifest, data, writes, deletes


def apply_sfx_edits(files, body, entries=None):
    """body's edits of the sound-effect set (each optional), as the music pool's are (apply_pool_edits), against
    the set's samples (with body's new sounds for sfx_samples/NN.wav):

    - "sfxInstruments": {N: record}: the set's instrument N (gax_sfx_manifest.json's record) edited as a music
      pool instrument is (crashdata.audio.pool.edited_instrument); an N after the set's (the next numbers) is a
      new instrument (pool.new_instrument: blank, a copy of another, from a sample);
    - "newSfxSamples": {"NN": base64 WAV}: new samples (sfx_samples/NN.wav, the manifest's `samples`),
      numbered after the set's;
    - "deleteSfxSamples": [NN]: samples no instrument of the set plays, taken out;
    - "deleteSfxInstruments": [N]: instruments no sound effect plays (entries: the sound-effect table's entries
      after the save, by their slot_id) taken out (pool.taken_out: emptied, the set's last ones removed).

    Returns (the sound-effect manifest, {sample: its signed bytes, None when it is taken out}, {rel: bytes to
    write}, [rel to delete])."""
    sfx = D.read_json(files.sfx_manifest)
    edits = body.get("sfxInstruments") or {}
    if not isinstance(edits, dict):
        raise EditError("sfxInstruments: {N: its gax_sfx_manifest.json record}")
    if not (edits or body.get("newSfxSamples") or body.get("deleteSfxSamples") or body.get("deleteSfxInstruments")):
        return sfx, {}, {}, []
    sfx = json.loads(json.dumps(sfx))
    data, writes, deletes = {}, {}, []
    rel = files.rel
    lengths = {}
    for k in sfx["samples"]:
        path = "sfx_samples/%02d.wav" % int(k)
        if path in (body.get("samples") or {}):
            data[int(k)] = sample_wav(_b64decode(body["samples"][path], "samples/" + path), path)[0]
            lengths[int(k)] = len(data[int(k)])
        elif (files.sound / path).is_file():
            lengths[int(k)] = len(G.load_sample_wav(str(files.sound / path)))
    have = sorted(int(k) for k in sfx["samples"])
    for k, b64 in sorted((body.get("newSfxSamples") or {}).items(), key=lambda kv: int(kv[0])
                         if str(kv[0]).isdigit() else -1):
        where = "newSfxSamples/%s" % k
        want = max(have, default=0) + 1
        if not str(k).isdigit() or int(k) != want:
            raise EditError("%s: a new sample is the set's next, %d" % (where, want))
        if want > G.MAX_INDEX:
            raise EditError("%s: an instrument names its samples by a byte (at most %d)" % (where, G.MAX_INDEX))
        raw = _b64decode(b64, where)
        pcm, _ = sample_wav(raw, where)
        if len(pcm) > G.MAX_SAMPLE_LENGTH:
            raise EditError("%s: %d samples (at most %d)" % (where, len(pcm), G.MAX_SAMPLE_LENGTH))
        p = files.sfx_samples / ("%02d.wav" % want)
        if p.exists():
            raise EditError("%s: %s is there already" % (where, rel(p)))
        sfx["samples"][str(want)] = {"has_data": True}
        data[want] = pcm
        lengths[want] = len(pcm)
        writes[rel(p)] = raw
        have.append(want)
    # new instruments: the set's next numbers
    top = max(int(k) for k in sfx["instruments"])
    new = sorted(int(n) for n in edits if str(n).isdigit() and str(int(n)) not in sfx["instruments"])
    if new and new != list(range(top + 1, top + 1 + len(new))):
        raise EditError("sfxInstruments: new instruments are the set's next numbers, from %d (not %s)"
                        % (top + 1, ", ".join(map(str, new))))
    if new and new[-1] > G.MAX_INDEX:
        raise EditError("sfxInstruments/%d: GAX_fx_ex names an instrument by a byte (at most %d)"
                        % (new[-1], G.MAX_INDEX))
    for n, rec in sorted(edits.items(), key=lambda kv: int(kv[0]) if str(kv[0]).isdigit() else -1):
        where = "sfxInstruments/%s" % n
        if not str(n).isdigit() or str(int(n)) != str(n) or str(n) == "0":
            raise EditError("%s: not one of the sound-effect set's instruments (1-%d)" % (where, top))
        try:
            if int(n) in new:
                sfx["instruments"][str(n)] = P.new_instrument(rec, lengths, where)
            else:
                sfx["instruments"][str(n)] = P.edited_instrument(sfx["instruments"][str(n)], rec, lengths, where)
        except P.PoolError as e:
            raise EditError(str(e).replace("the music pool's", "the sound-effect set's"))
    # instruments taken out: no sound effect may play them
    gone = body.get("deleteSfxInstruments") or []
    if not isinstance(gone, list):
        raise EditError("deleteSfxInstruments: [N, ...]")
    if gone:
        played = {}
        for i, e in enumerate(entries or []):
            if isinstance(e, dict) and isinstance(e.get("slot_id"), int):
                played.setdefault(e["slot_id"], []).append("SFX_%s" % e["name"] if e.get("name") else "entry %d" % i)
        for i, n in enumerate(gone):
            where = "deleteSfxInstruments/%d" % i
            if isinstance(n, bool) or not isinstance(n, int) or str(n) not in sfx["instruments"] or n == 0:
                raise EditError("%s: %r isn't one of the sound-effect set's instruments (1-%d)" % (where, n, top))
            if str(n) in edits:
                raise EditError("%s: instrument %d is edited by the same save" % (where, n))
            if played.get(n):
                raise EditError("%s: slot %d is played by %s" % (where, n, ", ".join(played[n][:3])))
        sfx["instruments"] = P.taken_out(sfx["instruments"], gone)
    for idx in body.get("deleteSfxSamples") or []:
        where = "deleteSfxSamples/%s" % idx
        if isinstance(idx, bool) or not isinstance(idx, int) or str(idx) not in sfx["samples"] or idx == 0:
            raise EditError("%s: not one of the sound-effect set's samples" % where)
        who = P.users(sfx["instruments"], idx)
        if who:
            raise EditError("%s: instruments %s play it" % (where, ", ".join(map(str, who))))
        del sfx["samples"][str(idx)]
        data[idx] = None
        p = files.sfx_samples / ("%02d.wav" % idx)
        writes.pop(rel(p), None)
        if p.is_file():
            deletes.append(rel(p))
    return sfx, data, writes, deletes


def _xm_now(files, xms, key, what):
    if key in xms:
        return xms[key]
    path = files.songs / ("%s.xm" % key)
    if not path.is_file():
        raise EditError("%s: songs/%s.xm isn't a file" % (what, key))
    return path.read_bytes()


def apply_song_edits(files, manifest, keys, xms, body):
    """body's "songInstruments", "xmInstruments" (a song's own XM
    instruments: xm_with_own_instrument), "structures", then its
    "patterns", on the XMs (xms: the new songs' bytes, the others are the
    files'). Returns
    (the manifest, {key: XM bytes} of the songs that change)."""
    added = body.get("songInstruments") or {}
    if not isinstance(added, dict):
        raise EditError("songInstruments: {song: [pool instrument N, ...]}")
    own = body.get("xmInstruments") or {}
    if not isinstance(own, dict):
        raise EditError("xmInstruments: {song: {XM instrument N: its fields}}")
    named = json_instrument_names(body)
    grown = []
    for key in list(added) + [k for k in own if k not in added]:
        part = "songInstruments" if key in added else "xmInstruments"
        if key not in keys:
            raise EditError("%s: %s isn't a song of the song table" % (part, key))
        specs = own.get(key) or {}
        if not isinstance(specs, dict):
            raise EditError("xmInstruments/%s: {XM instrument N: its fields}" % key)
        for n in specs:
            if not str(n).isdigit() or str(int(n)) != str(n):
                raise EditError("xmInstruments/%s/%s: an XM instrument is a number (1-based)" % (key, n))
        buf = _xm_now(files, xms, key, part)
        # the instruments added to its list: the pool's (songInstruments) and its own new ones (the numbers of
        # xmInstruments after its own), in their numbers' order
        count = len(xm_instrument_names(buf))
        fresh = {int(n) for n in specs if int(n) > count}
        new = buf
        if key in added or fresh:
            new = xm_with_instruments(buf, added.get(key) or [], "songInstruments/%s" % key, fresh)
        for n, spec in sorted(specs.items(), key=lambda kv: int(kv[0])):
            new = xm_with_own_instrument(files, new, int(n), spec, "xmInstruments/%s/%s" % (key, n), named)
        if new != buf:
            xms[key] = new
        if fresh:
            grown.append(key)
    structures = body.get("structures") or {}
    if not isinstance(structures, dict):
        raise EditError("structures: {song: {channels, patterns, order, restart, speed}}")
    for key, st in structures.items():
        if key not in keys:
            raise EditError("structures: %s isn't a song of the song table" % key)
        buf = _xm_now(files, xms, key, "structures")
        new = xm_with_structure(buf, st, "songs/%s.xm" % key)
        if new != buf:
            xms[key] = new
            manifest = descaffolded(manifest, key)
    # a ROM song built with its recorded scaffold plays the pool's instruments by their names (gax.rom_song)
    for key in grown:
        if key in manifest["songs"] and G.scaffolded(manifest["songs"][key]):
            raise EditError("xmInstruments/%s: a ROM song built with its recorded scaffold plays only the pool's "
                            "instruments (Instrument N): change its structure first, and it is built from its XM"
                            % key)
    for key, edits in (body.get("patterns") or {}).items():
        if key not in keys:
            raise EditError("patterns: %s isn't a song of the song table" % key)
        if key in structures:
            raise EditError("patterns: %s's cells are in its structure" % key)
        new, manifest, changed = edited_song(files, manifest, key, edits, _xm_now(files, xms, key, "patterns"))
        if changed:
            xms[key] = new
    return manifest, xms


# ---------------------------------------------------------------------------
# A song's settings: a ROM song's in the manifest (`songs`), a new song's
# in songs/<key>.json (crashdata.audio.gax.NEW_SONG_DEFAULTS)

# what the editor edits: GaxSongData's mix rate, volume and half-rate
# sound effects, the mixer's echo taps (struct GaxDspParams); a new song's
# title string too (a ROM song's is its recorded bytes). The rest the game
# doesn't read (num_fx_channels: StartSong asks for 3 voices) or isn't
# known (ushort_0a).
ROM_SETTINGS = ("sample_rate", "volume", "half_rate_fx", "mixer_data_bytes")
NEW_SETTINGS = ("title", "artist") + ROM_SETTINGS
MAX_ECHO_GAIN = 255


def settings_of(files, manifest, key):
    """Song `key`'s settings as the build reads them, and whether they are a ROM song's (the manifest's)."""
    sm = manifest["songs"].get(key)
    if sm is not None:
        return {f: sm[f] for f in G.SCAFFOLD_SETTINGS if f in sm}, True
    s = dict(G.NEW_SONG_DEFAULTS)
    js = files.songs / ("%s.json" % key)
    if js.is_file():
        s.update(D.read_json(js))
    return s, False


def echo_taps(mixer_data_bytes):
    """The mixer's data (struct GaxDspParams, 7 words): the channels mixed before the echo, then the three
    taps' delay (ms) and gain (x/16); the third tap's gain isn't read (the ARM echo adds it at 1/16)."""
    return list(struct.unpack("<7I", bytes(mixer_data_bytes)))


def check_settings(s, channels, where):
    """Why GAX2 or the game can't play a song with settings `s` and `channels` channels (an EditError)."""
    def whole(f, lo, hi):
        v = s.get(f)
        if isinstance(v, bool) or not isinstance(v, int) or not lo <= v <= hi:
            raise EditError("%s/%s: %r (%d-%d)" % (where, f, v, lo, hi))
        return v

    if s.get("sample_rate") not in G.GAX_MIX_RATES:
        raise EditError("%s/sample_rate: %r: GAX2 mixes at %s Hz (gGaxMixRates)"
                        % (where, s.get("sample_rate"), ", ".join(map(str, G.GAX_MIX_RATES))))
    whole("volume", 0, 0xFFFF)
    whole("half_rate_fx", 0, 1)
    m = s.get("mixer_data_bytes")
    if not isinstance(m, list) or len(m) != 28 or not all(isinstance(b, int) and not isinstance(b, bool)
                                                         and 0 <= b <= 0xFF for b in m):
        raise EditError("%s/mixer_data_bytes: 28 bytes (the mixer's echo taps)" % where)
    taps = echo_taps(m)
    if taps[0] > channels:
        raise EditError("%s/mixer_data_bytes: %d channels mixed before the echo, but the song has %d"
                        % (where, taps[0], channels))
    for i in (2, 4):
        if taps[i] > MAX_ECHO_GAIN:
            raise EditError("%s/mixer_data_bytes: echo tap %d's gain %d (0-%d, 16 is the tap as it is)"
                            % (where, i // 2, taps[i], MAX_ECHO_GAIN))
    need = G.work_size(channels, s["sample_rate"], s["half_rate_fx"], m)
    if need > G.GAX_WORK_SIZE:
        raise EditError("%s: GAX2 needs %#x bytes of work RAM for %d channels at %d Hz with these echo taps "
                        "(GAX2_estimate), but StartSong gives it %#x: a lower mix rate or shorter echo delays"
                        % (where, need, channels, s["sample_rate"], G.GAX_WORK_SIZE))
    for f in ("title", "artist"):
        if f in s and s[f] is not None and (not isinstance(s[f], str) or len(s[f].encode("latin-1", "replace"))
                                             > 200):
            raise EditError("%s/%s: a string (Latin-1, at most 200 characters)" % (where, f))


def apply_settings(files, manifest, keys, xms, body):
    """body's "songSettings" {song: settings} (ROM_SETTINGS of a ROM song, NEW_SETTINGS of a new one): checked
    against the song's channels (its XM after the edits). Returns (the manifest, {key: a new song's settings
    as the build will read them}, [(rel, songs/<key>.json's new value)])."""
    edits = body.get("songSettings") or {}
    if not isinstance(edits, dict):
        raise EditError("songSettings: {song: its settings}")
    manifest = json.loads(json.dumps(manifest))
    out, writes = {}, []
    for key, given in edits.items():
        where = "songSettings/%s" % key
        if key not in keys:
            raise EditError("%s: not a song of the song table" % where)
        if not isinstance(given, dict):
            raise EditError("%s: {%s}" % (where, ", ".join(NEW_SETTINGS)))
        now, rom = settings_of(files, manifest, key)
        allowed = ROM_SETTINGS if rom else NEW_SETTINGS
        unknown = sorted(set(given) - set(allowed))
        if unknown:
            raise EditError("%s: %s can't be edited (%s's are %s)" % (where, ", ".join(unknown),
                                                                      "a ROM song" if rom else "a new song",
                                                                      ", ".join(allowed)))
        given = {f: (None if f in ("title", "artist") and v == "" else v) for f, v in given.items()}
        merged = dict(now, **given)
        sm = manifest["songs"].get(key)
        if rom and G.scaffolded(sm):
            channels = sm["num_channels"]
        else:
            channels = XmLayout(_xm_now(files, xms, key, where)).channels
        check_settings(merged, channels, where)
        if rom:
            for f in ROM_SETTINGS:
                if f in given:
                    sm[f] = given[f]
            continue
        out[key] = merged
        js = files.songs / ("%s.json" % key)
        old = D.read_json(js) if js.is_file() else {}
        value = dict(old)
        for f, v in given.items():
            if v == now.get(f):
                continue
            if v is None:
                value.pop(f, None)
            else:
                value[f] = v
        if value != old:
            writes.append((files.rel(js), value))
    return manifest, out, writes


def _read_xm_bytes(path, data):
    try:
        return G.read_xm(str(path), data)
    except (ValueError, struct.error, IndexError) as e:
        raise EditError("%s: not an XM module (%s)" % (path.name, e))


# ---------------------------------------------------------------------------
# XM patterns: read as cells, edited, written back (only the patterns
# whose cells change are packed again; the rest keep their bytes)

CELL = 5  # note, instrument, volume column, effect, parameter
XM_KEY_OFF = 97
XM_EFFECTS = 36  # 0-9, A-Z


class XmLayout:
    """Where an XM's patterns are: [(start, header length, rows, packed
    size)], and the byte after the last one."""

    def __init__(self, buf):
        if buf[:17] != b"Extended Module: ":
            raise EditError("not an XM module")
        header_size = struct.unpack_from("<I", buf, 60)[0]
        self.channels = struct.unpack_from("<H", buf, 68)[0]
        count = struct.unpack_from("<H", buf, 70)[0]
        pos = 60 + header_size
        self.patterns = []
        for _ in range(count):
            hlen, _, rows, packed = struct.unpack_from("<IBHH", buf, pos)
            self.patterns.append((pos, hlen, rows, packed))
            pos += hlen + packed
        self.end = pos


def unpack_cells(buf, layout, index):
    """XM pattern `index` as rows * channels cells of 5 bytes."""
    start, hlen, rows, packed = layout.patterns[index]
    out = bytearray(rows * layout.channels * CELL)
    if packed == 0:
        return out
    pos = start + hlen
    for k in range(rows * layout.channels):
        first = buf[pos]
        if first & 0x80:
            pos += 1
            for bit in range(CELL):
                if first & (1 << bit):
                    out[k * CELL + bit] = buf[pos]
                    pos += 1
        else:
            out[k * CELL:k * CELL + CELL] = buf[pos:pos + CELL]
            pos += CELL
    return out


def pack_cells(cells):
    """Cells packed as the songs' XMs pack them: every cell a flags byte
    (0x80 and a bit per field that isn't 0; an effect's parameter with
    it, 0 too), then those fields."""
    out = bytearray()
    for k in range(0, len(cells), CELL):
        c = cells[k:k + CELL]
        flags = 0x80
        for bit in range(CELL):
            if c[bit] or bit >= 3 and (c[3] or c[4]):
                flags |= 1 << bit
        out.append(flags)
        out += bytes(v for bit, v in enumerate(c) if flags & (1 << bit))
    return bytes(out)


def check_cell(cell, instruments, where):
    """A cell's values: (note 0-97, instrument 0-N, volume column, effect,
    parameter) as bytes, or an EditError."""
    if not isinstance(cell, list) or len(cell) != CELL or not all(isinstance(v, int) and not isinstance(v, bool)
                                                                 for v in cell):
        raise EditError("%s: a cell is [note, instrument, volume, effect, parameter]" % where)
    note, ins, vol, eff, param = cell
    # 0 none, 1-96 C-0 to B-7, 97 key off; the songs' XMs (gaxm's export)
    # have other bytes too, which the build reads as (note - transpose) & 0x7F
    if not 0 <= note <= 0xFF:
        raise EditError("%s: note %d (0 none, 1-96 C-0 to B-7, 97 key off)" % (where, note))
    if not 0 <= ins <= instruments:
        raise EditError("%s: instrument %d: the module has %d" % (where, ins, instruments))
    if not 0 <= vol <= 0xFF or not 0 <= eff < XM_EFFECTS or not 0 <= param <= 0xFF:
        raise EditError("%s: volume, effect and parameter are bytes (an effect 0-%d)" % (where, XM_EFFECTS - 1))
    return bytes(cell)


def xm_with_cells(buf, edits, where="xm"):
    """The XM's bytes with cells changed: edits [(pattern, row, channel,
    cell bytes)]. A pattern whose cells come out the same keeps its bytes.
    Returns (bytes, the patterns that changed)."""
    layout = XmLayout(buf)
    cells = {}
    for pi, row, ch, cell in edits:
        if not (isinstance(pi, int) and 0 <= pi < len(layout.patterns)):
            raise EditError("%s: no pattern %s (the module has %d)" % (where, pi, len(layout.patterns)))
        rows = layout.patterns[pi][2]
        if not (isinstance(row, int) and 0 <= row < rows and isinstance(ch, int) and 0 <= ch < layout.channels):
            raise EditError("%s: pattern %d has %d rows of %d channels (row %s, channel %s)"
                            % (where, pi, rows, layout.channels, row, ch))
        if pi not in cells:
            cells[pi] = unpack_cells(buf, layout, pi)
        k = (row * layout.channels + ch) * CELL
        cells[pi][k:k + CELL] = cell
    changed = sorted(pi for pi, c in cells.items() if c != unpack_cells(buf, layout, pi))
    if not changed:
        return bytes(buf), []
    out = bytearray(buf[:layout.patterns[0][0]])
    for pi, (start, hlen, rows, packed) in enumerate(layout.patterns):
        if pi in changed:
            data = pack_cells(cells[pi])
            if len(data) > 0xFFFF:
                raise EditError("%s: pattern %d packs to %d bytes (an XM pattern holds 65535)" % (where, pi, len(data)))
            head = bytearray(buf[start:start + hlen])
            struct.pack_into("<H", head, 7, len(data))
            out += head + data
        else:
            out += buf[start:start + hlen + packed]
    out += buf[layout.end:]
    return bytes(out), changed


# What an XM's structure can be (FT2's limits; GAX2's speed is a byte, a
# speed effect's under 0x20)
MAX_CHANNELS = 32
MAX_PATTERNS = 256
MAX_ROWS = 256
MAX_ORDER = 256
MAX_SPEED = 31


def structure(buf):
    """An XM's structure as the editor edits it: {channels, patterns:
    [{rows, cells}] (cells base64, 5 bytes a cell, row by row), order,
    restart, speed}."""
    layout = XmLayout(buf)
    song_length, restart = struct.unpack_from("<HH", buf, 64)
    return {"channels": layout.channels,
            "patterns": [{"rows": p[2], "cells": _b64(unpack_cells(buf, layout, i))}
                         for i, p in enumerate(layout.patterns)],
            "order": list(buf[80:80 + song_length]), "restart": restart,
            "speed": struct.unpack_from("<H", buf, 76)[0]}


def xm_with_structure(buf, st, where="xm"):
    """The XM's bytes with a new structure (structure()'s form): its
    channels, patterns, order list, restart position and speed; its
    header's other fields and its instruments as they are. A pattern whose
    cells and rows are the old pattern's of that number keeps its bytes."""
    if not isinstance(st, dict):
        raise EditError("%s: a structure is {channels, patterns, order, restart, speed}" % where)
    layout = XmLayout(buf)
    header_size = struct.unpack_from("<I", buf, 60)[0]
    instruments = struct.unpack_from("<H", buf, 72)[0]

    def whole(v, lo, hi, what):
        if isinstance(v, bool) or not isinstance(v, int) or not lo <= v <= hi:
            raise EditError("%s: %s %r (%d-%d)" % (where, what, v, lo, hi))
        return v

    channels = whole(st.get("channels"), 1, MAX_CHANNELS, "channels")
    pats = st.get("patterns")
    if not isinstance(pats, list) or not 1 <= len(pats) <= MAX_PATTERNS:
        raise EditError("%s: 1-%d patterns" % (where, MAX_PATTERNS))
    order = st.get("order")
    if not isinstance(order, list) or not 1 <= len(order) <= min(MAX_ORDER, header_size - 20):
        raise EditError("%s: the order list has 1-%d positions" % (where, min(MAX_ORDER, header_size - 20)))
    for i, p in enumerate(order):
        whole(p, 0, len(pats) - 1, "order position %d's pattern" % i)
    restart = whole(st.get("restart"), 0, len(order) - 1, "restart position")
    speed = whole(st.get("speed"), 1, MAX_SPEED, "speed")
    head = bytearray(buf[:60 + header_size])
    struct.pack_into("<HHHH", head, 64, len(order), restart, channels, len(pats))
    struct.pack_into("<H", head, 76, speed)
    head[80:60 + header_size] = bytes(order) + bytes(60 + header_size - 80 - len(order))
    out = head
    for i, p in enumerate(pats):
        here = "%s: pattern %d" % (where, i)
        if not isinstance(p, dict):
            raise EditError("%s: {rows, cells}" % here)
        rows = whole(p.get("rows"), 1, MAX_ROWS, "pattern %d's rows" % i)
        cells = _b64decode(p.get("cells"), here)
        if len(cells) != rows * channels * CELL:
            raise EditError("%s: %d rows of %d channels are %d bytes of cells, not %d"
                            % (here, rows, channels, rows * channels * CELL, len(cells)))
        for k in range(0, len(cells), CELL):
            c = cells[k:k + CELL]
            check_cell(list(c), instruments, "%s, row %d, channel %d" % (here, k // CELL // channels,
                                                                         k // CELL % channels))
        if i < len(layout.patterns) and channels == layout.channels and rows == layout.patterns[i][2] \
                and cells == unpack_cells(buf, layout, i):
            start, hlen, _, packed = layout.patterns[i]
            out += buf[start:start + hlen + packed]
            continue
        data = pack_cells(cells)
        if len(data) > 0xFFFF:
            raise EditError("%s packs to %d bytes (an XM pattern holds 65535)" % (here, len(data)))
        if i < len(layout.patterns):
            start, hlen = layout.patterns[i][:2]
            ph = bytearray(buf[start:start + hlen])
            struct.pack_into("<HH", ph, 5, rows, len(data))
        else:
            ph = struct.pack("<IBHH", 9, 0, rows, len(data))
        out += ph + data
    out += buf[layout.end:]
    return bytes(out)


MAX_XM_INSTRUMENTS = 128  # FT2's


def xm_instrument_names(buf):
    """An XM's instruments' names, in order."""
    layout = XmLayout(buf)
    count = struct.unpack_from("<H", buf, 72)[0]
    pos, out = layout.end, []
    for _ in range(count):
        size = struct.unpack_from("<I", buf, pos)[0]
        out.append(buf[pos + 4:pos + 26].split(b"\x00")[0].decode("ascii", errors="replace"))
        samples = struct.unpack_from("<H", buf, pos + 27)[0]
        header = struct.unpack_from("<I", buf, pos + 29)[0] if samples and size >= 33 else 40
        lengths = [struct.unpack_from("<I", buf, pos + size + k * header)[0] for k in range(samples)]
        pos += size + samples * header + sum(lengths)
    return out


def xm_with_instruments(buf, pool_numbers, where="xm", own=()):
    """The XM with an instrument `Instrument N` added at the end of its
    list for each pool instrument N (a cell then plays it by its new
    number). The build reads only the name of an `Instrument N` (its XM
    samples and envelopes aren't GAX2's), so it is a name with no sample:
    a tracker plays it silent.

    own: the numbers of new instruments of the module's own among the
    added ones (xmInstruments' numbers after the module's), each added as
    an empty one (no name, no sample) for xm_with_own_instrument to make;
    the pool's take the other numbers, in their order."""
    if not isinstance(pool_numbers, list):
        raise EditError("%s: [pool instrument N, ...]" % where)
    names = xm_instrument_names(buf)
    out = bytearray(buf)
    first, total = len(names) + 1, len(pool_numbers) + len(own)
    stray = sorted(k for k in own if not first <= k < first + total)
    if stray:
        raise EditError("%s: a new instrument is the module's next number (%d-%d), not %s"
                        % (where, first, first + total - 1, ", ".join(map(str, stray))))
    if first - 1 + total > MAX_XM_INSTRUMENTS:
        raise EditError("%s: an XM has at most %d instruments" % (where, MAX_XM_INSTRUMENTS))
    pool = iter(enumerate(pool_numbers))
    for number in range(first, first + total):
        if number in own:
            out += struct.pack("<I22sBH", 29, b"", 0, 0)
            names.append("")
            continue
        i, n = next(pool)
        if isinstance(n, bool) or not isinstance(n, int) or not 1 <= n <= G.MAX_INDEX:
            raise EditError("%s/%d: %r isn't one of the music pool's instruments (1-%d)" % (where, i, n, G.MAX_INDEX))
        name = "Instrument %d" % n
        if name in names:
            raise EditError("%s/%d: the module has %s already (its instrument %d)" % (where, i, name,
                                                                                    names.index(name) + 1))
        out += struct.pack("<I22sBH", 29, name.encode("ascii"), 0, 0)
        names.append(name)
    struct.pack_into("<H", out, 72, len(names))
    return bytes(out)


def named_instruments(files, keys, xms):
    """{pool instrument N: the songs whose XMs name it (`Instrument N`)}, of the song table's songs (xms: the
    songs whose bytes aren't the files')."""
    out = {}
    for key in keys:
        p = files.songs / ("%s.xm" % key)
        if key not in xms and not p.is_file():
            continue
        for name in xm_instrument_names(xms[key] if key in xms else p.read_bytes()):
            number = name[len("Instrument "):] if name.startswith("Instrument ") else ""
            if number.isdigit():
                out.setdefault(int(number), []).append(key)
    return out


# ---------------------------------------------------------------------------
# A song's own XM instruments: those that aren't `Instrument N` (the pool's)
# or named like instruments/<name>.json, which the build makes into new
# pool instruments from their sample, loop, volume, relative note,
# finetune, volume envelope and auto-vibrato (gax.xm_instrument_spec). The
# editor edits those fields, written in the XM (the instrument's header,
# its sample's header and data); the build reads nothing else of them.

XM_LOOPS = ("none", "forward", "pingpong")
XM_ENVELOPE_POINTS = 12  # FT2's
XM_INSTRUMENT_HEADER = 241  # the fields up to the fadeout (an instrument with samples has 263)
XM_INSTRUMENT_FULL = 263  # an instrument header with samples, as FT2 writes it
XM_SAMPLE_HEADER = 40
XM_SAMPLES = 16  # FT2's: an instrument has at most 16 samples
XM_NAME = 22
XM_KEYS = 96  # the notes a key map maps, C-0 to B-7
SAMPLE_FIELDS = ("volume", "relative_note", "finetune", "loop", "loop_start", "loop_end")
OWN_FIELDS = ("name", "keymap", "envelope", "vibrato", "samples")


def _json_instrument(files, name):
    """Whether an XM instrument's name is instruments/<name>.json's (gax.named_instrument, in files' tree)."""
    return bool(name) and Path(name).name == name and (files.instruments / ("%s.json" % name)).is_file()


def _pool_name(name):
    number = name[len("Instrument "):] if name.startswith("Instrument ") else ""
    return number.isdigit()


def xm_instrument_spans(buf):
    """Where each XM instrument is: (start, its header's size, a sample header's size, [sample header offsets],
    [(sample data offset, length)], end)."""
    layout = XmLayout(buf)
    count = struct.unpack_from("<H", buf, 72)[0]
    pos, out = layout.end, []
    for _ in range(count):
        size = struct.unpack_from("<I", buf, pos)[0]
        samples = struct.unpack_from("<H", buf, pos + 27)[0]
        header = struct.unpack_from("<I", buf, pos + 29)[0] if samples and size >= 33 else 40
        heads = [pos + size + k * header for k in range(samples)]
        data, at = [], pos + size + samples * header
        for h in heads:
            n = struct.unpack_from("<I", buf, h)[0]
            data.append((at, n))
            at += n
        out.append((pos, size, header, heads, data, at))
        pos = at
    return out


def played_notes(xm):
    """{XM instrument: the notes its cells play} over the order list, a note with no instrument the channel's
    last one's (as gax.new_song reads it)."""
    out = {}
    for p in xm["order"]:
        current = [0] * xm["num_channels"]
        for row in xm["patterns"][p] if p < len(xm["patterns"]) else ():
            for c, cell in enumerate(row):
                if cell.instrument:
                    current[c] = cell.instrument
                if 1 <= cell.note <= 96 and current[c]:
                    out.setdefault(current[c], set()).add(cell.note)
    return out


def own_sample(xs):
    """An XM sample's fields the build reads: its volume, tune and loop."""
    unit = 2 if xs["type"] & 0x10 else 1
    kind = xs["type"] & 3
    return {"volume": min(64, xs["volume"]), "relative_note": xs["relnote"], "finetune": xs["finetune"],
            "loop": XM_LOOPS[kind] if xs["loop_len"] and kind < 3 else "none",
            "loop_start": xs["loop_start"] // unit, "loop_end": (xs["loop_start"] + xs["loop_len"]) // unit}


def own_spec(xi):
    """XM instrument xi's fields the build reads, and its name: what the editor edits. Its key map says which of
    its samples each note plays (the build makes the one the song's notes play its sample)."""
    on = xi.get("vol_type", 0)
    env = None
    if on & 1:
        env = {"points": [list(p) for p in xi["vol_points"]],
               "sustain": xi["vol_sustain"] if on & 2 else None,
               "loop_start": xi["vol_loop_start"] if on & 4 else None,
               "loop_end": xi["vol_loop_end"] if on & 4 else None}
    return {"name": xi["name"], "keymap": list(xi.get("keymap") or [0] * XM_KEYS), "envelope": env,
            "vibrato": {"depth": xi.get("vib_depth", 0), "rate": xi.get("vib_rate", 0)},
            "samples": [own_sample(xs) for xs in xi["samples"]]}


def own_instruments(files, xm):
    """{XM instrument number (1-based, as a cell names it): its own instrument} of an XM (read_xm's): those that
    aren't the pool's or a JSON's, with a sample or a name (a tracker's empty slots have neither). Each is {spec
    (own_spec), sounds (each sample's: bits, length, data, base64 of it as the build reads it, signed 8-bit, or
    the problem the build has with it), notes (the notes the song's cells play on it), plays (the samples its key
    map plays on those notes, C-4's if none: more than one, and the build refuses it)}."""
    notes = played_notes(xm)
    out = {}
    for n, xi in enumerate(xm["instruments"], 1):
        if _pool_name(xi["name"]) or _json_instrument(files, xi["name"]) or not (xi["samples"] or xi["name"].strip()):
            continue
        keymap = xi.get("keymap") or [0] * XM_KEYS
        played = sorted(notes.get(n, ()))
        plays = sorted({keymap[k - 1] for k in played}) or ([keymap[G.XM_C4 - 1]] if xi["samples"] else [])
        sounds = []
        for xs in xi["samples"]:
            bits = 16 if xs["type"] & 0x10 else 8
            try:
                data = G.xm_sample_data(xs, _raise)
            except EditError as e:
                sounds.append({"bits": bits, "length": 0, "data": "", "problem": str(e)})
                continue
            sounds.append({"bits": bits, "length": len(data), "data": _b64(data)})
        out[n] = {"spec": own_spec(xi), "sounds": sounds, "notes": played, "plays": plays}
    return out


def _raise(msg):
    raise EditError(msg)


def _deltas(data):
    """Signed 8-bit samples as an XM stores them: deltas."""
    out, prev = bytearray(), 0
    for b in data:
        out.append((b - prev) & 0xFF)
        prev = b
    return bytes(out)


def _whole(v, lo, hi, where):
    if isinstance(v, bool) or not isinstance(v, int) or not lo <= v <= hi:
        raise EditError("%s: %r (%d to %d)" % (where, v, lo, hi))
    return v


# a new sample's fields: FT2's defaults (full volume, C-4 at 8363 Hz, no loop)
NEW_SAMPLE = {"volume": 64, "relative_note": 0, "finetune": 0, "loop": "none", "loop_start": 0, "loop_end": 0}


def _edited_sample(sh, data, now, given, wav, where):
    """An XM sample's header and data (deltas) with its fields as `given` says (own_sample's form) and `wav`
    (base64, 8-bit mono, written as 8-bit) its new sound: only what differs from `now` is written."""
    unit = 2 if sh[14] & 0x10 else 1
    length = len(data) // unit
    if wav is not None:
        pcm, _ = sample_wav(_b64decode(wav, where + "/wav"), where + "/wav")
        if len(pcm) > G.MAX_SAMPLE_LENGTH:
            raise EditError("%s/wav: %d samples (at most %d)" % (where, len(pcm), G.MAX_SAMPLE_LENGTH))
        data, unit, length = _deltas(pcm), 1, len(pcm)
        struct.pack_into("<I", sh, 0, len(pcm))
        sh[14] &= ~0x10 & 0xFF
    # a volume over 64 is read as 64: one not edited keeps its byte
    if given["volume"] != now["volume"]:
        sh[12] = _whole(given["volume"], 0, 64, where + "/volume")
    if given["relative_note"] != now["relative_note"]:
        sh[16] = _whole(given["relative_note"], -128, 127, where + "/relative_note") & 0xFF
    if given["finetune"] != now["finetune"]:
        sh[13] = _whole(given["finetune"], -128, 127, where + "/finetune") & 0xFF
    loop = (given["loop"], given["loop_start"], given["loop_end"])
    if loop != (now["loop"], now["loop_start"], now["loop_end"]) or wav is not None:
        if given["loop"] not in XM_LOOPS:
            raise EditError("%s/loop: %r (%s)" % (where, given["loop"], ", ".join(XM_LOOPS)))
        if given["loop"] == "none":
            sh[14] &= ~3 & 0xFF
            if wav is not None:
                struct.pack_into("<II", sh, 4, 0, 0)
        else:
            ls = _whole(given["loop_start"], 0, max(0, length - 1), where + "/loop_start")
            le = _whole(given["loop_end"], ls + 1, length, where + "/loop_end")
            sh[14] = (sh[14] & ~3 & 0xFF) | XM_LOOPS.index(given["loop"])
            struct.pack_into("<II", sh, 4, ls * unit, (le - ls) * unit)
    return sh, data


def xm_with_own_instrument(files, buf, n, spec, where="xm", named=()):
    """The XM with its own instrument n as `spec` says (own_spec's form, each field optional: what is left out
    stays): its name, its key map (which of its samples each note plays), its volume envelope and auto-vibrato,
    and its samples' fields, each with "wav" (base64, 8-bit mono, written as 8-bit) for a new sound; a sample after
    its own is a new one, with its sound. Only what differs from the XM's is written; the rest keeps its bytes.
    What GAX2 can't play of what the edit turns on is turned off with it: a volume envelope switched on or
    changed zeroes the fadeout (GAX2 has none), a vibrato set makes its waveform a sine with no sweep.

    Its name can't become a pool instrument's (`Instrument N`), nor a JSON's (instruments/<name>.json, or one of
    `named`, a save's new ones) unless it has no sample: an instrument of that name is the JSON's, a name the build
    reads and nothing else."""
    xm = _read_xm_bytes(Path(where), buf)
    if isinstance(n, bool) or not isinstance(n, int) or not 1 <= n <= len(xm["instruments"]):
        raise EditError("%s: no instrument %r (the module has %d)" % (where, n, len(xm["instruments"])))
    xi = xm["instruments"][n - 1]
    start, size, header, heads, datas, end = xm_instrument_spans(buf)[n - 1]
    if _pool_name(xi["name"]) or _json_instrument(files, xi["name"]):
        raise EditError("%s: %r is the pool's or instruments/%s.json's, not one of the module's own"
                        % (where, xi["name"], xi["name"]))
    if not isinstance(spec, dict):
        raise EditError("%s: {name, keymap, envelope, vibrato, samples}" % where)
    unknown = sorted(set(spec) - set(OWN_FIELDS))
    if unknown:
        raise EditError("%s: %s isn't one of an XM instrument's fields the build reads (%s)"
                        % (where, ", ".join(unknown), ", ".join(OWN_FIELDS)))
    now = own_spec(xi)
    given = dict(now, **spec)
    count = len(now["samples"])
    listed = given["samples"]
    if not isinstance(listed, list) or len(listed) > XM_SAMPLES:
        raise EditError("%s/samples: a list of its samples' fields (an XM instrument has at most %d)"
                        % (where, XM_SAMPLES))
    total = max(count, len(listed))
    samples = []
    for i in range(total):
        w = "%s/samples/%d" % (where, i)
        cur = now["samples"][i] if i < count else NEW_SAMPLE
        g = listed[i] if i < len(listed) else {}
        if not isinstance(g, dict):
            raise EditError("%s: {volume, relative_note, finetune, loop, loop_start, loop_end, wav}" % w)
        unknown = sorted(set(g) - set(SAMPLE_FIELDS) - {"wav"})
        if unknown:
            raise EditError("%s: %s isn't one of an XM sample's fields the build reads" % (w, ", ".join(unknown)))
        if i >= count and g.get("wav") is None:
            raise EditError("%s: a new sample has its sound (wav)" % w)
        samples.append((cur, dict(cur, **{f: v for f, v in g.items() if f != "wav"}), g.get("wav")))
    rest = {f: given[f] for f in OWN_FIELDS if f != "samples"}
    if total == count and rest == {f: now[f] for f in rest} and all(c == s and w is None for c, s, w in samples):
        return bytes(buf)
    if total and header < XM_SAMPLE_HEADER and count:
        raise EditError("%s: its samples' headers are %d bytes (an XM's are %d)" % (where, header, XM_SAMPLE_HEADER))
    head = bytearray(buf[start:start + size])
    if total and size < XM_INSTRUMENT_HEADER:
        # an instrument with no sample (a name): an instrument header with samples has the key map, the envelopes
        # and the vibrato
        head = bytearray(XM_INSTRUMENT_FULL)
        head[:min(size, 29)] = buf[start:start + min(size, 29)]
        struct.pack_into("<I", head, 0, XM_INSTRUMENT_FULL)
    if total and not count:
        # its first sample: no key map, envelope or vibrato yet (a tracker ignores those of an instrument with none)
        head[33:] = bytes(len(head) - 33)
        header = XM_SAMPLE_HEADER
        struct.pack_into("<I", head, 29, header)
    if len(head) >= 29:
        struct.pack_into("<H", head, 27, total)
    # its name
    name = given["name"]
    if name != now["name"]:
        if not isinstance(name, str) or len(name) > XM_NAME or any(not 0x20 <= ord(c) < 0x7F for c in name):
            raise EditError("%s/name: %r: at most %d ASCII characters" % (where, name, XM_NAME))
        if _pool_name(name):
            raise EditError("%s/name: %r names the pool's instrument (a cell plays one from the pool's list)"
                            % (where, name))
        if (_json_instrument(files, name) or name in named) and total:
            raise EditError("%s/name: %r is instruments/%s.json's: an instrument of that name plays it, not its own "
                            "samples" % (where, name, name))
        head[4:26] = name.encode("ascii").ljust(XM_NAME, b"\0")
    rest_now = {f: now[f] for f in ("keymap", "envelope", "vibrato")}
    if not total:
        if {f: given[f] for f in rest_now} != rest_now:
            raise EditError("%s: it has no sample: its key map, envelope and vibrato come with its first sound" % where)
    # its key map: the sample each of the 96 notes plays
    keymap = given["keymap"]
    if total and (keymap != now["keymap"] or not count):
        if not isinstance(keymap, list) or len(keymap) != XM_KEYS:
            raise EditError("%s/keymap: the sample each of the %d notes plays" % (where, XM_KEYS))
        for k, s in enumerate(keymap):
            _whole(s, 0, total - 1, "%s/keymap/%d (%s)" % (where, k, G.note_name(k + 1)))
        head[33:33 + XM_KEYS] = bytes(keymap)
    env = given["envelope"]
    if env != now["envelope"]:
        if env is None:
            head[233] &= ~1 & 0xFF
        else:
            if not isinstance(env, dict) or not isinstance(env.get("points"), list):
                raise EditError("%s/envelope: {points: [[tick, volume]], sustain, loop_start, loop_end}" % where)
            pts = env["points"]
            if not 1 <= len(pts) <= XM_ENVELOPE_POINTS:
                raise EditError("%s/envelope: %d points (an XM's has 1 to %d)" % (where, len(pts),
                                                                                 XM_ENVELOPE_POINTS))
            for i, p in enumerate(pts):
                if not isinstance(p, list) or len(p) != 2:
                    raise EditError("%s/envelope/points/%d: [tick, volume]" % (where, i))
                x = _whole(p[0], 0 if i == 0 else pts[i - 1][0] + 1, 0 if i == 0 else 0xFFFF,
                           "%s/envelope/points/%d's tick" % (where, i))
                y = _whole(p[1], 0, 64, "%s/envelope/points/%d's volume" % (where, i))
                struct.pack_into("<HH", head, 129 + 4 * i, x, y)
            marks = {}
            for f in ("sustain", "loop_start", "loop_end"):
                v = env.get(f)
                marks[f] = None if v is None else _whole(v, 0, len(pts) - 1, "%s/envelope/%s" % (where, f))
            if (marks["loop_start"] is None) != (marks["loop_end"] is None) or \
                    marks["loop_start"] is not None and marks["loop_start"] > marks["loop_end"]:
                raise EditError("%s/envelope: a loop has a start and an end point, the start first" % where)
            head[225] = len(pts)
            for f, at in (("sustain", 227), ("loop_start", 228), ("loop_end", 229)):
                if marks[f] is not None:
                    head[at] = marks[f]
            head[233] = (head[233] & ~7 & 0xFF) | 1 | (2 if marks["sustain"] is not None else 0) \
                | (4 if marks["loop_start"] is not None else 0)
            struct.pack_into("<H", head, 239, 0)
    vib = given["vibrato"]
    if vib != now["vibrato"]:
        if not isinstance(vib, dict) or set(vib) != {"depth", "rate"}:
            raise EditError("%s/vibrato: {depth, rate}" % where)
        head[237] = _whole(vib["depth"], 0, 255, where + "/vibrato/depth")
        head[238] = _whole(vib["rate"], 0, 255, where + "/vibrato/rate")
        if vib["depth"] and vib["rate"]:
            head[235] = head[236] = 0
    # its samples: the headers, then the data
    sheads, sdata = [], []
    for i, (cur, g, wav) in enumerate(samples):
        if i < count:
            sh, d = bytearray(buf[heads[i]:heads[i] + header]), buf[datas[i][0]:datas[i][0] + datas[i][1]]
        else:
            sh, d = bytearray(header), b""
            sh[12], sh[15] = NEW_SAMPLE["volume"], 0x80  # full volume, panned to the middle
        if i >= count or g != cur or wav is not None:
            sh, d = _edited_sample(sh, d, cur, g, wav, "%s/samples/%d" % (where, i))
        sheads.append(bytes(sh))
        sdata.append(bytes(d))
    return bytes(buf[:start]) + bytes(head) + b"".join(sheads) + b"".join(sdata) + bytes(buf[end:])


# ---------------------------------------------------------------------------
# instruments/<name>.json: an instrument from a WAV, which an XM instrument of
# that name plays and a sound effect names (gax.json_instrument_spec,
# docs/audio.md "From a WAV"). The editor edits the file's fields and gives
# its WAV a new sound, or adds a new one with its WAV.

def json_instrument_names(body):
    """The names of the JSON instruments body's "jsonInstruments" edits or adds."""
    edits = body.get("jsonInstruments") if isinstance(body, dict) else None
    return set(edits) if isinstance(edits, dict) else set()


def _sound_path(files, rel, where):
    """games/<game>/sound/rel, a WAV under it."""
    if not isinstance(rel, str) or not rel.endswith(".wav"):
        raise EditError("%s: %r: the WAV, a path under %s (e.g. samples/<name>.wav)"
                        % (where, rel, files.rel(files.sound)))
    p = (files.sound / rel).resolve()
    try:
        p.relative_to(files.sound.resolve())
    except ValueError:
        raise EditError("%s: %r isn't under %s" % (where, rel, files.rel(files.sound)))
    return p


def apply_json_instruments(files, body, entries=None):
    """body's "jsonInstruments": {name: {"json": instruments/<name>.json's value, "wav": base64 (optional), a new
    sound for the WAV its "sample" names}}: a JSON instrument's fields edited, or a new one (a name the tree
    hasn't, lower case letters, digits and _) with its WAV. Each is checked as the build reads it, as a song's
    instrument and, when a sound effect names it (entries: the sound-effect table's after the save), at a fixed
    pitch. A WAV a pool's sample is (samples/NN.wav, sfx_samples/NN.wav) or another JSON plays isn't given a new
    sound here. Returns ({name: (its value, its WAV's bytes or None)}, [(rel, value)] of the files that change,
    {rel: bytes})."""
    edits = body.get("jsonInstruments") or {}
    if not isinstance(edits, dict):
        raise EditError("jsonInstruments: {name: {json, wav}}")
    named, out_json, writes = {}, [], {}
    if not edits:
        return named, out_json, writes
    plays = {}  # the WAVs the tree's JSON instruments play, as these edits leave them
    if files.instruments.is_dir():
        for p in sorted(files.instruments.glob("*.json")):
            if p.stem not in edits:
                try:
                    plays.setdefault(_sound_path(files, D.read_json(p).get("sample"), p.name), []).append(p.stem)
                except (EditError, ValueError, AttributeError):
                    pass
    sfx_named = {e.get("instrument") for e in entries or [] if isinstance(e, dict)}
    for name, e in sorted(edits.items()):
        where = "jsonInstruments/%s" % name
        js = files.instruments / ("%s.json" % name)
        exists = Path(name).name == name and js.is_file()
        if not exists and not NAME.match(name):
            raise EditError("%s: a new instrument's name is lower case letters, digits and _" % where)
        if not isinstance(e, dict) or set(e) - {"json", "wav"} or not isinstance(e.get("json"), dict):
            raise EditError("%s: {json: instruments/%s.json's value, wav}" % (where, name))
        value = e["json"]
        p = _sound_path(files, value.get("sample"), where + "/json/sample")
        wav = None
        if e.get("wav") is not None:
            wav = _b64decode(e["wav"], where + "/wav")
            rel = files.sound_rel(p)
            if SAMPLE_PATH.match(rel):
                raise EditError("%s/wav: %s is a pool's sample: the Samples tab gives it a new sound" % (where, rel))
            if plays.get(p):
                raise EditError("%s/wav: instruments/%s.json plays %s too" % (where, plays[p][0], rel))
            writes[files.rel(p)] = wav
        elif not p.is_file():
            raise EditError("%s: %s isn't a file: give it a sound (wav)" % (where, files.sound_rel(p)))
        plays.setdefault(p, []).append(name)

        def fail(msg, where=where):
            raise EditError("%s: %s" % (where, msg))

        with bound(files.game):
            G.json_instrument_spec(name, False, fail, (value, wav))
            if name in sfx_named:
                G.json_instrument_spec(name, True, fail, (value, wav))
        named[name] = (value, wav)
        if not exists or D.read_json(js) != value:
            out_json.append((files.rel(js), value))
    return named, out_json, writes


def json_instruments(files, keys, xms=None):
    """instruments/<name>.json's instruments as the Instruments tab lists them: {name, file, data (its value),
    sound (its WAV as the build reads it: length, rate, data, base64 signed 8-bit; or the problem with it), songs
    (those whose XMs name it)}."""
    if not files.instruments.is_dir():
        return []
    names = {}
    for key in keys:
        p = files.songs / ("%s.xm" % key)
        if xms and key in xms:
            buf = xms[key]
        elif p.is_file():
            buf = p.read_bytes()
        else:
            continue
        for name in xm_instrument_names(buf):
            names.setdefault(name, []).append(key)
    out = []
    for p in sorted(files.instruments.glob("*.json")):
        data = D.read_json(p)
        sound = None
        try:
            wav = _sound_path(files, data.get("sample") if isinstance(data, dict) else None, p.name)
            if not wav.is_file():
                raise EditError("%s isn't a file" % files.sound_rel(wav))
            with bound(files.game):
                pcm, rate = G.load_wav(str(wav), _raise)
            sound = {"path": files.sound_rel(wav), "length": len(pcm), "rate": rate, "data": _b64(pcm)}
        except EditError as e:
            sound = {"problem": str(e)}
        out.append({"name": p.stem, "file": files.rel(p), "data": data, "sound": sound,
                    "songs": names.get(p.stem, [])})
    return out


def descaffolded(manifest, key):
    """The manifest with ROM song `key`'s scaffold taken out (gax.SCAFFOLD:
    it is then built from its XM, with its settings), or as it is."""
    sm = manifest["songs"].get(key)
    if sm is None or not G.scaffolded(sm):
        return manifest
    manifest = json.loads(json.dumps(manifest))
    for f in G.SCAFFOLD:
        manifest["songs"][key].pop(f, None)
    return manifest


def shared_cells(sm, pi, ch):
    """A ROM song's pattern sharing: (order position, channel, transpose
    difference) of every pattern stored once with channel `ch`'s at order
    position `pi`, itself included (its group: the ROM shares the bytes,
    so an edit of one is an edit of all of them)."""
    groups = sm["pattern_groups"]
    transposes = sm["channel_transposes"]
    gid = groups[ch][pi]
    t0 = transposes[ch][pi]
    return [(pj, cj, transposes[cj][pj] - t0) for cj, row in enumerate(groups) for pj, g in enumerate(row) if g == gid]


def expand_edits(manifest, key, edits, instruments, current=None):
    """A song's cell edits ({"pattern", "row", "channel", "cell"}) as
    (pattern, row, channel, cell bytes), each one that changes its cell
    (current(pattern, row, channel): the cell now) copied to the patterns
    its ROM pattern shares its bytes with (the note moved by their
    transposes)."""
    sm = manifest["songs"].get(key)
    if sm is not None and not G.scaffolded(sm):
        sm = None  # built from its XM: no patterns shared
    out = []
    if not isinstance(edits, list):
        raise EditError("%s: the edits are [{pattern, row, channel, cell}]" % key)
    for i, e in enumerate(edits):
        where = "patterns/%s/%d" % (key, i)
        if not isinstance(e, dict):
            raise EditError("%s: {pattern, row, channel, cell}" % where)
        pi, row, ch = e.get("pattern"), e.get("row"), e.get("channel")
        cell = check_cell(e.get("cell"), instruments, where)
        if sm is None:
            out.append((pi, row, ch, cell))
            continue
        if not (isinstance(pi, int) and 0 <= pi < sm["num_patterns_per_channel"]
                and isinstance(ch, int) and 0 <= ch < sm["num_channels"]):
            raise EditError("%s: no pattern %s, channel %s in %s" % (where, pi, ch, key))
        if current is not None and current(pi, row, ch) == cell:
            continue
        for pj, cj, dt in shared_cells(sm, pi, ch):
            c = bytearray(cell)
            if 1 <= c[0] <= 96:
                c[0] = max(1, min(96, c[0] + dt))
            out.append((pj, row, cj, bytes(c)))
    return out


def edited_song(files, manifest, key, edits, buf=None):
    """A song's XM bytes after cell edits; the build packs an edited
    pattern from its cells as it packs every other (the XM says all a
    pattern's bytes hold, crashdata.audio.gax._xm_effect_to_gax). Returns
    (XM bytes, the manifest, the XM patterns that changed)."""
    if buf is None:
        buf = (files.songs / ("%s.xm" % key)).read_bytes()
    instruments = struct.unpack_from("<H", buf, 72)[0]
    layout, cache = XmLayout(buf), {}

    def current(pi, row, ch):
        if not (isinstance(row, int) and 0 <= pi < len(layout.patterns) and 0 <= row < layout.patterns[pi][2]
                and 0 <= ch < layout.channels):
            return None
        if pi not in cache:
            cache[pi] = unpack_cells(buf, layout, pi)
        k = (row * layout.channels + ch) * CELL
        return bytes(cache[pi][k:k + CELL])

    new, changed = xm_with_cells(buf, expand_edits(manifest, key, edits, instruments, current), "songs/%s.xm" % key)
    return new, manifest, changed


def patterns(game, key, xm=None):
    """A song's XM for the pattern view: its patterns' cells (base64, 5
    bytes a cell, row by row), rows, order list, instruments and, for a
    ROM song, its pattern sharing and transposes. Order position p plays
    XM pattern playOrder[p] (a ROM song's p, a new song's its order
    list's). xm: an unsaved new song's XM (base64), whose key no song has."""
    files = Files(game)
    manifest = D.read_json(files.manifest)
    path = files.songs / ("%s.xm" % key)
    if xm is not None:
        if not isinstance(key, str) or not NAME.match(key) or key in [t["song"] for t in manifest["song_table"]]:
            raise EditError("%r isn't a new song's key" % (key,))
        buf = _b64decode(xm, "xm")
        manifest["songs"].pop(key, None)
    elif key not in [t["song"] for t in manifest["song_table"]]:
        raise EditError("%s isn't a song of the song table" % key)
    else:
        buf = path.read_bytes()
    layout = XmLayout(buf)
    xm = _read_xm_bytes(path, buf)
    sm = manifest["songs"].get(key)
    out = {
        "key": key, "rom": sm is not None, "scaffold": sm is not None and G.scaffolded(sm), "name": xm["name"],
        "channels": layout.channels,
        "rows": [p[2] for p in layout.patterns],
        "cells": [_b64(unpack_cells(buf, layout, i)) for i in range(len(layout.patterns))],
        "order": xm["order"], "restart": xm["restart"], "speed": xm["tempo"], "bpm": xm["bpm"],
        "instruments": [{"name": xi["name"], "pool": idx} for xi, idx in zip(xm["instruments"],
                                                                              xm["gax_instrument_indices"])],
    }
    # its own instruments, as the build reads them (own_instruments): what the Instruments tab edits
    for n, own in own_instruments(files, xm).items():
        out["instruments"][n - 1]["own"] = own
    if out["scaffold"]:
        out["groups"] = sm["pattern_groups"]
        out["transposes"] = sm["channel_transposes"]
        out["playOrder"] = list(range(sm["num_patterns_per_channel"]))
        out["loopOrder"] = sm["loop_point"]
    else:
        out["playOrder"] = xm["order"]
        out["loopOrder"] = xm["restart"]
    return out


# ---------------------------------------------------------------------------
# WAVs: read (any PCM, or 32-bit float), made into a GAX2 sample

def read_wav(data):
    """(channels, bits, rate, float, frames: a list of -1..1 floats per channel)."""
    if data[:4] != b"RIFF" or data[8:12] != b"WAVE":
        raise EditError("not a WAV file (RIFF/WAVE)")
    pos, fmt, pcm = 12, None, None
    while pos + 8 <= len(data):
        cid, size = data[pos:pos + 4], struct.unpack_from("<I", data, pos + 4)[0]
        body = data[pos + 8:pos + 8 + size]
        if cid == b"fmt ":
            fmt = body
        elif cid == b"data":
            pcm = body
        pos += 8 + size + (size & 1)
    if fmt is None or pcm is None or len(fmt) < 16:
        raise EditError("the WAV has no fmt or data chunk")
    tag, channels, rate, _, _, bits = struct.unpack_from("<HHIIHH", fmt)
    if tag == 0xFFFE and len(fmt) >= 26:  # WAVE_FORMAT_EXTENSIBLE: its sub-format
        tag = struct.unpack_from("<H", fmt, 24)[0]
    if tag not in (1, 3) or channels < 1 or rate < 1:
        raise EditError("the WAV is format %d: a sample is integer PCM or 32-bit float" % tag)
    width = bits // 8
    if (tag == 1 and width not in (1, 2, 3, 4)) or (tag == 3 and width != 4):
        raise EditError("%d-bit samples: a WAV's are 8, 16, 24 or 32-bit PCM, or 32-bit float" % bits)
    n = len(pcm) // (width * channels)
    out = [[0.0] * n for _ in range(channels)]
    for i in range(n):
        for c in range(channels):
            o = (i * channels + c) * width
            if tag == 3:
                v = struct.unpack_from("<f", pcm, o)[0]
            elif width == 1:
                v = (pcm[o] - 128) / 128
            else:
                v = int.from_bytes(pcm[o:o + width], "little", signed=True) / (1 << (8 * width - 1))
            out[c][i] = v
    return channels, bits, rate, tag == 3, out


def wav_bytes(samples, rate):
    """An 8-bit mono WAV of signed samples, as the tree's are."""
    buf = io.BytesIO()
    with wave.open(buf, "wb") as w:
        w.setnchannels(1)
        w.setsampwidth(1)
        w.setframerate(rate)
        w.writeframes(bytes((s + 128) & 0xFF for s in samples))
    return buf.getvalue()


def import_wav(data, rate=None):
    """A WAV made into a GAX2 sample: mixed down to mono, resampled to
    `rate` (linearly; the WAV's own rate by default) and rounded to 8
    bits. Returns {wav (base64), length, rate, source: {channels, bits,
    rate, float}, peak, clipped, how}."""
    channels, bits, src_rate, is_float, frames = read_wav(data)
    n = len(frames[0])
    if n == 0:
        raise EditError("the WAV has no samples")
    mono = [sum(ch[i] for ch in frames) / channels for i in range(n)]
    rate = src_rate if rate is None else rate
    if isinstance(rate, bool) or not isinstance(rate, int) or not 1000 <= rate <= 200000:
        raise EditError("rate %r: 1000-200000 Hz" % (rate,))
    if rate != src_rate:
        m = max(1, int(round(n * rate / src_rate)))
        res = []
        for j in range(m):
            x = j * src_rate / rate
            i = int(math.floor(x))
            f = x - i
            a = mono[min(i, n - 1)]
            b = mono[min(i + 1, n - 1)]
            res.append(a + (b - a) * f)
        mono = res
    if len(mono) > G.MAX_SAMPLE_LENGTH:
        raise EditError("%d samples: GAX2 plays %d at most (Q11 positions in 32 bits)"
                        % (len(mono), G.MAX_SAMPLE_LENGTH))
    out, clipped = [], 0
    for v in mono:
        s = int(math.floor(v * 128 + 0.5))
        if s > 127 or s < -128:
            clipped += 1
            s = max(-128, min(127, s))
        out.append(s)
    how = "%d-bit%s %s at %d Hz" % (bits, " float" if is_float else "",
                                    "mono" if channels == 1 else "%d channels mixed to mono" % channels, src_rate)
    if rate != src_rate:
        how += ", resampled to %d Hz" % rate
    return {"wav": _b64(wav_bytes(out, rate)), "length": len(out), "rate": rate,
            "source": {"channels": channels, "bits": bits, "rate": src_rate, "float": is_float},
            "peak": max(abs(s) for s in out), "clipped": clipped, "how": how}


def sample_wav(data, where):
    """A sample's WAV as the build reads it (gax.load_sample_wav: 8-bit
    mono): (its signed samples, its rate), or an EditError."""
    try:
        with wave.open(io.BytesIO(data), "rb") as w:
            if w.getnchannels() != 1 or w.getsampwidth() != 1:
                raise EditError("%s: a sample is an 8-bit mono WAV (import it to convert it)" % where)
            rate = w.getframerate()
            frames = w.readframes(w.getnframes())
    except (wave.Error, EOFError) as e:
        raise EditError("%s: not a PCM WAV (%s)" % (where, e or "it ends early"))
    if not frames:
        raise EditError("%s: the sample is empty" % where)
    return bytes((b - 128) & 0xFF for b in frames), rate


def sample_needs(instruments, idx):
    """The shortest sample the instruments that play sample `idx` can
    take: what their wave rows' start, loop and sweep reach (GaxChannelMix
    reads the sample up to there), and which instruments those are."""
    need, who = 1, []
    for n, ins in sorted(instruments.items()):
        for r, w in enumerate(ins["sample_indices"]):
            if w != idx or ins["byte_00"]:
                continue
            s = ins["samples"][r]
            who.append(n)
            need = max(need, s["start_position"] + 1)
            if s["loop_start"] < s["loop_end"]:
                need = max(need, s["loop_end"])
            if s["byte_02"]:
                need = max(need, s["start_position"] + s["int_10"])
    return need, sorted(set(who))


# ---------------------------------------------------------------------------
# The view

def rate_hz(period, index):
    """The rate a gGaxPeriodTable index plays a sample at, Hz."""
    return period[max(0, min(len(period) - 1, index))] / 2048


def sfx_references(files, names):
    """Where each SFX_<NAME> is named: code (engine/), the cutscene slides."""
    out = {n: [] for n in names}
    pattern = re.compile(r"\bSFX_([A-Z0-9_]+)\b")
    for d in (files.root / "engine",):
        for p in sorted(d.rglob("*")) if d.is_dir() else ():
            if p.suffix not in (".c", ".cpp", ".h", ".hpp", ".inc") or not p.is_file():
                continue
            for m in set(pattern.findall(p.read_text(encoding="latin-1"))):
                if m in out:
                    out[m].append(files.rel(p))
    if files.cutscenes.is_file():
        data = D.read_json(files.cutscenes)
        for s in data.get("slides", []):
            sfx = s.get("sfx", "")
            if sfx.startswith("SFX_") and sfx[4:] in out:
                out[sfx[4:]].append("cutscenes.json: slide %s" % s.get("name"))
    return out


def song_uses(files, keys):
    """What plays each song: [{kind, name, comment}] for the level themes
    (levels.json), the screens (music.json) and the cutscene slides."""
    out = {k: [] for k in keys}
    if files.levels.is_file():
        for t in D.read_json(files.levels).get("themes", []):
            if t.get("song") in out:
                out[t["song"]].append({"kind": "theme", "name": t.get("constant"), "comment": t.get("name", "")})
    for s in D.read_json(files.music)["screen_music"]:
        if s["song"] in out:
            out[s["song"]].append({"kind": "screen", "name": s["screen"], "comment": s.get("comment", "")})
    if files.cutscenes.is_file():
        data = D.read_json(files.cutscenes)
        by_record = {}
        for ci, c in enumerate(data.get("cutscenes", [])):
            for s in c.get("slides", []):
                by_record.setdefault(s.get("slide"), set()).add(c.get("constant", str(ci)))
        for s in data.get("slides", []):
            key = (s.get("song") or "")[len("SONG_"):].lower()
            if key in out:
                out[key].append({"kind": "cutscene", "name": s.get("name"),
                                 "comment": ", ".join(sorted(by_record.get(s.get("name"), ())))})
    return out


def view(game):
    """The music editor's view of the game's sound."""
    files = Files(game)
    with bound(game):
        manifest = D.read_json(files.manifest)
        sfx_manifest = D.read_json(files.sfx_manifest)
        table = D.read_json(files.sfx_table)
        music = D.read_json(files.music)
        period = tables(files)["period"]
        keys = [t["song"] for t in manifest["song_table"]]
        uses = song_uses(files, keys)
        songs = []
        instrument_songs = {}
        for i, t in enumerate(manifest["song_table"]):
            key = t["song"]
            path = files.songs / ("%s.xm" % key)
            sm = manifest["songs"].get(key)
            entry = {"key": key, "id": i, "constant": "SONG_" + key.upper(), "comment": t.get("comment", ""),
                     "rom": sm is not None, "file": files.rel(path), "uses": uses[key]}
            try:
                xm = G.read_xm(str(path))
            except (OSError, ValueError, struct.error, IndexError) as e:
                entry["error"] = "%s: %s" % (files.rel(path), e)
                songs.append(entry)
                continue
            for idx in xm["gax_instrument_indices"]:
                if idx is not None:
                    instrument_songs.setdefault(idx, set()).add(key)
            entry["xm"] = {"name": xm["name"], "channels": xm["num_channels"], "patterns": len(xm["patterns"]),
                           "order": len(xm["order"]), "speed": xm["tempo"], "bpm": xm["bpm"],
                           "instruments": [xi["name"] for xi in xm["instruments"]],
                           # its own instruments' numbers (own_instruments: the Instruments tab edits them)
                           "own": sorted(own_instruments(files, xm))}
            settings = sm
            if settings is None:
                settings = dict(G.NEW_SONG_DEFAULTS)
                js = files.songs / ("%s.json" % key)
                if js.is_file():
                    settings.update(D.read_json(js))
            entry["mixRate"] = settings["sample_rate"]
            entry["volume"] = settings["volume"]
            entry["halfRateFx"] = settings["half_rate_fx"]
            entry["title"] = settings.get("title") or xm["name"] or key
            entry["artist"] = settings.get("artist")
            # what the Music tab's settings edit (ROM_SETTINGS, NEW_SETTINGS), as the build reads them
            entry["settings"], _ = settings_of(files, manifest, key)
            if sm is None:
                entry["settingsFile"] = files.rel(files.songs / ("%s.json" % key))
            entry["workSize"] = G.work_size(xm["num_channels"], settings["sample_rate"], settings["half_rate_fx"],
                                            settings["mixer_data_bytes"])
            songs.append(entry)
        # the music pool's samples
        samples = []
        for k in sorted(manifest["samples"], key=int):
            idx = int(k)
            p = files.samples / ("%02d.wav" % idx)
            if not p.is_file():
                continue
            data, rate = sample_wav(p.read_bytes(), files.rel(p))
            need, who = sample_needs({int(n): v for n, v in manifest["instruments"].items()}, idx)
            samples.append({"index": idx, "path": files.sound_rel(p), "length": len(data), "rate": rate,
                            "minLength": need, "instruments": who,
                            "songs": sorted({s for n in who for s in instrument_songs.get(n, ())})})
        # the music pool's instruments: their records, and the songs whose XMs name them (`Instrument N`)
        pool_instruments = [{"index": int(n), "record": rec, "songs": sorted(instrument_songs.get(int(n), ()))}
                            for n, rec in sorted(manifest["instruments"].items(), key=lambda kv: int(kv[0]))]
        # the sound-effect set's instruments: slot N plays sample N at one rate
        slots = []
        sfx_ins = {int(n): v for n, v in sfx_manifest["instruments"].items()}
        for n in sorted(sfx_ins):
            ins = sfx_ins[n]
            if ins["byte_00"] or not ins["sample_indices"][0]:
                continue
            idx = ins["sample_indices"][0]
            p = files.sfx_samples / ("%02d.wav" % idx)
            row = ins["rows"][0]
            note = ((row["relative_note_number"] & 0xFF) - 2) * 32
            data, rate = sample_wav(p.read_bytes(), files.rel(p)) if p.is_file() else (b"", None)
            need, _ = sample_needs(sfx_ins, idx)
            slots.append({"slot": n, "sample": idx, "path": files.sound_rel(p), "length": len(data), "wavRate": rate,
                          "rate": round(rate_hz(period, note + ins["samples"][0]["pitch"]), 1),
                          "fixedPitch": bool(row["dont_use_note_pitch"]), "minLength": need})
        sfx_records = [{"index": n, "record": sfx_ins[n]} for n in sorted(sfx_ins)]
        # the set's samples (sfx_samples/NN.wav) and the instruments that play each
        sfx_samples = []
        for k in sorted(sfx_manifest["samples"], key=int):
            p = files.sfx_samples / ("%02d.wav" % int(k))
            if not p.is_file():
                continue
            data, rate = sample_wav(p.read_bytes(), files.rel(p))
            need, who = sample_needs(sfx_ins, int(k))
            sfx_samples.append({"index": int(k), "path": files.sound_rel(p), "length": len(data), "rate": rate,
                                "minLength": need, "instruments": who})
        names = [e["name"] for e in table["entries"] if "name" in e]
        refs = sfx_references(files, names)
        sfx = []
        for i, e in enumerate(table["entries"]):
            sfx.append(dict(e, id=i, constant="SFX_%s" % e["name"] if "name" in e else None,
                            uses=refs.get(e.get("name"), [])))
        # instruments/<name>.json's, with their sound and the songs that play them
        new_instruments = json_instruments(files, keys)
        themes = []
        if files.levels.is_file():
            themes = [{"constant": t.get("constant"), "name": t.get("name"), "song": t.get("song")}
                      for t in D.read_json(files.levels).get("themes", [])]
    return {
        "songs": songs,
        "screens": music["screen_music"],
        "themes": themes,
        "sfx": sfx,
        "sfxNoneComment": table.get("none_comment", ""),
        "sfxSlots": slots,
        "samples": samples,
        "instruments": new_instruments,
        "poolInstruments": pool_instruments,
        "sfxInstruments": sfx_records,
        "sfxSamples": sfx_samples,
        # a new pool instrument (blank: from a sample, none yet), a new sound-effect instrument (at the set's one
        # rate, sfxRate Hz) and what one taken out becomes
        "instrumentTemplates": {"blank": P.blank_instrument(), "sfxBlank": P.blank_sfx_instrument(),
                                "empty": P.empty_instrument()},
        "sfxRate": round(rate_hz(period, P.SFX_TUNE), 1),
        "songDefaults": dict(G.NEW_SONG_DEFAULTS),
        "mixRates": list(G.GAX_MIX_RATES),
        "limits": {"maxIndex": G.MAX_INDEX, "maxSampleLength": G.MAX_SAMPLE_LENGTH, "romSfxSlots": max(sfx_ins),
                   "workSize": G.GAX_WORK_SIZE, "maxEchoGain": MAX_ECHO_GAIN, "xmInstruments": MAX_XM_INSTRUMENTS},
        "files": {"manifest": files.rel(files.manifest), "sfxManifest": files.rel(files.sfx_manifest),
                  "music": files.rel(files.music),
                  "sfxTable": files.rel(files.sfx_table), "levels": files.rel(files.levels)},
    }


# ---------------------------------------------------------------------------
# Saving

SAMPLE_PATH = re.compile(r"^(samples|sfx_samples)/(\d{2,})\.wav$")
NAME = re.compile(r"^[a-z0-9_]+$")
SFX_NAME = re.compile(r"^[A-Z0-9_]+$")


def plan_save(game, body):
    """A save's checks, then what it writes: {"json": [(rel, value)],
    "bytes": [(rel, bytes)]} (rel from the tree's root). body, each part
    optional:

    - "sfxTable": sfx_table.json's new value;
    - "music": music.json's new value (the screens' songs);
    - "themes": {THEME_<NAME>: song key}, the level themes' songs (levels.json);
    - "songComments": {song key: comment} (the song table's);
    - "newSongs": [{"key", "xm": base64, "comment"}], added at the song
      table's end (songs/<key>.xm);
    - "removeSongs": [song key], new songs taken out of the song table
      (their XM stays, unbuilt);
    - "structures": {song key: structure()}: a song's XM with new
      patterns, rows, channels, order list, restart position or speed
      (a ROM song then loses its scaffold: descaffolded);
    - "patterns": {song key: [{"pattern", "row", "channel", "cell"}]},
      cell edits of the songs' XMs;
    - "samples": {"samples/NN.wav" or "sfx_samples/NN.wav": base64 WAV},
      new sounds for the tree's samples (8-bit mono: import_wav's);
    - "newInstruments": [{"name", "wav": base64, "rate", "volume",
      "comment"}]: instruments/<name>.json with its sample
      sfx_samples/<name>.wav, for new sound effects (an sfxTable entry's
      "instrument");
    - "instruments", "newSamples", "deleteSamples", "deleteInstruments":
      the music pool's instruments and samples (apply_pool_edits: new
      instruments too);
    - "songInstruments": {song key: [pool instrument N]}: `Instrument N`
      added to the song's XM (xm_with_instruments), for its cells to play;
    - "songSettings": {song key: settings} (apply_settings): a ROM song's
      mix rate, volume, half-rate sound effects and echo taps (the
      manifest's), a new song's and its title (songs/<key>.json);
    - "xmInstruments": {song key: {N: fields}}: the song's own XM
      instrument N's fields the build reads and its name (its key map, its
      samples' sounds, loops, volumes and tunes, its volume envelope and
      auto-vibrato: xm_with_own_instrument); an N after the XM's
      instruments is a new one (with songInstruments', in their numbers'
      order);
    - "jsonInstruments": {name: {json, wav}}: instruments/<name>.json's
      fields, its WAV's new sound, or a new one (apply_json_instruments);
    - "sfxInstruments", "newSfxSamples", "deleteSfxSamples",
      "deleteSfxInstruments": the sound-effect set's instruments and samples
      (gax_sfx_manifest.json, apply_sfx_edits: new instruments too).

    Returns {"json": [(rel, value)], "bytes": [(rel, bytes)], "delete":
    [rel]}."""
    files = Files(game)
    if not available(game):
        raise EditError("%s has no games/%s/sound/: its sound isn't data yet" % (game.title, game.name))
    if not isinstance(body, dict):
        raise EditError("the edits are an object")
    out_json, out_bytes, diags = [], [], []
    manifest = D.read_json(files.manifest)
    old_manifest = json.dumps(manifest)
    m_rel = files.rel(files.manifest)
    keys = [t["song"] for t in manifest["song_table"]]

    # the song table: comments, new songs, songs taken out
    new_xms = {}
    for key, comment in (body.get("songComments") or {}).items():
        if key not in keys:
            raise EditError("songComments: %s isn't a song of the song table" % key)
        if not isinstance(comment, str):
            raise EditError("songComments/%s: a comment is a string" % key)
        manifest["song_table"][keys.index(key)]["comment"] = comment
    for i, ns in enumerate(body.get("newSongs") or []):
        where = "newSongs/%d" % i
        if not isinstance(ns, dict):
            raise EditError("%s: {key, xm, comment}" % where)
        key = ns.get("key")
        if not isinstance(key, str) or not NAME.match(key):
            diags.append(_diag("%r: a song's key is lower case letters, digits and _ (SONG_<KEY>)" % (key,),
                               m_rel, "/song_table"))
            continue
        if key in keys or key in manifest["songs"] or (files.songs / ("%s.xm" % key)).exists():
            diags.append(_diag("there is a song %s already (or songs/%s.xm)" % (key, key), m_rel, "/song_table"))
            continue
        data = _b64decode(ns.get("xm"), where)
        _read_xm_bytes(files.songs / ("%s.xm" % key), data)
        new_xms[key] = data
        entry = {"song": key, "comment": ns.get("comment") or "a new song (the music editor's)"}
        manifest["song_table"].append(entry)
        keys.append(key)
    for key in body.get("removeSongs") or []:
        if key not in keys:
            raise EditError("removeSongs: %s isn't a song of the song table" % key)
        if key in manifest["songs"]:
            diags.append(_diag("%s is one of the ROM's songs: it stays in the song table" % key, m_rel,
                               "/song_table/%d" % keys.index(key)))
            continue
        i = keys.index(key)
        del manifest["song_table"][i]
        del keys[i]

    # the songs' structures and cells (a new song's too)
    manifest, xms = apply_song_edits(files, manifest, keys, dict(new_xms), body)
    for key, data in xms.items():
        out_bytes.append((files.rel(files.songs / ("%s.xm" % key)), data))
    # the songs' settings: a ROM song's in the manifest, a new song's in songs/<key>.json
    manifest, _, settings_writes = apply_settings(files, manifest, keys, xms, body)
    out_json += settings_writes

    # what names a song: the themes, the screens, the cutscenes
    levels = D.read_json(files.levels) if files.levels.is_file() else None
    themes = body.get("themes") or {}
    if themes:
        if levels is None:
            raise EditError("there is no levels.json for the themes")
        levels = json.loads(json.dumps(levels))
        by_constant = {t.get("constant"): t for t in levels.get("themes", [])}
        for const, song in themes.items():
            if const not in by_constant:
                raise EditError("themes: %s isn't a theme of levels.json" % const)
            if song not in keys:
                diags.append(_diag("%r isn't a song of the song table" % (song,), files.rel(files.levels),
                                   "/themes/%d/song" % levels["themes"].index(by_constant[const])))
            by_constant[const]["song"] = song
        out_json.append((files.rel(files.levels), levels))
    music = body.get("music")
    if music is not None:
        old = D.read_json(files.music)
        rel = files.rel(files.music)
        if not isinstance(music, dict) or not isinstance(music.get("screen_music"), list):
            raise EditError("music: music.json's value, {screen_music: [...]}")
        screens = [s.get("screen") for s in music["screen_music"] if isinstance(s, dict)]
        if sorted(screens) != sorted(s["screen"] for s in old["screen_music"]):
            diags.append(_diag("the screens are the code's (SCREEN_SONG_<SCREEN>): only their songs change", rel,
                               "/screen_music"))
        for i, s in enumerate(music["screen_music"]):
            if isinstance(s, dict) and s.get("song") not in keys:
                diags.append(_diag("%r isn't a song of the song table" % (s.get("song"),), rel,
                                   "/screen_music/%d/song" % i))
        out_json.append((rel, music))
    final_music = music if music is not None else D.read_json(files.music)
    final_levels = levels if levels is not None else (D.read_json(files.levels) if files.levels.is_file() else {})
    named = {t.get("song") for t in final_levels.get("themes", [])}
    named |= {s.get("song") for s in final_music.get("screen_music", []) if isinstance(s, dict)}
    if files.cutscenes.is_file():
        named |= {(s.get("song") or "")[len("SONG_"):].lower() for s in D.read_json(files.cutscenes).get("slides", [])}
    for key in body.get("removeSongs") or []:
        if key in named:
            diags.append(_diag("%s is still played (a theme, a screen or a cutscene names it)" % key, m_rel,
                               "/song_table"))

    # samples
    new_names = set()
    for i, ni in enumerate(body.get("newInstruments") or []):
        where = "newInstruments/%d" % i
        if not isinstance(ni, dict):
            raise EditError("%s: {name, wav, rate, volume, comment}" % where)
        name = ni.get("name")
        if not isinstance(name, str) or not NAME.match(name):
            raise EditError("%s: %r: an instrument's name is lower case letters, digits and _" % (where, name))
        js = files.instruments / ("%s.json" % name)
        wav = files.sfx_samples / ("%s.wav" % name)
        if js.exists() or wav.exists() or name in new_names:
            raise EditError("%s: instruments/%s.json or sfx_samples/%s.wav is there already" % (where, name, name))
        data = _b64decode(ni.get("wav"), where)
        _, wav_rate = sample_wav(data, where)
        spec = {"comment": ni.get("comment") or "a sound effect's sample (the music editor's)",
                "sample": "sfx_samples/%s.wav" % name}
        rate = ni.get("rate", wav_rate)
        if isinstance(rate, bool) or not isinstance(rate, (int, float)) or not 523 <= rate <= 523382:
            raise EditError("%s: rate %r: a sound effect plays at 523-523382 Hz (gGaxPeriodTable)" % (where, rate))
        if rate != wav_rate:
            spec["rate"] = rate
        volume = ni.get("volume", 64)
        if isinstance(volume, bool) or not isinstance(volume, int) or not 0 <= volume <= 64:
            raise EditError("%s: volume %r: 0-64" % (where, volume))
        if volume != 64:
            spec["volume"] = volume
        new_names.add(name)
        out_bytes.append((files.rel(wav), data))
        out_json.append((files.rel(js), spec))
    # the music pool: its instruments, new samples, samples taken out
    manifest, _, pool_writes, pool_deletes = apply_pool_edits(files, manifest, body,
                                                              lambda: named_instruments(files, keys, xms))
    check_song_instruments(manifest, body)
    out_bytes += list(pool_writes.items())
    # the sound-effect set's instruments and samples (one taken out no entry of the table after the save plays)
    old_sfx = D.read_json(files.sfx_manifest)
    table = body.get("sfxTable")
    entries = table.get("entries") if isinstance(table, dict) else None
    if not isinstance(entries, list):
        entries = D.read_json(files.sfx_table)["entries"]
    sfx_manifest, _, sfx_writes, sfx_deletes = apply_sfx_edits(files, body, entries)
    if sfx_manifest != old_sfx:
        out_json.append((files.rel(files.sfx_manifest), sfx_manifest))
    out_bytes += list(sfx_writes.items())
    # the JSON instruments (a new one's name can't be a new sound effect's too)
    named, json_writes, wav_writes = apply_json_instruments(files, body, entries)
    for name in set(named) & new_names:
        raise EditError("jsonInstruments/%s: a new sound effect's instrument is %s too" % (name, name))
    out_json += json_writes
    out_bytes += list(wav_writes.items())
    manifest_ins = {int(n): v for n, v in manifest["instruments"].items()}
    sfx_manifest_ins = {int(n): v for n, v in sfx_manifest["instruments"].items()}
    for path, b64 in (body.get("samples") or {}).items():
        m = SAMPLE_PATH.match(path or "")
        p = files.sound / path if m else None
        if p is None or not p.is_file():
            raise EditError("samples: %r isn't one of the tree's samples (samples/NN.wav, sfx_samples/NN.wav)" % path)
        data = _b64decode(b64, "samples/" + path)
        pcm, _ = sample_wav(data, path)
        pool = manifest_ins if m.group(1) == "samples" else sfx_manifest_ins
        need, who = sample_needs(pool, int(m.group(2)))
        if len(pcm) < need:
            diags.append(_diag("%s: %d samples, but instruments %s play it up to %d (their start, loop or sweep)"
                               % (path, len(pcm), ", ".join(map(str, who)), need), files.rel(p), None, "music-sample"))
        out_bytes.append((files.rel(p), data))

    # the sound-effect table: its slots are the set's instruments after the save's edits
    sfx_ins = set(sfx_manifest_ins)
    if table is not None:
        rel = files.rel(files.sfx_table)
        old = D.read_json(files.sfx_table)
        entries = table.get("entries") if isinstance(table, dict) else None
        if not isinstance(entries, list):
            raise EditError("sfxTable: sfx_table.json's value, {entries: [...]}")
        old_names = [e["name"] for e in old["entries"] if "name" in e]
        refs = sfx_references(files, old_names)
        names = {}
        for i, e in enumerate(entries):
            ptr = "/entries/%d" % i
            if not isinstance(e, dict):
                continue
            n = e.get("name")
            if n is not None:
                if not isinstance(n, str) or not SFX_NAME.match(n):
                    diags.append(_diag("%r: a name is upper case letters, digits and _ (SFX_<NAME>)" % (n,), rel,
                                       ptr + "/name"))
                elif n in names:
                    diags.append(_diag("two sound effects are SFX_%s (%d and %d)" % (n, names[n], i), rel,
                                       ptr + "/name"))
                names[n] = i
            if "slot_id" in e and "instrument" in e or "slot_id" not in e and "instrument" not in e:
                diags.append(_diag("an entry plays a slot_id (an instrument of the set) or a new instrument", rel, ptr))
            elif "slot_id" in e and not (isinstance(e["slot_id"], int) and 0 < e["slot_id"] <= max(sfx_ins)):
                diags.append(_diag("slot_id %r: the set's instruments are 1-%d" % (e["slot_id"], max(sfx_ins)), rel,
                                   ptr + "/slot_id"))
            elif "instrument" in e and not ((files.instruments / ("%s.json" % e["instrument"])).is_file()
                                            or e["instrument"] in new_names or e["instrument"] in named):
                diags.append(_diag("there is no instruments/%s.json" % e["instrument"], rel, ptr + "/instrument"))
            v = e.get("volume")
            if isinstance(v, int) and not isinstance(v, bool) and v < 0:
                diags.append(_diag("volume %d: 8.8 fixed point, 0 or more (256 is 1.0)" % v, rel, ptr + "/volume"))
        for n in old_names:
            if refs.get(n) and n not in names:
                diags.append(_diag("SFX_%s is named by %s: keep an entry of that name" % (n, ", ".join(refs[n][:3])),
                                   rel, "/entries"))
        out_json.append((rel, table))

    if json.dumps(manifest) != old_manifest:
        out_json.append((m_rel, manifest))
    if diags:
        raise EditError("the music editor's checks reject it", diags)
    return {"json": out_json, "bytes": out_bytes, "delete": pool_deletes + sfx_deletes}


def _b64decode(v, where):
    if not isinstance(v, str):
        raise EditError("%s: base64 expected" % where)
    try:
        return base64.b64decode(v, validate=True)
    except ValueError:
        raise EditError("%s: not base64" % where)
