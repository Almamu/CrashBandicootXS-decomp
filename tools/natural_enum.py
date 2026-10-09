#!/usr/bin/env python3
"""Enumerate natural rewrites of one function and score each against the ROM.

    tools/natural_enum.py SPEC.py [-j N] [-k K] [--beam B] [--top T]

The decomp-permuter finds matches, but mostly with junk nobody would write
(#662). This is its opposite: it only tries the rewrites a spec lists, and
a spec only lists things a programmer plausibly writes (a local's type or
signedness, statement order, where a local is declared, splitting or
merging locals, a sub-expression as an inline helper, early return vs
if/else, loop forms, compound assignment, `?:` vs if, a class accessor
instead of the field). It tries every combination of up to K of them
(pairs and triples by default), then a beam search from the best, in
parallel, and prints the best variants by how far each is from the ROM.
The winner still has to be judged by hand: it must read like the
original author's code.

A spec is a Python file defining:

    SOURCE   = "src/bosses/hovercraft.cpp"   # the file the function is in
    SYMBOL   = "ConvertHovercraftTiles"      # its symbol in the object
    TARGET   = "p8r/target/hovercraft.o"     # an object with the ROM's code
    REGION   = '''...'''                     # exact text of SOURCE to replace
    TEMPLATE = '''...{axis}...'''            # REGION with {axis} slots
    AXES     = {"axis": ["default", "alternative", ...], ...}
    # optional: CONSTRAINT(choice) -> bool, choice = {axis: index}
    # optional: EXTRA_FLAGS = ["-f..."], PRELUDE = "text put before REGION"

TARGET is any object with the function as the ROM has it (a copy of the
matched build's object, taken before editing). Literal `{`/`}` in
TEMPLATE are written `{{`/`}}`. An axis value can itself contain
`{other}` slots (and no other braces). The compile command is the
Makefile's (`make -n`), so the object's compiler and flags are used.

The score is the number of differing instructions (a line diff of the
normalised disassembly: branch targets relative to the function, pool
words by relocation); 0 is a match. Variants that don't compile are
dropped. Results are cached by the rendered text in the work directory.
"""

import argparse
import difflib
import hashlib
import importlib.util
import itertools
import json
import os
import re
import shlex
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def load_spec(path):
    spec = importlib.util.spec_from_file_location("spec", path)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


def make_commands(source):
    """The Makefile's cpp and cc1 command lines for SOURCE's object."""
    obj = "build/crashbandicootxs/" + os.path.splitext(source)[0] + ".o"
    out = subprocess.run(
        ["make", "-n", "-W", source, obj],
        cwd=ROOT,
        capture_output=True,
        text=True,
    ).stdout
    for line in out.splitlines():
        if "|" in line and source in line and "agbc" in line:
            cpp, cc1 = line.split("|", 1)
            cpp = shlex.split(cpp)
            cc1 = shlex.split(cc1)
            drop = {"-MF", "-MT"}
            clean = []
            skip = False
            for a in cpp:
                if skip:
                    skip = False
                    continue
                if a in drop:
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


def symbol_names(symbol):
    """SYMBOL and its mangled name."""
    return {symbol} | {k for k, v in cxx_symbols().items() if v == symbol}


def objdump_func(obj, symbol):
    names = symbol_names(symbol)
    out = subprocess.run(
        ["arm-none-eabi-objdump", "-dr", "--no-show-raw-insn", obj],
        capture_output=True,
        text=True,
    ).stdout
    lines = []
    start = None
    inside = False
    for line in out.splitlines():
        m = re.match(r"^([0-9a-f]+) <(.+)>:$", line)
        if m:
            if inside:
                break
            if m.group(2) in names:
                inside = True
                start = int(m.group(1), 16)
            continue
        if not inside or not line.strip():
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
            text = re.sub(r" <[^>]*>", "", text)
            lines.append(text)
    return lines


def score(a, b):
    sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
    n = 0
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag != "equal":
            n += max(i2 - i1, j2 - j1)
    return n


class Runner:
    def __init__(self, spec, work):
        self.spec = spec
        self.work = work
        os.makedirs(work, exist_ok=True)
        self.cpp, self.cc1 = make_commands(spec.SOURCE)
        self.cc1 += list(getattr(spec, "EXTRA_FLAGS", []))
        with open(os.path.join(ROOT, spec.SOURCE)) as f:
            self.text = f.read()
        if spec.REGION not in self.text:
            sys.exit("REGION not found in " + spec.SOURCE)
        self.target = objdump_func(os.path.join(ROOT, spec.TARGET), spec.SYMBOL)
        if not self.target:
            sys.exit("symbol %s not in target" % spec.SYMBOL)
        self.keep_diff = False
        self.cache_path = os.path.join(work, "cache.json")
        try:
            with open(self.cache_path) as f:
                self.cache = json.load(f)
        except (OSError, ValueError):
            self.cache = {}

    def render(self, choice):
        vals = {k: self.spec.AXES[k][i] for k, i in choice.items()}

        def fill(text):
            # axis values may name other axes; they have no literal braces
            return re.sub(r"\{([a-z_0-9]+)\}", lambda m: fill(vals[m.group(1)]), text)

        return self.spec.TEMPLATE.format(**{k: fill(v) for k, v in vals.items()})

    def compile(self, body):
        key = hashlib.sha1(body.encode()).hexdigest()
        if key in self.cache:
            return self.cache[key]
        src = self.text.replace(self.spec.REGION, body, 1)
        d = tempfile.mkdtemp(dir=self.work)
        try:
            cppf = os.path.join(d, "v.cpp")
            with open(cppf, "w") as f:
                f.write(src)
            srcdir = os.path.dirname(os.path.join(ROOT, self.spec.SOURCE))
            ii = subprocess.run(
                self.cpp[:1] + ["-iquote", srcdir] + self.cpp[1:] + [cppf],
                cwd=ROOT,
                capture_output=True,
            )
            if ii.returncode:
                res = None
            else:
                s = os.path.join(d, "v.s")
                cc = subprocess.run(
                    self.cc1 + ["-o", s], input=ii.stdout, cwd=ROOT, capture_output=True
                )
                o = os.path.join(d, "v.o")
                if cc.returncode or subprocess.run(
                    ["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", o, s],
                    capture_output=True,
                ).returncode:
                    res = None
                else:
                    got = objdump_func(o, self.spec.SYMBOL)
                    res = [score(self.target, got), hashlib.sha1("\n".join(got).encode()).hexdigest()[:12]]
                    if self.keep_diff:
                        sys.stdout.writelines(
                            difflib.unified_diff(
                                [x + "\n" for x in self.target], [x + "\n" for x in got], "rom", "variant", n=2
                            )
                        )
        finally:
            subprocess.run(["rm", "-rf", d])
        self.cache[key] = res
        return res

    def save(self):
        with open(self.cache_path, "w") as f:
            json.dump(self.cache, f)


def distinct(results):
    """(score, choice) per distinct output, each with its fewest changes."""
    best = {}
    for k, s in results.items():
        if s is None:
            continue
        n = sum(1 for _, i in k if i)
        if s[1] not in best or n < best[s[1]][0]:
            best[s[1]] = (n, s[0], k)
    return sorted((sc, k) for n, sc, k in best.values())


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("spec")
    ap.add_argument("-j", type=int, default=os.cpu_count())
    ap.add_argument("-k", type=int, default=3, help="max axes changed at once (exhaustive)")
    ap.add_argument("--beam", type=int, default=8, help="beam width after the exhaustive pass")
    ap.add_argument("--rounds", type=int, default=3, help="beam rounds")
    ap.add_argument("--top", type=int, default=15)
    ap.add_argument("--work", default=None)
    ap.add_argument("--show", action="store_true", help="print the best variant's text and diff")
    ap.add_argument("--eval", metavar="AXIS=I,...", help="only score and diff this one variant")
    args = ap.parse_args()

    spec = load_spec(args.spec)
    work = args.work or os.path.join(ROOT, "build", "natural_enum", os.path.basename(args.spec))
    r = Runner(spec, work)
    axes = list(spec.AXES)
    ok = getattr(spec, "CONSTRAINT", lambda c: True)
    base = {a: 0 for a in axes}
    if args.eval:
        c = dict(base)
        for kv in args.eval.split(","):
            k, v = kv.split("=")
            c[k] = int(v)
        body = r.render(c)
        print(body)
        r.keep_diff = True
        r.cache.pop(hashlib.sha1(body.encode()).hexdigest(), None)
        print("score:", r.compile(body))
        return
    results = {}

    def run_all(choices):
        todo = []
        for c in choices:
            key = tuple(sorted(c.items()))
            if key in results or not ok(c):
                continue
            results[key] = None
            todo.append(c)
        with ThreadPoolExecutor(args.j) as ex:
            for c, s in zip(todo, ex.map(lambda c: r.compile(r.render(c)), todo)):
                results[tuple(sorted(c.items()))] = s
        r.save()
        return len(todo)

    def variants(c, axs):
        for a in axs:
            for i in range(len(spec.AXES[a])):
                if i != c[a]:
                    n = dict(c)
                    n[a] = i
                    yield n

    choices = []
    for k in range(0, args.k + 1):
        for combo in itertools.combinations(axes, k):
            for idx in itertools.product(*[range(1, len(spec.AXES[a])) for a in combo]):
                c = dict(base)
                c.update(zip(combo, idx))
                choices.append(c)
    n = run_all(choices)
    print("exhaustive (k<=%d): %d variants" % (args.k, n), file=sys.stderr)
    for rnd in range(args.rounds):
        best = distinct(results)[: args.beam]
        nxt = [n for _, k in best for n in variants(dict(k), axes)]
        n = run_all(nxt)
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
    for s, k in ranked[: args.top]:
        diff = {a: i for a, i in k if i}
        print("%4d  %s" % (s, diff))
    if args.show and ranked:
        body = r.render(dict(ranked[0][1]))
        print(body)
        r.keep_diff = True
        r.cache.pop(hashlib.sha1(body.encode()).hexdigest(), None)
        r.compile(body)


if __name__ == "__main__":
    main()
