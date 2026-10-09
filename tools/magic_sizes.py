#!/usr/bin/env python3
"""`tools/magic_numbers.py --sizes`: raw sizes, strides, offsets and
hardware values that have a `sizeof`, a member or a named constant (#820).

Scans the game code under src/ (not src/data/, not lib/) for integer
literals in six kinds of places:

  alloc    the size argument of mem_alloc, IwramAlloc, OperatorNew,
           OperatorNewArray and `operator new` (and mem_alloc's heap flag,
           MEM_HEAP_*), plus the `OperatorNew(0xNN)` sizes comments
           still quote for code that is now `new X`;
  copy     the length argument of DmaCopy*/DmaFill*/DmaClear* (include/
           gba/dma_macros.h), CpuSet/CpuFastSet (the unit count in the
           control word), MemCopy32, memcpy and memset;
  stride   `i * K` / `K * i` where K is the size of a struct or class;
  offset   `(u8 *)p + K`, `((u8 *)p)[K]` and `(u32)p + K` where p is a
           pointer to a struct or class with a member at offset K (or,
           with a stride term, a member of one element of an array
           member);
  hwaddr   `VRAM + K`, `PLTT + K`, ... and raw I/O, palette, VRAM and OAM
           addresses that are BG_SCREEN_ADDR(n), BG_CHAR_ADDR(n), OBJ_PLTT,
           REG_ADDR_*, ...;
  hw       literals assigned to, OR-ed into, masked out of or compared
           with a REG_* register (or a shadow named like one: dispcnt,
           bldcnt, bgNcnt, ...) that include/gba/io_reg.h's defines spell
           (`DISPCNT_BG2_ON | DISPCNT_OBJ_ON`, `BGCNT_PRIORITY(3) | ...`).

The struct and class sizes, member offsets and the types of every
variable, parameter and `this` come from the compiler's own debug info:
the layout database of tools/layout_audit.py (every TU compiled again
with -g into a temporary directory, build/ untouched), extended here with
each function's locals and parameters and each TU's globals, so an
expression like `self->players[i]` or `(LinkPlayer *)(self + 8)` gets a
type without parsing any header.

Each hit has a confidence:

  high     the type comes from the site itself: the cast or the variable
           the allocation is assigned to, the copy's source or
           destination, an expression the stride indexes or the pointer
           the offset is added to, or a hardware register;
  medium   the function uses a variable, global or member whose type is
           (a pointer to, an array of, or a struct with an array of) a
           struct of that size (a stride or copy length); a copy into
           something named like a palette or tiles; an offset read as a
           type of another width than the member; or the register is a
           shadow named like one;
  low      the value is the size of a few structs (listed; a stride needs
           a hex literal of 8 or more and at most four candidates), or an offset
           whose result is cast to another struct (a matching idiom like
           `(LinkPlayer *)((u8 *)this + 8)`), with nothing at the site to
           say which.

One hit per literal; each has the literal, the expression it replaces
(`expr`: the literal, or `VRAM + 0xE000`, `(u8 *)p + 0x24`), the
suggestions, the confidence and its context. A suggestion may use a
define include/gba/defines.h doesn't have yet (`new_defines`, `(new: ...)`
in the listing): PALETTE_SIZE_16 (0x20, one 16-colour palette),
BG_SCREEN_SIZE (0x800) and BG_CHAR_SIZE (0x4000).

Every suggestion has the literal's value, but not always its type:
`sizeof` is unsigned, and some literals are kept on purpose for matching
(#662). Rebuild and compare after each change.

Usage:
  tools/magic_numbers.py --sizes                    every hit, by directory
  tools/magic_numbers.py --sizes --show             ... with the source line
  tools/magic_numbers.py --sizes --category C       only C (alloc, copy, ...)
  tools/magic_numbers.py --sizes --min-confidence high
  tools/magic_numbers.py --sizes --path src/link    only files under a path
  tools/magic_numbers.py --sizes --json             machine-readable, for agents
  tools/magic_numbers.py --sizes --report [OUT]     Markdown counts + samples
  tools/magic_numbers.py --sizes --db /tmp/sizes.json ...
      save the type database and reuse it (--rebuild recompiles; do that
      after editing a header or a declaration)
"""

import collections
import concurrent.futures
import json
import os
import re
import shutil
import sys
import tempfile

TOOLS = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, TOOLS)
import layout_audit as la  # noqa: E402
import magic_numbers as mn  # noqa: E402

ROOT = mn.ROOT
GBA_HEADERS = [os.path.join(ROOT, "include", "gba", f) for f in ("defines.h", "io_reg.h", "dma_macros.h")]
MEMORY_HEADER = os.path.join(ROOT, "include", "memory.h")
DB_VERSION = 1

CATEGORIES = collections.OrderedDict(
    [
        ("alloc", "allocation sizes (mem_alloc, IwramAlloc, OperatorNew, operator new) and heap flags"),
        ("copy", "copy/fill lengths (DmaCopy*/DmaFill*/DmaClear*, CpuSet, CpuFastSet, MemCopy32, memcpy, memset)"),
        ("stride", "index strides `i * K` equal to a struct's size"),
        ("offset", "raw `+ K` byte offsets into a struct with a member at K"),
        ("hwaddr", "VRAM/palette/OAM/I/O addresses with a named base"),
        ("hw", "values written to, masked from or compared with a hardware register"),
    ]
)
CONFIDENCES = ("high", "medium", "low")

# Named sizes (include/gba/defines.h) and the ones #820 proposes.
NEW = " (new)"
# The defines `(new)` suggestions use, which include/gba/defines.h doesn't
# have yet: name -> (value, meaning).
NEW_DEFINES = {
    "PALETTE_SIZE_16": (0x20, "one 16-colour palette"),
    "BG_SCREEN_SIZE": (0x800, "one BG screen block (BG_SCREEN_ADDR step)"),
    "BG_CHAR_SIZE": (0x4000, "one BG character block (BG_CHAR_ADDR step)"),
}
LOW_CANDIDATES = 4  # value-only hits list at most this many structs

# ---------------------------------------------------------------------------
# The type database


def tree_of(t, off, depth=0):
    """A JSON-able type tree for the DIE at off: ["int", size], ["ptr", T],
    ["arr", [dims], T], ["agg", key, name], ["fn"], ["void"] or ["?"]."""
    if depth > 8:
        return ["?"]
    off = t.strip(off)
    d = t.dies.get(off)
    if d is None:
        return ["void"]
    tag = d["tag"]
    if tag in ("DW_TAG_pointer_type", "DW_TAG_reference_type"):
        return ["ptr", tree_of(t, la.attr_ref(d), depth + 1)]
    if tag == "DW_TAG_array_type":
        return ["arr", t.array_dims(d), tree_of(t, la.attr_ref(d), depth + 1)]
    if tag in la.AGGREGATE_TAGS:
        name = t.agg_name(off)
        if "DW_AT_declaration" in d["attrs"] or "DW_AT_byte_size" not in d["attrs"] or t.is_pmf(off):
            return ["agg", None, name]
        return ["agg", t.key_of(off), name]
    if tag in ("DW_TAG_base_type", "DW_TAG_enumeration_type"):
        return ["int", t.size_of(off) or 4]
    if tag == "DW_TAG_subroutine_type":
        return ["fn"]
    return ["?"]


def collect_tu(t, mtypes):
    """Records (via t.collect), member types, functions and globals of one TU."""
    t.collect()
    for off, d in t.dies.items():
        if d["tag"] in la.AGGREGATE_TAGS and "DW_AT_declaration" not in d["attrs"] \
                and "DW_AT_byte_size" in d["attrs"] and not t.is_pmf(off):
            key = t.key_of(off)
            if key is None or key in mtypes:
                continue
            ms = []
            for c in d["children"]:
                cd = t.dies[c]
                if cd["tag"] not in ("DW_TAG_member", "DW_TAG_inheritance"):
                    continue
                ms.append([la.attr_name(cd) or "", tree_of(t, la.attr_ref(cd))])
            mtypes[key] = ms
    funcs, globs = [], {}
    src = os.path.normpath(t.src)
    for off, d in t.dies.items():
        if d["tag"] == "DW_TAG_variable" and off in top_level(t):
            name = la.attr_name(d)
            if name and la.attr_ref(d) is not None:
                globs.setdefault(name, tree_of(t, la.attr_ref(d)))
        if d["tag"] != "DW_TAG_subprogram" or "DW_AT_low_pc" not in d["attrs"]:
            continue
        spec = d
        for ref in ("DW_AT_specification", "DW_AT_abstract_origin"):
            r = la.attr_ref(d, ref)
            if r is not None and r in t.dies:
                spec = t.dies[r]
        line = la.attr_int(d, "DW_AT_decl_line") or la.attr_int(spec, "DW_AT_decl_line")
        f = t.file_of(d) or t.file_of(spec)
        if not line or not f or not os.path.normpath(f).endswith(src):
            continue
        vars_ = []
        walk_vars(t, d, vars_)
        funcs.append({"name": la.attr_name(d) or la.attr_name(spec) or "?", "line": line, "vars": vars_})
    funcs.sort(key=lambda f: f["line"])
    return funcs, globs


def top_level(t):
    if not hasattr(t, "_top"):
        t._top = set()
        for off, d in t.dies.items():
            if d["tag"] == "DW_TAG_compile_unit":
                t._top.update(d["children"])
    return t._top


def walk_vars(t, d, out):
    for c in d["children"]:
        cd = t.dies[c]
        if cd["tag"] in ("DW_TAG_formal_parameter", "DW_TAG_variable"):
            name = la.attr_name(cd)
            if name and la.attr_ref(cd) is not None:
                out.append([name, la.attr_int(cd, "DW_AT_decl_line") or 0, tree_of(t, la.attr_ref(cd))])
        elif cd["tag"] == "DW_TAG_lexical_block":
            walk_vars(t, cd, out)


def build_db(jobs):
    workdir = tempfile.mkdtemp(prefix="magic_sizes_")
    try:
        objects = la.compile_objects(workdir, False, jobs)
        db = la.LayoutDB()
        with concurrent.futures.ThreadPoolExecutor(max_workers=jobs) as ex:
            dumps = list(ex.map(lambda so: (so[0],) + la.readelf(so[1]), objects))
        tus = [(src, la.parse_dies(info), la.parse_files(line)) for src, info, line in dumps]
        mtypes, files = {}, {}
        for src, dies, fl in sorted(tus, key=lambda x: not x[2]):
            funcs, globs = collect_tu(la.TU(src, dies, fl, db), mtypes)
            files[src] = {"funcs": funcs, "globals": globs}
    finally:
        shutil.rmtree(workdir, ignore_errors=True)
    print(f"{len(db.records)} layouts, {sum(len(f['funcs']) for f in files.values())} functions "
          f"from {len(objects)} objects", file=sys.stderr)
    return {"version": DB_VERSION, "records": db.records, "mtypes": mtypes, "files": files}


def load_db(path, rebuild, jobs):
    if path and os.path.exists(path) and not rebuild:
        with open(path) as f:
            db = json.load(f)
        if db.get("version") == DB_VERSION:
            return db
    db = build_db(jobs)
    if path:
        with open(path, "w") as f:
            json.dump(db, f)
    return db


# ---------------------------------------------------------------------------
# Types


INT_TYPES = {
    "u8": 1, "s8": 1, "vu8": 1, "vs8": 1, "char": 1, "bool8": 1, "uint8_t": 1, "int8_t": 1,
    "u16": 2, "s16": 2, "vu16": 2, "vs16": 2, "short": 2, "bool16": 2, "uint16_t": 2, "int16_t": 2,
    "u32": 4, "s32": 4, "vu32": 4, "vs32": 4, "int": 4, "long": 4, "unsigned": 4, "bool32": 4,
    "uint32_t": 4, "int32_t": 4, "uintptr_t": 4, "intptr_t": 4, "size_t": 4, "signed": 4,
    "u64": 8, "s64": 8, "f32": 4, "float": 4,
}
BYTE_PTR_CASTS = re.compile(
    r"\(\s*(?:const\s+|volatile\s+)*(?:u8|s8|vu8|char|unsigned\s+char|signed\s+char|void)\s*\*\s*\)")
INT_CASTS = re.compile(r"\(\s*(?:u32|s32|uintptr_t|intptr_t|int|unsigned(?:\s+int)?|long)\s*\)")


class Types:
    def __init__(self, db):
        self.rec = db["records"]
        self.mtypes = db["mtypes"]
        self.files = db["files"]
        self.by_name = collections.defaultdict(list)
        for k, r in self.rec.items():
            if r["name"] and not r["anon"]:
                self.by_name[r["name"]].append(k)
        self._sizes = None
        self._members = {}

    def key(self, name, src=None):
        cands = self.by_name.get(name, [])
        if src:
            mine = [k for k in cands if src in self.rec[k]["tus"]]
            cands = mine or cands
        return cands[0] if cands else None

    def resolve(self, tree, src=None):
        """An ["agg", None, name] (a declaration) resolved to its definition."""
        if tree and tree[0] == "agg" and tree[1] is None and tree[2]:
            k = self.key(tree[2], src)
            return ["agg", k, tree[2]] if k else tree
        return tree

    def size(self, tree, src=None):
        tree = self.resolve(tree, src)
        if not tree:
            return None
        k = tree[0]
        if k == "int":
            return tree[1]
        if k in ("ptr", "fn"):
            return 4
        if k == "agg":
            return self.rec[tree[1]]["size"] if tree[1] in self.rec else None
        if k == "arr":
            es = self.size(tree[2], src)
            n = 1
            for dim in tree[1]:
                if dim is None:
                    return None
                n *= dim
            return None if es is None else n * es
        return None

    def spell(self, tree, src=None):
        """C++ spelling of a type, for sizeof(...)."""
        tree = self.resolve(tree, src)
        k = tree[0]
        if k == "int":
            return {1: "u8", 2: "u16", 4: "u32", 8: "u64"}.get(tree[1], "u32")
        if k == "agg":
            r = self.rec.get(tree[1])
            name = tree[2] or (r and r["name"]) or "?"
            # The C-style lower-case tags keep their `struct` (as the code
            # spells them); the C++ classes don't need one.
            if r and r["kind"] in ("struct", "union") and name[:1].islower():
                return f"{r['kind']} {name}"
            return name
        if k == "ptr":
            return self.spell(tree[1], src) + " *"
        if k == "arr":
            return self.spell(tree[2], src) + "".join(f"[{d}]" for d in tree[1])
        return "?"

    def members(self, key):
        """[(name, tree)] of a record, base classes flattened in place."""
        if key in self._members:
            return self._members[key]
        out = []
        for (name, tree), m in zip(self.mtypes.get(key, []), self.rec[key]["members"]):
            if m.get("base") and m["sub"] in self.rec:
                out += self.members(m["sub"])
            else:
                out.append((name, tree))
        self._members[key] = out
        return out

    def member(self, key, name):
        for n, tree in self.members(key):
            if n == name:
                return tree
        # An anonymous union/struct member's fields.
        for n, tree in self.members(key):
            if not n and tree[0] == "agg" and tree[1] in self.rec:
                sub = self.member(tree[1], name)
                if sub:
                    return sub
        return None

    def struct_sizes(self):
        """size -> [record keys] of the named, non-data, non-trivial records."""
        if self._sizes is None:
            self._sizes = collections.defaultdict(list)
            for k, r in self.rec.items():
                if r["anon"] or r["file"] in (None, "<internal>") or r["file"].startswith("src/data/") \
                        or r["name"].startswith("__") or r["size"] < 4:
                    continue
                if not any(self.rec[x]["name"] == r["name"] for x in self._sizes[r["size"]]):
                    self._sizes[r["size"]].append(k)
        return self._sizes

    def paths_at(self, key, off, width=None):
        """Member paths starting at byte off of record key: (path, size, pad),
        best first (named, of the access width, outermost)."""
        out = []

        def walk(k, base, prefix, depth):
            if depth > 8:
                return
            for m in self.rec[k]["members"]:
                start = base + m["off"]
                size = m["size"] or 0
                if m.get("bitfield"):
                    continue
                if m.get("base"):
                    if m["sub"] in self.rec:
                        walk(m["sub"], start, prefix, depth + 1)
                    continue
                path = prefix + m["name"] if m["name"] else prefix.rstrip(".")
                if start == off and m["name"]:
                    out.append((path, size, la.is_pad(m["name"]), depth))
                if not (start <= off < start + max(size, 1)):
                    continue
                if m["sub"] in self.rec and m["class"] == "agg":
                    walk(m["sub"], start, (path + ".") if m["name"] else prefix, depth + 1)
                elif m["class"] == "array" and size and off >= start:
                    tree = self.member(k, m["name"])
                    if tree and tree[0] == "arr":
                        es = self.size(tree[2])
                        if es:
                            i, rest = divmod(off - start, es)
                            el = self.resolve(tree[2])
                            if rest == 0:
                                out.append((f"{path}[{i}]", es, la.is_pad(m["name"]), depth))
                            if el[0] == "agg" and el[1] in self.rec:
                                walk(el[1], start + i * es, f"{path}[{i}].", depth + 1)

        walk(key, 0, "", 0)
        out.sort(key=lambda p: (p[2], width is not None and p[1] != width, p[3]))
        seen, res = set(), []
        for p in out:
            if p[0] not in seen:
                seen.add(p[0])
                res.append(p)
        return res


def parse_type_name(text, types, src):
    """`struct X *`, `const u16 *`, `LinkPlayer` ... -> tree, or None."""
    text = re.sub(r"\b(?:const|volatile|struct|class|union|signed)\b", " ", text).strip()
    stars = text.count("*")
    base = text.replace("*", " ").split()
    if not base:
        return None
    if base[0] == "unsigned" and len(base) > 1:
        base = base[1:]
    name = base[-1] if base[0] in ("unsigned", "long", "short") and len(base) > 1 else base[0]
    if name in INT_TYPES:
        tree = ["int", INT_TYPES[name]]
    elif name == "void":
        tree = ["void"]
    else:
        k = types.key(name, src)
        if not k:
            return None
        tree = ["agg", k, name]
    for _ in range(stars):
        tree = ["ptr", tree]
    return tree


# ---------------------------------------------------------------------------
# Expressions


TOKEN_RE = re.compile(r"\s*(->|\+\+|--|<<|>>|<=|>=|==|!=|&&|\|\||[A-Za-z_]\w*|0[xX][0-9a-fA-F]+[uUlL]*"
                      r"|\d+[uUlL]*|.)", re.S)
TYPE_WORDS = set(INT_TYPES) | {"void", "const", "volatile", "struct", "class", "union", "unsigned", "signed"}


def tokenize(text):
    toks = []
    pos = 0
    while pos < len(text):
        m = TOKEN_RE.match(text, pos)
        if not m or m.end() == pos:
            break
        if m.group(1).strip():
            toks.append((m.group(1), m.start(1)))
        pos = m.end()
    return toks


def match_paren(toks, i):
    """The index of the bracket closing the one at token i, or None."""
    depth = 0
    for j in range(i, len(toks)):
        t = toks[j][0]
        if t in "([{":
            depth += 1
        elif t in ")]}":
            depth -= 1
            if depth == 0:
                return j
    return None


class Scope:
    """Name lookup at one line of one file: locals, `this`'s members, globals."""

    def __init__(self, types, src, line):
        self.types = types
        self.src = src
        info = types.files.get(src, {"funcs": [], "globals": {}})
        self.func = None
        for f in info["funcs"]:
            if f["line"] <= line:
                self.func = f
        self.globals = info["globals"]
        self.line = line

    def var(self, name):
        if self.func:
            best = None
            for n, decl, tree in self.func["vars"]:
                if n == name and (best is None or (decl <= self.line and decl >= best[0])):
                    best = (decl, tree)
            if best:
                return self.types.resolve(best[1], self.src)
            this = self.var_this()
            if this:
                t = self.member(this, name)
                if t:
                    return t
        t = self.globals.get(name)
        return self.types.resolve(t, self.src) if t else None

    def var_this(self):
        if not self.func:
            return None
        for n, _, tree in self.func["vars"]:
            if n == "this":
                return self.types.resolve(tree, self.src)
        return None

    def member(self, tree, name):
        tree = self.types.resolve(tree, self.src)
        if tree and tree[0] == "ptr":
            tree = self.types.resolve(tree[1], self.src)
        if tree and tree[0] == "agg" and tree[1] in self.types.rec:
            t = self.types.member(tree[1], name)
            return self.types.resolve(t, self.src) if t else None
        return None


class Expr:
    """A tiny recursive-descent reader of C unary/postfix expressions over
    tokens, giving each a type tree (or None)."""

    def __init__(self, toks, scope):
        self.toks = toks
        self.scope = scope
        self.types = scope.types

    def tok(self, i):
        return self.toks[i][0] if i < len(self.toks) else None

    def match_paren(self, i):
        return match_paren(self.toks, i)

    def is_cast(self, i):
        """Tokens i.. are `( type )`: returns the index of `)`, or None."""
        if self.tok(i) != "(":
            return None
        j = self.match_paren(i)
        if j is None:
            return None
        words = [t for t, _ in self.toks[i + 1:j]]
        if not words or any(w not in TYPE_WORDS and w != "*" and not re.match(r"[A-Za-z_]\w*$", w)
                            for w in words):
            return None
        names = [w for w in words if w != "*"]
        if not names:
            return None
        if all(w in TYPE_WORDS for w in names) or "*" in words or words[0] in ("struct", "class") \
                or (len(names) == 1 and self.types.key(names[0]) and not self.scope.var(names[0])):
            return j
        return None

    def unary(self, i):
        """(type tree, next index) of the unary expression at token i."""
        t = self.tok(i)
        if t is None:
            return None, i
        if t == "&":
            tree, j = self.unary(i + 1)
            return (["ptr", tree] if tree else None), j
        if t == "*":
            tree, j = self.unary(i + 1)
            return self.deref(tree), j
        if t in ("-", "+", "~", "!"):
            tree, j = self.unary(i + 1)
            return None, j
        if t == "sizeof":
            if self.tok(i + 1) == "(":
                j = self.match_paren(i + 1)
                return ["int", 4], (j + 1 if j is not None else len(self.toks))
            _, j = self.unary(i + 1)
            return ["int", 4], j
        c = self.is_cast(i)
        if c is not None:
            text = " ".join(x for x, _ in self.toks[i + 1:c])
            tree = parse_type_name(text, self.types, self.scope.src)
            _, j = self.unary(c + 1)
            return tree, j
        return self.postfix(i)

    def deref(self, tree):
        tree = self.types.resolve(tree, self.scope.src)
        if tree and tree[0] == "ptr":
            return self.types.resolve(tree[1], self.scope.src)
        if tree and tree[0] == "arr":
            return self.elem(tree)
        return None

    def elem(self, tree):
        if len(tree[1]) > 1:
            return ["arr", tree[1][1:], tree[2]]
        return self.types.resolve(tree[2], self.scope.src)

    def postfix(self, i):
        t = self.tok(i)
        if t == "(":
            j = self.match_paren(i)
            if j is None:
                return None, len(self.toks)
            tree = Expr(self.toks[i + 1:j], self.scope).full()
            i = j + 1
        elif t is not None and re.match(r"[A-Za-z_]\w*$", t):
            tree = self.scope.var_this() if t == "this" else self.scope.var(t)
            i += 1
            # A qualified name (Class::member) or a call: unknown type.
            if self.tok(i) == "::":
                return None, i
        elif t is not None and re.match(r"\d", t):
            return ["int", 4], i + 1
        else:
            return None, i + 1
        while True:
            t = self.tok(i)
            if t == "->" or t == ".":
                name = self.tok(i + 1)
                tree = self.scope.member(tree, name) if tree else None
                i += 2
            elif t == "[":
                j = self.match_paren(i)
                tree = self.deref(tree) if tree else None
                i = (j if j is not None else len(self.toks)) + 1
            elif t == "(":
                j = self.match_paren(i)
                tree = None
                i = (j if j is not None else len(self.toks)) + 1
            elif t in ("++", "--"):
                i += 1
            else:
                return tree, i

    def full(self):
        """The type of a whole expression: of its first operand when the
        rest is pointer arithmetic, else None."""
        tree, j = self.unary(0)
        if j >= len(self.toks):
            return tree
        if tree and tree[0] == "ptr" and self.tok(j) in ("+", "-"):
            return tree
        return None


def expr_type(text, scope):
    toks = tokenize(text)
    if not toks:
        return None
    return Expr(toks, scope).full()


def pointee(types, tree, src):
    """The struct a pointer or array points at (through one more array)."""
    tree = types.resolve(tree, src)
    while tree and tree[0] in ("ptr", "arr"):
        tree = types.resolve(tree[1] if tree[0] == "ptr" else tree[2], src)
    return tree if tree and tree[0] == "agg" and tree[1] in types.rec else None


# ---------------------------------------------------------------------------
# Named constants (include/gba/*.h, include/memory.h)


def load_defines(paths):
    """Object-like #defines whose body evaluates to an integer: name -> value,
    plus function-like ones: name -> (params, body)."""
    raw, funcs = {}, {}
    define = re.compile(r"^\s*#\s*define\s+([A-Za-z_]\w*)(\(([^)]*)\))?\s*(.*?)\s*$")
    for path in paths:
        if not os.path.exists(path):
            continue
        text = mn.strip_comments(open(path, encoding="utf-8", errors="replace").read())
        text = text.replace("\\\n", " ")
        for line in text.split("\n"):
            m = define.match(line)
            if not m or not m.group(4):
                continue
            if m.group(2):
                funcs[m.group(1)] = ([p.strip() for p in m.group(3).split(",")], m.group(4))
            else:
                raw.setdefault(m.group(1), m.group(4))
    values = {}

    def ev(name, depth=0):
        if name in values:
            return values[name]
        if depth > 10 or name not in raw:
            return None
        body = raw[name]
        body = re.sub(r"\(\s*(?:const\s+|volatile\s+)*(?:vu8|vu16|vu32|u8|u16|u32|s8|s16|s32|void|"
                      r"uint16_t|uint8_t|uint32_t|uintptr_t)\s*\*?\s*\)", "", body)
        out = []
        for tok in re.findall(r"[A-Za-z_]\w*|0[xX][0-9a-fA-F]+[uUlL]*|\d+[uUlL]*|<<|>>|\S", body):
            if re.match(r"[A-Za-z_]", tok):
                v = ev(tok, depth + 1)
                if v is None:
                    return None
                out.append(str(v))
            elif re.match(r"\d", tok):
                out.append(str(mn.parse_value(tok)))
            elif tok in "()|&~+-*<<>>/" or tok in ("<<", ">>"):
                out.append(tok)
            else:
                return None
        try:
            v = eval(" ".join(out), {"__builtins__": {}}, {})  # digits and operators only
        except Exception:
            return None
        if not isinstance(v, int):
            return None
        values[name] = v
        return v

    for name in raw:
        ev(name)
    return values, funcs


# Register family -> define prefixes (io_reg.h) and field macros.
REG_FAMILIES = [
    (r"DISPCNT", ("DISPCNT_",)),
    (r"DISPSTAT", ("DISPSTAT_",)),
    (r"BG[0-3]?CNT", ("BGCNT_",)),
    (r"WININ", ("WININ_",)),
    (r"WINOUT", ("WINOUT_",)),
    (r"BLDCNT", ("BLDCNT_",)),
    (r"BLDALPHA", ("BLDALPHA_",)),
    (r"IE|IF", ("INTR_FLAG_",)),
    (r"KEYINPUT|KEYCNT", ("_BUTTON", "DPAD_", "KEY_", "KEYS_")),
    (r"SOUNDCNT_[LHX]|SOUND[0-9]?CNT\w*", ("SOUND_",)),
    # Only the control halves: the 32-bit DMAnCNT/TMnCNT also hold the
    # count or the reload value.
    (r"DMA[0-3]CNT_H", ("DMA_",)),
    (r"TM[0-3]CNT_H", ("TIMER_",)),
    (r"SIOCNT|SIOMLT\w*|RCNT", ("SIO_",)),
    (r"WAITCNT", ("WAITCNT_",)),
]
SHADOW_RE = re.compile(r"^g?(?i:(dispcnt|bldcnt|bldalpha|bg[0-3]cnt|winin|winout))(?![a-z])")


class Hardware:
    def __init__(self):
        self.values, self.funcs = load_defines(GBA_HEADERS + [MEMORY_HEADER])
        self.by_prefix = {}
        self.reg_addr = {}
        for name, v in self.values.items():
            if name.startswith("REG_ADDR_"):
                self.reg_addr.setdefault(v, name)

    def family(self, reg):
        reg = re.sub(r"^REG_(?:ADDR_)?", "", reg)
        for pat, prefixes in REG_FAMILIES:
            if re.fullmatch(pat, reg, re.I):
                return reg, prefixes
        return reg, None

    def flags(self, prefixes):
        key = tuple(prefixes)
        if key not in self.by_prefix:
            fl = []
            for name, v in self.values.items():
                if v and (any(name.startswith(p) for p in prefixes if not p.startswith("_"))
                          or any(name.endswith(p) for p in prefixes if p.startswith("_"))):
                    fl.append((name, v))
            # Single defines first, then wider masks, by value.
            fl.sort(key=lambda nv: (-bin(nv[1]).count("1"), nv[0]))
            self.by_prefix[key] = fl
        return self.by_prefix[key]

    def spell(self, reg, value):
        """(spelling, exact) of value in reg's family, or None."""
        _, prefixes = self.family(reg)
        if not prefixes or value == 0:
            return None
        if prefixes == ("BGCNT_",):
            return self.spell_bgcnt(value), True
        if prefixes == ("BLDALPHA_",):
            return f"BLDALPHA_BLEND({value & 0x1f}, {(value >> 8) & 0x1f})", value & ~0x1f1f == 0
        fl = self.flags(prefixes)
        exact = [n for n, v in fl if v == value]
        if exact:
            return exact[0], True
        left, parts = value, []
        # Multi-bit masks only when wholly inside the value, widest first.
        for name, v in sorted(fl, key=lambda nv: (-bin(nv[1]).count("1"), nv[1])):
            if v & left == v:
                parts.append((v, name))
                left &= ~v
        if not parts:
            return None
        parts.sort()
        text = " | ".join(n for _, n in parts)
        if left:
            text += f" | {left:#x}"
        return text, not left

    def spell_bgcnt(self, v):
        parts = []
        if v & 3:
            parts.append(f"BGCNT_PRIORITY({v & 3})")
        if (v >> 2) & 3:
            parts.append(f"BGCNT_CHARBASE({(v >> 2) & 3})")
        if v & 0x40:
            parts.append("BGCNT_MOSAIC")
        if v & 0x80:
            parts.append("BGCNT_256COLOR")
        if (v >> 8) & 0x1f:
            parts.append(f"BGCNT_SCREENBASE({(v >> 8) & 0x1f})")
        if v & 0x2000:
            parts.append("BGCNT_WRAP")
        size = {0x4000: "BGCNT_TXT512x256", 0x8000: "BGCNT_TXT256x512", 0xC000: "BGCNT_TXT512x512"}
        if v & 0xC000:
            parts.append(size[v & 0xC000])
        if v & 0x30:
            parts.append(f"{v & 0x30:#x}")
        return " | ".join(parts)


def hw_region(value):
    """(base name, offset) of a GBA address in PLTT/VRAM/OAM/I/O, or None."""
    for base, start, size in (("PLTT", 0x5000000, 0x400), ("VRAM", 0x6000000, 0x18000),
                              ("OAM", 0x7000000, 0x400), ("REG_BASE", 0x4000000, 0x400)):
        if start <= value < start + size:
            return base, value - start
    return None


def spell_address(hw, base, off):
    """BG_SCREEN_ADDR(n) + r, OBJ_PLTT + ..., REG_ADDR_X ... for base + off."""
    if base == "REG_BASE":
        name = hw.reg_addr.get(0x4000000 + off)
        return [name] if name else []
    if base in ("PLTT", "BG_PLTT"):
        region, rel = ("OBJ_PLTT", off - 0x200) if off >= 0x200 else ("BG_PLTT", off)
        if rel == 0:
            return [region]
        if rel % 0x20 == 0:
            n = rel // 0x20
            return [f"{region} + {n} * PALETTE_SIZE_16" + NEW] if n > 1 else [f"{region} + PALETTE_SIZE_16" + NEW]
        return [f"{region} + {rel:#x}"] if region == "OBJ_PLTT" else []
    if base == "OBJ_PLTT":
        if off and off % 0x20 == 0:
            n = off // 0x20
            return [f"OBJ_PLTT + {n} * PALETTE_SIZE_16" + NEW] if n > 1 else ["OBJ_PLTT + PALETTE_SIZE_16" + NEW]
        return []
    if base in ("VRAM", "BG_VRAM"):
        if off >= 0x10000:
            return ["OBJ_VRAM0" + (f" + {off - 0x10000:#x}" if off > 0x10000 else "")]
        if off and off % 0x4000 == 0:
            return [f"BG_CHAR_ADDR({off // 0x4000})"]
        if off >= 0x800:
            n, rest = divmod(off, 0x800)
            return [f"BG_SCREEN_ADDR({n})" + (f" + {rest:#x}" if rest else "")]
        return []
    if base == "OAM":
        return []
    return []


def named_size(region, value):
    """Named sizes of a copy into region (PLTT, BG_PLTT, OBJ_PLTT, VRAM, OAM)."""
    out = []
    if region in ("PLTT", "BG_PLTT", "OBJ_PLTT"):
        if value == 0x400 and region == "PLTT":
            out.append("PLTT_SIZE")
        elif value == 0x200:
            out.append("OBJ_PLTT_SIZE" if region == "OBJ_PLTT" else "BG_PLTT_SIZE")
        elif value % 0x20 == 0 and value < 0x400:
            n = value // 0x20
            out.append(("PALETTE_SIZE_16" if n == 1 else f"{n} * PALETTE_SIZE_16") + NEW)
    elif region in ("VRAM", "BG_VRAM", "OBJ_VRAM0", "OBJ_VRAM1"):
        if value == 0x18000:
            out.append("VRAM_SIZE")
        elif value == 0x10000:
            out.append("BG_VRAM_SIZE")
        elif value == 0x8000 and region == "OBJ_VRAM0":
            out.append("OBJ_VRAM0_SIZE")
        elif value == 0x4000 and region.startswith("OBJ"):
            out.append("OBJ_VRAM1_SIZE")
        elif value % 0x4000 == 0:
            n = value // 0x4000
            out.append(("BG_CHAR_SIZE" if n == 1 else f"{n} * BG_CHAR_SIZE") + NEW)
        elif value % 0x800 == 0:
            n = value // 0x800
            out.append(("BG_SCREEN_SIZE" if n == 1 else f"{n} * BG_SCREEN_SIZE") + NEW)
        elif value % 0x20 == 0:
            n = value // 0x20
            out.append(f"{n} * TILE_SIZE_4BPP" if n > 1 else "TILE_SIZE_4BPP")
    elif region == "OAM":
        if value == 0x400:
            out.append("OAM_SIZE")
    return out


def region_of(text, hw):
    """The hardware region a destination expression points into."""
    t = text.replace(" ", "")
    for name in ("OBJ_PLTT", "BG_PLTT", "PLTT", "OBJ_VRAM0", "OBJ_VRAM1", "BG_VRAM", "VRAM", "OAM"):
        if re.search(r"\b" + name + r"\b", t):
            if name == "PLTT":
                m = re.search(r"\bPLTT\+(" + mn.LIT + r")", t)
                if m and mn.parse_value(m.group(1)) >= 0x200:
                    return "OBJ_PLTT"
            if name == "VRAM":
                m = re.search(r"\bVRAM\+(" + mn.LIT + r")", t)
                if m and mn.parse_value(m.group(1)) >= 0x10000:
                    return "OBJ_VRAM0"
            return name
    if re.search(r"\bBG_(?:SCREEN|CHAR|TILE)_ADDR\b", t):
        return "BG_VRAM"
    for m in re.finditer(mn.LIT, t):
        r = hw_region(mn.parse_value(m.group(0)))
        if r and r[0] != "REG_BASE":
            return "OBJ_VRAM0" if r[0] == "VRAM" and r[1] >= 0x10000 else (
                "OBJ_PLTT" if r[0] == "PLTT" and r[1] >= 0x200 else r[0])
    return None


# ---------------------------------------------------------------------------
# Scanning

LIT = mn.LIT
LIT_FULL = re.compile(r"^\s*(" + LIT + r")\s*$")
ALLOC_CALLS = [  # (name regex, size argument, heap-flag argument or None)
    (r"mem_alloc", 0, 1),
    (r"IwramAlloc", 0, None),
    (r"AllocIwram", 0, None),
    (r"OperatorNew", 0, None),
    (r"OperatorNewArray", 0, None),
    (r"operator\s+new(?:\s*\[\s*\])?", 0, None),
]
# (name regex, length argument, destination argument, source argument, unit)
COPY_CALLS = [
    (r"Dma(?:Copy|Fill)(?:16|32)(?:Defvars)?", 3, 2, 1, 1),
    (r"Dma(?:Copy|Fill)Large(?:16|32)|DmaFill(?:16|32)Large", 3, 2, 1, 1),
    (r"DmaClear(?:16|32)", 2, 1, None, 1),
    (r"DmaClearLarge(?:16|32)", 2, 1, None, 1),
    (r"Dma3(?:Copy|Fill)Large(?:16|32)_", 2, 1, 0, 1),
    (r"MemCopy32", 2, 0, 1, 1),
    (r"memcpy|memmove", 2, 0, 1, 1),
    (r"memset", 2, 0, None, 1),
    (r"CpuSet", 2, 1, 0, "cpuset"),
    (r"CpuFastSet", 2, 1, 0, "cpufastset"),
]
CPU_SET_32BIT = 0x04000000
CPU_SET_SRC_FIXED = 0x01000000


class Hit:
    __slots__ = ("file", "line", "col", "category", "literal", "value", "expr", "suggestions", "new_defines",
                 "confidence", "context", "source")

    def __init__(self, **kw):
        for k in self.__slots__:
            setattr(self, k, kw.get(k))

    def as_dict(self):
        return {k: getattr(self, k) for k in self.__slots__}


class FileScan:
    def __init__(self, rel, types, hw):
        self.rel = rel
        self.types = types
        self.hw = hw
        self.raw = open(os.path.join(ROOT, rel), encoding="utf-8", errors="replace").read()
        self.text = mn.blank_directives(mn.strip_comments(self.raw))
        self.lines = self.raw.split("\n")
        self.starts = [0]
        for ln in self.lines:
            self.starts.append(self.starts[-1] + len(ln) + 1)
        self.hits = {}
        self._fnames = {}

    def line_col(self, off):
        lo, hi = 0, len(self.starts) - 1
        while lo < hi - 1:
            mid = (lo + hi) // 2
            if self.starts[mid] <= off:
                lo = mid
            else:
                hi = mid
        return lo + 1, off - self.starts[lo] + 1

    def scope(self, off):
        return Scope(self.types, self.rel, self.line_col(off)[0])

    def add(self, category, off, lit, suggestions, confidence, context="", expr=None):
        lit = lit.strip()
        if not suggestions:
            return
        key = (off, category)
        if any(k[0] == off for k in self.hits):
            # One hit per literal: the more confident one wins.
            old = next(h for k, h in self.hits.items() if k[0] == off)
            if CONFIDENCES.index(old.confidence) <= CONFIDENCES.index(confidence):
                return
            del self.hits[(off, old.category)]
        line, col = self.line_col(off)
        new = sorted({n for x in suggestions if x.endswith(NEW) for n in NEW_DEFINES if n in x})
        suggestions = [x[: -len(NEW)] if x.endswith(NEW) else x for x in suggestions]
        self.hits[key] = Hit(new_defines=[f"{n} {NEW_DEFINES[n][0]:#x}" for n in new],file=self.rel, line=line, col=col, category=category, literal=lit,
                             value=mn.parse_value(lit), expr=expr or lit, suggestions=suggestions,
                             confidence=confidence,
                             context=context, source=self.lines[line - 1].strip())

    def statement(self, off):
        """The text span of the statement around off (from the previous ; { }
        to the next ;)."""
        s = max(self.text.rfind(c, 0, off) for c in ";{}") + 1
        e = self.text.find(";", off)
        e = len(self.text) if e < 0 else e
        return s, e

    def calls(self, name):
        pat = re.compile(r"(?<![\w.>:])(?:" + name + r")\s*\(")
        for m in pat.finditer(self.text):
            before = self.text[max(0, m.start() - 40):m.start()]
            if re.search(r"\b(?:void|s32|u32|extern|static|inline)\s*\**\s*$", before):
                continue
            args = mn.split_args(self.text, m.end() - 1)
            if args:
                yield m, args

    # -- value-only fallbacks

    def struct_candidates(self, value, scale=""):
        keys = self.types.struct_sizes().get(value, [])
        if not keys:
            return []
        names = sorted({self.types.spell(["agg", k, self.types.rec[k]["name"]], self.rel) for k in keys})
        out = [f"{scale}sizeof({n})" for n in names[:LOW_CANDIDATES]]
        if len(names) > LOW_CANDIDATES:
            out.append(f"... {len(names) - LOW_CANDIDATES} more")
        return out

    def size_suggestions(self, value, tree, what):
        """sizeof(T) or n * sizeof(T) for a value and a type (the thing
        allocated, copied or indexed)."""
        tree = self.types.resolve(tree, self.rel)
        if not tree:
            return []
        out = []
        size = self.types.size(tree, self.rel)
        spelled = None
        if tree[0] == "arr" and what:
            spelled = what
        elif tree[0] in ("agg", "ptr", "int"):
            spelled = self.types.spell(tree, self.rel)
        if size and size == value and spelled:
            out.append(f"sizeof({spelled})")
        if tree[0] == "arr":
            el = self.types.resolve(tree[2], self.rel)
            es = self.types.size(el, self.rel)
            if es and es > 1 and value % es == 0 and value != size:
                out.append(f"{value // es} * sizeof({self.types.spell(el, self.rel)})")
        elif size and size > 1 and value % size == 0 and value != size and tree[0] in ("agg", "ptr"):
            out.append(f"{value // size} * sizeof({spelled})")
        return out

    # -- categories

    def scan_alloc(self):
        for name, si, fi in ALLOC_CALLS:
            for m, args in self.calls(name):
                if si < len(args):
                    off, arg = args[si]
                    self.alloc_size(m, off, arg)
                if fi is not None and fi < len(args):
                    off, arg = args[fi]
                    lm = LIT_FULL.match(arg)
                    if lm:
                        v = mn.parse_value(lm.group(1))
                        names = [n for n, x in self.hw.values.items() if n.startswith("MEM_HEAP_") and x == v]
                        self.add("alloc", off + lm.start(1), lm.group(1), sorted(names), "high", "heap flag")
        # Sizes quoted in comments: `OperatorNew(0x28)` for code that is now `new X`.
        for m in re.finditer(r"\bOperatorNew(?:Array)?\((" + LIT + r")\)", self.raw):
            if self.text[m.start():m.end()] == self.raw[m.start():m.end()]:
                continue  # code, not a comment
            v = mn.parse_value(m.group(1))
            line = self.line_col(m.start())[0]
            near = " ".join(self.lines[max(0, line - 2):line + 1])
            # `new InputCtrl`, `InitInputCtrl(OperatorNew(0x28))`: the class's
            # name, or a word ending in it.
            named = [k for k in self.types.struct_sizes().get(v, []) if re.search(
                r"(?<![a-z0-9_])\w*" + re.escape(self.types.rec[k]["name"]) + r"\b", near)]
            if named:
                sugg = [f"sizeof({self.types.rec[k]['name']})" for k in named]
                self.add("alloc", m.start(1), m.group(1), sugg, "high", "comment: the code is `new X` now")
            else:
                self.add("alloc", m.start(1), m.group(1), self.struct_candidates(v), "low",
                         "comment: the code is `new X` now")

    def alloc_size(self, m, off, arg):
        lm = LIT_FULL.match(arg)
        scale = re.match(r"^\s*(?P<a>.+?)\s*\*\s*(?P<v>" + LIT + r")\s*$|^\s*(?P<w>" + LIT + r")\s*\*\s*(?P<b>.+?)\s*$",
                         arg)
        if lm:
            lit, loff = lm.group(1), off + lm.start(1)
            count = None
        elif scale and not LIT_FULL.match(scale.group("a") or scale.group("b") or "0"):
            g = "v" if scale.group("v") else "w"
            lit, loff = scale.group(g), off + scale.start(g)
            count = (scale.group("a") or scale.group("b")).strip()
        else:
            return
        v = mn.parse_value(lit)
        # The type: the cast in front of the call, else what it's assigned to.
        before = self.text[max(0, m.start() - 200):m.start()]
        cast = re.search(r"\(\s*((?:const\s+|struct\s+|class\s+)*\w+(?:\s*\*)+)\s*\)\s*$", before)
        tree = None
        ctx = ""
        scope = self.scope(m.start())
        if cast:
            t = parse_type_name(cast.group(1), self.types, self.rel)
            if t and t[0] == "ptr":
                tree, ctx = t[1], f"cast to {cast.group(1).strip()}"
        if tree is None:
            lhs = re.search(r"([\w>.\-\[\]]+)\s*=\s*(?:\([^()]*\)\s*)?$", before)
            if lhs:
                t = expr_type(lhs.group(1), scope)
                if t and t[0] == "ptr":
                    tree, ctx = t[1], f"assigned to {lhs.group(1)}"
        tree = self.types.resolve(tree, self.rel)
        if tree and tree[0] in ("agg", "ptr") or (tree and tree[0] == "int" and tree[1] > 1):
            es = self.types.size(tree, self.rel)
            spelled = self.types.spell(tree, self.rel)
            if es and v == es:
                self.add("alloc", loff, lit, [f"sizeof({spelled})"], "high", ctx)
                return
            if es and es > 1 and v % es == 0 and count is None:
                self.add("alloc", loff, lit, [f"{v // es} * sizeof({spelled})"], "high", ctx)
                return
        if count is None:
            self.add("alloc", loff, lit, self.struct_candidates(v), "low", "size only")
        else:
            self.add("alloc", loff, lit, self.struct_candidates(v), "low", f"{count} elements")

    def scan_copy(self):
        for name, li, di, si, unit in COPY_CALLS:
            for m, args in self.calls(name):
                if li >= len(args):
                    continue
                off, arg = args[li]
                dest = args[di][1] if di is not None and di < len(args) else ""
                src = args[si][1] if si is not None and si < len(args) else ""
                if unit in ("cpuset", "cpufastset"):
                    self.cpuset(m, off, arg, dest, src, unit)
                    continue
                lm = LIT_FULL.match(arg)
                if lm:
                    self.copy_length(m, off + lm.start(1), lm.group(1), mn.parse_value(lm.group(1)), dest, src,
                                     1, "")
                    continue
                sm = re.match(r"^\s*(?P<a>[^*]+?)\s*\*\s*(?P<v>" + LIT + r")\s*$|^\s*(?P<w>" + LIT
                              + r")\s*\*\s*(?P<b>[^*]+?)\s*$", arg)
                if sm:
                    g = "v" if sm.group("v") else "w"
                    count = (sm.group("a") or sm.group("b")).strip()
                    if not LIT_FULL.match(count):
                        self.copy_length(m, off + sm.start(g), sm.group(g), mn.parse_value(sm.group(g)), dest, src,
                                         1, count)

    def cpuset(self, m, off, arg, dest, src, kind):
        terms = []
        depth, start = 0, 0
        for i, c in enumerate(arg + "|"):
            if c in "([":
                depth += 1
            elif c in ")]":
                depth -= 1
            elif c == "|" and depth == 0:
                terms.append((start, arg[start:i]))
                start = i + 1
        wide = kind == "cpufastset" or any("CPU_SET_32BIT" in t for _, t in terms)
        for toff, t in terms:
            lm = LIT_FULL.match(t)
            if not lm:
                continue
            v = mn.parse_value(lm.group(1))
            flags = v & ~0x1FFFFF
            count = v & 0x1FFFFF
            w = wide or bool(flags & CPU_SET_32BIT)
            unit = 4 if w else 2
            loff = off + toff + lm.start(1)
            sugg = []
            if flags and kind == "cpuset":
                names = []
                if flags & CPU_SET_32BIT:
                    names.append("CPU_SET_32BIT")
                if flags & CPU_SET_SRC_FIXED:
                    names.append("CPU_SET_SRC_FIXED")
                if flags & ~(CPU_SET_32BIT | CPU_SET_SRC_FIXED) == 0:
                    sugg.append(" | ".join(names) + (f" | {count:#x}" if count else ""))
            if count:
                sizes = self.copy_suggestions(m, count * unit, dest, src, "")
                unit_t = "u32" if w else "u16"
                for s, conf, ctx in sizes:
                    q = s if re.fullmatch(r"sizeof\([^()]*(?:\([^()]*\))?[^()]*\)", s) else f"({s})"
                    self.add("copy", loff, lm.group(1), sugg + [f"{q} / sizeof({unit_t})"], conf,
                             f"{kind} count in {unit_t}s; " + ctx)
                    break
                else:
                    if sugg:
                        self.add("copy", loff, lm.group(1), sugg, "high", f"{kind} control word")
            elif sugg:
                self.add("copy", loff, lm.group(1), sugg, "high", f"{kind} control word")

    def copy_length(self, m, loff, lit, value, dest, src, unit, count):
        sizes = self.copy_suggestions(m, value, dest, src, count)
        if sizes:
            s, conf, ctx = sizes[0]
            more = [x[0] for x in sizes[1:] if x[1] == conf]
            self.add("copy", loff, lit, [s] + more, conf, ctx)

    def copy_suggestions(self, m, value, dest, src, count):
        """[(suggestion, confidence, context)] for a copy length, best first."""
        out = []
        scope = self.scope(m.start())
        scalar = False
        for role, text in (("destination", dest), ("source", src)):
            text = text.strip()
            if not text:
                continue
            tree = expr_type(text, scope)
            tree = self.types.resolve(tree, self.rel)
            if not tree:
                continue
            what = None
            t = tree
            if text.startswith("&"):
                what = text[1:].strip()
                t = self.types.resolve(tree[1], self.rel) if tree[0] == "ptr" else None
            elif tree[0] == "arr":
                what = text
            elif tree[0] == "ptr":
                t = self.types.resolve(tree[1], self.rel)
            if t and t[0] in ("int", "void") and not what:
                scalar = True  # a buffer of bytes/halfwords/words: no size in the type
                continue
            if not t or t[0] in ("void", "?"):
                continue
            if count:
                es = self.types.size(t, self.rel)
                if es == value and es > 1:
                    out.append((f"sizeof({self.types.spell(t, self.rel)})", "high", f"{role} {text}"))
                continue
            for s in self.size_suggestions(value, t, what):
                out.append((s, "high", f"{role} {text}"))
        region = region_of(dest, self.hw) if dest else None
        if region and not count:
            for s in named_size(region, value):
                out.append((s, "high", f"destination in {region}"))
        if not out and not count and value % 0x20 == 0 and value <= 0x200 \
                and re.search(r"(?i)pal|pltt", dest + " " + src):
            n = value // 0x20
            out.append((("PALETTE_SIZE_16" if n == 1 else f"{n} * PALETTE_SIZE_16") + NEW, "medium",
                        "a palette, by its name"))
        if not out and not count and value % 0x20 == 0 and re.search(r"(?i)tile", dest + " " + src):
            n = value // 0x20
            out.append(("TILE_SIZE_4BPP" if n == 1 else f"{n} * TILE_SIZE_4BPP", "medium", "tiles, by the name"))
        if not out and not count and value >= 8:
            near = {}
            for name, tree in self.function_names(m.start()):
                for cand in self.indexable(tree):
                    if self.types.size(cand, self.rel) == value:
                        near.setdefault(self.types.spell(cand, self.rel), name)
            for sp in sorted(near):
                out.append((f"sizeof({sp})", "medium", "the function uses " + near[sp]))
        if not out and not count and not scalar:
            for s in self.struct_candidates(value):
                out.append((s, "low", "size only"))
        return out

    def scan_stride(self):
        pat = re.compile(r"(?P<l>[\w\]\)])\s*\*\s*(?P<v>" + LIT + r")\b(?!\s*[\w(])|(?<![\w)\]])(?P<w>" + LIT
                         + r")\s*\*\s*(?=[\w(])")
        sizes = self.types.struct_sizes()
        for m in pat.finditer(self.text):
            g = "v" if m.group("v") else "w"
            lit = m.group(g)
            v = mn.parse_value(lit)
            if v < 4 or v not in sizes:
                continue
            if g == "v":
                # `x * K`: make sure x isn't a type (`(u8 *)0x...` is a cast).
                pre = self.text[max(0, m.start("l") - 30):m.end("l")]
                if re.search(r"\b(?:" + "|".join(TYPE_WORDS) + r")\s*$", pre):
                    continue
            # A copy/alloc argument found by scan_copy/scan_alloc keeps
            # that hit (add() keeps one per literal).
            s, e = self.statement(m.start(g))
            stmt = self.text[s:e]
            scope = self.scope(m.start(g))
            found = self.stride_types(stmt, scope, v)
            if found:
                self.add("stride", m.start(g), lit, [f"sizeof({x})" for x in found[0]], "high", found[1])
                continue
            if v < 8:
                continue  # too many 4-byte structs to tell from the value
            near = {}
            for name, tree in self.function_names(m.start(g)):
                for cand in self.indexable(tree):
                    if self.types.size(cand, self.rel) == v:
                        near.setdefault(self.types.spell(cand, self.rel), name)
            if near:
                self.add("stride", m.start(g), lit, [f"sizeof({x})" for x in sorted(near)], "medium",
                         "the function uses " + ", ".join(sorted(set(near.values()))))
                continue
            # Value only: a hex literal (a decimal `* 100` or `* 60` is
            # arithmetic far more often than a stride).
            cands = self.struct_candidates(v) if lit[:2].lower() == "0x" else []
            if cands and not cands[-1].startswith("..."):
                self.add("stride", m.start(g), lit, cands, "low", "size only")

    def function_names(self, off):
        """[(name, type)] of every variable, parameter or global the function
        around off names (and `this`)."""
        line = self.line_col(off)[0]
        funcs = self.types.files.get(self.rel, {}).get("funcs", [])
        idx = None
        for k, f in enumerate(funcs):
            if f["line"] <= line:
                idx = k
        if idx is None:
            return []
        if idx not in self._fnames:
            start = self.starts[funcs[idx]["line"] - 1]
            end = self.starts[funcs[idx + 1]["line"] - 1] if idx + 1 < len(funcs) else len(self.text)
            scope = Scope(self.types, self.rel, funcs[idx]["line"])
            out = []
            for name in sorted(set(re.findall(r"\b[A-Za-z_]\w*\b", self.text[start:end]))):
                if name in TYPE_WORDS:
                    continue
                tree = scope.var_this() if name == "this" else scope.var(name)
                if tree:
                    out.append((name, tree))
            self._fnames[idx] = out
        return self._fnames[idx]

    def stride_types(self, stmt, scope, v):
        """([type spellings], context) of a struct of size v the statement
        indexes: a pointer/array operand's element, or an array member of
        a struct an operand points at."""
        toks = tokenize(stmt)
        ex = Expr(toks, scope)
        found, ctx = [], []
        i = 0
        while i < len(toks):
            t = toks[i][0]
            if t == "(" and ex.is_cast(i) is not None:
                c = ex.is_cast(i)
                tree = parse_type_name(" ".join(x for x, _ in toks[i + 1:c]), self.types, self.rel)
                text = " ".join(x for x, _ in toks[i:c + 1])
                i = c + 1
            elif re.match(r"[A-Za-z_]\w*$", t) and t not in TYPE_WORDS \
                    and not (i and toks[i - 1][0] in ("->", ".")):
                # Every operand, including the ones inside a call's
                # arguments (`obj->spawn((T *)((u8 *)table + i * K))`).
                tree, j = ex.postfix(i)
                text = re.sub(r" ?(->|\.) ?", r"\1", " ".join(x for x, _ in toks[i:j]))
                if len(text) > 40:
                    text = t
                i += 1
            else:
                i += 1
                continue
            for cand in self.indexable(tree):
                if self.types.size(cand, self.rel) == v:
                    sp = self.types.spell(cand, self.rel)
                    if sp not in found:
                        found.append(sp)
                        ctx.append(text)
        return (found, "indexes " + ", ".join(ctx)) if found else None

    def indexable(self, tree):
        """Element types reachable from an operand: what a pointer or array
        points at, and the elements of that struct's array members."""
        tree = self.types.resolve(tree, self.rel)
        out = []
        if not tree:
            return out
        if tree[0] in ("ptr", "arr"):
            el = self.types.resolve(tree[1] if tree[0] == "ptr" else tree[2], self.rel)
            if el and el[0] == "agg" and el[1] in self.types.rec:
                out.append(el)
                tree = el
        if tree[0] == "agg" and tree[1] in self.types.rec:
            for _, mt in self.types.members(tree[1]):
                mt = self.types.resolve(mt, self.rel)
                if mt and mt[0] == "arr":
                    el = self.types.resolve(mt[2], self.rel)
                    if el and el[0] == "agg" and el[1] in self.types.rec:
                        out.append(el)
        return out

    def scan_offset(self):
        toks_cache = {}
        for pat in (BYTE_PTR_CASTS, INT_CASTS):
            for m in pat.finditer(self.text):
                s, e = self.statement(m.start())
                if s not in toks_cache:
                    toks_cache[s] = tokenize(self.text[s:e])
                toks = toks_cache[s]
                rel = m.start() - s
                i = next((k for k, (_, o) in enumerate(toks) if o == rel), None)
                if i is None:
                    continue
                scope = self.scope(m.start())
                ex = Expr(toks, scope)
                c = ex.is_cast(i)
                if c is None:
                    continue
                base, j = ex.unary(c + 1)
                p = pointee(self.types, base, self.rel) if base and base[0] == "ptr" else None
                if not p:
                    continue
                operand = " ".join(x for x, _ in toks[c + 1:j])
                # `((u8 *)p)[K]`
                if i and toks[i - 1][0] == "(" and j + 3 < len(toks) and toks[j][0] == ")" \
                        and toks[j + 1][0] == "[" and LIT_FULL.match(toks[j + 2][0]) and toks[j + 3][0] == "]":
                    self._cast_rel = toks[i - 1][1]
                    self.offset_at(s, toks[j + 2], [], p, operand, None)
                    continue
                self._cast_rel = rel
                lits, strides = self.additive_terms(toks, j)
                if len(lits) == 1:
                    self.offset_at(s, lits[0], strides, p, operand, self.outer_cast(toks, i))

    def outer_cast(self, toks, i):
        """The type of `(T *)(` in front of the cast at token i, if any."""
        if i >= 2 and toks[i - 1][0] == "(" and toks[i - 2][0] == ")":
            k = i - 2
            depth = 0
            for b in range(k, -1, -1):
                if toks[b][0] == ")":
                    depth += 1
                elif toks[b][0] == "(":
                    depth -= 1
                    if depth == 0:
                        words = [x for x, _ in toks[b + 1:k]]
                        if words and words[-1] == "*":
                            return parse_type_name(" ".join(words), self.types, self.rel)
                        return None
        return None

    def additive_terms(self, toks, j):
        """The `+ K` literals and `+ i * S` strides (S None for any other
        term) after token j, up to the end of the sum."""
        lits, strides = [], []
        k = j
        while k + 1 < len(toks) and toks[k][0] == "+":
            tok = toks[k + 1][0]
            nxt = toks[k + 2][0] if k + 2 < len(toks) else None
            if LIT_FULL.match(tok) and nxt not in ("*", "/", "<<", ">>", "%"):
                lits.append(toks[k + 1])
                k += 2
                continue
            if LIT_FULL.match(tok) and nxt == "*" and k + 3 < len(toks):
                strides.append((mn.parse_value(tok), toks[k + 3][0]))
                k += 4
                continue
            if nxt == "*" and k + 3 < len(toks) and LIT_FULL.match(toks[k + 3][0]):
                strides.append((mn.parse_value(toks[k + 3][0]), tok))
                k += 4
                continue
            if tok == "(":
                # `p + (i * S + K)`: the group's own terms, if it is a sum.
                close = match_paren(toks, k + 1)
                if close is not None:
                    inner = [("+", -1)] + toks[k + 2:close]
                    il, ist = self.additive_terms(inner, 0)
                    used = 2 * len(il) + sum(2 if s is None else 4 for s, _ in ist)
                    if used == len(inner) and all(s is not None for s, _ in ist):
                        lits += il
                        strides += ist
                        k = close + 1
                        continue
            depth, k2 = 0, k + 1
            while k2 < len(toks):
                t = toks[k2][0]
                if t in "([":
                    depth += 1
                elif t in ")]":
                    if depth == 0:
                        break
                    depth -= 1
                elif depth == 0 and t in ("+", "-", ";", ",", "=", "==", "!=", "<", ">", "<=", ">=", "&&", "||",
                                          "?", ":"):
                    break
                k2 += 1
            strides.append((None, " ".join(x for x, _ in toks[k + 1:k2])))
            k = k2
        return lits, strides

    def offset_at(self, s, lit_tok, strides, p, operand, outer):
        """A hit for the literal lit_tok, a byte offset into the struct p
        (plus the strides), if a member starts there."""
        lit, lrel = lit_tok
        rec = self.types.rec[p[1]]
        K = mn.parse_value(lit)
        if K <= 0 or K >= rec["size"]:
            return
        outer = self.types.resolve(outer, self.rel) if outer else None
        width = None
        if outer and outer[0] == "ptr":
            width = self.types.size(outer[1], self.rel)
        known = [x for x in strides if x[0] is not None]
        arrow = re.match(r"^[\w>.\-]+$", operand.replace(" ", "")) is not None
        op = operand.replace(" ", "")
        sugg, sizes = [], []
        if not strides:
            for path, size, pad, _ in self.types.paths_at(p[1], K, width)[:2]:
                sugg.append(f"&{op}->{path}" if arrow else f"offsetof({rec['name']}, {path})")
                sizes.append(size)
        elif len(known) == 1 and len(strides) == 1:
            S, idx = known[0]
            if S == rec["size"]:
                # `(u8 *)table + i * sizeof(*table) + K`: a member of table[i].
                for path, size, pad, _ in self.types.paths_at(p[1], K, width)[:2]:
                    sugg.append(f"&{op}[{idx}].{path}" if arrow else f"offsetof({rec['name']}, {path})")
                    sizes.append(size)
            for path, size, pad, _ in self.types.paths_at(p[1], K, None) if not sugg else ():
                arr = re.match(r"^(.*)\[(\d+)\](.*)$", path)
                if not arr:
                    continue
                head = arr.group(1)
                mt = self.member_path_type(p[1], head)
                if not mt or mt[0] != "arr" or self.types.size(mt[2], self.rel) != S:
                    continue
                sugg.append(f"&{op}->{head}[{idx}{' + ' + arr.group(2) if arr.group(2) != '0' else ''}]"
                            + arr.group(3))
                sizes.append(size)
                break
        if not sugg:
            return
        conf, ctx = "high", f"{op} is {self.types.spell(['ptr', p], self.rel)}"
        if outer and outer[0] == "ptr":
            target = self.types.resolve(outer[1], self.rel)
            if target and target[0] == "agg":
                if target[1] != p[1] and sizes and sizes[0] != self.types.size(target, self.rel):
                    conf, ctx = "low", ctx + f", read as {self.types.spell(outer, self.rel)}"
            elif width and sizes and sizes[0] != width:
                conf, ctx = "medium", ctx + f", read as {self.types.spell(outer, self.rel)}"
        end = s + lrel + len(lit)
        close = re.match(r"\s*\]", self.text[end:])
        if close:
            end += close.end()
        start = s + self._cast_rel
        while self.text[start:end].count("(") > self.text[start:end].count(")"):
            close = re.match(r"\s*\)", self.text[end:])
            if not close:
                break
            end += close.end()
        self.add("offset", s + lrel, lit, sugg, conf, ctx, expr=re.sub(r"\s+", " ", self.text[s + self._cast_rel:end]))

    def member_path_type(self, key, path):
        tree = ["agg", key, None]
        for part in path.split("."):
            m = re.match(r"^(\w+)((?:\[\d+\])*)$", part)
            if not m or tree[0] != "agg" or tree[1] not in self.types.rec:
                return None
            tree = self.types.resolve(self.types.member(tree[1], m.group(1)), self.rel)
            if not tree:
                return None
            for _ in re.findall(r"\[", m.group(2)):
                tree = self.types.resolve(tree[2], self.rel) if tree[0] == "arr" else None
                if not tree:
                    return None
        return tree

    def scan_hw(self):
        reg = r"(?P<r>\bREG_\w+|\*\s*\(\s*vu(?:8|16|32)\s*\*\s*\)\s*REG_ADDR_\w+|(?:[\w\]\)]+(?:->|\.))?(?P<s>\w+))"
        pat = re.compile(reg + r"\s*(?P<op>\|=|&=|==|!=|=(?!=)|&(?!&))\s*(?P<neg>~\s*)?(?P<v>" + LIT + r")\b(?!\s*[<*/+-])")
        rpat = re.compile(r"\b(?P<v>" + LIT + r")\s*(?P<op>==|!=)\s*(?P<r>\bREG_\w+)")
        for m in list(pat.finditer(self.text)) + list(rpat.finditer(self.text)):
            r = m.group("r")
            shadow = False
            name = re.sub(r"^\*\s*\(\s*vu\d+\s*\*\s*\)\s*", "", r)
            if not name.startswith("REG_"):
                sname = m.groupdict().get("s") or ""
                sm = SHADOW_RE.match(sname)
                if not sm:
                    continue
                name, shadow = sm.group(1).upper(), True
            v = mn.parse_value(m.group("v"))
            op = m.group("op")
            neg = m.groupdict().get("neg")
            mask = 0xFFFFFFFF if name.endswith(("BG2X", "BG2Y", "BG3X", "BG3Y")) else 0xFFFF
            if op == "&=" and not neg and v & mask != v:
                continue
            if op == "&=" and not neg:
                inv = ~v & mask
                sp = self.hw.spell(name, inv)
                if sp:
                    self.add("hw", m.start("v"), m.group("v"), [f"~({sp[0]})" if "|" in sp[0] else f"~{sp[0]}"],
                             "medium" if shadow or not sp[1] else "high", f"{name} &= ~bits")
                continue
            sp = self.hw.spell(name, v)
            if sp:
                self.add("hw", m.start("v"), m.group("v"), [sp[0]],
                         "medium" if shadow else ("high" if sp[1] else "low"),
                         f"{name} {op}" + (" (shadow)" if shadow else ""))

    def scan_hwaddr(self):
        pat = re.compile(r"\b(?P<b>VRAM|BG_VRAM|PLTT|BG_PLTT|OBJ_PLTT|OAM|REG_BASE)\s*\+\s*(?P<v>" + LIT + r")\b(?!\s*[*/<])")
        for m in pat.finditer(self.text):
            v = mn.parse_value(m.group("v"))
            sugg = spell_address(self.hw, m.group("b"), v)
            if sugg:
                self.add("hwaddr", m.start("v"), m.group("v"), sugg, "high", m.group("b"),
                         expr=f"{m.group('b')} + {m.group('v')}")
        for m in re.finditer(r"(?<![\w+])(?P<v>0[xX]0?[4-7]0[0-9a-fA-F]{5})[uUlL]*\b", self.text):
            v = mn.parse_value(m.group("v"))
            r = hw_region(v)
            if not r:
                continue
            base, off = r
            if base == "REG_BASE":
                sugg = spell_address(self.hw, base, off)
            else:
                sugg = spell_address(self.hw, base, off) or ([base] if off == 0 else [f"{base} + {off:#x}"])
            if sugg:
                self.add("hwaddr", m.start("v"), m.group("v"), sugg, "high", "raw address")

    def scan(self):
        self.scan_alloc()
        self.scan_copy()
        self.scan_hw()
        self.scan_hwaddr()
        self.scan_offset()
        self.scan_stride()
        return sorted(self.hits.values(), key=lambda h: (h.line, h.col))


def source_files(paths):
    out = []
    for dirpath, dirnames, filenames in os.walk(os.path.join(ROOT, "src")):
        dirnames.sort()
        rel_dir = os.path.relpath(dirpath, ROOT)
        if rel_dir == os.path.join("src", "data") or rel_dir.startswith(os.path.join("src", "data", "")):
            dirnames[:] = []
            continue
        for f in sorted(filenames):
            if f.endswith((".c", ".cpp", ".h", ".hpp")):
                rel = os.path.join(rel_dir, f)
                if not paths or any(rel.startswith(p.rstrip("/")) for p in paths):
                    out.append(rel)
    return out


def directory(rel):
    return os.path.dirname(rel)


# ---------------------------------------------------------------------------
# Output


def counts_table(hits, w):
    cats = [c for c in CATEGORIES if any(h.category == c for h in hits)]
    w("| Directory | " + " | ".join(f"`{c}`" for c in cats) + " | Total | high |\n")
    w("|---|" + "---:|" * (len(cats) + 2) + "\n")
    grid = collections.Counter((directory(h.file), h.category) for h in hits)
    highs = collections.Counter(directory(h.file) for h in hits if h.confidence == "high")
    for d in sorted({directory(h.file) for h in hits}):
        row = [grid[(d, c)] for c in cats]
        w(f"| {d} | " + " | ".join(str(x) if x else "" for x in row) + f" | {sum(row)} | {highs[d]} |\n")
    tot = [sum(1 for h in hits if h.category == c) for c in cats]
    w("| **Total** | " + " | ".join(f"**{x}**" for x in tot)
      + f" | **{len(hits)}** | **{sum(highs.values())}** |\n")


def report(hits, out, samples):
    w = out.write
    w("# Sizes, strides, offsets and hardware values (#820)\n\n")
    w("Generated by `tools/magic_numbers.py --sizes --report`.\n\n")
    w("| Category | What | high | medium | low | Total |\n|---|---|---:|---:|---:|---:|\n")
    for c, desc in CATEGORIES.items():
        n = collections.Counter(h.confidence for h in hits if h.category == c)
        w(f"| `{c}` | {desc} | {n['high']} | {n['medium']} | {n['low']} | {sum(n.values())} |\n")
    n = collections.Counter(h.confidence for h in hits)
    w(f"| | **Total** | **{n['high']}** | **{n['medium']}** | **{n['low']}** | **{len(hits)}** |\n\n")
    w("## Per directory\n\n")
    counts_table(hits, w)
    w("\n")
    if samples:
        w("## High-confidence samples\n\n")
        for c in CATEGORIES:
            sel = [h for h in hits if h.category == c and h.confidence == "high"][:samples]
            if not sel:
                continue
            w(f"### `{c}`\n\n")
            for h in sel:
                w(f"- `{h.file}:{h.line}` `{h.expr}` -> " + " or ".join(f"`{x}`" for x in h.suggestions)
                  + (f" ({h.context})" if h.context else "")
                  + (f", new define {', '.join(h.new_defines)}" if h.new_defines else "") + "\n")
            w("\n")


def run(args):
    types = Types(load_db(args.db, args.rebuild, args.jobs))
    hw = Hardware()
    hits = []
    for rel in source_files(args.path):
        hits += FileScan(rel, types, hw).scan()
    if args.category:
        hits = [h for h in hits if h.category in args.category]
    if args.min_confidence:
        keep = CONFIDENCES[:CONFIDENCES.index(args.min_confidence) + 1]
        hits = [h for h in hits if h.confidence in keep]
    hits.sort(key=lambda h: (directory(h.file), h.file, h.line, h.col))

    if args.json:
        json.dump([h.as_dict() for h in hits], sys.stdout, indent=1)
        sys.stdout.write("\n")
        return 0
    if args.report:
        if args.report == "-":
            report(hits, sys.stdout, args.samples)
        else:
            with open(args.report, "w") as f:
                report(hits, f, args.samples)
        return 0
    by_dir = collections.defaultdict(list)
    for h in hits:
        by_dir[directory(h.file)].append(h)
    for d, hs in by_dir.items():
        n = collections.Counter(h.category for h in hs)
        print(f"== {d}: {len(hs)} (" + ", ".join(f"{c} {n[c]}" for c in CATEGORIES if n[c]) + ")")
        for h in hs:
            print(f"{h.file}:{h.line}: {h.category} {h.confidence} {h.expr} -> {'  or  '.join(h.suggestions)}"
                  + (f"  [{h.context}]" if h.context else "")
                  + (f"  (new: {', '.join(h.new_defines)})" if h.new_defines else "")
                  + (f"\n    {h.source}" if args.show else ""))
    n = collections.Counter(h.category for h in hits)
    c = collections.Counter(h.confidence for h in hits)
    print(f"{len(hits)} sites: " + ", ".join(f"{k} {n[k]}" for k in CATEGORIES if n[k])
          + "; " + ", ".join(f"{k} {c[k]}" for k in CONFIDENCES if c[k]), file=sys.stderr)
    return 0
