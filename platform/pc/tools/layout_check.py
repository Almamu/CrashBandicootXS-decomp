#!/usr/bin/env python3
"""Checks the pc platform's placement of the linker-placed globals (#1093,
docs/platforms.md).

    layout_check.py --link host_link.ld --iwram sym_iwram.txt
                    --ewram sym_ewram.txt [--readelf READELF] EXE

host_link.py puts each global of sym_iwram.txt and sym_ewram.txt at twice
its GBA offset in gHostIwram or gHostEwram, on the assumption that no
global takes more than twice its GBA room on the host (8-byte pointers,
and the padding they bring). This checks it: each global's host size,
from the declarations in the executable's debug information (readelf's
dump of DWARF), must end before the next global's host offset, unless
the globals after it that it covers are its own parts (gIntrTableTimer2
is gIntrTable's Timer2 slot, ALIASES). It also lists every global that
needs more than its GBA slot (the gap to the next symbol), which is what
the doubling is for.

Exits 1 if a global overlaps another. A global with no size (declared
without its array bound, or as an `int` that only marks an address,
like iwram_end) is listed, not failed.
"""

import argparse
import os
import re
import subprocess
import sys

# A name inside another global: the inner name -> the global it is part of.
ALIASES = {
    "gIntrTableTimer2": "gIntrTable",
}

PLACE = re.compile(r"PROVIDE\((\S+) = (gHostIwram|gHostEwram) \+ (0x[0-9A-Fa-f]+)\);(?: /\* (\S+) \*/)?")
SYM = re.compile(r"\. = (0x[0-9A-Fa-f]+); (\w+) = \.;")
DIE = re.compile(r"^\s*<(\d+)><([0-9a-f]+)>: Abbrev Number: (\d+)(?: \((DW_TAG_\w+)\))?")
ATTR = re.compile(r"^\s*<[0-9a-f]+>\s+(DW_AT_\w+)\s*:\s*(.*)$")
SCOPES = {"DW_TAG_namespace", "DW_TAG_class_type", "DW_TAG_structure_type", "DW_TAG_union_type"}
SIZED = {"DW_TAG_base_type", "DW_TAG_pointer_type", "DW_TAG_structure_type", "DW_TAG_class_type",
         "DW_TAG_union_type", "DW_TAG_enumeration_type", "DW_TAG_reference_type",
         "DW_TAG_rvalue_reference_type", "DW_TAG_ptr_to_member_type"}
WRAPPERS = {"DW_TAG_typedef", "DW_TAG_const_type", "DW_TAG_volatile_type", "DW_TAG_restrict_type",
            "DW_TAG_atomic_type"}


def read_syms(path):
    out = []
    for line in open(path):
        m = SYM.search(line)
        if m:
            out.append((int(m.group(1), 16), m.group(2)))
    return out


def name_of(value):
    """An attribute's string: `(offset: 0x1234): name` or the name."""
    return value.split("): ", 1)[1] if value.startswith("(") and "): " in value else value


def ref_of(value):
    m = re.match(r"<0x([0-9a-f]+)>", value)
    return int(m.group(1), 16) if m else None


def int_of(value):
    m = re.match(r"(0x[0-9a-f]+|\d+)", value)
    return int(m.group(1), 0) if m else None


def read_dwarf(readelf, exe):
    """{offset: [tag, attrs, children, qualified name]} of every DIE."""
    env = dict(os.environ, LC_ALL="C")
    p = subprocess.Popen([readelf, "--debug-dump=info", "--wide", exe], stdout=subprocess.PIPE, text=True,
                         env=env, errors="replace")
    dies = {}
    stack = []  # (depth, offset, scope name or None)
    cur = None
    for line in p.stdout:
        m = DIE.match(line)
        if m:
            depth, off, tag = int(m.group(1)), int(m.group(2), 16), m.group(4)
            while stack and stack[-1][0] >= depth:
                stack.pop()
            if tag is None:  # the end of a DIE's children
                cur = None
                continue
            parent = stack[-1][1] if stack else None
            cur = [tag, {}, [], None, parent]
            dies[off] = cur
            if parent is not None:
                dies[parent][2].append(off)
            stack.append((depth, off))
            continue
        a = ATTR.match(line)
        if a and cur is not None:
            # `(form) value`, with --wide's forms (strp, ref4, data1, ...)
            cur[1][a.group(1)] = re.sub(r"^\((?:\w|_)+\) ", "", a.group(2).strip())
    p.wait()

    def qualified(off):
        d = dies[off]
        if d[3] is None:
            name = name_of(d[1].get("DW_AT_name", ""))
            parent = d[4]
            prefix = ""
            if parent is not None and dies[parent][0] in SCOPES:
                prefix = qualified(parent)
                prefix = prefix + "::" if prefix else ""
            d[3] = prefix + name if name else ""
        return d[3]

    for off in dies:
        qualified(off)
    return dies


def sizer(dies):
    complete = {}
    for off, d in dies.items():
        if d[0] in SIZED and "DW_AT_byte_size" in d[1] and d[3]:
            complete.setdefault(d[3], int_of(d[1]["DW_AT_byte_size"]))

    def size(off, seen=()):
        if off is None or off in seen or off not in dies:
            return None
        tag, attrs, children, qname, _ = dies[off]
        if tag in SIZED:
            if "DW_AT_byte_size" in attrs:
                return int_of(attrs["DW_AT_byte_size"])
            return complete.get(qname)
        if tag in WRAPPERS:
            return size(ref_of(attrs.get("DW_AT_type", "")), seen + (off,))
        if tag == "DW_TAG_array_type":
            n = size(ref_of(attrs.get("DW_AT_type", "")), seen + (off,))
            for c in children:
                ca = dies[c][1]
                if "DW_AT_count" in ca:
                    n = None if n is None else n * int_of(ca["DW_AT_count"])
                elif "DW_AT_upper_bound" in ca:
                    n = None if n is None else n * (int_of(ca["DW_AT_upper_bound"]) + 1)
                else:
                    return None
            return n
        return None
    return size


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--readelf", default="readelf")
    ap.add_argument("--cxxfilt", default="c++filt")
    ap.add_argument("--link", required=True)
    ap.add_argument("--iwram", required=True)
    ap.add_argument("--ewram", required=True)
    ap.add_argument("exe")
    args = ap.parse_args()

    # The host's qualified name of each placed C name (its own, or that of
    # the C++ symbol host_link.py gave it).
    host = {}
    for line in open(args.link):
        m = PLACE.search(line)
        if m:
            host.setdefault(m.group(4) or m.group(1), m.group(1))
    mangled = sorted(n for n in set(host.values()) if n.startswith("_Z"))
    out = subprocess.run([args.cxxfilt] + mangled, capture_output=True, text=True, check=True).stdout.split("\n")
    demangled = dict(zip(mangled, out))

    dies = read_dwarf(args.readelf, args.exe)
    size = sizer(dies)
    sizes = {}
    for off, d in dies.items():
        if d[0] in ("DW_TAG_variable", "DW_TAG_member") and d[3]:
            s = size(ref_of(d[1].get("DW_AT_type", "")))
            if s is not None:
                sizes.setdefault(d[3], s)

    bad = 0
    grown = 0
    unknown = []
    for region, syms, end in [("IWRAM", read_syms(args.iwram), 0x8000), ("EWRAM", read_syms(args.ewram), 0x40000)]:
        offsets = sorted({o for o, _ in syms} | {end})
        for off, name in syms:
            gba_slot = next(o for o in offsets if o > off) - off
            h = host.get(name, name)
            s = sizes.get(demangled.get(h, h))
            if s is None:
                unknown.append(name)
                continue
            covered = [n for o, n in syms if off < o and 2 * o < 2 * off + s and ALIASES.get(n) != name]
            bad += bool(covered)
            if covered or s > gba_slot:
                grown += s > gba_slot
                print(f"{region} {name}: GBA slot {gba_slot:#x}, host size {s:#x}, room {2 * gba_slot:#x}"
                      + (f": overlaps {', '.join(covered)}" if covered else ""))
    total = sum(len(read_syms(p)) for p in (args.iwram, args.ewram))
    print(f"layout_check: {total - len(unknown)} of {total} globals sized, {grown} bigger than their GBA slot, "
          f"{bad} overlapping" + (f"; no size for {' '.join(unknown)}" if unknown else ""))
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
