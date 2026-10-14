#include "frontend.hpp"
#include "graphics.hpp"
#include "logo_fly_in.h"

extern "C" {
#include "core.h"
}

/*
 * ROM 0x0817CFF4-0x0817D6C0. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

#include "actor_anim.h"

using Engine::Gfx::TitleBandicootObj;
using Engine::Gfx::TitleCrashObj;
using Engine::Gfx::TitleArrow1Obj;
using Engine::Gfx::TitleArrow2Obj;
using Engine::Gfx::TitleScreenBg;

/* DrawTitleLogoPieces (title_screen.cpp): the {x, y} offsets of the
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

/* The palettes InitTitleScreen (title_screen.cpp) DMAs to OBJ palettes
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

/* The four OBJ sprites LoadTitleScreenObjTiles (title_screen.cpp) uploads,
 * through the IWRAM table gTitleObjPackages (src/iwram/iwram_data.cpp),
 * which lists them in the order 0817D0A8, 0817D0D0, 0817D0BC, 0817D094. */
const struct bg_package gTitleBandicootObj = { 4, 6, (void *)TitleBandicootObj::palette, (void *)TitleBandicootObj::tiles, (void *)TitleBandicootObj::map };
const struct bg_package gTitleCrashObj = { 8, 40, (void *)TitleCrashObj::palette, (void *)TitleCrashObj::tiles, (void *)TitleCrashObj::map };
const struct bg_package gTitleArrow1Obj = { 4, 16, (void *)TitleArrow1Obj::palette, (void *)TitleArrow1Obj::tiles, (void *)TitleArrow1Obj::map };
const struct bg_package gTitleArrow2Obj = { 4, 16, (void *)TitleArrow2Obj::palette, (void *)TitleArrow2Obj::tiles, (void *)TitleArrow2Obj::map };

/* LoadTitleScreenBg's (title_screen.cpp) BG2 picture. */
const struct bg_package gTitleScreenBg = { 16, 16, (void *)TitleScreenBg::palette, (void *)TitleScreenBg::tiles, (void *)TitleScreenBg::map };

/* The motion sequences of the nine countdown slots of
 * title_screen.cpp (RunTitleScreen, ResetTitleLogoPieces), which
 * popup_glyphs_17cf40.cpp's gTitleLogoPieceSeeds seeds: each ends with a
 * zero hold. The pieces' x and y are on the 240x160 title art, from its
 * origin DISPLAY_ART_X/Y (screen.h, docs/screen.md); a negative one is a
 * u16 cast of an s16 (the field is a u16, see struct delta_record).
 * Pieces 0 and 1 fly in from below the art: their first step starts past
 * the screen's edge (LOGO_FLY_IN_STEP, logo_fly_in.h), and is the ROM's
 * on the GBA. */
extern const struct delta_record gTitleLogoPieceMotion0[4] = {
    LOGO_FLY_IN_STEP(11, DISPLAY_ART_X + 152, DISPLAY_ART_Y + 160, TITLE_LOGO_PIECE0_FLY_IN, TITLE_LOGO_PIECE0_DX, TITLE_LOGO_PIECE0_DY),
    { 19, DISPLAY_ART_X + 69, (u16)(DISPLAY_ART_Y - 1), 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 69, (u16)(DISPLAY_ART_Y - 1), 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gTitleLogoPieceMotion1[4] = {
    LOGO_FLY_IN_STEP(10, DISPLAY_ART_X - 96, DISPLAY_ART_Y + 160, TITLE_LOGO_PIECE1_FLY_IN, TITLE_LOGO_PIECE1_DX, TITLE_LOGO_PIECE1_DY),
    { 9, DISPLAY_ART_X + 80, DISPLAY_ART_Y + 14, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 80, DISPLAY_ART_Y + 14, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gTitleLogoPieceMotion2[6] = {
    { 7, DISPLAY_ART_X + 120, DISPLAY_ART_Y + 72, 0, 51, 51, 0, 0, 0, 7489, 7489 },
    { 3, DISPLAY_ART_X + 120, DISPLAY_ART_Y + 72, 0, 256, 256, 0, 0, 0, 2184, 2184 },
    { 3, DISPLAY_ART_X + 120, DISPLAY_ART_Y + 72, 0, 281, 281, 0, 0, 0, -2184, -2184 },
    { 41, DISPLAY_ART_X + 120, DISPLAY_ART_Y + 72, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 120, DISPLAY_ART_Y + 72, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gTitleLogoPieceMotion3[5] = {
    { 15, DISPLAY_ART_X + 169, DISPLAY_ART_Y + 61, 0, 358, 358, 0, 0, 0, -2184, -2184 },
    { 3, DISPLAY_ART_X + 169, DISPLAY_ART_Y + 61, 0, 230, 230, 0, 0, 0, 2184, 2185 },
    { 48, DISPLAY_ART_X + 169, DISPLAY_ART_Y + 61, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 169, DISPLAY_ART_Y + 61, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gTitleLogoPieceMotion4[5] = {
    { 15, DISPLAY_ART_X + 146, DISPLAY_ART_Y + 50, 0, 358, 358, 0, 0, 0, -2184, -2184 },
    { 3, DISPLAY_ART_X + 146, DISPLAY_ART_Y + 50, 0, 230, 230, 0, 0, 0, 2184, 2184 },
    { 60, DISPLAY_ART_X + 146, DISPLAY_ART_Y + 50, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 146, DISPLAY_ART_Y + 50, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gTitleLogoPieceMotion5[5] = {
    { 15, DISPLAY_ART_X + 117, DISPLAY_ART_Y + 50, 0, 358, 358, 0, 0, 0, -2184, -2184 },
    { 3, DISPLAY_ART_X + 117, DISPLAY_ART_Y + 50, 0, 230, 230, 0, 0, 0, 2184, 2184 },
    { 72, DISPLAY_ART_X + 117, DISPLAY_ART_Y + 50, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 117, DISPLAY_ART_Y + 50, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gTitleLogoPieceMotion6[5] = {
    { 15, DISPLAY_ART_X + 90, DISPLAY_ART_Y + 53, 0, 358, 358, 0, 0, 0, -2184, -2184 },
    { 3, DISPLAY_ART_X + 90, DISPLAY_ART_Y + 53, 0, 230, 230, 0, 0, 0, 2184, 2184 },
    { 84, DISPLAY_ART_X + 90, DISPLAY_ART_Y + 53, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 90, DISPLAY_ART_Y + 53, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gTitleLogoPieceMotion7[5] = {
    { 15, DISPLAY_ART_X + 68, DISPLAY_ART_Y + 65, 0, 358, 358, 0, 0, 0, -2184, -2184 },
    { 3, DISPLAY_ART_X + 68, DISPLAY_ART_Y + 65, 0, 230, 230, 0, 0, 0, 2184, 2184 },
    { 96, DISPLAY_ART_X + 68, DISPLAY_ART_Y + 65, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 68, DISPLAY_ART_Y + 65, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gTitleLogoPieceMotion8[6] = {
    { 5, DISPLAY_ART_X + 119, DISPLAY_ART_Y + 71, 0, 383, 384, 0, 0, 0, -6553, -6553 },
    { 3, DISPLAY_ART_X + 119, DISPLAY_ART_Y + 71, 0, 256, 256, 0, 0, 0, -2184, -2184 },
    { 3, DISPLAY_ART_X + 119, DISPLAY_ART_Y + 71, 0, 230, 230, 0, 0, 0, 2184, 2184 },
    { 30, DISPLAY_ART_X + 119, DISPLAY_ART_Y + 71, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 119, DISPLAY_ART_Y + 71, 0, 256, 256, 0, 0, 0, 0, 0 },
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
