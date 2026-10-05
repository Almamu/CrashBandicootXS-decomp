#include "core.h"
#include "sprite_bank.h"

/*
 * ROM 0x0816B2E0-0x0816B304. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* The OBJ shape/size index of a sprite piece (the low nibble of its piece
 * byte, include/sprite_bank.h) as width and height in pixels: square
 * 8-64, horizontal 16x8-64x32, vertical 8x16-32x64. Read by DrawSpritePieces
 * (sprite_pieces.c) and DrawAffineSpritePieces (affine_sprite_pieces.c). */
const u8 gObjPieceWidths[12] = {
    8, 16, 32, 64, 16, 32, 32, 64, 8, 8, 16, 32,
};

const u8 gObjPieceHeights[12] = {
    8, 16, 32, 64, 8, 8, 16, 32, 16, 32, 32, 64,
};

/* The box and the point used when a sprite frame has none (the layout
 * types without one, include/sprite_bank.h): all zero. */
const struct sprite_box gEmptySpriteBox = { 0 };
const struct sprite_point gEmptySpritePoint = { 0 };
