extern "C" {
#include "gba/types.h"
}
#include "sprites/sprite_frames.h"

/*
 * ROM 0x082BF120-0x084A5600: the sprite tile pool and the 125 OBJ
 * palettes. Linked in ROM order between data/data.s sections by ldscript.txt -
 * see docs/data.md and docs/data_map.md ("gLanguageSelectPalette3").
 *
 * The sprite-bank table gSpriteBankTable (sprite_banks_00_09.cpp) starts
 * with {banks, tileBase = gSpriteBank00Tiles, palettes =
 * gObjPalettes, 56, 125}. GetSpriteTileBase returns tileBase, and
 * sprite_renderer.cpp's DrawPieces/DrawAffinePieces upload a frame's pieces
 * from tileBase + (frame.tiles & 0xFFFFFF). Each of the 56 banks has its
 * own range of that pool, back to back in bank order, so each bank is one
 * array below. The only pointer into the pool is the header's tileBase; the
 * frames hold offsets from it, SPRITE_TILES_BANKnn_FRAMEkkk.
 *
 * Raw 4bpp (32-byte) OBJ tiles, no header. tools/sprite_frames.py pack
 * builds them from graphics/sprites/bankNN_<name>/ (a PNG per frame and
 * bank.json, see docs/graphics.md "Sprite banks"), and writes
 * sprites/sprite_frames.h with each bank's offset and size and each frame's
 * tile offset. So a bank can grow or shrink: the offsets follow.
 */

/* Bank 0: graphics/sprites/bank00_crash/. */
extern const u8 gSpriteBank00Tiles[SPRITE_TILES_BANK00_SIZE] = {
#include "sprites/bank00_tiles.inc"
};

/* Bank 1: graphics/sprites/bank01_crash_underwater/. */
extern const u8 gSpriteBank01Tiles[SPRITE_TILES_BANK01_SIZE] = {
#include "sprites/bank01_tiles.inc"
};

/* Bank 2: graphics/sprites/bank02_crash_hover/. */
extern const u8 gSpriteBank02Tiles[SPRITE_TILES_BANK02_SIZE] = {
#include "sprites/bank02_tiles.inc"
};

/* Bank 3: graphics/sprites/bank03_moray_eel/. */
extern const u8 gSpriteBank03Tiles[SPRITE_TILES_BANK03_SIZE] = {
#include "sprites/bank03_tiles.inc"
};

/* Bank 4: graphics/sprites/bank04_shark/. */
extern const u8 gSpriteBank04Tiles[SPRITE_TILES_BANK04_SIZE] = {
#include "sprites/bank04_tiles.inc"
};

/* Bank 5: graphics/sprites/bank05_pufferfish/. */
extern const u8 gSpriteBank05Tiles[SPRITE_TILES_BANK05_SIZE] = {
#include "sprites/bank05_tiles.inc"
};

/* Bank 6: graphics/sprites/bank06_sea_mine/. */
extern const u8 gSpriteBank06Tiles[SPRITE_TILES_BANK06_SIZE] = {
#include "sprites/bank06_tiles.inc"
};

/* Bank 7: graphics/sprites/bank07_squid/. */
extern const u8 gSpriteBank07Tiles[SPRITE_TILES_BANK07_SIZE] = {
#include "sprites/bank07_tiles.inc"
};

/* Bank 8: graphics/sprites/bank08_electric_eel/. */
extern const u8 gSpriteBank08Tiles[SPRITE_TILES_BANK08_SIZE] = {
#include "sprites/bank08_tiles.inc"
};

/* Bank 9: graphics/sprites/bank09_jellyfish/. */
extern const u8 gSpriteBank09Tiles[SPRITE_TILES_BANK09_SIZE] = {
#include "sprites/bank09_tiles.inc"
};

/* Bank 10: graphics/sprites/bank10_venus_flytrap/. */
extern const u8 gSpriteBank10Tiles[SPRITE_TILES_BANK10_SIZE] = {
#include "sprites/bank10_tiles.inc"
};

/* Bank 11: graphics/sprites/bank11_vulture/. */
extern const u8 gSpriteBank11Tiles[SPRITE_TILES_BANK11_SIZE] = {
#include "sprites/bank11_tiles.inc"
};

/* Bank 12: graphics/sprites/bank12_blowgun_tribesman/. */
extern const u8 gSpriteBank12Tiles[SPRITE_TILES_BANK12_SIZE] = {
#include "sprites/bank12_tiles.inc"
};

/* Bank 13: graphics/sprites/bank13_lizard/. */
extern const u8 gSpriteBank13Tiles[SPRITE_TILES_BANK13_SIZE] = {
#include "sprites/bank13_tiles.inc"
};

/* Bank 14: graphics/sprites/bank14_patrolling_jungle_enemy/. */
extern const u8 gSpriteBank14Tiles[SPRITE_TILES_BANK14_SIZE] = {
#include "sprites/bank14_tiles.inc"
};

/* Bank 15: graphics/sprites/bank15_penguin/. */
extern const u8 gSpriteBank15Tiles[SPRITE_TILES_BANK15_SIZE] = {
#include "sprites/bank15_tiles.inc"
};

/* Bank 16: graphics/sprites/bank16_polar_bear/. */
extern const u8 gSpriteBank16Tiles[SPRITE_TILES_BANK16_SIZE] = {
#include "sprites/bank16_tiles.inc"
};

/* Bank 17: graphics/sprites/bank17_seal/. */
extern const u8 gSpriteBank17Tiles[SPRITE_TILES_BANK17_SIZE] = {
#include "sprites/bank17_tiles.inc"
};

/* Bank 18: graphics/sprites/bank18_wooden_crusher/. */
extern const u8 gSpriteBank18Tiles[SPRITE_TILES_BANK18_SIZE] = {
#include "sprites/bank18_tiles.inc"
};

/* Bank 19: graphics/sprites/bank19_frog/. */
extern const u8 gSpriteBank19Tiles[SPRITE_TILES_BANK19_SIZE] = {
#include "sprites/bank19_tiles.inc"
};

/* Bank 20: graphics/sprites/bank20_patrolling_sewer_enemy/. */
extern const u8 gSpriteBank20Tiles[SPRITE_TILES_BANK20_SIZE] = {
#include "sprites/bank20_tiles.inc"
};

/* Bank 21: graphics/sprites/bank21_rat/. */
extern const u8 gSpriteBank21Tiles[SPRITE_TILES_BANK21_SIZE] = {
#include "sprites/bank21_tiles.inc"
};

/* Bank 22: graphics/sprites/bank22_homing_sewer_enemy/. */
extern const u8 gSpriteBank22Tiles[SPRITE_TILES_BANK22_SIZE] = {
#include "sprites/bank22_tiles.inc"
};

/* Bank 23: graphics/sprites/bank23_flamethrower_lab_assistant/. */
extern const u8 gSpriteBank23Tiles[SPRITE_TILES_BANK23_SIZE] = {
#include "sprites/bank23_tiles.inc"
};

/* Bank 24: graphics/sprites/bank24_patrolling_space_enemy/. */
extern const u8 gSpriteBank24Tiles[SPRITE_TILES_BANK24_SIZE] = {
#include "sprites/bank24_tiles.inc"
};

/* Bank 25: graphics/sprites/bank25_laser_barrier/. */
extern const u8 gSpriteBank25Tiles[SPRITE_TILES_BANK25_SIZE] = {
#include "sprites/bank25_tiles.inc"
};

/* Bank 26: graphics/sprites/bank26_piston_crusher/. */
extern const u8 gSpriteBank26Tiles[SPRITE_TILES_BANK26_SIZE] = {
#include "sprites/bank26_tiles.inc"
};

/* Bank 27: graphics/sprites/bank27_stationary_space_enemy/. */
extern const u8 gSpriteBank27Tiles[SPRITE_TILES_BANK27_SIZE] = {
#include "sprites/bank27_tiles.inc"
};

/* Bank 28: graphics/sprites/bank28_launch_pad/. */
extern const u8 gSpriteBank28Tiles[SPRITE_TILES_BANK28_SIZE] = {
#include "sprites/bank28_tiles.inc"
};

/* Bank 29: graphics/sprites/bank29_saucer_lab_assistant/. */
extern const u8 gSpriteBank29Tiles[SPRITE_TILES_BANK29_SIZE] = {
#include "sprites/bank29_tiles.inc"
};

/* Bank 30: graphics/sprites/bank30_mega_mix/. */
extern const u8 gSpriteBank30Tiles[SPRITE_TILES_BANK30_SIZE] = {
#include "sprites/bank30_tiles.inc"
};

/* Bank 31: graphics/sprites/bank31_crates/. */
extern const u8 gSpriteBank31Tiles[SPRITE_TILES_BANK31_SIZE] = {
#include "sprites/bank31_tiles.inc"
};

/* Bank 32: graphics/sprites/bank32_gems/. */
extern const u8 gSpriteBank32Tiles[SPRITE_TILES_BANK32_SIZE] = {
#include "sprites/bank32_tiles.inc"
};

/* Bank 33: graphics/sprites/bank33_trial_icons/. */
extern const u8 gSpriteBank33Tiles[SPRITE_TILES_BANK33_SIZE] = {
#include "sprites/bank33_tiles.inc"
};

/* Bank 34: graphics/sprites/bank34_aku_aku/. */
extern const u8 gSpriteBank34Tiles[SPRITE_TILES_BANK34_SIZE] = {
#include "sprites/bank34_tiles.inc"
};

/* Bank 35: graphics/sprites/bank35_wumpa/. */
extern const u8 gSpriteBank35Tiles[SPRITE_TILES_BANK35_SIZE] = {
#include "sprites/bank35_tiles.inc"
};

/* Bank 36: graphics/sprites/bank36_stopwatch/. */
extern const u8 gSpriteBank36Tiles[SPRITE_TILES_BANK36_SIZE] = {
#include "sprites/bank36_tiles.inc"
};

/* Bank 37: graphics/sprites/bank37_crystal/. */
extern const u8 gSpriteBank37Tiles[SPRITE_TILES_BANK37_SIZE] = {
#include "sprites/bank37_tiles.inc"
};

/* Bank 38: graphics/sprites/bank38_powers/. */
extern const u8 gSpriteBank38Tiles[SPRITE_TILES_BANK38_SIZE] = {
#include "sprites/bank38_tiles.inc"
};

/* Bank 39: graphics/sprites/bank39_platforms/. */
extern const u8 gSpriteBank39Tiles[SPRITE_TILES_BANK39_SIZE] = {
#include "sprites/bank39_tiles.inc"
};

/* Bank 40: graphics/sprites/bank40_bubbles/. */
extern const u8 gSpriteBank40Tiles[SPRITE_TILES_BANK40_SIZE] = {
#include "sprites/bank40_tiles.inc"
};

/* Bank 41: graphics/sprites/bank41_spark/. */
extern const u8 gSpriteBank41Tiles[SPRITE_TILES_BANK41_SIZE] = {
#include "sprites/bank41_tiles.inc"
};

/* Bank 42: graphics/sprites/bank42_checkpoint_text/. */
extern const u8 gSpriteBank42Tiles[SPRITE_TILES_BANK42_SIZE] = {
#include "sprites/bank42_tiles.inc"
};

/* Bank 43: graphics/sprites/bank43_sparkle/. */
extern const u8 gSpriteBank43Tiles[SPRITE_TILES_BANK43_SIZE] = {
#include "sprites/bank43_tiles.inc"
};

/* Bank 44: graphics/sprites/bank44_flame/. */
extern const u8 gSpriteBank44Tiles[SPRITE_TILES_BANK44_SIZE] = {
#include "sprites/bank44_tiles.inc"
};

/* Bank 45: graphics/sprites/bank45_seaweed/. */
extern const u8 gSpriteBank45Tiles[SPRITE_TILES_BANK45_SIZE] = {
#include "sprites/bank45_tiles.inc"
};

/* Bank 46: graphics/sprites/bank46_crash_eyes/. */
extern const u8 gSpriteBank46Tiles[SPRITE_TILES_BANK46_SIZE] = {
#include "sprites/bank46_tiles.inc"
};

/* Bank 47: graphics/sprites/bank47_icons/. */
extern const u8 gSpriteBank47Tiles[SPRITE_TILES_BANK47_SIZE] = {
#include "sprites/bank47_tiles.inc"
};

/* Bank 48: graphics/sprites/bank48_level_select_cursor/. */
extern const u8 gSpriteBank48Tiles[SPRITE_TILES_BANK48_SIZE] = {
#include "sprites/bank48_tiles.inc"
};

/* Bank 49: graphics/sprites/bank49_level_select_frame/. */
extern const u8 gSpriteBank49Tiles[SPRITE_TILES_BANK49_SIZE] = {
#include "sprites/bank49_tiles.inc"
};

/* Bank 50: graphics/sprites/bank50_twinkles/. */
extern const u8 gSpriteBank50Tiles[SPRITE_TILES_BANK50_SIZE] = {
#include "sprites/bank50_tiles.inc"
};

/* Bank 51: graphics/sprites/bank51_level_select_icons/. */
extern const u8 gSpriteBank51Tiles[SPRITE_TILES_BANK51_SIZE] = {
#include "sprites/bank51_tiles.inc"
};

/* Bank 52: graphics/sprites/bank52_level_select_arrows/. */
extern const u8 gSpriteBank52Tiles[SPRITE_TILES_BANK52_SIZE] = {
#include "sprites/bank52_tiles.inc"
};

/* Bank 53: graphics/sprites/bank53_cortex/. */
extern const u8 gSpriteBank53Tiles[SPRITE_TILES_BANK53_SIZE] = {
#include "sprites/bank53_tiles.inc"
};

/* Bank 54: graphics/sprites/bank54_dingodile/. */
extern const u8 gSpriteBank54Tiles[SPRITE_TILES_BANK54_SIZE] = {
#include "sprites/bank54_tiles.inc"
};

/* Bank 55: graphics/sprites/bank55_tiny/. */
extern const u8 gSpriteBank55Tiles[SPRITE_TILES_BANK55_SIZE] = {
#include "sprites/bank55_tiles.inc"
};

/* The 125 16-colour OBJ palettes (32 bytes each), the header's
 * palettes/paletteCount: InitLevelState and RunPauseMenu hand them to the
 * palette cache (SetPaletteCacheSource), and GetPaletteSlot copies palette
 * `paletteId` into a free OBJ palette slot. The sources are
 * graphics/sprites/palettes/NNN_<owner>.pal (JASC, NNN = paletteId), built with
 * tools/jasc_pal.py and concatenated in index order (graphics.mk). */
extern const u8 gObjPalettes[0xfa0] = {
#include "sprites/obj_palettes.bin.inc"
};
