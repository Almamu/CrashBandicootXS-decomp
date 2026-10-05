#include "tagged_asset.h"

/*
 * ROM 0x0817E78C-0x0824B638: the first 0x20 bytes of the old
 * gLanguageSelectPalette3 blob, then level BG tile sets 1-3. Linked in ROM
 * order between data/data.s sections by ldscript.txt - see docs/data.md
 * and docs/data_map.md ("gLanguageSelectPalette3").
 */

/* 16 BGR555 colours: InitLanguageSelectGraphics (counter_selector_icons.c) copies them
 * into tile-asset cache slot 2 (+0x20), after its three siblings
 * gLanguageSelectPalette0/1/2 (palettes_17e72c.c). */
const u16 gLanguageSelectPalette3[16] = {
    0x0000, 0x001F, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/*
 * Level BG tile sets: tag-0x00 raw tagged assets of 8bpp tiles (64 bytes
 * each). A room's struct bg_layer_desc (bg_scroll_layer_25fc8.c) points at
 * one through `tileData`; the pooled layer 0 (tile_slot_pool.c,
 * SetTileSlotPoolSource) reads the tiles at `tileData + 4`. The tile bytes come from
 * graphics/level_tilesets/tilesetN_<addr>.png via graphics.mk
 * (grit -gt -gB8 -p!, then tools/bin2c.py); tools/tile_pools.py extracted
 * the PNGs, with the palette of the first room that uses each set.
 */

/* Tile set 1: 6638 tiles, used by 10 rooms' layer descriptors. */
const TAGGED_RAW_ASSET(0x67B80) gStaticData_0817E7AC = {
    TAGGED_RAW_HEADER(0x67B80),
    {
#include "level_tilesets/tileset1_17e7ac.img.bin.inc"
    },
};

/* Tile set 2: 1707 tiles, 10 rooms. */
const TAGGED_RAW_ASSET(0x1AAC0) gStaticData_081E6330 = {
    TAGGED_RAW_HEADER(0x1AAC0),
    {
#include "level_tilesets/tileset2_1e6330.img.bin.inc"
    },
};

/* Tile set 3: 4769 tiles, 5 rooms. */
const TAGGED_RAW_ASSET(0x4A840) gStaticData_08200DF4 = {
    TAGGED_RAW_HEADER(0x4A840),
    {
#include "level_tilesets/tileset3_200df4.img.bin.inc"
    },
};
