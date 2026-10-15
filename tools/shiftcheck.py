#!/usr/bin/env python3
"""ROM shiftability check (#967): does anything in the ROM hold a fixed
address where it should hold a relocation?

The ROM's layout comes from games/xs/ldscript.txt and each object's definition
order, and every cross-reference should be a symbol, so that editing the
ROM's own data (a bigger room, a re-encoded picture) can shift everything
linked after it. This links the ROM again with `ld -q` (--emit-relocs, so
the ELF keeps every relocation) a few times, each with N bytes of padding
inserted in front of one object, and compares each shifted link with the
unshifted one, input section by input section:

1. Every byte that no relocation covers must be the same in both links.
   A difference is a value the build computed from the layout without a
   relocation (a size, an alignment pad, an address baked in by a tool).
2. Every R_ARM_ABS32 word must hold the shifted address of what it
   pointed at, so a `symbol + addend` that runs past the end of its
   symbol's object into a different one is caught.
3. No word that no relocation covers may hold an address in the ROM that
   is a symbol's address (or a Thumb function's address + 1): that is a
   pointer written as a number, which stays put when its target moves.
   Data that only happens to look like an address (graphics, compressed
   streams) rarely lands exactly on a symbol; KNOWN_DATA lists the few
   words that do.

Usage (the Makefile's `shiftcheck` target runs it):

    tools/shiftcheck.py --ld 'arm-none-eabi-ld ...' --ldscript games/xs/ldscript.txt \\
        --out build/shiftcheck [--shift NAME:OBJECT:BYTES ...] -- INPUTS...

INPUTS are the link's objects and libraries, as the ROM's link gets them.
Each --shift inserts `. += BYTES;` in front of the first games/xs/ldscript.txt
line that contains OBJECT; without any, the SHIFTS below are used. See
docs/shifting.md.
"""
import argparse
import bisect
import os
import re
import shlex
import struct
import subprocess
import sys

ROM_BASE = 0x08000000

# Where the default shifts insert their padding: in the code, in front of
# the data, and in front of each large block of data whose neighbours
# reference it. The odd sizes check that nothing assumes an alignment the
# linker doesn't give it.
SHIFTS = [
    ("code", "src/engine/sys/bios_util.o(.text)", 0x100),
    ("code_odd", "src/engine/audio/audio.o(.text)", 0x2),
    ("data", "data/data.o(.rodata);", 0x100),
    ("rooms", "src/game/level/data/level_rooms_main_bonus.o(.rodata)", 0x1004),
    ("sprite_banks", "src/engine/gfx/data/sprite_banks_00_09.o(.rodata)", 0x100),
    ("sound", "data/data.o(.rodata.084C0006)", 0x1),
    ("sound_even", "data/data.o(.rodata.084C0006)", 0x2),
    ("level_assets", "src/game/level/data/level_assets.o(.rodata)", 0x100),
    ("vtables", "entity.o(.gnu.linkonce.d._vt.Q34Game8Entities6Entity)", 0x3),
]

# Words that no relocation covers and that hold a symbol's address by
# coincidence, as {(object, section): {offset in the section: value}}.
# An entry stops applying when its word changes. When an edit makes a new
# one, check that the word is really data (pixels, PCM, a compressed
# stream) before adding it; the failure prints the entry.
KNOWN_DATA = {
    # sound-effect sample PCM (sound/sfx_samples/*.wav)
    ("data/data.o", ".rodata.084C0006"): {
        0x2F670: 0x0801071D, 0x3E884: 0x08000621, 0x4BE44: 0x08000519, 0x7A2D8: 0x08170C0C,
    },
    # cutscene pictures' pixels
    ("src/game/ui/data/cutscene_pictures.o", ".rodata"): {0x15988: 0x08000130, 0x5C244: 0x08000120},
    # the title screen's BG tiles
    ("src/game/ui/data/menu_graphics.o", ".rodata"): {0x13444: 0x0800013C},
    # gRoom24Asset's LZ77 stream
    ("src/game/level/data/level_assets.o", ".rodata"): {0x35EDC: 0x084B414C},
}

R_ARM_ABS32 = 2
# Relocation types whose field is 2 bytes; every other one patches 4.
TWO_BYTE_RELOCS = {5, 102, 103}  # R_ARM_ABS16, R_ARM_THM_JUMP11, R_ARM_THM_JUMP8
NO_FIELD_RELOCS = {40}  # R_ARM_V4BX


class Elf:
    """The parts of an ARM ELF32 executable linked with -q this needs."""

    def __init__(self, path):
        d = open(path, "rb").read()
        self.data = d
        phoff, shoff = struct.unpack_from("<II", d, 0x1C)
        phentsize, phnum, shentsize, shnum, shstrndx = struct.unpack_from("<5H", d, 0x2A)
        self.segments = [struct.unpack_from("<8I", d, phoff + i * phentsize) for i in range(phnum)]
        secs = [struct.unpack_from("<10I", d, shoff + i * shentsize) for i in range(shnum)]
        strtab = secs[shstrndx]

        def name(sec, off):
            o = sec[4] + off
            return d[o:d.index(b"\0", o)].decode()

        self.sections = [(name(strtab, s[0]),) + s[1:] for s in secs]
        self.image = self._image()
        self.relocs = []  # (load address, type)
        for s in self.sections:
            if s[1] != 9:  # SHT_REL
                continue
            target = self.sections[s[7]]
            delta = self.lma(target) - target[3]
            for o in range(s[4], s[4] + s[5], 8):
                r_offset, r_info = struct.unpack_from("<II", d, o)
                self.relocs.append((r_offset + delta, r_info & 0xFF))
        self.symbols = self._symbols()

    def lma(self, sec):
        """The load address of an allocated section."""
        for p_type, p_offset, p_vaddr, p_paddr, p_filesz, *_ in self.segments:
            if p_type == 1 and p_offset <= sec[4] < p_offset + p_filesz:
                return p_paddr + sec[4] - p_offset
        return sec[3]

    def _image(self):
        """The ROM image, as objcopy -O binary writes it."""
        loads = [p for p in self.segments if p[0] == 1 and p[4] and p[3] >= ROM_BASE]
        end = max(p[3] + p[4] for p in loads)
        img = bytearray(end - ROM_BASE)
        for _, off, _, paddr, filesz, *_ in loads:
            img[paddr - ROM_BASE:paddr - ROM_BASE + filesz] = self.data[off:off + filesz]
        return bytes(img)

    def _symbols(self):
        """{address: name} of the defined symbols, without mapping symbols
        ($a, $t, $d) and local labels."""
        out = {}
        for s in self.sections:
            if s[1] != 2:  # SHT_SYMTAB
                continue
            strs = self.sections[s[6]]
            for o in range(s[4] + 16, s[4] + s[5], 16):
                st_name, value, _, info, _, shndx = struct.unpack_from("<IIIBBH", self.data, o)
                if shndx == 0 or st_name == 0:
                    continue
                so = strs[4] + st_name
                nm = self.data[so:self.data.index(b"\0", so)].decode()
                if nm.startswith(("$", ".")):
                    continue
                # A Thumb function's value is its address + 1, which is
                # what a pointer to it holds.
                out.setdefault(value, nm)
        return out


class InputSection:
    __slots__ = ("key", "vma", "lma", "size")

    def __init__(self, key, vma, lma, size):
        self.key, self.vma, self.lma, self.size = key, vma, lma, size


LOADED_OUTPUT_SECTIONS = ("ROM", "iwram", "appended")


def read_map(path):
    """The input sections of the loaded output sections, in link order, from
    an ld -Map file."""
    sections = []
    seen = {}
    out_name, delta = None, 0
    pending = None
    for line in open(path):
        line = line.rstrip("\n")
        m = re.match(r"^(\S+)\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)(?: load address 0x([0-9a-f]+))?", line)
        if m and not line.startswith(" "):
            out_name = m.group(1) if m.group(1) in LOADED_OUTPUT_SECTIONS else None
            delta = int(m.group(4), 16) - int(m.group(2), 16) if m.group(4) else 0
            continue
        if re.match(r"^\S", line):
            out_name = line if line in LOADED_OUTPUT_SECTIONS else None
            delta = 0
            continue
        if out_name is None:
            continue
        m = re.match(r"^ (\.\S+)$", line)
        if m:
            pending = m.group(1)
            continue
        m = re.match(r"^ (\.\S+)?\s+0x([0-9a-f]+)\s+0x([0-9a-f]+) (\S.*)$", line)
        if m and (m.group(1) or pending):
            name = m.group(1) or pending
            pending = None
            vma, size = int(m.group(2), 16), int(m.group(3), 16)
            if size == 0:
                continue
            key = (m.group(4), name)
            n = seen.get(key, 0)
            seen[key] = n + 1
            if n:
                key = key + (n,)
            sections.append(InputSection(key, vma, vma + delta, size))
            continue
        pending = None
    return sections


def link(ld, ldscript, inputs, out_base):
    cmd = shlex.split(ld) + ["-q", "-T", ldscript, "-Map", out_base + ".map"] + inputs + ["-o", out_base + ".elf"]
    subprocess.run(cmd, check=True)
    return Elf(out_base + ".elf"), read_map(out_base + ".map")


def shifted_ldscript(ldscript, anchor, size, path):
    lines = open(ldscript).read().split("\n")
    hits = [i for i, l in enumerate(lines) if anchor in l and not l.strip().startswith(("/*", "*"))]
    if not hits:
        sys.exit(f"shiftcheck: no line of {ldscript} contains {anchor!r}")
    i = hits[0]
    indent = lines[i][:len(lines[i]) - len(lines[i].lstrip())]
    lines.insert(i, f"{indent}. += {size:#x}; /* shiftcheck */")
    with open(path, "w") as f:
        f.write("\n".join(lines))


class Layout:
    """Maps the addresses of one link to another, input section by input
    section."""

    def __init__(self, base, shifted):
        by_key = {s.key: s for s in shifted}
        missing = [s.key for s in base if s.key not in by_key]
        if missing:
            sys.exit(f"shiftcheck: input sections missing from the shifted link: {missing[:5]}")
        self.pairs = sorted(((s, by_key[s.key]) for s in base), key=lambda p: p[0].vma)
        self.starts = [p[0].vma for p in self.pairs]

    def map(self, v):
        """The addresses v may have become: v inside an input section moves
        with it, the end of one section may be either its end or the start
        of the next, and an address outside them all stays put."""
        out = set()
        i = bisect.bisect_right(self.starts, v) - 1
        for j in (i, i - 1):
            if 0 <= j < len(self.pairs):
                a, b = self.pairs[j]
                if a.vma <= v < a.vma + a.size or v == a.vma + a.size:
                    out.add(v - a.vma + b.vma)
        return out or {v}


def covered(elf):
    """A mask of the image's bytes that a relocation patches."""
    mask = bytearray(len(elf.image))
    for lma, rtype in elf.relocs:
        if rtype in NO_FIELD_RELOCS:
            continue
        o = lma - ROM_BASE
        if 0 <= o < len(mask):
            n = 2 if rtype in TWO_BYTE_RELOCS else 4
            mask[o:o + n] = b"\1" * n
    return mask


def describe(elf, addr):
    """addr, and the symbol it is in."""
    if not hasattr(elf, "labels"):
        elf.labels = sorted((a & ~1, n) for a, n in elf.symbols.items())
        elf.label_addrs = [a for a, _ in elf.labels]
    i = bisect.bisect_right(elf.label_addrs, addr & ~1) - 1
    if i < 0:
        return f"{addr:#010x}"
    a, n = elf.labels[i]
    return f"{addr:#010x} ({n}+{addr - a:#x})"


def fixed_pointers(elf, sections, mask):
    """Check 3: unrelocated words that hold a ROM symbol's address."""
    rom_end = max(s.vma + s.size for s in sections if s.vma >= ROM_BASE)
    found = []
    for s in sections:
        known = KNOWN_DATA.get((short_path(s.key[0]), s.key[1]), {})
        start = s.vma + (-s.vma % 4)
        for vma in range(start, s.vma + s.size - 3, 4):
            o = vma - s.vma + s.lma - ROM_BASE
            if mask[o:o + 4] != b"\0\0\0\0":
                continue
            v = struct.unpack_from("<I", elf.image, o)[0]
            # (not the ROM's first address, `start`: pixel data hits
            # 00 00 00 08 often, and nothing stores the entry point)
            if ROM_BASE < v < rom_end and v in elf.symbols and known.get(vma - s.vma) != v:
                found.append((s, vma, v))
    return found


def short_path(path):
    """An object's path without the build directory."""
    return path.split("build/crashbandicootxs/", 1)[-1]


def compare(name, base, base_secs, mask, shifted, shifted_secs, limit):
    """Checks 1 and 2 for one shifted link. Returns the problems found."""
    layout = Layout(base_secs, shifted_secs)
    problems = []
    for a, b in layout.pairs:
        ao, bo = a.lma - ROM_BASE, b.lma - ROM_BASE
        x = base.image[ao:ao + a.size]
        y = shifted.image[bo:bo + b.size]
        if x == y:
            continue
        m = mask[ao:ao + a.size]
        for i in range(a.size):
            if x[i] != y[i] and not m[i]:
                problems.append(f"{name}: {a.key[0]}({a.key[1]}) {describe(base, a.vma + i)}: "
                                f"an unrelocated byte changed ({x[i]:#04x} -> {y[i]:#04x})")
                break
    lmas = {}
    for a, b in layout.pairs:
        lmas[a.lma] = (a, b)
    starts = sorted(lmas)
    for lma, rtype in base.relocs:
        if rtype != R_ARM_ABS32:
            continue
        i = bisect.bisect_right(starts, lma) - 1
        if i < 0:
            continue
        a, b = lmas[starts[i]]
        if not a.lma <= lma < a.lma + a.size:
            continue
        v = struct.unpack_from("<I", base.image, lma - ROM_BASE)[0]
        w = struct.unpack_from("<I", shifted.image, lma - a.lma + b.lma - ROM_BASE)[0]
        if w not in layout.map(v) and (w ^ 1) not in layout.map(v ^ 1):
            problems.append(f"{name}: {a.key[0]}({a.key[1]}) {describe(base, a.vma + lma - a.lma)}: "
                            f"a pointer to {describe(base, v)} became {w:#010x}")
        if len(problems) > limit:
            break
    return problems


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--ld", required=True, help="the linker command, with its flags")
    ap.add_argument("--ldscript", default="games/xs/ldscript.txt")
    ap.add_argument("--out", default="build/shiftcheck", help="where the links go")
    ap.add_argument("--shift", action="append", default=[], metavar="NAME:OBJECT:BYTES")
    ap.add_argument("--limit", type=int, default=50, help="problems to list per check")
    ap.add_argument("--roms", action="store_true",
                    help="also write each shifted link's ROM image, NAME.gba, for tools/boottest.py")
    ap.add_argument("inputs", nargs="+")
    args = ap.parse_args()

    shifts = []
    for s in args.shift:
        nm, obj, size = s.rsplit(":", 2) if s.count(":") >= 2 else (None, None, None)
        if nm is None:
            sys.exit(f"shiftcheck: --shift {s!r}: expected NAME:OBJECT:BYTES")
        shifts.append((nm, obj, int(size, 0)))
    shifts = shifts or SHIFTS

    os.makedirs(args.out, exist_ok=True)
    base, base_secs = link(args.ld, args.ldscript, args.inputs, os.path.join(args.out, "base"))
    mask = covered(base)
    failed = False

    found = fixed_pointers(base, base_secs, mask)
    for s, vma, v in found[:args.limit]:
        print(f"fixed pointer: {s.key[0]}({s.key[1]}) {describe(base, vma)} holds {describe(base, v)}"
              f"  [KNOWN_DATA entry, if it is data: (\"{short_path(s.key[0])}\", \"{s.key[1]}\"):"
              f" {{{vma - s.vma:#x}: {v:#010x}}}]")
    if found:
        by_file = {}
        for s, _, _ in found:
            by_file[s.key[0]] = by_file.get(s.key[0], 0) + 1
        for f, n in sorted(by_file.items(), key=lambda kv: -kv[1]):
            print(f"fixed pointers: {n} in {f}")
        failed = True
    print("fixed pointers: " + (f"{len(found)} found" if found else "none"))

    for nm, obj, size in shifts:
        script = os.path.join(args.out, nm + ".ld")
        shifted_ldscript(args.ldscript, obj, size, script)
        shifted, shifted_secs = link(args.ld, script, args.inputs, os.path.join(args.out, nm))
        if args.roms:
            with open(os.path.join(args.out, nm + ".gba"), "wb") as f:
                f.write(shifted.image)
        problems = compare(nm, base, base_secs, mask, shifted, shifted_secs, args.limit)
        for p in problems[:args.limit]:
            print(p)
        if len(problems) > args.limit:
            print(f"{nm}: ... and more")
        print(f"shift {nm} ({size:#x} bytes before {obj}): "
              + ("OK" if not problems else f"{len(problems)} problem(s)"))
        failed |= bool(problems)

    print("shiftcheck: " + ("FAILED" if failed else "OK"))
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
