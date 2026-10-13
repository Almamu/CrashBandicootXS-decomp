extern "C" {
#include "gba/types.h"
}

/*
 * ROM 0x0861BADC-0x086377C0: the graphics of the menus, the title screen,
 * the continue prompt, the legal screen and the credits: their palettes
 * (each the palette of its picture's PNG, read by tools/jasc_pal.py
 * png-to-gba, which keeps the entries' bit 15), the
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
 * the palette of graphics/menus/sky.png.
 */
extern const u8 gMenuSkyBgPalette[] __attribute__((aligned(4))) = {
#include "menus/sky.gbapal.lz.inc"
};

/*
 * Palette of gPauseMenuBg, 256 colours: the palette of graphics/menus/pause.png.
 */
extern const u8 gPauseMenuBgPalette[] __attribute__((aligned(4))) = {
#include "menus/pause.gbapal.lz.inc"
};

/*
 * Palette of gLevelSelectPageBg, 256 colours: the palette of
 * graphics/bg_package/62556c_level_select_page.png.
 */
extern const u8 gLevelSelectPageBgPalette[] __attribute__((aligned(4))) = {
#include "bg_package/62556c_level_select_page.gbapal.lz.inc"
};

/*
 * Palette of gContinuePromptSmokeBg (the red smoke texture), 16 colours:
 * the palette of graphics/menus/continue_prompt/smoke.png.
 */
extern const u8 gContinuePromptSmokeBgPalette[] __attribute__((aligned(4))) = {
#include "menus/continue_prompt/smoke.gbapal.lz.inc"
};

/*
 * Palette of gContinuePromptGlowBg (the fire/aura glow), 16 colours:
 * the palette of graphics/menus/continue_prompt/glow.png.
 */
extern const u8 gContinuePromptGlowBgPalette[] __attribute__((aligned(4))) = {
#include "menus/continue_prompt/glow.gbapal.lz.inc"
};

/*
 * Palette of gContinuePromptUkaUkaBg, 256 colours: the palette of
 * graphics/menus/continue_prompt/uka_uka.png.
 */
extern const u8 gContinuePromptUkaUkaBgPalette[] __attribute__((aligned(4))) = {
#include "menus/continue_prompt/uka_uka.gbapal.lz.inc"
};

/*
 * OBJ palette of gTitleBandicootObj, 16 colours: the palette of
 * graphics/title/bandicoot.png.
 */
extern const u8 gTitleBandicootObjPalette[] __attribute__((aligned(4))) = {
#include "title/bandicoot.gbapal.lz.inc"
};

/*
 * OBJ palette of gTitleCrashObj, 16 colours: the palette of
 * graphics/title/crash.png.
 */
extern const u8 gTitleCrashObjPalette[] __attribute__((aligned(4))) = {
#include "title/crash.gbapal.lz.inc"
};

/*
 * OBJ palette of gTitleArrow1Obj, 16 colours: the palette of
 * graphics/title/arrow1.png.
 */
extern const u8 gTitleArrow1ObjPalette[] __attribute__((aligned(4))) = {
#include "title/arrow1.gbapal.lz.inc"
};

/*
 * OBJ palette of gTitleArrow2Obj, 16 colours: the palette of
 * graphics/title/arrow2.png.
 */
extern const u8 gTitleArrow2ObjPalette[] __attribute__((aligned(4))) = {
#include "title/arrow2.gbapal.lz.inc"
};

/*
 * Palette of gUniversalLogoBg (the Universal Interactive logo), 16 colours:
 * the palette of graphics/logos/universal.png.
 */
extern const u8 gUniversalLogoBgPalette[] __attribute__((aligned(4))) = {
#include "logos/universal.gbapal.lz.inc"
};

/*
 * Palette of gTitleScreenBg, 256 colours: the palette of
 * graphics/title/logo.png.
 */
extern const u8 gTitleScreenBgPalette[] __attribute__((aligned(4))) = {
#include "title/logo.gbapal.lz.inc"
};

/*
 * 4bpp tiles of gMenuSkyBg: the sky/clouds background (32x20 cells):
 * graphics/menus/sky.png.
 */
extern const u8 gMenuSkyBgTiles[] __attribute__((aligned(4))) = {
#include "menus/sky.img.bin.lz.inc"
};

/*
 * 8bpp tiles of gPauseMenuBg: Crash's face on the warp-room wall (30x20):
 * graphics/menus/pause.png.
 */
extern const u8 gPauseMenuBgTiles[] __attribute__((aligned(4))) = {
#include "menus/pause.img.bin.lz.inc"
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
 * graphics/menus/continue_prompt/smoke.png.
 */
extern const u8 gContinuePromptSmokeBgTiles[] __attribute__((aligned(4))) = {
#include "menus/continue_prompt/smoke.img.bin.lz.inc"
};

/*
 * 4bpp tiles of gContinuePromptGlowBg: the fire/aura glow (30x20):
 * graphics/menus/continue_prompt/glow.png.
 */
extern const u8 gContinuePromptGlowBgTiles[] __attribute__((aligned(4))) = {
#include "menus/continue_prompt/glow.img.bin.lz.inc"
};

/*
 * 8bpp tiles of gContinuePromptUkaUkaBg: Uka Uka's mask (30x20):
 * graphics/menus/continue_prompt/uka_uka.png.
 */
extern const u8 gContinuePromptUkaUkaBgTiles[] __attribute__((aligned(4))) = {
#include "menus/continue_prompt/uka_uka.img.bin.lz.inc"
};

/*
 * 4bpp OBJ tile sheet of gTitleBandicootObj: "BANDICOOT" (4x6):
 * graphics/title/bandicoot.png.
 */
extern const u8 gTitleBandicootObjTiles[] __attribute__((aligned(4))) = {
#include "title/bandicoot.img.bin.lz.inc"
};

/*
 * 4bpp OBJ tile sheet of gTitleCrashObj: the "CRASH" letters (8x40):
 * graphics/title/crash.png.
 */
extern const u8 gTitleCrashObjTiles[] __attribute__((aligned(4))) = {
#include "title/crash.img.bin.lz.inc"
};

/*
 * 4bpp OBJ tile sheet of gTitleArrow1Obj: the first menu arrow (4x16):
 * graphics/title/arrow1.png.
 */
extern const u8 gTitleArrow1ObjTiles[] __attribute__((aligned(4))) = {
#include "title/arrow1.img.bin.lz.inc"
};

/*
 * 4bpp OBJ tile sheet of gTitleArrow2Obj: the second menu arrow (4x16):
 * graphics/title/arrow2.png.
 */
extern const u8 gTitleArrow2ObjTiles[] __attribute__((aligned(4))) = {
#include "title/arrow2.img.bin.lz.inc"
};

/*
 * 8bpp tiles of gUniversalLogoBg, the Universal Interactive logo (30x20,
 * an affine BG2 of CompanyLogos::LoadUniversalLogoBg):
 * graphics/logos/universal.png.
 */
extern const u8 gUniversalLogoBgTiles[] __attribute__((aligned(4))) = {
#include "logos/universal.img.bin.lz.inc"
};

/*
 * 8bpp tiles of gTitleScreenBg: the "Crash Bandicoot XS" title logo
 * (16x16, affine BG2):
 * graphics/title/logo.png.
 */
extern const u8 gTitleScreenBgTiles[] __attribute__((aligned(4))) = {
#include "title/logo.img.bin.lz.inc"
};

/* Text BG map of gMenuSkyBg: graphics/menus/sky.png. */
extern const u8 gMenuSkyBgMap[] __attribute__((aligned(4))) = {
#include "menus/sky.map.bin.lz.inc"
};

/* Text BG map of gPauseMenuBg: graphics/menus/pause.png. */
extern const u8 gPauseMenuBgMap[] __attribute__((aligned(4))) = {
#include "menus/pause.map.bin.lz.inc"
};

/* Text BG map of gLevelSelectPageBg: graphics/bg_package/62556c_level_select_page.png. */
extern const u8 gLevelSelectPageBgMap[] __attribute__((aligned(4))) = {
#include "bg_package/62556c_level_select_page.map.bin.lz.inc"
};

/* Text BG map of gContinuePromptSmokeBg: graphics/menus/continue_prompt/smoke.png. */
extern const u8 gContinuePromptSmokeBgMap[] __attribute__((aligned(4))) = {
#include "menus/continue_prompt/smoke.map.bin.lz.inc"
};

/* Text BG map of gContinuePromptGlowBg: graphics/menus/continue_prompt/glow.png. */
extern const u8 gContinuePromptGlowBgMap[] __attribute__((aligned(4))) = {
#include "menus/continue_prompt/glow.map.bin.lz.inc"
};

/*
 * Text BG map of gContinuePromptUkaUkaBg:
 * graphics/menus/continue_prompt/uka_uka.png.
 */
extern const u8 gContinuePromptUkaUkaBgMap[] __attribute__((aligned(4))) = {
#include "menus/continue_prompt/uka_uka.map.bin.lz.inc"
};

/*
 * OBJ tile map of gTitleBandicootObj: a u16 per OBJ tile, whose low byte
 * picks the sheet tile (TitleScreen::LoadObjTiles):
 * graphics/title/bandicoot.png.
 */
extern const u8 gTitleBandicootObjMap[] __attribute__((aligned(4))) = {
#include "title/bandicoot.map.bin.lz.inc"
};

/*
 * OBJ tile map of gTitleCrashObj: a u16 per OBJ tile, whose low byte
 * picks the sheet tile (TitleScreen::LoadObjTiles):
 * graphics/title/crash.png.
 */
extern const u8 gTitleCrashObjMap[] __attribute__((aligned(4))) = {
#include "title/crash.map.bin.lz.inc"
};

/*
 * OBJ tile map of gTitleArrow1Obj: a u16 per OBJ tile, whose low byte
 * picks the sheet tile (TitleScreen::LoadObjTiles):
 * graphics/title/arrow1.png.
 */
extern const u8 gTitleArrow1ObjMap[] __attribute__((aligned(4))) = {
#include "title/arrow1.map.bin.lz.inc"
};

/*
 * OBJ tile map of gTitleArrow2Obj: a u16 per OBJ tile, whose low byte
 * picks the sheet tile (TitleScreen::LoadObjTiles):
 * graphics/title/arrow2.png.
 */
extern const u8 gTitleArrow2ObjMap[] __attribute__((aligned(4))) = {
#include "title/arrow2.map.bin.lz.inc"
};

/*
 * Affine BG map of gUniversalLogoBg (u16 entries, LoadUniversalLogoBg keeps
 * the low byte; 5 have a stray high byte,
 * graphics/logos/universal_high.json):
 * graphics/logos/universal.png.
 */
extern const u8 gUniversalLogoBgMap[] __attribute__((aligned(4))) = {
#include "logos/universal.map.bin.lz.inc"
};

/*
 * Affine BG map of gTitleScreenBg (u16 entries, LoadBg keeps the low byte):
 * graphics/title/logo.png.
 */
extern const u8 gTitleScreenBgMap[] __attribute__((aligned(4))) = {
#include "title/logo.map.bin.lz.inc"
};

/*
 * Palette of the credits' Red Eye Studios logo, 16 colours: the palette of
 * graphics/logos/credits/red_eye_studios.png.
 */
extern const u8 gCreditsRedEyeStudiosLogoPalette[] __attribute__((aligned(4))) = {
#include "logos/credits/red_eye_studios.gbapal.lz.inc"
};

/*
 * Palette of the credits' Shin'en logo, 16 colours: the palette of
 * graphics/logos/credits/shinen.png.
 */
extern const u8 gCreditsShinenLogoPalette[] __attribute__((aligned(4))) = {
#include "logos/credits/shinen.gbapal.lz.inc"
};

/*
 * Palette of the credits' Cosmigo logo, 16 colours: the palette of
 * graphics/logos/credits/cosmigo.png.
 */
extern const u8 gCreditsCosmigoLogoPalette[] __attribute__((aligned(4))) = {
#include "logos/credits/cosmigo.gbapal.lz.inc"
};

/*
 * Palette of the credits' Universal logo, 16 colours: the palette of
 * graphics/logos/credits/universal.png.
 */
extern const u8 gCreditsUniversalLogoPalette[] __attribute__((aligned(4))) = {
#include "logos/credits/universal.gbapal.lz.inc"
};

/*
 * Palette of the credits' VV logo, 16 colours: the palette of graphics/logos/credits/vicarious_visions.png.
 */
extern const u8 gCreditsVvLogoPalette[] __attribute__((aligned(4))) = {
#include "logos/credits/vicarious_visions.gbapal.lz.inc"
};

/*
 * Palette of the VV logo screen's emblem, 16 colours: the palette of
 * graphics/logos/vicarious_visions/emblem.png.
 */
extern const u8 gVvLogoEmblemPalette[] __attribute__((aligned(4))) = {
#include "logos/vicarious_visions/emblem.gbapal.lz.inc"
};

/*
 * Palette of the VV logo screen's letters, 16 colours: the palette of
 * graphics/logos/vicarious_visions/letters.png.
 */
extern const u8 gVvLogoLettersPalette[] __attribute__((aligned(4))) = {
#include "logos/vicarious_visions/letters.gbapal.lz.inc"
};

/*
 * Palette of the VV logo screen's URL, 16 colours: the palette of
 * graphics/logos/vicarious_visions/url.png.
 */
extern const u8 gVvLogoUrlPalette[] __attribute__((aligned(4))) = {
#include "logos/vicarious_visions/url.gbapal.lz.inc"
};

/* 4bpp tiles of the credits' Red Eye Studios logo: graphics/logos/credits/red_eye_studios.png. */
extern const u8 gCreditsRedEyeStudiosLogoTiles[] __attribute__((aligned(4))) = {
#include "logos/credits/red_eye_studios.img.bin.lz.inc"
};

/* 4bpp tiles of the credits' Shin'en logo: graphics/logos/credits/shinen.png. */
extern const u8 gCreditsShinenLogoTiles[] __attribute__((aligned(4))) = {
#include "logos/credits/shinen.img.bin.lz.inc"
};

/* 4bpp tiles of the credits' Cosmigo logo: graphics/logos/credits/cosmigo.png. */
extern const u8 gCreditsCosmigoLogoTiles[] __attribute__((aligned(4))) = {
#include "logos/credits/cosmigo.img.bin.lz.inc"
};

/* 4bpp tiles of the credits' Universal logo: graphics/logos/credits/universal.png. */
extern const u8 gCreditsUniversalLogoTiles[] __attribute__((aligned(4))) = {
#include "logos/credits/universal.img.bin.lz.inc"
};

/* 4bpp tiles of the credits' VV logo: graphics/logos/credits/vicarious_visions.png. */
extern const u8 gCreditsVvLogoTiles[] __attribute__((aligned(4))) = {
#include "logos/credits/vicarious_visions.img.bin.lz.inc"
};

/* 4bpp tiles of the VV logo screen's emblem: graphics/logos/vicarious_visions/emblem.png. */
extern const u8 gVvLogoEmblemTiles[] __attribute__((aligned(4))) = {
#include "logos/vicarious_visions/emblem.img.bin.lz.inc"
};

/* 4bpp tiles of the VV logo screen's letters: graphics/logos/vicarious_visions/letters.png. */
extern const u8 gVvLogoLettersTiles[] __attribute__((aligned(4))) = {
#include "logos/vicarious_visions/letters.img.bin.lz.inc"
};

/* 4bpp tiles of the VV logo screen's URL: graphics/logos/vicarious_visions/url.png. */
extern const u8 gVvLogoUrlTiles[] __attribute__((aligned(4))) = {
#include "logos/vicarious_visions/url.img.bin.lz.inc"
};
