#include "gba/types.h"

/*
 * ROM 0x082BF120-0x084A5600: the sprite tile pool and the 125 fixed OBJ
 * tiles. Linked in ROM order between data/data.s sections by ldscript.txt -
 * see docs/data.md and docs/data_map.md ("gStaticData_0817E78C").
 *
 * The sprite-bank table gSpriteBankTable (sprite_banks_4a5600.c) starts
 * with {banks, tileBase = gSpriteBank00Tiles, tilePool =
 * gFixedObjTiles, 56, 125}. GetSpriteTileBase returns tileBase, and
 * graphics_73dc.c/graphics_7634.c upload a frame's pieces from tileBase +
 * (frame.tiles & 0xFFFFFF). The 56 banks own disjoint, back-to-back ranges
 * of that pool in bank order (banks 42 and 47 also reuse one frame of bank
 * 0's), so each bank is one array below, labeled with its ROM address. The
 * only pointer into the pool is the header's tileBase; the frames hold
 * offsets from it, written as SPRITE_TILES_BANKnn + offset
 * (include/sprite_bank.h), so resizing a bank here means updating those.
 *
 * Raw 4bpp (32-byte) OBJ tiles, no header. The bytes come from
 * graphics/sprites/bankNN_<addr>.png via graphics.mk (grit -gt -gB4 -p!,
 * then tools/bin2c.py); tools/tile_pools.py extracted the PNGs. Each PNG is
 * as wide as the bank's most common OBJ piece, so with the 1D OBJ mapping
 * those pieces read correctly. Their palette is a grayscale placeholder:
 * OAM picks the palette per actor at run time, and no bank is tied to one.
 * A part drawn in 8bpp mode (part flag 28) looks scrambled in this 4bpp
 * view, but round-trips all the same.
 */

/* Bank 0: 7807 tiles (48 animations, 465 frames: probably the player). */
const u8 gSpriteBank00Tiles[0x3cfe0] = {
#include "sprites/bank00_2bf120.img.bin.inc"
};

/* Bank 1: 5548 tiles. */
const u8 gSpriteBank01Tiles[0x2b580] = {
#include "sprites/bank01_2fc100.img.bin.inc"
};

/* Bank 2: 979 tiles. */
const u8 gSpriteBank02Tiles[0x7a60] = {
#include "sprites/bank02_327680.img.bin.inc"
};

/* Bank 3: 827 tiles. */
const u8 gSpriteBank03Tiles[0x6760] = {
#include "sprites/bank03_32f0e0.img.bin.inc"
};

/* Bank 4: 1017 tiles. */
const u8 gSpriteBank04Tiles[0x7f20] = {
#include "sprites/bank04_335840.img.bin.inc"
};

/* Bank 5: 414 tiles. */
const u8 gSpriteBank05Tiles[0x33c0] = {
#include "sprites/bank05_33d760.img.bin.inc"
};

/* Bank 6: 158 tiles. */
const u8 gSpriteBank06Tiles[0x13c0] = {
#include "sprites/bank06_340b20.img.bin.inc"
};

/* Bank 7: 902 tiles. */
const u8 gSpriteBank07Tiles[0x70c0] = {
#include "sprites/bank07_341ee0.img.bin.inc"
};

/* Bank 8: 438 tiles. */
const u8 gSpriteBank08Tiles[0x36c0] = {
#include "sprites/bank08_348fa0.img.bin.inc"
};

/* Bank 9: 165 tiles. */
const u8 gSpriteBank09Tiles[0x14a0] = {
#include "sprites/bank09_34c660.img.bin.inc"
};

/* Bank 10: 774 tiles. */
const u8 gSpriteBank10Tiles[0x60c0] = {
#include "sprites/bank10_34db00.img.bin.inc"
};

/* Bank 11: 723 tiles. */
const u8 gSpriteBank11Tiles[0x5a60] = {
#include "sprites/bank11_353bc0.img.bin.inc"
};

/* Bank 12: 1381 tiles. */
const u8 gSpriteBank12Tiles[0xaca0] = {
#include "sprites/bank12_359620.img.bin.inc"
};

/* Bank 13: 263 tiles. */
const u8 gSpriteBank13Tiles[0x20e0] = {
#include "sprites/bank13_3642c0.img.bin.inc"
};

/* Bank 14: 618 tiles. */
const u8 gSpriteBank14Tiles[0x4d40] = {
#include "sprites/bank14_3663a0.img.bin.inc"
};

/* Bank 15: 754 tiles. */
const u8 gSpriteBank15Tiles[0x5e40] = {
#include "sprites/bank15_36b0e0.img.bin.inc"
};

/* Bank 16: 714 tiles. */
const u8 gSpriteBank16Tiles[0x5940] = {
#include "sprites/bank16_370f20.img.bin.inc"
};

/* Bank 17: 301 tiles. */
const u8 gSpriteBank17Tiles[0x25a0] = {
#include "sprites/bank17_376860.img.bin.inc"
};

/* Bank 18: 471 tiles. */
const u8 gSpriteBank18Tiles[0x3ae0] = {
#include "sprites/bank18_378e00.img.bin.inc"
};

/* Bank 19: 502 tiles. */
const u8 gSpriteBank19Tiles[0x3ec0] = {
#include "sprites/bank19_37c8e0.img.bin.inc"
};

/* Bank 20: 287 tiles. */
const u8 gSpriteBank20Tiles[0x23e0] = {
#include "sprites/bank20_3807a0.img.bin.inc"
};

/* Bank 21: 192 tiles. */
const u8 gSpriteBank21Tiles[0x1800] = {
#include "sprites/bank21_382b80.img.bin.inc"
};

/* Bank 22: 724 tiles. */
const u8 gSpriteBank22Tiles[0x5a80] = {
#include "sprites/bank22_384380.img.bin.inc"
};

/* Bank 23: 2542 tiles. */
const u8 gSpriteBank23Tiles[0x13dc0] = {
#include "sprites/bank23_389e00.img.bin.inc"
};

/* Bank 24: 1707 tiles. */
const u8 gSpriteBank24Tiles[0xd560] = {
#include "sprites/bank24_39dbc0.img.bin.inc"
};

/* Bank 25: 964 tiles. */
const u8 gSpriteBank25Tiles[0x7880] = {
#include "sprites/bank25_3ab120.img.bin.inc"
};

/* Bank 26: 468 tiles. */
const u8 gSpriteBank26Tiles[0x3a80] = {
#include "sprites/bank26_3b29a0.img.bin.inc"
};

/* Bank 27: 275 tiles. */
const u8 gSpriteBank27Tiles[0x2260] = {
#include "sprites/bank27_3b6420.img.bin.inc"
};

/* Bank 28: 24 tiles. */
const u8 gSpriteBank28Tiles[0x300] = {
#include "sprites/bank28_3b8680.img.bin.inc"
};

/* Bank 29: 1532 tiles. */
const u8 gSpriteBank29Tiles[0xbf80] = {
#include "sprites/bank29_3b8980.img.bin.inc"
};

/* Bank 30: 3238 tiles. */
const u8 gSpriteBank30Tiles[0x194c0] = {
#include "sprites/bank30_3c4900.img.bin.inc"
};

/* Bank 31: 2605 tiles. */
const u8 gSpriteBank31Tiles[0x145a0] = {
#include "sprites/bank31_3dddc0.img.bin.inc"
};

/* Bank 32: 102 tiles. */
const u8 gSpriteBank32Tiles[0xcc0] = {
#include "sprites/bank32_3f2360.img.bin.inc"
};

/* Bank 33: 224 tiles. */
const u8 gSpriteBank33Tiles[0x1c00] = {
#include "sprites/bank33_3f3020.img.bin.inc"
};

/* Bank 34: 127 tiles. */
const u8 gSpriteBank34Tiles[0xfe0] = {
#include "sprites/bank34_3f4c20.img.bin.inc"
};

/* Bank 35: 178 tiles. */
const u8 gSpriteBank35Tiles[0x1640] = {
#include "sprites/bank35_3f5c00.img.bin.inc"
};

/* Bank 36: 69 tiles. */
const u8 gSpriteBank36Tiles[0x8a0] = {
#include "sprites/bank36_3f7240.img.bin.inc"
};

/* Bank 37: 80 tiles. */
const u8 gSpriteBank37Tiles[0xa00] = {
#include "sprites/bank37_3f7ae0.img.bin.inc"
};

/* Bank 38: 680 tiles. */
const u8 gSpriteBank38Tiles[0x5500] = {
#include "sprites/bank38_3f84e0.img.bin.inc"
};

/* Bank 39: 820 tiles. */
const u8 gSpriteBank39Tiles[0x6680] = {
#include "sprites/bank39_3fd9e0.img.bin.inc"
};

/* Bank 40: 385 tiles. */
const u8 gSpriteBank40Tiles[0x3020] = {
#include "sprites/bank40_404060.img.bin.inc"
};

/* Bank 41: 88 tiles. */
const u8 gSpriteBank41Tiles[0xb00] = {
#include "sprites/bank41_407080.img.bin.inc"
};

/* Bank 42: 31 tiles. */
const u8 gSpriteBank42Tiles[0x3e0] = {
#include "sprites/bank42_407b80.img.bin.inc"
};

/* Bank 43: 493 tiles. */
const u8 gSpriteBank43Tiles[0x3da0] = {
#include "sprites/bank43_407f60.img.bin.inc"
};

/* Bank 44: 51 tiles. */
const u8 gSpriteBank44Tiles[0x660] = {
#include "sprites/bank44_40bd00.img.bin.inc"
};

/* Bank 45: 416 tiles. */
const u8 gSpriteBank45Tiles[0x3400] = {
#include "sprites/bank45_40c360.img.bin.inc"
};

/* Bank 46: 10 tiles. */
const u8 gSpriteBank46Tiles[0x140] = {
#include "sprites/bank46_40f760.img.bin.inc"
};

/* Bank 47: 246 tiles. */
const u8 gSpriteBank47Tiles[0x1ec0] = {
#include "sprites/bank47_40f8a0.img.bin.inc"
};

/* Bank 48: 1024 tiles. */
const u8 gSpriteBank48Tiles[0x8000] = {
#include "sprites/bank48_411760.img.bin.inc"
};

/* Bank 49: 28 tiles. */
const u8 gSpriteBank49Tiles[0x380] = {
#include "sprites/bank49_419760.img.bin.inc"
};

/* Bank 50: 309 tiles. */
const u8 gSpriteBank50Tiles[0x26a0] = {
#include "sprites/bank50_419ae0.img.bin.inc"
};

/* Bank 51: 106 tiles. */
const u8 gSpriteBank51Tiles[0xd40] = {
#include "sprites/bank51_41c180.img.bin.inc"
};

/* Bank 52: 16 tiles. */
const u8 gSpriteBank52Tiles[0x200] = {
#include "sprites/bank52_41cec0.img.bin.inc"
};

/* Bank 53: 2078 tiles. */
const u8 gSpriteBank53Tiles[0x103c0] = {
#include "sprites/bank53_41d0c0.img.bin.inc"
};

/* Bank 54: 7913 tiles. */
const u8 gSpriteBank54Tiles[0x3dd20] = {
#include "sprites/bank54_42d480.img.bin.inc"
};

/* Bank 55: 7334 tiles. */
const u8 gSpriteBank55Tiles[0x394c0] = {
#include "sprites/bank55_46b1a0.img.bin.inc"
};

/* The 125 fixed 4bpp tiles, the header's tilePool/npool: RunPauseMenu
 * builds the 125-slot tile-asset cache (sub_8006EF0/sub_8006DF8) from them. */
const u8 gFixedObjTiles[0xfa0] = {
#include "sprites/tile_pool_4a4660.img.bin.inc"
};
