#!/usr/bin/env python3
"""Find the matching workarounds that are no longer needed (#662).

For every workaround site in the given C/C++ sources, try the C without
it, rebuild that one object with the ROM's own compiler and flags, and
keep the removal if the object stays byte-identical. Default is a dry
run that lists what is removable; --write applies the removals.

Sites (each has a mechanical removal):

  MATCH_HOLD_REG(T, x, rN)      becomes a plain local `T x` (same
                                initializer); in a bundle (below) the
                                whole declaration may also be deleted
  MATCH_BARRIER(), MATCH_MEMORY_BARRIER(), MATCH_USE(x), MATCH_USE2(a, b),
  MATCH_USE2_VOLATILE, MATCH_KEEP(x), MATCH_KEEP_VOLATILE, MATCH_HOLD(x),
  MATCH_HOLD_VOLATILE, MATCH_CLOBBER(rN), MATCH_CLOBBER_VOLATILE,
  MATCH_KEEP_MEM(x), MATCH_USE_MEM(x)
                                the statement is deleted
  MATCH_CONST(v, K)             becomes `v = K;`
  MATCH_KEEP_EXPR(T, e)         becomes `((T)e)`
  BOX_ADDR(a)                   becomes `a`
  spelled-out empty-template asm (the few tools/match_idioms.py
  ALLOWED_SPELLED sites)        deleted, or `v = K;` for an "=r" output
                                from one input
  instruction asm               translated to C when every instruction is
                                one of mov/add/sub/neg/lsl/lsr/asr/and/
                                orr/eor/mul/ldr*/str* on %N operands and
                                immediates (`asm("add %0, %1, #0" :
                                "=r"(v) : "r"(x))` becomes `v = x;`);
                                any other asm (labels, swi, .byte, hard
                                registers) has no mechanical removal
  `T x = x;` self-initialisation becomes `T x;`

Headers aren't tried (a header's sites affect every object including
it), nor sites in a #define body or in a preprocessor arm the ROM build
doesn't compile (found by preprocessing the file with a marker at each
site, through the same make rule).

How an object is rebuilt: make runs on the repository's own Makefile,
with OBJ_DIR, C_SUBDIR/C_BUILDDIR, LIB_SUBDIR/LIB_BUILDDIR and
GENERATED_INCLUDE_DIR pointed into a per-worker temporary directory, and
the target is the object there. The worker's source tree mirrors the
file's directory with symlinks, with the trial version of the file in
place of its link. So every object gets exactly the ROM build's compiler
and flags (agbcc, old_agbcc, agbcp, old_agbcp, agbcc_arm(_patched);
OLD_AGBCC_OBJS, CXX_OBJS, NO_IMPLEMENT_INLINES_OBJS, the ARM and -O1
objects, single-object flags), and nothing in the checkout (build/ or
src/) is read as output or written. The generated constants headers are
made once into the temporary directory.

How objects are compared: the .o is parsed as ELF and reduced to
  - every section other than the symbol, string and relocation tables,
    in order: name, type, flags, alignment, entry size, size and bytes;
  - every relocation section, in order: the target section and each
    relocation's offset, type and symbol, the symbol given by name (a
    section symbol by its section's name), binding, type, section and
    value, not by its index;
  - the symbol table as a sorted list of the same descriptions plus size
    (STT_FILE entries left out).
Two objects are the same if those are equal, so a removal can't change
the code, the data, what they refer to or which symbols are defined,
but the order of the symbol table may differ. The baseline is the
unmodified file compiled the same way (and checked against build/'s
object when there is one).

Search: every site is tried alone against the current state (the sites
already removed); the successful ones are kept if they still match all
together, else added one at a time. That repeats until no single
removal succeeds. Then pairs of the remaining sites of one function are
tried, and "bundles": a pin with every MATCH_HOLD/USE/KEEP/CONST site of
its variable in the function (a hold needs all three gone at once).
Each pair or bundle that matches is kept (re-checked against what was
kept before it), and the singles are tried again, until a fixed point.

--write writes the result back: deleted statements take their line,
their trailing comment and a comment block directly above them (when
every line that comment introduces is a removed site and it names a
register or the workaround) with them; `#include "match.h"` goes when no
macro is left; the file is run through clang-format (tools/format.py's
rules) if it is available. That final text is rebuilt and compared once
more before the file is replaced (atomically, with Ctrl-C held off
until all files are written); if it doesn't match, the plain verified
edit is written instead. Comments elsewhere in the file that still name
a removed macro are listed for a manual edit. A dry run never touches
the sources, and nothing is written when interrupted before the end.

Usage:
  tools/match_prune.py src/                   dry run, per-file report and summary
  tools/match_prune.py -v src/util/aabb.cpp     also list the sites kept, and why
  tools/match_prune.py --write src/gfx/       apply the removals
  tools/match_prune.py --list src/            list the sites without building
  tools/match_prune.py --kinds MATCH_BARRIER,MATCH_USE src/
  tools/match_prune.py --json out.json src/   also write the results as JSON

Options: -j N workers (default: CPU count), --no-pairs, --max-pairs N
(per file and round, default 4000), --keep-temp DIR.
"""

import argparse
import collections
import concurrent.futures
import hashlib
import json
import os
import re
import shutil
import signal
import struct
import subprocess
import sys
import tempfile
import threading

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import match_idioms as mi  # noqa: E402

# ---------------------------------------------------------------------------
# Sites

# Macros whose statement is deleted.
DELETE_MACROS = ("MATCH_BARRIER", "MATCH_MEMORY_BARRIER", "MATCH_USE", "MATCH_USE2",
                 "MATCH_USE2_VOLATILE", "MATCH_KEEP", "MATCH_KEEP_VOLATILE", "MATCH_HOLD",
                 "MATCH_HOLD_VOLATILE", "MATCH_CLOBBER", "MATCH_CLOBBER_VOLATILE",
                 "MATCH_KEEP_MEM", "MATCH_USE_MEM")
OTHER_MACROS = ("MATCH_HOLD_REG", "MATCH_CONST", "MATCH_KEEP_EXPR", "BOX_ADDR")
MACRO_RE = re.compile(r"\b(" + "|".join(sorted(DELETE_MACROS + OTHER_MACROS,
                                              key=len, reverse=True)) + r")\s*\(")
# Kinds that aren't a macro.
ASM_EMPTY = "asm (empty template)"
ASM_INSN = "asm (instructions)"
SELF_INIT = "self-init `T x = x;`"
ALL_KINDS = DELETE_MACROS + OTHER_MACROS + (ASM_EMPTY, ASM_INSN, SELF_INIT)

# A comment above a removed site is removed with it only if it names one
# of these (a register, or the workaround).
WORKAROUND_WORDS = re.compile(
    r"\b(?:r\d{1,2}|ip|sb|sl|fp|pin|pins|pinned|pinning|MATCH_\w+|BOX_ADDR|register|hold|holds|"
    r"barrier|keep|keeps|reload|reloads|schedul\w*|cse|agbcc|agbcp|gcc|asm|constant-init|"
    r"workaround|nudge|clobber\w*)\b", re.I)


class Site:
    def __init__(self, sid, kind, start, end, line, group, alts, label):
        self.id = sid
        self.kind = kind
        self.start = start          # span in the source (for overlap checks)
        self.end = end
        self.line = line
        self.group = group          # enclosing top-level block (function)
        self.alts = alts            # [[(start, end, text)]], tried in order
        self.bundle_alts = []       # extra alternatives tried in bundles only
        self.label = label          # short source text for the report
        self.vars = set()           # variables the site is about
        self.deletes_line = False   # the edit deletes a whole statement
        self.pin_var = None


def prev_char(text, pos):
    i = pos - 1
    while i >= 0 and text[i].isspace():
        i -= 1
    return (text[i], i) if i >= 0 else ("", -1)


def next_char(text, pos):
    i = pos
    while i < len(text) and text[i].isspace():
        i += 1
    return (text[i], i) if i < len(text) else ("", len(text))


def statement_delete(text, start, end):
    """The edit deleting the statement text[start:end] `;`, or None if it
    isn't followed by `;`. An empty statement is left where the C needs
    one (an unbraced if/else/loop body, a label before `}`)."""
    c, semi = next_char(text, end)
    if c != ";":
        return None
    p, pi = prev_char(text, start)
    repl = ";"
    if p in ";{}":
        repl = ""
    elif p == ":" and text[pi - 1:pi] != ":":
        repl = ";" if next_char(text, semi + 1)[0] == "}" else ""
    return (start, semi + 1, repl)


def declaration_end(text, pos):
    """Offset of the `;` ending the declaration whose declarator ends at
    pos (an initializer may follow), or None for a second declarator."""
    depth = 0
    i = pos
    while i < len(text):
        c = text[i]
        if c in "([{":
            depth += 1
        elif c in ")]}":
            depth -= 1
            if depth < 0:
                return None
        elif c == "," and depth == 0:
            return None
        elif c == ";" and depth == 0:
            return i
        elif c in "\"'":
            j = i + 1
            while j < len(text) and text[j] != c:
                j += 2 if text[j] == "\\" else 1
            i = j
        i += 1
    return None


def args_of(text, open_paren):
    end = mi.balanced(text, open_paren)
    return [a.strip() for a in mi.split_top(text[open_paren + 1:end - 1], ",")], end


SIMPLE_EXPR = re.compile(r"[&*]?[A-Za-z_]\w*(?:(?:\.|->)\w+|\[[^\[\]]*\])*")


def group_of(depths, text, pos):
    """Offset of the `{` opening the top-level block containing pos."""
    i = pos
    while i > 0:
        i = text.rfind("{", 0, i)
        if i < 0:
            return -1
        if depths[i] == 1:
            return i
    return -1


def find_sites(raw, kinds):
    """The workaround sites of one source file (raw text)."""
    text = mi.strip_comments(raw)
    depths = mi.brace_depths(text)
    sites = []

    def add(kind, start, end, alts, label, **kw):
        if kind not in kinds or not alts:
            return None
        s = Site(len(sites), kind, start, end, mi.line_of(text, start),
                 group_of(depths, text, start), alts, label)
        for k, v in kw.items():
            setattr(s, k, v)
        sites.append(s)
        return s

    def ok_pos(pos):
        return depths[pos] > 0 and not mi.in_define(text, pos)

    for m in MACRO_RE.finditer(text):
        name = m.group(1)
        if not ok_pos(m.start()):
            continue
        args, end = args_of(text, m.end() - 1)
        label = re.sub(r"\s+", " ", raw[m.start():end])
        if name == "MATCH_HOLD_REG":
            if len(args) != 3:
                continue
            typ, var = args[0], args[1]
            decl = typ + ("" if typ.endswith("*") else " ") + var
            alts = [[(m.start(), end, decl)]]
            s = add(name, m.start(), end, alts, label, vars={var}, pin_var=var)
            if s:
                # The whole declaration, initializer included.
                stop = declaration_end(text, end)
                d = statement_delete(text, m.start(), stop) if stop is not None else None
                if d:
                    s.bundle_alts = [[d]]
        elif name in DELETE_MACROS:
            d = statement_delete(text, m.start(), end)
            if d is None:
                continue
            vs = set()
            for a in args:
                vs |= set(re.findall(r"[A-Za-z_]\w*", a))
            add(name, m.start(), end, [[d]], label, vars=vs, deletes_line=True)
        elif name == "MATCH_CONST":
            if len(args) != 2 or next_char(text, end)[0] != ";":
                continue
            add(name, m.start(), end, [[(m.start(), end, "%s = %s" % (args[0], args[1]))]],
                label, vars={args[0]})
        elif name == "MATCH_KEEP_EXPR":
            if len(args) != 2:
                continue
            e = args[1] if SIMPLE_EXPR.fullmatch(args[1]) else "(%s)" % args[1]
            add(name, m.start(), end, [[(m.start(), end, "((%s)%s)" % (args[0], e))]], label)
        elif name == "BOX_ADDR":
            if len(args) != 1:
                continue
            nc = next_char(text, end)[0]
            a = args[0] if nc in ",);" else "(%s)" % args[0]
            add(name, m.start(), end, [[(m.start(), end, a)]], label)

    for m in mi.ASM_START.finditer(text):
        if not ok_pos(m.start()):
            continue
        end = mi.balanced(text, m.end() - 1)
        k = mi.classify_asm(text, m, end, depths[m.start()])
        if k in (["pin"], ["asm_label"]):
            continue
        label = re.sub(r"\s+", " ", raw[m.start():end])
        if len(label) > 70:
            label = label[:67] + "..."
        body = text[m.end():end - 1]
        if k[0] in ("insn",):
            stmts = translate_insn(body)
            if stmts is None:
                continue
            alts = []
            for st in stmts:
                d = statement_delete(text, m.start(), end)
                if d is None:
                    break
                repl = st
                if d[2] == ";" or (st.count(";") > 1 and prev_char(text, m.start())[0] not in ";{}"):
                    repl = "{ %s }" % st
                alts.append([(d[0], d[1], repl)])
            add(ASM_INSN, m.start(), end, alts, label)
        elif k[0] in ("empty", "empty_volatile", "use", "use_volatile", "keep",
                      "keep_volatile", "hold", "hold_volatile", "const", "const_volatile",
                      "mem_barrier", "reg_clobber", "mem_ref", "empty_other"):
            d = statement_delete(text, m.start(), end)
            if d is None:
                continue
            ops = operands(body)
            if ops is None:
                continue
            outs, ins, _ = ops
            alts = []
            assign = None
            if len(outs) == 1 and outs[0][0] == "=r":
                tied = [e for c, e in ins if c == "0"]
                plain = [e for c, e in ins if c == "r"]
                if tied:
                    assign = tied[0]
                elif len(plain) == 1 and len(ins) == 1:
                    assign = plain[0]
            if assign is not None:
                alts.append([(d[0], d[1], "%s = %s;" % (outs[0][1], assign))])
            if not any(c[0] == "=" and c != "=m" for c, _ in outs) or assign is None:
                alts.append([d])
            add(ASM_EMPTY, m.start(), end, alts, label, deletes_line=(assign is None))

    for m in mi.SELF_INIT.finditer(text):
        if not ok_pos(m.start(1)):
            continue
        name_end = m.end(1)
        rest = re.match(r"\s*=\s*" + re.escape(m.group(1)) + r"\s*", text[name_end:])
        if not rest:
            continue
        add(SELF_INIT, m.start(1), name_end + rest.end(),
            [[(name_end, name_end + rest.end(), "")]],
            re.sub(r"\s+", " ", raw[m.start(1):name_end + rest.end()]) + "...")
    sites.sort(key=lambda s: s.start)
    for i, s in enumerate(sites):
        s.id = i
    return sites, text


def operands(body):
    """(outputs, inputs, clobbers) of an asm body: [(constraint, expr)]."""
    sections = mi.split_top(body, ":")
    ops = []
    for sec in sections[1:3]:
        lst = []
        for op in mi.split_top(sec, ","):
            op = op.strip()
            if not op:
                continue
            mm = re.fullmatch(r'"([^"]*)"\s*\((.*)\)', op, re.S)
            if not mm:
                return None
            lst.append((mm.group(1), mm.group(2).strip()))
        ops.append(lst)
    while len(ops) < 2:
        ops.append([])
    clob = re.findall(r'"([^"]*)"', sections[3]) if len(sections) > 3 else []
    return ops[0], ops[1], clob


# ---------------------------------------------------------------------------
# Instruction asm -> C

LOAD_T = {"ldr": "u32", "ldrh": "u16", "ldrb": "u8", "ldrsh": "s16", "ldrsb": "s8"}
STORE_T = {"str": "u32", "strh": "u16", "strb": "u8"}
SHIFTS = ("lsl", "lsr", "asr")


def fmt_int(n):
    if -10 < n < 10:
        return str(n)
    return ("-0x%x" % -n) if n < 0 else ("0x%x" % n)


def translate_insn(body):
    """C statement texts equivalent to an instruction asm (one per
    spelling worth trying), or None if it has no mechanical translation."""
    try:
        return _translate(body)
    except (ValueError, KeyError, IndexError):
        return None


def _translate(body):
    sections = mi.split_top(body, ":")
    tm = re.fullmatch(r'\s*((?:"(?:[^"\\]|\\.)*"\s*)+)', sections[0])
    if not tm:
        return None
    template = "".join(re.findall(r'"((?:[^"\\]|\\.)*)"', tm.group(1)))
    ops = operands(body)
    if ops is None:
        return None
    outs, ins, clob = ops
    if any(c not in ("cc", "memory") for c in clob):
        return None
    nslots = len(outs) + len(ins)
    initial = {}
    for i, (c, e) in enumerate(outs):
        if c == "+r":
            initial[i] = Val(e)
        elif c not in ("=r", "=&r"):
            return None
    for j, (c, e) in enumerate(ins):
        i = len(outs) + j
        if c.isdigit() and int(c) < len(outs):
            initial[int(c)] = Val(e)
            initial[i] = Val(e)
        elif c == "m":
            initial[i] = Val(e, mem=True)
        elif c in ("r", "i", "n", "I"):
            initial[i] = Val(e)
        else:
            return None
    lines = [ln.strip() for ln in re.split(r"\\n|\\t|;|\n", template)]
    lines = [ln for ln in lines if ln]
    if not lines:
        return None
    parsed = []
    for ln in lines:
        mm = re.fullmatch(r"([a-z]+)\s+(.*)", ln)
        if not mm:
            return None
        op = mm.group(1)
        if op.endswith("s") and op[:-1] in ARITH:
            op = op[:-1]
        if op not in ARITH and op not in LOAD_T and op not in STORE_T:
            return None
        parsed.append((op, [parse_arg(p, nslots) for p in re.split(r",(?![^\[]*\])", mm.group(2))]))
    # A register-offset address [a, b] is tried both ways round.
    swaps = (False, True) if any(a[0] == "addr" and len(a[1]) == 2 and a[1][1][0] == "slot"
                                 for _, args in parsed for a in args) else (False,)
    results = []
    for swap in swaps:
        val = dict(initial)
        written = set()
        stmts = []

        def read(a):
            if a[0] == "int":
                return Val(a[1])
            if a[0] == "slot":
                v = val.get(a[1])
                if v is None or v.mem:
                    raise ValueError
                return v
            raise ValueError

        def mem(t, a):
            if a[0] == "slot" and a[1] in val and val[a[1]].mem:
                return val[a[1]].text  # an "m" operand: the lvalue itself
            if a[0] != "addr" or not 1 <= len(a[1]) <= 2:
                raise ValueError
            base = read(a[1][0])
            off = read(a[1][1]) if len(a[1]) == 2 else Val(0)
            if off.const == 0:
                return "*(%s *)%s" % (t, base.paren())
            if off.const is not None:
                return "*(%s *)((u32)%s + %s)" % (t, base.paren(), off.paren())
            if swap:
                base, off = off, base
            return "*(%s *)((u32)%s + (u32)%s)" % (t, base.paren(), off.paren())

        for op, args in parsed:
            if op in STORE_T:
                if len(args) != 2:
                    return None
                stmts.append("%s = %s;" % (mem(STORE_T[op], args[1]), read(args[0]).text))
                continue
            d = args[0]
            if d[0] != "slot" or d[1] >= len(outs):
                return None
            if op in LOAD_T:
                if len(args) != 2:
                    return None
                v = Val(mem(LOAD_T[op], args[1]))
            elif op == "mov" and len(args) == 2:
                v = read(args[1])
            elif op == "neg" and len(args) == 2:
                s = read(args[1])
                v = Val(-s.const) if s.const is not None else Val("-" + s.paren())
            elif len(args) in (2, 3) and op in BINOPS + SHIFTS:
                x, y = (read(d), read(args[1])) if len(args) == 2 else \
                    (read(args[1]), read(args[2]))
                v = binop(op, x, y)
            else:
                return None
            val[d[1]] = v
            written.add(d[1])
        lvalues = [e for _, e in outs]
        for i, (c, e) in enumerate(outs):
            if i not in written or val[i].text == e:
                continue
            # An output computed from another output's old value would
            # need a temporary.
            if any(o != e and re.search(r"\b%s\b" % re.escape(o), val[i].text)
                   for o in lvalues[:i]):
                return None
            stmts.append("%s = %s;" % (e, val[i].text))
        s = " ".join(stmts) or ";"
        if s not in results:
            results.append(s)
    return results


ARITH = ("mov", "add", "sub", "neg", "lsl", "lsr", "asr", "and", "orr", "eor", "mul")
BINOPS = ("add", "sub", "and", "orr", "eor", "mul")
OPSYM = {"add": "+", "sub": "-", "and": "&", "orr": "|", "eor": "^", "mul": "*"}


class Val:
    """A value in a register while translating: a constant or a C
    expression; shl remembers an `lsl #k` for the extension peephole."""

    def __init__(self, v, mem=False, shl=None):
        self.const = v if isinstance(v, int) else None
        self.text = fmt_int(v) if isinstance(v, int) else v
        self.mem = mem
        self.shl = shl

    def paren(self):
        t = self.text
        if self.const is not None:
            return t if self.const >= 0 else "(%s)" % t
        return t if SIMPLE_EXPR.fullmatch(t) or re.fullmatch(r"\w+", t) else "(%s)" % t


def parse_arg(a, nslots):
    a = a.strip()
    mm = re.fullmatch(r"%(\d+)", a)
    if mm:
        if int(mm.group(1)) >= nslots:
            raise ValueError
        return ("slot", int(mm.group(1)))
    mm = re.fullmatch(r"#?(-?(?:0x[0-9a-fA-F]+|\d+))", a)
    if mm:
        return ("int", int(mm.group(1), 0))
    mm = re.fullmatch(r"\[(.*)\]", a)
    if mm:
        return ("addr", [parse_arg(p, nslots) for p in mm.group(1).split(",")])
    raise ValueError  # a hard register, a label, ...


def binop(op, x, y):
    if x.const is not None and y.const is not None:
        a, b = x.const, y.const
        r = {"add": a + b, "sub": a - b, "and": a & b, "orr": a | b, "eor": a ^ b,
             "mul": a * b, "lsl": (a << b) & 0xFFFFFFFF, "lsr": (a & 0xFFFFFFFF) >> b,
             "asr": a >> b}[op]
        return Val(r)
    if y.const == 0 and op in ("add", "sub", "orr", "eor", "lsl", "lsr", "asr"):
        return x
    if op in OPSYM:
        return Val("%s %s %s" % (x.paren(), OPSYM[op], y.paren()))
    if op == "lsl":
        return Val("%s << %s" % (x.paren(), y.paren()),
                   shl=(x, y.const) if y.const is not None else None)
    # lsl #k then lsr/asr #k: a zero or sign extension.
    if x.shl and y.const == x.shl[1]:
        cast = {(16, "lsr"): "u16", (24, "lsr"): "u8", (16, "asr"): "s16",
                (24, "asr"): "s8"}.get((y.const, op))
        if cast:
            return Val("(%s)%s" % (cast, x.shl[0].paren()))
    return Val("(%s)%s >> %s" % ("u32" if op == "lsr" else "s32", x.paren(), y.paren()))


# ---------------------------------------------------------------------------
# Applying edits

def apply_edits(raw, edits):
    """raw with the (start, end, text) edits applied, or None if two of
    them overlap."""
    edits = sorted(edits)
    for a, b in zip(edits, edits[1:]):
        if b[0] < a[1]:
            return None
    out = []
    pos = 0
    for s, e, t in edits:
        out.append(raw[pos:s])
        out.append(t)
        pos = e
    out.append(raw[pos:])
    return "".join(out)


def edits_for(sites, choice):
    """The edits of the chosen alternatives: choice maps site id -> (list
    name, index)."""
    edits = []
    for sid, (which, idx) in choice.items():
        s = sites[sid]
        edits.extend((s.alts if which == "alts" else s.bundle_alts)[idx])
    return edits


# ---------------------------------------------------------------------------
# Object comparison

SHT_SYMTAB, SHT_STRTAB, SHT_RELA, SHT_NOBITS, SHT_REL = 2, 3, 4, 8, 9


def cstr(data, off):
    end = data.index(b"\0", off)
    return data[off:end].decode("latin-1")


def elf_summary(path):
    """The comparable content of an ELF32 little-endian object (see the
    module docstring)."""
    with open(path, "rb") as f:
        data = f.read()
    if data[:4] != b"\x7fELF" or data[4] != 1 or data[5] != 1:
        raise ValueError("%s: not an ELF32 LE object" % path)
    shoff, = struct.unpack_from("<I", data, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", data, 0x2E)
    secs = []
    for i in range(shnum):
        secs.append(struct.unpack_from("<10I", data, shoff + i * shentsize))
    shstr = secs[shstrndx]
    names = [cstr(data, shstr[4] + s[0]) for s in secs]

    def body(i):
        s = secs[i]
        return b"" if s[1] == SHT_NOBITS else data[s[4]:s[4] + s[5]]

    symtabs = [i for i, s in enumerate(secs) if s[1] == SHT_SYMTAB]
    syms = []
    if symtabs:
        st = secs[symtabs[0]]
        strtab = secs[st[6]]
        for k in range(st[5] // 16):
            nm, value, size, info, other, shndx = struct.unpack_from(
                "<IIIBBH", data, st[4] + k * 16)
            name = cstr(data, strtab[4] + nm)
            typ, bind = info & 0xF, info >> 4
            if typ == 3 and shndx < len(names):  # STT_SECTION
                name = names[shndx]
            sec = names[shndx] if 0 < shndx < len(names) else {0: "UND", 0xFFF1: "ABS",
                                                                0xFFF2: "COM"}.get(shndx, shndx)
            syms.append((name, bind, typ, sec, value, size, other))
    sections = []
    relocs = []
    for i, s in enumerate(secs):
        if i == 0 or s[1] in (SHT_SYMTAB, SHT_STRTAB):
            continue
        if s[1] in (SHT_REL, SHT_RELA):
            ent = 8 if s[1] == SHT_REL else 12
            rl = []
            for k in range(s[5] // ent):
                off, info = struct.unpack_from("<II", data, s[4] + k * ent)
                add = struct.unpack_from("<i", data, s[4] + k * ent + 8)[0] if ent == 12 else 0
                sym = syms[info >> 8][:5] if (info >> 8) < len(syms) else info >> 8
                rl.append((off, info & 0xFF, sym, add))
            relocs.append((names[s[7]] if s[7] < len(names) else s[7], tuple(rl)))
            continue
        sections.append((names[i], s[1], s[2], s[8], s[9], s[5], body(i)))
    symset = sorted(x for x in syms[1:] if x[2] != 4)  # not STT_FILE
    return (tuple(sections), tuple(relocs), tuple(symset))


def digest(summary):
    return hashlib.sha1(repr(summary).encode()).hexdigest()


def first_difference(a, b):
    """A short description of where two summaries differ."""
    sa, sb = dict((x[0], x) for x in a[0]), dict((x[0], x) for x in b[0])
    for name in sorted(set(sa) | set(sb)):
        if sa.get(name) != sb.get(name):
            x, y = sa.get(name), sb.get(name)
            if x and y and x[5] != y[5]:
                return "%s size 0x%x -> 0x%x" % (name, x[5], y[5])
            if x and y:
                n = next(i for i in range(min(len(x[6]), len(y[6])) + 1)
                         if i == min(len(x[6]), len(y[6])) or x[6][i] != y[6][i])
                return "%s differs at +0x%x" % (name, n & ~1)
            return "section %s %s" % (name, "added" if y else "gone")
    if a[1] != b[1]:
        return "relocations differ"
    return "symbols differ"


# ---------------------------------------------------------------------------
# Building

PROBE_CAT = """#!/bin/sh
# Stands in for the compiler proper: copies the preprocessed source to the
# file after -o.
while [ $# -gt 0 ]; do
	if [ "$1" = -o ]; then cat > "$2"; exit 0; fi
	shift
done
cat > /dev/null
"""

# The override makefile (make -f Makefile -f this). With MATCH_PRUNE_PROBE
# set to an object, that object's rule only preprocesses: the compiler
# proper is replaced by PROBE_CAT and as/objcopy by `true`, so the .s
# holds the preprocessor output, from the same cpp command and flags.
OVERRIDE_MK = """
ifdef MATCH_PRUNE_PROBE
$(MATCH_PRUNE_PROBE): CC1 := sh $(MATCH_PRUNE_CAT)
$(MATCH_PRUNE_PROBE): CXX1 := sh $(MATCH_PRUNE_CAT)
$(MATCH_PRUNE_PROBE): AS := true
$(MATCH_PRUNE_PROBE): OBJCOPY := true
endif
"""

GENERATED = ("entities.h", "crates.h", "levels.h", "songs.h", "sfx.h")


class Builder:
    def __init__(self, tmp, jobs):
        self.tmp = tmp
        self.inc = os.path.join(tmp, "include")
        self.cat = os.path.join(tmp, "probe_cat.sh")
        self.mk = os.path.join(tmp, "match_prune.mk")
        with open(self.cat, "w") as f:
            f.write(PROBE_CAT)
        with open(self.mk, "w") as f:
            f.write(OVERRIDE_MK)
        self.workers = []
        self.free = collections.deque()
        self.cv = threading.Condition()
        for k in range(jobs):
            w = os.path.join(tmp, "w%d" % k)
            os.makedirs(w)
            self.workers.append(w)
            self.free.append(w)
        self.mirrored = set()
        self.lock = threading.Lock()
        self.builds = 0
        self.stopped = False
        r = subprocess.run(["make", "--no-print-directory", "-f", "Makefile", "-f", self.mk,
                            "OBJ_DIR=" + os.path.join(tmp, "gen"),
                            "GENERATED_INCLUDE_DIR=" + self.inc]
                           + [os.path.join(self.inc, "constants", h) for h in GENERATED],
                           cwd=ROOT, capture_output=True, text=True)
        if r.returncode:
            sys.exit("match_prune.py: generating the constants headers failed:\n"
                     + r.stdout[-2000:] + r.stderr[-2000:])

    def acquire(self):
        with self.cv:
            while not self.free:
                self.cv.wait()
            return self.free.popleft()

    def release(self, w):
        with self.cv:
            self.free.append(w)
            self.cv.notify()

    def mirror(self, w, rel):
        d = os.path.dirname(rel)
        key = (w, d)
        with self.lock:
            if key in self.mirrored:
                return
            self.mirrored.add(key)
        dst = os.path.join(w, d)
        os.makedirs(dst, exist_ok=True)
        src = os.path.join(ROOT, d)
        for fn in os.listdir(src):
            p = os.path.join(src, fn)
            if os.path.isfile(p):
                os.symlink(p, os.path.join(dst, fn))

    def make_args(self, w, obj, probe=False):
        args = ["make", "--no-print-directory", "-f", "Makefile", "-f", self.mk,
                "OBJ_DIR=" + os.path.join(w, "obj"),
                "C_SUBDIR=" + os.path.join(w, "src"),
                "C_BUILDDIR=" + os.path.join(w, "obj", "src"),
                "LIB_SUBDIR=" + os.path.join(w, "lib"),
                "LIB_BUILDDIR=" + os.path.join(w, "obj", "lib"),
                "GENERATED_INCLUDE_DIR=" + self.inc]
        if probe:
            args += ["MATCH_PRUNE_PROBE=" + obj, "MATCH_PRUNE_CAT=" + self.cat]
        return args + [obj]

    def run(self, rel, text, probe=False):
        """Build rel from text. Returns (summary, None) or (None, error);
        with probe, (preprocessed text, None)."""
        if self.stopped:
            raise KeyboardInterrupt
        w = self.acquire()
        try:
            self.mirror(w, rel)
            path = os.path.join(w, rel)
            if os.path.lexists(path):
                os.unlink(path)
            with open(path, "w", encoding="utf-8") as f:
                f.write(text)
            stem = os.path.splitext(rel)[0]
            obj = os.path.join(w, "obj", stem + ".o")
            asm = os.path.join(w, "obj", stem + ".s")
            for p in (obj, asm):
                if os.path.exists(p):
                    os.unlink(p)
            r = subprocess.run(self.make_args(w, obj, probe), cwd=ROOT,
                               capture_output=True, text=True)
            with self.lock:
                self.builds += 1
            os.unlink(path)
            os.symlink(os.path.join(ROOT, rel), path)
            if probe:
                if not os.path.exists(asm):
                    return None, (r.stdout + r.stderr)[-1500:]
                with open(asm, encoding="utf-8", errors="replace") as f:
                    return f.read(), None
            if r.returncode or not os.path.exists(obj):
                return None, compile_error(r.stdout + r.stderr).replace(w + os.sep, "")
            return elf_summary(obj), None
        finally:
            self.release(w)


def compile_error(log):
    for ln in log.splitlines():
        if re.search(r"(warning|error|undeclared|parse|syntax|invalid|incompatible):?", ln) \
                and "treated as errors" not in ln and not ln.startswith(("make", "arm-none")):
            return ln.strip()[:160]
    return (log.strip().splitlines() or ["?"])[-1][:160]


# ---------------------------------------------------------------------------
# The search

class FileResult:
    def __init__(self, rel):
        self.rel = rel
        self.sites = []
        self.removed = {}       # site id -> (which, idx, how)
        self.kept = {}          # site id -> reason
        self.inactive = []
        self.error = None
        self.final_text = None
        self.raw = None
        self.baseline = None


def prune_file(rel, builder, args, log):
    res = FileResult(rel)
    with open(os.path.join(ROOT, rel), encoding="utf-8") as f:
        raw = f.read()
    res.raw = raw
    sites, text = find_sites(raw, args.kinds)
    res.sites = sites
    if not sites:
        return res
    # Which sites does the ROM build compile? Preprocess with a marker
    # in front of each.
    marked = apply_edits(raw, [(s.start, s.start, " __match_prune_%d__ " % s.id)
                               for s in sites])
    pre, err = builder.run(rel, marked, probe=True)
    if pre is None:
        res.error = "preprocessing failed: " + (err or "?").strip().splitlines()[-1]
        return res
    seen = set(int(x) for x in re.findall(r"__match_prune_(\d+)__", pre))
    active = [s for s in sites if s.id in seen]
    res.inactive = [s for s in sites if s.id not in seen]
    if not active:
        return res
    base, err = builder.run(rel, raw)
    if base is None:
        res.error = "the unmodified file doesn't build: " + err
        return res
    res.baseline = base
    want = digest(base)
    ref = os.path.join(ROOT, "build", "crashbandicootxs", os.path.splitext(rel)[0] + ".o")
    if os.path.exists(ref):
        try:
            if digest(elf_summary(ref)) != want:
                log("warning: %s: build/'s object differs from a fresh compile (stale build?)"
                    % rel)
        except (ValueError, struct.error):
            pass

    state = {}     # site id -> (which, idx)
    how = {}
    reasons = {}
    pool = builder.pool

    def test(choice):
        edits = edits_for(sites, choice)
        t = apply_edits(raw, edits)
        if t is None:
            return False, "overlaps another site"
        summ, err = builder.run(rel, t)
        if summ is None:
            return False, "doesn't compile: " + err
        if digest(summ) == want:
            return True, None
        return False, "object differs: " + first_difference(base, summ)

    def try_units(units):
        """units: [(key, {sid: (which, idx)}...alternatives)]; returns the
        first matching alternative of each unit (tested in parallel)."""
        futs = {}
        for key, alts in units:
            futs[key] = pool.submit(lambda alts=alts: first_ok(alts))
        return {k: f.result() for k, f in futs.items()}

    def first_ok(alts):
        last = None
        for ch in alts:
            ok, why = test({**state, **ch})
            if ok:
                return ch, None
            last = why
        return None, last

    def applicable(ch):
        """ch adds a site, and changes no kept removal other than turning
        a plain-local pin into a deleted one."""
        if all(sid in state for sid in ch):
            return False
        return all(sid not in state or state[sid] == c
                   or (sites[sid].pin_var and c[0] == "bundle")
                   for sid, c in ch.items())

    def accept(found, label):
        """Add the units that matched alone (found: [(key, choice)]),
        checking they still match together."""
        found = [(k, ch) for k, ch in found if applicable(ch)]
        if not found:
            return False
        merged = {}
        for _, ch in found:
            merged.update(ch)
        disjoint = sum(len(ch) for _, ch in found) == len(merged)
        if len(found) > 1 and disjoint and test({**state, **merged})[0]:
            for k, ch in found:
                state.update(ch)
                for sid in ch:
                    how[sid] = label(k)
            return True
        added = False
        for i, (k, ch) in enumerate(found):
            if not applicable(ch):
                continue
            if (i == 0 and not added) or test({**state, **ch})[0]:
                state.update(ch)
                for sid in ch:
                    how[sid] = label(k)
                added = True
        return added

    def single_alts(s):
        return [{s.id: ("alts", i)} for i in range(len(s.alts))]

    while True:
        left = [s for s in active if s.id not in state]
        if not left:
            break
        out = try_units([(s.id, single_alts(s)) for s in left])
        for sid, (ch, why) in out.items():
            if ch is None:
                reasons[sid] = why
        if accept([(sid, ch) for sid, (ch, _) in sorted(out.items()) if ch],
                  lambda k: "alone"):
            continue
        if args.no_pairs:
            break
        left = [s for s in active if s.id not in state]
        units = []
        by_group = collections.defaultdict(list)
        for s in left:
            by_group[s.group].append(s)
        for grp in by_group.values():
            for i, a in enumerate(grp):
                for b in grp[i + 1:]:
                    alts = [{**x, **y} for x in single_alts(a) for y in single_alts(b)]
                    units.append((("pair", a.id, b.id), alts))
            # Bundles: a pin with the hold/use/keep/const sites of its
            # variable, the pin made a plain local or its declaration
            # deleted (also when it was made a plain local before).
            for p in [s for s in active if s.group == grp[0].group and s.pin_var]:
                mates = [s for s in grp if s is not p and p.pin_var in s.vars
                         and not s.pin_var]
                if not mates or (len(mates) < 2 and not p.bundle_alts):
                    continue
                base_ch = {s.id: ("alts", 0) for s in mates}
                alts = []
                if p.id not in state:
                    alts.append({**base_ch, p.id: ("alts", 0)})
                if p.bundle_alts:
                    alts.append({**base_ch, p.id: ("bundle", 0)})
                if alts:
                    units.append((("bundle", p.id) + tuple(s.id for s in mates), alts))
        if len(units) > args.max_pairs:
            log("note: %s: %d pairs/bundles, trying the first %d (--max-pairs)"
                % (rel, len(units), args.max_pairs))
            units = units[:args.max_pairs]
        if not units:
            break
        out = try_units(units)
        found = [(k, ch) for k, (ch, _) in sorted(out.items(), key=lambda x: str(x[0])) if ch]
        if not accept(found, lambda k: k):
            break
    for sid, k in how.items():
        if isinstance(k, tuple):
            others = [sites[x].line for x in k[1:] if x != sid]
            how[sid] = ("%s with line%s %s" % ("pair" if k[0] == "pair" else "bundle",
                                                "s" if len(others) > 1 else "",
                                                ", ".join(map(str, others))))
    res.removed = {sid: state[sid] + (how.get(sid, "alone"),) for sid in state}
    res.kept = {s.id: reasons.get(s.id, "?") for s in active if s.id not in state}
    if state:
        res.final_text = apply_edits(raw, edits_for(sites, state))
    return res


# ---------------------------------------------------------------------------
# Writing back

def write_form(res):
    """The cleaned-up text to write: emptied lines, the comments that
    explained them and an unused #include "match.h" dropped."""
    raw = res.raw
    sites = res.sites
    removed = set(res.removed)
    # Each edit keeps the newlines of the text it replaces, so line i of
    # the result is line i of the original.
    edits = [(s, e, t + "\n" * raw.count("\n", s, e))
             for s, e, t in edits_for(sites, {sid: v[:2] for sid, v in res.removed.items()})]
    text = apply_edits(raw, edits)
    old_lines = raw.split("\n")
    new_lines = text.split("\n")
    code_old = mi.strip_comments(raw).split("\n")
    code_new = mi.strip_comments(text).split("\n")
    # The lines each site spans.
    site_lines = collections.defaultdict(list)
    for s in sites:
        for ln in range(s.line - 1, s.line + raw.count("\n", s.start, s.end)):
            site_lines[ln].append(s)
    for s, e, _ in edits:
        first = raw.count("\n", 0, s)
        for ln in range(first, first + raw.count("\n", s, e) + 1):
            site_lines.setdefault(ln, [])

    def all_removed(ln):
        return all(s.id in removed for s in site_lines.get(ln, []))

    # Lines an edit left with no code (a trailing comment goes too).
    drop = set(ln for ln in site_lines if all_removed(ln) and code_old[ln].strip()
               and not code_new[ln].strip())

    # A comment block directly above dropped or rewritten site lines, if
    # it names a register or the workaround and what it introduces is
    # gone: either the lines up to the next blank line are all removed
    # sites, or the first lines after it are all dropped.
    for ln in sorted(drop | set(x for x in site_lines if all_removed(x) and site_lines[x])):
        j = ln - 1
        first = ln
        if j >= 0 and old_lines[j].strip() == "// clang-format off":
            j -= 1
        if j < 0 or j in drop or (j in site_lines and site_lines[j]):
            continue
        if code_old[j].strip() or not old_lines[j].strip():
            continue  # not a comment-only line
        top = j
        while top - 1 >= 0 and not code_old[top - 1].strip() and old_lines[top - 1].strip() \
                and not old_lines[top].lstrip().startswith(("/*", "//")):
            top -= 1
        if not old_lines[top].lstrip().startswith(("/*", "//")):
            continue
        subject = []
        k = first
        while k < len(old_lines) and code_old[k].strip():
            subject.append(k)
            k += 1
        run = []
        k = first
        while k in drop:
            run.append(k)
            k += 1
        comment = "\n".join(old_lines[top:j + 1])
        if not WORKAROUND_WORDS.search(comment) and not re.search(r"\bno code\b", comment, re.I):
            continue
        if (subject and all(site_lines.get(x) and all_removed(x) for x in subject)) or run:
            drop.update(range(top, j + 1))
    # A replaced asm's `// clang-format off`/`on` (tools/format.py's) go
    # when no asm is left between them.
    for i, l in enumerate(new_lines):
        if l.strip() != "// clang-format off":
            continue
        k = i + 1
        while k < len(new_lines) and new_lines[k].strip() != "// clang-format on":
            k += 1
        if k < len(new_lines) and any(new_lines[x] != old_lines[x] for x in range(i, k)) \
                and not any(mi.ASM_START.search(code_new[x]) for x in range(i, k)):
            drop.update((i, k))

    # No blank-line pile-ups where lines went: a run of dropped lines
    # between two blank lines (or after a `{`, or before a `}`) takes
    # one of them along.
    def blank(i):
        return 0 <= i < len(new_lines) and i not in drop and not new_lines[i].strip()

    for a in sorted(drop):
        if a - 1 in drop or a not in drop:
            continue
        b = a
        while b + 1 in drop:
            b += 1
        before, after = a - 1, b + 1
        if blank(after) and (blank(before) or (before >= 0
                                               and new_lines[before].rstrip().endswith("{"))):
            drop.add(after)
        elif blank(before) and after < len(new_lines) and new_lines[after].strip().startswith("}"):
            drop.add(before)
    text = "\n".join(l for i, l in enumerate(new_lines) if i not in drop)
    if not MACRO_RE.search(mi.strip_comments(text)):
        text = re.sub(r'^#include "match\.h"[^\n]*\n', "", text, flags=re.M)
    return text


def clang_formatted(rel, text):
    try:
        import format as fmt
        exe = os.environ.get("CLANG_FORMAT") or shutil.which("clang-format")
        if not exe:
            return None
        r = subprocess.run([exe, "--style=file", "--assume-filename=" + os.path.join(ROOT, rel)],
                           input=fmt.protect_asm(text), capture_output=True, text=True, cwd=ROOT)
        return r.stdout if r.returncode == 0 else None
    except Exception:
        return None


def stale_mentions(res, text):
    """Comment lines that still name a removed site's macro and variable."""
    out = []
    names = set()
    for sid in res.removed:
        s = res.sites[sid]
        if s.kind in ALL_KINDS[:len(DELETE_MACROS + OTHER_MACROS)]:
            names.add(s.kind)
    if not names:
        return out
    code = mi.strip_comments(text).split("\n")
    for i, ln in enumerate(text.split("\n")):
        if code[i].strip() == ln.strip():
            continue
        for n in names:
            if re.search(r"\b%s\b" % n, ln) and not re.search(r"\b%s\s*\(" % n, code[i]):
                out.append((i + 1, ln.strip()[:90]))
                break
    return out


# ---------------------------------------------------------------------------
# Reporting

def report(results, args):
    per_dir = collections.defaultdict(lambda: [0, 0, 0])
    per_kind = collections.defaultdict(lambda: [0, 0, 0])
    total = [0, 0, 0]
    for r in results:
        tried = len(r.removed) + len(r.kept)
        if r.error:
            print("%s: ERROR %s" % (r.rel, r.error))
        if not r.sites:
            continue
        if tried or args.verbose:
            print("%s: %d tried, %d removable, %d kept%s" % (
                r.rel, tried, len(r.removed), len(r.kept),
                ", %d not compiled by the ROM build" % len(r.inactive) if r.inactive else ""))
        for s in r.sites:
            if s.id in r.removed:
                print("    removable %5d  %-44s %s" % (s.line, s.label[:44], r.removed[s.id][2]))
            elif s.id in r.kept and args.verbose:
                print("    kept      %5d  %-44s %s" % (s.line, s.label[:44], r.kept[s.id]))
        d = os.path.dirname(r.rel)
        for s in r.sites:
            if s.id in r.removed:
                col = 1
            elif s.id in r.kept:
                col = 2
            else:
                continue
            for acc in (per_dir[d], per_kind[s.kind], total):
                acc[0] += 1
                acc[col] += 1
    print()
    print("%-28s %6s %9s %6s" % ("directory", "tried", "removable", "kept"))
    for d in sorted(per_dir):
        print("%-28s %6d %9d %6d" % ((d,) + tuple(per_dir[d])))
    print("%-28s %6d %9d %6d" % (("total",) + tuple(total)))
    print()
    print("%-28s %6s %9s %6s" % ("kind", "tried", "removable", "kept"))
    for k in ALL_KINDS:
        if k in per_kind:
            print("%-28s %6d %9d %6d" % ((k,) + tuple(per_kind[k])))


def as_json(results):
    out = []
    for r in results:
        for s in r.sites:
            st = "removable" if s.id in r.removed else "kept" if s.id in r.kept else "inactive"
            out.append({"file": r.rel, "line": s.line, "kind": s.kind, "site": s.label,
                        "status": st,
                        "detail": r.removed[s.id][2] if s.id in r.removed
                        else r.kept.get(s.id, "")})
    return out


def collect(paths):
    files = []
    for p in paths:
        full = os.path.abspath(p)
        if os.path.isfile(full):
            files.append(full)
        elif os.path.isdir(full):
            for dp, dns, fns in os.walk(full):
                dns.sort()
                for fn in sorted(fns):
                    if fn.endswith((".c", ".cpp")):
                        files.append(os.path.join(dp, fn))
        else:
            sys.exit("match_prune.py: no such file or directory: %s" % p)
    rels = []
    for f in files:
        rel = os.path.relpath(f, ROOT)
        if rel.startswith(".."):
            sys.exit("match_prune.py: %s is outside the repository" % f)
        if not rel.endswith((".c", ".cpp")):
            print("skipping %s: only .c/.cpp translation units are tried" % rel, file=sys.stderr)
            continue
        if not (re.fullmatch(r"src/[^/]+/[^/]+\.(?:c|cpp)", rel)
                or re.fullmatch(r"lib/[^/]+/(?:src|data)/[^/]+\.c", rel)):
            print("skipping %s: not an object the Makefile builds" % rel, file=sys.stderr)
            continue
        rels.append(rel)
    return sorted(set(rels))


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0],
                                 formatter_class=argparse.RawDescriptionHelpFormatter,
                                 epilog="See the module docstring (head tools/match_prune.py) "
                                        "for how objects are built and compared.")
    ap.add_argument("paths", nargs="+", help=".c/.cpp files or directories")
    ap.add_argument("--write", action="store_true", help="apply the removals to the sources")
    ap.add_argument("--list", action="store_true", help="only list the sites, build nothing")
    ap.add_argument("-v", "--verbose", action="store_true", help="also list the kept sites")
    ap.add_argument("-j", "--jobs", type=int, default=os.cpu_count() or 4)
    ap.add_argument("--kinds", help="comma-separated site kinds (default all): "
                    + ", ".join(k for k in ALL_KINDS if k.startswith(("MATCH", "BOX")))
                    + ", asm (empty-template asm), insn (instruction asm), self_init")
    ap.add_argument("--no-pairs", action="store_true", help="only try sites one at a time")
    ap.add_argument("--max-pairs", type=int, default=4000,
                    help="pairs and bundles tried per file and round (default 4000)")
    ap.add_argument("--json", metavar="FILE", help="also write the per-site results as JSON")
    ap.add_argument("--keep-temp", metavar="DIR", help="work in DIR and keep it")
    args = ap.parse_args()
    if args.kinds:
        want = set(k.strip() for k in args.kinds.split(","))
        alias = {"asm": ASM_EMPTY, "insn": ASM_INSN, "self_init": SELF_INIT}
        args.kinds = set(alias.get(k, k) for k in want)
        bad = args.kinds - set(ALL_KINDS)
        if bad:
            ap.error("unknown kind(s) %s" % ", ".join(sorted(bad)))
    else:
        args.kinds = set(ALL_KINDS)

    rels = collect(args.paths)
    if args.list:
        n = 0
        for rel in rels:
            with open(os.path.join(ROOT, rel), encoding="utf-8") as f:
                sites, _ = find_sites(f.read(), args.kinds)
            for s in sites:
                print("%s:%d: %-22s %s" % (rel, s.line, s.kind, s.label))
                n += 1
        print("%d sites" % n, file=sys.stderr)
        return 0

    tmp = args.keep_temp or tempfile.mkdtemp(prefix="match_prune_")
    if args.keep_temp:
        if os.path.exists(tmp) and os.listdir(tmp):
            sys.exit("match_prune.py: --keep-temp %s isn't empty" % tmp)
        os.makedirs(tmp, exist_ok=True)
    lock = threading.Lock()

    def log(msg):
        with lock:
            print(msg, file=sys.stderr)

    results = []
    builder = drivers = None
    try:
        builder = Builder(tmp, max(1, args.jobs))
        # Builds run on `jobs` worker directories; the per-file drivers and
        # their parallel trials only wait on them.
        builder.pool = concurrent.futures.ThreadPoolExecutor(max_workers=max(4, args.jobs * 4))
        drivers = concurrent.futures.ThreadPoolExecutor(max_workers=max(1, args.jobs))
        futs = {rel: drivers.submit(prune_file, rel, builder, args, log) for rel in rels}
        done = 0
        for rel in rels:
            try:
                r = futs[rel].result()
            except Exception as e:  # report and go on
                r = FileResult(rel)
                r.error = "%s: %s" % (type(e).__name__, e)
            results.append(r)
            done += 1
            if r.sites and sys.stderr.isatty():
                log("[%d/%d] %s: %d removable of %d" % (done, len(rels), rel, len(r.removed),
                                                       len(r.removed) + len(r.kept)))
        writes = []
        if args.write:
            # The text to write, checked once more: the cleaned-up and
            # formatted form if it still matches, else the plain edit.
            def final(r):
                pretty = write_form(r)
                for cand in [c for c in (clang_formatted(r.rel, pretty), pretty, r.final_text) if c]:
                    summ, _ = builder.run(r.rel, cand)
                    if summ is not None and digest(summ) == digest(r.baseline):
                        return cand
                return None
            todo = [r for r in results if r.final_text]
            for r, text in zip(todo, builder.pool.map(final, todo)):
                if text is None:
                    log("%s: no written form matches; not written" % r.rel)
                else:
                    writes.append((r, text))
    except KeyboardInterrupt:
        if builder:
            builder.stopped = True
        for ex in (builder and getattr(builder, "pool", None), drivers):
            if ex:
                ex.shutdown(wait=True, cancel_futures=True)
        print("\ninterrupted: no source was modified", file=sys.stderr)
        return 130
    finally:
        if builder and getattr(builder, "pool", None):
            builder.pool.shutdown(wait=False)
        if drivers:
            drivers.shutdown(wait=False)
        if not args.keep_temp:
            shutil.rmtree(tmp, ignore_errors=True)

    report(results, args)
    print("\n%d builds" % builder.builds, file=sys.stderr)
    if args.json:
        with open(args.json, "w") as f:
            json.dump(as_json(results), f, indent=1)
    if args.write:
        old = signal.signal(signal.SIGINT, signal.SIG_IGN)
        try:
            for r, text in list(writes):
                path = os.path.join(ROOT, r.rel)
                with open(path, encoding="utf-8") as f:
                    if f.read() != r.raw:
                        print("%s changed while the tool ran; not written" % r.rel)
                        writes.remove((r, text))
                        continue
                tmpf = path + ".match_prune.tmp"
                with open(tmpf, "w", encoding="utf-8") as f:
                    f.write(text)
                shutil.copymode(path, tmpf)
                os.replace(tmpf, path)
        finally:
            signal.signal(signal.SIGINT, old)
        print("\n%d file(s) written" % len(writes))
        for r, text in writes:
            for ln, s in stale_mentions(r, text):
                print("check comment: %s:%d: %s" % (r.rel, ln, s))
    return 0


if __name__ == "__main__":
    sys.exit(main())
