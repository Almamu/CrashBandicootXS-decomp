#!/usr/bin/env python3
# Splits the two category-descriptor sprite sheets into one PNG+.frames
# pair per distinct entity (one per unique table_B address - i.e. one per
# genuinely distinct set of frame data, regardless of how many animation-
# table slots/keyframe-timings reuse it), replacing whatever
# graphics/unknown/<sheet>/*.png|*.frames files currently exist.
#
# Why per-entity rather than per-byte-range like the first pass: the
# animation tables (docs/graphics.md, "Found the real per-actor
# animation-frame system") turned out to have many more records than
# first catalogued, so the original split (done before that was known)
# bundled several unrelated objects into single files (e.g. "barrel.png"
# actually held 3 distinct objects). This re-derives the true boundaries
# from every valid animation-table slot's table_B address and re-splits
# cleanly, so each entity can be edited on its own without wading through
# unrelated sprites.
#
# Run this BEFORE tools/dump_entities.py (which reads whatever *.frames
# files are present to resolve keyframes to asset files/frame indices).

import glob
import os
import struct
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(__file__))
from dump_entities import rd, u32, hx, lz77_decompress  # noqa: E402

FRAMED_GFX = os.path.join(os.path.dirname(__file__), 'framed_gfx.py')
GRAPHICS_ROOT = os.path.join(os.path.dirname(__file__), '..', 'graphics', 'unknown')


def all_anchors_by_table_b(anim_base, slot_count, sheet_size):
    """One entry per distinct table_B address whose first entry is a
    pool offset (not the mask-style absolute ROM address scheme),
    sorted by that offset - this is the authoritative partition of the
    sheet into per-entity byte ranges (verified gapless/non-overlapping
    against the full sheet size for both sheets during development)."""
    slots_by_tb = {}
    for slot in range(slot_count):
        base = anim_base + slot * 40
        idx, table_a, table_b, hdr = struct.unpack_from('<4I', rd(base, 16))
        if table_a == 0 or not (0x08000000 <= table_a < 0x08800000):
            continue
        first = u32(table_b)
        if first >= 0x08000000 or not (0 <= first < sheet_size):
            continue
        slots_by_tb.setdefault(table_b, {'start': first, 'slots': []})
        slots_by_tb[table_b]['slots'].append(slot)
    anchors = [(info['start'], tb, sorted(info['slots'])) for tb, info in slots_by_tb.items()]
    anchors.sort()
    return anchors


def name_for(slots, labels):
    for s in slots:
        if s in labels:
            return labels[s]
    return f'record_slot{slots[0]:02d}'


def split_family(sheet_rom_addr, decompressed_size, sheet_dir, anim_table_base,
                  record_slot_count, labels, palette_rom_addr):
    sheet_buf = lz77_decompress(rd(sheet_rom_addr, decompressed_size * 2))
    assert len(sheet_buf) == decompressed_size

    anchors = all_anchors_by_table_b(anim_table_base, record_slot_count, decompressed_size)
    assert anchors[0][0] == 0, 'first entity does not start at offset 0'

    pal_path = os.path.join(tempfile.gettempdir(), f'split_pal_{palette_rom_addr:08x}.bin')
    with open(pal_path, 'wb') as f:
        f.write(rd(palette_rom_addr, 32))

    for old in glob.glob(os.path.join(sheet_dir, '*.png')) + glob.glob(os.path.join(sheet_dir, '*.frames')):
        os.remove(old)

    used_names = set()
    for i, (start, table_b, slots) in enumerate(anchors):
        end = anchors[i + 1][0] if i + 1 < len(anchors) else decompressed_size
        name = name_for(slots, labels)
        assert name not in used_names, f'duplicate entity name: {name}'
        used_names.add(name)

        chunk = sheet_buf[start:end]
        bin_path = os.path.join(tempfile.gettempdir(), f'split_chunk_{i:02d}.bin')
        with open(bin_path, 'wb') as f:
            f.write(chunk)

        png_path = os.path.join(sheet_dir, f'{i:02d}_{name}.png')
        frames_path = os.path.join(sheet_dir, f'{i:02d}_{name}.frames')
        subprocess.run(
            [sys.executable, FRAMED_GFX, 'to-png', bin_path, png_path, frames_path, pal_path],
            check=True,
        )
        print(f'{os.path.basename(png_path)}: {end - start} bytes, slots {slots}')


def main():
    FAMILY1_LABELS = {
        1: 'tnt_crate', 2: 'small_creature', 3: 'barrel', 4: 'nitro_crate',
        5: 'crate_variant_a', 6: 'crate_variant_b', 7: 'crate_variant_c',
        8: 'crate_variant_d', 9: 'crate_variant_e', 11: 'wumpa_fruit',
        22: 'emerging_creature', 23: 'guard_barrier', 40: 'checkpoint_text',
    }
    FAMILY2_LABELS = {
        1: 'winged_creature', 12: 'treasure_chest', 19: 'crate_question_mark',
        24: 'crate_1', 25: 'crate_2', 26: 'crate_3', 27: 'parachute_crate',
        29: 'clock', 31: 'crescent_arch', 40: 'balloon_red_yellow',
        41: 'balloon_orange_blue', 42: 'balloon_yellow_blue_cross',
        46: 'checkpoint_text',
    }

    split_family(
        sheet_rom_addr=0x080B2120, decompressed_size=213064,
        sheet_dir=os.path.join(GRAPHICS_ROOT, '00_0b2120'),
        anim_table_base=0x081796CC, record_slot_count=41,
        labels=FAMILY1_LABELS, palette_rom_addr=0x08178F80,
    )
    split_family(
        sheet_rom_addr=0x0814174C, decompressed_size=207124,
        sheet_dir=os.path.join(GRAPHICS_ROOT, '01_14174c'),
        anim_table_base=0x0817B2A4, record_slot_count=47,
        labels=FAMILY2_LABELS, palette_rom_addr=0x0817AAA4,
    )


if __name__ == '__main__':
    main()
