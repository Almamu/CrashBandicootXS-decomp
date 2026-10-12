extern "C" {
#include "core.h"
}

/*
 * ROM 0x0815A050-0x08167AD4: a zero-run-compressed OBJ frame set (the old
 * "rotation strip C"), same format as rle_sprites_0c2758.cpp. Linked in ROM
 * order between data/data.s sections by ldscript.txt - see docs/data.md
 * ("Compressed sprite frames").
 */

#include "vehicles/jetpack/player.h"

/* Categories 3-6's animation record 0 (table_B gJetpackPlayerFrames):
 * 80 frames of 8x8 tiles (64x64). Built from
 * graphics/vehicles/jetpack/player.png. */
extern const u8 gJetpackPlayerRleFrames[RLE_SPRITES_JETPACK_PLAYER_SIZE] = {
#include "vehicles/jetpack/player.inc"
};
