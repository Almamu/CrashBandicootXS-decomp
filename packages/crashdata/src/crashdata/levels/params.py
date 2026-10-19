"""A room's entity parameter records, by name (#1167, #1093; docs/levels.md, "Parameter records").

An entity's "param" is the index of one of its room's parameter records,
the bytes at `params + paramOffsets[param]` that its spawner reads
through one of the views of engine/include/game/level/level_data.h's
`struct entity_params` (the flags byte, then per-type words) or
crate.hpp's `struct crate_placement`. In room.json a record is either

- its 32-bit words, `[0, 327707]`: the raw form, for a record whose
  meaning isn't known (a record no entity uses, one that
  entities of two views share);
- or the view's fields by name, `{"layout": "crate", "flags":
  ["TRIAL_KIND"], "options": [], "unk_02": 0, "trial_kind": "TIME_CRATE_3",
  "param6": 5, "param7": 0}`.

Which view an entity type's spawner reads is entity_types.json's
"params" (LAYOUTS below, by name). N-Tranced's spawners read some of XS's
views (its patrolling enemies' `patrol`, the `flags` byte) and views of
their own, named from its assembly (N2_LAYOUTS below; docs/n_tranced.md,
"Parameter records"): its crates' bytes are XS's, but their flag and
option bits mean other things, so they have views of their own. The named form holds the record's
bytes exactly: the fields in order (a record may end before the view
does, at a field's end rounded up to a word: the ROM's records are as
long as what their entities read), every byte the view doesn't name
zero, and the words past the view's end, which nothing names yet, as
"extra". So the build writes the same words either way, and the ROM is
the same.

`tools/levels.py name-params [ROOMDIR ...]` rewrites raw records in the
named form where the room's entity types say how (the one-time migration
of #1167; `extract` writes the named form itself), and `make data-check`
fails on a raw record that could be named.
"""
import struct

# The flags byte (ENTITY_PARAMS_FLAG_*, constants/entity_params.h; bits 5-7
# CRATE_PLACEMENT_FLAG_*, crate.hpp; docs/flags/entity_params.md): one name
# per bit. Bit 6 of a checkpoint crate is its respawn's X mirror
# (CRATE_PLACEMENT_FLAG_CHECKPOINT_MIRROR_X).
FLAG_BITS = ["EXIT_MIRROR", "FLIP_X", "FLIP_Y", "UNK3", "UNK4", "TRIAL_KIND", "ASSIST_AKU_AKU", "ASSIST_CHECKPOINT"]
# A crate placement's options byte (CRATE_PLACEMENT_OPTION_*, crate.hpp).
OPTION_BITS = ["ASSIST_LIFE", "SLOT_MYSTERY", "SLOT_LIFE", "SLOT_TNT", "BIT4", "BIT5", "BIT6", "BIT7"]
# N-Tranced's crate placement bits (its CreateCrate, sub_08011C80 in
# games/n2/asm/code_08011C80.s): the flags' bits 1, 2 and 5 are XS's (the
# flips, the time-trial kind); bit 6 sets the crate's +0x7A (BreakCrate
# reads it; what for isn't known), bit 7 turns a "?" or slot crate into an
# Aku Aku crate with crate assistance (XS's bit 6) and is a checkpoint
# crate's respawn mirror. The options: bit 0 a checkpoint, bit 1 a life
# crate with crate assistance; bits 2-4 a slot crate's faces (XS's 1-3).
N2_CRATE_FLAG_BITS = ["BIT0", "FLIP_X", "FLIP_Y", "UNK3", "UNK4", "TRIAL_KIND", "UNK6", "ASSIST_AKU_AKU"]
N2_CRATE_OPTION_BITS = ["ASSIST_CHECKPOINT", "ASSIST_LIFE", "SLOT_MYSTERY", "SLOT_LIFE", "SLOT_TNT",
                        "BIT5", "BIT6", "BIT7"]
# N-Tranced's type 73's flags (sub_0801ED08): bit 0 sets the sprite's +0x18
# bit 4 (always active), bit 1 starts its controller triggered
# (sub_08014090: state 4, +0x1C set). Its other bits aren't read.
N2_TRAP_FLAG_BITS = ["ALWAYS_ACTIVE", "STARTS_TRIGGERED", "BIT2", "BIT3", "BIT4", "BIT5", "BIT6", "BIT7"]
# N-Tranced's type 106's flags (sub_0801FF2C): bit 3 (its controller's
# +0x10) locks the camera (its mode 3) once Crash is in the trigger's box
# and plays the cutscene when he is off screen (IsSpriteObjOnScreen);
# without it the cutscene plays as he enters. Its other bits aren't read.
N2_CUTSCENE_FLAG_BITS = ["BIT0", "BIT1", "BIT2", "LOCK_CAMERA", "BIT4", "BIT5", "BIT6", "BIT7"]

# A field: (name, byte offset, kind). The kinds: a struct format letter
# (B, b, H, h, I, i: u8, s8, u16, s16, u32, s32), "flags"/"options" (a byte as
# the list of its bits' names; "n2_crate_flags"/"n2_crate_options",
# N-Tranced's crates', "n2_trap_flags"/"n2_cutscene_flags" its types 73's
# and 106's) or "entity" (an s16 entity type: its name in
# entity_types.json, or its number).
_FLAGS = ("flags", 0, "flags")
_CRATE = [_FLAGS, ("options", 1, "options"), ("unk_02", 2, "H"), ("trial_kind", 4, "entity"),
          ("param6", 6, "B"), ("param7", 7, "B")]

# The views, by name: their fields (level_data.h's struct entity_params.p
# views, and crate.hpp's struct crate_placement), and the struct's size.
LAYOUTS = {
    "flags": {"fields": [_FLAGS], "size": 1,
              "about": "the flags byte only: a spawner that reads no per-type words"},
    "patrol": {"fields": [_FLAGS, ("range_x", 4, "i")], "size": 8,
               "about": "p.patrol: the X range either side of the spawn point (SetRangeX)"},
    "cycle": {"fields": [_FLAGS, ("idle_time", 4, "i"), ("attack_time", 8, "i"), ("cycle_offset", 12, "i"),
                         ("wave_amplitude", 16, "i"), ("wave_period", 20, "i"), ("wave_phase", 24, "i")],
              "size": 28, "about": "p.cycle: the attack cycle (SetAttackCycle), then the wave (SetWave)"},
    "attack_first": {"fields": [_FLAGS, ("attack_time", 4, "i"), ("idle_time", 8, "i"), ("cycle_offset", 12, "i")],
                     "size": 16, "about": "p.attackFirst: the attack cycle, the attack's time first"},
    "range_cycle": {"fields": [_FLAGS, ("range_x", 4, "i"), ("idle_time", 8, "i"), ("attack_time", 12, "i"),
                               ("cycle_offset", 16, "i")], "size": 20,
                    "about": "p.rangeCycle: the X range and the attack cycle"},
    "range_attack_first": {"fields": [_FLAGS, ("range_x", 4, "i"), ("attack_time", 8, "i"), ("idle_time", 12, "i"),
                                      ("cycle_offset", 16, "i")], "size": 20,
                           "about": "p.rangeAttackFirst: the X range and the attack cycle, the attack first"},
    "range_homing_y": {"fields": [_FLAGS, ("range_x", 4, "i"), ("range_y", 8, "i"), ("speed_y", 12, "i"),
                                  ("accel_y", 16, "i")], "size": 20,
                       "about": "p.rangeHomingY: the X range, and Y homing (SetRangeYSpeed)"},
    "wave": {"fields": [_FLAGS, ("amplitude", 4, "i"), ("period", 8, "i"), ("phase", 12, "i")], "size": 16,
             "about": "p.wave: SetWave's"},
    "homing_x": {"fields": [_FLAGS, ("range_x", 4, "i"), ("speed_x", 8, "i"), ("accel_x", 12, "i")], "size": 16,
                 "about": "p.homingX: X homing (SetRangeXSpeed)"},
    "homing_xy": {"fields": [_FLAGS, ("range_y", 4, "i"), ("speed_y", 8, "i"), ("accel_y", 12, "i"),
                             ("range_x", 16, "i"), ("speed_x", 20, "i"), ("accel_x", 24, "i")], "size": 28,
                  "about": "p.homingXY: Y homing, then X homing"},
    "platform": {"fields": [_FLAGS, ("type", 4, "I"), ("dist_x", 8, "i"), ("dist_y", 12, "i"), ("dir_x", 16, "h"),
                            ("dir_y", 18, "h"), ("flag", 20, "h")], "size": 22,
                 "about": "p.platform (Platform::Create): PLATFORM_TYPE_*, a moving platform's path and "
                          "directions, and whether it is always active"},
    "crate": {"fields": _CRATE, "size": 8,
              "about": "struct crate_placement (Crate::Create): the flags, the options, the time-trial kind "
                       "and two per-kind bytes"},
    "iron_switch_crate": {"fields": _CRATE[:4] + [("group", 6, "B"), ("steps", 7, "B"), ("step_delay", 8, "B")],
                          "size": 9, "about": "an iron switch crate's placement: its outline crates' group, "
                                              "the step count and the step delay"},
    "outline_crate": {"fields": _CRATE[:4] + [("group", 6, "B"), ("step", 7, "B"), ("solid_kind", 8, "entity")],
                      "size": 10, "about": "an outline crate's placement: its group, its step and solidKind"},
}


# N-Tranced's views (#1167), each from what its spawners read in
# games/n2/asm (the record at params + paramOffsets[param], through the
# level at 0x02000E64, as XS's): docs/n_tranced.md, "Parameter records",
# gives each one's evidence. Its types that read only the flags byte (or
# nothing) and its patrolling enemies (the X range either side, `ldr` +4
# into the controller's +0x198) use XS's "flags" and "patrol".
_N2_CRATE = [("flags", 0, "n2_crate_flags"), ("options", 1, "n2_crate_options"), ("unk_02", 2, "H"),
             ("trial_kind", 4, "entity"), ("param6", 6, "B"), ("param7", 7, "B")]
N2_LAYOUTS = {
    "n2_crate": {"fields": _N2_CRATE, "size": 8,
                 "about": "N-Tranced's crate placement (CreateCrate, sub_08011C80): XS's bytes, its own bits"},
    "n2_iron_switch_crate": {"fields": _N2_CRATE[:4] + [("group", 6, "B"), ("steps", 7, "B"), ("step_delay", 8, "B")],
                             "size": 9, "about": "N-Tranced's iron switch crate: its outline crates' group, the "
                                                 "step count and the step delay"},
    "n2_outline_crate": {"fields": _N2_CRATE[:4] + [("group", 6, "B"), ("step", 7, "B"), ("solid_kind", 8, "entity")],
                         "size": 10, "about": "N-Tranced's outline crate: its group, its step and solidKind"},
    "patrol_xy": {"fields": [_FLAGS, ("range_x", 4, "i"), ("range_y", 8, "i")], "size": 12,
                  "about": "N-Tranced's scarab: the X and Y ranges either side of the spawn point"},
    "patrol_y": {"fields": [_FLAGS, ("range_y", 4, "i")], "size": 8,
                 "about": "N-Tranced's standing scorpion: the Y range either side of the spawn point"},
    "platform_group": {"fields": [_FLAGS, ("type", 4, "I"), ("dist_x", 8, "i"), ("dist_y", 12, "i"),
                                  ("dir_x", 16, "h"), ("dir_y", 18, "h"), ("flag", 20, "h"), ("group", 24, "i")],
                       "size": 28, "about": "N-Tranced's platforms: XS's platform view (type isn't read), and the "
                                            "1-based group it is listed in (0: none)"},
    "circle_platform": {"fields": [_FLAGS, ("type", 4, "I"), ("radius_x", 8, "i"), ("radius_y", 12, "i"),
                                   ("angle", 20, "i"), ("speed", 24, "i")], "size": 28,
                        "about": "N-Tranced's circling platform: its radii, start angle (degrees, steps of 45) and "
                                 "angular speed (type isn't read)"},
    "group": {"fields": [_FLAGS, ("group", 4, "i")], "size": 8,
              "about": "N-Tranced's grouped hazards: the 1-based group they are listed in (sub_08025DD0)"},
    "group_state": {"fields": [_FLAGS, ("group", 4, "i"), ("state", 8, "B")], "size": 9,
                    "about": "N-Tranced's type 67: its group, and its state (1: starts triggered, in no group)"},
    "width": {"fields": [_FLAGS, ("width", 4, "i")], "size": 8,
              "about": "N-Tranced's type 72: its box's width in pixels"},
    "path_point": {"fields": [_FLAGS, ("path", 4, "b"), ("action", 5, "B"), ("index", 6, "b")], "size": 7,
                   "about": "N-Tranced's path points: the path, what a follower does there (1 reverse, 2 stop) "
                            "and the point's index"},
    "path": {"fields": [_FLAGS, ("path", 4, "b")], "size": 5,
             "about": "N-Tranced's path follower: the path it follows"},
    "hint_text": {"fields": [_FLAGS, ("text", 4, "b")], "size": 5,
                  "about": "N-Tranced's power hint: its UI text (0x7A + text)"},
    "trap_group": {"fields": [("flags", 0, "n2_trap_flags"), ("group", 4, "i")], "size": 8,
                   "about": "N-Tranced's type 73: always active, starts triggered, and the 1-based group it "
                            "joins (0: none)"},
    "carrier_path": {"fields": [_FLAGS, ("path", 4, "i")], "size": 8,
                     "about": "N-Tranced's type 92: the path it carries Crash along (the path table at "
                              "0x020001C8; its flags aren't read)"},
    "cutscene_trigger": {"fields": [("flags", 0, "n2_cutscene_flags"), ("cutscene", 4, "b")], "size": 5,
                         "about": "N-Tranced's type 106: the cutscene it plays (sub_08043A20) and whether it "
                                  "locks the camera first"},
}
LAYOUTS.update(N2_LAYOUTS)

_FMT = {"B": "<B", "b": "<b", "H": "<H", "h": "<h", "I": "<I", "i": "<i", "flags": "<B", "options": "<B",
        "n2_crate_flags": "<B", "n2_crate_options": "<B", "n2_trap_flags": "<B", "n2_cutscene_flags": "<B",
        "entity": "<h"}
_BITS = {"flags": FLAG_BITS, "options": OPTION_BITS, "n2_crate_flags": N2_CRATE_FLAG_BITS,
         "n2_crate_options": N2_CRATE_OPTION_BITS, "n2_trap_flags": N2_TRAP_FLAG_BITS,
         "n2_cutscene_flags": N2_CUTSCENE_FLAG_BITS}


def _size(kind):
    return struct.calcsize(_FMT[kind])


def _align(n):
    return (n + 3) & ~3


def _layout_end(layout):
    return max(o + _size(k) for _, o, k in LAYOUTS[layout]["fields"])


def bits_list(v, names):
    return [names[b] for b in range(8) if v >> b & 1]


def decode(words, layout, type_names=()):
    """A record's words in `layout`'s named form, or None when the named
    form can't hold them exactly (a field cut short, a byte the view
    doesn't name that isn't zero, a value it can't spell)."""
    data = struct.pack("<%dI" % len(words), *words)
    fields = LAYOUTS[layout]["fields"]
    end = _align(_layout_end(layout))
    out = {"layout": layout}
    used = bytearray(len(data))
    last = 0
    for name, off, kind in fields:
        size = _size(kind)
        if off + size > len(data):
            if off < len(data):
                return None  # a field cut short
            break
        (v,) = struct.unpack_from(_FMT[kind], data, off)
        if kind in _BITS:
            v = bits_list(v, _BITS[kind])
        elif kind == "entity":
            v = type_names[v] if 0 <= v < len(type_names) and type_names[v] else v
        out[name] = v
        used[off:off + size] = b"\1" * size
        last = off + size
    body = len(data) if len(data) <= end else end
    if _align(last) != body:
        return None  # the record ends inside the view, not after a field
    if any(b and not u for b, u in zip(data[:body], used[:body])):
        return None  # a byte the view doesn't name
    if len(data) > end:
        out["extra"] = list(words[end // 4:])
    if encode(out, {n: t for t, n in enumerate(type_names) if n}) != list(words):
        return None
    return out


def encode(rec, type_by_name):
    """A record's words: a raw record's own, a named one's from its fields."""
    if isinstance(rec, list):
        return list(rec)
    layout = rec["layout"]
    fields = LAYOUTS[layout]["fields"]
    last = 0
    for name, off, kind in fields:
        if name in rec:
            last = off + _size(kind)
    extra = rec.get("extra", [])
    end = _align(_layout_end(layout))
    data = bytearray(end if extra else _align(last))
    for name, off, kind in fields:
        if name not in rec:
            continue
        v = rec[name]
        if kind in _BITS:
            v = sum(1 << _BITS[kind].index(b) for b in v)
        elif kind == "entity" and isinstance(v, str):
            v = type_by_name[v]
        struct.pack_into(_FMT[kind], data, off, v)
    words = list(struct.unpack("<%dI" % (len(data) // 4), bytes(data)))
    return words + list(extra)


def check(rec, type_by_name):
    """What's wrong with a named record, as (field, message) pairs: a field
    missing before the last one given, extra words without every field,
    an unknown entity type name."""
    if isinstance(rec, list):
        return []
    out = []
    fields = LAYOUTS[rec["layout"]]["fields"]
    given = [name in rec for name, _, _ in fields]
    if True in given:
        last = len(given) - given[::-1].index(True)
        for (name, _, _), g in zip(fields[:last], given[:last]):
            if not g:
                out.append((name, "no \"%s\": a record holds every field up to its last one" % name))
    if rec.get("extra") and not all(given):
        out.append(("extra", "\"extra\" words come after every field of the %s view" % rec["layout"]))
    for name, _, kind in fields:
        if kind == "entity" and isinstance(rec.get(name), str) and rec[name] not in type_by_name:
            out.append((name, "unknown entity type %s" % rec[name]))
    return out


# ---------------------------------------------------------------------------
# The schemas (crashdata.levels.schema)

def record_schemas(S):
    """{"param_<layout>": schema} of each named form."""
    out = {}
    for layout, spec in LAYOUTS.items():
        props = {"layout": S.const(layout)}
        for name, _, kind in spec["fields"]:
            if kind in _BITS:
                props[name] = S.array(S.enum(_BITS[kind]), description="the byte's bits, by name")
            elif kind == "entity":
                props[name] = S.any_of(S.string(r"^[A-Z][A-Z0-9_]*$", description="an entity type's name"),
                                       S.integer(S.S16))
            else:
                lo, hi = {"B": S.U8, "b": (-0x80, 0x7F), "H": S.U16, "h": S.S16, "I": S.U32, "i": S.S32}[kind]
                props[name] = S.integer(lo, hi)
        props["extra"] = S.array(S.integer(S.U32), min_items=1,
                                 description="the record's words past the view, which nothing names yet")
        out["param_" + layout] = S.obj(props, required=["layout"], description=spec["about"])
    return out


def record_schema(S, raw):
    return S.any_of(raw, *[S.ref("param_" + layout) for layout in LAYOUTS])


# ---------------------------------------------------------------------------
# Naming a room's records

def layouts_of(room, type_layouts, type_names):
    """{record index: the set of layouts its entities' types read it with}
    (None for a type with no "params" in entity_types.json)."""
    by_name = {n: t for t, n in enumerate(type_names) if n}
    out = {}
    for e in room["entities"]:
        t = by_name.get(e["type"], e["type"]) if isinstance(e["type"], str) else e["type"]
        out.setdefault(e["param"], set()).add(type_layouts[t] if 0 <= t < len(type_layouts) else None)
    return out


def nameable(room, type_layouts, type_names):
    """{record index: its named form} of the room's raw records that one
    layout names exactly."""
    out = {}
    for i, users in layouts_of(room, type_layouts, type_names).items():
        if not 0 <= i < len(room["params"]) or len(users) != 1 or None in users:
            continue
        rec = room["params"][i]
        if isinstance(rec, list):
            named = decode(rec, next(iter(users)), type_names)
            if named is not None:
                out[i] = named
    return out
