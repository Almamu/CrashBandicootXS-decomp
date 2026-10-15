extern "C" {
#include "gba/types.h"
#include "engine/gfx/sprite_bank.h"
}
#include "sprites/sprite_frames.h"

/*
 * ROM 0x0847e318-0x08485d00: sprite banks 7-25 of the sprite-bank
 * animation system (gSpriteBankTable, include/engine/gfx/sprite_bank.h). Linked in
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

/* ---------------------------------------------------------------------- */
/* Bank 7: 5 animations, 55 frames, games/n2/graphics/sprites/bank07_fire_juggler/. */

extern const u16 gSpriteBank07Anim00Seq[18];
extern const u16 gSpriteBank07Anim01Seq[9];
extern const u16 gSpriteBank07Anim02Seq[9];
extern const u16 gSpriteBank07Anim03Seq[9];
extern const u16 gSpriteBank07Anim04Seq[17];

extern const struct sprite_anim gSpriteBank07Anims[5] = {
    /* 0 */ {
        /* seq */ gSpriteBank07Anim00Seq,
        /* box */ { { -13, -11, 27, 22 }, { -14, -22, 31, 42 } },
        /* paletteId */ 14,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank07Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank07Anim01Seq,
        /* box */ { { -12, -10, 25, 21 }, { -13, -22, 30, 42 } },
        /* paletteId */ 14,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank07Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank07Anim02Seq,
        /* box */ { { -12, -10, 25, 21 }, { -14, -21, 28, 36 } },
        /* paletteId */ 14,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank07Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank07Anim03Seq,
        /* box */ { { -5, 6, 12, 17 }, { -5, 0, 11, 22 } },
        /* paletteId */ 15,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank07Anim03Seq),
        /* flags */ 0,
    },
    /* 4 */ {
        /* seq */ gSpriteBank07Anim04Seq,
        /* box */ { { -11, -55, 26, 81 }, { -65, -102, 79, 127 } },
        /* paletteId */ 12,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank07Anim04Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank07Anim00Seq[18] = {
    0, 1, 2, 2, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9, 10, 10,
    10, 11,
};
const u16 gSpriteBank07Anim01Seq[9] = {
    12, 13, 14, 15, 16, 17, 18, 19, 20,
};
const u16 gSpriteBank07Anim02Seq[9] = {
    21, 22, 23, 24, 25, 26, 27, 28, 29,
};
const u16 gSpriteBank07Anim03Seq[9] = {
    30, 31, 32, 33, 34, 35, 36, 37, 38,
};
const u16 gSpriteBank07Anim04Seq[17] = {
    39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54,
    39,
};

extern const struct sprite_frame *const gSpriteBank07Frames[55] = {
    &gSpriteBank07Frame000.frame,
    &gSpriteBank07Frame001.frame,
    &gSpriteBank07Frame002.frame,
    &gSpriteBank07Frame003.frame,
    &gSpriteBank07Frame004.frame,
    &gSpriteBank07Frame005.frame,
    &gSpriteBank07Frame006.frame,
    &gSpriteBank07Frame007.frame,
    &gSpriteBank07Frame008.frame,
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
    &gSpriteBank07Frame030,
    &gSpriteBank07Frame031,
    &gSpriteBank07Frame032.frame,
    &gSpriteBank07Frame033.frame,
    &gSpriteBank07Frame034.frame,
    &gSpriteBank07Frame035.frame,
    &gSpriteBank07Frame036.frame,
    &gSpriteBank07Frame037.frame,
    &gSpriteBank07Frame038.frame,
    &gSpriteBank07Frame039,
    &gSpriteBank07Frame040.frame,
    &gSpriteBank07Frame041.frame,
    &gSpriteBank07Frame042.frame,
    &gSpriteBank07Frame043.frame,
    &gSpriteBank07Frame044.frame,
    &gSpriteBank07Frame045.frame,
    &gSpriteBank07Frame046.frame,
    &gSpriteBank07Frame047.frame,
    &gSpriteBank07Frame048.frame,
    &gSpriteBank07Frame049.frame,
    &gSpriteBank07Frame050.frame,
    &gSpriteBank07Frame051.frame,
    &gSpriteBank07Frame052.frame,
    &gSpriteBank07Frame053.frame,
    &gSpriteBank07Frame054.frame,
};

const struct sprite_frame_1box gSpriteBank07Frame000 = {
    SPRITE_FRAME(gSpriteBank07Frame000, SPRITE_TILES_BANK07_FRAME000),
    { { -11, -12, 24, 22 } },
};
const struct sprite_frame_1box gSpriteBank07Frame001 = {
    SPRITE_FRAME(gSpriteBank07Frame001, SPRITE_TILES_BANK07_FRAME001),
    { { -11, -12, 24, 22 } },
};
const struct sprite_frame_1box gSpriteBank07Frame002 = {
    SPRITE_FRAME(gSpriteBank07Frame002, SPRITE_TILES_BANK07_FRAME002),
    { { -11, -12, 24, 22 } },
};
const struct sprite_frame_1box gSpriteBank07Frame003 = {
    SPRITE_FRAME(gSpriteBank07Frame003, SPRITE_TILES_BANK07_FRAME003),
    { { -11, -12, 24, 22 } },
};
const struct sprite_frame_1box gSpriteBank07Frame004 = {
    SPRITE_FRAME(gSpriteBank07Frame004, SPRITE_TILES_BANK07_FRAME004),
    { { -11, -12, 24, 22 } },
};
const struct sprite_frame_1box gSpriteBank07Frame005 = {
    SPRITE_FRAME(gSpriteBank07Frame005, SPRITE_TILES_BANK07_FRAME005),
    { { -11, -12, 24, 22 } },
};
const struct sprite_frame_1box gSpriteBank07Frame006 = {
    SPRITE_FRAME(gSpriteBank07Frame006, SPRITE_TILES_BANK07_FRAME006),
    { { -11, -12, 24, 22 } },
};
const struct sprite_frame_1box gSpriteBank07Frame007 = {
    SPRITE_FRAME(gSpriteBank07Frame007, SPRITE_TILES_BANK07_FRAME007),
    { { -11, -12, 24, 22 } },
};
const struct sprite_frame_1box gSpriteBank07Frame008 = {
    SPRITE_FRAME(gSpriteBank07Frame008, SPRITE_TILES_BANK07_FRAME008),
    { { -11, -12, 24, 22 } },
};
const struct sprite_frame_1box gSpriteBank07Frame009 = {
    SPRITE_FRAME(gSpriteBank07Frame009, SPRITE_TILES_BANK07_FRAME009),
    { { -11, -12, 24, 22 } },
};
const struct sprite_frame_1box gSpriteBank07Frame010 = {
    SPRITE_FRAME(gSpriteBank07Frame010, SPRITE_TILES_BANK07_FRAME010),
    { { -11, -12, 24, 22 } },
};
const struct sprite_frame_1box gSpriteBank07Frame011 = {
    SPRITE_FRAME(gSpriteBank07Frame011, SPRITE_TILES_BANK07_FRAME011),
    { { -11, -12, 24, 22 } },
};
const struct sprite_frame_1box gSpriteBank07Frame012 = {
    SPRITE_FRAME(gSpriteBank07Frame012, SPRITE_TILES_BANK07_FRAME012),
    { { -8, -9, 21, 20 } },
};
const struct sprite_frame_1box gSpriteBank07Frame013 = {
    SPRITE_FRAME(gSpriteBank07Frame013, SPRITE_TILES_BANK07_FRAME013),
    { { -8, -9, 21, 20 } },
};
const struct sprite_frame_1box gSpriteBank07Frame014 = {
    SPRITE_FRAME(gSpriteBank07Frame014, SPRITE_TILES_BANK07_FRAME014),
    { { -8, -9, 21, 20 } },
};
const struct sprite_frame_1box gSpriteBank07Frame015 = {
    SPRITE_FRAME(gSpriteBank07Frame015, SPRITE_TILES_BANK07_FRAME015),
    { { -8, -9, 21, 20 } },
};
const struct sprite_frame_1box gSpriteBank07Frame016 = {
    SPRITE_FRAME(gSpriteBank07Frame016, SPRITE_TILES_BANK07_FRAME016),
    { { -8, -9, 21, 20 } },
};
const struct sprite_frame_1box gSpriteBank07Frame017 = {
    SPRITE_FRAME(gSpriteBank07Frame017, SPRITE_TILES_BANK07_FRAME017),
    { { -8, -9, 21, 20 } },
};
const struct sprite_frame_1box gSpriteBank07Frame018 = {
    SPRITE_FRAME(gSpriteBank07Frame018, SPRITE_TILES_BANK07_FRAME018),
    { { -8, -9, 21, 20 } },
};
const struct sprite_frame_1box gSpriteBank07Frame019 = {
    SPRITE_FRAME(gSpriteBank07Frame019, SPRITE_TILES_BANK07_FRAME019),
    { { -8, -9, 21, 20 } },
};
const struct sprite_frame_1box gSpriteBank07Frame020 = {
    SPRITE_FRAME(gSpriteBank07Frame020, SPRITE_TILES_BANK07_FRAME020),
    { { -8, -9, 21, 20 } },
};
const struct sprite_frame_1box gSpriteBank07Frame021 = {
    SPRITE_FRAME(gSpriteBank07Frame021, SPRITE_TILES_BANK07_FRAME021),
    { { -9, -11, 21, 22 } },
};
const struct sprite_frame_1box gSpriteBank07Frame022 = {
    SPRITE_FRAME(gSpriteBank07Frame022, SPRITE_TILES_BANK07_FRAME022),
    { { -9, -11, 21, 22 } },
};
const struct sprite_frame_1box gSpriteBank07Frame023 = {
    SPRITE_FRAME(gSpriteBank07Frame023, SPRITE_TILES_BANK07_FRAME023),
    { { -9, -11, 21, 22 } },
};
const struct sprite_frame_1box gSpriteBank07Frame024 = {
    SPRITE_FRAME(gSpriteBank07Frame024, SPRITE_TILES_BANK07_FRAME024),
    { { -9, -11, 21, 22 } },
};
const struct sprite_frame_1box gSpriteBank07Frame025 = {
    SPRITE_FRAME(gSpriteBank07Frame025, SPRITE_TILES_BANK07_FRAME025),
    { { -9, -11, 21, 22 } },
};
const struct sprite_frame_1box gSpriteBank07Frame026 = {
    SPRITE_FRAME(gSpriteBank07Frame026, SPRITE_TILES_BANK07_FRAME026),
    { { -9, -11, 21, 22 } },
};
const struct sprite_frame_1box gSpriteBank07Frame027 = {
    SPRITE_FRAME(gSpriteBank07Frame027, SPRITE_TILES_BANK07_FRAME027),
    { { -9, -11, 21, 22 } },
};
const struct sprite_frame_1box gSpriteBank07Frame028 = {
    SPRITE_FRAME(gSpriteBank07Frame028, SPRITE_TILES_BANK07_FRAME028),
    { { -9, -11, 21, 22 } },
};
const struct sprite_frame_1box gSpriteBank07Frame029 = {
    SPRITE_FRAME(gSpriteBank07Frame029, SPRITE_TILES_BANK07_FRAME029),
    { { -9, -11, 21, 22 } },
};
const struct sprite_frame gSpriteBank07Frame030 = SPRITE_FRAME(gSpriteBank07Frame030, SPRITE_TILES_BANK07_FRAME030);
const struct sprite_frame gSpriteBank07Frame031 = SPRITE_FRAME(gSpriteBank07Frame031, SPRITE_TILES_BANK07_FRAME031);
const struct sprite_frame_1box gSpriteBank07Frame032 = {
    SPRITE_FRAME(gSpriteBank07Frame032, SPRITE_TILES_BANK07_FRAME032),
    { { -1, 1, 3, 4 } },
};
const struct sprite_frame_1box gSpriteBank07Frame033 = {
    SPRITE_FRAME(gSpriteBank07Frame033, SPRITE_TILES_BANK07_FRAME033),
    { { -2, 3, 5, 4 } },
};
const struct sprite_frame_1box gSpriteBank07Frame034 = {
    SPRITE_FRAME(gSpriteBank07Frame034, SPRITE_TILES_BANK07_FRAME034),
    { { -2, 4, 5, 5 } },
};
const struct sprite_frame_1box gSpriteBank07Frame035 = {
    SPRITE_FRAME(gSpriteBank07Frame035, SPRITE_TILES_BANK07_FRAME035),
    { { -1, 4, 5, 6 } },
};
const struct sprite_frame_1box gSpriteBank07Frame036 = {
    SPRITE_FRAME(gSpriteBank07Frame036, SPRITE_TILES_BANK07_FRAME036),
    { { -3, 3, 8, 11 } },
};
const struct sprite_frame_1box gSpriteBank07Frame037 = {
    SPRITE_FRAME(gSpriteBank07Frame037, SPRITE_TILES_BANK07_FRAME037),
    { { -4, 8, 9, 10 } },
};
const struct sprite_frame_1box gSpriteBank07Frame038 = {
    SPRITE_FRAME(gSpriteBank07Frame038, SPRITE_TILES_BANK07_FRAME038),
    { { -5, 7, 11, 15 } },
};
const struct sprite_frame gSpriteBank07Frame039 = { gSpriteBank07Frame040Pos, gSpriteBank07Frame040Pieces, SPRITE_FRAME_TILES(SPRITE_TILES_BANK07_FRAME039, 0) };
const struct sprite_frame_1box gSpriteBank07Frame040 = {
    SPRITE_FRAME(gSpriteBank07Frame040, SPRITE_TILES_BANK07_FRAME040),
    { { -6, -18, 12, 29 } },
};
const struct sprite_frame_1box gSpriteBank07Frame041 = {
    SPRITE_FRAME(gSpriteBank07Frame041, SPRITE_TILES_BANK07_FRAME041),
    { { -7, -27, 12, 38 } },
};
const struct sprite_frame_1box gSpriteBank07Frame042 = {
    SPRITE_FRAME(gSpriteBank07Frame042, SPRITE_TILES_BANK07_FRAME042),
    { { -11, -29, 22, 48 } },
};
const struct sprite_frame_1box gSpriteBank07Frame043 = {
    SPRITE_FRAME(gSpriteBank07Frame043, SPRITE_TILES_BANK07_FRAME043),
    { { -7, -24, 17, 42 } },
};
const struct sprite_frame_1box gSpriteBank07Frame044 = {
    SPRITE_FRAME(gSpriteBank07Frame044, SPRITE_TILES_BANK07_FRAME044),
    { { -11, -35, 21, 53 } },
};
const struct sprite_frame_1box gSpriteBank07Frame045 = {
    SPRITE_FRAME(gSpriteBank07Frame045, SPRITE_TILES_BANK07_FRAME045),
    { { -11, -33, 21, 50 } },
};
const struct sprite_frame_1box gSpriteBank07Frame046 = {
    SPRITE_FRAME(gSpriteBank07Frame046, SPRITE_TILES_BANK07_FRAME046),
    { { -9, -34, 20, 53 } },
};
const struct sprite_frame_1box gSpriteBank07Frame047 = {
    SPRITE_FRAME(gSpriteBank07Frame047, SPRITE_TILES_BANK07_FRAME047),
    { { -9, -28, 19, 45 } },
};
const struct sprite_frame_1box gSpriteBank07Frame048 = {
    SPRITE_FRAME(gSpriteBank07Frame048, SPRITE_TILES_BANK07_FRAME048),
    { { -9, -32, 20, 52 } },
};
const struct sprite_frame_1box gSpriteBank07Frame049 = {
    SPRITE_FRAME(gSpriteBank07Frame049, SPRITE_TILES_BANK07_FRAME049),
    { { -8, -29, 17, 51 } },
};
const struct sprite_frame_1box gSpriteBank07Frame050 = {
    SPRITE_FRAME(gSpriteBank07Frame050, SPRITE_TILES_BANK07_FRAME050),
    { { -9, -30, 18, 51 } },
};
const struct sprite_frame_1box gSpriteBank07Frame051 = {
    SPRITE_FRAME(gSpriteBank07Frame051, SPRITE_TILES_BANK07_FRAME051),
    { { -9, -24, 19, 42 } },
};
const struct sprite_frame_1box gSpriteBank07Frame052 = {
    SPRITE_FRAME(gSpriteBank07Frame052, SPRITE_TILES_BANK07_FRAME052),
    { { -7, -20, 16, 39 } },
};
const struct sprite_frame_1box gSpriteBank07Frame053 = {
    SPRITE_FRAME(gSpriteBank07Frame053, SPRITE_TILES_BANK07_FRAME053),
    { { -7, -22, 12, 31 } },
};
const struct sprite_frame_1box gSpriteBank07Frame054 = {
    SPRITE_FRAME(gSpriteBank07Frame054, SPRITE_TILES_BANK07_FRAME054),
    { { -4, -11, 8, 20 } },
};

/* The piece arrays, from games/n2/graphics/sprites/bank07_fire_juggler/bank.json. */
#include "sprites/bank07_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 8: 3 animations, 30 frames, games/n2/graphics/sprites/bank08_carpet_rider/. */

extern const u16 gSpriteBank08Anim00Seq[14];
extern const u16 gSpriteBank08Anim01Seq[14];
extern const u16 gSpriteBank08Anim02Seq[14];

extern const struct sprite_anim gSpriteBank08Anims[3] = {
    /* 0 */ {
        /* seq */ gSpriteBank08Anim00Seq,
        /* box */ { { -15, -21, 32, 36 }, { -32, -23, 64, 47 } },
        /* paletteId */ 14,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank08Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank08Anim01Seq,
        /* box */ { { -15, -21, 32, 36 }, { -32, -27, 64, 51 } },
        /* paletteId */ 14,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank08Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank08Anim02Seq,
        /* box */ { { -15, -21, 32, 36 }, { -34, -23, 67, 48 } },
        /* paletteId */ 14,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank08Anim02Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank08Anim00Seq[14] = {
    0, 1, 2, 2, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9,
};
const u16 gSpriteBank08Anim01Seq[14] = {
    10, 11, 12, 12, 13, 14, 14, 15, 16, 16, 17, 18, 18, 19,
};
const u16 gSpriteBank08Anim02Seq[14] = {
    20, 21, 22, 22, 23, 24, 24, 25, 26, 26, 27, 28, 28, 29,
};

extern const struct sprite_frame *const gSpriteBank08Frames[30] = {
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
};

const struct sprite_frame_1box gSpriteBank08Frame000 = {
    SPRITE_FRAME(gSpriteBank08Frame000, SPRITE_TILES_BANK08_FRAME000),
    { { -16, -18, 39, 36 } },
};
const struct sprite_frame_1box gSpriteBank08Frame001 = {
    SPRITE_FRAME(gSpriteBank08Frame001, SPRITE_TILES_BANK08_FRAME001),
    { { -16, -18, 39, 36 } },
};
const struct sprite_frame_1box gSpriteBank08Frame002 = {
    SPRITE_FRAME(gSpriteBank08Frame002, SPRITE_TILES_BANK08_FRAME002),
    { { -16, -18, 39, 36 } },
};
const struct sprite_frame_1box gSpriteBank08Frame003 = {
    SPRITE_FRAME(gSpriteBank08Frame003, SPRITE_TILES_BANK08_FRAME003),
    { { -16, -18, 39, 36 } },
};
const struct sprite_frame_1box gSpriteBank08Frame004 = {
    SPRITE_FRAME(gSpriteBank08Frame004, SPRITE_TILES_BANK08_FRAME004),
    { { -16, -18, 39, 36 } },
};
const struct sprite_frame_1box gSpriteBank08Frame005 = {
    SPRITE_FRAME(gSpriteBank08Frame005, SPRITE_TILES_BANK08_FRAME005),
    { { -16, -18, 39, 36 } },
};
const struct sprite_frame_1box gSpriteBank08Frame006 = {
    SPRITE_FRAME(gSpriteBank08Frame006, SPRITE_TILES_BANK08_FRAME006),
    { { -16, -18, 39, 36 } },
};
const struct sprite_frame_1box gSpriteBank08Frame007 = {
    SPRITE_FRAME(gSpriteBank08Frame007, SPRITE_TILES_BANK08_FRAME007),
    { { -16, -18, 39, 36 } },
};
const struct sprite_frame_1box gSpriteBank08Frame008 = {
    SPRITE_FRAME(gSpriteBank08Frame008, SPRITE_TILES_BANK08_FRAME008),
    { { -16, -18, 39, 36 } },
};
const struct sprite_frame_1box gSpriteBank08Frame009 = {
    SPRITE_FRAME(gSpriteBank08Frame009, SPRITE_TILES_BANK08_FRAME009),
    { { -16, -18, 39, 36 } },
};
const struct sprite_frame_1box gSpriteBank08Frame010 = {
    SPRITE_FRAME(gSpriteBank08Frame010, SPRITE_TILES_BANK08_FRAME010),
    { { -12, -19, 33, 37 } },
};
const struct sprite_frame_1box gSpriteBank08Frame011 = {
    SPRITE_FRAME(gSpriteBank08Frame011, SPRITE_TILES_BANK08_FRAME011),
    { { -12, -19, 33, 37 } },
};
const struct sprite_frame_1box gSpriteBank08Frame012 = {
    SPRITE_FRAME(gSpriteBank08Frame012, SPRITE_TILES_BANK08_FRAME012),
    { { -12, -19, 33, 37 } },
};
const struct sprite_frame_1box gSpriteBank08Frame013 = {
    SPRITE_FRAME(gSpriteBank08Frame013, SPRITE_TILES_BANK08_FRAME013),
    { { -12, -19, 33, 37 } },
};
const struct sprite_frame_1box gSpriteBank08Frame014 = {
    SPRITE_FRAME(gSpriteBank08Frame014, SPRITE_TILES_BANK08_FRAME014),
    { { -12, -19, 33, 37 } },
};
const struct sprite_frame_1box gSpriteBank08Frame015 = {
    SPRITE_FRAME(gSpriteBank08Frame015, SPRITE_TILES_BANK08_FRAME015),
    { { -12, -19, 33, 37 } },
};
const struct sprite_frame_1box gSpriteBank08Frame016 = {
    SPRITE_FRAME(gSpriteBank08Frame016, SPRITE_TILES_BANK08_FRAME016),
    { { -12, -19, 33, 37 } },
};
const struct sprite_frame_1box gSpriteBank08Frame017 = {
    SPRITE_FRAME(gSpriteBank08Frame017, SPRITE_TILES_BANK08_FRAME017),
    { { -12, -19, 33, 37 } },
};
const struct sprite_frame_1box gSpriteBank08Frame018 = {
    SPRITE_FRAME(gSpriteBank08Frame018, SPRITE_TILES_BANK08_FRAME018),
    { { -12, -19, 33, 37 } },
};
const struct sprite_frame_1box gSpriteBank08Frame019 = {
    SPRITE_FRAME(gSpriteBank08Frame019, SPRITE_TILES_BANK08_FRAME019),
    { { -12, -19, 33, 37 } },
};
const struct sprite_frame_1box gSpriteBank08Frame020 = {
    SPRITE_FRAME(gSpriteBank08Frame020, SPRITE_TILES_BANK08_FRAME020),
    { { -18, -19, 39, 36 } },
};
const struct sprite_frame_1box gSpriteBank08Frame021 = {
    SPRITE_FRAME(gSpriteBank08Frame021, SPRITE_TILES_BANK08_FRAME021),
    { { -18, -19, 39, 36 } },
};
const struct sprite_frame_1box gSpriteBank08Frame022 = {
    SPRITE_FRAME(gSpriteBank08Frame022, SPRITE_TILES_BANK08_FRAME022),
    { { -18, -19, 39, 36 } },
};
const struct sprite_frame_1box gSpriteBank08Frame023 = {
    SPRITE_FRAME(gSpriteBank08Frame023, SPRITE_TILES_BANK08_FRAME023),
    { { -18, -19, 39, 36 } },
};
const struct sprite_frame_1box gSpriteBank08Frame024 = {
    SPRITE_FRAME(gSpriteBank08Frame024, SPRITE_TILES_BANK08_FRAME024),
    { { -18, -19, 39, 36 } },
};
const struct sprite_frame_1box gSpriteBank08Frame025 = {
    SPRITE_FRAME(gSpriteBank08Frame025, SPRITE_TILES_BANK08_FRAME025),
    { { -18, -19, 39, 36 } },
};
const struct sprite_frame_1box gSpriteBank08Frame026 = {
    SPRITE_FRAME(gSpriteBank08Frame026, SPRITE_TILES_BANK08_FRAME026),
    { { -18, -19, 39, 36 } },
};
const struct sprite_frame_1box gSpriteBank08Frame027 = {
    SPRITE_FRAME(gSpriteBank08Frame027, SPRITE_TILES_BANK08_FRAME027),
    { { -18, -19, 39, 36 } },
};
const struct sprite_frame_1box gSpriteBank08Frame028 = {
    SPRITE_FRAME(gSpriteBank08Frame028, SPRITE_TILES_BANK08_FRAME028),
    { { -18, -19, 39, 36 } },
};
const struct sprite_frame_1box gSpriteBank08Frame029 = {
    SPRITE_FRAME(gSpriteBank08Frame029, SPRITE_TILES_BANK08_FRAME029),
    { { -18, -19, 39, 36 } },
};

/* The piece arrays, from games/n2/graphics/sprites/bank08_carpet_rider/bank.json. */
#include "sprites/bank08_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 9: 2 animations, 19 frames, games/n2/graphics/sprites/bank09_scorpion_standing/. */

extern const u16 gSpriteBank09Anim00Seq[9];
extern const u16 gSpriteBank09Anim01Seq[10];

extern const struct sprite_anim gSpriteBank09Anims[2] = {
    /* 0 */ {
        /* seq */ gSpriteBank09Anim00Seq,
        /* box */ { { -14, -19, 29, 39 }, { -17, -24, 35, 46 } },
        /* paletteId */ 11,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank09Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank09Anim01Seq,
        /* box */ { { -15, -21, 30, 42 }, { -17, -24, 32, 45 } },
        /* paletteId */ 11,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank09Anim01Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank09Anim00Seq[9] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8,
};
const u16 gSpriteBank09Anim01Seq[10] = {
    9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
};

extern const struct sprite_frame *const gSpriteBank09Frames[19] = {
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
    &gSpriteBank09Frame014.frame,
    &gSpriteBank09Frame015.frame,
    &gSpriteBank09Frame016.frame,
    &gSpriteBank09Frame017.frame,
    &gSpriteBank09Frame018.frame,
};

const struct sprite_frame_1box gSpriteBank09Frame000 = {
    SPRITE_FRAME(gSpriteBank09Frame000, SPRITE_TILES_BANK09_FRAME000),
    { { -20, -13, 25, 30 } },
};
const struct sprite_frame_1box gSpriteBank09Frame001 = {
    SPRITE_FRAME(gSpriteBank09Frame001, SPRITE_TILES_BANK09_FRAME001),
    { { -20, -13, 25, 30 } },
};
const struct sprite_frame_1box gSpriteBank09Frame002 = {
    SPRITE_FRAME(gSpriteBank09Frame002, SPRITE_TILES_BANK09_FRAME002),
    { { -20, -13, 25, 30 } },
};
const struct sprite_frame_1box gSpriteBank09Frame003 = {
    SPRITE_FRAME(gSpriteBank09Frame003, SPRITE_TILES_BANK09_FRAME003),
    { { -20, -13, 25, 30 } },
};
const struct sprite_frame_1box gSpriteBank09Frame004 = {
    SPRITE_FRAME(gSpriteBank09Frame004, SPRITE_TILES_BANK09_FRAME004),
    { { -20, -13, 25, 30 } },
};
const struct sprite_frame_1box gSpriteBank09Frame005 = {
    SPRITE_FRAME(gSpriteBank09Frame005, SPRITE_TILES_BANK09_FRAME005),
    { { -20, -13, 25, 30 } },
};
const struct sprite_frame_1box gSpriteBank09Frame006 = {
    SPRITE_FRAME(gSpriteBank09Frame006, SPRITE_TILES_BANK09_FRAME006),
    { { -20, -13, 25, 30 } },
};
const struct sprite_frame_1box gSpriteBank09Frame007 = {
    SPRITE_FRAME(gSpriteBank09Frame007, SPRITE_TILES_BANK09_FRAME007),
    { { -20, -13, 25, 30 } },
};
const struct sprite_frame_1box gSpriteBank09Frame008 = {
    SPRITE_FRAME(gSpriteBank09Frame008, SPRITE_TILES_BANK09_FRAME008),
    { { -20, -13, 25, 30 } },
};
const struct sprite_frame_1box gSpriteBank09Frame009 = {
    SPRITE_FRAME(gSpriteBank09Frame009, SPRITE_TILES_BANK09_FRAME009),
    { { -20, -19, 25, 36 } },
};
const struct sprite_frame_1box gSpriteBank09Frame010 = {
    SPRITE_FRAME(gSpriteBank09Frame010, SPRITE_TILES_BANK09_FRAME010),
    { { -20, -19, 25, 36 } },
};
const struct sprite_frame_1box gSpriteBank09Frame011 = {
    SPRITE_FRAME(gSpriteBank09Frame011, SPRITE_TILES_BANK09_FRAME011),
    { { -20, -19, 25, 36 } },
};
const struct sprite_frame_1box gSpriteBank09Frame012 = {
    SPRITE_FRAME(gSpriteBank09Frame012, SPRITE_TILES_BANK09_FRAME012),
    { { -20, -19, 25, 36 } },
};
const struct sprite_frame_1box gSpriteBank09Frame013 = {
    SPRITE_FRAME(gSpriteBank09Frame013, SPRITE_TILES_BANK09_FRAME013),
    { { -20, -19, 25, 36 } },
};
const struct sprite_frame_1box gSpriteBank09Frame014 = {
    SPRITE_FRAME(gSpriteBank09Frame014, SPRITE_TILES_BANK09_FRAME014),
    { { -20, -19, 25, 36 } },
};
const struct sprite_frame_1box gSpriteBank09Frame015 = {
    SPRITE_FRAME(gSpriteBank09Frame015, SPRITE_TILES_BANK09_FRAME015),
    { { -20, -19, 25, 36 } },
};
const struct sprite_frame_1box gSpriteBank09Frame016 = {
    SPRITE_FRAME(gSpriteBank09Frame016, SPRITE_TILES_BANK09_FRAME016),
    { { -20, -19, 25, 36 } },
};
const struct sprite_frame_1box gSpriteBank09Frame017 = {
    SPRITE_FRAME(gSpriteBank09Frame017, SPRITE_TILES_BANK09_FRAME017),
    { { -20, -19, 25, 36 } },
};
const struct sprite_frame_1box gSpriteBank09Frame018 = {
    SPRITE_FRAME(gSpriteBank09Frame018, SPRITE_TILES_BANK09_FRAME018),
    { { -20, -19, 25, 36 } },
};

/* The piece arrays, from games/n2/graphics/sprites/bank09_scorpion_standing/bank.json. */
#include "sprites/bank09_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 10: 2 animations, 15 frames, games/n2/graphics/sprites/bank10_rock_pusher/. */

extern const u16 gSpriteBank10Anim00Seq[15];
extern const u16 gSpriteBank10Anim01Seq[14];

extern const struct sprite_anim gSpriteBank10Anims[2] = {
    /* 0 */ {
        /* seq */ gSpriteBank10Anim00Seq,
        /* box */ { { -16, -28, 24, 57 }, { -27, -28, 55, 56 } },
        /* paletteId */ 16,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank10Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank10Anim01Seq,
        /* box */ { { -16, -28, 24, 57 }, { -16, -30, 33, 58 } },
        /* paletteId */ 16,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank10Anim01Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank10Anim00Seq[15] = {
    0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7,
};
const u16 gSpriteBank10Anim01Seq[14] = {
    8, 9, 10, 11, 12, 10, 11, 11, 11, 11, 11, 11, 13, 14,
};

extern const struct sprite_frame *const gSpriteBank10Frames[15] = {
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
};

const struct sprite_frame_1box gSpriteBank10Frame000 = {
    SPRITE_FRAME(gSpriteBank10Frame000, SPRITE_TILES_BANK10_FRAME000),
    { { -16, -28, 22, 55 } },
};
const struct sprite_frame_1box gSpriteBank10Frame001 = {
    SPRITE_FRAME(gSpriteBank10Frame001, SPRITE_TILES_BANK10_FRAME001),
    { { -16, -28, 22, 55 } },
};
const struct sprite_frame_1box gSpriteBank10Frame002 = {
    SPRITE_FRAME(gSpriteBank10Frame002, SPRITE_TILES_BANK10_FRAME002),
    { { -16, -28, 22, 55 } },
};
const struct sprite_frame_1box gSpriteBank10Frame003 = {
    SPRITE_FRAME(gSpriteBank10Frame003, SPRITE_TILES_BANK10_FRAME003),
    { { -16, -28, 22, 55 } },
};
const struct sprite_frame_1box gSpriteBank10Frame004 = {
    SPRITE_FRAME(gSpriteBank10Frame004, SPRITE_TILES_BANK10_FRAME004),
    { { -16, -28, 22, 55 } },
};
const struct sprite_frame_1box gSpriteBank10Frame005 = {
    SPRITE_FRAME(gSpriteBank10Frame005, SPRITE_TILES_BANK10_FRAME005),
    { { -16, -28, 22, 55 } },
};
const struct sprite_frame_1box gSpriteBank10Frame006 = {
    SPRITE_FRAME(gSpriteBank10Frame006, SPRITE_TILES_BANK10_FRAME006),
    { { -16, -28, 22, 55 } },
};
const struct sprite_frame_1box gSpriteBank10Frame007 = {
    SPRITE_FRAME(gSpriteBank10Frame007, SPRITE_TILES_BANK10_FRAME007),
    { { -16, -28, 22, 55 } },
};
const struct sprite_frame_1box gSpriteBank10Frame008 = {
    SPRITE_FRAME(gSpriteBank10Frame008, SPRITE_TILES_BANK10_FRAME008),
    { { -17, -29, 21, 58 } },
};
const struct sprite_frame_1box gSpriteBank10Frame009 = {
    SPRITE_FRAME(gSpriteBank10Frame009, SPRITE_TILES_BANK10_FRAME009),
    { { -17, -29, 21, 58 } },
};
const struct sprite_frame_1box gSpriteBank10Frame010 = {
    SPRITE_FRAME(gSpriteBank10Frame010, SPRITE_TILES_BANK10_FRAME010),
    { { -17, -29, 21, 58 } },
};
const struct sprite_frame_1box gSpriteBank10Frame011 = {
    SPRITE_FRAME(gSpriteBank10Frame011, SPRITE_TILES_BANK10_FRAME011),
    { { -17, -29, 21, 58 } },
};
const struct sprite_frame_1box gSpriteBank10Frame012 = {
    SPRITE_FRAME(gSpriteBank10Frame012, SPRITE_TILES_BANK10_FRAME012),
    { { -17, -29, 21, 58 } },
};
const struct sprite_frame_1box gSpriteBank10Frame013 = {
    SPRITE_FRAME(gSpriteBank10Frame013, SPRITE_TILES_BANK10_FRAME013),
    { { -17, -29, 21, 58 } },
};
const struct sprite_frame_1box gSpriteBank10Frame014 = {
    SPRITE_FRAME(gSpriteBank10Frame014, SPRITE_TILES_BANK10_FRAME014),
    { { -17, -29, 21, 58 } },
};

/* The piece arrays, from games/n2/graphics/sprites/bank10_rock_pusher/bank.json. */
#include "sprites/bank10_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 11: 2 animations, 23 frames, games/n2/graphics/sprites/bank11_spike_block_mummy/. */

extern const u16 gSpriteBank11Anim00Seq[15];
extern const u16 gSpriteBank11Anim01Seq[15];

extern const struct sprite_anim gSpriteBank11Anims[2] = {
    /* 0 */ {
        /* seq */ gSpriteBank11Anim00Seq,
        /* box */ { { -10, -40, 23, 63 }, { -22, -41, 40, 66 } },
        /* paletteId */ 17,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank11Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank11Anim01Seq,
        /* box */ { { -10, -40, 23, 63 }, { -23, -41, 39, 66 } },
        /* paletteId */ 17,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank11Anim01Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank11Anim00Seq[15] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
};
const u16 gSpriteBank11Anim01Seq[15] = {
    15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22,
};

extern const struct sprite_frame *const gSpriteBank11Frames[23] = {
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
};

const struct sprite_frame_2box gSpriteBank11Frame000 = {
    SPRITE_FRAME(gSpriteBank11Frame000, SPRITE_TILES_BANK11_FRAME000),
    { { -10, -26, 23, 22 }, { -12, -7, 26, 30 } },
};
const struct sprite_frame_2box gSpriteBank11Frame001 = {
    SPRITE_FRAME(gSpriteBank11Frame001, SPRITE_TILES_BANK11_FRAME001),
    { { -10, -26, 23, 22 }, { -11, -10, 23, 33 } },
};
const struct sprite_frame_2box gSpriteBank11Frame002 = {
    SPRITE_FRAME(gSpriteBank11Frame002, SPRITE_TILES_BANK11_FRAME002),
    { { -10, -40, 21, 24 }, { -11, -13, 23, 34 } },
};
const struct sprite_frame_2box gSpriteBank11Frame003 = {
    SPRITE_FRAME(gSpriteBank11Frame003, SPRITE_TILES_BANK11_FRAME003),
    { { -11, -40, 19, 25 }, { -13, -13, 24, 34 } },
};
const struct sprite_frame_2box gSpriteBank11Frame004 = {
    SPRITE_FRAME(gSpriteBank11Frame004, SPRITE_TILES_BANK11_FRAME004),
    { { -13, -41, 21, 29 }, { -14, -11, 23, 32 } },
};
const struct sprite_frame_2box gSpriteBank11Frame005 = {
    SPRITE_FRAME(gSpriteBank11Frame005, SPRITE_TILES_BANK11_FRAME005),
    { { -11, -33, 17, 21 }, { -15, -10, 24, 32 } },
};
const struct sprite_frame_2box gSpriteBank11Frame006 = {
    SPRITE_FRAME(gSpriteBank11Frame006, SPRITE_TILES_BANK11_FRAME006),
    { { -10, -26, 23, 22 }, { -14, -10, 22, 34 } },
};
const struct sprite_frame_2box gSpriteBank11Frame007 = {
    SPRITE_FRAME(gSpriteBank11Frame007, SPRITE_TILES_BANK11_FRAME007),
    { { -10, -26, 23, 22 }, { -15, -4, 22, 31 } },
};
const struct sprite_frame_2box gSpriteBank11Frame008 = {
    SPRITE_FRAME(gSpriteBank11Frame008, SPRITE_TILES_BANK11_FRAME008),
    { { -10, -26, 23, 22 }, { -14, -6, 22, 32 } },
};
const struct sprite_frame_2box gSpriteBank11Frame009 = {
    SPRITE_FRAME(gSpriteBank11Frame009, SPRITE_TILES_BANK11_FRAME009),
    { { -12, -36, 22, 28 }, { -14, -8, 23, 32 } },
};
const struct sprite_frame_2box gSpriteBank11Frame010 = {
    SPRITE_FRAME(gSpriteBank11Frame010, SPRITE_TILES_BANK11_FRAME010),
    { { -14, -41, 21, 29 }, { -15, -11, 24, 34 } },
};
const struct sprite_frame_2box gSpriteBank11Frame011 = {
    SPRITE_FRAME(gSpriteBank11Frame011, SPRITE_TILES_BANK11_FRAME011),
    { { -15, -40, 20, 27 }, { -15, -11, 25, 32 } },
};
const struct sprite_frame_2box gSpriteBank11Frame012 = {
    SPRITE_FRAME(gSpriteBank11Frame012, SPRITE_TILES_BANK11_FRAME012),
    { { -12, -38, 20, 26 }, { -15, -11, 26, 32 } },
};
const struct sprite_frame_2box gSpriteBank11Frame013 = {
    SPRITE_FRAME(gSpriteBank11Frame013, SPRITE_TILES_BANK11_FRAME013),
    { { -11, -34, 18, 24 }, { -14, -11, 25, 32 } },
};
const struct sprite_frame_2box gSpriteBank11Frame014 = {
    SPRITE_FRAME(gSpriteBank11Frame014, SPRITE_TILES_BANK11_FRAME014),
    { { -10, -26, 23, 22 }, { -11, -9, 22, 31 } },
};
const struct sprite_frame_2box gSpriteBank11Frame015 = {
    SPRITE_FRAME(gSpriteBank11Frame015, SPRITE_TILES_BANK11_FRAME015),
    { { -14, -34, 25, 28 }, { -16, -5, 31, 29 } },
};
const struct sprite_frame_2box gSpriteBank11Frame016 = {
    SPRITE_FRAME(gSpriteBank11Frame016, SPRITE_TILES_BANK11_FRAME016),
    { { -14, -34, 25, 28 }, { -16, -5, 31, 29 } },
};
const struct sprite_frame_2box gSpriteBank11Frame017 = {
    SPRITE_FRAME(gSpriteBank11Frame017, SPRITE_TILES_BANK11_FRAME017),
    { { -14, -34, 25, 28 }, { -16, -5, 31, 29 } },
};
const struct sprite_frame_2box gSpriteBank11Frame018 = {
    SPRITE_FRAME(gSpriteBank11Frame018, SPRITE_TILES_BANK11_FRAME018),
    { { -14, -34, 25, 28 }, { -16, -5, 31, 29 } },
};
const struct sprite_frame_2box gSpriteBank11Frame019 = {
    SPRITE_FRAME(gSpriteBank11Frame019, SPRITE_TILES_BANK11_FRAME019),
    { { -14, -34, 25, 28 }, { -16, -5, 31, 29 } },
};
const struct sprite_frame_2box gSpriteBank11Frame020 = {
    SPRITE_FRAME(gSpriteBank11Frame020, SPRITE_TILES_BANK11_FRAME020),
    { { -14, -34, 25, 28 }, { -16, -5, 31, 29 } },
};
const struct sprite_frame_2box gSpriteBank11Frame021 = {
    SPRITE_FRAME(gSpriteBank11Frame021, SPRITE_TILES_BANK11_FRAME021),
    { { -14, -34, 25, 28 }, { -16, -5, 31, 29 } },
};
const struct sprite_frame_2box gSpriteBank11Frame022 = {
    SPRITE_FRAME(gSpriteBank11Frame022, SPRITE_TILES_BANK11_FRAME022),
    { { -14, -34, 25, 28 }, { -16, -5, 31, 29 } },
};

/* The piece arrays, from games/n2/graphics/sprites/bank11_spike_block_mummy/bank.json. */
#include "sprites/bank11_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 12: 4 animations, 21 frames, games/n2/graphics/sprites/bank12_shield_guard/. */

extern const u16 gSpriteBank12Anim00Seq[8];
extern const u16 gSpriteBank12Anim01Seq[7];
extern const u16 gSpriteBank12Anim02Seq[4];
extern const u16 gSpriteBank12Anim03Seq[4];

extern const struct sprite_anim gSpriteBank12Anims[4] = {
    /* 0 */ {
        /* seq */ gSpriteBank12Anim00Seq,
        /* box */ { { -12, -30, 24, 60 }, { -13, -30, 25, 60 } },
        /* paletteId */ 16,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank12Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank12Anim01Seq,
        /* box */ { { -12, -30, 24, 60 }, { -14, -29, 26, 59 } },
        /* paletteId */ 16,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank12Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank12Anim02Seq,
        /* box */ { { -12, -30, 24, 60 }, { -13, -29, 25, 59 } },
        /* paletteId */ 16,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank12Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank12Anim03Seq,
        /* box */ { { -12, -30, 24, 60 }, { -14, -15, 26, 45 } },
        /* paletteId */ 16,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank12Anim03Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank12Anim00Seq[8] = {
    0, 1, 2, 2, 3, 3, 4, 5,
};
const u16 gSpriteBank12Anim01Seq[7] = {
    6, 7, 8, 9, 10, 11, 12,
};
const u16 gSpriteBank12Anim02Seq[4] = {
    13, 14, 15, 16,
};
const u16 gSpriteBank12Anim03Seq[4] = {
    17, 18, 19, 20,
};

extern const struct sprite_frame *const gSpriteBank12Frames[21] = {
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
};

const struct sprite_frame_3box_anchor gSpriteBank12Frame000 = {
    SPRITE_FRAME(gSpriteBank12Frame000, SPRITE_TILES_BANK12_FRAME000),
    { { -13, -26, 11, 57 }, { 0, 0, 0, 0 }, { 0, -30, 12, 45 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank12Frame001 = {
    SPRITE_FRAME(gSpriteBank12Frame001, SPRITE_TILES_BANK12_FRAME001),
    { { -13, -26, 11, 57 }, { 0, 0, 0, 0 }, { 0, -30, 12, 45 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank12Frame002 = {
    SPRITE_FRAME(gSpriteBank12Frame002, SPRITE_TILES_BANK12_FRAME002),
    { { -13, -26, 11, 57 }, { 0, 0, 0, 0 }, { 0, -30, 12, 45 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank12Frame003 = {
    SPRITE_FRAME(gSpriteBank12Frame003, SPRITE_TILES_BANK12_FRAME003),
    { { -13, -26, 11, 57 }, { 0, 0, 0, 0 }, { 0, -30, 12, 45 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank12Frame004 = {
    SPRITE_FRAME(gSpriteBank12Frame004, SPRITE_TILES_BANK12_FRAME004),
    { { -13, -26, 11, 57 }, { 0, 0, 0, 0 }, { 0, -30, 12, 45 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank12Frame005 = {
    SPRITE_FRAME(gSpriteBank12Frame005, SPRITE_TILES_BANK12_FRAME005),
    { { -13, -26, 11, 57 }, { 0, 0, 0, 0 }, { 0, -30, 12, 45 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank12Frame006 = {
    SPRITE_FRAME(gSpriteBank12Frame006, SPRITE_TILES_BANK12_FRAME006),
    { { -13, -25, 12, 56 }, { 0, 0, 0, 0 }, { 0, -28, 12, 44 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank12Frame007 = {
    SPRITE_FRAME(gSpriteBank12Frame007, SPRITE_TILES_BANK12_FRAME007),
    { { -12, -21, 10, 51 }, { 0, 0, 0, 0 }, { 0, -28, 12, 44 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank12Frame008 = {
    SPRITE_FRAME(gSpriteBank12Frame008, SPRITE_TILES_BANK12_FRAME008),
    { { -13, -17, 11, 47 }, { 0, 0, 0, 0 }, { 1, -18, 12, 42 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank12Frame009 = {
    SPRITE_FRAME(gSpriteBank12Frame009, SPRITE_TILES_BANK12_FRAME009),
    { { -13, -14, 11, 45 }, { 0, 0, 0, 0 }, { 0, -13, 13, 42 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank12Frame010 = {
    SPRITE_FRAME(gSpriteBank12Frame010, SPRITE_TILES_BANK12_FRAME010),
    { { -15, -13, 13, 43 }, { 0, 0, 0, 0 }, { 0, -14, 13, 44 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank12Frame011 = {
    SPRITE_FRAME(gSpriteBank12Frame011, SPRITE_TILES_BANK12_FRAME011),
    { { -14, -14, 12, 45 }, { 0, 0, 0, 0 }, { 0, -14, 12, 45 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank12Frame012 = {
    SPRITE_FRAME(gSpriteBank12Frame012, SPRITE_TILES_BANK12_FRAME012),
    { { -14, -15, 12, 45 }, { 0, 0, 0, 0 }, { 0, -13, 12, 44 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank12Frame013 = {
    SPRITE_FRAME(gSpriteBank12Frame013, SPRITE_TILES_BANK12_FRAME013),
    { { -13, -15, 12, 45 }, { 0, 0, 0, 0 }, { 0, -14, 12, 44 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank12Frame014 = {
    SPRITE_FRAME(gSpriteBank12Frame014, SPRITE_TILES_BANK12_FRAME014),
    { { -12, -17, 11, 47 }, { 0, 0, 0, 0 }, { -1, -18, 13, 42 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank12Frame015 = {
    SPRITE_FRAME(gSpriteBank12Frame015, SPRITE_TILES_BANK12_FRAME015),
    { { -12, -22, 10, 52 }, { 0, 0, 0, 0 }, { 0, -23, 13, 43 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank12Frame016 = {
    SPRITE_FRAME(gSpriteBank12Frame016, SPRITE_TILES_BANK12_FRAME016),
    { { -12, -26, 10, 55 }, { 0, 0, 0, 0 }, { -2, -27, 15, 42 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank12Frame017 = {
    SPRITE_FRAME(gSpriteBank12Frame017, SPRITE_TILES_BANK12_FRAME017),
    { { -14, -14, 11, 43 }, { 0, 0, 0, 0 }, { -1, -13, 13, 43 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank12Frame018 = {
    SPRITE_FRAME(gSpriteBank12Frame018, SPRITE_TILES_BANK12_FRAME018),
    { { -14, -14, 11, 43 }, { 0, 0, 0, 0 }, { -1, -13, 13, 43 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank12Frame019 = {
    SPRITE_FRAME(gSpriteBank12Frame019, SPRITE_TILES_BANK12_FRAME019),
    { { -14, -14, 11, 43 }, { 0, 0, 0, 0 }, { -1, -13, 13, 43 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank12Frame020 = {
    SPRITE_FRAME(gSpriteBank12Frame020, SPRITE_TILES_BANK12_FRAME020),
    { { -14, -14, 11, 43 }, { 0, 0, 0, 0 }, { -1, -13, 13, 43 } },
    { 0, 0 },
};

/* The piece arrays, from games/n2/graphics/sprites/bank12_shield_guard/bank.json. */
#include "sprites/bank12_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 13: 4 animations, 38 frames, games/n2/graphics/sprites/bank13_scarab/. */

extern const u16 gSpriteBank13Anim00Seq[10];
extern const u16 gSpriteBank13Anim01Seq[10];
extern const u16 gSpriteBank13Anim02Seq[10];
extern const u16 gSpriteBank13Anim03Seq[8];

extern const struct sprite_anim gSpriteBank13Anims[4] = {
    /* 0 */ {
        /* seq */ gSpriteBank13Anim00Seq,
        /* box */ { { -14, -8, 32, 18 }, { -18, -22, 37, 43 } },
        /* paletteId */ 18,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank13Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank13Anim01Seq,
        /* box */ { { -14, -8, 32, 18 }, { -18, -22, 42, 42 } },
        /* paletteId */ 18,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank13Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank13Anim02Seq,
        /* box */ { { -14, -8, 32, 18 }, { -18, -22, 42, 42 } },
        /* paletteId */ 18,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank13Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank13Anim03Seq,
        /* box */ { { -14, -8, 32, 18 }, { -21, -11, 51, 22 } },
        /* paletteId */ 18,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank13Anim03Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank13Anim00Seq[10] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
};
const u16 gSpriteBank13Anim01Seq[10] = {
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
};
const u16 gSpriteBank13Anim02Seq[10] = {
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
};
const u16 gSpriteBank13Anim03Seq[8] = {
    30, 31, 32, 33, 34, 35, 36, 37,
};

extern const struct sprite_frame *const gSpriteBank13Frames[38] = {
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
    &gSpriteBank13Frame019.frame,
    &gSpriteBank13Frame020.frame,
    &gSpriteBank13Frame021.frame,
    &gSpriteBank13Frame022.frame,
    &gSpriteBank13Frame023.frame,
    &gSpriteBank13Frame024.frame,
    &gSpriteBank13Frame025.frame,
    &gSpriteBank13Frame026.frame,
    &gSpriteBank13Frame027.frame,
    &gSpriteBank13Frame028.frame,
    &gSpriteBank13Frame029.frame,
    &gSpriteBank13Frame030.frame,
    &gSpriteBank13Frame031.frame,
    &gSpriteBank13Frame032.frame,
    &gSpriteBank13Frame033.frame,
    &gSpriteBank13Frame034.frame,
    &gSpriteBank13Frame035.frame,
    &gSpriteBank13Frame036.frame,
    &gSpriteBank13Frame037.frame,
};

const struct sprite_frame_1box gSpriteBank13Frame000 = {
    SPRITE_FRAME(gSpriteBank13Frame000, SPRITE_TILES_BANK13_FRAME000),
    { { -10, -10, 27, 22 } },
};
const struct sprite_frame_1box gSpriteBank13Frame001 = {
    SPRITE_FRAME(gSpriteBank13Frame001, SPRITE_TILES_BANK13_FRAME001),
    { { -10, -10, 27, 22 } },
};
const struct sprite_frame_1box gSpriteBank13Frame002 = {
    SPRITE_FRAME(gSpriteBank13Frame002, SPRITE_TILES_BANK13_FRAME002),
    { { -10, -10, 27, 22 } },
};
const struct sprite_frame_1box gSpriteBank13Frame003 = {
    SPRITE_FRAME(gSpriteBank13Frame003, SPRITE_TILES_BANK13_FRAME003),
    { { -10, -10, 27, 22 } },
};
const struct sprite_frame_1box gSpriteBank13Frame004 = {
    SPRITE_FRAME(gSpriteBank13Frame004, SPRITE_TILES_BANK13_FRAME004),
    { { -10, -10, 27, 22 } },
};
const struct sprite_frame_1box gSpriteBank13Frame005 = {
    SPRITE_FRAME(gSpriteBank13Frame005, SPRITE_TILES_BANK13_FRAME005),
    { { -10, -10, 27, 22 } },
};
const struct sprite_frame_1box gSpriteBank13Frame006 = {
    SPRITE_FRAME(gSpriteBank13Frame006, SPRITE_TILES_BANK13_FRAME006),
    { { -10, -10, 27, 22 } },
};
const struct sprite_frame_1box gSpriteBank13Frame007 = {
    SPRITE_FRAME(gSpriteBank13Frame007, SPRITE_TILES_BANK13_FRAME007),
    { { -10, -10, 27, 22 } },
};
const struct sprite_frame_1box gSpriteBank13Frame008 = {
    SPRITE_FRAME(gSpriteBank13Frame008, SPRITE_TILES_BANK13_FRAME008),
    { { -10, -10, 27, 22 } },
};
const struct sprite_frame_1box gSpriteBank13Frame009 = {
    SPRITE_FRAME(gSpriteBank13Frame009, SPRITE_TILES_BANK13_FRAME009),
    { { -10, -10, 27, 22 } },
};
const struct sprite_frame_1box gSpriteBank13Frame010 = {
    SPRITE_FRAME(gSpriteBank13Frame010, SPRITE_TILES_BANK13_FRAME010),
    { { -9, -5, 27, 17 } },
};
const struct sprite_frame_1box gSpriteBank13Frame011 = {
    SPRITE_FRAME(gSpriteBank13Frame011, SPRITE_TILES_BANK13_FRAME011),
    { { -9, -5, 27, 17 } },
};
const struct sprite_frame_1box gSpriteBank13Frame012 = {
    SPRITE_FRAME(gSpriteBank13Frame012, SPRITE_TILES_BANK13_FRAME012),
    { { -9, -5, 27, 17 } },
};
const struct sprite_frame_1box gSpriteBank13Frame013 = {
    SPRITE_FRAME(gSpriteBank13Frame013, SPRITE_TILES_BANK13_FRAME013),
    { { -9, -5, 27, 17 } },
};
const struct sprite_frame_1box gSpriteBank13Frame014 = {
    SPRITE_FRAME(gSpriteBank13Frame014, SPRITE_TILES_BANK13_FRAME014),
    { { -9, -5, 27, 17 } },
};
const struct sprite_frame_1box gSpriteBank13Frame015 = {
    SPRITE_FRAME(gSpriteBank13Frame015, SPRITE_TILES_BANK13_FRAME015),
    { { -9, -5, 27, 17 } },
};
const struct sprite_frame_1box gSpriteBank13Frame016 = {
    SPRITE_FRAME(gSpriteBank13Frame016, SPRITE_TILES_BANK13_FRAME016),
    { { -9, -5, 27, 17 } },
};
const struct sprite_frame_1box gSpriteBank13Frame017 = {
    SPRITE_FRAME(gSpriteBank13Frame017, SPRITE_TILES_BANK13_FRAME017),
    { { -9, -5, 27, 17 } },
};
const struct sprite_frame_1box gSpriteBank13Frame018 = {
    SPRITE_FRAME(gSpriteBank13Frame018, SPRITE_TILES_BANK13_FRAME018),
    { { -9, -5, 27, 17 } },
};
const struct sprite_frame_1box gSpriteBank13Frame019 = {
    SPRITE_FRAME(gSpriteBank13Frame019, SPRITE_TILES_BANK13_FRAME019),
    { { -9, -5, 27, 17 } },
};
const struct sprite_frame_1box gSpriteBank13Frame020 = {
    SPRITE_FRAME(gSpriteBank13Frame020, SPRITE_TILES_BANK13_FRAME020),
    { { -13, -8, 33, 19 } },
};
const struct sprite_frame_1box gSpriteBank13Frame021 = {
    SPRITE_FRAME(gSpriteBank13Frame021, SPRITE_TILES_BANK13_FRAME021),
    { { -13, -8, 33, 19 } },
};
const struct sprite_frame_1box gSpriteBank13Frame022 = {
    SPRITE_FRAME(gSpriteBank13Frame022, SPRITE_TILES_BANK13_FRAME022),
    { { -13, -8, 33, 19 } },
};
const struct sprite_frame_1box gSpriteBank13Frame023 = {
    SPRITE_FRAME(gSpriteBank13Frame023, SPRITE_TILES_BANK13_FRAME023),
    { { -13, -8, 33, 19 } },
};
const struct sprite_frame_1box gSpriteBank13Frame024 = {
    SPRITE_FRAME(gSpriteBank13Frame024, SPRITE_TILES_BANK13_FRAME024),
    { { -13, -8, 33, 19 } },
};
const struct sprite_frame_1box gSpriteBank13Frame025 = {
    SPRITE_FRAME(gSpriteBank13Frame025, SPRITE_TILES_BANK13_FRAME025),
    { { -13, -8, 33, 19 } },
};
const struct sprite_frame_1box gSpriteBank13Frame026 = {
    SPRITE_FRAME(gSpriteBank13Frame026, SPRITE_TILES_BANK13_FRAME026),
    { { -13, -8, 33, 19 } },
};
const struct sprite_frame_1box gSpriteBank13Frame027 = {
    SPRITE_FRAME(gSpriteBank13Frame027, SPRITE_TILES_BANK13_FRAME027),
    { { -13, -8, 33, 19 } },
};
const struct sprite_frame_1box gSpriteBank13Frame028 = {
    SPRITE_FRAME(gSpriteBank13Frame028, SPRITE_TILES_BANK13_FRAME028),
    { { -13, -8, 33, 19 } },
};
const struct sprite_frame_1box gSpriteBank13Frame029 = {
    SPRITE_FRAME(gSpriteBank13Frame029, SPRITE_TILES_BANK13_FRAME029),
    { { -13, -8, 33, 19 } },
};
const struct sprite_frame_1box gSpriteBank13Frame030 = {
    SPRITE_FRAME(gSpriteBank13Frame030, SPRITE_TILES_BANK13_FRAME030),
    { { -11, -9, 30, 14 } },
};
const struct sprite_frame_1box gSpriteBank13Frame031 = {
    SPRITE_FRAME(gSpriteBank13Frame031, SPRITE_TILES_BANK13_FRAME031),
    { { -11, -9, 30, 14 } },
};
const struct sprite_frame_1box gSpriteBank13Frame032 = {
    SPRITE_FRAME(gSpriteBank13Frame032, SPRITE_TILES_BANK13_FRAME032),
    { { -11, -9, 30, 14 } },
};
const struct sprite_frame_1box gSpriteBank13Frame033 = {
    SPRITE_FRAME(gSpriteBank13Frame033, SPRITE_TILES_BANK13_FRAME033),
    { { -11, -9, 30, 14 } },
};
const struct sprite_frame_1box gSpriteBank13Frame034 = {
    SPRITE_FRAME(gSpriteBank13Frame034, SPRITE_TILES_BANK13_FRAME034),
    { { -11, -9, 30, 14 } },
};
const struct sprite_frame_1box gSpriteBank13Frame035 = {
    SPRITE_FRAME(gSpriteBank13Frame035, SPRITE_TILES_BANK13_FRAME035),
    { { -11, -9, 30, 14 } },
};
const struct sprite_frame_1box gSpriteBank13Frame036 = {
    SPRITE_FRAME(gSpriteBank13Frame036, SPRITE_TILES_BANK13_FRAME036),
    { { -11, -9, 30, 14 } },
};
const struct sprite_frame_1box gSpriteBank13Frame037 = {
    SPRITE_FRAME(gSpriteBank13Frame037, SPRITE_TILES_BANK13_FRAME037),
    { { -11, -9, 30, 14 } },
};

/* The piece arrays, from games/n2/graphics/sprites/bank13_scarab/bank.json. */
#include "sprites/bank13_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 14: 3 animations, 30 frames, games/n2/graphics/sprites/bank14_sarcophagus/. */

extern const u16 gSpriteBank14Anim00Seq[15];
extern const u16 gSpriteBank14Anim01Seq[15];
extern const u16 gSpriteBank14Anim02Seq[7];

extern const struct sprite_anim gSpriteBank14Anims[3] = {
    /* 0 */ {
        /* seq */ gSpriteBank14Anim00Seq,
        /* box */ { { -13, -26, 27, 53 }, { -18, -36, 33, 64 } },
        /* paletteId */ 19,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank14Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank14Anim01Seq,
        /* box */ { { -13, -26, 27, 53 }, { -18, -37, 38, 64 } },
        /* paletteId */ 19,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank14Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank14Anim02Seq,
        /* box */ { { -13, -26, 27, 53 }, { -33, -34, 60, 66 } },
        /* paletteId */ 19,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank14Anim02Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank14Anim00Seq[15] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
};
const u16 gSpriteBank14Anim01Seq[15] = {
    15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22,
};
const u16 gSpriteBank14Anim02Seq[7] = {
    23, 24, 25, 26, 27, 28, 29,
};

extern const struct sprite_frame *const gSpriteBank14Frames[30] = {
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
    &gSpriteBank14Frame023,
    &gSpriteBank14Frame024,
    &gSpriteBank14Frame025,
    &gSpriteBank14Frame026,
    &gSpriteBank14Frame027,
    &gSpriteBank14Frame028,
    &gSpriteBank14Frame029,
};

const struct sprite_frame_1box gSpriteBank14Frame000 = {
    SPRITE_FRAME(gSpriteBank14Frame000, SPRITE_TILES_BANK14_FRAME000),
    { { -18, -22, 27, 45 } },
};
const struct sprite_frame_1box gSpriteBank14Frame001 = {
    SPRITE_FRAME(gSpriteBank14Frame001, SPRITE_TILES_BANK14_FRAME001),
    { { -18, -22, 27, 45 } },
};
const struct sprite_frame_1box gSpriteBank14Frame002 = {
    SPRITE_FRAME(gSpriteBank14Frame002, SPRITE_TILES_BANK14_FRAME002),
    { { -18, -22, 27, 45 } },
};
const struct sprite_frame_1box gSpriteBank14Frame003 = {
    SPRITE_FRAME(gSpriteBank14Frame003, SPRITE_TILES_BANK14_FRAME003),
    { { -18, -22, 27, 45 } },
};
const struct sprite_frame_1box gSpriteBank14Frame004 = {
    SPRITE_FRAME(gSpriteBank14Frame004, SPRITE_TILES_BANK14_FRAME004),
    { { -18, -22, 27, 45 } },
};
const struct sprite_frame_1box gSpriteBank14Frame005 = {
    SPRITE_FRAME(gSpriteBank14Frame005, SPRITE_TILES_BANK14_FRAME005),
    { { -18, -22, 27, 45 } },
};
const struct sprite_frame_1box gSpriteBank14Frame006 = {
    SPRITE_FRAME(gSpriteBank14Frame006, SPRITE_TILES_BANK14_FRAME006),
    { { -18, -22, 27, 45 } },
};
const struct sprite_frame_1box gSpriteBank14Frame007 = {
    SPRITE_FRAME(gSpriteBank14Frame007, SPRITE_TILES_BANK14_FRAME007),
    { { -18, -22, 27, 45 } },
};
const struct sprite_frame_1box gSpriteBank14Frame008 = {
    SPRITE_FRAME(gSpriteBank14Frame008, SPRITE_TILES_BANK14_FRAME008),
    { { -18, -22, 27, 45 } },
};
const struct sprite_frame_1box gSpriteBank14Frame009 = {
    SPRITE_FRAME(gSpriteBank14Frame009, SPRITE_TILES_BANK14_FRAME009),
    { { -18, -22, 27, 45 } },
};
const struct sprite_frame_1box gSpriteBank14Frame010 = {
    SPRITE_FRAME(gSpriteBank14Frame010, SPRITE_TILES_BANK14_FRAME010),
    { { -18, -22, 27, 45 } },
};
const struct sprite_frame_1box gSpriteBank14Frame011 = {
    SPRITE_FRAME(gSpriteBank14Frame011, SPRITE_TILES_BANK14_FRAME011),
    { { -18, -22, 27, 45 } },
};
const struct sprite_frame_1box gSpriteBank14Frame012 = {
    SPRITE_FRAME(gSpriteBank14Frame012, SPRITE_TILES_BANK14_FRAME012),
    { { -18, -22, 27, 45 } },
};
const struct sprite_frame_1box gSpriteBank14Frame013 = {
    SPRITE_FRAME(gSpriteBank14Frame013, SPRITE_TILES_BANK14_FRAME013),
    { { -18, -22, 27, 45 } },
};
const struct sprite_frame_1box gSpriteBank14Frame014 = {
    SPRITE_FRAME(gSpriteBank14Frame014, SPRITE_TILES_BANK14_FRAME014),
    { { -18, -22, 27, 45 } },
};
const struct sprite_frame_1box gSpriteBank14Frame015 = {
    SPRITE_FRAME(gSpriteBank14Frame015, SPRITE_TILES_BANK14_FRAME015),
    { { -11, -24, 21, 48 } },
};
const struct sprite_frame_1box gSpriteBank14Frame016 = {
    SPRITE_FRAME(gSpriteBank14Frame016, SPRITE_TILES_BANK14_FRAME016),
    { { -11, -24, 21, 48 } },
};
const struct sprite_frame_1box gSpriteBank14Frame017 = {
    SPRITE_FRAME(gSpriteBank14Frame017, SPRITE_TILES_BANK14_FRAME017),
    { { -11, -24, 21, 48 } },
};
const struct sprite_frame_1box gSpriteBank14Frame018 = {
    SPRITE_FRAME(gSpriteBank14Frame018, SPRITE_TILES_BANK14_FRAME018),
    { { -11, -24, 21, 48 } },
};
const struct sprite_frame_1box gSpriteBank14Frame019 = {
    SPRITE_FRAME(gSpriteBank14Frame019, SPRITE_TILES_BANK14_FRAME019),
    { { -11, -24, 21, 48 } },
};
const struct sprite_frame_1box gSpriteBank14Frame020 = {
    SPRITE_FRAME(gSpriteBank14Frame020, SPRITE_TILES_BANK14_FRAME020),
    { { -11, -24, 21, 48 } },
};
const struct sprite_frame_1box gSpriteBank14Frame021 = {
    SPRITE_FRAME(gSpriteBank14Frame021, SPRITE_TILES_BANK14_FRAME021),
    { { -11, -24, 21, 48 } },
};
const struct sprite_frame_1box gSpriteBank14Frame022 = {
    SPRITE_FRAME(gSpriteBank14Frame022, SPRITE_TILES_BANK14_FRAME022),
    { { -11, -24, 21, 48 } },
};
const struct sprite_frame gSpriteBank14Frame023 = SPRITE_FRAME(gSpriteBank14Frame023, SPRITE_TILES_BANK14_FRAME023);
const struct sprite_frame gSpriteBank14Frame024 = SPRITE_FRAME(gSpriteBank14Frame024, SPRITE_TILES_BANK14_FRAME024);
const struct sprite_frame gSpriteBank14Frame025 = SPRITE_FRAME(gSpriteBank14Frame025, SPRITE_TILES_BANK14_FRAME025);
const struct sprite_frame gSpriteBank14Frame026 = SPRITE_FRAME(gSpriteBank14Frame026, SPRITE_TILES_BANK14_FRAME026);
const struct sprite_frame gSpriteBank14Frame027 = SPRITE_FRAME(gSpriteBank14Frame027, SPRITE_TILES_BANK14_FRAME027);
const struct sprite_frame gSpriteBank14Frame028 = SPRITE_FRAME(gSpriteBank14Frame028, SPRITE_TILES_BANK14_FRAME028);
const struct sprite_frame gSpriteBank14Frame029 = SPRITE_FRAME(gSpriteBank14Frame029, SPRITE_TILES_BANK14_FRAME029);

/* The piece arrays, from games/n2/graphics/sprites/bank14_sarcophagus/bank.json. */
#include "sprites/bank14_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 15: 2 animations, 56 frames, games/n2/graphics/sprites/bank15_mummy/. */

extern const u16 gSpriteBank15Anim00Seq[33];
extern const u16 gSpriteBank15Anim01Seq[30];

extern const struct sprite_anim gSpriteBank15Anims[2] = {
    /* 0 */ {
        /* seq */ gSpriteBank15Anim00Seq,
        /* box */ { { -17, -29, 34, 59 }, { -29, -30, 48, 62 } },
        /* paletteId */ 20,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank15Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank15Anim01Seq,
        /* box */ { { -17, -29, 34, 59 }, { -27, -30, 49, 63 } },
        /* paletteId */ 20,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank15Anim01Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank15Anim00Seq[33] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 14,
    15, 16, 17, 18, 19, 20, 21, 21, 22, 23, 24, 25, 26, 26, 27, 27,
    28,
};
const u16 gSpriteBank15Anim01Seq[30] = {
    29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 43,
    44, 45, 46, 47, 48, 49, 50, 50, 51, 52, 53, 54, 55, 55,
};

extern const struct sprite_frame *const gSpriteBank15Frames[56] = {
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
};

const struct sprite_frame_1box gSpriteBank15Frame000 = {
    SPRITE_FRAME(gSpriteBank15Frame000, SPRITE_TILES_BANK15_FRAME000),
    { { -11, -29, 16, 58 } },
};
const struct sprite_frame_1box gSpriteBank15Frame001 = {
    SPRITE_FRAME(gSpriteBank15Frame001, SPRITE_TILES_BANK15_FRAME001),
    { { -11, -29, 16, 58 } },
};
const struct sprite_frame_1box gSpriteBank15Frame002 = {
    SPRITE_FRAME(gSpriteBank15Frame002, SPRITE_TILES_BANK15_FRAME002),
    { { -11, -29, 16, 58 } },
};
const struct sprite_frame_1box gSpriteBank15Frame003 = {
    SPRITE_FRAME(gSpriteBank15Frame003, SPRITE_TILES_BANK15_FRAME003),
    { { -11, -29, 16, 58 } },
};
const struct sprite_frame_1box gSpriteBank15Frame004 = {
    SPRITE_FRAME(gSpriteBank15Frame004, SPRITE_TILES_BANK15_FRAME004),
    { { -11, -29, 16, 58 } },
};
const struct sprite_frame_1box gSpriteBank15Frame005 = {
    SPRITE_FRAME(gSpriteBank15Frame005, SPRITE_TILES_BANK15_FRAME005),
    { { -11, -29, 16, 58 } },
};
const struct sprite_frame_1box gSpriteBank15Frame006 = {
    SPRITE_FRAME(gSpriteBank15Frame006, SPRITE_TILES_BANK15_FRAME006),
    { { -11, -29, 16, 58 } },
};
const struct sprite_frame_1box gSpriteBank15Frame007 = {
    SPRITE_FRAME(gSpriteBank15Frame007, SPRITE_TILES_BANK15_FRAME007),
    { { -11, -29, 16, 58 } },
};
const struct sprite_frame_1box gSpriteBank15Frame008 = {
    SPRITE_FRAME(gSpriteBank15Frame008, SPRITE_TILES_BANK15_FRAME008),
    { { -11, -29, 16, 58 } },
};
const struct sprite_frame_1box gSpriteBank15Frame009 = {
    SPRITE_FRAME(gSpriteBank15Frame009, SPRITE_TILES_BANK15_FRAME009),
    { { -11, -29, 16, 58 } },
};
const struct sprite_frame_1box gSpriteBank15Frame010 = {
    SPRITE_FRAME(gSpriteBank15Frame010, SPRITE_TILES_BANK15_FRAME010),
    { { -11, -29, 16, 58 } },
};
const struct sprite_frame_1box gSpriteBank15Frame011 = {
    SPRITE_FRAME(gSpriteBank15Frame011, SPRITE_TILES_BANK15_FRAME011),
    { { -11, -29, 16, 58 } },
};
const struct sprite_frame_1box gSpriteBank15Frame012 = {
    SPRITE_FRAME(gSpriteBank15Frame012, SPRITE_TILES_BANK15_FRAME012),
    { { -11, -29, 16, 58 } },
};
const struct sprite_frame_1box gSpriteBank15Frame013 = {
    SPRITE_FRAME(gSpriteBank15Frame013, SPRITE_TILES_BANK15_FRAME013),
    { { -11, -29, 16, 58 } },
};
const struct sprite_frame_1box gSpriteBank15Frame014 = {
    SPRITE_FRAME(gSpriteBank15Frame014, SPRITE_TILES_BANK15_FRAME014),
    { { -11, -29, 16, 58 } },
};
const struct sprite_frame_1box gSpriteBank15Frame015 = {
    SPRITE_FRAME(gSpriteBank15Frame015, SPRITE_TILES_BANK15_FRAME015),
    { { -11, -29, 16, 58 } },
};
const struct sprite_frame_1box gSpriteBank15Frame016 = {
    SPRITE_FRAME(gSpriteBank15Frame016, SPRITE_TILES_BANK15_FRAME016),
    { { -11, -29, 16, 58 } },
};
const struct sprite_frame_1box gSpriteBank15Frame017 = {
    SPRITE_FRAME(gSpriteBank15Frame017, SPRITE_TILES_BANK15_FRAME017),
    { { -11, -29, 16, 58 } },
};
const struct sprite_frame_1box gSpriteBank15Frame018 = {
    SPRITE_FRAME(gSpriteBank15Frame018, SPRITE_TILES_BANK15_FRAME018),
    { { -11, -29, 16, 58 } },
};
const struct sprite_frame_1box gSpriteBank15Frame019 = {
    SPRITE_FRAME(gSpriteBank15Frame019, SPRITE_TILES_BANK15_FRAME019),
    { { -11, -29, 16, 58 } },
};
const struct sprite_frame_1box gSpriteBank15Frame020 = {
    SPRITE_FRAME(gSpriteBank15Frame020, SPRITE_TILES_BANK15_FRAME020),
    { { -11, -29, 16, 58 } },
};
const struct sprite_frame_1box gSpriteBank15Frame021 = {
    SPRITE_FRAME(gSpriteBank15Frame021, SPRITE_TILES_BANK15_FRAME021),
    { { -11, -29, 16, 58 } },
};
const struct sprite_frame_1box gSpriteBank15Frame022 = {
    SPRITE_FRAME(gSpriteBank15Frame022, SPRITE_TILES_BANK15_FRAME022),
    { { -11, -29, 16, 58 } },
};
const struct sprite_frame_1box gSpriteBank15Frame023 = {
    SPRITE_FRAME(gSpriteBank15Frame023, SPRITE_TILES_BANK15_FRAME023),
    { { -11, -29, 16, 58 } },
};
const struct sprite_frame_1box gSpriteBank15Frame024 = {
    SPRITE_FRAME(gSpriteBank15Frame024, SPRITE_TILES_BANK15_FRAME024),
    { { -11, -29, 16, 58 } },
};
const struct sprite_frame_1box gSpriteBank15Frame025 = {
    SPRITE_FRAME(gSpriteBank15Frame025, SPRITE_TILES_BANK15_FRAME025),
    { { -11, -29, 16, 58 } },
};
const struct sprite_frame_1box gSpriteBank15Frame026 = {
    SPRITE_FRAME(gSpriteBank15Frame026, SPRITE_TILES_BANK15_FRAME026),
    { { -11, -29, 16, 58 } },
};
const struct sprite_frame_1box gSpriteBank15Frame027 = {
    SPRITE_FRAME(gSpriteBank15Frame027, SPRITE_TILES_BANK15_FRAME027),
    { { -11, -29, 16, 58 } },
};
const struct sprite_frame_1box gSpriteBank15Frame028 = {
    SPRITE_FRAME(gSpriteBank15Frame028, SPRITE_TILES_BANK15_FRAME028),
    { { -11, -29, 16, 58 } },
};
const struct sprite_frame_1box gSpriteBank15Frame029 = {
    SPRITE_FRAME(gSpriteBank15Frame029, SPRITE_TILES_BANK15_FRAME029),
    { { -10, -29, 16, 59 } },
};
const struct sprite_frame_1box gSpriteBank15Frame030 = {
    SPRITE_FRAME(gSpriteBank15Frame030, SPRITE_TILES_BANK15_FRAME030),
    { { -10, -29, 16, 59 } },
};
const struct sprite_frame_1box gSpriteBank15Frame031 = {
    SPRITE_FRAME(gSpriteBank15Frame031, SPRITE_TILES_BANK15_FRAME031),
    { { -10, -29, 16, 59 } },
};
const struct sprite_frame_1box gSpriteBank15Frame032 = {
    SPRITE_FRAME(gSpriteBank15Frame032, SPRITE_TILES_BANK15_FRAME032),
    { { -10, -29, 16, 59 } },
};
const struct sprite_frame_1box gSpriteBank15Frame033 = {
    SPRITE_FRAME(gSpriteBank15Frame033, SPRITE_TILES_BANK15_FRAME033),
    { { -10, -29, 16, 59 } },
};
const struct sprite_frame_1box gSpriteBank15Frame034 = {
    SPRITE_FRAME(gSpriteBank15Frame034, SPRITE_TILES_BANK15_FRAME034),
    { { -10, -29, 16, 59 } },
};
const struct sprite_frame_1box gSpriteBank15Frame035 = {
    SPRITE_FRAME(gSpriteBank15Frame035, SPRITE_TILES_BANK15_FRAME035),
    { { -10, -29, 16, 59 } },
};
const struct sprite_frame_1box gSpriteBank15Frame036 = {
    SPRITE_FRAME(gSpriteBank15Frame036, SPRITE_TILES_BANK15_FRAME036),
    { { -10, -29, 16, 59 } },
};
const struct sprite_frame_1box gSpriteBank15Frame037 = {
    SPRITE_FRAME(gSpriteBank15Frame037, SPRITE_TILES_BANK15_FRAME037),
    { { -10, -29, 16, 59 } },
};
const struct sprite_frame_1box gSpriteBank15Frame038 = {
    SPRITE_FRAME(gSpriteBank15Frame038, SPRITE_TILES_BANK15_FRAME038),
    { { -10, -29, 16, 59 } },
};
const struct sprite_frame_1box gSpriteBank15Frame039 = {
    SPRITE_FRAME(gSpriteBank15Frame039, SPRITE_TILES_BANK15_FRAME039),
    { { -10, -29, 16, 59 } },
};
const struct sprite_frame_1box gSpriteBank15Frame040 = {
    SPRITE_FRAME(gSpriteBank15Frame040, SPRITE_TILES_BANK15_FRAME040),
    { { -10, -29, 16, 59 } },
};
const struct sprite_frame_1box gSpriteBank15Frame041 = {
    SPRITE_FRAME(gSpriteBank15Frame041, SPRITE_TILES_BANK15_FRAME041),
    { { -10, -29, 16, 59 } },
};
const struct sprite_frame_1box gSpriteBank15Frame042 = {
    SPRITE_FRAME(gSpriteBank15Frame042, SPRITE_TILES_BANK15_FRAME042),
    { { -10, -29, 16, 59 } },
};
const struct sprite_frame_1box gSpriteBank15Frame043 = {
    SPRITE_FRAME(gSpriteBank15Frame043, SPRITE_TILES_BANK15_FRAME043),
    { { -10, -29, 16, 59 } },
};
const struct sprite_frame_1box gSpriteBank15Frame044 = {
    SPRITE_FRAME(gSpriteBank15Frame044, SPRITE_TILES_BANK15_FRAME044),
    { { -10, -29, 16, 59 } },
};
const struct sprite_frame_1box gSpriteBank15Frame045 = {
    SPRITE_FRAME(gSpriteBank15Frame045, SPRITE_TILES_BANK15_FRAME045),
    { { -10, -29, 16, 59 } },
};
const struct sprite_frame_1box gSpriteBank15Frame046 = {
    SPRITE_FRAME(gSpriteBank15Frame046, SPRITE_TILES_BANK15_FRAME046),
    { { -10, -29, 16, 59 } },
};
const struct sprite_frame_1box gSpriteBank15Frame047 = {
    SPRITE_FRAME(gSpriteBank15Frame047, SPRITE_TILES_BANK15_FRAME047),
    { { -10, -29, 16, 59 } },
};
const struct sprite_frame_1box gSpriteBank15Frame048 = {
    SPRITE_FRAME(gSpriteBank15Frame048, SPRITE_TILES_BANK15_FRAME048),
    { { -10, -29, 16, 59 } },
};
const struct sprite_frame_1box gSpriteBank15Frame049 = {
    SPRITE_FRAME(gSpriteBank15Frame049, SPRITE_TILES_BANK15_FRAME049),
    { { -10, -29, 16, 59 } },
};
const struct sprite_frame_1box gSpriteBank15Frame050 = {
    SPRITE_FRAME(gSpriteBank15Frame050, SPRITE_TILES_BANK15_FRAME050),
    { { -10, -29, 16, 59 } },
};
const struct sprite_frame_1box gSpriteBank15Frame051 = {
    SPRITE_FRAME(gSpriteBank15Frame051, SPRITE_TILES_BANK15_FRAME051),
    { { -10, -29, 16, 59 } },
};
const struct sprite_frame_1box gSpriteBank15Frame052 = {
    SPRITE_FRAME(gSpriteBank15Frame052, SPRITE_TILES_BANK15_FRAME052),
    { { -10, -29, 16, 59 } },
};
const struct sprite_frame_1box gSpriteBank15Frame053 = {
    SPRITE_FRAME(gSpriteBank15Frame053, SPRITE_TILES_BANK15_FRAME053),
    { { -10, -29, 16, 59 } },
};
const struct sprite_frame_1box gSpriteBank15Frame054 = {
    SPRITE_FRAME(gSpriteBank15Frame054, SPRITE_TILES_BANK15_FRAME054),
    { { -10, -29, 16, 59 } },
};
const struct sprite_frame_1box gSpriteBank15Frame055 = {
    SPRITE_FRAME(gSpriteBank15Frame055, SPRITE_TILES_BANK15_FRAME055),
    { { -10, -29, 16, 59 } },
};

/* The piece arrays, from games/n2/graphics/sprites/bank15_mummy/bank.json. */
#include "sprites/bank15_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 16: 1 animation, 15 frames, games/n2/graphics/sprites/bank16_crocodile/. */

extern const u16 gSpriteBank16Anim00Seq[15];

extern const struct sprite_anim gSpriteBank16Anims[1] = {
    /* 0 */ {
        /* seq */ gSpriteBank16Anim00Seq,
        /* box */ { { -15, -25, 31, 50 }, { -20, -41, 41, 77 } },
        /* paletteId */ 21,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank16Anim00Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank16Anim00Seq[15] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
};

extern const struct sprite_frame *const gSpriteBank16Frames[15] = {
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
};

const struct sprite_frame_1box gSpriteBank16Frame000 = {
    SPRITE_FRAME(gSpriteBank16Frame000, SPRITE_TILES_BANK16_FRAME000),
    { { -12, -23, 21, 45 } },
};
const struct sprite_frame_1box gSpriteBank16Frame001 = {
    SPRITE_FRAME(gSpriteBank16Frame001, SPRITE_TILES_BANK16_FRAME001),
    { { -15, -25, 26, 45 } },
};
const struct sprite_frame_1box gSpriteBank16Frame002 = {
    SPRITE_FRAME(gSpriteBank16Frame002, SPRITE_TILES_BANK16_FRAME002),
    { { -13, -26, 25, 46 } },
};
const struct sprite_frame_1box gSpriteBank16Frame003 = {
    SPRITE_FRAME(gSpriteBank16Frame003, SPRITE_TILES_BANK16_FRAME003),
    { { -12, -26, 25, 48 } },
};
const struct sprite_frame_1box gSpriteBank16Frame004 = {
    SPRITE_FRAME(gSpriteBank16Frame004, SPRITE_TILES_BANK16_FRAME004),
    { { -9, -27, 21, 48 } },
};
const struct sprite_frame_1box gSpriteBank16Frame005 = {
    SPRITE_FRAME(gSpriteBank16Frame005, SPRITE_TILES_BANK16_FRAME005),
    { { -11, -25, 23, 48 } },
};
const struct sprite_frame_1box gSpriteBank16Frame006 = {
    SPRITE_FRAME(gSpriteBank16Frame006, SPRITE_TILES_BANK16_FRAME006),
    { { -8, -22, 16, 39 } },
};
const struct sprite_frame_1box gSpriteBank16Frame007 = {
    SPRITE_FRAME(gSpriteBank16Frame007, SPRITE_TILES_BANK16_FRAME007),
    { { -11, -24, 26, 37 } },
};
const struct sprite_frame_1box gSpriteBank16Frame008 = {
    SPRITE_FRAME(gSpriteBank16Frame008, SPRITE_TILES_BANK16_FRAME008),
    { { -12, -22, 28, 35 } },
};
const struct sprite_frame_1box gSpriteBank16Frame009 = {
    SPRITE_FRAME(gSpriteBank16Frame009, SPRITE_TILES_BANK16_FRAME009),
    { { -14, -12, 32, 25 } },
};
const struct sprite_frame_1box gSpriteBank16Frame010 = {
    SPRITE_FRAME(gSpriteBank16Frame010, SPRITE_TILES_BANK16_FRAME010),
    { { -12, -12, 27, 31 } },
};
const struct sprite_frame_1box gSpriteBank16Frame011 = {
    SPRITE_FRAME(gSpriteBank16Frame011, SPRITE_TILES_BANK16_FRAME011),
    { { -16, -23, 29, 50 } },
};
const struct sprite_frame_1box gSpriteBank16Frame012 = {
    SPRITE_FRAME(gSpriteBank16Frame012, SPRITE_TILES_BANK16_FRAME012),
    { { -13, -26, 21, 59 } },
};
const struct sprite_frame_1box gSpriteBank16Frame013 = {
    SPRITE_FRAME(gSpriteBank16Frame013, SPRITE_TILES_BANK16_FRAME013),
    { { -17, -30, 26, 64 } },
};
const struct sprite_frame_1box gSpriteBank16Frame014 = {
    SPRITE_FRAME(gSpriteBank16Frame014, SPRITE_TILES_BANK16_FRAME014),
    { { -17, -26, 25, 57 } },
};

/* The piece arrays, from games/n2/graphics/sprites/bank16_crocodile/bank.json. */
#include "sprites/bank16_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 17: 2 animations, 44 frames, games/n2/graphics/sprites/bank17_cobra_basket/. */

extern const u16 gSpriteBank17Anim00Seq[19];
extern const u16 gSpriteBank17Anim01Seq[26];

extern const struct sprite_anim gSpriteBank17Anims[2] = {
    /* 0 */ {
        /* seq */ gSpriteBank17Anim00Seq,
        /* box */ { { -16, -22, 32, 44 }, { -18, -24, 59, 46 } },
        /* paletteId */ 22,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank17Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank17Anim01Seq,
        /* box */ { { -16, -22, 32, 44 }, { -16, -24, 32, 46 } },
        /* paletteId */ 22,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank17Anim01Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank17Anim00Seq[19] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    15, 16, 17,
};
const u16 gSpriteBank17Anim01Seq[26] = {
    18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
    34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
};

extern const struct sprite_frame *const gSpriteBank17Frames[44] = {
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
    &gSpriteBank17Frame015.frame,
    &gSpriteBank17Frame016.frame,
    &gSpriteBank17Frame017.frame,
    &gSpriteBank17Frame018.frame,
    &gSpriteBank17Frame019.frame,
    &gSpriteBank17Frame020.frame,
    &gSpriteBank17Frame021.frame,
    &gSpriteBank17Frame022.frame,
    &gSpriteBank17Frame023.frame,
    &gSpriteBank17Frame024.frame,
    &gSpriteBank17Frame025.frame,
    &gSpriteBank17Frame026.frame,
    &gSpriteBank17Frame027.frame,
    &gSpriteBank17Frame028.frame,
    &gSpriteBank17Frame029.frame,
    &gSpriteBank17Frame030.frame,
    &gSpriteBank17Frame031.frame,
    &gSpriteBank17Frame032,
    &gSpriteBank17Frame033,
    &gSpriteBank17Frame034,
    &gSpriteBank17Frame035,
    &gSpriteBank17Frame036,
    &gSpriteBank17Frame037,
    &gSpriteBank17Frame038,
    &gSpriteBank17Frame039,
    &gSpriteBank17Frame040,
    &gSpriteBank17Frame041,
    &gSpriteBank17Frame042,
    &gSpriteBank17Frame043,
};

const struct sprite_frame_1box gSpriteBank17Frame000 = {
    SPRITE_FRAME(gSpriteBank17Frame000, SPRITE_TILES_BANK17_FRAME000),
    { { -12, -22, 26, 44 } },
};
const struct sprite_frame_1box gSpriteBank17Frame001 = {
    SPRITE_FRAME(gSpriteBank17Frame001, SPRITE_TILES_BANK17_FRAME001),
    { { -12, -22, 26, 44 } },
};
const struct sprite_frame_1box gSpriteBank17Frame002 = {
    SPRITE_FRAME(gSpriteBank17Frame002, SPRITE_TILES_BANK17_FRAME002),
    { { -12, -22, 26, 44 } },
};
const struct sprite_frame_1box gSpriteBank17Frame003 = {
    SPRITE_FRAME(gSpriteBank17Frame003, SPRITE_TILES_BANK17_FRAME003),
    { { -12, -22, 26, 44 } },
};
const struct sprite_frame_1box gSpriteBank17Frame004 = {
    SPRITE_FRAME(gSpriteBank17Frame004, SPRITE_TILES_BANK17_FRAME004),
    { { -12, -22, 26, 44 } },
};
const struct sprite_frame_1box gSpriteBank17Frame005 = {
    SPRITE_FRAME(gSpriteBank17Frame005, SPRITE_TILES_BANK17_FRAME005),
    { { -12, -22, 26, 44 } },
};
const struct sprite_frame_2box gSpriteBank17Frame006 = {
    SPRITE_FRAME(gSpriteBank17Frame006, SPRITE_TILES_BANK17_FRAME006),
    { { -12, -22, 26, 44 }, { 21, -25, 9, 15 } },
};
const struct sprite_frame_2box gSpriteBank17Frame007 = {
    SPRITE_FRAME(gSpriteBank17Frame007, SPRITE_TILES_BANK17_FRAME007),
    { { -12, -22, 41, 44 }, { 31, -12, 10, 15 } },
};
const struct sprite_frame_2box gSpriteBank17Frame008 = {
    SPRITE_FRAME(gSpriteBank17Frame008, SPRITE_TILES_BANK17_FRAME008),
    { { -12, -22, 42, 44 }, { 32, -7, 11, 13 } },
};
const struct sprite_frame_1box gSpriteBank17Frame009 = {
    SPRITE_FRAME(gSpriteBank17Frame009, SPRITE_TILES_BANK17_FRAME009),
    { { -12, -22, 50, 44 } },
};
const struct sprite_frame_1box gSpriteBank17Frame010 = {
    SPRITE_FRAME(gSpriteBank17Frame010, SPRITE_TILES_BANK17_FRAME010),
    { { -12, -22, 43, 44 } },
};
const struct sprite_frame_1box gSpriteBank17Frame011 = {
    SPRITE_FRAME(gSpriteBank17Frame011, SPRITE_TILES_BANK17_FRAME011),
    { { -12, -22, 38, 44 } },
};
const struct sprite_frame_1box gSpriteBank17Frame012 = {
    SPRITE_FRAME(gSpriteBank17Frame012, SPRITE_TILES_BANK17_FRAME012),
    { { -12, -22, 34, 44 } },
};
const struct sprite_frame_1box gSpriteBank17Frame013 = {
    SPRITE_FRAME(gSpriteBank17Frame013, SPRITE_TILES_BANK17_FRAME013),
    { { -12, -22, 33, 44 } },
};
const struct sprite_frame_1box gSpriteBank17Frame014 = {
    SPRITE_FRAME(gSpriteBank17Frame014, SPRITE_TILES_BANK17_FRAME014),
    { { -12, -22, 28, 44 } },
};
const struct sprite_frame_1box gSpriteBank17Frame015 = {
    SPRITE_FRAME(gSpriteBank17Frame015, SPRITE_TILES_BANK17_FRAME015),
    { { -12, -22, 26, 44 } },
};
const struct sprite_frame_1box gSpriteBank17Frame016 = {
    SPRITE_FRAME(gSpriteBank17Frame016, SPRITE_TILES_BANK17_FRAME016),
    { { -12, -22, 26, 44 } },
};
const struct sprite_frame_1box gSpriteBank17Frame017 = {
    SPRITE_FRAME(gSpriteBank17Frame017, SPRITE_TILES_BANK17_FRAME017),
    { { -12, -22, 26, 44 } },
};
const struct sprite_frame_1box gSpriteBank17Frame018 = {
    SPRITE_FRAME(gSpriteBank17Frame018, SPRITE_TILES_BANK17_FRAME018),
    { { -13, -21, 27, 43 } },
};
const struct sprite_frame_1box gSpriteBank17Frame019 = {
    SPRITE_FRAME(gSpriteBank17Frame019, SPRITE_TILES_BANK17_FRAME019),
    { { -13, -21, 27, 43 } },
};
const struct sprite_frame_1box gSpriteBank17Frame020 = {
    SPRITE_FRAME(gSpriteBank17Frame020, SPRITE_TILES_BANK17_FRAME020),
    { { -13, -21, 27, 43 } },
};
const struct sprite_frame_1box gSpriteBank17Frame021 = {
    SPRITE_FRAME(gSpriteBank17Frame021, SPRITE_TILES_BANK17_FRAME021),
    { { -13, -21, 27, 43 } },
};
const struct sprite_frame_1box gSpriteBank17Frame022 = {
    SPRITE_FRAME(gSpriteBank17Frame022, SPRITE_TILES_BANK17_FRAME022),
    { { -13, -21, 27, 43 } },
};
const struct sprite_frame_1box gSpriteBank17Frame023 = {
    SPRITE_FRAME(gSpriteBank17Frame023, SPRITE_TILES_BANK17_FRAME023),
    { { -13, -21, 27, 43 } },
};
const struct sprite_frame_1box gSpriteBank17Frame024 = {
    SPRITE_FRAME(gSpriteBank17Frame024, SPRITE_TILES_BANK17_FRAME024),
    { { -13, -21, 27, 43 } },
};
const struct sprite_frame_1box gSpriteBank17Frame025 = {
    SPRITE_FRAME(gSpriteBank17Frame025, SPRITE_TILES_BANK17_FRAME025),
    { { -13, -21, 27, 43 } },
};
const struct sprite_frame_1box gSpriteBank17Frame026 = {
    SPRITE_FRAME(gSpriteBank17Frame026, SPRITE_TILES_BANK17_FRAME026),
    { { -13, -21, 27, 43 } },
};
const struct sprite_frame_1box gSpriteBank17Frame027 = {
    SPRITE_FRAME(gSpriteBank17Frame027, SPRITE_TILES_BANK17_FRAME027),
    { { -13, -21, 27, 43 } },
};
const struct sprite_frame_1box gSpriteBank17Frame028 = {
    SPRITE_FRAME(gSpriteBank17Frame028, SPRITE_TILES_BANK17_FRAME028),
    { { -13, -21, 27, 43 } },
};
const struct sprite_frame_1box gSpriteBank17Frame029 = {
    SPRITE_FRAME(gSpriteBank17Frame029, SPRITE_TILES_BANK17_FRAME029),
    { { -13, -21, 27, 43 } },
};
const struct sprite_frame_1box gSpriteBank17Frame030 = {
    SPRITE_FRAME(gSpriteBank17Frame030, SPRITE_TILES_BANK17_FRAME030),
    { { -13, -21, 27, 43 } },
};
const struct sprite_frame_1box gSpriteBank17Frame031 = {
    SPRITE_FRAME(gSpriteBank17Frame031, SPRITE_TILES_BANK17_FRAME031),
    { { -13, -21, 27, 43 } },
};
const struct sprite_frame gSpriteBank17Frame032 = SPRITE_FRAME(gSpriteBank17Frame032, SPRITE_TILES_BANK17_FRAME032);
const struct sprite_frame gSpriteBank17Frame033 = SPRITE_FRAME(gSpriteBank17Frame033, SPRITE_TILES_BANK17_FRAME033);
const struct sprite_frame gSpriteBank17Frame034 = SPRITE_FRAME(gSpriteBank17Frame034, SPRITE_TILES_BANK17_FRAME034);
const struct sprite_frame gSpriteBank17Frame035 = SPRITE_FRAME(gSpriteBank17Frame035, SPRITE_TILES_BANK17_FRAME035);
const struct sprite_frame gSpriteBank17Frame036 = SPRITE_FRAME(gSpriteBank17Frame036, SPRITE_TILES_BANK17_FRAME036);
const struct sprite_frame gSpriteBank17Frame037 = SPRITE_FRAME(gSpriteBank17Frame037, SPRITE_TILES_BANK17_FRAME037);
const struct sprite_frame gSpriteBank17Frame038 = SPRITE_FRAME(gSpriteBank17Frame038, SPRITE_TILES_BANK17_FRAME038);
const struct sprite_frame gSpriteBank17Frame039 = SPRITE_FRAME(gSpriteBank17Frame039, SPRITE_TILES_BANK17_FRAME039);
const struct sprite_frame gSpriteBank17Frame040 = SPRITE_FRAME(gSpriteBank17Frame040, SPRITE_TILES_BANK17_FRAME040);
const struct sprite_frame gSpriteBank17Frame041 = SPRITE_FRAME(gSpriteBank17Frame041, SPRITE_TILES_BANK17_FRAME041);
const struct sprite_frame gSpriteBank17Frame042 = SPRITE_FRAME(gSpriteBank17Frame042, SPRITE_TILES_BANK17_FRAME042);
const struct sprite_frame gSpriteBank17Frame043 = SPRITE_FRAME(gSpriteBank17Frame043, SPRITE_TILES_BANK17_FRAME043);

/* The piece arrays, from games/n2/graphics/sprites/bank17_cobra_basket/bank.json. */
#include "sprites/bank17_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 18: 4 animations, 33 frames, games/n2/graphics/sprites/bank18_spear_tribesman/. */

extern const u16 gSpriteBank18Anim00Seq[17];
extern const u16 gSpriteBank18Anim01Seq[6];
extern const u16 gSpriteBank18Anim02Seq[1];
extern const u16 gSpriteBank18Anim03Seq[9];

extern const struct sprite_anim gSpriteBank18Anims[4] = {
    /* 0 */ {
        /* seq */ gSpriteBank18Anim00Seq,
        /* box */ { { -13, -32, 27, 65 }, { -38, -36, 78, 69 } },
        /* paletteId */ 23,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank18Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank18Anim01Seq,
        /* box */ { { -13, -32, 27, 65 }, { -13, -32, 27, 65 } },
        /* paletteId */ 23,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank18Anim01Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 2 */ {
        /* seq */ gSpriteBank18Anim02Seq,
        /* box */ { { -26, -3, 53, 7 }, { -26, -3, 53, 7 } },
        /* paletteId */ 23,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank18Anim02Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 3 */ {
        /* seq */ gSpriteBank18Anim03Seq,
        /* box */ { { -13, -32, 27, 65 }, { -13, -33, 53, 66 } },
        /* paletteId */ 23,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank18Anim03Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank18Anim00Seq[17] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16,
};
const u16 gSpriteBank18Anim01Seq[6] = {
    17, 18, 19, 20, 21, 22,
};
const u16 gSpriteBank18Anim02Seq[1] = {
    23,
};
const u16 gSpriteBank18Anim03Seq[9] = {
    24, 25, 26, 27, 28, 29, 30, 31, 32,
};

extern const struct sprite_frame *const gSpriteBank18Frames[33] = {
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
    &gSpriteBank18Frame014.frame,
    &gSpriteBank18Frame015.frame,
    &gSpriteBank18Frame016.frame,
    &gSpriteBank18Frame017.frame,
    &gSpriteBank18Frame018.frame,
    &gSpriteBank18Frame019.frame,
    &gSpriteBank18Frame020.frame,
    &gSpriteBank18Frame021.frame,
    &gSpriteBank18Frame022.frame,
    &gSpriteBank18Frame023.frame,
    &gSpriteBank18Frame024.frame,
    &gSpriteBank18Frame025.frame,
    &gSpriteBank18Frame026.frame,
    &gSpriteBank18Frame027.frame,
    &gSpriteBank18Frame028.frame,
    &gSpriteBank18Frame029.frame,
    &gSpriteBank18Frame030.frame,
    &gSpriteBank18Frame031.frame,
    &gSpriteBank18Frame032.frame,
};

const struct sprite_frame_1box gSpriteBank18Frame000 = {
    SPRITE_FRAME(gSpriteBank18Frame000, SPRITE_TILES_BANK18_FRAME000),
    { { -5, -25, 13, 57 } },
};
const struct sprite_frame_1box gSpriteBank18Frame001 = {
    SPRITE_FRAME(gSpriteBank18Frame001, SPRITE_TILES_BANK18_FRAME001),
    { { -5, -25, 13, 57 } },
};
const struct sprite_frame_1box gSpriteBank18Frame002 = {
    SPRITE_FRAME(gSpriteBank18Frame002, SPRITE_TILES_BANK18_FRAME002),
    { { -5, -25, 13, 57 } },
};
const struct sprite_frame_1box gSpriteBank18Frame003 = {
    SPRITE_FRAME(gSpriteBank18Frame003, SPRITE_TILES_BANK18_FRAME003),
    { { -5, -25, 13, 57 } },
};
const struct sprite_frame_1box gSpriteBank18Frame004 = {
    SPRITE_FRAME(gSpriteBank18Frame004, SPRITE_TILES_BANK18_FRAME004),
    { { 1, -15, 16, 45 } },
};
const struct sprite_frame_1box gSpriteBank18Frame005 = {
    SPRITE_FRAME(gSpriteBank18Frame005, SPRITE_TILES_BANK18_FRAME005),
    { { 2, -16, 18, 45 } },
};
const struct sprite_frame_1box gSpriteBank18Frame006 = {
    SPRITE_FRAME(gSpriteBank18Frame006, SPRITE_TILES_BANK18_FRAME006),
    { { -5, -25, 13, 57 } },
};
const struct sprite_frame_1box gSpriteBank18Frame007 = {
    SPRITE_FRAME(gSpriteBank18Frame007, SPRITE_TILES_BANK18_FRAME007),
    { { -5, -25, 13, 57 } },
};
const struct sprite_frame_1box gSpriteBank18Frame008 = {
    SPRITE_FRAME(gSpriteBank18Frame008, SPRITE_TILES_BANK18_FRAME008),
    { { -5, -25, 13, 57 } },
};
const struct sprite_frame_1box gSpriteBank18Frame009 = {
    SPRITE_FRAME(gSpriteBank18Frame009, SPRITE_TILES_BANK18_FRAME009),
    { { -5, -25, 13, 57 } },
};
const struct sprite_frame_1box gSpriteBank18Frame010 = {
    SPRITE_FRAME(gSpriteBank18Frame010, SPRITE_TILES_BANK18_FRAME010),
    { { -16, -13, 22, 44 } },
};
const struct sprite_frame_1box gSpriteBank18Frame011 = {
    SPRITE_FRAME(gSpriteBank18Frame011, SPRITE_TILES_BANK18_FRAME011),
    { { -14, -10, 21, 40 } },
};
const struct sprite_frame_1box gSpriteBank18Frame012 = {
    SPRITE_FRAME(gSpriteBank18Frame012, SPRITE_TILES_BANK18_FRAME012),
    { { -5, -25, 13, 57 } },
};
const struct sprite_frame_1box gSpriteBank18Frame013 = {
    SPRITE_FRAME(gSpriteBank18Frame013, SPRITE_TILES_BANK18_FRAME013),
    { { -5, -25, 13, 57 } },
};
const struct sprite_frame_1box gSpriteBank18Frame014 = {
    SPRITE_FRAME(gSpriteBank18Frame014, SPRITE_TILES_BANK18_FRAME014),
    { { -5, -25, 13, 57 } },
};
const struct sprite_frame_1box gSpriteBank18Frame015 = {
    SPRITE_FRAME(gSpriteBank18Frame015, SPRITE_TILES_BANK18_FRAME015),
    { { -5, -25, 13, 57 } },
};
const struct sprite_frame_1box gSpriteBank18Frame016 = {
    SPRITE_FRAME(gSpriteBank18Frame016, SPRITE_TILES_BANK18_FRAME016),
    { { 6, -12, 16, 40 } },
};
const struct sprite_frame_1box gSpriteBank18Frame017 = {
    SPRITE_FRAME(gSpriteBank18Frame017, SPRITE_TILES_BANK18_FRAME017),
    { { -5, -23, 14, 56 } },
};
const struct sprite_frame_1box gSpriteBank18Frame018 = {
    SPRITE_FRAME(gSpriteBank18Frame018, SPRITE_TILES_BANK18_FRAME018),
    { { -5, -23, 14, 56 } },
};
const struct sprite_frame_1box gSpriteBank18Frame019 = {
    SPRITE_FRAME(gSpriteBank18Frame019, SPRITE_TILES_BANK18_FRAME019),
    { { -5, -23, 14, 56 } },
};
const struct sprite_frame_1box gSpriteBank18Frame020 = {
    SPRITE_FRAME(gSpriteBank18Frame020, SPRITE_TILES_BANK18_FRAME020),
    { { -5, -23, 14, 56 } },
};
const struct sprite_frame_1box gSpriteBank18Frame021 = {
    SPRITE_FRAME(gSpriteBank18Frame021, SPRITE_TILES_BANK18_FRAME021),
    { { -5, -23, 14, 56 } },
};
const struct sprite_frame_1box gSpriteBank18Frame022 = {
    SPRITE_FRAME(gSpriteBank18Frame022, SPRITE_TILES_BANK18_FRAME022),
    { { -5, -23, 14, 56 } },
};
const struct sprite_frame_3box_anchor gSpriteBank18Frame023 = {
    SPRITE_FRAME(gSpriteBank18Frame023, SPRITE_TILES_BANK18_FRAME023),
    { { 0, 0, 0, 0 }, { 16, -3, 12, 7 }, { -26, -2, 45, 6 } },
    { 0, 0 },
};
const struct sprite_frame_1box gSpriteBank18Frame024 = {
    SPRITE_FRAME(gSpriteBank18Frame024, SPRITE_TILES_BANK18_FRAME024),
    { { 8, -15, 19, 46 } },
};
const struct sprite_frame_1box gSpriteBank18Frame025 = {
    SPRITE_FRAME(gSpriteBank18Frame025, SPRITE_TILES_BANK18_FRAME025),
    { { 12, -13, 15, 43 } },
};
const struct sprite_frame_1box gSpriteBank18Frame026 = {
    SPRITE_FRAME(gSpriteBank18Frame026, SPRITE_TILES_BANK18_FRAME026),
    { { 10, -13, 14, 43 } },
};
const struct sprite_frame_1box gSpriteBank18Frame027 = {
    SPRITE_FRAME(gSpriteBank18Frame027, SPRITE_TILES_BANK18_FRAME027),
    { { 7, -12, 16, 42 } },
};
const struct sprite_frame_1box gSpriteBank18Frame028 = {
    SPRITE_FRAME(gSpriteBank18Frame028, SPRITE_TILES_BANK18_FRAME028),
    { { 5, -19, 12, 51 } },
};
const struct sprite_frame_1box gSpriteBank18Frame029 = {
    SPRITE_FRAME(gSpriteBank18Frame029, SPRITE_TILES_BANK18_FRAME029),
    { { 4, -21, 11, 54 } },
};
const struct sprite_frame_1box gSpriteBank18Frame030 = {
    SPRITE_FRAME(gSpriteBank18Frame030, SPRITE_TILES_BANK18_FRAME030),
    { { -1, -20, 12, 49 } },
};
const struct sprite_frame_1box gSpriteBank18Frame031 = {
    SPRITE_FRAME(gSpriteBank18Frame031, SPRITE_TILES_BANK18_FRAME031),
    { { -2, -19, 13, 50 } },
};
const struct sprite_frame_1box gSpriteBank18Frame032 = {
    SPRITE_FRAME(gSpriteBank18Frame032, SPRITE_TILES_BANK18_FRAME032),
    { { -3, -18, 13, 48 } },
};

/* The piece arrays, from games/n2/graphics/sprites/bank18_spear_tribesman/bank.json. */
#include "sprites/bank18_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 19: 3 animations, 47 frames, games/n2/graphics/sprites/bank19_parrot/. */

extern const u16 gSpriteBank19Anim00Seq[23];
extern const u16 gSpriteBank19Anim01Seq[12];
extern const u16 gSpriteBank19Anim02Seq[12];

extern const struct sprite_anim gSpriteBank19Anims[3] = {
    /* 0 */ {
        /* seq */ gSpriteBank19Anim00Seq,
        /* box */ { { -12, 1, 26, 23 }, { -23, -21, 47, 45 } },
        /* paletteId */ 24,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank19Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank19Anim01Seq,
        /* box */ { { -12, 1, 26, 23 }, { -15, -22, 31, 46 } },
        /* paletteId */ 24,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank19Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank19Anim02Seq,
        /* box */ { { -12, 1, 26, 23 }, { -31, -17, 63, 33 } },
        /* paletteId */ 24,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank19Anim02Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank19Anim00Seq[23] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22,
};
const u16 gSpriteBank19Anim01Seq[12] = {
    23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34,
};
const u16 gSpriteBank19Anim02Seq[12] = {
    35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46,
};

extern const struct sprite_frame *const gSpriteBank19Frames[47] = {
    &gSpriteBank19Frame000,
    &gSpriteBank19Frame001,
    &gSpriteBank19Frame002,
    &gSpriteBank19Frame003,
    &gSpriteBank19Frame004,
    &gSpriteBank19Frame005,
    &gSpriteBank19Frame006,
    &gSpriteBank19Frame007,
    &gSpriteBank19Frame008,
    &gSpriteBank19Frame009,
    &gSpriteBank19Frame010,
    &gSpriteBank19Frame011,
    &gSpriteBank19Frame012,
    &gSpriteBank19Frame013,
    &gSpriteBank19Frame014,
    &gSpriteBank19Frame015,
    &gSpriteBank19Frame016,
    &gSpriteBank19Frame017,
    &gSpriteBank19Frame018,
    &gSpriteBank19Frame019,
    &gSpriteBank19Frame020,
    &gSpriteBank19Frame021,
    &gSpriteBank19Frame022,
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
    &gSpriteBank19Frame035.frame,
    &gSpriteBank19Frame036.frame,
    &gSpriteBank19Frame037.frame,
    &gSpriteBank19Frame038.frame,
    &gSpriteBank19Frame039.frame,
    &gSpriteBank19Frame040.frame,
    &gSpriteBank19Frame041.frame,
    &gSpriteBank19Frame042.frame,
    &gSpriteBank19Frame043.frame,
    &gSpriteBank19Frame044.frame,
    &gSpriteBank19Frame045.frame,
    &gSpriteBank19Frame046.frame,
};

const struct sprite_frame gSpriteBank19Frame000 = SPRITE_FRAME(gSpriteBank19Frame000, SPRITE_TILES_BANK19_FRAME000);
const struct sprite_frame gSpriteBank19Frame001 = SPRITE_FRAME(gSpriteBank19Frame001, SPRITE_TILES_BANK19_FRAME001);
const struct sprite_frame gSpriteBank19Frame002 = SPRITE_FRAME(gSpriteBank19Frame002, SPRITE_TILES_BANK19_FRAME002);
const struct sprite_frame gSpriteBank19Frame003 = SPRITE_FRAME(gSpriteBank19Frame003, SPRITE_TILES_BANK19_FRAME003);
const struct sprite_frame gSpriteBank19Frame004 = SPRITE_FRAME(gSpriteBank19Frame004, SPRITE_TILES_BANK19_FRAME004);
const struct sprite_frame gSpriteBank19Frame005 = SPRITE_FRAME(gSpriteBank19Frame005, SPRITE_TILES_BANK19_FRAME005);
const struct sprite_frame gSpriteBank19Frame006 = SPRITE_FRAME(gSpriteBank19Frame006, SPRITE_TILES_BANK19_FRAME006);
const struct sprite_frame gSpriteBank19Frame007 = SPRITE_FRAME(gSpriteBank19Frame007, SPRITE_TILES_BANK19_FRAME007);
const struct sprite_frame gSpriteBank19Frame008 = SPRITE_FRAME(gSpriteBank19Frame008, SPRITE_TILES_BANK19_FRAME008);
const struct sprite_frame gSpriteBank19Frame009 = SPRITE_FRAME(gSpriteBank19Frame009, SPRITE_TILES_BANK19_FRAME009);
const struct sprite_frame gSpriteBank19Frame010 = SPRITE_FRAME(gSpriteBank19Frame010, SPRITE_TILES_BANK19_FRAME010);
const struct sprite_frame gSpriteBank19Frame011 = SPRITE_FRAME(gSpriteBank19Frame011, SPRITE_TILES_BANK19_FRAME011);
const struct sprite_frame gSpriteBank19Frame012 = SPRITE_FRAME(gSpriteBank19Frame012, SPRITE_TILES_BANK19_FRAME012);
const struct sprite_frame gSpriteBank19Frame013 = SPRITE_FRAME(gSpriteBank19Frame013, SPRITE_TILES_BANK19_FRAME013);
const struct sprite_frame gSpriteBank19Frame014 = SPRITE_FRAME(gSpriteBank19Frame014, SPRITE_TILES_BANK19_FRAME014);
const struct sprite_frame gSpriteBank19Frame015 = SPRITE_FRAME(gSpriteBank19Frame015, SPRITE_TILES_BANK19_FRAME015);
const struct sprite_frame gSpriteBank19Frame016 = SPRITE_FRAME(gSpriteBank19Frame016, SPRITE_TILES_BANK19_FRAME016);
const struct sprite_frame gSpriteBank19Frame017 = SPRITE_FRAME(gSpriteBank19Frame017, SPRITE_TILES_BANK19_FRAME017);
const struct sprite_frame gSpriteBank19Frame018 = SPRITE_FRAME(gSpriteBank19Frame018, SPRITE_TILES_BANK19_FRAME018);
const struct sprite_frame gSpriteBank19Frame019 = SPRITE_FRAME(gSpriteBank19Frame019, SPRITE_TILES_BANK19_FRAME019);
const struct sprite_frame gSpriteBank19Frame020 = SPRITE_FRAME(gSpriteBank19Frame020, SPRITE_TILES_BANK19_FRAME020);
const struct sprite_frame gSpriteBank19Frame021 = SPRITE_FRAME(gSpriteBank19Frame021, SPRITE_TILES_BANK19_FRAME021);
const struct sprite_frame gSpriteBank19Frame022 = SPRITE_FRAME(gSpriteBank19Frame022, SPRITE_TILES_BANK19_FRAME022);
const struct sprite_frame_1box gSpriteBank19Frame023 = {
    SPRITE_FRAME(gSpriteBank19Frame023, SPRITE_TILES_BANK19_FRAME023),
    { { -14, 2, 26, 22 } },
};
const struct sprite_frame_1box gSpriteBank19Frame024 = {
    SPRITE_FRAME(gSpriteBank19Frame024, SPRITE_TILES_BANK19_FRAME024),
    { { -14, -7, 27, 21 } },
};
const struct sprite_frame_1box gSpriteBank19Frame025 = {
    SPRITE_FRAME(gSpriteBank19Frame025, SPRITE_TILES_BANK19_FRAME025),
    { { -12, -16, 27, 22 } },
};
const struct sprite_frame_1box gSpriteBank19Frame026 = {
    SPRITE_FRAME(gSpriteBank19Frame026, SPRITE_TILES_BANK19_FRAME026),
    { { -9, -22, 25, 26 } },
};
const struct sprite_frame_2box gSpriteBank19Frame027 = {
    SPRITE_FRAME(gSpriteBank19Frame027, SPRITE_TILES_BANK19_FRAME027),
    { { -5, -20, 20, 20 }, { -3, 1, 15, 8 } },
};
const struct sprite_frame_2box gSpriteBank19Frame028 = {
    SPRITE_FRAME(gSpriteBank19Frame028, SPRITE_TILES_BANK19_FRAME028),
    { { -8, -19, 17, 22 }, { -6, 4, 13, 10 } },
};
const struct sprite_frame_2box gSpriteBank19Frame029 = {
    SPRITE_FRAME(gSpriteBank19Frame029, SPRITE_TILES_BANK19_FRAME029),
    { { -12, -15, 17, 20 }, { -11, 6, 12, 9 } },
};
const struct sprite_frame_2box gSpriteBank19Frame030 = {
    SPRITE_FRAME(gSpriteBank19Frame030, SPRITE_TILES_BANK19_FRAME030),
    { { -13, -13, 18, 16 }, { -10, 6, 13, 10 } },
};
const struct sprite_frame_2box gSpriteBank19Frame031 = {
    SPRITE_FRAME(gSpriteBank19Frame031, SPRITE_TILES_BANK19_FRAME031),
    { { -10, -12, 18, 16 }, { -7, 7, 12, 9 } },
};
const struct sprite_frame_2box gSpriteBank19Frame032 = {
    SPRITE_FRAME(gSpriteBank19Frame032, SPRITE_TILES_BANK19_FRAME032),
    { { -5, -12, 15, 19 }, { -4, 8, 12, 10 } },
};
const struct sprite_frame_2box gSpriteBank19Frame033 = {
    SPRITE_FRAME(gSpriteBank19Frame033, SPRITE_TILES_BANK19_FRAME033),
    { { -3, -9, 14, 18 }, { -3, 10, 12, 9 } },
};
const struct sprite_frame_2box gSpriteBank19Frame034 = {
    SPRITE_FRAME(gSpriteBank19Frame034, SPRITE_TILES_BANK19_FRAME034),
    { { -5, -9, 13, 19 }, { -2, 12, 8, 8 } },
};
const struct sprite_frame_1box gSpriteBank19Frame035 = {
    SPRITE_FRAME(gSpriteBank19Frame035, SPRITE_TILES_BANK19_FRAME035),
    { { -12, 1, 26, 23 } },
};
const struct sprite_frame_1box gSpriteBank19Frame036 = {
    SPRITE_FRAME(gSpriteBank19Frame036, SPRITE_TILES_BANK19_FRAME036),
    { { -12, 1, 26, 23 } },
};
const struct sprite_frame_1box gSpriteBank19Frame037 = {
    SPRITE_FRAME(gSpriteBank19Frame037, SPRITE_TILES_BANK19_FRAME037),
    { { -12, 1, 26, 23 } },
};
const struct sprite_frame_1box gSpriteBank19Frame038 = {
    SPRITE_FRAME(gSpriteBank19Frame038, SPRITE_TILES_BANK19_FRAME038),
    { { -12, 1, 26, 23 } },
};
const struct sprite_frame_1box gSpriteBank19Frame039 = {
    SPRITE_FRAME(gSpriteBank19Frame039, SPRITE_TILES_BANK19_FRAME039),
    { { -12, 1, 26, 23 } },
};
const struct sprite_frame_1box gSpriteBank19Frame040 = {
    SPRITE_FRAME(gSpriteBank19Frame040, SPRITE_TILES_BANK19_FRAME040),
    { { -12, 1, 26, 23 } },
};
const struct sprite_frame_1box gSpriteBank19Frame041 = {
    SPRITE_FRAME(gSpriteBank19Frame041, SPRITE_TILES_BANK19_FRAME041),
    { { -12, 1, 26, 23 } },
};
const struct sprite_frame_1box gSpriteBank19Frame042 = {
    SPRITE_FRAME(gSpriteBank19Frame042, SPRITE_TILES_BANK19_FRAME042),
    { { -12, 1, 26, 23 } },
};
const struct sprite_frame_1box gSpriteBank19Frame043 = {
    SPRITE_FRAME(gSpriteBank19Frame043, SPRITE_TILES_BANK19_FRAME043),
    { { -12, 1, 26, 23 } },
};
const struct sprite_frame_1box gSpriteBank19Frame044 = {
    SPRITE_FRAME(gSpriteBank19Frame044, SPRITE_TILES_BANK19_FRAME044),
    { { -12, 1, 26, 23 } },
};
const struct sprite_frame_1box gSpriteBank19Frame045 = {
    SPRITE_FRAME(gSpriteBank19Frame045, SPRITE_TILES_BANK19_FRAME045),
    { { -12, 1, 26, 23 } },
};
const struct sprite_frame_1box gSpriteBank19Frame046 = {
    SPRITE_FRAME(gSpriteBank19Frame046, SPRITE_TILES_BANK19_FRAME046),
    { { -12, 1, 26, 23 } },
};

/* The piece arrays, from games/n2/graphics/sprites/bank19_parrot/bank.json. */
#include "sprites/bank19_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 20: 5 animations, 61 frames, games/n2/graphics/sprites/bank20_fireball_tribesman/. */

extern const u16 gSpriteBank20Anim00Seq[7];
extern const u16 gSpriteBank20Anim01Seq[20];
extern const u16 gSpriteBank20Anim02Seq[20];
extern const u16 gSpriteBank20Anim03Seq[15];
extern const u16 gSpriteBank20Anim04Seq[15];

extern const struct sprite_anim gSpriteBank20Anims[5] = {
    /* 0 */ {
        /* seq */ gSpriteBank20Anim00Seq,
        /* box */ { { 12, -64, 32, 61 }, { 7, -63, 36, 60 } },
        /* paletteId */ 23,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank20Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank20Anim01Seq,
        /* box */ { { 12, -64, 32, 61 }, { 11, -65, 33, 62 } },
        /* paletteId */ 23,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank20Anim01Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 2 */ {
        /* seq */ gSpriteBank20Anim02Seq,
        /* box */ { { 12, -64, 32, 61 }, { 2, -62, 43, 59 } },
        /* paletteId */ 23,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank20Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank20Anim03Seq,
        /* box */ { { 12, -64, 32, 61 }, { 7, -54, 37, 51 } },
        /* paletteId */ 23,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank20Anim03Seq),
        /* flags */ 0,
    },
    /* 4 */ {
        /* seq */ gSpriteBank20Anim04Seq,
        /* box */ { { 12, -64, 32, 61 }, { 43, -50, 58, 21 } },
        /* paletteId */ 25,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank20Anim04Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank20Anim00Seq[7] = {
    0, 1, 2, 3, 4, 5, 6,
};
const u16 gSpriteBank20Anim01Seq[20] = {
    7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14,
    15, 15, 16, 16,
};
const u16 gSpriteBank20Anim02Seq[20] = {
    17, 17, 18, 18, 19, 19, 20, 20, 21, 22, 23, 23, 24, 25, 26, 26,
    27, 28, 29, 30,
};
const u16 gSpriteBank20Anim03Seq[15] = {
    31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45,
};
const u16 gSpriteBank20Anim04Seq[15] = {
    46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
};

extern const struct sprite_frame *const gSpriteBank20Frames[61] = {
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
    &gSpriteBank20Frame032.frame,
    &gSpriteBank20Frame033.frame,
    &gSpriteBank20Frame034.frame,
    &gSpriteBank20Frame035.frame,
    &gSpriteBank20Frame036.frame,
    &gSpriteBank20Frame037.frame,
    &gSpriteBank20Frame038.frame,
    &gSpriteBank20Frame039.frame,
    &gSpriteBank20Frame040.frame,
    &gSpriteBank20Frame041.frame,
    &gSpriteBank20Frame042.frame,
    &gSpriteBank20Frame043.frame,
    &gSpriteBank20Frame044.frame,
    &gSpriteBank20Frame045.frame,
    &gSpriteBank20Frame046.frame,
    &gSpriteBank20Frame047.frame,
    &gSpriteBank20Frame048.frame,
    &gSpriteBank20Frame049.frame,
    &gSpriteBank20Frame050.frame,
    &gSpriteBank20Frame051.frame,
    &gSpriteBank20Frame052.frame,
    &gSpriteBank20Frame053.frame,
    &gSpriteBank20Frame054.frame,
    &gSpriteBank20Frame055.frame,
    &gSpriteBank20Frame056.frame,
    &gSpriteBank20Frame057.frame,
    &gSpriteBank20Frame058.frame,
    &gSpriteBank20Frame059.frame,
    &gSpriteBank20Frame060.frame,
};

const struct sprite_frame_1box gSpriteBank20Frame000 = {
    SPRITE_FRAME(gSpriteBank20Frame000, SPRITE_TILES_BANK20_FRAME000),
    { { 16, -40, 22, 36 } },
};
const struct sprite_frame_1box gSpriteBank20Frame001 = {
    SPRITE_FRAME(gSpriteBank20Frame001, SPRITE_TILES_BANK20_FRAME001),
    { { 16, -40, 22, 36 } },
};
const struct sprite_frame_1box gSpriteBank20Frame002 = {
    SPRITE_FRAME(gSpriteBank20Frame002, SPRITE_TILES_BANK20_FRAME002),
    { { 16, -40, 22, 36 } },
};
const struct sprite_frame_1box gSpriteBank20Frame003 = {
    SPRITE_FRAME(gSpriteBank20Frame003, SPRITE_TILES_BANK20_FRAME003),
    { { 16, -40, 22, 36 } },
};
const struct sprite_frame_1box gSpriteBank20Frame004 = {
    SPRITE_FRAME(gSpriteBank20Frame004, SPRITE_TILES_BANK20_FRAME004),
    { { 19, -47, 18, 44 } },
};
const struct sprite_frame_1box gSpriteBank20Frame005 = {
    SPRITE_FRAME(gSpriteBank20Frame005, SPRITE_TILES_BANK20_FRAME005),
    { { 15, -48, 18, 42 } },
};
const struct sprite_frame_1box gSpriteBank20Frame006 = {
    SPRITE_FRAME(gSpriteBank20Frame006, SPRITE_TILES_BANK20_FRAME006),
    { { 13, -51, 17, 47 } },
};
const struct sprite_frame_1box gSpriteBank20Frame007 = {
    SPRITE_FRAME(gSpriteBank20Frame007, SPRITE_TILES_BANK20_FRAME007),
    { { 21, -52, 15, 47 } },
};
const struct sprite_frame_1box gSpriteBank20Frame008 = {
    SPRITE_FRAME(gSpriteBank20Frame008, SPRITE_TILES_BANK20_FRAME008),
    { { 21, -52, 15, 47 } },
};
const struct sprite_frame_1box gSpriteBank20Frame009 = {
    SPRITE_FRAME(gSpriteBank20Frame009, SPRITE_TILES_BANK20_FRAME009),
    { { 21, -52, 15, 47 } },
};
const struct sprite_frame_1box gSpriteBank20Frame010 = {
    SPRITE_FRAME(gSpriteBank20Frame010, SPRITE_TILES_BANK20_FRAME010),
    { { 21, -52, 15, 47 } },
};
const struct sprite_frame_1box gSpriteBank20Frame011 = {
    SPRITE_FRAME(gSpriteBank20Frame011, SPRITE_TILES_BANK20_FRAME011),
    { { 21, -52, 15, 47 } },
};
const struct sprite_frame_1box gSpriteBank20Frame012 = {
    SPRITE_FRAME(gSpriteBank20Frame012, SPRITE_TILES_BANK20_FRAME012),
    { { 21, -52, 15, 47 } },
};
const struct sprite_frame_1box gSpriteBank20Frame013 = {
    SPRITE_FRAME(gSpriteBank20Frame013, SPRITE_TILES_BANK20_FRAME013),
    { { 21, -52, 15, 47 } },
};
const struct sprite_frame_1box gSpriteBank20Frame014 = {
    SPRITE_FRAME(gSpriteBank20Frame014, SPRITE_TILES_BANK20_FRAME014),
    { { 21, -52, 15, 47 } },
};
const struct sprite_frame_1box gSpriteBank20Frame015 = {
    SPRITE_FRAME(gSpriteBank20Frame015, SPRITE_TILES_BANK20_FRAME015),
    { { 21, -52, 15, 47 } },
};
const struct sprite_frame_1box gSpriteBank20Frame016 = {
    SPRITE_FRAME(gSpriteBank20Frame016, SPRITE_TILES_BANK20_FRAME016),
    { { 21, -52, 15, 47 } },
};
const struct sprite_frame_1box gSpriteBank20Frame017 = {
    SPRITE_FRAME(gSpriteBank20Frame017, SPRITE_TILES_BANK20_FRAME017),
    { { 15, -52, 21, 49 } },
};
const struct sprite_frame_1box gSpriteBank20Frame018 = {
    SPRITE_FRAME(gSpriteBank20Frame018, SPRITE_TILES_BANK20_FRAME018),
    { { 15, -52, 21, 49 } },
};
const struct sprite_frame_1box gSpriteBank20Frame019 = {
    SPRITE_FRAME(gSpriteBank20Frame019, SPRITE_TILES_BANK20_FRAME019),
    { { 15, -52, 21, 49 } },
};
const struct sprite_frame_1box gSpriteBank20Frame020 = {
    SPRITE_FRAME(gSpriteBank20Frame020, SPRITE_TILES_BANK20_FRAME020),
    { { 15, -52, 21, 49 } },
};
const struct sprite_frame_1box gSpriteBank20Frame021 = {
    SPRITE_FRAME(gSpriteBank20Frame021, SPRITE_TILES_BANK20_FRAME021),
    { { 15, -52, 21, 49 } },
};
const struct sprite_frame_1box gSpriteBank20Frame022 = {
    SPRITE_FRAME(gSpriteBank20Frame022, SPRITE_TILES_BANK20_FRAME022),
    { { 15, -52, 20, 49 } },
};
const struct sprite_frame_1box gSpriteBank20Frame023 = {
    SPRITE_FRAME(gSpriteBank20Frame023, SPRITE_TILES_BANK20_FRAME023),
    { { 15, -52, 20, 49 } },
};
const struct sprite_frame_1box gSpriteBank20Frame024 = {
    SPRITE_FRAME(gSpriteBank20Frame024, SPRITE_TILES_BANK20_FRAME024),
    { { 15, -52, 21, 49 } },
};
const struct sprite_frame_1box gSpriteBank20Frame025 = {
    SPRITE_FRAME(gSpriteBank20Frame025, SPRITE_TILES_BANK20_FRAME025),
    { { 16, -52, 20, 49 } },
};
const struct sprite_frame_1box gSpriteBank20Frame026 = {
    SPRITE_FRAME(gSpriteBank20Frame026, SPRITE_TILES_BANK20_FRAME026),
    { { 15, -52, 21, 49 } },
};
const struct sprite_frame_1box gSpriteBank20Frame027 = {
    SPRITE_FRAME(gSpriteBank20Frame027, SPRITE_TILES_BANK20_FRAME027),
    { { 16, -52, 21, 49 } },
};
const struct sprite_frame_1box gSpriteBank20Frame028 = {
    SPRITE_FRAME(gSpriteBank20Frame028, SPRITE_TILES_BANK20_FRAME028),
    { { 18, -52, 21, 49 } },
};
const struct sprite_frame_1box gSpriteBank20Frame029 = {
    SPRITE_FRAME(gSpriteBank20Frame029, SPRITE_TILES_BANK20_FRAME029),
    { { 23, -48, 19, 46 } },
};
const struct sprite_frame_1box gSpriteBank20Frame030 = {
    SPRITE_FRAME(gSpriteBank20Frame030, SPRITE_TILES_BANK20_FRAME030),
    { { 25, -46, 23, 42 } },
};
const struct sprite_frame_1box gSpriteBank20Frame031 = {
    SPRITE_FRAME(gSpriteBank20Frame031, SPRITE_TILES_BANK20_FRAME031),
    { { 23, -44, 15, 41 } },
};
const struct sprite_frame_1box gSpriteBank20Frame032 = {
    SPRITE_FRAME(gSpriteBank20Frame032, SPRITE_TILES_BANK20_FRAME032),
    { { 22, -44, 15, 41 } },
};
const struct sprite_frame_1box gSpriteBank20Frame033 = {
    SPRITE_FRAME(gSpriteBank20Frame033, SPRITE_TILES_BANK20_FRAME033),
    { { 22, -44, 17, 41 } },
};
const struct sprite_frame_1box gSpriteBank20Frame034 = {
    SPRITE_FRAME(gSpriteBank20Frame034, SPRITE_TILES_BANK20_FRAME034),
    { { 22, -44, 17, 41 } },
};
const struct sprite_frame_1box gSpriteBank20Frame035 = {
    SPRITE_FRAME(gSpriteBank20Frame035, SPRITE_TILES_BANK20_FRAME035),
    { { 22, -44, 16, 41 } },
};
const struct sprite_frame_1box gSpriteBank20Frame036 = {
    SPRITE_FRAME(gSpriteBank20Frame036, SPRITE_TILES_BANK20_FRAME036),
    { { 21, -44, 18, 41 } },
};
const struct sprite_frame_1box gSpriteBank20Frame037 = {
    SPRITE_FRAME(gSpriteBank20Frame037, SPRITE_TILES_BANK20_FRAME037),
    { { 21, -44, 20, 41 } },
};
const struct sprite_frame_1box gSpriteBank20Frame038 = {
    SPRITE_FRAME(gSpriteBank20Frame038, SPRITE_TILES_BANK20_FRAME038),
    { { 21, -44, 18, 41 } },
};
const struct sprite_frame_1box gSpriteBank20Frame039 = {
    SPRITE_FRAME(gSpriteBank20Frame039, SPRITE_TILES_BANK20_FRAME039),
    { { 21, -44, 18, 41 } },
};
const struct sprite_frame_1box gSpriteBank20Frame040 = {
    SPRITE_FRAME(gSpriteBank20Frame040, SPRITE_TILES_BANK20_FRAME040),
    { { 20, -44, 18, 41 } },
};
const struct sprite_frame_1box gSpriteBank20Frame041 = {
    SPRITE_FRAME(gSpriteBank20Frame041, SPRITE_TILES_BANK20_FRAME041),
    { { 21, -44, 19, 41 } },
};
const struct sprite_frame_1box gSpriteBank20Frame042 = {
    SPRITE_FRAME(gSpriteBank20Frame042, SPRITE_TILES_BANK20_FRAME042),
    { { 20, -44, 19, 41 } },
};
const struct sprite_frame_1box gSpriteBank20Frame043 = {
    SPRITE_FRAME(gSpriteBank20Frame043, SPRITE_TILES_BANK20_FRAME043),
    { { 20, -44, 17, 41 } },
};
const struct sprite_frame_1box gSpriteBank20Frame044 = {
    SPRITE_FRAME(gSpriteBank20Frame044, SPRITE_TILES_BANK20_FRAME044),
    { { 20, -44, 17, 41 } },
};
const struct sprite_frame_1box gSpriteBank20Frame045 = {
    SPRITE_FRAME(gSpriteBank20Frame045, SPRITE_TILES_BANK20_FRAME045),
    { { 19, -44, 19, 41 } },
};
const struct sprite_frame_1box gSpriteBank20Frame046 = {
    SPRITE_FRAME(gSpriteBank20Frame046, SPRITE_TILES_BANK20_FRAME046),
    { { 41, -37, 12, 7 } },
};
const struct sprite_frame_1box gSpriteBank20Frame047 = {
    SPRITE_FRAME(gSpriteBank20Frame047, SPRITE_TILES_BANK20_FRAME047),
    { { 43, -39, 13, 8 } },
};
const struct sprite_frame_1box gSpriteBank20Frame048 = {
    SPRITE_FRAME(gSpriteBank20Frame048, SPRITE_TILES_BANK20_FRAME048),
    { { 46, -41, 15, 10 } },
};
const struct sprite_frame_1box gSpriteBank20Frame049 = {
    SPRITE_FRAME(gSpriteBank20Frame049, SPRITE_TILES_BANK20_FRAME049),
    { { 42, -41, 32, 11 } },
};
const struct sprite_frame_1box gSpriteBank20Frame050 = {
    SPRITE_FRAME(gSpriteBank20Frame050, SPRITE_TILES_BANK20_FRAME050),
    { { 42, -42, 38, 14 } },
};
const struct sprite_frame_1box gSpriteBank20Frame051 = {
    SPRITE_FRAME(gSpriteBank20Frame051, SPRITE_TILES_BANK20_FRAME051),
    { { 46, -42, 35, 14 } },
};
const struct sprite_frame_1box gSpriteBank20Frame052 = {
    SPRITE_FRAME(gSpriteBank20Frame052, SPRITE_TILES_BANK20_FRAME052),
    { { 46, -46, 41, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame053 = {
    SPRITE_FRAME(gSpriteBank20Frame053, SPRITE_TILES_BANK20_FRAME053),
    { { 47, -50, 43, 22 } },
};
const struct sprite_frame_1box gSpriteBank20Frame054 = {
    SPRITE_FRAME(gSpriteBank20Frame054, SPRITE_TILES_BANK20_FRAME054),
    { { 52, -49, 48, 21 } },
};
const struct sprite_frame_1box gSpriteBank20Frame055 = {
    SPRITE_FRAME(gSpriteBank20Frame055, SPRITE_TILES_BANK20_FRAME055),
    { { 51, -49, 48, 20 } },
};
const struct sprite_frame_1box gSpriteBank20Frame056 = {
    SPRITE_FRAME(gSpriteBank20Frame056, SPRITE_TILES_BANK20_FRAME056),
    { { 54, -48, 43, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame057 = {
    SPRITE_FRAME(gSpriteBank20Frame057, SPRITE_TILES_BANK20_FRAME057),
    { { 55, -48, 40, 14 } },
};
const struct sprite_frame_1box gSpriteBank20Frame058 = {
    SPRITE_FRAME(gSpriteBank20Frame058, SPRITE_TILES_BANK20_FRAME058),
    { { 66, -42, 29, 8 } },
};
const struct sprite_frame_1box gSpriteBank20Frame059 = {
    SPRITE_FRAME(gSpriteBank20Frame059, SPRITE_TILES_BANK20_FRAME059),
    { { 80, -42, 18, 7 } },
};
const struct sprite_frame_1box gSpriteBank20Frame060 = {
    SPRITE_FRAME(gSpriteBank20Frame060, SPRITE_TILES_BANK20_FRAME060),
    { { 88, -41, 14, 5 } },
};

/* The piece arrays, from games/n2/graphics/sprites/bank20_fireball_tribesman/bank.json. */
#include "sprites/bank20_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 21: 3 animations, 65 frames, games/n2/graphics/sprites/bank21_banana_monkey/. */

extern const u16 gSpriteBank21Anim00Seq[12];
extern const u16 gSpriteBank21Anim01Seq[41];
extern const u16 gSpriteBank21Anim02Seq[12];

extern const struct sprite_anim gSpriteBank21Anims[3] = {
    /* 0 */ {
        /* seq */ gSpriteBank21Anim00Seq,
        /* box */ { { -18, -14, 29, 29 }, { -19, -14, 32, 29 } },
        /* paletteId */ 26,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank21Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank21Anim01Seq,
        /* box */ { { -18, -14, 29, 29 }, { -29, -30, 54, 46 } },
        /* paletteId */ 26,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank21Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank21Anim02Seq,
        /* box */ { { -18, -14, 29, 29 }, { -19, -14, 34, 30 } },
        /* paletteId */ 26,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank21Anim02Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank21Anim00Seq[12] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
};
const u16 gSpriteBank21Anim01Seq[41] = {
    12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
    28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
    44, 45, 46, 47, 48, 49, 50, 51, 52,
};
const u16 gSpriteBank21Anim02Seq[12] = {
    53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64,
};

extern const struct sprite_frame *const gSpriteBank21Frames[65] = {
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
    &gSpriteBank21Frame015.frame,
    &gSpriteBank21Frame016.frame,
    &gSpriteBank21Frame017.frame,
    &gSpriteBank21Frame018.frame,
    &gSpriteBank21Frame019.frame,
    &gSpriteBank21Frame020.frame,
    &gSpriteBank21Frame021.frame,
    &gSpriteBank21Frame022.frame,
    &gSpriteBank21Frame023.frame,
    &gSpriteBank21Frame024.frame,
    &gSpriteBank21Frame025.frame,
    &gSpriteBank21Frame026.frame,
    &gSpriteBank21Frame027.frame,
    &gSpriteBank21Frame028.frame,
    &gSpriteBank21Frame029.frame,
    &gSpriteBank21Frame030.frame,
    &gSpriteBank21Frame031.frame,
    &gSpriteBank21Frame032.frame,
    &gSpriteBank21Frame033.frame,
    &gSpriteBank21Frame034.frame,
    &gSpriteBank21Frame035.frame,
    &gSpriteBank21Frame036.frame,
    &gSpriteBank21Frame037.frame,
    &gSpriteBank21Frame038.frame,
    &gSpriteBank21Frame039.frame,
    &gSpriteBank21Frame040.frame,
    &gSpriteBank21Frame041.frame,
    &gSpriteBank21Frame042.frame,
    &gSpriteBank21Frame043.frame,
    &gSpriteBank21Frame044.frame,
    &gSpriteBank21Frame045.frame,
    &gSpriteBank21Frame046.frame,
    &gSpriteBank21Frame047.frame,
    &gSpriteBank21Frame048.frame,
    &gSpriteBank21Frame049.frame,
    &gSpriteBank21Frame050.frame,
    &gSpriteBank21Frame051.frame,
    &gSpriteBank21Frame052.frame,
    &gSpriteBank21Frame053.frame,
    &gSpriteBank21Frame054.frame,
    &gSpriteBank21Frame055.frame,
    &gSpriteBank21Frame056.frame,
    &gSpriteBank21Frame057.frame,
    &gSpriteBank21Frame058.frame,
    &gSpriteBank21Frame059.frame,
    &gSpriteBank21Frame060.frame,
    &gSpriteBank21Frame061.frame,
    &gSpriteBank21Frame062.frame,
    &gSpriteBank21Frame063.frame,
    &gSpriteBank21Frame064.frame,
};

const struct sprite_frame_3box_anchor gSpriteBank21Frame000 = {
    SPRITE_FRAME(gSpriteBank21Frame000, SPRITE_TILES_BANK21_FRAME000),
    { { -18, 0, 28, 15 }, { 0, 0, 0, 0 }, { -18, -12, 30, 12 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame001 = {
    SPRITE_FRAME(gSpriteBank21Frame001, SPRITE_TILES_BANK21_FRAME001),
    { { -18, 0, 28, 15 }, { 0, 0, 0, 0 }, { -18, -12, 30, 12 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame002 = {
    SPRITE_FRAME(gSpriteBank21Frame002, SPRITE_TILES_BANK21_FRAME002),
    { { -18, 0, 28, 15 }, { 0, 0, 0, 0 }, { -18, -12, 30, 12 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame003 = {
    SPRITE_FRAME(gSpriteBank21Frame003, SPRITE_TILES_BANK21_FRAME003),
    { { -18, 0, 28, 15 }, { 0, 0, 0, 0 }, { -18, -12, 30, 12 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame004 = {
    SPRITE_FRAME(gSpriteBank21Frame004, SPRITE_TILES_BANK21_FRAME004),
    { { -18, 0, 28, 15 }, { 0, 0, 0, 0 }, { -18, -12, 30, 12 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame005 = {
    SPRITE_FRAME(gSpriteBank21Frame005, SPRITE_TILES_BANK21_FRAME005),
    { { -18, 0, 28, 15 }, { 0, 0, 0, 0 }, { -18, -12, 30, 12 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame006 = {
    SPRITE_FRAME(gSpriteBank21Frame006, SPRITE_TILES_BANK21_FRAME006),
    { { -18, 0, 28, 15 }, { 0, 0, 0, 0 }, { -18, -12, 30, 12 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame007 = {
    SPRITE_FRAME(gSpriteBank21Frame007, SPRITE_TILES_BANK21_FRAME007),
    { { -18, 0, 28, 15 }, { 0, 0, 0, 0 }, { -18, -12, 30, 12 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame008 = {
    SPRITE_FRAME(gSpriteBank21Frame008, SPRITE_TILES_BANK21_FRAME008),
    { { -18, 0, 28, 15 }, { 0, 0, 0, 0 }, { -18, -12, 30, 12 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame009 = {
    SPRITE_FRAME(gSpriteBank21Frame009, SPRITE_TILES_BANK21_FRAME009),
    { { -18, 0, 28, 15 }, { 0, 0, 0, 0 }, { -18, -12, 30, 12 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame010 = {
    SPRITE_FRAME(gSpriteBank21Frame010, SPRITE_TILES_BANK21_FRAME010),
    { { -18, 0, 28, 15 }, { 0, 0, 0, 0 }, { -18, -12, 30, 12 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame011 = {
    SPRITE_FRAME(gSpriteBank21Frame011, SPRITE_TILES_BANK21_FRAME011),
    { { -18, 0, 28, 15 }, { 0, 0, 0, 0 }, { -18, -12, 30, 12 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame012 = {
    SPRITE_FRAME(gSpriteBank21Frame012, SPRITE_TILES_BANK21_FRAME012),
    { { -18, -3, 26, 19 }, { 0, 0, 0, 0 }, { -19, -12, 28, 9 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame013 = {
    SPRITE_FRAME(gSpriteBank21Frame013, SPRITE_TILES_BANK21_FRAME013),
    { { -17, -6, 22, 17 }, { 0, 0, 0, 0 }, { -18, -16, 24, 10 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame014 = {
    SPRITE_FRAME(gSpriteBank21Frame014, SPRITE_TILES_BANK21_FRAME014),
    { { -15, -14, 16, 19 }, { 0, 0, 0, 0 }, { -17, -25, 24, 12 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame015 = {
    SPRITE_FRAME(gSpriteBank21Frame015, SPRITE_TILES_BANK21_FRAME015),
    { { -14, -14, 0, 0 }, { 0, 0, 0, 0 }, { -20, -29, 25, 11 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame016 = {
    SPRITE_FRAME(gSpriteBank21Frame016, SPRITE_TILES_BANK21_FRAME016),
    { { -16, -10, 19, 11 }, { 0, 0, 0, 0 }, { -20, -23, 24, 12 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame017 = {
    SPRITE_FRAME(gSpriteBank21Frame017, SPRITE_TILES_BANK21_FRAME017),
    { { -16, -5, 21, 18 }, { 0, 0, 0, 0 }, { -19, -15, 25, 10 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame018 = {
    SPRITE_FRAME(gSpriteBank21Frame018, SPRITE_TILES_BANK21_FRAME018),
    { { -18, 1, 23, 14 }, { 0, 0, 0, 0 }, { -18, -11, 23, 12 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame019 = {
    SPRITE_FRAME(gSpriteBank21Frame019, SPRITE_TILES_BANK21_FRAME019),
    { { -19, 3, 24, 12 }, { 0, 0, 0, 0 }, { -20, -8, 27, 11 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame020 = {
    SPRITE_FRAME(gSpriteBank21Frame020, SPRITE_TILES_BANK21_FRAME020),
    { { -15, -6, 17, 19 }, { 0, 0, 0, 0 }, { -19, -17, 23, 13 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame021 = {
    SPRITE_FRAME(gSpriteBank21Frame021, SPRITE_TILES_BANK21_FRAME021),
    { { -15, -11, 17, 18 }, { 0, 0, 0, 0 }, { -21, -21, 24, 9 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame022 = {
    SPRITE_FRAME(gSpriteBank21Frame022, SPRITE_TILES_BANK21_FRAME022),
    { { -15, -16, 18, 18 }, { 0, 0, 0, 0 }, { -21, -26, 25, 10 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame023 = {
    SPRITE_FRAME(gSpriteBank21Frame023, SPRITE_TILES_BANK21_FRAME023),
    { { -15, -13, 17, 19 }, { 0, 0, 0, 0 }, { -21, -23, 24, 10 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame024 = {
    SPRITE_FRAME(gSpriteBank21Frame024, SPRITE_TILES_BANK21_FRAME024),
    { { -19, -5, 22, 16 }, { 0, 0, 0, 0 }, { -19, -19, 23, 14 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame025 = {
    SPRITE_FRAME(gSpriteBank21Frame025, SPRITE_TILES_BANK21_FRAME025),
    { { -18, -1, 22, 16 }, { 0, 0, 0, 0 }, { -19, -13, 24, 11 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame026 = {
    SPRITE_FRAME(gSpriteBank21Frame026, SPRITE_TILES_BANK21_FRAME026),
    { { -16, 0, 24, 15 }, { 0, 0, 0, 0 }, { -17, -10, 27, 9 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame027 = {
    SPRITE_FRAME(gSpriteBank21Frame027, SPRITE_TILES_BANK21_FRAME027),
    { { -13, -4, 22, 18 }, { 0, 0, 0, 0 }, { -13, -14, 24, 10 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame028 = {
    SPRITE_FRAME(gSpriteBank21Frame028, SPRITE_TILES_BANK21_FRAME028),
    { { -13, -10, 18, 22 }, { 0, 0, 0, 0 }, { -20, -16, 27, 7 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame029 = {
    SPRITE_FRAME(gSpriteBank21Frame029, SPRITE_TILES_BANK21_FRAME029),
    { { -15, -3, 17, 16 }, { 0, 0, 0, 0 }, { -26, -14, 29, 11 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame030 = {
    SPRITE_FRAME(gSpriteBank21Frame030, SPRITE_TILES_BANK21_FRAME030),
    { { -16, -5, 19, 16 }, { 0, 0, 0, 0 }, { -19, -13, 23, 8 } },
    { 0, 0 },
};
const struct sprite_frame_2box gSpriteBank21Frame031 = {
    SPRITE_FRAME(gSpriteBank21Frame031, SPRITE_TILES_BANK21_FRAME031),
    { { -20, -8, 26, 16 }, { 6, -19, 11, 22 } },
};
const struct sprite_frame_2box gSpriteBank21Frame032 = {
    SPRITE_FRAME(gSpriteBank21Frame032, SPRITE_TILES_BANK21_FRAME032),
    { { -12, -18, 20, 22 }, { 0, 5, 29, 10 } },
};
const struct sprite_frame_2box gSpriteBank21Frame033 = {
    SPRITE_FRAME(gSpriteBank21Frame033, SPRITE_TILES_BANK21_FRAME033),
    { { -14, -19, 22, 16 }, { -1, -2, 26, 13 } },
};
const struct sprite_frame_2box gSpriteBank21Frame034 = {
    SPRITE_FRAME(gSpriteBank21Frame034, SPRITE_TILES_BANK21_FRAME034),
    { { -22, -18, 35, 14 }, { 1, -2, 24, 13 } },
};
const struct sprite_frame_2box gSpriteBank21Frame035 = {
    SPRITE_FRAME(gSpriteBank21Frame035, SPRITE_TILES_BANK21_FRAME035),
    { { -14, -25, 25, 18 }, { 2, 0, 24, 11 } },
};
const struct sprite_frame_1box gSpriteBank21Frame036 = {
    SPRITE_FRAME(gSpriteBank21Frame036, SPRITE_TILES_BANK21_FRAME036),
    { { -13, -21, 13, 23 } },
};
const struct sprite_frame_1box gSpriteBank21Frame037 = {
    SPRITE_FRAME(gSpriteBank21Frame037, SPRITE_TILES_BANK21_FRAME037),
    { { -14, -13, 15, 24 } },
};
const struct sprite_frame_1box gSpriteBank21Frame038 = {
    SPRITE_FRAME(gSpriteBank21Frame038, SPRITE_TILES_BANK21_FRAME038),
    { { -16, -11, 16, 25 } },
};
const struct sprite_frame_1box gSpriteBank21Frame039 = {
    SPRITE_FRAME(gSpriteBank21Frame039, SPRITE_TILES_BANK21_FRAME039),
    { { -18, -8, 21, 22 } },
};
const struct sprite_frame_1box gSpriteBank21Frame040 = {
    SPRITE_FRAME(gSpriteBank21Frame040, SPRITE_TILES_BANK21_FRAME040),
    { { -16, -9, 18, 23 } },
};
const struct sprite_frame_1box gSpriteBank21Frame041 = {
    SPRITE_FRAME(gSpriteBank21Frame041, SPRITE_TILES_BANK21_FRAME041),
    { { -16, -7, 18, 22 } },
};
const struct sprite_frame_1box gSpriteBank21Frame042 = {
    SPRITE_FRAME(gSpriteBank21Frame042, SPRITE_TILES_BANK21_FRAME042),
    { { -17, -7, 19, 22 } },
};
const struct sprite_frame_1box gSpriteBank21Frame043 = {
    SPRITE_FRAME(gSpriteBank21Frame043, SPRITE_TILES_BANK21_FRAME043),
    { { -16, -7, 19, 21 } },
};
const struct sprite_frame_1box gSpriteBank21Frame044 = {
    SPRITE_FRAME(gSpriteBank21Frame044, SPRITE_TILES_BANK21_FRAME044),
    { { -16, -8, 23, 22 } },
};
const struct sprite_frame_1box gSpriteBank21Frame045 = {
    SPRITE_FRAME(gSpriteBank21Frame045, SPRITE_TILES_BANK21_FRAME045),
    { { -17, -10, 23, 24 } },
};
const struct sprite_frame_1box gSpriteBank21Frame046 = {
    SPRITE_FRAME(gSpriteBank21Frame046, SPRITE_TILES_BANK21_FRAME046),
    { { -16, -7, 23, 20 } },
};
const struct sprite_frame_1box gSpriteBank21Frame047 = {
    SPRITE_FRAME(gSpriteBank21Frame047, SPRITE_TILES_BANK21_FRAME047),
    { { -16, -16, 17, 29 } },
};
const struct sprite_frame_1box gSpriteBank21Frame048 = {
    SPRITE_FRAME(gSpriteBank21Frame048, SPRITE_TILES_BANK21_FRAME048),
    { { -17, -16, 19, 29 } },
};
const struct sprite_frame_1box gSpriteBank21Frame049 = {
    SPRITE_FRAME(gSpriteBank21Frame049, SPRITE_TILES_BANK21_FRAME049),
    { { -17, -16, 19, 30 } },
};
const struct sprite_frame_1box gSpriteBank21Frame050 = {
    SPRITE_FRAME(gSpriteBank21Frame050, SPRITE_TILES_BANK21_FRAME050),
    { { -15, -13, 19, 27 } },
};
const struct sprite_frame_1box gSpriteBank21Frame051 = {
    SPRITE_FRAME(gSpriteBank21Frame051, SPRITE_TILES_BANK21_FRAME051),
    { { -16, -11, 21, 24 } },
};
const struct sprite_frame_1box gSpriteBank21Frame052 = {
    SPRITE_FRAME(gSpriteBank21Frame052, SPRITE_TILES_BANK21_FRAME052),
    { { -16, -6, 22, 22 } },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame053 = {
    SPRITE_FRAME(gSpriteBank21Frame053, SPRITE_TILES_BANK21_FRAME053),
    { { -18, -1, 29, 16 }, { 0, 0, 0, 0 }, { -18, -14, 29, 12 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame054 = {
    SPRITE_FRAME(gSpriteBank21Frame054, SPRITE_TILES_BANK21_FRAME054),
    { { -18, -1, 29, 16 }, { 0, 0, 0, 0 }, { -18, -14, 29, 12 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame055 = {
    SPRITE_FRAME(gSpriteBank21Frame055, SPRITE_TILES_BANK21_FRAME055),
    { { -18, -1, 29, 16 }, { 0, 0, 0, 0 }, { -18, -14, 29, 12 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame056 = {
    SPRITE_FRAME(gSpriteBank21Frame056, SPRITE_TILES_BANK21_FRAME056),
    { { -18, -1, 29, 16 }, { 0, 0, 0, 0 }, { -18, -14, 29, 12 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame057 = {
    SPRITE_FRAME(gSpriteBank21Frame057, SPRITE_TILES_BANK21_FRAME057),
    { { -18, -1, 29, 16 }, { 0, 0, 0, 0 }, { -18, -14, 29, 12 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame058 = {
    SPRITE_FRAME(gSpriteBank21Frame058, SPRITE_TILES_BANK21_FRAME058),
    { { -18, -1, 29, 16 }, { 0, 0, 0, 0 }, { -18, -14, 29, 12 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame059 = {
    SPRITE_FRAME(gSpriteBank21Frame059, SPRITE_TILES_BANK21_FRAME059),
    { { -18, -1, 29, 16 }, { 0, 0, 0, 0 }, { -18, -14, 29, 12 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame060 = {
    SPRITE_FRAME(gSpriteBank21Frame060, SPRITE_TILES_BANK21_FRAME060),
    { { -18, -1, 29, 16 }, { 0, 0, 0, 0 }, { -18, -14, 29, 12 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame061 = {
    SPRITE_FRAME(gSpriteBank21Frame061, SPRITE_TILES_BANK21_FRAME061),
    { { -18, -1, 29, 16 }, { 0, 0, 0, 0 }, { -18, -14, 29, 12 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame062 = {
    SPRITE_FRAME(gSpriteBank21Frame062, SPRITE_TILES_BANK21_FRAME062),
    { { -18, -1, 29, 16 }, { 0, 0, 0, 0 }, { -18, -14, 29, 12 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame063 = {
    SPRITE_FRAME(gSpriteBank21Frame063, SPRITE_TILES_BANK21_FRAME063),
    { { -18, -1, 29, 16 }, { 0, 0, 0, 0 }, { -18, -14, 29, 12 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank21Frame064 = {
    SPRITE_FRAME(gSpriteBank21Frame064, SPRITE_TILES_BANK21_FRAME064),
    { { -18, -1, 29, 16 }, { 0, 0, 0, 0 }, { -18, -14, 29, 12 } },
    { 0, 0 },
};

/* The piece arrays, from games/n2/graphics/sprites/bank21_banana_monkey/bank.json. */
#include "sprites/bank21_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 22: 4 animations, 28 frames, games/n2/graphics/sprites/bank22_fire_burst/. */

extern const u16 gSpriteBank22Anim00Seq[5];
extern const u16 gSpriteBank22Anim01Seq[6];
extern const u16 gSpriteBank22Anim02Seq[14];
extern const u16 gSpriteBank22Anim03Seq[3];

extern const struct sprite_anim gSpriteBank22Anims[4] = {
    /* 0 */ {
        /* seq */ gSpriteBank22Anim00Seq,
        /* box */ { { -18, -11, 36, 14 }, { -20, -30, 39, 32 } },
        /* paletteId */ 27,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank22Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank22Anim01Seq,
        /* box */ { { -18, -11, 36, 14 }, { -20, -21, 39, 23 } },
        /* paletteId */ 27,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank22Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank22Anim02Seq,
        /* box */ { { -11, -18, 23, 36 }, { -13, -18, 27, 43 } },
        /* paletteId */ 27,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank22Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank22Anim03Seq,
        /* box */ { { -18, -11, 36, 14 }, { -19, -10, 37, 12 } },
        /* paletteId */ 27,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank22Anim03Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank22Anim00Seq[5] = {
    0, 1, 2, 3, 4,
};
const u16 gSpriteBank22Anim01Seq[6] = {
    5, 6, 7, 8, 9, 10,
};
const u16 gSpriteBank22Anim02Seq[14] = {
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
};
const u16 gSpriteBank22Anim03Seq[3] = {
    25, 26, 27,
};

extern const struct sprite_frame *const gSpriteBank22Frames[28] = {
    &gSpriteBank22Frame000,
    &gSpriteBank22Frame001,
    &gSpriteBank22Frame002,
    &gSpriteBank22Frame003,
    &gSpriteBank22Frame004,
    &gSpriteBank22Frame005,
    &gSpriteBank22Frame006,
    &gSpriteBank22Frame007,
    &gSpriteBank22Frame008,
    &gSpriteBank22Frame009,
    &gSpriteBank22Frame010,
    &gSpriteBank22Frame011.frame,
    &gSpriteBank22Frame012.frame,
    &gSpriteBank22Frame013.frame,
    &gSpriteBank22Frame014.frame,
    &gSpriteBank22Frame015.frame,
    &gSpriteBank22Frame016.frame,
    &gSpriteBank22Frame017.frame,
    &gSpriteBank22Frame018.frame,
    &gSpriteBank22Frame019.frame,
    &gSpriteBank22Frame020.frame,
    &gSpriteBank22Frame021.frame,
    &gSpriteBank22Frame022.frame,
    &gSpriteBank22Frame023.frame,
    &gSpriteBank22Frame024.frame,
    &gSpriteBank22Frame025,
    &gSpriteBank22Frame026,
    &gSpriteBank22Frame027,
};

const struct sprite_frame gSpriteBank22Frame000 = SPRITE_FRAME(gSpriteBank22Frame000, SPRITE_TILES_BANK22_FRAME000);
const struct sprite_frame gSpriteBank22Frame001 = SPRITE_FRAME(gSpriteBank22Frame001, SPRITE_TILES_BANK22_FRAME001);
const struct sprite_frame gSpriteBank22Frame002 = SPRITE_FRAME(gSpriteBank22Frame002, SPRITE_TILES_BANK22_FRAME002);
const struct sprite_frame gSpriteBank22Frame003 = SPRITE_FRAME(gSpriteBank22Frame003, SPRITE_TILES_BANK22_FRAME003);
const struct sprite_frame gSpriteBank22Frame004 = SPRITE_FRAME(gSpriteBank22Frame004, SPRITE_TILES_BANK22_FRAME004);
const struct sprite_frame gSpriteBank22Frame005 = SPRITE_FRAME(gSpriteBank22Frame005, SPRITE_TILES_BANK22_FRAME005);
const struct sprite_frame gSpriteBank22Frame006 = SPRITE_FRAME(gSpriteBank22Frame006, SPRITE_TILES_BANK22_FRAME006);
const struct sprite_frame gSpriteBank22Frame007 = SPRITE_FRAME(gSpriteBank22Frame007, SPRITE_TILES_BANK22_FRAME007);
const struct sprite_frame gSpriteBank22Frame008 = SPRITE_FRAME(gSpriteBank22Frame008, SPRITE_TILES_BANK22_FRAME008);
const struct sprite_frame gSpriteBank22Frame009 = SPRITE_FRAME(gSpriteBank22Frame009, SPRITE_TILES_BANK22_FRAME009);
const struct sprite_frame gSpriteBank22Frame010 = SPRITE_FRAME(gSpriteBank22Frame010, SPRITE_TILES_BANK22_FRAME010);
const struct sprite_frame_1box gSpriteBank22Frame011 = {
    SPRITE_FRAME(gSpriteBank22Frame011, SPRITE_TILES_BANK22_FRAME011),
    { { -10, -15, 23, 18 } },
};
const struct sprite_frame_1box gSpriteBank22Frame012 = {
    SPRITE_FRAME(gSpriteBank22Frame012, SPRITE_TILES_BANK22_FRAME012),
    { { -10, -16, 22, 18 } },
};
const struct sprite_frame_1box gSpriteBank22Frame013 = {
    SPRITE_FRAME(gSpriteBank22Frame013, SPRITE_TILES_BANK22_FRAME013),
    { { -10, -17, 22, 18 } },
};
const struct sprite_frame_1box gSpriteBank22Frame014 = {
    SPRITE_FRAME(gSpriteBank22Frame014, SPRITE_TILES_BANK22_FRAME014),
    { { -10, -16, 22, 16 } },
};
const struct sprite_frame_1box gSpriteBank22Frame015 = {
    SPRITE_FRAME(gSpriteBank22Frame015, SPRITE_TILES_BANK22_FRAME015),
    { { -9, -16, 20, 14 } },
};
const struct sprite_frame_1box gSpriteBank22Frame016 = {
    SPRITE_FRAME(gSpriteBank22Frame016, SPRITE_TILES_BANK22_FRAME016),
    { { -9, -15, 21, 14 } },
};
const struct sprite_frame_1box gSpriteBank22Frame017 = {
    SPRITE_FRAME(gSpriteBank22Frame017, SPRITE_TILES_BANK22_FRAME017),
    { { -9, -17, 20, 16 } },
};
const struct sprite_frame_1box gSpriteBank22Frame018 = {
    SPRITE_FRAME(gSpriteBank22Frame018, SPRITE_TILES_BANK22_FRAME018),
    { { -8, -15, 18, 13 } },
};
const struct sprite_frame_1box gSpriteBank22Frame019 = {
    SPRITE_FRAME(gSpriteBank22Frame019, SPRITE_TILES_BANK22_FRAME019),
    { { -8, -14, 19, 14 } },
};
const struct sprite_frame_1box gSpriteBank22Frame020 = {
    SPRITE_FRAME(gSpriteBank22Frame020, SPRITE_TILES_BANK22_FRAME020),
    { { -9, -13, 20, 13 } },
};
const struct sprite_frame_1box gSpriteBank22Frame021 = {
    SPRITE_FRAME(gSpriteBank22Frame021, SPRITE_TILES_BANK22_FRAME021),
    { { -9, -15, 20, 10 } },
};
const struct sprite_frame_1box gSpriteBank22Frame022 = {
    SPRITE_FRAME(gSpriteBank22Frame022, SPRITE_TILES_BANK22_FRAME022),
    { { -8, -16, 18, 11 } },
};
const struct sprite_frame_1box gSpriteBank22Frame023 = {
    SPRITE_FRAME(gSpriteBank22Frame023, SPRITE_TILES_BANK22_FRAME023),
    { { -9, -18, 20, 12 } },
};
const struct sprite_frame_1box gSpriteBank22Frame024 = {
    SPRITE_FRAME(gSpriteBank22Frame024, SPRITE_TILES_BANK22_FRAME024),
    { { -9, -16, 19, 11 } },
};
const struct sprite_frame gSpriteBank22Frame025 = SPRITE_FRAME(gSpriteBank22Frame025, SPRITE_TILES_BANK22_FRAME025);
const struct sprite_frame gSpriteBank22Frame026 = SPRITE_FRAME(gSpriteBank22Frame026, SPRITE_TILES_BANK22_FRAME026);
const struct sprite_frame gSpriteBank22Frame027 = SPRITE_FRAME(gSpriteBank22Frame027, SPRITE_TILES_BANK22_FRAME027);

/* The piece arrays, from games/n2/graphics/sprites/bank22_fire_burst/bank.json. */
#include "sprites/bank22_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 23: 43 animations, 132 frames, games/n2/graphics/sprites/bank23_crates/. */

extern const u16 gSpriteBank23Anim00Seq[1];
extern const u16 gSpriteBank23Anim01Seq[8];
extern const u16 gSpriteBank23Anim02Seq[1];
extern const u16 gSpriteBank23Anim03Seq[1];
extern const u16 gSpriteBank23Anim04Seq[8];
extern const u16 gSpriteBank23Anim05Seq[1];
extern const u16 gSpriteBank23Anim06Seq[8];
extern const u16 gSpriteBank23Anim07Seq[1];
extern const u16 gSpriteBank23Anim08Seq[1];
extern const u16 gSpriteBank23Anim09Seq[1];
extern const u16 gSpriteBank23Anim10Seq[1];
extern const u16 gSpriteBank23Anim11Seq[1];
extern const u16 gSpriteBank23Anim12Seq[1];
extern const u16 gSpriteBank23Anim13Seq[1];
extern const u16 gSpriteBank23Anim14Seq[1];
extern const u16 gSpriteBank23Anim15Seq[1];
extern const u16 gSpriteBank23Anim16Seq[1];
extern const u16 gSpriteBank23Anim17Seq[1];
extern const u16 gSpriteBank23Anim18Seq[1];
extern const u16 gSpriteBank23Anim19Seq[1];
extern const u16 gSpriteBank23Anim20Seq[1];
extern const u16 gSpriteBank23Anim21Seq[1];
extern const u16 gSpriteBank23Anim22Seq[1];
extern const u16 gSpriteBank23Anim23Seq[1];
extern const u16 gSpriteBank23Anim24Seq[8];
extern const u16 gSpriteBank23Anim25Seq[7];
extern const u16 gSpriteBank23Anim26Seq[1];
extern const u16 gSpriteBank23Anim27Seq[14];
extern const u16 gSpriteBank23Anim28Seq[1];
extern const u16 gSpriteBank23Anim29Seq[9];
extern const u16 gSpriteBank23Anim30Seq[1];
extern const u16 gSpriteBank23Anim31Seq[1];
extern const u16 gSpriteBank23Anim32Seq[1];
extern const u16 gSpriteBank23Anim33Seq[8];
extern const u16 gSpriteBank23Anim34Seq[9];
extern const u16 gSpriteBank23Anim35Seq[9];
extern const u16 gSpriteBank23Anim36Seq[1];
extern const u16 gSpriteBank23Anim37Seq[1];
extern const u16 gSpriteBank23Anim38Seq[1];
extern const u16 gSpriteBank23Anim39Seq[1];
extern const u16 gSpriteBank23Anim40Seq[1];
extern const u16 gSpriteBank23Anim41Seq[8];
extern const u16 gSpriteBank23Anim42Seq[9];

extern const struct sprite_anim gSpriteBank23Anims[43] = {
    /* 0 */ {
        /* seq */ gSpriteBank23Anim00Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 28,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank23Anim01Seq,
        /* box */ { { -12, -6, 19, 19 }, { -30, -49, 63, 62 } },
        /* paletteId */ 29,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank23Anim02Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 30,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank23Anim03Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 30,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim03Seq),
        /* flags */ 0,
    },
    /* 4 */ {
        /* seq */ gSpriteBank23Anim04Seq,
        /* box */ { { -12, -6, 19, 19 }, { -16, -15, 33, 28 } },
        /* paletteId */ 29,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim04Seq),
        /* flags */ 0,
    },
    /* 5 */ {
        /* seq */ gSpriteBank23Anim05Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 29,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim05Seq),
        /* flags */ 0,
    },
    /* 6 */ {
        /* seq */ gSpriteBank23Anim06Seq,
        /* box */ { { -12, -6, 19, 19 }, { -16, -15, 36, 28 } },
        /* paletteId */ 31,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim06Seq),
        /* flags */ 0,
    },
    /* 7 */ {
        /* seq */ gSpriteBank23Anim07Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 28,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim07Seq),
        /* flags */ 0,
    },
    /* 8 */ {
        /* seq */ gSpriteBank23Anim08Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 28,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim08Seq),
        /* flags */ 0,
    },
    /* 9 */ {
        /* seq */ gSpriteBank23Anim09Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 28,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim09Seq),
        /* flags */ 0,
    },
    /* 10 */ {
        /* seq */ gSpriteBank23Anim10Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 28,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim10Seq),
        /* flags */ 0,
    },
    /* 11 */ {
        /* seq */ gSpriteBank23Anim11Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 28,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim11Seq),
        /* flags */ 0,
    },
    /* 12 */ {
        /* seq */ gSpriteBank23Anim12Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 28,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim12Seq),
        /* flags */ 0,
    },
    /* 13 */ {
        /* seq */ gSpriteBank23Anim13Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 28,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim13Seq),
        /* flags */ 0,
    },
    /* 14 */ {
        /* seq */ gSpriteBank23Anim14Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 29,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim14Seq),
        /* flags */ 0,
    },
    /* 15 */ {
        /* seq */ gSpriteBank23Anim15Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 29,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim15Seq),
        /* flags */ 0,
    },
    /* 16 */ {
        /* seq */ gSpriteBank23Anim16Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 29,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim16Seq),
        /* flags */ 0,
    },
    /* 17 */ {
        /* seq */ gSpriteBank23Anim17Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 29,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim17Seq),
        /* flags */ 0,
    },
    /* 18 */ {
        /* seq */ gSpriteBank23Anim18Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 29,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim18Seq),
        /* flags */ 0,
    },
    /* 19 */ {
        /* seq */ gSpriteBank23Anim19Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 29,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim19Seq),
        /* flags */ 0,
    },
    /* 20 */ {
        /* seq */ gSpriteBank23Anim20Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 29,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim20Seq),
        /* flags */ 0,
    },
    /* 21 */ {
        /* seq */ gSpriteBank23Anim21Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 31,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim21Seq),
        /* flags */ 0,
    },
    /* 22 */ {
        /* seq */ gSpriteBank23Anim22Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 28,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim22Seq),
        /* flags */ 0,
    },
    /* 23 */ {
        /* seq */ gSpriteBank23Anim23Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 28,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim23Seq),
        /* flags */ 0,
    },
    /* 24 */ {
        /* seq */ gSpriteBank23Anim24Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -14, 26, 27 } },
        /* paletteId */ 28,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim24Seq),
        /* flags */ 0,
    },
    /* 25 */ {
        /* seq */ gSpriteBank23Anim25Seq,
        /* box */ { { -12, -6, 19, 19 }, { -14, -15, 30, 28 } },
        /* paletteId */ 28,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim25Seq),
        /* flags */ 0,
    },
    /* 26 */ {
        /* seq */ gSpriteBank23Anim26Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 28,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim26Seq),
        /* flags */ 0,
    },
    /* 27 */ {
        /* seq */ gSpriteBank23Anim27Seq,
        /* box */ { { -12, -13, 25, 26 }, { -32, -13, 66, 32 } },
        /* paletteId */ 28,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim27Seq),
        /* flags */ 0,
    },
    /* 28 */ {
        /* seq */ gSpriteBank23Anim28Seq,
        /* box */ { { -12, -6, 19, 19 }, { -14, -13, 27, 26 } },
        /* paletteId */ 28,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim28Seq),
        /* flags */ 0,
    },
    /* 29 */ {
        /* seq */ gSpriteBank23Anim29Seq,
        /* box */ { { -12, -6, 19, 19 }, { -30, -49, 63, 62 } },
        /* paletteId */ 28,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim29Seq),
        /* flags */ 0,
    },
    /* 30 */ {
        /* seq */ gSpriteBank23Anim30Seq,
        /* box */ { { -12, -6, 19, 19 }, { -14, -13, 27, 26 } },
        /* paletteId */ 28,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim30Seq),
        /* flags */ 0,
    },
    /* 31 */ {
        /* seq */ gSpriteBank23Anim31Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 28,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim31Seq),
        /* flags */ 0,
    },
    /* 32 */ {
        /* seq */ gSpriteBank23Anim32Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 30,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim32Seq),
        /* flags */ 0,
    },
    /* 33 */ {
        /* seq */ gSpriteBank23Anim33Seq,
        /* box */ { { -12, -6, 19, 19 }, { -30, -49, 63, 62 } },
        /* paletteId */ 29,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim33Seq),
        /* flags */ 0,
    },
    /* 34 */ {
        /* seq */ gSpriteBank23Anim34Seq,
        /* box */ { { -12, -6, 19, 19 }, { -16, -40, 33, 53 } },
        /* paletteId */ 30,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim34Seq),
        /* flags */ 0,
    },
    /* 35 */ {
        /* seq */ gSpriteBank23Anim35Seq,
        /* box */ { { -12, -6, 19, 19 }, { -16, -40, 33, 53 } },
        /* paletteId */ 29,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim35Seq),
        /* flags */ 0,
    },
    /* 36 */ {
        /* seq */ gSpriteBank23Anim36Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 29,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim36Seq),
        /* flags */ 0,
    },
    /* 37 */ {
        /* seq */ gSpriteBank23Anim37Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 29,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim37Seq),
        /* flags */ 0,
    },
    /* 38 */ {
        /* seq */ gSpriteBank23Anim38Seq,
        /* box */ { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        /* paletteId */ 28,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim38Seq),
        /* flags */ 0,
    },
    /* 39 */ {
        /* seq */ gSpriteBank23Anim39Seq,
        /* box */ { { -13, -6, 20, 24 }, { -12, -13, 25, 26 } },
        /* paletteId */ 28,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim39Seq),
        /* flags */ 0,
    },
    /* 40 */ {
        /* seq */ gSpriteBank23Anim40Seq,
        /* box */ { { -12, -6, 19, 24 }, { -12, -13, 25, 26 } },
        /* paletteId */ 30,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim40Seq),
        /* flags */ 0,
    },
    /* 41 */ {
        /* seq */ gSpriteBank23Anim41Seq,
        /* box */ { { -12, -6, 18, 20 }, { -16, -40, 33, 53 } },
        /* paletteId */ 30,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim41Seq),
        /* flags */ 0,
    },
    /* 42 */ {
        /* seq */ gSpriteBank23Anim42Seq,
        /* box */ { { -12, -7, 19, 21 }, { -30, -49, 63, 62 } },
        /* paletteId */ 32,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank23Anim42Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank23Anim00Seq[1] = {
    0,
};
const u16 gSpriteBank23Anim01Seq[8] = {
    1, 2, 3, 4, 5, 6, 7, 8,
};
const u16 gSpriteBank23Anim02Seq[1] = {
    9,
};
const u16 gSpriteBank23Anim03Seq[1] = {
    10,
};
const u16 gSpriteBank23Anim04Seq[8] = {
    11, 12, 13, 14, 15, 16, 17, 18,
};
const u16 gSpriteBank23Anim05Seq[1] = {
    19,
};
const u16 gSpriteBank23Anim06Seq[8] = {
    20, 21, 22, 23, 24, 25, 26, 27,
};
const u16 gSpriteBank23Anim07Seq[1] = {
    28,
};
const u16 gSpriteBank23Anim08Seq[1] = {
    29,
};
const u16 gSpriteBank23Anim09Seq[1] = {
    30,
};
const u16 gSpriteBank23Anim10Seq[1] = {
    31,
};
const u16 gSpriteBank23Anim11Seq[1] = {
    32,
};
const u16 gSpriteBank23Anim12Seq[1] = {
    33,
};
const u16 gSpriteBank23Anim13Seq[1] = {
    34,
};
const u16 gSpriteBank23Anim14Seq[1] = {
    35,
};
const u16 gSpriteBank23Anim15Seq[1] = {
    36,
};
const u16 gSpriteBank23Anim16Seq[1] = {
    37,
};
const u16 gSpriteBank23Anim17Seq[1] = {
    38,
};
const u16 gSpriteBank23Anim18Seq[1] = {
    39,
};
const u16 gSpriteBank23Anim19Seq[1] = {
    40,
};
const u16 gSpriteBank23Anim20Seq[1] = {
    41,
};
const u16 gSpriteBank23Anim21Seq[1] = {
    42,
};
const u16 gSpriteBank23Anim22Seq[1] = {
    43,
};
const u16 gSpriteBank23Anim23Seq[1] = {
    44,
};
const u16 gSpriteBank23Anim24Seq[8] = {
    45, 46, 47, 48, 49, 50, 51, 52,
};
const u16 gSpriteBank23Anim25Seq[7] = {
    53, 54, 55, 54, 53, 56, 57,
};
const u16 gSpriteBank23Anim26Seq[1] = {
    58,
};
const u16 gSpriteBank23Anim27Seq[14] = {
    59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72,
};
const u16 gSpriteBank23Anim28Seq[1] = {
    73,
};
const u16 gSpriteBank23Anim29Seq[9] = {
    74, 75, 76, 77, 78, 79, 80, 81, 82,
};
const u16 gSpriteBank23Anim30Seq[1] = {
    73,
};
const u16 gSpriteBank23Anim31Seq[1] = {
    43,
};
const u16 gSpriteBank23Anim32Seq[1] = {
    83,
};
const u16 gSpriteBank23Anim33Seq[8] = {
    84, 85, 86, 87, 88, 89, 90, 91,
};
const u16 gSpriteBank23Anim34Seq[9] = {
    92, 93, 94, 95, 96, 97, 98, 99, 100,
};
const u16 gSpriteBank23Anim35Seq[9] = {
    101, 102, 103, 104, 105, 106, 107, 108, 109,
};
const u16 gSpriteBank23Anim36Seq[1] = {
    110,
};
const u16 gSpriteBank23Anim37Seq[1] = {
    111,
};
const u16 gSpriteBank23Anim38Seq[1] = {
    112,
};
const u16 gSpriteBank23Anim39Seq[1] = {
    113,
};
const u16 gSpriteBank23Anim40Seq[1] = {
    114,
};
const u16 gSpriteBank23Anim41Seq[8] = {
    115, 116, 117, 118, 119, 120, 121, 122,
};
const u16 gSpriteBank23Anim42Seq[9] = {
    123, 124, 125, 126, 127, 128, 129, 130, 131,
};

extern const struct sprite_frame *const gSpriteBank23Frames[132] = {
    &gSpriteBank23Frame000.frame,
    &gSpriteBank23Frame001,
    &gSpriteBank23Frame002,
    &gSpriteBank23Frame003,
    &gSpriteBank23Frame004,
    &gSpriteBank23Frame005,
    &gSpriteBank23Frame006,
    &gSpriteBank23Frame007,
    &gSpriteBank23Frame008,
    &gSpriteBank23Frame009.frame,
    &gSpriteBank23Frame010.frame,
    &gSpriteBank23Frame011,
    &gSpriteBank23Frame012,
    &gSpriteBank23Frame013,
    &gSpriteBank23Frame014,
    &gSpriteBank23Frame015,
    &gSpriteBank23Frame016,
    &gSpriteBank23Frame017,
    &gSpriteBank23Frame018,
    &gSpriteBank23Frame019.frame,
    &gSpriteBank23Frame020,
    &gSpriteBank23Frame021,
    &gSpriteBank23Frame022,
    &gSpriteBank23Frame023,
    &gSpriteBank23Frame024,
    &gSpriteBank23Frame025,
    &gSpriteBank23Frame026,
    &gSpriteBank23Frame027,
    &gSpriteBank23Frame028,
    &gSpriteBank23Frame029,
    &gSpriteBank23Frame030,
    &gSpriteBank23Frame031,
    &gSpriteBank23Frame032,
    &gSpriteBank23Frame033,
    &gSpriteBank23Frame034.frame,
    &gSpriteBank23Frame035,
    &gSpriteBank23Frame036,
    &gSpriteBank23Frame037,
    &gSpriteBank23Frame038,
    &gSpriteBank23Frame039,
    &gSpriteBank23Frame040,
    &gSpriteBank23Frame041,
    &gSpriteBank23Frame042,
    &gSpriteBank23Frame043.frame,
    &gSpriteBank23Frame044,
    &gSpriteBank23Frame045,
    &gSpriteBank23Frame046,
    &gSpriteBank23Frame047,
    &gSpriteBank23Frame048,
    &gSpriteBank23Frame049,
    &gSpriteBank23Frame050,
    &gSpriteBank23Frame051,
    &gSpriteBank23Frame052,
    &gSpriteBank23Frame053.frame,
    &gSpriteBank23Frame054.frame,
    &gSpriteBank23Frame055.frame,
    &gSpriteBank23Frame056.frame,
    &gSpriteBank23Frame057.frame,
    &gSpriteBank23Frame058,
    &gSpriteBank23Frame059,
    &gSpriteBank23Frame060,
    &gSpriteBank23Frame061,
    &gSpriteBank23Frame062,
    &gSpriteBank23Frame063,
    &gSpriteBank23Frame064,
    &gSpriteBank23Frame065,
    &gSpriteBank23Frame066,
    &gSpriteBank23Frame067,
    &gSpriteBank23Frame068,
    &gSpriteBank23Frame069,
    &gSpriteBank23Frame070,
    &gSpriteBank23Frame071,
    &gSpriteBank23Frame072,
    &gSpriteBank23Frame073,
    &gSpriteBank23Frame074,
    &gSpriteBank23Frame075,
    &gSpriteBank23Frame076,
    &gSpriteBank23Frame077,
    &gSpriteBank23Frame078,
    &gSpriteBank23Frame079,
    &gSpriteBank23Frame080,
    &gSpriteBank23Frame081,
    &gSpriteBank23Frame082,
    &gSpriteBank23Frame083,
    &gSpriteBank23Frame084,
    &gSpriteBank23Frame085,
    &gSpriteBank23Frame086,
    &gSpriteBank23Frame087,
    &gSpriteBank23Frame088,
    &gSpriteBank23Frame089,
    &gSpriteBank23Frame090,
    &gSpriteBank23Frame091,
    &gSpriteBank23Frame092.frame,
    &gSpriteBank23Frame093.frame,
    &gSpriteBank23Frame094.frame,
    &gSpriteBank23Frame095.frame,
    &gSpriteBank23Frame096.frame,
    &gSpriteBank23Frame097.frame,
    &gSpriteBank23Frame098.frame,
    &gSpriteBank23Frame099.frame,
    &gSpriteBank23Frame100.frame,
    &gSpriteBank23Frame101.frame,
    &gSpriteBank23Frame102.frame,
    &gSpriteBank23Frame103.frame,
    &gSpriteBank23Frame104.frame,
    &gSpriteBank23Frame105.frame,
    &gSpriteBank23Frame106.frame,
    &gSpriteBank23Frame107.frame,
    &gSpriteBank23Frame108.frame,
    &gSpriteBank23Frame109.frame,
    &gSpriteBank23Frame110.frame,
    &gSpriteBank23Frame111,
    &gSpriteBank23Frame112,
    &gSpriteBank23Frame113,
    &gSpriteBank23Frame114,
    &gSpriteBank23Frame115,
    &gSpriteBank23Frame116,
    &gSpriteBank23Frame117,
    &gSpriteBank23Frame118,
    &gSpriteBank23Frame119,
    &gSpriteBank23Frame120,
    &gSpriteBank23Frame121,
    &gSpriteBank23Frame122,
    &gSpriteBank23Frame123,
    &gSpriteBank23Frame124,
    &gSpriteBank23Frame125,
    &gSpriteBank23Frame126,
    &gSpriteBank23Frame127,
    &gSpriteBank23Frame128,
    &gSpriteBank23Frame129,
    &gSpriteBank23Frame130,
    &gSpriteBank23Frame131,
};

const struct sprite_frame_1box gSpriteBank23Frame000 = {
    SPRITE_FRAME(gSpriteBank23Frame000, SPRITE_TILES_BANK23_FRAME000),
    { { -12, -13, 25, 26 } },
};
const struct sprite_frame gSpriteBank23Frame001 = SPRITE_FRAME(gSpriteBank23Frame001, SPRITE_TILES_BANK23_FRAME001);
const struct sprite_frame gSpriteBank23Frame002 = SPRITE_FRAME(gSpriteBank23Frame002, SPRITE_TILES_BANK23_FRAME002);
const struct sprite_frame gSpriteBank23Frame003 = SPRITE_FRAME(gSpriteBank23Frame003, SPRITE_TILES_BANK23_FRAME003);
const struct sprite_frame gSpriteBank23Frame004 = SPRITE_FRAME(gSpriteBank23Frame004, SPRITE_TILES_BANK23_FRAME004);
const struct sprite_frame gSpriteBank23Frame005 = SPRITE_FRAME(gSpriteBank23Frame005, SPRITE_TILES_BANK23_FRAME005);
const struct sprite_frame gSpriteBank23Frame006 = SPRITE_FRAME(gSpriteBank23Frame006, SPRITE_TILES_BANK23_FRAME006);
const struct sprite_frame gSpriteBank23Frame007 = SPRITE_FRAME(gSpriteBank23Frame007, SPRITE_TILES_BANK23_FRAME007);
const struct sprite_frame gSpriteBank23Frame008 = SPRITE_FRAME(gSpriteBank23Frame008, SPRITE_TILES_BANK23_FRAME008);
const struct sprite_frame_1box gSpriteBank23Frame009 = {
    SPRITE_FRAME(gSpriteBank23Frame009, SPRITE_TILES_BANK23_FRAME009),
    { { -12, -13, 25, 26 } },
};
const struct sprite_frame_1box gSpriteBank23Frame010 = {
    SPRITE_FRAME(gSpriteBank23Frame010, SPRITE_TILES_BANK23_FRAME010),
    { { -12, -13, 25, 26 } },
};
const struct sprite_frame gSpriteBank23Frame011 = SPRITE_FRAME(gSpriteBank23Frame011, SPRITE_TILES_BANK23_FRAME011);
const struct sprite_frame gSpriteBank23Frame012 = SPRITE_FRAME(gSpriteBank23Frame012, SPRITE_TILES_BANK23_FRAME012);
const struct sprite_frame gSpriteBank23Frame013 = SPRITE_FRAME(gSpriteBank23Frame013, SPRITE_TILES_BANK23_FRAME013);
const struct sprite_frame gSpriteBank23Frame014 = SPRITE_FRAME(gSpriteBank23Frame014, SPRITE_TILES_BANK23_FRAME014);
const struct sprite_frame gSpriteBank23Frame015 = SPRITE_FRAME(gSpriteBank23Frame015, SPRITE_TILES_BANK23_FRAME015);
const struct sprite_frame gSpriteBank23Frame016 = SPRITE_FRAME(gSpriteBank23Frame016, SPRITE_TILES_BANK23_FRAME016);
const struct sprite_frame gSpriteBank23Frame017 = SPRITE_FRAME(gSpriteBank23Frame017, SPRITE_TILES_BANK23_FRAME017);
const struct sprite_frame gSpriteBank23Frame018 = SPRITE_FRAME(gSpriteBank23Frame018, SPRITE_TILES_BANK23_FRAME018);
const struct sprite_frame_1box gSpriteBank23Frame019 = {
    SPRITE_FRAME(gSpriteBank23Frame019, SPRITE_TILES_BANK23_FRAME019),
    { { -12, -13, 25, 26 } },
};
const struct sprite_frame gSpriteBank23Frame020 = SPRITE_FRAME(gSpriteBank23Frame020, SPRITE_TILES_BANK23_FRAME020);
const struct sprite_frame gSpriteBank23Frame021 = SPRITE_FRAME(gSpriteBank23Frame021, SPRITE_TILES_BANK23_FRAME021);
const struct sprite_frame gSpriteBank23Frame022 = SPRITE_FRAME(gSpriteBank23Frame022, SPRITE_TILES_BANK23_FRAME022);
const struct sprite_frame gSpriteBank23Frame023 = SPRITE_FRAME(gSpriteBank23Frame023, SPRITE_TILES_BANK23_FRAME023);
const struct sprite_frame gSpriteBank23Frame024 = SPRITE_FRAME(gSpriteBank23Frame024, SPRITE_TILES_BANK23_FRAME024);
const struct sprite_frame gSpriteBank23Frame025 = SPRITE_FRAME(gSpriteBank23Frame025, SPRITE_TILES_BANK23_FRAME025);
const struct sprite_frame gSpriteBank23Frame026 = SPRITE_FRAME(gSpriteBank23Frame026, SPRITE_TILES_BANK23_FRAME026);
const struct sprite_frame gSpriteBank23Frame027 = SPRITE_FRAME(gSpriteBank23Frame027, SPRITE_TILES_BANK23_FRAME027);
const struct sprite_frame gSpriteBank23Frame028 = SPRITE_FRAME(gSpriteBank23Frame028, SPRITE_TILES_BANK23_FRAME028);
const struct sprite_frame gSpriteBank23Frame029 = SPRITE_FRAME(gSpriteBank23Frame029, SPRITE_TILES_BANK23_FRAME029);
const struct sprite_frame gSpriteBank23Frame030 = SPRITE_FRAME(gSpriteBank23Frame030, SPRITE_TILES_BANK23_FRAME030);
const struct sprite_frame gSpriteBank23Frame031 = SPRITE_FRAME(gSpriteBank23Frame031, SPRITE_TILES_BANK23_FRAME031);
const struct sprite_frame gSpriteBank23Frame032 = SPRITE_FRAME(gSpriteBank23Frame032, SPRITE_TILES_BANK23_FRAME032);
const struct sprite_frame gSpriteBank23Frame033 = SPRITE_FRAME(gSpriteBank23Frame033, SPRITE_TILES_BANK23_FRAME033);
const struct sprite_frame_1box gSpriteBank23Frame034 = {
    SPRITE_FRAME(gSpriteBank23Frame034, SPRITE_TILES_BANK23_FRAME034),
    { { -12, -13, 25, 26 } },
};
const struct sprite_frame gSpriteBank23Frame035 = SPRITE_FRAME(gSpriteBank23Frame035, SPRITE_TILES_BANK23_FRAME035);
const struct sprite_frame gSpriteBank23Frame036 = SPRITE_FRAME(gSpriteBank23Frame036, SPRITE_TILES_BANK23_FRAME036);
const struct sprite_frame gSpriteBank23Frame037 = SPRITE_FRAME(gSpriteBank23Frame037, SPRITE_TILES_BANK23_FRAME037);
const struct sprite_frame gSpriteBank23Frame038 = SPRITE_FRAME(gSpriteBank23Frame038, SPRITE_TILES_BANK23_FRAME038);
const struct sprite_frame gSpriteBank23Frame039 = SPRITE_FRAME(gSpriteBank23Frame039, SPRITE_TILES_BANK23_FRAME039);
const struct sprite_frame gSpriteBank23Frame040 = SPRITE_FRAME(gSpriteBank23Frame040, SPRITE_TILES_BANK23_FRAME040);
const struct sprite_frame gSpriteBank23Frame041 = SPRITE_FRAME(gSpriteBank23Frame041, SPRITE_TILES_BANK23_FRAME041);
const struct sprite_frame gSpriteBank23Frame042 = SPRITE_FRAME(gSpriteBank23Frame042, SPRITE_TILES_BANK23_FRAME042);
const struct sprite_frame_1box gSpriteBank23Frame043 = {
    SPRITE_FRAME(gSpriteBank23Frame043, SPRITE_TILES_BANK23_FRAME043),
    { { -12, -13, 25, 26 } },
};
const struct sprite_frame gSpriteBank23Frame044 = SPRITE_FRAME(gSpriteBank23Frame044, SPRITE_TILES_BANK23_FRAME044);
const struct sprite_frame gSpriteBank23Frame045 = SPRITE_FRAME(gSpriteBank23Frame045, SPRITE_TILES_BANK23_FRAME045);
const struct sprite_frame gSpriteBank23Frame046 = SPRITE_FRAME(gSpriteBank23Frame046, SPRITE_TILES_BANK23_FRAME046);
const struct sprite_frame gSpriteBank23Frame047 = SPRITE_FRAME(gSpriteBank23Frame047, SPRITE_TILES_BANK23_FRAME047);
const struct sprite_frame gSpriteBank23Frame048 = SPRITE_FRAME(gSpriteBank23Frame048, SPRITE_TILES_BANK23_FRAME048);
const struct sprite_frame gSpriteBank23Frame049 = SPRITE_FRAME(gSpriteBank23Frame049, SPRITE_TILES_BANK23_FRAME049);
const struct sprite_frame gSpriteBank23Frame050 = SPRITE_FRAME(gSpriteBank23Frame050, SPRITE_TILES_BANK23_FRAME050);
const struct sprite_frame gSpriteBank23Frame051 = SPRITE_FRAME(gSpriteBank23Frame051, SPRITE_TILES_BANK23_FRAME051);
const struct sprite_frame gSpriteBank23Frame052 = SPRITE_FRAME(gSpriteBank23Frame052, SPRITE_TILES_BANK23_FRAME052);
const struct sprite_frame_1box gSpriteBank23Frame053 = {
    SPRITE_FRAME(gSpriteBank23Frame053, SPRITE_TILES_BANK23_FRAME053),
    { { -12, -13, 25, 26 } },
};
const struct sprite_frame_1box gSpriteBank23Frame054 = {
    SPRITE_FRAME(gSpriteBank23Frame054, SPRITE_TILES_BANK23_FRAME054),
    { { -14, -9, 30, 22 } },
};
const struct sprite_frame_1box gSpriteBank23Frame055 = {
    SPRITE_FRAME(gSpriteBank23Frame055, SPRITE_TILES_BANK23_FRAME055),
    { { -14, -8, 30, 21 } },
};
const struct sprite_frame_1box gSpriteBank23Frame056 = {
    SPRITE_FRAME(gSpriteBank23Frame056, SPRITE_TILES_BANK23_FRAME056),
    { { -12, -15, 24, 28 } },
};
const struct sprite_frame_1box gSpriteBank23Frame057 = {
    SPRITE_FRAME(gSpriteBank23Frame057, SPRITE_TILES_BANK23_FRAME057),
    { { -12, -13, 26, 26 } },
};
const struct sprite_frame gSpriteBank23Frame058 = SPRITE_FRAME(gSpriteBank23Frame058, SPRITE_TILES_BANK23_FRAME058);
const struct sprite_frame gSpriteBank23Frame059 = SPRITE_FRAME(gSpriteBank23Frame059, SPRITE_TILES_BANK23_FRAME059);
const struct sprite_frame gSpriteBank23Frame060 = SPRITE_FRAME(gSpriteBank23Frame060, SPRITE_TILES_BANK23_FRAME060);
const struct sprite_frame gSpriteBank23Frame061 = SPRITE_FRAME(gSpriteBank23Frame061, SPRITE_TILES_BANK23_FRAME061);
const struct sprite_frame gSpriteBank23Frame062 = SPRITE_FRAME(gSpriteBank23Frame062, SPRITE_TILES_BANK23_FRAME062);
const struct sprite_frame gSpriteBank23Frame063 = SPRITE_FRAME(gSpriteBank23Frame063, SPRITE_TILES_BANK23_FRAME063);
const struct sprite_frame gSpriteBank23Frame064 = SPRITE_FRAME(gSpriteBank23Frame064, SPRITE_TILES_BANK23_FRAME064);
const struct sprite_frame gSpriteBank23Frame065 = SPRITE_FRAME(gSpriteBank23Frame065, SPRITE_TILES_BANK23_FRAME065);
const struct sprite_frame gSpriteBank23Frame066 = SPRITE_FRAME(gSpriteBank23Frame066, SPRITE_TILES_BANK23_FRAME066);
const struct sprite_frame gSpriteBank23Frame067 = SPRITE_FRAME(gSpriteBank23Frame067, SPRITE_TILES_BANK23_FRAME067);
const struct sprite_frame gSpriteBank23Frame068 = SPRITE_FRAME(gSpriteBank23Frame068, SPRITE_TILES_BANK23_FRAME068);
const struct sprite_frame gSpriteBank23Frame069 = SPRITE_FRAME(gSpriteBank23Frame069, SPRITE_TILES_BANK23_FRAME069);
const struct sprite_frame gSpriteBank23Frame070 = SPRITE_FRAME(gSpriteBank23Frame070, SPRITE_TILES_BANK23_FRAME070);
const struct sprite_frame gSpriteBank23Frame071 = SPRITE_FRAME(gSpriteBank23Frame071, SPRITE_TILES_BANK23_FRAME071);
const struct sprite_frame gSpriteBank23Frame072 = SPRITE_FRAME(gSpriteBank23Frame072, SPRITE_TILES_BANK23_FRAME072);
const struct sprite_frame gSpriteBank23Frame073 = SPRITE_FRAME(gSpriteBank23Frame073, SPRITE_TILES_BANK23_FRAME073);
const struct sprite_frame gSpriteBank23Frame074 = SPRITE_FRAME(gSpriteBank23Frame074, SPRITE_TILES_BANK23_FRAME074);
const struct sprite_frame gSpriteBank23Frame075 = SPRITE_FRAME(gSpriteBank23Frame075, SPRITE_TILES_BANK23_FRAME075);
const struct sprite_frame gSpriteBank23Frame076 = SPRITE_FRAME(gSpriteBank23Frame076, SPRITE_TILES_BANK23_FRAME076);
const struct sprite_frame gSpriteBank23Frame077 = SPRITE_FRAME(gSpriteBank23Frame077, SPRITE_TILES_BANK23_FRAME077);
const struct sprite_frame gSpriteBank23Frame078 = SPRITE_FRAME(gSpriteBank23Frame078, SPRITE_TILES_BANK23_FRAME078);
const struct sprite_frame gSpriteBank23Frame079 = SPRITE_FRAME(gSpriteBank23Frame079, SPRITE_TILES_BANK23_FRAME079);
const struct sprite_frame gSpriteBank23Frame080 = SPRITE_FRAME(gSpriteBank23Frame080, SPRITE_TILES_BANK23_FRAME080);
const struct sprite_frame gSpriteBank23Frame081 = SPRITE_FRAME(gSpriteBank23Frame081, SPRITE_TILES_BANK23_FRAME081);
const struct sprite_frame gSpriteBank23Frame082 = SPRITE_FRAME(gSpriteBank23Frame082, SPRITE_TILES_BANK23_FRAME082);
const struct sprite_frame gSpriteBank23Frame083 = SPRITE_FRAME(gSpriteBank23Frame083, SPRITE_TILES_BANK23_FRAME083);
const struct sprite_frame gSpriteBank23Frame084 = SPRITE_FRAME(gSpriteBank23Frame084, SPRITE_TILES_BANK23_FRAME084);
const struct sprite_frame gSpriteBank23Frame085 = SPRITE_FRAME(gSpriteBank23Frame085, SPRITE_TILES_BANK23_FRAME085);
const struct sprite_frame gSpriteBank23Frame086 = SPRITE_FRAME(gSpriteBank23Frame086, SPRITE_TILES_BANK23_FRAME086);
const struct sprite_frame gSpriteBank23Frame087 = SPRITE_FRAME(gSpriteBank23Frame087, SPRITE_TILES_BANK23_FRAME087);
const struct sprite_frame gSpriteBank23Frame088 = SPRITE_FRAME(gSpriteBank23Frame088, SPRITE_TILES_BANK23_FRAME088);
const struct sprite_frame gSpriteBank23Frame089 = SPRITE_FRAME(gSpriteBank23Frame089, SPRITE_TILES_BANK23_FRAME089);
const struct sprite_frame gSpriteBank23Frame090 = SPRITE_FRAME(gSpriteBank23Frame090, SPRITE_TILES_BANK23_FRAME090);
const struct sprite_frame gSpriteBank23Frame091 = SPRITE_FRAME(gSpriteBank23Frame091, SPRITE_TILES_BANK23_FRAME091);
const struct sprite_frame_1box gSpriteBank23Frame092 = {
    SPRITE_FRAME(gSpriteBank23Frame092, SPRITE_TILES_BANK23_FRAME092),
    { { -12, -13, 26, 26 } },
};
const struct sprite_frame_1box gSpriteBank23Frame093 = {
    SPRITE_FRAME(gSpriteBank23Frame093, SPRITE_TILES_BANK23_FRAME093),
    { { -15, -11, 31, 24 } },
};
const struct sprite_frame_1box gSpriteBank23Frame094 = {
    SPRITE_FRAME(gSpriteBank23Frame094, SPRITE_TILES_BANK23_FRAME094),
    { { -16, -21, 33, 34 } },
};
const struct sprite_frame_1box gSpriteBank23Frame095 = {
    SPRITE_FRAME(gSpriteBank23Frame095, SPRITE_TILES_BANK23_FRAME095),
    { { -15, -29, 31, 42 } },
};
const struct sprite_frame_1box gSpriteBank23Frame096 = {
    SPRITE_FRAME(gSpriteBank23Frame096, SPRITE_TILES_BANK23_FRAME096),
    { { -12, -31, 25, 44 } },
};
const struct sprite_frame_1box gSpriteBank23Frame097 = {
    SPRITE_FRAME(gSpriteBank23Frame097, SPRITE_TILES_BANK23_FRAME097),
    { { -12, -35, 25, 48 } },
};
const struct sprite_frame_1box gSpriteBank23Frame098 = {
    SPRITE_FRAME(gSpriteBank23Frame098, SPRITE_TILES_BANK23_FRAME098),
    { { -13, -39, 26, 52 } },
};
const struct sprite_frame_1box gSpriteBank23Frame099 = {
    SPRITE_FRAME(gSpriteBank23Frame099, SPRITE_TILES_BANK23_FRAME099),
    { { -13, -40, 26, 53 } },
};
const struct sprite_frame_1box gSpriteBank23Frame100 = {
    SPRITE_FRAME(gSpriteBank23Frame100, SPRITE_TILES_BANK23_FRAME100),
    { { -12, -13, 26, 26 } },
};
const struct sprite_frame_1box gSpriteBank23Frame101 = {
    SPRITE_FRAME(gSpriteBank23Frame101, SPRITE_TILES_BANK23_FRAME101),
    { { -12, -13, 25, 26 } },
};
const struct sprite_frame_1box gSpriteBank23Frame102 = {
    SPRITE_FRAME(gSpriteBank23Frame102, SPRITE_TILES_BANK23_FRAME102),
    { { -15, -11, 31, 24 } },
};
const struct sprite_frame_1box gSpriteBank23Frame103 = {
    SPRITE_FRAME(gSpriteBank23Frame103, SPRITE_TILES_BANK23_FRAME103),
    { { -16, -21, 33, 34 } },
};
const struct sprite_frame_1box gSpriteBank23Frame104 = {
    SPRITE_FRAME(gSpriteBank23Frame104, SPRITE_TILES_BANK23_FRAME104),
    { { -15, -29, 31, 42 } },
};
const struct sprite_frame_1box gSpriteBank23Frame105 = {
    SPRITE_FRAME(gSpriteBank23Frame105, SPRITE_TILES_BANK23_FRAME105),
    { { -12, -31, 25, 44 } },
};
const struct sprite_frame_1box gSpriteBank23Frame106 = {
    SPRITE_FRAME(gSpriteBank23Frame106, SPRITE_TILES_BANK23_FRAME106),
    { { -12, -35, 25, 48 } },
};
const struct sprite_frame_1box gSpriteBank23Frame107 = {
    SPRITE_FRAME(gSpriteBank23Frame107, SPRITE_TILES_BANK23_FRAME107),
    { { -13, -39, 26, 52 } },
};
const struct sprite_frame_1box gSpriteBank23Frame108 = {
    SPRITE_FRAME(gSpriteBank23Frame108, SPRITE_TILES_BANK23_FRAME108),
    { { -13, -40, 26, 53 } },
};
const struct sprite_frame_1box gSpriteBank23Frame109 = {
    SPRITE_FRAME(gSpriteBank23Frame109, SPRITE_TILES_BANK23_FRAME109),
    { { -12, -13, 25, 26 } },
};
const struct sprite_frame_1box gSpriteBank23Frame110 = {
    SPRITE_FRAME(gSpriteBank23Frame110, SPRITE_TILES_BANK23_FRAME110),
    { { -12, -13, 25, 26 } },
};
const struct sprite_frame gSpriteBank23Frame111 = SPRITE_FRAME(gSpriteBank23Frame111, SPRITE_TILES_BANK23_FRAME111);
const struct sprite_frame gSpriteBank23Frame112 = SPRITE_FRAME(gSpriteBank23Frame112, SPRITE_TILES_BANK23_FRAME112);
const struct sprite_frame gSpriteBank23Frame113 = SPRITE_FRAME(gSpriteBank23Frame113, SPRITE_TILES_BANK23_FRAME113);
const struct sprite_frame gSpriteBank23Frame114 = SPRITE_FRAME(gSpriteBank23Frame114, SPRITE_TILES_BANK23_FRAME114);
const struct sprite_frame gSpriteBank23Frame115 = SPRITE_FRAME(gSpriteBank23Frame115, SPRITE_TILES_BANK23_FRAME115);
const struct sprite_frame gSpriteBank23Frame116 = SPRITE_FRAME(gSpriteBank23Frame116, SPRITE_TILES_BANK23_FRAME116);
const struct sprite_frame gSpriteBank23Frame117 = SPRITE_FRAME(gSpriteBank23Frame117, SPRITE_TILES_BANK23_FRAME117);
const struct sprite_frame gSpriteBank23Frame118 = SPRITE_FRAME(gSpriteBank23Frame118, SPRITE_TILES_BANK23_FRAME118);
const struct sprite_frame gSpriteBank23Frame119 = SPRITE_FRAME(gSpriteBank23Frame119, SPRITE_TILES_BANK23_FRAME119);
const struct sprite_frame gSpriteBank23Frame120 = SPRITE_FRAME(gSpriteBank23Frame120, SPRITE_TILES_BANK23_FRAME120);
const struct sprite_frame gSpriteBank23Frame121 = SPRITE_FRAME(gSpriteBank23Frame121, SPRITE_TILES_BANK23_FRAME121);
const struct sprite_frame gSpriteBank23Frame122 = SPRITE_FRAME(gSpriteBank23Frame122, SPRITE_TILES_BANK23_FRAME122);
const struct sprite_frame gSpriteBank23Frame123 = SPRITE_FRAME(gSpriteBank23Frame123, SPRITE_TILES_BANK23_FRAME123);
const struct sprite_frame gSpriteBank23Frame124 = SPRITE_FRAME(gSpriteBank23Frame124, SPRITE_TILES_BANK23_FRAME124);
const struct sprite_frame gSpriteBank23Frame125 = SPRITE_FRAME(gSpriteBank23Frame125, SPRITE_TILES_BANK23_FRAME125);
const struct sprite_frame gSpriteBank23Frame126 = SPRITE_FRAME(gSpriteBank23Frame126, SPRITE_TILES_BANK23_FRAME126);
const struct sprite_frame gSpriteBank23Frame127 = SPRITE_FRAME(gSpriteBank23Frame127, SPRITE_TILES_BANK23_FRAME127);
const struct sprite_frame gSpriteBank23Frame128 = SPRITE_FRAME(gSpriteBank23Frame128, SPRITE_TILES_BANK23_FRAME128);
const struct sprite_frame gSpriteBank23Frame129 = SPRITE_FRAME(gSpriteBank23Frame129, SPRITE_TILES_BANK23_FRAME129);
const struct sprite_frame gSpriteBank23Frame130 = SPRITE_FRAME(gSpriteBank23Frame130, SPRITE_TILES_BANK23_FRAME130);
const struct sprite_frame gSpriteBank23Frame131 = SPRITE_FRAME(gSpriteBank23Frame131, SPRITE_TILES_BANK23_FRAME131);

/* The piece arrays, from games/n2/graphics/sprites/bank23_crates/bank.json. */
#include "sprites/bank23_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 24: 7 animations, 24 frames, games/n2/graphics/sprites/bank24_gems/. */

extern const u16 gSpriteBank24Anim00Seq[8];
extern const u16 gSpriteBank24Anim01Seq[8];
extern const u16 gSpriteBank24Anim02Seq[8];
extern const u16 gSpriteBank24Anim03Seq[8];
extern const u16 gSpriteBank24Anim04Seq[8];
extern const u16 gSpriteBank24Anim05Seq[8];
extern const u16 gSpriteBank24Anim06Seq[8];

extern const struct sprite_anim gSpriteBank24Anims[7] = {
    /* 0 */ {
        /* seq */ gSpriteBank24Anim00Seq,
        /* box */ { { -12, -11, 25, 23 }, { -12, -11, 25, 23 } },
        /* paletteId */ 13,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank24Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank24Anim01Seq,
        /* box */ { { -13, -12, 24, 25 }, { -13, -11, 24, 23 } },
        /* paletteId */ 32,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank24Anim01Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 2 */ {
        /* seq */ gSpriteBank24Anim02Seq,
        /* box */ { { -13, -12, 24, 25 }, { -13, -11, 24, 23 } },
        /* paletteId */ 33,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank24Anim02Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 3 */ {
        /* seq */ gSpriteBank24Anim03Seq,
        /* box */ { { -13, -12, 24, 25 }, { -13, -11, 24, 23 } },
        /* paletteId */ 34,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank24Anim03Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 4 */ {
        /* seq */ gSpriteBank24Anim04Seq,
        /* box */ { { -12, -11, 25, 23 }, { -12, -11, 25, 23 } },
        /* paletteId */ 34,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank24Anim04Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 5 */ {
        /* seq */ gSpriteBank24Anim05Seq,
        /* box */ { { -12, -11, 25, 23 }, { -12, -11, 25, 23 } },
        /* paletteId */ 33,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank24Anim05Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 6 */ {
        /* seq */ gSpriteBank24Anim06Seq,
        /* box */ { { -12, -11, 25, 23 }, { -12, -11, 25, 23 } },
        /* paletteId */ 32,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank24Anim06Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank24Anim00Seq[8] = {
    0, 1, 2, 3, 4, 5, 6, 7,
};
const u16 gSpriteBank24Anim01Seq[8] = {
    8, 9, 10, 11, 12, 13, 14, 15,
};
const u16 gSpriteBank24Anim02Seq[8] = {
    8, 9, 10, 11, 12, 13, 14, 15,
};
const u16 gSpriteBank24Anim03Seq[8] = {
    8, 9, 10, 11, 12, 13, 14, 15,
};
const u16 gSpriteBank24Anim04Seq[8] = {
    16, 17, 18, 19, 20, 21, 22, 23,
};
const u16 gSpriteBank24Anim05Seq[8] = {
    16, 17, 18, 19, 20, 21, 22, 23,
};
const u16 gSpriteBank24Anim06Seq[8] = {
    16, 17, 18, 19, 20, 21, 22, 23,
};

extern const struct sprite_frame *const gSpriteBank24Frames[24] = {
    &gSpriteBank24Frame000.frame,
    &gSpriteBank24Frame001.frame,
    &gSpriteBank24Frame002.frame,
    &gSpriteBank24Frame003.frame,
    &gSpriteBank24Frame004.frame,
    &gSpriteBank24Frame005.frame,
    &gSpriteBank24Frame006.frame,
    &gSpriteBank24Frame007.frame,
    &gSpriteBank24Frame008,
    &gSpriteBank24Frame009,
    &gSpriteBank24Frame010,
    &gSpriteBank24Frame011,
    &gSpriteBank24Frame012,
    &gSpriteBank24Frame013,
    &gSpriteBank24Frame014,
    &gSpriteBank24Frame015,
    &gSpriteBank24Frame016,
    &gSpriteBank24Frame017,
    &gSpriteBank24Frame018,
    &gSpriteBank24Frame019,
    &gSpriteBank24Frame020,
    &gSpriteBank24Frame021,
    &gSpriteBank24Frame022,
    &gSpriteBank24Frame023,
};

const struct sprite_frame_1box gSpriteBank24Frame000 = {
    SPRITE_FRAME(gSpriteBank24Frame000, SPRITE_TILES_BANK24_FRAME000),
    { { -12, -11, 25, 23 } },
};
const struct sprite_frame_1box gSpriteBank24Frame001 = {
    SPRITE_FRAME(gSpriteBank24Frame001, SPRITE_TILES_BANK24_FRAME001),
    { { -12, -11, 25, 23 } },
};
const struct sprite_frame_1box gSpriteBank24Frame002 = {
    SPRITE_FRAME(gSpriteBank24Frame002, SPRITE_TILES_BANK24_FRAME002),
    { { -12, -11, 25, 23 } },
};
const struct sprite_frame_1box gSpriteBank24Frame003 = {
    SPRITE_FRAME(gSpriteBank24Frame003, SPRITE_TILES_BANK24_FRAME003),
    { { -12, -11, 25, 23 } },
};
const struct sprite_frame_1box gSpriteBank24Frame004 = {
    SPRITE_FRAME(gSpriteBank24Frame004, SPRITE_TILES_BANK24_FRAME004),
    { { -12, -11, 25, 23 } },
};
const struct sprite_frame_1box gSpriteBank24Frame005 = {
    SPRITE_FRAME(gSpriteBank24Frame005, SPRITE_TILES_BANK24_FRAME005),
    { { -12, -11, 25, 23 } },
};
const struct sprite_frame_1box gSpriteBank24Frame006 = {
    SPRITE_FRAME(gSpriteBank24Frame006, SPRITE_TILES_BANK24_FRAME006),
    { { -12, -11, 25, 23 } },
};
const struct sprite_frame_1box gSpriteBank24Frame007 = {
    SPRITE_FRAME(gSpriteBank24Frame007, SPRITE_TILES_BANK24_FRAME007),
    { { -12, -11, 25, 23 } },
};
const struct sprite_frame gSpriteBank24Frame008 = SPRITE_FRAME(gSpriteBank24Frame008, SPRITE_TILES_BANK24_FRAME008);
const struct sprite_frame gSpriteBank24Frame009 = SPRITE_FRAME(gSpriteBank24Frame009, SPRITE_TILES_BANK24_FRAME009);
const struct sprite_frame gSpriteBank24Frame010 = SPRITE_FRAME(gSpriteBank24Frame010, SPRITE_TILES_BANK24_FRAME010);
const struct sprite_frame gSpriteBank24Frame011 = SPRITE_FRAME(gSpriteBank24Frame011, SPRITE_TILES_BANK24_FRAME011);
const struct sprite_frame gSpriteBank24Frame012 = SPRITE_FRAME(gSpriteBank24Frame012, SPRITE_TILES_BANK24_FRAME012);
const struct sprite_frame gSpriteBank24Frame013 = SPRITE_FRAME(gSpriteBank24Frame013, SPRITE_TILES_BANK24_FRAME013);
const struct sprite_frame gSpriteBank24Frame014 = SPRITE_FRAME(gSpriteBank24Frame014, SPRITE_TILES_BANK24_FRAME014);
const struct sprite_frame gSpriteBank24Frame015 = SPRITE_FRAME(gSpriteBank24Frame015, SPRITE_TILES_BANK24_FRAME015);
const struct sprite_frame gSpriteBank24Frame016 = SPRITE_FRAME(gSpriteBank24Frame016, SPRITE_TILES_BANK24_FRAME016);
const struct sprite_frame gSpriteBank24Frame017 = SPRITE_FRAME(gSpriteBank24Frame017, SPRITE_TILES_BANK24_FRAME017);
const struct sprite_frame gSpriteBank24Frame018 = SPRITE_FRAME(gSpriteBank24Frame018, SPRITE_TILES_BANK24_FRAME018);
const struct sprite_frame gSpriteBank24Frame019 = SPRITE_FRAME(gSpriteBank24Frame019, SPRITE_TILES_BANK24_FRAME019);
const struct sprite_frame gSpriteBank24Frame020 = SPRITE_FRAME(gSpriteBank24Frame020, SPRITE_TILES_BANK24_FRAME020);
const struct sprite_frame gSpriteBank24Frame021 = SPRITE_FRAME(gSpriteBank24Frame021, SPRITE_TILES_BANK24_FRAME021);
const struct sprite_frame gSpriteBank24Frame022 = SPRITE_FRAME(gSpriteBank24Frame022, SPRITE_TILES_BANK24_FRAME022);
const struct sprite_frame gSpriteBank24Frame023 = SPRITE_FRAME(gSpriteBank24Frame023, SPRITE_TILES_BANK24_FRAME023);

/* The piece arrays, from games/n2/graphics/sprites/bank24_gems/bank.json. */
#include "sprites/bank24_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 25: 3 animations, 16 frames, games/n2/graphics/sprites/bank25_trial_icons/. */

extern const u16 gSpriteBank25Anim00Seq[8];
extern const u16 gSpriteBank25Anim01Seq[8];
extern const u16 gSpriteBank25Anim02Seq[8];

extern const struct sprite_anim gSpriteBank25Anims[3] = {
    /* 0 */ {
        /* seq */ gSpriteBank25Anim00Seq,
        /* box */ { { -14, -22, 28, 45 }, { -15, -22, 29, 45 } },
        /* paletteId */ 35,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank25Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank25Anim01Seq,
        /* box */ { { -14, -22, 28, 45 }, { -15, -22, 29, 45 } },
        /* paletteId */ 36,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank25Anim01Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 2 */ {
        /* seq */ gSpriteBank25Anim02Seq,
        /* box */ { { -14, -22, 28, 45 }, { -15, -22, 29, 45 } },
        /* paletteId */ 37,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank25Anim02Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank25Anim00Seq[8] = {
    0, 1, 2, 3, 4, 5, 6, 7,
};
const u16 gSpriteBank25Anim01Seq[8] = {
    8, 9, 10, 11, 12, 13, 14, 15,
};
const u16 gSpriteBank25Anim02Seq[8] = {
    8, 9, 10, 11, 12, 13, 14, 15,
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
    { { -14, -22, 28, 45 } },
};
const struct sprite_frame_1box gSpriteBank25Frame001 = {
    SPRITE_FRAME(gSpriteBank25Frame001, SPRITE_TILES_BANK25_FRAME001),
    { { -15, -22, 29, 45 } },
};
const struct sprite_frame_1box gSpriteBank25Frame002 = {
    SPRITE_FRAME(gSpriteBank25Frame002, SPRITE_TILES_BANK25_FRAME002),
    { { -15, -22, 27, 45 } },
};
const struct sprite_frame_1box gSpriteBank25Frame003 = {
    SPRITE_FRAME(gSpriteBank25Frame003, SPRITE_TILES_BANK25_FRAME003),
    { { -12, -22, 21, 45 } },
};
const struct sprite_frame_1box gSpriteBank25Frame004 = {
    SPRITE_FRAME(gSpriteBank25Frame004, SPRITE_TILES_BANK25_FRAME004),
    { { -8, -22, 13, 44 } },
};
const struct sprite_frame_1box gSpriteBank25Frame005 = {
    SPRITE_FRAME(gSpriteBank25Frame005, SPRITE_TILES_BANK25_FRAME005),
    { { -3, -22, 7, 44 } },
};
const struct sprite_frame_1box gSpriteBank25Frame006 = {
    SPRITE_FRAME(gSpriteBank25Frame006, SPRITE_TILES_BANK25_FRAME006),
    { { -8, -22, 17, 45 } },
};
const struct sprite_frame_1box gSpriteBank25Frame007 = {
    SPRITE_FRAME(gSpriteBank25Frame007, SPRITE_TILES_BANK25_FRAME007),
    { { -12, -22, 25, 45 } },
};
const struct sprite_frame_1box gSpriteBank25Frame008 = {
    SPRITE_FRAME(gSpriteBank25Frame008, SPRITE_TILES_BANK25_FRAME008),
    { { -14, -22, 28, 45 } },
};
const struct sprite_frame_1box gSpriteBank25Frame009 = {
    SPRITE_FRAME(gSpriteBank25Frame009, SPRITE_TILES_BANK25_FRAME009),
    { { -15, -22, 29, 45 } },
};
const struct sprite_frame_1box gSpriteBank25Frame010 = {
    SPRITE_FRAME(gSpriteBank25Frame010, SPRITE_TILES_BANK25_FRAME010),
    { { -15, -22, 27, 45 } },
};
const struct sprite_frame_1box gSpriteBank25Frame011 = {
    SPRITE_FRAME(gSpriteBank25Frame011, SPRITE_TILES_BANK25_FRAME011),
    { { -12, -22, 21, 45 } },
};
const struct sprite_frame_1box gSpriteBank25Frame012 = {
    SPRITE_FRAME(gSpriteBank25Frame012, SPRITE_TILES_BANK25_FRAME012),
    { { -8, -22, 13, 44 } },
};
const struct sprite_frame_1box gSpriteBank25Frame013 = {
    SPRITE_FRAME(gSpriteBank25Frame013, SPRITE_TILES_BANK25_FRAME013),
    { { -3, -22, 7, 44 } },
};
const struct sprite_frame_1box gSpriteBank25Frame014 = {
    SPRITE_FRAME(gSpriteBank25Frame014, SPRITE_TILES_BANK25_FRAME014),
    { { -8, -22, 17, 45 } },
};
const struct sprite_frame_1box gSpriteBank25Frame015 = {
    SPRITE_FRAME(gSpriteBank25Frame015, SPRITE_TILES_BANK25_FRAME015),
    { { -12, -22, 25, 45 } },
};

/* The piece arrays, from games/n2/graphics/sprites/bank25_trial_icons/bank.json. */
#include "sprites/bank25_pieces.inc"
