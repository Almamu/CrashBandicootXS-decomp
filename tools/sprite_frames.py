#!/usr/bin/env python3
"""The sprite banks' frames (gSpriteBankTable's tile pool, 0x082BF120-
0x084A4660) as one PNG per frame, see docs/graphics.md "Sprite banks".

A frame of a sprite bank is a list of OBJ pieces (a shape/size and an x, y
offset from the sprite's position) and one run of 4bpp tiles in the pool,
each piece's tiles back to back in 1D OBJ order. The sources are, per
bank, graphics/sprites/bankNN_<name>/:

- fKKK.png: frame KKK, the pieces drawn where they go, as a 4bpp indexed
  PNG cropped to the pieces' bounding box, in the palette of the first
  animation that shows the frame (gObjPalettes[paletteId]). The palette
  is only for viewing: the build reads the indices.
- bank.json: per frame, in frame order:
    "origin": [x, y]   the sprite's position in the PNG
    "pieces": [["WxH", x, y], ...]   each piece's size in pixels and its
                       offset from the sprite's position, in piece order
    "layout": n        the frame's layout type (the high nibble of its first
                       piece byte, include/sprite_bank.h); it says which
                       struct the frame is in the C
    "tiles":           where the frame's tile run starts in the pool:
        "own"              at the end of the tiles placed so far (new tiles)
        {"frame": K, "tile": t}            at tile t of frame K's run (an
                           earlier frame of the bank); the run may go on
                           past the end, the rest being new tiles
        {"bank": B, "frame": K, "tile": t} the same in an earlier bank
    "palettes": [id, ...]  the paletteIds of the animations that show the
                       frame, in animation order; the PNG uses the first.
  A frame with no pieces (3 in the ROM) has no PNG; its C header reuses
  the next frame's arrays with a piece count of 0.

    tools/sprite_frames.py extract [baserom.gba]
        Writes graphics/sprites/bankNN_<name>/ from the ROM and checks
        that packing them gives the ROM's pool and frame offsets.

    tools/sprite_frames.py pack OUTDIR
        Builds, from graphics/sprites/bankNN_<name>/:
        OUTDIR/bankNN_tiles.inc   the bank's tile bytes (C initializer)
        OUTDIR/bankNN_pieces.inc  the bank's piece arrays (...FrameKKKPos,
                                  ...FrameKKKPieces), in the ROM's order
        OUTDIR/sprite_frames.h    SPRITE_TILES_BANKnn (the bank's offset in
                                  the pool), SPRITE_TILES_BANKnn_SIZE,
                                  SPRITE_TILES_BANKnn_FRAMEkkk (each frame's
                                  tile offset), and the extern declarations
                                  of the frames and their piece arrays

pack cuts each piece's rectangle out of the frame's PNG into tiles and
places the frames' runs in frame order. Tiles a frame shares with an
earlier frame must be the same pixels in both PNGs, or pack fails and
names the frame and the tile. So does a pixel drawn outside every piece.
"""
import json
import struct
import sys
from pathlib import Path

from PIL import Image

ROM_BASE = 0x08000000
BANK_TABLE = 0x084A5600
TILE_BASE = 0x082BF120
OBJ_PALETTES = 0x084A4660
OBJ_W = 0x0816B2E0  # gObjPieceWidths
OBJ_H = 0x0816B2EC  # gObjPieceHeights
SPRITES = Path("graphics/sprites")

# shape/size index (the low nibble of a piece byte) -> (width, height)
SHAPES = [(8, 8), (16, 16), (32, 32), (64, 64), (16, 8), (32, 8), (32, 16), (64, 32),
          (8, 16), (8, 32), (16, 32), (32, 64)]
SHAPE_NAMES = {f"{w}x{h}": i for i, (w, h) in enumerate(SHAPES)}

# layout type -> the frame's struct (include/sprite_bank.h)
LAYOUT_STRUCTS = {
    0: "sprite_frame_3box_anchor",
    1: "sprite_frame",
    2: "sprite_frame_1box",
    3: "sprite_frame_2box",
    4: "sprite_frame_3box",
    5: "sprite_frame_1box",
    6: "sprite_frame_1box_anchor",
}

# The banks' names: what their frames show, and the code that uses them
# (gSpriteBankSet->Bank(n)).
BANK_NAMES = [
    "crash",                       # 0: play_room.cpp, ROOM_KIND_ON_FOOT
    "crash_underwater",            # 1: ROOM_KIND_UNDERWATER (SwimCtrl)
    "crash_hover",                 # 2: ROOM_KIND_HOVER
    "moray_eel",                   # 3: SpawnMorayEel
    "shark",                       # 4: SpawnShark, Dingodile
    "pufferfish",                  # 5: SpawnPufferfish
    "sea_mine",                    # 6: SpawnSeaMine
    "squid",                       # 7: SpawnSquid
    "electric_eel",                # 8: SpawnElectricEel
    "jellyfish",                   # 9: SpawnJellyfish
    "venus_flytrap",               # 10: SpawnVenusFlytrap
    "vulture",                     # 11: SpawnVulture
    "blowgun_tribesman",           # 12: SpawnBlowgunTribesman
    "lizard",                      # 13: SpawnLizard
    "patrolling_jungle_enemy",     # 14: SpawnPatrollingJungleEnemy
    "penguin",                     # 15: SpawnPenguin
    "polar_bear",                  # 16: SpawnPolarBear
    "seal",                        # 17: SpawnSeal
    "wooden_crusher",              # 18: SpawnWoodenCrusher
    "frog",                        # 19: SpawnFrog
    "patrolling_sewer_enemy",      # 20: SpawnPatrollingSewerEnemy
    "rat",                         # 21: SpawnRat
    "homing_sewer_enemy",          # 22: SpawnHomingSewerEnemy
    "flamethrower_lab_assistant",  # 23: SpawnFlamethrowerLabAssistant
    "patrolling_space_enemy",      # 24: SpawnPatrollingSpaceEnemy
    "laser_barrier",               # 25: SpawnLaserBarrier
    "piston_crusher",              # 26: SpawnPistonCrusher
    "stationary_space_enemy",      # 27: SpawnStationarySpaceEnemy
    "launch_pad",                  # 28: launch_pad.cpp
    "saucer_lab_assistant",        # 29: SpawnSaucerLabAssistant
    "mega_mix",                    # 30: SpawnMegaMix
    "crates",                      # 31: crate_create.cpp
    "gems",                        # 32: spawn_gems.cpp, level_select.cpp
    "trial_icons",                 # 33: level_select.cpp (time trial)
    "aku_aku",                     # 34: Player's child sprite
    "wumpa",                       # 35: SpawnWumpa, the HUD
    "stopwatch",                   # 36: SpawnStopwatch
    "crystal",                     # 37: SpawnCrystal, level_select.cpp
    "powers",                      # 38: the four power pickups
    "platforms",                   # 39: CreatePlatform
    "bubbles",                     # 40: swim_ctrl_stroke.cpp effect
    "spark",                       # 41: action_ctrl_states.cpp effect
    "checkpoint_text",             # 42: "CHECKPOINT", Crate::OpenCheckpoint
    "sparkle",                     # 43: pickup and gem collect effects
    "flame",                       # 44: SpawnFlame
    "seaweed",                     # 45: SpawnSeaweed
    "crash_eyes",                  # 46: Crash's eyes, open and shut (no user found)
    "icons",                       # 47: the HUD, ExtraLife, level_select.cpp
    "level_select_cursor",         # 48: level_select_cursor.cpp
    "level_select_frame",          # 49: LevelSelectEntry's frame
    "twinkles",                    # 50: zoom_bg.cpp
    "level_select_icons",          # 51: LevelSelectEntry's icon
    "level_select_arrows",         # 52: level_select.cpp world arrows
    "cortex",                      # 53: Cortex
    "dingodile",                   # 54: Dingodile
    "tiny",                        # 55: Tiny
]


def fail(msg):
    sys.exit(f"sprite_frames: {msg}")


def tile_count(shape):
    w, h = SHAPES[shape]
    return w * h // 64


def run_tiles(pieces):
    return sum(tile_count(s) for s, _, _ in pieces)


# ---------------------------------------------------------------------------
# The ROM's tables


class Rom:
    def __init__(self, path):
        self.data = Path(path).read_bytes()

    def w(self, a):
        return struct.unpack_from("<I", self.data, a - ROM_BASE)[0]

    def h(self, a):
        return struct.unpack_from("<H", self.data, a - ROM_BASE)[0]

    def s(self, a):
        return struct.unpack_from("<h", self.data, a - ROM_BASE)[0]

    def b(self, a):
        return self.data[a - ROM_BASE]

    def bytes(self, a, n):
        return self.data[a - ROM_BASE:a - ROM_BASE + n]


def read_banks(rom):
    """[{anims: [(paletteId, seq)], frames: [{off, pieces, layout}]}]"""
    banks_p, tile_base = rom.w(BANK_TABLE), rom.w(BANK_TABLE + 4)
    assert tile_base == TILE_BASE and rom.w(BANK_TABLE + 8) == OBJ_PALETTES
    assert list(zip(rom.bytes(OBJ_W, 12), rom.bytes(OBJ_H, 12))) == SHAPES
    out = []
    for i in range(rom.h(BANK_TABLE + 12)):
        anims_p, frames_p, nanims = rom.w(banks_p + 12 * i), rom.w(banks_p + 12 * i + 4), rom.h(banks_p + 12 * i + 10)
        anims = []
        for k in range(nanims):
            a = anims_p + 0x1C * k
            seq = [rom.h(rom.w(a) + 2 * j) for j in range(rom.b(a + 0x16))]
            anims.append((rom.b(a + 0x14), seq))
        # frame_desc *frames[], directly followed by the first frame
        ptrs, a, first = [], frames_p, None
        while a != first:
            p = rom.w(a)
            first = p if first is None else min(first, p)
            ptrs.append(p)
            a += 4
        frames = []
        for p in ptrs:
            pos, ids, packed = rom.w(p), rom.w(p + 4), rom.w(p + 8)
            count = packed >> 24
            pieces = []
            for k in range(count):
                byte = rom.b(ids + k)
                assert k == 0 or byte >> 4 == 0, f"{p:#x}: piece {k} has a layout nibble"
                pieces.append((byte & 15, rom.s(pos + 4 * k), rom.s(pos + 4 * k + 2)))
            frames.append(dict(off=packed & 0xFFFFFF, pieces=pieces,
                               layout=rom.b(ids) >> 4 if count else None))
        out.append(dict(anims=anims, frames=frames))
    return out


def frame_palettes(bank):
    pals = [[] for _ in bank["frames"]]
    for pal, seq in bank["anims"]:
        for k in seq:
            if pal not in pals[k]:
                pals[k].append(pal)
    return pals


def tile_sources(banks):
    """Each frame's tile source ("own" or a reference), and each bank's
    pool range [start, end), from the frames' ROM offsets."""
    runs = []  # per bank: [(start, end) per frame, None for an empty one]
    ranges = []
    start = 0
    for i, bank in enumerate(banks):
        bank_runs, end = [], start
        for k, f in enumerate(bank["frames"]):
            o = f["off"]
            e = o + 32 * run_tiles(f["pieces"])
            f["tiles"] = None
            if o == end:
                f["tiles"] = "own"
            else:
                # the earliest frame whose run starts there, else the
                # earliest whose run holds it; this bank's frames first
                where = [(i, bank_runs)] if o >= start else list(enumerate(runs))
                for b, rr in where:
                    held = [(kk, r) for kk, r in enumerate(rr) if r and r[0] <= o < r[1]]
                    held.sort(key=lambda c: (c[1][0] != o, c[0]))
                    if held:
                        kk, r = held[0]
                        f["tiles"] = {"frame": kk, "tile": (o - r[0]) // 32}
                        if b != i:
                            f["tiles"] = {"bank": b, **f["tiles"]}
                            assert e <= ranges[b][1], f"bank {i} frame {k}: runs out of bank {b}"
                        break
            if f["tiles"] is None:
                fail(f"bank {i} frame {k}: no tile source for {o:#x}")
            if o >= start:
                end = max(end, e)
            bank_runs.append((o, e) if f["pieces"] else None)
        runs.append(bank_runs)
        ranges.append((start, end))
        start = end
    return ranges


# ---------------------------------------------------------------------------
# PNG <-> tiles


def tile_bytes(px, x0, y0):
    """The 32 bytes of the 4bpp tile at (x0, y0) of a pixel accessor."""
    out = bytearray(32)
    for y in range(8):
        for x in range(0, 8, 2):
            out[y * 4 + x // 2] = px[x0 + x, y0 + y] | px[x0 + x + 1, y0 + y] << 4
    return bytes(out)


def bbox(pieces):
    xs = [x for _, x, _ in pieces] + [x + SHAPES[s][0] for s, x, _ in pieces]
    ys = [y for _, _, y in pieces] + [y + SHAPES[s][1] for s, _, y in pieces]
    return min(xs), min(ys), max(xs), max(ys)


def draw_frame(pieces, data, palette):
    """The frame's image from its tile run `data`."""
    x0, y0, x1, y1 = bbox(pieces)
    img = Image.new("P", (x1 - x0, y1 - y0), 0)
    px = img.load()
    drawn = {}
    t = 0
    for s, x, y in pieces:
        w, h = SHAPES[s]
        for ty in range(h // 8):
            for tx in range(w // 8):
                tile = data[t * 32:(t + 1) * 32]
                t += 1
                for i in range(64):
                    v = tile[i // 2] >> 4 * (i & 1) & 15
                    p = (x - x0 + tx * 8 + i % 8, y - y0 + ty * 8 + i // 8)
                    if drawn.setdefault(p, v) != v:
                        raise ValueError(f"overlapping pieces differ at {p}")
                    px[p] = v
    img.putpalette([c for rgb in palette for c in rgb] + [0] * (768 - 3 * len(palette)))
    return img, (-x0, -y0)


def cut_frame(path, origin, pieces):
    """The tile run of a frame's PNG, piece by piece."""
    img = Image.open(path)
    if img.mode != "P":
        fail(f"{path}: not an indexed PNG")
    px = img.load()
    W, H = img.size
    if max(img.tobytes(), default=0) > 15:
        fail(f"{path}: colour index above 15 (a 4bpp frame has 16)")
    covered = set()
    out = bytearray()
    for n, (s, x, y) in enumerate(pieces):
        w, h = SHAPES[s]
        px0, py0 = origin[0] + x, origin[1] + y
        if px0 < 0 or py0 < 0 or px0 + w > W or py0 + h > H:
            fail(f"{path}: piece {n} ({w}x{h} at {x}, {y}) is outside the {W}x{H} image")
        for ty in range(0, h, 8):
            for tx in range(0, w, 8):
                out += tile_bytes(px, px0 + tx, py0 + ty)
        covered.update((xx, yy) for xx in range(px0, px0 + w) for yy in range(py0, py0 + h))
    for yy in range(H):
        for xx in range(W):
            if px[xx, yy] and (xx, yy) not in covered:
                fail(f"{path}: pixel ({xx}, {yy}) is drawn outside every piece")
    return bytes(out)


# ---------------------------------------------------------------------------
# bank.json


def bank_dirs():
    dirs = sorted(d for d in SPRITES.glob("bank[0-9][0-9]_*") if d.is_dir())
    for n, d in enumerate(dirs):
        if int(d.name[4:6]) != n:
            fail(f"{d}: expected bank {n:02}")
    return dirs


def write_json(path, frames):
    lines = []
    for k, f in enumerate(frames):
        e = {}
        if f["pieces"]:
            e["origin"] = list(f["origin"])
            e["pieces"] = [[f"{SHAPES[s][0]}x{SHAPES[s][1]}", x, y] for s, x, y in f["pieces"]]
            e["layout"] = f["layout"]
        else:
            e["pieces"] = []
        e["tiles"] = f["tiles"]
        e["palettes"] = f["palettes"]
        lines.append("    " + json.dumps(e, separators=(", ", ": ")))
    path.write_text('{\n  "frames": [\n' + ",\n".join(lines) + "\n  ]\n}\n")


def read_json(d):
    path = d / "bank.json"
    try:
        frames = json.loads(path.read_text())["frames"]
    except (OSError, ValueError, KeyError) as e:
        fail(f"{path}: {e}")
    out = []
    for k, e in enumerate(frames):
        pieces = []
        for p in e["pieces"]:
            if p[0] not in SHAPE_NAMES:
                fail(f"{path}: frame {k}: no OBJ piece is {p[0]}")
            pieces.append((SHAPE_NAMES[p[0]], int(p[1]), int(p[2])))
        f = dict(pieces=pieces, tiles=e["tiles"], palettes=e.get("palettes", []))
        if pieces:
            if e.get("layout") not in LAYOUT_STRUCTS:
                fail(f"{path}: frame {k}: layout {e.get('layout')!r}")
            f["layout"] = e["layout"]
            f["origin"] = tuple(e["origin"])
        out.append(f)
    return out


# ---------------------------------------------------------------------------
# pack


def pack(dirs):
    """-> per bank: (name, pool bytes, frames with "offset" (from the
    bank's start, or (bank, offset) for a reference into an earlier bank))."""
    banks = []
    for i, d in enumerate(dirs):
        frames = read_json(d)
        pool = bytearray()
        for k, f in enumerate(frames):
            where = f"{d}/bank.json: frame {k}"
            data = cut_frame(d / f"f{k:03}.png", f["origin"], f["pieces"]) if f["pieces"] else b""
            src = f["tiles"]
            if src == "own":
                b, at = i, len(pool)
            elif isinstance(src, dict) and set(src) <= {"bank", "frame", "tile"} and "frame" in src:
                b = src.get("bank", i)
                ref_frames = frames if b == i else banks[b][2] if b < i else None
                if ref_frames is None or not 0 <= src["frame"] < len(ref_frames) \
                        or (b == i and src["frame"] >= k) or not ref_frames[src["frame"]]["pieces"]:
                    fail(f"{where}: tiles refer to a frame that isn't an earlier frame with pieces")
                ref = ref_frames[src["frame"]]
                at = ref["offset"] + 32 * src.get("tile", 0)
                if not 0 <= src.get("tile", 0) < run_tiles(ref["pieces"]):
                    fail(f"{where}: tile {src.get('tile')} is outside frame {src['frame']}'s run")
            else:
                fail(f"{where}: bad tiles {src!r}")
            f["bank"], f["offset"] = b, at
            target = pool if b == i else banks[b][1]
            for t in range(len(data) // 32):
                tile = data[t * 32:(t + 1) * 32]
                q = at + 32 * t
                if q < len(target):
                    if target[q:q + 32] != tile:
                        fail(f"{where}: tile {t} differs from the same tile of the frames sharing it")
                elif b == i:
                    target += tile
                else:
                    fail(f"{where}: runs past the end of bank {b}'s tiles")
        banks.append((d.name, pool, frames))
    return banks


def write_pack(banks, outdir):
    outdir = Path(outdir)
    outdir.mkdir(parents=True, exist_ok=True)
    h = ["/* Generated by tools/sprite_frames.py pack from graphics/sprites/bankNN_<name>/:",
         " * the sprite tile pool's layout and the frames' piece arrays. */",
         ""]
    base = 0
    starts = []
    for i, (name, pool, frames) in enumerate(banks):
        starts.append(base)
        h.append(f"#define SPRITE_TILES_BANK{i:02} {base:#07x}")
        h.append(f"#define SPRITE_TILES_BANK{i:02}_SIZE {len(pool):#x}")
        base += len(pool)
    h.append("")
    for i, (name, pool, frames) in enumerate(banks):
        B = f"gSpriteBank{i:02}"
        h.append(f"/* {name} */")
        for k, f in enumerate(frames):
            off = f["offset"]
            h.append(f"#define SPRITE_TILES_BANK{i:02}_FRAME{k:03} (SPRITE_TILES_BANK{f['bank']:02} + {off:#07x})")
        for k, f in enumerate(frames):
            st = LAYOUT_STRUCTS[f["layout"]] if f["pieces"] else "sprite_frame"
            h.append(f"extern const struct {st} {B}Frame{k:03};")
        pos, ids = [], []
        for k, f in enumerate(frames):
            n = len(f["pieces"])
            if not n:
                continue
            h.append(f"extern const struct sprite_piece_pos {B}Frame{k:03}Pos[{n}];")
            h.append(f"extern const u8 {B}Frame{k:03}Pieces[{n}];")
            pos.append(f"const struct sprite_piece_pos {B}Frame{k:03}Pos[{n}] = {{ "
                       + ", ".join(f"{{ {x}, {y} }}" for _, x, y in f["pieces"]) + " };")
            ids.append(f"const u8 {B}Frame{k:03}Pieces[{n}] = {{ "
                       + ", ".join(f"SPRITE_PIECE({f['layout'] if j == 0 else 0}, {s})"
                                   for j, (s, _, _) in enumerate(f["pieces"])) + " };")
        h.append("")
        write_text(outdir / f"bank{i:02}_pieces.inc",
                         f"/* Generated by tools/sprite_frames.py pack from graphics/sprites/{name}/bank.json */\n"
                         + "\n".join(pos) + "\n\n" + "\n".join(ids) + "\n")
        lines = ["\t" + ", ".join(f"0x{b:02X}" for b in pool[j:j + 16]) + "," for j in range(0, len(pool), 16)]
        write_text(outdir / f"bank{i:02}_tiles.inc",
                         f"/* Generated by tools/sprite_frames.py pack from graphics/sprites/{name}/: "
                         f"{len(pool)} bytes */\n" + "\n".join(lines) + "\n")
    write_text(outdir / "sprite_frames.h", "\n".join(h) + "\n")


def write_text(path, text):
    path.write_text(text)


# ---------------------------------------------------------------------------
# extract


def bgr555(raw):
    out = []
    for i in range(0, len(raw), 2):
        c = raw[i] | raw[i + 1] << 8
        out.append(tuple(((c >> s) & 31) * 255 // 31 for s in (0, 5, 10)))
    return out


def extract(rom_path):
    rom = Rom(rom_path)
    banks = read_banks(rom)
    assert len(banks) == len(BANK_NAMES)
    ranges = tile_sources(banks)
    assert ranges[-1][1] == OBJ_PALETTES - TILE_BASE
    for i, bank in enumerate(banks):
        d = SPRITES / f"bank{i:02}_{BANK_NAMES[i]}"
        d.mkdir(parents=True, exist_ok=True)
        for old in d.glob("f*.png"):
            old.unlink()
        pals = frame_palettes(bank)
        for k, f in enumerate(bank["frames"]):
            f["palettes"] = pals[k]
            if not f["pieces"]:
                continue
            data = rom.bytes(TILE_BASE + f["off"], 32 * run_tiles(f["pieces"]))
            palette = bgr555(rom.bytes(OBJ_PALETTES + 32 * pals[k][0], 32))
            try:
                img, f["origin"] = draw_frame(f["pieces"], data, palette)
            except ValueError as e:
                fail(f"bank {i} frame {k}: {e}")
            img.save(d / f"f{k:03}.png", bits=4)
        write_json(d / "bank.json", bank["frames"])
    # The round trip: packing the files gives the ROM's pool.
    packed = pack(bank_dirs())
    pool = b"".join(p for _, p, _ in packed)
    if pool != rom.bytes(TILE_BASE, OBJ_PALETTES - TILE_BASE):
        fail("the packed pool differs from the ROM's")
    for i, (_, p, frames) in enumerate(packed):
        assert (ranges[i][1] - ranges[i][0]) == len(p)
        for f, rf in zip(frames, banks[i]["frames"]):
            assert ranges[f["bank"]][0] + f["offset"] == rf["off"]
    print(f"sprite_frames: {len(packed)} banks, {sum(len(b['frames']) for b in banks)} frames, "
          f"{len(pool)} bytes of tiles: round trip OK")


def main():
    args = sys.argv[1:]
    if args and args[0] == "extract" and len(args) <= 2:
        extract(args[1] if len(args) > 1 else "baserom.gba")
    elif args and args[0] == "pack" and len(args) == 2:
        write_pack(pack(bank_dirs()), args[1])
    else:
        sys.exit(__doc__)


if __name__ == "__main__":
    main()
