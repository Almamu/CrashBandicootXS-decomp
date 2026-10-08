#!/usr/bin/env python3
"""Classify the compiled objects by their g++ 2.x C++ traits (#664).

The game was written in C++ and built with the g++ 2.9 that agbcc comes
from (docs/cplusplus.md). The decompilation reproduces it as C, so the
C++ shows up as hand-written runtime structures. This tool counts them
per object (one src/*/*.c or lib/*/src/*.c file):

  method     functions defined here that a C pointer-to-member table
             (ACTOR_PMF) points at: member functions (g++ emits every
             vtable since #664 step 10b, docs/cplusplus.md)
  vptr       stores of a vtable address into an object (constructors and
             destructors set the vptr: `x->vtable = gFooVtable`)
  ctor       functions that store a vptr and return the object: a g++
             2.x constructor returns `this`
  dtor       functions that store a vptr and take the `flags` argument
             (a g++ 2.x destructor's `__in_chrg`: bit 0 frees the object,
             usually passed on to the base class's destructor)
  new        `OperatorNew(...)` / `OperatorNewArray(...)` calls (g++'s
             `new` expression: __builtin_new then the constructor)
  delete     OperatorDelete / OperatorDeleteArray calls
  vcall      virtual calls: a method-table slot's `this` adjustment added
             to the object before the call (ACTOR_VCALL, ACT_VCALL*,
             PART_METHOD, `.delta`/`.thisOffset` reads)
  pmf        pointer-to-member-function calls (ACTOR_PMF_CALL, PMF_CALL,
             PMF_DISPATCH, `vtableOffset` reads)
  fnptr      other calls through `_call_via_rN` with no `this`
             adjustment (plain function pointers, callbacks)

An object with a method, a vptr store, a new, a vcall or a pmf is listed
as "C++". The rest are "C-like": nothing in them needs C++ (they may
still be C++ member functions whose class has no virtuals). Objects
already written as C++ (src/*/*.cpp, the Makefile's CXX_OBJS) are
listed as "C++ source"; the C patterns don't apply to them.

The ROM-level searches (RTTI names, __pure_virtual, static constructor
lists, exception tables) are in docs/cplusplus.md.

Usage:
  tools/cpp_survey.py             summary
  tools/cpp_survey.py --objects   one row per object
  tools/cpp_survey.py --plain     list the C-like objects
"""

import argparse
import glob
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

TRAITS = ("method", "vptr", "ctor", "dtor", "new", "delete", "vcall", "pmf", "fnptr")
CXX_TRAITS = ("method", "vptr", "new", "vcall", "pmf")

FUNC_DEF = re.compile(r"^((?:static\s+)?(?:inline\s+)?)(?:[A-Za-z_][\w\s\*]*?[\s\*])?((?:\w+::)?~?\w+)\s*\(([^;{}]*)\)\s*$")
VPTR_STORE = re.compile(r"(?:->|\.)\s*(?:vtable|table|methods|vt)\s*=\s*(?:\([^)]*\)\s*)?&?\s*g\w*(?:Vtable|Methods)\b"
                        r"|=\s*(?:\([^)]*\)\s*)?g\w*Vtable\b")
# Calls of the C names, not definitions (`void *OperatorNew(u32 size)`).
NEW = re.compile(r"(?<!\*)\bOperatorNew(?:Array)?\s*\(")
DELETE = re.compile(r"(?<!void )\bOperatorDelete(?:Array)?\s*\(")
VCALL = re.compile(r"\b(?:ACTOR_VCALL|ACT_VCALL\d?|PART_METHOD|ACTOR_METHOD)\s*\(|\.\s*(?:delta|thisOffset)\b|->\s*thisOffset\b")
PMF = re.compile(r"\b(?:ACTOR_PMF_CALL|PMF_CALL|PMF_DISPATCH)\s*\(|\bvtableOffset\b")
CALL_VIA = re.compile(r"\b_call_via_r\d+\s*\(")
RETURN_SELF = re.compile(r"\breturn\s+\(?\s*(?:\([^)]*\)\s*)?(?:self|selfArg|this|obj|part|p)\s*\)?\s*;")
DTOR_FLAG = re.compile(r"&\s*1\b")
DTOR_PARAM = re.compile(r"\bs32\s+(?:flags|inChrg|mode)\b")


def strip_comments(text):
    text = re.sub(r"/\*.*?\*/", lambda m: "\n" * m.group(0).count("\n"), text, flags=re.S)
    return re.sub(r"//[^\n]*", "", text)


def functions(text):
    """Yield (name, inline, params, body) for each top-level function definition."""
    lines = text.split("\n")
    i = 0
    while i < len(lines):
        m = FUNC_DEF.match(lines[i])
        if m and i + 1 < len(lines) and lines[i + 1].strip() == "{" and m.group(2) not in ("if", "while", "for", "switch"):
            depth = 0
            body = []
            j = i + 1
            while j < len(lines):
                depth += lines[j].count("{") - lines[j].count("}")
                body.append(lines[j])
                if depth == 0:
                    break
                j += 1
            yield m.group(2), "inline" in m.group(1), m.group(3), "\n".join(body)
            i = j + 1
            continue
        i += 1


def table_targets():
    """Names a C PMF table entry points at."""
    names = set()
    for path in glob.glob(os.path.join(ROOT, "src/data/*.c")):
        text = strip_comments(open(path).read())
        names.update(re.findall(r"\bACTOR_PMF\(\s*(\w+)\s*\)", text))
    names.discard("NULL")
    return names


def survey():
    targets = table_targets()
    rows = []
    paths = sorted(glob.glob(os.path.join(ROOT, "src/*/*.c")) + glob.glob(os.path.join(ROOT, "src/*/*.cpp"))
                   + glob.glob(os.path.join(ROOT, "lib/*/src/*.c")))
    for path in paths:
        rel = os.path.relpath(path, ROOT)
        if rel.startswith("src/data/"):
            continue
        text = strip_comments(open(path).read())
        row = dict.fromkeys(TRAITS, 0)
        row["file"] = rel
        row["funcs"] = 0
        for name, inline, params, body in functions(text):
            if inline:
                continue
            row["funcs"] += 1
            if name in targets:
                row["method"] += 1
            stores = bool(VPTR_STORE.search(body))
            if stores and RETURN_SELF.search(body):
                row["ctor"] += 1
            if stores and (DTOR_PARAM.search(params) or (DELETE.search(body) and DTOR_FLAG.search(body))):
                row["dtor"] += 1
        row["vptr"] = len(VPTR_STORE.findall(text))
        row["new"] = len(NEW.findall(text))
        row["delete"] = len(DELETE.findall(text))
        row["vcall"] = len(VCALL.findall(text))
        row["pmf"] = len(PMF.findall(text))
        row["fnptr"] = max(0, len(CALL_VIA.findall(text)) - row["vcall"])
        row["cpp"] = rel.endswith(".cpp")
        row["cxx"] = row["cpp"] or any(row[t] for t in CXX_TRAITS)
        rows.append(row)
    return rows


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--objects", action="store_true", help="one row per object")
    ap.add_argument("--plain", action="store_true", help="list the C-like objects")
    args = ap.parse_args()

    rows = survey()
    game = [r for r in rows if r["file"].startswith("src/")]
    libs = [r for r in rows if r["file"].startswith("lib/")]

    if args.objects:
        print("%-46s %5s " % ("object", "funcs") + " ".join("%6s" % t for t in TRAITS) + "  kind")
        for r in rows:
            print("%-46s %5d " % (r["file"], r["funcs"]) + " ".join("%6d" % r[t] for t in TRAITS)
                  + ("  C++ source" if r["cpp"] else "  C++" if r["cxx"] else "  C-like"))
        print()
    if args.plain:
        for r in rows:
            if not r["cxx"]:
                print(r["file"])
        print()

    def summary(label, rs):
        cxx = [r for r in rs if r["cxx"]]
        print("%s: %d objects, %d functions; %d objects (%d functions) with C++ traits, %d C-like;"
              " %d already C++ source"
              % (label, len(rs), sum(r["funcs"] for r in rs), len(cxx), sum(r["funcs"] for r in cxx),
                 len(rs) - len(cxx), sum(1 for r in rs if r["cpp"])))
        for t in TRAITS:
            n = [r for r in rs if r[t]]
            print("  %-7s %5d in %3d objects" % (t, sum(r[t] for r in rs), len(n)))

    summary("game (src/)", game)
    summary("libraries (lib/)", libs)
    return 0


if __name__ == "__main__":
    sys.exit(main())
