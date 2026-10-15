extern "C" {
#include "gba/types.h"
}

/*
 * N-Tranced's ROM 0x080BF458: the framed OBJ sprite sheet of the actor categories
 * of its water levels (Lagoony Tunes, Flockful of Seagulls, Water Logged), LZ77 (gbagfx), as XS's polar and jetpack
 * families' (docs/graphics.md, "The framed sheets"). Built from
 * games/n2/graphics/vehicles/ocean/sprite_sheet/: its frames in the sheet's
 * order, one folder until the categories that use them are known
 * (docs/n_tranced.md, "Graphics").
 */

extern const u8 gOceanSpriteSheet[] __attribute__((aligned(4))) = {
#include "vehicles/ocean/sprite_sheet.bin.lz.inc"
};
