#!/usr/bin/env python3
"""Update cxx_symbols.txt after a change that renames C++ symbols only,
and check that it changed nothing else (#1033).

    tools/cxx_rename.py snapshot DIR   save the C++ objects' assembly
    tools/cxx_rename.py check DIR      list the renames, change nothing
    tools/cxx_rename.py apply DIR      rewrite cxx_symbols.txt's mangled names

Moving a class into a namespace, renaming a class or changing a
parameter's class changes the mangled names g++ gives its members
(`Setup__3Irq` becomes `Setup__Q36Engine3Sys3Irq`, and every method that
takes an `Irq *` changes too), but not their code. cxx_symbols.txt maps
the old names, so the link then fails on the new ones.

Run `snapshot` on a build of the tree before the change (it copies the
.s files g++ wrote for src/, build/crashbandicootxs/src/**/*.s), make the
change, build again (`make` stops at the link, after every object is
compiled), then `apply`. It compares each .s file with its snapshot line
by line: the lines must be the same except for symbol names, and each old
name must always become the same new one. Anything else (a different
instruction, register, constant or line count) is a code generation
change, which the tool reports and refuses to apply. With no difference
left, `apply` replaces each renamed mangled name in cxx_symbols.txt's
first column, keeping the C name, so the ROM and the .map stay the same.
Rebuild and `make compare` after it as always.
"""

import argparse
import os
import re
import shutil
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BUILD = os.path.join(ROOT, "build", "crashbandicootxs", "src")
CXX_SYMBOLS = os.path.join(ROOT, "cxx_symbols.txt")
TOKEN = re.compile(r"[A-Za-z_.$][\w.$]*|\S")
NAME = re.compile(r"[A-Za-z_.$]")


def asm_files(top):
    for root, _, files in os.walk(top):
        for f in sorted(files):
            if f.endswith(".s"):
                yield os.path.relpath(os.path.join(root, f), top)


def snapshot(out):
    if os.path.exists(out):
        shutil.rmtree(out)
    count = 0
    for rel in asm_files(BUILD):
        os.makedirs(os.path.dirname(os.path.join(out, rel)), exist_ok=True)
        shutil.copyfile(os.path.join(BUILD, rel), os.path.join(out, rel))
        count += 1
    print("%d .s files saved in %s" % (count, out))


def compare(base):
    """({old name: new name}, [problems])."""
    renames, problems = {}, []
    for rel in asm_files(base):
        new_path = os.path.join(BUILD, rel)
        if not os.path.exists(new_path):
            problems.append("%s: not built" % rel)
            continue
        old = open(os.path.join(base, rel)).read().split("\n")
        new = open(new_path).read().split("\n")
        if len(old) != len(new):
            problems.append("%s: %d lines, was %d" % (rel, len(new), len(old)))
            continue
        for num, (a, b) in enumerate(zip(old, new), 1):
            if a == b:
                continue
            ta, tb = TOKEN.findall(a), TOKEN.findall(b)
            if len(ta) != len(tb):
                problems.append("%s:%d: %r is now %r" % (rel, num, a, b))
                continue
            for x, y in zip(ta, tb):
                if x == y:
                    continue
                if not NAME.match(x) or not NAME.match(y):
                    problems.append("%s:%d: %r is now %r" % (rel, num, a, b))
                    break
                if renames.setdefault(x, y) != y:
                    problems.append("%s:%d: %s is both %s and %s" % (rel, num, x, renames[x], y))
    return renames, problems


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("mode", choices=("snapshot", "check", "apply"))
    ap.add_argument("dir", help="where the snapshot is")
    args = ap.parse_args()
    if args.mode == "snapshot":
        snapshot(args.dir)
        return 0
    renames, problems = compare(args.dir)
    for p in problems:
        print("code changed: " + p)
    lines = open(CXX_SYMBOLS).read().split("\n")
    mapped = set()
    for i, line in enumerate(lines):
        parts = line.split()
        if len(parts) == 2 and not parts[0].startswith("#") and parts[0] in renames:
            mapped.add(parts[0])
            lines[i] = line.replace(parts[0], renames[parts[0]], 1)
    for old in sorted(renames):
        note = "" if old in mapped else "  (not in cxx_symbols.txt)"
        print("%s -> %s%s" % (old, renames[old], note))
    print("%d renamed, %d in cxx_symbols.txt, %d code changes" % (len(renames), len(mapped), len(problems)))
    if problems:
        return 1
    if args.mode == "apply":
        open(CXX_SYMBOLS, "w").write("\n".join(lines))
    return 0


if __name__ == "__main__":
    sys.exit(main())
