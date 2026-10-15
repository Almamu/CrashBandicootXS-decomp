extern "C" {
#include "gba/types.h"
#include "engine/gfx/sprite_bank.h"
#include "globals.h"
}
#include "sprites/sprite_frames.h"

/*
 * ROM 0x084a5600-0x084b0ae0: sprite banks 0-9 of the sprite-bank
 * animation system (gSpriteBankTable, include/engine/gfx/sprite_bank.h). Linked in
 * ROM order between data/data.s sections by games/xs/ldscript.txt - see
 * docs/data.md and docs/data_map.md ("gSpriteBankTable").
 *
 * Extracted once from baserom.gba by tools/sprite_banks.py; this file is
 * the source now. Per bank: the animations (a frame-index sequence each),
 * the frame pointer array, the frames (header plus the boxes/anchor of
 * their layout type), then every frame's piece positions and piece bytes.
 * The pieces and the frames' tiles come from graphics/sprites/bankNN_<name>/
 * (a PNG per frame and bank.json, see docs/graphics.md "Sprite banks"):
 * tools/sprite_frames.py generates the piece arrays (sprites/bankNN_pieces.inc),
 * and sprites/sprite_frames.h declares the frames and gives each one's
 * offset in the sprite tile pool (src/engine/gfx/data/sprite_tiles.cpp),
 * SPRITE_TILES_BANKnn_FRAMEkkk. A frame's struct type has to match its
 * "layout" in bank.json.
 */

extern const u8 gSpriteBank00Tiles[];  /* sprite_tiles.cpp, bank 0's tiles */
extern const u8 gObjPalettes[0xfa0];
extern const struct sprite_bank gSpriteBanks[56];
extern const struct sprite_anim gSpriteBank10Anims[2];
extern const struct sprite_frame *const gSpriteBank10Frames[20];
extern const struct sprite_anim gSpriteBank11Anims[2];
extern const struct sprite_frame *const gSpriteBank11Frames[25];
extern const struct sprite_anim gSpriteBank12Anims[7];
extern const struct sprite_frame *const gSpriteBank12Frames[57];
extern const struct sprite_anim gSpriteBank13Anims[2];
extern const struct sprite_frame *const gSpriteBank13Frames[19];
extern const struct sprite_anim gSpriteBank14Anims[2];
extern const struct sprite_frame *const gSpriteBank14Frames[23];
extern const struct sprite_anim gSpriteBank15Anims[5];
extern const struct sprite_frame *const gSpriteBank15Frames[58];
extern const struct sprite_anim gSpriteBank16Anims[2];
extern const struct sprite_frame *const gSpriteBank16Frames[24];
extern const struct sprite_anim gSpriteBank17Anims[1];
extern const struct sprite_frame *const gSpriteBank17Frames[15];
extern const struct sprite_anim gSpriteBank18Anims[2];
extern const struct sprite_frame *const gSpriteBank18Frames[14];
extern const struct sprite_anim gSpriteBank19Anims[2];
extern const struct sprite_frame *const gSpriteBank19Frames[35];
extern const struct sprite_anim gSpriteBank20Anims[2];
extern const struct sprite_frame *const gSpriteBank20Frames[32];
extern const struct sprite_anim gSpriteBank21Anims[2];
extern const struct sprite_frame *const gSpriteBank21Frames[15];
extern const struct sprite_anim gSpriteBank22Anims[2];
extern const struct sprite_frame *const gSpriteBank22Frames[31];
extern const struct sprite_anim gSpriteBank23Anims[5];
extern const struct sprite_frame *const gSpriteBank23Frames[59];
extern const struct sprite_anim gSpriteBank24Anims[6];
extern const struct sprite_frame *const gSpriteBank24Frames[74];
extern const struct sprite_anim gSpriteBank25Anims[1];
extern const struct sprite_frame *const gSpriteBank25Frames[16];
extern const struct sprite_anim gSpriteBank26Anims[2];
extern const struct sprite_frame *const gSpriteBank26Frames[14];
extern const struct sprite_anim gSpriteBank27Anims[2];
extern const struct sprite_frame *const gSpriteBank27Frames[30];
extern const struct sprite_anim gSpriteBank28Anims[1];
extern const struct sprite_frame *const gSpriteBank28Frames[3];
extern const struct sprite_anim gSpriteBank29Anims[4];
extern const struct sprite_frame *const gSpriteBank29Frames[42];
extern const struct sprite_anim gSpriteBank30Anims[3];
extern const struct sprite_frame *const gSpriteBank30Frames[42];
extern const struct sprite_anim gSpriteBank31Anims[36];
extern const struct sprite_frame *const gSpriteBank31Frames[112];
extern const struct sprite_anim gSpriteBank32Anims[9];
extern const struct sprite_frame *const gSpriteBank32Frames[17];
extern const struct sprite_anim gSpriteBank33Anims[3];
extern const struct sprite_frame *const gSpriteBank33Frames[16];
extern const struct sprite_anim gSpriteBank34Anims[5];
extern const struct sprite_frame *const gSpriteBank34Frames[22];
extern const struct sprite_anim gSpriteBank35Anims[2];
extern const struct sprite_frame *const gSpriteBank35Frames[28];
extern const struct sprite_anim gSpriteBank36Anims[1];
extern const struct sprite_frame *const gSpriteBank36Frames[10];
extern const struct sprite_anim gSpriteBank37Anims[1];
extern const struct sprite_frame *const gSpriteBank37Frames[8];
extern const struct sprite_anim gSpriteBank38Anims[4];
extern const struct sprite_frame *const gSpriteBank38Frames[60];
extern const struct sprite_anim gSpriteBank39Anims[13];
extern const struct sprite_frame *const gSpriteBank39Frames[24];
extern const struct sprite_anim gSpriteBank40Anims[5];
extern const struct sprite_frame *const gSpriteBank40Frames[30];
extern const struct sprite_anim gSpriteBank41Anims[3];
extern const struct sprite_frame *const gSpriteBank41Frames[24];
extern const struct sprite_anim gSpriteBank42Anims[1];
extern const struct sprite_frame *const gSpriteBank42Frames[11];
extern const struct sprite_anim gSpriteBank43Anims[7];
extern const struct sprite_frame *const gSpriteBank43Frames[21];
extern const struct sprite_anim gSpriteBank44Anims[1];
extern const struct sprite_frame *const gSpriteBank44Frames[13];
extern const struct sprite_anim gSpriteBank45Anims[1];
extern const struct sprite_frame *const gSpriteBank45Frames[13];
extern const struct sprite_anim gSpriteBank46Anims[1];
extern const struct sprite_frame *const gSpriteBank46Frames[2];
extern const struct sprite_anim gSpriteBank47Anims[13];
extern const struct sprite_frame *const gSpriteBank47Frames[43];
extern const struct sprite_anim gSpriteBank48Anims[4];
extern const struct sprite_frame *const gSpriteBank48Frames[38];
extern const struct sprite_anim gSpriteBank49Anims[5];
extern const struct sprite_frame *const gSpriteBank49Frames[2];
extern const struct sprite_anim gSpriteBank50Anims[1];
extern const struct sprite_frame *const gSpriteBank50Frames[8];
extern const struct sprite_anim gSpriteBank51Anims[5];
extern const struct sprite_frame *const gSpriteBank51Frames[24 + SPRITE_BANK51_NEW_FRAME_COUNT];
extern const struct sprite_anim gSpriteBank52Anims[2];
extern const struct sprite_frame *const gSpriteBank52Frames[4];
extern const struct sprite_anim gSpriteBank53Anims[19];
extern const struct sprite_frame *const gSpriteBank53Frames[67];
extern const struct sprite_anim gSpriteBank54Anims[10];
extern const struct sprite_frame *const gSpriteBank54Frames[121];
extern const struct sprite_anim gSpriteBank55Anims[9];
extern const struct sprite_frame *const gSpriteBank55Frames[130];
extern const struct sprite_anim gSpriteBank00Anims[48];
extern const struct sprite_frame *const gSpriteBank00Frames[465];
extern const struct sprite_anim gSpriteBank01Anims[47];
extern const struct sprite_frame *const gSpriteBank01Frames[274];
extern const struct sprite_anim gSpriteBank02Anims[2];
extern const struct sprite_frame *const gSpriteBank02Frames[17];
extern const struct sprite_anim gSpriteBank03Anims[1];
extern const struct sprite_frame *const gSpriteBank03Frames[25];
extern const struct sprite_anim gSpriteBank04Anims[2];
extern const struct sprite_frame *const gSpriteBank04Frames[32];
extern const struct sprite_anim gSpriteBank05Anims[4];
extern const struct sprite_frame *const gSpriteBank05Frames[25];
extern const struct sprite_anim gSpriteBank06Anims[1];
extern const struct sprite_frame *const gSpriteBank06Frames[7];
extern const struct sprite_anim gSpriteBank07Anims[3];
extern const struct sprite_frame *const gSpriteBank07Frames[38];
extern const struct sprite_anim gSpriteBank08Anims[3];
extern const struct sprite_frame *const gSpriteBank08Frames[36];
extern const struct sprite_anim gSpriteBank09Anims[1];
extern const struct sprite_frame *const gSpriteBank09Frames[14];

/* The root of the system: InitLevelState (spawn_markers.cpp) points
 * *gSpriteBankSet here. GetSpriteTileBase returns tileBase;
 * InitLevelState and RunPauseMenu (pause_menu.cpp) build the palette
 * cache from palettes/paletteCount. */
extern const struct sprite_bank_table gSpriteBankTable = {
    /* banks */ gSpriteBanks,
    /* tileBase */ gSpriteBank00Tiles,
    /* palettes */ gObjPalettes,
    /* bankCount */ ARRAY_COUNT(gSpriteBanks),
    /* paletteCount */ sizeof(gObjPalettes) / 32,
};

/* Bank N is `**gSpriteBankSet + 12 * N` in the code (a part's +0x20). */
const struct sprite_bank gSpriteBanks[56] = {
    /* 0 */ { gSpriteBank00Anims, gSpriteBank00Frames, 0, ARRAY_COUNT(gSpriteBank00Anims) },
    /* 1 */ { gSpriteBank01Anims, gSpriteBank01Frames, 0, ARRAY_COUNT(gSpriteBank01Anims) },
    /* 2 */ { gSpriteBank02Anims, gSpriteBank02Frames, 0, ARRAY_COUNT(gSpriteBank02Anims) },
    /* 3 */ { gSpriteBank03Anims, gSpriteBank03Frames, 0, ARRAY_COUNT(gSpriteBank03Anims) },
    /* 4 */ { gSpriteBank04Anims, gSpriteBank04Frames, 0, ARRAY_COUNT(gSpriteBank04Anims) },
    /* 5 */ { gSpriteBank05Anims, gSpriteBank05Frames, 0, ARRAY_COUNT(gSpriteBank05Anims) },
    /* 6 */ { gSpriteBank06Anims, gSpriteBank06Frames, 0, ARRAY_COUNT(gSpriteBank06Anims) },
    /* 7 */ { gSpriteBank07Anims, gSpriteBank07Frames, 0, ARRAY_COUNT(gSpriteBank07Anims) },
    /* 8 */ { gSpriteBank08Anims, gSpriteBank08Frames, 0, ARRAY_COUNT(gSpriteBank08Anims) },
    /* 9 */ { gSpriteBank09Anims, gSpriteBank09Frames, 0, ARRAY_COUNT(gSpriteBank09Anims) },
    /* 10 */ { gSpriteBank10Anims, gSpriteBank10Frames, 0, ARRAY_COUNT(gSpriteBank10Anims) },
    /* 11 */ { gSpriteBank11Anims, gSpriteBank11Frames, 0, ARRAY_COUNT(gSpriteBank11Anims) },
    /* 12 */ { gSpriteBank12Anims, gSpriteBank12Frames, 0, ARRAY_COUNT(gSpriteBank12Anims) },
    /* 13 */ { gSpriteBank13Anims, gSpriteBank13Frames, 0, ARRAY_COUNT(gSpriteBank13Anims) },
    /* 14 */ { gSpriteBank14Anims, gSpriteBank14Frames, 0, ARRAY_COUNT(gSpriteBank14Anims) },
    /* 15 */ { gSpriteBank15Anims, gSpriteBank15Frames, 0, ARRAY_COUNT(gSpriteBank15Anims) },
    /* 16 */ { gSpriteBank16Anims, gSpriteBank16Frames, 0, ARRAY_COUNT(gSpriteBank16Anims) },
    /* 17 */ { gSpriteBank17Anims, gSpriteBank17Frames, 0, ARRAY_COUNT(gSpriteBank17Anims) },
    /* 18 */ { gSpriteBank18Anims, gSpriteBank18Frames, 0, ARRAY_COUNT(gSpriteBank18Anims) },
    /* 19 */ { gSpriteBank19Anims, gSpriteBank19Frames, 0, ARRAY_COUNT(gSpriteBank19Anims) },
    /* 20 */ { gSpriteBank20Anims, gSpriteBank20Frames, 0, ARRAY_COUNT(gSpriteBank20Anims) },
    /* 21 */ { gSpriteBank21Anims, gSpriteBank21Frames, 0, ARRAY_COUNT(gSpriteBank21Anims) },
    /* 22 */ { gSpriteBank22Anims, gSpriteBank22Frames, 0, ARRAY_COUNT(gSpriteBank22Anims) },
    /* 23 */ { gSpriteBank23Anims, gSpriteBank23Frames, 0, ARRAY_COUNT(gSpriteBank23Anims) },
    /* 24 */ { gSpriteBank24Anims, gSpriteBank24Frames, 0, ARRAY_COUNT(gSpriteBank24Anims) },
    /* 25 */ { gSpriteBank25Anims, gSpriteBank25Frames, 0, ARRAY_COUNT(gSpriteBank25Anims) },
    /* 26 */ { gSpriteBank26Anims, gSpriteBank26Frames, 0, ARRAY_COUNT(gSpriteBank26Anims) },
    /* 27 */ { gSpriteBank27Anims, gSpriteBank27Frames, 0, ARRAY_COUNT(gSpriteBank27Anims) },
    /* 28 */ { gSpriteBank28Anims, gSpriteBank28Frames, 0, ARRAY_COUNT(gSpriteBank28Anims) },
    /* 29 */ { gSpriteBank29Anims, gSpriteBank29Frames, 0, ARRAY_COUNT(gSpriteBank29Anims) },
    /* 30 */ { gSpriteBank30Anims, gSpriteBank30Frames, 0, ARRAY_COUNT(gSpriteBank30Anims) },
    /* 31 */ { gSpriteBank31Anims, gSpriteBank31Frames, 0, ARRAY_COUNT(gSpriteBank31Anims) },
    /* 32 */ { gSpriteBank32Anims, gSpriteBank32Frames, 0, ARRAY_COUNT(gSpriteBank32Anims) },
    /* 33 */ { gSpriteBank33Anims, gSpriteBank33Frames, 0, ARRAY_COUNT(gSpriteBank33Anims) },
    /* 34 */ { gSpriteBank34Anims, gSpriteBank34Frames, 0, ARRAY_COUNT(gSpriteBank34Anims) },
    /* 35 */ { gSpriteBank35Anims, gSpriteBank35Frames, 0, ARRAY_COUNT(gSpriteBank35Anims) },
    /* 36 */ { gSpriteBank36Anims, gSpriteBank36Frames, 0, ARRAY_COUNT(gSpriteBank36Anims) },
    /* 37 */ { gSpriteBank37Anims, gSpriteBank37Frames, 0, ARRAY_COUNT(gSpriteBank37Anims) },
    /* 38 */ { gSpriteBank38Anims, gSpriteBank38Frames, 0, ARRAY_COUNT(gSpriteBank38Anims) },
    /* 39 */ { gSpriteBank39Anims, gSpriteBank39Frames, 0, ARRAY_COUNT(gSpriteBank39Anims) },
    /* 40 */ { gSpriteBank40Anims, gSpriteBank40Frames, 0, ARRAY_COUNT(gSpriteBank40Anims) },
    /* 41 */ { gSpriteBank41Anims, gSpriteBank41Frames, 0, ARRAY_COUNT(gSpriteBank41Anims) },
    /* 42 */ { gSpriteBank42Anims, gSpriteBank42Frames, 0, ARRAY_COUNT(gSpriteBank42Anims) },
    /* 43 */ { gSpriteBank43Anims, gSpriteBank43Frames, 0, ARRAY_COUNT(gSpriteBank43Anims) },
    /* 44 */ { gSpriteBank44Anims, gSpriteBank44Frames, 0, ARRAY_COUNT(gSpriteBank44Anims) },
    /* 45 */ { gSpriteBank45Anims, gSpriteBank45Frames, 0, ARRAY_COUNT(gSpriteBank45Anims) },
    /* 46 */ { gSpriteBank46Anims, gSpriteBank46Frames, 0, ARRAY_COUNT(gSpriteBank46Anims) },
    /* 47 */ { gSpriteBank47Anims, gSpriteBank47Frames, 0, ARRAY_COUNT(gSpriteBank47Anims) },
    /* 48 */ { gSpriteBank48Anims, gSpriteBank48Frames, 0, ARRAY_COUNT(gSpriteBank48Anims) },
    /* 49 */ { gSpriteBank49Anims, gSpriteBank49Frames, 0, ARRAY_COUNT(gSpriteBank49Anims) },
    /* 50 */ { gSpriteBank50Anims, gSpriteBank50Frames, 0, ARRAY_COUNT(gSpriteBank50Anims) },
    /* 51 */ { gSpriteBank51Anims, gSpriteBank51Frames, 0, ARRAY_COUNT(gSpriteBank51Anims) },
    /* 52 */ { gSpriteBank52Anims, gSpriteBank52Frames, 0, ARRAY_COUNT(gSpriteBank52Anims) },
    /* 53 */ { gSpriteBank53Anims, gSpriteBank53Frames, 0, ARRAY_COUNT(gSpriteBank53Anims) },
    /* 54 */ { gSpriteBank54Anims, gSpriteBank54Frames, 0, ARRAY_COUNT(gSpriteBank54Anims) },
    /* 55 */ { gSpriteBank55Anims, gSpriteBank55Frames, 0, ARRAY_COUNT(gSpriteBank55Anims) },
};

/* ---------------------------------------------------------------------- */
/* Bank 0: 48 animations, 465 frames, graphics/sprites/bank00_crash/. */

extern const u16 gSpriteBank00Anim00Seq[12];
extern const u16 gSpriteBank00Anim01Seq[3];
extern const u16 gSpriteBank00Anim02Seq[4];
extern const u16 gSpriteBank00Anim03Seq[4];
extern const u16 gSpriteBank00Anim04Seq[1];
extern const u16 gSpriteBank00Anim05Seq[24];
extern const u16 gSpriteBank00Anim06Seq[9];
extern const u16 gSpriteBank00Anim07Seq[7];
extern const u16 gSpriteBank00Anim08Seq[3];
extern const u16 gSpriteBank00Anim09Seq[3];
extern const u16 gSpriteBank00Anim10Seq[10];
extern const u16 gSpriteBank00Anim11Seq[12];
extern const u16 gSpriteBank00Anim12Seq[10];
extern const u16 gSpriteBank00Anim13Seq[20];
extern const u16 gSpriteBank00Anim14Seq[24];
extern const u16 gSpriteBank00Anim15Seq[5];
extern const u16 gSpriteBank00Anim16Seq[10];
extern const u16 gSpriteBank00Anim17Seq[29];
extern const u16 gSpriteBank00Anim18Seq[10];
extern const u16 gSpriteBank00Anim19Seq[3];
extern const u16 gSpriteBank00Anim20Seq[3];
extern const u16 gSpriteBank00Anim21Seq[3];
extern const u16 gSpriteBank00Anim22Seq[13];
extern const u16 gSpriteBank00Anim23Seq[10];
extern const u16 gSpriteBank00Anim24Seq[5];
extern const u16 gSpriteBank00Anim25Seq[4];
extern const u16 gSpriteBank00Anim26Seq[107];
extern const u16 gSpriteBank00Anim27Seq[3];
extern const u16 gSpriteBank00Anim28Seq[8];
extern const u16 gSpriteBank00Anim29Seq[2];
extern const u16 gSpriteBank00Anim30Seq[10];
extern const u16 gSpriteBank00Anim31Seq[15];
extern const u16 gSpriteBank00Anim32Seq[3];
extern const u16 gSpriteBank00Anim33Seq[14];
extern const u16 gSpriteBank00Anim34Seq[3];
extern const u16 gSpriteBank00Anim35Seq[3];
extern const u16 gSpriteBank00Anim36Seq[19];
extern const u16 gSpriteBank00Anim37Seq[8];
extern const u16 gSpriteBank00Anim38Seq[12];
extern const u16 gSpriteBank00Anim39Seq[10];
extern const u16 gSpriteBank00Anim40Seq[10];
extern const u16 gSpriteBank00Anim41Seq[19];
extern const u16 gSpriteBank00Anim42Seq[35];
extern const u16 gSpriteBank00Anim43Seq[27];
extern const u16 gSpriteBank00Anim44Seq[15];
extern const u16 gSpriteBank00Anim45Seq[16];
extern const u16 gSpriteBank00Anim46Seq[24];
extern const u16 gSpriteBank00Anim47Seq[10];

const struct sprite_anim gSpriteBank00Anims[48] = {
    /* 0 */ {
        /* seq */ gSpriteBank00Anim00Seq,
        /* box */ { { -7, -6, 14, 20 }, { -21, -8, 41, 25 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank00Anim01Seq,
        /* box */ { { -7, -6, 14, 20 }, { -16, -14, 34, 29 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank00Anim02Seq,
        /* box */ { { -7, -27, 14, 41 }, { -10, -25, 26, 38 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank00Anim03Seq,
        /* box */ { { -7, -6, 14, 20 }, { -10, -25, 26, 38 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim03Seq),
        /* flags */ 0,
    },
    /* 4 */ {
        /* seq */ gSpriteBank00Anim04Seq,
        /* box */ { { -7, -6, 14, 20 }, { -6, -14, 22, 26 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim04Seq),
        /* flags */ 0,
    },
    /* 5 */ {
        /* seq */ gSpriteBank00Anim05Seq,
        /* box */ { { -7, -27, 14, 41 }, { -10, -30, 20, 43 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim05Seq),
        /* flags */ 0,
    },
    /* 6 */ {
        /* seq */ gSpriteBank00Anim06Seq,
        /* box */ { { -7, -27, 14, 41 }, { -18, -32, 40, 50 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim06Seq),
        /* flags */ 0,
    },
    /* 7 */ {
        /* seq */ gSpriteBank00Anim07Seq,
        /* box */ { { -7, -27, 14, 41 }, { -32, -23, 55, 26 } },
        /* paletteId */ 0,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim07Seq),
        /* flags */ 0,
    },
    /* 8 */ {
        /* seq */ gSpriteBank00Anim08Seq,
        /* box */ { { -7, -27, 14, 41 }, { -17, -24, 40, 38 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim08Seq),
        /* flags */ 0,
    },
    /* 9 */ {
        /* seq */ gSpriteBank00Anim09Seq,
        /* box */ { { -7, -27, 14, 41 }, { -25, -23, 48, 32 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim09Seq),
        /* flags */ 0,
    },
    /* 10 */ {
        /* seq */ gSpriteBank00Anim10Seq,
        /* box */ { { -7, -27, 14, 41 }, { -10, -29, 25, 49 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim10Seq),
        /* flags */ 0,
    },
    /* 11 */ {
        /* seq */ gSpriteBank00Anim11Seq,
        /* box */ { { -7, -27, 14, 41 }, { -11, -29, 30, 51 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim11Seq),
        /* flags */ 0,
    },
    /* 12 */ {
        /* seq */ gSpriteBank00Anim12Seq,
        /* box */ { { -7, -27, 14, 41 }, { -15, -34, 24, 51 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim12Seq),
        /* flags */ 0,
    },
    /* 13 */ {
        /* seq */ gSpriteBank00Anim13Seq,
        /* box */ { { -7, -27, 14, 41 }, { -22, -36, 41, 48 } },
        /* paletteId */ 0,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim13Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 14 */ {
        /* seq */ gSpriteBank00Anim14Seq,
        /* box */ { { -7, -27, 14, 41 }, { -14, -28, 24, 41 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim14Seq),
        /* flags */ 0,
    },
    /* 15 */ {
        /* seq */ gSpriteBank00Anim15Seq,
        /* box */ { { -7, -6, 14, 20 }, { -28, -27, 45, 42 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim15Seq),
        /* flags */ 0,
    },
    /* 16 */ {
        /* seq */ gSpriteBank00Anim16Seq,
        /* box */ { { -7, -27, 14, 41 }, { -25, -24, 45, 38 } },
        /* paletteId */ 0,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim16Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 17 */ {
        /* seq */ gSpriteBank00Anim17Seq,
        /* box */ { { -7, -27, 14, 41 }, { -33, -25, 63, 48 } },
        /* paletteId */ 0,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim17Seq),
        /* flags */ 0,
    },
    /* 18 */ {
        /* seq */ gSpriteBank00Anim18Seq,
        /* box */ { { -7, -27, 14, 41 }, { -11, -25, 22, 38 } },
        /* paletteId */ 1,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim18Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 19 */ {
        /* seq */ gSpriteBank00Anim19Seq,
        /* box */ { { -7, -27, 14, 41 }, { -15, -35, 27, 54 } },
        /* paletteId */ 0,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim19Seq),
        /* flags */ 0,
    },
    /* 20 */ {
        /* seq */ gSpriteBank00Anim20Seq,
        /* box */ { { -7, -6, 14, 20 }, { -16, -14, 34, 29 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim20Seq),
        /* flags */ 0,
    },
    /* 21 */ {
        /* seq */ gSpriteBank00Anim21Seq,
        /* box */ { { -7, -27, 14, 41 }, { -15, -33, 27, 52 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim21Seq),
        /* flags */ 0,
    },
    /* 22 */ {
        /* seq */ gSpriteBank00Anim22Seq,
        /* box */ { { -7, -27, 14, 41 }, { -17, -28, 31, 44 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim22Seq),
        /* flags */ 0,
    },
    /* 23 */ {
        /* seq */ gSpriteBank00Anim23Seq,
        /* box */ { { -7, -27, 14, 41 }, { -25, -24, 45, 38 } },
        /* paletteId */ 2,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim23Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 24 */ {
        /* seq */ gSpriteBank00Anim24Seq,
        /* box */ { { -7, -27, 14, 41 }, { -20, -26, 38, 39 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim24Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 25 */ {
        /* seq */ gSpriteBank00Anim25Seq,
        /* box */ { { -7, -27, 14, 41 }, { -24, -33, 47, 47 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim25Seq),
        /* flags */ 0,
    },
    /* 26 */ {
        /* seq */ gSpriteBank00Anim26Seq,
        /* box */ { { -7, -27, 14, 41 }, { -24, -25, 50, 39 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim26Seq),
        /* flags */ 0,
    },
    /* 27 */ {
        /* seq */ gSpriteBank00Anim27Seq,
        /* box */ { { -7, -27, 14, 41 }, { -20, -34, 39, 53 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim27Seq),
        /* flags */ 0,
    },
    /* 28 */ {
        /* seq */ gSpriteBank00Anim28Seq,
        /* box */ { { -25, -26, 36, 41 }, { -25, -28, 43, 44 } },
        /* paletteId */ 3,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim28Seq),
        /* flags */ 0,
    },
    /* 29 */ {
        /* seq */ gSpriteBank00Anim29Seq,
        /* box */ { { -7, -27, 14, 41 }, { -12, -28, 21, 43 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim29Seq),
        /* flags */ 0,
    },
    /* 30 */ {
        /* seq */ gSpriteBank00Anim30Seq,
        /* box */ { { -7, -27, 14, 41 }, { -33, -28, 63, 34 } },
        /* paletteId */ 0,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim30Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 31 */ {
        /* seq */ gSpriteBank00Anim31Seq,
        /* box */ { { -7, -27, 14, 41 }, { -17, -28, 28, 38 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim31Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 32 */ {
        /* seq */ gSpriteBank00Anim32Seq,
        /* box */ { { -7, -27, 14, 41 }, { -12, -28, 25, 39 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim32Seq),
        /* flags */ 0,
    },
    /* 33 */ {
        /* seq */ gSpriteBank00Anim33Seq,
        /* box */ { { -7, -27, 14, 41 }, { -27, -28, 42, 48 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim33Seq),
        /* flags */ 0,
    },
    /* 34 */ {
        /* seq */ gSpriteBank00Anim34Seq,
        /* box */ { { -7, -27, 14, 41 }, { -25, -28, 35, 38 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim34Seq),
        /* flags */ 0,
    },
    /* 35 */ {
        /* seq */ gSpriteBank00Anim35Seq,
        /* box */ { { -7, -27, 14, 41 }, { -13, -28, 24, 42 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim35Seq),
        /* flags */ 0,
    },
    /* 36 */ {
        /* seq */ gSpriteBank00Anim36Seq,
        /* box */ { { -11, -26, 21, 39 }, { -42, -72, 85, 94 } },
        /* paletteId */ 4,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim36Seq),
        /* flags */ 0,
    },
    /* 37 */ {
        /* seq */ gSpriteBank00Anim37Seq,
        /* box */ { { -7, -27, 14, 41 }, { -16, -24, 28, 36 } },
        /* paletteId */ 2,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim37Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 38 */ {
        /* seq */ gSpriteBank00Anim38Seq,
        /* box */ { { -7, -27, 14, 41 }, { -16, -24, 32, 38 } },
        /* paletteId */ 2,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim38Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 39 */ {
        /* seq */ gSpriteBank00Anim39Seq,
        /* box */ { { -7, -27, 14, 41 }, { -25, -24, 45, 38 } },
        /* paletteId */ 2,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim39Seq),
        /* flags */ 0,
    },
    /* 40 */ {
        /* seq */ gSpriteBank00Anim40Seq,
        /* box */ { { -7, -27, 14, 41 }, { -25, -24, 45, 38 } },
        /* paletteId */ 2,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim40Seq),
        /* flags */ 0,
    },
    /* 41 */ {
        /* seq */ gSpriteBank00Anim41Seq,
        /* box */ { { -11, -26, 21, 39 }, { -42, -72, 85, 94 } },
        /* paletteId */ 4,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim41Seq),
        /* flags */ 0,
    },
    /* 42 */ {
        /* seq */ gSpriteBank00Anim42Seq,
        /* box */ { { -10, -10, 20, 20 }, { -13, -10, 24, 20 } },
        /* paletteId */ 109,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim42Seq),
        /* flags */ 0,
    },
    /* 43 */ {
        /* seq */ gSpriteBank00Anim43Seq,
        /* box */ { { -12, -18, 25, 37 }, { -12, -18, 44, 51 } },
        /* paletteId */ 114,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim43Seq),
        /* flags */ 0,
    },
    /* 44 */ {
        /* seq */ gSpriteBank00Anim44Seq,
        /* box */ { { -14, -23, 28, 46 }, { -14, -23, 28, 46 } },
        /* paletteId */ 115,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim44Seq),
        /* flags */ 0,
    },
    /* 45 */ {
        /* seq */ gSpriteBank00Anim45Seq,
        /* box */ { { -12, -18, 25, 36 }, { -22, -29, 44, 48 } },
        /* paletteId */ 109,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim45Seq),
        /* flags */ 0,
    },
    /* 46 */ {
        /* seq */ gSpriteBank00Anim46Seq,
        /* box */ { { -11, -19, 23, 39 }, { -15, -19, 30, 40 } },
        /* paletteId */ 110,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim46Seq),
        /* flags */ 0,
    },
    /* 47 */ {
        /* seq */ gSpriteBank00Anim47Seq,
        /* box */ { { -10, -21, 20, 43 }, { -23, -27, 48, 49 } },
        /* paletteId */ 0,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim47Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank00Anim00Seq[12] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
};
const u16 gSpriteBank00Anim01Seq[3] = {
    12, 13, 14,
};
const u16 gSpriteBank00Anim02Seq[4] = {
    15, 16, 17, 18,
};
const u16 gSpriteBank00Anim03Seq[4] = {
    19, 20, 21, 22,
};
const u16 gSpriteBank00Anim04Seq[1] = {
    23,
};
const u16 gSpriteBank00Anim05Seq[24] = {
    24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47,
};
const u16 gSpriteBank00Anim06Seq[9] = {
    48, 49, 50, 51, 52, 53, 54, 55, 56,
};
const u16 gSpriteBank00Anim07Seq[7] = {
    57, 58, 59, 60, 61, 62, 63,
};
const u16 gSpriteBank00Anim08Seq[3] = {
    64, 65, 66,
};
const u16 gSpriteBank00Anim09Seq[3] = {
    67, 68, 69,
};
const u16 gSpriteBank00Anim10Seq[10] = {
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
};
const u16 gSpriteBank00Anim11Seq[12] = {
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91,
};
const u16 gSpriteBank00Anim12Seq[10] = {
    92, 93, 94, 95, 96, 97, 98, 99, 100, 101,
};
const u16 gSpriteBank00Anim13Seq[20] = {
    102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117,
    118, 119, 120, 121,
};
const u16 gSpriteBank00Anim14Seq[24] = {
    122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 132, 131,
    130, 129, 128, 127, 126, 125, 124, 123,
};
const u16 gSpriteBank00Anim15Seq[5] = {
    136, 137, 138, 139, 140,
};
const u16 gSpriteBank00Anim16Seq[10] = {
    141, 142, 143, 144, 145, 146, 147, 148, 149, 150,
};
const u16 gSpriteBank00Anim17Seq[29] = {
    151, 152, 153, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 155, 156, 157,
    158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170,
};
const u16 gSpriteBank00Anim18Seq[10] = {
    171, 172, 173, 174, 175, 176, 177, 178, 179, 180,
};
const u16 gSpriteBank00Anim19Seq[3] = {
    181, 182, 183,
};
const u16 gSpriteBank00Anim20Seq[3] = {
    184, 185, 186,
};
const u16 gSpriteBank00Anim21Seq[3] = {
    187, 188, 189,
};
const u16 gSpriteBank00Anim22Seq[13] = {
    190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202,
};
const u16 gSpriteBank00Anim23Seq[10] = {
    203, 204, 205, 206, 207, 208, 209, 210, 211, 212,
};
const u16 gSpriteBank00Anim24Seq[5] = {
    213, 214, 215, 216, 217,
};
const u16 gSpriteBank00Anim25Seq[4] = {
    218, 219, 220, 221,
};
const u16 gSpriteBank00Anim26Seq[107] = {
    222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237,
    238, 239, 240, 241, 242, 243, 244, 245, 246, 246, 247, 247, 246, 246, 248, 248,
    249, 250, 245, 244, 243, 242, 241, 240, 251, 252, 253, 254, 255, 256, 257, 258,
    259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 256, 257, 258, 259, 260, 261,
    262, 263, 264, 265, 266, 267, 268, 256, 257, 258, 259, 260, 261, 262, 263, 264,
    265, 266, 267, 268, 255, 254, 253, 252, 251, 239, 238, 237, 236, 235, 234, 233,
    232, 269, 270, 229, 228, 227, 226, 225, 224, 223, 222,
};
const u16 gSpriteBank00Anim27Seq[3] = {
    271, 272, 273,
};
const u16 gSpriteBank00Anim28Seq[8] = {
    274, 275, 276, 277, 278, 279, 280, 281,
};
const u16 gSpriteBank00Anim29Seq[2] = {
    282, 283,
};
const u16 gSpriteBank00Anim30Seq[10] = {
    284, 285, 286, 287, 288, 284, 285, 286, 287, 288,
};
const u16 gSpriteBank00Anim31Seq[15] = {
    289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 301, 302, 303,
};
const u16 gSpriteBank00Anim32Seq[3] = {
    304, 305, 306,
};
const u16 gSpriteBank00Anim33Seq[14] = {
    307, 308, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318, 319, 320,
};
const u16 gSpriteBank00Anim34Seq[3] = {
    321, 322, 323,
};
const u16 gSpriteBank00Anim35Seq[3] = {
    324, 325, 326,
};
const u16 gSpriteBank00Anim36Seq[19] = {
    327, 328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 340, 341, 342,
    343, 344, 345,
};
const u16 gSpriteBank00Anim37Seq[8] = {
    346, 347, 348, 349, 350, 351, 352, 353,
};
const u16 gSpriteBank00Anim38Seq[12] = {
    354, 355, 356, 357, 358, 359, 360, 361, 362, 363, 364, 365,
};
const u16 gSpriteBank00Anim39Seq[10] = {
    366, 367, 368, 369, 370, 371, 372, 373, 374, 375,
};
const u16 gSpriteBank00Anim40Seq[10] = {
    376, 377, 378, 379, 380, 381, 382, 383, 384, 385,
};
const u16 gSpriteBank00Anim41Seq[19] = {
    386, 387, 388, 389, 390, 391, 392, 393, 394, 395, 396, 397, 398, 399, 400, 401,
    402, 403, 327,
};
const u16 gSpriteBank00Anim42Seq[35] = {
    404, 405, 406, 407, 408, 409, 410, 411, 411, 411, 411, 411, 411, 412, 413, 411,
    411, 411, 411, 411, 411, 411, 411, 411, 411, 413, 411, 411, 413, 412, 411, 411,
    411, 411, 411,
};
const u16 gSpriteBank00Anim43Seq[27] = {
    414, 415, 416, 417, 418, 418, 418, 418, 418, 418, 419, 420, 421, 422, 423, 424,
    425, 426, 427, 428, 428, 428, 428, 428, 428, 428, 428,
};
const u16 gSpriteBank00Anim44Seq[15] = {
    429, 430, 431, 429, 430, 431, 429, 430, 431, 429, 430, 431, 429, 430, 431,
};
const u16 gSpriteBank00Anim45Seq[16] = {
    432, 433, 434, 434, 434, 434, 434, 434, 434, 434, 435, 436, 437, 438, 439, 440,
};
const u16 gSpriteBank00Anim46Seq[24] = {
    441, 441, 441, 441, 442, 443, 444, 445, 446, 447, 448, 449, 450, 451, 452, 453,
    454, 454, 454, 454, 454, 454, 454, 454,
};
const u16 gSpriteBank00Anim47Seq[10] = {
    455, 456, 457, 458, 459, 460, 461, 462, 463, 464,
};

const struct sprite_frame *const gSpriteBank00Frames[465] = {
    &gSpriteBank00Frame000.frame,
    &gSpriteBank00Frame001.frame,
    &gSpriteBank00Frame002.frame,
    &gSpriteBank00Frame003.frame,
    &gSpriteBank00Frame004.frame,
    &gSpriteBank00Frame005.frame,
    &gSpriteBank00Frame006.frame,
    &gSpriteBank00Frame007.frame,
    &gSpriteBank00Frame008.frame,
    &gSpriteBank00Frame009.frame,
    &gSpriteBank00Frame010.frame,
    &gSpriteBank00Frame011.frame,
    &gSpriteBank00Frame012.frame,
    &gSpriteBank00Frame013.frame,
    &gSpriteBank00Frame014.frame,
    &gSpriteBank00Frame015.frame,
    &gSpriteBank00Frame016.frame,
    &gSpriteBank00Frame017.frame,
    &gSpriteBank00Frame018.frame,
    &gSpriteBank00Frame019.frame,
    &gSpriteBank00Frame020.frame,
    &gSpriteBank00Frame021.frame,
    &gSpriteBank00Frame022.frame,
    &gSpriteBank00Frame023.frame,
    &gSpriteBank00Frame024.frame,
    &gSpriteBank00Frame025.frame,
    &gSpriteBank00Frame026.frame,
    &gSpriteBank00Frame027.frame,
    &gSpriteBank00Frame028.frame,
    &gSpriteBank00Frame029.frame,
    &gSpriteBank00Frame030.frame,
    &gSpriteBank00Frame031.frame,
    &gSpriteBank00Frame032.frame,
    &gSpriteBank00Frame033.frame,
    &gSpriteBank00Frame034.frame,
    &gSpriteBank00Frame035.frame,
    &gSpriteBank00Frame036.frame,
    &gSpriteBank00Frame037.frame,
    &gSpriteBank00Frame038.frame,
    &gSpriteBank00Frame039.frame,
    &gSpriteBank00Frame040.frame,
    &gSpriteBank00Frame041.frame,
    &gSpriteBank00Frame042.frame,
    &gSpriteBank00Frame043.frame,
    &gSpriteBank00Frame044.frame,
    &gSpriteBank00Frame045.frame,
    &gSpriteBank00Frame046.frame,
    &gSpriteBank00Frame047.frame,
    &gSpriteBank00Frame048.frame,
    &gSpriteBank00Frame049.frame,
    &gSpriteBank00Frame050.frame,
    &gSpriteBank00Frame051.frame,
    &gSpriteBank00Frame052.frame,
    &gSpriteBank00Frame053.frame,
    &gSpriteBank00Frame054.frame,
    &gSpriteBank00Frame055.frame,
    &gSpriteBank00Frame056.frame,
    &gSpriteBank00Frame057.frame,
    &gSpriteBank00Frame058.frame,
    &gSpriteBank00Frame059.frame,
    &gSpriteBank00Frame060.frame,
    &gSpriteBank00Frame061.frame,
    &gSpriteBank00Frame062.frame,
    &gSpriteBank00Frame063.frame,
    &gSpriteBank00Frame064.frame,
    &gSpriteBank00Frame065.frame,
    &gSpriteBank00Frame066.frame,
    &gSpriteBank00Frame067.frame,
    &gSpriteBank00Frame068.frame,
    &gSpriteBank00Frame069.frame,
    &gSpriteBank00Frame070.frame,
    &gSpriteBank00Frame071.frame,
    &gSpriteBank00Frame072.frame,
    &gSpriteBank00Frame073.frame,
    &gSpriteBank00Frame074.frame,
    &gSpriteBank00Frame075.frame,
    &gSpriteBank00Frame076.frame,
    &gSpriteBank00Frame077.frame,
    &gSpriteBank00Frame078.frame,
    &gSpriteBank00Frame079.frame,
    &gSpriteBank00Frame080.frame,
    &gSpriteBank00Frame081.frame,
    &gSpriteBank00Frame082.frame,
    &gSpriteBank00Frame083.frame,
    &gSpriteBank00Frame084.frame,
    &gSpriteBank00Frame085.frame,
    &gSpriteBank00Frame086.frame,
    &gSpriteBank00Frame087.frame,
    &gSpriteBank00Frame088.frame,
    &gSpriteBank00Frame089.frame,
    &gSpriteBank00Frame090.frame,
    &gSpriteBank00Frame091.frame,
    &gSpriteBank00Frame092.frame,
    &gSpriteBank00Frame093.frame,
    &gSpriteBank00Frame094.frame,
    &gSpriteBank00Frame095.frame,
    &gSpriteBank00Frame096.frame,
    &gSpriteBank00Frame097.frame,
    &gSpriteBank00Frame098.frame,
    &gSpriteBank00Frame099.frame,
    &gSpriteBank00Frame100.frame,
    &gSpriteBank00Frame101.frame,
    &gSpriteBank00Frame102.frame,
    &gSpriteBank00Frame103.frame,
    &gSpriteBank00Frame104.frame,
    &gSpriteBank00Frame105.frame,
    &gSpriteBank00Frame106.frame,
    &gSpriteBank00Frame107.frame,
    &gSpriteBank00Frame108.frame,
    &gSpriteBank00Frame109.frame,
    &gSpriteBank00Frame110.frame,
    &gSpriteBank00Frame111.frame,
    &gSpriteBank00Frame112.frame,
    &gSpriteBank00Frame113.frame,
    &gSpriteBank00Frame114.frame,
    &gSpriteBank00Frame115.frame,
    &gSpriteBank00Frame116.frame,
    &gSpriteBank00Frame117.frame,
    &gSpriteBank00Frame118.frame,
    &gSpriteBank00Frame119.frame,
    &gSpriteBank00Frame120.frame,
    &gSpriteBank00Frame121.frame,
    &gSpriteBank00Frame122.frame,
    &gSpriteBank00Frame123.frame,
    &gSpriteBank00Frame124.frame,
    &gSpriteBank00Frame125.frame,
    &gSpriteBank00Frame126.frame,
    &gSpriteBank00Frame127.frame,
    &gSpriteBank00Frame128.frame,
    &gSpriteBank00Frame129.frame,
    &gSpriteBank00Frame130.frame,
    &gSpriteBank00Frame131.frame,
    &gSpriteBank00Frame132.frame,
    &gSpriteBank00Frame133.frame,
    &gSpriteBank00Frame134.frame,
    &gSpriteBank00Frame135.frame,
    &gSpriteBank00Frame136.frame,
    &gSpriteBank00Frame137.frame,
    &gSpriteBank00Frame138.frame,
    &gSpriteBank00Frame139.frame,
    &gSpriteBank00Frame140.frame,
    &gSpriteBank00Frame141.frame,
    &gSpriteBank00Frame142.frame,
    &gSpriteBank00Frame143.frame,
    &gSpriteBank00Frame144.frame,
    &gSpriteBank00Frame145.frame,
    &gSpriteBank00Frame146.frame,
    &gSpriteBank00Frame147.frame,
    &gSpriteBank00Frame148.frame,
    &gSpriteBank00Frame149.frame,
    &gSpriteBank00Frame150.frame,
    &gSpriteBank00Frame151.frame,
    &gSpriteBank00Frame152.frame,
    &gSpriteBank00Frame153.frame,
    &gSpriteBank00Frame154.frame,
    &gSpriteBank00Frame155.frame,
    &gSpriteBank00Frame156.frame,
    &gSpriteBank00Frame157.frame,
    &gSpriteBank00Frame158.frame,
    &gSpriteBank00Frame159.frame,
    &gSpriteBank00Frame160.frame,
    &gSpriteBank00Frame161.frame,
    &gSpriteBank00Frame162.frame,
    &gSpriteBank00Frame163.frame,
    &gSpriteBank00Frame164.frame,
    &gSpriteBank00Frame165.frame,
    &gSpriteBank00Frame166.frame,
    &gSpriteBank00Frame167.frame,
    &gSpriteBank00Frame168.frame,
    &gSpriteBank00Frame169.frame,
    &gSpriteBank00Frame170.frame,
    &gSpriteBank00Frame171.frame,
    &gSpriteBank00Frame172.frame,
    &gSpriteBank00Frame173.frame,
    &gSpriteBank00Frame174.frame,
    &gSpriteBank00Frame175.frame,
    &gSpriteBank00Frame176.frame,
    &gSpriteBank00Frame177.frame,
    &gSpriteBank00Frame178.frame,
    &gSpriteBank00Frame179.frame,
    &gSpriteBank00Frame180.frame,
    &gSpriteBank00Frame181.frame,
    &gSpriteBank00Frame182.frame,
    &gSpriteBank00Frame183.frame,
    &gSpriteBank00Frame184.frame,
    &gSpriteBank00Frame185.frame,
    &gSpriteBank00Frame186.frame,
    &gSpriteBank00Frame187.frame,
    &gSpriteBank00Frame188.frame,
    &gSpriteBank00Frame189.frame,
    &gSpriteBank00Frame190.frame,
    &gSpriteBank00Frame191.frame,
    &gSpriteBank00Frame192.frame,
    &gSpriteBank00Frame193.frame,
    &gSpriteBank00Frame194.frame,
    &gSpriteBank00Frame195.frame,
    &gSpriteBank00Frame196.frame,
    &gSpriteBank00Frame197.frame,
    &gSpriteBank00Frame198.frame,
    &gSpriteBank00Frame199.frame,
    &gSpriteBank00Frame200.frame,
    &gSpriteBank00Frame201.frame,
    &gSpriteBank00Frame202.frame,
    &gSpriteBank00Frame203.frame,
    &gSpriteBank00Frame204.frame,
    &gSpriteBank00Frame205.frame,
    &gSpriteBank00Frame206.frame,
    &gSpriteBank00Frame207.frame,
    &gSpriteBank00Frame208.frame,
    &gSpriteBank00Frame209.frame,
    &gSpriteBank00Frame210.frame,
    &gSpriteBank00Frame211.frame,
    &gSpriteBank00Frame212.frame,
    &gSpriteBank00Frame213.frame,
    &gSpriteBank00Frame214.frame,
    &gSpriteBank00Frame215.frame,
    &gSpriteBank00Frame216.frame,
    &gSpriteBank00Frame217.frame,
    &gSpriteBank00Frame218.frame,
    &gSpriteBank00Frame219.frame,
    &gSpriteBank00Frame220.frame,
    &gSpriteBank00Frame221.frame,
    &gSpriteBank00Frame222.frame,
    &gSpriteBank00Frame223.frame,
    &gSpriteBank00Frame224.frame,
    &gSpriteBank00Frame225.frame,
    &gSpriteBank00Frame226.frame,
    &gSpriteBank00Frame227.frame,
    &gSpriteBank00Frame228.frame,
    &gSpriteBank00Frame229.frame,
    &gSpriteBank00Frame230.frame,
    &gSpriteBank00Frame231.frame,
    &gSpriteBank00Frame232.frame,
    &gSpriteBank00Frame233.frame,
    &gSpriteBank00Frame234.frame,
    &gSpriteBank00Frame235.frame,
    &gSpriteBank00Frame236.frame,
    &gSpriteBank00Frame237.frame,
    &gSpriteBank00Frame238.frame,
    &gSpriteBank00Frame239.frame,
    &gSpriteBank00Frame240.frame,
    &gSpriteBank00Frame241.frame,
    &gSpriteBank00Frame242.frame,
    &gSpriteBank00Frame243.frame,
    &gSpriteBank00Frame244.frame,
    &gSpriteBank00Frame245.frame,
    &gSpriteBank00Frame246.frame,
    &gSpriteBank00Frame247.frame,
    &gSpriteBank00Frame248.frame,
    &gSpriteBank00Frame249.frame,
    &gSpriteBank00Frame250.frame,
    &gSpriteBank00Frame251.frame,
    &gSpriteBank00Frame252.frame,
    &gSpriteBank00Frame253.frame,
    &gSpriteBank00Frame254.frame,
    &gSpriteBank00Frame255.frame,
    &gSpriteBank00Frame256.frame,
    &gSpriteBank00Frame257.frame,
    &gSpriteBank00Frame258.frame,
    &gSpriteBank00Frame259.frame,
    &gSpriteBank00Frame260.frame,
    &gSpriteBank00Frame261.frame,
    &gSpriteBank00Frame262.frame,
    &gSpriteBank00Frame263.frame,
    &gSpriteBank00Frame264.frame,
    &gSpriteBank00Frame265.frame,
    &gSpriteBank00Frame266.frame,
    &gSpriteBank00Frame267.frame,
    &gSpriteBank00Frame268.frame,
    &gSpriteBank00Frame269.frame,
    &gSpriteBank00Frame270.frame,
    &gSpriteBank00Frame271.frame,
    &gSpriteBank00Frame272.frame,
    &gSpriteBank00Frame273.frame,
    &gSpriteBank00Frame274.frame,
    &gSpriteBank00Frame275.frame,
    &gSpriteBank00Frame276.frame,
    &gSpriteBank00Frame277.frame,
    &gSpriteBank00Frame278.frame,
    &gSpriteBank00Frame279.frame,
    &gSpriteBank00Frame280.frame,
    &gSpriteBank00Frame281.frame,
    &gSpriteBank00Frame282.frame,
    &gSpriteBank00Frame283.frame,
    &gSpriteBank00Frame284.frame,
    &gSpriteBank00Frame285.frame,
    &gSpriteBank00Frame286.frame,
    &gSpriteBank00Frame287.frame,
    &gSpriteBank00Frame288.frame,
    &gSpriteBank00Frame289.frame,
    &gSpriteBank00Frame290.frame,
    &gSpriteBank00Frame291.frame,
    &gSpriteBank00Frame292.frame,
    &gSpriteBank00Frame293.frame,
    &gSpriteBank00Frame294.frame,
    &gSpriteBank00Frame295.frame,
    &gSpriteBank00Frame296.frame,
    &gSpriteBank00Frame297.frame,
    &gSpriteBank00Frame298.frame,
    &gSpriteBank00Frame299.frame,
    &gSpriteBank00Frame300.frame,
    &gSpriteBank00Frame301.frame,
    &gSpriteBank00Frame302.frame,
    &gSpriteBank00Frame303.frame,
    &gSpriteBank00Frame304.frame,
    &gSpriteBank00Frame305.frame,
    &gSpriteBank00Frame306.frame,
    &gSpriteBank00Frame307.frame,
    &gSpriteBank00Frame308.frame,
    &gSpriteBank00Frame309.frame,
    &gSpriteBank00Frame310.frame,
    &gSpriteBank00Frame311.frame,
    &gSpriteBank00Frame312.frame,
    &gSpriteBank00Frame313.frame,
    &gSpriteBank00Frame314.frame,
    &gSpriteBank00Frame315.frame,
    &gSpriteBank00Frame316.frame,
    &gSpriteBank00Frame317.frame,
    &gSpriteBank00Frame318.frame,
    &gSpriteBank00Frame319.frame,
    &gSpriteBank00Frame320.frame,
    &gSpriteBank00Frame321.frame,
    &gSpriteBank00Frame322.frame,
    &gSpriteBank00Frame323.frame,
    &gSpriteBank00Frame324.frame,
    &gSpriteBank00Frame325.frame,
    &gSpriteBank00Frame326.frame,
    &gSpriteBank00Frame327,
    &gSpriteBank00Frame328,
    &gSpriteBank00Frame329,
    &gSpriteBank00Frame330,
    &gSpriteBank00Frame331,
    &gSpriteBank00Frame332,
    &gSpriteBank00Frame333,
    &gSpriteBank00Frame334,
    &gSpriteBank00Frame335,
    &gSpriteBank00Frame336,
    &gSpriteBank00Frame337,
    &gSpriteBank00Frame338,
    &gSpriteBank00Frame339,
    &gSpriteBank00Frame340,
    &gSpriteBank00Frame341,
    &gSpriteBank00Frame342,
    &gSpriteBank00Frame343,
    &gSpriteBank00Frame344,
    &gSpriteBank00Frame345,
    &gSpriteBank00Frame346.frame,
    &gSpriteBank00Frame347.frame,
    &gSpriteBank00Frame348.frame,
    &gSpriteBank00Frame349.frame,
    &gSpriteBank00Frame350.frame,
    &gSpriteBank00Frame351.frame,
    &gSpriteBank00Frame352.frame,
    &gSpriteBank00Frame353.frame,
    &gSpriteBank00Frame354.frame,
    &gSpriteBank00Frame355.frame,
    &gSpriteBank00Frame356.frame,
    &gSpriteBank00Frame357.frame,
    &gSpriteBank00Frame358.frame,
    &gSpriteBank00Frame359.frame,
    &gSpriteBank00Frame360.frame,
    &gSpriteBank00Frame361.frame,
    &gSpriteBank00Frame362.frame,
    &gSpriteBank00Frame363.frame,
    &gSpriteBank00Frame364.frame,
    &gSpriteBank00Frame365.frame,
    &gSpriteBank00Frame366.frame,
    &gSpriteBank00Frame367.frame,
    &gSpriteBank00Frame368.frame,
    &gSpriteBank00Frame369.frame,
    &gSpriteBank00Frame370.frame,
    &gSpriteBank00Frame371.frame,
    &gSpriteBank00Frame372.frame,
    &gSpriteBank00Frame373.frame,
    &gSpriteBank00Frame374.frame,
    &gSpriteBank00Frame375.frame,
    &gSpriteBank00Frame376.frame,
    &gSpriteBank00Frame377.frame,
    &gSpriteBank00Frame378.frame,
    &gSpriteBank00Frame379.frame,
    &gSpriteBank00Frame380.frame,
    &gSpriteBank00Frame381.frame,
    &gSpriteBank00Frame382.frame,
    &gSpriteBank00Frame383.frame,
    &gSpriteBank00Frame384.frame,
    &gSpriteBank00Frame385.frame,
    &gSpriteBank00Frame386,
    &gSpriteBank00Frame387,
    &gSpriteBank00Frame388,
    &gSpriteBank00Frame389,
    &gSpriteBank00Frame390,
    &gSpriteBank00Frame391,
    &gSpriteBank00Frame392,
    &gSpriteBank00Frame393,
    &gSpriteBank00Frame394,
    &gSpriteBank00Frame395,
    &gSpriteBank00Frame396,
    &gSpriteBank00Frame397,
    &gSpriteBank00Frame398,
    &gSpriteBank00Frame399,
    &gSpriteBank00Frame400,
    &gSpriteBank00Frame401,
    &gSpriteBank00Frame402,
    &gSpriteBank00Frame403,
    &gSpriteBank00Frame404.frame,
    &gSpriteBank00Frame405.frame,
    &gSpriteBank00Frame406.frame,
    &gSpriteBank00Frame407.frame,
    &gSpriteBank00Frame408.frame,
    &gSpriteBank00Frame409.frame,
    &gSpriteBank00Frame410.frame,
    &gSpriteBank00Frame411.frame,
    &gSpriteBank00Frame412.frame,
    &gSpriteBank00Frame413.frame,
    &gSpriteBank00Frame414.frame,
    &gSpriteBank00Frame415.frame,
    &gSpriteBank00Frame416.frame,
    &gSpriteBank00Frame417.frame,
    &gSpriteBank00Frame418.frame,
    &gSpriteBank00Frame419.frame,
    &gSpriteBank00Frame420.frame,
    &gSpriteBank00Frame421.frame,
    &gSpriteBank00Frame422.frame,
    &gSpriteBank00Frame423.frame,
    &gSpriteBank00Frame424.frame,
    &gSpriteBank00Frame425.frame,
    &gSpriteBank00Frame426.frame,
    &gSpriteBank00Frame427.frame,
    &gSpriteBank00Frame428.frame,
    &gSpriteBank00Frame429.frame,
    &gSpriteBank00Frame430.frame,
    &gSpriteBank00Frame431.frame,
    &gSpriteBank00Frame432.frame,
    &gSpriteBank00Frame433.frame,
    &gSpriteBank00Frame434.frame,
    &gSpriteBank00Frame435.frame,
    &gSpriteBank00Frame436.frame,
    &gSpriteBank00Frame437.frame,
    &gSpriteBank00Frame438.frame,
    &gSpriteBank00Frame439.frame,
    &gSpriteBank00Frame440,
    &gSpriteBank00Frame441.frame,
    &gSpriteBank00Frame442.frame,
    &gSpriteBank00Frame443.frame,
    &gSpriteBank00Frame444.frame,
    &gSpriteBank00Frame445.frame,
    &gSpriteBank00Frame446.frame,
    &gSpriteBank00Frame447.frame,
    &gSpriteBank00Frame448.frame,
    &gSpriteBank00Frame449.frame,
    &gSpriteBank00Frame450.frame,
    &gSpriteBank00Frame451.frame,
    &gSpriteBank00Frame452.frame,
    &gSpriteBank00Frame453.frame,
    &gSpriteBank00Frame454.frame,
    &gSpriteBank00Frame455.frame,
    &gSpriteBank00Frame456.frame,
    &gSpriteBank00Frame457.frame,
    &gSpriteBank00Frame458.frame,
    &gSpriteBank00Frame459.frame,
    &gSpriteBank00Frame460.frame,
    &gSpriteBank00Frame461.frame,
    &gSpriteBank00Frame462.frame,
    &gSpriteBank00Frame463.frame,
    &gSpriteBank00Frame464.frame,
};

const struct sprite_frame_1box_anchor gSpriteBank00Frame000 = {
    SPRITE_FRAME(gSpriteBank00Frame000, SPRITE_TILES_BANK00_FRAME000),
    { { -12, -3, 28, 11 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame001 = {
    SPRITE_FRAME(gSpriteBank00Frame001, SPRITE_TILES_BANK00_FRAME001),
    { { -12, -3, 28, 11 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame002 = {
    SPRITE_FRAME(gSpriteBank00Frame002, SPRITE_TILES_BANK00_FRAME002),
    { { -12, -3, 28, 11 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame003 = {
    SPRITE_FRAME(gSpriteBank00Frame003, SPRITE_TILES_BANK00_FRAME003),
    { { -12, -3, 28, 11 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame004 = {
    SPRITE_FRAME(gSpriteBank00Frame004, SPRITE_TILES_BANK00_FRAME004),
    { { -12, -3, 28, 11 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame005 = {
    SPRITE_FRAME(gSpriteBank00Frame005, SPRITE_TILES_BANK00_FRAME005),
    { { -12, -3, 28, 11 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame006 = {
    SPRITE_FRAME(gSpriteBank00Frame006, SPRITE_TILES_BANK00_FRAME006),
    { { -12, -3, 28, 11 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame007 = {
    SPRITE_FRAME(gSpriteBank00Frame007, SPRITE_TILES_BANK00_FRAME007),
    { { -12, -3, 28, 11 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame008 = {
    SPRITE_FRAME(gSpriteBank00Frame008, SPRITE_TILES_BANK00_FRAME008),
    { { -12, -3, 28, 11 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame009 = {
    SPRITE_FRAME(gSpriteBank00Frame009, SPRITE_TILES_BANK00_FRAME009),
    { { -12, -3, 28, 11 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame010 = {
    SPRITE_FRAME(gSpriteBank00Frame010, SPRITE_TILES_BANK00_FRAME010),
    { { -12, -3, 28, 11 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame011 = {
    SPRITE_FRAME(gSpriteBank00Frame011, SPRITE_TILES_BANK00_FRAME011),
    { { -12, -3, 28, 11 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame012 = {
    SPRITE_FRAME(gSpriteBank00Frame012, SPRITE_TILES_BANK00_FRAME012),
    { { -11, -2, 23, 12 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame013 = {
    SPRITE_FRAME(gSpriteBank00Frame013, SPRITE_TILES_BANK00_FRAME013),
    { { -7, -5, 21, 15 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame014 = {
    SPRITE_FRAME(gSpriteBank00Frame014, SPRITE_TILES_BANK00_FRAME014),
    { { -6, -7, 15, 19 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame015 = {
    SPRITE_FRAME(gSpriteBank00Frame015, SPRITE_TILES_BANK00_FRAME015),
    { { -6, -14, 22, 26 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame016 = {
    SPRITE_FRAME(gSpriteBank00Frame016, SPRITE_TILES_BANK00_FRAME016),
    { { -6, -17, 20, 29 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame017 = {
    SPRITE_FRAME(gSpriteBank00Frame017, SPRITE_TILES_BANK00_FRAME017),
    { { -7, -21, 19, 34 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame018 = {
    SPRITE_FRAME(gSpriteBank00Frame018, SPRITE_TILES_BANK00_FRAME018),
    { { -10, -25, 20, 38 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame019 = {
    SPRITE_FRAME(gSpriteBank00Frame019, SPRITE_TILES_BANK00_FRAME019),
    { { -5, -8, 11, 21 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame020 = {
    SPRITE_FRAME(gSpriteBank00Frame020, SPRITE_TILES_BANK00_FRAME020),
    { { -5, -8, 11, 21 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame021 = {
    SPRITE_FRAME(gSpriteBank00Frame021, SPRITE_TILES_BANK00_FRAME021),
    { { -5, -8, 11, 21 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame022 = {
    SPRITE_FRAME(gSpriteBank00Frame022, SPRITE_TILES_BANK00_FRAME022),
    { { -5, -8, 11, 21 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame023 = {
    SPRITE_FRAME(gSpriteBank00Frame023, SPRITE_TILES_BANK00_FRAME023),
    { { -7, -7, 14, 21 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame024 = {
    SPRITE_FRAME(gSpriteBank00Frame024, SPRITE_TILES_BANK00_FRAME024),
    { { -10, -23, 20, 36 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame025 = {
    SPRITE_FRAME(gSpriteBank00Frame025, SPRITE_TILES_BANK00_FRAME025),
    { { -10, -24, 20, 37 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame026 = {
    SPRITE_FRAME(gSpriteBank00Frame026, SPRITE_TILES_BANK00_FRAME026),
    { { -10, -25, 20, 38 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame027 = {
    SPRITE_FRAME(gSpriteBank00Frame027, SPRITE_TILES_BANK00_FRAME027),
    { { -10, -27, 20, 40 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame028 = {
    SPRITE_FRAME(gSpriteBank00Frame028, SPRITE_TILES_BANK00_FRAME028),
    { { -10, -28, 20, 41 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame029 = {
    SPRITE_FRAME(gSpriteBank00Frame029, SPRITE_TILES_BANK00_FRAME029),
    { { -10, -30, 20, 43 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame030 = {
    SPRITE_FRAME(gSpriteBank00Frame030, SPRITE_TILES_BANK00_FRAME030),
    { { -10, -30, 20, 43 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame031 = {
    SPRITE_FRAME(gSpriteBank00Frame031, SPRITE_TILES_BANK00_FRAME031),
    { { -9, -30, 19, 43 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame032 = {
    SPRITE_FRAME(gSpriteBank00Frame032, SPRITE_TILES_BANK00_FRAME032),
    { { -9, -29, 19, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame033 = {
    SPRITE_FRAME(gSpriteBank00Frame033, SPRITE_TILES_BANK00_FRAME033),
    { { -9, -29, 19, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame034 = {
    SPRITE_FRAME(gSpriteBank00Frame034, SPRITE_TILES_BANK00_FRAME034),
    { { -8, -27, 18, 40 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame035 = {
    SPRITE_FRAME(gSpriteBank00Frame035, SPRITE_TILES_BANK00_FRAME035),
    { { -8, -25, 18, 38 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame036 = {
    SPRITE_FRAME(gSpriteBank00Frame036, SPRITE_TILES_BANK00_FRAME036),
    { { -8, -24, 17, 37 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame037 = {
    SPRITE_FRAME(gSpriteBank00Frame037, SPRITE_TILES_BANK00_FRAME037),
    { { -8, -22, 17, 35 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame038 = {
    SPRITE_FRAME(gSpriteBank00Frame038, SPRITE_TILES_BANK00_FRAME038),
    { { -8, -22, 17, 35 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame039 = {
    SPRITE_FRAME(gSpriteBank00Frame039, SPRITE_TILES_BANK00_FRAME039),
    { { -8, -21, 17, 34 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame040 = {
    SPRITE_FRAME(gSpriteBank00Frame040, SPRITE_TILES_BANK00_FRAME040),
    { { -8, -21, 18, 34 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame041 = {
    SPRITE_FRAME(gSpriteBank00Frame041, SPRITE_TILES_BANK00_FRAME041),
    { { -8, -20, 18, 33 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame042 = {
    SPRITE_FRAME(gSpriteBank00Frame042, SPRITE_TILES_BANK00_FRAME042),
    { { -8, -21, 18, 34 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame043 = {
    SPRITE_FRAME(gSpriteBank00Frame043, SPRITE_TILES_BANK00_FRAME043),
    { { -8, -21, 18, 34 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame044 = {
    SPRITE_FRAME(gSpriteBank00Frame044, SPRITE_TILES_BANK00_FRAME044),
    { { -8, -22, 18, 35 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame045 = {
    SPRITE_FRAME(gSpriteBank00Frame045, SPRITE_TILES_BANK00_FRAME045),
    { { -9, -23, 19, 36 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame046 = {
    SPRITE_FRAME(gSpriteBank00Frame046, SPRITE_TILES_BANK00_FRAME046),
    { { -10, -23, 20, 36 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame047 = {
    SPRITE_FRAME(gSpriteBank00Frame047, SPRITE_TILES_BANK00_FRAME047),
    { { -10, -23, 20, 36 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame048 = {
    SPRITE_FRAME(gSpriteBank00Frame048, SPRITE_TILES_BANK00_FRAME048),
    { { -5, -25, 8, 36 } },
    { -3, -32 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame049 = {
    SPRITE_FRAME(gSpriteBank00Frame049, SPRITE_TILES_BANK00_FRAME049),
    { { -4, -20, 8, 28 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame050 = {
    SPRITE_FRAME(gSpriteBank00Frame050, SPRITE_TILES_BANK00_FRAME050),
    { { -4, -17, 11, 20 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame051 = {
    SPRITE_FRAME(gSpriteBank00Frame051, SPRITE_TILES_BANK00_FRAME051),
    { { -5, -11, 15, 13 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame052 = {
    SPRITE_FRAME(gSpriteBank00Frame052, SPRITE_TILES_BANK00_FRAME052),
    { { -7, -14, 14, 13 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame053 = {
    SPRITE_FRAME(gSpriteBank00Frame053, SPRITE_TILES_BANK00_FRAME053),
    { { -9, -15, 22, 14 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame054 = {
    SPRITE_FRAME(gSpriteBank00Frame054, SPRITE_TILES_BANK00_FRAME054),
    { { 0, 0, 0, 0 }, { -7, -12, 28, 18 }, { 0, 0, 0, 0 } },
    { -12, -24 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame055 = {
    SPRITE_FRAME(gSpriteBank00Frame055, SPRITE_TILES_BANK00_FRAME055),
    { { -14, -25, 14, 15 }, { -7, -7, 19, 25 }, { 0, 0, 0, 0 } },
    { -7, -28 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame056 = {
    SPRITE_FRAME(gSpriteBank00Frame056, SPRITE_TILES_BANK00_FRAME056),
    { { -8, -24, 12, 15 }, { -6, -6, 12, 23 }, { 0, 0, 0, 0 } },
    { 6, -25 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame057 = {
    SPRITE_FRAME(gSpriteBank00Frame057, SPRITE_TILES_BANK00_FRAME057),
    { { 0, 0, 0, 0 }, { -20, -14, 34, 11 }, { -15, -5, 30, 6 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame058 = {
    SPRITE_FRAME(gSpriteBank00Frame058, SPRITE_TILES_BANK00_FRAME058),
    { { 0, 0, 0, 0 }, { -15, -7, 21, 5 }, { -14, -5, 18, 8 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame059 = {
    SPRITE_FRAME(gSpriteBank00Frame059, SPRITE_TILES_BANK00_FRAME059),
    { { 0, 0, 0, 0 }, { -20, -12, 29, 12 }, { -18, -4, 26, 7 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame060 = {
    SPRITE_FRAME(gSpriteBank00Frame060, SPRITE_TILES_BANK00_FRAME060),
    { { 0, 0, 0, 0 }, { -20, -11, 31, 9 }, { -17, -4, 26, 5 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame061 = {
    SPRITE_FRAME(gSpriteBank00Frame061, SPRITE_TILES_BANK00_FRAME061),
    { { 0, 0, 0, 0 }, { -11, -12, 21, 8 }, { -9, -6, 18, 6 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame062 = {
    SPRITE_FRAME(gSpriteBank00Frame062, SPRITE_TILES_BANK00_FRAME062),
    { { 0, 0, 0, 0 }, { -10, -10, 26, 8 }, { -10, -7, 25, 7 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame063 = {
    SPRITE_FRAME(gSpriteBank00Frame063, SPRITE_TILES_BANK00_FRAME063),
    { { 0, 0, 0, 0 }, { -19, -10, 42, 10 }, { -9, -4, 25, 6 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame064 = {
    SPRITE_FRAME(gSpriteBank00Frame064, SPRITE_TILES_BANK00_FRAME064),
    { { -14, -16, 18, 21 }, { -13, -12, 17, 17 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame065 = {
    SPRITE_FRAME(gSpriteBank00Frame065, SPRITE_TILES_BANK00_FRAME065),
    { { -14, -16, 29, 22 }, { -17, -8, 32, 8 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame066 = {
    SPRITE_FRAME(gSpriteBank00Frame066, SPRITE_TILES_BANK00_FRAME066),
    { { 0, 0, 0, 0 }, { -18, -10, 40, 10 }, { -10, -10, 26, 6 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame067 = {
    SPRITE_FRAME(gSpriteBank00Frame067, SPRITE_TILES_BANK00_FRAME067),
    { { -12, -18, 26, 9 }, { -8, -9, 16, 17 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame068 = {
    SPRITE_FRAME(gSpriteBank00Frame068, SPRITE_TILES_BANK00_FRAME068),
    { { -20, -18, 31, 18 }, { -17, -13, 30, 11 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame069 = {
    SPRITE_FRAME(gSpriteBank00Frame069, SPRITE_TILES_BANK00_FRAME069),
    { { 0, 0, 0, 0 }, { -19, -12, 43, 11 }, { -9, -9, 25, 5 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame070 = {
    SPRITE_FRAME(gSpriteBank00Frame070, SPRITE_TILES_BANK00_FRAME070),
    { { -8, -16, 17, 15 }, { -9, -8, 19, 29 }, { 0, 0, 0, 0 } },
    { 9, -22 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame071 = {
    SPRITE_FRAME(gSpriteBank00Frame071, SPRITE_TILES_BANK00_FRAME071),
    { { -7, -19, 14, 20 }, { -8, -10, 16, 32 }, { 0, 0, 0, 0 } },
    { 9, -24 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame072 = {
    SPRITE_FRAME(gSpriteBank00Frame072, SPRITE_TILES_BANK00_FRAME072),
    { { -5, -19, 15, 14 }, { -4, -9, 13, 24 }, { 0, 0, 0, 0 } },
    { 9, -22 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame073 = {
    SPRITE_FRAME(gSpriteBank00Frame073, SPRITE_TILES_BANK00_FRAME073),
    { { -4, -21, 15, 8 }, { -4, -10, 15, 16 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame074 = {
    SPRITE_FRAME(gSpriteBank00Frame074, SPRITE_TILES_BANK00_FRAME074),
    { { -4, -24, 14, 8 }, { -4, -14, 14, 18 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame075 = {
    SPRITE_FRAME(gSpriteBank00Frame075, SPRITE_TILES_BANK00_FRAME075),
    { { -4, -26, 14, 11 }, { -4, -15, 14, 19 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame076 = {
    SPRITE_FRAME(gSpriteBank00Frame076, SPRITE_TILES_BANK00_FRAME076),
    { { -4, -24, 15, 10 }, { -4, -13, 15, 18 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame077 = {
    SPRITE_FRAME(gSpriteBank00Frame077, SPRITE_TILES_BANK00_FRAME077),
    { { -5, -21, 16, 16 }, { -5, -10, 16, 24 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame078 = {
    SPRITE_FRAME(gSpriteBank00Frame078, SPRITE_TILES_BANK00_FRAME078),
    { { -6, -21, 14, 20 }, { -6, -12, 14, 32 }, { 0, 0, 0, 0 } },
    { 7, -23 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame079 = {
    SPRITE_FRAME(gSpriteBank00Frame079, SPRITE_TILES_BANK00_FRAME079),
    { { -8, -23, 14, 21 }, { -8, -14, 14, 33 }, { 0, 0, 0, 0 } },
    { 3, -26 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame080 = {
    SPRITE_FRAME(gSpriteBank00Frame080, SPRITE_TILES_BANK00_FRAME080),
    { { -6, -15, 22, 10 }, { -5, -1, 15, 14 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame081 = {
    SPRITE_FRAME(gSpriteBank00Frame081, SPRITE_TILES_BANK00_FRAME081),
    { { -4, -19, 13, 23 }, { -5, 2, 10, 16 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame082 = {
    SPRITE_FRAME(gSpriteBank00Frame082, SPRITE_TILES_BANK00_FRAME082),
    { { -7, -20, 13, 27 }, { -9, 6, 12, 13 }, { 0, 0, 0, 0 } },
    { 9, -22 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame083 = {
    SPRITE_FRAME(gSpriteBank00Frame083, SPRITE_TILES_BANK00_FRAME083),
    { { -7, -22, 14, 24 }, { -8, 2, 10, 17 }, { 0, 0, 0, 0 } },
    { 7, -24 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame084 = {
    SPRITE_FRAME(gSpriteBank00Frame084, SPRITE_TILES_BANK00_FRAME084),
    { { -3, -21, 11, 16 }, { -5, -2, 12, 15 }, { 0, 0, 0, 0 } },
    { 10, -22 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame085 = {
    SPRITE_FRAME(gSpriteBank00Frame085, SPRITE_TILES_BANK00_FRAME085),
    { { -4, -23, 15, 10 }, { -5, -9, 16, 14 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame086 = {
    SPRITE_FRAME(gSpriteBank00Frame086, SPRITE_TILES_BANK00_FRAME086),
    { { -4, -18, 15, 7 }, { -6, -7, 17, 11 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame087 = {
    SPRITE_FRAME(gSpriteBank00Frame087, SPRITE_TILES_BANK00_FRAME087),
    { { -4, -17, 14, 13 }, { -5, -6, 16, 9 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame088 = {
    SPRITE_FRAME(gSpriteBank00Frame088, SPRITE_TILES_BANK00_FRAME088),
    { { -4, -20, 14, 11 }, { -5, -8, 17, 12 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame089 = {
    SPRITE_FRAME(gSpriteBank00Frame089, SPRITE_TILES_BANK00_FRAME089),
    { { -5, -22, 16, 16 }, { -5, -3, 14, 18 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame090 = {
    SPRITE_FRAME(gSpriteBank00Frame090, SPRITE_TILES_BANK00_FRAME090),
    { { -7, -24, 16, 24 }, { -6, 2, 14, 18 }, { 0, 0, 0, 0 } },
    { 7, -23 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame091 = {
    SPRITE_FRAME(gSpriteBank00Frame091, SPRITE_TILES_BANK00_FRAME091),
    { { -9, -26, 16, 25 }, { -5, -2, 13, 21 }, { 0, 0, 0, 0 } },
    { 3, -26 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame092 = {
    SPRITE_FRAME(gSpriteBank00Frame092, SPRITE_TILES_BANK00_FRAME092),
    { { -16, -29, 13, 25 }, { -5, 6, 8, 11 }, { 0, 0, 0, 0 } },
    { -8, -33 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame093 = {
    SPRITE_FRAME(gSpriteBank00Frame093, SPRITE_TILES_BANK00_FRAME093),
    { { -16, -28, 13, 22 }, { -5, 5, 6, 10 }, { 0, 0, 0, 0 } },
    { -7, -33 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame094 = {
    SPRITE_FRAME(gSpriteBank00Frame094, SPRITE_TILES_BANK00_FRAME094),
    { { -15, -27, 14, 20 }, { -5, 2, 8, 12 }, { 0, 0, 0, 0 } },
    { -5, -32 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame095 = {
    SPRITE_FRAME(gSpriteBank00Frame095, SPRITE_TILES_BANK00_FRAME095),
    { { -14, -26, 15, 19 }, { -6, 6, 8, 9 }, { 0, 0, 0, 0 } },
    { -3, -30 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame096 = {
    SPRITE_FRAME(gSpriteBank00Frame096, SPRITE_TILES_BANK00_FRAME096),
    { { -13, -22, 14, 18 }, { -5, 6, 8, 10 }, { 0, 0, 0, 0 } },
    { -3, -26 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame097 = {
    SPRITE_FRAME(gSpriteBank00Frame097, SPRITE_TILES_BANK00_FRAME097),
    { { -13, -19, 14, 17 }, { -5, 5, 9, 10 }, { 0, 0, 0, 0 } },
    { -3, -23 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame098 = {
    SPRITE_FRAME(gSpriteBank00Frame098, SPRITE_TILES_BANK00_FRAME098),
    { { -13, -17, 14, 18 }, { -5, 5, 10, 11 }, { 0, 0, 0, 0 } },
    { -3, -22 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame099 = {
    SPRITE_FRAME(gSpriteBank00Frame099, SPRITE_TILES_BANK00_FRAME099),
    { { -12, -18, 14, 21 }, { -4, 5, 10, 10 }, { 0, 0, 0, 0 } },
    { -2, -23 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame100 = {
    SPRITE_FRAME(gSpriteBank00Frame100, SPRITE_TILES_BANK00_FRAME100),
    { { -7, -16, 13, 19 }, { -4, 5, 9, 10 }, { 0, 0, 0, 0 } },
    { 4, -20 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame101 = {
    SPRITE_FRAME(gSpriteBank00Frame101, SPRITE_TILES_BANK00_FRAME101),
    { { -7, -20, 13, 20 }, { -3, 6, 9, 10 }, { 0, 0, 0, 0 } },
    { 4, -25 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame102 = {
    SPRITE_FRAME(gSpriteBank00Frame102, SPRITE_TILES_BANK00_FRAME102),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame103 = {
    SPRITE_FRAME(gSpriteBank00Frame103, SPRITE_TILES_BANK00_FRAME103),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame104 = {
    SPRITE_FRAME(gSpriteBank00Frame104, SPRITE_TILES_BANK00_FRAME104),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame105 = {
    SPRITE_FRAME(gSpriteBank00Frame105, SPRITE_TILES_BANK00_FRAME105),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame106 = {
    SPRITE_FRAME(gSpriteBank00Frame106, SPRITE_TILES_BANK00_FRAME106),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame107 = {
    SPRITE_FRAME(gSpriteBank00Frame107, SPRITE_TILES_BANK00_FRAME107),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame108 = {
    SPRITE_FRAME(gSpriteBank00Frame108, SPRITE_TILES_BANK00_FRAME108),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame109 = {
    SPRITE_FRAME(gSpriteBank00Frame109, SPRITE_TILES_BANK00_FRAME109),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame110 = {
    SPRITE_FRAME(gSpriteBank00Frame110, SPRITE_TILES_BANK00_FRAME110),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame111 = {
    SPRITE_FRAME(gSpriteBank00Frame111, SPRITE_TILES_BANK00_FRAME111),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame112 = {
    SPRITE_FRAME(gSpriteBank00Frame112, SPRITE_TILES_BANK00_FRAME112),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame113 = {
    SPRITE_FRAME(gSpriteBank00Frame113, SPRITE_TILES_BANK00_FRAME113),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame114 = {
    SPRITE_FRAME(gSpriteBank00Frame114, SPRITE_TILES_BANK00_FRAME114),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame115 = {
    SPRITE_FRAME(gSpriteBank00Frame115, SPRITE_TILES_BANK00_FRAME115),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame116 = {
    SPRITE_FRAME(gSpriteBank00Frame116, SPRITE_TILES_BANK00_FRAME116),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame117 = {
    SPRITE_FRAME(gSpriteBank00Frame117, SPRITE_TILES_BANK00_FRAME117),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame118 = {
    SPRITE_FRAME(gSpriteBank00Frame118, SPRITE_TILES_BANK00_FRAME118),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame119 = {
    SPRITE_FRAME(gSpriteBank00Frame119, SPRITE_TILES_BANK00_FRAME119),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame120 = {
    SPRITE_FRAME(gSpriteBank00Frame120, SPRITE_TILES_BANK00_FRAME120),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame121 = {
    SPRITE_FRAME(gSpriteBank00Frame121, SPRITE_TILES_BANK00_FRAME121),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame122 = {
    SPRITE_FRAME(gSpriteBank00Frame122, SPRITE_TILES_BANK00_FRAME122),
    { { -10, -22, 20, 35 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame123 = {
    SPRITE_FRAME(gSpriteBank00Frame123, SPRITE_TILES_BANK00_FRAME123),
    { { -9, -23, 19, 36 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame124 = {
    SPRITE_FRAME(gSpriteBank00Frame124, SPRITE_TILES_BANK00_FRAME124),
    { { -8, -24, 18, 37 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame125 = {
    SPRITE_FRAME(gSpriteBank00Frame125, SPRITE_TILES_BANK00_FRAME125),
    { { -9, -25, 19, 38 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame126 = {
    SPRITE_FRAME(gSpriteBank00Frame126, SPRITE_TILES_BANK00_FRAME126),
    { { -9, -25, 19, 38 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame127 = {
    SPRITE_FRAME(gSpriteBank00Frame127, SPRITE_TILES_BANK00_FRAME127),
    { { -10, -27, 18, 40 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame128 = {
    SPRITE_FRAME(gSpriteBank00Frame128, SPRITE_TILES_BANK00_FRAME128),
    { { -11, -27, 18, 40 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame129 = {
    SPRITE_FRAME(gSpriteBank00Frame129, SPRITE_TILES_BANK00_FRAME129),
    { { -12, -27, 18, 40 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame130 = {
    SPRITE_FRAME(gSpriteBank00Frame130, SPRITE_TILES_BANK00_FRAME130),
    { { -13, -25, 19, 38 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame131 = {
    SPRITE_FRAME(gSpriteBank00Frame131, SPRITE_TILES_BANK00_FRAME131),
    { { -13, -27, 19, 40 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame132 = {
    SPRITE_FRAME(gSpriteBank00Frame132, SPRITE_TILES_BANK00_FRAME132),
    { { -13, -28, 19, 41 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame133 = {
    SPRITE_FRAME(gSpriteBank00Frame133, SPRITE_TILES_BANK00_FRAME133),
    { { -13, -26, 19, 39 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame134 = {
    SPRITE_FRAME(gSpriteBank00Frame134, SPRITE_TILES_BANK00_FRAME134),
    { { -14, -27, 20, 40 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame135 = {
    SPRITE_FRAME(gSpriteBank00Frame135, SPRITE_TILES_BANK00_FRAME135),
    { { -13, -26, 19, 39 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame136 = {
    SPRITE_FRAME(gSpriteBank00Frame136, SPRITE_TILES_BANK00_FRAME136),
    { { -18, -8, 17, 17 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame137 = {
    SPRITE_FRAME(gSpriteBank00Frame137, SPRITE_TILES_BANK00_FRAME137),
    { { -18, -8, 17, 17 }, { -4, 2, 20, 10 }, { 5, 3, 12, 9 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame138 = {
    SPRITE_FRAME(gSpriteBank00Frame138, SPRITE_TILES_BANK00_FRAME138),
    { { -18, -8, 17, 17 }, { -5, 1, 22, 11 }, { 6, 4, 12, 7 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame139 = {
    SPRITE_FRAME(gSpriteBank00Frame139, SPRITE_TILES_BANK00_FRAME139),
    { { -18, -8, 17, 17 }, { -4, 2, 19, 11 }, { 7, 3, 9, 7 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame140 = {
    SPRITE_FRAME(gSpriteBank00Frame140, SPRITE_TILES_BANK00_FRAME140),
    { { -18, -8, 17, 17 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame141 = {
    SPRITE_FRAME(gSpriteBank00Frame141, SPRITE_TILES_BANK00_FRAME141),
    { { 0, 0, 0, 0 }, { -18, -22, 32, 32 }, { -18, -8, 32, 16 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame142 = {
    SPRITE_FRAME(gSpriteBank00Frame142, SPRITE_TILES_BANK00_FRAME142),
    { { 0, 0, 0, 0 }, { -20, -20, 35, 29 }, { -18, -8, 32, 16 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame143 = {
    SPRITE_FRAME(gSpriteBank00Frame143, SPRITE_TILES_BANK00_FRAME143),
    { { 0, 0, 0, 0 }, { -20, -16, 32, 28 }, { -18, -8, 32, 16 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame144 = {
    SPRITE_FRAME(gSpriteBank00Frame144, SPRITE_TILES_BANK00_FRAME144),
    { { 0, 0, 0, 0 }, { -22, -14, 36, 27 }, { -18, -8, 32, 16 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame145 = {
    SPRITE_FRAME(gSpriteBank00Frame145, SPRITE_TILES_BANK00_FRAME145),
    { { 0, 0, 0, 0 }, { -24, -14, 39, 26 }, { -18, -8, 32, 16 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame146 = {
    SPRITE_FRAME(gSpriteBank00Frame146, SPRITE_TILES_BANK00_FRAME146),
    { { 0, 0, 0, 0 }, { -22, -14, 40, 27 }, { -18, -8, 32, 16 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame147 = {
    SPRITE_FRAME(gSpriteBank00Frame147, SPRITE_TILES_BANK00_FRAME147),
    { { 0, 0, 0, 0 }, { -20, -15, 36, 27 }, { -18, -8, 32, 16 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame148 = {
    SPRITE_FRAME(gSpriteBank00Frame148, SPRITE_TILES_BANK00_FRAME148),
    { { 0, 0, 0, 0 }, { -20, -15, 40, 27 }, { -18, -8, 32, 16 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame149 = {
    SPRITE_FRAME(gSpriteBank00Frame149, SPRITE_TILES_BANK00_FRAME149),
    { { 0, 0, 0, 0 }, { -21, -18, 35, 27 }, { -18, -8, 32, 16 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame150 = {
    SPRITE_FRAME(gSpriteBank00Frame150, SPRITE_TILES_BANK00_FRAME150),
    { { 0, 0, 0, 0 }, { -19, -21, 29, 32 }, { -18, -8, 32, 16 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame151 = {
    SPRITE_FRAME(gSpriteBank00Frame151, SPRITE_TILES_BANK00_FRAME151),
    { { -33, -2, 63, 25 }, { -22, 3, 38, 9 }, { -19, 6, 39, 10 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame152 = {
    SPRITE_FRAME(gSpriteBank00Frame152, SPRITE_TILES_BANK00_FRAME152),
    { { -31, -2, 58, 25 }, { -22, 3, 39, 10 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame153 = {
    SPRITE_FRAME(gSpriteBank00Frame153, SPRITE_TILES_BANK00_FRAME153),
    { { -29, -1, 52, 24 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame154 = {
    SPRITE_FRAME(gSpriteBank00Frame154, SPRITE_TILES_BANK00_FRAME154),
    { { -26, -1, 46, 22 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame155 = {
    SPRITE_FRAME(gSpriteBank00Frame155, SPRITE_TILES_BANK00_FRAME155),
    { { -26, -1, 43, 20 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame156 = {
    SPRITE_FRAME(gSpriteBank00Frame156, SPRITE_TILES_BANK00_FRAME156),
    { { -25, -5, 40, 23 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame157 = {
    SPRITE_FRAME(gSpriteBank00Frame157, SPRITE_TILES_BANK00_FRAME157),
    { { -25, -7, 40, 25 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame158 = {
    SPRITE_FRAME(gSpriteBank00Frame158, SPRITE_TILES_BANK00_FRAME158),
    { { -25, -8, 38, 26 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame159 = {
    SPRITE_FRAME(gSpriteBank00Frame159, SPRITE_TILES_BANK00_FRAME159),
    { { -24, -8, 36, 27 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame160 = {
    SPRITE_FRAME(gSpriteBank00Frame160, SPRITE_TILES_BANK00_FRAME160),
    { { -23, -7, 37, 26 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame161 = {
    SPRITE_FRAME(gSpriteBank00Frame161, SPRITE_TILES_BANK00_FRAME161),
    { { -21, -7, 37, 26 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame162 = {
    SPRITE_FRAME(gSpriteBank00Frame162, SPRITE_TILES_BANK00_FRAME162),
    { { -17, -13, 31, 26 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame163 = {
    SPRITE_FRAME(gSpriteBank00Frame163, SPRITE_TILES_BANK00_FRAME163),
    { { -15, -17, 27, 33 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame164 = {
    SPRITE_FRAME(gSpriteBank00Frame164, SPRITE_TILES_BANK00_FRAME164),
    { { -14, -20, 24, 35 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame165 = {
    SPRITE_FRAME(gSpriteBank00Frame165, SPRITE_TILES_BANK00_FRAME165),
    { { -13, -21, 21, 35 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame166 = {
    SPRITE_FRAME(gSpriteBank00Frame166, SPRITE_TILES_BANK00_FRAME166),
    { { -12, -23, 21, 37 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame167 = {
    SPRITE_FRAME(gSpriteBank00Frame167, SPRITE_TILES_BANK00_FRAME167),
    { { -11, -24, 20, 37 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame168 = {
    SPRITE_FRAME(gSpriteBank00Frame168, SPRITE_TILES_BANK00_FRAME168),
    { { -9, -25, 19, 38 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame169 = {
    SPRITE_FRAME(gSpriteBank00Frame169, SPRITE_TILES_BANK00_FRAME169),
    { { -10, -25, 20, 38 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame170 = {
    SPRITE_FRAME(gSpriteBank00Frame170, SPRITE_TILES_BANK00_FRAME170),
    { { -10, -25, 20, 38 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame171 = {
    SPRITE_FRAME(gSpriteBank00Frame171, SPRITE_TILES_BANK00_FRAME171),
    { { -7, -27, 14, 41 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame172 = {
    SPRITE_FRAME(gSpriteBank00Frame172, SPRITE_TILES_BANK00_FRAME172),
    { { -7, -27, 14, 41 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame173 = {
    SPRITE_FRAME(gSpriteBank00Frame173, SPRITE_TILES_BANK00_FRAME173),
    { { -7, -27, 14, 41 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame174 = {
    SPRITE_FRAME(gSpriteBank00Frame174, SPRITE_TILES_BANK00_FRAME174),
    { { -7, -27, 14, 41 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame175 = {
    SPRITE_FRAME(gSpriteBank00Frame175, SPRITE_TILES_BANK00_FRAME175),
    { { -7, -27, 14, 41 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame176 = {
    SPRITE_FRAME(gSpriteBank00Frame176, SPRITE_TILES_BANK00_FRAME176),
    { { -7, -27, 14, 41 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame177 = {
    SPRITE_FRAME(gSpriteBank00Frame177, SPRITE_TILES_BANK00_FRAME177),
    { { -7, -27, 14, 41 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame178 = {
    SPRITE_FRAME(gSpriteBank00Frame178, SPRITE_TILES_BANK00_FRAME178),
    { { -7, -27, 14, 41 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame179 = {
    SPRITE_FRAME(gSpriteBank00Frame179, SPRITE_TILES_BANK00_FRAME179),
    { { -7, -27, 14, 41 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame180 = {
    SPRITE_FRAME(gSpriteBank00Frame180, SPRITE_TILES_BANK00_FRAME180),
    { { -7, -27, 14, 41 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame181 = {
    SPRITE_FRAME(gSpriteBank00Frame181, SPRITE_TILES_BANK00_FRAME181),
    { { -9, -26, 14, 38 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame182 = {
    SPRITE_FRAME(gSpriteBank00Frame182, SPRITE_TILES_BANK00_FRAME182),
    { { -14, -26, 21, 41 } },
    { 5, -29 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame183 = {
    SPRITE_FRAME(gSpriteBank00Frame183, SPRITE_TILES_BANK00_FRAME183),
    { { -15, -30, 13, 23 }, { -7, -8, 7, 25 }, { 0, 0, 0, 0 } },
    { -10, -35 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame184 = {
    SPRITE_FRAME(gSpriteBank00Frame184, SPRITE_TILES_BANK00_FRAME184),
    { { -7, -6, 15, 20 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame185 = {
    SPRITE_FRAME(gSpriteBank00Frame185, SPRITE_TILES_BANK00_FRAME185),
    { { -7, -6, 15, 20 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame186 = {
    SPRITE_FRAME(gSpriteBank00Frame186, SPRITE_TILES_BANK00_FRAME186),
    { { -7, -6, 15, 20 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame187 = {
    SPRITE_FRAME(gSpriteBank00Frame187, SPRITE_TILES_BANK00_FRAME187),
    { { -8, -24, 11, 25 }, { -7, 2, 9, 12 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame188 = {
    SPRITE_FRAME(gSpriteBank00Frame188, SPRITE_TILES_BANK00_FRAME188),
    { { -11, -24, 14, 21 }, { -7, -6, 9, 24 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame189 = {
    SPRITE_FRAME(gSpriteBank00Frame189, SPRITE_TILES_BANK00_FRAME189),
    { { -18, -29, 17, 25 }, { -7, -7, 7, 25 }, { 0, 0, 0, 0 } },
    { -8, -32 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame190 = {
    SPRITE_FRAME(gSpriteBank00Frame190, SPRITE_TILES_BANK00_FRAME190),
    { { -3, -28, 9, 44 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame191 = {
    SPRITE_FRAME(gSpriteBank00Frame191, SPRITE_TILES_BANK00_FRAME191),
    { { -4, -24, 8, 37 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame192 = {
    SPRITE_FRAME(gSpriteBank00Frame192, SPRITE_TILES_BANK00_FRAME192),
    { { -9, -18, 14, 31 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame193 = {
    SPRITE_FRAME(gSpriteBank00Frame193, SPRITE_TILES_BANK00_FRAME193),
    { { -11, -15, 18, 28 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame194 = {
    SPRITE_FRAME(gSpriteBank00Frame194, SPRITE_TILES_BANK00_FRAME194),
    { { -9, -14, 17, 27 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame195 = {
    SPRITE_FRAME(gSpriteBank00Frame195, SPRITE_TILES_BANK00_FRAME195),
    { { -11, -12, 19, 25 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame196 = {
    SPRITE_FRAME(gSpriteBank00Frame196, SPRITE_TILES_BANK00_FRAME196),
    { { -10, -9, 18, 22 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame197 = {
    SPRITE_FRAME(gSpriteBank00Frame197, SPRITE_TILES_BANK00_FRAME197),
    { { -3, -11, 11, 24 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame198 = {
    SPRITE_FRAME(gSpriteBank00Frame198, SPRITE_TILES_BANK00_FRAME198),
    { { 0, -14, 8, 27 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame199 = {
    SPRITE_FRAME(gSpriteBank00Frame199, SPRITE_TILES_BANK00_FRAME199),
    { { -1, -18, 7, 31 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame200 = {
    SPRITE_FRAME(gSpriteBank00Frame200, SPRITE_TILES_BANK00_FRAME200),
    { { -2, -22, 7, 35 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame201 = {
    SPRITE_FRAME(gSpriteBank00Frame201, SPRITE_TILES_BANK00_FRAME201),
    { { -4, -24, 8, 37 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame202 = {
    SPRITE_FRAME(gSpriteBank00Frame202, SPRITE_TILES_BANK00_FRAME202),
    { { -4, -25, 8, 38 } },
    { -1, 13 },
};
const struct sprite_frame_3box gSpriteBank00Frame203 = {
    SPRITE_FRAME(gSpriteBank00Frame203, SPRITE_TILES_BANK00_FRAME203),
    { { -11, -22, 16, 9 }, { -15, -19, 27, 27 }, { -18, -8, 32, 15 } },
};
const struct sprite_frame_3box gSpriteBank00Frame204 = {
    SPRITE_FRAME(gSpriteBank00Frame204, SPRITE_TILES_BANK00_FRAME204),
    { { -11, -22, 16, 9 }, { -16, -17, 28, 25 }, { -18, -8, 32, 15 } },
};
const struct sprite_frame_3box gSpriteBank00Frame205 = {
    SPRITE_FRAME(gSpriteBank00Frame205, SPRITE_TILES_BANK00_FRAME205),
    { { -11, -17, 17, 9 }, { -21, -14, 33, 23 }, { -18, -8, 32, 15 } },
};
const struct sprite_frame_3box gSpriteBank00Frame206 = {
    SPRITE_FRAME(gSpriteBank00Frame206, SPRITE_TILES_BANK00_FRAME206),
    { { -12, -15, 17, 7 }, { -19, -12, 31, 22 }, { -18, -8, 32, 15 } },
};
const struct sprite_frame_3box gSpriteBank00Frame207 = {
    SPRITE_FRAME(gSpriteBank00Frame207, SPRITE_TILES_BANK00_FRAME207),
    { { -11, -17, 15, 6 }, { -21, -12, 34, 22 }, { -18, -8, 32, 15 } },
};
const struct sprite_frame_3box gSpriteBank00Frame208 = {
    SPRITE_FRAME(gSpriteBank00Frame208, SPRITE_TILES_BANK00_FRAME208),
    { { -10, -15, 16, 5 }, { -19, -12, 35, 22 }, { -18, -8, 32, 15 } },
};
const struct sprite_frame_3box gSpriteBank00Frame209 = {
    SPRITE_FRAME(gSpriteBank00Frame209, SPRITE_TILES_BANK00_FRAME209),
    { { -13, -15, 20, 8 }, { -18, -12, 32, 22 }, { -18, -8, 32, 15 } },
};
const struct sprite_frame_3box gSpriteBank00Frame210 = {
    SPRITE_FRAME(gSpriteBank00Frame210, SPRITE_TILES_BANK00_FRAME210),
    { { -12, -16, 20, 8 }, { -18, -13, 34, 22 }, { -18, -8, 32, 15 } },
};
const struct sprite_frame_3box gSpriteBank00Frame211 = {
    SPRITE_FRAME(gSpriteBank00Frame211, SPRITE_TILES_BANK00_FRAME211),
    { { -11, -22, 16, 9 }, { -19, -17, 29, 25 }, { -18, -8, 32, 15 } },
};
const struct sprite_frame_3box gSpriteBank00Frame212 = {
    SPRITE_FRAME(gSpriteBank00Frame212, SPRITE_TILES_BANK00_FRAME212),
    { { -11, -22, 16, 9 }, { -18, -20, 26, 28 }, { -18, -8, 32, 15 } },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame213 = {
    SPRITE_FRAME(gSpriteBank00Frame213, SPRITE_TILES_BANK00_FRAME213),
    { { -7, -28, 14, 36 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame214 = {
    SPRITE_FRAME(gSpriteBank00Frame214, SPRITE_TILES_BANK00_FRAME214),
    { { -7, -28, 14, 36 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame215 = {
    SPRITE_FRAME(gSpriteBank00Frame215, SPRITE_TILES_BANK00_FRAME215),
    { { -7, -28, 14, 36 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame216 = {
    SPRITE_FRAME(gSpriteBank00Frame216, SPRITE_TILES_BANK00_FRAME216),
    { { -7, -28, 14, 36 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame217 = {
    SPRITE_FRAME(gSpriteBank00Frame217, SPRITE_TILES_BANK00_FRAME217),
    { { -7, -27, 14, 38 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame218 = {
    SPRITE_FRAME(gSpriteBank00Frame218, SPRITE_TILES_BANK00_FRAME218),
    { { -12, -29, 11, 23 }, { -8, -6, 8, 18 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame219 = {
    SPRITE_FRAME(gSpriteBank00Frame219, SPRITE_TILES_BANK00_FRAME219),
    { { -7, -24, 13, 16 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame220 = {
    SPRITE_FRAME(gSpriteBank00Frame220, SPRITE_TILES_BANK00_FRAME220),
    { { -8, -20, 16, 13 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame221 = {
    SPRITE_FRAME(gSpriteBank00Frame221, SPRITE_TILES_BANK00_FRAME221),
    { { 0, 0, 0, 0 }, { -10, -19, 28, 15 }, { -11, -4, 27, 5 } },
    { -1, 13 },
};
const struct sprite_frame_1box gSpriteBank00Frame222 = {
    SPRITE_FRAME(gSpriteBank00Frame222, SPRITE_TILES_BANK00_FRAME222),
    { { -10, -23, 20, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame223 = {
    SPRITE_FRAME(gSpriteBank00Frame223, SPRITE_TILES_BANK00_FRAME223),
    { { -11, -23, 21, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame224 = {
    SPRITE_FRAME(gSpriteBank00Frame224, SPRITE_TILES_BANK00_FRAME224),
    { { -11, -22, 20, 35 } },
};
const struct sprite_frame_1box gSpriteBank00Frame225 = {
    SPRITE_FRAME(gSpriteBank00Frame225, SPRITE_TILES_BANK00_FRAME225),
    { { -12, -22, 20, 35 } },
};
const struct sprite_frame_1box gSpriteBank00Frame226 = {
    SPRITE_FRAME(gSpriteBank00Frame226, SPRITE_TILES_BANK00_FRAME226),
    { { -12, -22, 18, 35 } },
};
const struct sprite_frame_1box gSpriteBank00Frame227 = {
    SPRITE_FRAME(gSpriteBank00Frame227, SPRITE_TILES_BANK00_FRAME227),
    { { -12, -22, 18, 35 } },
};
const struct sprite_frame_1box gSpriteBank00Frame228 = {
    SPRITE_FRAME(gSpriteBank00Frame228, SPRITE_TILES_BANK00_FRAME228),
    { { -12, -22, 18, 35 } },
};
const struct sprite_frame_1box gSpriteBank00Frame229 = {
    SPRITE_FRAME(gSpriteBank00Frame229, SPRITE_TILES_BANK00_FRAME229),
    { { -12, -23, 18, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame230 = {
    SPRITE_FRAME(gSpriteBank00Frame230, SPRITE_TILES_BANK00_FRAME230),
    { { -12, -23, 18, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame231 = {
    SPRITE_FRAME(gSpriteBank00Frame231, SPRITE_TILES_BANK00_FRAME231),
    { { -12, -23, 18, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame232 = {
    SPRITE_FRAME(gSpriteBank00Frame232, SPRITE_TILES_BANK00_FRAME232),
    { { -13, -23, 19, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame233 = {
    SPRITE_FRAME(gSpriteBank00Frame233, SPRITE_TILES_BANK00_FRAME233),
    { { -16, -23, 22, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame234 = {
    SPRITE_FRAME(gSpriteBank00Frame234, SPRITE_TILES_BANK00_FRAME234),
    { { -22, -24, 28, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame235 = {
    SPRITE_FRAME(gSpriteBank00Frame235, SPRITE_TILES_BANK00_FRAME235),
    { { -24, -24, 31, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame236 = {
    SPRITE_FRAME(gSpriteBank00Frame236, SPRITE_TILES_BANK00_FRAME236),
    { { -14, -25, 22, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame237 = {
    SPRITE_FRAME(gSpriteBank00Frame237, SPRITE_TILES_BANK00_FRAME237),
    { { -9, -25, 29, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame238 = {
    SPRITE_FRAME(gSpriteBank00Frame238, SPRITE_TILES_BANK00_FRAME238),
    { { -9, -24, 29, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame239 = {
    SPRITE_FRAME(gSpriteBank00Frame239, SPRITE_TILES_BANK00_FRAME239),
    { { -8, -24, 22, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame240 = {
    SPRITE_FRAME(gSpriteBank00Frame240, SPRITE_TILES_BANK00_FRAME240),
    { { -8, -24, 20, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame241 = {
    SPRITE_FRAME(gSpriteBank00Frame241, SPRITE_TILES_BANK00_FRAME241),
    { { -9, -24, 21, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame242 = {
    SPRITE_FRAME(gSpriteBank00Frame242, SPRITE_TILES_BANK00_FRAME242),
    { { -9, -25, 21, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame243 = {
    SPRITE_FRAME(gSpriteBank00Frame243, SPRITE_TILES_BANK00_FRAME243),
    { { -10, -25, 25, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame244 = {
    SPRITE_FRAME(gSpriteBank00Frame244, SPRITE_TILES_BANK00_FRAME244),
    { { -13, -24, 38, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame245 = {
    SPRITE_FRAME(gSpriteBank00Frame245, SPRITE_TILES_BANK00_FRAME245),
    { { -15, -24, 41, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame246 = {
    SPRITE_FRAME(gSpriteBank00Frame246, SPRITE_TILES_BANK00_FRAME246),
    { { -17, -24, 43, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame247 = {
    SPRITE_FRAME(gSpriteBank00Frame247, SPRITE_TILES_BANK00_FRAME247),
    { { -17, -25, 43, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame248 = {
    SPRITE_FRAME(gSpriteBank00Frame248, SPRITE_TILES_BANK00_FRAME248),
    { { -17, -25, 43, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame249 = {
    SPRITE_FRAME(gSpriteBank00Frame249, SPRITE_TILES_BANK00_FRAME249),
    { { -17, -24, 43, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame250 = {
    SPRITE_FRAME(gSpriteBank00Frame250, SPRITE_TILES_BANK00_FRAME250),
    { { -17, -24, 43, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame251 = {
    SPRITE_FRAME(gSpriteBank00Frame251, SPRITE_TILES_BANK00_FRAME251),
    { { -8, -24, 20, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame252 = {
    SPRITE_FRAME(gSpriteBank00Frame252, SPRITE_TILES_BANK00_FRAME252),
    { { -8, -24, 20, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame253 = {
    SPRITE_FRAME(gSpriteBank00Frame253, SPRITE_TILES_BANK00_FRAME253),
    { { -9, -24, 21, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame254 = {
    SPRITE_FRAME(gSpriteBank00Frame254, SPRITE_TILES_BANK00_FRAME254),
    { { -9, -24, 22, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame255 = {
    SPRITE_FRAME(gSpriteBank00Frame255, SPRITE_TILES_BANK00_FRAME255),
    { { -10, -24, 23, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame256 = {
    SPRITE_FRAME(gSpriteBank00Frame256, SPRITE_TILES_BANK00_FRAME256),
    { { -10, -24, 23, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame257 = {
    SPRITE_FRAME(gSpriteBank00Frame257, SPRITE_TILES_BANK00_FRAME257),
    { { -10, -24, 23, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame258 = {
    SPRITE_FRAME(gSpriteBank00Frame258, SPRITE_TILES_BANK00_FRAME258),
    { { -10, -24, 23, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame259 = {
    SPRITE_FRAME(gSpriteBank00Frame259, SPRITE_TILES_BANK00_FRAME259),
    { { -9, -24, 22, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame260 = {
    SPRITE_FRAME(gSpriteBank00Frame260, SPRITE_TILES_BANK00_FRAME260),
    { { -8, -24, 21, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame261 = {
    SPRITE_FRAME(gSpriteBank00Frame261, SPRITE_TILES_BANK00_FRAME261),
    { { -8, -25, 21, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame262 = {
    SPRITE_FRAME(gSpriteBank00Frame262, SPRITE_TILES_BANK00_FRAME262),
    { { -8, -25, 21, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame263 = {
    SPRITE_FRAME(gSpriteBank00Frame263, SPRITE_TILES_BANK00_FRAME263),
    { { -8, -25, 21, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame264 = {
    SPRITE_FRAME(gSpriteBank00Frame264, SPRITE_TILES_BANK00_FRAME264),
    { { -8, -25, 21, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame265 = {
    SPRITE_FRAME(gSpriteBank00Frame265, SPRITE_TILES_BANK00_FRAME265),
    { { -9, -25, 22, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame266 = {
    SPRITE_FRAME(gSpriteBank00Frame266, SPRITE_TILES_BANK00_FRAME266),
    { { -9, -24, 22, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame267 = {
    SPRITE_FRAME(gSpriteBank00Frame267, SPRITE_TILES_BANK00_FRAME267),
    { { -9, -24, 22, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame268 = {
    SPRITE_FRAME(gSpriteBank00Frame268, SPRITE_TILES_BANK00_FRAME268),
    { { -10, -24, 23, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame269 = {
    SPRITE_FRAME(gSpriteBank00Frame269, SPRITE_TILES_BANK00_FRAME269),
    { { -12, -23, 18, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame270 = {
    SPRITE_FRAME(gSpriteBank00Frame270, SPRITE_TILES_BANK00_FRAME270),
    { { -12, -23, 18, 36 } },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame271 = {
    SPRITE_FRAME(gSpriteBank00Frame271, SPRITE_TILES_BANK00_FRAME271),
    { { -9, -30, 13, 18 }, { -8, -8, 14, 11 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame272 = {
    SPRITE_FRAME(gSpriteBank00Frame272, SPRITE_TILES_BANK00_FRAME272),
    { { -13, -27, 14, 27 }, { -11, 1, 15, 13 }, { 0, 0, 0, 0 } },
    { 6, -26 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame273 = {
    SPRITE_FRAME(gSpriteBank00Frame273, SPRITE_TILES_BANK00_FRAME273),
    { { -14, -29, 12, 31 }, { -8, -3, 8, 17 }, { 0, 0, 0, 0 } },
    { -8, -33 },
};
const struct sprite_frame_1box gSpriteBank00Frame274 = {
    SPRITE_FRAME(gSpriteBank00Frame274, SPRITE_TILES_BANK00_FRAME274),
    { { -25, -26, 36, 41 } },
};
const struct sprite_frame_1box gSpriteBank00Frame275 = {
    SPRITE_FRAME(gSpriteBank00Frame275, SPRITE_TILES_BANK00_FRAME275),
    { { -21, -25, 33, 41 } },
};
const struct sprite_frame_1box gSpriteBank00Frame276 = {
    SPRITE_FRAME(gSpriteBank00Frame276, SPRITE_TILES_BANK00_FRAME276),
    { { -24, -26, 36, 41 } },
};
const struct sprite_frame_1box gSpriteBank00Frame277 = {
    SPRITE_FRAME(gSpriteBank00Frame277, SPRITE_TILES_BANK00_FRAME277),
    { { -23, -27, 34, 41 } },
};
const struct sprite_frame_1box gSpriteBank00Frame278 = {
    SPRITE_FRAME(gSpriteBank00Frame278, SPRITE_TILES_BANK00_FRAME278),
    { { -6, -28, 24, 41 } },
};
const struct sprite_frame_1box gSpriteBank00Frame279 = {
    SPRITE_FRAME(gSpriteBank00Frame279, SPRITE_TILES_BANK00_FRAME279),
    { { -6, -28, 23, 40 } },
};
const struct sprite_frame_1box gSpriteBank00Frame280 = {
    SPRITE_FRAME(gSpriteBank00Frame280, SPRITE_TILES_BANK00_FRAME280),
    { { -12, -27, 27, 40 } },
};
const struct sprite_frame_1box gSpriteBank00Frame281 = {
    SPRITE_FRAME(gSpriteBank00Frame281, SPRITE_TILES_BANK00_FRAME281),
    { { -24, -27, 35, 41 } },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame282 = {
    SPRITE_FRAME(gSpriteBank00Frame282, SPRITE_TILES_BANK00_FRAME282),
    { { -8, -25, 11, 37 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame283 = {
    SPRITE_FRAME(gSpriteBank00Frame283, SPRITE_TILES_BANK00_FRAME283),
    { { -7, -25, 12, 32 } },
    { -2, -29 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame284 = {
    SPRITE_FRAME(gSpriteBank00Frame284, SPRITE_TILES_BANK00_FRAME284),
    { { -15, -23, 22, 21 }, { -31, -19, 57, 24 }, { -21, -19, 30, 13 } },
    { -2, -29 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame285 = {
    SPRITE_FRAME(gSpriteBank00Frame285, SPRITE_TILES_BANK00_FRAME285),
    { { -15, -23, 26, 23 }, { -31, -19, 57, 24 }, { -21, -19, 34, 15 } },
    { -2, -29 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame286 = {
    SPRITE_FRAME(gSpriteBank00Frame286, SPRITE_TILES_BANK00_FRAME286),
    { { -15, -23, 29, 24 }, { -31, -19, 57, 24 }, { -21, -19, 37, 16 } },
    { -2, -29 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame287 = {
    SPRITE_FRAME(gSpriteBank00Frame287, SPRITE_TILES_BANK00_FRAME287),
    { { -14, -23, 30, 21 }, { -31, -19, 57, 24 }, { -20, -19, 38, 13 } },
    { -2, -29 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame288 = {
    SPRITE_FRAME(gSpriteBank00Frame288, SPRITE_TILES_BANK00_FRAME288),
    { { -15, -23, 26, 22 }, { -31, -19, 57, 24 }, { -21, -19, 34, 14 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame289 = {
    SPRITE_FRAME(gSpriteBank00Frame289, SPRITE_TILES_BANK00_FRAME289),
    { { -8, -23, 11, 27 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame290 = {
    SPRITE_FRAME(gSpriteBank00Frame290, SPRITE_TILES_BANK00_FRAME290),
    { { -6, -23, 11, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame291 = {
    SPRITE_FRAME(gSpriteBank00Frame291, SPRITE_TILES_BANK00_FRAME291),
    { { -4, -23, 9, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame292 = {
    SPRITE_FRAME(gSpriteBank00Frame292, SPRITE_TILES_BANK00_FRAME292),
    { { -4, -23, 10, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame293 = {
    SPRITE_FRAME(gSpriteBank00Frame293, SPRITE_TILES_BANK00_FRAME293),
    { { -4, -23, 10, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame294 = {
    SPRITE_FRAME(gSpriteBank00Frame294, SPRITE_TILES_BANK00_FRAME294),
    { { -4, -23, 10, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame295 = {
    SPRITE_FRAME(gSpriteBank00Frame295, SPRITE_TILES_BANK00_FRAME295),
    { { -4, -23, 10, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame296 = {
    SPRITE_FRAME(gSpriteBank00Frame296, SPRITE_TILES_BANK00_FRAME296),
    { { -5, -23, 10, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame297 = {
    SPRITE_FRAME(gSpriteBank00Frame297, SPRITE_TILES_BANK00_FRAME297),
    { { -5, -23, 10, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame298 = {
    SPRITE_FRAME(gSpriteBank00Frame298, SPRITE_TILES_BANK00_FRAME298),
    { { -7, -23, 10, 27 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame299 = {
    SPRITE_FRAME(gSpriteBank00Frame299, SPRITE_TILES_BANK00_FRAME299),
    { { -9, -23, 12, 27 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame300 = {
    SPRITE_FRAME(gSpriteBank00Frame300, SPRITE_TILES_BANK00_FRAME300),
    { { -11, -23, 13, 27 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame301 = {
    SPRITE_FRAME(gSpriteBank00Frame301, SPRITE_TILES_BANK00_FRAME301),
    { { -12, -23, 14, 26 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame302 = {
    SPRITE_FRAME(gSpriteBank00Frame302, SPRITE_TILES_BANK00_FRAME302),
    { { -12, -23, 14, 27 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame303 = {
    SPRITE_FRAME(gSpriteBank00Frame303, SPRITE_TILES_BANK00_FRAME303),
    { { -10, -23, 13, 27 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame304 = {
    SPRITE_FRAME(gSpriteBank00Frame304, SPRITE_TILES_BANK00_FRAME304),
    { { -7, -23, 13, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame305 = {
    SPRITE_FRAME(gSpriteBank00Frame305, SPRITE_TILES_BANK00_FRAME305),
    { { -8, -23, 15, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame306 = {
    SPRITE_FRAME(gSpriteBank00Frame306, SPRITE_TILES_BANK00_FRAME306),
    { { -9, -23, 19, 29 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame307 = {
    SPRITE_FRAME(gSpriteBank00Frame307, SPRITE_TILES_BANK00_FRAME307),
    { { -19, -23, 21, 26 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame308 = {
    SPRITE_FRAME(gSpriteBank00Frame308, SPRITE_TILES_BANK00_FRAME308),
    { { -20, -23, 21, 31 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame309 = {
    SPRITE_FRAME(gSpriteBank00Frame309, SPRITE_TILES_BANK00_FRAME309),
    { { -7, -23, 9, 33 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame310 = {
    SPRITE_FRAME(gSpriteBank00Frame310, SPRITE_TILES_BANK00_FRAME310),
    { { -1, -23, 5, 33 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame311 = {
    SPRITE_FRAME(gSpriteBank00Frame311, SPRITE_TILES_BANK00_FRAME311),
    { { -2, -23, 6, 32 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame312 = {
    SPRITE_FRAME(gSpriteBank00Frame312, SPRITE_TILES_BANK00_FRAME312),
    { { -4, -23, 10, 29 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame313 = {
    SPRITE_FRAME(gSpriteBank00Frame313, SPRITE_TILES_BANK00_FRAME313),
    { { -10, -23, 14, 25 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame314 = {
    SPRITE_FRAME(gSpriteBank00Frame314, SPRITE_TILES_BANK00_FRAME314),
    { { -13, -23, 14, 24 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame315 = {
    SPRITE_FRAME(gSpriteBank00Frame315, SPRITE_TILES_BANK00_FRAME315),
    { { -8, -23, 9, 31 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame316 = {
    SPRITE_FRAME(gSpriteBank00Frame316, SPRITE_TILES_BANK00_FRAME316),
    { { -6, -23, 8, 36 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame317 = {
    SPRITE_FRAME(gSpriteBank00Frame317, SPRITE_TILES_BANK00_FRAME317),
    { { -3, -23, 5, 38 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame318 = {
    SPRITE_FRAME(gSpriteBank00Frame318, SPRITE_TILES_BANK00_FRAME318),
    { { -7, -23, 10, 37 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame319 = {
    SPRITE_FRAME(gSpriteBank00Frame319, SPRITE_TILES_BANK00_FRAME319),
    { { -10, -23, 18, 32 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame320 = {
    SPRITE_FRAME(gSpriteBank00Frame320, SPRITE_TILES_BANK00_FRAME320),
    { { -13, -23, 20, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame321 = {
    SPRITE_FRAME(gSpriteBank00Frame321, SPRITE_TILES_BANK00_FRAME321),
    { { -19, -23, 20, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame322 = {
    SPRITE_FRAME(gSpriteBank00Frame322, SPRITE_TILES_BANK00_FRAME322),
    { { -7, -23, 10, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame323 = {
    SPRITE_FRAME(gSpriteBank00Frame323, SPRITE_TILES_BANK00_FRAME323),
    { { -4, -23, 8, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame324 = {
    SPRITE_FRAME(gSpriteBank00Frame324, SPRITE_TILES_BANK00_FRAME324),
    { { -7, -23, 11, 32 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame325 = {
    SPRITE_FRAME(gSpriteBank00Frame325, SPRITE_TILES_BANK00_FRAME325),
    { { -5, -23, 9, 31 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame326 = {
    SPRITE_FRAME(gSpriteBank00Frame326, SPRITE_TILES_BANK00_FRAME326),
    { { -3, -23, 8, 29 } },
    { -2, -29 },
};
const struct sprite_frame gSpriteBank00Frame327 = SPRITE_FRAME(gSpriteBank00Frame327, SPRITE_TILES_BANK00_FRAME327);
const struct sprite_frame gSpriteBank00Frame328 = SPRITE_FRAME(gSpriteBank00Frame328, SPRITE_TILES_BANK00_FRAME328);
const struct sprite_frame gSpriteBank00Frame329 = SPRITE_FRAME(gSpriteBank00Frame329, SPRITE_TILES_BANK00_FRAME329);
const struct sprite_frame gSpriteBank00Frame330 = SPRITE_FRAME(gSpriteBank00Frame330, SPRITE_TILES_BANK00_FRAME330);
const struct sprite_frame gSpriteBank00Frame331 = SPRITE_FRAME(gSpriteBank00Frame331, SPRITE_TILES_BANK00_FRAME331);
const struct sprite_frame gSpriteBank00Frame332 = SPRITE_FRAME(gSpriteBank00Frame332, SPRITE_TILES_BANK00_FRAME332);
const struct sprite_frame gSpriteBank00Frame333 = SPRITE_FRAME(gSpriteBank00Frame333, SPRITE_TILES_BANK00_FRAME333);
const struct sprite_frame gSpriteBank00Frame334 = SPRITE_FRAME(gSpriteBank00Frame334, SPRITE_TILES_BANK00_FRAME334);
const struct sprite_frame gSpriteBank00Frame335 = SPRITE_FRAME(gSpriteBank00Frame335, SPRITE_TILES_BANK00_FRAME335);
const struct sprite_frame gSpriteBank00Frame336 = SPRITE_FRAME(gSpriteBank00Frame336, SPRITE_TILES_BANK00_FRAME336);
const struct sprite_frame gSpriteBank00Frame337 = SPRITE_FRAME(gSpriteBank00Frame337, SPRITE_TILES_BANK00_FRAME337);
const struct sprite_frame gSpriteBank00Frame338 = SPRITE_FRAME(gSpriteBank00Frame338, SPRITE_TILES_BANK00_FRAME338);
const struct sprite_frame gSpriteBank00Frame339 = SPRITE_FRAME(gSpriteBank00Frame339, SPRITE_TILES_BANK00_FRAME339);
const struct sprite_frame gSpriteBank00Frame340 = SPRITE_FRAME(gSpriteBank00Frame340, SPRITE_TILES_BANK00_FRAME340);
const struct sprite_frame gSpriteBank00Frame341 = SPRITE_FRAME(gSpriteBank00Frame341, SPRITE_TILES_BANK00_FRAME341);
const struct sprite_frame gSpriteBank00Frame342 = SPRITE_FRAME(gSpriteBank00Frame342, SPRITE_TILES_BANK00_FRAME342);
const struct sprite_frame gSpriteBank00Frame343 = SPRITE_FRAME(gSpriteBank00Frame343, SPRITE_TILES_BANK00_FRAME343);
const struct sprite_frame gSpriteBank00Frame344 = SPRITE_FRAME(gSpriteBank00Frame344, SPRITE_TILES_BANK00_FRAME344);
const struct sprite_frame gSpriteBank00Frame345 = SPRITE_FRAME(gSpriteBank00Frame345, SPRITE_TILES_BANK00_FRAME345);
const struct sprite_frame_1box gSpriteBank00Frame346 = {
    SPRITE_FRAME(gSpriteBank00Frame346, SPRITE_TILES_BANK00_FRAME346),
    { { -9, -22, 12, 34 } },
};
const struct sprite_frame_1box gSpriteBank00Frame347 = {
    SPRITE_FRAME(gSpriteBank00Frame347, SPRITE_TILES_BANK00_FRAME347),
    { { -9, -22, 12, 34 } },
};
const struct sprite_frame_1box gSpriteBank00Frame348 = {
    SPRITE_FRAME(gSpriteBank00Frame348, SPRITE_TILES_BANK00_FRAME348),
    { { -9, -22, 12, 34 } },
};
const struct sprite_frame_1box gSpriteBank00Frame349 = {
    SPRITE_FRAME(gSpriteBank00Frame349, SPRITE_TILES_BANK00_FRAME349),
    { { -9, -22, 12, 34 } },
};
const struct sprite_frame_1box gSpriteBank00Frame350 = {
    SPRITE_FRAME(gSpriteBank00Frame350, SPRITE_TILES_BANK00_FRAME350),
    { { -9, -22, 12, 34 } },
};
const struct sprite_frame_1box gSpriteBank00Frame351 = {
    SPRITE_FRAME(gSpriteBank00Frame351, SPRITE_TILES_BANK00_FRAME351),
    { { -9, -22, 12, 34 } },
};
const struct sprite_frame_1box gSpriteBank00Frame352 = {
    SPRITE_FRAME(gSpriteBank00Frame352, SPRITE_TILES_BANK00_FRAME352),
    { { -9, -22, 12, 34 } },
};
const struct sprite_frame_1box gSpriteBank00Frame353 = {
    SPRITE_FRAME(gSpriteBank00Frame353, SPRITE_TILES_BANK00_FRAME353),
    { { -9, -22, 12, 34 } },
};
const struct sprite_frame_1box gSpriteBank00Frame354 = {
    SPRITE_FRAME(gSpriteBank00Frame354, SPRITE_TILES_BANK00_FRAME354),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame355 = {
    SPRITE_FRAME(gSpriteBank00Frame355, SPRITE_TILES_BANK00_FRAME355),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame356 = {
    SPRITE_FRAME(gSpriteBank00Frame356, SPRITE_TILES_BANK00_FRAME356),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame357 = {
    SPRITE_FRAME(gSpriteBank00Frame357, SPRITE_TILES_BANK00_FRAME357),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame358 = {
    SPRITE_FRAME(gSpriteBank00Frame358, SPRITE_TILES_BANK00_FRAME358),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame359 = {
    SPRITE_FRAME(gSpriteBank00Frame359, SPRITE_TILES_BANK00_FRAME359),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame360 = {
    SPRITE_FRAME(gSpriteBank00Frame360, SPRITE_TILES_BANK00_FRAME360),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame361 = {
    SPRITE_FRAME(gSpriteBank00Frame361, SPRITE_TILES_BANK00_FRAME361),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame362 = {
    SPRITE_FRAME(gSpriteBank00Frame362, SPRITE_TILES_BANK00_FRAME362),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame363 = {
    SPRITE_FRAME(gSpriteBank00Frame363, SPRITE_TILES_BANK00_FRAME363),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame364 = {
    SPRITE_FRAME(gSpriteBank00Frame364, SPRITE_TILES_BANK00_FRAME364),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame365 = {
    SPRITE_FRAME(gSpriteBank00Frame365, SPRITE_TILES_BANK00_FRAME365),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame366 = {
    SPRITE_FRAME(gSpriteBank00Frame366, SPRITE_TILES_BANK00_FRAME366),
    { { 0, 0, 0, 0 }, { -15, -19, 27, 27 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame367 = {
    SPRITE_FRAME(gSpriteBank00Frame367, SPRITE_TILES_BANK00_FRAME367),
    { { 0, 0, 0, 0 }, { -16, -17, 28, 25 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame368 = {
    SPRITE_FRAME(gSpriteBank00Frame368, SPRITE_TILES_BANK00_FRAME368),
    { { 0, 0, 0, 0 }, { -21, -14, 33, 23 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame369 = {
    SPRITE_FRAME(gSpriteBank00Frame369, SPRITE_TILES_BANK00_FRAME369),
    { { 0, 0, 0, 0 }, { -19, -12, 31, 22 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame370 = {
    SPRITE_FRAME(gSpriteBank00Frame370, SPRITE_TILES_BANK00_FRAME370),
    { { 0, 0, 0, 0 }, { -21, -12, 34, 22 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame371 = {
    SPRITE_FRAME(gSpriteBank00Frame371, SPRITE_TILES_BANK00_FRAME371),
    { { 0, 0, 0, 0 }, { -19, -12, 35, 22 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame372 = {
    SPRITE_FRAME(gSpriteBank00Frame372, SPRITE_TILES_BANK00_FRAME372),
    { { 0, 0, 0, 0 }, { -18, -12, 32, 22 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame373 = {
    SPRITE_FRAME(gSpriteBank00Frame373, SPRITE_TILES_BANK00_FRAME373),
    { { 0, 0, 0, 0 }, { -18, -13, 34, 22 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame374 = {
    SPRITE_FRAME(gSpriteBank00Frame374, SPRITE_TILES_BANK00_FRAME374),
    { { 0, 0, 0, 0 }, { -19, -17, 29, 25 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame375 = {
    SPRITE_FRAME(gSpriteBank00Frame375, SPRITE_TILES_BANK00_FRAME375),
    { { 0, 0, 0, 0 }, { -18, -20, 26, 28 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame376 = {
    SPRITE_FRAME(gSpriteBank00Frame376, SPRITE_TILES_BANK00_FRAME376),
    { { 0, 0, 0, 0 }, { -15, -19, 27, 27 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame377 = {
    SPRITE_FRAME(gSpriteBank00Frame377, SPRITE_TILES_BANK00_FRAME377),
    { { 0, 0, 0, 0 }, { -16, -17, 28, 25 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame378 = {
    SPRITE_FRAME(gSpriteBank00Frame378, SPRITE_TILES_BANK00_FRAME378),
    { { 0, 0, 0, 0 }, { -21, -14, 33, 23 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame379 = {
    SPRITE_FRAME(gSpriteBank00Frame379, SPRITE_TILES_BANK00_FRAME379),
    { { 0, 0, 0, 0 }, { -19, -12, 31, 22 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame380 = {
    SPRITE_FRAME(gSpriteBank00Frame380, SPRITE_TILES_BANK00_FRAME380),
    { { 0, 0, 0, 0 }, { -21, -12, 34, 22 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame381 = {
    SPRITE_FRAME(gSpriteBank00Frame381, SPRITE_TILES_BANK00_FRAME381),
    { { 0, 0, 0, 0 }, { -19, -12, 35, 22 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame382 = {
    SPRITE_FRAME(gSpriteBank00Frame382, SPRITE_TILES_BANK00_FRAME382),
    { { 0, 0, 0, 0 }, { -18, -12, 32, 22 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame383 = {
    SPRITE_FRAME(gSpriteBank00Frame383, SPRITE_TILES_BANK00_FRAME383),
    { { 0, 0, 0, 0 }, { -18, -13, 34, 22 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame384 = {
    SPRITE_FRAME(gSpriteBank00Frame384, SPRITE_TILES_BANK00_FRAME384),
    { { 0, 0, 0, 0 }, { -19, -17, 29, 25 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame385 = {
    SPRITE_FRAME(gSpriteBank00Frame385, SPRITE_TILES_BANK00_FRAME385),
    { { 0, 0, 0, 0 }, { -18, -20, 26, 28 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame gSpriteBank00Frame386 = SPRITE_FRAME(gSpriteBank00Frame386, SPRITE_TILES_BANK00_FRAME386);
const struct sprite_frame gSpriteBank00Frame387 = SPRITE_FRAME(gSpriteBank00Frame387, SPRITE_TILES_BANK00_FRAME387);
const struct sprite_frame gSpriteBank00Frame388 = SPRITE_FRAME(gSpriteBank00Frame388, SPRITE_TILES_BANK00_FRAME388);
const struct sprite_frame gSpriteBank00Frame389 = SPRITE_FRAME(gSpriteBank00Frame389, SPRITE_TILES_BANK00_FRAME389);
const struct sprite_frame gSpriteBank00Frame390 = SPRITE_FRAME(gSpriteBank00Frame390, SPRITE_TILES_BANK00_FRAME390);
const struct sprite_frame gSpriteBank00Frame391 = SPRITE_FRAME(gSpriteBank00Frame391, SPRITE_TILES_BANK00_FRAME391);
const struct sprite_frame gSpriteBank00Frame392 = SPRITE_FRAME(gSpriteBank00Frame392, SPRITE_TILES_BANK00_FRAME392);
const struct sprite_frame gSpriteBank00Frame393 = SPRITE_FRAME(gSpriteBank00Frame393, SPRITE_TILES_BANK00_FRAME393);
const struct sprite_frame gSpriteBank00Frame394 = SPRITE_FRAME(gSpriteBank00Frame394, SPRITE_TILES_BANK00_FRAME394);
const struct sprite_frame gSpriteBank00Frame395 = SPRITE_FRAME(gSpriteBank00Frame395, SPRITE_TILES_BANK00_FRAME395);
const struct sprite_frame gSpriteBank00Frame396 = SPRITE_FRAME(gSpriteBank00Frame396, SPRITE_TILES_BANK00_FRAME396);
const struct sprite_frame gSpriteBank00Frame397 = SPRITE_FRAME(gSpriteBank00Frame397, SPRITE_TILES_BANK00_FRAME397);
const struct sprite_frame gSpriteBank00Frame398 = SPRITE_FRAME(gSpriteBank00Frame398, SPRITE_TILES_BANK00_FRAME398);
const struct sprite_frame gSpriteBank00Frame399 = SPRITE_FRAME(gSpriteBank00Frame399, SPRITE_TILES_BANK00_FRAME399);
const struct sprite_frame gSpriteBank00Frame400 = SPRITE_FRAME(gSpriteBank00Frame400, SPRITE_TILES_BANK00_FRAME400);
const struct sprite_frame gSpriteBank00Frame401 = SPRITE_FRAME(gSpriteBank00Frame401, SPRITE_TILES_BANK00_FRAME401);
const struct sprite_frame gSpriteBank00Frame402 = SPRITE_FRAME(gSpriteBank00Frame402, SPRITE_TILES_BANK00_FRAME402);
const struct sprite_frame gSpriteBank00Frame403 = SPRITE_FRAME(gSpriteBank00Frame403, SPRITE_TILES_BANK00_FRAME403);
const struct sprite_frame_1box gSpriteBank00Frame404 = {
    SPRITE_FRAME(gSpriteBank00Frame404, SPRITE_TILES_BANK00_FRAME404),
    { { -10, -10, 20, 20 } },
};
const struct sprite_frame_1box gSpriteBank00Frame405 = {
    SPRITE_FRAME(gSpriteBank00Frame405, SPRITE_TILES_BANK00_FRAME405),
    { { -10, -10, 21, 17 } },
};
const struct sprite_frame_1box gSpriteBank00Frame406 = {
    SPRITE_FRAME(gSpriteBank00Frame406, SPRITE_TILES_BANK00_FRAME406),
    { { -10, -10, 20, 15 } },
};
const struct sprite_frame_1box gSpriteBank00Frame407 = {
    SPRITE_FRAME(gSpriteBank00Frame407, SPRITE_TILES_BANK00_FRAME407),
    { { -11, -10, 20, 16 } },
};
const struct sprite_frame_1box gSpriteBank00Frame408 = {
    SPRITE_FRAME(gSpriteBank00Frame408, SPRITE_TILES_BANK00_FRAME408),
    { { -11, -9, 17, 18 } },
};
const struct sprite_frame_1box gSpriteBank00Frame409 = {
    SPRITE_FRAME(gSpriteBank00Frame409, SPRITE_TILES_BANK00_FRAME409),
    { { -12, -9, 18, 19 } },
};
const struct sprite_frame_1box gSpriteBank00Frame410 = {
    SPRITE_FRAME(gSpriteBank00Frame410, SPRITE_TILES_BANK00_FRAME410),
    { { -13, -9, 20, 18 } },
};
const struct sprite_frame_1box gSpriteBank00Frame411 = {
    SPRITE_FRAME(gSpriteBank00Frame411, SPRITE_TILES_BANK00_FRAME411),
    { { -13, -9, 20, 18 } },
};
const struct sprite_frame_1box gSpriteBank00Frame412 = {
    SPRITE_FRAME(gSpriteBank00Frame412, SPRITE_TILES_BANK00_FRAME412),
    { { -13, -9, 20, 18 } },
};
const struct sprite_frame_1box gSpriteBank00Frame413 = {
    SPRITE_FRAME(gSpriteBank00Frame413, SPRITE_TILES_BANK00_FRAME413),
    { { -13, -9, 20, 18 } },
};
const struct sprite_frame_1box gSpriteBank00Frame414 = {
    SPRITE_FRAME(gSpriteBank00Frame414, SPRITE_TILES_BANK00_FRAME414),
    { { -12, -18, 25, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame415 = {
    SPRITE_FRAME(gSpriteBank00Frame415, SPRITE_TILES_BANK00_FRAME415),
    { { -12, -18, 25, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame416 = {
    SPRITE_FRAME(gSpriteBank00Frame416, SPRITE_TILES_BANK00_FRAME416),
    { { -12, -18, 25, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame417 = {
    SPRITE_FRAME(gSpriteBank00Frame417, SPRITE_TILES_BANK00_FRAME417),
    { { -12, -18, 25, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame418 = {
    SPRITE_FRAME(gSpriteBank00Frame418, SPRITE_TILES_BANK00_FRAME418),
    { { -12, -18, 25, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame419 = {
    SPRITE_FRAME(gSpriteBank00Frame419, SPRITE_TILES_BANK00_FRAME419),
    { { -12, -18, 25, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame420 = {
    SPRITE_FRAME(gSpriteBank00Frame420, SPRITE_TILES_BANK00_FRAME420),
    { { -12, -18, 26, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame421 = {
    SPRITE_FRAME(gSpriteBank00Frame421, SPRITE_TILES_BANK00_FRAME421),
    { { -11, -18, 26, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame422 = {
    SPRITE_FRAME(gSpriteBank00Frame422, SPRITE_TILES_BANK00_FRAME422),
    { { -11, -18, 28, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame423 = {
    SPRITE_FRAME(gSpriteBank00Frame423, SPRITE_TILES_BANK00_FRAME423),
    { { -9, -17, 29, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame424 = {
    SPRITE_FRAME(gSpriteBank00Frame424, SPRITE_TILES_BANK00_FRAME424),
    { { -7, -15, 31, 33 } },
};
const struct sprite_frame_1box gSpriteBank00Frame425 = {
    SPRITE_FRAME(gSpriteBank00Frame425, SPRITE_TILES_BANK00_FRAME425),
    { { -3, -7, 35, 27 } },
};
const struct sprite_frame_1box gSpriteBank00Frame426 = {
    SPRITE_FRAME(gSpriteBank00Frame426, SPRITE_TILES_BANK00_FRAME426),
    { { -11, 3, 43, 30 } },
};
const struct sprite_frame_1box gSpriteBank00Frame427 = {
    SPRITE_FRAME(gSpriteBank00Frame427, SPRITE_TILES_BANK00_FRAME427),
    { { -10, 3, 41, 29 } },
};
const struct sprite_frame_1box gSpriteBank00Frame428 = {
    SPRITE_FRAME(gSpriteBank00Frame428, SPRITE_TILES_BANK00_FRAME428),
    { { -9, 4, 38, 25 } },
};
const struct sprite_frame_1box gSpriteBank00Frame429 = {
    SPRITE_FRAME(gSpriteBank00Frame429, SPRITE_TILES_BANK00_FRAME429),
    { { -14, -23, 28, 46 } },
};
const struct sprite_frame_1box gSpriteBank00Frame430 = {
    SPRITE_FRAME(gSpriteBank00Frame430, SPRITE_TILES_BANK00_FRAME430),
    { { -14, -23, 28, 46 } },
};
const struct sprite_frame_1box gSpriteBank00Frame431 = {
    SPRITE_FRAME(gSpriteBank00Frame431, SPRITE_TILES_BANK00_FRAME431),
    { { -14, -23, 28, 46 } },
};
const struct sprite_frame_1box gSpriteBank00Frame432 = {
    SPRITE_FRAME(gSpriteBank00Frame432, SPRITE_TILES_BANK00_FRAME432),
    { { -12, -18, 25, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame433 = {
    SPRITE_FRAME(gSpriteBank00Frame433, SPRITE_TILES_BANK00_FRAME433),
    { { -16, -17, 30, 35 } },
};
const struct sprite_frame_1box gSpriteBank00Frame434 = {
    SPRITE_FRAME(gSpriteBank00Frame434, SPRITE_TILES_BANK00_FRAME434),
    { { -17, -17, 32, 35 } },
};
const struct sprite_frame_1box gSpriteBank00Frame435 = {
    SPRITE_FRAME(gSpriteBank00Frame435, SPRITE_TILES_BANK00_FRAME435),
    { { -21, -25, 40, 44 } },
};
const struct sprite_frame_1box gSpriteBank00Frame436 = {
    SPRITE_FRAME(gSpriteBank00Frame436, SPRITE_TILES_BANK00_FRAME436),
    { { -22, -29, 44, 48 } },
};
const struct sprite_frame_1box gSpriteBank00Frame437 = {
    SPRITE_FRAME(gSpriteBank00Frame437, SPRITE_TILES_BANK00_FRAME437),
    { { -20, -25, 40, 44 } },
};
const struct sprite_frame_1box gSpriteBank00Frame438 = {
    SPRITE_FRAME(gSpriteBank00Frame438, SPRITE_TILES_BANK00_FRAME438),
    { { -13, -10, 24, 27 } },
};
const struct sprite_frame_1box gSpriteBank00Frame439 = {
    SPRITE_FRAME(gSpriteBank00Frame439, SPRITE_TILES_BANK00_FRAME439),
    { { -4, 6, 7, 9 } },
};
const struct sprite_frame gSpriteBank00Frame440 = { gSpriteBank00Frame441Pos, gSpriteBank00Frame441Pieces, SPRITE_FRAME_TILES(SPRITE_TILES_BANK00_FRAME440, 0) };
const struct sprite_frame_1box gSpriteBank00Frame441 = {
    SPRITE_FRAME(gSpriteBank00Frame441, SPRITE_TILES_BANK00_FRAME441),
    { { -11, -19, 23, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame442 = {
    SPRITE_FRAME(gSpriteBank00Frame442, SPRITE_TILES_BANK00_FRAME442),
    { { -10, -19, 22, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame443 = {
    SPRITE_FRAME(gSpriteBank00Frame443, SPRITE_TILES_BANK00_FRAME443),
    { { -10, -19, 22, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame444 = {
    SPRITE_FRAME(gSpriteBank00Frame444, SPRITE_TILES_BANK00_FRAME444),
    { { -11, -19, 23, 40 } },
};
const struct sprite_frame_1box gSpriteBank00Frame445 = {
    SPRITE_FRAME(gSpriteBank00Frame445, SPRITE_TILES_BANK00_FRAME445),
    { { -13, -19, 25, 40 } },
};
const struct sprite_frame_1box gSpriteBank00Frame446 = {
    SPRITE_FRAME(gSpriteBank00Frame446, SPRITE_TILES_BANK00_FRAME446),
    { { -14, -18, 25, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame447 = {
    SPRITE_FRAME(gSpriteBank00Frame447, SPRITE_TILES_BANK00_FRAME447),
    { { -14, -15, 25, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame448 = {
    SPRITE_FRAME(gSpriteBank00Frame448, SPRITE_TILES_BANK00_FRAME448),
    { { -14, -9, 25, 30 } },
};
const struct sprite_frame_1box gSpriteBank00Frame449 = {
    SPRITE_FRAME(gSpriteBank00Frame449, SPRITE_TILES_BANK00_FRAME449),
    { { -15, -14, 28, 35 } },
};
const struct sprite_frame_1box gSpriteBank00Frame450 = {
    SPRITE_FRAME(gSpriteBank00Frame450, SPRITE_TILES_BANK00_FRAME450),
    { { -15, -11, 30, 32 } },
};
const struct sprite_frame_1box gSpriteBank00Frame451 = {
    SPRITE_FRAME(gSpriteBank00Frame451, SPRITE_TILES_BANK00_FRAME451),
    { { -15, -10, 30, 31 } },
};
const struct sprite_frame_1box gSpriteBank00Frame452 = {
    SPRITE_FRAME(gSpriteBank00Frame452, SPRITE_TILES_BANK00_FRAME452),
    { { -14, -7, 25, 28 } },
};
const struct sprite_frame_1box gSpriteBank00Frame453 = {
    SPRITE_FRAME(gSpriteBank00Frame453, SPRITE_TILES_BANK00_FRAME453),
    { { -14, -4, 25, 25 } },
};
const struct sprite_frame_1box gSpriteBank00Frame454 = {
    SPRITE_FRAME(gSpriteBank00Frame454, SPRITE_TILES_BANK00_FRAME454),
    { { -14, 8, 25, 13 } },
};
const struct sprite_frame_1box gSpriteBank00Frame455 = {
    SPRITE_FRAME(gSpriteBank00Frame455, SPRITE_TILES_BANK00_FRAME455),
    { { -10, -21, 20, 43 } },
};
const struct sprite_frame_1box gSpriteBank00Frame456 = {
    SPRITE_FRAME(gSpriteBank00Frame456, SPRITE_TILES_BANK00_FRAME456),
    { { -8, -18, 18, 40 } },
};
const struct sprite_frame_1box gSpriteBank00Frame457 = {
    SPRITE_FRAME(gSpriteBank00Frame457, SPRITE_TILES_BANK00_FRAME457),
    { { -8, -15, 17, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame458 = {
    SPRITE_FRAME(gSpriteBank00Frame458, SPRITE_TILES_BANK00_FRAME458),
    { { -18, -17, 28, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame459 = {
    SPRITE_FRAME(gSpriteBank00Frame459, SPRITE_TILES_BANK00_FRAME459),
    { { -19, -23, 34, 45 } },
};
const struct sprite_frame_1box gSpriteBank00Frame460 = {
    SPRITE_FRAME(gSpriteBank00Frame460, SPRITE_TILES_BANK00_FRAME460),
    { { -23, -27, 38, 49 } },
};
const struct sprite_frame_1box gSpriteBank00Frame461 = {
    SPRITE_FRAME(gSpriteBank00Frame461, SPRITE_TILES_BANK00_FRAME461),
    { { -15, -16, 39, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame462 = {
    SPRITE_FRAME(gSpriteBank00Frame462, SPRITE_TILES_BANK00_FRAME462),
    { { -15, -4, 37, 26 } },
};
const struct sprite_frame_1box gSpriteBank00Frame463 = {
    SPRITE_FRAME(gSpriteBank00Frame463, SPRITE_TILES_BANK00_FRAME463),
    { { -15, -7, 40, 29 } },
};
const struct sprite_frame_1box gSpriteBank00Frame464 = {
    SPRITE_FRAME(gSpriteBank00Frame464, SPRITE_TILES_BANK00_FRAME464),
    { { -15, 4, 39, 18 } },
};

/* The piece arrays, from graphics/sprites/bank00_crash/bank.json. */
#include "sprites/bank00_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 1: 47 animations, 274 frames, graphics/sprites/bank01_crash_underwater/. */

extern const u16 gSpriteBank01Anim00Seq[2];
extern const u16 gSpriteBank01Anim01Seq[4];
extern const u16 gSpriteBank01Anim02Seq[3];
extern const u16 gSpriteBank01Anim03Seq[4];
extern const u16 gSpriteBank01Anim04Seq[2];
extern const u16 gSpriteBank01Anim05Seq[4];
extern const u16 gSpriteBank01Anim06Seq[2];
extern const u16 gSpriteBank01Anim07Seq[8];
extern const u16 gSpriteBank01Anim08Seq[4];
extern const u16 gSpriteBank01Anim09Seq[3];
extern const u16 gSpriteBank01Anim10Seq[4];
extern const u16 gSpriteBank01Anim11Seq[2];
extern const u16 gSpriteBank01Anim12Seq[4];
extern const u16 gSpriteBank01Anim13Seq[2];
extern const u16 gSpriteBank01Anim14Seq[8];
extern const u16 gSpriteBank01Anim15Seq[8];
extern const u16 gSpriteBank01Anim16Seq[8];
extern const u16 gSpriteBank01Anim17Seq[8];
extern const u16 gSpriteBank01Anim18Seq[8];
extern const u16 gSpriteBank01Anim19Seq[4];
extern const u16 gSpriteBank01Anim20Seq[4];
extern const u16 gSpriteBank01Anim21Seq[2];
extern const u16 gSpriteBank01Anim22Seq[2];
extern const u16 gSpriteBank01Anim23Seq[8];
extern const u16 gSpriteBank01Anim24Seq[4];
extern const u16 gSpriteBank01Anim25Seq[6];
extern const u16 gSpriteBank01Anim26Seq[6];
extern const u16 gSpriteBank01Anim27Seq[6];
extern const u16 gSpriteBank01Anim28Seq[6];
extern const u16 gSpriteBank01Anim29Seq[3];
extern const u16 gSpriteBank01Anim30Seq[5];
extern const u16 gSpriteBank01Anim31Seq[15];
extern const u16 gSpriteBank01Anim32Seq[8];
extern const u16 gSpriteBank01Anim33Seq[8];
extern const u16 gSpriteBank01Anim34Seq[8];
extern const u16 gSpriteBank01Anim35Seq[8];
extern const u16 gSpriteBank01Anim36Seq[8];
extern const u16 gSpriteBank01Anim37Seq[8];
extern const u16 gSpriteBank01Anim38Seq[4];
extern const u16 gSpriteBank01Anim39Seq[4];
extern const u16 gSpriteBank01Anim40Seq[2];
extern const u16 gSpriteBank01Anim41Seq[8];
extern const u16 gSpriteBank01Anim42Seq[1];
extern const u16 gSpriteBank01Anim43Seq[15];
extern const u16 gSpriteBank01Anim44Seq[15];
extern const u16 gSpriteBank01Anim45Seq[9];
extern const u16 gSpriteBank01Anim46Seq[21];

const struct sprite_anim gSpriteBank01Anims[47] = {
    /* 0 */ {
        /* seq */ gSpriteBank01Anim00Seq,
        /* box */ { { -5, -7, 18, 20 }, { -12, -17, 34, 43 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank01Anim01Seq,
        /* box */ { { -5, -7, 18, 20 }, { -20, -21, 37, 45 } },
        /* paletteId */ 5,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank01Anim02Seq,
        /* box */ { { -5, -7, 18, 20 }, { -18, -17, 38, 42 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim02Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 3 */ {
        /* seq */ gSpriteBank01Anim03Seq,
        /* box */ { { -5, -7, 18, 20 }, { -20, -21, 47, 42 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim03Seq),
        /* flags */ 0,
    },
    /* 4 */ {
        /* seq */ gSpriteBank01Anim04Seq,
        /* box */ { { -5, -7, 18, 20 }, { -13, -12, 32, 34 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim04Seq),
        /* flags */ 0,
    },
    /* 5 */ {
        /* seq */ gSpriteBank01Anim05Seq,
        /* box */ { { -5, -7, 18, 20 }, { -25, -21, 47, 45 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim05Seq),
        /* flags */ 0,
    },
    /* 6 */ {
        /* seq */ gSpriteBank01Anim06Seq,
        /* box */ { { -5, -7, 18, 20 }, { -15, -19, 37, 43 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim06Seq),
        /* flags */ 0,
    },
    /* 7 */ {
        /* seq */ gSpriteBank01Anim07Seq,
        /* box */ { { -5, -7, 18, 20 }, { -22, -12, 45, 38 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim07Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 8 */ {
        /* seq */ gSpriteBank01Anim08Seq,
        /* box */ { { -5, -7, 18, 20 }, { -22, -11, 45, 37 } },
        /* paletteId */ 5,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim08Seq),
        /* flags */ 0,
    },
    /* 9 */ {
        /* seq */ gSpriteBank01Anim09Seq,
        /* box */ { { -5, -7, 18, 20 }, { -18, -14, 42, 38 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim09Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 10 */ {
        /* seq */ gSpriteBank01Anim10Seq,
        /* box */ { { -5, -7, 18, 20 }, { -12, -11, 32, 41 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim10Seq),
        /* flags */ 0,
    },
    /* 11 */ {
        /* seq */ gSpriteBank01Anim11Seq,
        /* box */ { { -5, -7, 18, 20 }, { -13, -15, 32, 34 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim11Seq),
        /* flags */ 0,
    },
    /* 12 */ {
        /* seq */ gSpriteBank01Anim12Seq,
        /* box */ { { -5, -7, 18, 20 }, { -12, -14, 36, 43 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim12Seq),
        /* flags */ 0,
    },
    /* 13 */ {
        /* seq */ gSpriteBank01Anim13Seq,
        /* box */ { { -5, -7, 18, 20 }, { -15, -17, 37, 43 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim13Seq),
        /* flags */ 0,
    },
    /* 14 */ {
        /* seq */ gSpriteBank01Anim14Seq,
        /* box */ { { -5, -7, 18, 20 }, { -19, -19, 37, 42 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim14Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 15 */ {
        /* seq */ gSpriteBank01Anim15Seq,
        /* box */ { { -5, -7, 18, 20 }, { -20, -16, 41, 36 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim15Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 16 */ {
        /* seq */ gSpriteBank01Anim16Seq,
        /* box */ { { -5, -7, 18, 20 }, { -20, -19, 39, 40 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim16Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 17 */ {
        /* seq */ gSpriteBank01Anim17Seq,
        /* box */ { { -5, -7, 18, 20 }, { -20, -22, 35, 47 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim17Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 18 */ {
        /* seq */ gSpriteBank01Anim18Seq,
        /* box */ { { -5, -7, 18, 20 }, { -18, -21, 35, 47 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim18Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 19 */ {
        /* seq */ gSpriteBank01Anim19Seq,
        /* box */ { { -5, -7, 18, 20 }, { -19, -19, 37, 42 } },
        /* paletteId */ 5,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim19Seq),
        /* flags */ 0,
    },
    /* 20 */ {
        /* seq */ gSpriteBank01Anim20Seq,
        /* box */ { { -5, -7, 18, 20 }, { -10, -19, 32, 41 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim20Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 21 */ {
        /* seq */ gSpriteBank01Anim21Seq,
        /* box */ { { -5, -7, 18, 20 }, { -12, -19, 34, 43 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim21Seq),
        /* flags */ 0,
    },
    /* 22 */ {
        /* seq */ gSpriteBank01Anim22Seq,
        /* box */ { { -5, -7, 18, 20 }, { -11, -19, 24, 40 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim22Seq),
        /* flags */ 0,
    },
    /* 23 */ {
        /* seq */ gSpriteBank01Anim23Seq,
        /* box */ { { -5, -7, 18, 20 }, { -22, -16, 47, 34 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim23Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 24 */ {
        /* seq */ gSpriteBank01Anim24Seq,
        /* box */ { { -5, -7, 18, 20 }, { -22, -15, 47, 30 } },
        /* paletteId */ 5,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim24Seq),
        /* flags */ 0,
    },
    /* 25 */ {
        /* seq */ gSpriteBank01Anim25Seq,
        /* box */ { { -5, -7, 18, 20 }, { -25, -19, 53, 33 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim25Seq),
        /* flags */ 0,
    },
    /* 26 */ {
        /* seq */ gSpriteBank01Anim26Seq,
        /* box */ { { -5, -7, 18, 20 }, { -20, -15, 46, 35 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim26Seq),
        /* flags */ 0,
    },
    /* 27 */ {
        /* seq */ gSpriteBank01Anim27Seq,
        /* box */ { { -5, -7, 18, 20 }, { -25, -18, 49, 35 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim27Seq),
        /* flags */ 0,
    },
    /* 28 */ {
        /* seq */ gSpriteBank01Anim28Seq,
        /* box */ { { -5, -7, 18, 20 }, { -24, -13, 55, 36 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim28Seq),
        /* flags */ 0,
    },
    /* 29 */ {
        /* seq */ gSpriteBank01Anim29Seq,
        /* box */ { { -5, -7, 18, 20 }, { -23, -11, 47, 32 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim29Seq),
        /* flags */ 0,
    },
    /* 30 */ {
        /* seq */ gSpriteBank01Anim30Seq,
        /* box */ { { -5, -7, 18, 20 }, { -24, -13, 48, 33 } },
        /* paletteId */ 5,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim30Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 31 */ {
        /* seq */ gSpriteBank01Anim31Seq,
        /* box */ { { -5, -7, 18, 20 }, { -24, -11, 49, 35 } },
        /* paletteId */ 7,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim31Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 32 */ {
        /* seq */ gSpriteBank01Anim32Seq,
        /* box */ { { -5, -7, 18, 20 }, { -23, -16, 47, 37 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim32Seq),
        /* flags */ 0,
    },
    /* 33 */ {
        /* seq */ gSpriteBank01Anim33Seq,
        /* box */ { { -5, -7, 18, 20 }, { -13, -16, 37, 42 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim33Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 34 */ {
        /* seq */ gSpriteBank01Anim34Seq,
        /* box */ { { -5, -7, 18, 20 }, { -22, -11, 47, 35 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim34Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 35 */ {
        /* seq */ gSpriteBank01Anim35Seq,
        /* box */ { { -5, -7, 18, 20 }, { -23, -9, 47, 35 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim35Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 36 */ {
        /* seq */ gSpriteBank01Anim36Seq,
        /* box */ { { -5, -7, 18, 20 }, { -20, -13, 40, 39 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim36Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 37 */ {
        /* seq */ gSpriteBank01Anim37Seq,
        /* box */ { { -5, -7, 18, 20 }, { -17, -15, 36, 41 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim37Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 38 */ {
        /* seq */ gSpriteBank01Anim38Seq,
        /* box */ { { -5, -7, 18, 20 }, { -13, -16, 37, 42 } },
        /* paletteId */ 5,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim38Seq),
        /* flags */ 0,
    },
    /* 39 */ {
        /* seq */ gSpriteBank01Anim39Seq,
        /* box */ { { -5, -7, 18, 20 }, { -10, -15, 32, 41 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim39Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 40 */ {
        /* seq */ gSpriteBank01Anim40Seq,
        /* box */ { { -5, -7, 18, 20 }, { -11, -14, 24, 40 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim40Seq),
        /* flags */ 0,
    },
    /* 41 */ {
        /* seq */ gSpriteBank01Anim41Seq,
        /* box */ { { -5, -7, 18, 20 }, { -20, -21, 38, 45 } },
        /* paletteId */ 5,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim41Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 42 */ {
        /* seq */ gSpriteBank01Anim42Seq,
        /* box */ { { -6, -2, 13, 4 }, { -6, -2, 13, 4 } },
        /* paletteId */ 7,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim42Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 43 */ {
        /* seq */ gSpriteBank01Anim43Seq,
        /* box */ { { -16, -28, 32, 57 }, { -16, -28, 32, 57 } },
        /* paletteId */ 116,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim43Seq),
        /* flags */ 0,
    },
    /* 44 */ {
        /* seq */ gSpriteBank01Anim44Seq,
        /* box */ { { -15, -23, 31, 46 }, { -69, -51, 85, 74 } },
        /* paletteId */ 5,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim44Seq),
        /* flags */ 0,
    },
    /* 45 */ {
        /* seq */ gSpriteBank01Anim45Seq,
        /* box */ { { -25, -15, 51, 31 }, { -25, -30, 57, 56 } },
        /* paletteId */ 5,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim45Seq),
        /* flags */ 0,
    },
    /* 46 */ {
        /* seq */ gSpriteBank01Anim46Seq,
        /* box */ { { -22, -17, 45, 34 }, { -22, -17, 47, 41 } },
        /* paletteId */ 121,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim46Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank01Anim00Seq[2] = {
    0, 1,
};
const u16 gSpriteBank01Anim01Seq[4] = {
    2, 3, 4, 5,
};
const u16 gSpriteBank01Anim02Seq[3] = {
    6, 7, 8,
};
const u16 gSpriteBank01Anim03Seq[4] = {
    9, 10, 11, 12,
};
const u16 gSpriteBank01Anim04Seq[2] = {
    13, 14,
};
const u16 gSpriteBank01Anim05Seq[4] = {
    15, 16, 17, 18,
};
const u16 gSpriteBank01Anim06Seq[2] = {
    19, 20,
};
const u16 gSpriteBank01Anim07Seq[8] = {
    21, 22, 23, 24, 25, 26, 27, 28,
};
const u16 gSpriteBank01Anim08Seq[4] = {
    29, 30, 31, 32,
};
const u16 gSpriteBank01Anim09Seq[3] = {
    33, 34, 35,
};
const u16 gSpriteBank01Anim10Seq[4] = {
    36, 37, 38, 39,
};
const u16 gSpriteBank01Anim11Seq[2] = {
    40, 41,
};
const u16 gSpriteBank01Anim12Seq[4] = {
    42, 43, 44, 45,
};
const u16 gSpriteBank01Anim13Seq[2] = {
    46, 47,
};
const u16 gSpriteBank01Anim14Seq[8] = {
    48, 49, 50, 51, 52, 53, 54, 55,
};
const u16 gSpriteBank01Anim15Seq[8] = {
    56, 57, 58, 59, 60, 61, 62, 63,
};
const u16 gSpriteBank01Anim16Seq[8] = {
    64, 65, 66, 67, 68, 69, 70, 71,
};
const u16 gSpriteBank01Anim17Seq[8] = {
    72, 73, 74, 75, 76, 77, 78, 79,
};
const u16 gSpriteBank01Anim18Seq[8] = {
    80, 81, 82, 83, 84, 85, 86, 87,
};
const u16 gSpriteBank01Anim19Seq[4] = {
    88, 89, 90, 91,
};
const u16 gSpriteBank01Anim20Seq[4] = {
    92, 93, 94, 95,
};
const u16 gSpriteBank01Anim21Seq[2] = {
    96, 97,
};
const u16 gSpriteBank01Anim22Seq[2] = {
    98, 99,
};
const u16 gSpriteBank01Anim23Seq[8] = {
    100, 101, 102, 103, 104, 105, 106, 107,
};
const u16 gSpriteBank01Anim24Seq[4] = {
    108, 109, 110, 111,
};
const u16 gSpriteBank01Anim25Seq[6] = {
    112, 113, 114, 115, 116, 117,
};
const u16 gSpriteBank01Anim26Seq[6] = {
    118, 119, 120, 121, 122, 123,
};
const u16 gSpriteBank01Anim27Seq[6] = {
    124, 125, 126, 127, 128, 129,
};
const u16 gSpriteBank01Anim28Seq[6] = {
    130, 131, 132, 133, 134, 135,
};
const u16 gSpriteBank01Anim29Seq[3] = {
    136, 137, 138,
};
const u16 gSpriteBank01Anim30Seq[5] = {
    139, 140, 141, 142, 143,
};
const u16 gSpriteBank01Anim31Seq[15] = {
    144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158,
};
const u16 gSpriteBank01Anim32Seq[8] = {
    159, 160, 161, 162, 163, 164, 165, 166,
};
const u16 gSpriteBank01Anim33Seq[8] = {
    167, 168, 169, 170, 171, 172, 173, 174,
};
const u16 gSpriteBank01Anim34Seq[8] = {
    175, 176, 177, 178, 179, 180, 181, 182,
};
const u16 gSpriteBank01Anim35Seq[8] = {
    183, 184, 185, 186, 187, 188, 189, 190,
};
const u16 gSpriteBank01Anim36Seq[8] = {
    191, 192, 193, 194, 195, 196, 197, 198,
};
const u16 gSpriteBank01Anim37Seq[8] = {
    199, 200, 201, 202, 203, 204, 205, 206,
};
const u16 gSpriteBank01Anim38Seq[4] = {
    207, 208, 209, 210,
};
const u16 gSpriteBank01Anim39Seq[4] = {
    211, 212, 213, 214,
};
const u16 gSpriteBank01Anim40Seq[2] = {
    215, 216,
};
const u16 gSpriteBank01Anim41Seq[8] = {
    217, 218, 219, 220, 221, 222, 223, 224,
};
const u16 gSpriteBank01Anim42Seq[1] = {
    225,
};
const u16 gSpriteBank01Anim43Seq[15] = {
    226, 227, 228, 226, 227, 228, 226, 227, 228, 226, 227, 228, 226, 227, 228,
};
const u16 gSpriteBank01Anim44Seq[15] = {
    229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243,
};
const u16 gSpriteBank01Anim45Seq[9] = {
    244, 245, 246, 247, 248, 249, 250, 251, 252,
};
const u16 gSpriteBank01Anim46Seq[21] = {
    253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268,
    269, 270, 271, 272, 273,
};

const struct sprite_frame *const gSpriteBank01Frames[274] = {
    &gSpriteBank01Frame000.frame,
    &gSpriteBank01Frame001.frame,
    &gSpriteBank01Frame002.frame,
    &gSpriteBank01Frame003.frame,
    &gSpriteBank01Frame004.frame,
    &gSpriteBank01Frame005.frame,
    &gSpriteBank01Frame006.frame,
    &gSpriteBank01Frame007.frame,
    &gSpriteBank01Frame008.frame,
    &gSpriteBank01Frame009.frame,
    &gSpriteBank01Frame010.frame,
    &gSpriteBank01Frame011.frame,
    &gSpriteBank01Frame012.frame,
    &gSpriteBank01Frame013.frame,
    &gSpriteBank01Frame014.frame,
    &gSpriteBank01Frame015.frame,
    &gSpriteBank01Frame016.frame,
    &gSpriteBank01Frame017.frame,
    &gSpriteBank01Frame018.frame,
    &gSpriteBank01Frame019.frame,
    &gSpriteBank01Frame020.frame,
    &gSpriteBank01Frame021.frame,
    &gSpriteBank01Frame022.frame,
    &gSpriteBank01Frame023.frame,
    &gSpriteBank01Frame024.frame,
    &gSpriteBank01Frame025.frame,
    &gSpriteBank01Frame026.frame,
    &gSpriteBank01Frame027.frame,
    &gSpriteBank01Frame028.frame,
    &gSpriteBank01Frame029.frame,
    &gSpriteBank01Frame030.frame,
    &gSpriteBank01Frame031.frame,
    &gSpriteBank01Frame032.frame,
    &gSpriteBank01Frame033.frame,
    &gSpriteBank01Frame034.frame,
    &gSpriteBank01Frame035.frame,
    &gSpriteBank01Frame036.frame,
    &gSpriteBank01Frame037.frame,
    &gSpriteBank01Frame038.frame,
    &gSpriteBank01Frame039.frame,
    &gSpriteBank01Frame040.frame,
    &gSpriteBank01Frame041.frame,
    &gSpriteBank01Frame042.frame,
    &gSpriteBank01Frame043.frame,
    &gSpriteBank01Frame044.frame,
    &gSpriteBank01Frame045.frame,
    &gSpriteBank01Frame046.frame,
    &gSpriteBank01Frame047.frame,
    &gSpriteBank01Frame048.frame,
    &gSpriteBank01Frame049.frame,
    &gSpriteBank01Frame050.frame,
    &gSpriteBank01Frame051.frame,
    &gSpriteBank01Frame052.frame,
    &gSpriteBank01Frame053.frame,
    &gSpriteBank01Frame054.frame,
    &gSpriteBank01Frame055.frame,
    &gSpriteBank01Frame056.frame,
    &gSpriteBank01Frame057.frame,
    &gSpriteBank01Frame058.frame,
    &gSpriteBank01Frame059.frame,
    &gSpriteBank01Frame060.frame,
    &gSpriteBank01Frame061.frame,
    &gSpriteBank01Frame062.frame,
    &gSpriteBank01Frame063.frame,
    &gSpriteBank01Frame064.frame,
    &gSpriteBank01Frame065.frame,
    &gSpriteBank01Frame066.frame,
    &gSpriteBank01Frame067.frame,
    &gSpriteBank01Frame068.frame,
    &gSpriteBank01Frame069.frame,
    &gSpriteBank01Frame070.frame,
    &gSpriteBank01Frame071.frame,
    &gSpriteBank01Frame072.frame,
    &gSpriteBank01Frame073.frame,
    &gSpriteBank01Frame074.frame,
    &gSpriteBank01Frame075.frame,
    &gSpriteBank01Frame076.frame,
    &gSpriteBank01Frame077.frame,
    &gSpriteBank01Frame078.frame,
    &gSpriteBank01Frame079.frame,
    &gSpriteBank01Frame080.frame,
    &gSpriteBank01Frame081.frame,
    &gSpriteBank01Frame082.frame,
    &gSpriteBank01Frame083.frame,
    &gSpriteBank01Frame084.frame,
    &gSpriteBank01Frame085.frame,
    &gSpriteBank01Frame086.frame,
    &gSpriteBank01Frame087.frame,
    &gSpriteBank01Frame088.frame,
    &gSpriteBank01Frame089.frame,
    &gSpriteBank01Frame090.frame,
    &gSpriteBank01Frame091.frame,
    &gSpriteBank01Frame092.frame,
    &gSpriteBank01Frame093.frame,
    &gSpriteBank01Frame094.frame,
    &gSpriteBank01Frame095.frame,
    &gSpriteBank01Frame096.frame,
    &gSpriteBank01Frame097.frame,
    &gSpriteBank01Frame098.frame,
    &gSpriteBank01Frame099.frame,
    &gSpriteBank01Frame100.frame,
    &gSpriteBank01Frame101.frame,
    &gSpriteBank01Frame102.frame,
    &gSpriteBank01Frame103.frame,
    &gSpriteBank01Frame104.frame,
    &gSpriteBank01Frame105.frame,
    &gSpriteBank01Frame106.frame,
    &gSpriteBank01Frame107.frame,
    &gSpriteBank01Frame108.frame,
    &gSpriteBank01Frame109.frame,
    &gSpriteBank01Frame110.frame,
    &gSpriteBank01Frame111.frame,
    &gSpriteBank01Frame112.frame,
    &gSpriteBank01Frame113.frame,
    &gSpriteBank01Frame114.frame,
    &gSpriteBank01Frame115.frame,
    &gSpriteBank01Frame116.frame,
    &gSpriteBank01Frame117.frame,
    &gSpriteBank01Frame118.frame,
    &gSpriteBank01Frame119.frame,
    &gSpriteBank01Frame120.frame,
    &gSpriteBank01Frame121.frame,
    &gSpriteBank01Frame122.frame,
    &gSpriteBank01Frame123.frame,
    &gSpriteBank01Frame124.frame,
    &gSpriteBank01Frame125.frame,
    &gSpriteBank01Frame126.frame,
    &gSpriteBank01Frame127.frame,
    &gSpriteBank01Frame128.frame,
    &gSpriteBank01Frame129.frame,
    &gSpriteBank01Frame130.frame,
    &gSpriteBank01Frame131.frame,
    &gSpriteBank01Frame132.frame,
    &gSpriteBank01Frame133.frame,
    &gSpriteBank01Frame134.frame,
    &gSpriteBank01Frame135.frame,
    &gSpriteBank01Frame136.frame,
    &gSpriteBank01Frame137.frame,
    &gSpriteBank01Frame138.frame,
    &gSpriteBank01Frame139.frame,
    &gSpriteBank01Frame140.frame,
    &gSpriteBank01Frame141.frame,
    &gSpriteBank01Frame142.frame,
    &gSpriteBank01Frame143.frame,
    &gSpriteBank01Frame144.frame,
    &gSpriteBank01Frame145.frame,
    &gSpriteBank01Frame146.frame,
    &gSpriteBank01Frame147.frame,
    &gSpriteBank01Frame148.frame,
    &gSpriteBank01Frame149.frame,
    &gSpriteBank01Frame150.frame,
    &gSpriteBank01Frame151.frame,
    &gSpriteBank01Frame152.frame,
    &gSpriteBank01Frame153.frame,
    &gSpriteBank01Frame154.frame,
    &gSpriteBank01Frame155.frame,
    &gSpriteBank01Frame156.frame,
    &gSpriteBank01Frame157.frame,
    &gSpriteBank01Frame158.frame,
    &gSpriteBank01Frame159.frame,
    &gSpriteBank01Frame160.frame,
    &gSpriteBank01Frame161.frame,
    &gSpriteBank01Frame162.frame,
    &gSpriteBank01Frame163.frame,
    &gSpriteBank01Frame164.frame,
    &gSpriteBank01Frame165.frame,
    &gSpriteBank01Frame166.frame,
    &gSpriteBank01Frame167.frame,
    &gSpriteBank01Frame168.frame,
    &gSpriteBank01Frame169.frame,
    &gSpriteBank01Frame170.frame,
    &gSpriteBank01Frame171.frame,
    &gSpriteBank01Frame172.frame,
    &gSpriteBank01Frame173.frame,
    &gSpriteBank01Frame174.frame,
    &gSpriteBank01Frame175.frame,
    &gSpriteBank01Frame176.frame,
    &gSpriteBank01Frame177.frame,
    &gSpriteBank01Frame178.frame,
    &gSpriteBank01Frame179.frame,
    &gSpriteBank01Frame180.frame,
    &gSpriteBank01Frame181.frame,
    &gSpriteBank01Frame182.frame,
    &gSpriteBank01Frame183.frame,
    &gSpriteBank01Frame184.frame,
    &gSpriteBank01Frame185.frame,
    &gSpriteBank01Frame186.frame,
    &gSpriteBank01Frame187.frame,
    &gSpriteBank01Frame188.frame,
    &gSpriteBank01Frame189.frame,
    &gSpriteBank01Frame190.frame,
    &gSpriteBank01Frame191.frame,
    &gSpriteBank01Frame192.frame,
    &gSpriteBank01Frame193.frame,
    &gSpriteBank01Frame194.frame,
    &gSpriteBank01Frame195.frame,
    &gSpriteBank01Frame196.frame,
    &gSpriteBank01Frame197.frame,
    &gSpriteBank01Frame198.frame,
    &gSpriteBank01Frame199.frame,
    &gSpriteBank01Frame200.frame,
    &gSpriteBank01Frame201.frame,
    &gSpriteBank01Frame202.frame,
    &gSpriteBank01Frame203.frame,
    &gSpriteBank01Frame204.frame,
    &gSpriteBank01Frame205.frame,
    &gSpriteBank01Frame206.frame,
    &gSpriteBank01Frame207.frame,
    &gSpriteBank01Frame208.frame,
    &gSpriteBank01Frame209.frame,
    &gSpriteBank01Frame210.frame,
    &gSpriteBank01Frame211.frame,
    &gSpriteBank01Frame212.frame,
    &gSpriteBank01Frame213.frame,
    &gSpriteBank01Frame214.frame,
    &gSpriteBank01Frame215.frame,
    &gSpriteBank01Frame216.frame,
    &gSpriteBank01Frame217.frame,
    &gSpriteBank01Frame218.frame,
    &gSpriteBank01Frame219.frame,
    &gSpriteBank01Frame220.frame,
    &gSpriteBank01Frame221.frame,
    &gSpriteBank01Frame222.frame,
    &gSpriteBank01Frame223.frame,
    &gSpriteBank01Frame224.frame,
    &gSpriteBank01Frame225.frame,
    &gSpriteBank01Frame226,
    &gSpriteBank01Frame227,
    &gSpriteBank01Frame228,
    &gSpriteBank01Frame229,
    &gSpriteBank01Frame230,
    &gSpriteBank01Frame231,
    &gSpriteBank01Frame232,
    &gSpriteBank01Frame233,
    &gSpriteBank01Frame234,
    &gSpriteBank01Frame235,
    &gSpriteBank01Frame236,
    &gSpriteBank01Frame237,
    &gSpriteBank01Frame238,
    &gSpriteBank01Frame239,
    &gSpriteBank01Frame240,
    &gSpriteBank01Frame241,
    &gSpriteBank01Frame242,
    &gSpriteBank01Frame243,
    &gSpriteBank01Frame244,
    &gSpriteBank01Frame245,
    &gSpriteBank01Frame246,
    &gSpriteBank01Frame247,
    &gSpriteBank01Frame248,
    &gSpriteBank01Frame249,
    &gSpriteBank01Frame250,
    &gSpriteBank01Frame251,
    &gSpriteBank01Frame252,
    &gSpriteBank01Frame253.frame,
    &gSpriteBank01Frame254.frame,
    &gSpriteBank01Frame255.frame,
    &gSpriteBank01Frame256.frame,
    &gSpriteBank01Frame257.frame,
    &gSpriteBank01Frame258.frame,
    &gSpriteBank01Frame259.frame,
    &gSpriteBank01Frame260.frame,
    &gSpriteBank01Frame261.frame,
    &gSpriteBank01Frame262.frame,
    &gSpriteBank01Frame263.frame,
    &gSpriteBank01Frame264.frame,
    &gSpriteBank01Frame265.frame,
    &gSpriteBank01Frame266.frame,
    &gSpriteBank01Frame267.frame,
    &gSpriteBank01Frame268.frame,
    &gSpriteBank01Frame269.frame,
    &gSpriteBank01Frame270.frame,
    &gSpriteBank01Frame271.frame,
    &gSpriteBank01Frame272.frame,
    &gSpriteBank01Frame273.frame,
};

const struct sprite_frame_1box gSpriteBank01Frame000 = {
    SPRITE_FRAME(gSpriteBank01Frame000, SPRITE_TILES_BANK01_FRAME000),
    { { -4, -9, 13, 24 } },
};
const struct sprite_frame_1box gSpriteBank01Frame001 = {
    SPRITE_FRAME(gSpriteBank01Frame001, SPRITE_TILES_BANK01_FRAME001),
    { { -2, -12, 10, 27 } },
};
const struct sprite_frame_1box gSpriteBank01Frame002 = {
    SPRITE_FRAME(gSpriteBank01Frame002, SPRITE_TILES_BANK01_FRAME002),
    { { -4, -2, 16, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame003 = {
    SPRITE_FRAME(gSpriteBank01Frame003, SPRITE_TILES_BANK01_FRAME003),
    { { -4, -2, 16, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame004 = {
    SPRITE_FRAME(gSpriteBank01Frame004, SPRITE_TILES_BANK01_FRAME004),
    { { -4, -2, 16, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame005 = {
    SPRITE_FRAME(gSpriteBank01Frame005, SPRITE_TILES_BANK01_FRAME005),
    { { -4, -2, 16, 20 } },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame006 = {
    SPRITE_FRAME(gSpriteBank01Frame006, SPRITE_TILES_BANK01_FRAME006),
    { { 0, 0, 0, 0 }, { -13, -6, 31, 30 }, { -16, -12, 33, 35 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame007 = {
    SPRITE_FRAME(gSpriteBank01Frame007, SPRITE_TILES_BANK01_FRAME007),
    { { 0, 0, 0, 0 }, { -13, -6, 31, 30 }, { -15, -12, 33, 34 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame008 = {
    SPRITE_FRAME(gSpriteBank01Frame008, SPRITE_TILES_BANK01_FRAME008),
    { { 0, 0, 0, 0 }, { -13, -6, 31, 30 }, { -16, -15, 31, 37 } },
    { 0, 0 },
};
const struct sprite_frame_1box gSpriteBank01Frame009 = {
    SPRITE_FRAME(gSpriteBank01Frame009, SPRITE_TILES_BANK01_FRAME009),
    { { -6, -4, 11, 21 } },
};
const struct sprite_frame_1box gSpriteBank01Frame010 = {
    SPRITE_FRAME(gSpriteBank01Frame010, SPRITE_TILES_BANK01_FRAME010),
    { { -10, -2, 17, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame011 = {
    SPRITE_FRAME(gSpriteBank01Frame011, SPRITE_TILES_BANK01_FRAME011),
    { { -1, -3, 14, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame012 = {
    SPRITE_FRAME(gSpriteBank01Frame012, SPRITE_TILES_BANK01_FRAME012),
    { { -1, -3, 14, 20 } },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame013 = {
    SPRITE_FRAME(gSpriteBank01Frame013, SPRITE_TILES_BANK01_FRAME013),
    { { 0, 0, 0, 0 }, { -10, -9, 26, 25 }, { -12, -12, 31, 31 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame014 = {
    SPRITE_FRAME(gSpriteBank01Frame014, SPRITE_TILES_BANK01_FRAME014),
    { { 0, 0, 0, 0 }, { -8, -10, 26, 27 }, { -11, -10, 29, 28 } },
    { 0, 0 },
};
const struct sprite_frame_1box gSpriteBank01Frame015 = {
    SPRITE_FRAME(gSpriteBank01Frame015, SPRITE_TILES_BANK01_FRAME015),
    { { -3, 0, 17, 18 } },
};
const struct sprite_frame_1box gSpriteBank01Frame016 = {
    SPRITE_FRAME(gSpriteBank01Frame016, SPRITE_TILES_BANK01_FRAME016),
    { { 0, -4, 14, 22 } },
};
const struct sprite_frame_1box gSpriteBank01Frame017 = {
    SPRITE_FRAME(gSpriteBank01Frame017, SPRITE_TILES_BANK01_FRAME017),
    { { -1, -1, 13, 21 } },
};
const struct sprite_frame_1box gSpriteBank01Frame018 = {
    SPRITE_FRAME(gSpriteBank01Frame018, SPRITE_TILES_BANK01_FRAME018),
    { { -4, -3, 14, 22 } },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame019 = {
    SPRITE_FRAME(gSpriteBank01Frame019, SPRITE_TILES_BANK01_FRAME019),
    { { 0, 0, 0, 0 }, { -10, -5, 31, 25 }, { -10, -15, 30, 36 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame020 = {
    SPRITE_FRAME(gSpriteBank01Frame020, SPRITE_TILES_BANK01_FRAME020),
    { { 0, 0, 0, 0 }, { -15, -6, 27, 25 }, { -12, -10, 31, 32 } },
    { 0, 0 },
};
const struct sprite_frame_1box gSpriteBank01Frame021 = {
    SPRITE_FRAME(gSpriteBank01Frame021, SPRITE_TILES_BANK01_FRAME021),
    { { -2, -3, 18, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame022 = {
    SPRITE_FRAME(gSpriteBank01Frame022, SPRITE_TILES_BANK01_FRAME022),
    { { -2, -3, 18, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame023 = {
    SPRITE_FRAME(gSpriteBank01Frame023, SPRITE_TILES_BANK01_FRAME023),
    { { -2, -3, 18, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame024 = {
    SPRITE_FRAME(gSpriteBank01Frame024, SPRITE_TILES_BANK01_FRAME024),
    { { -2, -3, 18, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame025 = {
    SPRITE_FRAME(gSpriteBank01Frame025, SPRITE_TILES_BANK01_FRAME025),
    { { -2, -3, 18, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame026 = {
    SPRITE_FRAME(gSpriteBank01Frame026, SPRITE_TILES_BANK01_FRAME026),
    { { -2, -3, 18, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame027 = {
    SPRITE_FRAME(gSpriteBank01Frame027, SPRITE_TILES_BANK01_FRAME027),
    { { -2, -3, 18, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame028 = {
    SPRITE_FRAME(gSpriteBank01Frame028, SPRITE_TILES_BANK01_FRAME028),
    { { -2, -3, 18, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame029 = {
    SPRITE_FRAME(gSpriteBank01Frame029, SPRITE_TILES_BANK01_FRAME029),
    { { -1, -6, 18, 13 } },
};
const struct sprite_frame_1box gSpriteBank01Frame030 = {
    SPRITE_FRAME(gSpriteBank01Frame030, SPRITE_TILES_BANK01_FRAME030),
    { { -1, -6, 18, 13 } },
};
const struct sprite_frame_1box gSpriteBank01Frame031 = {
    SPRITE_FRAME(gSpriteBank01Frame031, SPRITE_TILES_BANK01_FRAME031),
    { { -1, -6, 18, 13 } },
};
const struct sprite_frame_1box gSpriteBank01Frame032 = {
    SPRITE_FRAME(gSpriteBank01Frame032, SPRITE_TILES_BANK01_FRAME032),
    { { -1, -6, 18, 13 } },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame033 = {
    SPRITE_FRAME(gSpriteBank01Frame033, SPRITE_TILES_BANK01_FRAME033),
    { { 0, 0, 0, 0 }, { -10, -12, 31, 33 }, { -11, -9, 31, 30 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame034 = {
    SPRITE_FRAME(gSpriteBank01Frame034, SPRITE_TILES_BANK01_FRAME034),
    { { 0, 0, 0, 0 }, { -10, -12, 31, 33 }, { -11, -9, 31, 30 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame035 = {
    SPRITE_FRAME(gSpriteBank01Frame035, SPRITE_TILES_BANK01_FRAME035),
    { { 0, 0, 0, 0 }, { -10, -12, 31, 33 }, { -11, -9, 31, 30 } },
    { 0, 0 },
};
const struct sprite_frame_1box gSpriteBank01Frame036 = {
    SPRITE_FRAME(gSpriteBank01Frame036, SPRITE_TILES_BANK01_FRAME036),
    { { -3, -7, 13, 19 } },
};
const struct sprite_frame_1box gSpriteBank01Frame037 = {
    SPRITE_FRAME(gSpriteBank01Frame037, SPRITE_TILES_BANK01_FRAME037),
    { { -3, -7, 13, 19 } },
};
const struct sprite_frame_1box gSpriteBank01Frame038 = {
    SPRITE_FRAME(gSpriteBank01Frame038, SPRITE_TILES_BANK01_FRAME038),
    { { -3, -7, 13, 19 } },
};
const struct sprite_frame_1box gSpriteBank01Frame039 = {
    SPRITE_FRAME(gSpriteBank01Frame039, SPRITE_TILES_BANK01_FRAME039),
    { { -3, -7, 13, 19 } },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame040 = {
    SPRITE_FRAME(gSpriteBank01Frame040, SPRITE_TILES_BANK01_FRAME040),
    { { 0, 0, 0, 0 }, { -8, -9, 24, 28 }, { -11, -10, 28, 30 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame041 = {
    SPRITE_FRAME(gSpriteBank01Frame041, SPRITE_TILES_BANK01_FRAME041),
    { { 0, 0, 0, 0 }, { -8, -9, 24, 28 }, { -11, -12, 29, 31 } },
    { 0, 0 },
};
const struct sprite_frame_1box gSpriteBank01Frame042 = {
    SPRITE_FRAME(gSpriteBank01Frame042, SPRITE_TILES_BANK01_FRAME042),
    { { 2, -7, 10, 16 } },
};
const struct sprite_frame_1box gSpriteBank01Frame043 = {
    SPRITE_FRAME(gSpriteBank01Frame043, SPRITE_TILES_BANK01_FRAME043),
    { { 2, -10, 9, 24 } },
};
const struct sprite_frame_1box gSpriteBank01Frame044 = {
    SPRITE_FRAME(gSpriteBank01Frame044, SPRITE_TILES_BANK01_FRAME044),
    { { -5, -7, 11, 17 } },
};
const struct sprite_frame_1box gSpriteBank01Frame045 = {
    SPRITE_FRAME(gSpriteBank01Frame045, SPRITE_TILES_BANK01_FRAME045),
    { { -5, -5, 12, 18 } },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame046 = {
    SPRITE_FRAME(gSpriteBank01Frame046, SPRITE_TILES_BANK01_FRAME046),
    { { 0, 0, 0, 0 }, { -10, -16, 31, 30 }, { -12, -12, 31, 34 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame047 = {
    SPRITE_FRAME(gSpriteBank01Frame047, SPRITE_TILES_BANK01_FRAME047),
    { { 0, 0, 0, 0 }, { -12, -12, 28, 28 }, { -13, -15, 34, 38 } },
    { 0, 0 },
};
const struct sprite_frame_1box gSpriteBank01Frame048 = {
    SPRITE_FRAME(gSpriteBank01Frame048, SPRITE_TILES_BANK01_FRAME048),
    { { -4, -6, 9, 24 } },
};
const struct sprite_frame_1box gSpriteBank01Frame049 = {
    SPRITE_FRAME(gSpriteBank01Frame049, SPRITE_TILES_BANK01_FRAME049),
    { { -4, -6, 9, 24 } },
};
const struct sprite_frame_1box gSpriteBank01Frame050 = {
    SPRITE_FRAME(gSpriteBank01Frame050, SPRITE_TILES_BANK01_FRAME050),
    { { -4, -6, 9, 24 } },
};
const struct sprite_frame_1box gSpriteBank01Frame051 = {
    SPRITE_FRAME(gSpriteBank01Frame051, SPRITE_TILES_BANK01_FRAME051),
    { { -4, -6, 9, 24 } },
};
const struct sprite_frame_1box gSpriteBank01Frame052 = {
    SPRITE_FRAME(gSpriteBank01Frame052, SPRITE_TILES_BANK01_FRAME052),
    { { -4, -6, 9, 24 } },
};
const struct sprite_frame_1box gSpriteBank01Frame053 = {
    SPRITE_FRAME(gSpriteBank01Frame053, SPRITE_TILES_BANK01_FRAME053),
    { { -4, -6, 9, 24 } },
};
const struct sprite_frame_1box gSpriteBank01Frame054 = {
    SPRITE_FRAME(gSpriteBank01Frame054, SPRITE_TILES_BANK01_FRAME054),
    { { -4, -6, 9, 24 } },
};
const struct sprite_frame_1box gSpriteBank01Frame055 = {
    SPRITE_FRAME(gSpriteBank01Frame055, SPRITE_TILES_BANK01_FRAME055),
    { { -4, -6, 9, 24 } },
};
const struct sprite_frame_1box gSpriteBank01Frame056 = {
    SPRITE_FRAME(gSpriteBank01Frame056, SPRITE_TILES_BANK01_FRAME056),
    { { -7, 1, 23, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame057 = {
    SPRITE_FRAME(gSpriteBank01Frame057, SPRITE_TILES_BANK01_FRAME057),
    { { -7, 1, 23, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame058 = {
    SPRITE_FRAME(gSpriteBank01Frame058, SPRITE_TILES_BANK01_FRAME058),
    { { -7, 1, 23, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame059 = {
    SPRITE_FRAME(gSpriteBank01Frame059, SPRITE_TILES_BANK01_FRAME059),
    { { -7, 1, 23, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame060 = {
    SPRITE_FRAME(gSpriteBank01Frame060, SPRITE_TILES_BANK01_FRAME060),
    { { -7, 1, 23, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame061 = {
    SPRITE_FRAME(gSpriteBank01Frame061, SPRITE_TILES_BANK01_FRAME061),
    { { -7, 1, 23, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame062 = {
    SPRITE_FRAME(gSpriteBank01Frame062, SPRITE_TILES_BANK01_FRAME062),
    { { -7, 1, 23, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame063 = {
    SPRITE_FRAME(gSpriteBank01Frame063, SPRITE_TILES_BANK01_FRAME063),
    { { -7, 1, 23, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame064 = {
    SPRITE_FRAME(gSpriteBank01Frame064, SPRITE_TILES_BANK01_FRAME064),
    { { -5, 6, 20, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame065 = {
    SPRITE_FRAME(gSpriteBank01Frame065, SPRITE_TILES_BANK01_FRAME065),
    { { -5, 6, 20, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame066 = {
    SPRITE_FRAME(gSpriteBank01Frame066, SPRITE_TILES_BANK01_FRAME066),
    { { -5, 6, 20, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame067 = {
    SPRITE_FRAME(gSpriteBank01Frame067, SPRITE_TILES_BANK01_FRAME067),
    { { -5, 6, 20, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame068 = {
    SPRITE_FRAME(gSpriteBank01Frame068, SPRITE_TILES_BANK01_FRAME068),
    { { -5, 6, 20, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame069 = {
    SPRITE_FRAME(gSpriteBank01Frame069, SPRITE_TILES_BANK01_FRAME069),
    { { -5, 6, 20, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame070 = {
    SPRITE_FRAME(gSpriteBank01Frame070, SPRITE_TILES_BANK01_FRAME070),
    { { -5, 6, 20, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame071 = {
    SPRITE_FRAME(gSpriteBank01Frame071, SPRITE_TILES_BANK01_FRAME071),
    { { -5, 6, 20, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame072 = {
    SPRITE_FRAME(gSpriteBank01Frame072, SPRITE_TILES_BANK01_FRAME072),
    { { 0, 2, 11, 17 } },
};
const struct sprite_frame_1box gSpriteBank01Frame073 = {
    SPRITE_FRAME(gSpriteBank01Frame073, SPRITE_TILES_BANK01_FRAME073),
    { { 0, 2, 11, 17 } },
};
const struct sprite_frame_1box gSpriteBank01Frame074 = {
    SPRITE_FRAME(gSpriteBank01Frame074, SPRITE_TILES_BANK01_FRAME074),
    { { 0, 2, 11, 17 } },
};
const struct sprite_frame_1box gSpriteBank01Frame075 = {
    SPRITE_FRAME(gSpriteBank01Frame075, SPRITE_TILES_BANK01_FRAME075),
    { { 0, 2, 11, 17 } },
};
const struct sprite_frame_1box gSpriteBank01Frame076 = {
    SPRITE_FRAME(gSpriteBank01Frame076, SPRITE_TILES_BANK01_FRAME076),
    { { 0, 2, 11, 17 } },
};
const struct sprite_frame_1box gSpriteBank01Frame077 = {
    SPRITE_FRAME(gSpriteBank01Frame077, SPRITE_TILES_BANK01_FRAME077),
    { { 0, 2, 11, 17 } },
};
const struct sprite_frame_1box gSpriteBank01Frame078 = {
    SPRITE_FRAME(gSpriteBank01Frame078, SPRITE_TILES_BANK01_FRAME078),
    { { 0, 2, 11, 17 } },
};
const struct sprite_frame_1box gSpriteBank01Frame079 = {
    SPRITE_FRAME(gSpriteBank01Frame079, SPRITE_TILES_BANK01_FRAME079),
    { { 0, 2, 11, 17 } },
};
const struct sprite_frame_1box gSpriteBank01Frame080 = {
    SPRITE_FRAME(gSpriteBank01Frame080, SPRITE_TILES_BANK01_FRAME080),
    { { 0, 0, 10, 18 } },
};
const struct sprite_frame_1box gSpriteBank01Frame081 = {
    SPRITE_FRAME(gSpriteBank01Frame081, SPRITE_TILES_BANK01_FRAME081),
    { { 0, 0, 10, 18 } },
};
const struct sprite_frame_1box gSpriteBank01Frame082 = {
    SPRITE_FRAME(gSpriteBank01Frame082, SPRITE_TILES_BANK01_FRAME082),
    { { 0, 0, 10, 18 } },
};
const struct sprite_frame_1box gSpriteBank01Frame083 = {
    SPRITE_FRAME(gSpriteBank01Frame083, SPRITE_TILES_BANK01_FRAME083),
    { { 0, 0, 10, 18 } },
};
const struct sprite_frame_1box gSpriteBank01Frame084 = {
    SPRITE_FRAME(gSpriteBank01Frame084, SPRITE_TILES_BANK01_FRAME084),
    { { 0, 0, 10, 18 } },
};
const struct sprite_frame_1box gSpriteBank01Frame085 = {
    SPRITE_FRAME(gSpriteBank01Frame085, SPRITE_TILES_BANK01_FRAME085),
    { { 0, 0, 10, 18 } },
};
const struct sprite_frame_1box gSpriteBank01Frame086 = {
    SPRITE_FRAME(gSpriteBank01Frame086, SPRITE_TILES_BANK01_FRAME086),
    { { 0, 0, 10, 18 } },
};
const struct sprite_frame_1box gSpriteBank01Frame087 = {
    SPRITE_FRAME(gSpriteBank01Frame087, SPRITE_TILES_BANK01_FRAME087),
    { { 0, 0, 10, 18 } },
};
const struct sprite_frame_1box gSpriteBank01Frame088 = {
    SPRITE_FRAME(gSpriteBank01Frame088, SPRITE_TILES_BANK01_FRAME088),
    { { -5, -7, 12, 23 } },
};
const struct sprite_frame_1box gSpriteBank01Frame089 = {
    SPRITE_FRAME(gSpriteBank01Frame089, SPRITE_TILES_BANK01_FRAME089),
    { { -5, -7, 12, 23 } },
};
const struct sprite_frame_1box gSpriteBank01Frame090 = {
    SPRITE_FRAME(gSpriteBank01Frame090, SPRITE_TILES_BANK01_FRAME090),
    { { -5, -7, 12, 23 } },
};
const struct sprite_frame_1box gSpriteBank01Frame091 = {
    SPRITE_FRAME(gSpriteBank01Frame091, SPRITE_TILES_BANK01_FRAME091),
    { { -5, -7, 12, 23 } },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame092 = {
    SPRITE_FRAME(gSpriteBank01Frame092, SPRITE_TILES_BANK01_FRAME092),
    { { 0, 0, 0, 0 }, { -10, -9, 32, 30 }, { -9, -19, 30, 41 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame093 = {
    SPRITE_FRAME(gSpriteBank01Frame093, SPRITE_TILES_BANK01_FRAME093),
    { { 0, 0, 0, 0 }, { -10, -9, 32, 30 }, { -10, -19, 31, 41 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame094 = {
    SPRITE_FRAME(gSpriteBank01Frame094, SPRITE_TILES_BANK01_FRAME094),
    { { 0, 0, 0, 0 }, { -10, -9, 32, 30 }, { -9, -19, 27, 41 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame095 = {
    SPRITE_FRAME(gSpriteBank01Frame095, SPRITE_TILES_BANK01_FRAME095),
    { { 0, 0, 0, 0 }, { -10, -9, 32, 30 }, { -8, -19, 30, 40 } },
    { 0, 0 },
};
const struct sprite_frame_1box gSpriteBank01Frame096 = {
    SPRITE_FRAME(gSpriteBank01Frame096, SPRITE_TILES_BANK01_FRAME096),
    { { -4, -4, 14, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame097 = {
    SPRITE_FRAME(gSpriteBank01Frame097, SPRITE_TILES_BANK01_FRAME097),
    { { -4, -4, 14, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame098 = {
    SPRITE_FRAME(gSpriteBank01Frame098, SPRITE_TILES_BANK01_FRAME098),
    { { -4, -8, 12, 24 } },
};
const struct sprite_frame_1box gSpriteBank01Frame099 = {
    SPRITE_FRAME(gSpriteBank01Frame099, SPRITE_TILES_BANK01_FRAME099),
    { { -4, -8, 12, 24 } },
};
const struct sprite_frame_1box gSpriteBank01Frame100 = {
    SPRITE_FRAME(gSpriteBank01Frame100, SPRITE_TILES_BANK01_FRAME100),
    { { -4, -1, 22, 9 } },
};
const struct sprite_frame_1box gSpriteBank01Frame101 = {
    SPRITE_FRAME(gSpriteBank01Frame101, SPRITE_TILES_BANK01_FRAME101),
    { { -4, -1, 22, 9 } },
};
const struct sprite_frame_1box gSpriteBank01Frame102 = {
    SPRITE_FRAME(gSpriteBank01Frame102, SPRITE_TILES_BANK01_FRAME102),
    { { -4, -1, 22, 9 } },
};
const struct sprite_frame_1box gSpriteBank01Frame103 = {
    SPRITE_FRAME(gSpriteBank01Frame103, SPRITE_TILES_BANK01_FRAME103),
    { { -4, -1, 22, 9 } },
};
const struct sprite_frame_1box gSpriteBank01Frame104 = {
    SPRITE_FRAME(gSpriteBank01Frame104, SPRITE_TILES_BANK01_FRAME104),
    { { -4, -1, 22, 9 } },
};
const struct sprite_frame_1box gSpriteBank01Frame105 = {
    SPRITE_FRAME(gSpriteBank01Frame105, SPRITE_TILES_BANK01_FRAME105),
    { { -4, -1, 22, 9 } },
};
const struct sprite_frame_1box gSpriteBank01Frame106 = {
    SPRITE_FRAME(gSpriteBank01Frame106, SPRITE_TILES_BANK01_FRAME106),
    { { -4, -1, 22, 9 } },
};
const struct sprite_frame_1box gSpriteBank01Frame107 = {
    SPRITE_FRAME(gSpriteBank01Frame107, SPRITE_TILES_BANK01_FRAME107),
    { { -4, -1, 22, 9 } },
};
const struct sprite_frame_1box gSpriteBank01Frame108 = {
    SPRITE_FRAME(gSpriteBank01Frame108, SPRITE_TILES_BANK01_FRAME108),
    { { -5, -1, 23, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame109 = {
    SPRITE_FRAME(gSpriteBank01Frame109, SPRITE_TILES_BANK01_FRAME109),
    { { -5, -1, 23, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame110 = {
    SPRITE_FRAME(gSpriteBank01Frame110, SPRITE_TILES_BANK01_FRAME110),
    { { -5, -1, 23, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame111 = {
    SPRITE_FRAME(gSpriteBank01Frame111, SPRITE_TILES_BANK01_FRAME111),
    { { -5, -1, 23, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame112 = {
    SPRITE_FRAME(gSpriteBank01Frame112, SPRITE_TILES_BANK01_FRAME112),
    { { -17, -2, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame113 = {
    SPRITE_FRAME(gSpriteBank01Frame113, SPRITE_TILES_BANK01_FRAME113),
    { { -17, -2, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame114 = {
    SPRITE_FRAME(gSpriteBank01Frame114, SPRITE_TILES_BANK01_FRAME114),
    { { -7, -5, 20, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame115 = {
    SPRITE_FRAME(gSpriteBank01Frame115, SPRITE_TILES_BANK01_FRAME115),
    { { -4, -5, 18, 9 } },
};
const struct sprite_frame_1box gSpriteBank01Frame116 = {
    SPRITE_FRAME(gSpriteBank01Frame116, SPRITE_TILES_BANK01_FRAME116),
    { { -4, -7, 21, 9 } },
};
const struct sprite_frame_1box gSpriteBank01Frame117 = {
    SPRITE_FRAME(gSpriteBank01Frame117, SPRITE_TILES_BANK01_FRAME117),
    { { -3, -3, 23, 10 } },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame118 = {
    SPRITE_FRAME(gSpriteBank01Frame118, SPRITE_TILES_BANK01_FRAME118),
    { { 0, 0, 0, 0 }, { -18, -10, 35, 25 }, { -17, -10, 38, 27 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame119 = {
    SPRITE_FRAME(gSpriteBank01Frame119, SPRITE_TILES_BANK01_FRAME119),
    { { 0, 0, 0, 0 }, { -16, -11, 32, 29 }, { -17, -12, 34, 26 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame120 = {
    SPRITE_FRAME(gSpriteBank01Frame120, SPRITE_TILES_BANK01_FRAME120),
    { { 0, 0, 0, 0 }, { -15, -15, 33, 31 }, { -13, -13, 29, 28 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame121 = {
    SPRITE_FRAME(gSpriteBank01Frame121, SPRITE_TILES_BANK01_FRAME121),
    { { 0, 0, 0, 0 }, { -13, -10, 32, 28 }, { -14, -8, 33, 25 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame122 = {
    SPRITE_FRAME(gSpriteBank01Frame122, SPRITE_TILES_BANK01_FRAME122),
    { { 0, 0, 0, 0 }, { -12, -12, 31, 29 }, { -13, -12, 33, 31 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame123 = {
    SPRITE_FRAME(gSpriteBank01Frame123, SPRITE_TILES_BANK01_FRAME123),
    { { 0, 0, 0, 0 }, { -11, -13, 32, 31 }, { -17, -12, 40, 31 } },
    { 0, 0 },
};
const struct sprite_frame_1box gSpriteBank01Frame124 = {
    SPRITE_FRAME(gSpriteBank01Frame124, SPRITE_TILES_BANK01_FRAME124),
    { { -25, -10, 49, 17 } },
};
const struct sprite_frame_1box gSpriteBank01Frame125 = {
    SPRITE_FRAME(gSpriteBank01Frame125, SPRITE_TILES_BANK01_FRAME125),
    { { -20, -17, 40, 27 } },
};
const struct sprite_frame_1box gSpriteBank01Frame126 = {
    SPRITE_FRAME(gSpriteBank01Frame126, SPRITE_TILES_BANK01_FRAME126),
    { { -16, -18, 32, 31 } },
};
const struct sprite_frame_1box gSpriteBank01Frame127 = {
    SPRITE_FRAME(gSpriteBank01Frame127, SPRITE_TILES_BANK01_FRAME127),
    { { -14, -14, 30, 30 } },
};
const struct sprite_frame_1box gSpriteBank01Frame128 = {
    SPRITE_FRAME(gSpriteBank01Frame128, SPRITE_TILES_BANK01_FRAME128),
    { { -14, -11, 28, 27 } },
};
const struct sprite_frame_1box gSpriteBank01Frame129 = {
    SPRITE_FRAME(gSpriteBank01Frame129, SPRITE_TILES_BANK01_FRAME129),
    { { -19, -11, 35, 28 } },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame130 = {
    SPRITE_FRAME(gSpriteBank01Frame130, SPRITE_TILES_BANK01_FRAME130),
    { { 0, 0, 0, 0 }, { -7, -12, 29, 28 }, { -22, -10, 46, 26 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame131 = {
    SPRITE_FRAME(gSpriteBank01Frame131, SPRITE_TILES_BANK01_FRAME131),
    { { 0, 0, 0, 0 }, { -13, -10, 31, 24 }, { -22, -10, 46, 26 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame132 = {
    SPRITE_FRAME(gSpriteBank01Frame132, SPRITE_TILES_BANK01_FRAME132),
    { { 0, 0, 0, 0 }, { -10, -9, 28, 26 }, { -12, -10, 33, 28 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame133 = {
    SPRITE_FRAME(gSpriteBank01Frame133, SPRITE_TILES_BANK01_FRAME133),
    { { 0, 0, 0, 0 }, { -11, -9, 29, 25 }, { -13, -9, 38, 24 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame134 = {
    SPRITE_FRAME(gSpriteBank01Frame134, SPRITE_TILES_BANK01_FRAME134),
    { { 0, 0, 0, 0 }, { -14, -11, 31, 25 }, { -16, -10, 38, 26 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame135 = {
    SPRITE_FRAME(gSpriteBank01Frame135, SPRITE_TILES_BANK01_FRAME135),
    { { 0, 0, 0, 0 }, { -14, -9, 33, 28 }, { -17, -9, 44, 27 } },
    { 0, 0 },
};
const struct sprite_frame_1box gSpriteBank01Frame136 = {
    SPRITE_FRAME(gSpriteBank01Frame136, SPRITE_TILES_BANK01_FRAME136),
    { { -3, 0, 19, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame137 = {
    SPRITE_FRAME(gSpriteBank01Frame137, SPRITE_TILES_BANK01_FRAME137),
    { { -3, 0, 19, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame138 = {
    SPRITE_FRAME(gSpriteBank01Frame138, SPRITE_TILES_BANK01_FRAME138),
    { { -3, 0, 19, 6 } },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame139 = {
    SPRITE_FRAME(gSpriteBank01Frame139, SPRITE_TILES_BANK01_FRAME139),
    { { 0, 0, 0, 0 }, { -9, -9, 33, 28 }, { -19, -9, 41, 27 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame140 = {
    SPRITE_FRAME(gSpriteBank01Frame140, SPRITE_TILES_BANK01_FRAME140),
    { { 0, 0, 0, 0 }, { -8, -11, 29, 28 }, { -19, -11, 41, 26 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame141 = {
    SPRITE_FRAME(gSpriteBank01Frame141, SPRITE_TILES_BANK01_FRAME141),
    { { 0, 0, 0, 0 }, { -10, -8, 32, 26 }, { -22, -8, 43, 25 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame142 = {
    SPRITE_FRAME(gSpriteBank01Frame142, SPRITE_TILES_BANK01_FRAME142),
    { { 0, 0, 0, 0 }, { -7, -11, 29, 30 }, { -18, -10, 40, 28 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame143 = {
    SPRITE_FRAME(gSpriteBank01Frame143, SPRITE_TILES_BANK01_FRAME143),
    { { 0, 0, 0, 0 }, { -9, -10, 29, 29 }, { -19, -9, 41, 27 } },
    { 0, 0 },
};
const struct sprite_frame_1box gSpriteBank01Frame144 = {
    SPRITE_FRAME(gSpriteBank01Frame144, SPRITE_TILES_BANK01_FRAME144),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame145 = {
    SPRITE_FRAME(gSpriteBank01Frame145, SPRITE_TILES_BANK01_FRAME145),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame146 = {
    SPRITE_FRAME(gSpriteBank01Frame146, SPRITE_TILES_BANK01_FRAME146),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame147 = {
    SPRITE_FRAME(gSpriteBank01Frame147, SPRITE_TILES_BANK01_FRAME147),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame148 = {
    SPRITE_FRAME(gSpriteBank01Frame148, SPRITE_TILES_BANK01_FRAME148),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame149 = {
    SPRITE_FRAME(gSpriteBank01Frame149, SPRITE_TILES_BANK01_FRAME149),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame150 = {
    SPRITE_FRAME(gSpriteBank01Frame150, SPRITE_TILES_BANK01_FRAME150),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame151 = {
    SPRITE_FRAME(gSpriteBank01Frame151, SPRITE_TILES_BANK01_FRAME151),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame152 = {
    SPRITE_FRAME(gSpriteBank01Frame152, SPRITE_TILES_BANK01_FRAME152),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame153 = {
    SPRITE_FRAME(gSpriteBank01Frame153, SPRITE_TILES_BANK01_FRAME153),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame154 = {
    SPRITE_FRAME(gSpriteBank01Frame154, SPRITE_TILES_BANK01_FRAME154),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame155 = {
    SPRITE_FRAME(gSpriteBank01Frame155, SPRITE_TILES_BANK01_FRAME155),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame156 = {
    SPRITE_FRAME(gSpriteBank01Frame156, SPRITE_TILES_BANK01_FRAME156),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame157 = {
    SPRITE_FRAME(gSpriteBank01Frame157, SPRITE_TILES_BANK01_FRAME157),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame158 = {
    SPRITE_FRAME(gSpriteBank01Frame158, SPRITE_TILES_BANK01_FRAME158),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame159 = {
    SPRITE_FRAME(gSpriteBank01Frame159, SPRITE_TILES_BANK01_FRAME159),
    { { -6, 0, 24, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame160 = {
    SPRITE_FRAME(gSpriteBank01Frame160, SPRITE_TILES_BANK01_FRAME160),
    { { -6, 0, 24, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame161 = {
    SPRITE_FRAME(gSpriteBank01Frame161, SPRITE_TILES_BANK01_FRAME161),
    { { -6, 0, 24, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame162 = {
    SPRITE_FRAME(gSpriteBank01Frame162, SPRITE_TILES_BANK01_FRAME162),
    { { -6, 0, 24, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame163 = {
    SPRITE_FRAME(gSpriteBank01Frame163, SPRITE_TILES_BANK01_FRAME163),
    { { -6, 0, 24, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame164 = {
    SPRITE_FRAME(gSpriteBank01Frame164, SPRITE_TILES_BANK01_FRAME164),
    { { -6, 0, 24, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame165 = {
    SPRITE_FRAME(gSpriteBank01Frame165, SPRITE_TILES_BANK01_FRAME165),
    { { -6, 0, 24, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame166 = {
    SPRITE_FRAME(gSpriteBank01Frame166, SPRITE_TILES_BANK01_FRAME166),
    { { -6, 0, 24, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame167 = {
    SPRITE_FRAME(gSpriteBank01Frame167, SPRITE_TILES_BANK01_FRAME167),
    { { 0, -10, 9, 26 } },
};
const struct sprite_frame_1box gSpriteBank01Frame168 = {
    SPRITE_FRAME(gSpriteBank01Frame168, SPRITE_TILES_BANK01_FRAME168),
    { { 0, -10, 9, 26 } },
};
const struct sprite_frame_1box gSpriteBank01Frame169 = {
    SPRITE_FRAME(gSpriteBank01Frame169, SPRITE_TILES_BANK01_FRAME169),
    { { 0, -10, 9, 26 } },
};
const struct sprite_frame_1box gSpriteBank01Frame170 = {
    SPRITE_FRAME(gSpriteBank01Frame170, SPRITE_TILES_BANK01_FRAME170),
    { { 0, -10, 9, 26 } },
};
const struct sprite_frame_1box gSpriteBank01Frame171 = {
    SPRITE_FRAME(gSpriteBank01Frame171, SPRITE_TILES_BANK01_FRAME171),
    { { 0, -10, 9, 26 } },
};
const struct sprite_frame_1box gSpriteBank01Frame172 = {
    SPRITE_FRAME(gSpriteBank01Frame172, SPRITE_TILES_BANK01_FRAME172),
    { { 0, -10, 9, 26 } },
};
const struct sprite_frame_1box gSpriteBank01Frame173 = {
    SPRITE_FRAME(gSpriteBank01Frame173, SPRITE_TILES_BANK01_FRAME173),
    { { 0, -10, 9, 26 } },
};
const struct sprite_frame_1box gSpriteBank01Frame174 = {
    SPRITE_FRAME(gSpriteBank01Frame174, SPRITE_TILES_BANK01_FRAME174),
    { { 0, -10, 9, 26 } },
};
const struct sprite_frame_1box gSpriteBank01Frame175 = {
    SPRITE_FRAME(gSpriteBank01Frame175, SPRITE_TILES_BANK01_FRAME175),
    { { -1, -3, 24, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame176 = {
    SPRITE_FRAME(gSpriteBank01Frame176, SPRITE_TILES_BANK01_FRAME176),
    { { -1, -3, 24, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame177 = {
    SPRITE_FRAME(gSpriteBank01Frame177, SPRITE_TILES_BANK01_FRAME177),
    { { -1, -3, 24, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame178 = {
    SPRITE_FRAME(gSpriteBank01Frame178, SPRITE_TILES_BANK01_FRAME178),
    { { -1, -3, 24, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame179 = {
    SPRITE_FRAME(gSpriteBank01Frame179, SPRITE_TILES_BANK01_FRAME179),
    { { -1, -3, 24, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame180 = {
    SPRITE_FRAME(gSpriteBank01Frame180, SPRITE_TILES_BANK01_FRAME180),
    { { -1, -3, 24, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame181 = {
    SPRITE_FRAME(gSpriteBank01Frame181, SPRITE_TILES_BANK01_FRAME181),
    { { -1, -3, 24, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame182 = {
    SPRITE_FRAME(gSpriteBank01Frame182, SPRITE_TILES_BANK01_FRAME182),
    { { -1, -3, 24, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame183 = {
    SPRITE_FRAME(gSpriteBank01Frame183, SPRITE_TILES_BANK01_FRAME183),
    { { -2, -3, 21, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame184 = {
    SPRITE_FRAME(gSpriteBank01Frame184, SPRITE_TILES_BANK01_FRAME184),
    { { -2, -3, 21, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame185 = {
    SPRITE_FRAME(gSpriteBank01Frame185, SPRITE_TILES_BANK01_FRAME185),
    { { -2, -3, 21, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame186 = {
    SPRITE_FRAME(gSpriteBank01Frame186, SPRITE_TILES_BANK01_FRAME186),
    { { -2, -3, 21, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame187 = {
    SPRITE_FRAME(gSpriteBank01Frame187, SPRITE_TILES_BANK01_FRAME187),
    { { -2, -3, 21, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame188 = {
    SPRITE_FRAME(gSpriteBank01Frame188, SPRITE_TILES_BANK01_FRAME188),
    { { -2, -3, 21, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame189 = {
    SPRITE_FRAME(gSpriteBank01Frame189, SPRITE_TILES_BANK01_FRAME189),
    { { -2, -3, 21, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame190 = {
    SPRITE_FRAME(gSpriteBank01Frame190, SPRITE_TILES_BANK01_FRAME190),
    { { -2, -3, 21, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame191 = {
    SPRITE_FRAME(gSpriteBank01Frame191, SPRITE_TILES_BANK01_FRAME191),
    { { 2, -9, 9, 19 } },
};
const struct sprite_frame_1box gSpriteBank01Frame192 = {
    SPRITE_FRAME(gSpriteBank01Frame192, SPRITE_TILES_BANK01_FRAME192),
    { { 2, -9, 9, 19 } },
};
const struct sprite_frame_1box gSpriteBank01Frame193 = {
    SPRITE_FRAME(gSpriteBank01Frame193, SPRITE_TILES_BANK01_FRAME193),
    { { 2, -9, 9, 19 } },
};
const struct sprite_frame_1box gSpriteBank01Frame194 = {
    SPRITE_FRAME(gSpriteBank01Frame194, SPRITE_TILES_BANK01_FRAME194),
    { { 2, -9, 9, 19 } },
};
const struct sprite_frame_1box gSpriteBank01Frame195 = {
    SPRITE_FRAME(gSpriteBank01Frame195, SPRITE_TILES_BANK01_FRAME195),
    { { 2, -9, 9, 19 } },
};
const struct sprite_frame_1box gSpriteBank01Frame196 = {
    SPRITE_FRAME(gSpriteBank01Frame196, SPRITE_TILES_BANK01_FRAME196),
    { { 2, -9, 9, 19 } },
};
const struct sprite_frame_1box gSpriteBank01Frame197 = {
    SPRITE_FRAME(gSpriteBank01Frame197, SPRITE_TILES_BANK01_FRAME197),
    { { 2, -9, 9, 19 } },
};
const struct sprite_frame_1box gSpriteBank01Frame198 = {
    SPRITE_FRAME(gSpriteBank01Frame198, SPRITE_TILES_BANK01_FRAME198),
    { { 2, -9, 9, 19 } },
};
const struct sprite_frame_1box gSpriteBank01Frame199 = {
    SPRITE_FRAME(gSpriteBank01Frame199, SPRITE_TILES_BANK01_FRAME199),
    { { 0, -10, 10, 22 } },
};
const struct sprite_frame_1box gSpriteBank01Frame200 = {
    SPRITE_FRAME(gSpriteBank01Frame200, SPRITE_TILES_BANK01_FRAME200),
    { { 0, -10, 10, 22 } },
};
const struct sprite_frame_1box gSpriteBank01Frame201 = {
    SPRITE_FRAME(gSpriteBank01Frame201, SPRITE_TILES_BANK01_FRAME201),
    { { 0, -10, 10, 22 } },
};
const struct sprite_frame_1box gSpriteBank01Frame202 = {
    SPRITE_FRAME(gSpriteBank01Frame202, SPRITE_TILES_BANK01_FRAME202),
    { { 0, -10, 10, 22 } },
};
const struct sprite_frame_1box gSpriteBank01Frame203 = {
    SPRITE_FRAME(gSpriteBank01Frame203, SPRITE_TILES_BANK01_FRAME203),
    { { 0, -10, 10, 22 } },
};
const struct sprite_frame_1box gSpriteBank01Frame204 = {
    SPRITE_FRAME(gSpriteBank01Frame204, SPRITE_TILES_BANK01_FRAME204),
    { { 0, -10, 10, 22 } },
};
const struct sprite_frame_1box gSpriteBank01Frame205 = {
    SPRITE_FRAME(gSpriteBank01Frame205, SPRITE_TILES_BANK01_FRAME205),
    { { 0, -10, 10, 22 } },
};
const struct sprite_frame_1box gSpriteBank01Frame206 = {
    SPRITE_FRAME(gSpriteBank01Frame206, SPRITE_TILES_BANK01_FRAME206),
    { { 0, -10, 10, 22 } },
};
const struct sprite_frame_1box gSpriteBank01Frame207 = {
    SPRITE_FRAME(gSpriteBank01Frame207, SPRITE_TILES_BANK01_FRAME207),
    { { -1, -4, 11, 19 } },
};
const struct sprite_frame_1box gSpriteBank01Frame208 = {
    SPRITE_FRAME(gSpriteBank01Frame208, SPRITE_TILES_BANK01_FRAME208),
    { { -1, -4, 11, 19 } },
};
const struct sprite_frame_1box gSpriteBank01Frame209 = {
    SPRITE_FRAME(gSpriteBank01Frame209, SPRITE_TILES_BANK01_FRAME209),
    { { -1, -4, 11, 19 } },
};
const struct sprite_frame_1box gSpriteBank01Frame210 = {
    SPRITE_FRAME(gSpriteBank01Frame210, SPRITE_TILES_BANK01_FRAME210),
    { { -1, -4, 11, 19 } },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame211 = {
    SPRITE_FRAME(gSpriteBank01Frame211, SPRITE_TILES_BANK01_FRAME211),
    { { 0, 0, 0, 0 }, { -7, -14, 28, 34 }, { -7, -13, 26, 37 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame212 = {
    SPRITE_FRAME(gSpriteBank01Frame212, SPRITE_TILES_BANK01_FRAME212),
    { { 0, 0, 0, 0 }, { -8, -14, 28, 37 }, { -8, -13, 27, 37 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame213 = {
    SPRITE_FRAME(gSpriteBank01Frame213, SPRITE_TILES_BANK01_FRAME213),
    { { 0, 0, 0, 0 }, { -7, -14, 24, 34 }, { -7, -13, 23, 37 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame214 = {
    SPRITE_FRAME(gSpriteBank01Frame214, SPRITE_TILES_BANK01_FRAME214),
    { { 0, 0, 0, 0 }, { -7, -13, 29, 32 }, { -6, -12, 26, 36 } },
    { 0, 0 },
};
const struct sprite_frame_1box gSpriteBank01Frame215 = {
    SPRITE_FRAME(gSpriteBank01Frame215, SPRITE_TILES_BANK01_FRAME215),
    { { -2, -10, 10, 25 } },
};
const struct sprite_frame_1box gSpriteBank01Frame216 = {
    SPRITE_FRAME(gSpriteBank01Frame216, SPRITE_TILES_BANK01_FRAME216),
    { { -2, -10, 10, 25 } },
};
const struct sprite_frame_1box gSpriteBank01Frame217 = {
    SPRITE_FRAME(gSpriteBank01Frame217, SPRITE_TILES_BANK01_FRAME217),
    { { 1, -1, 9, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame218 = {
    SPRITE_FRAME(gSpriteBank01Frame218, SPRITE_TILES_BANK01_FRAME218),
    { { 1, -1, 9, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame219 = {
    SPRITE_FRAME(gSpriteBank01Frame219, SPRITE_TILES_BANK01_FRAME219),
    { { 1, -1, 9, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame220 = {
    SPRITE_FRAME(gSpriteBank01Frame220, SPRITE_TILES_BANK01_FRAME220),
    { { 1, -1, 9, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame221 = {
    SPRITE_FRAME(gSpriteBank01Frame221, SPRITE_TILES_BANK01_FRAME221),
    { { 1, -1, 9, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame222 = {
    SPRITE_FRAME(gSpriteBank01Frame222, SPRITE_TILES_BANK01_FRAME222),
    { { 1, -1, 9, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame223 = {
    SPRITE_FRAME(gSpriteBank01Frame223, SPRITE_TILES_BANK01_FRAME223),
    { { 1, -1, 9, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame224 = {
    SPRITE_FRAME(gSpriteBank01Frame224, SPRITE_TILES_BANK01_FRAME224),
    { { 1, -1, 9, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame225 = {
    SPRITE_FRAME(gSpriteBank01Frame225, SPRITE_TILES_BANK01_FRAME225),
    { { -6, -2, 13, 4 } },
};
const struct sprite_frame gSpriteBank01Frame226 = SPRITE_FRAME(gSpriteBank01Frame226, SPRITE_TILES_BANK01_FRAME226);
const struct sprite_frame gSpriteBank01Frame227 = SPRITE_FRAME(gSpriteBank01Frame227, SPRITE_TILES_BANK01_FRAME227);
const struct sprite_frame gSpriteBank01Frame228 = SPRITE_FRAME(gSpriteBank01Frame228, SPRITE_TILES_BANK01_FRAME228);
const struct sprite_frame gSpriteBank01Frame229 = SPRITE_FRAME(gSpriteBank01Frame229, SPRITE_TILES_BANK01_FRAME229);
const struct sprite_frame gSpriteBank01Frame230 = SPRITE_FRAME(gSpriteBank01Frame230, SPRITE_TILES_BANK01_FRAME230);
const struct sprite_frame gSpriteBank01Frame231 = SPRITE_FRAME(gSpriteBank01Frame231, SPRITE_TILES_BANK01_FRAME231);
const struct sprite_frame gSpriteBank01Frame232 = SPRITE_FRAME(gSpriteBank01Frame232, SPRITE_TILES_BANK01_FRAME232);
const struct sprite_frame gSpriteBank01Frame233 = SPRITE_FRAME(gSpriteBank01Frame233, SPRITE_TILES_BANK01_FRAME233);
const struct sprite_frame gSpriteBank01Frame234 = SPRITE_FRAME(gSpriteBank01Frame234, SPRITE_TILES_BANK01_FRAME234);
const struct sprite_frame gSpriteBank01Frame235 = SPRITE_FRAME(gSpriteBank01Frame235, SPRITE_TILES_BANK01_FRAME235);
const struct sprite_frame gSpriteBank01Frame236 = SPRITE_FRAME(gSpriteBank01Frame236, SPRITE_TILES_BANK01_FRAME236);
const struct sprite_frame gSpriteBank01Frame237 = SPRITE_FRAME(gSpriteBank01Frame237, SPRITE_TILES_BANK01_FRAME237);
const struct sprite_frame gSpriteBank01Frame238 = SPRITE_FRAME(gSpriteBank01Frame238, SPRITE_TILES_BANK01_FRAME238);
const struct sprite_frame gSpriteBank01Frame239 = SPRITE_FRAME(gSpriteBank01Frame239, SPRITE_TILES_BANK01_FRAME239);
const struct sprite_frame gSpriteBank01Frame240 = SPRITE_FRAME(gSpriteBank01Frame240, SPRITE_TILES_BANK01_FRAME240);
const struct sprite_frame gSpriteBank01Frame241 = SPRITE_FRAME(gSpriteBank01Frame241, SPRITE_TILES_BANK01_FRAME241);
const struct sprite_frame gSpriteBank01Frame242 = SPRITE_FRAME(gSpriteBank01Frame242, SPRITE_TILES_BANK01_FRAME242);
const struct sprite_frame gSpriteBank01Frame243 = SPRITE_FRAME(gSpriteBank01Frame243, SPRITE_TILES_BANK01_FRAME243);
const struct sprite_frame gSpriteBank01Frame244 = SPRITE_FRAME(gSpriteBank01Frame244, SPRITE_TILES_BANK01_FRAME244);
const struct sprite_frame gSpriteBank01Frame245 = SPRITE_FRAME(gSpriteBank01Frame245, SPRITE_TILES_BANK01_FRAME245);
const struct sprite_frame gSpriteBank01Frame246 = SPRITE_FRAME(gSpriteBank01Frame246, SPRITE_TILES_BANK01_FRAME246);
const struct sprite_frame gSpriteBank01Frame247 = SPRITE_FRAME(gSpriteBank01Frame247, SPRITE_TILES_BANK01_FRAME247);
const struct sprite_frame gSpriteBank01Frame248 = SPRITE_FRAME(gSpriteBank01Frame248, SPRITE_TILES_BANK01_FRAME248);
const struct sprite_frame gSpriteBank01Frame249 = SPRITE_FRAME(gSpriteBank01Frame249, SPRITE_TILES_BANK01_FRAME249);
const struct sprite_frame gSpriteBank01Frame250 = SPRITE_FRAME(gSpriteBank01Frame250, SPRITE_TILES_BANK01_FRAME250);
const struct sprite_frame gSpriteBank01Frame251 = SPRITE_FRAME(gSpriteBank01Frame251, SPRITE_TILES_BANK01_FRAME251);
const struct sprite_frame gSpriteBank01Frame252 = SPRITE_FRAME(gSpriteBank01Frame252, SPRITE_TILES_BANK01_FRAME252);
const struct sprite_frame_1box gSpriteBank01Frame253 = {
    SPRITE_FRAME(gSpriteBank01Frame253, SPRITE_TILES_BANK01_FRAME253),
    { { -22, -17, 45, 34 } },
};
const struct sprite_frame_1box gSpriteBank01Frame254 = {
    SPRITE_FRAME(gSpriteBank01Frame254, SPRITE_TILES_BANK01_FRAME254),
    { { -20, -16, 42, 33 } },
};
const struct sprite_frame_1box gSpriteBank01Frame255 = {
    SPRITE_FRAME(gSpriteBank01Frame255, SPRITE_TILES_BANK01_FRAME255),
    { { -21, -17, 43, 32 } },
};
const struct sprite_frame_1box gSpriteBank01Frame256 = {
    SPRITE_FRAME(gSpriteBank01Frame256, SPRITE_TILES_BANK01_FRAME256),
    { { -21, -17, 43, 32 } },
};
const struct sprite_frame_1box gSpriteBank01Frame257 = {
    SPRITE_FRAME(gSpriteBank01Frame257, SPRITE_TILES_BANK01_FRAME257),
    { { -21, -10, 43, 33 } },
};
const struct sprite_frame_1box gSpriteBank01Frame258 = {
    SPRITE_FRAME(gSpriteBank01Frame258, SPRITE_TILES_BANK01_FRAME258),
    { { -22, -10, 45, 34 } },
};
const struct sprite_frame_1box gSpriteBank01Frame259 = {
    SPRITE_FRAME(gSpriteBank01Frame259, SPRITE_TILES_BANK01_FRAME259),
    { { -22, -10, 45, 32 } },
};
const struct sprite_frame_1box gSpriteBank01Frame260 = {
    SPRITE_FRAME(gSpriteBank01Frame260, SPRITE_TILES_BANK01_FRAME260),
    { { -22, -10, 46, 32 } },
};
const struct sprite_frame_1box gSpriteBank01Frame261 = {
    SPRITE_FRAME(gSpriteBank01Frame261, SPRITE_TILES_BANK01_FRAME261),
    { { -22, -12, 47, 32 } },
};
const struct sprite_frame_1box gSpriteBank01Frame262 = {
    SPRITE_FRAME(gSpriteBank01Frame262, SPRITE_TILES_BANK01_FRAME262),
    { { -22, -14, 47, 34 } },
};
const struct sprite_frame_1box gSpriteBank01Frame263 = {
    SPRITE_FRAME(gSpriteBank01Frame263, SPRITE_TILES_BANK01_FRAME263),
    { { -22, -16, 47, 35 } },
};
const struct sprite_frame_1box gSpriteBank01Frame264 = {
    SPRITE_FRAME(gSpriteBank01Frame264, SPRITE_TILES_BANK01_FRAME264),
    { { -21, -15, 46, 34 } },
};
const struct sprite_frame_1box gSpriteBank01Frame265 = {
    SPRITE_FRAME(gSpriteBank01Frame265, SPRITE_TILES_BANK01_FRAME265),
    { { -21, -15, 46, 33 } },
};
const struct sprite_frame_1box gSpriteBank01Frame266 = {
    SPRITE_FRAME(gSpriteBank01Frame266, SPRITE_TILES_BANK01_FRAME266),
    { { -20, -15, 44, 33 } },
};
const struct sprite_frame_1box gSpriteBank01Frame267 = {
    SPRITE_FRAME(gSpriteBank01Frame267, SPRITE_TILES_BANK01_FRAME267),
    { { -19, -9, 43, 27 } },
};
const struct sprite_frame_1box gSpriteBank01Frame268 = {
    SPRITE_FRAME(gSpriteBank01Frame268, SPRITE_TILES_BANK01_FRAME268),
    { { -19, -9, 43, 28 } },
};
const struct sprite_frame_1box gSpriteBank01Frame269 = {
    SPRITE_FRAME(gSpriteBank01Frame269, SPRITE_TILES_BANK01_FRAME269),
    { { -18, -9, 42, 29 } },
};
const struct sprite_frame_1box gSpriteBank01Frame270 = {
    SPRITE_FRAME(gSpriteBank01Frame270, SPRITE_TILES_BANK01_FRAME270),
    { { -18, -10, 42, 29 } },
};
const struct sprite_frame_1box gSpriteBank01Frame271 = {
    SPRITE_FRAME(gSpriteBank01Frame271, SPRITE_TILES_BANK01_FRAME271),
    { { -18, -10, 42, 29 } },
};
const struct sprite_frame_1box gSpriteBank01Frame272 = {
    SPRITE_FRAME(gSpriteBank01Frame272, SPRITE_TILES_BANK01_FRAME272),
    { { -18, -13, 41, 32 } },
};
const struct sprite_frame_1box gSpriteBank01Frame273 = {
    SPRITE_FRAME(gSpriteBank01Frame273, SPRITE_TILES_BANK01_FRAME273),
    { { -21, -15, 44, 33 } },
};

/* The piece arrays, from graphics/sprites/bank01_crash_underwater/bank.json. */
#include "sprites/bank01_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 2: 2 animations, 17 frames, graphics/sprites/bank02_crash_hover/. */

extern const u16 gSpriteBank02Anim00Seq[6];
extern const u16 gSpriteBank02Anim01Seq[11];

const struct sprite_anim gSpriteBank02Anims[2] = {
    /* 0 */ {
        /* seq */ gSpriteBank02Anim00Seq,
        /* box */ { { -22, -20, 45, 40 }, { -23, -21, 47, 42 } },
        /* paletteId */ 8,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank02Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank02Anim01Seq,
        /* box */ { { -22, -13, 44, 26 }, { -51, -60, 106, 107 } },
        /* paletteId */ 120,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank02Anim01Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank02Anim00Seq[6] = {
    0, 1, 2, 3, 4, 5,
};
const u16 gSpriteBank02Anim01Seq[11] = {
    6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
};

const struct sprite_frame *const gSpriteBank02Frames[17] = {
    &gSpriteBank02Frame000.frame,
    &gSpriteBank02Frame001.frame,
    &gSpriteBank02Frame002.frame,
    &gSpriteBank02Frame003.frame,
    &gSpriteBank02Frame004.frame,
    &gSpriteBank02Frame005.frame,
    &gSpriteBank02Frame006.frame,
    &gSpriteBank02Frame007.frame,
    &gSpriteBank02Frame008.frame,
    &gSpriteBank02Frame009.frame,
    &gSpriteBank02Frame010.frame,
    &gSpriteBank02Frame011.frame,
    &gSpriteBank02Frame012.frame,
    &gSpriteBank02Frame013.frame,
    &gSpriteBank02Frame014.frame,
    &gSpriteBank02Frame015.frame,
    &gSpriteBank02Frame016.frame,
};

const struct sprite_frame_1box gSpriteBank02Frame000 = {
    SPRITE_FRAME(gSpriteBank02Frame000, SPRITE_TILES_BANK02_FRAME000),
    { { -4, -15, 17, 30 } },
};
const struct sprite_frame_1box gSpriteBank02Frame001 = {
    SPRITE_FRAME(gSpriteBank02Frame001, SPRITE_TILES_BANK02_FRAME001),
    { { -4, -15, 17, 30 } },
};
const struct sprite_frame_1box gSpriteBank02Frame002 = {
    SPRITE_FRAME(gSpriteBank02Frame002, SPRITE_TILES_BANK02_FRAME002),
    { { -4, -15, 17, 30 } },
};
const struct sprite_frame_1box gSpriteBank02Frame003 = {
    SPRITE_FRAME(gSpriteBank02Frame003, SPRITE_TILES_BANK02_FRAME003),
    { { -4, -15, 17, 30 } },
};
const struct sprite_frame_1box gSpriteBank02Frame004 = {
    SPRITE_FRAME(gSpriteBank02Frame004, SPRITE_TILES_BANK02_FRAME004),
    { { -4, -15, 17, 30 } },
};
const struct sprite_frame_1box gSpriteBank02Frame005 = {
    SPRITE_FRAME(gSpriteBank02Frame005, SPRITE_TILES_BANK02_FRAME005),
    { { -4, -15, 17, 30 } },
};
const struct sprite_frame_1box gSpriteBank02Frame006 = {
    SPRITE_FRAME(gSpriteBank02Frame006, SPRITE_TILES_BANK02_FRAME006),
    { { -22, -13, 44, 26 } },
};
const struct sprite_frame_1box gSpriteBank02Frame007 = {
    SPRITE_FRAME(gSpriteBank02Frame007, SPRITE_TILES_BANK02_FRAME007),
    { { -22, -15, 44, 28 } },
};
const struct sprite_frame_1box gSpriteBank02Frame008 = {
    SPRITE_FRAME(gSpriteBank02Frame008, SPRITE_TILES_BANK02_FRAME008),
    { { -23, -19, 47, 32 } },
};
const struct sprite_frame_1box gSpriteBank02Frame009 = {
    SPRITE_FRAME(gSpriteBank02Frame009, SPRITE_TILES_BANK02_FRAME009),
    { { -25, -24, 53, 41 } },
};
const struct sprite_frame_1box gSpriteBank02Frame010 = {
    SPRITE_FRAME(gSpriteBank02Frame010, SPRITE_TILES_BANK02_FRAME010),
    { { -29, -29, 60, 51 } },
};
const struct sprite_frame_1box gSpriteBank02Frame011 = {
    SPRITE_FRAME(gSpriteBank02Frame011, SPRITE_TILES_BANK02_FRAME011),
    { { -33, -33, 68, 58 } },
};
const struct sprite_frame_1box gSpriteBank02Frame012 = {
    SPRITE_FRAME(gSpriteBank02Frame012, SPRITE_TILES_BANK02_FRAME012),
    { { -37, -37, 75, 66 } },
};
const struct sprite_frame_1box gSpriteBank02Frame013 = {
    SPRITE_FRAME(gSpriteBank02Frame013, SPRITE_TILES_BANK02_FRAME013),
    { { -39, -42, 80, 75 } },
};
const struct sprite_frame_1box gSpriteBank02Frame014 = {
    SPRITE_FRAME(gSpriteBank02Frame014, SPRITE_TILES_BANK02_FRAME014),
    { { -43, -46, 88, 83 } },
};
const struct sprite_frame_1box gSpriteBank02Frame015 = {
    SPRITE_FRAME(gSpriteBank02Frame015, SPRITE_TILES_BANK02_FRAME015),
    { { -47, -51, 95, 91 } },
};
const struct sprite_frame_1box gSpriteBank02Frame016 = {
    SPRITE_FRAME(gSpriteBank02Frame016, SPRITE_TILES_BANK02_FRAME016),
    { { -51, -60, 106, 107 } },
};

/* The piece arrays, from graphics/sprites/bank02_crash_hover/bank.json. */
#include "sprites/bank02_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 3: 1 animation, 25 frames, graphics/sprites/bank03_moray_eel/. */

extern const u16 gSpriteBank03Anim00Seq[25];

const struct sprite_anim gSpriteBank03Anims[1] = {
    /* 0 */ {
        /* seq */ gSpriteBank03Anim00Seq,
        /* box */ { { -19, -11, 39, 22 }, { -74, -36, 94, 70 } },
        /* paletteId */ 9,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank03Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank03Anim00Seq[25] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24,
};

const struct sprite_frame *const gSpriteBank03Frames[25] = {
    &gSpriteBank03Frame000.frame,
    &gSpriteBank03Frame001.frame,
    &gSpriteBank03Frame002.frame,
    &gSpriteBank03Frame003.frame,
    &gSpriteBank03Frame004.frame,
    &gSpriteBank03Frame005.frame,
    &gSpriteBank03Frame006.frame,
    &gSpriteBank03Frame007.frame,
    &gSpriteBank03Frame008.frame,
    &gSpriteBank03Frame009.frame,
    &gSpriteBank03Frame010.frame,
    &gSpriteBank03Frame011.frame,
    &gSpriteBank03Frame012.frame,
    &gSpriteBank03Frame013.frame,
    &gSpriteBank03Frame014.frame,
    &gSpriteBank03Frame015.frame,
    &gSpriteBank03Frame016.frame,
    &gSpriteBank03Frame017.frame,
    &gSpriteBank03Frame018.frame,
    &gSpriteBank03Frame019.frame,
    &gSpriteBank03Frame020.frame,
    &gSpriteBank03Frame021.frame,
    &gSpriteBank03Frame022.frame,
    &gSpriteBank03Frame023.frame,
    &gSpriteBank03Frame024.frame,
};

const struct sprite_frame_1box gSpriteBank03Frame000 = {
    SPRITE_FRAME(gSpriteBank03Frame000, SPRITE_TILES_BANK03_FRAME000),
    { { -19, -11, 39, 22 } },
};
const struct sprite_frame_1box gSpriteBank03Frame001 = {
    SPRITE_FRAME(gSpriteBank03Frame001, SPRITE_TILES_BANK03_FRAME001),
    { { -21, -10, 40, 22 } },
};
const struct sprite_frame_1box gSpriteBank03Frame002 = {
    SPRITE_FRAME(gSpriteBank03Frame002, SPRITE_TILES_BANK03_FRAME002),
    { { -25, -9, 42, 28 } },
};
const struct sprite_frame_1box gSpriteBank03Frame003 = {
    SPRITE_FRAME(gSpriteBank03Frame003, SPRITE_TILES_BANK03_FRAME003),
    { { -41, -12, 58, 26 } },
};
const struct sprite_frame_1box gSpriteBank03Frame004 = {
    SPRITE_FRAME(gSpriteBank03Frame004, SPRITE_TILES_BANK03_FRAME004),
    { { -44, -25, 61, 32 } },
};
const struct sprite_frame_1box gSpriteBank03Frame005 = {
    SPRITE_FRAME(gSpriteBank03Frame005, SPRITE_TILES_BANK03_FRAME005),
    { { -34, -36, 51, 43 } },
};
const struct sprite_frame_2box gSpriteBank03Frame006 = {
    SPRITE_FRAME(gSpriteBank03Frame006, SPRITE_TILES_BANK03_FRAME006),
    { { -73, -13, 90, 22 }, { -71, -9, 32, 15 } },
};
const struct sprite_frame_1box gSpriteBank03Frame007 = {
    SPRITE_FRAME(gSpriteBank03Frame007, SPRITE_TILES_BANK03_FRAME007),
    { { -73, -8, 90, 42 } },
};
const struct sprite_frame_1box gSpriteBank03Frame008 = {
    SPRITE_FRAME(gSpriteBank03Frame008, SPRITE_TILES_BANK03_FRAME008),
    { { -74, -9, 91, 37 } },
};
const struct sprite_frame_1box gSpriteBank03Frame009 = {
    SPRITE_FRAME(gSpriteBank03Frame009, SPRITE_TILES_BANK03_FRAME009),
    { { -67, -9, 84, 25 } },
};
const struct sprite_frame_1box gSpriteBank03Frame010 = {
    SPRITE_FRAME(gSpriteBank03Frame010, SPRITE_TILES_BANK03_FRAME010),
    { { -63, -9, 80, 26 } },
};
const struct sprite_frame_1box gSpriteBank03Frame011 = {
    SPRITE_FRAME(gSpriteBank03Frame011, SPRITE_TILES_BANK03_FRAME011),
    { { -64, -9, 81, 28 } },
};
const struct sprite_frame_1box gSpriteBank03Frame012 = {
    SPRITE_FRAME(gSpriteBank03Frame012, SPRITE_TILES_BANK03_FRAME012),
    { { -65, -9, 82, 30 } },
};
const struct sprite_frame_1box gSpriteBank03Frame013 = {
    SPRITE_FRAME(gSpriteBank03Frame013, SPRITE_TILES_BANK03_FRAME013),
    { { -71, -9, 88, 27 } },
};
const struct sprite_frame_1box gSpriteBank03Frame014 = {
    SPRITE_FRAME(gSpriteBank03Frame014, SPRITE_TILES_BANK03_FRAME014),
    { { -72, -9, 89, 29 } },
};
const struct sprite_frame_1box gSpriteBank03Frame015 = {
    SPRITE_FRAME(gSpriteBank03Frame015, SPRITE_TILES_BANK03_FRAME015),
    { { -68, -9, 85, 29 } },
};
const struct sprite_frame_1box gSpriteBank03Frame016 = {
    SPRITE_FRAME(gSpriteBank03Frame016, SPRITE_TILES_BANK03_FRAME016),
    { { -58, -9, 75, 32 } },
};
const struct sprite_frame_1box gSpriteBank03Frame017 = {
    SPRITE_FRAME(gSpriteBank03Frame017, SPRITE_TILES_BANK03_FRAME017),
    { { -52, -9, 69, 24 } },
};
const struct sprite_frame_1box gSpriteBank03Frame018 = {
    SPRITE_FRAME(gSpriteBank03Frame018, SPRITE_TILES_BANK03_FRAME018),
    { { -44, -11, 61, 27 } },
};
const struct sprite_frame_1box gSpriteBank03Frame019 = {
    SPRITE_FRAME(gSpriteBank03Frame019, SPRITE_TILES_BANK03_FRAME019),
    { { -37, -9, 54, 24 } },
};
const struct sprite_frame_1box gSpriteBank03Frame020 = {
    SPRITE_FRAME(gSpriteBank03Frame020, SPRITE_TILES_BANK03_FRAME020),
    { { -28, -9, 45, 28 } },
};
const struct sprite_frame_1box gSpriteBank03Frame021 = {
    SPRITE_FRAME(gSpriteBank03Frame021, SPRITE_TILES_BANK03_FRAME021),
    { { -26, -10, 43, 25 } },
};
const struct sprite_frame_1box gSpriteBank03Frame022 = {
    SPRITE_FRAME(gSpriteBank03Frame022, SPRITE_TILES_BANK03_FRAME022),
    { { -21, -12, 38, 21 } },
};
const struct sprite_frame_1box gSpriteBank03Frame023 = {
    SPRITE_FRAME(gSpriteBank03Frame023, SPRITE_TILES_BANK03_FRAME023),
    { { -19, -12, 39, 23 } },
};
const struct sprite_frame_1box gSpriteBank03Frame024 = {
    SPRITE_FRAME(gSpriteBank03Frame024, SPRITE_TILES_BANK03_FRAME024),
    { { -19, -11, 39, 22 } },
};

/* The piece arrays, from graphics/sprites/bank03_moray_eel/bank.json. */
#include "sprites/bank03_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 4: 2 animations, 32 frames, graphics/sprites/bank04_shark/. */

extern const u16 gSpriteBank04Anim00Seq[16];
extern const u16 gSpriteBank04Anim01Seq[16];

const struct sprite_anim gSpriteBank04Anims[2] = {
    /* 0 */ {
        /* seq */ gSpriteBank04Anim00Seq,
        /* box */ { { -27, -17, 55, 35 }, { -29, -17, 60, 47 } },
        /* paletteId */ 10,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank04Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank04Anim01Seq,
        /* box */ { { -27, -17, 55, 35 }, { -29, -18, 57, 36 } },
        /* paletteId */ 10,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank04Anim01Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank04Anim00Seq[16] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
};
const u16 gSpriteBank04Anim01Seq[16] = {
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
};

const struct sprite_frame *const gSpriteBank04Frames[32] = {
    &gSpriteBank04Frame000.frame,
    &gSpriteBank04Frame001.frame,
    &gSpriteBank04Frame002.frame,
    &gSpriteBank04Frame003.frame,
    &gSpriteBank04Frame004.frame,
    &gSpriteBank04Frame005.frame,
    &gSpriteBank04Frame006.frame,
    &gSpriteBank04Frame007.frame,
    &gSpriteBank04Frame008.frame,
    &gSpriteBank04Frame009.frame,
    &gSpriteBank04Frame010.frame,
    &gSpriteBank04Frame011.frame,
    &gSpriteBank04Frame012.frame,
    &gSpriteBank04Frame013.frame,
    &gSpriteBank04Frame014.frame,
    &gSpriteBank04Frame015.frame,
    &gSpriteBank04Frame016.frame,
    &gSpriteBank04Frame017.frame,
    &gSpriteBank04Frame018.frame,
    &gSpriteBank04Frame019.frame,
    &gSpriteBank04Frame020.frame,
    &gSpriteBank04Frame021.frame,
    &gSpriteBank04Frame022.frame,
    &gSpriteBank04Frame023.frame,
    &gSpriteBank04Frame024.frame,
    &gSpriteBank04Frame025.frame,
    &gSpriteBank04Frame026.frame,
    &gSpriteBank04Frame027.frame,
    &gSpriteBank04Frame028.frame,
    &gSpriteBank04Frame029.frame,
    &gSpriteBank04Frame030.frame,
    &gSpriteBank04Frame031.frame,
};

const struct sprite_frame_1box gSpriteBank04Frame000 = {
    SPRITE_FRAME(gSpriteBank04Frame000, SPRITE_TILES_BANK04_FRAME000),
    { { -19, -5, 40, 18 } },
};
const struct sprite_frame_1box gSpriteBank04Frame001 = {
    SPRITE_FRAME(gSpriteBank04Frame001, SPRITE_TILES_BANK04_FRAME001),
    { { -19, -5, 41, 19 } },
};
const struct sprite_frame_1box gSpriteBank04Frame002 = {
    SPRITE_FRAME(gSpriteBank04Frame002, SPRITE_TILES_BANK04_FRAME002),
    { { -20, -6, 40, 18 } },
};
const struct sprite_frame_1box gSpriteBank04Frame003 = {
    SPRITE_FRAME(gSpriteBank04Frame003, SPRITE_TILES_BANK04_FRAME003),
    { { -20, -6, 40, 19 } },
};
const struct sprite_frame_1box gSpriteBank04Frame004 = {
    SPRITE_FRAME(gSpriteBank04Frame004, SPRITE_TILES_BANK04_FRAME004),
    { { -18, -4, 36, 17 } },
};
const struct sprite_frame_1box gSpriteBank04Frame005 = {
    SPRITE_FRAME(gSpriteBank04Frame005, SPRITE_TILES_BANK04_FRAME005),
    { { -17, -5, 34, 22 } },
};
const struct sprite_frame_1box gSpriteBank04Frame006 = {
    SPRITE_FRAME(gSpriteBank04Frame006, SPRITE_TILES_BANK04_FRAME006),
    { { -15, -4, 31, 18 } },
};
const struct sprite_frame_1box gSpriteBank04Frame007 = {
    SPRITE_FRAME(gSpriteBank04Frame007, SPRITE_TILES_BANK04_FRAME007),
    { { -12, -5, 29, 22 } },
};
const struct sprite_frame_1box gSpriteBank04Frame008 = {
    SPRITE_FRAME(gSpriteBank04Frame008, SPRITE_TILES_BANK04_FRAME008),
    { { -5, -5, 18, 23 } },
};
const struct sprite_frame_1box gSpriteBank04Frame009 = {
    SPRITE_FRAME(gSpriteBank04Frame009, SPRITE_TILES_BANK04_FRAME009),
    { { -9, -8, 24, 27 } },
};
const struct sprite_frame_1box gSpriteBank04Frame010 = {
    SPRITE_FRAME(gSpriteBank04Frame010, SPRITE_TILES_BANK04_FRAME010),
    { { -12, -7, 28, 24 } },
};
const struct sprite_frame_1box gSpriteBank04Frame011 = {
    SPRITE_FRAME(gSpriteBank04Frame011, SPRITE_TILES_BANK04_FRAME011),
    { { -14, -6, 30, 22 } },
};
const struct sprite_frame_1box gSpriteBank04Frame012 = {
    SPRITE_FRAME(gSpriteBank04Frame012, SPRITE_TILES_BANK04_FRAME012),
    { { -15, -6, 32, 21 } },
};
const struct sprite_frame_1box gSpriteBank04Frame013 = {
    SPRITE_FRAME(gSpriteBank04Frame013, SPRITE_TILES_BANK04_FRAME013),
    { { -15, -5, 36, 18 } },
};
const struct sprite_frame_1box gSpriteBank04Frame014 = {
    SPRITE_FRAME(gSpriteBank04Frame014, SPRITE_TILES_BANK04_FRAME014),
    { { -17, -7, 34, 19 } },
};
const struct sprite_frame_1box gSpriteBank04Frame015 = {
    SPRITE_FRAME(gSpriteBank04Frame015, SPRITE_TILES_BANK04_FRAME015),
    { { -18, -8, 36, 21 } },
};
const struct sprite_frame_1box gSpriteBank04Frame016 = {
    SPRITE_FRAME(gSpriteBank04Frame016, SPRITE_TILES_BANK04_FRAME016),
    { { -18, -5, 40, 17 } },
};
const struct sprite_frame_1box gSpriteBank04Frame017 = {
    SPRITE_FRAME(gSpriteBank04Frame017, SPRITE_TILES_BANK04_FRAME017),
    { { -21, -5, 43, 15 } },
};
const struct sprite_frame_1box gSpriteBank04Frame018 = {
    SPRITE_FRAME(gSpriteBank04Frame018, SPRITE_TILES_BANK04_FRAME018),
    { { -16, -3, 38, 15 } },
};
const struct sprite_frame_1box gSpriteBank04Frame019 = {
    SPRITE_FRAME(gSpriteBank04Frame019, SPRITE_TILES_BANK04_FRAME019),
    { { -18, -5, 37, 16 } },
};
const struct sprite_frame_1box gSpriteBank04Frame020 = {
    SPRITE_FRAME(gSpriteBank04Frame020, SPRITE_TILES_BANK04_FRAME020),
    { { -16, -6, 37, 16 } },
};
const struct sprite_frame_1box gSpriteBank04Frame021 = {
    SPRITE_FRAME(gSpriteBank04Frame021, SPRITE_TILES_BANK04_FRAME021),
    { { -15, -5, 35, 15 } },
};
const struct sprite_frame_1box gSpriteBank04Frame022 = {
    SPRITE_FRAME(gSpriteBank04Frame022, SPRITE_TILES_BANK04_FRAME022),
    { { -14, -7, 36, 19 } },
};
const struct sprite_frame_1box gSpriteBank04Frame023 = {
    SPRITE_FRAME(gSpriteBank04Frame023, SPRITE_TILES_BANK04_FRAME023),
    { { -16, -6, 39, 19 } },
};
const struct sprite_frame_1box gSpriteBank04Frame024 = {
    SPRITE_FRAME(gSpriteBank04Frame024, SPRITE_TILES_BANK04_FRAME024),
    { { -16, -6, 38, 18 } },
};
const struct sprite_frame_1box gSpriteBank04Frame025 = {
    SPRITE_FRAME(gSpriteBank04Frame025, SPRITE_TILES_BANK04_FRAME025),
    { { -18, -6, 42, 20 } },
};
const struct sprite_frame_1box gSpriteBank04Frame026 = {
    SPRITE_FRAME(gSpriteBank04Frame026, SPRITE_TILES_BANK04_FRAME026),
    { { -20, -7, 43, 19 } },
};
const struct sprite_frame_1box gSpriteBank04Frame027 = {
    SPRITE_FRAME(gSpriteBank04Frame027, SPRITE_TILES_BANK04_FRAME027),
    { { -21, -6, 45, 19 } },
};
const struct sprite_frame_1box gSpriteBank04Frame028 = {
    SPRITE_FRAME(gSpriteBank04Frame028, SPRITE_TILES_BANK04_FRAME028),
    { { -20, -7, 42, 18 } },
};
const struct sprite_frame_1box gSpriteBank04Frame029 = {
    SPRITE_FRAME(gSpriteBank04Frame029, SPRITE_TILES_BANK04_FRAME029),
    { { -20, -7, 43, 19 } },
};
const struct sprite_frame_1box gSpriteBank04Frame030 = {
    SPRITE_FRAME(gSpriteBank04Frame030, SPRITE_TILES_BANK04_FRAME030),
    { { -19, -7, 41, 19 } },
};
const struct sprite_frame_1box gSpriteBank04Frame031 = {
    SPRITE_FRAME(gSpriteBank04Frame031, SPRITE_TILES_BANK04_FRAME031),
    { { -20, -6, 42, 18 } },
};

/* The piece arrays, from graphics/sprites/bank04_shark/bank.json. */
#include "sprites/bank04_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 5: 4 animations, 25 frames, graphics/sprites/bank05_pufferfish/. */

extern const u16 gSpriteBank05Anim00Seq[7];
extern const u16 gSpriteBank05Anim01Seq[9];
extern const u16 gSpriteBank05Anim02Seq[5];
extern const u16 gSpriteBank05Anim03Seq[7];

const struct sprite_anim gSpriteBank05Anims[4] = {
    /* 0 */ {
        /* seq */ gSpriteBank05Anim00Seq,
        /* box */ { { -12, -7, 25, 14 }, { -22, -24, 40, 44 } },
        /* paletteId */ 11,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank05Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank05Anim01Seq,
        /* box */ { { -12, -7, 25, 14 }, { -21, -21, 40, 40 } },
        /* paletteId */ 11,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank05Anim01Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 2 */ {
        /* seq */ gSpriteBank05Anim02Seq,
        /* box */ { { -12, -7, 25, 14 }, { -15, -7, 28, 14 } },
        /* paletteId */ 11,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank05Anim02Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 3 */ {
        /* seq */ gSpriteBank05Anim03Seq,
        /* box */ { { -12, -7, 25, 14 }, { -22, -24, 40, 44 } },
        /* paletteId */ 11,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank05Anim03Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank05Anim00Seq[7] = {
    0, 1, 2, 3, 4, 5, 6,
};
const u16 gSpriteBank05Anim01Seq[9] = {
    7, 8, 9, 10, 11, 12, 13, 14, 15,
};
const u16 gSpriteBank05Anim02Seq[5] = {
    16, 17, 18, 19, 20,
};
const u16 gSpriteBank05Anim03Seq[7] = {
    21, 22, 23, 24, 2, 1, 0,
};

const struct sprite_frame *const gSpriteBank05Frames[25] = {
    &gSpriteBank05Frame000.frame,
    &gSpriteBank05Frame001.frame,
    &gSpriteBank05Frame002.frame,
    &gSpriteBank05Frame003.frame,
    &gSpriteBank05Frame004.frame,
    &gSpriteBank05Frame005.frame,
    &gSpriteBank05Frame006.frame,
    &gSpriteBank05Frame007.frame,
    &gSpriteBank05Frame008.frame,
    &gSpriteBank05Frame009.frame,
    &gSpriteBank05Frame010.frame,
    &gSpriteBank05Frame011.frame,
    &gSpriteBank05Frame012.frame,
    &gSpriteBank05Frame013.frame,
    &gSpriteBank05Frame014.frame,
    &gSpriteBank05Frame015.frame,
    &gSpriteBank05Frame016.frame,
    &gSpriteBank05Frame017.frame,
    &gSpriteBank05Frame018.frame,
    &gSpriteBank05Frame019.frame,
    &gSpriteBank05Frame020.frame,
    &gSpriteBank05Frame021.frame,
    &gSpriteBank05Frame022.frame,
    &gSpriteBank05Frame023.frame,
    &gSpriteBank05Frame024.frame,
};

const struct sprite_frame_1box gSpriteBank05Frame000 = {
    SPRITE_FRAME(gSpriteBank05Frame000, SPRITE_TILES_BANK05_FRAME000),
    { { -12, -6, 25, 11 } },
};
const struct sprite_frame_1box gSpriteBank05Frame001 = {
    SPRITE_FRAME(gSpriteBank05Frame001, SPRITE_TILES_BANK05_FRAME001),
    { { -13, -5, 27, 10 } },
};
const struct sprite_frame_1box gSpriteBank05Frame002 = {
    SPRITE_FRAME(gSpriteBank05Frame002, SPRITE_TILES_BANK05_FRAME002),
    { { -14, -5, 28, 15 } },
};
const struct sprite_frame_1box gSpriteBank05Frame003 = {
    SPRITE_FRAME(gSpriteBank05Frame003, SPRITE_TILES_BANK05_FRAME003),
    { { -14, -7, 25, 16 } },
};
const struct sprite_frame_1box gSpriteBank05Frame004 = {
    SPRITE_FRAME(gSpriteBank05Frame004, SPRITE_TILES_BANK05_FRAME004),
    { { -14, -9, 26, 23 } },
};
const struct sprite_frame_1box gSpriteBank05Frame005 = {
    SPRITE_FRAME(gSpriteBank05Frame005, SPRITE_TILES_BANK05_FRAME005),
    { { -16, -15, 29, 25 } },
};
const struct sprite_frame_1box gSpriteBank05Frame006 = {
    SPRITE_FRAME(gSpriteBank05Frame006, SPRITE_TILES_BANK05_FRAME006),
    { { -14, -13, 28, 25 } },
};
const struct sprite_frame_1box gSpriteBank05Frame007 = {
    SPRITE_FRAME(gSpriteBank05Frame007, SPRITE_TILES_BANK05_FRAME007),
    { { -13, -17, 26, 31 } },
};
const struct sprite_frame_1box gSpriteBank05Frame008 = {
    SPRITE_FRAME(gSpriteBank05Frame008, SPRITE_TILES_BANK05_FRAME008),
    { { -12, -15, 25, 24 } },
};
const struct sprite_frame_1box gSpriteBank05Frame009 = {
    SPRITE_FRAME(gSpriteBank05Frame009, SPRITE_TILES_BANK05_FRAME009),
    { { -11, -14, 24, 26 } },
};
const struct sprite_frame_1box gSpriteBank05Frame010 = {
    SPRITE_FRAME(gSpriteBank05Frame010, SPRITE_TILES_BANK05_FRAME010),
    { { -11, -14, 24, 27 } },
};
const struct sprite_frame_1box gSpriteBank05Frame011 = {
    SPRITE_FRAME(gSpriteBank05Frame011, SPRITE_TILES_BANK05_FRAME011),
    { { -11, -14, 23, 27 } },
};
const struct sprite_frame_1box gSpriteBank05Frame012 = {
    SPRITE_FRAME(gSpriteBank05Frame012, SPRITE_TILES_BANK05_FRAME012),
    { { -12, -13, 24, 24 } },
};
const struct sprite_frame_1box gSpriteBank05Frame013 = {
    SPRITE_FRAME(gSpriteBank05Frame013, SPRITE_TILES_BANK05_FRAME013),
    { { -14, -13, 26, 25 } },
};
const struct sprite_frame_1box gSpriteBank05Frame014 = {
    SPRITE_FRAME(gSpriteBank05Frame014, SPRITE_TILES_BANK05_FRAME014),
    { { -13, -13, 25, 23 } },
};
const struct sprite_frame_1box gSpriteBank05Frame015 = {
    SPRITE_FRAME(gSpriteBank05Frame015, SPRITE_TILES_BANK05_FRAME015),
    { { -13, -12, 25, 23 } },
};
const struct sprite_frame_1box gSpriteBank05Frame016 = {
    SPRITE_FRAME(gSpriteBank05Frame016, SPRITE_TILES_BANK05_FRAME016),
    { { -12, -7, 25, 14 } },
};
const struct sprite_frame_1box gSpriteBank05Frame017 = {
    SPRITE_FRAME(gSpriteBank05Frame017, SPRITE_TILES_BANK05_FRAME017),
    { { -14, -6, 27, 13 } },
};
const struct sprite_frame_1box gSpriteBank05Frame018 = {
    SPRITE_FRAME(gSpriteBank05Frame018, SPRITE_TILES_BANK05_FRAME018),
    { { -15, -6, 28, 13 } },
};
const struct sprite_frame_1box gSpriteBank05Frame019 = {
    SPRITE_FRAME(gSpriteBank05Frame019, SPRITE_TILES_BANK05_FRAME019),
    { { -14, -7, 27, 13 } },
};
const struct sprite_frame_1box gSpriteBank05Frame020 = {
    SPRITE_FRAME(gSpriteBank05Frame020, SPRITE_TILES_BANK05_FRAME020),
    { { -12, -7, 25, 13 } },
};
const struct sprite_frame_1box gSpriteBank05Frame021 = {
    SPRITE_FRAME(gSpriteBank05Frame021, SPRITE_TILES_BANK05_FRAME021),
    { { -13, -15, 25, 26 } },
};
const struct sprite_frame_1box gSpriteBank05Frame022 = {
    SPRITE_FRAME(gSpriteBank05Frame022, SPRITE_TILES_BANK05_FRAME022),
    { { -14, -15, 26, 26 } },
};
const struct sprite_frame_1box gSpriteBank05Frame023 = {
    SPRITE_FRAME(gSpriteBank05Frame023, SPRITE_TILES_BANK05_FRAME023),
    { { -13, -13, 24, 27 } },
};
const struct sprite_frame_1box gSpriteBank05Frame024 = {
    SPRITE_FRAME(gSpriteBank05Frame024, SPRITE_TILES_BANK05_FRAME024),
    { { -13, -7, 23, 17 } },
};

/* The piece arrays, from graphics/sprites/bank05_pufferfish/bank.json. */
#include "sprites/bank05_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 6: 1 animation, 7 frames, graphics/sprites/bank06_sea_mine/. */

extern const u16 gSpriteBank06Anim00Seq[7];

const struct sprite_anim gSpriteBank06Anims[1] = {
    /* 0 */ {
        /* seq */ gSpriteBank06Anim00Seq,
        /* box */ { { -19, -18, 39, 36 }, { -19, -18, 39, 38 } },
        /* paletteId */ 12,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank06Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank06Anim00Seq[7] = {
    0, 1, 2, 3, 4, 5, 6,
};

const struct sprite_frame *const gSpriteBank06Frames[7] = {
    &gSpriteBank06Frame000.frame,
    &gSpriteBank06Frame001.frame,
    &gSpriteBank06Frame002.frame,
    &gSpriteBank06Frame003.frame,
    &gSpriteBank06Frame004.frame,
    &gSpriteBank06Frame005.frame,
    &gSpriteBank06Frame006.frame,
};

const struct sprite_frame_1box gSpriteBank06Frame000 = {
    SPRITE_FRAME(gSpriteBank06Frame000, SPRITE_TILES_BANK06_FRAME000),
    { { -11, -11, 23, 24 } },
};
const struct sprite_frame_1box gSpriteBank06Frame001 = {
    SPRITE_FRAME(gSpriteBank06Frame001, SPRITE_TILES_BANK06_FRAME001),
    { { -13, -10, 25, 21 } },
};
const struct sprite_frame_1box gSpriteBank06Frame002 = {
    SPRITE_FRAME(gSpriteBank06Frame002, SPRITE_TILES_BANK06_FRAME002),
    { { -12, -11, 24, 25 } },
};
const struct sprite_frame_1box gSpriteBank06Frame003 = {
    SPRITE_FRAME(gSpriteBank06Frame003, SPRITE_TILES_BANK06_FRAME003),
    { { -12, -10, 24, 24 } },
};
const struct sprite_frame_1box gSpriteBank06Frame004 = {
    SPRITE_FRAME(gSpriteBank06Frame004, SPRITE_TILES_BANK06_FRAME004),
    { { -12, -11, 25, 24 } },
};
const struct sprite_frame_1box gSpriteBank06Frame005 = {
    SPRITE_FRAME(gSpriteBank06Frame005, SPRITE_TILES_BANK06_FRAME005),
    { { -12, -10, 24, 22 } },
};
const struct sprite_frame_1box gSpriteBank06Frame006 = {
    SPRITE_FRAME(gSpriteBank06Frame006, SPRITE_TILES_BANK06_FRAME006),
    { { -13, -12, 25, 25 } },
};

/* The piece arrays, from graphics/sprites/bank06_sea_mine/bank.json. */
#include "sprites/bank06_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 7: 3 animations, 38 frames, graphics/sprites/bank07_squid/. */

extern const u16 gSpriteBank07Anim00Seq[9];
extern const u16 gSpriteBank07Anim01Seq[9];
extern const u16 gSpriteBank07Anim02Seq[20];

const struct sprite_anim gSpriteBank07Anims[3] = {
    /* 0 */ {
        /* seq */ gSpriteBank07Anim00Seq,
        /* box */ { { -20, -19, 40, 38 }, { -26, -18, 56, 36 } },
        /* paletteId */ 13,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank07Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank07Anim01Seq,
        /* box */ { { -20, -19, 40, 38 }, { -24, -25, 49, 51 } },
        /* paletteId */ 13,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank07Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank07Anim02Seq,
        /* box */ { { -20, -19, 40, 38 }, { -24, -26, 48, 53 } },
        /* paletteId */ 13,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank07Anim02Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank07Anim00Seq[9] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8,
};
const u16 gSpriteBank07Anim01Seq[9] = {
    9, 10, 11, 12, 13, 14, 15, 16, 17,
};
const u16 gSpriteBank07Anim02Seq[20] = {
    18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
    34, 35, 36, 37,
};

const struct sprite_frame *const gSpriteBank07Frames[38] = {
    &gSpriteBank07Frame000,
    &gSpriteBank07Frame001,
    &gSpriteBank07Frame002,
    &gSpriteBank07Frame003,
    &gSpriteBank07Frame004,
    &gSpriteBank07Frame005,
    &gSpriteBank07Frame006,
    &gSpriteBank07Frame007,
    &gSpriteBank07Frame008,
    &gSpriteBank07Frame009.frame,
    &gSpriteBank07Frame010.frame,
    &gSpriteBank07Frame011.frame,
    &gSpriteBank07Frame012.frame,
    &gSpriteBank07Frame013.frame,
    &gSpriteBank07Frame014.frame,
    &gSpriteBank07Frame015.frame,
    &gSpriteBank07Frame016.frame,
    &gSpriteBank07Frame017.frame,
    &gSpriteBank07Frame018.frame,
    &gSpriteBank07Frame019.frame,
    &gSpriteBank07Frame020.frame,
    &gSpriteBank07Frame021.frame,
    &gSpriteBank07Frame022.frame,
    &gSpriteBank07Frame023.frame,
    &gSpriteBank07Frame024.frame,
    &gSpriteBank07Frame025.frame,
    &gSpriteBank07Frame026.frame,
    &gSpriteBank07Frame027.frame,
    &gSpriteBank07Frame028.frame,
    &gSpriteBank07Frame029.frame,
    &gSpriteBank07Frame030.frame,
    &gSpriteBank07Frame031.frame,
    &gSpriteBank07Frame032.frame,
    &gSpriteBank07Frame033,
    &gSpriteBank07Frame034,
    &gSpriteBank07Frame035,
    &gSpriteBank07Frame036,
    &gSpriteBank07Frame037,
};

const struct sprite_frame gSpriteBank07Frame000 = SPRITE_FRAME(gSpriteBank07Frame000, SPRITE_TILES_BANK07_FRAME000);
const struct sprite_frame gSpriteBank07Frame001 = SPRITE_FRAME(gSpriteBank07Frame001, SPRITE_TILES_BANK07_FRAME001);
const struct sprite_frame gSpriteBank07Frame002 = SPRITE_FRAME(gSpriteBank07Frame002, SPRITE_TILES_BANK07_FRAME002);
const struct sprite_frame gSpriteBank07Frame003 = SPRITE_FRAME(gSpriteBank07Frame003, SPRITE_TILES_BANK07_FRAME003);
const struct sprite_frame gSpriteBank07Frame004 = SPRITE_FRAME(gSpriteBank07Frame004, SPRITE_TILES_BANK07_FRAME004);
const struct sprite_frame gSpriteBank07Frame005 = SPRITE_FRAME(gSpriteBank07Frame005, SPRITE_TILES_BANK07_FRAME005);
const struct sprite_frame gSpriteBank07Frame006 = SPRITE_FRAME(gSpriteBank07Frame006, SPRITE_TILES_BANK07_FRAME006);
const struct sprite_frame gSpriteBank07Frame007 = SPRITE_FRAME(gSpriteBank07Frame007, SPRITE_TILES_BANK07_FRAME007);
const struct sprite_frame gSpriteBank07Frame008 = SPRITE_FRAME(gSpriteBank07Frame008, SPRITE_TILES_BANK07_FRAME008);
const struct sprite_frame_1box gSpriteBank07Frame009 = {
    SPRITE_FRAME(gSpriteBank07Frame009, SPRITE_TILES_BANK07_FRAME009),
    { { 2, -14, 12, 14 } },
};
const struct sprite_frame_1box gSpriteBank07Frame010 = {
    SPRITE_FRAME(gSpriteBank07Frame010, SPRITE_TILES_BANK07_FRAME010),
    { { -2, -14, 11, 12 } },
};
const struct sprite_frame_1box gSpriteBank07Frame011 = {
    SPRITE_FRAME(gSpriteBank07Frame011, SPRITE_TILES_BANK07_FRAME011),
    { { -4, -19, 7, 14 } },
};
const struct sprite_frame_1box gSpriteBank07Frame012 = {
    SPRITE_FRAME(gSpriteBank07Frame012, SPRITE_TILES_BANK07_FRAME012),
    { { -9, -18, 9, 14 } },
};
const struct sprite_frame_1box gSpriteBank07Frame013 = {
    SPRITE_FRAME(gSpriteBank07Frame013, SPRITE_TILES_BANK07_FRAME013),
    { { -13, -14, 9, 12 } },
};
const struct sprite_frame_1box gSpriteBank07Frame014 = {
    SPRITE_FRAME(gSpriteBank07Frame014, SPRITE_TILES_BANK07_FRAME014),
    { { -15, -10, 11, 12 } },
};
const struct sprite_frame_1box gSpriteBank07Frame015 = {
    SPRITE_FRAME(gSpriteBank07Frame015, SPRITE_TILES_BANK07_FRAME015),
    { { -15, -10, 11, 10 } },
};
const struct sprite_frame_1box gSpriteBank07Frame016 = {
    SPRITE_FRAME(gSpriteBank07Frame016, SPRITE_TILES_BANK07_FRAME016),
    { { -15, -10, 14, 10 } },
};
const struct sprite_frame_1box gSpriteBank07Frame017 = {
    SPRITE_FRAME(gSpriteBank07Frame017, SPRITE_TILES_BANK07_FRAME017),
    { { -15, -14, 11, 11 } },
};
const struct sprite_frame_1box gSpriteBank07Frame018 = {
    SPRITE_FRAME(gSpriteBank07Frame018, SPRITE_TILES_BANK07_FRAME018),
    { { -16, -16, 12, 13 } },
};
const struct sprite_frame_1box gSpriteBank07Frame019 = {
    SPRITE_FRAME(gSpriteBank07Frame019, SPRITE_TILES_BANK07_FRAME019),
    { { -14, -13, 11, 10 } },
};
const struct sprite_frame_1box gSpriteBank07Frame020 = {
    SPRITE_FRAME(gSpriteBank07Frame020, SPRITE_TILES_BANK07_FRAME020),
    { { -12, -12, 12, 12 } },
};
const struct sprite_frame_1box gSpriteBank07Frame021 = {
    SPRITE_FRAME(gSpriteBank07Frame021, SPRITE_TILES_BANK07_FRAME021),
    { { -10, -7, 11, 9 } },
};
const struct sprite_frame_1box gSpriteBank07Frame022 = {
    SPRITE_FRAME(gSpriteBank07Frame022, SPRITE_TILES_BANK07_FRAME022),
    { { -8, -7, 10, 11 } },
};
const struct sprite_frame_1box gSpriteBank07Frame023 = {
    SPRITE_FRAME(gSpriteBank07Frame023, SPRITE_TILES_BANK07_FRAME023),
    { { -8, -4, 9, 10 } },
};
const struct sprite_frame_1box gSpriteBank07Frame024 = {
    SPRITE_FRAME(gSpriteBank07Frame024, SPRITE_TILES_BANK07_FRAME024),
    { { -7, -3, 11, 11 } },
};
const struct sprite_frame_1box gSpriteBank07Frame025 = {
    SPRITE_FRAME(gSpriteBank07Frame025, SPRITE_TILES_BANK07_FRAME025),
    { { -4, 1, 9, 9 } },
};
const struct sprite_frame_1box gSpriteBank07Frame026 = {
    SPRITE_FRAME(gSpriteBank07Frame026, SPRITE_TILES_BANK07_FRAME026),
    { { -5, -3, 10, 11 } },
};
const struct sprite_frame_1box gSpriteBank07Frame027 = {
    SPRITE_FRAME(gSpriteBank07Frame027, SPRITE_TILES_BANK07_FRAME027),
    { { -9, -11, 11, 13 } },
};
const struct sprite_frame_1box gSpriteBank07Frame028 = {
    SPRITE_FRAME(gSpriteBank07Frame028, SPRITE_TILES_BANK07_FRAME028),
    { { -16, -17, 11, 13 } },
};
const struct sprite_frame_1box gSpriteBank07Frame029 = {
    SPRITE_FRAME(gSpriteBank07Frame029, SPRITE_TILES_BANK07_FRAME029),
    { { -18, -19, 11, 12 } },
};
const struct sprite_frame_1box gSpriteBank07Frame030 = {
    SPRITE_FRAME(gSpriteBank07Frame030, SPRITE_TILES_BANK07_FRAME030),
    { { -19, -22, 9, 14 } },
};
const struct sprite_frame_1box gSpriteBank07Frame031 = {
    SPRITE_FRAME(gSpriteBank07Frame031, SPRITE_TILES_BANK07_FRAME031),
    { { -20, -21, 12, 13 } },
};
const struct sprite_frame_1box gSpriteBank07Frame032 = {
    SPRITE_FRAME(gSpriteBank07Frame032, SPRITE_TILES_BANK07_FRAME032),
    { { -21, -20, 12, 11 } },
};
const struct sprite_frame gSpriteBank07Frame033 = SPRITE_FRAME(gSpriteBank07Frame033, SPRITE_TILES_BANK07_FRAME033);
const struct sprite_frame gSpriteBank07Frame034 = SPRITE_FRAME(gSpriteBank07Frame034, SPRITE_TILES_BANK07_FRAME034);
const struct sprite_frame gSpriteBank07Frame035 = SPRITE_FRAME(gSpriteBank07Frame035, SPRITE_TILES_BANK07_FRAME035);
const struct sprite_frame gSpriteBank07Frame036 = SPRITE_FRAME(gSpriteBank07Frame036, SPRITE_TILES_BANK07_FRAME036);
const struct sprite_frame gSpriteBank07Frame037 = SPRITE_FRAME(gSpriteBank07Frame037, SPRITE_TILES_BANK07_FRAME037);

/* The piece arrays, from graphics/sprites/bank07_squid/bank.json. */
#include "sprites/bank07_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 8: 3 animations, 36 frames, graphics/sprites/bank08_electric_eel/. */

extern const u16 gSpriteBank08Anim00Seq[12];
extern const u16 gSpriteBank08Anim01Seq[12];
extern const u16 gSpriteBank08Anim02Seq[12];

const struct sprite_anim gSpriteBank08Anims[3] = {
    /* 0 */ {
        /* seq */ gSpriteBank08Anim00Seq,
        /* box */ { { -20, -7, 40, 15 }, { -26, -12, 46, 20 } },
        /* paletteId */ 122,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank08Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank08Anim01Seq,
        /* box */ { { -20, -7, 40, 15 }, { -28, -17, 49, 31 } },
        /* paletteId */ 122,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank08Anim01Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 2 */ {
        /* seq */ gSpriteBank08Anim02Seq,
        /* box */ { { -20, -7, 40, 15 }, { -28, -10, 49, 19 } },
        /* paletteId */ 122,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank08Anim02Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank08Anim00Seq[12] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
};
const u16 gSpriteBank08Anim01Seq[12] = {
    12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
};
const u16 gSpriteBank08Anim02Seq[12] = {
    24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
};

const struct sprite_frame *const gSpriteBank08Frames[36] = {
    &gSpriteBank08Frame000.frame,
    &gSpriteBank08Frame001.frame,
    &gSpriteBank08Frame002.frame,
    &gSpriteBank08Frame003.frame,
    &gSpriteBank08Frame004.frame,
    &gSpriteBank08Frame005.frame,
    &gSpriteBank08Frame006.frame,
    &gSpriteBank08Frame007.frame,
    &gSpriteBank08Frame008.frame,
    &gSpriteBank08Frame009.frame,
    &gSpriteBank08Frame010.frame,
    &gSpriteBank08Frame011.frame,
    &gSpriteBank08Frame012.frame,
    &gSpriteBank08Frame013.frame,
    &gSpriteBank08Frame014.frame,
    &gSpriteBank08Frame015.frame,
    &gSpriteBank08Frame016.frame,
    &gSpriteBank08Frame017.frame,
    &gSpriteBank08Frame018.frame,
    &gSpriteBank08Frame019.frame,
    &gSpriteBank08Frame020.frame,
    &gSpriteBank08Frame021.frame,
    &gSpriteBank08Frame022.frame,
    &gSpriteBank08Frame023.frame,
    &gSpriteBank08Frame024.frame,
    &gSpriteBank08Frame025.frame,
    &gSpriteBank08Frame026.frame,
    &gSpriteBank08Frame027.frame,
    &gSpriteBank08Frame028.frame,
    &gSpriteBank08Frame029.frame,
    &gSpriteBank08Frame030.frame,
    &gSpriteBank08Frame031.frame,
    &gSpriteBank08Frame032.frame,
    &gSpriteBank08Frame033.frame,
    &gSpriteBank08Frame034.frame,
    &gSpriteBank08Frame035.frame,
};

const struct sprite_frame_1box gSpriteBank08Frame000 = {
    SPRITE_FRAME(gSpriteBank08Frame000, SPRITE_TILES_BANK08_FRAME000),
    { { -13, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank08Frame001 = {
    SPRITE_FRAME(gSpriteBank08Frame001, SPRITE_TILES_BANK08_FRAME001),
    { { -14, -2, 28, 8 } },
};
const struct sprite_frame_1box gSpriteBank08Frame002 = {
    SPRITE_FRAME(gSpriteBank08Frame002, SPRITE_TILES_BANK08_FRAME002),
    { { -20, -4, 36, 7 } },
};
const struct sprite_frame_1box gSpriteBank08Frame003 = {
    SPRITE_FRAME(gSpriteBank08Frame003, SPRITE_TILES_BANK08_FRAME003),
    { { -19, -4, 33, 3 } },
};
const struct sprite_frame_1box gSpriteBank08Frame004 = {
    SPRITE_FRAME(gSpriteBank08Frame004, SPRITE_TILES_BANK08_FRAME004),
    { { -10, -8, 19, 9 } },
};
const struct sprite_frame_1box gSpriteBank08Frame005 = {
    SPRITE_FRAME(gSpriteBank08Frame005, SPRITE_TILES_BANK08_FRAME005),
    { { -6, -8, 13, 12 } },
};
const struct sprite_frame_1box gSpriteBank08Frame006 = {
    SPRITE_FRAME(gSpriteBank08Frame006, SPRITE_TILES_BANK08_FRAME006),
    { { -2, -11, 5, 16 } },
};
const struct sprite_frame_1box gSpriteBank08Frame007 = {
    SPRITE_FRAME(gSpriteBank08Frame007, SPRITE_TILES_BANK08_FRAME007),
    { { -7, -7, 12, 11 } },
};
const struct sprite_frame_1box gSpriteBank08Frame008 = {
    SPRITE_FRAME(gSpriteBank08Frame008, SPRITE_TILES_BANK08_FRAME008),
    { { -13, -4, 25, 5 } },
};
const struct sprite_frame_1box gSpriteBank08Frame009 = {
    SPRITE_FRAME(gSpriteBank08Frame009, SPRITE_TILES_BANK08_FRAME009),
    { { -18, -5, 33, 6 } },
};
const struct sprite_frame_1box gSpriteBank08Frame010 = {
    SPRITE_FRAME(gSpriteBank08Frame010, SPRITE_TILES_BANK08_FRAME010),
    { { -19, -5, 32, 6 } },
};
const struct sprite_frame_1box gSpriteBank08Frame011 = {
    SPRITE_FRAME(gSpriteBank08Frame011, SPRITE_TILES_BANK08_FRAME011),
    { { -22, -4, 32, 3 } },
};
const struct sprite_frame_1box gSpriteBank08Frame012 = {
    SPRITE_FRAME(gSpriteBank08Frame012, SPRITE_TILES_BANK08_FRAME012),
    { { -15, -3, 30, 4 } },
};
const struct sprite_frame_1box gSpriteBank08Frame013 = {
    SPRITE_FRAME(gSpriteBank08Frame013, SPRITE_TILES_BANK08_FRAME013),
    { { -15, -3, 30, 4 } },
};
const struct sprite_frame_1box gSpriteBank08Frame014 = {
    SPRITE_FRAME(gSpriteBank08Frame014, SPRITE_TILES_BANK08_FRAME014),
    { { -15, -3, 30, 4 } },
};
const struct sprite_frame_1box gSpriteBank08Frame015 = {
    SPRITE_FRAME(gSpriteBank08Frame015, SPRITE_TILES_BANK08_FRAME015),
    { { -15, -3, 30, 4 } },
};
const struct sprite_frame_1box gSpriteBank08Frame016 = {
    SPRITE_FRAME(gSpriteBank08Frame016, SPRITE_TILES_BANK08_FRAME016),
    { { -15, -3, 30, 4 } },
};
const struct sprite_frame_1box gSpriteBank08Frame017 = {
    SPRITE_FRAME(gSpriteBank08Frame017, SPRITE_TILES_BANK08_FRAME017),
    { { -15, -3, 30, 4 } },
};
const struct sprite_frame_1box gSpriteBank08Frame018 = {
    SPRITE_FRAME(gSpriteBank08Frame018, SPRITE_TILES_BANK08_FRAME018),
    { { -15, -3, 30, 4 } },
};
const struct sprite_frame_1box gSpriteBank08Frame019 = {
    SPRITE_FRAME(gSpriteBank08Frame019, SPRITE_TILES_BANK08_FRAME019),
    { { -15, -3, 30, 4 } },
};
const struct sprite_frame_1box gSpriteBank08Frame020 = {
    SPRITE_FRAME(gSpriteBank08Frame020, SPRITE_TILES_BANK08_FRAME020),
    { { -15, -3, 30, 4 } },
};
const struct sprite_frame_1box gSpriteBank08Frame021 = {
    SPRITE_FRAME(gSpriteBank08Frame021, SPRITE_TILES_BANK08_FRAME021),
    { { -15, -3, 30, 4 } },
};
const struct sprite_frame_1box gSpriteBank08Frame022 = {
    SPRITE_FRAME(gSpriteBank08Frame022, SPRITE_TILES_BANK08_FRAME022),
    { { -15, -3, 30, 4 } },
};
const struct sprite_frame_1box gSpriteBank08Frame023 = {
    SPRITE_FRAME(gSpriteBank08Frame023, SPRITE_TILES_BANK08_FRAME023),
    { { -15, -3, 30, 4 } },
};
const struct sprite_frame_1box gSpriteBank08Frame024 = {
    SPRITE_FRAME(gSpriteBank08Frame024, SPRITE_TILES_BANK08_FRAME024),
    { { -13, -5, 28, 3 } },
};
const struct sprite_frame_1box gSpriteBank08Frame025 = {
    SPRITE_FRAME(gSpriteBank08Frame025, SPRITE_TILES_BANK08_FRAME025),
    { { -13, -5, 28, 3 } },
};
const struct sprite_frame_1box gSpriteBank08Frame026 = {
    SPRITE_FRAME(gSpriteBank08Frame026, SPRITE_TILES_BANK08_FRAME026),
    { { -13, -5, 28, 3 } },
};
const struct sprite_frame_1box gSpriteBank08Frame027 = {
    SPRITE_FRAME(gSpriteBank08Frame027, SPRITE_TILES_BANK08_FRAME027),
    { { -13, -5, 28, 3 } },
};
const struct sprite_frame_1box gSpriteBank08Frame028 = {
    SPRITE_FRAME(gSpriteBank08Frame028, SPRITE_TILES_BANK08_FRAME028),
    { { -13, -5, 28, 3 } },
};
const struct sprite_frame_1box gSpriteBank08Frame029 = {
    SPRITE_FRAME(gSpriteBank08Frame029, SPRITE_TILES_BANK08_FRAME029),
    { { -13, -5, 28, 3 } },
};
const struct sprite_frame_1box gSpriteBank08Frame030 = {
    SPRITE_FRAME(gSpriteBank08Frame030, SPRITE_TILES_BANK08_FRAME030),
    { { -13, -5, 28, 3 } },
};
const struct sprite_frame_1box gSpriteBank08Frame031 = {
    SPRITE_FRAME(gSpriteBank08Frame031, SPRITE_TILES_BANK08_FRAME031),
    { { -13, -5, 28, 3 } },
};
const struct sprite_frame_1box gSpriteBank08Frame032 = {
    SPRITE_FRAME(gSpriteBank08Frame032, SPRITE_TILES_BANK08_FRAME032),
    { { -13, -5, 28, 3 } },
};
const struct sprite_frame_1box gSpriteBank08Frame033 = {
    SPRITE_FRAME(gSpriteBank08Frame033, SPRITE_TILES_BANK08_FRAME033),
    { { -13, -5, 28, 3 } },
};
const struct sprite_frame_1box gSpriteBank08Frame034 = {
    SPRITE_FRAME(gSpriteBank08Frame034, SPRITE_TILES_BANK08_FRAME034),
    { { -13, -5, 28, 3 } },
};
const struct sprite_frame_1box gSpriteBank08Frame035 = {
    SPRITE_FRAME(gSpriteBank08Frame035, SPRITE_TILES_BANK08_FRAME035),
    { { -13, -5, 28, 3 } },
};

/* The piece arrays, from graphics/sprites/bank08_electric_eel/bank.json. */
#include "sprites/bank08_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 9: 1 animation, 14 frames, graphics/sprites/bank09_jellyfish/. */

extern const u16 gSpriteBank09Anim00Seq[14];

const struct sprite_anim gSpriteBank09Anims[1] = {
    /* 0 */ {
        /* seq */ gSpriteBank09Anim00Seq,
        /* box */ { { -11, -17, 22, 35 }, { -11, -23, 22, 44 } },
        /* paletteId */ 16,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank09Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank09Anim00Seq[14] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
};

const struct sprite_frame *const gSpriteBank09Frames[14] = {
    &gSpriteBank09Frame000.frame,
    &gSpriteBank09Frame001.frame,
    &gSpriteBank09Frame002.frame,
    &gSpriteBank09Frame003.frame,
    &gSpriteBank09Frame004.frame,
    &gSpriteBank09Frame005.frame,
    &gSpriteBank09Frame006.frame,
    &gSpriteBank09Frame007.frame,
    &gSpriteBank09Frame008.frame,
    &gSpriteBank09Frame009.frame,
    &gSpriteBank09Frame010.frame,
    &gSpriteBank09Frame011.frame,
    &gSpriteBank09Frame012.frame,
    &gSpriteBank09Frame013.frame,
};

const struct sprite_frame_2box gSpriteBank09Frame000 = {
    SPRITE_FRAME(gSpriteBank09Frame000, SPRITE_TILES_BANK09_FRAME000),
    { { -8, -15, 16, 17 }, { -7, -16, 15, 15 } },
};
const struct sprite_frame_2box gSpriteBank09Frame001 = {
    SPRITE_FRAME(gSpriteBank09Frame001, SPRITE_TILES_BANK09_FRAME001),
    { { -8, -15, 16, 17 }, { -8, -15, 16, 11 } },
};
const struct sprite_frame_2box gSpriteBank09Frame002 = {
    SPRITE_FRAME(gSpriteBank09Frame002, SPRITE_TILES_BANK09_FRAME002),
    { { -8, -15, 16, 17 }, { -7, -17, 14, 12 } },
};
const struct sprite_frame_2box gSpriteBank09Frame003 = {
    SPRITE_FRAME(gSpriteBank09Frame003, SPRITE_TILES_BANK09_FRAME003),
    { { -8, -15, 16, 17 }, { -7, -19, 13, 16 } },
};
const struct sprite_frame_2box gSpriteBank09Frame004 = {
    SPRITE_FRAME(gSpriteBank09Frame004, SPRITE_TILES_BANK09_FRAME004),
    { { -8, -15, 16, 17 }, { -7, -18, 15, 13 } },
};
const struct sprite_frame_2box gSpriteBank09Frame005 = {
    SPRITE_FRAME(gSpriteBank09Frame005, SPRITE_TILES_BANK09_FRAME005),
    { { -8, -15, 16, 17 }, { -7, -20, 14, 15 } },
};
const struct sprite_frame_2box gSpriteBank09Frame006 = {
    SPRITE_FRAME(gSpriteBank09Frame006, SPRITE_TILES_BANK09_FRAME006),
    { { -8, -15, 16, 17 }, { -6, -20, 13, 13 } },
};
const struct sprite_frame_2box gSpriteBank09Frame007 = {
    SPRITE_FRAME(gSpriteBank09Frame007, SPRITE_TILES_BANK09_FRAME007),
    { { -8, -15, 16, 17 }, { -7, -20, 15, 16 } },
};
const struct sprite_frame_2box gSpriteBank09Frame008 = {
    SPRITE_FRAME(gSpriteBank09Frame008, SPRITE_TILES_BANK09_FRAME008),
    { { -8, -15, 16, 17 }, { -7, -19, 15, 15 } },
};
const struct sprite_frame_2box gSpriteBank09Frame009 = {
    SPRITE_FRAME(gSpriteBank09Frame009, SPRITE_TILES_BANK09_FRAME009),
    { { -8, -15, 16, 17 }, { -6, -20, 12, 17 } },
};
const struct sprite_frame_2box gSpriteBank09Frame010 = {
    SPRITE_FRAME(gSpriteBank09Frame010, SPRITE_TILES_BANK09_FRAME010),
    { { -8, -15, 16, 17 }, { -7, -19, 12, 19 } },
};
const struct sprite_frame_2box gSpriteBank09Frame011 = {
    SPRITE_FRAME(gSpriteBank09Frame011, SPRITE_TILES_BANK09_FRAME011),
    { { -8, -15, 16, 17 }, { -7, -16, 14, 15 } },
};
const struct sprite_frame_2box gSpriteBank09Frame012 = {
    SPRITE_FRAME(gSpriteBank09Frame012, SPRITE_TILES_BANK09_FRAME012),
    { { -8, -15, 16, 17 }, { -8, -15, 14, 15 } },
};
const struct sprite_frame_2box gSpriteBank09Frame013 = {
    SPRITE_FRAME(gSpriteBank09Frame013, SPRITE_TILES_BANK09_FRAME013),
    { { -8, -15, 16, 17 }, { -6, -15, 12, 14 } },
};

/* The piece arrays, from graphics/sprites/bank09_jellyfish/bank.json. */
#include "sprites/bank09_pieces.inc"
