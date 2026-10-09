#!/usr/bin/env python3
"""Count the matching workarounds left in the C sources (#576).

Scans every .c, .h, .cpp and .hpp file under src/, lib/ and include/ and counts each
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
  tools/match_idioms.py --check      fail if a spelled-out idiom that has a
                                     match.h macro appears outside
                                     ALLOWED_SPELLED, or a kind the build
                                     made redundant (REDUNDANT_KINDS)
                                     appears at all (run by CI)
  tools/match_idioms.py --functions  "N/2059 functions with no matching
                                     workarounds" and a per-directory
                                     table (README.md's numbers; needs a
                                     build); with --files, also list each
                                     function that has one, and its kinds
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
    ("use_macro", "MATCH_USE(x) / MATCH_USE2(a, b) / MATCH_USE2_VOLATILE(a, b)"),
    ("keep", 'value keeper: `asm("" : "+r"(x))`'),
    ("keep_volatile", 'value keeper: `asm volatile("" : "+r"(x))`'),
    ("keep_macro", "MATCH_KEEP(x) / MATCH_KEEP_VOLATILE(x)"),
    ("hold", 'hold start / undefined value: `asm("" : "=r"(x))`'),
    ("hold_volatile", 'hold start: `asm volatile("" : "=r"(x))`'),
    ("hold_macro", "MATCH_HOLD(x) / MATCH_HOLD_VOLATILE(x)"),
    ("const", 'constant-init: `asm("" : "=r"(v) : "0"(K))`'),
    ("const_volatile", 'constant-init: `asm volatile("" : "=r"(v) : "0"(K))`'),
    ("const_macro", "MATCH_CONST(v, K)"),
    ("mem_barrier", 'memory barrier: `asm volatile("" ::: "memory")`'),
    ("mem_barrier_macro", "MATCH_MEMORY_BARRIER()"),
    ("reg_clobber", 'register clobber: `asm("" : : : "rN")`'),
    ("clobber_macro", "MATCH_CLOBBER(rN) / MATCH_CLOBBER_VOLATILE(rN)"),
    ("mem_ref", 'memory-operand nudge: `asm("" : "+m"(x))` / `asm("" : : "m"(x))`'),
    ("mem_ref_macro", "MATCH_KEEP_MEM(x) / MATCH_USE_MEM(x)"),
    ("empty_other", "other empty-template asm with operands"),
    ("insn", "inline asm that emits instructions (`add`, `lsl`, `mov`, ...)"),
    ("file_align", 'file-scope `asm(".align 2, 0")` padding (redundant since #663; --check rejects it)'),
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
        if re.fullmatch(r"\s*\.(?:align\s+2|balign\s+4|p2align\s+2)\s*(?:,\s*0\s*)?", template):
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


def scan_file(path, rel, hits, offsets=None):
    """Add the file's sites to hits[kind] as (rel, line). With `offsets`
    (a list), also append (kind, offset) to it. Returns the
    comment-stripped text the offsets index."""
    with open(path, encoding="utf-8", errors="replace") as f:
        raw = f.read()
    text = strip_comments(raw)
    depths = brace_depths(text)

    def add(kind, pos):
        hits[kind].append((rel, line_of(text, pos)))
        if offsets is not None:
            offsets.append((kind, pos))

    for m in ASM_START.finditer(text):
        end = balanced(text, m.end() - 1)
        for k in classify_asm(text, m, end, depths[m.start()]):
            add(k, m.start())
    macro_re = {
        "pin_macro": HOLD_REG,
        "barrier_macro": re.compile(r"\bMATCH_BARRIER\s*\("),
        "use_macro": re.compile(r"\bMATCH_USE2?(?:_VOLATILE)?\s*\("),
        "keep_macro": re.compile(r"\bMATCH_KEEP(?:_VOLATILE)?\s*\("),
        "hold_macro": re.compile(r"\bMATCH_HOLD(?:_VOLATILE)?\s*\("),
        "const_macro": re.compile(r"\bMATCH_CONST(?:_VOLATILE)?\s*\("),
        "mem_barrier_macro": re.compile(r"\bMATCH_MEMORY_BARRIER\s*\("),
        "clobber_macro": re.compile(r"\bMATCH_CLOBBER(?:_VOLATILE)?\s*\("),
        "mem_ref_macro": re.compile(r"\bMATCH_(?:KEEP|USE)_MEM\s*\("),
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
    return text


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
    "pin": ("MATCH_HOLD_REG", None),  # declarator-aware: convert_pin
    "empty": ("MATCH_BARRIER", []),
    "use": ("MATCH_USE", ["r"]),
    "keep": ("MATCH_KEEP", ["+r"]),
    "keep_volatile": ("MATCH_KEEP_VOLATILE", ["+r"]),
    "hold": ("MATCH_HOLD", ["=r"]),
    "hold_volatile": ("MATCH_HOLD_VOLATILE", ["=r"]),
    "const": ("MATCH_CONST", ["=r", "0"]),
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


PIN_REG = re.compile(r"r\d+|ip|sb|sl|fp")
PIN_TYPE = re.compile(r"[A-Za-z_]\w*(?:\s+[A-Za-z_]\w*)*(?:\s*\*(?:\s*(?:const|volatile)\b)*)*")


def convert_pin(raw, text, m, end):
    """The (start, end, replacement) edit turning the register pin whose
    asm(...) is text[m.start():end] into MATCH_HOLD_REG, or a reason it
    can't be expressed. Only the `register T name asm("rN")` part is
    replaced; an initialiser after it stays where it is."""
    reg = re.fullmatch(r'\s*"([^"]*)"\s*', text[m.end():end - 1])
    if not reg or not PIN_REG.fullmatch(reg.group(1)):
        return "register name isn't a string literal r0-r12/ip/sb/sl/fp"
    # The declaration starts at the last `register` of the statement.
    stmt = max(text.rfind(c, 0, m.start()) for c in ";{}") + 1
    regs = list(re.finditer(r"\bregister\b", text[stmt:m.start()]))
    if len(regs) != 1:
        return "no single `register` keyword before the asm"
    start = stmt + regs[0].start()
    lead = text[stmt:start]
    # Only whitespace (or a #define's continuation backslash) may come
    # before `register`: `static register`, `const register`, a for-init
    # or a second declarator would change meaning.
    if not re.fullmatch(r"(?:\s|\\\n)*", lead):
        return "something other than whitespace before `register`"
    decl = text[stmt + regs[0].end():m.start()]
    if raw[start:m.start()] != text[start:m.start()]:
        return "comment inside the declaration"
    dm = re.fullmatch(r"\s+(.*?)\s*\b([A-Za-z_]\w*)\s*", decl, re.S)
    if not dm or "\n" in decl:
        return "declarator isn't `T name` on one line"
    typ = dm.group(1).strip()
    # Make `u8 *p` read `u8 *, p`; a bare `T*` or `T *` is kept as `T *`.
    typ = re.sub(r"\s*\*", " *", typ).replace("* *", "**").strip()
    while "* *" in typ:
        typ = typ.replace("* *", "**")
    if not PIN_TYPE.fullmatch(typ):
        return "type %r isn't a plain type (array or function pointer?)" % typ
    after = text[end:end + 40].lstrip()
    if not re.match(r"[;=]", after):
        return "the asm isn't followed by `;` or an initialiser"
    return (start, end, "MATCH_HOLD_REG(%s, %s, %s)" % (typ, dm.group(2), reg.group(1)))


def realign_continuations(raw, old_raw, edits):
    """Keep #define continuation backslashes in their original column on
    the lines an edit touched (one space before them if the line is now
    too long)."""
    old_lines = old_raw.split("\n")
    touched = {old_raw.count("\n", 0, s) for s, _, _ in edits}
    lines = raw.split("\n")
    for ln in touched:
        old, new = old_lines[ln], lines[ln]
        if not old.endswith("\\") or not new.endswith("\\"):
            continue
        col = len(old) - 1
        body = new[:-1].rstrip()
        lines[ln] = body + " " * max(1, col - len(body)) + "\\"
    return "\n".join(lines)


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
        if k[0] == "pin":
            e = convert_pin(raw, text, m, end)
            if isinstance(e, str):
                print("%s:%d: pin skipped: %s"
                      % (os.path.relpath(path, ROOT), line_of(text, m.start()), e),
                      file=sys.stderr)
            else:
                edits.append(e)
            continue
        macro, want = CONVERTERS[k[0]]
        ops = asm_operands(text[m.end():end - 1])
        if ops is None or [c for c, _ in ops] != want:
            # e.g. two "r" inputs in one asm: no one-operand macro says
            # that, and splitting it in two would change the insn stream.
            print("%s:%d: skipped, operands don't fit %s"
                  % (os.path.relpath(path, ROOT), line_of(text, m.start()), macro),
                  file=sys.stderr)
            continue
        args = ", ".join(e for _, e in ops)
        edits.append((m.start(), end, "%s(%s)" % (macro, args)))
    if not edits:
        return 0
    old_raw = raw
    for start, end, new in reversed(edits):
        raw = raw[:start] + new + raw[end:]
    raw = realign_continuations(raw, old_raw, edits)
    raw = add_include(raw)
    with open(path, "w", encoding="utf-8") as f:
        f.write(raw)
    return len(edits)


# --check-macros: each match.h macro must expand to the same tokens as the
# spelled-out form it replaces, so a conversion is byte-neutral.
MACRO_CHECKS = [
    ('MATCH_HOLD_REG(u8 *, p, r1) = q;', 'register u8 *p asm("r1") = q;'),
    ('MATCH_HOLD_REG(s32, h, ip);', 'register s32 h asm("ip");'),
    ('MATCH_HOLD_REG(struct settings_icon_actor **, q, sb) = (void *)0;',
     'register struct settings_icon_actor **q asm("sb") = (void *)0;'),
    ('MATCH_HOLD_REG(const u8 *, r, r2) = t + 1;', 'register const u8 *r asm("r2") = t + 1;'),
    ('MATCH_BARRIER();', 'asm("");'),
    ('MATCH_USE(x);', 'asm("" : : "r"(x));'),
    ('MATCH_USE2(ax, px);', 'asm("" : : "r"(ax), "r"(px));'),
    ('MATCH_USE2_VOLATILE(r0, r1);', 'asm volatile("" : : "r"(r0), "r"(r1));'),
    ('MATCH_KEEP(x);', 'asm("" : "+r"(x));'),
    ('MATCH_KEEP_VOLATILE(x);', 'asm volatile("" : "+r"(x));'),
    ('MATCH_HOLD(hold);', 'asm("" : "=r"(hold));'),
    ('MATCH_HOLD_VOLATILE(hold);', 'asm volatile("" : "=r"(hold));'),
    ('MATCH_CONST(zero, 0);', 'asm("" : "=r"(zero) : "0"(0));'),
    ('MATCH_CONST(t2, (void *)label2);', 'asm("" : "=r"(t2) : "0"((void *)label2));'),
    ('MATCH_CLOBBER(r5);', 'asm("" : : : "r5");'),
    ('MATCH_CLOBBER_VOLATILE(r4);', 'asm volatile("" ::: "r4");'),
    ('MATCH_MEMORY_BARRIER();', 'asm volatile("" ::: "memory");'),
    ('MATCH_KEEP_MEM(heights[k]);', 'asm("" : "+m"(heights[k]));'),
    ('MATCH_USE_MEM(q2);', 'asm("" : : "m"(q2));'),
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


# --check: the kinds that have a match.h macro and so must not be spelled
# out in new code. use_volatile and const_volatile have no macro since the
# C++ conversion's cleanup (#664: no site needed MATCH_USE_VOLATILE or
# MATCH_CONST_VOLATILE any more); a new site would add it back to match.h.
SPELLED_KINDS = ("pin", "empty", "empty_volatile", "use", "use_volatile", "keep",
                 "keep_volatile", "hold", "hold_volatile", "const", "const_volatile",
                 "mem_barrier", "reg_clobber", "mem_ref", "empty_other")
# The sites that stay spelled out, as (file, kind) -> (count, reason).
# docs/matching_techniques.md lists them too. Keep the counts exact: a
# new site fails the check, and so does a stale entry.
ALLOWED_SPELLED = {
    ("lib/gax/src/gax_swi.c", "mem_ref"):
        (1, 'two "m" inputs in one volatile asm; no macro has that shape'),
}


# Kinds the build makes unnecessary, so --check rejects every site:
# kind -> what to do instead.
REDUNDANT_KINDS = {
    "file_align": "drop it; the Makefile's ZERO_PAD_TEXT zero-fills the end of "
                  "every compiled object's .text (#663, docs/matching_techniques.md)",
}


def check_spelled(hits):
    bad = 0
    for kind, why in REDUNDANT_KINDS.items():
        for r, ln in sorted(hits.get(kind, [])):
            bad += 1
            print("%s:%d: redundant %s; %s" % (r, ln, kind, why))
    counts = collections.Counter()
    for kind in SPELLED_KINDS:
        for rel, _ in hits.get(kind, []):
            counts[(rel, kind)] += 1
    for (rel, kind), n in sorted(counts.items()):
        if n > ALLOWED_SPELLED.get((rel, kind), (0, None))[0]:
            bad += 1
            for r, ln in sorted(hits[kind]):
                if r == rel:
                    print("%s:%d: spelled-out %s; use its include/match.h macro (%s)"
                          % (r, ln, kind, KINDS[kind]))
    for (rel, kind), (n, _) in sorted(ALLOWED_SPELLED.items()):
        if counts[(rel, kind)] < n:
            bad += 1
            print("%s: ALLOWED_SPELLED expects %d %s site(s), found %d; update the list"
                  % (rel, n, kind, counts[(rel, kind)]))
    if not bad:
        print("ok: no spelled-out matching idioms outside the %d documented exceptions"
              % sum(n for n, _ in ALLOWED_SPELLED.values()))
    return 1 if bad else 0


# --functions: which ROM functions still carry a workaround (#662). A
# site of one of these kinds inside a function's definition (or in an
# inline function or macro the function uses) counts against it.
# asm_label (a symbol alias) and the file-scope kinds don't change a
# function's code, so they don't count, except a file-scope asm block
# that defines a function (`.type NAME, function`), which counts as one
# function with a workaround.
WORKAROUND_KINDS = (
    "pin", "pin_macro", "empty", "empty_volatile", "barrier_macro", "use", "use_volatile",
    "use_macro", "keep", "keep_volatile", "keep_macro", "hold", "hold_volatile", "hold_macro",
    "const", "const_volatile", "const_macro", "mem_barrier", "mem_barrier_macro",
    "reg_clobber", "clobber_macro", "mem_ref", "mem_ref_macro", "empty_other", "insn", "pool",
    "box_addr", "field_cast_store", "field_cast_load", "self_init", "non_matching", "naked",
    "volatile_local")
# Any include/match.h macro, including the ones with no kind of their own
# (MATCH_KEEP_EXPR).
ANY_MATCH_MACRO = re.compile(r"\bMATCH_[A-Z0-9_]+\s*\(")
FUNC_NAME = re.compile(r"(?:operator\s*\S+|~?[A-Za-z_]\w*(?:\s*::\s*~?[A-Za-z_]\w*)*)\s*$")
NOT_FUNC_NAME = {"__attribute__", "asm", "__asm__", "if", "while", "for", "switch", "return",
                 "sizeof"}
ASM_FUNCTION = re.compile(r"\.type\s+(\w+)\s*,\s*[%@]?function")
DEFINE = re.compile(r"^[ \t]*#[ \t]*define[ \t]+(\w+)(?:[^\n]*\\\n)*[^\n]*", re.M)


def blank_preprocessor(text):
    """Blank the preprocessor lines (and their continuations), keeping
    every offset."""
    out = list(text)
    for m in re.finditer(r"^[ \t]*#(?:[^\n]*\\\n)*[^\n]*", text, re.M):
        for i in range(m.start(), m.end()):
            if out[i] != "\n":
                out[i] = " "
    return "".join(out)


def head_function_name(head):
    """The name a definition's head (the text before its `{`) declares:
    the identifier (`f`, `Class::Method`, `~Class`) before the first
    top-level `(`, or None if it isn't a function head."""
    depth = 0
    for i, c in enumerate(head):
        if c == "(":
            if depth == 0:
                m = FUNC_NAME.search(head[:i])
                if m:
                    name = re.sub(r"\s+", "", m.group(0))
                    if name.split("::")[-1] not in NOT_FUNC_NAME:
                        return name
            depth += 1
        elif c == ")":
            depth -= 1
    return None


def function_spans(text):
    """The function definitions in comment- and preprocessor-blanked
    text, at file scope, in `extern "C" {}`/namespaces or in a class body:
    [(name, head_start, end, head)], `end` just past the closing brace."""
    found = []
    stack = []  # one kind per open brace: func, other, class, open
    last = 0  # where the current declaration's head starts
    i, n = 0, len(text)
    while i < n:
        c = text[i]
        if c in "\"'":
            j = i + 1
            while j < n and text[j] != c:
                j += 2 if text[j] == "\\" else 1
            i = j + 1
            continue
        if stack and stack[-1][0] in ("func", "other"):
            if c == "{":
                stack.append(("other", None))
            elif c == "}":
                kind, info = stack.pop()
                if kind == "func":
                    found.append(info + (i + 1,))
                    last = i + 1
            i += 1
            continue
        if c == ";":
            last = i + 1
        elif c == "{":
            head = text[last:i]
            h = head.strip()
            if re.fullmatch(r'extern\s+"C"', h) or re.match(r"namespace\b", h):
                stack.append(("open", None))
            elif "=" in re.sub(r"\([^()]*\)", "", h) and "operator" not in h:
                stack.append(("other", None))  # an initialiser
            elif re.search(r"\b(?:struct|class|union|enum)\b", h) and "(" not in h:
                stack.append(("class", None))
            else:
                name = head_function_name(h)
                if name:
                    start = last + len(head) - len(head.lstrip())
                    stack.append(("func", (name, start, h)))
                else:
                    stack.append(("other", None))
            last = i + 1
        elif c == "}":
            if stack:
                stack.pop()
            last = i + 1
        elif c == ":" and stack and stack[-1][0] == "class" \
                and re.search(r"\b(?:public|private|protected)\s*$", text[last:i]):
            last = i + 1
        i += 1
    return [(name, start, end, h) for name, start, h, end in found]


def report_objects():
    """The objects tools/report_units.py turns into report units, as
    paths relative to build/crashbandicootxs/ without `.o`: the code
    decomp.dev counts (crt0/boot and the hand-written assembly have no
    unit)."""
    sys.path.insert(0, os.path.join(ROOT, "tools"))
    import report_units
    objs = set()
    for _, base, _ in report_units.UNITS + report_units.IWRAM_UNITS:
        if base and base != report_units.HANDWRITTEN:
            objs.add(os.path.splitext(base)[0])
    return objs


def object_function_count(obj):
    """The number of functions (FUNC symbols) defined in a built object."""
    import subprocess
    out = subprocess.run(["arm-none-eabi-readelf", "-sW", obj], capture_output=True,
                         text=True, check=True).stdout
    return sum(1 for ln in out.splitlines()
               if re.search(r"\sFUNC\s", ln) and not re.search(r"\sUND\s", ln))


def workaround_functions():
    """Every function definition, as {(rel, name): [kinds]} for those with
    a workaround, plus the set of ROM-function definitions [(rel, name)]
    (the non-inline ones in .c/.cpp files and file-scope asm functions)."""
    defs = []  # (rel, name, inline, body_text, kinds)
    macros = {}  # macro name -> kinds
    for d in SCAN_DIRS:
        for dirpath, _, files in sorted(os.walk(os.path.join(ROOT, d))):
            for fn in sorted(files):
                p = os.path.join(dirpath, fn)
                rel = os.path.relpath(p, ROOT)
                if not fn.endswith((".c", ".h", ".cpp", ".hpp")) or rel in SKIP_FILES:
                    continue
                offsets = []
                text = scan_file(p, rel, collections.defaultdict(list), offsets)
                sites = [(k, pos) for k, pos in offsets if k in WORKAROUND_KINDS]
                seen = set(pos for _, pos in offsets)
                sites += [(m.group(0).rstrip("( \t\n"), m.start())
                          for m in ANY_MATCH_MACRO.finditer(text) if m.start() not in seen]
                for m in DEFINE.finditer(text):
                    kinds = [k for k, pos in sites if m.start() <= pos < m.end()]
                    if kinds:
                        macros[m.group(1)] = kinds
                header = fn.endswith((".h", ".hpp"))
                for name, start, end, head in function_spans(blank_preprocessor(text)):
                    kinds = [k for k, pos in sites if start <= pos < end]
                    inline = header or bool(re.search(r"\binline\b", head))
                    defs.append((rel, name, inline, text[start:end], kinds))
                for k, pos in offsets:
                    if k == "file_asm_other":
                        end = balanced(text, text.index("(", pos))
                        for m in ASM_FUNCTION.finditer(text[pos:end]):
                            defs.append((rel, m.group(1), False, "", ["file_asm"]))
    # An inline function's or macro's workaround is in the code of every
    # function that uses it (the inline function itself isn't a ROM
    # function unless it's emitted out of line, which isn't counted).
    carriers = dict(macros)
    changed = True
    while changed:
        changed = False
        for rel, name, inline, body, kinds in defs:
            key = name.split("::")[-1]
            if not inline:
                continue
            used = [c for c in carriers if c != key and re.search(r"\b%s\b" % c, body)]
            if (kinds or used) and key not in carriers:
                carriers[key] = kinds or ["via " + used[0]]
                changed = True
    dirty = {}
    rom = []
    for rel, name, inline, body, kinds in defs:
        if inline:
            continue
        rom.append((rel, name))
        used = ["via " + c for c in carriers if re.search(r"\b%s\b" % c, body)]
        if kinds or used:
            dirty[(rel, name)] = sorted(set(kinds)) + used
    return dirty, rom


def function_counts():
    """The functions the progress report counts, per object: (total,
    with_wa, dirty), where total and with_wa are Counters of the FUNC
    symbols and of the functions with a workaround, keyed by object path
    (relative to build/crashbandicootxs/, no `.o`), and dirty is
    workaround_functions()'s. Needs a build (`make`); prints why and
    returns None if an object is missing or the counts don't add up.
    tools/badges.py uses it too."""
    dirty, rom = workaround_functions()
    total = collections.Counter()
    missing = []
    for obj in sorted(report_objects()):
        path = os.path.join(ROOT, "build", "crashbandicootxs", obj + ".o")
        if not os.path.exists(path):
            missing.append(obj)
            continue
        total[obj] = object_function_count(path)
    if missing:
        print("not built: %s%s; run `make` first"
              % (", ".join(missing[:5]), " ..." if len(missing) > 5 else ""), file=sys.stderr)
        return None
    with_wa = collections.Counter()
    for rel, name in dirty:
        obj = os.path.splitext(rel)[0]
        if obj not in total:
            continue  # not a report unit (bios_util.cpp)
        with_wa[obj] += 1
    for obj in total:
        if with_wa[obj] > total[obj]:
            print("%s: %d functions with a workaround but only %d in the object"
                  % (obj, with_wa[obj], total[obj]), file=sys.stderr)
            return None
    return total, with_wa, dirty


def functions_report(list_dirty):
    """--functions: how many ROM functions have no workaround, overall and
    per directory. The totals are the FUNC symbols of the built objects
    the progress report counts, so it needs a build (`make`)."""
    counts = function_counts()
    if counts is None:
        return 1
    total, with_wa, dirty = counts
    n = sum(total.values())
    w = sum(with_wa.values())
    print("%d/%d functions with no matching workarounds (%.1f%%); %d with at least one"
          % (n - w, n, 100.0 * (n - w) / n, w))
    print()
    print("| Directory | Functions | No workarounds | With workarounds |")
    print("|---|---:|---:|---:|")
    by_dir = collections.defaultdict(lambda: [0, 0])
    for obj, t in total.items():
        d = os.path.dirname(obj)
        if d.startswith("lib/"):
            d = "/".join(d.split("/")[:2])
        by_dir[d][0] += t
        by_dir[d][1] += with_wa[obj]
    for d, (t, wa) in sorted(by_dir.items()):
        print("| `%s/` | %d | %d | %d |" % (d, t, t - wa, wa))
    if list_dirty:
        print()
        for (rel, name), kinds in sorted(dirty.items()):
            if os.path.splitext(rel)[0] in total:
                print("%s: %s: %s" % (rel, name, ", ".join(kinds)))
    return 0


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--files", action="store_true", help="list files per kind")
    ap.add_argument("--kind", help="list every site of one kind")
    ap.add_argument("--show", action="store_true",
                    help="with --kind, print each site's source line")
    ap.add_argument("--list-kinds", action="store_true")
    ap.add_argument("--convert", metavar="KIND[,KIND]",
                    help="rewrite every site of these kinds in src/, lib/ and include/ "
                         "to its match.h macro, adding the #include")
    ap.add_argument("--check-macros", action="store_true",
                    help="check that each match.h macro expands to the tokens "
                         "of the spelling it replaces")
    ap.add_argument("--check", action="store_true",
                    help="fail if an idiom that has a match.h macro is spelled out "
                         "outside ALLOWED_SPELLED, or a REDUNDANT_KINDS site appears")
    ap.add_argument("--functions", action="store_true",
                    help="count the functions with no matching workarounds, overall and "
                         "per directory (needs a build); with --files, also list the "
                         "functions that have one")
    args = ap.parse_args()

    if args.functions:
        return functions_report(args.files)

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
        for d in SCAN_DIRS:
            for dirpath, _, files in sorted(os.walk(os.path.join(ROOT, d))):
                for fn in sorted(files):
                    p = os.path.join(dirpath, fn)
                    if fn.endswith((".c", ".h", ".cpp", ".hpp")) and os.path.relpath(p, ROOT) not in SKIP_FILES:
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
                if fn.endswith((".c", ".h", ".cpp", ".hpp")) and rel not in SKIP_FILES:
                    scan_file(p, rel, hits)

    if args.check:
        return check_spelled(hits)

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
