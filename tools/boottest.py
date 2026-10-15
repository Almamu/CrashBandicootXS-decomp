#!/usr/bin/env python3
"""Boots a ROM headless in mGBA and plays scripted input, recording what
the game does, so two builds (the matching ROM and a shifted one, see
docs/shifting.md) can be compared run against run.

    tools/boottest.py run ROM.gba ROM.elf [--script NAME ... | --keys FILE --frames N]
                      [--dump DIR] [--out LOG.json]
    tools/boottest.py host EXE --keys FILE --frames N [--level N [--room R]] [--dump DIR] [--screens DIR]
                      [--out LOG.json]
    tools/boottest.py screens ROM.gba --keys FILE --frames N [--elf ROM.elf --level N [--room R]]
                      [--screens DIR] [--save FILE] [--out LOG.json]
    tools/boottest.py compare A.json B.json
    tools/boottest.py diff-screens REFERENCE_DIR DIR

mGBA's SDL frontend (`mgba`) runs with SDL's dummy video and audio
drivers and its GDB stub, which this drives with the GDB remote protocol.
The input goes in through a breakpoint in UpdateKeys, right after it reads
REG_KEYINPUT: the script sets the key register there, once a frame. A
level is picked by setting LevelSelectLoop's return value, so a script
can play any level without navigating the level select. The log has, for
each script (each run boots the ROM from a fresh copy, so with no save):

- the frame of every PlaySong (the song ID), LoadRoom (the room's
  symbol), InitActorCategory (the category: the polar, jetpack and boss
  stages), PlayCutscene, RunSaveMenu and EEPROMWrite;
- every CHECKPOINT_EVERY frames, a hash of palette RAM, VRAM and OAM,
  the picture on the screen, which holds no ROM addresses, so it must be
  the same in both builds, whether the GAX2 mixer (GaxMixerPlay) ran
  since the checkpoint before, and the picture's hash (picture_hash: the
  same bytes, with the OAM bits the hardware ignores cleared);
- the hash of the save file at the end, and how the run ended.

ROM.elf is the ELF the ROM was linked from: it gives the symbols'
addresses. `compare` exits 1 if two logs differ.

A key file (--keys, with --frames) is a fixed script, the same on mGBA
and on the pc platform's host executable (platform/pc, #1093): a line
`FRAME KEYS` holds KEYS (`-`, a number, or names joined by `+`: A B
SELECT START RIGHT LEFT UP DOWN R L) from that frame on. `host` runs the
host executable with it and logs its checkpoints the same way (it has no
events), so `compare` of a `run` and a `host` log with the same key file
checks the host's palette RAM, VRAM and OAM against mGBA's, by their
picture hashes: the game leaves some OAM bits it doesn't use as whatever
was on the stack, which differs between the two. --dump writes
each checkpoint's bytes (palette RAM, VRAM, OAM) to DIR/FRAME.bin, to find
where two runs differ.

The host's checkpoints also have the hash of its PPU's picture (a 5th
field: the SHA-1 of the RGB bytes of the picture finished last,
platform/pc/src/ppu.c), and --screens DIR writes that picture as
DIR/FRAME.png. `screens` gets mGBA's pictures for a key file the same way:
it builds platform/pc/tools/mgba_screens.c against libmgba (the mGBA
whose `mgba` is on the PATH, or --mgba PREFIX; mods.yml's 0.10.5 has it)
and plays the key file there, frame N being the N-th read of REG_KEYINPUT
as in `run`. Its log has only the screen hashes; `compare` checks those
of a `host` log against them, and `diff-screens` compares the PNGs of two
directories pixel by pixel (those in the first). --level and --room pick
the level (and its first room) at the first level select, as the level
scripts do: on the host with its own --level and --room, on libmgba with
breakpoints at the addresses ROM.elf gives. --save keeps mGBA's EEPROM in
a file, the image the host's --save writes.
"""
import argparse
import hashlib
import json
import os
import shutil
import socket
import struct
import subprocess
import sys
import tempfile
import time

KEY_A, KEY_B, KEY_SELECT, KEY_START = 0x1, 0x2, 0x4, 0x8
KEY_RIGHT, KEY_LEFT, KEY_UP, KEY_DOWN, KEY_R, KEY_L = 0x10, 0x20, 0x40, 0x80, 0x100, 0x200

# UpdateKeys+0xA: r1 holds the keys read from REG_KEYINPUT, active high.
KEYS_HOOK = ("UpdateKeys", 0xA, 1)
CHECKPOINT_EVERY = 60
GDB_PORT = 2345

SONG_CUTSCENES_SPOOKY = 0xE
SONG_WARP_ROOM = 0x10


# ---------------------------------------------------------------------------
# Scripts: generators that yield the keys for each frame. `st.events` is
# the log so far, `st.frame` the frame, and `st.level` the level the next
# LevelSelectLoop returns (None: the one picked), `st.room` the room
# index the next SelectRoom starts from (None: the level's first).
# ---------------------------------------------------------------------------

def wait(n, keys=0):
    for _ in range(n):
        yield keys


def until_song(st, song, limit=3000):
    """Mashes Start and A until PlaySong(song)."""
    start = len(st.events)
    for i in range(limit):
        if any(e[1] == "PlaySong" and e[2] == song for e in st.events[start:]):
            return
        yield (KEY_START, 0, KEY_A, 0)[(i // 3) % 4]
    raise RuntimeError(f"no PlaySong({song:#x}) in {limit} frames")


def play(n):
    """Some generic play: run right, jump, attack, steer up and down (the
    polar and jetpack stages), fire."""
    pattern = [(KEY_RIGHT, 24), (KEY_RIGHT | KEY_A, 6), (KEY_RIGHT | KEY_B, 4),
               (KEY_UP | KEY_B, 12), (KEY_DOWN | KEY_A, 12), (KEY_LEFT, 6)]
    i = 0
    while n > 0:
        keys, k = pattern[i % len(pattern)]
        for _ in range(min(k, n)):
            yield keys
        n -= k
        i += 1


def script_level(level, room=None, frames=900):
    def script(st):
        yield from until_song(st, SONG_WARP_ROOM)  # the level select
        yield from wait(90)
        st.level = level
        st.room = room
        yield from wait(4, KEY_A)
        yield from wait(60)
        yield from play(frames)
    return script


def script_cutscene(st):
    """The intro cutscene (a new game's PlayCutscene(2)), without skipping."""
    yield from until_song(st, SONG_CUTSCENES_SPOOKY)
    yield from wait(1500)


def script_save(st):
    """Start at the level select opens the save menu: Save, slot 1, then
    back to the level select and into the first level."""
    yield from until_song(st, SONG_WARP_ROOM)
    yield from wait(90)
    for keys, n in [(KEY_START, 4), (0, 120), (KEY_DOWN, 4), (0, 20), (KEY_DOWN, 4), (0, 20),
                    (KEY_A, 4), (0, 60), (KEY_A, 4), (0, 240), (KEY_A, 4), (0, 120),
                    (KEY_B, 4), (0, 120), (KEY_B, 4), (0, 120), (KEY_START, 4), (0, 200)]:
        yield from wait(n, keys)
    st.level = 0
    yield from wait(4, KEY_A)
    yield from wait(60)
    yield from play(300)


# The levels: jungle jam (on foot), shipwrecked (underwater), rocket
# racket and blimp bonanza (jetpack stages), ruined, ace of space, the
# five bosses (n. gin's is a jetpack stage); and the polar stages in the
# middle of frostbite cavern, snow crash and snow job (their room 1).
LEVELS = [0, 1, 6, 9, 11, 14, 20, 21, 22, 23, 24]
STAGES = [3, 5, 10]
SCRIPTS = {"cutscene": script_cutscene, "save": script_save}
SCRIPTS.update({f"level{n}": script_level(n) for n in LEVELS})
SCRIPTS.update({f"level{n}_room1": script_level(n, 1) for n in STAGES})
DEFAULT_SCRIPTS = (["cutscene", "save"] + [f"level{n}" for n in LEVELS]
                   + [f"level{n}_room1" for n in STAGES])

KEY_NAMES = {"A": KEY_A, "B": KEY_B, "SELECT": KEY_SELECT, "START": KEY_START, "RIGHT": KEY_RIGHT,
             "LEFT": KEY_LEFT, "UP": KEY_UP, "DOWN": KEY_DOWN, "R": KEY_R, "L": KEY_L}


def read_keys(path):
    """A key file's [(frame, keys)], as platform/pc/src/input.c reads it."""
    runs = []
    for n, line in enumerate(open(path), 1):
        f = line.split("#")[0].split()
        if not f:
            continue
        if len(f) != 2:
            raise SystemExit(f"{path}:{n}: expected `FRAME KEYS`")
        if f[1] == "-":
            keys = 0
        elif f[1][0].isdigit():
            keys = int(f[1], 0) & 0x3FF
        else:
            keys = 0
            for k in f[1].split("+"):
                keys |= KEY_NAMES[k.upper()]
        if runs and int(f[0]) <= runs[-1][0]:
            raise SystemExit(f"{path}:{n}: frames out of order")
        runs.append((int(f[0]), keys))
    return runs


def script_keys(runs, frames):
    """A key file as a script: frames 1 to `frames`."""
    def script(st):
        k, i = 0, 0
        for frame in range(1, frames + 1):
            while i < len(runs) and runs[i][0] <= frame:
                k = runs[i][1]
                i += 1
            yield k
    return script


# ---------------------------------------------------------------------------
# The emulator
# ---------------------------------------------------------------------------

class Gdb:
    """A minimal GDB remote-protocol client."""

    def __init__(self, port):
        for _ in range(200):
            try:
                self.s = socket.create_connection(("127.0.0.1", port), timeout=60)
                # no Nagle: the ack and the next command are two small writes
                self.s.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
                break
            except OSError:
                time.sleep(0.05)
        else:
            raise RuntimeError("can't connect to mGBA's GDB stub")
        self.buf = b""

    def _recv_packet(self):
        while True:
            i = self.buf.find(b"$")
            j = self.buf.find(b"#", i + 1) if i >= 0 else -1
            if i >= 0 and j >= 0 and len(self.buf) >= j + 3:
                data = self.buf[i + 1:j]
                self.buf = self.buf[j + 3:]
                self.s.sendall(b"+")
                return data.decode("latin-1")
            chunk = self.s.recv(65536)
            if not chunk:
                raise RuntimeError("mGBA closed the GDB connection")
            self.buf += chunk

    def cmd(self, payload, reply=True):
        data = payload.encode("latin-1")
        self.s.sendall(b"$" + data + b"#" + f"{sum(data) & 0xFF:02x}".encode())
        return self._recv_packet() if reply else None

    def read(self, addr, n):
        out = bytearray()
        while n:
            k = min(n, 0x200)  # mGBA's stub answers at most this much
            r = self.cmd(f"m{addr:x},{k:x}")
            if r.startswith("E"):
                raise RuntimeError(f"can't read {addr:#x}")
            out += bytes.fromhex(r)
            addr, n = addr + k, n - k
        return bytes(out)

    def reg(self, n):
        return struct.unpack("<I", bytes.fromhex(self.cmd(f"p{n:x}")))[0]

    def set_reg(self, n, v):
        self.cmd(f"P{n:x}={struct.pack('<I', v & 0xFFFFFFFF).hex()}")

    def write(self, addr, data):
        if self.cmd(f"M{addr:x},{len(data):x}:{data.hex()}") != "OK":
            raise RuntimeError(f"can't write {addr:#x}")

    def bp(self, addr, on=True):
        r = self.cmd(f"{'Z' if on else 'z'}0,{addr:x},2")
        if r != "OK":
            raise RuntimeError(f"breakpoint at {addr:#x}: {r!r}")


def read_elf(elf):
    """({name: address} of the ELF's symbols, Thumb bit cleared), and a
    function reading ROM bytes by address."""
    d = open(elf, "rb").read()
    phoff, shoff = struct.unpack_from("<II", d, 0x1C)
    phentsize, phnum, shentsize, shnum = struct.unpack_from("<HHHH", d, 0x2A)
    secs = [struct.unpack_from("<10I", d, shoff + i * shentsize) for i in range(shnum)]
    syms = {}
    for s in secs:
        if s[1] != 2:
            continue
        strs = secs[s[6]]
        for o in range(s[4], s[4] + s[5], 16):
            st_name, value, _, _, _, shndx = struct.unpack_from("<IIIBBH", d, o)
            if st_name and shndx:
                so = strs[4] + st_name
                syms.setdefault(d[so:d.index(b"\0", so)].decode(), value & ~1)
    segs = [struct.unpack_from("<8I", d, phoff + i * phentsize) for i in range(phnum)]

    def rom(addr, n):
        for p_type, off, vaddr, _, filesz, *_ in segs:
            if p_type == 1 and vaddr <= addr < vaddr + filesz:
                return d[off + addr - vaddr:off + addr - vaddr + n]
        raise KeyError(f"{addr:#x}")
    return syms, rom


def call_return(rom, func, callee, limit=0x400):
    """The address after the Thumb BL from `func` to `callee`."""
    for a in range(func, func + limit, 2):
        hi, lo = struct.unpack("<HH", rom(a, 4))
        if hi & 0xF800 == 0xF000 and lo & 0xF800 == 0xF800:
            off = ((hi & 0x7FF) << 12 | (lo & 0x7FF) << 1)
            if off & 0x400000:
                off -= 0x800000
            if a + 4 + off == callee:
                return a + 4
    raise RuntimeError(f"no call to {callee:#x} in the function at {func:#x}")


class State:
    def __init__(self):
        self.frame = 0
        self.events = []
        self.level = None
        self.room = None
        self.actions = []  # functions of the Gdb, run before the next frame's keys


def picture_hash(data):
    """The hash of a checkpoint's bytes (palette RAM, VRAM, OAM) with the
    OAM bits the display ignores cleared: a hidden entry's attributes
    (but not its affine parameter, part of a matrix), and a regular
    entry's matrix bits (attr1 bits 9-11), which DrawSpritePieces leaves
    as whatever its stack held. platform/pc/src/input.c hashes the same."""
    oam = bytearray(data[-0x400:])
    for i in range(0, 0x400, 8):
        attr0, attr1 = struct.unpack_from("<HH", oam, i)
        mode = (attr0 >> 8) & 3
        if mode == 2:
            struct.pack_into("<HHH", oam, i, 0x200, 0, 0)
        elif mode == 0:
            struct.pack_into("<H", oam, i + 2, attr1 & ~0xE00)
    return hashlib.sha1(data[:-0x400] + bytes(oam)).hexdigest()[:16]


def run_script(rom_path, syms, rom, script, timeout, extra_hooks=None, dump=None,
               every=CHECKPOINT_EVERY):
    """Plays the script in a fresh mGBA and returns its log. extra_hooks:
    {address: function(gdb) -> event or None}, more breakpoints whose
    events go into the log (mods/sample/boottest.py's). dump: a directory
    for each checkpoint's bytes; every: the frames between checkpoints."""
    by_addr = {}
    for name, a in syms.items():
        by_addr.setdefault(a, name)
    keys_bp = syms[KEYS_HOOK[0]] + KEYS_HOOK[1]
    level_bp = call_return(rom, syms["RunLevelSelect"], syms["LevelSelectLoop"])
    room_bp = syms["SelectRoom"]  # LevelProgress::SelectRoom; roomIndex is at +4
    hooks = {
        syms["LoadRoom"]: lambda g: ["LoadRoom", by_addr.get(g.reg(1), f"{g.reg(1):#x}")],
        syms["InitActorCategory"]: lambda g: ["InitActorCategory", g.reg(0)],
        syms["PlaySong"]: lambda g: ["PlaySong", g.reg(1)],
        syms["PlayCutscene"]: lambda g: ["PlayCutscene", g.reg(1)],
        syms["RunSaveMenu"]: lambda g: ["RunSaveMenu", g.reg(0)],
        syms["EEPROMWrite"]: lambda g: ["EEPROMWrite", g.reg(0)],
    }
    hooks.update(extra_hooks or {})
    mixer = syms["GaxMixerPlay"]

    st = State()
    log = {"events": st.events, "checkpoints": [], "save": None, "end": None}
    work = tempfile.mkdtemp(prefix="boottest-")
    rom_copy = os.path.join(work, "rom.gba")
    shutil.copyfile(rom_path, rom_copy)
    env = dict(os.environ, SDL_VIDEODRIVER="dummy", SDL_AUDIODRIVER="dummy")
    emu = subprocess.Popen(["mgba", "-g", rom_copy], env=env, cwd=work, stdout=subprocess.DEVNULL,
                           stderr=subprocess.DEVNULL, start_new_session=True)
    start = time.time()
    try:
        g = Gdb(GDB_PORT)
        g.cmd("?")
        for a in [keys_bp, level_bp, room_bp, mixer] + list(hooks):
            g.bp(a)
        keys = script(st)
        mixer_ran = False
        stops = {}
        log["stops"] = stops
        while True:
            if time.time() - start > timeout:
                log["end"] = f"timeout at frame {st.frame}"
                break
            r = g.cmd("c")
            if not r.startswith(("T05", "S05")):
                log["end"] = f"stopped at frame {st.frame}: {r}"
                break
            pc = g.reg(15) & ~1
            stops[pc] = stops.get(pc, 0) + 1
            if pc == mixer:
                mixer_ran = True
                g.bp(mixer, False)  # once a checkpoint is enough
            elif pc in hooks:
                e = hooks[pc](g)
                if e is not None:
                    st.events.append([st.frame] + e)
            elif pc == level_bp:
                if st.level is not None:
                    st.events.append([st.frame, "SetLevel", st.level])
                    g.set_reg(0, st.level)
                    st.level = None
            elif pc == room_bp:
                if st.room is not None:
                    st.events.append([st.frame, "SetRoom", st.room])
                    g.write(g.reg(0) + 4, struct.pack("<i", st.room))
                    st.room = None
            elif pc == keys_bp:
                st.frame += 1
                while st.actions:
                    st.actions.pop(0)(g)
                try:
                    k = next(keys)
                except StopIteration:
                    log["end"] = f"done at frame {st.frame}"
                    break
                g.set_reg(KEYS_HOOK[2], k)
                if st.frame % every == 0:
                    data = b"".join(g.read(base, n) for base, n in
                                    ((0x05000000, 0x400), (0x06000000, 0x18000), (0x07000000, 0x400)))
                    if dump:
                        with open(os.path.join(dump, f"{st.frame:05}.bin"), "wb") as f:
                            f.write(data)
                    log["checkpoints"].append([st.frame, hashlib.sha1(data).hexdigest()[:16], mixer_ran,
                                               picture_hash(data)])
                    mixer_ran = False
                    g.bp(mixer)
            else:
                log["end"] = f"unexpected stop at {pc:#x}, frame {st.frame}"
                break
        try:
            g.cmd("D")
        except (OSError, RuntimeError):
            pass
    except socket.timeout:
        log["end"] = f"no breakpoint for 60 s after frame {st.frame}: the game hung or crashed"
    except (RuntimeError, OSError) as e:
        log["end"] = f"error at frame {st.frame}: {e}"
    finally:
        emu.kill()
        emu.wait()
        sav = os.path.join(work, "rom.sav")
        if os.path.exists(sav):
            data = open(sav, "rb").read()
            log["save"] = hashlib.sha1(data).hexdigest()[:16]
            log["save_size"] = len(data)
        shutil.rmtree(work, ignore_errors=True)
    log["seconds"] = round(time.time() - start, 1)
    return log


def run_host(exe, keys, frames, timeout, dump=None, every=CHECKPOINT_EVERY, screens=None, level=None,
             room=None):
    """Plays a key file on the pc platform's host executable (headless)
    and returns its log: the checkpoints (with no mixer flag, None, and
    the screen's hash) and how it ended; no events (None) and no save."""
    cmd = [exe, "--headless", "--keys", keys, "--frames", str(frames), "--checkpoint-every", str(every)]
    if level is not None:
        cmd += ["--level", str(level)] + (["--room", str(room)] if room is not None else [])
    if dump:
        cmd += ["--dump", dump]
    if screens:
        cmd += ["--screens", screens]
    start = time.time()
    log = {"events": None, "checkpoints": [], "save": None, "end": None}
    try:
        p = subprocess.run(cmd, capture_output=True, text=True, timeout=timeout)
        out, code = p.stdout, p.returncode
    except subprocess.TimeoutExpired as e:
        out, code = e.stdout.decode() if e.stdout else "", None
    for line in out.splitlines():
        f = line.split()
        if f[:1] == ["checkpoint"]:
            log["checkpoints"].append([int(f[1]), f[2], None, f[3]] + f[4:5])
        elif f[:1] == ["end"]:
            log["end"] = " ".join(f[1:5])  # "done at frame N", as run's
    if code is None:
        log["end"] = "timeout"
    elif code != 0 or log["end"] is None:
        log["end"] = f"exit status {code}: {log['end']}"
    log["seconds"] = round(time.time() - start, 1)
    return log


def mgba_prefix(given):
    """The install prefix of the mGBA with libmgba's headers and library."""
    mgba = shutil.which("mgba")
    prefix = given or (os.path.dirname(os.path.dirname(os.path.realpath(mgba))) if mgba else None)
    if not prefix or not os.path.exists(os.path.join(prefix, "include", "mgba", "core", "core.h")):
        raise SystemExit(f"no libmgba headers under {prefix!r}: give --mgba PREFIX (an mGBA install)")
    return prefix


def run_screens(rom, keys, frames, timeout, prefix, every=CHECKPOINT_EVERY, screens=None, elf=None,
                level=None, room=None, save=None):
    """Plays a key file on libmgba with platform/pc/tools/mgba_screens.c
    and returns a log of its pictures' hashes (no other fields)."""
    extra = ["--save", save] if save else []
    if level is not None:
        if not elf:
            raise SystemExit("--level needs --elf (the ROM's ELF, for the addresses)")
        syms, romdata = read_elf(elf)
        extra += ["--level", hex(call_return(romdata, syms["RunLevelSelect"], syms["LevelSelectLoop"])), str(level)]
        if room is not None:
            extra += ["--room", hex(syms["SelectRoom"]), str(room)]
    here = os.path.dirname(os.path.abspath(__file__))
    pc = os.path.join(here, "..", "platform", "pc")
    lib = next((os.path.join(prefix, d) for d in ("lib", "lib64")
                if os.path.exists(os.path.join(prefix, d, "libmgba.so"))), os.path.join(prefix, "lib"))
    work = tempfile.mkdtemp(prefix="boottest-")
    start = time.time()
    log = {"events": None, "checkpoints": [], "save": None, "end": None}
    try:
        exe = os.path.join(work, "mgba_screens")
        subprocess.run([os.environ.get("CC", "cc"), "-O2", "-o", exe,
                        os.path.join(pc, "tools", "mgba_screens.c"), os.path.join(pc, "src", "sha1.c"),
                        os.path.join(pc, "src", "png.c"), "-I", os.path.join(prefix, "include"),
                        "-I", os.path.join(pc, "include"), "-I", os.path.join(pc, "src"),
                        "-I", os.path.join(here, "..", "include"), "-L", lib, "-lmgba",
                        f"-Wl,-rpath,{lib}"], check=True)
        with open(os.path.join(work, "keys.txt"), "w") as f:
            for frame, k in read_keys(keys):
                f.write(f"{frame} {k}\n")
        try:
            p = subprocess.run([exe, rom, os.path.join(work, "keys.txt"), str(frames), str(every),
                                screens or ""] + extra, capture_output=True, text=True, timeout=timeout)
            out, code = p.stdout, p.returncode
        except subprocess.TimeoutExpired as e:
            out, code = e.stdout.decode() if e.stdout else "", None
    finally:
        shutil.rmtree(work, ignore_errors=True)
    for line in out.splitlines():
        f = line.split()
        if f[:1] == ["screen"]:
            log["checkpoints"].append([int(f[1]), None, None, None, f[2]])
        elif f[:1] == ["end"]:
            log["end"] = " ".join(f[1:5])
    if code is None:
        log["end"] = "timeout"
    elif code != 0 or log["end"] is None:
        log["end"] = f"exit status {code}: {log['end']}"
    log["seconds"] = round(time.time() - start, 1)
    return log


def checkpoint_fields(c, fields):
    """A checkpoint's frame and the fields `fields` (None where missing)."""
    return [c[0]] + [c[i] if len(c) > i else None for i in fields]


def compare(a, b):
    """The differences between two logs, as lines. A host log (no events,
    no mixer flags, no save) is compared by its checkpoints' frames, the
    picture and screen hashes both logs have (a `screens` log has only
    the screens) and how it ended."""
    out = []
    for name in sorted(set(a) | set(b)):
        if name not in a or name not in b:
            out.append(f"{name}: only in one log")
            continue
        x, y = a[name], b[name]
        if x["events"] is None or y["events"] is None:
            # the fields both have: the picture hash (3), the screen's (4)
            fields = [i for i in (3, 4)
                      if any(len(c) > i and c[i] is not None for c in x["checkpoints"])
                      and any(len(c) > i and c[i] is not None for c in y["checkpoints"])]
            x = dict(x, checkpoints=[checkpoint_fields(c, fields) for c in x["checkpoints"]], save=None)
            y = dict(y, checkpoints=[checkpoint_fields(c, fields) for c in y["checkpoints"]], save=None)
        elif x["events"] != y["events"]:
            diff = next(((p, q) for p, q in zip(x["events"], y["events"]) if p != q), None)
            out.append(f"{name}: events differ: " + (f"{diff[0]} vs {diff[1]}" if diff else
                                                     f"{len(x['events'])} vs {len(y['events'])}"))
        diff = next(((p, q) for p, q in zip(x["checkpoints"], y["checkpoints"]) if p != q), None)
        if diff or len(x["checkpoints"]) != len(y["checkpoints"]):
            out.append(f"{name}: checkpoints differ: " + (f"{diff[0]} vs {diff[1]}" if diff else "count"))
        for k in ("save", "end"):
            if x[k] != y[k]:
                out.append(f"{name}: {k}: {x[k]!r} vs {y[k]!r}")
    return out


def diff_screens(ref, other):
    """The PNGs of `ref` against the same names in `other`, pixel by
    pixel: the differences as lines (how many pixels, where), and how many
    pictures were compared."""
    from PIL import Image, ImageChops
    out = []
    names = sorted(n for n in os.listdir(ref) if n.endswith(".png"))
    for n in names:
        p = os.path.join(other, n)
        if not os.path.exists(p):
            out.append(f"{n}: missing")
            continue
        x = Image.open(os.path.join(ref, n)).convert("RGB")
        y = Image.open(p).convert("RGB")
        if x.size != y.size:
            out.append(f"{n}: {x.size} vs {y.size}")
            continue
        d = ImageChops.difference(x, y).convert("L").point(lambda v: 255 if v else 0)
        box = d.getbbox()
        if box:
            out.append(f"{n}: {d.histogram()[255]} pixels differ, in {box}")
    return out, len(names)


def write_log(logs, path):
    """The logs as JSON, a checkpoint to a line."""
    lines = ["{"]
    for i, (name, log) in enumerate(logs.items()):
        lines.append(f" {json.dumps(name)}: {{")
        for k, v in log.items():
            if k != "checkpoints":
                lines.append(f"  {json.dumps(k)}: {json.dumps(v)},")
        rows = [json.dumps(c, separators=(",", ":")) for c in log["checkpoints"]]
        lines.append('  "checkpoints": [')
        lines += [f"   {r}," for r in rows[:-1]] + [f"   {r}" for r in rows[-1:]]
        lines.append("  ]")
        lines.append(" }" + ("," if i + 1 < len(logs) else ""))
    lines.append("}")
    with open(path, "w") as f:
        f.write("\n".join(lines) + "\n")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    r = sub.add_parser("run", help="boot a ROM and run scripts")
    r.add_argument("rom")
    r.add_argument("elf")
    r.add_argument("--script", action="append", choices=sorted(SCRIPTS),
                   help=f"default: {' '.join(DEFAULT_SCRIPTS)}")
    r.add_argument("--keys", help="a key file, the script (with --frames)")
    r.add_argument("--frames", type=int, help="the key file's frames")
    r.add_argument("--dump", help="write each checkpoint's bytes to this directory")
    r.add_argument("--every", type=int, default=CHECKPOINT_EVERY, help="frames between checkpoints")
    r.add_argument("--out")
    r.add_argument("--timeout", type=float, default=600, help="seconds per script")
    h = sub.add_parser("host", help="play a key file on the pc platform's host executable")
    h.add_argument("exe")
    h.add_argument("--keys", required=True)
    h.add_argument("--frames", type=int, required=True)
    h.add_argument("--dump")
    h.add_argument("--screens", help="write each checkpoint's picture to this directory")
    h.add_argument("--level", type=int, help="the level the first level select picks")
    h.add_argument("--room", type=int, help="with --level: the room it starts in")
    h.add_argument("--every", type=int, default=CHECKPOINT_EVERY)
    h.add_argument("--out")
    h.add_argument("--timeout", type=float, default=600)
    sc = sub.add_parser("screens", help="play a key file on libmgba, logging (and writing) its pictures")
    sc.add_argument("rom")
    sc.add_argument("--keys", required=True)
    sc.add_argument("--frames", type=int, required=True)
    sc.add_argument("--screens", help="write each checkpoint's picture to this directory")
    sc.add_argument("--elf", help="the ROM's ELF (for --level's and --room's addresses)")
    sc.add_argument("--level", type=int, help="the level the first level select picks")
    sc.add_argument("--room", type=int, help="with --level: the room it starts in")
    sc.add_argument("--save", help="keep mGBA's EEPROM in this file")
    sc.add_argument("--every", type=int, default=CHECKPOINT_EVERY)
    sc.add_argument("--mgba", help="mGBA's install prefix (default: the `mgba` on the PATH)")
    sc.add_argument("--out")
    sc.add_argument("--timeout", type=float, default=600)
    c = sub.add_parser("compare", help="compare two logs")
    c.add_argument("logs", nargs=2)
    d = sub.add_parser("diff-screens", help="compare the PNGs of a reference directory with another's")
    d.add_argument("dirs", nargs=2)
    args = ap.parse_args()

    if args.cmd == "diff-screens":
        diffs, n = diff_screens(*args.dirs)
        for line in diffs:
            print(line)
        print(f"boottest: {args.dirs[1]}: " + (f"{len(diffs)} of {n} pictures differ" if diffs
                                                 else f"the same pictures ({n})"))
        return 1 if diffs else 0

    if args.cmd == "compare":
        a, b = (json.load(open(p)) for p in args.logs)
        diffs = compare(a, b)
        for d in diffs:
            print(d)
        print(f"boottest: {args.logs[0]} and {args.logs[1]}: "
              + ("differ" if diffs else f"the same ({len(a)} scripts)"))
        return 1 if diffs else 0

    if (args.keys is None) != (args.frames is None):
        ap.error("--keys and --frames go together")
    if getattr(args, "dump", None):
        os.makedirs(args.dump, exist_ok=True)
    if args.cmd in ("host", "screens"):
        if args.screens:
            os.makedirs(args.screens, exist_ok=True)
        if args.cmd == "host":
            log = run_host(args.exe, args.keys, args.frames, args.timeout, args.dump, args.every,
                           args.screens, args.level, args.room)
        else:
            log = run_screens(args.rom, args.keys, args.frames, args.timeout, mgba_prefix(args.mgba),
                              args.every, args.screens, args.elf, args.level, args.room, args.save)
        print(f"keys: {log['end']} ({log['seconds']} s); {len(log['checkpoints'])} checkpoints")
        if args.out:
            write_log({"keys": log}, args.out)
        return 0 if log["end"].startswith("done") else 1

    syms, rom = read_elf(args.elf)
    logs = {}
    failed = False
    if args.keys:
        if args.script:
            ap.error("--keys is the script")
        SCRIPTS["keys"] = script_keys(read_keys(args.keys), args.frames)
    for name in ["keys"] if args.keys else args.script or DEFAULT_SCRIPTS:
        log = run_script(args.rom, syms, rom, SCRIPTS[name], args.timeout, dump=args.dump,
                         every=args.every)
        logs[name] = log
        kinds = {}
        for e in log["events"]:
            kinds.setdefault(e[1], []).append(e[2])
        mixer = sum(1 for c in log["checkpoints"] if c[2])
        print(f"{name}: {log['end']} ({log['seconds']} s); "
              + "; ".join(f"{k} {' '.join(map(str, v[:6]))}{' ...' if len(v) > 6 else ''}"
                          for k, v in kinds.items())
              + f"; mixer ran in {mixer}/{len(log['checkpoints'])} checkpoints; save {log['save']}")
        failed |= not log["end"].startswith("done")
    if args.out:
        with open(args.out, "w") as f:
            json.dump(logs, f, indent=1)
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
