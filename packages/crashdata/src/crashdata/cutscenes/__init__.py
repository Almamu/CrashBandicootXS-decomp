#!/usr/bin/env python3
"""games/xs/data/cutscenes.json: the cutscenes, their slides and pages (#1092).

The cutscenes are data: games/xs/data/cutscenes.json lists them (in id order, the
index of gCutscenes and PlayCutscene's argument), each with its slides,
and the slide records (picture, timing, fades, music cue, sound effect).
A cutscene's slide names a slide record and, if text shows with it, the
page of text: `cutscene<NN>.page<P>`, whose strings are the template's
`cutscene<NN>.page<P>.<I>` entries (games/xs/text/crashxs.pot, docs/text.md). The
page's string count is the number of those entries, so a string added
to a page needs no other edit. docs/data.md, "Cutscenes", has the
format.

    tools/cutscenes.py cpp cutscenes|cutscene_slides OUT
        Writes the C++ that engine/src/game/ui/data/cutscenes.cpp (the language
        tables, gCutscenes, the English pages, the slide lists) or
        cutscene_slides.cpp (the other languages' pages, the slides)
        #includes.

    tools/cutscenes.py constants OUT
        Writes constants/cutscenes.h: a CUTSCENE_* id per cutscene and
        CUTSCENE_COUNT. Written only if it changed.

    tools/cutscenes.py check
        The checks the generators make: every slide's picture is a
        cutscene picture of games/xs/graphics/manifest.json, every page is in the
        template, every page of the template and every slide record is
        used, the names are unique.

The ROM stores the cutscenes' tables in an order of its own (the English
pages: cutscene 1 to 10, then 0; the slide lists: 0-3, 6, 4, 5, 7-10).
cutscenes.json's "rom_order" gives it; a cutscene it doesn't list comes
after the listed ones, in id order.
"""

import json
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import text  # noqa: E402

ROOT = text.ROOT
CUTSCENES_JSON = os.path.join(ROOT, "games", "xs", "data", "cutscenes.json")
MANIFEST = text.GRAPHICS_MANIFEST
LANGUAGES = [lang for _, lang in text.LANGUAGES]

# struct cutscene_slide's fields after the picture, in order, and what
# each holds (engine/include/game/ui/cutscene.hpp).
SLIDE_FIELDS = ["wait", "fade", "fade_after", "buttons", "duck_music", "rearm_sfx", "song", "sfx"]
NUMBER_FIELDS = {"wait": (-0x80000000, 0x7FFFFFFF), "fade": (-0x80000000, 0x7FFFFFFF),
                 "fade_after": (-0x80000000, 0x7FFFFFFF), "buttons": (0, 255), "duck_music": (0, 255),
                 "rearm_sfx": (0, 255)}
ROM_ORDERS = ["pages", "slide_lists"]


def fail(msg):
    sys.exit("cutscenes.py: " + msg)


def rel(path):
    return os.path.relpath(path, ROOT)


def load():
    """cutscenes.json, checked against the template and the manifest:
    (cutscenes, slides by name, slide index by name, template pages by
    key)."""
    where = rel(CUTSCENES_JSON)
    with open(CUTSCENES_JSON, encoding="utf-8") as f:
        data = json.load(f)
    for k in ("cutscenes", "slides"):
        if not isinstance(data.get(k), list):
            fail("%s: no \"%s\" list" % (where, k))
    cutscenes, slides = data["cutscenes"], data["slides"]
    if not cutscenes:
        fail("%s: no cutscenes" % where)

    with open(MANIFEST, encoding="utf-8") as f:
        pictures = {e["path"]: e for e in json.load(f)["assets"]
                    if e.get("kind") == "bitmap" and e.get("path", "").startswith("cutscenes/")}
    slide_index = {}
    for i, s in enumerate(slides):
        name = s.get("name")
        what = "%s: slide %d (%s)" % (where, i, name)
        if not isinstance(name, str) or not name:
            fail("%s: slide %d has no name" % (where, i))
        if name in slide_index:
            fail("%s: two slides are named %s" % (where, name))
        slide_index[name] = i
        if s.get("picture") not in pictures:
            fail("%s: picture %r is no cutscene picture of %s (a \"bitmap\" entry under games/xs/graphics/cutscenes/)"
                 % (what, s.get("picture"), rel(MANIFEST)))
        for k in SLIDE_FIELDS:
            if k not in s:
                fail("%s: no \"%s\"" % (what, k))
            if k in NUMBER_FIELDS:
                lo, hi = NUMBER_FIELDS[k]
                if not isinstance(s[k], int) or isinstance(s[k], bool) or not lo <= s[k] <= hi:
                    fail("%s: \"%s\" is %r, not a number from %d to %d" % (what, k, s[k], lo, hi))
            elif not isinstance(s[k], str) or not s[k].isidentifier():
                fail("%s: \"%s\" is %r, not a constant's name" % (what, k, s[k]))
        extra = set(s) - set(SLIDE_FIELDS) - {"name", "picture"}
        if extra:
            fail("%s: unknown fields %s" % (what, ", ".join(sorted(extra))))

    _, template = text.read_template()
    pages = {"cutscene%02d.page%d" % (p["cutscene"], p["page"]): p for p in template}
    constants = set()
    used_slides, used_pages = set(), set()
    for ci, c in enumerate(cutscenes):
        const = c.get("constant")
        if not isinstance(const, str) or not const.startswith("CUTSCENE_") or not const.isidentifier():
            fail("%s: cutscene %d's constant %r isn't CUTSCENE_<NAME>" % (where, ci, const))
        if const in constants or const == "CUTSCENE_COUNT":
            fail("%s: %s twice" % (where, const))
        constants.add(const)
        if not c.get("slides"):
            fail("%s: %s has no slides" % (where, const))
        for si, s in enumerate(c["slides"]):
            what = "%s: %s's slide %d" % (where, const, si)
            if s.get("slide") not in slide_index:
                fail("%s: no slide named %r in \"slides\"" % (what, s.get("slide")))
            used_slides.add(s["slide"])
            key = s.get("text")
            if key is not None:
                if key not in pages:
                    fail("%s: page %s isn't in %s (its strings are the entries %s.0, %s.1, ...)"
                         % (what, key, rel(text.POT), key, key))
                used_pages.add(key)
            extra = set(s) - {"slide", "text"}
            if extra:
                fail("%s: unknown fields %s" % (what, ", ".join(sorted(extra))))
    for s in slides:
        if s["name"] not in used_slides:
            fail("%s: slide %s is in no cutscene" % (where, s["name"]))
    for key in pages:
        if key not in used_pages:
            fail("%s: page %s (the entries %s.*) is no cutscene slide's \"text\"" % (rel(text.POT), key, key))

    order = data.get("rom_order", {})
    extra = set(order) - set(ROM_ORDERS)
    if extra:
        fail("%s: unknown \"rom_order\" lists %s" % (where, ", ".join(sorted(extra))))
    ids = [c["constant"] for c in cutscenes]
    orders = {}
    for k in ROM_ORDERS:
        listed = order.get(k, [])
        for name in listed:
            if name not in constants:
                fail("%s: rom_order %s: no cutscene %s" % (where, k, name))
        if len(set(listed)) != len(listed):
            fail("%s: rom_order %s lists a cutscene twice" % (where, k))
        orders[k] = [ids.index(n) for n in listed] + [i for i, n in enumerate(ids) if n not in listed]
    return cutscenes, slides, slide_index, pages, orders, pictures


def page_symbol(page, lang):
    return text.page_symbol(page, lang)


def page_rows(cutscene, pages, lang):
    rows = []
    for s in cutscene["slides"]:
        key = s.get("text")
        if key is None:
            rows.append("    { 0, 0 },")
        else:
            sym = page_symbol(pages[key], lang)
            rows.append("    { %s, ARRAY_COUNT(%s) }," % (sym, sym))
    return rows


def pages_symbol(ci, lang):
    return "gCutscene%02d%s" % (ci, lang)


def language_pages(cutscenes, pages, order, lang):
    """A language's `struct cutscene_page` arrays, in the ROM's order."""
    out = []
    for ci in order:
        c = cutscenes[ci]
        out.append("const struct cutscene_page %s[%d] = {" % (pages_symbol(ci, lang), len(c["slides"])))
        out += page_rows(c, pages, lang)
        out.append("};")
    return out


def page_externs(cutscenes, pages, order, lang):
    """The page string arrays a language's pages point at, each once, in
    the order they are first used."""
    out, seen = [], set()
    for ci in order:
        for s in cutscenes[ci]["slides"]:
            key = s.get("text")
            if key is not None and key not in seen:
                seen.add(key)
                out.append("extern const u8 *const %s[%d];" % (page_symbol(pages[key], lang),
                                                              len(pages[key]["entries"])))
    return out


def slide_symbol(i):
    return "gCutsceneSlide%02d" % i


def slide_externs(slides):
    return ["extern const struct cutscene_slide %s;" % slide_symbol(i) for i in range(len(slides))]


def pages_externs(cutscenes, order, langs):
    return ["extern const struct cutscene_page %s[%d];" % (pages_symbol(ci, lang), len(cutscenes[ci]["slides"]))
            for lang in langs for ci in order]


HEADER = [
    "/* Generated by tools/cutscenes.py from games/xs/data/cutscenes.json and",
    " * games/xs/text/crashxs.pot (docs/data.md, \"Cutscenes\"): don't edit. */",
    "",
]


def language_table(cutscenes, lang):
    out = ["const struct cutscene_page *const gCutsceneText%s[CUTSCENE_COUNT] = {" % lang]
    out += ["    %s," % pages_symbol(ci, lang) for ci in range(len(cutscenes))]
    out.append("};")
    return out


def cutscenes_cpp(cutscenes, slides, slide_index, pages, orders):
    """cutscenes.cpp's tables: the English language table, gCutscenes, the
    English pages, the other languages' tables, the slide lists."""
    out = list(HEADER)
    out += pages_externs(cutscenes, orders["pages"], LANGUAGES)
    out += ["extern const struct cutscene_slide *const gCutscene%02dSlides[%d];" % (ci, len(cutscenes[ci]["slides"]))
            for ci in orders["slide_lists"]]
    out += page_externs(cutscenes, pages, orders["pages"], LANGUAGES[0])
    out += slide_externs(slides)
    out += [""] + language_table(cutscenes, LANGUAGES[0]) + [""]
    out.append("const struct cutscene_slides gCutscenes[CUTSCENE_COUNT] = {")
    out += ["    { gCutscene%02dSlides, ARRAY_COUNT(gCutscene%02dSlides) }, // %s" % (ci, ci, c["constant"])
            for ci, c in enumerate(cutscenes)]
    out += ["};", ""]
    out += language_pages(cutscenes, pages, orders["pages"], LANGUAGES[0]) + [""]
    for lang in LANGUAGES[1:]:
        out += language_table(cutscenes, lang) + [""]
    for ci in orders["slide_lists"]:
        c = cutscenes[ci]
        out.append("const struct cutscene_slide *const gCutscene%02dSlides[%d] = {" % (ci, len(c["slides"])))
        out += ["    &%s, // %s" % (slide_symbol(slide_index[s["slide"]]), s["slide"]) for s in c["slides"]]
        out.append("};")
    return out


def slide_record(i, s, pictures):
    cls = pictures[s["picture"]]["class"]
    vals = ["Engine::Gfx::%s::palette" % cls] + ["%d" % s[k] if k in NUMBER_FIELDS else s[k] for k in SLIDE_FIELDS]
    return ["const struct cutscene_slide %s = { // %s" % (slide_symbol(i), s["name"]),
            "    " + ", ".join(vals) + ",", "};"]


def cutscene_slides_cpp(cutscenes, slides, slide_index, pages, orders, pictures):
    """cutscene_slides.cpp's: the French to Dutch pages, then the slides."""
    out = list(HEADER)
    out += pages_externs(cutscenes, orders["pages"], LANGUAGES[1:])
    out += slide_externs(slides)
    for lang in LANGUAGES[1:]:
        out += page_externs(cutscenes, pages, orders["pages"], lang)
    out.append("")
    for lang in LANGUAGES[1:]:
        out += language_pages(cutscenes, pages, orders["pages"], lang)
    out.append("")
    for i, s in enumerate(slides):
        out += slide_record(i, s, pictures)
    return out


def constants_h(cutscenes):
    rows = [(c["constant"], "%d" % i, c.get("about")) for i, c in enumerate(cutscenes)]
    rows.append(("CUTSCENE_COUNT", "%d" % len(cutscenes), "the length of gCutscenes"))
    width = max(len(n) + len(v) for n, v, _ in rows)
    body = []
    for n, v, c in rows:
        line = "#define %s %s" % (n, v)
        if c:
            line += " " * (width - len(n) - len(v) + 1) + "// " + c
        body.append(line)
    guard = "GUARD_CONSTANTS_CUTSCENES_H"
    doc = """/*
 * Cutscene ids: the index of gCutscenes and of each language's pages
 * (gCutsceneTexts), PlayCutscene's argument. games/xs/data/cutscenes.json's
 * "cutscenes", in order (docs/data.md, "Cutscenes").
 */"""
    return "\n".join(["/* Generated by tools/cutscenes.py from games/xs/data/cutscenes.json. Do not edit. */",
                      "#ifndef " + guard, "#define " + guard, "", doc, ""] + body
                     + ["", "#endif /* %s */" % guard, ""])


def write_if_changed(path, data):
    if os.path.exists(path):
        with open(path, encoding="ascii") as f:
            if f.read() == data:
                return
    os.makedirs(os.path.dirname(os.path.abspath(path)), exist_ok=True)
    with open(path, "w", encoding="ascii", newline="\n") as f:
        f.write(data)


def main():
    args = sys.argv[1:]
    if not args or args[0] in ("-h", "--help"):
        print(__doc__)
        return
    cutscenes, slides, slide_index, pages, orders, pictures = load()
    if args[0] == "cpp" and len(args) == 3 and args[1] in ("cutscenes", "cutscene_slides"):
        if args[1] == "cutscenes":
            lines = cutscenes_cpp(cutscenes, slides, slide_index, pages, orders)
        else:
            lines = cutscene_slides_cpp(cutscenes, slides, slide_index, pages, orders, pictures)
        text.write_lines(args[2], lines)
    elif args[0] == "constants" and len(args) == 2:
        write_if_changed(args[1], constants_h(cutscenes))
    elif args[0] == "check" and len(args) == 1:
        print("cutscenes: OK")
    else:
        fail("usage: see tools/cutscenes.py --help")


if __name__ == "__main__":
    main()
