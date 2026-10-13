#!/usr/bin/env python3
"""`tools/magic_numbers.py --fixed`: the literals and shifts on values that
hold fixed-point quantities (#916, docs/fixed_point.md).

Scans the game code (src/ without src/data/, and include/) for:

  literal  an integer literal assigned to, compared with, added to or
           subtracted from a value (`velX = 0x200`, `y >= 0x800`,
           `x + 0x600`, `y -= 0x100`), or clamped against it
           (`LIMIT_MAX(vx, 0x200)`, `CLAMP(x, lo, hi)`, `MIN`/`MAX`)
  shift    `x >> N`, `x << N`, `x >>= N`, `x <<= N` for N = 4, 8, 12, 16,
           and `x / 256`, `x * 256`: a conversion between formats
  mask     `x & 0xFF` (the fraction) and `x & ~0xFF` / `x & 0xFFFFFF00`
           (the whole part) on a value
  spelled  a site that already says what it is: INT_TO_Q8(...),
           Q8_TO_INT(...), Q8(...), Q8_FRAC(...) and the other
           math_util.h helpers, and the named tuning constants (an
           object-like #define in include/ whose value is spelled with
           one of them: PLAYER_GRAVITY, ...)

and, in the speed-ramp tables of src/data (`struct speed_ramp`: start,
step, target; docs/fixed_point.md), every record's three values
(kind `table`).

Each site is put under the value it applies to (the "thing"): the class
that declares the member and the member path (`Entity::x`,
`MovingSprite::rampX.step`), or `local <name> (<function>)`, found with
magic_bits.py's resolver. A thing has a role from its name (ROLES:
position, velocity, acceleration, ramp, camera, affine, angle, subframe)
and a format from the evidence in the code: a thing that is shifted
right by 8 (`x >> 8`, `Q8_TO_INT(x)`) or assigned an INT_TO_Q8 is Q8; a
thing assigned a conversion's result (`px = Q8_TO_INT(x)`,
`px = x >> 8`) holds whole pixels. Then each site is classified:

  fixed     the value is fixed point: spell the literal with INT_TO_Q8(n)
            when it is a whole number of units, Q8(d) / Q8_FRAC(n, f)
            when it isn't, and the shift with Q8_TO_INT (QN_TO_INT for
            the other formats)
  integer   a plain integer that happens to sit next to a fixed-point
            name (a pixel coordinate, a counter, a tile position)
  bitfield  bits: OAM attributes, packed halfwords, nibbles, palette
            banks (masks other than the Q8 whole/fraction ones)

with the format (Q8, Q4, Q12, Q16, angle: 256 steps per turn) and why.
The counts of `fixed` sites that aren't spelled go down as sites are
converted; `spelled` sites are counted to show the progress.

Usage:
  tools/magic_numbers.py --fixed                  every site: file:line kind
                                                  category format thing value
  tools/magic_numbers.py --fixed --show           ... with the source line
  tools/magic_numbers.py --fixed --report [OUT]   Markdown: counts per format,
                                                  field and directory, the
                                                  raw fixed-point sites
  tools/magic_numbers.py --fixed --json           every site as JSON
  tools/magic_numbers.py --fixed --path P         only files under P
  tools/magic_numbers.py --fixed --field RE       only things matching RE
  tools/magic_numbers.py --fixed --category C     only category C (fixed,
                                                  integer, bitfield, spelled)
  tools/magic_numbers.py --fixed --values         per thing, each raw value
                                                  and its count (the tuning
                                                  constants to name)
"""

import collections
import json
import os
import re
import sys

import magic_bits as mb
import magic_numbers as mn

ROOT = mn.ROOT
CATEGORIES = ("fixed", "integer", "bitfield", "spelled")
KINDS = ("literal", "shift", "mask", "spelled", "table")

Site = collections.namedtuple(
    "Site", "file line kind op value literal thing role fmt category reason func source"
)

LITERAL = r"(0[xX][0-9a-fA-F]+|\d+)[uUlL]*"

# The roles, from the last name of the thing (first match wins). A name
# that matches none isn't a candidate: its literals aren't listed.
ROLES = [
    ("angle", r"(?i)^(?:angle\w*|\w*Angle|rot|rotation|heading|theta|phase)$"),
    ("affine", r"^(?:scale\w*|\w*Scale[XY]?|zoom\w*|\w*Zoom|p[abcd]|affine\w*)$"),
    ("ramp", r"^ramp[XY]?$"),
    ("acceleration", r"^(?:acc\w*|\w*Acc[XYZ]?|a[xyz]|grav\w*|\w*Gravity|accel\w*|\w*Accel\w*|friction\w*)$"),
    (
        "velocity",
        r"^(?:v[xyz]|vel\w*|\w*Vel[XYZ]?|velocity|speed\w*|\w*Speed[XYZ]?|d[xyz]|fall\w*|\w*Fall\w*|jump\w*|\w*Jump\w*|knockback\w*)$",
    ),
    ("camera", r"^(?:cam\w*|camera\w*|\w*Camera\w*|scroll\w*|\w*Scroll[XY]?|look\w*|\w*LookAhead\w*)$"),
    ("subframe", r"^(?:animTime|\w*SubFrame\w*|subFrame\w*)$"),
    (
        "position",
        r"^(?:[xyz]|[xyz][0-9]|pos|pos[XYZ]|\w*Pos[XYZ]?|[a-z]+[XYZ]|[cpt][xyz]|[xyz]Lo|[xyz]Hi|left|right|top|bottom|q)$",
    ),
]
ROLES = [(r, re.compile(p)) for r, p in ROLES]
# A thing whose role says bits, whatever the name looks like.
BITS_NAME = re.compile(
    r"(?i)(attr|oam|tile|pal|palette|nibble|packed|flags?|bits?|mask|color|colour|rgb|id|index|idx|kind|cell|key|word|hi|lo|seed|addr|ptr|vram|bank|byte)\w*$|^(?:hw|attr\d)$"
)
# Names that hold whole pixels or counts though they look like positions.
INT_NAME = re.compile(
    r"(?i)^(?:s[xy]|screen[XY]\w*|px|py|(?:mirror|map|grid|cursor|page|text|menu|icon|glyph|font|digit|slot|sub)\w*|pixel\w*|tile[XY]\w*|col\w*|row\w*|w|h|width|height|\w*Px|\w*Pixels?|\w*Tiles?|mx|my|n|i|j|k|count\w*|\w*Count)$"
)
# Shared structs (a point, a box) hold pixels in one place and Q8 in
# another: their sites are classified by value, not by the evidence.
GENERIC = re.compile(r"(?:vec2|vec3|aabb|hitbox\w*|oam_attrs|bresenham_line)::")
# A byte can't hold a Q8 position or speed: a flag, a count, an index.
BYTE_TYPES = ("u8", "s8", "char", "bool", "bool8")
# The formats a shift of N bits converts.
SHIFT_BITS = (4, 8, 12, 16)
# The directories whose positions are screen pixels (2D layouts), unless
# the code shows they are Q8.
UI = re.compile(r"src/(menus|save|hud|text|cutscene)/")

# The spelled forms: math_util.h's fixed-point helpers (and the named
# constants, CONSTANTS, loaded from include/).
SPELLED = {
    "INT_TO_Q8": "Q8",
    "Q8_TO_INT": "Q8",
    "Q8_TO_INT_INPLACE": "Q8",
    "Q8_MUL": "Q8",
    "Q8_DIV": "Q8",
    "Q8": "Q8",
    "Q8_FRAC": "Q8",
    "INT_TO_Q4": "Q4",
    "Q4_TO_INT": "Q4",
    "INT_TO_Q12": "Q12",
    "Q12_TO_INT": "Q12",
    "Q12_MUL": "Q12",
    "INT_TO_Q16": "Q16",
    "Q16_TO_INT": "Q16",
}
# The tuning constants (fixed.h), name -> format, filled by load_constants.
CONSTANTS = {}


def load_constants():
    """The named fixed-point constants: every object-like #define in
    include/ whose value is spelled with a fixed-point helper
    (`#define PLAYER_GRAVITY Q8_FRAC(0, 0x33)`)."""
    helper = re.compile(r"\b(?:INT_TO_(Q\d+)|(Q\d+)(?:_FRAC)?)\s*\(")
    for dirpath, _, filenames in os.walk(os.path.join(ROOT, "include")):
        for fn in filenames:
            if not fn.endswith((".h", ".hpp")) or fn == "math_util.h":
                continue
            text = open(os.path.join(dirpath, fn), encoding="utf-8", errors="replace").read()
            for m in re.finditer(r"^#define\s+([A-Z][A-Z0-9_]+)[ \t]+(.*)$", text, re.M):
                fm = helper.search(m.group(2))
                if fm:
                    CONSTANTS[m.group(1)] = fm.group(1) or fm.group(2)


def role_of(name):
    for role, pat in ROLES:
        if pat.match(name):
            return role
    return None


def last_name(thing_expr):
    """The last identifier of a chain (`p->rampX.step` -> `step`, with
    `rampX` kept for the ramp's fields)."""
    names = re.findall(r"[A-Za-z_]\w*", re.sub(r"\[[^\]]*\]|\([^()]*\)", "", thing_expr))
    if not names:
        return None
    if names[-1] in ("start", "step", "target") and len(names) > 1 and re.match(r"ramp|speed_ramp", names[-2]):
        return "ramp"
    return names[-1]


class FixedScan(mb.BitScan):
    def __init__(self, path, rel, model):
        super().__init__(path, rel, model)
        self.sites = []
        self.evidence = []  # (thing, format or "int")

    def decl_type(self, field, at):
        """The declared type of a thing: a member's (the model's) or a
        local's (its declaration in the function), or None."""
        if field.startswith("local "):
            name = field.split()[1]
            f = self.func_at(at)
            lo = f.head_start if f else 0
            best = None
            decl = re.compile(r"\b(\w+)\s*\**\s*\b" + re.escape(name) + r"\s*(?=[=;,)\[])")
            for m in decl.finditer(self.text, lo, at):
                if m.group(1) not in mn.KEYWORDS or m.group(1) in ("int", "char", "short", "long"):
                    best = m.group(1)
            return best
        if "::" in field and not field.startswith(("global ", "expr ")):
            owner, path = field.split(" [")[0].split("::", 1)
            names = re.findall(r"\w+", path.replace("()", ""))
            t, ft = owner, None
            for n in names:
                ft = self.model.field_type(t, n)
                if not ft:
                    return None
                t = ft[1]
            return ft and ft[1]
        return None

    def thing_at(self, start, end):
        """(thing key, name, role) of the expression text[start:end]."""
        field, expr = self.field_of(start, end)
        flat = re.sub(r"\s+", "", mb.strip_expr(self.text[start:end]))
        if re.match(r"(?:sizeof|COMPILE_TIME_ASSERT)\b", flat):
            return f"expr {flat[:48]}", None, None
        if re.match(r"Q\d+_TO_INT\(|.*>>\d+\)*$", flat):
            return f"expr {flat[:48]}", "converted", "converted"  # whole units
        if field and not field.endswith("()"):
            key = field.split(" (")[0].replace("local ", "").replace("global ", "")
            name = last_name(key.replace("::", "."))
            if name:
                role = role_of(name)
                if role and role != "angle" and self.decl_type(field, start) in BYTE_TYPES:
                    role = "byte"
                return field, name, role
        # an expression: the first name in it with a role
        for n in re.findall(r"[A-Za-z_]\w*", re.sub(r"\b(?:s32|u32|s16|u16|s8|u8|int)\b", "", flat)):
            if n in mn.KEYWORDS:
                continue
            r = role_of(n)
            if r:
                return f"expr {flat[:48]}", n, r
        return (field or f"expr {flat[:48]}"), None, None

    def record(self, kind, op, value, literal, at, thing, name, role, fmt_hint=None, spelled=False):
        f = self.func_at(at)
        fname = f and ((f.name + "::" if f.name and f.fname != f.name else "") + (f.fname or "?"))
        line = mn.line_of(self.text, at)
        self.sites.append(
            dict(
                file=self.rel,
                line=line,
                kind=kind,
                op=op,
                value=value,
                literal=literal,
                thing=thing,
                name=name,
                role=role,
                fmt_hint=fmt_hint,
                spelled=spelled,
                func=fname,
                source=self.src_lines[line - 1].strip(),
            )
        )

    # -- the scans

    OP = re.compile(
        r"(==|!=|<=|>=|\+=|-=|(?<![<>=!+\-*/%&|^])=(?!=)|(?<![<>-])<(?![<=])|(?<![-<>])>(?![>=])"
        r"|(?<![+])\+(?![+=])|(?<![-])-(?![-=>]))"
        r"\s*(-?)\s*" + LITERAL + r"(?![\w.])"
    )

    def scan_literals(self):
        text = self.text
        for m in self.OP.finditer(text):
            op, neg, lit = m.group(1), m.group(2), m.group(3)
            prev = text[: m.start()].rstrip()
            if not prev:
                continue
            if op in ("+", "-") and not (prev[-1].isalnum() or prev[-1] in "_)]"):
                continue  # a unary sign
            if prev.endswith(("return", "case")) or prev[-1] in "(,?:":
                continue
            rest = text[m.end() :].lstrip()
            if rest.startswith(("*", "/", "<<", ">>", "%", "&", "|", "^", "(")):
                continue  # the literal is a factor, not an operand
            if op == "=" and re.search(r"\b(?:enum|#define)\b", text[text.rfind("\n", 0, m.start()) : m.start()]):
                continue
            s = mb.expr_start(text, m.start())
            if s >= m.start():
                continue
            v = mn.parse_value(lit)
            if neg:
                v = -v
            if v == 0:
                continue
            thing, name, role = self.thing_at(s, m.start())
            if not role:
                continue
            self.record("literal", op, v, ("-" if neg else "") + lit, m.start(3), thing, name, role)
        # clamps: LIMIT_MAX(x, K), CLAMP(x, lo, hi), MIN/MAX(x, K)
        for m in re.finditer(r"\b(LIMIT_MAX|LIMIT_MIN|CLAMP_MIN|CLAMP_MAX|CLAMP|MIN|MAX)\s*\(", text):
            args = mn.split_args(text, m.end() - 1)
            if not args or len(args) < 2:
                continue
            (s0, a0) = args[0]
            thing, name, role = self.thing_at(s0, s0 + len(a0))
            if not role:
                continue
            for s, a in args[1:]:
                lm = re.fullmatch(r"\s*(-?)\s*" + LITERAL + r"\s*", a)
                if lm:
                    v = mn.parse_value(lm.group(2)) * (-1 if lm.group(1) else 1)
                    if v:
                        self.record("literal", m.group(1), v, lm.group(1) + lm.group(2), s, thing, name, role)

    # Calls that take a position or a speed in Q8 (outside the 2D layouts,
    # where SetPos/Move take pixels): a whole literal argument is one.
    POS_CALL = re.compile(r"\b(SetPos|SetDest|PlaceAt|SetVelocity|SetSpeed|Spawn\w*|Create\w*|MoveTo|SetOffset)\s*\(")

    def scan_args(self):
        if UI.search(self.rel):
            return
        text = self.text
        for m in self.POS_CALL.finditer(text):
            if re.search(r"\b(?:void|s32|u32|static|class)\s+(?:\w+::)?$", text[: m.start()]):
                continue  # a declaration or a definition
            for s, a in mn.split_args(text, m.end() - 1):
                lm = re.fullmatch(r"\s*(-?)\s*" + LITERAL + r"\s*", a)
                if not lm:
                    continue
                v = mn.parse_value(lm.group(2)) * (-1 if lm.group(1) else 1)
                if abs(v) < 0x100 or abs(v) % 0x10:
                    continue  # a kind, an id, a count, a flag
                self.record("literal", m.group(1) + "()", v, lm.group(1) + lm.group(2), s, f"arg {m.group(1)}()",
                            m.group(1), "position")

    SHIFT = re.compile(r"(>>=|<<=|>>|<<)\s*(\d+)(?![\w.])|(?<![/*])([/*])\s*(256|0x100)(?![\w.])")

    def scan_shifts(self):
        text = self.text
        for m in self.SHIFT.finditer(text):
            if m.group(1):
                op, n = m.group(1), int(m.group(2))
                if n not in SHIFT_BITS:
                    continue
            else:
                op, n = m.group(3), 8
            if text[: m.start()].rstrip().endswith("operator"):
                continue
            s = mb.expr_start(text, m.start())
            if s >= m.start():
                continue
            thing, name, role = self.thing_at(s, m.start())
            self.record("shift", op, n, m.group(0).strip(), m.start(), thing, name, role, fmt_hint=f"Q{n}")
            # evidence: `x >> 8` reads a Q8 x
            if op in (">>", ">>=", "/") and role:
                self.evidence.append((thing, f"Q{n}"))

    MASK = re.compile(r"(?<![&])&\s*(~\s*0[xX]0*[fF][fF]|0[xX][fF]{6}00|0[xX]0*[fF][fF])(?![\w.])")

    def scan_masks(self):
        text = self.text
        for m in self.MASK.finditer(text):
            prev = text[: m.start()].rstrip()
            if not prev or prev[-1] in "(,=?:":
                continue
            s = mb.expr_start(text, m.start())
            if s >= m.start():
                continue
            thing, name, role = self.thing_at(s, m.start())
            if not role:
                continue
            lit = re.sub(r"\s+", "", m.group(1))
            self.record("mask", "&", 0xFF if lit.lower().endswith("0xff") and "~" not in lit else -0x100, lit,
                        m.start(1), thing, name, role)

    def scan_spelled(self):
        text = self.text
        names = "|".join(sorted(SPELLED, key=len, reverse=True))
        for m in re.finditer(r"\b(" + names + r")\s*\(", text):
            o = m.end() - 1
            c = mn.match_close(text, o)
            arg = text[o + 1 : c]
            macro = m.group(1)
            if macro.endswith("_TO_INT") or macro.endswith("_INPLACE"):
                thing, name, role = self.thing_at(o + 1, c)
                if role:
                    self.evidence.append((thing, SPELLED[macro]))
                    if macro.endswith("_INPLACE"):
                        self.evidence.append((thing, "int"))  # whole units from here on
            else:
                thing, name, role = self.target_of(m.start(), c + 1)
            shown = re.sub(r"\s+", " ", arg.strip())[:40]
            self.record("spelled", macro, None, f"{macro}({shown})", m.start(),
                        thing, name, role, fmt_hint=SPELLED[macro], spelled=True)
            if macro.startswith("INT_TO_Q") and role:
                self.evidence.append((thing, SPELLED[macro]))
            if macro.startswith("INT_TO_Q"):
                # what is converted holds whole units: `INT_TO_Q8(dy - 1)`
                athing, _, arole = self.thing_at(o + 1, c)
                if arole:
                    self.evidence.append((athing, "int"))
        if CONSTANTS:
            for m in re.finditer(r"\b(" + "|".join(CONSTANTS) + r")\b", text):
                thing, name, role = self.target_of(m.start(), m.end())
                self.record("spelled", m.group(1), None, m.group(1), m.start(), thing, name, role,
                            fmt_hint=CONSTANTS[m.group(1)], spelled=True)

    def target_of(self, start, end):
        """The thing a value at text[start:end] is assigned to, compared
        with or added to: the expression before the operator in front of
        it, or the clamped value of a LIMIT_MAX(x, ...)."""
        text = self.text
        prev = text[:start].rstrip()
        prev = re.sub(r"-$", "", prev).rstrip()  # `= -INT_TO_Q8(2)`
        om = re.search(r"(==|!=|<=|>=|\+=|-=|=|<|>|\+|-)$", prev)
        if om:
            s = mb.expr_start(text, len(prev) - len(om.group(1)))
            if s < len(prev) - len(om.group(1)):
                return self.thing_at(s, len(prev) - len(om.group(1)))
        cm = re.search(r"\b(?:LIMIT_MAX|LIMIT_MIN|CLAMP_MIN|CLAMP_MAX|CLAMP|MIN|MAX)\s*\(([^,()]*),[^;]*$", prev)
        if cm:
            return self.thing_at(cm.start(1), cm.end(1))
        return "expr", None, None

    def scan_conversions(self):
        """`T = Q8_TO_INT(...)` / `T = ... >> 8;`: T holds whole units.
        `T = INT_TO_Q8(...)` / `T = ... << 8;`: T is Q8."""
        text = self.text
        for m in re.finditer(r"(?<![<>=!+\-*/%&|^])=(?!=)\s*([^;{}]*);", text):
            rhs = m.group(1).strip()
            s = mb.expr_start(text, m.start())
            if s >= m.start():
                continue
            fmt = None
            if re.search(r"\bQ\d+_TO_INT\s*\(", rhs) and not re.search(r"\bINT_TO_Q|<<", rhs):
                fmt = "int"  # `px = Q8_TO_INT(x)`, `dy = ABS(Q8_TO_INT(a) - py)`
            elif re.search(r">>\s*(4|8|12|16)\s*$", rhs) and not re.search(r"&|<<", rhs):
                fmt = "int"
            q = re.match(r"INT_TO_(Q\d+)\s*\(", rhs)
            if q:
                fmt = q.group(1)
            if fmt:
                thing, name, role = self.thing_at(s, m.start())
                if role:
                    self.evidence.append((thing, fmt))


def source_files():
    return mb.source_files()


def scan_tables():
    """Every `struct speed_ramp` record in src/data: start, step, target."""
    out = []
    for path in mn.source_files():
        rel = os.path.relpath(path, ROOT)
        if not rel.startswith(os.path.join("src", "data")):
            continue
        raw = open(path, encoding="utf-8", errors="replace").read()
        text = mn.strip_comments(raw)
        lines = raw.split("\n")
        for m in re.finditer(r"\bstruct\s+speed_ramp\s+(\w+)\s*(\[[^\]]*\])?\s*=\s*\{", text):
            end = mn.match_close(text, m.end() - 1)
            body_start = m.end()
            for r in re.finditer(r"\{([^{}]*)\}", text[body_start:end]):
                vals = [v.strip() for v in r.group(1).split(",") if v.strip()]
                for field, v in zip(("start", "step", "target"), vals):
                    off = body_start + r.start(1)
                    line = mn.line_of(text, off)
                    lm = re.fullmatch(r"-?\s*" + LITERAL, v)
                    spelled = not lm
                    value = mn.parse_value(v) if lm else None
                    out.append(
                        Site(
                            rel,
                            line,
                            "table",
                            field,
                            value,
                            v,
                            f"speed_ramp::{field} ({m.group(1)})",
                            "ramp",
                            "Q8",
                            "spelled" if spelled else ("fixed" if value else "integer"),
                            "a speed ramp record (Q8 px/frame, px/frame^2)" if not spelled else "spelled",
                            m.group(1),
                            lines[line - 1].strip(),
                        )
                    )
    return out


def classify(site, fmts, model_types):
    """(category, format, reason) of a raw site dict."""
    thing, role, name = site["thing"], site["role"], site["name"] or ""
    ev = fmts.get(thing, collections.Counter())
    if GENERIC.match(thing or ""):
        ev = collections.Counter()  # a shared struct: its uses differ
    qs = {k: v for k, v in ev.items() if k != "int"}
    fmt = max(qs, key=qs.get) if qs else None
    if site["spelled"]:
        return "spelled", site["fmt_hint"] or fmt or "Q8", "already spelled"
    kind = site["kind"]
    src = site["source"]
    if role == "angle":
        if kind == "shift":
            return "integer", "angle", "an angle (256 steps per turn) scaled for a table"
        return "integer", "angle", "an angle: 256 steps per turn, not fixed point"
    if role == "byte":
        return "integer", "-", "a byte (a flag, a count or an index), not fixed point"
    if role == "converted":
        return "integer", "-", "a value already converted to whole units (Q8_TO_INT, >> 8)"
    if kind == "shift":
        n = site["value"]
        if BITS_NAME.search(name or "") or re.search(r"OAM_|attr|palette|<< 16\) >> 16|pieces\[|nibble|tile|REG_", src):
            return "bitfield", "-", "bit packing (OAM, palette bank, nibbles, halves)"
        if not role:
            return "integer", "-", "not a fixed-point value (an index, a byte, a scale factor)"
        if ev.get("int") and not qs and site["op"] in ("<<", "<<=", "*"):
            return "fixed", f"Q{n}", "whole units to fixed point"
        return "fixed", f"Q{n}", f"a {role} converted between whole units and Q{n}"
    if kind == "mask":
        if site["value"] == 0xFF:
            return "fixed", fmt or "Q8", "the fraction of a Q8 value"
        return "fixed", fmt or "Q8", "the whole part of a Q8 value (floor)"
    # a literal
    if thing.startswith("arg "):
        return "fixed", "Q8", "a whole Q8 position or speed passed to a call"
    if BITS_NAME.search(name):
        return "bitfield", "-", "a bit or id field"
    if INT_NAME.match(name):
        return "integer", "-", "a pixel, tile or count"
    if ev.get("int") and not qs:
        return "integer", "-", "holds whole units (a Q8_TO_INT / >> 8 result, or an INT_TO_Q8 argument)"
    if ev.get("int") and abs(site["value"]) < 0x100:
        return "integer", "-", "a small offset to a value converted in place (Q8_TO_INT_INPLACE)"
    if role == "position" and UI.search(site["file"]) and abs(site["value"]) < 0x100:
        return "integer", "-", "a 2D layout position (screen pixels)"
    if fmt:
        return "fixed", fmt, f"a {role} (shifted by {fmt[1:]} where it is read)"
    if role == "position" and UI.search(site["file"]):
        return "integer", "-", "a 2D layout position (screen pixels)"
    v = abs(site["value"])
    if role in ("velocity", "acceleration", "ramp", "camera", "subframe", "affine"):
        return "fixed", "Q8", f"a {role} (Q8 by role)"
    if role == "position":
        if v >= 0x100 and v % 0x40 == 0:
            return "fixed", "Q8", "a position (Q8 by role and value)"
        if v < 0x100:
            return "integer", "-", "a small position offset or a pixel (not Q8 by value)"
        return "fixed", "Q8", "a position (Q8 by role)"
    return "integer", "-", "not a fixed-point value"


def scan(paths=None):
    load_constants()
    files = mn.source_files()
    model = mn.Model()
    model.load(files)
    mb.load_unions(files)
    raw, evidence = [], []
    for path, rel in source_files():
        s = FixedScan(path, rel, model)
        if not s.is_cxx:
            continue
        s.scan_literals()
        s.scan_args()
        s.scan_shifts()
        s.scan_masks()
        s.scan_spelled()
        s.scan_conversions()
        evidence += s.evidence
        if paths and not any(rel == p or rel.startswith(p.rstrip("/") + "/") for p in paths):
            continue
        raw += s.sites
    fmts = collections.defaultdict(collections.Counter)
    for thing, f in evidence:
        fmts[thing][f] += 1
    seen, out = set(), []
    for r in raw:
        k = (r["file"], r["line"], r["kind"], r["literal"], r["thing"], r["op"])
        if k in seen:
            continue
        seen.add(k)
        cat, fmt, reason = classify(r, fmts, None)
        out.append(
            Site(r["file"], r["line"], r["kind"], r["op"], r["value"], r["literal"], r["thing"], r["role"], fmt, cat,
                 reason, r["func"], r["source"])
        )
    tables = scan_tables()
    if paths:
        tables = [t for t in tables if any(t.file == p or t.file.startswith(p.rstrip("/") + "/") for p in paths)]
    out.sort(key=lambda s: (s.file, s.line))
    return out + tables


def suggestion(s):
    """How a raw fixed-point literal is spelled (docs/fixed_point.md)."""
    if s.category != "fixed" or s.kind not in ("literal", "table") or s.value is None:
        return ""
    bits = int(s.fmt[1:]) if s.fmt and s.fmt[1:].isdigit() else 8
    one = 1 << bits
    v = s.value
    sign = "-" if v < 0 else ""
    a = abs(v)
    if a % one == 0:
        return f"{sign}INT_TO_Q{bits}({a // one})"
    if bits == 8 and a % 0x10 == 0:
        d = a / one
        return f"{sign}Q8({d:g})"
    return f"{sign}Q{bits}_FRAC({a // one}, {a % one:#x})"


def report(sites, out):
    w = out.write
    w("# Fixed-point literals and shifts (#916)\n\n")
    cats = collections.Counter(s.category for s in sites)
    w(f"{len(sites)} sites: " + ", ".join(f"{c} {cats[c]}" for c in CATEGORIES if cats[c]) + "\n\n")
    w("## Per kind\n\n| kind | " + " | ".join(CATEGORIES) + " | total |\n|---|" + "---:|" * (len(CATEGORIES) + 1) + "\n")
    for k in KINDS:
        c = collections.Counter(s.category for s in sites if s.kind == k)
        if c:
            w(f"| {k} | " + " | ".join(str(c[x]) for x in CATEGORIES) + f" | {sum(c.values())} |\n")
    w("\n## Per format (fixed and spelled)\n\n| format | raw | spelled |\n|---|---:|---:|\n")
    fm = collections.defaultdict(collections.Counter)
    for s in sites:
        if s.category in ("fixed", "spelled"):
            fm[s.fmt][s.category] += 1
    for f in sorted(fm, key=lambda f: -sum(fm[f].values())):
        w(f"| {f} | {fm[f]['fixed']} | {fm[f]['spelled']} |\n")
    w("\n## Per role (fixed sites)\n\n| role | literal | shift | mask | table | spelled |\n|---|---:|---:|---:|---:|---:|\n")
    ro = collections.defaultdict(collections.Counter)
    for s in sites:
        if s.category == "fixed":
            ro[s.role or "-"][s.kind] += 1
        elif s.category == "spelled":
            ro[s.role or "-"]["spelled"] += 1
    for r in sorted(ro, key=lambda r: -sum(ro[r].values())):
        c = ro[r]
        w(f"| {r} | {c['literal']} | {c['shift']} | {c['mask']} | {c['table']} | {c['spelled']} |\n")
    w("\n## Per directory\n\n| directory | " + " | ".join(CATEGORIES) + " | total |\n|---|" + "---:|" * (len(CATEGORIES) + 1) + "\n")
    dirs = collections.defaultdict(collections.Counter)
    for s in sites:
        dirs[mn.directory(s.file)][s.category] += 1
    for d in sorted(dirs):
        c = dirs[d]
        w(f"| {d} | " + " | ".join(str(c[x]) for x in CATEGORIES) + f" | {sum(c.values())} |\n")
    w("\n## Per field (raw fixed-point sites, top 60)\n\n| field | role | format | sites | values |\n|---|---|---|---:|---|\n")
    things = collections.defaultdict(list)
    for s in sites:
        if s.category == "fixed":
            things[s.thing].append(s)
    for t, ss in sorted(things.items(), key=lambda kv: (-len(kv[1]), kv[0]))[:60]:
        vals = collections.Counter(s.literal for s in ss if s.kind in ("literal", "table"))
        cell = ", ".join(f"`{v}`" + (f" x{n}" if n > 1 else "") for v, n in vals.most_common(6))
        fmts = "/".join(sorted({s.fmt for s in ss}))
        w(f"| `{t}` | {ss[0].role} | {fmts} | {len(ss)} | {cell} |\n")
    w("\n## Raw fixed-point sites\n\n")
    by = collections.defaultdict(list)
    for s in sites:
        if s.category == "fixed" and s.kind != "table":
            by[mn.directory(s.file)].append(s)
    for d in sorted(by):
        w(f"### {d} ({len(by[d])})\n\n")
        for s in by[d]:
            hint = suggestion(s)
            hint = f" -> `{hint}`" if hint else ""
            w(f"- `{s.file}:{s.line}` {s.kind} `{s.literal}` {s.fmt} on `{s.thing}`{hint}: `{s.source}`\n")
        w("\n")


def run(args):
    sites = scan(args.path)
    if getattr(args, "field", None):
        pat = re.compile(args.field)
        sites = [s for s in sites if pat.search(s.thing or "")]
    if getattr(args, "category", None):
        sites = [s for s in sites if s.category in args.category]
    if args.json:
        json.dump([dict(s._asdict(), suggestion=suggestion(s)) for s in sites], sys.stdout, indent=1)
        sys.stdout.write("\n")
        return 0
    if args.report:
        if args.report == "-":
            report(sites, sys.stdout)
        else:
            with open(args.report, "w") as f:
                report(sites, f)
        return 0
    if getattr(args, "values", False):
        things = collections.defaultdict(collections.Counter)
        for s in sites:
            if s.category == "fixed" and s.kind in ("literal", "table"):
                things[s.thing][s.value] += 1
        for t, c in sorted(things.items(), key=lambda kv: -sum(kv[1].values())):
            print(f"{t}: " + ", ".join(f"{v:#x}: {n}" for v, n in sorted(c.items(), key=lambda x: (-x[1], x[0]))))
        return 0
    for s in sites:
        hint = suggestion(s)
        print(
            f"{s.file}:{s.line}: {s.kind} {s.category} {s.fmt} {s.thing} {s.literal}"
            + (f" -> {hint}" if hint else "")
            + (f" [{s.reason}]" if s.reason else "")
            + (f"\n    {s.source}" if args.show else "")
        )
    c = collections.Counter(s.category for s in sites)
    print(f"{len(sites)} sites: " + ", ".join(f"{k} {c[k]}" for k in CATEGORIES if c[k]), file=sys.stderr)
    return 0


if __name__ == "__main__":
    import argparse

    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--show", action="store_true")
    ap.add_argument("--report", nargs="?", const="-")
    ap.add_argument("--json", action="store_true")
    ap.add_argument("--values", action="store_true")
    ap.add_argument("--path", action="append")
    ap.add_argument("--field")
    ap.add_argument("--category", action="append", choices=CATEGORIES)
    sys.exit(run(ap.parse_args()))
