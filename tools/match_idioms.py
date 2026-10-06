#!/usr/bin/env python3
"""Count the matching workarounds left in the C sources (#576).

Scans every .c and .h file under src/, lib/ and include/ and counts each
workaround idiom by kind and by file, plus the per-object compiler flag
lists in the Makefile. The idioms and the macros that replace them are
described in docs/matching_techniques.md and include/match.h.

It does not run the preprocessor: both arms of an `#if NON_MATCHING` are
read. Comments are stripped before matching, so an idiom quoted in a
comment isn't counted.

Usage:
  tools/match_idioms.py              summary table, one row per kind
  tools/match_idioms.py --files      also list the files for each kind
  tools/match_idioms.py --kind K     list every site of kind K (file:line)
  tools/match_idioms.py --kind K --show   ... with the source line
  tools/match_idioms.py --list-kinds list the kind names
  tools/match_idioms.py --check-macros   check that each match.h macro
                                         expands to the spelling it replaces
  tools/match_idioms.py --convert K  rewrite kind K's sites to its match.h
                                     macro (see CONVERTERS)
"""

import argparse
import collections
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SCAN_DIRS = ("src", "lib", "include")
# Where the macros are defined: not counted as sites.
SKIP_FILES = ("include/match.h",)

# Kind name -> one-line description, in report order. A `*_macro` kind
# counts the sites already converted to the include/match.h macro.
KINDS = collections.OrderedDict([
    ("pin", 'register-variable pin: `register T x asm("rN")`'),
    ("pin_macro", "MATCH_HOLD_REG(T, x, rN)"),
    ("asm_label", 'asm-label alias: `T f(...) asm("Name");` / `extern T x asm("Name");`'),
    ("empty", 'empty statement: `asm("")` (barrier / padding)'),
    ("empty_volatile", 'empty statement: `asm volatile("")`'),
    ("barrier_macro", "MATCH_BARRIER()"),
    ("use", 'extra-reference nudge / hold end: `asm("" : : "r"(x))`'),
    ("use_volatile", 'extra-reference nudge: `asm volatile("" : : "r"(x))`'),
    ("use_macro", "MATCH_USE(x) / MATCH_USE_VOLATILE(x)"),
    ("keep", 'value keeper: `asm("" : "+r"(x))`'),
    ("keep_volatile", 'value keeper: `asm volatile("" : "+r"(x))`'),
    ("keep_macro", "MATCH_KEEP(x) / MATCH_KEEP_VOLATILE(x)"),
    ("hold", 'hold start / undefined value: `asm("" : "=r"(x))`'),
    ("hold_volatile", 'hold start: `asm volatile("" : "=r"(x))`'),
    ("hold_macro", "MATCH_HOLD(x) / MATCH_HOLD_VOLATILE(x)"),
    ("const", 'constant-init: `asm("" : "=r"(v) : "0"(K))`'),
    ("const_volatile", 'constant-init: `asm volatile("" : "=r"(v) : "0"(K))`'),
    ("const_macro", "MATCH_CONST(v, K) / MATCH_CONST_VOLATILE(v, K)"),
    ("mem_barrier", 'memory barrier: `asm volatile("" ::: "memory")`'),
    ("reg_clobber", 'register clobber: `asm("" : : : "rN")`'),
    ("mem_ref", 'memory-operand nudge: `asm("" : "+m"(x))` / `asm("" : : "m"(x))`'),
    ("empty_other", "other empty-template asm with operands"),
    ("insn", "inline asm that emits instructions (`add`, `lsl`, `mov`, ...)"),
    ("file_align", 'file-scope `asm(".align 2, 0")` padding'),
    ("pool", '`.pool` (literal-pool placement) in an asm statement'),
    ("set_alias", '`.set` symbol alias in an asm statement'),
    ("file_asm_other", "other file-scope asm block (hand-written routine, data)"),
    ("box_addr", "BOX_ADDR(a) uses"),
    ("box_addr_def", "BOX_ADDR definitions"),
    ("field_cast_store", "`*(T *)&s->field = ...` retyped field store"),
    ("field_cast_load", "`*(T *)&s->field` retyped field read"),
    ("self_init", "`T x = x;` self-initialisation (-Wuninitialized)"),
    ("non_matching", "`#if NON_MATCHING` / `#ifdef NON_MATCHING` block"),
    ("naked", "NAKED function"),
    ("volatile_local", "`volatile` inside a function body (scoped volatile casts and locals)"),
])

ASM_START = re.compile(r"\b(?:__asm__|asm)\b(\s*(?:volatile|__volatile__)\b)?\s*\(")
PIN = re.compile(r"\bregister\b[^;{}()]*?\b\w+\s*$")
HOLD_REG = re.compile(r"\bMATCH_HOLD_REG\s*\(")
BOX_ADDR_USE = re.compile(r"\bBOX_ADDR\s*\(")
BOX_ADDR_DEF = re.compile(r"#\s*define\s+BOX_ADDR\b")
FIELD_CAST = re.compile(
    r"\*\s*\(\s*(?:const\s+|volatile\s+)?\w+(?:\s+volatile)?\s*\*\s*\)\s*&\s*"
    r"[A-Za-z_]\w*(?:\[[^\]]*\])?(?:\s*(?:->|\.)\s*\w+(?:\[[^\]]*\])?)+")
SELF_INIT = re.compile(
    r"(?:^|[;{}(,])\s*(?:const\s+|volatile\s+|register\s+|unsigned\s+|signed\s+)*"
    r"(?:struct\s+)?[A-Za-z_]\w*[\s*]+(?:const\s+)?([A-Za-z_]\w*)\s*=\s*\1\s*[;,]")
# (`#ifndef NON_MATCHING` is core.h giving the toggle its default.)
NON_MATCHING = re.compile(r"^\s*#\s*(?:if|ifdef|elif)\b[^\n]*\bNON_MATCHING\b", re.M)
NAKED = re.compile(r"\bNAKED\b[^;{]*\)\s*\{")
VOLATILE_KW = re.compile(r"\b(?:volatile|__volatile__)\b")


def strip_comments(text):
    """Blank out comments (keeping newlines and string/char literals)."""
    out = []
    i, n = 0, len(text)
    while i < n:
        c = text[i]
        if c == "/" and i + 1 < n and text[i + 1] == "*":
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
            out.append(re.sub(r"[^\n]", " ", text[i:j]))
            i = j
        elif c == "/" and i + 1 < n and text[i + 1] == "/":
            j = text.find("\n", i)
            j = n if j < 0 else j
            out.append(" " * (j - i))
            i = j
        elif c in "\"'":
            j = i + 1
            while j < n and text[j] != c:
                j += 2 if text[j] == "\\" else 1
            out.append(text[i:j + 1])
            i = j + 1
        else:
            out.append(c)
            i += 1
    return "".join(out)


def balanced(text, start):
    """Index just past the ')' matching the '(' at text[start]."""
    depth = 0
    i, n = start, len(text)
    while i < n:
        c = text[i]
        if c in "\"'":
            j = i + 1
            while j < n and text[j] != c:
                j += 2 if text[j] == "\\" else 1
            i = j + 1
            continue
        if c == "(":
            depth += 1
        elif c == ")":
            depth -= 1
            if depth == 0:
                return i + 1
        i += 1
    return n


def brace_depths(text):
    """Return a list giving the {}-nesting depth at each offset."""
    depths = [0] * (len(text) + 1)
    d = 0
    in_str = None
    i = 0
    while i < len(text):
        c = text[i]
        if in_str:
            if c == "\\":
                depths[i] = d
                i += 1
            elif c == in_str:
                in_str = None
        elif c in "\"'":
            in_str = c
        elif c == "{":
            d += 1
        elif c == "}":
            d -= 1
        depths[i] = d
        i += 1
    depths[len(text)] = d
    return depths


def in_define(text, pos):
    """True if pos is inside a (possibly backslash-continued) #define."""
    start = pos
    while True:
        nl = text.rfind("\n", 0, start)
        line = text[nl + 1:start] if start == pos else text[nl + 1:start]
        if re.match(r"\s*#\s*define\b", line):
            return True
        if nl <= 0 or not text[:nl].rstrip(" \t").endswith("\\"):
            return False
        start = nl


def classify_asm(text, m, end, depth):
    """Classify one asm(...) occurrence. Returns a list of kinds."""
    volatile = bool(m.group(1))
    body = text[m.end():end - 1]
    before = text[max(0, m.start() - 400):m.start()]
    after = text[end:end + 40].lstrip()
    s = body.strip()
    tmpl = re.match(r'^((?:"(?:[^"\\]|\\.)*"\s*)+)', s)
    template = ""
    rest = s
    if tmpl:
        template = "".join(re.findall(r'"((?:[^"\\]|\\.)*)"', tmpl.group(1)))
        rest = s[tmpl.end():].strip()
    # Declarations: `register T x asm("rN")` or an asm label on a
    # prototype or extern (`T f(...) asm("Name");`).
    line_before = re.split(r"[;{}]", before)[-1]
    if PIN.search(line_before) and (re.fullmatch(r"r\d+|ip|sb|sl|fp|lr|sp", template)
                                    or (not tmpl and re.fullmatch(r"\w+", s))):
        return ["pin"]
    if not volatile and rest == "" and re.fullmatch(r"[A-Za-z_$][\w.$]*", template) \
            and re.match(r"[;,=)]", after):
        return ["asm_label"]
    kinds = []
    if depth == 0 and not in_define(text, m.start()):
        if ".pool" in template:
            kinds.append("pool")
        if ".set " in template:
            kinds.append("set_alias")
        if re.fullmatch(r"\s*\.align\s+2\s*,\s*0\s*", template):
            kinds.append("file_align")
        elif not kinds:
            kinds.append("file_asm_other")
        return kinds
    if ".pool" in template:
        kinds.append("pool")
    if template.strip() == "":
        v = "_volatile" if volatile else ""
        r = re.sub(r"\s+", "", rest)
        if r == "":
            return ["empty" + v]
        if re.fullmatch(r'::"r"\(.*\)', r):
            return ["use" + v]
        if re.fullmatch(r':"\+r"\(.*\)', r):
            return ["keep" + v]
        if re.fullmatch(r':"=r"\([^:]*\)', r):
            return ["hold" + v]
        if re.fullmatch(r':"=r"\([^:]*\):"0"\(.*\)', r):
            return ["const" + v]
        if re.fullmatch(r':::"memory"', r):
            return ["mem_barrier"]
        if re.fullmatch(r':::"(?:r\d+|ip|sb|sl|fp|lr)"(?:,"(?:r\d+|ip|sb|sl|fp|lr)")*', r):
            return ["reg_clobber"]
        if re.fullmatch(r':(?:"\+m"\(.*\))?(?::"m"\(.*\))?', r):
            return ["mem_ref"]
        return ["empty_other"]
    if not kinds:
        kinds.append("insn")
    return kinds


def line_of(text, pos):
    return text.count("\n", 0, pos) + 1


def scan_file(path, rel, hits):
    with open(path, encoding="utf-8", errors="replace") as f:
        raw = f.read()
    text = strip_comments(raw)
    depths = brace_depths(text)

    def add(kind, pos):
        hits[kind].append((rel, line_of(text, pos)))

    for m in ASM_START.finditer(text):
        end = balanced(text, m.end() - 1)
        for k in classify_asm(text, m, end, depths[m.start()]):
            add(k, m.start())
    macro_re = {
        "pin_macro": HOLD_REG,
        "barrier_macro": re.compile(r"\bMATCH_BARRIER\s*\("),
        "use_macro": re.compile(r"\bMATCH_USE(?:_VOLATILE)?\s*\("),
        "keep_macro": re.compile(r"\bMATCH_KEEP(?:_VOLATILE)?\s*\("),
        "hold_macro": re.compile(r"\bMATCH_HOLD(?:_VOLATILE)?\s*\("),
        "const_macro": re.compile(r"\bMATCH_CONST(?:_VOLATILE)?\s*\("),
    }
    for kind, rx in macro_re.items():
        for m in rx.finditer(text):
            add(kind, m.start())
    for m in BOX_ADDR_DEF.finditer(text):
        add("box_addr_def", m.start())
    for m in BOX_ADDR_USE.finditer(text):
        if not re.search(r"#\s*define\s*$", text[max(0, m.start() - 20):m.start()]):
            add("box_addr", m.start())
    for m in FIELD_CAST.finditer(text):
        after = text[m.end():m.end() + 3]
        if re.match(r"\s*=(?!=)", after):
            add("field_cast_store", m.start())
        else:
            add("field_cast_load", m.start())
    for m in SELF_INIT.finditer(text):
        if depths[m.start(1)] > 0:
            add("self_init", m.start(1))
    for m in NON_MATCHING.finditer(text):
        add("non_matching", text.index("#", m.start()))
    for m in NAKED.finditer(text):
        if not re.search(r"#\s*define\s*$", text[max(0, m.start() - 20):m.start()]):
            add("naked", m.start())
    for m in VOLATILE_KW.finditer(text):
        if depths[m.start()] > 0 and \
                not re.search(r"\b(?:asm|__asm__)\s*$", text[max(0, m.start() - 12):m.start()]):
            add("volatile_local", m.start())


def makefile_flags():
    """Per-object flag groups from the Makefile: name -> (flags, [objects])."""
    path = os.path.join(ROOT, "Makefile")
    with open(path) as f:
        text = f.read().replace("\\\n", " ")
    lists = {}
    for m in re.finditer(r"^([A-Z0-9_]+_OBJS)\s*:=\s*(.*)$", text, re.M):
        objs = re.findall(r"\$\((?:C|LIB)_BUILDDIR\)/(\S+)\.o", m.group(2))
        if objs:
            lists[m.group(1)] = objs
        elif m.group(2).strip().startswith("$("):
            lists[m.group(1)] = [m.group(2).strip()]
    flags = collections.OrderedDict()
    for name, objs in lists.items():
        rules = re.findall(r"^\$\(" + name + r"\):\s*(CC1\w*\s*[+:]?=.*)$", text, re.M)
        if rules:
            flags[name] = ("; ".join(r.strip() for r in rules), objs)
    for m in re.finditer(r"^\$\((?:C|LIB)_BUILDDIR\)/(\S+)\.o:\s*(CC1\w*\s*[+:]?=.*)$",
                         text, re.M):
        flags["(single) " + m.group(1)] = (m.group(2).strip(), [m.group(1)])
    return flags


# --convert: kinds that map one-to-one onto a match.h macro, as
# (macro, the asm's operand constraints in order). The rewrite keeps each
# operand expression's text as written.
CONVERTERS = {
    "empty": ("MATCH_BARRIER", []),
    "use": ("MATCH_USE", ["r"]),
    "use_volatile": ("MATCH_USE_VOLATILE", ["r"]),
    "keep": ("MATCH_KEEP", ["+r"]),
    "keep_volatile": ("MATCH_KEEP_VOLATILE", ["+r"]),
    "hold": ("MATCH_HOLD", ["=r"]),
    "hold_volatile": ("MATCH_HOLD_VOLATILE", ["=r"]),
    "const": ("MATCH_CONST", ["=r", "0"]),
    "const_volatile": ("MATCH_CONST_VOLATILE", ["=r", "0"]),
}


def split_top(text, sep):
    """Split text at top-level occurrences of sep (outside brackets/strings)."""
    parts, cur, depth, i = [], [], 0, 0
    while i < len(text):
        c = text[i]
        if c == '"':
            j = i + 1
            while j < len(text) and text[j] != '"':
                j += 2 if text[j] == "\\" else 1
            cur.append(text[i:j + 1])
            i = j + 1
            continue
        if c in "([{":
            depth += 1
        elif c in ")]}":
            depth -= 1
        if c == sep and depth == 0:
            parts.append("".join(cur))
            cur = []
        else:
            cur.append(c)
        i += 1
    parts.append("".join(cur))
    return parts


def asm_operands(body):
    """[(constraint, expr)] of an empty-template asm body, outputs first,
    or None if it has a clobber list or an operand it can't parse."""
    sections = split_top(body, ":")[1:]
    if len(sections) > 2:
        return None
    ops = []
    for sec in sections:
        for op in split_top(sec, ","):
            op = op.strip()
            if not op:
                continue
            m = re.fullmatch(r'"([^"]*)"\s*\((.*)\)', op, re.S)
            if not m:
                return None
            ops.append((m.group(1), m.group(2).strip()))
    return ops


def add_include(raw):
    """Add `#include "match.h"` after core.h (or the last #include)."""
    if re.search(r'^#include "match\.h"', raw, re.M):
        return raw
    m = re.search(r'^#include "core\.h"[^\n]*\n', raw, re.M)
    if not m:
        for m in re.finditer(r'^#include [^\n]*\n', raw, re.M):
            pass
    if not m:
        raise SystemExit("no #include to put match.h after")
    return raw[:m.end()] + '#include "match.h"\n' + raw[m.end():]


def convert_file(path, kinds):
    """Rewrite the sites of the given kinds in one file; return the count."""
    with open(path, encoding="utf-8") as f:
        raw = f.read()
    # strip_comments keeps every offset, so positions carry over to raw.
    text = strip_comments(raw)
    depths = brace_depths(text)
    edits = []
    for m in ASM_START.finditer(text):
        end = balanced(text, m.end() - 1)
        k = classify_asm(text, m, end, depths[m.start()])
        if len(k) != 1 or k[0] not in kinds:
            continue
        macro, want = CONVERTERS[k[0]]
        ops = asm_operands(text[m.end():end - 1])
        if ops is None or [c for c, _ in ops] != want:
            raise SystemExit("%s:%d: unexpected operands for %s"
                             % (path, line_of(text, m.start()), k[0]))
        args = ", ".join(e for _, e in ops)
        edits.append((m.start(), end, "%s(%s)" % (macro, args)))
    if not edits:
        return 0
    for start, end, new in reversed(edits):
        raw = raw[:start] + new + raw[end:]
    raw = add_include(raw)
    with open(path, "w", encoding="utf-8") as f:
        f.write(raw)
    return len(edits)


# --check-macros: each match.h macro must expand to the same tokens as the
# spelled-out form it replaces, so a conversion is byte-neutral.
MACRO_CHECKS = [
    ('MATCH_HOLD_REG(u8 *, p, r1) = q;', 'register u8 *p asm("r1") = q;'),
    ('MATCH_HOLD_REG(s32, h, ip);', 'register s32 h asm("ip");'),
    ('MATCH_BARRIER();', 'asm("");'),
    ('MATCH_USE(x);', 'asm("" : : "r"(x));'),
    ('MATCH_USE_VOLATILE(x);', 'asm volatile("" : : "r"(x));'),
    ('MATCH_KEEP(x);', 'asm("" : "+r"(x));'),
    ('MATCH_KEEP_VOLATILE(x);', 'asm volatile("" : "+r"(x));'),
    ('MATCH_HOLD(hold);', 'asm("" : "=r"(hold));'),
    ('MATCH_HOLD_VOLATILE(hold);', 'asm volatile("" : "=r"(hold));'),
    ('MATCH_CONST(zero, 0);', 'asm("" : "=r"(zero) : "0"(0));'),
    ('MATCH_CONST(t2, (void *)label2);', 'asm("" : "=r"(t2) : "0"((void *)label2));'),
    ('MATCH_CONST_VOLATILE(state, 2);', 'asm volatile("" : "=r"(state) : "0"(2));'),
    ('f(MATCH_KEEP_EXPR(u8 *, a + 1));',
     'f(({ u8 *_p = (a + 1); asm("" : "+r"(_p)); _p; }));'),
    ('f(BOX_ADDR(&f.a));',
     'f(({ struct aabb *_p = (&f.a); asm("" : "+r"(_p)); _p; }));'),
]
C_TOKEN = re.compile(r'"(?:[^"\\]|\\.)*"|[A-Za-z_]\w*|0[xX][0-9a-fA-F]+|\d+|->|\S')


def check_macros():
    import subprocess

    def tokens(src):
        out = subprocess.run(["arm-none-eabi-cpp", "-iquote", os.path.join(ROOT, "include"),
                              "-nostdinc", "-undef", "-P"], input=src,
                             capture_output=True, text=True, check=True).stdout
        return C_TOKEN.findall(out)

    bad = 0
    for macro, spelled in MACRO_CHECKS:
        ok = tokens('#include "match.h"\n' + macro + "\n") == tokens(spelled + "\n")
        bad += not ok
        print("%-4s %-34s %s" % ("ok" if ok else "DIFF", macro, spelled))
    return 1 if bad else 0


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--files", action="store_true", help="list files per kind")
    ap.add_argument("--kind", help="list every site of one kind")
    ap.add_argument("--show", action="store_true",
                    help="with --kind, print each site's source line")
    ap.add_argument("--list-kinds", action="store_true")
    ap.add_argument("--convert", metavar="KIND[,KIND]",
                    help="rewrite every site of these kinds in src/ and lib/ "
                         "to its match.h macro, adding the #include")
    ap.add_argument("--check-macros", action="store_true",
                    help="check that each match.h macro expands to the tokens "
                         "of the spelling it replaces")
    args = ap.parse_args()

    if args.check_macros:
        return check_macros()

    if args.convert:
        kinds = set(args.convert.split(","))
        if kinds - set(CONVERTERS):
            print("can't convert %s; convertible kinds: %s"
                  % (", ".join(sorted(kinds - set(CONVERTERS))), ", ".join(CONVERTERS)),
                  file=sys.stderr)
            return 1
        total = 0
        for d in ("src", "lib"):
            for dirpath, _, files in sorted(os.walk(os.path.join(ROOT, d))):
                for fn in sorted(files):
                    if fn.endswith((".c", ".h")):
                        p = os.path.join(dirpath, fn)
                        n = convert_file(p, kinds)
                        if n:
                            print("%3d  %s" % (n, os.path.relpath(p, ROOT)))
                        total += n
        print("%d site(s) converted" % total)
        return 0

    if args.list_kinds:
        for k, d in KINDS.items():
            print("%-18s %s" % (k, d))
        return 0

    hits = collections.defaultdict(list)
    for d in SCAN_DIRS:
        for dirpath, _, files in sorted(os.walk(os.path.join(ROOT, d))):
            for fn in sorted(files):
                p = os.path.join(dirpath, fn)
                rel = os.path.relpath(p, ROOT)
                if fn.endswith((".c", ".h")) and rel not in SKIP_FILES:
                    scan_file(p, rel, hits)

    if args.kind:
        if args.kind not in KINDS:
            print("unknown kind %r; see --list-kinds" % args.kind, file=sys.stderr)
            return 1
        for rel, ln in sorted(hits[args.kind]):
            if args.show:
                with open(os.path.join(ROOT, rel), errors="replace") as f:
                    src = f.read().split("\n")[ln - 1].strip()
                print("%s:%d: %s" % (rel, ln, src))
            else:
                print("%s:%d" % (rel, ln))
        return 0

    print("%-18s %6s %6s  %s" % ("kind", "sites", "files", "description"))
    for k, d in KINDS.items():
        sites = hits.get(k, [])
        files = collections.Counter(r for r, _ in sites)
        print("%-18s %6d %6d  %s" % (k, len(sites), len(files), d))
        if args.files and files:
            for f, c in sorted(files.items(), key=lambda x: (-x[1], x[0])):
                print("%28d  %s" % (c, f))

    print()
    print("Per-object compiler flags (Makefile):")
    for name, (rule, objs) in makefile_flags().items():
        print("  %-26s %3d object(s)  %s" % (name, len(objs), rule))
        if args.files:
            for o in objs:
                print("%34s%s" % ("", o))
    return 0


if __name__ == "__main__":
    sys.exit(main())
