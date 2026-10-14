#!/usr/bin/env python3
"""text/: the game's text as gettext files (docs/text.md, #1070).

text/crashxs.pot is the template: one entry per string, its key in
`msgctxt`, the English text as `msgid` and translator comments (`#.`).
text/<lang>.po has a language's text (fr, de, es, it, nl). The build
compiles each .po with msgfmt and generates the C++ of the three string
tables from the template and the .mo files.

    tools/text.py cpp TABLE OUT [--mo DIR]
        Writes the C++ definitions of TABLE (ui_text,
        cutscene_text_english or cutscene_text_translations), which
        src/.../data/TABLE.cpp #includes, reading each language's text from
        DIR/<lang>.mo with the stdlib gettext module (pgettext).

    tools/text.py check
        The translations' checks: `msgfmt --check --statistics` for each
        .po (fails on an untranslated or fuzzy entry), `msgcmp` both ways
        against the template (fails on a missing or extra key), and the
        generator's own checks (every character in the game's font).

    tools/text.py extract [baserom.gba] [--check]
        Writes text/crashxs.pot and the .po files from the ROM's tables,
        through msgcat so they are in gettext's own layout. With --check,
        writes them into a temporary directory and compares them with
        text/ byte for byte.

The tables (docs/data.md, "Text tables"):

- ui_text: per language (English, French, German, Spanish,
  Italian, Dutch) the strings it adds, then its `gUiText<Lang>` array of
  UI_TEXT_COUNT pointers, which GetUiText indexes by text id. The
  template's `ui.*` entries are in text id order.
- cutscene_text_english: the English cutscene pages, each page's strings
  then its `gCutscene<NN>EnglishPage<P>` array; cutscene_text_translations the
  other five languages' pages the same way. The template's
  `cutscene<NN>.page<P>.<I>` entries are in ROM order.

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

# gLanguage's order: the languages of gUiTextTables and gCutsceneTexts.
LANGUAGES = [("en", "English"), ("fr", "French"), ("de", "German"), ("es", "Spanish"), ("it", "Italian"),
             ("nl", "Dutch")]
TRANSLATIONS = [code for code, _ in LANGUAGES[1:]]

TABLES = ["ui_text", "cutscene_text_english", "cutscene_text_translations"]

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


def fail(msg):
    sys.exit("text.py: " + msg)


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
    missing = small.missing(data)
    if missing:
        fail("%s: the font has no %s (the font's characters: lower case letters, digits, %s)" % (
            where, show_bytes(missing), "the buttons A, B, L, R and the punctuation of gSmallFontChars"))
    if need_large:
        missing = large.missing(data)
        if missing:
            fail("%s: the large font, which the game also draws this in, has no %s" % (
                where, show_bytes(missing)))
    return data


# --- The template and the translations ------------------------------------

def read_template():
    """The template's entries: the UI ones (in text id order) and the
    cutscene pages (in ROM order), each page a list of its entries."""
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
            fail("%s: %s is neither ui.* nor cutsceneNN.pageP.I" % (POT, key))
    return ui, pages


def check_level_names(ui):
    """Each level's name_text (levels.json) must be a ui.level.* entry:
    the UI entries' order is their text ids, so an entry inserted in the
    middle would shift every later id."""
    with open(LEVELS_JSON, encoding="utf-8") as f:
        levels = json.load(f)["levels"]
    for lv in levels:
        t = lv["name_text"]
        if t >= len(ui) or not ui[t]["ctx"].startswith("ui.level."):
            fail("%s: %s's name_text %d is %s, not a ui.level.* entry (the ui.* entries are in text id order)"
                 % (POT, lv["constant"], t, ui[t]["ctx"] if t < len(ui) else "past the last one"))


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


HEADER = [
    "/* Generated by tools/text.py from text/crashxs.pot and text/*.po",
    " * (docs/text.md): don't edit. */",
    "",
]


def ui_table(ui, tr, small, large):
    out = list(HEADER)
    strings = Strings()
    for code, lang in LANGUAGES:
        lines, symbols = [], []
        for i, e in enumerate(ui):
            text = e["id"] if code == "en" else translate(tr[code], code, e)
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
        return list(HEADER) + english, None
    others = []
    for code, lang in LANGUAGES[1:]:
        others += cutscene_language(pages, code, lang, tr, strings, small, large)
    used = sorted(set(re.findall(r"&(gCutscene\d\dEnglish\w+)", "\n".join(others))) & defined,
                  key=lambda s: list(strings.symbol.values()).index(s))
    externs = ["extern const u8 %s[];" % s for s in used]
    return list(HEADER) + english, list(HEADER) + externs + ([""] if externs else []) + others


def generate(table, mo_dir):
    small, large = fonts()
    ui, pages = read_template()
    if table == "ui_text":
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
# cutscene<NN>.page<P>.<I>, the strings of a page in the order they show.
# The text is lower case: the fonts have no capital letters, and "A", "B",
# "L" and "R" draw the GBA's buttons. Characters are Latin-1 and must be in
# the font (tools/text.py checks).
"""

DATE = "2026-10-14 00:00+0000"


def po_header(code, lang):
    fields = [
        "Project-Id-Version: crashbandicootxs",
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
            key = "ui.level." + lv["constant"][len("LEVEL_"):].lower()
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


CUTSCENE_NOTE = ("The cutscene text is drawn in the small font, word-wrapped into a 228x30 px box (3 lines) under the "
                 "picture; longer text goes on after a key press.")


def write_po(path, entries, code, lang):
    lines = (POT_HEADER.splitlines() if not code else
             ["# Crash Bandicoot XS: the game's text in %s (docs/text.md)." % lang]) + po_header(code, lang)
    for key, english, texts, comments in entries:
        lines.append("")
        if key.startswith("cutscene") and key.endswith(".0"):
            comments = comments + [CUTSCENE_NOTE]
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


def extract(rom_path, check_only):
    entries = extract_entries(Rom(rom_path))
    out = tempfile.mkdtemp() if check_only else TEXT_DIR
    try:
        os.makedirs(out, exist_ok=True)
        write_po(os.path.join(out, "crashxs.pot"), entries, None, None)
        for code, lang in LANGUAGES[1:]:
            write_po(os.path.join(out, code + ".po"), entries, code, lang)
        if not check_only:
            return
        bad = []
        for name in ["crashxs.pot"] + [c + ".po" for c in TRANSLATIONS]:
            mine, theirs = os.path.join(out, name), os.path.join(TEXT_DIR, name)
            if not os.path.exists(theirs) or not filecmp.cmp(mine, theirs, shallow=False):
                bad.append(name)
                if os.path.exists(theirs):
                    with open(theirs, encoding="utf-8") as a, open(mine, encoding="utf-8") as b:
                        sys.stderr.writelines(list(difflib.unified_diff(
                            a.readlines(), b.readlines(), "text/" + name, "extracted/" + name))[:40])
        if bad:
            fail("text/ isn't what the ROM has: " + ", ".join(bad))
        print("text: the extracted files are the same")
    finally:
        if check_only:
            shutil.rmtree(out)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("cpp")
    p.add_argument("table", choices=TABLES)
    p.add_argument("out")
    p.add_argument("--mo", default=None, help="the directory of the compiled <lang>.mo files")
    sub.add_parser("check")
    p = sub.add_parser("extract")
    p.add_argument("rom", nargs="?", default=os.path.join(ROOT, "baserom.gba"))
    p.add_argument("--check", action="store_true")
    args = ap.parse_args()
    if args.cmd == "cpp":
        if args.table != "cutscene_text_english" and not args.mo:
            fail("cpp %s needs --mo" % args.table)
        write_lines(args.out, generate(args.table, args.mo))
    elif args.cmd == "check":
        check()
    else:
        extract(args.rom, args.check)


if __name__ == "__main__":
    main()
