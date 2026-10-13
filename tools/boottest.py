#!/usr/bin/env python3
"""Boots a ROM headless in mGBA and plays scripted input, recording what
the game does, so two builds (the matching ROM and a shifted one, see
docs/shifting.md) can be compared run against run.

    tools/boottest.py run ROM.gba ROM.elf [--script NAME ...] [--out LOG.json]
    tools/boottest.py compare A.json B.json

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
  the same in both builds, and whether the GAX2 mixer (GaxMixerPlay) ran
  since the checkpoint before;
- the hash of the save file at the end, and how the run ended.

ROM.elf is the ELF the ROM was linked from: it gives the symbols'
addresses. `compare` exits 1 if two logs differ.
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


def run_script(rom_path, syms, rom, script, timeout):
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
                st.events.append([st.frame] + hooks[pc](g))
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
                try:
                    k = next(keys)
                except StopIteration:
                    log["end"] = f"done at frame {st.frame}"
                    break
                g.set_reg(KEYS_HOOK[2], k)
                if st.frame % CHECKPOINT_EVERY == 0:
                    h = hashlib.sha1()
                    for base, n in ((0x05000000, 0x400), (0x06000000, 0x18000), (0x07000000, 0x400)):
                        h.update(g.read(base, n))
                    log["checkpoints"].append([st.frame, h.hexdigest()[:16], mixer_ran])
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
            log["save"] = hashlib.sha1(open(sav, "rb").read()).hexdigest()[:16]
        shutil.rmtree(work, ignore_errors=True)
    log["seconds"] = round(time.time() - start, 1)
    return log


def compare(a, b):
    """The differences between two logs, as lines."""
    out = []
    for name in sorted(set(a) | set(b)):
        if name not in a or name not in b:
            out.append(f"{name}: only in one log")
            continue
        x, y = a[name], b[name]
        if x["events"] != y["events"]:
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


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    r = sub.add_parser("run", help="boot a ROM and run scripts")
    r.add_argument("rom")
    r.add_argument("elf")
    r.add_argument("--script", action="append", choices=sorted(SCRIPTS),
                   help=f"default: {' '.join(DEFAULT_SCRIPTS)}")
    r.add_argument("--out")
    r.add_argument("--timeout", type=float, default=600, help="seconds per script")
    c = sub.add_parser("compare", help="compare two logs")
    c.add_argument("logs", nargs=2)
    args = ap.parse_args()

    if args.cmd == "compare":
        a, b = (json.load(open(p)) for p in args.logs)
        diffs = compare(a, b)
        for d in diffs:
            print(d)
        print(f"boottest: {args.logs[0]} and {args.logs[1]}: "
              + ("differ" if diffs else f"the same ({len(a)} scripts)"))
        return 1 if diffs else 0

    syms, rom = read_elf(args.elf)
    logs = {}
    failed = False
    for name in args.script or DEFAULT_SCRIPTS:
        log = run_script(args.rom, syms, rom, SCRIPTS[name], args.timeout)
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
