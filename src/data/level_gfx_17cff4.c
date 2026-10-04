#include "core.h"

/*
 * ROM 0x0817CFF4-0x0817D6C0. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

#include "graphics_package.h"

/* graphics_loading_35d1c.c's `struct delta_record`: one step of a
 * countdown slot's motion. When the slot's hold count runs out it loads
 * the next record: a new hold count (0 ends the sequence), three Q16.16
 * positions, two Q24.8 velocities and the five per-frame deltas added to
 * them while the hold lasts. */
struct delta_record
{
    s16 hold;
    u16 dPosA;
    u16 dPosB;
    u16 dPosC;
    s16 dVelA;
    s16 dVelB;
    s32 deltaA;
    s32 deltaB;
    s32 deltaC;
    s32 deltaD;
    s32 deltaE;
};

extern const u8 gStaticData_0861C15C[];
extern const u8 gStaticData_0861C184[];
extern const u8 gStaticData_0861C1AC[];
extern const u8 gStaticData_0861C1D4[];
extern const u8 gStaticData_0861C224[];
extern const u8 gStaticData_0862C2C8[];
extern const u8 gStaticData_0862C4B0[];
extern const u8 gStaticData_0862CC3C[];
extern const u8 gStaticData_0862CE70[];
extern const u8 gStaticData_0862E3B0[];
extern const u8 gStaticData_08631374[];
extern const u8 gStaticData_086313B0[];
extern const u8 gStaticData_086314F0[];
extern const u8 gStaticData_08631558[];
extern const u8 gStaticData_0863183C[];
/* DrawTitleLogoPieces (graphics_loading_35780.c): the {x, y} offsets of the
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

/* The palettes InitTitleScreen (level_graphics.c) DMAs to OBJ palettes
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

/* The four OBJ sprites LoadTitleScreenObjTiles (level_graphics.c) uploads,
 * through the IWRAM table gTitleObjPackages (src/iwram/iwram_data.c),
 * which lists them in the order 0817D0A8, 0817D0D0, 0817D0BC, 0817D094. */
const struct bg_package gTitleBandicootObj = { 4, 6, (void *)gStaticData_0861C15C, (void *)gStaticData_0862C2C8, (void *)gStaticData_08631374 };
const struct bg_package gTitleCrashObj = { 8, 40, (void *)gStaticData_0861C184, (void *)gStaticData_0862C4B0, (void *)gStaticData_086313B0 };
const struct bg_package gTitleArrow1Obj = { 4, 16, (void *)gStaticData_0861C1AC, (void *)gStaticData_0862CC3C, (void *)gStaticData_086314F0 };
const struct bg_package gTitleArrow2Obj = { 4, 16, (void *)gStaticData_0861C1D4, (void *)gStaticData_0862CE70, (void *)gStaticData_08631558 };

/* LoadTitleScreenBg's (level_graphics.c) BG2 picture. */
const struct bg_package gTitleScreenBg = { 16, 16, (void *)gStaticData_0861C224, (void *)gStaticData_0862E3B0, (void *)gStaticData_0863183C };

/* The motion sequences of the nine countdown slots of
 * graphics_loading_35d1c.c (RunTitleScreen, ResetTitleLogoPieces), which
 * popup_glyphs_17cf40.c's gTitleLogoPieceSeeds seeds: each ends with a
 * zero hold. */
const struct delta_record gStaticData_0817D0F8[4] = {
    { 11, 152, 160, 0, 256, 256, -494498, -959208, 0, 0, 0 },
    { 19, 69, 65535, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 69, 65535, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gStaticData_0817D178[4] = {
    { 10, 65440, 160, 0, 256, 256, 1153761, -956825, 0, 0, 0 },
    { 9, 80, 14, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 80, 14, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gStaticData_0817D1F8[6] = {
    { 7, 120, 72, 0, 51, 51, 0, 0, 0, 7489, 7489 },
    { 3, 120, 72, 0, 256, 256, 0, 0, 0, 2184, 2184 },
    { 3, 120, 72, 0, 281, 281, 0, 0, 0, -2184, -2184 },
    { 41, 120, 72, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 120, 72, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gStaticData_0817D2B8[5] = {
    { 15, 169, 61, 0, 358, 358, 0, 0, 0, -2184, -2184 },
    { 3, 169, 61, 0, 230, 230, 0, 0, 0, 2184, 2185 },
    { 48, 169, 61, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 169, 61, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gStaticData_0817D358[5] = {
    { 15, 146, 50, 0, 358, 358, 0, 0, 0, -2184, -2184 },
    { 3, 146, 50, 0, 230, 230, 0, 0, 0, 2184, 2184 },
    { 60, 146, 50, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 146, 50, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gStaticData_0817D3F8[5] = {
    { 15, 117, 50, 0, 358, 358, 0, 0, 0, -2184, -2184 },
    { 3, 117, 50, 0, 230, 230, 0, 0, 0, 2184, 2184 },
    { 72, 117, 50, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 117, 50, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gStaticData_0817D498[5] = {
    { 15, 90, 53, 0, 358, 358, 0, 0, 0, -2184, -2184 },
    { 3, 90, 53, 0, 230, 230, 0, 0, 0, 2184, 2184 },
    { 84, 90, 53, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 90, 53, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gStaticData_0817D538[5] = {
    { 15, 68, 65, 0, 358, 358, 0, 0, 0, -2184, -2184 },
    { 3, 68, 65, 0, 230, 230, 0, 0, 0, 2184, 2184 },
    { 96, 68, 65, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 68, 65, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gStaticData_0817D5D8[6] = {
    { 5, 119, 71, 0, 383, 384, 0, 0, 0, -6553, -6553 },
    { 3, 119, 71, 0, 256, 256, 0, 0, 0, -2184, -2184 },
    { 3, 119, 71, 0, 230, 230, 0, 0, 0, 2184, 2184 },
    { 30, 119, 71, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 119, 71, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

/* The animation record RunCompanyLogos (graphics_loading_35d1c.c) builds its
 * part from: actor_anim.h's `struct anim_table_record` (0x28 bytes),
 * written with its own view here - index, keyframes, frames, header
 * byte, a word, a {x, y, z, w, h, d} box and the spawn offsets. The
 * keyframes and frames are record 0's of the categories 0-2 family
 * (Crash riding the polar bear), inside gStaticData_08178F80. */
struct anim_record_view
{
    u32 index;
    const void *keyframes;
    const void *frames;
    u8 headerByte;
    u8 pad[3];
    u32 unknown_10;
    s16 box[6];
    s32 spawnX;
    s32 spawnY;
};

extern const u8 gStaticData_08178F80[];

const struct anim_record_view gLogoActorAnim = {
    0, gStaticData_08178F80 + 0x400, gStaticData_08178F80 + 0x49C,
    0, { 0 }, 0x100, { -10, -20, -1, 20, 44, 3 }, 0, 0,
};
