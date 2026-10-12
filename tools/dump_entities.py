#!/usr/bin/env python3
# One-off extraction tool (not part of the build) that bundles everything
# known about the "category descriptor -> vtable -> animation table ->
# table_A/table_B -> frame" system documented in docs/graphics.md ("Found
# the real per-actor animation-frame system" onward) into two JSON files,
# one per sprite-sheet family:
#
#   graphics/unknown/00_0b2120/entities.json  (categories 0-2)
#   graphics/unknown/01_14174c/entities.json  (categories 3-6)
#
# Purpose: once the actor/entity C code behind this system gets reversed,
# these JSON files are the data half - real ROM addresses, keyframe
# sequences, and (for records whose table_B holds small offsets rather
# than absolute ROM addresses) a direct reference to the already-split
# graphics/unknown/<sheet>/<folder>/*.png frames, so the frame pixel
# data doesn't need to be re-derived from raw hex again.
#
# Re-run any time to regenerate from baserom.gba + the current split
# graphics/ sources (this only reads them, never writes to graphics/).

import glob
import json
import os
import struct

ROM_PATH = os.path.join(os.path.dirname(__file__), '..', 'baserom.gba')
GRAPHICS_ROOT = os.path.join(os.path.dirname(__file__), '..', 'graphics', 'unknown')

with open(ROM_PATH, 'rb') as f:
    ROM = f.read()


def rd(addr, n):
    a = addr & 0x01FFFFFF
    return ROM[a:a + n]


def u32(addr):
    return struct.unpack_from('<I', rd(addr, 4))[0]


def hx(v):
    return f'0x{v:08X}'


def lz77_decompress(data):
    size = data[1] | (data[2] << 8) | (data[3] << 16)
    out = bytearray()
    pos = 4
    while len(out) < size:
        flags = data[pos]
        pos += 1
        for bit in range(8):
            if len(out) >= size:
                break
            if flags & (0x80 >> bit):
                b0 = data[pos]
                b1 = data[pos + 1]
                pos += 2
                length = (b0 >> 4) + 3
                disp = ((b0 & 0xF) << 8) | b1
                disp += 1
                start = len(out) - disp
                for i in range(length):
                    out.append(out[start + i])
            else:
                out.append(data[pos])
                pos += 1
    return bytes(out)


def frame_header_valid_at(buf, off):
    if off + 4 > len(buf):
        return None
    w, h, pad2, pad3 = buf[off], buf[off + 1], buf[off + 2], buf[off + 3]
    if w in (1, 2, 4, 8) and h in (1, 2, 4, 8) and pad2 == 0x10 and pad3 == 0:
        size = 4 + w * h * 32
        if off + size <= len(buf):
            return size
    return None


DESCRIPTOR_BASE = 0x08175558
DESCRIPTOR_STRIDE = 0x34
DESCRIPTOR_FIELD_NAMES = {
    0x00: 'type_00',
    0x04: 'family_const_04',
    0x08: 'family_const_08',
    0x0C: 'conditional_ptr_0C',
    0x10: 'palette',
    0x14: 'sub_effect_table_14',
    0x18: 'anim_table_base_18',
    0x1C: 'sprite_sheet_1C',
    0x20: 'maskAssistDeaths',
    0x24: 'bonusKindDeaths',
    0x28: 'retryBossDeaths',
    0x2C: 'bossLevel',
    0x30: 'retryBossLevel',
}


def read_descriptor(index):
    base = DESCRIPTOR_BASE + index * DESCRIPTOR_STRIDE
    words = struct.unpack_from('<13I', rd(base, 0x34))
    fields = {}
    for off, name in DESCRIPTOR_FIELD_NAMES.items():
        val = words[off // 4]
        # Pointer-shaped fields get hex; small integers stay decimal.
        if val >= 0x08000000 and val < 0x08800000:
            fields[name] = hx(val)
        else:
            fields[name] = val
    return fields


def build_frame_pool(sheet_dir):
    """Read every entity subdirectory's frame PNGs, in ROM order (entity
    folder name, then per-frame filename), and reconstruct the original
    decompressed sheet's byte layout, so a raw pool offset can be
    resolved back to (asset file). Each frame's tile size is just its own
    PNG dimensions / 8 - no sidecar metadata needed."""
    from PIL import Image

    pool = []
    offset = 0
    entity_dirs = sorted(d for d in glob.glob(os.path.join(sheet_dir, '*')) if os.path.isdir(d))
    for entity_dir in entity_dirs:
        frame_paths = sorted(glob.glob(os.path.join(entity_dir, '*.png')))
        for fp in frame_paths:
            with Image.open(fp) as img:
                w, h = img.width // 8, img.height // 8
            size = 4 + w * h * 32
            pool.append({
                'start': offset,
                'end': offset + size,
                'file': f'graphics/unknown/{os.path.basename(sheet_dir)}/{os.path.basename(entity_dir)}/{os.path.basename(fp)}',
                'w_tiles': w,
                'h_tiles': h,
            })
            offset += size
    return pool


def resolve_pool_offset(pool, offset):
    for entry in pool:
        if entry['start'] == offset:
            return {
                'asset_file': entry['file'],
                'w_tiles': entry['w_tiles'],
                'h_tiles': entry['h_tiles'],
            }
    # Falls inside a frame rather than at its start - still resolvable,
    # just note it (shouldn't normally happen for a real keyframe index).
    for entry in pool:
        if entry['start'] < offset < entry['end']:
            return {
                'asset_file': entry['file'],
                'w_tiles': entry['w_tiles'],
                'h_tiles': entry['h_tiles'],
                'note': f'offset {hx(offset)} is mid-frame (frame starts at {hx(entry["start"])})',
            }
    return {'note': f'offset {hx(offset)} not found in any split frame'}


def table_b_real_length(tableb_addr, sheet_buf, max_check=512):
    """Determine this record's real table_B length. table_B[0] is either
    a real ROM address (the overlapping/dedup "mask" scheme - not backed
    by a split sheet at all) or a small offset into the decompressed
    sheet; for the latter, each entry is only counted if it's genuinely a
    valid frame header in the decompressed sheet (not just "a small
    number"), which is what actually distinguishes a real per-object
    table_B array from a coincidentally-small value at a struct that
    isn't one of these at all."""
    first = u32(tableb_addr)
    if first >= 0x08000000:
        return None  # absolute-ROM-address mode, not this scheme
    count = 0
    for i in range(max_check):
        v = u32(tableb_addr + i * 4)
        if v < 0 or v >= len(sheet_buf):
            break
        if frame_header_valid_at(sheet_buf, v) is None:
            break
        count += 1
    return count


def read_table_a(tablea_addr, table_b_len, max_entries=256):
    """Parse table_A entries (12 bytes each): keyframe_index -> table_B
    index (signed halfword at +2). Stops at the first entry whose index
    is out of table_B's real bounds (garbage/next-structure spillover).

    Caveat: a long run of small in-bounds indices (e.g. a real "idle
    hold" on frame 0) is indistinguishable from reading into zeroed
    padding using this bounds check alone, so this is capped at
    max_entries rather than trusted to find the true end on its own -
    see 'truncated_at_cap' on the returned info."""
    entries = []
    truncated = False
    for i in range(max_entries):
        off = tablea_addr + i * 12
        w0, w1, w2 = struct.unpack_from('<3I', rd(off, 12))
        tb_index = struct.unpack_from('<h', rd(off + 2, 2))[0]
        if table_b_len is not None:
            if tb_index < 0 or tb_index >= table_b_len:
                break
        else:
            # absolute-ROM-address mode: table_B holds real ROM pointers,
            # so bound differently - just cap at a sane keyframe count.
            if tb_index < 0 or tb_index > 400:
                break
        entries.append({
            'keyframe_index': i,
            'table_B_index': tb_index,
            'raw_words': [hx(w0), hx(w1), hx(w2)],
        })
    else:
        truncated = True
    return entries, truncated


def dump_family(sheet_rom_addr, decompressed_size, sheet_dir, anim_table_base,
                 category_indices, record_slot_count, labels):
    pool = build_frame_pool(sheet_dir)
    sheet_buf = lz77_decompress(rd(sheet_rom_addr, decompressed_size * 2))
    assert len(sheet_buf) == decompressed_size, (len(sheet_buf), decompressed_size)

    categories = []
    for cat_idx in category_indices:
        categories.append({
            'category_index': cat_idx,
            'fields': read_descriptor(cat_idx),
        })

    records = []
    seen_pairs = {}
    for slot in range(record_slot_count):
        base = anim_table_base + slot * 40
        idx, table_a, table_b, hdr = struct.unpack_from('<4I', rd(base, 16))
        if table_a == 0 or not (0x08000000 <= table_a < 0x08800000):
            records.append({'slot': slot, 'valid': False})
            continue

        pair_key = (table_a, table_b)
        dup_of = seen_pairs.get(pair_key)
        if dup_of is None:
            seen_pairs[pair_key] = slot

        tb_len = table_b_real_length(table_b, sheet_buf)
        addressing_mode = 'absolute_rom' if tb_len is None else 'pool_offset'
        keyframes, truncated_at_cap = read_table_a(table_a, tb_len)

        frames = {}
        table_b_indices_used = sorted({kf['table_B_index'] for kf in keyframes})
        for tb_i in table_b_indices_used:
            value = u32(table_b + tb_i * 4)
            if addressing_mode == 'absolute_rom':
                w, h = struct.unpack_from('<2B', rd(value, 2))
                frames[str(tb_i)] = {'rom_address': hx(value), 'w_tiles': w, 'h_tiles': h}
            else:
                frames[str(tb_i)] = resolve_pool_offset(pool, value)

        rec = {
            'slot': slot,
            'valid': True,
            'table_A_address': hx(table_a),
            'table_B_address': hx(table_b),
            'palette_bank': hdr,
            'addressing_mode': addressing_mode,
            'duplicate_of_slot': dup_of if dup_of != slot else None,
            'keyframes': keyframes,
            'keyframes_truncated_at_cap': truncated_at_cap,
            'frames': frames,
        }
        rec['label'] = labels.get(slot)
        records.append(rec)

    return {
        'sheet_rom_address': hx(sheet_rom_addr),
        'decompressed_size': decompressed_size,
        'graphics_dir': f'graphics/unknown/{os.path.basename(sheet_dir)}',
        'descriptor_rom_address': hx(DESCRIPTOR_BASE),
        'descriptor_stride': DESCRIPTOR_STRIDE,
        'categories': categories,
        'animation_table_rom_address': hx(anim_table_base),
        'record_stride': 40,
        'records': records,
    }


# Each record's actor kind (include/constants/actor_kinds.h): the record's
# slot is the kind's number.
FAMILY1_LABELS = {
    0: 'POLAR_KIND_PLAYER',
    1: 'POLAR_KIND_BASIC_CRATE',
    2: 'POLAR_KIND_RIDERLESS_POLAR',
    3: 'POLAR_KIND_CHECKPOINT_CRATE',
    4: 'POLAR_KIND_NITRO_CRATE',
    5: 'POLAR_KIND_TIME_CRATE_1',
    6: 'POLAR_KIND_TIME_CRATE_2',
    7: 'POLAR_KIND_TIME_CRATE_3',
    8: 'POLAR_KIND_LIFE_CRATE',
    9: 'POLAR_KIND_AKU_AKU_CRATE',
    10: 'POLAR_KIND_FOUR_WUMPA_CRATE',
    11: 'POLAR_KIND_WUMPA',
    12: 'POLAR_KIND_BOOST_PAD',
    13: 'POLAR_KIND_OBSTACLE',
    14: 'POLAR_KIND_OBSTACLE_2',
    15: 'POLAR_KIND_UNK15',
    16: 'POLAR_KIND_ICICLE_A',
    17: 'POLAR_KIND_ICICLE_A_RIGHT',
    18: 'POLAR_KIND_ICICLE_B',
    19: 'POLAR_KIND_ICICLE_B_RIGHT',
    20: 'POLAR_KIND_ICICLE_C',
    21: 'POLAR_KIND_ICICLE_C_RIGHT',
    22: 'POLAR_KIND_LAUNCHER',
    23: 'POLAR_KIND_ELECTRIC_FENCE',
    24: 'POLAR_KIND_PENGUIN',
    25: 'POLAR_KIND_GOAL',
    26: 'POLAR_KIND_GOAL_2',
    27: 'POLAR_KIND_AKU_AKU',
    28: 'POLAR_KIND_QUESTION_CRATE_WUMPA_1',
    29: 'POLAR_KIND_QUESTION_CRATE_WUMPA_3',
    30: 'POLAR_KIND_QUESTION_CRATE_WUMPA_5',
    31: 'POLAR_KIND_QUESTION_CRATE_AKU_AKU',
    32: 'POLAR_KIND_PATH_SLOW',
    33: 'POLAR_KIND_PATH_MEDIUM',
    34: 'POLAR_KIND_PATH_FAST',
    38: 'POLAR_KIND_PALETTE_CYCLE_2',
    39: 'POLAR_KIND_PALETTE_CYCLE_3',
    40: 'POLAR_KIND_CHECKPOINT_TEXT',
}

FAMILY2_LABELS = {
    0: 'JETPACK_KIND_PLAYER',
    1: 'JETPACK_KIND_PLANE',
    2: 'JETPACK_KIND_SHOT',
    3: 'JETPACK_KIND_CANNONBALL',
    4: 'JETPACK_KIND_BOMBER_IDLE',
    5: 'JETPACK_KIND_BOMBER_HOME',
    6: 'JETPACK_KIND_BOMBER_BOB_VERTICAL',
    7: 'JETPACK_KIND_BOMBER_SWING_HORIZONTAL',
    8: 'JETPACK_KIND_BOMBER_CIRCLE',
    9: 'JETPACK_KIND_BOMBER_DROP',
    10: 'JETPACK_KIND_HOVERCRAFT',
    11: 'JETPACK_KIND_HOVERCRAFT_CANNON',
    12: 'JETPACK_KIND_HOVERCRAFT_LAUNCHER',
    13: 'JETPACK_KIND_HOVERCRAFT_SIDE_GUN',
    14: 'JETPACK_KIND_HOVERCRAFT_CANNON_FLASH',
    15: 'JETPACK_KIND_UNK15',
    16: 'JETPACK_KIND_AIRSHIP_0',
    17: 'JETPACK_KIND_AIRSHIP_1',
    18: 'JETPACK_KIND_AIRSHIP_2',
    19: 'JETPACK_KIND_HEALTH_CRATE',
    20: 'JETPACK_KIND_QUESTION_CRATE_WUMPA_1',
    21: 'JETPACK_KIND_QUESTION_CRATE_WUMPA_3',
    22: 'JETPACK_KIND_QUESTION_CRATE_WUMPA_5',
    23: 'JETPACK_KIND_QUESTION_CRATE_LIFE',
    24: 'JETPACK_KIND_TIME_CRATE_1',
    25: 'JETPACK_KIND_TIME_CRATE_2',
    26: 'JETPACK_KIND_TIME_CRATE_3',
    27: 'JETPACK_KIND_PARACHUTE_NITRO',
    28: 'JETPACK_KIND_ROCKET',
    29: 'JETPACK_KIND_STOPWATCH',
    30: 'JETPACK_KIND_UNK30',
    31: 'JETPACK_KIND_RING_LEFT',
    32: 'JETPACK_KIND_PATH_0',
    33: 'JETPACK_KIND_PATH_1',
    34: 'JETPACK_KIND_PATH_2',
    35: 'JETPACK_KIND_PATH_3',
    36: 'JETPACK_KIND_PATH_4',
    37: 'JETPACK_KIND_PATH_5',
    38: 'JETPACK_KIND_AIRSHIP_FIREBALL',
    39: 'JETPACK_KIND_HOVERCRAFT_FIREBALL',
    40: 'JETPACK_KIND_BALLOON_RED',
    41: 'JETPACK_KIND_BALLOON_YELLOW',
    42: 'JETPACK_KIND_BALLOON_RED_CROSS',
    43: 'JETPACK_KIND_RING_RIGHT',
    44: 'JETPACK_KIND_COLLECTED_WUMPA',
    45: 'JETPACK_KIND_EXPLOSION',
    46: 'JETPACK_KIND_CHECKPOINT_TEXT',
}


def main():
    family1 = dump_family(
        sheet_rom_addr=0x080B2120,
        decompressed_size=213064,
        sheet_dir=os.path.join(GRAPHICS_ROOT, '00_0b2120'),
        anim_table_base=0x081796CC,
        category_indices=[0, 1, 2],
        record_slot_count=41,
        labels=FAMILY1_LABELS,
    )
    out1 = os.path.join(GRAPHICS_ROOT, '00_0b2120', 'entities.json')
    with open(out1, 'w') as f:
        json.dump(family1, f, indent=2)
        f.write('\n')
    print('wrote', out1)

    family2 = dump_family(
        sheet_rom_addr=0x0814174C,
        decompressed_size=207124,
        sheet_dir=os.path.join(GRAPHICS_ROOT, '01_14174c'),
        anim_table_base=0x0817B2A4,
        category_indices=[3, 4, 5, 6],
        record_slot_count=47,
        labels=FAMILY2_LABELS,
    )
    out2 = os.path.join(GRAPHICS_ROOT, '01_14174c', 'entities.json')
    with open(out2, 'w') as f:
        json.dump(family2, f, indent=2)
        f.write('\n')
    print('wrote', out2)


if __name__ == '__main__':
    main()
