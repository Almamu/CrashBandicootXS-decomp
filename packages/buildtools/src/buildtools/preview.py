"""tools/preview.py: play a level's room in the PC port (#1167, docs/editors.md "Preview").

    tools/preview.py LEVEL [ROOM] [--headless] [--frames N] [--every N]
                     [--screens DIR] [--out PNG] [--keys FILE] [--sdl 0|1]
                     [--no-build] [--json]

Builds the host executable (`make PLATFORM=pc`, build/pc/crashbandicootxs_pc,
docs/platforms.md) from this tree's data, then runs it with a key file
that goes through the logos and menus to the level select
(platform/pc/preview.keys), where `--level` and `--room` (level_pick.c)
make the game start LEVEL in its room ROOM.

LEVEL is gLevelTable's index, a level's constant (LEVEL_JUNGLE_JAM or
JUNGLE_JAM), its name ("jungle jam") or folder (jungle_jam), or a room
folder (snow_crash/part2: its level, and the room). ROOM is the index in
the level's "rooms" (the rooms it plays in order; 0 by default) or one of
them by folder. A level's bonus and gem-path rooms aren't in "rooms":
they are entered from the level, not started.

In a window (an executable built with SDL2) the game runs paced, and the
keyboard and gamepad play it from the level select on. --headless (or a
build without SDL2) runs it as fast as it goes: it stops at --frames
(the level's first picture by default, PREVIEW_FRAME), writes the
pictures of every --every'th frame to --screens DIR (FRAME.png) and the
last one to --out. --json prints a report: the level and room, the
command and the pictures.

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


def room_entries(lv):
    return [r if isinstance(r, str) else "stage %s" % r.get("stage", r) for r in lv["rooms"]]


def resolve(level, room=None):
    """(level index, room index, the level's constant, the room) of the
    arguments, or a DataError."""
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
        if idx is None:
            extra = [lv["constant"] for lv in lvs if level in (lv.get("bonus"), lv.get("gem_path"))]
            hint = (" (%s's bonus or gem-path room: those are entered from the level, not started)" % extra[0]
                    if extra else "")
            D.error("no level %s%s" % (level, hint), where, "/levels", code="preview-level")
    lv = lvs[idx]
    if room is not None:
        if re.fullmatch(r"\d+", str(room)):
            ridx = int(room)
        elif room in lv["rooms"]:
            ridx = lv["rooms"].index(room)
        else:
            D.error("%s plays no room %s (its rooms: %s)" % (lv["constant"], room, ", ".join(room_entries(lv))),
                    where, "/levels/%d/rooms" % idx, code="preview-room")
    ridx = ridx or 0
    if not 0 <= ridx < len(lv["rooms"]):
        D.error("%s has %d rooms (0-%d): %s" % (lv["constant"], len(lv["rooms"]), len(lv["rooms"]) - 1,
                                                ", ".join(room_entries(lv))),
                where, "/levels/%d/rooms" % idx, code="preview-room")
    return idx, ridx, lv["constant"], room_entries(lv)[ridx]


def build(sdl):
    cmd = ["make", "-j%d" % (os.cpu_count() or 1), "PLATFORM=pc"] + ([] if sdl is None else ["HOST_SDL=%d" % sdl])
    sys.stdout.flush()
    r = subprocess.run(cmd, stdout=sys.stderr)
    if r.returncode:
        D.error("the host build failed (%s)" % " ".join(cmd), code="preview-build")


def run():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("level", help="the level: its index, constant, name, folder, or a room folder")
    ap.add_argument("room", nargs="?", help="the room: its index in the level's rooms, or its folder")
    ap.add_argument("--headless", action="store_true", help="no window: run, write the pictures and stop")
    ap.add_argument("--frames", type=int, help="stop after N game frames (headless: %d)" % PREVIEW_FRAME)
    ap.add_argument("--every", type=int, help="headless: a picture every N frames (only the last by default)")
    ap.add_argument("--screens", help="headless: write the pictures to DIR/FRAME.png")
    ap.add_argument("--out", help="headless: copy the last picture to this PNG")
    ap.add_argument("--keys", default=str(KEYS), help="the key file (%s)" % KEYS)
    ap.add_argument("--sdl", type=int, choices=[0, 1], help="build with SDL2's window (1) or not (0); by default "
                                                             "with it when pkg-config finds SDL2")
    ap.add_argument("--no-build", action="store_true", help="run the executable as it is")
    ap.add_argument("--json", action="store_true", help="print one JSON report (docs/editors.md)")
    args = ap.parse_args()
    os.chdir(ROOT)
    level, room, constant, entry = resolve(args.level, args.room)
    if not args.no_build:
        build(args.sdl)
    if not EXE.exists():
        D.error("no %s: build it (make PLATFORM=pc), or leave out --no-build" % EXE, code="preview-build")
    headless = args.headless or bool(args.out or args.screens)
    cmd = [str(EXE), "--keys", args.keys, "--level", str(level), "--room", str(room)]
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
    print("preview: %s, room %d (%s): %s" % (constant, room, entry, " ".join(cmd)))
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
