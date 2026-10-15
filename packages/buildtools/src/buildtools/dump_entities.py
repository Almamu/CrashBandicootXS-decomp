"""tools/dump_entities.py: the command line of crashdata.graphics.dump_entities (#1167)."""
import json
import os

from crashdata.graphics import dump_entities


def main():
    family1 = dump_entities.dump_family(
        sheet_rom_addr=0x080B2120,
        decompressed_size=213064,
        sheet_dir=os.path.join(dump_entities.GRAPHICS_ROOT, 'polar', 'sprite_sheet'),
        anim_table_base=0x081796CC,
        category_indices=[0, 1, 2],
        record_slot_count=41,
        labels=dump_entities.FAMILY1_LABELS,
    )
    os.makedirs(dump_entities.DOCS_ROOT, exist_ok=True)
    out1 = os.path.join(dump_entities.DOCS_ROOT, 'polar.json')
    with open(out1, 'w') as f:
        json.dump(family1, f, indent=2)
        f.write('\n')
    print('wrote', out1)

    family2 = dump_entities.dump_family(
        sheet_rom_addr=0x0814174C,
        decompressed_size=207124,
        sheet_dir=os.path.join(dump_entities.GRAPHICS_ROOT, 'jetpack', 'sprite_sheet'),
        anim_table_base=0x0817B2A4,
        category_indices=[3, 4, 5, 6],
        record_slot_count=47,
        labels=dump_entities.FAMILY2_LABELS,
    )
    out2 = os.path.join(dump_entities.DOCS_ROOT, 'jetpack.json')
    with open(out2, 'w') as f:
        json.dump(family2, f, indent=2)
        f.write('\n')
    print('wrote', out2)


if __name__ == '__main__':
    main()
