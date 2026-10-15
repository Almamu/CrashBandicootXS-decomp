#!/usr/bin/env python3
# Splits the two category-family framed OBJ sprite sheets (gPolarSpriteSheet,
# gJetpackSpriteSheet) into graphics/vehicles/<family>/sprite_sheet/<name>/
# folders of one indexed PNG per frame, replacing whatever is there, and
# writes graphics/vehicles/<family>/sprite_sheet/sheet.json (the folders in
# the sheet's order, which records use each folder and
# in which palette bank).
#
# Grouping: the sheet is cut wherever the set of animation-table records
# whose table_B array references a frame changes, so a folder is one run of
# consecutive frames that the same records use (docs/graphics.md, "The
# framed sheets"). A record's frames can therefore span several folders:
# the polar crates all end in the shared crate_shards run, the jetpack
# rocket's frames are rocket + explosion + rocket_burst, and so on. The
# order of sheet.json's "folders" list is the sheet's byte order, which
# is also the order the build packs them in (graphics.mk's
# FRAMED_SHEET_RULE, tools/framed_gfx.py order), so the names below may
# change freely.
#
# Colours: each folder's PNGs carry the 16 colours of the palette bank its
# first record (in table order) draws it with - anim_table_record.palette
# (+0x0C) into the family's 256-colour OBJ palette, gPolarCategoryPalette
# or gAirshipCategoryPalette; the hovercraft's own parts (records 10-14,
# only category 6 spawns them) use category 6's gHovercraftCategoryPalette.
# Only the colours change with the bank: the build reads the 4-bit indices
# (tools/framed_gfx.py), so the bank chosen never affects the ROM. A folder
# drawn with several banks lists the others in sheet.json. The sheet's
# graphics/manifest.json entry can pick another of them for a folder:
# "folder_banks": {"<folder>": bank} (the jetpack explosion is drawn in the
# explosion actor's bank 4, not the bomber's 2).
#
# Run this BEFORE tools/dump_entities.py (which resolves keyframes to the
# PNGs present). The animation tables' frame arrays (data/levels/anim_tables/)
# name frames by folder ("<folder>/<NN>", FRAMED_<SHEET>_<NAME>_<NN> in the C),
# so renaming a folder means renaming its uses there.

import glob
import json
import os
import struct
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(__file__))
from dump_entities import rd, hx, lz77_decompress  # noqa: E402

FRAMED_GFX = os.path.join(os.path.dirname(__file__), 'framed_gfx.py')
GRAPHICS_ROOT = os.path.join(os.path.dirname(__file__), '..', 'graphics', 'vehicles')
MANIFEST = os.path.join(os.path.dirname(__file__), '..', 'graphics', 'manifest.json')
RECORD_SIZE = 40

PALETTES = {
    0x08178F80: 'gPolarCategoryPalette',
    0x0817AAA4: 'gAirshipCategoryPalette',
    0x0817AEA4: 'gHovercraftCategoryPalette',
}

# Folder names, keyed by the slots of the records that use the run; a slot
# set that owns several runs lists their names in sheet order. The names are
# the records' actor kinds (include/constants/actor_kinds.h) and what the
# frames show.
POLAR_NAMES = {
    (5,): ['time_crate_1'],
    (6,): ['time_crate_2'],
    (7,): ['time_crate_3'],
    (28, 29, 30, 31, 35): ['question_crate'],
    (8,): ['life_crate'],
    (9,): ['aku_aku_crate'],
    (4,): ['nitro_crate', 'nitro_crate_burst'],
    (1, 10): ['basic_crate'],
    (25, 26): ['goal'],
    (13, 14): ['obstacle'],
    (27,): ['aku_aku', 'aku_aku_feathers'],
    (24,): ['penguin'],
    (3,): ['checkpoint_crate'],
    (12, 15, 36, 37, 38, 39): ['boost_pad'],
    (16, 17, 18, 19, 20, 21): ['icicle'],
    (2,): ['riderless_polar'],
    (23,): ['electric_fence', 'electric_fence_wreck'],
    (11,): ['wumpa'],
    (22,): ['launcher'],
    # the wooden crates' break-up: basic, question, life, Aku Aku and
    # checkpoint crates (banks 1, 3 and 5)
    (1, 3, 8, 9, 10, 28, 29, 30, 31, 35): ['crate_shards'],
    (5, 6, 7): ['time_crate_shards'],
    (40,): ['checkpoint_text'],
}

JETPACK_NAMES = {
    (2,): ['shot'],
    (1,): ['plane'],
    (4, 5, 6, 7, 8, 9): ['bomber'],
    (3,): ['cannonball'],
    (38, 39): ['fireball', 'fireball_burst'],
    (42,): ['balloon_red_cross'],
    (40,): ['balloon_red'],
    (41,): ['balloon_yellow'],
    (19, 20, 21, 22, 23): ['question_crate', 'crate_shards'],
    (24,): ['time_crate_1'],
    (25,): ['time_crate_2'],
    (26,): ['time_crate_3'],
    # the bomber's, the rocket's and the explosion actor's (banks 2, 5, 4);
    # drawn in 4 (graphics/manifest.json's folder_banks)
    (4, 5, 6, 7, 8, 9, 28, 30, 45): ['explosion'],
    (27,): ['parachute_nitro'],
    (44,): ['collected_wumpa'],
    (31, 43): ['ring'],
    (28, 30): ['rocket', 'rocket_burst'],
    (29,): ['stopwatch'],
    (24, 25, 26): ['time_crate_shards'],
    # the hovercraft's cannon; the airship kinds 16-18 and the path points
    # 32-37 point their records at it too
    (10, 11, 15, 16, 17, 18, 32, 33, 34, 35, 36, 37): ['hovercraft_cannon'],
    (12,): ['hovercraft_launcher'],
    (13,): ['hovercraft_side_gun'],
    (14,): ['hovercraft_cannon_flash'],
    (46,): ['checkpoint_text'],
}

# The hovercraft's own parts (jetpack records), drawn only in category 6.
HOVERCRAFT_SLOTS = {10, 11, 12, 13, 14}


def sheet_frames(buf):
    """The byte offset of every frame in a decompressed sheet."""
    offsets = []
    pos = 0
    while pos < len(buf):
        w, h = buf[pos], buf[pos + 1]
        offsets.append(pos)
        pos += 4 + w * h * 32
    assert pos == len(buf)
    return offsets


def read_records(anim_base, slot_count, data_end):
    """(slot, table_B address, palette bank, [frame offsets]) of every record
    whose table_B holds sheet offsets. A table_B array runs to the next
    array (or the table) any record points at, as src/.../data/ cuts them."""
    raw = []
    starts = {anim_base, anim_base + slot_count * RECORD_SIZE, data_end}
    for slot in range(slot_count):
        _, table_a, table_b, palette = struct.unpack_from('<4I', rd(anim_base + slot * RECORD_SIZE, 16))
        if table_a == 0:
            continue
        raw.append((slot, table_b, palette & 0xFF))
        starts |= {table_a, table_b}
    records = []
    for slot, table_b, bank in raw:
        end = min(s for s in starts if s > table_b)
        values = struct.unpack(f'<{(end - table_b) // 4}I', rd(table_b, end - table_b))
        if values[0] >= 0x08000000:
            continue  # the player's compressed frames (graphics/vehicles/*/player.png)
        records.append((slot, table_b, bank, values))
    return records


def folder_banks(sheet_rom_addr):
    """The manifest entry's "folder_banks" for the sheet at sheet_rom_addr:
    the bank to draw a folder in instead of its first record's."""
    with open(MANIFEST) as f:
        for e in json.load(f)['assets']:
            if e['kind'] == 'framed_sheet' and 'rom' in e and int(e['rom'], 16) == sheet_rom_addr:
                return e.get('folder_banks', {})
    raise SystemExit(f'{MANIFEST}: no framed_sheet entry at {hx(sheet_rom_addr)}')


def split_family(sheet_rom_addr, decompressed_size, sheet_dir, anim_table_base,
                 record_slot_count, data_end, names, family_palette, own_palettes=None):
    sheet_buf = lz77_decompress(rd(sheet_rom_addr, decompressed_size * 2))
    assert len(sheet_buf) == decompressed_size
    offsets = sheet_frames(sheet_buf)
    index_of = {off: i for i, off in enumerate(offsets)}

    users = [[] for _ in offsets]  # (slot, bank, table_B) per frame
    for slot, table_b, bank, values in read_records(anim_table_base, record_slot_count, data_end):
        for v in values:
            users[index_of[v]].append((slot, bank, table_b))

    runs = []  # [table_B set, first frame, last frame]
    for i, u in enumerate(users):
        assert u, f'{sheet_dir}: frame {i} is used by no record'
        key = sorted({tb for _, _, tb in u})
        if runs and runs[-1][0] == key:
            runs[-1][2] = i
        else:
            runs.append([key, i, i])

    for old in glob.glob(os.path.join(sheet_dir, '*')):
        if os.path.isdir(old):
            for f in glob.glob(os.path.join(old, '*')):
                os.remove(f)
            os.rmdir(old)
        else:
            os.remove(old)

    chosen = folder_banks(sheet_rom_addr)
    pending = {k: list(v) for k, v in names.items()}
    folders = []
    for n, (_, first, last) in enumerate(runs):
        u = sorted(users[first])
        slots = tuple(sorted({s for s, _, _ in u}))
        assert pending.get(slots), f'{sheet_dir}: no name for the frames {first}-{last} (records {slots})'
        name = pending[slots].pop(0)
        banks = []
        for _, bank, _ in u:
            if bank not in banks:
                banks.append(bank)
        if name in chosen:
            assert chosen[name] in banks, f'{sheet_dir}: {name}/ is never drawn in bank {chosen[name]}'
            banks.remove(chosen[name])
            banks.insert(0, chosen.pop(name))
        palette = (own_palettes or {}).get(u[0][0], family_palette)

        end = offsets[last + 1] if last + 1 < len(offsets) else decompressed_size
        tmp = tempfile.mkdtemp()
        bin_path = os.path.join(tmp, 'chunk.bin')
        pal_path = os.path.join(tmp, 'pal.bin')
        with open(bin_path, 'wb') as f:
            f.write(sheet_buf[offsets[first]:end])
        with open(pal_path, 'wb') as f:
            f.write(rd(palette + banks[0] * 32, 32))
        entity_dir = os.path.join(sheet_dir, name)
        subprocess.run([sys.executable, FRAMED_GFX, 'to-frames', bin_path, entity_dir, pal_path], check=True)
        os.remove(bin_path)
        os.remove(pal_path)
        os.rmdir(tmp)

        entry = {
            'folder': name,
            'frames': last - first + 1,
            'records': list(slots),
            'palette': PALETTES[palette],
            'bank': banks[0],
        }
        if len(banks) > 1:
            entry['other_banks'] = banks[1:]
        folders.append(entry)
        print(f'{entry["folder"]}/: {entry["frames"]} frames, records {list(slots)}, '
              f'{PALETTES[palette]} bank {banks}')

    left = {k: v for k, v in pending.items() if v}
    assert not left, f'{sheet_dir}: unused names {left}'
    assert not chosen, f'{sheet_dir}: folder_banks names no folder: {chosen}'
    with open(os.path.join(sheet_dir, 'sheet.json'), 'w') as f:
        json.dump({'sheet_rom_address': hx(sheet_rom_addr), 'folders': folders}, f, indent=2)
        f.write('\n')


def main():
    split_family(
        sheet_rom_addr=0x080B2120, decompressed_size=213064,
        sheet_dir=os.path.join(GRAPHICS_ROOT, 'polar', 'sprite_sheet'),
        anim_table_base=0x081796CC, record_slot_count=41, data_end=0x0817A6B8,
        names=POLAR_NAMES, family_palette=0x08178F80,
    )
    split_family(
        sheet_rom_addr=0x0814174C, decompressed_size=207124,
        sheet_dir=os.path.join(GRAPHICS_ROOT, 'jetpack', 'sprite_sheet'),
        anim_table_base=0x0817B2A4, record_slot_count=47, data_end=0x0817C1C0,
        names=JETPACK_NAMES, family_palette=0x0817AAA4,
        own_palettes={slot: 0x0817AEA4 for slot in HOVERCRAFT_SLOTS},
    )


if __name__ == '__main__':
    main()
