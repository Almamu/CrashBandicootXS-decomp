#!/usr/bin/env python3
"""`tools/magic_numbers.py --screen`: the literals that may come from the
screen size (#979).

Scans the game code and its data tables (src/, src/.../data/ included, and
include/; comments and strings blanked) for the literals a 240x160
screen leaves behind, and puts each one under a kind:

  size      240 / 160 (`0xF0` / `0xA0`): the screen's width and height
  last      239 / 159 (`0xEF` / `0x9F`): its last column and row
  centre    120 / 80 (`0x78` / `0x50`): half of it
  q8        the Q8 forms of those (`0xF000`, `0xA000`, `0x7800`,
            `0x5000`, `0xEF00`, `0x9F00`, `0xEFFF`, `0x9FFF`, and as
            words `0xFFFF1000` = -0xF000), and the few derived Q8 values
            found by hand (the camera's look-ahead, a sixth of the
            screen: `0x2800`, `0x1AAA`; the affine picture's centre,
            `0x7788` = 120 * 255)
  range     the screen plus a margin, found by hand: the 440x280
            activation window (`0x1B800` x `0x11800` in Q8)
  bitmap    a 240x160 frame in bytes, halfwords or nibbles (`0x9600`,
            `0x12C00`, `0x4B00`)
  tiles     30 / 20 (and 29 / 19, 31 / 21) next to a tile, row or
            column: the screen in tiles
  mask      `& 0x1FF` / `& 0xFF` (`511` / `255`) on a coordinate: OAM's
            9-bit X and 8-bit Y
  wrap      512 / 256 (`0x200` / `0x100`) compared with or added to a
            coordinate: OBJ X and Y wrap
  oam       128 (`0x80`) on a line that names the OAM: its entry count
  scanline  227 / 228 (`0xE3` / `0xE4`) and 68 on a VCOUNT line
  layout    a fixed position in a 2D layout (LAYOUTS: a SetPos/Move
            call, an icon or text box placement, a position table in
            src/.../data): an argument that adds a literal to nothing but
            measured sizes and counters, so it is measured from the
            screen's (0,0) instead of from an anchor (the art's origin
            DISPLAY_ART_X/Y, or an edge: DISPLAY_RIGHT/BOTTOM)

and then classifies it with CLASSES, a hand-made table of (category,
file, line pattern, reason), first match wins:

  screen      derived from the screen size: spell it with include/screen.h
  hardware    fixed by the GBA (a register's field width, the 32x32 BG
              screen block, the Mode 3/4 frame), the platform layer's
              job; spelled through a gba/ macro where the code masks
  coincidence the same number for something else (a timer, a speed, a
              struct offset); left, or named for what it is
  unclassified no rule yet

A literal that is already a name (`DISPLAY_WIDTH`, `DISPLAY_CENTER_X`,
`OAM_X(x)`) isn't a literal, and a literal added to a screen.h anchor
(`DISPLAY_ART_X + 0xa8`, `DISPLAY_RIGHT(28)`) is an offset from it, not
a candidate, so the screen counts go down as sites are converted; the
other two categories stay.

Usage:
  tools/magic_numbers.py --screen                  every site: file:line
                                                   kind value category reason
  tools/magic_numbers.py --screen --show           ... with the source line
  tools/magic_numbers.py --screen --report [OUT]   Markdown: counts per
                                                   category, kind and
                                                   directory, the sites
  tools/magic_numbers.py --screen --json           every site as JSON
  tools/magic_numbers.py --screen --path P         only files under P
  tools/magic_numbers.py --screen --category C     only category C
                                                   (repeatable)
  tools/magic_numbers.py --screen --check          fail if a site has no rule
                                                   (CI: a new screen literal)
"""

import collections
import json
import os
import re
import sys

import magic_numbers as mn

ROOT = mn.ROOT
CATEGORIES = ("screen", "hardware", "coincidence", "unclassified")

Site = collections.namedtuple("Site", "file line kind value literal category reason func source")

# value -> kind, for the literals that are candidates wherever they are.
VALUES = {
    240: "size",
    160: "size",
    239: "last",
    159: "last",
    120: "centre",
    80: "centre",
    0xF000: "q8",
    0xA000: "q8",
    0x7800: "q8",
    0x5000: "q8",
    0xEF00: "q8",
    0x9F00: "q8",
    0x9600: "bitmap",
    0x12C00: "bitmap",
    0x4B00: "bitmap",
    0xEFFF: "q8",
    0x9FFF: "q8",
    0xFFFF1000: "q8",  # -0xF000 as a word
    0xFFFF6000: "q8",  # -0xA000
    0x7788: "q8",  # 120 * 255
    0x4FB0: "q8",  # 80 * 255
    0x1AAA: "q8",  # INT_TO_Q8(160) / 6
    0x1AA9: "q8",
    440: "range",  # 240 + 2 * 100
    280: "range",  # 160 + 2 * 60
    0x1B800: "range",  # INT_TO_Q8(440)
    0x11800: "range",  # INT_TO_Q8(280)
}
# Candidates only in a context: value -> (kind, line regex).
TILES_RE = re.compile(r"(?i)tile|\bcols?\b|\brows?\b|column")
OAM_RE = re.compile(r"(?i)\boam|oam_|Oam")
VCOUNT_RE = re.compile(r"(?i)vcount|scanline")
CONTEXT = {
    30: ("tiles", TILES_RE),
    20: ("tiles", TILES_RE),
    29: ("tiles", TILES_RE),
    19: ("tiles", TILES_RE),
    31: ("tiles", TILES_RE),
    21: ("tiles", TILES_RE),
    128: ("oam", OAM_RE),
    227: ("scanline", VCOUNT_RE),
    228: ("scanline", VCOUNT_RE),
    68: ("scanline", VCOUNT_RE),
    0x2800: ("q8", re.compile(r"\bv[xy]\b")),  # INT_TO_Q8(240) / 6
    0x27FF: ("q8", re.compile(r"\bv[xy]\b")),
}
KINDS = ("size", "last", "centre", "q8", "range", "bitmap", "tiles", "mask", "wrap", "oam", "scanline", "layout")
MASKS = (0x1FF, 0xFF)
WRAPS = (0x200, 0x100)

LITERAL = re.compile(r"(?<![\w.])(0[xX][0-9a-fA-F]+|\d+)[uUlL]*(?![\w.])")
# A name that holds a screen coordinate: x, y, sx, sy, screenX, posY, left...
COORD = re.compile(
    r"(?<![\w.>])(?:s?[xy]\d?|[xy](?:Lo|Hi|Low|High)|screen[XY]\w*|pos[XY]?|\w*Pos[XY]|left|right|top|bottom)\b"
)

# A literal just after one of these is an offset from a screen.h anchor
# (`DISPLAY_ART_Y + 0x87`, `DISPLAY_RIGHT(0x14)`): spelled from the screen.
ANCHORED = re.compile(r"\bDISPLAY_\w+\s*[-+]\s*$|\bDISPLAY_(?:RIGHT|BOTTOM)\(\s*$")

# The 2D layouts (kind `layout`). On a line of a UI file (UI) with a
# position call (LAYOUT_CALLS: the call's name and which of its arguments
# are the x and y, None for all), a local row or column initialized to a
# literal (LAYOUT_LOCAL) or a sprite's Q8 position set to one
# (LAYOUT_Q8), and on a `{ x, y }` line of a position table in src/.../data
# (LAYOUT_TABLES), each such argument is a position. One that adds a
# literal to nothing but measured sizes, counters and slide offsets
# (LAYOUT_FREE: `0x87`, `0xa0 - w`, `item * 10 + 0x80`) is measured from
# the screen's (0,0): a layout site. One with a DISPLAY_ name or another
# position in it (`x`, `pos.x`, `gPauseGemIconPos[0].x`, `label1`) is
# anchored, or relative to something that is.
UI = re.compile(r"src/(game/ui|engine/save|engine/text)/|src/game/level/game_progress\.")
LAYOUT_CALLS = {
    "SetPos": None,
    "Move": None,
    "SetPixelPos": None,
    "SetEntityPixelPos": (1, 2),
    "SetAabbPos": (1, 2),
    "SET_ROW_OBJ_POS": (1, 2),
    "DRAW_ROW": (1, 2),
    "DrawSlotStats": (0, 1),
}
LAYOUT_CALL = re.compile(r"\b(" + "|".join(LAYOUT_CALLS) + r")\(")
LAYOUT_LOCAL = re.compile(r"^\s*(?:screen_\w+ |s32 |u32 )?([xy]) = ([^;]*);")
LAYOUT_Q8 = re.compile(r"(?:->|\.)[xy] = (INT_TO_Q8\(-?(?:0x[0-9a-fA-F]+|\d+)\)|0x[0-9a-fA-F]+00);")
LAYOUT_TABLES = re.compile(r"src/(?:[a-z_]+/)+data/(level_select_layout|menu_tables|hud_font_tables)\.cpp")
LAYOUT_PAIR = re.compile(r"\{ *(-?\d+), *(-?\d+) *\}")
# The logo pieces' motions (struct delta_record, frontend.hpp): a hold,
# then the piece's x and y on the logo art.
LAYOUT_MOTIONS = re.compile(r"src/(?:[a-z_]+/)+data/(universal_vv_logo_motions|title_screen_gfx)\.cpp")
LAYOUT_MOTION = re.compile(r"^\s*\{ *\d+, *(\d+), *(\d+), ")
LAYOUT_FREE = {
    "w",
    "width",
    "item",
    "MeasureText",
    "GetUiText",
    "gSmallFont",
    "gLargeFont",
    "font",
    "text",
    "nameText",
    "panelSlideX",
    "INT_TO_Q8",
    "u32",
    "s32",
    "label",
}
IDENT = re.compile(r"[A-Za-z_]\w*")


def split_args(line, open_paren):
    """The arguments of the call whose `(` is at `open_paren`: a list of
    (start, text) up to the matching `)`."""
    out = []
    depth = 0
    start = open_paren + 1
    for j in range(open_paren, len(line)):
        c = line[j]
        if c in "([{":
            depth += 1
        elif c in ")]}":
            depth -= 1
            if depth == 0:
                out.append((start, line[start:j]))
                return out
        elif c == "," and depth == 1:
            out.append((start, line[start:j]))
            start = j + 1
    out.append((start, line[start:]))
    return out


def layout_literals(start, arg):
    """The offsets in the line of `arg`'s layout literals: those it adds
    to nothing but LAYOUT_FREE terms. None when it has an anchor or another
    position in it."""
    bare = LITERAL.sub(" ", arg)
    if any(name not in LAYOUT_FREE for name in IDENT.findall(bare)):
        return []
    out = []
    for m in LITERAL.finditer(arg):
        before = arg[: m.start()].rstrip()
        after = arg[m.end() :].lstrip()
        if before.endswith(("*", "/", "<<", ">>")) or after.startswith(("*", "/", "<<", ">>")):
            continue
        out.append(start + m.start())
    return out


def layout_sites(rel, line):
    """The offsets of `line`'s layout literals (kind `layout`)."""
    offsets = []
    if UI.search(rel):
        for m in LAYOUT_CALL.finditer(line):
            which = LAYOUT_CALLS[m.group(1)]
            for k, (start, arg) in enumerate(split_args(line, m.end() - 1)):
                if which is None or k in which:
                    offsets += layout_literals(start, arg)
        m = LAYOUT_LOCAL.match(line)
        if m:
            offsets += layout_literals(m.start(2), m.group(2))
        m = LAYOUT_Q8.search(line)
        if m:
            offsets += layout_literals(m.start(1), m.group(1))
    if LAYOUT_TABLES.search(rel):
        for m in LAYOUT_PAIR.finditer(line):
            if m.group(1) != "0" or m.group(2) != "0":  # not an unused { 0, 0 } slot
                offsets += [m.start(1), m.start(2)]
    if LAYOUT_MOTIONS.search(rel):
        m = LAYOUT_MOTION.match(line)
        if m and (m.group(1) != "0" or m.group(2) != "0"):  # not the { 0, ... } terminator
            offsets += [m.start(1), m.start(2)]
    return offsets


# The classification: (category, path regex, line regex, reason). The
# path is relative to the repository root; the line regex is searched in
# the site's source line (comments blanked). First match wins.
CLASSES = [
    # --- coincidences that a broader rule below would take -------------
    ("coincidence", r"src/game/ui/level_select_pages\.cpp", r"GetScroll", "the page-turn progress (0x100 = a page)"),
    ("coincidence", r"src/game/ui/level_select_entry\.cpp", r"phase", "a bob phase (a byte)"),
    ("coincidence", r"src/game/entities/ground_sprite_collide\.cpp", r".", "a collision probe step"),
    ("coincidence", r".", r"COS_Q8\(.*\* 80", "an orbit radius"),
    # --- hardware: the GBA's own formats and sizes --------------------
    ("hardware", r"include/gba/defines\.h", r"DISPLAY_(WIDTH|HEIGHT)", "the screen size itself (gba/defines.h)"),
    ("hardware", r"include/screen\.h", r"DISPLAY_ART_", "the fixed 240x160 art's size (screen.h)"),
    ("hardware", r"include/gba/", r".", "a GBA register offset or field value"),
    (
        "hardware",
        r".",
        r"\b(?:s?[xy]|screen[XY])\s*&\s*0x1?ff\b",
        "OAM attribute width (9-bit X, 8-bit Y): spell with OAM_X()/OAM_Y()",
    ),
    ("hardware", r"include/game/entities/sprite_obj\.hpp", r"table\[0x80\]", "the OAM entry count: spell with OAM_ENTRY_COUNT"),
    ("hardware", r"src/engine/gfx/fade\.cpp", r"0x1FF", "the 512 palette entries (BG + OBJ)"),
    (
        "hardware",
        r"src/game/level/bg_streamer\.cpp|src/engine/gfx/cell_anim\.cpp",
        r"0x1f",
        "a 32-entry BG screen-block row or column",
    ),
    ("hardware", r"src/game/ui/slideshow\.cpp", r"VRAM \+ 0xA000", "Mode 4's second frame (VRAM + 0xA000)"),
    ("hardware", r"src/game/ui/level_select_cursor\.cpp", r"OBJ_VRAM0 \+", "an OBJ VRAM offset"),
    ("hardware", r"src/game/ui/starfield\.cpp", r"\| 0xF000", "a BG map entry's palette bank (15)"),
    ("hardware", r"src/engine/gfx/sprite_arm\.cpp", r"0xF0", "a palette nibble"),
    # --- the data tables (src/.../data) -----------------------------------
    (
        "hardware",
        r"src/(?:[a-z_]+/)+data/(universal_vv_logo_motions|credits_title_logos)\.cpp",
        r"bg_package|\{ 0x1e, 0x8,",
        "a picture's own size in tiles (fixed art, like DISPLAY_ART_WIDTH)",
    ),
    (
        "screen",
        r"src/(?:[a-z_]+/)+data/(universal_vv_logo_motions|title_screen_gfx)\.cpp",
        r"^\s*\{ \d+, \d+, \d+, ",
        "a logo piece's position on the 240x160 logo art (DISPLAY_ART_X/Y)",
    ),
    ("coincidence", r"src/game/bosses/data/boss_motion_tables\.cpp", r".", "a position in the boss arena (Dingodile's stops, Q8)"),
    ("coincidence", r"src/engine/actor/data/actor_palette_cycle_frames\.cpp", r".", "a colour (BGR555)"),
    ("coincidence", r"src/(?:[a-z_]+/)+data/", r"^\s*(0x[0-9A-F]{4}, ){4}", "a colour (BGR555)"),
    ("coincidence", r"src/(?:[a-z_]+/)+data/(polar|jetpack)_anim_family", r"\{ -?\d+, -?\d+, -?\d+, \d+, \d+, \d+ \}", "a hit box (offset and size)"),
    ("coincidence", r"src/(?:[a-z_]+/)+data/", r"(JETPACK|POLAR)_KIND_", "a 3D course's spawn record (offset, angle, distance)"),
    ("coincidence", r"src/game/bosses/data/boss_pictures\.cpp", r".", "the sine table"),
    ("coincidence", r"src/game/vehicles/polar/data/polar_cell_anim\.cpp", r"tiles\[", "a picture's frame in tiles"),
    ("coincidence", r"src/game/crash/data/motion_records\.cpp", r".", "a motion record's speeds"),
    ("coincidence", r"src/(?:[a-z_]+/)+data/(hovercraft|airship)_attacks", r".", "a boss's attack parameters (frames, counts)"),
    ("coincidence", r"src/game/ui/data/vv_logo\.cpp", r".", "a logo piece's first hold (frames)"),
    ("coincidence", r"src/(?:[a-z_]+/)+data/sprite_banks_", r".", "sprite bank data (frame numbers, boxes, palettes)"),
    ("coincidence", r"src/game/ui/data/cutscene_slides\.cpp", r".", "a slide's duration (frames)"),
    # --- screen: derived from the screen size -------------------------
    ("screen", r"src/game/level/camera\.cpp", r"INT_TO_Q8\((120|80)\)", "the camera's target at the screen centre"),
    ("screen", r"src/game/level/camera\.cpp", r"0x2800|0x27FF|0x1AA[A9]", "the camera's look-ahead, a sixth of the screen"),
    ("screen", r"src/game/level/bg_layer_base\.cpp", r"-= 0x(f0|a0)", "a BG's scroll limit (CAMERA_MAX_X/Y)"),
    ("screen", r"src/engine/actor/actor_bg\.cpp", r"0xFFFF[16]000", "the 3D BG's scroll limit (CAMERA_MAX_X/Y in Q8)"),
    ("screen", r"src/game/level/bg_layer\.cpp", r"0x(ef|EF|9f|9F)\) / 8", "the tile rows and columns the screen spans"),
    (
        "screen",
        r"src/game/(crates|entities|objects)/",
        r"INT_TO_Q8\((240|160|0xf0|0xa0)\)",
        "the visible area as a Q8 rectangle (culling)",
    ),
    (
        "screen",
        r"src/game/crates/crate_list_update\.cpp|src/game/entities/(part_list_collide|entity)\.cpp",
        r"0x1b800|0x11800|INT_TO_Q8\((440|280)\)",
        "the activation window: the screen plus a 100x60 margin",
    ),
    (
        "screen",
        r".",
        r"(?:<=|>)\s*(?:0x9[fF]|0x[eE][fF]|159|239)\b",
        "an on-screen test against the last row or column",
    ),
    ("screen", r".", r"MeasureText|penX|\(u32\)\(240 - w\)|\(0xf0 - w\)", "text centred on the screen"),
    ("screen", r"src/game/ui/credits\.cpp", r"y \+ 0xa0", "a popup's start, just below the screen"),
    ("screen", r"src/game/ui/starfield\.cpp", r"0x4B00", "a 240x160 4bpp frame (buffer and DMA size)"),
    ("screen", r"src/game/ui/starfield\.cpp", r"0x1d|0x13", "the screen in tiles (the BG0 map's last column and row)"),
    (
        "screen",
        r"src/game/ui/starfield\.cpp",
        r"0x7800|0x5000|0xEFFF|0x9FFF",
        "the screen centre and size in Q8 (the stars' box)",
    ),
    (
        "screen",
        r"src/game/ui/company_logos\.cpp",
        r"0x7800|0x5000|\* (120|80)\)",
        "the screen centre (the logos' zoom and fly-in)",
    ),
    ("screen", r"src/engine/gfx/bitmap_screen\.cpp", r".", "the affine picture's centre (BG2X/BG2Y)"),
    ("screen", r"src/game/ui/power_dialog\.cpp", r"<< 7", "the screen centre in Q8 ((240, 160) << 7)"),
    ("screen", r"src/game/vehicles/actor_destructors\.cpp", r"x = 120", "the screen's centre column"),
    (
        "screen",
        r"src/game/ui/level_select_cursor\.cpp|src/game/ui/level_select_pages\.cpp|src/game/ui/zoom_bg\.cpp",
        r"0x78",
        "the screen's centre column (the level select's zoom)",
    ),
    # --- coincidence: the same number for something else --------------
    ("coincidence", r".", r"COMPILE_TIME_ASSERT", "a struct size"),
    ("coincidence", r".", r"(?i)timer|period|time\b|time =|stateTime|RandRange|wait", "a frame count (timer, period)"),
    (
        "coincidence",
        r".",
        r"(?i)depth|distance|\bz\b|\bz =|Checkpoint|sz \+|ActorSelf\(|HpActor\(|gActorFarClipDepth",
        "a depth or distance in the 3D (vehicle) levels",
    ),
    (
        "coincidence",
        r"src/game/bosses/(cortex|hovercraft)\.cpp|src/game/vehicles/jetpack/jetpack_player_update\.cpp",
        r"\by\b|CLAMP_M",
        "a position in the boss arena or the jetpack course",
    ),
    (
        "coincidence",
        r"src/game/vehicles/jetpack/airship\.cpp",
        r"screen[XY] [<>]",
        "the airship's projected position, bounded in the arena (Steer's edge push)",
    ),
    ("coincidence", r".", r"affine =|Q8_MUL\(scale", "an affine scale"),
    ("coincidence", r".", r"(?i)vol|Sfx|LIMIT_MAX\(d, 0xa0\)", "a sound volume or its fall-off distance"),
    ("coincidence", r".", r"accY|tornadoTurn|y -= 0x100|[xy] [+-]= 0x100", "a speed or an angle"),
    ("coincidence", r".", r"DmaCopy16\(3, src|src \+= 0xa0", "a DMA length (tile rows)"),
    (
        "coincidence",
        r".",
        r"\bhp\b|0x64, 0x78|SetCellAnimSpeed|CellAnim::SetSpeed|SetAttackCycle",
        "a game value (health, a cycle, an orbit radius)",
    ),
    ("coincidence", r".", r"SpawnPowerTurboRun", "a world position (a spawn point)"),
]
CLASSES = [(c, re.compile(p), re.compile(l), r) for c, p, l, r in CLASSES]

# The layout sites' classification (kind `layout`), the same way. A layout
# over the 240x160 art (a menu, the title screen) is anchored to the art's
# origin, DISPLAY_ART_X/Y; one over the game's view (the HUD) to the
# screen's edges, DISPLAY_RIGHT/BOTTOM, or its centre (docs/screen.md,
# "Layouts").
LAYOUT_CLASSES = [
    (
        "screen",
        r"src/game/ui/hud|src/game/ui/data/hud_font_tables\.cpp",
        r".",
        "a HUD position: anchor to the screen's edges or centre (DISPLAY_RIGHT/BOTTOM, DISPLAY_CENTER_X)",
    ),
    (
        "screen",
        r"src/(?:[a-z_]+/)+data/(universal_vv_logo_motions|title_screen_gfx)\.cpp",
        r".",
        "a logo piece's position on the 240x160 logo art (DISPLAY_ART_X/Y)",
    ),
    ("screen", r".", r".", "a layout on the 240x160 art: anchor to DISPLAY_ART_X/Y"),
]
LAYOUT_CLASSES = [(c, re.compile(p), re.compile(l), r) for c, p, l, r in LAYOUT_CLASSES]


def classify(rel, source, kind=None):
    for cat, path_re, line_re, reason in LAYOUT_CLASSES if kind == "layout" else CLASSES:
        if path_re.search(rel) and line_re.search(source):
            return cat, reason
    return "unclassified", ""


def operand_before(line, i):
    """The operand that ends just before offset `i` of `line` (back to an
    unmatched `(`, `=`, `,`, `;` or the start)."""
    depth = 0
    j = i - 1
    while j >= 0:
        c = line[j]
        if c == ")":
            depth += 1
        elif c == "(":
            if depth == 0:
                break
            depth -= 1
        elif depth == 0 and c in ",;=?:{" and not (c == "=" and j > 0 and line[j - 1] in "<>!="):
            break
        j -= 1
    return line[j + 1 : i]


def operand_after(line, i):
    depth = 0
    j = i
    while j < len(line):
        c = line[j]
        if c == "(":
            depth += 1
        elif c == ")":
            if depth == 0:
                break
            depth -= 1
        elif depth == 0 and c in ",;?:}&|":
            break
        j += 1
    return line[i:j]


def candidate(value, line, start, end):
    """The kind of the literal at line[start:end], or None."""
    if ANCHORED.search(line[:start]):
        return None
    if value in VALUES:
        return VALUES[value]
    if value in CONTEXT:
        kind, ctx = CONTEXT[value]
        return kind if ctx.search(line) else None
    before = line[:start].rstrip()
    if value in MASKS and re.search(r"(?<![&])&=?$", before):
        if COORD.search(operand_before(line, len(before) - (1 if before.endswith("&") else 2))):
            return "mask"
    if value in WRAPS:
        m = re.search(r"(<=|>=|<|>|\+|-|%|\+=|-=)$", before)
        if m and not before.endswith("->"):
            if COORD.search(operand_before(line, len(before) - len(m.group(1)))):
                return "wrap"
        m = re.match(r"\s*(<=|>=|<|>|\+|-)", line[end:])
        if m and COORD.search(operand_after(line, end + m.end())):
            return "wrap"
    return None


def function_at(scopes, offset):
    best = None
    for s in scopes:
        if s.kind == "func" and s.start <= offset <= s.end:
            if best is None or s.start > best.start:
                best = s
    if best is None:
        return None
    return f"{best.name}::{best.fname}" if best.name else best.fname


def scan_file(path, rel):
    raw = open(path, encoding="utf-8", errors="replace").read()
    text = mn.strip_comments(raw)
    lines = text.split("\n")
    raw_lines = raw.split("\n")
    sc = None
    out = []
    offset = 0
    for n, line in enumerate(lines, 1):
        stripped = line.lstrip()
        guard = stripped.startswith("#") and re.match(r"#\s*(include|ifn?def|endif|pragma)\b", stripped)
        if not guard:
            layout = set(layout_sites(rel, line))
            for m in LITERAL.finditer(line):
                value = mn.parse_value(m.group(0))
                kind = "layout" if m.start() in layout else candidate(value, line, m.start(), m.end())
                if kind is None:
                    continue
                if sc is None:
                    sc = mn.scopes(text)
                cat, reason = classify(rel, line, kind)
                out.append(
                    Site(
                        rel,
                        n,
                        kind,
                        value,
                        m.group(0),
                        cat,
                        reason,
                        function_at(sc, offset + m.start()),
                        raw_lines[n - 1].strip(),
                    )
                )
        offset += len(line) + 1
    return out


def source_files():
    for path in mn.source_files():
        yield path, os.path.relpath(path, ROOT)


def scan(paths=None):
    sites = []
    for path, rel in source_files():
        if paths and not any(rel == p or rel.startswith(p.rstrip("/") + "/") for p in paths):
            continue
        sites += scan_file(path, rel)
    return sites


def report(sites, out):
    w = out.write
    w("# Screen-size literals (#979)\n\n")
    cats = collections.Counter(s.category for s in sites)
    w(f"{len(sites)} sites: " + ", ".join(f"{c} {cats[c]}" for c in CATEGORIES if cats[c]) + "\n\n")
    kinds = [k for k in KINDS if any(s.kind == k for s in sites)]
    w("| kind | " + " | ".join(CATEGORIES) + " | total |\n|---|" + "---:|" * (len(CATEGORIES) + 1) + "\n")
    for k in kinds:
        c = collections.Counter(s.category for s in sites if s.kind == k)
        w(f"| {k} | " + " | ".join(str(c[x]) for x in CATEGORIES) + f" | {sum(c.values())} |\n")
    w("\n| directory | " + " | ".join(CATEGORIES) + " | total |\n|---|" + "---:|" * (len(CATEGORIES) + 1) + "\n")
    dirs = collections.defaultdict(collections.Counter)
    for s in sites:
        dirs[mn.directory(s.file)][s.category] += 1
    for d in sorted(dirs):
        c = dirs[d]
        w(f"| {d} | " + " | ".join(str(c[x]) for x in CATEGORIES) + f" | {sum(c.values())} |\n")
    for cat in CATEGORIES:
        these = [s for s in sites if s.category == cat]
        if not these:
            continue
        w(f"\n## {cat} ({len(these)})\n\n")
        by = collections.defaultdict(list)
        for s in these:
            by[s.reason].append(s)
        for reason in sorted(by, key=lambda r: (-len(by[r]), r)):
            if reason:
                w(f"**{reason}** ({len(by[reason])})\n\n")
            for s in by[reason]:
                fn = f" ({s.func})" if s.func else ""
                w(f"- `{s.file}:{s.line}`{fn} {s.kind} `{s.literal}`: `{s.source}`\n")
            w("\n")


def run(args):
    sites = scan(args.path)
    if getattr(args, "check", False):
        new = [s for s in sites if s.category == "unclassified"]
        for s in new:
            print(f"{s.file}:{s.line}: {s.kind} {s.literal}: not classified\n    {s.source}")
        if new:
            print(
                f"{len(new)} screen-size literal(s) with no rule: spell them with include/screen.h, "
                "or add a CLASSES rule (tools/magic_screen.py) saying why they stay (docs/screen.md)",
                file=sys.stderr,
            )
            return 1
        print(f"ok: {len(sites)} screen-size literals, all classified")
        return 0
    if getattr(args, "category", None):
        sites = [s for s in sites if s.category in args.category]
    if args.json:
        json.dump([s._asdict() for s in sites], sys.stdout, indent=1)
        sys.stdout.write("\n")
        return 0
    if args.report:
        if args.report == "-":
            report(sites, sys.stdout)
        else:
            with open(args.report, "w") as f:
                report(sites, f)
        return 0
    for s in sites:
        print(
            f"{s.file}:{s.line}: {s.kind} {s.literal} {s.category}"
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
    ap.add_argument("--path", action="append")
    ap.add_argument("--category", action="append", choices=CATEGORIES)
    ap.add_argument("--check", action="store_true")
    sys.exit(run(ap.parse_args()))
