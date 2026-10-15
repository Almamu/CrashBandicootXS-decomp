/* N-Tranced BG1-3 tile sets (4bpp, LZ77-packed), which games/n2/levels.mk
 * makes from the rooms' BG1-3 pictures (tools/levels.py tileset), back to
 * back as in the ROM. Their names are placeholders until their themes are
 * known. See docs/n_tranced.md, "Levels". */

extern "C" {
#include "gba/types.h"
}

extern const u8 gVolcanoBgTiles[] __attribute__((aligned(4))) = {
#include "tilesets/gVolcanoBgTiles.bin.lz.inc"
};

extern const u8 gIslandBgTiles[] __attribute__((aligned(4))) = {
#include "tilesets/gIslandBgTiles.bin.lz.inc"
};
