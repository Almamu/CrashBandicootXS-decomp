#include "core.h"

/*
 * ROM 0x0815A050-0x08167AD4: a zero-run-compressed OBJ frame set (the old
 * "rotation strip C"), same format as rle_sprites_0c2758.c. Linked in ROM
 * order between data/data.s sections by ldscript.txt - see docs/data.md
 * ("Compressed sprite frames").
 */

#include "rle_sprites/15a050_frames.h"

/* Categories 3-6's animation record 0 (table_B gJetpackPlayerFrames):
 * 80 frames of 8x8 tiles (64x64). Built from
 * graphics/rle_sprites/15a050_frames.png. */
const u8 gJetpackPlayerRleFrames[RLE_SPRITES_15A050_SIZE] = {
#include "rle_sprites/15a050_frames.inc"
};
