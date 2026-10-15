"""tools/split_entities.py: the command line of crashdata.graphics.split_entities (#1167)."""
import os

from crashdata.graphics import split_entities


def main():
    split_entities.split_family(
        sheet_rom_addr=0x080B2120, decompressed_size=213064,
        sheet_dir=os.path.join(split_entities.GRAPHICS_ROOT, 'polar', 'sprite_sheet'),
        anim_table_base=0x081796CC, record_slot_count=41, data_end=0x0817A6B8,
        names=split_entities.POLAR_NAMES, family_palette=0x08178F80,
    )
    split_entities.split_family(
        sheet_rom_addr=0x0814174C, decompressed_size=207124,
        sheet_dir=os.path.join(split_entities.GRAPHICS_ROOT, 'jetpack', 'sprite_sheet'),
        anim_table_base=0x0817B2A4, record_slot_count=47, data_end=0x0817C1C0,
        names=split_entities.JETPACK_NAMES, family_palette=0x0817AAA4,
        own_palettes={slot: 0x0817AEA4 for slot in split_entities.HOVERCRAFT_SLOTS},
    )


if __name__ == '__main__':
    main()
