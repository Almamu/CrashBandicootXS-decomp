extern "C" {
#include "gba/types.h"
#include "sprite_bank.h"
}
#include "sprites/sprite_frames.h"

/*
 * ROM 0x084b9d7c-0x084c0006: sprite banks 39-55 of the sprite-bank
 * animation system (gSpriteBankTable, include/sprite_bank.h). Linked in
 * ROM order between data/data.s sections by ldscript.txt - see
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
 * offset in the sprite tile pool (src/data/sprite_tiles_2bf120.cpp),
 * SPRITE_TILES_BANKnn_FRAMEkkk. A frame's struct type has to match its
 * "layout" in bank.json.
 */

/* ---------------------------------------------------------------------- */
/* Bank 39: 13 animations, 24 frames, graphics/sprites/bank39_platforms/. */

extern const u16 gSpriteBank39Anim00Seq[1];
extern const u16 gSpriteBank39Anim01Seq[1];
extern const u16 gSpriteBank39Anim02Seq[1];
extern const u16 gSpriteBank39Anim03Seq[1];
extern const u16 gSpriteBank39Anim04Seq[1];
extern const u16 gSpriteBank39Anim05Seq[1];
extern const u16 gSpriteBank39Anim06Seq[33];
extern const u16 gSpriteBank39Anim07Seq[1];
extern const u16 gSpriteBank39Anim08Seq[8];
extern const u16 gSpriteBank39Anim09Seq[1];
extern const u16 gSpriteBank39Anim10Seq[1];
extern const u16 gSpriteBank39Anim11Seq[1];
extern const u16 gSpriteBank39Anim12Seq[1];

extern const struct sprite_anim gSpriteBank39Anims[13] = {
    /* 0 */ {
        /* seq */ gSpriteBank39Anim00Seq,
        /* box */ { { -56, 3, 106, 9 }, { -55, -11, 111, 23 } },
        /* paletteId */ 61,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank39Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank39Anim01Seq,
        /* box */ { { -23, 4, 38, 9 }, { -23, -11, 47, 23 } },
        /* paletteId */ 61,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank39Anim01Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 2 */ {
        /* seq */ gSpriteBank39Anim02Seq,
        /* box */ { { -39, 4, 69, 10 }, { -39, -11, 79, 23 } },
        /* paletteId */ 61,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank39Anim02Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 3 */ {
        /* seq */ gSpriteBank39Anim03Seq,
        /* box */ { { -20, -3, 43, 18 }, { -20, -14, 42, 28 } },
        /* paletteId */ 62,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank39Anim03Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 4 */ {
        /* seq */ gSpriteBank39Anim04Seq,
        /* box */ { { -20, 3, 42, 11 }, { -22, -13, 46, 28 } },
        /* paletteId */ 57,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank39Anim04Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 5 */ {
        /* seq */ gSpriteBank39Anim05Seq,
        /* box */ { { -18, -1, 37, 15 }, { -20, -14, 41, 28 } },
        /* paletteId */ 57,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank39Anim05Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 6 */ {
        /* seq */ gSpriteBank39Anim06Seq,
        /* box */ { { -21, 0, 42, 9 }, { -20, -9, 40, 22 } },
        /* paletteId */ 119,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank39Anim06Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 7 */ {
        /* seq */ gSpriteBank39Anim07Seq,
        /* box */ { { -18, -1, 37, 15 }, { -20, -14, 41, 28 } },
        /* paletteId */ 57,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank39Anim07Seq),
        /* flags */ 0,
    },
    /* 8 */ {
        /* seq */ gSpriteBank39Anim08Seq,
        /* box */ { { -35, -1, 64, 11 }, { -63, -17, 119, 37 } },
        /* paletteId */ 64,
        /* duration */ 7,
        /* frameCount */ ARRAY_COUNT(gSpriteBank39Anim08Seq),
        /* flags */ 0,
    },
    /* 9 */ {
        /* seq */ gSpriteBank39Anim09Seq,
        /* box */ { { -20, -3, 43, 18 }, { -20, -14, 42, 28 } },
        /* paletteId */ 53,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank39Anim09Seq),
        /* flags */ 0,
    },
    /* 10 */ {
        /* seq */ gSpriteBank39Anim10Seq,
        /* box */ { { -20, -3, 43, 18 }, { -20, -14, 42, 28 } },
        /* paletteId */ 51,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank39Anim10Seq),
        /* flags */ 0,
    },
    /* 11 */ {
        /* seq */ gSpriteBank39Anim11Seq,
        /* box */ { { -20, -3, 43, 18 }, { -20, -14, 42, 28 } },
        /* paletteId */ 52,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank39Anim11Seq),
        /* flags */ 0,
    },
    /* 12 */ {
        /* seq */ gSpriteBank39Anim12Seq,
        /* box */ { { -20, -3, 43, 18 }, { -20, -14, 42, 28 } },
        /* paletteId */ 50,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank39Anim12Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank39Anim00Seq[1] = {
    0,
};
const u16 gSpriteBank39Anim01Seq[1] = {
    1,
};
const u16 gSpriteBank39Anim02Seq[1] = {
    2,
};
const u16 gSpriteBank39Anim03Seq[1] = {
    3,
};
const u16 gSpriteBank39Anim04Seq[1] = {
    4,
};
const u16 gSpriteBank39Anim05Seq[1] = {
    5,
};
const u16 gSpriteBank39Anim06Seq[33] = {
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 6, 7, 6, 7,
    6, 7, 8, 9, 10, 11, 12, 13, 13, 13, 13, 12, 11, 10, 14, 15,
    7,
};
const u16 gSpriteBank39Anim07Seq[1] = {
    16,
};
const u16 gSpriteBank39Anim08Seq[8] = {
    17, 18, 19, 20, 21, 22, 22, 22,
};
const u16 gSpriteBank39Anim09Seq[1] = {
    23,
};
const u16 gSpriteBank39Anim10Seq[1] = {
    23,
};
const u16 gSpriteBank39Anim11Seq[1] = {
    23,
};
const u16 gSpriteBank39Anim12Seq[1] = {
    23,
};

extern const struct sprite_frame *const gSpriteBank39Frames[24] = {
    &gSpriteBank39Frame000.frame,
    &gSpriteBank39Frame001.frame,
    &gSpriteBank39Frame002.frame,
    &gSpriteBank39Frame003.frame,
    &gSpriteBank39Frame004.frame,
    &gSpriteBank39Frame005,
    &gSpriteBank39Frame006.frame,
    &gSpriteBank39Frame007.frame,
    &gSpriteBank39Frame008.frame,
    &gSpriteBank39Frame009.frame,
    &gSpriteBank39Frame010.frame,
    &gSpriteBank39Frame011.frame,
    &gSpriteBank39Frame012.frame,
    &gSpriteBank39Frame013.frame,
    &gSpriteBank39Frame014.frame,
    &gSpriteBank39Frame015.frame,
    &gSpriteBank39Frame016,
    &gSpriteBank39Frame017.frame,
    &gSpriteBank39Frame018.frame,
    &gSpriteBank39Frame019.frame,
    &gSpriteBank39Frame020.frame,
    &gSpriteBank39Frame021.frame,
    &gSpriteBank39Frame022.frame,
    &gSpriteBank39Frame023.frame,
};

const struct sprite_frame_1box gSpriteBank39Frame000 = {
    SPRITE_FRAME(gSpriteBank39Frame000, SPRITE_TILES_BANK39_FRAME000),
    { { -55, -11, 111, 23 } },
};
const struct sprite_frame_1box gSpriteBank39Frame001 = {
    SPRITE_FRAME(gSpriteBank39Frame001, SPRITE_TILES_BANK39_FRAME001),
    { { -23, -11, 47, 23 } },
};
const struct sprite_frame_1box gSpriteBank39Frame002 = {
    SPRITE_FRAME(gSpriteBank39Frame002, SPRITE_TILES_BANK39_FRAME002),
    { { -39, -11, 79, 23 } },
};
const struct sprite_frame_1box gSpriteBank39Frame003 = {
    SPRITE_FRAME(gSpriteBank39Frame003, SPRITE_TILES_BANK39_FRAME003),
    { { -20, -14, 41, 28 } },
};
const struct sprite_frame_1box gSpriteBank39Frame004 = {
    SPRITE_FRAME(gSpriteBank39Frame004, SPRITE_TILES_BANK39_FRAME004),
    { { -21, -12, 42, 25 } },
};
const struct sprite_frame gSpriteBank39Frame005 = SPRITE_FRAME(gSpriteBank39Frame005, SPRITE_TILES_BANK39_FRAME005);
const struct sprite_frame_1box gSpriteBank39Frame006 = {
    SPRITE_FRAME(gSpriteBank39Frame006, SPRITE_TILES_BANK39_FRAME006),
    { { -20, -7, 40, 15 } },
};
const struct sprite_frame_1box gSpriteBank39Frame007 = {
    SPRITE_FRAME(gSpriteBank39Frame007, SPRITE_TILES_BANK39_FRAME007),
    { { -20, -7, 40, 15 } },
};
const struct sprite_frame_1box gSpriteBank39Frame008 = {
    SPRITE_FRAME(gSpriteBank39Frame008, SPRITE_TILES_BANK39_FRAME008),
    { { -20, -7, 40, 17 } },
};
const struct sprite_frame_1box gSpriteBank39Frame009 = {
    SPRITE_FRAME(gSpriteBank39Frame009, SPRITE_TILES_BANK39_FRAME009),
    { { -18, -9, 36, 22 } },
};
const struct sprite_frame_1box gSpriteBank39Frame010 = {
    SPRITE_FRAME(gSpriteBank39Frame010, SPRITE_TILES_BANK39_FRAME010),
    { { -15, -12, 30, 29 } },
};
const struct sprite_frame_1box gSpriteBank39Frame011 = {
    SPRITE_FRAME(gSpriteBank39Frame011, SPRITE_TILES_BANK39_FRAME011),
    { { -12, -15, 23, 35 } },
};
const struct sprite_frame_1box gSpriteBank39Frame012 = {
    SPRITE_FRAME(gSpriteBank39Frame012, SPRITE_TILES_BANK39_FRAME012),
    { { -8, -16, 15, 38 } },
};
const struct sprite_frame_1box gSpriteBank39Frame013 = {
    SPRITE_FRAME(gSpriteBank39Frame013, SPRITE_TILES_BANK39_FRAME013),
    { { -3, -16, 10, 39 } },
};
const struct sprite_frame_1box gSpriteBank39Frame014 = {
    SPRITE_FRAME(gSpriteBank39Frame014, SPRITE_TILES_BANK39_FRAME014),
    { { -18, -9, 36, 22 } },
};
const struct sprite_frame_1box gSpriteBank39Frame015 = {
    SPRITE_FRAME(gSpriteBank39Frame015, SPRITE_TILES_BANK39_FRAME015),
    { { -20, -7, 40, 17 } },
};
const struct sprite_frame gSpriteBank39Frame016 = SPRITE_FRAME(gSpriteBank39Frame016, SPRITE_TILES_BANK39_FRAME016);
const struct sprite_frame_1box gSpriteBank39Frame017 = {
    SPRITE_FRAME(gSpriteBank39Frame017, SPRITE_TILES_BANK39_FRAME017),
    { { -36, -10, 72, 21 } },
};
const struct sprite_frame_1box gSpriteBank39Frame018 = {
    SPRITE_FRAME(gSpriteBank39Frame018, SPRITE_TILES_BANK39_FRAME018),
    { { -43, -11, 77, 26 } },
};
const struct sprite_frame_1box gSpriteBank39Frame019 = {
    SPRITE_FRAME(gSpriteBank39Frame019, SPRITE_TILES_BANK39_FRAME019),
    { { -63, -17, 119, 37 } },
};
const struct sprite_frame_1box gSpriteBank39Frame020 = {
    SPRITE_FRAME(gSpriteBank39Frame020, SPRITE_TILES_BANK39_FRAME020),
    { { -62, -37, 123, 57 } },
};
const struct sprite_frame_1box gSpriteBank39Frame021 = {
    SPRITE_FRAME(gSpriteBank39Frame021, SPRITE_TILES_BANK39_FRAME021),
    { { -65, -38, 126, 59 } },
};
const struct sprite_frame_1box gSpriteBank39Frame022 = {
    SPRITE_FRAME(gSpriteBank39Frame022, SPRITE_TILES_BANK39_FRAME022),
    { { -65, -38, 127, 59 } },
};
const struct sprite_frame_1box gSpriteBank39Frame023 = {
    SPRITE_FRAME(gSpriteBank39Frame023, SPRITE_TILES_BANK39_FRAME023),
    { { -20, -14, 42, 28 } },
};

/* The piece arrays, from graphics/sprites/bank39_platforms/bank.json. */
#include "sprites/bank39_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 40: 5 animations, 30 frames, graphics/sprites/bank40_bubbles/. */

extern const u16 gSpriteBank40Anim00Seq[3];
extern const u16 gSpriteBank40Anim01Seq[8];
extern const u16 gSpriteBank40Anim02Seq[8];
extern const u16 gSpriteBank40Anim03Seq[5];
extern const u16 gSpriteBank40Anim04Seq[6];

extern const struct sprite_anim gSpriteBank40Anims[5] = {
    /* 0 */ {
        /* seq */ gSpriteBank40Anim00Seq,
        /* box */ { { -7, -7, 15, 15 }, { -7, -7, 14, 15 } },
        /* paletteId */ 57,
        /* duration */ 29,
        /* frameCount */ ARRAY_COUNT(gSpriteBank40Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank40Anim01Seq,
        /* box */ { { -31, -15, 63, 31 }, { -31, -14, 63, 30 } },
        /* paletteId */ 57,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank40Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank40Anim02Seq,
        /* box */ { { -10, -7, 21, 15 }, { -13, -21, 24, 29 } },
        /* paletteId */ 57,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank40Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank40Anim03Seq,
        /* box */ { { -4, -5, 9, 10 }, { -12, -16, 27, 30 } },
        /* paletteId */ 57,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank40Anim03Seq),
        /* flags */ 0,
    },
    /* 4 */ {
        /* seq */ gSpriteBank40Anim04Seq,
        /* box */ { { -6, -6, 12, 13 }, { -15, -23, 23, 30 } },
        /* paletteId */ 57,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank40Anim04Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank40Anim00Seq[3] = {
    0, 1, 2,
};
const u16 gSpriteBank40Anim01Seq[8] = {
    3, 4, 5, 6, 7, 8, 9, 10,
};
const u16 gSpriteBank40Anim02Seq[8] = {
    11, 12, 13, 14, 15, 16, 17, 18,
};
const u16 gSpriteBank40Anim03Seq[5] = {
    19, 20, 21, 22, 23,
};
const u16 gSpriteBank40Anim04Seq[6] = {
    24, 25, 26, 27, 28, 29,
};

extern const struct sprite_frame *const gSpriteBank40Frames[30] = {
    &gSpriteBank40Frame000.frame,
    &gSpriteBank40Frame001.frame,
    &gSpriteBank40Frame002.frame,
    &gSpriteBank40Frame003.frame,
    &gSpriteBank40Frame004.frame,
    &gSpriteBank40Frame005.frame,
    &gSpriteBank40Frame006.frame,
    &gSpriteBank40Frame007.frame,
    &gSpriteBank40Frame008.frame,
    &gSpriteBank40Frame009.frame,
    &gSpriteBank40Frame010.frame,
    &gSpriteBank40Frame011.frame,
    &gSpriteBank40Frame012.frame,
    &gSpriteBank40Frame013.frame,
    &gSpriteBank40Frame014.frame,
    &gSpriteBank40Frame015.frame,
    &gSpriteBank40Frame016.frame,
    &gSpriteBank40Frame017.frame,
    &gSpriteBank40Frame018.frame,
    &gSpriteBank40Frame019.frame,
    &gSpriteBank40Frame020.frame,
    &gSpriteBank40Frame021.frame,
    &gSpriteBank40Frame022.frame,
    &gSpriteBank40Frame023.frame,
    &gSpriteBank40Frame024.frame,
    &gSpriteBank40Frame025.frame,
    &gSpriteBank40Frame026.frame,
    &gSpriteBank40Frame027.frame,
    &gSpriteBank40Frame028.frame,
    &gSpriteBank40Frame029.frame,
};

const struct sprite_frame_1box gSpriteBank40Frame000 = {
    SPRITE_FRAME(gSpriteBank40Frame000, SPRITE_TILES_BANK40_FRAME000),
    { { -7, -7, 15, 15 } },
};
const struct sprite_frame_1box gSpriteBank40Frame001 = {
    SPRITE_FRAME(gSpriteBank40Frame001, SPRITE_TILES_BANK40_FRAME001),
    { { -6, -5, 13, 12 } },
};
const struct sprite_frame_1box gSpriteBank40Frame002 = {
    SPRITE_FRAME(gSpriteBank40Frame002, SPRITE_TILES_BANK40_FRAME002),
    { { -7, -6, 14, 14 } },
};
const struct sprite_frame_1box gSpriteBank40Frame003 = {
    SPRITE_FRAME(gSpriteBank40Frame003, SPRITE_TILES_BANK40_FRAME003),
    { { -31, -15, 63, 31 } },
};
const struct sprite_frame_1box gSpriteBank40Frame004 = {
    SPRITE_FRAME(gSpriteBank40Frame004, SPRITE_TILES_BANK40_FRAME004),
    { { -31, -15, 63, 31 } },
};
const struct sprite_frame_1box gSpriteBank40Frame005 = {
    SPRITE_FRAME(gSpriteBank40Frame005, SPRITE_TILES_BANK40_FRAME005),
    { { -31, -15, 63, 31 } },
};
const struct sprite_frame_1box gSpriteBank40Frame006 = {
    SPRITE_FRAME(gSpriteBank40Frame006, SPRITE_TILES_BANK40_FRAME006),
    { { -31, -15, 63, 31 } },
};
const struct sprite_frame_1box gSpriteBank40Frame007 = {
    SPRITE_FRAME(gSpriteBank40Frame007, SPRITE_TILES_BANK40_FRAME007),
    { { -31, -15, 63, 31 } },
};
const struct sprite_frame_1box gSpriteBank40Frame008 = {
    SPRITE_FRAME(gSpriteBank40Frame008, SPRITE_TILES_BANK40_FRAME008),
    { { -31, -15, 63, 31 } },
};
const struct sprite_frame_1box gSpriteBank40Frame009 = {
    SPRITE_FRAME(gSpriteBank40Frame009, SPRITE_TILES_BANK40_FRAME009),
    { { -31, -15, 63, 31 } },
};
const struct sprite_frame_1box gSpriteBank40Frame010 = {
    SPRITE_FRAME(gSpriteBank40Frame010, SPRITE_TILES_BANK40_FRAME010),
    { { -31, -15, 63, 31 } },
};
const struct sprite_frame_1box gSpriteBank40Frame011 = {
    SPRITE_FRAME(gSpriteBank40Frame011, SPRITE_TILES_BANK40_FRAME011),
    { { -10, -7, 21, 15 } },
};
const struct sprite_frame_1box gSpriteBank40Frame012 = {
    SPRITE_FRAME(gSpriteBank40Frame012, SPRITE_TILES_BANK40_FRAME012),
    { { -11, -8, 22, 15 } },
};
const struct sprite_frame_1box gSpriteBank40Frame013 = {
    SPRITE_FRAME(gSpriteBank40Frame013, SPRITE_TILES_BANK40_FRAME013),
    { { -11, -10, 21, 13 } },
};
const struct sprite_frame_1box gSpriteBank40Frame014 = {
    SPRITE_FRAME(gSpriteBank40Frame014, SPRITE_TILES_BANK40_FRAME014),
    { { -13, -14, 23, 19 } },
};
const struct sprite_frame_1box gSpriteBank40Frame015 = {
    SPRITE_FRAME(gSpriteBank40Frame015, SPRITE_TILES_BANK40_FRAME015),
    { { -13, -19, 24, 24 } },
};
const struct sprite_frame_1box gSpriteBank40Frame016 = {
    SPRITE_FRAME(gSpriteBank40Frame016, SPRITE_TILES_BANK40_FRAME016),
    { { -13, -20, 24, 22 } },
};
const struct sprite_frame_1box gSpriteBank40Frame017 = {
    SPRITE_FRAME(gSpriteBank40Frame017, SPRITE_TILES_BANK40_FRAME017),
    { { -4, -21, 14, 23 } },
};
const struct sprite_frame_1box gSpriteBank40Frame018 = {
    SPRITE_FRAME(gSpriteBank40Frame018, SPRITE_TILES_BANK40_FRAME018),
    { { 4, -21, 7, 3 } },
};
const struct sprite_frame_1box gSpriteBank40Frame019 = {
    SPRITE_FRAME(gSpriteBank40Frame019, SPRITE_TILES_BANK40_FRAME019),
    { { -4, -5, 9, 10 } },
};
const struct sprite_frame_1box gSpriteBank40Frame020 = {
    SPRITE_FRAME(gSpriteBank40Frame020, SPRITE_TILES_BANK40_FRAME020),
    { { -9, -10, 17, 19 } },
};
const struct sprite_frame_1box gSpriteBank40Frame021 = {
    SPRITE_FRAME(gSpriteBank40Frame021, SPRITE_TILES_BANK40_FRAME021),
    { { -12, -13, 25, 27 } },
};
const struct sprite_frame_1box gSpriteBank40Frame022 = {
    SPRITE_FRAME(gSpriteBank40Frame022, SPRITE_TILES_BANK40_FRAME022),
    { { -11, -15, 26, 27 } },
};
const struct sprite_frame_1box gSpriteBank40Frame023 = {
    SPRITE_FRAME(gSpriteBank40Frame023, SPRITE_TILES_BANK40_FRAME023),
    { { -8, -16, 23, 28 } },
};
const struct sprite_frame_1box gSpriteBank40Frame024 = {
    SPRITE_FRAME(gSpriteBank40Frame024, SPRITE_TILES_BANK40_FRAME024),
    { { -6, -6, 12, 13 } },
};
const struct sprite_frame_1box gSpriteBank40Frame025 = {
    SPRITE_FRAME(gSpriteBank40Frame025, SPRITE_TILES_BANK40_FRAME025),
    { { -12, -9, 17, 15 } },
};
const struct sprite_frame_1box gSpriteBank40Frame026 = {
    SPRITE_FRAME(gSpriteBank40Frame026, SPRITE_TILES_BANK40_FRAME026),
    { { -15, -17, 19, 24 } },
};
const struct sprite_frame_1box gSpriteBank40Frame027 = {
    SPRITE_FRAME(gSpriteBank40Frame027, SPRITE_TILES_BANK40_FRAME027),
    { { -15, -22, 21, 24 } },
};
const struct sprite_frame_1box gSpriteBank40Frame028 = {
    SPRITE_FRAME(gSpriteBank40Frame028, SPRITE_TILES_BANK40_FRAME028),
    { { -12, -23, 20, 20 } },
};
const struct sprite_frame_1box gSpriteBank40Frame029 = {
    SPRITE_FRAME(gSpriteBank40Frame029, SPRITE_TILES_BANK40_FRAME029),
    { { -13, -23, 17, 16 } },
};

/* The piece arrays, from graphics/sprites/bank40_bubbles/bank.json. */
#include "sprites/bank40_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 41: 3 animations, 24 frames, graphics/sprites/bank41_spark/. */

extern const u16 gSpriteBank41Anim00Seq[6];
extern const u16 gSpriteBank41Anim01Seq[10];
extern const u16 gSpriteBank41Anim02Seq[8];

extern const struct sprite_anim gSpriteBank41Anims[3] = {
    /* 0 */ {
        /* seq */ gSpriteBank41Anim00Seq,
        /* box */ { { -2, -1, 5, 3 }, { -3, -4, 7, 6 } },
        /* paletteId */ 47,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank41Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank41Anim01Seq,
        /* box */ { { -4, -1, 8, 2 }, { -20, -23, 25, 24 } },
        /* paletteId */ 47,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank41Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank41Anim02Seq,
        /* box */ { { -9, -8, 19, 16 }, { -11, -17, 24, 25 } },
        /* paletteId */ 47,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank41Anim02Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank41Anim00Seq[6] = {
    0, 1, 2, 3, 4, 5,
};
const u16 gSpriteBank41Anim01Seq[10] = {
    6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
};
const u16 gSpriteBank41Anim02Seq[8] = {
    16, 17, 18, 19, 20, 21, 22, 23,
};

extern const struct sprite_frame *const gSpriteBank41Frames[24] = {
    &gSpriteBank41Frame000.frame,
    &gSpriteBank41Frame001.frame,
    &gSpriteBank41Frame002.frame,
    &gSpriteBank41Frame003.frame,
    &gSpriteBank41Frame004.frame,
    &gSpriteBank41Frame005.frame,
    &gSpriteBank41Frame006.frame,
    &gSpriteBank41Frame007.frame,
    &gSpriteBank41Frame008.frame,
    &gSpriteBank41Frame009.frame,
    &gSpriteBank41Frame010.frame,
    &gSpriteBank41Frame011.frame,
    &gSpriteBank41Frame012.frame,
    &gSpriteBank41Frame013.frame,
    &gSpriteBank41Frame014.frame,
    &gSpriteBank41Frame015.frame,
    &gSpriteBank41Frame016.frame,
    &gSpriteBank41Frame017.frame,
    &gSpriteBank41Frame018.frame,
    &gSpriteBank41Frame019.frame,
    &gSpriteBank41Frame020.frame,
    &gSpriteBank41Frame021.frame,
    &gSpriteBank41Frame022.frame,
    &gSpriteBank41Frame023.frame,
};

const struct sprite_frame_1box gSpriteBank41Frame000 = {
    SPRITE_FRAME(gSpriteBank41Frame000, SPRITE_TILES_BANK41_FRAME000),
    { { -2, -1, 5, 3 } },
};
const struct sprite_frame_1box gSpriteBank41Frame001 = {
    SPRITE_FRAME(gSpriteBank41Frame001, SPRITE_TILES_BANK41_FRAME001),
    { { -3, -4, 7, 5 } },
};
const struct sprite_frame_1box gSpriteBank41Frame002 = {
    SPRITE_FRAME(gSpriteBank41Frame002, SPRITE_TILES_BANK41_FRAME002),
    { { -2, -4, 5, 5 } },
};
const struct sprite_frame_1box gSpriteBank41Frame003 = {
    SPRITE_FRAME(gSpriteBank41Frame003, SPRITE_TILES_BANK41_FRAME003),
    { { -1, -4, 3, 4 } },
};
const struct sprite_frame_1box gSpriteBank41Frame004 = {
    SPRITE_FRAME(gSpriteBank41Frame004, SPRITE_TILES_BANK41_FRAME004),
    { { -1, -4, 3, 3 } },
};
const struct sprite_frame_1box gSpriteBank41Frame005 = {
    SPRITE_FRAME(gSpriteBank41Frame005, SPRITE_TILES_BANK41_FRAME005),
    { { -1, -4, 1, 1 } },
};
const struct sprite_frame_1box gSpriteBank41Frame006 = {
    SPRITE_FRAME(gSpriteBank41Frame006, SPRITE_TILES_BANK41_FRAME006),
    { { -4, -1, 8, 2 } },
};
const struct sprite_frame_1box gSpriteBank41Frame007 = {
    SPRITE_FRAME(gSpriteBank41Frame007, SPRITE_TILES_BANK41_FRAME007),
    { { -10, -2, 15, 3 } },
};
const struct sprite_frame_1box gSpriteBank41Frame008 = {
    SPRITE_FRAME(gSpriteBank41Frame008, SPRITE_TILES_BANK41_FRAME008),
    { { -15, -6, 20, 7 } },
};
const struct sprite_frame_1box gSpriteBank41Frame009 = {
    SPRITE_FRAME(gSpriteBank41Frame009, SPRITE_TILES_BANK41_FRAME009),
    { { -17, -11, 18, 12 } },
};
const struct sprite_frame_1box gSpriteBank41Frame010 = {
    SPRITE_FRAME(gSpriteBank41Frame010, SPRITE_TILES_BANK41_FRAME010),
    { { -20, -16, 17, 17 } },
};
const struct sprite_frame_1box gSpriteBank41Frame011 = {
    SPRITE_FRAME(gSpriteBank41Frame011, SPRITE_TILES_BANK41_FRAME011),
    { { -20, -17, 13, 16 } },
};
const struct sprite_frame_1box gSpriteBank41Frame012 = {
    SPRITE_FRAME(gSpriteBank41Frame012, SPRITE_TILES_BANK41_FRAME012),
    { { -20, -18, 12, 15 } },
};
const struct sprite_frame_1box gSpriteBank41Frame013 = {
    SPRITE_FRAME(gSpriteBank41Frame013, SPRITE_TILES_BANK41_FRAME013),
    { { -19, -22, 9, 15 } },
};
const struct sprite_frame_1box gSpriteBank41Frame014 = {
    SPRITE_FRAME(gSpriteBank41Frame014, SPRITE_TILES_BANK41_FRAME014),
    { { -18, -23, 6, 9 } },
};
const struct sprite_frame_1box gSpriteBank41Frame015 = {
    SPRITE_FRAME(gSpriteBank41Frame015, SPRITE_TILES_BANK41_FRAME015),
    { { -17, -22, 5, 5 } },
};
const struct sprite_frame_1box gSpriteBank41Frame016 = {
    SPRITE_FRAME(gSpriteBank41Frame016, SPRITE_TILES_BANK41_FRAME016),
    { { -9, -8, 19, 16 } },
};
const struct sprite_frame_1box gSpriteBank41Frame017 = {
    SPRITE_FRAME(gSpriteBank41Frame017, SPRITE_TILES_BANK41_FRAME017),
    { { -11, -12, 24, 20 } },
};
const struct sprite_frame_1box gSpriteBank41Frame018 = {
    SPRITE_FRAME(gSpriteBank41Frame018, SPRITE_TILES_BANK41_FRAME018),
    { { -10, -12, 22, 19 } },
};
const struct sprite_frame_1box gSpriteBank41Frame019 = {
    SPRITE_FRAME(gSpriteBank41Frame019, SPRITE_TILES_BANK41_FRAME019),
    { { -9, -12, 21, 16 } },
};
const struct sprite_frame_1box gSpriteBank41Frame020 = {
    SPRITE_FRAME(gSpriteBank41Frame020, SPRITE_TILES_BANK41_FRAME020),
    { { -8, -12, 18, 14 } },
};
const struct sprite_frame_1box gSpriteBank41Frame021 = {
    SPRITE_FRAME(gSpriteBank41Frame021, SPRITE_TILES_BANK41_FRAME021),
    { { -6, -14, 16, 11 } },
};
const struct sprite_frame_1box gSpriteBank41Frame022 = {
    SPRITE_FRAME(gSpriteBank41Frame022, SPRITE_TILES_BANK41_FRAME022),
    { { -6, -14, 16, 6 } },
};
const struct sprite_frame_1box gSpriteBank41Frame023 = {
    SPRITE_FRAME(gSpriteBank41Frame023, SPRITE_TILES_BANK41_FRAME023),
    { { -3, -17, 10, 5 } },
};

/* The piece arrays, from graphics/sprites/bank41_spark/bank.json. */
#include "sprites/bank41_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 42: 1 animation, 11 frames, graphics/sprites/bank42_checkpoint_text/. */

extern const u16 gSpriteBank42Anim00Seq[24];

extern const struct sprite_anim gSpriteBank42Anims[1] = {
    /* 0 */ {
        /* seq */ gSpriteBank42Anim00Seq,
        /* box */ { { -3, -3, 7, 7 }, { -3, -3, 39, 7 } },
        /* paletteId */ 33,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank42Anim00Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank42Anim00Seq[24] = {
    0, 0, 1, 2, 3, 4, 4, 4, 3, 2, 1, 0, 5, 6, 7, 8,
    9, 10, 10, 10, 9, 8, 7, 6,
};

extern const struct sprite_frame *const gSpriteBank42Frames[11] = {
    &gSpriteBank42Frame000.frame,
    &gSpriteBank42Frame001.frame,
    &gSpriteBank42Frame002.frame,
    &gSpriteBank42Frame003.frame,
    &gSpriteBank42Frame004.frame,
    &gSpriteBank42Frame005,
    &gSpriteBank42Frame006.frame,
    &gSpriteBank42Frame007.frame,
    &gSpriteBank42Frame008.frame,
    &gSpriteBank42Frame009.frame,
    &gSpriteBank42Frame010.frame,
};

const struct sprite_frame_1box gSpriteBank42Frame000 = {
    SPRITE_FRAME(gSpriteBank42Frame000, SPRITE_TILES_BANK42_FRAME000),
    { { -3, -3, 7, 7 } },
};
const struct sprite_frame_1box gSpriteBank42Frame001 = {
    SPRITE_FRAME(gSpriteBank42Frame001, SPRITE_TILES_BANK42_FRAME001),
    { { -3, -3, 15, 7 } },
};
const struct sprite_frame_1box gSpriteBank42Frame002 = {
    SPRITE_FRAME(gSpriteBank42Frame002, SPRITE_TILES_BANK42_FRAME002),
    { { -3, -3, 23, 7 } },
};
const struct sprite_frame_1box gSpriteBank42Frame003 = {
    SPRITE_FRAME(gSpriteBank42Frame003, SPRITE_TILES_BANK42_FRAME003),
    { { -3, -3, 31, 7 } },
};
const struct sprite_frame_1box gSpriteBank42Frame004 = {
    SPRITE_FRAME(gSpriteBank42Frame004, SPRITE_TILES_BANK42_FRAME004),
    { { -3, -3, 39, 7 } },
};
const struct sprite_frame gSpriteBank42Frame005 = { gSpriteBank42Frame006Pos, gSpriteBank42Frame006Pieces, SPRITE_FRAME_TILES(SPRITE_TILES_BANK42_FRAME005, 0) };
const struct sprite_frame_1box gSpriteBank42Frame006 = {
    SPRITE_FRAME(gSpriteBank42Frame006, SPRITE_TILES_BANK42_FRAME006),
    { { -3, -3, 7, 7 } },
};
const struct sprite_frame_1box gSpriteBank42Frame007 = {
    SPRITE_FRAME(gSpriteBank42Frame007, SPRITE_TILES_BANK42_FRAME007),
    { { -3, -3, 15, 7 } },
};
const struct sprite_frame_1box gSpriteBank42Frame008 = {
    SPRITE_FRAME(gSpriteBank42Frame008, SPRITE_TILES_BANK42_FRAME008),
    { { -3, -3, 21, 7 } },
};
const struct sprite_frame_1box gSpriteBank42Frame009 = {
    SPRITE_FRAME(gSpriteBank42Frame009, SPRITE_TILES_BANK42_FRAME009),
    { { -3, -3, 30, 7 } },
};
const struct sprite_frame_1box gSpriteBank42Frame010 = {
    SPRITE_FRAME(gSpriteBank42Frame010, SPRITE_TILES_BANK42_FRAME010),
    { { -3, -3, 38, 7 } },
};

/* The piece arrays, from graphics/sprites/bank42_checkpoint_text/bank.json. */
#include "sprites/bank42_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 43: 7 animations, 21 frames, graphics/sprites/bank43_sparkle/. */

extern const u16 gSpriteBank43Anim00Seq[10];
extern const u16 gSpriteBank43Anim01Seq[10];
extern const u16 gSpriteBank43Anim02Seq[10];
extern const u16 gSpriteBank43Anim03Seq[10];
extern const u16 gSpriteBank43Anim04Seq[10];
extern const u16 gSpriteBank43Anim05Seq[10];
extern const u16 gSpriteBank43Anim06Seq[10];

extern const struct sprite_anim gSpriteBank43Anims[7] = {
    /* 0 */ {
        /* seq */ gSpriteBank43Anim00Seq,
        /* box */ { { -11, -11, 22, 23 }, { -25, -26, 49, 57 } },
        /* paletteId */ 49,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank43Anim01Seq,
        /* box */ { { -11, -11, 22, 23 }, { -25, -26, 49, 57 } },
        /* paletteId */ 50,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank43Anim02Seq,
        /* box */ { { -21, -23, 42, 47 }, { -25, -28, 49, 57 } },
        /* paletteId */ 50,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank43Anim03Seq,
        /* box */ { { -11, -11, 22, 23 }, { -25, -26, 49, 57 } },
        /* paletteId */ 51,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim03Seq),
        /* flags */ 0,
    },
    /* 4 */ {
        /* seq */ gSpriteBank43Anim04Seq,
        /* box */ { { -11, -11, 22, 23 }, { -25, -26, 49, 57 } },
        /* paletteId */ 117,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim04Seq),
        /* flags */ 0,
    },
    /* 5 */ {
        /* seq */ gSpriteBank43Anim05Seq,
        /* box */ { { -11, -11, 22, 23 }, { -25, -26, 49, 57 } },
        /* paletteId */ 52,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim05Seq),
        /* flags */ 0,
    },
    /* 6 */ {
        /* seq */ gSpriteBank43Anim06Seq,
        /* box */ { { -11, -11, 22, 23 }, { -25, -26, 49, 57 } },
        /* paletteId */ 53,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim06Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank43Anim00Seq[10] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
};
const u16 gSpriteBank43Anim01Seq[10] = {
    0, 1, 2, 3, 10, 5, 6, 7, 8, 9,
};
const u16 gSpriteBank43Anim02Seq[10] = {
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
};
const u16 gSpriteBank43Anim03Seq[10] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
};
const u16 gSpriteBank43Anim04Seq[10] = {
    0, 1, 2, 3, 10, 5, 6, 7, 8, 9,
};
const u16 gSpriteBank43Anim05Seq[10] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
};
const u16 gSpriteBank43Anim06Seq[10] = {
    0, 1, 2, 3, 10, 5, 6, 7, 8, 9,
};

extern const struct sprite_frame *const gSpriteBank43Frames[21] = {
    &gSpriteBank43Frame000.frame,
    &gSpriteBank43Frame001.frame,
    &gSpriteBank43Frame002.frame,
    &gSpriteBank43Frame003.frame,
    &gSpriteBank43Frame004.frame,
    &gSpriteBank43Frame005.frame,
    &gSpriteBank43Frame006.frame,
    &gSpriteBank43Frame007.frame,
    &gSpriteBank43Frame008.frame,
    &gSpriteBank43Frame009.frame,
    &gSpriteBank43Frame010.frame,
    &gSpriteBank43Frame011.frame,
    &gSpriteBank43Frame012.frame,
    &gSpriteBank43Frame013.frame,
    &gSpriteBank43Frame014.frame,
    &gSpriteBank43Frame015.frame,
    &gSpriteBank43Frame016.frame,
    &gSpriteBank43Frame017.frame,
    &gSpriteBank43Frame018.frame,
    &gSpriteBank43Frame019.frame,
    &gSpriteBank43Frame020.frame,
};

const struct sprite_frame_1box gSpriteBank43Frame000 = {
    SPRITE_FRAME(gSpriteBank43Frame000, SPRITE_TILES_BANK43_FRAME000),
    { { -11, -11, 22, 23 } },
};
const struct sprite_frame_1box gSpriteBank43Frame001 = {
    SPRITE_FRAME(gSpriteBank43Frame001, SPRITE_TILES_BANK43_FRAME001),
    { { -17, -18, 38, 39 } },
};
const struct sprite_frame_1box gSpriteBank43Frame002 = {
    SPRITE_FRAME(gSpriteBank43Frame002, SPRITE_TILES_BANK43_FRAME002),
    { { -20, -19, 40, 43 } },
};
const struct sprite_frame_1box gSpriteBank43Frame003 = {
    SPRITE_FRAME(gSpriteBank43Frame003, SPRITE_TILES_BANK43_FRAME003),
    { { -25, -25, 49, 55 } },
};
const struct sprite_frame_1box gSpriteBank43Frame004 = {
    SPRITE_FRAME(gSpriteBank43Frame004, SPRITE_TILES_BANK43_FRAME004),
    { { -25, -26, 49, 57 } },
};
const struct sprite_frame_1box gSpriteBank43Frame005 = {
    SPRITE_FRAME(gSpriteBank43Frame005, SPRITE_TILES_BANK43_FRAME005),
    { { -25, -25, 49, 55 } },
};
const struct sprite_frame_1box gSpriteBank43Frame006 = {
    SPRITE_FRAME(gSpriteBank43Frame006, SPRITE_TILES_BANK43_FRAME006),
    { { -24, -24, 47, 54 } },
};
const struct sprite_frame_1box gSpriteBank43Frame007 = {
    SPRITE_FRAME(gSpriteBank43Frame007, SPRITE_TILES_BANK43_FRAME007),
    { { -23, -24, 46, 53 } },
};
const struct sprite_frame_1box gSpriteBank43Frame008 = {
    SPRITE_FRAME(gSpriteBank43Frame008, SPRITE_TILES_BANK43_FRAME008),
    { { -22, -22, 44, 49 } },
};
const struct sprite_frame_1box gSpriteBank43Frame009 = {
    SPRITE_FRAME(gSpriteBank43Frame009, SPRITE_TILES_BANK43_FRAME009),
    { { -21, -21, 42, 47 } },
};
const struct sprite_frame_1box gSpriteBank43Frame010 = {
    SPRITE_FRAME(gSpriteBank43Frame010, SPRITE_TILES_BANK43_FRAME010),
    { { -25, -26, 49, 57 } },
};
const struct sprite_frame_1box gSpriteBank43Frame011 = {
    SPRITE_FRAME(gSpriteBank43Frame011, SPRITE_TILES_BANK43_FRAME011),
    { { -21, -23, 42, 47 } },
};
const struct sprite_frame_1box gSpriteBank43Frame012 = {
    SPRITE_FRAME(gSpriteBank43Frame012, SPRITE_TILES_BANK43_FRAME012),
    { { -22, -24, 44, 49 } },
};
const struct sprite_frame_1box gSpriteBank43Frame013 = {
    SPRITE_FRAME(gSpriteBank43Frame013, SPRITE_TILES_BANK43_FRAME013),
    { { -23, -26, 46, 53 } },
};
const struct sprite_frame_1box gSpriteBank43Frame014 = {
    SPRITE_FRAME(gSpriteBank43Frame014, SPRITE_TILES_BANK43_FRAME014),
    { { -24, -26, 47, 54 } },
};
const struct sprite_frame_1box gSpriteBank43Frame015 = {
    SPRITE_FRAME(gSpriteBank43Frame015, SPRITE_TILES_BANK43_FRAME015),
    { { -25, -27, 49, 55 } },
};
const struct sprite_frame_1box gSpriteBank43Frame016 = {
    SPRITE_FRAME(gSpriteBank43Frame016, SPRITE_TILES_BANK43_FRAME016),
    { { -25, -28, 49, 57 } },
};
const struct sprite_frame_1box gSpriteBank43Frame017 = {
    SPRITE_FRAME(gSpriteBank43Frame017, SPRITE_TILES_BANK43_FRAME017),
    { { -25, -27, 49, 55 } },
};
const struct sprite_frame_1box gSpriteBank43Frame018 = {
    SPRITE_FRAME(gSpriteBank43Frame018, SPRITE_TILES_BANK43_FRAME018),
    { { -20, -21, 40, 43 } },
};
const struct sprite_frame_1box gSpriteBank43Frame019 = {
    SPRITE_FRAME(gSpriteBank43Frame019, SPRITE_TILES_BANK43_FRAME019),
    { { -17, -20, 38, 39 } },
};
const struct sprite_frame_1box gSpriteBank43Frame020 = {
    SPRITE_FRAME(gSpriteBank43Frame020, SPRITE_TILES_BANK43_FRAME020),
    { { -11, -13, 22, 23 } },
};

/* The piece arrays, from graphics/sprites/bank43_sparkle/bank.json. */
#include "sprites/bank43_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 44: 1 animation, 13 frames, graphics/sprites/bank44_flame/. */

extern const u16 gSpriteBank44Anim00Seq[13];

extern const struct sprite_anim gSpriteBank44Anims[1] = {
    /* 0 */ {
        /* seq */ gSpriteBank44Anim00Seq,
        /* box */ { { -3, -7, 7, 14 }, { -5, -11, 12, 19 } },
        /* paletteId */ 33,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank44Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank44Anim00Seq[13] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
};

extern const struct sprite_frame *const gSpriteBank44Frames[13] = {
    &gSpriteBank44Frame000.frame,
    &gSpriteBank44Frame001.frame,
    &gSpriteBank44Frame002.frame,
    &gSpriteBank44Frame003.frame,
    &gSpriteBank44Frame004.frame,
    &gSpriteBank44Frame005.frame,
    &gSpriteBank44Frame006.frame,
    &gSpriteBank44Frame007.frame,
    &gSpriteBank44Frame008.frame,
    &gSpriteBank44Frame009.frame,
    &gSpriteBank44Frame010.frame,
    &gSpriteBank44Frame011.frame,
    &gSpriteBank44Frame012.frame,
};

const struct sprite_frame_1box gSpriteBank44Frame000 = {
    SPRITE_FRAME(gSpriteBank44Frame000, SPRITE_TILES_BANK44_FRAME000),
    { { -3, -7, 7, 14 } },
};
const struct sprite_frame_1box gSpriteBank44Frame001 = {
    SPRITE_FRAME(gSpriteBank44Frame001, SPRITE_TILES_BANK44_FRAME001),
    { { -4, -8, 9, 16 } },
};
const struct sprite_frame_1box gSpriteBank44Frame002 = {
    SPRITE_FRAME(gSpriteBank44Frame002, SPRITE_TILES_BANK44_FRAME002),
    { { -5, -9, 9, 17 } },
};
const struct sprite_frame_1box gSpriteBank44Frame003 = {
    SPRITE_FRAME(gSpriteBank44Frame003, SPRITE_TILES_BANK44_FRAME003),
    { { -4, -5, 9, 11 } },
};
const struct sprite_frame_1box gSpriteBank44Frame004 = {
    SPRITE_FRAME(gSpriteBank44Frame004, SPRITE_TILES_BANK44_FRAME004),
    { { -5, -5, 9, 13 } },
};
const struct sprite_frame_1box gSpriteBank44Frame005 = {
    SPRITE_FRAME(gSpriteBank44Frame005, SPRITE_TILES_BANK44_FRAME005),
    { { -5, -9, 11, 17 } },
};
const struct sprite_frame_1box gSpriteBank44Frame006 = {
    SPRITE_FRAME(gSpriteBank44Frame006, SPRITE_TILES_BANK44_FRAME006),
    { { -3, -11, 10, 18 } },
};
const struct sprite_frame_1box gSpriteBank44Frame007 = {
    SPRITE_FRAME(gSpriteBank44Frame007, SPRITE_TILES_BANK44_FRAME007),
    { { -3, -9, 8, 15 } },
};
const struct sprite_frame_1box gSpriteBank44Frame008 = {
    SPRITE_FRAME(gSpriteBank44Frame008, SPRITE_TILES_BANK44_FRAME008),
    { { -4, -11, 9, 19 } },
};
const struct sprite_frame_1box gSpriteBank44Frame009 = {
    SPRITE_FRAME(gSpriteBank44Frame009, SPRITE_TILES_BANK44_FRAME009),
    { { -4, -11, 8, 18 } },
};
const struct sprite_frame_1box gSpriteBank44Frame010 = {
    SPRITE_FRAME(gSpriteBank44Frame010, SPRITE_TILES_BANK44_FRAME010),
    { { -4, -11, 7, 17 } },
};
const struct sprite_frame_1box gSpriteBank44Frame011 = {
    SPRITE_FRAME(gSpriteBank44Frame011, SPRITE_TILES_BANK44_FRAME011),
    { { -4, -6, 7, 13 } },
};
const struct sprite_frame_1box gSpriteBank44Frame012 = {
    SPRITE_FRAME(gSpriteBank44Frame012, SPRITE_TILES_BANK44_FRAME012),
    { { -4, -9, 8, 16 } },
};

/* The piece arrays, from graphics/sprites/bank44_flame/bank.json. */
#include "sprites/bank44_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 45: 1 animation, 13 frames, graphics/sprites/bank45_seaweed/. */

extern const u16 gSpriteBank45Anim00Seq[13];

extern const struct sprite_anim gSpriteBank45Anims[1] = {
    /* 0 */ {
        /* seq */ gSpriteBank45Anim00Seq,
        /* box */ { { -11, -31, 23, 62 }, { -11, -31, 24, 62 } },
        /* paletteId */ 123,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank45Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank45Anim00Seq[13] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
};

extern const struct sprite_frame *const gSpriteBank45Frames[13] = {
    &gSpriteBank45Frame000,
    &gSpriteBank45Frame001,
    &gSpriteBank45Frame002,
    &gSpriteBank45Frame003,
    &gSpriteBank45Frame004,
    &gSpriteBank45Frame005,
    &gSpriteBank45Frame006,
    &gSpriteBank45Frame007,
    &gSpriteBank45Frame008,
    &gSpriteBank45Frame009,
    &gSpriteBank45Frame010,
    &gSpriteBank45Frame011,
    &gSpriteBank45Frame012,
};

const struct sprite_frame gSpriteBank45Frame000 = SPRITE_FRAME(gSpriteBank45Frame000, SPRITE_TILES_BANK45_FRAME000);
const struct sprite_frame gSpriteBank45Frame001 = SPRITE_FRAME(gSpriteBank45Frame001, SPRITE_TILES_BANK45_FRAME001);
const struct sprite_frame gSpriteBank45Frame002 = SPRITE_FRAME(gSpriteBank45Frame002, SPRITE_TILES_BANK45_FRAME002);
const struct sprite_frame gSpriteBank45Frame003 = SPRITE_FRAME(gSpriteBank45Frame003, SPRITE_TILES_BANK45_FRAME003);
const struct sprite_frame gSpriteBank45Frame004 = SPRITE_FRAME(gSpriteBank45Frame004, SPRITE_TILES_BANK45_FRAME004);
const struct sprite_frame gSpriteBank45Frame005 = SPRITE_FRAME(gSpriteBank45Frame005, SPRITE_TILES_BANK45_FRAME005);
const struct sprite_frame gSpriteBank45Frame006 = SPRITE_FRAME(gSpriteBank45Frame006, SPRITE_TILES_BANK45_FRAME006);
const struct sprite_frame gSpriteBank45Frame007 = SPRITE_FRAME(gSpriteBank45Frame007, SPRITE_TILES_BANK45_FRAME007);
const struct sprite_frame gSpriteBank45Frame008 = SPRITE_FRAME(gSpriteBank45Frame008, SPRITE_TILES_BANK45_FRAME008);
const struct sprite_frame gSpriteBank45Frame009 = SPRITE_FRAME(gSpriteBank45Frame009, SPRITE_TILES_BANK45_FRAME009);
const struct sprite_frame gSpriteBank45Frame010 = SPRITE_FRAME(gSpriteBank45Frame010, SPRITE_TILES_BANK45_FRAME010);
const struct sprite_frame gSpriteBank45Frame011 = SPRITE_FRAME(gSpriteBank45Frame011, SPRITE_TILES_BANK45_FRAME011);
const struct sprite_frame gSpriteBank45Frame012 = SPRITE_FRAME(gSpriteBank45Frame012, SPRITE_TILES_BANK45_FRAME012);

/* The piece arrays, from graphics/sprites/bank45_seaweed/bank.json. */
#include "sprites/bank45_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 46: 1 animation, 2 frames, graphics/sprites/bank46_crash_eyes/. */

extern const u16 gSpriteBank46Anim00Seq[3];

extern const struct sprite_anim gSpriteBank46Anims[1] = {
    /* 0 */ {
        /* seq */ gSpriteBank46Anim00Seq,
        /* box */ { { -8, -4, 16, 9 }, { -8, -4, 16, 9 } },
        /* paletteId */ 124,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank46Anim00Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank46Anim00Seq[3] = {
    0, 1, 0,
};

extern const struct sprite_frame *const gSpriteBank46Frames[2] = {
    &gSpriteBank46Frame000.frame,
    &gSpriteBank46Frame001.frame,
};

const struct sprite_frame_1box gSpriteBank46Frame000 = {
    SPRITE_FRAME(gSpriteBank46Frame000, SPRITE_TILES_BANK46_FRAME000),
    { { -8, -4, 16, 9 } },
};
const struct sprite_frame_1box gSpriteBank46Frame001 = {
    SPRITE_FRAME(gSpriteBank46Frame001, SPRITE_TILES_BANK46_FRAME001),
    { { -8, -4, 16, 9 } },
};

/* The piece arrays, from graphics/sprites/bank46_crash_eyes/bank.json. */
#include "sprites/bank46_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 47: 13 animations, 43 frames, graphics/sprites/bank47_icons/. */

extern const u16 gSpriteBank47Anim00Seq[11];
extern const u16 gSpriteBank47Anim01Seq[12];
extern const u16 gSpriteBank47Anim02Seq[1];
extern const u16 gSpriteBank47Anim03Seq[1];
extern const u16 gSpriteBank47Anim04Seq[12];
extern const u16 gSpriteBank47Anim05Seq[5];
extern const u16 gSpriteBank47Anim06Seq[1];
extern const u16 gSpriteBank47Anim07Seq[1];
extern const u16 gSpriteBank47Anim08Seq[1];
extern const u16 gSpriteBank47Anim09Seq[1];
extern const u16 gSpriteBank47Anim10Seq[1];
extern const u16 gSpriteBank47Anim11Seq[32];
extern const u16 gSpriteBank47Anim12Seq[1];

extern const struct sprite_anim gSpriteBank47Anims[13] = {
    /* 0 */ {
        /* seq */ gSpriteBank47Anim00Seq,
        /* box */ { { -6, -11, 13, 23 }, { -7, -11, 15, 23 } },
        /* paletteId */ 33,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank47Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank47Anim01Seq,
        /* box */ { { -3, -5, 7, 11 }, { -3, -6, 7, 12 } },
        /* paletteId */ 33,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank47Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank47Anim02Seq,
        /* box */ { { -12, -12, 25, 25 }, { -12, -12, 25, 25 } },
        /* paletteId */ 45,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank47Anim02Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 3 */ {
        /* seq */ gSpriteBank47Anim03Seq,
        /* box */ { { -3, -5, 7, 11 }, { -3, -5, 7, 11 } },
        /* paletteId */ 33,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank47Anim03Seq),
        /* flags */ 0,
    },
    /* 4 */ {
        /* seq */ gSpriteBank47Anim04Seq,
        /* box */ { { -3, -5, 7, 11 }, { -3, -6, 7, 12 } },
        /* paletteId */ 67,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank47Anim04Seq),
        /* flags */ 0,
    },
    /* 5 */ {
        /* seq */ gSpriteBank47Anim05Seq,
        /* box */ { { -20, -4, 40, 9 }, { -20, -4, 40, 9 } },
        /* paletteId */ 68,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank47Anim05Seq),
        /* flags */ 0,
    },
    /* 6 */ {
        /* seq */ gSpriteBank47Anim06Seq,
        /* box */ { { -14, -11, 29, 22 }, { -14, -11, 29, 22 } },
        /* paletteId */ 69,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank47Anim06Seq),
        /* flags */ 0,
    },
    /* 7 */ {
        /* seq */ gSpriteBank47Anim07Seq,
        /* box */ { { -15, -9, 31, 18 }, { -15, -9, 31, 18 } },
        /* paletteId */ 70,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank47Anim07Seq),
        /* flags */ 0,
    },
    /* 8 */ {
        /* seq */ gSpriteBank47Anim08Seq,
        /* box */ { { -15, -10, 30, 21 }, { -15, -10, 30, 21 } },
        /* paletteId */ 71,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank47Anim08Seq),
        /* flags */ 0,
    },
    /* 9 */ {
        /* seq */ gSpriteBank47Anim09Seq,
        /* box */ { { -14, -10, 29, 20 }, { -14, -10, 29, 20 } },
        /* paletteId */ 72,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank47Anim09Seq),
        /* flags */ 0,
    },
    /* 10 */ {
        /* seq */ gSpriteBank47Anim10Seq,
        /* box */ { { -13, -11, 27, 23 }, { -13, -11, 27, 23 } },
        /* paletteId */ 45,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank47Anim10Seq),
        /* flags */ 0,
    },
    /* 11 */ {
        /* seq */ gSpriteBank47Anim11Seq,
        /* box */ { { -27, -6, 55, 13 }, { -27, -6, 55, 13 } },
        /* paletteId */ 33,
        /* duration */ 11,
        /* frameCount */ ARRAY_COUNT(gSpriteBank47Anim11Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 12 */ {
        /* seq */ gSpriteBank47Anim12Seq,
        /* box */ { { -15, -11, 31, 23 }, { -15, -11, 31, 23 } },
        /* paletteId */ 118,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank47Anim12Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank47Anim00Seq[11] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
};
const u16 gSpriteBank47Anim01Seq[12] = {
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
};
const u16 gSpriteBank47Anim02Seq[1] = {
    23,
};
const u16 gSpriteBank47Anim03Seq[1] = {
    24,
};
const u16 gSpriteBank47Anim04Seq[12] = {
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
};
const u16 gSpriteBank47Anim05Seq[5] = {
    25, 26, 27, 28, 29,
};
const u16 gSpriteBank47Anim06Seq[1] = {
    30,
};
const u16 gSpriteBank47Anim07Seq[1] = {
    31,
};
const u16 gSpriteBank47Anim08Seq[1] = {
    32,
};
const u16 gSpriteBank47Anim09Seq[1] = {
    33,
};
const u16 gSpriteBank47Anim10Seq[1] = {
    34,
};
const u16 gSpriteBank47Anim11Seq[32] = {
    35, 35, 36, 36, 36, 36, 36, 36, 36, 37, 36, 38, 36, 39, 36, 40,
    36, 41, 36, 36, 37, 38, 39, 40, 41, 36, 35, 35, 36, 36, 36, 36,
};
const u16 gSpriteBank47Anim12Seq[1] = {
    42,
};

extern const struct sprite_frame *const gSpriteBank47Frames[43] = {
    &gSpriteBank47Frame000.frame,
    &gSpriteBank47Frame001.frame,
    &gSpriteBank47Frame002.frame,
    &gSpriteBank47Frame003.frame,
    &gSpriteBank47Frame004.frame,
    &gSpriteBank47Frame005.frame,
    &gSpriteBank47Frame006.frame,
    &gSpriteBank47Frame007.frame,
    &gSpriteBank47Frame008.frame,
    &gSpriteBank47Frame009.frame,
    &gSpriteBank47Frame010.frame,
    &gSpriteBank47Frame011.frame,
    &gSpriteBank47Frame012.frame,
    &gSpriteBank47Frame013.frame,
    &gSpriteBank47Frame014.frame,
    &gSpriteBank47Frame015.frame,
    &gSpriteBank47Frame016.frame,
    &gSpriteBank47Frame017.frame,
    &gSpriteBank47Frame018.frame,
    &gSpriteBank47Frame019.frame,
    &gSpriteBank47Frame020.frame,
    &gSpriteBank47Frame021.frame,
    &gSpriteBank47Frame022.frame,
    &gSpriteBank47Frame023.frame,
    &gSpriteBank47Frame024,
    &gSpriteBank47Frame025.frame,
    &gSpriteBank47Frame026.frame,
    &gSpriteBank47Frame027.frame,
    &gSpriteBank47Frame028.frame,
    &gSpriteBank47Frame029.frame,
    &gSpriteBank47Frame030.frame,
    &gSpriteBank47Frame031.frame,
    &gSpriteBank47Frame032.frame,
    &gSpriteBank47Frame033.frame,
    &gSpriteBank47Frame034.frame,
    &gSpriteBank47Frame035.frame,
    &gSpriteBank47Frame036,
    &gSpriteBank47Frame037.frame,
    &gSpriteBank47Frame038.frame,
    &gSpriteBank47Frame039.frame,
    &gSpriteBank47Frame040.frame,
    &gSpriteBank47Frame041.frame,
    &gSpriteBank47Frame042.frame,
};

const struct sprite_frame_1box gSpriteBank47Frame000 = {
    SPRITE_FRAME(gSpriteBank47Frame000, SPRITE_TILES_BANK47_FRAME000),
    { { -6, -11, 13, 23 } },
};
const struct sprite_frame_1box gSpriteBank47Frame001 = {
    SPRITE_FRAME(gSpriteBank47Frame001, SPRITE_TILES_BANK47_FRAME001),
    { { -4, -11, 9, 23 } },
};
const struct sprite_frame_1box gSpriteBank47Frame002 = {
    SPRITE_FRAME(gSpriteBank47Frame002, SPRITE_TILES_BANK47_FRAME002),
    { { -7, -11, 15, 23 } },
};
const struct sprite_frame_1box gSpriteBank47Frame003 = {
    SPRITE_FRAME(gSpriteBank47Frame003, SPRITE_TILES_BANK47_FRAME003),
    { { -6, -11, 14, 23 } },
};
const struct sprite_frame_1box gSpriteBank47Frame004 = {
    SPRITE_FRAME(gSpriteBank47Frame004, SPRITE_TILES_BANK47_FRAME004),
    { { -6, -11, 14, 23 } },
};
const struct sprite_frame_1box gSpriteBank47Frame005 = {
    SPRITE_FRAME(gSpriteBank47Frame005, SPRITE_TILES_BANK47_FRAME005),
    { { -6, -11, 14, 23 } },
};
const struct sprite_frame_1box gSpriteBank47Frame006 = {
    SPRITE_FRAME(gSpriteBank47Frame006, SPRITE_TILES_BANK47_FRAME006),
    { { -7, -11, 14, 23 } },
};
const struct sprite_frame_1box gSpriteBank47Frame007 = {
    SPRITE_FRAME(gSpriteBank47Frame007, SPRITE_TILES_BANK47_FRAME007),
    { { -6, -10, 14, 22 } },
};
const struct sprite_frame_1box gSpriteBank47Frame008 = {
    SPRITE_FRAME(gSpriteBank47Frame008, SPRITE_TILES_BANK47_FRAME008),
    { { -6, -11, 13, 23 } },
};
const struct sprite_frame_1box gSpriteBank47Frame009 = {
    SPRITE_FRAME(gSpriteBank47Frame009, SPRITE_TILES_BANK47_FRAME009),
    { { -7, -11, 15, 23 } },
};
const struct sprite_frame_1box gSpriteBank47Frame010 = {
    SPRITE_FRAME(gSpriteBank47Frame010, SPRITE_TILES_BANK47_FRAME010),
    { { -7, -11, 15, 23 } },
};
const struct sprite_frame_1box gSpriteBank47Frame011 = {
    SPRITE_FRAME(gSpriteBank47Frame011, SPRITE_TILES_BANK47_FRAME011),
    { { -3, -5, 7, 11 } },
};
const struct sprite_frame_1box gSpriteBank47Frame012 = {
    SPRITE_FRAME(gSpriteBank47Frame012, SPRITE_TILES_BANK47_FRAME012),
    { { -2, -5, 5, 11 } },
};
const struct sprite_frame_1box gSpriteBank47Frame013 = {
    SPRITE_FRAME(gSpriteBank47Frame013, SPRITE_TILES_BANK47_FRAME013),
    { { -3, -6, 7, 12 } },
};
const struct sprite_frame_1box gSpriteBank47Frame014 = {
    SPRITE_FRAME(gSpriteBank47Frame014, SPRITE_TILES_BANK47_FRAME014),
    { { -3, -6, 7, 12 } },
};
const struct sprite_frame_1box gSpriteBank47Frame015 = {
    SPRITE_FRAME(gSpriteBank47Frame015, SPRITE_TILES_BANK47_FRAME015),
    { { -3, -6, 7, 12 } },
};
const struct sprite_frame_1box gSpriteBank47Frame016 = {
    SPRITE_FRAME(gSpriteBank47Frame016, SPRITE_TILES_BANK47_FRAME016),
    { { -3, -6, 7, 12 } },
};
const struct sprite_frame_1box gSpriteBank47Frame017 = {
    SPRITE_FRAME(gSpriteBank47Frame017, SPRITE_TILES_BANK47_FRAME017),
    { { -3, -5, 7, 11 } },
};
const struct sprite_frame_1box gSpriteBank47Frame018 = {
    SPRITE_FRAME(gSpriteBank47Frame018, SPRITE_TILES_BANK47_FRAME018),
    { { -3, -5, 7, 11 } },
};
const struct sprite_frame_1box gSpriteBank47Frame019 = {
    SPRITE_FRAME(gSpriteBank47Frame019, SPRITE_TILES_BANK47_FRAME019),
    { { -3, -6, 7, 12 } },
};
const struct sprite_frame_1box gSpriteBank47Frame020 = {
    SPRITE_FRAME(gSpriteBank47Frame020, SPRITE_TILES_BANK47_FRAME020),
    { { -3, -6, 7, 12 } },
};
const struct sprite_frame_1box gSpriteBank47Frame021 = {
    SPRITE_FRAME(gSpriteBank47Frame021, SPRITE_TILES_BANK47_FRAME021),
    { { -1, -4, 3, 10 } },
};
const struct sprite_frame_1box gSpriteBank47Frame022 = {
    SPRITE_FRAME(gSpriteBank47Frame022, SPRITE_TILES_BANK47_FRAME022),
    { { -1, 2, 3, 4 } },
};
const struct sprite_frame_1box gSpriteBank47Frame023 = {
    SPRITE_FRAME(gSpriteBank47Frame023, SPRITE_TILES_BANK47_FRAME023),
    { { -12, -12, 25, 25 } },
};
const struct sprite_frame gSpriteBank47Frame024 = SPRITE_FRAME(gSpriteBank47Frame024, SPRITE_TILES_BANK47_FRAME024);
const struct sprite_frame_1box gSpriteBank47Frame025 = {
    SPRITE_FRAME(gSpriteBank47Frame025, SPRITE_TILES_BANK47_FRAME025),
    { { -20, -4, 40, 9 } },
};
const struct sprite_frame_1box gSpriteBank47Frame026 = {
    SPRITE_FRAME(gSpriteBank47Frame026, SPRITE_TILES_BANK47_FRAME026),
    { { -20, -4, 32, 9 } },
};
const struct sprite_frame_1box gSpriteBank47Frame027 = {
    SPRITE_FRAME(gSpriteBank47Frame027, SPRITE_TILES_BANK47_FRAME027),
    { { -20, -4, 24, 9 } },
};
const struct sprite_frame_1box gSpriteBank47Frame028 = {
    SPRITE_FRAME(gSpriteBank47Frame028, SPRITE_TILES_BANK47_FRAME028),
    { { -20, -4, 16, 9 } },
};
const struct sprite_frame_1box gSpriteBank47Frame029 = {
    SPRITE_FRAME(gSpriteBank47Frame029, SPRITE_TILES_BANK47_FRAME029),
    { { -20, -4, 8, 9 } },
};
const struct sprite_frame_1box gSpriteBank47Frame030 = {
    SPRITE_FRAME(gSpriteBank47Frame030, SPRITE_TILES_BANK47_FRAME030),
    { { -14, -11, 29, 22 } },
};
const struct sprite_frame_1box gSpriteBank47Frame031 = {
    SPRITE_FRAME(gSpriteBank47Frame031, SPRITE_TILES_BANK47_FRAME031),
    { { -15, -9, 31, 18 } },
};
const struct sprite_frame_1box gSpriteBank47Frame032 = {
    SPRITE_FRAME(gSpriteBank47Frame032, SPRITE_TILES_BANK47_FRAME032),
    { { -15, -10, 30, 21 } },
};
const struct sprite_frame_1box gSpriteBank47Frame033 = {
    SPRITE_FRAME(gSpriteBank47Frame033, SPRITE_TILES_BANK47_FRAME033),
    { { -14, -10, 29, 20 } },
};
const struct sprite_frame_1box gSpriteBank47Frame034 = {
    SPRITE_FRAME(gSpriteBank47Frame034, SPRITE_TILES_BANK47_FRAME034),
    { { -13, -11, 27, 23 } },
};
const struct sprite_frame_1box gSpriteBank47Frame035 = {
    SPRITE_FRAME(gSpriteBank47Frame035, SPRITE_TILES_BANK47_FRAME035),
    { { -27, -6, 55, 13 } },
};
const struct sprite_frame gSpriteBank47Frame036 = { gSpriteBank47Frame037Pos, gSpriteBank47Frame037Pieces, SPRITE_FRAME_TILES(SPRITE_TILES_BANK47_FRAME036, 0) };
const struct sprite_frame_1box gSpriteBank47Frame037 = {
    SPRITE_FRAME(gSpriteBank47Frame037, SPRITE_TILES_BANK47_FRAME037),
    { { -27, -6, 11, 12 } },
};
const struct sprite_frame_1box gSpriteBank47Frame038 = {
    SPRITE_FRAME(gSpriteBank47Frame038, SPRITE_TILES_BANK47_FRAME038),
    { { -14, -6, 9, 13 } },
};
const struct sprite_frame_1box gSpriteBank47Frame039 = {
    SPRITE_FRAME(gSpriteBank47Frame039, SPRITE_TILES_BANK47_FRAME039),
    { { -3, -6, 9, 13 } },
};
const struct sprite_frame_1box gSpriteBank47Frame040 = {
    SPRITE_FRAME(gSpriteBank47Frame040, SPRITE_TILES_BANK47_FRAME040),
    { { 8, -6, 9, 13 } },
};
const struct sprite_frame_1box gSpriteBank47Frame041 = {
    SPRITE_FRAME(gSpriteBank47Frame041, SPRITE_TILES_BANK47_FRAME041),
    { { 19, -6, 9, 13 } },
};
const struct sprite_frame_1box gSpriteBank47Frame042 = {
    SPRITE_FRAME(gSpriteBank47Frame042, SPRITE_TILES_BANK47_FRAME042),
    { { -15, -11, 31, 23 } },
};

/* The piece arrays, from graphics/sprites/bank47_icons/bank.json. */
#include "sprites/bank47_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 48: 4 animations, 38 frames, graphics/sprites/bank48_level_select_cursor/. */

extern const u16 gSpriteBank48Anim00Seq[10];
extern const u16 gSpriteBank48Anim01Seq[9];
extern const u16 gSpriteBank48Anim02Seq[10];
extern const u16 gSpriteBank48Anim03Seq[10];

extern const struct sprite_anim gSpriteBank48Anims[4] = {
    /* 0 */ {
        /* seq */ gSpriteBank48Anim00Seq,
        /* box */ { { -15, -23, 30, 47 }, { -15, -25, 30, 49 } },
        /* paletteId */ 102,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank48Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank48Anim01Seq,
        /* box */ { { -15, -23, 30, 47 }, { -15, -27, 30, 51 } },
        /* paletteId */ 102,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank48Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank48Anim02Seq,
        /* box */ { { -15, -23, 30, 47 }, { -15, -28, 30, 52 } },
        /* paletteId */ 102,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank48Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank48Anim03Seq,
        /* box */ { { -15, -23, 30, 47 }, { -14, -26, 29, 50 } },
        /* paletteId */ 102,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank48Anim03Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank48Anim00Seq[10] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
};
const u16 gSpriteBank48Anim01Seq[9] = {
    10, 11, 12, 13, 14, 15, 16, 17, 18,
};
const u16 gSpriteBank48Anim02Seq[10] = {
    19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
};
const u16 gSpriteBank48Anim03Seq[10] = {
    29, 30, 31, 32, 33, 34, 35, 36, 37, 37,
};

extern const struct sprite_frame *const gSpriteBank48Frames[38] = {
    &gSpriteBank48Frame000.frame,
    &gSpriteBank48Frame001.frame,
    &gSpriteBank48Frame002.frame,
    &gSpriteBank48Frame003.frame,
    &gSpriteBank48Frame004.frame,
    &gSpriteBank48Frame005.frame,
    &gSpriteBank48Frame006.frame,
    &gSpriteBank48Frame007.frame,
    &gSpriteBank48Frame008.frame,
    &gSpriteBank48Frame009.frame,
    &gSpriteBank48Frame010.frame,
    &gSpriteBank48Frame011.frame,
    &gSpriteBank48Frame012.frame,
    &gSpriteBank48Frame013.frame,
    &gSpriteBank48Frame014.frame,
    &gSpriteBank48Frame015.frame,
    &gSpriteBank48Frame016.frame,
    &gSpriteBank48Frame017.frame,
    &gSpriteBank48Frame018.frame,
    &gSpriteBank48Frame019.frame,
    &gSpriteBank48Frame020.frame,
    &gSpriteBank48Frame021.frame,
    &gSpriteBank48Frame022.frame,
    &gSpriteBank48Frame023.frame,
    &gSpriteBank48Frame024.frame,
    &gSpriteBank48Frame025.frame,
    &gSpriteBank48Frame026.frame,
    &gSpriteBank48Frame027.frame,
    &gSpriteBank48Frame028.frame,
    &gSpriteBank48Frame029.frame,
    &gSpriteBank48Frame030.frame,
    &gSpriteBank48Frame031.frame,
    &gSpriteBank48Frame032.frame,
    &gSpriteBank48Frame033.frame,
    &gSpriteBank48Frame034.frame,
    &gSpriteBank48Frame035.frame,
    &gSpriteBank48Frame036.frame,
    &gSpriteBank48Frame037.frame,
};

const struct sprite_frame_1box gSpriteBank48Frame000 = {
    SPRITE_FRAME(gSpriteBank48Frame000, SPRITE_TILES_BANK48_FRAME000),
    { { -18, -27, 35, 55 } },
};
const struct sprite_frame_1box gSpriteBank48Frame001 = {
    SPRITE_FRAME(gSpriteBank48Frame001, SPRITE_TILES_BANK48_FRAME001),
    { { -18, -28, 35, 55 } },
};
const struct sprite_frame_1box gSpriteBank48Frame002 = {
    SPRITE_FRAME(gSpriteBank48Frame002, SPRITE_TILES_BANK48_FRAME002),
    { { -18, -29, 35, 56 } },
};
const struct sprite_frame_1box gSpriteBank48Frame003 = {
    SPRITE_FRAME(gSpriteBank48Frame003, SPRITE_TILES_BANK48_FRAME003),
    { { -18, -29, 35, 55 } },
};
const struct sprite_frame_1box gSpriteBank48Frame004 = {
    SPRITE_FRAME(gSpriteBank48Frame004, SPRITE_TILES_BANK48_FRAME004),
    { { -18, -30, 35, 56 } },
};
const struct sprite_frame_1box gSpriteBank48Frame005 = {
    SPRITE_FRAME(gSpriteBank48Frame005, SPRITE_TILES_BANK48_FRAME005),
    { { -18, -29, 35, 56 } },
};
const struct sprite_frame_1box gSpriteBank48Frame006 = {
    SPRITE_FRAME(gSpriteBank48Frame006, SPRITE_TILES_BANK48_FRAME006),
    { { -18, -29, 35, 56 } },
};
const struct sprite_frame_1box gSpriteBank48Frame007 = {
    SPRITE_FRAME(gSpriteBank48Frame007, SPRITE_TILES_BANK48_FRAME007),
    { { -18, -27, 35, 55 } },
};
const struct sprite_frame_1box gSpriteBank48Frame008 = {
    SPRITE_FRAME(gSpriteBank48Frame008, SPRITE_TILES_BANK48_FRAME008),
    { { -18, -27, 35, 55 } },
};
const struct sprite_frame_1box gSpriteBank48Frame009 = {
    SPRITE_FRAME(gSpriteBank48Frame009, SPRITE_TILES_BANK48_FRAME009),
    { { -18, -27, 35, 55 } },
};
const struct sprite_frame_1box gSpriteBank48Frame010 = {
    SPRITE_FRAME(gSpriteBank48Frame010, SPRITE_TILES_BANK48_FRAME010),
    { { -15, -23, 30, 46 } },
};
const struct sprite_frame_1box gSpriteBank48Frame011 = {
    SPRITE_FRAME(gSpriteBank48Frame011, SPRITE_TILES_BANK48_FRAME011),
    { { -15, -24, 30, 47 } },
};
const struct sprite_frame_1box gSpriteBank48Frame012 = {
    SPRITE_FRAME(gSpriteBank48Frame012, SPRITE_TILES_BANK48_FRAME012),
    { { -15, -25, 30, 47 } },
};
const struct sprite_frame_1box gSpriteBank48Frame013 = {
    SPRITE_FRAME(gSpriteBank48Frame013, SPRITE_TILES_BANK48_FRAME013),
    { { -15, -27, 30, 49 } },
};
const struct sprite_frame_1box gSpriteBank48Frame014 = {
    SPRITE_FRAME(gSpriteBank48Frame014, SPRITE_TILES_BANK48_FRAME014),
    { { -15, -27, 30, 50 } },
};
const struct sprite_frame_1box gSpriteBank48Frame015 = {
    SPRITE_FRAME(gSpriteBank48Frame015, SPRITE_TILES_BANK48_FRAME015),
    { { -15, -26, 30, 49 } },
};
const struct sprite_frame_1box gSpriteBank48Frame016 = {
    SPRITE_FRAME(gSpriteBank48Frame016, SPRITE_TILES_BANK48_FRAME016),
    { { -15, -26, 30, 50 } },
};
const struct sprite_frame_1box gSpriteBank48Frame017 = {
    SPRITE_FRAME(gSpriteBank48Frame017, SPRITE_TILES_BANK48_FRAME017),
    { { -15, -25, 30, 49 } },
};
const struct sprite_frame_1box gSpriteBank48Frame018 = {
    SPRITE_FRAME(gSpriteBank48Frame018, SPRITE_TILES_BANK48_FRAME018),
    { { -15, -24, 30, 48 } },
};
const struct sprite_frame_1box gSpriteBank48Frame019 = {
    SPRITE_FRAME(gSpriteBank48Frame019, SPRITE_TILES_BANK48_FRAME019),
    { { -18, -28, 35, 56 } },
};
const struct sprite_frame_1box gSpriteBank48Frame020 = {
    SPRITE_FRAME(gSpriteBank48Frame020, SPRITE_TILES_BANK48_FRAME020),
    { { -18, -29, 35, 56 } },
};
const struct sprite_frame_1box gSpriteBank48Frame021 = {
    SPRITE_FRAME(gSpriteBank48Frame021, SPRITE_TILES_BANK48_FRAME021),
    { { -18, -30, 35, 57 } },
};
const struct sprite_frame_1box gSpriteBank48Frame022 = {
    SPRITE_FRAME(gSpriteBank48Frame022, SPRITE_TILES_BANK48_FRAME022),
    { { -18, -31, 35, 57 } },
};
const struct sprite_frame_1box gSpriteBank48Frame023 = {
    SPRITE_FRAME(gSpriteBank48Frame023, SPRITE_TILES_BANK48_FRAME023),
    { { -18, -32, 35, 58 } },
};
const struct sprite_frame_1box gSpriteBank48Frame024 = {
    SPRITE_FRAME(gSpriteBank48Frame024, SPRITE_TILES_BANK48_FRAME024),
    { { -18, -31, 35, 58 } },
};
const struct sprite_frame_1box gSpriteBank48Frame025 = {
    SPRITE_FRAME(gSpriteBank48Frame025, SPRITE_TILES_BANK48_FRAME025),
    { { -19, -30, 36, 57 } },
};
const struct sprite_frame_1box gSpriteBank48Frame026 = {
    SPRITE_FRAME(gSpriteBank48Frame026, SPRITE_TILES_BANK48_FRAME026),
    { { -19, -29, 36, 57 } },
};
const struct sprite_frame_1box gSpriteBank48Frame027 = {
    SPRITE_FRAME(gSpriteBank48Frame027, SPRITE_TILES_BANK48_FRAME027),
    { { -18, -28, 35, 56 } },
};
const struct sprite_frame_1box gSpriteBank48Frame028 = {
    SPRITE_FRAME(gSpriteBank48Frame028, SPRITE_TILES_BANK48_FRAME028),
    { { -18, -28, 35, 56 } },
};
const struct sprite_frame_1box gSpriteBank48Frame029 = {
    SPRITE_FRAME(gSpriteBank48Frame029, SPRITE_TILES_BANK48_FRAME029),
    { { -18, -28, 35, 56 } },
};
const struct sprite_frame_1box gSpriteBank48Frame030 = {
    SPRITE_FRAME(gSpriteBank48Frame030, SPRITE_TILES_BANK48_FRAME030),
    { { -18, -29, 35, 56 } },
};
const struct sprite_frame_1box gSpriteBank48Frame031 = {
    SPRITE_FRAME(gSpriteBank48Frame031, SPRITE_TILES_BANK48_FRAME031),
    { { -18, -30, 35, 57 } },
};
const struct sprite_frame_1box gSpriteBank48Frame032 = {
    SPRITE_FRAME(gSpriteBank48Frame032, SPRITE_TILES_BANK48_FRAME032),
    { { -18, -31, 35, 57 } },
};
const struct sprite_frame_1box gSpriteBank48Frame033 = {
    SPRITE_FRAME(gSpriteBank48Frame033, SPRITE_TILES_BANK48_FRAME033),
    { { -18, -31, 35, 57 } },
};
const struct sprite_frame_1box gSpriteBank48Frame034 = {
    SPRITE_FRAME(gSpriteBank48Frame034, SPRITE_TILES_BANK48_FRAME034),
    { { -18, -30, 35, 57 } },
};
const struct sprite_frame_1box gSpriteBank48Frame035 = {
    SPRITE_FRAME(gSpriteBank48Frame035, SPRITE_TILES_BANK48_FRAME035),
    { { -18, -29, 35, 56 } },
};
const struct sprite_frame_1box gSpriteBank48Frame036 = {
    SPRITE_FRAME(gSpriteBank48Frame036, SPRITE_TILES_BANK48_FRAME036),
    { { -18, -27, 35, 55 } },
};
const struct sprite_frame_1box gSpriteBank48Frame037 = {
    SPRITE_FRAME(gSpriteBank48Frame037, SPRITE_TILES_BANK48_FRAME037),
    { { -18, -27, 35, 55 } },
};

/* The piece arrays, from graphics/sprites/bank48_level_select_cursor/bank.json. */
#include "sprites/bank48_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 49: 5 animations, 2 frames, graphics/sprites/bank49_level_select_frame/. */

extern const u16 gSpriteBank49Anim00Seq[2];
extern const u16 gSpriteBank49Anim01Seq[2];
extern const u16 gSpriteBank49Anim02Seq[2];
extern const u16 gSpriteBank49Anim03Seq[2];
extern const u16 gSpriteBank49Anim04Seq[2];

extern const struct sprite_anim gSpriteBank49Anims[5] = {
    /* 0 */ {
        /* seq */ gSpriteBank49Anim00Seq,
        /* box */ { { -19, -9, 39, 18 }, { -19, -9, 39, 18 } },
        /* paletteId */ 74,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank49Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank49Anim01Seq,
        /* box */ { { -19, -9, 39, 18 }, { -19, -9, 39, 18 } },
        /* paletteId */ 75,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank49Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank49Anim02Seq,
        /* box */ { { -19, -9, 39, 18 }, { -19, -9, 39, 18 } },
        /* paletteId */ 76,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank49Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank49Anim03Seq,
        /* box */ { { -19, -9, 39, 18 }, { -19, -9, 39, 18 } },
        /* paletteId */ 77,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank49Anim03Seq),
        /* flags */ 0,
    },
    /* 4 */ {
        /* seq */ gSpriteBank49Anim04Seq,
        /* box */ { { -19, -9, 39, 18 }, { -19, -9, 39, 18 } },
        /* paletteId */ 78,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank49Anim04Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank49Anim00Seq[2] = {
    0, 1,
};
const u16 gSpriteBank49Anim01Seq[2] = {
    0, 1,
};
const u16 gSpriteBank49Anim02Seq[2] = {
    0, 1,
};
const u16 gSpriteBank49Anim03Seq[2] = {
    0, 1,
};
const u16 gSpriteBank49Anim04Seq[2] = {
    0, 1,
};

extern const struct sprite_frame *const gSpriteBank49Frames[2] = {
    &gSpriteBank49Frame000.frame,
    &gSpriteBank49Frame001.frame,
};

const struct sprite_frame_1box gSpriteBank49Frame000 = {
    SPRITE_FRAME(gSpriteBank49Frame000, SPRITE_TILES_BANK49_FRAME000),
    { { -19, -9, 39, 18 } },
};
const struct sprite_frame_1box gSpriteBank49Frame001 = {
    SPRITE_FRAME(gSpriteBank49Frame001, SPRITE_TILES_BANK49_FRAME001),
    { { -19, -7, 39, 16 } },
};

/* The piece arrays, from graphics/sprites/bank49_level_select_frame/bank.json. */
#include "sprites/bank49_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 50: 1 animation, 8 frames, graphics/sprites/bank50_twinkles/. */

extern const u16 gSpriteBank50Anim00Seq[8];

extern const struct sprite_anim gSpriteBank50Anims[1] = {
    /* 0 */ {
        /* seq */ gSpriteBank50Anim00Seq,
        /* box */ { { -5, -67, 23, 65 }, { -5, -67, 69, 72 } },
        /* paletteId */ 79,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank50Anim00Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank50Anim00Seq[8] = {
    0, 1, 2, 3, 4, 5, 6, 7,
};

extern const struct sprite_frame *const gSpriteBank50Frames[8] = {
    &gSpriteBank50Frame000.frame,
    &gSpriteBank50Frame001.frame,
    &gSpriteBank50Frame002.frame,
    &gSpriteBank50Frame003.frame,
    &gSpriteBank50Frame004.frame,
    &gSpriteBank50Frame005.frame,
    &gSpriteBank50Frame006.frame,
    &gSpriteBank50Frame007.frame,
};

const struct sprite_frame_1box gSpriteBank50Frame000 = {
    SPRITE_FRAME(gSpriteBank50Frame000, SPRITE_TILES_BANK50_FRAME000),
    { { -5, -67, 23, 65 } },
};
const struct sprite_frame_1box gSpriteBank50Frame001 = {
    SPRITE_FRAME(gSpriteBank50Frame001, SPRITE_TILES_BANK50_FRAME001),
    { { -3, -67, 23, 63 } },
};
const struct sprite_frame_1box gSpriteBank50Frame002 = {
    SPRITE_FRAME(gSpriteBank50Frame002, SPRITE_TILES_BANK50_FRAME002),
    { { 0, -60, 42, 53 } },
};
const struct sprite_frame_1box gSpriteBank50Frame003 = {
    SPRITE_FRAME(gSpriteBank50Frame003, SPRITE_TILES_BANK50_FRAME003),
    { { 0, -59, 42, 54 } },
};
const struct sprite_frame_1box gSpriteBank50Frame004 = {
    SPRITE_FRAME(gSpriteBank50Frame004, SPRITE_TILES_BANK50_FRAME004),
    { { -1, -46, 61, 46 } },
};
const struct sprite_frame_1box gSpriteBank50Frame005 = {
    SPRITE_FRAME(gSpriteBank50Frame005, SPRITE_TILES_BANK50_FRAME005),
    { { 1, -49, 57, 48 } },
};
const struct sprite_frame_1box gSpriteBank50Frame006 = {
    SPRITE_FRAME(gSpriteBank50Frame006, SPRITE_TILES_BANK50_FRAME006),
    { { -2, -25, 66, 28 } },
};
const struct sprite_frame_1box gSpriteBank50Frame007 = {
    SPRITE_FRAME(gSpriteBank50Frame007, SPRITE_TILES_BANK50_FRAME007),
    { { -1, -24, 64, 32 } },
};

/* The piece arrays, from graphics/sprites/bank50_twinkles/bank.json. */
#include "sprites/bank50_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 51: 5 animations, 24 frames, graphics/sprites/bank51_level_select_icons/. */

extern const u16 gSpriteBank51Anim00Seq[20];
extern const u16 gSpriteBank51Anim01Seq[1];
extern const u16 gSpriteBank51Anim02Seq[1];
extern const u16 gSpriteBank51Anim03Seq[1];
extern const u16 gSpriteBank51Anim04Seq[1];

extern const struct sprite_anim gSpriteBank51Anims[5] = {
    /* 0 */ {
        /* seq */ gSpriteBank51Anim00Seq,
        /* box */ { { -3, -4, 6, 8 }, { -10, -4, 20, 8 } },
        /* paletteId */ 80,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank51Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank51Anim01Seq,
        /* box */ { { -9, -6, 19, 13 }, { -9, -6, 19, 13 } },
        /* paletteId */ 81,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank51Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank51Anim02Seq,
        /* box */ { { -10, -6, 20, 12 }, { -10, -6, 20, 12 } },
        /* paletteId */ 82,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank51Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank51Anim03Seq,
        /* box */ { { -10, -7, 21, 14 }, { -10, -7, 21, 14 } },
        /* paletteId */ 83,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank51Anim03Seq),
        /* flags */ 0,
    },
    /* 4 */ {
        /* seq */ gSpriteBank51Anim04Seq,
        /* box */ { { -13, -6, 26, 13 }, { -13, -6, 26, 13 } },
        /* paletteId */ 84,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank51Anim04Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank51Anim00Seq[20] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19,
};
const u16 gSpriteBank51Anim01Seq[1] = {
    20,
};
const u16 gSpriteBank51Anim02Seq[1] = {
    21,
};
const u16 gSpriteBank51Anim03Seq[1] = {
    22,
};
const u16 gSpriteBank51Anim04Seq[1] = {
    23,
};

extern const struct sprite_frame *const gSpriteBank51Frames[24] = {
    &gSpriteBank51Frame000.frame,
    &gSpriteBank51Frame001.frame,
    &gSpriteBank51Frame002.frame,
    &gSpriteBank51Frame003.frame,
    &gSpriteBank51Frame004.frame,
    &gSpriteBank51Frame005.frame,
    &gSpriteBank51Frame006.frame,
    &gSpriteBank51Frame007.frame,
    &gSpriteBank51Frame008.frame,
    &gSpriteBank51Frame009.frame,
    &gSpriteBank51Frame010.frame,
    &gSpriteBank51Frame011.frame,
    &gSpriteBank51Frame012.frame,
    &gSpriteBank51Frame013.frame,
    &gSpriteBank51Frame014.frame,
    &gSpriteBank51Frame015.frame,
    &gSpriteBank51Frame016.frame,
    &gSpriteBank51Frame017.frame,
    &gSpriteBank51Frame018.frame,
    &gSpriteBank51Frame019.frame,
    &gSpriteBank51Frame020.frame,
    &gSpriteBank51Frame021.frame,
    &gSpriteBank51Frame022.frame,
    &gSpriteBank51Frame023.frame,
};

const struct sprite_frame_1box gSpriteBank51Frame000 = {
    SPRITE_FRAME(gSpriteBank51Frame000, SPRITE_TILES_BANK51_FRAME000),
    { { -3, -4, 6, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame001 = {
    SPRITE_FRAME(gSpriteBank51Frame001, SPRITE_TILES_BANK51_FRAME001),
    { { -5, -4, 8, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame002 = {
    SPRITE_FRAME(gSpriteBank51Frame002, SPRITE_TILES_BANK51_FRAME002),
    { { -4, -4, 8, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame003 = {
    SPRITE_FRAME(gSpriteBank51Frame003, SPRITE_TILES_BANK51_FRAME003),
    { { -6, -4, 10, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame004 = {
    SPRITE_FRAME(gSpriteBank51Frame004, SPRITE_TILES_BANK51_FRAME004),
    { { -5, -4, 8, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame005 = {
    SPRITE_FRAME(gSpriteBank51Frame005, SPRITE_TILES_BANK51_FRAME005),
    { { -6, -4, 10, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame006 = {
    SPRITE_FRAME(gSpriteBank51Frame006, SPRITE_TILES_BANK51_FRAME006),
    { { -5, -4, 9, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame007 = {
    SPRITE_FRAME(gSpriteBank51Frame007, SPRITE_TILES_BANK51_FRAME007),
    { { -5, -4, 9, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame008 = {
    SPRITE_FRAME(gSpriteBank51Frame008, SPRITE_TILES_BANK51_FRAME008),
    { { -5, -4, 9, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame009 = {
    SPRITE_FRAME(gSpriteBank51Frame009, SPRITE_TILES_BANK51_FRAME009),
    { { -7, -4, 15, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame010 = {
    SPRITE_FRAME(gSpriteBank51Frame010, SPRITE_TILES_BANK51_FRAME010),
    { { -7, -4, 13, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame011 = {
    SPRITE_FRAME(gSpriteBank51Frame011, SPRITE_TILES_BANK51_FRAME011),
    { { -7, -4, 15, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame012 = {
    SPRITE_FRAME(gSpriteBank51Frame012, SPRITE_TILES_BANK51_FRAME012),
    { { -7, -4, 16, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame013 = {
    SPRITE_FRAME(gSpriteBank51Frame013, SPRITE_TILES_BANK51_FRAME013),
    { { -7, -4, 17, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame014 = {
    SPRITE_FRAME(gSpriteBank51Frame014, SPRITE_TILES_BANK51_FRAME014),
    { { -7, -4, 16, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame015 = {
    SPRITE_FRAME(gSpriteBank51Frame015, SPRITE_TILES_BANK51_FRAME015),
    { { -7, -4, 17, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame016 = {
    SPRITE_FRAME(gSpriteBank51Frame016, SPRITE_TILES_BANK51_FRAME016),
    { { -7, -4, 16, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame017 = {
    SPRITE_FRAME(gSpriteBank51Frame017, SPRITE_TILES_BANK51_FRAME017),
    { { -7, -4, 16, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame018 = {
    SPRITE_FRAME(gSpriteBank51Frame018, SPRITE_TILES_BANK51_FRAME018),
    { { -7, -4, 16, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame019 = {
    SPRITE_FRAME(gSpriteBank51Frame019, SPRITE_TILES_BANK51_FRAME019),
    { { -10, -4, 18, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame020 = {
    SPRITE_FRAME(gSpriteBank51Frame020, SPRITE_TILES_BANK51_FRAME020),
    { { -9, -6, 19, 13 } },
};
const struct sprite_frame_1box gSpriteBank51Frame021 = {
    SPRITE_FRAME(gSpriteBank51Frame021, SPRITE_TILES_BANK51_FRAME021),
    { { -10, -6, 20, 12 } },
};
const struct sprite_frame_1box gSpriteBank51Frame022 = {
    SPRITE_FRAME(gSpriteBank51Frame022, SPRITE_TILES_BANK51_FRAME022),
    { { -10, -7, 21, 14 } },
};
const struct sprite_frame_1box gSpriteBank51Frame023 = {
    SPRITE_FRAME(gSpriteBank51Frame023, SPRITE_TILES_BANK51_FRAME023),
    { { -13, -6, 26, 13 } },
};

/* The piece arrays, from graphics/sprites/bank51_level_select_icons/bank.json. */
#include "sprites/bank51_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 52: 2 animations, 4 frames, graphics/sprites/bank52_level_select_arrows/. */

extern const u16 gSpriteBank52Anim00Seq[2];
extern const u16 gSpriteBank52Anim01Seq[2];

extern const struct sprite_anim gSpriteBank52Anims[2] = {
    /* 0 */ {
        /* seq */ gSpriteBank52Anim00Seq,
        /* box */ { { -7, -7, 15, 15 }, { -7, -7, 15, 15 } },
        /* paletteId */ 33,
        /* duration */ 9,
        /* frameCount */ ARRAY_COUNT(gSpriteBank52Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank52Anim01Seq,
        /* box */ { { -7, -7, 15, 15 }, { -7, -7, 15, 15 } },
        /* paletteId */ 33,
        /* duration */ 9,
        /* frameCount */ ARRAY_COUNT(gSpriteBank52Anim01Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank52Anim00Seq[2] = {
    0, 1,
};
const u16 gSpriteBank52Anim01Seq[2] = {
    2, 3,
};

extern const struct sprite_frame *const gSpriteBank52Frames[4] = {
    &gSpriteBank52Frame000.frame,
    &gSpriteBank52Frame001.frame,
    &gSpriteBank52Frame002.frame,
    &gSpriteBank52Frame003.frame,
};

const struct sprite_frame_1box gSpriteBank52Frame000 = {
    SPRITE_FRAME(gSpriteBank52Frame000, SPRITE_TILES_BANK52_FRAME000),
    { { -7, -7, 15, 15 } },
};
const struct sprite_frame_1box gSpriteBank52Frame001 = {
    SPRITE_FRAME(gSpriteBank52Frame001, SPRITE_TILES_BANK52_FRAME001),
    { { -7, -7, 15, 15 } },
};
const struct sprite_frame_1box gSpriteBank52Frame002 = {
    SPRITE_FRAME(gSpriteBank52Frame002, SPRITE_TILES_BANK52_FRAME002),
    { { -7, -7, 15, 15 } },
};
const struct sprite_frame_1box gSpriteBank52Frame003 = {
    SPRITE_FRAME(gSpriteBank52Frame003, SPRITE_TILES_BANK52_FRAME003),
    { { -7, -7, 15, 15 } },
};

/* The piece arrays, from graphics/sprites/bank52_level_select_arrows/bank.json. */
#include "sprites/bank52_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 53: 19 animations, 67 frames, graphics/sprites/bank53_cortex/. */

extern const u16 gSpriteBank53Anim00Seq[1];
extern const u16 gSpriteBank53Anim01Seq[6];
extern const u16 gSpriteBank53Anim02Seq[1];
extern const u16 gSpriteBank53Anim03Seq[6];
extern const u16 gSpriteBank53Anim04Seq[6];
extern const u16 gSpriteBank53Anim05Seq[6];
extern const u16 gSpriteBank53Anim06Seq[1];
extern const u16 gSpriteBank53Anim07Seq[1];
extern const u16 gSpriteBank53Anim08Seq[1];
extern const u16 gSpriteBank53Anim09Seq[5];
extern const u16 gSpriteBank53Anim10Seq[5];
extern const u16 gSpriteBank53Anim11Seq[5];
extern const u16 gSpriteBank53Anim12Seq[5];
extern const u16 gSpriteBank53Anim13Seq[5];
extern const u16 gSpriteBank53Anim14Seq[4];
extern const u16 gSpriteBank53Anim15Seq[1];
extern const u16 gSpriteBank53Anim16Seq[2];
extern const u16 gSpriteBank53Anim17Seq[6];
extern const u16 gSpriteBank53Anim18Seq[3];

extern const struct sprite_anim gSpriteBank53Anims[19] = {
    /* 0 */ {
        /* seq */ gSpriteBank53Anim00Seq,
        /* box */ { { -7, -43, 15, 87 }, { -7, -43, 15, 87 } },
        /* paletteId */ 85,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank53Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank53Anim01Seq,
        /* box */ { { -31, -16, 52, 32 }, { -31, -16, 62, 32 } },
        /* paletteId */ 86,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank53Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank53Anim02Seq,
        /* box */ { { -7, -43, 15, 87 }, { -7, -43, 15, 87 } },
        /* paletteId */ 87,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank53Anim02Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 3 */ {
        /* seq */ gSpriteBank53Anim03Seq,
        /* box */ { { -72, -35, 68, 49 }, { -72, -36, 104, 50 } },
        /* paletteId */ 88,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank53Anim03Seq),
        /* flags */ 0,
    },
    /* 4 */ {
        /* seq */ gSpriteBank53Anim04Seq,
        /* box */ { { -71, -50, 70, 64 }, { -71, -50, 102, 64 } },
        /* paletteId */ 88,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank53Anim04Seq),
        /* flags */ 0,
    },
    /* 5 */ {
        /* seq */ gSpriteBank53Anim05Seq,
        /* box */ { { -68, -61, 70, 75 }, { -68, -61, 102, 75 } },
        /* paletteId */ 88,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank53Anim05Seq),
        /* flags */ 0,
    },
    /* 6 */ {
        /* seq */ gSpriteBank53Anim06Seq,
        /* box */ { { -7, -43, 15, 87 }, { -7, -43, 15, 87 } },
        /* paletteId */ 89,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank53Anim06Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 7 */ {
        /* seq */ gSpriteBank53Anim07Seq,
        /* box */ { { -7, -43, 15, 87 }, { -7, -43, 15, 87 } },
        /* paletteId */ 87,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank53Anim07Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 8 */ {
        /* seq */ gSpriteBank53Anim08Seq,
        /* box */ { { -7, -43, 15, 87 }, { -7, -43, 15, 87 } },
        /* paletteId */ 59,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank53Anim08Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 9 */ {
        /* seq */ gSpriteBank53Anim09Seq,
        /* box */ { { -20, -9, 41, 18 }, { -12, -12, 25, 23 } },
        /* paletteId */ 53,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank53Anim09Seq),
        /* flags */ 0,
    },
    /* 10 */ {
        /* seq */ gSpriteBank53Anim10Seq,
        /* box */ { { -18, -11, 37, 23 }, { -12, -11, 25, 23 } },
        /* paletteId */ 50,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank53Anim10Seq),
        /* flags */ 0,
    },
    /* 11 */ {
        /* seq */ gSpriteBank53Anim11Seq,
        /* box */ { { -23, -8, 46, 16 }, { -12, -10, 25, 23 } },
        /* paletteId */ 51,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank53Anim11Seq),
        /* flags */ 0,
    },
    /* 12 */ {
        /* seq */ gSpriteBank53Anim12Seq,
        /* box */ { { -17, -11, 35, 22 }, { -12, -11, 25, 23 } },
        /* paletteId */ 52,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank53Anim12Seq),
        /* flags */ 0,
    },
    /* 13 */ {
        /* seq */ gSpriteBank53Anim13Seq,
        /* box */ { { -13, -12, 27, 25 }, { -12, -11, 25, 23 } },
        /* paletteId */ 49,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank53Anim13Seq),
        /* flags */ 0,
    },
    /* 14 */ {
        /* seq */ gSpriteBank53Anim14Seq,
        /* box */ { { -6, -6, 12, 12 }, { -7, -7, 14, 14 } },
        /* paletteId */ 95,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank53Anim14Seq),
        /* flags */ 0,
    },
    /* 15 */ {
        /* seq */ gSpriteBank53Anim15Seq,
        /* box */ { { -24, -24, 49, 49 }, { -24, -24, 49, 49 } },
        /* paletteId */ 95,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank53Anim15Seq),
        /* flags */ 0,
    },
    /* 16 */ {
        /* seq */ gSpriteBank53Anim16Seq,
        /* box */ { { -24, -24, 49, 49 }, { -24, -24, 49, 49 } },
        /* paletteId */ 95,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank53Anim16Seq),
        /* flags */ 0,
    },
    /* 17 */ {
        /* seq */ gSpriteBank53Anim17Seq,
        /* box */ { { -14, -15, 28, 30 }, { -14, -15, 28, 30 } },
        /* paletteId */ 95,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank53Anim17Seq),
        /* flags */ 0,
    },
    /* 18 */ {
        /* seq */ gSpriteBank53Anim18Seq,
        /* box */ { { -24, -24, 49, 49 }, { -29, -24, 60, 49 } },
        /* paletteId */ 95,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank53Anim18Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank53Anim00Seq[1] = {
    0,
};
const u16 gSpriteBank53Anim01Seq[6] = {
    1, 2, 3, 4, 5, 6,
};
const u16 gSpriteBank53Anim02Seq[1] = {
    7,
};
const u16 gSpriteBank53Anim03Seq[6] = {
    8, 9, 10, 11, 12, 13,
};
const u16 gSpriteBank53Anim04Seq[6] = {
    14, 15, 16, 17, 18, 19,
};
const u16 gSpriteBank53Anim05Seq[6] = {
    20, 21, 22, 23, 24, 25,
};
const u16 gSpriteBank53Anim06Seq[1] = {
    26,
};
const u16 gSpriteBank53Anim07Seq[1] = {
    27,
};
const u16 gSpriteBank53Anim08Seq[1] = {
    28,
};
const u16 gSpriteBank53Anim09Seq[5] = {
    29, 30, 31, 32, 33,
};
const u16 gSpriteBank53Anim10Seq[5] = {
    34, 35, 36, 37, 38,
};
const u16 gSpriteBank53Anim11Seq[5] = {
    39, 40, 41, 42, 43,
};
const u16 gSpriteBank53Anim12Seq[5] = {
    44, 45, 46, 47, 48,
};
const u16 gSpriteBank53Anim13Seq[5] = {
    49, 50, 51, 52, 53,
};
const u16 gSpriteBank53Anim14Seq[4] = {
    54, 55, 56, 57,
};
const u16 gSpriteBank53Anim15Seq[1] = {
    58,
};
const u16 gSpriteBank53Anim16Seq[2] = {
    59, 60,
};
const u16 gSpriteBank53Anim17Seq[6] = {
    61, 62, 63, 64, 63, 62,
};
const u16 gSpriteBank53Anim18Seq[3] = {
    60, 65, 66,
};

extern const struct sprite_frame *const gSpriteBank53Frames[67] = {
    &gSpriteBank53Frame000.frame,
    &gSpriteBank53Frame001.frame,
    &gSpriteBank53Frame002.frame,
    &gSpriteBank53Frame003.frame,
    &gSpriteBank53Frame004.frame,
    &gSpriteBank53Frame005.frame,
    &gSpriteBank53Frame006.frame,
    &gSpriteBank53Frame007.frame,
    &gSpriteBank53Frame008.frame,
    &gSpriteBank53Frame009.frame,
    &gSpriteBank53Frame010.frame,
    &gSpriteBank53Frame011.frame,
    &gSpriteBank53Frame012.frame,
    &gSpriteBank53Frame013.frame,
    &gSpriteBank53Frame014.frame,
    &gSpriteBank53Frame015.frame,
    &gSpriteBank53Frame016.frame,
    &gSpriteBank53Frame017.frame,
    &gSpriteBank53Frame018.frame,
    &gSpriteBank53Frame019.frame,
    &gSpriteBank53Frame020.frame,
    &gSpriteBank53Frame021.frame,
    &gSpriteBank53Frame022.frame,
    &gSpriteBank53Frame023.frame,
    &gSpriteBank53Frame024.frame,
    &gSpriteBank53Frame025.frame,
    &gSpriteBank53Frame026.frame,
    &gSpriteBank53Frame027.frame,
    &gSpriteBank53Frame028.frame,
    &gSpriteBank53Frame029.frame,
    &gSpriteBank53Frame030.frame,
    &gSpriteBank53Frame031.frame,
    &gSpriteBank53Frame032.frame,
    &gSpriteBank53Frame033.frame,
    &gSpriteBank53Frame034.frame,
    &gSpriteBank53Frame035.frame,
    &gSpriteBank53Frame036.frame,
    &gSpriteBank53Frame037.frame,
    &gSpriteBank53Frame038.frame,
    &gSpriteBank53Frame039.frame,
    &gSpriteBank53Frame040.frame,
    &gSpriteBank53Frame041.frame,
    &gSpriteBank53Frame042.frame,
    &gSpriteBank53Frame043.frame,
    &gSpriteBank53Frame044.frame,
    &gSpriteBank53Frame045.frame,
    &gSpriteBank53Frame046.frame,
    &gSpriteBank53Frame047.frame,
    &gSpriteBank53Frame048.frame,
    &gSpriteBank53Frame049.frame,
    &gSpriteBank53Frame050.frame,
    &gSpriteBank53Frame051.frame,
    &gSpriteBank53Frame052.frame,
    &gSpriteBank53Frame053.frame,
    &gSpriteBank53Frame054.frame,
    &gSpriteBank53Frame055.frame,
    &gSpriteBank53Frame056.frame,
    &gSpriteBank53Frame057.frame,
    &gSpriteBank53Frame058.frame,
    &gSpriteBank53Frame059.frame,
    &gSpriteBank53Frame060.frame,
    &gSpriteBank53Frame061.frame,
    &gSpriteBank53Frame062.frame,
    &gSpriteBank53Frame063.frame,
    &gSpriteBank53Frame064.frame,
    &gSpriteBank53Frame065.frame,
    &gSpriteBank53Frame066.frame,
};

const struct sprite_frame_1box gSpriteBank53Frame000 = {
    SPRITE_FRAME(gSpriteBank53Frame000, SPRITE_TILES_BANK53_FRAME000),
    { { -7, -43, 15, 87 } },
};
const struct sprite_frame_1box gSpriteBank53Frame001 = {
    SPRITE_FRAME(gSpriteBank53Frame001, SPRITE_TILES_BANK53_FRAME001),
    { { -31, -16, 52, 32 } },
};
const struct sprite_frame_1box gSpriteBank53Frame002 = {
    SPRITE_FRAME(gSpriteBank53Frame002, SPRITE_TILES_BANK53_FRAME002),
    { { -31, -16, 53, 32 } },
};
const struct sprite_frame_1box gSpriteBank53Frame003 = {
    SPRITE_FRAME(gSpriteBank53Frame003, SPRITE_TILES_BANK53_FRAME003),
    { { -31, -16, 55, 32 } },
};
const struct sprite_frame_1box gSpriteBank53Frame004 = {
    SPRITE_FRAME(gSpriteBank53Frame004, SPRITE_TILES_BANK53_FRAME004),
    { { -31, -16, 59, 32 } },
};
const struct sprite_frame_1box gSpriteBank53Frame005 = {
    SPRITE_FRAME(gSpriteBank53Frame005, SPRITE_TILES_BANK53_FRAME005),
    { { -31, -16, 61, 32 } },
};
const struct sprite_frame_1box gSpriteBank53Frame006 = {
    SPRITE_FRAME(gSpriteBank53Frame006, SPRITE_TILES_BANK53_FRAME006),
    { { -31, -16, 62, 32 } },
};
const struct sprite_frame_1box gSpriteBank53Frame007 = {
    SPRITE_FRAME(gSpriteBank53Frame007, SPRITE_TILES_BANK53_FRAME007),
    { { -7, -43, 15, 87 } },
};
const struct sprite_frame_1box gSpriteBank53Frame008 = {
    SPRITE_FRAME(gSpriteBank53Frame008, SPRITE_TILES_BANK53_FRAME008),
    { { -72, -35, 68, 49 } },
};
const struct sprite_frame_1box gSpriteBank53Frame009 = {
    SPRITE_FRAME(gSpriteBank53Frame009, SPRITE_TILES_BANK53_FRAME009),
    { { -67, -35, 66, 49 } },
};
const struct sprite_frame_1box gSpriteBank53Frame010 = {
    SPRITE_FRAME(gSpriteBank53Frame010, SPRITE_TILES_BANK53_FRAME010),
    { { -54, -35, 60, 49 } },
};
const struct sprite_frame_1box gSpriteBank53Frame011 = {
    SPRITE_FRAME(gSpriteBank53Frame011, SPRITE_TILES_BANK53_FRAME011),
    { { -47, -35, 63, 49 } },
};
const struct sprite_frame_1box gSpriteBank53Frame012 = {
    SPRITE_FRAME(gSpriteBank53Frame012, SPRITE_TILES_BANK53_FRAME012),
    { { -39, -35, 64, 49 } },
};
const struct sprite_frame_1box gSpriteBank53Frame013 = {
    SPRITE_FRAME(gSpriteBank53Frame013, SPRITE_TILES_BANK53_FRAME013),
    { { -33, -36, 65, 50 } },
};
const struct sprite_frame_1box gSpriteBank53Frame014 = {
    SPRITE_FRAME(gSpriteBank53Frame014, SPRITE_TILES_BANK53_FRAME014),
    { { -71, -50, 70, 64 } },
};
const struct sprite_frame_1box gSpriteBank53Frame015 = {
    SPRITE_FRAME(gSpriteBank53Frame015, SPRITE_TILES_BANK53_FRAME015),
    { { -65, -50, 67, 64 } },
};
const struct sprite_frame_1box gSpriteBank53Frame016 = {
    SPRITE_FRAME(gSpriteBank53Frame016, SPRITE_TILES_BANK53_FRAME016),
    { { -55, -49, 64, 63 } },
};
const struct sprite_frame_1box gSpriteBank53Frame017 = {
    SPRITE_FRAME(gSpriteBank53Frame017, SPRITE_TILES_BANK53_FRAME017),
    { { -47, -48, 66, 62 } },
};
const struct sprite_frame_1box gSpriteBank53Frame018 = {
    SPRITE_FRAME(gSpriteBank53Frame018, SPRITE_TILES_BANK53_FRAME018),
    { { -40, -47, 67, 61 } },
};
const struct sprite_frame_1box gSpriteBank53Frame019 = {
    SPRITE_FRAME(gSpriteBank53Frame019, SPRITE_TILES_BANK53_FRAME019),
    { { -37, -46, 68, 60 } },
};
const struct sprite_frame_1box gSpriteBank53Frame020 = {
    SPRITE_FRAME(gSpriteBank53Frame020, SPRITE_TILES_BANK53_FRAME020),
    { { -68, -61, 70, 75 } },
};
const struct sprite_frame_1box gSpriteBank53Frame021 = {
    SPRITE_FRAME(gSpriteBank53Frame021, SPRITE_TILES_BANK53_FRAME021),
    { { -64, -61, 69, 75 } },
};
const struct sprite_frame_1box gSpriteBank53Frame022 = {
    SPRITE_FRAME(gSpriteBank53Frame022, SPRITE_TILES_BANK53_FRAME022),
    { { -55, -60, 65, 74 } },
};
const struct sprite_frame_1box gSpriteBank53Frame023 = {
    SPRITE_FRAME(gSpriteBank53Frame023, SPRITE_TILES_BANK53_FRAME023),
    { { -49, -60, 66, 74 } },
};
const struct sprite_frame_1box gSpriteBank53Frame024 = {
    SPRITE_FRAME(gSpriteBank53Frame024, SPRITE_TILES_BANK53_FRAME024),
    { { -42, -59, 68, 73 } },
};
const struct sprite_frame_1box gSpriteBank53Frame025 = {
    SPRITE_FRAME(gSpriteBank53Frame025, SPRITE_TILES_BANK53_FRAME025),
    { { -35, -59, 69, 73 } },
};
const struct sprite_frame_1box gSpriteBank53Frame026 = {
    SPRITE_FRAME(gSpriteBank53Frame026, SPRITE_TILES_BANK53_FRAME026),
    { { -7, -43, 15, 87 } },
};
const struct sprite_frame_1box gSpriteBank53Frame027 = {
    SPRITE_FRAME(gSpriteBank53Frame027, SPRITE_TILES_BANK53_FRAME027),
    { { -7, -43, 15, 87 } },
};
const struct sprite_frame_1box gSpriteBank53Frame028 = {
    SPRITE_FRAME(gSpriteBank53Frame028, SPRITE_TILES_BANK53_FRAME028),
    { { -7, -43, 15, 87 } },
};
const struct sprite_frame_1box gSpriteBank53Frame029 = {
    SPRITE_FRAME(gSpriteBank53Frame029, SPRITE_TILES_BANK53_FRAME029),
    { { -20, -9, 41, 18 } },
};
const struct sprite_frame_1box gSpriteBank53Frame030 = {
    SPRITE_FRAME(gSpriteBank53Frame030, SPRITE_TILES_BANK53_FRAME030),
    { { -16, -7, 33, 14 } },
};
const struct sprite_frame_1box gSpriteBank53Frame031 = {
    SPRITE_FRAME(gSpriteBank53Frame031, SPRITE_TILES_BANK53_FRAME031),
    { { -12, -5, 25, 10 } },
};
const struct sprite_frame_1box gSpriteBank53Frame032 = {
    SPRITE_FRAME(gSpriteBank53Frame032, SPRITE_TILES_BANK53_FRAME032),
    { { -5, -2, 10, 4 } },
};
const struct sprite_frame_1box gSpriteBank53Frame033 = {
    SPRITE_FRAME(gSpriteBank53Frame033, SPRITE_TILES_BANK53_FRAME033),
    { { -1, -1, 3, 1 } },
};
const struct sprite_frame_1box gSpriteBank53Frame034 = {
    SPRITE_FRAME(gSpriteBank53Frame034, SPRITE_TILES_BANK53_FRAME034),
    { { -18, -11, 37, 23 } },
};
const struct sprite_frame_1box gSpriteBank53Frame035 = {
    SPRITE_FRAME(gSpriteBank53Frame035, SPRITE_TILES_BANK53_FRAME035),
    { { -16, -10, 33, 21 } },
};
const struct sprite_frame_1box gSpriteBank53Frame036 = {
    SPRITE_FRAME(gSpriteBank53Frame036, SPRITE_TILES_BANK53_FRAME036),
    { { -12, -7, 25, 15 } },
};
const struct sprite_frame_1box gSpriteBank53Frame037 = {
    SPRITE_FRAME(gSpriteBank53Frame037, SPRITE_TILES_BANK53_FRAME037),
    { { -7, -5, 15, 11 } },
};
const struct sprite_frame_1box gSpriteBank53Frame038 = {
    SPRITE_FRAME(gSpriteBank53Frame038, SPRITE_TILES_BANK53_FRAME038),
    { { -2, -1, 5, 3 } },
};
const struct sprite_frame_1box gSpriteBank53Frame039 = {
    SPRITE_FRAME(gSpriteBank53Frame039, SPRITE_TILES_BANK53_FRAME039),
    { { -23, -8, 46, 16 } },
};
const struct sprite_frame_1box gSpriteBank53Frame040 = {
    SPRITE_FRAME(gSpriteBank53Frame040, SPRITE_TILES_BANK53_FRAME040),
    { { -18, -6, 36, 13 } },
};
const struct sprite_frame_1box gSpriteBank53Frame041 = {
    SPRITE_FRAME(gSpriteBank53Frame041, SPRITE_TILES_BANK53_FRAME041),
    { { -13, -5, 28, 11 } },
};
const struct sprite_frame_1box gSpriteBank53Frame042 = {
    SPRITE_FRAME(gSpriteBank53Frame042, SPRITE_TILES_BANK53_FRAME042),
    { { -8, -3, 17, 7 } },
};
const struct sprite_frame_1box gSpriteBank53Frame043 = {
    SPRITE_FRAME(gSpriteBank53Frame043, SPRITE_TILES_BANK53_FRAME043),
    { { -2, 0, 6, 1 } },
};
const struct sprite_frame_1box gSpriteBank53Frame044 = {
    SPRITE_FRAME(gSpriteBank53Frame044, SPRITE_TILES_BANK53_FRAME044),
    { { -17, -11, 35, 22 } },
};
const struct sprite_frame_1box gSpriteBank53Frame045 = {
    SPRITE_FRAME(gSpriteBank53Frame045, SPRITE_TILES_BANK53_FRAME045),
    { { -14, -9, 29, 18 } },
};
const struct sprite_frame_1box gSpriteBank53Frame046 = {
    SPRITE_FRAME(gSpriteBank53Frame046, SPRITE_TILES_BANK53_FRAME046),
    { { -11, -7, 23, 14 } },
};
const struct sprite_frame_1box gSpriteBank53Frame047 = {
    SPRITE_FRAME(gSpriteBank53Frame047, SPRITE_TILES_BANK53_FRAME047),
    { { -7, -5, 15, 10 } },
};
const struct sprite_frame_1box gSpriteBank53Frame048 = {
    SPRITE_FRAME(gSpriteBank53Frame048, SPRITE_TILES_BANK53_FRAME048),
    { { -1, -1, 3, 2 } },
};
const struct sprite_frame_1box gSpriteBank53Frame049 = {
    SPRITE_FRAME(gSpriteBank53Frame049, SPRITE_TILES_BANK53_FRAME049),
    { { -13, -12, 27, 25 } },
};
const struct sprite_frame_1box gSpriteBank53Frame050 = {
    SPRITE_FRAME(gSpriteBank53Frame050, SPRITE_TILES_BANK53_FRAME050),
    { { -11, -10, 23, 21 } },
};
const struct sprite_frame_1box gSpriteBank53Frame051 = {
    SPRITE_FRAME(gSpriteBank53Frame051, SPRITE_TILES_BANK53_FRAME051),
    { { -9, -8, 19, 17 } },
};
const struct sprite_frame_1box gSpriteBank53Frame052 = {
    SPRITE_FRAME(gSpriteBank53Frame052, SPRITE_TILES_BANK53_FRAME052),
    { { -4, -4, 10, 9 } },
};
const struct sprite_frame_1box gSpriteBank53Frame053 = {
    SPRITE_FRAME(gSpriteBank53Frame053, SPRITE_TILES_BANK53_FRAME053),
    { { -1, -1, 3, 3 } },
};
const struct sprite_frame_2box gSpriteBank53Frame054 = {
    SPRITE_FRAME(gSpriteBank53Frame054, SPRITE_TILES_BANK53_FRAME054),
    { { -6, -6, 12, 12 }, { -4, -4, 9, 9 } },
};
const struct sprite_frame_2box gSpriteBank53Frame055 = {
    SPRITE_FRAME(gSpriteBank53Frame055, SPRITE_TILES_BANK53_FRAME055),
    { { -7, -7, 14, 14 }, { -4, -4, 9, 9 } },
};
const struct sprite_frame_2box gSpriteBank53Frame056 = {
    SPRITE_FRAME(gSpriteBank53Frame056, SPRITE_TILES_BANK53_FRAME056),
    { { -4, -4, 8, 8 }, { -4, -4, 9, 9 } },
};
const struct sprite_frame_2box gSpriteBank53Frame057 = {
    SPRITE_FRAME(gSpriteBank53Frame057, SPRITE_TILES_BANK53_FRAME057),
    { { -2, -2, 4, 4 }, { -4, -4, 9, 9 } },
};
const struct sprite_frame_1box gSpriteBank53Frame058 = {
    SPRITE_FRAME(gSpriteBank53Frame058, SPRITE_TILES_BANK53_FRAME058),
    { { -24, -24, 49, 49 } },
};
const struct sprite_frame_1box gSpriteBank53Frame059 = {
    SPRITE_FRAME(gSpriteBank53Frame059, SPRITE_TILES_BANK53_FRAME059),
    { { -24, -24, 49, 49 } },
};
const struct sprite_frame_1box gSpriteBank53Frame060 = {
    SPRITE_FRAME(gSpriteBank53Frame060, SPRITE_TILES_BANK53_FRAME060),
    { { -24, -24, 49, 49 } },
};
const struct sprite_frame_2box gSpriteBank53Frame061 = {
    SPRITE_FRAME(gSpriteBank53Frame061, SPRITE_TILES_BANK53_FRAME061),
    { { -14, -15, 28, 30 }, { -7, -7, 15, 16 } },
};
const struct sprite_frame_2box gSpriteBank53Frame062 = {
    SPRITE_FRAME(gSpriteBank53Frame062, SPRITE_TILES_BANK53_FRAME062),
    { { -12, -13, 24, 26 }, { -7, -7, 15, 16 } },
};
const struct sprite_frame_2box gSpriteBank53Frame063 = {
    SPRITE_FRAME(gSpriteBank53Frame063, SPRITE_TILES_BANK53_FRAME063),
    { { -11, -10, 22, 20 }, { -7, -7, 15, 16 } },
};
const struct sprite_frame_2box gSpriteBank53Frame064 = {
    SPRITE_FRAME(gSpriteBank53Frame064, SPRITE_TILES_BANK53_FRAME064),
    { { -9, -9, 18, 18 }, { -7, -7, 15, 16 } },
};
const struct sprite_frame_1box gSpriteBank53Frame065 = {
    SPRITE_FRAME(gSpriteBank53Frame065, SPRITE_TILES_BANK53_FRAME065),
    { { -29, -18, 60, 38 } },
};
const struct sprite_frame_1box gSpriteBank53Frame066 = {
    SPRITE_FRAME(gSpriteBank53Frame066, SPRITE_TILES_BANK53_FRAME066),
    { { -29, -14, 60, 31 } },
};

/* The piece arrays, from graphics/sprites/bank53_cortex/bank.json. */
#include "sprites/bank53_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 54: 10 animations, 121 frames, graphics/sprites/bank54_dingodile/. */

extern const u16 gSpriteBank54Anim00Seq[16];
extern const u16 gSpriteBank54Anim01Seq[15];
extern const u16 gSpriteBank54Anim02Seq[13];
extern const u16 gSpriteBank54Anim03Seq[8];
extern const u16 gSpriteBank54Anim04Seq[33];
extern const u16 gSpriteBank54Anim05Seq[15];
extern const u16 gSpriteBank54Anim06Seq[12];
extern const u16 gSpriteBank54Anim07Seq[10];
extern const u16 gSpriteBank54Anim08Seq[6];
extern const u16 gSpriteBank54Anim09Seq[1];

extern const struct sprite_anim gSpriteBank54Anims[10] = {
    /* 0 */ {
        /* seq */ gSpriteBank54Anim00Seq,
        /* box */ { { -18, -27, 37, 57 }, { -43, -36, 78, 69 } },
        /* paletteId */ 96,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank54Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank54Anim01Seq,
        /* box */ { { -18, -27, 37, 57 }, { -43, -37, 78, 70 } },
        /* paletteId */ 96,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank54Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank54Anim02Seq,
        /* box */ { { -18, -27, 37, 57 }, { -43, -32, 78, 64 } },
        /* paletteId */ 96,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank54Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank54Anim03Seq,
        /* box */ { { -47, -47, 95, 95 }, { -47, -47, 95, 95 } },
        /* paletteId */ 97,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank54Anim03Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 4 */ {
        /* seq */ gSpriteBank54Anim04Seq,
        /* box */ { { -18, -27, 37, 57 }, { -49, -52, 84, 84 } },
        /* paletteId */ 96,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank54Anim04Seq),
        /* flags */ 0,
    },
    /* 5 */ {
        /* seq */ gSpriteBank54Anim05Seq,
        /* box */ { { -18, -27, 37, 57 }, { -43, -33, 80, 65 } },
        /* paletteId */ 96,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank54Anim05Seq),
        /* flags */ 0,
    },
    /* 6 */ {
        /* seq */ gSpriteBank54Anim06Seq,
        /* box */ { { -18, -27, 37, 57 }, { -41, -36, 76, 69 } },
        /* paletteId */ 96,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank54Anim06Seq),
        /* flags */ 0,
    },
    /* 7 */ {
        /* seq */ gSpriteBank54Anim07Seq,
        /* box */ { { -4, -10, 9, 21 }, { -5, -10, 11, 21 } },
        /* paletteId */ 98,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank54Anim07Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 8 */ {
        /* seq */ gSpriteBank54Anim08Seq,
        /* box */ { { -12, -23, 25, 47 }, { -29, -33, 60, 57 } },
        /* paletteId */ 99,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank54Anim08Seq),
        /* flags */ 0,
    },
    /* 9 */ {
        /* seq */ gSpriteBank54Anim09Seq,
        /* box */ { { -9, -26, 19, 60 }, { -9, -26, 19, 60 } },
        /* paletteId */ 99,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank54Anim09Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank54Anim00Seq[16] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
};
const u16 gSpriteBank54Anim01Seq[15] = {
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
const u16 gSpriteBank54Anim02Seq[13] = {
    31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
};
const u16 gSpriteBank54Anim03Seq[8] = {
    44, 45, 46, 47, 48, 49, 50, 51,
};
const u16 gSpriteBank54Anim04Seq[33] = {
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67,
    68, 69, 70, 71, 72, 73, 74, 61, 75, 60, 76, 57, 56, 55, 54, 53,
    52,
};
const u16 gSpriteBank54Anim05Seq[15] = {
    77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91,
};
const u16 gSpriteBank54Anim06Seq[12] = {
    92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103,
};
const u16 gSpriteBank54Anim07Seq[10] = {
    104, 105, 106, 107, 108, 109, 110, 111, 112, 113,
};
const u16 gSpriteBank54Anim08Seq[6] = {
    114, 115, 116, 117, 118, 119,
};
const u16 gSpriteBank54Anim09Seq[1] = {
    120,
};

extern const struct sprite_frame *const gSpriteBank54Frames[121] = {
    &gSpriteBank54Frame000,
    &gSpriteBank54Frame001,
    &gSpriteBank54Frame002,
    &gSpriteBank54Frame003,
    &gSpriteBank54Frame004,
    &gSpriteBank54Frame005,
    &gSpriteBank54Frame006,
    &gSpriteBank54Frame007,
    &gSpriteBank54Frame008,
    &gSpriteBank54Frame009,
    &gSpriteBank54Frame010,
    &gSpriteBank54Frame011,
    &gSpriteBank54Frame012,
    &gSpriteBank54Frame013,
    &gSpriteBank54Frame014,
    &gSpriteBank54Frame015,
    &gSpriteBank54Frame016,
    &gSpriteBank54Frame017,
    &gSpriteBank54Frame018,
    &gSpriteBank54Frame019,
    &gSpriteBank54Frame020,
    &gSpriteBank54Frame021,
    &gSpriteBank54Frame022,
    &gSpriteBank54Frame023,
    &gSpriteBank54Frame024,
    &gSpriteBank54Frame025,
    &gSpriteBank54Frame026,
    &gSpriteBank54Frame027,
    &gSpriteBank54Frame028,
    &gSpriteBank54Frame029,
    &gSpriteBank54Frame030,
    &gSpriteBank54Frame031,
    &gSpriteBank54Frame032,
    &gSpriteBank54Frame033,
    &gSpriteBank54Frame034,
    &gSpriteBank54Frame035,
    &gSpriteBank54Frame036,
    &gSpriteBank54Frame037,
    &gSpriteBank54Frame038,
    &gSpriteBank54Frame039,
    &gSpriteBank54Frame040,
    &gSpriteBank54Frame041,
    &gSpriteBank54Frame042,
    &gSpriteBank54Frame043,
    &gSpriteBank54Frame044.frame,
    &gSpriteBank54Frame045.frame,
    &gSpriteBank54Frame046.frame,
    &gSpriteBank54Frame047.frame,
    &gSpriteBank54Frame048.frame,
    &gSpriteBank54Frame049.frame,
    &gSpriteBank54Frame050.frame,
    &gSpriteBank54Frame051.frame,
    &gSpriteBank54Frame052,
    &gSpriteBank54Frame053,
    &gSpriteBank54Frame054,
    &gSpriteBank54Frame055,
    &gSpriteBank54Frame056,
    &gSpriteBank54Frame057,
    &gSpriteBank54Frame058,
    &gSpriteBank54Frame059,
    &gSpriteBank54Frame060,
    &gSpriteBank54Frame061,
    &gSpriteBank54Frame062,
    &gSpriteBank54Frame063,
    &gSpriteBank54Frame064,
    &gSpriteBank54Frame065,
    &gSpriteBank54Frame066,
    &gSpriteBank54Frame067,
    &gSpriteBank54Frame068,
    &gSpriteBank54Frame069,
    &gSpriteBank54Frame070,
    &gSpriteBank54Frame071,
    &gSpriteBank54Frame072,
    &gSpriteBank54Frame073,
    &gSpriteBank54Frame074,
    &gSpriteBank54Frame075,
    &gSpriteBank54Frame076,
    &gSpriteBank54Frame077.frame,
    &gSpriteBank54Frame078.frame,
    &gSpriteBank54Frame079.frame,
    &gSpriteBank54Frame080.frame,
    &gSpriteBank54Frame081.frame,
    &gSpriteBank54Frame082.frame,
    &gSpriteBank54Frame083.frame,
    &gSpriteBank54Frame084.frame,
    &gSpriteBank54Frame085.frame,
    &gSpriteBank54Frame086.frame,
    &gSpriteBank54Frame087.frame,
    &gSpriteBank54Frame088.frame,
    &gSpriteBank54Frame089.frame,
    &gSpriteBank54Frame090.frame,
    &gSpriteBank54Frame091.frame,
    &gSpriteBank54Frame092,
    &gSpriteBank54Frame093,
    &gSpriteBank54Frame094,
    &gSpriteBank54Frame095,
    &gSpriteBank54Frame096,
    &gSpriteBank54Frame097,
    &gSpriteBank54Frame098,
    &gSpriteBank54Frame099,
    &gSpriteBank54Frame100,
    &gSpriteBank54Frame101,
    &gSpriteBank54Frame102,
    &gSpriteBank54Frame103,
    &gSpriteBank54Frame104.frame,
    &gSpriteBank54Frame105.frame,
    &gSpriteBank54Frame106.frame,
    &gSpriteBank54Frame107.frame,
    &gSpriteBank54Frame108.frame,
    &gSpriteBank54Frame109.frame,
    &gSpriteBank54Frame110.frame,
    &gSpriteBank54Frame111.frame,
    &gSpriteBank54Frame112.frame,
    &gSpriteBank54Frame113.frame,
    &gSpriteBank54Frame114,
    &gSpriteBank54Frame115,
    &gSpriteBank54Frame116,
    &gSpriteBank54Frame117,
    &gSpriteBank54Frame118,
    &gSpriteBank54Frame119,
    &gSpriteBank54Frame120.frame,
};

const struct sprite_frame gSpriteBank54Frame000 = SPRITE_FRAME(gSpriteBank54Frame000, SPRITE_TILES_BANK54_FRAME000);
const struct sprite_frame gSpriteBank54Frame001 = SPRITE_FRAME(gSpriteBank54Frame001, SPRITE_TILES_BANK54_FRAME001);
const struct sprite_frame gSpriteBank54Frame002 = SPRITE_FRAME(gSpriteBank54Frame002, SPRITE_TILES_BANK54_FRAME002);
const struct sprite_frame gSpriteBank54Frame003 = SPRITE_FRAME(gSpriteBank54Frame003, SPRITE_TILES_BANK54_FRAME003);
const struct sprite_frame gSpriteBank54Frame004 = SPRITE_FRAME(gSpriteBank54Frame004, SPRITE_TILES_BANK54_FRAME004);
const struct sprite_frame gSpriteBank54Frame005 = SPRITE_FRAME(gSpriteBank54Frame005, SPRITE_TILES_BANK54_FRAME005);
const struct sprite_frame gSpriteBank54Frame006 = SPRITE_FRAME(gSpriteBank54Frame006, SPRITE_TILES_BANK54_FRAME006);
const struct sprite_frame gSpriteBank54Frame007 = SPRITE_FRAME(gSpriteBank54Frame007, SPRITE_TILES_BANK54_FRAME007);
const struct sprite_frame gSpriteBank54Frame008 = SPRITE_FRAME(gSpriteBank54Frame008, SPRITE_TILES_BANK54_FRAME008);
const struct sprite_frame gSpriteBank54Frame009 = SPRITE_FRAME(gSpriteBank54Frame009, SPRITE_TILES_BANK54_FRAME009);
const struct sprite_frame gSpriteBank54Frame010 = SPRITE_FRAME(gSpriteBank54Frame010, SPRITE_TILES_BANK54_FRAME010);
const struct sprite_frame gSpriteBank54Frame011 = SPRITE_FRAME(gSpriteBank54Frame011, SPRITE_TILES_BANK54_FRAME011);
const struct sprite_frame gSpriteBank54Frame012 = SPRITE_FRAME(gSpriteBank54Frame012, SPRITE_TILES_BANK54_FRAME012);
const struct sprite_frame gSpriteBank54Frame013 = SPRITE_FRAME(gSpriteBank54Frame013, SPRITE_TILES_BANK54_FRAME013);
const struct sprite_frame gSpriteBank54Frame014 = SPRITE_FRAME(gSpriteBank54Frame014, SPRITE_TILES_BANK54_FRAME014);
const struct sprite_frame gSpriteBank54Frame015 = SPRITE_FRAME(gSpriteBank54Frame015, SPRITE_TILES_BANK54_FRAME015);
const struct sprite_frame gSpriteBank54Frame016 = SPRITE_FRAME(gSpriteBank54Frame016, SPRITE_TILES_BANK54_FRAME016);
const struct sprite_frame gSpriteBank54Frame017 = SPRITE_FRAME(gSpriteBank54Frame017, SPRITE_TILES_BANK54_FRAME017);
const struct sprite_frame gSpriteBank54Frame018 = SPRITE_FRAME(gSpriteBank54Frame018, SPRITE_TILES_BANK54_FRAME018);
const struct sprite_frame gSpriteBank54Frame019 = SPRITE_FRAME(gSpriteBank54Frame019, SPRITE_TILES_BANK54_FRAME019);
const struct sprite_frame gSpriteBank54Frame020 = SPRITE_FRAME(gSpriteBank54Frame020, SPRITE_TILES_BANK54_FRAME020);
const struct sprite_frame gSpriteBank54Frame021 = SPRITE_FRAME(gSpriteBank54Frame021, SPRITE_TILES_BANK54_FRAME021);
const struct sprite_frame gSpriteBank54Frame022 = SPRITE_FRAME(gSpriteBank54Frame022, SPRITE_TILES_BANK54_FRAME022);
const struct sprite_frame gSpriteBank54Frame023 = SPRITE_FRAME(gSpriteBank54Frame023, SPRITE_TILES_BANK54_FRAME023);
const struct sprite_frame gSpriteBank54Frame024 = SPRITE_FRAME(gSpriteBank54Frame024, SPRITE_TILES_BANK54_FRAME024);
const struct sprite_frame gSpriteBank54Frame025 = SPRITE_FRAME(gSpriteBank54Frame025, SPRITE_TILES_BANK54_FRAME025);
const struct sprite_frame gSpriteBank54Frame026 = SPRITE_FRAME(gSpriteBank54Frame026, SPRITE_TILES_BANK54_FRAME026);
const struct sprite_frame gSpriteBank54Frame027 = SPRITE_FRAME(gSpriteBank54Frame027, SPRITE_TILES_BANK54_FRAME027);
const struct sprite_frame gSpriteBank54Frame028 = SPRITE_FRAME(gSpriteBank54Frame028, SPRITE_TILES_BANK54_FRAME028);
const struct sprite_frame gSpriteBank54Frame029 = SPRITE_FRAME(gSpriteBank54Frame029, SPRITE_TILES_BANK54_FRAME029);
const struct sprite_frame gSpriteBank54Frame030 = SPRITE_FRAME(gSpriteBank54Frame030, SPRITE_TILES_BANK54_FRAME030);
const struct sprite_frame gSpriteBank54Frame031 = SPRITE_FRAME(gSpriteBank54Frame031, SPRITE_TILES_BANK54_FRAME031);
const struct sprite_frame gSpriteBank54Frame032 = SPRITE_FRAME(gSpriteBank54Frame032, SPRITE_TILES_BANK54_FRAME032);
const struct sprite_frame gSpriteBank54Frame033 = SPRITE_FRAME(gSpriteBank54Frame033, SPRITE_TILES_BANK54_FRAME033);
const struct sprite_frame gSpriteBank54Frame034 = SPRITE_FRAME(gSpriteBank54Frame034, SPRITE_TILES_BANK54_FRAME034);
const struct sprite_frame gSpriteBank54Frame035 = SPRITE_FRAME(gSpriteBank54Frame035, SPRITE_TILES_BANK54_FRAME035);
const struct sprite_frame gSpriteBank54Frame036 = SPRITE_FRAME(gSpriteBank54Frame036, SPRITE_TILES_BANK54_FRAME036);
const struct sprite_frame gSpriteBank54Frame037 = SPRITE_FRAME(gSpriteBank54Frame037, SPRITE_TILES_BANK54_FRAME037);
const struct sprite_frame gSpriteBank54Frame038 = SPRITE_FRAME(gSpriteBank54Frame038, SPRITE_TILES_BANK54_FRAME038);
const struct sprite_frame gSpriteBank54Frame039 = SPRITE_FRAME(gSpriteBank54Frame039, SPRITE_TILES_BANK54_FRAME039);
const struct sprite_frame gSpriteBank54Frame040 = SPRITE_FRAME(gSpriteBank54Frame040, SPRITE_TILES_BANK54_FRAME040);
const struct sprite_frame gSpriteBank54Frame041 = SPRITE_FRAME(gSpriteBank54Frame041, SPRITE_TILES_BANK54_FRAME041);
const struct sprite_frame gSpriteBank54Frame042 = SPRITE_FRAME(gSpriteBank54Frame042, SPRITE_TILES_BANK54_FRAME042);
const struct sprite_frame gSpriteBank54Frame043 = SPRITE_FRAME(gSpriteBank54Frame043, SPRITE_TILES_BANK54_FRAME043);
const struct sprite_frame_1box gSpriteBank54Frame044 = {
    SPRITE_FRAME(gSpriteBank54Frame044, SPRITE_TILES_BANK54_FRAME044),
    { { -31, -30, 62, 68 } },
};
const struct sprite_frame_1box gSpriteBank54Frame045 = {
    SPRITE_FRAME(gSpriteBank54Frame045, SPRITE_TILES_BANK54_FRAME045),
    { { -31, -30, 62, 68 } },
};
const struct sprite_frame_1box gSpriteBank54Frame046 = {
    SPRITE_FRAME(gSpriteBank54Frame046, SPRITE_TILES_BANK54_FRAME046),
    { { -31, -30, 62, 68 } },
};
const struct sprite_frame_1box gSpriteBank54Frame047 = {
    SPRITE_FRAME(gSpriteBank54Frame047, SPRITE_TILES_BANK54_FRAME047),
    { { -31, -30, 62, 68 } },
};
const struct sprite_frame_1box gSpriteBank54Frame048 = {
    SPRITE_FRAME(gSpriteBank54Frame048, SPRITE_TILES_BANK54_FRAME048),
    { { -31, -30, 62, 68 } },
};
const struct sprite_frame_1box gSpriteBank54Frame049 = {
    SPRITE_FRAME(gSpriteBank54Frame049, SPRITE_TILES_BANK54_FRAME049),
    { { -31, -30, 62, 68 } },
};
const struct sprite_frame_1box gSpriteBank54Frame050 = {
    SPRITE_FRAME(gSpriteBank54Frame050, SPRITE_TILES_BANK54_FRAME050),
    { { -31, -30, 62, 68 } },
};
const struct sprite_frame_1box gSpriteBank54Frame051 = {
    SPRITE_FRAME(gSpriteBank54Frame051, SPRITE_TILES_BANK54_FRAME051),
    { { -31, -30, 62, 68 } },
};
const struct sprite_frame gSpriteBank54Frame052 = SPRITE_FRAME(gSpriteBank54Frame052, SPRITE_TILES_BANK54_FRAME052);
const struct sprite_frame gSpriteBank54Frame053 = SPRITE_FRAME(gSpriteBank54Frame053, SPRITE_TILES_BANK54_FRAME053);
const struct sprite_frame gSpriteBank54Frame054 = SPRITE_FRAME(gSpriteBank54Frame054, SPRITE_TILES_BANK54_FRAME054);
const struct sprite_frame gSpriteBank54Frame055 = SPRITE_FRAME(gSpriteBank54Frame055, SPRITE_TILES_BANK54_FRAME055);
const struct sprite_frame gSpriteBank54Frame056 = SPRITE_FRAME(gSpriteBank54Frame056, SPRITE_TILES_BANK54_FRAME056);
const struct sprite_frame gSpriteBank54Frame057 = SPRITE_FRAME(gSpriteBank54Frame057, SPRITE_TILES_BANK54_FRAME057);
const struct sprite_frame gSpriteBank54Frame058 = SPRITE_FRAME(gSpriteBank54Frame058, SPRITE_TILES_BANK54_FRAME058);
const struct sprite_frame gSpriteBank54Frame059 = SPRITE_FRAME(gSpriteBank54Frame059, SPRITE_TILES_BANK54_FRAME059);
const struct sprite_frame gSpriteBank54Frame060 = SPRITE_FRAME(gSpriteBank54Frame060, SPRITE_TILES_BANK54_FRAME060);
const struct sprite_frame gSpriteBank54Frame061 = SPRITE_FRAME(gSpriteBank54Frame061, SPRITE_TILES_BANK54_FRAME061);
const struct sprite_frame gSpriteBank54Frame062 = SPRITE_FRAME(gSpriteBank54Frame062, SPRITE_TILES_BANK54_FRAME062);
const struct sprite_frame gSpriteBank54Frame063 = SPRITE_FRAME(gSpriteBank54Frame063, SPRITE_TILES_BANK54_FRAME063);
const struct sprite_frame gSpriteBank54Frame064 = SPRITE_FRAME(gSpriteBank54Frame064, SPRITE_TILES_BANK54_FRAME064);
const struct sprite_frame gSpriteBank54Frame065 = SPRITE_FRAME(gSpriteBank54Frame065, SPRITE_TILES_BANK54_FRAME065);
const struct sprite_frame gSpriteBank54Frame066 = SPRITE_FRAME(gSpriteBank54Frame066, SPRITE_TILES_BANK54_FRAME066);
const struct sprite_frame gSpriteBank54Frame067 = SPRITE_FRAME(gSpriteBank54Frame067, SPRITE_TILES_BANK54_FRAME067);
const struct sprite_frame gSpriteBank54Frame068 = SPRITE_FRAME(gSpriteBank54Frame068, SPRITE_TILES_BANK54_FRAME068);
const struct sprite_frame gSpriteBank54Frame069 = SPRITE_FRAME(gSpriteBank54Frame069, SPRITE_TILES_BANK54_FRAME069);
const struct sprite_frame gSpriteBank54Frame070 = SPRITE_FRAME(gSpriteBank54Frame070, SPRITE_TILES_BANK54_FRAME070);
const struct sprite_frame gSpriteBank54Frame071 = SPRITE_FRAME(gSpriteBank54Frame071, SPRITE_TILES_BANK54_FRAME071);
const struct sprite_frame gSpriteBank54Frame072 = SPRITE_FRAME(gSpriteBank54Frame072, SPRITE_TILES_BANK54_FRAME072);
const struct sprite_frame gSpriteBank54Frame073 = SPRITE_FRAME(gSpriteBank54Frame073, SPRITE_TILES_BANK54_FRAME073);
const struct sprite_frame gSpriteBank54Frame074 = SPRITE_FRAME(gSpriteBank54Frame074, SPRITE_TILES_BANK54_FRAME074);
const struct sprite_frame gSpriteBank54Frame075 = SPRITE_FRAME(gSpriteBank54Frame075, SPRITE_TILES_BANK54_FRAME075);
const struct sprite_frame gSpriteBank54Frame076 = SPRITE_FRAME(gSpriteBank54Frame076, SPRITE_TILES_BANK54_FRAME076);
const struct sprite_frame_1box gSpriteBank54Frame077 = {
    SPRITE_FRAME(gSpriteBank54Frame077, SPRITE_TILES_BANK54_FRAME077),
    { { -20, -25, 25, 53 } },
};
const struct sprite_frame_1box gSpriteBank54Frame078 = {
    SPRITE_FRAME(gSpriteBank54Frame078, SPRITE_TILES_BANK54_FRAME078),
    { { -19, -18, 24, 47 } },
};
const struct sprite_frame_1box gSpriteBank54Frame079 = {
    SPRITE_FRAME(gSpriteBank54Frame079, SPRITE_TILES_BANK54_FRAME079),
    { { -19, -11, 23, 37 } },
};
const struct sprite_frame_1box gSpriteBank54Frame080 = {
    SPRITE_FRAME(gSpriteBank54Frame080, SPRITE_TILES_BANK54_FRAME080),
    { { -21, -8, 26, 36 } },
};
const struct sprite_frame_1box gSpriteBank54Frame081 = {
    SPRITE_FRAME(gSpriteBank54Frame081, SPRITE_TILES_BANK54_FRAME081),
    { { -19, -18, 23, 46 } },
};
const struct sprite_frame_1box gSpriteBank54Frame082 = {
    SPRITE_FRAME(gSpriteBank54Frame082, SPRITE_TILES_BANK54_FRAME082),
    { { -20, -25, 25, 53 } },
};
const struct sprite_frame_1box gSpriteBank54Frame083 = {
    SPRITE_FRAME(gSpriteBank54Frame083, SPRITE_TILES_BANK54_FRAME083),
    { { -20, -23, 24, 52 } },
};
const struct sprite_frame_1box gSpriteBank54Frame084 = {
    SPRITE_FRAME(gSpriteBank54Frame084, SPRITE_TILES_BANK54_FRAME084),
    { { -20, -26, 24, 55 } },
};
const struct sprite_frame_1box gSpriteBank54Frame085 = {
    SPRITE_FRAME(gSpriteBank54Frame085, SPRITE_TILES_BANK54_FRAME085),
    { { -22, -23, 26, 51 } },
};
const struct sprite_frame_1box gSpriteBank54Frame086 = {
    SPRITE_FRAME(gSpriteBank54Frame086, SPRITE_TILES_BANK54_FRAME086),
    { { -20, -25, 25, 53 } },
};
const struct sprite_frame_1box gSpriteBank54Frame087 = {
    SPRITE_FRAME(gSpriteBank54Frame087, SPRITE_TILES_BANK54_FRAME087),
    { { -20, -25, 25, 53 } },
};
const struct sprite_frame_1box gSpriteBank54Frame088 = {
    SPRITE_FRAME(gSpriteBank54Frame088, SPRITE_TILES_BANK54_FRAME088),
    { { -20, -25, 25, 53 } },
};
const struct sprite_frame_1box gSpriteBank54Frame089 = {
    SPRITE_FRAME(gSpriteBank54Frame089, SPRITE_TILES_BANK54_FRAME089),
    { { -20, -25, 25, 53 } },
};
const struct sprite_frame_1box gSpriteBank54Frame090 = {
    SPRITE_FRAME(gSpriteBank54Frame090, SPRITE_TILES_BANK54_FRAME090),
    { { -20, -25, 25, 53 } },
};
const struct sprite_frame_1box gSpriteBank54Frame091 = {
    SPRITE_FRAME(gSpriteBank54Frame091, SPRITE_TILES_BANK54_FRAME091),
    { { -20, -25, 25, 53 } },
};
const struct sprite_frame gSpriteBank54Frame092 = SPRITE_FRAME(gSpriteBank54Frame092, SPRITE_TILES_BANK54_FRAME092);
const struct sprite_frame gSpriteBank54Frame093 = SPRITE_FRAME(gSpriteBank54Frame093, SPRITE_TILES_BANK54_FRAME093);
const struct sprite_frame gSpriteBank54Frame094 = SPRITE_FRAME(gSpriteBank54Frame094, SPRITE_TILES_BANK54_FRAME094);
const struct sprite_frame gSpriteBank54Frame095 = SPRITE_FRAME(gSpriteBank54Frame095, SPRITE_TILES_BANK54_FRAME095);
const struct sprite_frame gSpriteBank54Frame096 = SPRITE_FRAME(gSpriteBank54Frame096, SPRITE_TILES_BANK54_FRAME096);
const struct sprite_frame gSpriteBank54Frame097 = SPRITE_FRAME(gSpriteBank54Frame097, SPRITE_TILES_BANK54_FRAME097);
const struct sprite_frame gSpriteBank54Frame098 = SPRITE_FRAME(gSpriteBank54Frame098, SPRITE_TILES_BANK54_FRAME098);
const struct sprite_frame gSpriteBank54Frame099 = SPRITE_FRAME(gSpriteBank54Frame099, SPRITE_TILES_BANK54_FRAME099);
const struct sprite_frame gSpriteBank54Frame100 = SPRITE_FRAME(gSpriteBank54Frame100, SPRITE_TILES_BANK54_FRAME100);
const struct sprite_frame gSpriteBank54Frame101 = SPRITE_FRAME(gSpriteBank54Frame101, SPRITE_TILES_BANK54_FRAME101);
const struct sprite_frame gSpriteBank54Frame102 = SPRITE_FRAME(gSpriteBank54Frame102, SPRITE_TILES_BANK54_FRAME102);
const struct sprite_frame gSpriteBank54Frame103 = SPRITE_FRAME(gSpriteBank54Frame103, SPRITE_TILES_BANK54_FRAME103);
const struct sprite_frame_1box gSpriteBank54Frame104 = {
    SPRITE_FRAME(gSpriteBank54Frame104, SPRITE_TILES_BANK54_FRAME104),
    { { -1, -9, 3, 18 } },
};
const struct sprite_frame_1box gSpriteBank54Frame105 = {
    SPRITE_FRAME(gSpriteBank54Frame105, SPRITE_TILES_BANK54_FRAME105),
    { { -1, -9, 3, 18 } },
};
const struct sprite_frame_1box gSpriteBank54Frame106 = {
    SPRITE_FRAME(gSpriteBank54Frame106, SPRITE_TILES_BANK54_FRAME106),
    { { -1, -9, 3, 18 } },
};
const struct sprite_frame_1box gSpriteBank54Frame107 = {
    SPRITE_FRAME(gSpriteBank54Frame107, SPRITE_TILES_BANK54_FRAME107),
    { { -1, -9, 3, 18 } },
};
const struct sprite_frame_1box gSpriteBank54Frame108 = {
    SPRITE_FRAME(gSpriteBank54Frame108, SPRITE_TILES_BANK54_FRAME108),
    { { -1, -9, 3, 18 } },
};
const struct sprite_frame_1box gSpriteBank54Frame109 = {
    SPRITE_FRAME(gSpriteBank54Frame109, SPRITE_TILES_BANK54_FRAME109),
    { { -1, -9, 3, 18 } },
};
const struct sprite_frame_1box gSpriteBank54Frame110 = {
    SPRITE_FRAME(gSpriteBank54Frame110, SPRITE_TILES_BANK54_FRAME110),
    { { -1, -9, 3, 18 } },
};
const struct sprite_frame_1box gSpriteBank54Frame111 = {
    SPRITE_FRAME(gSpriteBank54Frame111, SPRITE_TILES_BANK54_FRAME111),
    { { -1, -9, 3, 18 } },
};
const struct sprite_frame_1box gSpriteBank54Frame112 = {
    SPRITE_FRAME(gSpriteBank54Frame112, SPRITE_TILES_BANK54_FRAME112),
    { { -1, -9, 3, 18 } },
};
const struct sprite_frame_1box gSpriteBank54Frame113 = {
    SPRITE_FRAME(gSpriteBank54Frame113, SPRITE_TILES_BANK54_FRAME113),
    { { -1, -9, 3, 18 } },
};
const struct sprite_frame gSpriteBank54Frame114 = SPRITE_FRAME(gSpriteBank54Frame114, SPRITE_TILES_BANK54_FRAME114);
const struct sprite_frame gSpriteBank54Frame115 = SPRITE_FRAME(gSpriteBank54Frame115, SPRITE_TILES_BANK54_FRAME115);
const struct sprite_frame gSpriteBank54Frame116 = SPRITE_FRAME(gSpriteBank54Frame116, SPRITE_TILES_BANK54_FRAME116);
const struct sprite_frame gSpriteBank54Frame117 = SPRITE_FRAME(gSpriteBank54Frame117, SPRITE_TILES_BANK54_FRAME117);
const struct sprite_frame gSpriteBank54Frame118 = SPRITE_FRAME(gSpriteBank54Frame118, SPRITE_TILES_BANK54_FRAME118);
const struct sprite_frame gSpriteBank54Frame119 = SPRITE_FRAME(gSpriteBank54Frame119, SPRITE_TILES_BANK54_FRAME119);
const struct sprite_frame_1box gSpriteBank54Frame120 = {
    SPRITE_FRAME(gSpriteBank54Frame120, SPRITE_TILES_BANK54_FRAME120),
    { { -2, -25, 5, 48 } },
};

/* The piece arrays, from graphics/sprites/bank54_dingodile/bank.json. */
#include "sprites/bank54_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 55: 9 animations, 130 frames, graphics/sprites/bank55_tiny/. */

extern const u16 gSpriteBank55Anim00Seq[17];
extern const u16 gSpriteBank55Anim01Seq[18];
extern const u16 gSpriteBank55Anim02Seq[14];
extern const u16 gSpriteBank55Anim03Seq[10];
extern const u16 gSpriteBank55Anim04Seq[4];
extern const u16 gSpriteBank55Anim05Seq[15];
extern const u16 gSpriteBank55Anim06Seq[15];
extern const u16 gSpriteBank55Anim07Seq[25];
extern const u16 gSpriteBank55Anim08Seq[12];

extern const struct sprite_anim gSpriteBank55Anims[9] = {
    /* 0 */ {
        /* seq */ gSpriteBank55Anim00Seq,
        /* box */ { { -37, -63, 83, 97 }, { -68, -68, 117, 113 } },
        /* paletteId */ 100,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank55Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank55Anim01Seq,
        /* box */ { { -26, -22, 57, 67 }, { -51, -72, 106, 117 } },
        /* paletteId */ 100,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank55Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank55Anim02Seq,
        /* box */ { { -26, -22, 57, 67 }, { -38, -63, 88, 108 } },
        /* paletteId */ 100,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank55Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank55Anim03Seq,
        /* box */ { { -25, -45, 62, 78 }, { -31, -63, 81, 102 } },
        /* paletteId */ 100,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank55Anim03Seq),
        /* flags */ 0,
    },
    /* 4 */ {
        /* seq */ gSpriteBank55Anim04Seq,
        /* box */ { { -26, -22, 57, 67 }, { -35, -61, 66, 106 } },
        /* paletteId */ 100,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank55Anim04Seq),
        /* flags */ 0,
    },
    /* 5 */ {
        /* seq */ gSpriteBank55Anim05Seq,
        /* box */ { { -24, -14, 49, 28 }, { -28, -14, 57, 51 } },
        /* paletteId */ 101,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank55Anim05Seq),
        /* flags */ 0,
    },
    /* 6 */ {
        /* seq */ gSpriteBank55Anim06Seq,
        /* box */ { { -23, -54, 56, 87 }, { -33, -54, 66, 88 } },
        /* paletteId */ 100,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank55Anim06Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 7 */ {
        /* seq */ gSpriteBank55Anim07Seq,
        /* box */ { { -32, -47, 69, 70 }, { -68, -62, 121, 100 } },
        /* paletteId */ 100,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank55Anim07Seq),
        /* flags */ 0,
    },
    /* 8 */ {
        /* seq */ gSpriteBank55Anim08Seq,
        /* box */ { { -13, -45, 26, 91 }, { -92, -65, 126, 111 } },
        /* paletteId */ 100,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank55Anim08Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank55Anim00Seq[17] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16,
};
const u16 gSpriteBank55Anim01Seq[18] = {
    17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
    33, 34,
};
const u16 gSpriteBank55Anim02Seq[14] = {
    35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
};
const u16 gSpriteBank55Anim03Seq[10] = {
    49, 50, 51, 52, 53, 54, 55, 56, 57, 58,
};
const u16 gSpriteBank55Anim04Seq[4] = {
    59, 60, 61, 62,
};
const u16 gSpriteBank55Anim05Seq[15] = {
    63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77,
};
const u16 gSpriteBank55Anim06Seq[15] = {
    78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92,
};
const u16 gSpriteBank55Anim07Seq[25] = {
    93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108,
    109, 110, 111, 112, 113, 114, 115, 116, 117,
};
const u16 gSpriteBank55Anim08Seq[12] = {
    118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129,
};

extern const struct sprite_frame *const gSpriteBank55Frames[130] = {
    &gSpriteBank55Frame000.frame,
    &gSpriteBank55Frame001.frame,
    &gSpriteBank55Frame002.frame,
    &gSpriteBank55Frame003.frame,
    &gSpriteBank55Frame004.frame,
    &gSpriteBank55Frame005.frame,
    &gSpriteBank55Frame006.frame,
    &gSpriteBank55Frame007.frame,
    &gSpriteBank55Frame008.frame,
    &gSpriteBank55Frame009.frame,
    &gSpriteBank55Frame010.frame,
    &gSpriteBank55Frame011.frame,
    &gSpriteBank55Frame012.frame,
    &gSpriteBank55Frame013.frame,
    &gSpriteBank55Frame014.frame,
    &gSpriteBank55Frame015.frame,
    &gSpriteBank55Frame016.frame,
    &gSpriteBank55Frame017.frame,
    &gSpriteBank55Frame018.frame,
    &gSpriteBank55Frame019.frame,
    &gSpriteBank55Frame020.frame,
    &gSpriteBank55Frame021.frame,
    &gSpriteBank55Frame022.frame,
    &gSpriteBank55Frame023.frame,
    &gSpriteBank55Frame024.frame,
    &gSpriteBank55Frame025.frame,
    &gSpriteBank55Frame026.frame,
    &gSpriteBank55Frame027.frame,
    &gSpriteBank55Frame028.frame,
    &gSpriteBank55Frame029.frame,
    &gSpriteBank55Frame030.frame,
    &gSpriteBank55Frame031.frame,
    &gSpriteBank55Frame032.frame,
    &gSpriteBank55Frame033.frame,
    &gSpriteBank55Frame034.frame,
    &gSpriteBank55Frame035.frame,
    &gSpriteBank55Frame036.frame,
    &gSpriteBank55Frame037.frame,
    &gSpriteBank55Frame038.frame,
    &gSpriteBank55Frame039.frame,
    &gSpriteBank55Frame040.frame,
    &gSpriteBank55Frame041.frame,
    &gSpriteBank55Frame042.frame,
    &gSpriteBank55Frame043.frame,
    &gSpriteBank55Frame044.frame,
    &gSpriteBank55Frame045.frame,
    &gSpriteBank55Frame046.frame,
    &gSpriteBank55Frame047.frame,
    &gSpriteBank55Frame048.frame,
    &gSpriteBank55Frame049.frame,
    &gSpriteBank55Frame050.frame,
    &gSpriteBank55Frame051.frame,
    &gSpriteBank55Frame052.frame,
    &gSpriteBank55Frame053.frame,
    &gSpriteBank55Frame054.frame,
    &gSpriteBank55Frame055.frame,
    &gSpriteBank55Frame056.frame,
    &gSpriteBank55Frame057.frame,
    &gSpriteBank55Frame058.frame,
    &gSpriteBank55Frame059.frame,
    &gSpriteBank55Frame060.frame,
    &gSpriteBank55Frame061.frame,
    &gSpriteBank55Frame062.frame,
    &gSpriteBank55Frame063.frame,
    &gSpriteBank55Frame064.frame,
    &gSpriteBank55Frame065.frame,
    &gSpriteBank55Frame066.frame,
    &gSpriteBank55Frame067.frame,
    &gSpriteBank55Frame068.frame,
    &gSpriteBank55Frame069.frame,
    &gSpriteBank55Frame070.frame,
    &gSpriteBank55Frame071.frame,
    &gSpriteBank55Frame072.frame,
    &gSpriteBank55Frame073.frame,
    &gSpriteBank55Frame074.frame,
    &gSpriteBank55Frame075.frame,
    &gSpriteBank55Frame076.frame,
    &gSpriteBank55Frame077.frame,
    &gSpriteBank55Frame078.frame,
    &gSpriteBank55Frame079.frame,
    &gSpriteBank55Frame080.frame,
    &gSpriteBank55Frame081.frame,
    &gSpriteBank55Frame082.frame,
    &gSpriteBank55Frame083.frame,
    &gSpriteBank55Frame084.frame,
    &gSpriteBank55Frame085.frame,
    &gSpriteBank55Frame086.frame,
    &gSpriteBank55Frame087.frame,
    &gSpriteBank55Frame088.frame,
    &gSpriteBank55Frame089.frame,
    &gSpriteBank55Frame090.frame,
    &gSpriteBank55Frame091.frame,
    &gSpriteBank55Frame092.frame,
    &gSpriteBank55Frame093.frame,
    &gSpriteBank55Frame094.frame,
    &gSpriteBank55Frame095.frame,
    &gSpriteBank55Frame096.frame,
    &gSpriteBank55Frame097.frame,
    &gSpriteBank55Frame098.frame,
    &gSpriteBank55Frame099.frame,
    &gSpriteBank55Frame100.frame,
    &gSpriteBank55Frame101.frame,
    &gSpriteBank55Frame102.frame,
    &gSpriteBank55Frame103.frame,
    &gSpriteBank55Frame104.frame,
    &gSpriteBank55Frame105.frame,
    &gSpriteBank55Frame106.frame,
    &gSpriteBank55Frame107.frame,
    &gSpriteBank55Frame108.frame,
    &gSpriteBank55Frame109.frame,
    &gSpriteBank55Frame110.frame,
    &gSpriteBank55Frame111.frame,
    &gSpriteBank55Frame112.frame,
    &gSpriteBank55Frame113.frame,
    &gSpriteBank55Frame114.frame,
    &gSpriteBank55Frame115.frame,
    &gSpriteBank55Frame116.frame,
    &gSpriteBank55Frame117.frame,
    &gSpriteBank55Frame118.frame,
    &gSpriteBank55Frame119.frame,
    &gSpriteBank55Frame120.frame,
    &gSpriteBank55Frame121.frame,
    &gSpriteBank55Frame122.frame,
    &gSpriteBank55Frame123.frame,
    &gSpriteBank55Frame124.frame,
    &gSpriteBank55Frame125.frame,
    &gSpriteBank55Frame126.frame,
    &gSpriteBank55Frame127.frame,
    &gSpriteBank55Frame128.frame,
    &gSpriteBank55Frame129.frame,
};

const struct sprite_frame_2box gSpriteBank55Frame000 = {
    SPRITE_FRAME(gSpriteBank55Frame000, SPRITE_TILES_BANK55_FRAME000),
    { { -37, -63, 83, 97 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame001 = {
    SPRITE_FRAME(gSpriteBank55Frame001, SPRITE_TILES_BANK55_FRAME001),
    { { -68, -67, 80, 96 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame002 = {
    SPRITE_FRAME(gSpriteBank55Frame002, SPRITE_TILES_BANK55_FRAME002),
    { { -67, -68, 74, 101 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame003 = {
    SPRITE_FRAME(gSpriteBank55Frame003, SPRITE_TILES_BANK55_FRAME003),
    { { -66, -67, 69, 108 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame004 = {
    SPRITE_FRAME(gSpriteBank55Frame004, SPRITE_TILES_BANK55_FRAME004),
    { { -65, -65, 68, 110 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame005 = {
    SPRITE_FRAME(gSpriteBank55Frame005, SPRITE_TILES_BANK55_FRAME005),
    { { -65, -62, 68, 106 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame006 = {
    SPRITE_FRAME(gSpriteBank55Frame006, SPRITE_TILES_BANK55_FRAME006),
    { { -68, -56, 73, 93 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame007 = {
    SPRITE_FRAME(gSpriteBank55Frame007, SPRITE_TILES_BANK55_FRAME007),
    { { -66, -43, 85, 74 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame008 = {
    SPRITE_FRAME(gSpriteBank55Frame008, SPRITE_TILES_BANK55_FRAME008),
    { { -53, -36, 83, 67 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame009 = {
    SPRITE_FRAME(gSpriteBank55Frame009, SPRITE_TILES_BANK55_FRAME009),
    { { -46, -31, 75, 62 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame010 = {
    SPRITE_FRAME(gSpriteBank55Frame010, SPRITE_TILES_BANK55_FRAME010),
    { { -48, -28, 66, 59 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame011 = {
    SPRITE_FRAME(gSpriteBank55Frame011, SPRITE_TILES_BANK55_FRAME011),
    { { -50, -25, 61, 58 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame012 = {
    SPRITE_FRAME(gSpriteBank55Frame012, SPRITE_TILES_BANK55_FRAME012),
    { { -50, -25, 61, 58 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame013 = {
    SPRITE_FRAME(gSpriteBank55Frame013, SPRITE_TILES_BANK55_FRAME013),
    { { -50, -23, 65, 56 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame014 = {
    SPRITE_FRAME(gSpriteBank55Frame014, SPRITE_TILES_BANK55_FRAME014),
    { { -48, -19, 73, 52 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame015 = {
    SPRITE_FRAME(gSpriteBank55Frame015, SPRITE_TILES_BANK55_FRAME015),
    { { -45, -12, 88, 45 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame016 = {
    SPRITE_FRAME(gSpriteBank55Frame016, SPRITE_TILES_BANK55_FRAME016),
    { { -43, -8, 92, 41 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame017 = {
    SPRITE_FRAME(gSpriteBank55Frame017, SPRITE_TILES_BANK55_FRAME017),
    { { -26, -22, 57, 67 }, { -15, -12, 29, 54 } },
};
const struct sprite_frame_2box gSpriteBank55Frame018 = {
    SPRITE_FRAME(gSpriteBank55Frame018, SPRITE_TILES_BANK55_FRAME018),
    { { -27, -47, 48, 87 }, { -18, -28, 23, 61 } },
};
const struct sprite_frame_2box gSpriteBank55Frame019 = {
    SPRITE_FRAME(gSpriteBank55Frame019, SPRITE_TILES_BANK55_FRAME019),
    { { -33, -61, 44, 98 }, { -21, -35, 22, 66 } },
};
const struct sprite_frame_2box gSpriteBank55Frame020 = {
    SPRITE_FRAME(gSpriteBank55Frame020, SPRITE_TILES_BANK55_FRAME020),
    { { -35, -61, 44, 96 }, { -29, -34, 31, 62 } },
};
const struct sprite_frame_2box gSpriteBank55Frame021 = {
    SPRITE_FRAME(gSpriteBank55Frame021, SPRITE_TILES_BANK55_FRAME021),
    { { -37, -56, 50, 87 }, { -29, -44, 32, 71 } },
};
const struct sprite_frame_2box gSpriteBank55Frame022 = {
    SPRITE_FRAME(gSpriteBank55Frame022, SPRITE_TILES_BANK55_FRAME022),
    { { -37, -42, 64, 77 }, { -29, -44, 32, 71 } },
};
const struct sprite_frame_2box gSpriteBank55Frame023 = {
    SPRITE_FRAME(gSpriteBank55Frame023, SPRITE_TILES_BANK55_FRAME023),
    { { -38, -42, 82, 72 }, { -29, -44, 32, 71 } },
};
const struct sprite_frame_2box gSpriteBank55Frame024 = {
    SPRITE_FRAME(gSpriteBank55Frame024, SPRITE_TILES_BANK55_FRAME024),
    { { -42, -62, 88, 81 }, { -29, -44, 32, 71 } },
};
const struct sprite_frame_2box gSpriteBank55Frame025 = {
    SPRITE_FRAME(gSpriteBank55Frame025, SPRITE_TILES_BANK55_FRAME025),
    { { -51, -71, 82, 80 }, { -29, -44, 32, 71 } },
};
const struct sprite_frame_2box gSpriteBank55Frame026 = {
    SPRITE_FRAME(gSpriteBank55Frame026, SPRITE_TILES_BANK55_FRAME026),
    { { -48, -72, 81, 82 }, { -29, -44, 32, 71 } },
};
const struct sprite_frame_2box gSpriteBank55Frame027 = {
    SPRITE_FRAME(gSpriteBank55Frame027, SPRITE_TILES_BANK55_FRAME027),
    { { -36, -69, 91, 88 }, { -25, -55, 63, 43 } },
};
const struct sprite_frame_2box gSpriteBank55Frame028 = {
    SPRITE_FRAME(gSpriteBank55Frame028, SPRITE_TILES_BANK55_FRAME028),
    { { -30, -60, 78, 79 }, { -27, -12, 73, 31 } },
};
const struct sprite_frame_2box gSpriteBank55Frame029 = {
    SPRITE_FRAME(gSpriteBank55Frame029, SPRITE_TILES_BANK55_FRAME029),
    { { -33, -54, 66, 84 }, { -31, -15, 63, 42 } },
};
const struct sprite_frame_2box gSpriteBank55Frame030 = {
    SPRITE_FRAME(gSpriteBank55Frame030, SPRITE_TILES_BANK55_FRAME030),
    { { -28, -54, 61, 86 }, { -25, -4, 60, 32 } },
};
const struct sprite_frame_2box gSpriteBank55Frame031 = {
    SPRITE_FRAME(gSpriteBank55Frame031, SPRITE_TILES_BANK55_FRAME031),
    { { -26, -54, 59, 96 }, { -23, 4, 57, 24 } },
};
const struct sprite_frame_2box gSpriteBank55Frame032 = {
    SPRITE_FRAME(gSpriteBank55Frame032, SPRITE_TILES_BANK55_FRAME032),
    { { -32, -54, 65, 87 }, { -26, 2, 62, 27 } },
};
const struct sprite_frame_2box gSpriteBank55Frame033 = {
    SPRITE_FRAME(gSpriteBank55Frame033, SPRITE_TILES_BANK55_FRAME033),
    { { -31, -54, 64, 89 }, { -29, -44, 32, 71 } },
};
const struct sprite_frame_2box gSpriteBank55Frame034 = {
    SPRITE_FRAME(gSpriteBank55Frame034, SPRITE_TILES_BANK55_FRAME034),
    { { -25, -54, 58, 87 }, { -29, -44, 32, 71 } },
};
const struct sprite_frame_2box gSpriteBank55Frame035 = {
    SPRITE_FRAME(gSpriteBank55Frame035, SPRITE_TILES_BANK55_FRAME035),
    { { -26, -22, 57, 67 }, { -30, 14, 59, 29 } },
};
const struct sprite_frame_2box gSpriteBank55Frame036 = {
    SPRITE_FRAME(gSpriteBank55Frame036, SPRITE_TILES_BANK55_FRAME036),
    { { -27, -47, 48, 87 }, { -24, -19, 28, 56 } },
};
const struct sprite_frame_2box gSpriteBank55Frame037 = {
    SPRITE_FRAME(gSpriteBank55Frame037, SPRITE_TILES_BANK55_FRAME037),
    { { -33, -61, 44, 98 }, { -27, -23, 27, 60 } },
};
const struct sprite_frame_2box gSpriteBank55Frame038 = {
    SPRITE_FRAME(gSpriteBank55Frame038, SPRITE_TILES_BANK55_FRAME038),
    { { -35, -61, 44, 96 }, { -28, -29, 27, 61 } },
};
const struct sprite_frame_2box gSpriteBank55Frame039 = {
    SPRITE_FRAME(gSpriteBank55Frame039, SPRITE_TILES_BANK55_FRAME039),
    { { -37, -56, 50, 87 }, { -27, -32, 26, 59 } },
};
const struct sprite_frame_2box gSpriteBank55Frame040 = {
    SPRITE_FRAME(gSpriteBank55Frame040, SPRITE_TILES_BANK55_FRAME040),
    { { -38, -50, 56, 79 }, { -27, -31, 28, 57 } },
};
const struct sprite_frame_2box gSpriteBank55Frame041 = {
    SPRITE_FRAME(gSpriteBank55Frame041, SPRITE_TILES_BANK55_FRAME041),
    { { -36, -47, 61, 76 }, { -31, -19, 35, 44 } },
};
const struct sprite_frame_2box gSpriteBank55Frame042 = {
    SPRITE_FRAME(gSpriteBank55Frame042, SPRITE_TILES_BANK55_FRAME042),
    { { -30, -41, 67, 72 }, { -16, -32, 21, 59 } },
};
const struct sprite_frame_2box gSpriteBank55Frame043 = {
    SPRITE_FRAME(gSpriteBank55Frame043, SPRITE_TILES_BANK55_FRAME043),
    { { -25, -45, 62, 78 }, { -18, -30, 27, 58 } },
};
const struct sprite_frame_2box gSpriteBank55Frame044 = {
    SPRITE_FRAME(gSpriteBank55Frame044, SPRITE_TILES_BANK55_FRAME044),
    { { -23, -53, 58, 87 }, { -17, -31, 30, 55 } },
};
const struct sprite_frame_2box gSpriteBank55Frame045 = {
    SPRITE_FRAME(gSpriteBank55Frame045, SPRITE_TILES_BANK55_FRAME045),
    { { -22, -59, 56, 95 }, { -14, -31, 26, 59 } },
};
const struct sprite_frame_2box gSpriteBank55Frame046 = {
    SPRITE_FRAME(gSpriteBank55Frame046, SPRITE_TILES_BANK55_FRAME046),
    { { -20, -63, 55, 102 }, { -17, -37, 30, 67 } },
};
const struct sprite_frame_2box gSpriteBank55Frame047 = {
    SPRITE_FRAME(gSpriteBank55Frame047, SPRITE_TILES_BANK55_FRAME047),
    { { -31, -13, 81, 52 }, { -36, -2, 81, 39 } },
};
const struct sprite_frame_2box gSpriteBank55Frame048 = {
    SPRITE_FRAME(gSpriteBank55Frame048, SPRITE_TILES_BANK55_FRAME048),
    { { -33, -11, 67, 44 }, { -32, 11, 70, 25 } },
};
const struct sprite_frame_2box gSpriteBank55Frame049 = {
    SPRITE_FRAME(gSpriteBank55Frame049, SPRITE_TILES_BANK55_FRAME049),
    { { -25, -45, 62, 78 }, { -18, -29, 27, 59 } },
};
const struct sprite_frame_2box gSpriteBank55Frame050 = {
    SPRITE_FRAME(gSpriteBank55Frame050, SPRITE_TILES_BANK55_FRAME050),
    { { -23, -53, 58, 87 }, { -16, -34, 31, 64 } },
};
const struct sprite_frame_2box gSpriteBank55Frame051 = {
    SPRITE_FRAME(gSpriteBank55Frame051, SPRITE_TILES_BANK55_FRAME051),
    { { -22, -59, 56, 95 }, { -13, -35, 27, 66 } },
};
const struct sprite_frame_2box gSpriteBank55Frame052 = {
    SPRITE_FRAME(gSpriteBank55Frame052, SPRITE_TILES_BANK55_FRAME052),
    { { -20, -63, 55, 102 }, { -13, -42, 29, 74 } },
};
const struct sprite_frame_2box gSpriteBank55Frame053 = {
    SPRITE_FRAME(gSpriteBank55Frame053, SPRITE_TILES_BANK55_FRAME053),
    { { -31, -13, 81, 52 }, { -22, -5, 58, 42 } },
};
const struct sprite_frame_2box gSpriteBank55Frame054 = {
    SPRITE_FRAME(gSpriteBank55Frame054, SPRITE_TILES_BANK55_FRAME054),
    { { -28, 0, 67, 39 }, { -26, 15, 63, 21 } },
};
const struct sprite_frame_2box gSpriteBank55Frame055 = {
    SPRITE_FRAME(gSpriteBank55Frame055, SPRITE_TILES_BANK55_FRAME055),
    { { -28, 0, 67, 39 }, { -25, 10, 58, 26 } },
};
const struct sprite_frame_2box gSpriteBank55Frame056 = {
    SPRITE_FRAME(gSpriteBank55Frame056, SPRITE_TILES_BANK55_FRAME056),
    { { -28, 0, 67, 39 }, { -19, 18, 52, 18 } },
};
const struct sprite_frame_2box gSpriteBank55Frame057 = {
    SPRITE_FRAME(gSpriteBank55Frame057, SPRITE_TILES_BANK55_FRAME057),
    { { -28, 0, 67, 39 }, { -22, 10, 57, 25 } },
};
const struct sprite_frame_2box gSpriteBank55Frame058 = {
    SPRITE_FRAME(gSpriteBank55Frame058, SPRITE_TILES_BANK55_FRAME058),
    { { -28, 0, 67, 39 }, { -27, 8, 64, 25 } },
};
const struct sprite_frame_2box gSpriteBank55Frame059 = {
    SPRITE_FRAME(gSpriteBank55Frame059, SPRITE_TILES_BANK55_FRAME059),
    { { -26, -22, 57, 67 }, { -13, -12, 26, 51 } },
};
const struct sprite_frame_2box gSpriteBank55Frame060 = {
    SPRITE_FRAME(gSpriteBank55Frame060, SPRITE_TILES_BANK55_FRAME060),
    { { -27, -47, 48, 87 }, { -13, -12, 26, 51 } },
};
const struct sprite_frame_2box gSpriteBank55Frame061 = {
    SPRITE_FRAME(gSpriteBank55Frame061, SPRITE_TILES_BANK55_FRAME061),
    { { -33, -61, 44, 98 }, { -13, -12, 26, 51 } },
};
const struct sprite_frame_2box gSpriteBank55Frame062 = {
    SPRITE_FRAME(gSpriteBank55Frame062, SPRITE_TILES_BANK55_FRAME062),
    { { -35, -61, 44, 96 }, { -13, -12, 26, 51 } },
};
const struct sprite_frame_1box gSpriteBank55Frame063 = {
    SPRITE_FRAME(gSpriteBank55Frame063, SPRITE_TILES_BANK55_FRAME063),
    { { -24, -14, 49, 28 } },
};
const struct sprite_frame_1box gSpriteBank55Frame064 = {
    SPRITE_FRAME(gSpriteBank55Frame064, SPRITE_TILES_BANK55_FRAME064),
    { { -21, -14, 50, 29 } },
};
const struct sprite_frame_1box gSpriteBank55Frame065 = {
    SPRITE_FRAME(gSpriteBank55Frame065, SPRITE_TILES_BANK55_FRAME065),
    { { -23, -12, 51, 34 } },
};
const struct sprite_frame_1box gSpriteBank55Frame066 = {
    SPRITE_FRAME(gSpriteBank55Frame066, SPRITE_TILES_BANK55_FRAME066),
    { { -24, -7, 49, 33 } },
};
const struct sprite_frame_1box gSpriteBank55Frame067 = {
    SPRITE_FRAME(gSpriteBank55Frame067, SPRITE_TILES_BANK55_FRAME067),
    { { -26, 1, 48, 28 } },
};
const struct sprite_frame_1box gSpriteBank55Frame068 = {
    SPRITE_FRAME(gSpriteBank55Frame068, SPRITE_TILES_BANK55_FRAME068),
    { { -25, 6, 44, 25 } },
};
const struct sprite_frame_1box gSpriteBank55Frame069 = {
    SPRITE_FRAME(gSpriteBank55Frame069, SPRITE_TILES_BANK55_FRAME069),
    { { -24, 7, 45, 25 } },
};
const struct sprite_frame_1box gSpriteBank55Frame070 = {
    SPRITE_FRAME(gSpriteBank55Frame070, SPRITE_TILES_BANK55_FRAME070),
    { { -25, 12, 45, 20 } },
};
const struct sprite_frame_1box gSpriteBank55Frame071 = {
    SPRITE_FRAME(gSpriteBank55Frame071, SPRITE_TILES_BANK55_FRAME071),
    { { -27, 13, 52, 21 } },
};
const struct sprite_frame_1box gSpriteBank55Frame072 = {
    SPRITE_FRAME(gSpriteBank55Frame072, SPRITE_TILES_BANK55_FRAME072),
    { { -27, 15, 52, 20 } },
};
const struct sprite_frame_1box gSpriteBank55Frame073 = {
    SPRITE_FRAME(gSpriteBank55Frame073, SPRITE_TILES_BANK55_FRAME073),
    { { -28, 14, 50, 20 } },
};
const struct sprite_frame_1box gSpriteBank55Frame074 = {
    SPRITE_FRAME(gSpriteBank55Frame074, SPRITE_TILES_BANK55_FRAME074),
    { { -27, 15, 51, 19 } },
};
const struct sprite_frame_1box gSpriteBank55Frame075 = {
    SPRITE_FRAME(gSpriteBank55Frame075, SPRITE_TILES_BANK55_FRAME075),
    { { -25, 18, 46, 19 } },
};
const struct sprite_frame_1box gSpriteBank55Frame076 = {
    SPRITE_FRAME(gSpriteBank55Frame076, SPRITE_TILES_BANK55_FRAME076),
    { { -23, 23, 47, 12 } },
};
const struct sprite_frame_1box gSpriteBank55Frame077 = {
    SPRITE_FRAME(gSpriteBank55Frame077, SPRITE_TILES_BANK55_FRAME077),
    { { -18, 27, 38, 9 } },
};
const struct sprite_frame_1box gSpriteBank55Frame078 = {
    SPRITE_FRAME(gSpriteBank55Frame078, SPRITE_TILES_BANK55_FRAME078),
    { { -23, -54, 56, 87 } },
};
const struct sprite_frame_1box gSpriteBank55Frame079 = {
    SPRITE_FRAME(gSpriteBank55Frame079, SPRITE_TILES_BANK55_FRAME079),
    { { -23, -54, 56, 87 } },
};
const struct sprite_frame_1box gSpriteBank55Frame080 = {
    SPRITE_FRAME(gSpriteBank55Frame080, SPRITE_TILES_BANK55_FRAME080),
    { { -25, -54, 58, 87 } },
};
const struct sprite_frame_1box gSpriteBank55Frame081 = {
    SPRITE_FRAME(gSpriteBank55Frame081, SPRITE_TILES_BANK55_FRAME081),
    { { -27, -54, 60, 87 } },
};
const struct sprite_frame_1box gSpriteBank55Frame082 = {
    SPRITE_FRAME(gSpriteBank55Frame082, SPRITE_TILES_BANK55_FRAME082),
    { { -29, -54, 62, 88 } },
};
const struct sprite_frame_1box gSpriteBank55Frame083 = {
    SPRITE_FRAME(gSpriteBank55Frame083, SPRITE_TILES_BANK55_FRAME083),
    { { -31, -54, 64, 88 } },
};
const struct sprite_frame_1box gSpriteBank55Frame084 = {
    SPRITE_FRAME(gSpriteBank55Frame084, SPRITE_TILES_BANK55_FRAME084),
    { { -32, -54, 65, 87 } },
};
const struct sprite_frame_1box gSpriteBank55Frame085 = {
    SPRITE_FRAME(gSpriteBank55Frame085, SPRITE_TILES_BANK55_FRAME085),
    { { -33, -54, 66, 87 } },
};
const struct sprite_frame_1box gSpriteBank55Frame086 = {
    SPRITE_FRAME(gSpriteBank55Frame086, SPRITE_TILES_BANK55_FRAME086),
    { { -33, -54, 66, 87 } },
};
const struct sprite_frame_1box gSpriteBank55Frame087 = {
    SPRITE_FRAME(gSpriteBank55Frame087, SPRITE_TILES_BANK55_FRAME087),
    { { -33, -54, 66, 87 } },
};
const struct sprite_frame_1box gSpriteBank55Frame088 = {
    SPRITE_FRAME(gSpriteBank55Frame088, SPRITE_TILES_BANK55_FRAME088),
    { { -33, -54, 66, 87 } },
};
const struct sprite_frame_1box gSpriteBank55Frame089 = {
    SPRITE_FRAME(gSpriteBank55Frame089, SPRITE_TILES_BANK55_FRAME089),
    { { -33, -54, 66, 87 } },
};
const struct sprite_frame_1box gSpriteBank55Frame090 = {
    SPRITE_FRAME(gSpriteBank55Frame090, SPRITE_TILES_BANK55_FRAME090),
    { { -33, -54, 66, 87 } },
};
const struct sprite_frame_1box gSpriteBank55Frame091 = {
    SPRITE_FRAME(gSpriteBank55Frame091, SPRITE_TILES_BANK55_FRAME091),
    { { -32, -54, 65, 87 } },
};
const struct sprite_frame_1box gSpriteBank55Frame092 = {
    SPRITE_FRAME(gSpriteBank55Frame092, SPRITE_TILES_BANK55_FRAME092),
    { { -30, -54, 63, 87 } },
};
const struct sprite_frame_1box gSpriteBank55Frame093 = {
    SPRITE_FRAME(gSpriteBank55Frame093, SPRITE_TILES_BANK55_FRAME093),
    { { -32, -47, 69, 70 } },
};
const struct sprite_frame_1box gSpriteBank55Frame094 = {
    SPRITE_FRAME(gSpriteBank55Frame094, SPRITE_TILES_BANK55_FRAME094),
    { { -34, -52, 74, 50 } },
};
const struct sprite_frame_1box gSpriteBank55Frame095 = {
    SPRITE_FRAME(gSpriteBank55Frame095, SPRITE_TILES_BANK55_FRAME095),
    { { -27, -52, 69, 47 } },
};
const struct sprite_frame_1box gSpriteBank55Frame096 = {
    SPRITE_FRAME(gSpriteBank55Frame096, SPRITE_TILES_BANK55_FRAME096),
    { { -20, -49, 63, 50 } },
};
const struct sprite_frame_1box gSpriteBank55Frame097 = {
    SPRITE_FRAME(gSpriteBank55Frame097, SPRITE_TILES_BANK55_FRAME097),
    { { -35, -27, 66, 56 } },
};
const struct sprite_frame_1box gSpriteBank55Frame098 = {
    SPRITE_FRAME(gSpriteBank55Frame098, SPRITE_TILES_BANK55_FRAME098),
    { { -62, 5, 100, 33 } },
};
const struct sprite_frame_1box gSpriteBank55Frame099 = {
    SPRITE_FRAME(gSpriteBank55Frame099, SPRITE_TILES_BANK55_FRAME099),
    { { -65, 4, 97, 31 } },
};
const struct sprite_frame_1box gSpriteBank55Frame100 = {
    SPRITE_FRAME(gSpriteBank55Frame100, SPRITE_TILES_BANK55_FRAME100),
    { { -68, -4, 95, 37 } },
};
const struct sprite_frame_1box gSpriteBank55Frame101 = {
    SPRITE_FRAME(gSpriteBank55Frame101, SPRITE_TILES_BANK55_FRAME101),
    { { -64, 2, 93, 34 } },
};
const struct sprite_frame_1box gSpriteBank55Frame102 = {
    SPRITE_FRAME(gSpriteBank55Frame102, SPRITE_TILES_BANK55_FRAME102),
    { { -68, 7, 100, 30 } },
};
const struct sprite_frame_1box gSpriteBank55Frame103 = {
    SPRITE_FRAME(gSpriteBank55Frame103, SPRITE_TILES_BANK55_FRAME103),
    { { -67, 8, 98, 30 } },
};
const struct sprite_frame_1box gSpriteBank55Frame104 = {
    SPRITE_FRAME(gSpriteBank55Frame104, SPRITE_TILES_BANK55_FRAME104),
    { { -67, -7, 98, 43 } },
};
const struct sprite_frame_1box gSpriteBank55Frame105 = {
    SPRITE_FRAME(gSpriteBank55Frame105, SPRITE_TILES_BANK55_FRAME105),
    { { -68, -12, 102, 45 } },
};
const struct sprite_frame_1box gSpriteBank55Frame106 = {
    SPRITE_FRAME(gSpriteBank55Frame106, SPRITE_TILES_BANK55_FRAME106),
    { { -67, -13, 101, 43 } },
};
const struct sprite_frame_1box gSpriteBank55Frame107 = {
    SPRITE_FRAME(gSpriteBank55Frame107, SPRITE_TILES_BANK55_FRAME107),
    { { -68, -9, 102, 39 } },
};
const struct sprite_frame_1box gSpriteBank55Frame108 = {
    SPRITE_FRAME(gSpriteBank55Frame108, SPRITE_TILES_BANK55_FRAME108),
    { { -26, -50, 57, 83 } },
};
const struct sprite_frame_1box gSpriteBank55Frame109 = {
    SPRITE_FRAME(gSpriteBank55Frame109, SPRITE_TILES_BANK55_FRAME109),
    { { -27, -40, 75, 65 } },
};
const struct sprite_frame_1box gSpriteBank55Frame110 = {
    SPRITE_FRAME(gSpriteBank55Frame110, SPRITE_TILES_BANK55_FRAME110),
    { { -46, -26, 72, 57 } },
};
const struct sprite_frame_1box gSpriteBank55Frame111 = {
    SPRITE_FRAME(gSpriteBank55Frame111, SPRITE_TILES_BANK55_FRAME111),
    { { -52, -22, 83, 53 } },
};
const struct sprite_frame_1box gSpriteBank55Frame112 = {
    SPRITE_FRAME(gSpriteBank55Frame112, SPRITE_TILES_BANK55_FRAME112),
    { { -57, -19, 96, 50 } },
};
const struct sprite_frame_1box gSpriteBank55Frame113 = {
    SPRITE_FRAME(gSpriteBank55Frame113, SPRITE_TILES_BANK55_FRAME113),
    { { -58, -19, 105, 50 } },
};
const struct sprite_frame_1box gSpriteBank55Frame114 = {
    SPRITE_FRAME(gSpriteBank55Frame114, SPRITE_TILES_BANK55_FRAME114),
    { { -57, -20, 110, 51 } },
};
const struct sprite_frame_1box gSpriteBank55Frame115 = {
    SPRITE_FRAME(gSpriteBank55Frame115, SPRITE_TILES_BANK55_FRAME115),
    { { -55, -20, 108, 51 } },
};
const struct sprite_frame_1box gSpriteBank55Frame116 = {
    SPRITE_FRAME(gSpriteBank55Frame116, SPRITE_TILES_BANK55_FRAME116),
    { { -60, -42, 89, 76 } },
};
const struct sprite_frame_1box gSpriteBank55Frame117 = {
    SPRITE_FRAME(gSpriteBank55Frame117, SPRITE_TILES_BANK55_FRAME117),
    { { -42, -62, 53, 89 } },
};
const struct sprite_frame_1box gSpriteBank55Frame118 = {
    SPRITE_FRAME(gSpriteBank55Frame118, SPRITE_TILES_BANK55_FRAME118),
    { { -13, -45, 26, 91 } },
};
const struct sprite_frame_1box gSpriteBank55Frame119 = {
    SPRITE_FRAME(gSpriteBank55Frame119, SPRITE_TILES_BANK55_FRAME119),
    { { -30, -52, 51, 88 } },
};
const struct sprite_frame_1box gSpriteBank55Frame120 = {
    SPRITE_FRAME(gSpriteBank55Frame120, SPRITE_TILES_BANK55_FRAME120),
    { { -40, -56, 56, 95 } },
};
const struct sprite_frame_1box gSpriteBank55Frame121 = {
    SPRITE_FRAME(gSpriteBank55Frame121, SPRITE_TILES_BANK55_FRAME121),
    { { -38, -65, 51, 101 } },
};
const struct sprite_frame_1box gSpriteBank55Frame122 = {
    SPRITE_FRAME(gSpriteBank55Frame122, SPRITE_TILES_BANK55_FRAME122),
    { { -44, -58, 65, 87 } },
};
const struct sprite_frame_1box gSpriteBank55Frame123 = {
    SPRITE_FRAME(gSpriteBank55Frame123, SPRITE_TILES_BANK55_FRAME123),
    { { -52, -47, 76, 68 } },
};
const struct sprite_frame_1box gSpriteBank55Frame124 = {
    SPRITE_FRAME(gSpriteBank55Frame124, SPRITE_TILES_BANK55_FRAME124),
    { { -60, -38, 84, 63 } },
};
const struct sprite_frame_1box gSpriteBank55Frame125 = {
    SPRITE_FRAME(gSpriteBank55Frame125, SPRITE_TILES_BANK55_FRAME125),
    { { -75, -26, 104, 64 } },
};
const struct sprite_frame_1box gSpriteBank55Frame126 = {
    SPRITE_FRAME(gSpriteBank55Frame126, SPRITE_TILES_BANK55_FRAME126),
    { { -84, -9, 117, 23 } },
};
const struct sprite_frame_1box gSpriteBank55Frame127 = {
    SPRITE_FRAME(gSpriteBank55Frame127, SPRITE_TILES_BANK55_FRAME127),
    { { -92, 10, 126, 9 } },
};
const struct sprite_frame_1box gSpriteBank55Frame128 = {
    SPRITE_FRAME(gSpriteBank55Frame128, SPRITE_TILES_BANK55_FRAME128),
    { { -33, 19, 65, 12 } },
};
const struct sprite_frame_1box gSpriteBank55Frame129 = {
    SPRITE_FRAME(gSpriteBank55Frame129, SPRITE_TILES_BANK55_FRAME129),
    { { 13, 26, 17, 3 } },
};

/* The piece arrays, from graphics/sprites/bank55_tiny/bank.json. */
#include "sprites/bank55_pieces.inc"
