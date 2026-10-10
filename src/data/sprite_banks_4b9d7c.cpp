extern "C" {
#include "gba/types.h"
#include "sprite_bank.h"
}

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
 * A frame's tiles are an offset into the sprite tile pool
 * (src/data/sprite_tiles_2bf120.cpp), written relative to the pool range of
 * the bank that owns them (SPRITE_TILES_BANKnn). Editing a piece's shape,
 * adding a piece or moving the tiles means redrawing
 * graphics/sprites/bankNN_*.png to match.
 */

/* ---------------------------------------------------------------------- */
/* Bank 39: 13 animations, 24 frames, tiles in gSpriteBank39Tiles (SPRITE_TILES_BANK39). */

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
extern const struct sprite_frame_1box gSpriteBank39Frame000;
extern const struct sprite_frame_1box gSpriteBank39Frame001;
extern const struct sprite_frame_1box gSpriteBank39Frame002;
extern const struct sprite_frame_1box gSpriteBank39Frame003;
extern const struct sprite_frame_1box gSpriteBank39Frame004;
extern const struct sprite_frame gSpriteBank39Frame005;
extern const struct sprite_frame_1box gSpriteBank39Frame006;
extern const struct sprite_frame_1box gSpriteBank39Frame007;
extern const struct sprite_frame_1box gSpriteBank39Frame008;
extern const struct sprite_frame_1box gSpriteBank39Frame009;
extern const struct sprite_frame_1box gSpriteBank39Frame010;
extern const struct sprite_frame_1box gSpriteBank39Frame011;
extern const struct sprite_frame_1box gSpriteBank39Frame012;
extern const struct sprite_frame_1box gSpriteBank39Frame013;
extern const struct sprite_frame_1box gSpriteBank39Frame014;
extern const struct sprite_frame_1box gSpriteBank39Frame015;
extern const struct sprite_frame gSpriteBank39Frame016;
extern const struct sprite_frame_1box gSpriteBank39Frame017;
extern const struct sprite_frame_1box gSpriteBank39Frame018;
extern const struct sprite_frame_1box gSpriteBank39Frame019;
extern const struct sprite_frame_1box gSpriteBank39Frame020;
extern const struct sprite_frame_1box gSpriteBank39Frame021;
extern const struct sprite_frame_1box gSpriteBank39Frame022;
extern const struct sprite_frame_1box gSpriteBank39Frame023;
extern const struct sprite_piece_pos gSpriteBank39Frame000Pos[8];
extern const struct sprite_piece_pos gSpriteBank39Frame001Pos[4];
extern const struct sprite_piece_pos gSpriteBank39Frame002Pos[6];
extern const struct sprite_piece_pos gSpriteBank39Frame003Pos[3];
extern const struct sprite_piece_pos gSpriteBank39Frame004Pos[4];
extern const struct sprite_piece_pos gSpriteBank39Frame005Pos[3];
extern const struct sprite_piece_pos gSpriteBank39Frame006Pos[3];
extern const struct sprite_piece_pos gSpriteBank39Frame007Pos[3];
extern const struct sprite_piece_pos gSpriteBank39Frame008Pos[4];
extern const struct sprite_piece_pos gSpriteBank39Frame009Pos[4];
extern const struct sprite_piece_pos gSpriteBank39Frame010Pos[1];
extern const struct sprite_piece_pos gSpriteBank39Frame011Pos[2];
extern const struct sprite_piece_pos gSpriteBank39Frame012Pos[3];
extern const struct sprite_piece_pos gSpriteBank39Frame013Pos[3];
extern const struct sprite_piece_pos gSpriteBank39Frame014Pos[4];
extern const struct sprite_piece_pos gSpriteBank39Frame015Pos[4];
extern const struct sprite_piece_pos gSpriteBank39Frame016Pos[3];
extern const struct sprite_piece_pos gSpriteBank39Frame017Pos[7];
extern const struct sprite_piece_pos gSpriteBank39Frame018Pos[3];
extern const struct sprite_piece_pos gSpriteBank39Frame019Pos[5];
extern const struct sprite_piece_pos gSpriteBank39Frame020Pos[2];
extern const struct sprite_piece_pos gSpriteBank39Frame021Pos[2];
extern const struct sprite_piece_pos gSpriteBank39Frame022Pos[2];
extern const struct sprite_piece_pos gSpriteBank39Frame023Pos[3];
extern const u8 gSpriteBank39Frame000Pieces[8];
extern const u8 gSpriteBank39Frame001Pieces[4];
extern const u8 gSpriteBank39Frame002Pieces[6];
extern const u8 gSpriteBank39Frame003Pieces[3];
extern const u8 gSpriteBank39Frame004Pieces[4];
extern const u8 gSpriteBank39Frame005Pieces[3];
extern const u8 gSpriteBank39Frame006Pieces[3];
extern const u8 gSpriteBank39Frame007Pieces[3];
extern const u8 gSpriteBank39Frame008Pieces[4];
extern const u8 gSpriteBank39Frame009Pieces[4];
extern const u8 gSpriteBank39Frame010Pieces[1];
extern const u8 gSpriteBank39Frame011Pieces[2];
extern const u8 gSpriteBank39Frame012Pieces[3];
extern const u8 gSpriteBank39Frame013Pieces[3];
extern const u8 gSpriteBank39Frame014Pieces[4];
extern const u8 gSpriteBank39Frame015Pieces[4];
extern const u8 gSpriteBank39Frame016Pieces[3];
extern const u8 gSpriteBank39Frame017Pieces[7];
extern const u8 gSpriteBank39Frame018Pieces[3];
extern const u8 gSpriteBank39Frame019Pieces[5];
extern const u8 gSpriteBank39Frame020Pieces[2];
extern const u8 gSpriteBank39Frame021Pieces[2];
extern const u8 gSpriteBank39Frame022Pieces[2];
extern const u8 gSpriteBank39Frame023Pieces[3];

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
    SPRITE_FRAME(gSpriteBank39Frame000, SPRITE_TILES_BANK39 + 0x00000),
    { { -55, -11, 111, 23 } },
};
const struct sprite_frame_1box gSpriteBank39Frame001 = {
    SPRITE_FRAME(gSpriteBank39Frame001, SPRITE_TILES_BANK39 + 0x00520),
    { { -23, -11, 47, 23 } },
};
const struct sprite_frame_1box gSpriteBank39Frame002 = {
    SPRITE_FRAME(gSpriteBank39Frame002, SPRITE_TILES_BANK39 + 0x00740),
    { { -39, -11, 79, 23 } },
};
const struct sprite_frame_1box gSpriteBank39Frame003 = {
    SPRITE_FRAME(gSpriteBank39Frame003, SPRITE_TILES_BANK39 + 0x00ae0),
    { { -20, -14, 41, 28 } },
};
const struct sprite_frame_1box gSpriteBank39Frame004 = {
    SPRITE_FRAME(gSpriteBank39Frame004, SPRITE_TILES_BANK39 + 0x00da0),
    { { -21, -12, 42, 25 } },
};
const struct sprite_frame gSpriteBank39Frame005 = SPRITE_FRAME(gSpriteBank39Frame005, SPRITE_TILES_BANK39 + 0x01080);
const struct sprite_frame_1box gSpriteBank39Frame006 = {
    SPRITE_FRAME(gSpriteBank39Frame006, SPRITE_TILES_BANK39 + 0x01340),
    { { -20, -7, 40, 15 } },
};
const struct sprite_frame_1box gSpriteBank39Frame007 = {
    SPRITE_FRAME(gSpriteBank39Frame007, SPRITE_TILES_BANK39 + 0x014a0),
    { { -20, -7, 40, 15 } },
};
const struct sprite_frame_1box gSpriteBank39Frame008 = {
    SPRITE_FRAME(gSpriteBank39Frame008, SPRITE_TILES_BANK39 + 0x01600),
    { { -20, -7, 40, 17 } },
};
const struct sprite_frame_1box gSpriteBank39Frame009 = {
    SPRITE_FRAME(gSpriteBank39Frame009, SPRITE_TILES_BANK39 + 0x017a0),
    { { -18, -9, 36, 22 } },
};
const struct sprite_frame_1box gSpriteBank39Frame010 = {
    SPRITE_FRAME(gSpriteBank39Frame010, SPRITE_TILES_BANK39 + 0x01920),
    { { -15, -12, 30, 29 } },
};
const struct sprite_frame_1box gSpriteBank39Frame011 = {
    SPRITE_FRAME(gSpriteBank39Frame011, SPRITE_TILES_BANK39 + 0x01b20),
    { { -12, -15, 23, 35 } },
};
const struct sprite_frame_1box gSpriteBank39Frame012 = {
    SPRITE_FRAME(gSpriteBank39Frame012, SPRITE_TILES_BANK39 + 0x01d40),
    { { -8, -16, 15, 38 } },
};
const struct sprite_frame_1box gSpriteBank39Frame013 = {
    SPRITE_FRAME(gSpriteBank39Frame013, SPRITE_TILES_BANK39 + 0x01e80),
    { { -3, -16, 10, 39 } },
};
const struct sprite_frame_1box gSpriteBank39Frame014 = {
    SPRITE_FRAME(gSpriteBank39Frame014, SPRITE_TILES_BANK39 + 0x017a0),
    { { -18, -9, 36, 22 } },
};
const struct sprite_frame_1box gSpriteBank39Frame015 = {
    SPRITE_FRAME(gSpriteBank39Frame015, SPRITE_TILES_BANK39 + 0x01600),
    { { -20, -7, 40, 17 } },
};
const struct sprite_frame gSpriteBank39Frame016 = SPRITE_FRAME(gSpriteBank39Frame016, SPRITE_TILES_BANK39 + 0x01fa0);
const struct sprite_frame_1box gSpriteBank39Frame017 = {
    SPRITE_FRAME(gSpriteBank39Frame017, SPRITE_TILES_BANK39 + 0x02260),
    { { -36, -10, 72, 21 } },
};
const struct sprite_frame_1box gSpriteBank39Frame018 = {
    SPRITE_FRAME(gSpriteBank39Frame018, SPRITE_TILES_BANK39 + 0x025e0),
    { { -43, -11, 77, 26 } },
};
const struct sprite_frame_1box gSpriteBank39Frame019 = {
    SPRITE_FRAME(gSpriteBank39Frame019, SPRITE_TILES_BANK39 + 0x02aa0),
    { { -63, -17, 119, 37 } },
};
const struct sprite_frame_1box gSpriteBank39Frame020 = {
    SPRITE_FRAME(gSpriteBank39Frame020, SPRITE_TILES_BANK39 + 0x033c0),
    { { -62, -37, 123, 57 } },
};
const struct sprite_frame_1box gSpriteBank39Frame021 = {
    SPRITE_FRAME(gSpriteBank39Frame021, SPRITE_TILES_BANK39 + 0x043c0),
    { { -65, -38, 126, 59 } },
};
const struct sprite_frame_1box gSpriteBank39Frame022 = {
    SPRITE_FRAME(gSpriteBank39Frame022, SPRITE_TILES_BANK39 + 0x053c0),
    { { -65, -38, 127, 59 } },
};
const struct sprite_frame_1box gSpriteBank39Frame023 = {
    SPRITE_FRAME(gSpriteBank39Frame023, SPRITE_TILES_BANK39 + 0x063c0),
    { { -20, -14, 42, 28 } },
};

const struct sprite_piece_pos gSpriteBank39Frame000Pos[8] = { { -53, -11 }, { -23, -11 }, { 9, -11 }, { 41, -11 }, { -55, 5 }, { -23, 5 }, { 9, 5 }, { 41, 5 } };
const struct sprite_piece_pos gSpriteBank39Frame001Pos[4] = { { -21, -11 }, { 9, -11 }, { -23, 5 }, { 9, 5 } };
const struct sprite_piece_pos gSpriteBank39Frame002Pos[6] = { { -37, -11 }, { -7, -11 }, { 25, -11 }, { -39, 5 }, { -7, 5 }, { 25, 5 } };
const struct sprite_piece_pos gSpriteBank39Frame003Pos[3] = { { -20, -14 }, { 12, -12 }, { 20, -6 } };
const struct sprite_piece_pos gSpriteBank39Frame004Pos[4] = { { -22, -13 }, { 10, -12 }, { 18, -9 }, { 18, 7 } };
const struct sprite_piece_pos gSpriteBank39Frame005Pos[3] = { { -20, -14 }, { 12, -13 }, { 20, -6 } };
const struct sprite_piece_pos gSpriteBank39Frame006Pos[3] = { { -20, -7 }, { 12, -5 }, { 20, -1 } };
const struct sprite_piece_pos gSpriteBank39Frame007Pos[3] = { { -20, -7 }, { 12, -5 }, { 20, -1 } };
const struct sprite_piece_pos gSpriteBank39Frame008Pos[4] = { { -20, -7 }, { 12, -2 }, { 20, 5 }, { 1, 9 } };
const struct sprite_piece_pos gSpriteBank39Frame009Pos[4] = { { -18, -9 }, { 14, 4 }, { -4, 7 }, { 12, 7 } };
const struct sprite_piece_pos gSpriteBank39Frame010Pos[1] = { { -15, -12 } };
const struct sprite_piece_pos gSpriteBank39Frame011Pos[2] = { { -12, -15 }, { 4, 17 } };
const struct sprite_piece_pos gSpriteBank39Frame012Pos[3] = { { -8, -16 }, { -1, 16 }, { 7, 16 } };
const struct sprite_piece_pos gSpriteBank39Frame013Pos[3] = { { -3, -16 }, { 5, -15 }, { -2, 16 } };
const struct sprite_piece_pos gSpriteBank39Frame014Pos[4] = { { -18, -9 }, { 14, 4 }, { -4, 7 }, { 12, 7 } };
const struct sprite_piece_pos gSpriteBank39Frame015Pos[4] = { { -20, -7 }, { 12, -2 }, { 20, 5 }, { 1, 9 } };
const struct sprite_piece_pos gSpriteBank39Frame016Pos[3] = { { -20, -14 }, { 12, -13 }, { 20, -6 } };
const struct sprite_piece_pos gSpriteBank39Frame017Pos[7] = { { -36, -9 }, { -4, -10 }, { 28, -10 }, { 36, -8 }, { -35, 6 }, { -3, 6 }, { 29, 6 } };
const struct sprite_piece_pos gSpriteBank39Frame018Pos[3] = { { -43, -9 }, { 21, -11 }, { 29, -10 } };
const struct sprite_piece_pos gSpriteBank39Frame019Pos[5] = { { -63, -13 }, { 1, -17 }, { -45, 15 }, { -13, 15 }, { 29, 15 } };
const struct sprite_piece_pos gSpriteBank39Frame020Pos[2] = { { -62, -32 }, { 2, -37 } };
const struct sprite_piece_pos gSpriteBank39Frame021Pos[2] = { { -65, -32 }, { 1, -38 } };
const struct sprite_piece_pos gSpriteBank39Frame022Pos[2] = { { -65, -33 }, { 2, -38 } };
const struct sprite_piece_pos gSpriteBank39Frame023Pos[3] = { { -20, -14 }, { 12, -12 }, { 20, -6 } };

const u8 gSpriteBank39Frame000Pieces[8] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank39Frame001Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank39Frame002Pieces[6] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank39Frame003Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank39Frame004Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank39Frame005Pieces[3] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank39Frame006Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank39Frame007Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank39Frame008Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank39Frame009Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank39Frame010Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank39Frame011Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank39Frame012Pieces[3] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank39Frame013Pieces[3] = { SPRITE_PIECE(2, 9), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank39Frame014Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank39Frame015Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank39Frame016Pieces[3] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank39Frame017Pieces[7] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank39Frame018Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank39Frame019Pieces[5] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 7), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank39Frame020Pieces[2] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 3) };
const u8 gSpriteBank39Frame021Pieces[2] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 3) };
const u8 gSpriteBank39Frame022Pieces[2] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 3) };
const u8 gSpriteBank39Frame023Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };

/* ---------------------------------------------------------------------- */
/* Bank 40: 5 animations, 30 frames, tiles in gSpriteBank40Tiles (SPRITE_TILES_BANK40). */

extern const u16 gSpriteBank40Anim00Seq[3];
extern const u16 gSpriteBank40Anim01Seq[8];
extern const u16 gSpriteBank40Anim02Seq[8];
extern const u16 gSpriteBank40Anim03Seq[5];
extern const u16 gSpriteBank40Anim04Seq[6];
extern const struct sprite_frame_1box gSpriteBank40Frame000;
extern const struct sprite_frame_1box gSpriteBank40Frame001;
extern const struct sprite_frame_1box gSpriteBank40Frame002;
extern const struct sprite_frame_1box gSpriteBank40Frame003;
extern const struct sprite_frame_1box gSpriteBank40Frame004;
extern const struct sprite_frame_1box gSpriteBank40Frame005;
extern const struct sprite_frame_1box gSpriteBank40Frame006;
extern const struct sprite_frame_1box gSpriteBank40Frame007;
extern const struct sprite_frame_1box gSpriteBank40Frame008;
extern const struct sprite_frame_1box gSpriteBank40Frame009;
extern const struct sprite_frame_1box gSpriteBank40Frame010;
extern const struct sprite_frame_1box gSpriteBank40Frame011;
extern const struct sprite_frame_1box gSpriteBank40Frame012;
extern const struct sprite_frame_1box gSpriteBank40Frame013;
extern const struct sprite_frame_1box gSpriteBank40Frame014;
extern const struct sprite_frame_1box gSpriteBank40Frame015;
extern const struct sprite_frame_1box gSpriteBank40Frame016;
extern const struct sprite_frame_1box gSpriteBank40Frame017;
extern const struct sprite_frame_1box gSpriteBank40Frame018;
extern const struct sprite_frame_1box gSpriteBank40Frame019;
extern const struct sprite_frame_1box gSpriteBank40Frame020;
extern const struct sprite_frame_1box gSpriteBank40Frame021;
extern const struct sprite_frame_1box gSpriteBank40Frame022;
extern const struct sprite_frame_1box gSpriteBank40Frame023;
extern const struct sprite_frame_1box gSpriteBank40Frame024;
extern const struct sprite_frame_1box gSpriteBank40Frame025;
extern const struct sprite_frame_1box gSpriteBank40Frame026;
extern const struct sprite_frame_1box gSpriteBank40Frame027;
extern const struct sprite_frame_1box gSpriteBank40Frame028;
extern const struct sprite_frame_1box gSpriteBank40Frame029;
extern const struct sprite_piece_pos gSpriteBank40Frame000Pos[1];
extern const struct sprite_piece_pos gSpriteBank40Frame001Pos[1];
extern const struct sprite_piece_pos gSpriteBank40Frame002Pos[1];
extern const struct sprite_piece_pos gSpriteBank40Frame003Pos[1];
extern const struct sprite_piece_pos gSpriteBank40Frame004Pos[6];
extern const struct sprite_piece_pos gSpriteBank40Frame005Pos[2];
extern const struct sprite_piece_pos gSpriteBank40Frame006Pos[1];
extern const struct sprite_piece_pos gSpriteBank40Frame007Pos[1];
extern const struct sprite_piece_pos gSpriteBank40Frame008Pos[5];
extern const struct sprite_piece_pos gSpriteBank40Frame009Pos[1];
extern const struct sprite_piece_pos gSpriteBank40Frame010Pos[1];
extern const struct sprite_piece_pos gSpriteBank40Frame011Pos[2];
extern const struct sprite_piece_pos gSpriteBank40Frame012Pos[2];
extern const struct sprite_piece_pos gSpriteBank40Frame013Pos[2];
extern const struct sprite_piece_pos gSpriteBank40Frame014Pos[2];
extern const struct sprite_piece_pos gSpriteBank40Frame015Pos[1];
extern const struct sprite_piece_pos gSpriteBank40Frame016Pos[3];
extern const struct sprite_piece_pos gSpriteBank40Frame017Pos[2];
extern const struct sprite_piece_pos gSpriteBank40Frame018Pos[1];
extern const struct sprite_piece_pos gSpriteBank40Frame019Pos[2];
extern const struct sprite_piece_pos gSpriteBank40Frame020Pos[3];
extern const struct sprite_piece_pos gSpriteBank40Frame021Pos[1];
extern const struct sprite_piece_pos gSpriteBank40Frame022Pos[1];
extern const struct sprite_piece_pos gSpriteBank40Frame023Pos[1];
extern const struct sprite_piece_pos gSpriteBank40Frame024Pos[1];
extern const struct sprite_piece_pos gSpriteBank40Frame025Pos[2];
extern const struct sprite_piece_pos gSpriteBank40Frame026Pos[2];
extern const struct sprite_piece_pos gSpriteBank40Frame027Pos[2];
extern const struct sprite_piece_pos gSpriteBank40Frame028Pos[3];
extern const struct sprite_piece_pos gSpriteBank40Frame029Pos[3];
extern const u8 gSpriteBank40Frame000Pieces[1];
extern const u8 gSpriteBank40Frame001Pieces[1];
extern const u8 gSpriteBank40Frame002Pieces[1];
extern const u8 gSpriteBank40Frame003Pieces[1];
extern const u8 gSpriteBank40Frame004Pieces[6];
extern const u8 gSpriteBank40Frame005Pieces[2];
extern const u8 gSpriteBank40Frame006Pieces[1];
extern const u8 gSpriteBank40Frame007Pieces[1];
extern const u8 gSpriteBank40Frame008Pieces[5];
extern const u8 gSpriteBank40Frame009Pieces[1];
extern const u8 gSpriteBank40Frame010Pieces[1];
extern const u8 gSpriteBank40Frame011Pieces[2];
extern const u8 gSpriteBank40Frame012Pieces[2];
extern const u8 gSpriteBank40Frame013Pieces[2];
extern const u8 gSpriteBank40Frame014Pieces[2];
extern const u8 gSpriteBank40Frame015Pieces[1];
extern const u8 gSpriteBank40Frame016Pieces[3];
extern const u8 gSpriteBank40Frame017Pieces[2];
extern const u8 gSpriteBank40Frame018Pieces[1];
extern const u8 gSpriteBank40Frame019Pieces[2];
extern const u8 gSpriteBank40Frame020Pieces[3];
extern const u8 gSpriteBank40Frame021Pieces[1];
extern const u8 gSpriteBank40Frame022Pieces[1];
extern const u8 gSpriteBank40Frame023Pieces[1];
extern const u8 gSpriteBank40Frame024Pieces[1];
extern const u8 gSpriteBank40Frame025Pieces[2];
extern const u8 gSpriteBank40Frame026Pieces[2];
extern const u8 gSpriteBank40Frame027Pieces[2];
extern const u8 gSpriteBank40Frame028Pieces[3];
extern const u8 gSpriteBank40Frame029Pieces[3];

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
    SPRITE_FRAME(gSpriteBank40Frame000, SPRITE_TILES_BANK40 + 0x00000),
    { { -7, -7, 15, 15 } },
};
const struct sprite_frame_1box gSpriteBank40Frame001 = {
    SPRITE_FRAME(gSpriteBank40Frame001, SPRITE_TILES_BANK40 + 0x00080),
    { { -6, -5, 13, 12 } },
};
const struct sprite_frame_1box gSpriteBank40Frame002 = {
    SPRITE_FRAME(gSpriteBank40Frame002, SPRITE_TILES_BANK40 + 0x00100),
    { { -7, -6, 14, 14 } },
};
const struct sprite_frame_1box gSpriteBank40Frame003 = {
    SPRITE_FRAME(gSpriteBank40Frame003, SPRITE_TILES_BANK40 + 0x00180),
    { { -31, -15, 63, 31 } },
};
const struct sprite_frame_1box gSpriteBank40Frame004 = {
    SPRITE_FRAME(gSpriteBank40Frame004, SPRITE_TILES_BANK40 + 0x00580),
    { { -31, -15, 63, 31 } },
};
const struct sprite_frame_1box gSpriteBank40Frame005 = {
    SPRITE_FRAME(gSpriteBank40Frame005, SPRITE_TILES_BANK40 + 0x007e0),
    { { -31, -15, 63, 31 } },
};
const struct sprite_frame_1box gSpriteBank40Frame006 = {
    SPRITE_FRAME(gSpriteBank40Frame006, SPRITE_TILES_BANK40 + 0x009e0),
    { { -31, -15, 63, 31 } },
};
const struct sprite_frame_1box gSpriteBank40Frame007 = {
    SPRITE_FRAME(gSpriteBank40Frame007, SPRITE_TILES_BANK40 + 0x00de0),
    { { -31, -15, 63, 31 } },
};
const struct sprite_frame_1box gSpriteBank40Frame008 = {
    SPRITE_FRAME(gSpriteBank40Frame008, SPRITE_TILES_BANK40 + 0x011e0),
    { { -31, -15, 63, 31 } },
};
const struct sprite_frame_1box gSpriteBank40Frame009 = {
    SPRITE_FRAME(gSpriteBank40Frame009, SPRITE_TILES_BANK40 + 0x014a0),
    { { -31, -15, 63, 31 } },
};
const struct sprite_frame_1box gSpriteBank40Frame010 = {
    SPRITE_FRAME(gSpriteBank40Frame010, SPRITE_TILES_BANK40 + 0x018a0),
    { { -31, -15, 63, 31 } },
};
const struct sprite_frame_1box gSpriteBank40Frame011 = {
    SPRITE_FRAME(gSpriteBank40Frame011, SPRITE_TILES_BANK40 + 0x01ca0),
    { { -10, -7, 21, 15 } },
};
const struct sprite_frame_1box gSpriteBank40Frame012 = {
    SPRITE_FRAME(gSpriteBank40Frame012, SPRITE_TILES_BANK40 + 0x01d60),
    { { -11, -8, 22, 15 } },
};
const struct sprite_frame_1box gSpriteBank40Frame013 = {
    SPRITE_FRAME(gSpriteBank40Frame013, SPRITE_TILES_BANK40 + 0x01e20),
    { { -11, -10, 21, 13 } },
};
const struct sprite_frame_1box gSpriteBank40Frame014 = {
    SPRITE_FRAME(gSpriteBank40Frame014, SPRITE_TILES_BANK40 + 0x01ee0),
    { { -13, -14, 23, 19 } },
};
const struct sprite_frame_1box gSpriteBank40Frame015 = {
    SPRITE_FRAME(gSpriteBank40Frame015, SPRITE_TILES_BANK40 + 0x02000),
    { { -13, -19, 24, 24 } },
};
const struct sprite_frame_1box gSpriteBank40Frame016 = {
    SPRITE_FRAME(gSpriteBank40Frame016, SPRITE_TILES_BANK40 + 0x02200),
    { { -13, -20, 24, 22 } },
};
const struct sprite_frame_1box gSpriteBank40Frame017 = {
    SPRITE_FRAME(gSpriteBank40Frame017, SPRITE_TILES_BANK40 + 0x02340),
    { { -4, -21, 14, 23 } },
};
const struct sprite_frame_1box gSpriteBank40Frame018 = {
    SPRITE_FRAME(gSpriteBank40Frame018, SPRITE_TILES_BANK40 + 0x023e0),
    { { 4, -21, 7, 3 } },
};
const struct sprite_frame_1box gSpriteBank40Frame019 = {
    SPRITE_FRAME(gSpriteBank40Frame019, SPRITE_TILES_BANK40 + 0x02400),
    { { -4, -5, 9, 10 } },
};
const struct sprite_frame_1box gSpriteBank40Frame020 = {
    SPRITE_FRAME(gSpriteBank40Frame020, SPRITE_TILES_BANK40 + 0x02460),
    { { -9, -10, 17, 19 } },
};
const struct sprite_frame_1box gSpriteBank40Frame021 = {
    SPRITE_FRAME(gSpriteBank40Frame021, SPRITE_TILES_BANK40 + 0x02520),
    { { -12, -13, 25, 27 } },
};
const struct sprite_frame_1box gSpriteBank40Frame022 = {
    SPRITE_FRAME(gSpriteBank40Frame022, SPRITE_TILES_BANK40 + 0x02720),
    { { -11, -15, 26, 27 } },
};
const struct sprite_frame_1box gSpriteBank40Frame023 = {
    SPRITE_FRAME(gSpriteBank40Frame023, SPRITE_TILES_BANK40 + 0x02920),
    { { -8, -16, 23, 28 } },
};
const struct sprite_frame_1box gSpriteBank40Frame024 = {
    SPRITE_FRAME(gSpriteBank40Frame024, SPRITE_TILES_BANK40 + 0x02b20),
    { { -6, -6, 12, 13 } },
};
const struct sprite_frame_1box gSpriteBank40Frame025 = {
    SPRITE_FRAME(gSpriteBank40Frame025, SPRITE_TILES_BANK40 + 0x02ba0),
    { { -12, -9, 17, 15 } },
};
const struct sprite_frame_1box gSpriteBank40Frame026 = {
    SPRITE_FRAME(gSpriteBank40Frame026, SPRITE_TILES_BANK40 + 0x02c40),
    { { -15, -17, 19, 24 } },
};
const struct sprite_frame_1box gSpriteBank40Frame027 = {
    SPRITE_FRAME(gSpriteBank40Frame027, SPRITE_TILES_BANK40 + 0x02d80),
    { { -15, -22, 21, 24 } },
};
const struct sprite_frame_1box gSpriteBank40Frame028 = {
    SPRITE_FRAME(gSpriteBank40Frame028, SPRITE_TILES_BANK40 + 0x02ea0),
    { { -12, -23, 20, 20 } },
};
const struct sprite_frame_1box gSpriteBank40Frame029 = {
    SPRITE_FRAME(gSpriteBank40Frame029, SPRITE_TILES_BANK40 + 0x02f60),
    { { -13, -23, 17, 16 } },
};

const struct sprite_piece_pos gSpriteBank40Frame000Pos[1] = { { -7, -7 } };
const struct sprite_piece_pos gSpriteBank40Frame001Pos[1] = { { -6, -5 } };
const struct sprite_piece_pos gSpriteBank40Frame002Pos[1] = { { -6, -6 } };
const struct sprite_piece_pos gSpriteBank40Frame003Pos[1] = { { -31, -14 } };
const struct sprite_piece_pos gSpriteBank40Frame004Pos[6] = { { -29, -8 }, { 9, -9 }, { 19, -9 }, { -25, 7 }, { 11, 7 }, { 23, 9 } };
const struct sprite_piece_pos gSpriteBank40Frame005Pos[2] = { { -26, -6 }, { 6, -7 } };
const struct sprite_piece_pos gSpriteBank40Frame006Pos[1] = { { -31, -10 } };
const struct sprite_piece_pos gSpriteBank40Frame007Pos[1] = { { -31, -11 } };
const struct sprite_piece_pos gSpriteBank40Frame008Pos[5] = { { -24, -11 }, { 8, -9 }, { -15, 6 }, { 19, 5 }, { 25, 5 } };
const struct sprite_piece_pos gSpriteBank40Frame009Pos[1] = { { -29, -14 } };
const struct sprite_piece_pos gSpriteBank40Frame010Pos[1] = { { -29, -14 } };
const struct sprite_piece_pos gSpriteBank40Frame011Pos[2] = { { -10, -7 }, { 6, -7 } };
const struct sprite_piece_pos gSpriteBank40Frame012Pos[2] = { { -11, -7 }, { 5, -8 } };
const struct sprite_piece_pos gSpriteBank40Frame013Pos[2] = { { -11, -9 }, { 5, -10 } };
const struct sprite_piece_pos gSpriteBank40Frame014Pos[2] = { { -13, -14 }, { -1, 3 } };
const struct sprite_piece_pos gSpriteBank40Frame015Pos[1] = { { -13, -19 } };
const struct sprite_piece_pos gSpriteBank40Frame016Pos[3] = { { -13, -20 }, { -5, -4 }, { 3, 0 } };
const struct sprite_piece_pos gSpriteBank40Frame017Pos[2] = { { -4, -21 }, { -3, -5 } };
const struct sprite_piece_pos gSpriteBank40Frame018Pos[1] = { { 4, -21 } };
const struct sprite_piece_pos gSpriteBank40Frame019Pos[2] = { { -4, -5 }, { 4, -1 } };
const struct sprite_piece_pos gSpriteBank40Frame020Pos[3] = { { -9, -10 }, { 7, -2 }, { 2, 6 } };
const struct sprite_piece_pos gSpriteBank40Frame021Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank40Frame022Pos[1] = { { -11, -15 } };
const struct sprite_piece_pos gSpriteBank40Frame023Pos[1] = { { -8, -16 } };
const struct sprite_piece_pos gSpriteBank40Frame024Pos[1] = { { -6, -6 } };
const struct sprite_piece_pos gSpriteBank40Frame025Pos[2] = { { -12, -9 }, { 4, -5 } };
const struct sprite_piece_pos gSpriteBank40Frame026Pos[2] = { { -15, -17 }, { 1, -9 } };
const struct sprite_piece_pos gSpriteBank40Frame027Pos[2] = { { -15, -22 }, { 1, -16 } };
const struct sprite_piece_pos gSpriteBank40Frame028Pos[3] = { { -12, -23 }, { 6, -18 }, { -4, -5 } };
const struct sprite_piece_pos gSpriteBank40Frame029Pos[3] = { { -13, -23 }, { 3, -19 }, { -8, -7 } };

const u8 gSpriteBank40Frame000Pieces[1] = { SPRITE_PIECE(2, 1) };
const u8 gSpriteBank40Frame001Pieces[1] = { SPRITE_PIECE(2, 1) };
const u8 gSpriteBank40Frame002Pieces[1] = { SPRITE_PIECE(2, 1) };
const u8 gSpriteBank40Frame003Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank40Frame004Pieces[6] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank40Frame005Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank40Frame006Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank40Frame007Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank40Frame008Pieces[5] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank40Frame009Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank40Frame010Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank40Frame011Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank40Frame012Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank40Frame013Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank40Frame014Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank40Frame015Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank40Frame016Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank40Frame017Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank40Frame018Pieces[1] = { SPRITE_PIECE(2, 0) };
const u8 gSpriteBank40Frame019Pieces[2] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank40Frame020Pieces[3] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank40Frame021Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank40Frame022Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank40Frame023Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank40Frame024Pieces[1] = { SPRITE_PIECE(2, 1) };
const u8 gSpriteBank40Frame025Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank40Frame026Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank40Frame027Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank40Frame028Pieces[3] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank40Frame029Pieces[3] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };

/* ---------------------------------------------------------------------- */
/* Bank 41: 3 animations, 24 frames, tiles in gSpriteBank41Tiles (SPRITE_TILES_BANK41). */

extern const u16 gSpriteBank41Anim00Seq[6];
extern const u16 gSpriteBank41Anim01Seq[10];
extern const u16 gSpriteBank41Anim02Seq[8];
extern const struct sprite_frame_1box gSpriteBank41Frame000;
extern const struct sprite_frame_1box gSpriteBank41Frame001;
extern const struct sprite_frame_1box gSpriteBank41Frame002;
extern const struct sprite_frame_1box gSpriteBank41Frame003;
extern const struct sprite_frame_1box gSpriteBank41Frame004;
extern const struct sprite_frame_1box gSpriteBank41Frame005;
extern const struct sprite_frame_1box gSpriteBank41Frame006;
extern const struct sprite_frame_1box gSpriteBank41Frame007;
extern const struct sprite_frame_1box gSpriteBank41Frame008;
extern const struct sprite_frame_1box gSpriteBank41Frame009;
extern const struct sprite_frame_1box gSpriteBank41Frame010;
extern const struct sprite_frame_1box gSpriteBank41Frame011;
extern const struct sprite_frame_1box gSpriteBank41Frame012;
extern const struct sprite_frame_1box gSpriteBank41Frame013;
extern const struct sprite_frame_1box gSpriteBank41Frame014;
extern const struct sprite_frame_1box gSpriteBank41Frame015;
extern const struct sprite_frame_1box gSpriteBank41Frame016;
extern const struct sprite_frame_1box gSpriteBank41Frame017;
extern const struct sprite_frame_1box gSpriteBank41Frame018;
extern const struct sprite_frame_1box gSpriteBank41Frame019;
extern const struct sprite_frame_1box gSpriteBank41Frame020;
extern const struct sprite_frame_1box gSpriteBank41Frame021;
extern const struct sprite_frame_1box gSpriteBank41Frame022;
extern const struct sprite_frame_1box gSpriteBank41Frame023;
extern const struct sprite_piece_pos gSpriteBank41Frame000Pos[1];
extern const struct sprite_piece_pos gSpriteBank41Frame001Pos[1];
extern const struct sprite_piece_pos gSpriteBank41Frame002Pos[1];
extern const struct sprite_piece_pos gSpriteBank41Frame003Pos[1];
extern const struct sprite_piece_pos gSpriteBank41Frame004Pos[1];
extern const struct sprite_piece_pos gSpriteBank41Frame005Pos[1];
extern const struct sprite_piece_pos gSpriteBank41Frame006Pos[2];
extern const struct sprite_piece_pos gSpriteBank41Frame007Pos[1];
extern const struct sprite_piece_pos gSpriteBank41Frame008Pos[2];
extern const struct sprite_piece_pos gSpriteBank41Frame009Pos[2];
extern const struct sprite_piece_pos gSpriteBank41Frame010Pos[4];
extern const struct sprite_piece_pos gSpriteBank41Frame011Pos[2];
extern const struct sprite_piece_pos gSpriteBank41Frame012Pos[1];
extern const struct sprite_piece_pos gSpriteBank41Frame013Pos[2];
extern const struct sprite_piece_pos gSpriteBank41Frame014Pos[1];
extern const struct sprite_piece_pos gSpriteBank41Frame015Pos[1];
extern const struct sprite_piece_pos gSpriteBank41Frame016Pos[3];
extern const struct sprite_piece_pos gSpriteBank41Frame017Pos[3];
extern const struct sprite_piece_pos gSpriteBank41Frame018Pos[3];
extern const struct sprite_piece_pos gSpriteBank41Frame019Pos[3];
extern const struct sprite_piece_pos gSpriteBank41Frame020Pos[2];
extern const struct sprite_piece_pos gSpriteBank41Frame021Pos[2];
extern const struct sprite_piece_pos gSpriteBank41Frame022Pos[2];
extern const struct sprite_piece_pos gSpriteBank41Frame023Pos[2];
extern const u8 gSpriteBank41Frame000Pieces[1];
extern const u8 gSpriteBank41Frame001Pieces[1];
extern const u8 gSpriteBank41Frame002Pieces[1];
extern const u8 gSpriteBank41Frame003Pieces[1];
extern const u8 gSpriteBank41Frame004Pieces[1];
extern const u8 gSpriteBank41Frame005Pieces[1];
extern const u8 gSpriteBank41Frame006Pieces[2];
extern const u8 gSpriteBank41Frame007Pieces[1];
extern const u8 gSpriteBank41Frame008Pieces[2];
extern const u8 gSpriteBank41Frame009Pieces[2];
extern const u8 gSpriteBank41Frame010Pieces[4];
extern const u8 gSpriteBank41Frame011Pieces[2];
extern const u8 gSpriteBank41Frame012Pieces[1];
extern const u8 gSpriteBank41Frame013Pieces[2];
extern const u8 gSpriteBank41Frame014Pieces[1];
extern const u8 gSpriteBank41Frame015Pieces[1];
extern const u8 gSpriteBank41Frame016Pieces[3];
extern const u8 gSpriteBank41Frame017Pieces[3];
extern const u8 gSpriteBank41Frame018Pieces[3];
extern const u8 gSpriteBank41Frame019Pieces[3];
extern const u8 gSpriteBank41Frame020Pieces[2];
extern const u8 gSpriteBank41Frame021Pieces[2];
extern const u8 gSpriteBank41Frame022Pieces[2];
extern const u8 gSpriteBank41Frame023Pieces[2];

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
    SPRITE_FRAME(gSpriteBank41Frame000, SPRITE_TILES_BANK41 + 0x00000),
    { { -2, -1, 5, 3 } },
};
const struct sprite_frame_1box gSpriteBank41Frame001 = {
    SPRITE_FRAME(gSpriteBank41Frame001, SPRITE_TILES_BANK41 + 0x00020),
    { { -3, -4, 7, 5 } },
};
const struct sprite_frame_1box gSpriteBank41Frame002 = {
    SPRITE_FRAME(gSpriteBank41Frame002, SPRITE_TILES_BANK41 + 0x00040),
    { { -2, -4, 5, 5 } },
};
const struct sprite_frame_1box gSpriteBank41Frame003 = {
    SPRITE_FRAME(gSpriteBank41Frame003, SPRITE_TILES_BANK41 + 0x00060),
    { { -1, -4, 3, 4 } },
};
const struct sprite_frame_1box gSpriteBank41Frame004 = {
    SPRITE_FRAME(gSpriteBank41Frame004, SPRITE_TILES_BANK41 + 0x00080),
    { { -1, -4, 3, 3 } },
};
const struct sprite_frame_1box gSpriteBank41Frame005 = {
    SPRITE_FRAME(gSpriteBank41Frame005, SPRITE_TILES_BANK41 + 0x000a0),
    { { -1, -4, 1, 1 } },
};
const struct sprite_frame_1box gSpriteBank41Frame006 = {
    SPRITE_FRAME(gSpriteBank41Frame006, SPRITE_TILES_BANK41 + 0x000c0),
    { { -4, -1, 8, 2 } },
};
const struct sprite_frame_1box gSpriteBank41Frame007 = {
    SPRITE_FRAME(gSpriteBank41Frame007, SPRITE_TILES_BANK41 + 0x00100),
    { { -10, -2, 15, 3 } },
};
const struct sprite_frame_1box gSpriteBank41Frame008 = {
    SPRITE_FRAME(gSpriteBank41Frame008, SPRITE_TILES_BANK41 + 0x00140),
    { { -15, -6, 20, 7 } },
};
const struct sprite_frame_1box gSpriteBank41Frame009 = {
    SPRITE_FRAME(gSpriteBank41Frame009, SPRITE_TILES_BANK41 + 0x001a0),
    { { -17, -11, 18, 12 } },
};
const struct sprite_frame_1box gSpriteBank41Frame010 = {
    SPRITE_FRAME(gSpriteBank41Frame010, SPRITE_TILES_BANK41 + 0x00240),
    { { -20, -16, 17, 17 } },
};
const struct sprite_frame_1box gSpriteBank41Frame011 = {
    SPRITE_FRAME(gSpriteBank41Frame011, SPRITE_TILES_BANK41 + 0x00320),
    { { -20, -17, 13, 16 } },
};
const struct sprite_frame_1box gSpriteBank41Frame012 = {
    SPRITE_FRAME(gSpriteBank41Frame012, SPRITE_TILES_BANK41 + 0x003c0),
    { { -20, -18, 12, 15 } },
};
const struct sprite_frame_1box gSpriteBank41Frame013 = {
    SPRITE_FRAME(gSpriteBank41Frame013, SPRITE_TILES_BANK41 + 0x00440),
    { { -19, -22, 9, 15 } },
};
const struct sprite_frame_1box gSpriteBank41Frame014 = {
    SPRITE_FRAME(gSpriteBank41Frame014, SPRITE_TILES_BANK41 + 0x004a0),
    { { -18, -23, 6, 9 } },
};
const struct sprite_frame_1box gSpriteBank41Frame015 = {
    SPRITE_FRAME(gSpriteBank41Frame015, SPRITE_TILES_BANK41 + 0x004e0),
    { { -17, -22, 5, 5 } },
};
const struct sprite_frame_1box gSpriteBank41Frame016 = {
    SPRITE_FRAME(gSpriteBank41Frame016, SPRITE_TILES_BANK41 + 0x00500),
    { { -9, -8, 19, 16 } },
};
const struct sprite_frame_1box gSpriteBank41Frame017 = {
    SPRITE_FRAME(gSpriteBank41Frame017, SPRITE_TILES_BANK41 + 0x005e0),
    { { -11, -12, 24, 20 } },
};
const struct sprite_frame_1box gSpriteBank41Frame018 = {
    SPRITE_FRAME(gSpriteBank41Frame018, SPRITE_TILES_BANK41 + 0x00740),
    { { -10, -12, 22, 19 } },
};
const struct sprite_frame_1box gSpriteBank41Frame019 = {
    SPRITE_FRAME(gSpriteBank41Frame019, SPRITE_TILES_BANK41 + 0x00840),
    { { -9, -12, 21, 16 } },
};
const struct sprite_frame_1box gSpriteBank41Frame020 = {
    SPRITE_FRAME(gSpriteBank41Frame020, SPRITE_TILES_BANK41 + 0x00920),
    { { -8, -12, 18, 14 } },
};
const struct sprite_frame_1box gSpriteBank41Frame021 = {
    SPRITE_FRAME(gSpriteBank41Frame021, SPRITE_TILES_BANK41 + 0x009c0),
    { { -6, -14, 16, 11 } },
};
const struct sprite_frame_1box gSpriteBank41Frame022 = {
    SPRITE_FRAME(gSpriteBank41Frame022, SPRITE_TILES_BANK41 + 0x00a60),
    { { -6, -14, 16, 6 } },
};
const struct sprite_frame_1box gSpriteBank41Frame023 = {
    SPRITE_FRAME(gSpriteBank41Frame023, SPRITE_TILES_BANK41 + 0x00ac0),
    { { -3, -17, 10, 5 } },
};

const struct sprite_piece_pos gSpriteBank41Frame000Pos[1] = { { -2, -1 } };
const struct sprite_piece_pos gSpriteBank41Frame001Pos[1] = { { -3, -4 } };
const struct sprite_piece_pos gSpriteBank41Frame002Pos[1] = { { -2, -4 } };
const struct sprite_piece_pos gSpriteBank41Frame003Pos[1] = { { -1, -4 } };
const struct sprite_piece_pos gSpriteBank41Frame004Pos[1] = { { -1, -4 } };
const struct sprite_piece_pos gSpriteBank41Frame005Pos[1] = { { -1, -4 } };
const struct sprite_piece_pos gSpriteBank41Frame006Pos[2] = { { -4, -1 }, { 4, 0 } };
const struct sprite_piece_pos gSpriteBank41Frame007Pos[1] = { { -10, -2 } };
const struct sprite_piece_pos gSpriteBank41Frame008Pos[2] = { { -15, -6 }, { 1, -2 } };
const struct sprite_piece_pos gSpriteBank41Frame009Pos[2] = { { -17, -11 }, { -1, -1 } };
const struct sprite_piece_pos gSpriteBank41Frame010Pos[4] = { { -20, -16 }, { -4, -5 }, { -13, 0 }, { -5, 0 } };
const struct sprite_piece_pos gSpriteBank41Frame011Pos[2] = { { -20, -17 }, { -12, -1 } };
const struct sprite_piece_pos gSpriteBank41Frame012Pos[1] = { { -20, -18 } };
const struct sprite_piece_pos gSpriteBank41Frame013Pos[2] = { { -19, -22 }, { -11, -11 } };
const struct sprite_piece_pos gSpriteBank41Frame014Pos[1] = { { -18, -22 } };
const struct sprite_piece_pos gSpriteBank41Frame015Pos[1] = { { -17, -22 } };
const struct sprite_piece_pos gSpriteBank41Frame016Pos[3] = { { -9, -8 }, { 7, -8 }, { -3, 8 } };
const struct sprite_piece_pos gSpriteBank41Frame017Pos[3] = { { -11, -12 }, { -9, 4 }, { 7, 4 } };
const struct sprite_piece_pos gSpriteBank41Frame018Pos[3] = { { -10, -10 }, { 6, -12 }, { -5, 4 } };
const struct sprite_piece_pos gSpriteBank41Frame019Pos[3] = { { -9, -11 }, { 7, -12 }, { -2, 4 } };
const struct sprite_piece_pos gSpriteBank41Frame020Pos[2] = { { -8, -12 }, { 8, -7 } };
const struct sprite_piece_pos gSpriteBank41Frame021Pos[2] = { { -6, -14 }, { 10, -8 } };
const struct sprite_piece_pos gSpriteBank41Frame022Pos[2] = { { -6, -14 }, { 10, -11 } };
const struct sprite_piece_pos gSpriteBank41Frame023Pos[2] = { { -3, -17 }, { 7, -15 } };

const u8 gSpriteBank41Frame000Pieces[1] = { SPRITE_PIECE(2, 0) };
const u8 gSpriteBank41Frame001Pieces[1] = { SPRITE_PIECE(2, 0) };
const u8 gSpriteBank41Frame002Pieces[1] = { SPRITE_PIECE(2, 0) };
const u8 gSpriteBank41Frame003Pieces[1] = { SPRITE_PIECE(2, 0) };
const u8 gSpriteBank41Frame004Pieces[1] = { SPRITE_PIECE(2, 0) };
const u8 gSpriteBank41Frame005Pieces[1] = { SPRITE_PIECE(2, 0) };
const u8 gSpriteBank41Frame006Pieces[2] = { SPRITE_PIECE(2, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank41Frame007Pieces[1] = { SPRITE_PIECE(2, 4) };
const u8 gSpriteBank41Frame008Pieces[2] = { SPRITE_PIECE(2, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank41Frame009Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank41Frame010Pieces[4] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank41Frame011Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank41Frame012Pieces[1] = { SPRITE_PIECE(2, 1) };
const u8 gSpriteBank41Frame013Pieces[2] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank41Frame014Pieces[1] = { SPRITE_PIECE(2, 8) };
const u8 gSpriteBank41Frame015Pieces[1] = { SPRITE_PIECE(2, 0) };
const u8 gSpriteBank41Frame016Pieces[3] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank41Frame017Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank41Frame018Pieces[3] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank41Frame019Pieces[3] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank41Frame020Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank41Frame021Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank41Frame022Pieces[2] = { SPRITE_PIECE(2, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank41Frame023Pieces[2] = { SPRITE_PIECE(2, 0), SPRITE_PIECE(0, 0) };

/* ---------------------------------------------------------------------- */
/* Bank 42: 1 animation, 11 frames, tiles in gSpriteBank42Tiles (SPRITE_TILES_BANK42). */

extern const u16 gSpriteBank42Anim00Seq[24];
extern const struct sprite_frame_1box gSpriteBank42Frame000;
extern const struct sprite_frame_1box gSpriteBank42Frame001;
extern const struct sprite_frame_1box gSpriteBank42Frame002;
extern const struct sprite_frame_1box gSpriteBank42Frame003;
extern const struct sprite_frame_1box gSpriteBank42Frame004;
extern const struct sprite_frame gSpriteBank42Frame005;
extern const struct sprite_frame_1box gSpriteBank42Frame006;
extern const struct sprite_frame_1box gSpriteBank42Frame007;
extern const struct sprite_frame_1box gSpriteBank42Frame008;
extern const struct sprite_frame_1box gSpriteBank42Frame009;
extern const struct sprite_frame_1box gSpriteBank42Frame010;
extern const struct sprite_piece_pos gSpriteBank42Frame000Pos[1];
extern const struct sprite_piece_pos gSpriteBank42Frame001Pos[1];
extern const struct sprite_piece_pos gSpriteBank42Frame002Pos[1];
extern const struct sprite_piece_pos gSpriteBank42Frame003Pos[1];
extern const struct sprite_piece_pos gSpriteBank42Frame004Pos[2];
extern const struct sprite_piece_pos gSpriteBank42Frame006Pos[1];
extern const struct sprite_piece_pos gSpriteBank42Frame007Pos[1];
extern const struct sprite_piece_pos gSpriteBank42Frame008Pos[2];
extern const struct sprite_piece_pos gSpriteBank42Frame009Pos[1];
extern const struct sprite_piece_pos gSpriteBank42Frame010Pos[2];
extern const u8 gSpriteBank42Frame000Pieces[1];
extern const u8 gSpriteBank42Frame001Pieces[1];
extern const u8 gSpriteBank42Frame002Pieces[1];
extern const u8 gSpriteBank42Frame003Pieces[1];
extern const u8 gSpriteBank42Frame004Pieces[2];
extern const u8 gSpriteBank42Frame006Pieces[1];
extern const u8 gSpriteBank42Frame007Pieces[1];
extern const u8 gSpriteBank42Frame008Pieces[2];
extern const u8 gSpriteBank42Frame009Pieces[1];
extern const u8 gSpriteBank42Frame010Pieces[2];

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
    SPRITE_FRAME(gSpriteBank42Frame000, SPRITE_TILES_BANK42 + 0x00000),
    { { -3, -3, 7, 7 } },
};
const struct sprite_frame_1box gSpriteBank42Frame001 = {
    SPRITE_FRAME(gSpriteBank42Frame001, SPRITE_TILES_BANK42 + 0x00020),
    { { -3, -3, 15, 7 } },
};
const struct sprite_frame_1box gSpriteBank42Frame002 = {
    SPRITE_FRAME(gSpriteBank42Frame002, SPRITE_TILES_BANK42 + 0x00060),
    { { -3, -3, 23, 7 } },
};
const struct sprite_frame_1box gSpriteBank42Frame003 = {
    SPRITE_FRAME(gSpriteBank42Frame003, SPRITE_TILES_BANK42 + 0x000e0),
    { { -3, -3, 31, 7 } },
};
const struct sprite_frame_1box gSpriteBank42Frame004 = {
    SPRITE_FRAME(gSpriteBank42Frame004, SPRITE_TILES_BANK42 + 0x00160),
    { { -3, -3, 39, 7 } },
};
const struct sprite_frame gSpriteBank42Frame005 = { gSpriteBank42Frame006Pos, gSpriteBank42Frame006Pieces, SPRITE_FRAME_TILES(SPRITE_TILES_BANK00 + 0x39f00, 0) };
const struct sprite_frame_1box gSpriteBank42Frame006 = {
    SPRITE_FRAME(gSpriteBank42Frame006, SPRITE_TILES_BANK42 + 0x00200),
    { { -3, -3, 7, 7 } },
};
const struct sprite_frame_1box gSpriteBank42Frame007 = {
    SPRITE_FRAME(gSpriteBank42Frame007, SPRITE_TILES_BANK42 + 0x00220),
    { { -3, -3, 15, 7 } },
};
const struct sprite_frame_1box gSpriteBank42Frame008 = {
    SPRITE_FRAME(gSpriteBank42Frame008, SPRITE_TILES_BANK42 + 0x00260),
    { { -3, -3, 21, 7 } },
};
const struct sprite_frame_1box gSpriteBank42Frame009 = {
    SPRITE_FRAME(gSpriteBank42Frame009, SPRITE_TILES_BANK42 + 0x002c0),
    { { -3, -3, 30, 7 } },
};
const struct sprite_frame_1box gSpriteBank42Frame010 = {
    SPRITE_FRAME(gSpriteBank42Frame010, SPRITE_TILES_BANK42 + 0x00340),
    { { -3, -3, 38, 7 } },
};

const struct sprite_piece_pos gSpriteBank42Frame000Pos[1] = { { -3, -3 } };
const struct sprite_piece_pos gSpriteBank42Frame001Pos[1] = { { -3, -3 } };
const struct sprite_piece_pos gSpriteBank42Frame002Pos[1] = { { -3, -3 } };
const struct sprite_piece_pos gSpriteBank42Frame003Pos[1] = { { -3, -3 } };
const struct sprite_piece_pos gSpriteBank42Frame004Pos[2] = { { -3, -3 }, { 29, -3 } };
const struct sprite_piece_pos gSpriteBank42Frame006Pos[1] = { { -3, -3 } };
const struct sprite_piece_pos gSpriteBank42Frame007Pos[1] = { { -3, -3 } };
const struct sprite_piece_pos gSpriteBank42Frame008Pos[2] = { { -3, -3 }, { 13, -3 } };
const struct sprite_piece_pos gSpriteBank42Frame009Pos[1] = { { -3, -3 } };
const struct sprite_piece_pos gSpriteBank42Frame010Pos[2] = { { -3, -3 }, { 29, -3 } };

const u8 gSpriteBank42Frame000Pieces[1] = { SPRITE_PIECE(2, 0) };
const u8 gSpriteBank42Frame001Pieces[1] = { SPRITE_PIECE(2, 4) };
const u8 gSpriteBank42Frame002Pieces[1] = { SPRITE_PIECE(2, 5) };
const u8 gSpriteBank42Frame003Pieces[1] = { SPRITE_PIECE(2, 5) };
const u8 gSpriteBank42Frame004Pieces[2] = { SPRITE_PIECE(2, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank42Frame006Pieces[1] = { SPRITE_PIECE(2, 0) };
const u8 gSpriteBank42Frame007Pieces[1] = { SPRITE_PIECE(2, 4) };
const u8 gSpriteBank42Frame008Pieces[2] = { SPRITE_PIECE(2, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank42Frame009Pieces[1] = { SPRITE_PIECE(2, 5) };
const u8 gSpriteBank42Frame010Pieces[2] = { SPRITE_PIECE(2, 5), SPRITE_PIECE(0, 0) };

/* ---------------------------------------------------------------------- */
/* Bank 43: 7 animations, 21 frames, tiles in gSpriteBank43Tiles (SPRITE_TILES_BANK43). */

extern const u16 gSpriteBank43Anim00Seq[10];
extern const u16 gSpriteBank43Anim01Seq[10];
extern const u16 gSpriteBank43Anim02Seq[10];
extern const u16 gSpriteBank43Anim03Seq[10];
extern const u16 gSpriteBank43Anim04Seq[10];
extern const u16 gSpriteBank43Anim05Seq[10];
extern const u16 gSpriteBank43Anim06Seq[10];
extern const struct sprite_frame_1box gSpriteBank43Frame000;
extern const struct sprite_frame_1box gSpriteBank43Frame001;
extern const struct sprite_frame_1box gSpriteBank43Frame002;
extern const struct sprite_frame_1box gSpriteBank43Frame003;
extern const struct sprite_frame_1box gSpriteBank43Frame004;
extern const struct sprite_frame_1box gSpriteBank43Frame005;
extern const struct sprite_frame_1box gSpriteBank43Frame006;
extern const struct sprite_frame_1box gSpriteBank43Frame007;
extern const struct sprite_frame_1box gSpriteBank43Frame008;
extern const struct sprite_frame_1box gSpriteBank43Frame009;
extern const struct sprite_frame_1box gSpriteBank43Frame010;
extern const struct sprite_frame_1box gSpriteBank43Frame011;
extern const struct sprite_frame_1box gSpriteBank43Frame012;
extern const struct sprite_frame_1box gSpriteBank43Frame013;
extern const struct sprite_frame_1box gSpriteBank43Frame014;
extern const struct sprite_frame_1box gSpriteBank43Frame015;
extern const struct sprite_frame_1box gSpriteBank43Frame016;
extern const struct sprite_frame_1box gSpriteBank43Frame017;
extern const struct sprite_frame_1box gSpriteBank43Frame018;
extern const struct sprite_frame_1box gSpriteBank43Frame019;
extern const struct sprite_frame_1box gSpriteBank43Frame020;
extern const struct sprite_piece_pos gSpriteBank43Frame000Pos[4];
extern const struct sprite_piece_pos gSpriteBank43Frame001Pos[4];
extern const struct sprite_piece_pos gSpriteBank43Frame002Pos[4];
extern const struct sprite_piece_pos gSpriteBank43Frame003Pos[1];
extern const struct sprite_piece_pos gSpriteBank43Frame004Pos[1];
extern const struct sprite_piece_pos gSpriteBank43Frame005Pos[1];
extern const struct sprite_piece_pos gSpriteBank43Frame006Pos[1];
extern const struct sprite_piece_pos gSpriteBank43Frame007Pos[3];
extern const struct sprite_piece_pos gSpriteBank43Frame008Pos[3];
extern const struct sprite_piece_pos gSpriteBank43Frame009Pos[5];
extern const struct sprite_piece_pos gSpriteBank43Frame010Pos[1];
extern const struct sprite_piece_pos gSpriteBank43Frame011Pos[5];
extern const struct sprite_piece_pos gSpriteBank43Frame012Pos[3];
extern const struct sprite_piece_pos gSpriteBank43Frame013Pos[3];
extern const struct sprite_piece_pos gSpriteBank43Frame014Pos[1];
extern const struct sprite_piece_pos gSpriteBank43Frame015Pos[1];
extern const struct sprite_piece_pos gSpriteBank43Frame016Pos[1];
extern const struct sprite_piece_pos gSpriteBank43Frame017Pos[1];
extern const struct sprite_piece_pos gSpriteBank43Frame018Pos[4];
extern const struct sprite_piece_pos gSpriteBank43Frame019Pos[4];
extern const struct sprite_piece_pos gSpriteBank43Frame020Pos[4];
extern const u8 gSpriteBank43Frame000Pieces[4];
extern const u8 gSpriteBank43Frame001Pieces[4];
extern const u8 gSpriteBank43Frame002Pieces[4];
extern const u8 gSpriteBank43Frame003Pieces[1];
extern const u8 gSpriteBank43Frame004Pieces[1];
extern const u8 gSpriteBank43Frame005Pieces[1];
extern const u8 gSpriteBank43Frame006Pieces[1];
extern const u8 gSpriteBank43Frame007Pieces[3];
extern const u8 gSpriteBank43Frame008Pieces[3];
extern const u8 gSpriteBank43Frame009Pieces[5];
extern const u8 gSpriteBank43Frame010Pieces[1];
extern const u8 gSpriteBank43Frame011Pieces[5];
extern const u8 gSpriteBank43Frame012Pieces[3];
extern const u8 gSpriteBank43Frame013Pieces[3];
extern const u8 gSpriteBank43Frame014Pieces[1];
extern const u8 gSpriteBank43Frame015Pieces[1];
extern const u8 gSpriteBank43Frame016Pieces[1];
extern const u8 gSpriteBank43Frame017Pieces[1];
extern const u8 gSpriteBank43Frame018Pieces[4];
extern const u8 gSpriteBank43Frame019Pieces[4];
extern const u8 gSpriteBank43Frame020Pieces[4];

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
    SPRITE_FRAME(gSpriteBank43Frame000, SPRITE_TILES_BANK43 + 0x00000),
    { { -11, -11, 22, 23 } },
};
const struct sprite_frame_1box gSpriteBank43Frame001 = {
    SPRITE_FRAME(gSpriteBank43Frame001, SPRITE_TILES_BANK43 + 0x00120),
    { { -17, -18, 38, 39 } },
};
const struct sprite_frame_1box gSpriteBank43Frame002 = {
    SPRITE_FRAME(gSpriteBank43Frame002, SPRITE_TILES_BANK43 + 0x003c0),
    { { -20, -19, 40, 43 } },
};
const struct sprite_frame_1box gSpriteBank43Frame003 = {
    SPRITE_FRAME(gSpriteBank43Frame003, SPRITE_TILES_BANK43 + 0x00760),
    { { -25, -25, 49, 55 } },
};
const struct sprite_frame_1box gSpriteBank43Frame004 = {
    SPRITE_FRAME(gSpriteBank43Frame004, SPRITE_TILES_BANK43 + 0x00f60),
    { { -25, -26, 49, 57 } },
};
const struct sprite_frame_1box gSpriteBank43Frame005 = {
    SPRITE_FRAME(gSpriteBank43Frame005, SPRITE_TILES_BANK43 + 0x01760),
    { { -25, -25, 49, 55 } },
};
const struct sprite_frame_1box gSpriteBank43Frame006 = {
    SPRITE_FRAME(gSpriteBank43Frame006, SPRITE_TILES_BANK43 + 0x01f60),
    { { -24, -24, 47, 54 } },
};
const struct sprite_frame_1box gSpriteBank43Frame007 = {
    SPRITE_FRAME(gSpriteBank43Frame007, SPRITE_TILES_BANK43 + 0x02760),
    { { -23, -24, 46, 53 } },
};
const struct sprite_frame_1box gSpriteBank43Frame008 = {
    SPRITE_FRAME(gSpriteBank43Frame008, SPRITE_TILES_BANK43 + 0x02ca0),
    { { -22, -22, 44, 49 } },
};
const struct sprite_frame_1box gSpriteBank43Frame009 = {
    SPRITE_FRAME(gSpriteBank43Frame009, SPRITE_TILES_BANK43 + 0x031c0),
    { { -21, -21, 42, 47 } },
};
const struct sprite_frame_1box gSpriteBank43Frame010 = {
    SPRITE_FRAME(gSpriteBank43Frame010, SPRITE_TILES_BANK43 + 0x035a0),
    { { -25, -26, 49, 57 } },
};
const struct sprite_frame_1box gSpriteBank43Frame011 = {
    SPRITE_FRAME(gSpriteBank43Frame011, SPRITE_TILES_BANK43 + 0x031c0),
    { { -21, -23, 42, 47 } },
};
const struct sprite_frame_1box gSpriteBank43Frame012 = {
    SPRITE_FRAME(gSpriteBank43Frame012, SPRITE_TILES_BANK43 + 0x02ca0),
    { { -22, -24, 44, 49 } },
};
const struct sprite_frame_1box gSpriteBank43Frame013 = {
    SPRITE_FRAME(gSpriteBank43Frame013, SPRITE_TILES_BANK43 + 0x02760),
    { { -23, -26, 46, 53 } },
};
const struct sprite_frame_1box gSpriteBank43Frame014 = {
    SPRITE_FRAME(gSpriteBank43Frame014, SPRITE_TILES_BANK43 + 0x01f60),
    { { -24, -26, 47, 54 } },
};
const struct sprite_frame_1box gSpriteBank43Frame015 = {
    SPRITE_FRAME(gSpriteBank43Frame015, SPRITE_TILES_BANK43 + 0x01760),
    { { -25, -27, 49, 55 } },
};
const struct sprite_frame_1box gSpriteBank43Frame016 = {
    SPRITE_FRAME(gSpriteBank43Frame016, SPRITE_TILES_BANK43 + 0x035a0),
    { { -25, -28, 49, 57 } },
};
const struct sprite_frame_1box gSpriteBank43Frame017 = {
    SPRITE_FRAME(gSpriteBank43Frame017, SPRITE_TILES_BANK43 + 0x00760),
    { { -25, -27, 49, 55 } },
};
const struct sprite_frame_1box gSpriteBank43Frame018 = {
    SPRITE_FRAME(gSpriteBank43Frame018, SPRITE_TILES_BANK43 + 0x003c0),
    { { -20, -21, 40, 43 } },
};
const struct sprite_frame_1box gSpriteBank43Frame019 = {
    SPRITE_FRAME(gSpriteBank43Frame019, SPRITE_TILES_BANK43 + 0x00120),
    { { -17, -20, 38, 39 } },
};
const struct sprite_frame_1box gSpriteBank43Frame020 = {
    SPRITE_FRAME(gSpriteBank43Frame020, SPRITE_TILES_BANK43 + 0x00000),
    { { -11, -13, 22, 23 } },
};

const struct sprite_piece_pos gSpriteBank43Frame000Pos[4] = { { -11, -11 }, { 5, -6 }, { -9, 5 }, { 7, 5 } };
const struct sprite_piece_pos gSpriteBank43Frame001Pos[4] = { { -17, -18 }, { 15, -5 }, { -9, 14 }, { 7, 14 } };
const struct sprite_piece_pos gSpriteBank43Frame002Pos[4] = { { -20, -19 }, { 12, -7 }, { 20, -2 }, { -18, 13 } };
const struct sprite_piece_pos gSpriteBank43Frame003Pos[1] = { { -25, -25 } };
const struct sprite_piece_pos gSpriteBank43Frame004Pos[1] = { { -25, -26 } };
const struct sprite_piece_pos gSpriteBank43Frame005Pos[1] = { { -25, -25 } };
const struct sprite_piece_pos gSpriteBank43Frame006Pos[1] = { { -24, -24 } };
const struct sprite_piece_pos gSpriteBank43Frame007Pos[3] = { { -23, -24 }, { 9, -17 }, { 9, 18 } };
const struct sprite_piece_pos gSpriteBank43Frame008Pos[3] = { { -22, -22 }, { 10, -6 }, { 10, 26 } };
const struct sprite_piece_pos gSpriteBank43Frame009Pos[5] = { { -12, -21 }, { 11, -21 }, { 20, -5 }, { -21, 11 }, { 11, 11 } };
const struct sprite_piece_pos gSpriteBank43Frame010Pos[1] = { { -25, -26 } };
const struct sprite_piece_pos gSpriteBank43Frame011Pos[5] = { { -12, -23 }, { 11, -23 }, { 20, -7 }, { -21, 9 }, { 11, 9 } };
const struct sprite_piece_pos gSpriteBank43Frame012Pos[3] = { { -22, -24 }, { 10, -8 }, { 10, 24 } };
const struct sprite_piece_pos gSpriteBank43Frame013Pos[3] = { { -23, -26 }, { 9, -19 }, { 9, 16 } };
const struct sprite_piece_pos gSpriteBank43Frame014Pos[1] = { { -24, -26 } };
const struct sprite_piece_pos gSpriteBank43Frame015Pos[1] = { { -25, -27 } };
const struct sprite_piece_pos gSpriteBank43Frame016Pos[1] = { { -25, -28 } };
const struct sprite_piece_pos gSpriteBank43Frame017Pos[1] = { { -25, -27 } };
const struct sprite_piece_pos gSpriteBank43Frame018Pos[4] = { { -20, -21 }, { 12, -9 }, { 20, -4 }, { -18, 11 } };
const struct sprite_piece_pos gSpriteBank43Frame019Pos[4] = { { -17, -20 }, { 15, -7 }, { -9, 12 }, { 7, 12 } };
const struct sprite_piece_pos gSpriteBank43Frame020Pos[4] = { { -11, -13 }, { 5, -8 }, { -9, 3 }, { 7, 3 } };

const u8 gSpriteBank43Frame000Pieces[4] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank43Frame001Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank43Frame002Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank43Frame003Pieces[1] = { SPRITE_PIECE(2, 3) };
const u8 gSpriteBank43Frame004Pieces[1] = { SPRITE_PIECE(2, 3) };
const u8 gSpriteBank43Frame005Pieces[1] = { SPRITE_PIECE(2, 3) };
const u8 gSpriteBank43Frame006Pieces[1] = { SPRITE_PIECE(2, 3) };
const u8 gSpriteBank43Frame007Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank43Frame008Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank43Frame009Pieces[5] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank43Frame010Pieces[1] = { SPRITE_PIECE(2, 3) };
const u8 gSpriteBank43Frame011Pieces[5] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank43Frame012Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank43Frame013Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank43Frame014Pieces[1] = { SPRITE_PIECE(2, 3) };
const u8 gSpriteBank43Frame015Pieces[1] = { SPRITE_PIECE(2, 3) };
const u8 gSpriteBank43Frame016Pieces[1] = { SPRITE_PIECE(2, 3) };
const u8 gSpriteBank43Frame017Pieces[1] = { SPRITE_PIECE(2, 3) };
const u8 gSpriteBank43Frame018Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank43Frame019Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank43Frame020Pieces[4] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };

/* ---------------------------------------------------------------------- */
/* Bank 44: 1 animation, 13 frames, tiles in gSpriteBank44Tiles (SPRITE_TILES_BANK44). */

extern const u16 gSpriteBank44Anim00Seq[13];
extern const struct sprite_frame_1box gSpriteBank44Frame000;
extern const struct sprite_frame_1box gSpriteBank44Frame001;
extern const struct sprite_frame_1box gSpriteBank44Frame002;
extern const struct sprite_frame_1box gSpriteBank44Frame003;
extern const struct sprite_frame_1box gSpriteBank44Frame004;
extern const struct sprite_frame_1box gSpriteBank44Frame005;
extern const struct sprite_frame_1box gSpriteBank44Frame006;
extern const struct sprite_frame_1box gSpriteBank44Frame007;
extern const struct sprite_frame_1box gSpriteBank44Frame008;
extern const struct sprite_frame_1box gSpriteBank44Frame009;
extern const struct sprite_frame_1box gSpriteBank44Frame010;
extern const struct sprite_frame_1box gSpriteBank44Frame011;
extern const struct sprite_frame_1box gSpriteBank44Frame012;
extern const struct sprite_piece_pos gSpriteBank44Frame000Pos[1];
extern const struct sprite_piece_pos gSpriteBank44Frame001Pos[3];
extern const struct sprite_piece_pos gSpriteBank44Frame002Pos[3];
extern const struct sprite_piece_pos gSpriteBank44Frame003Pos[2];
extern const struct sprite_piece_pos gSpriteBank44Frame004Pos[2];
extern const struct sprite_piece_pos gSpriteBank44Frame005Pos[2];
extern const struct sprite_piece_pos gSpriteBank44Frame006Pos[3];
extern const struct sprite_piece_pos gSpriteBank44Frame007Pos[2];
extern const struct sprite_piece_pos gSpriteBank44Frame008Pos[3];
extern const struct sprite_piece_pos gSpriteBank44Frame009Pos[3];
extern const struct sprite_piece_pos gSpriteBank44Frame010Pos[2];
extern const struct sprite_piece_pos gSpriteBank44Frame011Pos[1];
extern const struct sprite_piece_pos gSpriteBank44Frame012Pos[3];
extern const u8 gSpriteBank44Frame000Pieces[1];
extern const u8 gSpriteBank44Frame001Pieces[3];
extern const u8 gSpriteBank44Frame002Pieces[3];
extern const u8 gSpriteBank44Frame003Pieces[2];
extern const u8 gSpriteBank44Frame004Pieces[2];
extern const u8 gSpriteBank44Frame005Pieces[2];
extern const u8 gSpriteBank44Frame006Pieces[3];
extern const u8 gSpriteBank44Frame007Pieces[2];
extern const u8 gSpriteBank44Frame008Pieces[3];
extern const u8 gSpriteBank44Frame009Pieces[3];
extern const u8 gSpriteBank44Frame010Pieces[2];
extern const u8 gSpriteBank44Frame011Pieces[1];
extern const u8 gSpriteBank44Frame012Pieces[3];

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
    SPRITE_FRAME(gSpriteBank44Frame000, SPRITE_TILES_BANK44 + 0x00000),
    { { -3, -7, 7, 14 } },
};
const struct sprite_frame_1box gSpriteBank44Frame001 = {
    SPRITE_FRAME(gSpriteBank44Frame001, SPRITE_TILES_BANK44 + 0x00040),
    { { -4, -8, 9, 16 } },
};
const struct sprite_frame_1box gSpriteBank44Frame002 = {
    SPRITE_FRAME(gSpriteBank44Frame002, SPRITE_TILES_BANK44 + 0x000e0),
    { { -5, -9, 9, 17 } },
};
const struct sprite_frame_1box gSpriteBank44Frame003 = {
    SPRITE_FRAME(gSpriteBank44Frame003, SPRITE_TILES_BANK44 + 0x00180),
    { { -4, -5, 9, 11 } },
};
const struct sprite_frame_1box gSpriteBank44Frame004 = {
    SPRITE_FRAME(gSpriteBank44Frame004, SPRITE_TILES_BANK44 + 0x00200),
    { { -5, -5, 9, 13 } },
};
const struct sprite_frame_1box gSpriteBank44Frame005 = {
    SPRITE_FRAME(gSpriteBank44Frame005, SPRITE_TILES_BANK44 + 0x00280),
    { { -5, -9, 11, 17 } },
};
const struct sprite_frame_1box gSpriteBank44Frame006 = {
    SPRITE_FRAME(gSpriteBank44Frame006, SPRITE_TILES_BANK44 + 0x00320),
    { { -3, -11, 10, 18 } },
};
const struct sprite_frame_1box gSpriteBank44Frame007 = {
    SPRITE_FRAME(gSpriteBank44Frame007, SPRITE_TILES_BANK44 + 0x003c0),
    { { -3, -9, 8, 15 } },
};
const struct sprite_frame_1box gSpriteBank44Frame008 = {
    SPRITE_FRAME(gSpriteBank44Frame008, SPRITE_TILES_BANK44 + 0x00420),
    { { -4, -11, 9, 19 } },
};
const struct sprite_frame_1box gSpriteBank44Frame009 = {
    SPRITE_FRAME(gSpriteBank44Frame009, SPRITE_TILES_BANK44 + 0x004a0),
    { { -4, -11, 8, 18 } },
};
const struct sprite_frame_1box gSpriteBank44Frame010 = {
    SPRITE_FRAME(gSpriteBank44Frame010, SPRITE_TILES_BANK44 + 0x00540),
    { { -4, -11, 7, 17 } },
};
const struct sprite_frame_1box gSpriteBank44Frame011 = {
    SPRITE_FRAME(gSpriteBank44Frame011, SPRITE_TILES_BANK44 + 0x005a0),
    { { -4, -6, 7, 13 } },
};
const struct sprite_frame_1box gSpriteBank44Frame012 = {
    SPRITE_FRAME(gSpriteBank44Frame012, SPRITE_TILES_BANK44 + 0x005e0),
    { { -4, -9, 8, 16 } },
};

const struct sprite_piece_pos gSpriteBank44Frame000Pos[1] = { { -3, -7 } };
const struct sprite_piece_pos gSpriteBank44Frame001Pos[3] = { { -4, -7 }, { 4, -8 }, { -4, 8 } };
const struct sprite_piece_pos gSpriteBank44Frame002Pos[3] = { { -5, -9 }, { 3, -9 }, { 0, 7 } };
const struct sprite_piece_pos gSpriteBank44Frame003Pos[2] = { { -4, -4 }, { 4, -5 } };
const struct sprite_piece_pos gSpriteBank44Frame004Pos[2] = { { -5, -4 }, { 3, -5 } };
const struct sprite_piece_pos gSpriteBank44Frame005Pos[2] = { { -5, -9 }, { -2, 7 } };
const struct sprite_piece_pos gSpriteBank44Frame006Pos[3] = { { -3, -11 }, { 5, -9 }, { -3, 5 } };
const struct sprite_piece_pos gSpriteBank44Frame007Pos[2] = { { -3, -9 }, { 5, -7 } };
const struct sprite_piece_pos gSpriteBank44Frame008Pos[3] = { { -4, -8 }, { 4, -11 }, { -4, 5 } };
const struct sprite_piece_pos gSpriteBank44Frame009Pos[3] = { { -4, -9 }, { 4, -10 }, { -3, 6 } };
const struct sprite_piece_pos gSpriteBank44Frame010Pos[2] = { { -4, -10 }, { -1, 6 } };
const struct sprite_piece_pos gSpriteBank44Frame011Pos[1] = { { -4, -6 } };
const struct sprite_piece_pos gSpriteBank44Frame012Pos[3] = { { -4, -9 }, { 4, 0 }, { -1, 7 } };

const u8 gSpriteBank44Frame000Pieces[1] = { SPRITE_PIECE(2, 8) };
const u8 gSpriteBank44Frame001Pieces[3] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank44Frame002Pieces[3] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank44Frame003Pieces[2] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank44Frame004Pieces[2] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank44Frame005Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank44Frame006Pieces[3] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank44Frame007Pieces[2] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank44Frame008Pieces[3] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank44Frame009Pieces[3] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank44Frame010Pieces[2] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank44Frame011Pieces[1] = { SPRITE_PIECE(2, 8) };
const u8 gSpriteBank44Frame012Pieces[3] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };

/* ---------------------------------------------------------------------- */
/* Bank 45: 1 animation, 13 frames, tiles in gSpriteBank45Tiles (SPRITE_TILES_BANK45). */

extern const u16 gSpriteBank45Anim00Seq[13];
extern const struct sprite_frame gSpriteBank45Frame000;
extern const struct sprite_frame gSpriteBank45Frame001;
extern const struct sprite_frame gSpriteBank45Frame002;
extern const struct sprite_frame gSpriteBank45Frame003;
extern const struct sprite_frame gSpriteBank45Frame004;
extern const struct sprite_frame gSpriteBank45Frame005;
extern const struct sprite_frame gSpriteBank45Frame006;
extern const struct sprite_frame gSpriteBank45Frame007;
extern const struct sprite_frame gSpriteBank45Frame008;
extern const struct sprite_frame gSpriteBank45Frame009;
extern const struct sprite_frame gSpriteBank45Frame010;
extern const struct sprite_frame gSpriteBank45Frame011;
extern const struct sprite_frame gSpriteBank45Frame012;
extern const struct sprite_piece_pos gSpriteBank45Frame000Pos[1];
extern const struct sprite_piece_pos gSpriteBank45Frame001Pos[1];
extern const struct sprite_piece_pos gSpriteBank45Frame002Pos[1];
extern const struct sprite_piece_pos gSpriteBank45Frame003Pos[1];
extern const struct sprite_piece_pos gSpriteBank45Frame004Pos[1];
extern const struct sprite_piece_pos gSpriteBank45Frame005Pos[1];
extern const struct sprite_piece_pos gSpriteBank45Frame006Pos[1];
extern const struct sprite_piece_pos gSpriteBank45Frame007Pos[1];
extern const struct sprite_piece_pos gSpriteBank45Frame008Pos[1];
extern const struct sprite_piece_pos gSpriteBank45Frame009Pos[1];
extern const struct sprite_piece_pos gSpriteBank45Frame010Pos[1];
extern const struct sprite_piece_pos gSpriteBank45Frame011Pos[1];
extern const struct sprite_piece_pos gSpriteBank45Frame012Pos[1];
extern const u8 gSpriteBank45Frame000Pieces[1];
extern const u8 gSpriteBank45Frame001Pieces[1];
extern const u8 gSpriteBank45Frame002Pieces[1];
extern const u8 gSpriteBank45Frame003Pieces[1];
extern const u8 gSpriteBank45Frame004Pieces[1];
extern const u8 gSpriteBank45Frame005Pieces[1];
extern const u8 gSpriteBank45Frame006Pieces[1];
extern const u8 gSpriteBank45Frame007Pieces[1];
extern const u8 gSpriteBank45Frame008Pieces[1];
extern const u8 gSpriteBank45Frame009Pieces[1];
extern const u8 gSpriteBank45Frame010Pieces[1];
extern const u8 gSpriteBank45Frame011Pieces[1];
extern const u8 gSpriteBank45Frame012Pieces[1];

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

const struct sprite_frame gSpriteBank45Frame000 = SPRITE_FRAME(gSpriteBank45Frame000, SPRITE_TILES_BANK45 + 0x00000);
const struct sprite_frame gSpriteBank45Frame001 = SPRITE_FRAME(gSpriteBank45Frame001, SPRITE_TILES_BANK45 + 0x00400);
const struct sprite_frame gSpriteBank45Frame002 = SPRITE_FRAME(gSpriteBank45Frame002, SPRITE_TILES_BANK45 + 0x00800);
const struct sprite_frame gSpriteBank45Frame003 = SPRITE_FRAME(gSpriteBank45Frame003, SPRITE_TILES_BANK45 + 0x00c00);
const struct sprite_frame gSpriteBank45Frame004 = SPRITE_FRAME(gSpriteBank45Frame004, SPRITE_TILES_BANK45 + 0x01000);
const struct sprite_frame gSpriteBank45Frame005 = SPRITE_FRAME(gSpriteBank45Frame005, SPRITE_TILES_BANK45 + 0x01400);
const struct sprite_frame gSpriteBank45Frame006 = SPRITE_FRAME(gSpriteBank45Frame006, SPRITE_TILES_BANK45 + 0x01800);
const struct sprite_frame gSpriteBank45Frame007 = SPRITE_FRAME(gSpriteBank45Frame007, SPRITE_TILES_BANK45 + 0x01c00);
const struct sprite_frame gSpriteBank45Frame008 = SPRITE_FRAME(gSpriteBank45Frame008, SPRITE_TILES_BANK45 + 0x02000);
const struct sprite_frame gSpriteBank45Frame009 = SPRITE_FRAME(gSpriteBank45Frame009, SPRITE_TILES_BANK45 + 0x02400);
const struct sprite_frame gSpriteBank45Frame010 = SPRITE_FRAME(gSpriteBank45Frame010, SPRITE_TILES_BANK45 + 0x02800);
const struct sprite_frame gSpriteBank45Frame011 = SPRITE_FRAME(gSpriteBank45Frame011, SPRITE_TILES_BANK45 + 0x02c00);
const struct sprite_frame gSpriteBank45Frame012 = SPRITE_FRAME(gSpriteBank45Frame012, SPRITE_TILES_BANK45 + 0x03000);

const struct sprite_piece_pos gSpriteBank45Frame000Pos[1] = { { -11, -31 } };
const struct sprite_piece_pos gSpriteBank45Frame001Pos[1] = { { -11, -31 } };
const struct sprite_piece_pos gSpriteBank45Frame002Pos[1] = { { -11, -31 } };
const struct sprite_piece_pos gSpriteBank45Frame003Pos[1] = { { -11, -31 } };
const struct sprite_piece_pos gSpriteBank45Frame004Pos[1] = { { -11, -31 } };
const struct sprite_piece_pos gSpriteBank45Frame005Pos[1] = { { -11, -31 } };
const struct sprite_piece_pos gSpriteBank45Frame006Pos[1] = { { -11, -31 } };
const struct sprite_piece_pos gSpriteBank45Frame007Pos[1] = { { -11, -31 } };
const struct sprite_piece_pos gSpriteBank45Frame008Pos[1] = { { -11, -30 } };
const struct sprite_piece_pos gSpriteBank45Frame009Pos[1] = { { -11, -30 } };
const struct sprite_piece_pos gSpriteBank45Frame010Pos[1] = { { -11, -30 } };
const struct sprite_piece_pos gSpriteBank45Frame011Pos[1] = { { -11, -30 } };
const struct sprite_piece_pos gSpriteBank45Frame012Pos[1] = { { -11, -30 } };

const u8 gSpriteBank45Frame000Pieces[1] = { SPRITE_PIECE(1, 11) };
const u8 gSpriteBank45Frame001Pieces[1] = { SPRITE_PIECE(1, 11) };
const u8 gSpriteBank45Frame002Pieces[1] = { SPRITE_PIECE(1, 11) };
const u8 gSpriteBank45Frame003Pieces[1] = { SPRITE_PIECE(1, 11) };
const u8 gSpriteBank45Frame004Pieces[1] = { SPRITE_PIECE(1, 11) };
const u8 gSpriteBank45Frame005Pieces[1] = { SPRITE_PIECE(1, 11) };
const u8 gSpriteBank45Frame006Pieces[1] = { SPRITE_PIECE(1, 11) };
const u8 gSpriteBank45Frame007Pieces[1] = { SPRITE_PIECE(1, 11) };
const u8 gSpriteBank45Frame008Pieces[1] = { SPRITE_PIECE(1, 11) };
const u8 gSpriteBank45Frame009Pieces[1] = { SPRITE_PIECE(1, 11) };
const u8 gSpriteBank45Frame010Pieces[1] = { SPRITE_PIECE(1, 11) };
const u8 gSpriteBank45Frame011Pieces[1] = { SPRITE_PIECE(1, 11) };
const u8 gSpriteBank45Frame012Pieces[1] = { SPRITE_PIECE(1, 11) };

/* ---------------------------------------------------------------------- */
/* Bank 46: 1 animation, 2 frames, tiles in gSpriteBank46Tiles (SPRITE_TILES_BANK46). */

extern const u16 gSpriteBank46Anim00Seq[3];
extern const struct sprite_frame_1box gSpriteBank46Frame000;
extern const struct sprite_frame_1box gSpriteBank46Frame001;
extern const struct sprite_piece_pos gSpriteBank46Frame000Pos[2];
extern const struct sprite_piece_pos gSpriteBank46Frame001Pos[2];
extern const u8 gSpriteBank46Frame000Pieces[2];
extern const u8 gSpriteBank46Frame001Pieces[2];

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
    SPRITE_FRAME(gSpriteBank46Frame000, SPRITE_TILES_BANK46 + 0x00000),
    { { -8, -4, 16, 9 } },
};
const struct sprite_frame_1box gSpriteBank46Frame001 = {
    SPRITE_FRAME(gSpriteBank46Frame001, SPRITE_TILES_BANK46 + 0x000a0),
    { { -8, -4, 16, 9 } },
};

const struct sprite_piece_pos gSpriteBank46Frame000Pos[2] = { { -8, -4 }, { 8, -2 } };
const struct sprite_piece_pos gSpriteBank46Frame001Pos[2] = { { -8, -4 }, { 8, -2 } };

const u8 gSpriteBank46Frame000Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank46Frame001Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0) };

/* ---------------------------------------------------------------------- */
/* Bank 47: 13 animations, 43 frames, tiles in gSpriteBank47Tiles (SPRITE_TILES_BANK47). */

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
extern const struct sprite_frame_1box gSpriteBank47Frame000;
extern const struct sprite_frame_1box gSpriteBank47Frame001;
extern const struct sprite_frame_1box gSpriteBank47Frame002;
extern const struct sprite_frame_1box gSpriteBank47Frame003;
extern const struct sprite_frame_1box gSpriteBank47Frame004;
extern const struct sprite_frame_1box gSpriteBank47Frame005;
extern const struct sprite_frame_1box gSpriteBank47Frame006;
extern const struct sprite_frame_1box gSpriteBank47Frame007;
extern const struct sprite_frame_1box gSpriteBank47Frame008;
extern const struct sprite_frame_1box gSpriteBank47Frame009;
extern const struct sprite_frame_1box gSpriteBank47Frame010;
extern const struct sprite_frame_1box gSpriteBank47Frame011;
extern const struct sprite_frame_1box gSpriteBank47Frame012;
extern const struct sprite_frame_1box gSpriteBank47Frame013;
extern const struct sprite_frame_1box gSpriteBank47Frame014;
extern const struct sprite_frame_1box gSpriteBank47Frame015;
extern const struct sprite_frame_1box gSpriteBank47Frame016;
extern const struct sprite_frame_1box gSpriteBank47Frame017;
extern const struct sprite_frame_1box gSpriteBank47Frame018;
extern const struct sprite_frame_1box gSpriteBank47Frame019;
extern const struct sprite_frame_1box gSpriteBank47Frame020;
extern const struct sprite_frame_1box gSpriteBank47Frame021;
extern const struct sprite_frame_1box gSpriteBank47Frame022;
extern const struct sprite_frame_1box gSpriteBank47Frame023;
extern const struct sprite_frame gSpriteBank47Frame024;
extern const struct sprite_frame_1box gSpriteBank47Frame025;
extern const struct sprite_frame_1box gSpriteBank47Frame026;
extern const struct sprite_frame_1box gSpriteBank47Frame027;
extern const struct sprite_frame_1box gSpriteBank47Frame028;
extern const struct sprite_frame_1box gSpriteBank47Frame029;
extern const struct sprite_frame_1box gSpriteBank47Frame030;
extern const struct sprite_frame_1box gSpriteBank47Frame031;
extern const struct sprite_frame_1box gSpriteBank47Frame032;
extern const struct sprite_frame_1box gSpriteBank47Frame033;
extern const struct sprite_frame_1box gSpriteBank47Frame034;
extern const struct sprite_frame_1box gSpriteBank47Frame035;
extern const struct sprite_frame gSpriteBank47Frame036;
extern const struct sprite_frame_1box gSpriteBank47Frame037;
extern const struct sprite_frame_1box gSpriteBank47Frame038;
extern const struct sprite_frame_1box gSpriteBank47Frame039;
extern const struct sprite_frame_1box gSpriteBank47Frame040;
extern const struct sprite_frame_1box gSpriteBank47Frame041;
extern const struct sprite_frame_1box gSpriteBank47Frame042;
extern const struct sprite_piece_pos gSpriteBank47Frame000Pos[2];
extern const struct sprite_piece_pos gSpriteBank47Frame001Pos[4];
extern const struct sprite_piece_pos gSpriteBank47Frame002Pos[2];
extern const struct sprite_piece_pos gSpriteBank47Frame003Pos[2];
extern const struct sprite_piece_pos gSpriteBank47Frame004Pos[2];
extern const struct sprite_piece_pos gSpriteBank47Frame005Pos[2];
extern const struct sprite_piece_pos gSpriteBank47Frame006Pos[2];
extern const struct sprite_piece_pos gSpriteBank47Frame007Pos[3];
extern const struct sprite_piece_pos gSpriteBank47Frame008Pos[2];
extern const struct sprite_piece_pos gSpriteBank47Frame009Pos[3];
extern const struct sprite_piece_pos gSpriteBank47Frame010Pos[2];
extern const struct sprite_piece_pos gSpriteBank47Frame011Pos[1];
extern const struct sprite_piece_pos gSpriteBank47Frame012Pos[1];
extern const struct sprite_piece_pos gSpriteBank47Frame013Pos[1];
extern const struct sprite_piece_pos gSpriteBank47Frame014Pos[1];
extern const struct sprite_piece_pos gSpriteBank47Frame015Pos[1];
extern const struct sprite_piece_pos gSpriteBank47Frame016Pos[1];
extern const struct sprite_piece_pos gSpriteBank47Frame017Pos[1];
extern const struct sprite_piece_pos gSpriteBank47Frame018Pos[1];
extern const struct sprite_piece_pos gSpriteBank47Frame019Pos[1];
extern const struct sprite_piece_pos gSpriteBank47Frame020Pos[1];
extern const struct sprite_piece_pos gSpriteBank47Frame021Pos[1];
extern const struct sprite_piece_pos gSpriteBank47Frame022Pos[1];
extern const struct sprite_piece_pos gSpriteBank47Frame023Pos[1];
extern const struct sprite_piece_pos gSpriteBank47Frame024Pos[1];
extern const struct sprite_piece_pos gSpriteBank47Frame025Pos[2];
extern const struct sprite_piece_pos gSpriteBank47Frame026Pos[2];
extern const struct sprite_piece_pos gSpriteBank47Frame027Pos[1];
extern const struct sprite_piece_pos gSpriteBank47Frame028Pos[2];
extern const struct sprite_piece_pos gSpriteBank47Frame029Pos[3];
extern const struct sprite_piece_pos gSpriteBank47Frame030Pos[2];
extern const struct sprite_piece_pos gSpriteBank47Frame031Pos[2];
extern const struct sprite_piece_pos gSpriteBank47Frame032Pos[2];
extern const struct sprite_piece_pos gSpriteBank47Frame033Pos[2];
extern const struct sprite_piece_pos gSpriteBank47Frame034Pos[2];
extern const struct sprite_piece_pos gSpriteBank47Frame035Pos[2];
extern const struct sprite_piece_pos gSpriteBank47Frame037Pos[1];
extern const struct sprite_piece_pos gSpriteBank47Frame038Pos[2];
extern const struct sprite_piece_pos gSpriteBank47Frame039Pos[2];
extern const struct sprite_piece_pos gSpriteBank47Frame040Pos[2];
extern const struct sprite_piece_pos gSpriteBank47Frame041Pos[2];
extern const struct sprite_piece_pos gSpriteBank47Frame042Pos[2];
extern const u8 gSpriteBank47Frame000Pieces[2];
extern const u8 gSpriteBank47Frame001Pieces[4];
extern const u8 gSpriteBank47Frame002Pieces[2];
extern const u8 gSpriteBank47Frame003Pieces[2];
extern const u8 gSpriteBank47Frame004Pieces[2];
extern const u8 gSpriteBank47Frame005Pieces[2];
extern const u8 gSpriteBank47Frame006Pieces[2];
extern const u8 gSpriteBank47Frame007Pieces[3];
extern const u8 gSpriteBank47Frame008Pieces[2];
extern const u8 gSpriteBank47Frame009Pieces[3];
extern const u8 gSpriteBank47Frame010Pieces[2];
extern const u8 gSpriteBank47Frame011Pieces[1];
extern const u8 gSpriteBank47Frame012Pieces[1];
extern const u8 gSpriteBank47Frame013Pieces[1];
extern const u8 gSpriteBank47Frame014Pieces[1];
extern const u8 gSpriteBank47Frame015Pieces[1];
extern const u8 gSpriteBank47Frame016Pieces[1];
extern const u8 gSpriteBank47Frame017Pieces[1];
extern const u8 gSpriteBank47Frame018Pieces[1];
extern const u8 gSpriteBank47Frame019Pieces[1];
extern const u8 gSpriteBank47Frame020Pieces[1];
extern const u8 gSpriteBank47Frame021Pieces[1];
extern const u8 gSpriteBank47Frame022Pieces[1];
extern const u8 gSpriteBank47Frame023Pieces[1];
extern const u8 gSpriteBank47Frame024Pieces[1];
extern const u8 gSpriteBank47Frame025Pieces[2];
extern const u8 gSpriteBank47Frame026Pieces[2];
extern const u8 gSpriteBank47Frame027Pieces[1];
extern const u8 gSpriteBank47Frame028Pieces[2];
extern const u8 gSpriteBank47Frame029Pieces[3];
extern const u8 gSpriteBank47Frame030Pieces[2];
extern const u8 gSpriteBank47Frame031Pieces[2];
extern const u8 gSpriteBank47Frame032Pieces[2];
extern const u8 gSpriteBank47Frame033Pieces[2];
extern const u8 gSpriteBank47Frame034Pieces[2];
extern const u8 gSpriteBank47Frame035Pieces[2];
extern const u8 gSpriteBank47Frame037Pieces[1];
extern const u8 gSpriteBank47Frame038Pieces[2];
extern const u8 gSpriteBank47Frame039Pieces[2];
extern const u8 gSpriteBank47Frame040Pieces[2];
extern const u8 gSpriteBank47Frame041Pieces[2];
extern const u8 gSpriteBank47Frame042Pieces[2];

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
    SPRITE_FRAME(gSpriteBank47Frame000, SPRITE_TILES_BANK47 + 0x00000),
    { { -6, -11, 13, 23 } },
};
const struct sprite_frame_1box gSpriteBank47Frame001 = {
    SPRITE_FRAME(gSpriteBank47Frame001, SPRITE_TILES_BANK47 + 0x000c0),
    { { -4, -11, 9, 23 } },
};
const struct sprite_frame_1box gSpriteBank47Frame002 = {
    SPRITE_FRAME(gSpriteBank47Frame002, SPRITE_TILES_BANK47 + 0x00180),
    { { -7, -11, 15, 23 } },
};
const struct sprite_frame_1box gSpriteBank47Frame003 = {
    SPRITE_FRAME(gSpriteBank47Frame003, SPRITE_TILES_BANK47 + 0x00240),
    { { -6, -11, 14, 23 } },
};
const struct sprite_frame_1box gSpriteBank47Frame004 = {
    SPRITE_FRAME(gSpriteBank47Frame004, SPRITE_TILES_BANK47 + 0x00300),
    { { -6, -11, 14, 23 } },
};
const struct sprite_frame_1box gSpriteBank47Frame005 = {
    SPRITE_FRAME(gSpriteBank47Frame005, SPRITE_TILES_BANK47 + 0x003a0),
    { { -6, -11, 14, 23 } },
};
const struct sprite_frame_1box gSpriteBank47Frame006 = {
    SPRITE_FRAME(gSpriteBank47Frame006, SPRITE_TILES_BANK47 + 0x00460),
    { { -7, -11, 14, 23 } },
};
const struct sprite_frame_1box gSpriteBank47Frame007 = {
    SPRITE_FRAME(gSpriteBank47Frame007, SPRITE_TILES_BANK47 + 0x00520),
    { { -6, -10, 14, 22 } },
};
const struct sprite_frame_1box gSpriteBank47Frame008 = {
    SPRITE_FRAME(gSpriteBank47Frame008, SPRITE_TILES_BANK47 + 0x005e0),
    { { -6, -11, 13, 23 } },
};
const struct sprite_frame_1box gSpriteBank47Frame009 = {
    SPRITE_FRAME(gSpriteBank47Frame009, SPRITE_TILES_BANK47 + 0x006a0),
    { { -7, -11, 15, 23 } },
};
const struct sprite_frame_1box gSpriteBank47Frame010 = {
    SPRITE_FRAME(gSpriteBank47Frame010, SPRITE_TILES_BANK47 + 0x00760),
    { { -7, -11, 15, 23 } },
};
const struct sprite_frame_1box gSpriteBank47Frame011 = {
    SPRITE_FRAME(gSpriteBank47Frame011, SPRITE_TILES_BANK47 + 0x00820),
    { { -3, -5, 7, 11 } },
};
const struct sprite_frame_1box gSpriteBank47Frame012 = {
    SPRITE_FRAME(gSpriteBank47Frame012, SPRITE_TILES_BANK47 + 0x00860),
    { { -2, -5, 5, 11 } },
};
const struct sprite_frame_1box gSpriteBank47Frame013 = {
    SPRITE_FRAME(gSpriteBank47Frame013, SPRITE_TILES_BANK47 + 0x008a0),
    { { -3, -6, 7, 12 } },
};
const struct sprite_frame_1box gSpriteBank47Frame014 = {
    SPRITE_FRAME(gSpriteBank47Frame014, SPRITE_TILES_BANK47 + 0x008e0),
    { { -3, -6, 7, 12 } },
};
const struct sprite_frame_1box gSpriteBank47Frame015 = {
    SPRITE_FRAME(gSpriteBank47Frame015, SPRITE_TILES_BANK47 + 0x00920),
    { { -3, -6, 7, 12 } },
};
const struct sprite_frame_1box gSpriteBank47Frame016 = {
    SPRITE_FRAME(gSpriteBank47Frame016, SPRITE_TILES_BANK47 + 0x00960),
    { { -3, -6, 7, 12 } },
};
const struct sprite_frame_1box gSpriteBank47Frame017 = {
    SPRITE_FRAME(gSpriteBank47Frame017, SPRITE_TILES_BANK47 + 0x009a0),
    { { -3, -5, 7, 11 } },
};
const struct sprite_frame_1box gSpriteBank47Frame018 = {
    SPRITE_FRAME(gSpriteBank47Frame018, SPRITE_TILES_BANK47 + 0x009e0),
    { { -3, -5, 7, 11 } },
};
const struct sprite_frame_1box gSpriteBank47Frame019 = {
    SPRITE_FRAME(gSpriteBank47Frame019, SPRITE_TILES_BANK47 + 0x00a20),
    { { -3, -6, 7, 12 } },
};
const struct sprite_frame_1box gSpriteBank47Frame020 = {
    SPRITE_FRAME(gSpriteBank47Frame020, SPRITE_TILES_BANK47 + 0x00a60),
    { { -3, -6, 7, 12 } },
};
const struct sprite_frame_1box gSpriteBank47Frame021 = {
    SPRITE_FRAME(gSpriteBank47Frame021, SPRITE_TILES_BANK47 + 0x00aa0),
    { { -1, -4, 3, 10 } },
};
const struct sprite_frame_1box gSpriteBank47Frame022 = {
    SPRITE_FRAME(gSpriteBank47Frame022, SPRITE_TILES_BANK47 + 0x00ae0),
    { { -1, 2, 3, 4 } },
};
const struct sprite_frame_1box gSpriteBank47Frame023 = {
    SPRITE_FRAME(gSpriteBank47Frame023, SPRITE_TILES_BANK47 + 0x00b00),
    { { -12, -12, 25, 25 } },
};
const struct sprite_frame gSpriteBank47Frame024 = SPRITE_FRAME(gSpriteBank47Frame024, SPRITE_TILES_BANK47 + 0x00d00);
const struct sprite_frame_1box gSpriteBank47Frame025 = {
    SPRITE_FRAME(gSpriteBank47Frame025, SPRITE_TILES_BANK47 + 0x00d40),
    { { -20, -4, 40, 9 } },
};
const struct sprite_frame_1box gSpriteBank47Frame026 = {
    SPRITE_FRAME(gSpriteBank47Frame026, SPRITE_TILES_BANK47 + 0x00da0),
    { { -20, -4, 32, 9 } },
};
const struct sprite_frame_1box gSpriteBank47Frame027 = {
    SPRITE_FRAME(gSpriteBank47Frame027, SPRITE_TILES_BANK47 + 0x00e40),
    { { -20, -4, 24, 9 } },
};
const struct sprite_frame_1box gSpriteBank47Frame028 = {
    SPRITE_FRAME(gSpriteBank47Frame028, SPRITE_TILES_BANK47 + 0x00f40),
    { { -20, -4, 16, 9 } },
};
const struct sprite_frame_1box gSpriteBank47Frame029 = {
    SPRITE_FRAME(gSpriteBank47Frame029, SPRITE_TILES_BANK47 + 0x01060),
    { { -20, -4, 8, 9 } },
};
const struct sprite_frame_1box gSpriteBank47Frame030 = {
    SPRITE_FRAME(gSpriteBank47Frame030, SPRITE_TILES_BANK47 + 0x011c0),
    { { -14, -11, 29, 22 } },
};
const struct sprite_frame_1box gSpriteBank47Frame031 = {
    SPRITE_FRAME(gSpriteBank47Frame031, SPRITE_TILES_BANK47 + 0x01340),
    { { -15, -9, 31, 18 } },
};
const struct sprite_frame_1box gSpriteBank47Frame032 = {
    SPRITE_FRAME(gSpriteBank47Frame032, SPRITE_TILES_BANK47 + 0x01480),
    { { -15, -10, 30, 21 } },
};
const struct sprite_frame_1box gSpriteBank47Frame033 = {
    SPRITE_FRAME(gSpriteBank47Frame033, SPRITE_TILES_BANK47 + 0x01600),
    { { -14, -10, 29, 20 } },
};
const struct sprite_frame_1box gSpriteBank47Frame034 = {
    SPRITE_FRAME(gSpriteBank47Frame034, SPRITE_TILES_BANK47 + 0x01780),
    { { -13, -11, 27, 23 } },
};
const struct sprite_frame_1box gSpriteBank47Frame035 = {
    SPRITE_FRAME(gSpriteBank47Frame035, SPRITE_TILES_BANK47 + 0x018c0),
    { { -27, -6, 55, 13 } },
};
const struct sprite_frame gSpriteBank47Frame036 = { gSpriteBank47Frame037Pos, gSpriteBank47Frame037Pieces, SPRITE_FRAME_TILES(SPRITE_TILES_BANK00 + 0x39f00, 0) };
const struct sprite_frame_1box gSpriteBank47Frame037 = {
    SPRITE_FRAME(gSpriteBank47Frame037, SPRITE_TILES_BANK47 + 0x01ac0),
    { { -27, -6, 11, 12 } },
};
const struct sprite_frame_1box gSpriteBank47Frame038 = {
    SPRITE_FRAME(gSpriteBank47Frame038, SPRITE_TILES_BANK47 + 0x01b40),
    { { -14, -6, 9, 13 } },
};
const struct sprite_frame_1box gSpriteBank47Frame039 = {
    SPRITE_FRAME(gSpriteBank47Frame039, SPRITE_TILES_BANK47 + 0x01bc0),
    { { -3, -6, 9, 13 } },
};
const struct sprite_frame_1box gSpriteBank47Frame040 = {
    SPRITE_FRAME(gSpriteBank47Frame040, SPRITE_TILES_BANK47 + 0x01c40),
    { { 8, -6, 9, 13 } },
};
const struct sprite_frame_1box gSpriteBank47Frame041 = {
    SPRITE_FRAME(gSpriteBank47Frame041, SPRITE_TILES_BANK47 + 0x01cc0),
    { { 19, -6, 9, 13 } },
};
const struct sprite_frame_1box gSpriteBank47Frame042 = {
    SPRITE_FRAME(gSpriteBank47Frame042, SPRITE_TILES_BANK47 + 0x01d40),
    { { -15, -11, 31, 23 } },
};

const struct sprite_piece_pos gSpriteBank47Frame000Pos[2] = { { -6, -11 }, { -6, 5 } };
const struct sprite_piece_pos gSpriteBank47Frame001Pos[4] = { { -4, -9 }, { 4, -11 }, { -4, 5 }, { 4, 10 } };
const struct sprite_piece_pos gSpriteBank47Frame002Pos[2] = { { -6, -11 }, { -7, 5 } };
const struct sprite_piece_pos gSpriteBank47Frame003Pos[2] = { { -6, -11 }, { -6, 5 } };
const struct sprite_piece_pos gSpriteBank47Frame004Pos[2] = { { -6, -11 }, { 0, 5 } };
const struct sprite_piece_pos gSpriteBank47Frame005Pos[2] = { { -5, -11 }, { -6, 5 } };
const struct sprite_piece_pos gSpriteBank47Frame006Pos[2] = { { -6, -11 }, { -7, 5 } };
const struct sprite_piece_pos gSpriteBank47Frame007Pos[3] = { { -6, -10 }, { -5, 6 }, { 3, 6 } };
const struct sprite_piece_pos gSpriteBank47Frame008Pos[2] = { { -6, -11 }, { -6, 5 } };
const struct sprite_piece_pos gSpriteBank47Frame009Pos[3] = { { -7, -11 }, { -3, 5 }, { 5, 10 } };
const struct sprite_piece_pos gSpriteBank47Frame010Pos[2] = { { -7, -11 }, { -5, 5 } };
const struct sprite_piece_pos gSpriteBank47Frame011Pos[1] = { { -3, -5 } };
const struct sprite_piece_pos gSpriteBank47Frame012Pos[1] = { { -2, -5 } };
const struct sprite_piece_pos gSpriteBank47Frame013Pos[1] = { { -3, -6 } };
const struct sprite_piece_pos gSpriteBank47Frame014Pos[1] = { { -3, -6 } };
const struct sprite_piece_pos gSpriteBank47Frame015Pos[1] = { { -3, -6 } };
const struct sprite_piece_pos gSpriteBank47Frame016Pos[1] = { { -3, -6 } };
const struct sprite_piece_pos gSpriteBank47Frame017Pos[1] = { { -3, -5 } };
const struct sprite_piece_pos gSpriteBank47Frame018Pos[1] = { { -3, -5 } };
const struct sprite_piece_pos gSpriteBank47Frame019Pos[1] = { { -3, -6 } };
const struct sprite_piece_pos gSpriteBank47Frame020Pos[1] = { { -3, -6 } };
const struct sprite_piece_pos gSpriteBank47Frame021Pos[1] = { { -1, -4 } };
const struct sprite_piece_pos gSpriteBank47Frame022Pos[1] = { { -1, 2 } };
const struct sprite_piece_pos gSpriteBank47Frame023Pos[1] = { { -12, -12 } };
const struct sprite_piece_pos gSpriteBank47Frame024Pos[1] = { { -3, -5 } };
const struct sprite_piece_pos gSpriteBank47Frame025Pos[2] = { { -20, -4 }, { -12, -3 } };
const struct sprite_piece_pos gSpriteBank47Frame026Pos[2] = { { -20, -4 }, { -4, -3 } };
const struct sprite_piece_pos gSpriteBank47Frame027Pos[1] = { { -20, -4 } };
const struct sprite_piece_pos gSpriteBank47Frame028Pos[2] = { { -20, -4 }, { 12, -3 } };
const struct sprite_piece_pos gSpriteBank47Frame029Pos[3] = { { -20, -4 }, { 12, -4 }, { 20, -3 } };
const struct sprite_piece_pos gSpriteBank47Frame030Pos[2] = { { -14, -11 }, { -12, 5 } };
const struct sprite_piece_pos gSpriteBank47Frame031Pos[2] = { { -15, -9 }, { 0, 7 } };
const struct sprite_piece_pos gSpriteBank47Frame032Pos[2] = { { -15, -10 }, { -12, 6 } };
const struct sprite_piece_pos gSpriteBank47Frame033Pos[2] = { { -14, -10 }, { -13, 6 } };
const struct sprite_piece_pos gSpriteBank47Frame034Pos[2] = { { -13, -11 }, { -7, 5 } };
const struct sprite_piece_pos gSpriteBank47Frame035Pos[2] = { { -27, -6 }, { 5, -6 } };
const struct sprite_piece_pos gSpriteBank47Frame037Pos[1] = { { -27, -6 } };
const struct sprite_piece_pos gSpriteBank47Frame038Pos[2] = { { -14, -6 }, { -6, -5 } };
const struct sprite_piece_pos gSpriteBank47Frame039Pos[2] = { { -3, -6 }, { 5, -5 } };
const struct sprite_piece_pos gSpriteBank47Frame040Pos[2] = { { 8, -5 }, { 16, -5 } };
const struct sprite_piece_pos gSpriteBank47Frame041Pos[2] = { { 19, -6 }, { 27, -5 } };
const struct sprite_piece_pos gSpriteBank47Frame042Pos[2] = { { -15, -11 }, { -15, 5 } };

const u8 gSpriteBank47Frame000Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank47Frame001Pieces[4] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank47Frame002Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank47Frame003Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank47Frame004Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank47Frame005Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank47Frame006Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank47Frame007Pieces[3] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank47Frame008Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank47Frame009Pieces[3] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank47Frame010Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank47Frame011Pieces[1] = { SPRITE_PIECE(2, 8) };
const u8 gSpriteBank47Frame012Pieces[1] = { SPRITE_PIECE(2, 8) };
const u8 gSpriteBank47Frame013Pieces[1] = { SPRITE_PIECE(2, 8) };
const u8 gSpriteBank47Frame014Pieces[1] = { SPRITE_PIECE(2, 8) };
const u8 gSpriteBank47Frame015Pieces[1] = { SPRITE_PIECE(2, 8) };
const u8 gSpriteBank47Frame016Pieces[1] = { SPRITE_PIECE(2, 8) };
const u8 gSpriteBank47Frame017Pieces[1] = { SPRITE_PIECE(2, 8) };
const u8 gSpriteBank47Frame018Pieces[1] = { SPRITE_PIECE(2, 8) };
const u8 gSpriteBank47Frame019Pieces[1] = { SPRITE_PIECE(2, 8) };
const u8 gSpriteBank47Frame020Pieces[1] = { SPRITE_PIECE(2, 8) };
const u8 gSpriteBank47Frame021Pieces[1] = { SPRITE_PIECE(2, 8) };
const u8 gSpriteBank47Frame022Pieces[1] = { SPRITE_PIECE(2, 0) };
const u8 gSpriteBank47Frame023Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank47Frame024Pieces[1] = { SPRITE_PIECE(1, 8) };
const u8 gSpriteBank47Frame025Pieces[2] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank47Frame026Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank47Frame027Pieces[1] = { SPRITE_PIECE(2, 6) };
const u8 gSpriteBank47Frame028Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank47Frame029Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank47Frame030Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank47Frame031Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank47Frame032Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank47Frame033Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank47Frame034Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank47Frame035Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank47Frame037Pieces[1] = { SPRITE_PIECE(2, 1) };
const u8 gSpriteBank47Frame038Pieces[2] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank47Frame039Pieces[2] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank47Frame040Pieces[2] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank47Frame041Pieces[2] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank47Frame042Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 5) };

/* ---------------------------------------------------------------------- */
/* Bank 48: 4 animations, 38 frames, tiles in gSpriteBank48Tiles (SPRITE_TILES_BANK48). */

extern const u16 gSpriteBank48Anim00Seq[10];
extern const u16 gSpriteBank48Anim01Seq[9];
extern const u16 gSpriteBank48Anim02Seq[10];
extern const u16 gSpriteBank48Anim03Seq[10];
extern const struct sprite_frame_1box gSpriteBank48Frame000;
extern const struct sprite_frame_1box gSpriteBank48Frame001;
extern const struct sprite_frame_1box gSpriteBank48Frame002;
extern const struct sprite_frame_1box gSpriteBank48Frame003;
extern const struct sprite_frame_1box gSpriteBank48Frame004;
extern const struct sprite_frame_1box gSpriteBank48Frame005;
extern const struct sprite_frame_1box gSpriteBank48Frame006;
extern const struct sprite_frame_1box gSpriteBank48Frame007;
extern const struct sprite_frame_1box gSpriteBank48Frame008;
extern const struct sprite_frame_1box gSpriteBank48Frame009;
extern const struct sprite_frame_1box gSpriteBank48Frame010;
extern const struct sprite_frame_1box gSpriteBank48Frame011;
extern const struct sprite_frame_1box gSpriteBank48Frame012;
extern const struct sprite_frame_1box gSpriteBank48Frame013;
extern const struct sprite_frame_1box gSpriteBank48Frame014;
extern const struct sprite_frame_1box gSpriteBank48Frame015;
extern const struct sprite_frame_1box gSpriteBank48Frame016;
extern const struct sprite_frame_1box gSpriteBank48Frame017;
extern const struct sprite_frame_1box gSpriteBank48Frame018;
extern const struct sprite_frame_1box gSpriteBank48Frame019;
extern const struct sprite_frame_1box gSpriteBank48Frame020;
extern const struct sprite_frame_1box gSpriteBank48Frame021;
extern const struct sprite_frame_1box gSpriteBank48Frame022;
extern const struct sprite_frame_1box gSpriteBank48Frame023;
extern const struct sprite_frame_1box gSpriteBank48Frame024;
extern const struct sprite_frame_1box gSpriteBank48Frame025;
extern const struct sprite_frame_1box gSpriteBank48Frame026;
extern const struct sprite_frame_1box gSpriteBank48Frame027;
extern const struct sprite_frame_1box gSpriteBank48Frame028;
extern const struct sprite_frame_1box gSpriteBank48Frame029;
extern const struct sprite_frame_1box gSpriteBank48Frame030;
extern const struct sprite_frame_1box gSpriteBank48Frame031;
extern const struct sprite_frame_1box gSpriteBank48Frame032;
extern const struct sprite_frame_1box gSpriteBank48Frame033;
extern const struct sprite_frame_1box gSpriteBank48Frame034;
extern const struct sprite_frame_1box gSpriteBank48Frame035;
extern const struct sprite_frame_1box gSpriteBank48Frame036;
extern const struct sprite_frame_1box gSpriteBank48Frame037;
extern const struct sprite_piece_pos gSpriteBank48Frame000Pos[2];
extern const struct sprite_piece_pos gSpriteBank48Frame001Pos[2];
extern const struct sprite_piece_pos gSpriteBank48Frame002Pos[1];
extern const struct sprite_piece_pos gSpriteBank48Frame003Pos[2];
extern const struct sprite_piece_pos gSpriteBank48Frame004Pos[2];
extern const struct sprite_piece_pos gSpriteBank48Frame005Pos[1];
extern const struct sprite_piece_pos gSpriteBank48Frame006Pos[2];
extern const struct sprite_piece_pos gSpriteBank48Frame007Pos[1];
extern const struct sprite_piece_pos gSpriteBank48Frame008Pos[2];
extern const struct sprite_piece_pos gSpriteBank48Frame009Pos[1];
extern const struct sprite_piece_pos gSpriteBank48Frame010Pos[2];
extern const struct sprite_piece_pos gSpriteBank48Frame011Pos[2];
extern const struct sprite_piece_pos gSpriteBank48Frame012Pos[2];
extern const struct sprite_piece_pos gSpriteBank48Frame013Pos[1];
extern const struct sprite_piece_pos gSpriteBank48Frame014Pos[1];
extern const struct sprite_piece_pos gSpriteBank48Frame015Pos[1];
extern const struct sprite_piece_pos gSpriteBank48Frame016Pos[1];
extern const struct sprite_piece_pos gSpriteBank48Frame017Pos[1];
extern const struct sprite_piece_pos gSpriteBank48Frame018Pos[1];
extern const struct sprite_piece_pos gSpriteBank48Frame019Pos[1];
extern const struct sprite_piece_pos gSpriteBank48Frame020Pos[2];
extern const struct sprite_piece_pos gSpriteBank48Frame021Pos[2];
extern const struct sprite_piece_pos gSpriteBank48Frame022Pos[1];
extern const struct sprite_piece_pos gSpriteBank48Frame023Pos[1];
extern const struct sprite_piece_pos gSpriteBank48Frame024Pos[1];
extern const struct sprite_piece_pos gSpriteBank48Frame025Pos[1];
extern const struct sprite_piece_pos gSpriteBank48Frame026Pos[1];
extern const struct sprite_piece_pos gSpriteBank48Frame027Pos[1];
extern const struct sprite_piece_pos gSpriteBank48Frame028Pos[2];
extern const struct sprite_piece_pos gSpriteBank48Frame029Pos[2];
extern const struct sprite_piece_pos gSpriteBank48Frame030Pos[1];
extern const struct sprite_piece_pos gSpriteBank48Frame031Pos[1];
extern const struct sprite_piece_pos gSpriteBank48Frame032Pos[1];
extern const struct sprite_piece_pos gSpriteBank48Frame033Pos[1];
extern const struct sprite_piece_pos gSpriteBank48Frame034Pos[2];
extern const struct sprite_piece_pos gSpriteBank48Frame035Pos[2];
extern const struct sprite_piece_pos gSpriteBank48Frame036Pos[2];
extern const struct sprite_piece_pos gSpriteBank48Frame037Pos[2];
extern const u8 gSpriteBank48Frame000Pieces[2];
extern const u8 gSpriteBank48Frame001Pieces[2];
extern const u8 gSpriteBank48Frame002Pieces[1];
extern const u8 gSpriteBank48Frame003Pieces[2];
extern const u8 gSpriteBank48Frame004Pieces[2];
extern const u8 gSpriteBank48Frame005Pieces[1];
extern const u8 gSpriteBank48Frame006Pieces[2];
extern const u8 gSpriteBank48Frame007Pieces[1];
extern const u8 gSpriteBank48Frame008Pieces[2];
extern const u8 gSpriteBank48Frame009Pieces[1];
extern const u8 gSpriteBank48Frame010Pieces[2];
extern const u8 gSpriteBank48Frame011Pieces[2];
extern const u8 gSpriteBank48Frame012Pieces[2];
extern const u8 gSpriteBank48Frame013Pieces[1];
extern const u8 gSpriteBank48Frame014Pieces[1];
extern const u8 gSpriteBank48Frame015Pieces[1];
extern const u8 gSpriteBank48Frame016Pieces[1];
extern const u8 gSpriteBank48Frame017Pieces[1];
extern const u8 gSpriteBank48Frame018Pieces[1];
extern const u8 gSpriteBank48Frame019Pieces[1];
extern const u8 gSpriteBank48Frame020Pieces[2];
extern const u8 gSpriteBank48Frame021Pieces[2];
extern const u8 gSpriteBank48Frame022Pieces[1];
extern const u8 gSpriteBank48Frame023Pieces[1];
extern const u8 gSpriteBank48Frame024Pieces[1];
extern const u8 gSpriteBank48Frame025Pieces[1];
extern const u8 gSpriteBank48Frame026Pieces[1];
extern const u8 gSpriteBank48Frame027Pieces[1];
extern const u8 gSpriteBank48Frame028Pieces[2];
extern const u8 gSpriteBank48Frame029Pieces[2];
extern const u8 gSpriteBank48Frame030Pieces[1];
extern const u8 gSpriteBank48Frame031Pieces[1];
extern const u8 gSpriteBank48Frame032Pieces[1];
extern const u8 gSpriteBank48Frame033Pieces[1];
extern const u8 gSpriteBank48Frame034Pieces[2];
extern const u8 gSpriteBank48Frame035Pieces[2];
extern const u8 gSpriteBank48Frame036Pieces[2];
extern const u8 gSpriteBank48Frame037Pieces[2];

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
    SPRITE_FRAME(gSpriteBank48Frame000, SPRITE_TILES_BANK48 + 0x00000),
    { { -18, -27, 35, 55 } },
};
const struct sprite_frame_1box gSpriteBank48Frame001 = {
    SPRITE_FRAME(gSpriteBank48Frame001, SPRITE_TILES_BANK48 + 0x00300),
    { { -18, -28, 35, 55 } },
};
const struct sprite_frame_1box gSpriteBank48Frame002 = {
    SPRITE_FRAME(gSpriteBank48Frame002, SPRITE_TILES_BANK48 + 0x00600),
    { { -18, -29, 35, 56 } },
};
const struct sprite_frame_1box gSpriteBank48Frame003 = {
    SPRITE_FRAME(gSpriteBank48Frame003, SPRITE_TILES_BANK48 + 0x00a00),
    { { -18, -29, 35, 55 } },
};
const struct sprite_frame_1box gSpriteBank48Frame004 = {
    SPRITE_FRAME(gSpriteBank48Frame004, SPRITE_TILES_BANK48 + 0x00d00),
    { { -18, -30, 35, 56 } },
};
const struct sprite_frame_1box gSpriteBank48Frame005 = {
    SPRITE_FRAME(gSpriteBank48Frame005, SPRITE_TILES_BANK48 + 0x01000),
    { { -18, -29, 35, 56 } },
};
const struct sprite_frame_1box gSpriteBank48Frame006 = {
    SPRITE_FRAME(gSpriteBank48Frame006, SPRITE_TILES_BANK48 + 0x01400),
    { { -18, -29, 35, 56 } },
};
const struct sprite_frame_1box gSpriteBank48Frame007 = {
    SPRITE_FRAME(gSpriteBank48Frame007, SPRITE_TILES_BANK48 + 0x01700),
    { { -18, -27, 35, 55 } },
};
const struct sprite_frame_1box gSpriteBank48Frame008 = {
    SPRITE_FRAME(gSpriteBank48Frame008, SPRITE_TILES_BANK48 + 0x01b00),
    { { -18, -27, 35, 55 } },
};
const struct sprite_frame_1box gSpriteBank48Frame009 = {
    SPRITE_FRAME(gSpriteBank48Frame009, SPRITE_TILES_BANK48 + 0x01e00),
    { { -18, -27, 35, 55 } },
};
const struct sprite_frame_1box gSpriteBank48Frame010 = {
    SPRITE_FRAME(gSpriteBank48Frame010, SPRITE_TILES_BANK48 + 0x02200),
    { { -15, -23, 30, 46 } },
};
const struct sprite_frame_1box gSpriteBank48Frame011 = {
    SPRITE_FRAME(gSpriteBank48Frame011, SPRITE_TILES_BANK48 + 0x02500),
    { { -15, -24, 30, 47 } },
};
const struct sprite_frame_1box gSpriteBank48Frame012 = {
    SPRITE_FRAME(gSpriteBank48Frame012, SPRITE_TILES_BANK48 + 0x02800),
    { { -15, -25, 30, 47 } },
};
const struct sprite_frame_1box gSpriteBank48Frame013 = {
    SPRITE_FRAME(gSpriteBank48Frame013, SPRITE_TILES_BANK48 + 0x02b00),
    { { -15, -27, 30, 49 } },
};
const struct sprite_frame_1box gSpriteBank48Frame014 = {
    SPRITE_FRAME(gSpriteBank48Frame014, SPRITE_TILES_BANK48 + 0x02f00),
    { { -15, -27, 30, 50 } },
};
const struct sprite_frame_1box gSpriteBank48Frame015 = {
    SPRITE_FRAME(gSpriteBank48Frame015, SPRITE_TILES_BANK48 + 0x03300),
    { { -15, -26, 30, 49 } },
};
const struct sprite_frame_1box gSpriteBank48Frame016 = {
    SPRITE_FRAME(gSpriteBank48Frame016, SPRITE_TILES_BANK48 + 0x03700),
    { { -15, -26, 30, 50 } },
};
const struct sprite_frame_1box gSpriteBank48Frame017 = {
    SPRITE_FRAME(gSpriteBank48Frame017, SPRITE_TILES_BANK48 + 0x03b00),
    { { -15, -25, 30, 49 } },
};
const struct sprite_frame_1box gSpriteBank48Frame018 = {
    SPRITE_FRAME(gSpriteBank48Frame018, SPRITE_TILES_BANK48 + 0x03f00),
    { { -15, -24, 30, 48 } },
};
const struct sprite_frame_1box gSpriteBank48Frame019 = {
    SPRITE_FRAME(gSpriteBank48Frame019, SPRITE_TILES_BANK48 + 0x04300),
    { { -18, -28, 35, 56 } },
};
const struct sprite_frame_1box gSpriteBank48Frame020 = {
    SPRITE_FRAME(gSpriteBank48Frame020, SPRITE_TILES_BANK48 + 0x04700),
    { { -18, -29, 35, 56 } },
};
const struct sprite_frame_1box gSpriteBank48Frame021 = {
    SPRITE_FRAME(gSpriteBank48Frame021, SPRITE_TILES_BANK48 + 0x04a00),
    { { -18, -30, 35, 57 } },
};
const struct sprite_frame_1box gSpriteBank48Frame022 = {
    SPRITE_FRAME(gSpriteBank48Frame022, SPRITE_TILES_BANK48 + 0x04d00),
    { { -18, -31, 35, 57 } },
};
const struct sprite_frame_1box gSpriteBank48Frame023 = {
    SPRITE_FRAME(gSpriteBank48Frame023, SPRITE_TILES_BANK48 + 0x05100),
    { { -18, -32, 35, 58 } },
};
const struct sprite_frame_1box gSpriteBank48Frame024 = {
    SPRITE_FRAME(gSpriteBank48Frame024, SPRITE_TILES_BANK48 + 0x05500),
    { { -18, -31, 35, 58 } },
};
const struct sprite_frame_1box gSpriteBank48Frame025 = {
    SPRITE_FRAME(gSpriteBank48Frame025, SPRITE_TILES_BANK48 + 0x05900),
    { { -19, -30, 36, 57 } },
};
const struct sprite_frame_1box gSpriteBank48Frame026 = {
    SPRITE_FRAME(gSpriteBank48Frame026, SPRITE_TILES_BANK48 + 0x05d00),
    { { -19, -29, 36, 57 } },
};
const struct sprite_frame_1box gSpriteBank48Frame027 = {
    SPRITE_FRAME(gSpriteBank48Frame027, SPRITE_TILES_BANK48 + 0x04300),
    { { -18, -28, 35, 56 } },
};
const struct sprite_frame_1box gSpriteBank48Frame028 = {
    SPRITE_FRAME(gSpriteBank48Frame028, SPRITE_TILES_BANK48 + 0x04700),
    { { -18, -28, 35, 56 } },
};
const struct sprite_frame_1box gSpriteBank48Frame029 = {
    SPRITE_FRAME(gSpriteBank48Frame029, SPRITE_TILES_BANK48 + 0x06100),
    { { -18, -28, 35, 56 } },
};
const struct sprite_frame_1box gSpriteBank48Frame030 = {
    SPRITE_FRAME(gSpriteBank48Frame030, SPRITE_TILES_BANK48 + 0x06400),
    { { -18, -29, 35, 56 } },
};
const struct sprite_frame_1box gSpriteBank48Frame031 = {
    SPRITE_FRAME(gSpriteBank48Frame031, SPRITE_TILES_BANK48 + 0x06800),
    { { -18, -30, 35, 57 } },
};
const struct sprite_frame_1box gSpriteBank48Frame032 = {
    SPRITE_FRAME(gSpriteBank48Frame032, SPRITE_TILES_BANK48 + 0x06c00),
    { { -18, -31, 35, 57 } },
};
const struct sprite_frame_1box gSpriteBank48Frame033 = {
    SPRITE_FRAME(gSpriteBank48Frame033, SPRITE_TILES_BANK48 + 0x07000),
    { { -18, -31, 35, 57 } },
};
const struct sprite_frame_1box gSpriteBank48Frame034 = {
    SPRITE_FRAME(gSpriteBank48Frame034, SPRITE_TILES_BANK48 + 0x07400),
    { { -18, -30, 35, 57 } },
};
const struct sprite_frame_1box gSpriteBank48Frame035 = {
    SPRITE_FRAME(gSpriteBank48Frame035, SPRITE_TILES_BANK48 + 0x07700),
    { { -18, -29, 35, 56 } },
};
const struct sprite_frame_1box gSpriteBank48Frame036 = {
    SPRITE_FRAME(gSpriteBank48Frame036, SPRITE_TILES_BANK48 + 0x07a00),
    { { -18, -27, 35, 55 } },
};
const struct sprite_frame_1box gSpriteBank48Frame037 = {
    SPRITE_FRAME(gSpriteBank48Frame037, SPRITE_TILES_BANK48 + 0x07d00),
    { { -18, -27, 35, 55 } },
};

const struct sprite_piece_pos gSpriteBank48Frame000Pos[2] = { { -13, -23 }, { -15, 9 } };
const struct sprite_piece_pos gSpriteBank48Frame001Pos[2] = { { -12, -24 }, { -15, 8 } };
const struct sprite_piece_pos gSpriteBank48Frame002Pos[1] = { { -15, -25 } };
const struct sprite_piece_pos gSpriteBank48Frame003Pos[2] = { { -12, -25 }, { -15, 7 } };
const struct sprite_piece_pos gSpriteBank48Frame004Pos[2] = { { -13, -25 }, { -15, 7 } };
const struct sprite_piece_pos gSpriteBank48Frame005Pos[1] = { { -15, -25 } };
const struct sprite_piece_pos gSpriteBank48Frame006Pos[2] = { { -14, -24 }, { -15, 8 } };
const struct sprite_piece_pos gSpriteBank48Frame007Pos[1] = { { -15, -24 } };
const struct sprite_piece_pos gSpriteBank48Frame008Pos[2] = { { -14, -23 }, { -15, 9 } };
const struct sprite_piece_pos gSpriteBank48Frame009Pos[1] = { { -15, -24 } };
const struct sprite_piece_pos gSpriteBank48Frame010Pos[2] = { { -12, -23 }, { -15, 9 } };
const struct sprite_piece_pos gSpriteBank48Frame011Pos[2] = { { -9, -24 }, { -15, 8 } };
const struct sprite_piece_pos gSpriteBank48Frame012Pos[2] = { { -7, -25 }, { -15, 7 } };
const struct sprite_piece_pos gSpriteBank48Frame013Pos[1] = { { -15, -27 } };
const struct sprite_piece_pos gSpriteBank48Frame014Pos[1] = { { -15, -27 } };
const struct sprite_piece_pos gSpriteBank48Frame015Pos[1] = { { -15, -26 } };
const struct sprite_piece_pos gSpriteBank48Frame016Pos[1] = { { -15, -26 } };
const struct sprite_piece_pos gSpriteBank48Frame017Pos[1] = { { -15, -25 } };
const struct sprite_piece_pos gSpriteBank48Frame018Pos[1] = { { -15, -24 } };
const struct sprite_piece_pos gSpriteBank48Frame019Pos[1] = { { -15, -24 } };
const struct sprite_piece_pos gSpriteBank48Frame020Pos[2] = { { -14, -23 }, { -14, 9 } };
const struct sprite_piece_pos gSpriteBank48Frame021Pos[2] = { { -14, -24 }, { -14, 8 } };
const struct sprite_piece_pos gSpriteBank48Frame022Pos[1] = { { -14, -26 } };
const struct sprite_piece_pos gSpriteBank48Frame023Pos[1] = { { -14, -27 } };
const struct sprite_piece_pos gSpriteBank48Frame024Pos[1] = { { -14, -28 } };
const struct sprite_piece_pos gSpriteBank48Frame025Pos[1] = { { -14, -27 } };
const struct sprite_piece_pos gSpriteBank48Frame026Pos[1] = { { -15, -26 } };
const struct sprite_piece_pos gSpriteBank48Frame027Pos[1] = { { -15, -25 } };
const struct sprite_piece_pos gSpriteBank48Frame028Pos[2] = { { -14, -24 }, { -14, 8 } };
const struct sprite_piece_pos gSpriteBank48Frame029Pos[2] = { { -11, -24 }, { -14, 8 } };
const struct sprite_piece_pos gSpriteBank48Frame030Pos[1] = { { -14, -25 } };
const struct sprite_piece_pos gSpriteBank48Frame031Pos[1] = { { -14, -26 } };
const struct sprite_piece_pos gSpriteBank48Frame032Pos[1] = { { -14, -26 } };
const struct sprite_piece_pos gSpriteBank48Frame033Pos[1] = { { -14, -25 } };
const struct sprite_piece_pos gSpriteBank48Frame034Pos[2] = { { -13, -24 }, { -14, 8 } };
const struct sprite_piece_pos gSpriteBank48Frame035Pos[2] = { { -14, -22 }, { -14, 10 } };
const struct sprite_piece_pos gSpriteBank48Frame036Pos[2] = { { -13, -22 }, { -14, 10 } };
const struct sprite_piece_pos gSpriteBank48Frame037Pos[2] = { { -13, -22 }, { -14, 10 } };

const u8 gSpriteBank48Frame000Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank48Frame001Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank48Frame002Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank48Frame003Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank48Frame004Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank48Frame005Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank48Frame006Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank48Frame007Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank48Frame008Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank48Frame009Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank48Frame010Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank48Frame011Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank48Frame012Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank48Frame013Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank48Frame014Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank48Frame015Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank48Frame016Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank48Frame017Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank48Frame018Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank48Frame019Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank48Frame020Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank48Frame021Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank48Frame022Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank48Frame023Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank48Frame024Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank48Frame025Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank48Frame026Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank48Frame027Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank48Frame028Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank48Frame029Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank48Frame030Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank48Frame031Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank48Frame032Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank48Frame033Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank48Frame034Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank48Frame035Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank48Frame036Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank48Frame037Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };

/* ---------------------------------------------------------------------- */
/* Bank 49: 5 animations, 2 frames, tiles in gSpriteBank49Tiles (SPRITE_TILES_BANK49). */

extern const u16 gSpriteBank49Anim00Seq[2];
extern const u16 gSpriteBank49Anim01Seq[2];
extern const u16 gSpriteBank49Anim02Seq[2];
extern const u16 gSpriteBank49Anim03Seq[2];
extern const u16 gSpriteBank49Anim04Seq[2];
extern const struct sprite_frame_1box gSpriteBank49Frame000;
extern const struct sprite_frame_1box gSpriteBank49Frame001;
extern const struct sprite_piece_pos gSpriteBank49Frame000Pos[3];
extern const struct sprite_piece_pos gSpriteBank49Frame001Pos[3];
extern const u8 gSpriteBank49Frame000Pieces[3];
extern const u8 gSpriteBank49Frame001Pieces[3];

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
    SPRITE_FRAME(gSpriteBank49Frame000, SPRITE_TILES_BANK49 + 0x00000),
    { { -19, -9, 39, 18 } },
};
const struct sprite_frame_1box gSpriteBank49Frame001 = {
    SPRITE_FRAME(gSpriteBank49Frame001, SPRITE_TILES_BANK49 + 0x001c0),
    { { -19, -7, 39, 16 } },
};

const struct sprite_piece_pos gSpriteBank49Frame000Pos[3] = { { -19, -9 }, { 13, -8 }, { -13, 7 } };
const struct sprite_piece_pos gSpriteBank49Frame001Pos[3] = { { -19, -7 }, { 13, -6 }, { -8, 9 } };

const u8 gSpriteBank49Frame000Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank49Frame001Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5) };

/* ---------------------------------------------------------------------- */
/* Bank 50: 1 animation, 8 frames, tiles in gSpriteBank50Tiles (SPRITE_TILES_BANK50). */

extern const u16 gSpriteBank50Anim00Seq[8];
extern const struct sprite_frame_1box gSpriteBank50Frame000;
extern const struct sprite_frame_1box gSpriteBank50Frame001;
extern const struct sprite_frame_1box gSpriteBank50Frame002;
extern const struct sprite_frame_1box gSpriteBank50Frame003;
extern const struct sprite_frame_1box gSpriteBank50Frame004;
extern const struct sprite_frame_1box gSpriteBank50Frame005;
extern const struct sprite_frame_1box gSpriteBank50Frame006;
extern const struct sprite_frame_1box gSpriteBank50Frame007;
extern const struct sprite_piece_pos gSpriteBank50Frame000Pos[2];
extern const struct sprite_piece_pos gSpriteBank50Frame001Pos[1];
extern const struct sprite_piece_pos gSpriteBank50Frame002Pos[3];
extern const struct sprite_piece_pos gSpriteBank50Frame003Pos[3];
extern const struct sprite_piece_pos gSpriteBank50Frame004Pos[2];
extern const struct sprite_piece_pos gSpriteBank50Frame005Pos[1];
extern const struct sprite_piece_pos gSpriteBank50Frame006Pos[3];
extern const struct sprite_piece_pos gSpriteBank50Frame007Pos[3];
extern const u8 gSpriteBank50Frame000Pieces[2];
extern const u8 gSpriteBank50Frame001Pieces[1];
extern const u8 gSpriteBank50Frame002Pieces[3];
extern const u8 gSpriteBank50Frame003Pieces[3];
extern const u8 gSpriteBank50Frame004Pieces[2];
extern const u8 gSpriteBank50Frame005Pieces[1];
extern const u8 gSpriteBank50Frame006Pieces[3];
extern const u8 gSpriteBank50Frame007Pieces[3];

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
    SPRITE_FRAME(gSpriteBank50Frame000, SPRITE_TILES_BANK50 + 0x00000),
    { { -5, -67, 23, 65 } },
};
const struct sprite_frame_1box gSpriteBank50Frame001 = {
    SPRITE_FRAME(gSpriteBank50Frame001, SPRITE_TILES_BANK50 + 0x00420),
    { { -3, -67, 23, 63 } },
};
const struct sprite_frame_1box gSpriteBank50Frame002 = {
    SPRITE_FRAME(gSpriteBank50Frame002, SPRITE_TILES_BANK50 + 0x007e0),
    { { 0, -60, 42, 53 } },
};
const struct sprite_frame_1box gSpriteBank50Frame003 = {
    SPRITE_FRAME(gSpriteBank50Frame003, SPRITE_TILES_BANK50 + 0x00c80),
    { { 0, -59, 42, 54 } },
};
const struct sprite_frame_1box gSpriteBank50Frame004 = {
    SPRITE_FRAME(gSpriteBank50Frame004, SPRITE_TILES_BANK50 + 0x01120),
    { { -1, -46, 61, 46 } },
};
const struct sprite_frame_1box gSpriteBank50Frame005 = {
    SPRITE_FRAME(gSpriteBank50Frame005, SPRITE_TILES_BANK50 + 0x015e0),
    { { 1, -49, 57, 48 } },
};
const struct sprite_frame_1box gSpriteBank50Frame006 = {
    SPRITE_FRAME(gSpriteBank50Frame006, SPRITE_TILES_BANK50 + 0x01da0),
    { { -2, -25, 66, 28 } },
};
const struct sprite_frame_1box gSpriteBank50Frame007 = {
    SPRITE_FRAME(gSpriteBank50Frame007, SPRITE_TILES_BANK50 + 0x02200),
    { { -1, -24, 64, 32 } },
};

const struct sprite_piece_pos gSpriteBank50Frame000Pos[2] = { { -5, -67 }, { -2, -3 } };
const struct sprite_piece_pos gSpriteBank50Frame001Pos[1] = { { -3, -67 } };
const struct sprite_piece_pos gSpriteBank50Frame002Pos[3] = { { 0, -54 }, { 32, -60 }, { 32, -44 } };
const struct sprite_piece_pos gSpriteBank50Frame003Pos[3] = { { 0, -54 }, { 32, -59 }, { 33, -43 } };
const struct sprite_piece_pos gSpriteBank50Frame004Pos[2] = { { 3, -46 }, { -1, -14 } };
const struct sprite_piece_pos gSpriteBank50Frame005Pos[1] = { { 1, -49 } };
const struct sprite_piece_pos gSpriteBank50Frame006Pos[3] = { { -2, -25 }, { 62, -24 }, { 62, -8 } };
const struct sprite_piece_pos gSpriteBank50Frame007Pos[3] = { { -1, -24 }, { 63, -24 }, { 3, 8 } };

const u8 gSpriteBank50Frame000Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank50Frame001Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank50Frame002Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank50Frame003Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank50Frame004Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank50Frame005Pieces[1] = { SPRITE_PIECE(2, 3) };
const u8 gSpriteBank50Frame006Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank50Frame007Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };

/* ---------------------------------------------------------------------- */
/* Bank 51: 5 animations, 24 frames, tiles in gSpriteBank51Tiles (SPRITE_TILES_BANK51). */

extern const u16 gSpriteBank51Anim00Seq[20];
extern const u16 gSpriteBank51Anim01Seq[1];
extern const u16 gSpriteBank51Anim02Seq[1];
extern const u16 gSpriteBank51Anim03Seq[1];
extern const u16 gSpriteBank51Anim04Seq[1];
extern const struct sprite_frame_1box gSpriteBank51Frame000;
extern const struct sprite_frame_1box gSpriteBank51Frame001;
extern const struct sprite_frame_1box gSpriteBank51Frame002;
extern const struct sprite_frame_1box gSpriteBank51Frame003;
extern const struct sprite_frame_1box gSpriteBank51Frame004;
extern const struct sprite_frame_1box gSpriteBank51Frame005;
extern const struct sprite_frame_1box gSpriteBank51Frame006;
extern const struct sprite_frame_1box gSpriteBank51Frame007;
extern const struct sprite_frame_1box gSpriteBank51Frame008;
extern const struct sprite_frame_1box gSpriteBank51Frame009;
extern const struct sprite_frame_1box gSpriteBank51Frame010;
extern const struct sprite_frame_1box gSpriteBank51Frame011;
extern const struct sprite_frame_1box gSpriteBank51Frame012;
extern const struct sprite_frame_1box gSpriteBank51Frame013;
extern const struct sprite_frame_1box gSpriteBank51Frame014;
extern const struct sprite_frame_1box gSpriteBank51Frame015;
extern const struct sprite_frame_1box gSpriteBank51Frame016;
extern const struct sprite_frame_1box gSpriteBank51Frame017;
extern const struct sprite_frame_1box gSpriteBank51Frame018;
extern const struct sprite_frame_1box gSpriteBank51Frame019;
extern const struct sprite_frame_1box gSpriteBank51Frame020;
extern const struct sprite_frame_1box gSpriteBank51Frame021;
extern const struct sprite_frame_1box gSpriteBank51Frame022;
extern const struct sprite_frame_1box gSpriteBank51Frame023;
extern const struct sprite_piece_pos gSpriteBank51Frame000Pos[1];
extern const struct sprite_piece_pos gSpriteBank51Frame001Pos[1];
extern const struct sprite_piece_pos gSpriteBank51Frame002Pos[2];
extern const struct sprite_piece_pos gSpriteBank51Frame003Pos[2];
extern const struct sprite_piece_pos gSpriteBank51Frame004Pos[2];
extern const struct sprite_piece_pos gSpriteBank51Frame005Pos[2];
extern const struct sprite_piece_pos gSpriteBank51Frame006Pos[2];
extern const struct sprite_piece_pos gSpriteBank51Frame007Pos[2];
extern const struct sprite_piece_pos gSpriteBank51Frame008Pos[2];
extern const struct sprite_piece_pos gSpriteBank51Frame009Pos[1];
extern const struct sprite_piece_pos gSpriteBank51Frame010Pos[1];
extern const struct sprite_piece_pos gSpriteBank51Frame011Pos[1];
extern const struct sprite_piece_pos gSpriteBank51Frame012Pos[2];
extern const struct sprite_piece_pos gSpriteBank51Frame013Pos[2];
extern const struct sprite_piece_pos gSpriteBank51Frame014Pos[2];
extern const struct sprite_piece_pos gSpriteBank51Frame015Pos[2];
extern const struct sprite_piece_pos gSpriteBank51Frame016Pos[2];
extern const struct sprite_piece_pos gSpriteBank51Frame017Pos[2];
extern const struct sprite_piece_pos gSpriteBank51Frame018Pos[2];
extern const struct sprite_piece_pos gSpriteBank51Frame019Pos[2];
extern const struct sprite_piece_pos gSpriteBank51Frame020Pos[2];
extern const struct sprite_piece_pos gSpriteBank51Frame021Pos[2];
extern const struct sprite_piece_pos gSpriteBank51Frame022Pos[2];
extern const struct sprite_piece_pos gSpriteBank51Frame023Pos[1];
extern const u8 gSpriteBank51Frame000Pieces[1];
extern const u8 gSpriteBank51Frame001Pieces[1];
extern const u8 gSpriteBank51Frame002Pieces[2];
extern const u8 gSpriteBank51Frame003Pieces[2];
extern const u8 gSpriteBank51Frame004Pieces[2];
extern const u8 gSpriteBank51Frame005Pieces[2];
extern const u8 gSpriteBank51Frame006Pieces[2];
extern const u8 gSpriteBank51Frame007Pieces[2];
extern const u8 gSpriteBank51Frame008Pieces[2];
extern const u8 gSpriteBank51Frame009Pieces[1];
extern const u8 gSpriteBank51Frame010Pieces[1];
extern const u8 gSpriteBank51Frame011Pieces[1];
extern const u8 gSpriteBank51Frame012Pieces[2];
extern const u8 gSpriteBank51Frame013Pieces[2];
extern const u8 gSpriteBank51Frame014Pieces[2];
extern const u8 gSpriteBank51Frame015Pieces[2];
extern const u8 gSpriteBank51Frame016Pieces[2];
extern const u8 gSpriteBank51Frame017Pieces[2];
extern const u8 gSpriteBank51Frame018Pieces[2];
extern const u8 gSpriteBank51Frame019Pieces[2];
extern const u8 gSpriteBank51Frame020Pieces[2];
extern const u8 gSpriteBank51Frame021Pieces[2];
extern const u8 gSpriteBank51Frame022Pieces[2];
extern const u8 gSpriteBank51Frame023Pieces[1];

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
    SPRITE_FRAME(gSpriteBank51Frame000, SPRITE_TILES_BANK51 + 0x00000),
    { { -3, -4, 6, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame001 = {
    SPRITE_FRAME(gSpriteBank51Frame001, SPRITE_TILES_BANK51 + 0x00040),
    { { -5, -4, 8, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame002 = {
    SPRITE_FRAME(gSpriteBank51Frame002, SPRITE_TILES_BANK51 + 0x00080),
    { { -4, -4, 8, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame003 = {
    SPRITE_FRAME(gSpriteBank51Frame003, SPRITE_TILES_BANK51 + 0x000e0),
    { { -6, -4, 10, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame004 = {
    SPRITE_FRAME(gSpriteBank51Frame004, SPRITE_TILES_BANK51 + 0x00160),
    { { -5, -4, 8, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame005 = {
    SPRITE_FRAME(gSpriteBank51Frame005, SPRITE_TILES_BANK51 + 0x001c0),
    { { -6, -4, 10, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame006 = {
    SPRITE_FRAME(gSpriteBank51Frame006, SPRITE_TILES_BANK51 + 0x00240),
    { { -5, -4, 9, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame007 = {
    SPRITE_FRAME(gSpriteBank51Frame007, SPRITE_TILES_BANK51 + 0x002a0),
    { { -5, -4, 9, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame008 = {
    SPRITE_FRAME(gSpriteBank51Frame008, SPRITE_TILES_BANK51 + 0x00320),
    { { -5, -4, 9, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame009 = {
    SPRITE_FRAME(gSpriteBank51Frame009, SPRITE_TILES_BANK51 + 0x00380),
    { { -7, -4, 15, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame010 = {
    SPRITE_FRAME(gSpriteBank51Frame010, SPRITE_TILES_BANK51 + 0x00400),
    { { -7, -4, 13, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame011 = {
    SPRITE_FRAME(gSpriteBank51Frame011, SPRITE_TILES_BANK51 + 0x00480),
    { { -7, -4, 15, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame012 = {
    SPRITE_FRAME(gSpriteBank51Frame012, SPRITE_TILES_BANK51 + 0x00500),
    { { -7, -4, 16, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame013 = {
    SPRITE_FRAME(gSpriteBank51Frame013, SPRITE_TILES_BANK51 + 0x005a0),
    { { -7, -4, 17, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame014 = {
    SPRITE_FRAME(gSpriteBank51Frame014, SPRITE_TILES_BANK51 + 0x00660),
    { { -7, -4, 16, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame015 = {
    SPRITE_FRAME(gSpriteBank51Frame015, SPRITE_TILES_BANK51 + 0x00700),
    { { -7, -4, 17, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame016 = {
    SPRITE_FRAME(gSpriteBank51Frame016, SPRITE_TILES_BANK51 + 0x007a0),
    { { -7, -4, 16, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame017 = {
    SPRITE_FRAME(gSpriteBank51Frame017, SPRITE_TILES_BANK51 + 0x00840),
    { { -7, -4, 16, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame018 = {
    SPRITE_FRAME(gSpriteBank51Frame018, SPRITE_TILES_BANK51 + 0x008e0),
    { { -7, -4, 16, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame019 = {
    SPRITE_FRAME(gSpriteBank51Frame019, SPRITE_TILES_BANK51 + 0x00980),
    { { -10, -4, 18, 8 } },
};
const struct sprite_frame_1box gSpriteBank51Frame020 = {
    SPRITE_FRAME(gSpriteBank51Frame020, SPRITE_TILES_BANK51 + 0x00a20),
    { { -9, -6, 19, 13 } },
};
const struct sprite_frame_1box gSpriteBank51Frame021 = {
    SPRITE_FRAME(gSpriteBank51Frame021, SPRITE_TILES_BANK51 + 0x00ae0),
    { { -10, -6, 20, 12 } },
};
const struct sprite_frame_1box gSpriteBank51Frame022 = {
    SPRITE_FRAME(gSpriteBank51Frame022, SPRITE_TILES_BANK51 + 0x00b80),
    { { -10, -7, 21, 14 } },
};
const struct sprite_frame_1box gSpriteBank51Frame023 = {
    SPRITE_FRAME(gSpriteBank51Frame023, SPRITE_TILES_BANK51 + 0x00c40),
    { { -13, -6, 26, 13 } },
};

const struct sprite_piece_pos gSpriteBank51Frame000Pos[1] = { { -3, -4 } };
const struct sprite_piece_pos gSpriteBank51Frame001Pos[1] = { { -4, -4 } };
const struct sprite_piece_pos gSpriteBank51Frame002Pos[2] = { { -4, -4 }, { 4, -2 } };
const struct sprite_piece_pos gSpriteBank51Frame003Pos[2] = { { -6, -4 }, { 2, -4 } };
const struct sprite_piece_pos gSpriteBank51Frame004Pos[2] = { { -5, -4 }, { 3, -4 } };
const struct sprite_piece_pos gSpriteBank51Frame005Pos[2] = { { -6, -4 }, { 2, -4 } };
const struct sprite_piece_pos gSpriteBank51Frame006Pos[2] = { { -5, -4 }, { 3, -4 } };
const struct sprite_piece_pos gSpriteBank51Frame007Pos[2] = { { -5, -4 }, { 3, -4 } };
const struct sprite_piece_pos gSpriteBank51Frame008Pos[2] = { { -5, -4 }, { 3, -4 } };
const struct sprite_piece_pos gSpriteBank51Frame009Pos[1] = { { -7, -4 } };
const struct sprite_piece_pos gSpriteBank51Frame010Pos[1] = { { -7, -4 } };
const struct sprite_piece_pos gSpriteBank51Frame011Pos[1] = { { -7, -4 } };
const struct sprite_piece_pos gSpriteBank51Frame012Pos[2] = { { -7, -4 }, { 9, -2 } };
const struct sprite_piece_pos gSpriteBank51Frame013Pos[2] = { { -7, -4 }, { 9, -4 } };
const struct sprite_piece_pos gSpriteBank51Frame014Pos[2] = { { -7, -4 }, { 9, -4 } };
const struct sprite_piece_pos gSpriteBank51Frame015Pos[2] = { { -7, -4 }, { 9, 0 } };
const struct sprite_piece_pos gSpriteBank51Frame016Pos[2] = { { -7, -4 }, { 9, -4 } };
const struct sprite_piece_pos gSpriteBank51Frame017Pos[2] = { { -7, -4 }, { 9, -3 } };
const struct sprite_piece_pos gSpriteBank51Frame018Pos[2] = { { -7, -4 }, { 9, -3 } };
const struct sprite_piece_pos gSpriteBank51Frame019Pos[2] = { { -9, -4 }, { 7, -3 } };
const struct sprite_piece_pos gSpriteBank51Frame020Pos[2] = { { -9, -6 }, { 7, -6 } };
const struct sprite_piece_pos gSpriteBank51Frame021Pos[2] = { { -10, -6 }, { 6, -2 } };
const struct sprite_piece_pos gSpriteBank51Frame022Pos[2] = { { -10, -7 }, { 6, -7 } };
const struct sprite_piece_pos gSpriteBank51Frame023Pos[1] = { { -13, -6 } };

const u8 gSpriteBank51Frame000Pieces[1] = { SPRITE_PIECE(2, 8) };
const u8 gSpriteBank51Frame001Pieces[1] = { SPRITE_PIECE(2, 8) };
const u8 gSpriteBank51Frame002Pieces[2] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank51Frame003Pieces[2] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank51Frame004Pieces[2] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank51Frame005Pieces[2] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank51Frame006Pieces[2] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank51Frame007Pieces[2] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank51Frame008Pieces[2] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank51Frame009Pieces[1] = { SPRITE_PIECE(2, 1) };
const u8 gSpriteBank51Frame010Pieces[1] = { SPRITE_PIECE(2, 1) };
const u8 gSpriteBank51Frame011Pieces[1] = { SPRITE_PIECE(2, 1) };
const u8 gSpriteBank51Frame012Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank51Frame013Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank51Frame014Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank51Frame015Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank51Frame016Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank51Frame017Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank51Frame018Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank51Frame019Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank51Frame020Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank51Frame021Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank51Frame022Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank51Frame023Pieces[1] = { SPRITE_PIECE(2, 6) };

/* ---------------------------------------------------------------------- */
/* Bank 52: 2 animations, 4 frames, tiles in gSpriteBank52Tiles (SPRITE_TILES_BANK52). */

extern const u16 gSpriteBank52Anim00Seq[2];
extern const u16 gSpriteBank52Anim01Seq[2];
extern const struct sprite_frame_1box gSpriteBank52Frame000;
extern const struct sprite_frame_1box gSpriteBank52Frame001;
extern const struct sprite_frame_1box gSpriteBank52Frame002;
extern const struct sprite_frame_1box gSpriteBank52Frame003;
extern const struct sprite_piece_pos gSpriteBank52Frame000Pos[1];
extern const struct sprite_piece_pos gSpriteBank52Frame001Pos[1];
extern const struct sprite_piece_pos gSpriteBank52Frame002Pos[1];
extern const struct sprite_piece_pos gSpriteBank52Frame003Pos[1];
extern const u8 gSpriteBank52Frame000Pieces[1];
extern const u8 gSpriteBank52Frame001Pieces[1];
extern const u8 gSpriteBank52Frame002Pieces[1];
extern const u8 gSpriteBank52Frame003Pieces[1];

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
    SPRITE_FRAME(gSpriteBank52Frame000, SPRITE_TILES_BANK52 + 0x00000),
    { { -7, -7, 15, 15 } },
};
const struct sprite_frame_1box gSpriteBank52Frame001 = {
    SPRITE_FRAME(gSpriteBank52Frame001, SPRITE_TILES_BANK52 + 0x00080),
    { { -7, -7, 15, 15 } },
};
const struct sprite_frame_1box gSpriteBank52Frame002 = {
    SPRITE_FRAME(gSpriteBank52Frame002, SPRITE_TILES_BANK52 + 0x00100),
    { { -7, -7, 15, 15 } },
};
const struct sprite_frame_1box gSpriteBank52Frame003 = {
    SPRITE_FRAME(gSpriteBank52Frame003, SPRITE_TILES_BANK52 + 0x00180),
    { { -7, -7, 15, 15 } },
};

const struct sprite_piece_pos gSpriteBank52Frame000Pos[1] = { { -7, -7 } };
const struct sprite_piece_pos gSpriteBank52Frame001Pos[1] = { { -7, -7 } };
const struct sprite_piece_pos gSpriteBank52Frame002Pos[1] = { { -7, -7 } };
const struct sprite_piece_pos gSpriteBank52Frame003Pos[1] = { { -7, -7 } };

const u8 gSpriteBank52Frame000Pieces[1] = { SPRITE_PIECE(2, 1) };
const u8 gSpriteBank52Frame001Pieces[1] = { SPRITE_PIECE(2, 1) };
const u8 gSpriteBank52Frame002Pieces[1] = { SPRITE_PIECE(2, 1) };
const u8 gSpriteBank52Frame003Pieces[1] = { SPRITE_PIECE(2, 1) };

/* ---------------------------------------------------------------------- */
/* Bank 53: 19 animations, 67 frames, tiles in gSpriteBank53Tiles (SPRITE_TILES_BANK53). */

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
extern const struct sprite_frame_1box gSpriteBank53Frame000;
extern const struct sprite_frame_1box gSpriteBank53Frame001;
extern const struct sprite_frame_1box gSpriteBank53Frame002;
extern const struct sprite_frame_1box gSpriteBank53Frame003;
extern const struct sprite_frame_1box gSpriteBank53Frame004;
extern const struct sprite_frame_1box gSpriteBank53Frame005;
extern const struct sprite_frame_1box gSpriteBank53Frame006;
extern const struct sprite_frame_1box gSpriteBank53Frame007;
extern const struct sprite_frame_1box gSpriteBank53Frame008;
extern const struct sprite_frame_1box gSpriteBank53Frame009;
extern const struct sprite_frame_1box gSpriteBank53Frame010;
extern const struct sprite_frame_1box gSpriteBank53Frame011;
extern const struct sprite_frame_1box gSpriteBank53Frame012;
extern const struct sprite_frame_1box gSpriteBank53Frame013;
extern const struct sprite_frame_1box gSpriteBank53Frame014;
extern const struct sprite_frame_1box gSpriteBank53Frame015;
extern const struct sprite_frame_1box gSpriteBank53Frame016;
extern const struct sprite_frame_1box gSpriteBank53Frame017;
extern const struct sprite_frame_1box gSpriteBank53Frame018;
extern const struct sprite_frame_1box gSpriteBank53Frame019;
extern const struct sprite_frame_1box gSpriteBank53Frame020;
extern const struct sprite_frame_1box gSpriteBank53Frame021;
extern const struct sprite_frame_1box gSpriteBank53Frame022;
extern const struct sprite_frame_1box gSpriteBank53Frame023;
extern const struct sprite_frame_1box gSpriteBank53Frame024;
extern const struct sprite_frame_1box gSpriteBank53Frame025;
extern const struct sprite_frame_1box gSpriteBank53Frame026;
extern const struct sprite_frame_1box gSpriteBank53Frame027;
extern const struct sprite_frame_1box gSpriteBank53Frame028;
extern const struct sprite_frame_1box gSpriteBank53Frame029;
extern const struct sprite_frame_1box gSpriteBank53Frame030;
extern const struct sprite_frame_1box gSpriteBank53Frame031;
extern const struct sprite_frame_1box gSpriteBank53Frame032;
extern const struct sprite_frame_1box gSpriteBank53Frame033;
extern const struct sprite_frame_1box gSpriteBank53Frame034;
extern const struct sprite_frame_1box gSpriteBank53Frame035;
extern const struct sprite_frame_1box gSpriteBank53Frame036;
extern const struct sprite_frame_1box gSpriteBank53Frame037;
extern const struct sprite_frame_1box gSpriteBank53Frame038;
extern const struct sprite_frame_1box gSpriteBank53Frame039;
extern const struct sprite_frame_1box gSpriteBank53Frame040;
extern const struct sprite_frame_1box gSpriteBank53Frame041;
extern const struct sprite_frame_1box gSpriteBank53Frame042;
extern const struct sprite_frame_1box gSpriteBank53Frame043;
extern const struct sprite_frame_1box gSpriteBank53Frame044;
extern const struct sprite_frame_1box gSpriteBank53Frame045;
extern const struct sprite_frame_1box gSpriteBank53Frame046;
extern const struct sprite_frame_1box gSpriteBank53Frame047;
extern const struct sprite_frame_1box gSpriteBank53Frame048;
extern const struct sprite_frame_1box gSpriteBank53Frame049;
extern const struct sprite_frame_1box gSpriteBank53Frame050;
extern const struct sprite_frame_1box gSpriteBank53Frame051;
extern const struct sprite_frame_1box gSpriteBank53Frame052;
extern const struct sprite_frame_1box gSpriteBank53Frame053;
extern const struct sprite_frame_2box gSpriteBank53Frame054;
extern const struct sprite_frame_2box gSpriteBank53Frame055;
extern const struct sprite_frame_2box gSpriteBank53Frame056;
extern const struct sprite_frame_2box gSpriteBank53Frame057;
extern const struct sprite_frame_1box gSpriteBank53Frame058;
extern const struct sprite_frame_1box gSpriteBank53Frame059;
extern const struct sprite_frame_1box gSpriteBank53Frame060;
extern const struct sprite_frame_2box gSpriteBank53Frame061;
extern const struct sprite_frame_2box gSpriteBank53Frame062;
extern const struct sprite_frame_2box gSpriteBank53Frame063;
extern const struct sprite_frame_2box gSpriteBank53Frame064;
extern const struct sprite_frame_1box gSpriteBank53Frame065;
extern const struct sprite_frame_1box gSpriteBank53Frame066;
extern const struct sprite_piece_pos gSpriteBank53Frame000Pos[3];
extern const struct sprite_piece_pos gSpriteBank53Frame001Pos[2];
extern const struct sprite_piece_pos gSpriteBank53Frame002Pos[2];
extern const struct sprite_piece_pos gSpriteBank53Frame003Pos[2];
extern const struct sprite_piece_pos gSpriteBank53Frame004Pos[2];
extern const struct sprite_piece_pos gSpriteBank53Frame005Pos[3];
extern const struct sprite_piece_pos gSpriteBank53Frame006Pos[3];
extern const struct sprite_piece_pos gSpriteBank53Frame007Pos[3];
extern const struct sprite_piece_pos gSpriteBank53Frame008Pos[3];
extern const struct sprite_piece_pos gSpriteBank53Frame009Pos[3];
extern const struct sprite_piece_pos gSpriteBank53Frame010Pos[1];
extern const struct sprite_piece_pos gSpriteBank53Frame011Pos[1];
extern const struct sprite_piece_pos gSpriteBank53Frame012Pos[2];
extern const struct sprite_piece_pos gSpriteBank53Frame013Pos[2];
extern const struct sprite_piece_pos gSpriteBank53Frame014Pos[5];
extern const struct sprite_piece_pos gSpriteBank53Frame015Pos[5];
extern const struct sprite_piece_pos gSpriteBank53Frame016Pos[2];
extern const struct sprite_piece_pos gSpriteBank53Frame017Pos[3];
extern const struct sprite_piece_pos gSpriteBank53Frame018Pos[3];
extern const struct sprite_piece_pos gSpriteBank53Frame019Pos[2];
extern const struct sprite_piece_pos gSpriteBank53Frame020Pos[5];
extern const struct sprite_piece_pos gSpriteBank53Frame021Pos[4];
extern const struct sprite_piece_pos gSpriteBank53Frame022Pos[4];
extern const struct sprite_piece_pos gSpriteBank53Frame023Pos[5];
extern const struct sprite_piece_pos gSpriteBank53Frame024Pos[5];
extern const struct sprite_piece_pos gSpriteBank53Frame025Pos[5];
extern const struct sprite_piece_pos gSpriteBank53Frame026Pos[3];
extern const struct sprite_piece_pos gSpriteBank53Frame027Pos[3];
extern const struct sprite_piece_pos gSpriteBank53Frame028Pos[3];
extern const struct sprite_piece_pos gSpriteBank53Frame029Pos[2];
extern const struct sprite_piece_pos gSpriteBank53Frame030Pos[4];
extern const struct sprite_piece_pos gSpriteBank53Frame031Pos[3];
extern const struct sprite_piece_pos gSpriteBank53Frame032Pos[1];
extern const struct sprite_piece_pos gSpriteBank53Frame033Pos[1];
extern const struct sprite_piece_pos gSpriteBank53Frame034Pos[2];
extern const struct sprite_piece_pos gSpriteBank53Frame035Pos[4];
extern const struct sprite_piece_pos gSpriteBank53Frame036Pos[3];
extern const struct sprite_piece_pos gSpriteBank53Frame037Pos[1];
extern const struct sprite_piece_pos gSpriteBank53Frame038Pos[1];
extern const struct sprite_piece_pos gSpriteBank53Frame039Pos[2];
extern const struct sprite_piece_pos gSpriteBank53Frame040Pos[4];
extern const struct sprite_piece_pos gSpriteBank53Frame041Pos[3];
extern const struct sprite_piece_pos gSpriteBank53Frame042Pos[1];
extern const struct sprite_piece_pos gSpriteBank53Frame043Pos[1];
extern const struct sprite_piece_pos gSpriteBank53Frame044Pos[2];
extern const struct sprite_piece_pos gSpriteBank53Frame045Pos[4];
extern const struct sprite_piece_pos gSpriteBank53Frame046Pos[3];
extern const struct sprite_piece_pos gSpriteBank53Frame047Pos[1];
extern const struct sprite_piece_pos gSpriteBank53Frame048Pos[1];
extern const struct sprite_piece_pos gSpriteBank53Frame049Pos[2];
extern const struct sprite_piece_pos gSpriteBank53Frame050Pos[4];
extern const struct sprite_piece_pos gSpriteBank53Frame051Pos[3];
extern const struct sprite_piece_pos gSpriteBank53Frame052Pos[1];
extern const struct sprite_piece_pos gSpriteBank53Frame053Pos[1];
extern const struct sprite_piece_pos gSpriteBank53Frame054Pos[1];
extern const struct sprite_piece_pos gSpriteBank53Frame055Pos[1];
extern const struct sprite_piece_pos gSpriteBank53Frame056Pos[1];
extern const struct sprite_piece_pos gSpriteBank53Frame057Pos[1];
extern const struct sprite_piece_pos gSpriteBank53Frame058Pos[1];
extern const struct sprite_piece_pos gSpriteBank53Frame059Pos[1];
extern const struct sprite_piece_pos gSpriteBank53Frame060Pos[1];
extern const struct sprite_piece_pos gSpriteBank53Frame061Pos[1];
extern const struct sprite_piece_pos gSpriteBank53Frame062Pos[1];
extern const struct sprite_piece_pos gSpriteBank53Frame063Pos[1];
extern const struct sprite_piece_pos gSpriteBank53Frame064Pos[1];
extern const struct sprite_piece_pos gSpriteBank53Frame065Pos[3];
extern const struct sprite_piece_pos gSpriteBank53Frame066Pos[1];
extern const u8 gSpriteBank53Frame000Pieces[3];
extern const u8 gSpriteBank53Frame001Pieces[2];
extern const u8 gSpriteBank53Frame002Pieces[2];
extern const u8 gSpriteBank53Frame003Pieces[2];
extern const u8 gSpriteBank53Frame004Pieces[2];
extern const u8 gSpriteBank53Frame005Pieces[3];
extern const u8 gSpriteBank53Frame006Pieces[3];
extern const u8 gSpriteBank53Frame007Pieces[3];
extern const u8 gSpriteBank53Frame008Pieces[3];
extern const u8 gSpriteBank53Frame009Pieces[3];
extern const u8 gSpriteBank53Frame010Pieces[1];
extern const u8 gSpriteBank53Frame011Pieces[1];
extern const u8 gSpriteBank53Frame012Pieces[2];
extern const u8 gSpriteBank53Frame013Pieces[2];
extern const u8 gSpriteBank53Frame014Pieces[5];
extern const u8 gSpriteBank53Frame015Pieces[5];
extern const u8 gSpriteBank53Frame016Pieces[2];
extern const u8 gSpriteBank53Frame017Pieces[3];
extern const u8 gSpriteBank53Frame018Pieces[3];
extern const u8 gSpriteBank53Frame019Pieces[2];
extern const u8 gSpriteBank53Frame020Pieces[5];
extern const u8 gSpriteBank53Frame021Pieces[4];
extern const u8 gSpriteBank53Frame022Pieces[4];
extern const u8 gSpriteBank53Frame023Pieces[5];
extern const u8 gSpriteBank53Frame024Pieces[5];
extern const u8 gSpriteBank53Frame025Pieces[5];
extern const u8 gSpriteBank53Frame026Pieces[3];
extern const u8 gSpriteBank53Frame027Pieces[3];
extern const u8 gSpriteBank53Frame028Pieces[3];
extern const u8 gSpriteBank53Frame029Pieces[2];
extern const u8 gSpriteBank53Frame030Pieces[4];
extern const u8 gSpriteBank53Frame031Pieces[3];
extern const u8 gSpriteBank53Frame032Pieces[1];
extern const u8 gSpriteBank53Frame033Pieces[1];
extern const u8 gSpriteBank53Frame034Pieces[2];
extern const u8 gSpriteBank53Frame035Pieces[4];
extern const u8 gSpriteBank53Frame036Pieces[3];
extern const u8 gSpriteBank53Frame037Pieces[1];
extern const u8 gSpriteBank53Frame038Pieces[1];
extern const u8 gSpriteBank53Frame039Pieces[2];
extern const u8 gSpriteBank53Frame040Pieces[4];
extern const u8 gSpriteBank53Frame041Pieces[3];
extern const u8 gSpriteBank53Frame042Pieces[1];
extern const u8 gSpriteBank53Frame043Pieces[1];
extern const u8 gSpriteBank53Frame044Pieces[2];
extern const u8 gSpriteBank53Frame045Pieces[4];
extern const u8 gSpriteBank53Frame046Pieces[3];
extern const u8 gSpriteBank53Frame047Pieces[1];
extern const u8 gSpriteBank53Frame048Pieces[1];
extern const u8 gSpriteBank53Frame049Pieces[2];
extern const u8 gSpriteBank53Frame050Pieces[4];
extern const u8 gSpriteBank53Frame051Pieces[3];
extern const u8 gSpriteBank53Frame052Pieces[1];
extern const u8 gSpriteBank53Frame053Pieces[1];
extern const u8 gSpriteBank53Frame054Pieces[1];
extern const u8 gSpriteBank53Frame055Pieces[1];
extern const u8 gSpriteBank53Frame056Pieces[1];
extern const u8 gSpriteBank53Frame057Pieces[1];
extern const u8 gSpriteBank53Frame058Pieces[1];
extern const u8 gSpriteBank53Frame059Pieces[1];
extern const u8 gSpriteBank53Frame060Pieces[1];
extern const u8 gSpriteBank53Frame061Pieces[1];
extern const u8 gSpriteBank53Frame062Pieces[1];
extern const u8 gSpriteBank53Frame063Pieces[1];
extern const u8 gSpriteBank53Frame064Pieces[1];
extern const u8 gSpriteBank53Frame065Pieces[3];
extern const u8 gSpriteBank53Frame066Pieces[1];

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
    SPRITE_FRAME(gSpriteBank53Frame000, SPRITE_TILES_BANK53 + 0x00000),
    { { -7, -43, 15, 87 } },
};
const struct sprite_frame_1box gSpriteBank53Frame001 = {
    SPRITE_FRAME(gSpriteBank53Frame001, SPRITE_TILES_BANK53 + 0x004c0),
    { { -31, -16, 52, 32 } },
};
const struct sprite_frame_1box gSpriteBank53Frame002 = {
    SPRITE_FRAME(gSpriteBank53Frame002, SPRITE_TILES_BANK53 + 0x00940),
    { { -31, -16, 53, 32 } },
};
const struct sprite_frame_1box gSpriteBank53Frame003 = {
    SPRITE_FRAME(gSpriteBank53Frame003, SPRITE_TILES_BANK53 + 0x00dc0),
    { { -31, -16, 55, 32 } },
};
const struct sprite_frame_1box gSpriteBank53Frame004 = {
    SPRITE_FRAME(gSpriteBank53Frame004, SPRITE_TILES_BANK53 + 0x01240),
    { { -31, -16, 59, 32 } },
};
const struct sprite_frame_1box gSpriteBank53Frame005 = {
    SPRITE_FRAME(gSpriteBank53Frame005, SPRITE_TILES_BANK53 + 0x016c0),
    { { -31, -16, 61, 32 } },
};
const struct sprite_frame_1box gSpriteBank53Frame006 = {
    SPRITE_FRAME(gSpriteBank53Frame006, SPRITE_TILES_BANK53 + 0x01b80),
    { { -31, -16, 62, 32 } },
};
const struct sprite_frame_1box gSpriteBank53Frame007 = {
    SPRITE_FRAME(gSpriteBank53Frame007, SPRITE_TILES_BANK53 + 0x02080),
    { { -7, -43, 15, 87 } },
};
const struct sprite_frame_1box gSpriteBank53Frame008 = {
    SPRITE_FRAME(gSpriteBank53Frame008, SPRITE_TILES_BANK53 + 0x02540),
    { { -72, -35, 68, 49 } },
};
const struct sprite_frame_1box gSpriteBank53Frame009 = {
    SPRITE_FRAME(gSpriteBank53Frame009, SPRITE_TILES_BANK53 + 0x02da0),
    { { -67, -35, 66, 49 } },
};
const struct sprite_frame_1box gSpriteBank53Frame010 = {
    SPRITE_FRAME(gSpriteBank53Frame010, SPRITE_TILES_BANK53 + 0x03600),
    { { -54, -35, 60, 49 } },
};
const struct sprite_frame_1box gSpriteBank53Frame011 = {
    SPRITE_FRAME(gSpriteBank53Frame011, SPRITE_TILES_BANK53 + 0x03e00),
    { { -47, -35, 63, 49 } },
};
const struct sprite_frame_1box gSpriteBank53Frame012 = {
    SPRITE_FRAME(gSpriteBank53Frame012, SPRITE_TILES_BANK53 + 0x045c0),
    { { -39, -35, 64, 49 } },
};
const struct sprite_frame_1box gSpriteBank53Frame013 = {
    SPRITE_FRAME(gSpriteBank53Frame013, SPRITE_TILES_BANK53 + 0x04de0),
    { { -33, -36, 65, 50 } },
};
const struct sprite_frame_1box gSpriteBank53Frame014 = {
    SPRITE_FRAME(gSpriteBank53Frame014, SPRITE_TILES_BANK53 + 0x05600),
    { { -71, -50, 70, 64 } },
};
const struct sprite_frame_1box gSpriteBank53Frame015 = {
    SPRITE_FRAME(gSpriteBank53Frame015, SPRITE_TILES_BANK53 + 0x05fa0),
    { { -65, -50, 67, 64 } },
};
const struct sprite_frame_1box gSpriteBank53Frame016 = {
    SPRITE_FRAME(gSpriteBank53Frame016, SPRITE_TILES_BANK53 + 0x06900),
    { { -55, -49, 64, 63 } },
};
const struct sprite_frame_1box gSpriteBank53Frame017 = {
    SPRITE_FRAME(gSpriteBank53Frame017, SPRITE_TILES_BANK53 + 0x07140),
    { { -47, -48, 66, 62 } },
};
const struct sprite_frame_1box gSpriteBank53Frame018 = {
    SPRITE_FRAME(gSpriteBank53Frame018, SPRITE_TILES_BANK53 + 0x079a0),
    { { -40, -47, 67, 61 } },
};
const struct sprite_frame_1box gSpriteBank53Frame019 = {
    SPRITE_FRAME(gSpriteBank53Frame019, SPRITE_TILES_BANK53 + 0x08200),
    { { -37, -46, 68, 60 } },
};
const struct sprite_frame_1box gSpriteBank53Frame020 = {
    SPRITE_FRAME(gSpriteBank53Frame020, SPRITE_TILES_BANK53 + 0x08a40),
    { { -68, -61, 70, 75 } },
};
const struct sprite_frame_1box gSpriteBank53Frame021 = {
    SPRITE_FRAME(gSpriteBank53Frame021, SPRITE_TILES_BANK53 + 0x094a0),
    { { -64, -61, 69, 75 } },
};
const struct sprite_frame_1box gSpriteBank53Frame022 = {
    SPRITE_FRAME(gSpriteBank53Frame022, SPRITE_TILES_BANK53 + 0x09ee0),
    { { -55, -60, 65, 74 } },
};
const struct sprite_frame_1box gSpriteBank53Frame023 = {
    SPRITE_FRAME(gSpriteBank53Frame023, SPRITE_TILES_BANK53 + 0x0a920),
    { { -49, -60, 66, 74 } },
};
const struct sprite_frame_1box gSpriteBank53Frame024 = {
    SPRITE_FRAME(gSpriteBank53Frame024, SPRITE_TILES_BANK53 + 0x0b380),
    { { -42, -59, 68, 73 } },
};
const struct sprite_frame_1box gSpriteBank53Frame025 = {
    SPRITE_FRAME(gSpriteBank53Frame025, SPRITE_TILES_BANK53 + 0x0be00),
    { { -35, -59, 69, 73 } },
};
const struct sprite_frame_1box gSpriteBank53Frame026 = {
    SPRITE_FRAME(gSpriteBank53Frame026, SPRITE_TILES_BANK53 + 0x0c880),
    { { -7, -43, 15, 87 } },
};
const struct sprite_frame_1box gSpriteBank53Frame027 = {
    SPRITE_FRAME(gSpriteBank53Frame027, SPRITE_TILES_BANK53 + 0x0cd40),
    { { -7, -43, 15, 87 } },
};
const struct sprite_frame_1box gSpriteBank53Frame028 = {
    SPRITE_FRAME(gSpriteBank53Frame028, SPRITE_TILES_BANK53 + 0x0d200),
    { { -7, -43, 15, 87 } },
};
const struct sprite_frame_1box gSpriteBank53Frame029 = {
    SPRITE_FRAME(gSpriteBank53Frame029, SPRITE_TILES_BANK53 + 0x0d6c0),
    { { -20, -9, 41, 18 } },
};
const struct sprite_frame_1box gSpriteBank53Frame030 = {
    SPRITE_FRAME(gSpriteBank53Frame030, SPRITE_TILES_BANK53 + 0x0d800),
    { { -16, -7, 33, 14 } },
};
const struct sprite_frame_1box gSpriteBank53Frame031 = {
    SPRITE_FRAME(gSpriteBank53Frame031, SPRITE_TILES_BANK53 + 0x0d900),
    { { -12, -5, 25, 10 } },
};
const struct sprite_frame_1box gSpriteBank53Frame032 = {
    SPRITE_FRAME(gSpriteBank53Frame032, SPRITE_TILES_BANK53 + 0x0d9e0),
    { { -5, -2, 10, 4 } },
};
const struct sprite_frame_1box gSpriteBank53Frame033 = {
    SPRITE_FRAME(gSpriteBank53Frame033, SPRITE_TILES_BANK53 + 0x0da60),
    { { -1, -1, 3, 1 } },
};
const struct sprite_frame_1box gSpriteBank53Frame034 = {
    SPRITE_FRAME(gSpriteBank53Frame034, SPRITE_TILES_BANK53 + 0x0d6c0),
    { { -18, -11, 37, 23 } },
};
const struct sprite_frame_1box gSpriteBank53Frame035 = {
    SPRITE_FRAME(gSpriteBank53Frame035, SPRITE_TILES_BANK53 + 0x0d800),
    { { -16, -10, 33, 21 } },
};
const struct sprite_frame_1box gSpriteBank53Frame036 = {
    SPRITE_FRAME(gSpriteBank53Frame036, SPRITE_TILES_BANK53 + 0x0d900),
    { { -12, -7, 25, 15 } },
};
const struct sprite_frame_1box gSpriteBank53Frame037 = {
    SPRITE_FRAME(gSpriteBank53Frame037, SPRITE_TILES_BANK53 + 0x0d9e0),
    { { -7, -5, 15, 11 } },
};
const struct sprite_frame_1box gSpriteBank53Frame038 = {
    SPRITE_FRAME(gSpriteBank53Frame038, SPRITE_TILES_BANK53 + 0x0da60),
    { { -2, -1, 5, 3 } },
};
const struct sprite_frame_1box gSpriteBank53Frame039 = {
    SPRITE_FRAME(gSpriteBank53Frame039, SPRITE_TILES_BANK53 + 0x0d6c0),
    { { -23, -8, 46, 16 } },
};
const struct sprite_frame_1box gSpriteBank53Frame040 = {
    SPRITE_FRAME(gSpriteBank53Frame040, SPRITE_TILES_BANK53 + 0x0d800),
    { { -18, -6, 36, 13 } },
};
const struct sprite_frame_1box gSpriteBank53Frame041 = {
    SPRITE_FRAME(gSpriteBank53Frame041, SPRITE_TILES_BANK53 + 0x0d900),
    { { -13, -5, 28, 11 } },
};
const struct sprite_frame_1box gSpriteBank53Frame042 = {
    SPRITE_FRAME(gSpriteBank53Frame042, SPRITE_TILES_BANK53 + 0x0d9e0),
    { { -8, -3, 17, 7 } },
};
const struct sprite_frame_1box gSpriteBank53Frame043 = {
    SPRITE_FRAME(gSpriteBank53Frame043, SPRITE_TILES_BANK53 + 0x0da60),
    { { -2, 0, 6, 1 } },
};
const struct sprite_frame_1box gSpriteBank53Frame044 = {
    SPRITE_FRAME(gSpriteBank53Frame044, SPRITE_TILES_BANK53 + 0x0d6c0),
    { { -17, -11, 35, 22 } },
};
const struct sprite_frame_1box gSpriteBank53Frame045 = {
    SPRITE_FRAME(gSpriteBank53Frame045, SPRITE_TILES_BANK53 + 0x0d800),
    { { -14, -9, 29, 18 } },
};
const struct sprite_frame_1box gSpriteBank53Frame046 = {
    SPRITE_FRAME(gSpriteBank53Frame046, SPRITE_TILES_BANK53 + 0x0d900),
    { { -11, -7, 23, 14 } },
};
const struct sprite_frame_1box gSpriteBank53Frame047 = {
    SPRITE_FRAME(gSpriteBank53Frame047, SPRITE_TILES_BANK53 + 0x0d9e0),
    { { -7, -5, 15, 10 } },
};
const struct sprite_frame_1box gSpriteBank53Frame048 = {
    SPRITE_FRAME(gSpriteBank53Frame048, SPRITE_TILES_BANK53 + 0x0da60),
    { { -1, -1, 3, 2 } },
};
const struct sprite_frame_1box gSpriteBank53Frame049 = {
    SPRITE_FRAME(gSpriteBank53Frame049, SPRITE_TILES_BANK53 + 0x0d6c0),
    { { -13, -12, 27, 25 } },
};
const struct sprite_frame_1box gSpriteBank53Frame050 = {
    SPRITE_FRAME(gSpriteBank53Frame050, SPRITE_TILES_BANK53 + 0x0d800),
    { { -11, -10, 23, 21 } },
};
const struct sprite_frame_1box gSpriteBank53Frame051 = {
    SPRITE_FRAME(gSpriteBank53Frame051, SPRITE_TILES_BANK53 + 0x0d900),
    { { -9, -8, 19, 17 } },
};
const struct sprite_frame_1box gSpriteBank53Frame052 = {
    SPRITE_FRAME(gSpriteBank53Frame052, SPRITE_TILES_BANK53 + 0x0d9e0),
    { { -4, -4, 10, 9 } },
};
const struct sprite_frame_1box gSpriteBank53Frame053 = {
    SPRITE_FRAME(gSpriteBank53Frame053, SPRITE_TILES_BANK53 + 0x0da60),
    { { -1, -1, 3, 3 } },
};
const struct sprite_frame_2box gSpriteBank53Frame054 = {
    SPRITE_FRAME(gSpriteBank53Frame054, SPRITE_TILES_BANK53 + 0x0da80),
    { { -6, -6, 12, 12 }, { -4, -4, 9, 9 } },
};
const struct sprite_frame_2box gSpriteBank53Frame055 = {
    SPRITE_FRAME(gSpriteBank53Frame055, SPRITE_TILES_BANK53 + 0x0db00),
    { { -7, -7, 14, 14 }, { -4, -4, 9, 9 } },
};
const struct sprite_frame_2box gSpriteBank53Frame056 = {
    SPRITE_FRAME(gSpriteBank53Frame056, SPRITE_TILES_BANK53 + 0x0db80),
    { { -4, -4, 8, 8 }, { -4, -4, 9, 9 } },
};
const struct sprite_frame_2box gSpriteBank53Frame057 = {
    SPRITE_FRAME(gSpriteBank53Frame057, SPRITE_TILES_BANK53 + 0x0dc00),
    { { -2, -2, 4, 4 }, { -4, -4, 9, 9 } },
};
const struct sprite_frame_1box gSpriteBank53Frame058 = {
    SPRITE_FRAME(gSpriteBank53Frame058, SPRITE_TILES_BANK53 + 0x0dc20),
    { { -24, -24, 49, 49 } },
};
const struct sprite_frame_1box gSpriteBank53Frame059 = {
    SPRITE_FRAME(gSpriteBank53Frame059, SPRITE_TILES_BANK53 + 0x0e3e0),
    { { -24, -24, 49, 49 } },
};
const struct sprite_frame_1box gSpriteBank53Frame060 = {
    SPRITE_FRAME(gSpriteBank53Frame060, SPRITE_TILES_BANK53 + 0x0eba0),
    { { -24, -24, 49, 49 } },
};
const struct sprite_frame_2box gSpriteBank53Frame061 = {
    SPRITE_FRAME(gSpriteBank53Frame061, SPRITE_TILES_BANK53 + 0x0f3a0),
    { { -14, -15, 28, 30 }, { -7, -7, 15, 16 } },
};
const struct sprite_frame_2box gSpriteBank53Frame062 = {
    SPRITE_FRAME(gSpriteBank53Frame062, SPRITE_TILES_BANK53 + 0x0f5a0),
    { { -12, -13, 24, 26 }, { -7, -7, 15, 16 } },
};
const struct sprite_frame_2box gSpriteBank53Frame063 = {
    SPRITE_FRAME(gSpriteBank53Frame063, SPRITE_TILES_BANK53 + 0x0f7a0),
    { { -11, -10, 22, 20 }, { -7, -7, 15, 16 } },
};
const struct sprite_frame_2box gSpriteBank53Frame064 = {
    SPRITE_FRAME(gSpriteBank53Frame064, SPRITE_TILES_BANK53 + 0x0f9a0),
    { { -9, -9, 18, 18 }, { -7, -7, 15, 16 } },
};
const struct sprite_frame_1box gSpriteBank53Frame065 = {
    SPRITE_FRAME(gSpriteBank53Frame065, SPRITE_TILES_BANK53 + 0x0fb60),
    { { -29, -18, 60, 38 } },
};
const struct sprite_frame_1box gSpriteBank53Frame066 = {
    SPRITE_FRAME(gSpriteBank53Frame066, SPRITE_TILES_BANK53 + 0x0ffc0),
    { { -29, -14, 60, 31 } },
};

const struct sprite_piece_pos gSpriteBank53Frame000Pos[3] = { { -7, -43 }, { -7, 21 }, { -7, 37 } };
const struct sprite_piece_pos gSpriteBank53Frame001Pos[2] = { { -31, -16 }, { -31, 16 } };
const struct sprite_piece_pos gSpriteBank53Frame002Pos[2] = { { -31, -16 }, { -31, 16 } };
const struct sprite_piece_pos gSpriteBank53Frame003Pos[2] = { { -31, -16 }, { -31, 16 } };
const struct sprite_piece_pos gSpriteBank53Frame004Pos[2] = { { -31, -16 }, { -31, 16 } };
const struct sprite_piece_pos gSpriteBank53Frame005Pos[3] = { { -31, -16 }, { -31, 16 }, { 1, 16 } };
const struct sprite_piece_pos gSpriteBank53Frame006Pos[3] = { { -31, -16 }, { -31, 16 }, { 1, 16 } };
const struct sprite_piece_pos gSpriteBank53Frame007Pos[3] = { { -7, -43 }, { -7, 21 }, { -7, 37 } };
const struct sprite_piece_pos gSpriteBank53Frame008Pos[3] = { { -72, -35 }, { -8, -3 }, { -8, 13 } };
const struct sprite_piece_pos gSpriteBank53Frame009Pos[3] = { { -67, -35 }, { -3, -2 }, { -3, 14 } };
const struct sprite_piece_pos gSpriteBank53Frame010Pos[1] = { { -54, -35 } };
const struct sprite_piece_pos gSpriteBank53Frame011Pos[1] = { { -47, -35 } };
const struct sprite_piece_pos gSpriteBank53Frame012Pos[2] = { { -39, -35 }, { 25, 11 } };
const struct sprite_piece_pos gSpriteBank53Frame013Pos[2] = { { -32, -36 }, { 32, 13 } };
const struct sprite_piece_pos gSpriteBank53Frame014Pos[5] = { { -71, -50 }, { -7, -11 }, { -71, 14 }, { -39, 14 }, { -7, 14 } };
const struct sprite_piece_pos gSpriteBank53Frame015Pos[5] = { { -65, -50 }, { -1, -9 }, { -1, 7 }, { -65, 14 }, { -33, 14 } };
const struct sprite_piece_pos gSpriteBank53Frame016Pos[2] = { { -55, -49 }, { 9, -5 } };
const struct sprite_piece_pos gSpriteBank53Frame017Pos[3] = { { -47, -48 }, { 17, -4 }, { 17, 12 } };
const struct sprite_piece_pos gSpriteBank53Frame018Pos[3] = { { -40, -47 }, { 24, -2 }, { 24, 14 } };
const struct sprite_piece_pos gSpriteBank53Frame019Pos[2] = { { -37, -46 }, { 27, -1 } };
const struct sprite_piece_pos gSpriteBank53Frame020Pos[5] = { { -68, -61 }, { -4, -14 }, { -4, 2 }, { -61, 3 }, { -29, 3 } };
const struct sprite_piece_pos gSpriteBank53Frame021Pos[4] = { { -64, -61 }, { 0, -12 }, { -59, 3 }, { -27, 3 } };
const struct sprite_piece_pos gSpriteBank53Frame022Pos[4] = { { -52, -60 }, { 9, -8 }, { -55, 4 }, { -23, 4 } };
const struct sprite_piece_pos gSpriteBank53Frame023Pos[5] = { { -45, -60 }, { 15, -9 }, { -49, 4 }, { -17, 4 }, { 15, 4 } };
const struct sprite_piece_pos gSpriteBank53Frame024Pos[5] = { { -40, -59 }, { 22, -8 }, { -42, 5 }, { -10, 5 }, { 22, 5 } };
const struct sprite_piece_pos gSpriteBank53Frame025Pos[5] = { { -34, -59 }, { 29, -7 }, { -35, 5 }, { -3, 5 }, { 29, 5 } };
const struct sprite_piece_pos gSpriteBank53Frame026Pos[3] = { { -7, -43 }, { -7, 21 }, { -7, 37 } };
const struct sprite_piece_pos gSpriteBank53Frame027Pos[3] = { { -7, -43 }, { -7, 21 }, { -7, 37 } };
const struct sprite_piece_pos gSpriteBank53Frame028Pos[3] = { { -7, -43 }, { -7, 21 }, { -7, 37 } };
const struct sprite_piece_pos gSpriteBank53Frame029Pos[2] = { { -12, -12 }, { -7, 4 } };
const struct sprite_piece_pos gSpriteBank53Frame030Pos[4] = { { -11, -11 }, { 5, -10 }, { -5, 5 }, { 3, 5 } };
const struct sprite_piece_pos gSpriteBank53Frame031Pos[3] = { { -9, -9 }, { 7, -7 }, { -2, 7 } };
const struct sprite_piece_pos gSpriteBank53Frame032Pos[1] = { { -5, -6 } };
const struct sprite_piece_pos gSpriteBank53Frame033Pos[1] = { { -1, -2 } };
const struct sprite_piece_pos gSpriteBank53Frame034Pos[2] = { { -12, -11 }, { -7, 5 } };
const struct sprite_piece_pos gSpriteBank53Frame035Pos[4] = { { -11, -10 }, { 5, -9 }, { -5, 6 }, { 3, 6 } };
const struct sprite_piece_pos gSpriteBank53Frame036Pos[3] = { { -9, -8 }, { 7, -6 }, { -2, 8 } };
const struct sprite_piece_pos gSpriteBank53Frame037Pos[1] = { { -5, -5 } };
const struct sprite_piece_pos gSpriteBank53Frame038Pos[1] = { { -1, -1 } };
const struct sprite_piece_pos gSpriteBank53Frame039Pos[2] = { { -12, -10 }, { -7, 6 } };
const struct sprite_piece_pos gSpriteBank53Frame040Pos[4] = { { -11, -9 }, { 5, -8 }, { -5, 7 }, { 3, 7 } };
const struct sprite_piece_pos gSpriteBank53Frame041Pos[3] = { { -9, -7 }, { 7, -5 }, { -2, 9 } };
const struct sprite_piece_pos gSpriteBank53Frame042Pos[1] = { { -5, -4 } };
const struct sprite_piece_pos gSpriteBank53Frame043Pos[1] = { { -1, 0 } };
const struct sprite_piece_pos gSpriteBank53Frame044Pos[2] = { { -12, -11 }, { -7, 5 } };
const struct sprite_piece_pos gSpriteBank53Frame045Pos[4] = { { -11, -10 }, { 5, -9 }, { -5, 6 }, { 3, 6 } };
const struct sprite_piece_pos gSpriteBank53Frame046Pos[3] = { { -9, -8 }, { 7, -6 }, { -2, 8 } };
const struct sprite_piece_pos gSpriteBank53Frame047Pos[1] = { { -5, -5 } };
const struct sprite_piece_pos gSpriteBank53Frame048Pos[1] = { { -1, -1 } };
const struct sprite_piece_pos gSpriteBank53Frame049Pos[2] = { { -12, -11 }, { -7, 5 } };
const struct sprite_piece_pos gSpriteBank53Frame050Pos[4] = { { -11, -10 }, { 5, -9 }, { -5, 6 }, { 3, 6 } };
const struct sprite_piece_pos gSpriteBank53Frame051Pos[3] = { { -9, -8 }, { 7, -6 }, { -2, 8 } };
const struct sprite_piece_pos gSpriteBank53Frame052Pos[1] = { { -5, -5 } };
const struct sprite_piece_pos gSpriteBank53Frame053Pos[1] = { { -1, -1 } };
const struct sprite_piece_pos gSpriteBank53Frame054Pos[1] = { { -6, -6 } };
const struct sprite_piece_pos gSpriteBank53Frame055Pos[1] = { { -6, -7 } };
const struct sprite_piece_pos gSpriteBank53Frame056Pos[1] = { { -4, -4 } };
const struct sprite_piece_pos gSpriteBank53Frame057Pos[1] = { { -2, -2 } };
const struct sprite_piece_pos gSpriteBank53Frame058Pos[1] = { { -24, -24 } };
const struct sprite_piece_pos gSpriteBank53Frame059Pos[1] = { { -24, -24 } };
const struct sprite_piece_pos gSpriteBank53Frame060Pos[1] = { { -24, -24 } };
const struct sprite_piece_pos gSpriteBank53Frame061Pos[1] = { { -14, -15 } };
const struct sprite_piece_pos gSpriteBank53Frame062Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank53Frame063Pos[1] = { { -11, -10 } };
const struct sprite_piece_pos gSpriteBank53Frame064Pos[1] = { { -9, -9 } };
const struct sprite_piece_pos gSpriteBank53Frame065Pos[3] = { { -29, -18 }, { -7, 14 }, { 9, 14 } };
const struct sprite_piece_pos gSpriteBank53Frame066Pos[1] = { { -29, -14 } };

const u8 gSpriteBank53Frame000Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank53Frame001Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank53Frame002Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank53Frame003Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank53Frame004Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank53Frame005Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank53Frame006Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank53Frame007Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank53Frame008Pieces[3] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank53Frame009Pieces[3] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank53Frame010Pieces[1] = { SPRITE_PIECE(2, 3) };
const u8 gSpriteBank53Frame011Pieces[1] = { SPRITE_PIECE(2, 3) };
const u8 gSpriteBank53Frame012Pieces[2] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank53Frame013Pieces[2] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank53Frame014Pieces[5] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank53Frame015Pieces[5] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank53Frame016Pieces[2] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank53Frame017Pieces[3] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank53Frame018Pieces[3] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank53Frame019Pieces[2] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank53Frame020Pieces[5] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank53Frame021Pieces[4] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank53Frame022Pieces[4] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank53Frame023Pieces[5] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank53Frame024Pieces[5] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank53Frame025Pieces[5] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank53Frame026Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank53Frame027Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank53Frame028Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank53Frame029Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank53Frame030Pieces[4] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank53Frame031Pieces[3] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank53Frame032Pieces[1] = { SPRITE_PIECE(2, 1) };
const u8 gSpriteBank53Frame033Pieces[1] = { SPRITE_PIECE(2, 0) };
const u8 gSpriteBank53Frame034Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank53Frame035Pieces[4] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank53Frame036Pieces[3] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank53Frame037Pieces[1] = { SPRITE_PIECE(2, 1) };
const u8 gSpriteBank53Frame038Pieces[1] = { SPRITE_PIECE(2, 0) };
const u8 gSpriteBank53Frame039Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank53Frame040Pieces[4] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank53Frame041Pieces[3] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank53Frame042Pieces[1] = { SPRITE_PIECE(2, 1) };
const u8 gSpriteBank53Frame043Pieces[1] = { SPRITE_PIECE(2, 0) };
const u8 gSpriteBank53Frame044Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank53Frame045Pieces[4] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank53Frame046Pieces[3] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank53Frame047Pieces[1] = { SPRITE_PIECE(2, 1) };
const u8 gSpriteBank53Frame048Pieces[1] = { SPRITE_PIECE(2, 0) };
const u8 gSpriteBank53Frame049Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank53Frame050Pieces[4] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank53Frame051Pieces[3] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank53Frame052Pieces[1] = { SPRITE_PIECE(2, 1) };
const u8 gSpriteBank53Frame053Pieces[1] = { SPRITE_PIECE(2, 0) };
const u8 gSpriteBank53Frame054Pieces[1] = { SPRITE_PIECE(3, 1) };
const u8 gSpriteBank53Frame055Pieces[1] = { SPRITE_PIECE(3, 1) };
const u8 gSpriteBank53Frame056Pieces[1] = { SPRITE_PIECE(3, 1) };
const u8 gSpriteBank53Frame057Pieces[1] = { SPRITE_PIECE(3, 0) };
const u8 gSpriteBank53Frame058Pieces[1] = { SPRITE_PIECE(2, 3) };
const u8 gSpriteBank53Frame059Pieces[1] = { SPRITE_PIECE(2, 3) };
const u8 gSpriteBank53Frame060Pieces[1] = { SPRITE_PIECE(2, 3) };
const u8 gSpriteBank53Frame061Pieces[1] = { SPRITE_PIECE(3, 2) };
const u8 gSpriteBank53Frame062Pieces[1] = { SPRITE_PIECE(3, 2) };
const u8 gSpriteBank53Frame063Pieces[1] = { SPRITE_PIECE(3, 2) };
const u8 gSpriteBank53Frame064Pieces[1] = { SPRITE_PIECE(3, 2) };
const u8 gSpriteBank53Frame065Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank53Frame066Pieces[1] = { SPRITE_PIECE(2, 7) };

/* ---------------------------------------------------------------------- */
/* Bank 54: 10 animations, 121 frames, tiles in gSpriteBank54Tiles (SPRITE_TILES_BANK54). */

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
extern const struct sprite_frame gSpriteBank54Frame000;
extern const struct sprite_frame gSpriteBank54Frame001;
extern const struct sprite_frame gSpriteBank54Frame002;
extern const struct sprite_frame gSpriteBank54Frame003;
extern const struct sprite_frame gSpriteBank54Frame004;
extern const struct sprite_frame gSpriteBank54Frame005;
extern const struct sprite_frame gSpriteBank54Frame006;
extern const struct sprite_frame gSpriteBank54Frame007;
extern const struct sprite_frame gSpriteBank54Frame008;
extern const struct sprite_frame gSpriteBank54Frame009;
extern const struct sprite_frame gSpriteBank54Frame010;
extern const struct sprite_frame gSpriteBank54Frame011;
extern const struct sprite_frame gSpriteBank54Frame012;
extern const struct sprite_frame gSpriteBank54Frame013;
extern const struct sprite_frame gSpriteBank54Frame014;
extern const struct sprite_frame gSpriteBank54Frame015;
extern const struct sprite_frame gSpriteBank54Frame016;
extern const struct sprite_frame gSpriteBank54Frame017;
extern const struct sprite_frame gSpriteBank54Frame018;
extern const struct sprite_frame gSpriteBank54Frame019;
extern const struct sprite_frame gSpriteBank54Frame020;
extern const struct sprite_frame gSpriteBank54Frame021;
extern const struct sprite_frame gSpriteBank54Frame022;
extern const struct sprite_frame gSpriteBank54Frame023;
extern const struct sprite_frame gSpriteBank54Frame024;
extern const struct sprite_frame gSpriteBank54Frame025;
extern const struct sprite_frame gSpriteBank54Frame026;
extern const struct sprite_frame gSpriteBank54Frame027;
extern const struct sprite_frame gSpriteBank54Frame028;
extern const struct sprite_frame gSpriteBank54Frame029;
extern const struct sprite_frame gSpriteBank54Frame030;
extern const struct sprite_frame gSpriteBank54Frame031;
extern const struct sprite_frame gSpriteBank54Frame032;
extern const struct sprite_frame gSpriteBank54Frame033;
extern const struct sprite_frame gSpriteBank54Frame034;
extern const struct sprite_frame gSpriteBank54Frame035;
extern const struct sprite_frame gSpriteBank54Frame036;
extern const struct sprite_frame gSpriteBank54Frame037;
extern const struct sprite_frame gSpriteBank54Frame038;
extern const struct sprite_frame gSpriteBank54Frame039;
extern const struct sprite_frame gSpriteBank54Frame040;
extern const struct sprite_frame gSpriteBank54Frame041;
extern const struct sprite_frame gSpriteBank54Frame042;
extern const struct sprite_frame gSpriteBank54Frame043;
extern const struct sprite_frame_1box gSpriteBank54Frame044;
extern const struct sprite_frame_1box gSpriteBank54Frame045;
extern const struct sprite_frame_1box gSpriteBank54Frame046;
extern const struct sprite_frame_1box gSpriteBank54Frame047;
extern const struct sprite_frame_1box gSpriteBank54Frame048;
extern const struct sprite_frame_1box gSpriteBank54Frame049;
extern const struct sprite_frame_1box gSpriteBank54Frame050;
extern const struct sprite_frame_1box gSpriteBank54Frame051;
extern const struct sprite_frame gSpriteBank54Frame052;
extern const struct sprite_frame gSpriteBank54Frame053;
extern const struct sprite_frame gSpriteBank54Frame054;
extern const struct sprite_frame gSpriteBank54Frame055;
extern const struct sprite_frame gSpriteBank54Frame056;
extern const struct sprite_frame gSpriteBank54Frame057;
extern const struct sprite_frame gSpriteBank54Frame058;
extern const struct sprite_frame gSpriteBank54Frame059;
extern const struct sprite_frame gSpriteBank54Frame060;
extern const struct sprite_frame gSpriteBank54Frame061;
extern const struct sprite_frame gSpriteBank54Frame062;
extern const struct sprite_frame gSpriteBank54Frame063;
extern const struct sprite_frame gSpriteBank54Frame064;
extern const struct sprite_frame gSpriteBank54Frame065;
extern const struct sprite_frame gSpriteBank54Frame066;
extern const struct sprite_frame gSpriteBank54Frame067;
extern const struct sprite_frame gSpriteBank54Frame068;
extern const struct sprite_frame gSpriteBank54Frame069;
extern const struct sprite_frame gSpriteBank54Frame070;
extern const struct sprite_frame gSpriteBank54Frame071;
extern const struct sprite_frame gSpriteBank54Frame072;
extern const struct sprite_frame gSpriteBank54Frame073;
extern const struct sprite_frame gSpriteBank54Frame074;
extern const struct sprite_frame gSpriteBank54Frame075;
extern const struct sprite_frame gSpriteBank54Frame076;
extern const struct sprite_frame_1box gSpriteBank54Frame077;
extern const struct sprite_frame_1box gSpriteBank54Frame078;
extern const struct sprite_frame_1box gSpriteBank54Frame079;
extern const struct sprite_frame_1box gSpriteBank54Frame080;
extern const struct sprite_frame_1box gSpriteBank54Frame081;
extern const struct sprite_frame_1box gSpriteBank54Frame082;
extern const struct sprite_frame_1box gSpriteBank54Frame083;
extern const struct sprite_frame_1box gSpriteBank54Frame084;
extern const struct sprite_frame_1box gSpriteBank54Frame085;
extern const struct sprite_frame_1box gSpriteBank54Frame086;
extern const struct sprite_frame_1box gSpriteBank54Frame087;
extern const struct sprite_frame_1box gSpriteBank54Frame088;
extern const struct sprite_frame_1box gSpriteBank54Frame089;
extern const struct sprite_frame_1box gSpriteBank54Frame090;
extern const struct sprite_frame_1box gSpriteBank54Frame091;
extern const struct sprite_frame gSpriteBank54Frame092;
extern const struct sprite_frame gSpriteBank54Frame093;
extern const struct sprite_frame gSpriteBank54Frame094;
extern const struct sprite_frame gSpriteBank54Frame095;
extern const struct sprite_frame gSpriteBank54Frame096;
extern const struct sprite_frame gSpriteBank54Frame097;
extern const struct sprite_frame gSpriteBank54Frame098;
extern const struct sprite_frame gSpriteBank54Frame099;
extern const struct sprite_frame gSpriteBank54Frame100;
extern const struct sprite_frame gSpriteBank54Frame101;
extern const struct sprite_frame gSpriteBank54Frame102;
extern const struct sprite_frame gSpriteBank54Frame103;
extern const struct sprite_frame_1box gSpriteBank54Frame104;
extern const struct sprite_frame_1box gSpriteBank54Frame105;
extern const struct sprite_frame_1box gSpriteBank54Frame106;
extern const struct sprite_frame_1box gSpriteBank54Frame107;
extern const struct sprite_frame_1box gSpriteBank54Frame108;
extern const struct sprite_frame_1box gSpriteBank54Frame109;
extern const struct sprite_frame_1box gSpriteBank54Frame110;
extern const struct sprite_frame_1box gSpriteBank54Frame111;
extern const struct sprite_frame_1box gSpriteBank54Frame112;
extern const struct sprite_frame_1box gSpriteBank54Frame113;
extern const struct sprite_frame gSpriteBank54Frame114;
extern const struct sprite_frame gSpriteBank54Frame115;
extern const struct sprite_frame gSpriteBank54Frame116;
extern const struct sprite_frame gSpriteBank54Frame117;
extern const struct sprite_frame gSpriteBank54Frame118;
extern const struct sprite_frame gSpriteBank54Frame119;
extern const struct sprite_frame_1box gSpriteBank54Frame120;
extern const struct sprite_piece_pos gSpriteBank54Frame000Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame001Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame002Pos[3];
extern const struct sprite_piece_pos gSpriteBank54Frame003Pos[4];
extern const struct sprite_piece_pos gSpriteBank54Frame004Pos[4];
extern const struct sprite_piece_pos gSpriteBank54Frame005Pos[4];
extern const struct sprite_piece_pos gSpriteBank54Frame006Pos[3];
extern const struct sprite_piece_pos gSpriteBank54Frame007Pos[4];
extern const struct sprite_piece_pos gSpriteBank54Frame008Pos[3];
extern const struct sprite_piece_pos gSpriteBank54Frame009Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame010Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame011Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame012Pos[4];
extern const struct sprite_piece_pos gSpriteBank54Frame013Pos[3];
extern const struct sprite_piece_pos gSpriteBank54Frame014Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame015Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame016Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame017Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame018Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame019Pos[3];
extern const struct sprite_piece_pos gSpriteBank54Frame020Pos[3];
extern const struct sprite_piece_pos gSpriteBank54Frame021Pos[3];
extern const struct sprite_piece_pos gSpriteBank54Frame022Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame023Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame024Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame025Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame026Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame027Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame028Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame029Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame030Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame031Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame032Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame033Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame034Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame035Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame036Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame037Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame038Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame039Pos[4];
extern const struct sprite_piece_pos gSpriteBank54Frame040Pos[4];
extern const struct sprite_piece_pos gSpriteBank54Frame041Pos[4];
extern const struct sprite_piece_pos gSpriteBank54Frame042Pos[4];
extern const struct sprite_piece_pos gSpriteBank54Frame043Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame044Pos[4];
extern const struct sprite_piece_pos gSpriteBank54Frame045Pos[4];
extern const struct sprite_piece_pos gSpriteBank54Frame046Pos[4];
extern const struct sprite_piece_pos gSpriteBank54Frame047Pos[4];
extern const struct sprite_piece_pos gSpriteBank54Frame048Pos[4];
extern const struct sprite_piece_pos gSpriteBank54Frame049Pos[4];
extern const struct sprite_piece_pos gSpriteBank54Frame050Pos[4];
extern const struct sprite_piece_pos gSpriteBank54Frame051Pos[4];
extern const struct sprite_piece_pos gSpriteBank54Frame052Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame053Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame054Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame055Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame056Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame057Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame058Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame059Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame060Pos[1];
extern const struct sprite_piece_pos gSpriteBank54Frame061Pos[1];
extern const struct sprite_piece_pos gSpriteBank54Frame062Pos[1];
extern const struct sprite_piece_pos gSpriteBank54Frame063Pos[1];
extern const struct sprite_piece_pos gSpriteBank54Frame064Pos[1];
extern const struct sprite_piece_pos gSpriteBank54Frame065Pos[4];
extern const struct sprite_piece_pos gSpriteBank54Frame066Pos[4];
extern const struct sprite_piece_pos gSpriteBank54Frame067Pos[7];
extern const struct sprite_piece_pos gSpriteBank54Frame068Pos[7];
extern const struct sprite_piece_pos gSpriteBank54Frame069Pos[6];
extern const struct sprite_piece_pos gSpriteBank54Frame070Pos[5];
extern const struct sprite_piece_pos gSpriteBank54Frame071Pos[1];
extern const struct sprite_piece_pos gSpriteBank54Frame072Pos[1];
extern const struct sprite_piece_pos gSpriteBank54Frame073Pos[1];
extern const struct sprite_piece_pos gSpriteBank54Frame074Pos[1];
extern const struct sprite_piece_pos gSpriteBank54Frame075Pos[1];
extern const struct sprite_piece_pos gSpriteBank54Frame076Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame077Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame078Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame079Pos[5];
extern const struct sprite_piece_pos gSpriteBank54Frame080Pos[5];
extern const struct sprite_piece_pos gSpriteBank54Frame081Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame082Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame083Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame084Pos[3];
extern const struct sprite_piece_pos gSpriteBank54Frame085Pos[4];
extern const struct sprite_piece_pos gSpriteBank54Frame086Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame087Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame088Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame089Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame090Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame091Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame092Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame093Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame094Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame095Pos[3];
extern const struct sprite_piece_pos gSpriteBank54Frame096Pos[4];
extern const struct sprite_piece_pos gSpriteBank54Frame097Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame098Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame099Pos[3];
extern const struct sprite_piece_pos gSpriteBank54Frame100Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame101Pos[3];
extern const struct sprite_piece_pos gSpriteBank54Frame102Pos[3];
extern const struct sprite_piece_pos gSpriteBank54Frame103Pos[3];
extern const struct sprite_piece_pos gSpriteBank54Frame104Pos[4];
extern const struct sprite_piece_pos gSpriteBank54Frame105Pos[4];
extern const struct sprite_piece_pos gSpriteBank54Frame106Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame107Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame108Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame109Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame110Pos[4];
extern const struct sprite_piece_pos gSpriteBank54Frame111Pos[4];
extern const struct sprite_piece_pos gSpriteBank54Frame112Pos[4];
extern const struct sprite_piece_pos gSpriteBank54Frame113Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame114Pos[2];
extern const struct sprite_piece_pos gSpriteBank54Frame115Pos[4];
extern const struct sprite_piece_pos gSpriteBank54Frame116Pos[4];
extern const struct sprite_piece_pos gSpriteBank54Frame117Pos[1];
extern const struct sprite_piece_pos gSpriteBank54Frame118Pos[1];
extern const struct sprite_piece_pos gSpriteBank54Frame119Pos[3];
extern const struct sprite_piece_pos gSpriteBank54Frame120Pos[1];
extern const u8 gSpriteBank54Frame000Pieces[2];
extern const u8 gSpriteBank54Frame001Pieces[2];
extern const u8 gSpriteBank54Frame002Pieces[3];
extern const u8 gSpriteBank54Frame003Pieces[4];
extern const u8 gSpriteBank54Frame004Pieces[4];
extern const u8 gSpriteBank54Frame005Pieces[4];
extern const u8 gSpriteBank54Frame006Pieces[3];
extern const u8 gSpriteBank54Frame007Pieces[4];
extern const u8 gSpriteBank54Frame008Pieces[3];
extern const u8 gSpriteBank54Frame009Pieces[2];
extern const u8 gSpriteBank54Frame010Pieces[2];
extern const u8 gSpriteBank54Frame011Pieces[2];
extern const u8 gSpriteBank54Frame012Pieces[4];
extern const u8 gSpriteBank54Frame013Pieces[3];
extern const u8 gSpriteBank54Frame014Pieces[2];
extern const u8 gSpriteBank54Frame015Pieces[2];
extern const u8 gSpriteBank54Frame016Pieces[2];
extern const u8 gSpriteBank54Frame017Pieces[2];
extern const u8 gSpriteBank54Frame018Pieces[2];
extern const u8 gSpriteBank54Frame019Pieces[3];
extern const u8 gSpriteBank54Frame020Pieces[3];
extern const u8 gSpriteBank54Frame021Pieces[3];
extern const u8 gSpriteBank54Frame022Pieces[2];
extern const u8 gSpriteBank54Frame023Pieces[2];
extern const u8 gSpriteBank54Frame024Pieces[2];
extern const u8 gSpriteBank54Frame025Pieces[2];
extern const u8 gSpriteBank54Frame026Pieces[2];
extern const u8 gSpriteBank54Frame027Pieces[2];
extern const u8 gSpriteBank54Frame028Pieces[2];
extern const u8 gSpriteBank54Frame029Pieces[2];
extern const u8 gSpriteBank54Frame030Pieces[2];
extern const u8 gSpriteBank54Frame031Pieces[2];
extern const u8 gSpriteBank54Frame032Pieces[2];
extern const u8 gSpriteBank54Frame033Pieces[2];
extern const u8 gSpriteBank54Frame034Pieces[2];
extern const u8 gSpriteBank54Frame035Pieces[2];
extern const u8 gSpriteBank54Frame036Pieces[2];
extern const u8 gSpriteBank54Frame037Pieces[2];
extern const u8 gSpriteBank54Frame038Pieces[2];
extern const u8 gSpriteBank54Frame039Pieces[4];
extern const u8 gSpriteBank54Frame040Pieces[4];
extern const u8 gSpriteBank54Frame041Pieces[4];
extern const u8 gSpriteBank54Frame042Pieces[4];
extern const u8 gSpriteBank54Frame043Pieces[2];
extern const u8 gSpriteBank54Frame044Pieces[4];
extern const u8 gSpriteBank54Frame045Pieces[4];
extern const u8 gSpriteBank54Frame046Pieces[4];
extern const u8 gSpriteBank54Frame047Pieces[4];
extern const u8 gSpriteBank54Frame048Pieces[4];
extern const u8 gSpriteBank54Frame049Pieces[4];
extern const u8 gSpriteBank54Frame050Pieces[4];
extern const u8 gSpriteBank54Frame051Pieces[4];
extern const u8 gSpriteBank54Frame052Pieces[2];
extern const u8 gSpriteBank54Frame053Pieces[2];
extern const u8 gSpriteBank54Frame054Pieces[2];
extern const u8 gSpriteBank54Frame055Pieces[2];
extern const u8 gSpriteBank54Frame056Pieces[2];
extern const u8 gSpriteBank54Frame057Pieces[2];
extern const u8 gSpriteBank54Frame058Pieces[2];
extern const u8 gSpriteBank54Frame059Pieces[2];
extern const u8 gSpriteBank54Frame060Pieces[1];
extern const u8 gSpriteBank54Frame061Pieces[1];
extern const u8 gSpriteBank54Frame062Pieces[1];
extern const u8 gSpriteBank54Frame063Pieces[1];
extern const u8 gSpriteBank54Frame064Pieces[1];
extern const u8 gSpriteBank54Frame065Pieces[4];
extern const u8 gSpriteBank54Frame066Pieces[4];
extern const u8 gSpriteBank54Frame067Pieces[7];
extern const u8 gSpriteBank54Frame068Pieces[7];
extern const u8 gSpriteBank54Frame069Pieces[6];
extern const u8 gSpriteBank54Frame070Pieces[5];
extern const u8 gSpriteBank54Frame071Pieces[1];
extern const u8 gSpriteBank54Frame072Pieces[1];
extern const u8 gSpriteBank54Frame073Pieces[1];
extern const u8 gSpriteBank54Frame074Pieces[1];
extern const u8 gSpriteBank54Frame075Pieces[1];
extern const u8 gSpriteBank54Frame076Pieces[2];
extern const u8 gSpriteBank54Frame077Pieces[2];
extern const u8 gSpriteBank54Frame078Pieces[2];
extern const u8 gSpriteBank54Frame079Pieces[5];
extern const u8 gSpriteBank54Frame080Pieces[5];
extern const u8 gSpriteBank54Frame081Pieces[2];
extern const u8 gSpriteBank54Frame082Pieces[2];
extern const u8 gSpriteBank54Frame083Pieces[2];
extern const u8 gSpriteBank54Frame084Pieces[3];
extern const u8 gSpriteBank54Frame085Pieces[4];
extern const u8 gSpriteBank54Frame086Pieces[2];
extern const u8 gSpriteBank54Frame087Pieces[2];
extern const u8 gSpriteBank54Frame088Pieces[2];
extern const u8 gSpriteBank54Frame089Pieces[2];
extern const u8 gSpriteBank54Frame090Pieces[2];
extern const u8 gSpriteBank54Frame091Pieces[2];
extern const u8 gSpriteBank54Frame092Pieces[2];
extern const u8 gSpriteBank54Frame093Pieces[2];
extern const u8 gSpriteBank54Frame094Pieces[2];
extern const u8 gSpriteBank54Frame095Pieces[3];
extern const u8 gSpriteBank54Frame096Pieces[4];
extern const u8 gSpriteBank54Frame097Pieces[2];
extern const u8 gSpriteBank54Frame098Pieces[2];
extern const u8 gSpriteBank54Frame099Pieces[3];
extern const u8 gSpriteBank54Frame100Pieces[2];
extern const u8 gSpriteBank54Frame101Pieces[3];
extern const u8 gSpriteBank54Frame102Pieces[3];
extern const u8 gSpriteBank54Frame103Pieces[3];
extern const u8 gSpriteBank54Frame104Pieces[4];
extern const u8 gSpriteBank54Frame105Pieces[4];
extern const u8 gSpriteBank54Frame106Pieces[2];
extern const u8 gSpriteBank54Frame107Pieces[2];
extern const u8 gSpriteBank54Frame108Pieces[2];
extern const u8 gSpriteBank54Frame109Pieces[2];
extern const u8 gSpriteBank54Frame110Pieces[4];
extern const u8 gSpriteBank54Frame111Pieces[4];
extern const u8 gSpriteBank54Frame112Pieces[4];
extern const u8 gSpriteBank54Frame113Pieces[2];
extern const u8 gSpriteBank54Frame114Pieces[2];
extern const u8 gSpriteBank54Frame115Pieces[4];
extern const u8 gSpriteBank54Frame116Pieces[4];
extern const u8 gSpriteBank54Frame117Pieces[1];
extern const u8 gSpriteBank54Frame118Pieces[1];
extern const u8 gSpriteBank54Frame119Pieces[3];
extern const u8 gSpriteBank54Frame120Pieces[1];

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

const struct sprite_frame gSpriteBank54Frame000 = SPRITE_FRAME(gSpriteBank54Frame000, SPRITE_TILES_BANK54 + 0x00000);
const struct sprite_frame gSpriteBank54Frame001 = SPRITE_FRAME(gSpriteBank54Frame001, SPRITE_TILES_BANK54 + 0x00820);
const struct sprite_frame gSpriteBank54Frame002 = SPRITE_FRAME(gSpriteBank54Frame002, SPRITE_TILES_BANK54 + 0x01040);
const struct sprite_frame gSpriteBank54Frame003 = SPRITE_FRAME(gSpriteBank54Frame003, SPRITE_TILES_BANK54 + 0x018c0);
const struct sprite_frame gSpriteBank54Frame004 = SPRITE_FRAME(gSpriteBank54Frame004, SPRITE_TILES_BANK54 + 0x02160);
const struct sprite_frame gSpriteBank54Frame005 = SPRITE_FRAME(gSpriteBank54Frame005, SPRITE_TILES_BANK54 + 0x02a00);
const struct sprite_frame gSpriteBank54Frame006 = SPRITE_FRAME(gSpriteBank54Frame006, SPRITE_TILES_BANK54 + 0x032a0);
const struct sprite_frame gSpriteBank54Frame007 = SPRITE_FRAME(gSpriteBank54Frame007, SPRITE_TILES_BANK54 + 0x03b20);
const struct sprite_frame gSpriteBank54Frame008 = SPRITE_FRAME(gSpriteBank54Frame008, SPRITE_TILES_BANK54 + 0x043a0);
const struct sprite_frame gSpriteBank54Frame009 = SPRITE_FRAME(gSpriteBank54Frame009, SPRITE_TILES_BANK54 + 0x04c00);
const struct sprite_frame gSpriteBank54Frame010 = SPRITE_FRAME(gSpriteBank54Frame010, SPRITE_TILES_BANK54 + 0x05440);
const struct sprite_frame gSpriteBank54Frame011 = SPRITE_FRAME(gSpriteBank54Frame011, SPRITE_TILES_BANK54 + 0x05c80);
const struct sprite_frame gSpriteBank54Frame012 = SPRITE_FRAME(gSpriteBank54Frame012, SPRITE_TILES_BANK54 + 0x064c0);
const struct sprite_frame gSpriteBank54Frame013 = SPRITE_FRAME(gSpriteBank54Frame013, SPRITE_TILES_BANK54 + 0x06d60);
const struct sprite_frame gSpriteBank54Frame014 = SPRITE_FRAME(gSpriteBank54Frame014, SPRITE_TILES_BANK54 + 0x075e0);
const struct sprite_frame gSpriteBank54Frame015 = SPRITE_FRAME(gSpriteBank54Frame015, SPRITE_TILES_BANK54 + 0x07e00);
const struct sprite_frame gSpriteBank54Frame016 = SPRITE_FRAME(gSpriteBank54Frame016, SPRITE_TILES_BANK54 + 0x08620);
const struct sprite_frame gSpriteBank54Frame017 = SPRITE_FRAME(gSpriteBank54Frame017, SPRITE_TILES_BANK54 + 0x08ea0);
const struct sprite_frame gSpriteBank54Frame018 = SPRITE_FRAME(gSpriteBank54Frame018, SPRITE_TILES_BANK54 + 0x09720);
const struct sprite_frame gSpriteBank54Frame019 = SPRITE_FRAME(gSpriteBank54Frame019, SPRITE_TILES_BANK54 + 0x09f60);
const struct sprite_frame gSpriteBank54Frame020 = SPRITE_FRAME(gSpriteBank54Frame020, SPRITE_TILES_BANK54 + 0x0a820);
const struct sprite_frame gSpriteBank54Frame021 = SPRITE_FRAME(gSpriteBank54Frame021, SPRITE_TILES_BANK54 + 0x0b0e0);
const struct sprite_frame gSpriteBank54Frame022 = SPRITE_FRAME(gSpriteBank54Frame022, SPRITE_TILES_BANK54 + 0x0b9a0);
const struct sprite_frame gSpriteBank54Frame023 = SPRITE_FRAME(gSpriteBank54Frame023, SPRITE_TILES_BANK54 + 0x0c1e0);
const struct sprite_frame gSpriteBank54Frame024 = SPRITE_FRAME(gSpriteBank54Frame024, SPRITE_TILES_BANK54 + 0x0ca60);
const struct sprite_frame gSpriteBank54Frame025 = SPRITE_FRAME(gSpriteBank54Frame025, SPRITE_TILES_BANK54 + 0x0d2e0);
const struct sprite_frame gSpriteBank54Frame026 = SPRITE_FRAME(gSpriteBank54Frame026, SPRITE_TILES_BANK54 + 0x0db60);
const struct sprite_frame gSpriteBank54Frame027 = SPRITE_FRAME(gSpriteBank54Frame027, SPRITE_TILES_BANK54 + 0x0e3a0);
const struct sprite_frame gSpriteBank54Frame028 = SPRITE_FRAME(gSpriteBank54Frame028, SPRITE_TILES_BANK54 + 0x0ec20);
const struct sprite_frame gSpriteBank54Frame029 = SPRITE_FRAME(gSpriteBank54Frame029, SPRITE_TILES_BANK54 + 0x0f4a0);
const struct sprite_frame gSpriteBank54Frame030 = SPRITE_FRAME(gSpriteBank54Frame030, SPRITE_TILES_BANK54 + 0x0fd20);
const struct sprite_frame gSpriteBank54Frame031 = SPRITE_FRAME(gSpriteBank54Frame031, SPRITE_TILES_BANK54 + 0x105a0);
const struct sprite_frame gSpriteBank54Frame032 = SPRITE_FRAME(gSpriteBank54Frame032, SPRITE_TILES_BANK54 + 0x10e20);
const struct sprite_frame gSpriteBank54Frame033 = SPRITE_FRAME(gSpriteBank54Frame033, SPRITE_TILES_BANK54 + 0x116a0);
const struct sprite_frame gSpriteBank54Frame034 = SPRITE_FRAME(gSpriteBank54Frame034, SPRITE_TILES_BANK54 + 0x11ee0);
const struct sprite_frame gSpriteBank54Frame035 = SPRITE_FRAME(gSpriteBank54Frame035, SPRITE_TILES_BANK54 + 0x12720);
const struct sprite_frame gSpriteBank54Frame036 = SPRITE_FRAME(gSpriteBank54Frame036, SPRITE_TILES_BANK54 + 0x12f60);
const struct sprite_frame gSpriteBank54Frame037 = SPRITE_FRAME(gSpriteBank54Frame037, SPRITE_TILES_BANK54 + 0x137a0);
const struct sprite_frame gSpriteBank54Frame038 = SPRITE_FRAME(gSpriteBank54Frame038, SPRITE_TILES_BANK54 + 0x13fe0);
const struct sprite_frame gSpriteBank54Frame039 = SPRITE_FRAME(gSpriteBank54Frame039, SPRITE_TILES_BANK54 + 0x14820);
const struct sprite_frame gSpriteBank54Frame040 = SPRITE_FRAME(gSpriteBank54Frame040, SPRITE_TILES_BANK54 + 0x150a0);
const struct sprite_frame gSpriteBank54Frame041 = SPRITE_FRAME(gSpriteBank54Frame041, SPRITE_TILES_BANK54 + 0x15920);
const struct sprite_frame gSpriteBank54Frame042 = SPRITE_FRAME(gSpriteBank54Frame042, SPRITE_TILES_BANK54 + 0x161a0);
const struct sprite_frame gSpriteBank54Frame043 = SPRITE_FRAME(gSpriteBank54Frame043, SPRITE_TILES_BANK54 + 0x16a20);
const struct sprite_frame_1box gSpriteBank54Frame044 = {
    SPRITE_FRAME(gSpriteBank54Frame044, SPRITE_TILES_BANK54 + 0x17260),
    { { -31, -30, 62, 68 } },
};
const struct sprite_frame_1box gSpriteBank54Frame045 = {
    SPRITE_FRAME(gSpriteBank54Frame045, SPRITE_TILES_BANK54 + 0x18420),
    { { -31, -30, 62, 68 } },
};
const struct sprite_frame_1box gSpriteBank54Frame046 = {
    SPRITE_FRAME(gSpriteBank54Frame046, SPRITE_TILES_BANK54 + 0x195e0),
    { { -31, -30, 62, 68 } },
};
const struct sprite_frame_1box gSpriteBank54Frame047 = {
    SPRITE_FRAME(gSpriteBank54Frame047, SPRITE_TILES_BANK54 + 0x1a7a0),
    { { -31, -30, 62, 68 } },
};
const struct sprite_frame_1box gSpriteBank54Frame048 = {
    SPRITE_FRAME(gSpriteBank54Frame048, SPRITE_TILES_BANK54 + 0x1b960),
    { { -31, -30, 62, 68 } },
};
const struct sprite_frame_1box gSpriteBank54Frame049 = {
    SPRITE_FRAME(gSpriteBank54Frame049, SPRITE_TILES_BANK54 + 0x1cb20),
    { { -31, -30, 62, 68 } },
};
const struct sprite_frame_1box gSpriteBank54Frame050 = {
    SPRITE_FRAME(gSpriteBank54Frame050, SPRITE_TILES_BANK54 + 0x1dce0),
    { { -31, -30, 62, 68 } },
};
const struct sprite_frame_1box gSpriteBank54Frame051 = {
    SPRITE_FRAME(gSpriteBank54Frame051, SPRITE_TILES_BANK54 + 0x1eea0),
    { { -31, -30, 62, 68 } },
};
const struct sprite_frame gSpriteBank54Frame052 = SPRITE_FRAME(gSpriteBank54Frame052, SPRITE_TILES_BANK54 + 0x200a0);
const struct sprite_frame gSpriteBank54Frame053 = SPRITE_FRAME(gSpriteBank54Frame053, SPRITE_TILES_BANK54 + 0x208e0);
const struct sprite_frame gSpriteBank54Frame054 = SPRITE_FRAME(gSpriteBank54Frame054, SPRITE_TILES_BANK54 + 0x21160);
const struct sprite_frame gSpriteBank54Frame055 = SPRITE_FRAME(gSpriteBank54Frame055, SPRITE_TILES_BANK54 + 0x219a0);
const struct sprite_frame gSpriteBank54Frame056 = SPRITE_FRAME(gSpriteBank54Frame056, SPRITE_TILES_BANK54 + 0x221e0);
const struct sprite_frame gSpriteBank54Frame057 = SPRITE_FRAME(gSpriteBank54Frame057, SPRITE_TILES_BANK54 + 0x22a20);
const struct sprite_frame gSpriteBank54Frame058 = SPRITE_FRAME(gSpriteBank54Frame058, SPRITE_TILES_BANK54 + 0x23260);
const struct sprite_frame gSpriteBank54Frame059 = SPRITE_FRAME(gSpriteBank54Frame059, SPRITE_TILES_BANK54 + 0x23aa0);
const struct sprite_frame gSpriteBank54Frame060 = SPRITE_FRAME(gSpriteBank54Frame060, SPRITE_TILES_BANK54 + 0x242c0);
const struct sprite_frame gSpriteBank54Frame061 = SPRITE_FRAME(gSpriteBank54Frame061, SPRITE_TILES_BANK54 + 0x24ac0);
const struct sprite_frame gSpriteBank54Frame062 = SPRITE_FRAME(gSpriteBank54Frame062, SPRITE_TILES_BANK54 + 0x252c0);
const struct sprite_frame gSpriteBank54Frame063 = SPRITE_FRAME(gSpriteBank54Frame063, SPRITE_TILES_BANK54 + 0x25ac0);
const struct sprite_frame gSpriteBank54Frame064 = SPRITE_FRAME(gSpriteBank54Frame064, SPRITE_TILES_BANK54 + 0x262c0);
const struct sprite_frame gSpriteBank54Frame065 = SPRITE_FRAME(gSpriteBank54Frame065, SPRITE_TILES_BANK54 + 0x26ac0);
const struct sprite_frame gSpriteBank54Frame066 = SPRITE_FRAME(gSpriteBank54Frame066, SPRITE_TILES_BANK54 + 0x27420);
const struct sprite_frame gSpriteBank54Frame067 = SPRITE_FRAME(gSpriteBank54Frame067, SPRITE_TILES_BANK54 + 0x27dc0);
const struct sprite_frame gSpriteBank54Frame068 = SPRITE_FRAME(gSpriteBank54Frame068, SPRITE_TILES_BANK54 + 0x287e0);
const struct sprite_frame gSpriteBank54Frame069 = SPRITE_FRAME(gSpriteBank54Frame069, SPRITE_TILES_BANK54 + 0x29200);
const struct sprite_frame gSpriteBank54Frame070 = SPRITE_FRAME(gSpriteBank54Frame070, SPRITE_TILES_BANK54 + 0x29c60);
const struct sprite_frame gSpriteBank54Frame071 = SPRITE_FRAME(gSpriteBank54Frame071, SPRITE_TILES_BANK54 + 0x2a640);
const struct sprite_frame gSpriteBank54Frame072 = SPRITE_FRAME(gSpriteBank54Frame072, SPRITE_TILES_BANK54 + 0x2ae40);
const struct sprite_frame gSpriteBank54Frame073 = SPRITE_FRAME(gSpriteBank54Frame073, SPRITE_TILES_BANK54 + 0x2b640);
const struct sprite_frame gSpriteBank54Frame074 = SPRITE_FRAME(gSpriteBank54Frame074, SPRITE_TILES_BANK54 + 0x2be40);
const struct sprite_frame gSpriteBank54Frame075 = SPRITE_FRAME(gSpriteBank54Frame075, SPRITE_TILES_BANK54 + 0x2c640);
const struct sprite_frame gSpriteBank54Frame076 = SPRITE_FRAME(gSpriteBank54Frame076, SPRITE_TILES_BANK54 + 0x2ce40);
const struct sprite_frame_1box gSpriteBank54Frame077 = {
    SPRITE_FRAME(gSpriteBank54Frame077, SPRITE_TILES_BANK54 + 0x2d660),
    { { -20, -25, 25, 53 } },
};
const struct sprite_frame_1box gSpriteBank54Frame078 = {
    SPRITE_FRAME(gSpriteBank54Frame078, SPRITE_TILES_BANK54 + 0x2dea0),
    { { -19, -18, 24, 47 } },
};
const struct sprite_frame_1box gSpriteBank54Frame079 = {
    SPRITE_FRAME(gSpriteBank54Frame079, SPRITE_TILES_BANK54 + 0x2e6e0),
    { { -19, -11, 23, 37 } },
};
const struct sprite_frame_1box gSpriteBank54Frame080 = {
    SPRITE_FRAME(gSpriteBank54Frame080, SPRITE_TILES_BANK54 + 0x2ed40),
    { { -21, -8, 26, 36 } },
};
const struct sprite_frame_1box gSpriteBank54Frame081 = {
    SPRITE_FRAME(gSpriteBank54Frame081, SPRITE_TILES_BANK54 + 0x2f3a0),
    { { -19, -18, 23, 46 } },
};
const struct sprite_frame_1box gSpriteBank54Frame082 = {
    SPRITE_FRAME(gSpriteBank54Frame082, SPRITE_TILES_BANK54 + 0x2fc20),
    { { -20, -25, 25, 53 } },
};
const struct sprite_frame_1box gSpriteBank54Frame083 = {
    SPRITE_FRAME(gSpriteBank54Frame083, SPRITE_TILES_BANK54 + 0x30460),
    { { -20, -23, 24, 52 } },
};
const struct sprite_frame_1box gSpriteBank54Frame084 = {
    SPRITE_FRAME(gSpriteBank54Frame084, SPRITE_TILES_BANK54 + 0x30ca0),
    { { -20, -26, 24, 55 } },
};
const struct sprite_frame_1box gSpriteBank54Frame085 = {
    SPRITE_FRAME(gSpriteBank54Frame085, SPRITE_TILES_BANK54 + 0x31520),
    { { -22, -23, 26, 51 } },
};
const struct sprite_frame_1box gSpriteBank54Frame086 = {
    SPRITE_FRAME(gSpriteBank54Frame086, SPRITE_TILES_BANK54 + 0x31e40),
    { { -20, -25, 25, 53 } },
};
const struct sprite_frame_1box gSpriteBank54Frame087 = {
    SPRITE_FRAME(gSpriteBank54Frame087, SPRITE_TILES_BANK54 + 0x326c0),
    { { -20, -25, 25, 53 } },
};
const struct sprite_frame_1box gSpriteBank54Frame088 = {
    SPRITE_FRAME(gSpriteBank54Frame088, SPRITE_TILES_BANK54 + 0x32f40),
    { { -20, -25, 25, 53 } },
};
const struct sprite_frame_1box gSpriteBank54Frame089 = {
    SPRITE_FRAME(gSpriteBank54Frame089, SPRITE_TILES_BANK54 + 0x337c0),
    { { -20, -25, 25, 53 } },
};
const struct sprite_frame_1box gSpriteBank54Frame090 = {
    SPRITE_FRAME(gSpriteBank54Frame090, SPRITE_TILES_BANK54 + 0x34040),
    { { -20, -25, 25, 53 } },
};
const struct sprite_frame_1box gSpriteBank54Frame091 = {
    SPRITE_FRAME(gSpriteBank54Frame091, SPRITE_TILES_BANK54 + 0x348c0),
    { { -20, -25, 25, 53 } },
};
const struct sprite_frame gSpriteBank54Frame092 = SPRITE_FRAME(gSpriteBank54Frame092, SPRITE_TILES_BANK54 + 0x35140);
const struct sprite_frame gSpriteBank54Frame093 = SPRITE_FRAME(gSpriteBank54Frame093, SPRITE_TILES_BANK54 + 0x35960);
const struct sprite_frame gSpriteBank54Frame094 = SPRITE_FRAME(gSpriteBank54Frame094, SPRITE_TILES_BANK54 + 0x361a0);
const struct sprite_frame gSpriteBank54Frame095 = SPRITE_FRAME(gSpriteBank54Frame095, SPRITE_TILES_BANK54 + 0x369e0);
const struct sprite_frame gSpriteBank54Frame096 = SPRITE_FRAME(gSpriteBank54Frame096, SPRITE_TILES_BANK54 + 0x37220);
const struct sprite_frame gSpriteBank54Frame097 = SPRITE_FRAME(gSpriteBank54Frame097, SPRITE_TILES_BANK54 + 0x37a60);
const struct sprite_frame gSpriteBank54Frame098 = SPRITE_FRAME(gSpriteBank54Frame098, SPRITE_TILES_BANK54 + 0x38260);
const struct sprite_frame gSpriteBank54Frame099 = SPRITE_FRAME(gSpriteBank54Frame099, SPRITE_TILES_BANK54 + 0x38a60);
const struct sprite_frame gSpriteBank54Frame100 = SPRITE_FRAME(gSpriteBank54Frame100, SPRITE_TILES_BANK54 + 0x392a0);
const struct sprite_frame gSpriteBank54Frame101 = SPRITE_FRAME(gSpriteBank54Frame101, SPRITE_TILES_BANK54 + 0x39ae0);
const struct sprite_frame gSpriteBank54Frame102 = SPRITE_FRAME(gSpriteBank54Frame102, SPRITE_TILES_BANK54 + 0x3a340);
const struct sprite_frame gSpriteBank54Frame103 = SPRITE_FRAME(gSpriteBank54Frame103, SPRITE_TILES_BANK54 + 0x3aba0);
const struct sprite_frame_1box gSpriteBank54Frame104 = {
    SPRITE_FRAME(gSpriteBank54Frame104, SPRITE_TILES_BANK54 + 0x3b440),
    { { -1, -9, 3, 18 } },
};
const struct sprite_frame_1box gSpriteBank54Frame105 = {
    SPRITE_FRAME(gSpriteBank54Frame105, SPRITE_TILES_BANK54 + 0x3b4e0),
    { { -1, -9, 3, 18 } },
};
const struct sprite_frame_1box gSpriteBank54Frame106 = {
    SPRITE_FRAME(gSpriteBank54Frame106, SPRITE_TILES_BANK54 + 0x3b580),
    { { -1, -9, 3, 18 } },
};
const struct sprite_frame_1box gSpriteBank54Frame107 = {
    SPRITE_FRAME(gSpriteBank54Frame107, SPRITE_TILES_BANK54 + 0x3b640),
    { { -1, -9, 3, 18 } },
};
const struct sprite_frame_1box gSpriteBank54Frame108 = {
    SPRITE_FRAME(gSpriteBank54Frame108, SPRITE_TILES_BANK54 + 0x3b700),
    { { -1, -9, 3, 18 } },
};
const struct sprite_frame_1box gSpriteBank54Frame109 = {
    SPRITE_FRAME(gSpriteBank54Frame109, SPRITE_TILES_BANK54 + 0x3b7c0),
    { { -1, -9, 3, 18 } },
};
const struct sprite_frame_1box gSpriteBank54Frame110 = {
    SPRITE_FRAME(gSpriteBank54Frame110, SPRITE_TILES_BANK54 + 0x3b880),
    { { -1, -9, 3, 18 } },
};
const struct sprite_frame_1box gSpriteBank54Frame111 = {
    SPRITE_FRAME(gSpriteBank54Frame111, SPRITE_TILES_BANK54 + 0x3b940),
    { { -1, -9, 3, 18 } },
};
const struct sprite_frame_1box gSpriteBank54Frame112 = {
    SPRITE_FRAME(gSpriteBank54Frame112, SPRITE_TILES_BANK54 + 0x3b9e0),
    { { -1, -9, 3, 18 } },
};
const struct sprite_frame_1box gSpriteBank54Frame113 = {
    SPRITE_FRAME(gSpriteBank54Frame113, SPRITE_TILES_BANK54 + 0x3ba80),
    { { -1, -9, 3, 18 } },
};
const struct sprite_frame gSpriteBank54Frame114 = SPRITE_FRAME(gSpriteBank54Frame114, SPRITE_TILES_BANK54 + 0x3bae0);
const struct sprite_frame gSpriteBank54Frame115 = SPRITE_FRAME(gSpriteBank54Frame115, SPRITE_TILES_BANK54 + 0x3bde0);
const struct sprite_frame gSpriteBank54Frame116 = SPRITE_FRAME(gSpriteBank54Frame116, SPRITE_TILES_BANK54 + 0x3c140);
const struct sprite_frame gSpriteBank54Frame117 = SPRITE_FRAME(gSpriteBank54Frame117, SPRITE_TILES_BANK54 + 0x3c6e0);
const struct sprite_frame gSpriteBank54Frame118 = SPRITE_FRAME(gSpriteBank54Frame118, SPRITE_TILES_BANK54 + 0x3cee0);
const struct sprite_frame gSpriteBank54Frame119 = SPRITE_FRAME(gSpriteBank54Frame119, SPRITE_TILES_BANK54 + 0x3d6e0);
const struct sprite_frame_1box gSpriteBank54Frame120 = {
    SPRITE_FRAME(gSpriteBank54Frame120, SPRITE_TILES_BANK54 + 0x3d920),
    { { -2, -25, 5, 48 } },
};

const struct sprite_piece_pos gSpriteBank54Frame000Pos[2] = { { -34, -31 }, { 30, 0 } };
const struct sprite_piece_pos gSpriteBank54Frame001Pos[2] = { { -35, -31 }, { 29, 1 } };
const struct sprite_piece_pos gSpriteBank54Frame002Pos[3] = { { -38, -33 }, { 26, 1 }, { -17, 31 } };
const struct sprite_piece_pos gSpriteBank54Frame003Pos[4] = { { -39, -34 }, { 25, -1 }, { -21, 30 }, { -5, 30 } };
const struct sprite_piece_pos gSpriteBank54Frame004Pos[4] = { { -41, -36 }, { 23, -4 }, { -24, 28 }, { -8, 28 } };
const struct sprite_piece_pos gSpriteBank54Frame005Pos[4] = { { -42, -36 }, { 22, -5 }, { -25, 28 }, { -9, 28 } };
const struct sprite_piece_pos gSpriteBank54Frame006Pos[3] = { { -42, -34 }, { 22, -5 }, { -21, 30 } };
const struct sprite_piece_pos gSpriteBank54Frame007Pos[4] = { { -42, -33 }, { 22, -4 }, { -20, 31 }, { -12, 31 } };
const struct sprite_piece_pos gSpriteBank54Frame008Pos[3] = { { -42, -31 }, { 22, -1 }, { -17, 33 } };
const struct sprite_piece_pos gSpriteBank54Frame009Pos[2] = { { -43, -31 }, { 21, 0 } };
const struct sprite_piece_pos gSpriteBank54Frame010Pos[2] = { { -42, -33 }, { 22, 0 } };
const struct sprite_piece_pos gSpriteBank54Frame011Pos[2] = { { -41, -34 }, { 23, -1 } };
const struct sprite_piece_pos gSpriteBank54Frame012Pos[4] = { { -39, -36 }, { 25, -4 }, { -12, 28 }, { 4, 28 } };
const struct sprite_piece_pos gSpriteBank54Frame013Pos[3] = { { -38, -36 }, { 26, -5 }, { -10, 28 } };
const struct sprite_piece_pos gSpriteBank54Frame014Pos[2] = { { -35, -34 }, { 29, -5 } };
const struct sprite_piece_pos gSpriteBank54Frame015Pos[2] = { { -34, -33 }, { 30, -3 } };
const struct sprite_piece_pos gSpriteBank54Frame016Pos[2] = { { -43, -24 }, { 21, 19 } };
const struct sprite_piece_pos gSpriteBank54Frame017Pos[2] = { { -42, -26 }, { 22, 15 } };
const struct sprite_piece_pos gSpriteBank54Frame018Pos[2] = { { -39, -31 }, { 25, 10 } };
const struct sprite_piece_pos gSpriteBank54Frame019Pos[3] = { { -38, -36 }, { 26, 3 }, { -19, 28 } };
const struct sprite_piece_pos gSpriteBank54Frame020Pos[3] = { { -39, -37 }, { 25, -2 }, { -19, 27 } };
const struct sprite_piece_pos gSpriteBank54Frame021Pos[3] = { { -40, -35 }, { 24, -3 }, { -19, 29 } };
const struct sprite_piece_pos gSpriteBank54Frame022Pos[2] = { { -42, -31 }, { 22, 2 } };
const struct sprite_piece_pos gSpriteBank54Frame023Pos[2] = { { -43, -23 }, { 21, 8 } };
const struct sprite_piece_pos gSpriteBank54Frame024Pos[2] = { { -43, -22 }, { 21, 16 } };
const struct sprite_piece_pos gSpriteBank54Frame025Pos[2] = { { -43, -23 }, { 21, 21 } };
const struct sprite_piece_pos gSpriteBank54Frame026Pos[2] = { { -43, -23 }, { 21, 24 } };
const struct sprite_piece_pos gSpriteBank54Frame027Pos[2] = { { -43, -23 }, { 21, 23 } };
const struct sprite_piece_pos gSpriteBank54Frame028Pos[2] = { { -43, -23 }, { 21, 22 } };
const struct sprite_piece_pos gSpriteBank54Frame029Pos[2] = { { -43, -23 }, { 21, 20 } };
const struct sprite_piece_pos gSpriteBank54Frame030Pos[2] = { { -43, -23 }, { 21, 20 } };
const struct sprite_piece_pos gSpriteBank54Frame031Pos[2] = { { -43, -24 }, { 21, 16 } };
const struct sprite_piece_pos gSpriteBank54Frame032Pos[2] = { { -42, -24 }, { 22, 12 } };
const struct sprite_piece_pos gSpriteBank54Frame033Pos[2] = { { -41, -26 }, { 23, 3 } };
const struct sprite_piece_pos gSpriteBank54Frame034Pos[2] = { { -40, -29 }, { 24, 0 } };
const struct sprite_piece_pos gSpriteBank54Frame035Pos[2] = { { -40, -31 }, { 24, -2 } };
const struct sprite_piece_pos gSpriteBank54Frame036Pos[2] = { { -41, -31 }, { 23, -2 } };
const struct sprite_piece_pos gSpriteBank54Frame037Pos[2] = { { -41, -31 }, { 23, -1 } };
const struct sprite_piece_pos gSpriteBank54Frame038Pos[2] = { { -41, -31 }, { 23, -1 } };
const struct sprite_piece_pos gSpriteBank54Frame039Pos[4] = { { -40, -32 }, { 24, -2 }, { -11, 32 }, { -8, 32 } };
const struct sprite_piece_pos gSpriteBank54Frame040Pos[4] = { { -39, -32 }, { 25, -2 }, { -11, 32 }, { -7, 32 } };
const struct sprite_piece_pos gSpriteBank54Frame041Pos[4] = { { -39, -32 }, { 25, -2 }, { -11, 32 }, { -7, 32 } };
const struct sprite_piece_pos gSpriteBank54Frame042Pos[4] = { { -39, -32 }, { 25, -1 }, { -11, 32 }, { -7, 32 } };
const struct sprite_piece_pos gSpriteBank54Frame043Pos[2] = { { -40, -31 }, { 24, -2 } };
const struct sprite_piece_pos gSpriteBank54Frame044Pos[4] = { { -47, -47 }, { 17, -45 }, { -45, 17 }, { 19, 17 } };
const struct sprite_piece_pos gSpriteBank54Frame045Pos[4] = { { -47, -47 }, { 17, -46 }, { -45, 17 }, { 19, 17 } };
const struct sprite_piece_pos gSpriteBank54Frame046Pos[4] = { { -47, -47 }, { 17, -45 }, { -44, 17 }, { 20, 17 } };
const struct sprite_piece_pos gSpriteBank54Frame047Pos[4] = { { -47, -47 }, { 17, -46 }, { -43, 17 }, { 21, 17 } };
const struct sprite_piece_pos gSpriteBank54Frame048Pos[4] = { { -47, -47 }, { 17, -45 }, { -46, 17 }, { 18, 17 } };
const struct sprite_piece_pos gSpriteBank54Frame049Pos[4] = { { -47, -47 }, { 17, -45 }, { -45, 17 }, { 19, 17 } };
const struct sprite_piece_pos gSpriteBank54Frame050Pos[4] = { { -46, -46 }, { 18, -44 }, { -44, 18 }, { 20, 18 } };
const struct sprite_piece_pos gSpriteBank54Frame051Pos[4] = { { -47, -47 }, { 17, -45 }, { -46, 17 }, { 18, 17 } };
const struct sprite_piece_pos gSpriteBank54Frame052Pos[2] = { { -40, -30 }, { 24, 0 } };
const struct sprite_piece_pos gSpriteBank54Frame053Pos[2] = { { -41, -28 }, { 23, 3 } };
const struct sprite_piece_pos gSpriteBank54Frame054Pos[2] = { { -42, -28 }, { 22, 5 } };
const struct sprite_piece_pos gSpriteBank54Frame055Pos[2] = { { -41, -29 }, { 23, 4 } };
const struct sprite_piece_pos gSpriteBank54Frame056Pos[2] = { { -41, -29 }, { 23, 3 } };
const struct sprite_piece_pos gSpriteBank54Frame057Pos[2] = { { -41, -30 }, { 23, 1 } };
const struct sprite_piece_pos gSpriteBank54Frame058Pos[2] = { { -42, -31 }, { 22, 1 } };
const struct sprite_piece_pos gSpriteBank54Frame059Pos[2] = { { -42, -29 }, { 22, 2 } };
const struct sprite_piece_pos gSpriteBank54Frame060Pos[1] = { { -42, -28 } };
const struct sprite_piece_pos gSpriteBank54Frame061Pos[1] = { { -42, -28 } };
const struct sprite_piece_pos gSpriteBank54Frame062Pos[1] = { { -42, -28 } };
const struct sprite_piece_pos gSpriteBank54Frame063Pos[1] = { { -42, -30 } };
const struct sprite_piece_pos gSpriteBank54Frame064Pos[1] = { { -42, -32 } };
const struct sprite_piece_pos gSpriteBank54Frame065Pos[4] = { { -42, -42 }, { -41, 22 }, { -9, 22 }, { 7, 26 } };
const struct sprite_piece_pos gSpriteBank54Frame066Pos[4] = { { -40, -49 }, { -42, 15 }, { -10, 15 }, { 6, 25 } };
const struct sprite_piece_pos gSpriteBank54Frame067Pos[7] = { { -41, -51 }, { -42, 13 }, { -10, 13 }, { 6, 25 }, { -35, 29 }, { -3, 29 }, { 5, 29 } };
const struct sprite_piece_pos gSpriteBank54Frame068Pos[7] = { { -41, -51 }, { -42, 13 }, { -10, 13 }, { 6, 25 }, { -35, 29 }, { -3, 29 }, { 5, 29 } };
const struct sprite_piece_pos gSpriteBank54Frame069Pos[6] = { { -41, -52 }, { -42, 12 }, { -10, 12 }, { 6, 25 }, { -36, 28 }, { -4, 28 } };
const struct sprite_piece_pos gSpriteBank54Frame070Pos[5] = { { -41, -50 }, { -42, 14 }, { -10, 14 }, { 6, 25 }, { -31, 30 } };
const struct sprite_piece_pos gSpriteBank54Frame071Pos[1] = { { -47, -25 } };
const struct sprite_piece_pos gSpriteBank54Frame072Pos[1] = { { -49, -20 } };
const struct sprite_piece_pos gSpriteBank54Frame073Pos[1] = { { -48, -21 } };
const struct sprite_piece_pos gSpriteBank54Frame074Pos[1] = { { -46, -24 } };
const struct sprite_piece_pos gSpriteBank54Frame075Pos[1] = { { -42, -28 } };
const struct sprite_piece_pos gSpriteBank54Frame076Pos[2] = { { -42, -30 }, { 22, 2 } };
const struct sprite_piece_pos gSpriteBank54Frame077Pos[2] = { { -40, -27 }, { 24, -2 } };
const struct sprite_piece_pos gSpriteBank54Frame078Pos[2] = { { -41, -18 }, { 23, 6 } };
const struct sprite_piece_pos gSpriteBank54Frame079Pos[5] = { { -33, -11 }, { 22, 13 }, { -42, 21 }, { -10, 21 }, { 35, 21 } };
const struct sprite_piece_pos gSpriteBank54Frame080Pos[5] = { { -42, -9 }, { 22, 19 }, { -41, 23 }, { -9, 23 }, { 23, 23 } };
const struct sprite_piece_pos gSpriteBank54Frame081Pos[2] = { { -41, -17 }, { 23, 20 } };
const struct sprite_piece_pos gSpriteBank54Frame082Pos[2] = { { -39, -25 }, { 25, 18 } };
const struct sprite_piece_pos gSpriteBank54Frame083Pos[2] = { { -38, -31 }, { 26, 12 } };
const struct sprite_piece_pos gSpriteBank54Frame084Pos[3] = { { -38, -33 }, { 26, 9 }, { -17, 31 } };
const struct sprite_piece_pos gSpriteBank54Frame085Pos[4] = { { -40, -32 }, { 24, 4 }, { -40, 32 }, { -8, 32 } };
const struct sprite_piece_pos gSpriteBank54Frame086Pos[2] = { { -41, -30 }, { 23, 5 } };
const struct sprite_piece_pos gSpriteBank54Frame087Pos[2] = { { -41, -27 }, { 23, 7 } };
const struct sprite_piece_pos gSpriteBank54Frame088Pos[2] = { { -43, -24 }, { 21, 13 } };
const struct sprite_piece_pos gSpriteBank54Frame089Pos[2] = { { -43, -23 }, { 21, 20 } };
const struct sprite_piece_pos gSpriteBank54Frame090Pos[2] = { { -43, -23 }, { 21, 19 } };
const struct sprite_piece_pos gSpriteBank54Frame091Pos[2] = { { -43, -23 }, { 21, 19 } };
const struct sprite_piece_pos gSpriteBank54Frame092Pos[2] = { { -32, -31 }, { 32, 2 } };
const struct sprite_piece_pos gSpriteBank54Frame093Pos[2] = { { -28, -33 }, { -17, 31 } };
const struct sprite_piece_pos gSpriteBank54Frame094Pos[2] = { { -25, -36 }, { -21, 28 } };
const struct sprite_piece_pos gSpriteBank54Frame095Pos[3] = { { -24, -36 }, { 8, -30 }, { -20, 28 } };
const struct sprite_piece_pos gSpriteBank54Frame096Pos[4] = { { -24, -35 }, { 8, -28 }, { -18, 29 }, { -10, 29 } };
const struct sprite_piece_pos gSpriteBank54Frame097Pos[2] = { { -23, -32 }, { 9, -25 } };
const struct sprite_piece_pos gSpriteBank54Frame098Pos[2] = { { -22, -31 }, { 10, -23 } };
const struct sprite_piece_pos gSpriteBank54Frame099Pos[3] = { { -21, -33 }, { 11, -24 }, { -3, 31 } };
const struct sprite_piece_pos gSpriteBank54Frame100Pos[2] = { { -29, -36 }, { -3, 28 } };
const struct sprite_piece_pos gSpriteBank54Frame101Pos[3] = { { -34, -36 }, { -2, 28 }, { 14, 28 } };
const struct sprite_piece_pos gSpriteBank54Frame102Pos[3] = { { -38, -35 }, { 26, 16 }, { 0, 29 } };
const struct sprite_piece_pos gSpriteBank54Frame103Pos[3] = { { -41, -32 }, { 23, 14 }, { 4, 32 } };
const struct sprite_piece_pos gSpriteBank54Frame104Pos[4] = { { -3, -10 }, { 4, 3 }, { -4, 6 }, { 4, 6 } };
const struct sprite_piece_pos gSpriteBank54Frame105Pos[4] = { { -4, -10 }, { 4, 2 }, { -4, 6 }, { 4, 6 } };
const struct sprite_piece_pos gSpriteBank54Frame106Pos[2] = { { -4, -10 }, { -5, 6 } };
const struct sprite_piece_pos gSpriteBank54Frame107Pos[2] = { { -4, -10 }, { -5, 6 } };
const struct sprite_piece_pos gSpriteBank54Frame108Pos[2] = { { -4, -10 }, { -5, 6 } };
const struct sprite_piece_pos gSpriteBank54Frame109Pos[2] = { { -4, -10 }, { -5, 6 } };
const struct sprite_piece_pos gSpriteBank54Frame110Pos[4] = { { -4, -10 }, { 3, -9 }, { -5, 6 }, { 3, 6 } };
const struct sprite_piece_pos gSpriteBank54Frame111Pos[4] = { { -4, -10 }, { 4, 2 }, { -4, 6 }, { 4, 6 } };
const struct sprite_piece_pos gSpriteBank54Frame112Pos[4] = { { -3, -10 }, { 4, 3 }, { -4, 6 }, { 4, 6 } };
const struct sprite_piece_pos gSpriteBank54Frame113Pos[2] = { { -3, -10 }, { -3, 6 } };
const struct sprite_piece_pos gSpriteBank54Frame114Pos[2] = { { -12, -23 }, { -12, 9 } };
const struct sprite_piece_pos gSpriteBank54Frame115Pos[4] = { { -22, -16 }, { 10, 2 }, { -21, 16 }, { 11, 16 } };
const struct sprite_piece_pos gSpriteBank54Frame116Pos[4] = { { -27, -21 }, { -24, 11 }, { 8, 11 }, { 24, 14 } };
const struct sprite_piece_pos gSpriteBank54Frame117Pos[1] = { { -29, -33 } };
const struct sprite_piece_pos gSpriteBank54Frame118Pos[1] = { { -26, -33 } };
const struct sprite_piece_pos gSpriteBank54Frame119Pos[3] = { { -13, -13 }, { 27, 6 }, { -2, 19 } };
const struct sprite_piece_pos gSpriteBank54Frame120Pos[1] = { { -9, -26 } };

const u8 gSpriteBank54Frame000Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame001Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame002Pieces[3] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame003Pieces[4] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame004Pieces[4] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame005Pieces[4] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame006Pieces[3] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame007Pieces[4] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame008Pieces[3] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame009Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame010Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame011Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame012Pieces[4] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame013Pieces[3] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame014Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame015Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame016Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank54Frame017Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank54Frame018Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame019Pieces[3] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank54Frame020Pieces[3] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank54Frame021Pieces[3] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank54Frame022Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame023Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank54Frame024Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank54Frame025Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank54Frame026Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank54Frame027Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank54Frame028Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank54Frame029Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank54Frame030Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank54Frame031Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank54Frame032Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank54Frame033Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame034Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame035Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame036Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame037Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame038Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame039Pieces[4] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame040Pieces[4] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame041Pieces[4] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame042Pieces[4] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame043Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame044Pieces[4] = { SPRITE_PIECE(5, 3), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 7), SPRITE_PIECE(0, 2) };
const u8 gSpriteBank54Frame045Pieces[4] = { SPRITE_PIECE(5, 3), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 7), SPRITE_PIECE(0, 2) };
const u8 gSpriteBank54Frame046Pieces[4] = { SPRITE_PIECE(5, 3), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 7), SPRITE_PIECE(0, 2) };
const u8 gSpriteBank54Frame047Pieces[4] = { SPRITE_PIECE(5, 3), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 7), SPRITE_PIECE(0, 2) };
const u8 gSpriteBank54Frame048Pieces[4] = { SPRITE_PIECE(5, 3), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 7), SPRITE_PIECE(0, 2) };
const u8 gSpriteBank54Frame049Pieces[4] = { SPRITE_PIECE(5, 3), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 7), SPRITE_PIECE(0, 2) };
const u8 gSpriteBank54Frame050Pieces[4] = { SPRITE_PIECE(5, 3), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 7), SPRITE_PIECE(0, 2) };
const u8 gSpriteBank54Frame051Pieces[4] = { SPRITE_PIECE(5, 3), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 7), SPRITE_PIECE(0, 2) };
const u8 gSpriteBank54Frame052Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame053Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank54Frame054Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame055Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame056Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame057Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame058Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame059Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame060Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank54Frame061Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank54Frame062Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank54Frame063Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank54Frame064Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank54Frame065Pieces[4] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame066Pieces[4] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame067Pieces[7] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame068Pieces[7] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame069Pieces[6] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame070Pieces[5] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame071Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank54Frame072Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank54Frame073Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank54Frame074Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank54Frame075Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank54Frame076Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame077Pieces[2] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame078Pieces[2] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame079Pieces[5] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame080Pieces[5] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame081Pieces[2] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank54Frame082Pieces[2] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame083Pieces[2] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame084Pieces[3] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame085Pieces[4] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame086Pieces[2] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank54Frame087Pieces[2] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank54Frame088Pieces[2] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank54Frame089Pieces[2] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank54Frame090Pieces[2] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank54Frame091Pieces[2] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank54Frame092Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame093Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame094Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame095Pieces[3] = { SPRITE_PIECE(1, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame096Pieces[4] = { SPRITE_PIECE(1, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame097Pieces[2] = { SPRITE_PIECE(1, 11), SPRITE_PIECE(0, 11) };
const u8 gSpriteBank54Frame098Pieces[2] = { SPRITE_PIECE(1, 11), SPRITE_PIECE(0, 11) };
const u8 gSpriteBank54Frame099Pieces[3] = { SPRITE_PIECE(1, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame100Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame101Pieces[3] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame102Pieces[3] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame103Pieces[3] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame104Pieces[4] = { SPRITE_PIECE(5, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame105Pieces[4] = { SPRITE_PIECE(5, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame106Pieces[2] = { SPRITE_PIECE(5, 1), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame107Pieces[2] = { SPRITE_PIECE(5, 1), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame108Pieces[2] = { SPRITE_PIECE(5, 1), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame109Pieces[2] = { SPRITE_PIECE(5, 1), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank54Frame110Pieces[4] = { SPRITE_PIECE(5, 8), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame111Pieces[4] = { SPRITE_PIECE(5, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame112Pieces[4] = { SPRITE_PIECE(5, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame113Pieces[2] = { SPRITE_PIECE(5, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame114Pieces[2] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank54Frame115Pieces[4] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame116Pieces[4] = { SPRITE_PIECE(1, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame117Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank54Frame118Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank54Frame119Pieces[3] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank54Frame120Pieces[1] = { SPRITE_PIECE(5, 11) };

/* ---------------------------------------------------------------------- */
/* Bank 55: 9 animations, 130 frames, tiles in gSpriteBank55Tiles (SPRITE_TILES_BANK55). */

extern const u16 gSpriteBank55Anim00Seq[17];
extern const u16 gSpriteBank55Anim01Seq[18];
extern const u16 gSpriteBank55Anim02Seq[14];
extern const u16 gSpriteBank55Anim03Seq[10];
extern const u16 gSpriteBank55Anim04Seq[4];
extern const u16 gSpriteBank55Anim05Seq[15];
extern const u16 gSpriteBank55Anim06Seq[15];
extern const u16 gSpriteBank55Anim07Seq[25];
extern const u16 gSpriteBank55Anim08Seq[12];
extern const struct sprite_frame_2box gSpriteBank55Frame000;
extern const struct sprite_frame_2box gSpriteBank55Frame001;
extern const struct sprite_frame_2box gSpriteBank55Frame002;
extern const struct sprite_frame_2box gSpriteBank55Frame003;
extern const struct sprite_frame_2box gSpriteBank55Frame004;
extern const struct sprite_frame_2box gSpriteBank55Frame005;
extern const struct sprite_frame_2box gSpriteBank55Frame006;
extern const struct sprite_frame_2box gSpriteBank55Frame007;
extern const struct sprite_frame_2box gSpriteBank55Frame008;
extern const struct sprite_frame_2box gSpriteBank55Frame009;
extern const struct sprite_frame_2box gSpriteBank55Frame010;
extern const struct sprite_frame_2box gSpriteBank55Frame011;
extern const struct sprite_frame_2box gSpriteBank55Frame012;
extern const struct sprite_frame_2box gSpriteBank55Frame013;
extern const struct sprite_frame_2box gSpriteBank55Frame014;
extern const struct sprite_frame_2box gSpriteBank55Frame015;
extern const struct sprite_frame_2box gSpriteBank55Frame016;
extern const struct sprite_frame_2box gSpriteBank55Frame017;
extern const struct sprite_frame_2box gSpriteBank55Frame018;
extern const struct sprite_frame_2box gSpriteBank55Frame019;
extern const struct sprite_frame_2box gSpriteBank55Frame020;
extern const struct sprite_frame_2box gSpriteBank55Frame021;
extern const struct sprite_frame_2box gSpriteBank55Frame022;
extern const struct sprite_frame_2box gSpriteBank55Frame023;
extern const struct sprite_frame_2box gSpriteBank55Frame024;
extern const struct sprite_frame_2box gSpriteBank55Frame025;
extern const struct sprite_frame_2box gSpriteBank55Frame026;
extern const struct sprite_frame_2box gSpriteBank55Frame027;
extern const struct sprite_frame_2box gSpriteBank55Frame028;
extern const struct sprite_frame_2box gSpriteBank55Frame029;
extern const struct sprite_frame_2box gSpriteBank55Frame030;
extern const struct sprite_frame_2box gSpriteBank55Frame031;
extern const struct sprite_frame_2box gSpriteBank55Frame032;
extern const struct sprite_frame_2box gSpriteBank55Frame033;
extern const struct sprite_frame_2box gSpriteBank55Frame034;
extern const struct sprite_frame_2box gSpriteBank55Frame035;
extern const struct sprite_frame_2box gSpriteBank55Frame036;
extern const struct sprite_frame_2box gSpriteBank55Frame037;
extern const struct sprite_frame_2box gSpriteBank55Frame038;
extern const struct sprite_frame_2box gSpriteBank55Frame039;
extern const struct sprite_frame_2box gSpriteBank55Frame040;
extern const struct sprite_frame_2box gSpriteBank55Frame041;
extern const struct sprite_frame_2box gSpriteBank55Frame042;
extern const struct sprite_frame_2box gSpriteBank55Frame043;
extern const struct sprite_frame_2box gSpriteBank55Frame044;
extern const struct sprite_frame_2box gSpriteBank55Frame045;
extern const struct sprite_frame_2box gSpriteBank55Frame046;
extern const struct sprite_frame_2box gSpriteBank55Frame047;
extern const struct sprite_frame_2box gSpriteBank55Frame048;
extern const struct sprite_frame_2box gSpriteBank55Frame049;
extern const struct sprite_frame_2box gSpriteBank55Frame050;
extern const struct sprite_frame_2box gSpriteBank55Frame051;
extern const struct sprite_frame_2box gSpriteBank55Frame052;
extern const struct sprite_frame_2box gSpriteBank55Frame053;
extern const struct sprite_frame_2box gSpriteBank55Frame054;
extern const struct sprite_frame_2box gSpriteBank55Frame055;
extern const struct sprite_frame_2box gSpriteBank55Frame056;
extern const struct sprite_frame_2box gSpriteBank55Frame057;
extern const struct sprite_frame_2box gSpriteBank55Frame058;
extern const struct sprite_frame_2box gSpriteBank55Frame059;
extern const struct sprite_frame_2box gSpriteBank55Frame060;
extern const struct sprite_frame_2box gSpriteBank55Frame061;
extern const struct sprite_frame_2box gSpriteBank55Frame062;
extern const struct sprite_frame_1box gSpriteBank55Frame063;
extern const struct sprite_frame_1box gSpriteBank55Frame064;
extern const struct sprite_frame_1box gSpriteBank55Frame065;
extern const struct sprite_frame_1box gSpriteBank55Frame066;
extern const struct sprite_frame_1box gSpriteBank55Frame067;
extern const struct sprite_frame_1box gSpriteBank55Frame068;
extern const struct sprite_frame_1box gSpriteBank55Frame069;
extern const struct sprite_frame_1box gSpriteBank55Frame070;
extern const struct sprite_frame_1box gSpriteBank55Frame071;
extern const struct sprite_frame_1box gSpriteBank55Frame072;
extern const struct sprite_frame_1box gSpriteBank55Frame073;
extern const struct sprite_frame_1box gSpriteBank55Frame074;
extern const struct sprite_frame_1box gSpriteBank55Frame075;
extern const struct sprite_frame_1box gSpriteBank55Frame076;
extern const struct sprite_frame_1box gSpriteBank55Frame077;
extern const struct sprite_frame_1box gSpriteBank55Frame078;
extern const struct sprite_frame_1box gSpriteBank55Frame079;
extern const struct sprite_frame_1box gSpriteBank55Frame080;
extern const struct sprite_frame_1box gSpriteBank55Frame081;
extern const struct sprite_frame_1box gSpriteBank55Frame082;
extern const struct sprite_frame_1box gSpriteBank55Frame083;
extern const struct sprite_frame_1box gSpriteBank55Frame084;
extern const struct sprite_frame_1box gSpriteBank55Frame085;
extern const struct sprite_frame_1box gSpriteBank55Frame086;
extern const struct sprite_frame_1box gSpriteBank55Frame087;
extern const struct sprite_frame_1box gSpriteBank55Frame088;
extern const struct sprite_frame_1box gSpriteBank55Frame089;
extern const struct sprite_frame_1box gSpriteBank55Frame090;
extern const struct sprite_frame_1box gSpriteBank55Frame091;
extern const struct sprite_frame_1box gSpriteBank55Frame092;
extern const struct sprite_frame_1box gSpriteBank55Frame093;
extern const struct sprite_frame_1box gSpriteBank55Frame094;
extern const struct sprite_frame_1box gSpriteBank55Frame095;
extern const struct sprite_frame_1box gSpriteBank55Frame096;
extern const struct sprite_frame_1box gSpriteBank55Frame097;
extern const struct sprite_frame_1box gSpriteBank55Frame098;
extern const struct sprite_frame_1box gSpriteBank55Frame099;
extern const struct sprite_frame_1box gSpriteBank55Frame100;
extern const struct sprite_frame_1box gSpriteBank55Frame101;
extern const struct sprite_frame_1box gSpriteBank55Frame102;
extern const struct sprite_frame_1box gSpriteBank55Frame103;
extern const struct sprite_frame_1box gSpriteBank55Frame104;
extern const struct sprite_frame_1box gSpriteBank55Frame105;
extern const struct sprite_frame_1box gSpriteBank55Frame106;
extern const struct sprite_frame_1box gSpriteBank55Frame107;
extern const struct sprite_frame_1box gSpriteBank55Frame108;
extern const struct sprite_frame_1box gSpriteBank55Frame109;
extern const struct sprite_frame_1box gSpriteBank55Frame110;
extern const struct sprite_frame_1box gSpriteBank55Frame111;
extern const struct sprite_frame_1box gSpriteBank55Frame112;
extern const struct sprite_frame_1box gSpriteBank55Frame113;
extern const struct sprite_frame_1box gSpriteBank55Frame114;
extern const struct sprite_frame_1box gSpriteBank55Frame115;
extern const struct sprite_frame_1box gSpriteBank55Frame116;
extern const struct sprite_frame_1box gSpriteBank55Frame117;
extern const struct sprite_frame_1box gSpriteBank55Frame118;
extern const struct sprite_frame_1box gSpriteBank55Frame119;
extern const struct sprite_frame_1box gSpriteBank55Frame120;
extern const struct sprite_frame_1box gSpriteBank55Frame121;
extern const struct sprite_frame_1box gSpriteBank55Frame122;
extern const struct sprite_frame_1box gSpriteBank55Frame123;
extern const struct sprite_frame_1box gSpriteBank55Frame124;
extern const struct sprite_frame_1box gSpriteBank55Frame125;
extern const struct sprite_frame_1box gSpriteBank55Frame126;
extern const struct sprite_frame_1box gSpriteBank55Frame127;
extern const struct sprite_frame_1box gSpriteBank55Frame128;
extern const struct sprite_frame_1box gSpriteBank55Frame129;
extern const struct sprite_piece_pos gSpriteBank55Frame000Pos[6];
extern const struct sprite_piece_pos gSpriteBank55Frame001Pos[5];
extern const struct sprite_piece_pos gSpriteBank55Frame002Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame003Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame004Pos[4];
extern const struct sprite_piece_pos gSpriteBank55Frame005Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame006Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame007Pos[4];
extern const struct sprite_piece_pos gSpriteBank55Frame008Pos[4];
extern const struct sprite_piece_pos gSpriteBank55Frame009Pos[2];
extern const struct sprite_piece_pos gSpriteBank55Frame010Pos[2];
extern const struct sprite_piece_pos gSpriteBank55Frame011Pos[1];
extern const struct sprite_piece_pos gSpriteBank55Frame012Pos[1];
extern const struct sprite_piece_pos gSpriteBank55Frame013Pos[2];
extern const struct sprite_piece_pos gSpriteBank55Frame014Pos[2];
extern const struct sprite_piece_pos gSpriteBank55Frame015Pos[5];
extern const struct sprite_piece_pos gSpriteBank55Frame016Pos[5];
extern const struct sprite_piece_pos gSpriteBank55Frame017Pos[2];
extern const struct sprite_piece_pos gSpriteBank55Frame018Pos[4];
extern const struct sprite_piece_pos gSpriteBank55Frame019Pos[5];
extern const struct sprite_piece_pos gSpriteBank55Frame020Pos[6];
extern const struct sprite_piece_pos gSpriteBank55Frame021Pos[5];
extern const struct sprite_piece_pos gSpriteBank55Frame022Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame023Pos[5];
extern const struct sprite_piece_pos gSpriteBank55Frame024Pos[5];
extern const struct sprite_piece_pos gSpriteBank55Frame025Pos[6];
extern const struct sprite_piece_pos gSpriteBank55Frame026Pos[6];
extern const struct sprite_piece_pos gSpriteBank55Frame027Pos[5];
extern const struct sprite_piece_pos gSpriteBank55Frame028Pos[6];
extern const struct sprite_piece_pos gSpriteBank55Frame029Pos[5];
extern const struct sprite_piece_pos gSpriteBank55Frame030Pos[7];
extern const struct sprite_piece_pos gSpriteBank55Frame031Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame032Pos[7];
extern const struct sprite_piece_pos gSpriteBank55Frame033Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame034Pos[5];
extern const struct sprite_piece_pos gSpriteBank55Frame035Pos[2];
extern const struct sprite_piece_pos gSpriteBank55Frame036Pos[4];
extern const struct sprite_piece_pos gSpriteBank55Frame037Pos[5];
extern const struct sprite_piece_pos gSpriteBank55Frame038Pos[6];
extern const struct sprite_piece_pos gSpriteBank55Frame039Pos[5];
extern const struct sprite_piece_pos gSpriteBank55Frame040Pos[2];
extern const struct sprite_piece_pos gSpriteBank55Frame041Pos[2];
extern const struct sprite_piece_pos gSpriteBank55Frame042Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame043Pos[2];
extern const struct sprite_piece_pos gSpriteBank55Frame044Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame045Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame046Pos[4];
extern const struct sprite_piece_pos gSpriteBank55Frame047Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame048Pos[5];
extern const struct sprite_piece_pos gSpriteBank55Frame049Pos[2];
extern const struct sprite_piece_pos gSpriteBank55Frame050Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame051Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame052Pos[4];
extern const struct sprite_piece_pos gSpriteBank55Frame053Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame054Pos[4];
extern const struct sprite_piece_pos gSpriteBank55Frame055Pos[4];
extern const struct sprite_piece_pos gSpriteBank55Frame056Pos[4];
extern const struct sprite_piece_pos gSpriteBank55Frame057Pos[4];
extern const struct sprite_piece_pos gSpriteBank55Frame058Pos[4];
extern const struct sprite_piece_pos gSpriteBank55Frame059Pos[2];
extern const struct sprite_piece_pos gSpriteBank55Frame060Pos[4];
extern const struct sprite_piece_pos gSpriteBank55Frame061Pos[5];
extern const struct sprite_piece_pos gSpriteBank55Frame062Pos[6];
extern const struct sprite_piece_pos gSpriteBank55Frame063Pos[1];
extern const struct sprite_piece_pos gSpriteBank55Frame064Pos[1];
extern const struct sprite_piece_pos gSpriteBank55Frame065Pos[2];
extern const struct sprite_piece_pos gSpriteBank55Frame066Pos[2];
extern const struct sprite_piece_pos gSpriteBank55Frame067Pos[1];
extern const struct sprite_piece_pos gSpriteBank55Frame068Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame069Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame070Pos[4];
extern const struct sprite_piece_pos gSpriteBank55Frame071Pos[5];
extern const struct sprite_piece_pos gSpriteBank55Frame072Pos[4];
extern const struct sprite_piece_pos gSpriteBank55Frame073Pos[4];
extern const struct sprite_piece_pos gSpriteBank55Frame074Pos[5];
extern const struct sprite_piece_pos gSpriteBank55Frame075Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame076Pos[2];
extern const struct sprite_piece_pos gSpriteBank55Frame077Pos[2];
extern const struct sprite_piece_pos gSpriteBank55Frame078Pos[5];
extern const struct sprite_piece_pos gSpriteBank55Frame079Pos[5];
extern const struct sprite_piece_pos gSpriteBank55Frame080Pos[5];
extern const struct sprite_piece_pos gSpriteBank55Frame081Pos[5];
extern const struct sprite_piece_pos gSpriteBank55Frame082Pos[2];
extern const struct sprite_piece_pos gSpriteBank55Frame083Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame084Pos[7];
extern const struct sprite_piece_pos gSpriteBank55Frame085Pos[6];
extern const struct sprite_piece_pos gSpriteBank55Frame086Pos[6];
extern const struct sprite_piece_pos gSpriteBank55Frame087Pos[6];
extern const struct sprite_piece_pos gSpriteBank55Frame088Pos[6];
extern const struct sprite_piece_pos gSpriteBank55Frame089Pos[6];
extern const struct sprite_piece_pos gSpriteBank55Frame090Pos[6];
extern const struct sprite_piece_pos gSpriteBank55Frame091Pos[6];
extern const struct sprite_piece_pos gSpriteBank55Frame092Pos[6];
extern const struct sprite_piece_pos gSpriteBank55Frame093Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame094Pos[2];
extern const struct sprite_piece_pos gSpriteBank55Frame095Pos[5];
extern const struct sprite_piece_pos gSpriteBank55Frame096Pos[1];
extern const struct sprite_piece_pos gSpriteBank55Frame097Pos[2];
extern const struct sprite_piece_pos gSpriteBank55Frame098Pos[4];
extern const struct sprite_piece_pos gSpriteBank55Frame099Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame100Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame101Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame102Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame103Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame104Pos[7];
extern const struct sprite_piece_pos gSpriteBank55Frame105Pos[6];
extern const struct sprite_piece_pos gSpriteBank55Frame106Pos[6];
extern const struct sprite_piece_pos gSpriteBank55Frame107Pos[7];
extern const struct sprite_piece_pos gSpriteBank55Frame108Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame109Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame110Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame111Pos[4];
extern const struct sprite_piece_pos gSpriteBank55Frame112Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame113Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame114Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame115Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame116Pos[4];
extern const struct sprite_piece_pos gSpriteBank55Frame117Pos[5];
extern const struct sprite_piece_pos gSpriteBank55Frame118Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame119Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame120Pos[4];
extern const struct sprite_piece_pos gSpriteBank55Frame121Pos[5];
extern const struct sprite_piece_pos gSpriteBank55Frame122Pos[4];
extern const struct sprite_piece_pos gSpriteBank55Frame123Pos[4];
extern const struct sprite_piece_pos gSpriteBank55Frame124Pos[4];
extern const struct sprite_piece_pos gSpriteBank55Frame125Pos[4];
extern const struct sprite_piece_pos gSpriteBank55Frame126Pos[4];
extern const struct sprite_piece_pos gSpriteBank55Frame127Pos[4];
extern const struct sprite_piece_pos gSpriteBank55Frame128Pos[3];
extern const struct sprite_piece_pos gSpriteBank55Frame129Pos[2];
extern const u8 gSpriteBank55Frame000Pieces[6];
extern const u8 gSpriteBank55Frame001Pieces[5];
extern const u8 gSpriteBank55Frame002Pieces[3];
extern const u8 gSpriteBank55Frame003Pieces[3];
extern const u8 gSpriteBank55Frame004Pieces[4];
extern const u8 gSpriteBank55Frame005Pieces[3];
extern const u8 gSpriteBank55Frame006Pieces[3];
extern const u8 gSpriteBank55Frame007Pieces[4];
extern const u8 gSpriteBank55Frame008Pieces[4];
extern const u8 gSpriteBank55Frame009Pieces[2];
extern const u8 gSpriteBank55Frame010Pieces[2];
extern const u8 gSpriteBank55Frame011Pieces[1];
extern const u8 gSpriteBank55Frame012Pieces[1];
extern const u8 gSpriteBank55Frame013Pieces[2];
extern const u8 gSpriteBank55Frame014Pieces[2];
extern const u8 gSpriteBank55Frame015Pieces[5];
extern const u8 gSpriteBank55Frame016Pieces[5];
extern const u8 gSpriteBank55Frame017Pieces[2];
extern const u8 gSpriteBank55Frame018Pieces[4];
extern const u8 gSpriteBank55Frame019Pieces[5];
extern const u8 gSpriteBank55Frame020Pieces[6];
extern const u8 gSpriteBank55Frame021Pieces[5];
extern const u8 gSpriteBank55Frame022Pieces[3];
extern const u8 gSpriteBank55Frame023Pieces[5];
extern const u8 gSpriteBank55Frame024Pieces[5];
extern const u8 gSpriteBank55Frame025Pieces[6];
extern const u8 gSpriteBank55Frame026Pieces[6];
extern const u8 gSpriteBank55Frame027Pieces[5];
extern const u8 gSpriteBank55Frame028Pieces[6];
extern const u8 gSpriteBank55Frame029Pieces[5];
extern const u8 gSpriteBank55Frame030Pieces[7];
extern const u8 gSpriteBank55Frame031Pieces[3];
extern const u8 gSpriteBank55Frame032Pieces[7];
extern const u8 gSpriteBank55Frame033Pieces[3];
extern const u8 gSpriteBank55Frame034Pieces[5];
extern const u8 gSpriteBank55Frame035Pieces[2];
extern const u8 gSpriteBank55Frame036Pieces[4];
extern const u8 gSpriteBank55Frame037Pieces[5];
extern const u8 gSpriteBank55Frame038Pieces[6];
extern const u8 gSpriteBank55Frame039Pieces[5];
extern const u8 gSpriteBank55Frame040Pieces[2];
extern const u8 gSpriteBank55Frame041Pieces[2];
extern const u8 gSpriteBank55Frame042Pieces[3];
extern const u8 gSpriteBank55Frame043Pieces[2];
extern const u8 gSpriteBank55Frame044Pieces[3];
extern const u8 gSpriteBank55Frame045Pieces[3];
extern const u8 gSpriteBank55Frame046Pieces[4];
extern const u8 gSpriteBank55Frame047Pieces[3];
extern const u8 gSpriteBank55Frame048Pieces[5];
extern const u8 gSpriteBank55Frame049Pieces[2];
extern const u8 gSpriteBank55Frame050Pieces[3];
extern const u8 gSpriteBank55Frame051Pieces[3];
extern const u8 gSpriteBank55Frame052Pieces[4];
extern const u8 gSpriteBank55Frame053Pieces[3];
extern const u8 gSpriteBank55Frame054Pieces[4];
extern const u8 gSpriteBank55Frame055Pieces[4];
extern const u8 gSpriteBank55Frame056Pieces[4];
extern const u8 gSpriteBank55Frame057Pieces[4];
extern const u8 gSpriteBank55Frame058Pieces[4];
extern const u8 gSpriteBank55Frame059Pieces[2];
extern const u8 gSpriteBank55Frame060Pieces[4];
extern const u8 gSpriteBank55Frame061Pieces[5];
extern const u8 gSpriteBank55Frame062Pieces[6];
extern const u8 gSpriteBank55Frame063Pieces[1];
extern const u8 gSpriteBank55Frame064Pieces[1];
extern const u8 gSpriteBank55Frame065Pieces[2];
extern const u8 gSpriteBank55Frame066Pieces[2];
extern const u8 gSpriteBank55Frame067Pieces[1];
extern const u8 gSpriteBank55Frame068Pieces[3];
extern const u8 gSpriteBank55Frame069Pieces[3];
extern const u8 gSpriteBank55Frame070Pieces[4];
extern const u8 gSpriteBank55Frame071Pieces[5];
extern const u8 gSpriteBank55Frame072Pieces[4];
extern const u8 gSpriteBank55Frame073Pieces[4];
extern const u8 gSpriteBank55Frame074Pieces[5];
extern const u8 gSpriteBank55Frame075Pieces[3];
extern const u8 gSpriteBank55Frame076Pieces[2];
extern const u8 gSpriteBank55Frame077Pieces[2];
extern const u8 gSpriteBank55Frame078Pieces[5];
extern const u8 gSpriteBank55Frame079Pieces[5];
extern const u8 gSpriteBank55Frame080Pieces[5];
extern const u8 gSpriteBank55Frame081Pieces[5];
extern const u8 gSpriteBank55Frame082Pieces[2];
extern const u8 gSpriteBank55Frame083Pieces[3];
extern const u8 gSpriteBank55Frame084Pieces[7];
extern const u8 gSpriteBank55Frame085Pieces[6];
extern const u8 gSpriteBank55Frame086Pieces[6];
extern const u8 gSpriteBank55Frame087Pieces[6];
extern const u8 gSpriteBank55Frame088Pieces[6];
extern const u8 gSpriteBank55Frame089Pieces[6];
extern const u8 gSpriteBank55Frame090Pieces[6];
extern const u8 gSpriteBank55Frame091Pieces[6];
extern const u8 gSpriteBank55Frame092Pieces[6];
extern const u8 gSpriteBank55Frame093Pieces[3];
extern const u8 gSpriteBank55Frame094Pieces[2];
extern const u8 gSpriteBank55Frame095Pieces[5];
extern const u8 gSpriteBank55Frame096Pieces[1];
extern const u8 gSpriteBank55Frame097Pieces[2];
extern const u8 gSpriteBank55Frame098Pieces[4];
extern const u8 gSpriteBank55Frame099Pieces[3];
extern const u8 gSpriteBank55Frame100Pieces[3];
extern const u8 gSpriteBank55Frame101Pieces[3];
extern const u8 gSpriteBank55Frame102Pieces[3];
extern const u8 gSpriteBank55Frame103Pieces[3];
extern const u8 gSpriteBank55Frame104Pieces[7];
extern const u8 gSpriteBank55Frame105Pieces[6];
extern const u8 gSpriteBank55Frame106Pieces[6];
extern const u8 gSpriteBank55Frame107Pieces[7];
extern const u8 gSpriteBank55Frame108Pieces[3];
extern const u8 gSpriteBank55Frame109Pieces[3];
extern const u8 gSpriteBank55Frame110Pieces[3];
extern const u8 gSpriteBank55Frame111Pieces[4];
extern const u8 gSpriteBank55Frame112Pieces[3];
extern const u8 gSpriteBank55Frame113Pieces[3];
extern const u8 gSpriteBank55Frame114Pieces[3];
extern const u8 gSpriteBank55Frame115Pieces[3];
extern const u8 gSpriteBank55Frame116Pieces[4];
extern const u8 gSpriteBank55Frame117Pieces[5];
extern const u8 gSpriteBank55Frame118Pieces[3];
extern const u8 gSpriteBank55Frame119Pieces[3];
extern const u8 gSpriteBank55Frame120Pieces[4];
extern const u8 gSpriteBank55Frame121Pieces[5];
extern const u8 gSpriteBank55Frame122Pieces[4];
extern const u8 gSpriteBank55Frame123Pieces[4];
extern const u8 gSpriteBank55Frame124Pieces[4];
extern const u8 gSpriteBank55Frame125Pieces[4];
extern const u8 gSpriteBank55Frame126Pieces[4];
extern const u8 gSpriteBank55Frame127Pieces[4];
extern const u8 gSpriteBank55Frame128Pieces[3];
extern const u8 gSpriteBank55Frame129Pieces[2];

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
    SPRITE_FRAME(gSpriteBank55Frame000, SPRITE_TILES_BANK55 + 0x00000),
    { { -37, -63, 83, 97 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame001 = {
    SPRITE_FRAME(gSpriteBank55Frame001, SPRITE_TILES_BANK55 + 0x00ae0),
    { { -68, -67, 80, 96 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame002 = {
    SPRITE_FRAME(gSpriteBank55Frame002, SPRITE_TILES_BANK55 + 0x01b20),
    { { -67, -68, 74, 101 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame003 = {
    SPRITE_FRAME(gSpriteBank55Frame003, SPRITE_TILES_BANK55 + 0x02740),
    { { -66, -67, 69, 108 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame004 = {
    SPRITE_FRAME(gSpriteBank55Frame004, SPRITE_TILES_BANK55 + 0x033c0),
    { { -65, -65, 68, 110 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame005 = {
    SPRITE_FRAME(gSpriteBank55Frame005, SPRITE_TILES_BANK55 + 0x04060),
    { { -65, -62, 68, 106 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame006 = {
    SPRITE_FRAME(gSpriteBank55Frame006, SPRITE_TILES_BANK55 + 0x04ca0),
    { { -68, -56, 73, 93 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame007 = {
    SPRITE_FRAME(gSpriteBank55Frame007, SPRITE_TILES_BANK55 + 0x05880),
    { { -66, -43, 85, 74 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame008 = {
    SPRITE_FRAME(gSpriteBank55Frame008, SPRITE_TILES_BANK55 + 0x06600),
    { { -53, -36, 83, 67 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame009 = {
    SPRITE_FRAME(gSpriteBank55Frame009, SPRITE_TILES_BANK55 + 0x06ee0),
    { { -46, -31, 75, 62 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame010 = {
    SPRITE_FRAME(gSpriteBank55Frame010, SPRITE_TILES_BANK55 + 0x07720),
    { { -48, -28, 66, 59 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame011 = {
    SPRITE_FRAME(gSpriteBank55Frame011, SPRITE_TILES_BANK55 + 0x07f40),
    { { -50, -25, 61, 58 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame012 = {
    SPRITE_FRAME(gSpriteBank55Frame012, SPRITE_TILES_BANK55 + 0x08700),
    { { -50, -25, 61, 58 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame013 = {
    SPRITE_FRAME(gSpriteBank55Frame013, SPRITE_TILES_BANK55 + 0x08ec0),
    { { -50, -23, 65, 56 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame014 = {
    SPRITE_FRAME(gSpriteBank55Frame014, SPRITE_TILES_BANK55 + 0x096e0),
    { { -48, -19, 73, 52 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame015 = {
    SPRITE_FRAME(gSpriteBank55Frame015, SPRITE_TILES_BANK55 + 0x09f20),
    { { -45, -12, 88, 45 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame016 = {
    SPRITE_FRAME(gSpriteBank55Frame016, SPRITE_TILES_BANK55 + 0x0a680),
    { { -43, -8, 92, 41 }, { -25, -21, 37, 46 } },
};
const struct sprite_frame_2box gSpriteBank55Frame017 = {
    SPRITE_FRAME(gSpriteBank55Frame017, SPRITE_TILES_BANK55 + 0x0ade0),
    { { -26, -22, 57, 67 }, { -15, -12, 29, 54 } },
};
const struct sprite_frame_2box gSpriteBank55Frame018 = {
    SPRITE_FRAME(gSpriteBank55Frame018, SPRITE_TILES_BANK55 + 0x0b600),
    { { -27, -47, 48, 87 }, { -18, -28, 23, 61 } },
};
const struct sprite_frame_2box gSpriteBank55Frame019 = {
    SPRITE_FRAME(gSpriteBank55Frame019, SPRITE_TILES_BANK55 + 0x0bf00),
    { { -33, -61, 44, 98 }, { -21, -35, 22, 66 } },
};
const struct sprite_frame_2box gSpriteBank55Frame020 = {
    SPRITE_FRAME(gSpriteBank55Frame020, SPRITE_TILES_BANK55 + 0x0c940),
    { { -35, -61, 44, 96 }, { -29, -34, 31, 62 } },
};
const struct sprite_frame_2box gSpriteBank55Frame021 = {
    SPRITE_FRAME(gSpriteBank55Frame021, SPRITE_TILES_BANK55 + 0x0d3c0),
    { { -37, -56, 50, 87 }, { -29, -44, 32, 71 } },
};
const struct sprite_frame_2box gSpriteBank55Frame022 = {
    SPRITE_FRAME(gSpriteBank55Frame022, SPRITE_TILES_BANK55 + 0x0dd80),
    { { -37, -42, 64, 77 }, { -29, -44, 32, 71 } },
};
const struct sprite_frame_2box gSpriteBank55Frame023 = {
    SPRITE_FRAME(gSpriteBank55Frame023, SPRITE_TILES_BANK55 + 0x0e620),
    { { -38, -42, 82, 72 }, { -29, -44, 32, 71 } },
};
const struct sprite_frame_2box gSpriteBank55Frame024 = {
    SPRITE_FRAME(gSpriteBank55Frame024, SPRITE_TILES_BANK55 + 0x0ef60),
    { { -42, -62, 88, 81 }, { -29, -44, 32, 71 } },
};
const struct sprite_frame_2box gSpriteBank55Frame025 = {
    SPRITE_FRAME(gSpriteBank55Frame025, SPRITE_TILES_BANK55 + 0x0f920),
    { { -51, -71, 82, 80 }, { -29, -44, 32, 71 } },
};
const struct sprite_frame_2box gSpriteBank55Frame026 = {
    SPRITE_FRAME(gSpriteBank55Frame026, SPRITE_TILES_BANK55 + 0x102e0),
    { { -48, -72, 81, 82 }, { -29, -44, 32, 71 } },
};
const struct sprite_frame_2box gSpriteBank55Frame027 = {
    SPRITE_FRAME(gSpriteBank55Frame027, SPRITE_TILES_BANK55 + 0x10ca0),
    { { -36, -69, 91, 88 }, { -25, -55, 63, 43 } },
};
const struct sprite_frame_2box gSpriteBank55Frame028 = {
    SPRITE_FRAME(gSpriteBank55Frame028, SPRITE_TILES_BANK55 + 0x11800),
    { { -30, -60, 78, 79 }, { -27, -12, 73, 31 } },
};
const struct sprite_frame_2box gSpriteBank55Frame029 = {
    SPRITE_FRAME(gSpriteBank55Frame029, SPRITE_TILES_BANK55 + 0x121e0),
    { { -33, -54, 66, 84 }, { -31, -15, 63, 42 } },
};
const struct sprite_frame_2box gSpriteBank55Frame030 = {
    SPRITE_FRAME(gSpriteBank55Frame030, SPRITE_TILES_BANK55 + 0x12b60),
    { { -28, -54, 61, 86 }, { -25, -4, 60, 32 } },
};
const struct sprite_frame_2box gSpriteBank55Frame031 = {
    SPRITE_FRAME(gSpriteBank55Frame031, SPRITE_TILES_BANK55 + 0x13580),
    { { -26, -54, 59, 96 }, { -23, 4, 57, 24 } },
};
const struct sprite_frame_2box gSpriteBank55Frame032 = {
    SPRITE_FRAME(gSpriteBank55Frame032, SPRITE_TILES_BANK55 + 0x141a0),
    { { -32, -54, 65, 87 }, { -26, 2, 62, 27 } },
};
const struct sprite_frame_2box gSpriteBank55Frame033 = {
    SPRITE_FRAME(gSpriteBank55Frame033, SPRITE_TILES_BANK55 + 0x14d00),
    { { -31, -54, 64, 89 }, { -29, -44, 32, 71 } },
};
const struct sprite_frame_2box gSpriteBank55Frame034 = {
    SPRITE_FRAME(gSpriteBank55Frame034, SPRITE_TILES_BANK55 + 0x15980),
    { { -25, -54, 58, 87 }, { -29, -44, 32, 71 } },
};
const struct sprite_frame_2box gSpriteBank55Frame035 = {
    SPRITE_FRAME(gSpriteBank55Frame035, SPRITE_TILES_BANK55 + 0x0ade0),
    { { -26, -22, 57, 67 }, { -30, 14, 59, 29 } },
};
const struct sprite_frame_2box gSpriteBank55Frame036 = {
    SPRITE_FRAME(gSpriteBank55Frame036, SPRITE_TILES_BANK55 + 0x0b600),
    { { -27, -47, 48, 87 }, { -24, -19, 28, 56 } },
};
const struct sprite_frame_2box gSpriteBank55Frame037 = {
    SPRITE_FRAME(gSpriteBank55Frame037, SPRITE_TILES_BANK55 + 0x0bf00),
    { { -33, -61, 44, 98 }, { -27, -23, 27, 60 } },
};
const struct sprite_frame_2box gSpriteBank55Frame038 = {
    SPRITE_FRAME(gSpriteBank55Frame038, SPRITE_TILES_BANK55 + 0x0c940),
    { { -35, -61, 44, 96 }, { -28, -29, 27, 61 } },
};
const struct sprite_frame_2box gSpriteBank55Frame039 = {
    SPRITE_FRAME(gSpriteBank55Frame039, SPRITE_TILES_BANK55 + 0x0d3c0),
    { { -37, -56, 50, 87 }, { -27, -32, 26, 59 } },
};
const struct sprite_frame_2box gSpriteBank55Frame040 = {
    SPRITE_FRAME(gSpriteBank55Frame040, SPRITE_TILES_BANK55 + 0x16400),
    { { -38, -50, 56, 79 }, { -27, -31, 28, 57 } },
};
const struct sprite_frame_2box gSpriteBank55Frame041 = {
    SPRITE_FRAME(gSpriteBank55Frame041, SPRITE_TILES_BANK55 + 0x16c80),
    { { -36, -47, 61, 76 }, { -31, -19, 35, 44 } },
};
const struct sprite_frame_2box gSpriteBank55Frame042 = {
    SPRITE_FRAME(gSpriteBank55Frame042, SPRITE_TILES_BANK55 + 0x17500),
    { { -30, -41, 67, 72 }, { -16, -32, 21, 59 } },
};
const struct sprite_frame_2box gSpriteBank55Frame043 = {
    SPRITE_FRAME(gSpriteBank55Frame043, SPRITE_TILES_BANK55 + 0x17da0),
    { { -25, -45, 62, 78 }, { -18, -30, 27, 58 } },
};
const struct sprite_frame_2box gSpriteBank55Frame044 = {
    SPRITE_FRAME(gSpriteBank55Frame044, SPRITE_TILES_BANK55 + 0x18620),
    { { -23, -53, 58, 87 }, { -17, -31, 30, 55 } },
};
const struct sprite_frame_2box gSpriteBank55Frame045 = {
    SPRITE_FRAME(gSpriteBank55Frame045, SPRITE_TILES_BANK55 + 0x18ee0),
    { { -22, -59, 56, 95 }, { -14, -31, 26, 59 } },
};
const struct sprite_frame_2box gSpriteBank55Frame046 = {
    SPRITE_FRAME(gSpriteBank55Frame046, SPRITE_TILES_BANK55 + 0x19800),
    { { -20, -63, 55, 102 }, { -17, -37, 30, 67 } },
};
const struct sprite_frame_2box gSpriteBank55Frame047 = {
    SPRITE_FRAME(gSpriteBank55Frame047, SPRITE_TILES_BANK55 + 0x1a1c0),
    { { -31, -13, 81, 52 }, { -36, -2, 81, 39 } },
};
const struct sprite_frame_2box gSpriteBank55Frame048 = {
    SPRITE_FRAME(gSpriteBank55Frame048, SPRITE_TILES_BANK55 + 0x1aa60),
    { { -33, -11, 67, 44 }, { -32, 11, 70, 25 } },
};
const struct sprite_frame_2box gSpriteBank55Frame049 = {
    SPRITE_FRAME(gSpriteBank55Frame049, SPRITE_TILES_BANK55 + 0x17da0),
    { { -25, -45, 62, 78 }, { -18, -29, 27, 59 } },
};
const struct sprite_frame_2box gSpriteBank55Frame050 = {
    SPRITE_FRAME(gSpriteBank55Frame050, SPRITE_TILES_BANK55 + 0x18620),
    { { -23, -53, 58, 87 }, { -16, -34, 31, 64 } },
};
const struct sprite_frame_2box gSpriteBank55Frame051 = {
    SPRITE_FRAME(gSpriteBank55Frame051, SPRITE_TILES_BANK55 + 0x18ee0),
    { { -22, -59, 56, 95 }, { -13, -35, 27, 66 } },
};
const struct sprite_frame_2box gSpriteBank55Frame052 = {
    SPRITE_FRAME(gSpriteBank55Frame052, SPRITE_TILES_BANK55 + 0x19800),
    { { -20, -63, 55, 102 }, { -13, -42, 29, 74 } },
};
const struct sprite_frame_2box gSpriteBank55Frame053 = {
    SPRITE_FRAME(gSpriteBank55Frame053, SPRITE_TILES_BANK55 + 0x1a1c0),
    { { -31, -13, 81, 52 }, { -22, -5, 58, 42 } },
};
const struct sprite_frame_2box gSpriteBank55Frame054 = {
    SPRITE_FRAME(gSpriteBank55Frame054, SPRITE_TILES_BANK55 + 0x1b000),
    { { -28, 0, 67, 39 }, { -26, 15, 63, 21 } },
};
const struct sprite_frame_2box gSpriteBank55Frame055 = {
    SPRITE_FRAME(gSpriteBank55Frame055, SPRITE_TILES_BANK55 + 0x1b000),
    { { -28, 0, 67, 39 }, { -25, 10, 58, 26 } },
};
const struct sprite_frame_2box gSpriteBank55Frame056 = {
    SPRITE_FRAME(gSpriteBank55Frame056, SPRITE_TILES_BANK55 + 0x1b000),
    { { -28, 0, 67, 39 }, { -19, 18, 52, 18 } },
};
const struct sprite_frame_2box gSpriteBank55Frame057 = {
    SPRITE_FRAME(gSpriteBank55Frame057, SPRITE_TILES_BANK55 + 0x1b000),
    { { -28, 0, 67, 39 }, { -22, 10, 57, 25 } },
};
const struct sprite_frame_2box gSpriteBank55Frame058 = {
    SPRITE_FRAME(gSpriteBank55Frame058, SPRITE_TILES_BANK55 + 0x1b000),
    { { -28, 0, 67, 39 }, { -27, 8, 64, 25 } },
};
const struct sprite_frame_2box gSpriteBank55Frame059 = {
    SPRITE_FRAME(gSpriteBank55Frame059, SPRITE_TILES_BANK55 + 0x0ade0),
    { { -26, -22, 57, 67 }, { -13, -12, 26, 51 } },
};
const struct sprite_frame_2box gSpriteBank55Frame060 = {
    SPRITE_FRAME(gSpriteBank55Frame060, SPRITE_TILES_BANK55 + 0x0b600),
    { { -27, -47, 48, 87 }, { -13, -12, 26, 51 } },
};
const struct sprite_frame_2box gSpriteBank55Frame061 = {
    SPRITE_FRAME(gSpriteBank55Frame061, SPRITE_TILES_BANK55 + 0x0bf00),
    { { -33, -61, 44, 98 }, { -13, -12, 26, 51 } },
};
const struct sprite_frame_2box gSpriteBank55Frame062 = {
    SPRITE_FRAME(gSpriteBank55Frame062, SPRITE_TILES_BANK55 + 0x0c940),
    { { -35, -61, 44, 96 }, { -13, -12, 26, 51 } },
};
const struct sprite_frame_1box gSpriteBank55Frame063 = {
    SPRITE_FRAME(gSpriteBank55Frame063, SPRITE_TILES_BANK55 + 0x1b540),
    { { -24, -14, 49, 28 } },
};
const struct sprite_frame_1box gSpriteBank55Frame064 = {
    SPRITE_FRAME(gSpriteBank55Frame064, SPRITE_TILES_BANK55 + 0x1b940),
    { { -21, -14, 50, 29 } },
};
const struct sprite_frame_1box gSpriteBank55Frame065 = {
    SPRITE_FRAME(gSpriteBank55Frame065, SPRITE_TILES_BANK55 + 0x1bd40),
    { { -23, -12, 51, 34 } },
};
const struct sprite_frame_1box gSpriteBank55Frame066 = {
    SPRITE_FRAME(gSpriteBank55Frame066, SPRITE_TILES_BANK55 + 0x1c1c0),
    { { -24, -7, 49, 33 } },
};
const struct sprite_frame_1box gSpriteBank55Frame067 = {
    SPRITE_FRAME(gSpriteBank55Frame067, SPRITE_TILES_BANK55 + 0x1c5e0),
    { { -26, 1, 48, 28 } },
};
const struct sprite_frame_1box gSpriteBank55Frame068 = {
    SPRITE_FRAME(gSpriteBank55Frame068, SPRITE_TILES_BANK55 + 0x1c9e0),
    { { -25, 6, 44, 25 } },
};
const struct sprite_frame_1box gSpriteBank55Frame069 = {
    SPRITE_FRAME(gSpriteBank55Frame069, SPRITE_TILES_BANK55 + 0x1cc60),
    { { -24, 7, 45, 25 } },
};
const struct sprite_frame_1box gSpriteBank55Frame070 = {
    SPRITE_FRAME(gSpriteBank55Frame070, SPRITE_TILES_BANK55 + 0x1cec0),
    { { -25, 12, 45, 20 } },
};
const struct sprite_frame_1box gSpriteBank55Frame071 = {
    SPRITE_FRAME(gSpriteBank55Frame071, SPRITE_TILES_BANK55 + 0x1d0a0),
    { { -27, 13, 52, 21 } },
};
const struct sprite_frame_1box gSpriteBank55Frame072 = {
    SPRITE_FRAME(gSpriteBank55Frame072, SPRITE_TILES_BANK55 + 0x1d2a0),
    { { -27, 15, 52, 20 } },
};
const struct sprite_frame_1box gSpriteBank55Frame073 = {
    SPRITE_FRAME(gSpriteBank55Frame073, SPRITE_TILES_BANK55 + 0x1d480),
    { { -28, 14, 50, 20 } },
};
const struct sprite_frame_1box gSpriteBank55Frame074 = {
    SPRITE_FRAME(gSpriteBank55Frame074, SPRITE_TILES_BANK55 + 0x1d660),
    { { -27, 15, 51, 19 } },
};
const struct sprite_frame_1box gSpriteBank55Frame075 = {
    SPRITE_FRAME(gSpriteBank55Frame075, SPRITE_TILES_BANK55 + 0x1d840),
    { { -25, 18, 46, 19 } },
};
const struct sprite_frame_1box gSpriteBank55Frame076 = {
    SPRITE_FRAME(gSpriteBank55Frame076, SPRITE_TILES_BANK55 + 0x1d9e0),
    { { -23, 23, 47, 12 } },
};
const struct sprite_frame_1box gSpriteBank55Frame077 = {
    SPRITE_FRAME(gSpriteBank55Frame077, SPRITE_TILES_BANK55 + 0x1daa0),
    { { -18, 27, 38, 9 } },
};
const struct sprite_frame_1box gSpriteBank55Frame078 = {
    SPRITE_FRAME(gSpriteBank55Frame078, SPRITE_TILES_BANK55 + 0x1dbc0),
    { { -23, -54, 56, 87 } },
};
const struct sprite_frame_1box gSpriteBank55Frame079 = {
    SPRITE_FRAME(gSpriteBank55Frame079, SPRITE_TILES_BANK55 + 0x1e640),
    { { -23, -54, 56, 87 } },
};
const struct sprite_frame_1box gSpriteBank55Frame080 = {
    SPRITE_FRAME(gSpriteBank55Frame080, SPRITE_TILES_BANK55 + 0x1f0c0),
    { { -25, -54, 58, 87 } },
};
const struct sprite_frame_1box gSpriteBank55Frame081 = {
    SPRITE_FRAME(gSpriteBank55Frame081, SPRITE_TILES_BANK55 + 0x1fb40),
    { { -27, -54, 60, 87 } },
};
const struct sprite_frame_1box gSpriteBank55Frame082 = {
    SPRITE_FRAME(gSpriteBank55Frame082, SPRITE_TILES_BANK55 + 0x205e0),
    { { -29, -54, 62, 88 } },
};
const struct sprite_frame_1box gSpriteBank55Frame083 = {
    SPRITE_FRAME(gSpriteBank55Frame083, SPRITE_TILES_BANK55 + 0x21160),
    { { -31, -54, 64, 88 } },
};
const struct sprite_frame_1box gSpriteBank55Frame084 = {
    SPRITE_FRAME(gSpriteBank55Frame084, SPRITE_TILES_BANK55 + 0x21de0),
    { { -32, -54, 65, 87 } },
};
const struct sprite_frame_1box gSpriteBank55Frame085 = {
    SPRITE_FRAME(gSpriteBank55Frame085, SPRITE_TILES_BANK55 + 0x22820),
    { { -33, -54, 66, 87 } },
};
const struct sprite_frame_1box gSpriteBank55Frame086 = {
    SPRITE_FRAME(gSpriteBank55Frame086, SPRITE_TILES_BANK55 + 0x23300),
    { { -33, -54, 66, 87 } },
};
const struct sprite_frame_1box gSpriteBank55Frame087 = {
    SPRITE_FRAME(gSpriteBank55Frame087, SPRITE_TILES_BANK55 + 0x23de0),
    { { -33, -54, 66, 87 } },
};
const struct sprite_frame_1box gSpriteBank55Frame088 = {
    SPRITE_FRAME(gSpriteBank55Frame088, SPRITE_TILES_BANK55 + 0x248c0),
    { { -33, -54, 66, 87 } },
};
const struct sprite_frame_1box gSpriteBank55Frame089 = {
    SPRITE_FRAME(gSpriteBank55Frame089, SPRITE_TILES_BANK55 + 0x253a0),
    { { -33, -54, 66, 87 } },
};
const struct sprite_frame_1box gSpriteBank55Frame090 = {
    SPRITE_FRAME(gSpriteBank55Frame090, SPRITE_TILES_BANK55 + 0x25e80),
    { { -33, -54, 66, 87 } },
};
const struct sprite_frame_1box gSpriteBank55Frame091 = {
    SPRITE_FRAME(gSpriteBank55Frame091, SPRITE_TILES_BANK55 + 0x26960),
    { { -32, -54, 65, 87 } },
};
const struct sprite_frame_1box gSpriteBank55Frame092 = {
    SPRITE_FRAME(gSpriteBank55Frame092, SPRITE_TILES_BANK55 + 0x27440),
    { { -30, -54, 63, 87 } },
};
const struct sprite_frame_1box gSpriteBank55Frame093 = {
    SPRITE_FRAME(gSpriteBank55Frame093, SPRITE_TILES_BANK55 + 0x27e80),
    { { -32, -47, 69, 70 } },
};
const struct sprite_frame_1box gSpriteBank55Frame094 = {
    SPRITE_FRAME(gSpriteBank55Frame094, SPRITE_TILES_BANK55 + 0x286e0),
    { { -34, -52, 74, 50 } },
};
const struct sprite_frame_1box gSpriteBank55Frame095 = {
    SPRITE_FRAME(gSpriteBank55Frame095, SPRITE_TILES_BANK55 + 0x28f60),
    { { -27, -52, 69, 47 } },
};
const struct sprite_frame_1box gSpriteBank55Frame096 = {
    SPRITE_FRAME(gSpriteBank55Frame096, SPRITE_TILES_BANK55 + 0x29540),
    { { -20, -49, 63, 50 } },
};
const struct sprite_frame_1box gSpriteBank55Frame097 = {
    SPRITE_FRAME(gSpriteBank55Frame097, SPRITE_TILES_BANK55 + 0x29d40),
    { { -35, -27, 66, 56 } },
};
const struct sprite_frame_1box gSpriteBank55Frame098 = {
    SPRITE_FRAME(gSpriteBank55Frame098, SPRITE_TILES_BANK55 + 0x2a560),
    { { -62, 5, 100, 33 } },
};
const struct sprite_frame_1box gSpriteBank55Frame099 = {
    SPRITE_FRAME(gSpriteBank55Frame099, SPRITE_TILES_BANK55 + 0x2abc0),
    { { -65, 4, 97, 31 } },
};
const struct sprite_frame_1box gSpriteBank55Frame100 = {
    SPRITE_FRAME(gSpriteBank55Frame100, SPRITE_TILES_BANK55 + 0x2b1e0),
    { { -68, -4, 95, 37 } },
};
const struct sprite_frame_1box gSpriteBank55Frame101 = {
    SPRITE_FRAME(gSpriteBank55Frame101, SPRITE_TILES_BANK55 + 0x2b860),
    { { -64, 2, 93, 34 } },
};
const struct sprite_frame_1box gSpriteBank55Frame102 = {
    SPRITE_FRAME(gSpriteBank55Frame102, SPRITE_TILES_BANK55 + 0x2be80),
    { { -68, 7, 100, 30 } },
};
const struct sprite_frame_1box gSpriteBank55Frame103 = {
    SPRITE_FRAME(gSpriteBank55Frame103, SPRITE_TILES_BANK55 + 0x2c4c0),
    { { -67, 8, 98, 30 } },
};
const struct sprite_frame_1box gSpriteBank55Frame104 = {
    SPRITE_FRAME(gSpriteBank55Frame104, SPRITE_TILES_BANK55 + 0x2cb00),
    { { -67, -7, 98, 43 } },
};
const struct sprite_frame_1box gSpriteBank55Frame105 = {
    SPRITE_FRAME(gSpriteBank55Frame105, SPRITE_TILES_BANK55 + 0x2d3c0),
    { { -68, -12, 102, 45 } },
};
const struct sprite_frame_1box gSpriteBank55Frame106 = {
    SPRITE_FRAME(gSpriteBank55Frame106, SPRITE_TILES_BANK55 + 0x2dce0),
    { { -67, -13, 101, 43 } },
};
const struct sprite_frame_1box gSpriteBank55Frame107 = {
    SPRITE_FRAME(gSpriteBank55Frame107, SPRITE_TILES_BANK55 + 0x2e600),
    { { -68, -9, 102, 39 } },
};
const struct sprite_frame_1box gSpriteBank55Frame108 = {
    SPRITE_FRAME(gSpriteBank55Frame108, SPRITE_TILES_BANK55 + 0x2ede0),
    { { -26, -50, 57, 83 } },
};
const struct sprite_frame_1box gSpriteBank55Frame109 = {
    SPRITE_FRAME(gSpriteBank55Frame109, SPRITE_TILES_BANK55 + 0x2f680),
    { { -27, -40, 75, 65 } },
};
const struct sprite_frame_1box gSpriteBank55Frame110 = {
    SPRITE_FRAME(gSpriteBank55Frame110, SPRITE_TILES_BANK55 + 0x2fee0),
    { { -46, -26, 72, 57 } },
};
const struct sprite_frame_1box gSpriteBank55Frame111 = {
    SPRITE_FRAME(gSpriteBank55Frame111, SPRITE_TILES_BANK55 + 0x307e0),
    { { -52, -22, 83, 53 } },
};
const struct sprite_frame_1box gSpriteBank55Frame112 = {
    SPRITE_FRAME(gSpriteBank55Frame112, SPRITE_TILES_BANK55 + 0x31180),
    { { -57, -19, 96, 50 } },
};
const struct sprite_frame_1box gSpriteBank55Frame113 = {
    SPRITE_FRAME(gSpriteBank55Frame113, SPRITE_TILES_BANK55 + 0x31da0),
    { { -58, -19, 105, 50 } },
};
const struct sprite_frame_1box gSpriteBank55Frame114 = {
    SPRITE_FRAME(gSpriteBank55Frame114, SPRITE_TILES_BANK55 + 0x329e0),
    { { -57, -20, 110, 51 } },
};
const struct sprite_frame_1box gSpriteBank55Frame115 = {
    SPRITE_FRAME(gSpriteBank55Frame115, SPRITE_TILES_BANK55 + 0x33620),
    { { -55, -20, 108, 51 } },
};
const struct sprite_frame_1box gSpriteBank55Frame116 = {
    SPRITE_FRAME(gSpriteBank55Frame116, SPRITE_TILES_BANK55 + 0x34260),
    { { -60, -42, 89, 76 } },
};
const struct sprite_frame_1box gSpriteBank55Frame117 = {
    SPRITE_FRAME(gSpriteBank55Frame117, SPRITE_TILES_BANK55 + 0x34f80),
    { { -42, -62, 53, 89 } },
};
const struct sprite_frame_1box gSpriteBank55Frame118 = {
    SPRITE_FRAME(gSpriteBank55Frame118, SPRITE_TILES_BANK55 + 0x35a60),
    { { -13, -45, 26, 91 } },
};
const struct sprite_frame_1box gSpriteBank55Frame119 = {
    SPRITE_FRAME(gSpriteBank55Frame119, SPRITE_TILES_BANK55 + 0x35f00),
    { { -30, -52, 51, 88 } },
};
const struct sprite_frame_1box gSpriteBank55Frame120 = {
    SPRITE_FRAME(gSpriteBank55Frame120, SPRITE_TILES_BANK55 + 0x36760),
    { { -40, -56, 56, 95 } },
};
const struct sprite_frame_1box gSpriteBank55Frame121 = {
    SPRITE_FRAME(gSpriteBank55Frame121, SPRITE_TILES_BANK55 + 0x36cc0),
    { { -38, -65, 51, 101 } },
};
const struct sprite_frame_1box gSpriteBank55Frame122 = {
    SPRITE_FRAME(gSpriteBank55Frame122, SPRITE_TILES_BANK55 + 0x371a0),
    { { -44, -58, 65, 87 } },
};
const struct sprite_frame_1box gSpriteBank55Frame123 = {
    SPRITE_FRAME(gSpriteBank55Frame123, SPRITE_TILES_BANK55 + 0x37ae0),
    { { -52, -47, 76, 68 } },
};
const struct sprite_frame_1box gSpriteBank55Frame124 = {
    SPRITE_FRAME(gSpriteBank55Frame124, SPRITE_TILES_BANK55 + 0x383a0),
    { { -60, -38, 84, 63 } },
};
const struct sprite_frame_1box gSpriteBank55Frame125 = {
    SPRITE_FRAME(gSpriteBank55Frame125, SPRITE_TILES_BANK55 + 0x38c60),
    { { -75, -26, 104, 64 } },
};
const struct sprite_frame_1box gSpriteBank55Frame126 = {
    SPRITE_FRAME(gSpriteBank55Frame126, SPRITE_TILES_BANK55 + 0x391a0),
    { { -84, -9, 117, 23 } },
};
const struct sprite_frame_1box gSpriteBank55Frame127 = {
    SPRITE_FRAME(gSpriteBank55Frame127, SPRITE_TILES_BANK55 + 0x392c0),
    { { -92, 10, 126, 9 } },
};
const struct sprite_frame_1box gSpriteBank55Frame128 = {
    SPRITE_FRAME(gSpriteBank55Frame128, SPRITE_TILES_BANK55 + 0x393c0),
    { { -33, 19, 65, 12 } },
};
const struct sprite_frame_1box gSpriteBank55Frame129 = {
    SPRITE_FRAME(gSpriteBank55Frame129, SPRITE_TILES_BANK55 + 0x39460),
    { { 13, 26, 17, 3 } },
};

const struct sprite_piece_pos gSpriteBank55Frame000Pos[6] = { { -37, -63 }, { 27, -41 }, { 43, -32 }, { -21, 1 }, { 11, 5 }, { -18, 33 } };
const struct sprite_piece_pos gSpriteBank55Frame001Pos[5] = { { -68, -64 }, { -4, -67 }, { -39, -3 }, { -9, 29 }, { -7, 29 } };
const struct sprite_piece_pos gSpriteBank55Frame002Pos[3] = { { -67, -68 }, { -48, -4 }, { -13, 28 } };
const struct sprite_piece_pos gSpriteBank55Frame003Pos[3] = { { -66, -67 }, { -50, -3 }, { -19, 29 } };
const struct sprite_piece_pos gSpriteBank55Frame004Pos[4] = { { -65, -65 }, { -51, -1 }, { -27, 31 }, { -11, 31 } };
const struct sprite_piece_pos gSpriteBank55Frame005Pos[3] = { { -65, -62 }, { -52, 2 }, { -34, 34 } };
const struct sprite_piece_pos gSpriteBank55Frame006Pos[3] = { { -68, -54 }, { -4, -56 }, { -51, 8 } };
const struct sprite_piece_pos gSpriteBank55Frame007Pos[4] = { { -66, -38 }, { -2, -43 }, { -42, 21 }, { -10, 21 } };
const struct sprite_piece_pos gSpriteBank55Frame008Pos[4] = { { -53, -36 }, { 11, -29 }, { 27, -27 }, { -20, 28 } };
const struct sprite_piece_pos gSpriteBank55Frame009Pos[2] = { { -46, -31 }, { 18, -12 } };
const struct sprite_piece_pos gSpriteBank55Frame010Pos[2] = { { -48, -28 }, { 16, 10 } };
const struct sprite_piece_pos gSpriteBank55Frame011Pos[1] = { { -50, -25 } };
const struct sprite_piece_pos gSpriteBank55Frame012Pos[1] = { { -50, -25 } };
const struct sprite_piece_pos gSpriteBank55Frame013Pos[2] = { { -50, -23 }, { 14, 16 } };
const struct sprite_piece_pos gSpriteBank55Frame014Pos[2] = { { -48, -19 }, { 16, 15 } };
const struct sprite_piece_pos gSpriteBank55Frame015Pos[5] = { { -27, -12 }, { 19, -5 }, { -45, 20 }, { -13, 20 }, { -5, 20 } };
const struct sprite_piece_pos gSpriteBank55Frame016Pos[5] = { { -24, -6 }, { 21, -8 }, { -43, 24 }, { -11, 24 }, { -3, 24 } };
const struct sprite_piece_pos gSpriteBank55Frame017Pos[2] = { { -26, -22 }, { 0, 42 } };
const struct sprite_piece_pos gSpriteBank55Frame018Pos[4] = { { -27, -47 }, { -18, 17 }, { -2, 17 }, { -13, 33 } };
const struct sprite_piece_pos gSpriteBank55Frame019Pos[5] = { { -33, -60 }, { -1, -59 }, { -28, 3 }, { 4, 8 }, { -13, 35 } };
const struct sprite_piece_pos gSpriteBank55Frame020Pos[6] = { { -35, -61 }, { -3, -52 }, { -31, 3 }, { 1, 5 }, { 9, 15 }, { -13, 35 } };
const struct sprite_piece_pos gSpriteBank55Frame021Pos[5] = { { -37, -56 }, { -34, 8 }, { -2, 8 }, { -20, 24 }, { -12, 24 } };
const struct sprite_piece_pos gSpriteBank55Frame022Pos[3] = { { -37, -42 }, { 27, 6 }, { -23, 22 } };
const struct sprite_piece_pos gSpriteBank55Frame023Pos[5] = { { -38, -42 }, { 26, -24 }, { 42, -13 }, { -32, 22 }, { -16, 22 } };
const struct sprite_piece_pos gSpriteBank55Frame024Pos[5] = { { -42, -62 }, { 22, -50 }, { -40, 2 }, { -8, 2 }, { -30, 18 } };
const struct sprite_piece_pos gSpriteBank55Frame025Pos[6] = { { -51, -68 }, { 13, -70 }, { 29, -71 }, { -40, -7 }, { -8, -7 }, { -27, 9 } };
const struct sprite_piece_pos gSpriteBank55Frame026Pos[6] = { { -48, -70 }, { 16, -72 }, { 32, -72 }, { -40, -8 }, { -8, -8 }, { -24, 8 } };
const struct sprite_piece_pos gSpriteBank55Frame027Pos[5] = { { -32, -69 }, { 28, -49 }, { -36, -5 }, { -4, 0 }, { -4, 16 } };
const struct sprite_piece_pos gSpriteBank55Frame028Pos[6] = { { -30, -60 }, { 34, -12 }, { -26, 4 }, { 6, 4 }, { 38, 4 }, { 46, 11 } };
const struct sprite_piece_pos gSpriteBank55Frame029Pos[5] = { { -32, -54 }, { -33, 10 }, { 14, 10 }, { 31, 14 }, { 30, 26 } };
const struct sprite_piece_pos gSpriteBank55Frame030Pos[7] = { { -28, -54 }, { -18, 10 }, { 14, 10 }, { 30, 10 }, { -15, 26 }, { 31, 26 }, { 33, 26 } };
const struct sprite_piece_pos gSpriteBank55Frame031Pos[3] = { { -19, -54 }, { -26, 10 }, { -25, 42 } };
const struct sprite_piece_pos gSpriteBank55Frame032Pos[7] = { { -20, -54 }, { -18, 10 }, { 0, 10 }, { 32, 14 }, { -32, 26 }, { 0, 26 }, { 32, 26 } };
const struct sprite_piece_pos gSpriteBank55Frame033Pos[3] = { { -22, -54 }, { -31, 10 }, { 33, 14 } };
const struct sprite_piece_pos gSpriteBank55Frame034Pos[5] = { { -24, -54 }, { -24, 10 }, { 25, 10 }, { -25, 26 }, { 7, 26 } };
const struct sprite_piece_pos gSpriteBank55Frame035Pos[2] = { { -26, -22 }, { 0, 42 } };
const struct sprite_piece_pos gSpriteBank55Frame036Pos[4] = { { -27, -47 }, { -18, 17 }, { -2, 17 }, { -13, 33 } };
const struct sprite_piece_pos gSpriteBank55Frame037Pos[5] = { { -33, -60 }, { -1, -59 }, { -28, 3 }, { 4, 8 }, { -13, 35 } };
const struct sprite_piece_pos gSpriteBank55Frame038Pos[6] = { { -35, -61 }, { -3, -52 }, { -31, 3 }, { 1, 5 }, { 9, 15 }, { -13, 35 } };
const struct sprite_piece_pos gSpriteBank55Frame039Pos[5] = { { -37, -56 }, { -34, 8 }, { -2, 8 }, { -20, 24 }, { -12, 24 } };
const struct sprite_piece_pos gSpriteBank55Frame040Pos[2] = { { -38, -50 }, { -22, 14 } };
const struct sprite_piece_pos gSpriteBank55Frame041Pos[2] = { { -36, -47 }, { -22, 17 } };
const struct sprite_piece_pos gSpriteBank55Frame042Pos[3] = { { -30, -41 }, { 34, -12 }, { -18, 23 } };
const struct sprite_piece_pos gSpriteBank55Frame043Pos[2] = { { -25, -45 }, { -16, 19 } };
const struct sprite_piece_pos gSpriteBank55Frame044Pos[3] = { { -23, -53 }, { -15, 11 }, { -11, 27 } };
const struct sprite_piece_pos gSpriteBank55Frame045Pos[3] = { { -22, -59 }, { -14, 5 }, { 2, 29 } };
const struct sprite_piece_pos gSpriteBank55Frame046Pos[4] = { { -20, -63 }, { -16, 1 }, { 0, 1 }, { -8, 33 } };
const struct sprite_piece_pos gSpriteBank55Frame047Pos[3] = { { -31, -13 }, { 33, -8 }, { 49, -6 } };
const struct sprite_piece_pos gSpriteBank55Frame048Pos[5] = { { -17, -11 }, { 31, -6 }, { -33, 21 }, { -1, 21 }, { 7, 27 } };
const struct sprite_piece_pos gSpriteBank55Frame049Pos[2] = { { -25, -45 }, { -16, 19 } };
const struct sprite_piece_pos gSpriteBank55Frame050Pos[3] = { { -23, -53 }, { -15, 11 }, { -11, 27 } };
const struct sprite_piece_pos gSpriteBank55Frame051Pos[3] = { { -22, -59 }, { -14, 5 }, { 2, 29 } };
const struct sprite_piece_pos gSpriteBank55Frame052Pos[4] = { { -20, -63 }, { -16, 1 }, { 0, 1 }, { -8, 33 } };
const struct sprite_piece_pos gSpriteBank55Frame053Pos[3] = { { -31, -13 }, { 33, -8 }, { 49, -6 } };
const struct sprite_piece_pos gSpriteBank55Frame054Pos[4] = { { -28, 0 }, { 36, 17 }, { -24, 32 }, { 8, 32 } };
const struct sprite_piece_pos gSpriteBank55Frame055Pos[4] = { { -28, 0 }, { 36, 17 }, { -24, 32 }, { 8, 32 } };
const struct sprite_piece_pos gSpriteBank55Frame056Pos[4] = { { -28, 0 }, { 36, 17 }, { -24, 32 }, { 8, 32 } };
const struct sprite_piece_pos gSpriteBank55Frame057Pos[4] = { { -28, 0 }, { 36, 17 }, { -24, 32 }, { 8, 32 } };
const struct sprite_piece_pos gSpriteBank55Frame058Pos[4] = { { -28, 0 }, { 36, 17 }, { -24, 32 }, { 8, 32 } };
const struct sprite_piece_pos gSpriteBank55Frame059Pos[2] = { { -26, -22 }, { 0, 42 } };
const struct sprite_piece_pos gSpriteBank55Frame060Pos[4] = { { -27, -47 }, { -18, 17 }, { -2, 17 }, { -13, 33 } };
const struct sprite_piece_pos gSpriteBank55Frame061Pos[5] = { { -33, -60 }, { -1, -59 }, { -28, 3 }, { 4, 8 }, { -13, 35 } };
const struct sprite_piece_pos gSpriteBank55Frame062Pos[6] = { { -35, -61 }, { -3, -52 }, { -31, 3 }, { 1, 5 }, { 9, 15 }, { -13, 35 } };
const struct sprite_piece_pos gSpriteBank55Frame063Pos[1] = { { -24, -14 } };
const struct sprite_piece_pos gSpriteBank55Frame064Pos[1] = { { -21, -14 } };
const struct sprite_piece_pos gSpriteBank55Frame065Pos[2] = { { -23, -12 }, { -23, 20 } };
const struct sprite_piece_pos gSpriteBank55Frame066Pos[2] = { { -24, -7 }, { -22, 25 } };
const struct sprite_piece_pos gSpriteBank55Frame067Pos[1] = { { -26, 1 } };
const struct sprite_piece_pos gSpriteBank55Frame068Pos[3] = { { -25, 6 }, { 7, 8 }, { 15, 11 } };
const struct sprite_piece_pos gSpriteBank55Frame069Pos[3] = { { -24, 10 }, { 8, 11 }, { 17, 7 } };
const struct sprite_piece_pos gSpriteBank55Frame070Pos[4] = { { -25, 15 }, { 7, 12 }, { -18, 28 }, { -2, 28 } };
const struct sprite_piece_pos gSpriteBank55Frame071Pos[5] = { { -27, 20 }, { 5, 13 }, { 21, 26 }, { -13, 29 }, { 3, 29 } };
const struct sprite_piece_pos gSpriteBank55Frame072Pos[4] = { { -27, 16 }, { 5, 15 }, { 21, 21 }, { -11, 31 } };
const struct sprite_piece_pos gSpriteBank55Frame073Pos[4] = { { -28, 14 }, { 4, 17 }, { 20, 17 }, { -8, 30 } };
const struct sprite_piece_pos gSpriteBank55Frame074Pos[5] = { { -27, 15 }, { 5, 16 }, { 21, 19 }, { -9, 31 }, { -1, 31 } };
const struct sprite_piece_pos gSpriteBank55Frame075Pos[3] = { { -25, 25 }, { 7, 18 }, { -20, 34 } };
const struct sprite_piece_pos gSpriteBank55Frame076Pos[2] = { { -23, 28 }, { 9, 23 } };
const struct sprite_piece_pos gSpriteBank55Frame077Pos[2] = { { -18, 27 }, { 14, 27 } };
const struct sprite_piece_pos gSpriteBank55Frame078Pos[5] = { { -23, -54 }, { -22, 10 }, { 25, 10 }, { -22, 26 }, { 10, 26 } };
const struct sprite_piece_pos gSpriteBank55Frame079Pos[5] = { { -22, -54 }, { -23, 10 }, { 25, 10 }, { -23, 26 }, { 9, 26 } };
const struct sprite_piece_pos gSpriteBank55Frame080Pos[5] = { { -20, -54 }, { -24, 10 }, { 25, 10 }, { -24, 26 }, { 8, 26 } };
const struct sprite_piece_pos gSpriteBank55Frame081Pos[5] = { { -19, -54 }, { -26, 10 }, { 11, 10 }, { -14, 26 }, { 31, 26 } };
const struct sprite_piece_pos gSpriteBank55Frame082Pos[2] = { { -20, -54 }, { -29, 10 } };
const struct sprite_piece_pos gSpriteBank55Frame083Pos[3] = { { -21, -54 }, { -31, 10 }, { 33, 14 } };
const struct sprite_piece_pos gSpriteBank55Frame084Pos[7] = { { -23, -54 }, { -32, 10 }, { 25, 10 }, { 32, 14 }, { -15, 26 }, { 31, 26 }, { 33, 26 } };
const struct sprite_piece_pos gSpriteBank55Frame085Pos[6] = { { -25, -54 }, { -33, 10 }, { -1, 10 }, { 31, 14 }, { -14, 26 }, { 31, 26 } };
const struct sprite_piece_pos gSpriteBank55Frame086Pos[6] = { { -25, -54 }, { -33, 10 }, { -1, 10 }, { 31, 14 }, { -14, 26 }, { 31, 26 } };
const struct sprite_piece_pos gSpriteBank55Frame087Pos[6] = { { -25, -54 }, { -33, 10 }, { -1, 10 }, { 31, 14 }, { -14, 26 }, { 31, 26 } };
const struct sprite_piece_pos gSpriteBank55Frame088Pos[6] = { { -25, -54 }, { -33, 10 }, { -1, 10 }, { 31, 14 }, { -14, 26 }, { 31, 26 } };
const struct sprite_piece_pos gSpriteBank55Frame089Pos[6] = { { -25, -54 }, { -33, 10 }, { -1, 10 }, { 31, 14 }, { -14, 26 }, { 31, 26 } };
const struct sprite_piece_pos gSpriteBank55Frame090Pos[6] = { { -25, -54 }, { -33, 10 }, { -1, 10 }, { 31, 14 }, { -14, 26 }, { 31, 26 } };
const struct sprite_piece_pos gSpriteBank55Frame091Pos[6] = { { -24, -54 }, { -32, 10 }, { 0, 10 }, { 32, 14 }, { -14, 26 }, { 31, 26 } };
const struct sprite_piece_pos gSpriteBank55Frame092Pos[6] = { { -23, -54 }, { -30, 10 }, { 25, 10 }, { -15, 26 }, { 31, 26 }, { 33, 26 } };
const struct sprite_piece_pos gSpriteBank55Frame093Pos[3] = { { -32, -47 }, { 32, -9 }, { -9, 17 } };
const struct sprite_piece_pos gSpriteBank55Frame094Pos[2] = { { -34, -52 }, { 30, -41 } };
const struct sprite_piece_pos gSpriteBank55Frame095Pos[5] = { { -27, -52 }, { 37, -41 }, { -17, -20 }, { 15, -20 }, { 31, -15 } };
const struct sprite_piece_pos gSpriteBank55Frame096Pos[1] = { { -20, -49 } };
const struct sprite_piece_pos gSpriteBank55Frame097Pos[2] = { { -35, -27 }, { 29, 5 } };
const struct sprite_piece_pos gSpriteBank55Frame098Pos[4] = { { -62, 5 }, { 2, 12 }, { 34, 21 }, { -27, 37 } };
const struct sprite_piece_pos gSpriteBank55Frame099Pos[3] = { { -65, 4 }, { -1, 8 }, { 31, 13 } };
const struct sprite_piece_pos gSpriteBank55Frame100Pos[3] = { { -68, -4 }, { -4, 2 }, { -36, 28 } };
const struct sprite_piece_pos gSpriteBank55Frame101Pos[3] = { { -64, 2 }, { 0, 12 }, { -35, 34 } };
const struct sprite_piece_pos gSpriteBank55Frame102Pos[3] = { { -68, 7 }, { -4, 12 }, { 28, 20 } };
const struct sprite_piece_pos gSpriteBank55Frame103Pos[3] = { { -67, 8 }, { -3, 9 }, { 29, 8 } };
const struct sprite_piece_pos gSpriteBank55Frame104Pos[7] = { { -55, -1 }, { -3, -7 }, { 29, 22 }, { -67, 25 }, { -35, 25 }, { -3, 25 }, { 29, 25 } };
const struct sprite_piece_pos gSpriteBank55Frame105Pos[6] = { { -56, -7 }, { -4, -12 }, { -68, 20 }, { -36, 20 }, { -4, 20 }, { 28, 23 } };
const struct sprite_piece_pos gSpriteBank55Frame106Pos[6] = { { -56, -3 }, { -3, -13 }, { -67, 19 }, { -35, 19 }, { -3, 19 }, { 29, 25 } };
const struct sprite_piece_pos gSpriteBank55Frame107Pos[7] = { { -57, -2 }, { -4, -9 }, { 28, -5 }, { -68, 23 }, { -36, 23 }, { -4, 23 }, { 28, 26 } };
const struct sprite_piece_pos gSpriteBank55Frame108Pos[3] = { { -26, -50 }, { 9, 14 }, { 9, 30 } };
const struct sprite_piece_pos gSpriteBank55Frame109Pos[3] = { { -27, -40 }, { 37, -8 }, { -18, 24 } };
const struct sprite_piece_pos gSpriteBank55Frame110Pos[3] = { { -46, -26 }, { 18, -15 }, { 18, 17 } };
const struct sprite_piece_pos gSpriteBank55Frame111Pos[4] = { { -52, -22 }, { 12, -18 }, { 12, 14 }, { 28, 24 } };
const struct sprite_piece_pos gSpriteBank55Frame112Pos[3] = { { -57, -19 }, { 7, -15 }, { 39, 30 } };
const struct sprite_piece_pos gSpriteBank55Frame113Pos[3] = { { -58, -19 }, { 6, -11 }, { 38, 27 } };
const struct sprite_piece_pos gSpriteBank55Frame114Pos[3] = { { -57, -20 }, { 7, -9 }, { 39, 23 } };
const struct sprite_piece_pos gSpriteBank55Frame115Pos[3] = { { -55, -20 }, { 9, -9 }, { 41, 26 } };
const struct sprite_piece_pos gSpriteBank55Frame116Pos[4] = { { -60, -39 }, { 4, -42 }, { -25, 22 }, { 23, 22 } };
const struct sprite_piece_pos gSpriteBank55Frame117Pos[5] = { { -39, -62 }, { -42, 2 }, { -10, 2 }, { -2, 2 }, { -1, 18 } };
const struct sprite_piece_pos gSpriteBank55Frame118Pos[3] = { { -13, -45 }, { 4, 19 }, { 12, 21 } };
const struct sprite_piece_pos gSpriteBank55Frame119Pos[3] = { { -30, -52 }, { 5, 20 }, { 9, 36 } };
const struct sprite_piece_pos gSpriteBank55Frame120Pos[4] = { { -40, -56 }, { 0, 16 }, { -8, 32 }, { 8, 32 } };
const struct sprite_piece_pos gSpriteBank55Frame121Pos[5] = { { -38, -65 }, { 1, -1 }, { 9, 26 }, { 4, 31 }, { 12, 31 } };
const struct sprite_piece_pos gSpriteBank55Frame122Pos[4] = { { -44, -58 }, { -3, 6 }, { 10, 22 }, { 18, 25 } };
const struct sprite_piece_pos gSpriteBank55Frame123Pos[4] = { { -52, -47 }, { 12, 6 }, { 8, 17 }, { 16, 17 } };
const struct sprite_piece_pos gSpriteBank55Frame124Pos[4] = { { -60, -38 }, { 7, 2 }, { 20, 9 }, { 4, 18 } };
const struct sprite_piece_pos gSpriteBank55Frame125Pos[4] = { { -75, -26 }, { 10, 7 }, { 21, 9 }, { 10, 38 } };
const struct sprite_piece_pos gSpriteBank55Frame126Pos[4] = { { -84, -9 }, { -27, -7 }, { -24, 7 }, { 14, 10 } };
const struct sprite_piece_pos gSpriteBank55Frame127Pos[4] = { { -92, 11 }, { -30, 10 }, { -28, 10 }, { 16, 12 } };
const struct sprite_piece_pos gSpriteBank55Frame128Pos[3] = { { -33, 21 }, { 16, 19 }, { 31, 19 } };
const struct sprite_piece_pos gSpriteBank55Frame129Pos[2] = { { 13, 26 }, { 29, 28 } };

const u8 gSpriteBank55Frame000Pieces[6] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame001Pieces[5] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 7), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame002Pieces[3] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 7), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame003Pieces[3] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 7), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank55Frame004Pieces[4] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 7), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame005Pieces[3] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 7), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank55Frame006Pieces[3] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 7) };
const u8 gSpriteBank55Frame007Pieces[4] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank55Frame008Pieces[4] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank55Frame009Pieces[2] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank55Frame010Pieces[2] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame011Pieces[1] = { SPRITE_PIECE(3, 3) };
const u8 gSpriteBank55Frame012Pieces[1] = { SPRITE_PIECE(3, 3) };
const u8 gSpriteBank55Frame013Pieces[2] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame014Pieces[2] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank55Frame015Pieces[5] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame016Pieces[5] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame017Pieces[2] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame018Pieces[4] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank55Frame019Pieces[5] = { SPRITE_PIECE(3, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame020Pieces[6] = { SPRITE_PIECE(3, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame021Pieces[5] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame022Pieces[3] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank55Frame023Pieces[5] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame024Pieces[5] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame025Pieces[6] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame026Pieces[6] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame027Pieces[5] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame028Pieces[6] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame029Pieces[5] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame030Pieces[7] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame031Pieces[3] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 7), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame032Pieces[7] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame033Pieces[3] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 7), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank55Frame034Pieces[5] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank55Frame035Pieces[2] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame036Pieces[4] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank55Frame037Pieces[5] = { SPRITE_PIECE(3, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame038Pieces[6] = { SPRITE_PIECE(3, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame039Pieces[5] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame040Pieces[2] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank55Frame041Pieces[2] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank55Frame042Pieces[3] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank55Frame043Pieces[2] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank55Frame044Pieces[3] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank55Frame045Pieces[3] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame046Pieces[4] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank55Frame047Pieces[3] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame048Pieces[5] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame049Pieces[2] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank55Frame050Pieces[3] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank55Frame051Pieces[3] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame052Pieces[4] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank55Frame053Pieces[3] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame054Pieces[4] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank55Frame055Pieces[4] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank55Frame056Pieces[4] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank55Frame057Pieces[4] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank55Frame058Pieces[4] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank55Frame059Pieces[2] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame060Pieces[4] = { SPRITE_PIECE(3, 3), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank55Frame061Pieces[5] = { SPRITE_PIECE(3, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame062Pieces[6] = { SPRITE_PIECE(3, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame063Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank55Frame064Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank55Frame065Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank55Frame066Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame067Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank55Frame068Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank55Frame069Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame070Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame071Pieces[5] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame072Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank55Frame073Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank55Frame074Pieces[5] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame075Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame076Pieces[2] = { SPRITE_PIECE(2, 5), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank55Frame077Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame078Pieces[5] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank55Frame079Pieces[5] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank55Frame080Pieces[5] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank55Frame081Pieces[5] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame082Pieces[2] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 7) };
const u8 gSpriteBank55Frame083Pieces[3] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 7), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank55Frame084Pieces[7] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame085Pieces[6] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame086Pieces[6] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame087Pieces[6] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame088Pieces[6] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame089Pieces[6] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame090Pieces[6] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame091Pieces[6] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame092Pieces[6] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame093Pieces[3] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame094Pieces[2] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank55Frame095Pieces[5] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame096Pieces[1] = { SPRITE_PIECE(2, 3) };
const u8 gSpriteBank55Frame097Pieces[2] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame098Pieces[4] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame099Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame100Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank55Frame101Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame102Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank55Frame103Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank55Frame104Pieces[7] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame105Pieces[6] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame106Pieces[6] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame107Pieces[7] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame108Pieces[3] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame109Pieces[3] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame110Pieces[3] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank55Frame111Pieces[4] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame112Pieces[3] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame113Pieces[3] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank55Frame114Pieces[3] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank55Frame115Pieces[3] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank55Frame116Pieces[4] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame117Pieces[5] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame118Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame119Pieces[3] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame120Pieces[4] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame121Pieces[5] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame122Pieces[4] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame123Pieces[4] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame124Pieces[4] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame125Pieces[4] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame126Pieces[4] = { SPRITE_PIECE(2, 4), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank55Frame127Pieces[4] = { SPRITE_PIECE(2, 4), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank55Frame128Pieces[3] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank55Frame129Pieces[2] = { SPRITE_PIECE(2, 4), SPRITE_PIECE(0, 0) };
