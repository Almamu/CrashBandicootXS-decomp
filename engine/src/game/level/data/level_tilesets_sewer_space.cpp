extern "C" {
#include "engine/gfx/tagged_asset.h"
}

/*
 * ROM 0x08270F08-0x082B91D0: level BG tile sets 4 and 5, the same
 * tag-0x00 raw 8bpp tile sets as src/game/level/data/level_tilesets_jungle_snow_underwater.cpp (see
 * there). Linked in ROM order between data/data.s sections by
 * games/xs/ldscript.txt - see docs/data.md.
 */

/* Tile set 4: 2619 tiles, 7 rooms. */
#include "tilesets/gSewerBg0Tiles.h"
extern "C" const TAGGED_RAW_ASSET(TILES_SIZE_gSewerBg0Tiles) gSewerBg0Tiles = {
    TAGGED_RAW_HEADER(TILES_SIZE_gSewerBg0Tiles),
    {
#include "tilesets/gSewerBg0Tiles.bin.inc"
    },
};

/* Tile set 5: 2000 tiles, 9 rooms. */
#include "tilesets/gSpaceBg0Tiles.h"
extern "C" const TAGGED_RAW_ASSET(TILES_SIZE_gSpaceBg0Tiles) gSpaceBg0Tiles = {
    TAGGED_RAW_HEADER(TILES_SIZE_gSpaceBg0Tiles),
    {
#include "tilesets/gSpaceBg0Tiles.bin.inc"
    },
};
