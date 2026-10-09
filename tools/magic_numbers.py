#!/usr/bin/env python3
"""List the magic numbers left at known call sites and fields (#655).

Scans the sources under src/ and include/ (the game's C++, .cpp/.hpp,
and the C that is left, .c/.h) for integer literals in places where the
game has named values: sound and song IDs, level-flag bits, mask levels,
event IDs, action-controller states, entity, crate, enemy and attack
kinds, actor-category exit statuses, level ids, room kinds, boss ids,
actor categories and `& 0xNN` flag tests. Each hit is put under one
topic. A value that is already a name (`SFX_CRATE_BREAK`,
`LEVEL_FLAG_CRATE_GEM`, ...) isn't a literal, so it isn't listed: the
counts go down as literals are replaced.

When a constants header (include/constants/, or one the build generates
into build/include/constants/, e.g. songs.h and sfx.h: run make first)
already has a define with the literal's value and the site's prefix, the
listing shows it (`-> NAME`), so the remaining sites of a topic that has
a header can be finished off.

C and C++. The call rules (CALLS) are written for the C functions:
`PlaySfx(ctx, id, volume)`, `SetActionCtrlMode(ctrl, state)`,
`CreateCrate(id, x, y, slot, kind)`. In C++ most of them are methods;
cxx_symbols.txt maps each C name to its `Class::Method`, and the
argument index drops `this` unless the method is static
(`gAudioContext->PlaySfx(SFX_X, 0x100)`: the ID is argument 0). A method
call is `obj->Method(...)`, `obj.Method(...)`, `Class::Method(...)` or,
inside one of the class's methods, a bare `Method(...)`. The class of
`obj` comes from the source: a parameter or local declared in the
function, a member of the method's class or its bases, an extern
global, a cast, then `->member` chains (`target->mover`). The method
called is the first one up the class's bases, and the rule is that
method's. A field rule (CXX_FIELDS) names the class too: `state` is an
action state on an ActionCtrl, `kind` is a crate kind on a Crate and an
enemy kind on an EnemyCtrl. Call results (CXX_RESULTS, `GetBossIndex()`)
and table elements (CXX_TABLES) are values like fields. A literal is a
hit where such a value is compared with it (`==`, `<`, ...), assigned
it, switched on (the `case` labels), range-checked (`(u32)(x - N) <= M`)
or clamped (`LIMIT_MAX(x, N)`), and where a ruled function returns it.

Values are followed through a function: a local whose every assignment
is one kind of ruled value or a literal (`u8 k = kind; ... k == 0xA`),
a local passed as a ruled argument (`id = 0x10; PlaySong(id)`), and the
parameters of a ruled method's definition. A function that passes a
parameter straight on as a ruled argument gets the rule for that
parameter (crate_break.cpp's `Sfx(id)`, ActionCtrl::SetModeAnimNow), so
its callers are checked too. The C rules (C_FIELDS, the vtable-slot
macros) are kept for the C files (src/data/, lib/).

Each hit has a confidence:
  high    the receiver's (or the field's owner's) class is known and its
          method or field has the rule; or every class with a method or
          field of that name has the same rule; or a pattern that only
          one thing matches (`GetBossIndex() == N`, a `case` of an event
          handler's `switch (event)`)
  medium  the static class's method or field has no rule, but the object
          is known to be one of a few subclasses, one of which has it
          (MEMBER_CLASSES: `gPlayer->mover->SetMode(0x29)`: the player's
          mover is an ActionCtrl, a SwimCtrl or an InputCtrl)
  low     the receiver's class isn't known and only some of the classes
          with that name have the rule, or only a subclass's override
          has it (`shield->mover->SetMode(2)`: a boss part's controller);
          a value above every name of its prefixes (`level <= 0x1000`);
          and the `& 0xNN` flag tests

On the C++ tree, the high-confidence hits are the sites to name; the
--fix mode only takes those unless --min-confidence says otherwise.

It doesn't run the preprocessor. Comments and strings are blanked out
first, so a literal quoted in a comment isn't counted.

Usage:
  tools/magic_numbers.py                  every hit, file:line topic confidence
                                          value [-> NAME]
  tools/magic_numbers.py --topic T        only topic T (--list-topics)
  tools/magic_numbers.py --min-confidence C   only hits of confidence C or more
  tools/magic_numbers.py --path P         only files under P (repeatable)
  tools/magic_numbers.py --show           ... with the source line
  tools/magic_numbers.py --json           the hits as JSON, for agents
  tools/magic_numbers.py --values         per topic, each value and its count
  tools/magic_numbers.py --report [OUT]   Markdown: counts per topic and
                                          confidence, per directory, samples
  tools/magic_numbers.py --topic T --fix  replace each high-confidence (or
                                          --min-confidence C) literal of topic
                                          T that has exactly one name (then
                                          rebuild and compare)

--sizes is a separate mode (#820, tools/magic_sizes.py): allocation sizes,
copy lengths, index strides and byte offsets that have a sizeof or a
member (from the compiler's debug info, tools/layout_audit.py), and
hardware addresses and register values that include/gba/ names:
  tools/magic_numbers.py --sizes [--show] [--json] [--report [OUT]]
      [--category C ...] [--min-confidence high|medium|low] [--path P ...]
      [--db FILE [--rebuild]] [-j N] [--samples N]
"""

import argparse
import collections
import functools
import json
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SCAN_DIRS = ("src", "include")
C_EXTS = (".c", ".h")
CXX_EXTS = (".cpp", ".hpp")
CXX_SYMBOLS = os.path.join(ROOT, "cxx_symbols.txt")
# The constants headers: hand-written ones, and the ones make generates
# from the data (build/include, e.g. the entity types, crate kinds, songs
# and sound effects).
CONSTANTS_DIRS = (os.path.join(ROOT, "include", "constants"), os.path.join(ROOT, "build", "include", "constants"))

CONFIDENCES = ("high", "medium", "low")

LIT = r"(?:0[xX][0-9a-fA-F]+|[0-9]+)[uUlL]*"
LIT_RE = re.compile(r"^-?\s*" + LIT + r"$")

# Topic -> (description, prefixes of its names in include/constants/).
TOPICS = collections.OrderedDict(
    [
        ("sfx", ("PlaySfx/PlayAmbientSfx/StopSfx sound-effect IDs", ("SFX_",))),
        ("song", ("PlaySong/StartSong song IDs, currentSong/pendingSong", ("SONG_",))),
        ("level_flags", ("levelFlags bits (GetCurrentLevelFlags, level_state.cpp)", ("LEVEL_FLAG_",))),
        ("mask_level", ("maskLevel values (SetMaskLevel, comparisons)", ("MASK_LEVEL_",))),
        ("event", ("event IDs (HandleEvent calls and case labels, an Entity's kind)", ("EVENT_",))),
        ("action_state", ("action-controller states (ActionCtrl::SetMode, its state)", ("ACTION_",))),
        (
            "kind",
            (
                "entity types, crate, enemy and attack kinds",
                ("ENTITY_", "CRATE_KIND_", "ENEMY_KIND_", "ATTACK_KIND_"),
            ),
        ),
        ("category_exit", ("SetActorCategoryExitStatus values", ("CATEGORY_EXIT_",))),
        (
            "level",
            (
                "level ids, room kinds, boss ids, actor categories",
                ("LEVEL_", "ROOM_KIND_", "BOSS_", "CATEGORY_", "CATEGORY_TYPE_"),
            ),
        ),
        ("flag_test", ("`& 0xNN` flag tests (not counted above)", ())),
    ]
)

# Calls: (topic, C function name regex, argument index in C[, options]).
# In C++ the name is mapped to its method through cxx_symbols.txt (and
# the index drops `this` for a non-static method). Options:
#   cxx=False   C only. Ctrl::SetMode (SetCtrlMode) is every
#               controller's (a boss's mode, a swim state); the action
#               states are ActionCtrl::SetMode's, which overrides it.
#   cxx_index=I the argument in C++, when it isn't the C one minus `this`.
#   prefixes=P  the names to suggest, when not all of the topic's.
CALLS = [
    ("sfx", r"PlaySfx", 1),
    ("sfx", r"PlayAmbientSfx", 1),
    ("sfx", r"StopSfx", 1),
    ("song", r"PlaySong", 1),
    ("song", r"StartSong", 1),
    ("mask_level", r"SetMaskLevel", 1),
    ("event", r"NOTIFY", 2),
    # The event methods (this, sender, event, arg). HitEnemy and
    # HitMovingSprite are EnemyCtrl's and MovingSprite's.
    ("event", r"\w*HandleEvent|HitEnemy|HitMovingSprite", 2),
    ("event", r"PhysCall3", 3),
    # Wrappers around the event slot (vtable +0x68).
    ("event", r"CALL_M68H?", 2),
    ("event", r"Call68", 2),
    ("event", r"OBJ_CALL68", 2),
    ("event", r"D18C_CALL68", 1),
    ("event", r"E08C_CALL68", 0),
    ("action_state", r"SetActionCtrlMode", 1),
    ("action_state", r"SetCtrlMode", 1, {"cxx": False}),
    ("action_state", r"SetActionCtrlModeAnim", 1),
    ("action_state", r"StartActionCtrlTornadoSpin", 1),
    ("action_state", r"StartActionCtrlTornadoSpin", 2),
    ("kind", r"CreateCrate", 4, {"prefixes": ("CRATE_KIND_",)}),
    ("kind", r"LevelHasEntityType", 1, {"prefixes": ("ENTITY_",)}),
    ("category_exit", r"SetActorCategoryExitStatus", 0),
]

# Field patterns that name the thing they read, for C and C++: (topic,
# regex with a `v` group for the literal, path prefix the rule is limited
# to or None, prefixes or None for the topic's).
CMP = r"(?:==|!=|<=|>=|<|>|=(?!=))"
FIELDS = [
    ("song", r"\b(?:currentSong|pendingSong)\s*" + CMP + r"\s*(?P<v>" + LIT + r")\b", None, None),
    ("mask_level", r"\bmaskLevel\s*" + CMP + r"\s*(?P<v>" + LIT + r")\b", None, None),
    ("level", r"GetBossIndex\s*\([\w*>-]*\)\s*(?:==|!=)\s*(?P<v>-?" + LIT + r")(?![\w])", None, ("BOSS_",)),
    (
        "level",
        r"(?:CUR_CATEGORY|gActorCategories\[\w+\])\.type\s*(?:==|!=)\s*(?P<v>" + LIT + r")\b",
        None,
        ("CATEGORY_TYPE_",),
    ),
    ("category_exit", r"\bgActorCategoryExitStatus\s*" + CMP + r"\s*(?P<v>" + LIT + r")\b", None, None),
    (
        "level_flags",
        r"Get(?:Current)?LevelFlags\s*\([^;]*?\)\s*(?:&|\|=)\s*(?P<v>" + LIT + r")\b",
        None,
        None,
    ),
]

# C only: the C code's field spellings. `->state` is a common field
# name, so only the player's action controller's is counted (the swim
# and input controllers in src/player/ have their own states). Its
# set-mode method is vtable slot 0x20 (`m20`, `mgr[4]`).
C_FIELDS = [
    (
        "action_state",
        r"(?:->|\.)(?:state|prevState)\s*(?:==|!=)\s*(?P<v>" + LIT + r")\b",
        os.path.join("src", "player", "action_ctrl"),
        None,
    ),
    (
        "action_state",
        r"(?:\bACT_V?CALL1\s*\(\s*\w+\s*,\s*m20\s*,|\bm20\.thisOffset\s*,|\bmgr\[4\]\.delta\s*,)"
        r"\s*(?:\(void \*\))?(?P<v>" + LIT + r")\b",
        os.path.join("src", "player", ""),
        None,
    ),
    # level topic: room kinds (src/level/'s only ->kind tests) and the
    # level id. Before "kind".
    ("level", r"->kind\s*(?:==|!=)\s*(?P<v>" + LIT + r")\b", os.path.join("src", "level", ""), ("ROOM_KIND_",)),
    ("level", r"\bself->level\s*" + CMP + r"\s*(?P<v>" + LIT + r")\b", None, ("LEVEL_",)),
    ("kind", r"(?:->|\.)kind\s*(?:==|!=)\s*(?P<v>" + LIT + r")\b", None, None),
]

# C++ fields: (topic, class, field names, prefixes). A field matches on
# an object of the class or a subclass (`gPlayer->kind`: a Player is an
# Entity), as `obj->field`, `obj.field` or, in the class's methods, a
# bare `field`; compared with (==, !=, <, ...), assigned or tested in a
# `switch (field)`'s case labels. The classes come from the headers'
# comments and the constants headers.
CXX_FIELDS = [
    ("song", "AudioContext", ("currentSong", "pendingSong"), None),
    ("song", "cutscene_slide", ("cue",), None),
    ("sfx", "cutscene_slide", ("sfx",), None),
    ("mask_level", "LevelState", ("maskLevel",), None),
    # Ctrl::state is the mode SetMode sets: an action state on an ActionCtrl.
    ("action_state", "ActionCtrl", ("state", "prevState"), None),
    ("level", "LevelProgress", ("level",), ("LEVEL_",)),
    ("level", "level_room", ("kind",), ("ROOM_KIND_",)),
    ("level", "category_descriptor", ("type",), ("CATEGORY_TYPE_",)),
    ("kind", "Crate", ("kind",), ("CRATE_KIND_",)),
    ("kind", "Crate", ("trialKind",), ("ENTITY_",)),
    ("kind", "EnemyCtrl", ("kind",), ("ENEMY_KIND_",)),
    ("kind", "level_entity", ("type",), ("ENTITY_",)),
    ("kind", "collision_candidate", ("kind",), ("ATTACK_KIND_",)),
    # The kind an object sends to the player's event method on contact
    # (events.h): the player's own is its attack.
    ("event", "Entity", ("kind",), ("EVENT_",)),
]

# C++ call results: (topic, class or None for a free function, function,
# prefixes). Compared, switched on or held in a local like a field; and
# the function's `return N;` statements.
CXX_RESULTS = [
    ("level", "LevelState", "GetBossIndex", ("BOSS_",)),
    # GetCtrlMode: the state, on an ActionCtrl
    ("action_state", "ActionCtrl", "GetMode", None),
    ("category_exit", None, "RunActorCategoryFrame", None),
]

# Tables whose elements are named values: (topic, table, prefixes). An
# element (`gActionCtrlStateAttackKinds[state]`) is a value like a field.
CXX_TABLES = [
    ("kind", "gActionCtrlStateAttackKinds", ("ATTACK_KIND_",)),
]

# Members whose object is one of a few subclasses of the declared class,
# known from the code that sets them: (class, member) -> the classes. A
# call or field rule of one of them is a medium-confidence hit through
# the member (`gPlayer->mover->SetMode(0x29)`, `->mover->state == 0x1E`).
MEMBER_CLASSES = {
    # The player's controller is the room kind's (player.hpp).
    ("Player", "mover"): ("ActionCtrl", "SwimCtrl", "InputCtrl"),
}

# level_state.cpp's crate-gem helpers build the OR mask in a pinned register
# right after taking GetCurrentLevelFlags' slot: a literal assigned within
# this many lines of it is the bit.
LEVEL_FLAGS_WINDOW = 3
LEVEL_FLAGS_ASSIGN = re.compile(r"\bmask\b[^;=]*=\s*(?P<v>" + LIT + r")\s*;")

# Event IDs: the `case` labels of the event handlers' switch on the event
# argument, and (C) the inline calls through the event slot
# (`&obj->vtable->handleEvent`, `PART_METHOD(obj, 0x68)`), whose event is
# the third argument (this, sender, event, arg) of the first call within
# EVENT_SLOT_REACH characters of the slot lookup.
EVENT_HANDLER_RE = re.compile(r"\b(?:\w*HandleEvent|HitEnemy|HitMovingSprite)\s*\(([^;{}]*)\)\s*\{")
CASE_RE = re.compile(r"\bcase\s+(?P<v>" + LIT + r")(?:\s*\.\.\.\s*(?P<w>" + LIT + r"))?\s*:")
EVENT_SLOT_RE = re.compile(r"->handleEvent\b|\bPART_METHOD\s*\(\s*\w+\s*,\s*0x68\s*\)")
EVENT_SLOT_CALL_RE = re.compile(r"\b_call_via_r4\s*\(|->fn\s*\)\s*\(")
EVENT_SLOT_REACH = 400

FLAG_TEST = re.compile(r"(?<![&])&\s*(?P<v>0[xX][0-9a-fA-F]+)[uUlL]*\b")
CALL_RE_CACHE = {}
ANYDECL_CACHE = {}
# A call: `obj->F(`, `obj.F(`, `C::F(` or a bare `F(`.
CALL_ANY_RE = re.compile(r"(?:(->|\.)\s*|(::)\s*|(?<![\w.>:~]))\b([A-Za-z_]\w*)\s*\(")

Hit = collections.namedtuple("Hit", "file line topic literal source offset confidence prefixes why")

KEYWORDS = {
    "return", "case", "else", "goto", "delete", "new", "sizeof", "if", "while", "for", "switch", "do",
    "const", "volatile", "static", "extern", "struct", "class", "union", "enum", "typedef", "this",
    "virtual", "inline", "public", "private", "protected", "operator", "throw", "default",
}  # fmt: skip


def strip_comments(text):
    """Blank comments, strings and char literals, keeping the newlines."""

    def blank(m):
        s = m.group(0)
        if s[0] in "\"'":
            return s[0] + NOT_NEWLINE.sub(" ", s[1:-1]) + s[-1]
        return NOT_NEWLINE.sub(" ", s)

    return COMMENT_OR_STRING.sub(blank, text)


COMMENT_OR_STRING = re.compile(r"/\*.*?(?:\*/|\Z)|//[^\n]*|\"(?:\\.|[^\"\\])*(?:\"|\Z)|'(?:\\.|[^'\\])*(?:'|\Z)", re.S)
NOT_NEWLINE = re.compile(r"[^\n]")


def blank_directives(text):
    """Blank preprocessor lines (#define bodies aren't use sites), keeping
    every offset."""
    lines = text.split("\n")
    i = 0
    while i < len(lines):
        if lines[i].lstrip().startswith("#"):
            while True:
                cont = lines[i].rstrip().endswith("\\")
                lines[i] = " " * len(lines[i])
                if not cont or i + 1 >= len(lines):
                    break
                i += 1
        i += 1
    return "\n".join(lines)


@functools.lru_cache(maxsize=None)
def read_blanked(path):
    raw = open(path, encoding="utf-8", errors="replace").read()
    return raw, blank_directives(strip_comments(raw))


def match_close(text, start):
    """The offset of the bracket that closes the one at `start`, or the end."""
    depth = 0
    for i in range(start, len(text)):
        c = text[i]
        if c in "([{":
            depth += 1
        elif c in ")]}":
            depth -= 1
            if depth == 0:
                return i
    return len(text) - 1


def match_open(text, end):
    """The offset of the bracket that opens the one that closes at `end`."""
    depth = 0
    for i in range(end, -1, -1):
        c = text[i]
        if c in ")]}":
            depth += 1
        elif c in "([{":
            depth -= 1
            if depth == 0:
                return i
    return 0


def split_args(text, start):
    """The top-level arguments of the call whose `(` is at `start`, as
    (offset, text) pairs, or None if the parentheses don't close."""
    depth = 0
    args = []
    arg_start = start + 1
    for i in range(start, len(text)):
        c = text[i]
        if c in "([{":
            depth += 1
        elif c in ")]}":
            depth -= 1
            if depth == 0:
                if args or text[arg_start:i].strip():
                    args.append((arg_start, text[arg_start:i]))
                return args
        elif c == "," and depth == 1:
            args.append((arg_start, text[arg_start:i]))
            arg_start = i + 1
    return None


def line_of(text, offset):
    return text.count("\n", 0, offset) + 1


def parse_value(lit):
    lit = lit.replace(" ", "").rstrip("uUlL")
    neg = lit.startswith("-")
    lit = lit.lstrip("-")
    v = int(lit, 16) if lit[:2].lower() == "0x" else int(lit, 10)
    return -v if neg else v


# ---------------------------------------------------------------------------
# The C++ model: classes, their bases, methods and fields, the globals,
# and per file the functions (with the class of a method) and their
# declarations. Read from the sources, not compiled.


class Scope:
    """A class body or a function body in one file."""

    def __init__(self, kind, name, head_start, start, end):
        self.kind = kind  # "class" or "func"
        self.name = name  # the class (of a method), or None
        self.head_start = head_start
        self.start = start  # the `{`
        self.end = end  # the `}`
        self.fname = None  # a function's name
        self.params = []  # a function's parameter names, in order
        self.decls = []  # (offset, name, type) of the known-class declarations


def scopes(text):
    """The class bodies and function bodies of a file. A method defined
    in its class or as `Class::Method(...) {` gets the class's name."""
    out = []

    def walk(lo, hi, cls):
        i, stmt = lo, lo
        while i < hi:
            c = text[i]
            if c in ";}":
                stmt = i + 1
            elif c == "(":
                i = match_close(text, i) + 1
                continue
            elif c == "{":
                end = match_close(text, i)
                head = text[stmt:i]
                m = re.search(r"\b(class|struct|union)\s*(\w*)\s*(?::\s*([^{;()]*))?$", head)
                if m:
                    # `typedef struct [tag] {...} Name;`: Name, an alias of the tag
                    after = re.match(r"\s*(\w+)\s*;", text[end + 1 :])
                    typedef = after.group(1) if after and re.search(r"\btypedef\b", head) else None
                    name = m.group(2) or typedef
                    s = Scope("class", name or None, stmt, i, end)
                    s.bases = m.group(3) or ""
                    s.typedef = typedef
                    out.append(s)
                    walk(i + 1, end, name or cls)
                elif re.search(r"\b(?:namespace|extern)\b[^()]*$", head):
                    walk(i + 1, end, cls)
                elif re.search(r"\)\s*(?:const\s*)?$", head) or re.search(r"\)\s*:[^;]*\)\s*$", head):
                    q = re.search(r"(\w+)\s*::\s*(~?\w+)\s*\(", head)
                    s = Scope("func", q.group(1) if q else cls, stmt, i, end)
                    n = q or re.search(r"(\w+)\s*\(", head)
                    s.fname = n.group(n.lastindex) if n else None
                    p = head.find("(", q.end() - 1 if q else 0)
                    if p >= 0:
                        args = split_args(text, stmt + p) or []
                        for _, a in args:
                            n = re.search(r"(\w+)\s*(?:\[[^\]]*\]\s*)*$", a.strip())
                            names = re.findall(r"\w+", a)
                            s.params.append(n.group(1) if n and len(names) > 1 else None)
                    out.append(s)
                i = end + 1
                stmt = i
                continue
            i += 1

    walk(0, len(text), None)
    return out


def body_statements(text, lo, hi):
    """The statements of a class body at its own level: (text, block)
    pairs, where `block` is the (start, end) of a nested `{...}` (an
    inline method's body or a nested struct) or None."""
    i, stmt, block = lo, lo, None
    while i < hi:
        c = text[i]
        if c == ";":
            yield text[stmt:i], block
            stmt, block = i + 1, None
        elif c == "(":
            i = match_close(text, i)
        elif c == "{":
            end = match_close(text, i)
            head = text[stmt:i]
            if "(" in head and not re.search(r"\b(?:struct|union|class|enum)\b", head):
                yield head, (i, end)  # an inline method
                stmt, block = end + 1, None
            else:
                block = (i, end)
            i = end
        i += 1


# A function returning a pointer, declared or defined: `T *F(` or `T *C::F(`.
FUNC_DECL = re.compile(r"\b(\w+)\s*\*\s*(?:(\w+)\s*::\s*)?(\w+)\s*\(")
QUALIFIERS = ("extern", "static", "inline", "virtual", "const", "class", "struct")


def statement_start(text, i):
    """Whether only qualifiers (`static inline`) stand between `i` and
    the start of its statement."""
    while True:
        while i > 0 and text[i - 1].isspace():
            i -= 1
        m = re.search(r"\b(\w+)$", text[max(0, i - 16) : i])
        if m and m.group(1) in QUALIFIERS:
            i -= len(m.group(1))
            continue
        return i == 0 or text[i - 1] in ";{}"


TYPE_RE = re.compile(
    r"\s*(?:(?:static|const|volatile|unsigned|signed|mutable|struct|class|union|enum|virtual|inline)\s+)*(\w+)"
)


class Model:
    def __init__(self):
        self.bases = collections.defaultdict(list)
        self.methods = collections.defaultdict(set)
        self.static = set()
        self.fields = collections.defaultdict(dict)
        self.aliases = {}
        self.globals = {}
        self.cxx = []  # (C name, class, method) from cxx_symbols.txt
        self.returns = {}  # (class or None, function) -> the class it returns a pointer to
        self.anon = 0

    def canon(self, t):
        seen = set()
        while t in self.aliases and t not in seen:
            seen.add(t)
            t = self.aliases[t]
        return t

    def known(self, t):
        t = self.canon(t)
        return t in self.fields or t in self.methods or t in self.bases

    def ancestry(self, cls):
        """cls and its bases, nearest first."""
        out, todo = [], [self.canon(cls)]
        while todo:
            c = todo.pop(0)
            if c in out:
                continue
            out.append(c)
            todo += [self.canon(b) for b in self.bases.get(c, ())]
        return out

    def is_a(self, cls, base):
        return cls is not None and self.canon(base) in self.ancestry(cls)

    def subclasses(self, cls):
        cls = self.canon(cls)
        return [c for c in list(self.bases) if c != cls and cls in self.ancestry(c)]

    def find_method(self, cls, name):
        for c in self.ancestry(cls):
            if name in self.methods.get(c, ()):
                return c
        return None

    def field_type(self, cls, name):
        """(owner, type) of field `name` of cls or a base, or None."""
        for c in self.ancestry(cls):
            if name in self.fields.get(c, {}):
                return c, self.fields[c][name]
        return None

    # -- loading

    def parse_class(self, text, s, name):
        if s.bases:
            for b in s.bases.split(","):
                words = [w for w in re.findall(r"\w+", b) if w not in ("public", "private", "protected", "virtual")]
                if words:
                    self.bases[name].append(words[-1])
        self.parse_body(text, s.start + 1, s.end, name)

    def parse_body(self, text, lo, hi, cls):
        self.fields.setdefault(cls, {})
        for stmt, block in body_statements(text, lo, hi):
            stmt = re.sub(r"^\s*(?:(?:public|private|protected)\s*:\s*)+", "", stmt)
            if not stmt.strip() or re.match(r"\s*(?:friend|typedef|using|enum)\b", stmt):
                continue
            if block and "(" in stmt and not re.search(r"\b(?:struct|union|class)\b", stmt):
                self.add_method(cls, stmt)
                continue
            if block:
                tag = re.match(r"\s*(?:struct|union|class)\s*(\w*)", stmt)
                decl = text[block[1] + 1 : text.find(";", block[1])]
                names = re.findall(r"(\w+)\s*(?:\[[^\]]*\]\s*)*(?:,|$)", decl.strip())
                if tag and tag.group(1):
                    for n in names:
                        self.fields[cls][n] = tag.group(1)
                elif names:
                    self.anon += 1
                    anon = f"{cls}::<anon{self.anon}>"
                    self.parse_body(text, block[0] + 1, block[1], anon)
                    for n in names:
                        self.fields[cls][n] = anon
                else:  # an anonymous union/struct: its fields are cls's
                    self.parse_body(text, block[0] + 1, block[1], cls)
                continue
            if re.match(r"[^(]*\(\s*\*", stmt):  # a function pointer
                m = re.search(r"\(\s*\*\s*(\w+)", stmt)
                if m:
                    self.fields[cls][m.group(1)] = None
                continue
            if "(" in stmt.split("=")[0]:
                self.add_method(cls, stmt)
                continue
            m = TYPE_RE.match(stmt)
            if not m:
                continue
            typ = m.group(1)
            rest = stmt[m.end() :].split("=")[0]
            for d in rest.split(","):
                d = re.sub(r":\s*\d+\s*$", "", d.strip())  # a bit-field
                n = re.search(r"(\w+)\s*(?:\[[^\]]*\]\s*)*$", d)
                if n:
                    self.fields[cls][n.group(1)] = typ

    def add_method(self, cls, stmt):
        p = stmt.find("(")
        m = re.search(r"(~?\w+)\s*$", stmt[:p])
        if not m:
            return
        self.methods[cls].add(m.group(1))
        if re.search(r"\bstatic\b", stmt[: m.start()]):
            self.static.add((cls, m.group(1)))
        r = re.search(r"(\w+)\s*\*\s*$", stmt[: m.start()])
        if r:
            self.add_return(cls, m.group(1), r.group(1))

    def add_return(self, cls, name, typ):
        key = (cls, name)
        if typ in KEYWORDS:
            return
        if self.returns.get(key, typ) != typ:
            typ = None  # overloads that return different classes
        self.returns[key] = typ

    def load(self, files):
        decl = re.compile(
            r"\bextern\s+(?:(?:const|volatile|class|struct|union)\s+)*(\w+)\s*\*?\s*(?:const\s+)?(\w+)\s*"
            r"(?:\[[^;\]]*\]\s*)*;"
        )
        for path in files:
            _, text = read_blanked(path)
            found = scopes(text)
            classes = [s for s in found if s.kind == "class"]
            for m in FUNC_DECL.finditer(text):
                if not statement_start(text, m.start()):
                    continue
                if not any(c.start < m.start() < c.end for c in classes):
                    self.add_return(m.group(2), m.group(3), m.group(1))
            for s in found:
                if s.kind != "class":
                    continue
                name = s.name
                if not name:
                    continue  # nested anonymous: parsed with its parent
                if s.typedef and s.typedef != name:
                    self.aliases[s.typedef] = name
                self.parse_class(text, s, name)
            for m in re.finditer(r"\btypedef\s+(?:struct|class|union)\s+(\w+)\s+(\w+)\s*;", text):
                if m.group(1) != m.group(2):
                    self.aliases[m.group(2)] = m.group(1)
            for m in decl.finditer(text):
                if m.group(1) == "void":
                    continue
                # `extern class X *g;` (C++) over `extern struct x *g;` (C)
                old = self.globals.get(m.group(2))
                if old and (old in self.methods or not re.search(r"\bclass\b", m.group(0))):
                    continue
                self.globals[m.group(2)] = m.group(1)
        self.load_cxx_symbols()

    def load_cxx_symbols(self):
        if not os.path.exists(CXX_SYMBOLS):
            return
        for line in open(CXX_SYMBOLS):
            parts = line.split()
            if len(parts) != 2 or parts[0].startswith("#"):
                continue
            m = re.match(r"^([A-Za-z]\w*?)__C?(\d+)(\w+)$", parts[0])
            if not m:
                continue
            n = int(m.group(2))
            cls = m.group(3)[:n]
            if len(cls) < n:
                continue
            self.cxx.append((parts[1], cls, m.group(1)))
            self.methods[cls].add(m.group(1))


# ---------------------------------------------------------------------------
# The rules, for C and C++.


def call_rules(model):
    """(C rules: name regex -> [(topic, index, prefixes)],
    C++ rules: (class, method) -> [(topic, index, prefixes)])."""
    crules, cxxrules = [], collections.defaultdict(list)
    for entry in CALLS:
        topic, name, index = entry[:3]
        opts = entry[3] if len(entry) > 3 else {}
        prefixes = opts.get("prefixes")
        crules.append((topic, name, index, prefixes))
        if opts.get("cxx", True) is False:
            continue
        pat = re.compile(name + r"$")
        for cname, cls, method in model.cxx:
            if pat.match(cname):
                i = opts.get("cxx_index", index if (cls, method) in model.static else index - 1)
                if i >= 0:
                    cxxrules[(cls, method)].append((topic, i, prefixes))
    return crules, cxxrules


class FileScan:
    def __init__(self, path, rel, model, cxxrules, crules):
        self.rel = rel
        self.model = model
        self.cxxrules = cxxrules
        self.crules = crules
        self.raw, self.text = read_blanked(path)
        self.src_lines = self.raw.split("\n")
        self.is_cxx = rel.endswith(CXX_EXTS)
        self.hits = []
        self.taken = set()
        self.scopes = scopes(self.text) if self.is_cxx else []
        self.funcs = [s for s in self.scopes if s.kind == "func"]
        self.file_decls = []
        self.all_calls = None  # (match, name, args) of every call, for cxx_calls
        if self.is_cxx:
            self.find_decls()

    # -- declarations and types

    DECL = re.compile(
        r"(?:(?<=[;{}(,])|(?<=^))\s*(?:(?:const|volatile|static|register|struct|class|union)\s+)*"
        r"([A-Za-z_]\w*)\s*(\*+\s*(?:const\s+)?|&\s*|\s)\s*([A-Za-z_]\w*)\s*(?=[;=,)\[])",
        re.M,
    )
    # Any declaration of `name` (a local of any type), for is_local.
    ANYDECL = (
        r"(?:^|(?<=[;{{}}(,]))\s*(?:(?:const|volatile|static|register|struct|class|union|unsigned|signed)\s+)*"
        r"(?!(?:{kw})\b)[A-Za-z_]\w*\s*[*&\s]\s*(?:const\s+)?\b{name}\s*(?=[;=,)\[])"
    )

    def find_decls(self):
        classes = [s for s in self.scopes if s.kind == "class"]
        for m in self.DECL.finditer(self.text):
            typ, name = m.group(1), m.group(3)
            if typ in KEYWORDS or not self.model.known(typ):
                continue
            off = m.start(3)
            f = self.func_at(off, heads=True)
            if f:
                f.decls.append((off, name, typ))
            elif not any(c.start < off < c.end for c in classes):
                self.file_decls.append((off, name, typ))

    def func_at(self, off, heads=False):
        for f in self.funcs:
            if (f.head_start if heads else f.start) <= off <= f.end:
                return f
        return None

    def local_type(self, f, name, off):
        """The known class a local, parameter or file-scope variable is
        declared as (the nearest declaration before `off`)."""
        best = None
        for o, n, t in (f.decls if f else []):
            if n == name and o < off:
                best = t
        if best:
            return best
        for o, n, t in self.file_decls:
            if n == name and o < off:
                best = t
        return best

    def is_local(self, f, name, off):
        """Whether `name` is a parameter or a local of the function (any type)."""
        if f is None:
            return False
        if name in f.params:
            return True
        pat = ANYDECL_CACHE.get(name)
        if pat is None:
            pat = ANYDECL_CACHE[name] = re.compile(
                self.ANYDECL.format(kw="|".join(KEYWORDS), name=re.escape(name)), re.M
            )
        return bool(pat.search(self.text, f.start, off))

    def name_type(self, name, off):
        """The class of identifier `name` at `off`: `this`, a local, a
        member of the method's class, a global."""
        f = self.func_at(off)
        if name == "this":
            return f.name if f else None
        t = self.local_type(f, name, off)
        if t:
            return self.model.canon(t)
        if f and f.name and not self.is_local(f, name, off):
            ft = self.model.field_type(f.name, name)
            if ft:
                return ft[1] and self.model.canon(ft[1])
        g = self.model.globals.get(name)
        return g and self.model.canon(g)

    def receiver(self, end):
        """The class of the expression that ends just before `end` (the
        `->`/`.` of a member access), or None; and the classes it can be
        at run time when MEMBER_CLASSES knows (or None)."""
        segs = self.chain(end)
        if not segs:
            return None, None
        model = self.model
        kind, val = segs[0]
        if kind == "type":
            t = model.canon(val)
        elif kind == "id":
            t = self.name_type(val, end)
        elif kind == "call":
            f = self.func_at(end)
            owner = f and f.name and model.find_method(f.name, val)
            t = model.returns.get((owner, val)) if owner else model.returns.get((None, val))
            t = t and model.canon(t)
        else:
            return None, None
        dynamic = None
        for kind, val in segs[1:]:
            if t is None:
                return None, None
            dynamic = None
            if kind == "mem":
                ft = model.field_type(t, val)
                dynamic = next((v for (c, m), v in MEMBER_CLASSES.items() if m == val and model.is_a(t, c)), None)
                t = ft and ft[1] and model.canon(ft[1])
            elif kind == "call":
                owner = model.find_method(t, val)
                t = owner and model.returns.get((owner, val))
                t = t and model.canon(t)
        return t, dynamic

    def chain(self, end):
        """The postfix chain that ends at `end`, outermost last:
        [("id", base) | ("type", cast) | ("call", function),
         ("mem", name) | ("call", method) | ("idx", None), ...]."""
        text = self.text
        segs = []
        i = end
        while True:
            while i > 0 and text[i - 1].isspace():
                i -= 1
            if i <= 0:
                return None
            c = text[i - 1]
            kind = "mem"
            if c == "]":
                i = match_open(text, i - 1)
                segs.append(("idx", None))
                continue
            if c == ")":
                o = match_open(text, i - 1)
                j = o
                while j > 0 and text[j - 1].isspace():
                    j -= 1
                if j > 0 and (text[j - 1].isalnum() or text[j - 1] == "_"):
                    i, kind = j, "call"  # a call's result: its name is next
                else:
                    inner = text[o + 1 : i - 1]
                    m = re.match(r"\s*\(\s*(?:const\s+)?(?:class\s+|struct\s+)?(\w+)\s*\*+\s*\)", inner)
                    if m:
                        segs.append(("type", m.group(1)))
                        return segs[::-1]
                    m = re.match(r"\s*\*?\s*(\w+)\s*$", inner)
                    if m:
                        segs.append(("id", m.group(1)))
                        return segs[::-1]
                    sub = self.chain(o + 1 + len(inner.rstrip()))
                    return sub and sub + segs[::-1]
            m = re.search(r"(\w+)$", text[max(0, i - 64) : i])
            if not m or m.group(1)[0].isdigit():
                return None
            name = m.group(1)
            i -= len(name)
            j = i
            while j > 0 and text[j - 1].isspace():
                j -= 1
            if text[j - 2 : j] == "->" or (text[j - 1 : j] == "." and text[j - 2 : j - 1] != "."):
                segs.append((kind, name))
                i = j - (2 if text[j - 2 : j] == "->" else 1)
                continue
            if text[j - 2 : j] == "::":
                q = re.search(r"(\w+)\s*$", text[max(0, j - 66) : j - 2])
                if q:
                    segs.append((kind, name))
                    segs.append(("type", q.group(1)))
                    return segs[::-1]
            segs.append(("call" if kind == "call" else "id", name))
            return segs[::-1]

    # -- hits

    def add(self, topic, offset, lit, confidence, prefixes=None, why=""):
        line = line_of(self.text, offset)
        key = (line, offset)
        if key in self.taken:
            return
        self.taken.add(key)
        lead = len(lit) - len(lit.lstrip())
        self.hits.append(
            Hit(
                self.rel,
                line,
                topic,
                lit.strip(),
                self.src_lines[line - 1].strip(),
                offset + lead,
                confidence,
                tuple(prefixes or TOPICS[topic][1]),
                why,
            )
        )

    def call_args(self, m):
        """The arguments of the call whose name `m` matched, or None for a
        declaration or definition."""
        before = self.text[max(0, m.start() - 40) : m.start()]
        if re.search(r"\b(?:void|s32|u32|u8|s8|u16|s16|bool|int|extern|static)\s*\**\s*$", before):
            return None
        p = self.text.index("(", m.start())
        args = split_args(self.text, p)
        if args is None:
            return None
        close = match_close(self.text, p)
        if re.match(r"\s*(?:const\s*)?(?:\{|:[^;]*\{)", self.text[close + 1 : close + 200]):
            return None  # a definition
        return args

    def c_calls(self):
        """The C rules' calls: (topic, offset, argument, prefixes)."""
        text = self.text
        for topic, name, index, prefixes in self.crules:
            if re.fullmatch(r"\w+", name) and name not in text:
                continue
            pat = CALL_RE_CACHE.setdefault(name, re.compile(r"\b(?:" + name + r")\s*\("))
            for m in pat.finditer(text):
                if self.is_cxx:
                    lead = text[max(0, m.start() - 2) : m.start()]
                    if lead.endswith((".", "->", "::")) or lead[-1:] == ">":
                        continue
                    f = self.func_at(m.start())
                    called = re.match(r"\w+", text[m.start() :]).group(0)
                    if f and f.name and self.model.find_method(f.name, called):
                        continue  # a method called on `this`
                    if any(c == called for _, _, c in self.model.cxx):
                        continue  # a method's name (PlaySfx), not the C function
                args = self.call_args(m)
                if args is None or index >= len(args):
                    continue
                yield topic, args[index][0], args[index][1], prefixes

    def cxx_calls(self):
        """The C++ calls of the methods and functions that have rules:
        (match, args, rules, confidence, why)."""
        if not self.is_cxx:
            return
        if self.all_calls is None:
            self.all_calls = []
            for m in CALL_ANY_RE.finditer(self.text):
                if m.group(3) not in KEYWORDS:
                    args = self.call_args(m)
                    if args:
                        self.all_calls.append((m, m.group(3), args))
        methods = {m for _, m in self.cxxrules}
        for m, method, args in self.all_calls:
            if method in methods:
                rules, conf, why = self.method_rules(m, method)
                if rules:
                    yield m, args, rules, conf, why

    def scan_calls(self):
        for topic, off, arg, prefixes in self.c_calls():
            if LIT_RE.match(arg.strip()):
                self.add(topic, off, arg, "high", prefixes, "call")
        for m, args, rules, conf, why in self.call_list:
            for topic, index, prefixes in rules:
                if index < len(args) and LIT_RE.match(args[index][1].strip()):
                    self.add(topic, args[index][0], args[index][1], conf, prefixes, why)

    def wrappers(self):
        """Rules for the functions that pass a parameter straight on as a
        ruled argument (`HitPlayer(pl, event)` calls `pl->HandleEvent(0,
        event, 0)`; ActionCtrl::SetModeAnimNow(mode, ...) calls
        `SetMode(mode)`): {(class or None, function): [(topic, index,
        prefixes)]}, from the high-confidence calls."""
        out = collections.defaultdict(list)
        for m, args, rules, conf, why in self.cxx_calls():
            if conf != "high":
                continue
            f = self.func_at(m.start())
            if f is None or not f.fname:
                continue
            for topic, index, prefixes in rules:
                if index >= len(args):
                    continue
                a = strip_casts(args[index][1])
                if re.fullmatch(r"\w+", a) and a in f.params:
                    out[(f.name, f.fname)].append((topic, f.params.index(a), prefixes))
        return out

    def method_rules(self, m, method):
        """The rules of the method a call calls, the confidence and why."""
        model = self.model
        dynamic = None
        if m.group(1):  # obj->Method( / obj.Method(
            recv, dynamic = self.receiver(m.start(1))
        elif m.group(2):  # Class::Method(
            q = re.search(r"(\w+)\s*$", self.text[max(0, m.start(2) - 64) : m.start(2)])
            recv = q and model.canon(q.group(1))
        else:  # Method( in a method: this->Method(; or a free function
            f = self.func_at(m.start())
            recv = f.name if f and f.name and model.find_method(f.name, method) else None
            if recv is None:
                rules = self.cxxrules.get((None, method))
                return (rules, "high", f"{method}()") if rules else ([], None, "")
        if recv:
            owner = model.find_method(recv, method)
            if owner:
                rules = self.cxxrules.get((owner, method))
                what = f"{owner}::{method}" + (f" of a {recv}" if recv != owner else "")
                if rules:
                    return rules, "high", what
                # A virtual call can reach a subclass's override.
                ruled = {c for c, mm in self.cxxrules if mm == method}
                subs = dynamic or model.subclasses(recv)
                over = sorted({model.find_method(s, method) for s in subs} & ruled)
                if over:
                    rules = self.cxxrules[(over[0], method)]
                    if dynamic:
                        return rules, "medium", f"{what}; the object is a {'/'.join(dynamic)}"
                    return rules, "low", f"{what}, overridden by {'/'.join(over)}"
                return [], None, ""
        # Unknown receiver: every class with the method.
        owners = sorted(c for c, ms in model.methods.items() if method in ms)
        ruled = [self.cxxrules.get((c, method)) for c in owners]
        firsts = [tuple(r) if r else None for r in ruled]
        if owners and all(firsts) and len(set(firsts)) == 1:
            return ruled[0], "high", f"every {method} ({len(owners)} classes)"
        known = [r for r in ruled if r]
        if known:
            with_rule = [c for c, r in zip(owners, ruled) if r]
            return known[0], "low", f"{method} of an unknown class ({'/'.join(with_rule)} of {len(owners)})"
        return [], None, ""

    def scan_fields(self, table):
        for topic, pattern, prefix, prefixes in table:
            if prefix and not self.rel.startswith(prefix):
                continue
            for m in re.finditer(pattern, self.text):
                self.add(topic, m.start("v"), m.group("v"), "high", prefixes, "field")

    # -- C++ values: fields, call results and the variables that hold them

    def literal_uses(self, start, end):
        """The literals the value `text[start:end]` is compared with,
        assigned, switched on (the case labels) or range-checked against
        (`(u32)(x - N) <= M`): [(offset, literal)]."""
        text = self.text
        start = self.chain_start(start)
        out = []
        c = USE_CMP_RE.match(text, end)
        if c:
            out.append((c.start("v"), c.group("v")))
        r = USE_RANGE_RE.match(text, end)
        if r and re.search(r"\(\s*$", text[max(0, start - 8) : start]):
            out.append((r.start("v"), r.group("v")))
        r = USE_LIMIT_RE.match(text, end)
        if r and re.search(r"\bLIMIT_M(?:AX|IN)\s*\(\s*$", text[max(0, start - 16) : start]):
            out.append((r.start("v"), r.group("v")))
        k = end
        while k < len(text) and text[k].isspace():
            k += 1
        if k < len(text) and text[k] == ")":
            o = match_open(text, k)
            inner = text[o + 1 : start]
            if re.fullmatch(r"\s*(?:\(\s*\w+\s*\)\s*)?", inner) and re.search(
                r"\bswitch\s*$", text[max(0, o - 16) : o]
            ):
                b = text.find("{", k)
                if b >= 0:
                    out += self.cases_of(b)
        return out

    def chain_start(self, pos):
        """The start of the postfix chain whose last name starts at `pos`
        (`room.cat->kind`, `parts[i].kind`, `f()->kind`)."""
        text = self.text
        while True:
            j = pos
            while j > 0 and text[j - 1].isspace():
                j -= 1
            if text[j - 2 : j] == "->":
                j -= 2
            elif text[j - 1 : j] == "." and text[j - 2 : j - 1] != ".":
                j -= 1
            else:
                return pos
            while j > 0 and text[j - 1].isspace():
                j -= 1
            while j > 0 and text[j - 1] in ")]":
                j = match_open(text, j - 1)
            m = re.search(r"\w+$", text[max(0, j - 64) : j])
            pos = j - len(m.group(0)) if m else j

    def cases_of(self, b):
        """The case labels at the top level of the switch body at `b`."""
        text = self.text
        end = match_close(text, b)
        nested = []
        for s in re.finditer(r"\bswitch\s*\(", text[b + 1 : end]):
            ob = text.find("{", b + 1 + s.end())
            if ob >= 0:
                nested.append((ob, match_close(text, ob)))
        out = []
        for c in CASE_RE.finditer(text, b, end):
            if any(lo < c.start() < hi for lo, hi in nested):
                continue
            for g in ("v", "w"):
                if c.group(g):
                    out.append((c.start(g), c.group(g)))
        return out

    def field_rule(self, name, op, pos):
        """(confidence, why, rule) for a use of field `name` at `pos`,
        reached through the `->`/`.` at `op` or (op None) bare."""
        model = self.model
        rules = FIELD_RULES.get(name)
        if not rules:
            return None, "", None
        dynamic = None
        if op is not None:
            recv, dynamic = self.receiver(op)
        else:
            f = self.func_at(pos)
            if f is None or not f.name or self.is_local(f, name, pos):
                return None, "", None
            recv = f.name
            if not model.field_type(recv, name):
                return None, "", None
        if recv:
            ft = model.field_type(recv, name)
            if not ft:
                return None, "", None
            what = f"{ft[0]}::{name}" + (f" of a {recv}" if recv != ft[0] else "")
            for rule in rules:
                if model.is_a(recv, rule[1]):
                    return "high", what, rule
            for rule in rules:
                if any(model.is_a(d, rule[1]) for d in dynamic or ()):
                    return "medium", f"{what}; the object is a {'/'.join(dynamic)}", rule
            return None, "", None
        owners = sorted(c for c, fs in model.fields.items() if name in fs and "<" not in c)
        ruled = [next((r for r in rules if model.is_a(c, r[1])), None) for c in owners]
        if owners and all(ruled) and len({(r[0], r[2]) for r in ruled}) == 1:
            return "high", f"every {name} ({len(owners)} classes)", ruled[0]
        known = [r for r in ruled if r]
        if known:
            with_rule = [c for c, r in zip(owners, ruled) if r]
            return "low", f"{name} of an unknown class ({'/'.join(with_rule)} of {len(owners)})", known[0]
        return None, "", None

    def result_rule(self, name, op, pos, colons=None):
        """(confidence, why, rule) for the result of a call of `name`."""
        model = self.model
        rules = RESULT_RULES.get(name)
        if not rules:
            return None, "", None
        if op is not None:
            recv, _ = self.receiver(op)
        elif colons is not None:
            q = re.search(r"(\w+)\s*$", self.text[max(0, colons - 64) : colons])
            recv = q and model.canon(q.group(1))
        else:
            f = self.func_at(pos)
            recv = f.name if f and f.name and model.find_method(f.name, name) else None
        for rule in rules:
            if rule[1] is None and recv is None:
                return "high", f"{name}()", rule
            if rule[1] is not None and recv and model.is_a(recv, rule[1]):
                return "high", f"{model.find_method(recv, name) or recv}::{name}", rule
        return None, "", None

    def expr_rule(self, start, end, ruled_vars):
        """(confidence, why, rule) for the expression text[start:end]: a
        field, a call result or a ruled variable, under casts."""
        text = self.text
        e = text[start:end]
        while True:
            m = re.match(r"\s*\(\s*(?:const\s+)?(?:unsigned\s+|signed\s+)?\w+\s*\**\s*\)", e)
            if m and not re.match(r"\s*\(\s*\w+\s*\)\s*(?:->|\.|\[|$)", e):
                start += m.end()
                e = e[m.end() :]
                continue
            s = e.strip()
            if s.startswith("(") and match_close(s, 0) == len(s) - 1:
                start += e.index("(") + 1
                e = s[1:-1]
                continue
            break
        s = e.rstrip()
        end = start + len(s)
        t = re.fullmatch(r"\s*(\w+)\s*\[[^\]]*\]", s)
        if t and t.group(1) in TABLE_RULES:
            topic, prefixes = TABLE_RULES[t.group(1)]
            return "high", f"{t.group(1)}[]", (topic, None, prefixes)
        if re.fullmatch(r"[\w\s.\->\[\]]*\w", s.strip()):
            name = re.search(r"(\w+)$", s).group(1)
            pos = end - len(name)
            j = pos
            while j > 0 and text[j - 1].isspace():
                j -= 1
            op = j - 2 if text[j - 2 : j] == "->" else j - 1 if text[j - 1 : j] == "." else None
            if op is None and name in ruled_vars:
                return ruled_vars[name]
            return self.field_rule(name, op, pos)
        if s.endswith(")"):
            o = match_open(text, end - 1)
            m = re.search(r"(\w+)\s*$", text[start:o])
            if m:
                name = m.group(1)
                pos = start + m.start(1)
                j = pos
                while j > 0 and text[j - 1].isspace():
                    j -= 1
                op = j - 2 if text[j - 2 : j] == "->" else j - 1 if text[j - 1 : j] == "." else None
                colons = j - 2 if text[j - 2 : j] == "::" else None
                return self.result_rule(name, op, pos, colons)
        return None, "", None

    def ruled_vars(self, f):
        """{name: (confidence, why, rule)} for the parameters of f that a
        rule names (a ruled method's or a wrapper's), and the locals whose
        every assignment is a ruled value of one kind or a literal."""
        text = self.text
        out = {}
        for topic, index, prefixes in self.cxxrules.get((f.name, f.fname), ()):
            if index < len(f.params) and f.params[index]:
                out[f.params[index]] = ("high", f"parameter {index} of {f.fname}", (topic, None, prefixes))
        # A local passed as a ruled argument (`id = ...; PlaySong(id)`).
        for m, args, rules, conf, why in self.call_list:
            if not f.start < m.start() < f.end:
                continue
            for topic, index, prefixes in rules:
                if index < len(args):
                    a = strip_casts(args[index][1])
                    if re.fullmatch(r"[A-Za-z_]\w*", a) and a not in out and self.is_local(f, a, m.start()):
                        out[a] = (conf, f"{a} is passed to {why}", (topic, None, prefixes))
        sources = collections.defaultdict(list)
        changed = set()
        for m in ASSIGN_RE.finditer(text, f.start, f.end):
            sources[m.group("name")].append((m.end(), expr_end(text, m.end())))
        for m in re.finditer(r"(?:\+\+|--)\s*(\w+)|(\w+)\s*(?:\+\+|--|[-+*/%&|^]=|<<=|>>=)", text[f.start : f.end]):
            changed.add(m.group(1) or m.group(2))
        for _ in range(2):  # a local copied from another ruled local
            for name, rhss in sources.items():
                if name in out or name in changed or not self.is_local(f, name, f.end):
                    continue
                found = []
                for a, b in rhss:
                    if LIT_RE.match(strip_casts(text[a:b])):
                        continue
                    conf, why, rule = self.expr_rule(a, b, out)
                    found.append((conf, why, rule))
                if not found or any(r is None for _, _, r in found):
                    continue
                kinds = {(r[0], tuple(r[2] or ())) for _, _, r in found}
                if len(kinds) != 1:
                    continue
                conf = max((c for c, _, _ in found), key=CONFIDENCES.index)
                out[name] = (conf, f"{name} = {found[0][1]}", found[0][2])
        return out

    def scan_values(self):
        """The literals compared with, assigned to or switched on with a
        ruled field (CXX_FIELDS), call result (CXX_RESULTS) or a variable
        that holds one; and those a ruled function returns."""
        text = self.text
        for m in FIELD_USE_RE.finditer(text):
            name = m.group(2)
            lits = self.literal_uses(m.start(2), m.end(2))
            if not lits:
                continue
            conf, why, rule = self.field_rule(name, m.start(1) if m.group(1) else None, m.start(2))
            if rule:
                for off, lit in lits:
                    self.add(rule[0], off, lit, conf, rule[2], why)
        for m in RESULT_USE_RE.finditer(text):
            o = text.index("(", m.end(2))
            close = match_close(text, o)
            lits = self.literal_uses(m.start(2), close + 1)
            if not lits:
                continue
            op = m.start(1) if m.group(1) in ("->", ".") else None
            colons = m.start(1) if m.group(1) == "::" else None
            conf, why, rule = self.result_rule(m.group(2), op, m.start(2), colons)
            if rule:
                for off, lit in lits:
                    self.add(rule[0], off, lit, conf, rule[2], why)
        for f in self.funcs:
            ruled = self.ruled_vars(f)
            for name, (conf, why, rule) in ruled.items():
                for m in re.finditer(r"(?<![\w.>])\b" + name + r"\b(?!\s*(?:\(|->|\.|\[))", text[f.start : f.end]):
                    pos = f.start + m.start()
                    for off, lit in self.literal_uses(pos, pos + len(name)):
                        self.add(rule[0], off, lit, conf, rule[2], why)
            for topic, cls, name, prefixes in CXX_RESULTS:
                if f.fname != name or (f.name is None) != (cls is None):
                    continue
                if cls is None or self.model.is_a(f.name, cls):
                    for r in re.finditer(r"\breturn\s+(?P<v>-?\s*" + LIT + r")\s*;", text[f.start : f.end]):
                        self.add(topic, f.start + r.start("v"), r.group("v"), "high", prefixes, f"{name} returns it")

    def scan_events(self):
        text = self.text
        for m in EVENT_HANDLER_RE.finditer(text):
            params = split_args(text, m.start(1) - 1) or []
            names = []
            for _, a in params:
                n = re.search(r"(\w+)\s*$", a.strip())
                names.append(n.group(1) if n and len(re.findall(r"\w+", a)) > 1 else None)
            # The event: (sender, event, arg) for a method, (this, sender, event, arg) in C.
            k = 1 if self.is_cxx and len(names) == 3 else 2
            event = names[k] if k < len(names) else None
            body = m.end() - 1
            end = match_close(text, body)
            if event is None:
                continue
            for s in re.finditer(r"\bswitch\s*\(", text[body:end]):
                o = body + s.end() - 1
                expr = text[o + 1 : match_close(text, o)]
                if re.fullmatch(r"\s*(?:\(\s*\w+\s*\)\s*)?" + event + r"\s*", expr):
                    for off, lit in self.cases_of(text.find("{", o)):
                        self.add("event", off, lit, "high", None, "event handler case")
        if self.is_cxx:
            return
        for m in EVENT_SLOT_RE.finditer(text):
            if "PhysCall3" in text[text.rfind("\n", 0, m.start()) + 1 : m.start()]:
                continue  # counted by CALLS
            c = EVENT_SLOT_CALL_RE.search(text, m.end(), m.end() + EVENT_SLOT_REACH)
            args = c and split_args(text, c.end() - 1)
            if args and len(args) == 4 and LIT_RE.match(args[2][1].strip()):
                self.add("event", args[2][0], args[2][1], "high", None, "event slot call")

    def scan_level_flags(self):
        text = self.text
        flag_lines = [line_of(text, m.start()) for m in re.finditer(r"\bGet(?:Current)?LevelFlags\s*\(", text)]
        if not flag_lines:
            return
        lines = text.split("\n")
        for fl in flag_lines:
            for ln in range(fl, min(fl + LEVEL_FLAGS_WINDOW, len(lines)) + 1):
                m = LEVEL_FLAGS_ASSIGN.search(lines[ln - 1])
                if m:
                    off = sum(len(l) + 1 for l in lines[: ln - 1]) + m.start("v")
                    self.add("level_flags", off, m.group("v"), "high", None, "level flags mask")

    def scan(self):
        self.call_list = list(self.cxx_calls())
        self.scan_calls()
        self.scan_fields(FIELDS)
        if self.is_cxx:
            self.scan_values()
        else:
            self.scan_fields(C_FIELDS)
        self.scan_events()
        self.scan_level_flags()
        counted = {h.line for h in self.hits}
        for m in FLAG_TEST.finditer(self.text):
            if line_of(self.text, m.start()) in counted:
                continue
            self.add("flag_test", m.start("v"), m.group("v"), "low", None, "& mask")
        return self.hits


def strip_casts(s):
    s = s.strip()
    while True:
        m = re.match(r"\(\s*(?:const\s+)?(?:unsigned\s+|signed\s+)?\w+\s*\**\s*\)\s*", s)
        if not m or m.end() == len(s):
            return s
        s = s[m.end() :]


FIELD_RULES = collections.defaultdict(list)
for _topic, _cls, _names, _prefixes in CXX_FIELDS:
    for _n in _names:
        FIELD_RULES[_n].append((_topic, _cls, _prefixes))
RESULT_RULES = collections.defaultdict(list)
for _topic, _cls, _name, _prefixes in CXX_RESULTS:
    RESULT_RULES[_name].append((_topic, _cls, _prefixes))
TABLE_RULES = {_name: (_topic, _prefixes) for _topic, _name, _prefixes in CXX_TABLES}
FIELD_USE_RE = re.compile(r"(?:(->|\.)\s*|(?<![\w.>:]))\b(" + "|".join(sorted(FIELD_RULES)) + r")\b(?!\s*\()")
RESULT_USE_RE = re.compile(r"(?:(->|\.|::)\s*|(?<![\w.>:]))\b(" + "|".join(sorted(RESULT_RULES)) + r")\s*\(")
USE_CMP_RE = re.compile(r"\s*" + CMP + r"\s*(?P<v>-?\s*" + LIT + r")(?![\w.])")
USE_LIMIT_RE = re.compile(r"\s*,\s*(?P<v>-?\s*" + LIT + r")\s*\)")
USE_RANGE_RE = re.compile(r"\s*-\s*(?P<v>" + LIT + r")\s*\)\s*(?:<=|<|>|>=)")
ASSIGN_RE = re.compile(r"(?<![\w.>])\b(?P<name>[A-Za-z_]\w*)\s*=(?!=)")


def expr_end(text, i):
    """The end of the expression that starts at `i`: the `;`, `,` or
    unbalanced closing bracket after it."""
    depth = 0
    for j in range(i, len(text)):
        c = text[j]
        if c in "([{":
            depth += 1
        elif c in ")]}":
            if depth == 0:
                return j
            depth -= 1
        elif c in ";," and depth == 0:
            return j
    return len(text)


def beyond_names(hits, names):
    """A value above every name of its prefixes isn't one of them
    (`level <= 0x1000`, a `case` past the last crate kind): low."""
    out = []
    for h in hits:
        values = [v for p in h.prefixes for v in names[p]]
        if h.topic not in ("level_flags", "flag_test") and values and parse_value(h.literal) > max(values):
            h = h._replace(confidence="low", why=h.why + "; above every name")
        out.append(h)
    return out


def source_files():
    out = []
    for d in SCAN_DIRS:
        for dirpath, dirnames, filenames in os.walk(os.path.join(ROOT, d)):
            dirnames.sort()
            if os.path.relpath(dirpath, ROOT).startswith(os.path.join("include", "constants")):
                continue
            for f in sorted(filenames):
                if f.endswith(C_EXTS + CXX_EXTS):
                    out.append(os.path.join(dirpath, f))
    return out


def scan(paths=None):
    files = source_files()
    model = Model()
    model.load(files)
    crules, cxxrules = call_rules(model)
    scans = [FileScan(path, os.path.relpath(path, ROOT), model, cxxrules, crules) for path in files]
    # Wrappers that pass a parameter on as a ruled argument get the rule
    # too, until nothing changes (a wrapper of a wrapper).
    for _ in range(4):
        new = 0
        for s in scans:
            for key, rules in s.wrappers().items():
                for r in rules:
                    if r not in cxxrules[key]:
                        cxxrules[key].append(r)
                        new += 1
        if not new:
            break
    hits = []
    for s in scans:
        if paths and not any(s.rel == p or s.rel.startswith(p.rstrip("/") + "/") for p in paths):
            continue
        hits += s.scan()
    hits = beyond_names(hits, load_constants())
    hits.sort(key=lambda h: (list(TOPICS).index(h.topic), h.file, h.line, h.offset))
    return hits


def load_constants():
    """prefix -> value -> [names], from the constants headers (CONSTANTS_DIRS).
    A name goes under the longest prefix it has (LEVEL_FLAG_ over LEVEL_)."""
    prefixes = {p for _, ps in TOPICS.values() for p in ps}
    by_prefix = collections.defaultdict(lambda: collections.defaultdict(list))
    define = re.compile(r"^\s*#\s*define\s+([A-Z][A-Z0-9_]*)\s+\(?\s*(-?\s*" + LIT + r")\s*\)?\s*(?:/[/*].*)?$")
    shift = re.compile(r"^\s*#\s*define\s+([A-Z][A-Z0-9_]*)\s+\(\s*1\s*<<\s*(\d+)\s*\)")
    paths = [os.path.join(d, n) for d in CONSTANTS_DIRS if os.path.isdir(d) for n in sorted(os.listdir(d))]
    for path in paths:
        if not path.endswith(".h"):
            continue
        for line in open(path):
            m = define.match(line)
            value = None
            if m:
                value = parse_value(m.group(2))
            else:
                m = shift.match(line)
                if m:
                    value = 1 << int(m.group(2))
            if value is None:
                continue
            best = max((p for p in prefixes if m.group(1).startswith(p)), key=len, default=None)
            # A table length (CRATE_KIND_COUNT) isn't one of the values.
            if best and not m.group(1).endswith("_COUNT"):
                by_prefix[best][value].append(m.group(1))
    return by_prefix


def names_for(names, hit):
    v = parse_value(hit.literal)
    return [n for p in hit.prefixes for n in names[p].get(v, [])]


def directory(rel):
    parts = rel.split(os.sep)
    if parts[0] == "src" and len(parts) > 2:
        return os.path.join(*parts[:2]) if parts[1] != "vehicle" or len(parts) < 4 else os.path.join(*parts[:3])
    return parts[0]


def report(hits, names, out, samples=6):
    w = out.write
    w("# Magic numbers left (#655)\n\n")
    w("Generated by `tools/magic_numbers.py --report`. A site is an integer literal\n")
    w("at a known call argument or field; named constants aren't counted.\n")
    w("`named` counts the high-confidence sites whose value already has a name.\n\n")
    w("| Topic | What | high | medium | low | Total | high, named |\n|---|---|---:|---:|---:|---:|---:|\n")
    conf = collections.Counter((h.topic, h.confidence) for h in hits)
    named = collections.Counter(h.topic for h in hits if h.confidence == "high" and names_for(names, h))
    for topic, (desc, _) in TOPICS.items():
        row = [conf[(topic, c)] for c in CONFIDENCES]
        w(f"| `{topic}` | {desc} | " + " | ".join(map(str, row)) + f" | {sum(row)} | {named[topic]} |\n")
    tot = [sum(conf[(t, c)] for t in TOPICS) for c in CONFIDENCES]
    w("| | **Total** | " + " | ".join(f"**{x}**" for x in tot))
    w(f" | **{len(hits)}** | **{sum(named.values())}** |\n\n")

    topics = [t for t in TOPICS if any(h.topic == t for h in hits)]
    w("## Per directory (high / all)\n\n")
    w("| Directory | " + " | ".join(f"`{t}`" for t in topics) + " | Total |\n")
    w("|---|" + "---:|" * (len(topics) + 1) + "\n")
    grid = collections.Counter((directory(h.file), h.topic) for h in hits)
    hgrid = collections.Counter((directory(h.file), h.topic) for h in hits if h.confidence == "high")
    for d in sorted({directory(h.file) for h in hits}):
        cells = [f"{hgrid[(d, t)]} / {grid[(d, t)]}" if grid[(d, t)] else "" for t in topics]
        tot_h = sum(hgrid[(d, t)] for t in topics)
        tot = sum(grid[(d, t)] for t in topics)
        w(f"| {d} | " + " | ".join(cells) + f" | {tot_h} / {tot} |\n")
    w("\n")

    for topic in topics:
        th = [h for h in hits if h.topic == topic]
        w(f"## `{topic}`: {TOPICS[topic][0]}\n\n")
        if topic != "flag_test":
            values = collections.Counter(parse_value(h.literal) for h in th if h.confidence == "high")
            if values:
                w("High-confidence values (value: sites): ")
                w(", ".join(f"{v:#x}: {c}" for v, c in sorted(values.items())) + "\n\n")
        # The high-confidence sites, the ones whose value has a name first.
        high = sorted((h for h in th if h.confidence == "high"), key=lambda h: not names_for(names, h))
        pick = high[:samples] or th[:samples]
        for h in pick:
            known = names_for(names, h)
            hint = f" -> `{'/'.join(known)}`" if known else ""
            w(f"- `{h.file}:{h.line}` ({h.confidence}, {h.why}) `{h.literal}`{hint}: `{h.source}`\n")
        w("\n")


def fix(hits, names):
    """Replace each literal whose value has exactly one name with the
    site's prefixes. The name must expand to the same value: check the
    objects after."""
    by_file = collections.defaultdict(list)
    for h in hits:
        known = names_for(names, h)
        if len(known) == 1:
            by_file[h.file].append((h.offset, h.literal, known[0]))
    done = 0
    for rel, edits in sorted(by_file.items()):
        path = os.path.join(ROOT, rel)
        text = open(path, encoding="utf-8").read()
        for offset, lit, name in sorted(set(edits), reverse=True):
            if text[offset : offset + len(lit)] != lit:
                sys.exit(f"{rel}: expected {lit!r} at offset {offset}")
            text = text[:offset] + name + text[offset + len(lit) :]
            done += 1
        with open(path, "w", encoding="utf-8") as f:
            f.write(text)
        print(f"{rel}: {len(edits)}")
    print(f"{done} sites replaced", file=sys.stderr)
    return 0


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--topic", choices=list(TOPICS))
    ap.add_argument("--show", action="store_true", help="print the source line too")
    ap.add_argument("--values", action="store_true", help="per topic, each value and its count")
    ap.add_argument("--report", nargs="?", const="-", metavar="OUT", help="Markdown report")
    ap.add_argument("--list-topics", action="store_true")
    ap.add_argument(
        "--fix",
        action="store_true",
        help="with --topic: replace each high-confidence (or --min-confidence) literal that has exactly one name",
    )
    ap.add_argument("--min-confidence", choices=CONFIDENCES, help="only hits of this confidence or more")
    ap.add_argument("--path", action="append", metavar="P", help="only files under P (repeatable)")
    ap.add_argument("--json", action="store_true", help="the hits as JSON, for agents")
    ap.add_argument("--samples", type=int, default=8, help="--report: samples per topic (--sizes: per category)")
    g = ap.add_argument_group("--sizes (#820, see tools/magic_sizes.py)")
    g.add_argument("--sizes", action="store_true", help="sizes, strides, offsets and hardware values")
    g.add_argument(
        "--category",
        action="append",
        metavar="C",
        help="only category C: alloc, copy, stride, offset, hwaddr, hw (repeatable)",
    )
    g.add_argument("--db", metavar="FILE", help="type database JSON: read if it exists, else written")
    g.add_argument("--rebuild", action="store_true", help="recompile even if --db exists")
    g.add_argument("-j", "--jobs", type=int, default=os.cpu_count() or 4)
    args = ap.parse_args()

    if args.sizes:
        import magic_sizes

        for c in args.category or ():
            if c not in magic_sizes.CATEGORIES:
                ap.error(f"unknown category {c!r} (one of {', '.join(magic_sizes.CATEGORIES)})")
        return magic_sizes.run(args)

    if args.list_topics:
        for topic, (desc, _) in TOPICS.items():
            print(f"{topic:14} {desc}")
        return 0

    hits = scan(args.path)
    if args.topic:
        hits = [h for h in hits if h.topic == args.topic]
    if args.fix and not args.min_confidence:
        args.min_confidence = "high"
    if args.min_confidence:
        keep = CONFIDENCES[: CONFIDENCES.index(args.min_confidence) + 1]
        hits = [h for h in hits if h.confidence in keep]
    names = load_constants()

    if args.json:
        out = []
        for h in hits:
            d = h._asdict()
            d["value"] = parse_value(h.literal)
            d["names"] = names_for(names, h)
            d["prefixes"] = list(h.prefixes)
            out.append(d)
        json.dump(out, sys.stdout, indent=1)
        sys.stdout.write("\n")
        return 0

    if args.report:
        if args.report == "-":
            report(hits, names, sys.stdout, args.samples)
        else:
            with open(args.report, "w") as f:
                report(hits, names, f, args.samples)
        return 0

    if args.values:
        for topic in TOPICS:
            values = collections.Counter(parse_value(h.literal) for h in hits if h.topic == topic)
            if values:
                print(f"{topic}: " + ", ".join(f"{v:#x}: {c}" for v, c in sorted(values.items())))
        return 0

    if args.fix:
        if not args.topic:
            ap.error("--fix needs --topic")
        return fix(hits, names)
    for h in hits:
        known = names_for(names, h)
        hint = f"  -> {'/'.join(known)}" if known else ""
        print(
            f"{h.file}:{h.line}: {h.topic} {h.confidence} {h.literal}{hint}  [{h.why}]"
            + (f"\n    {h.source}" if args.show else "")
        )
    c = collections.Counter(h.confidence for h in hits)
    print(f"{len(hits)} sites: " + ", ".join(f"{k} {c[k]}" for k in CONFIDENCES if c[k]), file=sys.stderr)
    return 0


if __name__ == "__main__":
    sys.exit(main())
