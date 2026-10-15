"""The sample mod (docs/modding.md): a fifth world, built from the data
alone. tools/apply_mod.py runs apply(tree) in a copy of the tree; a plain
`make` there builds it, and mods/sample/boottest.py plays it in mGBA.

What it adds (each step names its doc):

- a song, `sample_tune`: a short tune written for the mod (make_xm below),
  its melody on a new instrument with its own synthesized sample, its drum
  on one of the ROM's pool instruments (docs/audio.md, "Adding a song",
  "New instruments");
- a level theme, `THEME_SAMPLE`, playing it (docs/levels.md, "Adding a
  theme");
- a room, `sample_grove/main`: jungle jam's room copied under its own
  symbols, with one 8x8 block of BG0 repainted, a new tile (docs/levels.md,
  "Adding a room");
- an actor category, `CATEGORY_SAMPLE_ICE`: snow crash's polar stage with
  its own OBJ palette and BG palette cycle, both recoloured
  (docs/levels.md, "Category graphics");
- a cutscene, `CUTSCENE_SAMPLE_CLEARED`, with a picture drawn by
  make_picture below and a page of text in the six languages
  (docs/data.md, "A new cutscene");
- world 5 (world 4 in the data): three numbered levels and a boss, so a
  partial level-select page, its number icons 21-23, and 29 levels, past
  the 4 Kbit save's 28 (docs/levels.md, "Level-select pages", "The save"):
  - `LEVEL_SAMPLE_GROVE`, the new room, on the new theme;
  - `LEVEL_SAMPLE_SLOPE`, the new category's stage;
  - `LEVEL_SAMPLE_SEWER`, just in slime's room again;
  - `LEVEL_SAMPLE_SHOWDOWN`, the boss: Dingodile's room with
    `"boss_kind": "DINGODILE"` (the boss HUD) and an `"on_clear"` that
    plays the new cutscene and marks it cleared (docs/levels.md, "Boss
    levels");
- each level's name in the six languages (docs/text.md, "A new level's
  name").

Every asset is generated here or derived from the tree's own data, so the
mod needs no baserom.gba and has no copied art.
"""
import json
import re
import struct

import jasc_pal
from PIL import Image, ImageDraw

SONG = "sample_tune"
ROOM = "sample_grove/main"
CATEGORY = "SAMPLE_ICE"
CUTSCENE = "CUTSCENE_SAMPLE_CLEARED"
PICTURE = "cutscenes/24_sample_crate.png"
OBJ_PALETTE = "vehicles/polar/sample_ice.pal"
PALETTE_CYCLE = "vehicles/polar/sample_ice_cycle/"

LEVELS = [
    {"constant": "LEVEL_SAMPLE_GROVE", "names": {
        "en": "sample grove", "fr": "bosquet d'essai", "de": "probehain", "es": "bosque de prueba",
        "it": "boschetto di prova", "nl": "proefbos"}},
    {"constant": "LEVEL_SAMPLE_SLOPE", "names": {
        "en": "sample slope", "fr": "pente d'essai", "de": "probehang", "es": "pendiente de prueba",
        "it": "pendio di prova", "nl": "proefhelling"}},
    {"constant": "LEVEL_SAMPLE_SEWER", "names": {
        "en": "sample sewer", "fr": "égout d'essai", "de": "probekanal", "es": "cloaca de prueba",
        "it": "fogna di prova", "nl": "proefriool"}},
    {"constant": "LEVEL_SAMPLE_SHOWDOWN", "names": {
        "en": "sample showdown", "fr": "duel d'essai", "de": "probeduell", "es": "duelo de prueba",
        "it": "duello di prova", "nl": "proefduel"}},
]

CUTSCENE_TEXT = {
    "en": "aku aku:  the sample world is cleared, crash!",
    "fr": "aku aku:  le monde d'essai est terminé, crash!",
    "de": "aku aku:  die probewelt ist geschafft, crash!",
    "es": "aku aku:  ¡el mundo de prueba está superado, crash!",
    "it": "aku aku:  il mondo di prova è finito, crash!",
    "nl": "aku aku:  de proefwereld is klaar, crash!",
}


# ---------------------------------------------------------------------------
# The song: an XM of two channels, 16-row patterns, a drum on the pool's
# instrument 2 (docs/audio.md, "Adding a song": named "Instrument 2") and a
# melody on "sample lead", a new instrument (docs/audio.md, "New
# instruments"): its sample, 4 cycles of a buzzy wave synthesized here,
# looped, and a volume envelope with a sustain point.

def lead_instrument():
    buzz = bytes(round(56 * (i % 64) / 32 - 56 + (36 if i % 64 < 32 else -36)) & 0xFF for i in range(256))
    deltas = bytes((v - p) & 0xFF for v, p in zip(buzz, b"\0" + buzz))  # XM stores deltas
    env = [(0, 64), (6, 44), (12, 40), (24, 0)]
    head = struct.pack("<I22sBHI", 263, b"sample lead".ljust(22, b"\0"), 0, 1, 40) + bytes(96)
    head += b"".join(struct.pack("<HH", x, y) for x, y in env).ljust(48, b"\0") + bytes(48)
    # 4 points, sustain at point 2, no loop; envelope on with sustain
    head += bytes([len(env), 0, 2, 0, 0, 0, 0, 0, 1 | 2, 0, 0, 0, 0, 0]) + bytes(4)
    head = head.ljust(263, b"\0")
    # 256 8-bit samples, a forward loop over all of them, volume 48
    sample = struct.pack("<IIIBbBBbB22s", 256, 0, 256, 48, 0, 1, 0x80, 0, 0, b"buzz")
    return head + sample + deltas


def make_xm(path):
    rows, names = 16, ["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"]

    def note(n, octave):
        return octave * 12 + names.index(n) + 1  # XM: 1 is C-0

    def pattern(melody, drums):
        data = b""
        for m, d in zip(melody.split(), drums):
            data += b"\x80" if m == "." else bytes([0x83, note(m[:-1], int(m[-1])), 1])
            data += bytes([0x83, note("C", 4), 2]) if d == "x" else b"\x80"
        assert len(data) and len(melody.split()) == rows == len(drums)
        return struct.pack("<IBHH", 9, 0, rows, len(data)) + data

    pats = [
        pattern("E5 . G5 . A5 . G5 . E5 . D5 . C5 . D5 .", "x...x...x...x.x."),
        pattern("E5 . E5 G5 A5 . C6 . A5 . G5 . E5 . . .", "x...x.x.x...x.xx"),
        pattern("D5 . . . C5 . . . A4 . . . C5 . . .", "x.......x.......")]
    order = [0, 1, 0, 2]
    head = b"Extended Module: " + b"sample tune".ljust(20, b"\0") + b"\x1a" + b"apply_mod".ljust(20, b"\0")
    head += struct.pack("<H", 0x104) + struct.pack("<IHHHHHHHH", 276, len(order), 0, 2, len(pats), 2, 1, 6, 150)
    head += bytes(order).ljust(256, b"\0")
    inst = lead_instrument() + struct.pack("<I22sBH", 29, b"Instrument 2".ljust(22, b"\0"), 0, 0)
    with open(path, "wb") as f:
        f.write(head + b"".join(pats) + inst)


# ---------------------------------------------------------------------------
# The cutscene picture: 240x160, 8bpp, its 256-colour palette in the PNG
# (docs/graphics.md, "New graphics"). A night sky, a hill and a crate.

def make_picture(path):
    pal = []
    for i in range(32):  # 0-31: the sky, dark blue to purple
        pal.append((i // 4, i // 8, 8 + i // 2))
    pal += [(31, 30, 18), (12, 20, 6), (8, 14, 4), (22, 12, 4), (16, 8, 2), (28, 20, 8), (31, 31, 31)]
    colors = [jasc_pal.to_rgb(b << 10 | g << 5 | r) for r, g, b in pal]
    colors += [(0, 0, 0)] * (256 - len(colors))
    moon, hill, hill_dark, wood, wood_dark, band, star = range(32, 39)
    img = Image.new("P", (240, 160), 0)
    img.putpalette([c for rgb in colors for c in rgb])
    d = ImageDraw.Draw(img)
    for y in range(160):
        d.line([(0, y), (239, y)], fill=min(31, y // 4))
    for k in range(40):
        img.putpixel(((k * 97) % 240, (k * 53) % 90), star)
    d.ellipse([170, 14, 206, 50], fill=moon)
    d.ellipse([-60, 110, 300, 260], fill=hill)
    d.ellipse([-20, 124, 140, 220], fill=hill_dark)
    d.rectangle([96, 70, 150, 124], fill=wood, outline=wood_dark, width=3)
    d.line([(96, 70), (150, 124)], fill=wood_dark, width=4)
    d.line([(150, 70), (96, 124)], fill=wood_dark, width=4)
    d.rectangle([96, 94, 150, 100], fill=band)
    img.save(path)


# ---------------------------------------------------------------------------
# The number icons 21-23 of the level select (bank 51's animation 0,
# docs/graphics.md "New frames"): the "2" of frame 1 ("2") then the digit
# of frames 0-2 ("1"-"3").

def add_number_icons(tree, numbers):
    b = tree.path("graphics/sprites/bank51_level_select_icons")
    bank = json.loads((b / "bank.json").read_text())
    two = Image.open(b / "f001.png")
    steps = []
    for n in numbers:
        dig = Image.open(b / ("f%03d.png" % (n % 10 - 1)))
        w = two.size[0] + dig.size[0] - 1
        width = (w + 7) // 8 * 8
        img = Image.new("P", (width, 16), 0)
        img.putpalette(two.getpalette())
        for src, x0 in ((two, 0), (dig, two.size[0] - 1)):
            for y in range(16):
                for x in range(src.size[0]):
                    if src.getpixel((x, y)):
                        img.putpixel((x0 + x, y), src.getpixel((x, y)))
        k = len(bank["frames"])
        img.save(b / ("f%03d.png" % k), bits=4)
        ox = width // 2
        pieces = [["16x16", -ox, -4]] + ([["8x16", 16 - ox, -4]] if width == 24 else [])
        bank["frames"].append({"origin": [ox, 4], "pieces": pieces, "layout": 2, "tiles": "own",
                               "palettes": [80], "boxes": [[-ox, -4, w, 8]]})
        steps.append(k)
    bank["new_steps"] = {"0": steps}
    lines = ["    " + json.dumps(f, separators=(", ", ": ")) for f in bank["frames"]]
    (b / "bank.json").write_text('{\n  "frames": [\n' + ",\n".join(lines) + '\n  ],\n  "new_steps": '
                                 + json.dumps(bank["new_steps"]) + "\n}\n")


# ---------------------------------------------------------------------------
# The category's colours: the polar OBJ palette (gPolarCategoryPalette's 256
# colours, from its C) with red and blue swapped, and the polar BG palette
# cycle's 32 frames with red and green swapped.

def swap_rb(v):
    return (v & 0x83E0) | (v & 31) << 10 | (v >> 10 & 31)


def swap_rg(v):
    return (v & 0xFC00) | (v & 31) << 5 | (v >> 5 & 31)


def polar_obj_palette(tree):
    src = tree.path("src/game/vehicles/polar/data/polar_anim_family.cpp").read_text()
    body = src[src.index("gPolarCategoryPalette[0x200] = {"):]
    body = body[:body.index("};")]
    return [int(v, 16) for v in re.findall(r"0x([0-9A-Fa-f]{4})\b", body.split("{", 1)[1])][:256]


def make_category_graphics(tree):
    jasc_pal.write_jasc(tree.path("graphics/" + OBJ_PALETTE), [swap_rb(v) for v in polar_obj_palette(tree)])
    src = tree.path("graphics/vehicles/polar/palette_cycle")
    dst = tree.path("graphics/" + PALETTE_CYCLE)
    dst.mkdir()
    for k in range(32):
        frame = jasc_pal.read_jasc(src / ("%02d.pal" % k))
        jasc_pal.write_jasc(dst / ("%02d.pal" % k), [swap_rg(v) for v in frame])


# ---------------------------------------------------------------------------
# The room: jungle jam's, as room 41, with one block of BG0 repainted (its
# colours reversed in their 16-colour bank): a new tile of the jungle BG0
# set, after the ROM's.

def make_room(tree):
    tree.copy_room("jungle_jam/main", ROOM, {"gRoom17": "gRoom41", "gLevelRoom17": "gLevelRoom41"})
    p = tree.path("data/levels/" + ROOM + "/bg0.png")
    img = Image.open(p)
    x0, y0 = 4 * 8, 44 * 8  # a block of ground near the start marker (31, 363)
    for y in range(y0, y0 + 8):
        for x in range(x0, x0 + 8):
            v = img.getpixel((x, y))
            if v % 16:
                img.putpixel((x, y), v // 16 * 16 + 16 - v % 16)
    img.save(p)


def apply(tree):
    # the song and the theme that plays it
    make_xm(tree.path("sound/songs/%s.xm" % SONG))
    tree.path("sound/songs/%s.json" % SONG).write_text(json.dumps(
        {"title": "sample tune", "artist": "the sample mod"}, indent=2) + "\n")
    with tree.json("sound/gax_manifest.json") as m:
        m["song_table"].append({"song": SONG, "comment": "the sample mod's theme (THEME_SAMPLE) and cutscene"})

    # the graphics: the category's colours, the cutscene picture, the icons
    make_category_graphics(tree)
    make_picture(tree.path("graphics/" + PICTURE))
    tree.add_graphics(
        {"path": OBJ_PALETTE, "kind": "category_palette", "symbol": "gSampleIceCategoryPalette", "colors": 256},
        {"path": PALETTE_CYCLE, "kind": "palette_cycle", "symbol": "gSampleIcePaletteCycleFrames", "frames": 32},
        {"path": PICTURE, "kind": "bitmap", "symbol": "gCutscenePicture24Bitmap", "class": "CutscenePicture24",
         "about": "the sample mod's cutscene picture: a crate under the moon", "object": "cutscene_pictures",
         "palette": {"symbol": "gCutscenePicture24", "colors": 256}})
    add_number_icons(tree, [21, 22, 23])

    # the cutscene
    with tree.json("data/cutscenes.json") as c:
        c["slides"].append({"name": "sample_crate", "picture": PICTURE, "wait": 0, "fade": 0, "fade_after": 0,
                            "buttons": 1, "duck_music": 0, "rearm_sfx": 0, "song": "SONG_SAMPLE_TUNE",
                            "sfx": "SFX_NONE"})
        page = "cutscene%02d.page0" % len(c["cutscenes"])
        c["cutscenes"].append({"constant": CUTSCENE, "about": "the sample mod's boss beaten",
                               "slides": [{"slide": "sample_crate", "text": page}]})
    tree.add_cutscene_text(page + ".0", CUTSCENE_TEXT, "The sample mod's cutscene (mods/sample).")

    # the room, the category, the theme, the world and its levels
    make_room(tree)
    with tree.levels() as d:
        d["rooms"].append(ROOM)
        snow = next(c for c in d["categories"] if c["constant"] == "CATEGORY_SNOW_CRASH")
        d["categories"].append(dict(snow, constant="CATEGORY_" + CATEGORY, name="sample ice", palette=OBJ_PALETTE,
                                    palette_cycle=PALETTE_CYCLE))
        d["themes"].append({"constant": "THEME_SAMPLE", "name": "sample", "song": SONG,
                            "picture": "level_select/pictures/jungle.png", "palette_cycle": "jungle"})
        theme = len(d["themes"]) - 1
        d["worlds"].append({"page_title": 9, "entry_box": 4, "boss_icon": 2, "opened_by": {"levels_cleared": True}})
        by = {lv["constant"]: lv for lv in d["levels"]}
        common = {"world": 4, "times": [1500, 1200, 1000], "mask_assist_deaths": 4, "crate_assist_deaths": 4}
        new = [
            dict(common, theme=theme, rooms=[ROOM]),
            dict(common, theme=by["LEVEL_SNOW_CRASH"]["theme"], rooms=[{"stage": CATEGORY}]),
            dict(common, theme=by["LEVEL_JUST_IN_SLIME"]["theme"], rooms=["just_in_slime/main"]),
            dict(common, boss=True, boss_kind="DINGODILE", on_clear={"cutscene": CUTSCENE, "mark_cleared": True},
                 theme=by["LEVEL_DINGODILE"]["theme"], rooms=["dingodile/main"]),
        ]
        for lv, info in zip(new, LEVELS):
            lv["constant"] = info["constant"]
            lv["name"] = info["names"]["en"]
            lv["name_text"] = tree.add_level_name(info["constant"], info["names"])
        lv = d["levels"]
        last_boss = max(i for i, x in enumerate(lv) if x.get("boss") and "world" in x)
        numbered = max(i for i, x in enumerate(lv) if not x.get("boss"))
        # the numbered levels after the last world's, the boss after the
        # last boss, before mega-mix (docs/levels.md, "Adding a world")
        d["levels"] = lv[:numbered + 1] + new[:3] + lv[numbered + 1:last_boss + 1] + new[3:] + lv[last_boss + 1:]

    # the C names of the new graphics and the new room's class in
    # cxx_symbols.txt, which gfx_manifest.py check asks for
    tree.run("python3", "tools/gfx_manifest.py", "symbols")
    tree.run("python3", "tools/levels.py", "symbols")
