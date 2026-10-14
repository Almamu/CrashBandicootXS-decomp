#!/usr/bin/env python3
"""Scans the matched C sources (src/**/*.c and lib/**/*.c) for raw
pointer-arithmetic field access and raw hardware addresses, the sites
docs/workflow.md step 7 turns into named struct fields and REG_* macros.

tools/report_units.py uses scan_cleanup_candidates() to set each
objdiff unit's metadata.complete (docs/decomp_dev.md, "Matched" vs
"complete").

Usage: python3 tools/cleanup_scan.py [FILE]   # Markdown report of every
                                              # site, by subsystem and file
                                              # (default `-`, stdout)

It is read-only.
"""
import argparse
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SRC_DIR = ROOT / "src"
# The third-party/SDK libraries (docs/libraries.md): matched C too.
LIB_DIR = ROOT / "lib"


def c_sources():
    """Every C source under src/ and lib/."""
    return sorted(SRC_DIR.rglob("*.c")) + sorted(LIB_DIR.rglob("*.c"))


# Not an exhaustive linter: it looks for the spellings this project's
# matching passes actually produced, and leaves the rest to review. The
# kinds of site it reports (CLEANUP_KIND_DESCRIPTIONS):
#
#   offset_cast  *(T *)(p + 0xNN), *(T *)((u8 *)p + N), *(T **)(self + N):
#                a dereferenced cast of pointer-plus-constant, anywhere on
#                the line. The scan before #550 took a line starting with
#                `*` for a comment continuation and skipped it, and only
#                allowed one `*` in the cast type.
#   cast_index   ((T *)p)[N]: a constant index into a pointer cast.
#   unk_index    s->unk_XX[...]: an index into a struct's unnamed byte
#                array (a field still hiding behind padding, e.g. the old
#                actor_self unk_2C[] that ACTOR_RECORD replaced).
#   byte_deref   *((u8 *)p + 0x42): a byte pointer plus a constant,
#                dereferenced.
#   hw_addr      a literal in a mapped I/O, palette, VRAM or OAM range,
#                used as an address (see is_hw_address_literal).
#   offset_addr  (u8 *)p + 0x94 left as an address. Advisory only: it is
#                often a field's address, but just as often a payload
#                past a header, so it is listed but never blocks
#                "complete".
#
# offset_cast/cast_index/unk_index/byte_deref are the "offset" sites
# (OFFSET_KINDS); hw_addr is reported separately.

INT_LIT = r"(?:0[xX][0-9A-Fa-f]+|\d+)[uUlL]*"
# A pointer type inside a cast: `u8 *`, `const struct foo **`, `vu16 *`.
PTR_TYPE = (r"(?:(?:const|volatile|struct|union|enum|unsigned|signed)\s+)*"
            r"[A-Za-z_]\w*(?:\s+(?:const|volatile))?\s*\*+(?:\s*(?:const|volatile))?\s*")
# `*(T *)(` - a dereferenced cast whose operand is parenthesised; the
# operand itself is checked by find_offset_casts.
DEREF_CAST_OPEN_RE = re.compile(r"\*\s*\(\s*" + PTR_TYPE + r"\)\s*\(")
CAST_INDEX_RE = re.compile(
    r"\(\s*\(\s*" + PTR_TYPE + r"\)\s*[A-Za-z_][\w.>\-]*\s*\)\s*\[\s*" + INT_LIT + r"\s*\]"
)
UNK_INDEX_RE = re.compile(r"(?:->|\.)\s*(?:unk_?[0-9A-Fa-f]+|pad\w*|filler\w*)\s*\[")
# `(u8 *)p + 0xNN`: a byte pointer plus a constant. Dereferenced right
# away (`*((u8 *)p + 0x42)`) it's a byte_deref; otherwise (offset_addr)
# it takes a field's address.
BYTE_OFFSET_RE = re.compile(
    r"\(\s*(?:const\s+)?(?:u8|s8|char|unsigned\s+char)\s*\*\s*\)\s*[A-Za-z_][\w.>\-]*(?:\[[^\]]*\])?\s*[+-]\s*" + INT_LIT + r"(?![\w.])"
)
DEREF_BEFORE_RE = re.compile(r"\*\s*\(\s*$")
DEREF_CAST_BEFORE_RE = re.compile(r"\*\s*\(\s*" + PTR_TYPE + r"\)\s*\(\s*$")
# A `+ N`/`- N` term at the top level of an expression.
CONST_TERM_RE = re.compile(r"[+-]\s*" + INT_LIT + r"\s*(?:$|[+-])")
HEX_LIT_RE = re.compile(r"\b0[xX]([0-9A-Fa-f]{7,8})[uUlL]*\b")
REGISTER_ASM_RE = re.compile(r"\bregister\b.*\basm\s*\(")

# The GBA's memory-mapped hardware ranges [start, end). A value outside
# them that only starts with 0x04-0x07 (the GAX period table's
# 0x040013EF, 0x0500F98C, ...) is a plain number, not an address.
HW_RANGES = (
    (0x04000000, 0x04000400),  # I/O registers
    (0x04000800, 0x04000804),  # memory control
    (0x05000000, 0x05000400),  # palette RAM
    (0x06000000, 0x06018000),  # VRAM
    (0x07000000, 0x07000400),  # OAM
)
# Calls whose integer arguments are control words, not addresses: the
# CpuSet word 0x04000080 (32-bit, 0x80 units) reads as SOUNDCNT_L by
# value alone (#541).
CONTROL_WORD_CALL_RE = re.compile(r"\b(?:Cpu(?:Fast)?Set|Dma\w*|DMA\w*)\s*\(")
POINTER_CAST_BEFORE_RE = re.compile(r"\(\s*" + PTR_TYPE + r"\)\s*\(?\s*$")
DATA_ROW_RE = re.compile(r"\b0[xX][0-9A-Fa-f]+[uUlL]*\s*,")

CLEANUP_KIND_DESCRIPTIONS = {
    "offset_cast": "dereferenced cast of pointer + constant offset",
    "cast_index": "constant index into a pointer cast",
    "unk_index": "index into a struct's unnamed `unk_`/pad array",
    "byte_deref": "dereferenced byte pointer + constant offset",
    "hw_addr": "raw hardware address literal",
    "offset_addr": "byte pointer + constant offset (a field's address; advisory)",
}
# The kinds that keep a file from counting as "complete" (report_units.py).
# offset_addr is advisory: `(u8 *)hdr + 8` is often legitimately a
# payload past a header rather than a field.
OFFSET_KINDS = ("offset_cast", "cast_index", "unk_index", "byte_deref")


def strip_c_comments(text):
    """Blanks out comments and string/char literals, keeping every newline
    (so line numbers survive) and every column position."""
    out = []
    i, n = 0, len(text)
    while i < n:
        c = text[i]
        if text.startswith("/*", i):
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
            out.append("".join(ch if ch == "\n" else " " for ch in text[i:j]))
            i = j
        elif text.startswith("//", i):
            j = text.find("\n", i)
            j = n if j < 0 else j
            out.append(" " * (j - i))
            i = j
        elif c in "\"'":
            j = i + 1
            while j < n and text[j] not in (c, "\n"):
                j += 2 if text[j] == "\\" else 1
            j = min(j + 1, n)
            out.append(c + " " * max(j - i - 2, 0) + (c if j - i >= 2 else ""))
            i = j
        else:
            out.append(c)
            i += 1
    return "".join(out)


def _paren_contents(line, open_idx):
    """The text inside the parenthesis opened at line[open_idx], or None
    if it doesn't close on this line."""
    depth = 0
    for j in range(open_idx, len(line)):
        if line[j] == "(":
            depth += 1
        elif line[j] == ")":
            depth -= 1
            if depth == 0:
                return line[open_idx + 1:j]
    return None


def _top_level(expr):
    """expr with every nested (...)/[...] group collapsed to `_`, so only
    its top-level operators remain."""
    out, depth = [], 0
    for ch in expr:
        if ch in "([":
            if depth == 0:
                out.append("_")
            depth += 1
        elif ch in ")]":
            depth -= 1
        elif depth == 0:
            out.append(ch)
    return "".join(out)


def count_offset_casts(code):
    """How many `*(T *)(base + N)` reads/writes the line has. The operand
    must add a constant at its top level to something that isn't one: a
    cast of a plain constant is a hw_addr instead."""
    hits = 0
    for m in DEREF_CAST_OPEN_RE.finditer(code):
        operand = _paren_contents(code, m.end() - 1)
        if operand is None:
            continue
        top = _top_level(operand)
        non_literal = re.sub(r"\b" + INT_LIT + r"\b", "", top)
        if CONST_TERM_RE.search(top) and re.search(r"[A-Za-z_]", non_literal):
            hits += 1
    return hits


def is_hw_address_literal(code, m):
    """Whether hex literal `m` (a HEX_LIT_RE match in `code`) is a hardware
    address rather than a number that only looks like one. It has to fall
    in a mapped range (HW_RANGES); a pointer cast right before it settles
    it. Otherwise it must not be an argument of a CpuSet/DMA-style call
    (control words), must be halfword-aligned, and must not sit in a data
    table row (three or more comma-separated hex literals)."""
    value = int(m.group(1), 16)
    if not any(lo <= value < hi for lo, hi in HW_RANGES):
        return False
    before = code[:m.start()]
    if POINTER_CAST_BEFORE_RE.search(before):
        return True
    if CONTROL_WORD_CALL_RE.search(before):
        return False
    # An operand of a bitwise operator is a flag mask: `control |=
    # 0x4000000` sets CpuSet's 32-bit bit, not I/O register 0x04000000.
    if re.search(r"[|&^]=?\s*$", before) or re.match(r"[uUlL]*\s*[|&^]", code[m.end():]):
        return False
    if value & 1:
        return False
    return len(DATA_ROW_RE.findall(code)) < 3


def scan_file_cleanup_sites(path):
    """Every cleanup site in one C file as (lineno, kind, code), skipping
    `#if NON_MATCHING` blocks (parked functions, tracked separately), and
    the file's register-pin/inline-asm count."""
    raw_lines = path.read_text().splitlines()
    code_lines = strip_c_comments("\n".join(raw_lines)).split("\n")
    sites = []
    register_asm_count = 0
    # One entry per open #if: True when it, or one around it, is a
    # NON_MATCHING guard.
    if_stack = []
    for lineno, (raw, code) in enumerate(zip(raw_lines, code_lines), 1):
        stripped = code.strip()
        if stripped.startswith("#"):
            directive = stripped[1:].strip()
            if re.match(r"if(?:n?def)?\b", directive):
                parked = bool(re.match(r"if(?:def)?\s+NON_MATCHING\b", directive))
                if_stack.append(parked or (bool(if_stack) and if_stack[-1]))
            elif directive.startswith("endif") and if_stack:
                if_stack.pop()
            continue
        if (if_stack and if_stack[-1]) or not stripped:
            continue  # parked functions aren't this scan's job
        shown = raw.strip()
        sites += [(lineno, "offset_cast", shown)] * count_offset_casts(code)
        sites += [(lineno, "cast_index", shown)] * len(CAST_INDEX_RE.findall(code))
        sites += [(lineno, "unk_index", shown)] * len(UNK_INDEX_RE.findall(code))
        for m in BYTE_OFFSET_RE.finditer(code):
            before = code[:m.start()]
            if DEREF_BEFORE_RE.search(before):
                sites.append((lineno, "byte_deref", shown))
            elif not DEREF_CAST_BEFORE_RE.search(before):  # else an offset_cast
                sites.append((lineno, "offset_addr", shown))
        for m in HEX_LIT_RE.finditer(code):
            if is_hw_address_literal(code, m):
                sites.append((lineno, "hw_addr", shown))
        if REGISTER_ASM_RE.search(code):
            register_asm_count += 1
    return sites, register_asm_count


def subsystem_of(rel_path):
    """src/engine|game/<ns>/... -> engine|game/<ns>; src/<subsystem>/... ->
    <subsystem>; lib/<name>/... -> lib/<name>."""
    parts = Path(rel_path).parts
    if len(parts) >= 4 and parts[0] == "src" and parts[1] in ("engine", "game"):
        return f"{parts[1]}/{parts[2]}"
    if len(parts) >= 3 and parts[0] == "src":
        return parts[1]
    if len(parts) >= 3 and parts[0] == "lib":
        return f"lib/{parts[1]}"
    return parts[0]


def scan_cleanup_candidates():
    """Scans already-matched src/**/*.c and lib/**/*.c for the raw
    pointer-arithmetic offsets and raw hardware addresses that
    docs/workflow.md step 7 says should become named struct fields /
    REG_* macros (the kinds above). Also counts register-pin/inline-asm
    density per file as a rough "how risky would cleanup here be" signal,
    per the project's own carve-out for register-allocation-sensitive
    code.

    One entry per file with at least one site: `sites` holds every
    (lineno, kind, code); `offset_hits`/`hw_addr_hits` split them into
    (lineno, code) lists for the report and report_units.py."""
    results = []
    for path in c_sources():
        sites, register_asm_count = scan_file_cleanup_sites(path)
        if not sites:
            continue
        rel = str(path.relative_to(ROOT))
        offset_hits = [(ln, code) for ln, kind, code in sites if kind in OFFSET_KINDS]
        hw_addr_hits = [(ln, code) for ln, kind, code in sites if kind == "hw_addr"]
        results.append({
            "file": rel,
            "subsystem": subsystem_of(rel),
            "sites": sites,
            "offset_hits": offset_hits,
            "hw_addr_hits": hw_addr_hits,
            # False when the file only has advisory (offset_addr) sites.
            "blocking": bool(offset_hits or hw_addr_hits),
            "register_asm_count": register_asm_count,
        })
    return results


def render_cleanup_report(candidates):
    """A Markdown report of every cleanup site, grouped by subsystem and
    then file, with per-subsystem totals by kind up front."""
    kinds = list(CLEANUP_KIND_DESCRIPTIONS)
    by_sub = {}
    for entry in candidates:
        by_sub.setdefault(entry["subsystem"], []).append(entry)

    def kind_counts(entries):
        counts = dict.fromkeys(kinds, 0)
        for e in entries:
            for _, kind, _ in e["sites"]:
                counts[kind] += 1
        return counts

    total = kind_counts(candidates)
    order = sorted(by_sub, key=lambda s: (-sum(kind_counts(by_sub[s]).values()), s))
    lines = [
        "# Raw-offset cleanup sites",
        "",
        f"{sum(total.values())} sites in {len(candidates)} files.",
        "",
        "| Subsystem | Files | " + " | ".join(f"`{k}`" for k in kinds) + " | Total |",
        "|---|---:|" + "---:|" * len(kinds) + "---:|",
    ]
    for sub in order:
        c = kind_counts(by_sub[sub])
        lines.append(f"| {sub} | {len(by_sub[sub])} | "
                     + " | ".join(str(c[k]) for k in kinds) + f" | {sum(c.values())} |")
    lines.append(f"| **total** | {len(candidates)} | "
                 + " | ".join(str(total[k]) for k in kinds) + f" | {sum(total.values())} |")
    lines += ["", "Kinds:", ""]
    lines += [f"- `{k}`: {d}" for k, d in CLEANUP_KIND_DESCRIPTIONS.items()]
    for sub in order:
        lines += ["", f"## {sub}"]
        for e in sorted(by_sub[sub], key=lambda e: e["file"]):
            pins = f", {e['register_asm_count']} register pin(s)" if e["register_asm_count"] else ""
            lines += ["", f"### `{e['file']}` ({len(e['sites'])} sites{pins})", ""]
            lines += [f"- {ln} `{kind}`: `{code}`" for ln, kind, code in e["sites"]]
    return "\n".join(lines) + "\n"


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("out", nargs="?", default="-", metavar="FILE",
                    help="where to write the Markdown report (`-` for stdout, the default)")
    args = ap.parse_args()
    report = render_cleanup_report(scan_cleanup_candidates())
    if args.out == "-":
        sys.stdout.write(report)
    else:
        Path(args.out).write_text(report)
        print(f"Wrote {args.out}", file=sys.stderr)


if __name__ == "__main__":
    main()
