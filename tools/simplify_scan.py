#!/usr/bin/env python3
"""List, and try, the simplifications of #846 in the game code.

    tools/simplify_scan.py [PATH...]                 inventory (default: src/)
    tools/simplify_scan.py --list [PATH...]          every candidate, one per line
    tools/simplify_scan.py --try [-j N] [--kinds K] FILE.cpp...

The inventory counts, per directory and per category, the shapes #846
asks to simplify:

  block     a bare `{ }` statement block that declares locals (it only
            scopes them);
  braces    a bare block that declares nothing (redundant braces);
  caseblk   a `case X: { ... }` body with no declarations;
  nestif    `if (a) { if (b) S }` with nothing between and no `else`,
            which could be `if (a && b) S`;
  single    a local assigned once and read once (a single-use temporary);
  copy      the same, where the value is a plain variable or field (a
            copy chain);
  dupcase   switch cases with identical bodies.

A candidate is marked `documented` (and never tried) when a comment says
something about it: a `register` local, a local named in a comment of its
function (or the comment above it), or a block or `if` with a comment on
or just above its first line. The deliberate shapes (#841/#842's dead
tests, the round notes, the accepted redundant copies, the pins and
codegen views) are all commented, so they fall in this group.

--try applies the mechanical ones (block, braces, caseblk, nestif,
single, copy) one at a time to each FILE, rebuilds that file's object
with the Makefile's own commands (`make -n`), and keeps the change only
when the object's .text/.rodata/.data and relocations stay byte-identical
to the object built from the file as it was. Files are tried in parallel;
the kept changes are written to the files, and a TSV log of every try
goes to --log: `kept`, `needed` (the object changes; the log says how:
the registers, the order, a call's place, or N instructions more or
fewer), `error` (it doesn't compile) or `skip` (not applicable: a side
effect would move, the line would pass 100 columns, a macro evaluates
its argument twice, a name would shadow another).

The edits: a block loses its braces and its body moves out one level
(its declarations stay where they were, C++ style). When a jump (a
`goto` or a case label) would cross one of its initialized locals, the
declarations go to the top of the function instead and the initializers
stay as assignments; when the function already has a local of the same
name (the same temporary in two blocks), the block's becomes an
assignment to it. A nested `if` becomes one `&&` test. A single-use
local's value replaces its use, in parentheses only where needed.

--annotate then tries what is left once more and puts a one-line comment
above each local, block or nested `if` that is needed, saying what the
simpler form does (`/* `dir` is needed: inlined, the code is 2
instructions shorter */`). The comment names the local, so the next run
counts it as documented. Only .cpp files are tried: a header's inline
functions are in many objects.
"""
import argparse
import os
import re
import shlex
import subprocess
import sys
import tempfile
import textwrap
from collections import Counter, defaultdict
from concurrent.futures import ThreadPoolExecutor

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# ---------------------------------------------------------------- lexer

PUNCT = sorted(
    "->* <<= >>= ... -> ++ -- << >> <= >= == != && || += -= *= /= %= &= |= ^= :: .* "
    "{ } ( ) [ ] ; , : ? = < > + - * / % & | ^ ! ~ . #".split(),
    key=len,
    reverse=True,
)
ASSIGN = {"=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>="}
KEYWORDS = {
    "if", "else", "for", "while", "do", "switch", "case", "default", "break", "continue",
    "return", "goto", "sizeof", "asm", "__asm__", "new", "delete", "this", "operator",
}
DECL_LEAD = {"const", "volatile", "register", "static", "unsigned", "signed", "struct", "union", "enum", "class"}


class Tok:
    __slots__ = ("kind", "text", "start", "end", "line")

    def __init__(self, kind, text, start, end, line):
        self.kind, self.text, self.start, self.end, self.line = kind, text, start, end, line

    def __repr__(self):
        return self.text


ID_RE = re.compile(r"[A-Za-z_]\w*")
NUM_RE = re.compile(r"(0[xX][0-9a-fA-F]+|\d+\.?\d*([eE][-+]?\d+)?)[uUlLfF]*")


def lex(text):
    """Tokens, comments [(start, end, text, line)], with preprocessor lines skipped."""
    toks, comments = [], []
    i, n, line = 0, len(text), 1
    bol = True
    while i < n:
        c = text[i]
        if c == "\n":
            line += 1
            i += 1
            bol = True
            continue
        if c in " \t\r\f\v":
            i += 1
            continue
        if bol and c == "#":
            while i < n and text[i] != "\n":
                if text[i] == "\\" and i + 1 < n and text[i + 1] == "\n":
                    i += 2
                    line += 1
                    continue
                i += 1
            continue
        bol = False
        if text.startswith("//", i):
            j = text.find("\n", i)
            j = n if j < 0 else j
            comments.append((i, j, text[i:j], line))
            i = j
            continue
        if text.startswith("/*", i):
            j = text.find("*/", i + 2) + 2
            comments.append((i, j, text[i:j], line))
            line += text.count("\n", i, j)
            i = j
            continue
        if c in "\"'":
            j = i + 1
            while text[j] != c:
                j += 2 if text[j] == "\\" else 1
            toks.append(Tok("str", text[i : j + 1], i, j + 1, line))
            i = j + 1
            continue
        m = ID_RE.match(text, i)
        if m:
            toks.append(Tok("id", m.group(), i, m.end(), line))
            i = m.end()
            continue
        m = NUM_RE.match(text, i)
        if m:
            toks.append(Tok("num", m.group(), i, m.end(), line))
            i = m.end()
            continue
        for p in PUNCT:
            if text.startswith(p, i):
                toks.append(Tok("p", p, i, i + len(p), line))
                i += len(p)
                break
        else:
            raise SyntaxError("line %d: %r" % (line, text[i : i + 20]))
    return toks, comments


def match_brackets(toks):
    match, stack = {}, []
    pair = {")": "(", "]": "[", "}": "{"}
    for k, t in enumerate(toks):
        if t.text in "([{" and t.kind == "p":
            stack.append(k)
        elif t.text in ")]}" and t.kind == "p":
            o = stack.pop()
            assert toks[o].text == pair[t.text], (toks[o].line, t.line)
            match[o], match[k] = k, o
    return match


# ---------------------------------------------------------------- parser


class Node:
    def __init__(self, kind, a, b, **kw):
        self.kind, self.a, self.b = kind, a, b  # token range [a, b)
        self.kids = []
        self.__dict__.update(kw)


class Func:
    def __init__(self, name, body):
        self.name, self.body = name, body


class Parser:
    def __init__(self, text):
        self.text = text
        self.toks, self.comments = lex(text)
        self.match = match_brackets(self.toks)
        self.funcs = []
        self._find_funcs(0, len(self.toks))

    def t(self, k):
        return self.toks[k].text if k < len(self.toks) else ""

    def _find_funcs(self, a, b):
        k = a
        while k < b:
            if self.t(k) != "{":
                k += 1
                continue
            e = self.match[k]
            p = k - 1
            if self.t(p) == "const":
                p -= 1
            if self.t(p) == ")":
                name = self._func_name(p)
                body = self.block(k)
                self.funcs.append(Func(name, body))
            elif self.t(p) == '"C"' or (self.t(p - 1) == "namespace"):
                self._find_funcs(k + 1, e)
            k = e + 1

    def _func_name(self, rp):
        lp = self.match[rp]
        q = lp - 1
        # a constructor's initializer list: walk back over `: a(x), b(y)`
        while self.t(q - 1) in (",", ":") and self.t(q - 2) == ")":
            rp = q - 2 if self.t(q - 1) == "," else rp
            q = self.match[q - 2] - 1 if self.t(q - 1) == "," else q
            if self.t(q - 1) == ":":
                rp = q - 2
                lp = self.match[rp]
                q = lp - 1
                break
        name = self.t(q)
        while self.t(q - 1) == "::":
            q -= 2
            name = self.t(q) + "::" + name
        return name

    def block(self, k):
        e = self.match[k]
        n = Node("block", k, e + 1)
        i = k + 1
        while i < e:
            s = self.stmt(i)
            n.kids.append(s)
            i = s.b
        return n

    def stmt(self, i):
        t = self.t(i)
        if t == "{":
            return self.block(i)
        if t == "if":
            rp = self.match[i + 1]
            th = self.stmt(rp + 1)
            n = Node("if", i, th.b, cond=(i + 2, rp), then=th, els=None)
            if self.t(th.b) == "else":
                n.els = self.stmt(th.b + 1)
                n.b = n.els.b
            return n
        if t in ("for", "while", "switch"):
            rp = self.match[i + 1]
            body = self.stmt(rp + 1)
            n = Node(t, i, body.b, body=body)
            return n
        if t == "do":
            body = self.stmt(i + 1)
            k = body.b
            assert self.t(k) == "while", self.toks[k].line
            rp = self.match[k + 1]
            return Node("do", i, rp + 2, body=body)
        if t == "case" or (t == "default" and self.t(i + 1) == ":"):
            k = i
            while self.t(k) != ":":
                k = self.match.get(k, k) + 1 if self.t(k) in "([" else k + 1
            return Node("label", i, k + 1, case=True)
        if self.toks[i].kind == "id" and self.t(i + 1) == ":" and t not in KEYWORDS:
            return Node("label", i, i + 2, case=False)
        k = i
        while self.t(k) not in (";", "}"):
            k = self.match[k] + 1 if self.t(k) in ("(", "[", "{") else k + 1
        if self.t(k) == "}":  # a statement macro with no `;` (SET_VEL(...))
            return Node("expr", i, k, macro=True)
        kind = "decl" if self.is_decl(i, k) else "expr"
        return Node(kind, i, k + 1)

    def is_decl(self, i, k):
        j = i
        while self.t(j) in DECL_LEAD:
            j += 1
        if self.toks[j].kind != "id" or self.t(j) in KEYWORDS:
            return False
        if j > i and self.t(i) in ("register", "static", "const", "volatile", "struct", "union", "enum", "class"):
            return True
        j += 1
        while self.t(j) == "::" and self.toks[j + 1].kind == "id":
            j += 2
        if self.t(j) == "<":  # a template type
            return False
        while self.t(j) in ("*", "&", "const"):
            j += 1
        return self.toks[j].kind == "id" and self.t(j) not in KEYWORDS and self.t(j + 1) in ("=", ";", ",", "[", "(")

    # -- declarators of a decl statement: [(name_tok, init_range or None, start, end)]
    def declarators(self, node):
        i = node.a
        quals = set()
        while self.t(i) in DECL_LEAD:
            quals.add(self.t(i))
            i += 1
        i += 1
        while self.t(i) == "::":
            i += 2
        out = []
        end = node.b - 1  # the ';'
        while i < end:
            s = i
            while self.t(i) in ("*", "&", "const"):
                i += 1
            name = i
            i += 1
            arr = False
            while self.t(i) == "[":
                arr = True
                i = self.match[i] + 1
            init = None
            ctor = False
            if self.t(i) == "(":
                ctor = True
                i = self.match[i] + 1
            if self.t(i) == "=":
                a = i + 1
                while self.t(i) not in (",", ";") or i < a:
                    i = self.match[i] + 1 if self.t(i) in ("(", "[", "{") else i + 1
                init = (a, i)
            out.append(dict(name=name, init=init, start=s, end=i, arr=arr or ctor, ptr=self.t(s) in ("*", "&")))
            if self.t(i) == ",":
                i += 1
        return out, quals


def walk(n, parent=None, depth=0):
    yield n, parent
    for c in n.kids:
        yield from walk(c, n, depth + 1)
    for attr in ("then", "els", "body"):
        c = getattr(n, attr, None)
        if c is not None:
            yield from walk(c, n, depth + 1)


# ---------------------------------------------------------------- candidates

DOC_WORDS = re.compile(
    r"scope|block|brace|own |separate|live|register|\br\d+\b|pseudo|pin|round|#84[12]|deliberate|match|ROM|codegen|keep|nest",
    re.I,
)


class Cand:
    def __init__(self, kind, func, line, key, doc=False, **kw):
        self.kind, self.func, self.line, self.key, self.doc = kind, func, line, key, doc
        self.__dict__.update(kw)


def norm(p, a, b):
    return " ".join(p.t(k) for k in range(a, b))


def comments_near(p, tokidx, above=2):
    """Comments on the line of token tokidx, or ending on one of the `above` lines above it."""
    line = p.toks[tokidx].line
    out = []
    for s, e, txt, l in p.comments:
        el = l + txt.count("\n")
        if l == line or line - above <= el < line:
            out.append(txt)
    return out


def func_comments(p, f):
    """Text of the comments inside f and the comment block just above it."""
    a, b = p.toks[f.body.a].start, p.toks[f.body.b - 1].end
    out = []
    # the header comment: the last comment ending before the function's line, if within 3 lines
    hdr_line = p.toks[f.body.a].line
    k = f.body.a
    while k > 0 and p.toks[k - 1].text not in ("}", ";"):
        k -= 1
    sig_line = p.toks[k].line if k < len(p.toks) else hdr_line
    for s, e, txt, l in p.comments:
        el = l + txt.count("\n")
        if a <= s < b or sig_line - 2 <= el <= sig_line:
            out.append(txt)
    return "\n".join(out)


def mentions(comments, name):
    if re.search(r"`[^`]*\b%s\b[^`]*`" % re.escape(name), comments):
        return True
    if len(name) >= 3 and re.search(r"(?<![\w>.])%s(?!\w)" % re.escape(name), comments):
        return True
    return False


def scan(text, path=""):
    p = Parser(text)
    cands = []
    for f in p.funcs:
        fc = None
        nodes = list(walk(f.body))
        for n, parent in nodes:
            if n.kind == "block" and parent is not None and parent.kind == "block":
                idx = parent.kids.index(n)
                after_label = idx > 0 and parent.kids[idx - 1].kind == "label" and parent.kids[idx - 1].case
                decls = [c for c in n.kids if c.kind == "decl"]
                kind = "block" if decls else ("caseblk" if after_label else "braces")
                if after_label and decls:
                    continue  # a case body with locals is fine (#846)
                near = comments_near(p, n.a) + comments_near(p, n.a + 1 if n.a + 1 < n.b else n.a, 0)
                doc = any(DOC_WORDS.search(c) for c in near)
                cands.append(Cand(kind, f.name, p.toks[n.a].line, (f.name, kind, norm(p, n.a, n.b)), doc, node=(n.a, n.b), fn=f, blk=n))
            if n.kind == "if" and n.els is None:
                inner = n.then
                if inner.kind == "block" and len(inner.kids) == 1:
                    inner = inner.kids[0]
                if inner.kind == "if" and inner.els is None and (parent is None or not (parent.kind == "if" and parent.then is n and parent.els is not None)):
                    near = comments_near(p, n.a) + comments_near(p, inner.a)
                    doc = any(DOC_WORDS.search(c) for c in near) or bool(comments_in(p, n.then.a, inner.a))
                    cands.append(Cand("nestif", f.name, p.toks[n.a].line, (f.name, "nestif", norm(p, *n.cond), norm(p, *inner.cond)), doc, outer=n, inner=inner))
            if n.kind == "switch" and n.body.kind == "block":
                groups = defaultdict(list)
                kids = n.body.kids
                i = 0
                while i < len(kids):
                    if kids[i].kind != "label":
                        i += 1
                        continue
                    j = i
                    while j < len(kids) and kids[j].kind == "label":
                        j += 1
                    k = j
                    while k < len(kids) and kids[k].kind != "label":
                        k += 1
                    if k > j:
                        body = " ".join(norm(p, kids[x].a, kids[x].b) for x in range(j, k))
                        if re.search(r"\b(break|return|continue|goto)\b[^;]*;$", body):
                            groups[body].append(p.toks[kids[i].a].line)
                    i = k
                for body, lines in groups.items():
                    if len(lines) > 1:
                        cands.append(Cand("dupcase", f.name, lines[0], (f.name, "dupcase", body), False, lines=lines))
        # single-use locals
        decls = [(n, parent) for n, parent in nodes if n.kind == "decl"]
        if not decls:
            continue
        uses = defaultdict(list)
        for k in range(f.body.a, f.body.b):
            tk = p.toks[k]
            if tk.kind == "id" and p.t(k - 1) not in (".", "->", "::"):
                uses[tk.text].append(k)
        for n, parent in decls:
            dl, quals = p.declarators(n)
            if "static" in quals or "register" in quals:
                if "register" in quals:
                    for d in dl:
                        cands.append(Cand("single", f.name, p.toks[d["name"]].line, (f.name, "single", p.t(d["name"])), True))
                continue
            for d in dl:
                if d["arr"]:
                    continue
                name = p.t(d["name"])
                occ = [k for k in uses[name] if k != d["name"]]
                if p.t(d["name"] + 1) == "(":
                    continue
                if d["init"]:
                    if len(occ) != 1 or len(dl) != 1:
                        continue
                    defk, valr, stmt = None, d["init"], (n.a, n.b)
                    use = occ[0]
                else:
                    if len(occ) != 2:
                        continue
                    a, use = occ
                    # `name = expr;` as a whole statement
                    st = next((x for x, _ in nodes if x.kind == "expr" and x.a == a), None)
                    if st is None or p.t(a + 1) != "=":
                        continue
                    valr, stmt = (a + 2, st.b - 1), (st.a, st.b)
                    defk = a
                if p.t(use + 1) in ASSIGN or p.t(use + 1) in ("++", "--") or p.t(use - 1) in ("&", "++", "--"):
                    continue
                if p.t(use - 1) == "&" or use < valr[1]:
                    continue
                val = norm(p, *valr)
                if fc is None:
                    fc = func_comments(p, f)
                doc = mentions(fc, name) or any(comments_near(p, stmt[0], 1))
                kind = "copy" if re.fullmatch(r"\w+( (\.|->) \w+)*", val) else "single"
                cands.append(Cand(kind, f.name, p.toks[d["name"]].line, (f.name, kind, name, val), doc,
                                  decl=n, dcl=d, ndecl=len(dl), defstmt=stmt, val=valr, use=use, name=name))
    # the same shape twice in a function: told apart by occurrence
    seen = Counter()
    for c in cands:
        seen[c.key] += 1
        c.key = c.key + (seen[c.key],)
    return p, cands


def comments_in(p, a, b):
    s, e = p.toks[a].start, p.toks[b].start
    return [c for c in p.comments if s <= c[0] < e]


# ---------------------------------------------------------------- edits


def line_bounds(text, pos):
    a = text.rfind("\n", 0, pos) + 1
    b = text.find("\n", pos)
    return a, (len(text) if b < 0 else b)


def dedent(text, a, b, n=4):
    seg = text[a:b].split("\n")
    seg = [s[n:] if s.startswith(" " * n) else s for s in seg]
    return text[:a] + "\n".join(seg) + text[b:]


def drop_brace_line(text, pos, closing):
    """Remove the brace at pos: its whole line if alone, else just ` {`."""
    a, b = line_bounds(text, pos)
    if text[a:b].strip() == text[pos]:
        return text[:a] + text[b + 1 :], True
    if not closing and text[pos + 1 : b].strip() == "":
        q = pos
        while q > a and text[q - 1] == " ":
            q -= 1
        return text[:q] + text[pos + 1 :], True
    return text, False


def apply_block(p, text, c):
    a, b = c.node
    lb, rb = p.toks[a].start, p.toks[b - 1].start
    la, _ = line_bounds(text, lb)
    if text[la:lb].strip() and not text[la:lb].rstrip().endswith(":"):
        return None
    # body lines: from the line after `{` to the line before `}`
    _, l1 = line_bounds(text, lb)
    l2, _ = line_bounds(text, rb)
    if text[l2:rb].strip():
        return None
    _, rend = line_bounds(text, rb)
    if text[rb + 1 : rend].strip():
        return None
    body = text[l1 + 1 : l2]
    if text[la:lb].strip():  # `case X: {`: keep the body's indentation
        new_body = body
    else:
        new_body = "\n".join(s[4:] if s.startswith("    ") else s for s in body.split("\n"))
    head = text[:la] + (text[la:lb].rstrip() + "\n" if text[la:lb].strip() else "")
    # declarations now in the middle of a block: a blank line before them
    prev = text[:la].rstrip(" ").rsplit("\n", 2)
    prev = prev[-2] if len(prev) >= 2 else ""
    if c.kind == "block" and prev.strip() and not prev.rstrip().endswith(("{", ":")) and new_body.strip():
        head += "\n"
    return head + new_body + text[rend + 1 :]


def apply_hoist(p, text, c):
    """Flatten a block whose locals a jump would cross: the declarations go
    to the function's own (without their initializers, which stay as
    assignments where they were)."""
    n, f = c.blk, c.fn
    lead = []
    for k in f.body.kids:
        if k.kind != "decl":
            break
        lead.append(k)
    hoisted, edits = [], []
    # a name the function declares elsewhere too would shadow, or be shadowed
    others = set()
    for x, _ in walk(f.body):
        if x.kind == "decl" and not (n.a <= x.a < n.b):
            others |= {p.t(y["name"]) for y in p.declarators(x)[0]}
    for d in (k for k in n.kids if k.kind == "decl"):
        dl, quals = p.declarators(d)
        if quals & {"static", "register"} or {p.t(x["name"]) for x in dl} & others:
            return None
        prefix = p.text[p.toks[d.a].start : p.toks[dl[0]["start"]].start].rstrip()
        assigns = []
        for x in dl:
            nm_end = x["name"] + 1
            while p.t(nm_end) == "[":
                nm_end = p.match[nm_end] + 1
            hoisted.append(prefix + " " + p.text[p.toks[x["start"]].start : p.toks[nm_end - 1].end] + ";")
            if x["init"]:
                if x["arr"]:
                    return None
                ia, ib = x["init"]
                assigns.append(p.t(x["name"]) + " = " + p.text[p.toks[ia].start : p.toks[ib - 1].end] + ";")
        s, e = p.toks[d.a].start, p.toks[d.b - 1].end
        if assigns:
            a, _ = line_bounds(text, s)
            edits.append((s, e, ("\n" + text[a:s]).join(assigns)))
        else:
            edits.append(_stmt_line(p, text, d.a, d.b))
    # the declarations, after the function's own (the edits are all below them)
    if lead:
        pos = line_bounds(text, p.toks[lead[-1].b - 1].start)[1] + 1
        ins = "".join("    " + h + "\n" for h in hoisted)
    else:
        pos = line_bounds(text, p.toks[f.body.a].start)[1] + 1
        ins = "".join("    " + h + "\n" for h in hoisted) + "\n"
    edits.append((pos, pos, ins))
    for s, e, r in sorted(edits, key=lambda x: (x[0], x[1]), reverse=True):
        text = text[:s] + r + text[e:]
    # the block, now without declarations: the same block index in the new text
    q = Parser(text)
    blocks = [x for x, par in walk(q.funcs[[g.name for g in p.funcs].index(f.name)].body) if x.kind == "block" and par is not None and par.kind == "block"]
    old = [x for x, par in walk(f.body) if x.kind == "block" and par is not None and par.kind == "block"]
    if len(blocks) != len(old):
        return None
    nb = blocks[old.index(n)]
    text = apply_block(q, text, Cand("braces", c.func, c.line, c.key, node=(nb.a, nb.b)))
    if text is None:
        return None
    text = re.sub(r"\{\n\s*\n", "{\n", text)
    text = re.sub(r"\n[ \t]*\n([ \t]*\n)+", "\n\n", text)
    return text


def apply_merge(p, text, c, names):
    """Flatten a block whose locals repeat ones of the function (the same
    kind of temporary in two blocks): the repeats become assignments to
    the function's."""
    n = c.blk
    edits = []
    for d in (k for k in n.kids if k.kind == "decl"):
        dl, quals = p.declarators(d)
        if quals & {"static", "register"}:
            return None
        rep = [x for x in dl if p.t(x["name"]) in names]
        if not rep:
            continue
        keep = [x for x in dl if p.t(x["name"]) not in names]
        assigns = []
        for x in rep:
            if x["arr"]:
                return None
            if x["init"]:
                ia, ib = x["init"]
                assigns.append(p.t(x["name"]) + " = " + p.text[p.toks[ia].start : p.toks[ib - 1].end] + ";")
        s, e = p.toks[d.a].start, p.toks[d.b - 1].end
        a, _ = line_bounds(text, s)
        ind = text[a:s]
        out = []
        if keep:
            prefix = p.text[p.toks[d.a].start : p.toks[dl[0]["start"]].start].rstrip()
            out.append(prefix + " " + ", ".join(p.text[p.toks[x["start"]].start : p.toks[x["end"] - 1].end] for x in keep) + ";")
        out += assigns
        if out:
            edits.append((s, e, ("\n" + ind).join(out)))
        else:
            edits.append(_stmt_line(p, text, d.a, d.b))
    if not edits:
        return None
    a, b = n.a, n.b
    for s, e, r in sorted(edits, reverse=True):
        text = text[:s] + r + text[e:]
    q = Parser(text)
    f = q.funcs[[g.name for g in p.funcs].index(c.fn.name)]
    blocks = [x for x, par in walk(f.body) if x.kind == "block" and par is not None and par.kind == "block"]
    old = [x for x, par in walk(c.fn.body) if x.kind == "block" and par is not None and par.kind == "block"]
    if len(blocks) != len(old):
        return None
    nb = blocks[old.index(n)]
    return apply_block(q, text, Cand("block", c.func, c.line, c.key, node=(nb.a, nb.b)))


BINPREC = {
    "*": 13, "/": 13, "%": 13, "+": 12, "-": 12, "<<": 11, ">>": 11, "<": 10, "<=": 10, ">": 10, ">=": 10,
    "==": 9, "!=": 9, "&": 8, "^": 7, "|": 6, "&&": 5, "||": 4, "?": 3, ":": 3, ",": 1,
}
BINPREC.update({op: 2 for op in ASSIGN})
OPERAND_END = ("id", "num", "str")


def is_binary(p, k):
    """Whether the operator token k is binary (it follows an operand)."""
    prev = p.toks[k - 1]
    return prev.kind in OPERAND_END and prev.text not in KEYWORDS or prev.text in (")", "]")


def expr_prec(p, a, b):
    """The precedence of [a, b)'s loosest top-level operator (16: a primary or postfix)."""
    prec = 16
    k = a
    while k < b:
        t = p.t(k)
        if t in ("(", "["):
            if t == "(" and k == a and p.match[k] + 1 < b and not is_binary(p, k) and p.toks[p.match[k] + 1].kind in OPERAND_END + ("p",) and p.t(p.match[k] + 1) not in BINPREC:
                prec = min(prec, 15)  # a cast
            k = p.match[k] + 1
            continue
        if p.toks[k].kind == "p" and t in BINPREC and k > a and is_binary(p, k):
            prec = min(prec, BINPREC[t])
        elif p.toks[k].kind == "p" and t in ("!", "~", "-", "+", "*", "&", "++", "--") and k == a:
            prec = min(prec, 15)
        elif t == "sizeof" and k == a:
            prec = min(prec, 15)
        k += 1
    return prec


def side_effects(p, a, b):
    return any(p.t(k) in ("++", "--") or (p.t(k) in ASSIGN and p.toks[k].kind == "p") for k in range(a, b))


def apply_nestif(p, text, c):
    o, i = c.outer, c.inner
    ca, cb = o.cond
    ia, ib = i.cond
    if side_effects(p, ca, cb) or side_effects(p, ia, ib):
        return None
    oc = p.text[p.toks[ca].start : p.toks[cb - 1].end]
    ic = p.text[p.toks[ia].start : p.toks[ib - 1].end]
    # `&&` binds tighter than `||`/`?:`; a bitwise operand gets them for clarity
    if expr_prec(p, ca, cb) <= 8 and expr_prec(p, ca, cb) != 5:
        oc = "(" + oc + ")"
    if expr_prec(p, ia, ib) <= 8 and expr_prec(p, ia, ib) != 5:
        ic = "(" + ic + ")"
    if "\n" in oc or "\n" in ic:
        return None
    if comments_in(p, o.a, i.then.a):
        return None
    # rebuild: `if (oc && ic)` + inner's then, with the outer's indentation
    ostart = p.toks[o.a].start
    la, _ = line_bounds(text, ostart)
    indent = text[la:ostart]
    if indent.strip():
        return None
    th = i.then
    tstart, tend = p.toks[th.a].start, p.toks[th.b - 1].end
    ilin, _ = line_bounds(text, p.toks[i.a].start)
    inner_indent = text[ilin : p.toks[i.a].start]
    shift = len(inner_indent) - len(indent)
    body = text[tstart:tend]
    if shift:
        body = "\n".join(s[shift:] if s.startswith(" " * shift) else s for s in body.split("\n"))
    sep = " " if th.kind == "block" else ("\n" + indent + "    " if text[p.toks[i.b - 1].start - 1] != " " or "\n" in text[p.toks[ib].end : tstart] else " ")
    if th.kind != "block":
        sep = "\n" + indent + "    " if "\n" in text[p.toks[ib].end : tstart] else " "
    new = "if (" + oc + " && " + ic + ")" + sep + body.lstrip()
    oend = p.toks[o.b - 1].end
    return text[:ostart] + new + text[oend:]


COLUMNS = 100  # .clang-format's ColumnLimit
_MACROS = None


def overlong(old, new):
    """Whether new has a line past COLUMNS that old doesn't: the changed
    lines measured as they come out, with their indentation (a folded
    `&&` test, an inlined value, a hoisted declaration)."""
    def long_lines(text):
        return Counter(l for l in text.split("\n") if len(l.expandtabs(4)) > COLUMNS)

    return bool(long_lines(new) - long_lines(old))


def macros(text):
    """{name: max uses of one parameter} for the function-like macros of include/ and text."""
    global _MACROS
    if _MACROS is None:
        # filled before it is published: --try's threads read it while it's built
        found = {}
        for top in ("include", os.path.join("platform", "gba", "include")):
            for d, _, fs in os.walk(os.path.join(ROOT, top)):
                for f in fs:
                    with open(os.path.join(d, f), errors="replace") as fh:
                        found.update(_macro_defs(fh.read()))
        _MACROS = found
    out = dict(_MACROS)
    out.update(_macro_defs(text))
    return out


def _macro_defs(text):
    out = {}
    text = text.replace("\\\n", " ")
    for m in re.finditer(r"^\s*#\s*define\s+(\w+)\(([^)]*)\)(.*)$", text, re.M):
        params = [x.strip() for x in m.group(2).split(",") if x.strip()]
        body = m.group(3)
        out[m.group(1)] = max([len(re.findall(r"\b%s\b" % re.escape(x), body)) for x in params] or [0])
    return out


def in_multi_eval_macro(p, k):
    """Whether token k is inside the arguments of a macro that evaluates a parameter twice."""
    mac = macros(p.text)
    j = k
    while j > 0:
        j -= 1
        if p.t(j) == "(" and p.match.get(j, 0) > k:
            name = p.t(j - 1)
            if mac.get(name, 0) > 1:
                return True
        if p.t(j) in (";", "{", "}"):
            return False
    return False


def apply_inline(p, text, c):
    va, vb = c.val
    if side_effects(p, va, vb) or any(p.t(k) == "," for k in _top(p, va, vb)):
        return None
    if in_multi_eval_macro(p, c.use) and vb - va > 1:
        return None
    val = p.text[p.toks[va].start : p.toks[vb - 1].end]
    if "\n" in val:
        val = re.sub(r"\s*\n\s*", " ", val)
    # parentheses only where the use's neighbours bind tighter than the value
    vp = expr_prec(p, va, vb)
    need = 0
    L, R = p.toks[c.use - 1], p.toks[c.use + 1]
    if L.kind == "p" and L.text in BINPREC and is_binary(p, c.use - 1):
        need = max(need, BINPREC[L.text] + 1)
    elif L.kind == "p" and L.text in ("!", "~", "-", "+", "*", "&"):
        need = max(need, 15)  # a unary operator
    elif L.text == ")":
        before = p.toks[p.match[c.use - 1] - 1]
        if before.kind == "p" and before.text not in (")", "]") or before.text == "return":
            need = max(need, 15)  # a cast
    if R.kind == "p" and R.text in ("->", ".", "[", "(", "++", "--"):
        need = max(need, 16)
    elif R.kind == "p" and R.text in BINPREC and R.text not in (",", ":"):
        need = max(need, BINPREC[R.text])
    if L.text == "?" or R.text == ":":
        need = max(need, 4)
    u = p.toks[c.use]
    edits = []
    # `&x` into `*use` or `use->f`: `x` and `x.f`, not `*&x` and `(&x)->f`
    if p.t(va) == "&" and expr_prec(p, va + 1, vb) == 16 and (R.text == "->" or L.text == "*" and not is_binary(p, c.use - 1)):
        rest = p.text[p.toks[va + 1].start : p.toks[vb - 1].end]
        if R.text == "->":
            edits.append((u.start, R.end, rest + "."))
        else:
            edits.append((L.start, u.end, rest))
    else:
        if vp < need:
            val = "(" + val + ")"
        edits.append((u.start, u.end, val))
    sa, sb = c.defstmt
    if c.dcl["init"] is None:
        # the assignment statement, and the declarator in its decl
        edits.append(_stmt_line(p, text, sa, sb))
        if c.ndecl == 1:
            edits.append(_stmt_line(p, text, c.decl.a, c.decl.b))
        else:
            d = c.dcl
            s, e = p.toks[d["start"]].start, p.toks[d["end"] - 1].end
            if p.t(d["end"]) == ",":
                e = p.toks[d["end"] + 1].start
            else:
                s = p.toks[d["start"] - 1].start  # the ',' before it
            edits.append((s, e, ""))
    else:
        edits.append(_stmt_line(p, text, sa, sb))
    for s, e, r in sorted(edits, reverse=True):
        text = text[:s] + r + text[e:]
    # a decl removed from a block that leaves a blank line after `{` or two blank lines
    text = re.sub(r"\{\n\s*\n", "{\n", text)
    text = re.sub(r"\n[ \t]*\n([ \t]*\n)+", "\n\n", text)
    return text


def _top(p, a, b):
    k = a
    while k < b:
        yield k
        k = p.match[k] + 1 if p.t(k) in ("(", "[") else k + 1


def _stmt_line(p, text, a, b):
    s, e = p.toks[a].start, p.toks[b - 1].end
    la, lb = line_bounds(text, s)
    ea, eb = line_bounds(text, e - 1)
    if not text[la:s].strip() and not text[e:eb].strip():
        return (la, eb + 1, "")
    return (s, e, "")


APPLY = {"block": apply_block, "braces": apply_block, "caseblk": apply_block, "nestif": apply_nestif, "single": apply_inline, "copy": apply_inline}

# ---------------------------------------------------------------- build


def compile_cmds(src):
    obj = "build/crashbandicootxs/" + os.path.splitext(src)[0] + ".o"
    out = subprocess.run(["make", "-n", "-B", obj], cwd=ROOT, capture_output=True, text=True).stdout
    cpp = cc1 = asm = None
    for line in out.splitlines():
        if "|" in line and src in line and "agbc" in line:
            a, b = line.split("|", 1)
            cpp = [x for x in shlex.split(a)]
            for f in ("-MF", "-MT"):
                if f in cpp:
                    i = cpp.index(f)
                    del cpp[i : i + 2]
            cpp = [x for x in cpp if x not in ("-MMD", "-MP", src)]
            cc1 = shlex.split(b)
            i = cc1.index("-o")
            del cc1[i : i + 2]
        elif line.startswith("arm-none-eabi-as") and obj in line:
            asm = shlex.split(line)
            i = asm.index("-o")
            del asm[i : i + 2]
            asm = asm[:-1]
    if not cpp or not asm:
        raise SystemExit("no compile command for " + src)
    return cpp, cc1, asm


def _err(stderr):
    lines = [l for l in stderr.splitlines() if not re.search(r": In (method|function)|^In file", l)] or ["?"]
    return " / ".join(re.sub(r"^[^:]*:\d+: ", "", l).strip() for l in lines[:3])


def build_log(cmds, text, work):
    """The compiler's whole stderr for text."""
    cpp, cc1, _ = cmds
    pre = subprocess.run(cpp + ["-"], cwd=ROOT, input=text, capture_output=True, text=True)
    r = subprocess.run(cc1 + ["-o", os.path.join(work, "t.s")], cwd=ROOT, input=pre.stdout, capture_output=True, text=True)
    return pre.stderr + r.stderr


def build(cmds, text, work):
    """Signature of the object built from text: (sections, relocs, regnorm), or an error message."""
    cpp, cc1, asm = cmds
    pre = subprocess.run(cpp + ["-"], cwd=ROOT, input=text, capture_output=True, text=True)
    if pre.returncode:
        return _err(pre.stderr)
    s = os.path.join(work, "t.s")
    o = os.path.join(work, "t.o")
    r = subprocess.run(cc1 + ["-o", s], cwd=ROOT, input=pre.stdout, capture_output=True, text=True)
    if r.returncode:
        return _err(r.stderr)
    with open(s, "a") as fh:
        fh.write("\t.text\n\t.align\t2, 0\n")
    r = subprocess.run(asm + ["-o", o, s], cwd=ROOT, capture_output=True, text=True)
    if r.returncode:
        return _err(r.stderr)
    secs = subprocess.run(["arm-none-eabi-objdump", "-s", "-j", ".text", "-j", ".rodata", "-j", ".data", o], capture_output=True, text=True).stdout
    secs = secs.split("\n", 3)[-1]
    rel = subprocess.run(["arm-none-eabi-objdump", "-r", o], capture_output=True, text=True).stdout
    rel = "\n".join(l for l in rel.splitlines() if not l.startswith(o) and "file format" not in l)
    dis = subprocess.run(["arm-none-eabi-objdump", "-d", "--no-show-raw-insn", o], capture_output=True, text=True).stdout
    funcs, cur, start = {}, None, 0
    for line in dis.splitlines():
        m = re.match(r"^([0-9a-f]+) <(.+)>:$", line)
        if m:
            cur, start = funcs.setdefault(m.group(2), []), int(m.group(1), 16)
            continue
        m = re.match(r"^\s*[0-9a-f]+:\s*(.*)$", line)
        if m and cur is not None:
            ins = re.sub(r"\s+", " ", re.sub(r" <[^>]*>|\s*[;@].*", "", m.group(1))).strip()
            bm = re.match(r"^(b\S*) ([0-9a-f]+)$", ins)
            if bm and bm.group(1) != "bl":
                ins = "%s .%+x" % (bm.group(1), int(bm.group(2), 16) - start)
            if re.match(r"^ldr \w+, \[pc", ins):
                ins = re.sub(r"#\d+", "#pool", ins)
            cur.append(ins)
    return secs, rel, funcs


REG = re.compile(r"\b(r\d+|sl|fp|ip|lr)\b")
def classify(base, got):
    """What changed between two builds: the registers, the order of the
    same instructions (a call's place), or the code (by how many
    instructions), as a phrase for a comment."""
    bf, gf = base[2], got[2]
    diff = [k for k in bf if bf[k] != gf.get(k)]
    rn = lambda xs: [REG.sub("R", x) for x in xs]
    calls = lambda xs: [x for x in xs if x.startswith("bl ")]
    kinds, delta = set(), 0
    for k in diff:
        b, g = bf[k], gf.get(k, [])
        if rn(b) == rn(g):
            kinds.add("registers")
        elif sorted(rn(b)) == sorted(rn(g)):
            kinds.add("call" if [i for i, x in enumerate(b) if x.startswith("bl ")] != [i for i, x in enumerate(g) if x.startswith("bl ")] else "order")
        else:
            kinds.add("code")
            delta += len(g) - len(b)
    if "code" in kinds or not kinds:
        if delta:
            return "the code is %d instruction%s %s" % (abs(delta), "s" if abs(delta) > 1 else "", "longer" if delta > 0 else "shorter")
        return "the instructions differ"
    if "call" in kinds:
        return "a call moves"
    if "order" in kinds:
        return "the instructions are reordered"
    return "the registers differ"


def try_file(src, kinds, annotate=False):
    path = os.path.join(ROOT, src)
    with open(path) as fh:
        text = fh.read()
    cmds = compile_cmds(src)
    rows = []
    with tempfile.TemporaryDirectory(prefix="simplify_") as work:
        base = build(cmds, text, work)
        if isinstance(base, str):
            return [(src, "-", "-", 0, "base-error", "")]
        tried = set()
        while True:
            p, cands = scan(text, src)
            todo = [c for c in cands if c.kind in kinds and not c.doc and c.key not in tried]
            if not todo:
                break
            c = todo[0]
            tried.add(c.key)
            res, new, what = attempt(cmds, base, work, p, text, c, rows, src)
            if res == "needed":
                what = (getattr(c, "name", "") + " " + what).strip()
            if res == "kept":
                # the later occurrences of the same shape move down one
                k, n = c.key[:-1], c.key[-1]
                tried = {t[:-1] + (t[-1] - 1,) if t[:-1] == k and t[-1] > n else t for t in tried if t != c.key}
                text = new
                with open(path, "w") as fh:
                    fh.write(text)
            rows.append((src, c.func, c.kind, c.line, res, what))
        if annotate:
            # what is left and needed, tried again on the final text
            p, cands = scan(text, src)
            needed = []
            for c in cands:
                if c.kind in kinds and not c.doc:
                    res, new, why = attempt(cmds, base, work, p, text, c, [], src)
                    if res == "needed" and c.kind in ("single", "copy", "nestif", "merge"):
                        needed.append((c, why))
            new = annotate_needed(p, text, needed)
            got = build(cmds, new, work)
            if not isinstance(got, str) and got[:2] == base[:2]:
                with open(path, "w") as fh:
                    fh.write(new)
    return rows


def attempt(cmds, base, work, p, text, c, rows, src):
    """Try candidate c: ("kept", new text, "") or (result, None, what)."""
    new = APPLY[c.kind](p, text, c)
    what = getattr(c, "name", "")
    if new is None or new == text or overlong(text, new):
        return "skip", None, what
    got = build(cmds, new, work)
    if isinstance(got, str) and c.kind == "block" and "cross" in got:
        # a jump crosses a declaration: hoist the declarations instead
        rows.append((src, c.func, c.kind, c.line, "error", got))
        new = apply_hoist(p, text, c)
        if new is None or overlong(text, new):
            return "skip", None, what
        got = build(cmds, new, work)
        c.kind = "hoist"
    elif isinstance(got, str) and c.kind == "block" and "redeclaration" in got:
        # the same temporary in two blocks: one local for both
        rows.append((src, c.func, c.kind, c.line, "error", got))
        names = set(re.findall(r"redeclaration of `[^']*?(\w+)'", build_log(cmds, new, work)))
        new = apply_merge(p, text, c, names)
        if new is None or overlong(text, new):
            return "skip", None, what
        got = build(cmds, new, work)
        c.kind = "merge"
    if isinstance(got, str):
        return "error", None, (what + " " + got).strip()
    if got[:2] == base[:2]:
        return "kept", new, what
    return "needed", None, classify(base, got)


def annotate_needed(p, text, needed):
    """A one-line comment above each needed temporary, block or nested if."""
    at = {}  # line start -> (indent, [names], why) or (indent, None, message)
    for c, why in needed:
        kind = c.kind
        if kind in ("single", "copy"):
            tok = c.decl.a if c.dcl["init"] or c.ndecl == 1 else c.defstmt[0]
            msg = None
        elif kind == "merge":
            tok = c.node[0]
            names = "/".join("`%s`" % n for n in merge_names(p, c))
            msg = "scoped: its own %s (shared, %s)" % (names, why)
        else:
            tok = c.outer.a
            msg = "nested: as one `&&` test, %s" % why
        pos = p.toks[tok].start
        a, _ = line_bounds(text, pos)
        indent = text[a:pos]
        if indent.strip():
            continue
        if msg is None:
            prev = at.get(a)
            if prev and prev[1] is not None and prev[2] == why:
                prev[1].append(c.name)
                continue
            at[a] = (indent, [c.name], why)
        else:
            at[a] = (indent, None, msg)
    # adjacent declarations needed for the same reason share one comment
    starts = sorted(at)
    merged = {}
    for a in starts:
        indent, names, why = at[a]
        prev = max((s for s in merged if s < a), default=None)
        if names is not None and prev is not None:
            pi, pn, pw = merged[prev]
            gap = text[prev:a]
            if pn is not None and pw == why and pi == indent and gap.count("\n") == len(pn) and "/*" not in gap:
                pn.extend(names)
                continue
        merged[a] = (indent, names, why)
    ins = []
    for a, (indent, names, why) in merged.items():
        if names is None:
            msg = why
        elif len(names) == 1:
            msg = "`%s` is needed: inlined, %s" % (names[0], why)
        else:
            msg = "%s are needed: inlined, %s" % (", ".join("`%s`" % n for n in names), why)
        lines = textwrap.wrap(msg, width=COLUMNS - len(indent) - 6)
        body = ("\n" + indent + " * ").join(lines)
        ins.append((a, indent + "/* " + body + " */\n"))
    for pos, s in sorted(set(ins), reverse=True):
        text = text[:pos] + s + text[pos:]
    return text


def merge_names(p, c):
    """A block's locals that the function declares elsewhere too."""
    mine = []
    for d in (k for k in c.blk.kids if k.kind == "decl"):
        mine += [p.t(x["name"]) for x in p.declarators(d)[0]]
    other = set()
    for n, _ in walk(c.fn.body):
        if n.kind == "decl" and not (c.blk.a <= n.a < c.blk.b):
            other |= {p.t(x["name"]) for x in p.declarators(n)[0]}
    return [x for x in mine if x in other]


# ---------------------------------------------------------------- main


def sources(paths):
    out = []
    for p in paths or ["src"]:
        if os.path.isfile(os.path.join(ROOT, p)):
            out.append(p)
            continue
        for d, _, fs in os.walk(os.path.join(ROOT, p)):
            for f in fs:
                if f.endswith(".cpp"):
                    out.append(os.path.relpath(os.path.join(d, f), ROOT))
    return sorted(out)


KINDS = ["block", "braces", "caseblk", "nestif", "single", "copy", "dupcase"]


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("paths", nargs="*")
    ap.add_argument("--list", action="store_true", help="every candidate, one per line")
    ap.add_argument("--try", dest="try_", action="store_true", help="try the mechanical candidates")
    ap.add_argument("--kinds", default="block,braces,caseblk,nestif,copy,single")
    ap.add_argument("-j", type=int, default=os.cpu_count())
    ap.add_argument("--log", default="simplify_try.tsv")
    ap.add_argument("--annotate", action="store_true", help="--try: comment what has to stay")
    a = ap.parse_args()
    files = sources(a.paths)
    if a.try_:
        kinds = set(a.kinds.split(","))
        with ThreadPoolExecutor(a.j) as ex, open(a.log, "w") as log:
            log.write("file\tfunction\tkind\tline\tresult\tlocal\n")
            for rows in ex.map(lambda f: try_file(f, kinds, a.annotate), files):
                for r in rows:
                    log.write("\t".join(map(str, r)) + "\n")
                log.flush()
                kept = sum(1 for r in rows if r[4] == "kept")
                print("%-50s %3d tried, %3d kept" % (rows[0][0] if rows else "?", len(rows), kept), flush=True)
        return
    total = Counter()
    doc = Counter()
    bydir = defaultdict(Counter)
    funcs = defaultdict(set)
    for f in files:
        with open(os.path.join(ROOT, f)) as fh:
            _, cands = scan(fh.read(), f)
        d = os.path.dirname(f)
        for c in cands:
            if a.list:
                extra = getattr(c, "name", "") or ",".join(map(str, getattr(c, "lines", [])))
                print("%s:%d\t%s\t%s\t%s%s" % (f, c.line, c.kind, c.func, extra, "\tdocumented" if c.doc else ""))
            (doc if c.doc else total)[c.kind] += 1
            if not c.doc:
                bydir[d][c.kind] += 1
                funcs[c.kind].add((f, c.func))
    if a.list:
        return
    print("%-10s %8s %10s %9s" % ("kind", "open", "documented", "functions"))
    for k in sorted(KINDS, key=lambda k: -total[k]):
        print("%-10s %8d %10d %9d" % (k, total[k], doc[k], len(funcs[k])))
    print()
    print("%-28s " % "directory" + " ".join("%7s" % k for k in KINDS) + "   total")
    for d in sorted(bydir, key=lambda d: -sum(bydir[d].values())):
        print("%-28s " % d + " ".join("%7d" % bydir[d][k] for k in KINDS) + "   %5d" % sum(bydir[d].values()))


if __name__ == "__main__":
    main()
