#include "core.h"

/*
 * ROM 0x0817CFF4-0x0817D6C0. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

#include "frontend.h"
#include "actor_anim.h"

extern const u8 gTitleBandicootObjPalette[];
extern const u8 gTitleCrashObjPalette[];
extern const u8 gTitleArrow1ObjPalette[];
extern const u8 gTitleArrow2ObjPalette[];
extern const u8 gTitleScreenBgPalette[];
extern const u8 gTitleBandicootObjTiles[];
extern const u8 gTitleCrashObjTiles[];
extern const u8 gTitleArrow1ObjTiles[];
extern const u8 gTitleArrow2ObjTiles[];
extern const u8 gTitleScreenBgTiles[];
extern const u8 gTitleBandicootObjMap[];
extern const u8 gTitleCrashObjMap[];
extern const u8 gTitleArrow1ObjMap[];
extern const u8 gTitleArrow2ObjMap[];
extern const u8 gTitleScreenBgMap[];
/* DrawTitleLogoPieces (title_screen_init.cpp): the {x, y} offsets of the
 * eight OAM pieces it draws around each of its two slots. */
const s32 gTitleArrowPieceOffsets[8][2] = {
    { 0, 0 },
    { 15, 32 },
    { 32, 64 },
    { 49, 96 },
    { 61, 0 },
    { 34, 32 },
    { 5, 61 },
    { 0, 70 },
};

/* The palettes InitTitleScreen (title_screen_init.cpp) DMAs to OBJ palettes
 * 13, 14 and 15. */
const u16 gTitleMenuPalette[16] = {
    0x83E0, 0x9CC6, 0x107F, 0x0D04, 0x0F9F, 0x894C, 0x0864, 0x05D4,
    0x0ABE, 0xA27F, 0x09BE, 0x1D5F, 0x886B, 0x94DF, 0x0C9B, 0x0873,
};
const u16 gTitleMenuSelectedPalette[16] = {
    0x03E0, 0x1CC6, 0x359E, 0x0D04, 0x4FDE, 0x094C, 0x0864, 0x05D4,
    0x3AFE, 0x36BE, 0x223E, 0x35FE, 0x086B, 0x35DE, 0x35DB, 0x0873,
};
const u16 gTitleMenuBlinkPalette[16] = {
    0x0000, 0x9CC6, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* The four OBJ sprites LoadTitleScreenObjTiles (title_screen_init.cpp) uploads,
 * through the IWRAM table gTitleObjPackages (src/iwram/iwram_data.cpp),
 * which lists them in the order 0817D0A8, 0817D0D0, 0817D0BC, 0817D094. */
const struct bg_package gTitleBandicootObj = { 4, 6, (void *)gTitleBandicootObjPalette, (void *)gTitleBandicootObjTiles, (void *)gTitleBandicootObjMap };
const struct bg_package gTitleCrashObj = { 8, 40, (void *)gTitleCrashObjPalette, (void *)gTitleCrashObjTiles, (void *)gTitleCrashObjMap };
const struct bg_package gTitleArrow1Obj = { 4, 16, (void *)gTitleArrow1ObjPalette, (void *)gTitleArrow1ObjTiles, (void *)gTitleArrow1ObjMap };
const struct bg_package gTitleArrow2Obj = { 4, 16, (void *)gTitleArrow2ObjPalette, (void *)gTitleArrow2ObjTiles, (void *)gTitleArrow2ObjMap };

/* LoadTitleScreenBg's (title_screen_init.cpp) BG2 picture. */
const struct bg_package gTitleScreenBg = { 16, 16, (void *)gTitleScreenBgPalette, (void *)gTitleScreenBgTiles, (void *)gTitleScreenBgMap };

/* The motion sequences of the nine countdown slots of
 * title_screen.cpp (RunTitleScreen, ResetTitleLogoPieces), which
 * popup_glyphs_17cf40.c's gTitleLogoPieceSeeds seeds: each ends with a
 * zero hold. */
const struct delta_record gTitleLogoPieceMotion0[4] = {
    { 11, 152, 160, 0, 256, 256, -494498, -959208, 0, 0, 0 },
    { 19, 69, 65535, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 69, 65535, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gTitleLogoPieceMotion1[4] = {
    { 10, 65440, 160, 0, 256, 256, 1153761, -956825, 0, 0, 0 },
    { 9, 80, 14, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 80, 14, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gTitleLogoPieceMotion2[6] = {
    { 7, 120, 72, 0, 51, 51, 0, 0, 0, 7489, 7489 },
    { 3, 120, 72, 0, 256, 256, 0, 0, 0, 2184, 2184 },
    { 3, 120, 72, 0, 281, 281, 0, 0, 0, -2184, -2184 },
    { 41, 120, 72, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 120, 72, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gTitleLogoPieceMotion3[5] = {
    { 15, 169, 61, 0, 358, 358, 0, 0, 0, -2184, -2184 },
    { 3, 169, 61, 0, 230, 230, 0, 0, 0, 2184, 2185 },
    { 48, 169, 61, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 169, 61, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gTitleLogoPieceMotion4[5] = {
    { 15, 146, 50, 0, 358, 358, 0, 0, 0, -2184, -2184 },
    { 3, 146, 50, 0, 230, 230, 0, 0, 0, 2184, 2184 },
    { 60, 146, 50, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 146, 50, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gTitleLogoPieceMotion5[5] = {
    { 15, 117, 50, 0, 358, 358, 0, 0, 0, -2184, -2184 },
    { 3, 117, 50, 0, 230, 230, 0, 0, 0, 2184, 2184 },
    { 72, 117, 50, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 117, 50, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gTitleLogoPieceMotion6[5] = {
    { 15, 90, 53, 0, 358, 358, 0, 0, 0, -2184, -2184 },
    { 3, 90, 53, 0, 230, 230, 0, 0, 0, 2184, 2184 },
    { 84, 90, 53, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 90, 53, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gTitleLogoPieceMotion7[5] = {
    { 15, 68, 65, 0, 358, 358, 0, 0, 0, -2184, -2184 },
    { 3, 68, 65, 0, 230, 230, 0, 0, 0, 2184, 2184 },
    { 96, 68, 65, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 68, 65, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gTitleLogoPieceMotion8[6] = {
    { 5, 119, 71, 0, 383, 384, 0, 0, 0, -6553, -6553 },
    { 3, 119, 71, 0, 256, 256, 0, 0, 0, -2184, -2184 },
    { 3, 119, 71, 0, 230, 230, 0, 0, 0, 2184, 2184 },
    { 30, 119, 71, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 119, 71, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

/* The animation record RunCompanyLogos (company_logos.cpp) builds its
 * part from (actor_anim.h's `struct anim_table_record`; this file had its
 * own view, `struct anim_record_view`). The keyframes and frames are
 * record 0's of the categories 0-2 family (Crash riding the polar bear),
 * inside gPolarCategoryPalette. */
const struct anim_table_record gLogoActorAnim = {
    0, (struct anim_frame_record *)((const u8 *)gPolarCategoryPalette + 0x400),
    (u32 *)((const u8 *)gPolarCategoryPalette + 0x49C),
    0, 0x100, { -10, -20, -1, 20, 44, 3 }, 0, 0,
};
