#include "gba/types.h"
#include "sprite_bank.h"

/*
 * ROM 0x084b0ae0-0x084b414c: sprite banks 10-21 of the sprite-bank
 * animation system (gSpriteBankTable, include/sprite_bank.h). Linked in
 * ROM order between data/data.s sections by ldscript.txt - see
 * docs/data.md and docs/data_map.md ("gSpriteBankTable").
 *
 * Extracted once from baserom.gba by tools/sprite_banks.py; this file is
 * the source now. Per bank: the animations (a frame-index sequence each),
 * the frame pointer array, the frames (header plus the boxes/anchor of
 * their layout type), then every frame's piece positions and piece bytes.
 * A frame's tiles are an offset into the sprite tile pool
 * (src/data/sprite_tiles_2bf120.c), written relative to the pool range of
 * the bank that owns them (SPRITE_TILES_BANKnn). Editing a piece's shape,
 * adding a piece or moving the tiles means redrawing
 * graphics/sprites/bankNN_*.png to match.
 */

/* ---------------------------------------------------------------------- */
/* Bank 10: 2 animations, 20 frames, tiles in gSpriteBank10Tiles (SPRITE_TILES_BANK10). */

extern const u16 gSpriteBank10Anim00Seq[9];
extern const u16 gSpriteBank10Anim01Seq[11];
extern const struct sprite_frame_1box gSpriteBank10Frame000;
extern const struct sprite_frame_1box gSpriteBank10Frame001;
extern const struct sprite_frame_1box gSpriteBank10Frame002;
extern const struct sprite_frame_1box gSpriteBank10Frame003;
extern const struct sprite_frame_1box gSpriteBank10Frame004;
extern const struct sprite_frame_1box gSpriteBank10Frame005;
extern const struct sprite_frame_1box gSpriteBank10Frame006;
extern const struct sprite_frame_1box gSpriteBank10Frame007;
extern const struct sprite_frame_1box gSpriteBank10Frame008;
extern const struct sprite_frame_1box gSpriteBank10Frame009;
extern const struct sprite_frame_1box gSpriteBank10Frame010;
extern const struct sprite_frame_1box gSpriteBank10Frame011;
extern const struct sprite_frame_1box gSpriteBank10Frame012;
extern const struct sprite_frame_1box gSpriteBank10Frame013;
extern const struct sprite_frame_1box gSpriteBank10Frame014;
extern const struct sprite_frame_1box gSpriteBank10Frame015;
extern const struct sprite_frame_1box gSpriteBank10Frame016;
extern const struct sprite_frame_1box gSpriteBank10Frame017;
extern const struct sprite_frame_1box gSpriteBank10Frame018;
extern const struct sprite_frame_1box gSpriteBank10Frame019;
extern const struct sprite_piece_pos gSpriteBank10Frame000Pos[5];
extern const struct sprite_piece_pos gSpriteBank10Frame001Pos[5];
extern const struct sprite_piece_pos gSpriteBank10Frame002Pos[3];
extern const struct sprite_piece_pos gSpriteBank10Frame003Pos[3];
extern const struct sprite_piece_pos gSpriteBank10Frame004Pos[1];
extern const struct sprite_piece_pos gSpriteBank10Frame005Pos[6];
extern const struct sprite_piece_pos gSpriteBank10Frame006Pos[4];
extern const struct sprite_piece_pos gSpriteBank10Frame007Pos[3];
extern const struct sprite_piece_pos gSpriteBank10Frame008Pos[4];
extern const struct sprite_piece_pos gSpriteBank10Frame009Pos[3];
extern const struct sprite_piece_pos gSpriteBank10Frame010Pos[4];
extern const struct sprite_piece_pos gSpriteBank10Frame011Pos[2];
extern const struct sprite_piece_pos gSpriteBank10Frame012Pos[2];
extern const struct sprite_piece_pos gSpriteBank10Frame013Pos[4];
extern const struct sprite_piece_pos gSpriteBank10Frame014Pos[4];
extern const struct sprite_piece_pos gSpriteBank10Frame015Pos[3];
extern const struct sprite_piece_pos gSpriteBank10Frame016Pos[2];
extern const struct sprite_piece_pos gSpriteBank10Frame017Pos[1];
extern const struct sprite_piece_pos gSpriteBank10Frame018Pos[1];
extern const struct sprite_piece_pos gSpriteBank10Frame019Pos[5];
extern const u8 gSpriteBank10Frame000Pieces[5];
extern const u8 gSpriteBank10Frame001Pieces[5];
extern const u8 gSpriteBank10Frame002Pieces[3];
extern const u8 gSpriteBank10Frame003Pieces[3];
extern const u8 gSpriteBank10Frame004Pieces[1];
extern const u8 gSpriteBank10Frame005Pieces[6];
extern const u8 gSpriteBank10Frame006Pieces[4];
extern const u8 gSpriteBank10Frame007Pieces[3];
extern const u8 gSpriteBank10Frame008Pieces[4];
extern const u8 gSpriteBank10Frame009Pieces[3];
extern const u8 gSpriteBank10Frame010Pieces[4];
extern const u8 gSpriteBank10Frame011Pieces[2];
extern const u8 gSpriteBank10Frame012Pieces[2];
extern const u8 gSpriteBank10Frame013Pieces[4];
extern const u8 gSpriteBank10Frame014Pieces[4];
extern const u8 gSpriteBank10Frame015Pieces[3];
extern const u8 gSpriteBank10Frame016Pieces[2];
extern const u8 gSpriteBank10Frame017Pieces[1];
extern const u8 gSpriteBank10Frame018Pieces[1];
extern const u8 gSpriteBank10Frame019Pieces[5];

const struct sprite_anim gSpriteBank10Anims[2] = {
    [0] = {
        .seq = gSpriteBank10Anim00Seq,
        .box = { { -19, -21, 38, 42 }, { -29, -28, 49, 52 } },
        .tileRecord = 17,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank10Anim00Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [1] = {
        .seq = gSpriteBank10Anim01Seq,
        .box = { { -19, -21, 38, 42 }, { -36, -31, 94, 55 } },
        .tileRecord = 17,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank10Anim01Seq),
        .flags = 0,
    },
};

const u16 gSpriteBank10Anim00Seq[9] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8,
};
const u16 gSpriteBank10Anim01Seq[11] = {
    9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
};

const struct sprite_frame *const gSpriteBank10Frames[20] = {
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
    SPRITE_FRAME(gSpriteBank10Frame000, SPRITE_TILES_BANK10 + 0x00000),
    { { -10, -18, 24, 35 } },
};
const struct sprite_frame_1box gSpriteBank10Frame001 = {
    SPRITE_FRAME(gSpriteBank10Frame001, SPRITE_TILES_BANK10 + 0x00380),
    { { -12, -22, 25, 39 } },
};
const struct sprite_frame_1box gSpriteBank10Frame002 = {
    SPRITE_FRAME(gSpriteBank10Frame002, SPRITE_TILES_BANK10 + 0x00740),
    { { -15, -24, 27, 41 } },
};
const struct sprite_frame_1box gSpriteBank10Frame003 = {
    SPRITE_FRAME(gSpriteBank10Frame003, SPRITE_TILES_BANK10 + 0x00c60),
    { { -17, -25, 26, 42 } },
};
const struct sprite_frame_1box gSpriteBank10Frame004 = {
    SPRITE_FRAME(gSpriteBank10Frame004, SPRITE_TILES_BANK10 + 0x01180),
    { { -17, -26, 27, 44 } },
};
const struct sprite_frame_1box gSpriteBank10Frame005 = {
    SPRITE_FRAME(gSpriteBank10Frame005, SPRITE_TILES_BANK10 + 0x01980),
    { { -15, -20, 28, 43 } },
};
const struct sprite_frame_1box gSpriteBank10Frame006 = {
    SPRITE_FRAME(gSpriteBank10Frame006, SPRITE_TILES_BANK10 + 0x01d80),
    { { -10, -15, 23, 35 } },
};
const struct sprite_frame_1box gSpriteBank10Frame007 = {
    SPRITE_FRAME(gSpriteBank10Frame007, SPRITE_TILES_BANK10 + 0x020a0),
    { { -10, -12, 22, 33 } },
};
const struct sprite_frame_1box gSpriteBank10Frame008 = {
    SPRITE_FRAME(gSpriteBank10Frame008, SPRITE_TILES_BANK10 + 0x023a0),
    { { -11, -14, 23, 33 } },
};
const struct sprite_frame_1box gSpriteBank10Frame009 = {
    SPRITE_FRAME(gSpriteBank10Frame009, SPRITE_TILES_BANK10 + 0x026c0),
    { { -19, -18, 23, 38 } },
};
const struct sprite_frame_1box gSpriteBank10Frame010 = {
    SPRITE_FRAME(gSpriteBank10Frame010, SPRITE_TILES_BANK10 + 0x02a40),
    { { -24, -14, 27, 33 } },
};
const struct sprite_frame_1box gSpriteBank10Frame011 = {
    SPRITE_FRAME(gSpriteBank10Frame011, SPRITE_TILES_BANK10 + 0x02e00),
    { { -15, -27, 22, 46 } },
};
const struct sprite_frame_1box gSpriteBank10Frame012 = {
    SPRITE_FRAME(gSpriteBank10Frame012, SPRITE_TILES_BANK10 + 0x03280),
    { { -4, -16, 40, 35 } },
};
const struct sprite_frame_1box gSpriteBank10Frame013 = {
    SPRITE_FRAME(gSpriteBank10Frame013, SPRITE_TILES_BANK10 + 0x03aa0),
    { { -5, -5, 63, 26 } },
};
const struct sprite_frame_1box gSpriteBank10Frame014 = {
    SPRITE_FRAME(gSpriteBank10Frame014, SPRITE_TILES_BANK10 + 0x03f40),
    { { -5, -5, 61, 22 } },
};
const struct sprite_frame_1box gSpriteBank10Frame015 = {
    SPRITE_FRAME(gSpriteBank10Frame015, SPRITE_TILES_BANK10 + 0x04420),
    { { -7, -6, 54, 23 } },
};
const struct sprite_frame_1box gSpriteBank10Frame016 = {
    SPRITE_FRAME(gSpriteBank10Frame016, SPRITE_TILES_BANK10 + 0x04860),
    { { -8, -11, 51, 25 } },
};
const struct sprite_frame_1box gSpriteBank10Frame017 = {
    SPRITE_FRAME(gSpriteBank10Frame017, SPRITE_TILES_BANK10 + 0x04d20),
    { { -4, -13, 39, 33 } },
};
const struct sprite_frame_1box gSpriteBank10Frame018 = {
    SPRITE_FRAME(gSpriteBank10Frame018, SPRITE_TILES_BANK10 + 0x054e0),
    { { -7, -16, 34, 34 } },
};
const struct sprite_frame_1box gSpriteBank10Frame019 = {
    SPRITE_FRAME(gSpriteBank10Frame019, SPRITE_TILES_BANK10 + 0x05ce0),
    { { -7, -17, 29, 35 } },
};

const struct sprite_piece_pos gSpriteBank10Frame000Pos[5] = { { -19, -21 }, { 13, -20 }, { 13, -4 }, { -19, 11 }, { 13, 13 } };
const struct sprite_piece_pos gSpriteBank10Frame001Pos[5] = { { -19, -23 }, { 11, -23 }, { 19, -14 }, { -21, 9 }, { 11, 12 } };
const struct sprite_piece_pos gSpriteBank10Frame002Pos[3] = { { -24, -25 }, { 8, -25 }, { 8, 9 } };
const struct sprite_piece_pos gSpriteBank10Frame003Pos[3] = { { -27, -27 }, { 5, -27 }, { 5, 11 } };
const struct sprite_piece_pos gSpriteBank10Frame004Pos[1] = { { -29, -28 } };
const struct sprite_piece_pos gSpriteBank10Frame005Pos[6] = { { -21, -20 }, { 6, -20 }, { 14, -20 }, { 14, -4 }, { -26, 12 }, { 6, 12 } };
const struct sprite_piece_pos gSpriteBank10Frame006Pos[4] = { { -17, -16 }, { 13, -12 }, { -19, 16 }, { 13, 16 } };
const struct sprite_piece_pos gSpriteBank10Frame007Pos[3] = { { -17, -14 }, { 14, -12 }, { -18, 18 } };
const struct sprite_piece_pos gSpriteBank10Frame008Pos[4] = { { -17, -17 }, { 13, -16 }, { -19, 15 }, { 13, 15 } };
const struct sprite_piece_pos gSpriteBank10Frame009Pos[3] = { { -28, -18 }, { 4, -14 }, { -24, 14 } };
const struct sprite_piece_pos gSpriteBank10Frame010Pos[4] = { { -36, -16 }, { -4, -11 }, { 4, 5 }, { -25, 16 } };
const struct sprite_piece_pos gSpriteBank10Frame011Pos[2] = { { -25, -31 }, { 7, -15 } };
const struct sprite_piece_pos gSpriteBank10Frame012Pos[2] = { { -21, -26 }, { 43, -1 } };
const struct sprite_piece_pos gSpriteBank10Frame013Pos[4] = { { -17, -10 }, { 47, -4 }, { 55, 0 }, { -9, 22 } };
const struct sprite_piece_pos gSpriteBank10Frame014Pos[4] = { { -18, -12 }, { 46, -3 }, { 54, 2 }, { -9, 20 } };
const struct sprite_piece_pos gSpriteBank10Frame015Pos[3] = { { -18, -12 }, { 46, 10 }, { -9, 20 } };
const struct sprite_piece_pos gSpriteBank10Frame016Pos[2] = { { -19, -19 }, { -10, 13 } };
const struct sprite_piece_pos gSpriteBank10Frame017Pos[1] = { { -19, -25 } };
const struct sprite_piece_pos gSpriteBank10Frame018Pos[1] = { { -20, -25 } };
const struct sprite_piece_pos gSpriteBank10Frame019Pos[5] = { { -16, -21 }, { 13, -19 }, { 21, -17 }, { -19, 11 }, { 13, 13 } };

const u8 gSpriteBank10Frame000Pieces[5] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank10Frame001Pieces[5] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank10Frame002Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank10Frame003Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank10Frame004Pieces[1] = { SPRITE_PIECE(2, 3) };
const u8 gSpriteBank10Frame005Pieces[6] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank10Frame006Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank10Frame007Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank10Frame008Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank10Frame009Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank10Frame010Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank10Frame011Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank10Frame012Pieces[2] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank10Frame013Pieces[4] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank10Frame014Pieces[4] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank10Frame015Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank10Frame016Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank10Frame017Pieces[1] = { SPRITE_PIECE(2, 3) };
const u8 gSpriteBank10Frame018Pieces[1] = { SPRITE_PIECE(2, 3) };
const u8 gSpriteBank10Frame019Pieces[5] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };

/* ---------------------------------------------------------------------- */
/* Bank 11: 2 animations, 25 frames, tiles in gSpriteBank11Tiles (SPRITE_TILES_BANK11). */

extern const u16 gSpriteBank11Anim00Seq[11];
extern const u16 gSpriteBank11Anim01Seq[14];
extern const struct sprite_frame_1box gSpriteBank11Frame000;
extern const struct sprite_frame_1box gSpriteBank11Frame001;
extern const struct sprite_frame_1box gSpriteBank11Frame002;
extern const struct sprite_frame_1box gSpriteBank11Frame003;
extern const struct sprite_frame_1box gSpriteBank11Frame004;
extern const struct sprite_frame_1box gSpriteBank11Frame005;
extern const struct sprite_frame_1box gSpriteBank11Frame006;
extern const struct sprite_frame_1box gSpriteBank11Frame007;
extern const struct sprite_frame_1box gSpriteBank11Frame008;
extern const struct sprite_frame_1box gSpriteBank11Frame009;
extern const struct sprite_frame_1box gSpriteBank11Frame010;
extern const struct sprite_frame_1box gSpriteBank11Frame011;
extern const struct sprite_frame_1box gSpriteBank11Frame012;
extern const struct sprite_frame_1box gSpriteBank11Frame013;
extern const struct sprite_frame_1box gSpriteBank11Frame014;
extern const struct sprite_frame_1box gSpriteBank11Frame015;
extern const struct sprite_frame_1box gSpriteBank11Frame016;
extern const struct sprite_frame_1box gSpriteBank11Frame017;
extern const struct sprite_frame_1box gSpriteBank11Frame018;
extern const struct sprite_frame_1box gSpriteBank11Frame019;
extern const struct sprite_frame_1box gSpriteBank11Frame020;
extern const struct sprite_frame_1box gSpriteBank11Frame021;
extern const struct sprite_frame_1box gSpriteBank11Frame022;
extern const struct sprite_frame_1box gSpriteBank11Frame023;
extern const struct sprite_frame_1box gSpriteBank11Frame024;
extern const struct sprite_piece_pos gSpriteBank11Frame000Pos[4];
extern const struct sprite_piece_pos gSpriteBank11Frame001Pos[2];
extern const struct sprite_piece_pos gSpriteBank11Frame002Pos[4];
extern const struct sprite_piece_pos gSpriteBank11Frame003Pos[3];
extern const struct sprite_piece_pos gSpriteBank11Frame004Pos[4];
extern const struct sprite_piece_pos gSpriteBank11Frame005Pos[2];
extern const struct sprite_piece_pos gSpriteBank11Frame006Pos[4];
extern const struct sprite_piece_pos gSpriteBank11Frame007Pos[1];
extern const struct sprite_piece_pos gSpriteBank11Frame008Pos[3];
extern const struct sprite_piece_pos gSpriteBank11Frame009Pos[2];
extern const struct sprite_piece_pos gSpriteBank11Frame010Pos[4];
extern const struct sprite_piece_pos gSpriteBank11Frame011Pos[1];
extern const struct sprite_piece_pos gSpriteBank11Frame012Pos[2];
extern const struct sprite_piece_pos gSpriteBank11Frame013Pos[2];
extern const struct sprite_piece_pos gSpriteBank11Frame014Pos[2];
extern const struct sprite_piece_pos gSpriteBank11Frame015Pos[2];
extern const struct sprite_piece_pos gSpriteBank11Frame016Pos[3];
extern const struct sprite_piece_pos gSpriteBank11Frame017Pos[3];
extern const struct sprite_piece_pos gSpriteBank11Frame018Pos[2];
extern const struct sprite_piece_pos gSpriteBank11Frame019Pos[2];
extern const struct sprite_piece_pos gSpriteBank11Frame020Pos[2];
extern const struct sprite_piece_pos gSpriteBank11Frame021Pos[2];
extern const struct sprite_piece_pos gSpriteBank11Frame022Pos[2];
extern const struct sprite_piece_pos gSpriteBank11Frame023Pos[1];
extern const struct sprite_piece_pos gSpriteBank11Frame024Pos[1];
extern const u8 gSpriteBank11Frame000Pieces[4];
extern const u8 gSpriteBank11Frame001Pieces[2];
extern const u8 gSpriteBank11Frame002Pieces[4];
extern const u8 gSpriteBank11Frame003Pieces[3];
extern const u8 gSpriteBank11Frame004Pieces[4];
extern const u8 gSpriteBank11Frame005Pieces[2];
extern const u8 gSpriteBank11Frame006Pieces[4];
extern const u8 gSpriteBank11Frame007Pieces[1];
extern const u8 gSpriteBank11Frame008Pieces[3];
extern const u8 gSpriteBank11Frame009Pieces[2];
extern const u8 gSpriteBank11Frame010Pieces[4];
extern const u8 gSpriteBank11Frame011Pieces[1];
extern const u8 gSpriteBank11Frame012Pieces[2];
extern const u8 gSpriteBank11Frame013Pieces[2];
extern const u8 gSpriteBank11Frame014Pieces[2];
extern const u8 gSpriteBank11Frame015Pieces[2];
extern const u8 gSpriteBank11Frame016Pieces[3];
extern const u8 gSpriteBank11Frame017Pieces[3];
extern const u8 gSpriteBank11Frame018Pieces[2];
extern const u8 gSpriteBank11Frame019Pieces[2];
extern const u8 gSpriteBank11Frame020Pieces[2];
extern const u8 gSpriteBank11Frame021Pieces[2];
extern const u8 gSpriteBank11Frame022Pieces[2];
extern const u8 gSpriteBank11Frame023Pieces[1];
extern const u8 gSpriteBank11Frame024Pieces[1];

const struct sprite_anim gSpriteBank11Anims[2] = {
    [0] = {
        .seq = gSpriteBank11Anim00Seq,
        .box = { { -21, -17, 42, 35 }, { -37, -21, 58, 52 } },
        .tileRecord = 18,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank11Anim00Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [1] = {
        .seq = gSpriteBank11Anim01Seq,
        .box = { { -21, -17, 42, 35 }, { -36, -37, 60, 68 } },
        .tileRecord = 18,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank11Anim01Seq),
        .flags = 0,
    },
};

const u16 gSpriteBank11Anim00Seq[11] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
};
const u16 gSpriteBank11Anim01Seq[14] = {
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
};

const struct sprite_frame *const gSpriteBank11Frames[25] = {
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
    SPRITE_FRAME(gSpriteBank11Frame000, SPRITE_TILES_BANK11 + 0x00000),
    { { -17, -9, 37, 21 } },
};
const struct sprite_frame_1box gSpriteBank11Frame001 = {
    SPRITE_FRAME(gSpriteBank11Frame001, SPRITE_TILES_BANK11 + 0x002c0),
    { { -23, -12, 41, 24 } },
};
const struct sprite_frame_1box gSpriteBank11Frame002 = {
    SPRITE_FRAME(gSpriteBank11Frame002, SPRITE_TILES_BANK11 + 0x006e0),
    { { -16, -1, 32, 26 } },
};
const struct sprite_frame_1box gSpriteBank11Frame003 = {
    SPRITE_FRAME(gSpriteBank11Frame003, SPRITE_TILES_BANK11 + 0x009c0),
    { { -12, -2, 28, 19 } },
};
const struct sprite_frame_1box gSpriteBank11Frame004 = {
    SPRITE_FRAME(gSpriteBank11Frame004, SPRITE_TILES_BANK11 + 0x00c20),
    { { -19, -4, 37, 17 } },
};
const struct sprite_frame_1box gSpriteBank11Frame005 = {
    SPRITE_FRAME(gSpriteBank11Frame005, SPRITE_TILES_BANK11 + 0x00f00),
    { { -23, -7, 34, 19 } },
};
const struct sprite_frame_1box gSpriteBank11Frame006 = {
    SPRITE_FRAME(gSpriteBank11Frame006, SPRITE_TILES_BANK11 + 0x01320),
    { { -18, -11, 28, 19 } },
};
const struct sprite_frame_1box gSpriteBank11Frame007 = {
    SPRITE_FRAME(gSpriteBank11Frame007, SPRITE_TILES_BANK11 + 0x01600),
    { { -31, -12, 39, 17 } },
};
const struct sprite_frame_1box gSpriteBank11Frame008 = {
    SPRITE_FRAME(gSpriteBank11Frame008, SPRITE_TILES_BANK11 + 0x01a00),
    { { -12, -14, 26, 24 } },
};
const struct sprite_frame_1box gSpriteBank11Frame009 = {
    SPRITE_FRAME(gSpriteBank11Frame009, SPRITE_TILES_BANK11 + 0x01c80),
    { { -12, -14, 28, 19 } },
};
const struct sprite_frame_1box gSpriteBank11Frame010 = {
    SPRITE_FRAME(gSpriteBank11Frame010, SPRITE_TILES_BANK11 + 0x01f00),
    { { -17, -15, 32, 21 } },
};
const struct sprite_frame_1box gSpriteBank11Frame011 = {
    SPRITE_FRAME(gSpriteBank11Frame011, SPRITE_TILES_BANK11 + 0x021e0),
    { { -20, -13, 42, 20 } },
};
const struct sprite_frame_1box gSpriteBank11Frame012 = {
    SPRITE_FRAME(gSpriteBank11Frame012, SPRITE_TILES_BANK11 + 0x025e0),
    { { -19, -21, 41, 30 } },
};
const struct sprite_frame_1box gSpriteBank11Frame013 = {
    SPRITE_FRAME(gSpriteBank11Frame013, SPRITE_TILES_BANK11 + 0x02a60),
    { { -9, -27, 26, 42 } },
};
const struct sprite_frame_1box gSpriteBank11Frame014 = {
    SPRITE_FRAME(gSpriteBank11Frame014, SPRITE_TILES_BANK11 + 0x03260),
    { { -11, -23, 31, 44 } },
};
const struct sprite_frame_1box gSpriteBank11Frame015 = {
    SPRITE_FRAME(gSpriteBank11Frame015, SPRITE_TILES_BANK11 + 0x036a0),
    { { -9, -16, 30, 40 } },
};
const struct sprite_frame_1box gSpriteBank11Frame016 = {
    SPRITE_FRAME(gSpriteBank11Frame016, SPRITE_TILES_BANK11 + 0x03ae0),
    { { -11, -10, 30, 38 } },
};
const struct sprite_frame_1box gSpriteBank11Frame017 = {
    SPRITE_FRAME(gSpriteBank11Frame017, SPRITE_TILES_BANK11 + 0x03da0),
    { { -22, -5, 43, 34 } },
};
const struct sprite_frame_1box gSpriteBank11Frame018 = {
    SPRITE_FRAME(gSpriteBank11Frame018, SPRITE_TILES_BANK11 + 0x041e0),
    { { -22, -2, 43, 24 } },
};
const struct sprite_frame_1box gSpriteBank11Frame019 = {
    SPRITE_FRAME(gSpriteBank11Frame019, SPRITE_TILES_BANK11 + 0x04600),
    { { -27, -2, 49, 23 } },
};
const struct sprite_frame_1box gSpriteBank11Frame020 = {
    SPRITE_FRAME(gSpriteBank11Frame020, SPRITE_TILES_BANK11 + 0x04a20),
    { { -28, -4, 51, 26 } },
};
const struct sprite_frame_1box gSpriteBank11Frame021 = {
    SPRITE_FRAME(gSpriteBank11Frame021, SPRITE_TILES_BANK11 + 0x04600),
    { { -27, -4, 46, 27 } },
};
const struct sprite_frame_1box gSpriteBank11Frame022 = {
    SPRITE_FRAME(gSpriteBank11Frame022, SPRITE_TILES_BANK11 + 0x04e40),
    { { -29, -3, 49, 24 } },
};
const struct sprite_frame_1box gSpriteBank11Frame023 = {
    SPRITE_FRAME(gSpriteBank11Frame023, SPRITE_TILES_BANK11 + 0x05260),
    { { -27, 0, 47, 17 } },
};
const struct sprite_frame_1box gSpriteBank11Frame024 = {
    SPRITE_FRAME(gSpriteBank11Frame024, SPRITE_TILES_BANK11 + 0x05660),
    { { -31, -8, 48, 19 } },
};

const struct sprite_piece_pos gSpriteBank11Frame000Pos[4] = { { -21, -17 }, { 11, -10 }, { 19, 5 }, { -3, 15 } };
const struct sprite_piece_pos gSpriteBank11Frame001Pos[2] = { { -30, -16 }, { -6, 16 } };
const struct sprite_piece_pos gSpriteBank11Frame002Pos[4] = { { -22, -4 }, { 10, -8 }, { 18, 9 }, { -22, 24 } };
const struct sprite_piece_pos gSpriteBank11Frame003Pos[3] = { { -18, -9 }, { 14, -7 }, { 14, 9 } };
const struct sprite_piece_pos gSpriteBank11Frame004Pos[4] = { { -22, -5 }, { 10, -12 }, { 18, 3 }, { -3, 20 } };
const struct sprite_piece_pos gSpriteBank11Frame005Pos[2] = { { -29, -14 }, { -2, 18 } };
const struct sprite_piece_pos gSpriteBank11Frame006Pos[4] = { { -24, -18 }, { 8, -17 }, { 16, -3 }, { 1, 14 } };
const struct sprite_piece_pos gSpriteBank11Frame007Pos[1] = { { -37, -19 } };
const struct sprite_piece_pos gSpriteBank11Frame008Pos[3] = { { -17, -21 }, { 15, -6 }, { -14, 11 } };
const struct sprite_piece_pos gSpriteBank11Frame009Pos[2] = { { -18, -21 }, { 14, -20 } };
const struct sprite_piece_pos gSpriteBank11Frame010Pos[4] = { { -23, -14 }, { 9, -21 }, { 17, -6 }, { -3, 11 } };
const struct sprite_piece_pos gSpriteBank11Frame011Pos[1] = { { -26, -17 } };
const struct sprite_piece_pos gSpriteBank11Frame012Pos[2] = { { -27, -27 }, { 10, 5 } };
const struct sprite_piece_pos gSpriteBank11Frame013Pos[2] = { { -16, -37 }, { 16, -32 } };
const struct sprite_piece_pos gSpriteBank11Frame014Pos[2] = { { -14, -28 }, { 18, -25 } };
const struct sprite_piece_pos gSpriteBank11Frame015Pos[2] = { { -13, -23 }, { 19, -20 } };
const struct sprite_piece_pos gSpriteBank11Frame016Pos[3] = { { -19, -14 }, { 13, -13 }, { 4, 18 } };
const struct sprite_piece_pos gSpriteBank11Frame017Pos[3] = { { -28, -9 }, { 12, 23 }, { 20, 24 } };
const struct sprite_piece_pos gSpriteBank11Frame018Pos[2] = { { -30, -7 }, { 16, 25 } };
const struct sprite_piece_pos gSpriteBank11Frame019Pos[2] = { { -32, -7 }, { 17, 25 } };
const struct sprite_piece_pos gSpriteBank11Frame020Pos[2] = { { -34, -7 }, { 18, 25 } };
const struct sprite_piece_pos gSpriteBank11Frame021Pos[2] = { { -32, -7 }, { 17, 25 } };
const struct sprite_piece_pos gSpriteBank11Frame022Pos[2] = { { -30, -6 }, { 19, 26 } };
const struct sprite_piece_pos gSpriteBank11Frame023Pos[1] = { { -33, -5 } };
const struct sprite_piece_pos gSpriteBank11Frame024Pos[1] = { { -36, -12 } };

const u8 gSpriteBank11Frame000Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank11Frame001Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank11Frame002Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank11Frame003Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank11Frame004Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank11Frame005Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank11Frame006Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank11Frame007Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank11Frame008Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank11Frame009Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank11Frame010Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank11Frame011Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank11Frame012Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank11Frame013Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11) };
const u8 gSpriteBank11Frame014Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank11Frame015Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank11Frame016Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank11Frame017Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank11Frame018Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank11Frame019Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank11Frame020Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank11Frame021Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank11Frame022Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank11Frame023Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank11Frame024Pieces[1] = { SPRITE_PIECE(2, 7) };

/* ---------------------------------------------------------------------- */
/* Bank 12: 7 animations, 57 frames, tiles in gSpriteBank12Tiles (SPRITE_TILES_BANK12). */

extern const u16 gSpriteBank12Anim00Seq[17];
extern const u16 gSpriteBank12Anim01Seq[5];
extern const u16 gSpriteBank12Anim02Seq[6];
extern const u16 gSpriteBank12Anim03Seq[5];
extern const u16 gSpriteBank12Anim04Seq[6];
extern const u16 gSpriteBank12Anim05Seq[16];
extern const u16 gSpriteBank12Anim06Seq[2];
extern const struct sprite_frame_1box gSpriteBank12Frame000;
extern const struct sprite_frame_1box gSpriteBank12Frame001;
extern const struct sprite_frame_1box gSpriteBank12Frame002;
extern const struct sprite_frame_1box gSpriteBank12Frame003;
extern const struct sprite_frame_1box gSpriteBank12Frame004;
extern const struct sprite_frame_1box gSpriteBank12Frame005;
extern const struct sprite_frame_1box gSpriteBank12Frame006;
extern const struct sprite_frame_1box gSpriteBank12Frame007;
extern const struct sprite_frame_1box gSpriteBank12Frame008;
extern const struct sprite_frame_1box gSpriteBank12Frame009;
extern const struct sprite_frame_1box gSpriteBank12Frame010;
extern const struct sprite_frame_1box gSpriteBank12Frame011;
extern const struct sprite_frame_1box gSpriteBank12Frame012;
extern const struct sprite_frame_1box gSpriteBank12Frame013;
extern const struct sprite_frame_1box gSpriteBank12Frame014;
extern const struct sprite_frame_1box gSpriteBank12Frame015;
extern const struct sprite_frame_1box gSpriteBank12Frame016;
extern const struct sprite_frame_1box gSpriteBank12Frame017;
extern const struct sprite_frame_1box gSpriteBank12Frame018;
extern const struct sprite_frame_1box gSpriteBank12Frame019;
extern const struct sprite_frame_1box gSpriteBank12Frame020;
extern const struct sprite_frame_1box gSpriteBank12Frame021;
extern const struct sprite_frame_1box gSpriteBank12Frame022;
extern const struct sprite_frame_1box gSpriteBank12Frame023;
extern const struct sprite_frame_1box gSpriteBank12Frame024;
extern const struct sprite_frame_1box gSpriteBank12Frame025;
extern const struct sprite_frame_1box gSpriteBank12Frame026;
extern const struct sprite_frame_1box gSpriteBank12Frame027;
extern const struct sprite_frame_1box gSpriteBank12Frame028;
extern const struct sprite_frame_1box gSpriteBank12Frame029;
extern const struct sprite_frame_1box gSpriteBank12Frame030;
extern const struct sprite_frame_1box gSpriteBank12Frame031;
extern const struct sprite_frame_1box gSpriteBank12Frame032;
extern const struct sprite_frame_1box gSpriteBank12Frame033;
extern const struct sprite_frame_1box gSpriteBank12Frame034;
extern const struct sprite_frame_1box gSpriteBank12Frame035;
extern const struct sprite_frame_1box gSpriteBank12Frame036;
extern const struct sprite_frame_1box gSpriteBank12Frame037;
extern const struct sprite_frame_1box gSpriteBank12Frame038;
extern const struct sprite_frame_1box gSpriteBank12Frame039;
extern const struct sprite_frame_1box gSpriteBank12Frame040;
extern const struct sprite_frame_1box gSpriteBank12Frame041;
extern const struct sprite_frame_1box gSpriteBank12Frame042;
extern const struct sprite_frame_1box gSpriteBank12Frame043;
extern const struct sprite_frame_1box gSpriteBank12Frame044;
extern const struct sprite_frame_1box gSpriteBank12Frame045;
extern const struct sprite_frame_1box gSpriteBank12Frame046;
extern const struct sprite_frame_1box gSpriteBank12Frame047;
extern const struct sprite_frame_1box gSpriteBank12Frame048;
extern const struct sprite_frame_1box gSpriteBank12Frame049;
extern const struct sprite_frame_1box gSpriteBank12Frame050;
extern const struct sprite_frame_1box gSpriteBank12Frame051;
extern const struct sprite_frame_1box gSpriteBank12Frame052;
extern const struct sprite_frame_1box gSpriteBank12Frame053;
extern const struct sprite_frame_1box gSpriteBank12Frame054;
extern const struct sprite_frame_1box gSpriteBank12Frame055;
extern const struct sprite_frame_1box gSpriteBank12Frame056;
extern const struct sprite_piece_pos gSpriteBank12Frame000Pos[1];
extern const struct sprite_piece_pos gSpriteBank12Frame001Pos[2];
extern const struct sprite_piece_pos gSpriteBank12Frame002Pos[2];
extern const struct sprite_piece_pos gSpriteBank12Frame003Pos[2];
extern const struct sprite_piece_pos gSpriteBank12Frame004Pos[2];
extern const struct sprite_piece_pos gSpriteBank12Frame005Pos[2];
extern const struct sprite_piece_pos gSpriteBank12Frame006Pos[2];
extern const struct sprite_piece_pos gSpriteBank12Frame007Pos[2];
extern const struct sprite_piece_pos gSpriteBank12Frame008Pos[2];
extern const struct sprite_piece_pos gSpriteBank12Frame009Pos[2];
extern const struct sprite_piece_pos gSpriteBank12Frame010Pos[2];
extern const struct sprite_piece_pos gSpriteBank12Frame011Pos[2];
extern const struct sprite_piece_pos gSpriteBank12Frame012Pos[2];
extern const struct sprite_piece_pos gSpriteBank12Frame013Pos[2];
extern const struct sprite_piece_pos gSpriteBank12Frame014Pos[2];
extern const struct sprite_piece_pos gSpriteBank12Frame015Pos[2];
extern const struct sprite_piece_pos gSpriteBank12Frame016Pos[1];
extern const struct sprite_piece_pos gSpriteBank12Frame017Pos[2];
extern const struct sprite_piece_pos gSpriteBank12Frame018Pos[2];
extern const struct sprite_piece_pos gSpriteBank12Frame019Pos[2];
extern const struct sprite_piece_pos gSpriteBank12Frame020Pos[2];
extern const struct sprite_piece_pos gSpriteBank12Frame021Pos[2];
extern const struct sprite_piece_pos gSpriteBank12Frame022Pos[2];
extern const struct sprite_piece_pos gSpriteBank12Frame023Pos[2];
extern const struct sprite_piece_pos gSpriteBank12Frame024Pos[2];
extern const struct sprite_piece_pos gSpriteBank12Frame025Pos[1];
extern const struct sprite_piece_pos gSpriteBank12Frame026Pos[1];
extern const struct sprite_piece_pos gSpriteBank12Frame027Pos[1];
extern const struct sprite_piece_pos gSpriteBank12Frame028Pos[1];
extern const struct sprite_piece_pos gSpriteBank12Frame029Pos[1];
extern const struct sprite_piece_pos gSpriteBank12Frame030Pos[1];
extern const struct sprite_piece_pos gSpriteBank12Frame031Pos[1];
extern const struct sprite_piece_pos gSpriteBank12Frame032Pos[1];
extern const struct sprite_piece_pos gSpriteBank12Frame033Pos[1];
extern const struct sprite_piece_pos gSpriteBank12Frame034Pos[1];
extern const struct sprite_piece_pos gSpriteBank12Frame035Pos[1];
extern const struct sprite_piece_pos gSpriteBank12Frame036Pos[2];
extern const struct sprite_piece_pos gSpriteBank12Frame037Pos[2];
extern const struct sprite_piece_pos gSpriteBank12Frame038Pos[2];
extern const struct sprite_piece_pos gSpriteBank12Frame039Pos[3];
extern const struct sprite_piece_pos gSpriteBank12Frame040Pos[4];
extern const struct sprite_piece_pos gSpriteBank12Frame041Pos[4];
extern const struct sprite_piece_pos gSpriteBank12Frame042Pos[4];
extern const struct sprite_piece_pos gSpriteBank12Frame043Pos[4];
extern const struct sprite_piece_pos gSpriteBank12Frame044Pos[4];
extern const struct sprite_piece_pos gSpriteBank12Frame045Pos[4];
extern const struct sprite_piece_pos gSpriteBank12Frame046Pos[4];
extern const struct sprite_piece_pos gSpriteBank12Frame047Pos[4];
extern const struct sprite_piece_pos gSpriteBank12Frame048Pos[4];
extern const struct sprite_piece_pos gSpriteBank12Frame049Pos[4];
extern const struct sprite_piece_pos gSpriteBank12Frame050Pos[4];
extern const struct sprite_piece_pos gSpriteBank12Frame051Pos[4];
extern const struct sprite_piece_pos gSpriteBank12Frame052Pos[4];
extern const struct sprite_piece_pos gSpriteBank12Frame053Pos[2];
extern const struct sprite_piece_pos gSpriteBank12Frame054Pos[2];
extern const struct sprite_piece_pos gSpriteBank12Frame055Pos[2];
extern const struct sprite_piece_pos gSpriteBank12Frame056Pos[1];
extern const u8 gSpriteBank12Frame000Pieces[1];
extern const u8 gSpriteBank12Frame001Pieces[2];
extern const u8 gSpriteBank12Frame002Pieces[2];
extern const u8 gSpriteBank12Frame003Pieces[2];
extern const u8 gSpriteBank12Frame004Pieces[2];
extern const u8 gSpriteBank12Frame005Pieces[2];
extern const u8 gSpriteBank12Frame006Pieces[2];
extern const u8 gSpriteBank12Frame007Pieces[2];
extern const u8 gSpriteBank12Frame008Pieces[2];
extern const u8 gSpriteBank12Frame009Pieces[2];
extern const u8 gSpriteBank12Frame010Pieces[2];
extern const u8 gSpriteBank12Frame011Pieces[2];
extern const u8 gSpriteBank12Frame012Pieces[2];
extern const u8 gSpriteBank12Frame013Pieces[2];
extern const u8 gSpriteBank12Frame014Pieces[2];
extern const u8 gSpriteBank12Frame015Pieces[2];
extern const u8 gSpriteBank12Frame016Pieces[1];
extern const u8 gSpriteBank12Frame017Pieces[2];
extern const u8 gSpriteBank12Frame018Pieces[2];
extern const u8 gSpriteBank12Frame019Pieces[2];
extern const u8 gSpriteBank12Frame020Pieces[2];
extern const u8 gSpriteBank12Frame021Pieces[2];
extern const u8 gSpriteBank12Frame022Pieces[2];
extern const u8 gSpriteBank12Frame023Pieces[2];
extern const u8 gSpriteBank12Frame024Pieces[2];
extern const u8 gSpriteBank12Frame025Pieces[1];
extern const u8 gSpriteBank12Frame026Pieces[1];
extern const u8 gSpriteBank12Frame027Pieces[1];
extern const u8 gSpriteBank12Frame028Pieces[1];
extern const u8 gSpriteBank12Frame029Pieces[1];
extern const u8 gSpriteBank12Frame030Pieces[1];
extern const u8 gSpriteBank12Frame031Pieces[1];
extern const u8 gSpriteBank12Frame032Pieces[1];
extern const u8 gSpriteBank12Frame033Pieces[1];
extern const u8 gSpriteBank12Frame034Pieces[1];
extern const u8 gSpriteBank12Frame035Pieces[1];
extern const u8 gSpriteBank12Frame036Pieces[2];
extern const u8 gSpriteBank12Frame037Pieces[2];
extern const u8 gSpriteBank12Frame038Pieces[2];
extern const u8 gSpriteBank12Frame039Pieces[3];
extern const u8 gSpriteBank12Frame040Pieces[4];
extern const u8 gSpriteBank12Frame041Pieces[4];
extern const u8 gSpriteBank12Frame042Pieces[4];
extern const u8 gSpriteBank12Frame043Pieces[4];
extern const u8 gSpriteBank12Frame044Pieces[4];
extern const u8 gSpriteBank12Frame045Pieces[4];
extern const u8 gSpriteBank12Frame046Pieces[4];
extern const u8 gSpriteBank12Frame047Pieces[4];
extern const u8 gSpriteBank12Frame048Pieces[4];
extern const u8 gSpriteBank12Frame049Pieces[4];
extern const u8 gSpriteBank12Frame050Pieces[4];
extern const u8 gSpriteBank12Frame051Pieces[4];
extern const u8 gSpriteBank12Frame052Pieces[4];
extern const u8 gSpriteBank12Frame053Pieces[2];
extern const u8 gSpriteBank12Frame054Pieces[2];
extern const u8 gSpriteBank12Frame055Pieces[2];
extern const u8 gSpriteBank12Frame056Pieces[1];

const struct sprite_anim gSpriteBank12Anims[7] = {
    [0] = {
        .seq = gSpriteBank12Anim00Seq,
        .box = { { -14, -27, 28, 55 }, { -19, -30, 44, 58 } },
        .tileRecord = 19,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank12Anim00Seq),
        .flags = 0,
    },
    [1] = {
        .seq = gSpriteBank12Anim01Seq,
        .box = { { -14, -27, 28, 55 }, { -15, -14, 33, 42 } },
        .tileRecord = 19,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank12Anim01Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [2] = {
        .seq = gSpriteBank12Anim02Seq,
        .box = { { -14, -27, 28, 55 }, { -15, -27, 32, 55 } },
        .tileRecord = 19,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank12Anim02Seq),
        .flags = 0,
    },
    [3] = {
        .seq = gSpriteBank12Anim03Seq,
        .box = { { -14, -27, 28, 55 }, { -15, -27, 29, 55 } },
        .tileRecord = 19,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank12Anim03Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [4] = {
        .seq = gSpriteBank12Anim04Seq,
        .box = { { -14, -27, 28, 55 }, { -15, -27, 32, 55 } },
        .tileRecord = 19,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank12Anim04Seq),
        .flags = 0,
    },
    [5] = {
        .seq = gSpriteBank12Anim05Seq,
        .box = { { -14, -27, 28, 55 }, { -17, -15, 41, 43 } },
        .tileRecord = 19,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank12Anim05Seq),
        .flags = 0,
    },
    [6] = {
        .seq = gSpriteBank12Anim06Seq,
        .box = { { -4, -2, 9, 4 }, { -4, -2, 9, 4 } },
        .tileRecord = 57,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank12Anim06Seq),
        .flags = SPRITE_ANIM_LOOP,
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

const struct sprite_frame *const gSpriteBank12Frames[57] = {
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
    SPRITE_FRAME(gSpriteBank12Frame000, SPRITE_TILES_BANK12 + 0x00000),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame001 = {
    SPRITE_FRAME(gSpriteBank12Frame001, SPRITE_TILES_BANK12 + 0x00400),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame002 = {
    SPRITE_FRAME(gSpriteBank12Frame002, SPRITE_TILES_BANK12 + 0x00820),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame003 = {
    SPRITE_FRAME(gSpriteBank12Frame003, SPRITE_TILES_BANK12 + 0x00c40),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame004 = {
    SPRITE_FRAME(gSpriteBank12Frame004, SPRITE_TILES_BANK12 + 0x01080),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame005 = {
    SPRITE_FRAME(gSpriteBank12Frame005, SPRITE_TILES_BANK12 + 0x014c0),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame006 = {
    SPRITE_FRAME(gSpriteBank12Frame006, SPRITE_TILES_BANK12 + 0x01900),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame007 = {
    SPRITE_FRAME(gSpriteBank12Frame007, SPRITE_TILES_BANK12 + 0x01d20),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame008 = {
    SPRITE_FRAME(gSpriteBank12Frame008, SPRITE_TILES_BANK12 + 0x02140),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame009 = {
    SPRITE_FRAME(gSpriteBank12Frame009, SPRITE_TILES_BANK12 + 0x02560),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame010 = {
    SPRITE_FRAME(gSpriteBank12Frame010, SPRITE_TILES_BANK12 + 0x02980),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame011 = {
    SPRITE_FRAME(gSpriteBank12Frame011, SPRITE_TILES_BANK12 + 0x02da0),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame012 = {
    SPRITE_FRAME(gSpriteBank12Frame012, SPRITE_TILES_BANK12 + 0x031c0),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame013 = {
    SPRITE_FRAME(gSpriteBank12Frame013, SPRITE_TILES_BANK12 + 0x035e0),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame014 = {
    SPRITE_FRAME(gSpriteBank12Frame014, SPRITE_TILES_BANK12 + 0x03a00),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame015 = {
    SPRITE_FRAME(gSpriteBank12Frame015, SPRITE_TILES_BANK12 + 0x03e20),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame016 = {
    SPRITE_FRAME(gSpriteBank12Frame016, SPRITE_TILES_BANK12 + 0x04240),
    { { -5, -19, 8, 43 } },
};
const struct sprite_frame_1box gSpriteBank12Frame017 = {
    SPRITE_FRAME(gSpriteBank12Frame017, SPRITE_TILES_BANK12 + 0x04640),
    { { -6, -5, 6, 31 } },
};
const struct sprite_frame_1box gSpriteBank12Frame018 = {
    SPRITE_FRAME(gSpriteBank12Frame018, SPRITE_TILES_BANK12 + 0x04940),
    { { -6, -5, 6, 31 } },
};
const struct sprite_frame_1box gSpriteBank12Frame019 = {
    SPRITE_FRAME(gSpriteBank12Frame019, SPRITE_TILES_BANK12 + 0x04c40),
    { { -6, -5, 6, 31 } },
};
const struct sprite_frame_1box gSpriteBank12Frame020 = {
    SPRITE_FRAME(gSpriteBank12Frame020, SPRITE_TILES_BANK12 + 0x04f40),
    { { -6, -5, 6, 31 } },
};
const struct sprite_frame_1box gSpriteBank12Frame021 = {
    SPRITE_FRAME(gSpriteBank12Frame021, SPRITE_TILES_BANK12 + 0x05240),
    { { -6, -5, 6, 31 } },
};
const struct sprite_frame_1box gSpriteBank12Frame022 = {
    SPRITE_FRAME(gSpriteBank12Frame022, SPRITE_TILES_BANK12 + 0x05540),
    { { -6, -5, 6, 29 } },
};
const struct sprite_frame_1box gSpriteBank12Frame023 = {
    SPRITE_FRAME(gSpriteBank12Frame023, SPRITE_TILES_BANK12 + 0x05840),
    { { -6, -5, 6, 29 } },
};
const struct sprite_frame_1box gSpriteBank12Frame024 = {
    SPRITE_FRAME(gSpriteBank12Frame024, SPRITE_TILES_BANK12 + 0x05b40),
    { { -6, -5, 6, 29 } },
};
const struct sprite_frame_1box gSpriteBank12Frame025 = {
    SPRITE_FRAME(gSpriteBank12Frame025, SPRITE_TILES_BANK12 + 0x05f60),
    { { -6, -5, 6, 29 } },
};
const struct sprite_frame_1box gSpriteBank12Frame026 = {
    SPRITE_FRAME(gSpriteBank12Frame026, SPRITE_TILES_BANK12 + 0x06360),
    { { -6, -5, 6, 29 } },
};
const struct sprite_frame_1box gSpriteBank12Frame027 = {
    SPRITE_FRAME(gSpriteBank12Frame027, SPRITE_TILES_BANK12 + 0x06760),
    { { -6, -5, 6, 29 } },
};
const struct sprite_frame_1box gSpriteBank12Frame028 = {
    SPRITE_FRAME(gSpriteBank12Frame028, SPRITE_TILES_BANK12 + 0x06b60),
    { { -5, -19, 6, 45 } },
};
const struct sprite_frame_1box gSpriteBank12Frame029 = {
    SPRITE_FRAME(gSpriteBank12Frame029, SPRITE_TILES_BANK12 + 0x06f60),
    { { -5, -19, 6, 45 } },
};
const struct sprite_frame_1box gSpriteBank12Frame030 = {
    SPRITE_FRAME(gSpriteBank12Frame030, SPRITE_TILES_BANK12 + 0x07360),
    { { -5, -19, 6, 45 } },
};
const struct sprite_frame_1box gSpriteBank12Frame031 = {
    SPRITE_FRAME(gSpriteBank12Frame031, SPRITE_TILES_BANK12 + 0x07760),
    { { -5, -19, 6, 45 } },
};
const struct sprite_frame_1box gSpriteBank12Frame032 = {
    SPRITE_FRAME(gSpriteBank12Frame032, SPRITE_TILES_BANK12 + 0x07b60),
    { { -5, -19, 6, 45 } },
};
const struct sprite_frame_1box gSpriteBank12Frame033 = {
    SPRITE_FRAME(gSpriteBank12Frame033, SPRITE_TILES_BANK12 + 0x06760),
    { { -5, -17, 6, 40 } },
};
const struct sprite_frame_1box gSpriteBank12Frame034 = {
    SPRITE_FRAME(gSpriteBank12Frame034, SPRITE_TILES_BANK12 + 0x06360),
    { { -5, -17, 6, 40 } },
};
const struct sprite_frame_1box gSpriteBank12Frame035 = {
    SPRITE_FRAME(gSpriteBank12Frame035, SPRITE_TILES_BANK12 + 0x05f60),
    { { -5, -17, 6, 40 } },
};
const struct sprite_frame_1box gSpriteBank12Frame036 = {
    SPRITE_FRAME(gSpriteBank12Frame036, SPRITE_TILES_BANK12 + 0x05b40),
    { { -5, -17, 6, 40 } },
};
const struct sprite_frame_1box gSpriteBank12Frame037 = {
    SPRITE_FRAME(gSpriteBank12Frame037, SPRITE_TILES_BANK12 + 0x05840),
    { { -6, -10, 7, 36 } },
};
const struct sprite_frame_1box gSpriteBank12Frame038 = {
    SPRITE_FRAME(gSpriteBank12Frame038, SPRITE_TILES_BANK12 + 0x05540),
    { { -6, -9, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame039 = {
    SPRITE_FRAME(gSpriteBank12Frame039, SPRITE_TILES_BANK12 + 0x07f60),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame040 = {
    SPRITE_FRAME(gSpriteBank12Frame040, SPRITE_TILES_BANK12 + 0x08280),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame041 = {
    SPRITE_FRAME(gSpriteBank12Frame041, SPRITE_TILES_BANK12 + 0x08540),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame042 = {
    SPRITE_FRAME(gSpriteBank12Frame042, SPRITE_TILES_BANK12 + 0x08800),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame043 = {
    SPRITE_FRAME(gSpriteBank12Frame043, SPRITE_TILES_BANK12 + 0x08ac0),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame044 = {
    SPRITE_FRAME(gSpriteBank12Frame044, SPRITE_TILES_BANK12 + 0x08d80),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame045 = {
    SPRITE_FRAME(gSpriteBank12Frame045, SPRITE_TILES_BANK12 + 0x09040),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame046 = {
    SPRITE_FRAME(gSpriteBank12Frame046, SPRITE_TILES_BANK12 + 0x09300),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame047 = {
    SPRITE_FRAME(gSpriteBank12Frame047, SPRITE_TILES_BANK12 + 0x095c0),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame048 = {
    SPRITE_FRAME(gSpriteBank12Frame048, SPRITE_TILES_BANK12 + 0x09880),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame049 = {
    SPRITE_FRAME(gSpriteBank12Frame049, SPRITE_TILES_BANK12 + 0x09b40),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame050 = {
    SPRITE_FRAME(gSpriteBank12Frame050, SPRITE_TILES_BANK12 + 0x09e00),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame051 = {
    SPRITE_FRAME(gSpriteBank12Frame051, SPRITE_TILES_BANK12 + 0x0a0c0),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame052 = {
    SPRITE_FRAME(gSpriteBank12Frame052, SPRITE_TILES_BANK12 + 0x0a380),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame053 = {
    SPRITE_FRAME(gSpriteBank12Frame053, SPRITE_TILES_BANK12 + 0x0a640),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame054 = {
    SPRITE_FRAME(gSpriteBank12Frame054, SPRITE_TILES_BANK12 + 0x0a940),
    { { -7, -7, 7, 32 } },
};
const struct sprite_frame_1box gSpriteBank12Frame055 = {
    SPRITE_FRAME(gSpriteBank12Frame055, SPRITE_TILES_BANK12 + 0x0ac40),
    { { -4, -2, 6, 4 } },
};
const struct sprite_frame_1box gSpriteBank12Frame056 = {
    SPRITE_FRAME(gSpriteBank12Frame056, SPRITE_TILES_BANK12 + 0x0ac80),
    { { -4, -2, 6, 4 } },
};

const struct sprite_piece_pos gSpriteBank12Frame000Pos[1] = { { -14, -27 } };
const struct sprite_piece_pos gSpriteBank12Frame001Pos[2] = { { -14, -28 }, { 18, 7 } };
const struct sprite_piece_pos gSpriteBank12Frame002Pos[2] = { { -15, -27 }, { 17, 0 } };
const struct sprite_piece_pos gSpriteBank12Frame003Pos[2] = { { -17, -27 }, { 15, -5 } };
const struct sprite_piece_pos gSpriteBank12Frame004Pos[2] = { { -18, -27 }, { 14, -10 } };
const struct sprite_piece_pos gSpriteBank12Frame005Pos[2] = { { -19, -27 }, { 13, -14 } };
const struct sprite_piece_pos gSpriteBank12Frame006Pos[2] = { { -17, -29 }, { 15, -16 } };
const struct sprite_piece_pos gSpriteBank12Frame007Pos[2] = { { -14, -30 }, { 18, -16 } };
const struct sprite_piece_pos gSpriteBank12Frame008Pos[2] = { { -11, -30 }, { 21, -12 } };
const struct sprite_piece_pos gSpriteBank12Frame009Pos[2] = { { -11, -28 }, { 21, -11 } };
const struct sprite_piece_pos gSpriteBank12Frame010Pos[2] = { { -13, -27 }, { 19, -11 } };
const struct sprite_piece_pos gSpriteBank12Frame011Pos[2] = { { -15, -26 }, { 17, -13 } };
const struct sprite_piece_pos gSpriteBank12Frame012Pos[2] = { { -15, -27 }, { 17, -11 } };
const struct sprite_piece_pos gSpriteBank12Frame013Pos[2] = { { -15, -27 }, { 17, -7 } };
const struct sprite_piece_pos gSpriteBank12Frame014Pos[2] = { { -15, -27 }, { 17, -1 } };
const struct sprite_piece_pos gSpriteBank12Frame015Pos[2] = { { -14, -27 }, { 18, 4 } };
const struct sprite_piece_pos gSpriteBank12Frame016Pos[1] = { { -14, -27 } };
const struct sprite_piece_pos gSpriteBank12Frame017Pos[2] = { { -15, -13 }, { -7, 19 } };
const struct sprite_piece_pos gSpriteBank12Frame018Pos[2] = { { -15, -14 }, { -7, 18 } };
const struct sprite_piece_pos gSpriteBank12Frame019Pos[2] = { { -14, -14 }, { -7, 18 } };
const struct sprite_piece_pos gSpriteBank12Frame020Pos[2] = { { -15, -14 }, { -7, 18 } };
const struct sprite_piece_pos gSpriteBank12Frame021Pos[2] = { { -15, -14 }, { -7, 18 } };
const struct sprite_piece_pos gSpriteBank12Frame022Pos[2] = { { -15, -15 }, { -7, 17 } };
const struct sprite_piece_pos gSpriteBank12Frame023Pos[2] = { { -15, -17 }, { -7, 15 } };
const struct sprite_piece_pos gSpriteBank12Frame024Pos[2] = { { -15, -20 }, { 17, 17 } };
const struct sprite_piece_pos gSpriteBank12Frame025Pos[1] = { { -15, -22 } };
const struct sprite_piece_pos gSpriteBank12Frame026Pos[1] = { { -14, -25 } };
const struct sprite_piece_pos gSpriteBank12Frame027Pos[1] = { { -15, -27 } };
const struct sprite_piece_pos gSpriteBank12Frame028Pos[1] = { { -14, -27 } };
const struct sprite_piece_pos gSpriteBank12Frame029Pos[1] = { { -15, -27 } };
const struct sprite_piece_pos gSpriteBank12Frame030Pos[1] = { { -14, -27 } };
const struct sprite_piece_pos gSpriteBank12Frame031Pos[1] = { { -14, -27 } };
const struct sprite_piece_pos gSpriteBank12Frame032Pos[1] = { { -14, -27 } };
const struct sprite_piece_pos gSpriteBank12Frame033Pos[1] = { { -15, -27 } };
const struct sprite_piece_pos gSpriteBank12Frame034Pos[1] = { { -14, -25 } };
const struct sprite_piece_pos gSpriteBank12Frame035Pos[1] = { { -15, -22 } };
const struct sprite_piece_pos gSpriteBank12Frame036Pos[2] = { { -15, -20 }, { 17, 17 } };
const struct sprite_piece_pos gSpriteBank12Frame037Pos[2] = { { -15, -17 }, { -7, 15 } };
const struct sprite_piece_pos gSpriteBank12Frame038Pos[2] = { { -15, -15 }, { -7, 17 } };
const struct sprite_piece_pos gSpriteBank12Frame039Pos[3] = { { -16, -13 }, { 16, 16 }, { -7, 19 } };
const struct sprite_piece_pos gSpriteBank12Frame040Pos[4] = { { -17, -14 }, { 15, 9 }, { -7, 18 }, { 9, 26 } };
const struct sprite_piece_pos gSpriteBank12Frame041Pos[4] = { { -17, -14 }, { 15, 4 }, { -7, 18 }, { 9, 26 } };
const struct sprite_piece_pos gSpriteBank12Frame042Pos[4] = { { -15, -13 }, { 17, 4 }, { -7, 19 }, { 9, 26 } };
const struct sprite_piece_pos gSpriteBank12Frame043Pos[4] = { { -14, -13 }, { 18, 4 }, { -7, 19 }, { 9, 26 } };
const struct sprite_piece_pos gSpriteBank12Frame044Pos[4] = { { -11, -13 }, { 21, 3 }, { -7, 19 }, { 9, 26 } };
const struct sprite_piece_pos gSpriteBank12Frame045Pos[4] = { { -10, -13 }, { 22, 3 }, { -7, 19 }, { 9, 26 } };
const struct sprite_piece_pos gSpriteBank12Frame046Pos[4] = { { -12, -14 }, { 20, 2 }, { -7, 18 }, { 9, 26 } };
const struct sprite_piece_pos gSpriteBank12Frame047Pos[4] = { { -14, -15 }, { 18, 4 }, { -7, 17 }, { 9, 26 } };
const struct sprite_piece_pos gSpriteBank12Frame048Pos[4] = { { -14, -14 }, { 18, 4 }, { -7, 18 }, { 9, 26 } };
const struct sprite_piece_pos gSpriteBank12Frame049Pos[4] = { { -15, -14 }, { 17, 5 }, { -7, 18 }, { 9, 26 } };
const struct sprite_piece_pos gSpriteBank12Frame050Pos[4] = { { -15, -14 }, { 17, 6 }, { -7, 18 }, { 9, 26 } };
const struct sprite_piece_pos gSpriteBank12Frame051Pos[4] = { { -13, -14 }, { 19, 9 }, { -7, 18 }, { 9, 26 } };
const struct sprite_piece_pos gSpriteBank12Frame052Pos[4] = { { -13, -14 }, { 19, 14 }, { -7, 18 }, { 9, 26 } };
const struct sprite_piece_pos gSpriteBank12Frame053Pos[2] = { { -14, -14 }, { -7, 18 } };
const struct sprite_piece_pos gSpriteBank12Frame054Pos[2] = { { -15, -14 }, { -7, 18 } };
const struct sprite_piece_pos gSpriteBank12Frame055Pos[2] = { { -4, -2 }, { 4, 0 } };
const struct sprite_piece_pos gSpriteBank12Frame056Pos[1] = { { -3, -2 } };

const u8 gSpriteBank12Frame000Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank12Frame001Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank12Frame002Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank12Frame003Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank12Frame004Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank12Frame005Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank12Frame006Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank12Frame007Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank12Frame008Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank12Frame009Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank12Frame010Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank12Frame011Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank12Frame012Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank12Frame013Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank12Frame014Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank12Frame015Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank12Frame016Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank12Frame017Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank12Frame018Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank12Frame019Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank12Frame020Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank12Frame021Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank12Frame022Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank12Frame023Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank12Frame024Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank12Frame025Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank12Frame026Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank12Frame027Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank12Frame028Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank12Frame029Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank12Frame030Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank12Frame031Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank12Frame032Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank12Frame033Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank12Frame034Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank12Frame035Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank12Frame036Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank12Frame037Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank12Frame038Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank12Frame039Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank12Frame040Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank12Frame041Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank12Frame042Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank12Frame043Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank12Frame044Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank12Frame045Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank12Frame046Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank12Frame047Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank12Frame048Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank12Frame049Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank12Frame050Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank12Frame051Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank12Frame052Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank12Frame053Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank12Frame054Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank12Frame055Pieces[2] = { SPRITE_PIECE(5, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank12Frame056Pieces[1] = { SPRITE_PIECE(5, 0) };

/* ---------------------------------------------------------------------- */
/* Bank 13: 2 animations, 19 frames, tiles in gSpriteBank13Tiles (SPRITE_TILES_BANK13). */

extern const u16 gSpriteBank13Anim00Seq[10];
extern const u16 gSpriteBank13Anim01Seq[9];
extern const struct sprite_frame_1box gSpriteBank13Frame000;
extern const struct sprite_frame_1box gSpriteBank13Frame001;
extern const struct sprite_frame_1box gSpriteBank13Frame002;
extern const struct sprite_frame_1box gSpriteBank13Frame003;
extern const struct sprite_frame_1box gSpriteBank13Frame004;
extern const struct sprite_frame_1box gSpriteBank13Frame005;
extern const struct sprite_frame_1box gSpriteBank13Frame006;
extern const struct sprite_frame_1box gSpriteBank13Frame007;
extern const struct sprite_frame_1box gSpriteBank13Frame008;
extern const struct sprite_frame_1box gSpriteBank13Frame009;
extern const struct sprite_frame_1box gSpriteBank13Frame010;
extern const struct sprite_frame_1box gSpriteBank13Frame011;
extern const struct sprite_frame_1box gSpriteBank13Frame012;
extern const struct sprite_frame_1box gSpriteBank13Frame013;
extern const struct sprite_frame_1box gSpriteBank13Frame014;
extern const struct sprite_frame_1box gSpriteBank13Frame015;
extern const struct sprite_frame_1box gSpriteBank13Frame016;
extern const struct sprite_frame_1box gSpriteBank13Frame017;
extern const struct sprite_frame_1box gSpriteBank13Frame018;
extern const struct sprite_piece_pos gSpriteBank13Frame000Pos[4];
extern const struct sprite_piece_pos gSpriteBank13Frame001Pos[4];
extern const struct sprite_piece_pos gSpriteBank13Frame002Pos[4];
extern const struct sprite_piece_pos gSpriteBank13Frame003Pos[4];
extern const struct sprite_piece_pos gSpriteBank13Frame004Pos[5];
extern const struct sprite_piece_pos gSpriteBank13Frame005Pos[5];
extern const struct sprite_piece_pos gSpriteBank13Frame006Pos[5];
extern const struct sprite_piece_pos gSpriteBank13Frame007Pos[4];
extern const struct sprite_piece_pos gSpriteBank13Frame008Pos[4];
extern const struct sprite_piece_pos gSpriteBank13Frame009Pos[4];
extern const struct sprite_piece_pos gSpriteBank13Frame010Pos[4];
extern const struct sprite_piece_pos gSpriteBank13Frame011Pos[3];
extern const struct sprite_piece_pos gSpriteBank13Frame012Pos[3];
extern const struct sprite_piece_pos gSpriteBank13Frame013Pos[3];
extern const struct sprite_piece_pos gSpriteBank13Frame014Pos[2];
extern const struct sprite_piece_pos gSpriteBank13Frame015Pos[3];
extern const struct sprite_piece_pos gSpriteBank13Frame016Pos[3];
extern const struct sprite_piece_pos gSpriteBank13Frame017Pos[3];
extern const struct sprite_piece_pos gSpriteBank13Frame018Pos[4];
extern const u8 gSpriteBank13Frame000Pieces[4];
extern const u8 gSpriteBank13Frame001Pieces[4];
extern const u8 gSpriteBank13Frame002Pieces[4];
extern const u8 gSpriteBank13Frame003Pieces[4];
extern const u8 gSpriteBank13Frame004Pieces[5];
extern const u8 gSpriteBank13Frame005Pieces[5];
extern const u8 gSpriteBank13Frame006Pieces[5];
extern const u8 gSpriteBank13Frame007Pieces[4];
extern const u8 gSpriteBank13Frame008Pieces[4];
extern const u8 gSpriteBank13Frame009Pieces[4];
extern const u8 gSpriteBank13Frame010Pieces[4];
extern const u8 gSpriteBank13Frame011Pieces[3];
extern const u8 gSpriteBank13Frame012Pieces[3];
extern const u8 gSpriteBank13Frame013Pieces[3];
extern const u8 gSpriteBank13Frame014Pieces[2];
extern const u8 gSpriteBank13Frame015Pieces[3];
extern const u8 gSpriteBank13Frame016Pieces[3];
extern const u8 gSpriteBank13Frame017Pieces[3];
extern const u8 gSpriteBank13Frame018Pieces[4];

const struct sprite_anim gSpriteBank13Anims[2] = {
    [0] = {
        .seq = gSpriteBank13Anim00Seq,
        .box = { { -25, -8, 50, 17 }, { -27, -9, 53, 18 } },
        .tileRecord = 21,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank13Anim00Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [1] = {
        .seq = gSpriteBank13Anim01Seq,
        .box = { { -25, -8, 50, 17 }, { -27, -9, 52, 20 } },
        .tileRecord = 21,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank13Anim01Seq),
        .flags = 0,
    },
};

const u16 gSpriteBank13Anim00Seq[10] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
};
const u16 gSpriteBank13Anim01Seq[9] = {
    10, 11, 12, 13, 14, 15, 16, 17, 18,
};

const struct sprite_frame *const gSpriteBank13Frames[19] = {
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
    SPRITE_FRAME(gSpriteBank13Frame000, SPRITE_TILES_BANK13 + 0x00000),
    { { -12, -7, 31, 11 } },
};
const struct sprite_frame_1box gSpriteBank13Frame001 = {
    SPRITE_FRAME(gSpriteBank13Frame001, SPRITE_TILES_BANK13 + 0x001c0),
    { { -13, -8, 32, 12 } },
};
const struct sprite_frame_1box gSpriteBank13Frame002 = {
    SPRITE_FRAME(gSpriteBank13Frame002, SPRITE_TILES_BANK13 + 0x00380),
    { { -14, -8, 30, 12 } },
};
const struct sprite_frame_1box gSpriteBank13Frame003 = {
    SPRITE_FRAME(gSpriteBank13Frame003, SPRITE_TILES_BANK13 + 0x00540),
    { { -15, -7, 31, 12 } },
};
const struct sprite_frame_1box gSpriteBank13Frame004 = {
    SPRITE_FRAME(gSpriteBank13Frame004, SPRITE_TILES_BANK13 + 0x00700),
    { { -12, -7, 28, 12 } },
};
const struct sprite_frame_1box gSpriteBank13Frame005 = {
    SPRITE_FRAME(gSpriteBank13Frame005, SPRITE_TILES_BANK13 + 0x00960),
    { { -10, -6, 28, 12 } },
};
const struct sprite_frame_1box gSpriteBank13Frame006 = {
    SPRITE_FRAME(gSpriteBank13Frame006, SPRITE_TILES_BANK13 + 0x00bc0),
    { { -10, -6, 25, 12 } },
};
const struct sprite_frame_1box gSpriteBank13Frame007 = {
    SPRITE_FRAME(gSpriteBank13Frame007, SPRITE_TILES_BANK13 + 0x00e20),
    { { -11, -6, 30, 11 } },
};
const struct sprite_frame_1box gSpriteBank13Frame008 = {
    SPRITE_FRAME(gSpriteBank13Frame008, SPRITE_TILES_BANK13 + 0x01000),
    { { -10, -6, 27, 11 } },
};
const struct sprite_frame_1box gSpriteBank13Frame009 = {
    SPRITE_FRAME(gSpriteBank13Frame009, SPRITE_TILES_BANK13 + 0x011c0),
    { { -10, -7, 28, 12 } },
};
const struct sprite_frame_1box gSpriteBank13Frame010 = {
    SPRITE_FRAME(gSpriteBank13Frame010, SPRITE_TILES_BANK13 + 0x01380),
    { { -15, -7, 34, 16 } },
};
const struct sprite_frame_1box gSpriteBank13Frame011 = {
    SPRITE_FRAME(gSpriteBank13Frame011, SPRITE_TILES_BANK13 + 0x01540),
    { { -15, -8, 32, 16 } },
};
const struct sprite_frame_1box gSpriteBank13Frame012 = {
    SPRITE_FRAME(gSpriteBank13Frame012, SPRITE_TILES_BANK13 + 0x016e0),
    { { -13, -7, 32, 12 } },
};
const struct sprite_frame_1box gSpriteBank13Frame013 = {
    SPRITE_FRAME(gSpriteBank13Frame013, SPRITE_TILES_BANK13 + 0x01880),
    { { -13, -6, 28, 12 } },
};
const struct sprite_frame_1box gSpriteBank13Frame014 = {
    SPRITE_FRAME(gSpriteBank13Frame014, SPRITE_TILES_BANK13 + 0x019c0),
    { { -12, -6, 22, 12 } },
};
const struct sprite_frame_1box gSpriteBank13Frame015 = {
    SPRITE_FRAME(gSpriteBank13Frame015, SPRITE_TILES_BANK13 + 0x01ae0),
    { { -16, -7, 25, 12 } },
};
const struct sprite_frame_1box gSpriteBank13Frame016 = {
    SPRITE_FRAME(gSpriteBank13Frame016, SPRITE_TILES_BANK13 + 0x01c20),
    { { -15, -8, 23, 12 } },
};
const struct sprite_frame_1box gSpriteBank13Frame017 = {
    SPRITE_FRAME(gSpriteBank13Frame017, SPRITE_TILES_BANK13 + 0x01d80),
    { { -17, -8, 26, 12 } },
};
const struct sprite_frame_1box gSpriteBank13Frame018 = {
    SPRITE_FRAME(gSpriteBank13Frame018, SPRITE_TILES_BANK13 + 0x01f20),
    { { -19, -7, 30, 12 } },
};

const struct sprite_piece_pos gSpriteBank13Frame000Pos[4] = { { -25, -8 }, { 7, -5 }, { 23, -4 }, { 13, 8 } };
const struct sprite_piece_pos gSpriteBank13Frame001Pos[4] = { { -26, -9 }, { 6, -7 }, { 22, -4 }, { 9, 7 } };
const struct sprite_piece_pos gSpriteBank13Frame002Pos[4] = { { -26, -9 }, { 6, -7 }, { 22, -3 }, { 7, 7 } };
const struct sprite_piece_pos gSpriteBank13Frame003Pos[4] = { { -26, -8 }, { 6, -6 }, { 22, -3 }, { 4, 8 } };
const struct sprite_piece_pos gSpriteBank13Frame004Pos[5] = { { -25, -8 }, { 7, -6 }, { 23, -8 }, { -9, 8 }, { 23, 8 } };
const struct sprite_piece_pos gSpriteBank13Frame005Pos[5] = { { -26, -8 }, { 6, -6 }, { 22, -1 }, { -10, 8 }, { 22, 8 } };
const struct sprite_piece_pos gSpriteBank13Frame006Pos[5] = { { -27, -9 }, { 5, -7 }, { 21, -4 }, { -12, 7 }, { 20, 7 } };
const struct sprite_piece_pos gSpriteBank13Frame007Pos[4] = { { -27, -9 }, { 5, -7 }, { 21, -3 }, { -14, 7 } };
const struct sprite_piece_pos gSpriteBank13Frame008Pos[4] = { { -25, -8 }, { 7, -6 }, { 23, -4 }, { -14, 8 } };
const struct sprite_piece_pos gSpriteBank13Frame009Pos[4] = { { -24, -8 }, { 8, -6 }, { 24, -4 }, { 16, 8 } };
const struct sprite_piece_pos gSpriteBank13Frame010Pos[4] = { { -23, -8 }, { 9, -5 }, { 25, -1 }, { 12, 8 } };
const struct sprite_piece_pos gSpriteBank13Frame011Pos[3] = { { -21, -9 }, { 11, -3 }, { 5, 7 } };
const struct sprite_piece_pos gSpriteBank13Frame012Pos[3] = { { -21, -9 }, { 11, -3 }, { -3, 7 } };
const struct sprite_piece_pos gSpriteBank13Frame013Pos[3] = { { -19, -8 }, { 13, 1 }, { -12, 8 } };
const struct sprite_piece_pos gSpriteBank13Frame014Pos[2] = { { -17, -8 }, { 4, 8 } };
const struct sprite_piece_pos gSpriteBank13Frame015Pos[3] = { { -21, -8 }, { 11, -2 }, { -2, 8 } };
const struct sprite_piece_pos gSpriteBank13Frame016Pos[3] = { { -23, -9 }, { 9, -6 }, { -3, 7 } };
const struct sprite_piece_pos gSpriteBank13Frame017Pos[3] = { { -25, -9 }, { 7, -6 }, { -5, 7 } };
const struct sprite_piece_pos gSpriteBank13Frame018Pos[4] = { { -27, -8 }, { 5, -5 }, { 21, -3 }, { -5, 8 } };

const u8 gSpriteBank13Frame000Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank13Frame001Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank13Frame002Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank13Frame003Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank13Frame004Pieces[5] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank13Frame005Pieces[5] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank13Frame006Pieces[5] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank13Frame007Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank13Frame008Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank13Frame009Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank13Frame010Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank13Frame011Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank13Frame012Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank13Frame013Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank13Frame014Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank13Frame015Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank13Frame016Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank13Frame017Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank13Frame018Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };

/* ---------------------------------------------------------------------- */
/* Bank 14: 2 animations, 23 frames, tiles in gSpriteBank14Tiles (SPRITE_TILES_BANK14). */

extern const u16 gSpriteBank14Anim00Seq[12];
extern const u16 gSpriteBank14Anim01Seq[11];
extern const struct sprite_frame_2box gSpriteBank14Frame000;
extern const struct sprite_frame_2box gSpriteBank14Frame001;
extern const struct sprite_frame_2box gSpriteBank14Frame002;
extern const struct sprite_frame_2box gSpriteBank14Frame003;
extern const struct sprite_frame_2box gSpriteBank14Frame004;
extern const struct sprite_frame_2box gSpriteBank14Frame005;
extern const struct sprite_frame_2box gSpriteBank14Frame006;
extern const struct sprite_frame_2box gSpriteBank14Frame007;
extern const struct sprite_frame_2box gSpriteBank14Frame008;
extern const struct sprite_frame_2box gSpriteBank14Frame009;
extern const struct sprite_frame_2box gSpriteBank14Frame010;
extern const struct sprite_frame_2box gSpriteBank14Frame011;
extern const struct sprite_frame_2box gSpriteBank14Frame012;
extern const struct sprite_frame_2box gSpriteBank14Frame013;
extern const struct sprite_frame_2box gSpriteBank14Frame014;
extern const struct sprite_frame_2box gSpriteBank14Frame015;
extern const struct sprite_frame_2box gSpriteBank14Frame016;
extern const struct sprite_frame_2box gSpriteBank14Frame017;
extern const struct sprite_frame_2box gSpriteBank14Frame018;
extern const struct sprite_frame_2box gSpriteBank14Frame019;
extern const struct sprite_frame_2box gSpriteBank14Frame020;
extern const struct sprite_frame_2box gSpriteBank14Frame021;
extern const struct sprite_frame_2box gSpriteBank14Frame022;
extern const struct sprite_piece_pos gSpriteBank14Frame000Pos[5];
extern const struct sprite_piece_pos gSpriteBank14Frame001Pos[5];
extern const struct sprite_piece_pos gSpriteBank14Frame002Pos[4];
extern const struct sprite_piece_pos gSpriteBank14Frame003Pos[5];
extern const struct sprite_piece_pos gSpriteBank14Frame004Pos[6];
extern const struct sprite_piece_pos gSpriteBank14Frame005Pos[3];
extern const struct sprite_piece_pos gSpriteBank14Frame006Pos[3];
extern const struct sprite_piece_pos gSpriteBank14Frame007Pos[3];
extern const struct sprite_piece_pos gSpriteBank14Frame008Pos[3];
extern const struct sprite_piece_pos gSpriteBank14Frame009Pos[3];
extern const struct sprite_piece_pos gSpriteBank14Frame010Pos[5];
extern const struct sprite_piece_pos gSpriteBank14Frame011Pos[3];
extern const struct sprite_piece_pos gSpriteBank14Frame012Pos[5];
extern const struct sprite_piece_pos gSpriteBank14Frame013Pos[5];
extern const struct sprite_piece_pos gSpriteBank14Frame014Pos[5];
extern const struct sprite_piece_pos gSpriteBank14Frame015Pos[4];
extern const struct sprite_piece_pos gSpriteBank14Frame016Pos[4];
extern const struct sprite_piece_pos gSpriteBank14Frame017Pos[3];
extern const struct sprite_piece_pos gSpriteBank14Frame018Pos[4];
extern const struct sprite_piece_pos gSpriteBank14Frame019Pos[4];
extern const struct sprite_piece_pos gSpriteBank14Frame020Pos[5];
extern const struct sprite_piece_pos gSpriteBank14Frame021Pos[4];
extern const struct sprite_piece_pos gSpriteBank14Frame022Pos[5];
extern const u8 gSpriteBank14Frame000Pieces[5];
extern const u8 gSpriteBank14Frame001Pieces[5];
extern const u8 gSpriteBank14Frame002Pieces[4];
extern const u8 gSpriteBank14Frame003Pieces[5];
extern const u8 gSpriteBank14Frame004Pieces[6];
extern const u8 gSpriteBank14Frame005Pieces[3];
extern const u8 gSpriteBank14Frame006Pieces[3];
extern const u8 gSpriteBank14Frame007Pieces[3];
extern const u8 gSpriteBank14Frame008Pieces[3];
extern const u8 gSpriteBank14Frame009Pieces[3];
extern const u8 gSpriteBank14Frame010Pieces[5];
extern const u8 gSpriteBank14Frame011Pieces[3];
extern const u8 gSpriteBank14Frame012Pieces[5];
extern const u8 gSpriteBank14Frame013Pieces[5];
extern const u8 gSpriteBank14Frame014Pieces[5];
extern const u8 gSpriteBank14Frame015Pieces[4];
extern const u8 gSpriteBank14Frame016Pieces[4];
extern const u8 gSpriteBank14Frame017Pieces[3];
extern const u8 gSpriteBank14Frame018Pieces[4];
extern const u8 gSpriteBank14Frame019Pieces[4];
extern const u8 gSpriteBank14Frame020Pieces[5];
extern const u8 gSpriteBank14Frame021Pieces[4];
extern const u8 gSpriteBank14Frame022Pieces[5];

const struct sprite_anim gSpriteBank14Anims[2] = {
    [0] = {
        .seq = gSpriteBank14Anim00Seq,
        .box = { { -22, -21, 44, 42 }, { -25, -25, 50, 47 } },
        .tileRecord = 22,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank14Anim00Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [1] = {
        .seq = gSpriteBank14Anim01Seq,
        .box = { { -22, -21, 44, 42 }, { -22, -25, 44, 47 } },
        .tileRecord = 22,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank14Anim01Seq),
        .flags = 0,
    },
};

const u16 gSpriteBank14Anim00Seq[12] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
};
const u16 gSpriteBank14Anim01Seq[11] = {
    12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
};

const struct sprite_frame *const gSpriteBank14Frames[23] = {
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
    SPRITE_FRAME(gSpriteBank14Frame000, SPRITE_TILES_BANK14 + 0x00000),
    { { -10, 5, 24, 14 }, { -16, -12, 41, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame001 = {
    SPRITE_FRAME(gSpriteBank14Frame001, SPRITE_TILES_BANK14 + 0x003e0),
    { { -10, 5, 24, 14 }, { -16, -12, 41, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame002 = {
    SPRITE_FRAME(gSpriteBank14Frame002, SPRITE_TILES_BANK14 + 0x007a0),
    { { -10, 5, 24, 14 }, { -16, -12, 41, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame003 = {
    SPRITE_FRAME(gSpriteBank14Frame003, SPRITE_TILES_BANK14 + 0x00b40),
    { { -10, 5, 24, 14 }, { -16, -12, 41, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame004 = {
    SPRITE_FRAME(gSpriteBank14Frame004, SPRITE_TILES_BANK14 + 0x00f20),
    { { -10, 5, 24, 14 }, { -16, -12, 41, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame005 = {
    SPRITE_FRAME(gSpriteBank14Frame005, SPRITE_TILES_BANK14 + 0x01320),
    { { -10, 5, 24, 14 }, { -16, -12, 41, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame006 = {
    SPRITE_FRAME(gSpriteBank14Frame006, SPRITE_TILES_BANK14 + 0x01640),
    { { -10, 5, 24, 14 }, { -16, -12, 41, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame007 = {
    SPRITE_FRAME(gSpriteBank14Frame007, SPRITE_TILES_BANK14 + 0x01960),
    { { -10, 5, 24, 14 }, { -16, -12, 41, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame008 = {
    SPRITE_FRAME(gSpriteBank14Frame008, SPRITE_TILES_BANK14 + 0x01c80),
    { { -10, 5, 24, 14 }, { -16, -12, 41, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame009 = {
    SPRITE_FRAME(gSpriteBank14Frame009, SPRITE_TILES_BANK14 + 0x01fa0),
    { { -10, 5, 24, 14 }, { -16, -12, 41, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame010 = {
    SPRITE_FRAME(gSpriteBank14Frame010, SPRITE_TILES_BANK14 + 0x022c0),
    { { -10, 5, 24, 14 }, { -16, -12, 41, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame011 = {
    SPRITE_FRAME(gSpriteBank14Frame011, SPRITE_TILES_BANK14 + 0x026a0),
    { { -10, 5, 24, 14 }, { -16, -12, 41, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame012 = {
    SPRITE_FRAME(gSpriteBank14Frame012, SPRITE_TILES_BANK14 + 0x00000),
    { { -12, 4, 22, 16 }, { -19, -13, 40, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame013 = {
    SPRITE_FRAME(gSpriteBank14Frame013, SPRITE_TILES_BANK14 + 0x02b40),
    { { -12, 4, 22, 16 }, { -19, -13, 40, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame014 = {
    SPRITE_FRAME(gSpriteBank14Frame014, SPRITE_TILES_BANK14 + 0x02f00),
    { { -12, 4, 22, 16 }, { -19, -13, 40, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame015 = {
    SPRITE_FRAME(gSpriteBank14Frame015, SPRITE_TILES_BANK14 + 0x032c0),
    { { -12, 4, 22, 16 }, { -19, -13, 40, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame016 = {
    SPRITE_FRAME(gSpriteBank14Frame016, SPRITE_TILES_BANK14 + 0x03620),
    { { -12, 4, 22, 16 }, { -19, -13, 40, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame017 = {
    SPRITE_FRAME(gSpriteBank14Frame017, SPRITE_TILES_BANK14 + 0x03980),
    { { -12, 4, 22, 16 }, { -19, -13, 40, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame018 = {
    SPRITE_FRAME(gSpriteBank14Frame018, SPRITE_TILES_BANK14 + 0x03ca0),
    { { -12, 4, 22, 16 }, { -19, -13, 40, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame019 = {
    SPRITE_FRAME(gSpriteBank14Frame019, SPRITE_TILES_BANK14 + 0x03fe0),
    { { -12, 4, 22, 16 }, { -19, -13, 40, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame020 = {
    SPRITE_FRAME(gSpriteBank14Frame020, SPRITE_TILES_BANK14 + 0x04320),
    { { -12, 4, 22, 16 }, { -19, -13, 40, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame021 = {
    SPRITE_FRAME(gSpriteBank14Frame021, SPRITE_TILES_BANK14 + 0x046a0),
    { { -12, 4, 22, 16 }, { -19, -13, 40, 13 } },
};
const struct sprite_frame_2box gSpriteBank14Frame022 = {
    SPRITE_FRAME(gSpriteBank14Frame022, SPRITE_TILES_BANK14 + 0x049e0),
    { { -12, 4, 22, 16 }, { -19, -13, 40, 13 } },
};

const struct sprite_piece_pos gSpriteBank14Frame000Pos[5] = { { -16, -21 }, { 10, -19 }, { 18, -4 }, { -22, 11 }, { 10, 20 } };
const struct sprite_piece_pos gSpriteBank14Frame001Pos[5] = { { -16, -22 }, { 12, -16 }, { 20, -4 }, { -20, 10 }, { 12, 20 } };
const struct sprite_piece_pos gSpriteBank14Frame002Pos[4] = { { -17, -24 }, { 11, -22 }, { 19, -6 }, { -21, 8 } };
const struct sprite_piece_pos gSpriteBank14Frame003Pos[5] = { { -17, -25 }, { 10, -23 }, { 18, -8 }, { -22, 7 }, { 10, 7 } };
const struct sprite_piece_pos gSpriteBank14Frame004Pos[6] = { { -16, -24 }, { 10, -22 }, { 18, -7 }, { -22, 8 }, { 10, 8 }, { 18, 8 } };
const struct sprite_piece_pos gSpriteBank14Frame005Pos[3] = { { -15, -22 }, { 17, -5 }, { -13, 10 } };
const struct sprite_piece_pos gSpriteBank14Frame006Pos[3] = { { -14, -21 }, { 18, -5 }, { -9, 11 } };
const struct sprite_piece_pos gSpriteBank14Frame007Pos[3] = { { -14, -22 }, { 18, -6 }, { -7, 10 } };
const struct sprite_piece_pos gSpriteBank14Frame008Pos[3] = { { -13, -24 }, { 19, -7 }, { -10, 8 } };
const struct sprite_piece_pos gSpriteBank14Frame009Pos[3] = { { -14, -25 }, { 17, -8 }, { -15, 7 } };
const struct sprite_piece_pos gSpriteBank14Frame010Pos[5] = { { -14, -24 }, { 11, -21 }, { 19, -7 }, { -21, 8 }, { 11, 9 } };
const struct sprite_piece_pos gSpriteBank14Frame011Pos[3] = { { -15, -22 }, { -25, 10 }, { 7, 11 } };
const struct sprite_piece_pos gSpriteBank14Frame012Pos[5] = { { -16, -21 }, { 10, -19 }, { 18, -4 }, { -22, 11 }, { 10, 20 } };
const struct sprite_piece_pos gSpriteBank14Frame013Pos[5] = { { -16, -22 }, { 11, -20 }, { 19, -4 }, { -21, 10 }, { 11, 20 } };
const struct sprite_piece_pos gSpriteBank14Frame014Pos[5] = { { -18, -24 }, { 11, -13 }, { 19, -4 }, { -21, 8 }, { 11, 11 } };
const struct sprite_piece_pos gSpriteBank14Frame015Pos[4] = { { -19, -25 }, { 12, -8 }, { -20, 7 }, { 12, 7 } };
const struct sprite_piece_pos gSpriteBank14Frame016Pos[4] = { { -19, -24 }, { 13, -6 }, { -13, 8 }, { 19, 8 } };
const struct sprite_piece_pos gSpriteBank14Frame017Pos[3] = { { -17, -22 }, { 15, -3 }, { -13, 10 } };
const struct sprite_piece_pos gSpriteBank14Frame018Pos[4] = { { -16, -21 }, { 16, -5 }, { -14, 11 }, { 18, 11 } };
const struct sprite_piece_pos gSpriteBank14Frame019Pos[4] = { { -15, -22 }, { 17, -6 }, { -15, 10 }, { 17, 15 } };
const struct sprite_piece_pos gSpriteBank14Frame020Pos[5] = { { -15, -23 }, { 17, -9 }, { 17, 7 }, { -15, 9 }, { 17, 9 } };
const struct sprite_piece_pos gSpriteBank14Frame021Pos[4] = { { -18, -25 }, { 14, -11 }, { -13, 7 }, { 19, 15 } };
const struct sprite_piece_pos gSpriteBank14Frame022Pos[5] = { { -19, -24 }, { 13, -10 }, { -19, 8 }, { 13, 10 }, { 21, 10 } };

const u8 gSpriteBank14Frame000Pieces[5] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank14Frame001Pieces[5] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank14Frame002Pieces[4] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank14Frame003Pieces[5] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank14Frame004Pieces[6] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank14Frame005Pieces[3] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank14Frame006Pieces[3] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank14Frame007Pieces[3] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank14Frame008Pieces[3] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank14Frame009Pieces[3] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank14Frame010Pieces[5] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank14Frame011Pieces[3] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank14Frame012Pieces[5] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank14Frame013Pieces[5] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank14Frame014Pieces[5] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank14Frame015Pieces[4] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank14Frame016Pieces[4] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank14Frame017Pieces[3] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank14Frame018Pieces[4] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank14Frame019Pieces[4] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank14Frame020Pieces[5] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank14Frame021Pieces[4] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank14Frame022Pieces[5] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };

/* ---------------------------------------------------------------------- */
/* Bank 15: 5 animations, 58 frames, tiles in gSpriteBank15Tiles (SPRITE_TILES_BANK15). */

extern const u16 gSpriteBank15Anim00Seq[15];
extern const u16 gSpriteBank15Anim01Seq[4];
extern const u16 gSpriteBank15Anim02Seq[9];
extern const u16 gSpriteBank15Anim03Seq[15];
extern const u16 gSpriteBank15Anim04Seq[17];
extern const struct sprite_frame_1box gSpriteBank15Frame000;
extern const struct sprite_frame_1box gSpriteBank15Frame001;
extern const struct sprite_frame_1box gSpriteBank15Frame002;
extern const struct sprite_frame_1box gSpriteBank15Frame003;
extern const struct sprite_frame_1box gSpriteBank15Frame004;
extern const struct sprite_frame_1box gSpriteBank15Frame005;
extern const struct sprite_frame_1box gSpriteBank15Frame006;
extern const struct sprite_frame_1box gSpriteBank15Frame007;
extern const struct sprite_frame_1box gSpriteBank15Frame008;
extern const struct sprite_frame_1box gSpriteBank15Frame009;
extern const struct sprite_frame_1box gSpriteBank15Frame010;
extern const struct sprite_frame_1box gSpriteBank15Frame011;
extern const struct sprite_frame_1box gSpriteBank15Frame012;
extern const struct sprite_frame_1box gSpriteBank15Frame013;
extern const struct sprite_frame_1box gSpriteBank15Frame014;
extern const struct sprite_frame_1box gSpriteBank15Frame015;
extern const struct sprite_frame_1box gSpriteBank15Frame016;
extern const struct sprite_frame_1box gSpriteBank15Frame017;
extern const struct sprite_frame_1box gSpriteBank15Frame018;
extern const struct sprite_frame_1box gSpriteBank15Frame019;
extern const struct sprite_frame_1box gSpriteBank15Frame020;
extern const struct sprite_frame_1box gSpriteBank15Frame021;
extern const struct sprite_frame_1box gSpriteBank15Frame022;
extern const struct sprite_frame_1box gSpriteBank15Frame023;
extern const struct sprite_frame_1box gSpriteBank15Frame024;
extern const struct sprite_frame_1box gSpriteBank15Frame025;
extern const struct sprite_frame_1box gSpriteBank15Frame026;
extern const struct sprite_frame_1box gSpriteBank15Frame027;
extern const struct sprite_frame_1box gSpriteBank15Frame028;
extern const struct sprite_frame_1box gSpriteBank15Frame029;
extern const struct sprite_frame_1box gSpriteBank15Frame030;
extern const struct sprite_frame_1box gSpriteBank15Frame031;
extern const struct sprite_frame_1box gSpriteBank15Frame032;
extern const struct sprite_frame_1box gSpriteBank15Frame033;
extern const struct sprite_frame_1box gSpriteBank15Frame034;
extern const struct sprite_frame_1box gSpriteBank15Frame035;
extern const struct sprite_frame_1box gSpriteBank15Frame036;
extern const struct sprite_frame_1box gSpriteBank15Frame037;
extern const struct sprite_frame_1box gSpriteBank15Frame038;
extern const struct sprite_frame_1box gSpriteBank15Frame039;
extern const struct sprite_frame_1box gSpriteBank15Frame040;
extern const struct sprite_frame_1box gSpriteBank15Frame041;
extern const struct sprite_frame_1box gSpriteBank15Frame042;
extern const struct sprite_frame_1box gSpriteBank15Frame043;
extern const struct sprite_frame_1box gSpriteBank15Frame044;
extern const struct sprite_frame_1box gSpriteBank15Frame045;
extern const struct sprite_frame_1box gSpriteBank15Frame046;
extern const struct sprite_frame_1box gSpriteBank15Frame047;
extern const struct sprite_frame_1box gSpriteBank15Frame048;
extern const struct sprite_frame_1box gSpriteBank15Frame049;
extern const struct sprite_frame_1box gSpriteBank15Frame050;
extern const struct sprite_frame_1box gSpriteBank15Frame051;
extern const struct sprite_frame_1box gSpriteBank15Frame052;
extern const struct sprite_frame_1box gSpriteBank15Frame053;
extern const struct sprite_frame_1box gSpriteBank15Frame054;
extern const struct sprite_frame_1box gSpriteBank15Frame055;
extern const struct sprite_frame_1box gSpriteBank15Frame056;
extern const struct sprite_frame_1box gSpriteBank15Frame057;
extern const struct sprite_piece_pos gSpriteBank15Frame000Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame001Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame002Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame003Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame004Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame005Pos[1];
extern const struct sprite_piece_pos gSpriteBank15Frame006Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame007Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame008Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame009Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame010Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame011Pos[1];
extern const struct sprite_piece_pos gSpriteBank15Frame012Pos[1];
extern const struct sprite_piece_pos gSpriteBank15Frame013Pos[1];
extern const struct sprite_piece_pos gSpriteBank15Frame014Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame015Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame016Pos[1];
extern const struct sprite_piece_pos gSpriteBank15Frame017Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame018Pos[1];
extern const struct sprite_piece_pos gSpriteBank15Frame019Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame020Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame021Pos[1];
extern const struct sprite_piece_pos gSpriteBank15Frame022Pos[1];
extern const struct sprite_piece_pos gSpriteBank15Frame023Pos[1];
extern const struct sprite_piece_pos gSpriteBank15Frame024Pos[1];
extern const struct sprite_piece_pos gSpriteBank15Frame025Pos[1];
extern const struct sprite_piece_pos gSpriteBank15Frame026Pos[1];
extern const struct sprite_piece_pos gSpriteBank15Frame027Pos[1];
extern const struct sprite_piece_pos gSpriteBank15Frame028Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame029Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame030Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame031Pos[1];
extern const struct sprite_piece_pos gSpriteBank15Frame032Pos[1];
extern const struct sprite_piece_pos gSpriteBank15Frame033Pos[1];
extern const struct sprite_piece_pos gSpriteBank15Frame034Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame035Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame036Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame037Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame038Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame039Pos[1];
extern const struct sprite_piece_pos gSpriteBank15Frame040Pos[1];
extern const struct sprite_piece_pos gSpriteBank15Frame041Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame042Pos[1];
extern const struct sprite_piece_pos gSpriteBank15Frame043Pos[1];
extern const struct sprite_piece_pos gSpriteBank15Frame044Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame045Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame046Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame047Pos[3];
extern const struct sprite_piece_pos gSpriteBank15Frame048Pos[3];
extern const struct sprite_piece_pos gSpriteBank15Frame049Pos[3];
extern const struct sprite_piece_pos gSpriteBank15Frame050Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame051Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame052Pos[3];
extern const struct sprite_piece_pos gSpriteBank15Frame053Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame054Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame055Pos[2];
extern const struct sprite_piece_pos gSpriteBank15Frame056Pos[3];
extern const struct sprite_piece_pos gSpriteBank15Frame057Pos[2];
extern const u8 gSpriteBank15Frame000Pieces[2];
extern const u8 gSpriteBank15Frame001Pieces[2];
extern const u8 gSpriteBank15Frame002Pieces[2];
extern const u8 gSpriteBank15Frame003Pieces[2];
extern const u8 gSpriteBank15Frame004Pieces[2];
extern const u8 gSpriteBank15Frame005Pieces[1];
extern const u8 gSpriteBank15Frame006Pieces[2];
extern const u8 gSpriteBank15Frame007Pieces[2];
extern const u8 gSpriteBank15Frame008Pieces[2];
extern const u8 gSpriteBank15Frame009Pieces[2];
extern const u8 gSpriteBank15Frame010Pieces[2];
extern const u8 gSpriteBank15Frame011Pieces[1];
extern const u8 gSpriteBank15Frame012Pieces[1];
extern const u8 gSpriteBank15Frame013Pieces[1];
extern const u8 gSpriteBank15Frame014Pieces[2];
extern const u8 gSpriteBank15Frame015Pieces[2];
extern const u8 gSpriteBank15Frame016Pieces[1];
extern const u8 gSpriteBank15Frame017Pieces[2];
extern const u8 gSpriteBank15Frame018Pieces[1];
extern const u8 gSpriteBank15Frame019Pieces[2];
extern const u8 gSpriteBank15Frame020Pieces[2];
extern const u8 gSpriteBank15Frame021Pieces[1];
extern const u8 gSpriteBank15Frame022Pieces[1];
extern const u8 gSpriteBank15Frame023Pieces[1];
extern const u8 gSpriteBank15Frame024Pieces[1];
extern const u8 gSpriteBank15Frame025Pieces[1];
extern const u8 gSpriteBank15Frame026Pieces[1];
extern const u8 gSpriteBank15Frame027Pieces[1];
extern const u8 gSpriteBank15Frame028Pieces[2];
extern const u8 gSpriteBank15Frame029Pieces[2];
extern const u8 gSpriteBank15Frame030Pieces[2];
extern const u8 gSpriteBank15Frame031Pieces[1];
extern const u8 gSpriteBank15Frame032Pieces[1];
extern const u8 gSpriteBank15Frame033Pieces[1];
extern const u8 gSpriteBank15Frame034Pieces[2];
extern const u8 gSpriteBank15Frame035Pieces[2];
extern const u8 gSpriteBank15Frame036Pieces[2];
extern const u8 gSpriteBank15Frame037Pieces[2];
extern const u8 gSpriteBank15Frame038Pieces[2];
extern const u8 gSpriteBank15Frame039Pieces[1];
extern const u8 gSpriteBank15Frame040Pieces[1];
extern const u8 gSpriteBank15Frame041Pieces[2];
extern const u8 gSpriteBank15Frame042Pieces[1];
extern const u8 gSpriteBank15Frame043Pieces[1];
extern const u8 gSpriteBank15Frame044Pieces[2];
extern const u8 gSpriteBank15Frame045Pieces[2];
extern const u8 gSpriteBank15Frame046Pieces[2];
extern const u8 gSpriteBank15Frame047Pieces[3];
extern const u8 gSpriteBank15Frame048Pieces[3];
extern const u8 gSpriteBank15Frame049Pieces[3];
extern const u8 gSpriteBank15Frame050Pieces[2];
extern const u8 gSpriteBank15Frame051Pieces[2];
extern const u8 gSpriteBank15Frame052Pieces[3];
extern const u8 gSpriteBank15Frame053Pieces[2];
extern const u8 gSpriteBank15Frame054Pieces[2];
extern const u8 gSpriteBank15Frame055Pieces[2];
extern const u8 gSpriteBank15Frame056Pieces[3];
extern const u8 gSpriteBank15Frame057Pieces[2];

const struct sprite_anim gSpriteBank15Anims[5] = {
    [0] = {
        .seq = gSpriteBank15Anim00Seq,
        .box = { { -10, -13, 20, 27 }, { -14, -15, 25, 30 } },
        .tileRecord = 23,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank15Anim00Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [1] = {
        .seq = gSpriteBank15Anim01Seq,
        .box = { { -10, -13, 20, 27 }, { -17, -15, 33, 30 } },
        .tileRecord = 23,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank15Anim01Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [2] = {
        .seq = gSpriteBank15Anim02Seq,
        .box = { { -10, -13, 20, 27 }, { -16, -15, 29, 30 } },
        .tileRecord = 23,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank15Anim02Seq),
        .flags = 0,
    },
    [3] = {
        .seq = gSpriteBank15Anim03Seq,
        .box = { { -10, -13, 20, 27 }, { -14, -15, 25, 30 } },
        .tileRecord = 23,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank15Anim03Seq),
        .flags = 0,
    },
    [4] = {
        .seq = gSpriteBank15Anim04Seq,
        .box = { { -10, -13, 20, 27 }, { -14, -15, 27, 30 } },
        .tileRecord = 23,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank15Anim04Seq),
        .flags = 0,
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

const struct sprite_frame *const gSpriteBank15Frames[58] = {
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
    SPRITE_FRAME(gSpriteBank15Frame000, SPRITE_TILES_BANK15 + 0x00000),
    { { -6, -13, 12, 27 } },
};
const struct sprite_frame_1box gSpriteBank15Frame001 = {
    SPRITE_FRAME(gSpriteBank15Frame001, SPRITE_TILES_BANK15 + 0x00180),
    { { -6, -13, 12, 27 } },
};
const struct sprite_frame_1box gSpriteBank15Frame002 = {
    SPRITE_FRAME(gSpriteBank15Frame002, SPRITE_TILES_BANK15 + 0x00300),
    { { -6, -14, 11, 27 } },
};
const struct sprite_frame_1box gSpriteBank15Frame003 = {
    SPRITE_FRAME(gSpriteBank15Frame003, SPRITE_TILES_BANK15 + 0x00480),
    { { -7, -15, 11, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame004 = {
    SPRITE_FRAME(gSpriteBank15Frame004, SPRITE_TILES_BANK15 + 0x00600),
    { { -9, -15, 12, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame005 = {
    SPRITE_FRAME(gSpriteBank15Frame005, SPRITE_TILES_BANK15 + 0x00780),
    { { -6, -12, 12, 23 } },
};
const struct sprite_frame_1box gSpriteBank15Frame006 = {
    SPRITE_FRAME(gSpriteBank15Frame006, SPRITE_TILES_BANK15 + 0x00980),
    { { -6, -12, 12, 23 } },
};
const struct sprite_frame_1box gSpriteBank15Frame007 = {
    SPRITE_FRAME(gSpriteBank15Frame007, SPRITE_TILES_BANK15 + 0x00b00),
    { { -8, -14, 12, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame008 = {
    SPRITE_FRAME(gSpriteBank15Frame008, SPRITE_TILES_BANK15 + 0x00c80),
    { { -8, -14, 12, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame009 = {
    SPRITE_FRAME(gSpriteBank15Frame009, SPRITE_TILES_BANK15 + 0x00e00),
    { { -8, -14, 12, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame010 = {
    SPRITE_FRAME(gSpriteBank15Frame010, SPRITE_TILES_BANK15 + 0x00f80),
    { { -9, -15, 13, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame011 = {
    SPRITE_FRAME(gSpriteBank15Frame011, SPRITE_TILES_BANK15 + 0x01100),
    { { -9, -15, 15, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame012 = {
    SPRITE_FRAME(gSpriteBank15Frame012, SPRITE_TILES_BANK15 + 0x01300),
    { { -9, -15, 16, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame013 = {
    SPRITE_FRAME(gSpriteBank15Frame013, SPRITE_TILES_BANK15 + 0x01500),
    { { -8, -15, 15, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame014 = {
    SPRITE_FRAME(gSpriteBank15Frame014, SPRITE_TILES_BANK15 + 0x01700),
    { { -6, -14, 12, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame015 = {
    SPRITE_FRAME(gSpriteBank15Frame015, SPRITE_TILES_BANK15 + 0x01880),
    { { -9, -13, 18, 27 } },
};
const struct sprite_frame_1box gSpriteBank15Frame016 = {
    SPRITE_FRAME(gSpriteBank15Frame016, SPRITE_TILES_BANK15 + 0x01aa0),
    { { -9, -13, 18, 27 } },
};
const struct sprite_frame_1box gSpriteBank15Frame017 = {
    SPRITE_FRAME(gSpriteBank15Frame017, SPRITE_TILES_BANK15 + 0x01ca0),
    { { -9, -13, 18, 27 } },
};
const struct sprite_frame_1box gSpriteBank15Frame018 = {
    SPRITE_FRAME(gSpriteBank15Frame018, SPRITE_TILES_BANK15 + 0x01ec0),
    { { -9, -13, 18, 27 } },
};
const struct sprite_frame_1box gSpriteBank15Frame019 = {
    SPRITE_FRAME(gSpriteBank15Frame019, SPRITE_TILES_BANK15 + 0x020c0),
    { { -4, -13, 10, 27 } },
};
const struct sprite_frame_1box gSpriteBank15Frame020 = {
    SPRITE_FRAME(gSpriteBank15Frame020, SPRITE_TILES_BANK15 + 0x02240),
    { { -4, -14, 10, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame021 = {
    SPRITE_FRAME(gSpriteBank15Frame021, SPRITE_TILES_BANK15 + 0x023c0),
    { { -6, -14, 13, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame022 = {
    SPRITE_FRAME(gSpriteBank15Frame022, SPRITE_TILES_BANK15 + 0x025c0),
    { { -9, -14, 16, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame023 = {
    SPRITE_FRAME(gSpriteBank15Frame023, SPRITE_TILES_BANK15 + 0x027c0),
    { { -10, -15, 17, 30 } },
};
const struct sprite_frame_1box gSpriteBank15Frame024 = {
    SPRITE_FRAME(gSpriteBank15Frame024, SPRITE_TILES_BANK15 + 0x029c0),
    { { -10, -15, 17, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame025 = {
    SPRITE_FRAME(gSpriteBank15Frame025, SPRITE_TILES_BANK15 + 0x02bc0),
    { { -10, -15, 18, 30 } },
};
const struct sprite_frame_1box gSpriteBank15Frame026 = {
    SPRITE_FRAME(gSpriteBank15Frame026, SPRITE_TILES_BANK15 + 0x02dc0),
    { { -10, -14, 19, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame027 = {
    SPRITE_FRAME(gSpriteBank15Frame027, SPRITE_TILES_BANK15 + 0x02fc0),
    { { -8, -14, 14, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame028 = {
    SPRITE_FRAME(gSpriteBank15Frame028, SPRITE_TILES_BANK15 + 0x031c0),
    { { -6, -13, 12, 27 } },
};
const struct sprite_frame_1box gSpriteBank15Frame029 = {
    SPRITE_FRAME(gSpriteBank15Frame029, SPRITE_TILES_BANK15 + 0x03340),
    { { -7, -14, 12, 27 } },
};
const struct sprite_frame_1box gSpriteBank15Frame030 = {
    SPRITE_FRAME(gSpriteBank15Frame030, SPRITE_TILES_BANK15 + 0x034c0),
    { { -8, -15, 12, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame031 = {
    SPRITE_FRAME(gSpriteBank15Frame031, SPRITE_TILES_BANK15 + 0x03640),
    { { -10, -15, 15, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame032 = {
    SPRITE_FRAME(gSpriteBank15Frame032, SPRITE_TILES_BANK15 + 0x03840),
    { { -10, -15, 17, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame033 = {
    SPRITE_FRAME(gSpriteBank15Frame033, SPRITE_TILES_BANK15 + 0x03a40),
    { { -9, -15, 16, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame034 = {
    SPRITE_FRAME(gSpriteBank15Frame034, SPRITE_TILES_BANK15 + 0x03c40),
    { { -7, -14, 11, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame035 = {
    SPRITE_FRAME(gSpriteBank15Frame035, SPRITE_TILES_BANK15 + 0x03d80),
    { { -7, -14, 11, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame036 = {
    SPRITE_FRAME(gSpriteBank15Frame036, SPRITE_TILES_BANK15 + 0x03ec0),
    { { -7, -14, 12, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame037 = {
    SPRITE_FRAME(gSpriteBank15Frame037, SPRITE_TILES_BANK15 + 0x04000),
    { { -7, -15, 12, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame038 = {
    SPRITE_FRAME(gSpriteBank15Frame038, SPRITE_TILES_BANK15 + 0x04120),
    { { -8, -15, 14, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame039 = {
    SPRITE_FRAME(gSpriteBank15Frame039, SPRITE_TILES_BANK15 + 0x04260),
    { { -9, -15, 16, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame040 = {
    SPRITE_FRAME(gSpriteBank15Frame040, SPRITE_TILES_BANK15 + 0x04460),
    { { -9, -14, 16, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame041 = {
    SPRITE_FRAME(gSpriteBank15Frame041, SPRITE_TILES_BANK15 + 0x04660),
    { { -8, -14, 14, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame042 = {
    SPRITE_FRAME(gSpriteBank15Frame042, SPRITE_TILES_BANK15 + 0x047a0),
    { { -7, -14, 14, 27 } },
};
const struct sprite_frame_1box gSpriteBank15Frame043 = {
    SPRITE_FRAME(gSpriteBank15Frame043, SPRITE_TILES_BANK15 + 0x049a0),
    { { -11, -14, 18, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame044 = {
    SPRITE_FRAME(gSpriteBank15Frame044, SPRITE_TILES_BANK15 + 0x04ba0),
    { { -10, -14, 16, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame045 = {
    SPRITE_FRAME(gSpriteBank15Frame045, SPRITE_TILES_BANK15 + 0x04d20),
    { { -8, -14, 15, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame046 = {
    SPRITE_FRAME(gSpriteBank15Frame046, SPRITE_TILES_BANK15 + 0x04ea0),
    { { -4, -14, 14, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame047 = {
    SPRITE_FRAME(gSpriteBank15Frame047, SPRITE_TILES_BANK15 + 0x04fe0),
    { { -4, -14, 12, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame048 = {
    SPRITE_FRAME(gSpriteBank15Frame048, SPRITE_TILES_BANK15 + 0x05120),
    { { -4, -14, 11, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame049 = {
    SPRITE_FRAME(gSpriteBank15Frame049, SPRITE_TILES_BANK15 + 0x05260),
    { { -4, -14, 12, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame050 = {
    SPRITE_FRAME(gSpriteBank15Frame050, SPRITE_TILES_BANK15 + 0x053c0),
    { { -4, -14, 12, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame051 = {
    SPRITE_FRAME(gSpriteBank15Frame051, SPRITE_TILES_BANK15 + 0x05500),
    { { -4, -14, 12, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame052 = {
    SPRITE_FRAME(gSpriteBank15Frame052, SPRITE_TILES_BANK15 + 0x05640),
    { { -4, -14, 12, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame053 = {
    SPRITE_FRAME(gSpriteBank15Frame053, SPRITE_TILES_BANK15 + 0x057a0),
    { { -4, -14, 12, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame054 = {
    SPRITE_FRAME(gSpriteBank15Frame054, SPRITE_TILES_BANK15 + 0x058e0),
    { { -4, -14, 12, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame055 = {
    SPRITE_FRAME(gSpriteBank15Frame055, SPRITE_TILES_BANK15 + 0x05a20),
    { { -5, -15, 13, 29 } },
};
const struct sprite_frame_1box gSpriteBank15Frame056 = {
    SPRITE_FRAME(gSpriteBank15Frame056, SPRITE_TILES_BANK15 + 0x05b60),
    { { -6, -14, 14, 28 } },
};
const struct sprite_frame_1box gSpriteBank15Frame057 = {
    SPRITE_FRAME(gSpriteBank15Frame057, SPRITE_TILES_BANK15 + 0x05cc0),
    { { -7, -14, 14, 28 } },
};

const struct sprite_piece_pos gSpriteBank15Frame000Pos[2] = { { -10, -13 }, { 6, -11 } };
const struct sprite_piece_pos gSpriteBank15Frame001Pos[2] = { { -10, -13 }, { 6, -11 } };
const struct sprite_piece_pos gSpriteBank15Frame002Pos[2] = { { -10, -14 }, { 6, -11 } };
const struct sprite_piece_pos gSpriteBank15Frame003Pos[2] = { { -11, -15 }, { 5, -12 } };
const struct sprite_piece_pos gSpriteBank15Frame004Pos[2] = { { -13, -15 }, { 3, -13 } };
const struct sprite_piece_pos gSpriteBank15Frame005Pos[1] = { { -14, -15 } };
const struct sprite_piece_pos gSpriteBank15Frame006Pos[2] = { { -13, -14 }, { 3, -13 } };
const struct sprite_piece_pos gSpriteBank15Frame007Pos[2] = { { -12, -14 }, { 4, -12 } };
const struct sprite_piece_pos gSpriteBank15Frame008Pos[2] = { { -12, -14 }, { 4, -13 } };
const struct sprite_piece_pos gSpriteBank15Frame009Pos[2] = { { -12, -14 }, { 4, -13 } };
const struct sprite_piece_pos gSpriteBank15Frame010Pos[2] = { { -13, -15 }, { 3, -15 } };
const struct sprite_piece_pos gSpriteBank15Frame011Pos[1] = { { -13, -15 } };
const struct sprite_piece_pos gSpriteBank15Frame012Pos[1] = { { -13, -15 } };
const struct sprite_piece_pos gSpriteBank15Frame013Pos[1] = { { -12, -15 } };
const struct sprite_piece_pos gSpriteBank15Frame014Pos[2] = { { -10, -14 }, { 6, -11 } };
const struct sprite_piece_pos gSpriteBank15Frame015Pos[2] = { { -17, -15 }, { 15, -3 } };
const struct sprite_piece_pos gSpriteBank15Frame016Pos[1] = { { -16, -15 } };
const struct sprite_piece_pos gSpriteBank15Frame017Pos[2] = { { -17, -14 }, { 15, -1 } };
const struct sprite_piece_pos gSpriteBank15Frame018Pos[1] = { { -16, -14 } };
const struct sprite_piece_pos gSpriteBank15Frame019Pos[2] = { { -10, -13 }, { 6, -11 } };
const struct sprite_piece_pos gSpriteBank15Frame020Pos[2] = { { -9, -14 }, { 7, -10 } };
const struct sprite_piece_pos gSpriteBank15Frame021Pos[1] = { { -12, -14 } };
const struct sprite_piece_pos gSpriteBank15Frame022Pos[1] = { { -15, -14 } };
const struct sprite_piece_pos gSpriteBank15Frame023Pos[1] = { { -16, -15 } };
const struct sprite_piece_pos gSpriteBank15Frame024Pos[1] = { { -16, -15 } };
const struct sprite_piece_pos gSpriteBank15Frame025Pos[1] = { { -16, -15 } };
const struct sprite_piece_pos gSpriteBank15Frame026Pos[1] = { { -16, -14 } };
const struct sprite_piece_pos gSpriteBank15Frame027Pos[1] = { { -14, -14 } };
const struct sprite_piece_pos gSpriteBank15Frame028Pos[2] = { { -10, -13 }, { 6, -11 } };
const struct sprite_piece_pos gSpriteBank15Frame029Pos[2] = { { -11, -14 }, { 5, -12 } };
const struct sprite_piece_pos gSpriteBank15Frame030Pos[2] = { { -12, -15 }, { 4, -12 } };
const struct sprite_piece_pos gSpriteBank15Frame031Pos[1] = { { -14, -15 } };
const struct sprite_piece_pos gSpriteBank15Frame032Pos[1] = { { -14, -15 } };
const struct sprite_piece_pos gSpriteBank15Frame033Pos[1] = { { -13, -15 } };
const struct sprite_piece_pos gSpriteBank15Frame034Pos[2] = { { -11, -14 }, { 5, -1 } };
const struct sprite_piece_pos gSpriteBank15Frame035Pos[2] = { { -11, -14 }, { 5, 1 } };
const struct sprite_piece_pos gSpriteBank15Frame036Pos[2] = { { -11, -14 }, { 5, 3 } };
const struct sprite_piece_pos gSpriteBank15Frame037Pos[2] = { { -11, -15 }, { 5, 4 } };
const struct sprite_piece_pos gSpriteBank15Frame038Pos[2] = { { -12, -15 }, { 4, 1 } };
const struct sprite_piece_pos gSpriteBank15Frame039Pos[1] = { { -13, -15 } };
const struct sprite_piece_pos gSpriteBank15Frame040Pos[1] = { { -13, -14 } };
const struct sprite_piece_pos gSpriteBank15Frame041Pos[2] = { { -12, -14 }, { 4, 1 } };
const struct sprite_piece_pos gSpriteBank15Frame042Pos[1] = { { -14, -14 } };
const struct sprite_piece_pos gSpriteBank15Frame043Pos[1] = { { -14, -14 } };
const struct sprite_piece_pos gSpriteBank15Frame044Pos[2] = { { -13, -14 }, { 3, -12 } };
const struct sprite_piece_pos gSpriteBank15Frame045Pos[2] = { { -11, -14 }, { 5, -12 } };
const struct sprite_piece_pos gSpriteBank15Frame046Pos[2] = { { -7, -14 }, { 9, -1 } };
const struct sprite_piece_pos gSpriteBank15Frame047Pos[3] = { { -7, -14 }, { 9, -7 }, { 9, 12 } };
const struct sprite_piece_pos gSpriteBank15Frame048Pos[3] = { { -7, -14 }, { 9, -6 }, { 9, 12 } };
const struct sprite_piece_pos gSpriteBank15Frame049Pos[3] = { { -7, -14 }, { 9, -3 }, { 9, 13 } };
const struct sprite_piece_pos gSpriteBank15Frame050Pos[2] = { { -7, -14 }, { 9, 0 } };
const struct sprite_piece_pos gSpriteBank15Frame051Pos[2] = { { -7, -14 }, { 9, 0 } };
const struct sprite_piece_pos gSpriteBank15Frame052Pos[3] = { { -7, -14 }, { 9, -6 }, { 9, 12 } };
const struct sprite_piece_pos gSpriteBank15Frame053Pos[2] = { { -7, -14 }, { 9, 0 } };
const struct sprite_piece_pos gSpriteBank15Frame054Pos[2] = { { -7, -14 }, { 9, 1 } };
const struct sprite_piece_pos gSpriteBank15Frame055Pos[2] = { { -8, -15 }, { 8, 1 } };
const struct sprite_piece_pos gSpriteBank15Frame056Pos[3] = { { -9, -14 }, { 7, -11 }, { 7, 5 } };
const struct sprite_piece_pos gSpriteBank15Frame057Pos[2] = { { -10, -14 }, { 6, -11 } };

const u8 gSpriteBank15Frame000Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank15Frame001Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank15Frame002Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank15Frame003Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank15Frame004Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank15Frame005Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank15Frame006Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank15Frame007Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank15Frame008Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank15Frame009Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank15Frame010Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank15Frame011Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank15Frame012Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank15Frame013Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank15Frame014Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank15Frame015Pieces[2] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank15Frame016Pieces[1] = { SPRITE_PIECE(5, 2) };
const u8 gSpriteBank15Frame017Pieces[2] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank15Frame018Pieces[1] = { SPRITE_PIECE(5, 2) };
const u8 gSpriteBank15Frame019Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank15Frame020Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank15Frame021Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank15Frame022Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank15Frame023Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank15Frame024Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank15Frame025Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank15Frame026Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank15Frame027Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank15Frame028Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank15Frame029Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank15Frame030Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank15Frame031Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank15Frame032Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank15Frame033Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank15Frame034Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank15Frame035Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank15Frame036Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank15Frame037Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank15Frame038Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank15Frame039Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank15Frame040Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank15Frame041Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank15Frame042Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank15Frame043Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank15Frame044Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank15Frame045Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank15Frame046Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank15Frame047Pieces[3] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank15Frame048Pieces[3] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank15Frame049Pieces[3] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank15Frame050Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank15Frame051Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank15Frame052Pieces[3] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank15Frame053Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank15Frame054Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank15Frame055Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank15Frame056Pieces[3] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank15Frame057Pieces[2] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9) };

/* ---------------------------------------------------------------------- */
/* Bank 16: 2 animations, 24 frames, tiles in gSpriteBank16Tiles (SPRITE_TILES_BANK16). */

extern const u16 gSpriteBank16Anim00Seq[12];
extern const u16 gSpriteBank16Anim01Seq[12];
extern const struct sprite_frame_1box gSpriteBank16Frame000;
extern const struct sprite_frame_1box gSpriteBank16Frame001;
extern const struct sprite_frame_1box gSpriteBank16Frame002;
extern const struct sprite_frame_1box gSpriteBank16Frame003;
extern const struct sprite_frame_1box gSpriteBank16Frame004;
extern const struct sprite_frame_1box gSpriteBank16Frame005;
extern const struct sprite_frame_1box gSpriteBank16Frame006;
extern const struct sprite_frame_1box gSpriteBank16Frame007;
extern const struct sprite_frame_1box gSpriteBank16Frame008;
extern const struct sprite_frame_1box gSpriteBank16Frame009;
extern const struct sprite_frame_1box gSpriteBank16Frame010;
extern const struct sprite_frame_1box gSpriteBank16Frame011;
extern const struct sprite_frame_1box gSpriteBank16Frame012;
extern const struct sprite_frame_1box gSpriteBank16Frame013;
extern const struct sprite_frame_1box gSpriteBank16Frame014;
extern const struct sprite_frame_1box gSpriteBank16Frame015;
extern const struct sprite_frame_1box gSpriteBank16Frame016;
extern const struct sprite_frame_1box gSpriteBank16Frame017;
extern const struct sprite_frame_1box gSpriteBank16Frame018;
extern const struct sprite_frame_1box gSpriteBank16Frame019;
extern const struct sprite_frame_1box gSpriteBank16Frame020;
extern const struct sprite_frame_1box gSpriteBank16Frame021;
extern const struct sprite_frame_1box gSpriteBank16Frame022;
extern const struct sprite_frame_1box gSpriteBank16Frame023;
extern const struct sprite_piece_pos gSpriteBank16Frame000Pos[1];
extern const struct sprite_piece_pos gSpriteBank16Frame001Pos[1];
extern const struct sprite_piece_pos gSpriteBank16Frame002Pos[1];
extern const struct sprite_piece_pos gSpriteBank16Frame003Pos[1];
extern const struct sprite_piece_pos gSpriteBank16Frame004Pos[1];
extern const struct sprite_piece_pos gSpriteBank16Frame005Pos[1];
extern const struct sprite_piece_pos gSpriteBank16Frame006Pos[2];
extern const struct sprite_piece_pos gSpriteBank16Frame007Pos[2];
extern const struct sprite_piece_pos gSpriteBank16Frame008Pos[2];
extern const struct sprite_piece_pos gSpriteBank16Frame009Pos[2];
extern const struct sprite_piece_pos gSpriteBank16Frame010Pos[2];
extern const struct sprite_piece_pos gSpriteBank16Frame011Pos[2];
extern const struct sprite_piece_pos gSpriteBank16Frame012Pos[1];
extern const struct sprite_piece_pos gSpriteBank16Frame013Pos[1];
extern const struct sprite_piece_pos gSpriteBank16Frame014Pos[1];
extern const struct sprite_piece_pos gSpriteBank16Frame015Pos[1];
extern const struct sprite_piece_pos gSpriteBank16Frame016Pos[3];
extern const struct sprite_piece_pos gSpriteBank16Frame017Pos[3];
extern const struct sprite_piece_pos gSpriteBank16Frame018Pos[1];
extern const struct sprite_piece_pos gSpriteBank16Frame019Pos[2];
extern const struct sprite_piece_pos gSpriteBank16Frame020Pos[3];
extern const struct sprite_piece_pos gSpriteBank16Frame021Pos[1];
extern const struct sprite_piece_pos gSpriteBank16Frame022Pos[1];
extern const struct sprite_piece_pos gSpriteBank16Frame023Pos[1];
extern const u8 gSpriteBank16Frame000Pieces[1];
extern const u8 gSpriteBank16Frame001Pieces[1];
extern const u8 gSpriteBank16Frame002Pieces[1];
extern const u8 gSpriteBank16Frame003Pieces[1];
extern const u8 gSpriteBank16Frame004Pieces[1];
extern const u8 gSpriteBank16Frame005Pieces[1];
extern const u8 gSpriteBank16Frame006Pieces[2];
extern const u8 gSpriteBank16Frame007Pieces[2];
extern const u8 gSpriteBank16Frame008Pieces[2];
extern const u8 gSpriteBank16Frame009Pieces[2];
extern const u8 gSpriteBank16Frame010Pieces[2];
extern const u8 gSpriteBank16Frame011Pieces[2];
extern const u8 gSpriteBank16Frame012Pieces[1];
extern const u8 gSpriteBank16Frame013Pieces[1];
extern const u8 gSpriteBank16Frame014Pieces[1];
extern const u8 gSpriteBank16Frame015Pieces[1];
extern const u8 gSpriteBank16Frame016Pieces[3];
extern const u8 gSpriteBank16Frame017Pieces[3];
extern const u8 gSpriteBank16Frame018Pieces[1];
extern const u8 gSpriteBank16Frame019Pieces[2];
extern const u8 gSpriteBank16Frame020Pieces[3];
extern const u8 gSpriteBank16Frame021Pieces[1];
extern const u8 gSpriteBank16Frame022Pieces[1];
extern const u8 gSpriteBank16Frame023Pieces[1];

const struct sprite_anim gSpriteBank16Anims[2] = {
    [0] = {
        .seq = gSpriteBank16Anim00Seq,
        .box = { { -30, -14, 60, 29 }, { -38, -14, 71, 29 } },
        .tileRecord = 24,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank16Anim00Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [1] = {
        .seq = gSpriteBank16Anim01Seq,
        .box = { { -30, -14, 60, 29 }, { -35, -14, 65, 29 } },
        .tileRecord = 24,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank16Anim01Seq),
        .flags = 0,
    },
};

const u16 gSpriteBank16Anim00Seq[12] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
};
const u16 gSpriteBank16Anim01Seq[12] = {
    12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
};

const struct sprite_frame *const gSpriteBank16Frames[24] = {
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
    SPRITE_FRAME(gSpriteBank16Frame000, SPRITE_TILES_BANK16 + 0x00000),
    { { -25, -10, 50, 21 } },
};
const struct sprite_frame_1box gSpriteBank16Frame001 = {
    SPRITE_FRAME(gSpriteBank16Frame001, SPRITE_TILES_BANK16 + 0x00400),
    { { -24, -9, 49, 20 } },
};
const struct sprite_frame_1box gSpriteBank16Frame002 = {
    SPRITE_FRAME(gSpriteBank16Frame002, SPRITE_TILES_BANK16 + 0x00800),
    { { -24, -8, 49, 18 } },
};
const struct sprite_frame_1box gSpriteBank16Frame003 = {
    SPRITE_FRAME(gSpriteBank16Frame003, SPRITE_TILES_BANK16 + 0x00c00),
    { { -23, -8, 49, 18 } },
};
const struct sprite_frame_1box gSpriteBank16Frame004 = {
    SPRITE_FRAME(gSpriteBank16Frame004, SPRITE_TILES_BANK16 + 0x01000),
    { { -23, -8, 49, 18 } },
};
const struct sprite_frame_1box gSpriteBank16Frame005 = {
    SPRITE_FRAME(gSpriteBank16Frame005, SPRITE_TILES_BANK16 + 0x01400),
    { { -25, -9, 52, 20 } },
};
const struct sprite_frame_1box gSpriteBank16Frame006 = {
    SPRITE_FRAME(gSpriteBank16Frame006, SPRITE_TILES_BANK16 + 0x01800),
    { { -29, -10, 57, 21 } },
};
const struct sprite_frame_1box gSpriteBank16Frame007 = {
    SPRITE_FRAME(gSpriteBank16Frame007, SPRITE_TILES_BANK16 + 0x01c20),
    { { -31, -9, 59, 20 } },
};
const struct sprite_frame_1box gSpriteBank16Frame008 = {
    SPRITE_FRAME(gSpriteBank16Frame008, SPRITE_TILES_BANK16 + 0x02060),
    { { -33, -8, 61, 18 } },
};
const struct sprite_frame_1box gSpriteBank16Frame009 = {
    SPRITE_FRAME(gSpriteBank16Frame009, SPRITE_TILES_BANK16 + 0x024a0),
    { { -33, -8, 60, 18 } },
};
const struct sprite_frame_1box gSpriteBank16Frame010 = {
    SPRITE_FRAME(gSpriteBank16Frame010, SPRITE_TILES_BANK16 + 0x028e0),
    { { -33, -8, 59, 18 } },
};
const struct sprite_frame_1box gSpriteBank16Frame011 = {
    SPRITE_FRAME(gSpriteBank16Frame011, SPRITE_TILES_BANK16 + 0x02d20),
    { { -29, -9, 55, 19 } },
};
const struct sprite_frame_1box gSpriteBank16Frame012 = {
    SPRITE_FRAME(gSpriteBank16Frame012, SPRITE_TILES_BANK16 + 0x03140),
    { { -27, -11, 54, 23 } },
};
const struct sprite_frame_1box gSpriteBank16Frame013 = {
    SPRITE_FRAME(gSpriteBank16Frame013, SPRITE_TILES_BANK16 + 0x03540),
    { { -25, -10, 51, 22 } },
};
const struct sprite_frame_1box gSpriteBank16Frame014 = {
    SPRITE_FRAME(gSpriteBank16Frame014, SPRITE_TILES_BANK16 + 0x03940),
    { { -23, -9, 47, 20 } },
};
const struct sprite_frame_1box gSpriteBank16Frame015 = {
    SPRITE_FRAME(gSpriteBank16Frame015, SPRITE_TILES_BANK16 + 0x03d40),
    { { -20, -9, 41, 20 } },
};
const struct sprite_frame_1box gSpriteBank16Frame016 = {
    SPRITE_FRAME(gSpriteBank16Frame016, SPRITE_TILES_BANK16 + 0x04140),
    { { -19, -9, 36, 20 } },
};
const struct sprite_frame_1box gSpriteBank16Frame017 = {
    SPRITE_FRAME(gSpriteBank16Frame017, SPRITE_TILES_BANK16 + 0x04400),
    { { -18, -10, 31, 21 } },
};
const struct sprite_frame_1box gSpriteBank16Frame018 = {
    SPRITE_FRAME(gSpriteBank16Frame018, SPRITE_TILES_BANK16 + 0x04660),
    { { -13, -10, 23, 22 } },
};
const struct sprite_frame_1box gSpriteBank16Frame019 = {
    SPRITE_FRAME(gSpriteBank16Frame019, SPRITE_TILES_BANK16 + 0x04860),
    { { -13, -10, 27, 22 } },
};
const struct sprite_frame_1box gSpriteBank16Frame020 = {
    SPRITE_FRAME(gSpriteBank16Frame020, SPRITE_TILES_BANK16 + 0x04aa0),
    { { -18, -9, 34, 20 } },
};
const struct sprite_frame_1box gSpriteBank16Frame021 = {
    SPRITE_FRAME(gSpriteBank16Frame021, SPRITE_TILES_BANK16 + 0x04d40),
    { { -24, -9, 41, 20 } },
};
const struct sprite_frame_1box gSpriteBank16Frame022 = {
    SPRITE_FRAME(gSpriteBank16Frame022, SPRITE_TILES_BANK16 + 0x05140),
    { { -29, -10, 48, 21 } },
};
const struct sprite_frame_1box gSpriteBank16Frame023 = {
    SPRITE_FRAME(gSpriteBank16Frame023, SPRITE_TILES_BANK16 + 0x05540),
    { { -32, -11, 55, 22 } },
};

const struct sprite_piece_pos gSpriteBank16Frame000Pos[1] = { { -30, -14 } };
const struct sprite_piece_pos gSpriteBank16Frame001Pos[1] = { { -29, -13 } };
const struct sprite_piece_pos gSpriteBank16Frame002Pos[1] = { { -29, -12 } };
const struct sprite_piece_pos gSpriteBank16Frame003Pos[1] = { { -28, -12 } };
const struct sprite_piece_pos gSpriteBank16Frame004Pos[1] = { { -28, -12 } };
const struct sprite_piece_pos gSpriteBank16Frame005Pos[1] = { { -30, -13 } };
const struct sprite_piece_pos gSpriteBank16Frame006Pos[2] = { { -34, -14 }, { 30, 0 } };
const struct sprite_piece_pos gSpriteBank16Frame007Pos[2] = { { -36, -13 }, { 28, -2 } };
const struct sprite_piece_pos gSpriteBank16Frame008Pos[2] = { { -38, -12 }, { 26, -4 } };
const struct sprite_piece_pos gSpriteBank16Frame009Pos[2] = { { -38, -12 }, { 26, -4 } };
const struct sprite_piece_pos gSpriteBank16Frame010Pos[2] = { { -38, -12 }, { 26, -2 } };
const struct sprite_piece_pos gSpriteBank16Frame011Pos[2] = { { -34, -13 }, { 30, 1 } };
const struct sprite_piece_pos gSpriteBank16Frame012Pos[1] = { { -30, -14 } };
const struct sprite_piece_pos gSpriteBank16Frame013Pos[1] = { { -28, -13 } };
const struct sprite_piece_pos gSpriteBank16Frame014Pos[1] = { { -26, -12 } };
const struct sprite_piece_pos gSpriteBank16Frame015Pos[1] = { { -23, -12 } };
const struct sprite_piece_pos gSpriteBank16Frame016Pos[3] = { { -22, -12 }, { 10, -9 }, { 18, -2 } };
const struct sprite_piece_pos gSpriteBank16Frame017Pos[3] = { { -21, -13 }, { 11, -7 }, { 11, 9 } };
const struct sprite_piece_pos gSpriteBank16Frame018Pos[1] = { { -16, -13 } };
const struct sprite_piece_pos gSpriteBank16Frame019Pos[2] = { { -16, -13 }, { 16, 0 } };
const struct sprite_piece_pos gSpriteBank16Frame020Pos[3] = { { -21, -12 }, { 11, -12 }, { 19, 7 } };
const struct sprite_piece_pos gSpriteBank16Frame021Pos[1] = { { -27, -12 } };
const struct sprite_piece_pos gSpriteBank16Frame022Pos[1] = { { -32, -13 } };
const struct sprite_piece_pos gSpriteBank16Frame023Pos[1] = { { -35, -14 } };

const u8 gSpriteBank16Frame000Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank16Frame001Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank16Frame002Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank16Frame003Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank16Frame004Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank16Frame005Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank16Frame006Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank16Frame007Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank16Frame008Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank16Frame009Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank16Frame010Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank16Frame011Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank16Frame012Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank16Frame013Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank16Frame014Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank16Frame015Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank16Frame016Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank16Frame017Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank16Frame018Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank16Frame019Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank16Frame020Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank16Frame021Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank16Frame022Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank16Frame023Pieces[1] = { SPRITE_PIECE(2, 7) };

/* ---------------------------------------------------------------------- */
/* Bank 17: 1 animation, 15 frames, tiles in gSpriteBank17Tiles (SPRITE_TILES_BANK17). */

extern const u16 gSpriteBank17Anim00Seq[15];
extern const struct sprite_frame_1box gSpriteBank17Frame000;
extern const struct sprite_frame_1box gSpriteBank17Frame001;
extern const struct sprite_frame_1box gSpriteBank17Frame002;
extern const struct sprite_frame_1box gSpriteBank17Frame003;
extern const struct sprite_frame_1box gSpriteBank17Frame004;
extern const struct sprite_frame_1box gSpriteBank17Frame005;
extern const struct sprite_frame_1box gSpriteBank17Frame006;
extern const struct sprite_frame_1box gSpriteBank17Frame007;
extern const struct sprite_frame_1box gSpriteBank17Frame008;
extern const struct sprite_frame_1box gSpriteBank17Frame009;
extern const struct sprite_frame_1box gSpriteBank17Frame010;
extern const struct sprite_frame_1box gSpriteBank17Frame011;
extern const struct sprite_frame_1box gSpriteBank17Frame012;
extern const struct sprite_frame_1box gSpriteBank17Frame013;
extern const struct sprite_frame_1box gSpriteBank17Frame014;
extern const struct sprite_piece_pos gSpriteBank17Frame000Pos[3];
extern const struct sprite_piece_pos gSpriteBank17Frame001Pos[3];
extern const struct sprite_piece_pos gSpriteBank17Frame002Pos[2];
extern const struct sprite_piece_pos gSpriteBank17Frame003Pos[3];
extern const struct sprite_piece_pos gSpriteBank17Frame004Pos[1];
extern const struct sprite_piece_pos gSpriteBank17Frame005Pos[5];
extern const struct sprite_piece_pos gSpriteBank17Frame006Pos[4];
extern const struct sprite_piece_pos gSpriteBank17Frame007Pos[4];
extern const struct sprite_piece_pos gSpriteBank17Frame008Pos[4];
extern const struct sprite_piece_pos gSpriteBank17Frame009Pos[5];
extern const struct sprite_piece_pos gSpriteBank17Frame010Pos[4];
extern const struct sprite_piece_pos gSpriteBank17Frame011Pos[1];
extern const struct sprite_piece_pos gSpriteBank17Frame012Pos[3];
extern const struct sprite_piece_pos gSpriteBank17Frame013Pos[3];
extern const struct sprite_piece_pos gSpriteBank17Frame014Pos[3];
extern const u8 gSpriteBank17Frame000Pieces[3];
extern const u8 gSpriteBank17Frame001Pieces[3];
extern const u8 gSpriteBank17Frame002Pieces[2];
extern const u8 gSpriteBank17Frame003Pieces[3];
extern const u8 gSpriteBank17Frame004Pieces[1];
extern const u8 gSpriteBank17Frame005Pieces[5];
extern const u8 gSpriteBank17Frame006Pieces[4];
extern const u8 gSpriteBank17Frame007Pieces[4];
extern const u8 gSpriteBank17Frame008Pieces[4];
extern const u8 gSpriteBank17Frame009Pieces[5];
extern const u8 gSpriteBank17Frame010Pieces[4];
extern const u8 gSpriteBank17Frame011Pieces[1];
extern const u8 gSpriteBank17Frame012Pieces[3];
extern const u8 gSpriteBank17Frame013Pieces[3];
extern const u8 gSpriteBank17Frame014Pieces[3];

const struct sprite_anim gSpriteBank17Anims[1] = {
    [0] = {
        .seq = gSpriteBank17Anim00Seq,
        .box = { { 7, -4, 11, 18 }, { -30, -19, 54, 35 } },
        .tileRecord = 25,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank17Anim00Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank17Anim00Seq[15] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
};

const struct sprite_frame *const gSpriteBank17Frames[15] = {
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
    SPRITE_FRAME(gSpriteBank17Frame000, SPRITE_TILES_BANK17 + 0x00000),
    { { -10, -4, 29, 13 } },
};
const struct sprite_frame_1box gSpriteBank17Frame001 = {
    SPRITE_FRAME(gSpriteBank17Frame001, SPRITE_TILES_BANK17 + 0x00260),
    { { -10, -4, 29, 13 } },
};
const struct sprite_frame_1box gSpriteBank17Frame002 = {
    SPRITE_FRAME(gSpriteBank17Frame002, SPRITE_TILES_BANK17 + 0x004c0),
    { { -10, -4, 29, 13 } },
};
const struct sprite_frame_1box gSpriteBank17Frame003 = {
    SPRITE_FRAME(gSpriteBank17Frame003, SPRITE_TILES_BANK17 + 0x00740),
    { { -10, -4, 29, 13 } },
};
const struct sprite_frame_1box gSpriteBank17Frame004 = {
    SPRITE_FRAME(gSpriteBank17Frame004, SPRITE_TILES_BANK17 + 0x00a00),
    { { -10, -4, 29, 13 } },
};
const struct sprite_frame_1box gSpriteBank17Frame005 = {
    SPRITE_FRAME(gSpriteBank17Frame005, SPRITE_TILES_BANK17 + 0x00e00),
    { { -10, -4, 29, 13 } },
};
const struct sprite_frame_1box gSpriteBank17Frame006 = {
    SPRITE_FRAME(gSpriteBank17Frame006, SPRITE_TILES_BANK17 + 0x01060),
    { { -10, -4, 29, 13 } },
};
const struct sprite_frame_1box gSpriteBank17Frame007 = {
    SPRITE_FRAME(gSpriteBank17Frame007, SPRITE_TILES_BANK17 + 0x01260),
    { { -10, -4, 29, 13 } },
};
const struct sprite_frame_1box gSpriteBank17Frame008 = {
    SPRITE_FRAME(gSpriteBank17Frame008, SPRITE_TILES_BANK17 + 0x01440),
    { { -10, -4, 29, 13 } },
};
const struct sprite_frame_1box gSpriteBank17Frame009 = {
    SPRITE_FRAME(gSpriteBank17Frame009, SPRITE_TILES_BANK17 + 0x01620),
    { { -10, -4, 29, 13 } },
};
const struct sprite_frame_1box gSpriteBank17Frame010 = {
    SPRITE_FRAME(gSpriteBank17Frame010, SPRITE_TILES_BANK17 + 0x01800),
    { { -10, -4, 29, 13 } },
};
const struct sprite_frame_1box gSpriteBank17Frame011 = {
    SPRITE_FRAME(gSpriteBank17Frame011, SPRITE_TILES_BANK17 + 0x019e0),
    { { -10, -4, 29, 13 } },
};
const struct sprite_frame_1box gSpriteBank17Frame012 = {
    SPRITE_FRAME(gSpriteBank17Frame012, SPRITE_TILES_BANK17 + 0x01de0),
    { { -10, -4, 29, 13 } },
};
const struct sprite_frame_1box gSpriteBank17Frame013 = {
    SPRITE_FRAME(gSpriteBank17Frame013, SPRITE_TILES_BANK17 + 0x020a0),
    { { -10, -4, 29, 13 } },
};
const struct sprite_frame_1box gSpriteBank17Frame014 = {
    SPRITE_FRAME(gSpriteBank17Frame014, SPRITE_TILES_BANK17 + 0x02340),
    { { -10, -4, 29, 13 } },
};

const struct sprite_piece_pos gSpriteBank17Frame000Pos[3] = { { -18, -15 }, { 14, -14 }, { 14, 2 } };
const struct sprite_piece_pos gSpriteBank17Frame001Pos[3] = { { -18, -16 }, { 14, -14 }, { 14, 2 } };
const struct sprite_piece_pos gSpriteBank17Frame002Pos[2] = { { -19, -18 }, { 13, -15 } };
const struct sprite_piece_pos gSpriteBank17Frame003Pos[3] = { { -22, -19 }, { 10, -17 }, { 18, -15 } };
const struct sprite_piece_pos gSpriteBank17Frame004Pos[1] = { { -26, -17 } };
const struct sprite_piece_pos gSpriteBank17Frame005Pos[5] = { { -26, -6 }, { 3, -12 }, { 19, -5 }, { -29, 4 }, { 3, 4 } };
const struct sprite_piece_pos gSpriteBank17Frame006Pos[4] = { { -30, -5 }, { 2, -8 }, { 18, -5 }, { -4, 8 } };
const struct sprite_piece_pos gSpriteBank17Frame007Pos[4] = { { -30, -5 }, { 2, -5 }, { 18, -5 }, { 1, 11 } };
const struct sprite_piece_pos gSpriteBank17Frame008Pos[4] = { { -29, -5 }, { 3, -5 }, { 19, -3 }, { 0, 11 } };
const struct sprite_piece_pos gSpriteBank17Frame009Pos[5] = { { -27, -5 }, { 5, -6 }, { 21, -3 }, { 1, 10 }, { 9, 10 } };
const struct sprite_piece_pos gSpriteBank17Frame010Pos[4] = { { -26, -5 }, { 6, -7 }, { 22, -4 }, { -3, 9 } };
const struct sprite_piece_pos gSpriteBank17Frame011Pos[1] = { { -24, -9 } };
const struct sprite_piece_pos gSpriteBank17Frame012Pos[3] = { { -22, -5 }, { 10, -11 }, { 18, -10 } };
const struct sprite_piece_pos gSpriteBank17Frame013Pos[3] = { { -20, -8 }, { 12, -13 }, { 20, -10 } };
const struct sprite_piece_pos gSpriteBank17Frame014Pos[3] = { { -18, -14 }, { 14, -14 }, { 14, 2 } };

const u8 gSpriteBank17Frame000Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank17Frame001Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank17Frame002Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank17Frame003Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank17Frame004Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank17Frame005Pieces[5] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank17Frame006Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank17Frame007Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank17Frame008Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank17Frame009Pieces[5] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank17Frame010Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank17Frame011Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank17Frame012Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank17Frame013Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank17Frame014Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };

/* ---------------------------------------------------------------------- */
/* Bank 18: 2 animations, 14 frames, tiles in gSpriteBank18Tiles (SPRITE_TILES_BANK18). */

extern const u16 gSpriteBank18Anim00Seq[9];
extern const u16 gSpriteBank18Anim01Seq[5];
extern const struct sprite_frame_1box gSpriteBank18Frame000;
extern const struct sprite_frame_1box gSpriteBank18Frame001;
extern const struct sprite_frame_1box gSpriteBank18Frame002;
extern const struct sprite_frame_1box gSpriteBank18Frame003;
extern const struct sprite_frame_1box gSpriteBank18Frame004;
extern const struct sprite_frame_1box gSpriteBank18Frame005;
extern const struct sprite_frame_1box gSpriteBank18Frame006;
extern const struct sprite_frame_1box gSpriteBank18Frame007;
extern const struct sprite_frame_1box gSpriteBank18Frame008;
extern const struct sprite_frame_1box gSpriteBank18Frame009;
extern const struct sprite_frame_1box gSpriteBank18Frame010;
extern const struct sprite_frame_1box gSpriteBank18Frame011;
extern const struct sprite_frame_1box gSpriteBank18Frame012;
extern const struct sprite_frame_1box gSpriteBank18Frame013;
extern const struct sprite_piece_pos gSpriteBank18Frame000Pos[2];
extern const struct sprite_piece_pos gSpriteBank18Frame001Pos[2];
extern const struct sprite_piece_pos gSpriteBank18Frame002Pos[3];
extern const struct sprite_piece_pos gSpriteBank18Frame003Pos[2];
extern const struct sprite_piece_pos gSpriteBank18Frame004Pos[3];
extern const struct sprite_piece_pos gSpriteBank18Frame005Pos[1];
extern const struct sprite_piece_pos gSpriteBank18Frame006Pos[1];
extern const struct sprite_piece_pos gSpriteBank18Frame007Pos[2];
extern const struct sprite_piece_pos gSpriteBank18Frame008Pos[2];
extern const struct sprite_piece_pos gSpriteBank18Frame009Pos[2];
extern const struct sprite_piece_pos gSpriteBank18Frame010Pos[2];
extern const struct sprite_piece_pos gSpriteBank18Frame011Pos[1];
extern const struct sprite_piece_pos gSpriteBank18Frame012Pos[2];
extern const struct sprite_piece_pos gSpriteBank18Frame013Pos[2];
extern const u8 gSpriteBank18Frame000Pieces[2];
extern const u8 gSpriteBank18Frame001Pieces[2];
extern const u8 gSpriteBank18Frame002Pieces[3];
extern const u8 gSpriteBank18Frame003Pieces[2];
extern const u8 gSpriteBank18Frame004Pieces[3];
extern const u8 gSpriteBank18Frame005Pieces[1];
extern const u8 gSpriteBank18Frame006Pieces[1];
extern const u8 gSpriteBank18Frame007Pieces[2];
extern const u8 gSpriteBank18Frame008Pieces[2];
extern const u8 gSpriteBank18Frame009Pieces[2];
extern const u8 gSpriteBank18Frame010Pieces[2];
extern const u8 gSpriteBank18Frame011Pieces[1];
extern const u8 gSpriteBank18Frame012Pieces[2];
extern const u8 gSpriteBank18Frame013Pieces[2];

const struct sprite_anim gSpriteBank18Anims[2] = {
    [0] = {
        .seq = gSpriteBank18Anim00Seq,
        .box = { { -15, -51, 31, 103 }, { -15, -67, 31, 119 } },
        .tileRecord = 105,
        .duration = 0,
        .frameCount = ARRAY_COUNT(gSpriteBank18Anim00Seq),
        .flags = 0,
    },
    [1] = {
        .seq = gSpriteBank18Anim01Seq,
        .box = { { -15, -51, 31, 103 }, { -15, -67, 31, 119 } },
        .tileRecord = 105,
        .duration = 0,
        .frameCount = ARRAY_COUNT(gSpriteBank18Anim01Seq),
        .flags = 0,
    },
};

const u16 gSpriteBank18Anim00Seq[9] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8,
};
const u16 gSpriteBank18Anim01Seq[5] = {
    9, 10, 11, 12, 13,
};

const struct sprite_frame *const gSpriteBank18Frames[14] = {
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
    SPRITE_FRAME(gSpriteBank18Frame000, SPRITE_TILES_BANK18 + 0x00000),
    { { -11, -67, 23, 116 } },
};
const struct sprite_frame_1box gSpriteBank18Frame001 = {
    SPRITE_FRAME(gSpriteBank18Frame001, SPRITE_TILES_BANK18 + 0x00800),
    { { -11, -68, 23, 125 } },
};
const struct sprite_frame_1box gSpriteBank18Frame002 = {
    SPRITE_FRAME(gSpriteBank18Frame002, SPRITE_TILES_BANK18 + 0x01000),
    { { -11, -67, 23, 100 } },
};
const struct sprite_frame_1box gSpriteBank18Frame003 = {
    SPRITE_FRAME(gSpriteBank18Frame003, SPRITE_TILES_BANK18 + 0x01700),
    { { -11, -66, 23, 81 } },
};
const struct sprite_frame_1box gSpriteBank18Frame004 = {
    SPRITE_FRAME(gSpriteBank18Frame004, SPRITE_TILES_BANK18 + 0x01d00),
    { { -11, -67, 23, 59 } },
};
const struct sprite_frame_1box gSpriteBank18Frame005 = {
    SPRITE_FRAME(gSpriteBank18Frame005, SPRITE_TILES_BANK18 + 0x02160),
    { { -11, -66, 23, 50 } },
};
const struct sprite_frame_1box gSpriteBank18Frame006 = {
    SPRITE_FRAME(gSpriteBank18Frame006, SPRITE_TILES_BANK18 + 0x02560),
    { { -11, -67, 23, 44 } },
};
const struct sprite_frame_1box gSpriteBank18Frame007 = {
    SPRITE_FRAME(gSpriteBank18Frame007, SPRITE_TILES_BANK18 + 0x02960),
    { { -11, -68, 23, 39 } },
};
const struct sprite_frame_1box gSpriteBank18Frame008 = {
    SPRITE_FRAME(gSpriteBank18Frame008, SPRITE_TILES_BANK18 + 0x02c60),
    { { -11, -66, 23, 30 } },
};
const struct sprite_frame_1box gSpriteBank18Frame009 = {
    SPRITE_FRAME(gSpriteBank18Frame009, SPRITE_TILES_BANK18 + 0x02c60),
    { { -11, -67, 24, 32 } },
};
const struct sprite_frame_1box gSpriteBank18Frame010 = {
    SPRITE_FRAME(gSpriteBank18Frame010, SPRITE_TILES_BANK18 + 0x02ee0),
    { { -10, -67, 22, 40 } },
};
const struct sprite_frame_1box gSpriteBank18Frame011 = {
    SPRITE_FRAME(gSpriteBank18Frame011, SPRITE_TILES_BANK18 + 0x031e0),
    { { -11, -67, 23, 48 } },
};
const struct sprite_frame_1box gSpriteBank18Frame012 = {
    SPRITE_FRAME(gSpriteBank18Frame012, SPRITE_TILES_BANK18 + 0x035e0),
    { { -11, -67, 23, 75 } },
};
const struct sprite_frame_1box gSpriteBank18Frame013 = {
    SPRITE_FRAME(gSpriteBank18Frame013, SPRITE_TILES_BANK18 + 0x00000),
    { { -11, -68, 23, 113 } },
};

const struct sprite_piece_pos gSpriteBank18Frame000Pos[2] = { { -11, -67 }, { -15, -3 } };
const struct sprite_piece_pos gSpriteBank18Frame001Pos[2] = { { -11, -67 }, { -15, -3 } };
const struct sprite_piece_pos gSpriteBank18Frame002Pos[3] = { { -11, -67 }, { -13, -3 }, { -15, 29 } };
const struct sprite_piece_pos gSpriteBank18Frame003Pos[2] = { { -11, -67 }, { -15, -3 } };
const struct sprite_piece_pos gSpriteBank18Frame004Pos[3] = { { -15, -67 }, { -8, -3 }, { 8, -3 } };
const struct sprite_piece_pos gSpriteBank18Frame005Pos[1] = { { -15, -67 } };
const struct sprite_piece_pos gSpriteBank18Frame006Pos[1] = { { -15, -67 } };
const struct sprite_piece_pos gSpriteBank18Frame007Pos[2] = { { -11, -67 }, { -15, -35 } };
const struct sprite_piece_pos gSpriteBank18Frame008Pos[2] = { { -15, -67 }, { -13, -35 } };
const struct sprite_piece_pos gSpriteBank18Frame009Pos[2] = { { -15, -67 }, { -13, -35 } };
const struct sprite_piece_pos gSpriteBank18Frame010Pos[2] = { { -11, -67 }, { -15, -35 } };
const struct sprite_piece_pos gSpriteBank18Frame011Pos[1] = { { -15, -67 } };
const struct sprite_piece_pos gSpriteBank18Frame012Pos[2] = { { -11, -67 }, { -15, -3 } };
const struct sprite_piece_pos gSpriteBank18Frame013Pos[2] = { { -11, -67 }, { -15, -3 } };

const u8 gSpriteBank18Frame000Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11) };
const u8 gSpriteBank18Frame001Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11) };
const u8 gSpriteBank18Frame002Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank18Frame003Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 2) };
const u8 gSpriteBank18Frame004Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank18Frame005Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank18Frame006Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank18Frame007Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank18Frame008Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank18Frame009Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank18Frame010Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank18Frame011Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank18Frame012Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank18Frame013Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11) };

/* ---------------------------------------------------------------------- */
/* Bank 19: 2 animations, 35 frames, tiles in gSpriteBank19Tiles (SPRITE_TILES_BANK19). */

extern const u16 gSpriteBank19Anim00Seq[14];
extern const u16 gSpriteBank19Anim01Seq[21];
extern const struct sprite_frame_1box gSpriteBank19Frame000;
extern const struct sprite_frame_1box gSpriteBank19Frame001;
extern const struct sprite_frame_1box gSpriteBank19Frame002;
extern const struct sprite_frame_1box gSpriteBank19Frame003;
extern const struct sprite_frame_1box gSpriteBank19Frame004;
extern const struct sprite_frame_1box gSpriteBank19Frame005;
extern const struct sprite_frame_1box gSpriteBank19Frame006;
extern const struct sprite_frame_1box gSpriteBank19Frame007;
extern const struct sprite_frame_1box gSpriteBank19Frame008;
extern const struct sprite_frame_1box gSpriteBank19Frame009;
extern const struct sprite_frame_1box gSpriteBank19Frame010;
extern const struct sprite_frame_1box gSpriteBank19Frame011;
extern const struct sprite_frame_1box gSpriteBank19Frame012;
extern const struct sprite_frame_1box gSpriteBank19Frame013;
extern const struct sprite_frame_1box gSpriteBank19Frame014;
extern const struct sprite_frame_1box gSpriteBank19Frame015;
extern const struct sprite_frame_1box gSpriteBank19Frame016;
extern const struct sprite_frame_1box gSpriteBank19Frame017;
extern const struct sprite_frame_1box gSpriteBank19Frame018;
extern const struct sprite_frame_1box gSpriteBank19Frame019;
extern const struct sprite_frame_1box gSpriteBank19Frame020;
extern const struct sprite_frame_1box gSpriteBank19Frame021;
extern const struct sprite_frame_1box gSpriteBank19Frame022;
extern const struct sprite_frame_1box gSpriteBank19Frame023;
extern const struct sprite_frame_1box gSpriteBank19Frame024;
extern const struct sprite_frame_1box gSpriteBank19Frame025;
extern const struct sprite_frame_1box gSpriteBank19Frame026;
extern const struct sprite_frame_1box gSpriteBank19Frame027;
extern const struct sprite_frame_1box gSpriteBank19Frame028;
extern const struct sprite_frame_1box gSpriteBank19Frame029;
extern const struct sprite_frame_1box gSpriteBank19Frame030;
extern const struct sprite_frame_1box gSpriteBank19Frame031;
extern const struct sprite_frame_1box gSpriteBank19Frame032;
extern const struct sprite_frame_1box gSpriteBank19Frame033;
extern const struct sprite_frame_1box gSpriteBank19Frame034;
extern const struct sprite_piece_pos gSpriteBank19Frame000Pos[2];
extern const struct sprite_piece_pos gSpriteBank19Frame001Pos[2];
extern const struct sprite_piece_pos gSpriteBank19Frame002Pos[2];
extern const struct sprite_piece_pos gSpriteBank19Frame003Pos[1];
extern const struct sprite_piece_pos gSpriteBank19Frame004Pos[1];
extern const struct sprite_piece_pos gSpriteBank19Frame005Pos[1];
extern const struct sprite_piece_pos gSpriteBank19Frame006Pos[2];
extern const struct sprite_piece_pos gSpriteBank19Frame007Pos[2];
extern const struct sprite_piece_pos gSpriteBank19Frame008Pos[2];
extern const struct sprite_piece_pos gSpriteBank19Frame009Pos[3];
extern const struct sprite_piece_pos gSpriteBank19Frame010Pos[4];
extern const struct sprite_piece_pos gSpriteBank19Frame011Pos[3];
extern const struct sprite_piece_pos gSpriteBank19Frame012Pos[2];
extern const struct sprite_piece_pos gSpriteBank19Frame013Pos[1];
extern const struct sprite_piece_pos gSpriteBank19Frame014Pos[2];
extern const struct sprite_piece_pos gSpriteBank19Frame015Pos[2];
extern const struct sprite_piece_pos gSpriteBank19Frame016Pos[2];
extern const struct sprite_piece_pos gSpriteBank19Frame017Pos[1];
extern const struct sprite_piece_pos gSpriteBank19Frame018Pos[1];
extern const struct sprite_piece_pos gSpriteBank19Frame019Pos[1];
extern const struct sprite_piece_pos gSpriteBank19Frame020Pos[2];
extern const struct sprite_piece_pos gSpriteBank19Frame021Pos[2];
extern const struct sprite_piece_pos gSpriteBank19Frame022Pos[3];
extern const struct sprite_piece_pos gSpriteBank19Frame023Pos[2];
extern const struct sprite_piece_pos gSpriteBank19Frame024Pos[3];
extern const struct sprite_piece_pos gSpriteBank19Frame025Pos[4];
extern const struct sprite_piece_pos gSpriteBank19Frame026Pos[2];
extern const struct sprite_piece_pos gSpriteBank19Frame027Pos[2];
extern const struct sprite_piece_pos gSpriteBank19Frame028Pos[2];
extern const struct sprite_piece_pos gSpriteBank19Frame029Pos[2];
extern const struct sprite_piece_pos gSpriteBank19Frame030Pos[2];
extern const struct sprite_piece_pos gSpriteBank19Frame031Pos[2];
extern const struct sprite_piece_pos gSpriteBank19Frame032Pos[2];
extern const struct sprite_piece_pos gSpriteBank19Frame033Pos[2];
extern const struct sprite_piece_pos gSpriteBank19Frame034Pos[2];
extern const u8 gSpriteBank19Frame000Pieces[2];
extern const u8 gSpriteBank19Frame001Pieces[2];
extern const u8 gSpriteBank19Frame002Pieces[2];
extern const u8 gSpriteBank19Frame003Pieces[1];
extern const u8 gSpriteBank19Frame004Pieces[1];
extern const u8 gSpriteBank19Frame005Pieces[1];
extern const u8 gSpriteBank19Frame006Pieces[2];
extern const u8 gSpriteBank19Frame007Pieces[2];
extern const u8 gSpriteBank19Frame008Pieces[2];
extern const u8 gSpriteBank19Frame009Pieces[3];
extern const u8 gSpriteBank19Frame010Pieces[4];
extern const u8 gSpriteBank19Frame011Pieces[3];
extern const u8 gSpriteBank19Frame012Pieces[2];
extern const u8 gSpriteBank19Frame013Pieces[1];
extern const u8 gSpriteBank19Frame014Pieces[2];
extern const u8 gSpriteBank19Frame015Pieces[2];
extern const u8 gSpriteBank19Frame016Pieces[2];
extern const u8 gSpriteBank19Frame017Pieces[1];
extern const u8 gSpriteBank19Frame018Pieces[1];
extern const u8 gSpriteBank19Frame019Pieces[1];
extern const u8 gSpriteBank19Frame020Pieces[2];
extern const u8 gSpriteBank19Frame021Pieces[2];
extern const u8 gSpriteBank19Frame022Pieces[3];
extern const u8 gSpriteBank19Frame023Pieces[2];
extern const u8 gSpriteBank19Frame024Pieces[3];
extern const u8 gSpriteBank19Frame025Pieces[4];
extern const u8 gSpriteBank19Frame026Pieces[2];
extern const u8 gSpriteBank19Frame027Pieces[2];
extern const u8 gSpriteBank19Frame028Pieces[2];
extern const u8 gSpriteBank19Frame029Pieces[2];
extern const u8 gSpriteBank19Frame030Pieces[2];
extern const u8 gSpriteBank19Frame031Pieces[2];
extern const u8 gSpriteBank19Frame032Pieces[2];
extern const u8 gSpriteBank19Frame033Pieces[2];
extern const u8 gSpriteBank19Frame034Pieces[2];

const struct sprite_anim gSpriteBank19Anims[2] = {
    [0] = {
        .seq = gSpriteBank19Anim00Seq,
        .box = { { -17, -14, 34, 29 }, { -22, -43, 42, 58 } },
        .tileRecord = 27,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank19Anim00Seq),
        .flags = 0,
    },
    [1] = {
        .seq = gSpriteBank19Anim01Seq,
        .box = { { -17, -14, 34, 29 }, { -27, -44, 46, 59 } },
        .tileRecord = 27,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank19Anim01Seq),
        .flags = 0,
    },
};

const u16 gSpriteBank19Anim00Seq[14] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
};
const u16 gSpriteBank19Anim01Seq[21] = {
    14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34,
};

const struct sprite_frame *const gSpriteBank19Frames[35] = {
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
    SPRITE_FRAME(gSpriteBank19Frame000, SPRITE_TILES_BANK19 + 0x00000),
    { { -15, -10, 30, 11 } },
};
const struct sprite_frame_1box gSpriteBank19Frame001 = {
    SPRITE_FRAME(gSpriteBank19Frame001, SPRITE_TILES_BANK19 + 0x00220),
    { { -15, -9, 30, 10 } },
};
const struct sprite_frame_1box gSpriteBank19Frame002 = {
    SPRITE_FRAME(gSpriteBank19Frame002, SPRITE_TILES_BANK19 + 0x00440),
    { { -14, -8, 28, 9 } },
};
const struct sprite_frame_1box gSpriteBank19Frame003 = {
    SPRITE_FRAME(gSpriteBank19Frame003, SPRITE_TILES_BANK19 + 0x00660),
    { { -14, -8, 26, 9 } },
};
const struct sprite_frame_1box gSpriteBank19Frame004 = {
    SPRITE_FRAME(gSpriteBank19Frame004, SPRITE_TILES_BANK19 + 0x00860),
    { { -14, -8, 25, 9 } },
};
const struct sprite_frame_1box gSpriteBank19Frame005 = {
    SPRITE_FRAME(gSpriteBank19Frame005, SPRITE_TILES_BANK19 + 0x00a60),
    { { -14, -8, 26, 8 } },
};
const struct sprite_frame_1box gSpriteBank19Frame006 = {
    SPRITE_FRAME(gSpriteBank19Frame006, SPRITE_TILES_BANK19 + 0x00c60),
    { { -14, -8, 28, 8 } },
};
const struct sprite_frame_1box gSpriteBank19Frame007 = {
    SPRITE_FRAME(gSpriteBank19Frame007, SPRITE_TILES_BANK19 + 0x00e80),
    { { -14, -8, 29, 8 } },
};
const struct sprite_frame_1box gSpriteBank19Frame008 = {
    SPRITE_FRAME(gSpriteBank19Frame008, SPRITE_TILES_BANK19 + 0x010a0),
    { { -16, -12, 30, 6 } },
};
const struct sprite_frame_1box gSpriteBank19Frame009 = {
    SPRITE_FRAME(gSpriteBank19Frame009, SPRITE_TILES_BANK19 + 0x012c0),
    { { -20, -34, 31, 21 } },
};
const struct sprite_frame_1box gSpriteBank19Frame010 = {
    SPRITE_FRAME(gSpriteBank19Frame010, SPRITE_TILES_BANK19 + 0x015c0),
    { { -16, -39, 29, 20 } },
};
const struct sprite_frame_1box gSpriteBank19Frame011 = {
    SPRITE_FRAME(gSpriteBank19Frame011, SPRITE_TILES_BANK19 + 0x01840),
    { { -15, -23, 28, 1 } },
};
const struct sprite_frame_1box gSpriteBank19Frame012 = {
    SPRITE_FRAME(gSpriteBank19Frame012, SPRITE_TILES_BANK19 + 0x019e0),
    { { -17, -16, 35, 6 } },
};
const struct sprite_frame_1box gSpriteBank19Frame013 = {
    SPRITE_FRAME(gSpriteBank19Frame013, SPRITE_TILES_BANK19 + 0x01c00),
    { { -18, -11, 27, 12 } },
};
const struct sprite_frame_1box gSpriteBank19Frame014 = {
    SPRITE_FRAME(gSpriteBank19Frame014, SPRITE_TILES_BANK19 + 0x00000),
    { { -14, -11, 28, 13 } },
};
const struct sprite_frame_1box gSpriteBank19Frame015 = {
    SPRITE_FRAME(gSpriteBank19Frame015, SPRITE_TILES_BANK19 + 0x00220),
    { { -14, -10, 28, 12 } },
};
const struct sprite_frame_1box gSpriteBank19Frame016 = {
    SPRITE_FRAME(gSpriteBank19Frame016, SPRITE_TILES_BANK19 + 0x00440),
    { { -13, -9, 26, 11 } },
};
const struct sprite_frame_1box gSpriteBank19Frame017 = {
    SPRITE_FRAME(gSpriteBank19Frame017, SPRITE_TILES_BANK19 + 0x00660),
    { { -13, -9, 24, 11 } },
};
const struct sprite_frame_1box gSpriteBank19Frame018 = {
    SPRITE_FRAME(gSpriteBank19Frame018, SPRITE_TILES_BANK19 + 0x00860),
    { { -13, -9, 23, 11 } },
};
const struct sprite_frame_1box gSpriteBank19Frame019 = {
    SPRITE_FRAME(gSpriteBank19Frame019, SPRITE_TILES_BANK19 + 0x00a60),
    { { -13, -9, 24, 10 } },
};
const struct sprite_frame_1box gSpriteBank19Frame020 = {
    SPRITE_FRAME(gSpriteBank19Frame020, SPRITE_TILES_BANK19 + 0x00c60),
    { { -13, -9, 26, 10 } },
};
const struct sprite_frame_1box gSpriteBank19Frame021 = {
    SPRITE_FRAME(gSpriteBank19Frame021, SPRITE_TILES_BANK19 + 0x01e00),
    { { -13, -9, 28, 11 } },
};
const struct sprite_frame_1box gSpriteBank19Frame022 = {
    SPRITE_FRAME(gSpriteBank19Frame022, SPRITE_TILES_BANK19 + 0x02020),
    { { -15, -22, 28, 17 } },
};
const struct sprite_frame_1box gSpriteBank19Frame023 = {
    SPRITE_FRAME(gSpriteBank19Frame023, SPRITE_TILES_BANK19 + 0x02260),
    { { -18, -41, 29, 33 } },
};
const struct sprite_frame_1box gSpriteBank19Frame024 = {
    SPRITE_FRAME(gSpriteBank19Frame024, SPRITE_TILES_BANK19 + 0x026a0),
    { { -24, -31, 39, 14 } },
};
const struct sprite_frame_1box gSpriteBank19Frame025 = {
    SPRITE_FRAME(gSpriteBank19Frame025, SPRITE_TILES_BANK19 + 0x02940),
    { { -13, -18, 29, 9 } },
};
const struct sprite_frame_1box gSpriteBank19Frame026 = {
    SPRITE_FRAME(gSpriteBank19Frame026, SPRITE_TILES_BANK19 + 0x02b00),
    { { -16, -14, 29, 11 } },
};
const struct sprite_frame_1box gSpriteBank19Frame027 = {
    SPRITE_FRAME(gSpriteBank19Frame027, SPRITE_TILES_BANK19 + 0x02d40),
    { { -13, -8, 28, 10 } },
};
const struct sprite_frame_1box gSpriteBank19Frame028 = {
    SPRITE_FRAME(gSpriteBank19Frame028, SPRITE_TILES_BANK19 + 0x02f80),
    { { -14, -8, 30, 10 } },
};
const struct sprite_frame_1box gSpriteBank19Frame029 = {
    SPRITE_FRAME(gSpriteBank19Frame029, SPRITE_TILES_BANK19 + 0x031c0),
    { { -15, -8, 30, 10 } },
};
const struct sprite_frame_1box gSpriteBank19Frame030 = {
    SPRITE_FRAME(gSpriteBank19Frame030, SPRITE_TILES_BANK19 + 0x03400),
    { { -14, -9, 28, 11 } },
};
const struct sprite_frame_1box gSpriteBank19Frame031 = {
    SPRITE_FRAME(gSpriteBank19Frame031, SPRITE_TILES_BANK19 + 0x03640),
    { { -13, -9, 27, 11 } },
};
const struct sprite_frame_1box gSpriteBank19Frame032 = {
    SPRITE_FRAME(gSpriteBank19Frame032, SPRITE_TILES_BANK19 + 0x03860),
    { { -13, -9, 27, 11 } },
};
const struct sprite_frame_1box gSpriteBank19Frame033 = {
    SPRITE_FRAME(gSpriteBank19Frame033, SPRITE_TILES_BANK19 + 0x03a80),
    { { -13, -10, 27, 12 } },
};
const struct sprite_frame_1box gSpriteBank19Frame034 = {
    SPRITE_FRAME(gSpriteBank19Frame034, SPRITE_TILES_BANK19 + 0x03ca0),
    { { -14, -11, 28, 13 } },
};

const struct sprite_piece_pos gSpriteBank19Frame000Pos[2] = { { -17, -14 }, { 15, -8 } };
const struct sprite_piece_pos gSpriteBank19Frame001Pos[2] = { { -17, -13 }, { 15, -7 } };
const struct sprite_piece_pos gSpriteBank19Frame002Pos[2] = { { -16, -12 }, { 16, -3 } };
const struct sprite_piece_pos gSpriteBank19Frame003Pos[1] = { { -16, -12 } };
const struct sprite_piece_pos gSpriteBank19Frame004Pos[1] = { { -16, -12 } };
const struct sprite_piece_pos gSpriteBank19Frame005Pos[1] = { { -16, -12 } };
const struct sprite_piece_pos gSpriteBank19Frame006Pos[2] = { { -16, -12 }, { 16, 0 } };
const struct sprite_piece_pos gSpriteBank19Frame007Pos[2] = { { -16, -12 }, { 16, -3 } };
const struct sprite_piece_pos gSpriteBank19Frame008Pos[2] = { { -18, -16 }, { 14, -12 } };
const struct sprite_piece_pos gSpriteBank19Frame009Pos[3] = { { -22, -38 }, { 10, -37 }, { -16, -6 } };
const struct sprite_piece_pos gSpriteBank19Frame010Pos[4] = { { -18, -43 }, { 14, -20 }, { -12, -11 }, { 4, -11 } };
const struct sprite_piece_pos gSpriteBank19Frame011Pos[3] = { { -16, -27 }, { 15, -16 }, { -17, -11 } };
const struct sprite_piece_pos gSpriteBank19Frame012Pos[2] = { { -19, -20 }, { 13, -19 } };
const struct sprite_piece_pos gSpriteBank19Frame013Pos[1] = { { -20, -15 } };
const struct sprite_piece_pos gSpriteBank19Frame014Pos[2] = { { -17, -14 }, { 15, -8 } };
const struct sprite_piece_pos gSpriteBank19Frame015Pos[2] = { { -17, -13 }, { 15, -7 } };
const struct sprite_piece_pos gSpriteBank19Frame016Pos[2] = { { -16, -12 }, { 16, -3 } };
const struct sprite_piece_pos gSpriteBank19Frame017Pos[1] = { { -16, -12 } };
const struct sprite_piece_pos gSpriteBank19Frame018Pos[1] = { { -16, -12 } };
const struct sprite_piece_pos gSpriteBank19Frame019Pos[1] = { { -16, -12 } };
const struct sprite_piece_pos gSpriteBank19Frame020Pos[2] = { { -16, -12 }, { 16, 0 } };
const struct sprite_piece_pos gSpriteBank19Frame021Pos[2] = { { -16, -12 }, { 16, -6 } };
const struct sprite_piece_pos gSpriteBank19Frame022Pos[3] = { { -18, -25 }, { 14, -24 }, { -18, 7 } };
const struct sprite_piece_pos gSpriteBank19Frame023Pos[2] = { { -21, -43 }, { 11, -44 } };
const struct sprite_piece_pos gSpriteBank19Frame024Pos[3] = { { -27, -34 }, { 5, -34 }, { 13, -31 } };
const struct sprite_piece_pos gSpriteBank19Frame025Pos[4] = { { -27, -22 }, { 5, -19 }, { -6, -6 }, { 15, -6 } };
const struct sprite_piece_pos gSpriteBank19Frame026Pos[2] = { { -19, -17 }, { 13, -9 } };
const struct sprite_piece_pos gSpriteBank19Frame027Pos[2] = { { -16, -11 }, { 16, 0 } };
const struct sprite_piece_pos gSpriteBank19Frame028Pos[2] = { { -17, -11 }, { 15, -3 } };
const struct sprite_piece_pos gSpriteBank19Frame029Pos[2] = { { -18, -11 }, { 14, -5 } };
const struct sprite_piece_pos gSpriteBank19Frame030Pos[2] = { { -17, -12 }, { 15, -2 } };
const struct sprite_piece_pos gSpriteBank19Frame031Pos[2] = { { -16, -12 }, { 16, -1 } };
const struct sprite_piece_pos gSpriteBank19Frame032Pos[2] = { { -16, -12 }, { 16, -4 } };
const struct sprite_piece_pos gSpriteBank19Frame033Pos[2] = { { -16, -13 }, { 16, -6 } };
const struct sprite_piece_pos gSpriteBank19Frame034Pos[2] = { { -17, -14 }, { 15, -8 } };

const u8 gSpriteBank19Frame000Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank19Frame001Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank19Frame002Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank19Frame003Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank19Frame004Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank19Frame005Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank19Frame006Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank19Frame007Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank19Frame008Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank19Frame009Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank19Frame010Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank19Frame011Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank19Frame012Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank19Frame013Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank19Frame014Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank19Frame015Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank19Frame016Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank19Frame017Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank19Frame018Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank19Frame019Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank19Frame020Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank19Frame021Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank19Frame022Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank19Frame023Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank19Frame024Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank19Frame025Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank19Frame026Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank19Frame027Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank19Frame028Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank19Frame029Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank19Frame030Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank19Frame031Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank19Frame032Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank19Frame033Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank19Frame034Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };

/* ---------------------------------------------------------------------- */
/* Bank 20: 2 animations, 32 frames, tiles in gSpriteBank20Tiles (SPRITE_TILES_BANK20). */

extern const u16 gSpriteBank20Anim00Seq[16];
extern const u16 gSpriteBank20Anim01Seq[16];
extern const struct sprite_frame_1box gSpriteBank20Frame000;
extern const struct sprite_frame_1box gSpriteBank20Frame001;
extern const struct sprite_frame_1box gSpriteBank20Frame002;
extern const struct sprite_frame_1box gSpriteBank20Frame003;
extern const struct sprite_frame_1box gSpriteBank20Frame004;
extern const struct sprite_frame_1box gSpriteBank20Frame005;
extern const struct sprite_frame_1box gSpriteBank20Frame006;
extern const struct sprite_frame_1box gSpriteBank20Frame007;
extern const struct sprite_frame_1box gSpriteBank20Frame008;
extern const struct sprite_frame_1box gSpriteBank20Frame009;
extern const struct sprite_frame_1box gSpriteBank20Frame010;
extern const struct sprite_frame_1box gSpriteBank20Frame011;
extern const struct sprite_frame_1box gSpriteBank20Frame012;
extern const struct sprite_frame_1box gSpriteBank20Frame013;
extern const struct sprite_frame_1box gSpriteBank20Frame014;
extern const struct sprite_frame_1box gSpriteBank20Frame015;
extern const struct sprite_frame_1box gSpriteBank20Frame016;
extern const struct sprite_frame_1box gSpriteBank20Frame017;
extern const struct sprite_frame_1box gSpriteBank20Frame018;
extern const struct sprite_frame_1box gSpriteBank20Frame019;
extern const struct sprite_frame_1box gSpriteBank20Frame020;
extern const struct sprite_frame_1box gSpriteBank20Frame021;
extern const struct sprite_frame_1box gSpriteBank20Frame022;
extern const struct sprite_frame_1box gSpriteBank20Frame023;
extern const struct sprite_frame_1box gSpriteBank20Frame024;
extern const struct sprite_frame_1box gSpriteBank20Frame025;
extern const struct sprite_frame_1box gSpriteBank20Frame026;
extern const struct sprite_frame_1box gSpriteBank20Frame027;
extern const struct sprite_frame_1box gSpriteBank20Frame028;
extern const struct sprite_frame_1box gSpriteBank20Frame029;
extern const struct sprite_frame_1box gSpriteBank20Frame030;
extern const struct sprite_frame_1box gSpriteBank20Frame031;
extern const struct sprite_piece_pos gSpriteBank20Frame000Pos[2];
extern const struct sprite_piece_pos gSpriteBank20Frame001Pos[2];
extern const struct sprite_piece_pos gSpriteBank20Frame002Pos[2];
extern const struct sprite_piece_pos gSpriteBank20Frame003Pos[2];
extern const struct sprite_piece_pos gSpriteBank20Frame004Pos[2];
extern const struct sprite_piece_pos gSpriteBank20Frame005Pos[2];
extern const struct sprite_piece_pos gSpriteBank20Frame006Pos[2];
extern const struct sprite_piece_pos gSpriteBank20Frame007Pos[3];
extern const struct sprite_piece_pos gSpriteBank20Frame008Pos[3];
extern const struct sprite_piece_pos gSpriteBank20Frame009Pos[2];
extern const struct sprite_piece_pos gSpriteBank20Frame010Pos[2];
extern const struct sprite_piece_pos gSpriteBank20Frame011Pos[2];
extern const struct sprite_piece_pos gSpriteBank20Frame012Pos[2];
extern const struct sprite_piece_pos gSpriteBank20Frame013Pos[2];
extern const struct sprite_piece_pos gSpriteBank20Frame014Pos[2];
extern const struct sprite_piece_pos gSpriteBank20Frame015Pos[2];
extern const struct sprite_piece_pos gSpriteBank20Frame016Pos[2];
extern const struct sprite_piece_pos gSpriteBank20Frame017Pos[2];
extern const struct sprite_piece_pos gSpriteBank20Frame018Pos[2];
extern const struct sprite_piece_pos gSpriteBank20Frame019Pos[2];
extern const struct sprite_piece_pos gSpriteBank20Frame020Pos[2];
extern const struct sprite_piece_pos gSpriteBank20Frame021Pos[2];
extern const struct sprite_piece_pos gSpriteBank20Frame022Pos[2];
extern const struct sprite_piece_pos gSpriteBank20Frame023Pos[2];
extern const struct sprite_piece_pos gSpriteBank20Frame024Pos[2];
extern const struct sprite_piece_pos gSpriteBank20Frame025Pos[2];
extern const struct sprite_piece_pos gSpriteBank20Frame026Pos[2];
extern const struct sprite_piece_pos gSpriteBank20Frame027Pos[2];
extern const struct sprite_piece_pos gSpriteBank20Frame028Pos[2];
extern const struct sprite_piece_pos gSpriteBank20Frame029Pos[2];
extern const struct sprite_piece_pos gSpriteBank20Frame030Pos[2];
extern const struct sprite_piece_pos gSpriteBank20Frame031Pos[2];
extern const u8 gSpriteBank20Frame000Pieces[2];
extern const u8 gSpriteBank20Frame001Pieces[2];
extern const u8 gSpriteBank20Frame002Pieces[2];
extern const u8 gSpriteBank20Frame003Pieces[2];
extern const u8 gSpriteBank20Frame004Pieces[2];
extern const u8 gSpriteBank20Frame005Pieces[2];
extern const u8 gSpriteBank20Frame006Pieces[2];
extern const u8 gSpriteBank20Frame007Pieces[3];
extern const u8 gSpriteBank20Frame008Pieces[3];
extern const u8 gSpriteBank20Frame009Pieces[2];
extern const u8 gSpriteBank20Frame010Pieces[2];
extern const u8 gSpriteBank20Frame011Pieces[2];
extern const u8 gSpriteBank20Frame012Pieces[2];
extern const u8 gSpriteBank20Frame013Pieces[2];
extern const u8 gSpriteBank20Frame014Pieces[2];
extern const u8 gSpriteBank20Frame015Pieces[2];
extern const u8 gSpriteBank20Frame016Pieces[2];
extern const u8 gSpriteBank20Frame017Pieces[2];
extern const u8 gSpriteBank20Frame018Pieces[2];
extern const u8 gSpriteBank20Frame019Pieces[2];
extern const u8 gSpriteBank20Frame020Pieces[2];
extern const u8 gSpriteBank20Frame021Pieces[2];
extern const u8 gSpriteBank20Frame022Pieces[2];
extern const u8 gSpriteBank20Frame023Pieces[2];
extern const u8 gSpriteBank20Frame024Pieces[2];
extern const u8 gSpriteBank20Frame025Pieces[2];
extern const u8 gSpriteBank20Frame026Pieces[2];
extern const u8 gSpriteBank20Frame027Pieces[2];
extern const u8 gSpriteBank20Frame028Pieces[2];
extern const u8 gSpriteBank20Frame029Pieces[2];
extern const u8 gSpriteBank20Frame030Pieces[2];
extern const u8 gSpriteBank20Frame031Pieces[2];

const struct sprite_anim gSpriteBank20Anims[2] = {
    [0] = {
        .seq = gSpriteBank20Anim00Seq,
        .box = { { -14, -9, 28, 19 }, { -15, -9, 30, 19 } },
        .tileRecord = 28,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank20Anim00Seq),
        .flags = 0,
    },
    [1] = {
        .seq = gSpriteBank20Anim01Seq,
        .box = { { -14, -9, 28, 19 }, { -15, -9, 29, 19 } },
        .tileRecord = 28,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank20Anim01Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank20Anim00Seq[16] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
};
const u16 gSpriteBank20Anim01Seq[16] = {
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
};

const struct sprite_frame *const gSpriteBank20Frames[32] = {
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
    SPRITE_FRAME(gSpriteBank20Frame000, SPRITE_TILES_BANK20 + 0x00000),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame001 = {
    SPRITE_FRAME(gSpriteBank20Frame001, SPRITE_TILES_BANK20 + 0x00120),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame002 = {
    SPRITE_FRAME(gSpriteBank20Frame002, SPRITE_TILES_BANK20 + 0x00240),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame003 = {
    SPRITE_FRAME(gSpriteBank20Frame003, SPRITE_TILES_BANK20 + 0x00360),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame004 = {
    SPRITE_FRAME(gSpriteBank20Frame004, SPRITE_TILES_BANK20 + 0x00480),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame005 = {
    SPRITE_FRAME(gSpriteBank20Frame005, SPRITE_TILES_BANK20 + 0x005a0),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame006 = {
    SPRITE_FRAME(gSpriteBank20Frame006, SPRITE_TILES_BANK20 + 0x006c0),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame007 = {
    SPRITE_FRAME(gSpriteBank20Frame007, SPRITE_TILES_BANK20 + 0x007e0),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame008 = {
    SPRITE_FRAME(gSpriteBank20Frame008, SPRITE_TILES_BANK20 + 0x00920),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame009 = {
    SPRITE_FRAME(gSpriteBank20Frame009, SPRITE_TILES_BANK20 + 0x00a00),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame010 = {
    SPRITE_FRAME(gSpriteBank20Frame010, SPRITE_TILES_BANK20 + 0x00b20),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame011 = {
    SPRITE_FRAME(gSpriteBank20Frame011, SPRITE_TILES_BANK20 + 0x00c40),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame012 = {
    SPRITE_FRAME(gSpriteBank20Frame012, SPRITE_TILES_BANK20 + 0x00d60),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame013 = {
    SPRITE_FRAME(gSpriteBank20Frame013, SPRITE_TILES_BANK20 + 0x00e80),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame014 = {
    SPRITE_FRAME(gSpriteBank20Frame014, SPRITE_TILES_BANK20 + 0x00fa0),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame015 = {
    SPRITE_FRAME(gSpriteBank20Frame015, SPRITE_TILES_BANK20 + 0x010c0),
    { { -12, -8, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank20Frame016 = {
    SPRITE_FRAME(gSpriteBank20Frame016, SPRITE_TILES_BANK20 + 0x011e0),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame017 = {
    SPRITE_FRAME(gSpriteBank20Frame017, SPRITE_TILES_BANK20 + 0x01300),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame018 = {
    SPRITE_FRAME(gSpriteBank20Frame018, SPRITE_TILES_BANK20 + 0x01420),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame019 = {
    SPRITE_FRAME(gSpriteBank20Frame019, SPRITE_TILES_BANK20 + 0x01540),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame020 = {
    SPRITE_FRAME(gSpriteBank20Frame020, SPRITE_TILES_BANK20 + 0x01660),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame021 = {
    SPRITE_FRAME(gSpriteBank20Frame021, SPRITE_TILES_BANK20 + 0x01780),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame022 = {
    SPRITE_FRAME(gSpriteBank20Frame022, SPRITE_TILES_BANK20 + 0x018a0),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame023 = {
    SPRITE_FRAME(gSpriteBank20Frame023, SPRITE_TILES_BANK20 + 0x019c0),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame024 = {
    SPRITE_FRAME(gSpriteBank20Frame024, SPRITE_TILES_BANK20 + 0x01ae0),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame025 = {
    SPRITE_FRAME(gSpriteBank20Frame025, SPRITE_TILES_BANK20 + 0x01c00),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame026 = {
    SPRITE_FRAME(gSpriteBank20Frame026, SPRITE_TILES_BANK20 + 0x01d20),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame027 = {
    SPRITE_FRAME(gSpriteBank20Frame027, SPRITE_TILES_BANK20 + 0x01e40),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame028 = {
    SPRITE_FRAME(gSpriteBank20Frame028, SPRITE_TILES_BANK20 + 0x01f60),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame029 = {
    SPRITE_FRAME(gSpriteBank20Frame029, SPRITE_TILES_BANK20 + 0x02080),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame030 = {
    SPRITE_FRAME(gSpriteBank20Frame030, SPRITE_TILES_BANK20 + 0x021a0),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank20Frame031 = {
    SPRITE_FRAME(gSpriteBank20Frame031, SPRITE_TILES_BANK20 + 0x022c0),
    { { -12, -8, 24, 17 } },
};

const struct sprite_piece_pos gSpriteBank20Frame000Pos[2] = { { -14, -9 }, { 1, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame001Pos[2] = { { -13, -9 }, { 1, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame002Pos[2] = { { -14, -9 }, { 2, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame003Pos[2] = { { -14, -9 }, { 1, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame004Pos[2] = { { -14, -9 }, { 0, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame005Pos[2] = { { -14, -9 }, { -3, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame006Pos[2] = { { -13, -9 }, { 1, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame007Pos[3] = { { -12, -9 }, { -8, 7 }, { 0, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame008Pos[3] = { { -11, -9 }, { 5, -9 }, { -1, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame009Pos[2] = { { -12, -9 }, { -2, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame010Pos[2] = { { -14, -9 }, { -3, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame011Pos[2] = { { -15, -9 }, { -3, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame012Pos[2] = { { -15, -9 }, { -3, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame013Pos[2] = { { -15, -9 }, { -3, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame014Pos[2] = { { -15, -9 }, { -3, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame015Pos[2] = { { -15, -9 }, { -3, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame016Pos[2] = { { -14, -9 }, { 1, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame017Pos[2] = { { -13, -9 }, { 1, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame018Pos[2] = { { -13, -9 }, { 2, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame019Pos[2] = { { -14, -9 }, { 2, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame020Pos[2] = { { -14, -9 }, { 2, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame021Pos[2] = { { -14, -9 }, { 2, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame022Pos[2] = { { -14, -9 }, { 1, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame023Pos[2] = { { -14, -9 }, { 0, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame024Pos[2] = { { -14, -9 }, { 0, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame025Pos[2] = { { -13, -9 }, { 0, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame026Pos[2] = { { -14, -9 }, { -1, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame027Pos[2] = { { -15, -9 }, { -1, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame028Pos[2] = { { -14, -9 }, { -1, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame029Pos[2] = { { -14, -9 }, { 0, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame030Pos[2] = { { -14, -9 }, { 0, 7 } };
const struct sprite_piece_pos gSpriteBank20Frame031Pos[2] = { { -14, -9 }, { 0, 7 } };

const u8 gSpriteBank20Frame000Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame001Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame002Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame003Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame004Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame005Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame006Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame007Pieces[3] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame008Pieces[3] = { SPRITE_PIECE(5, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame009Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame010Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame011Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame012Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame013Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame014Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame015Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame016Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame017Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame018Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame019Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame020Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame021Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame022Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame023Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame024Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame025Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame026Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame027Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame028Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame029Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame030Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank20Frame031Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };

/* ---------------------------------------------------------------------- */
/* Bank 21: 2 animations, 15 frames, tiles in gSpriteBank21Tiles (SPRITE_TILES_BANK21). */

extern const u16 gSpriteBank21Anim00Seq[8];
extern const u16 gSpriteBank21Anim01Seq[7];
extern const struct sprite_frame_1box gSpriteBank21Frame000;
extern const struct sprite_frame_1box gSpriteBank21Frame001;
extern const struct sprite_frame_1box gSpriteBank21Frame002;
extern const struct sprite_frame_1box gSpriteBank21Frame003;
extern const struct sprite_frame_1box gSpriteBank21Frame004;
extern const struct sprite_frame_1box gSpriteBank21Frame005;
extern const struct sprite_frame_1box gSpriteBank21Frame006;
extern const struct sprite_frame_1box gSpriteBank21Frame007;
extern const struct sprite_frame_1box gSpriteBank21Frame008;
extern const struct sprite_frame_1box gSpriteBank21Frame009;
extern const struct sprite_frame_1box gSpriteBank21Frame010;
extern const struct sprite_frame_1box gSpriteBank21Frame011;
extern const struct sprite_frame_1box gSpriteBank21Frame012;
extern const struct sprite_frame_1box gSpriteBank21Frame013;
extern const struct sprite_frame_1box gSpriteBank21Frame014;
extern const struct sprite_piece_pos gSpriteBank21Frame000Pos[4];
extern const struct sprite_piece_pos gSpriteBank21Frame001Pos[4];
extern const struct sprite_piece_pos gSpriteBank21Frame002Pos[4];
extern const struct sprite_piece_pos gSpriteBank21Frame003Pos[3];
extern const struct sprite_piece_pos gSpriteBank21Frame004Pos[2];
extern const struct sprite_piece_pos gSpriteBank21Frame005Pos[2];
extern const struct sprite_piece_pos gSpriteBank21Frame006Pos[3];
extern const struct sprite_piece_pos gSpriteBank21Frame007Pos[3];
extern const struct sprite_piece_pos gSpriteBank21Frame008Pos[5];
extern const struct sprite_piece_pos gSpriteBank21Frame009Pos[2];
extern const struct sprite_piece_pos gSpriteBank21Frame010Pos[4];
extern const struct sprite_piece_pos gSpriteBank21Frame011Pos[3];
extern const struct sprite_piece_pos gSpriteBank21Frame012Pos[3];
extern const struct sprite_piece_pos gSpriteBank21Frame013Pos[3];
extern const struct sprite_piece_pos gSpriteBank21Frame014Pos[3];
extern const u8 gSpriteBank21Frame000Pieces[4];
extern const u8 gSpriteBank21Frame001Pieces[4];
extern const u8 gSpriteBank21Frame002Pieces[4];
extern const u8 gSpriteBank21Frame003Pieces[3];
extern const u8 gSpriteBank21Frame004Pieces[2];
extern const u8 gSpriteBank21Frame005Pieces[2];
extern const u8 gSpriteBank21Frame006Pieces[3];
extern const u8 gSpriteBank21Frame007Pieces[3];
extern const u8 gSpriteBank21Frame008Pieces[5];
extern const u8 gSpriteBank21Frame009Pieces[2];
extern const u8 gSpriteBank21Frame010Pieces[4];
extern const u8 gSpriteBank21Frame011Pieces[3];
extern const u8 gSpriteBank21Frame012Pieces[3];
extern const u8 gSpriteBank21Frame013Pieces[3];
extern const u8 gSpriteBank21Frame014Pieces[3];

const struct sprite_anim gSpriteBank21Anims[2] = {
    [0] = {
        .seq = gSpriteBank21Anim00Seq,
        .box = { { -21, -10, 43, 21 }, { -22, -11, 46, 22 } },
        .tileRecord = 29,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank21Anim00Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [1] = {
        .seq = gSpriteBank21Anim01Seq,
        .box = { { -21, -10, 43, 21 }, { -20, -11, 42, 21 } },
        .tileRecord = 29,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank21Anim01Seq),
        .flags = 0,
    },
};

const u16 gSpriteBank21Anim00Seq[8] = {
    0, 1, 2, 3, 4, 5, 6, 7,
};
const u16 gSpriteBank21Anim01Seq[7] = {
    8, 9, 10, 11, 12, 13, 14,
};

const struct sprite_frame *const gSpriteBank21Frames[15] = {
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
    SPRITE_FRAME(gSpriteBank21Frame000, SPRITE_TILES_BANK21 + 0x00000),
    { { -6, -7, 25, 15 } },
};
const struct sprite_frame_1box gSpriteBank21Frame001 = {
    SPRITE_FRAME(gSpriteBank21Frame001, SPRITE_TILES_BANK21 + 0x00200),
    { { -7, -7, 27, 14 } },
};
const struct sprite_frame_1box gSpriteBank21Frame002 = {
    SPRITE_FRAME(gSpriteBank21Frame002, SPRITE_TILES_BANK21 + 0x00440),
    { { -3, -6, 24, 12 } },
};
const struct sprite_frame_1box gSpriteBank21Frame003 = {
    SPRITE_FRAME(gSpriteBank21Frame003, SPRITE_TILES_BANK21 + 0x00640),
    { { -4, -4, 24, 13 } },
};
const struct sprite_frame_1box gSpriteBank21Frame004 = {
    SPRITE_FRAME(gSpriteBank21Frame004, SPRITE_TILES_BANK21 + 0x007e0),
    { { -6, -4, 25, 13 } },
};
const struct sprite_frame_1box gSpriteBank21Frame005 = {
    SPRITE_FRAME(gSpriteBank21Frame005, SPRITE_TILES_BANK21 + 0x00960),
    { { -7, -3, 26, 12 } },
};
const struct sprite_frame_1box gSpriteBank21Frame006 = {
    SPRITE_FRAME(gSpriteBank21Frame006, SPRITE_TILES_BANK21 + 0x00ae0),
    { { -6, -1, 24, 10 } },
};
const struct sprite_frame_1box gSpriteBank21Frame007 = {
    SPRITE_FRAME(gSpriteBank21Frame007, SPRITE_TILES_BANK21 + 0x00ca0),
    { { -5, -2, 21, 10 } },
};
const struct sprite_frame_1box gSpriteBank21Frame008 = {
    SPRITE_FRAME(gSpriteBank21Frame008, SPRITE_TILES_BANK21 + 0x00e40),
    { { -4, -2, 22, 11 } },
};
const struct sprite_frame_1box gSpriteBank21Frame009 = {
    SPRITE_FRAME(gSpriteBank21Frame009, SPRITE_TILES_BANK21 + 0x01040),
    { { -4, -3, 19, 12 } },
};
const struct sprite_frame_1box gSpriteBank21Frame010 = {
    SPRITE_FRAME(gSpriteBank21Frame010, SPRITE_TILES_BANK21 + 0x011c0),
    { { -3, -3, 17, 11 } },
};
const struct sprite_frame_1box gSpriteBank21Frame011 = {
    SPRITE_FRAME(gSpriteBank21Frame011, SPRITE_TILES_BANK21 + 0x012e0),
    { { -4, -5, 18, 14 } },
};
const struct sprite_frame_1box gSpriteBank21Frame012 = {
    SPRITE_FRAME(gSpriteBank21Frame012, SPRITE_TILES_BANK21 + 0x013c0),
    { { -7, -3, 18, 13 } },
};
const struct sprite_frame_1box gSpriteBank21Frame013 = {
    SPRITE_FRAME(gSpriteBank21Frame013, SPRITE_TILES_BANK21 + 0x014c0),
    { { -12, -2, 19, 11 } },
};
const struct sprite_frame_1box gSpriteBank21Frame014 = {
    SPRITE_FRAME(gSpriteBank21Frame014, SPRITE_TILES_BANK21 + 0x01660),
    { { -15, -3, 24, 12 } },
};

const struct sprite_piece_pos gSpriteBank21Frame000Pos[4] = { { -8, -7 }, { 11, -10 }, { -21, 6 }, { 11, 6 } };
const struct sprite_piece_pos gSpriteBank21Frame001Pos[4] = { { -9, -5 }, { 10, -10 }, { -22, 6 }, { 10, 6 } };
const struct sprite_piece_pos gSpriteBank21Frame002Pos[4] = { { -18, -5 }, { 14, -9 }, { 22, -9 }, { -5, 7 } };
const struct sprite_piece_pos gSpriteBank21Frame003Pos[3] = { { -11, -9 }, { 21, 5 }, { -6, 7 } };
const struct sprite_piece_pos gSpriteBank21Frame004Pos[2] = { { -6, -11 }, { -7, 5 } };
const struct sprite_piece_pos gSpriteBank21Frame005Pos[2] = { { -7, -10 }, { -8, 6 } };
const struct sprite_piece_pos gSpriteBank21Frame006Pos[3] = { { -9, -9 }, { 23, -9 }, { -6, 7 } };
const struct sprite_piece_pos gSpriteBank21Frame007Pos[3] = { { -16, -9 }, { 16, -1 }, { -4, 7 } };
const struct sprite_piece_pos gSpriteBank21Frame008Pos[5] = { { -12, -6 }, { 12, -10 }, { -20, 6 }, { 12, 6 }, { 20, 6 } };
const struct sprite_piece_pos gSpriteBank21Frame009Pos[2] = { { -13, -9 }, { -6, 7 } };
const struct sprite_piece_pos gSpriteBank21Frame010Pos[4] = { { -6, -9 }, { 10, -8 }, { -4, 7 }, { 12, 7 } };
const struct sprite_piece_pos gSpriteBank21Frame011Pos[3] = { { -5, -11 }, { 11, -7 }, { -4, 5 } };
const struct sprite_piece_pos gSpriteBank21Frame012Pos[3] = { { -7, -10 }, { 9, -5 }, { -7, 6 } };
const struct sprite_piece_pos gSpriteBank21Frame013Pos[3] = { { -13, -9 }, { 16, 3 }, { -16, 7 } };
const struct sprite_piece_pos gSpriteBank21Frame014Pos[3] = { { -17, -9 }, { 13, 2 }, { -19, 7 } };

const u8 gSpriteBank21Frame000Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank21Frame001Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank21Frame002Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank21Frame003Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank21Frame004Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank21Frame005Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank21Frame006Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank21Frame007Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank21Frame008Pieces[5] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank21Frame009Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank21Frame010Pieces[4] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank21Frame011Pieces[3] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank21Frame012Pieces[3] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank21Frame013Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank21Frame014Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5) };
