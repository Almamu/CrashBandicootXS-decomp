#!/usr/bin/env python3
"""`tools/magic_numbers.py --casts`: the casts in the game code, grouped
by the thing being cast (#855).

Scans the game code (engine/src/ without engine/src/.../data/, and engine/include/; the C++ and
the C that is left) for C-style casts `(T)x` and puts each one under the
thing it converts: a field (`Sprite::x`, the class that declares the
member and the member path from there), a local or a parameter (`local x
(Func, file)`), a global, a function's result (`GetFoo()`,
`Class::Method()`), the address of one of those (`&Sprite::x`) or, for
an expression that is none of these, `<expr>`. The model of the sources
is magic_numbers.py's (and magic_bits.py's field resolution): a local, a
parameter, a member of the method's class or its bases, a global, a
cast, then the `->`/`.` chain.

For each thing it gives the cast types, its declared type when known,
how many of its uses are cast, and the kind of each cast:

  scalar    to an integer type from an integer (narrowing, widening or
            a signedness change: the `sub` column);
  ptrint    a pointer to an integer (`(u32)p`, `(u32)p + n`);
  pointer   to a pointer type from a pointer of another type, an integer
            or an unknown type;
  derived   to a class pointer from a pointer to a class it derives
            from or that derives from it (Sprite * -> Player *);
  byteoff   `(T *)((u8 *)p + N)`, `(T *)((u32)p + N)`: a field or a
            struct view at a byte offset;
  same      to the type the thing already has (likely not needed).

A thing whose every read is cast the same way is a candidate to retype
(the field, the local, the parameter or the function's return type).
The list is ranked by cast uses x consistency (the share of the reads
that are cast to the most common type), so `Foo::bar cast to u16 at
14/14 reads` comes first.

It doesn't run the preprocessor: macro bodies aren't scanned, and a cast
in a macro shows at its uses only if it is written there.

Usage:
  tools/magic_numbers.py --casts                  per thing, ranked: casts,
                                                  uses and sites
  tools/magic_numbers.py --casts --report [OUT]   the same as Markdown
  tools/magic_numbers.py --casts --json           every cast as JSON
  tools/magic_numbers.py --casts --path P         only files under P
  tools/magic_numbers.py --casts --field RE       only things matching RE
  tools/magic_numbers.py --casts --kind K         only casts of kind K
                                                  (repeatable)
"""

import collections
import json
import re
import sys

import magic_bits as mb
import magic_numbers as mn

KINDS = ("scalar", "ptrint", "pointer", "derived", "byteoff", "same")

# Integer types: (bits, signed).
SCALARS = {
    "u8": (8, False),
    "s8": (8, True),
    "u16": (16, False),
    "s16": (16, True),
    "u32": (32, False),
    "s32": (32, True),
    "u64": (64, False),
    "s64": (64, True),
    "vu8": (8, False),
    "vs8": (8, True),
    "vu16": (16, False),
    "vs16": (16, True),
    "vu32": (32, False),
    "vs32": (32, True),
    "bool8": (8, False),
    "bool32": (32, False),
    "bool": (8, False),
    "char": (8, True),
    "short": (16, True),
    "int": (32, True),
    "long": (32, True),
    "unsigned": (32, False),
    "signed": (32, True),
    "size_t": (32, False),
    "uintptr_t": (32, False),
    "intptr_t": (32, True),
}
BYTE_PTRS = ("u8", "s8", "char", "vu8")

# A cast's parenthesised type: `(u16)`, `(const struct foo *)`, `(unsigned int)`.
CAST_RE = re.compile(
    r"\(\s*(?:const\s+|volatile\s+)*(?:(struct|class|union|enum)\s+)?"
    r"((?:unsigned|signed)\s+(?:char|short|int|long)|[A-Za-z_]\w*)"
    r"\s*(?:const\s*)?(\**)\s*(?:const\s*)?\)"
)
NOT_TYPES = mn.KEYWORDS | {"sizeof", "alignof", "__attribute__", "asm", "__asm__", "defined"}

Cast = collections.namedtuple("Cast", "thing target kind sub declared file line func source op")


def norm_type(base, stars):
    base = re.sub(r"\s+", " ", base.strip())
    base = {"unsigned int": "u32", "unsigned char": "u8", "unsigned short": "u16", "signed char": "s8"}.get(base, base)
    return base + (" " + stars if stars else "")


def collapse(flat):
    """`a[b[i].c].d(x, y(z))` -> `a[].d()`."""
    out, i = [], 0
    while i < len(flat):
        c = flat[i]
        if c in "([":
            out.append("()" if c == "(" else "[]")
            i = mn.match_close(flat, i) + 1
            continue
        out.append(c)
        i += 1
    return "".join(out)


def split_type(t):
    """`Foo **` -> ("Foo", 2)."""
    if t is None:
        return None, 0
    t = t.replace("[]", "*")
    return t.rstrip("* ").strip(), t.count("*")


class TypedModel(mn.Model):
    """magic_numbers' Model, with the declared types of the fields
    (with their `*`s), the function return types and the globals."""

    def __init__(self):
        super().__init__()
        self.full = {}  # (class, field) -> "u16", "Sprite *", "u8 *" (an array is a `*`)
        self.rettypes = collections.defaultdict(set)  # (class or None, function) -> {type}
        self.gtypes = {}  # global -> type

    def parse_body(self, text, lo, hi, cls):
        super().parse_body(text, lo, hi, cls)
        for stmt, block in mn.body_statements(text, lo, hi):
            stmt = re.sub(r"^\s*(?:(?:public|private|protected)\s*:\s*)+", "", stmt)
            if block or not stmt.strip() or re.match(r"\s*(?:friend|typedef|using|enum)\b", stmt):
                if block and "(" in stmt:
                    self.add_rettype(cls, stmt)
                continue
            if "(" in stmt.split("=")[0]:
                self.add_rettype(cls, stmt)
                continue
            m = mn.TYPE_RE.match(stmt)
            if not m:
                continue
            typ = m.group(1)
            for d in stmt[m.end() :].split("=")[0].split(","):
                d = re.sub(r":\s*\d+\s*$", "", d.strip())
                n = re.search(r"(\w+)\s*((?:\[[^\]]*\]\s*)*)$", d)
                if n:
                    stars = d[: n.start()].count("*") + (1 if n.group(2) else 0)
                    self.full[(cls, n.group(1))] = norm_type(typ, "*" * stars)

    def add_rettype(self, cls, stmt):
        p = stmt.find("(")
        m = re.search(r"((?:[A-Za-z_]\w*\s+|\*\s*)+)(~?\w+)\s*$", stmt[:p])
        if not m:
            return
        words = [w for w in re.findall(r"\w+", m.group(1)) if w not in mn.QUALIFIERS and w != "explicit"]
        if not words or words[-1] in NOT_TYPES:
            return
        self.rettypes[(cls, m.group(2))].add(norm_type(words[-1], "*" * m.group(1).count("*")))

    def load(self, files):
        super().load(files)
        fdecl = re.compile(
            r"(?:^|(?<=[;}]))\s*(?:(?:extern|static|inline|const|struct|class|union|virtual)\s+)*"
            r"([A-Za-z_]\w*)\s*(\**)\s*(?:(\w+)\s*::\s*)?(~?[A-Za-z_]\w*)\s*\(",
            re.M,
        )
        gdecl = re.compile(
            r"(?:^|(?<=[;}]))\s*(?:(?:extern|static|const|volatile|struct|class|union)\s+)*"
            r"([A-Za-z_]\w*)\s*(\**)\s*(?:const\s+)?([A-Za-z_]\w*)\s*((?:\[[^;\]]*\]\s*)*)(?=[;=])",
            re.M,
        )
        for path in files:
            _, text = mn.read_blanked(path)
            found = mn.scopes(text)
            inside = [(s.start, s.end) for s in found]
            for m in fdecl.finditer(text):
                if m.group(1) in NOT_TYPES or any(a < m.start(1) < b for a, b in inside):
                    continue
                self.rettypes[(m.group(3), m.group(4))].add(norm_type(m.group(1), m.group(2)))
            for m in gdecl.finditer(text):
                if m.group(1) in NOT_TYPES or any(a < m.start(1) < b for a, b in inside):
                    continue
                stars = m.group(2) + ("*" if m.group(4) else "")
                self.gtypes.setdefault(m.group(3), norm_type(m.group(1), stars))

    def field_full(self, cls, name):
        for c in self.ancestry(cls):
            if (c, name) in self.full:
                return self.full[(c, name)]
        return None

    def rettype(self, cls, name):
        for c in (self.ancestry(cls) if cls else []) + [None]:
            t = self.rettypes.get((c, name))
            if t:
                return next(iter(t)) if len(t) == 1 else None
        return None


class CastScan(mb.BitScan):
    # A declaration of a local or a parameter, with its type: `u16 x`, `Foo *p`.
    LOCAL = re.compile(
        r"(?:^|(?<=[;{}(,]))\s*(?:(?:const|volatile|static|register|struct|class|union)\s+)*"
        r"((?:unsigned|signed)\s+(?:char|short|int|long)|[A-Za-z_]\w*)\s*(\**)\s*(?:const\s+)?"
        r"([A-Za-z_]\w*)\s*((?:\[[^\]]*\]\s*)*)(?=[;=,)])",
        re.M,
    )

    def __init__(self, path, rel, model):
        super().__init__(path, rel, model)
        self.casts = []
        self.ltypes = collections.defaultdict(list)  # func scope start -> [(offset, name, type)]
        for m in self.LOCAL.finditer(self.text):
            if m.group(1) in NOT_TYPES or m.group(3) in NOT_TYPES or m.group(1) in ("return", "goto"):
                continue
            f = self.func_at(m.start(3), heads=True)
            if f:
                stars = m.group(2) + ("*" if m.group(4) else "")
                self.ltypes[f.start].append((m.start(3), m.group(3), norm_type(m.group(1), stars)))

    # -- types

    def is_type(self, word, tag):
        if tag or word in SCALARS or word in ("void", "f32", "float", "double") or " " in word:
            return True
        if word in NOT_TYPES:
            return False
        return self.model.known(word) or word in self.model.aliases

    def local_full(self, f, name, off):
        best = None
        for o, n, t in self.ltypes.get(f.start, ()) if f else ():
            if n == name and o <= off:
                best = t
        return best

    def type_of(self, expr, off):
        """The declared type of a chain (`p->f.x`, `x`, `Foo()`), or None."""
        flat = collapse(re.sub(r"\s+", "", expr))
        f = self.func_at(off)
        m = re.match(r"(\w+)", flat)
        if not m:
            return None
        base, rest = m.group(1), flat[m.end() :]
        if base == "this" and not rest:
            return (f.name + " *") if f and f.name else None
        segs = re.findall(r"(->|\.)(\w+)(\(\))?|(\[[^\]]*\])|(\([^()]*\))", rest)
        # the base's type
        if rest.startswith("("):
            cls = f.name if f and f.name and self.model.find_method(f.name, base) else None
            t = self.model.rettype(self.model.find_method(cls, base) if cls else None, base)
            segs = segs[1:]
        elif base == "this":
            t = (f.name + " *") if f and f.name else None
        else:
            t = self.local_full(f, base, off)
            if t is None and f and f.name and not self.is_local(f, base, off):
                t = self.model.field_full(f.name, base)
            if t is None:
                t = self.model.gtypes.get(base)
        for arrow, name, call, idx, args in segs:
            if t is None:
                return None
            cls, stars = split_type(t)
            if idx:
                t = norm_type(cls, "*" * (stars - 1)) if stars else None
                continue
            if args:
                continue
            if call:
                owner = self.model.find_method(cls, name)
                t = self.model.rettype(owner, name) if owner else None
                continue
            t = self.model.field_full(self.model.canon(cls), name)
        return t

    # -- casts

    def operand_end(self, i):
        """The end of the unary expression that starts at `i`."""
        text = self.text
        n = len(text)
        while i < n and text[i].isspace():
            i += 1
        while i < n and (text[i] in "-~!*&+" and text[i : i + 2] not in ("->",)):
            i += 1
            while i < n and text[i].isspace():
                i += 1
        if i >= n:
            return i
        if text[i] == "(":
            m = CAST_RE.match(text, i)
            j = mn.match_close(text, i) + 1
            if m and self.is_type(m.group(2), m.group(1)) and m.end() == j:
                return self.operand_end(j)  # a nested cast
            i = j
        else:
            m = re.match(r"(?:0[xX][0-9a-fA-F]+|\d+)[uUlL]*|[A-Za-z_]\w*", text[i:])
            if not m:
                return i
            i += m.end()
        while True:
            j = i
            while j < n and text[j].isspace():
                j += 1
            if j < n and text[j] in "([":
                i = mn.match_close(text, j) + 1
            elif text[j : j + 2] == "->":
                m = re.match(r"->\s*[A-Za-z_]\w*", text[j:])
                if not m:
                    return i
                i = j + m.end()
            elif text[j : j + 1] == "." and re.match(r"\.\s*[A-Za-z_]", text[j:]):
                i = j + re.match(r"\.\s*[A-Za-z_]\w*", text[j:]).end()
            elif text[j : j + 2] in ("++", "--"):
                i = j + 2
            else:
                return i

    def scan_casts(self):
        text = self.text
        for m in CAST_RE.finditer(text):
            if not self.func_at(m.start()):
                continue
            tag, word, stars = m.group(1), m.group(2), m.group(3)
            if not self.is_type(word, tag):
                continue
            j = m.start()
            while j > 0 and text[j - 1].isspace():
                j -= 1
            prev = text[j - 1] if j else ";"
            if prev.isalnum() or prev in "_)]" or text[max(0, j - 6) : j] == "sizeof":
                if not re.search(r"\b(?:return|case)$", text[max(0, j - 6) : j]):
                    continue  # a call, a declarator or `sizeof(T)`
            k = m.end()
            while k < len(text) and text[k].isspace():
                k += 1
            if k >= len(text) or not (text[k].isalnum() or text[k] in "_(*&-~!"):
                continue
            end = self.operand_end(m.end())
            if end <= k:
                continue
            target = norm_type(word, stars)
            self.add_cast(m.start(), k, end, target)

    def add_cast(self, at, start, end, target):
        text = self.text
        operand = text[start:end]
        inner = mb.strip_expr(operand)
        op = re.sub(r"\s+", " ", operand.strip())
        tbase, tstars = split_type(target)
        kind, sub, thing, declared = None, "", None, None
        bo = self.byte_offset(operand, start, tstars)
        if bo:
            thing, sub = bo
            kind = "byteoff"
        elif inner.startswith("&"):
            k = self.thing_of(inner[1:].strip(), start)
            thing = "&" + k if k else None
            kind = "pointer"
        else:
            thing = self.thing_of(inner, start)
            if thing:
                declared = self.type_of(inner, start)
        if thing is None:
            thing = "<expr>"
        if kind is None:
            dbase, dstars = split_type(declared)
            if declared and norm_type(dbase, "*" * dstars) == target:
                kind = "same"
            elif tstars == 0 and tbase in SCALARS:
                if declared and dstars:
                    kind = "ptrint"
                else:
                    kind = "scalar"
                    if declared in SCALARS:
                        (db, ds), (tb, ts) = SCALARS[declared], SCALARS[tbase]
                        sub = "narrow" if tb < db else "widen" if tb > db else "sign"
                        if tb != db and ds != ts:
                            sub += "+sign"
            elif tstars:
                kind = "pointer"
                if declared and dstars == tstars and dbase != tbase:
                    a, b = self.model.canon(dbase), self.model.canon(tbase)
                    if self.model.is_a(a, b) or self.model.is_a(b, a):
                        kind = "derived"
                        sub = "down" if self.model.is_a(b, a) else "up"
                    elif dbase == "void":
                        sub = "from void *"
                    elif self.model.known(a) and self.model.known(b):
                        sub = "view"
                elif declared and not dstars:
                    sub = "from int"
            else:
                kind = "scalar" if tbase not in ("void",) else "void"
        if kind == "void":
            return
        f = self.func_at(at)
        fname = f and ((f.name + "::" if f.name and f.fname != f.name else "") + (f.fname or "?"))
        line = mn.line_of(text, at)
        self.casts.append(
            Cast(thing, target, kind, sub, declared, self.rel, line, fname, self.src_lines[line - 1].strip(), op)
        )

    def byte_offset(self, operand, off, tstars):
        """(thing, `base + N`) when the operand is a byte offset from a
        pointer: `((u8 *)p + N)`, `((u32)p + N)`, `(p + N)` with p a byte
        pointer, an integer address or an address macro; or None."""
        s = operand.strip()
        if not (s.startswith("(") and mn.match_close(s, 0) == len(s) - 1):
            return None
        s = s[1:-1]
        # the first top-level binary `+` or `-` (not `->`, `++`, `--`)
        depth, at = 0, -1
        for k, c in enumerate(s):
            if c in "([":
                depth += 1
            elif c in ")]":
                depth -= 1
            elif depth == 0 and c in "+-" and k > 0 and s[k + 1 : k + 2] not in (">", "+", "-"):
                if s[k - 1] in "+-":
                    continue
                at = k
                break
        if at < 0:
            return None
        left, right = s[:at].strip(), re.sub(r"\s+", " ", s[at + 1 :].strip())
        m = CAST_RE.match(left)
        if m and m.start() == 0:
            base = mb.strip_expr(left)
            bt, bs = norm_type(m.group(2), ""), len(m.group(3))
            if not ((bt in BYTE_PTRS and bs == 1) or (bt in SCALARS and bs == 0)):
                return None
            if bs == 0:  # `(u32)p + N`: only a pointer p
                d = self.type_of(re.sub(r"\s+", "", base), off)
                if not (d and split_type(d)[1]) and not re.match(r"^[A-Z][A-Z0-9_]*\b", base):
                    return None
        else:
            if not tstars:
                return None
            base = mb.strip_expr(left)
            d = self.type_of(re.sub(r"\s+", "", base), off)
            db, ds = split_type(d)
            if d is None:
                if not re.match(r"^[A-Z][A-Z0-9_]*\b", base):
                    return None  # an unknown local: not known to be bytes
            elif not ((db in BYTE_PTRS and ds == 1) or (db in SCALARS and ds == 0)):
                return None
        thing = self.thing_of(base, off) if base else None
        return thing or "<expr>", f"{thing or base} {s[at]} {right}"

    def thing_of(self, expr, off):
        flat = collapse(re.sub(r"\s+", "", expr))
        if not flat or not mb.CHAIN.match(flat) or re.match(r"^(?:0[xX]|\d)", flat):
            return None
        # field_of wants offsets into self.text: find the chain from `off`
        i = self.text.find(expr.strip(), off)
        key = None
        if i >= 0:
            try:
                key, _ = self.field_of(i, self.operand_end(i))
            except (IndexError, AttributeError):
                key = None
        if not key or key.startswith("?") or key.endswith("()") and "::" not in key:
            key = self.resolve(flat, off) or key
        return self.norm_key(key)

    def resolve(self, flat, off):
        """The thing of a chain field_of can't take (nested `[]`, call
        arguments): its last member and the class that declares it."""
        depth, last, sep = 0, -1, ""
        for k, c in enumerate(flat):
            if c in "([":
                depth += 1
            elif c in ")]":
                depth -= 1
            elif depth == 0 and flat[k : k + 2] == "->":
                last, sep = k, "->"
            elif depth == 0 and c == "." and flat[k - 1 : k] != "-":
                last, sep = k, "."
        f = self.func_at(off)
        if last < 0:
            m = re.match(r"(\w+)(.*)$", flat)
            if not m:
                return None
            base, rest = m.group(1), m.group(2)
            if rest.startswith("()"):
                owner = f and f.name and self.model.find_method(f.name, base)
                return f"{owner}::{base}()" if owner else f"{base}()"
            if base == "this":
                return f"this ({f.name if f else '?'})"
            if f and self.is_local(f, base, off):
                return f"local {base} ({f.fname or '?'}, {self.rel})"
            if f and f.name and self.model.field_type(f.name, base):
                return f"{self.model.field_type(f.name, base)[0]}::{base}{rest}"
            return f"global {base}{rest}"
        prefix, member = flat[:last], flat[last + len(sep) :]
        t = self.type_of(prefix, off)
        cls = t and self.model.canon(split_type(t)[0])
        name = re.match(r"\w+", member).group(0)
        owner = None
        if cls:
            ft = self.model.field_type(cls, name)
            owner = ft[0] if ft else self.model.find_method(cls, name)
        if owner and "::<anon" in owner:
            owner = owner.split("::<anon")[0]
        return f"{owner or '?'}::{member}"

    def norm_key(self, key):
        """`Foo::Bar(id)` -> `Foo::Bar()`; an undeclared upper-case name
        (`OBJ_PLTT`, `BG_SCREEN_ADDR(26)`) is a macro."""
        if not key:
            return key
        if not key.startswith("local "):
            while re.search(r"(?<=\w)\([^()]+\)", key):
                key = re.sub(r"(?<=\w)\([^()]+\)", "()", key)
        m = re.match(r"(?:local|global) ([A-Z][A-Z0-9_]*)\b", key) or re.match(r"([A-Z][A-Z0-9_]*)\(\)$", key)
        if m and m.group(1) not in self.model.gtypes:
            return f"macro {m.group(1)}"
        return key

    # -- uses (the cast and the uncast reads of each thing)

    def count_uses(self, names, keys):
        """{thing: [reads, writes]} for the things in `keys`."""
        out = collections.defaultdict(lambda: [0, 0])
        text = self.text
        for m in re.finditer(r"\b[A-Za-z_]\w*\b", text):
            name = m.group(0)
            if name not in names:
                continue
            f = self.func_at(m.start())
            if not f:
                continue
            j = m.end()
            while j < len(text) and text[j].isspace():
                j += 1
            call = text[j : j + 1] == "("
            if call:
                j = mn.match_close(text, j) + 1
            while j < len(text) and text[j : j + 1] == "[":
                j = mn.match_close(text, j) + 1
            k = j
            while k < len(text) and text[k].isspace():
                k += 1
            if text[k : k + 2] == "->" or (text[k : k + 1] == "." and text[k : k + 2] != ".."):
                continue  # not the end of the chain
            s = mb.expr_start(text, j)
            # skip a declaration: `u16 x`, `Foo *x`
            if not call and re.search(r"[A-Za-z_]\w*\s*\**\s*$", text[max(0, m.start() - 40) : m.start()]):
                before = text[max(0, m.start() - 40) : m.start()]
                w = re.search(r"([A-Za-z_]\w*)\s*\**\s*$", before).group(1)
                if w not in NOT_TYPES or w in ("const", "struct", "class", "union"):
                    if text[m.start() - 1 : m.start()] not in (">", "."):
                        continue
            # the chain's own start (without a leading cast, `*` or `!`)
            c = s
            while True:
                cm = CAST_RE.match(text, c)
                if cm and self.is_type(cm.group(2), cm.group(1)):
                    c = cm.end()
                    while text[c].isspace():
                        c += 1
                    continue
                if text[c] in "*!&-~(":
                    c += 1
                    while text[c].isspace():
                        c += 1
                    continue
                break
            key = self.norm_key(self.field_of(c, j)[0])
            if not key or key not in keys:
                if key and "&" + key in keys and text[max(0, c - 2) : c].strip().endswith("&"):
                    key = "&" + key
                else:
                    continue
            rest = text[k : k + 3]
            write = (rest[:1] == "=" and rest[:2] != "==") or rest[:2] in ("++", "--") or re.match(r"[-+*/|&^]=", rest)
            out[key][1 if write and not call else 0] += 1
        return out


def scan(paths=None):
    files = mn.source_files()
    model = TypedModel()
    model.load(files)
    mb.load_unions(files)
    scans = []
    for path, rel in mb.source_files():
        if paths and not any(rel == p or rel.startswith(p.rstrip("/") + "/") for p in paths):
            continue
        s = CastScan(path, rel, model)
        s.scan_casts()
        scans.append(s)
    casts = [c for s in scans for c in s.casts]
    keys = {c.thing for c in casts if c.thing != "<expr>"}
    names = {re.findall(r"\w+", k.split("(")[0] if k.startswith("local") else k)[-1] for k in keys}
    names |= {re.sub(r"\(\)$", "", k).split("::")[-1].split(".")[-1].replace("[]", "") for k in keys}
    names = {n for n in names if n}
    uses = collections.defaultdict(lambda: [0, 0])
    for s in scans:
        for k, (r, w) in s.count_uses(names, keys).items():
            uses[k][0] += r
            uses[k][1] += w
    return casts, uses


def grouped(casts, uses):
    things = collections.defaultdict(list)
    for c in casts:
        things[c.thing].append(c)
    rows = []
    for thing, cs in things.items():
        reads = max(uses.get(thing, [0, 0])[0], len(cs))
        top, n = collections.Counter(c.target for c in cs).most_common(1)[0]
        consistency = n / reads if reads else 0
        rows.append((thing, cs, reads, uses.get(thing, [0, 0])[1], top, n, consistency))
    rows.sort(key=lambda r: (r[0] == "<expr>", -(r[5] * r[6]), -len(r[1]), r[0]))
    return rows


def kinds_of(cs):
    c = collections.Counter(c.kind + (f"/{c.sub}" if c.sub and c.kind != "byteoff" else "") for c in cs)
    return ", ".join(f"{k} {n}" for k, n in c.most_common())


def declared_of(cs):
    d = {c.declared for c in cs if c.declared}
    return "/".join(sorted(d)) if d else "?"


def text_listing(rows, out, samples):
    for thing, cs, reads, writes, top, n, cons in rows:
        targets = collections.Counter(c.target for c in cs)
        tl = ", ".join(f"({t}) {k}" for t, k in targets.most_common())
        out.write(
            f"{thing} [{declared_of(cs)}]: cast {len(cs)}/{reads} reads"
            f" (+{writes} writes); {tl}; {kinds_of(cs)}\n"
        )
        for c in cs[:samples] if samples else cs:
            out.write(f"    {c.file}:{c.line} {c.func or ''}: {c.source}\n")
        if samples and len(cs) > samples:
            out.write(f"    ... {len(cs) - samples} more\n")


def summary(casts):
    k = collections.Counter(c.kind for c in casts)
    return ", ".join(f"{x} {k[x]}" for x in KINDS if k[x])


# What a thing is, from its key.
CATEGORIES = ("field", "local", "global", "result", "address", "macro", "expr")


def category(thing):
    if thing == "<expr>":
        return "expr"
    if thing.startswith("macro "):
        return "macro"
    if thing.startswith("&"):
        return "address"
    if thing.startswith(("local ", "this ")):
        return "local"
    if thing.startswith("global "):
        return "global"
    if thing.endswith("()"):
        return "result"
    return "field"


def table(rows, out, samples, top):
    out.write("| thing | declared | cast / reads | to | kinds | where |\n|---|---|---:|---|---|---|\n")
    for thing, cs, reads, writes, t, n, cons in rows[:top]:
        targets = ", ".join(f"`{x}` {k}" for x, k in collections.Counter(c.target for c in cs).most_common(3))
        funcs = sorted({c.func or "?" for c in cs})
        more = " ..." if len(funcs) > samples else ""
        where = ", ".join(funcs[:samples]) + more
        out.write(
            f"| `{thing}` | `{declared_of(cs)}` | {len(cs)}/{reads} | {targets} | {kinds_of(cs)} | {where} |\n"
        )
    if len(rows) > top:
        out.write(f"\n... and {len(rows) - top} more (`tools/magic_numbers.py --casts`).\n")
    out.write("\n")


def report(rows, casts, out, samples=4, top=60):
    out.write("# Casts per thing (#855)\n\n")
    out.write(f"{len(casts)} casts on {len(rows)} things: {summary(casts)}.\n\n")
    out.write("| thing | things | casts | " + " | ".join(KINDS) + " |\n|---|---:|---:|" + "---:|" * len(KINDS) + "\n")
    for cat in CATEGORIES:
        sel = [r for r in rows if category(r[0]) == cat]
        cs = [c for r in sel for c in r[1]]
        k = collections.Counter(c.kind for c in cs)
        out.write(f"| {cat} | {len(sel)} | {len(cs)} | " + " | ".join(str(k[x]) for x in KINDS) + " |\n")
    out.write("\n")
    out.write("## Retype candidates\n\n")
    out.write(
        "Fields, locals, parameters, globals and function results, ranked by cast uses x consistency\n"
        "(the share of the reads cast to the most common type).\n\n"
    )
    table([r for r in rows if category(r[0]) in ("field", "local", "global", "result")], out, samples, top)
    out.write("## Addresses (`(T *)&x`): struct views and AsX() helpers\n\n")
    table([r for r in rows if category(r[0]) == "address"], out, samples, top // 2)
    out.write("## Address macros (`(T *)REG_ADDR_X`, `(T *)(BG_PLTT + N)`)\n\n")
    table([r for r in rows if category(r[0]) == "macro"], out, samples, top // 2)
    for kind in KINDS:
        ks = [r for r in rows if any(c.kind == kind for c in r[1])]
        out.write(f"## {kind}: {sum(1 for c in casts if c.kind == kind)} casts, {len(ks)} things\n\n")
        for thing, cs, reads, writes, t, n, cons in ks[:top]:
            sel = [c for c in cs if c.kind == kind]
            subs = collections.Counter(c.sub or c.target for c in sel)
            cells = ", ".join(f"{s} ({k})" for s, k in subs.most_common(4))
            out.write(f"- `{thing}`: {len(sel)} ({cells})\n")
        out.write("\n")


def run(args):
    casts, uses = scan(args.path)
    if getattr(args, "field", None):
        pat = re.compile(args.field)
        casts = [c for c in casts if pat.search(c.thing)]
    if getattr(args, "kind", None):
        casts = [c for c in casts if c.kind in args.kind]
    if args.json:
        json.dump([c._asdict() for c in casts], sys.stdout, indent=1)
        sys.stdout.write("\n")
        return 0
    rows = grouped(casts, uses)
    if args.report:
        if args.report == "-":
            report(rows, casts, sys.stdout, args.samples)
        else:
            with open(args.report, "w") as f:
                report(rows, casts, f, args.samples)
        return 0
    text_listing(rows, sys.stdout, args.samples)
    print(f"{len(casts)} casts on {len(rows)} things: {summary(casts)}", file=sys.stderr)
    return 0
