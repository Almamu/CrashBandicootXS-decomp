#include "tagged_asset.h"

/*
 * ROM 0x08270F08-0x082B91D0: level BG tile sets 4 and 5, the same
 * tag-0x00 raw 8bpp tile sets as src/data/level_tilesets_17e78c.c (see
 * there). Linked in ROM order between data/data.s sections by
 * ldscript.txt - see docs/data.md.
 */

/* Tile set 4: 2619 tiles, 7 rooms. */
const TAGGED_RAW_ASSET(0x28EC0) gSewerBg0Tiles = {
    TAGGED_RAW_HEADER(0x28EC0),
    {
#include "level_tilesets/tileset4_270f08.img.bin.inc"
    },
};

/* Tile set 5: 2000 tiles, 9 rooms. */
const TAGGED_RAW_ASSET(0x1F400) gSpaceBg0Tiles = {
    TAGGED_RAW_HEADER(0x1F400),
    {
#include "level_tilesets/tileset5_299dcc.img.bin.inc"
    },
};
