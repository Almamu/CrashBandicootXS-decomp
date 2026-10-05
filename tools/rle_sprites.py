#!/usr/bin/env python3
"""The zero-run-compressed OBJ frame sets of the actor categories (the old
"rotation strips" gPolarPlayerRleFrames, gYetiRleFrames and
gJetpackPlayerRleFrames, see docs/data.md "Compressed sprite frames").

Every frame is a 4-byte header {w, h, 0x30, 0} (w x h 8x8 tiles), then a
stream of u16 counts. The IWRAM decoder at 0x03000634 (the
gUnpackRleSpriteFrameFunc hook) fills w*h*32 bytes of VRAM from it, one halfword
at a time: a first count of zero halfwords, then alternately a literal
count followed by that many halfwords of 4bpp tile data, and a zero count,
until the frame is full. The frames of a set are stored back to back.

The encoder that made them turns every stretch of at least 7 zero
halfwords into a zero run and keeps shorter ones inside the literal runs;
the frame's leading zeros are always a zero run (empty if the frame starts
with pixels). That rule rebuilds all 344 frames byte for byte.

    tools/rle_sprites.py extract [baserom.gba]
        Writes graphics/rle_sprites/<addr>_frames.png for the three sets:
        indexed 4bpp, one frame per w*8 x h*8 block, stacked top to bottom
        (so grit's row-major tile output is the frames back to back). Checks
        that re-encoding gives the ROM's bytes.

    tools/rle_sprites.py pack TILES OUT.inc OUT.h --frame WxH --name NAME
        TILES is grit's 4bpp tile output (-gt -gB4 -ftb) of such a PNG.
        Writes the compressed frames as the body of a u8 initializer, and a
        header with each frame's byte offset in it, NAME_FRAME_nnn.
"""
import argparse
import struct
import sys
from pathlib import Path

ROM_BASE = 0x08000000
TAG = 0x30
MIN_ZERO_RUN = 7

# (address, end, palette for the PNG): the categories' 256-colour OBJ
# palettes (category_descriptor.palette), bank 0, which draws them cleanly.
SETS = [
    (0x080C2758, 0x080DA1D8, 0x08178F80),
    (0x080DA1D8, 0x080FF1B0, 0x08178F80),
    (0x0815A050, 0x08167AD4, 0x0817AAA4),
]


def decode(data, pos):
    """-> (w, h, halfwords, end) for the frame at data[pos:]."""
    w, h, tag, pad = data[pos:pos + 4]
    if tag != TAG or pad != 0:
        raise ValueError(f"frame at {pos:#x}: header {data[pos:pos + 4].hex()}")
    total = w * h * 16
    out = []
    pos += 4
    zero = True
    while True:
        (n,) = struct.unpack_from("<H", data, pos)
        pos += 2
        if zero:
            out += [0] * n
        else:
            out += struct.unpack_from(f"<{n}H", data, pos)
            pos += 2 * n
        if len(out) >= total:
            break
        zero = not zero
    if len(out) != total:
        raise ValueError(f"frame ends at {pos:#x}: {len(out)} of {total} halfwords")
    return w, h, out, pos


def encode(w, h, hw):
    assert len(hw) == w * h * 16
    runs = []  # [is_zero, [halfwords]]
    i, n = 0, len(hw)
    while i < n:
        j = i
        if hw[i] == 0:
            while j < n and hw[j] == 0:
                j += 1
            if i == 0 or j - i >= MIN_ZERO_RUN:
                runs.append([True, hw[i:j]])
                i = j
                continue
        else:
            j = i + 1
        if runs and not runs[-1][0]:
            runs[-1][1].extend(hw[i:j])
        else:
            runs.append([False, list(hw[i:j])])
        i = j
    if not runs[0][0]:
        runs.insert(0, [True, []])
    out = bytearray([w, h, TAG, 0])
    for is_zero, values in runs:
        out += struct.pack("<H", len(values))
        if not is_zero:
            out += struct.pack(f"<{len(values)}H", *values)
    return bytes(out)


def extract(rom_path):
    from PIL import Image

    rom = Path(rom_path).read_bytes()
    for start, end, pal in SETS:
        pos, frames = start - ROM_BASE, []
        while pos < end - ROM_BASE:
            w, h, hw, nxt = decode(rom, pos)
            if encode(w, h, hw) != rom[pos:nxt]:
                sys.exit(f"rle_sprites: frame at {pos + ROM_BASE:#x} does not re-encode")
            frames.append((w, h, struct.pack(f"<{len(hw)}H", *hw)))
            pos = nxt
        if pos != end - ROM_BASE or len({f[:2] for f in frames}) != 1:
            sys.exit(f"rle_sprites: {start:#x}: frames don't tile the set")
        w, h = frames[0][:2]
        img = Image.new("P", (w * 8, h * 8 * len(frames)), 0)
        px = img.load()
        for f, (_, _, tiles) in enumerate(frames):
            for t in range(w * h):
                tx, ty = t % w * 8, (f * h + t // w) * 8
                for i in range(64):
                    px[tx + i % 8, ty + i // 8] = tiles[t * 32 + i // 2] >> (4 * (i & 1)) & 0xF
        palette = []
        for i in range(16):
            (c,) = struct.unpack_from("<H", rom, pal - ROM_BASE + 2 * i)
            palette += [((c >> s) & 31) * 255 // 31 for s in (0, 5, 10)]
        img.putpalette(palette)
        path = Path(f"graphics/rle_sprites/{start - ROM_BASE:06x}_frames.png")
        path.parent.mkdir(parents=True, exist_ok=True)
        img.save(path, bits=4)
        print(f"{path}: {len(frames)} frames of {w}x{h} tiles, {end - start:#x} bytes")


def pack(args):
    try:
        w, h = (int(x) for x in args.frame.split("x"))
    except ValueError:
        sys.exit("rle_sprites: --frame is WxH (in tiles)")
    tiles = Path(args.input).read_bytes()
    size = w * h * 32
    if not tiles or len(tiles) % size:
        sys.exit(f"rle_sprites: {args.input}: {len(tiles)} bytes is not a whole number of {w}x{h}-tile frames")
    out, offsets = bytearray(), []
    for f in range(len(tiles) // size):
        offsets.append(len(out))
        chunk = tiles[f * size:(f + 1) * size]
        out += encode(w, h, list(struct.unpack(f"<{size // 2}H", chunk)))
    lines = [f"/* generated by tools/rle_sprites.py from {args.input} */"]
    for f, off in enumerate(offsets):
        nxt = offsets[f + 1] if f + 1 < len(offsets) else len(out)
        lines.append(f"\t/* frame {f} */")
        lines += ["\t" + ", ".join(f"0x{b:02X}" for b in out[i:min(i + 16, nxt)]) + ","
                  for i in range(off, nxt, 16)]
    Path(args.output).write_text("\n".join(lines) + "\n")
    hdr = [f"/* generated by tools/rle_sprites.py from {args.input} */",
           f"#define {args.name}_SIZE 0x{len(out):x}",
           f"#define {args.name}_COUNT {len(offsets)}"]
    hdr += [f"#define {args.name}_FRAME_{f:03d} 0x{off:x}" for f, off in enumerate(offsets)]
    Path(args.header).write_text("\n".join(hdr) + "\n")


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    sub = ap.add_subparsers(dest="mode", required=True)
    ex = sub.add_parser("extract")
    ex.add_argument("rom", nargs="?", default="baserom.gba")
    pk = sub.add_parser("pack")
    pk.add_argument("input")
    pk.add_argument("output")
    pk.add_argument("header")
    pk.add_argument("--frame", required=True)
    pk.add_argument("--name", required=True)
    args = ap.parse_args()
    if args.mode == "extract":
        extract(args.rom)
    else:
        pack(args)


if __name__ == "__main__":
    main()
