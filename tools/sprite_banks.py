#!/usr/bin/env python3
"""Extracts the sprite-bank animation tables (gSpriteBankTable,
0x084A5600-0x084C0006) from baserom.gba as typed C, the src/.../data/
sprite_banks_*.cpp files (see docs/data.md and docs/data_map.md).

This is a one-time extraction: the C it writes is
the editable source, and the build compiles it like any other data file.
Rerunning it overwrites those files with the ROM's tables.

The layout, from the matched readers (include/engine/gfx/sprite_bank.h):

    header (struct sprite_bank_table)
    struct sprite_bank[56]
    per bank, back to back:
        struct sprite_anim[animCount]
        u16 seq[] per animation (frame indices)
        const struct sprite_frame *frames[]
        the frames: a 12-byte struct sprite_frame and the boxes/anchor its
            layout type (the high nibble of its first piece byte) adds
        struct sprite_piece_pos[count] per frame
        u8 pieces[count] per frame

Everything between those is zero alignment padding, which agbcc reproduces
from the types alone. A frame with no pieces shares the next frame's
arrays, and its 12-byte header is directly followed by the next frame.

The piece arrays (positions and piece bytes) and each frame's tile offset
are not written here: tools/sprite_frames.py generates them from
graphics/sprites/bankNN_<name>/bank.json (`sprites/bankNN_pieces.inc`,
`sprites/sprite_frames.h`), which `sprite_frames.py extract` writes from
the same ROM. This script still checks that they sit where the ROM has them.

    tools/sprite_banks.py [baserom.gba]
"""
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from sprite_frames import BANK_NAMES, NEW_FRAME_HOOKS, Rom  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
TABLE = 0x084A5600
END = 0x084C0006
TILE_BASE = 0x082BF120
OBJ_PALETTES = 0x084A4660

# The banks each C file holds. A file must end 4-aligned (the next one
# starts with a word-aligned struct and tools/report_units.py doesn't allow
# linker fill), so the cuts are after banks whose piece bytes end on a word.
FILES = [(0, 9), (10, 21), (22, 38), (39, 55)]

# layout type -> (struct, number of boxes, has anchor)
LAYOUTS = {
    0: ("sprite_frame_3box_anchor", 3, True),
    1: ("sprite_frame", 0, False),
    2: ("sprite_frame_1box", 1, False),
    3: ("sprite_frame_2box", 2, False),
    4: ("sprite_frame_3box", 3, False),
    5: ("sprite_frame_1box", 1, False),
    6: ("sprite_frame_1box_anchor", 1, True),
}


def frames_bound(i, n):
    """The bound of bank i's frame table: its n frames, and for a bank of
    NEW_FRAME_HOOKS the new frames sprite_frames.py pack adds."""
    return f"{n} + SPRITE_BANK{i:02}_NEW_FRAME_COUNT" if i in NEW_FRAME_HOOKS else f"{n}"


def seq_bound(i, k, n):
    """The bound of animation k's sequence of bank i: likewise its new steps."""
    return f"{n} + SPRITE_BANK{i:02}_ANIM{k:02}_NEW_STEP_COUNT" if k in NEW_FRAME_HOOKS.get(i, []) else f"{n}"


def frame_size(layout):
    _, boxes, anchor = LAYOUTS[layout]
    return 12 + 8 * boxes + 4 * anchor


class Emitter:
    """Checks that the objects are emitted exactly where the ROM has them,
    with only zero padding in between that agbcc's alignment recreates."""

    def __init__(self, rom, start):
        self.rom = rom
        self.pos = start

    def place(self, addr, size, align):
        pad = (-self.pos) % align
        assert addr == self.pos + pad, f"{addr:#x}: expected {self.pos + pad:#x}"
        assert not any(self.rom.bytes(self.pos, pad)), f"non-zero padding at {self.pos:#x}"
        self.pos = addr + size


def box(rom, a):
    x, y, w, h, unk = struct.unpack_from("<hhBBH", rom.data, a - 0x08000000)
    assert unk == 0, f"{a:#x}: box padding {unk:#x}"
    return f"{{ {x}, {y}, {w}, {h} }}"


def point(rom, a):
    x, y = struct.unpack_from("<hh", rom.data, a - 0x08000000)
    return f"{{ {x}, {y} }}"


def read_bank(rom, i):
    base = rom.w(TABLE) + 12 * i
    anims, frames, unk, nanims = rom.w(base), rom.w(base + 4), rom.h(base + 8), rom.h(base + 10)
    assert unk == 0
    ptrs, a, first = [], frames, None
    while a != first:
        p = rom.w(a)
        first = p if first is None else min(first, p)
        ptrs.append(p)
        a += 4
    assert ptrs == sorted(ptrs) and len(set(ptrs)) == len(ptrs)
    return anims, frames, nanims, ptrs


def emit_bank(rom, i, em, out, decls):
    anims, frames, nanims, ptrs = read_bank(rom, i)
    B = f"gSpriteBank{i:02}"
    nframes = len(ptrs)

    # Frames: layout from the first piece byte, but never past the next
    # frame (a frame with no pieces is just its 12-byte header).
    fr = []
    for k, p in enumerate(ptrs):
        pos, pieces, packed = rom.w(p), rom.w(p + 4), rom.w(p + 8)
        count = packed >> 24
        layout = rom.b(pieces) >> 4
        size = frame_size(layout)
        nxt = ptrs[k + 1] if k + 1 < nframes else None
        if count == 0:
            assert nxt == p + 12, f"{p:#x}: empty frame not followed by a frame"
            layout, size = 1, 12
        assert nxt is None or nxt == p + size, f"{p:#x}: size {size} but next at {nxt:#x}"
        fr.append(dict(addr=p, pos=pos, pieces=pieces, count=count, off=packed & 0xFFFFFF,
                       layout=layout, name=f"{B}Frame{k:03}"))
    owner_pos = {f["pos"]: f for f in fr if f["count"]}
    owner_pieces = {f["pieces"]: f for f in fr if f["count"]}
    for f in fr:
        if f["count"] == 0:
            f["shares"] = owner_pos[f["pos"]]
            assert owner_pieces[f["pieces"]] is f["shares"]

    # Animations and their sequences.
    an = []
    for k in range(nanims):
        a = anims + 0x1C * k
        seq, count = rom.w(a), rom.b(a + 0x16)
        palette_id, duration, flags = rom.b(a + 0x14), rom.b(a + 0x15), rom.b(a + 0x17)
        assert rom.w(a + 0x18) == 0 and flags & ~2 == 0
        values = [rom.h(seq + 2 * j) for j in range(count)]
        assert all(v < nframes for v in values)
        an.append(dict(seq=seq, count=count, values=values, boxes=(box(rom, a + 4), box(rom, a + 12)),
                       palette_id=palette_id, duration=duration, flags=flags,
                       name=f"{B}Anim{k:02}Seq", bound=seq_bound(i, k, count)))
    assert len({x["seq"] for x in an}) == nanims

    d = f"bank{i:02}_{BANK_NAMES[i]}"
    out.append(f"/* {'-' * 70} */")
    head = (f"/* Bank {i}: {nanims} animation{'s' if nanims != 1 else ''}, {nframes} frames,"
            f" graphics/sprites/{d}/.")
    if i in NEW_FRAME_HOOKS:
        which = " and ".join(str(k) for k in NEW_FRAME_HOOKS[i])
        out.append(head)
        out.append(f" * The frame table and animation {which}'s sequence end with the new frames and")
        out.append(" * steps of bank.json (\"boxes\", \"new_steps\"; tools/sprite_frames.py,")
        out.append(" * NEW_FRAME_HOOKS; docs/graphics.md \"New frames\"), none in the ROM. */")
    else:
        out.append(head + " */")
    out.append("")

    # Forward declarations (everything below is referenced before it is
    # defined, the ROM's order). The frames and their piece arrays are
    # declared in sprites/sprite_frames.h.
    decls.append(f"extern const struct sprite_anim {B}Anims[{nanims}];")
    decls.append(f"extern const struct sprite_frame *const {B}Frames[{frames_bound(i, nframes)}];")
    for x in sorted(an, key=lambda x: x["seq"]):
        out.append(f"extern const u16 {x['name']}[{x['bound']}];")
    out.append("")

    em.place(anims, 0x1C * nanims, 4)
    # The first file declares every bank's Anims and Frames up front; in
    # the others they are defined with `extern`, which C++ needs for a
    # const object with no earlier declaration to keep external linkage.
    ext = "extern " if i >= FILES[0][1] + 1 else ""
    out.append(f"{ext}const struct sprite_anim {B}Anims[{nanims}] = {{")
    for k, x in enumerate(an):
        flags = "SPRITE_ANIM_LOOP" if x["flags"] else "0"
        out.append(f"    /* {k} */ {{")
        out.append(f"        /* seq */ {x['name']},")
        out.append(f"        /* box */ {{ {x['boxes'][0]}, {x['boxes'][1]} }},")
        out.append(f"        /* paletteId */ {x['palette_id']},")
        out.append(f"        /* duration */ {x['duration']},")
        out.append(f"        /* frameCount */ ARRAY_COUNT({x['name']}),")
        out.append(f"        /* flags */ {flags},")
        out.append("    },")
    out.append("};")
    out.append("")

    for x in sorted(an, key=lambda x: x["seq"]):
        em.place(x["seq"], 2 * x["count"], 2)
        out.append(f"const u16 {x['name']}[{x['bound']}] = {{")
        vals = [str(v) for v in x["values"]]
        rows = ["    " + ", ".join(vals[j:j + 16]) + "," for j in range(0, len(vals), 16)]
        if x["bound"] != str(x["count"]):
            # the new steps (sprite_frames.py's _NEW_STEPS)
            rows[-1] += " " + x["bound"].split(" + ")[1].replace("_NEW_STEP_COUNT", "_NEW_STEPS")
        out += rows
        out.append("};")
    out.append("")

    em.place(frames, 4 * nframes, 4)
    out.append(f"{ext}const struct sprite_frame *const {B}Frames[{frames_bound(i, nframes)}] = {{")
    for f in fr:
        ref = f"&{f['name']}" if f["layout"] == 1 else f"&{f['name']}.frame"
        out.append(f"    {ref},")
    if i in NEW_FRAME_HOOKS:
        out.append(f"    SPRITE_BANK{i:02}_NEW_FRAMES")
    out.append("};")
    out.append("")

    for k, f in enumerate(fr):
        em.place(f["addr"], frame_size(f["layout"]), 4)
        tiles = f"SPRITE_TILES_BANK{i:02}_FRAME{k:03}"
        struct_name, nbox, anchor = LAYOUTS[f["layout"]]
        if f["count"]:
            head = f"SPRITE_FRAME({f['name']}, {tiles})"
        else:
            s = f["shares"]["name"]
            head = f"{{ {s}Pos, {s}Pieces, SPRITE_FRAME_TILES({tiles}, 0) }}"
        if nbox == 0:
            out.append(f"const struct {struct_name} {f['name']} = {head};")
            continue
        out.append(f"const struct {struct_name} {f['name']} = {{")
        out.append(f"    {head},")
        boxes = ", ".join(box(rom, f["addr"] + 12 + 8 * b) for b in range(nbox))
        out.append(f"    {{ {boxes} }},")
        if anchor:
            out.append(f"    {point(rom, f['addr'] + 12 + 8 * nbox)},")
        out.append("};")
    out.append("")

    # The piece arrays: every frame's positions, then every frame's piece
    # bytes (tools/sprite_frames.py pack).
    for f in fr:
        if f["count"]:
            em.place(f["pos"], 4 * f["count"], 4)
    for f in fr:
        if f["count"]:
            em.place(f["pieces"], f["count"], 1)
    out.append(f"/* The piece arrays, from graphics/sprites/{d}/bank.json. */")
    out.append(f'#include "sprites/bank{i:02}_pieces.inc"')
    out.append("")


FILE_COMMENT = """\
/*
 * ROM {start:#010x}-{end:#010x}: sprite banks {first}-{last} of the sprite-bank
 * animation system (gSpriteBankTable, include/engine/gfx/sprite_bank.h). Linked in
 * ROM order between data/data.s sections by games/xs/ldscript.txt - see
 * docs/data.md and docs/data_map.md ("gSpriteBankTable").
 *
 * Extracted once from baserom.gba by tools/sprite_banks.py; this file is
 * the source now. Per bank: the animations (a frame-index sequence each),
 * the frame pointer array, the frames (header plus the boxes/anchor of
 * their layout type), then every frame's piece positions and piece bytes.
 * The pieces and the frames' tiles come from graphics/sprites/bankNN_<name>/
 * (a PNG per frame and bank.json, see docs/graphics.md "Sprite banks"):
 * tools/sprite_frames.py generates the piece arrays (sprites/bankNN_pieces.inc),
 * and sprites/sprite_frames.h declares the frames and gives each one's
 * offset in the sprite tile pool (src/engine/gfx/data/sprite_tiles.cpp),
 * SPRITE_TILES_BANKnn_FRAMEkkk. A frame's struct type has to match its
 * "layout" in bank.json.
 */
"""


def main():
    rom = Rom(sys.argv[1] if len(sys.argv) > 1 else ROOT / "baserom.gba")
    assert rom.w(TABLE + 4) == TILE_BASE and rom.w(TABLE + 8) == OBJ_PALETTES
    assert rom.w(TABLE) == TABLE + 16 and rom.h(TABLE + 12) == 56 and rom.h(TABLE + 14) == 125
    nbanks = 56
    starts = [read_bank(rom, i)[0] for i in range(nbanks)]

    em = Emitter(rom, TABLE)
    for fi, (lo, hi) in enumerate(FILES):
        start = TABLE if lo == 0 else starts[lo]
        end = starts[hi + 1] if hi + 1 < nbanks else END
        name = f"sprite_banks_{lo:02}_{hi:02}.cpp"
        out, decls = [], []
        body = []
        if lo == 0:
            em.place(TABLE, 16, 4)
            em.place(TABLE + 16, 12 * nbanks, 4)
        for i in range(lo, hi + 1):
            emit_bank(rom, i, em, body, decls)
        assert em.pos == end, f"{name}: ends at {em.pos:#x}, expected {end:#x}"
        if hi + 1 < nbanks:
            assert end % 4 == 0

        out.append('extern "C" {')
        out.append('#include "gba/types.h"')
        out.append('#include "engine/gfx/sprite_bank.h"')
        if lo == 0:
            out.append('#include "globals.h"')
        out.append("}")
        out.append('#include "sprites/sprite_frames.h"')
        out.append("")
        out.append(FILE_COMMENT.format(start=start, end=end, first=lo, last=hi))
        if lo == 0:
            out.append("extern const u8 gSpriteBank00Tiles[];  /* sprite_tiles.cpp, bank 0's tiles */")
            out.append("extern const u8 gObjPalettes[0xfa0];")
            out.append("extern const struct sprite_bank gSpriteBanks[56];")
            for i in range(nbanks):
                if not (lo <= i <= hi):
                    b = f"gSpriteBank{i:02}"
                    _, _, nanims, ptrs = read_bank(rom, i)
                    out.append(f"extern const struct sprite_anim {b}Anims[{nanims}];")
                    out.append(f"extern const struct sprite_frame *const {b}Frames[{frames_bound(i, len(ptrs))}];")
            out.extend(decls)
            out.append("")
            out.append("/* The root of the system: InitLevelState (spawn_markers.cpp) points")
            out.append(" * *gSpriteBankSet here. GetSpriteTileBase returns tileBase;")
            out.append(" * InitLevelState and RunPauseMenu (pause_menu.cpp) build the palette")
            out.append(" * cache from palettes/paletteCount. */")
            out.append("extern const struct sprite_bank_table gSpriteBankTable = {")
            out.append("    /* banks */ gSpriteBanks,")
            out.append("    /* tileBase */ gSpriteBank00Tiles,")
            out.append("    /* palettes */ gObjPalettes,")
            out.append("    /* bankCount */ ARRAY_COUNT(gSpriteBanks),")
            out.append("    /* paletteCount */ sizeof(gObjPalettes) / 32,")
            out.append("};")
            out.append("")
            out.append("/* Bank N is `**gSpriteBankSet + 12 * N` in the code (a part's +0x20). */")
            out.append("const struct sprite_bank gSpriteBanks[56] = {")
            for i in range(nbanks):
                b = f"gSpriteBank{i:02}"
                out.append(f"    /* {i} */ {{ {b}Anims, {b}Frames, 0, ARRAY_COUNT({b}Anims) }},")
            out.append("};")
            out.append("")
        out.extend(body)
        while out and out[-1] == "":
            out.pop()
        (ROOT / "src" / "engine" / "gfx" / "data" / name).write_text("\n".join(out) + "\n")
        print(f"wrote src/engine/gfx/data/{name}: {start:#x}-{end:#x}")
    assert em.pos == END


if __name__ == "__main__":
    main()
