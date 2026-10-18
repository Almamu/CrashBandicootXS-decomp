"""The cutscenes as the cutscene editor sees them, and its edits written back (#1167, docs/editors.md "The cutscene editor").

The cutscene editor (apps/editor) shows a cutscene's slides the way the
game does and edits them; this module is what the backend
(buildtools.editor_server) gives it and how it writes the edits. It works
on a crashdata.common.game.Game, any tree's (XS's, a mod's), reading every
file by its path in that tree:

- view(game): cutscenes.json's value, the cutscene pictures of the
  graphics manifest, every page's strings in the six languages (the text
  template and the .po files, docs/text.md), the songs and sound effects
  a slide can name, the small font (its characters, widths, glyph
  records and glyph pixels: what PlayCutscene draws the text with), the
  text box, and what names each cutscene (code, levels.json);
- picture(game, path): a cutscene picture's palette and pixels;
- import_picture(png, palette): a PNG made into a cutscene picture
  (240x160, 256 colours), on its own palette or mapped to one;
- plan_save(game, body): the files a save changes, after its checks:
  cutscenes.json's new value; the text's files (crashdata.text.po: only
  the entries that change are written again, as msgcat writes them); the
  pictures (a picture written only if its pixels or its palette change:
  a new palette alone replaces its PLTE, crashdata.levels.room_edit's
  rule); a new picture's PNG, its manifest entry and its two lines of
  cxx_symbols.txt's graphics classes block.

N-Tranced's cutscenes aren't data yet (its games/n2/data has no
cutscenes.json): available(game) says so.
"""
import base64
import io
import json
import re
from pathlib import Path

from crashdata import text as T
from crashdata.common import diagnostics as D
from crashdata.common import jasc_pal
from crashdata.cutscenes import NUMBER_FIELDS
from crashdata.levels import room_edit
from crashdata.text import po

# The screen and the text box: PlayCutscene's (engine/src/game/level/level_cutscene.cpp:
# DISPLAY_ART_X + 7, DISPLAY_ART_Y + 0x7E, 0xE4 x 0x1E), in the small font
# (gSmallFont: 9-pixel lines, 4-pixel spaces; HeightToLines(30) = 3 lines).
SCREEN = (240, 160)
BOX = {"x": 7, "y": 126, "w": 228, "h": 30}
FONT_LINE_HEIGHT = 9
FONT_SPACE = 4
# the OBJ sizes of attribute 0's shape, at size 0 (the small font's)
SHAPE_SIZES = {0: (8, 8), 1: (16, 8), 2: (8, 16)}
GLYPH_CELL = 16  # fonts/small.png: one 16x16 cell per glyph (its obj_sheet's --cell)
PICTURE_DIR = "cutscenes/"
PICTURE_OBJECT = "cutscene_pictures"
SYMBOLS_END = "# end of graphics classes"
MAX_CUTSCENES = 100  # the text keys' two digits (cutsceneNN.pageP.I)


class EditError(Exception):
    def __init__(self, message, diagnostics=None):
        super().__init__(message)
        self.diagnostics = diagnostics


class Files:
    """Where a game's cutscene data is in its tree."""

    def __init__(self, game):
        self.game = game
        self.root = Path(game.root)
        self.cutscenes = game.data_dir / "cutscenes.json"
        self.manifest = game.graphics_dir / "manifest.json"
        self.pot = game.text_dir / "crashxs.pot"
        self.fonts_src = self.root / "engine" / "src" / "game" / "ui" / "data" / "hud_font_tables.cpp"
        self.font_png = game.graphics_dir / "fonts" / "small.png"
        self.gax = game.sound_dir / "gax_manifest.json"
        self.sfx = game.sound_dir / "sfx_table.json"
        self.levels = game.levels_dir / "levels.json"
        self.symbols = self.root / "cxx_symbols.txt"

    def po(self, code):
        return self.game.text_dir / ("%s.po" % code)

    def text_files(self):
        """(language code or None for the template, path), the template first."""
        return [(None, self.pot)] + [(code, self.po(code)) for code in T.TRANSLATIONS]

    def rel(self, p):
        return Path(p).resolve().relative_to(self.root.resolve()).as_posix()


def available(game):
    return game.is_xs and Files(game).cutscenes.is_file()


def _diag(message, file=None, pointer=None, code="cutscenes"):
    return {"file": file, "pointer": pointer, "line": None, "col": None, "severity": D.ERROR, "code": code,
            "message": message}


# ---------------------------------------------------------------------------
# Reading

def pictures(files):
    """The manifest's cutscene pictures: [{path, symbol, class, palette, about, rom}]."""
    out = []
    for e in D.read_json(files.manifest)["assets"]:
        if e.get("kind") == "bitmap" and e.get("path", "").startswith(PICTURE_DIR):
            out.append({"path": e["path"], "symbol": e.get("symbol"), "class": e.get("class"),
                        "palette": e.get("palette", {}).get("symbol"), "about": e.get("about"),
                        "rom": "rom" in e})
    return out


def read_pages(files):
    """Every cutscene page of the text: {page key: [{"key", "en", "fr", ...}]},
    the strings in order, and the pages' order in the template."""
    entries = T.read_po(str(files.pot))
    tr = {code: {e["ctx"]: e["str"] for e in T.read_po(str(p))} for code, p in files.text_files() if code}
    pages, order = {}, []
    for e in entries:
        m = T.KEY_CUTSCENE.match(e["ctx"] or "")
        if not m:
            continue
        page = e["ctx"].rsplit(".", 1)[0]
        if page not in pages:
            pages[page] = []
            order.append(page)
        s = {"key": e["ctx"], "en": e["id"]}
        for code in T.TRANSLATIONS:
            s[code] = tr[code].get(e["ctx"], "")
        pages[page].append(s)
    return pages, order


def songs(files):
    """The songs a slide's "song" names: SONG_<KEY>, in song id order (constants/songs.h)."""
    m = D.read_json(files.gax)
    return [{"name": "SONG_" + t["song"].upper(), "comment": t.get("comment", "")} for t in m["song_table"]]


def sound_effects(files):
    """The sound effects a slide's "sfx" names: SFX_<NAME> (the table's named
    entries, constants/sfx.h), then SFX_NONE."""
    t = D.read_json(files.sfx)
    out = [{"name": "SFX_" + e["name"], "comment": e.get("comment", "")} for e in t["entries"] if "name" in e]
    out.append({"name": "SFX_NONE", "comment": t.get("none_comment", "no sound effect")})
    return out


def _glyph_records(src, name):
    m = re.search(re.escape(name) + r"\[\d+\]\s*=\s*\{(.*?)\n\};", src, re.S)
    if not m:
        raise EditError("%s: no %s" % (name, name))
    return [(int(w), int(s), int(y)) for w, s, y in re.findall(r"^\s*\{\s*(\d+),\s*(\d+),\s*(\d+),", m.group(1), re.M)]


def font(files):
    """The small font as PlayCutscene draws it: its characters (Latin-1
    codes, glyph = the first index of the character), each glyph's advance,
    OBJ size and Y offset, its pixels (the 16x16 cell of fonts/small.png, a
    byte a pixel) and its 16 colours."""
    from PIL import Image

    src = files.fonts_src.read_text(encoding="latin-1")
    chars = T.read_c_string(src, "gSmallFontChars")
    records = _glyph_records(src, "gSmallFontGlyphs")
    img = Image.open(files.font_png)
    img.load()
    w, _ = img.size
    raw = img.tobytes()
    cols = w // GLYPH_CELL
    glyphs = bytearray()
    for g in range(len(records)):
        cx, cy = (g % cols) * GLYPH_CELL, (g // cols) * GLYPH_CELL
        for y in range(GLYPH_CELL):
            o = (cy + y) * w + cx
            glyphs += raw[o:o + GLYPH_CELL]
    rgb = img.getpalette()[:48]
    return {
        "chars": list(chars),
        "widths": [r[0] for r in records],
        "shapes": [r[1] for r in records],
        "yOffsets": [r[2] for r in records],
        "space": FONT_SPACE,
        "lineHeight": FONT_LINE_HEIGHT,
        "cell": GLYPH_CELL,
        "glyphs": base64.b64encode(bytes(glyphs)).decode("ascii"),
        "palette": [jasc_pal.from_rgb(rgb[i:i + 3]) for i in range(0, len(rgb), 3)],
    }


def references(files, cutscenes):
    """What plays each cutscene: [{constant: [where, ...]}]: code that names
    its constant, levels.json's "on_clear" that names its id."""
    out = {c["constant"]: [] for c in cutscenes}
    ids = {i: c["constant"] for i, c in enumerate(cutscenes)}
    code = files.root / "engine" / "src"
    generated = code / "game" / "ui" / "data"
    for p in sorted(code.rglob("*.cpp")) if code.is_dir() else ():
        if p.is_relative_to(generated):
            continue
        s = p.read_text(encoding="latin-1")
        for const in out:
            if re.search(r"\b%s\b" % const, s):
                out[const].append(files.rel(p))
        for m in re.finditer(r"PlayCutscene\((\d+)\)", s):
            if int(m.group(1)) in ids:
                out[ids[int(m.group(1))]].append("%s (PlayCutscene(%s))" % (files.rel(p), m.group(1)))

    def walk(v, where):
        if isinstance(v, dict):
            for k, x in v.items():
                # by its id or its constant
                name = ids.get(x) if isinstance(x, int) else x if isinstance(x, str) else None
                if k == "cutscene" and name in out:
                    out[name].append("levels.json: %s" % where)
                walk(x, where)
        elif isinstance(v, list):
            for x in v:
                walk(x, where)
    if files.levels.is_file():
        for lv in D.read_json(files.levels).get("levels", []):
            walk(lv.get("on_clear"), "%s's on_clear" % lv.get("constant"))
    return out


def view(game):
    files = Files(game)
    data = D.read_json(files.cutscenes)
    pages, order = read_pages(files)
    return {
        "cutscenes": data,
        "pictures": pictures(files),
        "pages": pages,
        "pageOrder": order,
        "languages": [{"code": c, "name": n} for c, n in T.LANGUAGES],
        "songs": songs(files),
        "sfx": sound_effects(files),
        "font": font(files),
        "box": dict(BOX),
        "screen": list(SCREEN),
        "encoding": T.ENCODING,
        "references": references(files, data.get("cutscenes", [])),
        "numberFields": {k: list(v) for k, v in NUMBER_FIELDS.items()},
    }


def _open_picture(data, where):
    from PIL import Image

    try:
        img = Image.open(io.BytesIO(data))
        img.load()
    except Exception as e:  # PIL's errors are many
        raise EditError("%s: not a picture PIL can read (%s)" % (where, e))
    return img


def picture(game, path):
    """A cutscene picture's (width, height, 256 BGR555 colours, a byte a pixel)."""
    files = Files(game)
    if not path.startswith(PICTURE_DIR) or path not in {p["path"] for p in pictures(files)}:
        raise EditError("%s isn't a cutscene picture of the graphics manifest" % path)
    p = game.graphics_dir / path
    img = _open_picture(p.read_bytes(), path)
    if img.mode != "P":
        raise EditError("%s: an image of mode %s, not an 8bpp indexed PNG" % (path, img.mode))
    rgb = (img.getpalette() or []) + [0] * 768
    pal = [jasc_pal.from_rgb(rgb[i:i + 3]) for i in range(0, 768, 3)]
    return img.size[0], img.size[1], pal, img.tobytes()


# ---------------------------------------------------------------------------
# Importing a picture

def import_picture(png, palette=None):
    """A PNG as a cutscene picture: {width, height, palette (256 BGR555),
    pixels (base64, a byte a pixel), colours, exact, how}. It must be
    240x160. With `palette` (256 entries: the slide's picture's), each pixel
    is the nearest of its colours; otherwise an indexed PNG of up to 256
    colours keeps its palette and indices, and any other is made into 256
    colours of the GBA's 15 bits (exactly, if it has no more; else median
    cut, without dithering)."""
    from PIL import Image

    img = _open_picture(png, "the picture")
    if img.size != SCREEN:
        raise EditError("the picture is %dx%d pixels; a cutscene picture is %dx%d (Mode 4's screen): crop or scale it "
                        "to that first" % (img.size + SCREEN))
    if palette is not None:
        rgb_pal = room_edit.palette_rgb(palette)
        pimg = Image.new("P", (1, 1))
        pimg.putpalette(rgb_pal)
        out = _flat_rgb(img).quantize(palette=pimg, dither=Image.Dither.NONE)
        used = len(set(out.tobytes()))
        return _picture_answer(list(palette), out.tobytes(), used, False,
                               "mapped to the picture's palette (%d of its colours used)" % used)
    if img.mode == "P" and "transparency" not in img.info:
        rgb = img.getpalette() or []
        n = len(rgb) // 3
        if n <= 256:
            pal = [jasc_pal.from_rgb(rgb[i:i + 3]) for i in range(0, len(rgb), 3)] + [0] * (256 - n)
            return _picture_answer(pal, img.tobytes(), len(set(img.tobytes())), True,
                                   "an indexed picture: its own palette and pixels")
    flat = _flat_rgb(img)
    # the GBA's colours: 5 bits a channel
    five = flat.point(lambda c: jasc_pal.standard(c >> 3))
    colours = five.getcolors(maxcolors=256)
    if colours is not None:
        colours.sort(key=lambda x: (-x[0], x[1]))
        order = {c: i for i, (_, c) in enumerate(colours)}
        raw = five.tobytes()
        data = bytes(order[tuple(raw[i:i + 3])] for i in range(0, len(raw), 3))
        pal = [jasc_pal.from_rgb(c) for _, c in colours] + [0] * (256 - len(colours))
        return _picture_answer(pal, data, len(colours), True, "%d colours, each kept" % len(colours))
    q = five.quantize(colors=256, method=Image.Quantize.MEDIANCUT, dither=Image.Dither.NONE)
    rgb = (q.getpalette() or []) + [0] * 768
    pal = [jasc_pal.from_rgb(rgb[i:i + 3]) for i in range(0, 768, 3)]
    used = len(set(q.tobytes()))
    return _picture_answer(pal, q.tobytes(), used, False, "more than 256 colours: reduced to %d (median cut)" % used)


def _flat_rgb(img):
    from PIL import Image

    if img.mode in ("RGBA", "LA") or (img.mode == "P" and "transparency" in img.info):
        rgba = img.convert("RGBA")
        bg = Image.new("RGBA", rgba.size, (0, 0, 0, 255))
        return Image.alpha_composite(bg, rgba).convert("RGB")
    return img.convert("RGB")


def _picture_answer(pal, data, used, exact, how):
    return {"width": SCREEN[0], "height": SCREEN[1], "palette": pal,
            "pixels": base64.b64encode(data).decode("ascii"), "colours": used, "exact": exact, "how": how}


def picture_png(palette, pixels):
    """A cutscene picture's PNG: 240x160, 8bpp indexed, its 256 colours as
    jasc_pal writes them (room_edit's write_layer_png settings)."""
    from PIL import Image

    out = Image.frombytes("P", SCREEN, bytes(pixels))
    out.putpalette(room_edit.palette_rgb(palette))
    buf = io.BytesIO()
    out.save(buf, format="PNG", optimize=True)
    return buf.getvalue()


def _picture_bytes(old, palette, pixels, where):
    """A picture's new bytes: its own when nothing changes, its PLTE
    replaced when only the palette does, else written whole."""
    if not isinstance(palette, list) or len(palette) != 256 or not all(
            isinstance(v, int) and 0 <= v <= 0xFFFF for v in palette):
        raise EditError("%s: a palette is 256 entries of 0-0xFFFF (BGR555, bit 15 kept)" % where)
    if len(pixels) != SCREEN[0] * SCREEN[1]:
        raise EditError("%s: %d pixels, a cutscene picture has %d (240x160)" % (where, len(pixels),
                                                                                  SCREEN[0] * SCREEN[1]))
    if old is not None:
        img = _open_picture(old, where)
        rgb = (img.getpalette() or []) + [0] * 768
        old_pal = [jasc_pal.from_rgb(rgb[i:i + 3]) for i in range(0, 768, 3)]
        if img.mode == "P" and img.size == SCREEN and img.tobytes() == bytes(pixels):
            return old if old_pal == palette else room_edit.with_palette(old, palette)
    return picture_png(palette, pixels)


# ---------------------------------------------------------------------------
# Saving

def _b64(v, where):
    if not isinstance(v, str):
        raise EditError("%s: the pixels are base64" % where)
    try:
        return base64.b64decode(v, validate=True)
    except ValueError:
        raise EditError("%s: the pixels aren't base64" % where)


def text_problems(files, pages):
    """What's wrong with pages' strings: [diagnostic dict]. Each string is
    in every language, not empty, in the game's encoding, and every
    character in the small font (tools/text.py's checks)."""
    src = files.fonts_src.read_text(encoding="latin-1")
    small = T.Font(src, "gSmallFontChars", "gSmallFontGlyphs", FONT_SPACE)
    out = []
    for page, strings in pages.items():
        if strings is None:
            continue
        if not T.KEY_CUTSCENE.match(page + ".0"):
            out.append(_diag("%r isn't a page key (cutsceneNN.pageP)" % page, "text", "/" + page, "text"))
            continue
        if not isinstance(strings, list) or not strings:
            out.append(_diag("%s: a page has one string or more" % page, "text", "/" + page, "text"))
            continue
        for i, s in enumerate(strings):
            for code, name in T.LANGUAGES:
                where = "/%s/%d/%s" % (page, i, code)
                v = s.get(code) if isinstance(s, dict) else None
                if not isinstance(v, str) or not v:
                    out.append(_diag("%s.%d has no %s text (where the game keeps the English, it is the English)"
                                     % (page, i, name), "text", where, "text"))
                    continue
                try:
                    data = v.encode(T.ENCODING)
                except UnicodeEncodeError as e:
                    out.append(_diag("%s.%d (%s): %r can't be written in the game's encoding (Latin-1)"
                                     % (page, i, name, e.object[e.start:e.end]), "text", where, "text"))
                    continue
                missing = small.missing(data)
                if missing:
                    out.append(_diag("%s.%d (%s): the small font has no %s" % (page, i, name, T.show_bytes(missing)),
                                     "text", where, "text"))
    return out


def page_comments(page, i, n, picture):
    c = int(page[len("cutscene"):len("cutscene") + 2])
    p = int(page.split(".page")[1])
    out = ["Cutscene %d, page %d (over games/xs/graphics/%s)%s." % (
        c, p, picture or "?", ", string %d of %d" % (i + 1, n) if n > 1 else "")]
    if i == 0:
        out.append(T.CUTSCENE_NOTE)
    return out


def text_edits(files, pages, page_pictures):
    """The text files' new contents for `pages` ({page: [strings] or None to
    delete it}): {path: its new text}, only the files that change.
    A page's existing strings keep their blocks unless their text changes; a
    page whose string count changes gets its comments again; a new page goes
    after the template's last cutscene entry."""
    docs = {code: po.PoFile.read(p) for code, p in files.text_files()}
    old_pages, _ = read_pages(files)
    for page, strings in pages.items():
        old = old_pages.get(page, [])
        if strings is None:
            for s in old:
                for d in docs.values():
                    d.remove(s["key"])
            continue
        n = len(strings)
        recomment = n != len(old)
        for i, s in enumerate(strings):
            key = "%s.%d" % (page, i)
            comments = page_comments(page, i, n, page_pictures.get(page)) if recomment else None
            if i < len(old):
                for code, d in docs.items():
                    d.set(key, msgid=s["en"], msgstr="" if code is None else s[code], comments=comments)
            else:
                after = "%s.%d" % (page, i - 1) if i else "cutscene*"
                for code, d in docs.items():
                    d.insert_after(after, key, s["en"], "" if code is None else s[code],
                                   page_comments(page, i, n, page_pictures.get(page)))
        for i in range(n, len(old)):
            for d in docs.values():
                d.remove(old[i]["key"])
    out = {}
    for code, p in files.text_files():
        new = docs[code].text()
        if new != docs[code].original:
            out[p] = new
    return out


def plan_save(game, body):
    """A save's checks, then what it writes: {"json": [(rel, value)],
    "bytes": [(rel, bytes)]} (rel from the tree's root). body: {"cutscenes":
    cutscenes.json's value, "pages": {page: [{en, fr, ...}] or null},
    "pictures": {path: {"palette", "pixels"}}, "newPictures": [{"path",
    "about", "palette", "pixels"}]}, each optional."""
    files = Files(game)
    if not available(game):
        raise EditError("%s has no cutscenes.json: its cutscenes aren't data yet" % game.title)
    out_json, out_bytes = [], []
    diags = []
    data = body.get("cutscenes")
    pages = body.get("pages") or {}
    if not isinstance(pages, dict):
        raise EditError("\"pages\" is {page: [strings] or null}")
    pics = body.get("pictures") or {}
    new_pics = body.get("newPictures") or []
    if not isinstance(pics, dict) or not isinstance(new_pics, list):
        raise EditError("\"pictures\" is {path: {palette, pixels}}, \"newPictures\" [{path, about, palette, pixels}]")
    manifest = D.read_json(files.manifest)
    known = {p["path"] for p in pictures(files)}

    # new pictures: their PNG, manifest entry and classes block lines
    entries = []
    numbers = [int(m.group(1)) for p in known if (m := re.match(r"cutscenes/(\d+)_", p))]
    classes = {e.get("class") for e in manifest["assets"]}
    for k, np in enumerate(new_pics):
        where = "newPictures/%d" % k
        if not isinstance(np, dict):
            raise EditError("%s: {path, about, palette, pixels}" % where)
        path = np.get("path")
        m = re.fullmatch(r"cutscenes/(\d{2,})_([a-z0-9_]+)\.png", path or "")
        if not m:
            raise EditError("%s: %r isn't a new picture's path (cutscenes/NN_<name>.png, a lower-case name)"
                            % (where, path))
        n = int(m.group(1))
        if path in known or n in numbers:
            raise EditError("%s: picture %02d is there already (%s)" % (where, n, path))
        cls = "CutscenePicture%02d" % n
        if cls in classes:
            raise EditError("%s: the class %s is there already" % (where, cls))
        numbers.append(n)
        known.add(path)
        entry = {"path": path, "kind": "bitmap", "symbol": "gCutscenePicture%02dBitmap" % n, "class": cls,
                 "about": str(np.get("about") or "a cutscene picture (the cutscene editor's)"),
                 "object": PICTURE_OBJECT, "palette": {"symbol": "gCutscenePicture%02d" % n, "colors": 256}}
        entries.append(entry)
        png = _picture_bytes(None, np.get("palette"), _b64(np.get("pixels"), where), where)
        out_bytes.append((files.rel(game.graphics_dir) + "/" + path, png))
    if entries:
        manifest = json.loads(json.dumps(manifest))
        manifest["assets"] += entries
        out_json.append((files.rel(files.manifest), manifest))
        text = files.symbols.read_text()
        if SYMBOLS_END not in text:
            raise EditError("cxx_symbols.txt has no graphics classes block")
        mangle = lambda cls, member: "_Q36Engine3Gfx%d%s.%s" % (len(cls), cls, member)  # noqa: E731
        lines = "".join("%s %s\n%s %s\n" % (mangle(e["class"], "palette"), e["palette"]["symbol"],
                                              mangle(e["class"], "bitmap"), e["symbol"]) for e in entries)
        out_bytes.append(("cxx_symbols.txt", text.replace(SYMBOLS_END, lines + SYMBOLS_END, 1).encode()))

    # changed pictures
    for path, pic in pics.items():
        where = "pictures/" + path
        if path not in {p["path"] for p in pictures(files)}:
            raise EditError("%s isn't a cutscene picture of the graphics manifest" % path)
        if not isinstance(pic, dict):
            raise EditError("%s: {palette, pixels}" % where)
        p = game.graphics_dir / path
        data_px = _b64(pic.get("pixels"), where)
        new = _picture_bytes(p.read_bytes(), pic.get("palette"), data_px, where)
        out_bytes.append((files.rel(p), new))

    # cutscenes.json: the names it gives must be there
    final_pages, _ = read_pages(files)
    for page, strings in pages.items():
        if strings is None:
            final_pages.pop(page, None)
        else:
            final_pages[page] = strings
    if data is not None:
        diags += model_problems(files, data, known, final_pages)
        out_json.append((files.rel(files.cutscenes), data))
    diags += text_problems(files, pages)
    if diags:
        raise EditError("the cutscenes' checks reject it", diags)

    if pages:
        value = data if data is not None else D.read_json(files.cutscenes)
        records = {s.get("name"): s.get("picture") for s in value.get("slides", [])}
        page_pictures = {}
        for c in value.get("cutscenes", []):
            for s in c.get("slides", []):
                if s.get("text") and s["text"] not in page_pictures:
                    page_pictures[s["text"]] = records.get(s.get("slide"))
        for p, text in text_edits(files, pages, page_pictures).items():
            out_bytes.append((files.rel(p), text.encode("utf-8")))
    return {"json": out_json, "bytes": out_bytes}


def model_problems(files, data, picture_paths, pages):
    """cutscenes.json's checks the editor can name a place for (the
    build's own, crashdata.cutscenes.load, run after the save): the slide
    records' names, pictures, songs and sound effects; each cutscene's
    slides' records and pages; no record or page left unused."""
    out = []
    where = files.rel(files.cutscenes)
    if not isinstance(data, dict):
        return [_diag("cutscenes.json is an object", where, "")]
    song_names = {s["name"] for s in songs(files)}
    sfx_names = {s["name"] for s in sound_effects(files)}
    slides = data.get("slides") if isinstance(data.get("slides"), list) else []
    cutscenes = data.get("cutscenes") if isinstance(data.get("cutscenes"), list) else []
    names = {}
    for i, s in enumerate(slides):
        if not isinstance(s, dict):
            continue
        ptr = "/slides/%d" % i
        if s.get("name") in names:
            out.append(_diag("two slide records are named %r" % s.get("name"), where, ptr + "/name"))
        names[s.get("name")] = i
        if s.get("picture") not in picture_paths:
            out.append(_diag("%r is no cutscene picture of the graphics manifest" % s.get("picture"), where,
                             ptr + "/picture"))
        if s.get("song") not in song_names:
            out.append(_diag("%r is no song of the song table (SONG_*)" % s.get("song"), where, ptr + "/song"))
        if s.get("sfx") not in sfx_names:
            out.append(_diag("%r is no sound effect of the table (SFX_*, SFX_NONE)" % s.get("sfx"), where,
                             ptr + "/sfx"))
    if len(cutscenes) > MAX_CUTSCENES:
        out.append(_diag("%d cutscenes: the text's keys have room for %d (cutsceneNN)" % (len(cutscenes),
                                                                                          MAX_CUTSCENES),
                         where, "/cutscenes"))
    used, used_pages = set(), set()
    for ci, c in enumerate(cutscenes):
        if not isinstance(c, dict):
            continue
        for si, s in enumerate(c.get("slides") or []):
            if not isinstance(s, dict):
                continue
            ptr = "/cutscenes/%d/slides/%d" % (ci, si)
            if s.get("slide") not in names:
                out.append(_diag("no slide record named %r" % s.get("slide"), where, ptr + "/slide"))
            used.add(s.get("slide"))
            if s.get("text") is not None:
                if s["text"] not in pages:
                    out.append(_diag("page %s has no text" % s["text"], where, ptr + "/text"))
                used_pages.add(s["text"])
    for name, i in names.items():
        if name not in used:
            out.append(_diag("slide record %s is in no cutscene" % name, where, "/slides/%d" % i))
    for page in pages:
        if page not in used_pages:
            out.append(_diag("page %s is no slide's text (delete it, or show it)" % page, "text", "/" + page))
    return out


# ---------------------------------------------------------------------------
# The text as the game draws it

class TextFont:
    """The small font's metrics, as Font's methods use them (font.cpp,
    font_glyph.cpp): `chars` (glyph g is chars[g]; a character not there
    draws glyph 0), each glyph's (advance, shape, Y offset)."""

    def __init__(self, chars, records, space=FONT_SPACE, line_height=FONT_LINE_HEIGHT):
        self.lookup = {}
        for j, c in enumerate(bytes(chars) + b"\0"):  # InitSmallFont's 0x50 entries
            self.lookup.setdefault(c, j)
        self.records = records
        self.space, self.line_height = space, line_height

    @classmethod
    def of(cls, files):
        src = files.fonts_src.read_text(encoding="latin-1")
        return cls(T.read_c_string(src, "gSmallFontChars"), _glyph_records(src, "gSmallFontGlyphs"))

    def glyph(self, c):
        g = self.lookup.get(c, 0)
        return g if g < len(self.records) else 0

    def measure(self, token):
        """Font::MeasureChars."""
        w = 0
        for c in token:
            if c == 0x20:
                w += self.space
            elif c != 0x0A:
                w += self.records[self.glyph(c)][0]
        return w


def word_length(text, i):
    """GetWordLength (text_box.cpp): the token at i, up to and including a space."""
    n = 0
    while i + n < len(text):
        n += 1
        if text[i + n - 1] == 0x20:
            break
    return n


def draw_wrapped(text, font, box=BOX):
    """DrawWrappedText (wrapped_text.cpp) in WRAPPED_TEXT_MODE_PER_WORD, as
    CutscenePlayer::Run calls it: (the bytes it consumed, the glyphs it drew
    as (glyph, x, y): the OBJs' top-left corners, y in OAM's 8 bits)."""
    limit = box["h"] // font.line_height
    x, y = box["x"], box["y"]
    width = lines = used = 0
    drawn = []
    i = 0

    def put(c):
        nonlocal x, y
        if c == 0x20:
            x += font.space
        elif c == 0x0A:
            x, y = box["x"], y + font.line_height
        else:
            g = font.glyph(c)
            adv, _, yoff = font.records[g]
            drawn.append((g, x, (y + yoff) & 0xFF))
            x += adv

    token = 0
    while (token < len(text)) and lines < limit:
        n = word_length(text, i)
        token, i = i, i + n
        if n == 0:
            break
        tok = text[token:token + n]
        if tok[0] == 0x2F:  # '/': /b a half-line break, /n a newline, any other word skipped
            if tok[1:2] == b"b":
                y += 4
            if tok[1:2] in (b"b", b"n"):
                put(0x0A)
                width = 0
                lines += 1
            used += n
            continue
        w = font.measure(tok)
        if width + w <= box["w"]:
            for c in tok:
                put(c)
            width += w
            used += n
        else:
            lines += 1
            if lines >= limit:
                break
            put(0x0A)
            for c in tok:
                put(c)
            width = w
            used += n
    return used, drawn


def screens(text, font, box=BOX):
    """A string's screens: what each WaitForKeyPress shows (CutscenePlayer::Run
    calls DrawWrappedText until the string is drawn), each its glyphs."""
    out, pos = [], 0
    while pos < len(text):
        n, drawn = draw_wrapped(text[pos:], font, box)
        out.append(drawn)
        if n == 0:  # the game would draw this screen forever
            break
        pos += n
    return out


def cutscene_steps(game, index, language="en"):
    """A cutscene's key presses as the game waits for them: [(slide index,
    string index or None, screen index, wait, buttons)], in order (the
    text in `language`)."""
    files = Files(game)
    data = D.read_json(files.cutscenes)
    c = data["cutscenes"][index]
    records = {s["name"]: s for s in data["slides"]}
    pages, _ = read_pages(files)
    font = TextFont.of(files)
    out = []
    for si, s in enumerate(c["slides"]):
        r = records[s["slide"]]
        strings = pages.get(s.get("text"), []) if s.get("text") else []
        if not strings:
            out.append((si, None, 0, r["wait"], r["buttons"]))
            continue
        for j, st in enumerate(strings):
            data_ = st[language].encode(T.ENCODING, "replace")
            for k, _ in enumerate(screens(data_, font)):
                out.append((si, j, k, r["wait"], r["buttons"]))
    return out
