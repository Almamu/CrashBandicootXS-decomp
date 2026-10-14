extern "C" {
#include "gba/types.h"
#include "engine/gfx/sprite_bank.h"
}
#include "sprites/sprite_frames.h"

/*
 * ROM 0x084b0ae0-0x084b414c: sprite banks 10-21 of the sprite-bank
 * animation system (gSpriteBankTable, include/engine/gfx/sprite_bank.h). Linked in
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
 * offset in the sprite tile pool (src/engine/gfx/data/sprite_tiles.cpp),
 * SPRITE_TILES_BANKnn_FRAMEkkk. A frame's struct type has to match its
 * "layout" in bank.json.
 */

/* ---------------------------------------------------------------------- */
/* Bank 10: 2 animations, 20 frames, graphics/sprites/bank10_venus_flytrap/. */

extern const u16 gSpriteBank10Anim00Seq[9];
extern const u16 gSpriteBank10Anim01Seq[11];

extern const struct sprite_anim gSpriteBank10Anims[2] = {
    /* 0 */ {
        /* seq */ gSpriteBank10Anim00Seq,
        /* box */ { { -19, -21, 38, 42 }, { -29, -28, 49, 52 } },
        /* paletteId */ 17,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank10Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank10Anim01Seq,
        /* box */ { { -19, -21, 38, 42 }, { -36, -31, 94, 55 } },
        /* paletteId */ 17,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank10Anim01Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank10Anim00Seq[9] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8,
};
const u16 gSpriteBank10Anim01Seq[11] = {
    9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
};

extern const struct sprite_frame *const gSpriteBank10Frames[20] = {
    &gSpriteBank10Frame000.frame,
    &gSpriteBank10Frame001.frame,
    &gSpriteBank10Frame002.frame,
    &gSpriteBank10Frame003.frame,
    &gSpriteBank10Frame004.frame,
    &gSpriteBank10Frame005.frame,
    &gSpriteBank10Frame006.frame,
    &gSpriteBank10Frame007.frame,
    &gSpriteBank10Frame008.frame,
    &gSpriteBank10Frame009.frame,
    &gSpriteBank10Frame010.frame,
    &gSpriteBank10Frame011.frame,
    &gSpriteBank10Frame012.frame,
    &gSpriteBank10Frame013.frame,
    &gSpriteBank10Frame014.frame,
    &gSpriteBank10Frame015.frame,
    &gSpriteBank10Frame016.frame,
    &gSpriteBank10Frame017.frame,
    &gSpriteBank10Frame018.frame,
    &gSpriteBank10Frame019.frame,
};

const struct sprite_frame_1box gSpriteBank10Frame000 = {
    SPRITE_FRAME(gSpriteBank10Frame000, SPRITE_TILES_BANK10_FRAME000),
    { { -10, -18, 24, 35 } },
};
const struct sprite_frame_1box gSpriteBank10Frame001 = {
    SPRITE_FRAME(gSpriteBank10Frame001, SPRITE_TILES_BANK10_FRAME001),
    { { -12, -22, 25, 39 } },
};
const struct sprite_frame_1box gSpriteBank10Frame002 = {
    SPRITE_FRAME(gSpriteBank10Frame002, SPRITE_TILES_BANK10_FRAME002),
    { { -15, -24, 27, 41 } },
};
const struct sprite_frame_1box gSpriteBank10Frame003 = {
    SPRITE_FRAME(gSpriteBank10Frame003, SPRITE_TILES_BANK10_FRAME003),
    { { -17, -25, 26, 42 } },
};
const struct sprite_frame_1box gSpriteBank10Frame004 = {
    SPRITE_FRAME(gSpriteBank10Frame004, SPRITE_TILES_BANK10_FRAME004),
    { { -17, -26, 27, 44 } },
};
const struct sprite_frame_1box gSpriteBank10Frame005 = {
    SPRITE_FRAME(gSpriteBank10Frame005, SPRITE_TILES_BANK10_FRAME005),
    { { -15, -20, 28, 43 } },
};
const struct sprite_frame_1box gSpriteBank10Frame006 = {
    SPRITE_FRAME(gSpriteBank10Frame006, SPRITE_TILES_BANK10_FRAME006),
    { { -10, -15, 23, 35 } },
};
const struct sprite_frame_1box gSpriteBank10Frame007 = {
    SPRITE_FRAME(gSpriteBank10Frame007, SPRITE_TILES_BANK10_FRAME007),
    { { -10, -12, 22, 33 } },
};
const struct sprite_frame_1box gSpriteBank10Frame008 = {
    SPRITE_FRAME(gSpriteBank10Frame008, SPRITE_TILES_BANK10_FRAME008),
    { { -11, -14, 23, 33 } },
};
const struct sprite_frame_1box gSpriteBank10Frame009 = {
    SPRITE_FRAME(gSpriteBank10Frame009, SPRITE_TILES_BANK10_FRAME009),
    { { -19, -18, 23, 38 } },
};
const struct sprite_frame_1box gSpriteBank10Frame010 = {
    SPRITE_FRAME(gSpriteBank10Frame010, SPRITE_TILES_BANK10_FRAME010),
    { { -24, -14, 27, 33 } },
};
const struct sprite_frame_1box gSpriteBank10Frame011 = {
    SPRITE_FRAME(gSpriteBank10Frame011, SPRITE_TILES_BANK10_FRAME011),
    { { -15, -27, 22, 46 } },
};
const struct sprite_frame_1box gSpriteBank10Frame012 = {
    SPRITE_FRAME(gSpriteBank10Frame012, SPRITE_TILES_BANK10_FRAME012),
    { { -4, -16, 40, 35 } },
};
const struct sprite_frame_1box gSpriteBank10Frame013 = {
    SPRITE_FRAME(gSpriteBank10Frame013, SPRITE_TILES_BANK10_FRAME013),
    { { -5, -5, 63, 26 } },
};
const struct sprite_frame_1box gSpriteBank10Frame014 = {
    SPRITE_FRAME(gSpriteBank10Frame014, SPRITE_TILES_BANK10_FRAME014),
    { { -5, -5, 61, 22 } },
};
const struct sprite_frame_1box gSpriteBank10Frame015 = {
    SPRITE_FRAME(gSpriteBank10Frame015, SPRITE_TILES_BANK10_FRAME015),
    { { -7, -6, 54, 23 } },
};
const struct sprite_frame_1box gSpriteBank10Frame016 = {
    SPRITE_FRAME(gSpriteBank10Frame016, SPRITE_TILES_BANK10_FRAME016),
    { { -8, -11, 51, 25 } },
};
const struct sprite_frame_1box gSpriteBank10Frame017 = {
    SPRITE_FRAME(gSpriteBank10Frame017, SPRITE_TILES_BANK10_FRAME017),
    { { -4, -13, 39, 33 } },
};
const struct sprite_frame_1box gSpriteBank10Frame018 = {
    SPRITE_FRAME(gSpriteBank10Frame018, SPRITE_TILES_BANK10_FRAME018),
    { { -7, -16, 34, 34 } },
};
const struct sprite_frame_1box gSpriteBank10Frame019 = {
    SPRITE_FRAME(gSpriteBank10Frame019, SPRITE_TILES_BANK10_FRAME019),
    { { -7, -17, 29, 35 } },
};

/* The piece arrays, from graphics/sprites/bank10_venus_flytrap/bank.json. */
#include "sprites/bank10_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 11: 2 animations, 25 frames, graphics/sprites/bank11_vulture/. */

extern const u16 gSpriteBank11Anim00Seq[11];
extern const u16 gSpriteBank11Anim01Seq[14];

extern const struct sprite_anim gSpriteBank11Anims[2] = {
    /* 0 */ {
        /* seq */ gSpriteBank11Anim00Seq,
        /* box */ { { -21, -17, 42, 35 }, { -37, -21, 58, 52 } },
        /* paletteId */ 18,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank11Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank11Anim01Seq,
        /* box */ { { -21, -17, 42, 35 }, { -36, -37, 60, 68 } },
        /* paletteId */ 18,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank11Anim01Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank11Anim00Seq[11] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
};
const u16 gSpriteBank11Anim01Seq[14] = {
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
};

extern const struct sprite_frame *const gSpriteBank11Frames[25] = {
    &gSpriteBank11Frame000.frame,
    &gSpriteBank11Frame001.frame,
    &gSpriteBank11Frame002.frame,
    &gSpriteBank11Frame003.frame,
    &gSpriteBank11Frame004.frame,
    &gSpriteBank11Frame005.frame,
    &gSpriteBank11Frame006.frame,
    &gSpriteBank11Frame007.frame,
    &gSpriteBank11Frame008.frame,
    &gSpriteBank11Frame009.frame,
    &gSpriteBank11Frame010.frame,
    &gSpriteBank11Frame011.frame,
    &gSpriteBank11Frame012.frame,
    &gSpriteBank11Frame013.frame,
    &gSpriteBank11Frame014.frame,
    &gSpriteBank11Frame015.frame,
    &gSpriteBank11Frame016.frame,
    &gSpriteBank11Frame017.frame,
    &gSpriteBank11Frame018.frame,
    &gSpriteBank11Frame019.frame,
    &gSpriteBank11Frame020.frame,
    &gSpriteBank11Frame021.frame,
    &gSpriteBank11Frame022.frame,
    &gSpriteBank11Frame023.frame,
    &gSpriteBank11Frame024.frame,
};

const struct sprite_frame_1box gSpriteBank11Frame000 = {
    SPRITE_FRAME(gSpriteBank11Frame000, SPRITE_TILES_BANK11_FRAME000),
    { { -17, -9, 37, 21 } },
};
const struct sprite_frame_1box gSpriteBank11Frame001 = {
    SPRITE_FRAME(gSpriteBank11Frame001, SPRITE_TILES_BANK11_FRAME001),
    { { -23, -12, 41, 24 } },
};
const struct sprite_frame_1box gSpriteBank11Frame002 = {
    SPRITE_FRAME(gSpriteBank11Frame002, SPRITE_TILES_BANK11_FRAME002),
    { { -16, -1, 32, 26 } },
};
const struct sprite_frame_1box gSpriteBank11Frame003 = {
    SPRITE_FRAME(gSpriteBank11Frame003, SPRITE_TILES_BANK11_FRAME003),
    { { -12, -2, 28, 19 } },
};
const struct sprite_frame_1box gSpriteBank11Frame004 = {
    SPRITE_FRAME(gSpriteBank11Frame004, SPRITE_TILES_BANK11_FRAME004),
    { { -19, -4, 37, 17 } },
};
const struct sprite_frame_1box gSpriteBank11Frame005 = {
    SPRITE_FRAME(gSpriteBank11Frame005, SPRITE_TILES_BANK11_FRAME005),
    { { -23, -7, 34, 19 } },
};
const struct sprite_frame_1box gSpriteBank11Frame006 = {
    SPRITE_FRAME(gSpriteBank11Frame006, SPRITE_TILES_BANK11_FRAME006),
    { { -18, -11, 28, 19 } },
};
const struct sprite_frame_1box gSpriteBank11Frame007 = {
    SPRITE_FRAME(gSpriteBank11Frame007, SPRITE_TILES_BANK11_FRAME007),
    { { -31, -12, 39, 17 } },
};
const struct sprite_frame_1box gSpriteBank11Frame008 = {
    SPRITE_FRAME(gSpriteBank11Frame008, SPRITE_TILES_BANK11_FRAME008),
    { { -12, -14, 26, 24 } },
};
const struct sprite_frame_1box gSpriteBank11Frame009 = {
    SPRITE_FRAME(gSpriteBank11Frame009, SPRITE_TILES_BANK11_FRAME009),
    { { -12, -14, 28, 19 } },
};
const struct sprite_frame_1box gSpriteBank11Frame010 = {
    SPRITE_FRAME(gSpriteBank11Frame010, SPRITE_TILES_BANK11_FRAME010),
    { { -17, -15, 32, 21 } },
};
const struct sprite_frame_1box gSpriteBank11Frame011 = {
    SPRITE_FRAME(gSpriteBank11Frame011, SPRITE_TILES_BANK11_FRAME011),
    { { -20, -13, 42, 20 } },
};
const struct sprite_frame_1box gSpriteBank11Frame012 = {
    SPRITE_FRAME(gSpriteBank11Frame012, SPRITE_TILES_BANK11_FRAME012),
    { { -19, -21, 41, 30 } },
};
const struct sprite_frame_1box gSpriteBank11Frame013 = {
    SPRITE_FRAME(gSpriteBank11Frame013, SPRITE_TILES_BANK11_FRAME013),
    { { -9, -27, 26, 42 } },
};
const struct sprite_frame_1box gSpriteBank11Frame014 = {
    SPRITE_FRAME(gSpriteBank11Frame014, SPRITE_TILES_BANK11_FRAME014),
    { { -11, -23, 31, 44 } },
};
const struct sprite_frame_1box gSpriteBank11Frame015 = {
    SPRITE_FRAME(gSpriteBank11Frame015, SPRITE_TILES_BANK11_FRAME015),
    { { -9, -16, 30, 40 } },
};
const struct sprite_frame_1box gSpriteBank11Frame016 = {
    SPRITE_FRAME(gSpriteBank11Frame016, SPRITE_TILES_BANK11_FRAME016),
    { { -11, -10, 30, 38 } },
};
const struct sprite_frame_1box gSpriteBank11Frame017 = {
    SPRITE_FRAME(gSpriteBank11Frame017, SPRITE_TILES_BANK11_FRAME017),
    { { -22, -5, 43, 34 } },
};
const struct sprite_frame_1box gSpriteBank11Frame018 = {
    SPRITE_FRAME(gSpriteBank11Frame018, SPRITE_TILES_BANK11_FRAME018),
    { { -22, -2, 43, 24 } },
};
const struct sprite_frame_1box gSpriteBank11Frame019 = {
    SPRITE_FRAME(gSpriteBank11Frame019, SPRITE_TILES_BANK11_FRAME019),
    { { -27, -2, 49, 23 } },
};
const struct sprite_frame_1box gSpriteBank11Frame020 = {
    SPRITE_FRAME(gSpriteBank11Frame020, SPRITE_TILES_BANK11_FRAME020),
    { { -28, -4, 51, 26 } },
};
const struct sprite_frame_1box gSpriteBank11Frame021 = {
    SPRITE_FRAME(gSpriteBank11Frame021, SPRITE_TILES_BANK11_FRAME021),
    { { -27, -4, 46, 27 } },
};
const struct sprite_frame_1box gSpriteBank11Frame022 = {
    SPRITE_FRAME(gSpriteBank11Frame022, SPRITE_TILES_BANK11_FRAME022),
    { { -29, -3, 49, 24 } },
};
const struct sprite_frame_1box gSpriteBank11Frame023 = {
    SPRITE_FRAME(gSpriteBank11Frame023, SPRITE_TILES_BANK11_FRAME023),
    { { -27, 0, 47, 17 } },
};
const struct sprite_frame_1box gSpriteBank11Frame024 = {
    SPRITE_FRAME(gSpriteBank11Frame024, SPRITE_TILES_BANK11_FRAME024),
    { { -31, -8, 48, 19 } },
};

/* The piece arrays, from graphics/sprites/bank11_vulture/bank.json. */
#include "sprites/bank11_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 12: 7 animations, 57 frames, graphics/sprites/bank12_blowgun_tribesman/. */

extern const u16 gSpriteBank12Anim00Seq[17];
extern const u16 gSpriteBank12Anim01Seq[5];
extern const u16 gSpriteBank12Anim02Seq[6];
extern const u16 gSpriteBank12Anim03Seq[5];
extern const u16 gSpriteBank12Anim04Seq[6];
extern const u16 gSpriteBank12Anim05Seq[16];
extern const u16 gSpriteBank12Anim06Seq[2];

extern const struct sprite_anim gSpriteBank12Anims[7] = {
    /* 0 */ {
        /* seq */ gSpriteBank12Anim00Seq,
        /* box */ { { -14, -27, 28, 55 }, { -19, -30, 44, 58 } },
        /* paletteId */ 19,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank12Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank12Anim01Seq,
        /* box */ { { -14, -27, 28, 55 }, { -15, -14, 33, 42 } },
        /* paletteId */ 19,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank12Anim01Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 2 */ {
        /* seq */ gSpriteBank12Anim02Seq,
        /* box */ { { -14, -27, 28, 55 }, { -15, -27, 32, 55 } },
        /* paletteId */ 19,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank12Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank12Anim03Seq,
        /* box */ { { -14, -27, 28, 55 }, { -15, -27, 29, 55 } },
        /* paletteId */ 19,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank12Anim03Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 4 */ {
        /* seq */ gSpriteBank12Anim04Seq,
        /* box */ { { -14, -27, 28, 55 }, { -15, -27, 32, 55 } },
        /* paletteId */ 19,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank12Anim04Seq),
        /* flags */ 0,
    },
    /* 5 */ {
        /* seq */ gSpriteBank12Anim05Seq,
        /* box */ { { -14, -27, 28, 55 }, { -17, -15, 41, 43 } },
        /* paletteId */ 19,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank12Anim05Seq),
        /* flags */ 0,
    },
    /* 6 */ {
        /* seq */ gSpriteBank12Anim06Seq,
        /* box */ { { -4, -2, 9, 4 }, { -4, -2, 9, 4 } },
        /* paletteId */ 57,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank12Anim06Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank12Anim00Seq[17] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16,
};
const u16 gSpriteBank12Anim01Seq[5] = {
    17, 18, 19, 20, 21,
};
const u16 gSpriteBank12Anim02Seq[6] = {
    22, 23, 24, 25, 26, 27,
};
const u16 gSpriteBank12Anim03Seq[5] = {
    28, 29, 30, 31, 32,
};
const u16 gSpriteBank12Anim04Seq[6] = {
    33, 34, 35, 36, 37, 38,
};
const u16 gSpriteBank12Anim05Seq[16] = {
    39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54,
};
const u16 gSpriteBank12Anim06Seq[2] = {
    55, 56,
};

extern const struct sprite_frame *const gSpriteBank12Frames[57] = {
    &gSpriteBank12Frame000.frame,
    &gSpriteBank12Frame001.frame,
    &gSpriteBank12Frame002.frame,
    &gSpriteBank12Frame003.frame,
    &gSpriteBank12Frame004.frame,
    &gSpriteBank12Frame005.frame,
    &gSpriteBank12Frame006.frame,
    &gSpriteBank12Frame007.frame,
    &gSpriteBank12Frame008.frame,
    &gSpriteBank12Frame009.frame,
    &gSpriteBank12Frame010.frame,
    &gSpriteBank12Frame011.frame,
    &gSpriteBank12Frame012.frame,
    &gSpriteBank12Frame013.frame,
    &gSpriteBank12Frame014.frame,
    &gSpriteBank12Frame015.frame,
    &gSpriteBank12Frame016.frame,
    &gSpriteBank12Frame017.frame,
    &gSpriteBank12Frame018.frame,
    &gSpriteBank12Frame019.frame,
    &gSpriteBank12Frame020.frame,
    &gSpriteBank12Frame021.frame,
    &gSpriteBank12Frame022.frame,
    &gSpriteBank12Frame023.frame,
    &gSpriteBank12Frame024.frame,
    &gSpriteBank12Frame025.frame,
    &gSpriteBank12Frame026.frame,
    &gSpriteBank12Frame027.frame,
    &gSpriteBank12Frame028.frame,
    &gSpriteBank12Frame029.frame,
    &gSpriteBank12Frame030.frame,
    &gSpriteBank12Frame031.frame,
    &gSpriteBank12Frame032.frame,
    &gSpriteBank12Frame033.frame,
    &gSpriteBank12Frame034.frame,
    &gSpriteBank12Frame035.frame,
    &gSpriteBank12Frame036.frame,
    &gSpriteBank12Frame037.frame,
    &gSpriteBank12Frame038.frame,
    &gSpriteBank12Frame039.frame,
    &gSpriteBank12Frame040.frame,
    &gSpriteBank12Frame041.frame,
    &gSpriteBank12Frame042.frame,
    &gSpriteBank12Frame043.frame,
    &gSpriteBank12Frame044.frame,
    &gSpriteBank12Frame045.frame,
    &gSpriteBank12Frame046.frame,
    &gSpriteBank12Frame047.frame,
    &gSpriteBank12Frame048.frame,
    &gSpriteBank12Frame049.frame,
    &gSpriteBank12Frame050.frame,
    &gSpriteBank12Frame051.frame,
    &gSpriteBank12Frame052.frame,
    &gSpriteBank12Frame053.frame,
    &gSpriteBank12Frame054.frame,
    &gSpriteBank12Frame055.frame,
    &gSpriteBank12Frame056.frame,
};

const struct sprite_frame_1box gSpriteBank12Frame000 = {
    SPRITE_FRAME(gSpriteBank12Frame000, SPRITE_TILES_BANK12_FRAME000),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame001 = {
    SPRITE_FRAME(gSpriteBank12Frame001, SPRITE_TILES_BANK12_FRAME001),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame002 = {
    SPRITE_FRAME(gSpriteBank12Frame002, SPRITE_TILES_BANK12_FRAME002),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame003 = {
    SPRITE_FRAME(gSpriteBank12Frame003, SPRITE_TILES_BANK12_FRAME003),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame004 = {
    SPRITE_FRAME(gSpriteBank12Frame004, SPRITE_TILES_BANK12_FRAME004),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame005 = {
    SPRITE_FRAME(gSpriteBank12Frame005, SPRITE_TILES_BANK12_FRAME005),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame006 = {
    SPRITE_FRAME(gSpriteBank12Frame006, SPRITE_TILES_BANK12_FRAME006),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame007 = {
    SPRITE_FRAME(gSpriteBank12Frame007, SPRITE_TILES_BANK12_FRAME007),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame008 = {
    SPRITE_FRAME(gSpriteBank12Frame008, SPRITE_TILES_BANK12_FRAME008),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame009 = {
    SPRITE_FRAME(gSpriteBank12Frame009, SPRITE_TILES_BANK12_FRAME009),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame010 = {
    SPRITE_FRAME(gSpriteBank12Frame010, SPRITE_TILES_BANK12_FRAME010),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame011 = {
    SPRITE_FRAME(gSpriteBank12Frame011, SPRITE_TILES_BANK12_FRAME011),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame012 = {
    SPRITE_FRAME(gSpriteBank12Frame012, SPRITE_TILES_BANK12_FRAME012),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame013 = {
    SPRITE_FRAME(gSpriteBank12Frame013, SPRITE_TILES_BANK12_FRAME013),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame014 = {
    SPRITE_FRAME(gSpriteBank12Frame014, SPRITE_TILES_BANK12_FRAME014),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame015 = {
    SPRITE_FRAME(gSpriteBank12Frame015, SPRITE_TILES_BANK12_FRAME015),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame016 = {
    SPRITE_FRAME(gSpriteBank12Frame016, SPRITE_TILES_BANK12_FRAME016),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame017 = {
    SPRITE_FRAME(gSpriteBank12Frame017, SPRITE_TILES_BANK12_FRAME017),
    { { -6, -5, 6, 31 } },
};
const struct sprite_frame_1box gSpriteBank12Frame018 = {
    SPRITE_FRAME(gSpriteBank12Frame018, SPRITE_TILES_BANK12_FRAME018),
    { { -6, -5, 6, 31 } },
};
const struct sprite_frame_1box gSpriteBank12Frame019 = {
    SPRITE_FRAME(gSpriteBank12Frame019, SPRITE_TILES_BANK12_FRAME019),
    { { -6, -5, 6, 31 } },
};
const struct sprite_frame_1box gSpriteBank12Frame020 = {
    SPRITE_FRAME(gSpriteBank12Frame020, SPRITE_TILES_BANK12_FRAME020),
    { { -6, -5, 6, 31 } },
};
const struct sprite_frame_1box gSpriteBank12Frame021 = {
    SPRITE_FRAME(gSpriteBank12Frame021, SPRITE_TILES_BANK12_FRAME021),
    { { -6, -5, 6, 31 } },
};
const struct sprite_frame_1box gSpriteBank12Frame022 = {
    SPRITE_FRAME(gSpriteBank12Frame022, SPRITE_TILES_BANK12_FRAME022),
    { { -6, -5, 6, 29 } },
};
const struct sprite_frame_1box gSpriteBank12Frame023 = {
    SPRITE_FRAME(gSpriteBank12Frame023, SPRITE_TILES_BANK12_FRAME023),
    { { -6, -5, 6, 29 } },
};
const struct sprite_frame_1box gSpriteBank12Frame024 = {
    SPRITE_FRAME(gSpriteBank12Frame024, SPRITE_TILES_BANK12_FRAME024),
    { { -6, -5, 6, 29 } },
};
const struct sprite_frame_1box gSpriteBank12Frame025 = {
    SPRITE_FRAME(gSpriteBank12Frame025, SPRITE_TILES_BANK12_FRAME025),
    { { -6, -5, 6, 29 } },
};
const struct sprite_frame_1box gSpriteBank12Frame026 = {
    SPRITE_FRAME(gSpriteBank12Frame026, SPRITE_TILES_BANK12_FRAME026),
    { { -6, -5, 6, 29 } },
};
const struct sprite_frame_1box gSpriteBank12Frame027 = {
    SPRITE_FRAME(gSpriteBank12Frame027, SPRITE_TILES_BANK12_FRAME027),
    { { -6, -5, 6, 29 } },
};
const struct sprite_frame_1box gSpriteBank12Frame028 = {
    SPRITE_FRAME(gSpriteBank12Frame028, SPRITE_TILES_BANK12_FRAME028),
    { { -5, -19, 6, 45 } },
};
const struct sprite_frame_1box gSpriteBank12Frame029 = {
    SPRITE_FRAME(gSpriteBank12Frame029, SPRITE_TILES_BANK12_FRAME029),
    { { -5, -19, 6, 45 } },
};
const struct sprite_frame_1box gSpriteBank12Frame030 = {
    SPRITE_FRAME(gSpriteBank12Frame030, SPRITE_TILES_BANK12_FRAME030),
    { { -5, -19, 6, 45 } },
};
const struct sprite_frame_1box gSpriteBank12Frame031 = {
    SPRITE_FRAME(gSpriteBank12Frame031, SPRITE_TILES_BANK12_FRAME031),
    { { -5, -19, 6, 45 } },
};
const struct sprite_frame_1box gSpriteBank12Frame032 = {
    SPRITE_FRAME(gSpriteBank12Frame032, SPRITE_TILES_BANK12_FRAME032),
    { { -5, -19, 6, 45 } },
};
const struct sprite_frame_1box gSpriteBank12Frame033 = {
    SPRITE_FRAME(gSpriteBank12Frame033, SPRITE_TILES_BANK12_FRAME033),
    { { -5, -17, 6, 40 } },
};
const struct sprite_frame_1box gSpriteBank12Frame034 = {
    SPRITE_FRAME(gSpriteBank12Frame034, SPRITE_TILES_BANK12_FRAME034),
    { { -5, -17, 6, 40 } },
};
const struct sprite_frame_1box gSpriteBank12Frame035 = {
    SPRITE_FRAME(gSpriteBank12Frame035, SPRITE_TILES_BANK12_FRAME035),
    { { -5, -17, 6, 40 } },
};
const struct sprite_frame_1box gSpriteBank12Frame036 = {
    SPRITE_FRAME(gSpriteBank12Frame036, SPRITE_TILES_BANK12_FRAME036),
    { { -5, -17, 6, 40 } },
};
const struct sprite_frame_1box gSpriteBank12Frame037 = {
    SPRITE_FRAME(gSpriteBank12Frame037, SPRITE_TILES_BANK12_FRAME037),
    { { -6, -10, 7, 36 } },
};
const struct sprite_frame_1box gSpriteBank12Frame038 = {
    SPRITE_FRAME(gSpriteBank12Frame038, SPRITE_TILES_BANK12_FRAME038),
    { { -6, -9, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame039 = {
    SPRITE_FRAME(gSpriteBank12Frame039, SPRITE_TILES_BANK12_FRAME039),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame040 = {
    SPRITE_FRAME(gSpriteBank12Frame040, SPRITE_TILES_BANK12_FRAME040),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame041 = {
    SPRITE_FRAME(gSpriteBank12Frame041, SPRITE_TILES_BANK12_FRAME041),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame042 = {
    SPRITE_FRAME(gSpriteBank12Frame042, SPRITE_TILES_BANK12_FRAME042),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame043 = {
    SPRITE_FRAME(gSpriteBank12Frame043, SPRITE_TILES_BANK12_FRAME043),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame044 = {
    SPRITE_FRAME(gSpriteBank12Frame044, SPRITE_TILES_BANK12_FRAME044),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame045 = {
    SPRITE_FRAME(gSpriteBank12Frame045, SPRITE_TILES_BANK12_FRAME045),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame046 = {
    SPRITE_FRAME(gSpriteBank12Frame046, SPRITE_TILES_BANK12_FRAME046),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame047 = {
    SPRITE_FRAME(gSpriteBank12Frame047, SPRITE_TILES_BANK12_FRAME047),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame048 = {
    SPRITE_FRAME(gSpriteBank12Frame048, SPRITE_TILES_BANK12_FRAME048),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame049 = {
    SPRITE_FRAME(gSpriteBank12Frame049, SPRITE_TILES_BANK12_FRAME049),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame050 = {
    SPRITE_FRAME(gSpriteBank12Frame050, SPRITE_TILES_BANK12_FRAME050),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame051 = {
    SPRITE_FRAME(gSpriteBank12Frame051, SPRITE_TILES_BANK12_FRAME051),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame052 = {
    SPRITE_FRAME(gSpriteBank12Frame052, SPRITE_TILES_BANK12_FRAME052),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame053 = {
    SPRITE_FRAME(gSpriteBank12Frame053, SPRITE_TILES_BANK12_FRAME053),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame054 = {
    SPRITE_FRAME(gSpriteBank12Frame054, SPRITE_TILES_BANK12_FRAME054),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame055 = {
    SPRITE_FRAME(gSpriteBank12Frame055, SPRITE_TILES_BANK12_FRAME055),
    { { -4, -2, 6, 4 } },
};
const struct sprite_frame_1box gSpriteBank12Frame056 = {
    SPRITE_FRAME(gSpriteBank12Frame056, SPRITE_TILES_BANK12_FRAME056),
    { { -4, -2, 6, 4 } },
};

/* The piece arrays, from graphics/sprites/bank12_blowgun_tribesman/bank.json. */
#include "sprites/bank12_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 13: 2 animations, 19 frames, graphics/sprites/bank13_lizard/. */

extern const u16 gSpriteBank13Anim00Seq[10];
extern const u16 gSpriteBank13Anim01Seq[9];

extern const struct sprite_anim gSpriteBank13Anims[2] = {
    /* 0 */ {
        /* seq */ gSpriteBank13Anim00Seq,
        /* box */ { { -25, -8, 50, 17 }, { -27, -9, 53, 18 } },
        /* paletteId */ 21,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank13Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank13Anim01Seq,
        /* box */ { { -25, -8, 50, 17 }, { -27, -9, 52, 20 } },
        /* paletteId */ 21,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank13Anim01Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank13Anim00Seq[10] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
};
const u16 gSpriteBank13Anim01Seq[9] = {
    10, 11, 12, 13, 14, 15, 16, 17, 18,
};

extern const struct sprite_frame *const gSpriteBank13Frames[19] = {
    &gSpriteBank13Frame000.frame,
    &gSpriteBank13Frame001.frame,
    &gSpriteBank13Frame002.frame,
    &gSpriteBank13Frame003.frame,
    &gSpriteBank13Frame004.frame,
    &gSpriteBank13Frame005.frame,
    &gSpriteBank13Frame006.frame,
    &gSpriteBank13Frame007.frame,
    &gSpriteBank13Frame008.frame,
    &gSpriteBank13Frame009.frame,
    &gSpriteBank13Frame010.frame,
    &gSpriteBank13Frame011.frame,
    &gSpriteBank13Frame012.frame,
    &gSpriteBank13Frame013.frame,
    &gSpriteBank13Frame014.frame,
    &gSpriteBank13Frame015.frame,
    &gSpriteBank13Frame016.frame,
    &gSpriteBank13Frame017.frame,
    &gSpriteBank13Frame018.frame,
};

const struct sprite_frame_1box gSpriteBank13Frame000 = {
    SPRITE_FRAME(gSpriteBank13Frame000, SPRITE_TILES_BANK13_FRAME000),
    { { -12, -7, 31, 11 } },
};
const struct sprite_frame_1box gSpriteBank13Frame001 = {
    SPRITE_FRAME(gSpriteBank13Frame001, SPRITE_TILES_BANK13_FRAME001),
    { { -13, -8, 32, 12 } },
};
const struct sprite_frame_1box gSpriteBank13Frame002 = {
    SPRITE_FRAME(gSpriteBank13Frame002, SPRITE_TILES_BANK13_FRAME002),
    { { -14, -8, 30, 12 } },
};
const struct sprite_frame_1box gSpriteBank13Frame003 = {
    SPRITE_FRAME(gSpriteBank13Frame003, SPRITE_TILES_BANK13_FRAME003),
    { { -15, -7, 31, 12 } },
};
const struct sprite_frame_1box gSpriteBank13Frame004 = {
    SPRITE_FRAME(gSpriteBank13Frame004, SPRITE_TILES_BANK13_FRAME004),
    { { -12, -7, 28, 12 } },
};
const struct sprite_frame_1box gSpriteBank13Frame005 = {
    SPRITE_FRAME(gSpriteBank13Frame005, SPRITE_TILES_BANK13_FRAME005),
    { { -10, -6, 28, 12 } },
};
const struct sprite_frame_1box gSpriteBank13Frame006 = {
    SPRITE_FRAME(gSpriteBank13Frame006, SPRITE_TILES_BANK13_FRAME006),
    { { -10, -6, 25, 12 } },
};
const struct sprite_frame_1box gSpriteBank13Frame007 = {
    SPRITE_FRAME(gSpriteBank13Frame007, SPRITE_TILES_BANK13_FRAME007),
    { { -11, -6, 30, 11 } },
};
const struct sprite_frame_1box gSpriteBank13Frame008 = {
    SPRITE_FRAME(gSpriteBank13Frame008, SPRITE_TILES_BANK13_FRAME008),
    { { -10, -6, 27, 11 } },
};
const struct sprite_frame_1box gSpriteBank13Frame009 = {
    SPRITE_FRAME(gSpriteBank13Frame009, SPRITE_TILES_BANK13_FRAME009),
    { { -10, -7, 28, 12 } },
};
const struct sprite_frame_1box gSpriteBank13Frame010 = {
    SPRITE_FRAME(gSpriteBank13Frame010, SPRITE_TILES_BANK13_FRAME010),
    { { -15, -7, 34, 16 } },
};
const struct sprite_frame_1box gSpriteBank13Frame011 = {
    SPRITE_FRAME(gSpriteBank13Frame011, SPRITE_TILES_BANK13_FRAME011),
    { { -15, -8, 32, 16 } },
};
const struct sprite_frame_1box gSpriteBank13Frame012 = {
    SPRITE_FRAME(gSpriteBank13Frame012, SPRITE_TILES_BANK13_FRAME012),
    { { -13, -7, 32, 12 } },
};
const struct sprite_frame_1box gSpriteBank13Frame013 = {
    SPRITE_FRAME(gSpriteBank13Frame013, SPRITE_TILES_BANK13_FRAME013),
    { { -13, -6, 28, 12 } },
};
const struct sprite_frame_1box gSpriteBank13Frame014 = {
    SPRITE_FRAME(gSpriteBank13Frame014, SPRITE_TILES_BANK13_FRAME014),
    { { -12, -6, 22, 12 } },
};
const struct sprite_frame_1box gSpriteBank13Frame015 = {
    SPRITE_FRAME(gSpriteBank13Frame015, SPRITE_TILES_BANK13_FRAME015),
    { { -16, -7, 25, 12 } },
};
const struct sprite_frame_1box gSpriteBank13Frame016 = {
    SPRITE_FRAME(gSpriteBank13Frame016, SPRITE_TILES_BANK13_FRAME016),
    { { -15, -8, 23, 12 } },
};
const struct sprite_frame_1box gSpriteBank13Frame017 = {
    SPRITE_FRAME(gSpriteBank13Frame017, SPRITE_TILES_BANK13_FRAME017),
    { { -17, -8, 26, 12 } },
};
const struct sprite_frame_1box gSpriteBank13Frame018 = {
    SPRITE_FRAME(gSpriteBank13Frame018, SPRITE_TILES_BANK13_FRAME018),
    { { -19, -7, 30, 12 } },
};

/* The piece arrays, from graphics/sprites/bank13_lizard/bank.json. */
#include "sprites/bank13_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 14: 2 animations, 23 frames, graphics/sprites/bank14_patrolling_jungle_enemy/. */

extern const u16 gSpriteBank14Anim00Seq[12];
extern const u16 gSpriteBank14Anim01Seq[11];

extern const struct sprite_anim gSpriteBank14Anims[2] = {
    /* 0 */ {
        /* seq */ gSpriteBank14Anim00Seq,
        /* box */ { { -22, -21, 44, 42 }, { -25, -25, 50, 47 } },
        /* paletteId */ 22,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank14Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank14Anim01Seq,
        /* box */ { { -22, -21, 44, 42 }, { -22, -25, 44, 47 } },
        /* paletteId */ 22,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank14Anim01Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank14Anim00Seq[12] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
};
const u16 gSpriteBank14Anim01Seq[11] = {
    12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
};

extern const struct sprite_frame *const gSpriteBank14Frames[23] = {
    &gSpriteBank14Frame000.frame,
    &gSpriteBank14Frame001.frame,
    &gSpriteBank14Frame002.frame,
    &gSpriteBank14Frame003.frame,
    &gSpriteBank14Frame004.frame,
    &gSpriteBank14Frame005.frame,
    &gSpriteBank14Frame006.frame,
    &gSpriteBank14Frame007.frame,
    &gSpriteBank14Frame008.frame,
    &gSpriteBank14Frame009.frame,
    &gSpriteBank14Frame010.frame,
    &gSpriteBank14Frame011.frame,
    &gSpriteBank14Frame012.frame,
    &gSpriteBank14Frame013.frame,
    &gSpriteBank14Frame014.frame,
    &gSpriteBank14Frame015.frame,
    &gSpriteBank14Frame016.frame,
    &gSpriteBank14Frame017.frame,
    &gSpriteBank14Frame018.frame,
    &gSpriteBank14Frame019.frame,
    &gSpriteBank14Frame020.frame,
    &gSpriteBank14Frame021.frame,
    &gSpriteBank14Frame022.frame,
};

const struct sprite_frame_2box gSpriteBank14Frame000 = {
    SPRITE_FRAME(gSpriteBank14Frame000, SPRITE_TILES_BANK14_FRAME000),
    { { -10, 5, 24, 14 }, { -16, -12, 41, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame001 = {
    SPRITE_FRAME(gSpriteBank14Frame001, SPRITE_TILES_BANK14_FRAME001),
    { { -10, 5, 24, 14 }, { -16, -12, 41, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame002 = {
    SPRITE_FRAME(gSpriteBank14Frame002, SPRITE_TILES_BANK14_FRAME002),
    { { -10, 5, 24, 14 }, { -16, -12, 41, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame003 = {
    SPRITE_FRAME(gSpriteBank14Frame003, SPRITE_TILES_BANK14_FRAME003),
    { { -10, 5, 24, 14 }, { -16, -12, 41, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame004 = {
    SPRITE_FRAME(gSpriteBank14Frame004, SPRITE_TILES_BANK14_FRAME004),
    { { -10, 5, 24, 14 }, { -16, -12, 41, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame005 = {
    SPRITE_FRAME(gSpriteBank14Frame005, SPRITE_TILES_BANK14_FRAME005),
    { { -10, 5, 24, 14 }, { -16, -12, 41, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame006 = {
    SPRITE_FRAME(gSpriteBank14Frame006, SPRITE_TILES_BANK14_FRAME006),
    { { -10, 5, 24, 14 }, { -16, -12, 41, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame007 = {
    SPRITE_FRAME(gSpriteBank14Frame007, SPRITE_TILES_BANK14_FRAME007),
    { { -10, 5, 24, 14 }, { -16, -12, 41, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame008 = {
    SPRITE_FRAME(gSpriteBank14Frame008, SPRITE_TILES_BANK14_FRAME008),
    { { -10, 5, 24, 14 }, { -16, -12, 41, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame009 = {
    SPRITE_FRAME(gSpriteBank14Frame009, SPRITE_TILES_BANK14_FRAME009),
    { { -10, 5, 24, 14 }, { -16, -12, 41, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame010 = {
    SPRITE_FRAME(gSpriteBank14Frame010, SPRITE_TILES_BANK14_FRAME010),
    { { -10, 5, 24, 14 }, { -16, -12, 41, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame011 = {
    SPRITE_FRAME(gSpriteBank14Frame011, SPRITE_TILES_BANK14_FRAME011),
    { { -10, 5, 24, 14 }, { -16, -12, 41, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame012 = {
    SPRITE_FRAME(gSpriteBank14Frame012, SPRITE_TILES_BANK14_FRAME012),
    { { -12, 4, 22, 16 }, { -19, -13, 40, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame013 = {
    SPRITE_FRAME(gSpriteBank14Frame013, SPRITE_TILES_BANK14_FRAME013),
    { { -12, 4, 22, 16 }, { -19, -13, 40, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame014 = {
    SPRITE_FRAME(gSpriteBank14Frame014, SPRITE_TILES_BANK14_FRAME014),
    { { -12, 4, 22, 16 }, { -19, -13, 40, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame015 = {
    SPRITE_FRAME(gSpriteBank14Frame015, SPRITE_TILES_BANK14_FRAME015),
    { { -12, 4, 22, 16 }, { -19, -13, 40, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame016 = {
    SPRITE_FRAME(gSpriteBank14Frame016, SPRITE_TILES_BANK14_FRAME016),
    { { -12, 4, 22, 16 }, { -19, -13, 40, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame017 = {
    SPRITE_FRAME(gSpriteBank14Frame017, SPRITE_TILES_BANK14_FRAME017),
    { { -12, 4, 22, 16 }, { -19, -13, 40, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame018 = {
    SPRITE_FRAME(gSpriteBank14Frame018, SPRITE_TILES_BANK14_FRAME018),
    { { -12, 4, 22, 16 }, { -19, -13, 40, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame019 = {
    SPRITE_FRAME(gSpriteBank14Frame019, SPRITE_TILES_BANK14_FRAME019),
    { { -12, 4, 22, 16 }, { -19, -13, 40, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame020 = {
    SPRITE_FRAME(gSpriteBank14Frame020, SPRITE_TILES_BANK14_FRAME020),
    { { -12, 4, 22, 16 }, { -19, -13, 40, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame021 = {
    SPRITE_FRAME(gSpriteBank14Frame021, SPRITE_TILES_BANK14_FRAME021),
    { { -12, 4, 22, 16 }, { -19, -13, 40, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame022 = {
    SPRITE_FRAME(gSpriteBank14Frame022, SPRITE_TILES_BANK14_FRAME022),
    { { -12, 4, 22, 16 }, { -19, -13, 40, 13 } },
};

/* The piece arrays, from graphics/sprites/bank14_patrolling_jungle_enemy/bank.json. */
#include "sprites/bank14_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 15: 5 animations, 58 frames, graphics/sprites/bank15_penguin/. */

extern const u16 gSpriteBank15Anim00Seq[15];
extern const u16 gSpriteBank15Anim01Seq[4];
extern const u16 gSpriteBank15Anim02Seq[9];
extern const u16 gSpriteBank15Anim03Seq[15];
extern const u16 gSpriteBank15Anim04Seq[17];

extern const struct sprite_anim gSpriteBank15Anims[5] = {
    /* 0 */ {
        /* seq */ gSpriteBank15Anim00Seq,
        /* box */ { { -10, -13, 20, 27 }, { -14, -15, 25, 30 } },
        /* paletteId */ 23,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank15Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank15Anim01Seq,
        /* box */ { { -10, -13, 20, 27 }, { -17, -15, 33, 30 } },
        /* paletteId */ 23,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank15Anim01Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 2 */ {
        /* seq */ gSpriteBank15Anim02Seq,
        /* box */ { { -10, -13, 20, 27 }, { -16, -15, 29, 30 } },
        /* paletteId */ 23,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank15Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank15Anim03Seq,
        /* box */ { { -10, -13, 20, 27 }, { -14, -15, 25, 30 } },
        /* paletteId */ 23,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank15Anim03Seq),
        /* flags */ 0,
    },
    /* 4 */ {
        /* seq */ gSpriteBank15Anim04Seq,
        /* box */ { { -10, -13, 20, 27 }, { -14, -15, 27, 30 } },
        /* paletteId */ 23,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank15Anim04Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank15Anim00Seq[15] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
};
const u16 gSpriteBank15Anim01Seq[4] = {
    15, 16, 17, 18,
};
const u16 gSpriteBank15Anim02Seq[9] = {
    19, 20, 21, 22, 23, 24, 25, 26, 27,
};
const u16 gSpriteBank15Anim03Seq[15] = {
    0, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
};
const u16 gSpriteBank15Anim04Seq[17] = {
    42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 51, 53, 54, 55, 56,
    57,
};

extern const struct sprite_frame *const gSpriteBank15Frames[58] = {
    &gSpriteBank15Frame000.frame,
    &gSpriteBank15Frame001.frame,
    &gSpriteBank15Frame002.frame,
    &gSpriteBank15Frame003.frame,
    &gSpriteBank15Frame004.frame,
    &gSpriteBank15Frame005.frame,
    &gSpriteBank15Frame006.frame,
    &gSpriteBank15Frame007.frame,
    &gSpriteBank15Frame008.frame,
    &gSpriteBank15Frame009.frame,
    &gSpriteBank15Frame010.frame,
    &gSpriteBank15Frame011.frame,
    &gSpriteBank15Frame012.frame,
    &gSpriteBank15Frame013.frame,
    &gSpriteBank15Frame014.frame,
    &gSpriteBank15Frame015.frame,
    &gSpriteBank15Frame016.frame,
    &gSpriteBank15Frame017.frame,
    &gSpriteBank15Frame018.frame,
    &gSpriteBank15Frame019.frame,
    &gSpriteBank15Frame020.frame,
    &gSpriteBank15Frame021.frame,
    &gSpriteBank15Frame022.frame,
    &gSpriteBank15Frame023.frame,
    &gSpriteBank15Frame024.frame,
    &gSpriteBank15Frame025.frame,
    &gSpriteBank15Frame026.frame,
    &gSpriteBank15Frame027.frame,
    &gSpriteBank15Frame028.frame,
    &gSpriteBank15Frame029.frame,
    &gSpriteBank15Frame030.frame,
    &gSpriteBank15Frame031.frame,
    &gSpriteBank15Frame032.frame,
    &gSpriteBank15Frame033.frame,
    &gSpriteBank15Frame034.frame,
    &gSpriteBank15Frame035.frame,
    &gSpriteBank15Frame036.frame,
    &gSpriteBank15Frame037.frame,
    &gSpriteBank15Frame038.frame,
    &gSpriteBank15Frame039.frame,
    &gSpriteBank15Frame040.frame,
    &gSpriteBank15Frame041.frame,
    &gSpriteBank15Frame042.frame,
    &gSpriteBank15Frame043.frame,
    &gSpriteBank15Frame044.frame,
    &gSpriteBank15Frame045.frame,
    &gSpriteBank15Frame046.frame,
    &gSpriteBank15Frame047.frame,
    &gSpriteBank15Frame048.frame,
    &gSpriteBank15Frame049.frame,
    &gSpriteBank15Frame050.frame,
    &gSpriteBank15Frame051.frame,
    &gSpriteBank15Frame052.frame,
    &gSpriteBank15Frame053.frame,
    &gSpriteBank15Frame054.frame,
    &gSpriteBank15Frame055.frame,
    &gSpriteBank15Frame056.frame,
    &gSpriteBank15Frame057.frame,
};

const struct sprite_frame_1box gSpriteBank15Frame000 = {
    SPRITE_FRAME(gSpriteBank15Frame000, SPRITE_TILES_BANK15_FRAME000),
    { { -6, -13, 12, 27 } },
};
const struct sprite_frame_1box gSpriteBank15Frame001 = {
    SPRITE_FRAME(gSpriteBank15Frame001, SPRITE_TILES_BANK15_FRAME001),
    { { -6, -13, 12, 27 } },
};
const struct sprite_frame_1box gSpriteBank15Frame002 = {
    SPRITE_FRAME(gSpriteBank15Frame002, SPRITE_TILES_BANK15_FRAME002),
    { { -6, -14, 11, 27 } },
};
const struct sprite_frame_1box gSpriteBank15Frame003 = {
    SPRITE_FRAME(gSpriteBank15Frame003, SPRITE_TILES_BANK15_FRAME003),
    { { -7, -15, 11, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame004 = {
    SPRITE_FRAME(gSpriteBank15Frame004, SPRITE_TILES_BANK15_FRAME004),
    { { -9, -15, 12, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame005 = {
    SPRITE_FRAME(gSpriteBank15Frame005, SPRITE_TILES_BANK15_FRAME005),
    { { -6, -12, 12, 23 } },
};
const struct sprite_frame_1box gSpriteBank15Frame006 = {
    SPRITE_FRAME(gSpriteBank15Frame006, SPRITE_TILES_BANK15_FRAME006),
    { { -6, -12, 12, 23 } },
};
const struct sprite_frame_1box gSpriteBank15Frame007 = {
    SPRITE_FRAME(gSpriteBank15Frame007, SPRITE_TILES_BANK15_FRAME007),
    { { -8, -14, 12, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame008 = {
    SPRITE_FRAME(gSpriteBank15Frame008, SPRITE_TILES_BANK15_FRAME008),
    { { -8, -14, 12, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame009 = {
    SPRITE_FRAME(gSpriteBank15Frame009, SPRITE_TILES_BANK15_FRAME009),
    { { -8, -14, 12, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame010 = {
    SPRITE_FRAME(gSpriteBank15Frame010, SPRITE_TILES_BANK15_FRAME010),
    { { -9, -15, 13, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame011 = {
    SPRITE_FRAME(gSpriteBank15Frame011, SPRITE_TILES_BANK15_FRAME011),
    { { -9, -15, 15, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame012 = {
    SPRITE_FRAME(gSpriteBank15Frame012, SPRITE_TILES_BANK15_FRAME012),
    { { -9, -15, 16, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame013 = {
    SPRITE_FRAME(gSpriteBank15Frame013, SPRITE_TILES_BANK15_FRAME013),
    { { -8, -15, 15, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame014 = {
    SPRITE_FRAME(gSpriteBank15Frame014, SPRITE_TILES_BANK15_FRAME014),
    { { -6, -14, 12, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame015 = {
    SPRITE_FRAME(gSpriteBank15Frame015, SPRITE_TILES_BANK15_FRAME015),
    { { -9, -13, 18, 27 } },
};
const struct sprite_frame_1box gSpriteBank15Frame016 = {
    SPRITE_FRAME(gSpriteBank15Frame016, SPRITE_TILES_BANK15_FRAME016),
    { { -9, -13, 18, 27 } },
};
const struct sprite_frame_1box gSpriteBank15Frame017 = {
    SPRITE_FRAME(gSpriteBank15Frame017, SPRITE_TILES_BANK15_FRAME017),
    { { -9, -13, 18, 27 } },
};
const struct sprite_frame_1box gSpriteBank15Frame018 = {
    SPRITE_FRAME(gSpriteBank15Frame018, SPRITE_TILES_BANK15_FRAME018),
    { { -9, -13, 18, 27 } },
};
const struct sprite_frame_1box gSpriteBank15Frame019 = {
    SPRITE_FRAME(gSpriteBank15Frame019, SPRITE_TILES_BANK15_FRAME019),
    { { -4, -13, 10, 27 } },
};
const struct sprite_frame_1box gSpriteBank15Frame020 = {
    SPRITE_FRAME(gSpriteBank15Frame020, SPRITE_TILES_BANK15_FRAME020),
    { { -4, -14, 10, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame021 = {
    SPRITE_FRAME(gSpriteBank15Frame021, SPRITE_TILES_BANK15_FRAME021),
    { { -6, -14, 13, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame022 = {
    SPRITE_FRAME(gSpriteBank15Frame022, SPRITE_TILES_BANK15_FRAME022),
    { { -9, -14, 16, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame023 = {
    SPRITE_FRAME(gSpriteBank15Frame023, SPRITE_TILES_BANK15_FRAME023),
    { { -10, -15, 17, 30 } },
};
const struct sprite_frame_1box gSpriteBank15Frame024 = {
    SPRITE_FRAME(gSpriteBank15Frame024, SPRITE_TILES_BANK15_FRAME024),
    { { -10, -15, 17, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame025 = {
    SPRITE_FRAME(gSpriteBank15Frame025, SPRITE_TILES_BANK15_FRAME025),
    { { -10, -15, 18, 30 } },
};
const struct sprite_frame_1box gSpriteBank15Frame026 = {
    SPRITE_FRAME(gSpriteBank15Frame026, SPRITE_TILES_BANK15_FRAME026),
    { { -10, -14, 19, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame027 = {
    SPRITE_FRAME(gSpriteBank15Frame027, SPRITE_TILES_BANK15_FRAME027),
    { { -8, -14, 14, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame028 = {
    SPRITE_FRAME(gSpriteBank15Frame028, SPRITE_TILES_BANK15_FRAME028),
    { { -6, -13, 12, 27 } },
};
const struct sprite_frame_1box gSpriteBank15Frame029 = {
    SPRITE_FRAME(gSpriteBank15Frame029, SPRITE_TILES_BANK15_FRAME029),
    { { -7, -14, 12, 27 } },
};
const struct sprite_frame_1box gSpriteBank15Frame030 = {
    SPRITE_FRAME(gSpriteBank15Frame030, SPRITE_TILES_BANK15_FRAME030),
    { { -8, -15, 12, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame031 = {
    SPRITE_FRAME(gSpriteBank15Frame031, SPRITE_TILES_BANK15_FRAME031),
    { { -10, -15, 15, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame032 = {
    SPRITE_FRAME(gSpriteBank15Frame032, SPRITE_TILES_BANK15_FRAME032),
    { { -10, -15, 17, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame033 = {
    SPRITE_FRAME(gSpriteBank15Frame033, SPRITE_TILES_BANK15_FRAME033),
    { { -9, -15, 16, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame034 = {
    SPRITE_FRAME(gSpriteBank15Frame034, SPRITE_TILES_BANK15_FRAME034),
    { { -7, -14, 11, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame035 = {
    SPRITE_FRAME(gSpriteBank15Frame035, SPRITE_TILES_BANK15_FRAME035),
    { { -7, -14, 11, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame036 = {
    SPRITE_FRAME(gSpriteBank15Frame036, SPRITE_TILES_BANK15_FRAME036),
    { { -7, -14, 12, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame037 = {
    SPRITE_FRAME(gSpriteBank15Frame037, SPRITE_TILES_BANK15_FRAME037),
    { { -7, -15, 12, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame038 = {
    SPRITE_FRAME(gSpriteBank15Frame038, SPRITE_TILES_BANK15_FRAME038),
    { { -8, -15, 14, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame039 = {
    SPRITE_FRAME(gSpriteBank15Frame039, SPRITE_TILES_BANK15_FRAME039),
    { { -9, -15, 16, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame040 = {
    SPRITE_FRAME(gSpriteBank15Frame040, SPRITE_TILES_BANK15_FRAME040),
    { { -9, -14, 16, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame041 = {
    SPRITE_FRAME(gSpriteBank15Frame041, SPRITE_TILES_BANK15_FRAME041),
    { { -8, -14, 14, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame042 = {
    SPRITE_FRAME(gSpriteBank15Frame042, SPRITE_TILES_BANK15_FRAME042),
    { { -7, -14, 14, 27 } },
};
const struct sprite_frame_1box gSpriteBank15Frame043 = {
    SPRITE_FRAME(gSpriteBank15Frame043, SPRITE_TILES_BANK15_FRAME043),
    { { -11, -14, 18, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame044 = {
    SPRITE_FRAME(gSpriteBank15Frame044, SPRITE_TILES_BANK15_FRAME044),
    { { -10, -14, 16, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame045 = {
    SPRITE_FRAME(gSpriteBank15Frame045, SPRITE_TILES_BANK15_FRAME045),
    { { -8, -14, 15, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame046 = {
    SPRITE_FRAME(gSpriteBank15Frame046, SPRITE_TILES_BANK15_FRAME046),
    { { -4, -14, 14, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame047 = {
    SPRITE_FRAME(gSpriteBank15Frame047, SPRITE_TILES_BANK15_FRAME047),
    { { -4, -14, 12, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame048 = {
    SPRITE_FRAME(gSpriteBank15Frame048, SPRITE_TILES_BANK15_FRAME048),
    { { -4, -14, 11, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame049 = {
    SPRITE_FRAME(gSpriteBank15Frame049, SPRITE_TILES_BANK15_FRAME049),
    { { -4, -14, 12, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame050 = {
    SPRITE_FRAME(gSpriteBank15Frame050, SPRITE_TILES_BANK15_FRAME050),
    { { -4, -14, 12, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame051 = {
    SPRITE_FRAME(gSpriteBank15Frame051, SPRITE_TILES_BANK15_FRAME051),
    { { -4, -14, 12, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame052 = {
    SPRITE_FRAME(gSpriteBank15Frame052, SPRITE_TILES_BANK15_FRAME052),
    { { -4, -14, 12, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame053 = {
    SPRITE_FRAME(gSpriteBank15Frame053, SPRITE_TILES_BANK15_FRAME053),
    { { -4, -14, 12, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame054 = {
    SPRITE_FRAME(gSpriteBank15Frame054, SPRITE_TILES_BANK15_FRAME054),
    { { -4, -14, 12, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame055 = {
    SPRITE_FRAME(gSpriteBank15Frame055, SPRITE_TILES_BANK15_FRAME055),
    { { -5, -15, 13, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame056 = {
    SPRITE_FRAME(gSpriteBank15Frame056, SPRITE_TILES_BANK15_FRAME056),
    { { -6, -14, 14, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame057 = {
    SPRITE_FRAME(gSpriteBank15Frame057, SPRITE_TILES_BANK15_FRAME057),
    { { -7, -14, 14, 28 } },
};

/* The piece arrays, from graphics/sprites/bank15_penguin/bank.json. */
#include "sprites/bank15_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 16: 2 animations, 24 frames, graphics/sprites/bank16_polar_bear/. */

extern const u16 gSpriteBank16Anim00Seq[12];
extern const u16 gSpriteBank16Anim01Seq[12];

extern const struct sprite_anim gSpriteBank16Anims[2] = {
    /* 0 */ {
        /* seq */ gSpriteBank16Anim00Seq,
        /* box */ { { -30, -14, 60, 29 }, { -38, -14, 71, 29 } },
        /* paletteId */ 24,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank16Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank16Anim01Seq,
        /* box */ { { -30, -14, 60, 29 }, { -35, -14, 65, 29 } },
        /* paletteId */ 24,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank16Anim01Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank16Anim00Seq[12] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
};
const u16 gSpriteBank16Anim01Seq[12] = {
    12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
};

extern const struct sprite_frame *const gSpriteBank16Frames[24] = {
    &gSpriteBank16Frame000.frame,
    &gSpriteBank16Frame001.frame,
    &gSpriteBank16Frame002.frame,
    &gSpriteBank16Frame003.frame,
    &gSpriteBank16Frame004.frame,
    &gSpriteBank16Frame005.frame,
    &gSpriteBank16Frame006.frame,
    &gSpriteBank16Frame007.frame,
    &gSpriteBank16Frame008.frame,
    &gSpriteBank16Frame009.frame,
    &gSpriteBank16Frame010.frame,
    &gSpriteBank16Frame011.frame,
    &gSpriteBank16Frame012.frame,
    &gSpriteBank16Frame013.frame,
    &gSpriteBank16Frame014.frame,
    &gSpriteBank16Frame015.frame,
    &gSpriteBank16Frame016.frame,
    &gSpriteBank16Frame017.frame,
    &gSpriteBank16Frame018.frame,
    &gSpriteBank16Frame019.frame,
    &gSpriteBank16Frame020.frame,
    &gSpriteBank16Frame021.frame,
    &gSpriteBank16Frame022.frame,
    &gSpriteBank16Frame023.frame,
};

const struct sprite_frame_1box gSpriteBank16Frame000 = {
    SPRITE_FRAME(gSpriteBank16Frame000, SPRITE_TILES_BANK16_FRAME000),
    { { -25, -10, 50, 21 } },
};
const struct sprite_frame_1box gSpriteBank16Frame001 = {
    SPRITE_FRAME(gSpriteBank16Frame001, SPRITE_TILES_BANK16_FRAME001),
    { { -24, -9, 49, 20 } },
};
const struct sprite_frame_1box gSpriteBank16Frame002 = {
    SPRITE_FRAME(gSpriteBank16Frame002, SPRITE_TILES_BANK16_FRAME002),
    { { -24, -8, 49, 18 } },
};
const struct sprite_frame_1box gSpriteBank16Frame003 = {
    SPRITE_FRAME(gSpriteBank16Frame003, SPRITE_TILES_BANK16_FRAME003),
    { { -23, -8, 49, 18 } },
};
const struct sprite_frame_1box gSpriteBank16Frame004 = {
    SPRITE_FRAME(gSpriteBank16Frame004, SPRITE_TILES_BANK16_FRAME004),
    { { -23, -8, 49, 18 } },
};
const struct sprite_frame_1box gSpriteBank16Frame005 = {
    SPRITE_FRAME(gSpriteBank16Frame005, SPRITE_TILES_BANK16_FRAME005),
    { { -25, -9, 52, 20 } },
};
const struct sprite_frame_1box gSpriteBank16Frame006 = {
    SPRITE_FRAME(gSpriteBank16Frame006, SPRITE_TILES_BANK16_FRAME006),
    { { -29, -10, 57, 21 } },
};
const struct sprite_frame_1box gSpriteBank16Frame007 = {
    SPRITE_FRAME(gSpriteBank16Frame007, SPRITE_TILES_BANK16_FRAME007),
    { { -31, -9, 59, 20 } },
};
const struct sprite_frame_1box gSpriteBank16Frame008 = {
    SPRITE_FRAME(gSpriteBank16Frame008, SPRITE_TILES_BANK16_FRAME008),
    { { -33, -8, 61, 18 } },
};
const struct sprite_frame_1box gSpriteBank16Frame009 = {
    SPRITE_FRAME(gSpriteBank16Frame009, SPRITE_TILES_BANK16_FRAME009),
    { { -33, -8, 60, 18 } },
};
const struct sprite_frame_1box gSpriteBank16Frame010 = {
    SPRITE_FRAME(gSpriteBank16Frame010, SPRITE_TILES_BANK16_FRAME010),
    { { -33, -8, 59, 18 } },
};
const struct sprite_frame_1box gSpriteBank16Frame011 = {
    SPRITE_FRAME(gSpriteBank16Frame011, SPRITE_TILES_BANK16_FRAME011),
    { { -29, -9, 55, 19 } },
};
const struct sprite_frame_1box gSpriteBank16Frame012 = {
    SPRITE_FRAME(gSpriteBank16Frame012, SPRITE_TILES_BANK16_FRAME012),
    { { -27, -11, 54, 23 } },
};
const struct sprite_frame_1box gSpriteBank16Frame013 = {
    SPRITE_FRAME(gSpriteBank16Frame013, SPRITE_TILES_BANK16_FRAME013),
    { { -25, -10, 51, 22 } },
};
const struct sprite_frame_1box gSpriteBank16Frame014 = {
    SPRITE_FRAME(gSpriteBank16Frame014, SPRITE_TILES_BANK16_FRAME014),
    { { -23, -9, 47, 20 } },
};
const struct sprite_frame_1box gSpriteBank16Frame015 = {
    SPRITE_FRAME(gSpriteBank16Frame015, SPRITE_TILES_BANK16_FRAME015),
    { { -20, -9, 41, 20 } },
};
const struct sprite_frame_1box gSpriteBank16Frame016 = {
    SPRITE_FRAME(gSpriteBank16Frame016, SPRITE_TILES_BANK16_FRAME016),
    { { -19, -9, 36, 20 } },
};
const struct sprite_frame_1box gSpriteBank16Frame017 = {
    SPRITE_FRAME(gSpriteBank16Frame017, SPRITE_TILES_BANK16_FRAME017),
    { { -18, -10, 31, 21 } },
};
const struct sprite_frame_1box gSpriteBank16Frame018 = {
    SPRITE_FRAME(gSpriteBank16Frame018, SPRITE_TILES_BANK16_FRAME018),
    { { -13, -10, 23, 22 } },
};
const struct sprite_frame_1box gSpriteBank16Frame019 = {
    SPRITE_FRAME(gSpriteBank16Frame019, SPRITE_TILES_BANK16_FRAME019),
    { { -13, -10, 27, 22 } },
};
const struct sprite_frame_1box gSpriteBank16Frame020 = {
    SPRITE_FRAME(gSpriteBank16Frame020, SPRITE_TILES_BANK16_FRAME020),
    { { -18, -9, 34, 20 } },
};
const struct sprite_frame_1box gSpriteBank16Frame021 = {
    SPRITE_FRAME(gSpriteBank16Frame021, SPRITE_TILES_BANK16_FRAME021),
    { { -24, -9, 41, 20 } },
};
const struct sprite_frame_1box gSpriteBank16Frame022 = {
    SPRITE_FRAME(gSpriteBank16Frame022, SPRITE_TILES_BANK16_FRAME022),
    { { -29, -10, 48, 21 } },
};
const struct sprite_frame_1box gSpriteBank16Frame023 = {
    SPRITE_FRAME(gSpriteBank16Frame023, SPRITE_TILES_BANK16_FRAME023),
    { { -32, -11, 55, 22 } },
};

/* The piece arrays, from graphics/sprites/bank16_polar_bear/bank.json. */
#include "sprites/bank16_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 17: 1 animation, 15 frames, graphics/sprites/bank17_seal/. */

extern const u16 gSpriteBank17Anim00Seq[15];

extern const struct sprite_anim gSpriteBank17Anims[1] = {
    /* 0 */ {
        /* seq */ gSpriteBank17Anim00Seq,
        /* box */ { { 7, -4, 11, 18 }, { -30, -19, 54, 35 } },
        /* paletteId */ 25,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank17Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank17Anim00Seq[15] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
};

extern const struct sprite_frame *const gSpriteBank17Frames[15] = {
    &gSpriteBank17Frame000.frame,
    &gSpriteBank17Frame001.frame,
    &gSpriteBank17Frame002.frame,
    &gSpriteBank17Frame003.frame,
    &gSpriteBank17Frame004.frame,
    &gSpriteBank17Frame005.frame,
    &gSpriteBank17Frame006.frame,
    &gSpriteBank17Frame007.frame,
    &gSpriteBank17Frame008.frame,
    &gSpriteBank17Frame009.frame,
    &gSpriteBank17Frame010.frame,
    &gSpriteBank17Frame011.frame,
    &gSpriteBank17Frame012.frame,
    &gSpriteBank17Frame013.frame,
    &gSpriteBank17Frame014.frame,
};

const struct sprite_frame_1box gSpriteBank17Frame000 = {
    SPRITE_FRAME(gSpriteBank17Frame000, SPRITE_TILES_BANK17_FRAME000),
    { { -10, -4, 29, 13 } },
};
const struct sprite_frame_1box gSpriteBank17Frame001 = {
    SPRITE_FRAME(gSpriteBank17Frame001, SPRITE_TILES_BANK17_FRAME001),
    { { -10, -4, 29, 13 } },
};
const struct sprite_frame_1box gSpriteBank17Frame002 = {
    SPRITE_FRAME(gSpriteBank17Frame002, SPRITE_TILES_BANK17_FRAME002),
    { { -10, -4, 29, 13 } },
};
const struct sprite_frame_1box gSpriteBank17Frame003 = {
    SPRITE_FRAME(gSpriteBank17Frame003, SPRITE_TILES_BANK17_FRAME003),
    { { -10, -4, 29, 13 } },
};
const struct sprite_frame_1box gSpriteBank17Frame004 = {
    SPRITE_FRAME(gSpriteBank17Frame004, SPRITE_TILES_BANK17_FRAME004),
    { { -10, -4, 29, 13 } },
};
const struct sprite_frame_1box gSpriteBank17Frame005 = {
    SPRITE_FRAME(gSpriteBank17Frame005, SPRITE_TILES_BANK17_FRAME005),
    { { -10, -4, 29, 13 } },
};
const struct sprite_frame_1box gSpriteBank17Frame006 = {
    SPRITE_FRAME(gSpriteBank17Frame006, SPRITE_TILES_BANK17_FRAME006),
    { { -10, -4, 29, 13 } },
};
const struct sprite_frame_1box gSpriteBank17Frame007 = {
    SPRITE_FRAME(gSpriteBank17Frame007, SPRITE_TILES_BANK17_FRAME007),
    { { -10, -4, 29, 13 } },
};
const struct sprite_frame_1box gSpriteBank17Frame008 = {
    SPRITE_FRAME(gSpriteBank17Frame008, SPRITE_TILES_BANK17_FRAME008),
    { { -10, -4, 29, 13 } },
};
const struct sprite_frame_1box gSpriteBank17Frame009 = {
    SPRITE_FRAME(gSpriteBank17Frame009, SPRITE_TILES_BANK17_FRAME009),
    { { -10, -4, 29, 13 } },
};
const struct sprite_frame_1box gSpriteBank17Frame010 = {
    SPRITE_FRAME(gSpriteBank17Frame010, SPRITE_TILES_BANK17_FRAME010),
    { { -10, -4, 29, 13 } },
};
const struct sprite_frame_1box gSpriteBank17Frame011 = {
    SPRITE_FRAME(gSpriteBank17Frame011, SPRITE_TILES_BANK17_FRAME011),
    { { -10, -4, 29, 13 } },
};
const struct sprite_frame_1box gSpriteBank17Frame012 = {
    SPRITE_FRAME(gSpriteBank17Frame012, SPRITE_TILES_BANK17_FRAME012),
    { { -10, -4, 29, 13 } },
};
const struct sprite_frame_1box gSpriteBank17Frame013 = {
    SPRITE_FRAME(gSpriteBank17Frame013, SPRITE_TILES_BANK17_FRAME013),
    { { -10, -4, 29, 13 } },
};
const struct sprite_frame_1box gSpriteBank17Frame014 = {
    SPRITE_FRAME(gSpriteBank17Frame014, SPRITE_TILES_BANK17_FRAME014),
    { { -10, -4, 29, 13 } },
};

/* The piece arrays, from graphics/sprites/bank17_seal/bank.json. */
#include "sprites/bank17_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 18: 2 animations, 14 frames, graphics/sprites/bank18_wooden_crusher/. */

extern const u16 gSpriteBank18Anim00Seq[9];
extern const u16 gSpriteBank18Anim01Seq[5];

extern const struct sprite_anim gSpriteBank18Anims[2] = {
    /* 0 */ {
        /* seq */ gSpriteBank18Anim00Seq,
        /* box */ { { -15, -51, 31, 103 }, { -15, -67, 31, 119 } },
        /* paletteId */ 105,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank18Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank18Anim01Seq,
        /* box */ { { -15, -51, 31, 103 }, { -15, -67, 31, 119 } },
        /* paletteId */ 105,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank18Anim01Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank18Anim00Seq[9] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8,
};
const u16 gSpriteBank18Anim01Seq[5] = {
    9, 10, 11, 12, 13,
};

extern const struct sprite_frame *const gSpriteBank18Frames[14] = {
    &gSpriteBank18Frame000.frame,
    &gSpriteBank18Frame001.frame,
    &gSpriteBank18Frame002.frame,
    &gSpriteBank18Frame003.frame,
    &gSpriteBank18Frame004.frame,
    &gSpriteBank18Frame005.frame,
    &gSpriteBank18Frame006.frame,
    &gSpriteBank18Frame007.frame,
    &gSpriteBank18Frame008.frame,
    &gSpriteBank18Frame009.frame,
    &gSpriteBank18Frame010.frame,
    &gSpriteBank18Frame011.frame,
    &gSpriteBank18Frame012.frame,
    &gSpriteBank18Frame013.frame,
};

const struct sprite_frame_1box gSpriteBank18Frame000 = {
    SPRITE_FRAME(gSpriteBank18Frame000, SPRITE_TILES_BANK18_FRAME000),
    { { -11, -67, 23, 116 } },
};
const struct sprite_frame_1box gSpriteBank18Frame001 = {
    SPRITE_FRAME(gSpriteBank18Frame001, SPRITE_TILES_BANK18_FRAME001),
    { { -11, -68, 23, 125 } },
};
const struct sprite_frame_1box gSpriteBank18Frame002 = {
    SPRITE_FRAME(gSpriteBank18Frame002, SPRITE_TILES_BANK18_FRAME002),
    { { -11, -67, 23, 100 } },
};
const struct sprite_frame_1box gSpriteBank18Frame003 = {
    SPRITE_FRAME(gSpriteBank18Frame003, SPRITE_TILES_BANK18_FRAME003),
    { { -11, -66, 23, 81 } },
};
const struct sprite_frame_1box gSpriteBank18Frame004 = {
    SPRITE_FRAME(gSpriteBank18Frame004, SPRITE_TILES_BANK18_FRAME004),
    { { -11, -67, 23, 59 } },
};
const struct sprite_frame_1box gSpriteBank18Frame005 = {
    SPRITE_FRAME(gSpriteBank18Frame005, SPRITE_TILES_BANK18_FRAME005),
    { { -11, -66, 23, 50 } },
};
const struct sprite_frame_1box gSpriteBank18Frame006 = {
    SPRITE_FRAME(gSpriteBank18Frame006, SPRITE_TILES_BANK18_FRAME006),
    { { -11, -67, 23, 44 } },
};
const struct sprite_frame_1box gSpriteBank18Frame007 = {
    SPRITE_FRAME(gSpriteBank18Frame007, SPRITE_TILES_BANK18_FRAME007),
    { { -11, -68, 23, 39 } },
};
const struct sprite_frame_1box gSpriteBank18Frame008 = {
    SPRITE_FRAME(gSpriteBank18Frame008, SPRITE_TILES_BANK18_FRAME008),
    { { -11, -66, 23, 30 } },
};
const struct sprite_frame_1box gSpriteBank18Frame009 = {
    SPRITE_FRAME(gSpriteBank18Frame009, SPRITE_TILES_BANK18_FRAME009),
    { { -11, -67, 24, 32 } },
};
const struct sprite_frame_1box gSpriteBank18Frame010 = {
    SPRITE_FRAME(gSpriteBank18Frame010, SPRITE_TILES_BANK18_FRAME010),
    { { -10, -67, 22, 40 } },
};
const struct sprite_frame_1box gSpriteBank18Frame011 = {
    SPRITE_FRAME(gSpriteBank18Frame011, SPRITE_TILES_BANK18_FRAME011),
    { { -11, -67, 23, 48 } },
};
const struct sprite_frame_1box gSpriteBank18Frame012 = {
    SPRITE_FRAME(gSpriteBank18Frame012, SPRITE_TILES_BANK18_FRAME012),
    { { -11, -67, 23, 75 } },
};
const struct sprite_frame_1box gSpriteBank18Frame013 = {
    SPRITE_FRAME(gSpriteBank18Frame013, SPRITE_TILES_BANK18_FRAME013),
    { { -11, -68, 23, 113 } },
};

/* The piece arrays, from graphics/sprites/bank18_wooden_crusher/bank.json. */
#include "sprites/bank18_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 19: 2 animations, 35 frames, graphics/sprites/bank19_frog/. */

extern const u16 gSpriteBank19Anim00Seq[14];
extern const u16 gSpriteBank19Anim01Seq[21];

extern const struct sprite_anim gSpriteBank19Anims[2] = {
    /* 0 */ {
        /* seq */ gSpriteBank19Anim00Seq,
        /* box */ { { -17, -14, 34, 29 }, { -22, -43, 42, 58 } },
        /* paletteId */ 27,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank19Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank19Anim01Seq,
        /* box */ { { -17, -14, 34, 29 }, { -27, -44, 46, 59 } },
        /* paletteId */ 27,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank19Anim01Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank19Anim00Seq[14] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
};
const u16 gSpriteBank19Anim01Seq[21] = {
    14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34,
};

extern const struct sprite_frame *const gSpriteBank19Frames[35] = {
    &gSpriteBank19Frame000.frame,
    &gSpriteBank19Frame001.frame,
    &gSpriteBank19Frame002.frame,
    &gSpriteBank19Frame003.frame,
    &gSpriteBank19Frame004.frame,
    &gSpriteBank19Frame005.frame,
    &gSpriteBank19Frame006.frame,
    &gSpriteBank19Frame007.frame,
    &gSpriteBank19Frame008.frame,
    &gSpriteBank19Frame009.frame,
    &gSpriteBank19Frame010.frame,
    &gSpriteBank19Frame011.frame,
    &gSpriteBank19Frame012.frame,
    &gSpriteBank19Frame013.frame,
    &gSpriteBank19Frame014.frame,
    &gSpriteBank19Frame015.frame,
    &gSpriteBank19Frame016.frame,
    &gSpriteBank19Frame017.frame,
    &gSpriteBank19Frame018.frame,
    &gSpriteBank19Frame019.frame,
    &gSpriteBank19Frame020.frame,
    &gSpriteBank19Frame021.frame,
    &gSpriteBank19Frame022.frame,
    &gSpriteBank19Frame023.frame,
    &gSpriteBank19Frame024.frame,
    &gSpriteBank19Frame025.frame,
    &gSpriteBank19Frame026.frame,
    &gSpriteBank19Frame027.frame,
    &gSpriteBank19Frame028.frame,
    &gSpriteBank19Frame029.frame,
    &gSpriteBank19Frame030.frame,
    &gSpriteBank19Frame031.frame,
    &gSpriteBank19Frame032.frame,
    &gSpriteBank19Frame033.frame,
    &gSpriteBank19Frame034.frame,
};

const struct sprite_frame_1box gSpriteBank19Frame000 = {
    SPRITE_FRAME(gSpriteBank19Frame000, SPRITE_TILES_BANK19_FRAME000),
    { { -15, -10, 30, 11 } },
};
const struct sprite_frame_1box gSpriteBank19Frame001 = {
    SPRITE_FRAME(gSpriteBank19Frame001, SPRITE_TILES_BANK19_FRAME001),
    { { -15, -9, 30, 10 } },
};
const struct sprite_frame_1box gSpriteBank19Frame002 = {
    SPRITE_FRAME(gSpriteBank19Frame002, SPRITE_TILES_BANK19_FRAME002),
    { { -14, -8, 28, 9 } },
};
const struct sprite_frame_1box gSpriteBank19Frame003 = {
    SPRITE_FRAME(gSpriteBank19Frame003, SPRITE_TILES_BANK19_FRAME003),
    { { -14, -8, 26, 9 } },
};
const struct sprite_frame_1box gSpriteBank19Frame004 = {
    SPRITE_FRAME(gSpriteBank19Frame004, SPRITE_TILES_BANK19_FRAME004),
    { { -14, -8, 25, 9 } },
};
const struct sprite_frame_1box gSpriteBank19Frame005 = {
    SPRITE_FRAME(gSpriteBank19Frame005, SPRITE_TILES_BANK19_FRAME005),
    { { -14, -8, 26, 8 } },
};
const struct sprite_frame_1box gSpriteBank19Frame006 = {
    SPRITE_FRAME(gSpriteBank19Frame006, SPRITE_TILES_BANK19_FRAME006),
    { { -14, -8, 28, 8 } },
};
const struct sprite_frame_1box gSpriteBank19Frame007 = {
    SPRITE_FRAME(gSpriteBank19Frame007, SPRITE_TILES_BANK19_FRAME007),
    { { -14, -8, 29, 8 } },
};
const struct sprite_frame_1box gSpriteBank19Frame008 = {
    SPRITE_FRAME(gSpriteBank19Frame008, SPRITE_TILES_BANK19_FRAME008),
    { { -16, -12, 30, 6 } },
};
const struct sprite_frame_1box gSpriteBank19Frame009 = {
    SPRITE_FRAME(gSpriteBank19Frame009, SPRITE_TILES_BANK19_FRAME009),
    { { -20, -34, 31, 21 } },
};
const struct sprite_frame_1box gSpriteBank19Frame010 = {
    SPRITE_FRAME(gSpriteBank19Frame010, SPRITE_TILES_BANK19_FRAME010),
    { { -16, -39, 29, 20 } },
};
const struct sprite_frame_1box gSpriteBank19Frame011 = {
    SPRITE_FRAME(gSpriteBank19Frame011, SPRITE_TILES_BANK19_FRAME011),
    { { -15, -23, 28, 1 } },
};
const struct sprite_frame_1box gSpriteBank19Frame012 = {
    SPRITE_FRAME(gSpriteBank19Frame012, SPRITE_TILES_BANK19_FRAME012),
    { { -17, -16, 35, 6 } },
};
const struct sprite_frame_1box gSpriteBank19Frame013 = {
    SPRITE_FRAME(gSpriteBank19Frame013, SPRITE_TILES_BANK19_FRAME013),
    { { -18, -11, 27, 12 } },
};
const struct sprite_frame_1box gSpriteBank19Frame014 = {
    SPRITE_FRAME(gSpriteBank19Frame014, SPRITE_TILES_BANK19_FRAME014),
    { { -14, -11, 28, 13 } },
};
const struct sprite_frame_1box gSpriteBank19Frame015 = {
    SPRITE_FRAME(gSpriteBank19Frame015, SPRITE_TILES_BANK19_FRAME015),
    { { -14, -10, 28, 12 } },
};
const struct sprite_frame_1box gSpriteBank19Frame016 = {
    SPRITE_FRAME(gSpriteBank19Frame016, SPRITE_TILES_BANK19_FRAME016),
    { { -13, -9, 26, 11 } },
};
const struct sprite_frame_1box gSpriteBank19Frame017 = {
    SPRITE_FRAME(gSpriteBank19Frame017, SPRITE_TILES_BANK19_FRAME017),
    { { -13, -9, 24, 11 } },
};
const struct sprite_frame_1box gSpriteBank19Frame018 = {
    SPRITE_FRAME(gSpriteBank19Frame018, SPRITE_TILES_BANK19_FRAME018),
    { { -13, -9, 23, 11 } },
};
const struct sprite_frame_1box gSpriteBank19Frame019 = {
    SPRITE_FRAME(gSpriteBank19Frame019, SPRITE_TILES_BANK19_FRAME019),
    { { -13, -9, 24, 10 } },
};
const struct sprite_frame_1box gSpriteBank19Frame020 = {
    SPRITE_FRAME(gSpriteBank19Frame020, SPRITE_TILES_BANK19_FRAME020),
    { { -13, -9, 26, 10 } },
};
const struct sprite_frame_1box gSpriteBank19Frame021 = {
    SPRITE_FRAME(gSpriteBank19Frame021, SPRITE_TILES_BANK19_FRAME021),
    { { -13, -9, 28, 11 } },
};
const struct sprite_frame_1box gSpriteBank19Frame022 = {
    SPRITE_FRAME(gSpriteBank19Frame022, SPRITE_TILES_BANK19_FRAME022),
    { { -15, -22, 28, 17 } },
};
const struct sprite_frame_1box gSpriteBank19Frame023 = {
    SPRITE_FRAME(gSpriteBank19Frame023, SPRITE_TILES_BANK19_FRAME023),
    { { -18, -41, 29, 33 } },
};
const struct sprite_frame_1box gSpriteBank19Frame024 = {
    SPRITE_FRAME(gSpriteBank19Frame024, SPRITE_TILES_BANK19_FRAME024),
    { { -24, -31, 39, 14 } },
};
const struct sprite_frame_1box gSpriteBank19Frame025 = {
    SPRITE_FRAME(gSpriteBank19Frame025, SPRITE_TILES_BANK19_FRAME025),
    { { -13, -18, 29, 9 } },
};
const struct sprite_frame_1box gSpriteBank19Frame026 = {
    SPRITE_FRAME(gSpriteBank19Frame026, SPRITE_TILES_BANK19_FRAME026),
    { { -16, -14, 29, 11 } },
};
const struct sprite_frame_1box gSpriteBank19Frame027 = {
    SPRITE_FRAME(gSpriteBank19Frame027, SPRITE_TILES_BANK19_FRAME027),
    { { -13, -8, 28, 10 } },
};
const struct sprite_frame_1box gSpriteBank19Frame028 = {
    SPRITE_FRAME(gSpriteBank19Frame028, SPRITE_TILES_BANK19_FRAME028),
    { { -14, -8, 30, 10 } },
};
const struct sprite_frame_1box gSpriteBank19Frame029 = {
    SPRITE_FRAME(gSpriteBank19Frame029, SPRITE_TILES_BANK19_FRAME029),
    { { -15, -8, 30, 10 } },
};
const struct sprite_frame_1box gSpriteBank19Frame030 = {
    SPRITE_FRAME(gSpriteBank19Frame030, SPRITE_TILES_BANK19_FRAME030),
    { { -14, -9, 28, 11 } },
};
const struct sprite_frame_1box gSpriteBank19Frame031 = {
    SPRITE_FRAME(gSpriteBank19Frame031, SPRITE_TILES_BANK19_FRAME031),
    { { -13, -9, 27, 11 } },
};
const struct sprite_frame_1box gSpriteBank19Frame032 = {
    SPRITE_FRAME(gSpriteBank19Frame032, SPRITE_TILES_BANK19_FRAME032),
    { { -13, -9, 27, 11 } },
};
const struct sprite_frame_1box gSpriteBank19Frame033 = {
    SPRITE_FRAME(gSpriteBank19Frame033, SPRITE_TILES_BANK19_FRAME033),
    { { -13, -10, 27, 12 } },
};
const struct sprite_frame_1box gSpriteBank19Frame034 = {
    SPRITE_FRAME(gSpriteBank19Frame034, SPRITE_TILES_BANK19_FRAME034),
    { { -14, -11, 28, 13 } },
};

/* The piece arrays, from graphics/sprites/bank19_frog/bank.json. */
#include "sprites/bank19_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 20: 2 animations, 32 frames, graphics/sprites/bank20_patrolling_sewer_enemy/. */

extern const u16 gSpriteBank20Anim00Seq[16];
extern const u16 gSpriteBank20Anim01Seq[16];

extern const struct sprite_anim gSpriteBank20Anims[2] = {
    /* 0 */ {
        /* seq */ gSpriteBank20Anim00Seq,
        /* box */ { { -14, -9, 28, 19 }, { -15, -9, 30, 19 } },
        /* paletteId */ 28,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank20Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank20Anim01Seq,
        /* box */ { { -14, -9, 28, 19 }, { -15, -9, 29, 19 } },
        /* paletteId */ 28,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank20Anim01Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank20Anim00Seq[16] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
};
const u16 gSpriteBank20Anim01Seq[16] = {
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
};

extern const struct sprite_frame *const gSpriteBank20Frames[32] = {
    &gSpriteBank20Frame000.frame,
    &gSpriteBank20Frame001.frame,
    &gSpriteBank20Frame002.frame,
    &gSpriteBank20Frame003.frame,
    &gSpriteBank20Frame004.frame,
    &gSpriteBank20Frame005.frame,
    &gSpriteBank20Frame006.frame,
    &gSpriteBank20Frame007.frame,
    &gSpriteBank20Frame008.frame,
    &gSpriteBank20Frame009.frame,
    &gSpriteBank20Frame010.frame,
    &gSpriteBank20Frame011.frame,
    &gSpriteBank20Frame012.frame,
    &gSpriteBank20Frame013.frame,
    &gSpriteBank20Frame014.frame,
    &gSpriteBank20Frame015.frame,
    &gSpriteBank20Frame016.frame,
    &gSpriteBank20Frame017.frame,
    &gSpriteBank20Frame018.frame,
    &gSpriteBank20Frame019.frame,
    &gSpriteBank20Frame020.frame,
    &gSpriteBank20Frame021.frame,
    &gSpriteBank20Frame022.frame,
    &gSpriteBank20Frame023.frame,
    &gSpriteBank20Frame024.frame,
    &gSpriteBank20Frame025.frame,
    &gSpriteBank20Frame026.frame,
    &gSpriteBank20Frame027.frame,
    &gSpriteBank20Frame028.frame,
    &gSpriteBank20Frame029.frame,
    &gSpriteBank20Frame030.frame,
    &gSpriteBank20Frame031.frame,
};

const struct sprite_frame_1box gSpriteBank20Frame000 = {
    SPRITE_FRAME(gSpriteBank20Frame000, SPRITE_TILES_BANK20_FRAME000),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame001 = {
    SPRITE_FRAME(gSpriteBank20Frame001, SPRITE_TILES_BANK20_FRAME001),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame002 = {
    SPRITE_FRAME(gSpriteBank20Frame002, SPRITE_TILES_BANK20_FRAME002),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame003 = {
    SPRITE_FRAME(gSpriteBank20Frame003, SPRITE_TILES_BANK20_FRAME003),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame004 = {
    SPRITE_FRAME(gSpriteBank20Frame004, SPRITE_TILES_BANK20_FRAME004),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame005 = {
    SPRITE_FRAME(gSpriteBank20Frame005, SPRITE_TILES_BANK20_FRAME005),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame006 = {
    SPRITE_FRAME(gSpriteBank20Frame006, SPRITE_TILES_BANK20_FRAME006),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame007 = {
    SPRITE_FRAME(gSpriteBank20Frame007, SPRITE_TILES_BANK20_FRAME007),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame008 = {
    SPRITE_FRAME(gSpriteBank20Frame008, SPRITE_TILES_BANK20_FRAME008),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame009 = {
    SPRITE_FRAME(gSpriteBank20Frame009, SPRITE_TILES_BANK20_FRAME009),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame010 = {
    SPRITE_FRAME(gSpriteBank20Frame010, SPRITE_TILES_BANK20_FRAME010),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame011 = {
    SPRITE_FRAME(gSpriteBank20Frame011, SPRITE_TILES_BANK20_FRAME011),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame012 = {
    SPRITE_FRAME(gSpriteBank20Frame012, SPRITE_TILES_BANK20_FRAME012),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame013 = {
    SPRITE_FRAME(gSpriteBank20Frame013, SPRITE_TILES_BANK20_FRAME013),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame014 = {
    SPRITE_FRAME(gSpriteBank20Frame014, SPRITE_TILES_BANK20_FRAME014),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame015 = {
    SPRITE_FRAME(gSpriteBank20Frame015, SPRITE_TILES_BANK20_FRAME015),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame016 = {
    SPRITE_FRAME(gSpriteBank20Frame016, SPRITE_TILES_BANK20_FRAME016),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame017 = {
    SPRITE_FRAME(gSpriteBank20Frame017, SPRITE_TILES_BANK20_FRAME017),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame018 = {
    SPRITE_FRAME(gSpriteBank20Frame018, SPRITE_TILES_BANK20_FRAME018),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame019 = {
    SPRITE_FRAME(gSpriteBank20Frame019, SPRITE_TILES_BANK20_FRAME019),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame020 = {
    SPRITE_FRAME(gSpriteBank20Frame020, SPRITE_TILES_BANK20_FRAME020),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame021 = {
    SPRITE_FRAME(gSpriteBank20Frame021, SPRITE_TILES_BANK20_FRAME021),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame022 = {
    SPRITE_FRAME(gSpriteBank20Frame022, SPRITE_TILES_BANK20_FRAME022),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame023 = {
    SPRITE_FRAME(gSpriteBank20Frame023, SPRITE_TILES_BANK20_FRAME023),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame024 = {
    SPRITE_FRAME(gSpriteBank20Frame024, SPRITE_TILES_BANK20_FRAME024),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame025 = {
    SPRITE_FRAME(gSpriteBank20Frame025, SPRITE_TILES_BANK20_FRAME025),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame026 = {
    SPRITE_FRAME(gSpriteBank20Frame026, SPRITE_TILES_BANK20_FRAME026),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame027 = {
    SPRITE_FRAME(gSpriteBank20Frame027, SPRITE_TILES_BANK20_FRAME027),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame028 = {
    SPRITE_FRAME(gSpriteBank20Frame028, SPRITE_TILES_BANK20_FRAME028),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame029 = {
    SPRITE_FRAME(gSpriteBank20Frame029, SPRITE_TILES_BANK20_FRAME029),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame030 = {
    SPRITE_FRAME(gSpriteBank20Frame030, SPRITE_TILES_BANK20_FRAME030),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame031 = {
    SPRITE_FRAME(gSpriteBank20Frame031, SPRITE_TILES_BANK20_FRAME031),
    { { -12, -8, 24, 17 } },
};

/* The piece arrays, from graphics/sprites/bank20_patrolling_sewer_enemy/bank.json. */
#include "sprites/bank20_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 21: 2 animations, 15 frames, graphics/sprites/bank21_rat/. */

extern const u16 gSpriteBank21Anim00Seq[8];
extern const u16 gSpriteBank21Anim01Seq[7];

extern const struct sprite_anim gSpriteBank21Anims[2] = {
    /* 0 */ {
        /* seq */ gSpriteBank21Anim00Seq,
        /* box */ { { -21, -10, 43, 21 }, { -22, -11, 46, 22 } },
        /* paletteId */ 29,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank21Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank21Anim01Seq,
        /* box */ { { -21, -10, 43, 21 }, { -20, -11, 42, 21 } },
        /* paletteId */ 29,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank21Anim01Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank21Anim00Seq[8] = {
    0, 1, 2, 3, 4, 5, 6, 7,
};
const u16 gSpriteBank21Anim01Seq[7] = {
    8, 9, 10, 11, 12, 13, 14,
};

extern const struct sprite_frame *const gSpriteBank21Frames[15] = {
    &gSpriteBank21Frame000.frame,
    &gSpriteBank21Frame001.frame,
    &gSpriteBank21Frame002.frame,
    &gSpriteBank21Frame003.frame,
    &gSpriteBank21Frame004.frame,
    &gSpriteBank21Frame005.frame,
    &gSpriteBank21Frame006.frame,
    &gSpriteBank21Frame007.frame,
    &gSpriteBank21Frame008.frame,
    &gSpriteBank21Frame009.frame,
    &gSpriteBank21Frame010.frame,
    &gSpriteBank21Frame011.frame,
    &gSpriteBank21Frame012.frame,
    &gSpriteBank21Frame013.frame,
    &gSpriteBank21Frame014.frame,
};

const struct sprite_frame_1box gSpriteBank21Frame000 = {
    SPRITE_FRAME(gSpriteBank21Frame000, SPRITE_TILES_BANK21_FRAME000),
    { { -6, -7, 25, 15 } },
};
const struct sprite_frame_1box gSpriteBank21Frame001 = {
    SPRITE_FRAME(gSpriteBank21Frame001, SPRITE_TILES_BANK21_FRAME001),
    { { -7, -7, 27, 14 } },
};
const struct sprite_frame_1box gSpriteBank21Frame002 = {
    SPRITE_FRAME(gSpriteBank21Frame002, SPRITE_TILES_BANK21_FRAME002),
    { { -3, -6, 24, 12 } },
};
const struct sprite_frame_1box gSpriteBank21Frame003 = {
    SPRITE_FRAME(gSpriteBank21Frame003, SPRITE_TILES_BANK21_FRAME003),
    { { -4, -4, 24, 13 } },
};
const struct sprite_frame_1box gSpriteBank21Frame004 = {
    SPRITE_FRAME(gSpriteBank21Frame004, SPRITE_TILES_BANK21_FRAME004),
    { { -6, -4, 25, 13 } },
};
const struct sprite_frame_1box gSpriteBank21Frame005 = {
    SPRITE_FRAME(gSpriteBank21Frame005, SPRITE_TILES_BANK21_FRAME005),
    { { -7, -3, 26, 12 } },
};
const struct sprite_frame_1box gSpriteBank21Frame006 = {
    SPRITE_FRAME(gSpriteBank21Frame006, SPRITE_TILES_BANK21_FRAME006),
    { { -6, -1, 24, 10 } },
};
const struct sprite_frame_1box gSpriteBank21Frame007 = {
    SPRITE_FRAME(gSpriteBank21Frame007, SPRITE_TILES_BANK21_FRAME007),
    { { -5, -2, 21, 10 } },
};
const struct sprite_frame_1box gSpriteBank21Frame008 = {
    SPRITE_FRAME(gSpriteBank21Frame008, SPRITE_TILES_BANK21_FRAME008),
    { { -4, -2, 22, 11 } },
};
const struct sprite_frame_1box gSpriteBank21Frame009 = {
    SPRITE_FRAME(gSpriteBank21Frame009, SPRITE_TILES_BANK21_FRAME009),
    { { -4, -3, 19, 12 } },
};
const struct sprite_frame_1box gSpriteBank21Frame010 = {
    SPRITE_FRAME(gSpriteBank21Frame010, SPRITE_TILES_BANK21_FRAME010),
    { { -3, -3, 17, 11 } },
};
const struct sprite_frame_1box gSpriteBank21Frame011 = {
    SPRITE_FRAME(gSpriteBank21Frame011, SPRITE_TILES_BANK21_FRAME011),
    { { -4, -5, 18, 14 } },
};
const struct sprite_frame_1box gSpriteBank21Frame012 = {
    SPRITE_FRAME(gSpriteBank21Frame012, SPRITE_TILES_BANK21_FRAME012),
    { { -7, -3, 18, 13 } },
};
const struct sprite_frame_1box gSpriteBank21Frame013 = {
    SPRITE_FRAME(gSpriteBank21Frame013, SPRITE_TILES_BANK21_FRAME013),
    { { -12, -2, 19, 11 } },
};
const struct sprite_frame_1box gSpriteBank21Frame014 = {
    SPRITE_FRAME(gSpriteBank21Frame014, SPRITE_TILES_BANK21_FRAME014),
    { { -15, -3, 24, 12 } },
};

/* The piece arrays, from graphics/sprites/bank21_rat/bank.json. */
#include "sprites/bank21_pieces.inc"
