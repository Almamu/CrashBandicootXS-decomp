extern "C" {
#include "gba/types.h"
}

/*
 * N-Tranced's ROM 0x08106C58: the framed OBJ sprite sheet of the actor categories
 * of its space levels (Run from the Sun, Spaced Out), LZ77 (gbagfx), as XS's polar and jetpack
 * families' (docs/graphics.md, "The framed sheets"). Built from
 * games/n2/graphics/vehicles/space/sprite_sheet/: its frames in the sheet's
 * order, one folder until the categories that use them are known
 * (docs/n_tranced.md, "Graphics").
 */

extern const u8 gSpaceSpriteSheet[] __attribute__((aligned(4))) = {
#include "vehicles/space/sprite_sheet.bin.lz.inc"
};
