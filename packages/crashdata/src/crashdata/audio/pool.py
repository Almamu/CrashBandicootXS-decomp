"""The music pool's instruments and samples as the music editor edits them (#1167, docs/editors.md "The
music editor"): an instrument's record of gax_manifest.json changed (its volume envelope, vibrato, wave
rows: sample, tune, start, loop or sweep; its sequence), a new sample (samples/NN.wav, the next number)
and an unused one taken out. crashdata.audio.edit plays them unsaved and saves them.

An instrument's record is the manifest's (crashdata.audio.gax.Instrument); the bytes the engine never
reads (`unknown12`, the second envelope; `byte_05`-`byte_07`, `byte_0b`, `ushort_12`) and `byte_00`
(instrument 0, the empty one) aren't edited.
"""
import json

# what an edit may change of an instrument's record
EDITABLE = ("sample_indices", "vibrato_delay", "vibrato_depth", "vibrato_speed", "row_speed", "samples", "rows",
            "envelope")
WAVE_ROWS = 4
MAX_SEQ = 255  # GaxChannelInstrument.seqLen is a byte
MAX_POINTS = 255  # GaxEnvelope.count
NONE = 0xFF  # an envelope's sustain or loop point: none


class PoolError(Exception):
    pass


def _int(v, lo, hi, where):
    if isinstance(v, bool) or not isinstance(v, int) or not lo <= v <= hi:
        raise PoolError("%s: %r (%d to %d)" % (where, v, lo, hi))
    return v


def _bool(v, where):
    if not isinstance(v, bool):
        raise PoolError("%s: %r (true or false)" % (where, v))
    return v


def slope(prev, point):
    """GaxEnvelopePoint.slope of a point from the one before it: Q8 per tick, rounded toward 0, as the ROM's
    (crashdata.audio.gax.gax_envelope)."""
    dx = point["x"] - prev["x"]
    q = abs(point["y"] - prev["y"]) * 256 // dx
    return max(-0x7FFF, min(0x7FFF, q if point["y"] >= prev["y"] else -q))


def edited_envelope(old, env, where):
    """A volume envelope after an edit: {points: [{x, y}], sustain, loop_start, loop_end} (None: no point).
    A point that keeps its place and its neighbour's keeps its recorded slope (two of the ROM's, between points
    a tick apart, are past what the formula gives, and the engine doesn't use them); the others get theirs."""
    if env is None:
        return None
    if not isinstance(env, dict) or not isinstance(env.get("points"), list):
        raise PoolError("%s: an envelope is {points: [{x, y}], sustain, loop_start, loop_end}" % where)
    pts = env["points"]
    if not 1 <= len(pts) <= MAX_POINTS:
        raise PoolError("%s: %d points (1 to %d)" % (where, len(pts), MAX_POINTS))
    out = []
    for i, p in enumerate(pts):
        if not isinstance(p, dict):
            raise PoolError("%s/points/%d: {x, y}" % (where, i))
        x = _int(p.get("x"), 0, 0xFFFF, "%s/points/%d/x" % (where, i))
        y = _int(p.get("y"), 0, 0xFF, "%s/points/%d/y" % (where, i))
        if i == 0 and x != 0:
            raise PoolError("%s: the first point is at tick 0" % where)
        if i and x <= out[-1]["x"]:
            raise PoolError("%s/points/%d: its tick %d isn't after the point before's (%d)" % (where, i, x,
                                                                                               out[-1]["x"]))
        q = {"x": x, "interpolation": 0, "y": y, "byte_05": 0, "ushort_06": 0}
        if i:
            was = old["points"] if old else []
            kept = i < len(was) and (was[i]["x"], was[i]["y"], was[i - 1]["x"], was[i - 1]["y"]) == \
                (x, y, out[-1]["x"], out[-1]["y"])
            q["interpolation"] = was[i]["interpolation"] if kept else slope(out[-1], q)
        out.append(q)
    res = {"points": out}
    for k in ("sustain", "loop_start", "loop_end"):
        v = env.get(k)
        res[k] = None if v is None or v == NONE else _int(v, 0, len(out) - 1, "%s/%s" % (where, k))
    if (res["loop_start"] is None) != (res["loop_end"] is None) or \
            res["loop_start"] is not None and res["loop_start"] > res["loop_end"]:
        raise PoolError("%s: a loop has a start and an end point, the start first" % where)
    # none is 0xff in the data, null or 255 in the manifest: written as the record had it
    for k in ("sustain", "loop_start", "loop_end"):
        if res[k] is None and old and old[k] == NONE:
            res[k] = NONE
    return {"sustain": res["sustain"], "loop_start": res["loop_start"], "loop_end": res["loop_end"],
            "points": out}


def wave_row_needs(row):
    """The shortest sample a wave row can play (GaxChannelMix reads it up to its start, its loop's end or its
    sweep's reach)."""
    need = row["start_position"] + 1
    if row["loop_start"] < row["loop_end"]:
        need = max(need, row["loop_end"])
    if row["byte_02"]:
        need = max(need, row["start_position"] + row["int_10"], row["loop_end"])
    return need


def edited_instrument(old, new, lengths, where):
    """An instrument's record after an edit: new's editable fields (EDITABLE), checked, over old's.
    lengths: {sample: its length} of the pool's samples, after the edits."""
    if not isinstance(new, dict):
        raise PoolError("%s: an instrument is its manifest record" % where)
    for k, v in new.items():
        if k not in old:
            raise PoolError("%s: %s isn't one of an instrument's fields" % (where, k))
        if k not in EDITABLE and v != old[k]:
            raise PoolError("%s/%s: not edited (the engine doesn't read it, or it is instrument 0's)" % (where, k))
    out = json.loads(json.dumps(old))
    idx = new.get("sample_indices", old["sample_indices"])
    if not isinstance(idx, list) or len(idx) != WAVE_ROWS:
        raise PoolError("%s/sample_indices: %d samples, one a wave row" % (where, WAVE_ROWS))
    out["sample_indices"] = [_int(v, 0, 0xFF, "%s/sample_indices/%d" % (where, i)) for i, v in enumerate(idx)]
    for k, top in (("vibrato_delay", 0xFF), ("vibrato_depth", 0xFF), ("vibrato_speed", 0xFF), ("row_speed", 0xFF)):
        out[k] = _int(new.get(k, old[k]), 0, top, "%s/%s" % (where, k))
    rows = new.get("samples", old["samples"])
    if not isinstance(rows, list) or len(rows) != WAVE_ROWS:
        raise PoolError("%s/samples: %d wave rows" % (where, WAVE_ROWS))
    out["samples"] = []
    for r, w in enumerate(rows):
        here = "%s/samples/%d" % (where, r)
        if not isinstance(w, dict) or set(w) != set(old["samples"][r]):
            raise PoolError("%s: a wave row has %s" % (here, ", ".join(old["samples"][r])))
        q = {"byte_02": _int(w["byte_02"], 0, 0xFF, here + "/byte_02"),
             "is_bidirectional": _bool(w["is_bidirectional"], here + "/is_bidirectional"),
             "start_position": _int(w["start_position"], 0, 0x7FFFFFFF, here + "/start_position"),
             "loop_start": _int(w["loop_start"], 0, 0x7FFFFFFF, here + "/loop_start"),
             "loop_end": _int(w["loop_end"], 0, 0x7FFFFFFF, here + "/loop_end"),
             "int_10": _int(w["int_10"], 0, 0x7FFFFFFF, here + "/int_10"),
             "ushort_14": _int(w["ushort_14"], 0, 0xFFFF, here + "/ushort_14"),
             "ushort_16": _int(w["ushort_16"], 0, 0xFFFF, here + "/ushort_16"),
             "pitch": _int(w["pitch"], -0x8000, 0x7FFF, here + "/pitch")}
        out["samples"].append(q)
        # a wave row the edit changes (its sample or its fields) must stay in its sample; the ROM's are as
        # they are
        s = out["sample_indices"][r]
        if s and not out["byte_00"] and (q != old["samples"][r] or s != old["sample_indices"][r]):
            if s not in lengths:
                raise PoolError("%s: sample %d isn't one of the music pool's" % (here, s))
            if wave_row_needs(q) > lengths[s]:
                raise PoolError("%s: its start, loop or sweep reach sample %d, but sample %d has %d"
                                % (here, wave_row_needs(q), s, lengths[s]))
    seq = new.get("rows", old["rows"])
    if not isinstance(seq, list) or not 1 <= len(seq) <= MAX_SEQ:
        raise PoolError("%s/rows: 1 to %d sequence steps" % (where, MAX_SEQ))
    out["rows"] = []
    for i, e in enumerate(seq):
        here = "%s/rows/%d" % (where, i)
        if not isinstance(e, dict) or not isinstance(e.get("effects"), list) or len(e["effects"]) != 2:
            raise PoolError("%s: a step is {relative_note_number, dont_use_note_pitch, sample_index, byte_03, "
                            "effects: [2]}" % here)
        out["rows"].append({
            "relative_note_number": _int(e.get("relative_note_number"), -0x80, 0xFF, here + "/relative_note_number"),
            "dont_use_note_pitch": _bool(e.get("dont_use_note_pitch"), here + "/dont_use_note_pitch"),
            "sample_index": _int(e.get("sample_index"), 0, WAVE_ROWS, here + "/sample_index"),
            "byte_03": _int(e.get("byte_03", 0), 0, 0xFF, here + "/byte_03"),
            "effects": [{"type": _int((f or {}).get("type"), 0, 0xFF, "%s/effects/%d/type" % (here, k)),
                         "param": _int((f or {}).get("param"), 0, 0xFF, "%s/effects/%d/param" % (here, k))}
                        for k, f in enumerate(e["effects"])]})
    if "envelope" in new:
        out["envelope"] = edited_envelope(old["envelope"], new["envelope"], where + "/envelope")
    return out


def users(instruments, idx):
    """The instruments whose wave rows name sample idx."""
    return sorted(int(n) for n, ins in instruments.items() if idx in ins["sample_indices"])


# -- new instruments, and instruments taken out --------------------------------------------------------------------

def _wave_row(**kw):
    return dict({"byte_02": 0, "is_bidirectional": False, "start_position": 0, "loop_start": 0, "loop_end": 0,
                 "int_10": 0, "ushort_14": 0, "ushort_16": 0, "pitch": 0}, **kw)


def _record(empty, sample, tune):
    return {
        "byte_00": empty, "sample_indices": [sample, 0, 0, 0], "byte_05": 0, "byte_06": 0, "byte_07": 0,
        "vibrato_delay": 0, "vibrato_depth": 0, "vibrato_speed": 0, "byte_0b": 0,
        "unknown12": [1, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0], "row_speed": 1, "ushort_12": 0,
        "samples": [_wave_row(pitch=tune), _wave_row(), _wave_row(), _wave_row()],
        "rows": [{"relative_note_number": 2 if not empty else 0, "dont_use_note_pitch": False,
                  "sample_index": 1 if not empty else 0, "byte_03": 0,
                  "effects": [{"type": 0, "param": 0}, {"type": 0, "param": 0}]}],
        "envelope": {"sustain": None if not empty else NONE, "loop_start": None if not empty else NONE,
                     "loop_end": None if not empty else NONE,
                     "points": [{"x": 0, "interpolation": 0, "y": 255, "byte_05": 0, "ushort_06": 0}]},
    }


def blank_instrument(sample=0):
    """A new instrument as the build makes one from a sample (crashdata.audio.gax.instrument_entry): one wave row
    playing `sample` (0: none yet) at 8363 Hz on XM's C-4, no loop, a one-step sequence starting it at the note, a
    flat envelope cut at the key off, no vibrato."""
    from crashdata.audio import gax as G

    return _record(0, sample, G.tune_for_rate(G.XM_C4_RATE, False))


# the tune every ROM sound effect plays its sample at, at a fixed pitch: gGaxPeriodTable[1511], about 8002.6 Hz
SFX_TUNE = 1511


def blank_sfx_instrument(sample=0):
    """A new sound-effect instrument as the ROM's are: one wave row playing `sample` (0: none yet) at the set's one
    rate (SFX_TUNE, a fixed pitch), no loop, a one-step sequence starting it, a flat envelope, no vibrato."""
    r = _record(0, sample, SFX_TUNE)
    r["rows"][0]["dont_use_note_pitch"] = True
    return r


def empty_instrument():
    """The empty instrument, as the pool's instrument 0 is (`byte_00`): a note on it plays nothing. An instrument
    taken out of the pool that isn't its last becomes one, so the numbers after it stay."""
    return _record(1, 0, 0)


def new_instrument(rec, lengths, where):
    """A new instrument's record (blank_instrument's fields, the editable ones edited: a copy of another
    instrument is one), checked as an edit is."""
    base = blank_instrument()
    env = rec.get("envelope") if isinstance(rec, dict) else None
    if isinstance(env, dict):
        # none written as the record has it (0xff or null)
        for k in ("sustain", "loop_start", "loop_end"):
            if env.get(k) == NONE:
                base["envelope"][k] = NONE
    return edited_instrument(base, rec, lengths, where)


def taken_out(instruments, gone):
    """The pool's instruments (manifest records by number) with those of `gone` taken out: each becomes the empty
    instrument, and the empty ones at the end (after instrument 0) leave the pool."""
    out = dict(instruments)
    for n in gone:
        out[str(n)] = empty_instrument()
    while len(out) > 1 and out[str(max(map(int, out)))]["byte_00"]:
        del out[str(max(map(int, out)))]
    return out
