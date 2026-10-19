"""The sound data's JSON Schemas (#1167, docs/editors.md).

games/xs/sound/: the music and sound-effect manifests (the ROM's songs,
instruments and samples, as crashdata.audio.gax reads them), music.json
(screen_music), sfx_table.json (crashdata.audio.sfx_table), a new song's
settings (songs/<key>.json: gax.NEW_SONG_DEFAULTS) and a new instrument
(instruments/<name>.json: gax.INSTRUMENT_FIELDS).
"""
from crashdata.common import schema as S

B64 = S.string(r"^[A-Za-z0-9+/]*={0,2}$", description="base64")
SONG_KEY = S.string(r"^[a-z0-9_]+$", description="a song's key (SONG_<KEY>)")
INDEX_KEY = r"^[0-9]+$"


def song_info():
    return {"num_channels": S.integer(S.U16), "num_rows_per_pattern": S.integer(S.U16),
            "num_patterns_per_channel": S.integer(S.U16), "loop_point": S.integer(S.U16),
            "volume": S.integer(S.U16), "ushort_0a": S.integer(S.U16), "sample_rate": S.integer(S.U16),
            "num_fx_channels": S.integer(S.U16), "half_rate_fx": S.integer(S.U16)}


# a ROM song's structure, part of its scaffold (crashdata.audio.gax.SCAFFOLD)
STRUCTURE = ("num_channels", "num_rows_per_pattern", "num_patterns_per_channel", "loop_point")


def instrument():
    sample = S.obj({"byte_02": S.integer(S.U8), "is_bidirectional": S.boolean(),
                    "start_position": S.integer(S.S32), "loop_start": S.integer(S.S32),
                    "loop_end": S.integer(S.S32), "int_10": S.integer(S.S32), "ushort_14": S.integer(S.U16),
                    "ushort_16": S.integer(S.U16), "pitch": S.integer(S.S16)}, required="all")
    row = S.obj({"relative_note_number": S.integer(-0x80, 0xFF), "dont_use_note_pitch": S.boolean(),
                 "sample_index": S.integer(S.U8), "byte_03": S.integer(S.U8),
                 "effects": S.array(S.obj({"type": S.integer(S.U8), "param": S.integer(S.U8)}, required="all"),
                                    length=2)}, required="all")
    point = S.obj({"x": S.integer(S.U16), "interpolation": S.integer(S.S16), "y": S.integer(S.U8),
                   "byte_05": S.integer(S.U8), "ushort_06": S.integer(S.U16)}, required="all")
    env = S.obj({"sustain": S.nullable(S.integer(S.U8)), "loop_start": S.nullable(S.integer(S.U8)),
                 "loop_end": S.nullable(S.integer(S.U8)), "points": S.array(point, min_items=1)}, required="all")
    return S.obj({
        "byte_00": S.integer(S.U8), "sample_indices": S.array(S.integer(S.U8), length=4),
        "byte_05": S.integer(S.U8), "byte_06": S.integer(S.U8), "byte_07": S.integer(S.U8),
        "vibrato_delay": S.integer(S.U8), "vibrato_depth": S.integer(S.U8), "vibrato_speed": S.integer(S.U8),
        "byte_0b": S.integer(S.U8), "unknown12": S.array(S.integer(S.U8), length=12),
        "row_speed": S.integer(S.U8), "ushort_12": S.integer(S.U16),
        "samples": S.array(sample), "rows": S.array(row, min_items=1), "envelope": S.nullable(env),
    }, required="all")


def gax_manifest():
    from crashdata.audio import gax as G

    # a song's settings as GAX2 plays them (the music editor edits them): one of its mix rates, the
    # sound effects at half rate or not
    settings = {"sample_rate": S.enum(G.GAX_MIX_RATES, description="the mix rate, Hz (gGaxMixRates)"),
                "half_rate_fx": S.integer(0, 1, description="the sound-effect voices mixed at half the mix rate")}
    song = S.obj(dict(song_info(), **settings, **{
        "num_items": S.integer(S.U16), "unk_ptr_is_null": S.boolean(), "no_pool": S.boolean(),
        "mixer_data_bytes": S.array(S.integer(S.U8), length=28, description="the mixer's echo taps"),
        "channel_transposes": S.array(S.array(S.integer(-0x8000, 0xFFFF))),
        "pattern_groups": S.array(S.array(S.integer(0, 0xFFFF))),
        "title": S.string(), "artist": S.string(), "title_bytes": B64,
        "pattern_group_order": S.array(S.integer(0, 0xFFFF)), "channel_order": S.array(S.integer(0, 0xFFFF)),
    }), required=[f for f in song_info() if f not in STRUCTURE] + ["mixer_data_bytes", "title", "artist"],
        description="a ROM song: its settings and, while it keeps it, the scaffold the ROM recorded (its "
                    "structure, pattern sharing and layout); a song without it is built from its XM")
    root = S.obj({
        "instruments": S.obj(pattern_properties={INDEX_KEY: S.ref("instrument")},
                             description="the ROM's instrument pool, by index"),
        "samples": S.obj(pattern_properties={INDEX_KEY: S.obj({"has_data": S.boolean()}, required="all")},
                         description="the ROM's samples, by index (their WAVs under samples/)"),
        "songs": S.obj(pattern_properties={r"^[a-z0-9_]+$": S.ref("song")},
                       description="the ROM's songs (their XMs under songs/)"),
        "song_order": S.array(SONG_KEY, description="the ROM's songs in their music block order"),
        "song_table": S.array(S.obj({"song": SONG_KEY, "comment": S.string()}, required=["song"]),
                              description="gSongTable, in song id order: SONG_<KEY> is the index"),
        "no_song_comment": S.string(),
        "default_song": S.ref("song"),
    }, required=["instruments", "samples", "songs", "song_order", "song_table"])
    return S.document("gax_manifest.schema.json", "The music (gax_manifest.json)", root,
                      {"instrument": instrument(), "song": song},
                      "games/xs/sound/gax_manifest.json (docs/audio.md, \"Build pipeline\")")


def gax_sfx_manifest():
    root = S.obj({
        "song_info": S.obj(song_info(), required="all"),
        "voice_types": S.integer(S.U16),
        "instruments": S.obj(pattern_properties={INDEX_KEY: S.ref("instrument")}),
        "samples": S.obj(pattern_properties={INDEX_KEY: S.obj({"has_data": S.boolean()}, required="all")}),
    }, required="all")
    return S.document("gax_sfx_manifest.schema.json", "The sound-effect set (gax_sfx_manifest.json)", root,
                      {"instrument": instrument()}, "games/xs/sound/gax_sfx_manifest.json (docs/audio.md, "
                                                    "\"Sound effects\")")


def music():
    root = S.obj({"screen_music": S.array(S.obj({
        "screen": S.string(r"^[a-z0-9_]+$", description="SCREEN_SONG_<SCREEN>"),
        "song": SONG_KEY, "comment": S.string()}, required=["screen", "song"]))}, required="all")
    return S.document("music.schema.json", "The screens' music (music.json)", root,
                      description="games/xs/sound/music.json (docs/audio.md, \"Which song plays where\")")


def sfx_table():
    entry = S.obj({"slot_id": S.integer(S.S32, description="an instrument of the sound-effect set"),
                   "instrument": S.string(r"^[a-z0-9_]+$", description="a new instrument, instruments/<name>.json"),
                   "pitch_offset": S.integer(S.S32), "volume": S.integer(S.S32),
                   "name": S.string(r"^[A-Z0-9_]+$", description="SFX_<NAME>"), "comment": S.string()},
                  required=["pitch_offset", "volume"])
    root = S.obj({"none_comment": S.string(),
                  "entries": S.array(S.one_of(dict(entry, required=entry["required"] + ["slot_id"]),
                                              dict(entry, required=entry["required"] + ["instrument"])),
                                     description="gSfxTable, in sound id order")},
                 required=["entries"])
    return S.document("sfx_table.schema.json", "The sound effects (sfx_table.json)", root,
                      description="games/xs/sound/sfx_table.json (docs/audio.md, \"Sound effects\")")


def song_settings():
    from crashdata.audio import gax as G
    props = {"title": S.string(), "artist": S.string(), "volume": S.integer(S.U16, description="Q8: 256 is 1.0"),
             "sample_rate": S.enum(G.GAX_MIX_RATES, description="the mix rate, Hz"),
             "num_fx_channels": S.integer(S.U16), "half_rate_fx": S.integer(0, 1),
             "mixer_data_bytes": S.array(S.integer(S.U8), length=28)}
    props = S.with_keys(props, G.NEW_SONG_DEFAULTS)
    return S.document("song_settings.schema.json", "A new song's settings (songs/<key>.json)", S.obj(props),
                      description="games/xs/sound/songs/<key>.json, next to its XM (docs/audio.md, \"Adding a "
                                  "song\")")


def new_instrument():
    from crashdata.audio import gax as G
    props = {
        "sample": S.string(min_length=1, description="the WAV, a path under games/xs/sound/"),
        "rate": S.any_of(S.integer(1, None), {"type": "number", "minimum": 0}, description="Hz"),
        "loop": S.enum(G.LOOP_KINDS), "loop_start": S.integer(0, None), "loop_end": S.integer(0, None),
        "volume": S.integer(0, 64),
        "envelope": S.obj({"points": S.array(S.array(S.integer(0, 0xFFFF), length=2), min_items=1,
                                             max_items=G.MAX_ENVELOPE_POINTS, description="[tick, volume 0-64]"),
                           "sustain": S.integer(S.U8), "loop_start": S.integer(S.U8), "loop_end": S.integer(S.U8)},
                          required=["points"]),
        "vibrato": S.obj({"delay": S.integer(0, 255), "depth": S.integer(0, 255), "speed": S.integer(0, 63)}),
        "comment": S.string(),
    }
    props = S.with_keys(props, G.INSTRUMENT_FIELDS)
    return S.document("instrument.schema.json", "A new instrument (instruments/<name>.json)",
                      S.obj(props, required=["sample"]),
                      description="games/xs/sound/instruments/<name>.json (docs/audio.md, \"From a WAV\")")


# schema name -> (builder, the files' globs under the game's sound directory)
SCHEMAS = {
    "gax_manifest": (gax_manifest, ["gax_manifest.json"]),
    "gax_sfx_manifest": (gax_sfx_manifest, ["gax_sfx_manifest.json"]),
    "music": (music, ["music.json"]),
    "sfx_table": (sfx_table, ["sfx_table.json"]),
    "song_settings": (song_settings, ["songs/*.json"]),
    "instrument": (new_instrument, ["instruments/*.json"]),
}
