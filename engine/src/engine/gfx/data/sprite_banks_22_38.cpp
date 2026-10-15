extern "C" {
#include "gba/types.h"
#include "engine/gfx/sprite_bank.h"
}
#include "sprites/sprite_frames.h"

/*
 * ROM 0x084b414c-0x084b9d7c: sprite banks 22-38 of the sprite-bank
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

/* ---------------------------------------------------------------------- */
/* Bank 22: 2 animations, 31 frames, graphics/sprites/bank22_homing_sewer_enemy/. */

extern const u16 gSpriteBank22Anim00Seq[16];
extern const u16 gSpriteBank22Anim01Seq[15];

extern const struct sprite_anim gSpriteBank22Anims[2] = {
    /* 0 */ {
        /* seq */ gSpriteBank22Anim00Seq,
        /* box */ { { -15, -21, 31, 42 }, { -22, -25, 38, 46 } },
        /* paletteId */ 30,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank22Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank22Anim01Seq,
        /* box */ { { -15, -21, 31, 42 }, { -23, -22, 41, 44 } },
        /* paletteId */ 30,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank22Anim01Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank22Anim00Seq[16] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
};
const u16 gSpriteBank22Anim01Seq[15] = {
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};

extern const struct sprite_frame *const gSpriteBank22Frames[31] = {
    &gSpriteBank22Frame000.frame,
    &gSpriteBank22Frame001.frame,
    &gSpriteBank22Frame002.frame,
    &gSpriteBank22Frame003.frame,
    &gSpriteBank22Frame004.frame,
    &gSpriteBank22Frame005.frame,
    &gSpriteBank22Frame006.frame,
    &gSpriteBank22Frame007.frame,
    &gSpriteBank22Frame008.frame,
    &gSpriteBank22Frame009.frame,
    &gSpriteBank22Frame010.frame,
    &gSpriteBank22Frame011.frame,
    &gSpriteBank22Frame012.frame,
    &gSpriteBank22Frame013.frame,
    &gSpriteBank22Frame014.frame,
    &gSpriteBank22Frame015.frame,
    &gSpriteBank22Frame016,
    &gSpriteBank22Frame017,
    &gSpriteBank22Frame018,
    &gSpriteBank22Frame019,
    &gSpriteBank22Frame020,
    &gSpriteBank22Frame021,
    &gSpriteBank22Frame022,
    &gSpriteBank22Frame023,
    &gSpriteBank22Frame024,
    &gSpriteBank22Frame025,
    &gSpriteBank22Frame026,
    &gSpriteBank22Frame027,
    &gSpriteBank22Frame028,
    &gSpriteBank22Frame029,
    &gSpriteBank22Frame030,
};

const struct sprite_frame_1box gSpriteBank22Frame000 = {
    SPRITE_FRAME(gSpriteBank22Frame000, SPRITE_TILES_BANK22_FRAME000),
    { { -12, -9, 25, 28 } },
};
const struct sprite_frame_1box gSpriteBank22Frame001 = {
    SPRITE_FRAME(gSpriteBank22Frame001, SPRITE_TILES_BANK22_FRAME001),
    { { -12, -9, 25, 28 } },
};
const struct sprite_frame_1box gSpriteBank22Frame002 = {
    SPRITE_FRAME(gSpriteBank22Frame002, SPRITE_TILES_BANK22_FRAME002),
    { { -16, -8, 29, 27 } },
};
const struct sprite_frame_1box gSpriteBank22Frame003 = {
    SPRITE_FRAME(gSpriteBank22Frame003, SPRITE_TILES_BANK22_FRAME003),
    { { -19, -8, 32, 27 } },
};
const struct sprite_frame_1box gSpriteBank22Frame004 = {
    SPRITE_FRAME(gSpriteBank22Frame004, SPRITE_TILES_BANK22_FRAME004),
    { { -17, -11, 30, 30 } },
};
const struct sprite_frame_1box gSpriteBank22Frame005 = {
    SPRITE_FRAME(gSpriteBank22Frame005, SPRITE_TILES_BANK22_FRAME005),
    { { -14, -13, 27, 32 } },
};
const struct sprite_frame_1box gSpriteBank22Frame006 = {
    SPRITE_FRAME(gSpriteBank22Frame006, SPRITE_TILES_BANK22_FRAME006),
    { { -14, -8, 27, 27 } },
};
const struct sprite_frame_1box gSpriteBank22Frame007 = {
    SPRITE_FRAME(gSpriteBank22Frame007, SPRITE_TILES_BANK22_FRAME007),
    { { -12, -8, 25, 27 } },
};
const struct sprite_frame_1box gSpriteBank22Frame008 = {
    SPRITE_FRAME(gSpriteBank22Frame008, SPRITE_TILES_BANK22_FRAME008),
    { { -12, -9, 25, 28 } },
};
const struct sprite_frame_1box gSpriteBank22Frame009 = {
    SPRITE_FRAME(gSpriteBank22Frame009, SPRITE_TILES_BANK22_FRAME009),
    { { -11, -7, 24, 26 } },
};
const struct sprite_frame_1box gSpriteBank22Frame010 = {
    SPRITE_FRAME(gSpriteBank22Frame010, SPRITE_TILES_BANK22_FRAME010),
    { { -12, -5, 25, 24 } },
};
const struct sprite_frame_1box gSpriteBank22Frame011 = {
    SPRITE_FRAME(gSpriteBank22Frame011, SPRITE_TILES_BANK22_FRAME011),
    { { -11, -4, 24, 23 } },
};
const struct sprite_frame_1box gSpriteBank22Frame012 = {
    SPRITE_FRAME(gSpriteBank22Frame012, SPRITE_TILES_BANK22_FRAME012),
    { { -11, -6, 24, 25 } },
};
const struct sprite_frame_1box gSpriteBank22Frame013 = {
    SPRITE_FRAME(gSpriteBank22Frame013, SPRITE_TILES_BANK22_FRAME013),
    { { -11, -7, 24, 26 } },
};
const struct sprite_frame_1box gSpriteBank22Frame014 = {
    SPRITE_FRAME(gSpriteBank22Frame014, SPRITE_TILES_BANK22_FRAME014),
    { { -12, -6, 25, 25 } },
};
const struct sprite_frame_1box gSpriteBank22Frame015 = {
    SPRITE_FRAME(gSpriteBank22Frame015, SPRITE_TILES_BANK22_FRAME015),
    { { -12, -7, 25, 26 } },
};
const struct sprite_frame gSpriteBank22Frame016 = SPRITE_FRAME(gSpriteBank22Frame016, SPRITE_TILES_BANK22_FRAME016);
const struct sprite_frame gSpriteBank22Frame017 = SPRITE_FRAME(gSpriteBank22Frame017, SPRITE_TILES_BANK22_FRAME017);
const struct sprite_frame gSpriteBank22Frame018 = SPRITE_FRAME(gSpriteBank22Frame018, SPRITE_TILES_BANK22_FRAME018);
const struct sprite_frame gSpriteBank22Frame019 = SPRITE_FRAME(gSpriteBank22Frame019, SPRITE_TILES_BANK22_FRAME019);
const struct sprite_frame gSpriteBank22Frame020 = SPRITE_FRAME(gSpriteBank22Frame020, SPRITE_TILES_BANK22_FRAME020);
const struct sprite_frame gSpriteBank22Frame021 = SPRITE_FRAME(gSpriteBank22Frame021, SPRITE_TILES_BANK22_FRAME021);
const struct sprite_frame gSpriteBank22Frame022 = SPRITE_FRAME(gSpriteBank22Frame022, SPRITE_TILES_BANK22_FRAME022);
const struct sprite_frame gSpriteBank22Frame023 = SPRITE_FRAME(gSpriteBank22Frame023, SPRITE_TILES_BANK22_FRAME023);
const struct sprite_frame gSpriteBank22Frame024 = SPRITE_FRAME(gSpriteBank22Frame024, SPRITE_TILES_BANK22_FRAME024);
const struct sprite_frame gSpriteBank22Frame025 = SPRITE_FRAME(gSpriteBank22Frame025, SPRITE_TILES_BANK22_FRAME025);
const struct sprite_frame gSpriteBank22Frame026 = SPRITE_FRAME(gSpriteBank22Frame026, SPRITE_TILES_BANK22_FRAME026);
const struct sprite_frame gSpriteBank22Frame027 = SPRITE_FRAME(gSpriteBank22Frame027, SPRITE_TILES_BANK22_FRAME027);
const struct sprite_frame gSpriteBank22Frame028 = SPRITE_FRAME(gSpriteBank22Frame028, SPRITE_TILES_BANK22_FRAME028);
const struct sprite_frame gSpriteBank22Frame029 = SPRITE_FRAME(gSpriteBank22Frame029, SPRITE_TILES_BANK22_FRAME029);
const struct sprite_frame gSpriteBank22Frame030 = SPRITE_FRAME(gSpriteBank22Frame030, SPRITE_TILES_BANK22_FRAME030);

/* The piece arrays, from graphics/sprites/bank22_homing_sewer_enemy/bank.json. */
#include "sprites/bank22_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 23: 5 animations, 59 frames, graphics/sprites/bank23_flamethrower_lab_assistant/. */

extern const u16 gSpriteBank23Anim00Seq[6];
extern const u16 gSpriteBank23Anim01Seq[14];
extern const u16 gSpriteBank23Anim02Seq[15];
extern const u16 gSpriteBank23Anim03Seq[12];
extern const u16 gSpriteBank23Anim04Seq[12];

extern const struct sprite_anim gSpriteBank23Anims[5] = {
    /* 0 */ {
        /* seq */ gSpriteBank23Anim00Seq,
        /* box */ { { -21, -37, 42, 74 }, { -21, -37, 42, 75 } },
        /* paletteId */ 32,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank23Anim01Seq,
        /* box */ { { -21, -37, 42, 74 }, { -21, -37, 42, 76 } },
        /* paletteId */ 32,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim01Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 2 */ {
        /* seq */ gSpriteBank23Anim02Seq,
        /* box */ { { -21, -37, 42, 74 }, { -22, -38, 48, 76 } },
        /* paletteId */ 32,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank23Anim03Seq,
        /* box */ { { -21, -37, 42, 74 }, { -20, -37, 41, 76 } },
        /* paletteId */ 32,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim03Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 4 */ {
        /* seq */ gSpriteBank23Anim04Seq,
        /* box */ { { -6, -2, 12, 4 }, { -53, -12, 59, 21 } },
        /* paletteId */ 33,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim04Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank23Anim00Seq[6] = {
    0, 1, 2, 3, 4, 5,
};
const u16 gSpriteBank23Anim01Seq[14] = {
    6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
};
const u16 gSpriteBank23Anim02Seq[15] = {
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34,
};
const u16 gSpriteBank23Anim03Seq[12] = {
    35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46,
};
const u16 gSpriteBank23Anim04Seq[12] = {
    47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58,
};

extern const struct sprite_frame *const gSpriteBank23Frames[59] = {
    &gSpriteBank23Frame000.frame,
    &gSpriteBank23Frame001.frame,
    &gSpriteBank23Frame002.frame,
    &gSpriteBank23Frame003.frame,
    &gSpriteBank23Frame004.frame,
    &gSpriteBank23Frame005.frame,
    &gSpriteBank23Frame006.frame,
    &gSpriteBank23Frame007.frame,
    &gSpriteBank23Frame008.frame,
    &gSpriteBank23Frame009.frame,
    &gSpriteBank23Frame010.frame,
    &gSpriteBank23Frame011.frame,
    &gSpriteBank23Frame012.frame,
    &gSpriteBank23Frame013.frame,
    &gSpriteBank23Frame014.frame,
    &gSpriteBank23Frame015.frame,
    &gSpriteBank23Frame016.frame,
    &gSpriteBank23Frame017.frame,
    &gSpriteBank23Frame018.frame,
    &gSpriteBank23Frame019.frame,
    &gSpriteBank23Frame020.frame,
    &gSpriteBank23Frame021.frame,
    &gSpriteBank23Frame022.frame,
    &gSpriteBank23Frame023.frame,
    &gSpriteBank23Frame024.frame,
    &gSpriteBank23Frame025.frame,
    &gSpriteBank23Frame026.frame,
    &gSpriteBank23Frame027.frame,
    &gSpriteBank23Frame028.frame,
    &gSpriteBank23Frame029.frame,
    &gSpriteBank23Frame030.frame,
    &gSpriteBank23Frame031.frame,
    &gSpriteBank23Frame032.frame,
    &gSpriteBank23Frame033.frame,
    &gSpriteBank23Frame034.frame,
    &gSpriteBank23Frame035.frame,
    &gSpriteBank23Frame036.frame,
    &gSpriteBank23Frame037.frame,
    &gSpriteBank23Frame038.frame,
    &gSpriteBank23Frame039.frame,
    &gSpriteBank23Frame040.frame,
    &gSpriteBank23Frame041.frame,
    &gSpriteBank23Frame042.frame,
    &gSpriteBank23Frame043.frame,
    &gSpriteBank23Frame044.frame,
    &gSpriteBank23Frame045.frame,
    &gSpriteBank23Frame046.frame,
    &gSpriteBank23Frame047.frame,
    &gSpriteBank23Frame048.frame,
    &gSpriteBank23Frame049.frame,
    &gSpriteBank23Frame050.frame,
    &gSpriteBank23Frame051.frame,
    &gSpriteBank23Frame052.frame,
    &gSpriteBank23Frame053.frame,
    &gSpriteBank23Frame054.frame,
    &gSpriteBank23Frame055.frame,
    &gSpriteBank23Frame056.frame,
    &gSpriteBank23Frame057.frame,
    &gSpriteBank23Frame058.frame,
};

const struct sprite_frame_1box gSpriteBank23Frame000 = {
    SPRITE_FRAME(gSpriteBank23Frame000, SPRITE_TILES_BANK23_FRAME000),
    { { -13, -25, 27, 54 } },
};
const struct sprite_frame_1box gSpriteBank23Frame001 = {
    SPRITE_FRAME(gSpriteBank23Frame001, SPRITE_TILES_BANK23_FRAME001),
    { { -13, -25, 27, 54 } },
};
const struct sprite_frame_1box gSpriteBank23Frame002 = {
    SPRITE_FRAME(gSpriteBank23Frame002, SPRITE_TILES_BANK23_FRAME002),
    { { -13, -25, 27, 55 } },
};
const struct sprite_frame_1box gSpriteBank23Frame003 = {
    SPRITE_FRAME(gSpriteBank23Frame003, SPRITE_TILES_BANK23_FRAME003),
    { { -12, -25, 26, 55 } },
};
const struct sprite_frame_1box gSpriteBank23Frame004 = {
    SPRITE_FRAME(gSpriteBank23Frame004, SPRITE_TILES_BANK23_FRAME004),
    { { -12, -25, 26, 55 } },
};
const struct sprite_frame_1box gSpriteBank23Frame005 = {
    SPRITE_FRAME(gSpriteBank23Frame005, SPRITE_TILES_BANK23_FRAME005),
    { { -14, -25, 28, 55 } },
};
const struct sprite_frame_1box gSpriteBank23Frame006 = {
    SPRITE_FRAME(gSpriteBank23Frame006, SPRITE_TILES_BANK23_FRAME006),
    { { -14, -29, 28, 58 } },
};
const struct sprite_frame_1box gSpriteBank23Frame007 = {
    SPRITE_FRAME(gSpriteBank23Frame007, SPRITE_TILES_BANK23_FRAME007),
    { { -14, -29, 28, 58 } },
};
const struct sprite_frame_1box gSpriteBank23Frame008 = {
    SPRITE_FRAME(gSpriteBank23Frame008, SPRITE_TILES_BANK23_FRAME008),
    { { -13, -29, 27, 58 } },
};
const struct sprite_frame_1box gSpriteBank23Frame009 = {
    SPRITE_FRAME(gSpriteBank23Frame009, SPRITE_TILES_BANK23_FRAME009),
    { { -10, -29, 24, 60 } },
};
const struct sprite_frame_1box gSpriteBank23Frame010 = {
    SPRITE_FRAME(gSpriteBank23Frame010, SPRITE_TILES_BANK23_FRAME010),
    { { -8, -29, 22, 60 } },
};
const struct sprite_frame_1box gSpriteBank23Frame011 = {
    SPRITE_FRAME(gSpriteBank23Frame011, SPRITE_TILES_BANK23_FRAME011),
    { { -8, -29, 22, 57 } },
};
const struct sprite_frame_1box gSpriteBank23Frame012 = {
    SPRITE_FRAME(gSpriteBank23Frame012, SPRITE_TILES_BANK23_FRAME012),
    { { -8, -29, 22, 57 } },
};
const struct sprite_frame_1box gSpriteBank23Frame013 = {
    SPRITE_FRAME(gSpriteBank23Frame013, SPRITE_TILES_BANK23_FRAME013),
    { { -9, -29, 23, 57 } },
};
const struct sprite_frame_1box gSpriteBank23Frame014 = {
    SPRITE_FRAME(gSpriteBank23Frame014, SPRITE_TILES_BANK23_FRAME014),
    { { -8, -29, 22, 57 } },
};
const struct sprite_frame_1box gSpriteBank23Frame015 = {
    SPRITE_FRAME(gSpriteBank23Frame015, SPRITE_TILES_BANK23_FRAME015),
    { { -9, -29, 23, 57 } },
};
const struct sprite_frame_1box gSpriteBank23Frame016 = {
    SPRITE_FRAME(gSpriteBank23Frame016, SPRITE_TILES_BANK23_FRAME016),
    { { -9, -29, 23, 58 } },
};
const struct sprite_frame_1box gSpriteBank23Frame017 = {
    SPRITE_FRAME(gSpriteBank23Frame017, SPRITE_TILES_BANK23_FRAME017),
    { { -9, -29, 23, 58 } },
};
const struct sprite_frame_1box gSpriteBank23Frame018 = {
    SPRITE_FRAME(gSpriteBank23Frame018, SPRITE_TILES_BANK23_FRAME018),
    { { -10, -29, 24, 59 } },
};
const struct sprite_frame_1box gSpriteBank23Frame019 = {
    SPRITE_FRAME(gSpriteBank23Frame019, SPRITE_TILES_BANK23_FRAME019),
    { { -13, -29, 27, 59 } },
};
const struct sprite_frame_1box gSpriteBank23Frame020 = {
    SPRITE_FRAME(gSpriteBank23Frame020, SPRITE_TILES_BANK23_FRAME020),
    { { -16, -27, 31, 54 } },
};
const struct sprite_frame_1box gSpriteBank23Frame021 = {
    SPRITE_FRAME(gSpriteBank23Frame021, SPRITE_TILES_BANK23_FRAME021),
    { { -16, -27, 31, 54 } },
};
const struct sprite_frame_1box gSpriteBank23Frame022 = {
    SPRITE_FRAME(gSpriteBank23Frame022, SPRITE_TILES_BANK23_FRAME022),
    { { -15, -27, 30, 55 } },
};
const struct sprite_frame_1box gSpriteBank23Frame023 = {
    SPRITE_FRAME(gSpriteBank23Frame023, SPRITE_TILES_BANK23_FRAME023),
    { { -13, -27, 28, 55 } },
};
const struct sprite_frame_1box gSpriteBank23Frame024 = {
    SPRITE_FRAME(gSpriteBank23Frame024, SPRITE_TILES_BANK23_FRAME024),
    { { -13, -27, 28, 55 } },
};
const struct sprite_frame_1box gSpriteBank23Frame025 = {
    SPRITE_FRAME(gSpriteBank23Frame025, SPRITE_TILES_BANK23_FRAME025),
    { { -14, -27, 29, 55 } },
};
const struct sprite_frame_1box gSpriteBank23Frame026 = {
    SPRITE_FRAME(gSpriteBank23Frame026, SPRITE_TILES_BANK23_FRAME026),
    { { -14, -27, 29, 54 } },
};
const struct sprite_frame_1box gSpriteBank23Frame027 = {
    SPRITE_FRAME(gSpriteBank23Frame027, SPRITE_TILES_BANK23_FRAME027),
    { { -14, -27, 29, 54 } },
};
const struct sprite_frame_1box gSpriteBank23Frame028 = {
    SPRITE_FRAME(gSpriteBank23Frame028, SPRITE_TILES_BANK23_FRAME028),
    { { -3, -28, 22, 51 } },
};
const struct sprite_frame_1box gSpriteBank23Frame029 = {
    SPRITE_FRAME(gSpriteBank23Frame029, SPRITE_TILES_BANK23_FRAME029),
    { { -6, -28, 26, 45 } },
};
const struct sprite_frame_1box gSpriteBank23Frame030 = {
    SPRITE_FRAME(gSpriteBank23Frame030, SPRITE_TILES_BANK23_FRAME030),
    { { -7, -28, 26, 47 } },
};
const struct sprite_frame_1box gSpriteBank23Frame031 = {
    SPRITE_FRAME(gSpriteBank23Frame031, SPRITE_TILES_BANK23_FRAME031),
    { { -6, -27, 24, 51 } },
};
const struct sprite_frame_1box gSpriteBank23Frame032 = {
    SPRITE_FRAME(gSpriteBank23Frame032, SPRITE_TILES_BANK23_FRAME032),
    { { -9, -27, 25, 55 } },
};
const struct sprite_frame_1box gSpriteBank23Frame033 = {
    SPRITE_FRAME(gSpriteBank23Frame033, SPRITE_TILES_BANK23_FRAME033),
    { { -12, -27, 27, 54 } },
};
const struct sprite_frame_1box gSpriteBank23Frame034 = {
    SPRITE_FRAME(gSpriteBank23Frame034, SPRITE_TILES_BANK23_FRAME034),
    { { -13, -27, 28, 54 } },
};
const struct sprite_frame_1box gSpriteBank23Frame035 = {
    SPRITE_FRAME(gSpriteBank23Frame035, SPRITE_TILES_BANK23_FRAME035),
    { { -11, -30, 25, 61 } },
};
const struct sprite_frame_1box gSpriteBank23Frame036 = {
    SPRITE_FRAME(gSpriteBank23Frame036, SPRITE_TILES_BANK23_FRAME036),
    { { -11, -30, 25, 62 } },
};
const struct sprite_frame_1box gSpriteBank23Frame037 = {
    SPRITE_FRAME(gSpriteBank23Frame037, SPRITE_TILES_BANK23_FRAME037),
    { { -12, -30, 25, 61 } },
};
const struct sprite_frame_1box gSpriteBank23Frame038 = {
    SPRITE_FRAME(gSpriteBank23Frame038, SPRITE_TILES_BANK23_FRAME038),
    { { -12, -30, 25, 59 } },
};
const struct sprite_frame_1box gSpriteBank23Frame039 = {
    SPRITE_FRAME(gSpriteBank23Frame039, SPRITE_TILES_BANK23_FRAME039),
    { { -13, -30, 26, 58 } },
};
const struct sprite_frame_1box gSpriteBank23Frame040 = {
    SPRITE_FRAME(gSpriteBank23Frame040, SPRITE_TILES_BANK23_FRAME040),
    { { -13, -30, 26, 56 } },
};
const struct sprite_frame_1box gSpriteBank23Frame041 = {
    SPRITE_FRAME(gSpriteBank23Frame041, SPRITE_TILES_BANK23_FRAME041),
    { { -13, -30, 26, 57 } },
};
const struct sprite_frame_1box gSpriteBank23Frame042 = {
    SPRITE_FRAME(gSpriteBank23Frame042, SPRITE_TILES_BANK23_FRAME042),
    { { -13, -30, 26, 58 } },
};
const struct sprite_frame_1box gSpriteBank23Frame043 = {
    SPRITE_FRAME(gSpriteBank23Frame043, SPRITE_TILES_BANK23_FRAME043),
    { { -11, -30, 24, 60 } },
};
const struct sprite_frame_1box gSpriteBank23Frame044 = {
    SPRITE_FRAME(gSpriteBank23Frame044, SPRITE_TILES_BANK23_FRAME044),
    { { -11, -30, 24, 61 } },
};
const struct sprite_frame_1box gSpriteBank23Frame045 = {
    SPRITE_FRAME(gSpriteBank23Frame045, SPRITE_TILES_BANK23_FRAME045),
    { { -11, -30, 25, 61 } },
};
const struct sprite_frame_1box gSpriteBank23Frame046 = {
    SPRITE_FRAME(gSpriteBank23Frame046, SPRITE_TILES_BANK23_FRAME046),
    { { -11, -30, 25, 60 } },
};
const struct sprite_frame_1box gSpriteBank23Frame047 = {
    SPRITE_FRAME(gSpriteBank23Frame047, SPRITE_TILES_BANK23_FRAME047),
    { { -6, -2, 12, 4 } },
};
const struct sprite_frame_1box gSpriteBank23Frame048 = {
    SPRITE_FRAME(gSpriteBank23Frame048, SPRITE_TILES_BANK23_FRAME048),
    { { -9, -3, 15, 6 } },
};
const struct sprite_frame_1box gSpriteBank23Frame049 = {
    SPRITE_FRAME(gSpriteBank23Frame049, SPRITE_TILES_BANK23_FRAME049),
    { { -23, -8, 29, 11 } },
};
const struct sprite_frame_1box gSpriteBank23Frame050 = {
    SPRITE_FRAME(gSpriteBank23Frame050, SPRITE_TILES_BANK23_FRAME050),
    { { -30, -8, 36, 13 } },
};
const struct sprite_frame_1box gSpriteBank23Frame051 = {
    SPRITE_FRAME(gSpriteBank23Frame051, SPRITE_TILES_BANK23_FRAME051),
    { { -34, -8, 40, 15 } },
};
const struct sprite_frame_1box gSpriteBank23Frame052 = {
    SPRITE_FRAME(gSpriteBank23Frame052, SPRITE_TILES_BANK23_FRAME052),
    { { -36, -12, 42, 21 } },
};
const struct sprite_frame_1box gSpriteBank23Frame053 = {
    SPRITE_FRAME(gSpriteBank23Frame053, SPRITE_TILES_BANK23_FRAME053),
    { { -43, -12, 48, 21 } },
};
const struct sprite_frame_1box gSpriteBank23Frame054 = {
    SPRITE_FRAME(gSpriteBank23Frame054, SPRITE_TILES_BANK23_FRAME054),
    { { -40, -12, 45, 18 } },
};
const struct sprite_frame_1box gSpriteBank23Frame055 = {
    SPRITE_FRAME(gSpriteBank23Frame055, SPRITE_TILES_BANK23_FRAME055),
    { { -46, -11, 43, 17 } },
};
const struct sprite_frame_1box gSpriteBank23Frame056 = {
    SPRITE_FRAME(gSpriteBank23Frame056, SPRITE_TILES_BANK23_FRAME056),
    { { -48, -5, 29, 9 } },
};
const struct sprite_frame_1box gSpriteBank23Frame057 = {
    SPRITE_FRAME(gSpriteBank23Frame057, SPRITE_TILES_BANK23_FRAME057),
    { { -51, -4, 15, 7 } },
};
const struct sprite_frame_1box gSpriteBank23Frame058 = {
    SPRITE_FRAME(gSpriteBank23Frame058, SPRITE_TILES_BANK23_FRAME058),
    { { -53, -3, 11, 4 } },
};

/* The piece arrays, from graphics/sprites/bank23_flamethrower_lab_assistant/bank.json. */
#include "sprites/bank23_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 24: 6 animations, 74 frames, graphics/sprites/bank24_patrolling_space_enemy/. */

extern const u16 gSpriteBank24Anim00Seq[13];
extern const u16 gSpriteBank24Anim01Seq[11];
extern const u16 gSpriteBank24Anim02Seq[13];
extern const u16 gSpriteBank24Anim03Seq[13];
extern const u16 gSpriteBank24Anim04Seq[11];
extern const u16 gSpriteBank24Anim05Seq[13];

extern const struct sprite_anim gSpriteBank24Anims[6] = {
    /* 0 */ {
        /* seq */ gSpriteBank24Anim00Seq,
        /* box */ { { -15, -15, 30, 30 }, { -21, -22, 45, 39 } },
        /* paletteId */ 34,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank24Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank24Anim01Seq,
        /* box */ { { -15, -15, 30, 30 }, { -30, -23, 63, 39 } },
        /* paletteId */ 34,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank24Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank24Anim02Seq,
        /* box */ { { -15, -15, 30, 30 }, { -30, -22, 62, 42 } },
        /* paletteId */ 34,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank24Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank24Anim03Seq,
        /* box */ { { -15, -15, 30, 30 }, { -15, -23, 30, 39 } },
        /* paletteId */ 34,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank24Anim03Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 4 */ {
        /* seq */ gSpriteBank24Anim04Seq,
        /* box */ { { -15, -15, 30, 30 }, { -30, -23, 63, 39 } },
        /* paletteId */ 34,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank24Anim04Seq),
        /* flags */ 0,
    },
    /* 5 */ {
        /* seq */ gSpriteBank24Anim05Seq,
        /* box */ { { -15, -15, 30, 30 }, { -30, -23, 63, 39 } },
        /* paletteId */ 34,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank24Anim05Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank24Anim00Seq[13] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
};
const u16 gSpriteBank24Anim01Seq[11] = {
    13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
};
const u16 gSpriteBank24Anim02Seq[13] = {
    24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36,
};
const u16 gSpriteBank24Anim03Seq[13] = {
    37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
};
const u16 gSpriteBank24Anim04Seq[11] = {
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
};
const u16 gSpriteBank24Anim05Seq[13] = {
    61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73,
};

extern const struct sprite_frame *const gSpriteBank24Frames[74] = {
    &gSpriteBank24Frame000.frame,
    &gSpriteBank24Frame001.frame,
    &gSpriteBank24Frame002.frame,
    &gSpriteBank24Frame003.frame,
    &gSpriteBank24Frame004.frame,
    &gSpriteBank24Frame005.frame,
    &gSpriteBank24Frame006.frame,
    &gSpriteBank24Frame007.frame,
    &gSpriteBank24Frame008.frame,
    &gSpriteBank24Frame009.frame,
    &gSpriteBank24Frame010.frame,
    &gSpriteBank24Frame011.frame,
    &gSpriteBank24Frame012.frame,
    &gSpriteBank24Frame013.frame,
    &gSpriteBank24Frame014.frame,
    &gSpriteBank24Frame015.frame,
    &gSpriteBank24Frame016.frame,
    &gSpriteBank24Frame017.frame,
    &gSpriteBank24Frame018.frame,
    &gSpriteBank24Frame019.frame,
    &gSpriteBank24Frame020.frame,
    &gSpriteBank24Frame021.frame,
    &gSpriteBank24Frame022.frame,
    &gSpriteBank24Frame023.frame,
    &gSpriteBank24Frame024.frame,
    &gSpriteBank24Frame025.frame,
    &gSpriteBank24Frame026.frame,
    &gSpriteBank24Frame027.frame,
    &gSpriteBank24Frame028.frame,
    &gSpriteBank24Frame029.frame,
    &gSpriteBank24Frame030.frame,
    &gSpriteBank24Frame031.frame,
    &gSpriteBank24Frame032.frame,
    &gSpriteBank24Frame033.frame,
    &gSpriteBank24Frame034.frame,
    &gSpriteBank24Frame035.frame,
    &gSpriteBank24Frame036.frame,
    &gSpriteBank24Frame037.frame,
    &gSpriteBank24Frame038.frame,
    &gSpriteBank24Frame039.frame,
    &gSpriteBank24Frame040.frame,
    &gSpriteBank24Frame041.frame,
    &gSpriteBank24Frame042.frame,
    &gSpriteBank24Frame043.frame,
    &gSpriteBank24Frame044.frame,
    &gSpriteBank24Frame045.frame,
    &gSpriteBank24Frame046.frame,
    &gSpriteBank24Frame047.frame,
    &gSpriteBank24Frame048.frame,
    &gSpriteBank24Frame049.frame,
    &gSpriteBank24Frame050.frame,
    &gSpriteBank24Frame051.frame,
    &gSpriteBank24Frame052.frame,
    &gSpriteBank24Frame053.frame,
    &gSpriteBank24Frame054.frame,
    &gSpriteBank24Frame055.frame,
    &gSpriteBank24Frame056.frame,
    &gSpriteBank24Frame057.frame,
    &gSpriteBank24Frame058.frame,
    &gSpriteBank24Frame059.frame,
    &gSpriteBank24Frame060.frame,
    &gSpriteBank24Frame061.frame,
    &gSpriteBank24Frame062.frame,
    &gSpriteBank24Frame063.frame,
    &gSpriteBank24Frame064.frame,
    &gSpriteBank24Frame065.frame,
    &gSpriteBank24Frame066.frame,
    &gSpriteBank24Frame067.frame,
    &gSpriteBank24Frame068.frame,
    &gSpriteBank24Frame069.frame,
    &gSpriteBank24Frame070.frame,
    &gSpriteBank24Frame071.frame,
    &gSpriteBank24Frame072.frame,
    &gSpriteBank24Frame073.frame,
};

const struct sprite_frame_2box gSpriteBank24Frame000 = {
    SPRITE_FRAME(gSpriteBank24Frame000, SPRITE_TILES_BANK24_FRAME000),
    { { -16, -1, 32, 16 }, { -8, -15, 19, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame001 = {
    SPRITE_FRAME(gSpriteBank24Frame001, SPRITE_TILES_BANK24_FRAME001),
    { { -16, -1, 32, 16 }, { -8, -15, 19, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame002 = {
    SPRITE_FRAME(gSpriteBank24Frame002, SPRITE_TILES_BANK24_FRAME002),
    { { -16, -1, 32, 16 }, { -8, -15, 19, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame003 = {
    SPRITE_FRAME(gSpriteBank24Frame003, SPRITE_TILES_BANK24_FRAME003),
    { { -16, -1, 32, 16 }, { -8, -15, 19, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame004 = {
    SPRITE_FRAME(gSpriteBank24Frame004, SPRITE_TILES_BANK24_FRAME004),
    { { -16, -1, 32, 16 }, { -8, -15, 19, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame005 = {
    SPRITE_FRAME(gSpriteBank24Frame005, SPRITE_TILES_BANK24_FRAME005),
    { { -16, -1, 32, 16 }, { -8, -15, 19, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame006 = {
    SPRITE_FRAME(gSpriteBank24Frame006, SPRITE_TILES_BANK24_FRAME006),
    { { -16, -1, 32, 16 }, { -8, -15, 19, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame007 = {
    SPRITE_FRAME(gSpriteBank24Frame007, SPRITE_TILES_BANK24_FRAME007),
    { { -16, -1, 32, 16 }, { -8, -15, 19, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame008 = {
    SPRITE_FRAME(gSpriteBank24Frame008, SPRITE_TILES_BANK24_FRAME008),
    { { -16, -1, 32, 16 }, { -8, -15, 19, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame009 = {
    SPRITE_FRAME(gSpriteBank24Frame009, SPRITE_TILES_BANK24_FRAME009),
    { { -16, -1, 32, 16 }, { -8, -15, 19, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame010 = {
    SPRITE_FRAME(gSpriteBank24Frame010, SPRITE_TILES_BANK24_FRAME010),
    { { -16, -1, 32, 16 }, { -8, -15, 19, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame011 = {
    SPRITE_FRAME(gSpriteBank24Frame011, SPRITE_TILES_BANK24_FRAME011),
    { { -16, -1, 32, 16 }, { -8, -15, 19, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame012 = {
    SPRITE_FRAME(gSpriteBank24Frame012, SPRITE_TILES_BANK24_FRAME012),
    { { -16, -1, 32, 16 }, { -8, -15, 19, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame013 = {
    SPRITE_FRAME(gSpriteBank24Frame013, SPRITE_TILES_BANK24_FRAME013),
    { { -14, -13, 33, 18 }, { -28, 5, 59, 7 } },
};
const struct sprite_frame_2box gSpriteBank24Frame014 = {
    SPRITE_FRAME(gSpriteBank24Frame014, SPRITE_TILES_BANK24_FRAME014),
    { { -16, -12, 36, 16 }, { -27, 3, 57, 8 } },
};
const struct sprite_frame_2box gSpriteBank24Frame015 = {
    SPRITE_FRAME(gSpriteBank24Frame015, SPRITE_TILES_BANK24_FRAME015),
    { { -15, -14, 36, 18 }, { -27, 3, 57, 8 } },
};
const struct sprite_frame_2box gSpriteBank24Frame016 = {
    SPRITE_FRAME(gSpriteBank24Frame016, SPRITE_TILES_BANK24_FRAME016),
    { { -17, -10, 38, 14 }, { -27, 1, 58, 12 } },
};
const struct sprite_frame_2box gSpriteBank24Frame017 = {
    SPRITE_FRAME(gSpriteBank24Frame017, SPRITE_TILES_BANK24_FRAME017),
    { { -16, -13, 37, 19 }, { -27, 5, 58, 8 } },
};
const struct sprite_frame_2box gSpriteBank24Frame018 = {
    SPRITE_FRAME(gSpriteBank24Frame018, SPRITE_TILES_BANK24_FRAME018),
    { { -15, -14, 35, 16 }, { -28, 1, 58, 7 } },
};
const struct sprite_frame_2box gSpriteBank24Frame019 = {
    SPRITE_FRAME(gSpriteBank24Frame019, SPRITE_TILES_BANK24_FRAME019),
    { { -11, -20, 24, 34 }, { -23, -7, 51, 5 } },
};
const struct sprite_frame_2box gSpriteBank24Frame020 = {
    SPRITE_FRAME(gSpriteBank24Frame020, SPRITE_TILES_BANK24_FRAME020),
    { { -14, 3, 29, 13 }, { -16, -17, 32, 9 } },
};
const struct sprite_frame_2box gSpriteBank24Frame021 = {
    SPRITE_FRAME(gSpriteBank24Frame021, SPRITE_TILES_BANK24_FRAME021),
    { { -16, 0, 32, 16 }, { -12, -13, 25, 8 } },
};
const struct sprite_frame_2box gSpriteBank24Frame022 = {
    SPRITE_FRAME(gSpriteBank24Frame022, SPRITE_TILES_BANK24_FRAME022),
    { { -16, -2, 32, 18 }, { -12, -14, 25, 8 } },
};
const struct sprite_frame_2box gSpriteBank24Frame023 = {
    SPRITE_FRAME(gSpriteBank24Frame023, SPRITE_TILES_BANK24_FRAME023),
    { { -16, -3, 31, 18 }, { -14, -14, 28, 7 } },
};
const struct sprite_frame_2box gSpriteBank24Frame024 = {
    SPRITE_FRAME(gSpriteBank24Frame024, SPRITE_TILES_BANK24_FRAME024),
    { { -11, -16, 26, 19 }, { -28, 3, 60, 9 } },
};
const struct sprite_frame_2box gSpriteBank24Frame025 = {
    SPRITE_FRAME(gSpriteBank24Frame025, SPRITE_TILES_BANK24_FRAME025),
    { { -11, -15, 26, 19 }, { -28, 4, 60, 9 } },
};
const struct sprite_frame_2box gSpriteBank24Frame026 = {
    SPRITE_FRAME(gSpriteBank24Frame026, SPRITE_TILES_BANK24_FRAME026),
    { { -12, -15, 22, 26 }, { -26, 8, 56, 6 } },
};
const struct sprite_frame_2box gSpriteBank24Frame027 = {
    SPRITE_FRAME(gSpriteBank24Frame027, SPRITE_TILES_BANK24_FRAME027),
    { { -12, -14, 16, 22 }, { -24, 7, 51, 7 } },
};
const struct sprite_frame_2box gSpriteBank24Frame028 = {
    SPRITE_FRAME(gSpriteBank24Frame028, SPRITE_TILES_BANK24_FRAME028),
    { { -10, -15, 19, 25 }, { -20, 8, 45, 8 } },
};
const struct sprite_frame_2box gSpriteBank24Frame029 = {
    SPRITE_FRAME(gSpriteBank24Frame029, SPRITE_TILES_BANK24_FRAME029),
    { { -8, -13, 15, 22 }, { -14, 8, 38, 9 } },
};
const struct sprite_frame_2box gSpriteBank24Frame030 = {
    SPRITE_FRAME(gSpriteBank24Frame030, SPRITE_TILES_BANK24_FRAME030),
    { { -7, -14, 18, 21 }, { -18, 4, 38, 13 } },
};
const struct sprite_frame_2box gSpriteBank24Frame031 = {
    SPRITE_FRAME(gSpriteBank24Frame031, SPRITE_TILES_BANK24_FRAME031),
    { { -3, -15, 15, 25 }, { -22, 3, 40, 14 } },
};
const struct sprite_frame_2box gSpriteBank24Frame032 = {
    SPRITE_FRAME(gSpriteBank24Frame032, SPRITE_TILES_BANK24_FRAME032),
    { { -2, -12, 17, 25 }, { -22, 9, 45, 7 } },
};
const struct sprite_frame_2box gSpriteBank24Frame033 = {
    SPRITE_FRAME(gSpriteBank24Frame033, SPRITE_TILES_BANK24_FRAME033),
    { { -4, -12, 20, 21 }, { -24, 11, 51, 4 } },
};
const struct sprite_frame_2box gSpriteBank24Frame034 = {
    SPRITE_FRAME(gSpriteBank24Frame034, SPRITE_TILES_BANK24_FRAME034),
    { { -5, -15, 22, 24 }, { -28, 9, 58, 5 } },
};
const struct sprite_frame_2box gSpriteBank24Frame035 = {
    SPRITE_FRAME(gSpriteBank24Frame035, SPRITE_TILES_BANK24_FRAME035),
    { { -6, -14, 24, 23 }, { -29, 10, 60, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame036 = {
    SPRITE_FRAME(gSpriteBank24Frame036, SPRITE_TILES_BANK24_FRAME036),
    { { -6, -15, 24, 26 }, { -30, 8, 61, 5 } },
};
const struct sprite_frame_2box gSpriteBank24Frame037 = {
    SPRITE_FRAME(gSpriteBank24Frame037, SPRITE_TILES_BANK24_FRAME037),
    { { -16, -3, 31, 16 }, { -10, -16, 21, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame038 = {
    SPRITE_FRAME(gSpriteBank24Frame038, SPRITE_TILES_BANK24_FRAME038),
    { { -16, -3, 31, 16 }, { -10, -16, 21, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame039 = {
    SPRITE_FRAME(gSpriteBank24Frame039, SPRITE_TILES_BANK24_FRAME039),
    { { -16, -3, 31, 16 }, { -10, -16, 21, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame040 = {
    SPRITE_FRAME(gSpriteBank24Frame040, SPRITE_TILES_BANK24_FRAME040),
    { { -16, -3, 31, 16 }, { -10, -16, 21, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame041 = {
    SPRITE_FRAME(gSpriteBank24Frame041, SPRITE_TILES_BANK24_FRAME041),
    { { -16, -3, 31, 16 }, { -10, -16, 21, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame042 = {
    SPRITE_FRAME(gSpriteBank24Frame042, SPRITE_TILES_BANK24_FRAME042),
    { { -16, -3, 31, 16 }, { -10, -16, 21, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame043 = {
    SPRITE_FRAME(gSpriteBank24Frame043, SPRITE_TILES_BANK24_FRAME043),
    { { -16, -3, 31, 16 }, { -10, -16, 21, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame044 = {
    SPRITE_FRAME(gSpriteBank24Frame044, SPRITE_TILES_BANK24_FRAME044),
    { { -16, -3, 31, 16 }, { -10, -16, 21, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame045 = {
    SPRITE_FRAME(gSpriteBank24Frame045, SPRITE_TILES_BANK24_FRAME045),
    { { -16, -3, 31, 16 }, { -10, -16, 21, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame046 = {
    SPRITE_FRAME(gSpriteBank24Frame046, SPRITE_TILES_BANK24_FRAME046),
    { { -16, -3, 31, 16 }, { -10, -16, 21, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame047 = {
    SPRITE_FRAME(gSpriteBank24Frame047, SPRITE_TILES_BANK24_FRAME047),
    { { -16, -3, 31, 16 }, { -10, -16, 21, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame048 = {
    SPRITE_FRAME(gSpriteBank24Frame048, SPRITE_TILES_BANK24_FRAME048),
    { { -16, -3, 31, 16 }, { -10, -16, 21, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame049 = {
    SPRITE_FRAME(gSpriteBank24Frame049, SPRITE_TILES_BANK24_FRAME049),
    { { -16, -3, 31, 16 }, { -10, -16, 21, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame050 = {
    SPRITE_FRAME(gSpriteBank24Frame050, SPRITE_TILES_BANK24_FRAME050),
    { { -13, -13, 28, 9 }, { -8, -13, 16, 9 } },
};
const struct sprite_frame_2box gSpriteBank24Frame051 = {
    SPRITE_FRAME(gSpriteBank24Frame051, SPRITE_TILES_BANK24_FRAME051),
    { { -13, -10, 26, 6 }, { -8, -12, 18, 7 } },
};
const struct sprite_frame_2box gSpriteBank24Frame052 = {
    SPRITE_FRAME(gSpriteBank24Frame052, SPRITE_TILES_BANK24_FRAME052),
    { { -13, -11, 26, 7 }, { -9, -10, 19, 6 } },
};
const struct sprite_frame_1box gSpriteBank24Frame053 = {
    SPRITE_FRAME(gSpriteBank24Frame053, SPRITE_TILES_BANK24_FRAME053),
    { { -16, -11, 33, 7 } },
};
const struct sprite_frame_2box gSpriteBank24Frame054 = {
    SPRITE_FRAME(gSpriteBank24Frame054, SPRITE_TILES_BANK24_FRAME054),
    { { -12, -22, 25, 35 }, { -20, -7, 44, 8 } },
};
const struct sprite_frame_2box gSpriteBank24Frame055 = {
    SPRITE_FRAME(gSpriteBank24Frame055, SPRITE_TILES_BANK24_FRAME055),
    { { -12, -15, 22, 29 }, { -26, 0, 56, 6 } },
};
const struct sprite_frame_2box gSpriteBank24Frame056 = {
    SPRITE_FRAME(gSpriteBank24Frame056, SPRITE_TILES_BANK24_FRAME056),
    { { -11, -15, 17, 27 }, { -27, 6, 58, 5 } },
};
const struct sprite_frame_2box gSpriteBank24Frame057 = {
    SPRITE_FRAME(gSpriteBank24Frame057, SPRITE_TILES_BANK24_FRAME057),
    { { -12, -15, 18, 29 }, { -27, 3, 57, 8 } },
};
const struct sprite_frame_2box gSpriteBank24Frame058 = {
    SPRITE_FRAME(gSpriteBank24Frame058, SPRITE_TILES_BANK24_FRAME058),
    { { -13, -14, 20, 29 }, { -26, 2, 55, 8 } },
};
const struct sprite_frame_2box gSpriteBank24Frame059 = {
    SPRITE_FRAME(gSpriteBank24Frame059, SPRITE_TILES_BANK24_FRAME059),
    { { -11, -14, 20, 28 }, { -27, 3, 56, 7 } },
};
const struct sprite_frame_2box gSpriteBank24Frame060 = {
    SPRITE_FRAME(gSpriteBank24Frame060, SPRITE_TILES_BANK24_FRAME060),
    { { -10, -16, 19, 29 }, { -25, 1, 54, 9 } },
};
const struct sprite_frame_2box gSpriteBank24Frame061 = {
    SPRITE_FRAME(gSpriteBank24Frame061, SPRITE_TILES_BANK24_FRAME061),
    { { -15, -17, 30, 24 }, { -27, 3, 57, 9 } },
};
const struct sprite_frame_2box gSpriteBank24Frame062 = {
    SPRITE_FRAME(gSpriteBank24Frame062, SPRITE_TILES_BANK24_FRAME062),
    { { -15, -17, 30, 24 }, { -27, 0, 57, 13 } },
};
const struct sprite_frame_2box gSpriteBank24Frame063 = {
    SPRITE_FRAME(gSpriteBank24Frame063, SPRITE_TILES_BANK24_FRAME063),
    { { -15, -17, 30, 24 }, { -27, 4, 57, 9 } },
};
const struct sprite_frame_2box gSpriteBank24Frame064 = {
    SPRITE_FRAME(gSpriteBank24Frame064, SPRITE_TILES_BANK24_FRAME064),
    { { -15, -17, 30, 24 }, { -27, 6, 57, 7 } },
};
const struct sprite_frame_2box gSpriteBank24Frame065 = {
    SPRITE_FRAME(gSpriteBank24Frame065, SPRITE_TILES_BANK24_FRAME065),
    { { -15, -17, 30, 24 }, { -27, 4, 57, 9 } },
};
const struct sprite_frame_2box gSpriteBank24Frame066 = {
    SPRITE_FRAME(gSpriteBank24Frame066, SPRITE_TILES_BANK24_FRAME066),
    { { -15, -17, 30, 24 }, { -27, 6, 57, 7 } },
};
const struct sprite_frame_2box gSpriteBank24Frame067 = {
    SPRITE_FRAME(gSpriteBank24Frame067, SPRITE_TILES_BANK24_FRAME067),
    { { -15, -17, 30, 24 }, { -27, 3, 57, 10 } },
};
const struct sprite_frame_2box gSpriteBank24Frame068 = {
    SPRITE_FRAME(gSpriteBank24Frame068, SPRITE_TILES_BANK24_FRAME068),
    { { -15, -17, 30, 24 }, { -27, 0, 57, 12 } },
};
const struct sprite_frame_2box gSpriteBank24Frame069 = {
    SPRITE_FRAME(gSpriteBank24Frame069, SPRITE_TILES_BANK24_FRAME069),
    { { -15, -17, 30, 24 }, { -26, 4, 56, 8 } },
};
const struct sprite_frame_2box gSpriteBank24Frame070 = {
    SPRITE_FRAME(gSpriteBank24Frame070, SPRITE_TILES_BANK24_FRAME070),
    { { -15, -17, 30, 24 }, { -26, 9, 56, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame071 = {
    SPRITE_FRAME(gSpriteBank24Frame071, SPRITE_TILES_BANK24_FRAME071),
    { { -15, -17, 30, 24 }, { -26, 8, 56, 4 } },
};
const struct sprite_frame_2box gSpriteBank24Frame072 = {
    SPRITE_FRAME(gSpriteBank24Frame072, SPRITE_TILES_BANK24_FRAME072),
    { { -15, -17, 30, 24 }, { -26, 7, 56, 5 } },
};
const struct sprite_frame_2box gSpriteBank24Frame073 = {
    SPRITE_FRAME(gSpriteBank24Frame073, SPRITE_TILES_BANK24_FRAME073),
    { { -15, -17, 30, 24 }, { -27, 8, 57, 4 } },
};

/* The piece arrays, from graphics/sprites/bank24_patrolling_space_enemy/bank.json. */
#include "sprites/bank24_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 25: 1 animation, 16 frames, graphics/sprites/bank25_laser_barrier/. */

extern const u16 gSpriteBank25Anim00Seq[16];

extern const struct sprite_anim gSpriteBank25Anims[1] = {
    /* 0 */ {
        /* seq */ gSpriteBank25Anim00Seq,
        /* box */ { { -30, -22, 60, 44 }, { -31, -32, 61, 59 } },
        /* paletteId */ 35,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank25Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank25Anim00Seq[16] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
};

extern const struct sprite_frame *const gSpriteBank25Frames[16] = {
    &gSpriteBank25Frame000.frame,
    &gSpriteBank25Frame001.frame,
    &gSpriteBank25Frame002.frame,
    &gSpriteBank25Frame003.frame,
    &gSpriteBank25Frame004.frame,
    &gSpriteBank25Frame005.frame,
    &gSpriteBank25Frame006.frame,
    &gSpriteBank25Frame007.frame,
    &gSpriteBank25Frame008.frame,
    &gSpriteBank25Frame009.frame,
    &gSpriteBank25Frame010.frame,
    &gSpriteBank25Frame011.frame,
    &gSpriteBank25Frame012.frame,
    &gSpriteBank25Frame013.frame,
    &gSpriteBank25Frame014.frame,
    &gSpriteBank25Frame015.frame,
};

const struct sprite_frame_1box gSpriteBank25Frame000 = {
    SPRITE_FRAME(gSpriteBank25Frame000, SPRITE_TILES_BANK25_FRAME000),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame001 = {
    SPRITE_FRAME(gSpriteBank25Frame001, SPRITE_TILES_BANK25_FRAME001),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame002 = {
    SPRITE_FRAME(gSpriteBank25Frame002, SPRITE_TILES_BANK25_FRAME002),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame003 = {
    SPRITE_FRAME(gSpriteBank25Frame003, SPRITE_TILES_BANK25_FRAME003),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame004 = {
    SPRITE_FRAME(gSpriteBank25Frame004, SPRITE_TILES_BANK25_FRAME004),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame005 = {
    SPRITE_FRAME(gSpriteBank25Frame005, SPRITE_TILES_BANK25_FRAME005),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame006 = {
    SPRITE_FRAME(gSpriteBank25Frame006, SPRITE_TILES_BANK25_FRAME006),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame007 = {
    SPRITE_FRAME(gSpriteBank25Frame007, SPRITE_TILES_BANK25_FRAME007),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame008 = {
    SPRITE_FRAME(gSpriteBank25Frame008, SPRITE_TILES_BANK25_FRAME008),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame009 = {
    SPRITE_FRAME(gSpriteBank25Frame009, SPRITE_TILES_BANK25_FRAME009),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame010 = {
    SPRITE_FRAME(gSpriteBank25Frame010, SPRITE_TILES_BANK25_FRAME010),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame011 = {
    SPRITE_FRAME(gSpriteBank25Frame011, SPRITE_TILES_BANK25_FRAME011),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame012 = {
    SPRITE_FRAME(gSpriteBank25Frame012, SPRITE_TILES_BANK25_FRAME012),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame013 = {
    SPRITE_FRAME(gSpriteBank25Frame013, SPRITE_TILES_BANK25_FRAME013),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame014 = {
    SPRITE_FRAME(gSpriteBank25Frame014, SPRITE_TILES_BANK25_FRAME014),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame015 = {
    SPRITE_FRAME(gSpriteBank25Frame015, SPRITE_TILES_BANK25_FRAME015),
    { { -17, -1, 18, 17 } },
};

/* The piece arrays, from graphics/sprites/bank25_laser_barrier/bank.json. */
#include "sprites/bank25_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 26: 2 animations, 14 frames, graphics/sprites/bank26_piston_crusher/. */

extern const u16 gSpriteBank26Anim00Seq[9];
extern const u16 gSpriteBank26Anim01Seq[5];

extern const struct sprite_anim gSpriteBank26Anims[2] = {
    /* 0 */ {
        /* seq */ gSpriteBank26Anim00Seq,
        /* box */ { { -15, -49, 30, 99 }, { -15, -49, 30, 99 } },
        /* paletteId */ 36,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank26Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank26Anim01Seq,
        /* box */ { { -15, -49, 30, 99 }, { -15, -49, 30, 99 } },
        /* paletteId */ 36,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank26Anim01Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank26Anim00Seq[9] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8,
};
const u16 gSpriteBank26Anim01Seq[5] = {
    9, 10, 11, 12, 13,
};

extern const struct sprite_frame *const gSpriteBank26Frames[14] = {
    &gSpriteBank26Frame000.frame,
    &gSpriteBank26Frame001.frame,
    &gSpriteBank26Frame002.frame,
    &gSpriteBank26Frame003.frame,
    &gSpriteBank26Frame004.frame,
    &gSpriteBank26Frame005.frame,
    &gSpriteBank26Frame006.frame,
    &gSpriteBank26Frame007.frame,
    &gSpriteBank26Frame008.frame,
    &gSpriteBank26Frame009.frame,
    &gSpriteBank26Frame010.frame,
    &gSpriteBank26Frame011.frame,
    &gSpriteBank26Frame012.frame,
    &gSpriteBank26Frame013.frame,
};

const struct sprite_frame_1box gSpriteBank26Frame000 = {
    SPRITE_FRAME(gSpriteBank26Frame000, SPRITE_TILES_BANK26_FRAME000),
    { { -10, -44, 19, 90 } },
};
const struct sprite_frame_1box gSpriteBank26Frame001 = {
    SPRITE_FRAME(gSpriteBank26Frame001, SPRITE_TILES_BANK26_FRAME001),
    { { -9, -43, 18, 84 } },
};
const struct sprite_frame_1box gSpriteBank26Frame002 = {
    SPRITE_FRAME(gSpriteBank26Frame002, SPRITE_TILES_BANK26_FRAME002),
    { { -10, -41, 20, 74 } },
};
const struct sprite_frame_1box gSpriteBank26Frame003 = {
    SPRITE_FRAME(gSpriteBank26Frame003, SPRITE_TILES_BANK26_FRAME003),
    { { -9, -40, 19, 60 } },
};
const struct sprite_frame_1box gSpriteBank26Frame004 = {
    SPRITE_FRAME(gSpriteBank26Frame004, SPRITE_TILES_BANK26_FRAME004),
    { { -10, -40, 22, 50 } },
};
const struct sprite_frame_1box gSpriteBank26Frame005 = {
    SPRITE_FRAME(gSpriteBank26Frame005, SPRITE_TILES_BANK26_FRAME005),
    { { -11, -38, 23, 35 } },
};
const struct sprite_frame_1box gSpriteBank26Frame006 = {
    SPRITE_FRAME(gSpriteBank26Frame006, SPRITE_TILES_BANK26_FRAME006),
    { { -11, -41, 23, 29 } },
};
const struct sprite_frame_1box gSpriteBank26Frame007 = {
    SPRITE_FRAME(gSpriteBank26Frame007, SPRITE_TILES_BANK26_FRAME007),
    { { -11, -39, 23, 18 } },
};
const struct sprite_frame_1box gSpriteBank26Frame008 = {
    SPRITE_FRAME(gSpriteBank26Frame008, SPRITE_TILES_BANK26_FRAME008),
    { { -12, -40, 25, 19 } },
};
const struct sprite_frame_1box gSpriteBank26Frame009 = {
    SPRITE_FRAME(gSpriteBank26Frame009, SPRITE_TILES_BANK26_FRAME009),
    { { -10, -39, 21, 18 } },
};
const struct sprite_frame_1box gSpriteBank26Frame010 = {
    SPRITE_FRAME(gSpriteBank26Frame010, SPRITE_TILES_BANK26_FRAME010),
    { { -12, -38, 25, 21 } },
};
const struct sprite_frame_1box gSpriteBank26Frame011 = {
    SPRITE_FRAME(gSpriteBank26Frame011, SPRITE_TILES_BANK26_FRAME011),
    { { -12, -38, 25, 31 } },
};
const struct sprite_frame_1box gSpriteBank26Frame012 = {
    SPRITE_FRAME(gSpriteBank26Frame012, SPRITE_TILES_BANK26_FRAME012),
    { { -11, -40, 23, 55 } },
};
const struct sprite_frame_1box gSpriteBank26Frame013 = {
    SPRITE_FRAME(gSpriteBank26Frame013, SPRITE_TILES_BANK26_FRAME013),
    { { -11, -38, 21, 83 } },
};

/* The piece arrays, from graphics/sprites/bank26_piston_crusher/bank.json. */
#include "sprites/bank26_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 27: 2 animations, 30 frames, graphics/sprites/bank27_stationary_space_enemy/. */

extern const u16 gSpriteBank27Anim00Seq[15];
extern const u16 gSpriteBank27Anim01Seq[15];

extern const struct sprite_anim gSpriteBank27Anims[2] = {
    /* 0 */ {
        /* seq */ gSpriteBank27Anim00Seq,
        /* box */ { { -16, -11, 32, 23 }, { -29, -24, 59, 37 } },
        /* paletteId */ 37,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank27Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank27Anim01Seq,
        /* box */ { { -16, -11, 32, 23 }, { -29, -24, 59, 37 } },
        /* paletteId */ 37,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank27Anim01Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank27Anim00Seq[15] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
};
const u16 gSpriteBank27Anim01Seq[15] = {
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
};

extern const struct sprite_frame *const gSpriteBank27Frames[30] = {
    &gSpriteBank27Frame000.frame,
    &gSpriteBank27Frame001.frame,
    &gSpriteBank27Frame002.frame,
    &gSpriteBank27Frame003.frame,
    &gSpriteBank27Frame004.frame,
    &gSpriteBank27Frame005.frame,
    &gSpriteBank27Frame006.frame,
    &gSpriteBank27Frame007.frame,
    &gSpriteBank27Frame008.frame,
    &gSpriteBank27Frame009.frame,
    &gSpriteBank27Frame010.frame,
    &gSpriteBank27Frame011.frame,
    &gSpriteBank27Frame012.frame,
    &gSpriteBank27Frame013.frame,
    &gSpriteBank27Frame014.frame,
    &gSpriteBank27Frame015.frame,
    &gSpriteBank27Frame016.frame,
    &gSpriteBank27Frame017.frame,
    &gSpriteBank27Frame018.frame,
    &gSpriteBank27Frame019.frame,
    &gSpriteBank27Frame020.frame,
    &gSpriteBank27Frame021.frame,
    &gSpriteBank27Frame022.frame,
    &gSpriteBank27Frame023.frame,
    &gSpriteBank27Frame024.frame,
    &gSpriteBank27Frame025.frame,
    &gSpriteBank27Frame026.frame,
    &gSpriteBank27Frame027.frame,
    &gSpriteBank27Frame028.frame,
    &gSpriteBank27Frame029.frame,
};

const struct sprite_frame_1box gSpriteBank27Frame000 = {
    SPRITE_FRAME(gSpriteBank27Frame000, SPRITE_TILES_BANK27_FRAME000),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank27Frame001 = {
    SPRITE_FRAME(gSpriteBank27Frame001, SPRITE_TILES_BANK27_FRAME001),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank27Frame002 = {
    SPRITE_FRAME(gSpriteBank27Frame002, SPRITE_TILES_BANK27_FRAME002),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank27Frame003 = {
    SPRITE_FRAME(gSpriteBank27Frame003, SPRITE_TILES_BANK27_FRAME003),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank27Frame004 = {
    SPRITE_FRAME(gSpriteBank27Frame004, SPRITE_TILES_BANK27_FRAME004),
    { { -12, -7, 24, 16 } },
};
const struct sprite_frame_1box gSpriteBank27Frame005 = {
    SPRITE_FRAME(gSpriteBank27Frame005, SPRITE_TILES_BANK27_FRAME005),
    { { -12, -2, 24, 11 } },
};
const struct sprite_frame_1box gSpriteBank27Frame006 = {
    SPRITE_FRAME(gSpriteBank27Frame006, SPRITE_TILES_BANK27_FRAME006),
    { { -12, -2, 24, 11 } },
};
const struct sprite_frame_1box gSpriteBank27Frame007 = {
    SPRITE_FRAME(gSpriteBank27Frame007, SPRITE_TILES_BANK27_FRAME007),
    { { -13, -1, 26, 10 } },
};
const struct sprite_frame_1box gSpriteBank27Frame008 = {
    SPRITE_FRAME(gSpriteBank27Frame008, SPRITE_TILES_BANK27_FRAME008),
    { { -14, 1, 28, 9 } },
};
const struct sprite_frame_1box gSpriteBank27Frame009 = {
    SPRITE_FRAME(gSpriteBank27Frame009, SPRITE_TILES_BANK27_FRAME009),
    { { -14, 1, 28, 9 } },
};
const struct sprite_frame_1box gSpriteBank27Frame010 = {
    SPRITE_FRAME(gSpriteBank27Frame010, SPRITE_TILES_BANK27_FRAME010),
    { { -13, -2, 27, 11 } },
};
const struct sprite_frame_1box gSpriteBank27Frame011 = {
    SPRITE_FRAME(gSpriteBank27Frame011, SPRITE_TILES_BANK27_FRAME011),
    { { -18, -17, 41, 24 } },
};
const struct sprite_frame_1box gSpriteBank27Frame012 = {
    SPRITE_FRAME(gSpriteBank27Frame012, SPRITE_TILES_BANK27_FRAME012),
    { { -21, -10, 47, 13 } },
};
const struct sprite_frame_1box gSpriteBank27Frame013 = {
    SPRITE_FRAME(gSpriteBank27Frame013, SPRITE_TILES_BANK27_FRAME013),
    { { -22, -13, 47, 16 } },
};
const struct sprite_frame_1box gSpriteBank27Frame014 = {
    SPRITE_FRAME(gSpriteBank27Frame014, SPRITE_TILES_BANK27_FRAME014),
    { { -21, -14, 46, 16 } },
};
const struct sprite_frame_1box gSpriteBank27Frame015 = {
    SPRITE_FRAME(gSpriteBank27Frame015, SPRITE_TILES_BANK27_FRAME015),
    { { -20, -11, 45, 12 } },
};
const struct sprite_frame_1box gSpriteBank27Frame016 = {
    SPRITE_FRAME(gSpriteBank27Frame016, SPRITE_TILES_BANK27_FRAME016),
    { { -22, -12, 47, 13 } },
};
const struct sprite_frame_1box gSpriteBank27Frame017 = {
    SPRITE_FRAME(gSpriteBank27Frame017, SPRITE_TILES_BANK27_FRAME017),
    { { -21, -11, 47, 13 } },
};
const struct sprite_frame_1box gSpriteBank27Frame018 = {
    SPRITE_FRAME(gSpriteBank27Frame018, SPRITE_TILES_BANK27_FRAME018),
    { { -23, -15, 49, 14 } },
};
const struct sprite_frame_1box gSpriteBank27Frame019 = {
    SPRITE_FRAME(gSpriteBank27Frame019, SPRITE_TILES_BANK27_FRAME019),
    { { -14, -2, 29, 11 } },
};
const struct sprite_frame_1box gSpriteBank27Frame020 = {
    SPRITE_FRAME(gSpriteBank27Frame020, SPRITE_TILES_BANK27_FRAME020),
    { { -15, 1, 30, 9 } },
};
const struct sprite_frame_1box gSpriteBank27Frame021 = {
    SPRITE_FRAME(gSpriteBank27Frame021, SPRITE_TILES_BANK27_FRAME021),
    { { -15, 1, 30, 9 } },
};
const struct sprite_frame_1box gSpriteBank27Frame022 = {
    SPRITE_FRAME(gSpriteBank27Frame022, SPRITE_TILES_BANK27_FRAME022),
    { { -14, -1, 28, 10 } },
};
const struct sprite_frame_1box gSpriteBank27Frame023 = {
    SPRITE_FRAME(gSpriteBank27Frame023, SPRITE_TILES_BANK27_FRAME023),
    { { -13, -2, 26, 11 } },
};
const struct sprite_frame_1box gSpriteBank27Frame024 = {
    SPRITE_FRAME(gSpriteBank27Frame024, SPRITE_TILES_BANK27_FRAME024),
    { { -13, -2, 26, 11 } },
};
const struct sprite_frame_1box gSpriteBank27Frame025 = {
    SPRITE_FRAME(gSpriteBank27Frame025, SPRITE_TILES_BANK27_FRAME025),
    { { -13, -7, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank27Frame026 = {
    SPRITE_FRAME(gSpriteBank27Frame026, SPRITE_TILES_BANK27_FRAME026),
    { { -13, -8, 26, 17 } },
};
const struct sprite_frame_1box gSpriteBank27Frame027 = {
    SPRITE_FRAME(gSpriteBank27Frame027, SPRITE_TILES_BANK27_FRAME027),
    { { -13, -8, 26, 17 } },
};
const struct sprite_frame_1box gSpriteBank27Frame028 = {
    SPRITE_FRAME(gSpriteBank27Frame028, SPRITE_TILES_BANK27_FRAME028),
    { { -13, -8, 26, 17 } },
};
const struct sprite_frame_1box gSpriteBank27Frame029 = {
    SPRITE_FRAME(gSpriteBank27Frame029, SPRITE_TILES_BANK27_FRAME029),
    { { -13, -8, 26, 17 } },
};

/* The piece arrays, from graphics/sprites/bank27_stationary_space_enemy/bank.json. */
#include "sprites/bank27_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 28: 1 animation, 3 frames, graphics/sprites/bank28_launch_pad/. */

extern const u16 gSpriteBank28Anim00Seq[8];

extern const struct sprite_anim gSpriteBank28Anims[1] = {
    /* 0 */ {
        /* seq */ gSpriteBank28Anim00Seq,
        /* box */ { { -14, -1, 29, 9 }, { -13, -7, 26, 15 } },
        /* paletteId */ 41,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank28Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank28Anim00Seq[8] = {
    0, 1, 2, 0, 1, 2, 0, 1,
};

extern const struct sprite_frame *const gSpriteBank28Frames[3] = {
    &gSpriteBank28Frame000,
    &gSpriteBank28Frame001,
    &gSpriteBank28Frame002,
};

const struct sprite_frame gSpriteBank28Frame000 = SPRITE_FRAME(gSpriteBank28Frame000, SPRITE_TILES_BANK28_FRAME000);
const struct sprite_frame gSpriteBank28Frame001 = SPRITE_FRAME(gSpriteBank28Frame001, SPRITE_TILES_BANK28_FRAME001);
const struct sprite_frame gSpriteBank28Frame002 = SPRITE_FRAME(gSpriteBank28Frame002, SPRITE_TILES_BANK28_FRAME002);

/* The piece arrays, from graphics/sprites/bank28_launch_pad/bank.json. */
#include "sprites/bank28_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 29: 4 animations, 42 frames, graphics/sprites/bank29_saucer_lab_assistant/. */

extern const u16 gSpriteBank29Anim00Seq[20];
extern const u16 gSpriteBank29Anim01Seq[6];
extern const u16 gSpriteBank29Anim02Seq[10];
extern const u16 gSpriteBank29Anim03Seq[6];

extern const struct sprite_anim gSpriteBank29Anims[4] = {
    /* 0 */ {
        /* seq */ gSpriteBank29Anim00Seq,
        /* box */ { { -28, -22, 57, 44 }, { -30, -23, 61, 43 } },
        /* paletteId */ 106,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank29Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank29Anim01Seq,
        /* box */ { { -28, -22, 57, 44 }, { -29, -22, 58, 49 } },
        /* paletteId */ 106,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank29Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank29Anim02Seq,
        /* box */ { { -28, -22, 57, 44 }, { -30, -23, 59, 45 } },
        /* paletteId */ 106,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank29Anim02Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 3 */ {
        /* seq */ gSpriteBank29Anim03Seq,
        /* box */ { { -28, -22, 57, 44 }, { -29, -22, 58, 49 } },
        /* paletteId */ 106,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank29Anim03Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank29Anim00Seq[20] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19,
};
const u16 gSpriteBank29Anim01Seq[6] = {
    20, 21, 22, 23, 24, 25,
};
const u16 gSpriteBank29Anim02Seq[10] = {
    26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
};
const u16 gSpriteBank29Anim03Seq[6] = {
    36, 37, 38, 39, 40, 41,
};

extern const struct sprite_frame *const gSpriteBank29Frames[42] = {
    &gSpriteBank29Frame000.frame,
    &gSpriteBank29Frame001.frame,
    &gSpriteBank29Frame002.frame,
    &gSpriteBank29Frame003.frame,
    &gSpriteBank29Frame004.frame,
    &gSpriteBank29Frame005.frame,
    &gSpriteBank29Frame006.frame,
    &gSpriteBank29Frame007.frame,
    &gSpriteBank29Frame008.frame,
    &gSpriteBank29Frame009.frame,
    &gSpriteBank29Frame010.frame,
    &gSpriteBank29Frame011.frame,
    &gSpriteBank29Frame012.frame,
    &gSpriteBank29Frame013.frame,
    &gSpriteBank29Frame014.frame,
    &gSpriteBank29Frame015.frame,
    &gSpriteBank29Frame016.frame,
    &gSpriteBank29Frame017.frame,
    &gSpriteBank29Frame018.frame,
    &gSpriteBank29Frame019.frame,
    &gSpriteBank29Frame020.frame,
    &gSpriteBank29Frame021.frame,
    &gSpriteBank29Frame022.frame,
    &gSpriteBank29Frame023.frame,
    &gSpriteBank29Frame024.frame,
    &gSpriteBank29Frame025.frame,
    &gSpriteBank29Frame026.frame,
    &gSpriteBank29Frame027.frame,
    &gSpriteBank29Frame028.frame,
    &gSpriteBank29Frame029.frame,
    &gSpriteBank29Frame030.frame,
    &gSpriteBank29Frame031.frame,
    &gSpriteBank29Frame032.frame,
    &gSpriteBank29Frame033.frame,
    &gSpriteBank29Frame034.frame,
    &gSpriteBank29Frame035.frame,
    &gSpriteBank29Frame036.frame,
    &gSpriteBank29Frame037.frame,
    &gSpriteBank29Frame038.frame,
    &gSpriteBank29Frame039.frame,
    &gSpriteBank29Frame040.frame,
    &gSpriteBank29Frame041.frame,
};

const struct sprite_frame_1box gSpriteBank29Frame000 = {
    SPRITE_FRAME(gSpriteBank29Frame000, SPRITE_TILES_BANK29_FRAME000),
    { { -16, -23, 47, 40 } },
};
const struct sprite_frame_1box gSpriteBank29Frame001 = {
    SPRITE_FRAME(gSpriteBank29Frame001, SPRITE_TILES_BANK29_FRAME001),
    { { -11, -16, 36, 32 } },
};
const struct sprite_frame_1box gSpriteBank29Frame002 = {
    SPRITE_FRAME(gSpriteBank29Frame002, SPRITE_TILES_BANK29_FRAME002),
    { { -12, -21, 38, 37 } },
};
const struct sprite_frame_1box gSpriteBank29Frame003 = {
    SPRITE_FRAME(gSpriteBank29Frame003, SPRITE_TILES_BANK29_FRAME003),
    { { -10, -20, 33, 37 } },
};
const struct sprite_frame_1box gSpriteBank29Frame004 = {
    SPRITE_FRAME(gSpriteBank29Frame004, SPRITE_TILES_BANK29_FRAME004),
    { { -13, -19, 39, 33 } },
};
const struct sprite_frame_1box gSpriteBank29Frame005 = {
    SPRITE_FRAME(gSpriteBank29Frame005, SPRITE_TILES_BANK29_FRAME005),
    { { -12, -20, 32, 37 } },
};
const struct sprite_frame_1box gSpriteBank29Frame006 = {
    SPRITE_FRAME(gSpriteBank29Frame006, SPRITE_TILES_BANK29_FRAME006),
    { { -14, -20, 36, 40 } },
};
const struct sprite_frame_1box gSpriteBank29Frame007 = {
    SPRITE_FRAME(gSpriteBank29Frame007, SPRITE_TILES_BANK29_FRAME007),
    { { -12, -20, 31, 38 } },
};
const struct sprite_frame_1box gSpriteBank29Frame008 = {
    SPRITE_FRAME(gSpriteBank29Frame008, SPRITE_TILES_BANK29_FRAME008),
    { { -13, -20, 30, 37 } },
};
const struct sprite_frame_1box gSpriteBank29Frame009 = {
    SPRITE_FRAME(gSpriteBank29Frame009, SPRITE_TILES_BANK29_FRAME009),
    { { -12, -24, 29, 42 } },
};
const struct sprite_frame_1box gSpriteBank29Frame010 = {
    SPRITE_FRAME(gSpriteBank29Frame010, SPRITE_TILES_BANK29_FRAME010),
    { { -12, -20, 31, 36 } },
};
const struct sprite_frame_1box gSpriteBank29Frame011 = {
    SPRITE_FRAME(gSpriteBank29Frame011, SPRITE_TILES_BANK29_FRAME011),
    { { -13, -21, 32, 39 } },
};
const struct sprite_frame_1box gSpriteBank29Frame012 = {
    SPRITE_FRAME(gSpriteBank29Frame012, SPRITE_TILES_BANK29_FRAME012),
    { { -23, -7, 38, 21 } },
};
const struct sprite_frame_1box gSpriteBank29Frame013 = {
    SPRITE_FRAME(gSpriteBank29Frame013, SPRITE_TILES_BANK29_FRAME013),
    { { -23, -10, 46, 23 } },
};
const struct sprite_frame_1box gSpriteBank29Frame014 = {
    SPRITE_FRAME(gSpriteBank29Frame014, SPRITE_TILES_BANK29_FRAME014),
    { { -26, -16, 53, 32 } },
};
const struct sprite_frame_1box gSpriteBank29Frame015 = {
    SPRITE_FRAME(gSpriteBank29Frame015, SPRITE_TILES_BANK29_FRAME015),
    { { -24, -13, 50, 30 } },
};
const struct sprite_frame_1box gSpriteBank29Frame016 = {
    SPRITE_FRAME(gSpriteBank29Frame016, SPRITE_TILES_BANK29_FRAME016),
    { { -24, -10, 51, 24 } },
};
const struct sprite_frame_1box gSpriteBank29Frame017 = {
    SPRITE_FRAME(gSpriteBank29Frame017, SPRITE_TILES_BANK29_FRAME017),
    { { -24, -9, 51, 26 } },
};
const struct sprite_frame_1box gSpriteBank29Frame018 = {
    SPRITE_FRAME(gSpriteBank29Frame018, SPRITE_TILES_BANK29_FRAME018),
    { { -23, -10, 46, 28 } },
};
const struct sprite_frame_1box gSpriteBank29Frame019 = {
    SPRITE_FRAME(gSpriteBank29Frame019, SPRITE_TILES_BANK29_FRAME019),
    { { -25, -15, 54, 34 } },
};
const struct sprite_frame_1box gSpriteBank29Frame020 = {
    SPRITE_FRAME(gSpriteBank29Frame020, SPRITE_TILES_BANK29_FRAME020),
    { { -20, -8, 45, 22 } },
};
const struct sprite_frame_1box gSpriteBank29Frame021 = {
    SPRITE_FRAME(gSpriteBank29Frame021, SPRITE_TILES_BANK29_FRAME021),
    { { -20, -8, 45, 22 } },
};
const struct sprite_frame_1box gSpriteBank29Frame022 = {
    SPRITE_FRAME(gSpriteBank29Frame022, SPRITE_TILES_BANK29_FRAME022),
    { { -20, -8, 45, 22 } },
};
const struct sprite_frame_1box gSpriteBank29Frame023 = {
    SPRITE_FRAME(gSpriteBank29Frame023, SPRITE_TILES_BANK29_FRAME023),
    { { -20, -8, 45, 22 } },
};
const struct sprite_frame_1box gSpriteBank29Frame024 = {
    SPRITE_FRAME(gSpriteBank29Frame024, SPRITE_TILES_BANK29_FRAME024),
    { { -20, -8, 45, 22 } },
};
const struct sprite_frame_1box gSpriteBank29Frame025 = {
    SPRITE_FRAME(gSpriteBank29Frame025, SPRITE_TILES_BANK29_FRAME025),
    { { -20, -8, 45, 22 } },
};
const struct sprite_frame_1box gSpriteBank29Frame026 = {
    SPRITE_FRAME(gSpriteBank29Frame026, SPRITE_TILES_BANK29_FRAME026),
    { { -20, -8, 44, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame027 = {
    SPRITE_FRAME(gSpriteBank29Frame027, SPRITE_TILES_BANK29_FRAME027),
    { { -20, -8, 44, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame028 = {
    SPRITE_FRAME(gSpriteBank29Frame028, SPRITE_TILES_BANK29_FRAME028),
    { { -20, -8, 44, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame029 = {
    SPRITE_FRAME(gSpriteBank29Frame029, SPRITE_TILES_BANK29_FRAME029),
    { { -20, -8, 44, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame030 = {
    SPRITE_FRAME(gSpriteBank29Frame030, SPRITE_TILES_BANK29_FRAME030),
    { { -20, -8, 44, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame031 = {
    SPRITE_FRAME(gSpriteBank29Frame031, SPRITE_TILES_BANK29_FRAME031),
    { { -20, -8, 44, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame032 = {
    SPRITE_FRAME(gSpriteBank29Frame032, SPRITE_TILES_BANK29_FRAME032),
    { { -20, -8, 44, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame033 = {
    SPRITE_FRAME(gSpriteBank29Frame033, SPRITE_TILES_BANK29_FRAME033),
    { { -20, -8, 44, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame034 = {
    SPRITE_FRAME(gSpriteBank29Frame034, SPRITE_TILES_BANK29_FRAME034),
    { { -20, -8, 44, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame035 = {
    SPRITE_FRAME(gSpriteBank29Frame035, SPRITE_TILES_BANK29_FRAME035),
    { { -20, -8, 44, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame036 = {
    SPRITE_FRAME(gSpriteBank29Frame036, SPRITE_TILES_BANK29_FRAME036),
    { { -18, -6, 42, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame037 = {
    SPRITE_FRAME(gSpriteBank29Frame037, SPRITE_TILES_BANK29_FRAME037),
    { { -18, -6, 42, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame038 = {
    SPRITE_FRAME(gSpriteBank29Frame038, SPRITE_TILES_BANK29_FRAME038),
    { { -18, -6, 42, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame039 = {
    SPRITE_FRAME(gSpriteBank29Frame039, SPRITE_TILES_BANK29_FRAME039),
    { { -18, -6, 42, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame040 = {
    SPRITE_FRAME(gSpriteBank29Frame040, SPRITE_TILES_BANK29_FRAME040),
    { { -18, -6, 42, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame041 = {
    SPRITE_FRAME(gSpriteBank29Frame041, SPRITE_TILES_BANK29_FRAME041),
    { { -18, -6, 42, 19 } },
};

/* The piece arrays, from graphics/sprites/bank29_saucer_lab_assistant/bank.json. */
#include "sprites/bank29_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 30: 3 animations, 42 frames, graphics/sprites/bank30_mega_mix/. */

extern const u16 gSpriteBank30Anim00Seq[20];
extern const u16 gSpriteBank30Anim01Seq[14];
extern const u16 gSpriteBank30Anim02Seq[8];

extern const struct sprite_anim gSpriteBank30Anims[3] = {
    /* 0 */ {
        /* seq */ gSpriteBank30Anim00Seq,
        /* box */ { { -32, -32, 64, 64 }, { -45, -50, 85, 85 } },
        /* paletteId */ 44,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank30Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank30Anim01Seq,
        /* box */ { { -32, -32, 64, 64 }, { -46, -65, 95, 98 } },
        /* paletteId */ 44,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank30Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank30Anim02Seq,
        /* box */ { { -32, -32, 64, 64 }, { -40, -44, 60, 77 } },
        /* paletteId */ 44,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank30Anim02Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank30Anim00Seq[20] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19,
};
const u16 gSpriteBank30Anim01Seq[14] = {
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
};
const u16 gSpriteBank30Anim02Seq[8] = {
    34, 35, 36, 37, 38, 39, 40, 41,
};

extern const struct sprite_frame *const gSpriteBank30Frames[42] = {
    &gSpriteBank30Frame000.frame,
    &gSpriteBank30Frame001.frame,
    &gSpriteBank30Frame002.frame,
    &gSpriteBank30Frame003.frame,
    &gSpriteBank30Frame004.frame,
    &gSpriteBank30Frame005.frame,
    &gSpriteBank30Frame006.frame,
    &gSpriteBank30Frame007.frame,
    &gSpriteBank30Frame008.frame,
    &gSpriteBank30Frame009.frame,
    &gSpriteBank30Frame010.frame,
    &gSpriteBank30Frame011.frame,
    &gSpriteBank30Frame012.frame,
    &gSpriteBank30Frame013.frame,
    &gSpriteBank30Frame014.frame,
    &gSpriteBank30Frame015.frame,
    &gSpriteBank30Frame016.frame,
    &gSpriteBank30Frame017.frame,
    &gSpriteBank30Frame018.frame,
    &gSpriteBank30Frame019.frame,
    &gSpriteBank30Frame020.frame,
    &gSpriteBank30Frame021.frame,
    &gSpriteBank30Frame022.frame,
    &gSpriteBank30Frame023.frame,
    &gSpriteBank30Frame024.frame,
    &gSpriteBank30Frame025.frame,
    &gSpriteBank30Frame026.frame,
    &gSpriteBank30Frame027.frame,
    &gSpriteBank30Frame028.frame,
    &gSpriteBank30Frame029.frame,
    &gSpriteBank30Frame030.frame,
    &gSpriteBank30Frame031.frame,
    &gSpriteBank30Frame032.frame,
    &gSpriteBank30Frame033.frame,
    &gSpriteBank30Frame034.frame,
    &gSpriteBank30Frame035.frame,
    &gSpriteBank30Frame036.frame,
    &gSpriteBank30Frame037.frame,
    &gSpriteBank30Frame038.frame,
    &gSpriteBank30Frame039.frame,
    &gSpriteBank30Frame040.frame,
    &gSpriteBank30Frame041.frame,
};

const struct sprite_frame_1box gSpriteBank30Frame000 = {
    SPRITE_FRAME(gSpriteBank30Frame000, SPRITE_TILES_BANK30_FRAME000),
    { { -32, -32, 64, 64 } },
};
const struct sprite_frame_1box gSpriteBank30Frame001 = {
    SPRITE_FRAME(gSpriteBank30Frame001, SPRITE_TILES_BANK30_FRAME001),
    { { -34, -35, 68, 65 } },
};
const struct sprite_frame_1box gSpriteBank30Frame002 = {
    SPRITE_FRAME(gSpriteBank30Frame002, SPRITE_TILES_BANK30_FRAME002),
    { { -37, -40, 71, 64 } },
};
const struct sprite_frame_1box gSpriteBank30Frame003 = {
    SPRITE_FRAME(gSpriteBank30Frame003, SPRITE_TILES_BANK30_FRAME003),
    { { -40, -46, 74, 63 } },
};
const struct sprite_frame_1box gSpriteBank30Frame004 = {
    SPRITE_FRAME(gSpriteBank30Frame004, SPRITE_TILES_BANK30_FRAME004),
    { { -42, -50, 74, 61 } },
};
const struct sprite_frame_1box gSpriteBank30Frame005 = {
    SPRITE_FRAME(gSpriteBank30Frame005, SPRITE_TILES_BANK30_FRAME005),
    { { -44, -50, 74, 64 } },
};
const struct sprite_frame_1box gSpriteBank30Frame006 = {
    SPRITE_FRAME(gSpriteBank30Frame006, SPRITE_TILES_BANK30_FRAME006),
    { { -45, -46, 75, 67 } },
};
const struct sprite_frame_1box gSpriteBank30Frame007 = {
    SPRITE_FRAME(gSpriteBank30Frame007, SPRITE_TILES_BANK30_FRAME007),
    { { -45, -41, 76, 67 } },
};
const struct sprite_frame_1box gSpriteBank30Frame008 = {
    SPRITE_FRAME(gSpriteBank30Frame008, SPRITE_TILES_BANK30_FRAME008),
    { { -45, -37, 78, 66 } },
};
const struct sprite_frame_1box gSpriteBank30Frame009 = {
    SPRITE_FRAME(gSpriteBank30Frame009, SPRITE_TILES_BANK30_FRAME009),
    { { -43, -33, 77, 66 } },
};
const struct sprite_frame_1box gSpriteBank30Frame010 = {
    SPRITE_FRAME(gSpriteBank30Frame010, SPRITE_TILES_BANK30_FRAME010),
    { { -39, -32, 73, 63 } },
};
const struct sprite_frame_1box gSpriteBank30Frame011 = {
    SPRITE_FRAME(gSpriteBank30Frame011, SPRITE_TILES_BANK30_FRAME011),
    { { -40, -35, 74, 63 } },
};
const struct sprite_frame_1box gSpriteBank30Frame012 = {
    SPRITE_FRAME(gSpriteBank30Frame012, SPRITE_TILES_BANK30_FRAME012),
    { { -39, -40, 76, 63 } },
};
const struct sprite_frame_1box gSpriteBank30Frame013 = {
    SPRITE_FRAME(gSpriteBank30Frame013, SPRITE_TILES_BANK30_FRAME013),
    { { -36, -46, 75, 62 } },
};
const struct sprite_frame_1box gSpriteBank30Frame014 = {
    SPRITE_FRAME(gSpriteBank30Frame014, SPRITE_TILES_BANK30_FRAME014),
    { { -34, -50, 73, 63 } },
};
const struct sprite_frame_1box gSpriteBank30Frame015 = {
    SPRITE_FRAME(gSpriteBank30Frame015, SPRITE_TILES_BANK30_FRAME015),
    { { -36, -49, 76, 65 } },
};
const struct sprite_frame_1box gSpriteBank30Frame016 = {
    SPRITE_FRAME(gSpriteBank30Frame016, SPRITE_TILES_BANK30_FRAME016),
    { { -38, -46, 77, 68 } },
};
const struct sprite_frame_1box gSpriteBank30Frame017 = {
    SPRITE_FRAME(gSpriteBank30Frame017, SPRITE_TILES_BANK30_FRAME017),
    { { -39, -42, 73, 70 } },
};
const struct sprite_frame_1box gSpriteBank30Frame018 = {
    SPRITE_FRAME(gSpriteBank30Frame018, SPRITE_TILES_BANK30_FRAME018),
    { { -33, -38, 63, 68 } },
};
const struct sprite_frame_1box gSpriteBank30Frame019 = {
    SPRITE_FRAME(gSpriteBank30Frame019, SPRITE_TILES_BANK30_FRAME019),
    { { -28, -34, 59, 69 } },
};
const struct sprite_frame_1box gSpriteBank30Frame020 = {
    SPRITE_FRAME(gSpriteBank30Frame020, SPRITE_TILES_BANK30_FRAME020),
    { { -36, -36, 64, 62 } },
};
const struct sprite_frame_1box gSpriteBank30Frame021 = {
    SPRITE_FRAME(gSpriteBank30Frame021, SPRITE_TILES_BANK30_FRAME021),
    { { -36, -48, 63, 73 } },
};
const struct sprite_frame_1box gSpriteBank30Frame022 = {
    SPRITE_FRAME(gSpriteBank30Frame022, SPRITE_TILES_BANK30_FRAME022),
    { { -41, -59, 60, 82 } },
};
const struct sprite_frame_1box gSpriteBank30Frame023 = {
    SPRITE_FRAME(gSpriteBank30Frame023, SPRITE_TILES_BANK30_FRAME023),
    { { -43, -65, 59, 81 } },
};
const struct sprite_frame_1box gSpriteBank30Frame024 = {
    SPRITE_FRAME(gSpriteBank30Frame024, SPRITE_TILES_BANK30_FRAME024),
    { { -44, -64, 60, 79 } },
};
const struct sprite_frame_1box gSpriteBank30Frame025 = {
    SPRITE_FRAME(gSpriteBank30Frame025, SPRITE_TILES_BANK30_FRAME025),
    { { -45, -61, 63, 80 } },
};
const struct sprite_frame_1box gSpriteBank30Frame026 = {
    SPRITE_FRAME(gSpriteBank30Frame026, SPRITE_TILES_BANK30_FRAME026),
    { { -46, -58, 67, 85 } },
};
const struct sprite_frame_1box gSpriteBank30Frame027 = {
    SPRITE_FRAME(gSpriteBank30Frame027, SPRITE_TILES_BANK30_FRAME027),
    { { -46, -46, 95, 77 } },
};
const struct sprite_frame_1box gSpriteBank30Frame028 = {
    SPRITE_FRAME(gSpriteBank30Frame028, SPRITE_TILES_BANK30_FRAME028),
    { { -45, -38, 94, 71 } },
};
const struct sprite_frame_1box gSpriteBank30Frame029 = {
    SPRITE_FRAME(gSpriteBank30Frame029, SPRITE_TILES_BANK30_FRAME029),
    { { -41, -36, 80, 67 } },
};
const struct sprite_frame_1box gSpriteBank30Frame030 = {
    SPRITE_FRAME(gSpriteBank30Frame030, SPRITE_TILES_BANK30_FRAME030),
    { { -35, -38, 64, 70 } },
};
const struct sprite_frame_1box gSpriteBank30Frame031 = {
    SPRITE_FRAME(gSpriteBank30Frame031, SPRITE_TILES_BANK30_FRAME031),
    { { -35, -41, 61, 74 } },
};
const struct sprite_frame_1box gSpriteBank30Frame032 = {
    SPRITE_FRAME(gSpriteBank30Frame032, SPRITE_TILES_BANK30_FRAME032),
    { { -34, -45, 55, 78 } },
};
const struct sprite_frame_1box gSpriteBank30Frame033 = {
    SPRITE_FRAME(gSpriteBank30Frame033, SPRITE_TILES_BANK30_FRAME033),
    { { -37, -45, 57, 78 } },
};
const struct sprite_frame_1box gSpriteBank30Frame034 = {
    SPRITE_FRAME(gSpriteBank30Frame034, SPRITE_TILES_BANK30_FRAME034),
    { { -40, -44, 59, 77 } },
};
const struct sprite_frame_1box gSpriteBank30Frame035 = {
    SPRITE_FRAME(gSpriteBank30Frame035, SPRITE_TILES_BANK30_FRAME035),
    { { -39, -44, 57, 77 } },
};
const struct sprite_frame_1box gSpriteBank30Frame036 = {
    SPRITE_FRAME(gSpriteBank30Frame036, SPRITE_TILES_BANK30_FRAME036),
    { { -40, -43, 59, 76 } },
};
const struct sprite_frame_1box gSpriteBank30Frame037 = {
    SPRITE_FRAME(gSpriteBank30Frame037, SPRITE_TILES_BANK30_FRAME037),
    { { -40, -42, 60, 75 } },
};
const struct sprite_frame_1box gSpriteBank30Frame038 = {
    SPRITE_FRAME(gSpriteBank30Frame038, SPRITE_TILES_BANK30_FRAME038),
    { { -39, -42, 59, 75 } },
};
const struct sprite_frame_1box gSpriteBank30Frame039 = {
    SPRITE_FRAME(gSpriteBank30Frame039, SPRITE_TILES_BANK30_FRAME039),
    { { -40, -42, 60, 75 } },
};
const struct sprite_frame_1box gSpriteBank30Frame040 = {
    SPRITE_FRAME(gSpriteBank30Frame040, SPRITE_TILES_BANK30_FRAME040),
    { { -40, -43, 60, 76 } },
};
const struct sprite_frame_1box gSpriteBank30Frame041 = {
    SPRITE_FRAME(gSpriteBank30Frame041, SPRITE_TILES_BANK30_FRAME041),
    { { -39, -44, 58, 77 } },
};

/* The piece arrays, from graphics/sprites/bank30_mega_mix/bank.json. */
#include "sprites/bank30_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 31: 36 animations, 112 frames, graphics/sprites/bank31_crates/. */

extern const u16 gSpriteBank31Anim00Seq[1];
extern const u16 gSpriteBank31Anim01Seq[9];
extern const u16 gSpriteBank31Anim02Seq[1];
extern const u16 gSpriteBank31Anim03Seq[1];
extern const u16 gSpriteBank31Anim04Seq[8];
extern const u16 gSpriteBank31Anim05Seq[1];
extern const u16 gSpriteBank31Anim06Seq[8];
extern const u16 gSpriteBank31Anim07Seq[1];
extern const u16 gSpriteBank31Anim08Seq[1];
extern const u16 gSpriteBank31Anim09Seq[1];
extern const u16 gSpriteBank31Anim10Seq[1];
extern const u16 gSpriteBank31Anim11Seq[1];
extern const u16 gSpriteBank31Anim12Seq[1];
extern const u16 gSpriteBank31Anim13Seq[1];
extern const u16 gSpriteBank31Anim14Seq[1];
extern const u16 gSpriteBank31Anim15Seq[1];
extern const u16 gSpriteBank31Anim16Seq[1];
extern const u16 gSpriteBank31Anim17Seq[1];
extern const u16 gSpriteBank31Anim18Seq[1];
extern const u16 gSpriteBank31Anim19Seq[1];
extern const u16 gSpriteBank31Anim20Seq[1];
extern const u16 gSpriteBank31Anim21Seq[1];
extern const u16 gSpriteBank31Anim22Seq[1];
extern const u16 gSpriteBank31Anim23Seq[1];
extern const u16 gSpriteBank31Anim24Seq[8];
extern const u16 gSpriteBank31Anim25Seq[7];
extern const u16 gSpriteBank31Anim26Seq[1];
extern const u16 gSpriteBank31Anim27Seq[14];
extern const u16 gSpriteBank31Anim28Seq[1];
extern const u16 gSpriteBank31Anim29Seq[9];
extern const u16 gSpriteBank31Anim30Seq[1];
extern const u16 gSpriteBank31Anim31Seq[1];
extern const u16 gSpriteBank31Anim32Seq[1];
extern const u16 gSpriteBank31Anim33Seq[9];
extern const u16 gSpriteBank31Anim34Seq[9];
extern const u16 gSpriteBank31Anim35Seq[9];

extern const struct sprite_anim gSpriteBank31Anims[36] = {
    /* 0 */ {
        /* seq */ gSpriteBank31Anim00Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 45,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank31Anim01Seq,
        /* box */ { { -12, -6, 19, 19 }, { -30, -49, 63, 62 } },
        /* paletteId */ 46,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank31Anim02Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 47,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank31Anim03Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 47,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim03Seq),
        /* flags */ 0,
    },
    /* 4 */ {
        /* seq */ gSpriteBank31Anim04Seq,
        /* box */ { { -12, -6, 19, 19 }, { -16, -15, 33, 28 } },
        /* paletteId */ 46,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim04Seq),
        /* flags */ 0,
    },
    /* 5 */ {
        /* seq */ gSpriteBank31Anim05Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 46,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim05Seq),
        /* flags */ 0,
    },
    /* 6 */ {
        /* seq */ gSpriteBank31Anim06Seq,
        /* box */ { { -12, -6, 19, 19 }, { -16, -15, 36, 28 } },
        /* paletteId */ 48,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim06Seq),
        /* flags */ 0,
    },
    /* 7 */ {
        /* seq */ gSpriteBank31Anim07Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 45,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim07Seq),
        /* flags */ 0,
    },
    /* 8 */ {
        /* seq */ gSpriteBank31Anim08Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 45,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim08Seq),
        /* flags */ 0,
    },
    /* 9 */ {
        /* seq */ gSpriteBank31Anim09Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 45,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim09Seq),
        /* flags */ 0,
    },
    /* 10 */ {
        /* seq */ gSpriteBank31Anim10Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 45,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim10Seq),
        /* flags */ 0,
    },
    /* 11 */ {
        /* seq */ gSpriteBank31Anim11Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 45,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim11Seq),
        /* flags */ 0,
    },
    /* 12 */ {
        /* seq */ gSpriteBank31Anim12Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 45,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim12Seq),
        /* flags */ 0,
    },
    /* 13 */ {
        /* seq */ gSpriteBank31Anim13Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 45,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim13Seq),
        /* flags */ 0,
    },
    /* 14 */ {
        /* seq */ gSpriteBank31Anim14Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 46,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim14Seq),
        /* flags */ 0,
    },
    /* 15 */ {
        /* seq */ gSpriteBank31Anim15Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 46,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim15Seq),
        /* flags */ 0,
    },
    /* 16 */ {
        /* seq */ gSpriteBank31Anim16Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 46,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim16Seq),
        /* flags */ 0,
    },
    /* 17 */ {
        /* seq */ gSpriteBank31Anim17Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 46,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim17Seq),
        /* flags */ 0,
    },
    /* 18 */ {
        /* seq */ gSpriteBank31Anim18Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 46,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim18Seq),
        /* flags */ 0,
    },
    /* 19 */ {
        /* seq */ gSpriteBank31Anim19Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 46,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim19Seq),
        /* flags */ 0,
    },
    /* 20 */ {
        /* seq */ gSpriteBank31Anim20Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 46,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim20Seq),
        /* flags */ 0,
    },
    /* 21 */ {
        /* seq */ gSpriteBank31Anim21Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 48,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim21Seq),
        /* flags */ 0,
    },
    /* 22 */ {
        /* seq */ gSpriteBank31Anim22Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 45,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim22Seq),
        /* flags */ 0,
    },
    /* 23 */ {
        /* seq */ gSpriteBank31Anim23Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 45,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim23Seq),
        /* flags */ 0,
    },
    /* 24 */ {
        /* seq */ gSpriteBank31Anim24Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -14, 26, 27 } },
        /* paletteId */ 45,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim24Seq),
        /* flags */ 0,
    },
    /* 25 */ {
        /* seq */ gSpriteBank31Anim25Seq,
        /* box */ { { -12, -6, 19, 19 }, { -14, -15, 30, 28 } },
        /* paletteId */ 45,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim25Seq),
        /* flags */ 0,
    },
    /* 26 */ {
        /* seq */ gSpriteBank31Anim26Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 45,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim26Seq),
        /* flags */ 0,
    },
    /* 27 */ {
        /* seq */ gSpriteBank31Anim27Seq,
        /* box */ { { -12, -13, 25, 26 }, { -32, -13, 66, 32 } },
        /* paletteId */ 45,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim27Seq),
        /* flags */ 0,
    },
    /* 28 */ {
        /* seq */ gSpriteBank31Anim28Seq,
        /* box */ { { -12, -6, 19, 19 }, { -14, -13, 27, 26 } },
        /* paletteId */ 45,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim28Seq),
        /* flags */ 0,
    },
    /* 29 */ {
        /* seq */ gSpriteBank31Anim29Seq,
        /* box */ { { -12, -6, 19, 19 }, { -30, -49, 63, 62 } },
        /* paletteId */ 45,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim29Seq),
        /* flags */ 0,
    },
    /* 30 */ {
        /* seq */ gSpriteBank31Anim30Seq,
        /* box */ { { -12, -6, 19, 19 }, { -14, -13, 27, 26 } },
        /* paletteId */ 45,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim30Seq),
        /* flags */ 0,
    },
    /* 31 */ {
        /* seq */ gSpriteBank31Anim31Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 45,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim31Seq),
        /* flags */ 0,
    },
    /* 32 */ {
        /* seq */ gSpriteBank31Anim32Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 47,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim32Seq),
        /* flags */ 0,
    },
    /* 33 */ {
        /* seq */ gSpriteBank31Anim33Seq,
        /* box */ { { -12, -6, 19, 19 }, { -30, -49, 63, 62 } },
        /* paletteId */ 46,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim33Seq),
        /* flags */ 0,
    },
    /* 34 */ {
        /* seq */ gSpriteBank31Anim34Seq,
        /* box */ { { -12, -6, 19, 19 }, { -16, -40, 33, 53 } },
        /* paletteId */ 47,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim34Seq),
        /* flags */ 0,
    },
    /* 35 */ {
        /* seq */ gSpriteBank31Anim35Seq,
        /* box */ { { -12, -6, 19, 19 }, { -16, -40, 33, 53 } },
        /* paletteId */ 46,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim35Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank31Anim00Seq[1] = {
    0,
};
const u16 gSpriteBank31Anim01Seq[9] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9,
};
const u16 gSpriteBank31Anim02Seq[1] = {
    10,
};
const u16 gSpriteBank31Anim03Seq[1] = {
    11,
};
const u16 gSpriteBank31Anim04Seq[8] = {
    12, 13, 14, 15, 16, 17, 18, 19,
};
const u16 gSpriteBank31Anim05Seq[1] = {
    20,
};
const u16 gSpriteBank31Anim06Seq[8] = {
    21, 22, 23, 24, 25, 26, 27, 28,
};
const u16 gSpriteBank31Anim07Seq[1] = {
    29,
};
const u16 gSpriteBank31Anim08Seq[1] = {
    30,
};
const u16 gSpriteBank31Anim09Seq[1] = {
    31,
};
const u16 gSpriteBank31Anim10Seq[1] = {
    32,
};
const u16 gSpriteBank31Anim11Seq[1] = {
    33,
};
const u16 gSpriteBank31Anim12Seq[1] = {
    34,
};
const u16 gSpriteBank31Anim13Seq[1] = {
    35,
};
const u16 gSpriteBank31Anim14Seq[1] = {
    36,
};
const u16 gSpriteBank31Anim15Seq[1] = {
    37,
};
const u16 gSpriteBank31Anim16Seq[1] = {
    38,
};
const u16 gSpriteBank31Anim17Seq[1] = {
    39,
};
const u16 gSpriteBank31Anim18Seq[1] = {
    40,
};
const u16 gSpriteBank31Anim19Seq[1] = {
    41,
};
const u16 gSpriteBank31Anim20Seq[1] = {
    42,
};
const u16 gSpriteBank31Anim21Seq[1] = {
    43,
};
const u16 gSpriteBank31Anim22Seq[1] = {
    44,
};
const u16 gSpriteBank31Anim23Seq[1] = {
    45,
};
const u16 gSpriteBank31Anim24Seq[8] = {
    46, 47, 48, 49, 50, 51, 52, 53,
};
const u16 gSpriteBank31Anim25Seq[7] = {
    54, 55, 56, 55, 54, 57, 58,
};
const u16 gSpriteBank31Anim26Seq[1] = {
    59,
};
const u16 gSpriteBank31Anim27Seq[14] = {
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73,
};
const u16 gSpriteBank31Anim28Seq[1] = {
    74,
};
const u16 gSpriteBank31Anim29Seq[9] = {
    75, 76, 77, 78, 79, 80, 81, 82, 83,
};
const u16 gSpriteBank31Anim30Seq[1] = {
    74,
};
const u16 gSpriteBank31Anim31Seq[1] = {
    44,
};
const u16 gSpriteBank31Anim32Seq[1] = {
    84,
};
const u16 gSpriteBank31Anim33Seq[9] = {
    85, 86, 87, 88, 89, 90, 91, 92, 93,
};
const u16 gSpriteBank31Anim34Seq[9] = {
    94, 95, 96, 97, 98, 99, 100, 101, 102,
};
const u16 gSpriteBank31Anim35Seq[9] = {
    103, 104, 105, 106, 107, 108, 109, 110, 111,
};

extern const struct sprite_frame *const gSpriteBank31Frames[112] = {
    &gSpriteBank31Frame000.frame,
    &gSpriteBank31Frame001,
    &gSpriteBank31Frame002,
    &gSpriteBank31Frame003,
    &gSpriteBank31Frame004,
    &gSpriteBank31Frame005,
    &gSpriteBank31Frame006,
    &gSpriteBank31Frame007,
    &gSpriteBank31Frame008,
    &gSpriteBank31Frame009,
    &gSpriteBank31Frame010.frame,
    &gSpriteBank31Frame011.frame,
    &gSpriteBank31Frame012,
    &gSpriteBank31Frame013,
    &gSpriteBank31Frame014,
    &gSpriteBank31Frame015,
    &gSpriteBank31Frame016,
    &gSpriteBank31Frame017,
    &gSpriteBank31Frame018,
    &gSpriteBank31Frame019,
    &gSpriteBank31Frame020.frame,
    &gSpriteBank31Frame021,
    &gSpriteBank31Frame022,
    &gSpriteBank31Frame023,
    &gSpriteBank31Frame024,
    &gSpriteBank31Frame025,
    &gSpriteBank31Frame026,
    &gSpriteBank31Frame027,
    &gSpriteBank31Frame028,
    &gSpriteBank31Frame029,
    &gSpriteBank31Frame030,
    &gSpriteBank31Frame031,
    &gSpriteBank31Frame032,
    &gSpriteBank31Frame033,
    &gSpriteBank31Frame034,
    &gSpriteBank31Frame035.frame,
    &gSpriteBank31Frame036,
    &gSpriteBank31Frame037,
    &gSpriteBank31Frame038,
    &gSpriteBank31Frame039,
    &gSpriteBank31Frame040,
    &gSpriteBank31Frame041,
    &gSpriteBank31Frame042,
    &gSpriteBank31Frame043,
    &gSpriteBank31Frame044.frame,
    &gSpriteBank31Frame045,
    &gSpriteBank31Frame046,
    &gSpriteBank31Frame047,
    &gSpriteBank31Frame048,
    &gSpriteBank31Frame049,
    &gSpriteBank31Frame050,
    &gSpriteBank31Frame051,
    &gSpriteBank31Frame052,
    &gSpriteBank31Frame053,
    &gSpriteBank31Frame054.frame,
    &gSpriteBank31Frame055.frame,
    &gSpriteBank31Frame056.frame,
    &gSpriteBank31Frame057.frame,
    &gSpriteBank31Frame058.frame,
    &gSpriteBank31Frame059,
    &gSpriteBank31Frame060,
    &gSpriteBank31Frame061,
    &gSpriteBank31Frame062,
    &gSpriteBank31Frame063,
    &gSpriteBank31Frame064,
    &gSpriteBank31Frame065,
    &gSpriteBank31Frame066,
    &gSpriteBank31Frame067,
    &gSpriteBank31Frame068,
    &gSpriteBank31Frame069,
    &gSpriteBank31Frame070,
    &gSpriteBank31Frame071,
    &gSpriteBank31Frame072,
    &gSpriteBank31Frame073,
    &gSpriteBank31Frame074,
    &gSpriteBank31Frame075,
    &gSpriteBank31Frame076,
    &gSpriteBank31Frame077,
    &gSpriteBank31Frame078,
    &gSpriteBank31Frame079,
    &gSpriteBank31Frame080,
    &gSpriteBank31Frame081,
    &gSpriteBank31Frame082,
    &gSpriteBank31Frame083,
    &gSpriteBank31Frame084,
    &gSpriteBank31Frame085,
    &gSpriteBank31Frame086,
    &gSpriteBank31Frame087,
    &gSpriteBank31Frame088,
    &gSpriteBank31Frame089,
    &gSpriteBank31Frame090,
    &gSpriteBank31Frame091,
    &gSpriteBank31Frame092,
    &gSpriteBank31Frame093,
    &gSpriteBank31Frame094.frame,
    &gSpriteBank31Frame095.frame,
    &gSpriteBank31Frame096.frame,
    &gSpriteBank31Frame097.frame,
    &gSpriteBank31Frame098.frame,
    &gSpriteBank31Frame099.frame,
    &gSpriteBank31Frame100.frame,
    &gSpriteBank31Frame101.frame,
    &gSpriteBank31Frame102.frame,
    &gSpriteBank31Frame103.frame,
    &gSpriteBank31Frame104.frame,
    &gSpriteBank31Frame105.frame,
    &gSpriteBank31Frame106.frame,
    &gSpriteBank31Frame107.frame,
    &gSpriteBank31Frame108.frame,
    &gSpriteBank31Frame109.frame,
    &gSpriteBank31Frame110.frame,
    &gSpriteBank31Frame111.frame,
};

const struct sprite_frame_1box gSpriteBank31Frame000 = {
    SPRITE_FRAME(gSpriteBank31Frame000, SPRITE_TILES_BANK31_FRAME000),
    { { -12, -13, 25, 26 } },
};
const struct sprite_frame gSpriteBank31Frame001 = SPRITE_FRAME(gSpriteBank31Frame001, SPRITE_TILES_BANK31_FRAME001);
const struct sprite_frame gSpriteBank31Frame002 = SPRITE_FRAME(gSpriteBank31Frame002, SPRITE_TILES_BANK31_FRAME002);
const struct sprite_frame gSpriteBank31Frame003 = SPRITE_FRAME(gSpriteBank31Frame003, SPRITE_TILES_BANK31_FRAME003);
const struct sprite_frame gSpriteBank31Frame004 = SPRITE_FRAME(gSpriteBank31Frame004, SPRITE_TILES_BANK31_FRAME004);
const struct sprite_frame gSpriteBank31Frame005 = SPRITE_FRAME(gSpriteBank31Frame005, SPRITE_TILES_BANK31_FRAME005);
const struct sprite_frame gSpriteBank31Frame006 = SPRITE_FRAME(gSpriteBank31Frame006, SPRITE_TILES_BANK31_FRAME006);
const struct sprite_frame gSpriteBank31Frame007 = SPRITE_FRAME(gSpriteBank31Frame007, SPRITE_TILES_BANK31_FRAME007);
const struct sprite_frame gSpriteBank31Frame008 = SPRITE_FRAME(gSpriteBank31Frame008, SPRITE_TILES_BANK31_FRAME008);
const struct sprite_frame gSpriteBank31Frame009 = SPRITE_FRAME(gSpriteBank31Frame009, SPRITE_TILES_BANK31_FRAME009);
const struct sprite_frame_1box gSpriteBank31Frame010 = {
    SPRITE_FRAME(gSpriteBank31Frame010, SPRITE_TILES_BANK31_FRAME010),
    { { -12, -13, 25, 26 } },
};
const struct sprite_frame_1box gSpriteBank31Frame011 = {
    SPRITE_FRAME(gSpriteBank31Frame011, SPRITE_TILES_BANK31_FRAME011),
    { { -12, -13, 25, 26 } },
};
const struct sprite_frame gSpriteBank31Frame012 = SPRITE_FRAME(gSpriteBank31Frame012, SPRITE_TILES_BANK31_FRAME012);
const struct sprite_frame gSpriteBank31Frame013 = SPRITE_FRAME(gSpriteBank31Frame013, SPRITE_TILES_BANK31_FRAME013);
const struct sprite_frame gSpriteBank31Frame014 = SPRITE_FRAME(gSpriteBank31Frame014, SPRITE_TILES_BANK31_FRAME014);
const struct sprite_frame gSpriteBank31Frame015 = SPRITE_FRAME(gSpriteBank31Frame015, SPRITE_TILES_BANK31_FRAME015);
const struct sprite_frame gSpriteBank31Frame016 = SPRITE_FRAME(gSpriteBank31Frame016, SPRITE_TILES_BANK31_FRAME016);
const struct sprite_frame gSpriteBank31Frame017 = SPRITE_FRAME(gSpriteBank31Frame017, SPRITE_TILES_BANK31_FRAME017);
const struct sprite_frame gSpriteBank31Frame018 = SPRITE_FRAME(gSpriteBank31Frame018, SPRITE_TILES_BANK31_FRAME018);
const struct sprite_frame gSpriteBank31Frame019 = SPRITE_FRAME(gSpriteBank31Frame019, SPRITE_TILES_BANK31_FRAME019);
const struct sprite_frame_1box gSpriteBank31Frame020 = {
    SPRITE_FRAME(gSpriteBank31Frame020, SPRITE_TILES_BANK31_FRAME020),
    { { -12, -13, 25, 26 } },
};
const struct sprite_frame gSpriteBank31Frame021 = SPRITE_FRAME(gSpriteBank31Frame021, SPRITE_TILES_BANK31_FRAME021);
const struct sprite_frame gSpriteBank31Frame022 = SPRITE_FRAME(gSpriteBank31Frame022, SPRITE_TILES_BANK31_FRAME022);
const struct sprite_frame gSpriteBank31Frame023 = SPRITE_FRAME(gSpriteBank31Frame023, SPRITE_TILES_BANK31_FRAME023);
const struct sprite_frame gSpriteBank31Frame024 = SPRITE_FRAME(gSpriteBank31Frame024, SPRITE_TILES_BANK31_FRAME024);
const struct sprite_frame gSpriteBank31Frame025 = SPRITE_FRAME(gSpriteBank31Frame025, SPRITE_TILES_BANK31_FRAME025);
const struct sprite_frame gSpriteBank31Frame026 = SPRITE_FRAME(gSpriteBank31Frame026, SPRITE_TILES_BANK31_FRAME026);
const struct sprite_frame gSpriteBank31Frame027 = SPRITE_FRAME(gSpriteBank31Frame027, SPRITE_TILES_BANK31_FRAME027);
const struct sprite_frame gSpriteBank31Frame028 = SPRITE_FRAME(gSpriteBank31Frame028, SPRITE_TILES_BANK31_FRAME028);
const struct sprite_frame gSpriteBank31Frame029 = SPRITE_FRAME(gSpriteBank31Frame029, SPRITE_TILES_BANK31_FRAME029);
const struct sprite_frame gSpriteBank31Frame030 = SPRITE_FRAME(gSpriteBank31Frame030, SPRITE_TILES_BANK31_FRAME030);
const struct sprite_frame gSpriteBank31Frame031 = SPRITE_FRAME(gSpriteBank31Frame031, SPRITE_TILES_BANK31_FRAME031);
const struct sprite_frame gSpriteBank31Frame032 = SPRITE_FRAME(gSpriteBank31Frame032, SPRITE_TILES_BANK31_FRAME032);
const struct sprite_frame gSpriteBank31Frame033 = SPRITE_FRAME(gSpriteBank31Frame033, SPRITE_TILES_BANK31_FRAME033);
const struct sprite_frame gSpriteBank31Frame034 = SPRITE_FRAME(gSpriteBank31Frame034, SPRITE_TILES_BANK31_FRAME034);
const struct sprite_frame_1box gSpriteBank31Frame035 = {
    SPRITE_FRAME(gSpriteBank31Frame035, SPRITE_TILES_BANK31_FRAME035),
    { { -12, -13, 25, 26 } },
};
const struct sprite_frame gSpriteBank31Frame036 = SPRITE_FRAME(gSpriteBank31Frame036, SPRITE_TILES_BANK31_FRAME036);
const struct sprite_frame gSpriteBank31Frame037 = SPRITE_FRAME(gSpriteBank31Frame037, SPRITE_TILES_BANK31_FRAME037);
const struct sprite_frame gSpriteBank31Frame038 = SPRITE_FRAME(gSpriteBank31Frame038, SPRITE_TILES_BANK31_FRAME038);
const struct sprite_frame gSpriteBank31Frame039 = SPRITE_FRAME(gSpriteBank31Frame039, SPRITE_TILES_BANK31_FRAME039);
const struct sprite_frame gSpriteBank31Frame040 = SPRITE_FRAME(gSpriteBank31Frame040, SPRITE_TILES_BANK31_FRAME040);
const struct sprite_frame gSpriteBank31Frame041 = SPRITE_FRAME(gSpriteBank31Frame041, SPRITE_TILES_BANK31_FRAME041);
const struct sprite_frame gSpriteBank31Frame042 = SPRITE_FRAME(gSpriteBank31Frame042, SPRITE_TILES_BANK31_FRAME042);
const struct sprite_frame gSpriteBank31Frame043 = SPRITE_FRAME(gSpriteBank31Frame043, SPRITE_TILES_BANK31_FRAME043);
const struct sprite_frame_1box gSpriteBank31Frame044 = {
    SPRITE_FRAME(gSpriteBank31Frame044, SPRITE_TILES_BANK31_FRAME044),
    { { -12, -13, 25, 26 } },
};
const struct sprite_frame gSpriteBank31Frame045 = SPRITE_FRAME(gSpriteBank31Frame045, SPRITE_TILES_BANK31_FRAME045);
const struct sprite_frame gSpriteBank31Frame046 = SPRITE_FRAME(gSpriteBank31Frame046, SPRITE_TILES_BANK31_FRAME046);
const struct sprite_frame gSpriteBank31Frame047 = SPRITE_FRAME(gSpriteBank31Frame047, SPRITE_TILES_BANK31_FRAME047);
const struct sprite_frame gSpriteBank31Frame048 = SPRITE_FRAME(gSpriteBank31Frame048, SPRITE_TILES_BANK31_FRAME048);
const struct sprite_frame gSpriteBank31Frame049 = SPRITE_FRAME(gSpriteBank31Frame049, SPRITE_TILES_BANK31_FRAME049);
const struct sprite_frame gSpriteBank31Frame050 = SPRITE_FRAME(gSpriteBank31Frame050, SPRITE_TILES_BANK31_FRAME050);
const struct sprite_frame gSpriteBank31Frame051 = SPRITE_FRAME(gSpriteBank31Frame051, SPRITE_TILES_BANK31_FRAME051);
const struct sprite_frame gSpriteBank31Frame052 = SPRITE_FRAME(gSpriteBank31Frame052, SPRITE_TILES_BANK31_FRAME052);
const struct sprite_frame gSpriteBank31Frame053 = SPRITE_FRAME(gSpriteBank31Frame053, SPRITE_TILES_BANK31_FRAME053);
const struct sprite_frame_1box gSpriteBank31Frame054 = {
    SPRITE_FRAME(gSpriteBank31Frame054, SPRITE_TILES_BANK31_FRAME054),
    { { -12, -13, 25, 26 } },
};
const struct sprite_frame_1box gSpriteBank31Frame055 = {
    SPRITE_FRAME(gSpriteBank31Frame055, SPRITE_TILES_BANK31_FRAME055),
    { { -14, -9, 30, 22 } },
};
const struct sprite_frame_1box gSpriteBank31Frame056 = {
    SPRITE_FRAME(gSpriteBank31Frame056, SPRITE_TILES_BANK31_FRAME056),
    { { -14, -8, 30, 21 } },
};
const struct sprite_frame_1box gSpriteBank31Frame057 = {
    SPRITE_FRAME(gSpriteBank31Frame057, SPRITE_TILES_BANK31_FRAME057),
    { { -12, -15, 24, 28 } },
};
const struct sprite_frame_1box gSpriteBank31Frame058 = {
    SPRITE_FRAME(gSpriteBank31Frame058, SPRITE_TILES_BANK31_FRAME058),
    { { -12, -13, 26, 26 } },
};
const struct sprite_frame gSpriteBank31Frame059 = SPRITE_FRAME(gSpriteBank31Frame059, SPRITE_TILES_BANK31_FRAME059);
const struct sprite_frame gSpriteBank31Frame060 = SPRITE_FRAME(gSpriteBank31Frame060, SPRITE_TILES_BANK31_FRAME060);
const struct sprite_frame gSpriteBank31Frame061 = SPRITE_FRAME(gSpriteBank31Frame061, SPRITE_TILES_BANK31_FRAME061);
const struct sprite_frame gSpriteBank31Frame062 = SPRITE_FRAME(gSpriteBank31Frame062, SPRITE_TILES_BANK31_FRAME062);
const struct sprite_frame gSpriteBank31Frame063 = SPRITE_FRAME(gSpriteBank31Frame063, SPRITE_TILES_BANK31_FRAME063);
const struct sprite_frame gSpriteBank31Frame064 = SPRITE_FRAME(gSpriteBank31Frame064, SPRITE_TILES_BANK31_FRAME064);
const struct sprite_frame gSpriteBank31Frame065 = SPRITE_FRAME(gSpriteBank31Frame065, SPRITE_TILES_BANK31_FRAME065);
const struct sprite_frame gSpriteBank31Frame066 = SPRITE_FRAME(gSpriteBank31Frame066, SPRITE_TILES_BANK31_FRAME066);
const struct sprite_frame gSpriteBank31Frame067 = SPRITE_FRAME(gSpriteBank31Frame067, SPRITE_TILES_BANK31_FRAME067);
const struct sprite_frame gSpriteBank31Frame068 = SPRITE_FRAME(gSpriteBank31Frame068, SPRITE_TILES_BANK31_FRAME068);
const struct sprite_frame gSpriteBank31Frame069 = SPRITE_FRAME(gSpriteBank31Frame069, SPRITE_TILES_BANK31_FRAME069);
const struct sprite_frame gSpriteBank31Frame070 = SPRITE_FRAME(gSpriteBank31Frame070, SPRITE_TILES_BANK31_FRAME070);
const struct sprite_frame gSpriteBank31Frame071 = SPRITE_FRAME(gSpriteBank31Frame071, SPRITE_TILES_BANK31_FRAME071);
const struct sprite_frame gSpriteBank31Frame072 = SPRITE_FRAME(gSpriteBank31Frame072, SPRITE_TILES_BANK31_FRAME072);
const struct sprite_frame gSpriteBank31Frame073 = SPRITE_FRAME(gSpriteBank31Frame073, SPRITE_TILES_BANK31_FRAME073);
const struct sprite_frame gSpriteBank31Frame074 = SPRITE_FRAME(gSpriteBank31Frame074, SPRITE_TILES_BANK31_FRAME074);
const struct sprite_frame gSpriteBank31Frame075 = SPRITE_FRAME(gSpriteBank31Frame075, SPRITE_TILES_BANK31_FRAME075);
const struct sprite_frame gSpriteBank31Frame076 = SPRITE_FRAME(gSpriteBank31Frame076, SPRITE_TILES_BANK31_FRAME076);
const struct sprite_frame gSpriteBank31Frame077 = SPRITE_FRAME(gSpriteBank31Frame077, SPRITE_TILES_BANK31_FRAME077);
const struct sprite_frame gSpriteBank31Frame078 = SPRITE_FRAME(gSpriteBank31Frame078, SPRITE_TILES_BANK31_FRAME078);
const struct sprite_frame gSpriteBank31Frame079 = SPRITE_FRAME(gSpriteBank31Frame079, SPRITE_TILES_BANK31_FRAME079);
const struct sprite_frame gSpriteBank31Frame080 = SPRITE_FRAME(gSpriteBank31Frame080, SPRITE_TILES_BANK31_FRAME080);
const struct sprite_frame gSpriteBank31Frame081 = SPRITE_FRAME(gSpriteBank31Frame081, SPRITE_TILES_BANK31_FRAME081);
const struct sprite_frame gSpriteBank31Frame082 = SPRITE_FRAME(gSpriteBank31Frame082, SPRITE_TILES_BANK31_FRAME082);
const struct sprite_frame gSpriteBank31Frame083 = SPRITE_FRAME(gSpriteBank31Frame083, SPRITE_TILES_BANK31_FRAME083);
const struct sprite_frame gSpriteBank31Frame084 = SPRITE_FRAME(gSpriteBank31Frame084, SPRITE_TILES_BANK31_FRAME084);
const struct sprite_frame gSpriteBank31Frame085 = SPRITE_FRAME(gSpriteBank31Frame085, SPRITE_TILES_BANK31_FRAME085);
const struct sprite_frame gSpriteBank31Frame086 = SPRITE_FRAME(gSpriteBank31Frame086, SPRITE_TILES_BANK31_FRAME086);
const struct sprite_frame gSpriteBank31Frame087 = SPRITE_FRAME(gSpriteBank31Frame087, SPRITE_TILES_BANK31_FRAME087);
const struct sprite_frame gSpriteBank31Frame088 = SPRITE_FRAME(gSpriteBank31Frame088, SPRITE_TILES_BANK31_FRAME088);
const struct sprite_frame gSpriteBank31Frame089 = SPRITE_FRAME(gSpriteBank31Frame089, SPRITE_TILES_BANK31_FRAME089);
const struct sprite_frame gSpriteBank31Frame090 = SPRITE_FRAME(gSpriteBank31Frame090, SPRITE_TILES_BANK31_FRAME090);
const struct sprite_frame gSpriteBank31Frame091 = SPRITE_FRAME(gSpriteBank31Frame091, SPRITE_TILES_BANK31_FRAME091);
const struct sprite_frame gSpriteBank31Frame092 = SPRITE_FRAME(gSpriteBank31Frame092, SPRITE_TILES_BANK31_FRAME092);
const struct sprite_frame gSpriteBank31Frame093 = SPRITE_FRAME(gSpriteBank31Frame093, SPRITE_TILES_BANK31_FRAME093);
const struct sprite_frame_1box gSpriteBank31Frame094 = {
    SPRITE_FRAME(gSpriteBank31Frame094, SPRITE_TILES_BANK31_FRAME094),
    { { -12, -13, 26, 26 } },
};
const struct sprite_frame_1box gSpriteBank31Frame095 = {
    SPRITE_FRAME(gSpriteBank31Frame095, SPRITE_TILES_BANK31_FRAME095),
    { { -15, -11, 31, 24 } },
};
const struct sprite_frame_1box gSpriteBank31Frame096 = {
    SPRITE_FRAME(gSpriteBank31Frame096, SPRITE_TILES_BANK31_FRAME096),
    { { -16, -21, 33, 34 } },
};
const struct sprite_frame_1box gSpriteBank31Frame097 = {
    SPRITE_FRAME(gSpriteBank31Frame097, SPRITE_TILES_BANK31_FRAME097),
    { { -15, -29, 31, 42 } },
};
const struct sprite_frame_1box gSpriteBank31Frame098 = {
    SPRITE_FRAME(gSpriteBank31Frame098, SPRITE_TILES_BANK31_FRAME098),
    { { -12, -31, 25, 44 } },
};
const struct sprite_frame_1box gSpriteBank31Frame099 = {
    SPRITE_FRAME(gSpriteBank31Frame099, SPRITE_TILES_BANK31_FRAME099),
    { { -12, -35, 25, 48 } },
};
const struct sprite_frame_1box gSpriteBank31Frame100 = {
    SPRITE_FRAME(gSpriteBank31Frame100, SPRITE_TILES_BANK31_FRAME100),
    { { -13, -39, 26, 52 } },
};
const struct sprite_frame_1box gSpriteBank31Frame101 = {
    SPRITE_FRAME(gSpriteBank31Frame101, SPRITE_TILES_BANK31_FRAME101),
    { { -13, -40, 26, 53 } },
};
const struct sprite_frame_1box gSpriteBank31Frame102 = {
    SPRITE_FRAME(gSpriteBank31Frame102, SPRITE_TILES_BANK31_FRAME102),
    { { -12, -13, 26, 26 } },
};
const struct sprite_frame_1box gSpriteBank31Frame103 = {
    SPRITE_FRAME(gSpriteBank31Frame103, SPRITE_TILES_BANK31_FRAME103),
    { { -12, -13, 25, 26 } },
};
const struct sprite_frame_1box gSpriteBank31Frame104 = {
    SPRITE_FRAME(gSpriteBank31Frame104, SPRITE_TILES_BANK31_FRAME104),
    { { -15, -11, 31, 24 } },
};
const struct sprite_frame_1box gSpriteBank31Frame105 = {
    SPRITE_FRAME(gSpriteBank31Frame105, SPRITE_TILES_BANK31_FRAME105),
    { { -16, -21, 33, 34 } },
};
const struct sprite_frame_1box gSpriteBank31Frame106 = {
    SPRITE_FRAME(gSpriteBank31Frame106, SPRITE_TILES_BANK31_FRAME106),
    { { -15, -29, 31, 42 } },
};
const struct sprite_frame_1box gSpriteBank31Frame107 = {
    SPRITE_FRAME(gSpriteBank31Frame107, SPRITE_TILES_BANK31_FRAME107),
    { { -12, -31, 25, 44 } },
};
const struct sprite_frame_1box gSpriteBank31Frame108 = {
    SPRITE_FRAME(gSpriteBank31Frame108, SPRITE_TILES_BANK31_FRAME108),
    { { -12, -35, 25, 48 } },
};
const struct sprite_frame_1box gSpriteBank31Frame109 = {
    SPRITE_FRAME(gSpriteBank31Frame109, SPRITE_TILES_BANK31_FRAME109),
    { { -13, -39, 26, 52 } },
};
const struct sprite_frame_1box gSpriteBank31Frame110 = {
    SPRITE_FRAME(gSpriteBank31Frame110, SPRITE_TILES_BANK31_FRAME110),
    { { -13, -40, 26, 53 } },
};
const struct sprite_frame_1box gSpriteBank31Frame111 = {
    SPRITE_FRAME(gSpriteBank31Frame111, SPRITE_TILES_BANK31_FRAME111),
    { { -12, -13, 25, 26 } },
};

/* The piece arrays, from graphics/sprites/bank31_crates/bank.json. */
#include "sprites/bank31_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 32: 9 animations, 17 frames, graphics/sprites/bank32_gems/. */

extern const u16 gSpriteBank32Anim00Seq[8];
extern const u16 gSpriteBank32Anim01Seq[8];
extern const u16 gSpriteBank32Anim02Seq[8];
extern const u16 gSpriteBank32Anim03Seq[8];
extern const u16 gSpriteBank32Anim04Seq[8];
extern const u16 gSpriteBank32Anim05Seq[1];
extern const u16 gSpriteBank32Anim06Seq[1];
extern const u16 gSpriteBank32Anim07Seq[1];
extern const u16 gSpriteBank32Anim08Seq[1];

extern const struct sprite_anim gSpriteBank32Anims[9] = {
    /* 0 */ {
        /* seq */ gSpriteBank32Anim00Seq,
        /* box */ { { -12, -11, 25, 23 }, { -12, -11, 25, 23 } },
        /* paletteId */ 49,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank32Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank32Anim01Seq,
        /* box */ { { -12, -11, 25, 23 }, { -12, -11, 25, 23 } },
        /* paletteId */ 50,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank32Anim01Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 2 */ {
        /* seq */ gSpriteBank32Anim02Seq,
        /* box */ { { -12, -11, 25, 23 }, { -12, -11, 25, 23 } },
        /* paletteId */ 51,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank32Anim02Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 3 */ {
        /* seq */ gSpriteBank32Anim03Seq,
        /* box */ { { -12, -11, 25, 23 }, { -12, -11, 25, 23 } },
        /* paletteId */ 52,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank32Anim03Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 4 */ {
        /* seq */ gSpriteBank32Anim04Seq,
        /* box */ { { -12, -11, 25, 23 }, { -12, -11, 25, 23 } },
        /* paletteId */ 53,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank32Anim04Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 5 */ {
        /* seq */ gSpriteBank32Anim05Seq,
        /* box */ { { -20, -14, 41, 28 }, { -20, -14, 41, 28 } },
        /* paletteId */ 49,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank32Anim05Seq),
        /* flags */ 0,
    },
    /* 6 */ {
        /* seq */ gSpriteBank32Anim06Seq,
        /* box */ { { -20, -14, 41, 28 }, { -20, -14, 41, 28 } },
        /* paletteId */ 51,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank32Anim06Seq),
        /* flags */ 0,
    },
    /* 7 */ {
        /* seq */ gSpriteBank32Anim07Seq,
        /* box */ { { -20, -14, 41, 28 }, { -20, -14, 41, 28 } },
        /* paletteId */ 52,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank32Anim07Seq),
        /* flags */ 0,
    },
    /* 8 */ {
        /* seq */ gSpriteBank32Anim08Seq,
        /* box */ { { -20, -14, 41, 28 }, { -20, -14, 41, 28 } },
        /* paletteId */ 53,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank32Anim08Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank32Anim00Seq[8] = {
    0, 1, 2, 3, 4, 5, 6, 7,
};
const u16 gSpriteBank32Anim01Seq[8] = {
    0, 1, 2, 3, 4, 5, 6, 7,
};
const u16 gSpriteBank32Anim02Seq[8] = {
    8, 9, 10, 11, 12, 13, 14, 15,
};
const u16 gSpriteBank32Anim03Seq[8] = {
    0, 1, 2, 3, 4, 5, 6, 7,
};
const u16 gSpriteBank32Anim04Seq[8] = {
    0, 1, 2, 3, 4, 5, 6, 7,
};
const u16 gSpriteBank32Anim05Seq[1] = {
    16,
};
const u16 gSpriteBank32Anim06Seq[1] = {
    16,
};
const u16 gSpriteBank32Anim07Seq[1] = {
    16,
};
const u16 gSpriteBank32Anim08Seq[1] = {
    16,
};

extern const struct sprite_frame *const gSpriteBank32Frames[17] = {
    &gSpriteBank32Frame000.frame,
    &gSpriteBank32Frame001.frame,
    &gSpriteBank32Frame002.frame,
    &gSpriteBank32Frame003.frame,
    &gSpriteBank32Frame004.frame,
    &gSpriteBank32Frame005.frame,
    &gSpriteBank32Frame006.frame,
    &gSpriteBank32Frame007.frame,
    &gSpriteBank32Frame008,
    &gSpriteBank32Frame009,
    &gSpriteBank32Frame010,
    &gSpriteBank32Frame011,
    &gSpriteBank32Frame012,
    &gSpriteBank32Frame013,
    &gSpriteBank32Frame014,
    &gSpriteBank32Frame015,
    &gSpriteBank32Frame016.frame,
};

const struct sprite_frame_1box gSpriteBank32Frame000 = {
    SPRITE_FRAME(gSpriteBank32Frame000, SPRITE_TILES_BANK32_FRAME000),
    { { -12, -11, 25, 23 } },
};
const struct sprite_frame_1box gSpriteBank32Frame001 = {
    SPRITE_FRAME(gSpriteBank32Frame001, SPRITE_TILES_BANK32_FRAME001),
    { { -12, -11, 25, 23 } },
};
const struct sprite_frame_1box gSpriteBank32Frame002 = {
    SPRITE_FRAME(gSpriteBank32Frame002, SPRITE_TILES_BANK32_FRAME002),
    { { -12, -11, 25, 23 } },
};
const struct sprite_frame_1box gSpriteBank32Frame003 = {
    SPRITE_FRAME(gSpriteBank32Frame003, SPRITE_TILES_BANK32_FRAME003),
    { { -12, -11, 25, 23 } },
};
const struct sprite_frame_1box gSpriteBank32Frame004 = {
    SPRITE_FRAME(gSpriteBank32Frame004, SPRITE_TILES_BANK32_FRAME004),
    { { -12, -11, 25, 23 } },
};
const struct sprite_frame_1box gSpriteBank32Frame005 = {
    SPRITE_FRAME(gSpriteBank32Frame005, SPRITE_TILES_BANK32_FRAME005),
    { { -12, -11, 25, 23 } },
};
const struct sprite_frame_1box gSpriteBank32Frame006 = {
    SPRITE_FRAME(gSpriteBank32Frame006, SPRITE_TILES_BANK32_FRAME006),
    { { -12, -11, 25, 23 } },
};
const struct sprite_frame_1box gSpriteBank32Frame007 = {
    SPRITE_FRAME(gSpriteBank32Frame007, SPRITE_TILES_BANK32_FRAME007),
    { { -12, -11, 25, 23 } },
};
const struct sprite_frame gSpriteBank32Frame008 = SPRITE_FRAME(gSpriteBank32Frame008, SPRITE_TILES_BANK32_FRAME008);
const struct sprite_frame gSpriteBank32Frame009 = SPRITE_FRAME(gSpriteBank32Frame009, SPRITE_TILES_BANK32_FRAME009);
const struct sprite_frame gSpriteBank32Frame010 = SPRITE_FRAME(gSpriteBank32Frame010, SPRITE_TILES_BANK32_FRAME010);
const struct sprite_frame gSpriteBank32Frame011 = SPRITE_FRAME(gSpriteBank32Frame011, SPRITE_TILES_BANK32_FRAME011);
const struct sprite_frame gSpriteBank32Frame012 = SPRITE_FRAME(gSpriteBank32Frame012, SPRITE_TILES_BANK32_FRAME012);
const struct sprite_frame gSpriteBank32Frame013 = SPRITE_FRAME(gSpriteBank32Frame013, SPRITE_TILES_BANK32_FRAME013);
const struct sprite_frame gSpriteBank32Frame014 = SPRITE_FRAME(gSpriteBank32Frame014, SPRITE_TILES_BANK32_FRAME014);
const struct sprite_frame gSpriteBank32Frame015 = SPRITE_FRAME(gSpriteBank32Frame015, SPRITE_TILES_BANK32_FRAME015);
const struct sprite_frame_1box gSpriteBank32Frame016 = {
    SPRITE_FRAME(gSpriteBank32Frame016, SPRITE_TILES_BANK32_FRAME016),
    { { -20, -14, 41, 28 } },
};

/* The piece arrays, from graphics/sprites/bank32_gems/bank.json. */
#include "sprites/bank32_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 33: 3 animations, 16 frames, graphics/sprites/bank33_trial_icons/. */

extern const u16 gSpriteBank33Anim00Seq[8];
extern const u16 gSpriteBank33Anim01Seq[8];
extern const u16 gSpriteBank33Anim02Seq[8];

extern const struct sprite_anim gSpriteBank33Anims[3] = {
    /* 0 */ {
        /* seq */ gSpriteBank33Anim00Seq,
        /* box */ { { -14, -22, 28, 45 }, { -15, -22, 29, 45 } },
        /* paletteId */ 54,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank33Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank33Anim01Seq,
        /* box */ { { -14, -22, 28, 45 }, { -15, -22, 29, 45 } },
        /* paletteId */ 55,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank33Anim01Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 2 */ {
        /* seq */ gSpriteBank33Anim02Seq,
        /* box */ { { -14, -22, 28, 45 }, { -15, -22, 29, 45 } },
        /* paletteId */ 56,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank33Anim02Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank33Anim00Seq[8] = {
    0, 1, 2, 3, 4, 5, 6, 7,
};
const u16 gSpriteBank33Anim01Seq[8] = {
    8, 9, 10, 11, 12, 13, 14, 15,
};
const u16 gSpriteBank33Anim02Seq[8] = {
    8, 9, 10, 11, 12, 13, 14, 15,
};

extern const struct sprite_frame *const gSpriteBank33Frames[16] = {
    &gSpriteBank33Frame000.frame,
    &gSpriteBank33Frame001.frame,
    &gSpriteBank33Frame002.frame,
    &gSpriteBank33Frame003.frame,
    &gSpriteBank33Frame004.frame,
    &gSpriteBank33Frame005.frame,
    &gSpriteBank33Frame006.frame,
    &gSpriteBank33Frame007.frame,
    &gSpriteBank33Frame008.frame,
    &gSpriteBank33Frame009.frame,
    &gSpriteBank33Frame010.frame,
    &gSpriteBank33Frame011.frame,
    &gSpriteBank33Frame012.frame,
    &gSpriteBank33Frame013.frame,
    &gSpriteBank33Frame014.frame,
    &gSpriteBank33Frame015.frame,
};

const struct sprite_frame_1box gSpriteBank33Frame000 = {
    SPRITE_FRAME(gSpriteBank33Frame000, SPRITE_TILES_BANK33_FRAME000),
    { { -14, -22, 28, 45 } },
};
const struct sprite_frame_1box gSpriteBank33Frame001 = {
    SPRITE_FRAME(gSpriteBank33Frame001, SPRITE_TILES_BANK33_FRAME001),
    { { -15, -22, 29, 45 } },
};
const struct sprite_frame_1box gSpriteBank33Frame002 = {
    SPRITE_FRAME(gSpriteBank33Frame002, SPRITE_TILES_BANK33_FRAME002),
    { { -15, -22, 27, 45 } },
};
const struct sprite_frame_1box gSpriteBank33Frame003 = {
    SPRITE_FRAME(gSpriteBank33Frame003, SPRITE_TILES_BANK33_FRAME003),
    { { -12, -22, 21, 45 } },
};
const struct sprite_frame_1box gSpriteBank33Frame004 = {
    SPRITE_FRAME(gSpriteBank33Frame004, SPRITE_TILES_BANK33_FRAME004),
    { { -8, -22, 13, 44 } },
};
const struct sprite_frame_1box gSpriteBank33Frame005 = {
    SPRITE_FRAME(gSpriteBank33Frame005, SPRITE_TILES_BANK33_FRAME005),
    { { -3, -22, 7, 44 } },
};
const struct sprite_frame_1box gSpriteBank33Frame006 = {
    SPRITE_FRAME(gSpriteBank33Frame006, SPRITE_TILES_BANK33_FRAME006),
    { { -8, -22, 17, 45 } },
};
const struct sprite_frame_1box gSpriteBank33Frame007 = {
    SPRITE_FRAME(gSpriteBank33Frame007, SPRITE_TILES_BANK33_FRAME007),
    { { -12, -22, 25, 45 } },
};
const struct sprite_frame_1box gSpriteBank33Frame008 = {
    SPRITE_FRAME(gSpriteBank33Frame008, SPRITE_TILES_BANK33_FRAME008),
    { { -14, -22, 28, 45 } },
};
const struct sprite_frame_1box gSpriteBank33Frame009 = {
    SPRITE_FRAME(gSpriteBank33Frame009, SPRITE_TILES_BANK33_FRAME009),
    { { -15, -22, 29, 45 } },
};
const struct sprite_frame_1box gSpriteBank33Frame010 = {
    SPRITE_FRAME(gSpriteBank33Frame010, SPRITE_TILES_BANK33_FRAME010),
    { { -15, -22, 27, 45 } },
};
const struct sprite_frame_1box gSpriteBank33Frame011 = {
    SPRITE_FRAME(gSpriteBank33Frame011, SPRITE_TILES_BANK33_FRAME011),
    { { -12, -22, 21, 45 } },
};
const struct sprite_frame_1box gSpriteBank33Frame012 = {
    SPRITE_FRAME(gSpriteBank33Frame012, SPRITE_TILES_BANK33_FRAME012),
    { { -8, -22, 13, 44 } },
};
const struct sprite_frame_1box gSpriteBank33Frame013 = {
    SPRITE_FRAME(gSpriteBank33Frame013, SPRITE_TILES_BANK33_FRAME013),
    { { -3, -22, 7, 44 } },
};
const struct sprite_frame_1box gSpriteBank33Frame014 = {
    SPRITE_FRAME(gSpriteBank33Frame014, SPRITE_TILES_BANK33_FRAME014),
    { { -8, -22, 17, 45 } },
};
const struct sprite_frame_1box gSpriteBank33Frame015 = {
    SPRITE_FRAME(gSpriteBank33Frame015, SPRITE_TILES_BANK33_FRAME015),
    { { -12, -22, 25, 45 } },
};

/* The piece arrays, from graphics/sprites/bank33_trial_icons/bank.json. */
#include "sprites/bank33_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 34: 5 animations, 22 frames, graphics/sprites/bank34_aku_aku/. */

extern const u16 gSpriteBank34Anim00Seq[4];
extern const u16 gSpriteBank34Anim01Seq[4];
extern const u16 gSpriteBank34Anim02Seq[4];
extern const u16 gSpriteBank34Anim03Seq[5];
extern const u16 gSpriteBank34Anim04Seq[5];

extern const struct sprite_anim gSpriteBank34Anims[5] = {
    /* 0 */ {
        /* seq */ gSpriteBank34Anim00Seq,
        /* box */ { { -4, -9, 9, 19 }, { -8, -11, 15, 21 } },
        /* paletteId */ 57,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank34Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank34Anim01Seq,
        /* box */ { { -4, -9, 9, 19 }, { -8, -11, 15, 21 } },
        /* paletteId */ 57,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank34Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank34Anim02Seq,
        /* box */ { { -4, -9, 9, 19 }, { -8, -11, 15, 21 } },
        /* paletteId */ 57,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank34Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank34Anim03Seq,
        /* box */ { { -8, -4, 16, 9 }, { -15, -13, 30, 30 } },
        /* paletteId */ 57,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank34Anim03Seq),
        /* flags */ 0,
    },
    /* 4 */ {
        /* seq */ gSpriteBank34Anim04Seq,
        /* box */ { { -8, -4, 16, 9 }, { -15, -13, 30, 30 } },
        /* paletteId */ 57,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank34Anim04Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank34Anim00Seq[4] = {
    0, 1, 2, 3,
};
const u16 gSpriteBank34Anim01Seq[4] = {
    4, 5, 6, 7,
};
const u16 gSpriteBank34Anim02Seq[4] = {
    8, 9, 10, 11,
};
const u16 gSpriteBank34Anim03Seq[5] = {
    12, 13, 14, 15, 16,
};
const u16 gSpriteBank34Anim04Seq[5] = {
    17, 18, 19, 20, 21,
};

extern const struct sprite_frame *const gSpriteBank34Frames[22] = {
    &gSpriteBank34Frame000.frame,
    &gSpriteBank34Frame001.frame,
    &gSpriteBank34Frame002.frame,
    &gSpriteBank34Frame003.frame,
    &gSpriteBank34Frame004.frame,
    &gSpriteBank34Frame005.frame,
    &gSpriteBank34Frame006.frame,
    &gSpriteBank34Frame007.frame,
    &gSpriteBank34Frame008.frame,
    &gSpriteBank34Frame009.frame,
    &gSpriteBank34Frame010.frame,
    &gSpriteBank34Frame011.frame,
    &gSpriteBank34Frame012.frame,
    &gSpriteBank34Frame013.frame,
    &gSpriteBank34Frame014.frame,
    &gSpriteBank34Frame015.frame,
    &gSpriteBank34Frame016.frame,
    &gSpriteBank34Frame017.frame,
    &gSpriteBank34Frame018.frame,
    &gSpriteBank34Frame019.frame,
    &gSpriteBank34Frame020.frame,
    &gSpriteBank34Frame021.frame,
};

const struct sprite_frame_1box gSpriteBank34Frame000 = {
    SPRITE_FRAME(gSpriteBank34Frame000, SPRITE_TILES_BANK34_FRAME000),
    { { -4, -9, 9, 19 } },
};
const struct sprite_frame_1box gSpriteBank34Frame001 = {
    SPRITE_FRAME(gSpriteBank34Frame001, SPRITE_TILES_BANK34_FRAME001),
    { { -5, -9, 12, 19 } },
};
const struct sprite_frame_1box gSpriteBank34Frame002 = {
    SPRITE_FRAME(gSpriteBank34Frame002, SPRITE_TILES_BANK34_FRAME002),
    { { -8, -11, 12, 21 } },
};
const struct sprite_frame_1box gSpriteBank34Frame003 = {
    SPRITE_FRAME(gSpriteBank34Frame003, SPRITE_TILES_BANK34_FRAME003),
    { { -8, -11, 10, 21 } },
};
const struct sprite_frame_1box gSpriteBank34Frame004 = {
    SPRITE_FRAME(gSpriteBank34Frame004, SPRITE_TILES_BANK34_FRAME004),
    { { -4, -9, 9, 19 } },
};
const struct sprite_frame_1box gSpriteBank34Frame005 = {
    SPRITE_FRAME(gSpriteBank34Frame005, SPRITE_TILES_BANK34_FRAME005),
    { { -5, -9, 12, 19 } },
};
const struct sprite_frame_1box gSpriteBank34Frame006 = {
    SPRITE_FRAME(gSpriteBank34Frame006, SPRITE_TILES_BANK34_FRAME006),
    { { -8, -11, 12, 21 } },
};
const struct sprite_frame_1box gSpriteBank34Frame007 = {
    SPRITE_FRAME(gSpriteBank34Frame007, SPRITE_TILES_BANK34_FRAME007),
    { { -8, -11, 10, 21 } },
};
const struct sprite_frame_1box gSpriteBank34Frame008 = {
    SPRITE_FRAME(gSpriteBank34Frame008, SPRITE_TILES_BANK34_FRAME008),
    { { -4, -9, 9, 19 } },
};
const struct sprite_frame_1box gSpriteBank34Frame009 = {
    SPRITE_FRAME(gSpriteBank34Frame009, SPRITE_TILES_BANK34_FRAME009),
    { { -5, -9, 12, 19 } },
};
const struct sprite_frame_1box gSpriteBank34Frame010 = {
    SPRITE_FRAME(gSpriteBank34Frame010, SPRITE_TILES_BANK34_FRAME010),
    { { -8, -11, 12, 21 } },
};
const struct sprite_frame_1box gSpriteBank34Frame011 = {
    SPRITE_FRAME(gSpriteBank34Frame011, SPRITE_TILES_BANK34_FRAME011),
    { { -8, -11, 10, 21 } },
};
const struct sprite_frame_1box gSpriteBank34Frame012 = {
    SPRITE_FRAME(gSpriteBank34Frame012, SPRITE_TILES_BANK34_FRAME012),
    { { -8, -4, 16, 9 } },
};
const struct sprite_frame_1box gSpriteBank34Frame013 = {
    SPRITE_FRAME(gSpriteBank34Frame013, SPRITE_TILES_BANK34_FRAME013),
    { { -11, 1, 22, 10 } },
};
const struct sprite_frame_1box gSpriteBank34Frame014 = {
    SPRITE_FRAME(gSpriteBank34Frame014, SPRITE_TILES_BANK34_FRAME014),
    { { -10, 7, 19, 7 } },
};
const struct sprite_frame_1box gSpriteBank34Frame015 = {
    SPRITE_FRAME(gSpriteBank34Frame015, SPRITE_TILES_BANK34_FRAME015),
    { { -8, 10, 19, 8 } },
};
const struct sprite_frame_1box gSpriteBank34Frame016 = {
    SPRITE_FRAME(gSpriteBank34Frame016, SPRITE_TILES_BANK34_FRAME016),
    { { -8, 13, 15, 4 } },
};
const struct sprite_frame_1box gSpriteBank34Frame017 = {
    SPRITE_FRAME(gSpriteBank34Frame017, SPRITE_TILES_BANK34_FRAME017),
    { { -8, -4, 16, 9 } },
};
const struct sprite_frame_1box gSpriteBank34Frame018 = {
    SPRITE_FRAME(gSpriteBank34Frame018, SPRITE_TILES_BANK34_FRAME018),
    { { -11, 1, 22, 10 } },
};
const struct sprite_frame_1box gSpriteBank34Frame019 = {
    SPRITE_FRAME(gSpriteBank34Frame019, SPRITE_TILES_BANK34_FRAME019),
    { { -10, 7, 19, 7 } },
};
const struct sprite_frame_1box gSpriteBank34Frame020 = {
    SPRITE_FRAME(gSpriteBank34Frame020, SPRITE_TILES_BANK34_FRAME020),
    { { -8, 10, 19, 8 } },
};
const struct sprite_frame_1box gSpriteBank34Frame021 = {
    SPRITE_FRAME(gSpriteBank34Frame021, SPRITE_TILES_BANK34_FRAME021),
    { { -8, 13, 15, 4 } },
};

/* The piece arrays, from graphics/sprites/bank34_aku_aku/bank.json. */
#include "sprites/bank34_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 35: 2 animations, 28 frames, graphics/sprites/bank35_wumpa/. */

extern const u16 gSpriteBank35Anim00Seq[14];
extern const u16 gSpriteBank35Anim01Seq[14];

extern const struct sprite_anim gSpriteBank35Anims[2] = {
    /* 0 */ {
        /* seq */ gSpriteBank35Anim00Seq,
        /* box */ { { -9, -11, 19, 23 }, { -9, -11, 20, 23 } },
        /* paletteId */ 33,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank35Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank35Anim01Seq,
        /* box */ { { -6, -7, 12, 15 }, { -6, -7, 12, 15 } },
        /* paletteId */ 33,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank35Anim01Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank35Anim00Seq[14] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
};
const u16 gSpriteBank35Anim01Seq[14] = {
    14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
};

extern const struct sprite_frame *const gSpriteBank35Frames[28] = {
    &gSpriteBank35Frame000,
    &gSpriteBank35Frame001,
    &gSpriteBank35Frame002,
    &gSpriteBank35Frame003,
    &gSpriteBank35Frame004,
    &gSpriteBank35Frame005,
    &gSpriteBank35Frame006,
    &gSpriteBank35Frame007,
    &gSpriteBank35Frame008,
    &gSpriteBank35Frame009,
    &gSpriteBank35Frame010,
    &gSpriteBank35Frame011,
    &gSpriteBank35Frame012,
    &gSpriteBank35Frame013,
    &gSpriteBank35Frame014,
    &gSpriteBank35Frame015,
    &gSpriteBank35Frame016,
    &gSpriteBank35Frame017,
    &gSpriteBank35Frame018,
    &gSpriteBank35Frame019,
    &gSpriteBank35Frame020,
    &gSpriteBank35Frame021,
    &gSpriteBank35Frame022,
    &gSpriteBank35Frame023,
    &gSpriteBank35Frame024,
    &gSpriteBank35Frame025,
    &gSpriteBank35Frame026,
    &gSpriteBank35Frame027,
};

const struct sprite_frame gSpriteBank35Frame000 = SPRITE_FRAME(gSpriteBank35Frame000, SPRITE_TILES_BANK35_FRAME000);
const struct sprite_frame gSpriteBank35Frame001 = SPRITE_FRAME(gSpriteBank35Frame001, SPRITE_TILES_BANK35_FRAME001);
const struct sprite_frame gSpriteBank35Frame002 = SPRITE_FRAME(gSpriteBank35Frame002, SPRITE_TILES_BANK35_FRAME002);
const struct sprite_frame gSpriteBank35Frame003 = SPRITE_FRAME(gSpriteBank35Frame003, SPRITE_TILES_BANK35_FRAME003);
const struct sprite_frame gSpriteBank35Frame004 = SPRITE_FRAME(gSpriteBank35Frame004, SPRITE_TILES_BANK35_FRAME004);
const struct sprite_frame gSpriteBank35Frame005 = SPRITE_FRAME(gSpriteBank35Frame005, SPRITE_TILES_BANK35_FRAME005);
const struct sprite_frame gSpriteBank35Frame006 = SPRITE_FRAME(gSpriteBank35Frame006, SPRITE_TILES_BANK35_FRAME006);
const struct sprite_frame gSpriteBank35Frame007 = SPRITE_FRAME(gSpriteBank35Frame007, SPRITE_TILES_BANK35_FRAME007);
const struct sprite_frame gSpriteBank35Frame008 = SPRITE_FRAME(gSpriteBank35Frame008, SPRITE_TILES_BANK35_FRAME008);
const struct sprite_frame gSpriteBank35Frame009 = SPRITE_FRAME(gSpriteBank35Frame009, SPRITE_TILES_BANK35_FRAME009);
const struct sprite_frame gSpriteBank35Frame010 = SPRITE_FRAME(gSpriteBank35Frame010, SPRITE_TILES_BANK35_FRAME010);
const struct sprite_frame gSpriteBank35Frame011 = SPRITE_FRAME(gSpriteBank35Frame011, SPRITE_TILES_BANK35_FRAME011);
const struct sprite_frame gSpriteBank35Frame012 = SPRITE_FRAME(gSpriteBank35Frame012, SPRITE_TILES_BANK35_FRAME012);
const struct sprite_frame gSpriteBank35Frame013 = SPRITE_FRAME(gSpriteBank35Frame013, SPRITE_TILES_BANK35_FRAME013);
const struct sprite_frame gSpriteBank35Frame014 = SPRITE_FRAME(gSpriteBank35Frame014, SPRITE_TILES_BANK35_FRAME014);
const struct sprite_frame gSpriteBank35Frame015 = SPRITE_FRAME(gSpriteBank35Frame015, SPRITE_TILES_BANK35_FRAME015);
const struct sprite_frame gSpriteBank35Frame016 = SPRITE_FRAME(gSpriteBank35Frame016, SPRITE_TILES_BANK35_FRAME016);
const struct sprite_frame gSpriteBank35Frame017 = SPRITE_FRAME(gSpriteBank35Frame017, SPRITE_TILES_BANK35_FRAME017);
const struct sprite_frame gSpriteBank35Frame018 = SPRITE_FRAME(gSpriteBank35Frame018, SPRITE_TILES_BANK35_FRAME018);
const struct sprite_frame gSpriteBank35Frame019 = SPRITE_FRAME(gSpriteBank35Frame019, SPRITE_TILES_BANK35_FRAME019);
const struct sprite_frame gSpriteBank35Frame020 = SPRITE_FRAME(gSpriteBank35Frame020, SPRITE_TILES_BANK35_FRAME020);
const struct sprite_frame gSpriteBank35Frame021 = SPRITE_FRAME(gSpriteBank35Frame021, SPRITE_TILES_BANK35_FRAME021);
const struct sprite_frame gSpriteBank35Frame022 = SPRITE_FRAME(gSpriteBank35Frame022, SPRITE_TILES_BANK35_FRAME022);
const struct sprite_frame gSpriteBank35Frame023 = SPRITE_FRAME(gSpriteBank35Frame023, SPRITE_TILES_BANK35_FRAME023);
const struct sprite_frame gSpriteBank35Frame024 = SPRITE_FRAME(gSpriteBank35Frame024, SPRITE_TILES_BANK35_FRAME024);
const struct sprite_frame gSpriteBank35Frame025 = SPRITE_FRAME(gSpriteBank35Frame025, SPRITE_TILES_BANK35_FRAME025);
const struct sprite_frame gSpriteBank35Frame026 = SPRITE_FRAME(gSpriteBank35Frame026, SPRITE_TILES_BANK35_FRAME026);
const struct sprite_frame gSpriteBank35Frame027 = SPRITE_FRAME(gSpriteBank35Frame027, SPRITE_TILES_BANK35_FRAME027);

/* The piece arrays, from graphics/sprites/bank35_wumpa/bank.json. */
#include "sprites/bank35_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 36: 1 animation, 10 frames, graphics/sprites/bank36_stopwatch/. */

extern const u16 gSpriteBank36Anim00Seq[10];

extern const struct sprite_anim gSpriteBank36Anims[1] = {
    /* 0 */ {
        /* seq */ gSpriteBank36Anim00Seq,
        /* box */ { { -8, -11, 17, 22 }, { -9, -11, 18, 23 } },
        /* paletteId */ 58,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank36Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank36Anim00Seq[10] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
};

extern const struct sprite_frame *const gSpriteBank36Frames[10] = {
    &gSpriteBank36Frame000,
    &gSpriteBank36Frame001,
    &gSpriteBank36Frame002,
    &gSpriteBank36Frame003,
    &gSpriteBank36Frame004,
    &gSpriteBank36Frame005,
    &gSpriteBank36Frame006,
    &gSpriteBank36Frame007,
    &gSpriteBank36Frame008,
    &gSpriteBank36Frame009,
};

const struct sprite_frame gSpriteBank36Frame000 = SPRITE_FRAME(gSpriteBank36Frame000, SPRITE_TILES_BANK36_FRAME000);
const struct sprite_frame gSpriteBank36Frame001 = SPRITE_FRAME(gSpriteBank36Frame001, SPRITE_TILES_BANK36_FRAME001);
const struct sprite_frame gSpriteBank36Frame002 = SPRITE_FRAME(gSpriteBank36Frame002, SPRITE_TILES_BANK36_FRAME002);
const struct sprite_frame gSpriteBank36Frame003 = SPRITE_FRAME(gSpriteBank36Frame003, SPRITE_TILES_BANK36_FRAME003);
const struct sprite_frame gSpriteBank36Frame004 = SPRITE_FRAME(gSpriteBank36Frame004, SPRITE_TILES_BANK36_FRAME004);
const struct sprite_frame gSpriteBank36Frame005 = SPRITE_FRAME(gSpriteBank36Frame005, SPRITE_TILES_BANK36_FRAME005);
const struct sprite_frame gSpriteBank36Frame006 = SPRITE_FRAME(gSpriteBank36Frame006, SPRITE_TILES_BANK36_FRAME006);
const struct sprite_frame gSpriteBank36Frame007 = SPRITE_FRAME(gSpriteBank36Frame007, SPRITE_TILES_BANK36_FRAME007);
const struct sprite_frame gSpriteBank36Frame008 = SPRITE_FRAME(gSpriteBank36Frame008, SPRITE_TILES_BANK36_FRAME008);
const struct sprite_frame gSpriteBank36Frame009 = SPRITE_FRAME(gSpriteBank36Frame009, SPRITE_TILES_BANK36_FRAME009);

/* The piece arrays, from graphics/sprites/bank36_stopwatch/bank.json. */
#include "sprites/bank36_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 37: 1 animation, 8 frames, graphics/sprites/bank37_crystal/. */

extern const u16 gSpriteBank37Anim00Seq[8];

extern const struct sprite_anim gSpriteBank37Anims[1] = {
    /* 0 */ {
        /* seq */ gSpriteBank37Anim00Seq,
        /* box */ { { -6, -21, 13, 43 }, { -6, -21, 13, 43 } },
        /* paletteId */ 117,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank37Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank37Anim00Seq[8] = {
    0, 1, 2, 3, 4, 5, 6, 7,
};

extern const struct sprite_frame *const gSpriteBank37Frames[8] = {
    &gSpriteBank37Frame000,
    &gSpriteBank37Frame001,
    &gSpriteBank37Frame002,
    &gSpriteBank37Frame003,
    &gSpriteBank37Frame004,
    &gSpriteBank37Frame005,
    &gSpriteBank37Frame006,
    &gSpriteBank37Frame007,
};

const struct sprite_frame gSpriteBank37Frame000 = SPRITE_FRAME(gSpriteBank37Frame000, SPRITE_TILES_BANK37_FRAME000);
const struct sprite_frame gSpriteBank37Frame001 = SPRITE_FRAME(gSpriteBank37Frame001, SPRITE_TILES_BANK37_FRAME001);
const struct sprite_frame gSpriteBank37Frame002 = SPRITE_FRAME(gSpriteBank37Frame002, SPRITE_TILES_BANK37_FRAME002);
const struct sprite_frame gSpriteBank37Frame003 = SPRITE_FRAME(gSpriteBank37Frame003, SPRITE_TILES_BANK37_FRAME003);
const struct sprite_frame gSpriteBank37Frame004 = SPRITE_FRAME(gSpriteBank37Frame004, SPRITE_TILES_BANK37_FRAME004);
const struct sprite_frame gSpriteBank37Frame005 = SPRITE_FRAME(gSpriteBank37Frame005, SPRITE_TILES_BANK37_FRAME005);
const struct sprite_frame gSpriteBank37Frame006 = SPRITE_FRAME(gSpriteBank37Frame006, SPRITE_TILES_BANK37_FRAME006);
const struct sprite_frame gSpriteBank37Frame007 = SPRITE_FRAME(gSpriteBank37Frame007, SPRITE_TILES_BANK37_FRAME007);

/* The piece arrays, from graphics/sprites/bank37_crystal/bank.json. */
#include "sprites/bank37_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 38: 4 animations, 60 frames, graphics/sprites/bank38_powers/. */

extern const u16 gSpriteBank38Anim00Seq[15];
extern const u16 gSpriteBank38Anim01Seq[15];
extern const u16 gSpriteBank38Anim02Seq[15];
extern const u16 gSpriteBank38Anim03Seq[15];

extern const struct sprite_anim gSpriteBank38Anims[4] = {
    /* 0 */ {
        /* seq */ gSpriteBank38Anim00Seq,
        /* box */ { { -2, -14, 4, 29 }, { -13, -14, 26, 29 } },
        /* paletteId */ 60,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank38Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank38Anim01Seq,
        /* box */ { { -2, -14, 4, 29 }, { -13, -14, 26, 29 } },
        /* paletteId */ 60,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank38Anim01Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 2 */ {
        /* seq */ gSpriteBank38Anim02Seq,
        /* box */ { { -2, -14, 4, 29 }, { -13, -14, 26, 29 } },
        /* paletteId */ 60,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank38Anim02Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 3 */ {
        /* seq */ gSpriteBank38Anim03Seq,
        /* box */ { { -2, -14, 4, 29 }, { -13, -14, 26, 29 } },
        /* paletteId */ 60,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank38Anim03Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank38Anim00Seq[15] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
};
const u16 gSpriteBank38Anim01Seq[15] = {
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
};
const u16 gSpriteBank38Anim02Seq[15] = {
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44,
};
const u16 gSpriteBank38Anim03Seq[15] = {
    45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
};

extern const struct sprite_frame *const gSpriteBank38Frames[60] = {
    &gSpriteBank38Frame000,
    &gSpriteBank38Frame001,
    &gSpriteBank38Frame002,
    &gSpriteBank38Frame003,
    &gSpriteBank38Frame004,
    &gSpriteBank38Frame005,
    &gSpriteBank38Frame006,
    &gSpriteBank38Frame007,
    &gSpriteBank38Frame008,
    &gSpriteBank38Frame009,
    &gSpriteBank38Frame010,
    &gSpriteBank38Frame011,
    &gSpriteBank38Frame012,
    &gSpriteBank38Frame013,
    &gSpriteBank38Frame014,
    &gSpriteBank38Frame015,
    &gSpriteBank38Frame016,
    &gSpriteBank38Frame017,
    &gSpriteBank38Frame018,
    &gSpriteBank38Frame019,
    &gSpriteBank38Frame020,
    &gSpriteBank38Frame021,
    &gSpriteBank38Frame022,
    &gSpriteBank38Frame023,
    &gSpriteBank38Frame024,
    &gSpriteBank38Frame025,
    &gSpriteBank38Frame026,
    &gSpriteBank38Frame027,
    &gSpriteBank38Frame028,
    &gSpriteBank38Frame029,
    &gSpriteBank38Frame030,
    &gSpriteBank38Frame031,
    &gSpriteBank38Frame032,
    &gSpriteBank38Frame033,
    &gSpriteBank38Frame034,
    &gSpriteBank38Frame035,
    &gSpriteBank38Frame036,
    &gSpriteBank38Frame037,
    &gSpriteBank38Frame038,
    &gSpriteBank38Frame039,
    &gSpriteBank38Frame040,
    &gSpriteBank38Frame041,
    &gSpriteBank38Frame042,
    &gSpriteBank38Frame043,
    &gSpriteBank38Frame044,
    &gSpriteBank38Frame045,
    &gSpriteBank38Frame046,
    &gSpriteBank38Frame047,
    &gSpriteBank38Frame048,
    &gSpriteBank38Frame049,
    &gSpriteBank38Frame050,
    &gSpriteBank38Frame051,
    &gSpriteBank38Frame052,
    &gSpriteBank38Frame053,
    &gSpriteBank38Frame054,
    &gSpriteBank38Frame055,
    &gSpriteBank38Frame056,
    &gSpriteBank38Frame057,
    &gSpriteBank38Frame058,
    &gSpriteBank38Frame059,
};

const struct sprite_frame gSpriteBank38Frame000 = SPRITE_FRAME(gSpriteBank38Frame000, SPRITE_TILES_BANK38_FRAME000);
const struct sprite_frame gSpriteBank38Frame001 = SPRITE_FRAME(gSpriteBank38Frame001, SPRITE_TILES_BANK38_FRAME001);
const struct sprite_frame gSpriteBank38Frame002 = SPRITE_FRAME(gSpriteBank38Frame002, SPRITE_TILES_BANK38_FRAME002);
const struct sprite_frame gSpriteBank38Frame003 = SPRITE_FRAME(gSpriteBank38Frame003, SPRITE_TILES_BANK38_FRAME003);
const struct sprite_frame gSpriteBank38Frame004 = SPRITE_FRAME(gSpriteBank38Frame004, SPRITE_TILES_BANK38_FRAME004);
const struct sprite_frame gSpriteBank38Frame005 = SPRITE_FRAME(gSpriteBank38Frame005, SPRITE_TILES_BANK38_FRAME005);
const struct sprite_frame gSpriteBank38Frame006 = SPRITE_FRAME(gSpriteBank38Frame006, SPRITE_TILES_BANK38_FRAME006);
const struct sprite_frame gSpriteBank38Frame007 = SPRITE_FRAME(gSpriteBank38Frame007, SPRITE_TILES_BANK38_FRAME007);
const struct sprite_frame gSpriteBank38Frame008 = SPRITE_FRAME(gSpriteBank38Frame008, SPRITE_TILES_BANK38_FRAME008);
const struct sprite_frame gSpriteBank38Frame009 = SPRITE_FRAME(gSpriteBank38Frame009, SPRITE_TILES_BANK38_FRAME009);
const struct sprite_frame gSpriteBank38Frame010 = SPRITE_FRAME(gSpriteBank38Frame010, SPRITE_TILES_BANK38_FRAME010);
const struct sprite_frame gSpriteBank38Frame011 = SPRITE_FRAME(gSpriteBank38Frame011, SPRITE_TILES_BANK38_FRAME011);
const struct sprite_frame gSpriteBank38Frame012 = SPRITE_FRAME(gSpriteBank38Frame012, SPRITE_TILES_BANK38_FRAME012);
const struct sprite_frame gSpriteBank38Frame013 = SPRITE_FRAME(gSpriteBank38Frame013, SPRITE_TILES_BANK38_FRAME013);
const struct sprite_frame gSpriteBank38Frame014 = SPRITE_FRAME(gSpriteBank38Frame014, SPRITE_TILES_BANK38_FRAME014);
const struct sprite_frame gSpriteBank38Frame015 = SPRITE_FRAME(gSpriteBank38Frame015, SPRITE_TILES_BANK38_FRAME015);
const struct sprite_frame gSpriteBank38Frame016 = SPRITE_FRAME(gSpriteBank38Frame016, SPRITE_TILES_BANK38_FRAME016);
const struct sprite_frame gSpriteBank38Frame017 = SPRITE_FRAME(gSpriteBank38Frame017, SPRITE_TILES_BANK38_FRAME017);
const struct sprite_frame gSpriteBank38Frame018 = SPRITE_FRAME(gSpriteBank38Frame018, SPRITE_TILES_BANK38_FRAME018);
const struct sprite_frame gSpriteBank38Frame019 = SPRITE_FRAME(gSpriteBank38Frame019, SPRITE_TILES_BANK38_FRAME019);
const struct sprite_frame gSpriteBank38Frame020 = SPRITE_FRAME(gSpriteBank38Frame020, SPRITE_TILES_BANK38_FRAME020);
const struct sprite_frame gSpriteBank38Frame021 = SPRITE_FRAME(gSpriteBank38Frame021, SPRITE_TILES_BANK38_FRAME021);
const struct sprite_frame gSpriteBank38Frame022 = SPRITE_FRAME(gSpriteBank38Frame022, SPRITE_TILES_BANK38_FRAME022);
const struct sprite_frame gSpriteBank38Frame023 = SPRITE_FRAME(gSpriteBank38Frame023, SPRITE_TILES_BANK38_FRAME023);
const struct sprite_frame gSpriteBank38Frame024 = SPRITE_FRAME(gSpriteBank38Frame024, SPRITE_TILES_BANK38_FRAME024);
const struct sprite_frame gSpriteBank38Frame025 = SPRITE_FRAME(gSpriteBank38Frame025, SPRITE_TILES_BANK38_FRAME025);
const struct sprite_frame gSpriteBank38Frame026 = SPRITE_FRAME(gSpriteBank38Frame026, SPRITE_TILES_BANK38_FRAME026);
const struct sprite_frame gSpriteBank38Frame027 = SPRITE_FRAME(gSpriteBank38Frame027, SPRITE_TILES_BANK38_FRAME027);
const struct sprite_frame gSpriteBank38Frame028 = SPRITE_FRAME(gSpriteBank38Frame028, SPRITE_TILES_BANK38_FRAME028);
const struct sprite_frame gSpriteBank38Frame029 = SPRITE_FRAME(gSpriteBank38Frame029, SPRITE_TILES_BANK38_FRAME029);
const struct sprite_frame gSpriteBank38Frame030 = SPRITE_FRAME(gSpriteBank38Frame030, SPRITE_TILES_BANK38_FRAME030);
const struct sprite_frame gSpriteBank38Frame031 = SPRITE_FRAME(gSpriteBank38Frame031, SPRITE_TILES_BANK38_FRAME031);
const struct sprite_frame gSpriteBank38Frame032 = SPRITE_FRAME(gSpriteBank38Frame032, SPRITE_TILES_BANK38_FRAME032);
const struct sprite_frame gSpriteBank38Frame033 = SPRITE_FRAME(gSpriteBank38Frame033, SPRITE_TILES_BANK38_FRAME033);
const struct sprite_frame gSpriteBank38Frame034 = SPRITE_FRAME(gSpriteBank38Frame034, SPRITE_TILES_BANK38_FRAME034);
const struct sprite_frame gSpriteBank38Frame035 = SPRITE_FRAME(gSpriteBank38Frame035, SPRITE_TILES_BANK38_FRAME035);
const struct sprite_frame gSpriteBank38Frame036 = SPRITE_FRAME(gSpriteBank38Frame036, SPRITE_TILES_BANK38_FRAME036);
const struct sprite_frame gSpriteBank38Frame037 = SPRITE_FRAME(gSpriteBank38Frame037, SPRITE_TILES_BANK38_FRAME037);
const struct sprite_frame gSpriteBank38Frame038 = SPRITE_FRAME(gSpriteBank38Frame038, SPRITE_TILES_BANK38_FRAME038);
const struct sprite_frame gSpriteBank38Frame039 = SPRITE_FRAME(gSpriteBank38Frame039, SPRITE_TILES_BANK38_FRAME039);
const struct sprite_frame gSpriteBank38Frame040 = SPRITE_FRAME(gSpriteBank38Frame040, SPRITE_TILES_BANK38_FRAME040);
const struct sprite_frame gSpriteBank38Frame041 = SPRITE_FRAME(gSpriteBank38Frame041, SPRITE_TILES_BANK38_FRAME041);
const struct sprite_frame gSpriteBank38Frame042 = SPRITE_FRAME(gSpriteBank38Frame042, SPRITE_TILES_BANK38_FRAME042);
const struct sprite_frame gSpriteBank38Frame043 = SPRITE_FRAME(gSpriteBank38Frame043, SPRITE_TILES_BANK38_FRAME043);
const struct sprite_frame gSpriteBank38Frame044 = SPRITE_FRAME(gSpriteBank38Frame044, SPRITE_TILES_BANK38_FRAME044);
const struct sprite_frame gSpriteBank38Frame045 = SPRITE_FRAME(gSpriteBank38Frame045, SPRITE_TILES_BANK38_FRAME045);
const struct sprite_frame gSpriteBank38Frame046 = SPRITE_FRAME(gSpriteBank38Frame046, SPRITE_TILES_BANK38_FRAME046);
const struct sprite_frame gSpriteBank38Frame047 = SPRITE_FRAME(gSpriteBank38Frame047, SPRITE_TILES_BANK38_FRAME047);
const struct sprite_frame gSpriteBank38Frame048 = SPRITE_FRAME(gSpriteBank38Frame048, SPRITE_TILES_BANK38_FRAME048);
const struct sprite_frame gSpriteBank38Frame049 = SPRITE_FRAME(gSpriteBank38Frame049, SPRITE_TILES_BANK38_FRAME049);
const struct sprite_frame gSpriteBank38Frame050 = SPRITE_FRAME(gSpriteBank38Frame050, SPRITE_TILES_BANK38_FRAME050);
const struct sprite_frame gSpriteBank38Frame051 = SPRITE_FRAME(gSpriteBank38Frame051, SPRITE_TILES_BANK38_FRAME051);
const struct sprite_frame gSpriteBank38Frame052 = SPRITE_FRAME(gSpriteBank38Frame052, SPRITE_TILES_BANK38_FRAME052);
const struct sprite_frame gSpriteBank38Frame053 = SPRITE_FRAME(gSpriteBank38Frame053, SPRITE_TILES_BANK38_FRAME053);
const struct sprite_frame gSpriteBank38Frame054 = SPRITE_FRAME(gSpriteBank38Frame054, SPRITE_TILES_BANK38_FRAME054);
const struct sprite_frame gSpriteBank38Frame055 = SPRITE_FRAME(gSpriteBank38Frame055, SPRITE_TILES_BANK38_FRAME055);
const struct sprite_frame gSpriteBank38Frame056 = SPRITE_FRAME(gSpriteBank38Frame056, SPRITE_TILES_BANK38_FRAME056);
const struct sprite_frame gSpriteBank38Frame057 = SPRITE_FRAME(gSpriteBank38Frame057, SPRITE_TILES_BANK38_FRAME057);
const struct sprite_frame gSpriteBank38Frame058 = SPRITE_FRAME(gSpriteBank38Frame058, SPRITE_TILES_BANK38_FRAME058);
const struct sprite_frame gSpriteBank38Frame059 = SPRITE_FRAME(gSpriteBank38Frame059, SPRITE_TILES_BANK38_FRAME059);

/* The piece arrays, from graphics/sprites/bank38_powers/bank.json. */
#include "sprites/bank38_pieces.inc"
