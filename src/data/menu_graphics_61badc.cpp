extern "C" {
#include "gba/types.h"
}

/*
 * ROM 0x0861BADC-0x086377C0: the graphics of the menus, the title screen,
 * the continue prompt, the legal screen and the credits: their palettes
 * (JASC .pal, tools/jasc_pal.py, which keeps the entries' bit 15), the
 * struct bg_package pictures (tiles and maps rebuilt from one PNG each,
 * docs/graphics.md "Menu and title pictures") and the credits and VV logo
 * tile sheets.
 * Every asset is an LZ77 stream (gbagfx) that starts on a word boundary,
 * hence aligned(4): the 0-3 zero bytes after a stream are the next one's
 * alignment. tools/bin2c.py --lz writes the stream's bytes (graphics.mk).
 * Linked in ROM order between data/data.s sections by ldscript.txt, see
 * docs/data.md.
 */

/*
 * Palette of gMenuSkyBg (the sky/clouds menu background), 16 colours:
 * graphics/intro/24_61badc.pal.
 */
extern const u8 gMenuSkyBgPalette[] __attribute__((aligned(4))) = {
#include "intro/24_61badc.gbapal.lz.inc"
};

/* Palette of gPauseMenuBg, 256 colours: graphics/intro/25_61bb04.pal. */
extern const u8 gPauseMenuBgPalette[] __attribute__((aligned(4))) = {
#include "intro/25_61bb04.gbapal.lz.inc"
};

/* Palette of gLevelSelectPageBg, 256 colours: graphics/intro/26_61bd48.pal. */
extern const u8 gLevelSelectPageBgPalette[] __attribute__((aligned(4))) = {
#include "intro/26_61bd48.gbapal.lz.inc"
};

/*
 * Palette of gContinuePromptSmokeBg (the red smoke texture), 16 colours:
 * graphics/intro/27_61bf30.pal.
 */
extern const u8 gContinuePromptSmokeBgPalette[] __attribute__((aligned(4))) = {
#include "intro/27_61bf30.gbapal.lz.inc"
};

/*
 * Palette of gContinuePromptGlowBg (the fire/aura glow), 16 colours:
 * graphics/intro/28_61bf58.pal.
 */
extern const u8 gContinuePromptGlowBgPalette[] __attribute__((aligned(4))) = {
#include "intro/28_61bf58.gbapal.lz.inc"
};

/* Palette of gContinuePromptUkaUkaBg, 256 colours: graphics/intro/29_61bf80.pal. */
extern const u8 gContinuePromptUkaUkaBgPalette[] __attribute__((aligned(4))) = {
#include "intro/29_61bf80.gbapal.lz.inc"
};

/* OBJ palette of gTitleBandicootObj, 16 colours: graphics/intro/30_61c15c.pal. */
extern const u8 gTitleBandicootObjPalette[] __attribute__((aligned(4))) = {
#include "intro/30_61c15c.gbapal.lz.inc"
};

/* OBJ palette of gTitleCrashObj, 16 colours: graphics/intro/31_61c184.pal. */
extern const u8 gTitleCrashObjPalette[] __attribute__((aligned(4))) = {
#include "intro/31_61c184.gbapal.lz.inc"
};

/* OBJ palette of gTitleArrow1Obj, 16 colours: graphics/intro/32_61c1ac.pal. */
extern const u8 gTitleArrow1ObjPalette[] __attribute__((aligned(4))) = {
#include "intro/32_61c1ac.gbapal.lz.inc"
};

/* OBJ palette of gTitleArrow2Obj, 16 colours: graphics/intro/33_61c1d4.pal. */
extern const u8 gTitleArrow2ObjPalette[] __attribute__((aligned(4))) = {
#include "intro/33_61c1d4.gbapal.lz.inc"
};

/*
 * Palette of gUniversalLogoBg (the legal/credits text screen), 16 colours:
 * graphics/intro/34_61c1fc.pal.
 */
extern const u8 gUniversalLogoBgPalette[] __attribute__((aligned(4))) = {
#include "intro/34_61c1fc.gbapal.lz.inc"
};

/* Palette of gTitleScreenBg, 256 colours: graphics/intro/35_61c224.pal. */
extern const u8 gTitleScreenBgPalette[] __attribute__((aligned(4))) = {
#include "intro/35_61c224.gbapal.lz.inc"
};

/*
 * 4bpp tiles of gMenuSkyBg: the sky/clouds background (32x20 cells):
 * graphics/bg_package/61c30c_menu_sky.png.
 */
extern const u8 gMenuSkyBgTiles[] __attribute__((aligned(4))) = {
#include "bg_package/61c30c_menu_sky.img.bin.lz.inc"
};

/*
 * 8bpp tiles of gPauseMenuBg: Crash's face on the warp-room wall (30x20):
 * graphics/bg_package/61e5f8_pause_menu.png.
 */
extern const u8 gPauseMenuBgTiles[] __attribute__((aligned(4))) = {
#include "bg_package/61e5f8_pause_menu.img.bin.lz.inc"
};

/*
 * 8bpp tiles of gLevelSelectPageBg: the level-select platform icon (32x32):
 * graphics/bg_package/62556c_level_select_page.png.
 */
extern const u8 gLevelSelectPageBgTiles[] __attribute__((aligned(4))) = {
#include "bg_package/62556c_level_select_page.img.bin.lz.inc"
};

/*
 * 4bpp tiles of gContinuePromptSmokeBg: the red smoke texture (30x20):
 * graphics/bg_package/628c50_continue_prompt_smoke.png.
 */
extern const u8 gContinuePromptSmokeBgTiles[] __attribute__((aligned(4))) = {
#include "bg_package/628c50_continue_prompt_smoke.img.bin.lz.inc"
};

/*
 * 4bpp tiles of gContinuePromptGlowBg: the fire/aura glow (30x20):
 * graphics/bg_package/62a958_continue_prompt_glow.png.
 */
extern const u8 gContinuePromptGlowBgTiles[] __attribute__((aligned(4))) = {
#include "bg_package/62a958_continue_prompt_glow.img.bin.lz.inc"
};

/*
 * 8bpp tiles of gContinuePromptUkaUkaBg: Uka Uka's mask (30x20):
 * graphics/bg_package/62b34c_continue_prompt_uka_uka.png.
 */
extern const u8 gContinuePromptUkaUkaBgTiles[] __attribute__((aligned(4))) = {
#include "bg_package/62b34c_continue_prompt_uka_uka.img.bin.lz.inc"
};

/*
 * 4bpp OBJ tile sheet of gTitleBandicootObj: "BANDICOOT" (4x6):
 * graphics/bg_package/62c2c8_title_bandicoot_obj.png.
 */
extern const u8 gTitleBandicootObjTiles[] __attribute__((aligned(4))) = {
#include "bg_package/62c2c8_title_bandicoot_obj.img.bin.lz.inc"
};

/*
 * 4bpp OBJ tile sheet of gTitleCrashObj: the "CRASH" letters (8x40):
 * graphics/bg_package/62c4b0_title_crash_obj.png.
 */
extern const u8 gTitleCrashObjTiles[] __attribute__((aligned(4))) = {
#include "bg_package/62c4b0_title_crash_obj.img.bin.lz.inc"
};

/*
 * 4bpp OBJ tile sheet of gTitleArrow1Obj: the first menu arrow (4x16):
 * graphics/bg_package/62cc3c_title_arrow1_obj.png.
 */
extern const u8 gTitleArrow1ObjTiles[] __attribute__((aligned(4))) = {
#include "bg_package/62cc3c_title_arrow1_obj.img.bin.lz.inc"
};

/*
 * 4bpp OBJ tile sheet of gTitleArrow2Obj: the second menu arrow (4x16):
 * graphics/bg_package/62ce70_title_arrow2_obj.png.
 */
extern const u8 gTitleArrow2ObjTiles[] __attribute__((aligned(4))) = {
#include "bg_package/62ce70_title_arrow2_obj.img.bin.lz.inc"
};

/*
 * 4bpp tiles of gUniversalLogoBg: the legal/credits text screen (30x20):
 * graphics/bg_package/62d0cc_universal_logo_tiles.png.
 */
extern const u8 gUniversalLogoBgTiles[] __attribute__((aligned(4))) = {
#include "bg_package/62d0cc_universal_logo.img.bin.lz.inc"
};

/*
 * 8bpp tiles of gTitleScreenBg: the "Crash Bandicoot XS" title logo
 * (16x16, affine BG2):
 * graphics/bg_package/62e3b0_title_screen.png.
 */
extern const u8 gTitleScreenBgTiles[] __attribute__((aligned(4))) = {
#include "bg_package/62e3b0_title_screen.img.bin.lz.inc"
};

/* Text BG map of gMenuSkyBg: graphics/bg_package/61c30c_menu_sky.png. */
extern const u8 gMenuSkyBgMap[] __attribute__((aligned(4))) = {
#include "bg_package/61c30c_menu_sky.map.bin.lz.inc"
};

/* Text BG map of gPauseMenuBg: graphics/bg_package/61e5f8_pause_menu.png. */
extern const u8 gPauseMenuBgMap[] __attribute__((aligned(4))) = {
#include "bg_package/61e5f8_pause_menu.map.bin.lz.inc"
};

/* Text BG map of gLevelSelectPageBg: graphics/bg_package/62556c_level_select_page.png. */
extern const u8 gLevelSelectPageBgMap[] __attribute__((aligned(4))) = {
#include "bg_package/62556c_level_select_page.map.bin.lz.inc"
};

/* Text BG map of gContinuePromptSmokeBg: graphics/bg_package/628c50_continue_prompt_smoke.png. */
extern const u8 gContinuePromptSmokeBgMap[] __attribute__((aligned(4))) = {
#include "bg_package/628c50_continue_prompt_smoke.map.bin.lz.inc"
};

/* Text BG map of gContinuePromptGlowBg: graphics/bg_package/62a958_continue_prompt_glow.png. */
extern const u8 gContinuePromptGlowBgMap[] __attribute__((aligned(4))) = {
#include "bg_package/62a958_continue_prompt_glow.map.bin.lz.inc"
};

/*
 * Text BG map of gContinuePromptUkaUkaBg:
 * graphics/bg_package/62b34c_continue_prompt_uka_uka.png.
 */
extern const u8 gContinuePromptUkaUkaBgMap[] __attribute__((aligned(4))) = {
#include "bg_package/62b34c_continue_prompt_uka_uka.map.bin.lz.inc"
};

/*
 * OBJ tile map of gTitleBandicootObj: a u16 per OBJ tile, whose low byte
 * picks the sheet tile (TitleScreen::LoadObjTiles):
 * graphics/bg_package/62c2c8_title_bandicoot_obj.png.
 */
extern const u8 gTitleBandicootObjMap[] __attribute__((aligned(4))) = {
#include "bg_package/62c2c8_title_bandicoot_obj.map.bin.lz.inc"
};

/*
 * OBJ tile map of gTitleCrashObj: a u16 per OBJ tile, whose low byte
 * picks the sheet tile (TitleScreen::LoadObjTiles):
 * graphics/bg_package/62c4b0_title_crash_obj.png.
 */
extern const u8 gTitleCrashObjMap[] __attribute__((aligned(4))) = {
#include "bg_package/62c4b0_title_crash_obj.map.bin.lz.inc"
};

/*
 * OBJ tile map of gTitleArrow1Obj: a u16 per OBJ tile, whose low byte
 * picks the sheet tile (TitleScreen::LoadObjTiles):
 * graphics/bg_package/62cc3c_title_arrow1_obj.png.
 */
extern const u8 gTitleArrow1ObjMap[] __attribute__((aligned(4))) = {
#include "bg_package/62cc3c_title_arrow1_obj.map.bin.lz.inc"
};

/*
 * OBJ tile map of gTitleArrow2Obj: a u16 per OBJ tile, whose low byte
 * picks the sheet tile (TitleScreen::LoadObjTiles):
 * graphics/bg_package/62ce70_title_arrow2_obj.png.
 */
extern const u8 gTitleArrow2ObjMap[] __attribute__((aligned(4))) = {
#include "bg_package/62ce70_title_arrow2_obj.map.bin.lz.inc"
};

/* Text BG map of gUniversalLogoBg: graphics/bg_package/62d0cc_universal_logo.png. */
extern const u8 gUniversalLogoBgMap[] __attribute__((aligned(4))) = {
#include "bg_package/62d0cc_universal_logo.map.bin.lz.inc"
};

/*
 * Affine BG map of gTitleScreenBg (u16 entries, LoadBg keeps the low byte):
 * graphics/bg_package/62e3b0_title_screen.png.
 */
extern const u8 gTitleScreenBgMap[] __attribute__((aligned(4))) = {
#include "bg_package/62e3b0_title_screen.map.bin.lz.inc"
};

/* Palette of the credits' Red Eye Studios logo, 16 colours: graphics/intro/60_6319a0.pal. */
extern const u8 gCreditsRedEyeStudiosLogoPalette[] __attribute__((aligned(4))) = {
#include "intro/60_6319a0.gbapal.lz.inc"
};

/* Palette of the credits' Shin'en logo, 16 colours: graphics/intro/61_6319c8.pal. */
extern const u8 gCreditsShinenLogoPalette[] __attribute__((aligned(4))) = {
#include "intro/61_6319c8.gbapal.lz.inc"
};

/* Palette of the credits' Cosmigo logo, 16 colours: graphics/intro/62_6319f0.pal. */
extern const u8 gCreditsCosmigoLogoPalette[] __attribute__((aligned(4))) = {
#include "intro/62_6319f0.gbapal.lz.inc"
};

/* Palette of the credits' Universal logo, 16 colours: graphics/intro/63_631a18.pal. */
extern const u8 gCreditsUniversalLogoPalette[] __attribute__((aligned(4))) = {
#include "intro/63_631a18.gbapal.lz.inc"
};

/* Palette of the credits' VV logo, 16 colours: graphics/intro/64_631a40.pal. */
extern const u8 gCreditsVvLogoPalette[] __attribute__((aligned(4))) = {
#include "intro/64_631a40.gbapal.lz.inc"
};

/* Palette of the VV logo screen's emblem, 16 colours: graphics/intro/65_631a68.pal. */
extern const u8 gVvLogoEmblemPalette[] __attribute__((aligned(4))) = {
#include "intro/65_631a68.gbapal.lz.inc"
};

/* Palette of the VV logo screen's letters, 16 colours: graphics/intro/66_631a90.pal. */
extern const u8 gVvLogoLettersPalette[] __attribute__((aligned(4))) = {
#include "intro/66_631a90.gbapal.lz.inc"
};

/* Palette of the VV logo screen's URL, 16 colours: graphics/intro/67_631ab8.pal. */
extern const u8 gVvLogoUrlPalette[] __attribute__((aligned(4))) = {
#include "intro/67_631ab8.gbapal.lz.inc"
};

/* 4bpp tiles of the credits' Red Eye Studios logo: graphics/intro/68_631acc_tiles.png. */
extern const u8 gCreditsRedEyeStudiosLogoTiles[] __attribute__((aligned(4))) = {
#include "intro/68_631acc_tiles.img.bin.lz.inc"
};

/* 4bpp tiles of the credits' Shin'en logo: graphics/intro/69_6324b4_tiles.png. */
extern const u8 gCreditsShinenLogoTiles[] __attribute__((aligned(4))) = {
#include "intro/69_6324b4_tiles.img.bin.lz.inc"
};

/* 4bpp tiles of the credits' Cosmigo logo: graphics/intro/70_632820_tiles.png. */
extern const u8 gCreditsCosmigoLogoTiles[] __attribute__((aligned(4))) = {
#include "intro/70_632820_tiles.img.bin.lz.inc"
};

/* 4bpp tiles of the credits' Universal logo: graphics/intro/71_632bc4_tiles.png. */
extern const u8 gCreditsUniversalLogoTiles[] __attribute__((aligned(4))) = {
#include "intro/71_632bc4_tiles.img.bin.lz.inc"
};

/* 4bpp tiles of the credits' VV logo: graphics/intro/72_6334c4_tiles.png. */
extern const u8 gCreditsVvLogoTiles[] __attribute__((aligned(4))) = {
#include "intro/72_6334c4_tiles.img.bin.lz.inc"
};

/* 4bpp tiles of the VV logo screen's emblem: graphics/intro/73_634270_tiles.png. */
extern const u8 gVvLogoEmblemTiles[] __attribute__((aligned(4))) = {
#include "intro/73_634270_tiles.img.bin.lz.inc"
};

/* 4bpp tiles of the VV logo screen's letters: graphics/intro/74_636ef4_tiles.png. */
extern const u8 gVvLogoLettersTiles[] __attribute__((aligned(4))) = {
#include "intro/74_636ef4_tiles.img.bin.lz.inc"
};

/* 4bpp tiles of the VV logo screen's URL: graphics/intro/75_637604_tiles.png. */
extern const u8 gVvLogoUrlTiles[] __attribute__((aligned(4))) = {
#include "intro/75_637604_tiles.img.bin.lz.inc"
};
