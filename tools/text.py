#!/usr/bin/env python3
"""text/: the game's text as gettext files (docs/text.md, #1070).

text/crashxs.pot is the template: one entry per string, its key in
`msgctxt`, the English text as `msgid` and translator comments (`#.`).
text/<lang>.po has a language's text (fr, de, es, it, nl). The build
compiles each .po with msgfmt and generates the C++ of the three string
tables and the credits from the template and the .mo files.

    tools/text.py cpp TABLE OUT [--mo DIR]
        Writes the C++ definitions of TABLE (ui_text,
        cutscene_text_english, cutscene_text_translations or
        credits_text), which src/.../data/TABLE.cpp #includes, reading each
        language's text from DIR/<lang>.mo with the stdlib gettext module
        (pgettext).

    tools/text.py check
        The translations' checks: `msgfmt --check --statistics` for each
        .po (fails on an untranslated or fuzzy entry), `msgcmp` both ways
        against the template (fails on a missing or extra key), and the
        generator's own checks (every character in the game's font), and
        that each .po's credits.* entries are the English, unchanged.

    tools/text.py extract [baserom.gba] [--check]
        Writes text/crashxs.pot and the .po files from the ROM's tables
        (the credits' keys from data/credits.json),
        through msgcat so they are in gettext's own layout. With --check,
        writes them into a temporary directory and compares them with
        text/ byte for byte.

    tools/text.py --game n2 cpp|check|extract ...
        N-Tranced's text (docs/n_tranced.md, "Text"): games/n2/text/
        (ntranced.pot and the .po files), its UI text only (ui_text, 196
        ids; extract reads baserom_n2.gba). Its keys are ui.level.<level>
        for the level names (levels.json's "level_names", by the level
        table's name_text) and ui.text.<id> for the others, until they
        have names. Its font isn't known yet, so the font checks are XS's
        only.

The tables (docs/data.md, "Text tables"):

- ui_text: per language (English, French, German, Spanish,
  Italian, Dutch) the strings it adds, then its `gUiText<Lang>` array of
  UI_TEXT_COUNT pointers, which GetUiText indexes by text id. The
  template's `ui.*` entries are in text id order.
- cutscene_text_english: the English cutscene pages, each page's strings
  then its `gCutscene<NN>EnglishPage<P>` array; cutscene_text_translations the
  other five languages' pages the same way. The template's
  `cutscene<NN>.page<P>.<I>` entries are in ROM order.
- credits_text: gCreditsText, the credits' one stream (every language
  shows it): the template's credits.* entries, in English (each .po has
  them too, as copies of the English that check() holds equal), in the
  order data/credits.json lists them, with the font switches and the logos
  it gives.

A string is stored once, under its first use in this order; a later use
(another language, or another id) points at it. The ROM has no repeat
stored twice. Every pointer is spelled `(const u8 *)&gX`: g++ 2.9's
decay_conversion turns a plain `gX` (and `&gX[0]`, `gX + 0`, ...) into
a pointer to a new copy of the string literal.
"""

import argparse
import difflib
import filecmp
import gettext
import json
import os
import re
import shutil
import struct
import subprocess
import sys
import tempfile
import textwrap

ROOT = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
TEXT_DIR = os.path.join(ROOT, "text")
POT = os.path.join(TEXT_DIR, "crashxs.pot")
FONTS_SRC = os.path.join(ROOT, "src", "game", "ui", "data", "hud_font_tables.cpp")
LEVELS_JSON = os.path.join(ROOT, "data", "levels", "levels.json")
GRAPHICS_MANIFEST = os.path.join(ROOT, "graphics", "manifest.json")
CREDITS_JSON = os.path.join(ROOT, "data", "credits.json")

# gLanguage's order: the languages of gUiTextTables and gCutsceneTexts.
LANGUAGES = [("en", "English"), ("fr", "French"), ("de", "German"), ("es", "Spanish"), ("it", "Italian"),
             ("nl", "Dutch")]
TRANSLATIONS = [code for code, _ in LANGUAGES[1:]]

TABLES = ["ui_text", "cutscene_text_english", "cutscene_text_translations", "credits_text"]

# The text's encoding: Latin-1, and the font's 0x9C is "œ", Windows-1252's
# code for it. A character the encoding lacks is an error; one the font
# lacks too (below).
ENCODING = "cp1252"

# The UI strings the game also draws in the large font (gLargeFont): the
# level names (the level select's title), the save menu's titles
# (SaveMenu::DrawTitle) and the powers' names (the power dialog's title).
LARGE_FONT_KEYS = {"ui.menu.load_game", "ui.save.load_link_game", "ui.save.delete_game", "ui.save.save_game",
                   "ui.power.super_body_slam", "ui.power.double_jump", "ui.power.tornado_spin",
                   "ui.power.turbo_run"}


def large_font(key):
    return key.startswith("ui.level.") or key in LARGE_FONT_KEYS


KEY_UI = re.compile(r"^ui\.[a-z0-9_.]+$")
KEY_CUTSCENE = re.compile(r"^cutscene(\d\d)\.page(\d+)\.(\d+)$")
KEY_CREDITS = re.compile(r"^credits\.[a-z0-9_.]+$")


def fail(msg):
    sys.exit("text.py: " + msg)


def rel(path):
    return os.path.relpath(path, ROOT)


# --- PO files -------------------------------------------------------------

def po_unquote(s):
    out = []
    i = 0
    esc = {"n": "\n", "t": "\t", '"': '"', "\\": "\\", "r": "\r"}
    while i < len(s):
        c = s[i]
        if c == "\\" and i + 1 < len(s):
            if s[i + 1] not in esc:
                raise ValueError("unknown escape \\" + s[i + 1])
            out.append(esc[s[i + 1]])
            i += 2
        else:
            out.append(c)
            i += 1
    return "".join(out)


def read_po(path):
    """The entries of a PO file, in order: dicts with `ctx`, `id`, `str`,
    `comments` (the `#.` lines) and `flags`. Obsolete (`#~`) entries and
    the header are left out."""
    entries = []
    cur = None
    field = None

    def finish():
        if cur is not None and "id" in cur and not cur.get("obsolete"):
            if cur["id"] != "" or cur.get("ctx") is not None:
                entries.append(cur)

    with open(path, encoding="utf-8") as f:
        lines = f.read().split("\n")
    for n, line in enumerate(lines, 1):
        line = line.strip()
        if not line:
            finish()
            cur, field = None, None
            continue
        if cur is None:
            cur = {"ctx": None, "comments": [], "flags": []}
        if line.startswith("#~"):
            cur["obsolete"] = True
            continue
        if line.startswith("#."):
            cur["comments"].append(line[3:] if line.startswith("#. ") else line[2:])
            continue
        if line.startswith("#,"):
            cur["flags"] += [x.strip() for x in line[2:].split(",")]
            continue
        if line.startswith("#"):
            continue
        m = re.match(r'^(msgctxt|msgid|msgstr)\s+"(.*)"$', line)
        if m:
            field = {"msgctxt": "ctx", "msgid": "id", "msgstr": "str"}[m.group(1)]
            if field in cur and cur[field] is not None:
                fail("%s:%d: a second %s" % (path, n, m.group(1)))
            cur[field] = po_unquote(m.group(2))
            continue
        m = re.match(r'^"(.*)"$', line)
        if m and field:
            cur[field] += po_unquote(m.group(1))
            continue
        fail("%s:%d: can't read %r" % (path, n, line))
    finish()
    return entries


def po_quote(s):
    return '"' + s.replace("\\", "\\\\").replace('"', '\\"').replace("\n", "\\n").replace("\t", "\\t") + '"'


# --- The font -------------------------------------------------------------

def read_c_string(src, name):
    m = re.search(re.escape(name) + r'\[\]\s*=\s*((?:"(?:[^"\\]|\\.)*"\s*)+);', src)
    if not m:
        fail("%s: no %s" % (FONTS_SRC, name))
    raw = "".join(re.findall(r'"((?:[^"\\]|\\.)*)"', m.group(1)))
    return re.sub(r"\\([0-7]{1,3})", lambda x: chr(int(x.group(1), 8)), raw).encode("latin-1").decode(
        "unicode_escape").encode("latin-1")


def read_glyph_widths(src, name):
    m = re.search(re.escape(name) + r"\[\d+\]\s*=\s*\{(.*?)\n\};", src, re.S)
    if not m:
        fail("%s: no %s" % (FONTS_SRC, name))
    return [int(w) for w in re.findall(r"^\s*\{\s*(\d+),", m.group(1), re.M)]


class Font:
    """One of the HUD fonts: its characters (gSmallFontChars or
    gLargeFontChars, src/game/ui/data/hud_font_tables.cpp) and their widths."""

    def __init__(self, src, chars, glyphs, space):
        self.chars = read_c_string(src, chars)
        self.widths = read_glyph_widths(src, glyphs)
        self.space = space

    def missing(self, data):
        return sorted(set(b for b in data if b not in self.chars and b not in b" \n"))

    def width(self, data):
        """Font::MeasureText: the widest line's advance."""
        best = cur = 0
        for b in data:
            if b == 0x20:
                cur += self.space
            elif b == 0x0A:
                best, cur = max(best, cur), 0
            else:
                i = self.chars.index(b) if b in self.chars else 0
                cur += self.widths[i]
        return max(best, cur)


def fonts():
    if GAME != "xs":
        return None, None  # its font isn't known yet: no font checks
    with open(FONTS_SRC, encoding="latin-1") as f:
        src = f.read()
    return (Font(src, "gSmallFontChars", "gSmallFontGlyphs", 4),
            Font(src, "gLargeFontChars", "gLargeFontGlyphs", 6))


def show_bytes(data):
    return ", ".join(repr(bytes([b]).decode(ENCODING, "replace")) for b in data)


def encode(text, where, small, large, need_large):
    """`text` as the ROM's bytes, or an error naming `where`."""
    try:
        data = text.encode(ENCODING)
    except UnicodeEncodeError as e:
        fail("%s: %r can't be written in the game's encoding (Latin-1)" % (where, e.object[e.start:e.end]))
    if 0 in data:
        fail("%s: a NUL character" % where)
    missing = small.missing(data) if small else []
    if missing:
        fail("%s: the font has no %s (the font's characters: lower case letters, digits, %s)" % (
            where, show_bytes(missing), "the buttons A, B, L, R and the punctuation of gSmallFontChars"))
    if need_large and large:
        missing = large.missing(data)
        if missing:
            fail("%s: the large font, which the game also draws this in, has no %s" % (
                where, show_bytes(missing)))
    return data


# --- The template and the translations ------------------------------------

def read_template():
    """The template's entries: the UI ones (in text id order) and the
    cutscene pages (in ROM order), each page a list of its entries. The
    credits.* entries are credits_entries()'."""
    ui, pages = [], []
    seen = set()
    for e in read_po(POT):
        key = e["ctx"]
        if key is None:
            fail("%s: an entry without a key (msgctxt): %r" % (POT, e["id"]))
        if key in seen:
            fail("%s: the key %s twice" % (POT, key))
        seen.add(key)
        m = KEY_CUTSCENE.match(key)
        if KEY_CREDITS.match(key):
            continue
        if KEY_UI.match(key):
            ui.append(e)
        elif m:
            c, p, i = (int(x) for x in m.groups())
            if pages and pages[-1]["cutscene"] == c and pages[-1]["page"] == p:
                page = pages[-1]
            else:
                if any(pg["cutscene"] == c and pg["page"] == p for pg in pages):
                    fail("%s: cutscene %d page %d's strings aren't together" % (POT, c, p))
                page = {"cutscene": c, "page": p, "entries": []}
                pages.append(page)
            if i != len(page["entries"]):
                fail("%s: %s: expected string %d of the page" % (POT, key, len(page["entries"])))
            page["entries"].append(e)
        else:
            fail("%s: %s is neither ui.*, cutsceneNN.pageP.I nor credits.*" % (POT, key))
    return ui, pages


def level_key(lv):
    """A level's name's key: ui.level.<level>, the level's constant without
    LEVEL_, in lower case (LEVEL_JUNGLE_JAM: ui.level.jungle_jam)."""
    return "ui.level." + lv["constant"][len("LEVEL_"):].lower()


def check_level_names(ui):
    """Each level's name_text (levels.json) must be the text id of its own
    ui.level.<level> entry, and each ui.level.* entry a level's name: the
    UI entries' order is their text ids, so an entry inserted in the middle
    would shift every later id, and a level pointing at another level's
    name, or a name no level uses, is a slip in the data."""
    with open(LEVELS_JSON, encoding="utf-8") as f:
        levels = json.load(f)["levels"]
    for lv in levels:
        t = lv["name_text"]
        if t >= len(ui) or not ui[t]["ctx"].startswith("ui.level."):
            fail("%s: %s's name_text %d is %s, not a ui.level.* entry (the ui.* entries are in text id order)"
                 % (POT, lv["constant"], t, ui[t]["ctx"] if t < len(ui) else "past the last one"))
        if ui[t]["ctx"] != level_key(lv):
            ids = [i for i, e in enumerate(ui) if e["ctx"] == level_key(lv)]
            fail("%s: %s's name_text %d is %s, not its own %s%s" % (
                POT, lv["constant"], t, ui[t]["ctx"], level_key(lv),
                " (text id %d)" % ids[0] if ids else ", which the template doesn't have: add it after the last "
                "ui.* entry"))
    used = {lv["name_text"] for lv in levels}
    for i, e in enumerate(ui):
        if e["ctx"].startswith("ui.level.") and i not in used:
            fail("%s: %s (text id %d) is no level's name_text" % (POT, e["ctx"], i))


def load_translations(mo_dir):
    out = {}
    for code in TRANSLATIONS:
        path = os.path.join(mo_dir, code + ".mo")
        with open(path, "rb") as f:
            out[code] = gettext.GNUTranslations(f)
    return out


def translate(tr, code, entry):
    """The language's text for a template entry: pgettext, after checking
    the .mo has it (msgfmt leaves untranslated and fuzzy entries out, and
    pgettext would return the English)."""
    ctx, msgid = entry["ctx"], entry["id"]
    if (ctx + "\x04" + msgid) not in tr._catalog:
        fail("text/%s.po: %s is untranslated or fuzzy (where the game keeps the English, msgstr is the "
             "English text)" % (code, ctx))
    return tr.pgettext(ctx, msgid)


# --- The C++ --------------------------------------------------------------

def c_string(data):
    out = []
    for b in data:
        if b == 0x22:
            out.append('\\"')
        elif b == 0x5C:
            out.append("\\\\")
        elif b == 0x0A:
            out.append("\\n")
        elif 0x20 <= b < 0x7F:
            out.append(chr(b))
        else:
            out.append("\\%03o" % b)
    return '"' + "".join(out) + '"'


class Strings:
    """The strings stored so far: each distinct one once, under its first
    use."""

    def __init__(self):
        self.symbol = {}

    def use(self, data, name, lines):
        if data not in self.symbol:
            self.symbol[data] = name
            lines.append("extern const u8 %s[] = %s;" % (name, c_string(data)))
        return self.symbol[data]


def pointer_array(name, symbols, size=None):
    lines = ["extern const u8 *const %s[%s] = {" % (name, size if size is not None else len(symbols))]
    lines += ["    (const u8 *)&%s," % s for s in symbols]
    lines.append("};")
    return lines


def header():
    return ["/* Generated by tools/text.py from %s and %s/*.po" % (rel(POT), rel(TEXT_DIR)),
            " * (docs/text.md): don't edit. */",
            ""]


def ui_table(ui, tr, small, large):
    out = header()
    strings = Strings()
    for code, lang in LANGUAGES:
        lines, symbols = [], []
        for i, e in enumerate(ui):
            text = from_po(e["id"] if code == "en" else translate(tr[code], code, e))
            data = encode(text, "%s (%s)" % (e["ctx"], code), small, large, large_font(e["ctx"]))
            symbols.append(strings.use(data, "gUiText%s%02d" % (lang, i), lines))
        out += lines + [""] + pointer_array("gUiText" + lang, symbols) + [""]
    return out


def page_symbol(page, lang):
    return "gCutscene%02d%sPage%d" % (page["cutscene"], lang, page["page"])


def cutscene_language(pages, code, lang, tr, strings, small, large):
    lines = []
    for page in pages:
        symbols = []
        multi = len(page["entries"]) > 1
        for i, e in enumerate(page["entries"]):
            text = e["id"] if code == "en" else translate(tr[code], code, e)
            data = encode(text, "%s (%s)" % (e["ctx"], code), small, large, False)
            name = page_symbol(page, lang) + ("_%d" % i if multi else "") + "Text"
            symbols.append(strings.use(data, name, lines))
        lines += pointer_array(page_symbol(page, lang), symbols)
    return lines


def cutscene_tables(pages, tr, small, large):
    """The English table's lines and the other languages', which point at
    an English string where they have the same text."""
    strings = Strings()
    english = cutscene_language(pages, "en", "English", tr, strings, small, large)
    defined = set(strings.symbol.values())
    if tr is None:
        return header() + english, None
    others = []
    for code, lang in LANGUAGES[1:]:
        others += cutscene_language(pages, code, lang, tr, strings, small, large)
    used = sorted(set(re.findall(r"&(gCutscene\d\dEnglish\w+)", "\n".join(others))) & defined,
                  key=lambda s: list(strings.symbol.values()).index(s))
    externs = ["extern const u8 %s[];" % s for s in used]
    return header() + english, header() + externs + ([""] if externs else []) + others


# --- The credits ----------------------------------------------------------

# The credits stream's opcodes (include/game/ui/frontend.hpp, CREDITS_OP_*)
# and its logos (Credits::logos, gCreditsLogos).
CREDITS_OP_PICTURE, CREDITS_OP_SMALL, CREDITS_OP_LARGE = 1, 2, 3
CREDITS_LOGOS = 5


def read_credits():
    """data/credits.json's items, checked: each is {"large": key},
    {"small": key} (the font switch, then the text), {"text": key} (the
    text in the font so far), {"picture": n} (logo n) or {"blank": n} (n
    empty lines). Each text item is a line, or lines, then a newline."""
    where = rel(CREDITS_JSON)
    with open(CREDITS_JSON, encoding="utf-8") as f:
        items = json.load(f).get("credits")
    if not isinstance(items, list) or not items:
        fail("%s: no \"credits\" list" % where)
    for i, it in enumerate(items):
        if not isinstance(it, dict) or len(it) != 1:
            fail("%s: item %d: not one of {\"large\"|\"small\"|\"text\": key}, {\"picture\": n}, {\"blank\": n}"
                 % (where, i))
        (k, v), = it.items()
        if k in ("large", "small", "text"):
            if not isinstance(v, str) or not KEY_CREDITS.match(v):
                fail("%s: item %d: %r isn't a credits.* key" % (where, i, v))
        elif k == "picture":
            if not isinstance(v, int) or isinstance(v, bool) or not 0 <= v < CREDITS_LOGOS:
                fail("%s: item %d: picture %r isn't a logo (0 to %d)" % (where, i, v, CREDITS_LOGOS - 1))
        elif k == "blank":
            if not isinstance(v, int) or isinstance(v, bool) or v < 1:
                fail("%s: item %d: blank %r isn't a number of lines" % (where, i, v))
        else:
            fail("%s: item %d: unknown item %r" % (where, i, k))
    return items


def credits_entries(path=POT):
    """The credits.* entries of the template (or of a .po) by key."""
    entries = {}
    for e in read_po(path):
        if e["ctx"] is None or not KEY_CREDITS.match(e["ctx"]):
            continue
        if e["ctx"] in entries:
            fail("%s: the key %s twice" % (rel(path), e["ctx"]))
        entries[e["ctx"]] = e
    return entries


def check_credits_copies(code):
    """The credits are shown in English in every language (Credits::Init
    reads gCreditsText, the template's text), so a .po's credits.* entries
    are copies of the English: each must be there, not fuzzy, with msgstr
    the template's msgid. The problems, as messages."""
    po = os.path.join(TEXT_DIR, code + ".po")
    mine = credits_entries(po)
    bad = []
    for key, e in credits_entries().items():
        t = mine.get(key)
        if t is None:
            bad.append("%s: no %s entry" % (rel(po), key))
        elif "fuzzy" in t["flags"]:
            bad.append("%s: %s is fuzzy" % (rel(po), key))
        elif t["id"] != e["id"]:
            bad.append("%s: %s's msgid isn't the template's (run `make text-update`)" % (rel(po), key))
        elif t["str"] != e["id"]:
            bad.append("%s: %s's msgstr %r isn't the English %r" % (rel(po), key, t["str"], e["id"]))
    if bad:
        bad.append("%s: the credits are shown in English in every language: each credits.* msgstr is "
                   "the English msgid, unchanged (docs/text.md, \"The credits\")" % rel(po))
    return bad


def credits_stream(small, large):
    """gCreditsText's lines, as (bytes, what) pairs: the opcodes and text
    of each line, its newline included."""
    items = read_credits()
    entries = credits_entries()
    used = set()
    font = "small"  # Credits::Init: largeFont = 0
    out = []
    for it in items:
        (k, v), = it.items()
        if k == "blank":
            out += [(b"\n", None)] * v
        elif k == "picture":
            out.append((bytes([CREDITS_OP_PICTURE, v]) + b"\n", None))
        else:
            if v not in entries:
                fail("%s: %s isn't in %s" % (rel(CREDITS_JSON), v, rel(POT)))
            used.add(v)
            op = b""
            if k != "text":
                font = k
                op = bytes([CREDITS_OP_LARGE if k == "large" else CREDITS_OP_SMALL])
            text = entries[v]["id"]
            if not text:
                fail("%s: %s is empty (a blank line is {\"blank\": 1})" % (rel(POT), v))
            try:
                data = text.encode(ENCODING)
            except UnicodeEncodeError as e:
                fail("%s: %r can't be written in the game's encoding (Latin-1)" % (v, e.object[e.start:e.end]))
            if any(b < 0x20 and b != 0x0A for b in data):
                fail("%s: a control character" % v)
            missing = (large if font == "large" else small).missing(data)
            if missing:
                fail("%s: the %s font, which the credits draw it in, has no %s" % (v, font, show_bytes(missing)))
            out.append((op + data + b"\n", v))
    unused = sorted(set(entries) - used)
    if unused:
        fail("%s: %s: in no item of %s" % (rel(POT), ", ".join(unused), rel(CREDITS_JSON)))
    return out


def credits_table(small, large):
    out = ["/* Generated by tools/text.py from text/crashxs.pot and data/credits.json",
           " * (docs/text.md, \"The credits\"): don't edit. gCreditsText's initializer:",
           " * one string per line, \\001 n before a logo's line (n: the logo),",
           " * \\002 or \\003 before a line in the small or large font. */"]
    for data, key in credits_stream(small, large):
        out.append("    %s%s" % (c_string(data), "" if key is None else " // " + key))
    return out


def generate(table, mo_dir):
    small, large = fonts()
    if table == "credits_text":
        return credits_table(small, large)
    ui, pages = read_template()
    if table == "ui_text":
        if GAME == "xs":  # N-Tranced's level names' keys come from its ROM's ids (extract)
            check_level_names(ui)
        return ui_table(ui, load_translations(mo_dir), small, large)
    if table == "cutscene_text_english":
        return cutscene_tables(pages, None, small, large)[0]
    if table == "cutscene_text_translations":
        return cutscene_tables(pages, load_translations(mo_dir), small, large)[1]
    fail("no table " + table)


def write_lines(path, lines):
    text = "\n".join(lines).rstrip("\n") + "\n"
    os.makedirs(os.path.dirname(os.path.abspath(path)), exist_ok=True)
    with open(path, "w", encoding="ascii", newline="\n") as f:
        f.write(text)


# --- check ----------------------------------------------------------------

def run(cmd):
    # The C locale: check() reads msgfmt's statistics, which are translated.
    env = dict(os.environ, LC_ALL="C", LANGUAGE="")
    try:
        return subprocess.run(cmd, capture_output=True, text=True, env=env)
    except FileNotFoundError:
        fail("%s not found: the gettext tools are needed (the devshell has them; apt: gettext)" % cmd[0])


def check():
    bad = []
    with tempfile.TemporaryDirectory() as tmp:
        for code in TRANSLATIONS:
            po = os.path.join(TEXT_DIR, code + ".po")
            rel = os.path.relpath(po, ROOT)
            r = run(["msgfmt", "--check", "--statistics", "-o", os.path.join(tmp, code + ".mo"), po])
            stats = r.stderr.strip()
            print("%s: %s" % (rel, stats.splitlines()[-1] if stats else "(no statistics)"))
            if r.returncode:
                bad.append("%s: msgfmt --check failed:\n%s" % (rel, r.stderr.rstrip()))
                continue
            m = re.search(r"(\d+) fuzzy", stats)
            if m and int(m.group(1)):
                bad.append("%s: %s fuzzy entries (review them, then drop the fuzzy flag)" % (rel, m.group(1)))
            m = re.search(r"(\d+) untranslated", stats)
            if m and int(m.group(1)):
                bad.append("%s: %s untranslated entries (where the game keeps the English, msgstr is the "
                           "English text)" % (rel, m.group(1)))
            # Every key of the template translated in the .po ...
            r = run(["msgcmp", po, POT])
            if r.returncode:
                bad.append("%s: msgcmp against the template:\n%s" % (rel, r.stderr.rstrip()))
            # ... and no key the template doesn't have.
            r = run(["msgcmp", "--use-untranslated", POT, po])
            if r.returncode:
                bad.append("%s: keys the template doesn't have (run `make text-update`):\n%s"
                           % (rel, r.stderr.rstrip()))
            # The credits: copies of the English.
            if GAME == "xs":
                bad += check_credits_copies(code)
        if bad:
            fail("\n".join(bad))
        # The generator's own checks: the encoding, the font, the order.
        for table in TABLES:
            generate(table, tmp)
    print("text: OK")


# --- extract --------------------------------------------------------------

ROM_BASE = 0x08000000
# The tables in the ROM: gUiText<Lang> (70 pointers each),
# gCutsceneText<Lang> (11 `struct cutscene_page` arrays each) and
# gCutscenes (11 `struct cutscene_slides`, the page counts).
UI_TABLES = [0x081730D8, 0x08173630, 0x08173B70, 0x081740D0, 0x0817462C, 0x08174AC8]
UI_ROM_COUNT = 70
CUTSCENE_TABLES = [0x0816D1C8, 0x0816D36C, 0x0816D398, 0x0816D3C4, 0x0816D3F0, 0x0816D41C]
CUTSCENES = 0x0816D1F4
CUTSCENE_COUNT = 11

# The UI text ids that aren't level names (levels.json's name_text gives
# those their keys, ui.level.<level>): the key and what the game shows it
# as. Text ids are GetUiText's argument.
UI_KEYS = {
    0: ("ui.pause.level", "The label before the level's number on the pause menu's info page."),
    26: ("ui.title.new_game", "Title screen menu item."),
    27: ("ui.menu.load_game", "Title screen menu item, and the save menu's first option and the title of "
                              "its screen (large font)."),
    28: ("ui.save.load_link_game", "Save menu option, and the title of its screen (large font)."),
    29: ("ui.save.delete_game", "Save menu option, and the title of its screen (large font)."),
    30: ("ui.save.save_game", "Save menu option, and the title of its screen (large font)."),
    31: ("ui.menu.quit", "Menu label."),
    32: ("ui.save.exit", "Save menu option: leave the save menu."),
    33: ("ui.save.error_saving", "Save menu message."),
    34: ("ui.save.save_ok", "Save menu message."),
    35: ("ui.save.cancel", "Save menu: the cancel label."),
    36: ("ui.save.complete", "Save menu: a save slot's completion."),
    37: ("ui.save.empty", "Save menu: an empty save slot."),
    38: ("ui.save.delete_prompt", "Save menu question before deleting a save."),
    39: ("ui.save.overwrite_prompt", "Save menu question before overwriting a save."),
    40: ("ui.continue.prompt", "The continue prompt's question (after a game over)."),
    41: ("ui.common.yes", "Answer to the save menu's and the continue prompt's questions."),
    42: ("ui.common.no", "Answer to the save menu's and the continue prompt's questions."),
    43: ("ui.link.transferring", "Link cable transfer message (first line)."),
    44: ("ui.link.transfer_error", "Link cable transfer message (first line)."),
    45: ("ui.link.abort_hint", "Link cable transfer message (second line). \"B\" draws the B button."),
    46: ("ui.common.push_start", "Second line of the link messages, and the power dialog's prompt."),
    47: ("ui.level_select.load_save_hint", "Level select hint: START opens the save menu."),
    48: ("ui.pause.music", "Pause menu row: the music volume."),
    49: ("ui.pause.sound", "Pause menu row: the sound effects volume."),
    50: ("ui.pause.resume", "Pause menu row."),
    51: ("ui.pause.warp_room", "Pause menu row: quit the level for the warp room (the level select)."),
    52: ("ui.pause.restart_trial", "Pause menu row: restart the time trial."),
    53: ("ui.pause.powers", "Pause menu page title."),
    54: ("ui.pause.crystals", "Pause menu page title."),
    55: ("ui.pause.gems", "Pause menu page title."),
    56: ("ui.pause.relics", "Pause menu page title."),
    57: ("ui.pause.time_trial", "Pause menu page title."),
    58: ("ui.pause.none", "Pause menu: shown on a page with nothing collected yet."),
    59: ("ui.title.credits", "Title screen menu item."),
    60: ("ui.power.super_body_slam", "A power's name: the power dialog's title (large font)."),
    61: ("ui.power.double_jump", "A power's name: the power dialog's title (large font)."),
    62: ("ui.power.tornado_spin", "A power's name: the power dialog's title (large font)."),
    63: ("ui.power.turbo_run", "A power's name: the power dialog's title (large font)."),
    64: ("ui.power.super_body_slam.help", "The power dialog's text, word-wrapped. \"R\" draws the R button."),
    65: ("ui.power.double_jump.help", "The power dialog's text, word-wrapped. \"A\" draws the A button."),
    66: ("ui.power.tornado_spin.help", "The power dialog's text, word-wrapped. \"B\" draws the B button."),
    67: ("ui.power.turbo_run.help", "The power dialog's text, word-wrapped. \"L\" draws the L button."),
    68: ("ui.misc.bonus", "No GetUiText call with this id has been found yet."),
    69: ("ui.misc.pause", "No GetUiText call with this id has been found yet."),
}

POT_HEADER = """\
# Crash Bandicoot XS: the game's text (docs/text.md).
#
# msgctxt is the key, msgid the English text. The ui.* entries are in
# GetUiText's text id order (append new ones); the cutscene ones are
# cutscene<NN>.page<P>.<I>, the strings of a page in the order they show;
# the credits ones are credits.<heading>[.names], which data/credits.json
# lists in the order the credits show them, with the fonts and the logos.
# The text is lower case: the fonts have no capital letters, and "A", "B",
# "L" and "R" draw the GBA's buttons. Characters are Latin-1 and must be in
# the font (tools/text.py checks).
"""

DATE = "2026-10-14 00:00+0000"


def po_header(code, lang):
    fields = [
        "Project-Id-Version: " + PROJECT,
        "Report-Msgid-Bugs-To: https://github.com/Almamu/CrashBandicootXS-decomp/issues",
        "POT-Creation-Date: " + DATE,
        "PO-Revision-Date: " + (DATE if code else "YEAR-MO-DA HO:MI+ZONE"),
        "Last-Translator: " + ("the original game's translators (extracted from the ROM)" if code
                               else "FULL NAME <EMAIL@ADDRESS>"),
        "Language-Team: " + (lang if code else "LANGUAGE"),
        "Language: " + (code or ""),
        "MIME-Version: 1.0",
        "Content-Type: text/plain; charset=UTF-8",
        "Content-Transfer-Encoding: 8bit",
    ]
    return ['msgid ""', 'msgstr ""'] + [po_quote(f + "\n") for f in fields]


class Rom:
    def __init__(self, path):
        with open(path, "rb") as f:
            self.data = f.read()

    def w(self, addr):
        return struct.unpack_from("<I", self.data, addr - ROM_BASE)[0]

    def string(self, addr):
        start = addr - ROM_BASE
        end = self.data.index(b"\0", start)
        return self.data[start:end].decode(ENCODING)


def extract_entries(rom):
    """The template's entries from the ROM: (key, English, {lang: text},
    comments), UI first, then the cutscene pages in ROM order."""
    small, large = fonts()
    with open(LEVELS_JSON, encoding="utf-8") as f:
        levels = json.load(f)["levels"]
    level_keys = {lv["name_text"]: lv for lv in levels}
    entries = []

    # The UI text.
    texts = [[rom.string(rom.w(t + 4 * i)) for i in range(UI_ROM_COUNT)] for t in UI_TABLES]
    for i in range(UI_ROM_COUNT):
        if i in level_keys:
            lv = level_keys[i]
            key = level_key(lv)
            what = "A level's name (%s): the level select's title (large font) and the pause menu's " \
                   "info page." % lv["constant"]
        else:
            key, what = UI_KEYS[i]
        enc = [t[i].encode(ENCODING) for t in texts]
        comments = ["Text id %d (0x%02X). %s" % (i, i, what),
                    "Widest shipped text: %d px in the small font%s; the screen is 240 px wide." % (
                        max(small.width(b) for b in enc),
                        ", %d px in the large font" % max(large.width(b) for b in enc) if large_font(key)
                        else "")]
        entries.append((key, texts[0][i], {code: texts[n + 1][i] for n, code in enumerate(TRANSLATIONS)},
                        comments))

    # The cutscenes: per language, page (cutscene, page) -> its strings'
    # array, and each array's first use.
    with open(GRAPHICS_MANIFEST, encoding="utf-8") as f:
        manifest = json.load(f)
    pictures = {int(e["palette"]["rom"], 16): e["path"] for e in manifest["assets"]
                if e.get("kind") == "bitmap" and e["path"].startswith("cutscenes/")}
    counts = [rom.w(CUTSCENES + 8 * c + 4) for c in range(CUTSCENE_COUNT)]
    langs = []
    for table in CUTSCENE_TABLES:
        arrays = {}
        uses = {}
        for c in range(CUTSCENE_COUNT):
            pages = rom.w(table + 4 * c)
            for p in range(counts[c]):
                ptr, n = rom.w(pages + 8 * p), rom.w(pages + 8 * p + 4)
                if ptr:
                    arrays.setdefault(ptr, (c, p, n))
                    uses.setdefault(ptr, []).append((c, p))
        langs.append((arrays, uses))
    english = sorted(langs[0][0].items())
    first = {(c, p): (ptr, n) for ptr, (c, p, n) in english}
    for li, (arrays, uses) in enumerate(langs[1:], 1):
        mine = sorted(arrays.items())
        if [v[:2] for _, v in mine] != [v[:2] for _, v in english]:
            fail("%s's cutscene pages aren't in the English order" % LANGUAGES[li][1])
        for (ptr, (c, p, n)), (eptr, (_, _, en)) in zip(mine, english):
            if n != en or uses[ptr] != langs[0][1][eptr]:
                fail("%s's cutscene %d page %d isn't shaped like the English one" % (LANGUAGES[li][1], c, p))
    for ptr, (c, p, n) in english:
        slide = rom.w(rom.w(CUTSCENES + 8 * c) + 4 * p)
        picture = pictures.get(rom.w(slide), "?")
        also = [u for u in langs[0][1][ptr] if u != (c, p)]
        for i in range(n):
            texts = []
            for arrays, uses in langs:
                aptr = [a for a, v in arrays.items() if v[:2] == (c, p)][0]
                texts.append(rom.string(rom.w(aptr + 4 * i)))
            comments = ["Cutscene %d, page %d (over graphics/%s)%s." % (
                c, p, picture, ", string %d of %d" % (i + 1, n) if n > 1 else "")]
            if also:
                comments.append("Also shown as " + ", ".join("cutscene %d's page %d" % u for u in also) + ".")
            entries.append(("cutscene%02d.page%d.%d" % (c, p, i), texts[0],
                            {code: texts[k + 1] for k, code in enumerate(TRANSLATIONS)}, comments))
    return entries


CREDITS_NOTE = ("The credits are shown in English in every language (Credits::Init reads one text, gCreditsText): "
                "msgstr is the English text, unchanged (tools/text.py checks).")

CUTSCENE_NOTE = ("The cutscene text is drawn in the small font, word-wrapped into a 228x30 px box (3 lines) under the "
                 "picture; longer text goes on after a key press.")


def write_po(path, entries, code, lang):
    lines = ((POT_HEADER if GAME == "xs" else N2_POT_HEADER).splitlines() if not code else
             ["# %s: the game's text in %s (docs/text.md)." % (GAME_TITLE, lang)]) + po_header(code, lang)
    for key, english, texts, comments in entries:
        lines.append("")
        if key.startswith("cutscene") and key.endswith(".0"):
            comments = comments + [CUTSCENE_NOTE]
        if key.startswith("credits."):
            comments = comments + [CREDITS_NOTE]
        for c in comments:
            lines += ["#. " + x for x in textwrap.wrap(c, 76)]
        lines.append("msgctxt " + po_quote(key))
        lines.append("msgid " + po_quote(english))
        lines.append("msgstr " + po_quote(texts[code] if code else ""))
    raw = path + ".raw"
    with open(raw, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(lines) + "\n")
    # gettext's own layout (line wrapping), which msgmerge keeps.
    r = run(["msgcat", "-o", path, raw])
    os.unlink(raw)
    if r.returncode:
        fail("msgcat %s:\n%s" % (path, r.stderr))


# gCreditsText (credits_text.cpp).
CREDITS_TEXT = 0x0817C5D0

def extract_credits(rom):
    """The credits' entries from the ROM's stream, keyed by data/credits.json:
    (key, English, {lang: text}, comments) in the order the items list
    them, every language's text the English. A text item's
    lines run on across empty lines that a plain line (no opcode) follows,
    except a large-font item's; the extracted structure must be the
    items'."""
    start = end = CREDITS_TEXT - ROM_BASE
    while rom.data[end] != 0:  # the end; a logo's index can be 0
        end += 2 if rom.data[end] == CREDITS_OP_PICTURE else 1
    data = rom.data[start:end]
    lines = data[:-1].split(b"\n")
    items = read_credits()
    entries, i, font = [], 0, "small"
    where = rel(CREDITS_JSON)
    for n, it in enumerate(items):
        (k, v), = it.items()
        if k == "blank":
            if lines[i:i + v] != [b""] * v:
                fail("%s: item %d: the ROM has no %d empty lines there" % (where, n, v))
            i += v
        elif k == "picture":
            if lines[i] != bytes([CREDITS_OP_PICTURE, v]):
                fail("%s: item %d: the ROM has no logo %d there" % (where, n, v))
            i += 1
        else:
            line = lines[i]
            if k != "text":
                font = k
                if line[:1] != bytes([CREDITS_OP_LARGE if k == "large" else CREDITS_OP_SMALL]):
                    fail("%s: item %d: the ROM has no %s font switch there" % (where, n, k))
                line = line[1:]
            text = [line]
            i += 1
            while i < len(lines):
                j = i
                while k != "large" and j < len(lines) and lines[j] == b"":
                    j += 1
                if j < len(lines) and lines[j] and lines[j][0] >= 0x20:
                    text += lines[i:j + 1]
                    i = j + 1
                else:
                    break
            what = "a heading" if k == "large" else "lines"
            english = b"\n".join(text).decode(ENCODING)
            entries.append((v, english, {code: english for code in TRANSLATIONS},
                            ["The credits: %s, in the %s font." % (what, font)]))
    if i != len(lines):
        fail("%s: the ROM's credits go on after the last item" % where)
    return entries


def extract_entries_n2(rom):
    """N-Tranced's template entries: its UI text, ui.level.<level> for the
    level names (its level table's name_text, levels.json's
    "level_names"), ui.text.<id> for the others."""
    with open(N2_LEVELS_JSON, encoding="utf-8") as f:
        level_names = json.load(f)["level_names"]
    level_keys = {rom.w(N2_LEVEL_TABLE + N2_LEVEL_INFO_SIZE * i): name for i, name in enumerate(level_names)}
    texts = [[rom.string(rom.w(t + 4 * i)) for i in range(UI_ROM_COUNT)] for t in UI_TABLES]
    entries = []
    for i in range(UI_ROM_COUNT):
        if i in level_keys:
            key, what = "ui.level." + level_keys[i], "A level's name."
        else:
            key, what = "ui.text.%03d" % i, "Not named yet."
        entries.append((key, to_po(texts[0][i]),
                        {code: to_po(texts[n + 1][i]) for n, code in enumerate(TRANSLATIONS)},
                        ["Text id %d (0x%02X). %s" % (i, i, what)]))
    return entries


def extract(rom_path, check_only):
    rom = Rom(rom_path)
    entries = extract_entries(rom) + extract_credits(rom) if GAME == "xs" else extract_entries_n2(rom)
    pot = os.path.basename(POT)
    out = tempfile.mkdtemp() if check_only else TEXT_DIR
    try:
        os.makedirs(out, exist_ok=True)
        write_po(os.path.join(out, pot), entries, None, None)
        for code, lang in LANGUAGES[1:]:
            write_po(os.path.join(out, code + ".po"), entries, code, lang)
        if not check_only:
            return
        bad = []
        for name in [pot] + [c + ".po" for c in TRANSLATIONS]:
            mine, theirs = os.path.join(out, name), os.path.join(TEXT_DIR, name)
            if not os.path.exists(theirs) or not filecmp.cmp(mine, theirs, shallow=False):
                bad.append(name)
                if os.path.exists(theirs):
                    with open(theirs, encoding="utf-8") as a, open(mine, encoding="utf-8") as b:
                        sys.stderr.writelines(list(difflib.unified_diff(
                            a.readlines(), b.readlines(), rel(theirs), "extracted/" + name))[:40])
        if bad:
            fail("%s/ isn't what the ROM has: %s" % (rel(TEXT_DIR), ", ".join(bad)))
        print("text: the extracted files are the same")
    finally:
        if check_only:
            shutil.rmtree(out)


# N-Tranced's (--game n2): its UI tables (196 pointers each, in the
# languages' order), its level table (name_text, the record's first word)
GAME, GAME_TITLE, PROJECT = "xs", "Crash Bandicoot XS", "crashbandicootxs"
# N-Tranced's .po files write a text's final newline as FINAL_NEWLINE (U+2424,
# not Latin-1, so no text has it): its ROM's French, German and Spanish
# texts 36 and 37 end in a newline the English doesn't, or the other way
# round, and msgfmt rejects an entry whose msgid and msgstr don't both end
# in one
FINAL_NEWLINE = "\u2424"


def to_po(text):
    """N-Tranced's text as its .po files write it (FINAL_NEWLINE)."""
    return text[:-1] + FINAL_NEWLINE if GAME == "n2" and text.endswith("\n") else text


def from_po(text):
    """The text a .po file's entry stands for (FINAL_NEWLINE)."""
    return text[:-1] + "\n" if GAME == "n2" and text.endswith(FINAL_NEWLINE) else text
N2_UI_TABLES = [0x081D3984, 0x081D5C20, 0x081D7F50, 0x081DA158, 0x081DC444, 0x081DE730]
N2_UI_COUNT = 196
N2_LEVEL_TABLE, N2_LEVEL_INFO_SIZE = 0x081D0430, 0x24
N2_LEVELS_JSON = os.path.join(ROOT, "games", "n2", "data", "levels", "levels.json")
N2_POT_HEADER = """\
# Crash Bandicoot 2: N-Tranced: the game's text (docs/n_tranced.md, "Text").
#
# msgctxt is the key, msgid the English text. The entries are the UI text,
# in GetUiText's text id order: ui.level.<level> for the level names,
# ui.text.<id> for the others until they have names. Characters are
# Latin-1, and a text's final newline is written \u2424 (U+2424), so that
# msgfmt takes the texts whose translation ends in one and whose English
# doesn't, or the other way round (tools/text.py's FINAL_NEWLINE).
"""


def use_game(game):
    """Work on `game`'s text: XS's, or N-Tranced's (n2)."""
    global GAME, GAME_TITLE, PROJECT, TEXT_DIR, POT, TABLES, UI_TABLES, UI_ROM_COUNT
    if game == "n2":
        GAME, GAME_TITLE, PROJECT = "n2", "Crash Bandicoot 2: N-Tranced", "ntranced"
        TEXT_DIR = os.path.join(ROOT, "games", "n2", "text")
        POT = os.path.join(TEXT_DIR, "ntranced.pot")
        TABLES = ["ui_text"]
        UI_TABLES, UI_ROM_COUNT = N2_UI_TABLES, N2_UI_COUNT


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--game", choices=["xs", "n2"], default="xs")
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("cpp")
    p.add_argument("table", choices=["ui_text", "cutscene_text_english", "cutscene_text_translations",
                                     "credits_text"])
    p.add_argument("out")
    p.add_argument("--mo", default=None, help="the directory of the compiled <lang>.mo files")
    sub.add_parser("check")
    p = sub.add_parser("extract")
    p.add_argument("rom", nargs="?", default=None)
    p.add_argument("--check", action="store_true")
    args = ap.parse_args()
    use_game(args.game)
    if args.cmd == "extract" and args.rom is None:
        args.rom = os.path.join(ROOT, "baserom.gba" if GAME == "xs" else "baserom_n2.gba")
    if args.cmd == "cpp" and args.table not in TABLES:
        fail("%s has no table %s" % (GAME_TITLE, args.table))
    if args.cmd == "cpp":
        if args.table not in ("cutscene_text_english", "credits_text") and not args.mo:
            fail("cpp %s needs --mo" % args.table)
        write_lines(args.out, generate(args.table, args.mo))
    elif args.cmd == "check":
        check()
    else:
        extract(args.rom, args.check)


if __name__ == "__main__":
    main()
