#!/usr/bin/env python3
"""Index the matched corpus at the RTL level and query it (#662).

    tools/rtl_corpus.py build [-j N] [--dir D]     compile every object with -da -g
    tools/rtl_corpus.py list                       the queries
    tools/rtl_corpus.py query NAME [--dir D] [--func F] [--src]

Round 7 mined the final instructions of the matched functions (`-g`
keeps every Thumb object byte-identical, so `objdump -dl` maps an
instruction to its C line). This goes one level earlier: every object
the Makefile builds is compiled again, with its own compiler and flags
plus `-da -g`, into D (default build/rtl_corpus, about 530 MB; delete
it when done), and the per-pass RTL dumps are read per function. Each
query finds one
situation that decides a kept workaround (#662's sites) in MATCHED code
and prints it with the C source line that produced it, so the construct
that gives the ROM's code can be copied from a function that already
matches. Every function the build compiles is matched, so every hit is
ROM evidence (except in the functions that still have a workaround,
whose RTL the workaround shaped).

The parser keeps, per function and pass, each insn's uid, kind
(insn/jump_insn/call_insn/code_label/note), its text and the source
file and line in force (the line-number notes `-g` adds). The queries
are plain Python over that (see QUERIES); add one per new situation.
`--src` prints the C line behind each hit.
"""

import argparse
import json
import os
import re
import shlex
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


# ---------------------------------------------------------------- build


def compile_lines():
    out = subprocess.run(["make", "-n", "-B", "report"], cwd=ROOT, capture_output=True, text=True).stdout
    for line in out.splitlines():
        if "| tools/agbcc/bin/" not in line:
            continue
        cpp, cc1 = line.split("|", 1)
        cpp = shlex.split(cpp)
        cc1 = shlex.split(cc1)
        src = [a for a in cpp if a.endswith((".c", ".cpp"))]
        if len(src) != 1:
            continue
        clean, skip = [], False
        for a in cpp:
            if skip:
                skip = False
                continue
            if a in ("-MF", "-MT"):
                skip = True
                continue
            if a in ("-MMD", "-MP"):
                continue
            clean.append(a)
        i = cc1.index("-o")
        del cc1[i : i + 2]
        yield src[0], clean, cc1


def build_one(d, src, cpp, cc1):
    stem = os.path.join(d, os.path.splitext(src)[0])
    os.makedirs(os.path.dirname(stem), exist_ok=True)
    ii = stem + ".ii"
    with open(ii, "wb") as f:
        r = subprocess.run(cpp, cwd=ROOT, stdout=f, stderr=subprocess.DEVNULL)
    if r.returncode:
        return src, "cpp failed"
    # cc1 names the dumps after its input: stem.ii.<pass>
    r = subprocess.run(
        [os.path.join(ROOT, cc1[0])] + cc1[1:] + ["-da", "-g", os.path.basename(ii), "-o", os.path.basename(stem) + ".s"],
        cwd=os.path.dirname(stem),
        capture_output=True,
    )
    if r.returncode:
        return src, "cc1 failed: " + r.stderr.decode()[:200]
    for p in ("bp", "range", "sched", "sched2"):
        try:
            os.unlink(ii + "." + p)
        except OSError:
            pass
    return src, None


def cmd_build(args):
    jobs = list(compile_lines())
    with ThreadPoolExecutor(args.j) as ex:
        for src, err in ex.map(lambda j: build_one(args.dir, *j), jobs):
            if err:
                print(src, err, file=sys.stderr)
    print("%d objects in %s" % (len(jobs), args.dir))


# ---------------------------------------------------------------- parse

HEAD = re.compile(r"^\((insn|jump_insn|call_insn|code_label|note|barrier|insn_list)/?\S* (\d+) (\d+) (\d+)")


def parse_dump(path):
    """{function: [insn dict]} for one dump file."""
    funcs = {}
    cur = None
    line = ("", 0)
    buf = []

    def flush():
        if not buf or cur is None:
            return
        t = " ".join(s.strip() for s in buf)
        m = HEAD.match(t)
        if not m:
            return
        kind, uid = m.group(1), int(m.group(2))
        nonlocal line
        ln = re.match(r'^\(note \d+ \d+ \d+ \("([^"]*)"\) (\d+)\)', t)
        if ln:
            line = (ln.group(1), int(ln.group(2)))
            return
        funcs[cur].append({"k": kind, "u": uid, "t": t, "l": line})

    with open(path, errors="replace") as f:
        for raw in f:
            if raw.startswith(";; Function "):
                flush()
                buf = []
                cur = raw[len(";; Function ") :].strip()
                funcs[cur] = []
                line = ("", 0)
                continue
            if raw.startswith("("):
                flush()
                buf = [raw]
            elif raw.strip() == "" or raw.startswith(";;"):
                flush()
                buf = []
            elif buf:
                buf.append(raw)
        flush()
    return funcs


def sources(d):
    for dp, dn, fn in os.walk(d):
        for f in fn:
            if f.endswith(".ii"):
                yield os.path.join(dp, f)


def load(d, passes, func_filter=None):
    """Yield (src, function, {pass: insns})."""
    for ii in sorted(sources(d)):
        src = os.path.relpath(ii, d)[: -len(".ii")]
        per = {}
        for p in passes:
            path = ii + "." + p
            if os.path.exists(path):
                per[p] = parse_dump(path)
        names = set()
        for v in per.values():
            names |= set(v)
        for n in sorted(names):
            if func_filter and func_filter not in n:
                continue
            yield src, n, {p: per[p].get(n, []) for p in per}


# ---------------------------------------------------------------- helpers

REG = r"\(reg(?:/[a-z]+)*:(\w+) (\d+)(?: \w+)?\)"
SET = re.compile(r"^\(insn \d+ \d+ \d+ \(set " + REG + r" (.*)\) -?\d+ ")


def sets(insns):
    """[(index, dest regno, mode, src text)] for each single-set insn."""
    out = []
    for i, x in enumerate(insns):
        if x["k"] != "insn":
            continue
        m = SET.match(x["t"])
        if m:
            out.append((i, int(m.group(2)), m.group(1), m.group(3)))
    return out


def const_regs(insns):
    """pseudo -> (index, value) for pseudos whose only set is a const_int."""
    count = {}
    val = {}
    for i, r, mode, s in sets(insns):
        count[r] = count.get(r, 0) + 1
        m = re.match(r"^\(const_int (-?\d+)", s)
        val[r] = (i, int(m.group(1))) if m else None
    return {r: v for r, v in val.items() if count[r] == 1 and v and r >= 16}


def loop_depths(insns):
    """Loop nesting depth at each insn index (NOTE_INSN_LOOP_BEG/END)."""
    d = 0
    out = []
    for x in insns:
        if x["k"] == "note" and "NOTE_INSN_LOOP_BEG" in x["t"]:
            d += 1
        out.append(d)
        if x["k"] == "note" and "NOTE_INSN_LOOP_END" in x["t"]:
            d -= 1
    return out


def labels_between(insns, a, b):
    return sum(1 for x in insns[a + 1 : b] if x["k"] == "code_label")


def uses_reg(text, r):
    return re.search(r"\(reg(?:/[a-z]+)*:\w+ %d(?: \w+)?\)" % r, text) is not None


# ---------------------------------------------------------------- queries


def q_const_first_and(src, fn, d):
    """An AND (or IOR/XOR) whose FIRST operand is a pseudo holding a
    constant after cse1: cse1's fold_rtx would have put a known constant
    second, so the constant was hidden from it there (ConvertTiles' mask).
    Reports where the constant is set relative to the use (labels between,
    loop depth of each) and whether loop.c moved the set (.loop dump)."""
    ins = d.get("cse", [])
    cr = const_regs(ins)
    dep = loop_depths(ins)
    for i, x in enumerate(ins):
        m = re.search(r"\((and|ior|xor):SI " + REG + r" " + REG + r"\)", x["t"])
        if not m:
            continue
        a, b = int(m.group(3)), int(m.group(5))
        if a in cr and b not in cr:
            si, v = cr[a]
            yield {
                "line": x["l"],
                "op": m.group(1),
                "const": v,
                "set_line": ins[si]["l"],
                "labels_between": labels_between(ins, si, i) if si < i else -1,
                "depth_set": dep[si],
                "depth_use": dep[i],
            }


def q_shift_after_loop(src, fn, d):
    """A shift by a constant of a pseudo that is not set inside the loop
    just before it, computed after that loop in the .gcse dump: gcse's PRE
    left it where it is (Credits::LoadLogos' palette address, which PRE
    hoists in front of the tile loops in the natural source)."""
    ins = d.get("gcse", [])
    if not ins:
        return
    beg = []
    loops = []
    for i, x in enumerate(ins):
        if x["k"] == "note" and "NOTE_INSN_LOOP_BEG" in x["t"]:
            beg.append(i)
        if x["k"] == "note" and "NOTE_INSN_LOOP_END" in x["t"] and beg:
            loops.append((beg.pop(), i))
    st = sets(ins)
    for i, r, mode, s in st:
        m = re.match(r"^\(ashift:SI " + REG + r" \(const_int (\d+)", s)
        if not m:
            continue
        p = int(m.group(2))
        # the nearest loop that ends before i
        prev = [lp for lp in loops if lp[1] < i]
        if not prev:
            continue
        lb, le = max(prev, key=lambda lp: lp[1])
        set_in_loop = any(j for j, rr, _, _ in st if rr == p and lb < j < le)
        set_before = any(j for j, rr, _, _ in st if rr == p and j < lb)
        if set_in_loop or not set_before:
            continue
        yield {"line": ins[i]["l"], "shift": int(m.group(3)), "loop_lines": [ins[lb]["l"], ins[le]["l"]], "depth": loop_depths(ins)[i]}


def q_pre_hoists(src, fn, d):
    """What gcse's PRE inserted (the .gcse dump's PRE/HOIST lines) in each
    function, with the line of the expression it hoisted."""
    path = os.path.join(ARGS.dir, src + ".ii.gcse")
    # parse_dump drops ;; lines, so read them here
    txt = open(path, errors="replace").read()
    part = txt.split(";; Function " + fn + "\n", 1)
    if len(part) < 2:
        return
    body = part[1].split(";; Function ", 1)[0]
    for m in re.finditer(r"PRE/HOIST: (.*)", body):
        yield {"pre": m.group(1).strip()}


def q_unfolded_const_chain(src, fn, d):
    """An IOR/PLUS of a register with a constant where that register holds
    a known constant at the end: constant arithmetic left
    unfolded (the .mach dump, the final RTL), as DingodileShieldCtrl's BLDCNT chain in r5."""
    ins = d.get("mach", [])
    known = {}
    for i, x in enumerate(ins):
        if x["k"] == "code_label":
            known = {}
        if x["k"] == "call_insn":
            known = {k: v for k, v in known.items() if k >= 4}
        m = SET.match(x["t"]) if x["k"] == "insn" else None
        if not m:
            continue
        r = int(m.group(2))
        s = m.group(3)
        c = re.match(r"^\(const_int (-?\d+)", s)
        o = re.match(r"^\((ior|plus|and|xor|ashift):SI " + REG + r" \(const_int (-?\d+)", s)
        o2 = re.match(r"^\((ior|plus|and|xor):SI " + REG + r" " + REG + r"\)", s)
        if o and int(o.group(3)) in known:
            yield {"line": x["l"], "op": o.group(1), "reg": int(o.group(3)), "known": known[int(o.group(3))]}
        elif o2 and (int(o2.group(3)) in known and int(o2.group(5)) in known):
            yield {"line": x["l"], "op": o2.group(1), "regs": [int(o2.group(3)), int(o2.group(5))]}
        if c:
            known[r] = int(c.group(1))
        else:
            known.pop(r, None)


def q_two_same_const(src, fn, d):
    """Two pseudos set to the same constant in one block, both live
    (.cse2 dump): the u16-operand idiom of round 7 (#816)."""
    ins = d.get("cse2", [])
    seen = {}
    for i, x in enumerate(ins):
        if x["k"] == "code_label":
            seen = {}
        m = SET.match(x["t"]) if x["k"] == "insn" else None
        if not m:
            continue
        c = re.match(r"^\(const_int (-?\d+)", m.group(3))
        if not c:
            continue
        v = int(c.group(1))
        r = int(m.group(2))
        if v in seen and seen[v][0] != r:
            yield {"line": x["l"], "const": v, "first_line": seen[v][1], "modes": [seen[v][2], m.group(1)]}
        seen.setdefault(v, (r, x["l"], m.group(1)))


def q_lreg_refs(src, fn, d):
    """Per pseudo, refs and live length from the .lreg dump header
    ('Register N used M times across L insns'), for comparing a pseudo
    with `this`."""
    path = os.path.join(ARGS.dir, src + ".ii.lreg")
    txt = open(path, errors="replace").read()
    part = txt.split(";; Function " + fn + "\n", 1)
    if len(part) < 2:
        return
    body = part[1].split(";; Function ", 1)[0]
    for m in re.finditer(r"Register (\d+) used (\d+) times across (\d+) insns(.*?)\.", body):
        yield {"reg": int(m.group(1)), "refs": int(m.group(2)), "live": int(m.group(3)), "notes": m.group(4).strip()}


def q_copy_of_const_in_loop(src, fn, d):
    """A register copy, inside a loop, of a pseudo whose only set is a
    constant made outside that loop's blocks (.loop dump): cse1 can't see
    the constant at the copy's uses, loop.c moves the copy to the
    innermost preheader, and reload rematerialises the constant there.
    Plain C reproduces the ConvertTiles mask this way
    (`u32 mask = 0xf;` ... `u32 m = mask;` in the pixel loop)."""
    ins = d.get("loop", [])
    cr = const_regs(ins)
    dep = loop_depths(ins)
    for i, r, mode, s in sets(ins):
        m = re.match("^" + REG + "$", s)
        if not m:
            continue
        b = int(m.group(2))
        if b in cr and dep[i] >= 1 and dep[cr[b][0]] < dep[i]:
            yield {"line": ins[i]["l"], "const": cr[b][1], "set_line": ins[cr[b][0]]["l"], "depth": dep[i]}


QUERIES = {
    "copy-of-const-in-loop": (q_copy_of_const_in_loop, ["loop"]),
    "const-first-and": (q_const_first_and, ["cse"]),
    "shift-after-loop": (q_shift_after_loop, ["gcse"]),
    "pre-hoists": (q_pre_hoists, ["gcse"]),
    "unfolded-const-chain": (q_unfolded_const_chain, ["mach"]),
    "two-same-const": (q_two_same_const, ["cse2"]),
    "lreg-refs": (q_lreg_refs, ["lreg"]),
}


def source_line(loc):
    path, line = loc
    try:
        with open(os.path.join(ROOT, path)) as f:
            return f.read().splitlines()[line - 1].strip()
    except (OSError, IndexError):
        return ""


def cmd_query(args):
    fn, passes = QUERIES[args.name]
    n = 0
    for src, name, d in load(args.dir, passes, args.func):
        for hit in fn(src, name, d):
            n += 1
            print("%s: %s: %s" % (src, name, json.dumps(hit)))
            if args.src:
                for k, v in hit.items():
                    if k.endswith("line") and isinstance(v, (list, tuple)) and len(v) == 2:
                        print("    %s %s:%d: %s" % (k, v[0], v[1], source_line(v)))
    print("%d hits" % n, file=sys.stderr)


def main():
    global ARGS
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("cmd", choices=["build", "query", "list"])
    ap.add_argument("name", nargs="?")
    ap.add_argument("--dir", default=os.path.join(ROOT, "build", "rtl_corpus"))
    ap.add_argument("-j", type=int, default=os.cpu_count())
    ap.add_argument("--func", help="only functions whose name contains this")
    ap.add_argument("--src", action="store_true", help="print the C source line of each hit")
    ARGS = args = ap.parse_args()
    args.dir = os.path.abspath(args.dir)
    if args.cmd == "build":
        cmd_build(args)
    elif args.cmd == "list":
        for k, (f, _) in QUERIES.items():
            print("%-22s %s" % (k, f.__doc__.split("\n")[0]))
    else:
        cmd_query(args)


if __name__ == "__main__":
    main()
