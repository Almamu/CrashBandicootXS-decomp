#include "core.h"

/*
 * ROM 0x080C2758-0x080FF1B0: two zero-run-compressed OBJ frame sets (the
 * old "rotation strips A and B"). Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md ("Compressed sprite frames").
 *
 * Each frame is a {w, h, 0x30, 0} header and a stream of u16 run counts:
 * a zero run, then alternately a literal run (count + that many halfwords
 * of 4bpp tile data) and a zero run, until w*h tiles are filled. The IWRAM
 * decoder behind gUnpackRleSpriteFrameFunc (0x03000634) unpacks one frame into a
 * VRAM tile block (polar_player.c, jetpack_spawn.c,
 * company_logos.cpp). The frames are stored back to back; the frame
 * pointer tables point at their headers.
 */

#include "rle_sprites/0c2758_frames.h"
#include "rle_sprites/0da1d8_frames.h"

/* Categories 0-2's animation record 0 (table_B gPolarPlayerFrames):
 * 152 frames of 8x8 tiles (64x64), Crash riding the polar bear. Built from
 * graphics/rle_sprites/0c2758_frames.png. */
const u8 gPolarPlayerRleFrames[RLE_SPRITES_0C2758_SIZE] = {
#include "rle_sprites/0c2758_frames.inc"
};

/* The CreateYeti singleton's frames (table_B gYetiFrames,
 * frame_table_17a880.c): 112 frames of 10x10 tiles (80x80), the yeti.
 * Built from graphics/rle_sprites/0da1d8_frames.png. */
const u8 gYetiRleFrames[RLE_SPRITES_0DA1D8_SIZE] = {
#include "rle_sprites/0da1d8_frames.inc"
};
