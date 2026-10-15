extern "C" {
#include "gba/types.h"
}

/*
 * ROM 0x0863D4C0-0x0864F830: the six 4bpp tile sets of the level BG1-3
 * layers, which the rooms' layer descriptors name. They aren't a source:
 * tools/levels.py tileset generates each set from the pictures of the
 * room layers that name it (docs/levels.md, "Tile sets"), and gbagfx
 * compresses it.
 * Every asset is an LZ77 stream (gbagfx) that starts on a word boundary,
 * hence aligned(4): the 0-3 zero bytes after a stream are the next one's
 * alignment. tools/bin2c.py --lz writes the stream's bytes (levels.mk).
 * Linked in ROM order between data/data.s sections by games/xs/ldscript.txt, see
 * docs/data.md.
 */

/* 4bpp tile set of the level BG1-3 layers (snow rooms). */
extern const u8 gSnowBgTiles[] __attribute__((aligned(4))) = {
#include "tilesets/gSnowBgTiles.bin.lz.inc"
};

/* 4bpp tile set of the level BG1-3 layers (jungle rooms). */
extern const u8 gJungleBgTiles[] __attribute__((aligned(4))) = {
#include "tilesets/gJungleBgTiles.bin.lz.inc"
};

/* 4bpp tile set of the level BG1-3 layers (temple rooms). */
extern const u8 gTempleBgTiles[] __attribute__((aligned(4))) = {
#include "tilesets/gTempleBgTiles.bin.lz.inc"
};

/* 4bpp tile set of the level BG1-3 layers (sewer rooms). */
extern const u8 gSewerBgTiles[] __attribute__((aligned(4))) = {
#include "tilesets/gSewerBgTiles.bin.lz.inc"
};

/* 4bpp tile set of the level BG1-3 layers (space rooms). */
extern const u8 gSpaceBgTiles[] __attribute__((aligned(4))) = {
#include "tilesets/gSpaceBgTiles.bin.lz.inc"
};

/* 4bpp tile set of the level BG1-3 layers (underwater rooms). */
extern const u8 gUnderwaterBgTiles[] __attribute__((aligned(4))) = {
#include "tilesets/gUnderwaterBgTiles.bin.lz.inc"
};
