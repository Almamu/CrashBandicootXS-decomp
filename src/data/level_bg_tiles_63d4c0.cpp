extern "C" {
#include "gba/types.h"
}

/*
 * ROM 0x0863D4C0-0x0864F830: the six 4bpp tile sets of the level BG1-3
 * layers, which the rooms' layer descriptors name (docs/graphics.md, "Level
 * BG tile sets"). Each PNG is padded to whole rows of tiles, which
 * tools/bin2c.py --size trims before gbagfx compresses it.
 * Every asset is an LZ77 stream (gbagfx) that starts on a word boundary,
 * hence aligned(4): the 0-3 zero bytes after a stream are the next one's
 * alignment. tools/bin2c.py --lz writes the stream's bytes (graphics.mk).
 * Linked in ROM order between data/data.s sections by ldscript.txt, see
 * docs/data.md.
 */

/* 4bpp tile set of the level BG1-3 layers (snow rooms): graphics/level_bg_tiles/63d4c0_snow.png. */
extern const u8 gSnowBgTiles[] __attribute__((aligned(4))) = {
#include "level_bg_tiles/63d4c0_snow.tiles.bin.lz.inc"
};

/*
 * 4bpp tile set of the level BG1-3 layers (jungle rooms):
 * graphics/level_bg_tiles/640ef8_jungle.png.
 */
extern const u8 gJungleBgTiles[] __attribute__((aligned(4))) = {
#include "level_bg_tiles/640ef8_jungle.tiles.bin.lz.inc"
};

/*
 * 4bpp tile set of the level BG1-3 layers (temple rooms):
 * graphics/level_bg_tiles/644a88_temple.png.
 */
extern const u8 gTempleBgTiles[] __attribute__((aligned(4))) = {
#include "level_bg_tiles/644a88_temple.tiles.bin.lz.inc"
};

/*
 * 4bpp tile set of the level BG1-3 layers (sewer rooms):
 * graphics/level_bg_tiles/649418_sewer.png.
 */
extern const u8 gSewerBgTiles[] __attribute__((aligned(4))) = {
#include "level_bg_tiles/649418_sewer.tiles.bin.lz.inc"
};

/*
 * 4bpp tile set of the level BG1-3 layers (space rooms):
 * graphics/level_bg_tiles/64bb5c_space.png.
 */
extern const u8 gSpaceBgTiles[] __attribute__((aligned(4))) = {
#include "level_bg_tiles/64bb5c_space.tiles.bin.lz.inc"
};

/*
 * 4bpp tile set of the level BG1-3 layers (underwater rooms):
 * graphics/level_bg_tiles/64d7e8_underwater.png.
 */
extern const u8 gUnderwaterBgTiles[] __attribute__((aligned(4))) = {
#include "level_bg_tiles/64d7e8_underwater.tiles.bin.lz.inc"
};
