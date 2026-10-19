"""tools/preview.py: play a level's room, or a cutscene, in the PC port (#1167, docs/editors.md "Preview").

    tools/preview.py LEVEL [ROOM] [--headless] [--frames N] [--every N]
                     [--screens DIR] [--out PNG] [--keys FILE] [--sdl 0|1]
                     [--no-build] [--json]
    tools/preview.py --cutscene CUTSCENE [--language LANG] [--headless] [--every N] ...

Builds the host executable (`make PLATFORM=pc`, build/pc/crashbandicootxs_pc,
docs/platforms.md) from this tree's data, then runs it with a key file
that goes through the logos and menus to the level select
(platform/pc/preview.keys), where `--level` and `--room` (level_pick.c)
make the game start LEVEL in its room ROOM.

LEVEL is gLevelTable's index, a level's constant (LEVEL_JUNGLE_JAM or
JUNGLE_JAM), its name ("jungle jam") or folder (jungle_jam), or a room
folder (snow_crash/part2 or jungle_jam/bonus: its level, and the room).
ROOM is the index in the level's "rooms" (the rooms it plays in order; 0
by default) or one of them by folder, or "bonus" or "gem_path": the
level's bonus or gem-path room, which the game enters from the level and
the PC port's `--room bonus|gem-path` (level_pick.c) starts in.

In a window (an executable built with SDL2) the game runs paced, and the
keyboard and gamepad play it from the level select on. --headless (or a
build without SDL2) runs it as fast as it goes: it stops at --frames
(the level's first picture by default, PREVIEW_FRAME), writes the
pictures of every --every'th frame to --screens DIR (FRAME.png) and the
last one to --out. --json prints a report: the level and room, the
command and the pictures.

With --cutscene (an id, or a constant: CUTSCENE_INTRO or INTRO) the game
plays that cutscene at boot, in place of the legal notice
(platform/pc/src/cutscene_pick.c), in LANG (en, fr, de, es, it, nl or
0-5; English by default), then boots on. In a window the keyboard plays
it; headless, A is pressed every --every frames (60 by default) and the
picture before each press is kept: a picture per screen of text (or
slide without text), --frames by default as many as the cutscene's
screens need (crashdata.cutscenes.edit.cutscene_steps).

The build takes HOST_SDL from --sdl (1: with the window; 0: headless
only; by default with the window when pkg-config finds SDL2, as
platform/pc/platform.mk does). For a mod, run it in the mod's tree
(`make preview MOD=mods/sample LEVEL=...`).
"""
import argparse
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

from crashdata.common import diagnostics as D
from crashdata.common.game import ROOT

EXE = Path("build/pc/crashbandicootxs_pc")
KEYS = Path("platform/pc/preview.keys")
# The keys reach the level select at frame 517 and the level's room has
# loaded and drawn its first picture by frame 700 (boot.keys's run).
PREVIEW_FRAME = 700


def levels():
    from crashdata import levels as L
    return L.read_json(L.LEVELS_JSON)["levels"]


def norm(s):
    return re.sub(r"[^a-z0-9]+", "_", str(s).lower()).strip("_")


# A level's rooms that aren't in its "rooms": levels.json's key, and the
# PC port's --room for it (level_pick.c)
EXTRA_ROOMS = {"bonus": "bonus", "gem_path": "gem-path"}


def room_entries(lv):
    return [r if isinstance(r, str) else "stage %s" % r.get("stage", r) for r in lv["rooms"]]


def extra_room(lv, room):
    """The key of `lv`'s extra room that `room` names ("bonus", "gem_path",
    "gem-path" or its folder), or None."""
    key = norm(room)
    for k in EXTRA_ROOMS:
        if lv.get(k) and (key == k or room == lv[k]):
            return k
    return None


def resolve(level, room=None):
    """(level index, room: its index in "rooms" or "bonus"/"gem_path", the
    level's constant, the room) of the arguments, or a DataError."""
    lvs = levels()
    where = "games/xs/data/levels/levels.json"
    idx, ridx = None, None
    if re.fullmatch(r"\d+", str(level)):
        idx = int(level)
        if idx >= len(lvs):
            D.error("level %d: there are %d levels (0-%d)" % (idx, len(lvs), len(lvs) - 1), where, "/levels",
                    code="preview-level")
    else:
        key = norm(level)
        for i, lv in enumerate(lvs):
            names = {norm(lv["constant"]), norm(lv["constant"][len("LEVEL_"):]), norm(lv["name"])}
            names |= {norm(r.split("/")[0]) for r in lv["rooms"] if isinstance(r, str)}
            if key in names:
                idx = i
                break
            if "/" in str(level) and level in lv["rooms"]:
                idx, ridx = i, lv["rooms"].index(level)
                break
            if "/" in str(level) and extra_room(lv, level):
                idx, ridx = i, extra_room(lv, level)
                break
        if idx is None:
            D.error("no level %s" % level, where, "/levels", code="preview-level")
    lv = lvs[idx]
    if room is not None and extra_room(lv, room):
        ridx = extra_room(lv, room)
    elif room is not None and norm(room) in EXTRA_ROOMS:
        D.error("%s has no %s room" % (lv["constant"], norm(room).replace("_", "-")), where, "/levels/%d" % idx,
                code="preview-room")
    elif room is not None:
        if re.fullmatch(r"\d+", str(room)):
            ridx = int(room)
        elif room in lv["rooms"]:
            ridx = lv["rooms"].index(room)
        else:
            D.error("%s plays no room %s (its rooms: %s)" % (lv["constant"], room, ", ".join(room_entries(lv))),
                    where, "/levels/%d/rooms" % idx, code="preview-room")
    if ridx in EXTRA_ROOMS:
        return idx, ridx, lv["constant"], lv[ridx]
    ridx = ridx or 0
    if not 0 <= ridx < len(lv["rooms"]):
        D.error("%s has %d rooms (0-%d): %s" % (lv["constant"], len(lv["rooms"]), len(lv["rooms"]) - 1,
                                                ", ".join(room_entries(lv))),
                where, "/levels/%d/rooms" % idx, code="preview-room")
    return idx, ridx, lv["constant"], room_entries(lv)[ridx]


LANGUAGES = ["en", "fr", "de", "es", "it", "nl"]


def resolve_cutscene(cutscene, language):
    """(the cutscene's id, its constant, the language's index and code)."""
    from crashdata.common.game import Game
    from crashdata.cutscenes import edit

    game = Game("xs", ".")
    where = "games/xs/data/cutscenes.json"
    if not edit.available(game):
        D.error("this tree has no %s: no cutscenes to play" % where, code="preview-cutscene")
    cutscenes = D.read_json(edit.Files(game).cutscenes)["cutscenes"]
    names = [c["constant"] for c in cutscenes]
    key = str(cutscene).upper()
    if re.fullmatch(r"\d+", key) and int(key) < len(names):
        idx = int(key)
    elif key in names or "CUTSCENE_" + key in names:
        idx = names.index(key if key in names else "CUTSCENE_" + key)
    else:
        D.error("no cutscene %s (%s)" % (cutscene, ", ".join(names)), where, "/cutscenes", code="preview-cutscene")
    lang = str(language or "en").lower()
    if lang.isdigit() and int(lang) < len(LANGUAGES):
        lang = LANGUAGES[int(lang)]
    if lang not in LANGUAGES:
        D.error("no language %s (%s, or 0-5)" % (language, ", ".join(LANGUAGES)), code="preview-cutscene")
    return idx, names[idx], LANGUAGES.index(lang), lang


def cutscene_keys(path, every, presses):
    """A key file that presses A every `every` frames, `presses` times."""
    lines = ["# tools/preview.py --cutscene: A every %d frames" % every]
    for k in range(1, presses + 1):
        lines += ["%d A" % (k * every + 1), "%d -" % (k * every + 3)]
    Path(path).write_text("\n".join(lines) + "\n")


def build(sdl):
    cmd = ["make", "-j%d" % (os.cpu_count() or 1), "PLATFORM=pc"] + ([] if sdl is None else ["HOST_SDL=%d" % sdl])
    sys.stdout.flush()
    r = subprocess.run(cmd, stdout=sys.stderr)
    if r.returncode:
        D.error("the host build failed (%s)" % " ".join(cmd), code="preview-build")


def run():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("level", nargs="?", help="the level: its index, constant, name, folder, or a room folder")
    ap.add_argument("room", nargs="?", help="the room: its index in the level's rooms, or its folder")
    ap.add_argument("--headless", action="store_true", help="no window: run, write the pictures and stop")
    ap.add_argument("--frames", type=int, help="stop after N game frames (headless: %d)" % PREVIEW_FRAME)
    ap.add_argument("--every", type=int, help="headless: a picture every N frames (only the last by default)")
    ap.add_argument("--screens", help="headless: write the pictures to DIR/FRAME.png")
    ap.add_argument("--out", help="headless: copy the last picture to this PNG")
    ap.add_argument("--keys", default=str(KEYS), help="the key file (%s)" % KEYS)
    ap.add_argument("--sdl", type=int, choices=[0, 1], help="build with SDL2's window (1) or not (0); by default "
                                                             "with it when pkg-config finds SDL2")
    ap.add_argument("--cutscene", help="play this cutscene at boot (an id or a CUTSCENE_* constant), not a level")
    ap.add_argument("--language", help="with --cutscene: its text's language (en, fr, de, es, it, nl or 0-5)")
    ap.add_argument("--no-build", action="store_true", help="run the executable as it is")
    ap.add_argument("--json", action="store_true", help="print one JSON report (docs/editors.md)")
    args = ap.parse_args()
    os.chdir(ROOT)
    headless = args.headless or bool(args.out or args.screens)
    if args.cutscene is not None:
        return run_cutscene(args, headless)
    if args.level is None:
        ap.error("give a LEVEL, or --cutscene")
    level, room, constant, entry = resolve(args.level, args.room)
    if not args.no_build:
        build(args.sdl)
    if not EXE.exists():
        D.error("no %s: build it (make PLATFORM=pc), or leave out --no-build" % EXE, code="preview-build")
    cmd = [str(EXE), "--keys", args.keys, "--level", str(level), "--room", str(EXTRA_ROOMS.get(room, room))]
    pictures = []
    tmp = None
    if headless:
        frames = args.frames or PREVIEW_FRAME
        screens = args.screens or tempfile.mkdtemp(prefix="preview-")
        tmp = None if args.screens else screens
        os.makedirs(screens, exist_ok=True)
        cmd += ["--headless", "--quiet", "--frames", str(frames), "--checkpoint-every", str(args.every or frames),
                "--screens", screens]
    elif args.frames:
        cmd += ["--frames", str(args.frames)]
    print("preview: %s, room %s (%s): %s" % (constant, room, entry, " ".join(cmd)))
    sys.stdout.flush()
    r = subprocess.run(cmd)
    if r.returncode:
        D.error("%s exited with status %d" % (EXE, r.returncode), code="preview-run")
    if headless:
        pictures = sorted(str(p) for p in Path(screens).glob("*.png"))
        if not pictures:
            D.error("no picture was written (did the run end before frame %d?)" % (args.every or frames),
                    code="preview-run")
        last = pictures[-1]
        if args.out:
            shutil.copyfile(last, args.out)
            last = args.out
            print("preview: %s" % args.out)
        if tmp:
            shutil.rmtree(tmp)
            pictures = [last] if args.out else []
    RESULT.update(level=level, constant=constant, room=room, entry=entry, command=cmd, pictures=pictures)
    return 0


def run_cutscene(args, headless):
    from crashdata.common.game import Game
    from crashdata.cutscenes import edit

    idx, constant, lang_index, lang = resolve_cutscene(args.cutscene, args.language)
    steps = edit.cutscene_steps(Game("xs", "."), idx, lang)
    if not args.no_build:
        build(args.sdl)
    if not EXE.exists():
        D.error("no %s: build it (make PLATFORM=pc), or leave out --no-build" % EXE, code="preview-build")
    cmd = [str(EXE), "--cutscene", str(idx), "--language", str(lang_index)]
    pictures, tmp = [], None
    if headless:
        every = args.every or 60
        # a slide that ignores the keys waits its own frames
        frames = args.frames or every * (len(steps) + 1) + sum(w for _, _, k, w, b in steps if not b and k == 0)
        screens = args.screens or tempfile.mkdtemp(prefix="preview-")
        tmp = None if args.screens else screens
        os.makedirs(screens, exist_ok=True)
        keys = os.path.join(screens, "cutscene.keys")
        cutscene_keys(keys, every, len(steps) + 1)
        cmd += ["--keys", keys, "--headless", "--quiet", "--frames", str(frames), "--checkpoint-every", str(every),
                "--screens", screens]
    elif args.frames:
        cmd += ["--frames", str(args.frames)]
    print("preview: %s (cutscene %d) in %s, %d screens: %s" % (constant, idx, lang, len(steps), " ".join(cmd)))
    sys.stdout.flush()
    r = subprocess.run(cmd)
    if r.returncode:
        D.error("%s exited with status %d" % (EXE, r.returncode), code="preview-run")
    if headless:
        pictures = sorted(str(p) for p in Path(screens).glob("*.png"))
        if not pictures:
            D.error("no picture was written", code="preview-run")
        if args.out:
            shutil.copyfile(pictures[-1], args.out)
        if tmp:
            shutil.rmtree(tmp)
            pictures = [args.out] if args.out else []
    RESULT.update(cutscene=idx, constant=constant, language=lang, steps=len(steps), command=cmd,
                  pictures=pictures)
    return 0


# What run() previewed, for --json's report.
RESULT = {}


def main():
    """run(), or with --json one JSON report (docs/editors.md, "Errors"): its
    diagnostics, what it printed, and "preview": the level and room, the
    command and the pictures."""
    if "--json" not in sys.argv[1:]:
        return run()
    sys.argv.remove("--json")
    status = 0
    with D.collect() as diags, D.capture_fd_stdout() as printed:
        try:
            status = run() or 0
        except SystemExit as e:
            diags.extend(D.from_exit(e))
            status = e.code if isinstance(e.code, int) else 1
    failed = bool(status) or any(d.severity == D.ERROR for d in diags)
    report = D.report("preview", D.relativize(diags), printed.text, ok=not failed)
    report["preview"] = RESULT or None
    print(json.dumps(report, indent=1))
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
