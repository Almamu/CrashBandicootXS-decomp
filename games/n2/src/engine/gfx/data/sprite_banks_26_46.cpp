extern "C" {
#include "gba/types.h"
#include "engine/gfx/sprite_bank.h"
}
#include "sprites/sprite_frames.h"

/*
 * ROM 0x08485d00-0x08492908: sprite banks 26-46 of the sprite-bank
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

/* ---------------------------------------------------------------------- */
/* Bank 26: 7 animations, 31 frames, games/n2/graphics/sprites/bank26_aku_aku/. */

extern const u16 gSpriteBank26Anim00Seq[4];
extern const u16 gSpriteBank26Anim01Seq[4];
extern const u16 gSpriteBank26Anim02Seq[4];
extern const u16 gSpriteBank26Anim03Seq[5];
extern const u16 gSpriteBank26Anim04Seq[5];
extern const u16 gSpriteBank26Anim05Seq[5];
extern const u16 gSpriteBank26Anim06Seq[4];

extern const struct sprite_anim gSpriteBank26Anims[7] = {
    /* 0 */ {
        /* seq */ gSpriteBank26Anim00Seq,
        /* box */ { { -4, -9, 9, 19 }, { -8, -11, 15, 21 } },
        /* paletteId */ 38,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank26Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank26Anim01Seq,
        /* box */ { { -4, -9, 9, 19 }, { -8, -11, 15, 21 } },
        /* paletteId */ 38,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank26Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank26Anim02Seq,
        /* box */ { { -4, -9, 9, 19 }, { -8, -11, 15, 21 } },
        /* paletteId */ 38,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank26Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank26Anim03Seq,
        /* box */ { { -8, -4, 16, 9 }, { -15, -13, 30, 30 } },
        /* paletteId */ 38,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank26Anim03Seq),
        /* flags */ 0,
    },
    /* 4 */ {
        /* seq */ gSpriteBank26Anim04Seq,
        /* box */ { { -8, -4, 16, 9 }, { -15, -13, 30, 30 } },
        /* paletteId */ 38,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank26Anim04Seq),
        /* flags */ 0,
    },
    /* 5 */ {
        /* seq */ gSpriteBank26Anim05Seq,
        /* box */ { { -4, -5, 9, 10 }, { -5, -5, 12, 20 } },
        /* paletteId */ 38,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank26Anim05Seq),
        /* flags */ 0,
    },
    /* 6 */ {
        /* seq */ gSpriteBank26Anim06Seq,
        /* box */ { { -3, -8, 6, 16 }, { -3, -8, 8, 16 } },
        /* paletteId */ 38,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank26Anim06Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank26Anim00Seq[4] = {
    0, 1, 2, 3,
};
const u16 gSpriteBank26Anim01Seq[4] = {
    4, 5, 6, 7,
};
const u16 gSpriteBank26Anim02Seq[4] = {
    8, 9, 10, 11,
};
const u16 gSpriteBank26Anim03Seq[5] = {
    12, 13, 14, 15, 16,
};
const u16 gSpriteBank26Anim04Seq[5] = {
    17, 18, 19, 20, 21,
};
const u16 gSpriteBank26Anim05Seq[5] = {
    22, 23, 24, 25, 26,
};
const u16 gSpriteBank26Anim06Seq[4] = {
    27, 28, 29, 30,
};

extern const struct sprite_frame *const gSpriteBank26Frames[31] = {
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
    &gSpriteBank26Frame014.frame,
    &gSpriteBank26Frame015.frame,
    &gSpriteBank26Frame016.frame,
    &gSpriteBank26Frame017.frame,
    &gSpriteBank26Frame018.frame,
    &gSpriteBank26Frame019.frame,
    &gSpriteBank26Frame020.frame,
    &gSpriteBank26Frame021.frame,
    &gSpriteBank26Frame022,
    &gSpriteBank26Frame023,
    &gSpriteBank26Frame024,
    &gSpriteBank26Frame025,
    &gSpriteBank26Frame026,
    &gSpriteBank26Frame027,
    &gSpriteBank26Frame028,
    &gSpriteBank26Frame029,
    &gSpriteBank26Frame030,
};

const struct sprite_frame_1box gSpriteBank26Frame000 = {
    SPRITE_FRAME(gSpriteBank26Frame000, SPRITE_TILES_BANK26_FRAME000),
    { { -4, -9, 9, 19 } },
};
const struct sprite_frame_1box gSpriteBank26Frame001 = {
    SPRITE_FRAME(gSpriteBank26Frame001, SPRITE_TILES_BANK26_FRAME001),
    { { -5, -9, 12, 19 } },
};
const struct sprite_frame_1box gSpriteBank26Frame002 = {
    SPRITE_FRAME(gSpriteBank26Frame002, SPRITE_TILES_BANK26_FRAME002),
    { { -8, -11, 12, 21 } },
};
const struct sprite_frame_1box gSpriteBank26Frame003 = {
    SPRITE_FRAME(gSpriteBank26Frame003, SPRITE_TILES_BANK26_FRAME003),
    { { -8, -11, 10, 21 } },
};
const struct sprite_frame_1box gSpriteBank26Frame004 = {
    SPRITE_FRAME(gSpriteBank26Frame004, SPRITE_TILES_BANK26_FRAME004),
    { { -4, -9, 9, 19 } },
};
const struct sprite_frame_1box gSpriteBank26Frame005 = {
    SPRITE_FRAME(gSpriteBank26Frame005, SPRITE_TILES_BANK26_FRAME005),
    { { -5, -9, 12, 19 } },
};
const struct sprite_frame_1box gSpriteBank26Frame006 = {
    SPRITE_FRAME(gSpriteBank26Frame006, SPRITE_TILES_BANK26_FRAME006),
    { { -8, -11, 12, 21 } },
};
const struct sprite_frame_1box gSpriteBank26Frame007 = {
    SPRITE_FRAME(gSpriteBank26Frame007, SPRITE_TILES_BANK26_FRAME007),
    { { -8, -11, 10, 21 } },
};
const struct sprite_frame_1box gSpriteBank26Frame008 = {
    SPRITE_FRAME(gSpriteBank26Frame008, SPRITE_TILES_BANK26_FRAME008),
    { { -4, -9, 9, 19 } },
};
const struct sprite_frame_1box gSpriteBank26Frame009 = {
    SPRITE_FRAME(gSpriteBank26Frame009, SPRITE_TILES_BANK26_FRAME009),
    { { -5, -9, 12, 19 } },
};
const struct sprite_frame_1box gSpriteBank26Frame010 = {
    SPRITE_FRAME(gSpriteBank26Frame010, SPRITE_TILES_BANK26_FRAME010),
    { { -8, -11, 12, 21 } },
};
const struct sprite_frame_1box gSpriteBank26Frame011 = {
    SPRITE_FRAME(gSpriteBank26Frame011, SPRITE_TILES_BANK26_FRAME011),
    { { -8, -11, 10, 21 } },
};
const struct sprite_frame_1box gSpriteBank26Frame012 = {
    SPRITE_FRAME(gSpriteBank26Frame012, SPRITE_TILES_BANK26_FRAME012),
    { { -8, -4, 16, 9 } },
};
const struct sprite_frame_1box gSpriteBank26Frame013 = {
    SPRITE_FRAME(gSpriteBank26Frame013, SPRITE_TILES_BANK26_FRAME013),
    { { -11, 1, 22, 10 } },
};
const struct sprite_frame_1box gSpriteBank26Frame014 = {
    SPRITE_FRAME(gSpriteBank26Frame014, SPRITE_TILES_BANK26_FRAME014),
    { { -10, 7, 19, 7 } },
};
const struct sprite_frame_1box gSpriteBank26Frame015 = {
    SPRITE_FRAME(gSpriteBank26Frame015, SPRITE_TILES_BANK26_FRAME015),
    { { -8, 10, 19, 8 } },
};
const struct sprite_frame_1box gSpriteBank26Frame016 = {
    SPRITE_FRAME(gSpriteBank26Frame016, SPRITE_TILES_BANK26_FRAME016),
    { { -8, 13, 15, 4 } },
};
const struct sprite_frame_1box gSpriteBank26Frame017 = {
    SPRITE_FRAME(gSpriteBank26Frame017, SPRITE_TILES_BANK26_FRAME017),
    { { -8, -4, 16, 9 } },
};
const struct sprite_frame_1box gSpriteBank26Frame018 = {
    SPRITE_FRAME(gSpriteBank26Frame018, SPRITE_TILES_BANK26_FRAME018),
    { { -11, 1, 22, 10 } },
};
const struct sprite_frame_1box gSpriteBank26Frame019 = {
    SPRITE_FRAME(gSpriteBank26Frame019, SPRITE_TILES_BANK26_FRAME019),
    { { -10, 7, 19, 7 } },
};
const struct sprite_frame_1box gSpriteBank26Frame020 = {
    SPRITE_FRAME(gSpriteBank26Frame020, SPRITE_TILES_BANK26_FRAME020),
    { { -8, 10, 19, 8 } },
};
const struct sprite_frame_1box gSpriteBank26Frame021 = {
    SPRITE_FRAME(gSpriteBank26Frame021, SPRITE_TILES_BANK26_FRAME021),
    { { -8, 13, 15, 4 } },
};
const struct sprite_frame gSpriteBank26Frame022 = SPRITE_FRAME(gSpriteBank26Frame022, SPRITE_TILES_BANK26_FRAME022);
const struct sprite_frame gSpriteBank26Frame023 = SPRITE_FRAME(gSpriteBank26Frame023, SPRITE_TILES_BANK26_FRAME023);
const struct sprite_frame gSpriteBank26Frame024 = SPRITE_FRAME(gSpriteBank26Frame024, SPRITE_TILES_BANK26_FRAME024);
const struct sprite_frame gSpriteBank26Frame025 = SPRITE_FRAME(gSpriteBank26Frame025, SPRITE_TILES_BANK26_FRAME025);
const struct sprite_frame gSpriteBank26Frame026 = SPRITE_FRAME(gSpriteBank26Frame026, SPRITE_TILES_BANK26_FRAME026);
const struct sprite_frame gSpriteBank26Frame027 = SPRITE_FRAME(gSpriteBank26Frame027, SPRITE_TILES_BANK26_FRAME027);
const struct sprite_frame gSpriteBank26Frame028 = SPRITE_FRAME(gSpriteBank26Frame028, SPRITE_TILES_BANK26_FRAME028);
const struct sprite_frame gSpriteBank26Frame029 = SPRITE_FRAME(gSpriteBank26Frame029, SPRITE_TILES_BANK26_FRAME029);
const struct sprite_frame gSpriteBank26Frame030 = SPRITE_FRAME(gSpriteBank26Frame030, SPRITE_TILES_BANK26_FRAME030);

/* The piece arrays, from games/n2/graphics/sprites/bank26_aku_aku/bank.json. */
#include "sprites/bank26_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 27: 2 animations, 28 frames, games/n2/graphics/sprites/bank27_wumpa/. */

extern const u16 gSpriteBank27Anim00Seq[14];
extern const u16 gSpriteBank27Anim01Seq[14];

extern const struct sprite_anim gSpriteBank27Anims[2] = {
    /* 0 */ {
        /* seq */ gSpriteBank27Anim00Seq,
        /* box */ { { -9, -11, 19, 23 }, { -9, -11, 20, 23 } },
        /* paletteId */ 39,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank27Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank27Anim01Seq,
        /* box */ { { -6, -7, 12, 15 }, { -6, -7, 12, 15 } },
        /* paletteId */ 39,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank27Anim01Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank27Anim00Seq[14] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
};
const u16 gSpriteBank27Anim01Seq[14] = {
    14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
};

extern const struct sprite_frame *const gSpriteBank27Frames[28] = {
    &gSpriteBank27Frame000,
    &gSpriteBank27Frame001,
    &gSpriteBank27Frame002,
    &gSpriteBank27Frame003,
    &gSpriteBank27Frame004,
    &gSpriteBank27Frame005,
    &gSpriteBank27Frame006,
    &gSpriteBank27Frame007,
    &gSpriteBank27Frame008,
    &gSpriteBank27Frame009,
    &gSpriteBank27Frame010,
    &gSpriteBank27Frame011,
    &gSpriteBank27Frame012,
    &gSpriteBank27Frame013,
    &gSpriteBank27Frame014,
    &gSpriteBank27Frame015,
    &gSpriteBank27Frame016,
    &gSpriteBank27Frame017,
    &gSpriteBank27Frame018,
    &gSpriteBank27Frame019,
    &gSpriteBank27Frame020,
    &gSpriteBank27Frame021,
    &gSpriteBank27Frame022,
    &gSpriteBank27Frame023,
    &gSpriteBank27Frame024,
    &gSpriteBank27Frame025,
    &gSpriteBank27Frame026,
    &gSpriteBank27Frame027,
};

const struct sprite_frame gSpriteBank27Frame000 = SPRITE_FRAME(gSpriteBank27Frame000, SPRITE_TILES_BANK27_FRAME000);
const struct sprite_frame gSpriteBank27Frame001 = SPRITE_FRAME(gSpriteBank27Frame001, SPRITE_TILES_BANK27_FRAME001);
const struct sprite_frame gSpriteBank27Frame002 = SPRITE_FRAME(gSpriteBank27Frame002, SPRITE_TILES_BANK27_FRAME002);
const struct sprite_frame gSpriteBank27Frame003 = SPRITE_FRAME(gSpriteBank27Frame003, SPRITE_TILES_BANK27_FRAME003);
const struct sprite_frame gSpriteBank27Frame004 = SPRITE_FRAME(gSpriteBank27Frame004, SPRITE_TILES_BANK27_FRAME004);
const struct sprite_frame gSpriteBank27Frame005 = SPRITE_FRAME(gSpriteBank27Frame005, SPRITE_TILES_BANK27_FRAME005);
const struct sprite_frame gSpriteBank27Frame006 = SPRITE_FRAME(gSpriteBank27Frame006, SPRITE_TILES_BANK27_FRAME006);
const struct sprite_frame gSpriteBank27Frame007 = SPRITE_FRAME(gSpriteBank27Frame007, SPRITE_TILES_BANK27_FRAME007);
const struct sprite_frame gSpriteBank27Frame008 = SPRITE_FRAME(gSpriteBank27Frame008, SPRITE_TILES_BANK27_FRAME008);
const struct sprite_frame gSpriteBank27Frame009 = SPRITE_FRAME(gSpriteBank27Frame009, SPRITE_TILES_BANK27_FRAME009);
const struct sprite_frame gSpriteBank27Frame010 = SPRITE_FRAME(gSpriteBank27Frame010, SPRITE_TILES_BANK27_FRAME010);
const struct sprite_frame gSpriteBank27Frame011 = SPRITE_FRAME(gSpriteBank27Frame011, SPRITE_TILES_BANK27_FRAME011);
const struct sprite_frame gSpriteBank27Frame012 = SPRITE_FRAME(gSpriteBank27Frame012, SPRITE_TILES_BANK27_FRAME012);
const struct sprite_frame gSpriteBank27Frame013 = SPRITE_FRAME(gSpriteBank27Frame013, SPRITE_TILES_BANK27_FRAME013);
const struct sprite_frame gSpriteBank27Frame014 = SPRITE_FRAME(gSpriteBank27Frame014, SPRITE_TILES_BANK27_FRAME014);
const struct sprite_frame gSpriteBank27Frame015 = SPRITE_FRAME(gSpriteBank27Frame015, SPRITE_TILES_BANK27_FRAME015);
const struct sprite_frame gSpriteBank27Frame016 = SPRITE_FRAME(gSpriteBank27Frame016, SPRITE_TILES_BANK27_FRAME016);
const struct sprite_frame gSpriteBank27Frame017 = SPRITE_FRAME(gSpriteBank27Frame017, SPRITE_TILES_BANK27_FRAME017);
const struct sprite_frame gSpriteBank27Frame018 = SPRITE_FRAME(gSpriteBank27Frame018, SPRITE_TILES_BANK27_FRAME018);
const struct sprite_frame gSpriteBank27Frame019 = SPRITE_FRAME(gSpriteBank27Frame019, SPRITE_TILES_BANK27_FRAME019);
const struct sprite_frame gSpriteBank27Frame020 = SPRITE_FRAME(gSpriteBank27Frame020, SPRITE_TILES_BANK27_FRAME020);
const struct sprite_frame gSpriteBank27Frame021 = SPRITE_FRAME(gSpriteBank27Frame021, SPRITE_TILES_BANK27_FRAME021);
const struct sprite_frame gSpriteBank27Frame022 = SPRITE_FRAME(gSpriteBank27Frame022, SPRITE_TILES_BANK27_FRAME022);
const struct sprite_frame gSpriteBank27Frame023 = SPRITE_FRAME(gSpriteBank27Frame023, SPRITE_TILES_BANK27_FRAME023);
const struct sprite_frame gSpriteBank27Frame024 = SPRITE_FRAME(gSpriteBank27Frame024, SPRITE_TILES_BANK27_FRAME024);
const struct sprite_frame gSpriteBank27Frame025 = SPRITE_FRAME(gSpriteBank27Frame025, SPRITE_TILES_BANK27_FRAME025);
const struct sprite_frame gSpriteBank27Frame026 = SPRITE_FRAME(gSpriteBank27Frame026, SPRITE_TILES_BANK27_FRAME026);
const struct sprite_frame gSpriteBank27Frame027 = SPRITE_FRAME(gSpriteBank27Frame027, SPRITE_TILES_BANK27_FRAME027);

/* The piece arrays, from games/n2/graphics/sprites/bank27_wumpa/bank.json. */
#include "sprites/bank27_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 28: 1 animation, 10 frames, games/n2/graphics/sprites/bank28_stopwatch/. */

extern const u16 gSpriteBank28Anim00Seq[10];

extern const struct sprite_anim gSpriteBank28Anims[1] = {
    /* 0 */ {
        /* seq */ gSpriteBank28Anim00Seq,
        /* box */ { { -8, -11, 17, 22 }, { -9, -11, 18, 23 } },
        /* paletteId */ 40,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank28Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank28Anim00Seq[10] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
};

extern const struct sprite_frame *const gSpriteBank28Frames[10] = {
    &gSpriteBank28Frame000,
    &gSpriteBank28Frame001,
    &gSpriteBank28Frame002,
    &gSpriteBank28Frame003,
    &gSpriteBank28Frame004,
    &gSpriteBank28Frame005,
    &gSpriteBank28Frame006,
    &gSpriteBank28Frame007,
    &gSpriteBank28Frame008,
    &gSpriteBank28Frame009,
};

const struct sprite_frame gSpriteBank28Frame000 = SPRITE_FRAME(gSpriteBank28Frame000, SPRITE_TILES_BANK28_FRAME000);
const struct sprite_frame gSpriteBank28Frame001 = SPRITE_FRAME(gSpriteBank28Frame001, SPRITE_TILES_BANK28_FRAME001);
const struct sprite_frame gSpriteBank28Frame002 = SPRITE_FRAME(gSpriteBank28Frame002, SPRITE_TILES_BANK28_FRAME002);
const struct sprite_frame gSpriteBank28Frame003 = SPRITE_FRAME(gSpriteBank28Frame003, SPRITE_TILES_BANK28_FRAME003);
const struct sprite_frame gSpriteBank28Frame004 = SPRITE_FRAME(gSpriteBank28Frame004, SPRITE_TILES_BANK28_FRAME004);
const struct sprite_frame gSpriteBank28Frame005 = SPRITE_FRAME(gSpriteBank28Frame005, SPRITE_TILES_BANK28_FRAME005);
const struct sprite_frame gSpriteBank28Frame006 = SPRITE_FRAME(gSpriteBank28Frame006, SPRITE_TILES_BANK28_FRAME006);
const struct sprite_frame gSpriteBank28Frame007 = SPRITE_FRAME(gSpriteBank28Frame007, SPRITE_TILES_BANK28_FRAME007);
const struct sprite_frame gSpriteBank28Frame008 = SPRITE_FRAME(gSpriteBank28Frame008, SPRITE_TILES_BANK28_FRAME008);
const struct sprite_frame gSpriteBank28Frame009 = SPRITE_FRAME(gSpriteBank28Frame009, SPRITE_TILES_BANK28_FRAME009);

/* The piece arrays, from games/n2/graphics/sprites/bank28_stopwatch/bank.json. */
#include "sprites/bank28_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 29: 1 animation, 8 frames, games/n2/graphics/sprites/bank29_crystal/. */

extern const u16 gSpriteBank29Anim00Seq[8];

extern const struct sprite_anim gSpriteBank29Anims[1] = {
    /* 0 */ {
        /* seq */ gSpriteBank29Anim00Seq,
        /* box */ { { -6, -21, 13, 43 }, { -6, -21, 13, 43 } },
        /* paletteId */ 7,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank29Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank29Anim00Seq[8] = {
    0, 1, 2, 3, 4, 5, 6, 7,
};

extern const struct sprite_frame *const gSpriteBank29Frames[8] = {
    &gSpriteBank29Frame000,
    &gSpriteBank29Frame001,
    &gSpriteBank29Frame002,
    &gSpriteBank29Frame003,
    &gSpriteBank29Frame004,
    &gSpriteBank29Frame005,
    &gSpriteBank29Frame006,
    &gSpriteBank29Frame007,
};

const struct sprite_frame gSpriteBank29Frame000 = SPRITE_FRAME(gSpriteBank29Frame000, SPRITE_TILES_BANK29_FRAME000);
const struct sprite_frame gSpriteBank29Frame001 = SPRITE_FRAME(gSpriteBank29Frame001, SPRITE_TILES_BANK29_FRAME001);
const struct sprite_frame gSpriteBank29Frame002 = SPRITE_FRAME(gSpriteBank29Frame002, SPRITE_TILES_BANK29_FRAME002);
const struct sprite_frame gSpriteBank29Frame003 = SPRITE_FRAME(gSpriteBank29Frame003, SPRITE_TILES_BANK29_FRAME003);
const struct sprite_frame gSpriteBank29Frame004 = SPRITE_FRAME(gSpriteBank29Frame004, SPRITE_TILES_BANK29_FRAME004);
const struct sprite_frame gSpriteBank29Frame005 = SPRITE_FRAME(gSpriteBank29Frame005, SPRITE_TILES_BANK29_FRAME005);
const struct sprite_frame gSpriteBank29Frame006 = SPRITE_FRAME(gSpriteBank29Frame006, SPRITE_TILES_BANK29_FRAME006);
const struct sprite_frame gSpriteBank29Frame007 = SPRITE_FRAME(gSpriteBank29Frame007, SPRITE_TILES_BANK29_FRAME007);

/* The piece arrays, from games/n2/graphics/sprites/bank29_crystal/bank.json. */
#include "sprites/bank29_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 30: 5 animations, 72 frames, games/n2/graphics/sprites/bank30_powers/. */

extern const u16 gSpriteBank30Anim00Seq[15];
extern const u16 gSpriteBank30Anim01Seq[15];
extern const u16 gSpriteBank30Anim02Seq[15];
extern const u16 gSpriteBank30Anim03Seq[15];
extern const u16 gSpriteBank30Anim04Seq[15];

extern const struct sprite_anim gSpriteBank30Anims[5] = {
    /* 0 */ {
        /* seq */ gSpriteBank30Anim00Seq,
        /* box */ { { -2, -14, 4, 29 }, { -13, -14, 26, 29 } },
        /* paletteId */ 41,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank30Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank30Anim01Seq,
        /* box */ { { -2, -14, 4, 29 }, { -13, -14, 26, 29 } },
        /* paletteId */ 41,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank30Anim01Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 2 */ {
        /* seq */ gSpriteBank30Anim02Seq,
        /* box */ { { -2, -14, 4, 29 }, { -12, -14, 24, 29 } },
        /* paletteId */ 41,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank30Anim02Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 3 */ {
        /* seq */ gSpriteBank30Anim03Seq,
        /* box */ { { -2, -14, 4, 29 }, { -13, -14, 26, 29 } },
        /* paletteId */ 41,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank30Anim03Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 4 */ {
        /* seq */ gSpriteBank30Anim04Seq,
        /* box */ { { -2, -14, 4, 29 }, { -13, -14, 26, 29 } },
        /* paletteId */ 41,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank30Anim04Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank30Anim00Seq[15] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
};
const u16 gSpriteBank30Anim01Seq[15] = {
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
};
const u16 gSpriteBank30Anim02Seq[15] = {
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44,
};
const u16 gSpriteBank30Anim03Seq[15] = {
    45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
};
const u16 gSpriteBank30Anim04Seq[15] = {
    45, 46, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 59,
};

extern const struct sprite_frame *const gSpriteBank30Frames[72] = {
    &gSpriteBank30Frame000,
    &gSpriteBank30Frame001,
    &gSpriteBank30Frame002,
    &gSpriteBank30Frame003,
    &gSpriteBank30Frame004,
    &gSpriteBank30Frame005,
    &gSpriteBank30Frame006,
    &gSpriteBank30Frame007,
    &gSpriteBank30Frame008,
    &gSpriteBank30Frame009,
    &gSpriteBank30Frame010,
    &gSpriteBank30Frame011,
    &gSpriteBank30Frame012,
    &gSpriteBank30Frame013,
    &gSpriteBank30Frame014,
    &gSpriteBank30Frame015,
    &gSpriteBank30Frame016,
    &gSpriteBank30Frame017,
    &gSpriteBank30Frame018,
    &gSpriteBank30Frame019,
    &gSpriteBank30Frame020,
    &gSpriteBank30Frame021,
    &gSpriteBank30Frame022,
    &gSpriteBank30Frame023,
    &gSpriteBank30Frame024,
    &gSpriteBank30Frame025,
    &gSpriteBank30Frame026,
    &gSpriteBank30Frame027,
    &gSpriteBank30Frame028,
    &gSpriteBank30Frame029,
    &gSpriteBank30Frame030,
    &gSpriteBank30Frame031,
    &gSpriteBank30Frame032,
    &gSpriteBank30Frame033,
    &gSpriteBank30Frame034,
    &gSpriteBank30Frame035,
    &gSpriteBank30Frame036,
    &gSpriteBank30Frame037,
    &gSpriteBank30Frame038,
    &gSpriteBank30Frame039,
    &gSpriteBank30Frame040,
    &gSpriteBank30Frame041,
    &gSpriteBank30Frame042,
    &gSpriteBank30Frame043,
    &gSpriteBank30Frame044,
    &gSpriteBank30Frame045,
    &gSpriteBank30Frame046,
    &gSpriteBank30Frame047,
    &gSpriteBank30Frame048,
    &gSpriteBank30Frame049,
    &gSpriteBank30Frame050,
    &gSpriteBank30Frame051,
    &gSpriteBank30Frame052,
    &gSpriteBank30Frame053,
    &gSpriteBank30Frame054,
    &gSpriteBank30Frame055,
    &gSpriteBank30Frame056,
    &gSpriteBank30Frame057,
    &gSpriteBank30Frame058,
    &gSpriteBank30Frame059,
    &gSpriteBank30Frame060,
    &gSpriteBank30Frame061,
    &gSpriteBank30Frame062,
    &gSpriteBank30Frame063,
    &gSpriteBank30Frame064,
    &gSpriteBank30Frame065,
    &gSpriteBank30Frame066,
    &gSpriteBank30Frame067,
    &gSpriteBank30Frame068,
    &gSpriteBank30Frame069,
    &gSpriteBank30Frame070,
    &gSpriteBank30Frame071,
};

const struct sprite_frame gSpriteBank30Frame000 = SPRITE_FRAME(gSpriteBank30Frame000, SPRITE_TILES_BANK30_FRAME000);
const struct sprite_frame gSpriteBank30Frame001 = SPRITE_FRAME(gSpriteBank30Frame001, SPRITE_TILES_BANK30_FRAME001);
const struct sprite_frame gSpriteBank30Frame002 = SPRITE_FRAME(gSpriteBank30Frame002, SPRITE_TILES_BANK30_FRAME002);
const struct sprite_frame gSpriteBank30Frame003 = SPRITE_FRAME(gSpriteBank30Frame003, SPRITE_TILES_BANK30_FRAME003);
const struct sprite_frame gSpriteBank30Frame004 = SPRITE_FRAME(gSpriteBank30Frame004, SPRITE_TILES_BANK30_FRAME004);
const struct sprite_frame gSpriteBank30Frame005 = SPRITE_FRAME(gSpriteBank30Frame005, SPRITE_TILES_BANK30_FRAME005);
const struct sprite_frame gSpriteBank30Frame006 = SPRITE_FRAME(gSpriteBank30Frame006, SPRITE_TILES_BANK30_FRAME006);
const struct sprite_frame gSpriteBank30Frame007 = SPRITE_FRAME(gSpriteBank30Frame007, SPRITE_TILES_BANK30_FRAME007);
const struct sprite_frame gSpriteBank30Frame008 = SPRITE_FRAME(gSpriteBank30Frame008, SPRITE_TILES_BANK30_FRAME008);
const struct sprite_frame gSpriteBank30Frame009 = SPRITE_FRAME(gSpriteBank30Frame009, SPRITE_TILES_BANK30_FRAME009);
const struct sprite_frame gSpriteBank30Frame010 = SPRITE_FRAME(gSpriteBank30Frame010, SPRITE_TILES_BANK30_FRAME010);
const struct sprite_frame gSpriteBank30Frame011 = SPRITE_FRAME(gSpriteBank30Frame011, SPRITE_TILES_BANK30_FRAME011);
const struct sprite_frame gSpriteBank30Frame012 = SPRITE_FRAME(gSpriteBank30Frame012, SPRITE_TILES_BANK30_FRAME012);
const struct sprite_frame gSpriteBank30Frame013 = SPRITE_FRAME(gSpriteBank30Frame013, SPRITE_TILES_BANK30_FRAME013);
const struct sprite_frame gSpriteBank30Frame014 = SPRITE_FRAME(gSpriteBank30Frame014, SPRITE_TILES_BANK30_FRAME014);
const struct sprite_frame gSpriteBank30Frame015 = SPRITE_FRAME(gSpriteBank30Frame015, SPRITE_TILES_BANK30_FRAME015);
const struct sprite_frame gSpriteBank30Frame016 = SPRITE_FRAME(gSpriteBank30Frame016, SPRITE_TILES_BANK30_FRAME016);
const struct sprite_frame gSpriteBank30Frame017 = SPRITE_FRAME(gSpriteBank30Frame017, SPRITE_TILES_BANK30_FRAME017);
const struct sprite_frame gSpriteBank30Frame018 = SPRITE_FRAME(gSpriteBank30Frame018, SPRITE_TILES_BANK30_FRAME018);
const struct sprite_frame gSpriteBank30Frame019 = SPRITE_FRAME(gSpriteBank30Frame019, SPRITE_TILES_BANK30_FRAME019);
const struct sprite_frame gSpriteBank30Frame020 = SPRITE_FRAME(gSpriteBank30Frame020, SPRITE_TILES_BANK30_FRAME020);
const struct sprite_frame gSpriteBank30Frame021 = SPRITE_FRAME(gSpriteBank30Frame021, SPRITE_TILES_BANK30_FRAME021);
const struct sprite_frame gSpriteBank30Frame022 = SPRITE_FRAME(gSpriteBank30Frame022, SPRITE_TILES_BANK30_FRAME022);
const struct sprite_frame gSpriteBank30Frame023 = SPRITE_FRAME(gSpriteBank30Frame023, SPRITE_TILES_BANK30_FRAME023);
const struct sprite_frame gSpriteBank30Frame024 = SPRITE_FRAME(gSpriteBank30Frame024, SPRITE_TILES_BANK30_FRAME024);
const struct sprite_frame gSpriteBank30Frame025 = SPRITE_FRAME(gSpriteBank30Frame025, SPRITE_TILES_BANK30_FRAME025);
const struct sprite_frame gSpriteBank30Frame026 = SPRITE_FRAME(gSpriteBank30Frame026, SPRITE_TILES_BANK30_FRAME026);
const struct sprite_frame gSpriteBank30Frame027 = SPRITE_FRAME(gSpriteBank30Frame027, SPRITE_TILES_BANK30_FRAME027);
const struct sprite_frame gSpriteBank30Frame028 = SPRITE_FRAME(gSpriteBank30Frame028, SPRITE_TILES_BANK30_FRAME028);
const struct sprite_frame gSpriteBank30Frame029 = SPRITE_FRAME(gSpriteBank30Frame029, SPRITE_TILES_BANK30_FRAME029);
const struct sprite_frame gSpriteBank30Frame030 = SPRITE_FRAME(gSpriteBank30Frame030, SPRITE_TILES_BANK30_FRAME030);
const struct sprite_frame gSpriteBank30Frame031 = SPRITE_FRAME(gSpriteBank30Frame031, SPRITE_TILES_BANK30_FRAME031);
const struct sprite_frame gSpriteBank30Frame032 = SPRITE_FRAME(gSpriteBank30Frame032, SPRITE_TILES_BANK30_FRAME032);
const struct sprite_frame gSpriteBank30Frame033 = SPRITE_FRAME(gSpriteBank30Frame033, SPRITE_TILES_BANK30_FRAME033);
const struct sprite_frame gSpriteBank30Frame034 = SPRITE_FRAME(gSpriteBank30Frame034, SPRITE_TILES_BANK30_FRAME034);
const struct sprite_frame gSpriteBank30Frame035 = SPRITE_FRAME(gSpriteBank30Frame035, SPRITE_TILES_BANK30_FRAME035);
const struct sprite_frame gSpriteBank30Frame036 = SPRITE_FRAME(gSpriteBank30Frame036, SPRITE_TILES_BANK30_FRAME036);
const struct sprite_frame gSpriteBank30Frame037 = SPRITE_FRAME(gSpriteBank30Frame037, SPRITE_TILES_BANK30_FRAME037);
const struct sprite_frame gSpriteBank30Frame038 = SPRITE_FRAME(gSpriteBank30Frame038, SPRITE_TILES_BANK30_FRAME038);
const struct sprite_frame gSpriteBank30Frame039 = SPRITE_FRAME(gSpriteBank30Frame039, SPRITE_TILES_BANK30_FRAME039);
const struct sprite_frame gSpriteBank30Frame040 = SPRITE_FRAME(gSpriteBank30Frame040, SPRITE_TILES_BANK30_FRAME040);
const struct sprite_frame gSpriteBank30Frame041 = SPRITE_FRAME(gSpriteBank30Frame041, SPRITE_TILES_BANK30_FRAME041);
const struct sprite_frame gSpriteBank30Frame042 = SPRITE_FRAME(gSpriteBank30Frame042, SPRITE_TILES_BANK30_FRAME042);
const struct sprite_frame gSpriteBank30Frame043 = SPRITE_FRAME(gSpriteBank30Frame043, SPRITE_TILES_BANK30_FRAME043);
const struct sprite_frame gSpriteBank30Frame044 = SPRITE_FRAME(gSpriteBank30Frame044, SPRITE_TILES_BANK30_FRAME044);
const struct sprite_frame gSpriteBank30Frame045 = SPRITE_FRAME(gSpriteBank30Frame045, SPRITE_TILES_BANK30_FRAME045);
const struct sprite_frame gSpriteBank30Frame046 = SPRITE_FRAME(gSpriteBank30Frame046, SPRITE_TILES_BANK30_FRAME046);
const struct sprite_frame gSpriteBank30Frame047 = SPRITE_FRAME(gSpriteBank30Frame047, SPRITE_TILES_BANK30_FRAME047);
const struct sprite_frame gSpriteBank30Frame048 = SPRITE_FRAME(gSpriteBank30Frame048, SPRITE_TILES_BANK30_FRAME048);
const struct sprite_frame gSpriteBank30Frame049 = SPRITE_FRAME(gSpriteBank30Frame049, SPRITE_TILES_BANK30_FRAME049);
const struct sprite_frame gSpriteBank30Frame050 = SPRITE_FRAME(gSpriteBank30Frame050, SPRITE_TILES_BANK30_FRAME050);
const struct sprite_frame gSpriteBank30Frame051 = SPRITE_FRAME(gSpriteBank30Frame051, SPRITE_TILES_BANK30_FRAME051);
const struct sprite_frame gSpriteBank30Frame052 = SPRITE_FRAME(gSpriteBank30Frame052, SPRITE_TILES_BANK30_FRAME052);
const struct sprite_frame gSpriteBank30Frame053 = SPRITE_FRAME(gSpriteBank30Frame053, SPRITE_TILES_BANK30_FRAME053);
const struct sprite_frame gSpriteBank30Frame054 = SPRITE_FRAME(gSpriteBank30Frame054, SPRITE_TILES_BANK30_FRAME054);
const struct sprite_frame gSpriteBank30Frame055 = SPRITE_FRAME(gSpriteBank30Frame055, SPRITE_TILES_BANK30_FRAME055);
const struct sprite_frame gSpriteBank30Frame056 = SPRITE_FRAME(gSpriteBank30Frame056, SPRITE_TILES_BANK30_FRAME056);
const struct sprite_frame gSpriteBank30Frame057 = SPRITE_FRAME(gSpriteBank30Frame057, SPRITE_TILES_BANK30_FRAME057);
const struct sprite_frame gSpriteBank30Frame058 = SPRITE_FRAME(gSpriteBank30Frame058, SPRITE_TILES_BANK30_FRAME058);
const struct sprite_frame gSpriteBank30Frame059 = SPRITE_FRAME(gSpriteBank30Frame059, SPRITE_TILES_BANK30_FRAME059);
const struct sprite_frame gSpriteBank30Frame060 = SPRITE_FRAME(gSpriteBank30Frame060, SPRITE_TILES_BANK30_FRAME060);
const struct sprite_frame gSpriteBank30Frame061 = SPRITE_FRAME(gSpriteBank30Frame061, SPRITE_TILES_BANK30_FRAME061);
const struct sprite_frame gSpriteBank30Frame062 = SPRITE_FRAME(gSpriteBank30Frame062, SPRITE_TILES_BANK30_FRAME062);
const struct sprite_frame gSpriteBank30Frame063 = SPRITE_FRAME(gSpriteBank30Frame063, SPRITE_TILES_BANK30_FRAME063);
const struct sprite_frame gSpriteBank30Frame064 = SPRITE_FRAME(gSpriteBank30Frame064, SPRITE_TILES_BANK30_FRAME064);
const struct sprite_frame gSpriteBank30Frame065 = SPRITE_FRAME(gSpriteBank30Frame065, SPRITE_TILES_BANK30_FRAME065);
const struct sprite_frame gSpriteBank30Frame066 = SPRITE_FRAME(gSpriteBank30Frame066, SPRITE_TILES_BANK30_FRAME066);
const struct sprite_frame gSpriteBank30Frame067 = SPRITE_FRAME(gSpriteBank30Frame067, SPRITE_TILES_BANK30_FRAME067);
const struct sprite_frame gSpriteBank30Frame068 = SPRITE_FRAME(gSpriteBank30Frame068, SPRITE_TILES_BANK30_FRAME068);
const struct sprite_frame gSpriteBank30Frame069 = SPRITE_FRAME(gSpriteBank30Frame069, SPRITE_TILES_BANK30_FRAME069);
const struct sprite_frame gSpriteBank30Frame070 = SPRITE_FRAME(gSpriteBank30Frame070, SPRITE_TILES_BANK30_FRAME070);
const struct sprite_frame gSpriteBank30Frame071 = SPRITE_FRAME(gSpriteBank30Frame071, SPRITE_TILES_BANK30_FRAME071);

/* The piece arrays, from games/n2/graphics/sprites/bank30_powers/bank.json. */
#include "sprites/bank30_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 31: 13 animations, 97 frames, games/n2/graphics/sprites/bank31_platforms/. */

extern const u16 gSpriteBank31Anim00Seq[1];
extern const u16 gSpriteBank31Anim01Seq[1];
extern const u16 gSpriteBank31Anim02Seq[1];
extern const u16 gSpriteBank31Anim03Seq[1];
extern const u16 gSpriteBank31Anim04Seq[18];
extern const u16 gSpriteBank31Anim05Seq[18];
extern const u16 gSpriteBank31Anim06Seq[18];
extern const u16 gSpriteBank31Anim07Seq[15];
extern const u16 gSpriteBank31Anim08Seq[1];
extern const u16 gSpriteBank31Anim09Seq[11];
extern const u16 gSpriteBank31Anim10Seq[17];
extern const u16 gSpriteBank31Anim11Seq[1];
extern const u16 gSpriteBank31Anim12Seq[7];

extern const struct sprite_anim gSpriteBank31Anims[13] = {
    /* 0 */ {
        /* seq */ gSpriteBank31Anim00Seq,
        /* box */ { { -20, 3, 42, 11 }, { -22, -13, 46, 28 } },
        /* paletteId */ 38,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank31Anim01Seq,
        /* box */ { { -18, -1, 37, 15 }, { -20, -14, 41, 28 } },
        /* paletteId */ 38,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim01Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 2 */ {
        /* seq */ gSpriteBank31Anim02Seq,
        /* box */ { { -18, -1, 37, 15 }, { -20, -14, 41, 28 } },
        /* paletteId */ 38,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank31Anim03Seq,
        /* box */ { { -20, -3, 43, 18 }, { -20, -14, 42, 28 } },
        /* paletteId */ 32,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim03Seq),
        /* flags */ 0,
    },
    /* 4 */ {
        /* seq */ gSpriteBank31Anim04Seq,
        /* box */ { { -30, 1, 51, 16 }, { -31, -19, 63, 39 } },
        /* paletteId */ 42,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim04Seq),
        /* flags */ 0,
    },
    /* 5 */ {
        /* seq */ gSpriteBank31Anim05Seq,
        /* box */ { { -31, 1, 52, 16 }, { -31, -19, 63, 39 } },
        /* paletteId */ 43,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim05Seq),
        /* flags */ 0,
    },
    /* 6 */ {
        /* seq */ gSpriteBank31Anim06Seq,
        /* box */ { { -31, 2, 54, 15 }, { -31, -19, 63, 39 } },
        /* paletteId */ 42,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim06Seq),
        /* flags */ 0,
    },
    /* 7 */ {
        /* seq */ gSpriteBank31Anim07Seq,
        /* box */ { { -34, -3, 61, 18 }, { -34, -14, 68, 29 } },
        /* paletteId */ 44,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim07Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 8 */ {
        /* seq */ gSpriteBank31Anim08Seq,
        /* box */ { { -23, -3, 43, 14 }, { -23, -11, 47, 23 } },
        /* paletteId */ 45,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim08Seq),
        /* flags */ 0,
    },
    /* 9 */ {
        /* seq */ gSpriteBank31Anim09Seq,
        /* box */ { { -21, -4, 44, 18 }, { -22, -13, 45, 27 } },
        /* paletteId */ 46,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim09Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 10 */ {
        /* seq */ gSpriteBank31Anim10Seq,
        /* box */ { { -34, 1, 58, 17 }, { -34, -16, 69, 33 } },
        /* paletteId */ 47,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim10Seq),
        /* flags */ 0,
    },
    /* 11 */ {
        /* seq */ gSpriteBank31Anim11Seq,
        /* box */ { { -27, 0, 45, 15 }, { -27, -13, 55, 27 } },
        /* paletteId */ 47,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim11Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 12 */ {
        /* seq */ gSpriteBank31Anim12Seq,
        /* box */ { { -27, 0, 45, 15 }, { -29, -13, 58, 64 } },
        /* paletteId */ 47,
        /* duration */ 7,
        /* frameCount */ ARRAY_COUNT(gSpriteBank31Anim12Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank31Anim00Seq[1] = {
    0,
};
const u16 gSpriteBank31Anim01Seq[1] = {
    1,
};
const u16 gSpriteBank31Anim02Seq[1] = {
    2,
};
const u16 gSpriteBank31Anim03Seq[1] = {
    3,
};
const u16 gSpriteBank31Anim04Seq[18] = {
    4, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 17,
    4, 18,
};
const u16 gSpriteBank31Anim05Seq[18] = {
    19, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 32,
    19, 33,
};
const u16 gSpriteBank31Anim06Seq[18] = {
    34, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 47,
    34, 48,
};
const u16 gSpriteBank31Anim07Seq[15] = {
    49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
};
const u16 gSpriteBank31Anim08Seq[1] = {
    64,
};
const u16 gSpriteBank31Anim09Seq[11] = {
    65, 66, 67, 68, 67, 65, 69, 70, 71, 70, 69,
};
const u16 gSpriteBank31Anim10Seq[17] = {
    72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
    88,
};
const u16 gSpriteBank31Anim11Seq[1] = {
    89,
};
const u16 gSpriteBank31Anim12Seq[7] = {
    90, 91, 92, 93, 94, 95, 96,
};

extern const struct sprite_frame *const gSpriteBank31Frames[97] = {
    &gSpriteBank31Frame000.frame,
    &gSpriteBank31Frame001,
    &gSpriteBank31Frame002,
    &gSpriteBank31Frame003.frame,
    &gSpriteBank31Frame004.frame,
    &gSpriteBank31Frame005.frame,
    &gSpriteBank31Frame006.frame,
    &gSpriteBank31Frame007.frame,
    &gSpriteBank31Frame008.frame,
    &gSpriteBank31Frame009.frame,
    &gSpriteBank31Frame010.frame,
    &gSpriteBank31Frame011.frame,
    &gSpriteBank31Frame012.frame,
    &gSpriteBank31Frame013.frame,
    &gSpriteBank31Frame014.frame,
    &gSpriteBank31Frame015.frame,
    &gSpriteBank31Frame016.frame,
    &gSpriteBank31Frame017.frame,
    &gSpriteBank31Frame018,
    &gSpriteBank31Frame019.frame,
    &gSpriteBank31Frame020.frame,
    &gSpriteBank31Frame021.frame,
    &gSpriteBank31Frame022.frame,
    &gSpriteBank31Frame023.frame,
    &gSpriteBank31Frame024.frame,
    &gSpriteBank31Frame025.frame,
    &gSpriteBank31Frame026.frame,
    &gSpriteBank31Frame027.frame,
    &gSpriteBank31Frame028.frame,
    &gSpriteBank31Frame029.frame,
    &gSpriteBank31Frame030.frame,
    &gSpriteBank31Frame031.frame,
    &gSpriteBank31Frame032.frame,
    &gSpriteBank31Frame033,
    &gSpriteBank31Frame034.frame,
    &gSpriteBank31Frame035.frame,
    &gSpriteBank31Frame036.frame,
    &gSpriteBank31Frame037.frame,
    &gSpriteBank31Frame038.frame,
    &gSpriteBank31Frame039.frame,
    &gSpriteBank31Frame040.frame,
    &gSpriteBank31Frame041.frame,
    &gSpriteBank31Frame042.frame,
    &gSpriteBank31Frame043.frame,
    &gSpriteBank31Frame044.frame,
    &gSpriteBank31Frame045.frame,
    &gSpriteBank31Frame046.frame,
    &gSpriteBank31Frame047.frame,
    &gSpriteBank31Frame048,
    &gSpriteBank31Frame049,
    &gSpriteBank31Frame050,
    &gSpriteBank31Frame051,
    &gSpriteBank31Frame052,
    &gSpriteBank31Frame053,
    &gSpriteBank31Frame054,
    &gSpriteBank31Frame055,
    &gSpriteBank31Frame056,
    &gSpriteBank31Frame057,
    &gSpriteBank31Frame058,
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
    &gSpriteBank31Frame094,
    &gSpriteBank31Frame095,
    &gSpriteBank31Frame096,
};

const struct sprite_frame_1box gSpriteBank31Frame000 = {
    SPRITE_FRAME(gSpriteBank31Frame000, SPRITE_TILES_BANK31_FRAME000),
    { { -21, -12, 42, 25 } },
};
const struct sprite_frame gSpriteBank31Frame001 = SPRITE_FRAME(gSpriteBank31Frame001, SPRITE_TILES_BANK31_FRAME001);
const struct sprite_frame gSpriteBank31Frame002 = SPRITE_FRAME(gSpriteBank31Frame002, SPRITE_TILES_BANK31_FRAME002);
const struct sprite_frame_1box gSpriteBank31Frame003 = {
    SPRITE_FRAME(gSpriteBank31Frame003, SPRITE_TILES_BANK31_FRAME003),
    { { -20, -14, 42, 28 } },
};
const struct sprite_frame_1box gSpriteBank31Frame004 = {
    SPRITE_FRAME(gSpriteBank31Frame004, SPRITE_TILES_BANK31_FRAME004),
    { { -31, -15, 63, 31 } },
};
const struct sprite_frame_1box gSpriteBank31Frame005 = {
    SPRITE_FRAME(gSpriteBank31Frame005, SPRITE_TILES_BANK31_FRAME005),
    { { -31, -15, 63, 35 } },
};
const struct sprite_frame_1box gSpriteBank31Frame006 = {
    SPRITE_FRAME(gSpriteBank31Frame006, SPRITE_TILES_BANK31_FRAME006),
    { { -31, -15, 63, 38 } },
};
const struct sprite_frame_1box gSpriteBank31Frame007 = {
    SPRITE_FRAME(gSpriteBank31Frame007, SPRITE_TILES_BANK31_FRAME007),
    { { -31, -15, 63, 35 } },
};
const struct sprite_frame_1box gSpriteBank31Frame008 = {
    SPRITE_FRAME(gSpriteBank31Frame008, SPRITE_TILES_BANK31_FRAME008),
    { { -31, -16, 63, 32 } },
};
const struct sprite_frame_1box gSpriteBank31Frame009 = {
    SPRITE_FRAME(gSpriteBank31Frame009, SPRITE_TILES_BANK31_FRAME009),
    { { -31, -19, 63, 35 } },
};
const struct sprite_frame_1box gSpriteBank31Frame010 = {
    SPRITE_FRAME(gSpriteBank31Frame010, SPRITE_TILES_BANK31_FRAME010),
    { { -31, -21, 63, 37 } },
};
const struct sprite_frame_1box gSpriteBank31Frame011 = {
    SPRITE_FRAME(gSpriteBank31Frame011, SPRITE_TILES_BANK31_FRAME011),
    { { -31, -19, 63, 35 } },
};
const struct sprite_frame_1box gSpriteBank31Frame012 = {
    SPRITE_FRAME(gSpriteBank31Frame012, SPRITE_TILES_BANK31_FRAME012),
    { { -31, -17, 63, 33 } },
};
const struct sprite_frame_1box gSpriteBank31Frame013 = {
    SPRITE_FRAME(gSpriteBank31Frame013, SPRITE_TILES_BANK31_FRAME013),
    { { -31, -15, 63, 32 } },
};
const struct sprite_frame_1box gSpriteBank31Frame014 = {
    SPRITE_FRAME(gSpriteBank31Frame014, SPRITE_TILES_BANK31_FRAME014),
    { { -31, -15, 63, 34 } },
};
const struct sprite_frame_1box gSpriteBank31Frame015 = {
    SPRITE_FRAME(gSpriteBank31Frame015, SPRITE_TILES_BANK31_FRAME015),
    { { -31, -15, 63, 34 } },
};
const struct sprite_frame_1box gSpriteBank31Frame016 = {
    SPRITE_FRAME(gSpriteBank31Frame016, SPRITE_TILES_BANK31_FRAME016),
    { { -31, -15, 63, 31 } },
};
const struct sprite_frame_1box gSpriteBank31Frame017 = {
    SPRITE_FRAME(gSpriteBank31Frame017, SPRITE_TILES_BANK31_FRAME017),
    { { -31, -16, 63, 32 } },
};
const struct sprite_frame gSpriteBank31Frame018 = SPRITE_FRAME(gSpriteBank31Frame018, SPRITE_TILES_BANK31_FRAME018);
const struct sprite_frame_1box gSpriteBank31Frame019 = {
    SPRITE_FRAME(gSpriteBank31Frame019, SPRITE_TILES_BANK31_FRAME019),
    { { -31, -15, 63, 31 } },
};
const struct sprite_frame_1box gSpriteBank31Frame020 = {
    SPRITE_FRAME(gSpriteBank31Frame020, SPRITE_TILES_BANK31_FRAME020),
    { { -31, -15, 63, 35 } },
};
const struct sprite_frame_1box gSpriteBank31Frame021 = {
    SPRITE_FRAME(gSpriteBank31Frame021, SPRITE_TILES_BANK31_FRAME021),
    { { -31, -15, 63, 37 } },
};
const struct sprite_frame_1box gSpriteBank31Frame022 = {
    SPRITE_FRAME(gSpriteBank31Frame022, SPRITE_TILES_BANK31_FRAME022),
    { { -31, -15, 63, 34 } },
};
const struct sprite_frame_1box gSpriteBank31Frame023 = {
    SPRITE_FRAME(gSpriteBank31Frame023, SPRITE_TILES_BANK31_FRAME023),
    { { -31, -16, 63, 32 } },
};
const struct sprite_frame_1box gSpriteBank31Frame024 = {
    SPRITE_FRAME(gSpriteBank31Frame024, SPRITE_TILES_BANK31_FRAME024),
    { { -31, -19, 63, 35 } },
};
const struct sprite_frame_1box gSpriteBank31Frame025 = {
    SPRITE_FRAME(gSpriteBank31Frame025, SPRITE_TILES_BANK31_FRAME025),
    { { -31, -21, 63, 37 } },
};
const struct sprite_frame_1box gSpriteBank31Frame026 = {
    SPRITE_FRAME(gSpriteBank31Frame026, SPRITE_TILES_BANK31_FRAME026),
    { { -31, -19, 63, 35 } },
};
const struct sprite_frame_1box gSpriteBank31Frame027 = {
    SPRITE_FRAME(gSpriteBank31Frame027, SPRITE_TILES_BANK31_FRAME027),
    { { -31, -17, 63, 33 } },
};
const struct sprite_frame_1box gSpriteBank31Frame028 = {
    SPRITE_FRAME(gSpriteBank31Frame028, SPRITE_TILES_BANK31_FRAME028),
    { { -31, -15, 63, 32 } },
};
const struct sprite_frame_1box gSpriteBank31Frame029 = {
    SPRITE_FRAME(gSpriteBank31Frame029, SPRITE_TILES_BANK31_FRAME029),
    { { -31, -15, 63, 34 } },
};
const struct sprite_frame_1box gSpriteBank31Frame030 = {
    SPRITE_FRAME(gSpriteBank31Frame030, SPRITE_TILES_BANK31_FRAME030),
    { { -31, -15, 63, 34 } },
};
const struct sprite_frame_1box gSpriteBank31Frame031 = {
    SPRITE_FRAME(gSpriteBank31Frame031, SPRITE_TILES_BANK31_FRAME031),
    { { -31, -15, 63, 31 } },
};
const struct sprite_frame_1box gSpriteBank31Frame032 = {
    SPRITE_FRAME(gSpriteBank31Frame032, SPRITE_TILES_BANK31_FRAME032),
    { { -31, -16, 63, 32 } },
};
const struct sprite_frame gSpriteBank31Frame033 = SPRITE_FRAME(gSpriteBank31Frame033, SPRITE_TILES_BANK31_FRAME033);
const struct sprite_frame_1box gSpriteBank31Frame034 = {
    SPRITE_FRAME(gSpriteBank31Frame034, SPRITE_TILES_BANK31_FRAME034),
    { { -31, -15, 63, 31 } },
};
const struct sprite_frame_1box gSpriteBank31Frame035 = {
    SPRITE_FRAME(gSpriteBank31Frame035, SPRITE_TILES_BANK31_FRAME035),
    { { -31, -15, 63, 35 } },
};
const struct sprite_frame_1box gSpriteBank31Frame036 = {
    SPRITE_FRAME(gSpriteBank31Frame036, SPRITE_TILES_BANK31_FRAME036),
    { { -31, -15, 63, 38 } },
};
const struct sprite_frame_1box gSpriteBank31Frame037 = {
    SPRITE_FRAME(gSpriteBank31Frame037, SPRITE_TILES_BANK31_FRAME037),
    { { -31, -15, 63, 35 } },
};
const struct sprite_frame_1box gSpriteBank31Frame038 = {
    SPRITE_FRAME(gSpriteBank31Frame038, SPRITE_TILES_BANK31_FRAME038),
    { { -31, -16, 63, 32 } },
};
const struct sprite_frame_1box gSpriteBank31Frame039 = {
    SPRITE_FRAME(gSpriteBank31Frame039, SPRITE_TILES_BANK31_FRAME039),
    { { -31, -19, 63, 35 } },
};
const struct sprite_frame_1box gSpriteBank31Frame040 = {
    SPRITE_FRAME(gSpriteBank31Frame040, SPRITE_TILES_BANK31_FRAME040),
    { { -31, -21, 63, 37 } },
};
const struct sprite_frame_1box gSpriteBank31Frame041 = {
    SPRITE_FRAME(gSpriteBank31Frame041, SPRITE_TILES_BANK31_FRAME041),
    { { -31, -19, 63, 35 } },
};
const struct sprite_frame_1box gSpriteBank31Frame042 = {
    SPRITE_FRAME(gSpriteBank31Frame042, SPRITE_TILES_BANK31_FRAME042),
    { { -31, -17, 63, 33 } },
};
const struct sprite_frame_1box gSpriteBank31Frame043 = {
    SPRITE_FRAME(gSpriteBank31Frame043, SPRITE_TILES_BANK31_FRAME043),
    { { -31, -15, 63, 32 } },
};
const struct sprite_frame_1box gSpriteBank31Frame044 = {
    SPRITE_FRAME(gSpriteBank31Frame044, SPRITE_TILES_BANK31_FRAME044),
    { { -31, -15, 63, 34 } },
};
const struct sprite_frame_1box gSpriteBank31Frame045 = {
    SPRITE_FRAME(gSpriteBank31Frame045, SPRITE_TILES_BANK31_FRAME045),
    { { -31, -15, 63, 34 } },
};
const struct sprite_frame_1box gSpriteBank31Frame046 = {
    SPRITE_FRAME(gSpriteBank31Frame046, SPRITE_TILES_BANK31_FRAME046),
    { { -31, -15, 63, 31 } },
};
const struct sprite_frame_1box gSpriteBank31Frame047 = {
    SPRITE_FRAME(gSpriteBank31Frame047, SPRITE_TILES_BANK31_FRAME047),
    { { -31, -16, 63, 32 } },
};
const struct sprite_frame gSpriteBank31Frame048 = SPRITE_FRAME(gSpriteBank31Frame048, SPRITE_TILES_BANK31_FRAME048);
const struct sprite_frame gSpriteBank31Frame049 = SPRITE_FRAME(gSpriteBank31Frame049, SPRITE_TILES_BANK31_FRAME049);
const struct sprite_frame gSpriteBank31Frame050 = SPRITE_FRAME(gSpriteBank31Frame050, SPRITE_TILES_BANK31_FRAME050);
const struct sprite_frame gSpriteBank31Frame051 = SPRITE_FRAME(gSpriteBank31Frame051, SPRITE_TILES_BANK31_FRAME051);
const struct sprite_frame gSpriteBank31Frame052 = SPRITE_FRAME(gSpriteBank31Frame052, SPRITE_TILES_BANK31_FRAME052);
const struct sprite_frame gSpriteBank31Frame053 = SPRITE_FRAME(gSpriteBank31Frame053, SPRITE_TILES_BANK31_FRAME053);
const struct sprite_frame gSpriteBank31Frame054 = SPRITE_FRAME(gSpriteBank31Frame054, SPRITE_TILES_BANK31_FRAME054);
const struct sprite_frame gSpriteBank31Frame055 = SPRITE_FRAME(gSpriteBank31Frame055, SPRITE_TILES_BANK31_FRAME055);
const struct sprite_frame gSpriteBank31Frame056 = SPRITE_FRAME(gSpriteBank31Frame056, SPRITE_TILES_BANK31_FRAME056);
const struct sprite_frame gSpriteBank31Frame057 = SPRITE_FRAME(gSpriteBank31Frame057, SPRITE_TILES_BANK31_FRAME057);
const struct sprite_frame gSpriteBank31Frame058 = SPRITE_FRAME(gSpriteBank31Frame058, SPRITE_TILES_BANK31_FRAME058);
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
const struct sprite_frame gSpriteBank31Frame094 = SPRITE_FRAME(gSpriteBank31Frame094, SPRITE_TILES_BANK31_FRAME094);
const struct sprite_frame gSpriteBank31Frame095 = SPRITE_FRAME(gSpriteBank31Frame095, SPRITE_TILES_BANK31_FRAME095);
const struct sprite_frame gSpriteBank31Frame096 = SPRITE_FRAME(gSpriteBank31Frame096, SPRITE_TILES_BANK31_FRAME096);

/* The piece arrays, from games/n2/graphics/sprites/bank31_platforms/bank.json. */
#include "sprites/bank31_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 32: 3 animations, 24 frames, games/n2/graphics/sprites/bank32_spark/. */

extern const u16 gSpriteBank32Anim00Seq[6];
extern const u16 gSpriteBank32Anim01Seq[10];
extern const u16 gSpriteBank32Anim02Seq[8];

extern const struct sprite_anim gSpriteBank32Anims[3] = {
    /* 0 */ {
        /* seq */ gSpriteBank32Anim00Seq,
        /* box */ { { -2, -1, 5, 3 }, { -3, -4, 7, 6 } },
        /* paletteId */ 30,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank32Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank32Anim01Seq,
        /* box */ { { -4, -1, 8, 2 }, { -20, -23, 25, 24 } },
        /* paletteId */ 30,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank32Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank32Anim02Seq,
        /* box */ { { -9, -8, 19, 16 }, { -11, -17, 24, 25 } },
        /* paletteId */ 30,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank32Anim02Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank32Anim00Seq[6] = {
    0, 1, 2, 3, 4, 5,
};
const u16 gSpriteBank32Anim01Seq[10] = {
    6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
};
const u16 gSpriteBank32Anim02Seq[8] = {
    16, 17, 18, 19, 20, 21, 22, 23,
};

extern const struct sprite_frame *const gSpriteBank32Frames[24] = {
    &gSpriteBank32Frame000.frame,
    &gSpriteBank32Frame001.frame,
    &gSpriteBank32Frame002.frame,
    &gSpriteBank32Frame003.frame,
    &gSpriteBank32Frame004.frame,
    &gSpriteBank32Frame005.frame,
    &gSpriteBank32Frame006.frame,
    &gSpriteBank32Frame007.frame,
    &gSpriteBank32Frame008.frame,
    &gSpriteBank32Frame009.frame,
    &gSpriteBank32Frame010.frame,
    &gSpriteBank32Frame011.frame,
    &gSpriteBank32Frame012.frame,
    &gSpriteBank32Frame013.frame,
    &gSpriteBank32Frame014.frame,
    &gSpriteBank32Frame015.frame,
    &gSpriteBank32Frame016.frame,
    &gSpriteBank32Frame017.frame,
    &gSpriteBank32Frame018.frame,
    &gSpriteBank32Frame019.frame,
    &gSpriteBank32Frame020.frame,
    &gSpriteBank32Frame021.frame,
    &gSpriteBank32Frame022.frame,
    &gSpriteBank32Frame023.frame,
};

const struct sprite_frame_1box gSpriteBank32Frame000 = {
    SPRITE_FRAME(gSpriteBank32Frame000, SPRITE_TILES_BANK32_FRAME000),
    { { -2, -1, 5, 3 } },
};
const struct sprite_frame_1box gSpriteBank32Frame001 = {
    SPRITE_FRAME(gSpriteBank32Frame001, SPRITE_TILES_BANK32_FRAME001),
    { { -3, -4, 7, 5 } },
};
const struct sprite_frame_1box gSpriteBank32Frame002 = {
    SPRITE_FRAME(gSpriteBank32Frame002, SPRITE_TILES_BANK32_FRAME002),
    { { -2, -4, 5, 5 } },
};
const struct sprite_frame_1box gSpriteBank32Frame003 = {
    SPRITE_FRAME(gSpriteBank32Frame003, SPRITE_TILES_BANK32_FRAME003),
    { { -1, -4, 3, 4 } },
};
const struct sprite_frame_1box gSpriteBank32Frame004 = {
    SPRITE_FRAME(gSpriteBank32Frame004, SPRITE_TILES_BANK32_FRAME004),
    { { -1, -4, 3, 3 } },
};
const struct sprite_frame_1box gSpriteBank32Frame005 = {
    SPRITE_FRAME(gSpriteBank32Frame005, SPRITE_TILES_BANK32_FRAME005),
    { { -1, -4, 1, 1 } },
};
const struct sprite_frame_1box gSpriteBank32Frame006 = {
    SPRITE_FRAME(gSpriteBank32Frame006, SPRITE_TILES_BANK32_FRAME006),
    { { -4, -1, 8, 2 } },
};
const struct sprite_frame_1box gSpriteBank32Frame007 = {
    SPRITE_FRAME(gSpriteBank32Frame007, SPRITE_TILES_BANK32_FRAME007),
    { { -10, -2, 15, 3 } },
};
const struct sprite_frame_1box gSpriteBank32Frame008 = {
    SPRITE_FRAME(gSpriteBank32Frame008, SPRITE_TILES_BANK32_FRAME008),
    { { -15, -6, 20, 7 } },
};
const struct sprite_frame_1box gSpriteBank32Frame009 = {
    SPRITE_FRAME(gSpriteBank32Frame009, SPRITE_TILES_BANK32_FRAME009),
    { { -17, -11, 18, 12 } },
};
const struct sprite_frame_1box gSpriteBank32Frame010 = {
    SPRITE_FRAME(gSpriteBank32Frame010, SPRITE_TILES_BANK32_FRAME010),
    { { -20, -16, 17, 17 } },
};
const struct sprite_frame_1box gSpriteBank32Frame011 = {
    SPRITE_FRAME(gSpriteBank32Frame011, SPRITE_TILES_BANK32_FRAME011),
    { { -20, -17, 13, 16 } },
};
const struct sprite_frame_1box gSpriteBank32Frame012 = {
    SPRITE_FRAME(gSpriteBank32Frame012, SPRITE_TILES_BANK32_FRAME012),
    { { -20, -18, 12, 15 } },
};
const struct sprite_frame_1box gSpriteBank32Frame013 = {
    SPRITE_FRAME(gSpriteBank32Frame013, SPRITE_TILES_BANK32_FRAME013),
    { { -19, -22, 9, 15 } },
};
const struct sprite_frame_1box gSpriteBank32Frame014 = {
    SPRITE_FRAME(gSpriteBank32Frame014, SPRITE_TILES_BANK32_FRAME014),
    { { -18, -23, 6, 9 } },
};
const struct sprite_frame_1box gSpriteBank32Frame015 = {
    SPRITE_FRAME(gSpriteBank32Frame015, SPRITE_TILES_BANK32_FRAME015),
    { { -17, -22, 5, 5 } },
};
const struct sprite_frame_1box gSpriteBank32Frame016 = {
    SPRITE_FRAME(gSpriteBank32Frame016, SPRITE_TILES_BANK32_FRAME016),
    { { -9, -8, 19, 16 } },
};
const struct sprite_frame_1box gSpriteBank32Frame017 = {
    SPRITE_FRAME(gSpriteBank32Frame017, SPRITE_TILES_BANK32_FRAME017),
    { { -11, -12, 24, 20 } },
};
const struct sprite_frame_1box gSpriteBank32Frame018 = {
    SPRITE_FRAME(gSpriteBank32Frame018, SPRITE_TILES_BANK32_FRAME018),
    { { -10, -12, 22, 19 } },
};
const struct sprite_frame_1box gSpriteBank32Frame019 = {
    SPRITE_FRAME(gSpriteBank32Frame019, SPRITE_TILES_BANK32_FRAME019),
    { { -9, -12, 21, 16 } },
};
const struct sprite_frame_1box gSpriteBank32Frame020 = {
    SPRITE_FRAME(gSpriteBank32Frame020, SPRITE_TILES_BANK32_FRAME020),
    { { -8, -12, 18, 14 } },
};
const struct sprite_frame_1box gSpriteBank32Frame021 = {
    SPRITE_FRAME(gSpriteBank32Frame021, SPRITE_TILES_BANK32_FRAME021),
    { { -6, -14, 16, 11 } },
};
const struct sprite_frame_1box gSpriteBank32Frame022 = {
    SPRITE_FRAME(gSpriteBank32Frame022, SPRITE_TILES_BANK32_FRAME022),
    { { -6, -14, 16, 6 } },
};
const struct sprite_frame_1box gSpriteBank32Frame023 = {
    SPRITE_FRAME(gSpriteBank32Frame023, SPRITE_TILES_BANK32_FRAME023),
    { { -3, -17, 10, 5 } },
};

/* The piece arrays, from games/n2/graphics/sprites/bank32_spark/bank.json. */
#include "sprites/bank32_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 33: 1 animation, 11 frames, games/n2/graphics/sprites/bank33_checkpoint_text/. */

extern const u16 gSpriteBank33Anim00Seq[24];

extern const struct sprite_anim gSpriteBank33Anims[1] = {
    /* 0 */ {
        /* seq */ gSpriteBank33Anim00Seq,
        /* box */ { { -3, -3, 7, 7 }, { -3, -3, 39, 7 } },
        /* paletteId */ 39,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank33Anim00Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank33Anim00Seq[24] = {
    0, 0, 1, 2, 3, 4, 4, 4, 3, 2, 1, 0, 5, 6, 7, 8,
    9, 10, 10, 10, 9, 8, 7, 6,
};

extern const struct sprite_frame *const gSpriteBank33Frames[11] = {
    &gSpriteBank33Frame000.frame,
    &gSpriteBank33Frame001.frame,
    &gSpriteBank33Frame002.frame,
    &gSpriteBank33Frame003.frame,
    &gSpriteBank33Frame004.frame,
    &gSpriteBank33Frame005,
    &gSpriteBank33Frame006.frame,
    &gSpriteBank33Frame007.frame,
    &gSpriteBank33Frame008.frame,
    &gSpriteBank33Frame009.frame,
    &gSpriteBank33Frame010.frame,
};

const struct sprite_frame_1box gSpriteBank33Frame000 = {
    SPRITE_FRAME(gSpriteBank33Frame000, SPRITE_TILES_BANK33_FRAME000),
    { { -3, -3, 7, 7 } },
};
const struct sprite_frame_1box gSpriteBank33Frame001 = {
    SPRITE_FRAME(gSpriteBank33Frame001, SPRITE_TILES_BANK33_FRAME001),
    { { -3, -3, 15, 7 } },
};
const struct sprite_frame_1box gSpriteBank33Frame002 = {
    SPRITE_FRAME(gSpriteBank33Frame002, SPRITE_TILES_BANK33_FRAME002),
    { { -3, -3, 23, 7 } },
};
const struct sprite_frame_1box gSpriteBank33Frame003 = {
    SPRITE_FRAME(gSpriteBank33Frame003, SPRITE_TILES_BANK33_FRAME003),
    { { -3, -3, 31, 7 } },
};
const struct sprite_frame_1box gSpriteBank33Frame004 = {
    SPRITE_FRAME(gSpriteBank33Frame004, SPRITE_TILES_BANK33_FRAME004),
    { { -3, -3, 39, 7 } },
};
const struct sprite_frame gSpriteBank33Frame005 = { gSpriteBank33Frame006Pos, gSpriteBank33Frame006Pieces, SPRITE_FRAME_TILES(SPRITE_TILES_BANK33_FRAME005, 0) };
const struct sprite_frame_1box gSpriteBank33Frame006 = {
    SPRITE_FRAME(gSpriteBank33Frame006, SPRITE_TILES_BANK33_FRAME006),
    { { -3, -3, 7, 7 } },
};
const struct sprite_frame_1box gSpriteBank33Frame007 = {
    SPRITE_FRAME(gSpriteBank33Frame007, SPRITE_TILES_BANK33_FRAME007),
    { { -3, -3, 15, 7 } },
};
const struct sprite_frame_1box gSpriteBank33Frame008 = {
    SPRITE_FRAME(gSpriteBank33Frame008, SPRITE_TILES_BANK33_FRAME008),
    { { -3, -3, 21, 7 } },
};
const struct sprite_frame_1box gSpriteBank33Frame009 = {
    SPRITE_FRAME(gSpriteBank33Frame009, SPRITE_TILES_BANK33_FRAME009),
    { { -3, -3, 30, 7 } },
};
const struct sprite_frame_1box gSpriteBank33Frame010 = {
    SPRITE_FRAME(gSpriteBank33Frame010, SPRITE_TILES_BANK33_FRAME010),
    { { -3, -3, 38, 7 } },
};

/* The piece arrays, from games/n2/graphics/sprites/bank33_checkpoint_text/bank.json. */
#include "sprites/bank33_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 34: 7 animations, 21 frames, games/n2/graphics/sprites/bank34_sparkle/. */

extern const u16 gSpriteBank34Anim00Seq[10];
extern const u16 gSpriteBank34Anim01Seq[10];
extern const u16 gSpriteBank34Anim02Seq[10];
extern const u16 gSpriteBank34Anim03Seq[10];
extern const u16 gSpriteBank34Anim04Seq[10];
extern const u16 gSpriteBank34Anim05Seq[10];
extern const u16 gSpriteBank34Anim06Seq[10];

extern const struct sprite_anim gSpriteBank34Anims[7] = {
    /* 0 */ {
        /* seq */ gSpriteBank34Anim00Seq,
        /* box */ { { -11, -11, 22, 23 }, { -25, -26, 49, 57 } },
        /* paletteId */ 48,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank34Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank34Anim01Seq,
        /* box */ { { -11, -11, 22, 23 }, { -25, -26, 49, 57 } },
        /* paletteId */ 13,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank34Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank34Anim02Seq,
        /* box */ { { -21, -23, 42, 47 }, { -25, -28, 49, 57 } },
        /* paletteId */ 13,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank34Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank34Anim03Seq,
        /* box */ { { -11, -11, 22, 23 }, { -25, -26, 49, 57 } },
        /* paletteId */ 33,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank34Anim03Seq),
        /* flags */ 0,
    },
    /* 4 */ {
        /* seq */ gSpriteBank34Anim04Seq,
        /* box */ { { -11, -11, 22, 23 }, { -25, -26, 49, 57 } },
        /* paletteId */ 7,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank34Anim04Seq),
        /* flags */ 0,
    },
    /* 5 */ {
        /* seq */ gSpriteBank34Anim05Seq,
        /* box */ { { -11, -11, 22, 23 }, { -25, -26, 49, 57 } },
        /* paletteId */ 34,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank34Anim05Seq),
        /* flags */ 0,
    },
    /* 6 */ {
        /* seq */ gSpriteBank34Anim06Seq,
        /* box */ { { -11, -11, 22, 23 }, { -25, -26, 49, 57 } },
        /* paletteId */ 32,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank34Anim06Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank34Anim00Seq[10] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
};
const u16 gSpriteBank34Anim01Seq[10] = {
    0, 1, 2, 3, 10, 5, 6, 7, 8, 9,
};
const u16 gSpriteBank34Anim02Seq[10] = {
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
};
const u16 gSpriteBank34Anim03Seq[10] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
};
const u16 gSpriteBank34Anim04Seq[10] = {
    0, 1, 2, 3, 10, 5, 6, 7, 8, 9,
};
const u16 gSpriteBank34Anim05Seq[10] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
};
const u16 gSpriteBank34Anim06Seq[10] = {
    0, 1, 2, 3, 10, 5, 6, 7, 8, 9,
};

extern const struct sprite_frame *const gSpriteBank34Frames[21] = {
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
};

const struct sprite_frame_1box gSpriteBank34Frame000 = {
    SPRITE_FRAME(gSpriteBank34Frame000, SPRITE_TILES_BANK34_FRAME000),
    { { -11, -11, 22, 23 } },
};
const struct sprite_frame_1box gSpriteBank34Frame001 = {
    SPRITE_FRAME(gSpriteBank34Frame001, SPRITE_TILES_BANK34_FRAME001),
    { { -17, -18, 38, 39 } },
};
const struct sprite_frame_1box gSpriteBank34Frame002 = {
    SPRITE_FRAME(gSpriteBank34Frame002, SPRITE_TILES_BANK34_FRAME002),
    { { -20, -19, 40, 43 } },
};
const struct sprite_frame_1box gSpriteBank34Frame003 = {
    SPRITE_FRAME(gSpriteBank34Frame003, SPRITE_TILES_BANK34_FRAME003),
    { { -25, -25, 49, 55 } },
};
const struct sprite_frame_1box gSpriteBank34Frame004 = {
    SPRITE_FRAME(gSpriteBank34Frame004, SPRITE_TILES_BANK34_FRAME004),
    { { -25, -26, 49, 57 } },
};
const struct sprite_frame_1box gSpriteBank34Frame005 = {
    SPRITE_FRAME(gSpriteBank34Frame005, SPRITE_TILES_BANK34_FRAME005),
    { { -25, -25, 49, 55 } },
};
const struct sprite_frame_1box gSpriteBank34Frame006 = {
    SPRITE_FRAME(gSpriteBank34Frame006, SPRITE_TILES_BANK34_FRAME006),
    { { -24, -24, 47, 54 } },
};
const struct sprite_frame_1box gSpriteBank34Frame007 = {
    SPRITE_FRAME(gSpriteBank34Frame007, SPRITE_TILES_BANK34_FRAME007),
    { { -23, -24, 46, 53 } },
};
const struct sprite_frame_1box gSpriteBank34Frame008 = {
    SPRITE_FRAME(gSpriteBank34Frame008, SPRITE_TILES_BANK34_FRAME008),
    { { -22, -22, 44, 49 } },
};
const struct sprite_frame_1box gSpriteBank34Frame009 = {
    SPRITE_FRAME(gSpriteBank34Frame009, SPRITE_TILES_BANK34_FRAME009),
    { { -21, -21, 42, 47 } },
};
const struct sprite_frame_1box gSpriteBank34Frame010 = {
    SPRITE_FRAME(gSpriteBank34Frame010, SPRITE_TILES_BANK34_FRAME010),
    { { -25, -26, 49, 57 } },
};
const struct sprite_frame_1box gSpriteBank34Frame011 = {
    SPRITE_FRAME(gSpriteBank34Frame011, SPRITE_TILES_BANK34_FRAME011),
    { { -21, -23, 42, 47 } },
};
const struct sprite_frame_1box gSpriteBank34Frame012 = {
    SPRITE_FRAME(gSpriteBank34Frame012, SPRITE_TILES_BANK34_FRAME012),
    { { -22, -24, 44, 49 } },
};
const struct sprite_frame_1box gSpriteBank34Frame013 = {
    SPRITE_FRAME(gSpriteBank34Frame013, SPRITE_TILES_BANK34_FRAME013),
    { { -23, -26, 46, 53 } },
};
const struct sprite_frame_1box gSpriteBank34Frame014 = {
    SPRITE_FRAME(gSpriteBank34Frame014, SPRITE_TILES_BANK34_FRAME014),
    { { -24, -26, 47, 54 } },
};
const struct sprite_frame_1box gSpriteBank34Frame015 = {
    SPRITE_FRAME(gSpriteBank34Frame015, SPRITE_TILES_BANK34_FRAME015),
    { { -25, -27, 49, 55 } },
};
const struct sprite_frame_1box gSpriteBank34Frame016 = {
    SPRITE_FRAME(gSpriteBank34Frame016, SPRITE_TILES_BANK34_FRAME016),
    { { -25, -28, 49, 57 } },
};
const struct sprite_frame_1box gSpriteBank34Frame017 = {
    SPRITE_FRAME(gSpriteBank34Frame017, SPRITE_TILES_BANK34_FRAME017),
    { { -25, -27, 49, 55 } },
};
const struct sprite_frame_1box gSpriteBank34Frame018 = {
    SPRITE_FRAME(gSpriteBank34Frame018, SPRITE_TILES_BANK34_FRAME018),
    { { -20, -21, 40, 43 } },
};
const struct sprite_frame_1box gSpriteBank34Frame019 = {
    SPRITE_FRAME(gSpriteBank34Frame019, SPRITE_TILES_BANK34_FRAME019),
    { { -17, -20, 38, 39 } },
};
const struct sprite_frame_1box gSpriteBank34Frame020 = {
    SPRITE_FRAME(gSpriteBank34Frame020, SPRITE_TILES_BANK34_FRAME020),
    { { -11, -13, 22, 23 } },
};

/* The piece arrays, from games/n2/graphics/sprites/bank34_sparkle/bank.json. */
#include "sprites/bank34_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 35: 1 animation, 13 frames, games/n2/graphics/sprites/bank35_flame/. */

extern const u16 gSpriteBank35Anim00Seq[13];

extern const struct sprite_anim gSpriteBank35Anims[1] = {
    /* 0 */ {
        /* seq */ gSpriteBank35Anim00Seq,
        /* box */ { { -3, -7, 7, 14 }, { -5, -11, 12, 19 } },
        /* paletteId */ 25,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank35Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank35Anim00Seq[13] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
};

extern const struct sprite_frame *const gSpriteBank35Frames[13] = {
    &gSpriteBank35Frame000.frame,
    &gSpriteBank35Frame001.frame,
    &gSpriteBank35Frame002.frame,
    &gSpriteBank35Frame003.frame,
    &gSpriteBank35Frame004.frame,
    &gSpriteBank35Frame005.frame,
    &gSpriteBank35Frame006.frame,
    &gSpriteBank35Frame007.frame,
    &gSpriteBank35Frame008.frame,
    &gSpriteBank35Frame009.frame,
    &gSpriteBank35Frame010.frame,
    &gSpriteBank35Frame011.frame,
    &gSpriteBank35Frame012.frame,
};

const struct sprite_frame_1box gSpriteBank35Frame000 = {
    SPRITE_FRAME(gSpriteBank35Frame000, SPRITE_TILES_BANK35_FRAME000),
    { { -3, -7, 7, 14 } },
};
const struct sprite_frame_1box gSpriteBank35Frame001 = {
    SPRITE_FRAME(gSpriteBank35Frame001, SPRITE_TILES_BANK35_FRAME001),
    { { -4, -8, 9, 16 } },
};
const struct sprite_frame_1box gSpriteBank35Frame002 = {
    SPRITE_FRAME(gSpriteBank35Frame002, SPRITE_TILES_BANK35_FRAME002),
    { { -5, -9, 9, 17 } },
};
const struct sprite_frame_1box gSpriteBank35Frame003 = {
    SPRITE_FRAME(gSpriteBank35Frame003, SPRITE_TILES_BANK35_FRAME003),
    { { -4, -5, 9, 11 } },
};
const struct sprite_frame_1box gSpriteBank35Frame004 = {
    SPRITE_FRAME(gSpriteBank35Frame004, SPRITE_TILES_BANK35_FRAME004),
    { { -5, -5, 9, 13 } },
};
const struct sprite_frame_1box gSpriteBank35Frame005 = {
    SPRITE_FRAME(gSpriteBank35Frame005, SPRITE_TILES_BANK35_FRAME005),
    { { -5, -9, 11, 17 } },
};
const struct sprite_frame_1box gSpriteBank35Frame006 = {
    SPRITE_FRAME(gSpriteBank35Frame006, SPRITE_TILES_BANK35_FRAME006),
    { { -3, -11, 10, 18 } },
};
const struct sprite_frame_1box gSpriteBank35Frame007 = {
    SPRITE_FRAME(gSpriteBank35Frame007, SPRITE_TILES_BANK35_FRAME007),
    { { -3, -9, 8, 15 } },
};
const struct sprite_frame_1box gSpriteBank35Frame008 = {
    SPRITE_FRAME(gSpriteBank35Frame008, SPRITE_TILES_BANK35_FRAME008),
    { { -4, -11, 9, 19 } },
};
const struct sprite_frame_1box gSpriteBank35Frame009 = {
    SPRITE_FRAME(gSpriteBank35Frame009, SPRITE_TILES_BANK35_FRAME009),
    { { -4, -11, 8, 18 } },
};
const struct sprite_frame_1box gSpriteBank35Frame010 = {
    SPRITE_FRAME(gSpriteBank35Frame010, SPRITE_TILES_BANK35_FRAME010),
    { { -4, -11, 7, 17 } },
};
const struct sprite_frame_1box gSpriteBank35Frame011 = {
    SPRITE_FRAME(gSpriteBank35Frame011, SPRITE_TILES_BANK35_FRAME011),
    { { -4, -6, 7, 13 } },
};
const struct sprite_frame_1box gSpriteBank35Frame012 = {
    SPRITE_FRAME(gSpriteBank35Frame012, SPRITE_TILES_BANK35_FRAME012),
    { { -4, -9, 8, 16 } },
};

/* The piece arrays, from games/n2/graphics/sprites/bank35_flame/bank.json. */
#include "sprites/bank35_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 36: 4 animations, 13 frames, games/n2/graphics/sprites/bank36_dialogue/. */

extern const u16 gSpriteBank36Anim00Seq[5];
extern const u16 gSpriteBank36Anim01Seq[1];
extern const u16 gSpriteBank36Anim02Seq[5];
extern const u16 gSpriteBank36Anim03Seq[5];

extern const struct sprite_anim gSpriteBank36Anims[4] = {
    /* 0 */ {
        /* seq */ gSpriteBank36Anim00Seq,
        /* box */ { { -23, -20, 46, 41 }, { -23, -20, 46, 41 } },
        /* paletteId */ 49,
        /* duration */ 9,
        /* frameCount */ ARRAY_COUNT(gSpriteBank36Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank36Anim01Seq,
        /* box */ { { -76, -28, 153, 32 }, { -79, -31, 159, 63 } },
        /* paletteId */ 38,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank36Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank36Anim02Seq,
        /* box */ { { -16, -22, 32, 45 }, { -16, -22, 32, 45 } },
        /* paletteId */ 50,
        /* duration */ 9,
        /* frameCount */ ARRAY_COUNT(gSpriteBank36Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank36Anim03Seq,
        /* box */ { { -21, -17, 42, 34 }, { -21, -17, 42, 34 } },
        /* paletteId */ 51,
        /* duration */ 14,
        /* frameCount */ ARRAY_COUNT(gSpriteBank36Anim03Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank36Anim00Seq[5] = {
    0, 1, 2, 3, 3,
};
const u16 gSpriteBank36Anim01Seq[1] = {
    4,
};
const u16 gSpriteBank36Anim02Seq[5] = {
    5, 6, 7, 8, 8,
};
const u16 gSpriteBank36Anim03Seq[5] = {
    9, 10, 11, 12, 12,
};

extern const struct sprite_frame *const gSpriteBank36Frames[13] = {
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
    &gSpriteBank36Frame010,
    &gSpriteBank36Frame011,
    &gSpriteBank36Frame012,
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
const struct sprite_frame gSpriteBank36Frame010 = SPRITE_FRAME(gSpriteBank36Frame010, SPRITE_TILES_BANK36_FRAME010);
const struct sprite_frame gSpriteBank36Frame011 = SPRITE_FRAME(gSpriteBank36Frame011, SPRITE_TILES_BANK36_FRAME011);
const struct sprite_frame gSpriteBank36Frame012 = SPRITE_FRAME(gSpriteBank36Frame012, SPRITE_TILES_BANK36_FRAME012);

/* The piece arrays, from games/n2/graphics/sprites/bank36_dialogue/bank.json. */
#include "sprites/bank36_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 37: 10 animations, 48 frames, games/n2/graphics/sprites/bank37_traps/. */

extern const u16 gSpriteBank37Anim00Seq[2];
extern const u16 gSpriteBank37Anim01Seq[1];
extern const u16 gSpriteBank37Anim02Seq[43];
extern const u16 gSpriteBank37Anim03Seq[10];
extern const u16 gSpriteBank37Anim04Seq[1];
extern const u16 gSpriteBank37Anim05Seq[7];
extern const u16 gSpriteBank37Anim06Seq[1];
extern const u16 gSpriteBank37Anim07Seq[30];
extern const u16 gSpriteBank37Anim08Seq[7];
extern const u16 gSpriteBank37Anim09Seq[1];

extern const struct sprite_anim gSpriteBank37Anims[10] = {
    /* 0 */ {
        /* seq */ gSpriteBank37Anim00Seq,
        /* box */ { { -27, 5, 32, 11 }, { -27, -15, 55, 31 } },
        /* paletteId */ 52,
        /* duration */ 14,
        /* frameCount */ ARRAY_COUNT(gSpriteBank37Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank37Anim01Seq,
        /* box */ { { -26, -37, 30, 90 }, { -27, -59, 55, 119 } },
        /* paletteId */ 53,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank37Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank37Anim02Seq,
        /* box */ { { -15, -10, 31, 21 }, { -15, -10, 31, 22 } },
        /* paletteId */ 54,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank37Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank37Anim03Seq,
        /* box */ { { -11, -7, 24, 35 }, { -11, 4, 23, 24 } },
        /* paletteId */ 45,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank37Anim03Seq),
        /* flags */ 0,
    },
    /* 4 */ {
        /* seq */ gSpriteBank37Anim04Seq,
        /* box */ { { -31, -23, 29, 34 }, { -31, -33, 42, 47 } },
        /* paletteId */ 53,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank37Anim04Seq),
        /* flags */ 0,
    },
    /* 5 */ {
        /* seq */ gSpriteBank37Anim05Seq,
        /* box */ { { -11, -7, 24, 35 }, { -11, -19, 23, 47 } },
        /* paletteId */ 45,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank37Anim05Seq),
        /* flags */ 0,
    },
    /* 6 */ {
        /* seq */ gSpriteBank37Anim06Seq,
        /* box */ { { -22, -63, 25, 126 }, { -23, -63, 46, 126 } },
        /* paletteId */ 55,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank37Anim06Seq),
        /* flags */ 0,
    },
    /* 7 */ {
        /* seq */ gSpriteBank37Anim07Seq,
        /* box */ { { -8, -4, 17, 9 }, { -10, -31, 20, 36 } },
        /* paletteId */ 29,
        /* duration */ 2,
        /* frameCount */ ARRAY_COUNT(gSpriteBank37Anim07Seq),
        /* flags */ 0,
    },
    /* 8 */ {
        /* seq */ gSpriteBank37Anim08Seq,
        /* box */ { { -11, -7, 24, 35 }, { -11, -15, 23, 43 } },
        /* paletteId */ 45,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank37Anim08Seq),
        /* flags */ 0,
    },
    /* 9 */ {
        /* seq */ gSpriteBank37Anim09Seq,
        /* box */ { { -7, -7, 15, 15 }, { -7, -15, 23, 23 } },
        /* paletteId */ 56,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank37Anim09Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank37Anim00Seq[2] = {
    0, 1,
};
const u16 gSpriteBank37Anim01Seq[1] = {
    2,
};
const u16 gSpriteBank37Anim02Seq[43] = {
    3, 4, 5, 6, 7, 8, 4, 6, 8, 3, 6, 3, 6, 3, 6, 3,
    6, 3, 6, 4, 6, 8, 4, 6, 8, 3, 4, 5, 6, 7, 8, 3,
    3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8,
};
const u16 gSpriteBank37Anim03Seq[10] = {
    9, 10, 11, 12, 12, 12, 12, 12, 12, 12,
};
const u16 gSpriteBank37Anim04Seq[1] = {
    13,
};
const u16 gSpriteBank37Anim05Seq[7] = {
    14, 15, 15, 15, 15, 15, 15,
};
const u16 gSpriteBank37Anim06Seq[1] = {
    16,
};
const u16 gSpriteBank37Anim07Seq[30] = {
    17, 17, 18, 18, 18, 19, 20, 21, 22, 23, 24, 25, 25, 26, 27, 28,
    29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
};
const u16 gSpriteBank37Anim08Seq[7] = {
    43, 44, 45, 46, 12, 10, 9,
};
const u16 gSpriteBank37Anim09Seq[1] = {
    47,
};

extern const struct sprite_frame *const gSpriteBank37Frames[48] = {
    &gSpriteBank37Frame000,
    &gSpriteBank37Frame001,
    &gSpriteBank37Frame002.frame,
    &gSpriteBank37Frame003,
    &gSpriteBank37Frame004,
    &gSpriteBank37Frame005,
    &gSpriteBank37Frame006,
    &gSpriteBank37Frame007,
    &gSpriteBank37Frame008,
    &gSpriteBank37Frame009,
    &gSpriteBank37Frame010,
    &gSpriteBank37Frame011,
    &gSpriteBank37Frame012,
    &gSpriteBank37Frame013.frame,
    &gSpriteBank37Frame014.frame,
    &gSpriteBank37Frame015.frame,
    &gSpriteBank37Frame016.frame,
    &gSpriteBank37Frame017,
    &gSpriteBank37Frame018,
    &gSpriteBank37Frame019,
    &gSpriteBank37Frame020,
    &gSpriteBank37Frame021,
    &gSpriteBank37Frame022,
    &gSpriteBank37Frame023,
    &gSpriteBank37Frame024,
    &gSpriteBank37Frame025,
    &gSpriteBank37Frame026,
    &gSpriteBank37Frame027,
    &gSpriteBank37Frame028.frame,
    &gSpriteBank37Frame029.frame,
    &gSpriteBank37Frame030.frame,
    &gSpriteBank37Frame031.frame,
    &gSpriteBank37Frame032.frame,
    &gSpriteBank37Frame033.frame,
    &gSpriteBank37Frame034.frame,
    &gSpriteBank37Frame035.frame,
    &gSpriteBank37Frame036.frame,
    &gSpriteBank37Frame037.frame,
    &gSpriteBank37Frame038.frame,
    &gSpriteBank37Frame039.frame,
    &gSpriteBank37Frame040,
    &gSpriteBank37Frame041,
    &gSpriteBank37Frame042,
    &gSpriteBank37Frame043.frame,
    &gSpriteBank37Frame044.frame,
    &gSpriteBank37Frame045.frame,
    &gSpriteBank37Frame046,
    &gSpriteBank37Frame047.frame,
};

const struct sprite_frame gSpriteBank37Frame000 = SPRITE_FRAME(gSpriteBank37Frame000, SPRITE_TILES_BANK37_FRAME000);
const struct sprite_frame gSpriteBank37Frame001 = SPRITE_FRAME(gSpriteBank37Frame001, SPRITE_TILES_BANK37_FRAME001);
const struct sprite_frame_1box gSpriteBank37Frame002 = {
    SPRITE_FRAME(gSpriteBank37Frame002, SPRITE_TILES_BANK37_FRAME002),
    { { -23, 29, 24, 32 } },
};
const struct sprite_frame gSpriteBank37Frame003 = SPRITE_FRAME(gSpriteBank37Frame003, SPRITE_TILES_BANK37_FRAME003);
const struct sprite_frame gSpriteBank37Frame004 = SPRITE_FRAME(gSpriteBank37Frame004, SPRITE_TILES_BANK37_FRAME004);
const struct sprite_frame gSpriteBank37Frame005 = SPRITE_FRAME(gSpriteBank37Frame005, SPRITE_TILES_BANK37_FRAME005);
const struct sprite_frame gSpriteBank37Frame006 = SPRITE_FRAME(gSpriteBank37Frame006, SPRITE_TILES_BANK37_FRAME006);
const struct sprite_frame gSpriteBank37Frame007 = SPRITE_FRAME(gSpriteBank37Frame007, SPRITE_TILES_BANK37_FRAME007);
const struct sprite_frame gSpriteBank37Frame008 = SPRITE_FRAME(gSpriteBank37Frame008, SPRITE_TILES_BANK37_FRAME008);
const struct sprite_frame gSpriteBank37Frame009 = SPRITE_FRAME(gSpriteBank37Frame009, SPRITE_TILES_BANK37_FRAME009);
const struct sprite_frame gSpriteBank37Frame010 = SPRITE_FRAME(gSpriteBank37Frame010, SPRITE_TILES_BANK37_FRAME010);
const struct sprite_frame gSpriteBank37Frame011 = SPRITE_FRAME(gSpriteBank37Frame011, SPRITE_TILES_BANK37_FRAME011);
const struct sprite_frame gSpriteBank37Frame012 = SPRITE_FRAME(gSpriteBank37Frame012, SPRITE_TILES_BANK37_FRAME012);
const struct sprite_frame_1box gSpriteBank37Frame013 = {
    SPRITE_FRAME(gSpriteBank37Frame013, SPRITE_TILES_BANK37_FRAME013),
    { { -29, -3, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank37Frame014 = {
    SPRITE_FRAME(gSpriteBank37Frame014, SPRITE_TILES_BANK37_FRAME014),
    { { -6, -19, 18, 41 } },
};
const struct sprite_frame_1box gSpriteBank37Frame015 = {
    SPRITE_FRAME(gSpriteBank37Frame015, SPRITE_TILES_BANK37_FRAME015),
    { { -6, -19, 18, 41 } },
};
const struct sprite_frame_1box gSpriteBank37Frame016 = {
    SPRITE_FRAME(gSpriteBank37Frame016, SPRITE_TILES_BANK37_FRAME016),
    { { -17, -63, 17, 122 } },
};
const struct sprite_frame gSpriteBank37Frame017 = SPRITE_FRAME(gSpriteBank37Frame017, SPRITE_TILES_BANK37_FRAME017);
const struct sprite_frame gSpriteBank37Frame018 = SPRITE_FRAME(gSpriteBank37Frame018, SPRITE_TILES_BANK37_FRAME018);
const struct sprite_frame gSpriteBank37Frame019 = SPRITE_FRAME(gSpriteBank37Frame019, SPRITE_TILES_BANK37_FRAME019);
const struct sprite_frame gSpriteBank37Frame020 = SPRITE_FRAME(gSpriteBank37Frame020, SPRITE_TILES_BANK37_FRAME020);
const struct sprite_frame gSpriteBank37Frame021 = SPRITE_FRAME(gSpriteBank37Frame021, SPRITE_TILES_BANK37_FRAME021);
const struct sprite_frame gSpriteBank37Frame022 = SPRITE_FRAME(gSpriteBank37Frame022, SPRITE_TILES_BANK37_FRAME022);
const struct sprite_frame gSpriteBank37Frame023 = SPRITE_FRAME(gSpriteBank37Frame023, SPRITE_TILES_BANK37_FRAME023);
const struct sprite_frame gSpriteBank37Frame024 = SPRITE_FRAME(gSpriteBank37Frame024, SPRITE_TILES_BANK37_FRAME024);
const struct sprite_frame gSpriteBank37Frame025 = SPRITE_FRAME(gSpriteBank37Frame025, SPRITE_TILES_BANK37_FRAME025);
const struct sprite_frame gSpriteBank37Frame026 = SPRITE_FRAME(gSpriteBank37Frame026, SPRITE_TILES_BANK37_FRAME026);
const struct sprite_frame gSpriteBank37Frame027 = SPRITE_FRAME(gSpriteBank37Frame027, SPRITE_TILES_BANK37_FRAME027);
const struct sprite_frame_1box gSpriteBank37Frame028 = {
    SPRITE_FRAME(gSpriteBank37Frame028, SPRITE_TILES_BANK37_FRAME028),
    { { -5, -16, 11, 15 } },
};
const struct sprite_frame_1box gSpriteBank37Frame029 = {
    SPRITE_FRAME(gSpriteBank37Frame029, SPRITE_TILES_BANK37_FRAME029),
    { { -5, -24, 11, 23 } },
};
const struct sprite_frame_1box gSpriteBank37Frame030 = {
    SPRITE_FRAME(gSpriteBank37Frame030, SPRITE_TILES_BANK37_FRAME030),
    { { -7, -27, 14, 28 } },
};
const struct sprite_frame_1box gSpriteBank37Frame031 = {
    SPRITE_FRAME(gSpriteBank37Frame031, SPRITE_TILES_BANK37_FRAME031),
    { { -5, -24, 12, 24 } },
};
const struct sprite_frame_1box gSpriteBank37Frame032 = {
    SPRITE_FRAME(gSpriteBank37Frame032, SPRITE_TILES_BANK37_FRAME032),
    { { -3, -26, 10, 27 } },
};
const struct sprite_frame_1box gSpriteBank37Frame033 = {
    SPRITE_FRAME(gSpriteBank37Frame033, SPRITE_TILES_BANK37_FRAME033),
    { { -4, -28, 10, 29 } },
};
const struct sprite_frame_1box gSpriteBank37Frame034 = {
    SPRITE_FRAME(gSpriteBank37Frame034, SPRITE_TILES_BANK37_FRAME034),
    { { -4, -25, 11, 25 } },
};
const struct sprite_frame_1box gSpriteBank37Frame035 = {
    SPRITE_FRAME(gSpriteBank37Frame035, SPRITE_TILES_BANK37_FRAME035),
    { { -5, -23, 12, 24 } },
};
const struct sprite_frame_1box gSpriteBank37Frame036 = {
    SPRITE_FRAME(gSpriteBank37Frame036, SPRITE_TILES_BANK37_FRAME036),
    { { -5, -20, 12, 21 } },
};
const struct sprite_frame_1box gSpriteBank37Frame037 = {
    SPRITE_FRAME(gSpriteBank37Frame037, SPRITE_TILES_BANK37_FRAME037),
    { { -3, -15, 9, 17 } },
};
const struct sprite_frame_1box gSpriteBank37Frame038 = {
    SPRITE_FRAME(gSpriteBank37Frame038, SPRITE_TILES_BANK37_FRAME038),
    { { -4, -15, 9, 15 } },
};
const struct sprite_frame_1box gSpriteBank37Frame039 = {
    SPRITE_FRAME(gSpriteBank37Frame039, SPRITE_TILES_BANK37_FRAME039),
    { { -2, -13, 8, 12 } },
};
const struct sprite_frame gSpriteBank37Frame040 = SPRITE_FRAME(gSpriteBank37Frame040, SPRITE_TILES_BANK37_FRAME040);
const struct sprite_frame gSpriteBank37Frame041 = SPRITE_FRAME(gSpriteBank37Frame041, SPRITE_TILES_BANK37_FRAME041);
const struct sprite_frame gSpriteBank37Frame042 = SPRITE_FRAME(gSpriteBank37Frame042, SPRITE_TILES_BANK37_FRAME042);
const struct sprite_frame_1box gSpriteBank37Frame043 = {
    SPRITE_FRAME(gSpriteBank37Frame043, SPRITE_TILES_BANK37_FRAME043),
    { { -6, -14, 18, 36 } },
};
const struct sprite_frame_1box gSpriteBank37Frame044 = {
    SPRITE_FRAME(gSpriteBank37Frame044, SPRITE_TILES_BANK37_FRAME044),
    { { -6, -6, 18, 28 } },
};
const struct sprite_frame_1box gSpriteBank37Frame045 = {
    SPRITE_FRAME(gSpriteBank37Frame045, SPRITE_TILES_BANK37_FRAME045),
    { { -6, -3, 18, 25 } },
};
const struct sprite_frame gSpriteBank37Frame046 = SPRITE_FRAME(gSpriteBank37Frame046, SPRITE_TILES_BANK37_FRAME046);
const struct sprite_frame_1box gSpriteBank37Frame047 = {
    SPRITE_FRAME(gSpriteBank37Frame047, SPRITE_TILES_BANK37_FRAME047),
    { { -7, -7, 15, 15 } },
};

/* The piece arrays, from games/n2/graphics/sprites/bank37_traps/bank.json. */
#include "sprites/bank37_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 38: 4 animations, 40 frames, games/n2/graphics/sprites/bank38_bursts/. */

extern const u16 gSpriteBank38Anim00Seq[10];
extern const u16 gSpriteBank38Anim01Seq[10];
extern const u16 gSpriteBank38Anim02Seq[10];
extern const u16 gSpriteBank38Anim03Seq[10];

extern const struct sprite_anim gSpriteBank38Anims[4] = {
    /* 0 */ {
        /* seq */ gSpriteBank38Anim00Seq,
        /* box */ { { -13, -14, 27, 29 }, { -29, -30, 61, 63 } },
        /* paletteId */ 33,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank38Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank38Anim01Seq,
        /* box */ { { -5, -2, 10, 5 }, { -7, -8, 13, 13 } },
        /* paletteId */ 7,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank38Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank38Anim02Seq,
        /* box */ { { -13, -14, 27, 29 }, { -29, -30, 61, 63 } },
        /* paletteId */ 27,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank38Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank38Anim03Seq,
        /* box */ { { -11, -11, 27, 22 }, { -11, -12, 28, 25 } },
        /* paletteId */ 30,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank38Anim03Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank38Anim00Seq[10] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
};
const u16 gSpriteBank38Anim01Seq[10] = {
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
};
const u16 gSpriteBank38Anim02Seq[10] = {
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
};
const u16 gSpriteBank38Anim03Seq[10] = {
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
};

extern const struct sprite_frame *const gSpriteBank38Frames[40] = {
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

/* The piece arrays, from games/n2/graphics/sprites/bank38_bursts/bank.json. */
#include "sprites/bank38_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 39: 4 animations, 13 frames, games/n2/graphics/sprites/bank39_rocket_and_portals/. */

extern const u16 gSpriteBank39Anim00Seq[4];
extern const u16 gSpriteBank39Anim01Seq[1];
extern const u16 gSpriteBank39Anim02Seq[4];
extern const u16 gSpriteBank39Anim03Seq[4];

extern const struct sprite_anim gSpriteBank39Anims[4] = {
    /* 0 */ {
        /* seq */ gSpriteBank39Anim00Seq,
        /* box */ { { -7, -27, 14, 41 }, { -9, -34, 19, 58 } },
        /* paletteId */ 27,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank39Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank39Anim01Seq,
        /* box */ { { -7, -27, 14, 41 }, { -5, -34, 14, 57 } },
        /* paletteId */ 27,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank39Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank39Anim02Seq,
        /* box */ { { -22, -25, 45, 45 }, { -22, -25, 45, 45 } },
        /* paletteId */ 32,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank39Anim02Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 3 */ {
        /* seq */ gSpriteBank39Anim03Seq,
        /* box */ { { -22, -25, 45, 45 }, { -31, -31, 63, 63 } },
        /* paletteId */ 30,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank39Anim03Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank39Anim00Seq[4] = {
    0, 1, 2, 3,
};
const u16 gSpriteBank39Anim01Seq[1] = {
    4,
};
const u16 gSpriteBank39Anim02Seq[4] = {
    5, 6, 7, 8,
};
const u16 gSpriteBank39Anim03Seq[4] = {
    9, 10, 11, 12,
};

extern const struct sprite_frame *const gSpriteBank39Frames[13] = {
    &gSpriteBank39Frame000,
    &gSpriteBank39Frame001,
    &gSpriteBank39Frame002,
    &gSpriteBank39Frame003,
    &gSpriteBank39Frame004.frame,
    &gSpriteBank39Frame005.frame,
    &gSpriteBank39Frame006.frame,
    &gSpriteBank39Frame007.frame,
    &gSpriteBank39Frame008.frame,
    &gSpriteBank39Frame009,
    &gSpriteBank39Frame010,
    &gSpriteBank39Frame011,
    &gSpriteBank39Frame012,
};

const struct sprite_frame gSpriteBank39Frame000 = SPRITE_FRAME(gSpriteBank39Frame000, SPRITE_TILES_BANK39_FRAME000);
const struct sprite_frame gSpriteBank39Frame001 = SPRITE_FRAME(gSpriteBank39Frame001, SPRITE_TILES_BANK39_FRAME001);
const struct sprite_frame gSpriteBank39Frame002 = SPRITE_FRAME(gSpriteBank39Frame002, SPRITE_TILES_BANK39_FRAME002);
const struct sprite_frame gSpriteBank39Frame003 = SPRITE_FRAME(gSpriteBank39Frame003, SPRITE_TILES_BANK39_FRAME003);
const struct sprite_frame_1box gSpriteBank39Frame004 = {
    SPRITE_FRAME(gSpriteBank39Frame004, SPRITE_TILES_BANK39_FRAME004),
    { { -5, -34, 15, 58 } },
};
const struct sprite_frame_1box gSpriteBank39Frame005 = {
    SPRITE_FRAME(gSpriteBank39Frame005, SPRITE_TILES_BANK39_FRAME005),
    { { -1, -5, 4, 6 } },
};
const struct sprite_frame_1box gSpriteBank39Frame006 = {
    SPRITE_FRAME(gSpriteBank39Frame006, SPRITE_TILES_BANK39_FRAME006),
    { { -1, -5, 4, 6 } },
};
const struct sprite_frame_1box gSpriteBank39Frame007 = {
    SPRITE_FRAME(gSpriteBank39Frame007, SPRITE_TILES_BANK39_FRAME007),
    { { -1, -5, 4, 6 } },
};
const struct sprite_frame_1box gSpriteBank39Frame008 = {
    SPRITE_FRAME(gSpriteBank39Frame008, SPRITE_TILES_BANK39_FRAME008),
    { { -1, -5, 4, 6 } },
};
const struct sprite_frame gSpriteBank39Frame009 = SPRITE_FRAME(gSpriteBank39Frame009, SPRITE_TILES_BANK39_FRAME009);
const struct sprite_frame gSpriteBank39Frame010 = SPRITE_FRAME(gSpriteBank39Frame010, SPRITE_TILES_BANK39_FRAME010);
const struct sprite_frame gSpriteBank39Frame011 = SPRITE_FRAME(gSpriteBank39Frame011, SPRITE_TILES_BANK39_FRAME011);
const struct sprite_frame gSpriteBank39Frame012 = SPRITE_FRAME(gSpriteBank39Frame012, SPRITE_TILES_BANK39_FRAME012);

/* The piece arrays, from games/n2/graphics/sprites/bank39_rocket_and_portals/bank.json. */
#include "sprites/bank39_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 40: 5 animations, 5 frames, games/n2/graphics/sprites/bank40_boss_icons/. */

extern const u16 gSpriteBank40Anim00Seq[1];
extern const u16 gSpriteBank40Anim01Seq[1];
extern const u16 gSpriteBank40Anim02Seq[1];
extern const u16 gSpriteBank40Anim03Seq[1];
extern const u16 gSpriteBank40Anim04Seq[1];

extern const struct sprite_anim gSpriteBank40Anims[5] = {
    /* 0 */ {
        /* seq */ gSpriteBank40Anim00Seq,
        /* box */ { { -14, -10, 29, 20 }, { -14, -10, 29, 20 } },
        /* paletteId */ 57,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank40Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank40Anim01Seq,
        /* box */ { { -15, -9, 31, 18 }, { -15, -9, 31, 18 } },
        /* paletteId */ 57,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank40Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank40Anim02Seq,
        /* box */ { { -12, -11, 25, 23 }, { -12, -11, 25, 23 } },
        /* paletteId */ 57,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank40Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank40Anim03Seq,
        /* box */ { { -15, -10, 30, 21 }, { -15, -10, 30, 21 } },
        /* paletteId */ 57,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank40Anim03Seq),
        /* flags */ 0,
    },
    /* 4 */ {
        /* seq */ gSpriteBank40Anim04Seq,
        /* box */ { { -14, -11, 29, 22 }, { -14, -11, 29, 22 } },
        /* paletteId */ 57,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank40Anim04Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank40Anim00Seq[1] = {
    0,
};
const u16 gSpriteBank40Anim01Seq[1] = {
    1,
};
const u16 gSpriteBank40Anim02Seq[1] = {
    2,
};
const u16 gSpriteBank40Anim03Seq[1] = {
    3,
};
const u16 gSpriteBank40Anim04Seq[1] = {
    4,
};

extern const struct sprite_frame *const gSpriteBank40Frames[5] = {
    &gSpriteBank40Frame000,
    &gSpriteBank40Frame001,
    &gSpriteBank40Frame002,
    &gSpriteBank40Frame003,
    &gSpriteBank40Frame004,
};

const struct sprite_frame gSpriteBank40Frame000 = SPRITE_FRAME(gSpriteBank40Frame000, SPRITE_TILES_BANK40_FRAME000);
const struct sprite_frame gSpriteBank40Frame001 = SPRITE_FRAME(gSpriteBank40Frame001, SPRITE_TILES_BANK40_FRAME001);
const struct sprite_frame gSpriteBank40Frame002 = SPRITE_FRAME(gSpriteBank40Frame002, SPRITE_TILES_BANK40_FRAME002);
const struct sprite_frame gSpriteBank40Frame003 = SPRITE_FRAME(gSpriteBank40Frame003, SPRITE_TILES_BANK40_FRAME003);
const struct sprite_frame gSpriteBank40Frame004 = SPRITE_FRAME(gSpriteBank40Frame004, SPRITE_TILES_BANK40_FRAME004);

/* The piece arrays, from games/n2/graphics/sprites/bank40_boss_icons/bank.json. */
#include "sprites/bank40_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 41: 22 animations, 65 frames, games/n2/graphics/sprites/bank41_icons/. */

extern const u16 gSpriteBank41Anim00Seq[11];
extern const u16 gSpriteBank41Anim01Seq[12];
extern const u16 gSpriteBank41Anim02Seq[1];
extern const u16 gSpriteBank41Anim03Seq[1];
extern const u16 gSpriteBank41Anim04Seq[12];
extern const u16 gSpriteBank41Anim05Seq[1];
extern const u16 gSpriteBank41Anim06Seq[32];
extern const u16 gSpriteBank41Anim07Seq[6];
extern const u16 gSpriteBank41Anim08Seq[1];
extern const u16 gSpriteBank41Anim09Seq[1];
extern const u16 gSpriteBank41Anim10Seq[1];
extern const u16 gSpriteBank41Anim11Seq[1];
extern const u16 gSpriteBank41Anim12Seq[1];
extern const u16 gSpriteBank41Anim13Seq[1];
extern const u16 gSpriteBank41Anim14Seq[1];
extern const u16 gSpriteBank41Anim15Seq[1];
extern const u16 gSpriteBank41Anim16Seq[1];
extern const u16 gSpriteBank41Anim17Seq[1];
extern const u16 gSpriteBank41Anim18Seq[1];
extern const u16 gSpriteBank41Anim19Seq[1];
extern const u16 gSpriteBank41Anim20Seq[13];
extern const u16 gSpriteBank41Anim21Seq[1];

extern const struct sprite_anim gSpriteBank41Anims[22] = {
    /* 0 */ {
        /* seq */ gSpriteBank41Anim00Seq,
        /* box */ { { -6, -11, 13, 23 }, { -7, -11, 15, 23 } },
        /* paletteId */ 39,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank41Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank41Anim01Seq,
        /* box */ { { -3, -5, 7, 11 }, { -3, -6, 7, 12 } },
        /* paletteId */ 39,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank41Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank41Anim02Seq,
        /* box */ { { -12, -12, 25, 25 }, { -12, -12, 25, 25 } },
        /* paletteId */ 28,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank41Anim02Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 3 */ {
        /* seq */ gSpriteBank41Anim03Seq,
        /* box */ { { -3, -5, 7, 11 }, { -3, -5, 7, 11 } },
        /* paletteId */ 39,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank41Anim03Seq),
        /* flags */ 0,
    },
    /* 4 */ {
        /* seq */ gSpriteBank41Anim04Seq,
        /* box */ { { -3, -5, 7, 11 }, { -3, -6, 7, 12 } },
        /* paletteId */ 58,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank41Anim04Seq),
        /* flags */ 0,
    },
    /* 5 */ {
        /* seq */ gSpriteBank41Anim05Seq,
        /* box */ { { -13, -11, 27, 23 }, { -13, -11, 27, 23 } },
        /* paletteId */ 28,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank41Anim05Seq),
        /* flags */ 0,
    },
    /* 6 */ {
        /* seq */ gSpriteBank41Anim06Seq,
        /* box */ { { -27, -6, 55, 13 }, { -27, -6, 55, 13 } },
        /* paletteId */ 39,
        /* duration */ 11,
        /* frameCount */ ARRAY_COUNT(gSpriteBank41Anim06Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 7 */ {
        /* seq */ gSpriteBank41Anim07Seq,
        /* box */ { { -4, -4, 8, 9 }, { -4, -4, 40, 9 } },
        /* paletteId */ 59,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank41Anim07Seq),
        /* flags */ 0,
    },
    /* 8 */ {
        /* seq */ gSpriteBank41Anim08Seq,
        /* box */ { { -12, -10, 24, 21 }, { -13, -11, 26, 23 } },
        /* paletteId */ 60,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank41Anim08Seq),
        /* flags */ 0,
    },
    /* 9 */ {
        /* seq */ gSpriteBank41Anim09Seq,
        /* box */ { { -13, -11, 26, 23 }, { -14, -11, 28, 23 } },
        /* paletteId */ 61,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank41Anim09Seq),
        /* flags */ 0,
    },
    /* 10 */ {
        /* seq */ gSpriteBank41Anim10Seq,
        /* box */ { { -7, -7, 15, 15 }, { -13, -15, 27, 23 } },
        /* paletteId */ 62,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank41Anim10Seq),
        /* flags */ 0,
    },
    /* 11 */ {
        /* seq */ gSpriteBank41Anim11Seq,
        /* box */ { { -15, -11, 31, 23 }, { -15, -11, 31, 23 } },
        /* paletteId */ 63,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank41Anim11Seq),
        /* flags */ 0,
    },
    /* 12 */ {
        /* seq */ gSpriteBank41Anim12Seq,
        /* box */ { { -5, -11, 11, 23 }, { -5, -11, 11, 23 } },
        /* paletteId */ 64,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank41Anim12Seq),
        /* flags */ 0,
    },
    /* 13 */ {
        /* seq */ gSpriteBank41Anim13Seq,
        /* box */ { { -13, -11, 27, 23 }, { -14, -11, 29, 23 } },
        /* paletteId */ 50,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank41Anim13Seq),
        /* flags */ 0,
    },
    /* 14 */ {
        /* seq */ gSpriteBank41Anim14Seq,
        /* box */ { { -19, -11, 39, 22 }, { -19, -11, 39, 22 } },
        /* paletteId */ 65,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank41Anim14Seq),
        /* flags */ 0,
    },
    /* 15 */ {
        /* seq */ gSpriteBank41Anim15Seq,
        /* box */ { { -3, -31, 7, 63 }, { -3, -31, 7, 63 } },
        /* paletteId */ 57,
        /* duration */ 11,
        /* frameCount */ ARRAY_COUNT(gSpriteBank41Anim15Seq),
        /* flags */ 0,
    },
    /* 16 */ {
        /* seq */ gSpriteBank41Anim16Seq,
        /* box */ { { -7, -10, 15, 21 }, { -7, -10, 15, 21 } },
        /* paletteId */ 28,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank41Anim16Seq),
        /* flags */ 0,
    },
    /* 17 */ {
        /* seq */ gSpriteBank41Anim17Seq,
        /* box */ { { -7, -11, 15, 22 }, { -7, -11, 15, 22 } },
        /* paletteId */ 28,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank41Anim17Seq),
        /* flags */ 0,
    },
    /* 18 */ {
        /* seq */ gSpriteBank41Anim18Seq,
        /* box */ { { -111, -6, 223, 12 }, { -111, -6, 223, 12 } },
        /* paletteId */ 38,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank41Anim18Seq),
        /* flags */ 0,
    },
    /* 19 */ {
        /* seq */ gSpriteBank41Anim19Seq,
        /* box */ { { -8, -8, 16, 16 }, { -8, -8, 16, 16 } },
        /* paletteId */ 57,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank41Anim19Seq),
        /* flags */ 0,
    },
    /* 20 */ {
        /* seq */ gSpriteBank41Anim20Seq,
        /* box */ { { -3, -6, 7, 12 }, { -4, -6, 10, 13 } },
        /* paletteId */ 39,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank41Anim20Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 21 */ {
        /* seq */ gSpriteBank41Anim21Seq,
        /* box */ { { -15, -11, 30, 23 }, { -15, -11, 30, 23 } },
        /* paletteId */ 57,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank41Anim21Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank41Anim00Seq[11] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
};
const u16 gSpriteBank41Anim01Seq[12] = {
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
};
const u16 gSpriteBank41Anim02Seq[1] = {
    23,
};
const u16 gSpriteBank41Anim03Seq[1] = {
    24,
};
const u16 gSpriteBank41Anim04Seq[12] = {
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
};
const u16 gSpriteBank41Anim05Seq[1] = {
    25,
};
const u16 gSpriteBank41Anim06Seq[32] = {
    26, 26, 27, 27, 27, 27, 27, 27, 27, 28, 27, 29, 27, 30, 27, 31,
    27, 32, 27, 27, 28, 29, 30, 31, 32, 27, 26, 26, 27, 27, 27, 27,
};
const u16 gSpriteBank41Anim07Seq[6] = {
    33, 34, 35, 36, 37, 38,
};
const u16 gSpriteBank41Anim08Seq[1] = {
    39,
};
const u16 gSpriteBank41Anim09Seq[1] = {
    40,
};
const u16 gSpriteBank41Anim10Seq[1] = {
    41,
};
const u16 gSpriteBank41Anim11Seq[1] = {
    42,
};
const u16 gSpriteBank41Anim12Seq[1] = {
    43,
};
const u16 gSpriteBank41Anim13Seq[1] = {
    44,
};
const u16 gSpriteBank41Anim14Seq[1] = {
    45,
};
const u16 gSpriteBank41Anim15Seq[1] = {
    46,
};
const u16 gSpriteBank41Anim16Seq[1] = {
    47,
};
const u16 gSpriteBank41Anim17Seq[1] = {
    48,
};
const u16 gSpriteBank41Anim18Seq[1] = {
    49,
};
const u16 gSpriteBank41Anim19Seq[1] = {
    50,
};
const u16 gSpriteBank41Anim20Seq[13] = {
    51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
};
const u16 gSpriteBank41Anim21Seq[1] = {
    64,
};

extern const struct sprite_frame *const gSpriteBank41Frames[65] = {
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
    &gSpriteBank41Frame024,
    &gSpriteBank41Frame025.frame,
    &gSpriteBank41Frame026.frame,
    &gSpriteBank41Frame027,
    &gSpriteBank41Frame028.frame,
    &gSpriteBank41Frame029.frame,
    &gSpriteBank41Frame030.frame,
    &gSpriteBank41Frame031.frame,
    &gSpriteBank41Frame032.frame,
    &gSpriteBank41Frame033,
    &gSpriteBank41Frame034,
    &gSpriteBank41Frame035,
    &gSpriteBank41Frame036,
    &gSpriteBank41Frame037,
    &gSpriteBank41Frame038,
    &gSpriteBank41Frame039,
    &gSpriteBank41Frame040,
    &gSpriteBank41Frame041,
    &gSpriteBank41Frame042,
    &gSpriteBank41Frame043,
    &gSpriteBank41Frame044,
    &gSpriteBank41Frame045,
    &gSpriteBank41Frame046,
    &gSpriteBank41Frame047,
    &gSpriteBank41Frame048,
    &gSpriteBank41Frame049,
    &gSpriteBank41Frame050,
    &gSpriteBank41Frame051,
    &gSpriteBank41Frame052,
    &gSpriteBank41Frame053,
    &gSpriteBank41Frame054,
    &gSpriteBank41Frame055,
    &gSpriteBank41Frame056,
    &gSpriteBank41Frame057,
    &gSpriteBank41Frame058,
    &gSpriteBank41Frame059,
    &gSpriteBank41Frame060,
    &gSpriteBank41Frame061,
    &gSpriteBank41Frame062,
    &gSpriteBank41Frame063,
    &gSpriteBank41Frame064,
};

const struct sprite_frame_1box gSpriteBank41Frame000 = {
    SPRITE_FRAME(gSpriteBank41Frame000, SPRITE_TILES_BANK41_FRAME000),
    { { -6, -11, 13, 23 } },
};
const struct sprite_frame_1box gSpriteBank41Frame001 = {
    SPRITE_FRAME(gSpriteBank41Frame001, SPRITE_TILES_BANK41_FRAME001),
    { { -4, -11, 9, 23 } },
};
const struct sprite_frame_1box gSpriteBank41Frame002 = {
    SPRITE_FRAME(gSpriteBank41Frame002, SPRITE_TILES_BANK41_FRAME002),
    { { -7, -11, 15, 23 } },
};
const struct sprite_frame_1box gSpriteBank41Frame003 = {
    SPRITE_FRAME(gSpriteBank41Frame003, SPRITE_TILES_BANK41_FRAME003),
    { { -6, -11, 14, 23 } },
};
const struct sprite_frame_1box gSpriteBank41Frame004 = {
    SPRITE_FRAME(gSpriteBank41Frame004, SPRITE_TILES_BANK41_FRAME004),
    { { -6, -11, 14, 23 } },
};
const struct sprite_frame_1box gSpriteBank41Frame005 = {
    SPRITE_FRAME(gSpriteBank41Frame005, SPRITE_TILES_BANK41_FRAME005),
    { { -6, -11, 14, 23 } },
};
const struct sprite_frame_1box gSpriteBank41Frame006 = {
    SPRITE_FRAME(gSpriteBank41Frame006, SPRITE_TILES_BANK41_FRAME006),
    { { -7, -11, 14, 23 } },
};
const struct sprite_frame_1box gSpriteBank41Frame007 = {
    SPRITE_FRAME(gSpriteBank41Frame007, SPRITE_TILES_BANK41_FRAME007),
    { { -6, -10, 14, 22 } },
};
const struct sprite_frame_1box gSpriteBank41Frame008 = {
    SPRITE_FRAME(gSpriteBank41Frame008, SPRITE_TILES_BANK41_FRAME008),
    { { -6, -11, 13, 23 } },
};
const struct sprite_frame_1box gSpriteBank41Frame009 = {
    SPRITE_FRAME(gSpriteBank41Frame009, SPRITE_TILES_BANK41_FRAME009),
    { { -7, -11, 15, 23 } },
};
const struct sprite_frame_1box gSpriteBank41Frame010 = {
    SPRITE_FRAME(gSpriteBank41Frame010, SPRITE_TILES_BANK41_FRAME010),
    { { -7, -11, 15, 23 } },
};
const struct sprite_frame_1box gSpriteBank41Frame011 = {
    SPRITE_FRAME(gSpriteBank41Frame011, SPRITE_TILES_BANK41_FRAME011),
    { { -3, -5, 7, 11 } },
};
const struct sprite_frame_1box gSpriteBank41Frame012 = {
    SPRITE_FRAME(gSpriteBank41Frame012, SPRITE_TILES_BANK41_FRAME012),
    { { -2, -5, 5, 11 } },
};
const struct sprite_frame_1box gSpriteBank41Frame013 = {
    SPRITE_FRAME(gSpriteBank41Frame013, SPRITE_TILES_BANK41_FRAME013),
    { { -3, -6, 7, 12 } },
};
const struct sprite_frame_1box gSpriteBank41Frame014 = {
    SPRITE_FRAME(gSpriteBank41Frame014, SPRITE_TILES_BANK41_FRAME014),
    { { -3, -6, 7, 12 } },
};
const struct sprite_frame_1box gSpriteBank41Frame015 = {
    SPRITE_FRAME(gSpriteBank41Frame015, SPRITE_TILES_BANK41_FRAME015),
    { { -3, -6, 7, 12 } },
};
const struct sprite_frame_1box gSpriteBank41Frame016 = {
    SPRITE_FRAME(gSpriteBank41Frame016, SPRITE_TILES_BANK41_FRAME016),
    { { -3, -6, 7, 12 } },
};
const struct sprite_frame_1box gSpriteBank41Frame017 = {
    SPRITE_FRAME(gSpriteBank41Frame017, SPRITE_TILES_BANK41_FRAME017),
    { { -3, -5, 7, 11 } },
};
const struct sprite_frame_1box gSpriteBank41Frame018 = {
    SPRITE_FRAME(gSpriteBank41Frame018, SPRITE_TILES_BANK41_FRAME018),
    { { -3, -5, 7, 11 } },
};
const struct sprite_frame_1box gSpriteBank41Frame019 = {
    SPRITE_FRAME(gSpriteBank41Frame019, SPRITE_TILES_BANK41_FRAME019),
    { { -3, -6, 7, 12 } },
};
const struct sprite_frame_1box gSpriteBank41Frame020 = {
    SPRITE_FRAME(gSpriteBank41Frame020, SPRITE_TILES_BANK41_FRAME020),
    { { -3, -6, 7, 12 } },
};
const struct sprite_frame_1box gSpriteBank41Frame021 = {
    SPRITE_FRAME(gSpriteBank41Frame021, SPRITE_TILES_BANK41_FRAME021),
    { { -1, -4, 3, 10 } },
};
const struct sprite_frame_1box gSpriteBank41Frame022 = {
    SPRITE_FRAME(gSpriteBank41Frame022, SPRITE_TILES_BANK41_FRAME022),
    { { -1, 2, 3, 4 } },
};
const struct sprite_frame_1box gSpriteBank41Frame023 = {
    SPRITE_FRAME(gSpriteBank41Frame023, SPRITE_TILES_BANK41_FRAME023),
    { { -12, -12, 25, 25 } },
};
const struct sprite_frame gSpriteBank41Frame024 = SPRITE_FRAME(gSpriteBank41Frame024, SPRITE_TILES_BANK41_FRAME024);
const struct sprite_frame_1box gSpriteBank41Frame025 = {
    SPRITE_FRAME(gSpriteBank41Frame025, SPRITE_TILES_BANK41_FRAME025),
    { { -13, -11, 27, 23 } },
};
const struct sprite_frame_1box gSpriteBank41Frame026 = {
    SPRITE_FRAME(gSpriteBank41Frame026, SPRITE_TILES_BANK41_FRAME026),
    { { -27, -6, 55, 13 } },
};
const struct sprite_frame gSpriteBank41Frame027 = { gSpriteBank41Frame028Pos, gSpriteBank41Frame028Pieces, SPRITE_FRAME_TILES(SPRITE_TILES_BANK41_FRAME027, 0) };
const struct sprite_frame_1box gSpriteBank41Frame028 = {
    SPRITE_FRAME(gSpriteBank41Frame028, SPRITE_TILES_BANK41_FRAME028),
    { { -27, -6, 11, 12 } },
};
const struct sprite_frame_1box gSpriteBank41Frame029 = {
    SPRITE_FRAME(gSpriteBank41Frame029, SPRITE_TILES_BANK41_FRAME029),
    { { -14, -6, 9, 13 } },
};
const struct sprite_frame_1box gSpriteBank41Frame030 = {
    SPRITE_FRAME(gSpriteBank41Frame030, SPRITE_TILES_BANK41_FRAME030),
    { { -3, -6, 9, 13 } },
};
const struct sprite_frame_1box gSpriteBank41Frame031 = {
    SPRITE_FRAME(gSpriteBank41Frame031, SPRITE_TILES_BANK41_FRAME031),
    { { 8, -6, 9, 13 } },
};
const struct sprite_frame_1box gSpriteBank41Frame032 = {
    SPRITE_FRAME(gSpriteBank41Frame032, SPRITE_TILES_BANK41_FRAME032),
    { { 19, -6, 9, 13 } },
};
const struct sprite_frame gSpriteBank41Frame033 = SPRITE_FRAME(gSpriteBank41Frame033, SPRITE_TILES_BANK41_FRAME033);
const struct sprite_frame gSpriteBank41Frame034 = SPRITE_FRAME(gSpriteBank41Frame034, SPRITE_TILES_BANK41_FRAME034);
const struct sprite_frame gSpriteBank41Frame035 = SPRITE_FRAME(gSpriteBank41Frame035, SPRITE_TILES_BANK41_FRAME035);
const struct sprite_frame gSpriteBank41Frame036 = SPRITE_FRAME(gSpriteBank41Frame036, SPRITE_TILES_BANK41_FRAME036);
const struct sprite_frame gSpriteBank41Frame037 = SPRITE_FRAME(gSpriteBank41Frame037, SPRITE_TILES_BANK41_FRAME037);
const struct sprite_frame gSpriteBank41Frame038 = SPRITE_FRAME(gSpriteBank41Frame038, SPRITE_TILES_BANK41_FRAME038);
const struct sprite_frame gSpriteBank41Frame039 = SPRITE_FRAME(gSpriteBank41Frame039, SPRITE_TILES_BANK41_FRAME039);
const struct sprite_frame gSpriteBank41Frame040 = SPRITE_FRAME(gSpriteBank41Frame040, SPRITE_TILES_BANK41_FRAME040);
const struct sprite_frame gSpriteBank41Frame041 = SPRITE_FRAME(gSpriteBank41Frame041, SPRITE_TILES_BANK41_FRAME041);
const struct sprite_frame gSpriteBank41Frame042 = SPRITE_FRAME(gSpriteBank41Frame042, SPRITE_TILES_BANK41_FRAME042);
const struct sprite_frame gSpriteBank41Frame043 = SPRITE_FRAME(gSpriteBank41Frame043, SPRITE_TILES_BANK41_FRAME043);
const struct sprite_frame gSpriteBank41Frame044 = SPRITE_FRAME(gSpriteBank41Frame044, SPRITE_TILES_BANK41_FRAME044);
const struct sprite_frame gSpriteBank41Frame045 = SPRITE_FRAME(gSpriteBank41Frame045, SPRITE_TILES_BANK41_FRAME045);
const struct sprite_frame gSpriteBank41Frame046 = SPRITE_FRAME(gSpriteBank41Frame046, SPRITE_TILES_BANK41_FRAME046);
const struct sprite_frame gSpriteBank41Frame047 = SPRITE_FRAME(gSpriteBank41Frame047, SPRITE_TILES_BANK41_FRAME047);
const struct sprite_frame gSpriteBank41Frame048 = SPRITE_FRAME(gSpriteBank41Frame048, SPRITE_TILES_BANK41_FRAME048);
const struct sprite_frame gSpriteBank41Frame049 = SPRITE_FRAME(gSpriteBank41Frame049, SPRITE_TILES_BANK41_FRAME049);
const struct sprite_frame gSpriteBank41Frame050 = SPRITE_FRAME(gSpriteBank41Frame050, SPRITE_TILES_BANK41_FRAME050);
const struct sprite_frame gSpriteBank41Frame051 = SPRITE_FRAME(gSpriteBank41Frame051, SPRITE_TILES_BANK41_FRAME051);
const struct sprite_frame gSpriteBank41Frame052 = SPRITE_FRAME(gSpriteBank41Frame052, SPRITE_TILES_BANK41_FRAME052);
const struct sprite_frame gSpriteBank41Frame053 = SPRITE_FRAME(gSpriteBank41Frame053, SPRITE_TILES_BANK41_FRAME053);
const struct sprite_frame gSpriteBank41Frame054 = SPRITE_FRAME(gSpriteBank41Frame054, SPRITE_TILES_BANK41_FRAME054);
const struct sprite_frame gSpriteBank41Frame055 = SPRITE_FRAME(gSpriteBank41Frame055, SPRITE_TILES_BANK41_FRAME055);
const struct sprite_frame gSpriteBank41Frame056 = SPRITE_FRAME(gSpriteBank41Frame056, SPRITE_TILES_BANK41_FRAME056);
const struct sprite_frame gSpriteBank41Frame057 = SPRITE_FRAME(gSpriteBank41Frame057, SPRITE_TILES_BANK41_FRAME057);
const struct sprite_frame gSpriteBank41Frame058 = SPRITE_FRAME(gSpriteBank41Frame058, SPRITE_TILES_BANK41_FRAME058);
const struct sprite_frame gSpriteBank41Frame059 = SPRITE_FRAME(gSpriteBank41Frame059, SPRITE_TILES_BANK41_FRAME059);
const struct sprite_frame gSpriteBank41Frame060 = SPRITE_FRAME(gSpriteBank41Frame060, SPRITE_TILES_BANK41_FRAME060);
const struct sprite_frame gSpriteBank41Frame061 = SPRITE_FRAME(gSpriteBank41Frame061, SPRITE_TILES_BANK41_FRAME061);
const struct sprite_frame gSpriteBank41Frame062 = SPRITE_FRAME(gSpriteBank41Frame062, SPRITE_TILES_BANK41_FRAME062);
const struct sprite_frame gSpriteBank41Frame063 = SPRITE_FRAME(gSpriteBank41Frame063, SPRITE_TILES_BANK41_FRAME063);
const struct sprite_frame gSpriteBank41Frame064 = SPRITE_FRAME(gSpriteBank41Frame064, SPRITE_TILES_BANK41_FRAME064);

/* The piece arrays, from games/n2/graphics/sprites/bank41_icons/bank.json. */
#include "sprites/bank41_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 42: 10 animations, 135 frames, games/n2/graphics/sprites/bank42_evil_crunch/. */

extern const u16 gSpriteBank42Anim00Seq[12];
extern const u16 gSpriteBank42Anim01Seq[11];
extern const u16 gSpriteBank42Anim02Seq[13];
extern const u16 gSpriteBank42Anim03Seq[15];
extern const u16 gSpriteBank42Anim04Seq[12];
extern const u16 gSpriteBank42Anim05Seq[17];
extern const u16 gSpriteBank42Anim06Seq[27];
extern const u16 gSpriteBank42Anim07Seq[7];
extern const u16 gSpriteBank42Anim08Seq[18];
extern const u16 gSpriteBank42Anim09Seq[3];

extern const struct sprite_anim gSpriteBank42Anims[10] = {
    /* 0 */ {
        /* seq */ gSpriteBank42Anim00Seq,
        /* box */ { { -8, -23, 35, 52 }, { -52, -27, 82, 65 } },
        /* paletteId */ 72,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank42Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank42Anim01Seq,
        /* box */ { { -4, -24, 29, 48 }, { -20, -20, 48, 54 } },
        /* paletteId */ 72,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank42Anim01Seq),
        /* flags */ 0,
    },
    /* 2 */ {
        /* seq */ gSpriteBank42Anim02Seq,
        /* box */ { { -4, -24, 29, 48 }, { -30, -34, 60, 68 } },
        /* paletteId */ 72,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank42Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank42Anim03Seq,
        /* box */ { { -4, -24, 29, 48 }, { -29, -47, 61, 83 } },
        /* paletteId */ 72,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank42Anim03Seq),
        /* flags */ 0,
    },
    /* 4 */ {
        /* seq */ gSpriteBank42Anim04Seq,
        /* box */ { { -4, -24, 29, 48 }, { -30, -30, 60, 62 } },
        /* paletteId */ 72,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank42Anim04Seq),
        /* flags */ 0,
    },
    /* 5 */ {
        /* seq */ gSpriteBank42Anim05Seq,
        /* box */ { { -4, -24, 29, 48 }, { -29, -47, 61, 83 } },
        /* paletteId */ 72,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank42Anim05Seq),
        /* flags */ 0,
    },
    /* 6 */ {
        /* seq */ gSpriteBank42Anim06Seq,
        /* box */ { { -4, -24, 29, 48 }, { -30, -51, 61, 83 } },
        /* paletteId */ 72,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank42Anim06Seq),
        /* flags */ 0,
    },
    /* 7 */ {
        /* seq */ gSpriteBank42Anim07Seq,
        /* box */ { { -4, -24, 29, 48 }, { -17, -44, 49, 57 } },
        /* paletteId */ 72,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank42Anim07Seq),
        /* flags */ 0,
    },
    /* 8 */ {
        /* seq */ gSpriteBank42Anim08Seq,
        /* box */ { { -4, -24, 29, 48 }, { -29, -47, 68, 83 } },
        /* paletteId */ 72,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank42Anim08Seq),
        /* flags */ 0,
    },
    /* 9 */ {
        /* seq */ gSpriteBank42Anim09Seq,
        /* box */ { { -7, -2, 14, 4 }, { -7, -2, 14, 4 } },
        /* paletteId */ 8,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank42Anim09Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank42Anim00Seq[12] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
};
const u16 gSpriteBank42Anim01Seq[11] = {
    12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
};
const u16 gSpriteBank42Anim02Seq[13] = {
    23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
};
const u16 gSpriteBank42Anim03Seq[15] = {
    36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
};
const u16 gSpriteBank42Anim04Seq[12] = {
    51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
};
const u16 gSpriteBank42Anim05Seq[17] = {
    63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78,
    79,
};
const u16 gSpriteBank42Anim06Seq[27] = {
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
    96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106,
};
const u16 gSpriteBank42Anim07Seq[7] = {
    107, 108, 109, 110, 111, 112, 113,
};
const u16 gSpriteBank42Anim08Seq[18] = {
    114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129,
    130, 131,
};
const u16 gSpriteBank42Anim09Seq[3] = {
    132, 133, 134,
};

extern const struct sprite_frame *const gSpriteBank42Frames[135] = {
    &gSpriteBank42Frame000.frame,
    &gSpriteBank42Frame001.frame,
    &gSpriteBank42Frame002.frame,
    &gSpriteBank42Frame003.frame,
    &gSpriteBank42Frame004.frame,
    &gSpriteBank42Frame005.frame,
    &gSpriteBank42Frame006.frame,
    &gSpriteBank42Frame007.frame,
    &gSpriteBank42Frame008.frame,
    &gSpriteBank42Frame009.frame,
    &gSpriteBank42Frame010.frame,
    &gSpriteBank42Frame011.frame,
    &gSpriteBank42Frame012.frame,
    &gSpriteBank42Frame013.frame,
    &gSpriteBank42Frame014.frame,
    &gSpriteBank42Frame015.frame,
    &gSpriteBank42Frame016.frame,
    &gSpriteBank42Frame017.frame,
    &gSpriteBank42Frame018.frame,
    &gSpriteBank42Frame019.frame,
    &gSpriteBank42Frame020.frame,
    &gSpriteBank42Frame021.frame,
    &gSpriteBank42Frame022.frame,
    &gSpriteBank42Frame023.frame,
    &gSpriteBank42Frame024.frame,
    &gSpriteBank42Frame025.frame,
    &gSpriteBank42Frame026.frame,
    &gSpriteBank42Frame027.frame,
    &gSpriteBank42Frame028.frame,
    &gSpriteBank42Frame029.frame,
    &gSpriteBank42Frame030.frame,
    &gSpriteBank42Frame031.frame,
    &gSpriteBank42Frame032.frame,
    &gSpriteBank42Frame033.frame,
    &gSpriteBank42Frame034.frame,
    &gSpriteBank42Frame035.frame,
    &gSpriteBank42Frame036.frame,
    &gSpriteBank42Frame037.frame,
    &gSpriteBank42Frame038.frame,
    &gSpriteBank42Frame039.frame,
    &gSpriteBank42Frame040.frame,
    &gSpriteBank42Frame041.frame,
    &gSpriteBank42Frame042.frame,
    &gSpriteBank42Frame043.frame,
    &gSpriteBank42Frame044.frame,
    &gSpriteBank42Frame045.frame,
    &gSpriteBank42Frame046.frame,
    &gSpriteBank42Frame047.frame,
    &gSpriteBank42Frame048.frame,
    &gSpriteBank42Frame049.frame,
    &gSpriteBank42Frame050.frame,
    &gSpriteBank42Frame051.frame,
    &gSpriteBank42Frame052.frame,
    &gSpriteBank42Frame053.frame,
    &gSpriteBank42Frame054.frame,
    &gSpriteBank42Frame055.frame,
    &gSpriteBank42Frame056.frame,
    &gSpriteBank42Frame057.frame,
    &gSpriteBank42Frame058.frame,
    &gSpriteBank42Frame059.frame,
    &gSpriteBank42Frame060.frame,
    &gSpriteBank42Frame061.frame,
    &gSpriteBank42Frame062.frame,
    &gSpriteBank42Frame063.frame,
    &gSpriteBank42Frame064.frame,
    &gSpriteBank42Frame065.frame,
    &gSpriteBank42Frame066.frame,
    &gSpriteBank42Frame067.frame,
    &gSpriteBank42Frame068.frame,
    &gSpriteBank42Frame069.frame,
    &gSpriteBank42Frame070.frame,
    &gSpriteBank42Frame071.frame,
    &gSpriteBank42Frame072.frame,
    &gSpriteBank42Frame073.frame,
    &gSpriteBank42Frame074.frame,
    &gSpriteBank42Frame075.frame,
    &gSpriteBank42Frame076.frame,
    &gSpriteBank42Frame077.frame,
    &gSpriteBank42Frame078.frame,
    &gSpriteBank42Frame079.frame,
    &gSpriteBank42Frame080.frame,
    &gSpriteBank42Frame081.frame,
    &gSpriteBank42Frame082.frame,
    &gSpriteBank42Frame083.frame,
    &gSpriteBank42Frame084.frame,
    &gSpriteBank42Frame085.frame,
    &gSpriteBank42Frame086.frame,
    &gSpriteBank42Frame087.frame,
    &gSpriteBank42Frame088.frame,
    &gSpriteBank42Frame089.frame,
    &gSpriteBank42Frame090.frame,
    &gSpriteBank42Frame091.frame,
    &gSpriteBank42Frame092.frame,
    &gSpriteBank42Frame093.frame,
    &gSpriteBank42Frame094.frame,
    &gSpriteBank42Frame095.frame,
    &gSpriteBank42Frame096.frame,
    &gSpriteBank42Frame097.frame,
    &gSpriteBank42Frame098.frame,
    &gSpriteBank42Frame099.frame,
    &gSpriteBank42Frame100.frame,
    &gSpriteBank42Frame101.frame,
    &gSpriteBank42Frame102.frame,
    &gSpriteBank42Frame103.frame,
    &gSpriteBank42Frame104.frame,
    &gSpriteBank42Frame105.frame,
    &gSpriteBank42Frame106.frame,
    &gSpriteBank42Frame107,
    &gSpriteBank42Frame108,
    &gSpriteBank42Frame109,
    &gSpriteBank42Frame110,
    &gSpriteBank42Frame111,
    &gSpriteBank42Frame112,
    &gSpriteBank42Frame113,
    &gSpriteBank42Frame114,
    &gSpriteBank42Frame115,
    &gSpriteBank42Frame116,
    &gSpriteBank42Frame117,
    &gSpriteBank42Frame118,
    &gSpriteBank42Frame119,
    &gSpriteBank42Frame120,
    &gSpriteBank42Frame121,
    &gSpriteBank42Frame122,
    &gSpriteBank42Frame123,
    &gSpriteBank42Frame124,
    &gSpriteBank42Frame125,
    &gSpriteBank42Frame126,
    &gSpriteBank42Frame127,
    &gSpriteBank42Frame128,
    &gSpriteBank42Frame129,
    &gSpriteBank42Frame130,
    &gSpriteBank42Frame131,
    &gSpriteBank42Frame132.frame,
    &gSpriteBank42Frame133.frame,
    &gSpriteBank42Frame134.frame,
};

const struct sprite_frame_1box gSpriteBank42Frame000 = {
    SPRITE_FRAME(gSpriteBank42Frame000, SPRITE_TILES_BANK42_FRAME000),
    { { -2, -24, 28, 56 } },
};
const struct sprite_frame_1box gSpriteBank42Frame001 = {
    SPRITE_FRAME(gSpriteBank42Frame001, SPRITE_TILES_BANK42_FRAME001),
    { { -2, -24, 28, 56 } },
};
const struct sprite_frame_1box gSpriteBank42Frame002 = {
    SPRITE_FRAME(gSpriteBank42Frame002, SPRITE_TILES_BANK42_FRAME002),
    { { -2, -24, 28, 56 } },
};
const struct sprite_frame_1box gSpriteBank42Frame003 = {
    SPRITE_FRAME(gSpriteBank42Frame003, SPRITE_TILES_BANK42_FRAME003),
    { { -2, -24, 28, 56 } },
};
const struct sprite_frame_1box gSpriteBank42Frame004 = {
    SPRITE_FRAME(gSpriteBank42Frame004, SPRITE_TILES_BANK42_FRAME004),
    { { -2, -24, 28, 56 } },
};
const struct sprite_frame_1box gSpriteBank42Frame005 = {
    SPRITE_FRAME(gSpriteBank42Frame005, SPRITE_TILES_BANK42_FRAME005),
    { { -2, -24, 28, 56 } },
};
const struct sprite_frame_1box gSpriteBank42Frame006 = {
    SPRITE_FRAME(gSpriteBank42Frame006, SPRITE_TILES_BANK42_FRAME006),
    { { -2, -24, 28, 56 } },
};
const struct sprite_frame_1box gSpriteBank42Frame007 = {
    SPRITE_FRAME(gSpriteBank42Frame007, SPRITE_TILES_BANK42_FRAME007),
    { { -2, -24, 28, 56 } },
};
const struct sprite_frame_1box gSpriteBank42Frame008 = {
    SPRITE_FRAME(gSpriteBank42Frame008, SPRITE_TILES_BANK42_FRAME008),
    { { -2, -24, 28, 56 } },
};
const struct sprite_frame_1box gSpriteBank42Frame009 = {
    SPRITE_FRAME(gSpriteBank42Frame009, SPRITE_TILES_BANK42_FRAME009),
    { { -2, -24, 28, 56 } },
};
const struct sprite_frame_1box gSpriteBank42Frame010 = {
    SPRITE_FRAME(gSpriteBank42Frame010, SPRITE_TILES_BANK42_FRAME010),
    { { -2, -24, 28, 56 } },
};
const struct sprite_frame_1box gSpriteBank42Frame011 = {
    SPRITE_FRAME(gSpriteBank42Frame011, SPRITE_TILES_BANK42_FRAME011),
    { { -2, -24, 28, 56 } },
};
const struct sprite_frame_2box gSpriteBank42Frame012 = {
    SPRITE_FRAME(gSpriteBank42Frame012, SPRITE_TILES_BANK42_FRAME012),
    { { -9, -12, 34, 41 }, { -8, -11, 31, 41 } },
};
const struct sprite_frame_1box gSpriteBank42Frame013 = {
    SPRITE_FRAME(gSpriteBank42Frame013, SPRITE_TILES_BANK42_FRAME013),
    { { -8, -11, 31, 41 } },
};
const struct sprite_frame_1box gSpriteBank42Frame014 = {
    SPRITE_FRAME(gSpriteBank42Frame014, SPRITE_TILES_BANK42_FRAME014),
    { { -8, -11, 31, 41 } },
};
const struct sprite_frame_1box gSpriteBank42Frame015 = {
    SPRITE_FRAME(gSpriteBank42Frame015, SPRITE_TILES_BANK42_FRAME015),
    { { -8, -11, 31, 41 } },
};
const struct sprite_frame_1box gSpriteBank42Frame016 = {
    SPRITE_FRAME(gSpriteBank42Frame016, SPRITE_TILES_BANK42_FRAME016),
    { { -8, -11, 31, 41 } },
};
const struct sprite_frame_1box gSpriteBank42Frame017 = {
    SPRITE_FRAME(gSpriteBank42Frame017, SPRITE_TILES_BANK42_FRAME017),
    { { -8, -11, 31, 41 } },
};
const struct sprite_frame_1box gSpriteBank42Frame018 = {
    SPRITE_FRAME(gSpriteBank42Frame018, SPRITE_TILES_BANK42_FRAME018),
    { { -8, -11, 31, 41 } },
};
const struct sprite_frame_1box gSpriteBank42Frame019 = {
    SPRITE_FRAME(gSpriteBank42Frame019, SPRITE_TILES_BANK42_FRAME019),
    { { -8, -11, 31, 41 } },
};
const struct sprite_frame_1box gSpriteBank42Frame020 = {
    SPRITE_FRAME(gSpriteBank42Frame020, SPRITE_TILES_BANK42_FRAME020),
    { { -8, -11, 31, 41 } },
};
const struct sprite_frame_1box gSpriteBank42Frame021 = {
    SPRITE_FRAME(gSpriteBank42Frame021, SPRITE_TILES_BANK42_FRAME021),
    { { -8, -11, 31, 41 } },
};
const struct sprite_frame_1box gSpriteBank42Frame022 = {
    SPRITE_FRAME(gSpriteBank42Frame022, SPRITE_TILES_BANK42_FRAME022),
    { { -8, -11, 31, 41 } },
};
const struct sprite_frame_1box gSpriteBank42Frame023 = {
    SPRITE_FRAME(gSpriteBank42Frame023, SPRITE_TILES_BANK42_FRAME023),
    { { -10, -13, 35, 43 } },
};
const struct sprite_frame_1box gSpriteBank42Frame024 = {
    SPRITE_FRAME(gSpriteBank42Frame024, SPRITE_TILES_BANK42_FRAME024),
    { { -10, -13, 35, 43 } },
};
const struct sprite_frame_1box gSpriteBank42Frame025 = {
    SPRITE_FRAME(gSpriteBank42Frame025, SPRITE_TILES_BANK42_FRAME025),
    { { -10, -13, 35, 43 } },
};
const struct sprite_frame_1box gSpriteBank42Frame026 = {
    SPRITE_FRAME(gSpriteBank42Frame026, SPRITE_TILES_BANK42_FRAME026),
    { { -10, -13, 35, 43 } },
};
const struct sprite_frame_1box gSpriteBank42Frame027 = {
    SPRITE_FRAME(gSpriteBank42Frame027, SPRITE_TILES_BANK42_FRAME027),
    { { -10, -13, 35, 43 } },
};
const struct sprite_frame_1box gSpriteBank42Frame028 = {
    SPRITE_FRAME(gSpriteBank42Frame028, SPRITE_TILES_BANK42_FRAME028),
    { { -10, -13, 35, 43 } },
};
const struct sprite_frame_1box gSpriteBank42Frame029 = {
    SPRITE_FRAME(gSpriteBank42Frame029, SPRITE_TILES_BANK42_FRAME029),
    { { -10, -13, 35, 43 } },
};
const struct sprite_frame_1box gSpriteBank42Frame030 = {
    SPRITE_FRAME(gSpriteBank42Frame030, SPRITE_TILES_BANK42_FRAME030),
    { { -10, -13, 35, 43 } },
};
const struct sprite_frame_1box gSpriteBank42Frame031 = {
    SPRITE_FRAME(gSpriteBank42Frame031, SPRITE_TILES_BANK42_FRAME031),
    { { -10, -13, 35, 43 } },
};
const struct sprite_frame_1box gSpriteBank42Frame032 = {
    SPRITE_FRAME(gSpriteBank42Frame032, SPRITE_TILES_BANK42_FRAME032),
    { { -10, -13, 35, 43 } },
};
const struct sprite_frame_1box gSpriteBank42Frame033 = {
    SPRITE_FRAME(gSpriteBank42Frame033, SPRITE_TILES_BANK42_FRAME033),
    { { -10, -13, 35, 43 } },
};
const struct sprite_frame_1box gSpriteBank42Frame034 = {
    SPRITE_FRAME(gSpriteBank42Frame034, SPRITE_TILES_BANK42_FRAME034),
    { { -10, -13, 35, 43 } },
};
const struct sprite_frame_1box gSpriteBank42Frame035 = {
    SPRITE_FRAME(gSpriteBank42Frame035, SPRITE_TILES_BANK42_FRAME035),
    { { -10, -13, 35, 43 } },
};
const struct sprite_frame_1box gSpriteBank42Frame036 = {
    SPRITE_FRAME(gSpriteBank42Frame036, SPRITE_TILES_BANK42_FRAME036),
    { { -2, -21, 28, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame037 = {
    SPRITE_FRAME(gSpriteBank42Frame037, SPRITE_TILES_BANK42_FRAME037),
    { { -2, -21, 28, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame038 = {
    SPRITE_FRAME(gSpriteBank42Frame038, SPRITE_TILES_BANK42_FRAME038),
    { { -2, -21, 28, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame039 = {
    SPRITE_FRAME(gSpriteBank42Frame039, SPRITE_TILES_BANK42_FRAME039),
    { { -2, -21, 28, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame040 = {
    SPRITE_FRAME(gSpriteBank42Frame040, SPRITE_TILES_BANK42_FRAME040),
    { { -2, -21, 28, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame041 = {
    SPRITE_FRAME(gSpriteBank42Frame041, SPRITE_TILES_BANK42_FRAME041),
    { { -2, -21, 28, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame042 = {
    SPRITE_FRAME(gSpriteBank42Frame042, SPRITE_TILES_BANK42_FRAME042),
    { { -2, -21, 28, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame043 = {
    SPRITE_FRAME(gSpriteBank42Frame043, SPRITE_TILES_BANK42_FRAME043),
    { { -2, -21, 28, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame044 = {
    SPRITE_FRAME(gSpriteBank42Frame044, SPRITE_TILES_BANK42_FRAME044),
    { { -2, -21, 28, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame045 = {
    SPRITE_FRAME(gSpriteBank42Frame045, SPRITE_TILES_BANK42_FRAME045),
    { { -2, -21, 28, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame046 = {
    SPRITE_FRAME(gSpriteBank42Frame046, SPRITE_TILES_BANK42_FRAME046),
    { { -2, -21, 28, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame047 = {
    SPRITE_FRAME(gSpriteBank42Frame047, SPRITE_TILES_BANK42_FRAME047),
    { { -2, -21, 28, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame048 = {
    SPRITE_FRAME(gSpriteBank42Frame048, SPRITE_TILES_BANK42_FRAME048),
    { { -2, -21, 28, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame049 = {
    SPRITE_FRAME(gSpriteBank42Frame049, SPRITE_TILES_BANK42_FRAME049),
    { { -2, -21, 28, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame050 = {
    SPRITE_FRAME(gSpriteBank42Frame050, SPRITE_TILES_BANK42_FRAME050),
    { { -2, -21, 28, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame051 = {
    SPRITE_FRAME(gSpriteBank42Frame051, SPRITE_TILES_BANK42_FRAME051),
    { { -5, -21, 30, 45 } },
};
const struct sprite_frame_1box gSpriteBank42Frame052 = {
    SPRITE_FRAME(gSpriteBank42Frame052, SPRITE_TILES_BANK42_FRAME052),
    { { -5, -21, 30, 45 } },
};
const struct sprite_frame_1box gSpriteBank42Frame053 = {
    SPRITE_FRAME(gSpriteBank42Frame053, SPRITE_TILES_BANK42_FRAME053),
    { { -5, -21, 30, 45 } },
};
const struct sprite_frame_1box gSpriteBank42Frame054 = {
    SPRITE_FRAME(gSpriteBank42Frame054, SPRITE_TILES_BANK42_FRAME054),
    { { -5, -21, 30, 45 } },
};
const struct sprite_frame_1box gSpriteBank42Frame055 = {
    SPRITE_FRAME(gSpriteBank42Frame055, SPRITE_TILES_BANK42_FRAME055),
    { { -5, -21, 30, 45 } },
};
const struct sprite_frame_1box gSpriteBank42Frame056 = {
    SPRITE_FRAME(gSpriteBank42Frame056, SPRITE_TILES_BANK42_FRAME056),
    { { -5, -21, 30, 45 } },
};
const struct sprite_frame_1box gSpriteBank42Frame057 = {
    SPRITE_FRAME(gSpriteBank42Frame057, SPRITE_TILES_BANK42_FRAME057),
    { { -5, -21, 30, 45 } },
};
const struct sprite_frame_1box gSpriteBank42Frame058 = {
    SPRITE_FRAME(gSpriteBank42Frame058, SPRITE_TILES_BANK42_FRAME058),
    { { -5, -21, 30, 45 } },
};
const struct sprite_frame_1box gSpriteBank42Frame059 = {
    SPRITE_FRAME(gSpriteBank42Frame059, SPRITE_TILES_BANK42_FRAME059),
    { { -5, -21, 30, 45 } },
};
const struct sprite_frame_1box gSpriteBank42Frame060 = {
    SPRITE_FRAME(gSpriteBank42Frame060, SPRITE_TILES_BANK42_FRAME060),
    { { -5, -21, 30, 45 } },
};
const struct sprite_frame_1box gSpriteBank42Frame061 = {
    SPRITE_FRAME(gSpriteBank42Frame061, SPRITE_TILES_BANK42_FRAME061),
    { { -5, -21, 30, 45 } },
};
const struct sprite_frame_1box gSpriteBank42Frame062 = {
    SPRITE_FRAME(gSpriteBank42Frame062, SPRITE_TILES_BANK42_FRAME062),
    { { -5, -21, 30, 45 } },
};
const struct sprite_frame_1box gSpriteBank42Frame063 = {
    SPRITE_FRAME(gSpriteBank42Frame063, SPRITE_TILES_BANK42_FRAME063),
    { { -9, -10, 35, 39 } },
};
const struct sprite_frame_1box gSpriteBank42Frame064 = {
    SPRITE_FRAME(gSpriteBank42Frame064, SPRITE_TILES_BANK42_FRAME064),
    { { -9, -10, 35, 39 } },
};
const struct sprite_frame_1box gSpriteBank42Frame065 = {
    SPRITE_FRAME(gSpriteBank42Frame065, SPRITE_TILES_BANK42_FRAME065),
    { { -9, -10, 35, 39 } },
};
const struct sprite_frame_1box gSpriteBank42Frame066 = {
    SPRITE_FRAME(gSpriteBank42Frame066, SPRITE_TILES_BANK42_FRAME066),
    { { -9, -10, 35, 39 } },
};
const struct sprite_frame_1box gSpriteBank42Frame067 = {
    SPRITE_FRAME(gSpriteBank42Frame067, SPRITE_TILES_BANK42_FRAME067),
    { { -9, -10, 35, 39 } },
};
const struct sprite_frame_1box gSpriteBank42Frame068 = {
    SPRITE_FRAME(gSpriteBank42Frame068, SPRITE_TILES_BANK42_FRAME068),
    { { -9, -10, 35, 39 } },
};
const struct sprite_frame_1box gSpriteBank42Frame069 = {
    SPRITE_FRAME(gSpriteBank42Frame069, SPRITE_TILES_BANK42_FRAME069),
    { { -9, -10, 35, 39 } },
};
const struct sprite_frame_1box gSpriteBank42Frame070 = {
    SPRITE_FRAME(gSpriteBank42Frame070, SPRITE_TILES_BANK42_FRAME070),
    { { -9, -10, 35, 39 } },
};
const struct sprite_frame_1box gSpriteBank42Frame071 = {
    SPRITE_FRAME(gSpriteBank42Frame071, SPRITE_TILES_BANK42_FRAME071),
    { { -9, -10, 35, 39 } },
};
const struct sprite_frame_1box gSpriteBank42Frame072 = {
    SPRITE_FRAME(gSpriteBank42Frame072, SPRITE_TILES_BANK42_FRAME072),
    { { -9, -10, 35, 39 } },
};
const struct sprite_frame_1box gSpriteBank42Frame073 = {
    SPRITE_FRAME(gSpriteBank42Frame073, SPRITE_TILES_BANK42_FRAME073),
    { { -9, -10, 35, 39 } },
};
const struct sprite_frame_1box gSpriteBank42Frame074 = {
    SPRITE_FRAME(gSpriteBank42Frame074, SPRITE_TILES_BANK42_FRAME074),
    { { -9, -10, 35, 39 } },
};
const struct sprite_frame_1box gSpriteBank42Frame075 = {
    SPRITE_FRAME(gSpriteBank42Frame075, SPRITE_TILES_BANK42_FRAME075),
    { { -9, -10, 35, 39 } },
};
const struct sprite_frame_1box gSpriteBank42Frame076 = {
    SPRITE_FRAME(gSpriteBank42Frame076, SPRITE_TILES_BANK42_FRAME076),
    { { -9, -10, 35, 39 } },
};
const struct sprite_frame_1box gSpriteBank42Frame077 = {
    SPRITE_FRAME(gSpriteBank42Frame077, SPRITE_TILES_BANK42_FRAME077),
    { { -9, -10, 35, 39 } },
};
const struct sprite_frame_1box gSpriteBank42Frame078 = {
    SPRITE_FRAME(gSpriteBank42Frame078, SPRITE_TILES_BANK42_FRAME078),
    { { -9, -10, 35, 39 } },
};
const struct sprite_frame_1box gSpriteBank42Frame079 = {
    SPRITE_FRAME(gSpriteBank42Frame079, SPRITE_TILES_BANK42_FRAME079),
    { { -9, -10, 35, 39 } },
};
const struct sprite_frame_1box gSpriteBank42Frame080 = {
    SPRITE_FRAME(gSpriteBank42Frame080, SPRITE_TILES_BANK42_FRAME080),
    { { -5, -24, 31, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame081 = {
    SPRITE_FRAME(gSpriteBank42Frame081, SPRITE_TILES_BANK42_FRAME081),
    { { -5, -24, 31, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame082 = {
    SPRITE_FRAME(gSpriteBank42Frame082, SPRITE_TILES_BANK42_FRAME082),
    { { -5, -24, 31, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame083 = {
    SPRITE_FRAME(gSpriteBank42Frame083, SPRITE_TILES_BANK42_FRAME083),
    { { -5, -24, 31, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame084 = {
    SPRITE_FRAME(gSpriteBank42Frame084, SPRITE_TILES_BANK42_FRAME084),
    { { -5, -24, 31, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame085 = {
    SPRITE_FRAME(gSpriteBank42Frame085, SPRITE_TILES_BANK42_FRAME085),
    { { -5, -24, 31, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame086 = {
    SPRITE_FRAME(gSpriteBank42Frame086, SPRITE_TILES_BANK42_FRAME086),
    { { -5, -24, 31, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame087 = {
    SPRITE_FRAME(gSpriteBank42Frame087, SPRITE_TILES_BANK42_FRAME087),
    { { -5, -24, 31, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame088 = {
    SPRITE_FRAME(gSpriteBank42Frame088, SPRITE_TILES_BANK42_FRAME088),
    { { -5, -24, 31, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame089 = {
    SPRITE_FRAME(gSpriteBank42Frame089, SPRITE_TILES_BANK42_FRAME089),
    { { -5, -24, 31, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame090 = {
    SPRITE_FRAME(gSpriteBank42Frame090, SPRITE_TILES_BANK42_FRAME090),
    { { -5, -24, 31, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame091 = {
    SPRITE_FRAME(gSpriteBank42Frame091, SPRITE_TILES_BANK42_FRAME091),
    { { -5, -24, 31, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame092 = {
    SPRITE_FRAME(gSpriteBank42Frame092, SPRITE_TILES_BANK42_FRAME092),
    { { -5, -24, 31, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame093 = {
    SPRITE_FRAME(gSpriteBank42Frame093, SPRITE_TILES_BANK42_FRAME093),
    { { -5, -24, 31, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame094 = {
    SPRITE_FRAME(gSpriteBank42Frame094, SPRITE_TILES_BANK42_FRAME094),
    { { -5, -24, 31, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame095 = {
    SPRITE_FRAME(gSpriteBank42Frame095, SPRITE_TILES_BANK42_FRAME095),
    { { -5, -24, 31, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame096 = {
    SPRITE_FRAME(gSpriteBank42Frame096, SPRITE_TILES_BANK42_FRAME096),
    { { -5, -24, 31, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame097 = {
    SPRITE_FRAME(gSpriteBank42Frame097, SPRITE_TILES_BANK42_FRAME097),
    { { -5, -24, 31, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame098 = {
    SPRITE_FRAME(gSpriteBank42Frame098, SPRITE_TILES_BANK42_FRAME098),
    { { -5, -24, 31, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame099 = {
    SPRITE_FRAME(gSpriteBank42Frame099, SPRITE_TILES_BANK42_FRAME099),
    { { -5, -24, 31, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame100 = {
    SPRITE_FRAME(gSpriteBank42Frame100, SPRITE_TILES_BANK42_FRAME100),
    { { -5, -24, 31, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame101 = {
    SPRITE_FRAME(gSpriteBank42Frame101, SPRITE_TILES_BANK42_FRAME101),
    { { -5, -24, 31, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame102 = {
    SPRITE_FRAME(gSpriteBank42Frame102, SPRITE_TILES_BANK42_FRAME102),
    { { -5, -24, 31, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame103 = {
    SPRITE_FRAME(gSpriteBank42Frame103, SPRITE_TILES_BANK42_FRAME103),
    { { -5, -24, 31, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame104 = {
    SPRITE_FRAME(gSpriteBank42Frame104, SPRITE_TILES_BANK42_FRAME104),
    { { -5, -24, 31, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame105 = {
    SPRITE_FRAME(gSpriteBank42Frame105, SPRITE_TILES_BANK42_FRAME105),
    { { -5, -24, 31, 49 } },
};
const struct sprite_frame_1box gSpriteBank42Frame106 = {
    SPRITE_FRAME(gSpriteBank42Frame106, SPRITE_TILES_BANK42_FRAME106),
    { { -5, -24, 31, 49 } },
};
const struct sprite_frame gSpriteBank42Frame107 = SPRITE_FRAME(gSpriteBank42Frame107, SPRITE_TILES_BANK42_FRAME107);
const struct sprite_frame gSpriteBank42Frame108 = SPRITE_FRAME(gSpriteBank42Frame108, SPRITE_TILES_BANK42_FRAME108);
const struct sprite_frame gSpriteBank42Frame109 = SPRITE_FRAME(gSpriteBank42Frame109, SPRITE_TILES_BANK42_FRAME109);
const struct sprite_frame gSpriteBank42Frame110 = SPRITE_FRAME(gSpriteBank42Frame110, SPRITE_TILES_BANK42_FRAME110);
const struct sprite_frame gSpriteBank42Frame111 = SPRITE_FRAME(gSpriteBank42Frame111, SPRITE_TILES_BANK42_FRAME111);
const struct sprite_frame gSpriteBank42Frame112 = SPRITE_FRAME(gSpriteBank42Frame112, SPRITE_TILES_BANK42_FRAME112);
const struct sprite_frame gSpriteBank42Frame113 = SPRITE_FRAME(gSpriteBank42Frame113, SPRITE_TILES_BANK42_FRAME113);
const struct sprite_frame gSpriteBank42Frame114 = SPRITE_FRAME(gSpriteBank42Frame114, SPRITE_TILES_BANK42_FRAME114);
const struct sprite_frame gSpriteBank42Frame115 = SPRITE_FRAME(gSpriteBank42Frame115, SPRITE_TILES_BANK42_FRAME115);
const struct sprite_frame gSpriteBank42Frame116 = SPRITE_FRAME(gSpriteBank42Frame116, SPRITE_TILES_BANK42_FRAME116);
const struct sprite_frame gSpriteBank42Frame117 = SPRITE_FRAME(gSpriteBank42Frame117, SPRITE_TILES_BANK42_FRAME117);
const struct sprite_frame gSpriteBank42Frame118 = SPRITE_FRAME(gSpriteBank42Frame118, SPRITE_TILES_BANK42_FRAME118);
const struct sprite_frame gSpriteBank42Frame119 = SPRITE_FRAME(gSpriteBank42Frame119, SPRITE_TILES_BANK42_FRAME119);
const struct sprite_frame gSpriteBank42Frame120 = SPRITE_FRAME(gSpriteBank42Frame120, SPRITE_TILES_BANK42_FRAME120);
const struct sprite_frame gSpriteBank42Frame121 = SPRITE_FRAME(gSpriteBank42Frame121, SPRITE_TILES_BANK42_FRAME121);
const struct sprite_frame gSpriteBank42Frame122 = SPRITE_FRAME(gSpriteBank42Frame122, SPRITE_TILES_BANK42_FRAME122);
const struct sprite_frame gSpriteBank42Frame123 = SPRITE_FRAME(gSpriteBank42Frame123, SPRITE_TILES_BANK42_FRAME123);
const struct sprite_frame gSpriteBank42Frame124 = SPRITE_FRAME(gSpriteBank42Frame124, SPRITE_TILES_BANK42_FRAME124);
const struct sprite_frame gSpriteBank42Frame125 = SPRITE_FRAME(gSpriteBank42Frame125, SPRITE_TILES_BANK42_FRAME125);
const struct sprite_frame gSpriteBank42Frame126 = SPRITE_FRAME(gSpriteBank42Frame126, SPRITE_TILES_BANK42_FRAME126);
const struct sprite_frame gSpriteBank42Frame127 = SPRITE_FRAME(gSpriteBank42Frame127, SPRITE_TILES_BANK42_FRAME127);
const struct sprite_frame gSpriteBank42Frame128 = SPRITE_FRAME(gSpriteBank42Frame128, SPRITE_TILES_BANK42_FRAME128);
const struct sprite_frame gSpriteBank42Frame129 = SPRITE_FRAME(gSpriteBank42Frame129, SPRITE_TILES_BANK42_FRAME129);
const struct sprite_frame gSpriteBank42Frame130 = SPRITE_FRAME(gSpriteBank42Frame130, SPRITE_TILES_BANK42_FRAME130);
const struct sprite_frame gSpriteBank42Frame131 = SPRITE_FRAME(gSpriteBank42Frame131, SPRITE_TILES_BANK42_FRAME131);
const struct sprite_frame_1box gSpriteBank42Frame132 = {
    SPRITE_FRAME(gSpriteBank42Frame132, SPRITE_TILES_BANK42_FRAME132),
    { { -7, -2, 14, 4 } },
};
const struct sprite_frame_1box gSpriteBank42Frame133 = {
    SPRITE_FRAME(gSpriteBank42Frame133, SPRITE_TILES_BANK42_FRAME133),
    { { -7, -2, 14, 4 } },
};
const struct sprite_frame_1box gSpriteBank42Frame134 = {
    SPRITE_FRAME(gSpriteBank42Frame134, SPRITE_TILES_BANK42_FRAME134),
    { { -7, -2, 14, 4 } },
};

/* The piece arrays, from games/n2/graphics/sprites/bank42_evil_crunch/bank.json. */
#include "sprites/bank42_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 43: 34 animations, 234 frames, games/n2/graphics/sprites/bank43_evil_coco/. */

extern const u16 gSpriteBank43Anim00Seq[9];
extern const u16 gSpriteBank43Anim01Seq[10];
extern const u16 gSpriteBank43Anim02Seq[10];
extern const u16 gSpriteBank43Anim03Seq[10];
extern const u16 gSpriteBank43Anim04Seq[10];
extern const u16 gSpriteBank43Anim05Seq[10];
extern const u16 gSpriteBank43Anim06Seq[1];
extern const u16 gSpriteBank43Anim07Seq[10];
extern const u16 gSpriteBank43Anim08Seq[10];
extern const u16 gSpriteBank43Anim09Seq[10];
extern const u16 gSpriteBank43Anim10Seq[10];
extern const u16 gSpriteBank43Anim11Seq[10];
extern const u16 gSpriteBank43Anim12Seq[10];
extern const u16 gSpriteBank43Anim13Seq[10];
extern const u16 gSpriteBank43Anim14Seq[10];
extern const u16 gSpriteBank43Anim15Seq[10];
extern const u16 gSpriteBank43Anim16Seq[10];
extern const u16 gSpriteBank43Anim17Seq[10];
extern const u16 gSpriteBank43Anim18Seq[10];
extern const u16 gSpriteBank43Anim19Seq[10];
extern const u16 gSpriteBank43Anim20Seq[10];
extern const u16 gSpriteBank43Anim21Seq[10];
extern const u16 gSpriteBank43Anim22Seq[10];
extern const u16 gSpriteBank43Anim23Seq[10];
extern const u16 gSpriteBank43Anim24Seq[1];
extern const u16 gSpriteBank43Anim25Seq[1];
extern const u16 gSpriteBank43Anim26Seq[10];
extern const u16 gSpriteBank43Anim27Seq[10];
extern const u16 gSpriteBank43Anim28Seq[10];
extern const u16 gSpriteBank43Anim29Seq[10];
extern const u16 gSpriteBank43Anim30Seq[5];
extern const u16 gSpriteBank43Anim31Seq[5];
extern const u16 gSpriteBank43Anim32Seq[38];
extern const u16 gSpriteBank43Anim33Seq[10];

extern const struct sprite_anim gSpriteBank43Anims[34] = {
    /* 0 */ {
        /* seq */ gSpriteBank43Anim00Seq,
        /* box */ { { -32, -25, 65, 51 }, { -62, -82, 38, 61 } },
        /* paletteId */ 73,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank43Anim01Seq,
        /* box */ { { -32, -25, 65, 51 }, { -42, -42, 87, 87 } },
        /* paletteId */ 74,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim01Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 2 */ {
        /* seq */ gSpriteBank43Anim02Seq,
        /* box */ { { -32, -25, 65, 51 }, { -33, -25, 68, 61 } },
        /* paletteId */ 75,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank43Anim03Seq,
        /* box */ { { -32, -25, 65, 51 }, { -3, 24, 21, 5 } },
        /* paletteId */ 75,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim03Seq),
        /* flags */ 0,
    },
    /* 4 */ {
        /* seq */ gSpriteBank43Anim04Seq,
        /* box */ { { -32, -25, 65, 51 }, { 8, 24, 19, 19 } },
        /* paletteId */ 75,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim04Seq),
        /* flags */ 0,
    },
    /* 5 */ {
        /* seq */ gSpriteBank43Anim05Seq,
        /* box */ { { -32, -25, 65, 51 }, { -18, -21, 15, 17 } },
        /* paletteId */ 75,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim05Seq),
        /* flags */ 0,
    },
    /* 6 */ {
        /* seq */ gSpriteBank43Anim06Seq,
        /* box */ { { -32, -25, 65, 51 }, { -30, -25, 63, 67 } },
        /* paletteId */ 75,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim06Seq),
        /* flags */ 0,
    },
    /* 7 */ {
        /* seq */ gSpriteBank43Anim07Seq,
        /* box */ { { -32, -25, 65, 51 }, { -32, -25, 65, 51 } },
        /* paletteId */ 75,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim07Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 8 */ {
        /* seq */ gSpriteBank43Anim08Seq,
        /* box */ { { -32, -25, 65, 51 }, { -2, 25, 17, 4 } },
        /* paletteId */ 75,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim08Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 9 */ {
        /* seq */ gSpriteBank43Anim09Seq,
        /* box */ { { -32, -25, 65, 51 }, { -2, 25, 17, 4 } },
        /* paletteId */ 75,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim09Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 10 */ {
        /* seq */ gSpriteBank43Anim10Seq,
        /* box */ { { -32, -25, 65, 51 }, { -2, 25, 17, 4 } },
        /* paletteId */ 75,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim10Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 11 */ {
        /* seq */ gSpriteBank43Anim11Seq,
        /* box */ { { -32, -25, 65, 51 }, { 9, 26, 7, 17 } },
        /* paletteId */ 75,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim11Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 12 */ {
        /* seq */ gSpriteBank43Anim12Seq,
        /* box */ { { -32, -25, 65, 51 }, { -62, -82, 78, 125 } },
        /* paletteId */ 75,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim12Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 13 */ {
        /* seq */ gSpriteBank43Anim13Seq,
        /* box */ { { -32, -25, 65, 51 }, { -62, -82, 78, 125 } },
        /* paletteId */ 75,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim13Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 14 */ {
        /* seq */ gSpriteBank43Anim14Seq,
        /* box */ { { -32, -25, 65, 51 }, { -13, 20, 23, 20 } },
        /* paletteId */ 75,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim14Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 15 */ {
        /* seq */ gSpriteBank43Anim15Seq,
        /* box */ { { -32, -25, 65, 51 }, { -62, -82, 79, 127 } },
        /* paletteId */ 74,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim15Seq),
        /* flags */ 0,
    },
    /* 16 */ {
        /* seq */ gSpriteBank43Anim16Seq,
        /* box */ { { -32, -25, 65, 51 }, { -19, -21, 16, 15 } },
        /* paletteId */ 75,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim16Seq),
        /* flags */ 0,
    },
    /* 17 */ {
        /* seq */ gSpriteBank43Anim17Seq,
        /* box */ { { -32, -25, 65, 51 }, { -19, -21, 16, 15 } },
        /* paletteId */ 75,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim17Seq),
        /* flags */ 0,
    },
    /* 18 */ {
        /* seq */ gSpriteBank43Anim18Seq,
        /* box */ { { -32, -25, 65, 51 }, { -19, -21, 16, 15 } },
        /* paletteId */ 75,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim18Seq),
        /* flags */ 0,
    },
    /* 19 */ {
        /* seq */ gSpriteBank43Anim19Seq,
        /* box */ { { -32, -25, 65, 51 }, { -32, -25, 65, 51 } },
        /* paletteId */ 75,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim19Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 20 */ {
        /* seq */ gSpriteBank43Anim20Seq,
        /* box */ { { -32, -25, 65, 51 }, { -2, 24, 17, 6 } },
        /* paletteId */ 75,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim20Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 21 */ {
        /* seq */ gSpriteBank43Anim21Seq,
        /* box */ { { -32, -25, 65, 51 }, { 9, 25, 7, 19 } },
        /* paletteId */ 75,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim21Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 22 */ {
        /* seq */ gSpriteBank43Anim22Seq,
        /* box */ { { -32, -25, 65, 51 }, { 8, 25, 9, 20 } },
        /* paletteId */ 75,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim22Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 23 */ {
        /* seq */ gSpriteBank43Anim23Seq,
        /* box */ { { -32, -25, 65, 51 }, { -13, 19, 23, 21 } },
        /* paletteId */ 75,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim23Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 24 */ {
        /* seq */ gSpriteBank43Anim24Seq,
        /* box */ { { -32, -25, 65, 51 }, { 1, -26, 12, 19 } },
        /* paletteId */ 76,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim24Seq),
        /* flags */ 0,
    },
    /* 25 */ {
        /* seq */ gSpriteBank43Anim25Seq,
        /* box */ { { -32, -25, 65, 51 }, { -54, -42, 36, 15 } },
        /* paletteId */ 76,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim25Seq),
        /* flags */ 0,
    },
    /* 26 */ {
        /* seq */ gSpriteBank43Anim26Seq,
        /* box */ { { -32, -25, 65, 51 }, { -16, -22, 12, 13 } },
        /* paletteId */ 75,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim26Seq),
        /* flags */ 0,
    },
    /* 27 */ {
        /* seq */ gSpriteBank43Anim27Seq,
        /* box */ { { -32, -25, 65, 51 }, { -16, -22, 12, 13 } },
        /* paletteId */ 75,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim27Seq),
        /* flags */ 0,
    },
    /* 28 */ {
        /* seq */ gSpriteBank43Anim28Seq,
        /* box */ { { -32, -25, 65, 51 }, { -62, -82, 58, 72 } },
        /* paletteId */ 75,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim28Seq),
        /* flags */ 0,
    },
    /* 29 */ {
        /* seq */ gSpriteBank43Anim29Seq,
        /* box */ { { -32, -25, 65, 51 }, { -62, -82, 58, 72 } },
        /* paletteId */ 75,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim29Seq),
        /* flags */ 0,
    },
    /* 30 */ {
        /* seq */ gSpriteBank43Anim30Seq,
        /* box */ { { -32, -25, 65, 51 }, { -37, -45, 14, 24 } },
        /* paletteId */ 73,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim30Seq),
        /* flags */ 0,
    },
    /* 31 */ {
        /* seq */ gSpriteBank43Anim31Seq,
        /* box */ { { -6, -9, 13, 19 }, { -7, -9, 15, 19 } },
        /* paletteId */ 75,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim31Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 32 */ {
        /* seq */ gSpriteBank43Anim32Seq,
        /* box */ { { -4, -24, 9, 48 }, { -9, -24, 17, 198 } },
        /* paletteId */ 77,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim32Seq),
        /* flags */ 0,
    },
    /* 33 */ {
        /* seq */ gSpriteBank43Anim33Seq,
        /* box */ { { -12, -9, 25, 19 }, { -15, -14, 28, 27 } },
        /* paletteId */ 78,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank43Anim33Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank43Anim00Seq[9] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8,
};
const u16 gSpriteBank43Anim01Seq[10] = {
    9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
};
const u16 gSpriteBank43Anim02Seq[10] = {
    19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
};
const u16 gSpriteBank43Anim03Seq[10] = {
    29, 30, 31, 32, 33, 34, 35, 36, 33, 37,
};
const u16 gSpriteBank43Anim04Seq[10] = {
    38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
};
const u16 gSpriteBank43Anim05Seq[10] = {
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
};
const u16 gSpriteBank43Anim06Seq[1] = {
    58,
};
const u16 gSpriteBank43Anim07Seq[10] = {
    59, 60, 61, 62, 63, 64, 65, 66, 67, 68,
};
const u16 gSpriteBank43Anim08Seq[10] = {
    29, 29, 29, 69, 69, 70, 69, 69, 29, 29,
};
const u16 gSpriteBank43Anim09Seq[10] = {
    29, 29, 69, 71, 72, 73, 74, 75, 76, 77,
};
const u16 gSpriteBank43Anim10Seq[10] = {
    77, 76, 75, 74, 73, 72, 71, 69, 29, 29,
};
const u16 gSpriteBank43Anim11Seq[10] = {
    78, 79, 80, 81, 82, 83, 84, 85, 80, 86,
};
const u16 gSpriteBank43Anim12Seq[10] = {
    86, 80, 87, 88, 89, 90, 91, 0, 0, 0,
};
const u16 gSpriteBank43Anim13Seq[10] = {
    0, 0, 0, 91, 90, 89, 88, 87, 80, 86,
};
const u16 gSpriteBank43Anim14Seq[10] = {
    92, 93, 94, 95, 96, 97, 98, 99, 100, 101,
};
const u16 gSpriteBank43Anim15Seq[10] = {
    0, 102, 103, 104, 105, 106, 107, 108, 109, 110,
};
const u16 gSpriteBank43Anim16Seq[10] = {
    111, 112, 113, 114, 115, 116, 117, 118, 119, 120,
};
const u16 gSpriteBank43Anim17Seq[10] = {
    121, 122, 123, 124, 125, 126, 127, 128, 129, 130,
};
const u16 gSpriteBank43Anim18Seq[10] = {
    130, 129, 128, 127, 126, 125, 124, 123, 122, 121,
};
const u16 gSpriteBank43Anim19Seq[10] = {
    59, 131, 132, 133, 134, 135, 136, 137, 138, 139,
};
const u16 gSpriteBank43Anim20Seq[10] = {
    29, 140, 141, 142, 143, 144, 145, 146, 147, 148,
};
const u16 gSpriteBank43Anim21Seq[10] = {
    78, 149, 150, 151, 152, 153, 154, 155, 156, 157,
};
const u16 gSpriteBank43Anim22Seq[10] = {
    78, 158, 159, 160, 161, 153, 162, 163, 164, 165,
};
const u16 gSpriteBank43Anim23Seq[10] = {
    92, 166, 167, 168, 169, 170, 171, 172, 173, 174,
};
const u16 gSpriteBank43Anim24Seq[1] = {
    175,
};
const u16 gSpriteBank43Anim25Seq[1] = {
    176,
};
const u16 gSpriteBank43Anim26Seq[10] = {
    177, 178, 179, 180, 181, 182, 181, 183, 184, 185,
};
const u16 gSpriteBank43Anim27Seq[10] = {
    186, 187, 188, 189, 190, 191, 192, 193, 194, 195,
};
const u16 gSpriteBank43Anim28Seq[10] = {
    0, 0, 0, 0, 196, 197, 198, 199, 200, 201,
};
const u16 gSpriteBank43Anim29Seq[10] = {
    201, 200, 199, 198, 197, 196, 0, 0, 0, 0,
};
const u16 gSpriteBank43Anim30Seq[5] = {
    202, 203, 204, 205, 206,
};
const u16 gSpriteBank43Anim31Seq[5] = {
    207, 208, 209, 210, 211,
};
const u16 gSpriteBank43Anim32Seq[38] = {
    212, 213, 214, 215, 216, 217, 218, 219, 220, 220, 220, 220, 220, 221, 220, 220,
    221, 220, 221, 220, 221, 220, 222, 220, 221, 220, 222, 220, 223, 220, 223, 220,
    223, 220, 223, 220, 223, 220,
};
const u16 gSpriteBank43Anim33Seq[10] = {
    224, 225, 226, 227, 228, 229, 230, 231, 232, 233,
};

extern const struct sprite_frame *const gSpriteBank43Frames[234] = {
    &gSpriteBank43Frame000,
    &gSpriteBank43Frame001,
    &gSpriteBank43Frame002,
    &gSpriteBank43Frame003,
    &gSpriteBank43Frame004,
    &gSpriteBank43Frame005,
    &gSpriteBank43Frame006,
    &gSpriteBank43Frame007,
    &gSpriteBank43Frame008,
    &gSpriteBank43Frame009,
    &gSpriteBank43Frame010,
    &gSpriteBank43Frame011,
    &gSpriteBank43Frame012,
    &gSpriteBank43Frame013,
    &gSpriteBank43Frame014,
    &gSpriteBank43Frame015,
    &gSpriteBank43Frame016,
    &gSpriteBank43Frame017,
    &gSpriteBank43Frame018,
    &gSpriteBank43Frame019.frame,
    &gSpriteBank43Frame020.frame,
    &gSpriteBank43Frame021.frame,
    &gSpriteBank43Frame022.frame,
    &gSpriteBank43Frame023.frame,
    &gSpriteBank43Frame024.frame,
    &gSpriteBank43Frame025.frame,
    &gSpriteBank43Frame026.frame,
    &gSpriteBank43Frame027.frame,
    &gSpriteBank43Frame028.frame,
    &gSpriteBank43Frame029,
    &gSpriteBank43Frame030,
    &gSpriteBank43Frame031,
    &gSpriteBank43Frame032,
    &gSpriteBank43Frame033,
    &gSpriteBank43Frame034,
    &gSpriteBank43Frame035,
    &gSpriteBank43Frame036,
    &gSpriteBank43Frame037,
    &gSpriteBank43Frame038,
    &gSpriteBank43Frame039,
    &gSpriteBank43Frame040,
    &gSpriteBank43Frame041,
    &gSpriteBank43Frame042,
    &gSpriteBank43Frame043,
    &gSpriteBank43Frame044,
    &gSpriteBank43Frame045,
    &gSpriteBank43Frame046,
    &gSpriteBank43Frame047,
    &gSpriteBank43Frame048,
    &gSpriteBank43Frame049,
    &gSpriteBank43Frame050,
    &gSpriteBank43Frame051,
    &gSpriteBank43Frame052,
    &gSpriteBank43Frame053,
    &gSpriteBank43Frame054,
    &gSpriteBank43Frame055,
    &gSpriteBank43Frame056,
    &gSpriteBank43Frame057,
    &gSpriteBank43Frame058,
    &gSpriteBank43Frame059.frame,
    &gSpriteBank43Frame060.frame,
    &gSpriteBank43Frame061.frame,
    &gSpriteBank43Frame062.frame,
    &gSpriteBank43Frame063.frame,
    &gSpriteBank43Frame064.frame,
    &gSpriteBank43Frame065.frame,
    &gSpriteBank43Frame066.frame,
    &gSpriteBank43Frame067.frame,
    &gSpriteBank43Frame068.frame,
    &gSpriteBank43Frame069,
    &gSpriteBank43Frame070,
    &gSpriteBank43Frame071,
    &gSpriteBank43Frame072,
    &gSpriteBank43Frame073,
    &gSpriteBank43Frame074,
    &gSpriteBank43Frame075,
    &gSpriteBank43Frame076,
    &gSpriteBank43Frame077,
    &gSpriteBank43Frame078,
    &gSpriteBank43Frame079,
    &gSpriteBank43Frame080,
    &gSpriteBank43Frame081,
    &gSpriteBank43Frame082,
    &gSpriteBank43Frame083,
    &gSpriteBank43Frame084,
    &gSpriteBank43Frame085,
    &gSpriteBank43Frame086,
    &gSpriteBank43Frame087,
    &gSpriteBank43Frame088,
    &gSpriteBank43Frame089,
    &gSpriteBank43Frame090,
    &gSpriteBank43Frame091,
    &gSpriteBank43Frame092,
    &gSpriteBank43Frame093,
    &gSpriteBank43Frame094,
    &gSpriteBank43Frame095,
    &gSpriteBank43Frame096,
    &gSpriteBank43Frame097,
    &gSpriteBank43Frame098,
    &gSpriteBank43Frame099,
    &gSpriteBank43Frame100,
    &gSpriteBank43Frame101,
    &gSpriteBank43Frame102,
    &gSpriteBank43Frame103,
    &gSpriteBank43Frame104,
    &gSpriteBank43Frame105,
    &gSpriteBank43Frame106,
    &gSpriteBank43Frame107,
    &gSpriteBank43Frame108,
    &gSpriteBank43Frame109,
    &gSpriteBank43Frame110,
    &gSpriteBank43Frame111,
    &gSpriteBank43Frame112,
    &gSpriteBank43Frame113,
    &gSpriteBank43Frame114,
    &gSpriteBank43Frame115,
    &gSpriteBank43Frame116,
    &gSpriteBank43Frame117,
    &gSpriteBank43Frame118,
    &gSpriteBank43Frame119,
    &gSpriteBank43Frame120,
    &gSpriteBank43Frame121,
    &gSpriteBank43Frame122,
    &gSpriteBank43Frame123,
    &gSpriteBank43Frame124,
    &gSpriteBank43Frame125,
    &gSpriteBank43Frame126,
    &gSpriteBank43Frame127,
    &gSpriteBank43Frame128,
    &gSpriteBank43Frame129,
    &gSpriteBank43Frame130,
    &gSpriteBank43Frame131.frame,
    &gSpriteBank43Frame132.frame,
    &gSpriteBank43Frame133.frame,
    &gSpriteBank43Frame134.frame,
    &gSpriteBank43Frame135.frame,
    &gSpriteBank43Frame136.frame,
    &gSpriteBank43Frame137.frame,
    &gSpriteBank43Frame138.frame,
    &gSpriteBank43Frame139.frame,
    &gSpriteBank43Frame140,
    &gSpriteBank43Frame141,
    &gSpriteBank43Frame142,
    &gSpriteBank43Frame143,
    &gSpriteBank43Frame144,
    &gSpriteBank43Frame145,
    &gSpriteBank43Frame146,
    &gSpriteBank43Frame147,
    &gSpriteBank43Frame148,
    &gSpriteBank43Frame149,
    &gSpriteBank43Frame150,
    &gSpriteBank43Frame151,
    &gSpriteBank43Frame152,
    &gSpriteBank43Frame153,
    &gSpriteBank43Frame154,
    &gSpriteBank43Frame155,
    &gSpriteBank43Frame156,
    &gSpriteBank43Frame157,
    &gSpriteBank43Frame158,
    &gSpriteBank43Frame159,
    &gSpriteBank43Frame160,
    &gSpriteBank43Frame161,
    &gSpriteBank43Frame162,
    &gSpriteBank43Frame163,
    &gSpriteBank43Frame164,
    &gSpriteBank43Frame165,
    &gSpriteBank43Frame166,
    &gSpriteBank43Frame167,
    &gSpriteBank43Frame168,
    &gSpriteBank43Frame169,
    &gSpriteBank43Frame170,
    &gSpriteBank43Frame171,
    &gSpriteBank43Frame172,
    &gSpriteBank43Frame173,
    &gSpriteBank43Frame174,
    &gSpriteBank43Frame175,
    &gSpriteBank43Frame176,
    &gSpriteBank43Frame177,
    &gSpriteBank43Frame178,
    &gSpriteBank43Frame179,
    &gSpriteBank43Frame180,
    &gSpriteBank43Frame181,
    &gSpriteBank43Frame182,
    &gSpriteBank43Frame183,
    &gSpriteBank43Frame184,
    &gSpriteBank43Frame185,
    &gSpriteBank43Frame186,
    &gSpriteBank43Frame187,
    &gSpriteBank43Frame188,
    &gSpriteBank43Frame189,
    &gSpriteBank43Frame190,
    &gSpriteBank43Frame191,
    &gSpriteBank43Frame192,
    &gSpriteBank43Frame193,
    &gSpriteBank43Frame194,
    &gSpriteBank43Frame195,
    &gSpriteBank43Frame196,
    &gSpriteBank43Frame197,
    &gSpriteBank43Frame198,
    &gSpriteBank43Frame199,
    &gSpriteBank43Frame200,
    &gSpriteBank43Frame201,
    &gSpriteBank43Frame202,
    &gSpriteBank43Frame203,
    &gSpriteBank43Frame204,
    &gSpriteBank43Frame205,
    &gSpriteBank43Frame206,
    &gSpriteBank43Frame207.frame,
    &gSpriteBank43Frame208.frame,
    &gSpriteBank43Frame209.frame,
    &gSpriteBank43Frame210.frame,
    &gSpriteBank43Frame211.frame,
    &gSpriteBank43Frame212,
    &gSpriteBank43Frame213,
    &gSpriteBank43Frame214,
    &gSpriteBank43Frame215,
    &gSpriteBank43Frame216,
    &gSpriteBank43Frame217,
    &gSpriteBank43Frame218,
    &gSpriteBank43Frame219.frame,
    &gSpriteBank43Frame220.frame,
    &gSpriteBank43Frame221.frame,
    &gSpriteBank43Frame222.frame,
    &gSpriteBank43Frame223.frame,
    &gSpriteBank43Frame224,
    &gSpriteBank43Frame225,
    &gSpriteBank43Frame226,
    &gSpriteBank43Frame227,
    &gSpriteBank43Frame228,
    &gSpriteBank43Frame229,
    &gSpriteBank43Frame230,
    &gSpriteBank43Frame231,
    &gSpriteBank43Frame232,
    &gSpriteBank43Frame233,
};

const struct sprite_frame gSpriteBank43Frame000 = { gSpriteBank43Frame001Pos, gSpriteBank43Frame001Pieces, SPRITE_FRAME_TILES(SPRITE_TILES_BANK43_FRAME000, 0) };
const struct sprite_frame gSpriteBank43Frame001 = SPRITE_FRAME(gSpriteBank43Frame001, SPRITE_TILES_BANK43_FRAME001);
const struct sprite_frame gSpriteBank43Frame002 = SPRITE_FRAME(gSpriteBank43Frame002, SPRITE_TILES_BANK43_FRAME002);
const struct sprite_frame gSpriteBank43Frame003 = SPRITE_FRAME(gSpriteBank43Frame003, SPRITE_TILES_BANK43_FRAME003);
const struct sprite_frame gSpriteBank43Frame004 = SPRITE_FRAME(gSpriteBank43Frame004, SPRITE_TILES_BANK43_FRAME004);
const struct sprite_frame gSpriteBank43Frame005 = SPRITE_FRAME(gSpriteBank43Frame005, SPRITE_TILES_BANK43_FRAME005);
const struct sprite_frame gSpriteBank43Frame006 = SPRITE_FRAME(gSpriteBank43Frame006, SPRITE_TILES_BANK43_FRAME006);
const struct sprite_frame gSpriteBank43Frame007 = SPRITE_FRAME(gSpriteBank43Frame007, SPRITE_TILES_BANK43_FRAME007);
const struct sprite_frame gSpriteBank43Frame008 = SPRITE_FRAME(gSpriteBank43Frame008, SPRITE_TILES_BANK43_FRAME008);
const struct sprite_frame gSpriteBank43Frame009 = SPRITE_FRAME(gSpriteBank43Frame009, SPRITE_TILES_BANK43_FRAME009);
const struct sprite_frame gSpriteBank43Frame010 = SPRITE_FRAME(gSpriteBank43Frame010, SPRITE_TILES_BANK43_FRAME010);
const struct sprite_frame gSpriteBank43Frame011 = SPRITE_FRAME(gSpriteBank43Frame011, SPRITE_TILES_BANK43_FRAME011);
const struct sprite_frame gSpriteBank43Frame012 = SPRITE_FRAME(gSpriteBank43Frame012, SPRITE_TILES_BANK43_FRAME012);
const struct sprite_frame gSpriteBank43Frame013 = SPRITE_FRAME(gSpriteBank43Frame013, SPRITE_TILES_BANK43_FRAME013);
const struct sprite_frame gSpriteBank43Frame014 = SPRITE_FRAME(gSpriteBank43Frame014, SPRITE_TILES_BANK43_FRAME014);
const struct sprite_frame gSpriteBank43Frame015 = SPRITE_FRAME(gSpriteBank43Frame015, SPRITE_TILES_BANK43_FRAME015);
const struct sprite_frame gSpriteBank43Frame016 = SPRITE_FRAME(gSpriteBank43Frame016, SPRITE_TILES_BANK43_FRAME016);
const struct sprite_frame gSpriteBank43Frame017 = SPRITE_FRAME(gSpriteBank43Frame017, SPRITE_TILES_BANK43_FRAME017);
const struct sprite_frame gSpriteBank43Frame018 = SPRITE_FRAME(gSpriteBank43Frame018, SPRITE_TILES_BANK43_FRAME018);
const struct sprite_frame_1box gSpriteBank43Frame019 = {
    SPRITE_FRAME(gSpriteBank43Frame019, SPRITE_TILES_BANK43_FRAME019),
    { { -32, -25, 65, 51 } },
};
const struct sprite_frame_1box gSpriteBank43Frame020 = {
    SPRITE_FRAME(gSpriteBank43Frame020, SPRITE_TILES_BANK43_FRAME020),
    { { -29, -25, 63, 59 } },
};
const struct sprite_frame_1box gSpriteBank43Frame021 = {
    SPRITE_FRAME(gSpriteBank43Frame021, SPRITE_TILES_BANK43_FRAME021),
    { { -28, -25, 63, 61 } },
};
const struct sprite_frame_1box gSpriteBank43Frame022 = {
    SPRITE_FRAME(gSpriteBank43Frame022, SPRITE_TILES_BANK43_FRAME022),
    { { -29, -25, 63, 57 } },
};
const struct sprite_frame_1box gSpriteBank43Frame023 = {
    SPRITE_FRAME(gSpriteBank43Frame023, SPRITE_TILES_BANK43_FRAME023),
    { { -32, -25, 65, 51 } },
};
const struct sprite_frame_1box gSpriteBank43Frame024 = {
    SPRITE_FRAME(gSpriteBank43Frame024, SPRITE_TILES_BANK43_FRAME024),
    { { -33, -25, 66, 51 } },
};
const struct sprite_frame_1box gSpriteBank43Frame025 = {
    SPRITE_FRAME(gSpriteBank43Frame025, SPRITE_TILES_BANK43_FRAME025),
    { { -33, -25, 66, 51 } },
};
const struct sprite_frame_1box gSpriteBank43Frame026 = {
    SPRITE_FRAME(gSpriteBank43Frame026, SPRITE_TILES_BANK43_FRAME026),
    { { -32, -25, 65, 51 } },
};
const struct sprite_frame_1box gSpriteBank43Frame027 = {
    SPRITE_FRAME(gSpriteBank43Frame027, SPRITE_TILES_BANK43_FRAME027),
    { { -32, -25, 65, 52 } },
};
const struct sprite_frame_1box gSpriteBank43Frame028 = {
    SPRITE_FRAME(gSpriteBank43Frame028, SPRITE_TILES_BANK43_FRAME028),
    { { -32, -25, 65, 51 } },
};
const struct sprite_frame gSpriteBank43Frame029 = SPRITE_FRAME(gSpriteBank43Frame029, SPRITE_TILES_BANK43_FRAME029);
const struct sprite_frame gSpriteBank43Frame030 = SPRITE_FRAME(gSpriteBank43Frame030, SPRITE_TILES_BANK43_FRAME030);
const struct sprite_frame gSpriteBank43Frame031 = SPRITE_FRAME(gSpriteBank43Frame031, SPRITE_TILES_BANK43_FRAME031);
const struct sprite_frame gSpriteBank43Frame032 = SPRITE_FRAME(gSpriteBank43Frame032, SPRITE_TILES_BANK43_FRAME032);
const struct sprite_frame gSpriteBank43Frame033 = SPRITE_FRAME(gSpriteBank43Frame033, SPRITE_TILES_BANK43_FRAME033);
const struct sprite_frame gSpriteBank43Frame034 = SPRITE_FRAME(gSpriteBank43Frame034, SPRITE_TILES_BANK43_FRAME034);
const struct sprite_frame gSpriteBank43Frame035 = SPRITE_FRAME(gSpriteBank43Frame035, SPRITE_TILES_BANK43_FRAME035);
const struct sprite_frame gSpriteBank43Frame036 = SPRITE_FRAME(gSpriteBank43Frame036, SPRITE_TILES_BANK43_FRAME036);
const struct sprite_frame gSpriteBank43Frame037 = SPRITE_FRAME(gSpriteBank43Frame037, SPRITE_TILES_BANK43_FRAME037);
const struct sprite_frame gSpriteBank43Frame038 = SPRITE_FRAME(gSpriteBank43Frame038, SPRITE_TILES_BANK43_FRAME038);
const struct sprite_frame gSpriteBank43Frame039 = SPRITE_FRAME(gSpriteBank43Frame039, SPRITE_TILES_BANK43_FRAME039);
const struct sprite_frame gSpriteBank43Frame040 = SPRITE_FRAME(gSpriteBank43Frame040, SPRITE_TILES_BANK43_FRAME040);
const struct sprite_frame gSpriteBank43Frame041 = SPRITE_FRAME(gSpriteBank43Frame041, SPRITE_TILES_BANK43_FRAME041);
const struct sprite_frame gSpriteBank43Frame042 = SPRITE_FRAME(gSpriteBank43Frame042, SPRITE_TILES_BANK43_FRAME042);
const struct sprite_frame gSpriteBank43Frame043 = SPRITE_FRAME(gSpriteBank43Frame043, SPRITE_TILES_BANK43_FRAME043);
const struct sprite_frame gSpriteBank43Frame044 = SPRITE_FRAME(gSpriteBank43Frame044, SPRITE_TILES_BANK43_FRAME044);
const struct sprite_frame gSpriteBank43Frame045 = SPRITE_FRAME(gSpriteBank43Frame045, SPRITE_TILES_BANK43_FRAME045);
const struct sprite_frame gSpriteBank43Frame046 = SPRITE_FRAME(gSpriteBank43Frame046, SPRITE_TILES_BANK43_FRAME046);
const struct sprite_frame gSpriteBank43Frame047 = SPRITE_FRAME(gSpriteBank43Frame047, SPRITE_TILES_BANK43_FRAME047);
const struct sprite_frame gSpriteBank43Frame048 = SPRITE_FRAME(gSpriteBank43Frame048, SPRITE_TILES_BANK43_FRAME048);
const struct sprite_frame gSpriteBank43Frame049 = SPRITE_FRAME(gSpriteBank43Frame049, SPRITE_TILES_BANK43_FRAME049);
const struct sprite_frame gSpriteBank43Frame050 = SPRITE_FRAME(gSpriteBank43Frame050, SPRITE_TILES_BANK43_FRAME050);
const struct sprite_frame gSpriteBank43Frame051 = SPRITE_FRAME(gSpriteBank43Frame051, SPRITE_TILES_BANK43_FRAME051);
const struct sprite_frame gSpriteBank43Frame052 = SPRITE_FRAME(gSpriteBank43Frame052, SPRITE_TILES_BANK43_FRAME052);
const struct sprite_frame gSpriteBank43Frame053 = SPRITE_FRAME(gSpriteBank43Frame053, SPRITE_TILES_BANK43_FRAME053);
const struct sprite_frame gSpriteBank43Frame054 = SPRITE_FRAME(gSpriteBank43Frame054, SPRITE_TILES_BANK43_FRAME054);
const struct sprite_frame gSpriteBank43Frame055 = SPRITE_FRAME(gSpriteBank43Frame055, SPRITE_TILES_BANK43_FRAME055);
const struct sprite_frame gSpriteBank43Frame056 = SPRITE_FRAME(gSpriteBank43Frame056, SPRITE_TILES_BANK43_FRAME056);
const struct sprite_frame gSpriteBank43Frame057 = SPRITE_FRAME(gSpriteBank43Frame057, SPRITE_TILES_BANK43_FRAME057);
const struct sprite_frame gSpriteBank43Frame058 = SPRITE_FRAME(gSpriteBank43Frame058, SPRITE_TILES_BANK43_FRAME058);
const struct sprite_frame_1box gSpriteBank43Frame059 = {
    SPRITE_FRAME(gSpriteBank43Frame059, SPRITE_TILES_BANK43_FRAME059),
    { { -32, -25, 65, 51 } },
};
const struct sprite_frame_1box gSpriteBank43Frame060 = {
    SPRITE_FRAME(gSpriteBank43Frame060, SPRITE_TILES_BANK43_FRAME060),
    { { -32, -25, 65, 51 } },
};
const struct sprite_frame_1box gSpriteBank43Frame061 = {
    SPRITE_FRAME(gSpriteBank43Frame061, SPRITE_TILES_BANK43_FRAME061),
    { { -32, -25, 65, 51 } },
};
const struct sprite_frame_1box gSpriteBank43Frame062 = {
    SPRITE_FRAME(gSpriteBank43Frame062, SPRITE_TILES_BANK43_FRAME062),
    { { -32, -25, 65, 51 } },
};
const struct sprite_frame_1box gSpriteBank43Frame063 = {
    SPRITE_FRAME(gSpriteBank43Frame063, SPRITE_TILES_BANK43_FRAME063),
    { { -32, -25, 65, 51 } },
};
const struct sprite_frame_1box gSpriteBank43Frame064 = {
    SPRITE_FRAME(gSpriteBank43Frame064, SPRITE_TILES_BANK43_FRAME064),
    { { -32, -25, 65, 51 } },
};
const struct sprite_frame_1box gSpriteBank43Frame065 = {
    SPRITE_FRAME(gSpriteBank43Frame065, SPRITE_TILES_BANK43_FRAME065),
    { { -32, -25, 65, 51 } },
};
const struct sprite_frame_1box gSpriteBank43Frame066 = {
    SPRITE_FRAME(gSpriteBank43Frame066, SPRITE_TILES_BANK43_FRAME066),
    { { -32, -25, 65, 51 } },
};
const struct sprite_frame_1box gSpriteBank43Frame067 = {
    SPRITE_FRAME(gSpriteBank43Frame067, SPRITE_TILES_BANK43_FRAME067),
    { { -32, -25, 65, 51 } },
};
const struct sprite_frame_1box gSpriteBank43Frame068 = {
    SPRITE_FRAME(gSpriteBank43Frame068, SPRITE_TILES_BANK43_FRAME068),
    { { -32, -25, 65, 51 } },
};
const struct sprite_frame gSpriteBank43Frame069 = SPRITE_FRAME(gSpriteBank43Frame069, SPRITE_TILES_BANK43_FRAME069);
const struct sprite_frame gSpriteBank43Frame070 = SPRITE_FRAME(gSpriteBank43Frame070, SPRITE_TILES_BANK43_FRAME070);
const struct sprite_frame gSpriteBank43Frame071 = SPRITE_FRAME(gSpriteBank43Frame071, SPRITE_TILES_BANK43_FRAME071);
const struct sprite_frame gSpriteBank43Frame072 = SPRITE_FRAME(gSpriteBank43Frame072, SPRITE_TILES_BANK43_FRAME072);
const struct sprite_frame gSpriteBank43Frame073 = SPRITE_FRAME(gSpriteBank43Frame073, SPRITE_TILES_BANK43_FRAME073);
const struct sprite_frame gSpriteBank43Frame074 = SPRITE_FRAME(gSpriteBank43Frame074, SPRITE_TILES_BANK43_FRAME074);
const struct sprite_frame gSpriteBank43Frame075 = SPRITE_FRAME(gSpriteBank43Frame075, SPRITE_TILES_BANK43_FRAME075);
const struct sprite_frame gSpriteBank43Frame076 = SPRITE_FRAME(gSpriteBank43Frame076, SPRITE_TILES_BANK43_FRAME076);
const struct sprite_frame gSpriteBank43Frame077 = SPRITE_FRAME(gSpriteBank43Frame077, SPRITE_TILES_BANK43_FRAME077);
const struct sprite_frame gSpriteBank43Frame078 = SPRITE_FRAME(gSpriteBank43Frame078, SPRITE_TILES_BANK43_FRAME078);
const struct sprite_frame gSpriteBank43Frame079 = SPRITE_FRAME(gSpriteBank43Frame079, SPRITE_TILES_BANK43_FRAME079);
const struct sprite_frame gSpriteBank43Frame080 = SPRITE_FRAME(gSpriteBank43Frame080, SPRITE_TILES_BANK43_FRAME080);
const struct sprite_frame gSpriteBank43Frame081 = SPRITE_FRAME(gSpriteBank43Frame081, SPRITE_TILES_BANK43_FRAME081);
const struct sprite_frame gSpriteBank43Frame082 = SPRITE_FRAME(gSpriteBank43Frame082, SPRITE_TILES_BANK43_FRAME082);
const struct sprite_frame gSpriteBank43Frame083 = SPRITE_FRAME(gSpriteBank43Frame083, SPRITE_TILES_BANK43_FRAME083);
const struct sprite_frame gSpriteBank43Frame084 = SPRITE_FRAME(gSpriteBank43Frame084, SPRITE_TILES_BANK43_FRAME084);
const struct sprite_frame gSpriteBank43Frame085 = SPRITE_FRAME(gSpriteBank43Frame085, SPRITE_TILES_BANK43_FRAME085);
const struct sprite_frame gSpriteBank43Frame086 = SPRITE_FRAME(gSpriteBank43Frame086, SPRITE_TILES_BANK43_FRAME086);
const struct sprite_frame gSpriteBank43Frame087 = SPRITE_FRAME(gSpriteBank43Frame087, SPRITE_TILES_BANK43_FRAME087);
const struct sprite_frame gSpriteBank43Frame088 = SPRITE_FRAME(gSpriteBank43Frame088, SPRITE_TILES_BANK43_FRAME088);
const struct sprite_frame gSpriteBank43Frame089 = SPRITE_FRAME(gSpriteBank43Frame089, SPRITE_TILES_BANK43_FRAME089);
const struct sprite_frame gSpriteBank43Frame090 = SPRITE_FRAME(gSpriteBank43Frame090, SPRITE_TILES_BANK43_FRAME090);
const struct sprite_frame gSpriteBank43Frame091 = SPRITE_FRAME(gSpriteBank43Frame091, SPRITE_TILES_BANK43_FRAME091);
const struct sprite_frame gSpriteBank43Frame092 = SPRITE_FRAME(gSpriteBank43Frame092, SPRITE_TILES_BANK43_FRAME092);
const struct sprite_frame gSpriteBank43Frame093 = SPRITE_FRAME(gSpriteBank43Frame093, SPRITE_TILES_BANK43_FRAME093);
const struct sprite_frame gSpriteBank43Frame094 = SPRITE_FRAME(gSpriteBank43Frame094, SPRITE_TILES_BANK43_FRAME094);
const struct sprite_frame gSpriteBank43Frame095 = SPRITE_FRAME(gSpriteBank43Frame095, SPRITE_TILES_BANK43_FRAME095);
const struct sprite_frame gSpriteBank43Frame096 = SPRITE_FRAME(gSpriteBank43Frame096, SPRITE_TILES_BANK43_FRAME096);
const struct sprite_frame gSpriteBank43Frame097 = SPRITE_FRAME(gSpriteBank43Frame097, SPRITE_TILES_BANK43_FRAME097);
const struct sprite_frame gSpriteBank43Frame098 = SPRITE_FRAME(gSpriteBank43Frame098, SPRITE_TILES_BANK43_FRAME098);
const struct sprite_frame gSpriteBank43Frame099 = SPRITE_FRAME(gSpriteBank43Frame099, SPRITE_TILES_BANK43_FRAME099);
const struct sprite_frame gSpriteBank43Frame100 = SPRITE_FRAME(gSpriteBank43Frame100, SPRITE_TILES_BANK43_FRAME100);
const struct sprite_frame gSpriteBank43Frame101 = SPRITE_FRAME(gSpriteBank43Frame101, SPRITE_TILES_BANK43_FRAME101);
const struct sprite_frame gSpriteBank43Frame102 = SPRITE_FRAME(gSpriteBank43Frame102, SPRITE_TILES_BANK43_FRAME102);
const struct sprite_frame gSpriteBank43Frame103 = SPRITE_FRAME(gSpriteBank43Frame103, SPRITE_TILES_BANK43_FRAME103);
const struct sprite_frame gSpriteBank43Frame104 = SPRITE_FRAME(gSpriteBank43Frame104, SPRITE_TILES_BANK43_FRAME104);
const struct sprite_frame gSpriteBank43Frame105 = SPRITE_FRAME(gSpriteBank43Frame105, SPRITE_TILES_BANK43_FRAME105);
const struct sprite_frame gSpriteBank43Frame106 = SPRITE_FRAME(gSpriteBank43Frame106, SPRITE_TILES_BANK43_FRAME106);
const struct sprite_frame gSpriteBank43Frame107 = SPRITE_FRAME(gSpriteBank43Frame107, SPRITE_TILES_BANK43_FRAME107);
const struct sprite_frame gSpriteBank43Frame108 = SPRITE_FRAME(gSpriteBank43Frame108, SPRITE_TILES_BANK43_FRAME108);
const struct sprite_frame gSpriteBank43Frame109 = SPRITE_FRAME(gSpriteBank43Frame109, SPRITE_TILES_BANK43_FRAME109);
const struct sprite_frame gSpriteBank43Frame110 = SPRITE_FRAME(gSpriteBank43Frame110, SPRITE_TILES_BANK43_FRAME110);
const struct sprite_frame gSpriteBank43Frame111 = SPRITE_FRAME(gSpriteBank43Frame111, SPRITE_TILES_BANK43_FRAME111);
const struct sprite_frame gSpriteBank43Frame112 = SPRITE_FRAME(gSpriteBank43Frame112, SPRITE_TILES_BANK43_FRAME112);
const struct sprite_frame gSpriteBank43Frame113 = SPRITE_FRAME(gSpriteBank43Frame113, SPRITE_TILES_BANK43_FRAME113);
const struct sprite_frame gSpriteBank43Frame114 = SPRITE_FRAME(gSpriteBank43Frame114, SPRITE_TILES_BANK43_FRAME114);
const struct sprite_frame gSpriteBank43Frame115 = SPRITE_FRAME(gSpriteBank43Frame115, SPRITE_TILES_BANK43_FRAME115);
const struct sprite_frame gSpriteBank43Frame116 = SPRITE_FRAME(gSpriteBank43Frame116, SPRITE_TILES_BANK43_FRAME116);
const struct sprite_frame gSpriteBank43Frame117 = SPRITE_FRAME(gSpriteBank43Frame117, SPRITE_TILES_BANK43_FRAME117);
const struct sprite_frame gSpriteBank43Frame118 = SPRITE_FRAME(gSpriteBank43Frame118, SPRITE_TILES_BANK43_FRAME118);
const struct sprite_frame gSpriteBank43Frame119 = SPRITE_FRAME(gSpriteBank43Frame119, SPRITE_TILES_BANK43_FRAME119);
const struct sprite_frame gSpriteBank43Frame120 = SPRITE_FRAME(gSpriteBank43Frame120, SPRITE_TILES_BANK43_FRAME120);
const struct sprite_frame gSpriteBank43Frame121 = SPRITE_FRAME(gSpriteBank43Frame121, SPRITE_TILES_BANK43_FRAME121);
const struct sprite_frame gSpriteBank43Frame122 = SPRITE_FRAME(gSpriteBank43Frame122, SPRITE_TILES_BANK43_FRAME122);
const struct sprite_frame gSpriteBank43Frame123 = SPRITE_FRAME(gSpriteBank43Frame123, SPRITE_TILES_BANK43_FRAME123);
const struct sprite_frame gSpriteBank43Frame124 = SPRITE_FRAME(gSpriteBank43Frame124, SPRITE_TILES_BANK43_FRAME124);
const struct sprite_frame gSpriteBank43Frame125 = SPRITE_FRAME(gSpriteBank43Frame125, SPRITE_TILES_BANK43_FRAME125);
const struct sprite_frame gSpriteBank43Frame126 = SPRITE_FRAME(gSpriteBank43Frame126, SPRITE_TILES_BANK43_FRAME126);
const struct sprite_frame gSpriteBank43Frame127 = SPRITE_FRAME(gSpriteBank43Frame127, SPRITE_TILES_BANK43_FRAME127);
const struct sprite_frame gSpriteBank43Frame128 = SPRITE_FRAME(gSpriteBank43Frame128, SPRITE_TILES_BANK43_FRAME128);
const struct sprite_frame gSpriteBank43Frame129 = SPRITE_FRAME(gSpriteBank43Frame129, SPRITE_TILES_BANK43_FRAME129);
const struct sprite_frame gSpriteBank43Frame130 = SPRITE_FRAME(gSpriteBank43Frame130, SPRITE_TILES_BANK43_FRAME130);
const struct sprite_frame_1box gSpriteBank43Frame131 = {
    SPRITE_FRAME(gSpriteBank43Frame131, SPRITE_TILES_BANK43_FRAME131),
    { { -32, -25, 65, 51 } },
};
const struct sprite_frame_1box gSpriteBank43Frame132 = {
    SPRITE_FRAME(gSpriteBank43Frame132, SPRITE_TILES_BANK43_FRAME132),
    { { -32, -25, 65, 50 } },
};
const struct sprite_frame_1box gSpriteBank43Frame133 = {
    SPRITE_FRAME(gSpriteBank43Frame133, SPRITE_TILES_BANK43_FRAME133),
    { { -32, -24, 65, 50 } },
};
const struct sprite_frame_1box gSpriteBank43Frame134 = {
    SPRITE_FRAME(gSpriteBank43Frame134, SPRITE_TILES_BANK43_FRAME134),
    { { -32, -25, 65, 50 } },
};
const struct sprite_frame_1box gSpriteBank43Frame135 = {
    SPRITE_FRAME(gSpriteBank43Frame135, SPRITE_TILES_BANK43_FRAME135),
    { { -32, -25, 65, 51 } },
};
const struct sprite_frame_1box gSpriteBank43Frame136 = {
    SPRITE_FRAME(gSpriteBank43Frame136, SPRITE_TILES_BANK43_FRAME136),
    { { -32, -24, 65, 50 } },
};
const struct sprite_frame_1box gSpriteBank43Frame137 = {
    SPRITE_FRAME(gSpriteBank43Frame137, SPRITE_TILES_BANK43_FRAME137),
    { { -32, -25, 65, 51 } },
};
const struct sprite_frame_1box gSpriteBank43Frame138 = {
    SPRITE_FRAME(gSpriteBank43Frame138, SPRITE_TILES_BANK43_FRAME138),
    { { -32, -25, 65, 50 } },
};
const struct sprite_frame_1box gSpriteBank43Frame139 = {
    SPRITE_FRAME(gSpriteBank43Frame139, SPRITE_TILES_BANK43_FRAME139),
    { { -32, -24, 65, 50 } },
};
const struct sprite_frame gSpriteBank43Frame140 = SPRITE_FRAME(gSpriteBank43Frame140, SPRITE_TILES_BANK43_FRAME140);
const struct sprite_frame gSpriteBank43Frame141 = SPRITE_FRAME(gSpriteBank43Frame141, SPRITE_TILES_BANK43_FRAME141);
const struct sprite_frame gSpriteBank43Frame142 = SPRITE_FRAME(gSpriteBank43Frame142, SPRITE_TILES_BANK43_FRAME142);
const struct sprite_frame gSpriteBank43Frame143 = SPRITE_FRAME(gSpriteBank43Frame143, SPRITE_TILES_BANK43_FRAME143);
const struct sprite_frame gSpriteBank43Frame144 = SPRITE_FRAME(gSpriteBank43Frame144, SPRITE_TILES_BANK43_FRAME144);
const struct sprite_frame gSpriteBank43Frame145 = SPRITE_FRAME(gSpriteBank43Frame145, SPRITE_TILES_BANK43_FRAME145);
const struct sprite_frame gSpriteBank43Frame146 = SPRITE_FRAME(gSpriteBank43Frame146, SPRITE_TILES_BANK43_FRAME146);
const struct sprite_frame gSpriteBank43Frame147 = SPRITE_FRAME(gSpriteBank43Frame147, SPRITE_TILES_BANK43_FRAME147);
const struct sprite_frame gSpriteBank43Frame148 = SPRITE_FRAME(gSpriteBank43Frame148, SPRITE_TILES_BANK43_FRAME148);
const struct sprite_frame gSpriteBank43Frame149 = SPRITE_FRAME(gSpriteBank43Frame149, SPRITE_TILES_BANK43_FRAME149);
const struct sprite_frame gSpriteBank43Frame150 = SPRITE_FRAME(gSpriteBank43Frame150, SPRITE_TILES_BANK43_FRAME150);
const struct sprite_frame gSpriteBank43Frame151 = SPRITE_FRAME(gSpriteBank43Frame151, SPRITE_TILES_BANK43_FRAME151);
const struct sprite_frame gSpriteBank43Frame152 = SPRITE_FRAME(gSpriteBank43Frame152, SPRITE_TILES_BANK43_FRAME152);
const struct sprite_frame gSpriteBank43Frame153 = SPRITE_FRAME(gSpriteBank43Frame153, SPRITE_TILES_BANK43_FRAME153);
const struct sprite_frame gSpriteBank43Frame154 = SPRITE_FRAME(gSpriteBank43Frame154, SPRITE_TILES_BANK43_FRAME154);
const struct sprite_frame gSpriteBank43Frame155 = SPRITE_FRAME(gSpriteBank43Frame155, SPRITE_TILES_BANK43_FRAME155);
const struct sprite_frame gSpriteBank43Frame156 = SPRITE_FRAME(gSpriteBank43Frame156, SPRITE_TILES_BANK43_FRAME156);
const struct sprite_frame gSpriteBank43Frame157 = SPRITE_FRAME(gSpriteBank43Frame157, SPRITE_TILES_BANK43_FRAME157);
const struct sprite_frame gSpriteBank43Frame158 = SPRITE_FRAME(gSpriteBank43Frame158, SPRITE_TILES_BANK43_FRAME158);
const struct sprite_frame gSpriteBank43Frame159 = SPRITE_FRAME(gSpriteBank43Frame159, SPRITE_TILES_BANK43_FRAME159);
const struct sprite_frame gSpriteBank43Frame160 = SPRITE_FRAME(gSpriteBank43Frame160, SPRITE_TILES_BANK43_FRAME160);
const struct sprite_frame gSpriteBank43Frame161 = SPRITE_FRAME(gSpriteBank43Frame161, SPRITE_TILES_BANK43_FRAME161);
const struct sprite_frame gSpriteBank43Frame162 = SPRITE_FRAME(gSpriteBank43Frame162, SPRITE_TILES_BANK43_FRAME162);
const struct sprite_frame gSpriteBank43Frame163 = SPRITE_FRAME(gSpriteBank43Frame163, SPRITE_TILES_BANK43_FRAME163);
const struct sprite_frame gSpriteBank43Frame164 = SPRITE_FRAME(gSpriteBank43Frame164, SPRITE_TILES_BANK43_FRAME164);
const struct sprite_frame gSpriteBank43Frame165 = SPRITE_FRAME(gSpriteBank43Frame165, SPRITE_TILES_BANK43_FRAME165);
const struct sprite_frame gSpriteBank43Frame166 = SPRITE_FRAME(gSpriteBank43Frame166, SPRITE_TILES_BANK43_FRAME166);
const struct sprite_frame gSpriteBank43Frame167 = SPRITE_FRAME(gSpriteBank43Frame167, SPRITE_TILES_BANK43_FRAME167);
const struct sprite_frame gSpriteBank43Frame168 = SPRITE_FRAME(gSpriteBank43Frame168, SPRITE_TILES_BANK43_FRAME168);
const struct sprite_frame gSpriteBank43Frame169 = SPRITE_FRAME(gSpriteBank43Frame169, SPRITE_TILES_BANK43_FRAME169);
const struct sprite_frame gSpriteBank43Frame170 = SPRITE_FRAME(gSpriteBank43Frame170, SPRITE_TILES_BANK43_FRAME170);
const struct sprite_frame gSpriteBank43Frame171 = SPRITE_FRAME(gSpriteBank43Frame171, SPRITE_TILES_BANK43_FRAME171);
const struct sprite_frame gSpriteBank43Frame172 = SPRITE_FRAME(gSpriteBank43Frame172, SPRITE_TILES_BANK43_FRAME172);
const struct sprite_frame gSpriteBank43Frame173 = SPRITE_FRAME(gSpriteBank43Frame173, SPRITE_TILES_BANK43_FRAME173);
const struct sprite_frame gSpriteBank43Frame174 = SPRITE_FRAME(gSpriteBank43Frame174, SPRITE_TILES_BANK43_FRAME174);
const struct sprite_frame gSpriteBank43Frame175 = SPRITE_FRAME(gSpriteBank43Frame175, SPRITE_TILES_BANK43_FRAME175);
const struct sprite_frame gSpriteBank43Frame176 = SPRITE_FRAME(gSpriteBank43Frame176, SPRITE_TILES_BANK43_FRAME176);
const struct sprite_frame gSpriteBank43Frame177 = SPRITE_FRAME(gSpriteBank43Frame177, SPRITE_TILES_BANK43_FRAME177);
const struct sprite_frame gSpriteBank43Frame178 = SPRITE_FRAME(gSpriteBank43Frame178, SPRITE_TILES_BANK43_FRAME178);
const struct sprite_frame gSpriteBank43Frame179 = SPRITE_FRAME(gSpriteBank43Frame179, SPRITE_TILES_BANK43_FRAME179);
const struct sprite_frame gSpriteBank43Frame180 = SPRITE_FRAME(gSpriteBank43Frame180, SPRITE_TILES_BANK43_FRAME180);
const struct sprite_frame gSpriteBank43Frame181 = SPRITE_FRAME(gSpriteBank43Frame181, SPRITE_TILES_BANK43_FRAME181);
const struct sprite_frame gSpriteBank43Frame182 = SPRITE_FRAME(gSpriteBank43Frame182, SPRITE_TILES_BANK43_FRAME182);
const struct sprite_frame gSpriteBank43Frame183 = SPRITE_FRAME(gSpriteBank43Frame183, SPRITE_TILES_BANK43_FRAME183);
const struct sprite_frame gSpriteBank43Frame184 = SPRITE_FRAME(gSpriteBank43Frame184, SPRITE_TILES_BANK43_FRAME184);
const struct sprite_frame gSpriteBank43Frame185 = SPRITE_FRAME(gSpriteBank43Frame185, SPRITE_TILES_BANK43_FRAME185);
const struct sprite_frame gSpriteBank43Frame186 = SPRITE_FRAME(gSpriteBank43Frame186, SPRITE_TILES_BANK43_FRAME186);
const struct sprite_frame gSpriteBank43Frame187 = SPRITE_FRAME(gSpriteBank43Frame187, SPRITE_TILES_BANK43_FRAME187);
const struct sprite_frame gSpriteBank43Frame188 = SPRITE_FRAME(gSpriteBank43Frame188, SPRITE_TILES_BANK43_FRAME188);
const struct sprite_frame gSpriteBank43Frame189 = SPRITE_FRAME(gSpriteBank43Frame189, SPRITE_TILES_BANK43_FRAME189);
const struct sprite_frame gSpriteBank43Frame190 = SPRITE_FRAME(gSpriteBank43Frame190, SPRITE_TILES_BANK43_FRAME190);
const struct sprite_frame gSpriteBank43Frame191 = SPRITE_FRAME(gSpriteBank43Frame191, SPRITE_TILES_BANK43_FRAME191);
const struct sprite_frame gSpriteBank43Frame192 = SPRITE_FRAME(gSpriteBank43Frame192, SPRITE_TILES_BANK43_FRAME192);
const struct sprite_frame gSpriteBank43Frame193 = SPRITE_FRAME(gSpriteBank43Frame193, SPRITE_TILES_BANK43_FRAME193);
const struct sprite_frame gSpriteBank43Frame194 = SPRITE_FRAME(gSpriteBank43Frame194, SPRITE_TILES_BANK43_FRAME194);
const struct sprite_frame gSpriteBank43Frame195 = SPRITE_FRAME(gSpriteBank43Frame195, SPRITE_TILES_BANK43_FRAME195);
const struct sprite_frame gSpriteBank43Frame196 = SPRITE_FRAME(gSpriteBank43Frame196, SPRITE_TILES_BANK43_FRAME196);
const struct sprite_frame gSpriteBank43Frame197 = SPRITE_FRAME(gSpriteBank43Frame197, SPRITE_TILES_BANK43_FRAME197);
const struct sprite_frame gSpriteBank43Frame198 = SPRITE_FRAME(gSpriteBank43Frame198, SPRITE_TILES_BANK43_FRAME198);
const struct sprite_frame gSpriteBank43Frame199 = SPRITE_FRAME(gSpriteBank43Frame199, SPRITE_TILES_BANK43_FRAME199);
const struct sprite_frame gSpriteBank43Frame200 = SPRITE_FRAME(gSpriteBank43Frame200, SPRITE_TILES_BANK43_FRAME200);
const struct sprite_frame gSpriteBank43Frame201 = SPRITE_FRAME(gSpriteBank43Frame201, SPRITE_TILES_BANK43_FRAME201);
const struct sprite_frame gSpriteBank43Frame202 = SPRITE_FRAME(gSpriteBank43Frame202, SPRITE_TILES_BANK43_FRAME202);
const struct sprite_frame gSpriteBank43Frame203 = SPRITE_FRAME(gSpriteBank43Frame203, SPRITE_TILES_BANK43_FRAME203);
const struct sprite_frame gSpriteBank43Frame204 = SPRITE_FRAME(gSpriteBank43Frame204, SPRITE_TILES_BANK43_FRAME204);
const struct sprite_frame gSpriteBank43Frame205 = SPRITE_FRAME(gSpriteBank43Frame205, SPRITE_TILES_BANK43_FRAME205);
const struct sprite_frame gSpriteBank43Frame206 = SPRITE_FRAME(gSpriteBank43Frame206, SPRITE_TILES_BANK43_FRAME206);
const struct sprite_frame_1box gSpriteBank43Frame207 = {
    SPRITE_FRAME(gSpriteBank43Frame207, SPRITE_TILES_BANK43_FRAME207),
    { { -6, -9, 13, 19 } },
};
const struct sprite_frame_1box gSpriteBank43Frame208 = {
    SPRITE_FRAME(gSpriteBank43Frame208, SPRITE_TILES_BANK43_FRAME208),
    { { -7, -9, 14, 19 } },
};
const struct sprite_frame_1box gSpriteBank43Frame209 = {
    SPRITE_FRAME(gSpriteBank43Frame209, SPRITE_TILES_BANK43_FRAME209),
    { { -5, -9, 11, 19 } },
};
const struct sprite_frame_1box gSpriteBank43Frame210 = {
    SPRITE_FRAME(gSpriteBank43Frame210, SPRITE_TILES_BANK43_FRAME210),
    { { -5, -9, 12, 19 } },
};
const struct sprite_frame_1box gSpriteBank43Frame211 = {
    SPRITE_FRAME(gSpriteBank43Frame211, SPRITE_TILES_BANK43_FRAME211),
    { { -6, -9, 14, 19 } },
};
const struct sprite_frame gSpriteBank43Frame212 = SPRITE_FRAME(gSpriteBank43Frame212, SPRITE_TILES_BANK43_FRAME212);
const struct sprite_frame gSpriteBank43Frame213 = SPRITE_FRAME(gSpriteBank43Frame213, SPRITE_TILES_BANK43_FRAME213);
const struct sprite_frame gSpriteBank43Frame214 = SPRITE_FRAME(gSpriteBank43Frame214, SPRITE_TILES_BANK43_FRAME214);
const struct sprite_frame gSpriteBank43Frame215 = SPRITE_FRAME(gSpriteBank43Frame215, SPRITE_TILES_BANK43_FRAME215);
const struct sprite_frame gSpriteBank43Frame216 = SPRITE_FRAME(gSpriteBank43Frame216, SPRITE_TILES_BANK43_FRAME216);
const struct sprite_frame gSpriteBank43Frame217 = SPRITE_FRAME(gSpriteBank43Frame217, SPRITE_TILES_BANK43_FRAME217);
const struct sprite_frame gSpriteBank43Frame218 = SPRITE_FRAME(gSpriteBank43Frame218, SPRITE_TILES_BANK43_FRAME218);
const struct sprite_frame_1box gSpriteBank43Frame219 = {
    SPRITE_FRAME(gSpriteBank43Frame219, SPRITE_TILES_BANK43_FRAME219),
    { { -5, -18, 13, 189 } },
};
const struct sprite_frame_1box gSpriteBank43Frame220 = {
    SPRITE_FRAME(gSpriteBank43Frame220, SPRITE_TILES_BANK43_FRAME220),
    { { -5, -18, 13, 189 } },
};
const struct sprite_frame_1box gSpriteBank43Frame221 = {
    SPRITE_FRAME(gSpriteBank43Frame221, SPRITE_TILES_BANK43_FRAME221),
    { { -5, -18, 13, 189 } },
};
const struct sprite_frame_1box gSpriteBank43Frame222 = {
    SPRITE_FRAME(gSpriteBank43Frame222, SPRITE_TILES_BANK43_FRAME222),
    { { -5, -18, 13, 189 } },
};
const struct sprite_frame_1box gSpriteBank43Frame223 = {
    SPRITE_FRAME(gSpriteBank43Frame223, SPRITE_TILES_BANK43_FRAME223),
    { { -5, -18, 13, 189 } },
};
const struct sprite_frame gSpriteBank43Frame224 = SPRITE_FRAME(gSpriteBank43Frame224, SPRITE_TILES_BANK43_FRAME224);
const struct sprite_frame gSpriteBank43Frame225 = SPRITE_FRAME(gSpriteBank43Frame225, SPRITE_TILES_BANK43_FRAME225);
const struct sprite_frame gSpriteBank43Frame226 = SPRITE_FRAME(gSpriteBank43Frame226, SPRITE_TILES_BANK43_FRAME226);
const struct sprite_frame gSpriteBank43Frame227 = SPRITE_FRAME(gSpriteBank43Frame227, SPRITE_TILES_BANK43_FRAME227);
const struct sprite_frame gSpriteBank43Frame228 = SPRITE_FRAME(gSpriteBank43Frame228, SPRITE_TILES_BANK43_FRAME228);
const struct sprite_frame gSpriteBank43Frame229 = SPRITE_FRAME(gSpriteBank43Frame229, SPRITE_TILES_BANK43_FRAME229);
const struct sprite_frame gSpriteBank43Frame230 = SPRITE_FRAME(gSpriteBank43Frame230, SPRITE_TILES_BANK43_FRAME230);
const struct sprite_frame gSpriteBank43Frame231 = SPRITE_FRAME(gSpriteBank43Frame231, SPRITE_TILES_BANK43_FRAME231);
const struct sprite_frame gSpriteBank43Frame232 = SPRITE_FRAME(gSpriteBank43Frame232, SPRITE_TILES_BANK43_FRAME232);
const struct sprite_frame gSpriteBank43Frame233 = SPRITE_FRAME(gSpriteBank43Frame233, SPRITE_TILES_BANK43_FRAME233);

/* The piece arrays, from games/n2/graphics/sprites/bank43_evil_coco/bank.json. */
#include "sprites/bank43_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 44: 23 animations, 190 frames, games/n2/graphics/sprites/bank44_n_trance/. */

extern const u16 gSpriteBank44Anim00Seq[9];
extern const u16 gSpriteBank44Anim01Seq[4];
extern const u16 gSpriteBank44Anim02Seq[4];
extern const u16 gSpriteBank44Anim03Seq[9];
extern const u16 gSpriteBank44Anim04Seq[9];
extern const u16 gSpriteBank44Anim05Seq[5];
extern const u16 gSpriteBank44Anim06Seq[12];
extern const u16 gSpriteBank44Anim07Seq[23];
extern const u16 gSpriteBank44Anim08Seq[9];
extern const u16 gSpriteBank44Anim09Seq[7];
extern const u16 gSpriteBank44Anim10Seq[16];
extern const u16 gSpriteBank44Anim11Seq[10];
extern const u16 gSpriteBank44Anim12Seq[12];
extern const u16 gSpriteBank44Anim13Seq[12];
extern const u16 gSpriteBank44Anim14Seq[11];
extern const u16 gSpriteBank44Anim15Seq[10];
extern const u16 gSpriteBank44Anim16Seq[1];
extern const u16 gSpriteBank44Anim17Seq[1];
extern const u16 gSpriteBank44Anim18Seq[1];
extern const u16 gSpriteBank44Anim19Seq[10];
extern const u16 gSpriteBank44Anim20Seq[10];
extern const u16 gSpriteBank44Anim21Seq[6];
extern const u16 gSpriteBank44Anim22Seq[17];

extern const struct sprite_anim gSpriteBank44Anims[23] = {
    /* 0 */ {
        /* seq */ gSpriteBank44Anim00Seq,
        /* box */ { { -24, -20, 37, 41 }, { -21, 11, 33, 30 } },
        /* paletteId */ 79,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank44Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank44Anim01Seq,
        /* box */ { { -7, -7, 15, 15 }, { -7, -7, 15, 15 } },
        /* paletteId */ 80,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank44Anim01Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 2 */ {
        /* seq */ gSpriteBank44Anim02Seq,
        /* box */ { { -14, -14, 29, 29 }, { -14, -14, 30, 30 } },
        /* paletteId */ 81,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank44Anim02Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 3 */ {
        /* seq */ gSpriteBank44Anim03Seq,
        /* box */ { { -24, -20, 37, 41 }, { -37, -35, 61, 39 } },
        /* paletteId */ 82,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank44Anim03Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 4 */ {
        /* seq */ gSpriteBank44Anim04Seq,
        /* box */ { { -24, -20, 37, 41 }, { -37, -35, 61, 39 } },
        /* paletteId */ 83,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank44Anim04Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 5 */ {
        /* seq */ gSpriteBank44Anim05Seq,
        /* box */ { { -24, -20, 37, 41 }, { -25, -21, 38, 42 } },
        /* paletteId */ 84,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank44Anim05Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 6 */ {
        /* seq */ gSpriteBank44Anim06Seq,
        /* box */ { { -24, -20, 37, 41 }, { -26, -23, 40, 45 } },
        /* paletteId */ 84,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank44Anim06Seq),
        /* flags */ 0,
    },
    /* 7 */ {
        /* seq */ gSpriteBank44Anim07Seq,
        /* box */ { { -24, -20, 37, 41 }, { -26, -23, 40, 45 } },
        /* paletteId */ 84,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank44Anim07Seq),
        /* flags */ 0,
    },
    /* 8 */ {
        /* seq */ gSpriteBank44Anim08Seq,
        /* box */ { { -24, -20, 37, 41 }, { -24, -22, 37, 43 } },
        /* paletteId */ 84,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank44Anim08Seq),
        /* flags */ 0,
    },
    /* 9 */ {
        /* seq */ gSpriteBank44Anim09Seq,
        /* box */ { { -25, -33, 33, 66 }, { -34, -35, 76, 69 } },
        /* paletteId */ 84,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank44Anim09Seq),
        /* flags */ 0,
    },
    /* 10 */ {
        /* seq */ gSpriteBank44Anim10Seq,
        /* box */ { { -25, -33, 33, 66 }, { -40, -35, 95, 70 } },
        /* paletteId */ 84,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank44Anim10Seq),
        /* flags */ 0,
    },
    /* 11 */ {
        /* seq */ gSpriteBank44Anim11Seq,
        /* box */ { { -25, -33, 33, 66 }, { -34, -35, 82, 69 } },
        /* paletteId */ 84,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank44Anim11Seq),
        /* flags */ 0,
    },
    /* 12 */ {
        /* seq */ gSpriteBank44Anim12Seq,
        /* box */ { { -25, -33, 33, 66 }, { -34, -35, 69, 69 } },
        /* paletteId */ 84,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank44Anim12Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 13 */ {
        /* seq */ gSpriteBank44Anim13Seq,
        /* box */ { { -25, -33, 33, 66 }, { -40, -47, 79, 81 } },
        /* paletteId */ 84,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank44Anim13Seq),
        /* flags */ 0,
    },
    /* 14 */ {
        /* seq */ gSpriteBank44Anim14Seq,
        /* box */ { { -25, -33, 33, 66 }, { -39, -50, 93, 84 } },
        /* paletteId */ 84,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank44Anim14Seq),
        /* flags */ 0,
    },
    /* 15 */ {
        /* seq */ gSpriteBank44Anim15Seq,
        /* box */ { { -25, -33, 33, 66 }, { -56, -92, 111, 113 } },
        /* paletteId */ 84,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank44Anim15Seq),
        /* flags */ 0,
    },
    /* 16 */ {
        /* seq */ gSpriteBank44Anim16Seq,
        /* box */ { { -2, -2, 5, 4 }, { -2, -2, 5, 4 } },
        /* paletteId */ 84,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank44Anim16Seq),
        /* flags */ 0,
    },
    /* 17 */ {
        /* seq */ gSpriteBank44Anim17Seq,
        /* box */ { { -5, -3, 10, 7 }, { -6, -3, 10, 7 } },
        /* paletteId */ 84,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank44Anim17Seq),
        /* flags */ 0,
    },
    /* 18 */ {
        /* seq */ gSpriteBank44Anim18Seq,
        /* box */ { { -10, -10, 21, 20 }, { -10, -10, 21, 20 } },
        /* paletteId */ 84,
        /* duration */ 0,
        /* frameCount */ ARRAY_COUNT(gSpriteBank44Anim18Seq),
        /* flags */ 0,
    },
    /* 19 */ {
        /* seq */ gSpriteBank44Anim19Seq,
        /* box */ { { -25, -33, 33, 66 }, { -38, -40, 73, 75 } },
        /* paletteId */ 84,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank44Anim19Seq),
        /* flags */ 0,
    },
    /* 20 */ {
        /* seq */ gSpriteBank44Anim20Seq,
        /* box */ { { -25, -33, 33, 66 }, { -39, -52, 83, 87 } },
        /* paletteId */ 84,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank44Anim20Seq),
        /* flags */ 0,
    },
    /* 21 */ {
        /* seq */ gSpriteBank44Anim21Seq,
        /* box */ { { -25, -33, 33, 66 }, { -32, -31, 66, 66 } },
        /* paletteId */ 84,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank44Anim21Seq),
        /* flags */ 0,
    },
    /* 22 */ {
        /* seq */ gSpriteBank44Anim22Seq,
        /* box */ { { -25, -33, 33, 66 }, { -35, -35, 81, 69 } },
        /* paletteId */ 84,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank44Anim22Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank44Anim00Seq[9] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8,
};
const u16 gSpriteBank44Anim01Seq[4] = {
    9, 10, 11, 12,
};
const u16 gSpriteBank44Anim02Seq[4] = {
    13, 14, 15, 16,
};
const u16 gSpriteBank44Anim03Seq[9] = {
    17, 18, 19, 20, 21, 22, 23, 24, 25,
};
const u16 gSpriteBank44Anim04Seq[9] = {
    17, 18, 19, 20, 21, 22, 23, 24, 25,
};
const u16 gSpriteBank44Anim05Seq[5] = {
    26, 27, 28, 29, 30,
};
const u16 gSpriteBank44Anim06Seq[12] = {
    31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
};
const u16 gSpriteBank44Anim07Seq[23] = {
    43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 55, 56, 55,
    57, 58, 58, 58, 58, 58, 58,
};
const u16 gSpriteBank44Anim08Seq[9] = {
    59, 60, 61, 62, 63, 64, 65, 66, 67,
};
const u16 gSpriteBank44Anim09Seq[7] = {
    68, 69, 70, 71, 72, 73, 74,
};
const u16 gSpriteBank44Anim10Seq[16] = {
    75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90,
};
const u16 gSpriteBank44Anim11Seq[10] = {
    91, 92, 93, 94, 95, 96, 97, 98, 99, 100,
};
const u16 gSpriteBank44Anim12Seq[12] = {
    101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112,
};
const u16 gSpriteBank44Anim13Seq[12] = {
    113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124,
};
const u16 gSpriteBank44Anim14Seq[11] = {
    125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
};
const u16 gSpriteBank44Anim15Seq[10] = {
    136, 137, 138, 139, 140, 141, 142, 143, 143, 143,
};
const u16 gSpriteBank44Anim16Seq[1] = {
    144,
};
const u16 gSpriteBank44Anim17Seq[1] = {
    145,
};
const u16 gSpriteBank44Anim18Seq[1] = {
    146,
};
const u16 gSpriteBank44Anim19Seq[10] = {
    147, 148, 149, 150, 151, 152, 153, 154, 155, 156,
};
const u16 gSpriteBank44Anim20Seq[10] = {
    157, 158, 159, 160, 161, 162, 163, 164, 165, 166,
};
const u16 gSpriteBank44Anim21Seq[6] = {
    167, 168, 169, 170, 171, 172,
};
const u16 gSpriteBank44Anim22Seq[17] = {
    173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188,
    189,
};

extern const struct sprite_frame *const gSpriteBank44Frames[190] = {
    &gSpriteBank44Frame000,
    &gSpriteBank44Frame001,
    &gSpriteBank44Frame002,
    &gSpriteBank44Frame003,
    &gSpriteBank44Frame004,
    &gSpriteBank44Frame005,
    &gSpriteBank44Frame006,
    &gSpriteBank44Frame007,
    &gSpriteBank44Frame008,
    &gSpriteBank44Frame009.frame,
    &gSpriteBank44Frame010.frame,
    &gSpriteBank44Frame011.frame,
    &gSpriteBank44Frame012.frame,
    &gSpriteBank44Frame013.frame,
    &gSpriteBank44Frame014.frame,
    &gSpriteBank44Frame015.frame,
    &gSpriteBank44Frame016.frame,
    &gSpriteBank44Frame017,
    &gSpriteBank44Frame018,
    &gSpriteBank44Frame019,
    &gSpriteBank44Frame020,
    &gSpriteBank44Frame021,
    &gSpriteBank44Frame022,
    &gSpriteBank44Frame023,
    &gSpriteBank44Frame024,
    &gSpriteBank44Frame025,
    &gSpriteBank44Frame026.frame,
    &gSpriteBank44Frame027.frame,
    &gSpriteBank44Frame028.frame,
    &gSpriteBank44Frame029.frame,
    &gSpriteBank44Frame030.frame,
    &gSpriteBank44Frame031,
    &gSpriteBank44Frame032,
    &gSpriteBank44Frame033,
    &gSpriteBank44Frame034,
    &gSpriteBank44Frame035,
    &gSpriteBank44Frame036,
    &gSpriteBank44Frame037,
    &gSpriteBank44Frame038,
    &gSpriteBank44Frame039,
    &gSpriteBank44Frame040,
    &gSpriteBank44Frame041,
    &gSpriteBank44Frame042,
    &gSpriteBank44Frame043,
    &gSpriteBank44Frame044,
    &gSpriteBank44Frame045,
    &gSpriteBank44Frame046,
    &gSpriteBank44Frame047,
    &gSpriteBank44Frame048,
    &gSpriteBank44Frame049,
    &gSpriteBank44Frame050,
    &gSpriteBank44Frame051,
    &gSpriteBank44Frame052,
    &gSpriteBank44Frame053,
    &gSpriteBank44Frame054,
    &gSpriteBank44Frame055,
    &gSpriteBank44Frame056,
    &gSpriteBank44Frame057,
    &gSpriteBank44Frame058,
    &gSpriteBank44Frame059.frame,
    &gSpriteBank44Frame060.frame,
    &gSpriteBank44Frame061.frame,
    &gSpriteBank44Frame062.frame,
    &gSpriteBank44Frame063.frame,
    &gSpriteBank44Frame064.frame,
    &gSpriteBank44Frame065.frame,
    &gSpriteBank44Frame066.frame,
    &gSpriteBank44Frame067.frame,
    &gSpriteBank44Frame068.frame,
    &gSpriteBank44Frame069.frame,
    &gSpriteBank44Frame070.frame,
    &gSpriteBank44Frame071.frame,
    &gSpriteBank44Frame072.frame,
    &gSpriteBank44Frame073.frame,
    &gSpriteBank44Frame074.frame,
    &gSpriteBank44Frame075.frame,
    &gSpriteBank44Frame076.frame,
    &gSpriteBank44Frame077.frame,
    &gSpriteBank44Frame078.frame,
    &gSpriteBank44Frame079.frame,
    &gSpriteBank44Frame080.frame,
    &gSpriteBank44Frame081.frame,
    &gSpriteBank44Frame082.frame,
    &gSpriteBank44Frame083.frame,
    &gSpriteBank44Frame084.frame,
    &gSpriteBank44Frame085.frame,
    &gSpriteBank44Frame086.frame,
    &gSpriteBank44Frame087.frame,
    &gSpriteBank44Frame088.frame,
    &gSpriteBank44Frame089.frame,
    &gSpriteBank44Frame090.frame,
    &gSpriteBank44Frame091.frame,
    &gSpriteBank44Frame092.frame,
    &gSpriteBank44Frame093.frame,
    &gSpriteBank44Frame094.frame,
    &gSpriteBank44Frame095.frame,
    &gSpriteBank44Frame096.frame,
    &gSpriteBank44Frame097.frame,
    &gSpriteBank44Frame098.frame,
    &gSpriteBank44Frame099.frame,
    &gSpriteBank44Frame100.frame,
    &gSpriteBank44Frame101.frame,
    &gSpriteBank44Frame102.frame,
    &gSpriteBank44Frame103.frame,
    &gSpriteBank44Frame104.frame,
    &gSpriteBank44Frame105.frame,
    &gSpriteBank44Frame106.frame,
    &gSpriteBank44Frame107.frame,
    &gSpriteBank44Frame108.frame,
    &gSpriteBank44Frame109.frame,
    &gSpriteBank44Frame110.frame,
    &gSpriteBank44Frame111.frame,
    &gSpriteBank44Frame112.frame,
    &gSpriteBank44Frame113.frame,
    &gSpriteBank44Frame114.frame,
    &gSpriteBank44Frame115.frame,
    &gSpriteBank44Frame116.frame,
    &gSpriteBank44Frame117.frame,
    &gSpriteBank44Frame118.frame,
    &gSpriteBank44Frame119.frame,
    &gSpriteBank44Frame120.frame,
    &gSpriteBank44Frame121.frame,
    &gSpriteBank44Frame122.frame,
    &gSpriteBank44Frame123.frame,
    &gSpriteBank44Frame124.frame,
    &gSpriteBank44Frame125,
    &gSpriteBank44Frame126,
    &gSpriteBank44Frame127,
    &gSpriteBank44Frame128,
    &gSpriteBank44Frame129,
    &gSpriteBank44Frame130,
    &gSpriteBank44Frame131,
    &gSpriteBank44Frame132,
    &gSpriteBank44Frame133,
    &gSpriteBank44Frame134,
    &gSpriteBank44Frame135.frame,
    &gSpriteBank44Frame136,
    &gSpriteBank44Frame137,
    &gSpriteBank44Frame138,
    &gSpriteBank44Frame139,
    &gSpriteBank44Frame140,
    &gSpriteBank44Frame141,
    &gSpriteBank44Frame142,
    &gSpriteBank44Frame143,
    &gSpriteBank44Frame144,
    &gSpriteBank44Frame145,
    &gSpriteBank44Frame146.frame,
    &gSpriteBank44Frame147.frame,
    &gSpriteBank44Frame148.frame,
    &gSpriteBank44Frame149.frame,
    &gSpriteBank44Frame150.frame,
    &gSpriteBank44Frame151.frame,
    &gSpriteBank44Frame152.frame,
    &gSpriteBank44Frame153.frame,
    &gSpriteBank44Frame154.frame,
    &gSpriteBank44Frame155.frame,
    &gSpriteBank44Frame156.frame,
    &gSpriteBank44Frame157.frame,
    &gSpriteBank44Frame158.frame,
    &gSpriteBank44Frame159.frame,
    &gSpriteBank44Frame160.frame,
    &gSpriteBank44Frame161.frame,
    &gSpriteBank44Frame162.frame,
    &gSpriteBank44Frame163.frame,
    &gSpriteBank44Frame164.frame,
    &gSpriteBank44Frame165.frame,
    &gSpriteBank44Frame166.frame,
    &gSpriteBank44Frame167.frame,
    &gSpriteBank44Frame168.frame,
    &gSpriteBank44Frame169.frame,
    &gSpriteBank44Frame170.frame,
    &gSpriteBank44Frame171.frame,
    &gSpriteBank44Frame172.frame,
    &gSpriteBank44Frame173.frame,
    &gSpriteBank44Frame174.frame,
    &gSpriteBank44Frame175.frame,
    &gSpriteBank44Frame176.frame,
    &gSpriteBank44Frame177.frame,
    &gSpriteBank44Frame178.frame,
    &gSpriteBank44Frame179.frame,
    &gSpriteBank44Frame180.frame,
    &gSpriteBank44Frame181.frame,
    &gSpriteBank44Frame182.frame,
    &gSpriteBank44Frame183.frame,
    &gSpriteBank44Frame184.frame,
    &gSpriteBank44Frame185.frame,
    &gSpriteBank44Frame186.frame,
    &gSpriteBank44Frame187.frame,
    &gSpriteBank44Frame188.frame,
    &gSpriteBank44Frame189.frame,
};

const struct sprite_frame gSpriteBank44Frame000 = SPRITE_FRAME(gSpriteBank44Frame000, SPRITE_TILES_BANK44_FRAME000);
const struct sprite_frame gSpriteBank44Frame001 = SPRITE_FRAME(gSpriteBank44Frame001, SPRITE_TILES_BANK44_FRAME001);
const struct sprite_frame gSpriteBank44Frame002 = SPRITE_FRAME(gSpriteBank44Frame002, SPRITE_TILES_BANK44_FRAME002);
const struct sprite_frame gSpriteBank44Frame003 = SPRITE_FRAME(gSpriteBank44Frame003, SPRITE_TILES_BANK44_FRAME003);
const struct sprite_frame gSpriteBank44Frame004 = SPRITE_FRAME(gSpriteBank44Frame004, SPRITE_TILES_BANK44_FRAME004);
const struct sprite_frame gSpriteBank44Frame005 = SPRITE_FRAME(gSpriteBank44Frame005, SPRITE_TILES_BANK44_FRAME005);
const struct sprite_frame gSpriteBank44Frame006 = SPRITE_FRAME(gSpriteBank44Frame006, SPRITE_TILES_BANK44_FRAME006);
const struct sprite_frame gSpriteBank44Frame007 = SPRITE_FRAME(gSpriteBank44Frame007, SPRITE_TILES_BANK44_FRAME007);
const struct sprite_frame gSpriteBank44Frame008 = SPRITE_FRAME(gSpriteBank44Frame008, SPRITE_TILES_BANK44_FRAME008);
const struct sprite_frame_1box gSpriteBank44Frame009 = {
    SPRITE_FRAME(gSpriteBank44Frame009, SPRITE_TILES_BANK44_FRAME009),
    { { -4, -4, 11, 11 } },
};
const struct sprite_frame_1box gSpriteBank44Frame010 = {
    SPRITE_FRAME(gSpriteBank44Frame010, SPRITE_TILES_BANK44_FRAME010),
    { { -4, -4, 11, 11 } },
};
const struct sprite_frame_1box gSpriteBank44Frame011 = {
    SPRITE_FRAME(gSpriteBank44Frame011, SPRITE_TILES_BANK44_FRAME011),
    { { -4, -4, 11, 11 } },
};
const struct sprite_frame_1box gSpriteBank44Frame012 = {
    SPRITE_FRAME(gSpriteBank44Frame012, SPRITE_TILES_BANK44_FRAME012),
    { { -4, -4, 11, 11 } },
};
const struct sprite_frame_1box gSpriteBank44Frame013 = {
    SPRITE_FRAME(gSpriteBank44Frame013, SPRITE_TILES_BANK44_FRAME013),
    { { -9, -8, 20, 18 } },
};
const struct sprite_frame_1box gSpriteBank44Frame014 = {
    SPRITE_FRAME(gSpriteBank44Frame014, SPRITE_TILES_BANK44_FRAME014),
    { { -9, -8, 20, 18 } },
};
const struct sprite_frame_1box gSpriteBank44Frame015 = {
    SPRITE_FRAME(gSpriteBank44Frame015, SPRITE_TILES_BANK44_FRAME015),
    { { -9, -8, 20, 18 } },
};
const struct sprite_frame_1box gSpriteBank44Frame016 = {
    SPRITE_FRAME(gSpriteBank44Frame016, SPRITE_TILES_BANK44_FRAME016),
    { { -9, -8, 20, 18 } },
};
const struct sprite_frame gSpriteBank44Frame017 = SPRITE_FRAME(gSpriteBank44Frame017, SPRITE_TILES_BANK44_FRAME017);
const struct sprite_frame gSpriteBank44Frame018 = SPRITE_FRAME(gSpriteBank44Frame018, SPRITE_TILES_BANK44_FRAME018);
const struct sprite_frame gSpriteBank44Frame019 = SPRITE_FRAME(gSpriteBank44Frame019, SPRITE_TILES_BANK44_FRAME019);
const struct sprite_frame gSpriteBank44Frame020 = SPRITE_FRAME(gSpriteBank44Frame020, SPRITE_TILES_BANK44_FRAME020);
const struct sprite_frame gSpriteBank44Frame021 = SPRITE_FRAME(gSpriteBank44Frame021, SPRITE_TILES_BANK44_FRAME021);
const struct sprite_frame gSpriteBank44Frame022 = SPRITE_FRAME(gSpriteBank44Frame022, SPRITE_TILES_BANK44_FRAME022);
const struct sprite_frame gSpriteBank44Frame023 = SPRITE_FRAME(gSpriteBank44Frame023, SPRITE_TILES_BANK44_FRAME023);
const struct sprite_frame gSpriteBank44Frame024 = SPRITE_FRAME(gSpriteBank44Frame024, SPRITE_TILES_BANK44_FRAME024);
const struct sprite_frame gSpriteBank44Frame025 = SPRITE_FRAME(gSpriteBank44Frame025, SPRITE_TILES_BANK44_FRAME025);
const struct sprite_frame_1box gSpriteBank44Frame026 = {
    SPRITE_FRAME(gSpriteBank44Frame026, SPRITE_TILES_BANK44_FRAME026),
    { { -19, -18, 30, 40 } },
};
const struct sprite_frame_1box gSpriteBank44Frame027 = {
    SPRITE_FRAME(gSpriteBank44Frame027, SPRITE_TILES_BANK44_FRAME027),
    { { -19, -18, 30, 40 } },
};
const struct sprite_frame_1box gSpriteBank44Frame028 = {
    SPRITE_FRAME(gSpriteBank44Frame028, SPRITE_TILES_BANK44_FRAME028),
    { { -19, -18, 30, 40 } },
};
const struct sprite_frame_1box gSpriteBank44Frame029 = {
    SPRITE_FRAME(gSpriteBank44Frame029, SPRITE_TILES_BANK44_FRAME029),
    { { -19, -18, 30, 40 } },
};
const struct sprite_frame_1box gSpriteBank44Frame030 = {
    SPRITE_FRAME(gSpriteBank44Frame030, SPRITE_TILES_BANK44_FRAME030),
    { { -19, -18, 30, 40 } },
};
const struct sprite_frame gSpriteBank44Frame031 = SPRITE_FRAME(gSpriteBank44Frame031, SPRITE_TILES_BANK44_FRAME031);
const struct sprite_frame gSpriteBank44Frame032 = SPRITE_FRAME(gSpriteBank44Frame032, SPRITE_TILES_BANK44_FRAME032);
const struct sprite_frame gSpriteBank44Frame033 = SPRITE_FRAME(gSpriteBank44Frame033, SPRITE_TILES_BANK44_FRAME033);
const struct sprite_frame gSpriteBank44Frame034 = SPRITE_FRAME(gSpriteBank44Frame034, SPRITE_TILES_BANK44_FRAME034);
const struct sprite_frame gSpriteBank44Frame035 = SPRITE_FRAME(gSpriteBank44Frame035, SPRITE_TILES_BANK44_FRAME035);
const struct sprite_frame gSpriteBank44Frame036 = SPRITE_FRAME(gSpriteBank44Frame036, SPRITE_TILES_BANK44_FRAME036);
const struct sprite_frame gSpriteBank44Frame037 = SPRITE_FRAME(gSpriteBank44Frame037, SPRITE_TILES_BANK44_FRAME037);
const struct sprite_frame gSpriteBank44Frame038 = SPRITE_FRAME(gSpriteBank44Frame038, SPRITE_TILES_BANK44_FRAME038);
const struct sprite_frame gSpriteBank44Frame039 = SPRITE_FRAME(gSpriteBank44Frame039, SPRITE_TILES_BANK44_FRAME039);
const struct sprite_frame gSpriteBank44Frame040 = SPRITE_FRAME(gSpriteBank44Frame040, SPRITE_TILES_BANK44_FRAME040);
const struct sprite_frame gSpriteBank44Frame041 = SPRITE_FRAME(gSpriteBank44Frame041, SPRITE_TILES_BANK44_FRAME041);
const struct sprite_frame gSpriteBank44Frame042 = SPRITE_FRAME(gSpriteBank44Frame042, SPRITE_TILES_BANK44_FRAME042);
const struct sprite_frame gSpriteBank44Frame043 = SPRITE_FRAME(gSpriteBank44Frame043, SPRITE_TILES_BANK44_FRAME043);
const struct sprite_frame gSpriteBank44Frame044 = SPRITE_FRAME(gSpriteBank44Frame044, SPRITE_TILES_BANK44_FRAME044);
const struct sprite_frame gSpriteBank44Frame045 = SPRITE_FRAME(gSpriteBank44Frame045, SPRITE_TILES_BANK44_FRAME045);
const struct sprite_frame gSpriteBank44Frame046 = SPRITE_FRAME(gSpriteBank44Frame046, SPRITE_TILES_BANK44_FRAME046);
const struct sprite_frame gSpriteBank44Frame047 = SPRITE_FRAME(gSpriteBank44Frame047, SPRITE_TILES_BANK44_FRAME047);
const struct sprite_frame gSpriteBank44Frame048 = SPRITE_FRAME(gSpriteBank44Frame048, SPRITE_TILES_BANK44_FRAME048);
const struct sprite_frame gSpriteBank44Frame049 = SPRITE_FRAME(gSpriteBank44Frame049, SPRITE_TILES_BANK44_FRAME049);
const struct sprite_frame gSpriteBank44Frame050 = SPRITE_FRAME(gSpriteBank44Frame050, SPRITE_TILES_BANK44_FRAME050);
const struct sprite_frame gSpriteBank44Frame051 = SPRITE_FRAME(gSpriteBank44Frame051, SPRITE_TILES_BANK44_FRAME051);
const struct sprite_frame gSpriteBank44Frame052 = SPRITE_FRAME(gSpriteBank44Frame052, SPRITE_TILES_BANK44_FRAME052);
const struct sprite_frame gSpriteBank44Frame053 = SPRITE_FRAME(gSpriteBank44Frame053, SPRITE_TILES_BANK44_FRAME053);
const struct sprite_frame gSpriteBank44Frame054 = SPRITE_FRAME(gSpriteBank44Frame054, SPRITE_TILES_BANK44_FRAME054);
const struct sprite_frame gSpriteBank44Frame055 = SPRITE_FRAME(gSpriteBank44Frame055, SPRITE_TILES_BANK44_FRAME055);
const struct sprite_frame gSpriteBank44Frame056 = SPRITE_FRAME(gSpriteBank44Frame056, SPRITE_TILES_BANK44_FRAME056);
const struct sprite_frame gSpriteBank44Frame057 = SPRITE_FRAME(gSpriteBank44Frame057, SPRITE_TILES_BANK44_FRAME057);
const struct sprite_frame gSpriteBank44Frame058 = SPRITE_FRAME(gSpriteBank44Frame058, SPRITE_TILES_BANK44_FRAME058);
const struct sprite_frame_1box gSpriteBank44Frame059 = {
    SPRITE_FRAME(gSpriteBank44Frame059, SPRITE_TILES_BANK44_FRAME059),
    { { -20, -18, 31, 39 } },
};
const struct sprite_frame_1box gSpriteBank44Frame060 = {
    SPRITE_FRAME(gSpriteBank44Frame060, SPRITE_TILES_BANK44_FRAME060),
    { { -16, -18, 31, 39 } },
};
const struct sprite_frame_1box gSpriteBank44Frame061 = {
    SPRITE_FRAME(gSpriteBank44Frame061, SPRITE_TILES_BANK44_FRAME061),
    { { -16, -18, 31, 39 } },
};
const struct sprite_frame_1box gSpriteBank44Frame062 = {
    SPRITE_FRAME(gSpriteBank44Frame062, SPRITE_TILES_BANK44_FRAME062),
    { { -16, -18, 31, 39 } },
};
const struct sprite_frame_1box gSpriteBank44Frame063 = {
    SPRITE_FRAME(gSpriteBank44Frame063, SPRITE_TILES_BANK44_FRAME063),
    { { -16, -18, 31, 39 } },
};
const struct sprite_frame_1box gSpriteBank44Frame064 = {
    SPRITE_FRAME(gSpriteBank44Frame064, SPRITE_TILES_BANK44_FRAME064),
    { { -16, -18, 31, 39 } },
};
const struct sprite_frame_1box gSpriteBank44Frame065 = {
    SPRITE_FRAME(gSpriteBank44Frame065, SPRITE_TILES_BANK44_FRAME065),
    { { -16, -18, 31, 39 } },
};
const struct sprite_frame_1box gSpriteBank44Frame066 = {
    SPRITE_FRAME(gSpriteBank44Frame066, SPRITE_TILES_BANK44_FRAME066),
    { { -16, -18, 31, 39 } },
};
const struct sprite_frame_1box gSpriteBank44Frame067 = {
    SPRITE_FRAME(gSpriteBank44Frame067, SPRITE_TILES_BANK44_FRAME067),
    { { -16, -18, 31, 39 } },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame068 = {
    SPRITE_FRAME(gSpriteBank44Frame068, SPRITE_TILES_BANK44_FRAME068),
    { { -29, -35, 32, 69 } },
    { 38, -3 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame069 = {
    SPRITE_FRAME(gSpriteBank44Frame069, SPRITE_TILES_BANK44_FRAME069),
    { { -29, -35, 32, 69 } },
    { 35, 0 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame070 = {
    SPRITE_FRAME(gSpriteBank44Frame070, SPRITE_TILES_BANK44_FRAME070),
    { { -29, -35, 32, 69 } },
    { 32, 4 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame071 = {
    SPRITE_FRAME(gSpriteBank44Frame071, SPRITE_TILES_BANK44_FRAME071),
    { { -29, -35, 32, 69 } },
    { 29, 7 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame072 = {
    SPRITE_FRAME(gSpriteBank44Frame072, SPRITE_TILES_BANK44_FRAME072),
    { { -29, -35, 32, 69 } },
    { 27, 8 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame073 = {
    SPRITE_FRAME(gSpriteBank44Frame073, SPRITE_TILES_BANK44_FRAME073),
    { { -29, -35, 32, 69 } },
    { 29, 8 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame074 = {
    SPRITE_FRAME(gSpriteBank44Frame074, SPRITE_TILES_BANK44_FRAME074),
    { { -29, -35, 32, 69 } },
    { 30, 8 },
};
const struct sprite_frame_1box gSpriteBank44Frame075 = {
    SPRITE_FRAME(gSpriteBank44Frame075, SPRITE_TILES_BANK44_FRAME075),
    { { -28, -34, 28, 67 } },
};
const struct sprite_frame_1box gSpriteBank44Frame076 = {
    SPRITE_FRAME(gSpriteBank44Frame076, SPRITE_TILES_BANK44_FRAME076),
    { { -28, -34, 28, 67 } },
};
const struct sprite_frame_1box gSpriteBank44Frame077 = {
    SPRITE_FRAME(gSpriteBank44Frame077, SPRITE_TILES_BANK44_FRAME077),
    { { -28, -34, 28, 67 } },
};
const struct sprite_frame_1box gSpriteBank44Frame078 = {
    SPRITE_FRAME(gSpriteBank44Frame078, SPRITE_TILES_BANK44_FRAME078),
    { { -28, -34, 28, 67 } },
};
const struct sprite_frame_1box gSpriteBank44Frame079 = {
    SPRITE_FRAME(gSpriteBank44Frame079, SPRITE_TILES_BANK44_FRAME079),
    { { -28, -34, 28, 67 } },
};
const struct sprite_frame_1box gSpriteBank44Frame080 = {
    SPRITE_FRAME(gSpriteBank44Frame080, SPRITE_TILES_BANK44_FRAME080),
    { { -28, -34, 28, 67 } },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame081 = {
    SPRITE_FRAME(gSpriteBank44Frame081, SPRITE_TILES_BANK44_FRAME081),
    { { -28, -34, 28, 67 } },
    { 35, -8 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame082 = {
    SPRITE_FRAME(gSpriteBank44Frame082, SPRITE_TILES_BANK44_FRAME082),
    { { -28, -34, 28, 67 } },
    { 32, -8 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame083 = {
    SPRITE_FRAME(gSpriteBank44Frame083, SPRITE_TILES_BANK44_FRAME083),
    { { -28, -34, 28, 67 } },
    { 30, -7 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame084 = {
    SPRITE_FRAME(gSpriteBank44Frame084, SPRITE_TILES_BANK44_FRAME084),
    { { -28, -34, 28, 67 } },
    { 34, -5 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame085 = {
    SPRITE_FRAME(gSpriteBank44Frame085, SPRITE_TILES_BANK44_FRAME085),
    { { -28, -34, 28, 67 } },
    { 40, -4 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame086 = {
    SPRITE_FRAME(gSpriteBank44Frame086, SPRITE_TILES_BANK44_FRAME086),
    { { -28, -34, 28, 67 } },
    { 42, -3 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame087 = {
    SPRITE_FRAME(gSpriteBank44Frame087, SPRITE_TILES_BANK44_FRAME087),
    { { -28, -34, 28, 67 } },
    { 41, -4 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame088 = {
    SPRITE_FRAME(gSpriteBank44Frame088, SPRITE_TILES_BANK44_FRAME088),
    { { -28, -34, 28, 67 } },
    { 39, -4 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame089 = {
    SPRITE_FRAME(gSpriteBank44Frame089, SPRITE_TILES_BANK44_FRAME089),
    { { -28, -34, 28, 67 } },
    { 39, -4 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame090 = {
    SPRITE_FRAME(gSpriteBank44Frame090, SPRITE_TILES_BANK44_FRAME090),
    { { -28, -34, 28, 67 } },
    { 39, -4 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame091 = {
    SPRITE_FRAME(gSpriteBank44Frame091, SPRITE_TILES_BANK44_FRAME091),
    { { -29, -34, 31, 68 } },
    { 29, 7 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame092 = {
    SPRITE_FRAME(gSpriteBank44Frame092, SPRITE_TILES_BANK44_FRAME092),
    { { -29, -34, 31, 68 } },
    { 28, 7 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame093 = {
    SPRITE_FRAME(gSpriteBank44Frame093, SPRITE_TILES_BANK44_FRAME093),
    { { -29, -34, 31, 68 } },
    { 28, 8 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame094 = {
    SPRITE_FRAME(gSpriteBank44Frame094, SPRITE_TILES_BANK44_FRAME094),
    { { -29, -34, 31, 68 } },
    { 28, 6 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame095 = {
    SPRITE_FRAME(gSpriteBank44Frame095, SPRITE_TILES_BANK44_FRAME095),
    { { -29, -34, 31, 68 } },
    { 31, 4 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame096 = {
    SPRITE_FRAME(gSpriteBank44Frame096, SPRITE_TILES_BANK44_FRAME096),
    { { -29, -34, 31, 68 } },
    { 36, 0 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame097 = {
    SPRITE_FRAME(gSpriteBank44Frame097, SPRITE_TILES_BANK44_FRAME097),
    { { -29, -34, 31, 68 } },
    { 39, -3 },
};
const struct sprite_frame_1box gSpriteBank44Frame098 = {
    SPRITE_FRAME(gSpriteBank44Frame098, SPRITE_TILES_BANK44_FRAME098),
    { { -29, -34, 31, 68 } },
};
const struct sprite_frame_1box gSpriteBank44Frame099 = {
    SPRITE_FRAME(gSpriteBank44Frame099, SPRITE_TILES_BANK44_FRAME099),
    { { -29, -34, 31, 68 } },
};
const struct sprite_frame_1box gSpriteBank44Frame100 = {
    SPRITE_FRAME(gSpriteBank44Frame100, SPRITE_TILES_BANK44_FRAME100),
    { { -29, -34, 31, 68 } },
};
const struct sprite_frame_1box gSpriteBank44Frame101 = {
    SPRITE_FRAME(gSpriteBank44Frame101, SPRITE_TILES_BANK44_FRAME101),
    { { -25, -32, 35, 64 } },
};
const struct sprite_frame_1box gSpriteBank44Frame102 = {
    SPRITE_FRAME(gSpriteBank44Frame102, SPRITE_TILES_BANK44_FRAME102),
    { { -25, -32, 35, 64 } },
};
const struct sprite_frame_1box gSpriteBank44Frame103 = {
    SPRITE_FRAME(gSpriteBank44Frame103, SPRITE_TILES_BANK44_FRAME103),
    { { -25, -32, 35, 64 } },
};
const struct sprite_frame_1box gSpriteBank44Frame104 = {
    SPRITE_FRAME(gSpriteBank44Frame104, SPRITE_TILES_BANK44_FRAME104),
    { { -25, -32, 35, 64 } },
};
const struct sprite_frame_1box gSpriteBank44Frame105 = {
    SPRITE_FRAME(gSpriteBank44Frame105, SPRITE_TILES_BANK44_FRAME105),
    { { -25, -32, 35, 64 } },
};
const struct sprite_frame_1box gSpriteBank44Frame106 = {
    SPRITE_FRAME(gSpriteBank44Frame106, SPRITE_TILES_BANK44_FRAME106),
    { { -25, -32, 35, 64 } },
};
const struct sprite_frame_1box gSpriteBank44Frame107 = {
    SPRITE_FRAME(gSpriteBank44Frame107, SPRITE_TILES_BANK44_FRAME107),
    { { -25, -32, 35, 64 } },
};
const struct sprite_frame_1box gSpriteBank44Frame108 = {
    SPRITE_FRAME(gSpriteBank44Frame108, SPRITE_TILES_BANK44_FRAME108),
    { { -25, -32, 35, 64 } },
};
const struct sprite_frame_1box gSpriteBank44Frame109 = {
    SPRITE_FRAME(gSpriteBank44Frame109, SPRITE_TILES_BANK44_FRAME109),
    { { -25, -32, 35, 64 } },
};
const struct sprite_frame_1box gSpriteBank44Frame110 = {
    SPRITE_FRAME(gSpriteBank44Frame110, SPRITE_TILES_BANK44_FRAME110),
    { { -25, -32, 35, 64 } },
};
const struct sprite_frame_1box gSpriteBank44Frame111 = {
    SPRITE_FRAME(gSpriteBank44Frame111, SPRITE_TILES_BANK44_FRAME111),
    { { -25, -32, 35, 64 } },
};
const struct sprite_frame_1box gSpriteBank44Frame112 = {
    SPRITE_FRAME(gSpriteBank44Frame112, SPRITE_TILES_BANK44_FRAME112),
    { { -25, -32, 35, 64 } },
};
const struct sprite_frame_3box_anchor gSpriteBank44Frame113 = {
    SPRITE_FRAME(gSpriteBank44Frame113, SPRITE_TILES_BANK44_FRAME113),
    { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { 29, 7 },
};
const struct sprite_frame_3box_anchor gSpriteBank44Frame114 = {
    SPRITE_FRAME(gSpriteBank44Frame114, SPRITE_TILES_BANK44_FRAME114),
    { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { 27, 4 },
};
const struct sprite_frame_3box_anchor gSpriteBank44Frame115 = {
    SPRITE_FRAME(gSpriteBank44Frame115, SPRITE_TILES_BANK44_FRAME115),
    { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { 25, -6 },
};
const struct sprite_frame_3box_anchor gSpriteBank44Frame116 = {
    SPRITE_FRAME(gSpriteBank44Frame116, SPRITE_TILES_BANK44_FRAME116),
    { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { 24, -17 },
};
const struct sprite_frame_3box_anchor gSpriteBank44Frame117 = {
    SPRITE_FRAME(gSpriteBank44Frame117, SPRITE_TILES_BANK44_FRAME117),
    { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { 24, -23 },
};
const struct sprite_frame_3box_anchor gSpriteBank44Frame118 = {
    SPRITE_FRAME(gSpriteBank44Frame118, SPRITE_TILES_BANK44_FRAME118),
    { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { 29, -25 },
};
const struct sprite_frame_3box_anchor gSpriteBank44Frame119 = {
    SPRITE_FRAME(gSpriteBank44Frame119, SPRITE_TILES_BANK44_FRAME119),
    { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { 32, -8 },
};
const struct sprite_frame_3box_anchor gSpriteBank44Frame120 = {
    SPRITE_FRAME(gSpriteBank44Frame120, SPRITE_TILES_BANK44_FRAME120),
    { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { 31, 9 },
};
const struct sprite_frame_3box_anchor gSpriteBank44Frame121 = {
    SPRITE_FRAME(gSpriteBank44Frame121, SPRITE_TILES_BANK44_FRAME121),
    { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { 29, 11 },
};
const struct sprite_frame_3box_anchor gSpriteBank44Frame122 = {
    SPRITE_FRAME(gSpriteBank44Frame122, SPRITE_TILES_BANK44_FRAME122),
    { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { 27, 15 },
};
const struct sprite_frame_3box_anchor gSpriteBank44Frame123 = {
    SPRITE_FRAME(gSpriteBank44Frame123, SPRITE_TILES_BANK44_FRAME123),
    { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { 28, 11 },
};
const struct sprite_frame_3box_anchor gSpriteBank44Frame124 = {
    SPRITE_FRAME(gSpriteBank44Frame124, SPRITE_TILES_BANK44_FRAME124),
    { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { 29, 8 },
};
const struct sprite_frame gSpriteBank44Frame125 = SPRITE_FRAME(gSpriteBank44Frame125, SPRITE_TILES_BANK44_FRAME125);
const struct sprite_frame gSpriteBank44Frame126 = SPRITE_FRAME(gSpriteBank44Frame126, SPRITE_TILES_BANK44_FRAME126);
const struct sprite_frame gSpriteBank44Frame127 = SPRITE_FRAME(gSpriteBank44Frame127, SPRITE_TILES_BANK44_FRAME127);
const struct sprite_frame gSpriteBank44Frame128 = SPRITE_FRAME(gSpriteBank44Frame128, SPRITE_TILES_BANK44_FRAME128);
const struct sprite_frame gSpriteBank44Frame129 = SPRITE_FRAME(gSpriteBank44Frame129, SPRITE_TILES_BANK44_FRAME129);
const struct sprite_frame gSpriteBank44Frame130 = SPRITE_FRAME(gSpriteBank44Frame130, SPRITE_TILES_BANK44_FRAME130);
const struct sprite_frame gSpriteBank44Frame131 = SPRITE_FRAME(gSpriteBank44Frame131, SPRITE_TILES_BANK44_FRAME131);
const struct sprite_frame gSpriteBank44Frame132 = SPRITE_FRAME(gSpriteBank44Frame132, SPRITE_TILES_BANK44_FRAME132);
const struct sprite_frame gSpriteBank44Frame133 = SPRITE_FRAME(gSpriteBank44Frame133, SPRITE_TILES_BANK44_FRAME133);
const struct sprite_frame gSpriteBank44Frame134 = SPRITE_FRAME(gSpriteBank44Frame134, SPRITE_TILES_BANK44_FRAME134);
const struct sprite_frame_3box_anchor gSpriteBank44Frame135 = {
    SPRITE_FRAME(gSpriteBank44Frame135, SPRITE_TILES_BANK44_FRAME135),
    { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { 40, -8 },
};
const struct sprite_frame gSpriteBank44Frame136 = SPRITE_FRAME(gSpriteBank44Frame136, SPRITE_TILES_BANK44_FRAME136);
const struct sprite_frame gSpriteBank44Frame137 = SPRITE_FRAME(gSpriteBank44Frame137, SPRITE_TILES_BANK44_FRAME137);
const struct sprite_frame gSpriteBank44Frame138 = SPRITE_FRAME(gSpriteBank44Frame138, SPRITE_TILES_BANK44_FRAME138);
const struct sprite_frame gSpriteBank44Frame139 = SPRITE_FRAME(gSpriteBank44Frame139, SPRITE_TILES_BANK44_FRAME139);
const struct sprite_frame gSpriteBank44Frame140 = SPRITE_FRAME(gSpriteBank44Frame140, SPRITE_TILES_BANK44_FRAME140);
const struct sprite_frame gSpriteBank44Frame141 = SPRITE_FRAME(gSpriteBank44Frame141, SPRITE_TILES_BANK44_FRAME141);
const struct sprite_frame gSpriteBank44Frame142 = SPRITE_FRAME(gSpriteBank44Frame142, SPRITE_TILES_BANK44_FRAME142);
const struct sprite_frame gSpriteBank44Frame143 = { gSpriteBank44Frame144Pos, gSpriteBank44Frame144Pieces, SPRITE_FRAME_TILES(SPRITE_TILES_BANK44_FRAME143, 0) };
const struct sprite_frame gSpriteBank44Frame144 = SPRITE_FRAME(gSpriteBank44Frame144, SPRITE_TILES_BANK44_FRAME144);
const struct sprite_frame gSpriteBank44Frame145 = SPRITE_FRAME(gSpriteBank44Frame145, SPRITE_TILES_BANK44_FRAME145);
const struct sprite_frame_1box gSpriteBank44Frame146 = {
    SPRITE_FRAME(gSpriteBank44Frame146, SPRITE_TILES_BANK44_FRAME146),
    { { -8, -9, 17, 17 } },
};
const struct sprite_frame_1box gSpriteBank44Frame147 = {
    SPRITE_FRAME(gSpriteBank44Frame147, SPRITE_TILES_BANK44_FRAME147),
    { { -33, -35, 38, 61 } },
};
const struct sprite_frame_1box gSpriteBank44Frame148 = {
    SPRITE_FRAME(gSpriteBank44Frame148, SPRITE_TILES_BANK44_FRAME148),
    { { -33, -21, 38, 51 } },
};
const struct sprite_frame_1box gSpriteBank44Frame149 = {
    SPRITE_FRAME(gSpriteBank44Frame149, SPRITE_TILES_BANK44_FRAME149),
    { { -33, -35, 38, 61 } },
};
const struct sprite_frame_1box gSpriteBank44Frame150 = {
    SPRITE_FRAME(gSpriteBank44Frame150, SPRITE_TILES_BANK44_FRAME150),
    { { -33, -35, 38, 61 } },
};
const struct sprite_frame_1box gSpriteBank44Frame151 = {
    SPRITE_FRAME(gSpriteBank44Frame151, SPRITE_TILES_BANK44_FRAME151),
    { { -33, -35, 38, 61 } },
};
const struct sprite_frame_1box gSpriteBank44Frame152 = {
    SPRITE_FRAME(gSpriteBank44Frame152, SPRITE_TILES_BANK44_FRAME152),
    { { -33, -35, 38, 61 } },
};
const struct sprite_frame_1box gSpriteBank44Frame153 = {
    SPRITE_FRAME(gSpriteBank44Frame153, SPRITE_TILES_BANK44_FRAME153),
    { { -33, -35, 38, 61 } },
};
const struct sprite_frame_1box gSpriteBank44Frame154 = {
    SPRITE_FRAME(gSpriteBank44Frame154, SPRITE_TILES_BANK44_FRAME154),
    { { -33, -35, 38, 61 } },
};
const struct sprite_frame_1box gSpriteBank44Frame155 = {
    SPRITE_FRAME(gSpriteBank44Frame155, SPRITE_TILES_BANK44_FRAME155),
    { { -33, -35, 38, 61 } },
};
const struct sprite_frame_1box gSpriteBank44Frame156 = {
    SPRITE_FRAME(gSpriteBank44Frame156, SPRITE_TILES_BANK44_FRAME156),
    { { -33, -35, 38, 61 } },
};
const struct sprite_frame_1box gSpriteBank44Frame157 = {
    SPRITE_FRAME(gSpriteBank44Frame157, SPRITE_TILES_BANK44_FRAME157),
    { { -29, -45, 39, 71 } },
};
const struct sprite_frame_1box gSpriteBank44Frame158 = {
    SPRITE_FRAME(gSpriteBank44Frame158, SPRITE_TILES_BANK44_FRAME158),
    { { -29, -45, 39, 71 } },
};
const struct sprite_frame_1box gSpriteBank44Frame159 = {
    SPRITE_FRAME(gSpriteBank44Frame159, SPRITE_TILES_BANK44_FRAME159),
    { { -29, -45, 39, 71 } },
};
const struct sprite_frame_1box gSpriteBank44Frame160 = {
    SPRITE_FRAME(gSpriteBank44Frame160, SPRITE_TILES_BANK44_FRAME160),
    { { -29, -45, 39, 71 } },
};
const struct sprite_frame_1box gSpriteBank44Frame161 = {
    SPRITE_FRAME(gSpriteBank44Frame161, SPRITE_TILES_BANK44_FRAME161),
    { { -29, -45, 39, 71 } },
};
const struct sprite_frame_1box gSpriteBank44Frame162 = {
    SPRITE_FRAME(gSpriteBank44Frame162, SPRITE_TILES_BANK44_FRAME162),
    { { -29, -45, 39, 71 } },
};
const struct sprite_frame_1box gSpriteBank44Frame163 = {
    SPRITE_FRAME(gSpriteBank44Frame163, SPRITE_TILES_BANK44_FRAME163),
    { { -29, -45, 39, 71 } },
};
const struct sprite_frame_1box gSpriteBank44Frame164 = {
    SPRITE_FRAME(gSpriteBank44Frame164, SPRITE_TILES_BANK44_FRAME164),
    { { -29, -45, 39, 71 } },
};
const struct sprite_frame_1box gSpriteBank44Frame165 = {
    SPRITE_FRAME(gSpriteBank44Frame165, SPRITE_TILES_BANK44_FRAME165),
    { { -31, -36, 37, 64 } },
};
const struct sprite_frame_1box gSpriteBank44Frame166 = {
    SPRITE_FRAME(gSpriteBank44Frame166, SPRITE_TILES_BANK44_FRAME166),
    { { -29, -34, 32, 62 } },
};
const struct sprite_frame_1box gSpriteBank44Frame167 = {
    SPRITE_FRAME(gSpriteBank44Frame167, SPRITE_TILES_BANK44_FRAME167),
    { { -25, -27, 34, 60 } },
};
const struct sprite_frame_1box gSpriteBank44Frame168 = {
    SPRITE_FRAME(gSpriteBank44Frame168, SPRITE_TILES_BANK44_FRAME168),
    { { -25, -27, 34, 60 } },
};
const struct sprite_frame_1box gSpriteBank44Frame169 = {
    SPRITE_FRAME(gSpriteBank44Frame169, SPRITE_TILES_BANK44_FRAME169),
    { { -25, -27, 34, 60 } },
};
const struct sprite_frame_1box gSpriteBank44Frame170 = {
    SPRITE_FRAME(gSpriteBank44Frame170, SPRITE_TILES_BANK44_FRAME170),
    { { -25, -27, 34, 60 } },
};
const struct sprite_frame_1box gSpriteBank44Frame171 = {
    SPRITE_FRAME(gSpriteBank44Frame171, SPRITE_TILES_BANK44_FRAME171),
    { { -25, -27, 34, 60 } },
};
const struct sprite_frame_1box gSpriteBank44Frame172 = {
    SPRITE_FRAME(gSpriteBank44Frame172, SPRITE_TILES_BANK44_FRAME172),
    { { -25, -27, 34, 60 } },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame173 = {
    SPRITE_FRAME(gSpriteBank44Frame173, SPRITE_TILES_BANK44_FRAME173),
    { { -22, -33, 39, 66 } },
    { 37, 5 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame174 = {
    SPRITE_FRAME(gSpriteBank44Frame174, SPRITE_TILES_BANK44_FRAME174),
    { { -22, -33, 39, 66 } },
    { 34, 5 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame175 = {
    SPRITE_FRAME(gSpriteBank44Frame175, SPRITE_TILES_BANK44_FRAME175),
    { { -22, -33, 39, 66 } },
    { 30, 3 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame176 = {
    SPRITE_FRAME(gSpriteBank44Frame176, SPRITE_TILES_BANK44_FRAME176),
    { { -22, -33, 39, 66 } },
    { 26, 1 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame177 = {
    SPRITE_FRAME(gSpriteBank44Frame177, SPRITE_TILES_BANK44_FRAME177),
    { { -22, -33, 39, 66 } },
    { 26, 1 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame178 = {
    SPRITE_FRAME(gSpriteBank44Frame178, SPRITE_TILES_BANK44_FRAME178),
    { { -22, -33, 39, 66 } },
    { 29, 3 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame179 = {
    SPRITE_FRAME(gSpriteBank44Frame179, SPRITE_TILES_BANK44_FRAME179),
    { { -22, -33, 39, 66 } },
    { 29, 4 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame180 = {
    SPRITE_FRAME(gSpriteBank44Frame180, SPRITE_TILES_BANK44_FRAME180),
    { { -22, -33, 39, 66 } },
    { 28, 3 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame181 = {
    SPRITE_FRAME(gSpriteBank44Frame181, SPRITE_TILES_BANK44_FRAME181),
    { { -22, -33, 39, 66 } },
    { 25, 1 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame182 = {
    SPRITE_FRAME(gSpriteBank44Frame182, SPRITE_TILES_BANK44_FRAME182),
    { { -22, -33, 39, 66 } },
    { 23, 0 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame183 = {
    SPRITE_FRAME(gSpriteBank44Frame183, SPRITE_TILES_BANK44_FRAME183),
    { { -22, -33, 39, 66 } },
    { 22, -1 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame184 = {
    SPRITE_FRAME(gSpriteBank44Frame184, SPRITE_TILES_BANK44_FRAME184),
    { { -22, -33, 39, 66 } },
    { 24, 0 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame185 = {
    SPRITE_FRAME(gSpriteBank44Frame185, SPRITE_TILES_BANK44_FRAME185),
    { { -22, -33, 39, 66 } },
    { 30, 3 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame186 = {
    SPRITE_FRAME(gSpriteBank44Frame186, SPRITE_TILES_BANK44_FRAME186),
    { { -22, -33, 39, 66 } },
    { 35, 6 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame187 = {
    SPRITE_FRAME(gSpriteBank44Frame187, SPRITE_TILES_BANK44_FRAME187),
    { { -22, -33, 39, 66 } },
    { 39, 7 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame188 = {
    SPRITE_FRAME(gSpriteBank44Frame188, SPRITE_TILES_BANK44_FRAME188),
    { { -22, -33, 39, 66 } },
    { 40, 8 },
};
const struct sprite_frame_1box_anchor gSpriteBank44Frame189 = {
    SPRITE_FRAME(gSpriteBank44Frame189, SPRITE_TILES_BANK44_FRAME189),
    { { -22, -33, 39, 66 } },
    { 40, 7 },
};

/* The piece arrays, from games/n2/graphics/sprites/bank44_n_trance/bank.json. */
#include "sprites/bank44_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 45: 20 animations, 163 frames, games/n2/graphics/sprites/bank45_n_tropy/. */

extern const u16 gSpriteBank45Anim00Seq[10];
extern const u16 gSpriteBank45Anim01Seq[8];
extern const u16 gSpriteBank45Anim02Seq[9];
extern const u16 gSpriteBank45Anim03Seq[9];
extern const u16 gSpriteBank45Anim04Seq[5];
extern const u16 gSpriteBank45Anim05Seq[4];
extern const u16 gSpriteBank45Anim06Seq[7];
extern const u16 gSpriteBank45Anim07Seq[12];
extern const u16 gSpriteBank45Anim08Seq[8];
extern const u16 gSpriteBank45Anim09Seq[11];
extern const u16 gSpriteBank45Anim10Seq[8];
extern const u16 gSpriteBank45Anim11Seq[8];
extern const u16 gSpriteBank45Anim12Seq[8];
extern const u16 gSpriteBank45Anim13Seq[10];
extern const u16 gSpriteBank45Anim14Seq[9];
extern const u16 gSpriteBank45Anim15Seq[6];
extern const u16 gSpriteBank45Anim16Seq[3];
extern const u16 gSpriteBank45Anim17Seq[8];
extern const u16 gSpriteBank45Anim18Seq[12];
extern const u16 gSpriteBank45Anim19Seq[8];

extern const struct sprite_anim gSpriteBank45Anims[20] = {
    /* 0 */ {
        /* seq */ gSpriteBank45Anim00Seq,
        /* box */ { { -19, -40, 39, 80 }, { -51, -47, 72, 87 } },
        /* paletteId */ 85,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank45Anim00Seq),
        /* flags */ 0,
    },
    /* 1 */ {
        /* seq */ gSpriteBank45Anim01Seq,
        /* box */ { { -19, -40, 39, 80 }, { -38, -39, 59, 79 } },
        /* paletteId */ 85,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank45Anim01Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 2 */ {
        /* seq */ gSpriteBank45Anim02Seq,
        /* box */ { { -19, -40, 39, 80 }, { -36, -39, 67, 79 } },
        /* paletteId */ 85,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank45Anim02Seq),
        /* flags */ 0,
    },
    /* 3 */ {
        /* seq */ gSpriteBank45Anim03Seq,
        /* box */ { { -19, -40, 39, 80 }, { -28, -54, 55, 94 } },
        /* paletteId */ 85,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank45Anim03Seq),
        /* flags */ 0,
    },
    /* 4 */ {
        /* seq */ gSpriteBank45Anim04Seq,
        /* box */ { { -19, -40, 39, 80 }, { -28, -38, 48, 78 } },
        /* paletteId */ 85,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank45Anim04Seq),
        /* flags */ 0,
    },
    /* 5 */ {
        /* seq */ gSpriteBank45Anim05Seq,
        /* box */ { { -19, -40, 39, 80 }, { -28, -31, 45, 71 } },
        /* paletteId */ 85,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank45Anim05Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 6 */ {
        /* seq */ gSpriteBank45Anim06Seq,
        /* box */ { { -19, -40, 39, 80 }, { -28, -40, 48, 80 } },
        /* paletteId */ 85,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank45Anim06Seq),
        /* flags */ 0,
    },
    /* 7 */ {
        /* seq */ gSpriteBank45Anim07Seq,
        /* box */ { { -19, -40, 39, 80 }, { -46, -46, 79, 86 } },
        /* paletteId */ 85,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank45Anim07Seq),
        /* flags */ 0,
    },
    /* 8 */ {
        /* seq */ gSpriteBank45Anim08Seq,
        /* box */ { { -19, -40, 39, 80 }, { -19, -40, 40, 80 } },
        /* paletteId */ 85,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank45Anim08Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 9 */ {
        /* seq */ gSpriteBank45Anim09Seq,
        /* box */ { { -19, -40, 39, 80 }, { -26, -40, 61, 80 } },
        /* paletteId */ 85,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank45Anim09Seq),
        /* flags */ 0,
    },
    /* 10 */ {
        /* seq */ gSpriteBank45Anim10Seq,
        /* box */ { { -19, -40, 39, 80 }, { -50, -46, 81, 86 } },
        /* paletteId */ 85,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank45Anim10Seq),
        /* flags */ 0,
    },
    /* 11 */ {
        /* seq */ gSpriteBank45Anim11Seq,
        /* box */ { { -19, -40, 39, 80 }, { -30, -39, 63, 77 } },
        /* paletteId */ 85,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank45Anim11Seq),
        /* flags */ 0,
    },
    /* 12 */ {
        /* seq */ gSpriteBank45Anim12Seq,
        /* box */ { { -19, -40, 39, 80 }, { -36, -39, 70, 77 } },
        /* paletteId */ 85,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank45Anim12Seq),
        /* flags */ 0,
    },
    /* 13 */ {
        /* seq */ gSpriteBank45Anim13Seq,
        /* box */ { { -19, -40, 39, 80 }, { -38, -32, 65, 72 } },
        /* paletteId */ 85,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank45Anim13Seq),
        /* flags */ 0,
    },
    /* 14 */ {
        /* seq */ gSpriteBank45Anim14Seq,
        /* box */ { { -19, -40, 39, 80 }, { -36, -40, 57, 80 } },
        /* paletteId */ 85,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank45Anim14Seq),
        /* flags */ 0,
    },
    /* 15 */ {
        /* seq */ gSpriteBank45Anim15Seq,
        /* box */ { { -19, -40, 39, 80 }, { -67, -66, 52, 23 } },
        /* paletteId */ 86,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank45Anim15Seq),
        /* flags */ 0,
    },
    /* 16 */ {
        /* seq */ gSpriteBank45Anim16Seq,
        /* box */ { { -28, -10, 57, 20 }, { -28, -11, 57, 24 } },
        /* paletteId */ 86,
        /* duration */ 5,
        /* frameCount */ ARRAY_COUNT(gSpriteBank45Anim16Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 17 */ {
        /* seq */ gSpriteBank45Anim17Seq,
        /* box */ { { -14, -12, 28, 24 }, { -14, -12, 30, 31 } },
        /* paletteId */ 87,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank45Anim17Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 18 */ {
        /* seq */ gSpriteBank45Anim18Seq,
        /* box */ { { -19, -40, 39, 80 }, { -46, -42, 79, 82 } },
        /* paletteId */ 85,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank45Anim18Seq),
        /* flags */ 0,
    },
    /* 19 */ {
        /* seq */ gSpriteBank45Anim19Seq,
        /* box */ { { -19, -40, 39, 80 }, { -50, -46, 79, 86 } },
        /* paletteId */ 85,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank45Anim19Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank45Anim00Seq[10] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
};
const u16 gSpriteBank45Anim01Seq[8] = {
    10, 11, 12, 13, 14, 15, 16, 17,
};
const u16 gSpriteBank45Anim02Seq[9] = {
    18, 19, 20, 21, 22, 23, 24, 25, 26,
};
const u16 gSpriteBank45Anim03Seq[9] = {
    27, 28, 29, 30, 31, 32, 33, 34, 35,
};
const u16 gSpriteBank45Anim04Seq[5] = {
    36, 37, 38, 39, 40,
};
const u16 gSpriteBank45Anim05Seq[4] = {
    41, 42, 43, 44,
};
const u16 gSpriteBank45Anim06Seq[7] = {
    45, 46, 47, 48, 49, 50, 51,
};
const u16 gSpriteBank45Anim07Seq[12] = {
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
};
const u16 gSpriteBank45Anim08Seq[8] = {
    64, 65, 66, 67, 68, 69, 70, 71,
};
const u16 gSpriteBank45Anim09Seq[11] = {
    72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82,
};
const u16 gSpriteBank45Anim10Seq[8] = {
    83, 84, 85, 86, 87, 88, 89, 90,
};
const u16 gSpriteBank45Anim11Seq[8] = {
    91, 92, 93, 94, 95, 96, 97, 98,
};
const u16 gSpriteBank45Anim12Seq[8] = {
    99, 100, 101, 102, 103, 104, 105, 106,
};
const u16 gSpriteBank45Anim13Seq[10] = {
    107, 108, 109, 110, 111, 112, 113, 114, 115, 116,
};
const u16 gSpriteBank45Anim14Seq[9] = {
    117, 118, 119, 120, 121, 122, 123, 124, 125,
};
const u16 gSpriteBank45Anim15Seq[6] = {
    126, 127, 128, 129, 130, 131,
};
const u16 gSpriteBank45Anim16Seq[3] = {
    132, 133, 134,
};
const u16 gSpriteBank45Anim17Seq[8] = {
    135, 136, 137, 138, 139, 140, 141, 142,
};
const u16 gSpriteBank45Anim18Seq[12] = {
    143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154,
};
const u16 gSpriteBank45Anim19Seq[8] = {
    155, 156, 157, 158, 159, 160, 161, 162,
};

extern const struct sprite_frame *const gSpriteBank45Frames[163] = {
    &gSpriteBank45Frame000.frame,
    &gSpriteBank45Frame001.frame,
    &gSpriteBank45Frame002.frame,
    &gSpriteBank45Frame003.frame,
    &gSpriteBank45Frame004.frame,
    &gSpriteBank45Frame005.frame,
    &gSpriteBank45Frame006.frame,
    &gSpriteBank45Frame007.frame,
    &gSpriteBank45Frame008.frame,
    &gSpriteBank45Frame009.frame,
    &gSpriteBank45Frame010.frame,
    &gSpriteBank45Frame011.frame,
    &gSpriteBank45Frame012.frame,
    &gSpriteBank45Frame013.frame,
    &gSpriteBank45Frame014.frame,
    &gSpriteBank45Frame015.frame,
    &gSpriteBank45Frame016.frame,
    &gSpriteBank45Frame017.frame,
    &gSpriteBank45Frame018.frame,
    &gSpriteBank45Frame019.frame,
    &gSpriteBank45Frame020.frame,
    &gSpriteBank45Frame021.frame,
    &gSpriteBank45Frame022.frame,
    &gSpriteBank45Frame023.frame,
    &gSpriteBank45Frame024.frame,
    &gSpriteBank45Frame025.frame,
    &gSpriteBank45Frame026.frame,
    &gSpriteBank45Frame027,
    &gSpriteBank45Frame028,
    &gSpriteBank45Frame029,
    &gSpriteBank45Frame030,
    &gSpriteBank45Frame031,
    &gSpriteBank45Frame032,
    &gSpriteBank45Frame033,
    &gSpriteBank45Frame034,
    &gSpriteBank45Frame035,
    &gSpriteBank45Frame036.frame,
    &gSpriteBank45Frame037.frame,
    &gSpriteBank45Frame038.frame,
    &gSpriteBank45Frame039.frame,
    &gSpriteBank45Frame040.frame,
    &gSpriteBank45Frame041.frame,
    &gSpriteBank45Frame042.frame,
    &gSpriteBank45Frame043.frame,
    &gSpriteBank45Frame044.frame,
    &gSpriteBank45Frame045.frame,
    &gSpriteBank45Frame046.frame,
    &gSpriteBank45Frame047.frame,
    &gSpriteBank45Frame048.frame,
    &gSpriteBank45Frame049.frame,
    &gSpriteBank45Frame050.frame,
    &gSpriteBank45Frame051.frame,
    &gSpriteBank45Frame052.frame,
    &gSpriteBank45Frame053.frame,
    &gSpriteBank45Frame054.frame,
    &gSpriteBank45Frame055.frame,
    &gSpriteBank45Frame056.frame,
    &gSpriteBank45Frame057.frame,
    &gSpriteBank45Frame058.frame,
    &gSpriteBank45Frame059.frame,
    &gSpriteBank45Frame060.frame,
    &gSpriteBank45Frame061.frame,
    &gSpriteBank45Frame062.frame,
    &gSpriteBank45Frame063.frame,
    &gSpriteBank45Frame064.frame,
    &gSpriteBank45Frame065.frame,
    &gSpriteBank45Frame066.frame,
    &gSpriteBank45Frame067.frame,
    &gSpriteBank45Frame068.frame,
    &gSpriteBank45Frame069.frame,
    &gSpriteBank45Frame070.frame,
    &gSpriteBank45Frame071.frame,
    &gSpriteBank45Frame072,
    &gSpriteBank45Frame073,
    &gSpriteBank45Frame074,
    &gSpriteBank45Frame075,
    &gSpriteBank45Frame076,
    &gSpriteBank45Frame077,
    &gSpriteBank45Frame078,
    &gSpriteBank45Frame079,
    &gSpriteBank45Frame080,
    &gSpriteBank45Frame081,
    &gSpriteBank45Frame082,
    &gSpriteBank45Frame083.frame,
    &gSpriteBank45Frame084.frame,
    &gSpriteBank45Frame085.frame,
    &gSpriteBank45Frame086.frame,
    &gSpriteBank45Frame087.frame,
    &gSpriteBank45Frame088.frame,
    &gSpriteBank45Frame089.frame,
    &gSpriteBank45Frame090.frame,
    &gSpriteBank45Frame091.frame,
    &gSpriteBank45Frame092.frame,
    &gSpriteBank45Frame093.frame,
    &gSpriteBank45Frame094.frame,
    &gSpriteBank45Frame095.frame,
    &gSpriteBank45Frame096.frame,
    &gSpriteBank45Frame097.frame,
    &gSpriteBank45Frame098.frame,
    &gSpriteBank45Frame099.frame,
    &gSpriteBank45Frame100.frame,
    &gSpriteBank45Frame101.frame,
    &gSpriteBank45Frame102.frame,
    &gSpriteBank45Frame103.frame,
    &gSpriteBank45Frame104.frame,
    &gSpriteBank45Frame105.frame,
    &gSpriteBank45Frame106.frame,
    &gSpriteBank45Frame107,
    &gSpriteBank45Frame108,
    &gSpriteBank45Frame109,
    &gSpriteBank45Frame110,
    &gSpriteBank45Frame111,
    &gSpriteBank45Frame112,
    &gSpriteBank45Frame113,
    &gSpriteBank45Frame114,
    &gSpriteBank45Frame115,
    &gSpriteBank45Frame116,
    &gSpriteBank45Frame117.frame,
    &gSpriteBank45Frame118.frame,
    &gSpriteBank45Frame119.frame,
    &gSpriteBank45Frame120.frame,
    &gSpriteBank45Frame121.frame,
    &gSpriteBank45Frame122.frame,
    &gSpriteBank45Frame123.frame,
    &gSpriteBank45Frame124.frame,
    &gSpriteBank45Frame125.frame,
    &gSpriteBank45Frame126,
    &gSpriteBank45Frame127,
    &gSpriteBank45Frame128,
    &gSpriteBank45Frame129,
    &gSpriteBank45Frame130,
    &gSpriteBank45Frame131,
    &gSpriteBank45Frame132.frame,
    &gSpriteBank45Frame133.frame,
    &gSpriteBank45Frame134.frame,
    &gSpriteBank45Frame135.frame,
    &gSpriteBank45Frame136.frame,
    &gSpriteBank45Frame137.frame,
    &gSpriteBank45Frame138.frame,
    &gSpriteBank45Frame139.frame,
    &gSpriteBank45Frame140.frame,
    &gSpriteBank45Frame141.frame,
    &gSpriteBank45Frame142.frame,
    &gSpriteBank45Frame143.frame,
    &gSpriteBank45Frame144.frame,
    &gSpriteBank45Frame145.frame,
    &gSpriteBank45Frame146.frame,
    &gSpriteBank45Frame147.frame,
    &gSpriteBank45Frame148.frame,
    &gSpriteBank45Frame149.frame,
    &gSpriteBank45Frame150.frame,
    &gSpriteBank45Frame151.frame,
    &gSpriteBank45Frame152.frame,
    &gSpriteBank45Frame153.frame,
    &gSpriteBank45Frame154.frame,
    &gSpriteBank45Frame155.frame,
    &gSpriteBank45Frame156.frame,
    &gSpriteBank45Frame157.frame,
    &gSpriteBank45Frame158.frame,
    &gSpriteBank45Frame159.frame,
    &gSpriteBank45Frame160.frame,
    &gSpriteBank45Frame161.frame,
    &gSpriteBank45Frame162.frame,
};

const struct sprite_frame_1box gSpriteBank45Frame000 = {
    SPRITE_FRAME(gSpriteBank45Frame000, SPRITE_TILES_BANK45_FRAME000),
    { { -8, -28, 27, 68 } },
};
const struct sprite_frame_1box gSpriteBank45Frame001 = {
    SPRITE_FRAME(gSpriteBank45Frame001, SPRITE_TILES_BANK45_FRAME001),
    { { -8, -28, 27, 68 } },
};
const struct sprite_frame_1box gSpriteBank45Frame002 = {
    SPRITE_FRAME(gSpriteBank45Frame002, SPRITE_TILES_BANK45_FRAME002),
    { { -8, -28, 27, 68 } },
};
const struct sprite_frame_1box gSpriteBank45Frame003 = {
    SPRITE_FRAME(gSpriteBank45Frame003, SPRITE_TILES_BANK45_FRAME003),
    { { -8, -28, 27, 68 } },
};
const struct sprite_frame_1box gSpriteBank45Frame004 = {
    SPRITE_FRAME(gSpriteBank45Frame004, SPRITE_TILES_BANK45_FRAME004),
    { { -8, -28, 27, 68 } },
};
const struct sprite_frame_1box gSpriteBank45Frame005 = {
    SPRITE_FRAME(gSpriteBank45Frame005, SPRITE_TILES_BANK45_FRAME005),
    { { -8, -28, 27, 68 } },
};
const struct sprite_frame_1box gSpriteBank45Frame006 = {
    SPRITE_FRAME(gSpriteBank45Frame006, SPRITE_TILES_BANK45_FRAME006),
    { { -8, -28, 27, 68 } },
};
const struct sprite_frame_1box gSpriteBank45Frame007 = {
    SPRITE_FRAME(gSpriteBank45Frame007, SPRITE_TILES_BANK45_FRAME007),
    { { -8, -28, 27, 68 } },
};
const struct sprite_frame_1box gSpriteBank45Frame008 = {
    SPRITE_FRAME(gSpriteBank45Frame008, SPRITE_TILES_BANK45_FRAME008),
    { { -8, -28, 27, 68 } },
};
const struct sprite_frame_1box gSpriteBank45Frame009 = {
    SPRITE_FRAME(gSpriteBank45Frame009, SPRITE_TILES_BANK45_FRAME009),
    { { -8, -28, 27, 68 } },
};
const struct sprite_frame_1box gSpriteBank45Frame010 = {
    SPRITE_FRAME(gSpriteBank45Frame010, SPRITE_TILES_BANK45_FRAME010),
    { { -6, -33, 20, 71 } },
};
const struct sprite_frame_1box gSpriteBank45Frame011 = {
    SPRITE_FRAME(gSpriteBank45Frame011, SPRITE_TILES_BANK45_FRAME011),
    { { -6, -33, 20, 71 } },
};
const struct sprite_frame_1box gSpriteBank45Frame012 = {
    SPRITE_FRAME(gSpriteBank45Frame012, SPRITE_TILES_BANK45_FRAME012),
    { { -6, -33, 20, 71 } },
};
const struct sprite_frame_1box gSpriteBank45Frame013 = {
    SPRITE_FRAME(gSpriteBank45Frame013, SPRITE_TILES_BANK45_FRAME013),
    { { -6, -33, 20, 71 } },
};
const struct sprite_frame_1box gSpriteBank45Frame014 = {
    SPRITE_FRAME(gSpriteBank45Frame014, SPRITE_TILES_BANK45_FRAME014),
    { { -6, -33, 20, 71 } },
};
const struct sprite_frame_1box gSpriteBank45Frame015 = {
    SPRITE_FRAME(gSpriteBank45Frame015, SPRITE_TILES_BANK45_FRAME015),
    { { -6, -33, 20, 71 } },
};
const struct sprite_frame_1box gSpriteBank45Frame016 = {
    SPRITE_FRAME(gSpriteBank45Frame016, SPRITE_TILES_BANK45_FRAME016),
    { { -6, -33, 20, 71 } },
};
const struct sprite_frame_1box gSpriteBank45Frame017 = {
    SPRITE_FRAME(gSpriteBank45Frame017, SPRITE_TILES_BANK45_FRAME017),
    { { -6, -33, 20, 71 } },
};
const struct sprite_frame_1box gSpriteBank45Frame018 = {
    SPRITE_FRAME(gSpriteBank45Frame018, SPRITE_TILES_BANK45_FRAME018),
    { { -9, -30, 25, 70 } },
};
const struct sprite_frame_1box gSpriteBank45Frame019 = {
    SPRITE_FRAME(gSpriteBank45Frame019, SPRITE_TILES_BANK45_FRAME019),
    { { -9, -30, 25, 70 } },
};
const struct sprite_frame_1box gSpriteBank45Frame020 = {
    SPRITE_FRAME(gSpriteBank45Frame020, SPRITE_TILES_BANK45_FRAME020),
    { { -9, -30, 25, 70 } },
};
const struct sprite_frame_1box gSpriteBank45Frame021 = {
    SPRITE_FRAME(gSpriteBank45Frame021, SPRITE_TILES_BANK45_FRAME021),
    { { -9, -30, 25, 70 } },
};
const struct sprite_frame_1box gSpriteBank45Frame022 = {
    SPRITE_FRAME(gSpriteBank45Frame022, SPRITE_TILES_BANK45_FRAME022),
    { { -9, -30, 25, 70 } },
};
const struct sprite_frame_1box gSpriteBank45Frame023 = {
    SPRITE_FRAME(gSpriteBank45Frame023, SPRITE_TILES_BANK45_FRAME023),
    { { -9, -30, 25, 70 } },
};
const struct sprite_frame_1box gSpriteBank45Frame024 = {
    SPRITE_FRAME(gSpriteBank45Frame024, SPRITE_TILES_BANK45_FRAME024),
    { { -9, -30, 25, 70 } },
};
const struct sprite_frame_1box gSpriteBank45Frame025 = {
    SPRITE_FRAME(gSpriteBank45Frame025, SPRITE_TILES_BANK45_FRAME025),
    { { -9, -30, 25, 70 } },
};
const struct sprite_frame_1box gSpriteBank45Frame026 = {
    SPRITE_FRAME(gSpriteBank45Frame026, SPRITE_TILES_BANK45_FRAME026),
    { { -9, -30, 25, 70 } },
};
const struct sprite_frame gSpriteBank45Frame027 = SPRITE_FRAME(gSpriteBank45Frame027, SPRITE_TILES_BANK45_FRAME027);
const struct sprite_frame gSpriteBank45Frame028 = SPRITE_FRAME(gSpriteBank45Frame028, SPRITE_TILES_BANK45_FRAME028);
const struct sprite_frame gSpriteBank45Frame029 = SPRITE_FRAME(gSpriteBank45Frame029, SPRITE_TILES_BANK45_FRAME029);
const struct sprite_frame gSpriteBank45Frame030 = SPRITE_FRAME(gSpriteBank45Frame030, SPRITE_TILES_BANK45_FRAME030);
const struct sprite_frame gSpriteBank45Frame031 = SPRITE_FRAME(gSpriteBank45Frame031, SPRITE_TILES_BANK45_FRAME031);
const struct sprite_frame gSpriteBank45Frame032 = SPRITE_FRAME(gSpriteBank45Frame032, SPRITE_TILES_BANK45_FRAME032);
const struct sprite_frame gSpriteBank45Frame033 = SPRITE_FRAME(gSpriteBank45Frame033, SPRITE_TILES_BANK45_FRAME033);
const struct sprite_frame gSpriteBank45Frame034 = SPRITE_FRAME(gSpriteBank45Frame034, SPRITE_TILES_BANK45_FRAME034);
const struct sprite_frame gSpriteBank45Frame035 = SPRITE_FRAME(gSpriteBank45Frame035, SPRITE_TILES_BANK45_FRAME035);
const struct sprite_frame_1box gSpriteBank45Frame036 = {
    SPRITE_FRAME(gSpriteBank45Frame036, SPRITE_TILES_BANK45_FRAME036),
    { { -21, 4, 38, 37 } },
};
const struct sprite_frame_1box gSpriteBank45Frame037 = {
    SPRITE_FRAME(gSpriteBank45Frame037, SPRITE_TILES_BANK45_FRAME037),
    { { -25, 0, 42, 40 } },
};
const struct sprite_frame_1box gSpriteBank45Frame038 = {
    SPRITE_FRAME(gSpriteBank45Frame038, SPRITE_TILES_BANK45_FRAME038),
    { { -17, -4, 35, 52 } },
};
const struct sprite_frame_1box gSpriteBank45Frame039 = {
    SPRITE_FRAME(gSpriteBank45Frame039, SPRITE_TILES_BANK45_FRAME039),
    { { -14, -28, 33, 74 } },
};
const struct sprite_frame_1box gSpriteBank45Frame040 = {
    SPRITE_FRAME(gSpriteBank45Frame040, SPRITE_TILES_BANK45_FRAME040),
    { { -12, -35, 34, 80 } },
};
const struct sprite_frame_1box gSpriteBank45Frame041 = {
    SPRITE_FRAME(gSpriteBank45Frame041, SPRITE_TILES_BANK45_FRAME041),
    { { -21, 4, 38, 37 } },
};
const struct sprite_frame_1box gSpriteBank45Frame042 = {
    SPRITE_FRAME(gSpriteBank45Frame042, SPRITE_TILES_BANK45_FRAME042),
    { { -21, 4, 38, 37 } },
};
const struct sprite_frame_1box gSpriteBank45Frame043 = {
    SPRITE_FRAME(gSpriteBank45Frame043, SPRITE_TILES_BANK45_FRAME043),
    { { -21, 4, 38, 37 } },
};
const struct sprite_frame_1box gSpriteBank45Frame044 = {
    SPRITE_FRAME(gSpriteBank45Frame044, SPRITE_TILES_BANK45_FRAME044),
    { { -21, 4, 38, 37 } },
};
const struct sprite_frame_1box gSpriteBank45Frame045 = {
    SPRITE_FRAME(gSpriteBank45Frame045, SPRITE_TILES_BANK45_FRAME045),
    { { -23, 5, 40, 36 } },
};
const struct sprite_frame_1box gSpriteBank45Frame046 = {
    SPRITE_FRAME(gSpriteBank45Frame046, SPRITE_TILES_BANK45_FRAME046),
    { { -23, 5, 40, 36 } },
};
const struct sprite_frame_1box gSpriteBank45Frame047 = {
    SPRITE_FRAME(gSpriteBank45Frame047, SPRITE_TILES_BANK45_FRAME047),
    { { -23, 5, 40, 36 } },
};
const struct sprite_frame_1box gSpriteBank45Frame048 = {
    SPRITE_FRAME(gSpriteBank45Frame048, SPRITE_TILES_BANK45_FRAME048),
    { { -23, 5, 40, 36 } },
};
const struct sprite_frame_1box gSpriteBank45Frame049 = {
    SPRITE_FRAME(gSpriteBank45Frame049, SPRITE_TILES_BANK45_FRAME049),
    { { -23, 5, 40, 36 } },
};
const struct sprite_frame_1box gSpriteBank45Frame050 = {
    SPRITE_FRAME(gSpriteBank45Frame050, SPRITE_TILES_BANK45_FRAME050),
    { { -23, 5, 40, 36 } },
};
const struct sprite_frame_1box gSpriteBank45Frame051 = {
    SPRITE_FRAME(gSpriteBank45Frame051, SPRITE_TILES_BANK45_FRAME051),
    { { -23, 5, 40, 36 } },
};
const struct sprite_frame_1box gSpriteBank45Frame052 = {
    SPRITE_FRAME(gSpriteBank45Frame052, SPRITE_TILES_BANK45_FRAME052),
    { { -8, -31, 26, 74 } },
};
const struct sprite_frame_1box gSpriteBank45Frame053 = {
    SPRITE_FRAME(gSpriteBank45Frame053, SPRITE_TILES_BANK45_FRAME053),
    { { -8, -31, 26, 74 } },
};
const struct sprite_frame_1box gSpriteBank45Frame054 = {
    SPRITE_FRAME(gSpriteBank45Frame054, SPRITE_TILES_BANK45_FRAME054),
    { { -8, -31, 26, 74 } },
};
const struct sprite_frame_1box gSpriteBank45Frame055 = {
    SPRITE_FRAME(gSpriteBank45Frame055, SPRITE_TILES_BANK45_FRAME055),
    { { -8, -31, 26, 74 } },
};
const struct sprite_frame_1box gSpriteBank45Frame056 = {
    SPRITE_FRAME(gSpriteBank45Frame056, SPRITE_TILES_BANK45_FRAME056),
    { { -8, -31, 26, 74 } },
};
const struct sprite_frame_1box gSpriteBank45Frame057 = {
    SPRITE_FRAME(gSpriteBank45Frame057, SPRITE_TILES_BANK45_FRAME057),
    { { -8, -31, 26, 74 } },
};
const struct sprite_frame_1box gSpriteBank45Frame058 = {
    SPRITE_FRAME(gSpriteBank45Frame058, SPRITE_TILES_BANK45_FRAME058),
    { { -8, -31, 26, 74 } },
};
const struct sprite_frame_1box gSpriteBank45Frame059 = {
    SPRITE_FRAME(gSpriteBank45Frame059, SPRITE_TILES_BANK45_FRAME059),
    { { -8, -31, 26, 74 } },
};
const struct sprite_frame_1box gSpriteBank45Frame060 = {
    SPRITE_FRAME(gSpriteBank45Frame060, SPRITE_TILES_BANK45_FRAME060),
    { { -8, -31, 26, 74 } },
};
const struct sprite_frame_1box gSpriteBank45Frame061 = {
    SPRITE_FRAME(gSpriteBank45Frame061, SPRITE_TILES_BANK45_FRAME061),
    { { -8, -31, 26, 74 } },
};
const struct sprite_frame_1box gSpriteBank45Frame062 = {
    SPRITE_FRAME(gSpriteBank45Frame062, SPRITE_TILES_BANK45_FRAME062),
    { { -8, -31, 26, 74 } },
};
const struct sprite_frame_1box gSpriteBank45Frame063 = {
    SPRITE_FRAME(gSpriteBank45Frame063, SPRITE_TILES_BANK45_FRAME063),
    { { -8, -31, 26, 74 } },
};
const struct sprite_frame_1box gSpriteBank45Frame064 = {
    SPRITE_FRAME(gSpriteBank45Frame064, SPRITE_TILES_BANK45_FRAME064),
    { { -9, -31, 28, 70 } },
};
const struct sprite_frame_1box gSpriteBank45Frame065 = {
    SPRITE_FRAME(gSpriteBank45Frame065, SPRITE_TILES_BANK45_FRAME065),
    { { -9, -31, 28, 70 } },
};
const struct sprite_frame_1box gSpriteBank45Frame066 = {
    SPRITE_FRAME(gSpriteBank45Frame066, SPRITE_TILES_BANK45_FRAME066),
    { { -9, -31, 28, 70 } },
};
const struct sprite_frame_1box gSpriteBank45Frame067 = {
    SPRITE_FRAME(gSpriteBank45Frame067, SPRITE_TILES_BANK45_FRAME067),
    { { -9, -31, 28, 70 } },
};
const struct sprite_frame_1box gSpriteBank45Frame068 = {
    SPRITE_FRAME(gSpriteBank45Frame068, SPRITE_TILES_BANK45_FRAME068),
    { { -9, -31, 28, 70 } },
};
const struct sprite_frame_1box gSpriteBank45Frame069 = {
    SPRITE_FRAME(gSpriteBank45Frame069, SPRITE_TILES_BANK45_FRAME069),
    { { -9, -31, 28, 70 } },
};
const struct sprite_frame_1box gSpriteBank45Frame070 = {
    SPRITE_FRAME(gSpriteBank45Frame070, SPRITE_TILES_BANK45_FRAME070),
    { { -9, -31, 28, 70 } },
};
const struct sprite_frame_1box gSpriteBank45Frame071 = {
    SPRITE_FRAME(gSpriteBank45Frame071, SPRITE_TILES_BANK45_FRAME071),
    { { -9, -31, 28, 70 } },
};
const struct sprite_frame gSpriteBank45Frame072 = SPRITE_FRAME(gSpriteBank45Frame072, SPRITE_TILES_BANK45_FRAME072);
const struct sprite_frame gSpriteBank45Frame073 = SPRITE_FRAME(gSpriteBank45Frame073, SPRITE_TILES_BANK45_FRAME073);
const struct sprite_frame gSpriteBank45Frame074 = SPRITE_FRAME(gSpriteBank45Frame074, SPRITE_TILES_BANK45_FRAME074);
const struct sprite_frame gSpriteBank45Frame075 = SPRITE_FRAME(gSpriteBank45Frame075, SPRITE_TILES_BANK45_FRAME075);
const struct sprite_frame gSpriteBank45Frame076 = SPRITE_FRAME(gSpriteBank45Frame076, SPRITE_TILES_BANK45_FRAME076);
const struct sprite_frame gSpriteBank45Frame077 = SPRITE_FRAME(gSpriteBank45Frame077, SPRITE_TILES_BANK45_FRAME077);
const struct sprite_frame gSpriteBank45Frame078 = SPRITE_FRAME(gSpriteBank45Frame078, SPRITE_TILES_BANK45_FRAME078);
const struct sprite_frame gSpriteBank45Frame079 = SPRITE_FRAME(gSpriteBank45Frame079, SPRITE_TILES_BANK45_FRAME079);
const struct sprite_frame gSpriteBank45Frame080 = SPRITE_FRAME(gSpriteBank45Frame080, SPRITE_TILES_BANK45_FRAME080);
const struct sprite_frame gSpriteBank45Frame081 = SPRITE_FRAME(gSpriteBank45Frame081, SPRITE_TILES_BANK45_FRAME081);
const struct sprite_frame gSpriteBank45Frame082 = SPRITE_FRAME(gSpriteBank45Frame082, SPRITE_TILES_BANK45_FRAME082);
const struct sprite_frame_1box gSpriteBank45Frame083 = {
    SPRITE_FRAME(gSpriteBank45Frame083, SPRITE_TILES_BANK45_FRAME083),
    { { -8, -28, 28, 66 } },
};
const struct sprite_frame_1box gSpriteBank45Frame084 = {
    SPRITE_FRAME(gSpriteBank45Frame084, SPRITE_TILES_BANK45_FRAME084),
    { { -8, -28, 28, 66 } },
};
const struct sprite_frame_1box gSpriteBank45Frame085 = {
    SPRITE_FRAME(gSpriteBank45Frame085, SPRITE_TILES_BANK45_FRAME085),
    { { -8, -28, 28, 66 } },
};
const struct sprite_frame_1box gSpriteBank45Frame086 = {
    SPRITE_FRAME(gSpriteBank45Frame086, SPRITE_TILES_BANK45_FRAME086),
    { { -8, -28, 28, 66 } },
};
const struct sprite_frame_1box gSpriteBank45Frame087 = {
    SPRITE_FRAME(gSpriteBank45Frame087, SPRITE_TILES_BANK45_FRAME087),
    { { -8, -28, 28, 66 } },
};
const struct sprite_frame_1box gSpriteBank45Frame088 = {
    SPRITE_FRAME(gSpriteBank45Frame088, SPRITE_TILES_BANK45_FRAME088),
    { { -8, -28, 28, 66 } },
};
const struct sprite_frame_1box gSpriteBank45Frame089 = {
    SPRITE_FRAME(gSpriteBank45Frame089, SPRITE_TILES_BANK45_FRAME089),
    { { -8, -28, 28, 66 } },
};
const struct sprite_frame_1box gSpriteBank45Frame090 = {
    SPRITE_FRAME(gSpriteBank45Frame090, SPRITE_TILES_BANK45_FRAME090),
    { { -8, -28, 28, 66 } },
};
const struct sprite_frame_1box gSpriteBank45Frame091 = {
    SPRITE_FRAME(gSpriteBank45Frame091, SPRITE_TILES_BANK45_FRAME091),
    { { -4, -30, 30, 69 } },
};
const struct sprite_frame_1box gSpriteBank45Frame092 = {
    SPRITE_FRAME(gSpriteBank45Frame092, SPRITE_TILES_BANK45_FRAME092),
    { { -4, -30, 30, 69 } },
};
const struct sprite_frame_1box gSpriteBank45Frame093 = {
    SPRITE_FRAME(gSpriteBank45Frame093, SPRITE_TILES_BANK45_FRAME093),
    { { -4, -30, 30, 69 } },
};
const struct sprite_frame_1box gSpriteBank45Frame094 = {
    SPRITE_FRAME(gSpriteBank45Frame094, SPRITE_TILES_BANK45_FRAME094),
    { { -4, -30, 30, 69 } },
};
const struct sprite_frame_1box gSpriteBank45Frame095 = {
    SPRITE_FRAME(gSpriteBank45Frame095, SPRITE_TILES_BANK45_FRAME095),
    { { -4, -30, 30, 69 } },
};
const struct sprite_frame_1box gSpriteBank45Frame096 = {
    SPRITE_FRAME(gSpriteBank45Frame096, SPRITE_TILES_BANK45_FRAME096),
    { { -4, -30, 30, 69 } },
};
const struct sprite_frame_1box gSpriteBank45Frame097 = {
    SPRITE_FRAME(gSpriteBank45Frame097, SPRITE_TILES_BANK45_FRAME097),
    { { -4, -30, 30, 69 } },
};
const struct sprite_frame_1box gSpriteBank45Frame098 = {
    SPRITE_FRAME(gSpriteBank45Frame098, SPRITE_TILES_BANK45_FRAME098),
    { { -4, -30, 30, 69 } },
};
const struct sprite_frame_1box gSpriteBank45Frame099 = {
    SPRITE_FRAME(gSpriteBank45Frame099, SPRITE_TILES_BANK45_FRAME099),
    { { -7, -30, 26, 69 } },
};
const struct sprite_frame_1box gSpriteBank45Frame100 = {
    SPRITE_FRAME(gSpriteBank45Frame100, SPRITE_TILES_BANK45_FRAME100),
    { { -7, -30, 26, 69 } },
};
const struct sprite_frame_1box gSpriteBank45Frame101 = {
    SPRITE_FRAME(gSpriteBank45Frame101, SPRITE_TILES_BANK45_FRAME101),
    { { -7, -30, 26, 69 } },
};
const struct sprite_frame_1box gSpriteBank45Frame102 = {
    SPRITE_FRAME(gSpriteBank45Frame102, SPRITE_TILES_BANK45_FRAME102),
    { { -7, -30, 26, 69 } },
};
const struct sprite_frame_1box gSpriteBank45Frame103 = {
    SPRITE_FRAME(gSpriteBank45Frame103, SPRITE_TILES_BANK45_FRAME103),
    { { -7, -30, 26, 69 } },
};
const struct sprite_frame_1box gSpriteBank45Frame104 = {
    SPRITE_FRAME(gSpriteBank45Frame104, SPRITE_TILES_BANK45_FRAME104),
    { { -7, -30, 26, 69 } },
};
const struct sprite_frame_1box gSpriteBank45Frame105 = {
    SPRITE_FRAME(gSpriteBank45Frame105, SPRITE_TILES_BANK45_FRAME105),
    { { -7, -30, 26, 69 } },
};
const struct sprite_frame_1box gSpriteBank45Frame106 = {
    SPRITE_FRAME(gSpriteBank45Frame106, SPRITE_TILES_BANK45_FRAME106),
    { { -7, -30, 26, 69 } },
};
const struct sprite_frame gSpriteBank45Frame107 = SPRITE_FRAME(gSpriteBank45Frame107, SPRITE_TILES_BANK45_FRAME107);
const struct sprite_frame gSpriteBank45Frame108 = SPRITE_FRAME(gSpriteBank45Frame108, SPRITE_TILES_BANK45_FRAME108);
const struct sprite_frame gSpriteBank45Frame109 = SPRITE_FRAME(gSpriteBank45Frame109, SPRITE_TILES_BANK45_FRAME109);
const struct sprite_frame gSpriteBank45Frame110 = SPRITE_FRAME(gSpriteBank45Frame110, SPRITE_TILES_BANK45_FRAME110);
const struct sprite_frame gSpriteBank45Frame111 = SPRITE_FRAME(gSpriteBank45Frame111, SPRITE_TILES_BANK45_FRAME111);
const struct sprite_frame gSpriteBank45Frame112 = SPRITE_FRAME(gSpriteBank45Frame112, SPRITE_TILES_BANK45_FRAME112);
const struct sprite_frame gSpriteBank45Frame113 = SPRITE_FRAME(gSpriteBank45Frame113, SPRITE_TILES_BANK45_FRAME113);
const struct sprite_frame gSpriteBank45Frame114 = SPRITE_FRAME(gSpriteBank45Frame114, SPRITE_TILES_BANK45_FRAME114);
const struct sprite_frame gSpriteBank45Frame115 = SPRITE_FRAME(gSpriteBank45Frame115, SPRITE_TILES_BANK45_FRAME115);
const struct sprite_frame gSpriteBank45Frame116 = SPRITE_FRAME(gSpriteBank45Frame116, SPRITE_TILES_BANK45_FRAME116);
const struct sprite_frame_1box gSpriteBank45Frame117 = {
    SPRITE_FRAME(gSpriteBank45Frame117, SPRITE_TILES_BANK45_FRAME117),
    { { -7, -31, 26, 71 } },
};
const struct sprite_frame_1box gSpriteBank45Frame118 = {
    SPRITE_FRAME(gSpriteBank45Frame118, SPRITE_TILES_BANK45_FRAME118),
    { { -7, -31, 26, 71 } },
};
const struct sprite_frame_1box gSpriteBank45Frame119 = {
    SPRITE_FRAME(gSpriteBank45Frame119, SPRITE_TILES_BANK45_FRAME119),
    { { -7, -31, 26, 71 } },
};
const struct sprite_frame_1box gSpriteBank45Frame120 = {
    SPRITE_FRAME(gSpriteBank45Frame120, SPRITE_TILES_BANK45_FRAME120),
    { { -7, -31, 26, 71 } },
};
const struct sprite_frame_1box gSpriteBank45Frame121 = {
    SPRITE_FRAME(gSpriteBank45Frame121, SPRITE_TILES_BANK45_FRAME121),
    { { -7, -31, 26, 71 } },
};
const struct sprite_frame_1box gSpriteBank45Frame122 = {
    SPRITE_FRAME(gSpriteBank45Frame122, SPRITE_TILES_BANK45_FRAME122),
    { { -7, -31, 26, 71 } },
};
const struct sprite_frame_1box gSpriteBank45Frame123 = {
    SPRITE_FRAME(gSpriteBank45Frame123, SPRITE_TILES_BANK45_FRAME123),
    { { -7, -31, 26, 71 } },
};
const struct sprite_frame_1box gSpriteBank45Frame124 = {
    SPRITE_FRAME(gSpriteBank45Frame124, SPRITE_TILES_BANK45_FRAME124),
    { { -7, -31, 26, 71 } },
};
const struct sprite_frame_1box gSpriteBank45Frame125 = {
    SPRITE_FRAME(gSpriteBank45Frame125, SPRITE_TILES_BANK45_FRAME125),
    { { -7, -31, 26, 71 } },
};
const struct sprite_frame gSpriteBank45Frame126 = SPRITE_FRAME(gSpriteBank45Frame126, SPRITE_TILES_BANK45_FRAME126);
const struct sprite_frame gSpriteBank45Frame127 = SPRITE_FRAME(gSpriteBank45Frame127, SPRITE_TILES_BANK45_FRAME127);
const struct sprite_frame gSpriteBank45Frame128 = SPRITE_FRAME(gSpriteBank45Frame128, SPRITE_TILES_BANK45_FRAME128);
const struct sprite_frame gSpriteBank45Frame129 = SPRITE_FRAME(gSpriteBank45Frame129, SPRITE_TILES_BANK45_FRAME129);
const struct sprite_frame gSpriteBank45Frame130 = SPRITE_FRAME(gSpriteBank45Frame130, SPRITE_TILES_BANK45_FRAME130);
const struct sprite_frame gSpriteBank45Frame131 = SPRITE_FRAME(gSpriteBank45Frame131, SPRITE_TILES_BANK45_FRAME131);
const struct sprite_frame_1box gSpriteBank45Frame132 = {
    SPRITE_FRAME(gSpriteBank45Frame132, SPRITE_TILES_BANK45_FRAME132),
    { { -26, -6, 52, 12 } },
};
const struct sprite_frame_1box gSpriteBank45Frame133 = {
    SPRITE_FRAME(gSpriteBank45Frame133, SPRITE_TILES_BANK45_FRAME133),
    { { -26, -6, 52, 12 } },
};
const struct sprite_frame_1box gSpriteBank45Frame134 = {
    SPRITE_FRAME(gSpriteBank45Frame134, SPRITE_TILES_BANK45_FRAME134),
    { { -26, -6, 52, 12 } },
};
const struct sprite_frame_1box gSpriteBank45Frame135 = {
    SPRITE_FRAME(gSpriteBank45Frame135, SPRITE_TILES_BANK45_FRAME135),
    { { -8, -7, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank45Frame136 = {
    SPRITE_FRAME(gSpriteBank45Frame136, SPRITE_TILES_BANK45_FRAME136),
    { { -8, -7, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank45Frame137 = {
    SPRITE_FRAME(gSpriteBank45Frame137, SPRITE_TILES_BANK45_FRAME137),
    { { -8, -7, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank45Frame138 = {
    SPRITE_FRAME(gSpriteBank45Frame138, SPRITE_TILES_BANK45_FRAME138),
    { { -8, -7, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank45Frame139 = {
    SPRITE_FRAME(gSpriteBank45Frame139, SPRITE_TILES_BANK45_FRAME139),
    { { -8, -7, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank45Frame140 = {
    SPRITE_FRAME(gSpriteBank45Frame140, SPRITE_TILES_BANK45_FRAME140),
    { { -8, -7, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank45Frame141 = {
    SPRITE_FRAME(gSpriteBank45Frame141, SPRITE_TILES_BANK45_FRAME141),
    { { -8, -7, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank45Frame142 = {
    SPRITE_FRAME(gSpriteBank45Frame142, SPRITE_TILES_BANK45_FRAME142),
    { { -8, -7, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank45Frame143 = {
    SPRITE_FRAME(gSpriteBank45Frame143, SPRITE_TILES_BANK45_FRAME143),
    { { -2, -30, 28, 68 } },
};
const struct sprite_frame_1box gSpriteBank45Frame144 = {
    SPRITE_FRAME(gSpriteBank45Frame144, SPRITE_TILES_BANK45_FRAME144),
    { { -2, -30, 28, 68 } },
};
const struct sprite_frame_1box gSpriteBank45Frame145 = {
    SPRITE_FRAME(gSpriteBank45Frame145, SPRITE_TILES_BANK45_FRAME145),
    { { -2, -30, 28, 68 } },
};
const struct sprite_frame_1box gSpriteBank45Frame146 = {
    SPRITE_FRAME(gSpriteBank45Frame146, SPRITE_TILES_BANK45_FRAME146),
    { { -2, -30, 28, 68 } },
};
const struct sprite_frame_1box gSpriteBank45Frame147 = {
    SPRITE_FRAME(gSpriteBank45Frame147, SPRITE_TILES_BANK45_FRAME147),
    { { -2, -30, 28, 68 } },
};
const struct sprite_frame_1box gSpriteBank45Frame148 = {
    SPRITE_FRAME(gSpriteBank45Frame148, SPRITE_TILES_BANK45_FRAME148),
    { { -2, -30, 28, 68 } },
};
const struct sprite_frame_1box gSpriteBank45Frame149 = {
    SPRITE_FRAME(gSpriteBank45Frame149, SPRITE_TILES_BANK45_FRAME149),
    { { -2, -30, 28, 68 } },
};
const struct sprite_frame_1box gSpriteBank45Frame150 = {
    SPRITE_FRAME(gSpriteBank45Frame150, SPRITE_TILES_BANK45_FRAME150),
    { { -2, -30, 28, 68 } },
};
const struct sprite_frame_1box gSpriteBank45Frame151 = {
    SPRITE_FRAME(gSpriteBank45Frame151, SPRITE_TILES_BANK45_FRAME151),
    { { -2, -30, 28, 68 } },
};
const struct sprite_frame_1box gSpriteBank45Frame152 = {
    SPRITE_FRAME(gSpriteBank45Frame152, SPRITE_TILES_BANK45_FRAME152),
    { { -2, -30, 28, 68 } },
};
const struct sprite_frame_1box gSpriteBank45Frame153 = {
    SPRITE_FRAME(gSpriteBank45Frame153, SPRITE_TILES_BANK45_FRAME153),
    { { -2, -30, 28, 68 } },
};
const struct sprite_frame_1box gSpriteBank45Frame154 = {
    SPRITE_FRAME(gSpriteBank45Frame154, SPRITE_TILES_BANK45_FRAME154),
    { { -2, -30, 28, 68 } },
};
const struct sprite_frame_1box gSpriteBank45Frame155 = {
    SPRITE_FRAME(gSpriteBank45Frame155, SPRITE_TILES_BANK45_FRAME155),
    { { -7, -30, 28, 66 } },
};
const struct sprite_frame_1box gSpriteBank45Frame156 = {
    SPRITE_FRAME(gSpriteBank45Frame156, SPRITE_TILES_BANK45_FRAME156),
    { { -7, -30, 28, 66 } },
};
const struct sprite_frame_1box gSpriteBank45Frame157 = {
    SPRITE_FRAME(gSpriteBank45Frame157, SPRITE_TILES_BANK45_FRAME157),
    { { -7, -30, 28, 66 } },
};
const struct sprite_frame_1box gSpriteBank45Frame158 = {
    SPRITE_FRAME(gSpriteBank45Frame158, SPRITE_TILES_BANK45_FRAME158),
    { { -7, -30, 28, 66 } },
};
const struct sprite_frame_1box gSpriteBank45Frame159 = {
    SPRITE_FRAME(gSpriteBank45Frame159, SPRITE_TILES_BANK45_FRAME159),
    { { -7, -30, 28, 66 } },
};
const struct sprite_frame_1box gSpriteBank45Frame160 = {
    SPRITE_FRAME(gSpriteBank45Frame160, SPRITE_TILES_BANK45_FRAME160),
    { { -7, -30, 28, 66 } },
};
const struct sprite_frame_1box gSpriteBank45Frame161 = {
    SPRITE_FRAME(gSpriteBank45Frame161, SPRITE_TILES_BANK45_FRAME161),
    { { -7, -30, 28, 66 } },
};
const struct sprite_frame_1box gSpriteBank45Frame162 = {
    SPRITE_FRAME(gSpriteBank45Frame162, SPRITE_TILES_BANK45_FRAME162),
    { { -7, -30, 28, 66 } },
};

/* The piece arrays, from games/n2/graphics/sprites/bank45_n_tropy/bank.json. */
#include "sprites/bank45_pieces.inc"

/* ---------------------------------------------------------------------- */
/* Bank 46: 5 animations, 88 frames, games/n2/graphics/sprites/bank46_crash_extra/. */

extern const u16 gSpriteBank46Anim00Seq[10];
extern const u16 gSpriteBank46Anim01Seq[20];
extern const u16 gSpriteBank46Anim02Seq[20];
extern const u16 gSpriteBank46Anim03Seq[20];
extern const u16 gSpriteBank46Anim04Seq[18];

extern const struct sprite_anim gSpriteBank46Anims[5] = {
    /* 0 */ {
        /* seq */ gSpriteBank46Anim00Seq,
        /* box */ { { -6, 9, 12, 10 }, { -11, -20, 22, 38 } },
        /* paletteId */ 1,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank46Anim00Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 1 */ {
        /* seq */ gSpriteBank46Anim01Seq,
        /* box */ { { -6, 9, 12, 10 }, { -18, -26, 29, 49 } },
        /* paletteId */ 0,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank46Anim01Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 2 */ {
        /* seq */ gSpriteBank46Anim02Seq,
        /* box */ { { -6, 9, 12, 10 }, { -19, -25, 30, 49 } },
        /* paletteId */ 0,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank46Anim02Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 3 */ {
        /* seq */ gSpriteBank46Anim03Seq,
        /* box */ { { -6, 9, 12, 10 }, { -22, -31, 41, 48 } },
        /* paletteId */ 0,
        /* duration */ 1,
        /* frameCount */ ARRAY_COUNT(gSpriteBank46Anim03Seq),
        /* flags */ SPRITE_ANIM_LOOP,
    },
    /* 4 */ {
        /* seq */ gSpriteBank46Anim04Seq,
        /* box */ { { -6, 9, 12, 10 }, { -18, -25, 33, 42 } },
        /* paletteId */ 0,
        /* duration */ 3,
        /* frameCount */ ARRAY_COUNT(gSpriteBank46Anim04Seq),
        /* flags */ 0,
    },
};

const u16 gSpriteBank46Anim00Seq[10] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
};
const u16 gSpriteBank46Anim01Seq[20] = {
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
    26, 27, 28, 29,
};
const u16 gSpriteBank46Anim02Seq[20] = {
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45,
    46, 47, 48, 49,
};
const u16 gSpriteBank46Anim03Seq[20] = {
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
    66, 67, 68, 69,
};
const u16 gSpriteBank46Anim04Seq[18] = {
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85,
    86, 87,
};

extern const struct sprite_frame *const gSpriteBank46Frames[88] = {
    &gSpriteBank46Frame000,
    &gSpriteBank46Frame001,
    &gSpriteBank46Frame002,
    &gSpriteBank46Frame003,
    &gSpriteBank46Frame004,
    &gSpriteBank46Frame005,
    &gSpriteBank46Frame006,
    &gSpriteBank46Frame007,
    &gSpriteBank46Frame008,
    &gSpriteBank46Frame009,
    &gSpriteBank46Frame010,
    &gSpriteBank46Frame011,
    &gSpriteBank46Frame012,
    &gSpriteBank46Frame013,
    &gSpriteBank46Frame014,
    &gSpriteBank46Frame015,
    &gSpriteBank46Frame016,
    &gSpriteBank46Frame017,
    &gSpriteBank46Frame018,
    &gSpriteBank46Frame019,
    &gSpriteBank46Frame020,
    &gSpriteBank46Frame021,
    &gSpriteBank46Frame022,
    &gSpriteBank46Frame023,
    &gSpriteBank46Frame024,
    &gSpriteBank46Frame025,
    &gSpriteBank46Frame026,
    &gSpriteBank46Frame027,
    &gSpriteBank46Frame028,
    &gSpriteBank46Frame029,
    &gSpriteBank46Frame030,
    &gSpriteBank46Frame031,
    &gSpriteBank46Frame032,
    &gSpriteBank46Frame033,
    &gSpriteBank46Frame034,
    &gSpriteBank46Frame035,
    &gSpriteBank46Frame036,
    &gSpriteBank46Frame037,
    &gSpriteBank46Frame038,
    &gSpriteBank46Frame039,
    &gSpriteBank46Frame040,
    &gSpriteBank46Frame041,
    &gSpriteBank46Frame042,
    &gSpriteBank46Frame043,
    &gSpriteBank46Frame044,
    &gSpriteBank46Frame045,
    &gSpriteBank46Frame046,
    &gSpriteBank46Frame047,
    &gSpriteBank46Frame048,
    &gSpriteBank46Frame049,
    &gSpriteBank46Frame050,
    &gSpriteBank46Frame051,
    &gSpriteBank46Frame052,
    &gSpriteBank46Frame053,
    &gSpriteBank46Frame054,
    &gSpriteBank46Frame055,
    &gSpriteBank46Frame056,
    &gSpriteBank46Frame057,
    &gSpriteBank46Frame058,
    &gSpriteBank46Frame059,
    &gSpriteBank46Frame060,
    &gSpriteBank46Frame061,
    &gSpriteBank46Frame062,
    &gSpriteBank46Frame063,
    &gSpriteBank46Frame064,
    &gSpriteBank46Frame065,
    &gSpriteBank46Frame066,
    &gSpriteBank46Frame067,
    &gSpriteBank46Frame068,
    &gSpriteBank46Frame069,
    &gSpriteBank46Frame070,
    &gSpriteBank46Frame071,
    &gSpriteBank46Frame072,
    &gSpriteBank46Frame073,
    &gSpriteBank46Frame074,
    &gSpriteBank46Frame075,
    &gSpriteBank46Frame076,
    &gSpriteBank46Frame077,
    &gSpriteBank46Frame078,
    &gSpriteBank46Frame079,
    &gSpriteBank46Frame080,
    &gSpriteBank46Frame081,
    &gSpriteBank46Frame082,
    &gSpriteBank46Frame083,
    &gSpriteBank46Frame084,
    &gSpriteBank46Frame085,
    &gSpriteBank46Frame086,
    &gSpriteBank46Frame087,
};

const struct sprite_frame gSpriteBank46Frame000 = SPRITE_FRAME(gSpriteBank46Frame000, SPRITE_TILES_BANK46_FRAME000);
const struct sprite_frame gSpriteBank46Frame001 = SPRITE_FRAME(gSpriteBank46Frame001, SPRITE_TILES_BANK46_FRAME001);
const struct sprite_frame gSpriteBank46Frame002 = SPRITE_FRAME(gSpriteBank46Frame002, SPRITE_TILES_BANK46_FRAME002);
const struct sprite_frame gSpriteBank46Frame003 = SPRITE_FRAME(gSpriteBank46Frame003, SPRITE_TILES_BANK46_FRAME003);
const struct sprite_frame gSpriteBank46Frame004 = SPRITE_FRAME(gSpriteBank46Frame004, SPRITE_TILES_BANK46_FRAME004);
const struct sprite_frame gSpriteBank46Frame005 = SPRITE_FRAME(gSpriteBank46Frame005, SPRITE_TILES_BANK46_FRAME005);
const struct sprite_frame gSpriteBank46Frame006 = SPRITE_FRAME(gSpriteBank46Frame006, SPRITE_TILES_BANK46_FRAME006);
const struct sprite_frame gSpriteBank46Frame007 = SPRITE_FRAME(gSpriteBank46Frame007, SPRITE_TILES_BANK46_FRAME007);
const struct sprite_frame gSpriteBank46Frame008 = SPRITE_FRAME(gSpriteBank46Frame008, SPRITE_TILES_BANK46_FRAME008);
const struct sprite_frame gSpriteBank46Frame009 = SPRITE_FRAME(gSpriteBank46Frame009, SPRITE_TILES_BANK46_FRAME009);
const struct sprite_frame gSpriteBank46Frame010 = SPRITE_FRAME(gSpriteBank46Frame010, SPRITE_TILES_BANK46_FRAME010);
const struct sprite_frame gSpriteBank46Frame011 = SPRITE_FRAME(gSpriteBank46Frame011, SPRITE_TILES_BANK46_FRAME011);
const struct sprite_frame gSpriteBank46Frame012 = SPRITE_FRAME(gSpriteBank46Frame012, SPRITE_TILES_BANK46_FRAME012);
const struct sprite_frame gSpriteBank46Frame013 = SPRITE_FRAME(gSpriteBank46Frame013, SPRITE_TILES_BANK46_FRAME013);
const struct sprite_frame gSpriteBank46Frame014 = SPRITE_FRAME(gSpriteBank46Frame014, SPRITE_TILES_BANK46_FRAME014);
const struct sprite_frame gSpriteBank46Frame015 = SPRITE_FRAME(gSpriteBank46Frame015, SPRITE_TILES_BANK46_FRAME015);
const struct sprite_frame gSpriteBank46Frame016 = SPRITE_FRAME(gSpriteBank46Frame016, SPRITE_TILES_BANK46_FRAME016);
const struct sprite_frame gSpriteBank46Frame017 = SPRITE_FRAME(gSpriteBank46Frame017, SPRITE_TILES_BANK46_FRAME017);
const struct sprite_frame gSpriteBank46Frame018 = SPRITE_FRAME(gSpriteBank46Frame018, SPRITE_TILES_BANK46_FRAME018);
const struct sprite_frame gSpriteBank46Frame019 = SPRITE_FRAME(gSpriteBank46Frame019, SPRITE_TILES_BANK46_FRAME019);
const struct sprite_frame gSpriteBank46Frame020 = SPRITE_FRAME(gSpriteBank46Frame020, SPRITE_TILES_BANK46_FRAME020);
const struct sprite_frame gSpriteBank46Frame021 = SPRITE_FRAME(gSpriteBank46Frame021, SPRITE_TILES_BANK46_FRAME021);
const struct sprite_frame gSpriteBank46Frame022 = SPRITE_FRAME(gSpriteBank46Frame022, SPRITE_TILES_BANK46_FRAME022);
const struct sprite_frame gSpriteBank46Frame023 = SPRITE_FRAME(gSpriteBank46Frame023, SPRITE_TILES_BANK46_FRAME023);
const struct sprite_frame gSpriteBank46Frame024 = SPRITE_FRAME(gSpriteBank46Frame024, SPRITE_TILES_BANK46_FRAME024);
const struct sprite_frame gSpriteBank46Frame025 = SPRITE_FRAME(gSpriteBank46Frame025, SPRITE_TILES_BANK46_FRAME025);
const struct sprite_frame gSpriteBank46Frame026 = SPRITE_FRAME(gSpriteBank46Frame026, SPRITE_TILES_BANK46_FRAME026);
const struct sprite_frame gSpriteBank46Frame027 = SPRITE_FRAME(gSpriteBank46Frame027, SPRITE_TILES_BANK46_FRAME027);
const struct sprite_frame gSpriteBank46Frame028 = SPRITE_FRAME(gSpriteBank46Frame028, SPRITE_TILES_BANK46_FRAME028);
const struct sprite_frame gSpriteBank46Frame029 = SPRITE_FRAME(gSpriteBank46Frame029, SPRITE_TILES_BANK46_FRAME029);
const struct sprite_frame gSpriteBank46Frame030 = SPRITE_FRAME(gSpriteBank46Frame030, SPRITE_TILES_BANK46_FRAME030);
const struct sprite_frame gSpriteBank46Frame031 = SPRITE_FRAME(gSpriteBank46Frame031, SPRITE_TILES_BANK46_FRAME031);
const struct sprite_frame gSpriteBank46Frame032 = SPRITE_FRAME(gSpriteBank46Frame032, SPRITE_TILES_BANK46_FRAME032);
const struct sprite_frame gSpriteBank46Frame033 = SPRITE_FRAME(gSpriteBank46Frame033, SPRITE_TILES_BANK46_FRAME033);
const struct sprite_frame gSpriteBank46Frame034 = SPRITE_FRAME(gSpriteBank46Frame034, SPRITE_TILES_BANK46_FRAME034);
const struct sprite_frame gSpriteBank46Frame035 = SPRITE_FRAME(gSpriteBank46Frame035, SPRITE_TILES_BANK46_FRAME035);
const struct sprite_frame gSpriteBank46Frame036 = SPRITE_FRAME(gSpriteBank46Frame036, SPRITE_TILES_BANK46_FRAME036);
const struct sprite_frame gSpriteBank46Frame037 = SPRITE_FRAME(gSpriteBank46Frame037, SPRITE_TILES_BANK46_FRAME037);
const struct sprite_frame gSpriteBank46Frame038 = SPRITE_FRAME(gSpriteBank46Frame038, SPRITE_TILES_BANK46_FRAME038);
const struct sprite_frame gSpriteBank46Frame039 = SPRITE_FRAME(gSpriteBank46Frame039, SPRITE_TILES_BANK46_FRAME039);
const struct sprite_frame gSpriteBank46Frame040 = SPRITE_FRAME(gSpriteBank46Frame040, SPRITE_TILES_BANK46_FRAME040);
const struct sprite_frame gSpriteBank46Frame041 = SPRITE_FRAME(gSpriteBank46Frame041, SPRITE_TILES_BANK46_FRAME041);
const struct sprite_frame gSpriteBank46Frame042 = SPRITE_FRAME(gSpriteBank46Frame042, SPRITE_TILES_BANK46_FRAME042);
const struct sprite_frame gSpriteBank46Frame043 = SPRITE_FRAME(gSpriteBank46Frame043, SPRITE_TILES_BANK46_FRAME043);
const struct sprite_frame gSpriteBank46Frame044 = SPRITE_FRAME(gSpriteBank46Frame044, SPRITE_TILES_BANK46_FRAME044);
const struct sprite_frame gSpriteBank46Frame045 = SPRITE_FRAME(gSpriteBank46Frame045, SPRITE_TILES_BANK46_FRAME045);
const struct sprite_frame gSpriteBank46Frame046 = SPRITE_FRAME(gSpriteBank46Frame046, SPRITE_TILES_BANK46_FRAME046);
const struct sprite_frame gSpriteBank46Frame047 = SPRITE_FRAME(gSpriteBank46Frame047, SPRITE_TILES_BANK46_FRAME047);
const struct sprite_frame gSpriteBank46Frame048 = SPRITE_FRAME(gSpriteBank46Frame048, SPRITE_TILES_BANK46_FRAME048);
const struct sprite_frame gSpriteBank46Frame049 = SPRITE_FRAME(gSpriteBank46Frame049, SPRITE_TILES_BANK46_FRAME049);
const struct sprite_frame gSpriteBank46Frame050 = SPRITE_FRAME(gSpriteBank46Frame050, SPRITE_TILES_BANK46_FRAME050);
const struct sprite_frame gSpriteBank46Frame051 = SPRITE_FRAME(gSpriteBank46Frame051, SPRITE_TILES_BANK46_FRAME051);
const struct sprite_frame gSpriteBank46Frame052 = SPRITE_FRAME(gSpriteBank46Frame052, SPRITE_TILES_BANK46_FRAME052);
const struct sprite_frame gSpriteBank46Frame053 = SPRITE_FRAME(gSpriteBank46Frame053, SPRITE_TILES_BANK46_FRAME053);
const struct sprite_frame gSpriteBank46Frame054 = SPRITE_FRAME(gSpriteBank46Frame054, SPRITE_TILES_BANK46_FRAME054);
const struct sprite_frame gSpriteBank46Frame055 = SPRITE_FRAME(gSpriteBank46Frame055, SPRITE_TILES_BANK46_FRAME055);
const struct sprite_frame gSpriteBank46Frame056 = SPRITE_FRAME(gSpriteBank46Frame056, SPRITE_TILES_BANK46_FRAME056);
const struct sprite_frame gSpriteBank46Frame057 = SPRITE_FRAME(gSpriteBank46Frame057, SPRITE_TILES_BANK46_FRAME057);
const struct sprite_frame gSpriteBank46Frame058 = SPRITE_FRAME(gSpriteBank46Frame058, SPRITE_TILES_BANK46_FRAME058);
const struct sprite_frame gSpriteBank46Frame059 = SPRITE_FRAME(gSpriteBank46Frame059, SPRITE_TILES_BANK46_FRAME059);
const struct sprite_frame gSpriteBank46Frame060 = SPRITE_FRAME(gSpriteBank46Frame060, SPRITE_TILES_BANK46_FRAME060);
const struct sprite_frame gSpriteBank46Frame061 = SPRITE_FRAME(gSpriteBank46Frame061, SPRITE_TILES_BANK46_FRAME061);
const struct sprite_frame gSpriteBank46Frame062 = SPRITE_FRAME(gSpriteBank46Frame062, SPRITE_TILES_BANK46_FRAME062);
const struct sprite_frame gSpriteBank46Frame063 = SPRITE_FRAME(gSpriteBank46Frame063, SPRITE_TILES_BANK46_FRAME063);
const struct sprite_frame gSpriteBank46Frame064 = SPRITE_FRAME(gSpriteBank46Frame064, SPRITE_TILES_BANK46_FRAME064);
const struct sprite_frame gSpriteBank46Frame065 = SPRITE_FRAME(gSpriteBank46Frame065, SPRITE_TILES_BANK46_FRAME065);
const struct sprite_frame gSpriteBank46Frame066 = SPRITE_FRAME(gSpriteBank46Frame066, SPRITE_TILES_BANK46_FRAME066);
const struct sprite_frame gSpriteBank46Frame067 = SPRITE_FRAME(gSpriteBank46Frame067, SPRITE_TILES_BANK46_FRAME067);
const struct sprite_frame gSpriteBank46Frame068 = SPRITE_FRAME(gSpriteBank46Frame068, SPRITE_TILES_BANK46_FRAME068);
const struct sprite_frame gSpriteBank46Frame069 = SPRITE_FRAME(gSpriteBank46Frame069, SPRITE_TILES_BANK46_FRAME069);
const struct sprite_frame gSpriteBank46Frame070 = SPRITE_FRAME(gSpriteBank46Frame070, SPRITE_TILES_BANK46_FRAME070);
const struct sprite_frame gSpriteBank46Frame071 = SPRITE_FRAME(gSpriteBank46Frame071, SPRITE_TILES_BANK46_FRAME071);
const struct sprite_frame gSpriteBank46Frame072 = SPRITE_FRAME(gSpriteBank46Frame072, SPRITE_TILES_BANK46_FRAME072);
const struct sprite_frame gSpriteBank46Frame073 = SPRITE_FRAME(gSpriteBank46Frame073, SPRITE_TILES_BANK46_FRAME073);
const struct sprite_frame gSpriteBank46Frame074 = SPRITE_FRAME(gSpriteBank46Frame074, SPRITE_TILES_BANK46_FRAME074);
const struct sprite_frame gSpriteBank46Frame075 = SPRITE_FRAME(gSpriteBank46Frame075, SPRITE_TILES_BANK46_FRAME075);
const struct sprite_frame gSpriteBank46Frame076 = SPRITE_FRAME(gSpriteBank46Frame076, SPRITE_TILES_BANK46_FRAME076);
const struct sprite_frame gSpriteBank46Frame077 = SPRITE_FRAME(gSpriteBank46Frame077, SPRITE_TILES_BANK46_FRAME077);
const struct sprite_frame gSpriteBank46Frame078 = SPRITE_FRAME(gSpriteBank46Frame078, SPRITE_TILES_BANK46_FRAME078);
const struct sprite_frame gSpriteBank46Frame079 = SPRITE_FRAME(gSpriteBank46Frame079, SPRITE_TILES_BANK46_FRAME079);
const struct sprite_frame gSpriteBank46Frame080 = SPRITE_FRAME(gSpriteBank46Frame080, SPRITE_TILES_BANK46_FRAME080);
const struct sprite_frame gSpriteBank46Frame081 = SPRITE_FRAME(gSpriteBank46Frame081, SPRITE_TILES_BANK46_FRAME081);
const struct sprite_frame gSpriteBank46Frame082 = SPRITE_FRAME(gSpriteBank46Frame082, SPRITE_TILES_BANK46_FRAME082);
const struct sprite_frame gSpriteBank46Frame083 = SPRITE_FRAME(gSpriteBank46Frame083, SPRITE_TILES_BANK46_FRAME083);
const struct sprite_frame gSpriteBank46Frame084 = SPRITE_FRAME(gSpriteBank46Frame084, SPRITE_TILES_BANK46_FRAME084);
const struct sprite_frame gSpriteBank46Frame085 = SPRITE_FRAME(gSpriteBank46Frame085, SPRITE_TILES_BANK46_FRAME085);
const struct sprite_frame gSpriteBank46Frame086 = SPRITE_FRAME(gSpriteBank46Frame086, SPRITE_TILES_BANK46_FRAME086);
const struct sprite_frame gSpriteBank46Frame087 = SPRITE_FRAME(gSpriteBank46Frame087, SPRITE_TILES_BANK46_FRAME087);

/* The piece arrays, from games/n2/graphics/sprites/bank46_crash_extra/bank.json. */
#include "sprites/bank46_pieces.inc"
