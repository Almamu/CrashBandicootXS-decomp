#!/usr/bin/env python3
"""Index the matched corpus at the RTL level and query it (#662).

    tools/rtl_corpus.py build [-j N] [--dir D] [--only RE] [--no-dumps]
    tools/rtl_corpus.py list                       the queries
    tools/rtl_corpus.py query NAME [--dir D] [--func F] [--src]
                        [--file RE] [--grep RE] [--matched] [--limit N]

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
whose RTL the workaround shaped; `--matched` leaves those out).

Two kinds of query (`list` shows both):
- dump queries (QUERIES) read the dumps when asked, so they need them
  kept (the default);
- index queries (INDEX_KINDS) are computed per object at build time and
  stored in D/index/*.json (about 15 MB), so they still work after
  `--no-dumps` (which deletes each object's dumps once indexed).

The parser keeps, per function and pass, each insn's uid, kind
(insn/jump_insn/call_insn/code_label/note), its text, the source file
and line in force (the line-number notes `-g` adds) and its basic
block. The queries are plain Python over that; add one per new
situation. `--src` prints the C line behind each hit; `--grep`
filters hits by their text, `--file` by source file.
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
        # the object's own name (libgcc2.c builds one object per function)
        obj = os.path.relpath(os.path.splitext(cc1[i + 1])[0], "build/crashbandicootxs")
        del cc1[i : i + 2]
        yield src[0], obj, clean, cc1


def index_path(d, obj):
    return os.path.join(d, "index", obj.replace("/", "__") + ".json")


def build_one(d, src, obj, cpp, cc1, keep):
    stem = os.path.join(d, obj)
    os.makedirs(os.path.dirname(stem), exist_ok=True)
    ii = stem + ".ii"
    with open(ii, "wb") as f:
        r = subprocess.run(cpp, cwd=ROOT, stdout=f, stderr=subprocess.DEVNULL)
    if r.returncode:
        return src, "cpp failed"
    # -g warns about -fomit-frame-pointer on ARM, and -Werror would stop it
    cc1 = [a for a in cc1 if a != "-Werror"]
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
    idx = index_dumps(ii)
    os.makedirs(os.path.join(d, "index"), exist_ok=True)
    with open(index_path(d, obj), "w") as f:
        json.dump({"src": src, "compiler": os.path.basename(cc1[0]), "features": idx}, f)
    if not keep:
        base = os.path.basename(ii)
        for name in os.listdir(os.path.dirname(ii)):
            if name == base or name.startswith(base + ".") or name == os.path.basename(stem) + ".s":
                os.unlink(os.path.join(os.path.dirname(ii), name))
    return src, None


def cmd_build(args):
    jobs = [j for j in compile_lines() if not args.only or re.search(args.only, j[0])]
    with ThreadPoolExecutor(args.j) as ex:
        for src, err in ex.map(lambda j: build_one(args.dir, *j, not args.no_dumps), jobs):
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
    block = -1
    buf = []
    if not os.path.exists(path):
        return funcs

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
        funcs[cur].append({"k": kind, "u": uid, "t": t, "l": line, "b": block})

    with open(path, errors="replace") as f:
        for raw in f:
            if raw.startswith(";; Function "):
                flush()
                buf = []
                cur = raw[len(";; Function ") :].strip()
                funcs[cur] = []
                line = ("", 0)
                block = -1
                continue
            bb = re.match(r"^;; Start of basic block (\d+)", raw)
            if bb:
                flush()
                buf = []
                block = int(bb.group(1))
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


# ---------------------------------------------------------------- index

# The index queries (#662 round 8), computed per object at build time.
# Their parsed forms are (kind, uid, text, (file, line), block) tuples.

FUNC_RE = re.compile(r'^;; Function (.*)$')
REGNUM_RE = re.compile(r'\(reg(?:/\w+)*:(\w+) (\d+)')


def pretty_name(sig):
    """`s32 LinkSession::Update()` -> `LinkSession::Update`."""
    s = sig.split('(')[0].strip()
    s = s.split()[-1] if s else s
    return s.lstrip('*&')


def forms(path):
    """parse_dump's insns as tuples, for the index features."""
    return {fn: [(x["k"], x["u"], x["t"], x["l"], x["b"]) for x in xs] for fn, xs in parse_dump(path).items()}


def set_dest_src(text):
    """For `(insn N P Q (set DEST SRC) ...)`: (DEST, SRC) texts, else None."""
    i = text.find('(set ')
    if i < 0 or text.find('(parallel') >= 0:
        return None
    j = i + 5
    # DEST
    d0 = j
    depth = 0
    while j < len(text):
        c = text[j]
        if c == '(':
            depth += 1
        elif c == ')':
            depth -= 1
            if depth == 0:
                j += 1
                break
        j += 1
    dest = text[d0:j].strip()
    k = j
    depth = 0
    s0 = k
    while k < len(text):
        c = text[k]
        if c == '(':
            depth += 1
        elif c == ')':
            if depth == 0:
                break
            depth -= 1
            if depth == 0:
                k += 1
                break
        k += 1
    src = text[s0:k].strip()
    return dest, src


def regs_in(text):
    return {int(n) for _, n in REGNUM_RE.findall(text)}


def short(text, n=160):
    text = re.sub(r'\s+', ' ', text)
    return text if len(text) <= n else text[:n] + '...'


def loc_str(loc):
    return '%s:%d' % (loc[0], loc[1]) if loc[0] else '?'


# ---------------------------------------------------------------- features

CONST_SET_RE = re.compile(r'^\(set \(reg(?:/\w+)*:(\w+) (\d+)\) \(const_int (-?\d+)')


def f_narrow_const(rtl, cse):
    out = []
    for fn, forms in rtl.items():
        si_consts = {}
        narrow = []
        for kind, uid, text, loc, blk in forms:
            if kind != 'insn':
                continue
            ds = set_dest_src(text)
            if not ds:
                continue
            m = re.match(r'\(reg(?:/\w+)*:(\w+) (\d+)\)$', ds[0])
            c = re.match(r'\(const_int (-?\d+)', ds[1])
            if not m or not c or ds[1].count('(') != 1:
                continue
            mode, reg, k = m.group(1), int(m.group(2)), int(c.group(1))
            if mode == 'SI':
                si_consts.setdefault(k, []).append(reg)
            elif mode in ('QI', 'HI'):
                narrow.append((mode, reg, k, uid, loc))
        cforms = cse.get(fn, [])
        for mode, reg, k, uid, loc in narrow:
            pat = re.compile(r'\(reg(?:/\w+)*:\w+ %d\)' % reg)
            uses = []
            for kind, u, text, l2, blk in cforms:
                if u == uid or kind not in ('insn', 'jump_insn', 'call_insn'):
                    continue
                if pat.search(text):
                    uses.append('%s %s' % (loc_str(l2), short(text, 120)))
            out.append({'func': fn, 'loc': loc, 'detail': '%sImode %d (reg %d); SImode same value: %s; uses after cse: %d' % (
                mode[0], k, reg, 'yes' if k in si_consts else 'no', len(uses)), 'uses': uses[:8]})
    return out


def block_scan(forms):
    blocks = {}
    for f in forms:
        blocks.setdefault(f[4], []).append(f)
    return blocks


def f_const_regs(lreg):
    out = []
    for fn, forms in lreg.items():
        for blk, fs in block_scan(forms).items():
            byk = {}
            for kind, uid, text, loc, b in fs:
                if kind != 'insn':
                    continue
                ds = set_dest_src(text)
                if not ds:
                    continue
                m = re.match(r'\(reg(?:/\w+)*:(\w+) (\d+)', ds[0])
                c = re.match(r'\(const_int (-?\d+)', ds[1])
                if m and c and ds[1].count('(') == 1:
                    byk.setdefault(int(c.group(1)), []).append((m.group(1), int(m.group(2)), uid, loc))
            for k, sets in byk.items():
                regs = {r for _, r, _, _ in sets}
                if len(regs) < 2:
                    continue
                desc = []
                for mode, r, uid, loc in sets:
                    pat = re.compile(r'\(reg(?:/\w+)*:\w+ %d\)' % r)
                    uses = [loc_str(l) for kd, u, t, l, b in fs if u != uid and kd != 'note' and pat.search(t)]
                    desc.append('reg %d:%s set %s used %s' % (r, mode, loc_str(loc), ','.join(uses) or '-'))
                out.append({'func': fn, 'loc': sets[0][3], 'detail': 'const %d in %d regs, block %d: %s' % (k, len(regs), blk, '; '.join(desc))})
    return out


def f_dead_load(final):
    out = []
    for fn, forms in final.items():
        for blk, fs in block_scan(forms).items():
            insns = [f for f in fs if f[0] in ('insn', 'jump_insn', 'call_insn', 'code_label')]
            for i, (kind, uid, text, loc, b) in enumerate(insns):
                if kind != 'insn':
                    continue
                ds = set_dest_src(text)
                if not ds or not ds[1].startswith('(mem'):
                    continue
                m = re.match(r'\(reg(?:/\w+)*:\w+ (\d+)', ds[0])
                if not m:
                    continue
                r = int(m.group(1))
                pat = re.compile(r'\(reg(?:/\w+)*:\w+ %d [a-z0-9]+\)' % r)
                for kind2, uid2, t2, l2, b2 in insns[i + 1:]:
                    if kind2 != 'insn':
                        break
                    d2 = set_dest_src(t2)
                    if d2 and re.match(r'\(reg(?:/\w+)*:\w+ %d\b' % r, d2[0]) and not pat.search(d2[1]):
                        out.append({'func': fn, 'loc': loc, 'detail': 'r%d loaded (%s), set again at %s unread' % (
                            r, short(ds[1], 80), loc_str(l2))})
                        break
                    if pat.search(t2):
                        break
    return out


def f_regmove(combine, regmove):
    out = []
    for fn, forms in regmove.items():
        before = {uid: (text, loc) for kind, uid, text, loc, b in combine.get(fn, []) if kind in ('insn', 'jump_insn')}
        for kind, uid, text, loc, b in forms:
            if kind not in ('insn', 'jump_insn') or uid not in before:
                continue
            t0 = re.sub(r'\s+', ' ', before[uid][0].split(' (nil)')[0])
            t1 = re.sub(r'\s+', ' ', text.split(' (nil)')[0])
            p0 = re.sub(r'^\(\w+ \d+ \d+ \d+ ', '', t0)
            p1 = re.sub(r'^\(\w+ \d+ \d+ \d+ ', '', t1)
            if p0.split(' -1')[0] != p1.split(' -1')[0]:
                out.append({'func': fn, 'loc': loc, 'detail': 'insn %d: %s  =>  %s' % (uid, short(p0, 110), short(p1, 110))})
    return out


SORTED_RE = re.compile(r'^Register (\d+), refs = (\d+), live_length = (\d+)')
LREG_INFO_RE = re.compile(r'^Register (\d+) used (\d+) times across (\d+) insns(.*)$')


def f_alloc(greg_path, lreg_path, lreg):
    out = []
    if not os.path.exists(greg_path):
        return out
    sorted_regs = {}
    disp = {}
    infos = {}
    fn = None
    in_disp = False
    with open(greg_path, errors='replace') as f:
        for line in f:
            m = FUNC_RE.match(line)
            if m:
                fn = m.group(1)
                sorted_regs[fn] = []
                disp[fn] = {}
                in_disp = False
                continue
            m = SORTED_RE.match(line)
            if m and fn:
                sorted_regs[fn].append((int(m.group(1)), int(m.group(2)), int(m.group(3))))
                continue
            if line.startswith(';; Register dispositions'):
                in_disp = True
                continue
            if in_disp:
                if not line.strip():
                    in_disp = False
                    continue
                for a, b in re.findall(r'(\d+) in (\d+)', line):
                    disp[fn][int(a)] = int(b)
    with open(lreg_path, errors='replace') as f:
        fn = None
        for line in f:
            m = FUNC_RE.match(line)
            if m:
                fn = m.group(1)
                infos[fn] = {}
                continue
            m = LREG_INFO_RE.match(line)
            if m and fn:
                infos[fn][int(m.group(1))] = line.strip()
    for fn, regs in sorted_regs.items():
        first_set = {}
        for kind, uid, text, loc, b in lreg.get(fn, []):
            if kind != 'insn':
                continue
            ds = set_dest_src(text)
            if ds:
                m = re.match(r'\(reg(?:/\w+)*:\w+ (\d+)\)', ds[0])
                if m and int(m.group(1)) not in first_set:
                    first_set[int(m.group(1))] = loc
        for rank, (r, refs, ll) in enumerate(regs):
            lg = refs.bit_length() - 1 if refs > 0 else 0
            prio = (lg * refs / ll) if ll else 0.0
            info = infos.get(fn, {}).get(r, '')
            out.append({'func': fn, 'loc': first_set.get(r, (None, 0)), 'detail': 'reg %d rank %d refs %d live %d prio %.3f -> %s; %s' % (
                r, rank, refs, ll, prio, ('r%d' % disp[fn][r]) if r in disp.get(fn, {}) else 'mem/local', info.split(';', 1)[1].strip() if ';' in info else '')})
    return out


PRE_RE = re.compile(r'^PRE: redundant insn (\d+) \(expression (\d+)\) in bb (\d+), reaching reg is (\d+)')
HOIST_RE = re.compile(r'^PRE/HOIST: (.*)$')


def f_pre(gcse_path, gcse):
    out = []
    if not os.path.exists(gcse_path):
        return out
    fn = None
    with open(gcse_path, errors='replace') as f:
        lines = f.readlines()
    locs = {}
    for name, forms in gcse.items():
        locs[name] = {uid: (loc, text) for kind, uid, text, loc, b in forms}
    for line in lines:
        m = FUNC_RE.match(line)
        if m:
            fn = m.group(1)
            continue
        m = PRE_RE.match(line)
        if m and fn:
            uid = int(m.group(1))
            loc, text = locs.get(fn, {}).get(uid, ((None, 0), ''))
            out.append({'func': fn, 'loc': loc, 'detail': 'redundant insn %d (expr %s, bb %s) -> reg %s: %s' % (
                uid, m.group(2), m.group(3), m.group(4), short(text, 100))})
            continue
        m = HOIST_RE.match(line)
        if m and fn:
            out.append({'func': fn, 'loc': (None, 0), 'detail': 'insert: ' + m.group(1).strip()})
    return out


def f_reload_del(lreg, greg):
    out = []
    for fn, forms in lreg.items():
        after = {uid for kind, uid, text, loc, b in greg.get(fn, [])}
        for kind, uid, text, loc, b in forms:
            if kind != 'insn' or uid in after:
                continue
            ds = set_dest_src(text)
            if not ds:
                continue
            if ds[0].startswith('(mem') or ds[1].startswith('(mem'):
                out.append({'func': fn, 'loc': loc, 'detail': 'insn %d deleted after allocation: %s' % (uid, short(text, 120))})
    return out


def index_dumps(base):
    """The index features of one object's dumps (BASE.<pass>)."""
    rtl = forms(base + '.rtl')
    cse = forms(base + '.cse')
    combine = forms(base + '.combine')
    regmove = forms(base + '.regmove')
    lreg = forms(base + '.lreg')
    greg = forms(base + '.greg')
    gcse = forms(base + '.gcse')
    final = forms(base + '.jump2') or greg
    return {
        'narrow_const': f_narrow_const(rtl, cse),
        'const_regs': f_const_regs(lreg),
        'dead_load': f_dead_load(final),
        'regmove': f_regmove(combine, regmove),
        'alloc': f_alloc(base + '.greg', base + '.lreg', lreg),
        'pre': f_pre(base + '.gcse', gcse),
        'reload_del': f_reload_del(lreg, greg),
    }


INDEX_KINDS = {
    'narrow_const': 'a QImode/HImode constant made at expand (a failed narrow op) and its uses after cse',
    'const_regs': 'one constant in two or more pseudos in one block after local-alloc, with their uses',
    'dead_load': 'a load whose hard register is set again unread in its block (final RTL)',
    'regmove': 'an insn regmove rewrote (copy moved onto a test, operand retargeted): before => after',
    'alloc': "every pseudo global-alloc saw: rank, refs, live length, priority, hard reg, lreg's notes",
    'pre': 'a gcse PRE deletion (with the reaching register) or insertion',
    'reload_del': 'a load or store present before global-alloc and gone after it',
}


def workaround_functions():
    out = subprocess.run([sys.executable, os.path.join(ROOT, 'tools/match_idioms.py'), '--functions', '--files'],
                         cwd=ROOT, capture_output=True, text=True).stdout
    funcs = set()
    for line in out.splitlines():
        m = re.match(r'^(\S+): ([^:]+): ', line)
        if m:
            funcs.add(m.group(2).strip())
    return funcs


_src_cache = {}


def src_line(path, n):
    if not path:
        return ''
    p = os.path.join(ROOT, path)
    if p not in _src_cache:
        try:
            _src_cache[p] = open(p, errors='replace').read().splitlines()
        except OSError:
            _src_cache[p] = []
    lines = _src_cache[p]
    return lines[n - 1].strip() if 0 < n <= len(lines) else ''


def source_line(loc):
    path, line = loc
    return src_line(path, line)


def cmd_query(args):
    excluded = workaround_functions() if args.matched else set()
    n = 0

    def keep(src, name, text):
        if args.file and not re.search(args.file, src):
            return False
        if args.func and args.func not in name:
            return False
        if pretty_name(name) in excluded:
            return False
        return not args.grep or re.search(args.grep, text)

    if args.name in QUERIES:
        fn, passes = QUERIES[args.name]
        for src, name, d in load(args.dir, passes, args.func):
            for hit in fn(src, name, d):
                text = json.dumps(hit)
                if not keep(src, name, text):
                    continue
                n += 1
                print("%s: %s: %s" % (src, name, text))
                if args.src:
                    for k, v in hit.items():
                        if k.endswith("line") and isinstance(v, (list, tuple)) and len(v) == 2:
                            print("    %s %s:%d: %s" % (k, v[0], v[1], source_line(v)))
                if args.limit and n >= args.limit:
                    print("%d hits" % n, file=sys.stderr)
                    return
    elif args.name in INDEX_KINDS:
        idir = os.path.join(args.dir, "index")
        if not os.path.isdir(idir):
            sys.exit("no index in %s: run `build` first" % args.dir)
        for fname in sorted(os.listdir(idir)):
            data = json.load(open(os.path.join(idir, fname)))
            for hit in data["features"].get(args.name, []):
                text = hit["detail"] + " " + " ".join(hit.get("uses", []))
                if not keep(data["src"], hit["func"], text):
                    continue
                n += 1
                loc = hit["loc"] or ["", 0]
                print("%s: %s: %s:%d: %s" % (data["src"], pretty_name(hit["func"]), loc[0] or "?", loc[1], src_line(loc[0], loc[1])))
                print("    " + hit["detail"])
                for u in hit.get("uses", []):
                    print("      use " + u)
                if args.limit and n >= args.limit:
                    print("%d hits" % n, file=sys.stderr)
                    return
    else:
        sys.exit("unknown query %r (see `list`)" % args.name)
    print("%d hits" % n, file=sys.stderr)


def main():
    global ARGS
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("cmd", choices=["build", "query", "list"])
    ap.add_argument("name", nargs="?")
    ap.add_argument("--dir", default=os.path.join(ROOT, "build", "rtl_corpus"))
    ap.add_argument("-j", type=int, default=os.cpu_count())
    ap.add_argument("--only", help="build: only sources matching this regex")
    ap.add_argument("--no-dumps", action="store_true", help="build: keep only the index (dump queries need the dumps)")
    ap.add_argument("--func", help="only functions whose name contains this")
    ap.add_argument("--file", help="only sources matching this regex")
    ap.add_argument("--grep", help="only hits whose text matches this regex")
    ap.add_argument("--matched", action="store_true", help="leave out functions that still have a workaround")
    ap.add_argument("--limit", type=int, default=0)
    ap.add_argument("--src", action="store_true", help="print the C source line of each hit")
    ARGS = args = ap.parse_args()
    args.dir = os.path.abspath(args.dir)
    if args.cmd == "build":
        cmd_build(args)
    elif args.cmd == "list":
        print("dump queries (need the dumps):")
        for k, (f, _) in QUERIES.items():
            print("  %-22s %s" % (k, f.__doc__.split("\n")[0]))
        print("index queries (D/index):")
        for k, v in INDEX_KINDS.items():
            print("  %-22s %s" % (k, v))
    else:
        cmd_query(args)


if __name__ == "__main__":
    main()
