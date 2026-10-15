#include "game/ui/frontend.hpp"

extern "C" {
#include "engine/gfx/tagged_asset.h"
}

/*
 * ROM 0x0817E78C-0x0824B638: the first 0x20 bytes of the old
 * gLanguageSelectPalette3 blob, then level BG tile sets 1-3. Linked in ROM
 * order between data/data.s sections by games/xs/ldscript.txt - see docs/data.md
 * and docs/data_map.md ("gLanguageSelectPalette3").
 */

/* 16 BGR555 colours: InitLanguageSelectGraphics (language_select.cpp) copies them
 * into tile-asset cache slot 2 (+0x20), after its three siblings
 * gLanguageSelectPalette0/1/2 (language_select_palettes.cpp). */
const u16 gLanguageSelectPalette3[16] = {
    0x0000, 0x001F, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/*
 * Level BG tile sets: tag-0x00 raw tagged assets of 8bpp tiles (64 bytes
 * each). A room's struct level_layer_desc (BgLayer::Load, bg_layer.cpp)
 * points at one through `tileData`; the pooled layer 0 (tile_slot_pool.cpp,
 * SetTileSlotPoolSource) reads the tiles at `tileData + 4`. The tiles
 * aren't a source: tools/levels.py tileset generates each set from the
 * pictures of the room layers that name it (docs/levels.md, "Tile
 * sets"), with its size in tilesets/<set>.h, and tools/bin2c.py writes the
 * bytes (levels.mk).
 */

/* Tile set 1: 6638 tiles, used by 10 rooms' layer descriptors. */
#include "tilesets/gJungleBg0Tiles.h"
extern const TAGGED_RAW_ASSET(TILES_SIZE_gJungleBg0Tiles) gJungleBg0Tiles = {
    TAGGED_RAW_HEADER(TILES_SIZE_gJungleBg0Tiles),
    {
#include "tilesets/gJungleBg0Tiles.bin.inc"
    },
};

/* Tile set 2: 1707 tiles, 10 rooms. */
#include "tilesets/gSnowBg0Tiles.h"
extern const TAGGED_RAW_ASSET(TILES_SIZE_gSnowBg0Tiles) gSnowBg0Tiles = {
    TAGGED_RAW_HEADER(TILES_SIZE_gSnowBg0Tiles),
    {
#include "tilesets/gSnowBg0Tiles.bin.inc"
    },
};

/* Tile set 3: 4769 tiles, 5 rooms. */
#include "tilesets/gUnderwaterBg0Tiles.h"
extern const TAGGED_RAW_ASSET(TILES_SIZE_gUnderwaterBg0Tiles) gUnderwaterBg0Tiles = {
    TAGGED_RAW_HEADER(TILES_SIZE_gUnderwaterBg0Tiles),
    {
#include "tilesets/gUnderwaterBg0Tiles.bin.inc"
    },
};
