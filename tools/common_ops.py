#!/usr/bin/env python3
"""Find the common operations that are written out by hand (#667).

Scans the C sources under src/ and include/ (not the data tables in
src/data/) for the expression shapes that the shared helpers in
include/math_util.h and include/entity_bits.h can name, and for the
file-local helper macros that several files define for themselves.
Comments and strings are blanked out first, and #define bodies aren't
use sites (they're listed by --macros instead). It doesn't run the
preprocessor, so a site that is already a helper isn't counted: the
counts go down as the sites are converted.

Shapes (--list-shapes): the fixed-point shifts (Q8 `>> 8`/`<< 8`, Q12,
Q16), the Q8 product `(a * b) >> 8`, sine/cosine lookups into
gSineTable (also through a local alias of the table), min/max/abs
ternaries and the clamp `if`s, RGB555 packing, OBJ tile index
arithmetic, the bit-of-a-byte-array tests and the inline copies of
MarkEntityGone's bitmap set.

A shape is a candidate, not a verdict: before replacing a site, check
that the helper expands to exactly the same expression (operand order,
casts, signedness, parentheses), and compare the objects after.

Usage:
  tools/common_ops.py                      every site, file:line shape
  tools/common_ops.py --shape S [--show]   only shape S (with the line)
  tools/common_ops.py --macros             file-local helper macros that
                                           are defined in more than one file
  tools/common_ops.py --report [OUT]       Markdown: counts per shape, per
                                           subsystem and per file, and the
                                           duplicated macros (OUT or stdout)
"""

import argparse
import collections
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from magic_numbers import blank_directives, line_of, strip_comments  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SCAN_DIRS = ("src", "include")
SKIP_DIRS = (os.path.join("src", "data"),)

# An operand: an identifier, a field chain or a parenthesized group with
# no nested parentheses. Good enough for a survey.
OPND = r"(?:\([^()]*\)|[A-Za-z_]\w*(?:(?:->|\.)\w+|\[[^\]]*\])*|0[xX][0-9a-fA-F]+|\d+)"

# shape -> (description, helper(s), regex). Each regex match is one site.
SHAPES = collections.OrderedDict(
    [
        ("q8_mul", ("Q8 product `(a * b) >> 8`", "Q8_MUL",
                    re.compile(r"\((?:[^()]|\([^()]*\))*[^*/]\*[^*/](?:[^()]|\([^()]*\))*\)\s*>>\s*8\b(?!\s*[0-9xX])"))),
        ("q8_shr", ("Q8 to integer `>> 8`, `>>= 8` (not counting q8_mul)", "Q8_TO_INT",
                    re.compile(r">>=?\s*(?:8|0x8)\b"))),
        ("q8_shl", ("integer to Q8 `<< 8`, `<<= 8`", "INT_TO_Q8",
                    re.compile(r"<<=?\s*(?:8|0x8)\b"))),
        ("q12_shr", ("`>> 12`, `>>= 12`", "Q12_TO_INT", re.compile(r">>=?\s*(?:12|0xc|0xC)\b"))),
        ("q12_shl", ("`<< 12`, `<<= 12`", "INT_TO_Q12", re.compile(r"<<=?\s*(?:12|0xc|0xC)\b"))),
        ("q16_shr", ("`>> 16`, `>>= 16`", "Q16_TO_INT", re.compile(r">>=?\s*(?:16|0x10)\b"))),
        ("q16_shl", ("`<< 16`, `<<= 16`", "INT_TO_Q16", re.compile(r"<<=?\s*(?:16|0x10)\b"))),
        ("sine", ("sine lookup `gSineTable[i & 0xFF]` (or a local alias)", "SIN_Q8", None)),
        ("cosine", ("cosine lookup, the quarter-turn `+ 0x40` offset", "COS_Q8", None)),
        ("min_max_ternary", ("`a < b ? a : b` and friends", "MIN/MAX",
                             re.compile(r"(" + OPND + r")\s*([<>]=?)\s*(" + OPND + r")\s*\?\s*(" + OPND + r")\s*:\s*(" + OPND + r")"))),
        ("clamp_if", ("`if (x > hi) x = hi;` (and `<`, `>=`, `<=`)", "LIMIT_MAX/LIMIT_MIN",
                      re.compile(r"\bif\s*\(\s*(" + OPND + r")\s*([<>]=?)\s*(" + OPND + r")\s*\)\s*\{?\s*\1\s*=\s*([^;=][^;]*);"))),
        ("clamp_index", ("`if (i >= n) i = n - 1;` (last valid index)", "CLAMP_INDEX",
                         re.compile(r"\bif\s*\(\s*(" + OPND + r")\s*>=\s*(" + OPND + r")\s*\)\s*\{?\s*\1\s*=\s*\2\s*-\s*1\s*;"))),
        ("abs", ("`x < 0 ? -x : x`, `if (x < 0) x = -x;`", "ABS/MAKE_ABS",
                 re.compile(r"(" + OPND + r")\s*<\s*0\s*\?\s*-\s*\1\s*:\s*\1|\bif\s*\(\s*(" + OPND + r")\s*<\s*0\s*\)\s*\{?\s*\2\s*=\s*-\s*\2\s*;"))),
        ("rgb555", ("RGB555 packing `r | g << 5 | b << 10`", "RGB16 (gba/defines.h)",
                    re.compile(r"<<\s*5\b[^;]*<<\s*10\b|<<\s*10\b[^;]*<<\s*5\b"))),
        ("obj_tile", ("OBJ tile index `(addr - OBJ_VRAM0) >> 5`, `tileNum` arithmetic", "",
                      re.compile(r"OBJ_VRAM0[^;]*(?:>>\s*5|/\s*32)\b|tileNum\s*=[^;]*(?:>>\s*5|/\s*32|<<\s*5|\*\s*32)\b"))),
        ("byte_bitmap", ("bit of a byte array `[n >> 3] & (1 << (n & 7))`", "",
                         re.compile(r"\[[^\]]*(?:>>\s*3|/\s*8)\s*\][^;]*(?:&\s*7\b|%\s*8\b)"))),
        ("gone_bit_inline", ("inline MarkEntityGone bitmap set (`bits0Copy`/`+ 0x108`, `/ 32`)",
                             "ENTITY_SET_GONE_BIT", None)),
    ]
)

SINE_ALIAS = re.compile(r"\b(\w+)\s*=\s*gSineTable\b")
GONE_BASE = re.compile(r"bits0Copy|\+\s*0x108\b")
GONE_WORD = re.compile(r"/=?\s*32\b|>>=?\s*5\b|<<\s*5\b")
DEFINE_RE = re.compile(r"^[ \t]*#[ \t]*define[ \t]+(\w+)(\([^)]*\))?[ \t]*(.*)$")
# Names that are expected in several files (header guards, tables).
MACRO_SKIP = re.compile(r"^(GUARD_|__|_[A-Z])|_H_?$|^NON_MATCHING$")


def scan_dirs():
    for top in SCAN_DIRS:
        for dirpath, dirnames, filenames in os.walk(os.path.join(ROOT, top)):
            rel_dir = os.path.relpath(dirpath, ROOT)
            if any(rel_dir == d or rel_dir.startswith(d + os.sep) for d in SKIP_DIRS):
                dirnames[:] = []
                continue
            dirnames.sort()
            for name in sorted(filenames):
                if name.endswith((".c", ".h")):
                    path = os.path.join(dirpath, name)
                    yield path, os.path.relpath(path, ROOT)


def scan_file(path, rel):
    raw = open(path, encoding="utf-8", errors="replace").read()
    text = blank_directives(strip_comments(raw))
    src_lines = raw.split("\n")
    hits = []
    taken = set()  # (shape family, offset) already counted

    def add(shape, offset):
        line = line_of(text, offset)
        hits.append((rel, line, shape, src_lines[line - 1].strip()))

    for shape, (_, _, rx) in SHAPES.items():
        if rx is None:
            continue
        for m in rx.finditer(text):
            if shape == "q8_mul":
                # the `>> 8` of this product isn't also a q8_shr
                taken.add(("q8_shr", text.rfind(">>", m.start(), m.end())))
            elif shape == "q8_shr" and ("q8_shr", m.start()) in taken:
                continue
            elif shape == "min_max_ternary":
                a, b, t, f = m.group(1), m.group(3), m.group(4), m.group(5)
                if {a, b} != {t, f}:
                    continue
            elif shape == "clamp_if":
                if re.match(r"\s*" + re.escape(m.group(3)) + r"\s*-\s*1\s*$", m.group(4)) and m.group(2) == ">=":
                    continue  # clamp_index
                if m.group(4).strip() != m.group(3).strip():
                    continue
            add(shape, m.start())

    # sine/cosine: gSineTable and any local alias of it
    names = {"gSineTable"} | set(SINE_ALIAS.findall(text))
    look = re.compile(r"\b(?:" + "|".join(sorted(map(re.escape, names))) + r")\s*\[")
    for m in look.finditer(text):
        depth, j = 0, m.end() - 1
        while j < len(text):
            if text[j] == "[":
                depth += 1
            elif text[j] == "]":
                depth -= 1
                if depth == 0:
                    break
            j += 1
        index = text[m.end():j]
        if text[max(0, text.rfind("\n", 0, m.start())):m.start()].lstrip().startswith("extern"):
            continue
        add("cosine" if re.search(r"\+\s*0x40\b|\+\s*64\b", index) else "sine", m.start())

    # inline gone-bit sets: a `|= ... <<` within a few lines of the 0x108
    # base and the word index
    for m in re.finditer(r"\|=\s*[^;]*<<", text):
        window = text[max(0, m.start() - 700):m.end()]
        tail = window[window.rfind("}", 0, len(window) - (m.end() - m.start())) + 1:]
        if GONE_BASE.search(tail) and GONE_WORD.search(tail):
            add("gone_bit_inline", m.start())
    return hits


def scan():
    hits = []
    for path, rel in scan_dirs():
        hits += scan_file(path, rel)
    order = list(SHAPES)
    hits.sort(key=lambda h: (order.index(h[2]), h[0], h[1]))
    return hits


def scan_macros():
    """name -> list of (rel, line, params, normalized body) for every
    function-like or object-like #define in a .c file or a header."""
    defs = collections.defaultdict(list)
    for path, rel in scan_dirs():
        lines = strip_comments(open(path, encoding="utf-8", errors="replace").read()).split("\n")
        i = 0
        while i < len(lines):
            m = DEFINE_RE.match(lines[i])
            start = i
            if m:
                body = m.group(3)
                while body.rstrip().endswith("\\") and i + 1 < len(lines):
                    body = body.rstrip()[:-1] + " " + lines[i + 1]
                    i += 1
                name = m.group(1)
                if not MACRO_SKIP.search(name):
                    params = re.sub(r"\s+", "", m.group(2) or "")
                    defs[name].append((rel, start + 1, params, re.sub(r"\s+", " ", body).strip()))
            i += 1
    return defs


def duplicated_macros(defs):
    """(by name, by body): names defined in more than one file, and
    bodies (with parameters) that more than one name or file defines."""
    by_name = {n: d for n, d in defs.items() if len({x[0] for x in d}) > 1}
    by_body = collections.defaultdict(list)
    for name, d in defs.items():
        for rel, line, params, body in d:
            if params and len(body) > 20:
                # rename the parameters so `(x) >> 8` and `(v) >> 8` match
                ps = [p for p in params.strip("()").split(",") if p]
                norm = body
                for k, p in enumerate(ps):
                    norm = re.sub(r"\b" + re.escape(p.strip()) + r"\b", f"${k}", norm)
                by_body[(len(ps), norm)].append((name, rel, line))
    by_body = {k: v for k, v in by_body.items() if len({x[1] for x in v}) > 1}
    return by_name, by_body


def subsystem(rel):
    parts = rel.split(os.sep)
    if parts[0] == "src" and len(parts) > 2:
        return parts[1]
    return parts[0]


def report(hits, out):
    w = out.write
    by_shape = collections.Counter(h[2] for h in hits)
    files_by_shape = collections.defaultdict(collections.Counter)
    for h in hits:
        files_by_shape[h[2]][h[0]] += 1

    w("# Common operations written out by hand (#667)\n\n")
    w("Generated by `tools/common_ops.py --report`. Each site is a candidate for a\n")
    w("shared helper; the helper must expand to exactly the expression it replaces.\n\n")
    w("| Shape | What | Helper | Sites | Files |\n|---|---|---|---:|---:|\n")
    for shape, (desc, helper, _) in SHAPES.items():
        h = f"`{helper}`" if helper else ""
        w(f"| `{shape}` | {desc} | {h} | {by_shape[shape]} | {len(files_by_shape[shape])} |\n")
    w(f"| | **Total** | | **{len(hits)}** | **{len({h[0] for h in hits})}** |\n\n")

    w("## Per subsystem\n\n")
    shapes = [s for s in SHAPES if by_shape[s]]
    grid = collections.Counter((subsystem(h[0]), h[2]) for h in hits)
    files_per_sub = collections.defaultdict(set)
    for h in hits:
        files_per_sub[subsystem(h[0])].add(h[0])
    subs = sorted(files_per_sub, key=lambda s: -sum(grid[(s, t)] for t in shapes))
    w("| Subsystem | Files | " + " | ".join(f"`{s}`" for s in shapes) + " | Total |\n")
    w("|---|---:|" + "---:|" * (len(shapes) + 1) + "\n")
    for s in subs:
        row = [grid[(s, t)] for t in shapes]
        w(f"| {s} | {len(files_per_sub[s])} | " + " | ".join(str(c) if c else "" for c in row) + f" | {sum(row)} |\n")
    w("\n")

    w("## Per file\n\n")
    per_file = collections.defaultdict(collections.Counter)
    for h in hits:
        per_file[h[0]][h[2]] += 1
    w("| File | Sites | Shapes |\n|---|---:|---|\n")
    for f, c in sorted(per_file.items(), key=lambda x: (-sum(x[1].values()), x[0])):
        w(f"| {f} | {sum(c.values())} | " + ", ".join(f"{s} {n}" for s, n in sorted(c.items(), key=lambda x: -x[1])) + " |\n")
    w("\n")

    by_name, by_body = duplicated_macros(scan_macros())
    w("## File-local macros defined in more than one file\n\n")
    w("| Macro | Files |\n|---|---|\n")
    for name, d in sorted(by_name.items(), key=lambda x: (-len(x[1]), x[0])):
        w(f"| `{name}` | " + ", ".join(f"{rel}:{line}" for rel, line, _, _ in d) + " |\n")
    w("\n## Macro bodies defined in more than one file (any name)\n\n")
    w("| Macros | Body |\n|---|---|\n")
    for (n, body), d in sorted(by_body.items(), key=lambda x: (-len(x[1]), x[0][1])):
        short = body if len(body) <= 90 else body[:87] + "..."
        w("| " + ", ".join(f"`{name}` ({rel}:{line})" for name, rel, line in d) + f" | `{short}` |\n")
    w("\n")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--shape", choices=list(SHAPES))
    ap.add_argument("--show", action="store_true", help="print the source line too")
    ap.add_argument("--macros", action="store_true", help="file-local macros defined in more than one file")
    ap.add_argument("--report", nargs="?", const="-", metavar="OUT", help="Markdown report")
    ap.add_argument("--list-shapes", action="store_true")
    args = ap.parse_args()

    if args.list_shapes:
        for shape, (desc, helper, _) in SHAPES.items():
            print(f"{shape:16} {desc}" + (f"  [{helper}]" if helper else ""))
        return 0

    if args.macros:
        by_name, by_body = duplicated_macros(scan_macros())
        for name, d in sorted(by_name.items()):
            print(f"{name}: " + ", ".join(f"{rel}:{line}" for rel, line, _, _ in d))
        for (_, body), d in sorted(by_body.items(), key=lambda x: x[0][1]):
            print("same body: " + ", ".join(f"{name} ({rel}:{line})" for name, rel, line in d))
        return 0

    hits = scan()
    if args.shape:
        hits = [h for h in hits if h[2] == args.shape]

    if args.report:
        if args.report == "-":
            report(hits, sys.stdout)
        else:
            with open(args.report, "w") as f:
                report(hits, f)
        return 0

    for rel, line, shape, src in hits:
        print(f"{rel}:{line}: {shape}" + (f"\n    {src}" if args.show else ""))
    print(f"{len(hits)} sites", file=sys.stderr)
    return 0


if __name__ == "__main__":
    sys.exit(main())
