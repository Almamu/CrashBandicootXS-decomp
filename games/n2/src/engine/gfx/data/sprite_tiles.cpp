extern "C" {
#include "gba/types.h"
}
#include "sprites/sprite_frames.h"

/*
 * N-Tranced's ROM 0x082B3914-0x084768B4: the sprite tile pool and the 93
 * OBJ palettes, as XS's sprite_tiles.cpp (see it, and docs/graphics.md
 * "Sprite banks"): N-Tranced's gSpriteBankTable (0x084768B4, still
 * incbin'd) starts with {banks, tileBase = gSpriteBank00Tiles, palettes =
 * gObjPalettes, 49, 93}, and each of the 49 banks has its own range of the
 * pool, back to back in bank order, one array below.
 *
 * Raw 4bpp (32-byte) OBJ tiles, no header. tools/sprite_frames.py
 * --graphics-dir games/n2/graphics pack builds them from
 * games/n2/graphics/sprites/bankNN_<name>/ (docs/n_tranced.md, "Sprite
 * banks"). N-Tranced's frames hold offsets into the pool, which its
 * incbin'd bank table keeps: a bank can't change size until that table is
 * built from source too.
 * Linked in ROM order between the incbin'd data by games/n2/ldscript.txt.
 */

/* Bank 0: games/n2/graphics/sprites/bank00_crash/. */
extern const u8 gSpriteBank00Tiles[SPRITE_TILES_BANK00_SIZE] = {
#include "sprites/bank00_tiles.inc"
};

/* Bank 1: games/n2/graphics/sprites/bank01_crash_carpet/. */
extern const u8 gSpriteBank01Tiles[SPRITE_TILES_BANK01_SIZE] = {
#include "sprites/bank01_tiles.inc"
};

/* Bank 2: games/n2/graphics/sprites/bank02_crash_jetpack/. */
extern const u8 gSpriteBank02Tiles[SPRITE_TILES_BANK02_SIZE] = {
#include "sprites/bank02_tiles.inc"
};

/* Bank 3: games/n2/graphics/sprites/bank03_pot_carrier/. */
extern const u8 gSpriteBank03Tiles[SPRITE_TILES_BANK03_SIZE] = {
#include "sprites/bank03_tiles.inc"
};

/* Bank 4: games/n2/graphics/sprites/bank04_scimitar_guard/. */
extern const u8 gSpriteBank04Tiles[SPRITE_TILES_BANK04_SIZE] = {
#include "sprites/bank04_tiles.inc"
};

/* Bank 5: games/n2/graphics/sprites/bank05_scorpion/. */
extern const u8 gSpriteBank05Tiles[SPRITE_TILES_BANK05_SIZE] = {
#include "sprites/bank05_tiles.inc"
};

/* Bank 6: games/n2/graphics/sprites/bank06_genie/. */
extern const u8 gSpriteBank06Tiles[SPRITE_TILES_BANK06_SIZE] = {
#include "sprites/bank06_tiles.inc"
};

/* Bank 7: games/n2/graphics/sprites/bank07_fire_juggler/. */
extern const u8 gSpriteBank07Tiles[SPRITE_TILES_BANK07_SIZE] = {
#include "sprites/bank07_tiles.inc"
};

/* Bank 8: games/n2/graphics/sprites/bank08_carpet_rider/. */
extern const u8 gSpriteBank08Tiles[SPRITE_TILES_BANK08_SIZE] = {
#include "sprites/bank08_tiles.inc"
};

/* Bank 9: games/n2/graphics/sprites/bank09_scorpion_standing/. */
extern const u8 gSpriteBank09Tiles[SPRITE_TILES_BANK09_SIZE] = {
#include "sprites/bank09_tiles.inc"
};

/* Bank 10: games/n2/graphics/sprites/bank10_rock_pusher/. */
extern const u8 gSpriteBank10Tiles[SPRITE_TILES_BANK10_SIZE] = {
#include "sprites/bank10_tiles.inc"
};

/* Bank 11: games/n2/graphics/sprites/bank11_spike_block_mummy/. */
extern const u8 gSpriteBank11Tiles[SPRITE_TILES_BANK11_SIZE] = {
#include "sprites/bank11_tiles.inc"
};

/* Bank 12: games/n2/graphics/sprites/bank12_shield_guard/. */
extern const u8 gSpriteBank12Tiles[SPRITE_TILES_BANK12_SIZE] = {
#include "sprites/bank12_tiles.inc"
};

/* Bank 13: games/n2/graphics/sprites/bank13_scarab/. */
extern const u8 gSpriteBank13Tiles[SPRITE_TILES_BANK13_SIZE] = {
#include "sprites/bank13_tiles.inc"
};

/* Bank 14: games/n2/graphics/sprites/bank14_sarcophagus/. */
extern const u8 gSpriteBank14Tiles[SPRITE_TILES_BANK14_SIZE] = {
#include "sprites/bank14_tiles.inc"
};

/* Bank 15: games/n2/graphics/sprites/bank15_mummy/. */
extern const u8 gSpriteBank15Tiles[SPRITE_TILES_BANK15_SIZE] = {
#include "sprites/bank15_tiles.inc"
};

/* Bank 16: games/n2/graphics/sprites/bank16_crocodile/. */
extern const u8 gSpriteBank16Tiles[SPRITE_TILES_BANK16_SIZE] = {
#include "sprites/bank16_tiles.inc"
};

/* Bank 17: games/n2/graphics/sprites/bank17_cobra_basket/. */
extern const u8 gSpriteBank17Tiles[SPRITE_TILES_BANK17_SIZE] = {
#include "sprites/bank17_tiles.inc"
};

/* Bank 18: games/n2/graphics/sprites/bank18_spear_tribesman/. */
extern const u8 gSpriteBank18Tiles[SPRITE_TILES_BANK18_SIZE] = {
#include "sprites/bank18_tiles.inc"
};

/* Bank 19: games/n2/graphics/sprites/bank19_parrot/. */
extern const u8 gSpriteBank19Tiles[SPRITE_TILES_BANK19_SIZE] = {
#include "sprites/bank19_tiles.inc"
};

/* Bank 20: games/n2/graphics/sprites/bank20_fireball_tribesman/. */
extern const u8 gSpriteBank20Tiles[SPRITE_TILES_BANK20_SIZE] = {
#include "sprites/bank20_tiles.inc"
};

/* Bank 21: games/n2/graphics/sprites/bank21_banana_monkey/. */
extern const u8 gSpriteBank21Tiles[SPRITE_TILES_BANK21_SIZE] = {
#include "sprites/bank21_tiles.inc"
};

/* Bank 22: games/n2/graphics/sprites/bank22_fire_burst/. */
extern const u8 gSpriteBank22Tiles[SPRITE_TILES_BANK22_SIZE] = {
#include "sprites/bank22_tiles.inc"
};

/* Bank 23: games/n2/graphics/sprites/bank23_crates/. */
extern const u8 gSpriteBank23Tiles[SPRITE_TILES_BANK23_SIZE] = {
#include "sprites/bank23_tiles.inc"
};

/* Bank 24: games/n2/graphics/sprites/bank24_gems/. */
extern const u8 gSpriteBank24Tiles[SPRITE_TILES_BANK24_SIZE] = {
#include "sprites/bank24_tiles.inc"
};

/* Bank 25: games/n2/graphics/sprites/bank25_trial_icons/. */
extern const u8 gSpriteBank25Tiles[SPRITE_TILES_BANK25_SIZE] = {
#include "sprites/bank25_tiles.inc"
};

/* Bank 26: games/n2/graphics/sprites/bank26_aku_aku/. */
extern const u8 gSpriteBank26Tiles[SPRITE_TILES_BANK26_SIZE] = {
#include "sprites/bank26_tiles.inc"
};

/* Bank 27: games/n2/graphics/sprites/bank27_wumpa/. */
extern const u8 gSpriteBank27Tiles[SPRITE_TILES_BANK27_SIZE] = {
#include "sprites/bank27_tiles.inc"
};

/* Bank 28: games/n2/graphics/sprites/bank28_stopwatch/. */
extern const u8 gSpriteBank28Tiles[SPRITE_TILES_BANK28_SIZE] = {
#include "sprites/bank28_tiles.inc"
};

/* Bank 29: games/n2/graphics/sprites/bank29_crystal/. */
extern const u8 gSpriteBank29Tiles[SPRITE_TILES_BANK29_SIZE] = {
#include "sprites/bank29_tiles.inc"
};

/* Bank 30: games/n2/graphics/sprites/bank30_powers/. */
extern const u8 gSpriteBank30Tiles[SPRITE_TILES_BANK30_SIZE] = {
#include "sprites/bank30_tiles.inc"
};

/* Bank 31: games/n2/graphics/sprites/bank31_platforms/. */
extern const u8 gSpriteBank31Tiles[SPRITE_TILES_BANK31_SIZE] = {
#include "sprites/bank31_tiles.inc"
};

/* Bank 32: games/n2/graphics/sprites/bank32_spark/. */
extern const u8 gSpriteBank32Tiles[SPRITE_TILES_BANK32_SIZE] = {
#include "sprites/bank32_tiles.inc"
};

/* Bank 33: games/n2/graphics/sprites/bank33_checkpoint_text/. */
extern const u8 gSpriteBank33Tiles[SPRITE_TILES_BANK33_SIZE] = {
#include "sprites/bank33_tiles.inc"
};

/* Bank 34: games/n2/graphics/sprites/bank34_sparkle/. */
extern const u8 gSpriteBank34Tiles[SPRITE_TILES_BANK34_SIZE] = {
#include "sprites/bank34_tiles.inc"
};

/* Bank 35: games/n2/graphics/sprites/bank35_flame/. */
extern const u8 gSpriteBank35Tiles[SPRITE_TILES_BANK35_SIZE] = {
#include "sprites/bank35_tiles.inc"
};

/* Bank 36: games/n2/graphics/sprites/bank36_dialogue/. */
extern const u8 gSpriteBank36Tiles[SPRITE_TILES_BANK36_SIZE] = {
#include "sprites/bank36_tiles.inc"
};

/* Bank 37: games/n2/graphics/sprites/bank37_traps/. */
extern const u8 gSpriteBank37Tiles[SPRITE_TILES_BANK37_SIZE] = {
#include "sprites/bank37_tiles.inc"
};

/* Bank 38: games/n2/graphics/sprites/bank38_bursts/. */
extern const u8 gSpriteBank38Tiles[SPRITE_TILES_BANK38_SIZE] = {
#include "sprites/bank38_tiles.inc"
};

/* Bank 39: games/n2/graphics/sprites/bank39_rocket_and_portals/. */
extern const u8 gSpriteBank39Tiles[SPRITE_TILES_BANK39_SIZE] = {
#include "sprites/bank39_tiles.inc"
};

/* Bank 40: games/n2/graphics/sprites/bank40_boss_icons/. */
extern const u8 gSpriteBank40Tiles[SPRITE_TILES_BANK40_SIZE] = {
#include "sprites/bank40_tiles.inc"
};

/* Bank 41: games/n2/graphics/sprites/bank41_icons/. */
extern const u8 gSpriteBank41Tiles[SPRITE_TILES_BANK41_SIZE] = {
#include "sprites/bank41_tiles.inc"
};

/* Bank 42: games/n2/graphics/sprites/bank42_dingodile/. */
extern const u8 gSpriteBank42Tiles[SPRITE_TILES_BANK42_SIZE] = {
#include "sprites/bank42_tiles.inc"
};

/* Bank 43: games/n2/graphics/sprites/bank43_skull_hovercraft/. */
extern const u8 gSpriteBank43Tiles[SPRITE_TILES_BANK43_SIZE] = {
#include "sprites/bank43_tiles.inc"
};

/* Bank 44: games/n2/graphics/sprites/bank44_n_trance/. */
extern const u8 gSpriteBank44Tiles[SPRITE_TILES_BANK44_SIZE] = {
#include "sprites/bank44_tiles.inc"
};

/* Bank 45: games/n2/graphics/sprites/bank45_n_gin/. */
extern const u8 gSpriteBank45Tiles[SPRITE_TILES_BANK45_SIZE] = {
#include "sprites/bank45_tiles.inc"
};

/* Bank 46: games/n2/graphics/sprites/bank46_crash_extra/. */
extern const u8 gSpriteBank46Tiles[SPRITE_TILES_BANK46_SIZE] = {
#include "sprites/bank46_tiles.inc"
};

/* Bank 47: games/n2/graphics/sprites/bank47_ufo/. */
extern const u8 gSpriteBank47Tiles[SPRITE_TILES_BANK47_SIZE] = {
#include "sprites/bank47_tiles.inc"
};

/* Bank 48: games/n2/graphics/sprites/bank48_palm_trees/. */
extern const u8 gSpriteBank48Tiles[SPRITE_TILES_BANK48_SIZE] = {
#include "sprites/bank48_tiles.inc"
};

/* The 93 16-colour OBJ palettes (32 bytes each), the header's
 * palettes/paletteCount, as XS's gObjPalettes. The sources are
 * games/n2/graphics/sprites/palettes/NNN_<owner>.pal (JASC, NNN =
 * paletteId), concatenated in index order (graphics_rules.mk). */
extern const u8 gObjPalettes[0xba0] = {
#include "sprites/obj_palettes.bin.inc"
};
