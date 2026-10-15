/* N-Tranced BG1-3 tile sets (4bpp, LZ77-packed), which games/n2/levels.mk
 * makes from the rooms' BG1-3 pictures (tools/levels.py tileset), back to
 * back as in the ROM. Their names are placeholders until their themes are
 * known. See docs/n_tranced.md, "Levels". */

extern "C" {
#include "gba/types.h"
}

extern const u8 gStaticData_0853FD10[] __attribute__((aligned(4))) = {
#include "tilesets/gStaticData_0853FD10.bin.lz.inc"
};

extern const u8 gStaticData_0854309C[] __attribute__((aligned(4))) = {
#include "tilesets/gStaticData_0854309C.bin.lz.inc"
};
