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
# graphics/unknown/<sheet>/*.png + .frames sources, so the frame pixel
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
    0x20: 'unknown_20',
    0x24: 'threshold_24',
    0x28: 'unknown_28',
    0x2C: 'flag_2C',
    0x30: 'unknown_30',
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
            'header_byte': hdr,
            'addressing_mode': addressing_mode,
            'duplicate_of_slot': dup_of if dup_of != slot else None,
            'keyframes': keyframes,
            'keyframes_truncated_at_cap': truncated_at_cap,
            'frames': frames,
        }
        if slot in labels:
            rec['label'] = labels[slot]['label']
            rec['identified'] = labels[slot]['identified']
        else:
            rec['label'] = None
            rec['identified'] = False
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


FAMILY1_LABELS = {
    0: {'label': 'mask (possibly Aku Aku - unconfirmed)', 'identified': False},
    1: {'label': 'TNT crate', 'identified': True},
    2: {'label': 'small gray/white creature', 'identified': False},
    3: {'label': 'barrel/drum (unconfirmed beyond shape)', 'identified': False},
    4: {'label': 'Nitro crate ("NITRO" text visible)', 'identified': True},
    5: {'label': 'decorated crate variant A', 'identified': False},
    6: {'label': 'decorated crate variant B', 'identified': False},
    7: {'label': 'decorated crate variant C', 'identified': False},
    8: {'label': 'decorated crate variant D', 'identified': False},
    9: {'label': 'decorated crate variant E', 'identified': False},
    11: {'label': 'Wumpa Fruit', 'identified': True},
    22: {'label': 'creature/item emerging from a shell (unconfirmed identity)', 'identified': False},
    23: {'label': 'two guards holding a chain barrier', 'identified': True},
    40: {'label': '"CHECK POINT" text', 'identified': True},
}

FAMILY2_LABELS = {
    12: {'label': 'treasure chest', 'identified': True},
    19: {'label': 'wooden crate with a "?" mark', 'identified': True},
    24: {'label': 'wooden crate with a "1"', 'identified': True},
    25: {'label': 'wooden crate with a "2"', 'identified': True},
    26: {'label': 'wooden crate with a "3"-like digit', 'identified': True},
    27: {'label': 'parachute-dropped crate', 'identified': True},
    29: {'label': 'clock face', 'identified': True},
    31: {'label': 'crescent/arch-shaped striped object (unconfirmed identity)', 'identified': False},
    40: {'label': 'balloon (red/yellow)', 'identified': True},
    41: {'label': 'balloon (orange/red/blue)', 'identified': True},
    42: {'label': 'balloon (yellow/blue, cross/plus mark)', 'identified': True},
    46: {'label': '"CHECK POINT" text', 'identified': True},
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
