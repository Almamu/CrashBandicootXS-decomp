#!/usr/bin/env python3
"""`tools/magic_numbers.py --bits`: flag bits, masks and raw state values
still written as numbers, grouped by field (#843).

Scans the game code (engine/src/ without engine/src/.../data/, and engine/include/) for:

  shr      single-bit tests `(x >> n) & 1`;
  lsl31    single-bit extractions `((u32)x << m) >> 31` (bit 31 - m of
           the promoted value);
  maskbit  `x & (1 << n)`;
  mask     `x & K`, `x &= K`, `x &= ~K`, `x |= K`, `x | K` and `x ^ K`
           with an integer literal K (each set bit of K is listed;
           low masks like 0xFF are kept apart as `lowmask`);
  value    raw integers compared with (`==`, `!=`, `<`, ...), assigned to
           or switched on (`case N:`) a field or variable whose name says
           it holds a state: state, mode, type, kind, status, phase, step,
           flags (and names ending in one of them, `exitStatus`).

Each use is put under its field: the class that declares the member and
the member path from there (`Entity::f.flags`, `ActionCtrl::state`),
found with magic_numbers.py's model of the sources (a local, a
parameter, a member of the method's class, a global, a cast, then the
`->`/`.` chain). `f.bytes.flags` is `f.flags` (union EntityFlags). A
plain local or parameter is `local <name>` of its function; a field
whose class isn't known is `?::<path>`.

Usage:
  tools/magic_numbers.py --bits                 per field (most uses first):
                                                each bit/value and its sites
  tools/magic_numbers.py --bits --report [OUT]  the same as Markdown
  tools/magic_numbers.py --bits --json          every use as JSON
  tools/magic_numbers.py --bits --path P        only files under P
  tools/magic_numbers.py --bits --field RE      only fields matching RE
"""

import collections
import json
import os
import re
import sys

from buildtools import magic_numbers as mn

ROOT = mn.ROOT

STATE_NAME = re.compile(r"(?i)(?:^|[a-z_])(state|mode|type|kind|status|phase|step|flags\d*)$")

Use = collections.namedtuple("Use", "field kind bit value op file line func source")

# A variable or a member chain: `x`, `p->f.flags`, `a[i].state`, `Get()->mode`.
CHAIN = re.compile(r"^[A-Za-z_]\w*(?:\([^()]*\))?(?:(?:->|\.)[A-Za-z_]\w*(?:\([^()]*\))?|\[[^\]]*\])*$")

# A cast, `(u32)`, `(struct x *)`: at the start of a string, and at its end.
CAST = re.compile(r"\(\s*(?:const\s+)?(?:struct\s+|class\s+)?\w+\s*\**\s*\)\s*")
CAST_END = re.compile(r"\(\s*(?:const\s+)?(?:struct\s+|class\s+)?\w+\s*\**\s*\)$")

LITERAL = r"(0[xX][0-9a-fA-F]+|\d+)[uUlL]*"

# `x & K`, `x &= K`, `x &= ~K`, `x |= K`, `x | K`, `x ^ K` with a literal K.
MASK_OP = re.compile(r"(?<![&|<>=!^])(&=|\|=|\^=|&|\||\^)(?![&|=])\s*(~?)\s*\(?\s*" + LITERAL + r"(?![\w.])")
# A comparison with or an assignment of a literal.
VALUE_OP = re.compile(
    r"(==|!=|<=|>=|(?<![<>-])<(?![<=])|(?<![-<>])>(?![>=])|(?<![=!<>+\-*/%&|^])=(?!=))"
    r"\s*(-?)\s*" + LITERAL + r"(?![\w.])"
)


def expr_start(text, end):
    """The start of the postfix expression (with leading casts and a `*`
    or `!`) that ends at `end`."""
    i = end
    while i > 0 and text[i - 1].isspace():
        i -= 1
    while i > 0:
        c = text[i - 1]
        if c in ")]":
            i = mn.match_open(text, i - 1)
            if c == ")" and i > 0 and (text[i - 1].isalnum() or text[i - 1] == "_"):
                continue  # a call: its name is next
        elif c.isalnum() or c == "_":
            while i > 0 and (text[i - 1].isalnum() or text[i - 1] == "_"):
                i -= 1
        else:
            break
        j = i
        while j > 0 and text[j - 1].isspace():
            j -= 1
        if text[j - 2 : j] == "->":
            i = j - 2
        elif text[j - 1 : j] == "." and text[j - 2 : j - 1] != ".":
            i = j - 1
        elif text[j - 1 : j] == ")" and CAST_END.search(text[:j]):
            i = mn.match_open(text, j - 1)  # a cast
            break
        else:
            break
        while i > 0 and text[i - 1].isspace():
            i -= 1
    return i


def strip_expr(s):
    """`(u32)(x->f.flags)` -> `x->f.flags`."""
    s = s.strip()
    while True:
        m = CAST.match(s)
        if m:
            s = s[m.end() :]
            continue
        if s.startswith("(") and mn.match_close(s, 0) == len(s) - 1:
            s = s[1:-1].strip()
            continue
        if s and s[0] in "*!":
            s = s[1:].strip()
            continue
        return s


UNIONS = set()


def load_unions(files):
    for path in files:
        _, text = mn.read_blanked(path)
        UNIONS.update(re.findall(r"\bunion\s+(\w+)\s*\{", text))


class BitScan(mn.FileScan):
    def __init__(self, path, rel, model):
        super().__init__(path, rel, model, {}, [])
        self.uses = []
        self.static = None

    def field_of(self, start, end):
        """(field key, short expression) of the expression text[start:end];
        sets self.static to the receiver's class when it isn't the owner."""
        self.static = None
        expr = strip_expr(self.text[start:end])
        if not expr:
            return None, expr
        flat = re.sub(r"\s+", "", expr)
        if not CHAIN.match(flat):
            return None, flat  # not a variable or a field (`x >> 6`, `a + b`)
        off = start + self.text[start:end].find(expr[:1]) if expr else start
        # the last top-level `->` of the expression
        depth, last = 0, -1
        s = self.text[off:end]
        for i, c in enumerate(s):
            if c in "([":
                depth += 1
            elif c in ")]":
                depth -= 1
            elif depth == 0 and s[i : i + 2] == "->":
                last = i
        f = self.func_at(off)
        if last >= 0:
            t, _ = self.receiver(off + last)
            path = re.sub(r"\s+", "", s[last + 2 :])
        else:
            m = re.match(r"\s*(\w+)", s)
            base = m.group(1)
            path = re.sub(r"\s+", "", s[m.end() :]).lstrip(".")
            if path.startswith("("):
                return f"{base}()", flat  # a call or a macro (`INPUT_PRESSED(in)`)
            if f and f.name and not self.is_local(f, base, off) and self.model.field_type(f.name, base):
                t, path = f.name, re.sub(r"\s+", "", s)
            elif not path:
                if base in self.model.globals or not f:
                    return f"global {base}", flat
                return f"local {base} ({f.fname or '?'}, {self.rel})", flat
            else:
                t = self.name_type(base, off)
        path = re.sub(r"\[[^\]]*\]", "[]", path)
        path = re.sub(r"\(\)", "()", path)
        first = re.match(r"(\w+)", path)
        if not first:
            return None, flat
        owner, static = None, t
        if t:
            # Walk the members: a member that is a class (not a union or an
            # anonymous struct) starts the key again (`progress.flags` of a
            # LevelState is GameProgress::flags).
            names = re.findall(r"\w+", path.replace("()", ""))
            ft = self.model.field_type(t, names[0])
            owner = ft[0] if ft else t
            cur = ft and ft[1]
            for k, name in enumerate(names[1:], 1):
                if not cur or "::<anon" in cur or cur in UNIONS or not self.model.known(cur):
                    if cur and self.model.known(cur):
                        nft = self.model.field_type(cur, name)
                        cur = nft and nft[1]
                    else:
                        cur = None
                    continue
                nft = self.model.field_type(cur, name)
                if not nft:
                    break
                owner, static = nft[0], cur
                path = ".".join(names[k:])
                cur = nft[1]
        if owner and "::<anon" in owner:
            owner = owner.split("::<anon")[0]
        if path.startswith("f.bytes.") or path.startswith("f.b."):
            path = "f." + path.split(".", 2)[2] if path.startswith("f.bytes.") else path
        self.static = static if static and static != owner else None
        return f"{owner or '?'}::{path}", flat

    def add_use(self, kind, start, end, at, bit=None, value=None, op=""):
        field, expr = self.field_of(start, end)
        if not field:
            return
        if kind == "value" and self.static:
            field += f" [{self.static}]"  # a base's state: each subclass has its own values
        self.record(field, kind, bit, value, op, at)

    def record(self, field, kind, bit, value, op, at):
        f = self.func_at(at)
        fname = f and ((f.name + "::" if f.name and f.fname != f.name else "") + (f.fname or "?"))
        line = mn.line_of(self.text, at)
        self.uses.append(Use(field, kind, bit, value, op, self.rel, line, fname, self.src_lines[line - 1].strip()))

    def scan_bits(self):
        text = self.text
        # (x >> n) & 1
        for m in re.finditer(r">>\s*(\d+)\s*\)\s*&\s*1(?![\w.])", text):
            close = m.end() - len(m.group(0)) + m.group(0).index(")")
            o = mn.match_open(text, close)
            self.add_use("shr", o + 1, m.start(), m.start(), bit=int(m.group(1)), op="test")
        # ((u32)x << m) >> 31, (u32)(x << m) >> 31
        for m in re.finditer(r"<<\s*(\d+)\s*\)\s*>>\s*31(?!\d)", text):
            close = m.start() + m.group(0).index(")")
            o = mn.match_open(text, close)
            self.add_use("lsl31", o + 1, m.start(), m.start(), bit=31 - int(m.group(1)), op="test")
        # x & (1 << n)
        for m in re.finditer(r"(?<![&|])&\s*\(\s*1[uU]?\s*<<\s*(\d+)\s*\)", text):
            s = expr_start(text, m.start())
            self.add_use("maskbit", s, m.start(), m.start(), bit=int(m.group(1)), op="test")
        # masks with a literal: x & K, x &= K, x &= ~K, x |= K, x | K, x ^ K
        for m in MASK_OP.finditer(text):
            op, neg, lit = m.group(1), m.group(2), m.group(3)
            prev = text[: m.start()].rstrip()
            if not prev or prev[-1] in "(,=?:" or prev.endswith("return"):
                continue  # unary `&`, or a literal first
            v = int(lit, 16) if lit[:2].lower() == "0x" else int(lit)
            s = expr_start(text, m.start())
            if s >= m.start():
                continue
            kind = "lowmask" if v and (v & (v + 1)) == 0 and v > 1 else "mask"
            if neg:
                v = v & 0xFFFFFFFF
            bits = [b for b in range(32) if (v >> b) & 1]
            how = {"&": "test", "&=": "clear" if neg else "keep", "|=": "set", "|": "or", "^": "xor", "^=": "toggle"}
            if op == "&" and neg:
                how["&"] = "without"
            if (kind == "lowmask" and op in ("&", "&=")) or len(bits) > 4:
                # a value mask (`& 0xFF`, `& 0xFFFFFF00`), not flags
                kind = "lowmask" if kind == "lowmask" and op in ("&", "&=") else "widemask"
                self.add_use(kind, s, m.start(), m.start(), value=v, op=how[op])
                continue
            for b in bits if v else [None]:
                self.add_use(kind, s, m.start(), m.start(), bit=b, value=v, op=how[op])

    def scan_values(self):
        text = self.text
        for m in VALUE_OP.finditer(text):
            s = expr_start(text, m.start())
            if s >= m.start():
                continue
            expr = strip_expr(text[s : m.start()])
            name = re.search(r"(\w+)\s*(?:\[[^\]]*\]\s*)*$", expr)
            if not name or not STATE_NAME.search(name.group(1)):
                continue
            if m.group(1) == "=" and not re.match(r"\s*[;,)]", text[m.end() :]):
                continue  # `x = 3 + y`
            lit = m.group(3)
            v = int(lit, 16) if lit[:2].lower() == "0x" else int(lit)
            if m.group(2):
                v = -v
            op = "assign" if m.group(1) == "=" else "compare"
            self.add_use("value", s, m.start(), m.start(), value=v, op=op)
        for m in re.finditer(r"\bswitch\s*\(", text):
            o = m.end() - 1
            c = mn.match_close(text, o)
            expr = strip_expr(text[o + 1 : c])
            name = re.search(r"(\w+)\s*(?:\[[^\]]*\]\s*)*$", expr)
            if not name or not STATE_NAME.search(name.group(1)):
                continue
            b = text.find("{", c)
            if b < 0:
                continue
            e = mn.match_close(text, b)
            depth = 0
            body = text[b:e]
            for cm in re.finditer(r"[{}]|\bcase\s+(-?)\s*" + LITERAL + r"\s*:", body):
                if cm.group(0) == "{":
                    depth += 1
                elif cm.group(0) == "}":
                    depth -= 1
                elif depth == 1:
                    lit = cm.group(2)
                    v = int(lit, 16) if lit[:2].lower() == "0x" else int(lit)
                    if cm.group(1):
                        v = -v
                    field, _ = self.field_of(o + 1, c)
                    if field and self.static:
                        field += f" [{self.static}]"
                    if field:
                        self.record(field, "value", None, v, "case", b + cm.start())


def source_files():
    """The game's sources: engine/src/ without engine/src/.../data/, and engine/include/."""
    out = []
    for path in mn.source_files():
        rel = os.path.relpath(path, ROOT)
        if "data" in rel.split(os.sep)[1:-1]:
            continue
        if not rel.endswith(mn.CXX_EXTS + mn.C_EXTS):
            continue
        out.append((path, rel))
    return out


def scan(paths=None):
    files = mn.source_files()
    model = mn.Model()
    model.load(files)
    load_unions(files)
    uses = []
    for path, rel in source_files():
        if paths and not any(rel == p or rel.startswith(p.rstrip("/") + "/") for p in paths):
            continue
        s = BitScan(path, rel, model)
        if not s.is_cxx:
            continue
        s.scan_bits()
        s.scan_values()
        uses += s.uses
    seen, out = set(), []
    for u in uses:
        k = (u.file, u.line, u.field, u.kind, u.bit, u.value, u.op)
        if k not in seen:
            seen.add(k)
            out.append(u)
    return out


def grouped(uses):
    fields = collections.defaultdict(list)
    for u in uses:
        fields[u.field].append(u)
    return sorted(fields.items(), key=lambda kv: (-flag_uses(kv[1]), -len(kv[1]), kv[0]))


def flag_uses(us):
    """The uses that are bits or state values (not `& 0xFF` value masks)."""
    return sum(1 for u in us if u.kind not in ("lowmask", "widemask"))


def label(u):
    if u.kind == "value":
        return f"value {u.value:#x}" if abs(u.value) > 9 else f"value {u.value}"
    if u.kind in ("lowmask", "widemask"):
        return f"{u.kind} {u.value:#x}"
    return f"bit {u.bit}"


def keyorder(lbl):
    kind, _, v = lbl.partition(" ")
    try:
        n = int(v, 0)
    except ValueError:
        n = 0
    return ({"bit": 0, "value": 1, "lowmask": 2, "widemask": 3}.get(kind, 4), n)


def text_listing(uses, out):
    for field, us in grouped(uses):
        out.write(f"{field}: {len(us)} uses\n")
        by = collections.defaultdict(list)
        for u in us:
            by[label(u)].append(u)
        for lbl in sorted(by, key=keyorder):
            ops = collections.Counter(f"{u.kind}/{u.op}" for u in by[lbl])
            out.write(f"  {lbl}: " + ", ".join(f"{k} {c}" for k, c in sorted(ops.items())) + "\n")
            for u in by[lbl]:
                out.write(f"    {u.file}:{u.line} {u.func or ''}: {u.source}\n")


def report(uses, out, samples=4):
    fields = grouped(uses)
    kinds = collections.Counter(u.kind for u in uses)
    out.write("# Flag bits, masks and raw state values (#843)\n\n")
    counts = ", ".join(f"{k} {c}" for k, c in kinds.most_common())
    out.write(f"{len(uses)} uses in {len(fields)} fields: {counts}\n\n")
    out.write("| field | uses | bits / values (uses) |\n|---|---:|---|\n")
    for field, us in fields:
        c = collections.Counter(label(u) for u in us)
        cells = ", ".join(f"{k} ({c[k]})" for k in sorted(c, key=keyorder))
        out.write(f"| `{field}` | {len(us)} | {cells} |\n")
    out.write("\n")
    for field, us in fields:
        out.write(f"## `{field}` ({len(us)})\n\n")
        by = collections.defaultdict(list)
        for u in us:
            by[label(u)].append(u)
        for lbl in sorted(by, key=keyorder):
            ops = collections.Counter(u.op for u in by[lbl])
            funcs = sorted({u.func or "?" for u in by[lbl]})
            how = ", ".join(f"{k} {n}" for k, n in sorted(ops.items()))
            more = " ..." if len(funcs) > samples else ""
            out.write(f"- **{lbl}** ({how}): {', '.join(funcs[:samples])}{more}\n")
        out.write("\n")


def run(args):
    uses = scan(args.path)
    if getattr(args, "field", None):
        pat = re.compile(args.field)
        uses = [u for u in uses if pat.search(u.field)]
    if args.json:
        json.dump([u._asdict() for u in uses], sys.stdout, indent=1)
        sys.stdout.write("\n")
        return 0
    if args.report:
        if args.report == "-":
            report(uses, sys.stdout, args.samples)
        else:
            with open(args.report, "w") as f:
                report(uses, f, args.samples)
        return 0
    text_listing(uses, sys.stdout)
    c = collections.Counter(u.kind for u in uses)
    counts = ", ".join(f"{k} {n}" for k, n in c.most_common())
    print(f"{len(uses)} uses in {len(grouped(uses))} fields: {counts}", file=sys.stderr)
    return 0
