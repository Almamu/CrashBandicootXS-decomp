/* N-Tranced BG1-3 tile sets (4bpp, LZ77-packed), which games/n2/levels.mk
 * makes from the rooms' BG1-3 pictures (tools/levels.py tileset), back to
 * back as in the ROM. Their names are placeholders until their themes are
 * known. See docs/n_tranced.md, "Levels". */

extern "C" {
#include "gba/types.h"
}

extern const u8 gArabiaBgTiles[] __attribute__((aligned(4))) = {
#include "tilesets/gArabiaBgTiles.bin.lz.inc"
};

extern const u8 gArabiaSkyBgTiles[] __attribute__((aligned(4))) = {
#include "tilesets/gArabiaSkyBgTiles.bin.lz.inc"
};

extern const u8 gEgyptBgTiles[] __attribute__((aligned(4))) = {
#include "tilesets/gEgyptBgTiles.bin.lz.inc"
};
