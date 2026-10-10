#!/usr/bin/env python3
"""Enumerate natural rewrites of one function and score each against the ROM.

    tools/natural_enum.py SPEC.py [options]
    tools/natural_enum.py TEMPLATE --src SRC --sig 'SIGLINE' --sym SYM
                          [--ref OBJ] [options]

The decomp-permuter finds matches, but mostly with junk nobody would write
(#662). This is its opposite: it only tries the rewrites it is given, and
those are only things a programmer plausibly writes (a local's type or
signedness, statement order, where a local is declared, splitting or
merging locals, a sub-expression as an inline helper, early return vs
if/else, loop forms, compound assignment, `?:` vs if, a class accessor
instead of the field). The winner still has to be judged by hand: it must
read like the original author's code.

The rewrites come in one of two forms.

A spec is a Python file defining:

    SOURCE   = "src/bosses/hovercraft.cpp"   # the file the function is in
    SYMBOL   = "ConvertHovercraftTiles"      # its symbol in the object
    TARGET   = "p8r/target/hovercraft.o"     # an object with the ROM's code
    REGION   = '''...'''                     # exact text of SOURCE to replace
    TEMPLATE = '''...{axis}...'''            # REGION with {axis} slots
    AXES     = {"axis": ["default", "alternative", ...], ...}
    # optional: CONSTRAINT(choice) -> bool, choice = {axis: index}
    # optional: EXTRA_FLAGS = ["-f..."], PRELUDE = "text put before REGION"

Literal `{`/`}` in TEMPLATE are written `{{`/`}}`. An axis value can
itself contain `{other}` slots (and no other braces).

A template (any other file) is the function's text, from the line equal
to --sig to the next "\\n}\\n" of --src (helpers before it may be
included), with inline slots:

    @{a|b|c}@        one of the alternatives (an empty one is allowed)
    @name{a|b|c}@    every slot with this name takes the same index
    @T{s32}@         a type: s8, u8, s16, u16, s32, u32 (the given one first)
    @Tname{s32}@     a type slot tied by name

Slots nest; write `\\|` and `\\}` for a literal `|` and `}`.

Search. Without -k, every combination is tried when there are at most
--limit of them, else a fixed-seed sample of --limit (--sample forces
the sample). With -k K, every combination of up to K changed axes is
tried, then --rounds rounds of a beam search (width --beam) from the
best. Identical renderings are compiled once, and results are cached by
the rendered text in the work directory (build/natural_enum/<name>).

The compile command is the Makefile's (`make -n`), so the object's
compiler and flags are used. The score is the number of differing
instructions (normalised disassembly: branch targets relative to the
function, pool words by relocation; `--metric lines` counts a unified
diff's -/+ lines instead, as round 8's link/ sweeps did); 0 is a match.
--all also scores the object's other functions (collateral). --dump
writes every result as TSV (score, collateral, each axis's text).

A third form, an edit-list spec, lists edits instead of slots (the edit-list
workflow; the spec is exec'd from the current directory, so it may load
and extend another spec):

    src    = "src/iwram/string_arm.cpp"   # the source file
    obj    = "src/iwram/string_arm"       # its object, under build/crashbandicootxs/
    func   = "strncpy_arm"                # the symbol scored
    extra  = ["strlen_arm", ...]          # optional: other symbols that must stay
    base   = [(old, new), ...]            # takes the workarounds out
    alts   = [("name", [(old, new), ...]), ...]  # one natural edit each
    region = (start, end)                 # optional: bounds for the automatic edits
    auto   = ("type", "compound", "swap") # optional: which automatic edits
    cflags = [...]; drop_werror = True    # optional: test a flag or a warning-only form
    target = "path/to/object.o"           # optional: default the build's object

The target defaults to the matching build's object (run `make` first).
Besides the hand-written `alts`, it generates edits itself: every
integer local or parameter retyped (s8/u8/s16/u16/s32/u32), `x op= e`
<-> `x = x op e`, and two adjacent independent statements swapped.
Every combination of up to -k edits is compiled (no beam search); the
score adds up the instructions off over `func` and `extra`. `--keep DIR`
writes the best variants, `--max` caps the combinations, `--no-auto`
keeps only `alts`. Set NENUM_TMP to put its temporary directories
outside /tmp.
"""

import argparse
import concurrent.futures
import difflib
import hashlib
import importlib.util
import itertools
import json
import os
import random
import re
import shlex
import shutil
import subprocess
import sys
import tempfile
import threading
from concurrent.futures import ThreadPoolExecutor

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TYPES = ["s8", "u8", "s16", "u16", "s32", "u32"]

# ---------------------------------------------------------------- sources


def load_spec(path):
    spec = importlib.util.spec_from_file_location("spec", path)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


class SpecSource:
    """A SPEC.py: named axes, `{axis}` slots, a REGION of SOURCE."""

    def __init__(self, path):
        self.spec = spec = load_spec(path)
        self.source = spec.SOURCE
        self.symbol = spec.SYMBOL
        self.target = spec.TARGET
        self.region = spec.REGION
        self.extra_flags = list(getattr(spec, "EXTRA_FLAGS", []))
        self.axes = list(spec.AXES)
        self.sizes = {a: len(spec.AXES[a]) for a in self.axes}
        self.constraint = getattr(spec, "CONSTRAINT", lambda c: True)

    def values(self, choice):
        vals = {k: self.spec.AXES[k][i] for k, i in choice.items()}

        def fill(text):
            # axis values may name other axes; they have no literal braces
            return re.sub(r"\{([a-z_0-9]+)\}", lambda m: fill(vals[m.group(1)]), text)

        return {k: fill(v) for k, v in vals.items()}

    def render(self, choice):
        return self.spec.TEMPLATE.format(**self.values(choice))

    def axis_text(self, choice, axis):
        return self.values(choice)[axis]


OPEN_RE = re.compile(r"@(T?)(\w*)\{")


def parse_seq(text, pos, slots, stop):
    """Literal text and slots up to a `|` or `}@` (when stop) or the end.
    -> (pieces, pos); a piece is a string or a slot index."""
    pieces = []
    buf = []
    while pos < len(text):
        if text.startswith("\\|", pos) or text.startswith("\\}", pos):
            buf.append(text[pos + 1])
            pos += 2
            continue
        if stop and (text.startswith("}@", pos) or text[pos] == "|"):
            break
        m = OPEN_RE.match(text, pos)
        if m:
            pieces.append("".join(buf))
            buf = []
            is_type, name = m.group(1), m.group(2)
            pos = m.end()
            alts = []
            while True:
                alt, pos = parse_seq(text, pos, slots, True)
                alts.append(alt)
                if text.startswith("}@", pos):
                    pos += 2
                    break
                if pos >= len(text):
                    sys.exit("unterminated slot")
                pos += 1  # '|'
            if is_type:
                first = "".join(x for x in alts[0] if isinstance(x, str)).strip() or "s32"
                alts = [[first]] + [[t] for t in TYPES if t != first]
            pieces.append(len(slots))
            slots.append(((("T" if is_type else "") + name) if name else None, alts))
            continue
        buf.append(text[pos])
        pos += 1
    pieces.append("".join(buf))
    return pieces, pos


class TemplateSource:
    """An inline-slot template of one function (see the module doc)."""

    def __init__(self, path, src, sig, sym, ref):
        if not (src and sig and sym):
            sys.exit("a template needs --src, --sig and --sym")
        self.source = src
        self.symbol = sym
        self.target = ref or os.path.join("build/crashbandicootxs", os.path.splitext(src)[0] + ".o")
        text = open(os.path.join(ROOT, src)).read()
        i = text.index("\n" + sig + "\n") + 1
        j = text.index("\n}\n", i) + 3
        self.region = text[i:j]
        self.extra_flags = []
        self.constraint = lambda c: True
        self.slots = []
        self.pieces, _ = parse_seq(open(path).read(), 0, self.slots, False)
        # tied slots share one axis
        self.slot_axis = []
        self.axes = []
        self.sizes = {}
        for k, (name, alts) in enumerate(self.slots):
            axis = name or "s%d" % k
            if axis in self.sizes:
                if self.sizes[axis] != len(alts):
                    sys.exit("slot %s: tied slots need the same number of alternatives" % axis)
            else:
                self.axes.append(axis)
                self.sizes[axis] = len(alts)
            self.slot_axis.append(axis)

    def _render(self, pieces, choice):
        out = []
        for p in pieces:
            if isinstance(p, int):
                out.append(self._render(self.slots[p][1][choice[self.slot_axis[p]]], choice))
            else:
                out.append(p)
        return "".join(out)

    def render(self, choice):
        body = self._render(self.pieces, choice)
        return body if body.endswith("\n") else body + "\n"

    def axis_text(self, choice, axis):
        k = self.slot_axis.index(axis)
        return self._render(self.slots[k][1][choice[axis]], choice)


# ---------------------------------------------------------------- compile


def make_commands(source):
    """The Makefile's cpp and cc1 command lines for SOURCE's object."""
    obj = "build/crashbandicootxs/" + os.path.splitext(source)[0] + ".o"
    out = subprocess.run(["make", "-n", "-W", source, obj], cwd=ROOT, capture_output=True, text=True).stdout
    for line in out.splitlines():
        if "|" in line and source in line and "agbc" in line:
            cpp, cc1 = line.split("|", 1)
            cpp = shlex.split(cpp)
            cc1 = shlex.split(cc1)
            clean = []
            skip = False
            for a in cpp:
                if skip:
                    skip = False
                    continue
                if a in ("-MF", "-MT"):
                    skip = True
                    continue
                if a in ("-MMD", "-MP") or a == source:
                    continue
                clean.append(a)
            i = cc1.index("-o")
            del cc1[i : i + 2]
            return clean, cc1
    sys.exit("no compile command for %s:\n%s" % (source, out))


_CXX = None


def cxx_symbols():
    """cxx_symbols.txt: mangled name -> the C name the build renames it to."""
    global _CXX
    if _CXX is None:
        _CXX = {}
        try:
            with open(os.path.join(ROOT, "cxx_symbols.txt")) as f:
                for line in f:
                    p = line.split()
                    if len(p) == 2:
                        _CXX[p[0]] = p[1]
        except OSError:
            pass
    return _CXX


def objdump_all(obj):
    """{C symbol name: normalised instruction lines} for every function."""
    out = subprocess.run(["arm-none-eabi-objdump", "-dr", "--no-show-raw-insn", obj], capture_output=True, text=True).stdout
    funcs = {}
    lines = None
    start = 0
    for line in out.splitlines():
        m = re.match(r"^([0-9a-f]+) <(.+)>:$", line)
        if m:
            lines = funcs.setdefault(cxx_symbols().get(m.group(2), m.group(2)), [])
            start = int(m.group(1), 16)
            continue
        if lines is None or not line.strip():
            continue
        rm = re.match(r"^\s*[0-9a-f]+: (R_\S+)\s+(\S+)", line)
        if rm:
            if lines:
                lines[-1] += " [" + cxx_symbols().get(rm.group(2), rm.group(2)) + "]"
            continue
        m = re.match(r"^\s*([0-9a-f]+):\s*(.*)$", line)
        if m:
            text = m.group(2).split(";")[0].split("@")[0].rstrip()
            text = re.sub(r"\s+", " ", text)
            bm = re.match(r"^(b\S*) ([0-9a-f]+)( <[^>]*>)?$", text)
            if bm and bm.group(1) != "bl":
                text = "%s .%+x" % (bm.group(1), int(bm.group(2), 16) - start)
            elif bm and bm.group(3):
                # a call within the object: by name, not by address
                name = bm.group(3)[2:-1].split("+")[0]
                text = "bl " + cxx_symbols().get(name, name)
            text = re.sub(r" <[^>]*>", "", text)
            lines.append(text)
    return funcs


def objdump_func(obj, symbol):
    return objdump_all(obj).get(symbol, [])


def score_insns(a, b):
    sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
    return sum(max(i2 - i1, j2 - j1) for tag, i1, i2, j1, j2 in sm.get_opcodes() if tag != "equal")


def score_lines(a, b):
    return sum(1 for l in difflib.unified_diff(a, b, lineterm="", n=0) if l[:1] in "+-" and not l.startswith(("+++", "---")))


class Runner:
    def __init__(self, src, work, metric, collateral):
        self.src = src
        self.work = work
        os.makedirs(work, exist_ok=True)
        self.cpp, self.cc1 = make_commands(src.source)
        self.cc1 += src.extra_flags
        with open(os.path.join(ROOT, src.source)) as f:
            self.text = f.read()
        if src.region not in self.text:
            sys.exit("REGION not found in " + src.source)
        self.ref = objdump_all(os.path.join(ROOT, src.target))
        self.target = self.ref.get(src.symbol)
        if not self.target:
            sys.exit("symbol %s not in %s" % (src.symbol, src.target))
        self.score = score_lines if metric == "lines" else score_insns
        self.collateral = collateral
        self.mode = "%s%s" % (metric, "+all" if collateral else "")
        self.keep_diff = False
        self.lock = threading.Lock()
        self.cache_path = os.path.join(work, "cache.json")
        try:
            with open(self.cache_path) as f:
                self.cache = json.load(f)
        except (OSError, ValueError):
            self.cache = {}

    def key(self, body):
        return hashlib.sha1((self.mode + "\0" + body).encode()).hexdigest()

    def compile(self, body):
        """-> [score, output hash, collateral] or None."""
        key = self.key(body)
        with self.lock:
            if key in self.cache:
                return self.cache[key]
        src = self.text.replace(self.src.region, body, 1)
        d = tempfile.mkdtemp(dir=self.work)
        res = None
        try:
            cppf = os.path.join(d, "v" + os.path.splitext(self.src.source)[1])
            with open(cppf, "w") as f:
                f.write(src)
            srcdir = os.path.dirname(os.path.join(ROOT, self.src.source))
            ii = subprocess.run(self.cpp[:1] + ["-iquote", srcdir] + self.cpp[1:] + [cppf], cwd=ROOT, capture_output=True)
            if not ii.returncode:
                s = os.path.join(d, "v.s")
                o = os.path.join(d, "v.o")
                cc = subprocess.run(self.cc1 + ["-o", s], input=ii.stdout, cwd=ROOT, capture_output=True)
                if not cc.returncode:
                    # the Makefile's ZERO_PAD_TEXT (#663)
                    with open(s, "a") as f:
                        f.write("\t.text\n\t.align\t2, 0\n")
                if not cc.returncode and not subprocess.run(
                    ["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", o, s], capture_output=True
                ).returncode:
                    funcs = objdump_all(o)
                    got = funcs.get(self.src.symbol, [])
                    other = 0
                    if self.collateral:
                        other = sum(self.score(v, funcs.get(k, [])) for k, v in self.ref.items() if k != self.src.symbol)
                    res = [self.score(self.target, got), hashlib.sha1("\n".join(got).encode()).hexdigest()[:12], other]
                    if self.keep_diff:
                        for k, v in self.ref.items():
                            if k != self.src.symbol and self.collateral and v != funcs.get(k, []):
                                print("collateral: %s, %d" % (k, self.score(v, funcs.get(k, []))))
                        sys.stdout.writelines(
                            difflib.unified_diff([x + "\n" for x in self.target], [x + "\n" for x in got], "rom", "variant", n=2)
                        )
        finally:
            subprocess.run(["rm", "-rf", d])
        with self.lock:
            self.cache[key] = res
        return res

    def save(self):
        with open(self.cache_path, "w") as f:
            json.dump(self.cache, f)


# ---------------------------------------------------------------- search


# ---------------------------------------------------------------- edit-list specs

INT_TYPES = ('s8', 'u8', 's16', 'u16', 's32', 'u32')


def edits_compile_line(obj):
    out = subprocess.run(['make', '-n', '-B', f'build/crashbandicootxs/{obj}.o'], cwd=ROOT,
                         capture_output=True, text=True).stdout
    for line in out.splitlines():
        if '| tools/agbcc/bin/' in line:
            return line
    raise SystemExit(f'no compile line for {obj}')


def edits_func_lines(o, func):
    out = subprocess.run(['arm-none-eabi-objdump', '-dr', '--no-show-raw-insn', o], capture_output=True,
                         text=True).stdout
    lines = []
    on = False
    for l in out.splitlines():
        m = re.match(r'^[0-9a-f]+ <(.+)>:$', l)
        if m:
            on = m.group(1) == func
            continue
        if not on or not l.strip():
            continue
        l = re.sub(r'^\s*[0-9a-f]+:\s*', '', l)
        l = re.sub(r'\s+[0-9a-f]+ <[^>]*>', '', l)  # branch target addresses
        l = re.sub(r'@ \(.*\)|; \(.*\)', '', l).strip()
        lines.append(l)
    # drop trailing alignment padding
    while lines and lines[-1] in ('nop', '.word\t0x00000000', 'movs\tr0, r0', 'lsls\tr0, r0, #0'):
        lines.pop()
    return lines


class EditsCtx:
    def __init__(self, spec):
        self.spec = spec
        self.line = edits_compile_line(spec['obj'])
        cpp, cc = self.line.split(' | ', 1)
        cpp = shlex.split(cpp)
        args = []
        skip = 0
        for a in cpp:
            if skip:
                skip -= 1
                continue
            if a in ('-MF', '-MT'):
                skip = 1
                continue
            if a in ('-MMD', '-MP'):
                continue
            args.append(a)
        self.cpp = args[:-1] + ['-iquote', os.path.dirname(spec['src'])]
        cc = shlex.split(cc)
        oi = cc.index('-o')
        self.cc = [os.path.join(ROOT, cc[0])] + cc[1:oi] + list(spec.get('cflags', []))
        if spec.get('drop_werror'):
            self.cc = [a for a in self.cc if a != '-Werror']
        self.cxx = '-x' in args
        ref = os.path.join(ROOT, spec.get('target') or 'build/crashbandicootxs/' + spec['obj'] + '.o')
        self.funcs = [spec['func']] + list(spec.get('extra', []))
        self.target = {f: edits_func_lines(ref, f) for f in self.funcs}
        for f, t in self.target.items():
            if not t:
                raise SystemExit(f'{f}: not in {ref}')

    def run(self, text):
        d = tempfile.mkdtemp(prefix='nenum', dir=os.environ.get('NENUM_TMP'))
        try:
            src = os.path.join(d, 'v.cpp' if self.cxx else 'v.c')
            open(src, 'w').write(text)
            r = subprocess.run(self.cpp + [src], cwd=ROOT, capture_output=True, text=True)
            if r.returncode:
                return None, 'cpp: ' + r.stderr[-300:]
            r2 = subprocess.run(self.cc + ['-o', os.path.join(d, 'v.s')], input=r.stdout, cwd=ROOT,
                                capture_output=True, text=True)
            if r2.returncode:
                return None, 'cc: ' + r2.stderr[-300:]
            with open(os.path.join(d, 'v.s'), 'a') as f:
                f.write('\t.text\n\t.align\t2, 0\n')
            r3 = subprocess.run(['arm-none-eabi-as', '-mcpu=arm7tdmi', '-mthumb-interwork', '--defsym',
                                 'NON_MATCHING=0', '-o',
                                 os.path.join(d, 'v.o'), os.path.join(d, 'v.s')], capture_output=True,
                                text=True)
            if r3.returncode:
                return None, 'as: ' + r3.stderr[-300:]
            o = os.path.join(d, 'v.o')
            if self.cxx:
                subprocess.run(['arm-none-eabi-objcopy', '--redefine-syms=' + os.path.join(ROOT, 'cxx_symbols.txt'),
                                o], check=True)
            s = 0
            for f in self.funcs:
                s += score_insns(edits_func_lines(o, f), self.target[f])
            return s, None
        finally:
            shutil.rmtree(d, ignore_errors=True)


def edits_apply(text, edits):
    for old, new in edits:
        if old not in text:
            return None
        text = text.replace(old, new, 1)
    return text


def edits_auto_mutations(text, region, kinds):
    """Yield (name, [(old, new)]) for the automatic mutation kinds."""
    if region:
        a = text.index(region[0])
        b = text.index(region[1], a)
        body = text[a:b]
    else:
        body = text
    muts = []
    if 'type' in kinds:
        for m in re.finditer(r'(?m)^(\s*)(' + '|'.join(INT_TYPES) + r') (\w+)( = [^;]*)?;', body):
            for t in INT_TYPES:
                if t != m.group(2):
                    old = m.group(0)
                    muts.append((f'type {m.group(3)}:{t}', [(old, old.replace(m.group(2) + ' ', t + ' ', 1))]))
        for m in re.finditer(r'[(,]\s*(' + '|'.join(INT_TYPES) + r') (\w+)(?=[,)])', body):
            for t in INT_TYPES:
                if t != m.group(1):
                    old = m.group(0)
                    muts.append((f'param {m.group(2)}:{t}', [(old, old.replace(m.group(1) + ' ', t + ' ', 1))]))
    if 'compound' in kinds:
        for m in re.finditer(r'(?m)^(\s*)([\w.>\[\]-]+) ([-+*/|&^]|<<|>>)= ([^;]+);', body):
            old = m.group(0)
            muts.append((f'explicit {m.group(2)}', [(old, f'{m.group(1)}{m.group(2)} = {m.group(2)} '
                                                           f'{m.group(3)} {m.group(4)};')]))
        for m in re.finditer(r'(?m)^(\s*)([\w.>\[\]-]+) = \2 ([-+*/|&^]|<<|>>) ([^;]+);', body):
            old = m.group(0)
            muts.append((f'compound {m.group(2)}', [(old, f'{m.group(1)}{m.group(2)} {m.group(3)}= '
                                                           f'{m.group(4)};')]))
    if 'swap' in kinds:
        lines = body.split('\n')
        for i in range(len(lines) - 1):
            a, b = lines[i], lines[i + 1]
            simple = re.compile(r'^\s+[^{}/*#]*;\s*$')
            if not (simple.match(a) and simple.match(b)):
                continue
            ia = len(a) - len(a.lstrip())
            if ia != len(b) - len(b.lstrip()):
                continue
            if re.search(r'\b(return|break|continue|goto)\b', a + b):
                continue
            # independent: neither writes a name the other mentions
            wa = re.match(r'^\s*([\w]+)', a)
            wb = re.match(r'^\s*([\w]+)', b)
            if wa and wb and (re.search(r'\b' + wa.group(1) + r'\b', b) or re.search(r'\b' + wb.group(1) + r'\b', a)):
                continue
            muts.append((f'swap {a.strip()[:30]}', [(a + '\n' + b, b + '\n' + a)]))
    return muts


def edits_main(a):
    """The edit-list workflow (see the module docstring)."""
    spec = {}
    exec(open(a.spec).read(), spec)
    ctx = EditsCtx(spec)
    orig = open(os.path.join(ROOT, spec['src'])).read()
    s0, err = ctx.run(orig)
    print(f'current source: {s0} {err or ""}')
    base = edits_apply(orig, spec.get('base', []))
    if base is None:
        raise SystemExit('base edits do not apply')
    sb, err = ctx.run(base)
    print(f'plain base: {sb} {err or ""}')
    muts = [(f'alt{i}' + (f' {g[0]}' if isinstance(g[0], str) else ''), g[1] if isinstance(g[0], str) else g)
            for i, g in enumerate(spec.get('alts', []))]
    if not a.no_auto:
        muts += edits_auto_mutations(base, spec.get('region'), spec.get('auto', ('type', 'compound', 'swap')))
    print(f'{len(muts)} mutations')
    combos = []
    for k in range(1, (a.k or 2) + 1):
        for c in itertools.combinations(range(len(muts)), k):
            combos.append(c)
            if len(combos) >= a.max:
                break
    jobs = []
    for c in combos:
        t = base
        for i in c:
            t = edits_apply(t, muts[i][1])
            if t is None:
                break
        if t is not None:
            jobs.append((c, t))
    print(f'{len(jobs)} variants')
    results = []
    with concurrent.futures.ThreadPoolExecutor(a.j) as ex:
        futs = {ex.submit(ctx.run, t): (c, t) for c, t in jobs}
        for f in concurrent.futures.as_completed(futs):
            c, t = futs[f]
            s, err = f.result()
            if s is not None:
                results.append((s, c, t))
    results.sort(key=lambda r: (r[0], len(r[1])))
    for s, c, t in results[:a.top]:
        print(f'{s:5d}  ' + ' + '.join(muts[i][0] for i in c))
    if a.keep:
        os.makedirs(a.keep, exist_ok=True)
        for n, (s, c, t) in enumerate(results[:a.top]):
            open(os.path.join(a.keep, f'{n:02d}_{s}.txt'), 'w').write(
                '/* ' + ' + '.join(muts[i][0] for i in c) + ' */\n' + t)


def distinct(results):
    """(score, collateral, choice) per distinct output, each with its
    fewest changes."""
    best = {}
    for k, s in results.items():
        if s is None:
            continue
        n = sum(1 for _, i in k if i)
        if s[1] not in best or n < best[s[1]][0]:
            best[s[1]] = (n, s[0], s[2] if len(s) > 2 else 0, k)
    return sorted((sc, o, k) for n, sc, o, k in best.values())


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("spec", help="a SPEC.py or an inline-slot template")
    ap.add_argument("--src", help="template: the source file")
    ap.add_argument("--sig", help="template: the function's definition line")
    ap.add_argument("--sym", help="template: the symbol (the C name)")
    ap.add_argument("--ref", help="template: object with the ROM's code (default: build/'s)")
    ap.add_argument("-j", type=int, default=os.cpu_count())
    ap.add_argument("-k", type=int, default=None, help="max axes changed at once, then a beam search")
    ap.add_argument("--beam", type=int, default=8, help="beam width after the -k pass")
    ap.add_argument("--rounds", type=int, default=3, help="beam rounds")
    ap.add_argument("--limit", type=int, default=4000, help="full product up to this many, else a sample")
    ap.add_argument("--sample", action="store_true", help="sample --limit combinations even if fewer")
    ap.add_argument("--metric", choices=["insns", "lines"], default="insns")
    ap.add_argument("--all", action="store_true", help="also score the object's other functions")
    ap.add_argument("--top", type=int, default=15)
    ap.add_argument("-v", "--verbose", type=int, default=0, help="spell out the N best axis by axis")
    ap.add_argument("--dump", help="write every result as TSV")
    ap.add_argument("--best", help="write the best variant's text here")
    ap.add_argument("--work", default=None)
    ap.add_argument("--flags", default="", help="extra compiler flags")
    ap.add_argument("--show", action="store_true", help="print the best variant's text and diff")
    ap.add_argument("--eval", metavar="AXIS=I,...", help="only score and diff this one variant")
    ap.add_argument("--keep", help="edit-list specs: write the best variants here")
    ap.add_argument("--max", type=int, default=200000, help="edit-list specs: cap on combinations")
    ap.add_argument("--no-auto", action="store_true", help="edit-list specs: only the spec's alts")
    args = ap.parse_args()

    if args.spec.endswith(".py"):
        with open(args.spec) as f:
            if re.search(r"^(alts|base)\s*=", f.read(), re.M):
                return edits_main(args)
        src = SpecSource(args.spec)
    else:
        src = TemplateSource(args.spec, args.src, args.sig, args.sym, args.ref)
    src.extra_flags += shlex.split(args.flags)
    work = args.work or os.path.join(ROOT, "build", "natural_enum", os.path.basename(args.spec))
    r = Runner(src, work, args.metric, args.all)
    axes = src.axes
    base = {a: 0 for a in axes}

    if args.eval:
        c = dict(base)
        for kv in args.eval.split(","):
            k, v = kv.split("=")
            c[k] = int(v)
        body = src.render(c)
        print(body)
        r.keep_diff = True
        r.cache.pop(r.key(body), None)
        print("score:", r.compile(body))
        return

    results = {}
    seen_body = {}

    def run_all(choices):
        todo = []
        for c in choices:
            key = tuple(sorted(c.items()))
            if key in results or not src.constraint(c):
                continue
            results[key] = None
            todo.append((key, src.render(c)))
        uniq = {}
        for key, body in todo:
            uniq.setdefault(body, []).append(key)
        bodies = [b for b in uniq if b not in seen_body]
        with ThreadPoolExecutor(args.j) as ex:
            for b, s in zip(bodies, ex.map(r.compile, bodies)):
                seen_body[b] = s
        for body, keys in uniq.items():
            for key in keys:
                results[key] = seen_body[body]
        r.save()
        return len(bodies)

    def neighbours(c):
        for a in axes:
            for i in range(src.sizes[a]):
                if i != c[a]:
                    n = dict(c)
                    n[a] = i
                    yield n

    total = 1
    for a in axes:
        total *= src.sizes[a]
    if args.k is None:
        if total <= args.limit and not args.sample:
            choices = [dict(zip(axes, p)) for p in itertools.product(*[range(src.sizes[a]) for a in axes])]
        else:
            rnd = random.Random(662)
            picked = {tuple(0 for _ in axes)}
            while len(picked) < min(args.limit, total):
                picked.add(tuple(rnd.randrange(src.sizes[a]) for a in axes))
            choices = [dict(zip(axes, p)) for p in sorted(picked)]
        n = run_all(choices)
        print("%d axes, %d combinations, %d tried, %d compiled" % (len(axes), total, len(choices), n), file=sys.stderr)
    else:
        choices = []
        for k in range(0, args.k + 1):
            for combo in itertools.combinations(axes, k):
                for idx in itertools.product(*[range(1, src.sizes[a]) for a in combo]):
                    c = dict(base)
                    c.update(zip(combo, idx))
                    choices.append(c)
        n = run_all(choices)
        print("exhaustive (k<=%d): %d variants" % (args.k, n), file=sys.stderr)
        for rnd in range(args.rounds):
            best = distinct(results)[: args.beam]
            n = run_all([x for _, _, k in best for x in neighbours(dict(k))])
            print("beam round %d: %d new" % (rnd + 1, n), file=sys.stderr)
            if not n:
                break

    ranked = distinct(results)
    b = results.get(tuple(sorted(base.items())))
    print("base (all defaults): %s" % (b and b[0]))
    print(
        "compiled: %d (%d distinct outputs), failed: %d"
        % (sum(1 for s in results.values() if s), len(ranked), sum(1 for s in results.values() if s is None))
    )
    for s, o, k in ranked[: args.top]:
        diff = {a: i for a, i in k if i}
        print("%4d%s  %s" % (s, (" +%d other" % o) if args.all else "", diff))
    for s, o, k in ranked[: args.verbose]:
        c = dict(k)
        print("%4d:" % s)
        for a in axes:
            if src.sizes[a] > 1:
                print("    %-8s %s" % (a, " ".join(src.axis_text(c, a).split())[:90]))
    if args.dump:
        with open(args.dump, "w") as f:
            f.write("score\tother\t" + "\t".join(axes) + "\n")
            for k, s in sorted(results.items(), key=lambda kv: (kv[1] is None, kv[1] and kv[1][0])):
                if s is None:
                    continue
                c = dict(k)
                f.write("%d\t%d\t%s\n" % (s[0], s[2] if len(s) > 2 else 0, "\t".join(" ".join(src.axis_text(c, a).split()) for a in axes)))
    if ranked and args.best:
        with open(args.best, "w") as f:
            f.write(src.render(dict(ranked[0][2])))
    if args.show and ranked:
        body = src.render(dict(ranked[0][2]))
        print(body)
        r.keep_diff = True
        r.cache.pop(r.key(body), None)
        r.compile(body)


if __name__ == "__main__":
    main()
