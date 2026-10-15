extern "C" {
#include "gba/types.h"
#include "engine/gfx/sprite_bank.h"
#include "globals.h"
}
#include "sprites/sprite_frames.h"

/*
 * ROM 0x084768b4-0x0847e318: sprite banks 0-6 of the sprite-bank
 * animation system (gSpriteBankTable, engine/include/engine/gfx/sprite_bank.h). Linked in
 * ROM order between the incbin'd data by games/n2/ldscript.txt - see
 * docs/data.md and docs/data_map.md ("gSpriteBankTable").
 *
 * Extracted once from baserom_n2.gba by tools/sprite_banks.py --graphics-dir games/n2/graphics; this file is
 * the source now. Per bank: the animations (a frame-index sequence each),
 * the frame pointer array, the frames (header plus the boxes/anchor of
 * their layout type), then every frame's piece positions and piece bytes.
 * The pieces and the frames' tiles come from games/n2/graphics/sprites/bankNN_<name>/
 * (a PNG per frame and bank.json, see docs/graphics.md "Sprite banks"):
 * tools/sprite_frames.py generates the piece arrays (sprites/bankNN_pieces.inc),
 * and sprites/sprite_frames.h declares the frames and gives each one's
 * offset in the sprite tile pool (games/n2/src/engine/gfx/data/sprite_tiles.cpp),
 * SPRITE_TILES_BANKnn_FRAMEkkk. A frame's struct type has to match its
 * "layout" in bank.json.
 */

extern const u8 gSpriteBank00Tiles[];  /* sprite_tiles.cpp, bank 0's tiles */
extern const u8 gObjPalettes[0xba0];
extern const struct sprite_bank gSpriteBanks[49];
extern const struct sprite_anim gSpriteBank07Anims[5];
extern const struct sprite_frame *const gSpriteBank07Frames[55];
extern const struct sprite_anim gSpriteBank08Anims[3];
extern const struct sprite_frame *const gSpriteBank08Frames[30];
extern const struct sprite_anim gSpriteBank09Anims[2];
extern const struct sprite_frame *const gSpriteBank09Frames[19];
extern const struct sprite_anim gSpriteBank10Anims[2];
extern const struct sprite_frame *const gSpriteBank10Frames[15];
extern const struct sprite_anim gSpriteBank11Anims[2];
extern const struct sprite_frame *const gSpriteBank11Frames[23];
extern const struct sprite_anim gSpriteBank12Anims[4];
extern const struct sprite_frame *const gSpriteBank12Frames[21];
extern const struct sprite_anim gSpriteBank13Anims[4];
extern const struct sprite_frame *const gSpriteBank13Frames[38];
extern const struct sprite_anim gSpriteBank14Anims[3];
extern const struct sprite_frame *const gSpriteBank14Frames[30];
extern const struct sprite_anim gSpriteBank15Anims[2];
extern const struct sprite_frame *const gSpriteBank15Frames[56];
extern const struct sprite_anim gSpriteBank16Anims[1];
extern const struct sprite_frame *const gSpriteBank16Frames[15];
extern const struct sprite_anim gSpriteBank17Anims[2];
extern const struct sprite_frame *const gSpriteBank17Frames[44];
extern const struct sprite_anim gSpriteBank18Anims[4];
extern const struct sprite_frame *const gSpriteBank18Frames[33];
extern const struct sprite_anim gSpriteBank19Anims[3];
extern const struct sprite_frame *const gSpriteBank19Frames[47];
extern const struct sprite_anim gSpriteBank20Anims[5];
extern const struct sprite_frame *const gSpriteBank20Frames[61];
extern const struct sprite_anim gSpriteBank21Anims[3];
extern const struct sprite_frame *const gSpriteBank21Frames[65];
extern const struct sprite_anim gSpriteBank22Anims[4];
extern const struct sprite_frame *const gSpriteBank22Frames[28];
extern const struct sprite_anim gSpriteBank23Anims[43];
extern const struct sprite_frame *const gSpriteBank23Frames[132];
extern const struct sprite_anim gSpriteBank24Anims[7];
extern const struct sprite_frame *const gSpriteBank24Frames[24];
extern const struct sprite_anim gSpriteBank25Anims[3];
extern const struct sprite_frame *const gSpriteBank25Frames[16];
extern const struct sprite_anim gSpriteBank26Anims[7];
extern const struct sprite_frame *const gSpriteBank26Frames[31];
extern const struct sprite_anim gSpriteBank27Anims[2];
extern const struct sprite_frame *const gSpriteBank27Frames[28];
extern const struct sprite_anim gSpriteBank28Anims[1];
extern const struct sprite_frame *const gSpriteBank28Frames[10];
extern const struct sprite_anim gSpriteBank29Anims[1];
extern const struct sprite_frame *const gSpriteBank29Frames[8];
extern const struct sprite_anim gSpriteBank30Anims[5];
extern const struct sprite_frame *const gSpriteBank30Frames[72];
extern const struct sprite_anim gSpriteBank31Anims[13];
extern const struct sprite_frame *const gSpriteBank31Frames[97];
extern const struct sprite_anim gSpriteBank32Anims[3];
extern const struct sprite_frame *const gSpriteBank32Frames[24];
extern const struct sprite_anim gSpriteBank33Anims[1];
extern const struct sprite_frame *const gSpriteBank33Frames[11];
extern const struct sprite_anim gSpriteBank34Anims[7];
extern const struct sprite_frame *const gSpriteBank34Frames[21];
extern const struct sprite_anim gSpriteBank35Anims[1];
extern const struct sprite_frame *const gSpriteBank35Frames[13];
extern const struct sprite_anim gSpriteBank36Anims[4];
extern const struct sprite_frame *const gSpriteBank36Frames[13];
extern const struct sprite_anim gSpriteBank37Anims[10];
extern const struct sprite_frame *const gSpriteBank37Frames[48];
extern const struct sprite_anim gSpriteBank38Anims[4];
extern const struct sprite_frame *const gSpriteBank38Frames[40];
extern const struct sprite_anim gSpriteBank39Anims[4];
extern const struct sprite_frame *const gSpriteBank39Frames[13];
extern const struct sprite_anim gSpriteBank40Anims[5];
extern const struct sprite_frame *const gSpriteBank40Frames[5];
extern const struct sprite_anim gSpriteBank41Anims[22];
extern const struct sprite_frame *const gSpriteBank41Frames[65];
extern const struct sprite_anim gSpriteBank42Anims[10];
extern const struct sprite_frame *const gSpriteBank42Frames[135];
extern const struct sprite_anim gSpriteBank43Anims[34];
extern const struct sprite_frame *const gSpriteBank43Frames[234];
extern const struct sprite_anim gSpriteBank44Anims[23];
extern const struct sprite_frame *const gSpriteBank44Frames[190];
extern const struct sprite_anim gSpriteBank45Anims[20];
extern const struct sprite_frame *const gSpriteBank45Frames[163];
extern const struct sprite_anim gSpriteBank46Anims[5];
extern const struct sprite_frame *const gSpriteBank46Frames[88];
extern const struct sprite_anim gSpriteBank47Anims[1];
extern const struct sprite_frame *const gSpriteBank47Frames[3];
extern const struct sprite_anim gSpriteBank48Anims[5];
extern const struct sprite_frame *const gSpriteBank48Frames[5];
extern const struct sprite_anim gSpriteBank00Anims[59];
extern const struct sprite_frame *const gSpriteBank00Frames[473];
extern const struct sprite_anim gSpriteBank01Anims[3];
extern const struct sprite_frame *const gSpriteBank01Frames[22];
extern const struct sprite_anim gSpriteBank02Anims[3];
extern const struct sprite_frame *const gSpriteBank02Frames[36];
extern const struct sprite_anim gSpriteBank03Anims[5];
extern const struct sprite_frame *const gSpriteBank03Frames[65];
extern const struct sprite_anim gSpriteBank04Anims[2];
extern const struct sprite_frame *const gSpriteBank04Frames[18];
extern const struct sprite_anim gSpriteBank05Anims[2];
extern const struct sprite_frame *const gSpriteBank05Frames[19];
extern const struct sprite_anim gSpriteBank06Anims[5];
extern const struct sprite_frame *const gSpriteBank06Frames[52];

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
const struct sprite_bank gSpriteBanks[49] = {
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
};

/* ---------------------------------------------------------------------- */
/* Bank 0: 59 animations, 473 frames, games/n2/graphics/sprites/bank00_crash/. */

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
extern const u16 gSpriteBank00Anim26Seq[3];
extern const u16 gSpriteBank00Anim27Seq[8];
extern const u16 gSpriteBank00Anim28Seq[2];
extern const u16 gSpriteBank00Anim29Seq[10];
extern const u16 gSpriteBank00Anim30Seq[15];
extern const u16 gSpriteBank00Anim31Seq[3];
extern const u16 gSpriteBank00Anim32Seq[14];
extern const u16 gSpriteBank00Anim33Seq[3];
extern const u16 gSpriteBank00Anim34Seq[3];
extern const u16 gSpriteBank00Anim35Seq[8];
extern const u16 gSpriteBank00Anim36Seq[12];
extern const u16 gSpriteBank00Anim37Seq[10];
extern const u16 gSpriteBank00Anim38Seq[10];
extern const u16 gSpriteBank00Anim39Seq[35];
extern const u16 gSpriteBank00Anim40Seq[24];
extern const u16 gSpriteBank00Anim41Seq[10];
extern const u16 gSpriteBank00Anim42Seq[14];
extern const u16 gSpriteBank00Anim43Seq[1];
extern const u16 gSpriteBank00Anim44Seq[18];
extern const u16 gSpriteBank00Anim45Seq[4];
extern const u16 gSpriteBank00Anim46Seq[3];
extern const u16 gSpriteBank00Anim47Seq[3];
extern const u16 gSpriteBank00Anim48Seq[3];
extern const u16 gSpriteBank00Anim49Seq[4];
extern const u16 gSpriteBank00Anim50Seq[3];
extern const u16 gSpriteBank00Anim51Seq[10];
extern const u16 gSpriteBank00Anim52Seq[12];
extern const u16 gSpriteBank00Anim53Seq[4];
extern const u16 gSpriteBank00Anim54Seq[2];
extern const u16 gSpriteBank00Anim55Seq[24];
extern const u16 gSpriteBank00Anim56Seq[11];
extern const u16 gSpriteBank00Anim57Seq[10];
extern const u16 gSpriteBank00Anim58Seq[13];

const struct sprite_anim gSpriteBank00Anims[59] = {
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
        /* paletteId */ 0,
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
        /* box */ { { -7, -27, 14, 41 }, { -20, -34, 39, 53 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim26Seq),
        /* flags */ 0,
    },
    /* 27 */ {
        /* seq */ gSpriteBank00Anim27Seq,
        /* box */ { { -25, -26, 36, 41 }, { -25, -28, 43, 44 } },
        /* paletteId */ 2,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim27Seq),
        /* flags */ 0,
    },
    /* 28 */ {
        /* seq */ gSpriteBank00Anim28Seq,
        /* box */ { { -7, -27, 14, 41 }, { -12, -28, 21, 43 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim28Seq),
        /* flags */ 0,
    },
    /* 29 */ {
        /* seq */ gSpriteBank00Anim29Seq,
        /* box */ { { -7, -27, 14, 41 }, { -33, -28, 63, 34 } },
        /* paletteId */ 0,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim29Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 30 */ {
        /* seq */ gSpriteBank00Anim30Seq,
        /* box */ { { -7, -27, 14, 41 }, { -17, -28, 28, 38 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim30Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 31 */ {
        /* seq */ gSpriteBank00Anim31Seq,
        /* box */ { { -7, -27, 14, 41 }, { -12, -28, 25, 39 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim31Seq),
        /* flags */ 0,
    },
    /* 32 */ {
        /* seq */ gSpriteBank00Anim32Seq,
        /* box */ { { -7, -27, 14, 41 }, { -27, -28, 42, 48 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim32Seq),
        /* flags */ 0,
    },
    /* 33 */ {
        /* seq */ gSpriteBank00Anim33Seq,
        /* box */ { { -7, -27, 14, 41 }, { -25, -28, 35, 38 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim33Seq),
        /* flags */ 0,
    },
    /* 34 */ {
        /* seq */ gSpriteBank00Anim34Seq,
        /* box */ { { -7, -27, 14, 41 }, { -13, -28, 24, 42 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim34Seq),
        /* flags */ 0,
    },
    /* 35 */ {
        /* seq */ gSpriteBank00Anim35Seq,
        /* box */ { { -7, -27, 14, 41 }, { -16, -24, 28, 36 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim35Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 36 */ {
        /* seq */ gSpriteBank00Anim36Seq,
        /* box */ { { -7, -27, 14, 41 }, { -16, -24, 32, 38 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim36Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 37 */ {
        /* seq */ gSpriteBank00Anim37Seq,
        /* box */ { { -7, -27, 14, 41 }, { -25, -24, 45, 38 } },
        /* paletteId */ 0,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim37Seq),
        /* flags */ 0,
    },
    /* 38 */ {
        /* seq */ gSpriteBank00Anim38Seq,
        /* box */ { { -7, -27, 14, 41 }, { -25, -24, 45, 38 } },
        /* paletteId */ 0,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim38Seq),
        /* flags */ 0,
    },
    /* 39 */ {
        /* seq */ gSpriteBank00Anim39Seq,
        /* box */ { { -10, -10, 20, 20 }, { -13, -10, 24, 20 } },
        /* paletteId */ 3,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim39Seq),
        /* flags */ 0,
    },
    /* 40 */ {
        /* seq */ gSpriteBank00Anim40Seq,
        /* box */ { { -11, -19, 23, 39 }, { -15, -19, 30, 40 } },
        /* paletteId */ 4,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim40Seq),
        /* flags */ 0,
    },
    /* 41 */ {
        /* seq */ gSpriteBank00Anim41Seq,
        /* box */ { { -10, -21, 20, 43 }, { -23, -27, 48, 49 } },
        /* paletteId */ 0,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim41Seq),
        /* flags */ 0,
    },
    /* 42 */ {
        /* seq */ gSpriteBank00Anim42Seq,
        /* box */ { { -7, -27, 14, 41 }, { -22, -33, 39, 55 } },
        /* paletteId */ 0,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim42Seq),
        /* flags */ 0,
    },
    /* 43 */ {
        /* seq */ gSpriteBank00Anim43Seq,
        /* box */ { { -7, -27, 14, 41 }, { -15, -29, 21, 50 } },
        /* paletteId */ 0,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim43Seq),
        /* flags */ 0,
    },
    /* 44 */ {
        /* seq */ gSpriteBank00Anim44Seq,
        /* box */ { { -11, -19, 22, 39 }, { -16, -22, 33, 42 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim44Seq),
        /* flags */ 0,
    },
    /* 45 */ {
        /* seq */ gSpriteBank00Anim45Seq,
        /* box */ { { -7, -27, 14, 41 }, { -19, -26, 22, 41 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim45Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 46 */ {
        /* seq */ gSpriteBank00Anim46Seq,
        /* box */ { { -7, -27, 14, 41 }, { -17, -28, 29, 45 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim46Seq),
        /* flags */ 0,
    },
    /* 47 */ {
        /* seq */ gSpriteBank00Anim47Seq,
        /* box */ { { -7, -27, 14, 41 }, { -17, -28, 29, 45 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim47Seq),
        /* flags */ 0,
    },
    /* 48 */ {
        /* seq */ gSpriteBank00Anim48Seq,
        /* box */ { { -7, -27, 14, 41 }, { -19, -37, 33, 55 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim48Seq),
        /* flags */ 0,
    },
    /* 49 */ {
        /* seq */ gSpriteBank00Anim49Seq,
        /* box */ { { -7, -27, 14, 41 }, { -15, -36, 27, 41 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim49Seq),
        /* flags */ 0,
    },
    /* 50 */ {
        /* seq */ gSpriteBank00Anim50Seq,
        /* box */ { { -7, -27, 14, 41 }, { -19, -37, 33, 55 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim50Seq),
        /* flags */ 0,
    },
    /* 51 */ {
        /* seq */ gSpriteBank00Anim51Seq,
        /* box */ { { -7, -27, 14, 41 }, { -15, -34, 33, 28 } },
        /* paletteId */ 0,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim51Seq),
        /* flags */ 0,
    },
    /* 52 */ {
        /* seq */ gSpriteBank00Anim52Seq,
        /* box */ { { -7, -27, 14, 41 }, { -24, -34, 48, 54 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim52Seq),
        /* flags */ 0,
    },
    /* 53 */ {
        /* seq */ gSpriteBank00Anim53Seq,
        /* box */ { { -7, -27, 14, 41 }, { -24, -30, 37, 35 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim53Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 54 */ {
        /* seq */ gSpriteBank00Anim54Seq,
        /* box */ { { -7, -27, 14, 41 }, { -21, -36, 32, 53 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim54Seq),
        /* flags */ 0,
    },
    /* 55 */ {
        /* seq */ gSpriteBank00Anim55Seq,
        /* box */ { { -7, -27, 14, 41 }, { -20, -31, 44, 45 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim55Seq),
        /* flags */ 0,
    },
    /* 56 */ {
        /* seq */ gSpriteBank00Anim56Seq,
        /* box */ { { -7, -27, 14, 41 }, { -17, -33, 41, 46 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim56Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 57 */ {
        /* seq */ gSpriteBank00Anim57Seq,
        /* box */ { { -7, -27, 14, 41 }, { -17, -6, 31, 30 } },
        /* paletteId */ 0,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim57Seq),
        /* flags */ 0,
    },
    /* 58 */ {
        /* seq */ gSpriteBank00Anim58Seq,
        /* box */ { { -25, -18, 44, 38 }, { -26, -19, 52, 38 } },
        /* paletteId */ 5,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank00Anim58Seq),
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
const u16 gSpriteBank00Anim26Seq[3] = {
    222, 223, 224,
};
const u16 gSpriteBank00Anim27Seq[8] = {
    225, 226, 227, 228, 229, 230, 231, 232,
};
const u16 gSpriteBank00Anim28Seq[2] = {
    233, 234,
};
const u16 gSpriteBank00Anim29Seq[10] = {
    235, 236, 237, 238, 239, 235, 236, 237, 238, 239,
};
const u16 gSpriteBank00Anim30Seq[15] = {
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254,
};
const u16 gSpriteBank00Anim31Seq[3] = {
    255, 256, 257,
};
const u16 gSpriteBank00Anim32Seq[14] = {
    258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271,
};
const u16 gSpriteBank00Anim33Seq[3] = {
    272, 273, 274,
};
const u16 gSpriteBank00Anim34Seq[3] = {
    275, 276, 277,
};
const u16 gSpriteBank00Anim35Seq[8] = {
    278, 279, 280, 281, 282, 283, 284, 285,
};
const u16 gSpriteBank00Anim36Seq[12] = {
    286, 287, 288, 289, 290, 291, 292, 293, 294, 295, 296, 297,
};
const u16 gSpriteBank00Anim37Seq[10] = {
    298, 299, 300, 301, 302, 303, 304, 305, 306, 307,
};
const u16 gSpriteBank00Anim38Seq[10] = {
    308, 309, 310, 311, 312, 313, 314, 315, 316, 317,
};
const u16 gSpriteBank00Anim39Seq[35] = {
    318, 319, 320, 321, 322, 323, 324, 325, 325, 325, 325, 325, 325, 326, 327, 325,
    325, 325, 325, 325, 325, 325, 325, 325, 325, 327, 325, 325, 327, 326, 325, 325,
    325, 325, 325,
};
const u16 gSpriteBank00Anim40Seq[24] = {
    328, 328, 328, 328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 340,
    341, 341, 341, 341, 341, 341, 341, 341,
};
const u16 gSpriteBank00Anim41Seq[10] = {
    342, 343, 344, 345, 346, 347, 348, 349, 350, 351,
};
const u16 gSpriteBank00Anim42Seq[14] = {
    352, 353, 354, 355, 356, 357, 358, 359, 360, 361, 362, 363, 364, 365,
};
const u16 gSpriteBank00Anim43Seq[1] = {
    366,
};
const u16 gSpriteBank00Anim44Seq[18] = {
    367, 368, 369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 380, 381, 382,
    383, 384,
};
const u16 gSpriteBank00Anim45Seq[4] = {
    385, 386, 387, 388,
};
const u16 gSpriteBank00Anim46Seq[3] = {
    389, 390, 391,
};
const u16 gSpriteBank00Anim47Seq[3] = {
    391, 390, 389,
};
const u16 gSpriteBank00Anim48Seq[3] = {
    392, 393, 394,
};
const u16 gSpriteBank00Anim49Seq[4] = {
    395, 396, 397, 398,
};
const u16 gSpriteBank00Anim50Seq[3] = {
    394, 393, 392,
};
const u16 gSpriteBank00Anim51Seq[10] = {
    399, 400, 401, 402, 403, 404, 403, 402, 401, 400,
};
const u16 gSpriteBank00Anim52Seq[12] = {
    405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416,
};
const u16 gSpriteBank00Anim53Seq[4] = {
    417, 418, 419, 420,
};
const u16 gSpriteBank00Anim54Seq[2] = {
    421, 422,
};
const u16 gSpriteBank00Anim55Seq[24] = {
    423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433, 434, 435, 436, 437, 438,
    439, 440, 441, 442, 443, 444, 445, 446,
};
const u16 gSpriteBank00Anim56Seq[11] = {
    447, 448, 449, 450, 451, 452, 448, 453, 454, 455, 456,
};
const u16 gSpriteBank00Anim57Seq[10] = {
    457, 458, 459, 460, 461, 462, 463, 464, 465, 466,
};
const u16 gSpriteBank00Anim58Seq[13] = {
    467, 468, 468, 469, 469, 470, 471, 468, 468, 472, 472, 472, 472,
};

const struct sprite_frame *const gSpriteBank00Frames[473] = {
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
    &gSpriteBank00Frame327.frame,
    &gSpriteBank00Frame328.frame,
    &gSpriteBank00Frame329.frame,
    &gSpriteBank00Frame330.frame,
    &gSpriteBank00Frame331.frame,
    &gSpriteBank00Frame332.frame,
    &gSpriteBank00Frame333.frame,
    &gSpriteBank00Frame334.frame,
    &gSpriteBank00Frame335.frame,
    &gSpriteBank00Frame336.frame,
    &gSpriteBank00Frame337.frame,
    &gSpriteBank00Frame338.frame,
    &gSpriteBank00Frame339.frame,
    &gSpriteBank00Frame340.frame,
    &gSpriteBank00Frame341.frame,
    &gSpriteBank00Frame342.frame,
    &gSpriteBank00Frame343.frame,
    &gSpriteBank00Frame344.frame,
    &gSpriteBank00Frame345.frame,
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
    &gSpriteBank00Frame367,
    &gSpriteBank00Frame368,
    &gSpriteBank00Frame369,
    &gSpriteBank00Frame370,
    &gSpriteBank00Frame371,
    &gSpriteBank00Frame372,
    &gSpriteBank00Frame373,
    &gSpriteBank00Frame374,
    &gSpriteBank00Frame375,
    &gSpriteBank00Frame376,
    &gSpriteBank00Frame377,
    &gSpriteBank00Frame378,
    &gSpriteBank00Frame379,
    &gSpriteBank00Frame380,
    &gSpriteBank00Frame381,
    &gSpriteBank00Frame382,
    &gSpriteBank00Frame383,
    &gSpriteBank00Frame384,
    &gSpriteBank00Frame385,
    &gSpriteBank00Frame386,
    &gSpriteBank00Frame387,
    &gSpriteBank00Frame388,
    &gSpriteBank00Frame389,
    &gSpriteBank00Frame390,
    &gSpriteBank00Frame391,
    &gSpriteBank00Frame392.frame,
    &gSpriteBank00Frame393.frame,
    &gSpriteBank00Frame394.frame,
    &gSpriteBank00Frame395.frame,
    &gSpriteBank00Frame396.frame,
    &gSpriteBank00Frame397.frame,
    &gSpriteBank00Frame398.frame,
    &gSpriteBank00Frame399,
    &gSpriteBank00Frame400,
    &gSpriteBank00Frame401,
    &gSpriteBank00Frame402,
    &gSpriteBank00Frame403,
    &gSpriteBank00Frame404,
    &gSpriteBank00Frame405,
    &gSpriteBank00Frame406,
    &gSpriteBank00Frame407,
    &gSpriteBank00Frame408,
    &gSpriteBank00Frame409,
    &gSpriteBank00Frame410,
    &gSpriteBank00Frame411,
    &gSpriteBank00Frame412,
    &gSpriteBank00Frame413,
    &gSpriteBank00Frame414,
    &gSpriteBank00Frame415,
    &gSpriteBank00Frame416,
    &gSpriteBank00Frame417,
    &gSpriteBank00Frame418,
    &gSpriteBank00Frame419,
    &gSpriteBank00Frame420,
    &gSpriteBank00Frame421,
    &gSpriteBank00Frame422,
    &gSpriteBank00Frame423,
    &gSpriteBank00Frame424,
    &gSpriteBank00Frame425,
    &gSpriteBank00Frame426,
    &gSpriteBank00Frame427,
    &gSpriteBank00Frame428,
    &gSpriteBank00Frame429,
    &gSpriteBank00Frame430,
    &gSpriteBank00Frame431,
    &gSpriteBank00Frame432,
    &gSpriteBank00Frame433,
    &gSpriteBank00Frame434,
    &gSpriteBank00Frame435,
    &gSpriteBank00Frame436,
    &gSpriteBank00Frame437,
    &gSpriteBank00Frame438,
    &gSpriteBank00Frame439,
    &gSpriteBank00Frame440,
    &gSpriteBank00Frame441,
    &gSpriteBank00Frame442,
    &gSpriteBank00Frame443,
    &gSpriteBank00Frame444,
    &gSpriteBank00Frame445,
    &gSpriteBank00Frame446,
    &gSpriteBank00Frame447,
    &gSpriteBank00Frame448,
    &gSpriteBank00Frame449,
    &gSpriteBank00Frame450,
    &gSpriteBank00Frame451,
    &gSpriteBank00Frame452,
    &gSpriteBank00Frame453,
    &gSpriteBank00Frame454,
    &gSpriteBank00Frame455,
    &gSpriteBank00Frame456,
    &gSpriteBank00Frame457,
    &gSpriteBank00Frame458,
    &gSpriteBank00Frame459,
    &gSpriteBank00Frame460,
    &gSpriteBank00Frame461,
    &gSpriteBank00Frame462,
    &gSpriteBank00Frame463,
    &gSpriteBank00Frame464,
    &gSpriteBank00Frame465,
    &gSpriteBank00Frame466,
    &gSpriteBank00Frame467,
    &gSpriteBank00Frame468,
    &gSpriteBank00Frame469,
    &gSpriteBank00Frame470,
    &gSpriteBank00Frame471,
    &gSpriteBank00Frame472,
};

const struct sprite_frame_1box gSpriteBank00Frame000 = {
    SPRITE_FRAME(gSpriteBank00Frame000, SPRITE_TILES_BANK00_FRAME000),
    { { -12, -3, 28, 11 } },
};
const struct sprite_frame_1box gSpriteBank00Frame001 = {
    SPRITE_FRAME(gSpriteBank00Frame001, SPRITE_TILES_BANK00_FRAME001),
    { { -12, -3, 28, 11 } },
};
const struct sprite_frame_1box gSpriteBank00Frame002 = {
    SPRITE_FRAME(gSpriteBank00Frame002, SPRITE_TILES_BANK00_FRAME002),
    { { -12, -3, 28, 11 } },
};
const struct sprite_frame_1box gSpriteBank00Frame003 = {
    SPRITE_FRAME(gSpriteBank00Frame003, SPRITE_TILES_BANK00_FRAME003),
    { { -12, -3, 28, 11 } },
};
const struct sprite_frame_1box gSpriteBank00Frame004 = {
    SPRITE_FRAME(gSpriteBank00Frame004, SPRITE_TILES_BANK00_FRAME004),
    { { -12, -3, 28, 11 } },
};
const struct sprite_frame_1box gSpriteBank00Frame005 = {
    SPRITE_FRAME(gSpriteBank00Frame005, SPRITE_TILES_BANK00_FRAME005),
    { { -12, -3, 28, 11 } },
};
const struct sprite_frame_1box gSpriteBank00Frame006 = {
    SPRITE_FRAME(gSpriteBank00Frame006, SPRITE_TILES_BANK00_FRAME006),
    { { -12, -3, 28, 11 } },
};
const struct sprite_frame_1box gSpriteBank00Frame007 = {
    SPRITE_FRAME(gSpriteBank00Frame007, SPRITE_TILES_BANK00_FRAME007),
    { { -12, -3, 28, 11 } },
};
const struct sprite_frame_1box gSpriteBank00Frame008 = {
    SPRITE_FRAME(gSpriteBank00Frame008, SPRITE_TILES_BANK00_FRAME008),
    { { -12, -3, 28, 11 } },
};
const struct sprite_frame_1box gSpriteBank00Frame009 = {
    SPRITE_FRAME(gSpriteBank00Frame009, SPRITE_TILES_BANK00_FRAME009),
    { { -12, -3, 28, 11 } },
};
const struct sprite_frame_1box gSpriteBank00Frame010 = {
    SPRITE_FRAME(gSpriteBank00Frame010, SPRITE_TILES_BANK00_FRAME010),
    { { -12, -3, 28, 11 } },
};
const struct sprite_frame_1box gSpriteBank00Frame011 = {
    SPRITE_FRAME(gSpriteBank00Frame011, SPRITE_TILES_BANK00_FRAME011),
    { { -12, -3, 28, 11 } },
};
const struct sprite_frame_1box gSpriteBank00Frame012 = {
    SPRITE_FRAME(gSpriteBank00Frame012, SPRITE_TILES_BANK00_FRAME012),
    { { -11, -2, 23, 12 } },
};
const struct sprite_frame_1box gSpriteBank00Frame013 = {
    SPRITE_FRAME(gSpriteBank00Frame013, SPRITE_TILES_BANK00_FRAME013),
    { { -7, -5, 21, 15 } },
};
const struct sprite_frame_1box gSpriteBank00Frame014 = {
    SPRITE_FRAME(gSpriteBank00Frame014, SPRITE_TILES_BANK00_FRAME014),
    { { -6, -7, 15, 19 } },
};
const struct sprite_frame_1box gSpriteBank00Frame015 = {
    SPRITE_FRAME(gSpriteBank00Frame015, SPRITE_TILES_BANK00_FRAME015),
    { { -6, -14, 22, 26 } },
};
const struct sprite_frame_1box gSpriteBank00Frame016 = {
    SPRITE_FRAME(gSpriteBank00Frame016, SPRITE_TILES_BANK00_FRAME016),
    { { -6, -17, 20, 29 } },
};
const struct sprite_frame_1box gSpriteBank00Frame017 = {
    SPRITE_FRAME(gSpriteBank00Frame017, SPRITE_TILES_BANK00_FRAME017),
    { { -7, -21, 19, 34 } },
};
const struct sprite_frame_1box gSpriteBank00Frame018 = {
    SPRITE_FRAME(gSpriteBank00Frame018, SPRITE_TILES_BANK00_FRAME018),
    { { -10, -25, 20, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame019 = {
    SPRITE_FRAME(gSpriteBank00Frame019, SPRITE_TILES_BANK00_FRAME019),
    { { -5, -8, 11, 21 } },
};
const struct sprite_frame_1box gSpriteBank00Frame020 = {
    SPRITE_FRAME(gSpriteBank00Frame020, SPRITE_TILES_BANK00_FRAME020),
    { { -5, -8, 11, 21 } },
};
const struct sprite_frame_1box gSpriteBank00Frame021 = {
    SPRITE_FRAME(gSpriteBank00Frame021, SPRITE_TILES_BANK00_FRAME021),
    { { -5, -8, 11, 21 } },
};
const struct sprite_frame_1box gSpriteBank00Frame022 = {
    SPRITE_FRAME(gSpriteBank00Frame022, SPRITE_TILES_BANK00_FRAME022),
    { { -5, -8, 11, 21 } },
};
const struct sprite_frame_1box gSpriteBank00Frame023 = {
    SPRITE_FRAME(gSpriteBank00Frame023, SPRITE_TILES_BANK00_FRAME023),
    { { -7, -7, 14, 21 } },
};
const struct sprite_frame_1box gSpriteBank00Frame024 = {
    SPRITE_FRAME(gSpriteBank00Frame024, SPRITE_TILES_BANK00_FRAME024),
    { { -10, -23, 20, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame025 = {
    SPRITE_FRAME(gSpriteBank00Frame025, SPRITE_TILES_BANK00_FRAME025),
    { { -10, -24, 20, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame026 = {
    SPRITE_FRAME(gSpriteBank00Frame026, SPRITE_TILES_BANK00_FRAME026),
    { { -10, -25, 20, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame027 = {
    SPRITE_FRAME(gSpriteBank00Frame027, SPRITE_TILES_BANK00_FRAME027),
    { { -10, -27, 20, 40 } },
};
const struct sprite_frame_1box gSpriteBank00Frame028 = {
    SPRITE_FRAME(gSpriteBank00Frame028, SPRITE_TILES_BANK00_FRAME028),
    { { -10, -28, 20, 41 } },
};
const struct sprite_frame_1box gSpriteBank00Frame029 = {
    SPRITE_FRAME(gSpriteBank00Frame029, SPRITE_TILES_BANK00_FRAME029),
    { { -10, -30, 20, 43 } },
};
const struct sprite_frame_1box gSpriteBank00Frame030 = {
    SPRITE_FRAME(gSpriteBank00Frame030, SPRITE_TILES_BANK00_FRAME030),
    { { -10, -30, 20, 43 } },
};
const struct sprite_frame_1box gSpriteBank00Frame031 = {
    SPRITE_FRAME(gSpriteBank00Frame031, SPRITE_TILES_BANK00_FRAME031),
    { { -9, -30, 19, 43 } },
};
const struct sprite_frame_1box gSpriteBank00Frame032 = {
    SPRITE_FRAME(gSpriteBank00Frame032, SPRITE_TILES_BANK00_FRAME032),
    { { -9, -29, 19, 42 } },
};
const struct sprite_frame_1box gSpriteBank00Frame033 = {
    SPRITE_FRAME(gSpriteBank00Frame033, SPRITE_TILES_BANK00_FRAME033),
    { { -9, -29, 19, 42 } },
};
const struct sprite_frame_1box gSpriteBank00Frame034 = {
    SPRITE_FRAME(gSpriteBank00Frame034, SPRITE_TILES_BANK00_FRAME034),
    { { -8, -27, 18, 40 } },
};
const struct sprite_frame_1box gSpriteBank00Frame035 = {
    SPRITE_FRAME(gSpriteBank00Frame035, SPRITE_TILES_BANK00_FRAME035),
    { { -8, -25, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame036 = {
    SPRITE_FRAME(gSpriteBank00Frame036, SPRITE_TILES_BANK00_FRAME036),
    { { -8, -24, 17, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame037 = {
    SPRITE_FRAME(gSpriteBank00Frame037, SPRITE_TILES_BANK00_FRAME037),
    { { -8, -22, 17, 35 } },
};
const struct sprite_frame_1box gSpriteBank00Frame038 = {
    SPRITE_FRAME(gSpriteBank00Frame038, SPRITE_TILES_BANK00_FRAME038),
    { { -8, -22, 17, 35 } },
};
const struct sprite_frame_1box gSpriteBank00Frame039 = {
    SPRITE_FRAME(gSpriteBank00Frame039, SPRITE_TILES_BANK00_FRAME039),
    { { -8, -21, 17, 34 } },
};
const struct sprite_frame_1box gSpriteBank00Frame040 = {
    SPRITE_FRAME(gSpriteBank00Frame040, SPRITE_TILES_BANK00_FRAME040),
    { { -8, -21, 18, 34 } },
};
const struct sprite_frame_1box gSpriteBank00Frame041 = {
    SPRITE_FRAME(gSpriteBank00Frame041, SPRITE_TILES_BANK00_FRAME041),
    { { -8, -20, 18, 33 } },
};
const struct sprite_frame_1box gSpriteBank00Frame042 = {
    SPRITE_FRAME(gSpriteBank00Frame042, SPRITE_TILES_BANK00_FRAME042),
    { { -8, -21, 18, 34 } },
};
const struct sprite_frame_1box gSpriteBank00Frame043 = {
    SPRITE_FRAME(gSpriteBank00Frame043, SPRITE_TILES_BANK00_FRAME043),
    { { -8, -21, 18, 34 } },
};
const struct sprite_frame_1box gSpriteBank00Frame044 = {
    SPRITE_FRAME(gSpriteBank00Frame044, SPRITE_TILES_BANK00_FRAME044),
    { { -8, -22, 18, 35 } },
};
const struct sprite_frame_1box gSpriteBank00Frame045 = {
    SPRITE_FRAME(gSpriteBank00Frame045, SPRITE_TILES_BANK00_FRAME045),
    { { -9, -23, 19, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame046 = {
    SPRITE_FRAME(gSpriteBank00Frame046, SPRITE_TILES_BANK00_FRAME046),
    { { -10, -23, 20, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame047 = {
    SPRITE_FRAME(gSpriteBank00Frame047, SPRITE_TILES_BANK00_FRAME047),
    { { -10, -23, 20, 36 } },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame048 = {
    SPRITE_FRAME(gSpriteBank00Frame048, SPRITE_TILES_BANK00_FRAME048),
    { { -5, -25, 8, 36 } },
    { 1, -30 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame049 = {
    SPRITE_FRAME(gSpriteBank00Frame049, SPRITE_TILES_BANK00_FRAME049),
    { { -4, -20, 8, 28 } },
    { 2, -9 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame050 = {
    SPRITE_FRAME(gSpriteBank00Frame050, SPRITE_TILES_BANK00_FRAME050),
    { { -4, -17, 11, 20 } },
    { -3, -20 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame051 = {
    SPRITE_FRAME(gSpriteBank00Frame051, SPRITE_TILES_BANK00_FRAME051),
    { { -5, -11, 15, 13 } },
    { 0, -16 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame052 = {
    SPRITE_FRAME(gSpriteBank00Frame052, SPRITE_TILES_BANK00_FRAME052),
    { { -7, -14, 14, 13 } },
    { -1, -14 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame053 = {
    SPRITE_FRAME(gSpriteBank00Frame053, SPRITE_TILES_BANK00_FRAME053),
    { { -9, -15, 22, 14 } },
    { -1, -15 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame054 = {
    SPRITE_FRAME(gSpriteBank00Frame054, SPRITE_TILES_BANK00_FRAME054),
    { { 0, 0, 0, 0 }, { -7, -12, 28, 18 }, { 0, 0, 0, 0 } },
    { -4, -20 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame055 = {
    SPRITE_FRAME(gSpriteBank00Frame055, SPRITE_TILES_BANK00_FRAME055),
    { { -14, -25, 14, 15 }, { -7, -7, 19, 25 }, { 0, 0, 0, 0 } },
    { -1, -23 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame056 = {
    SPRITE_FRAME(gSpriteBank00Frame056, SPRITE_TILES_BANK00_FRAME056),
    { { -8, -24, 12, 15 }, { -6, -6, 12, 23 }, { 0, 0, 0, 0 } },
    { 7, -22 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame057 = {
    SPRITE_FRAME(gSpriteBank00Frame057, SPRITE_TILES_BANK00_FRAME057),
    { { 0, 0, 0, 0 }, { -20, -14, 34, 11 }, { -15, -5, 30, 6 } },
    { 4, -10 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame058 = {
    SPRITE_FRAME(gSpriteBank00Frame058, SPRITE_TILES_BANK00_FRAME058),
    { { 0, 0, 0, 0 }, { -15, -7, 21, 5 }, { -14, -5, 18, 8 } },
    { 0, -13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame059 = {
    SPRITE_FRAME(gSpriteBank00Frame059, SPRITE_TILES_BANK00_FRAME059),
    { { 0, 0, 0, 0 }, { -20, -12, 29, 12 }, { -18, -4, 26, 7 } },
    { -9, -10 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame060 = {
    SPRITE_FRAME(gSpriteBank00Frame060, SPRITE_TILES_BANK00_FRAME060),
    { { 0, 0, 0, 0 }, { -20, -11, 31, 9 }, { -17, -4, 26, 5 } },
    { -6, -5 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame061 = {
    SPRITE_FRAME(gSpriteBank00Frame061, SPRITE_TILES_BANK00_FRAME061),
    { { 0, 0, 0, 0 }, { -11, -12, 21, 8 }, { -9, -6, 18, 6 } },
    { 0, -4 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame062 = {
    SPRITE_FRAME(gSpriteBank00Frame062, SPRITE_TILES_BANK00_FRAME062),
    { { 0, 0, 0, 0 }, { -10, -10, 26, 8 }, { -10, -7, 25, 7 } },
    { 8, -7 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame063 = {
    SPRITE_FRAME(gSpriteBank00Frame063, SPRITE_TILES_BANK00_FRAME063),
    { { 0, 0, 0, 0 }, { -19, -10, 42, 10 }, { -9, -4, 25, 6 } },
    { 11, -10 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame064 = {
    SPRITE_FRAME(gSpriteBank00Frame064, SPRITE_TILES_BANK00_FRAME064),
    { { -14, -16, 18, 21 }, { -13, -12, 17, 17 }, { 0, 0, 0, 0 } },
    { 19, -7 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame065 = {
    SPRITE_FRAME(gSpriteBank00Frame065, SPRITE_TILES_BANK00_FRAME065),
    { { -14, -16, 29, 22 }, { -17, -8, 32, 8 }, { 0, 0, 0, 0 } },
    { 19, -7 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame066 = {
    SPRITE_FRAME(gSpriteBank00Frame066, SPRITE_TILES_BANK00_FRAME066),
    { { 0, 0, 0, 0 }, { -18, -10, 40, 10 }, { -10, -10, 26, 6 } },
    { 19, -7 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame067 = {
    SPRITE_FRAME(gSpriteBank00Frame067, SPRITE_TILES_BANK00_FRAME067),
    { { -12, -18, 26, 9 }, { -8, -9, 16, 17 }, { 0, 0, 0, 0 } },
    { 1, -20 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame068 = {
    SPRITE_FRAME(gSpriteBank00Frame068, SPRITE_TILES_BANK00_FRAME068),
    { { -20, -18, 31, 18 }, { -17, -13, 30, 11 }, { 0, 0, 0, 0 } },
    { 4, -17 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame069 = {
    SPRITE_FRAME(gSpriteBank00Frame069, SPRITE_TILES_BANK00_FRAME069),
    { { 0, 0, 0, 0 }, { -19, -12, 43, 11 }, { -9, -9, 25, 5 } },
    { 4, -13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame070 = {
    SPRITE_FRAME(gSpriteBank00Frame070, SPRITE_TILES_BANK00_FRAME070),
    { { -8, -16, 17, 15 }, { -9, -8, 19, 29 }, { 0, 0, 0, 0 } },
    { 8, -26 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame071 = {
    SPRITE_FRAME(gSpriteBank00Frame071, SPRITE_TILES_BANK00_FRAME071),
    { { -7, -19, 14, 20 }, { -8, -10, 16, 32 }, { 0, 0, 0, 0 } },
    { 5, -29 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame072 = {
    SPRITE_FRAME(gSpriteBank00Frame072, SPRITE_TILES_BANK00_FRAME072),
    { { -5, -19, 15, 14 }, { -4, -9, 13, 24 }, { 0, 0, 0, 0 } },
    { 7, -24 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame073 = {
    SPRITE_FRAME(gSpriteBank00Frame073, SPRITE_TILES_BANK00_FRAME073),
    { { -4, -21, 15, 8 }, { -4, -10, 15, 16 }, { 0, 0, 0, 0 } },
    { 9, -18 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame074 = {
    SPRITE_FRAME(gSpriteBank00Frame074, SPRITE_TILES_BANK00_FRAME074),
    { { -4, -24, 14, 8 }, { -4, -14, 14, 18 }, { 0, 0, 0, 0 } },
    { 7, -21 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame075 = {
    SPRITE_FRAME(gSpriteBank00Frame075, SPRITE_TILES_BANK00_FRAME075),
    { { -4, -26, 14, 11 }, { -4, -15, 14, 19 }, { 0, 0, 0, 0 } },
    { 7, -27 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame076 = {
    SPRITE_FRAME(gSpriteBank00Frame076, SPRITE_TILES_BANK00_FRAME076),
    { { -4, -24, 15, 10 }, { -4, -13, 15, 18 }, { 0, 0, 0, 0 } },
    { 7, -24 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame077 = {
    SPRITE_FRAME(gSpriteBank00Frame077, SPRITE_TILES_BANK00_FRAME077),
    { { -5, -21, 16, 16 }, { -5, -10, 16, 24 }, { 0, 0, 0, 0 } },
    { 8, -22 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame078 = {
    SPRITE_FRAME(gSpriteBank00Frame078, SPRITE_TILES_BANK00_FRAME078),
    { { -6, -21, 14, 20 }, { -6, -12, 14, 32 }, { 0, 0, 0, 0 } },
    { 2, -26 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame079 = {
    SPRITE_FRAME(gSpriteBank00Frame079, SPRITE_TILES_BANK00_FRAME079),
    { { -8, -23, 14, 21 }, { -8, -14, 14, 33 }, { 0, 0, 0, 0 } },
    { -2, -28 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame080 = {
    SPRITE_FRAME(gSpriteBank00Frame080, SPRITE_TILES_BANK00_FRAME080),
    { { -6, -15, 22, 10 }, { -5, -1, 15, 14 }, { 0, 0, 0, 0 } },
    { 13, -14 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame081 = {
    SPRITE_FRAME(gSpriteBank00Frame081, SPRITE_TILES_BANK00_FRAME081),
    { { -4, -19, 13, 23 }, { -5, 2, 10, 16 }, { 0, 0, 0, 0 } },
    { 6, -19 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame082 = {
    SPRITE_FRAME(gSpriteBank00Frame082, SPRITE_TILES_BANK00_FRAME082),
    { { -7, -20, 13, 27 }, { -9, 6, 12, 13 }, { 0, 0, 0, 0 } },
    { 5, -20 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame083 = {
    SPRITE_FRAME(gSpriteBank00Frame083, SPRITE_TILES_BANK00_FRAME083),
    { { -7, -22, 14, 24 }, { -8, 2, 10, 17 }, { 0, 0, 0, 0 } },
    { 4, -27 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame084 = {
    SPRITE_FRAME(gSpriteBank00Frame084, SPRITE_TILES_BANK00_FRAME084),
    { { -3, -21, 11, 16 }, { -5, -2, 12, 15 }, { 0, 0, 0, 0 } },
    { 7, -25 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame085 = {
    SPRITE_FRAME(gSpriteBank00Frame085, SPRITE_TILES_BANK00_FRAME085),
    { { -4, -23, 15, 10 }, { -5, -9, 16, 14 }, { 0, 0, 0, 0 } },
    { 9, -24 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame086 = {
    SPRITE_FRAME(gSpriteBank00Frame086, SPRITE_TILES_BANK00_FRAME086),
    { { -4, -18, 15, 7 }, { -6, -7, 17, 11 }, { 0, 0, 0, 0 } },
    { 10, -21 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame087 = {
    SPRITE_FRAME(gSpriteBank00Frame087, SPRITE_TILES_BANK00_FRAME087),
    { { -4, -17, 14, 13 }, { -5, -6, 16, 9 }, { 0, 0, 0, 0 } },
    { 6, -19 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame088 = {
    SPRITE_FRAME(gSpriteBank00Frame088, SPRITE_TILES_BANK00_FRAME088),
    { { -4, -20, 14, 11 }, { -5, -8, 17, 12 }, { 0, 0, 0, 0 } },
    { 6, -24 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame089 = {
    SPRITE_FRAME(gSpriteBank00Frame089, SPRITE_TILES_BANK00_FRAME089),
    { { -5, -22, 16, 16 }, { -5, -3, 14, 18 }, { 0, 0, 0, 0 } },
    { 8, -25 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame090 = {
    SPRITE_FRAME(gSpriteBank00Frame090, SPRITE_TILES_BANK00_FRAME090),
    { { -7, -24, 16, 24 }, { -6, 2, 14, 18 }, { 0, 0, 0, 0 } },
    { 7, -25 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame091 = {
    SPRITE_FRAME(gSpriteBank00Frame091, SPRITE_TILES_BANK00_FRAME091),
    { { -9, -26, 16, 25 }, { -5, -2, 13, 21 }, { 0, 0, 0, 0 } },
    { 4, -27 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame092 = {
    SPRITE_FRAME(gSpriteBank00Frame092, SPRITE_TILES_BANK00_FRAME092),
    { { -16, -29, 13, 25 }, { -5, 6, 8, 11 }, { 0, 0, 0, 0 } },
    { -2, -32 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame093 = {
    SPRITE_FRAME(gSpriteBank00Frame093, SPRITE_TILES_BANK00_FRAME093),
    { { -16, -28, 13, 22 }, { -5, 5, 6, 10 }, { 0, 0, 0, 0 } },
    { -1, -32 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame094 = {
    SPRITE_FRAME(gSpriteBank00Frame094, SPRITE_TILES_BANK00_FRAME094),
    { { -15, -27, 14, 20 }, { -5, 2, 8, 12 }, { 0, 0, 0, 0 } },
    { 2, -31 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame095 = {
    SPRITE_FRAME(gSpriteBank00Frame095, SPRITE_TILES_BANK00_FRAME095),
    { { -14, -26, 15, 19 }, { -6, 6, 8, 9 }, { 0, 0, 0, 0 } },
    { 4, -28 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame096 = {
    SPRITE_FRAME(gSpriteBank00Frame096, SPRITE_TILES_BANK00_FRAME096),
    { { -13, -22, 14, 18 }, { -5, 6, 8, 10 }, { 0, 0, 0, 0 } },
    { 5, -26 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame097 = {
    SPRITE_FRAME(gSpriteBank00Frame097, SPRITE_TILES_BANK00_FRAME097),
    { { -13, -19, 14, 17 }, { -5, 5, 9, 10 }, { 0, 0, 0, 0 } },
    { 4, -26 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame098 = {
    SPRITE_FRAME(gSpriteBank00Frame098, SPRITE_TILES_BANK00_FRAME098),
    { { -13, -17, 14, 18 }, { -5, 5, 10, 11 }, { 0, 0, 0, 0 } },
    { 3, -26 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame099 = {
    SPRITE_FRAME(gSpriteBank00Frame099, SPRITE_TILES_BANK00_FRAME099),
    { { -12, -18, 14, 21 }, { -4, 5, 10, 10 }, { 0, 0, 0, 0 } },
    { 3, -27 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame100 = {
    SPRITE_FRAME(gSpriteBank00Frame100, SPRITE_TILES_BANK00_FRAME100),
    { { -7, -16, 13, 19 }, { -4, 5, 9, 10 }, { 0, 0, 0, 0 } },
    { 5, -26 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame101 = {
    SPRITE_FRAME(gSpriteBank00Frame101, SPRITE_TILES_BANK00_FRAME101),
    { { -7, -20, 13, 20 }, { -3, 6, 9, 10 }, { 0, 0, 0, 0 } },
    { 6, -26 },
};
const struct sprite_frame_1box gSpriteBank00Frame102 = {
    SPRITE_FRAME(gSpriteBank00Frame102, SPRITE_TILES_BANK00_FRAME102),
    { { -7, -28, 14, 42 } },
};
const struct sprite_frame_1box gSpriteBank00Frame103 = {
    SPRITE_FRAME(gSpriteBank00Frame103, SPRITE_TILES_BANK00_FRAME103),
    { { -7, -28, 14, 42 } },
};
const struct sprite_frame_1box gSpriteBank00Frame104 = {
    SPRITE_FRAME(gSpriteBank00Frame104, SPRITE_TILES_BANK00_FRAME104),
    { { -7, -28, 14, 42 } },
};
const struct sprite_frame_1box gSpriteBank00Frame105 = {
    SPRITE_FRAME(gSpriteBank00Frame105, SPRITE_TILES_BANK00_FRAME105),
    { { -7, -28, 14, 42 } },
};
const struct sprite_frame_1box gSpriteBank00Frame106 = {
    SPRITE_FRAME(gSpriteBank00Frame106, SPRITE_TILES_BANK00_FRAME106),
    { { -7, -28, 14, 42 } },
};
const struct sprite_frame_1box gSpriteBank00Frame107 = {
    SPRITE_FRAME(gSpriteBank00Frame107, SPRITE_TILES_BANK00_FRAME107),
    { { -7, -28, 14, 42 } },
};
const struct sprite_frame_1box gSpriteBank00Frame108 = {
    SPRITE_FRAME(gSpriteBank00Frame108, SPRITE_TILES_BANK00_FRAME108),
    { { -7, -28, 14, 42 } },
};
const struct sprite_frame_1box gSpriteBank00Frame109 = {
    SPRITE_FRAME(gSpriteBank00Frame109, SPRITE_TILES_BANK00_FRAME109),
    { { -7, -28, 14, 42 } },
};
const struct sprite_frame_1box gSpriteBank00Frame110 = {
    SPRITE_FRAME(gSpriteBank00Frame110, SPRITE_TILES_BANK00_FRAME110),
    { { -7, -28, 14, 42 } },
};
const struct sprite_frame_1box gSpriteBank00Frame111 = {
    SPRITE_FRAME(gSpriteBank00Frame111, SPRITE_TILES_BANK00_FRAME111),
    { { -7, -28, 14, 42 } },
};
const struct sprite_frame_1box gSpriteBank00Frame112 = {
    SPRITE_FRAME(gSpriteBank00Frame112, SPRITE_TILES_BANK00_FRAME112),
    { { -7, -28, 14, 42 } },
};
const struct sprite_frame_1box gSpriteBank00Frame113 = {
    SPRITE_FRAME(gSpriteBank00Frame113, SPRITE_TILES_BANK00_FRAME113),
    { { -7, -28, 14, 42 } },
};
const struct sprite_frame_1box gSpriteBank00Frame114 = {
    SPRITE_FRAME(gSpriteBank00Frame114, SPRITE_TILES_BANK00_FRAME114),
    { { -7, -28, 14, 42 } },
};
const struct sprite_frame_1box gSpriteBank00Frame115 = {
    SPRITE_FRAME(gSpriteBank00Frame115, SPRITE_TILES_BANK00_FRAME115),
    { { -7, -28, 14, 42 } },
};
const struct sprite_frame_1box gSpriteBank00Frame116 = {
    SPRITE_FRAME(gSpriteBank00Frame116, SPRITE_TILES_BANK00_FRAME116),
    { { -7, -28, 14, 42 } },
};
const struct sprite_frame_1box gSpriteBank00Frame117 = {
    SPRITE_FRAME(gSpriteBank00Frame117, SPRITE_TILES_BANK00_FRAME117),
    { { -7, -28, 14, 42 } },
};
const struct sprite_frame_1box gSpriteBank00Frame118 = {
    SPRITE_FRAME(gSpriteBank00Frame118, SPRITE_TILES_BANK00_FRAME118),
    { { -7, -28, 14, 42 } },
};
const struct sprite_frame_1box gSpriteBank00Frame119 = {
    SPRITE_FRAME(gSpriteBank00Frame119, SPRITE_TILES_BANK00_FRAME119),
    { { -7, -28, 14, 42 } },
};
const struct sprite_frame_1box gSpriteBank00Frame120 = {
    SPRITE_FRAME(gSpriteBank00Frame120, SPRITE_TILES_BANK00_FRAME120),
    { { -7, -28, 14, 42 } },
};
const struct sprite_frame_1box gSpriteBank00Frame121 = {
    SPRITE_FRAME(gSpriteBank00Frame121, SPRITE_TILES_BANK00_FRAME121),
    { { -7, -28, 14, 42 } },
};
const struct sprite_frame_1box gSpriteBank00Frame122 = {
    SPRITE_FRAME(gSpriteBank00Frame122, SPRITE_TILES_BANK00_FRAME122),
    { { -10, -22, 20, 35 } },
};
const struct sprite_frame_1box gSpriteBank00Frame123 = {
    SPRITE_FRAME(gSpriteBank00Frame123, SPRITE_TILES_BANK00_FRAME123),
    { { -9, -23, 19, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame124 = {
    SPRITE_FRAME(gSpriteBank00Frame124, SPRITE_TILES_BANK00_FRAME124),
    { { -8, -24, 18, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame125 = {
    SPRITE_FRAME(gSpriteBank00Frame125, SPRITE_TILES_BANK00_FRAME125),
    { { -9, -25, 19, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame126 = {
    SPRITE_FRAME(gSpriteBank00Frame126, SPRITE_TILES_BANK00_FRAME126),
    { { -9, -25, 19, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame127 = {
    SPRITE_FRAME(gSpriteBank00Frame127, SPRITE_TILES_BANK00_FRAME127),
    { { -10, -27, 18, 40 } },
};
const struct sprite_frame_1box gSpriteBank00Frame128 = {
    SPRITE_FRAME(gSpriteBank00Frame128, SPRITE_TILES_BANK00_FRAME128),
    { { -11, -27, 18, 40 } },
};
const struct sprite_frame_1box gSpriteBank00Frame129 = {
    SPRITE_FRAME(gSpriteBank00Frame129, SPRITE_TILES_BANK00_FRAME129),
    { { -12, -27, 18, 40 } },
};
const struct sprite_frame_1box gSpriteBank00Frame130 = {
    SPRITE_FRAME(gSpriteBank00Frame130, SPRITE_TILES_BANK00_FRAME130),
    { { -13, -25, 19, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame131 = {
    SPRITE_FRAME(gSpriteBank00Frame131, SPRITE_TILES_BANK00_FRAME131),
    { { -13, -27, 19, 40 } },
};
const struct sprite_frame_1box gSpriteBank00Frame132 = {
    SPRITE_FRAME(gSpriteBank00Frame132, SPRITE_TILES_BANK00_FRAME132),
    { { -13, -28, 19, 41 } },
};
const struct sprite_frame_1box gSpriteBank00Frame133 = {
    SPRITE_FRAME(gSpriteBank00Frame133, SPRITE_TILES_BANK00_FRAME133),
    { { -13, -26, 19, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame134 = {
    SPRITE_FRAME(gSpriteBank00Frame134, SPRITE_TILES_BANK00_FRAME134),
    { { -14, -27, 20, 40 } },
};
const struct sprite_frame_1box gSpriteBank00Frame135 = {
    SPRITE_FRAME(gSpriteBank00Frame135, SPRITE_TILES_BANK00_FRAME135),
    { { -13, -26, 19, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame136 = {
    SPRITE_FRAME(gSpriteBank00Frame136, SPRITE_TILES_BANK00_FRAME136),
    { { -18, -8, 17, 17 } },
};
const struct sprite_frame_3box gSpriteBank00Frame137 = {
    SPRITE_FRAME(gSpriteBank00Frame137, SPRITE_TILES_BANK00_FRAME137),
    { { -18, -8, 17, 17 }, { -4, 2, 20, 10 }, { 5, 3, 12, 9 } },
};
const struct sprite_frame_3box gSpriteBank00Frame138 = {
    SPRITE_FRAME(gSpriteBank00Frame138, SPRITE_TILES_BANK00_FRAME138),
    { { -18, -8, 17, 17 }, { -5, 1, 22, 11 }, { 6, 4, 12, 7 } },
};
const struct sprite_frame_3box gSpriteBank00Frame139 = {
    SPRITE_FRAME(gSpriteBank00Frame139, SPRITE_TILES_BANK00_FRAME139),
    { { -18, -8, 17, 17 }, { -4, 2, 19, 11 }, { 7, 3, 9, 7 } },
};
const struct sprite_frame_1box gSpriteBank00Frame140 = {
    SPRITE_FRAME(gSpriteBank00Frame140, SPRITE_TILES_BANK00_FRAME140),
    { { -18, -8, 17, 17 } },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame141 = {
    SPRITE_FRAME(gSpriteBank00Frame141, SPRITE_TILES_BANK00_FRAME141),
    { { 0, 0, 0, 0 }, { -18, -22, 32, 32 }, { -18, -8, 32, 16 } },
    { -4, -13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame142 = {
    SPRITE_FRAME(gSpriteBank00Frame142, SPRITE_TILES_BANK00_FRAME142),
    { { 0, 0, 0, 0 }, { -20, -20, 35, 29 }, { -18, -8, 32, 16 } },
    { -4, -13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame143 = {
    SPRITE_FRAME(gSpriteBank00Frame143, SPRITE_TILES_BANK00_FRAME143),
    { { 0, 0, 0, 0 }, { -20, -16, 32, 28 }, { -18, -8, 32, 16 } },
    { -4, -13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame144 = {
    SPRITE_FRAME(gSpriteBank00Frame144, SPRITE_TILES_BANK00_FRAME144),
    { { 0, 0, 0, 0 }, { -22, -14, 36, 27 }, { -18, -8, 32, 16 } },
    { -4, -13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame145 = {
    SPRITE_FRAME(gSpriteBank00Frame145, SPRITE_TILES_BANK00_FRAME145),
    { { 0, 0, 0, 0 }, { -24, -14, 39, 26 }, { -18, -8, 32, 16 } },
    { -4, -13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame146 = {
    SPRITE_FRAME(gSpriteBank00Frame146, SPRITE_TILES_BANK00_FRAME146),
    { { 0, 0, 0, 0 }, { -22, -14, 40, 27 }, { -18, -8, 32, 16 } },
    { -4, -13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame147 = {
    SPRITE_FRAME(gSpriteBank00Frame147, SPRITE_TILES_BANK00_FRAME147),
    { { 0, 0, 0, 0 }, { -20, -15, 36, 27 }, { -18, -8, 32, 16 } },
    { -4, -13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame148 = {
    SPRITE_FRAME(gSpriteBank00Frame148, SPRITE_TILES_BANK00_FRAME148),
    { { 0, 0, 0, 0 }, { -20, -15, 40, 27 }, { -18, -8, 32, 16 } },
    { -4, -13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame149 = {
    SPRITE_FRAME(gSpriteBank00Frame149, SPRITE_TILES_BANK00_FRAME149),
    { { 0, 0, 0, 0 }, { -21, -18, 35, 27 }, { -18, -8, 32, 16 } },
    { -4, -13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame150 = {
    SPRITE_FRAME(gSpriteBank00Frame150, SPRITE_TILES_BANK00_FRAME150),
    { { 0, 0, 0, 0 }, { -19, -21, 29, 32 }, { -18, -8, 32, 16 } },
    { -4, -13 },
};
const struct sprite_frame_3box gSpriteBank00Frame151 = {
    SPRITE_FRAME(gSpriteBank00Frame151, SPRITE_TILES_BANK00_FRAME151),
    { { -33, -2, 63, 25 }, { -22, 3, 38, 9 }, { -19, 6, 39, 10 } },
};
const struct sprite_frame_2box gSpriteBank00Frame152 = {
    SPRITE_FRAME(gSpriteBank00Frame152, SPRITE_TILES_BANK00_FRAME152),
    { { -31, -2, 58, 25 }, { -22, 3, 39, 10 } },
};
const struct sprite_frame_1box gSpriteBank00Frame153 = {
    SPRITE_FRAME(gSpriteBank00Frame153, SPRITE_TILES_BANK00_FRAME153),
    { { -29, -1, 52, 24 } },
};
const struct sprite_frame_1box gSpriteBank00Frame154 = {
    SPRITE_FRAME(gSpriteBank00Frame154, SPRITE_TILES_BANK00_FRAME154),
    { { -26, -1, 46, 22 } },
};
const struct sprite_frame_1box gSpriteBank00Frame155 = {
    SPRITE_FRAME(gSpriteBank00Frame155, SPRITE_TILES_BANK00_FRAME155),
    { { -26, -1, 43, 20 } },
};
const struct sprite_frame_1box gSpriteBank00Frame156 = {
    SPRITE_FRAME(gSpriteBank00Frame156, SPRITE_TILES_BANK00_FRAME156),
    { { -25, -5, 40, 23 } },
};
const struct sprite_frame_1box gSpriteBank00Frame157 = {
    SPRITE_FRAME(gSpriteBank00Frame157, SPRITE_TILES_BANK00_FRAME157),
    { { -25, -7, 40, 25 } },
};
const struct sprite_frame_1box gSpriteBank00Frame158 = {
    SPRITE_FRAME(gSpriteBank00Frame158, SPRITE_TILES_BANK00_FRAME158),
    { { -25, -8, 38, 26 } },
};
const struct sprite_frame_1box gSpriteBank00Frame159 = {
    SPRITE_FRAME(gSpriteBank00Frame159, SPRITE_TILES_BANK00_FRAME159),
    { { -24, -8, 36, 27 } },
};
const struct sprite_frame_1box gSpriteBank00Frame160 = {
    SPRITE_FRAME(gSpriteBank00Frame160, SPRITE_TILES_BANK00_FRAME160),
    { { -23, -7, 37, 26 } },
};
const struct sprite_frame_1box gSpriteBank00Frame161 = {
    SPRITE_FRAME(gSpriteBank00Frame161, SPRITE_TILES_BANK00_FRAME161),
    { { -21, -7, 37, 26 } },
};
const struct sprite_frame_1box gSpriteBank00Frame162 = {
    SPRITE_FRAME(gSpriteBank00Frame162, SPRITE_TILES_BANK00_FRAME162),
    { { -17, -13, 31, 26 } },
};
const struct sprite_frame_1box gSpriteBank00Frame163 = {
    SPRITE_FRAME(gSpriteBank00Frame163, SPRITE_TILES_BANK00_FRAME163),
    { { -15, -17, 27, 33 } },
};
const struct sprite_frame_1box gSpriteBank00Frame164 = {
    SPRITE_FRAME(gSpriteBank00Frame164, SPRITE_TILES_BANK00_FRAME164),
    { { -14, -20, 24, 35 } },
};
const struct sprite_frame_1box gSpriteBank00Frame165 = {
    SPRITE_FRAME(gSpriteBank00Frame165, SPRITE_TILES_BANK00_FRAME165),
    { { -13, -21, 21, 35 } },
};
const struct sprite_frame_1box gSpriteBank00Frame166 = {
    SPRITE_FRAME(gSpriteBank00Frame166, SPRITE_TILES_BANK00_FRAME166),
    { { -12, -23, 21, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame167 = {
    SPRITE_FRAME(gSpriteBank00Frame167, SPRITE_TILES_BANK00_FRAME167),
    { { -11, -24, 20, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame168 = {
    SPRITE_FRAME(gSpriteBank00Frame168, SPRITE_TILES_BANK00_FRAME168),
    { { -9, -25, 19, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame169 = {
    SPRITE_FRAME(gSpriteBank00Frame169, SPRITE_TILES_BANK00_FRAME169),
    { { -10, -25, 20, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame170 = {
    SPRITE_FRAME(gSpriteBank00Frame170, SPRITE_TILES_BANK00_FRAME170),
    { { -10, -25, 20, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame171 = {
    SPRITE_FRAME(gSpriteBank00Frame171, SPRITE_TILES_BANK00_FRAME171),
    { { -7, -27, 14, 41 } },
};
const struct sprite_frame_1box gSpriteBank00Frame172 = {
    SPRITE_FRAME(gSpriteBank00Frame172, SPRITE_TILES_BANK00_FRAME172),
    { { -7, -27, 14, 41 } },
};
const struct sprite_frame_1box gSpriteBank00Frame173 = {
    SPRITE_FRAME(gSpriteBank00Frame173, SPRITE_TILES_BANK00_FRAME173),
    { { -7, -27, 14, 41 } },
};
const struct sprite_frame_1box gSpriteBank00Frame174 = {
    SPRITE_FRAME(gSpriteBank00Frame174, SPRITE_TILES_BANK00_FRAME174),
    { { -7, -27, 14, 41 } },
};
const struct sprite_frame_1box gSpriteBank00Frame175 = {
    SPRITE_FRAME(gSpriteBank00Frame175, SPRITE_TILES_BANK00_FRAME175),
    { { -7, -27, 14, 41 } },
};
const struct sprite_frame_1box gSpriteBank00Frame176 = {
    SPRITE_FRAME(gSpriteBank00Frame176, SPRITE_TILES_BANK00_FRAME176),
    { { -7, -27, 14, 41 } },
};
const struct sprite_frame_1box gSpriteBank00Frame177 = {
    SPRITE_FRAME(gSpriteBank00Frame177, SPRITE_TILES_BANK00_FRAME177),
    { { -7, -27, 14, 41 } },
};
const struct sprite_frame_1box gSpriteBank00Frame178 = {
    SPRITE_FRAME(gSpriteBank00Frame178, SPRITE_TILES_BANK00_FRAME178),
    { { -7, -27, 14, 41 } },
};
const struct sprite_frame_1box gSpriteBank00Frame179 = {
    SPRITE_FRAME(gSpriteBank00Frame179, SPRITE_TILES_BANK00_FRAME179),
    { { -7, -27, 14, 41 } },
};
const struct sprite_frame_1box gSpriteBank00Frame180 = {
    SPRITE_FRAME(gSpriteBank00Frame180, SPRITE_TILES_BANK00_FRAME180),
    { { -7, -27, 14, 41 } },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame181 = {
    SPRITE_FRAME(gSpriteBank00Frame181, SPRITE_TILES_BANK00_FRAME181),
    { { -9, -26, 14, 38 } },
    { -3, -33 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame182 = {
    SPRITE_FRAME(gSpriteBank00Frame182, SPRITE_TILES_BANK00_FRAME182),
    { { -14, -26, 21, 41 } },
    { -3, -33 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame183 = {
    SPRITE_FRAME(gSpriteBank00Frame183, SPRITE_TILES_BANK00_FRAME183),
    { { -15, -30, 13, 23 }, { -7, -8, 7, 25 }, { 0, 0, 0, 0 } },
    { -3, -33 },
};
const struct sprite_frame_1box gSpriteBank00Frame184 = {
    SPRITE_FRAME(gSpriteBank00Frame184, SPRITE_TILES_BANK00_FRAME184),
    { { -7, -6, 15, 20 } },
};
const struct sprite_frame_1box gSpriteBank00Frame185 = {
    SPRITE_FRAME(gSpriteBank00Frame185, SPRITE_TILES_BANK00_FRAME185),
    { { -7, -6, 15, 20 } },
};
const struct sprite_frame_1box gSpriteBank00Frame186 = {
    SPRITE_FRAME(gSpriteBank00Frame186, SPRITE_TILES_BANK00_FRAME186),
    { { -7, -6, 15, 20 } },
};
const struct sprite_frame_2box gSpriteBank00Frame187 = {
    SPRITE_FRAME(gSpriteBank00Frame187, SPRITE_TILES_BANK00_FRAME187),
    { { -8, -24, 11, 25 }, { -7, 2, 9, 12 } },
};
const struct sprite_frame_2box gSpriteBank00Frame188 = {
    SPRITE_FRAME(gSpriteBank00Frame188, SPRITE_TILES_BANK00_FRAME188),
    { { -11, -24, 14, 21 }, { -7, -6, 9, 24 } },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame189 = {
    SPRITE_FRAME(gSpriteBank00Frame189, SPRITE_TILES_BANK00_FRAME189),
    { { -18, -29, 17, 25 }, { -7, -7, 7, 25 }, { 0, 0, 0, 0 } },
    { -2, -29 },
};
const struct sprite_frame_1box gSpriteBank00Frame190 = {
    SPRITE_FRAME(gSpriteBank00Frame190, SPRITE_TILES_BANK00_FRAME190),
    { { -3, -28, 9, 44 } },
};
const struct sprite_frame_1box gSpriteBank00Frame191 = {
    SPRITE_FRAME(gSpriteBank00Frame191, SPRITE_TILES_BANK00_FRAME191),
    { { -4, -24, 8, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame192 = {
    SPRITE_FRAME(gSpriteBank00Frame192, SPRITE_TILES_BANK00_FRAME192),
    { { -9, -18, 14, 31 } },
};
const struct sprite_frame_1box gSpriteBank00Frame193 = {
    SPRITE_FRAME(gSpriteBank00Frame193, SPRITE_TILES_BANK00_FRAME193),
    { { -11, -15, 18, 28 } },
};
const struct sprite_frame_1box gSpriteBank00Frame194 = {
    SPRITE_FRAME(gSpriteBank00Frame194, SPRITE_TILES_BANK00_FRAME194),
    { { -9, -14, 17, 27 } },
};
const struct sprite_frame_1box gSpriteBank00Frame195 = {
    SPRITE_FRAME(gSpriteBank00Frame195, SPRITE_TILES_BANK00_FRAME195),
    { { -11, -12, 19, 25 } },
};
const struct sprite_frame_1box gSpriteBank00Frame196 = {
    SPRITE_FRAME(gSpriteBank00Frame196, SPRITE_TILES_BANK00_FRAME196),
    { { -10, -9, 18, 22 } },
};
const struct sprite_frame_1box gSpriteBank00Frame197 = {
    SPRITE_FRAME(gSpriteBank00Frame197, SPRITE_TILES_BANK00_FRAME197),
    { { -3, -11, 11, 24 } },
};
const struct sprite_frame_1box gSpriteBank00Frame198 = {
    SPRITE_FRAME(gSpriteBank00Frame198, SPRITE_TILES_BANK00_FRAME198),
    { { 0, -14, 8, 27 } },
};
const struct sprite_frame_1box gSpriteBank00Frame199 = {
    SPRITE_FRAME(gSpriteBank00Frame199, SPRITE_TILES_BANK00_FRAME199),
    { { -1, -18, 7, 31 } },
};
const struct sprite_frame_1box gSpriteBank00Frame200 = {
    SPRITE_FRAME(gSpriteBank00Frame200, SPRITE_TILES_BANK00_FRAME200),
    { { -2, -22, 7, 35 } },
};
const struct sprite_frame_1box gSpriteBank00Frame201 = {
    SPRITE_FRAME(gSpriteBank00Frame201, SPRITE_TILES_BANK00_FRAME201),
    { { -4, -24, 8, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame202 = {
    SPRITE_FRAME(gSpriteBank00Frame202, SPRITE_TILES_BANK00_FRAME202),
    { { -4, -25, 8, 38 } },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame203 = {
    SPRITE_FRAME(gSpriteBank00Frame203, SPRITE_TILES_BANK00_FRAME203),
    { { -11, -22, 16, 9 }, { -15, -19, 27, 27 }, { -18, -8, 32, 15 } },
    { -4, -7 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame204 = {
    SPRITE_FRAME(gSpriteBank00Frame204, SPRITE_TILES_BANK00_FRAME204),
    { { -11, -22, 16, 9 }, { -16, -17, 28, 25 }, { -18, -8, 32, 15 } },
    { -4, -7 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame205 = {
    SPRITE_FRAME(gSpriteBank00Frame205, SPRITE_TILES_BANK00_FRAME205),
    { { -11, -17, 17, 9 }, { -21, -14, 33, 23 }, { -18, -8, 32, 15 } },
    { -4, -7 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame206 = {
    SPRITE_FRAME(gSpriteBank00Frame206, SPRITE_TILES_BANK00_FRAME206),
    { { -12, -15, 17, 7 }, { -19, -12, 31, 22 }, { -18, -8, 32, 15 } },
    { -4, -7 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame207 = {
    SPRITE_FRAME(gSpriteBank00Frame207, SPRITE_TILES_BANK00_FRAME207),
    { { -11, -17, 15, 6 }, { -21, -12, 34, 22 }, { -18, -8, 32, 15 } },
    { -4, -7 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame208 = {
    SPRITE_FRAME(gSpriteBank00Frame208, SPRITE_TILES_BANK00_FRAME208),
    { { -10, -15, 16, 5 }, { -19, -12, 35, 22 }, { -18, -8, 32, 15 } },
    { -4, -7 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame209 = {
    SPRITE_FRAME(gSpriteBank00Frame209, SPRITE_TILES_BANK00_FRAME209),
    { { -13, -15, 20, 8 }, { -18, -12, 32, 22 }, { -18, -8, 32, 15 } },
    { -4, -7 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame210 = {
    SPRITE_FRAME(gSpriteBank00Frame210, SPRITE_TILES_BANK00_FRAME210),
    { { -12, -16, 20, 8 }, { -18, -13, 34, 22 }, { -18, -8, 32, 15 } },
    { -4, -7 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame211 = {
    SPRITE_FRAME(gSpriteBank00Frame211, SPRITE_TILES_BANK00_FRAME211),
    { { -11, -22, 16, 9 }, { -19, -17, 29, 25 }, { -18, -8, 32, 15 } },
    { -4, -7 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame212 = {
    SPRITE_FRAME(gSpriteBank00Frame212, SPRITE_TILES_BANK00_FRAME212),
    { { -11, -22, 16, 9 }, { -18, -20, 26, 28 }, { -18, -8, 32, 15 } },
    { -4, -7 },
};
const struct sprite_frame_1box gSpriteBank00Frame213 = {
    SPRITE_FRAME(gSpriteBank00Frame213, SPRITE_TILES_BANK00_FRAME213),
    { { -7, -28, 14, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame214 = {
    SPRITE_FRAME(gSpriteBank00Frame214, SPRITE_TILES_BANK00_FRAME214),
    { { -7, -28, 14, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame215 = {
    SPRITE_FRAME(gSpriteBank00Frame215, SPRITE_TILES_BANK00_FRAME215),
    { { -7, -28, 14, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame216 = {
    SPRITE_FRAME(gSpriteBank00Frame216, SPRITE_TILES_BANK00_FRAME216),
    { { -7, -28, 14, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame217 = {
    SPRITE_FRAME(gSpriteBank00Frame217, SPRITE_TILES_BANK00_FRAME217),
    { { -7, -27, 14, 38 } },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame218 = {
    SPRITE_FRAME(gSpriteBank00Frame218, SPRITE_TILES_BANK00_FRAME218),
    { { -12, -29, 11, 23 }, { -8, -6, 8, 18 }, { 0, 0, 0, 0 } },
    { 1, -30 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame219 = {
    SPRITE_FRAME(gSpriteBank00Frame219, SPRITE_TILES_BANK00_FRAME219),
    { { -7, -24, 13, 16 } },
    { 7, -25 },
};
const struct sprite_frame_1box gSpriteBank00Frame220 = {
    SPRITE_FRAME(gSpriteBank00Frame220, SPRITE_TILES_BANK00_FRAME220),
    { { -8, -20, 16, 13 } },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame221 = {
    SPRITE_FRAME(gSpriteBank00Frame221, SPRITE_TILES_BANK00_FRAME221),
    { { 0, 0, 0, 0 }, { -10, -19, 28, 15 }, { -11, -4, 27, 5 } },
    { 0, 0 },
};
const struct sprite_frame_2box gSpriteBank00Frame222 = {
    SPRITE_FRAME(gSpriteBank00Frame222, SPRITE_TILES_BANK00_FRAME222),
    { { -9, -30, 13, 18 }, { -8, -8, 14, 11 } },
};
const struct sprite_frame_2box gSpriteBank00Frame223 = {
    SPRITE_FRAME(gSpriteBank00Frame223, SPRITE_TILES_BANK00_FRAME223),
    { { -13, -27, 14, 27 }, { -11, 1, 15, 13 } },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame224 = {
    SPRITE_FRAME(gSpriteBank00Frame224, SPRITE_TILES_BANK00_FRAME224),
    { { -14, -29, 12, 31 }, { -8, -3, 8, 17 }, { 0, 0, 0, 0 } },
    { 0, -30 },
};
const struct sprite_frame_1box gSpriteBank00Frame225 = {
    SPRITE_FRAME(gSpriteBank00Frame225, SPRITE_TILES_BANK00_FRAME225),
    { { -25, -26, 36, 41 } },
};
const struct sprite_frame_1box gSpriteBank00Frame226 = {
    SPRITE_FRAME(gSpriteBank00Frame226, SPRITE_TILES_BANK00_FRAME226),
    { { -21, -25, 33, 41 } },
};
const struct sprite_frame_1box gSpriteBank00Frame227 = {
    SPRITE_FRAME(gSpriteBank00Frame227, SPRITE_TILES_BANK00_FRAME227),
    { { -24, -26, 36, 41 } },
};
const struct sprite_frame_1box gSpriteBank00Frame228 = {
    SPRITE_FRAME(gSpriteBank00Frame228, SPRITE_TILES_BANK00_FRAME228),
    { { -23, -27, 34, 41 } },
};
const struct sprite_frame_1box gSpriteBank00Frame229 = {
    SPRITE_FRAME(gSpriteBank00Frame229, SPRITE_TILES_BANK00_FRAME229),
    { { -6, -28, 24, 41 } },
};
const struct sprite_frame_1box gSpriteBank00Frame230 = {
    SPRITE_FRAME(gSpriteBank00Frame230, SPRITE_TILES_BANK00_FRAME230),
    { { -6, -28, 23, 40 } },
};
const struct sprite_frame_1box gSpriteBank00Frame231 = {
    SPRITE_FRAME(gSpriteBank00Frame231, SPRITE_TILES_BANK00_FRAME231),
    { { -12, -27, 27, 40 } },
};
const struct sprite_frame_1box gSpriteBank00Frame232 = {
    SPRITE_FRAME(gSpriteBank00Frame232, SPRITE_TILES_BANK00_FRAME232),
    { { -24, -27, 35, 41 } },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame233 = {
    SPRITE_FRAME(gSpriteBank00Frame233, SPRITE_TILES_BANK00_FRAME233),
    { { -8, -25, 11, 37 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame234 = {
    SPRITE_FRAME(gSpriteBank00Frame234, SPRITE_TILES_BANK00_FRAME234),
    { { -7, -25, 12, 32 } },
    { -2, -29 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame235 = {
    SPRITE_FRAME(gSpriteBank00Frame235, SPRITE_TILES_BANK00_FRAME235),
    { { -15, -23, 22, 21 }, { -31, -19, 57, 24 }, { -21, -19, 30, 13 } },
    { -2, -29 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame236 = {
    SPRITE_FRAME(gSpriteBank00Frame236, SPRITE_TILES_BANK00_FRAME236),
    { { -15, -23, 26, 23 }, { -31, -19, 57, 24 }, { -21, -19, 34, 15 } },
    { -2, -29 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame237 = {
    SPRITE_FRAME(gSpriteBank00Frame237, SPRITE_TILES_BANK00_FRAME237),
    { { -15, -23, 29, 24 }, { -31, -19, 57, 24 }, { -21, -19, 37, 16 } },
    { -2, -29 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame238 = {
    SPRITE_FRAME(gSpriteBank00Frame238, SPRITE_TILES_BANK00_FRAME238),
    { { -14, -23, 30, 21 }, { -31, -19, 57, 24 }, { -20, -19, 38, 13 } },
    { -2, -29 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame239 = {
    SPRITE_FRAME(gSpriteBank00Frame239, SPRITE_TILES_BANK00_FRAME239),
    { { -15, -23, 26, 22 }, { -31, -19, 57, 24 }, { -21, -19, 34, 14 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame240 = {
    SPRITE_FRAME(gSpriteBank00Frame240, SPRITE_TILES_BANK00_FRAME240),
    { { -8, -23, 11, 27 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame241 = {
    SPRITE_FRAME(gSpriteBank00Frame241, SPRITE_TILES_BANK00_FRAME241),
    { { -6, -23, 11, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame242 = {
    SPRITE_FRAME(gSpriteBank00Frame242, SPRITE_TILES_BANK00_FRAME242),
    { { -4, -23, 9, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame243 = {
    SPRITE_FRAME(gSpriteBank00Frame243, SPRITE_TILES_BANK00_FRAME243),
    { { -4, -23, 10, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame244 = {
    SPRITE_FRAME(gSpriteBank00Frame244, SPRITE_TILES_BANK00_FRAME244),
    { { -4, -23, 10, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame245 = {
    SPRITE_FRAME(gSpriteBank00Frame245, SPRITE_TILES_BANK00_FRAME245),
    { { -4, -23, 10, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame246 = {
    SPRITE_FRAME(gSpriteBank00Frame246, SPRITE_TILES_BANK00_FRAME246),
    { { -4, -23, 10, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame247 = {
    SPRITE_FRAME(gSpriteBank00Frame247, SPRITE_TILES_BANK00_FRAME247),
    { { -5, -23, 10, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame248 = {
    SPRITE_FRAME(gSpriteBank00Frame248, SPRITE_TILES_BANK00_FRAME248),
    { { -5, -23, 10, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame249 = {
    SPRITE_FRAME(gSpriteBank00Frame249, SPRITE_TILES_BANK00_FRAME249),
    { { -7, -23, 10, 27 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame250 = {
    SPRITE_FRAME(gSpriteBank00Frame250, SPRITE_TILES_BANK00_FRAME250),
    { { -9, -23, 12, 27 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame251 = {
    SPRITE_FRAME(gSpriteBank00Frame251, SPRITE_TILES_BANK00_FRAME251),
    { { -11, -23, 13, 27 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame252 = {
    SPRITE_FRAME(gSpriteBank00Frame252, SPRITE_TILES_BANK00_FRAME252),
    { { -12, -23, 14, 26 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame253 = {
    SPRITE_FRAME(gSpriteBank00Frame253, SPRITE_TILES_BANK00_FRAME253),
    { { -12, -23, 14, 27 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame254 = {
    SPRITE_FRAME(gSpriteBank00Frame254, SPRITE_TILES_BANK00_FRAME254),
    { { -10, -23, 13, 27 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame255 = {
    SPRITE_FRAME(gSpriteBank00Frame255, SPRITE_TILES_BANK00_FRAME255),
    { { -7, -23, 13, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame256 = {
    SPRITE_FRAME(gSpriteBank00Frame256, SPRITE_TILES_BANK00_FRAME256),
    { { -8, -23, 15, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame257 = {
    SPRITE_FRAME(gSpriteBank00Frame257, SPRITE_TILES_BANK00_FRAME257),
    { { -9, -23, 19, 29 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame258 = {
    SPRITE_FRAME(gSpriteBank00Frame258, SPRITE_TILES_BANK00_FRAME258),
    { { -19, -23, 21, 26 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame259 = {
    SPRITE_FRAME(gSpriteBank00Frame259, SPRITE_TILES_BANK00_FRAME259),
    { { -20, -23, 21, 31 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame260 = {
    SPRITE_FRAME(gSpriteBank00Frame260, SPRITE_TILES_BANK00_FRAME260),
    { { -7, -23, 9, 33 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame261 = {
    SPRITE_FRAME(gSpriteBank00Frame261, SPRITE_TILES_BANK00_FRAME261),
    { { -1, -23, 5, 33 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame262 = {
    SPRITE_FRAME(gSpriteBank00Frame262, SPRITE_TILES_BANK00_FRAME262),
    { { -2, -23, 6, 32 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame263 = {
    SPRITE_FRAME(gSpriteBank00Frame263, SPRITE_TILES_BANK00_FRAME263),
    { { -4, -23, 10, 29 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame264 = {
    SPRITE_FRAME(gSpriteBank00Frame264, SPRITE_TILES_BANK00_FRAME264),
    { { -10, -23, 14, 25 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame265 = {
    SPRITE_FRAME(gSpriteBank00Frame265, SPRITE_TILES_BANK00_FRAME265),
    { { -13, -23, 14, 24 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame266 = {
    SPRITE_FRAME(gSpriteBank00Frame266, SPRITE_TILES_BANK00_FRAME266),
    { { -8, -23, 9, 31 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame267 = {
    SPRITE_FRAME(gSpriteBank00Frame267, SPRITE_TILES_BANK00_FRAME267),
    { { -6, -23, 8, 36 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame268 = {
    SPRITE_FRAME(gSpriteBank00Frame268, SPRITE_TILES_BANK00_FRAME268),
    { { -3, -23, 5, 38 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame269 = {
    SPRITE_FRAME(gSpriteBank00Frame269, SPRITE_TILES_BANK00_FRAME269),
    { { -7, -23, 10, 37 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame270 = {
    SPRITE_FRAME(gSpriteBank00Frame270, SPRITE_TILES_BANK00_FRAME270),
    { { -10, -23, 18, 32 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame271 = {
    SPRITE_FRAME(gSpriteBank00Frame271, SPRITE_TILES_BANK00_FRAME271),
    { { -13, -23, 20, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame272 = {
    SPRITE_FRAME(gSpriteBank00Frame272, SPRITE_TILES_BANK00_FRAME272),
    { { -19, -23, 20, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame273 = {
    SPRITE_FRAME(gSpriteBank00Frame273, SPRITE_TILES_BANK00_FRAME273),
    { { -7, -23, 10, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame274 = {
    SPRITE_FRAME(gSpriteBank00Frame274, SPRITE_TILES_BANK00_FRAME274),
    { { -4, -23, 8, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame275 = {
    SPRITE_FRAME(gSpriteBank00Frame275, SPRITE_TILES_BANK00_FRAME275),
    { { -7, -23, 11, 32 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame276 = {
    SPRITE_FRAME(gSpriteBank00Frame276, SPRITE_TILES_BANK00_FRAME276),
    { { -5, -23, 9, 31 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame277 = {
    SPRITE_FRAME(gSpriteBank00Frame277, SPRITE_TILES_BANK00_FRAME277),
    { { -3, -23, 8, 29 } },
    { -2, -29 },
};
const struct sprite_frame_1box gSpriteBank00Frame278 = {
    SPRITE_FRAME(gSpriteBank00Frame278, SPRITE_TILES_BANK00_FRAME278),
    { { -9, -22, 12, 34 } },
};
const struct sprite_frame_1box gSpriteBank00Frame279 = {
    SPRITE_FRAME(gSpriteBank00Frame279, SPRITE_TILES_BANK00_FRAME279),
    { { -9, -22, 12, 34 } },
};
const struct sprite_frame_1box gSpriteBank00Frame280 = {
    SPRITE_FRAME(gSpriteBank00Frame280, SPRITE_TILES_BANK00_FRAME280),
    { { -9, -22, 12, 34 } },
};
const struct sprite_frame_1box gSpriteBank00Frame281 = {
    SPRITE_FRAME(gSpriteBank00Frame281, SPRITE_TILES_BANK00_FRAME281),
    { { -9, -22, 12, 34 } },
};
const struct sprite_frame_1box gSpriteBank00Frame282 = {
    SPRITE_FRAME(gSpriteBank00Frame282, SPRITE_TILES_BANK00_FRAME282),
    { { -9, -22, 12, 34 } },
};
const struct sprite_frame_1box gSpriteBank00Frame283 = {
    SPRITE_FRAME(gSpriteBank00Frame283, SPRITE_TILES_BANK00_FRAME283),
    { { -9, -22, 12, 34 } },
};
const struct sprite_frame_1box gSpriteBank00Frame284 = {
    SPRITE_FRAME(gSpriteBank00Frame284, SPRITE_TILES_BANK00_FRAME284),
    { { -9, -22, 12, 34 } },
};
const struct sprite_frame_1box gSpriteBank00Frame285 = {
    SPRITE_FRAME(gSpriteBank00Frame285, SPRITE_TILES_BANK00_FRAME285),
    { { -9, -22, 12, 34 } },
};
const struct sprite_frame_1box gSpriteBank00Frame286 = {
    SPRITE_FRAME(gSpriteBank00Frame286, SPRITE_TILES_BANK00_FRAME286),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame287 = {
    SPRITE_FRAME(gSpriteBank00Frame287, SPRITE_TILES_BANK00_FRAME287),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame288 = {
    SPRITE_FRAME(gSpriteBank00Frame288, SPRITE_TILES_BANK00_FRAME288),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame289 = {
    SPRITE_FRAME(gSpriteBank00Frame289, SPRITE_TILES_BANK00_FRAME289),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame290 = {
    SPRITE_FRAME(gSpriteBank00Frame290, SPRITE_TILES_BANK00_FRAME290),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame291 = {
    SPRITE_FRAME(gSpriteBank00Frame291, SPRITE_TILES_BANK00_FRAME291),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame292 = {
    SPRITE_FRAME(gSpriteBank00Frame292, SPRITE_TILES_BANK00_FRAME292),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame293 = {
    SPRITE_FRAME(gSpriteBank00Frame293, SPRITE_TILES_BANK00_FRAME293),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame294 = {
    SPRITE_FRAME(gSpriteBank00Frame294, SPRITE_TILES_BANK00_FRAME294),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame295 = {
    SPRITE_FRAME(gSpriteBank00Frame295, SPRITE_TILES_BANK00_FRAME295),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame296 = {
    SPRITE_FRAME(gSpriteBank00Frame296, SPRITE_TILES_BANK00_FRAME296),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame297 = {
    SPRITE_FRAME(gSpriteBank00Frame297, SPRITE_TILES_BANK00_FRAME297),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame298 = {
    SPRITE_FRAME(gSpriteBank00Frame298, SPRITE_TILES_BANK00_FRAME298),
    { { 0, 0, 0, 0 }, { -15, -19, 27, 27 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame299 = {
    SPRITE_FRAME(gSpriteBank00Frame299, SPRITE_TILES_BANK00_FRAME299),
    { { 0, 0, 0, 0 }, { -16, -17, 28, 25 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame300 = {
    SPRITE_FRAME(gSpriteBank00Frame300, SPRITE_TILES_BANK00_FRAME300),
    { { 0, 0, 0, 0 }, { -21, -14, 33, 23 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame301 = {
    SPRITE_FRAME(gSpriteBank00Frame301, SPRITE_TILES_BANK00_FRAME301),
    { { 0, 0, 0, 0 }, { -19, -12, 31, 22 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame302 = {
    SPRITE_FRAME(gSpriteBank00Frame302, SPRITE_TILES_BANK00_FRAME302),
    { { 0, 0, 0, 0 }, { -21, -12, 34, 22 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame303 = {
    SPRITE_FRAME(gSpriteBank00Frame303, SPRITE_TILES_BANK00_FRAME303),
    { { 0, 0, 0, 0 }, { -19, -12, 35, 22 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame304 = {
    SPRITE_FRAME(gSpriteBank00Frame304, SPRITE_TILES_BANK00_FRAME304),
    { { 0, 0, 0, 0 }, { -18, -12, 32, 22 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame305 = {
    SPRITE_FRAME(gSpriteBank00Frame305, SPRITE_TILES_BANK00_FRAME305),
    { { 0, 0, 0, 0 }, { -18, -13, 34, 22 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame306 = {
    SPRITE_FRAME(gSpriteBank00Frame306, SPRITE_TILES_BANK00_FRAME306),
    { { 0, 0, 0, 0 }, { -19, -17, 29, 25 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame307 = {
    SPRITE_FRAME(gSpriteBank00Frame307, SPRITE_TILES_BANK00_FRAME307),
    { { 0, 0, 0, 0 }, { -18, -20, 26, 28 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame308 = {
    SPRITE_FRAME(gSpriteBank00Frame308, SPRITE_TILES_BANK00_FRAME308),
    { { 0, 0, 0, 0 }, { -15, -19, 27, 27 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame309 = {
    SPRITE_FRAME(gSpriteBank00Frame309, SPRITE_TILES_BANK00_FRAME309),
    { { 0, 0, 0, 0 }, { -16, -17, 28, 25 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame310 = {
    SPRITE_FRAME(gSpriteBank00Frame310, SPRITE_TILES_BANK00_FRAME310),
    { { 0, 0, 0, 0 }, { -21, -14, 33, 23 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame311 = {
    SPRITE_FRAME(gSpriteBank00Frame311, SPRITE_TILES_BANK00_FRAME311),
    { { 0, 0, 0, 0 }, { -19, -12, 31, 22 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame312 = {
    SPRITE_FRAME(gSpriteBank00Frame312, SPRITE_TILES_BANK00_FRAME312),
    { { 0, 0, 0, 0 }, { -21, -12, 34, 22 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame313 = {
    SPRITE_FRAME(gSpriteBank00Frame313, SPRITE_TILES_BANK00_FRAME313),
    { { 0, 0, 0, 0 }, { -19, -12, 35, 22 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame314 = {
    SPRITE_FRAME(gSpriteBank00Frame314, SPRITE_TILES_BANK00_FRAME314),
    { { 0, 0, 0, 0 }, { -18, -12, 32, 22 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame315 = {
    SPRITE_FRAME(gSpriteBank00Frame315, SPRITE_TILES_BANK00_FRAME315),
    { { 0, 0, 0, 0 }, { -18, -13, 34, 22 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame316 = {
    SPRITE_FRAME(gSpriteBank00Frame316, SPRITE_TILES_BANK00_FRAME316),
    { { 0, 0, 0, 0 }, { -19, -17, 29, 25 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame317 = {
    SPRITE_FRAME(gSpriteBank00Frame317, SPRITE_TILES_BANK00_FRAME317),
    { { 0, 0, 0, 0 }, { -18, -20, 26, 28 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame_1box gSpriteBank00Frame318 = {
    SPRITE_FRAME(gSpriteBank00Frame318, SPRITE_TILES_BANK00_FRAME318),
    { { -10, -10, 20, 20 } },
};
const struct sprite_frame_1box gSpriteBank00Frame319 = {
    SPRITE_FRAME(gSpriteBank00Frame319, SPRITE_TILES_BANK00_FRAME319),
    { { -10, -10, 21, 17 } },
};
const struct sprite_frame_1box gSpriteBank00Frame320 = {
    SPRITE_FRAME(gSpriteBank00Frame320, SPRITE_TILES_BANK00_FRAME320),
    { { -10, -10, 20, 15 } },
};
const struct sprite_frame_1box gSpriteBank00Frame321 = {
    SPRITE_FRAME(gSpriteBank00Frame321, SPRITE_TILES_BANK00_FRAME321),
    { { -11, -10, 20, 16 } },
};
const struct sprite_frame_1box gSpriteBank00Frame322 = {
    SPRITE_FRAME(gSpriteBank00Frame322, SPRITE_TILES_BANK00_FRAME322),
    { { -11, -9, 17, 18 } },
};
const struct sprite_frame_1box gSpriteBank00Frame323 = {
    SPRITE_FRAME(gSpriteBank00Frame323, SPRITE_TILES_BANK00_FRAME323),
    { { -12, -9, 18, 19 } },
};
const struct sprite_frame_1box gSpriteBank00Frame324 = {
    SPRITE_FRAME(gSpriteBank00Frame324, SPRITE_TILES_BANK00_FRAME324),
    { { -13, -9, 20, 18 } },
};
const struct sprite_frame_1box gSpriteBank00Frame325 = {
    SPRITE_FRAME(gSpriteBank00Frame325, SPRITE_TILES_BANK00_FRAME325),
    { { -13, -9, 20, 18 } },
};
const struct sprite_frame_1box gSpriteBank00Frame326 = {
    SPRITE_FRAME(gSpriteBank00Frame326, SPRITE_TILES_BANK00_FRAME326),
    { { -13, -9, 20, 18 } },
};
const struct sprite_frame_1box gSpriteBank00Frame327 = {
    SPRITE_FRAME(gSpriteBank00Frame327, SPRITE_TILES_BANK00_FRAME327),
    { { -13, -9, 20, 18 } },
};
const struct sprite_frame_1box gSpriteBank00Frame328 = {
    SPRITE_FRAME(gSpriteBank00Frame328, SPRITE_TILES_BANK00_FRAME328),
    { { -11, -19, 23, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame329 = {
    SPRITE_FRAME(gSpriteBank00Frame329, SPRITE_TILES_BANK00_FRAME329),
    { { -10, -19, 22, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame330 = {
    SPRITE_FRAME(gSpriteBank00Frame330, SPRITE_TILES_BANK00_FRAME330),
    { { -10, -19, 22, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame331 = {
    SPRITE_FRAME(gSpriteBank00Frame331, SPRITE_TILES_BANK00_FRAME331),
    { { -11, -19, 23, 40 } },
};
const struct sprite_frame_1box gSpriteBank00Frame332 = {
    SPRITE_FRAME(gSpriteBank00Frame332, SPRITE_TILES_BANK00_FRAME332),
    { { -13, -19, 25, 40 } },
};
const struct sprite_frame_1box gSpriteBank00Frame333 = {
    SPRITE_FRAME(gSpriteBank00Frame333, SPRITE_TILES_BANK00_FRAME333),
    { { -14, -18, 25, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame334 = {
    SPRITE_FRAME(gSpriteBank00Frame334, SPRITE_TILES_BANK00_FRAME334),
    { { -14, -15, 25, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame335 = {
    SPRITE_FRAME(gSpriteBank00Frame335, SPRITE_TILES_BANK00_FRAME335),
    { { -14, -9, 25, 30 } },
};
const struct sprite_frame_1box gSpriteBank00Frame336 = {
    SPRITE_FRAME(gSpriteBank00Frame336, SPRITE_TILES_BANK00_FRAME336),
    { { -15, -14, 28, 35 } },
};
const struct sprite_frame_1box gSpriteBank00Frame337 = {
    SPRITE_FRAME(gSpriteBank00Frame337, SPRITE_TILES_BANK00_FRAME337),
    { { -15, -11, 30, 32 } },
};
const struct sprite_frame_1box gSpriteBank00Frame338 = {
    SPRITE_FRAME(gSpriteBank00Frame338, SPRITE_TILES_BANK00_FRAME338),
    { { -15, -10, 30, 31 } },
};
const struct sprite_frame_1box gSpriteBank00Frame339 = {
    SPRITE_FRAME(gSpriteBank00Frame339, SPRITE_TILES_BANK00_FRAME339),
    { { -14, -7, 25, 28 } },
};
const struct sprite_frame_1box gSpriteBank00Frame340 = {
    SPRITE_FRAME(gSpriteBank00Frame340, SPRITE_TILES_BANK00_FRAME340),
    { { -14, -4, 25, 25 } },
};
const struct sprite_frame_1box gSpriteBank00Frame341 = {
    SPRITE_FRAME(gSpriteBank00Frame341, SPRITE_TILES_BANK00_FRAME341),
    { { -14, 8, 25, 13 } },
};
const struct sprite_frame_1box gSpriteBank00Frame342 = {
    SPRITE_FRAME(gSpriteBank00Frame342, SPRITE_TILES_BANK00_FRAME342),
    { { -10, -21, 20, 43 } },
};
const struct sprite_frame_1box gSpriteBank00Frame343 = {
    SPRITE_FRAME(gSpriteBank00Frame343, SPRITE_TILES_BANK00_FRAME343),
    { { -8, -18, 18, 40 } },
};
const struct sprite_frame_1box gSpriteBank00Frame344 = {
    SPRITE_FRAME(gSpriteBank00Frame344, SPRITE_TILES_BANK00_FRAME344),
    { { -8, -15, 17, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame345 = {
    SPRITE_FRAME(gSpriteBank00Frame345, SPRITE_TILES_BANK00_FRAME345),
    { { -18, -17, 28, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame346 = {
    SPRITE_FRAME(gSpriteBank00Frame346, SPRITE_TILES_BANK00_FRAME346),
    { { -19, -23, 34, 45 } },
};
const struct sprite_frame_1box gSpriteBank00Frame347 = {
    SPRITE_FRAME(gSpriteBank00Frame347, SPRITE_TILES_BANK00_FRAME347),
    { { -23, -27, 38, 49 } },
};
const struct sprite_frame_1box gSpriteBank00Frame348 = {
    SPRITE_FRAME(gSpriteBank00Frame348, SPRITE_TILES_BANK00_FRAME348),
    { { -15, -16, 39, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame349 = {
    SPRITE_FRAME(gSpriteBank00Frame349, SPRITE_TILES_BANK00_FRAME349),
    { { -15, -4, 37, 26 } },
};
const struct sprite_frame_1box gSpriteBank00Frame350 = {
    SPRITE_FRAME(gSpriteBank00Frame350, SPRITE_TILES_BANK00_FRAME350),
    { { -15, -7, 40, 29 } },
};
const struct sprite_frame_1box gSpriteBank00Frame351 = {
    SPRITE_FRAME(gSpriteBank00Frame351, SPRITE_TILES_BANK00_FRAME351),
    { { -15, 4, 39, 18 } },
};
const struct sprite_frame_1box gSpriteBank00Frame352 = {
    SPRITE_FRAME(gSpriteBank00Frame352, SPRITE_TILES_BANK00_FRAME352),
    { { -11, -20, 14, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame353 = {
    SPRITE_FRAME(gSpriteBank00Frame353, SPRITE_TILES_BANK00_FRAME353),
    { { -11, -20, 14, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame354 = {
    SPRITE_FRAME(gSpriteBank00Frame354, SPRITE_TILES_BANK00_FRAME354),
    { { -11, -20, 14, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame355 = {
    SPRITE_FRAME(gSpriteBank00Frame355, SPRITE_TILES_BANK00_FRAME355),
    { { -11, -20, 14, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame356 = {
    SPRITE_FRAME(gSpriteBank00Frame356, SPRITE_TILES_BANK00_FRAME356),
    { { -11, -20, 14, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame357 = {
    SPRITE_FRAME(gSpriteBank00Frame357, SPRITE_TILES_BANK00_FRAME357),
    { { -11, -20, 14, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame358 = {
    SPRITE_FRAME(gSpriteBank00Frame358, SPRITE_TILES_BANK00_FRAME358),
    { { -11, -20, 14, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame359 = {
    SPRITE_FRAME(gSpriteBank00Frame359, SPRITE_TILES_BANK00_FRAME359),
    { { -11, -20, 14, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame360 = {
    SPRITE_FRAME(gSpriteBank00Frame360, SPRITE_TILES_BANK00_FRAME360),
    { { -11, -20, 14, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame361 = {
    SPRITE_FRAME(gSpriteBank00Frame361, SPRITE_TILES_BANK00_FRAME361),
    { { -11, -20, 14, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame362 = {
    SPRITE_FRAME(gSpriteBank00Frame362, SPRITE_TILES_BANK00_FRAME362),
    { { -11, -20, 14, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame363 = {
    SPRITE_FRAME(gSpriteBank00Frame363, SPRITE_TILES_BANK00_FRAME363),
    { { -11, -20, 14, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame364 = {
    SPRITE_FRAME(gSpriteBank00Frame364, SPRITE_TILES_BANK00_FRAME364),
    { { -11, -20, 14, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame365 = {
    SPRITE_FRAME(gSpriteBank00Frame365, SPRITE_TILES_BANK00_FRAME365),
    { { -11, -20, 14, 39 } },
};
const struct sprite_frame_2box gSpriteBank00Frame366 = {
    SPRITE_FRAME(gSpriteBank00Frame366, SPRITE_TILES_BANK00_FRAME366),
    { { -8, -22, 10, 39 }, { -8, -3, 11, 24 } },
};
const struct sprite_frame gSpriteBank00Frame367 = SPRITE_FRAME(gSpriteBank00Frame367, SPRITE_TILES_BANK00_FRAME367);
const struct sprite_frame gSpriteBank00Frame368 = SPRITE_FRAME(gSpriteBank00Frame368, SPRITE_TILES_BANK00_FRAME368);
const struct sprite_frame gSpriteBank00Frame369 = SPRITE_FRAME(gSpriteBank00Frame369, SPRITE_TILES_BANK00_FRAME369);
const struct sprite_frame gSpriteBank00Frame370 = SPRITE_FRAME(gSpriteBank00Frame370, SPRITE_TILES_BANK00_FRAME370);
const struct sprite_frame gSpriteBank00Frame371 = SPRITE_FRAME(gSpriteBank00Frame371, SPRITE_TILES_BANK00_FRAME371);
const struct sprite_frame gSpriteBank00Frame372 = SPRITE_FRAME(gSpriteBank00Frame372, SPRITE_TILES_BANK00_FRAME372);
const struct sprite_frame gSpriteBank00Frame373 = SPRITE_FRAME(gSpriteBank00Frame373, SPRITE_TILES_BANK00_FRAME373);
const struct sprite_frame gSpriteBank00Frame374 = SPRITE_FRAME(gSpriteBank00Frame374, SPRITE_TILES_BANK00_FRAME374);
const struct sprite_frame gSpriteBank00Frame375 = SPRITE_FRAME(gSpriteBank00Frame375, SPRITE_TILES_BANK00_FRAME375);
const struct sprite_frame gSpriteBank00Frame376 = SPRITE_FRAME(gSpriteBank00Frame376, SPRITE_TILES_BANK00_FRAME376);
const struct sprite_frame gSpriteBank00Frame377 = SPRITE_FRAME(gSpriteBank00Frame377, SPRITE_TILES_BANK00_FRAME377);
const struct sprite_frame gSpriteBank00Frame378 = SPRITE_FRAME(gSpriteBank00Frame378, SPRITE_TILES_BANK00_FRAME378);
const struct sprite_frame gSpriteBank00Frame379 = SPRITE_FRAME(gSpriteBank00Frame379, SPRITE_TILES_BANK00_FRAME379);
const struct sprite_frame gSpriteBank00Frame380 = SPRITE_FRAME(gSpriteBank00Frame380, SPRITE_TILES_BANK00_FRAME380);
const struct sprite_frame gSpriteBank00Frame381 = SPRITE_FRAME(gSpriteBank00Frame381, SPRITE_TILES_BANK00_FRAME381);
const struct sprite_frame gSpriteBank00Frame382 = SPRITE_FRAME(gSpriteBank00Frame382, SPRITE_TILES_BANK00_FRAME382);
const struct sprite_frame gSpriteBank00Frame383 = SPRITE_FRAME(gSpriteBank00Frame383, SPRITE_TILES_BANK00_FRAME383);
const struct sprite_frame gSpriteBank00Frame384 = SPRITE_FRAME(gSpriteBank00Frame384, SPRITE_TILES_BANK00_FRAME384);
const struct sprite_frame gSpriteBank00Frame385 = SPRITE_FRAME(gSpriteBank00Frame385, SPRITE_TILES_BANK00_FRAME385);
const struct sprite_frame gSpriteBank00Frame386 = SPRITE_FRAME(gSpriteBank00Frame386, SPRITE_TILES_BANK00_FRAME386);
const struct sprite_frame gSpriteBank00Frame387 = SPRITE_FRAME(gSpriteBank00Frame387, SPRITE_TILES_BANK00_FRAME387);
const struct sprite_frame gSpriteBank00Frame388 = SPRITE_FRAME(gSpriteBank00Frame388, SPRITE_TILES_BANK00_FRAME388);
const struct sprite_frame gSpriteBank00Frame389 = SPRITE_FRAME(gSpriteBank00Frame389, SPRITE_TILES_BANK00_FRAME389);
const struct sprite_frame gSpriteBank00Frame390 = SPRITE_FRAME(gSpriteBank00Frame390, SPRITE_TILES_BANK00_FRAME390);
const struct sprite_frame gSpriteBank00Frame391 = SPRITE_FRAME(gSpriteBank00Frame391, SPRITE_TILES_BANK00_FRAME391);
const struct sprite_frame_3box_anchor gSpriteBank00Frame392 = {
    SPRITE_FRAME(gSpriteBank00Frame392, SPRITE_TILES_BANK00_FRAME392),
    { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { -8, -33 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame393 = {
    SPRITE_FRAME(gSpriteBank00Frame393, SPRITE_TILES_BANK00_FRAME393),
    { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { -8, -33 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame394 = {
    SPRITE_FRAME(gSpriteBank00Frame394, SPRITE_TILES_BANK00_FRAME394),
    { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { -8, -33 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame395 = {
    SPRITE_FRAME(gSpriteBank00Frame395, SPRITE_TILES_BANK00_FRAME395),
    { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { -8, -33 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame396 = {
    SPRITE_FRAME(gSpriteBank00Frame396, SPRITE_TILES_BANK00_FRAME396),
    { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { -8, -33 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame397 = {
    SPRITE_FRAME(gSpriteBank00Frame397, SPRITE_TILES_BANK00_FRAME397),
    { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { -8, -33 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame398 = {
    SPRITE_FRAME(gSpriteBank00Frame398, SPRITE_TILES_BANK00_FRAME398),
    { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { -8, -33 },
};
const struct sprite_frame gSpriteBank00Frame399 = SPRITE_FRAME(gSpriteBank00Frame399, SPRITE_TILES_BANK00_FRAME399);
const struct sprite_frame gSpriteBank00Frame400 = SPRITE_FRAME(gSpriteBank00Frame400, SPRITE_TILES_BANK00_FRAME400);
const struct sprite_frame gSpriteBank00Frame401 = SPRITE_FRAME(gSpriteBank00Frame401, SPRITE_TILES_BANK00_FRAME401);
const struct sprite_frame gSpriteBank00Frame402 = SPRITE_FRAME(gSpriteBank00Frame402, SPRITE_TILES_BANK00_FRAME402);
const struct sprite_frame gSpriteBank00Frame403 = SPRITE_FRAME(gSpriteBank00Frame403, SPRITE_TILES_BANK00_FRAME403);
const struct sprite_frame gSpriteBank00Frame404 = SPRITE_FRAME(gSpriteBank00Frame404, SPRITE_TILES_BANK00_FRAME404);
const struct sprite_frame gSpriteBank00Frame405 = SPRITE_FRAME(gSpriteBank00Frame405, SPRITE_TILES_BANK00_FRAME405);
const struct sprite_frame gSpriteBank00Frame406 = SPRITE_FRAME(gSpriteBank00Frame406, SPRITE_TILES_BANK00_FRAME406);
const struct sprite_frame gSpriteBank00Frame407 = SPRITE_FRAME(gSpriteBank00Frame407, SPRITE_TILES_BANK00_FRAME407);
const struct sprite_frame gSpriteBank00Frame408 = SPRITE_FRAME(gSpriteBank00Frame408, SPRITE_TILES_BANK00_FRAME408);
const struct sprite_frame gSpriteBank00Frame409 = SPRITE_FRAME(gSpriteBank00Frame409, SPRITE_TILES_BANK00_FRAME409);
const struct sprite_frame gSpriteBank00Frame410 = SPRITE_FRAME(gSpriteBank00Frame410, SPRITE_TILES_BANK00_FRAME410);
const struct sprite_frame gSpriteBank00Frame411 = SPRITE_FRAME(gSpriteBank00Frame411, SPRITE_TILES_BANK00_FRAME411);
const struct sprite_frame gSpriteBank00Frame412 = SPRITE_FRAME(gSpriteBank00Frame412, SPRITE_TILES_BANK00_FRAME412);
const struct sprite_frame gSpriteBank00Frame413 = SPRITE_FRAME(gSpriteBank00Frame413, SPRITE_TILES_BANK00_FRAME413);
const struct sprite_frame gSpriteBank00Frame414 = SPRITE_FRAME(gSpriteBank00Frame414, SPRITE_TILES_BANK00_FRAME414);
const struct sprite_frame gSpriteBank00Frame415 = SPRITE_FRAME(gSpriteBank00Frame415, SPRITE_TILES_BANK00_FRAME415);
const struct sprite_frame gSpriteBank00Frame416 = SPRITE_FRAME(gSpriteBank00Frame416, SPRITE_TILES_BANK00_FRAME416);
const struct sprite_frame gSpriteBank00Frame417 = SPRITE_FRAME(gSpriteBank00Frame417, SPRITE_TILES_BANK00_FRAME417);
const struct sprite_frame gSpriteBank00Frame418 = SPRITE_FRAME(gSpriteBank00Frame418, SPRITE_TILES_BANK00_FRAME418);
const struct sprite_frame gSpriteBank00Frame419 = SPRITE_FRAME(gSpriteBank00Frame419, SPRITE_TILES_BANK00_FRAME419);
const struct sprite_frame gSpriteBank00Frame420 = SPRITE_FRAME(gSpriteBank00Frame420, SPRITE_TILES_BANK00_FRAME420);
const struct sprite_frame gSpriteBank00Frame421 = SPRITE_FRAME(gSpriteBank00Frame421, SPRITE_TILES_BANK00_FRAME421);
const struct sprite_frame gSpriteBank00Frame422 = SPRITE_FRAME(gSpriteBank00Frame422, SPRITE_TILES_BANK00_FRAME422);
const struct sprite_frame gSpriteBank00Frame423 = SPRITE_FRAME(gSpriteBank00Frame423, SPRITE_TILES_BANK00_FRAME423);
const struct sprite_frame gSpriteBank00Frame424 = SPRITE_FRAME(gSpriteBank00Frame424, SPRITE_TILES_BANK00_FRAME424);
const struct sprite_frame gSpriteBank00Frame425 = SPRITE_FRAME(gSpriteBank00Frame425, SPRITE_TILES_BANK00_FRAME425);
const struct sprite_frame gSpriteBank00Frame426 = SPRITE_FRAME(gSpriteBank00Frame426, SPRITE_TILES_BANK00_FRAME426);
const struct sprite_frame gSpriteBank00Frame427 = SPRITE_FRAME(gSpriteBank00Frame427, SPRITE_TILES_BANK00_FRAME427);
const struct sprite_frame gSpriteBank00Frame428 = SPRITE_FRAME(gSpriteBank00Frame428, SPRITE_TILES_BANK00_FRAME428);
const struct sprite_frame gSpriteBank00Frame429 = SPRITE_FRAME(gSpriteBank00Frame429, SPRITE_TILES_BANK00_FRAME429);
const struct sprite_frame gSpriteBank00Frame430 = SPRITE_FRAME(gSpriteBank00Frame430, SPRITE_TILES_BANK00_FRAME430);
const struct sprite_frame gSpriteBank00Frame431 = SPRITE_FRAME(gSpriteBank00Frame431, SPRITE_TILES_BANK00_FRAME431);
const struct sprite_frame gSpriteBank00Frame432 = SPRITE_FRAME(gSpriteBank00Frame432, SPRITE_TILES_BANK00_FRAME432);
const struct sprite_frame gSpriteBank00Frame433 = SPRITE_FRAME(gSpriteBank00Frame433, SPRITE_TILES_BANK00_FRAME433);
const struct sprite_frame gSpriteBank00Frame434 = SPRITE_FRAME(gSpriteBank00Frame434, SPRITE_TILES_BANK00_FRAME434);
const struct sprite_frame gSpriteBank00Frame435 = SPRITE_FRAME(gSpriteBank00Frame435, SPRITE_TILES_BANK00_FRAME435);
const struct sprite_frame gSpriteBank00Frame436 = SPRITE_FRAME(gSpriteBank00Frame436, SPRITE_TILES_BANK00_FRAME436);
const struct sprite_frame gSpriteBank00Frame437 = SPRITE_FRAME(gSpriteBank00Frame437, SPRITE_TILES_BANK00_FRAME437);
const struct sprite_frame gSpriteBank00Frame438 = SPRITE_FRAME(gSpriteBank00Frame438, SPRITE_TILES_BANK00_FRAME438);
const struct sprite_frame gSpriteBank00Frame439 = SPRITE_FRAME(gSpriteBank00Frame439, SPRITE_TILES_BANK00_FRAME439);
const struct sprite_frame gSpriteBank00Frame440 = SPRITE_FRAME(gSpriteBank00Frame440, SPRITE_TILES_BANK00_FRAME440);
const struct sprite_frame gSpriteBank00Frame441 = SPRITE_FRAME(gSpriteBank00Frame441, SPRITE_TILES_BANK00_FRAME441);
const struct sprite_frame gSpriteBank00Frame442 = SPRITE_FRAME(gSpriteBank00Frame442, SPRITE_TILES_BANK00_FRAME442);
const struct sprite_frame gSpriteBank00Frame443 = SPRITE_FRAME(gSpriteBank00Frame443, SPRITE_TILES_BANK00_FRAME443);
const struct sprite_frame gSpriteBank00Frame444 = SPRITE_FRAME(gSpriteBank00Frame444, SPRITE_TILES_BANK00_FRAME444);
const struct sprite_frame gSpriteBank00Frame445 = SPRITE_FRAME(gSpriteBank00Frame445, SPRITE_TILES_BANK00_FRAME445);
const struct sprite_frame gSpriteBank00Frame446 = SPRITE_FRAME(gSpriteBank00Frame446, SPRITE_TILES_BANK00_FRAME446);
const struct sprite_frame gSpriteBank00Frame447 = SPRITE_FRAME(gSpriteBank00Frame447, SPRITE_TILES_BANK00_FRAME447);
const struct sprite_frame gSpriteBank00Frame448 = SPRITE_FRAME(gSpriteBank00Frame448, SPRITE_TILES_BANK00_FRAME448);
const struct sprite_frame gSpriteBank00Frame449 = SPRITE_FRAME(gSpriteBank00Frame449, SPRITE_TILES_BANK00_FRAME449);
const struct sprite_frame gSpriteBank00Frame450 = SPRITE_FRAME(gSpriteBank00Frame450, SPRITE_TILES_BANK00_FRAME450);
const struct sprite_frame gSpriteBank00Frame451 = SPRITE_FRAME(gSpriteBank00Frame451, SPRITE_TILES_BANK00_FRAME451);
const struct sprite_frame gSpriteBank00Frame452 = SPRITE_FRAME(gSpriteBank00Frame452, SPRITE_TILES_BANK00_FRAME452);
const struct sprite_frame gSpriteBank00Frame453 = SPRITE_FRAME(gSpriteBank00Frame453, SPRITE_TILES_BANK00_FRAME453);
const struct sprite_frame gSpriteBank00Frame454 = SPRITE_FRAME(gSpriteBank00Frame454, SPRITE_TILES_BANK00_FRAME454);
const struct sprite_frame gSpriteBank00Frame455 = SPRITE_FRAME(gSpriteBank00Frame455, SPRITE_TILES_BANK00_FRAME455);
const struct sprite_frame gSpriteBank00Frame456 = SPRITE_FRAME(gSpriteBank00Frame456, SPRITE_TILES_BANK00_FRAME456);
const struct sprite_frame gSpriteBank00Frame457 = SPRITE_FRAME(gSpriteBank00Frame457, SPRITE_TILES_BANK00_FRAME457);
const struct sprite_frame gSpriteBank00Frame458 = SPRITE_FRAME(gSpriteBank00Frame458, SPRITE_TILES_BANK00_FRAME458);
const struct sprite_frame gSpriteBank00Frame459 = SPRITE_FRAME(gSpriteBank00Frame459, SPRITE_TILES_BANK00_FRAME459);
const struct sprite_frame gSpriteBank00Frame460 = SPRITE_FRAME(gSpriteBank00Frame460, SPRITE_TILES_BANK00_FRAME460);
const struct sprite_frame gSpriteBank00Frame461 = SPRITE_FRAME(gSpriteBank00Frame461, SPRITE_TILES_BANK00_FRAME461);
const struct sprite_frame gSpriteBank00Frame462 = SPRITE_FRAME(gSpriteBank00Frame462, SPRITE_TILES_BANK00_FRAME462);
const struct sprite_frame gSpriteBank00Frame463 = SPRITE_FRAME(gSpriteBank00Frame463, SPRITE_TILES_BANK00_FRAME463);
const struct sprite_frame gSpriteBank00Frame464 = SPRITE_FRAME(gSpriteBank00Frame464, SPRITE_TILES_BANK00_FRAME464);
const struct sprite_frame gSpriteBank00Frame465 = SPRITE_FRAME(gSpriteBank00Frame465, SPRITE_TILES_BANK00_FRAME465);
const struct sprite_frame gSpriteBank00Frame466 = SPRITE_FRAME(gSpriteBank00Frame466, SPRITE_TILES_BANK00_FRAME466);
const struct sprite_frame gSpriteBank00Frame467 = SPRITE_FRAME(gSpriteBank00Frame467, SPRITE_TILES_BANK00_FRAME467);
const struct sprite_frame gSpriteBank00Frame468 = SPRITE_FRAME(gSpriteBank00Frame468, SPRITE_TILES_BANK00_FRAME468);
const struct sprite_frame gSpriteBank00Frame469 = SPRITE_FRAME(gSpriteBank00Frame469, SPRITE_TILES_BANK00_FRAME469);
const struct sprite_frame gSpriteBank00Frame470 = SPRITE_FRAME(gSpriteBank00Frame470, SPRITE_TILES_BANK00_FRAME470);
const struct sprite_frame gSpriteBank00Frame471 = SPRITE_FRAME(gSpriteBank00Frame471, SPRITE_TILES_BANK00_FRAME471);
const struct sprite_frame gSpriteBank00Frame472 = SPRITE_FRAME(gSpriteBank00Frame472, SPRITE_TILES_BANK00_FRAME472);

/* The piece arrays, from games/n2/graphics/sprites/bank00_crash/bank.json. */
#include "sprites/bank00_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 1: 3 animations, 22 frames, games/n2/graphics/sprites/bank01_crash_carpet/. */

extern const u16 gSpriteBank01Anim00Seq[12];
extern const u16 gSpriteBank01Anim01Seq[6];
extern const u16 gSpriteBank01Anim02Seq[4];

const struct sprite_anim gSpriteBank01Anims[3] = {
    /* 0 */ {
        /* seq */ gSpriteBank01Anim00Seq,
        /* box */ { { -12, -17, 28, 28 }, { -27, -21, 58, 39 } },
        /* paletteId */ 6,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank01Anim01Seq,
        /* box */ { { -12, -17, 28, 28 }, { -29, -21, 62, 40 } },
        /* paletteId */ 6,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank01Anim02Seq,
        /* box */ { { -7, -3, 14, 6 }, { -5, -8, 9, 9 } },
        /* paletteId */ 7,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank01Anim02Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank01Anim00Seq[12] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
};
const u16 gSpriteBank01Anim01Seq[6] = {
    12, 13, 14, 15, 16, 17,
};
const u16 gSpriteBank01Anim02Seq[4] = {
    18, 19, 20, 21,
};

const struct sprite_frame *const gSpriteBank01Frames[22] = {
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
    &gSpriteBank01Frame018,
    &gSpriteBank01Frame019,
    &gSpriteBank01Frame020,
    &gSpriteBank01Frame021,
};

const struct sprite_frame_1box gSpriteBank01Frame000 = {
    SPRITE_FRAME(gSpriteBank01Frame000, SPRITE_TILES_BANK01_FRAME000),
    { { -12, -18, 29, 30 } },
};
const struct sprite_frame_1box gSpriteBank01Frame001 = {
    SPRITE_FRAME(gSpriteBank01Frame001, SPRITE_TILES_BANK01_FRAME001),
    { { -12, -18, 29, 30 } },
};
const struct sprite_frame_1box gSpriteBank01Frame002 = {
    SPRITE_FRAME(gSpriteBank01Frame002, SPRITE_TILES_BANK01_FRAME002),
    { { -12, -18, 29, 30 } },
};
const struct sprite_frame_1box gSpriteBank01Frame003 = {
    SPRITE_FRAME(gSpriteBank01Frame003, SPRITE_TILES_BANK01_FRAME003),
    { { -12, -18, 29, 30 } },
};
const struct sprite_frame_1box gSpriteBank01Frame004 = {
    SPRITE_FRAME(gSpriteBank01Frame004, SPRITE_TILES_BANK01_FRAME004),
    { { -12, -18, 29, 30 } },
};
const struct sprite_frame_1box gSpriteBank01Frame005 = {
    SPRITE_FRAME(gSpriteBank01Frame005, SPRITE_TILES_BANK01_FRAME005),
    { { -12, -18, 29, 30 } },
};
const struct sprite_frame_1box gSpriteBank01Frame006 = {
    SPRITE_FRAME(gSpriteBank01Frame006, SPRITE_TILES_BANK01_FRAME006),
    { { -12, -18, 29, 30 } },
};
const struct sprite_frame_1box gSpriteBank01Frame007 = {
    SPRITE_FRAME(gSpriteBank01Frame007, SPRITE_TILES_BANK01_FRAME007),
    { { -12, -18, 29, 30 } },
};
const struct sprite_frame_1box gSpriteBank01Frame008 = {
    SPRITE_FRAME(gSpriteBank01Frame008, SPRITE_TILES_BANK01_FRAME008),
    { { -12, -18, 29, 30 } },
};
const struct sprite_frame_1box gSpriteBank01Frame009 = {
    SPRITE_FRAME(gSpriteBank01Frame009, SPRITE_TILES_BANK01_FRAME009),
    { { -12, -18, 29, 30 } },
};
const struct sprite_frame_1box gSpriteBank01Frame010 = {
    SPRITE_FRAME(gSpriteBank01Frame010, SPRITE_TILES_BANK01_FRAME010),
    { { -12, -18, 29, 30 } },
};
const struct sprite_frame_1box gSpriteBank01Frame011 = {
    SPRITE_FRAME(gSpriteBank01Frame011, SPRITE_TILES_BANK01_FRAME011),
    { { -12, -18, 29, 30 } },
};
const struct sprite_frame_1box gSpriteBank01Frame012 = {
    SPRITE_FRAME(gSpriteBank01Frame012, SPRITE_TILES_BANK01_FRAME012),
    { { -12, -18, 30, 31 } },
};
const struct sprite_frame_1box gSpriteBank01Frame013 = {
    SPRITE_FRAME(gSpriteBank01Frame013, SPRITE_TILES_BANK01_FRAME013),
    { { -12, -18, 30, 31 } },
};
const struct sprite_frame_1box gSpriteBank01Frame014 = {
    SPRITE_FRAME(gSpriteBank01Frame014, SPRITE_TILES_BANK01_FRAME014),
    { { -12, -18, 30, 31 } },
};
const struct sprite_frame_1box gSpriteBank01Frame015 = {
    SPRITE_FRAME(gSpriteBank01Frame015, SPRITE_TILES_BANK01_FRAME015),
    { { -12, -18, 30, 31 } },
};
const struct sprite_frame_1box gSpriteBank01Frame016 = {
    SPRITE_FRAME(gSpriteBank01Frame016, SPRITE_TILES_BANK01_FRAME016),
    { { -12, -18, 30, 31 } },
};
const struct sprite_frame_1box gSpriteBank01Frame017 = {
    SPRITE_FRAME(gSpriteBank01Frame017, SPRITE_TILES_BANK01_FRAME017),
    { { -12, -18, 30, 31 } },
};
const struct sprite_frame gSpriteBank01Frame018 = SPRITE_FRAME(gSpriteBank01Frame018, SPRITE_TILES_BANK01_FRAME018);
const struct sprite_frame gSpriteBank01Frame019 = SPRITE_FRAME(gSpriteBank01Frame019, SPRITE_TILES_BANK01_FRAME019);
const struct sprite_frame gSpriteBank01Frame020 = SPRITE_FRAME(gSpriteBank01Frame020, SPRITE_TILES_BANK01_FRAME020);
const struct sprite_frame gSpriteBank01Frame021 = SPRITE_FRAME(gSpriteBank01Frame021, SPRITE_TILES_BANK01_FRAME021);

/* The piece arrays, from games/n2/graphics/sprites/bank01_crash_carpet/bank.json. */
#include "sprites/bank01_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 2: 3 animations, 36 frames, games/n2/graphics/sprites/bank02_crash_jetpack/. */

extern const u16 gSpriteBank02Anim00Seq[15];
extern const u16 gSpriteBank02Anim01Seq[7];
extern const u16 gSpriteBank02Anim02Seq[14];

const struct sprite_anim gSpriteBank02Anims[3] = {
    /* 0 */ {
        /* seq */ gSpriteBank02Anim00Seq,
        /* box */ { { -7, -18, 16, 42 }, { -24, -30, 44, 55 } },
        /* paletteId */ 8,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank02Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank02Anim01Seq,
        /* box */ { { -7, -18, 16, 42 }, { -31, -25, 63, 43 } },
        /* paletteId */ 8,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank02Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank02Anim02Seq,
        /* box */ { { -7, -18, 16, 42 }, { -23, -30, 48, 55 } },
        /* paletteId */ 8,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank02Anim02Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank02Anim00Seq[15] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
};
const u16 gSpriteBank02Anim01Seq[7] = {
    15, 16, 17, 18, 19, 20, 21,
};
const u16 gSpriteBank02Anim02Seq[14] = {
    22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
};

const struct sprite_frame *const gSpriteBank02Frames[36] = {
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
    &gSpriteBank02Frame017.frame,
    &gSpriteBank02Frame018.frame,
    &gSpriteBank02Frame019.frame,
    &gSpriteBank02Frame020.frame,
    &gSpriteBank02Frame021.frame,
    &gSpriteBank02Frame022.frame,
    &gSpriteBank02Frame023.frame,
    &gSpriteBank02Frame024.frame,
    &gSpriteBank02Frame025.frame,
    &gSpriteBank02Frame026.frame,
    &gSpriteBank02Frame027.frame,
    &gSpriteBank02Frame028.frame,
    &gSpriteBank02Frame029.frame,
    &gSpriteBank02Frame030.frame,
    &gSpriteBank02Frame031.frame,
    &gSpriteBank02Frame032.frame,
    &gSpriteBank02Frame033.frame,
    &gSpriteBank02Frame034.frame,
    &gSpriteBank02Frame035.frame,
};

const struct sprite_frame_1box gSpriteBank02Frame000 = {
    SPRITE_FRAME(gSpriteBank02Frame000, SPRITE_TILES_BANK02_FRAME000),
    { { -7, -18, 17, 42 } },
};
const struct sprite_frame_1box gSpriteBank02Frame001 = {
    SPRITE_FRAME(gSpriteBank02Frame001, SPRITE_TILES_BANK02_FRAME001),
    { { -7, -18, 17, 42 } },
};
const struct sprite_frame_1box gSpriteBank02Frame002 = {
    SPRITE_FRAME(gSpriteBank02Frame002, SPRITE_TILES_BANK02_FRAME002),
    { { -7, -18, 17, 42 } },
};
const struct sprite_frame_1box gSpriteBank02Frame003 = {
    SPRITE_FRAME(gSpriteBank02Frame003, SPRITE_TILES_BANK02_FRAME003),
    { { -7, -18, 17, 42 } },
};
const struct sprite_frame_1box gSpriteBank02Frame004 = {
    SPRITE_FRAME(gSpriteBank02Frame004, SPRITE_TILES_BANK02_FRAME004),
    { { -7, -18, 17, 42 } },
};
const struct sprite_frame_1box gSpriteBank02Frame005 = {
    SPRITE_FRAME(gSpriteBank02Frame005, SPRITE_TILES_BANK02_FRAME005),
    { { -7, -18, 17, 42 } },
};
const struct sprite_frame_1box gSpriteBank02Frame006 = {
    SPRITE_FRAME(gSpriteBank02Frame006, SPRITE_TILES_BANK02_FRAME006),
    { { -7, -18, 17, 42 } },
};
const struct sprite_frame_1box gSpriteBank02Frame007 = {
    SPRITE_FRAME(gSpriteBank02Frame007, SPRITE_TILES_BANK02_FRAME007),
    { { -7, -18, 17, 42 } },
};
const struct sprite_frame_1box gSpriteBank02Frame008 = {
    SPRITE_FRAME(gSpriteBank02Frame008, SPRITE_TILES_BANK02_FRAME008),
    { { -7, -18, 17, 42 } },
};
const struct sprite_frame_1box gSpriteBank02Frame009 = {
    SPRITE_FRAME(gSpriteBank02Frame009, SPRITE_TILES_BANK02_FRAME009),
    { { -7, -18, 17, 42 } },
};
const struct sprite_frame_1box gSpriteBank02Frame010 = {
    SPRITE_FRAME(gSpriteBank02Frame010, SPRITE_TILES_BANK02_FRAME010),
    { { -7, -18, 17, 42 } },
};
const struct sprite_frame_1box gSpriteBank02Frame011 = {
    SPRITE_FRAME(gSpriteBank02Frame011, SPRITE_TILES_BANK02_FRAME011),
    { { -7, -18, 17, 42 } },
};
const struct sprite_frame_1box gSpriteBank02Frame012 = {
    SPRITE_FRAME(gSpriteBank02Frame012, SPRITE_TILES_BANK02_FRAME012),
    { { -7, -18, 17, 42 } },
};
const struct sprite_frame_1box gSpriteBank02Frame013 = {
    SPRITE_FRAME(gSpriteBank02Frame013, SPRITE_TILES_BANK02_FRAME013),
    { { -7, -18, 17, 42 } },
};
const struct sprite_frame_1box gSpriteBank02Frame014 = {
    SPRITE_FRAME(gSpriteBank02Frame014, SPRITE_TILES_BANK02_FRAME014),
    { { -7, -18, 17, 42 } },
};
const struct sprite_frame_1box gSpriteBank02Frame015 = {
    SPRITE_FRAME(gSpriteBank02Frame015, SPRITE_TILES_BANK02_FRAME015),
    { { -24, -20, 49, 33 } },
};
const struct sprite_frame_1box gSpriteBank02Frame016 = {
    SPRITE_FRAME(gSpriteBank02Frame016, SPRITE_TILES_BANK02_FRAME016),
    { { -26, -19, 51, 32 } },
};
const struct sprite_frame_1box gSpriteBank02Frame017 = {
    SPRITE_FRAME(gSpriteBank02Frame017, SPRITE_TILES_BANK02_FRAME017),
    { { -25, -17, 51, 31 } },
};
const struct sprite_frame_1box gSpriteBank02Frame018 = {
    SPRITE_FRAME(gSpriteBank02Frame018, SPRITE_TILES_BANK02_FRAME018),
    { { -23, -18, 46, 33 } },
};
const struct sprite_frame_1box gSpriteBank02Frame019 = {
    SPRITE_FRAME(gSpriteBank02Frame019, SPRITE_TILES_BANK02_FRAME019),
    { { -25, -20, 54, 33 } },
};
const struct sprite_frame_1box gSpriteBank02Frame020 = {
    SPRITE_FRAME(gSpriteBank02Frame020, SPRITE_TILES_BANK02_FRAME020),
    { { -22, -19, 47, 31 } },
};
const struct sprite_frame_1box gSpriteBank02Frame021 = {
    SPRITE_FRAME(gSpriteBank02Frame021, SPRITE_TILES_BANK02_FRAME021),
    { { -22, -19, 44, 33 } },
};
const struct sprite_frame_1box gSpriteBank02Frame022 = {
    SPRITE_FRAME(gSpriteBank02Frame022, SPRITE_TILES_BANK02_FRAME022),
    { { -8, -15, 16, 33 } },
};
const struct sprite_frame_1box gSpriteBank02Frame023 = {
    SPRITE_FRAME(gSpriteBank02Frame023, SPRITE_TILES_BANK02_FRAME023),
    { { -8, -15, 16, 33 } },
};
const struct sprite_frame_1box gSpriteBank02Frame024 = {
    SPRITE_FRAME(gSpriteBank02Frame024, SPRITE_TILES_BANK02_FRAME024),
    { { -8, -15, 16, 33 } },
};
const struct sprite_frame_1box gSpriteBank02Frame025 = {
    SPRITE_FRAME(gSpriteBank02Frame025, SPRITE_TILES_BANK02_FRAME025),
    { { -8, -15, 16, 33 } },
};
const struct sprite_frame_1box gSpriteBank02Frame026 = {
    SPRITE_FRAME(gSpriteBank02Frame026, SPRITE_TILES_BANK02_FRAME026),
    { { -8, -15, 16, 33 } },
};
const struct sprite_frame_1box gSpriteBank02Frame027 = {
    SPRITE_FRAME(gSpriteBank02Frame027, SPRITE_TILES_BANK02_FRAME027),
    { { -8, -15, 16, 33 } },
};
const struct sprite_frame_1box gSpriteBank02Frame028 = {
    SPRITE_FRAME(gSpriteBank02Frame028, SPRITE_TILES_BANK02_FRAME028),
    { { -8, -15, 16, 33 } },
};
const struct sprite_frame_1box gSpriteBank02Frame029 = {
    SPRITE_FRAME(gSpriteBank02Frame029, SPRITE_TILES_BANK02_FRAME029),
    { { -8, -15, 16, 33 } },
};
const struct sprite_frame_1box gSpriteBank02Frame030 = {
    SPRITE_FRAME(gSpriteBank02Frame030, SPRITE_TILES_BANK02_FRAME030),
    { { -8, -15, 16, 33 } },
};
const struct sprite_frame_1box gSpriteBank02Frame031 = {
    SPRITE_FRAME(gSpriteBank02Frame031, SPRITE_TILES_BANK02_FRAME031),
    { { -8, -15, 16, 33 } },
};
const struct sprite_frame_1box gSpriteBank02Frame032 = {
    SPRITE_FRAME(gSpriteBank02Frame032, SPRITE_TILES_BANK02_FRAME032),
    { { -8, -15, 16, 33 } },
};
const struct sprite_frame_1box gSpriteBank02Frame033 = {
    SPRITE_FRAME(gSpriteBank02Frame033, SPRITE_TILES_BANK02_FRAME033),
    { { -8, -15, 16, 33 } },
};
const struct sprite_frame_1box gSpriteBank02Frame034 = {
    SPRITE_FRAME(gSpriteBank02Frame034, SPRITE_TILES_BANK02_FRAME034),
    { { -8, -15, 16, 33 } },
};
const struct sprite_frame_1box gSpriteBank02Frame035 = {
    SPRITE_FRAME(gSpriteBank02Frame035, SPRITE_TILES_BANK02_FRAME035),
    { { -8, -15, 16, 33 } },
};

/* The piece arrays, from games/n2/graphics/sprites/bank02_crash_jetpack/bank.json. */
#include "sprites/bank02_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 3: 5 animations, 65 frames, games/n2/graphics/sprites/bank03_pot_carrier/. */

extern const u16 gSpriteBank03Anim00Seq[10];
extern const u16 gSpriteBank03Anim01Seq[30];
extern const u16 gSpriteBank03Anim02Seq[30];
extern const u16 gSpriteBank03Anim03Seq[10];
extern const u16 gSpriteBank03Anim04Seq[30];

const struct sprite_anim gSpriteBank03Anims[5] = {
    /* 0 */ {
        /* seq */ gSpriteBank03Anim00Seq,
        /* box */ { { -13, -35, 26, 70 }, { -16, -48, 53, 85 } },
        /* paletteId */ 9,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank03Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank03Anim01Seq,
        /* box */ { { -13, -35, 26, 70 }, { -15, -37, 35, 72 } },
        /* paletteId */ 9,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank03Anim01Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 2 */ {
        /* seq */ gSpriteBank03Anim02Seq,
        /* box */ { { -13, -35, 26, 70 }, { -14, -37, 35, 72 } },
        /* paletteId */ 9,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank03Anim02Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 3 */ {
        /* seq */ gSpriteBank03Anim03Seq,
        /* box */ { { -13, -35, 26, 70 }, { -10, -8, 25, 43 } },
        /* paletteId */ 9,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank03Anim03Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 4 */ {
        /* seq */ gSpriteBank03Anim04Seq,
        /* box */ { { -13, -35, 26, 70 }, { -14, -37, 35, 72 } },
        /* paletteId */ 9,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank03Anim04Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank03Anim00Seq[10] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
};
const u16 gSpriteBank03Anim01Seq[30] = {
    10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17,
    18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 23, 24, 24,
};
const u16 gSpriteBank03Anim02Seq[30] = {
    25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54,
};
const u16 gSpriteBank03Anim03Seq[10] = {
    55, 56, 57, 58, 59, 60, 61, 62, 63, 64,
};
const u16 gSpriteBank03Anim04Seq[30] = {
    54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39,
    38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25,
};

const struct sprite_frame *const gSpriteBank03Frames[65] = {
    &gSpriteBank03Frame000,
    &gSpriteBank03Frame001,
    &gSpriteBank03Frame002,
    &gSpriteBank03Frame003,
    &gSpriteBank03Frame004,
    &gSpriteBank03Frame005,
    &gSpriteBank03Frame006,
    &gSpriteBank03Frame007,
    &gSpriteBank03Frame008,
    &gSpriteBank03Frame009,
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
    &gSpriteBank03Frame025.frame,
    &gSpriteBank03Frame026.frame,
    &gSpriteBank03Frame027.frame,
    &gSpriteBank03Frame028.frame,
    &gSpriteBank03Frame029.frame,
    &gSpriteBank03Frame030.frame,
    &gSpriteBank03Frame031.frame,
    &gSpriteBank03Frame032.frame,
    &gSpriteBank03Frame033.frame,
    &gSpriteBank03Frame034.frame,
    &gSpriteBank03Frame035.frame,
    &gSpriteBank03Frame036.frame,
    &gSpriteBank03Frame037.frame,
    &gSpriteBank03Frame038.frame,
    &gSpriteBank03Frame039.frame,
    &gSpriteBank03Frame040.frame,
    &gSpriteBank03Frame041.frame,
    &gSpriteBank03Frame042.frame,
    &gSpriteBank03Frame043.frame,
    &gSpriteBank03Frame044.frame,
    &gSpriteBank03Frame045.frame,
    &gSpriteBank03Frame046.frame,
    &gSpriteBank03Frame047.frame,
    &gSpriteBank03Frame048.frame,
    &gSpriteBank03Frame049.frame,
    &gSpriteBank03Frame050.frame,
    &gSpriteBank03Frame051.frame,
    &gSpriteBank03Frame052.frame,
    &gSpriteBank03Frame053.frame,
    &gSpriteBank03Frame054.frame,
    &gSpriteBank03Frame055.frame,
    &gSpriteBank03Frame056.frame,
    &gSpriteBank03Frame057.frame,
    &gSpriteBank03Frame058.frame,
    &gSpriteBank03Frame059.frame,
    &gSpriteBank03Frame060.frame,
    &gSpriteBank03Frame061.frame,
    &gSpriteBank03Frame062.frame,
    &gSpriteBank03Frame063.frame,
    &gSpriteBank03Frame064.frame,
};

const struct sprite_frame gSpriteBank03Frame000 = SPRITE_FRAME(gSpriteBank03Frame000, SPRITE_TILES_BANK03_FRAME000);
const struct sprite_frame gSpriteBank03Frame001 = SPRITE_FRAME(gSpriteBank03Frame001, SPRITE_TILES_BANK03_FRAME001);
const struct sprite_frame gSpriteBank03Frame002 = SPRITE_FRAME(gSpriteBank03Frame002, SPRITE_TILES_BANK03_FRAME002);
const struct sprite_frame gSpriteBank03Frame003 = SPRITE_FRAME(gSpriteBank03Frame003, SPRITE_TILES_BANK03_FRAME003);
const struct sprite_frame gSpriteBank03Frame004 = SPRITE_FRAME(gSpriteBank03Frame004, SPRITE_TILES_BANK03_FRAME004);
const struct sprite_frame gSpriteBank03Frame005 = SPRITE_FRAME(gSpriteBank03Frame005, SPRITE_TILES_BANK03_FRAME005);
const struct sprite_frame gSpriteBank03Frame006 = SPRITE_FRAME(gSpriteBank03Frame006, SPRITE_TILES_BANK03_FRAME006);
const struct sprite_frame gSpriteBank03Frame007 = SPRITE_FRAME(gSpriteBank03Frame007, SPRITE_TILES_BANK03_FRAME007);
const struct sprite_frame gSpriteBank03Frame008 = SPRITE_FRAME(gSpriteBank03Frame008, SPRITE_TILES_BANK03_FRAME008);
const struct sprite_frame gSpriteBank03Frame009 = SPRITE_FRAME(gSpriteBank03Frame009, SPRITE_TILES_BANK03_FRAME009);
const struct sprite_frame_1box gSpriteBank03Frame010 = {
    SPRITE_FRAME(gSpriteBank03Frame010, SPRITE_TILES_BANK03_FRAME010),
    { { -13, -3, 21, 39 } },
};
const struct sprite_frame_1box gSpriteBank03Frame011 = {
    SPRITE_FRAME(gSpriteBank03Frame011, SPRITE_TILES_BANK03_FRAME011),
    { { -13, -3, 21, 39 } },
};
const struct sprite_frame_1box gSpriteBank03Frame012 = {
    SPRITE_FRAME(gSpriteBank03Frame012, SPRITE_TILES_BANK03_FRAME012),
    { { -13, -3, 21, 39 } },
};
const struct sprite_frame_1box gSpriteBank03Frame013 = {
    SPRITE_FRAME(gSpriteBank03Frame013, SPRITE_TILES_BANK03_FRAME013),
    { { -13, -3, 21, 39 } },
};
const struct sprite_frame_1box gSpriteBank03Frame014 = {
    SPRITE_FRAME(gSpriteBank03Frame014, SPRITE_TILES_BANK03_FRAME014),
    { { -13, -3, 21, 39 } },
};
const struct sprite_frame_1box gSpriteBank03Frame015 = {
    SPRITE_FRAME(gSpriteBank03Frame015, SPRITE_TILES_BANK03_FRAME015),
    { { -13, -3, 21, 39 } },
};
const struct sprite_frame_1box gSpriteBank03Frame016 = {
    SPRITE_FRAME(gSpriteBank03Frame016, SPRITE_TILES_BANK03_FRAME016),
    { { -13, -3, 21, 39 } },
};
const struct sprite_frame_1box gSpriteBank03Frame017 = {
    SPRITE_FRAME(gSpriteBank03Frame017, SPRITE_TILES_BANK03_FRAME017),
    { { -13, -3, 21, 39 } },
};
const struct sprite_frame_1box gSpriteBank03Frame018 = {
    SPRITE_FRAME(gSpriteBank03Frame018, SPRITE_TILES_BANK03_FRAME018),
    { { -13, -3, 21, 39 } },
};
const struct sprite_frame_1box gSpriteBank03Frame019 = {
    SPRITE_FRAME(gSpriteBank03Frame019, SPRITE_TILES_BANK03_FRAME019),
    { { -13, -3, 21, 39 } },
};
const struct sprite_frame_1box gSpriteBank03Frame020 = {
    SPRITE_FRAME(gSpriteBank03Frame020, SPRITE_TILES_BANK03_FRAME020),
    { { -13, -3, 21, 39 } },
};
const struct sprite_frame_1box gSpriteBank03Frame021 = {
    SPRITE_FRAME(gSpriteBank03Frame021, SPRITE_TILES_BANK03_FRAME021),
    { { -13, -3, 21, 39 } },
};
const struct sprite_frame_1box gSpriteBank03Frame022 = {
    SPRITE_FRAME(gSpriteBank03Frame022, SPRITE_TILES_BANK03_FRAME022),
    { { -13, -3, 21, 39 } },
};
const struct sprite_frame_1box gSpriteBank03Frame023 = {
    SPRITE_FRAME(gSpriteBank03Frame023, SPRITE_TILES_BANK03_FRAME023),
    { { -13, -3, 21, 39 } },
};
const struct sprite_frame_1box gSpriteBank03Frame024 = {
    SPRITE_FRAME(gSpriteBank03Frame024, SPRITE_TILES_BANK03_FRAME024),
    { { -13, -3, 21, 39 } },
};
const struct sprite_frame_1box gSpriteBank03Frame025 = {
    SPRITE_FRAME(gSpriteBank03Frame025, SPRITE_TILES_BANK03_FRAME025),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank03Frame026 = {
    SPRITE_FRAME(gSpriteBank03Frame026, SPRITE_TILES_BANK03_FRAME026),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank03Frame027 = {
    SPRITE_FRAME(gSpriteBank03Frame027, SPRITE_TILES_BANK03_FRAME027),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank03Frame028 = {
    SPRITE_FRAME(gSpriteBank03Frame028, SPRITE_TILES_BANK03_FRAME028),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank03Frame029 = {
    SPRITE_FRAME(gSpriteBank03Frame029, SPRITE_TILES_BANK03_FRAME029),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank03Frame030 = {
    SPRITE_FRAME(gSpriteBank03Frame030, SPRITE_TILES_BANK03_FRAME030),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank03Frame031 = {
    SPRITE_FRAME(gSpriteBank03Frame031, SPRITE_TILES_BANK03_FRAME031),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank03Frame032 = {
    SPRITE_FRAME(gSpriteBank03Frame032, SPRITE_TILES_BANK03_FRAME032),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank03Frame033 = {
    SPRITE_FRAME(gSpriteBank03Frame033, SPRITE_TILES_BANK03_FRAME033),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank03Frame034 = {
    SPRITE_FRAME(gSpriteBank03Frame034, SPRITE_TILES_BANK03_FRAME034),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank03Frame035 = {
    SPRITE_FRAME(gSpriteBank03Frame035, SPRITE_TILES_BANK03_FRAME035),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank03Frame036 = {
    SPRITE_FRAME(gSpriteBank03Frame036, SPRITE_TILES_BANK03_FRAME036),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank03Frame037 = {
    SPRITE_FRAME(gSpriteBank03Frame037, SPRITE_TILES_BANK03_FRAME037),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank03Frame038 = {
    SPRITE_FRAME(gSpriteBank03Frame038, SPRITE_TILES_BANK03_FRAME038),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank03Frame039 = {
    SPRITE_FRAME(gSpriteBank03Frame039, SPRITE_TILES_BANK03_FRAME039),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank03Frame040 = {
    SPRITE_FRAME(gSpriteBank03Frame040, SPRITE_TILES_BANK03_FRAME040),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank03Frame041 = {
    SPRITE_FRAME(gSpriteBank03Frame041, SPRITE_TILES_BANK03_FRAME041),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank03Frame042 = {
    SPRITE_FRAME(gSpriteBank03Frame042, SPRITE_TILES_BANK03_FRAME042),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank03Frame043 = {
    SPRITE_FRAME(gSpriteBank03Frame043, SPRITE_TILES_BANK03_FRAME043),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank03Frame044 = {
    SPRITE_FRAME(gSpriteBank03Frame044, SPRITE_TILES_BANK03_FRAME044),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank03Frame045 = {
    SPRITE_FRAME(gSpriteBank03Frame045, SPRITE_TILES_BANK03_FRAME045),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank03Frame046 = {
    SPRITE_FRAME(gSpriteBank03Frame046, SPRITE_TILES_BANK03_FRAME046),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank03Frame047 = {
    SPRITE_FRAME(gSpriteBank03Frame047, SPRITE_TILES_BANK03_FRAME047),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank03Frame048 = {
    SPRITE_FRAME(gSpriteBank03Frame048, SPRITE_TILES_BANK03_FRAME048),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank03Frame049 = {
    SPRITE_FRAME(gSpriteBank03Frame049, SPRITE_TILES_BANK03_FRAME049),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank03Frame050 = {
    SPRITE_FRAME(gSpriteBank03Frame050, SPRITE_TILES_BANK03_FRAME050),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank03Frame051 = {
    SPRITE_FRAME(gSpriteBank03Frame051, SPRITE_TILES_BANK03_FRAME051),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank03Frame052 = {
    SPRITE_FRAME(gSpriteBank03Frame052, SPRITE_TILES_BANK03_FRAME052),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank03Frame053 = {
    SPRITE_FRAME(gSpriteBank03Frame053, SPRITE_TILES_BANK03_FRAME053),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_1box gSpriteBank03Frame054 = {
    SPRITE_FRAME(gSpriteBank03Frame054, SPRITE_TILES_BANK03_FRAME054),
    { { -13, -3, 18, 38 } },
};
const struct sprite_frame_3box_anchor gSpriteBank03Frame055 = {
    SPRITE_FRAME(gSpriteBank03Frame055, SPRITE_TILES_BANK03_FRAME055),
    { { -1, -4, 8, 37 }, { 0, 0, 0, 0 }, { -7, -6, 19, 40 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank03Frame056 = {
    SPRITE_FRAME(gSpriteBank03Frame056, SPRITE_TILES_BANK03_FRAME056),
    { { -1, -4, 8, 37 }, { 0, 0, 0, 0 }, { -7, -6, 19, 40 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank03Frame057 = {
    SPRITE_FRAME(gSpriteBank03Frame057, SPRITE_TILES_BANK03_FRAME057),
    { { -1, -4, 8, 37 }, { 0, 0, 0, 0 }, { -7, -6, 19, 40 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank03Frame058 = {
    SPRITE_FRAME(gSpriteBank03Frame058, SPRITE_TILES_BANK03_FRAME058),
    { { -1, -4, 8, 37 }, { 0, 0, 0, 0 }, { -7, -6, 19, 40 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank03Frame059 = {
    SPRITE_FRAME(gSpriteBank03Frame059, SPRITE_TILES_BANK03_FRAME059),
    { { -1, -4, 8, 37 }, { 0, 0, 0, 0 }, { -7, -6, 19, 40 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank03Frame060 = {
    SPRITE_FRAME(gSpriteBank03Frame060, SPRITE_TILES_BANK03_FRAME060),
    { { -1, -4, 8, 37 }, { 0, 0, 0, 0 }, { -7, -6, 19, 40 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank03Frame061 = {
    SPRITE_FRAME(gSpriteBank03Frame061, SPRITE_TILES_BANK03_FRAME061),
    { { -1, -4, 8, 37 }, { 0, 0, 0, 0 }, { -7, -6, 19, 40 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank03Frame062 = {
    SPRITE_FRAME(gSpriteBank03Frame062, SPRITE_TILES_BANK03_FRAME062),
    { { -1, -4, 8, 37 }, { 0, 0, 0, 0 }, { -7, -6, 19, 40 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank03Frame063 = {
    SPRITE_FRAME(gSpriteBank03Frame063, SPRITE_TILES_BANK03_FRAME063),
    { { -1, -4, 8, 37 }, { 0, 0, 0, 0 }, { -7, -6, 19, 40 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank03Frame064 = {
    SPRITE_FRAME(gSpriteBank03Frame064, SPRITE_TILES_BANK03_FRAME064),
    { { -1, -4, 8, 37 }, { 0, 0, 0, 0 }, { -7, -6, 19, 40 } },
    { 0, 0 },
};

/* The piece arrays, from games/n2/graphics/sprites/bank03_pot_carrier/bank.json. */
#include "sprites/bank03_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 4: 2 animations, 18 frames, games/n2/graphics/sprites/bank04_scimitar_guard/. */

extern const u16 gSpriteBank04Anim00Seq[15];
extern const u16 gSpriteBank04Anim01Seq[14];

const struct sprite_anim gSpriteBank04Anims[2] = {
    /* 0 */ {
        /* seq */ gSpriteBank04Anim00Seq,
        /* box */ { { -10, -22, 16, 46 }, { -27, -28, 55, 53 } },
        /* paletteId */ 10,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank04Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank04Anim01Seq,
        /* box */ { { -10, -22, 16, 46 }, { -27, -38, 59, 70 } },
        /* paletteId */ 10,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank04Anim01Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank04Anim00Seq[15] = {
    0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7,
};
const u16 gSpriteBank04Anim01Seq[14] = {
    8, 9, 10, 10, 11, 12, 12, 13, 14, 14, 15, 16, 16, 17,
};

const struct sprite_frame *const gSpriteBank04Frames[18] = {
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
};

const struct sprite_frame_2box gSpriteBank04Frame000 = {
    SPRITE_FRAME(gSpriteBank04Frame000, SPRITE_TILES_BANK04_FRAME000),
    { { -17, -22, 18, 46 }, { -1, -16, 15, 40 } },
};
const struct sprite_frame_2box gSpriteBank04Frame001 = {
    SPRITE_FRAME(gSpriteBank04Frame001, SPRITE_TILES_BANK04_FRAME001),
    { { -17, -22, 18, 46 }, { -1, -16, 15, 40 } },
};
const struct sprite_frame_2box gSpriteBank04Frame002 = {
    SPRITE_FRAME(gSpriteBank04Frame002, SPRITE_TILES_BANK04_FRAME002),
    { { -17, -22, 18, 46 }, { -1, -16, 15, 40 } },
};
const struct sprite_frame_2box gSpriteBank04Frame003 = {
    SPRITE_FRAME(gSpriteBank04Frame003, SPRITE_TILES_BANK04_FRAME003),
    { { -17, -22, 18, 46 }, { -1, -16, 15, 40 } },
};
const struct sprite_frame_2box gSpriteBank04Frame004 = {
    SPRITE_FRAME(gSpriteBank04Frame004, SPRITE_TILES_BANK04_FRAME004),
    { { -17, -22, 18, 46 }, { -1, -16, 15, 40 } },
};
const struct sprite_frame_2box gSpriteBank04Frame005 = {
    SPRITE_FRAME(gSpriteBank04Frame005, SPRITE_TILES_BANK04_FRAME005),
    { { -17, -22, 18, 46 }, { -1, -16, 15, 40 } },
};
const struct sprite_frame_2box gSpriteBank04Frame006 = {
    SPRITE_FRAME(gSpriteBank04Frame006, SPRITE_TILES_BANK04_FRAME006),
    { { -17, -22, 18, 46 }, { -1, -16, 15, 40 } },
};
const struct sprite_frame_2box gSpriteBank04Frame007 = {
    SPRITE_FRAME(gSpriteBank04Frame007, SPRITE_TILES_BANK04_FRAME007),
    { { -17, -22, 18, 46 }, { -1, -16, 15, 40 } },
};
const struct sprite_frame_1box gSpriteBank04Frame008 = {
    SPRITE_FRAME(gSpriteBank04Frame008, SPRITE_TILES_BANK04_FRAME008),
    { { -11, -19, 18, 42 } },
};
const struct sprite_frame_1box gSpriteBank04Frame009 = {
    SPRITE_FRAME(gSpriteBank04Frame009, SPRITE_TILES_BANK04_FRAME009),
    { { -11, -19, 18, 42 } },
};
const struct sprite_frame_1box gSpriteBank04Frame010 = {
    SPRITE_FRAME(gSpriteBank04Frame010, SPRITE_TILES_BANK04_FRAME010),
    { { -11, -19, 18, 42 } },
};
const struct sprite_frame_1box gSpriteBank04Frame011 = {
    SPRITE_FRAME(gSpriteBank04Frame011, SPRITE_TILES_BANK04_FRAME011),
    { { -11, -19, 18, 42 } },
};
const struct sprite_frame_1box gSpriteBank04Frame012 = {
    SPRITE_FRAME(gSpriteBank04Frame012, SPRITE_TILES_BANK04_FRAME012),
    { { -11, -19, 18, 42 } },
};
const struct sprite_frame_1box gSpriteBank04Frame013 = {
    SPRITE_FRAME(gSpriteBank04Frame013, SPRITE_TILES_BANK04_FRAME013),
    { { -11, -19, 18, 42 } },
};
const struct sprite_frame_1box gSpriteBank04Frame014 = {
    SPRITE_FRAME(gSpriteBank04Frame014, SPRITE_TILES_BANK04_FRAME014),
    { { -11, -19, 18, 42 } },
};
const struct sprite_frame_1box gSpriteBank04Frame015 = {
    SPRITE_FRAME(gSpriteBank04Frame015, SPRITE_TILES_BANK04_FRAME015),
    { { -11, -19, 18, 42 } },
};
const struct sprite_frame_1box gSpriteBank04Frame016 = {
    SPRITE_FRAME(gSpriteBank04Frame016, SPRITE_TILES_BANK04_FRAME016),
    { { -11, -19, 18, 42 } },
};
const struct sprite_frame_1box gSpriteBank04Frame017 = {
    SPRITE_FRAME(gSpriteBank04Frame017, SPRITE_TILES_BANK04_FRAME017),
    { { -11, -19, 18, 42 } },
};

/* The piece arrays, from games/n2/graphics/sprites/bank04_scimitar_guard/bank.json. */
#include "sprites/bank04_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 5: 2 animations, 19 frames, games/n2/graphics/sprites/bank05_scorpion/. */

extern const u16 gSpriteBank05Anim00Seq[10];
extern const u16 gSpriteBank05Anim01Seq[9];

const struct sprite_anim gSpriteBank05Anims[2] = {
    /* 0 */ {
        /* seq */ gSpriteBank05Anim00Seq,
        /* box */ { { -19, -3, 40, 18 }, { -21, -15, 45, 32 } },
        /* paletteId */ 11,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank05Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank05Anim01Seq,
        /* box */ { { -19, -3, 40, 18 }, { -22, -16, 46, 35 } },
        /* paletteId */ 11,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank05Anim01Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank05Anim00Seq[10] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
};
const u16 gSpriteBank05Anim01Seq[9] = {
    10, 11, 12, 13, 14, 15, 16, 17, 18,
};

const struct sprite_frame *const gSpriteBank05Frames[19] = {
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
};

const struct sprite_frame_1box gSpriteBank05Frame000 = {
    SPRITE_FRAME(gSpriteBank05Frame000, SPRITE_TILES_BANK05_FRAME000),
    { { -19, -5, 37, 19 } },
};
const struct sprite_frame_1box gSpriteBank05Frame001 = {
    SPRITE_FRAME(gSpriteBank05Frame001, SPRITE_TILES_BANK05_FRAME001),
    { { -19, -5, 37, 19 } },
};
const struct sprite_frame_1box gSpriteBank05Frame002 = {
    SPRITE_FRAME(gSpriteBank05Frame002, SPRITE_TILES_BANK05_FRAME002),
    { { -19, -5, 37, 19 } },
};
const struct sprite_frame_1box gSpriteBank05Frame003 = {
    SPRITE_FRAME(gSpriteBank05Frame003, SPRITE_TILES_BANK05_FRAME003),
    { { -19, -5, 37, 19 } },
};
const struct sprite_frame_1box gSpriteBank05Frame004 = {
    SPRITE_FRAME(gSpriteBank05Frame004, SPRITE_TILES_BANK05_FRAME004),
    { { -19, -5, 37, 19 } },
};
const struct sprite_frame_1box gSpriteBank05Frame005 = {
    SPRITE_FRAME(gSpriteBank05Frame005, SPRITE_TILES_BANK05_FRAME005),
    { { -19, -5, 37, 19 } },
};
const struct sprite_frame_1box gSpriteBank05Frame006 = {
    SPRITE_FRAME(gSpriteBank05Frame006, SPRITE_TILES_BANK05_FRAME006),
    { { -19, -5, 37, 19 } },
};
const struct sprite_frame_1box gSpriteBank05Frame007 = {
    SPRITE_FRAME(gSpriteBank05Frame007, SPRITE_TILES_BANK05_FRAME007),
    { { -19, -5, 37, 19 } },
};
const struct sprite_frame_1box gSpriteBank05Frame008 = {
    SPRITE_FRAME(gSpriteBank05Frame008, SPRITE_TILES_BANK05_FRAME008),
    { { -19, -5, 37, 19 } },
};
const struct sprite_frame_1box gSpriteBank05Frame009 = {
    SPRITE_FRAME(gSpriteBank05Frame009, SPRITE_TILES_BANK05_FRAME009),
    { { -19, -5, 37, 19 } },
};
const struct sprite_frame_1box gSpriteBank05Frame010 = {
    SPRITE_FRAME(gSpriteBank05Frame010, SPRITE_TILES_BANK05_FRAME010),
    { { -16, -3, 35, 17 } },
};
const struct sprite_frame_1box gSpriteBank05Frame011 = {
    SPRITE_FRAME(gSpriteBank05Frame011, SPRITE_TILES_BANK05_FRAME011),
    { { -16, -3, 35, 17 } },
};
const struct sprite_frame_1box gSpriteBank05Frame012 = {
    SPRITE_FRAME(gSpriteBank05Frame012, SPRITE_TILES_BANK05_FRAME012),
    { { -16, -3, 35, 17 } },
};
const struct sprite_frame_1box gSpriteBank05Frame013 = {
    SPRITE_FRAME(gSpriteBank05Frame013, SPRITE_TILES_BANK05_FRAME013),
    { { -16, -3, 35, 17 } },
};
const struct sprite_frame_1box gSpriteBank05Frame014 = {
    SPRITE_FRAME(gSpriteBank05Frame014, SPRITE_TILES_BANK05_FRAME014),
    { { -16, -3, 35, 17 } },
};
const struct sprite_frame_1box gSpriteBank05Frame015 = {
    SPRITE_FRAME(gSpriteBank05Frame015, SPRITE_TILES_BANK05_FRAME015),
    { { -16, -3, 35, 17 } },
};
const struct sprite_frame_1box gSpriteBank05Frame016 = {
    SPRITE_FRAME(gSpriteBank05Frame016, SPRITE_TILES_BANK05_FRAME016),
    { { -16, -3, 35, 17 } },
};
const struct sprite_frame_1box gSpriteBank05Frame017 = {
    SPRITE_FRAME(gSpriteBank05Frame017, SPRITE_TILES_BANK05_FRAME017),
    { { -16, -3, 35, 17 } },
};
const struct sprite_frame_1box gSpriteBank05Frame018 = {
    SPRITE_FRAME(gSpriteBank05Frame018, SPRITE_TILES_BANK05_FRAME018),
    { { -16, -3, 35, 17 } },
};

/* The piece arrays, from games/n2/graphics/sprites/bank05_scorpion/bank.json. */
#include "sprites/bank05_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 6: 5 animations, 52 frames, games/n2/graphics/sprites/bank06_genie/. */

extern const u16 gSpriteBank06Anim00Seq[10];
extern const u16 gSpriteBank06Anim01Seq[10];
extern const u16 gSpriteBank06Anim02Seq[20];
extern const u16 gSpriteBank06Anim03Seq[5];
extern const u16 gSpriteBank06Anim04Seq[20];

const struct sprite_anim gSpriteBank06Anims[5] = {
    /* 0 */ {
        /* seq */ gSpriteBank06Anim00Seq,
        /* box */ { { -2, -31, 21, 55 }, { -3, -37, 23, 61 } },
        /* paletteId */ 12,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank06Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank06Anim01Seq,
        /* box */ { { -2, -31, 21, 55 }, { -2, -34, 22, 58 } },
        /* paletteId */ 12,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank06Anim01Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 2 */ {
        /* seq */ gSpriteBank06Anim02Seq,
        /* box */ { { -2, -31, 21, 55 }, { -4, -39, 27, 63 } },
        /* paletteId */ 12,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank06Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank06Anim03Seq,
        /* box */ { { -11, -11, 23, 23 }, { -15, -15, 31, 31 } },
        /* paletteId */ 13,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank06Anim03Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 4 */ {
        /* seq */ gSpriteBank06Anim04Seq,
        /* box */ { { -2, -31, 21, 55 }, { -23, 10, 36, 22 } },
        /* paletteId */ 12,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank06Anim04Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank06Anim00Seq[10] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
};
const u16 gSpriteBank06Anim01Seq[10] = {
    10, 11, 12, 12, 13, 14, 14, 15, 16, 16,
};
const u16 gSpriteBank06Anim02Seq[20] = {
    17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 18, 19, 20, 21, 22,
    23, 24, 25, 26,
};
const u16 gSpriteBank06Anim03Seq[5] = {
    27, 28, 29, 30, 31,
};
const u16 gSpriteBank06Anim04Seq[20] = {
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
    48, 49, 50, 51,
};

const struct sprite_frame *const gSpriteBank06Frames[52] = {
    &gSpriteBank06Frame000.frame,
    &gSpriteBank06Frame001.frame,
    &gSpriteBank06Frame002.frame,
    &gSpriteBank06Frame003.frame,
    &gSpriteBank06Frame004.frame,
    &gSpriteBank06Frame005.frame,
    &gSpriteBank06Frame006.frame,
    &gSpriteBank06Frame007.frame,
    &gSpriteBank06Frame008.frame,
    &gSpriteBank06Frame009.frame,
    &gSpriteBank06Frame010.frame,
    &gSpriteBank06Frame011.frame,
    &gSpriteBank06Frame012.frame,
    &gSpriteBank06Frame013.frame,
    &gSpriteBank06Frame014.frame,
    &gSpriteBank06Frame015.frame,
    &gSpriteBank06Frame016.frame,
    &gSpriteBank06Frame017,
    &gSpriteBank06Frame018.frame,
    &gSpriteBank06Frame019.frame,
    &gSpriteBank06Frame020.frame,
    &gSpriteBank06Frame021.frame,
    &gSpriteBank06Frame022.frame,
    &gSpriteBank06Frame023.frame,
    &gSpriteBank06Frame024.frame,
    &gSpriteBank06Frame025.frame,
    &gSpriteBank06Frame026.frame,
    &gSpriteBank06Frame027.frame,
    &gSpriteBank06Frame028.frame,
    &gSpriteBank06Frame029.frame,
    &gSpriteBank06Frame030.frame,
    &gSpriteBank06Frame031.frame,
    &gSpriteBank06Frame032,
    &gSpriteBank06Frame033,
    &gSpriteBank06Frame034,
    &gSpriteBank06Frame035,
    &gSpriteBank06Frame036,
    &gSpriteBank06Frame037,
    &gSpriteBank06Frame038,
    &gSpriteBank06Frame039,
    &gSpriteBank06Frame040,
    &gSpriteBank06Frame041,
    &gSpriteBank06Frame042,
    &gSpriteBank06Frame043,
    &gSpriteBank06Frame044,
    &gSpriteBank06Frame045,
    &gSpriteBank06Frame046,
    &gSpriteBank06Frame047,
    &gSpriteBank06Frame048,
    &gSpriteBank06Frame049,
    &gSpriteBank06Frame050,
    &gSpriteBank06Frame051,
};

const struct sprite_frame_1box gSpriteBank06Frame000 = {
    SPRITE_FRAME(gSpriteBank06Frame000, SPRITE_TILES_BANK06_FRAME000),
    { { 1, -28, 17, 52 } },
};
const struct sprite_frame_1box gSpriteBank06Frame001 = {
    SPRITE_FRAME(gSpriteBank06Frame001, SPRITE_TILES_BANK06_FRAME001),
    { { 1, -28, 17, 52 } },
};
const struct sprite_frame_1box gSpriteBank06Frame002 = {
    SPRITE_FRAME(gSpriteBank06Frame002, SPRITE_TILES_BANK06_FRAME002),
    { { 1, -28, 17, 52 } },
};
const struct sprite_frame_1box gSpriteBank06Frame003 = {
    SPRITE_FRAME(gSpriteBank06Frame003, SPRITE_TILES_BANK06_FRAME003),
    { { 1, -28, 17, 52 } },
};
const struct sprite_frame_1box gSpriteBank06Frame004 = {
    SPRITE_FRAME(gSpriteBank06Frame004, SPRITE_TILES_BANK06_FRAME004),
    { { 1, -28, 17, 52 } },
};
const struct sprite_frame_1box gSpriteBank06Frame005 = {
    SPRITE_FRAME(gSpriteBank06Frame005, SPRITE_TILES_BANK06_FRAME005),
    { { 1, -28, 17, 52 } },
};
const struct sprite_frame_1box gSpriteBank06Frame006 = {
    SPRITE_FRAME(gSpriteBank06Frame006, SPRITE_TILES_BANK06_FRAME006),
    { { 1, -28, 17, 52 } },
};
const struct sprite_frame_1box gSpriteBank06Frame007 = {
    SPRITE_FRAME(gSpriteBank06Frame007, SPRITE_TILES_BANK06_FRAME007),
    { { 1, -28, 17, 52 } },
};
const struct sprite_frame_1box gSpriteBank06Frame008 = {
    SPRITE_FRAME(gSpriteBank06Frame008, SPRITE_TILES_BANK06_FRAME008),
    { { 1, -28, 17, 52 } },
};
const struct sprite_frame_1box gSpriteBank06Frame009 = {
    SPRITE_FRAME(gSpriteBank06Frame009, SPRITE_TILES_BANK06_FRAME009),
    { { 1, -28, 17, 52 } },
};
const struct sprite_frame_1box gSpriteBank06Frame010 = {
    SPRITE_FRAME(gSpriteBank06Frame010, SPRITE_TILES_BANK06_FRAME010),
    { { 0, -26, 19, 50 } },
};
const struct sprite_frame_1box gSpriteBank06Frame011 = {
    SPRITE_FRAME(gSpriteBank06Frame011, SPRITE_TILES_BANK06_FRAME011),
    { { 0, -26, 19, 50 } },
};
const struct sprite_frame_1box gSpriteBank06Frame012 = {
    SPRITE_FRAME(gSpriteBank06Frame012, SPRITE_TILES_BANK06_FRAME012),
    { { 0, -26, 19, 50 } },
};
const struct sprite_frame_1box gSpriteBank06Frame013 = {
    SPRITE_FRAME(gSpriteBank06Frame013, SPRITE_TILES_BANK06_FRAME013),
    { { 0, -26, 19, 50 } },
};
const struct sprite_frame_1box gSpriteBank06Frame014 = {
    SPRITE_FRAME(gSpriteBank06Frame014, SPRITE_TILES_BANK06_FRAME014),
    { { 0, -26, 19, 50 } },
};
const struct sprite_frame_1box gSpriteBank06Frame015 = {
    SPRITE_FRAME(gSpriteBank06Frame015, SPRITE_TILES_BANK06_FRAME015),
    { { 0, -26, 19, 50 } },
};
const struct sprite_frame_1box gSpriteBank06Frame016 = {
    SPRITE_FRAME(gSpriteBank06Frame016, SPRITE_TILES_BANK06_FRAME016),
    { { 0, -26, 19, 50 } },
};
const struct sprite_frame gSpriteBank06Frame017 = SPRITE_FRAME(gSpriteBank06Frame017, SPRITE_TILES_BANK06_FRAME017);
const struct sprite_frame_1box gSpriteBank06Frame018 = {
    SPRITE_FRAME(gSpriteBank06Frame018, SPRITE_TILES_BANK06_FRAME018),
    { { 3, -3, 11, 27 } },
};
const struct sprite_frame_1box gSpriteBank06Frame019 = {
    SPRITE_FRAME(gSpriteBank06Frame019, SPRITE_TILES_BANK06_FRAME019),
    { { -1, -20, 20, 40 } },
};
const struct sprite_frame_1box gSpriteBank06Frame020 = {
    SPRITE_FRAME(gSpriteBank06Frame020, SPRITE_TILES_BANK06_FRAME020),
    { { 0, -30, 18, 48 } },
};
const struct sprite_frame_1box gSpriteBank06Frame021 = {
    SPRITE_FRAME(gSpriteBank06Frame021, SPRITE_TILES_BANK06_FRAME021),
    { { 0, -34, 19, 53 } },
};
const struct sprite_frame_1box gSpriteBank06Frame022 = {
    SPRITE_FRAME(gSpriteBank06Frame022, SPRITE_TILES_BANK06_FRAME022),
    { { 2, -33, 16, 56 } },
};
const struct sprite_frame_1box gSpriteBank06Frame023 = {
    SPRITE_FRAME(gSpriteBank06Frame023, SPRITE_TILES_BANK06_FRAME023),
    { { -1, -30, 17, 52 } },
};
const struct sprite_frame_1box gSpriteBank06Frame024 = {
    SPRITE_FRAME(gSpriteBank06Frame024, SPRITE_TILES_BANK06_FRAME024),
    { { 3, -31, 14, 53 } },
};
const struct sprite_frame_1box gSpriteBank06Frame025 = {
    SPRITE_FRAME(gSpriteBank06Frame025, SPRITE_TILES_BANK06_FRAME025),
    { { 0, -26, 17, 47 } },
};
const struct sprite_frame_1box gSpriteBank06Frame026 = {
    SPRITE_FRAME(gSpriteBank06Frame026, SPRITE_TILES_BANK06_FRAME026),
    { { 2, -25, 14, 47 } },
};
const struct sprite_frame_1box gSpriteBank06Frame027 = {
    SPRITE_FRAME(gSpriteBank06Frame027, SPRITE_TILES_BANK06_FRAME027),
    { { -5, -7, 13, 17 } },
};
const struct sprite_frame_1box gSpriteBank06Frame028 = {
    SPRITE_FRAME(gSpriteBank06Frame028, SPRITE_TILES_BANK06_FRAME028),
    { { -5, -7, 13, 17 } },
};
const struct sprite_frame_1box gSpriteBank06Frame029 = {
    SPRITE_FRAME(gSpriteBank06Frame029, SPRITE_TILES_BANK06_FRAME029),
    { { -5, -7, 13, 17 } },
};
const struct sprite_frame_1box gSpriteBank06Frame030 = {
    SPRITE_FRAME(gSpriteBank06Frame030, SPRITE_TILES_BANK06_FRAME030),
    { { -5, -7, 13, 17 } },
};
const struct sprite_frame_1box gSpriteBank06Frame031 = {
    SPRITE_FRAME(gSpriteBank06Frame031, SPRITE_TILES_BANK06_FRAME031),
    { { -5, -7, 13, 17 } },
};
const struct sprite_frame gSpriteBank06Frame032 = SPRITE_FRAME(gSpriteBank06Frame032, SPRITE_TILES_BANK06_FRAME032);
const struct sprite_frame gSpriteBank06Frame033 = SPRITE_FRAME(gSpriteBank06Frame033, SPRITE_TILES_BANK06_FRAME033);
const struct sprite_frame gSpriteBank06Frame034 = SPRITE_FRAME(gSpriteBank06Frame034, SPRITE_TILES_BANK06_FRAME034);
const struct sprite_frame gSpriteBank06Frame035 = SPRITE_FRAME(gSpriteBank06Frame035, SPRITE_TILES_BANK06_FRAME035);
const struct sprite_frame gSpriteBank06Frame036 = SPRITE_FRAME(gSpriteBank06Frame036, SPRITE_TILES_BANK06_FRAME036);
const struct sprite_frame gSpriteBank06Frame037 = SPRITE_FRAME(gSpriteBank06Frame037, SPRITE_TILES_BANK06_FRAME037);
const struct sprite_frame gSpriteBank06Frame038 = SPRITE_FRAME(gSpriteBank06Frame038, SPRITE_TILES_BANK06_FRAME038);
const struct sprite_frame gSpriteBank06Frame039 = SPRITE_FRAME(gSpriteBank06Frame039, SPRITE_TILES_BANK06_FRAME039);
const struct sprite_frame gSpriteBank06Frame040 = SPRITE_FRAME(gSpriteBank06Frame040, SPRITE_TILES_BANK06_FRAME040);
const struct sprite_frame gSpriteBank06Frame041 = SPRITE_FRAME(gSpriteBank06Frame041, SPRITE_TILES_BANK06_FRAME041);
const struct sprite_frame gSpriteBank06Frame042 = SPRITE_FRAME(gSpriteBank06Frame042, SPRITE_TILES_BANK06_FRAME042);
const struct sprite_frame gSpriteBank06Frame043 = SPRITE_FRAME(gSpriteBank06Frame043, SPRITE_TILES_BANK06_FRAME043);
const struct sprite_frame gSpriteBank06Frame044 = SPRITE_FRAME(gSpriteBank06Frame044, SPRITE_TILES_BANK06_FRAME044);
const struct sprite_frame gSpriteBank06Frame045 = SPRITE_FRAME(gSpriteBank06Frame045, SPRITE_TILES_BANK06_FRAME045);
const struct sprite_frame gSpriteBank06Frame046 = SPRITE_FRAME(gSpriteBank06Frame046, SPRITE_TILES_BANK06_FRAME046);
const struct sprite_frame gSpriteBank06Frame047 = SPRITE_FRAME(gSpriteBank06Frame047, SPRITE_TILES_BANK06_FRAME047);
const struct sprite_frame gSpriteBank06Frame048 = SPRITE_FRAME(gSpriteBank06Frame048, SPRITE_TILES_BANK06_FRAME048);
const struct sprite_frame gSpriteBank06Frame049 = SPRITE_FRAME(gSpriteBank06Frame049, SPRITE_TILES_BANK06_FRAME049);
const struct sprite_frame gSpriteBank06Frame050 = SPRITE_FRAME(gSpriteBank06Frame050, SPRITE_TILES_BANK06_FRAME050);
const struct sprite_frame gSpriteBank06Frame051 = SPRITE_FRAME(gSpriteBank06Frame051, SPRITE_TILES_BANK06_FRAME051);

/* The piece arrays, from games/n2/graphics/sprites/bank06_genie/bank.json. */
#include "sprites/bank06_pieces.inc"
