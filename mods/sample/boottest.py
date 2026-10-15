#!/usr/bin/env python3
"""The sample mod's boot test (docs/modding.md): plays the modded ROM in
headless mGBA (tools/boottest.py's driver) and checks that what the mod
added is there. Run it in the modded tree, after `make`:

    python3 mods/sample/boottest.py [--out LOG.json] [--script NAME ...]

The expected values (level, song, category and cutscene ids) are read from
the tree's generated constants, the colours from the mod's palettes. The
scripts, each from a fresh boot with no save:

- grove: the level select picks LEVEL_SAMPLE_GROVE: it loads the new room
  (gLevelRoom41) and plays the new theme's song, SONG_SAMPLE_TUNE;
- slope: LEVEL_SAMPLE_SLOPE: InitActorCategory(CATEGORY_SAMPLE_ICE), OBJ
  palette RAM holds the category's own palette and BG palettes 0-13 a
  frame of its own palette cycle, not the polar type's;
- world5: with the powers and worlds 1-4's levels cleared poked into the
  save block, Up turns to the fifth page (world 4: 3 entries, no boss
  entry); with its levels cleared the boss entry shows (4 entries), and A
  on it starts LEVEL_SAMPLE_SHOWDOWN: the boss HUD (GetBossIndex is
  BOSS_DINGODILE, GetBossHealth 3); the room exit poked (as the boss's
  power pickup does), clearing it plays CUTSCENE_SAMPLE_CLEARED, whose
  slide plays SONG_SAMPLE_TUNE, and sets the level's cleared bit;
- save: the save menu saves slot 1: the save file is the 64 Kbit EEPROM's
  8 KiB (29 levels don't fit the ROM's 4 Kbit).

Exits 1 if a check fails. mGBA's SDL frontend `mgba` must be on PATH; it
listens on tools/boottest.py's fixed GDB port, so run one at a time (or
each in its own network namespace, `unshare -rn`).
"""
import argparse
import json
import re
import struct
import sys
from pathlib import Path

sys.path.insert(0, "tools")
import boottest as B  # noqa: E402
import jasc_pal  # noqa: E402

KEY_A, KEY_START, KEY_UP, KEY_DOWN, KEY_RIGHT = B.KEY_A, B.KEY_START, B.KEY_UP, B.KEY_DOWN, B.KEY_RIGHT
LS_LAST_INDEX, LS_INDEX, LS_WORLD, LS_SAVE = 0x04, 0x08, 0x0C, 0x9C  # LevelSelect (level_select.hpp)
PROGRESS_FLAGS, PROGRESS_LEVELS = 2, 4  # GameProgress: the flags byte, then a word per level
POWERS = 0xF0  # the four powers' bits of the flags (PROGRESS_BIT_*)
BOSS_COUNTER = 0x10  # BossCtrl::counter


def constants(*headers):
    out = {}
    for h in headers:
        for name, value in re.findall(r"(?m)^#define (\w+) (-?(?:0x[0-9A-Fa-f]+|\d+))\b", Path(h).read_text()):
            out[name] = int(value, 0)
    return out


C = constants("build/include/constants/levels.h", "build/include/constants/songs.h",
              "build/include/constants/categories.h", "build/include/constants/cutscenes.h",
              "include/constants/bosses.h")
OBJ_PALETTE = [v & 0x7FFF for v in jasc_pal.read_jasc("graphics/vehicles/polar/sample_ice.pal")]
CYCLE = [[v & 0x7FFF for v in jasc_pal.read_jasc("graphics/vehicles/polar/sample_ice_cycle/%02d.pal" % k)]
         for k in range(32)]
TYPE_CYCLE = [[v & 0x7FFF for v in jasc_pal.read_jasc("graphics/vehicles/polar/palette_cycle/%02d.pal" % k)]
              for k in range(32)]


def u16s(data):
    return [v & 0x7FFF for v in struct.unpack("<%dH" % (len(data) // 2), data)]


def u32(g, addr):
    return struct.unpack("<I", g.read(addr, 4))[0]


def event(st, *e):
    st.events.append([st.frame] + list(e))


def to_level_select(st):
    yield from B.until_song(st, B.SONG_WARP_ROOM)
    yield from B.wait(90)


def script_grove(st):
    yield from to_level_select(st)
    st.level = C["LEVEL_SAMPLE_GROVE"]
    yield from B.wait(4, KEY_A)
    yield from B.wait(60)
    yield from B.play(400)


def script_slope(st, syms):
    def palettes(g):
        obj = u16s(g.read(0x05000200, 0x200))
        bg = u16s(g.read(0x05000000, 0x1C0))
        banks = [k for k in range(16) if obj[16 * k:16 * k + 16] == OBJ_PALETTE[16 * k:16 * k + 16]]
        event(st, "Palettes", {"obj_banks_as_mod": banks,
                               "bg_cycle_frame": next((k for k, f in enumerate(CYCLE) if f == bg), None),
                               "bg_type_cycle_frame": next((k for k, f in enumerate(TYPE_CYCLE) if f == bg), None)})
    yield from to_level_select(st)
    st.level = C["LEVEL_SAMPLE_SLOPE"]
    yield from B.wait(4, KEY_A)
    yield from B.wait(60)
    for _ in range(3):
        yield from B.play(150)
        st.actions.append(palettes)
    yield from B.wait(2)


def script_world5(st, syms):
    def select(g):
        return u32(g, syms["gLevelSelect"])

    def clear(levels, powers=False):
        def poke(g):
            save = u32(g, select(g) + LS_SAVE)
            if powers:
                g.write(save + PROGRESS_FLAGS, bytes([g.read(save + PROGRESS_FLAGS, 1)[0] | POWERS]))
            for lv in levels:
                a = save + PROGRESS_LEVELS + 4 * lv
                g.write(a, struct.pack("<I", u32(g, a) | 1))
            event(st, "Poke", "cleared", list(levels))
        return poke

    def page(g):
        ls = select(g)
        event(st, "LevelSelect", {"world": u32(g, ls + LS_WORLD), "last_index": u32(g, ls + LS_LAST_INDEX),
                                  "index": u32(g, ls + LS_INDEX)})

    def press(keys, after=100):
        yield from B.wait(4, keys)
        yield from B.wait(after)

    yield from to_level_select(st)
    first = C["LEVEL_SAMPLE_GROVE"]
    st.actions.append(clear(range(first), powers=True))
    for _ in range(4):
        yield from press(KEY_UP)
    st.actions.append(page)
    yield from B.wait(2)
    st.actions.append(clear(range(first, C["LEVEL_SAMPLE_SEWER"] + 1)))
    yield from press(KEY_DOWN)
    yield from press(KEY_UP)
    for _ in range(3):
        yield from press(KEY_RIGHT, 30)
    st.actions.append(page)
    yield from B.wait(2)
    yield from press(KEY_A, 300)

    def exit_room(g):
        g.write(syms["gRoomExitRequested"], b"\1")
        event(st, "Poke", "gRoomExitRequested")
    st.actions.append(exit_room)
    start = len(st.events)
    # A turns the cutscene's page, then back to the level select
    for i in range(3000):
        if any(e[1] == "PlaySong" and e[2] == B.SONG_WARP_ROOM for e in st.events[start:]):
            break
        yield (0, 0, KEY_A, 0)[(i // 4) % 4] if i > 200 else 0
    yield from B.wait(60)

    def word(g):
        save = u32(g, select(g) + LS_SAVE)
        lv = C["LEVEL_SAMPLE_SHOWDOWN"]
        event(st, "LevelWord", lv, u32(g, save + PROGRESS_LEVELS + 4 * lv))
    st.actions.append(word)
    yield from B.wait(2)


SCRIPTS = {"grove": lambda syms: script_grove, "slope": lambda syms: lambda st: script_slope(st, syms),
           "world5": lambda syms: lambda st: script_world5(st, syms),
           "save": lambda syms: B.script_save}


def check(name, log):
    """The failed checks of a script's log, as lines."""
    ev = log["events"]

    def kind(k):
        return [e[2:] for e in ev if e[1] == k]
    fails = []

    def need(ok, what):
        if not ok:
            fails.append(what)
    need(log["end"].startswith("done"), "the run ended: %s" % log["end"])
    if name == "grove":
        need(["gLevelRoom41"] in kind("LoadRoom"), "LoadRoom(gLevelRoom41), the new room")
        need([C["SONG_SAMPLE_TUNE"]] in kind("PlaySong"), "PlaySong(SONG_SAMPLE_TUNE), the new theme's song")
    elif name == "slope":
        need([C["CATEGORY_SAMPLE_ICE"]] in kind("InitActorCategory"), "InitActorCategory(CATEGORY_SAMPLE_ICE)")
        pals = [e[0] for e in kind("Palettes")]
        need(len(pals) == 3, "three palette samples")
        need(all(len(p["obj_banks_as_mod"]) >= 12 for p in pals),
             "OBJ palette RAM: at least 12 of its 16 banks the category's own palette's")
        need(all(p["bg_cycle_frame"] is not None and p["bg_type_cycle_frame"] is None for p in pals),
             "BG palettes 0-13: a frame of the category's own palette cycle, not the type's")
    elif name == "world5":
        pages = [e[0] for e in kind("LevelSelect")]
        need(len(pages) == 2, "two level-select samples")
        if len(pages) == 2:
            need(pages[0]["world"] == C["WORLD_COUNT"] - 1 and pages[0]["last_index"] == 2,
                 "Up four times: the fifth page (world 4), 3 entries (last_index 2)")
            need(pages[1]["world"] == C["WORLD_COUNT"] - 1 and pages[1]["last_index"] == 3
                 and pages[1]["index"] == 3, "its levels cleared: the boss entry (last_index 3), the cursor on it")
        need([C["BOSS_DINGODILE"]] in kind("GetBossIndex"), "InitHud: GetBossIndex() = BOSS_DINGODILE")
        need([3] in kind("GetBossHealth"), "UpdateHudBoss: GetBossHealth() = 3")
        need([C["CUTSCENE_SAMPLE_CLEARED"]] in kind("PlayCutscene"), "PlayCutscene(CUTSCENE_SAMPLE_CLEARED)")
        cut = next((i for i, e in enumerate(ev) if e[1] == "PlayCutscene"), None)
        need(cut is not None and any(e[1] == "PlaySong" and e[2] == C["SONG_SAMPLE_TUNE"] for e in ev[cut:]),
             "the cutscene's slide plays SONG_SAMPLE_TUNE")
        words = kind("LevelWord")
        need(bool(words) and words[0][1] & 1, "LEVEL_SAMPLE_SHOWDOWN's cleared bit set (\"mark_cleared\")")
    elif name == "save":
        need(bool(kind("EEPROMWrite")), "EEPROMWrite: the save menu saved")
        need(log.get("save_size") == 0x2000, "the save file is 8 KiB (the 64 Kbit EEPROM), not %s"
             % log.get("save_size"))
    return fails


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--rom", default="crashbandicootxs.gba")
    ap.add_argument("--elf", default="crashbandicootxs.elf")
    ap.add_argument("--script", action="append", choices=sorted(SCRIPTS))
    ap.add_argument("--out")
    ap.add_argument("--timeout", type=float, default=600, help="seconds per script")
    args = ap.parse_args()
    syms, rom = B.read_elf(args.elf)
    signed = lambda v: v - (1 << 32) if v & 0x80000000 else v  # noqa: E731
    extra = {
        B.call_return(rom, syms["InitHud"], syms["GetBossIndex"]): lambda g: ["GetBossIndex", signed(g.reg(0))],
        B.call_return(rom, syms["UpdateHudBoss"], syms["GetBossHealth"]): lambda g: ["GetBossHealth", g.reg(0)],
    }
    logs, failed = {}, False
    for name in args.script or list(SCRIPTS):
        log = B.run_script(args.rom, syms, rom, SCRIPTS[name](syms), args.timeout, extra)
        # GetBossHealth runs every frame: keep its changes only
        last, events = None, []
        for e in log["events"]:
            if e[1] == "GetBossHealth":
                if e[2] == last:
                    continue
                last = e[2]
            events.append(e)
        log["events"] = events
        logs[name] = log
        fails = check(name, log)
        failed |= bool(fails)
        print("%s: %s (%s s), %d events" % (name, "FAILED" if fails else "OK", log["seconds"], len(events)))
        for e in events:
            if e[1] not in ("EEPROMWrite",):
                print("    frame %d: %s" % (e[0], " ".join(json.dumps(x) for x in e[1:])))
        for f in fails:
            print("  failed: " + f)
    if args.out:
        with open(args.out, "w") as f:
            json.dump(logs, f, indent=1)
    print("sample mod boot test: " + ("FAILED" if failed else "OK"))
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
