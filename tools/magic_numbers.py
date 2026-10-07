#!/usr/bin/env python3
"""List the magic numbers left at known call sites and fields (#655).

Scans the C sources under src/ and include/ for integer literals in
places where the game has named values: sound and song IDs, level-flag
bits, mask levels, event IDs, action-controller states, entity and crate
kinds, actor-category exit statuses and `& 0xNN` flag tests. Each hit is
put under one topic. A value that is already a name (`SFX_CRATE_BREAK`,
`LEVEL_FLAG_CRATE_GEM`, ...) isn't a literal, so it isn't listed: the
counts go down as literals are replaced.

When include/constants/<topic>.h already has a define with the literal's
value and the topic's prefix, the listing shows it (`-> NAME`), so the
remaining sites of a topic that has a header can be finished off.

It doesn't run the preprocessor. Comments and strings are blanked out
first, so a literal quoted in a comment isn't counted.

Usage:
  tools/magic_numbers.py                  every hit, file:line topic value
  tools/magic_numbers.py --topic T        only topic T (--list-topics)
  tools/magic_numbers.py --show           ... with the source line
  tools/magic_numbers.py --values         per topic, each value and its count
  tools/magic_numbers.py --report [OUT]   Markdown: counts per topic, then per
                                          subsystem and file (OUT or stdout)
  tools/magic_numbers.py --topic T --fix  replace each literal of topic T that
                                          has exactly one name in
                                          include/constants/ (then rebuild and
                                          compare the objects)
"""

import argparse
import collections
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SCAN_DIRS = ("src", "include")
CONSTANTS_DIR = os.path.join(ROOT, "include", "constants")

LIT = r"(?:0[xX][0-9a-fA-F]+|[0-9]+)[uUlL]*"
LIT_RE = re.compile(r"^-?\s*" + LIT + r"$")

# Topic -> (description, prefixes of its names in include/constants/).
TOPICS = collections.OrderedDict(
    [
        ("sfx", ("PlaySfx/PlayAmbientSfx/StopSfx sound-effect IDs", ("SFX_",))),
        ("song", ("PlaySong song IDs", ("SONG_",))),
        ("level_flags", ("levelFlags bits (GetCurrentLevelFlags, level_state.c)", ("LEVEL_FLAG_",))),
        ("mask_level", ("maskLevel values (SetMaskLevel, comparisons)", ("MASK_LEVEL_",))),
        ("event", ("NOTIFY / *HandleEvent event IDs", ("EVENT_",))),
        ("action_state", ("action-controller states (SetActionCtrlMode, ->state)", ("ACTION_",))),
        ("kind", ("entity / crate kinds (->kind comparisons, CreateCrate)", ("ENTITY_", "CRATE_KIND_"))),
        ("category_exit", ("SetActorCategoryExitStatus values", ("CATEGORY_EXIT_",))),
        ("flag_test", ("`& 0xNN` flag tests (not counted above)", ())),
    ]
)

# Calls: (topic, function name regex, argument index).
CALLS = [
    ("sfx", r"PlaySfx", 1),
    ("sfx", r"PlayAmbientSfx", 1),
    ("sfx", r"StopSfx", 1),
    ("song", r"PlaySong", 1),
    ("song", r"StartSong", 1),
    ("mask_level", r"SetMaskLevel", 1),
    ("event", r"NOTIFY", 2),
    ("event", r"\w*HandleEvent", 1),
    ("event", r"\w*HandleEvent", 2),
    ("event", r"PhysCall3", 3),
    ("action_state", r"SetActionCtrlMode", 1),
    ("action_state", r"SetCtrlMode", 1),
    ("kind", r"CreateCrate", 1),
    ("category_exit", r"SetActorCategoryExitStatus", 0),
]

# Field patterns: (topic, regex with a `v` group for the literal, path
# prefix the rule is limited to or None). `->state` is a common field
# name, so only the player's action controller's is counted.
CMP = r"(?:==|!=|<=|>=|<|>|=(?!=))"
FIELDS = [
    ("song", r"\b(?:currentSong|pendingSong)\s*" + CMP + r"\s*(?P<v>" + LIT + r")\b", None),
    ("mask_level", r"\bmaskLevel\s*" + CMP + r"\s*(?P<v>" + LIT + r")\b", None),
    (
        "action_state",
        r"(?:->|\.)(?:state|prevState)\s*(?:==|!=)\s*(?P<v>" + LIT + r")\b",
        os.path.join("src", "player", ""),
    ),
    ("kind", r"(?:->|\.)kind\s*(?:==|!=)\s*(?P<v>" + LIT + r")\b", None),
    (
        "level_flags",
        r"Get(?:Current)?LevelFlags\s*\([^;]*?\)\s*(?:&|\|=)\s*(?P<v>" + LIT + r")\b",
        None,
    ),
]

# level_state.c's crate-gem helpers build the OR mask in a pinned register
# right after taking GetCurrentLevelFlags' slot: a literal assigned within
# this many lines of it is the bit.
LEVEL_FLAGS_WINDOW = 3
LEVEL_FLAGS_ASSIGN = re.compile(r"\bmask\b[^;=]*=\s*(?P<v>" + LIT + r")\s*;")

FLAG_TEST = re.compile(r"(?<![&])&\s*(?P<v>0[xX][0-9a-fA-F]+)[uUlL]*\b")
CALL_RE_CACHE = {}


def strip_comments(text):
    """Blank comments, strings and char literals, keeping the newlines."""
    out = []
    i, n = 0, len(text)
    while i < n:
        c = text[i]
        if text.startswith("/*", i):
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
            out.append(re.sub(r"[^\n]", " ", text[i:j]))
            i = j
        elif text.startswith("//", i):
            j = text.find("\n", i)
            j = n if j < 0 else j
            out.append(" " * (j - i))
            i = j
        elif c in "\"'":
            j = i + 1
            while j < n and text[j] != c:
                j += 2 if text[j] == "\\" else 1
            j = min(j + 1, n)
            out.append(c + re.sub(r"[^\n]", " ", text[i + 1 : j - 1]) + c)
            i = j
        else:
            out.append(c)
            i += 1
    return "".join(out)


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


def scan_file(path, rel):
    raw = open(path, encoding="utf-8", errors="replace").read()
    text = blank_directives(strip_comments(raw))
    src_lines = raw.split("\n")
    hits = []
    taken = set()

    def add(topic, offset, lit):
        line = line_of(text, offset)
        key = (line, offset)
        if key in taken:
            return
        taken.add(key)
        lead = len(lit) - len(lit.lstrip())
        hits.append((rel, line, topic, lit.strip(), src_lines[line - 1].strip(), offset + lead))

    for topic, name, index in CALLS:
        pat = CALL_RE_CACHE.setdefault(name, re.compile(r"\b(?:" + name + r")\s*\("))
        for m in pat.finditer(text):
            # Skip declarations/definitions: preceded by a type keyword.
            before = text[max(0, m.start() - 40) : m.start()]
            if re.search(r"\b(?:void|s32|u32|extern|static)\s*\**\s*$", before):
                continue
            args = split_args(text, m.end() - 1)
            if args is None or index >= len(args):
                continue
            off, arg = args[index]
            if LIT_RE.match(arg.strip()):
                add(topic, off, arg)

    for topic, pattern, prefix in FIELDS:
        if prefix and not rel.startswith(prefix):
            continue
        for m in re.finditer(pattern, text):
            add(topic, m.start("v"), m.group("v"))

    flag_lines = [line_of(text, m.start()) for m in re.finditer(r"\bGet(?:Current)?LevelFlags\s*\(", text)]
    if flag_lines:
        lines = text.split("\n")
        for fl in flag_lines:
            for ln in range(fl, min(fl + LEVEL_FLAGS_WINDOW, len(lines)) + 1):
                m = LEVEL_FLAGS_ASSIGN.search(lines[ln - 1])
                if m:
                    off = sum(len(l) + 1 for l in lines[: ln - 1]) + m.start("v")
                    add("level_flags", off, m.group("v"))

    counted = {(h[0], h[1]) for h in hits}
    for m in FLAG_TEST.finditer(text):
        line = line_of(text, m.start())
        if (rel, line) in counted:
            continue
        add("flag_test", m.start("v"), m.group("v"))

    return hits


def load_constants():
    """value -> [names] per prefix, from include/constants/*.h."""
    by_prefix = collections.defaultdict(lambda: collections.defaultdict(list))
    if not os.path.isdir(CONSTANTS_DIR):
        return by_prefix
    define = re.compile(r"^\s*#\s*define\s+([A-Z][A-Z0-9_]*)\s+\(?\s*(" + LIT + r")\s*\)?\s*(?:/[/*].*)?$")
    shift = re.compile(r"^\s*#\s*define\s+([A-Z][A-Z0-9_]*)\s+\(\s*1\s*<<\s*(\d+)\s*\)")
    for name in sorted(os.listdir(CONSTANTS_DIR)):
        if not name.endswith(".h"):
            continue
        for line in open(os.path.join(CONSTANTS_DIR, name)):
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
            for topic, (_, prefixes) in TOPICS.items():
                for p in prefixes:
                    if m.group(1).startswith(p):
                        by_prefix[topic][value].append(m.group(1))
    return by_prefix


def scan():
    hits = []
    for d in SCAN_DIRS:
        for dirpath, dirnames, filenames in os.walk(os.path.join(ROOT, d)):
            dirnames.sort()
            if os.path.relpath(dirpath, ROOT).startswith(os.path.join("include", "constants")):
                continue
            for f in sorted(filenames):
                if f.endswith((".c", ".h")):
                    path = os.path.join(dirpath, f)
                    hits += scan_file(path, os.path.relpath(path, ROOT))
    hits.sort(key=lambda h: (list(TOPICS).index(h[2]), h[0], h[1]))
    return hits


def subsystem(rel):
    parts = rel.split(os.sep)
    if parts[0] == "src" and len(parts) > 2:
        return parts[1]
    return parts[0]


def report(hits, out):
    w = out.write
    by_topic = collections.Counter(h[2] for h in hits)
    files_by_topic = collections.defaultdict(collections.Counter)
    for h in hits:
        files_by_topic[h[2]][h[0]] += 1

    w("# Magic numbers left (#655)\n\n")
    w("Generated by `tools/magic_numbers.py --report`. A site is an integer literal\n")
    w("at a known call argument or field; named constants aren't counted.\n\n")
    w("| Topic | What | Sites | Files |\n|---|---|---:|---:|\n")
    for topic, (desc, _) in TOPICS.items():
        w(f"| `{topic}` | {desc} | {by_topic[topic]} | {len(files_by_topic[topic])} |\n")
    w(f"| | **Total** | **{len(hits)}** | **{len({h[0] for h in hits})}** |\n\n")

    w("## Per subsystem\n\n")
    subs = sorted({subsystem(h[0]) for h in hits})
    topics = [t for t in TOPICS if by_topic[t]]
    w("| Subsystem | " + " | ".join(f"`{t}`" for t in topics) + " | Total |\n")
    w("|---|" + "---:|" * (len(topics) + 1) + "\n")
    grid = collections.Counter((subsystem(h[0]), h[2]) for h in hits)
    for s in subs:
        row = [grid[(s, t)] for t in topics]
        w(f"| {s} | " + " | ".join(str(c) if c else "" for c in row) + f" | {sum(row)} |\n")
    w("\n")

    for topic in topics:
        desc = TOPICS[topic][0]
        w(f"## `{topic}`: {desc}\n\n")
        values = collections.Counter(parse_value(h[3]) for h in hits if h[2] == topic)
        if topic != "flag_test":
            w("Values (value: sites): " + ", ".join(f"{v:#x}: {c}" for v, c in sorted(values.items())) + "\n\n")
        w("| File | Sites |\n|---|---:|\n")
        for f, c in sorted(files_by_topic[topic].items(), key=lambda x: (-x[1], x[0])):
            w(f"| {f} | {c} |\n")
        w("\n")


def fix(hits, names):
    """Replace each literal whose value has exactly one name in the topic.
    The name must expand to the same value: check the objects after."""
    by_file = collections.defaultdict(list)
    for h in hits:
        known = names[h[2]].get(parse_value(h[3]), [])
        if len(known) == 1:
            by_file[h[0]].append((h[5], h[3], known[0]))
    done = 0
    for rel, edits in sorted(by_file.items()):
        path = os.path.join(ROOT, rel)
        text = open(path, encoding="utf-8").read()
        for offset, lit, name in sorted(edits, reverse=True):
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
        help="with --topic: replace each literal that has exactly one name in include/constants/",
    )
    args = ap.parse_args()

    if args.list_topics:
        for topic, (desc, _) in TOPICS.items():
            print(f"{topic:14} {desc}")
        return 0

    hits = scan()
    if args.topic:
        hits = [h for h in hits if h[2] == args.topic]

    if args.report:
        if args.report == "-":
            report(hits, sys.stdout)
        else:
            with open(args.report, "w") as f:
                report(hits, f)
        return 0

    if args.values:
        for topic in TOPICS:
            values = collections.Counter(parse_value(h[3]) for h in hits if h[2] == topic)
            if values:
                print(f"{topic}: " + ", ".join(f"{v:#x}: {c}" for v, c in sorted(values.items())))
        return 0

    names = load_constants()
    if args.fix:
        if not args.topic:
            ap.error("--fix needs --topic")
        return fix(hits, names)
    for rel, line, topic, lit, src, _ in hits:
        known = names[topic].get(parse_value(lit))
        hint = f"  -> {'/'.join(known)}" if known else ""
        print(f"{rel}:{line}: {topic} {lit}{hint}" + (f"\n    {src}" if args.show else ""))
    print(f"{len(hits)} sites", file=sys.stderr)
    return 0


if __name__ == "__main__":
    sys.exit(main())
