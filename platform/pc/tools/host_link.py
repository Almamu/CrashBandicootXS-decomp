#!/usr/bin/env python3
"""The pc platform's link symbols (#1093, docs/platforms.md).

    host_link.py --cxx-symbols cxx_symbols.txt --iwram sym_iwram.txt
                 --ewram sym_ewram.txt -o host_link.ld OBJ...

On the GBA, ldscript.txt places the globals that have no C definition
(sym_iwram.txt, sym_ewram.txt: `. = 0x9E8; gIntrTable = .;`), and every
C++ object's g++ 2.9 mangled names are renamed to the C names the rest of
the build uses (cxx_symbols.txt, `objcopy --redefine-syms`). The host
compiler mangles differently and has no ldscript, so this writes a linker
script of symbol assignments, passed to the host link as an input file:

- Each linker-placed global at twice its GBA offset in the host's IWRAM
  or EWRAM array (gHostIwram, gHostEwram, platform/pc/src/memory.c), so
  that it has room for its pointers at 8 bytes, and an alias inside a
  table of pointers (gIntrTableTimer2, gIntrTable's Timer2 slot) stays
  inside it. Under its C name, and under the host's mangled name of the
  C++ symbol cxx_symbols.txt gives that C name (Irq::table, gIntrTable).
- Each C name of a C++ symbol (cxx_symbols.txt) that a C file, an
  `extern "C"` declaration or an asm("CName") label refers to, as an
  alias of the host's definition of that symbol, found by its qualified
  name: `SetCheckpointAtPlayer = _ZN4Game5Level10LevelState21SetCheckpointAtPlayerEh;`.

Every assignment is a PROVIDE, so it only defines a name that the objects
use and don't define. The objects' symbols are read with nm, the host's
names demangled with c++filt.

With --libs, it also checks that no object defines a name the host's
libraries (libc, libm, libstdc++) export: the game's own strcpy, rand,
sprintf and the like would be linked in front of the C library's, for its
other users too, so platform.mk renames them in the game's objects
(platform/pc/libc_names.txt, given as --renames). So are the global
operator new and delete the game defines (operator_new.cpp): a C++ name
renamed there still stands for its qualified name (`operator new`), which
the C names in cxx_symbols.txt (OperatorNew) are bound to.
"""

import argparse
import re
import shlex
import subprocess
import sys


def parse_qualified(s, i):
    """A g++ 2.9 class or namespace name at s[i]: `4Ctrl` or
    `Q34Game8Entities4Ctrl`. Returns (parts, end) or None."""
    if i < len(s) and s[i] == "Q":
        i += 1
        if i < len(s) and s[i] == "_":
            j = s.index("_", i + 1)
            count = int(s[i + 1:j])
            i = j + 1
        else:
            if i >= len(s) or not s[i].isdigit():
                return None
            count = int(s[i])
            i += 1
        parts = []
        for _ in range(count):
            m = re.match(r"\d+", s[i:])
            if not m:
                return None
            n = int(m.group())
            i += len(m.group())
            parts.append(s[i:i + n])
            i += n
        return parts, i
    m = re.match(r"\d+", s[i:])
    if not m:
        return None
    n = int(m.group())
    i += len(m.group())
    return [s[i:i + n]], i + n


# g++ 2.x's assembler names of the global operators.
BUILTIN_OPERATORS = {
    "__builtin_new": "operator new",
    "__builtin_delete": "operator delete",
    "__builtin_vec_new": "operator new[]",
    "__builtin_vec_delete": "operator delete[]",
}


def demangle_gxx29(name):
    """(is_vtable, qualified name) of a g++ 2.9 mangled name, or None for
    a name that isn't one (`memcpy`, `__builtin_new`)."""
    if name in BUILTIN_OPERATORS:
        return (False, BUILTIN_OPERATORS[name])
    if name.startswith("_vt."):
        q = parse_qualified(name, 4)
        return (True, "::".join(q[0])) if q and q[1] == len(name) else None
    if name.startswith("_._"):
        q = parse_qualified(name, 3)
        return (False, "::".join(q[0] + ["~" + q[0][-1]])) if q else None
    if name.startswith("__") and len(name) > 2 and (name[2] == "Q" or name[2].isdigit()):
        q = parse_qualified(name, 2)
        return (False, "::".join(q[0] + [q[0][-1]])) if q else None
    if name.startswith("_") and len(name) > 1 and (name[1] == "Q" or name[1].isdigit()):
        q = parse_qualified(name, 1)
        if q and q[1] < len(name) and name[q[1]] in ".$":
            return (False, "::".join(q[0] + [name[q[1] + 1:]]))
        return None
    m = re.match(r"(.+?)__(C?)(?=Q|\d)", name)
    if m:
        q = parse_qualified(name, m.end())
        if q:
            return (False, "::".join(q[0] + [m.group(1)]))
    return None


def host_qualified(demangled):
    """(is_vtable, qualified name) of a c++filt-demangled host name, or None."""
    if demangled.startswith("vtable for "):
        return (True, demangled[len("vtable for "):])
    if " for " in demangled:  # typeinfo, guard variables, thunks
        return None
    return (False, demangled.split("(", 1)[0])


def read_placements(path, base):
    """The `. = OFFSET; NAME = .;` lines of a sym_*.txt."""
    out = {}
    with open(path) as f:
        for line in f:
            m = re.match(r"\s*\.\s*=\s*(0x[0-9A-Fa-f]+|\d+)\s*;\s*(\w+)\s*=\s*\.\s*;", line)
            if m:
                out[m.group(2)] = (base, 2 * int(m.group(1), 0))
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--nm", default="nm")
    ap.add_argument("--cxxfilt", default="c++filt")
    ap.add_argument("--cxx-symbols", required=True)
    ap.add_argument("--iwram", required=True)
    ap.add_argument("--ewram", required=True)
    ap.add_argument("--libs", nargs="*", default=[], help="the host's shared libraries, to check names against")
    ap.add_argument("--renames", help="the renames platform.mk made in the objects (OLD NEW lines)")
    ap.add_argument("-o", "--output", required=True)
    ap.add_argument("objects", nargs="+")
    args = ap.parse_args()

    placed = read_placements(args.iwram, "gHostIwram")
    placed.update(read_placements(args.ewram, "gHostEwram"))

    # C name -> the qualified name of the C++ symbol it renames, and back.
    c_to_qual = {}
    qual_to_c = {}
    with open(args.cxx_symbols) as f:
        for line in f:
            line = line.split("#", 1)[0].split()
            if len(line) != 2:
                continue
            q = demangle_gxx29(line[0])
            if q:
                c_to_qual[line[1]] = q
                qual_to_c.setdefault(q, line[1])

    nm = subprocess.run(shlex.split(args.nm) + ["-P"] + args.objects, capture_output=True, text=True, check=True).stdout
    defined = set()
    undefined = set()
    for line in nm.splitlines():
        f = line.split()
        if len(f) < 2 or f[0].endswith(":"):
            continue
        (undefined if f[1] in "Uw" else defined).add(f[0])
    undefined -= defined

    # The game's definitions of the host libraries' names.
    lib_names = set()
    for lib in args.libs:
        out = subprocess.run(shlex.split(args.nm) + ["-D", "--defined-only", "-P", lib], capture_output=True,
                             text=True, check=True).stdout
        lib_names.update(line.split()[0].split("@")[0] for line in out.splitlines() if line.split())
    clashes = sorted(defined & lib_names)
    for name in clashes:
        print(f"host_link.py: the objects define {name}, which the host's libraries do too: "
              f"add it to platform/pc/libc_names.txt", file=sys.stderr)
    if clashes:
        return 1

    # The objects' C++ names renamed from the host's mangled ones.
    renamed = {}
    if args.renames:
        with open(args.renames) as f:
            for line in f:
                line = line.split("#", 1)[0].split()
                if len(line) == 2 and line[0].startswith("_Z") and line[1] in defined:
                    renamed[line[0]] = line[1]

    names = sorted(defined | undefined | set(renamed))
    mangled = [n for n in names if n.startswith("_Z")]
    filt = subprocess.run(shlex.split(args.cxxfilt), input="\n".join(mangled) + "\n", capture_output=True, text=True, check=True).stdout.splitlines()
    host_qual = {}
    for raw, dem in zip(mangled, filt):
        q = host_qualified(dem)
        if q:
            host_qual[raw] = q

    # The host's definitions by qualified name. A constructor or
    # destructor has several (C1/C2, D0/D1/D2): the complete-object one
    # (C1, D1) is the one g++ 2.9's single one stands for.
    definitions = {}
    for raw in mangled:
        if raw in defined and raw in host_qual:
            definitions.setdefault(host_qual[raw], []).append(raw)
        elif raw in renamed and raw in host_qual:
            definitions.setdefault(host_qual[raw], []).append(renamed[raw])

    def pick(cands):
        if len(cands) == 1:
            return cands[0]
        complete = [c for c in cands if re.search(r"(C1|D1)E", c)]
        return complete[0] if len(complete) == 1 else None

    lines = []
    unresolved = []
    for name in sorted(undefined):
        if name in placed:
            base, off = placed[name]
            lines.append(f"PROVIDE({name} = {base} + 0x{off:X});")
        elif name in host_qual and host_qual[name] in qual_to_c and qual_to_c[host_qual[name]] in placed:
            c = qual_to_c[host_qual[name]]
            base, off = placed[c]
            lines.append(f"PROVIDE({name} = {base} + 0x{off:X}); /* {c} */")
        elif name in c_to_qual:
            target = pick(definitions.get(c_to_qual[name], []))
            if target:
                lines.append(f"PROVIDE({name} = {target});")
            else:
                unresolved.append(name)
        # Anything else is the host libraries' (libc, libstdc++, libm).

    with open(args.output, "w") as f:
        f.write("/* Generated by platform/pc/tools/host_link.py: the linker-placed globals\n"
                " * and the C names of C++ symbols, for the host link (docs/platforms.md). */\n")
        f.write("\n".join(lines) + "\n")
    for name in unresolved:
        print(f"host_link.py: no host definition for {name} ({c_to_qual[name][1]})", file=sys.stderr)
    return 1 if unresolved else 0


if __name__ == "__main__":
    sys.exit(main())
