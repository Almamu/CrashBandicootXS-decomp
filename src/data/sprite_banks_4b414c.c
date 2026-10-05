#include "gba/types.h"
#include "sprite_bank.h"

/*
 * ROM 0x084b414c-0x084b9d7c: sprite banks 22-38 of the sprite-bank
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
/* Bank 22: 2 animations, 31 frames, tiles in gSpriteBank22Tiles (SPRITE_TILES_BANK22). */

extern const u16 gSpriteBank22Anim00Seq[16];
extern const u16 gSpriteBank22Anim01Seq[15];
extern const struct sprite_frame_1box gSpriteBank22Frame000;
extern const struct sprite_frame_1box gSpriteBank22Frame001;
extern const struct sprite_frame_1box gSpriteBank22Frame002;
extern const struct sprite_frame_1box gSpriteBank22Frame003;
extern const struct sprite_frame_1box gSpriteBank22Frame004;
extern const struct sprite_frame_1box gSpriteBank22Frame005;
extern const struct sprite_frame_1box gSpriteBank22Frame006;
extern const struct sprite_frame_1box gSpriteBank22Frame007;
extern const struct sprite_frame_1box gSpriteBank22Frame008;
extern const struct sprite_frame_1box gSpriteBank22Frame009;
extern const struct sprite_frame_1box gSpriteBank22Frame010;
extern const struct sprite_frame_1box gSpriteBank22Frame011;
extern const struct sprite_frame_1box gSpriteBank22Frame012;
extern const struct sprite_frame_1box gSpriteBank22Frame013;
extern const struct sprite_frame_1box gSpriteBank22Frame014;
extern const struct sprite_frame_1box gSpriteBank22Frame015;
extern const struct sprite_frame gSpriteBank22Frame016;
extern const struct sprite_frame gSpriteBank22Frame017;
extern const struct sprite_frame gSpriteBank22Frame018;
extern const struct sprite_frame gSpriteBank22Frame019;
extern const struct sprite_frame gSpriteBank22Frame020;
extern const struct sprite_frame gSpriteBank22Frame021;
extern const struct sprite_frame gSpriteBank22Frame022;
extern const struct sprite_frame gSpriteBank22Frame023;
extern const struct sprite_frame gSpriteBank22Frame024;
extern const struct sprite_frame gSpriteBank22Frame025;
extern const struct sprite_frame gSpriteBank22Frame026;
extern const struct sprite_frame gSpriteBank22Frame027;
extern const struct sprite_frame gSpriteBank22Frame028;
extern const struct sprite_frame gSpriteBank22Frame029;
extern const struct sprite_frame gSpriteBank22Frame030;
extern const struct sprite_piece_pos gSpriteBank22Frame000Pos[2];
extern const struct sprite_piece_pos gSpriteBank22Frame001Pos[2];
extern const struct sprite_piece_pos gSpriteBank22Frame002Pos[3];
extern const struct sprite_piece_pos gSpriteBank22Frame003Pos[4];
extern const struct sprite_piece_pos gSpriteBank22Frame004Pos[3];
extern const struct sprite_piece_pos gSpriteBank22Frame005Pos[2];
extern const struct sprite_piece_pos gSpriteBank22Frame006Pos[3];
extern const struct sprite_piece_pos gSpriteBank22Frame007Pos[2];
extern const struct sprite_piece_pos gSpriteBank22Frame008Pos[2];
extern const struct sprite_piece_pos gSpriteBank22Frame009Pos[2];
extern const struct sprite_piece_pos gSpriteBank22Frame010Pos[2];
extern const struct sprite_piece_pos gSpriteBank22Frame011Pos[2];
extern const struct sprite_piece_pos gSpriteBank22Frame012Pos[2];
extern const struct sprite_piece_pos gSpriteBank22Frame013Pos[2];
extern const struct sprite_piece_pos gSpriteBank22Frame014Pos[2];
extern const struct sprite_piece_pos gSpriteBank22Frame015Pos[2];
extern const struct sprite_piece_pos gSpriteBank22Frame016Pos[2];
extern const struct sprite_piece_pos gSpriteBank22Frame017Pos[3];
extern const struct sprite_piece_pos gSpriteBank22Frame018Pos[3];
extern const struct sprite_piece_pos gSpriteBank22Frame019Pos[3];
extern const struct sprite_piece_pos gSpriteBank22Frame020Pos[3];
extern const struct sprite_piece_pos gSpriteBank22Frame021Pos[2];
extern const struct sprite_piece_pos gSpriteBank22Frame022Pos[2];
extern const struct sprite_piece_pos gSpriteBank22Frame023Pos[2];
extern const struct sprite_piece_pos gSpriteBank22Frame024Pos[2];
extern const struct sprite_piece_pos gSpriteBank22Frame025Pos[2];
extern const struct sprite_piece_pos gSpriteBank22Frame026Pos[3];
extern const struct sprite_piece_pos gSpriteBank22Frame027Pos[2];
extern const struct sprite_piece_pos gSpriteBank22Frame028Pos[2];
extern const struct sprite_piece_pos gSpriteBank22Frame029Pos[3];
extern const struct sprite_piece_pos gSpriteBank22Frame030Pos[2];
extern const u8 gSpriteBank22Frame000Pieces[2];
extern const u8 gSpriteBank22Frame001Pieces[2];
extern const u8 gSpriteBank22Frame002Pieces[3];
extern const u8 gSpriteBank22Frame003Pieces[4];
extern const u8 gSpriteBank22Frame004Pieces[3];
extern const u8 gSpriteBank22Frame005Pieces[2];
extern const u8 gSpriteBank22Frame006Pieces[3];
extern const u8 gSpriteBank22Frame007Pieces[2];
extern const u8 gSpriteBank22Frame008Pieces[2];
extern const u8 gSpriteBank22Frame009Pieces[2];
extern const u8 gSpriteBank22Frame010Pieces[2];
extern const u8 gSpriteBank22Frame011Pieces[2];
extern const u8 gSpriteBank22Frame012Pieces[2];
extern const u8 gSpriteBank22Frame013Pieces[2];
extern const u8 gSpriteBank22Frame014Pieces[2];
extern const u8 gSpriteBank22Frame015Pieces[2];
extern const u8 gSpriteBank22Frame016Pieces[2];
extern const u8 gSpriteBank22Frame017Pieces[3];
extern const u8 gSpriteBank22Frame018Pieces[3];
extern const u8 gSpriteBank22Frame019Pieces[3];
extern const u8 gSpriteBank22Frame020Pieces[3];
extern const u8 gSpriteBank22Frame021Pieces[2];
extern const u8 gSpriteBank22Frame022Pieces[2];
extern const u8 gSpriteBank22Frame023Pieces[2];
extern const u8 gSpriteBank22Frame024Pieces[2];
extern const u8 gSpriteBank22Frame025Pieces[2];
extern const u8 gSpriteBank22Frame026Pieces[3];
extern const u8 gSpriteBank22Frame027Pieces[2];
extern const u8 gSpriteBank22Frame028Pieces[2];
extern const u8 gSpriteBank22Frame029Pieces[3];
extern const u8 gSpriteBank22Frame030Pieces[2];

const struct sprite_anim gSpriteBank22Anims[2] = {
    [0] = {
        .seq = gSpriteBank22Anim00Seq,
        .box = { { -15, -21, 31, 42 }, { -22, -25, 38, 46 } },
        .paletteId = 30,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank22Anim00Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [1] = {
        .seq = gSpriteBank22Anim01Seq,
        .box = { { -15, -21, 31, 42 }, { -23, -22, 41, 44 } },
        .paletteId = 30,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank22Anim01Seq),
        .flags = 0,
    },
};

const u16 gSpriteBank22Anim00Seq[16] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
};
const u16 gSpriteBank22Anim01Seq[15] = {
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};

const struct sprite_frame *const gSpriteBank22Frames[31] = {
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
    SPRITE_FRAME(gSpriteBank22Frame000, SPRITE_TILES_BANK22 + 0x00000),
    { { -12, -9, 25, 28 } },
};
const struct sprite_frame_1box gSpriteBank22Frame001 = {
    SPRITE_FRAME(gSpriteBank22Frame001, SPRITE_TILES_BANK22 + 0x00300),
    { { -12, -9, 25, 28 } },
};
const struct sprite_frame_1box gSpriteBank22Frame002 = {
    SPRITE_FRAME(gSpriteBank22Frame002, SPRITE_TILES_BANK22 + 0x00600),
    { { -16, -8, 29, 27 } },
};
const struct sprite_frame_1box gSpriteBank22Frame003 = {
    SPRITE_FRAME(gSpriteBank22Frame003, SPRITE_TILES_BANK22 + 0x00920),
    { { -19, -8, 32, 27 } },
};
const struct sprite_frame_1box gSpriteBank22Frame004 = {
    SPRITE_FRAME(gSpriteBank22Frame004, SPRITE_TILES_BANK22 + 0x00c80),
    { { -17, -11, 30, 30 } },
};
const struct sprite_frame_1box gSpriteBank22Frame005 = {
    SPRITE_FRAME(gSpriteBank22Frame005, SPRITE_TILES_BANK22 + 0x00fc0),
    { { -14, -13, 27, 32 } },
};
const struct sprite_frame_1box gSpriteBank22Frame006 = {
    SPRITE_FRAME(gSpriteBank22Frame006, SPRITE_TILES_BANK22 + 0x012c0),
    { { -14, -8, 27, 27 } },
};
const struct sprite_frame_1box gSpriteBank22Frame007 = {
    SPRITE_FRAME(gSpriteBank22Frame007, SPRITE_TILES_BANK22 + 0x01640),
    { { -12, -8, 25, 27 } },
};
const struct sprite_frame_1box gSpriteBank22Frame008 = {
    SPRITE_FRAME(gSpriteBank22Frame008, SPRITE_TILES_BANK22 + 0x01940),
    { { -12, -9, 25, 28 } },
};
const struct sprite_frame_1box gSpriteBank22Frame009 = {
    SPRITE_FRAME(gSpriteBank22Frame009, SPRITE_TILES_BANK22 + 0x01c40),
    { { -11, -7, 24, 26 } },
};
const struct sprite_frame_1box gSpriteBank22Frame010 = {
    SPRITE_FRAME(gSpriteBank22Frame010, SPRITE_TILES_BANK22 + 0x01f40),
    { { -12, -5, 25, 24 } },
};
const struct sprite_frame_1box gSpriteBank22Frame011 = {
    SPRITE_FRAME(gSpriteBank22Frame011, SPRITE_TILES_BANK22 + 0x021c0),
    { { -11, -4, 24, 23 } },
};
const struct sprite_frame_1box gSpriteBank22Frame012 = {
    SPRITE_FRAME(gSpriteBank22Frame012, SPRITE_TILES_BANK22 + 0x02440),
    { { -11, -6, 24, 25 } },
};
const struct sprite_frame_1box gSpriteBank22Frame013 = {
    SPRITE_FRAME(gSpriteBank22Frame013, SPRITE_TILES_BANK22 + 0x026c0),
    { { -11, -7, 24, 26 } },
};
const struct sprite_frame_1box gSpriteBank22Frame014 = {
    SPRITE_FRAME(gSpriteBank22Frame014, SPRITE_TILES_BANK22 + 0x029c0),
    { { -12, -6, 25, 25 } },
};
const struct sprite_frame_1box gSpriteBank22Frame015 = {
    SPRITE_FRAME(gSpriteBank22Frame015, SPRITE_TILES_BANK22 + 0x02c40),
    { { -12, -7, 25, 26 } },
};
const struct sprite_frame gSpriteBank22Frame016 = SPRITE_FRAME(gSpriteBank22Frame016, SPRITE_TILES_BANK22 + 0x02f40);
const struct sprite_frame gSpriteBank22Frame017 = SPRITE_FRAME(gSpriteBank22Frame017, SPRITE_TILES_BANK22 + 0x03240);
const struct sprite_frame gSpriteBank22Frame018 = SPRITE_FRAME(gSpriteBank22Frame018, SPRITE_TILES_BANK22 + 0x03560);
const struct sprite_frame gSpriteBank22Frame019 = SPRITE_FRAME(gSpriteBank22Frame019, SPRITE_TILES_BANK22 + 0x038a0);
const struct sprite_frame gSpriteBank22Frame020 = SPRITE_FRAME(gSpriteBank22Frame020, SPRITE_TILES_BANK22 + 0x03be0);
const struct sprite_frame gSpriteBank22Frame021 = SPRITE_FRAME(gSpriteBank22Frame021, SPRITE_TILES_BANK22 + 0x03f00);
const struct sprite_frame gSpriteBank22Frame022 = SPRITE_FRAME(gSpriteBank22Frame022, SPRITE_TILES_BANK22 + 0x04180);
const struct sprite_frame gSpriteBank22Frame023 = SPRITE_FRAME(gSpriteBank22Frame023, SPRITE_TILES_BANK22 + 0x04400);
const struct sprite_frame gSpriteBank22Frame024 = SPRITE_FRAME(gSpriteBank22Frame024, SPRITE_TILES_BANK22 + 0x04700);
const struct sprite_frame gSpriteBank22Frame025 = SPRITE_FRAME(gSpriteBank22Frame025, SPRITE_TILES_BANK22 + 0x04a00);
const struct sprite_frame gSpriteBank22Frame026 = SPRITE_FRAME(gSpriteBank22Frame026, SPRITE_TILES_BANK22 + 0x04c80);
const struct sprite_frame gSpriteBank22Frame027 = SPRITE_FRAME(gSpriteBank22Frame027, SPRITE_TILES_BANK22 + 0x04ee0);
const struct sprite_frame gSpriteBank22Frame028 = SPRITE_FRAME(gSpriteBank22Frame028, SPRITE_TILES_BANK22 + 0x051e0);
const struct sprite_frame gSpriteBank22Frame029 = SPRITE_FRAME(gSpriteBank22Frame029, SPRITE_TILES_BANK22 + 0x054e0);
const struct sprite_frame gSpriteBank22Frame030 = SPRITE_FRAME(gSpriteBank22Frame030, SPRITE_TILES_BANK22 + 0x05800);

const struct sprite_piece_pos gSpriteBank22Frame000Pos[2] = { { -13, -21 }, { -15, 11 } };
const struct sprite_piece_pos gSpriteBank22Frame001Pos[2] = { { -14, -21 }, { -15, 11 } };
const struct sprite_piece_pos gSpriteBank22Frame002Pos[3] = { { -19, -20 }, { 13, 5 }, { -15, 12 } };
const struct sprite_piece_pos gSpriteBank22Frame003Pos[4] = { { -22, -20 }, { 10, -5 }, { 10, 11 }, { -15, 12 } };
const struct sprite_piece_pos gSpriteBank22Frame004Pos[3] = { { -20, -23 }, { 12, -4 }, { -15, 9 } };
const struct sprite_piece_pos gSpriteBank22Frame005Pos[2] = { { -17, -25 }, { -15, 7 } };
const struct sprite_piece_pos gSpriteBank22Frame006Pos[3] = { { -17, -20 }, { 15, -20 }, { -15, 12 } };
const struct sprite_piece_pos gSpriteBank22Frame007Pos[2] = { { -15, -20 }, { -15, 12 } };
const struct sprite_piece_pos gSpriteBank22Frame008Pos[2] = { { -12, -21 }, { -15, 11 } };
const struct sprite_piece_pos gSpriteBank22Frame009Pos[2] = { { -13, -19 }, { -14, 13 } };
const struct sprite_piece_pos gSpriteBank22Frame010Pos[2] = { { -14, -17 }, { -15, 15 } };
const struct sprite_piece_pos gSpriteBank22Frame011Pos[2] = { { -14, -16 }, { -13, 16 } };
const struct sprite_piece_pos gSpriteBank22Frame012Pos[2] = { { -13, -18 }, { -14, 14 } };
const struct sprite_piece_pos gSpriteBank22Frame013Pos[2] = { { -13, -19 }, { -14, 13 } };
const struct sprite_piece_pos gSpriteBank22Frame014Pos[2] = { { -13, -18 }, { -15, 14 } };
const struct sprite_piece_pos gSpriteBank22Frame015Pos[2] = { { -13, -19 }, { -15, 13 } };
const struct sprite_piece_pos gSpriteBank22Frame016Pos[2] = { { -14, -21 }, { -15, 11 } };
const struct sprite_piece_pos gSpriteBank22Frame017Pos[3] = { { -20, -20 }, { 12, 4 }, { -15, 12 } };
const struct sprite_piece_pos gSpriteBank22Frame018Pos[3] = { { -23, -19 }, { 9, 0 }, { -14, 13 } };
const struct sprite_piece_pos gSpriteBank22Frame019Pos[3] = { { -22, -20 }, { 10, 2 }, { -14, 12 } };
const struct sprite_piece_pos gSpriteBank22Frame020Pos[3] = { { -20, -18 }, { 12, 7 }, { -13, 14 } };
const struct sprite_piece_pos gSpriteBank22Frame021Pos[2] = { { -16, -17 }, { -13, 15 } };
const struct sprite_piece_pos gSpriteBank22Frame022Pos[2] = { { -15, -18 }, { -14, 14 } };
const struct sprite_piece_pos gSpriteBank22Frame023Pos[2] = { { -14, -20 }, { -14, 12 } };
const struct sprite_piece_pos gSpriteBank22Frame024Pos[2] = { { -13, -20 }, { -13, 12 } };
const struct sprite_piece_pos gSpriteBank22Frame025Pos[2] = { { -13, -18 }, { -14, 14 } };
const struct sprite_piece_pos gSpriteBank22Frame026Pos[3] = { { -13, -18 }, { -14, 14 }, { 2, 14 } };
const struct sprite_piece_pos gSpriteBank22Frame027Pos[2] = { { -12, -21 }, { -13, 11 } };
const struct sprite_piece_pos gSpriteBank22Frame028Pos[2] = { { -12, -22 }, { -14, 10 } };
const struct sprite_piece_pos gSpriteBank22Frame029Pos[3] = { { -13, -19 }, { 17, -11 }, { -15, 13 } };
const struct sprite_piece_pos gSpriteBank22Frame030Pos[2] = { { -14, -18 }, { -15, 14 } };

const u8 gSpriteBank22Frame000Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank22Frame001Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank22Frame002Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank22Frame003Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank22Frame004Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank22Frame005Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank22Frame006Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank22Frame007Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank22Frame008Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank22Frame009Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank22Frame010Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank22Frame011Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank22Frame012Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank22Frame013Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank22Frame014Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank22Frame015Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank22Frame016Pieces[2] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank22Frame017Pieces[3] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank22Frame018Pieces[3] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank22Frame019Pieces[3] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank22Frame020Pieces[3] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank22Frame021Pieces[2] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank22Frame022Pieces[2] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank22Frame023Pieces[2] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank22Frame024Pieces[2] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank22Frame025Pieces[2] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank22Frame026Pieces[3] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank22Frame027Pieces[2] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank22Frame028Pieces[2] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank22Frame029Pieces[3] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank22Frame030Pieces[2] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 5) };

/* ---------------------------------------------------------------------- */
/* Bank 23: 5 animations, 59 frames, tiles in gSpriteBank23Tiles (SPRITE_TILES_BANK23). */

extern const u16 gSpriteBank23Anim00Seq[6];
extern const u16 gSpriteBank23Anim01Seq[14];
extern const u16 gSpriteBank23Anim02Seq[15];
extern const u16 gSpriteBank23Anim03Seq[12];
extern const u16 gSpriteBank23Anim04Seq[12];
extern const struct sprite_frame_1box gSpriteBank23Frame000;
extern const struct sprite_frame_1box gSpriteBank23Frame001;
extern const struct sprite_frame_1box gSpriteBank23Frame002;
extern const struct sprite_frame_1box gSpriteBank23Frame003;
extern const struct sprite_frame_1box gSpriteBank23Frame004;
extern const struct sprite_frame_1box gSpriteBank23Frame005;
extern const struct sprite_frame_1box gSpriteBank23Frame006;
extern const struct sprite_frame_1box gSpriteBank23Frame007;
extern const struct sprite_frame_1box gSpriteBank23Frame008;
extern const struct sprite_frame_1box gSpriteBank23Frame009;
extern const struct sprite_frame_1box gSpriteBank23Frame010;
extern const struct sprite_frame_1box gSpriteBank23Frame011;
extern const struct sprite_frame_1box gSpriteBank23Frame012;
extern const struct sprite_frame_1box gSpriteBank23Frame013;
extern const struct sprite_frame_1box gSpriteBank23Frame014;
extern const struct sprite_frame_1box gSpriteBank23Frame015;
extern const struct sprite_frame_1box gSpriteBank23Frame016;
extern const struct sprite_frame_1box gSpriteBank23Frame017;
extern const struct sprite_frame_1box gSpriteBank23Frame018;
extern const struct sprite_frame_1box gSpriteBank23Frame019;
extern const struct sprite_frame_1box gSpriteBank23Frame020;
extern const struct sprite_frame_1box gSpriteBank23Frame021;
extern const struct sprite_frame_1box gSpriteBank23Frame022;
extern const struct sprite_frame_1box gSpriteBank23Frame023;
extern const struct sprite_frame_1box gSpriteBank23Frame024;
extern const struct sprite_frame_1box gSpriteBank23Frame025;
extern const struct sprite_frame_1box gSpriteBank23Frame026;
extern const struct sprite_frame_1box gSpriteBank23Frame027;
extern const struct sprite_frame_1box gSpriteBank23Frame028;
extern const struct sprite_frame_1box gSpriteBank23Frame029;
extern const struct sprite_frame_1box gSpriteBank23Frame030;
extern const struct sprite_frame_1box gSpriteBank23Frame031;
extern const struct sprite_frame_1box gSpriteBank23Frame032;
extern const struct sprite_frame_1box gSpriteBank23Frame033;
extern const struct sprite_frame_1box gSpriteBank23Frame034;
extern const struct sprite_frame_1box gSpriteBank23Frame035;
extern const struct sprite_frame_1box gSpriteBank23Frame036;
extern const struct sprite_frame_1box gSpriteBank23Frame037;
extern const struct sprite_frame_1box gSpriteBank23Frame038;
extern const struct sprite_frame_1box gSpriteBank23Frame039;
extern const struct sprite_frame_1box gSpriteBank23Frame040;
extern const struct sprite_frame_1box gSpriteBank23Frame041;
extern const struct sprite_frame_1box gSpriteBank23Frame042;
extern const struct sprite_frame_1box gSpriteBank23Frame043;
extern const struct sprite_frame_1box gSpriteBank23Frame044;
extern const struct sprite_frame_1box gSpriteBank23Frame045;
extern const struct sprite_frame_1box gSpriteBank23Frame046;
extern const struct sprite_frame_1box gSpriteBank23Frame047;
extern const struct sprite_frame_1box gSpriteBank23Frame048;
extern const struct sprite_frame_1box gSpriteBank23Frame049;
extern const struct sprite_frame_1box gSpriteBank23Frame050;
extern const struct sprite_frame_1box gSpriteBank23Frame051;
extern const struct sprite_frame_1box gSpriteBank23Frame052;
extern const struct sprite_frame_1box gSpriteBank23Frame053;
extern const struct sprite_frame_1box gSpriteBank23Frame054;
extern const struct sprite_frame_1box gSpriteBank23Frame055;
extern const struct sprite_frame_1box gSpriteBank23Frame056;
extern const struct sprite_frame_1box gSpriteBank23Frame057;
extern const struct sprite_frame_1box gSpriteBank23Frame058;
extern const struct sprite_piece_pos gSpriteBank23Frame000Pos[3];
extern const struct sprite_piece_pos gSpriteBank23Frame001Pos[4];
extern const struct sprite_piece_pos gSpriteBank23Frame002Pos[4];
extern const struct sprite_piece_pos gSpriteBank23Frame003Pos[4];
extern const struct sprite_piece_pos gSpriteBank23Frame004Pos[4];
extern const struct sprite_piece_pos gSpriteBank23Frame005Pos[3];
extern const struct sprite_piece_pos gSpriteBank23Frame006Pos[3];
extern const struct sprite_piece_pos gSpriteBank23Frame007Pos[3];
extern const struct sprite_piece_pos gSpriteBank23Frame008Pos[3];
extern const struct sprite_piece_pos gSpriteBank23Frame009Pos[4];
extern const struct sprite_piece_pos gSpriteBank23Frame010Pos[3];
extern const struct sprite_piece_pos gSpriteBank23Frame011Pos[3];
extern const struct sprite_piece_pos gSpriteBank23Frame012Pos[3];
extern const struct sprite_piece_pos gSpriteBank23Frame013Pos[4];
extern const struct sprite_piece_pos gSpriteBank23Frame014Pos[3];
extern const struct sprite_piece_pos gSpriteBank23Frame015Pos[4];
extern const struct sprite_piece_pos gSpriteBank23Frame016Pos[4];
extern const struct sprite_piece_pos gSpriteBank23Frame017Pos[4];
extern const struct sprite_piece_pos gSpriteBank23Frame018Pos[4];
extern const struct sprite_piece_pos gSpriteBank23Frame019Pos[3];
extern const struct sprite_piece_pos gSpriteBank23Frame020Pos[3];
extern const struct sprite_piece_pos gSpriteBank23Frame021Pos[3];
extern const struct sprite_piece_pos gSpriteBank23Frame022Pos[3];
extern const struct sprite_piece_pos gSpriteBank23Frame023Pos[4];
extern const struct sprite_piece_pos gSpriteBank23Frame024Pos[4];
extern const struct sprite_piece_pos gSpriteBank23Frame025Pos[4];
extern const struct sprite_piece_pos gSpriteBank23Frame026Pos[4];
extern const struct sprite_piece_pos gSpriteBank23Frame027Pos[3];
extern const struct sprite_piece_pos gSpriteBank23Frame028Pos[3];
extern const struct sprite_piece_pos gSpriteBank23Frame029Pos[5];
extern const struct sprite_piece_pos gSpriteBank23Frame030Pos[5];
extern const struct sprite_piece_pos gSpriteBank23Frame031Pos[3];
extern const struct sprite_piece_pos gSpriteBank23Frame032Pos[4];
extern const struct sprite_piece_pos gSpriteBank23Frame033Pos[3];
extern const struct sprite_piece_pos gSpriteBank23Frame034Pos[3];
extern const struct sprite_piece_pos gSpriteBank23Frame035Pos[3];
extern const struct sprite_piece_pos gSpriteBank23Frame036Pos[4];
extern const struct sprite_piece_pos gSpriteBank23Frame037Pos[3];
extern const struct sprite_piece_pos gSpriteBank23Frame038Pos[3];
extern const struct sprite_piece_pos gSpriteBank23Frame039Pos[3];
extern const struct sprite_piece_pos gSpriteBank23Frame040Pos[3];
extern const struct sprite_piece_pos gSpriteBank23Frame041Pos[4];
extern const struct sprite_piece_pos gSpriteBank23Frame042Pos[4];
extern const struct sprite_piece_pos gSpriteBank23Frame043Pos[4];
extern const struct sprite_piece_pos gSpriteBank23Frame044Pos[4];
extern const struct sprite_piece_pos gSpriteBank23Frame045Pos[3];
extern const struct sprite_piece_pos gSpriteBank23Frame046Pos[3];
extern const struct sprite_piece_pos gSpriteBank23Frame047Pos[1];
extern const struct sprite_piece_pos gSpriteBank23Frame048Pos[1];
extern const struct sprite_piece_pos gSpriteBank23Frame049Pos[1];
extern const struct sprite_piece_pos gSpriteBank23Frame050Pos[2];
extern const struct sprite_piece_pos gSpriteBank23Frame051Pos[3];
extern const struct sprite_piece_pos gSpriteBank23Frame052Pos[5];
extern const struct sprite_piece_pos gSpriteBank23Frame053Pos[5];
extern const struct sprite_piece_pos gSpriteBank23Frame054Pos[4];
extern const struct sprite_piece_pos gSpriteBank23Frame055Pos[3];
extern const struct sprite_piece_pos gSpriteBank23Frame056Pos[1];
extern const struct sprite_piece_pos gSpriteBank23Frame057Pos[1];
extern const struct sprite_piece_pos gSpriteBank23Frame058Pos[1];
extern const u8 gSpriteBank23Frame000Pieces[3];
extern const u8 gSpriteBank23Frame001Pieces[4];
extern const u8 gSpriteBank23Frame002Pieces[4];
extern const u8 gSpriteBank23Frame003Pieces[4];
extern const u8 gSpriteBank23Frame004Pieces[4];
extern const u8 gSpriteBank23Frame005Pieces[3];
extern const u8 gSpriteBank23Frame006Pieces[3];
extern const u8 gSpriteBank23Frame007Pieces[3];
extern const u8 gSpriteBank23Frame008Pieces[3];
extern const u8 gSpriteBank23Frame009Pieces[4];
extern const u8 gSpriteBank23Frame010Pieces[3];
extern const u8 gSpriteBank23Frame011Pieces[3];
extern const u8 gSpriteBank23Frame012Pieces[3];
extern const u8 gSpriteBank23Frame013Pieces[4];
extern const u8 gSpriteBank23Frame014Pieces[3];
extern const u8 gSpriteBank23Frame015Pieces[4];
extern const u8 gSpriteBank23Frame016Pieces[4];
extern const u8 gSpriteBank23Frame017Pieces[4];
extern const u8 gSpriteBank23Frame018Pieces[4];
extern const u8 gSpriteBank23Frame019Pieces[3];
extern const u8 gSpriteBank23Frame020Pieces[3];
extern const u8 gSpriteBank23Frame021Pieces[3];
extern const u8 gSpriteBank23Frame022Pieces[3];
extern const u8 gSpriteBank23Frame023Pieces[4];
extern const u8 gSpriteBank23Frame024Pieces[4];
extern const u8 gSpriteBank23Frame025Pieces[4];
extern const u8 gSpriteBank23Frame026Pieces[4];
extern const u8 gSpriteBank23Frame027Pieces[3];
extern const u8 gSpriteBank23Frame028Pieces[3];
extern const u8 gSpriteBank23Frame029Pieces[5];
extern const u8 gSpriteBank23Frame030Pieces[5];
extern const u8 gSpriteBank23Frame031Pieces[3];
extern const u8 gSpriteBank23Frame032Pieces[4];
extern const u8 gSpriteBank23Frame033Pieces[3];
extern const u8 gSpriteBank23Frame034Pieces[3];
extern const u8 gSpriteBank23Frame035Pieces[3];
extern const u8 gSpriteBank23Frame036Pieces[4];
extern const u8 gSpriteBank23Frame037Pieces[3];
extern const u8 gSpriteBank23Frame038Pieces[3];
extern const u8 gSpriteBank23Frame039Pieces[3];
extern const u8 gSpriteBank23Frame040Pieces[3];
extern const u8 gSpriteBank23Frame041Pieces[4];
extern const u8 gSpriteBank23Frame042Pieces[4];
extern const u8 gSpriteBank23Frame043Pieces[4];
extern const u8 gSpriteBank23Frame044Pieces[4];
extern const u8 gSpriteBank23Frame045Pieces[3];
extern const u8 gSpriteBank23Frame046Pieces[3];
extern const u8 gSpriteBank23Frame047Pieces[1];
extern const u8 gSpriteBank23Frame048Pieces[1];
extern const u8 gSpriteBank23Frame049Pieces[1];
extern const u8 gSpriteBank23Frame050Pieces[2];
extern const u8 gSpriteBank23Frame051Pieces[3];
extern const u8 gSpriteBank23Frame052Pieces[5];
extern const u8 gSpriteBank23Frame053Pieces[5];
extern const u8 gSpriteBank23Frame054Pieces[4];
extern const u8 gSpriteBank23Frame055Pieces[3];
extern const u8 gSpriteBank23Frame056Pieces[1];
extern const u8 gSpriteBank23Frame057Pieces[1];
extern const u8 gSpriteBank23Frame058Pieces[1];

const struct sprite_anim gSpriteBank23Anims[5] = {
    [0] = {
        .seq = gSpriteBank23Anim00Seq,
        .box = { { -21, -37, 42, 74 }, { -21, -37, 42, 75 } },
        .paletteId = 32,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank23Anim00Seq),
        .flags = 0,
    },
    [1] = {
        .seq = gSpriteBank23Anim01Seq,
        .box = { { -21, -37, 42, 74 }, { -21, -37, 42, 76 } },
        .paletteId = 32,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank23Anim01Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [2] = {
        .seq = gSpriteBank23Anim02Seq,
        .box = { { -21, -37, 42, 74 }, { -22, -38, 48, 76 } },
        .paletteId = 32,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank23Anim02Seq),
        .flags = 0,
    },
    [3] = {
        .seq = gSpriteBank23Anim03Seq,
        .box = { { -21, -37, 42, 74 }, { -20, -37, 41, 76 } },
        .paletteId = 32,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank23Anim03Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [4] = {
        .seq = gSpriteBank23Anim04Seq,
        .box = { { -6, -2, 12, 4 }, { -53, -12, 59, 21 } },
        .paletteId = 33,
        .duration = 2,
        .frameCount = ARRAY_COUNT(gSpriteBank23Anim04Seq),
        .flags = 0,
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

const struct sprite_frame *const gSpriteBank23Frames[59] = {
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
    SPRITE_FRAME(gSpriteBank23Frame000, SPRITE_TILES_BANK23 + 0x00000),
    { { -13, -25, 27, 54 } },
};
const struct sprite_frame_1box gSpriteBank23Frame001 = {
    SPRITE_FRAME(gSpriteBank23Frame001, SPRITE_TILES_BANK23 + 0x00880),
    { { -13, -25, 27, 54 } },
};
const struct sprite_frame_1box gSpriteBank23Frame002 = {
    SPRITE_FRAME(gSpriteBank23Frame002, SPRITE_TILES_BANK23 + 0x01120),
    { { -13, -25, 27, 55 } },
};
const struct sprite_frame_1box gSpriteBank23Frame003 = {
    SPRITE_FRAME(gSpriteBank23Frame003, SPRITE_TILES_BANK23 + 0x019c0),
    { { -12, -25, 26, 55 } },
};
const struct sprite_frame_1box gSpriteBank23Frame004 = {
    SPRITE_FRAME(gSpriteBank23Frame004, SPRITE_TILES_BANK23 + 0x02280),
    { { -12, -25, 26, 55 } },
};
const struct sprite_frame_1box gSpriteBank23Frame005 = {
    SPRITE_FRAME(gSpriteBank23Frame005, SPRITE_TILES_BANK23 + 0x02b00),
    { { -14, -25, 28, 55 } },
};
const struct sprite_frame_1box gSpriteBank23Frame006 = {
    SPRITE_FRAME(gSpriteBank23Frame006, SPRITE_TILES_BANK23 + 0x03400),
    { { -14, -29, 28, 58 } },
};
const struct sprite_frame_1box gSpriteBank23Frame007 = {
    SPRITE_FRAME(gSpriteBank23Frame007, SPRITE_TILES_BANK23 + 0x03d00),
    { { -14, -29, 28, 58 } },
};
const struct sprite_frame_1box gSpriteBank23Frame008 = {
    SPRITE_FRAME(gSpriteBank23Frame008, SPRITE_TILES_BANK23 + 0x04600),
    { { -13, -29, 27, 58 } },
};
const struct sprite_frame_1box gSpriteBank23Frame009 = {
    SPRITE_FRAME(gSpriteBank23Frame009, SPRITE_TILES_BANK23 + 0x04f00),
    { { -10, -29, 24, 60 } },
};
const struct sprite_frame_1box gSpriteBank23Frame010 = {
    SPRITE_FRAME(gSpriteBank23Frame010, SPRITE_TILES_BANK23 + 0x057c0),
    { { -8, -29, 22, 60 } },
};
const struct sprite_frame_1box gSpriteBank23Frame011 = {
    SPRITE_FRAME(gSpriteBank23Frame011, SPRITE_TILES_BANK23 + 0x05c80),
    { { -8, -29, 22, 57 } },
};
const struct sprite_frame_1box gSpriteBank23Frame012 = {
    SPRITE_FRAME(gSpriteBank23Frame012, SPRITE_TILES_BANK23 + 0x06140),
    { { -8, -29, 22, 57 } },
};
const struct sprite_frame_1box gSpriteBank23Frame013 = {
    SPRITE_FRAME(gSpriteBank23Frame013, SPRITE_TILES_BANK23 + 0x06600),
    { { -9, -29, 23, 57 } },
};
const struct sprite_frame_1box gSpriteBank23Frame014 = {
    SPRITE_FRAME(gSpriteBank23Frame014, SPRITE_TILES_BANK23 + 0x06ae0),
    { { -8, -29, 22, 57 } },
};
const struct sprite_frame_1box gSpriteBank23Frame015 = {
    SPRITE_FRAME(gSpriteBank23Frame015, SPRITE_TILES_BANK23 + 0x06fa0),
    { { -9, -29, 23, 57 } },
};
const struct sprite_frame_1box gSpriteBank23Frame016 = {
    SPRITE_FRAME(gSpriteBank23Frame016, SPRITE_TILES_BANK23 + 0x07480),
    { { -9, -29, 23, 58 } },
};
const struct sprite_frame_1box gSpriteBank23Frame017 = {
    SPRITE_FRAME(gSpriteBank23Frame017, SPRITE_TILES_BANK23 + 0x07960),
    { { -9, -29, 23, 58 } },
};
const struct sprite_frame_1box gSpriteBank23Frame018 = {
    SPRITE_FRAME(gSpriteBank23Frame018, SPRITE_TILES_BANK23 + 0x07e40),
    { { -10, -29, 24, 59 } },
};
const struct sprite_frame_1box gSpriteBank23Frame019 = {
    SPRITE_FRAME(gSpriteBank23Frame019, SPRITE_TILES_BANK23 + 0x08700),
    { { -13, -29, 27, 59 } },
};
const struct sprite_frame_1box gSpriteBank23Frame020 = {
    SPRITE_FRAME(gSpriteBank23Frame020, SPRITE_TILES_BANK23 + 0x09000),
    { { -16, -27, 31, 54 } },
};
const struct sprite_frame_1box gSpriteBank23Frame021 = {
    SPRITE_FRAME(gSpriteBank23Frame021, SPRITE_TILES_BANK23 + 0x09900),
    { { -16, -27, 31, 54 } },
};
const struct sprite_frame_1box gSpriteBank23Frame022 = {
    SPRITE_FRAME(gSpriteBank23Frame022, SPRITE_TILES_BANK23 + 0x02b00),
    { { -15, -27, 30, 55 } },
};
const struct sprite_frame_1box gSpriteBank23Frame023 = {
    SPRITE_FRAME(gSpriteBank23Frame023, SPRITE_TILES_BANK23 + 0x02280),
    { { -13, -27, 28, 55 } },
};
const struct sprite_frame_1box gSpriteBank23Frame024 = {
    SPRITE_FRAME(gSpriteBank23Frame024, SPRITE_TILES_BANK23 + 0x019c0),
    { { -13, -27, 28, 55 } },
};
const struct sprite_frame_1box gSpriteBank23Frame025 = {
    SPRITE_FRAME(gSpriteBank23Frame025, SPRITE_TILES_BANK23 + 0x01120),
    { { -14, -27, 29, 55 } },
};
const struct sprite_frame_1box gSpriteBank23Frame026 = {
    SPRITE_FRAME(gSpriteBank23Frame026, SPRITE_TILES_BANK23 + 0x00880),
    { { -14, -27, 29, 54 } },
};
const struct sprite_frame_1box gSpriteBank23Frame027 = {
    SPRITE_FRAME(gSpriteBank23Frame027, SPRITE_TILES_BANK23 + 0x00000),
    { { -14, -27, 29, 54 } },
};
const struct sprite_frame_1box gSpriteBank23Frame028 = {
    SPRITE_FRAME(gSpriteBank23Frame028, SPRITE_TILES_BANK23 + 0x0a200),
    { { -3, -28, 22, 51 } },
};
const struct sprite_frame_1box gSpriteBank23Frame029 = {
    SPRITE_FRAME(gSpriteBank23Frame029, SPRITE_TILES_BANK23 + 0x0a660),
    { { -6, -28, 26, 45 } },
};
const struct sprite_frame_1box gSpriteBank23Frame030 = {
    SPRITE_FRAME(gSpriteBank23Frame030, SPRITE_TILES_BANK23 + 0x0ab20),
    { { -7, -28, 26, 47 } },
};
const struct sprite_frame_1box gSpriteBank23Frame031 = {
    SPRITE_FRAME(gSpriteBank23Frame031, SPRITE_TILES_BANK23 + 0x0afe0),
    { { -6, -27, 24, 51 } },
};
const struct sprite_frame_1box gSpriteBank23Frame032 = {
    SPRITE_FRAME(gSpriteBank23Frame032, SPRITE_TILES_BANK23 + 0x0b460),
    { { -9, -27, 25, 55 } },
};
const struct sprite_frame_1box gSpriteBank23Frame033 = {
    SPRITE_FRAME(gSpriteBank23Frame033, SPRITE_TILES_BANK23 + 0x0b940),
    { { -12, -27, 27, 54 } },
};
const struct sprite_frame_1box gSpriteBank23Frame034 = {
    SPRITE_FRAME(gSpriteBank23Frame034, SPRITE_TILES_BANK23 + 0x0c1c0),
    { { -13, -27, 28, 54 } },
};
const struct sprite_frame_1box gSpriteBank23Frame035 = {
    SPRITE_FRAME(gSpriteBank23Frame035, SPRITE_TILES_BANK23 + 0x0ca40),
    { { -11, -30, 25, 61 } },
};
const struct sprite_frame_1box gSpriteBank23Frame036 = {
    SPRITE_FRAME(gSpriteBank23Frame036, SPRITE_TILES_BANK23 + 0x0d2c0),
    { { -11, -30, 25, 62 } },
};
const struct sprite_frame_1box gSpriteBank23Frame037 = {
    SPRITE_FRAME(gSpriteBank23Frame037, SPRITE_TILES_BANK23 + 0x0db40),
    { { -12, -30, 25, 61 } },
};
const struct sprite_frame_1box gSpriteBank23Frame038 = {
    SPRITE_FRAME(gSpriteBank23Frame038, SPRITE_TILES_BANK23 + 0x0e3c0),
    { { -12, -30, 25, 59 } },
};
const struct sprite_frame_1box gSpriteBank23Frame039 = {
    SPRITE_FRAME(gSpriteBank23Frame039, SPRITE_TILES_BANK23 + 0x0ec40),
    { { -13, -30, 26, 58 } },
};
const struct sprite_frame_1box gSpriteBank23Frame040 = {
    SPRITE_FRAME(gSpriteBank23Frame040, SPRITE_TILES_BANK23 + 0x0f4c0),
    { { -13, -30, 26, 56 } },
};
const struct sprite_frame_1box gSpriteBank23Frame041 = {
    SPRITE_FRAME(gSpriteBank23Frame041, SPRITE_TILES_BANK23 + 0x0fd00),
    { { -13, -30, 26, 57 } },
};
const struct sprite_frame_1box gSpriteBank23Frame042 = {
    SPRITE_FRAME(gSpriteBank23Frame042, SPRITE_TILES_BANK23 + 0x10560),
    { { -13, -30, 26, 58 } },
};
const struct sprite_frame_1box gSpriteBank23Frame043 = {
    SPRITE_FRAME(gSpriteBank23Frame043, SPRITE_TILES_BANK23 + 0x10e00),
    { { -11, -30, 24, 60 } },
};
const struct sprite_frame_1box gSpriteBank23Frame044 = {
    SPRITE_FRAME(gSpriteBank23Frame044, SPRITE_TILES_BANK23 + 0x116a0),
    { { -11, -30, 24, 61 } },
};
const struct sprite_frame_1box gSpriteBank23Frame045 = {
    SPRITE_FRAME(gSpriteBank23Frame045, SPRITE_TILES_BANK23 + 0x11f40),
    { { -11, -30, 25, 61 } },
};
const struct sprite_frame_1box gSpriteBank23Frame046 = {
    SPRITE_FRAME(gSpriteBank23Frame046, SPRITE_TILES_BANK23 + 0x127c0),
    { { -11, -30, 25, 60 } },
};
const struct sprite_frame_1box gSpriteBank23Frame047 = {
    SPRITE_FRAME(gSpriteBank23Frame047, SPRITE_TILES_BANK23 + 0x13040),
    { { -6, -2, 12, 4 } },
};
const struct sprite_frame_1box gSpriteBank23Frame048 = {
    SPRITE_FRAME(gSpriteBank23Frame048, SPRITE_TILES_BANK23 + 0x13080),
    { { -9, -3, 15, 6 } },
};
const struct sprite_frame_1box gSpriteBank23Frame049 = {
    SPRITE_FRAME(gSpriteBank23Frame049, SPRITE_TILES_BANK23 + 0x130c0),
    { { -23, -8, 29, 11 } },
};
const struct sprite_frame_1box gSpriteBank23Frame050 = {
    SPRITE_FRAME(gSpriteBank23Frame050, SPRITE_TILES_BANK23 + 0x131c0),
    { { -30, -8, 36, 13 } },
};
const struct sprite_frame_1box gSpriteBank23Frame051 = {
    SPRITE_FRAME(gSpriteBank23Frame051, SPRITE_TILES_BANK23 + 0x132e0),
    { { -34, -8, 40, 15 } },
};
const struct sprite_frame_1box gSpriteBank23Frame052 = {
    SPRITE_FRAME(gSpriteBank23Frame052, SPRITE_TILES_BANK23 + 0x13460),
    { { -36, -12, 42, 21 } },
};
const struct sprite_frame_1box gSpriteBank23Frame053 = {
    SPRITE_FRAME(gSpriteBank23Frame053, SPRITE_TILES_BANK23 + 0x13640),
    { { -43, -12, 48, 21 } },
};
const struct sprite_frame_1box gSpriteBank23Frame054 = {
    SPRITE_FRAME(gSpriteBank23Frame054, SPRITE_TILES_BANK23 + 0x138a0),
    { { -40, -12, 45, 18 } },
};
const struct sprite_frame_1box gSpriteBank23Frame055 = {
    SPRITE_FRAME(gSpriteBank23Frame055, SPRITE_TILES_BANK23 + 0x13a80),
    { { -46, -11, 43, 17 } },
};
const struct sprite_frame_1box gSpriteBank23Frame056 = {
    SPRITE_FRAME(gSpriteBank23Frame056, SPRITE_TILES_BANK23 + 0x13c40),
    { { -48, -5, 29, 9 } },
};
const struct sprite_frame_1box gSpriteBank23Frame057 = {
    SPRITE_FRAME(gSpriteBank23Frame057, SPRITE_TILES_BANK23 + 0x13d40),
    { { -51, -4, 15, 7 } },
};
const struct sprite_frame_1box gSpriteBank23Frame058 = {
    SPRITE_FRAME(gSpriteBank23Frame058, SPRITE_TILES_BANK23 + 0x13d80),
    { { -53, -3, 11, 4 } },
};

const struct sprite_piece_pos gSpriteBank23Frame000Pos[3] = { { -20, -37 }, { 12, -37 }, { -14, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame001Pos[4] = { { -20, -37 }, { 12, -37 }, { -14, 27 }, { 2, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame002Pos[4] = { { -20, -37 }, { 12, -37 }, { -16, 27 }, { 0, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame003Pos[4] = { { -19, -37 }, { 13, -37 }, { -17, 27 }, { -1, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame004Pos[4] = { { -17, -37 }, { 13, -37 }, { -19, 27 }, { -3, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame005Pos[3] = { { -17, -37 }, { 11, -37 }, { -21, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame006Pos[3] = { { -16, -37 }, { 11, -37 }, { -21, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame007Pos[3] = { { -19, -37 }, { 11, -37 }, { -21, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame008Pos[3] = { { -16, -37 }, { 12, -37 }, { -20, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame009Pos[4] = { { -15, -37 }, { 15, -37 }, { -17, 27 }, { -1, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame010Pos[3] = { { -15, -37 }, { 17, 6 }, { -14, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame011Pos[3] = { { -15, -37 }, { 17, 5 }, { -12, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame012Pos[3] = { { -15, -37 }, { 17, 5 }, { -15, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame013Pos[4] = { { -15, -37 }, { 16, -2 }, { 16, 14 }, { -16, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame014Pos[3] = { { -15, -37 }, { 17, 5 }, { -15, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame015Pos[4] = { { -16, -37 }, { 16, -1 }, { 16, 15 }, { -14, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame016Pos[4] = { { -16, -37 }, { 16, -1 }, { 16, 15 }, { -12, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame017Pos[4] = { { -16, -37 }, { 16, 0 }, { 16, 16 }, { -12, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame018Pos[4] = { { -16, -37 }, { 15, -37 }, { -17, 27 }, { -1, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame019Pos[3] = { { -16, -37 }, { 12, -37 }, { -20, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame020Pos[3] = { { -19, -37 }, { 10, -37 }, { -22, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame021Pos[3] = { { -18, -37 }, { 10, -37 }, { -22, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame022Pos[3] = { { -17, -37 }, { 11, -37 }, { -21, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame023Pos[4] = { { -17, -37 }, { 13, -37 }, { -19, 27 }, { -3, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame024Pos[4] = { { -19, -37 }, { 13, -37 }, { -17, 27 }, { -1, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame025Pos[4] = { { -20, -37 }, { 12, -37 }, { -16, 27 }, { 0, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame026Pos[4] = { { -20, -37 }, { 12, -37 }, { -14, 27 }, { 2, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame027Pos[3] = { { -20, -37 }, { 12, -37 }, { -14, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame028Pos[3] = { { -6, -38 }, { 23, 11 }, { -9, 26 } };
const struct sprite_piece_pos gSpriteBank23Frame029Pos[5] = { { -10, -38 }, { 20, -2 }, { 20, 14 }, { -12, 26 }, { 4, 26 } };
const struct sprite_piece_pos gSpriteBank23Frame030Pos[5] = { { -8, -38 }, { 19, -3 }, { 19, 13 }, { -12, 26 }, { 4, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame031Pos[3] = { { -9, -37 }, { 20, 7 }, { -11, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame032Pos[4] = { { -15, -37 }, { 17, 0 }, { 17, 16 }, { -9, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame033Pos[3] = { { -18, -37 }, { 14, -37 }, { -7, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame034Pos[3] = { { -19, -37 }, { 13, -37 }, { -7, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame035Pos[3] = { { -18, -37 }, { 14, -37 }, { -7, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame036Pos[4] = { { -18, -37 }, { 14, -37 }, { -9, 27 }, { -1, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame037Pos[3] = { { -19, -37 }, { 13, -37 }, { -11, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame038Pos[3] = { { -19, -37 }, { 13, -37 }, { -14, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame039Pos[3] = { { -20, -37 }, { 12, -37 }, { -16, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame040Pos[3] = { { -20, -37 }, { 12, -37 }, { -18, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame041Pos[4] = { { -19, -37 }, { 12, -37 }, { -20, 27 }, { -4, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame042Pos[4] = { { -18, -37 }, { 12, -37 }, { -20, 27 }, { -4, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame043Pos[4] = { { -17, -37 }, { 14, -37 }, { -18, 27 }, { -2, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame044Pos[4] = { { -18, -37 }, { 14, -37 }, { -15, 27 }, { 1, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame045Pos[3] = { { -18, -37 }, { 14, -37 }, { -10, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame046Pos[3] = { { -18, -37 }, { 14, -37 }, { -7, 27 } };
const struct sprite_piece_pos gSpriteBank23Frame047Pos[1] = { { -6, -2 } };
const struct sprite_piece_pos gSpriteBank23Frame048Pos[1] = { { -9, -3 } };
const struct sprite_piece_pos gSpriteBank23Frame049Pos[1] = { { -23, -8 } };
const struct sprite_piece_pos gSpriteBank23Frame050Pos[2] = { { -30, -8 }, { 2, -1 } };
const struct sprite_piece_pos gSpriteBank23Frame051Pos[3] = { { -34, -8 }, { -2, -3 }, { 6, -8 } };
const struct sprite_piece_pos gSpriteBank23Frame052Pos[5] = { { -36, -12 }, { -4, -4 }, { 4, 0 }, { -33, 4 }, { -1, 4 } };
const struct sprite_piece_pos gSpriteBank23Frame053Pos[5] = { { -43, -12 }, { -11, -8 }, { 5, -12 }, { -33, 4 }, { -1, 4 } };
const struct sprite_piece_pos gSpriteBank23Frame054Pos[4] = { { -40, -12 }, { -8, -7 }, { -33, 4 }, { -17, 4 } };
const struct sprite_piece_pos gSpriteBank23Frame055Pos[3] = { { -46, -11 }, { -14, -5 }, { -36, 5 } };
const struct sprite_piece_pos gSpriteBank23Frame056Pos[1] = { { -48, -5 } };
const struct sprite_piece_pos gSpriteBank23Frame057Pos[1] = { { -51, -4 } };
const struct sprite_piece_pos gSpriteBank23Frame058Pos[1] = { { -53, -3 } };

const u8 gSpriteBank23Frame000Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank23Frame001Pieces[4] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank23Frame002Pieces[4] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank23Frame003Pieces[4] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank23Frame004Pieces[4] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank23Frame005Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank23Frame006Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank23Frame007Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank23Frame008Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank23Frame009Pieces[4] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank23Frame010Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank23Frame011Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank23Frame012Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank23Frame013Pieces[4] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank23Frame014Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank23Frame015Pieces[4] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank23Frame016Pieces[4] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank23Frame017Pieces[4] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank23Frame018Pieces[4] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank23Frame019Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank23Frame020Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank23Frame021Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank23Frame022Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank23Frame023Pieces[4] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank23Frame024Pieces[4] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank23Frame025Pieces[4] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank23Frame026Pieces[4] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank23Frame027Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank23Frame028Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank23Frame029Pieces[5] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank23Frame030Pieces[5] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank23Frame031Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank23Frame032Pieces[4] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank23Frame033Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank23Frame034Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank23Frame035Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank23Frame036Pieces[4] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank23Frame037Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank23Frame038Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank23Frame039Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank23Frame040Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank23Frame041Pieces[4] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank23Frame042Pieces[4] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank23Frame043Pieces[4] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank23Frame044Pieces[4] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank23Frame045Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank23Frame046Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank23Frame047Pieces[1] = { SPRITE_PIECE(5, 4) };
const u8 gSpriteBank23Frame048Pieces[1] = { SPRITE_PIECE(5, 4) };
const u8 gSpriteBank23Frame049Pieces[1] = { SPRITE_PIECE(5, 6) };
const u8 gSpriteBank23Frame050Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank23Frame051Pieces[3] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank23Frame052Pieces[5] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank23Frame053Pieces[5] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank23Frame054Pieces[4] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank23Frame055Pieces[3] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank23Frame056Pieces[1] = { SPRITE_PIECE(5, 6) };
const u8 gSpriteBank23Frame057Pieces[1] = { SPRITE_PIECE(5, 4) };
const u8 gSpriteBank23Frame058Pieces[1] = { SPRITE_PIECE(5, 4) };

/* ---------------------------------------------------------------------- */
/* Bank 24: 6 animations, 74 frames, tiles in gSpriteBank24Tiles (SPRITE_TILES_BANK24). */

extern const u16 gSpriteBank24Anim00Seq[13];
extern const u16 gSpriteBank24Anim01Seq[11];
extern const u16 gSpriteBank24Anim02Seq[13];
extern const u16 gSpriteBank24Anim03Seq[13];
extern const u16 gSpriteBank24Anim04Seq[11];
extern const u16 gSpriteBank24Anim05Seq[13];
extern const struct sprite_frame_2box gSpriteBank24Frame000;
extern const struct sprite_frame_2box gSpriteBank24Frame001;
extern const struct sprite_frame_2box gSpriteBank24Frame002;
extern const struct sprite_frame_2box gSpriteBank24Frame003;
extern const struct sprite_frame_2box gSpriteBank24Frame004;
extern const struct sprite_frame_2box gSpriteBank24Frame005;
extern const struct sprite_frame_2box gSpriteBank24Frame006;
extern const struct sprite_frame_2box gSpriteBank24Frame007;
extern const struct sprite_frame_2box gSpriteBank24Frame008;
extern const struct sprite_frame_2box gSpriteBank24Frame009;
extern const struct sprite_frame_2box gSpriteBank24Frame010;
extern const struct sprite_frame_2box gSpriteBank24Frame011;
extern const struct sprite_frame_2box gSpriteBank24Frame012;
extern const struct sprite_frame_2box gSpriteBank24Frame013;
extern const struct sprite_frame_2box gSpriteBank24Frame014;
extern const struct sprite_frame_2box gSpriteBank24Frame015;
extern const struct sprite_frame_2box gSpriteBank24Frame016;
extern const struct sprite_frame_2box gSpriteBank24Frame017;
extern const struct sprite_frame_2box gSpriteBank24Frame018;
extern const struct sprite_frame_2box gSpriteBank24Frame019;
extern const struct sprite_frame_2box gSpriteBank24Frame020;
extern const struct sprite_frame_2box gSpriteBank24Frame021;
extern const struct sprite_frame_2box gSpriteBank24Frame022;
extern const struct sprite_frame_2box gSpriteBank24Frame023;
extern const struct sprite_frame_2box gSpriteBank24Frame024;
extern const struct sprite_frame_2box gSpriteBank24Frame025;
extern const struct sprite_frame_2box gSpriteBank24Frame026;
extern const struct sprite_frame_2box gSpriteBank24Frame027;
extern const struct sprite_frame_2box gSpriteBank24Frame028;
extern const struct sprite_frame_2box gSpriteBank24Frame029;
extern const struct sprite_frame_2box gSpriteBank24Frame030;
extern const struct sprite_frame_2box gSpriteBank24Frame031;
extern const struct sprite_frame_2box gSpriteBank24Frame032;
extern const struct sprite_frame_2box gSpriteBank24Frame033;
extern const struct sprite_frame_2box gSpriteBank24Frame034;
extern const struct sprite_frame_2box gSpriteBank24Frame035;
extern const struct sprite_frame_2box gSpriteBank24Frame036;
extern const struct sprite_frame_2box gSpriteBank24Frame037;
extern const struct sprite_frame_2box gSpriteBank24Frame038;
extern const struct sprite_frame_2box gSpriteBank24Frame039;
extern const struct sprite_frame_2box gSpriteBank24Frame040;
extern const struct sprite_frame_2box gSpriteBank24Frame041;
extern const struct sprite_frame_2box gSpriteBank24Frame042;
extern const struct sprite_frame_2box gSpriteBank24Frame043;
extern const struct sprite_frame_2box gSpriteBank24Frame044;
extern const struct sprite_frame_2box gSpriteBank24Frame045;
extern const struct sprite_frame_2box gSpriteBank24Frame046;
extern const struct sprite_frame_2box gSpriteBank24Frame047;
extern const struct sprite_frame_2box gSpriteBank24Frame048;
extern const struct sprite_frame_2box gSpriteBank24Frame049;
extern const struct sprite_frame_2box gSpriteBank24Frame050;
extern const struct sprite_frame_2box gSpriteBank24Frame051;
extern const struct sprite_frame_2box gSpriteBank24Frame052;
extern const struct sprite_frame_1box gSpriteBank24Frame053;
extern const struct sprite_frame_2box gSpriteBank24Frame054;
extern const struct sprite_frame_2box gSpriteBank24Frame055;
extern const struct sprite_frame_2box gSpriteBank24Frame056;
extern const struct sprite_frame_2box gSpriteBank24Frame057;
extern const struct sprite_frame_2box gSpriteBank24Frame058;
extern const struct sprite_frame_2box gSpriteBank24Frame059;
extern const struct sprite_frame_2box gSpriteBank24Frame060;
extern const struct sprite_frame_2box gSpriteBank24Frame061;
extern const struct sprite_frame_2box gSpriteBank24Frame062;
extern const struct sprite_frame_2box gSpriteBank24Frame063;
extern const struct sprite_frame_2box gSpriteBank24Frame064;
extern const struct sprite_frame_2box gSpriteBank24Frame065;
extern const struct sprite_frame_2box gSpriteBank24Frame066;
extern const struct sprite_frame_2box gSpriteBank24Frame067;
extern const struct sprite_frame_2box gSpriteBank24Frame068;
extern const struct sprite_frame_2box gSpriteBank24Frame069;
extern const struct sprite_frame_2box gSpriteBank24Frame070;
extern const struct sprite_frame_2box gSpriteBank24Frame071;
extern const struct sprite_frame_2box gSpriteBank24Frame072;
extern const struct sprite_frame_2box gSpriteBank24Frame073;
extern const struct sprite_piece_pos gSpriteBank24Frame000Pos[1];
extern const struct sprite_piece_pos gSpriteBank24Frame001Pos[2];
extern const struct sprite_piece_pos gSpriteBank24Frame002Pos[2];
extern const struct sprite_piece_pos gSpriteBank24Frame003Pos[3];
extern const struct sprite_piece_pos gSpriteBank24Frame004Pos[4];
extern const struct sprite_piece_pos gSpriteBank24Frame005Pos[5];
extern const struct sprite_piece_pos gSpriteBank24Frame006Pos[2];
extern const struct sprite_piece_pos gSpriteBank24Frame007Pos[3];
extern const struct sprite_piece_pos gSpriteBank24Frame008Pos[3];
extern const struct sprite_piece_pos gSpriteBank24Frame009Pos[3];
extern const struct sprite_piece_pos gSpriteBank24Frame010Pos[2];
extern const struct sprite_piece_pos gSpriteBank24Frame011Pos[2];
extern const struct sprite_piece_pos gSpriteBank24Frame012Pos[2];
extern const struct sprite_piece_pos gSpriteBank24Frame013Pos[4];
extern const struct sprite_piece_pos gSpriteBank24Frame014Pos[3];
extern const struct sprite_piece_pos gSpriteBank24Frame015Pos[3];
extern const struct sprite_piece_pos gSpriteBank24Frame016Pos[3];
extern const struct sprite_piece_pos gSpriteBank24Frame017Pos[3];
extern const struct sprite_piece_pos gSpriteBank24Frame018Pos[3];
extern const struct sprite_piece_pos gSpriteBank24Frame019Pos[2];
extern const struct sprite_piece_pos gSpriteBank24Frame020Pos[4];
extern const struct sprite_piece_pos gSpriteBank24Frame021Pos[1];
extern const struct sprite_piece_pos gSpriteBank24Frame022Pos[1];
extern const struct sprite_piece_pos gSpriteBank24Frame023Pos[1];
extern const struct sprite_piece_pos gSpriteBank24Frame024Pos[4];
extern const struct sprite_piece_pos gSpriteBank24Frame025Pos[4];
extern const struct sprite_piece_pos gSpriteBank24Frame026Pos[2];
extern const struct sprite_piece_pos gSpriteBank24Frame027Pos[3];
extern const struct sprite_piece_pos gSpriteBank24Frame028Pos[2];
extern const struct sprite_piece_pos gSpriteBank24Frame029Pos[4];
extern const struct sprite_piece_pos gSpriteBank24Frame030Pos[4];
extern const struct sprite_piece_pos gSpriteBank24Frame031Pos[5];
extern const struct sprite_piece_pos gSpriteBank24Frame032Pos[2];
extern const struct sprite_piece_pos gSpriteBank24Frame033Pos[2];
extern const struct sprite_piece_pos gSpriteBank24Frame034Pos[3];
extern const struct sprite_piece_pos gSpriteBank24Frame035Pos[1];
extern const struct sprite_piece_pos gSpriteBank24Frame036Pos[3];
extern const struct sprite_piece_pos gSpriteBank24Frame037Pos[1];
extern const struct sprite_piece_pos gSpriteBank24Frame038Pos[2];
extern const struct sprite_piece_pos gSpriteBank24Frame039Pos[2];
extern const struct sprite_piece_pos gSpriteBank24Frame040Pos[2];
extern const struct sprite_piece_pos gSpriteBank24Frame041Pos[2];
extern const struct sprite_piece_pos gSpriteBank24Frame042Pos[3];
extern const struct sprite_piece_pos gSpriteBank24Frame043Pos[3];
extern const struct sprite_piece_pos gSpriteBank24Frame044Pos[3];
extern const struct sprite_piece_pos gSpriteBank24Frame045Pos[2];
extern const struct sprite_piece_pos gSpriteBank24Frame046Pos[2];
extern const struct sprite_piece_pos gSpriteBank24Frame047Pos[1];
extern const struct sprite_piece_pos gSpriteBank24Frame048Pos[1];
extern const struct sprite_piece_pos gSpriteBank24Frame049Pos[1];
extern const struct sprite_piece_pos gSpriteBank24Frame050Pos[1];
extern const struct sprite_piece_pos gSpriteBank24Frame051Pos[1];
extern const struct sprite_piece_pos gSpriteBank24Frame052Pos[1];
extern const struct sprite_piece_pos gSpriteBank24Frame053Pos[4];
extern const struct sprite_piece_pos gSpriteBank24Frame054Pos[2];
extern const struct sprite_piece_pos gSpriteBank24Frame055Pos[3];
extern const struct sprite_piece_pos gSpriteBank24Frame056Pos[3];
extern const struct sprite_piece_pos gSpriteBank24Frame057Pos[3];
extern const struct sprite_piece_pos gSpriteBank24Frame058Pos[3];
extern const struct sprite_piece_pos gSpriteBank24Frame059Pos[3];
extern const struct sprite_piece_pos gSpriteBank24Frame060Pos[4];
extern const struct sprite_piece_pos gSpriteBank24Frame061Pos[4];
extern const struct sprite_piece_pos gSpriteBank24Frame062Pos[3];
extern const struct sprite_piece_pos gSpriteBank24Frame063Pos[4];
extern const struct sprite_piece_pos gSpriteBank24Frame064Pos[3];
extern const struct sprite_piece_pos gSpriteBank24Frame065Pos[2];
extern const struct sprite_piece_pos gSpriteBank24Frame066Pos[3];
extern const struct sprite_piece_pos gSpriteBank24Frame067Pos[3];
extern const struct sprite_piece_pos gSpriteBank24Frame068Pos[3];
extern const struct sprite_piece_pos gSpriteBank24Frame069Pos[2];
extern const struct sprite_piece_pos gSpriteBank24Frame070Pos[1];
extern const struct sprite_piece_pos gSpriteBank24Frame071Pos[1];
extern const struct sprite_piece_pos gSpriteBank24Frame072Pos[1];
extern const struct sprite_piece_pos gSpriteBank24Frame073Pos[1];
extern const u8 gSpriteBank24Frame000Pieces[1];
extern const u8 gSpriteBank24Frame001Pieces[2];
extern const u8 gSpriteBank24Frame002Pieces[2];
extern const u8 gSpriteBank24Frame003Pieces[3];
extern const u8 gSpriteBank24Frame004Pieces[4];
extern const u8 gSpriteBank24Frame005Pieces[5];
extern const u8 gSpriteBank24Frame006Pieces[2];
extern const u8 gSpriteBank24Frame007Pieces[3];
extern const u8 gSpriteBank24Frame008Pieces[3];
extern const u8 gSpriteBank24Frame009Pieces[3];
extern const u8 gSpriteBank24Frame010Pieces[2];
extern const u8 gSpriteBank24Frame011Pieces[2];
extern const u8 gSpriteBank24Frame012Pieces[2];
extern const u8 gSpriteBank24Frame013Pieces[4];
extern const u8 gSpriteBank24Frame014Pieces[3];
extern const u8 gSpriteBank24Frame015Pieces[3];
extern const u8 gSpriteBank24Frame016Pieces[3];
extern const u8 gSpriteBank24Frame017Pieces[3];
extern const u8 gSpriteBank24Frame018Pieces[3];
extern const u8 gSpriteBank24Frame019Pieces[2];
extern const u8 gSpriteBank24Frame020Pieces[4];
extern const u8 gSpriteBank24Frame021Pieces[1];
extern const u8 gSpriteBank24Frame022Pieces[1];
extern const u8 gSpriteBank24Frame023Pieces[1];
extern const u8 gSpriteBank24Frame024Pieces[4];
extern const u8 gSpriteBank24Frame025Pieces[4];
extern const u8 gSpriteBank24Frame026Pieces[2];
extern const u8 gSpriteBank24Frame027Pieces[3];
extern const u8 gSpriteBank24Frame028Pieces[2];
extern const u8 gSpriteBank24Frame029Pieces[4];
extern const u8 gSpriteBank24Frame030Pieces[4];
extern const u8 gSpriteBank24Frame031Pieces[5];
extern const u8 gSpriteBank24Frame032Pieces[2];
extern const u8 gSpriteBank24Frame033Pieces[2];
extern const u8 gSpriteBank24Frame034Pieces[3];
extern const u8 gSpriteBank24Frame035Pieces[1];
extern const u8 gSpriteBank24Frame036Pieces[3];
extern const u8 gSpriteBank24Frame037Pieces[1];
extern const u8 gSpriteBank24Frame038Pieces[2];
extern const u8 gSpriteBank24Frame039Pieces[2];
extern const u8 gSpriteBank24Frame040Pieces[2];
extern const u8 gSpriteBank24Frame041Pieces[2];
extern const u8 gSpriteBank24Frame042Pieces[3];
extern const u8 gSpriteBank24Frame043Pieces[3];
extern const u8 gSpriteBank24Frame044Pieces[3];
extern const u8 gSpriteBank24Frame045Pieces[2];
extern const u8 gSpriteBank24Frame046Pieces[2];
extern const u8 gSpriteBank24Frame047Pieces[1];
extern const u8 gSpriteBank24Frame048Pieces[1];
extern const u8 gSpriteBank24Frame049Pieces[1];
extern const u8 gSpriteBank24Frame050Pieces[1];
extern const u8 gSpriteBank24Frame051Pieces[1];
extern const u8 gSpriteBank24Frame052Pieces[1];
extern const u8 gSpriteBank24Frame053Pieces[4];
extern const u8 gSpriteBank24Frame054Pieces[2];
extern const u8 gSpriteBank24Frame055Pieces[3];
extern const u8 gSpriteBank24Frame056Pieces[3];
extern const u8 gSpriteBank24Frame057Pieces[3];
extern const u8 gSpriteBank24Frame058Pieces[3];
extern const u8 gSpriteBank24Frame059Pieces[3];
extern const u8 gSpriteBank24Frame060Pieces[4];
extern const u8 gSpriteBank24Frame061Pieces[4];
extern const u8 gSpriteBank24Frame062Pieces[3];
extern const u8 gSpriteBank24Frame063Pieces[4];
extern const u8 gSpriteBank24Frame064Pieces[3];
extern const u8 gSpriteBank24Frame065Pieces[2];
extern const u8 gSpriteBank24Frame066Pieces[3];
extern const u8 gSpriteBank24Frame067Pieces[3];
extern const u8 gSpriteBank24Frame068Pieces[3];
extern const u8 gSpriteBank24Frame069Pieces[2];
extern const u8 gSpriteBank24Frame070Pieces[1];
extern const u8 gSpriteBank24Frame071Pieces[1];
extern const u8 gSpriteBank24Frame072Pieces[1];
extern const u8 gSpriteBank24Frame073Pieces[1];

const struct sprite_anim gSpriteBank24Anims[6] = {
    [0] = {
        .seq = gSpriteBank24Anim00Seq,
        .box = { { -15, -15, 30, 30 }, { -21, -22, 45, 39 } },
        .paletteId = 34,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank24Anim00Seq),
        .flags = 0,
    },
    [1] = {
        .seq = gSpriteBank24Anim01Seq,
        .box = { { -15, -15, 30, 30 }, { -30, -23, 63, 39 } },
        .paletteId = 34,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank24Anim01Seq),
        .flags = 0,
    },
    [2] = {
        .seq = gSpriteBank24Anim02Seq,
        .box = { { -15, -15, 30, 30 }, { -30, -22, 62, 42 } },
        .paletteId = 34,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank24Anim02Seq),
        .flags = 0,
    },
    [3] = {
        .seq = gSpriteBank24Anim03Seq,
        .box = { { -15, -15, 30, 30 }, { -15, -23, 30, 39 } },
        .paletteId = 34,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank24Anim03Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [4] = {
        .seq = gSpriteBank24Anim04Seq,
        .box = { { -15, -15, 30, 30 }, { -30, -23, 63, 39 } },
        .paletteId = 34,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank24Anim04Seq),
        .flags = 0,
    },
    [5] = {
        .seq = gSpriteBank24Anim05Seq,
        .box = { { -15, -15, 30, 30 }, { -30, -23, 63, 39 } },
        .paletteId = 34,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank24Anim05Seq),
        .flags = SPRITE_ANIM_LOOP,
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

const struct sprite_frame *const gSpriteBank24Frames[74] = {
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
    SPRITE_FRAME(gSpriteBank24Frame000, SPRITE_TILES_BANK24 + 0x00000),
    { { -16, -1, 32, 16 }, { -8, -15, 19, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame001 = {
    SPRITE_FRAME(gSpriteBank24Frame001, SPRITE_TILES_BANK24 + 0x00200),
    { { -16, -1, 32, 16 }, { -8, -15, 19, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame002 = {
    SPRITE_FRAME(gSpriteBank24Frame002, SPRITE_TILES_BANK24 + 0x00440),
    { { -16, -1, 32, 16 }, { -8, -15, 19, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame003 = {
    SPRITE_FRAME(gSpriteBank24Frame003, SPRITE_TILES_BANK24 + 0x006c0),
    { { -16, -1, 32, 16 }, { -8, -15, 19, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame004 = {
    SPRITE_FRAME(gSpriteBank24Frame004, SPRITE_TILES_BANK24 + 0x00920),
    { { -16, -1, 32, 16 }, { -8, -15, 19, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame005 = {
    SPRITE_FRAME(gSpriteBank24Frame005, SPRITE_TILES_BANK24 + 0x00b80),
    { { -16, -1, 32, 16 }, { -8, -15, 19, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame006 = {
    SPRITE_FRAME(gSpriteBank24Frame006, SPRITE_TILES_BANK24 + 0x00e40),
    { { -16, -1, 32, 16 }, { -8, -15, 19, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame007 = {
    SPRITE_FRAME(gSpriteBank24Frame007, SPRITE_TILES_BANK24 + 0x01080),
    { { -16, -1, 32, 16 }, { -8, -15, 19, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame008 = {
    SPRITE_FRAME(gSpriteBank24Frame008, SPRITE_TILES_BANK24 + 0x012c0),
    { { -16, -1, 32, 16 }, { -8, -15, 19, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame009 = {
    SPRITE_FRAME(gSpriteBank24Frame009, SPRITE_TILES_BANK24 + 0x01520),
    { { -16, -1, 32, 16 }, { -8, -15, 19, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame010 = {
    SPRITE_FRAME(gSpriteBank24Frame010, SPRITE_TILES_BANK24 + 0x01780),
    { { -16, -1, 32, 16 }, { -8, -15, 19, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame011 = {
    SPRITE_FRAME(gSpriteBank24Frame011, SPRITE_TILES_BANK24 + 0x019a0),
    { { -16, -1, 32, 16 }, { -8, -15, 19, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame012 = {
    SPRITE_FRAME(gSpriteBank24Frame012, SPRITE_TILES_BANK24 + 0x01bc0),
    { { -16, -1, 32, 16 }, { -8, -15, 19, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame013 = {
    SPRITE_FRAME(gSpriteBank24Frame013, SPRITE_TILES_BANK24 + 0x01de0),
    { { -14, -13, 33, 18 }, { -28, 5, 59, 7 } },
};
const struct sprite_frame_2box gSpriteBank24Frame014 = {
    SPRITE_FRAME(gSpriteBank24Frame014, SPRITE_TILES_BANK24 + 0x022c0),
    { { -16, -12, 36, 16 }, { -27, 3, 57, 8 } },
};
const struct sprite_frame_2box gSpriteBank24Frame015 = {
    SPRITE_FRAME(gSpriteBank24Frame015, SPRITE_TILES_BANK24 + 0x02780),
    { { -15, -14, 36, 18 }, { -27, 3, 57, 8 } },
};
const struct sprite_frame_2box gSpriteBank24Frame016 = {
    SPRITE_FRAME(gSpriteBank24Frame016, SPRITE_TILES_BANK24 + 0x02be0),
    { { -17, -10, 38, 14 }, { -27, 1, 58, 12 } },
};
const struct sprite_frame_2box gSpriteBank24Frame017 = {
    SPRITE_FRAME(gSpriteBank24Frame017, SPRITE_TILES_BANK24 + 0x03080),
    { { -16, -13, 37, 19 }, { -27, 5, 58, 8 } },
};
const struct sprite_frame_2box gSpriteBank24Frame018 = {
    SPRITE_FRAME(gSpriteBank24Frame018, SPRITE_TILES_BANK24 + 0x03520),
    { { -15, -14, 35, 16 }, { -28, 1, 58, 7 } },
};
const struct sprite_frame_2box gSpriteBank24Frame019 = {
    SPRITE_FRAME(gSpriteBank24Frame019, SPRITE_TILES_BANK24 + 0x03980),
    { { -11, -20, 24, 34 }, { -23, -7, 51, 5 } },
};
const struct sprite_frame_2box gSpriteBank24Frame020 = {
    SPRITE_FRAME(gSpriteBank24Frame020, SPRITE_TILES_BANK24 + 0x03e00),
    { { -14, 3, 29, 13 }, { -16, -17, 32, 9 } },
};
const struct sprite_frame_2box gSpriteBank24Frame021 = {
    SPRITE_FRAME(gSpriteBank24Frame021, SPRITE_TILES_BANK24 + 0x040e0),
    { { -16, 0, 32, 16 }, { -12, -13, 25, 8 } },
};
const struct sprite_frame_2box gSpriteBank24Frame022 = {
    SPRITE_FRAME(gSpriteBank24Frame022, SPRITE_TILES_BANK24 + 0x042e0),
    { { -16, -2, 32, 18 }, { -12, -14, 25, 8 } },
};
const struct sprite_frame_2box gSpriteBank24Frame023 = {
    SPRITE_FRAME(gSpriteBank24Frame023, SPRITE_TILES_BANK24 + 0x044e0),
    { { -16, -3, 31, 18 }, { -14, -14, 28, 7 } },
};
const struct sprite_frame_2box gSpriteBank24Frame024 = {
    SPRITE_FRAME(gSpriteBank24Frame024, SPRITE_TILES_BANK24 + 0x046e0),
    { { -11, -16, 26, 19 }, { -28, 3, 60, 9 } },
};
const struct sprite_frame_2box gSpriteBank24Frame025 = {
    SPRITE_FRAME(gSpriteBank24Frame025, SPRITE_TILES_BANK24 + 0x04bc0),
    { { -11, -15, 26, 19 }, { -28, 4, 60, 9 } },
};
const struct sprite_frame_2box gSpriteBank24Frame026 = {
    SPRITE_FRAME(gSpriteBank24Frame026, SPRITE_TILES_BANK24 + 0x050a0),
    { { -12, -15, 22, 26 }, { -26, 8, 56, 6 } },
};
const struct sprite_frame_2box gSpriteBank24Frame027 = {
    SPRITE_FRAME(gSpriteBank24Frame027, SPRITE_TILES_BANK24 + 0x05520),
    { { -12, -14, 16, 22 }, { -24, 7, 51, 7 } },
};
const struct sprite_frame_2box gSpriteBank24Frame028 = {
    SPRITE_FRAME(gSpriteBank24Frame028, SPRITE_TILES_BANK24 + 0x059c0),
    { { -10, -15, 19, 25 }, { -20, 8, 45, 8 } },
};
const struct sprite_frame_2box gSpriteBank24Frame029 = {
    SPRITE_FRAME(gSpriteBank24Frame029, SPRITE_TILES_BANK24 + 0x05e40),
    { { -8, -13, 15, 22 }, { -14, 8, 38, 9 } },
};
const struct sprite_frame_2box gSpriteBank24Frame030 = {
    SPRITE_FRAME(gSpriteBank24Frame030, SPRITE_TILES_BANK24 + 0x06120),
    { { -7, -14, 18, 21 }, { -18, 4, 38, 13 } },
};
const struct sprite_frame_2box gSpriteBank24Frame031 = {
    SPRITE_FRAME(gSpriteBank24Frame031, SPRITE_TILES_BANK24 + 0x06460),
    { { -3, -15, 15, 25 }, { -22, 3, 40, 14 } },
};
const struct sprite_frame_2box gSpriteBank24Frame032 = {
    SPRITE_FRAME(gSpriteBank24Frame032, SPRITE_TILES_BANK24 + 0x06840),
    { { -2, -12, 17, 25 }, { -22, 9, 45, 7 } },
};
const struct sprite_frame_2box gSpriteBank24Frame033 = {
    SPRITE_FRAME(gSpriteBank24Frame033, SPRITE_TILES_BANK24 + 0x06cc0),
    { { -4, -12, 20, 21 }, { -24, 11, 51, 4 } },
};
const struct sprite_frame_2box gSpriteBank24Frame034 = {
    SPRITE_FRAME(gSpriteBank24Frame034, SPRITE_TILES_BANK24 + 0x07100),
    { { -5, -15, 22, 24 }, { -28, 9, 58, 5 } },
};
const struct sprite_frame_2box gSpriteBank24Frame035 = {
    SPRITE_FRAME(gSpriteBank24Frame035, SPRITE_TILES_BANK24 + 0x07560),
    { { -6, -14, 24, 23 }, { -29, 10, 60, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame036 = {
    SPRITE_FRAME(gSpriteBank24Frame036, SPRITE_TILES_BANK24 + 0x07960),
    { { -6, -15, 24, 26 }, { -30, 8, 61, 5 } },
};
const struct sprite_frame_2box gSpriteBank24Frame037 = {
    SPRITE_FRAME(gSpriteBank24Frame037, SPRITE_TILES_BANK24 + 0x07dc0),
    { { -16, -3, 31, 16 }, { -10, -16, 21, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame038 = {
    SPRITE_FRAME(gSpriteBank24Frame038, SPRITE_TILES_BANK24 + 0x07fc0),
    { { -16, -3, 31, 16 }, { -10, -16, 21, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame039 = {
    SPRITE_FRAME(gSpriteBank24Frame039, SPRITE_TILES_BANK24 + 0x08200),
    { { -16, -3, 31, 16 }, { -10, -16, 21, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame040 = {
    SPRITE_FRAME(gSpriteBank24Frame040, SPRITE_TILES_BANK24 + 0x08480),
    { { -16, -3, 31, 16 }, { -10, -16, 21, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame041 = {
    SPRITE_FRAME(gSpriteBank24Frame041, SPRITE_TILES_BANK24 + 0x08700),
    { { -16, -3, 31, 16 }, { -10, -16, 21, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame042 = {
    SPRITE_FRAME(gSpriteBank24Frame042, SPRITE_TILES_BANK24 + 0x08940),
    { { -16, -3, 31, 16 }, { -10, -16, 21, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame043 = {
    SPRITE_FRAME(gSpriteBank24Frame043, SPRITE_TILES_BANK24 + 0x08ba0),
    { { -16, -3, 31, 16 }, { -10, -16, 21, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame044 = {
    SPRITE_FRAME(gSpriteBank24Frame044, SPRITE_TILES_BANK24 + 0x08e00),
    { { -16, -3, 31, 16 }, { -10, -16, 21, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame045 = {
    SPRITE_FRAME(gSpriteBank24Frame045, SPRITE_TILES_BANK24 + 0x09060),
    { { -16, -3, 31, 16 }, { -10, -16, 21, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame046 = {
    SPRITE_FRAME(gSpriteBank24Frame046, SPRITE_TILES_BANK24 + 0x092e0),
    { { -16, -3, 31, 16 }, { -10, -16, 21, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame047 = {
    SPRITE_FRAME(gSpriteBank24Frame047, SPRITE_TILES_BANK24 + 0x09560),
    { { -16, -3, 31, 16 }, { -10, -16, 21, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame048 = {
    SPRITE_FRAME(gSpriteBank24Frame048, SPRITE_TILES_BANK24 + 0x09760),
    { { -16, -3, 31, 16 }, { -10, -16, 21, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame049 = {
    SPRITE_FRAME(gSpriteBank24Frame049, SPRITE_TILES_BANK24 + 0x09960),
    { { -16, -3, 31, 16 }, { -10, -16, 21, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame050 = {
    SPRITE_FRAME(gSpriteBank24Frame050, SPRITE_TILES_BANK24 + 0x044e0),
    { { -13, -13, 28, 9 }, { -8, -13, 16, 9 } },
};
const struct sprite_frame_2box gSpriteBank24Frame051 = {
    SPRITE_FRAME(gSpriteBank24Frame051, SPRITE_TILES_BANK24 + 0x042e0),
    { { -13, -10, 26, 6 }, { -8, -12, 18, 7 } },
};
const struct sprite_frame_2box gSpriteBank24Frame052 = {
    SPRITE_FRAME(gSpriteBank24Frame052, SPRITE_TILES_BANK24 + 0x040e0),
    { { -13, -11, 26, 7 }, { -9, -10, 19, 6 } },
};
const struct sprite_frame_1box gSpriteBank24Frame053 = {
    SPRITE_FRAME(gSpriteBank24Frame053, SPRITE_TILES_BANK24 + 0x03e00),
    { { -16, -11, 33, 7 } },
};
const struct sprite_frame_2box gSpriteBank24Frame054 = {
    SPRITE_FRAME(gSpriteBank24Frame054, SPRITE_TILES_BANK24 + 0x03980),
    { { -12, -22, 25, 35 }, { -20, -7, 44, 8 } },
};
const struct sprite_frame_2box gSpriteBank24Frame055 = {
    SPRITE_FRAME(gSpriteBank24Frame055, SPRITE_TILES_BANK24 + 0x03520),
    { { -12, -15, 22, 29 }, { -26, 0, 56, 6 } },
};
const struct sprite_frame_2box gSpriteBank24Frame056 = {
    SPRITE_FRAME(gSpriteBank24Frame056, SPRITE_TILES_BANK24 + 0x03080),
    { { -11, -15, 17, 27 }, { -27, 6, 58, 5 } },
};
const struct sprite_frame_2box gSpriteBank24Frame057 = {
    SPRITE_FRAME(gSpriteBank24Frame057, SPRITE_TILES_BANK24 + 0x02be0),
    { { -12, -15, 18, 29 }, { -27, 3, 57, 8 } },
};
const struct sprite_frame_2box gSpriteBank24Frame058 = {
    SPRITE_FRAME(gSpriteBank24Frame058, SPRITE_TILES_BANK24 + 0x02780),
    { { -13, -14, 20, 29 }, { -26, 2, 55, 8 } },
};
const struct sprite_frame_2box gSpriteBank24Frame059 = {
    SPRITE_FRAME(gSpriteBank24Frame059, SPRITE_TILES_BANK24 + 0x022c0),
    { { -11, -14, 20, 28 }, { -27, 3, 56, 7 } },
};
const struct sprite_frame_2box gSpriteBank24Frame060 = {
    SPRITE_FRAME(gSpriteBank24Frame060, SPRITE_TILES_BANK24 + 0x01de0),
    { { -10, -16, 19, 29 }, { -25, 1, 54, 9 } },
};
const struct sprite_frame_2box gSpriteBank24Frame061 = {
    SPRITE_FRAME(gSpriteBank24Frame061, SPRITE_TILES_BANK24 + 0x09b60),
    { { -15, -17, 30, 24 }, { -27, 3, 57, 9 } },
};
const struct sprite_frame_2box gSpriteBank24Frame062 = {
    SPRITE_FRAME(gSpriteBank24Frame062, SPRITE_TILES_BANK24 + 0x0a020),
    { { -15, -17, 30, 24 }, { -27, 0, 57, 13 } },
};
const struct sprite_frame_2box gSpriteBank24Frame063 = {
    SPRITE_FRAME(gSpriteBank24Frame063, SPRITE_TILES_BANK24 + 0x0a520),
    { { -15, -17, 30, 24 }, { -27, 4, 57, 9 } },
};
const struct sprite_frame_2box gSpriteBank24Frame064 = {
    SPRITE_FRAME(gSpriteBank24Frame064, SPRITE_TILES_BANK24 + 0x0a9e0),
    { { -15, -17, 30, 24 }, { -27, 6, 57, 7 } },
};
const struct sprite_frame_2box gSpriteBank24Frame065 = {
    SPRITE_FRAME(gSpriteBank24Frame065, SPRITE_TILES_BANK24 + 0x0ae40),
    { { -15, -17, 30, 24 }, { -27, 4, 57, 9 } },
};
const struct sprite_frame_2box gSpriteBank24Frame066 = {
    SPRITE_FRAME(gSpriteBank24Frame066, SPRITE_TILES_BANK24 + 0x0b2c0),
    { { -15, -17, 30, 24 }, { -27, 6, 57, 7 } },
};
const struct sprite_frame_2box gSpriteBank24Frame067 = {
    SPRITE_FRAME(gSpriteBank24Frame067, SPRITE_TILES_BANK24 + 0x0b720),
    { { -15, -17, 30, 24 }, { -27, 3, 57, 10 } },
};
const struct sprite_frame_2box gSpriteBank24Frame068 = {
    SPRITE_FRAME(gSpriteBank24Frame068, SPRITE_TILES_BANK24 + 0x0bbe0),
    { { -15, -17, 30, 24 }, { -27, 0, 57, 12 } },
};
const struct sprite_frame_2box gSpriteBank24Frame069 = {
    SPRITE_FRAME(gSpriteBank24Frame069, SPRITE_TILES_BANK24 + 0x0c0e0),
    { { -15, -17, 30, 24 }, { -26, 4, 56, 8 } },
};
const struct sprite_frame_2box gSpriteBank24Frame070 = {
    SPRITE_FRAME(gSpriteBank24Frame070, SPRITE_TILES_BANK24 + 0x0c560),
    { { -15, -17, 30, 24 }, { -26, 9, 56, 3 } },
};
const struct sprite_frame_2box gSpriteBank24Frame071 = {
    SPRITE_FRAME(gSpriteBank24Frame071, SPRITE_TILES_BANK24 + 0x0c960),
    { { -15, -17, 30, 24 }, { -26, 8, 56, 4 } },
};
const struct sprite_frame_2box gSpriteBank24Frame072 = {
    SPRITE_FRAME(gSpriteBank24Frame072, SPRITE_TILES_BANK24 + 0x0cd60),
    { { -15, -17, 30, 24 }, { -26, 7, 56, 5 } },
};
const struct sprite_frame_2box gSpriteBank24Frame073 = {
    SPRITE_FRAME(gSpriteBank24Frame073, SPRITE_TILES_BANK24 + 0x0d160),
    { { -15, -17, 30, 24 }, { -27, 8, 57, 4 } },
};

const struct sprite_piece_pos gSpriteBank24Frame000Pos[1] = { { -15, -15 } };
const struct sprite_piece_pos gSpriteBank24Frame001Pos[2] = { { -15, -17 }, { -6, 15 } };
const struct sprite_piece_pos gSpriteBank24Frame002Pos[2] = { { -15, -22 }, { -12, 10 } };
const struct sprite_piece_pos gSpriteBank24Frame003Pos[3] = { { -16, -20 }, { -11, 12 }, { 5, 12 } };
const struct sprite_piece_pos gSpriteBank24Frame004Pos[4] = { { -19, -16 }, { 13, -3 }, { -5, 16 }, { 3, 16 } };
const struct sprite_piece_pos gSpriteBank24Frame005Pos[5] = { { -21, -17 }, { 11, -11 }, { 11, 5 }, { -7, 15 }, { 9, 15 } };
const struct sprite_piece_pos gSpriteBank24Frame006Pos[2] = { { -16, -16 }, { -7, 16 } };
const struct sprite_piece_pos gSpriteBank24Frame007Pos[3] = { { -14, -16 }, { -3, 16 }, { 5, 16 } };
const struct sprite_piece_pos gSpriteBank24Frame008Pos[3] = { { -13, -20 }, { -10, 12 }, { 6, 12 } };
const struct sprite_piece_pos gSpriteBank24Frame009Pos[3] = { { -12, -21 }, { -9, 11 }, { 7, 11 } };
const struct sprite_piece_pos gSpriteBank24Frame010Pos[2] = { { -12, -13 }, { 20, -14 } };
const struct sprite_piece_pos gSpriteBank24Frame011Pos[2] = { { -12, -13 }, { 21, -11 } };
const struct sprite_piece_pos gSpriteBank24Frame012Pos[2] = { { -12, -15 }, { 20, -12 } };
const struct sprite_piece_pos gSpriteBank24Frame013Pos[4] = { { -30, -22 }, { -21, 10 }, { 11, 10 }, { 27, 10 } };
const struct sprite_piece_pos gSpriteBank24Frame014Pos[3] = { { -30, -21 }, { -20, 11 }, { 12, 11 } };
const struct sprite_piece_pos gSpriteBank24Frame015Pos[3] = { { -30, -17 }, { -6, 15 }, { 10, 15 } };
const struct sprite_piece_pos gSpriteBank24Frame016Pos[3] = { { -30, -18 }, { -8, 14 }, { 24, 14 } };
const struct sprite_piece_pos gSpriteBank24Frame017Pos[3] = { { -30, -18 }, { -8, 14 }, { 24, 14 } };
const struct sprite_piece_pos gSpriteBank24Frame018Pos[3] = { { -29, -18 }, { -8, 14 }, { 8, 14 } };
const struct sprite_piece_pos gSpriteBank24Frame019Pos[2] = { { -25, -23 }, { -14, 9 } };
const struct sprite_piece_pos gSpriteBank24Frame020Pos[4] = { { -17, -22 }, { 15, -12 }, { 15, 4 }, { -12, 10 } };
const struct sprite_piece_pos gSpriteBank24Frame021Pos[1] = { { -15, -16 } };
const struct sprite_piece_pos gSpriteBank24Frame022Pos[1] = { { -15, -15 } };
const struct sprite_piece_pos gSpriteBank24Frame023Pos[1] = { { -15, -16 } };
const struct sprite_piece_pos gSpriteBank24Frame024Pos[4] = { { -29, -22 }, { -21, 10 }, { 11, 10 }, { 27, 10 } };
const struct sprite_piece_pos gSpriteBank24Frame025Pos[4] = { { -28, -21 }, { -19, 11 }, { 13, 11 }, { 31, 11 } };
const struct sprite_piece_pos gSpriteBank24Frame026Pos[2] = { { -29, -17 }, { -7, 15 } };
const struct sprite_piece_pos gSpriteBank24Frame027Pos[3] = { { -27, -18 }, { -9, 14 }, { 23, 14 } };
const struct sprite_piece_pos gSpriteBank24Frame028Pos[2] = { { -21, -17 }, { -5, 15 } };
const struct sprite_piece_pos gSpriteBank24Frame029Pos[4] = { { -16, -17 }, { 16, 5 }, { 24, 8 }, { -11, 15 } };
const struct sprite_piece_pos gSpriteBank24Frame030Pos[4] = { { -17, -21 }, { 14, 6 }, { -18, 11 }, { 14, 11 } };
const struct sprite_piece_pos gSpriteBank24Frame031Pos[5] = { { -24, -14 }, { 8, -22 }, { 16, -6 }, { -20, 10 }, { 12, 10 } };
const struct sprite_piece_pos gSpriteBank24Frame032Pos[2] = { { -24, -16 }, { -18, 16 } };
const struct sprite_piece_pos gSpriteBank24Frame033Pos[2] = { { -27, -14 }, { -9, 18 } };
const struct sprite_piece_pos gSpriteBank24Frame034Pos[3] = { { -29, -16 }, { -16, 16 }, { 0, 16 } };
const struct sprite_piece_pos gSpriteBank24Frame035Pos[1] = { { -30, -15 } };
const struct sprite_piece_pos gSpriteBank24Frame036Pos[3] = { { -30, -17 }, { -11, 15 }, { 5, 15 } };
const struct sprite_piece_pos gSpriteBank24Frame037Pos[1] = { { -15, -15 } };
const struct sprite_piece_pos gSpriteBank24Frame038Pos[2] = { { -15, -17 }, { -5, 15 } };
const struct sprite_piece_pos gSpriteBank24Frame039Pos[2] = { { -15, -22 }, { -12, 10 } };
const struct sprite_piece_pos gSpriteBank24Frame040Pos[2] = { { -15, -21 }, { -12, 11 } };
const struct sprite_piece_pos gSpriteBank24Frame041Pos[2] = { { -15, -17 }, { -6, 15 } };
const struct sprite_piece_pos gSpriteBank24Frame042Pos[3] = { { -15, -18 }, { -8, 14 }, { 8, 14 } };
const struct sprite_piece_pos gSpriteBank24Frame043Pos[3] = { { -15, -18 }, { -8, 14 }, { 8, 14 } };
const struct sprite_piece_pos gSpriteBank24Frame044Pos[3] = { { -15, -18 }, { -8, 14 }, { 8, 14 } };
const struct sprite_piece_pos gSpriteBank24Frame045Pos[2] = { { -15, -23 }, { -12, 9 } };
const struct sprite_piece_pos gSpriteBank24Frame046Pos[2] = { { -15, -22 }, { -12, 10 } };
const struct sprite_piece_pos gSpriteBank24Frame047Pos[1] = { { -15, -16 } };
const struct sprite_piece_pos gSpriteBank24Frame048Pos[1] = { { -15, -15 } };
const struct sprite_piece_pos gSpriteBank24Frame049Pos[1] = { { -15, -16 } };
const struct sprite_piece_pos gSpriteBank24Frame050Pos[1] = { { -15, -16 } };
const struct sprite_piece_pos gSpriteBank24Frame051Pos[1] = { { -15, -15 } };
const struct sprite_piece_pos gSpriteBank24Frame052Pos[1] = { { -15, -16 } };
const struct sprite_piece_pos gSpriteBank24Frame053Pos[4] = { { -17, -22 }, { 15, -12 }, { 15, 4 }, { -12, 10 } };
const struct sprite_piece_pos gSpriteBank24Frame054Pos[2] = { { -25, -23 }, { -14, 9 } };
const struct sprite_piece_pos gSpriteBank24Frame055Pos[3] = { { -29, -18 }, { -8, 14 }, { 8, 14 } };
const struct sprite_piece_pos gSpriteBank24Frame056Pos[3] = { { -30, -18 }, { -8, 14 }, { 24, 14 } };
const struct sprite_piece_pos gSpriteBank24Frame057Pos[3] = { { -30, -18 }, { -8, 14 }, { 24, 14 } };
const struct sprite_piece_pos gSpriteBank24Frame058Pos[3] = { { -30, -17 }, { -6, 15 }, { 10, 15 } };
const struct sprite_piece_pos gSpriteBank24Frame059Pos[3] = { { -30, -21 }, { -20, 11 }, { 12, 11 } };
const struct sprite_piece_pos gSpriteBank24Frame060Pos[4] = { { -30, -22 }, { -21, 10 }, { 11, 10 }, { 27, 10 } };
const struct sprite_piece_pos gSpriteBank24Frame061Pos[4] = { { -29, -20 }, { -17, 12 }, { 15, 12 }, { 23, 12 } };
const struct sprite_piece_pos gSpriteBank24Frame062Pos[3] = { { -29, -23 }, { -23, 9 }, { 9, 9 } };
const struct sprite_piece_pos gSpriteBank24Frame063Pos[4] = { { -30, -19 }, { -17, 13 }, { 15, 13 }, { 24, 13 } };
const struct sprite_piece_pos gSpriteBank24Frame064Pos[3] = { { -30, -17 }, { -6, 15 }, { 10, 15 } };
const struct sprite_piece_pos gSpriteBank24Frame065Pos[2] = { { -30, -19 }, { -10, 13 } };
const struct sprite_piece_pos gSpriteBank24Frame066Pos[3] = { { -30, -17 }, { -6, 15 }, { 10, 15 } };
const struct sprite_piece_pos gSpriteBank24Frame067Pos[3] = { { -30, -21 }, { -20, 11 }, { 12, 11 } };
const struct sprite_piece_pos gSpriteBank24Frame068Pos[3] = { { -30, -23 }, { -23, 9 }, { 9, 9 } };
const struct sprite_piece_pos gSpriteBank24Frame069Pos[2] = { { -29, -19 }, { -9, 13 } };
const struct sprite_piece_pos gSpriteBank24Frame070Pos[1] = { { -30, -14 } };
const struct sprite_piece_pos gSpriteBank24Frame071Pos[1] = { { -29, -15 } };
const struct sprite_piece_pos gSpriteBank24Frame072Pos[1] = { { -30, -16 } };
const struct sprite_piece_pos gSpriteBank24Frame073Pos[1] = { { -30, -15 } };

const u8 gSpriteBank24Frame000Pieces[1] = { SPRITE_PIECE(3, 2) };
const u8 gSpriteBank24Frame001Pieces[2] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank24Frame002Pieces[2] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank24Frame003Pieces[3] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame004Pieces[4] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame005Pieces[5] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame006Pieces[2] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank24Frame007Pieces[3] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame008Pieces[3] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame009Pieces[3] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame010Pieces[2] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame011Pieces[2] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame012Pieces[2] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame013Pieces[4] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame014Pieces[3] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank24Frame015Pieces[3] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame016Pieces[3] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame017Pieces[3] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame018Pieces[3] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame019Pieces[2] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank24Frame020Pieces[4] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank24Frame021Pieces[1] = { SPRITE_PIECE(3, 2) };
const u8 gSpriteBank24Frame022Pieces[1] = { SPRITE_PIECE(3, 2) };
const u8 gSpriteBank24Frame023Pieces[1] = { SPRITE_PIECE(3, 2) };
const u8 gSpriteBank24Frame024Pieces[4] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame025Pieces[4] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame026Pieces[2] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank24Frame027Pieces[3] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame028Pieces[2] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank24Frame029Pieces[4] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank24Frame030Pieces[4] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame031Pieces[5] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame032Pieces[2] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank24Frame033Pieces[2] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank24Frame034Pieces[3] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame035Pieces[1] = { SPRITE_PIECE(3, 7) };
const u8 gSpriteBank24Frame036Pieces[3] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame037Pieces[1] = { SPRITE_PIECE(3, 2) };
const u8 gSpriteBank24Frame038Pieces[2] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank24Frame039Pieces[2] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank24Frame040Pieces[2] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank24Frame041Pieces[2] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank24Frame042Pieces[3] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame043Pieces[3] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame044Pieces[3] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame045Pieces[2] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank24Frame046Pieces[2] = { SPRITE_PIECE(3, 2), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank24Frame047Pieces[1] = { SPRITE_PIECE(3, 2) };
const u8 gSpriteBank24Frame048Pieces[1] = { SPRITE_PIECE(3, 2) };
const u8 gSpriteBank24Frame049Pieces[1] = { SPRITE_PIECE(3, 2) };
const u8 gSpriteBank24Frame050Pieces[1] = { SPRITE_PIECE(3, 2) };
const u8 gSpriteBank24Frame051Pieces[1] = { SPRITE_PIECE(3, 2) };
const u8 gSpriteBank24Frame052Pieces[1] = { SPRITE_PIECE(3, 2) };
const u8 gSpriteBank24Frame053Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank24Frame054Pieces[2] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank24Frame055Pieces[3] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame056Pieces[3] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame057Pieces[3] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame058Pieces[3] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame059Pieces[3] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank24Frame060Pieces[4] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame061Pieces[4] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame062Pieces[3] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank24Frame063Pieces[4] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame064Pieces[3] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame065Pieces[2] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank24Frame066Pieces[3] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank24Frame067Pieces[3] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank24Frame068Pieces[3] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank24Frame069Pieces[2] = { SPRITE_PIECE(3, 7), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank24Frame070Pieces[1] = { SPRITE_PIECE(3, 7) };
const u8 gSpriteBank24Frame071Pieces[1] = { SPRITE_PIECE(3, 7) };
const u8 gSpriteBank24Frame072Pieces[1] = { SPRITE_PIECE(3, 7) };
const u8 gSpriteBank24Frame073Pieces[1] = { SPRITE_PIECE(3, 7) };

/* ---------------------------------------------------------------------- */
/* Bank 25: 1 animation, 16 frames, tiles in gSpriteBank25Tiles (SPRITE_TILES_BANK25). */

extern const u16 gSpriteBank25Anim00Seq[16];
extern const struct sprite_frame_1box gSpriteBank25Frame000;
extern const struct sprite_frame_1box gSpriteBank25Frame001;
extern const struct sprite_frame_1box gSpriteBank25Frame002;
extern const struct sprite_frame_1box gSpriteBank25Frame003;
extern const struct sprite_frame_1box gSpriteBank25Frame004;
extern const struct sprite_frame_1box gSpriteBank25Frame005;
extern const struct sprite_frame_1box gSpriteBank25Frame006;
extern const struct sprite_frame_1box gSpriteBank25Frame007;
extern const struct sprite_frame_1box gSpriteBank25Frame008;
extern const struct sprite_frame_1box gSpriteBank25Frame009;
extern const struct sprite_frame_1box gSpriteBank25Frame010;
extern const struct sprite_frame_1box gSpriteBank25Frame011;
extern const struct sprite_frame_1box gSpriteBank25Frame012;
extern const struct sprite_frame_1box gSpriteBank25Frame013;
extern const struct sprite_frame_1box gSpriteBank25Frame014;
extern const struct sprite_frame_1box gSpriteBank25Frame015;
extern const struct sprite_piece_pos gSpriteBank25Frame000Pos[1];
extern const struct sprite_piece_pos gSpriteBank25Frame001Pos[1];
extern const struct sprite_piece_pos gSpriteBank25Frame002Pos[1];
extern const struct sprite_piece_pos gSpriteBank25Frame003Pos[1];
extern const struct sprite_piece_pos gSpriteBank25Frame004Pos[1];
extern const struct sprite_piece_pos gSpriteBank25Frame005Pos[1];
extern const struct sprite_piece_pos gSpriteBank25Frame006Pos[1];
extern const struct sprite_piece_pos gSpriteBank25Frame007Pos[1];
extern const struct sprite_piece_pos gSpriteBank25Frame008Pos[1];
extern const struct sprite_piece_pos gSpriteBank25Frame009Pos[1];
extern const struct sprite_piece_pos gSpriteBank25Frame010Pos[1];
extern const struct sprite_piece_pos gSpriteBank25Frame011Pos[1];
extern const struct sprite_piece_pos gSpriteBank25Frame012Pos[1];
extern const struct sprite_piece_pos gSpriteBank25Frame013Pos[1];
extern const struct sprite_piece_pos gSpriteBank25Frame014Pos[1];
extern const struct sprite_piece_pos gSpriteBank25Frame015Pos[1];
extern const u8 gSpriteBank25Frame000Pieces[1];
extern const u8 gSpriteBank25Frame001Pieces[1];
extern const u8 gSpriteBank25Frame002Pieces[1];
extern const u8 gSpriteBank25Frame003Pieces[1];
extern const u8 gSpriteBank25Frame004Pieces[1];
extern const u8 gSpriteBank25Frame005Pieces[1];
extern const u8 gSpriteBank25Frame006Pieces[1];
extern const u8 gSpriteBank25Frame007Pieces[1];
extern const u8 gSpriteBank25Frame008Pieces[1];
extern const u8 gSpriteBank25Frame009Pieces[1];
extern const u8 gSpriteBank25Frame010Pieces[1];
extern const u8 gSpriteBank25Frame011Pieces[1];
extern const u8 gSpriteBank25Frame012Pieces[1];
extern const u8 gSpriteBank25Frame013Pieces[1];
extern const u8 gSpriteBank25Frame014Pieces[1];
extern const u8 gSpriteBank25Frame015Pieces[1];

const struct sprite_anim gSpriteBank25Anims[1] = {
    [0] = {
        .seq = gSpriteBank25Anim00Seq,
        .box = { { -30, -22, 60, 44 }, { -31, -32, 61, 59 } },
        .paletteId = 35,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank25Anim00Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank25Anim00Seq[16] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
};

const struct sprite_frame *const gSpriteBank25Frames[16] = {
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
    SPRITE_FRAME(gSpriteBank25Frame000, SPRITE_TILES_BANK25 + 0x00000),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame001 = {
    SPRITE_FRAME(gSpriteBank25Frame001, SPRITE_TILES_BANK25 + 0x00780),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame002 = {
    SPRITE_FRAME(gSpriteBank25Frame002, SPRITE_TILES_BANK25 + 0x00f00),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame003 = {
    SPRITE_FRAME(gSpriteBank25Frame003, SPRITE_TILES_BANK25 + 0x01680),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame004 = {
    SPRITE_FRAME(gSpriteBank25Frame004, SPRITE_TILES_BANK25 + 0x01e00),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame005 = {
    SPRITE_FRAME(gSpriteBank25Frame005, SPRITE_TILES_BANK25 + 0x02580),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame006 = {
    SPRITE_FRAME(gSpriteBank25Frame006, SPRITE_TILES_BANK25 + 0x02d00),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame007 = {
    SPRITE_FRAME(gSpriteBank25Frame007, SPRITE_TILES_BANK25 + 0x03480),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame008 = {
    SPRITE_FRAME(gSpriteBank25Frame008, SPRITE_TILES_BANK25 + 0x03c00),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame009 = {
    SPRITE_FRAME(gSpriteBank25Frame009, SPRITE_TILES_BANK25 + 0x04380),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame010 = {
    SPRITE_FRAME(gSpriteBank25Frame010, SPRITE_TILES_BANK25 + 0x04b00),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame011 = {
    SPRITE_FRAME(gSpriteBank25Frame011, SPRITE_TILES_BANK25 + 0x05280),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame012 = {
    SPRITE_FRAME(gSpriteBank25Frame012, SPRITE_TILES_BANK25 + 0x05a00),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame013 = {
    SPRITE_FRAME(gSpriteBank25Frame013, SPRITE_TILES_BANK25 + 0x06180),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame014 = {
    SPRITE_FRAME(gSpriteBank25Frame014, SPRITE_TILES_BANK25 + 0x06900),
    { { -17, -1, 18, 17 } },
};
const struct sprite_frame_1box gSpriteBank25Frame015 = {
    SPRITE_FRAME(gSpriteBank25Frame015, SPRITE_TILES_BANK25 + 0x07080),
    { { -17, -1, 18, 17 } },
};

const struct sprite_piece_pos gSpriteBank25Frame000Pos[1] = { { -30, -29 } };
const struct sprite_piece_pos gSpriteBank25Frame001Pos[1] = { { -30, -29 } };
const struct sprite_piece_pos gSpriteBank25Frame002Pos[1] = { { -30, -30 } };
const struct sprite_piece_pos gSpriteBank25Frame003Pos[1] = { { -30, -30 } };
const struct sprite_piece_pos gSpriteBank25Frame004Pos[1] = { { -31, -31 } };
const struct sprite_piece_pos gSpriteBank25Frame005Pos[1] = { { -31, -32 } };
const struct sprite_piece_pos gSpriteBank25Frame006Pos[1] = { { -31, -32 } };
const struct sprite_piece_pos gSpriteBank25Frame007Pos[1] = { { -31, -31 } };
const struct sprite_piece_pos gSpriteBank25Frame008Pos[1] = { { -31, -31 } };
const struct sprite_piece_pos gSpriteBank25Frame009Pos[1] = { { -31, -31 } };
const struct sprite_piece_pos gSpriteBank25Frame010Pos[1] = { { -31, -30 } };
const struct sprite_piece_pos gSpriteBank25Frame011Pos[1] = { { -30, -29 } };
const struct sprite_piece_pos gSpriteBank25Frame012Pos[1] = { { -30, -29 } };
const struct sprite_piece_pos gSpriteBank25Frame013Pos[1] = { { -31, -29 } };
const struct sprite_piece_pos gSpriteBank25Frame014Pos[1] = { { -31, -29 } };
const struct sprite_piece_pos gSpriteBank25Frame015Pos[1] = { { -30, -29 } };

const u8 gSpriteBank25Frame000Pieces[1] = { SPRITE_PIECE(5, 3) };
const u8 gSpriteBank25Frame001Pieces[1] = { SPRITE_PIECE(5, 3) };
const u8 gSpriteBank25Frame002Pieces[1] = { SPRITE_PIECE(5, 3) };
const u8 gSpriteBank25Frame003Pieces[1] = { SPRITE_PIECE(5, 3) };
const u8 gSpriteBank25Frame004Pieces[1] = { SPRITE_PIECE(5, 3) };
const u8 gSpriteBank25Frame005Pieces[1] = { SPRITE_PIECE(5, 3) };
const u8 gSpriteBank25Frame006Pieces[1] = { SPRITE_PIECE(5, 3) };
const u8 gSpriteBank25Frame007Pieces[1] = { SPRITE_PIECE(5, 3) };
const u8 gSpriteBank25Frame008Pieces[1] = { SPRITE_PIECE(5, 3) };
const u8 gSpriteBank25Frame009Pieces[1] = { SPRITE_PIECE(5, 3) };
const u8 gSpriteBank25Frame010Pieces[1] = { SPRITE_PIECE(5, 3) };
const u8 gSpriteBank25Frame011Pieces[1] = { SPRITE_PIECE(5, 3) };
const u8 gSpriteBank25Frame012Pieces[1] = { SPRITE_PIECE(5, 3) };
const u8 gSpriteBank25Frame013Pieces[1] = { SPRITE_PIECE(5, 3) };
const u8 gSpriteBank25Frame014Pieces[1] = { SPRITE_PIECE(5, 3) };
const u8 gSpriteBank25Frame015Pieces[1] = { SPRITE_PIECE(5, 3) };

/* ---------------------------------------------------------------------- */
/* Bank 26: 2 animations, 14 frames, tiles in gSpriteBank26Tiles (SPRITE_TILES_BANK26). */

extern const u16 gSpriteBank26Anim00Seq[9];
extern const u16 gSpriteBank26Anim01Seq[5];
extern const struct sprite_frame_1box gSpriteBank26Frame000;
extern const struct sprite_frame_1box gSpriteBank26Frame001;
extern const struct sprite_frame_1box gSpriteBank26Frame002;
extern const struct sprite_frame_1box gSpriteBank26Frame003;
extern const struct sprite_frame_1box gSpriteBank26Frame004;
extern const struct sprite_frame_1box gSpriteBank26Frame005;
extern const struct sprite_frame_1box gSpriteBank26Frame006;
extern const struct sprite_frame_1box gSpriteBank26Frame007;
extern const struct sprite_frame_1box gSpriteBank26Frame008;
extern const struct sprite_frame_1box gSpriteBank26Frame009;
extern const struct sprite_frame_1box gSpriteBank26Frame010;
extern const struct sprite_frame_1box gSpriteBank26Frame011;
extern const struct sprite_frame_1box gSpriteBank26Frame012;
extern const struct sprite_frame_1box gSpriteBank26Frame013;
extern const struct sprite_piece_pos gSpriteBank26Frame000Pos[3];
extern const struct sprite_piece_pos gSpriteBank26Frame001Pos[2];
extern const struct sprite_piece_pos gSpriteBank26Frame002Pos[2];
extern const struct sprite_piece_pos gSpriteBank26Frame003Pos[2];
extern const struct sprite_piece_pos gSpriteBank26Frame004Pos[1];
extern const struct sprite_piece_pos gSpriteBank26Frame005Pos[1];
extern const struct sprite_piece_pos gSpriteBank26Frame006Pos[2];
extern const struct sprite_piece_pos gSpriteBank26Frame007Pos[2];
extern const struct sprite_piece_pos gSpriteBank26Frame008Pos[2];
extern const struct sprite_piece_pos gSpriteBank26Frame009Pos[2];
extern const struct sprite_piece_pos gSpriteBank26Frame010Pos[2];
extern const struct sprite_piece_pos gSpriteBank26Frame011Pos[2];
extern const struct sprite_piece_pos gSpriteBank26Frame012Pos[2];
extern const struct sprite_piece_pos gSpriteBank26Frame013Pos[3];
extern const u8 gSpriteBank26Frame000Pieces[3];
extern const u8 gSpriteBank26Frame001Pieces[2];
extern const u8 gSpriteBank26Frame002Pieces[2];
extern const u8 gSpriteBank26Frame003Pieces[2];
extern const u8 gSpriteBank26Frame004Pieces[1];
extern const u8 gSpriteBank26Frame005Pieces[1];
extern const u8 gSpriteBank26Frame006Pieces[2];
extern const u8 gSpriteBank26Frame007Pieces[2];
extern const u8 gSpriteBank26Frame008Pieces[2];
extern const u8 gSpriteBank26Frame009Pieces[2];
extern const u8 gSpriteBank26Frame010Pieces[2];
extern const u8 gSpriteBank26Frame011Pieces[2];
extern const u8 gSpriteBank26Frame012Pieces[2];
extern const u8 gSpriteBank26Frame013Pieces[3];

const struct sprite_anim gSpriteBank26Anims[2] = {
    [0] = {
        .seq = gSpriteBank26Anim00Seq,
        .box = { { -15, -49, 30, 99 }, { -15, -49, 30, 99 } },
        .paletteId = 36,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank26Anim00Seq),
        .flags = 0,
    },
    [1] = {
        .seq = gSpriteBank26Anim01Seq,
        .box = { { -15, -49, 30, 99 }, { -15, -49, 30, 99 } },
        .paletteId = 36,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank26Anim01Seq),
        .flags = 0,
    },
};

const u16 gSpriteBank26Anim00Seq[9] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8,
};
const u16 gSpriteBank26Anim01Seq[5] = {
    9, 10, 11, 12, 13,
};

const struct sprite_frame *const gSpriteBank26Frames[14] = {
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
    SPRITE_FRAME(gSpriteBank26Frame000, SPRITE_TILES_BANK26 + 0x00000),
    { { -10, -44, 19, 90 } },
};
const struct sprite_frame_1box gSpriteBank26Frame001 = {
    SPRITE_FRAME(gSpriteBank26Frame001, SPRITE_TILES_BANK26 + 0x00680),
    { { -9, -43, 18, 84 } },
};
const struct sprite_frame_1box gSpriteBank26Frame002 = {
    SPRITE_FRAME(gSpriteBank26Frame002, SPRITE_TILES_BANK26 + 0x00c80),
    { { -10, -41, 20, 74 } },
};
const struct sprite_frame_1box gSpriteBank26Frame003 = {
    SPRITE_FRAME(gSpriteBank26Frame003, SPRITE_TILES_BANK26 + 0x01280),
    { { -9, -40, 19, 60 } },
};
const struct sprite_frame_1box gSpriteBank26Frame004 = {
    SPRITE_FRAME(gSpriteBank26Frame004, SPRITE_TILES_BANK26 + 0x01780),
    { { -10, -40, 22, 50 } },
};
const struct sprite_frame_1box gSpriteBank26Frame005 = {
    SPRITE_FRAME(gSpriteBank26Frame005, SPRITE_TILES_BANK26 + 0x01b80),
    { { -11, -38, 23, 35 } },
};
const struct sprite_frame_1box gSpriteBank26Frame006 = {
    SPRITE_FRAME(gSpriteBank26Frame006, SPRITE_TILES_BANK26 + 0x01f80),
    { { -11, -41, 23, 29 } },
};
const struct sprite_frame_1box gSpriteBank26Frame007 = {
    SPRITE_FRAME(gSpriteBank26Frame007, SPRITE_TILES_BANK26 + 0x02280),
    { { -11, -39, 23, 18 } },
};
const struct sprite_frame_1box gSpriteBank26Frame008 = {
    SPRITE_FRAME(gSpriteBank26Frame008, SPRITE_TILES_BANK26 + 0x02500),
    { { -12, -40, 25, 19 } },
};
const struct sprite_frame_1box gSpriteBank26Frame009 = {
    SPRITE_FRAME(gSpriteBank26Frame009, SPRITE_TILES_BANK26 + 0x02780),
    { { -10, -39, 21, 18 } },
};
const struct sprite_frame_1box gSpriteBank26Frame010 = {
    SPRITE_FRAME(gSpriteBank26Frame010, SPRITE_TILES_BANK26 + 0x02a00),
    { { -12, -38, 25, 21 } },
};
const struct sprite_frame_1box gSpriteBank26Frame011 = {
    SPRITE_FRAME(gSpriteBank26Frame011, SPRITE_TILES_BANK26 + 0x02c80),
    { { -12, -38, 25, 31 } },
};
const struct sprite_frame_1box gSpriteBank26Frame012 = {
    SPRITE_FRAME(gSpriteBank26Frame012, SPRITE_TILES_BANK26 + 0x02f80),
    { { -11, -40, 23, 55 } },
};
const struct sprite_frame_1box gSpriteBank26Frame013 = {
    SPRITE_FRAME(gSpriteBank26Frame013, SPRITE_TILES_BANK26 + 0x03400),
    { { -11, -38, 21, 83 } },
};

const struct sprite_piece_pos gSpriteBank26Frame000Pos[3] = { { -15, -49 }, { -14, 15 }, { -12, 47 } };
const struct sprite_piece_pos gSpriteBank26Frame001Pos[2] = { { -15, -49 }, { -14, 15 } };
const struct sprite_piece_pos gSpriteBank26Frame002Pos[2] = { { -15, -49 }, { -14, 15 } };
const struct sprite_piece_pos gSpriteBank26Frame003Pos[2] = { { -15, -49 }, { -14, 15 } };
const struct sprite_piece_pos gSpriteBank26Frame004Pos[1] = { { -15, -49 } };
const struct sprite_piece_pos gSpriteBank26Frame005Pos[1] = { { -15, -49 } };
const struct sprite_piece_pos gSpriteBank26Frame006Pos[2] = { { -15, -49 }, { -14, -17 } };
const struct sprite_piece_pos gSpriteBank26Frame007Pos[2] = { { -15, -49 }, { -15, -17 } };
const struct sprite_piece_pos gSpriteBank26Frame008Pos[2] = { { -15, -49 }, { -15, -17 } };
const struct sprite_piece_pos gSpriteBank26Frame009Pos[2] = { { -15, -49 }, { -14, -17 } };
const struct sprite_piece_pos gSpriteBank26Frame010Pos[2] = { { -15, -49 }, { -14, -17 } };
const struct sprite_piece_pos gSpriteBank26Frame011Pos[2] = { { -15, -49 }, { -14, -17 } };
const struct sprite_piece_pos gSpriteBank26Frame012Pos[2] = { { -15, -49 }, { -14, 15 } };
const struct sprite_piece_pos gSpriteBank26Frame013Pos[3] = { { -15, -49 }, { -14, 15 }, { -13, 47 } };

const u8 gSpriteBank26Frame000Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank26Frame001Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 2) };
const u8 gSpriteBank26Frame002Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 2) };
const u8 gSpriteBank26Frame003Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank26Frame004Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank26Frame005Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank26Frame006Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank26Frame007Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank26Frame008Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank26Frame009Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank26Frame010Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank26Frame011Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank26Frame012Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank26Frame013Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 5) };

/* ---------------------------------------------------------------------- */
/* Bank 27: 2 animations, 30 frames, tiles in gSpriteBank27Tiles (SPRITE_TILES_BANK27). */

extern const u16 gSpriteBank27Anim00Seq[15];
extern const u16 gSpriteBank27Anim01Seq[15];
extern const struct sprite_frame_1box gSpriteBank27Frame000;
extern const struct sprite_frame_1box gSpriteBank27Frame001;
extern const struct sprite_frame_1box gSpriteBank27Frame002;
extern const struct sprite_frame_1box gSpriteBank27Frame003;
extern const struct sprite_frame_1box gSpriteBank27Frame004;
extern const struct sprite_frame_1box gSpriteBank27Frame005;
extern const struct sprite_frame_1box gSpriteBank27Frame006;
extern const struct sprite_frame_1box gSpriteBank27Frame007;
extern const struct sprite_frame_1box gSpriteBank27Frame008;
extern const struct sprite_frame_1box gSpriteBank27Frame009;
extern const struct sprite_frame_1box gSpriteBank27Frame010;
extern const struct sprite_frame_1box gSpriteBank27Frame011;
extern const struct sprite_frame_1box gSpriteBank27Frame012;
extern const struct sprite_frame_1box gSpriteBank27Frame013;
extern const struct sprite_frame_1box gSpriteBank27Frame014;
extern const struct sprite_frame_1box gSpriteBank27Frame015;
extern const struct sprite_frame_1box gSpriteBank27Frame016;
extern const struct sprite_frame_1box gSpriteBank27Frame017;
extern const struct sprite_frame_1box gSpriteBank27Frame018;
extern const struct sprite_frame_1box gSpriteBank27Frame019;
extern const struct sprite_frame_1box gSpriteBank27Frame020;
extern const struct sprite_frame_1box gSpriteBank27Frame021;
extern const struct sprite_frame_1box gSpriteBank27Frame022;
extern const struct sprite_frame_1box gSpriteBank27Frame023;
extern const struct sprite_frame_1box gSpriteBank27Frame024;
extern const struct sprite_frame_1box gSpriteBank27Frame025;
extern const struct sprite_frame_1box gSpriteBank27Frame026;
extern const struct sprite_frame_1box gSpriteBank27Frame027;
extern const struct sprite_frame_1box gSpriteBank27Frame028;
extern const struct sprite_frame_1box gSpriteBank27Frame029;
extern const struct sprite_piece_pos gSpriteBank27Frame000Pos[4];
extern const struct sprite_piece_pos gSpriteBank27Frame001Pos[4];
extern const struct sprite_piece_pos gSpriteBank27Frame002Pos[4];
extern const struct sprite_piece_pos gSpriteBank27Frame003Pos[4];
extern const struct sprite_piece_pos gSpriteBank27Frame004Pos[4];
extern const struct sprite_piece_pos gSpriteBank27Frame005Pos[4];
extern const struct sprite_piece_pos gSpriteBank27Frame006Pos[4];
extern const struct sprite_piece_pos gSpriteBank27Frame007Pos[3];
extern const struct sprite_piece_pos gSpriteBank27Frame008Pos[2];
extern const struct sprite_piece_pos gSpriteBank27Frame009Pos[2];
extern const struct sprite_piece_pos gSpriteBank27Frame010Pos[3];
extern const struct sprite_piece_pos gSpriteBank27Frame011Pos[2];
extern const struct sprite_piece_pos gSpriteBank27Frame012Pos[1];
extern const struct sprite_piece_pos gSpriteBank27Frame013Pos[1];
extern const struct sprite_piece_pos gSpriteBank27Frame014Pos[1];
extern const struct sprite_piece_pos gSpriteBank27Frame015Pos[1];
extern const struct sprite_piece_pos gSpriteBank27Frame016Pos[1];
extern const struct sprite_piece_pos gSpriteBank27Frame017Pos[1];
extern const struct sprite_piece_pos gSpriteBank27Frame018Pos[2];
extern const struct sprite_piece_pos gSpriteBank27Frame019Pos[3];
extern const struct sprite_piece_pos gSpriteBank27Frame020Pos[2];
extern const struct sprite_piece_pos gSpriteBank27Frame021Pos[2];
extern const struct sprite_piece_pos gSpriteBank27Frame022Pos[3];
extern const struct sprite_piece_pos gSpriteBank27Frame023Pos[4];
extern const struct sprite_piece_pos gSpriteBank27Frame024Pos[4];
extern const struct sprite_piece_pos gSpriteBank27Frame025Pos[4];
extern const struct sprite_piece_pos gSpriteBank27Frame026Pos[4];
extern const struct sprite_piece_pos gSpriteBank27Frame027Pos[4];
extern const struct sprite_piece_pos gSpriteBank27Frame028Pos[4];
extern const struct sprite_piece_pos gSpriteBank27Frame029Pos[4];
extern const u8 gSpriteBank27Frame000Pieces[4];
extern const u8 gSpriteBank27Frame001Pieces[4];
extern const u8 gSpriteBank27Frame002Pieces[4];
extern const u8 gSpriteBank27Frame003Pieces[4];
extern const u8 gSpriteBank27Frame004Pieces[4];
extern const u8 gSpriteBank27Frame005Pieces[4];
extern const u8 gSpriteBank27Frame006Pieces[4];
extern const u8 gSpriteBank27Frame007Pieces[3];
extern const u8 gSpriteBank27Frame008Pieces[2];
extern const u8 gSpriteBank27Frame009Pieces[2];
extern const u8 gSpriteBank27Frame010Pieces[3];
extern const u8 gSpriteBank27Frame011Pieces[2];
extern const u8 gSpriteBank27Frame012Pieces[1];
extern const u8 gSpriteBank27Frame013Pieces[1];
extern const u8 gSpriteBank27Frame014Pieces[1];
extern const u8 gSpriteBank27Frame015Pieces[1];
extern const u8 gSpriteBank27Frame016Pieces[1];
extern const u8 gSpriteBank27Frame017Pieces[1];
extern const u8 gSpriteBank27Frame018Pieces[2];
extern const u8 gSpriteBank27Frame019Pieces[3];
extern const u8 gSpriteBank27Frame020Pieces[2];
extern const u8 gSpriteBank27Frame021Pieces[2];
extern const u8 gSpriteBank27Frame022Pieces[3];
extern const u8 gSpriteBank27Frame023Pieces[4];
extern const u8 gSpriteBank27Frame024Pieces[4];
extern const u8 gSpriteBank27Frame025Pieces[4];
extern const u8 gSpriteBank27Frame026Pieces[4];
extern const u8 gSpriteBank27Frame027Pieces[4];
extern const u8 gSpriteBank27Frame028Pieces[4];
extern const u8 gSpriteBank27Frame029Pieces[4];

const struct sprite_anim gSpriteBank27Anims[2] = {
    [0] = {
        .seq = gSpriteBank27Anim00Seq,
        .box = { { -16, -11, 32, 23 }, { -29, -24, 59, 37 } },
        .paletteId = 37,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank27Anim00Seq),
        .flags = 0,
    },
    [1] = {
        .seq = gSpriteBank27Anim01Seq,
        .box = { { -16, -11, 32, 23 }, { -29, -24, 59, 37 } },
        .paletteId = 37,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank27Anim01Seq),
        .flags = 0,
    },
};

const u16 gSpriteBank27Anim00Seq[15] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
};
const u16 gSpriteBank27Anim01Seq[15] = {
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
};

const struct sprite_frame *const gSpriteBank27Frames[30] = {
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
    SPRITE_FRAME(gSpriteBank27Frame000, SPRITE_TILES_BANK27 + 0x00000),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank27Frame001 = {
    SPRITE_FRAME(gSpriteBank27Frame001, SPRITE_TILES_BANK27 + 0x001e0),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank27Frame002 = {
    SPRITE_FRAME(gSpriteBank27Frame002, SPRITE_TILES_BANK27 + 0x003c0),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank27Frame003 = {
    SPRITE_FRAME(gSpriteBank27Frame003, SPRITE_TILES_BANK27 + 0x005a0),
    { { -12, -8, 24, 17 } },
};
const struct sprite_frame_1box gSpriteBank27Frame004 = {
    SPRITE_FRAME(gSpriteBank27Frame004, SPRITE_TILES_BANK27 + 0x00780),
    { { -12, -7, 24, 16 } },
};
const struct sprite_frame_1box gSpriteBank27Frame005 = {
    SPRITE_FRAME(gSpriteBank27Frame005, SPRITE_TILES_BANK27 + 0x00940),
    { { -12, -2, 24, 11 } },
};
const struct sprite_frame_1box gSpriteBank27Frame006 = {
    SPRITE_FRAME(gSpriteBank27Frame006, SPRITE_TILES_BANK27 + 0x00ac0),
    { { -12, -2, 24, 11 } },
};
const struct sprite_frame_1box gSpriteBank27Frame007 = {
    SPRITE_FRAME(gSpriteBank27Frame007, SPRITE_TILES_BANK27 + 0x00c40),
    { { -13, -1, 26, 10 } },
};
const struct sprite_frame_1box gSpriteBank27Frame008 = {
    SPRITE_FRAME(gSpriteBank27Frame008, SPRITE_TILES_BANK27 + 0x00da0),
    { { -14, 1, 28, 9 } },
};
const struct sprite_frame_1box gSpriteBank27Frame009 = {
    SPRITE_FRAME(gSpriteBank27Frame009, SPRITE_TILES_BANK27 + 0x00ee0),
    { { -14, 1, 28, 9 } },
};
const struct sprite_frame_1box gSpriteBank27Frame010 = {
    SPRITE_FRAME(gSpriteBank27Frame010, SPRITE_TILES_BANK27 + 0x01020),
    { { -13, -2, 27, 11 } },
};
const struct sprite_frame_1box gSpriteBank27Frame011 = {
    SPRITE_FRAME(gSpriteBank27Frame011, SPRITE_TILES_BANK27 + 0x011e0),
    { { -18, -17, 41, 24 } },
};
const struct sprite_frame_1box gSpriteBank27Frame012 = {
    SPRITE_FRAME(gSpriteBank27Frame012, SPRITE_TILES_BANK27 + 0x01660),
    { { -21, -10, 47, 13 } },
};
const struct sprite_frame_1box gSpriteBank27Frame013 = {
    SPRITE_FRAME(gSpriteBank27Frame013, SPRITE_TILES_BANK27 + 0x01a60),
    { { -22, -13, 47, 16 } },
};
const struct sprite_frame_1box gSpriteBank27Frame014 = {
    SPRITE_FRAME(gSpriteBank27Frame014, SPRITE_TILES_BANK27 + 0x01e60),
    { { -21, -14, 46, 16 } },
};
const struct sprite_frame_1box gSpriteBank27Frame015 = {
    SPRITE_FRAME(gSpriteBank27Frame015, SPRITE_TILES_BANK27 + 0x01e60),
    { { -20, -11, 45, 12 } },
};
const struct sprite_frame_1box gSpriteBank27Frame016 = {
    SPRITE_FRAME(gSpriteBank27Frame016, SPRITE_TILES_BANK27 + 0x01a60),
    { { -22, -12, 47, 13 } },
};
const struct sprite_frame_1box gSpriteBank27Frame017 = {
    SPRITE_FRAME(gSpriteBank27Frame017, SPRITE_TILES_BANK27 + 0x01660),
    { { -21, -11, 47, 13 } },
};
const struct sprite_frame_1box gSpriteBank27Frame018 = {
    SPRITE_FRAME(gSpriteBank27Frame018, SPRITE_TILES_BANK27 + 0x011e0),
    { { -23, -15, 49, 14 } },
};
const struct sprite_frame_1box gSpriteBank27Frame019 = {
    SPRITE_FRAME(gSpriteBank27Frame019, SPRITE_TILES_BANK27 + 0x01020),
    { { -14, -2, 29, 11 } },
};
const struct sprite_frame_1box gSpriteBank27Frame020 = {
    SPRITE_FRAME(gSpriteBank27Frame020, SPRITE_TILES_BANK27 + 0x00ee0),
    { { -15, 1, 30, 9 } },
};
const struct sprite_frame_1box gSpriteBank27Frame021 = {
    SPRITE_FRAME(gSpriteBank27Frame021, SPRITE_TILES_BANK27 + 0x00da0),
    { { -15, 1, 30, 9 } },
};
const struct sprite_frame_1box gSpriteBank27Frame022 = {
    SPRITE_FRAME(gSpriteBank27Frame022, SPRITE_TILES_BANK27 + 0x00c40),
    { { -14, -1, 28, 10 } },
};
const struct sprite_frame_1box gSpriteBank27Frame023 = {
    SPRITE_FRAME(gSpriteBank27Frame023, SPRITE_TILES_BANK27 + 0x00ac0),
    { { -13, -2, 26, 11 } },
};
const struct sprite_frame_1box gSpriteBank27Frame024 = {
    SPRITE_FRAME(gSpriteBank27Frame024, SPRITE_TILES_BANK27 + 0x00940),
    { { -13, -2, 26, 11 } },
};
const struct sprite_frame_1box gSpriteBank27Frame025 = {
    SPRITE_FRAME(gSpriteBank27Frame025, SPRITE_TILES_BANK27 + 0x00780),
    { { -13, -7, 26, 16 } },
};
const struct sprite_frame_1box gSpriteBank27Frame026 = {
    SPRITE_FRAME(gSpriteBank27Frame026, SPRITE_TILES_BANK27 + 0x005a0),
    { { -13, -8, 26, 17 } },
};
const struct sprite_frame_1box gSpriteBank27Frame027 = {
    SPRITE_FRAME(gSpriteBank27Frame027, SPRITE_TILES_BANK27 + 0x003c0),
    { { -13, -8, 26, 17 } },
};
const struct sprite_frame_1box gSpriteBank27Frame028 = {
    SPRITE_FRAME(gSpriteBank27Frame028, SPRITE_TILES_BANK27 + 0x001e0),
    { { -13, -8, 26, 17 } },
};
const struct sprite_frame_1box gSpriteBank27Frame029 = {
    SPRITE_FRAME(gSpriteBank27Frame029, SPRITE_TILES_BANK27 + 0x00000),
    { { -13, -8, 26, 17 } },
};

const struct sprite_piece_pos gSpriteBank27Frame000Pos[4] = { { -15, -11 }, { 16, -11 }, { -16, 5 }, { 16, 5 } };
const struct sprite_piece_pos gSpriteBank27Frame001Pos[4] = { { -14, -11 }, { 16, -11 }, { -16, 5 }, { 16, 5 } };
const struct sprite_piece_pos gSpriteBank27Frame002Pos[4] = { { -14, -11 }, { 16, -11 }, { -16, 5 }, { 16, 5 } };
const struct sprite_piece_pos gSpriteBank27Frame003Pos[4] = { { -14, -11 }, { 16, -11 }, { -16, 5 }, { 16, 5 } };
const struct sprite_piece_pos gSpriteBank27Frame004Pos[4] = { { -15, -10 }, { 16, 4 }, { -16, 6 }, { 16, 6 } };
const struct sprite_piece_pos gSpriteBank27Frame005Pos[4] = { { -16, -5 }, { 16, 4 }, { -9, 11 }, { 7, 11 } };
const struct sprite_piece_pos gSpriteBank27Frame006Pos[4] = { { -16, -5 }, { 16, 4 }, { -9, 11 }, { 7, 11 } };
const struct sprite_piece_pos gSpriteBank27Frame007Pos[3] = { { -17, -4 }, { 15, 3 }, { -6, 12 } };
const struct sprite_piece_pos gSpriteBank27Frame008Pos[2] = { { -18, -2 }, { 14, 2 } };
const struct sprite_piece_pos gSpriteBank27Frame009Pos[2] = { { -18, -2 }, { 14, 2 } };
const struct sprite_piece_pos gSpriteBank27Frame010Pos[3] = { { -17, -5 }, { 15, 1 }, { -12, 11 } };
const struct sprite_piece_pos gSpriteBank27Frame011Pos[2] = { { -29, -24 }, { -15, 8 } };
const struct sprite_piece_pos gSpriteBank27Frame012Pos[1] = { { -28, -18 } };
const struct sprite_piece_pos gSpriteBank27Frame013Pos[1] = { { -27, -18 } };
const struct sprite_piece_pos gSpriteBank27Frame014Pos[1] = { { -26, -19 } };
const struct sprite_piece_pos gSpriteBank27Frame015Pos[1] = { { -26, -19 } };
const struct sprite_piece_pos gSpriteBank27Frame016Pos[1] = { { -27, -18 } };
const struct sprite_piece_pos gSpriteBank27Frame017Pos[1] = { { -28, -18 } };
const struct sprite_piece_pos gSpriteBank27Frame018Pos[2] = { { -29, -24 }, { -15, 8 } };
const struct sprite_piece_pos gSpriteBank27Frame019Pos[3] = { { -17, -5 }, { 15, 1 }, { -12, 11 } };
const struct sprite_piece_pos gSpriteBank27Frame020Pos[2] = { { -18, -2 }, { 14, 2 } };
const struct sprite_piece_pos gSpriteBank27Frame021Pos[2] = { { -18, -2 }, { 14, 2 } };
const struct sprite_piece_pos gSpriteBank27Frame022Pos[3] = { { -17, -4 }, { 15, 3 }, { -6, 12 } };
const struct sprite_piece_pos gSpriteBank27Frame023Pos[4] = { { -16, -5 }, { 16, 4 }, { -9, 11 }, { 7, 11 } };
const struct sprite_piece_pos gSpriteBank27Frame024Pos[4] = { { -16, -5 }, { 16, 4 }, { -9, 11 }, { 7, 11 } };
const struct sprite_piece_pos gSpriteBank27Frame025Pos[4] = { { -15, -10 }, { 16, 4 }, { -16, 6 }, { 16, 6 } };
const struct sprite_piece_pos gSpriteBank27Frame026Pos[4] = { { -14, -11 }, { 16, -11 }, { -16, 5 }, { 16, 5 } };
const struct sprite_piece_pos gSpriteBank27Frame027Pos[4] = { { -14, -11 }, { 16, -11 }, { -16, 5 }, { 16, 5 } };
const struct sprite_piece_pos gSpriteBank27Frame028Pos[4] = { { -14, -11 }, { 16, -11 }, { -16, 5 }, { 16, 5 } };
const struct sprite_piece_pos gSpriteBank27Frame029Pos[4] = { { -15, -11 }, { 16, -11 }, { -16, 5 }, { 16, 5 } };

const u8 gSpriteBank27Frame000Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank27Frame001Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank27Frame002Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank27Frame003Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank27Frame004Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank27Frame005Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank27Frame006Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank27Frame007Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank27Frame008Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank27Frame009Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank27Frame010Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank27Frame011Pieces[2] = { SPRITE_PIECE(5, 7), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank27Frame012Pieces[1] = { SPRITE_PIECE(5, 7) };
const u8 gSpriteBank27Frame013Pieces[1] = { SPRITE_PIECE(5, 7) };
const u8 gSpriteBank27Frame014Pieces[1] = { SPRITE_PIECE(5, 7) };
const u8 gSpriteBank27Frame015Pieces[1] = { SPRITE_PIECE(5, 7) };
const u8 gSpriteBank27Frame016Pieces[1] = { SPRITE_PIECE(5, 7) };
const u8 gSpriteBank27Frame017Pieces[1] = { SPRITE_PIECE(5, 7) };
const u8 gSpriteBank27Frame018Pieces[2] = { SPRITE_PIECE(5, 7), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank27Frame019Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank27Frame020Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank27Frame021Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank27Frame022Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank27Frame023Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank27Frame024Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank27Frame025Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank27Frame026Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank27Frame027Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank27Frame028Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank27Frame029Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };

/* ---------------------------------------------------------------------- */
/* Bank 28: 1 animation, 3 frames, tiles in gSpriteBank28Tiles (SPRITE_TILES_BANK28). */

extern const u16 gSpriteBank28Anim00Seq[8];
extern const struct sprite_frame gSpriteBank28Frame000;
extern const struct sprite_frame gSpriteBank28Frame001;
extern const struct sprite_frame gSpriteBank28Frame002;
extern const struct sprite_piece_pos gSpriteBank28Frame000Pos[1];
extern const struct sprite_piece_pos gSpriteBank28Frame001Pos[1];
extern const struct sprite_piece_pos gSpriteBank28Frame002Pos[1];
extern const u8 gSpriteBank28Frame000Pieces[1];
extern const u8 gSpriteBank28Frame001Pieces[1];
extern const u8 gSpriteBank28Frame002Pieces[1];

const struct sprite_anim gSpriteBank28Anims[1] = {
    [0] = {
        .seq = gSpriteBank28Anim00Seq,
        .box = { { -14, -1, 29, 9 }, { -13, -7, 26, 15 } },
        .paletteId = 41,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank28Anim00Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank28Anim00Seq[8] = {
    0, 1, 2, 0, 1, 2, 0, 1,
};

const struct sprite_frame *const gSpriteBank28Frames[3] = {
    &gSpriteBank28Frame000,
    &gSpriteBank28Frame001,
    &gSpriteBank28Frame002,
};

const struct sprite_frame gSpriteBank28Frame000 = SPRITE_FRAME(gSpriteBank28Frame000, SPRITE_TILES_BANK28 + 0x00000);
const struct sprite_frame gSpriteBank28Frame001 = SPRITE_FRAME(gSpriteBank28Frame001, SPRITE_TILES_BANK28 + 0x00100);
const struct sprite_frame gSpriteBank28Frame002 = SPRITE_FRAME(gSpriteBank28Frame002, SPRITE_TILES_BANK28 + 0x00200);

const struct sprite_piece_pos gSpriteBank28Frame000Pos[1] = { { -13, -7 } };
const struct sprite_piece_pos gSpriteBank28Frame001Pos[1] = { { -13, -7 } };
const struct sprite_piece_pos gSpriteBank28Frame002Pos[1] = { { -13, -7 } };

const u8 gSpriteBank28Frame000Pieces[1] = { SPRITE_PIECE(1, 6) };
const u8 gSpriteBank28Frame001Pieces[1] = { SPRITE_PIECE(1, 6) };
const u8 gSpriteBank28Frame002Pieces[1] = { SPRITE_PIECE(1, 6) };

/* ---------------------------------------------------------------------- */
/* Bank 29: 4 animations, 42 frames, tiles in gSpriteBank29Tiles (SPRITE_TILES_BANK29). */

extern const u16 gSpriteBank29Anim00Seq[20];
extern const u16 gSpriteBank29Anim01Seq[6];
extern const u16 gSpriteBank29Anim02Seq[10];
extern const u16 gSpriteBank29Anim03Seq[6];
extern const struct sprite_frame_1box gSpriteBank29Frame000;
extern const struct sprite_frame_1box gSpriteBank29Frame001;
extern const struct sprite_frame_1box gSpriteBank29Frame002;
extern const struct sprite_frame_1box gSpriteBank29Frame003;
extern const struct sprite_frame_1box gSpriteBank29Frame004;
extern const struct sprite_frame_1box gSpriteBank29Frame005;
extern const struct sprite_frame_1box gSpriteBank29Frame006;
extern const struct sprite_frame_1box gSpriteBank29Frame007;
extern const struct sprite_frame_1box gSpriteBank29Frame008;
extern const struct sprite_frame_1box gSpriteBank29Frame009;
extern const struct sprite_frame_1box gSpriteBank29Frame010;
extern const struct sprite_frame_1box gSpriteBank29Frame011;
extern const struct sprite_frame_1box gSpriteBank29Frame012;
extern const struct sprite_frame_1box gSpriteBank29Frame013;
extern const struct sprite_frame_1box gSpriteBank29Frame014;
extern const struct sprite_frame_1box gSpriteBank29Frame015;
extern const struct sprite_frame_1box gSpriteBank29Frame016;
extern const struct sprite_frame_1box gSpriteBank29Frame017;
extern const struct sprite_frame_1box gSpriteBank29Frame018;
extern const struct sprite_frame_1box gSpriteBank29Frame019;
extern const struct sprite_frame_1box gSpriteBank29Frame020;
extern const struct sprite_frame_1box gSpriteBank29Frame021;
extern const struct sprite_frame_1box gSpriteBank29Frame022;
extern const struct sprite_frame_1box gSpriteBank29Frame023;
extern const struct sprite_frame_1box gSpriteBank29Frame024;
extern const struct sprite_frame_1box gSpriteBank29Frame025;
extern const struct sprite_frame_1box gSpriteBank29Frame026;
extern const struct sprite_frame_1box gSpriteBank29Frame027;
extern const struct sprite_frame_1box gSpriteBank29Frame028;
extern const struct sprite_frame_1box gSpriteBank29Frame029;
extern const struct sprite_frame_1box gSpriteBank29Frame030;
extern const struct sprite_frame_1box gSpriteBank29Frame031;
extern const struct sprite_frame_1box gSpriteBank29Frame032;
extern const struct sprite_frame_1box gSpriteBank29Frame033;
extern const struct sprite_frame_1box gSpriteBank29Frame034;
extern const struct sprite_frame_1box gSpriteBank29Frame035;
extern const struct sprite_frame_1box gSpriteBank29Frame036;
extern const struct sprite_frame_1box gSpriteBank29Frame037;
extern const struct sprite_frame_1box gSpriteBank29Frame038;
extern const struct sprite_frame_1box gSpriteBank29Frame039;
extern const struct sprite_frame_1box gSpriteBank29Frame040;
extern const struct sprite_frame_1box gSpriteBank29Frame041;
extern const struct sprite_piece_pos gSpriteBank29Frame000Pos[3];
extern const struct sprite_piece_pos gSpriteBank29Frame001Pos[4];
extern const struct sprite_piece_pos gSpriteBank29Frame002Pos[4];
extern const struct sprite_piece_pos gSpriteBank29Frame003Pos[4];
extern const struct sprite_piece_pos gSpriteBank29Frame004Pos[4];
extern const struct sprite_piece_pos gSpriteBank29Frame005Pos[6];
extern const struct sprite_piece_pos gSpriteBank29Frame006Pos[6];
extern const struct sprite_piece_pos gSpriteBank29Frame007Pos[6];
extern const struct sprite_piece_pos gSpriteBank29Frame008Pos[6];
extern const struct sprite_piece_pos gSpriteBank29Frame009Pos[6];
extern const struct sprite_piece_pos gSpriteBank29Frame010Pos[6];
extern const struct sprite_piece_pos gSpriteBank29Frame011Pos[5];
extern const struct sprite_piece_pos gSpriteBank29Frame012Pos[4];
extern const struct sprite_piece_pos gSpriteBank29Frame013Pos[3];
extern const struct sprite_piece_pos gSpriteBank29Frame014Pos[3];
extern const struct sprite_piece_pos gSpriteBank29Frame015Pos[3];
extern const struct sprite_piece_pos gSpriteBank29Frame016Pos[3];
extern const struct sprite_piece_pos gSpriteBank29Frame017Pos[3];
extern const struct sprite_piece_pos gSpriteBank29Frame018Pos[3];
extern const struct sprite_piece_pos gSpriteBank29Frame019Pos[3];
extern const struct sprite_piece_pos gSpriteBank29Frame020Pos[3];
extern const struct sprite_piece_pos gSpriteBank29Frame021Pos[1];
extern const struct sprite_piece_pos gSpriteBank29Frame022Pos[1];
extern const struct sprite_piece_pos gSpriteBank29Frame023Pos[1];
extern const struct sprite_piece_pos gSpriteBank29Frame024Pos[1];
extern const struct sprite_piece_pos gSpriteBank29Frame025Pos[4];
extern const struct sprite_piece_pos gSpriteBank29Frame026Pos[4];
extern const struct sprite_piece_pos gSpriteBank29Frame027Pos[4];
extern const struct sprite_piece_pos gSpriteBank29Frame028Pos[3];
extern const struct sprite_piece_pos gSpriteBank29Frame029Pos[3];
extern const struct sprite_piece_pos gSpriteBank29Frame030Pos[4];
extern const struct sprite_piece_pos gSpriteBank29Frame031Pos[3];
extern const struct sprite_piece_pos gSpriteBank29Frame032Pos[3];
extern const struct sprite_piece_pos gSpriteBank29Frame033Pos[4];
extern const struct sprite_piece_pos gSpriteBank29Frame034Pos[4];
extern const struct sprite_piece_pos gSpriteBank29Frame035Pos[3];
extern const struct sprite_piece_pos gSpriteBank29Frame036Pos[4];
extern const struct sprite_piece_pos gSpriteBank29Frame037Pos[1];
extern const struct sprite_piece_pos gSpriteBank29Frame038Pos[1];
extern const struct sprite_piece_pos gSpriteBank29Frame039Pos[1];
extern const struct sprite_piece_pos gSpriteBank29Frame040Pos[1];
extern const struct sprite_piece_pos gSpriteBank29Frame041Pos[3];
extern const u8 gSpriteBank29Frame000Pieces[3];
extern const u8 gSpriteBank29Frame001Pieces[4];
extern const u8 gSpriteBank29Frame002Pieces[4];
extern const u8 gSpriteBank29Frame003Pieces[4];
extern const u8 gSpriteBank29Frame004Pieces[4];
extern const u8 gSpriteBank29Frame005Pieces[6];
extern const u8 gSpriteBank29Frame006Pieces[6];
extern const u8 gSpriteBank29Frame007Pieces[6];
extern const u8 gSpriteBank29Frame008Pieces[6];
extern const u8 gSpriteBank29Frame009Pieces[6];
extern const u8 gSpriteBank29Frame010Pieces[6];
extern const u8 gSpriteBank29Frame011Pieces[5];
extern const u8 gSpriteBank29Frame012Pieces[4];
extern const u8 gSpriteBank29Frame013Pieces[3];
extern const u8 gSpriteBank29Frame014Pieces[3];
extern const u8 gSpriteBank29Frame015Pieces[3];
extern const u8 gSpriteBank29Frame016Pieces[3];
extern const u8 gSpriteBank29Frame017Pieces[3];
extern const u8 gSpriteBank29Frame018Pieces[3];
extern const u8 gSpriteBank29Frame019Pieces[3];
extern const u8 gSpriteBank29Frame020Pieces[3];
extern const u8 gSpriteBank29Frame021Pieces[1];
extern const u8 gSpriteBank29Frame022Pieces[1];
extern const u8 gSpriteBank29Frame023Pieces[1];
extern const u8 gSpriteBank29Frame024Pieces[1];
extern const u8 gSpriteBank29Frame025Pieces[4];
extern const u8 gSpriteBank29Frame026Pieces[4];
extern const u8 gSpriteBank29Frame027Pieces[4];
extern const u8 gSpriteBank29Frame028Pieces[3];
extern const u8 gSpriteBank29Frame029Pieces[3];
extern const u8 gSpriteBank29Frame030Pieces[4];
extern const u8 gSpriteBank29Frame031Pieces[3];
extern const u8 gSpriteBank29Frame032Pieces[3];
extern const u8 gSpriteBank29Frame033Pieces[4];
extern const u8 gSpriteBank29Frame034Pieces[4];
extern const u8 gSpriteBank29Frame035Pieces[3];
extern const u8 gSpriteBank29Frame036Pieces[4];
extern const u8 gSpriteBank29Frame037Pieces[1];
extern const u8 gSpriteBank29Frame038Pieces[1];
extern const u8 gSpriteBank29Frame039Pieces[1];
extern const u8 gSpriteBank29Frame040Pieces[1];
extern const u8 gSpriteBank29Frame041Pieces[3];

const struct sprite_anim gSpriteBank29Anims[4] = {
    [0] = {
        .seq = gSpriteBank29Anim00Seq,
        .box = { { -28, -22, 57, 44 }, { -30, -23, 61, 43 } },
        .paletteId = 106,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank29Anim00Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [1] = {
        .seq = gSpriteBank29Anim01Seq,
        .box = { { -28, -22, 57, 44 }, { -29, -22, 58, 49 } },
        .paletteId = 106,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank29Anim01Seq),
        .flags = 0,
    },
    [2] = {
        .seq = gSpriteBank29Anim02Seq,
        .box = { { -28, -22, 57, 44 }, { -30, -23, 59, 45 } },
        .paletteId = 106,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank29Anim02Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [3] = {
        .seq = gSpriteBank29Anim03Seq,
        .box = { { -28, -22, 57, 44 }, { -29, -22, 58, 49 } },
        .paletteId = 106,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank29Anim03Seq),
        .flags = 0,
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

const struct sprite_frame *const gSpriteBank29Frames[42] = {
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
    SPRITE_FRAME(gSpriteBank29Frame000, SPRITE_TILES_BANK29 + 0x00000),
    { { -16, -23, 47, 40 } },
};
const struct sprite_frame_1box gSpriteBank29Frame001 = {
    SPRITE_FRAME(gSpriteBank29Frame001, SPRITE_TILES_BANK29 + 0x00580),
    { { -11, -16, 36, 32 } },
};
const struct sprite_frame_1box gSpriteBank29Frame002 = {
    SPRITE_FRAME(gSpriteBank29Frame002, SPRITE_TILES_BANK29 + 0x00ae0),
    { { -12, -21, 38, 37 } },
};
const struct sprite_frame_1box gSpriteBank29Frame003 = {
    SPRITE_FRAME(gSpriteBank29Frame003, SPRITE_TILES_BANK29 + 0x01040),
    { { -10, -20, 33, 37 } },
};
const struct sprite_frame_1box gSpriteBank29Frame004 = {
    SPRITE_FRAME(gSpriteBank29Frame004, SPRITE_TILES_BANK29 + 0x015e0),
    { { -13, -19, 39, 33 } },
};
const struct sprite_frame_1box gSpriteBank29Frame005 = {
    SPRITE_FRAME(gSpriteBank29Frame005, SPRITE_TILES_BANK29 + 0x01b80),
    { { -12, -20, 32, 37 } },
};
const struct sprite_frame_1box gSpriteBank29Frame006 = {
    SPRITE_FRAME(gSpriteBank29Frame006, SPRITE_TILES_BANK29 + 0x01f80),
    { { -14, -20, 36, 40 } },
};
const struct sprite_frame_1box gSpriteBank29Frame007 = {
    SPRITE_FRAME(gSpriteBank29Frame007, SPRITE_TILES_BANK29 + 0x02320),
    { { -12, -20, 31, 38 } },
};
const struct sprite_frame_1box gSpriteBank29Frame008 = {
    SPRITE_FRAME(gSpriteBank29Frame008, SPRITE_TILES_BANK29 + 0x02720),
    { { -13, -20, 30, 37 } },
};
const struct sprite_frame_1box gSpriteBank29Frame009 = {
    SPRITE_FRAME(gSpriteBank29Frame009, SPRITE_TILES_BANK29 + 0x02b20),
    { { -12, -24, 29, 42 } },
};
const struct sprite_frame_1box gSpriteBank29Frame010 = {
    SPRITE_FRAME(gSpriteBank29Frame010, SPRITE_TILES_BANK29 + 0x02ee0),
    { { -12, -20, 31, 36 } },
};
const struct sprite_frame_1box gSpriteBank29Frame011 = {
    SPRITE_FRAME(gSpriteBank29Frame011, SPRITE_TILES_BANK29 + 0x03280),
    { { -13, -21, 32, 39 } },
};
const struct sprite_frame_1box gSpriteBank29Frame012 = {
    SPRITE_FRAME(gSpriteBank29Frame012, SPRITE_TILES_BANK29 + 0x03620),
    { { -23, -7, 38, 21 } },
};
const struct sprite_frame_1box gSpriteBank29Frame013 = {
    SPRITE_FRAME(gSpriteBank29Frame013, SPRITE_TILES_BANK29 + 0x03b20),
    { { -23, -10, 46, 23 } },
};
const struct sprite_frame_1box gSpriteBank29Frame014 = {
    SPRITE_FRAME(gSpriteBank29Frame014, SPRITE_TILES_BANK29 + 0x040e0),
    { { -26, -16, 53, 32 } },
};
const struct sprite_frame_1box gSpriteBank29Frame015 = {
    SPRITE_FRAME(gSpriteBank29Frame015, SPRITE_TILES_BANK29 + 0x04620),
    { { -24, -13, 50, 30 } },
};
const struct sprite_frame_1box gSpriteBank29Frame016 = {
    SPRITE_FRAME(gSpriteBank29Frame016, SPRITE_TILES_BANK29 + 0x04b60),
    { { -24, -10, 51, 24 } },
};
const struct sprite_frame_1box gSpriteBank29Frame017 = {
    SPRITE_FRAME(gSpriteBank29Frame017, SPRITE_TILES_BANK29 + 0x05060),
    { { -24, -9, 51, 26 } },
};
const struct sprite_frame_1box gSpriteBank29Frame018 = {
    SPRITE_FRAME(gSpriteBank29Frame018, SPRITE_TILES_BANK29 + 0x05620),
    { { -23, -10, 46, 28 } },
};
const struct sprite_frame_1box gSpriteBank29Frame019 = {
    SPRITE_FRAME(gSpriteBank29Frame019, SPRITE_TILES_BANK29 + 0x05be0),
    { { -25, -15, 54, 34 } },
};
const struct sprite_frame_1box gSpriteBank29Frame020 = {
    SPRITE_FRAME(gSpriteBank29Frame020, SPRITE_TILES_BANK29 + 0x061a0),
    { { -20, -8, 45, 22 } },
};
const struct sprite_frame_1box gSpriteBank29Frame021 = {
    SPRITE_FRAME(gSpriteBank29Frame021, SPRITE_TILES_BANK29 + 0x066c0),
    { { -20, -8, 45, 22 } },
};
const struct sprite_frame_1box gSpriteBank29Frame022 = {
    SPRITE_FRAME(gSpriteBank29Frame022, SPRITE_TILES_BANK29 + 0x06e80),
    { { -20, -8, 45, 22 } },
};
const struct sprite_frame_1box gSpriteBank29Frame023 = {
    SPRITE_FRAME(gSpriteBank29Frame023, SPRITE_TILES_BANK29 + 0x07640),
    { { -20, -8, 45, 22 } },
};
const struct sprite_frame_1box gSpriteBank29Frame024 = {
    SPRITE_FRAME(gSpriteBank29Frame024, SPRITE_TILES_BANK29 + 0x07e00),
    { { -20, -8, 45, 22 } },
};
const struct sprite_frame_1box gSpriteBank29Frame025 = {
    SPRITE_FRAME(gSpriteBank29Frame025, SPRITE_TILES_BANK29 + 0x085c0),
    { { -20, -8, 45, 22 } },
};
const struct sprite_frame_1box gSpriteBank29Frame026 = {
    SPRITE_FRAME(gSpriteBank29Frame026, SPRITE_TILES_BANK29 + 0x08b00),
    { { -20, -8, 44, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame027 = {
    SPRITE_FRAME(gSpriteBank29Frame027, SPRITE_TILES_BANK29 + 0x09040),
    { { -20, -8, 44, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame028 = {
    SPRITE_FRAME(gSpriteBank29Frame028, SPRITE_TILES_BANK29 + 0x09580),
    { { -20, -8, 44, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame029 = {
    SPRITE_FRAME(gSpriteBank29Frame029, SPRITE_TILES_BANK29 + 0x09b00),
    { { -20, -8, 44, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame030 = {
    SPRITE_FRAME(gSpriteBank29Frame030, SPRITE_TILES_BANK29 + 0x0a040),
    { { -20, -8, 44, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame031 = {
    SPRITE_FRAME(gSpriteBank29Frame031, SPRITE_TILES_BANK29 + 0x0a580),
    { { -20, -8, 44, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame032 = {
    SPRITE_FRAME(gSpriteBank29Frame032, SPRITE_TILES_BANK29 + 0x0aaa0),
    { { -20, -8, 44, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame033 = {
    SPRITE_FRAME(gSpriteBank29Frame033, SPRITE_TILES_BANK29 + 0x0afc0),
    { { -20, -8, 44, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame034 = {
    SPRITE_FRAME(gSpriteBank29Frame034, SPRITE_TILES_BANK29 + 0x0b500),
    { { -20, -8, 44, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame035 = {
    SPRITE_FRAME(gSpriteBank29Frame035, SPRITE_TILES_BANK29 + 0x0ba40),
    { { -20, -8, 44, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame036 = {
    SPRITE_FRAME(gSpriteBank29Frame036, SPRITE_TILES_BANK29 + 0x085c0),
    { { -18, -6, 42, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame037 = {
    SPRITE_FRAME(gSpriteBank29Frame037, SPRITE_TILES_BANK29 + 0x07e00),
    { { -18, -6, 42, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame038 = {
    SPRITE_FRAME(gSpriteBank29Frame038, SPRITE_TILES_BANK29 + 0x07640),
    { { -18, -6, 42, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame039 = {
    SPRITE_FRAME(gSpriteBank29Frame039, SPRITE_TILES_BANK29 + 0x06e80),
    { { -18, -6, 42, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame040 = {
    SPRITE_FRAME(gSpriteBank29Frame040, SPRITE_TILES_BANK29 + 0x066c0),
    { { -18, -6, 42, 19 } },
};
const struct sprite_frame_1box gSpriteBank29Frame041 = {
    SPRITE_FRAME(gSpriteBank29Frame041, SPRITE_TILES_BANK29 + 0x061a0),
    { { -18, -6, 42, 19 } },
};

const struct sprite_piece_pos gSpriteBank29Frame000Pos[3] = { { -12, -23 }, { -16, 9 }, { 20, 9 } };
const struct sprite_piece_pos gSpriteBank29Frame001Pos[4] = { { -8, -23 }, { -17, 9 }, { 15, 9 }, { 31, 13 } };
const struct sprite_piece_pos gSpriteBank29Frame002Pos[4] = { { -16, -23 }, { -17, 9 }, { 15, 9 }, { 31, 13 } };
const struct sprite_piece_pos gSpriteBank29Frame003Pos[4] = { { -8, -23 }, { -17, 9 }, { 15, 9 }, { 31, 13 } };
const struct sprite_piece_pos gSpriteBank29Frame004Pos[4] = { { -12, -23 }, { -17, 9 }, { 15, 9 }, { 31, 12 } };
const struct sprite_piece_pos gSpriteBank29Frame005Pos[6] = { { -16, -23 }, { 14, -16 }, { 22, 7 }, { -18, 9 }, { 14, 9 }, { 22, 9 } };
const struct sprite_piece_pos gSpriteBank29Frame006Pos[6] = { { -17, -23 }, { 13, 6 }, { 21, 7 }, { -19, 9 }, { 13, 9 }, { 21, 9 } };
const struct sprite_piece_pos gSpriteBank29Frame007Pos[6] = { { -17, -23 }, { 13, -12 }, { 21, 7 }, { -19, 9 }, { 13, 9 }, { 21, 9 } };
const struct sprite_piece_pos gSpriteBank29Frame008Pos[6] = { { -17, -23 }, { 13, -12 }, { 21, 7 }, { -19, 9 }, { 13, 9 }, { 21, 9 } };
const struct sprite_piece_pos gSpriteBank29Frame009Pos[6] = { { -16, -23 }, { 13, -5 }, { 21, 7 }, { -19, 9 }, { 13, 9 }, { 21, 13 } };
const struct sprite_piece_pos gSpriteBank29Frame010Pos[6] = { { -21, -23 }, { 11, 5 }, { 19, 6 }, { -18, 9 }, { 14, 9 }, { 22, 15 } };
const struct sprite_piece_pos gSpriteBank29Frame011Pos[5] = { { -18, -23 }, { 14, -2 }, { -17, 9 }, { 15, 9 }, { 23, 16 } };
const struct sprite_piece_pos gSpriteBank29Frame012Pos[4] = { { -27, -23 }, { -29, 9 }, { 10, 9 }, { 19, 9 } };
const struct sprite_piece_pos gSpriteBank29Frame013Pos[3] = { { -28, -23 }, { -29, 9 }, { 3, 9 } };
const struct sprite_piece_pos gSpriteBank29Frame014Pos[3] = { { -28, -23 }, { -30, 9 }, { 2, 9 } };
const struct sprite_piece_pos gSpriteBank29Frame015Pos[3] = { { -28, -23 }, { -29, 9 }, { 3, 9 } };
const struct sprite_piece_pos gSpriteBank29Frame016Pos[3] = { { -27, -23 }, { -29, 9 }, { 3, 9 } };
const struct sprite_piece_pos gSpriteBank29Frame017Pos[3] = { { -26, -23 }, { -28, 9 }, { 4, 9 } };
const struct sprite_piece_pos gSpriteBank29Frame018Pos[3] = { { -27, -23 }, { -27, 9 }, { 5, 9 } };
const struct sprite_piece_pos gSpriteBank29Frame019Pos[3] = { { -28, -23 }, { -29, 9 }, { 3, 9 } };
const struct sprite_piece_pos gSpriteBank29Frame020Pos[3] = { { -29, -22 }, { -16, 10 }, { 16, 10 } };
const struct sprite_piece_pos gSpriteBank29Frame021Pos[1] = { { -29, -22 } };
const struct sprite_piece_pos gSpriteBank29Frame022Pos[1] = { { -29, -22 } };
const struct sprite_piece_pos gSpriteBank29Frame023Pos[1] = { { -28, -22 } };
const struct sprite_piece_pos gSpriteBank29Frame024Pos[1] = { { -28, -22 } };
const struct sprite_piece_pos gSpriteBank29Frame025Pos[4] = { { -28, -22 }, { -17, 10 }, { 15, 10 }, { 23, 10 } };
const struct sprite_piece_pos gSpriteBank29Frame026Pos[4] = { { -28, -22 }, { -17, 10 }, { 15, 10 }, { 23, 10 } };
const struct sprite_piece_pos gSpriteBank29Frame027Pos[4] = { { -29, -22 }, { -18, 10 }, { 14, 10 }, { 22, 10 } };
const struct sprite_piece_pos gSpriteBank29Frame028Pos[3] = { { -30, -23 }, { -19, 9 }, { 13, 9 } };
const struct sprite_piece_pos gSpriteBank29Frame029Pos[3] = { { -30, -23 }, { -19, 9 }, { 13, 9 } };
const struct sprite_piece_pos gSpriteBank29Frame030Pos[4] = { { -30, -22 }, { -17, 10 }, { 15, 10 }, { 23, 10 } };
const struct sprite_piece_pos gSpriteBank29Frame031Pos[3] = { { -29, -22 }, { -16, 10 }, { 16, 10 } };
const struct sprite_piece_pos gSpriteBank29Frame032Pos[3] = { { -28, -22 }, { -16, 10 }, { 16, 10 } };
const struct sprite_piece_pos gSpriteBank29Frame033Pos[4] = { { -27, -23 }, { -16, 9 }, { 16, 9 }, { 24, 9 } };
const struct sprite_piece_pos gSpriteBank29Frame034Pos[4] = { { -27, -23 }, { -16, 9 }, { 16, 9 }, { 24, 9 } };
const struct sprite_piece_pos gSpriteBank29Frame035Pos[3] = { { -27, -22 }, { -16, 10 }, { 16, 10 } };
const struct sprite_piece_pos gSpriteBank29Frame036Pos[4] = { { -28, -22 }, { -17, 10 }, { 15, 10 }, { 23, 10 } };
const struct sprite_piece_pos gSpriteBank29Frame037Pos[1] = { { -28, -22 } };
const struct sprite_piece_pos gSpriteBank29Frame038Pos[1] = { { -28, -22 } };
const struct sprite_piece_pos gSpriteBank29Frame039Pos[1] = { { -29, -22 } };
const struct sprite_piece_pos gSpriteBank29Frame040Pos[1] = { { -29, -22 } };
const struct sprite_piece_pos gSpriteBank29Frame041Pos[3] = { { -29, -22 }, { -16, 10 }, { 16, 10 } };

const u8 gSpriteBank29Frame000Pieces[3] = { SPRITE_PIECE(5, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank29Frame001Pieces[4] = { SPRITE_PIECE(5, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank29Frame002Pieces[4] = { SPRITE_PIECE(5, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank29Frame003Pieces[4] = { SPRITE_PIECE(5, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank29Frame004Pieces[4] = { SPRITE_PIECE(5, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank29Frame005Pieces[6] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank29Frame006Pieces[6] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank29Frame007Pieces[6] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank29Frame008Pieces[6] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank29Frame009Pieces[6] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank29Frame010Pieces[6] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank29Frame011Pieces[5] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank29Frame012Pieces[4] = { SPRITE_PIECE(5, 7), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank29Frame013Pieces[3] = { SPRITE_PIECE(5, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank29Frame014Pieces[3] = { SPRITE_PIECE(5, 7), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank29Frame015Pieces[3] = { SPRITE_PIECE(5, 7), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank29Frame016Pieces[3] = { SPRITE_PIECE(5, 7), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank29Frame017Pieces[3] = { SPRITE_PIECE(5, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank29Frame018Pieces[3] = { SPRITE_PIECE(5, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank29Frame019Pieces[3] = { SPRITE_PIECE(5, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank29Frame020Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank29Frame021Pieces[1] = { SPRITE_PIECE(2, 3) };
const u8 gSpriteBank29Frame022Pieces[1] = { SPRITE_PIECE(2, 3) };
const u8 gSpriteBank29Frame023Pieces[1] = { SPRITE_PIECE(2, 3) };
const u8 gSpriteBank29Frame024Pieces[1] = { SPRITE_PIECE(2, 3) };
const u8 gSpriteBank29Frame025Pieces[4] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank29Frame026Pieces[4] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank29Frame027Pieces[4] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank29Frame028Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank29Frame029Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank29Frame030Pieces[4] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank29Frame031Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank29Frame032Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank29Frame033Pieces[4] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank29Frame034Pieces[4] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank29Frame035Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank29Frame036Pieces[4] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank29Frame037Pieces[1] = { SPRITE_PIECE(2, 3) };
const u8 gSpriteBank29Frame038Pieces[1] = { SPRITE_PIECE(2, 3) };
const u8 gSpriteBank29Frame039Pieces[1] = { SPRITE_PIECE(2, 3) };
const u8 gSpriteBank29Frame040Pieces[1] = { SPRITE_PIECE(2, 3) };
const u8 gSpriteBank29Frame041Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };

/* ---------------------------------------------------------------------- */
/* Bank 30: 3 animations, 42 frames, tiles in gSpriteBank30Tiles (SPRITE_TILES_BANK30). */

extern const u16 gSpriteBank30Anim00Seq[20];
extern const u16 gSpriteBank30Anim01Seq[14];
extern const u16 gSpriteBank30Anim02Seq[8];
extern const struct sprite_frame_1box gSpriteBank30Frame000;
extern const struct sprite_frame_1box gSpriteBank30Frame001;
extern const struct sprite_frame_1box gSpriteBank30Frame002;
extern const struct sprite_frame_1box gSpriteBank30Frame003;
extern const struct sprite_frame_1box gSpriteBank30Frame004;
extern const struct sprite_frame_1box gSpriteBank30Frame005;
extern const struct sprite_frame_1box gSpriteBank30Frame006;
extern const struct sprite_frame_1box gSpriteBank30Frame007;
extern const struct sprite_frame_1box gSpriteBank30Frame008;
extern const struct sprite_frame_1box gSpriteBank30Frame009;
extern const struct sprite_frame_1box gSpriteBank30Frame010;
extern const struct sprite_frame_1box gSpriteBank30Frame011;
extern const struct sprite_frame_1box gSpriteBank30Frame012;
extern const struct sprite_frame_1box gSpriteBank30Frame013;
extern const struct sprite_frame_1box gSpriteBank30Frame014;
extern const struct sprite_frame_1box gSpriteBank30Frame015;
extern const struct sprite_frame_1box gSpriteBank30Frame016;
extern const struct sprite_frame_1box gSpriteBank30Frame017;
extern const struct sprite_frame_1box gSpriteBank30Frame018;
extern const struct sprite_frame_1box gSpriteBank30Frame019;
extern const struct sprite_frame_1box gSpriteBank30Frame020;
extern const struct sprite_frame_1box gSpriteBank30Frame021;
extern const struct sprite_frame_1box gSpriteBank30Frame022;
extern const struct sprite_frame_1box gSpriteBank30Frame023;
extern const struct sprite_frame_1box gSpriteBank30Frame024;
extern const struct sprite_frame_1box gSpriteBank30Frame025;
extern const struct sprite_frame_1box gSpriteBank30Frame026;
extern const struct sprite_frame_1box gSpriteBank30Frame027;
extern const struct sprite_frame_1box gSpriteBank30Frame028;
extern const struct sprite_frame_1box gSpriteBank30Frame029;
extern const struct sprite_frame_1box gSpriteBank30Frame030;
extern const struct sprite_frame_1box gSpriteBank30Frame031;
extern const struct sprite_frame_1box gSpriteBank30Frame032;
extern const struct sprite_frame_1box gSpriteBank30Frame033;
extern const struct sprite_frame_1box gSpriteBank30Frame034;
extern const struct sprite_frame_1box gSpriteBank30Frame035;
extern const struct sprite_frame_1box gSpriteBank30Frame036;
extern const struct sprite_frame_1box gSpriteBank30Frame037;
extern const struct sprite_frame_1box gSpriteBank30Frame038;
extern const struct sprite_frame_1box gSpriteBank30Frame039;
extern const struct sprite_frame_1box gSpriteBank30Frame040;
extern const struct sprite_frame_1box gSpriteBank30Frame041;
extern const struct sprite_piece_pos gSpriteBank30Frame000Pos[3];
extern const struct sprite_piece_pos gSpriteBank30Frame001Pos[3];
extern const struct sprite_piece_pos gSpriteBank30Frame002Pos[4];
extern const struct sprite_piece_pos gSpriteBank30Frame003Pos[2];
extern const struct sprite_piece_pos gSpriteBank30Frame004Pos[2];
extern const struct sprite_piece_pos gSpriteBank30Frame005Pos[3];
extern const struct sprite_piece_pos gSpriteBank30Frame006Pos[3];
extern const struct sprite_piece_pos gSpriteBank30Frame007Pos[4];
extern const struct sprite_piece_pos gSpriteBank30Frame008Pos[4];
extern const struct sprite_piece_pos gSpriteBank30Frame009Pos[4];
extern const struct sprite_piece_pos gSpriteBank30Frame010Pos[3];
extern const struct sprite_piece_pos gSpriteBank30Frame011Pos[3];
extern const struct sprite_piece_pos gSpriteBank30Frame012Pos[3];
extern const struct sprite_piece_pos gSpriteBank30Frame013Pos[3];
extern const struct sprite_piece_pos gSpriteBank30Frame014Pos[3];
extern const struct sprite_piece_pos gSpriteBank30Frame015Pos[4];
extern const struct sprite_piece_pos gSpriteBank30Frame016Pos[4];
extern const struct sprite_piece_pos gSpriteBank30Frame017Pos[4];
extern const struct sprite_piece_pos gSpriteBank30Frame018Pos[3];
extern const struct sprite_piece_pos gSpriteBank30Frame019Pos[3];
extern const struct sprite_piece_pos gSpriteBank30Frame020Pos[2];
extern const struct sprite_piece_pos gSpriteBank30Frame021Pos[3];
extern const struct sprite_piece_pos gSpriteBank30Frame022Pos[4];
extern const struct sprite_piece_pos gSpriteBank30Frame023Pos[5];
extern const struct sprite_piece_pos gSpriteBank30Frame024Pos[4];
extern const struct sprite_piece_pos gSpriteBank30Frame025Pos[5];
extern const struct sprite_piece_pos gSpriteBank30Frame026Pos[7];
extern const struct sprite_piece_pos gSpriteBank30Frame027Pos[4];
extern const struct sprite_piece_pos gSpriteBank30Frame028Pos[5];
extern const struct sprite_piece_pos gSpriteBank30Frame029Pos[6];
extern const struct sprite_piece_pos gSpriteBank30Frame030Pos[4];
extern const struct sprite_piece_pos gSpriteBank30Frame031Pos[2];
extern const struct sprite_piece_pos gSpriteBank30Frame032Pos[3];
extern const struct sprite_piece_pos gSpriteBank30Frame033Pos[4];
extern const struct sprite_piece_pos gSpriteBank30Frame034Pos[4];
extern const struct sprite_piece_pos gSpriteBank30Frame035Pos[4];
extern const struct sprite_piece_pos gSpriteBank30Frame036Pos[4];
extern const struct sprite_piece_pos gSpriteBank30Frame037Pos[4];
extern const struct sprite_piece_pos gSpriteBank30Frame038Pos[4];
extern const struct sprite_piece_pos gSpriteBank30Frame039Pos[4];
extern const struct sprite_piece_pos gSpriteBank30Frame040Pos[4];
extern const struct sprite_piece_pos gSpriteBank30Frame041Pos[4];
extern const u8 gSpriteBank30Frame000Pieces[3];
extern const u8 gSpriteBank30Frame001Pieces[3];
extern const u8 gSpriteBank30Frame002Pieces[4];
extern const u8 gSpriteBank30Frame003Pieces[2];
extern const u8 gSpriteBank30Frame004Pieces[2];
extern const u8 gSpriteBank30Frame005Pieces[3];
extern const u8 gSpriteBank30Frame006Pieces[3];
extern const u8 gSpriteBank30Frame007Pieces[4];
extern const u8 gSpriteBank30Frame008Pieces[4];
extern const u8 gSpriteBank30Frame009Pieces[4];
extern const u8 gSpriteBank30Frame010Pieces[3];
extern const u8 gSpriteBank30Frame011Pieces[3];
extern const u8 gSpriteBank30Frame012Pieces[3];
extern const u8 gSpriteBank30Frame013Pieces[3];
extern const u8 gSpriteBank30Frame014Pieces[3];
extern const u8 gSpriteBank30Frame015Pieces[4];
extern const u8 gSpriteBank30Frame016Pieces[4];
extern const u8 gSpriteBank30Frame017Pieces[4];
extern const u8 gSpriteBank30Frame018Pieces[3];
extern const u8 gSpriteBank30Frame019Pieces[3];
extern const u8 gSpriteBank30Frame020Pieces[2];
extern const u8 gSpriteBank30Frame021Pieces[3];
extern const u8 gSpriteBank30Frame022Pieces[4];
extern const u8 gSpriteBank30Frame023Pieces[5];
extern const u8 gSpriteBank30Frame024Pieces[4];
extern const u8 gSpriteBank30Frame025Pieces[5];
extern const u8 gSpriteBank30Frame026Pieces[7];
extern const u8 gSpriteBank30Frame027Pieces[4];
extern const u8 gSpriteBank30Frame028Pieces[5];
extern const u8 gSpriteBank30Frame029Pieces[6];
extern const u8 gSpriteBank30Frame030Pieces[4];
extern const u8 gSpriteBank30Frame031Pieces[2];
extern const u8 gSpriteBank30Frame032Pieces[3];
extern const u8 gSpriteBank30Frame033Pieces[4];
extern const u8 gSpriteBank30Frame034Pieces[4];
extern const u8 gSpriteBank30Frame035Pieces[4];
extern const u8 gSpriteBank30Frame036Pieces[4];
extern const u8 gSpriteBank30Frame037Pieces[4];
extern const u8 gSpriteBank30Frame038Pieces[4];
extern const u8 gSpriteBank30Frame039Pieces[4];
extern const u8 gSpriteBank30Frame040Pieces[4];
extern const u8 gSpriteBank30Frame041Pieces[4];

const struct sprite_anim gSpriteBank30Anims[3] = {
    [0] = {
        .seq = gSpriteBank30Anim00Seq,
        .box = { { -32, -32, 64, 64 }, { -45, -50, 85, 85 } },
        .paletteId = 44,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank30Anim00Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [1] = {
        .seq = gSpriteBank30Anim01Seq,
        .box = { { -32, -32, 64, 64 }, { -46, -65, 95, 98 } },
        .paletteId = 44,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank30Anim01Seq),
        .flags = 0,
    },
    [2] = {
        .seq = gSpriteBank30Anim02Seq,
        .box = { { -32, -32, 64, 64 }, { -40, -44, 60, 77 } },
        .paletteId = 44,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank30Anim02Seq),
        .flags = SPRITE_ANIM_LOOP,
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

const struct sprite_frame *const gSpriteBank30Frames[42] = {
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
    SPRITE_FRAME(gSpriteBank30Frame000, SPRITE_TILES_BANK30 + 0x00000),
    { { -32, -32, 64, 64 } },
};
const struct sprite_frame_1box gSpriteBank30Frame001 = {
    SPRITE_FRAME(gSpriteBank30Frame001, SPRITE_TILES_BANK30 + 0x00840),
    { { -34, -35, 68, 65 } },
};
const struct sprite_frame_1box gSpriteBank30Frame002 = {
    SPRITE_FRAME(gSpriteBank30Frame002, SPRITE_TILES_BANK30 + 0x010a0),
    { { -37, -40, 71, 64 } },
};
const struct sprite_frame_1box gSpriteBank30Frame003 = {
    SPRITE_FRAME(gSpriteBank30Frame003, SPRITE_TILES_BANK30 + 0x01920),
    { { -40, -46, 74, 63 } },
};
const struct sprite_frame_1box gSpriteBank30Frame004 = {
    SPRITE_FRAME(gSpriteBank30Frame004, SPRITE_TILES_BANK30 + 0x02220),
    { { -42, -50, 74, 61 } },
};
const struct sprite_frame_1box gSpriteBank30Frame005 = {
    SPRITE_FRAME(gSpriteBank30Frame005, SPRITE_TILES_BANK30 + 0x02da0),
    { { -44, -50, 74, 64 } },
};
const struct sprite_frame_1box gSpriteBank30Frame006 = {
    SPRITE_FRAME(gSpriteBank30Frame006, SPRITE_TILES_BANK30 + 0x039c0),
    { { -45, -46, 75, 67 } },
};
const struct sprite_frame_1box gSpriteBank30Frame007 = {
    SPRITE_FRAME(gSpriteBank30Frame007, SPRITE_TILES_BANK30 + 0x045e0),
    { { -45, -41, 76, 67 } },
};
const struct sprite_frame_1box gSpriteBank30Frame008 = {
    SPRITE_FRAME(gSpriteBank30Frame008, SPRITE_TILES_BANK30 + 0x04f20),
    { { -45, -37, 78, 66 } },
};
const struct sprite_frame_1box gSpriteBank30Frame009 = {
    SPRITE_FRAME(gSpriteBank30Frame009, SPRITE_TILES_BANK30 + 0x05860),
    { { -43, -33, 77, 66 } },
};
const struct sprite_frame_1box gSpriteBank30Frame010 = {
    SPRITE_FRAME(gSpriteBank30Frame010, SPRITE_TILES_BANK30 + 0x06200),
    { { -39, -32, 73, 63 } },
};
const struct sprite_frame_1box gSpriteBank30Frame011 = {
    SPRITE_FRAME(gSpriteBank30Frame011, SPRITE_TILES_BANK30 + 0x06b20),
    { { -40, -35, 74, 63 } },
};
const struct sprite_frame_1box gSpriteBank30Frame012 = {
    SPRITE_FRAME(gSpriteBank30Frame012, SPRITE_TILES_BANK30 + 0x07460),
    { { -39, -40, 76, 63 } },
};
const struct sprite_frame_1box gSpriteBank30Frame013 = {
    SPRITE_FRAME(gSpriteBank30Frame013, SPRITE_TILES_BANK30 + 0x07d80),
    { { -36, -46, 75, 62 } },
};
const struct sprite_frame_1box gSpriteBank30Frame014 = {
    SPRITE_FRAME(gSpriteBank30Frame014, SPRITE_TILES_BANK30 + 0x086a0),
    { { -34, -50, 73, 63 } },
};
const struct sprite_frame_1box gSpriteBank30Frame015 = {
    SPRITE_FRAME(gSpriteBank30Frame015, SPRITE_TILES_BANK30 + 0x08fc0),
    { { -36, -49, 76, 65 } },
};
const struct sprite_frame_1box gSpriteBank30Frame016 = {
    SPRITE_FRAME(gSpriteBank30Frame016, SPRITE_TILES_BANK30 + 0x098e0),
    { { -38, -46, 77, 68 } },
};
const struct sprite_frame_1box gSpriteBank30Frame017 = {
    SPRITE_FRAME(gSpriteBank30Frame017, SPRITE_TILES_BANK30 + 0x0a1e0),
    { { -39, -42, 73, 70 } },
};
const struct sprite_frame_1box gSpriteBank30Frame018 = {
    SPRITE_FRAME(gSpriteBank30Frame018, SPRITE_TILES_BANK30 + 0x0ae40),
    { { -33, -38, 63, 68 } },
};
const struct sprite_frame_1box gSpriteBank30Frame019 = {
    SPRITE_FRAME(gSpriteBank30Frame019, SPRITE_TILES_BANK30 + 0x0b6a0),
    { { -28, -34, 59, 69 } },
};
const struct sprite_frame_1box gSpriteBank30Frame020 = {
    SPRITE_FRAME(gSpriteBank30Frame020, SPRITE_TILES_BANK30 + 0x0bf00),
    { { -36, -36, 64, 62 } },
};
const struct sprite_frame_1box gSpriteBank30Frame021 = {
    SPRITE_FRAME(gSpriteBank30Frame021, SPRITE_TILES_BANK30 + 0x0c720),
    { { -36, -48, 63, 73 } },
};
const struct sprite_frame_1box gSpriteBank30Frame022 = {
    SPRITE_FRAME(gSpriteBank30Frame022, SPRITE_TILES_BANK30 + 0x0cfe0),
    { { -41, -59, 60, 82 } },
};
const struct sprite_frame_1box gSpriteBank30Frame023 = {
    SPRITE_FRAME(gSpriteBank30Frame023, SPRITE_TILES_BANK30 + 0x0d980),
    { { -43, -65, 59, 81 } },
};
const struct sprite_frame_1box gSpriteBank30Frame024 = {
    SPRITE_FRAME(gSpriteBank30Frame024, SPRITE_TILES_BANK30 + 0x0e340),
    { { -44, -64, 60, 79 } },
};
const struct sprite_frame_1box gSpriteBank30Frame025 = {
    SPRITE_FRAME(gSpriteBank30Frame025, SPRITE_TILES_BANK30 + 0x0ed00),
    { { -45, -61, 63, 80 } },
};
const struct sprite_frame_1box gSpriteBank30Frame026 = {
    SPRITE_FRAME(gSpriteBank30Frame026, SPRITE_TILES_BANK30 + 0x0f6e0),
    { { -46, -58, 67, 85 } },
};
const struct sprite_frame_1box gSpriteBank30Frame027 = {
    SPRITE_FRAME(gSpriteBank30Frame027, SPRITE_TILES_BANK30 + 0x10160),
    { { -46, -46, 95, 77 } },
};
const struct sprite_frame_1box gSpriteBank30Frame028 = {
    SPRITE_FRAME(gSpriteBank30Frame028, SPRITE_TILES_BANK30 + 0x10e00),
    { { -45, -38, 94, 71 } },
};
const struct sprite_frame_1box gSpriteBank30Frame029 = {
    SPRITE_FRAME(gSpriteBank30Frame029, SPRITE_TILES_BANK30 + 0x11880),
    { { -41, -36, 80, 67 } },
};
const struct sprite_frame_1box gSpriteBank30Frame030 = {
    SPRITE_FRAME(gSpriteBank30Frame030, SPRITE_TILES_BANK30 + 0x12200),
    { { -35, -38, 64, 70 } },
};
const struct sprite_frame_1box gSpriteBank30Frame031 = {
    SPRITE_FRAME(gSpriteBank30Frame031, SPRITE_TILES_BANK30 + 0x12a80),
    { { -35, -41, 61, 74 } },
};
const struct sprite_frame_1box gSpriteBank30Frame032 = {
    SPRITE_FRAME(gSpriteBank30Frame032, SPRITE_TILES_BANK30 + 0x13380),
    { { -34, -45, 55, 78 } },
};
const struct sprite_frame_1box gSpriteBank30Frame033 = {
    SPRITE_FRAME(gSpriteBank30Frame033, SPRITE_TILES_BANK30 + 0x13d00),
    { { -37, -45, 57, 78 } },
};
const struct sprite_frame_1box gSpriteBank30Frame034 = {
    SPRITE_FRAME(gSpriteBank30Frame034, SPRITE_TILES_BANK30 + 0x146c0),
    { { -40, -44, 59, 77 } },
};
const struct sprite_frame_1box gSpriteBank30Frame035 = {
    SPRITE_FRAME(gSpriteBank30Frame035, SPRITE_TILES_BANK30 + 0x15080),
    { { -39, -44, 57, 77 } },
};
const struct sprite_frame_1box gSpriteBank30Frame036 = {
    SPRITE_FRAME(gSpriteBank30Frame036, SPRITE_TILES_BANK30 + 0x15a40),
    { { -40, -43, 59, 76 } },
};
const struct sprite_frame_1box gSpriteBank30Frame037 = {
    SPRITE_FRAME(gSpriteBank30Frame037, SPRITE_TILES_BANK30 + 0x16400),
    { { -40, -42, 60, 75 } },
};
const struct sprite_frame_1box gSpriteBank30Frame038 = {
    SPRITE_FRAME(gSpriteBank30Frame038, SPRITE_TILES_BANK30 + 0x16dc0),
    { { -39, -42, 59, 75 } },
};
const struct sprite_frame_1box gSpriteBank30Frame039 = {
    SPRITE_FRAME(gSpriteBank30Frame039, SPRITE_TILES_BANK30 + 0x17780),
    { { -40, -42, 60, 75 } },
};
const struct sprite_frame_1box gSpriteBank30Frame040 = {
    SPRITE_FRAME(gSpriteBank30Frame040, SPRITE_TILES_BANK30 + 0x18140),
    { { -40, -43, 60, 76 } },
};
const struct sprite_frame_1box gSpriteBank30Frame041 = {
    SPRITE_FRAME(gSpriteBank30Frame041, SPRITE_TILES_BANK30 + 0x18b00),
    { { -39, -44, 58, 77 } },
};

const struct sprite_piece_pos gSpriteBank30Frame000Pos[3] = { { -32, -32 }, { 32, -26 }, { -7, 32 } };
const struct sprite_piece_pos gSpriteBank30Frame001Pos[3] = { { -34, -35 }, { 30, -29 }, { -14, 29 } };
const struct sprite_piece_pos gSpriteBank30Frame002Pos[4] = { { -37, -40 }, { 27, -36 }, { 27, -20 }, { -16, 24 } };
const struct sprite_piece_pos gSpriteBank30Frame003Pos[2] = { { -40, -46 }, { 24, -43 } };
const struct sprite_piece_pos gSpriteBank30Frame004Pos[2] = { { -42, -50 }, { 22, -49 } };
const struct sprite_piece_pos gSpriteBank30Frame005Pos[3] = { { -44, -49 }, { 20, -50 }, { 10, 14 } };
const struct sprite_piece_pos gSpriteBank30Frame006Pos[3] = { { -45, -45 }, { 19, -46 }, { 4, 18 } };
const struct sprite_piece_pos gSpriteBank30Frame007Pos[4] = { { -45, -41 }, { 19, -39 }, { -3, 23 }, { 5, 23 } };
const struct sprite_piece_pos gSpriteBank30Frame008Pos[4] = { { -45, -37 }, { 19, -35 }, { -8, 27 }, { 0, 27 } };
const struct sprite_piece_pos gSpriteBank30Frame009Pos[4] = { { -43, -33 }, { 21, -30 }, { 21, 2 }, { -16, 31 } };
const struct sprite_piece_pos gSpriteBank30Frame010Pos[3] = { { -39, -32 }, { 25, -27 }, { 25, 5 } };
const struct sprite_piece_pos gSpriteBank30Frame011Pos[3] = { { -40, -35 }, { 24, -29 }, { 24, 3 } };
const struct sprite_piece_pos gSpriteBank30Frame012Pos[3] = { { -39, -40 }, { 25, -35 }, { 25, -3 } };
const struct sprite_piece_pos gSpriteBank30Frame013Pos[3] = { { -36, -46 }, { 28, -42 }, { 28, -10 } };
const struct sprite_piece_pos gSpriteBank30Frame014Pos[3] = { { -34, -50 }, { 30, -48 }, { 30, -16 } };
const struct sprite_piece_pos gSpriteBank30Frame015Pos[4] = { { -36, -49 }, { 28, -49 }, { 28, -17 }, { -4, 15 } };
const struct sprite_piece_pos gSpriteBank30Frame016Pos[4] = { { -38, -46 }, { 26, -45 }, { 26, -8 }, { -9, 18 } };
const struct sprite_piece_pos gSpriteBank30Frame017Pos[4] = { { -39, -42 }, { 25, -40 }, { -13, 22 }, { 3, 22 } };
const struct sprite_piece_pos gSpriteBank30Frame018Pos[3] = { { -33, -38 }, { -15, 26 }, { 1, 26 } };
const struct sprite_piece_pos gSpriteBank30Frame019Pos[3] = { { -28, -34 }, { -15, 30 }, { 1, 30 } };
const struct sprite_piece_pos gSpriteBank30Frame020Pos[2] = { { -36, -36 }, { 28, -22 } };
const struct sprite_piece_pos gSpriteBank30Frame021Pos[3] = { { -36, -48 }, { -15, 16 }, { 1, 16 } };
const struct sprite_piece_pos gSpriteBank30Frame022Pos[4] = { { -41, -59 }, { -36, 5 }, { -4, 5 }, { -11, 21 } };
const struct sprite_piece_pos gSpriteBank30Frame023Pos[5] = { { -43, -65 }, { -38, -1 }, { -6, -1 }, { 10, -1 }, { -19, 15 } };
const struct sprite_piece_pos gSpriteBank30Frame024Pos[4] = { { -44, -64 }, { -38, 0 }, { -6, 0 }, { 10, 0 } };
const struct sprite_piece_pos gSpriteBank30Frame025Pos[5] = { { -45, -61 }, { -40, 3 }, { -8, 3 }, { 8, 4 }, { 1, 19 } };
const struct sprite_piece_pos gSpriteBank30Frame026Pos[7] = { { -46, -58 }, { 18, -57 }, { 18, -21 }, { -41, 6 }, { -9, 6 }, { 7, 14 }, { -8, 22 } };
const struct sprite_piece_pos gSpriteBank30Frame027Pos[4] = { { -46, -42 }, { 18, -46 }, { -14, 18 }, { 2, 18 } };
const struct sprite_piece_pos gSpriteBank30Frame028Pos[5] = { { -45, -38 }, { 19, -31 }, { 19, 1 }, { -13, 26 }, { 3, 28 } };
const struct sprite_piece_pos gSpriteBank30Frame029Pos[6] = { { -41, -36 }, { 23, -25 }, { 23, 7 }, { 39, 10 }, { -11, 28 }, { 5, 28 } };
const struct sprite_piece_pos gSpriteBank30Frame030Pos[4] = { { -35, -38 }, { 29, -17 }, { -15, 26 }, { 1, 26 } };
const struct sprite_piece_pos gSpriteBank30Frame031Pos[2] = { { -35, -41 }, { -14, 23 } };
const struct sprite_piece_pos gSpriteBank30Frame032Pos[3] = { { -27, -45 }, { -34, 19 }, { -2, 19 } };
const struct sprite_piece_pos gSpriteBank30Frame033Pos[4] = { { -26, -45 }, { -37, 19 }, { -5, 19 }, { 11, 20 } };
const struct sprite_piece_pos gSpriteBank30Frame034Pos[4] = { { -26, -44 }, { -40, 20 }, { -8, 20 }, { 8, 20 } };
const struct sprite_piece_pos gSpriteBank30Frame035Pos[4] = { { -26, -44 }, { -39, 20 }, { -7, 20 }, { 9, 20 } };
const struct sprite_piece_pos gSpriteBank30Frame036Pos[4] = { { -26, -43 }, { -40, 21 }, { -8, 21 }, { 8, 21 } };
const struct sprite_piece_pos gSpriteBank30Frame037Pos[4] = { { -27, -42 }, { -40, 22 }, { -8, 22 }, { 8, 22 } };
const struct sprite_piece_pos gSpriteBank30Frame038Pos[4] = { { -27, -42 }, { -39, 22 }, { -7, 22 }, { 9, 22 } };
const struct sprite_piece_pos gSpriteBank30Frame039Pos[4] = { { -26, -42 }, { -40, 22 }, { -8, 22 }, { 8, 22 } };
const struct sprite_piece_pos gSpriteBank30Frame040Pos[4] = { { -26, -43 }, { -40, 21 }, { -8, 21 }, { 8, 21 } };
const struct sprite_piece_pos gSpriteBank30Frame041Pos[4] = { { -26, -44 }, { -39, 20 }, { -7, 20 }, { 9, 20 } };

const u8 gSpriteBank30Frame000Pieces[3] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank30Frame001Pieces[3] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank30Frame002Pieces[4] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank30Frame003Pieces[2] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 10) };
const u8 gSpriteBank30Frame004Pieces[2] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 11) };
const u8 gSpriteBank30Frame005Pieces[3] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank30Frame006Pieces[3] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank30Frame007Pieces[4] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank30Frame008Pieces[4] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank30Frame009Pieces[4] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank30Frame010Pieces[3] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank30Frame011Pieces[3] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank30Frame012Pieces[3] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank30Frame013Pieces[3] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank30Frame014Pieces[3] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank30Frame015Pieces[4] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank30Frame016Pieces[4] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank30Frame017Pieces[4] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank30Frame018Pieces[3] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank30Frame019Pieces[3] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank30Frame020Pieces[2] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank30Frame021Pieces[3] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank30Frame022Pieces[4] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank30Frame023Pieces[5] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank30Frame024Pieces[4] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank30Frame025Pieces[5] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank30Frame026Pieces[7] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank30Frame027Pieces[4] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank30Frame028Pieces[5] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank30Frame029Pieces[6] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank30Frame030Pieces[4] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank30Frame031Pieces[2] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank30Frame032Pieces[3] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank30Frame033Pieces[4] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank30Frame034Pieces[4] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank30Frame035Pieces[4] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank30Frame036Pieces[4] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank30Frame037Pieces[4] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank30Frame038Pieces[4] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank30Frame039Pieces[4] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank30Frame040Pieces[4] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank30Frame041Pieces[4] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8) };

/* ---------------------------------------------------------------------- */
/* Bank 31: 36 animations, 112 frames, tiles in gSpriteBank31Tiles (SPRITE_TILES_BANK31). */

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
extern const struct sprite_frame_1box gSpriteBank31Frame000;
extern const struct sprite_frame gSpriteBank31Frame001;
extern const struct sprite_frame gSpriteBank31Frame002;
extern const struct sprite_frame gSpriteBank31Frame003;
extern const struct sprite_frame gSpriteBank31Frame004;
extern const struct sprite_frame gSpriteBank31Frame005;
extern const struct sprite_frame gSpriteBank31Frame006;
extern const struct sprite_frame gSpriteBank31Frame007;
extern const struct sprite_frame gSpriteBank31Frame008;
extern const struct sprite_frame gSpriteBank31Frame009;
extern const struct sprite_frame_1box gSpriteBank31Frame010;
extern const struct sprite_frame_1box gSpriteBank31Frame011;
extern const struct sprite_frame gSpriteBank31Frame012;
extern const struct sprite_frame gSpriteBank31Frame013;
extern const struct sprite_frame gSpriteBank31Frame014;
extern const struct sprite_frame gSpriteBank31Frame015;
extern const struct sprite_frame gSpriteBank31Frame016;
extern const struct sprite_frame gSpriteBank31Frame017;
extern const struct sprite_frame gSpriteBank31Frame018;
extern const struct sprite_frame gSpriteBank31Frame019;
extern const struct sprite_frame_1box gSpriteBank31Frame020;
extern const struct sprite_frame gSpriteBank31Frame021;
extern const struct sprite_frame gSpriteBank31Frame022;
extern const struct sprite_frame gSpriteBank31Frame023;
extern const struct sprite_frame gSpriteBank31Frame024;
extern const struct sprite_frame gSpriteBank31Frame025;
extern const struct sprite_frame gSpriteBank31Frame026;
extern const struct sprite_frame gSpriteBank31Frame027;
extern const struct sprite_frame gSpriteBank31Frame028;
extern const struct sprite_frame gSpriteBank31Frame029;
extern const struct sprite_frame gSpriteBank31Frame030;
extern const struct sprite_frame gSpriteBank31Frame031;
extern const struct sprite_frame gSpriteBank31Frame032;
extern const struct sprite_frame gSpriteBank31Frame033;
extern const struct sprite_frame gSpriteBank31Frame034;
extern const struct sprite_frame_1box gSpriteBank31Frame035;
extern const struct sprite_frame gSpriteBank31Frame036;
extern const struct sprite_frame gSpriteBank31Frame037;
extern const struct sprite_frame gSpriteBank31Frame038;
extern const struct sprite_frame gSpriteBank31Frame039;
extern const struct sprite_frame gSpriteBank31Frame040;
extern const struct sprite_frame gSpriteBank31Frame041;
extern const struct sprite_frame gSpriteBank31Frame042;
extern const struct sprite_frame gSpriteBank31Frame043;
extern const struct sprite_frame_1box gSpriteBank31Frame044;
extern const struct sprite_frame gSpriteBank31Frame045;
extern const struct sprite_frame gSpriteBank31Frame046;
extern const struct sprite_frame gSpriteBank31Frame047;
extern const struct sprite_frame gSpriteBank31Frame048;
extern const struct sprite_frame gSpriteBank31Frame049;
extern const struct sprite_frame gSpriteBank31Frame050;
extern const struct sprite_frame gSpriteBank31Frame051;
extern const struct sprite_frame gSpriteBank31Frame052;
extern const struct sprite_frame gSpriteBank31Frame053;
extern const struct sprite_frame_1box gSpriteBank31Frame054;
extern const struct sprite_frame_1box gSpriteBank31Frame055;
extern const struct sprite_frame_1box gSpriteBank31Frame056;
extern const struct sprite_frame_1box gSpriteBank31Frame057;
extern const struct sprite_frame_1box gSpriteBank31Frame058;
extern const struct sprite_frame gSpriteBank31Frame059;
extern const struct sprite_frame gSpriteBank31Frame060;
extern const struct sprite_frame gSpriteBank31Frame061;
extern const struct sprite_frame gSpriteBank31Frame062;
extern const struct sprite_frame gSpriteBank31Frame063;
extern const struct sprite_frame gSpriteBank31Frame064;
extern const struct sprite_frame gSpriteBank31Frame065;
extern const struct sprite_frame gSpriteBank31Frame066;
extern const struct sprite_frame gSpriteBank31Frame067;
extern const struct sprite_frame gSpriteBank31Frame068;
extern const struct sprite_frame gSpriteBank31Frame069;
extern const struct sprite_frame gSpriteBank31Frame070;
extern const struct sprite_frame gSpriteBank31Frame071;
extern const struct sprite_frame gSpriteBank31Frame072;
extern const struct sprite_frame gSpriteBank31Frame073;
extern const struct sprite_frame gSpriteBank31Frame074;
extern const struct sprite_frame gSpriteBank31Frame075;
extern const struct sprite_frame gSpriteBank31Frame076;
extern const struct sprite_frame gSpriteBank31Frame077;
extern const struct sprite_frame gSpriteBank31Frame078;
extern const struct sprite_frame gSpriteBank31Frame079;
extern const struct sprite_frame gSpriteBank31Frame080;
extern const struct sprite_frame gSpriteBank31Frame081;
extern const struct sprite_frame gSpriteBank31Frame082;
extern const struct sprite_frame gSpriteBank31Frame083;
extern const struct sprite_frame gSpriteBank31Frame084;
extern const struct sprite_frame gSpriteBank31Frame085;
extern const struct sprite_frame gSpriteBank31Frame086;
extern const struct sprite_frame gSpriteBank31Frame087;
extern const struct sprite_frame gSpriteBank31Frame088;
extern const struct sprite_frame gSpriteBank31Frame089;
extern const struct sprite_frame gSpriteBank31Frame090;
extern const struct sprite_frame gSpriteBank31Frame091;
extern const struct sprite_frame gSpriteBank31Frame092;
extern const struct sprite_frame gSpriteBank31Frame093;
extern const struct sprite_frame_1box gSpriteBank31Frame094;
extern const struct sprite_frame_1box gSpriteBank31Frame095;
extern const struct sprite_frame_1box gSpriteBank31Frame096;
extern const struct sprite_frame_1box gSpriteBank31Frame097;
extern const struct sprite_frame_1box gSpriteBank31Frame098;
extern const struct sprite_frame_1box gSpriteBank31Frame099;
extern const struct sprite_frame_1box gSpriteBank31Frame100;
extern const struct sprite_frame_1box gSpriteBank31Frame101;
extern const struct sprite_frame_1box gSpriteBank31Frame102;
extern const struct sprite_frame_1box gSpriteBank31Frame103;
extern const struct sprite_frame_1box gSpriteBank31Frame104;
extern const struct sprite_frame_1box gSpriteBank31Frame105;
extern const struct sprite_frame_1box gSpriteBank31Frame106;
extern const struct sprite_frame_1box gSpriteBank31Frame107;
extern const struct sprite_frame_1box gSpriteBank31Frame108;
extern const struct sprite_frame_1box gSpriteBank31Frame109;
extern const struct sprite_frame_1box gSpriteBank31Frame110;
extern const struct sprite_frame_1box gSpriteBank31Frame111;
extern const struct sprite_piece_pos gSpriteBank31Frame000Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame001Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame002Pos[2];
extern const struct sprite_piece_pos gSpriteBank31Frame003Pos[3];
extern const struct sprite_piece_pos gSpriteBank31Frame004Pos[3];
extern const struct sprite_piece_pos gSpriteBank31Frame005Pos[4];
extern const struct sprite_piece_pos gSpriteBank31Frame006Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame007Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame008Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame009Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame010Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame011Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame012Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame013Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame014Pos[3];
extern const struct sprite_piece_pos gSpriteBank31Frame015Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame016Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame017Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame018Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame019Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame020Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame021Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame022Pos[3];
extern const struct sprite_piece_pos gSpriteBank31Frame023Pos[4];
extern const struct sprite_piece_pos gSpriteBank31Frame024Pos[3];
extern const struct sprite_piece_pos gSpriteBank31Frame025Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame026Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame027Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame028Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame029Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame030Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame031Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame032Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame033Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame034Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame035Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame036Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame037Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame038Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame039Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame040Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame041Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame042Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame043Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame044Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame045Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame046Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame047Pos[2];
extern const struct sprite_piece_pos gSpriteBank31Frame048Pos[2];
extern const struct sprite_piece_pos gSpriteBank31Frame049Pos[2];
extern const struct sprite_piece_pos gSpriteBank31Frame050Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame051Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame052Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame053Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame054Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame055Pos[2];
extern const struct sprite_piece_pos gSpriteBank31Frame056Pos[2];
extern const struct sprite_piece_pos gSpriteBank31Frame057Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame058Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame059Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame060Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame061Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame062Pos[2];
extern const struct sprite_piece_pos gSpriteBank31Frame063Pos[3];
extern const struct sprite_piece_pos gSpriteBank31Frame064Pos[2];
extern const struct sprite_piece_pos gSpriteBank31Frame065Pos[3];
extern const struct sprite_piece_pos gSpriteBank31Frame066Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame067Pos[5];
extern const struct sprite_piece_pos gSpriteBank31Frame068Pos[4];
extern const struct sprite_piece_pos gSpriteBank31Frame069Pos[4];
extern const struct sprite_piece_pos gSpriteBank31Frame070Pos[3];
extern const struct sprite_piece_pos gSpriteBank31Frame071Pos[5];
extern const struct sprite_piece_pos gSpriteBank31Frame072Pos[3];
extern const struct sprite_piece_pos gSpriteBank31Frame073Pos[5];
extern const struct sprite_piece_pos gSpriteBank31Frame074Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame075Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame076Pos[3];
extern const struct sprite_piece_pos gSpriteBank31Frame077Pos[2];
extern const struct sprite_piece_pos gSpriteBank31Frame078Pos[4];
extern const struct sprite_piece_pos gSpriteBank31Frame079Pos[4];
extern const struct sprite_piece_pos gSpriteBank31Frame080Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame081Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame082Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame083Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame084Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame085Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame086Pos[2];
extern const struct sprite_piece_pos gSpriteBank31Frame087Pos[3];
extern const struct sprite_piece_pos gSpriteBank31Frame088Pos[3];
extern const struct sprite_piece_pos gSpriteBank31Frame089Pos[4];
extern const struct sprite_piece_pos gSpriteBank31Frame090Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame091Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame092Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame093Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame094Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame095Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame096Pos[3];
extern const struct sprite_piece_pos gSpriteBank31Frame097Pos[2];
extern const struct sprite_piece_pos gSpriteBank31Frame098Pos[2];
extern const struct sprite_piece_pos gSpriteBank31Frame099Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame100Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame101Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame102Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame103Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame104Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame105Pos[3];
extern const struct sprite_piece_pos gSpriteBank31Frame106Pos[2];
extern const struct sprite_piece_pos gSpriteBank31Frame107Pos[2];
extern const struct sprite_piece_pos gSpriteBank31Frame108Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame109Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame110Pos[1];
extern const struct sprite_piece_pos gSpriteBank31Frame111Pos[1];
extern const u8 gSpriteBank31Frame000Pieces[1];
extern const u8 gSpriteBank31Frame001Pieces[1];
extern const u8 gSpriteBank31Frame002Pieces[2];
extern const u8 gSpriteBank31Frame003Pieces[3];
extern const u8 gSpriteBank31Frame004Pieces[3];
extern const u8 gSpriteBank31Frame005Pieces[4];
extern const u8 gSpriteBank31Frame006Pieces[1];
extern const u8 gSpriteBank31Frame007Pieces[1];
extern const u8 gSpriteBank31Frame008Pieces[1];
extern const u8 gSpriteBank31Frame009Pieces[1];
extern const u8 gSpriteBank31Frame010Pieces[1];
extern const u8 gSpriteBank31Frame011Pieces[1];
extern const u8 gSpriteBank31Frame012Pieces[1];
extern const u8 gSpriteBank31Frame013Pieces[1];
extern const u8 gSpriteBank31Frame014Pieces[3];
extern const u8 gSpriteBank31Frame015Pieces[1];
extern const u8 gSpriteBank31Frame016Pieces[1];
extern const u8 gSpriteBank31Frame017Pieces[1];
extern const u8 gSpriteBank31Frame018Pieces[1];
extern const u8 gSpriteBank31Frame019Pieces[1];
extern const u8 gSpriteBank31Frame020Pieces[1];
extern const u8 gSpriteBank31Frame021Pieces[1];
extern const u8 gSpriteBank31Frame022Pieces[3];
extern const u8 gSpriteBank31Frame023Pieces[4];
extern const u8 gSpriteBank31Frame024Pieces[3];
extern const u8 gSpriteBank31Frame025Pieces[1];
extern const u8 gSpriteBank31Frame026Pieces[1];
extern const u8 gSpriteBank31Frame027Pieces[1];
extern const u8 gSpriteBank31Frame028Pieces[1];
extern const u8 gSpriteBank31Frame029Pieces[1];
extern const u8 gSpriteBank31Frame030Pieces[1];
extern const u8 gSpriteBank31Frame031Pieces[1];
extern const u8 gSpriteBank31Frame032Pieces[1];
extern const u8 gSpriteBank31Frame033Pieces[1];
extern const u8 gSpriteBank31Frame034Pieces[1];
extern const u8 gSpriteBank31Frame035Pieces[1];
extern const u8 gSpriteBank31Frame036Pieces[1];
extern const u8 gSpriteBank31Frame037Pieces[1];
extern const u8 gSpriteBank31Frame038Pieces[1];
extern const u8 gSpriteBank31Frame039Pieces[1];
extern const u8 gSpriteBank31Frame040Pieces[1];
extern const u8 gSpriteBank31Frame041Pieces[1];
extern const u8 gSpriteBank31Frame042Pieces[1];
extern const u8 gSpriteBank31Frame043Pieces[1];
extern const u8 gSpriteBank31Frame044Pieces[1];
extern const u8 gSpriteBank31Frame045Pieces[1];
extern const u8 gSpriteBank31Frame046Pieces[1];
extern const u8 gSpriteBank31Frame047Pieces[2];
extern const u8 gSpriteBank31Frame048Pieces[2];
extern const u8 gSpriteBank31Frame049Pieces[2];
extern const u8 gSpriteBank31Frame050Pieces[1];
extern const u8 gSpriteBank31Frame051Pieces[1];
extern const u8 gSpriteBank31Frame052Pieces[1];
extern const u8 gSpriteBank31Frame053Pieces[1];
extern const u8 gSpriteBank31Frame054Pieces[1];
extern const u8 gSpriteBank31Frame055Pieces[2];
extern const u8 gSpriteBank31Frame056Pieces[2];
extern const u8 gSpriteBank31Frame057Pieces[1];
extern const u8 gSpriteBank31Frame058Pieces[1];
extern const u8 gSpriteBank31Frame059Pieces[1];
extern const u8 gSpriteBank31Frame060Pieces[1];
extern const u8 gSpriteBank31Frame061Pieces[1];
extern const u8 gSpriteBank31Frame062Pieces[2];
extern const u8 gSpriteBank31Frame063Pieces[3];
extern const u8 gSpriteBank31Frame064Pieces[2];
extern const u8 gSpriteBank31Frame065Pieces[3];
extern const u8 gSpriteBank31Frame066Pieces[1];
extern const u8 gSpriteBank31Frame067Pieces[5];
extern const u8 gSpriteBank31Frame068Pieces[4];
extern const u8 gSpriteBank31Frame069Pieces[4];
extern const u8 gSpriteBank31Frame070Pieces[3];
extern const u8 gSpriteBank31Frame071Pieces[5];
extern const u8 gSpriteBank31Frame072Pieces[3];
extern const u8 gSpriteBank31Frame073Pieces[5];
extern const u8 gSpriteBank31Frame074Pieces[1];
extern const u8 gSpriteBank31Frame075Pieces[1];
extern const u8 gSpriteBank31Frame076Pieces[3];
extern const u8 gSpriteBank31Frame077Pieces[2];
extern const u8 gSpriteBank31Frame078Pieces[4];
extern const u8 gSpriteBank31Frame079Pieces[4];
extern const u8 gSpriteBank31Frame080Pieces[1];
extern const u8 gSpriteBank31Frame081Pieces[1];
extern const u8 gSpriteBank31Frame082Pieces[1];
extern const u8 gSpriteBank31Frame083Pieces[1];
extern const u8 gSpriteBank31Frame084Pieces[1];
extern const u8 gSpriteBank31Frame085Pieces[1];
extern const u8 gSpriteBank31Frame086Pieces[2];
extern const u8 gSpriteBank31Frame087Pieces[3];
extern const u8 gSpriteBank31Frame088Pieces[3];
extern const u8 gSpriteBank31Frame089Pieces[4];
extern const u8 gSpriteBank31Frame090Pieces[1];
extern const u8 gSpriteBank31Frame091Pieces[1];
extern const u8 gSpriteBank31Frame092Pieces[1];
extern const u8 gSpriteBank31Frame093Pieces[1];
extern const u8 gSpriteBank31Frame094Pieces[1];
extern const u8 gSpriteBank31Frame095Pieces[1];
extern const u8 gSpriteBank31Frame096Pieces[3];
extern const u8 gSpriteBank31Frame097Pieces[2];
extern const u8 gSpriteBank31Frame098Pieces[2];
extern const u8 gSpriteBank31Frame099Pieces[1];
extern const u8 gSpriteBank31Frame100Pieces[1];
extern const u8 gSpriteBank31Frame101Pieces[1];
extern const u8 gSpriteBank31Frame102Pieces[1];
extern const u8 gSpriteBank31Frame103Pieces[1];
extern const u8 gSpriteBank31Frame104Pieces[1];
extern const u8 gSpriteBank31Frame105Pieces[3];
extern const u8 gSpriteBank31Frame106Pieces[2];
extern const u8 gSpriteBank31Frame107Pieces[2];
extern const u8 gSpriteBank31Frame108Pieces[1];
extern const u8 gSpriteBank31Frame109Pieces[1];
extern const u8 gSpriteBank31Frame110Pieces[1];
extern const u8 gSpriteBank31Frame111Pieces[1];

const struct sprite_anim gSpriteBank31Anims[36] = {
    [0] = {
        .seq = gSpriteBank31Anim00Seq,
        .box = { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        .paletteId = 45,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim00Seq),
        .flags = 0,
    },
    [1] = {
        .seq = gSpriteBank31Anim01Seq,
        .box = { { -12, -6, 19, 19 }, { -30, -49, 63, 62 } },
        .paletteId = 46,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim01Seq),
        .flags = 0,
    },
    [2] = {
        .seq = gSpriteBank31Anim02Seq,
        .box = { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        .paletteId = 47,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim02Seq),
        .flags = 0,
    },
    [3] = {
        .seq = gSpriteBank31Anim03Seq,
        .box = { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        .paletteId = 47,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim03Seq),
        .flags = 0,
    },
    [4] = {
        .seq = gSpriteBank31Anim04Seq,
        .box = { { -12, -6, 19, 19 }, { -16, -15, 33, 28 } },
        .paletteId = 46,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim04Seq),
        .flags = 0,
    },
    [5] = {
        .seq = gSpriteBank31Anim05Seq,
        .box = { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        .paletteId = 46,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim05Seq),
        .flags = 0,
    },
    [6] = {
        .seq = gSpriteBank31Anim06Seq,
        .box = { { -12, -6, 19, 19 }, { -16, -15, 36, 28 } },
        .paletteId = 48,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim06Seq),
        .flags = 0,
    },
    [7] = {
        .seq = gSpriteBank31Anim07Seq,
        .box = { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        .paletteId = 45,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim07Seq),
        .flags = 0,
    },
    [8] = {
        .seq = gSpriteBank31Anim08Seq,
        .box = { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        .paletteId = 45,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim08Seq),
        .flags = 0,
    },
    [9] = {
        .seq = gSpriteBank31Anim09Seq,
        .box = { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        .paletteId = 45,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim09Seq),
        .flags = 0,
    },
    [10] = {
        .seq = gSpriteBank31Anim10Seq,
        .box = { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        .paletteId = 45,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim10Seq),
        .flags = 0,
    },
    [11] = {
        .seq = gSpriteBank31Anim11Seq,
        .box = { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        .paletteId = 45,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim11Seq),
        .flags = 0,
    },
    [12] = {
        .seq = gSpriteBank31Anim12Seq,
        .box = { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        .paletteId = 45,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim12Seq),
        .flags = 0,
    },
    [13] = {
        .seq = gSpriteBank31Anim13Seq,
        .box = { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        .paletteId = 45,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim13Seq),
        .flags = 0,
    },
    [14] = {
        .seq = gSpriteBank31Anim14Seq,
        .box = { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        .paletteId = 46,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim14Seq),
        .flags = 0,
    },
    [15] = {
        .seq = gSpriteBank31Anim15Seq,
        .box = { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        .paletteId = 46,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim15Seq),
        .flags = 0,
    },
    [16] = {
        .seq = gSpriteBank31Anim16Seq,
        .box = { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        .paletteId = 46,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim16Seq),
        .flags = 0,
    },
    [17] = {
        .seq = gSpriteBank31Anim17Seq,
        .box = { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        .paletteId = 46,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim17Seq),
        .flags = 0,
    },
    [18] = {
        .seq = gSpriteBank31Anim18Seq,
        .box = { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        .paletteId = 46,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim18Seq),
        .flags = 0,
    },
    [19] = {
        .seq = gSpriteBank31Anim19Seq,
        .box = { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        .paletteId = 46,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim19Seq),
        .flags = 0,
    },
    [20] = {
        .seq = gSpriteBank31Anim20Seq,
        .box = { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        .paletteId = 46,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim20Seq),
        .flags = 0,
    },
    [21] = {
        .seq = gSpriteBank31Anim21Seq,
        .box = { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        .paletteId = 48,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim21Seq),
        .flags = 0,
    },
    [22] = {
        .seq = gSpriteBank31Anim22Seq,
        .box = { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        .paletteId = 45,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim22Seq),
        .flags = 0,
    },
    [23] = {
        .seq = gSpriteBank31Anim23Seq,
        .box = { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        .paletteId = 45,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim23Seq),
        .flags = 0,
    },
    [24] = {
        .seq = gSpriteBank31Anim24Seq,
        .box = { { -12, -6, 19, 19 }, { -12, -14, 26, 27 } },
        .paletteId = 45,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim24Seq),
        .flags = 0,
    },
    [25] = {
        .seq = gSpriteBank31Anim25Seq,
        .box = { { -12, -6, 19, 19 }, { -14, -15, 30, 28 } },
        .paletteId = 45,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim25Seq),
        .flags = 0,
    },
    [26] = {
        .seq = gSpriteBank31Anim26Seq,
        .box = { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        .paletteId = 45,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim26Seq),
        .flags = 0,
    },
    [27] = {
        .seq = gSpriteBank31Anim27Seq,
        .box = { { -12, -13, 25, 26 }, { -32, -13, 66, 32 } },
        .paletteId = 45,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim27Seq),
        .flags = 0,
    },
    [28] = {
        .seq = gSpriteBank31Anim28Seq,
        .box = { { -12, -6, 19, 19 }, { -14, -13, 27, 26 } },
        .paletteId = 45,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim28Seq),
        .flags = 0,
    },
    [29] = {
        .seq = gSpriteBank31Anim29Seq,
        .box = { { -12, -6, 19, 19 }, { -30, -49, 63, 62 } },
        .paletteId = 45,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim29Seq),
        .flags = 0,
    },
    [30] = {
        .seq = gSpriteBank31Anim30Seq,
        .box = { { -12, -6, 19, 19 }, { -14, -13, 27, 26 } },
        .paletteId = 45,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim30Seq),
        .flags = 0,
    },
    [31] = {
        .seq = gSpriteBank31Anim31Seq,
        .box = { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        .paletteId = 45,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim31Seq),
        .flags = 0,
    },
    [32] = {
        .seq = gSpriteBank31Anim32Seq,
        .box = { { -12, -6, 19, 19 }, { -12, -13, 25, 26 } },
        .paletteId = 47,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim32Seq),
        .flags = 0,
    },
    [33] = {
        .seq = gSpriteBank31Anim33Seq,
        .box = { { -12, -6, 19, 19 }, { -30, -49, 63, 62 } },
        .paletteId = 46,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim33Seq),
        .flags = 0,
    },
    [34] = {
        .seq = gSpriteBank31Anim34Seq,
        .box = { { -12, -6, 19, 19 }, { -16, -40, 33, 53 } },
        .paletteId = 47,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim34Seq),
        .flags = 0,
    },
    [35] = {
        .seq = gSpriteBank31Anim35Seq,
        .box = { { -12, -6, 19, 19 }, { -16, -40, 33, 53 } },
        .paletteId = 46,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank31Anim35Seq),
        .flags = 0,
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

const struct sprite_frame *const gSpriteBank31Frames[112] = {
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
    SPRITE_FRAME(gSpriteBank31Frame000, SPRITE_TILES_BANK31 + 0x00000),
    { { -12, -13, 25, 26 } },
};
const struct sprite_frame gSpriteBank31Frame001 = SPRITE_FRAME(gSpriteBank31Frame001, SPRITE_TILES_BANK31 + 0x00200);
const struct sprite_frame gSpriteBank31Frame002 = SPRITE_FRAME(gSpriteBank31Frame002, SPRITE_TILES_BANK31 + 0x00400);
const struct sprite_frame gSpriteBank31Frame003 = SPRITE_FRAME(gSpriteBank31Frame003, SPRITE_TILES_BANK31 + 0x00580);
const struct sprite_frame gSpriteBank31Frame004 = SPRITE_FRAME(gSpriteBank31Frame004, SPRITE_TILES_BANK31 + 0x007e0);
const struct sprite_frame gSpriteBank31Frame005 = SPRITE_FRAME(gSpriteBank31Frame005, SPRITE_TILES_BANK31 + 0x00ca0);
const struct sprite_frame gSpriteBank31Frame006 = SPRITE_FRAME(gSpriteBank31Frame006, SPRITE_TILES_BANK31 + 0x01260);
const struct sprite_frame gSpriteBank31Frame007 = SPRITE_FRAME(gSpriteBank31Frame007, SPRITE_TILES_BANK31 + 0x01a20);
const struct sprite_frame gSpriteBank31Frame008 = SPRITE_FRAME(gSpriteBank31Frame008, SPRITE_TILES_BANK31 + 0x02220);
const struct sprite_frame gSpriteBank31Frame009 = SPRITE_FRAME(gSpriteBank31Frame009, SPRITE_TILES_BANK31 + 0x02a20);
const struct sprite_frame_1box gSpriteBank31Frame010 = {
    SPRITE_FRAME(gSpriteBank31Frame010, SPRITE_TILES_BANK31 + 0x03220),
    { { -12, -13, 25, 26 } },
};
const struct sprite_frame_1box gSpriteBank31Frame011 = {
    SPRITE_FRAME(gSpriteBank31Frame011, SPRITE_TILES_BANK31 + 0x03420),
    { { -12, -13, 25, 26 } },
};
const struct sprite_frame gSpriteBank31Frame012 = SPRITE_FRAME(gSpriteBank31Frame012, SPRITE_TILES_BANK31 + 0x03620);
const struct sprite_frame gSpriteBank31Frame013 = SPRITE_FRAME(gSpriteBank31Frame013, SPRITE_TILES_BANK31 + 0x03820);
const struct sprite_frame gSpriteBank31Frame014 = SPRITE_FRAME(gSpriteBank31Frame014, SPRITE_TILES_BANK31 + 0x03a20);
const struct sprite_frame gSpriteBank31Frame015 = SPRITE_FRAME(gSpriteBank31Frame015, SPRITE_TILES_BANK31 + 0x03be0);
const struct sprite_frame gSpriteBank31Frame016 = SPRITE_FRAME(gSpriteBank31Frame016, SPRITE_TILES_BANK31 + 0x03de0);
const struct sprite_frame gSpriteBank31Frame017 = SPRITE_FRAME(gSpriteBank31Frame017, SPRITE_TILES_BANK31 + 0x03fe0);
const struct sprite_frame gSpriteBank31Frame018 = SPRITE_FRAME(gSpriteBank31Frame018, SPRITE_TILES_BANK31 + 0x041e0);
const struct sprite_frame gSpriteBank31Frame019 = SPRITE_FRAME(gSpriteBank31Frame019, SPRITE_TILES_BANK31 + 0x043e0);
const struct sprite_frame_1box gSpriteBank31Frame020 = {
    SPRITE_FRAME(gSpriteBank31Frame020, SPRITE_TILES_BANK31 + 0x00200),
    { { -12, -13, 25, 26 } },
};
const struct sprite_frame gSpriteBank31Frame021 = SPRITE_FRAME(gSpriteBank31Frame021, SPRITE_TILES_BANK31 + 0x045e0);
const struct sprite_frame gSpriteBank31Frame022 = SPRITE_FRAME(gSpriteBank31Frame022, SPRITE_TILES_BANK31 + 0x047e0);
const struct sprite_frame gSpriteBank31Frame023 = SPRITE_FRAME(gSpriteBank31Frame023, SPRITE_TILES_BANK31 + 0x04a40);
const struct sprite_frame gSpriteBank31Frame024 = SPRITE_FRAME(gSpriteBank31Frame024, SPRITE_TILES_BANK31 + 0x04c20);
const struct sprite_frame gSpriteBank31Frame025 = SPRITE_FRAME(gSpriteBank31Frame025, SPRITE_TILES_BANK31 + 0x04e80);
const struct sprite_frame gSpriteBank31Frame026 = SPRITE_FRAME(gSpriteBank31Frame026, SPRITE_TILES_BANK31 + 0x05080);
const struct sprite_frame gSpriteBank31Frame027 = SPRITE_FRAME(gSpriteBank31Frame027, SPRITE_TILES_BANK31 + 0x05280);
const struct sprite_frame gSpriteBank31Frame028 = SPRITE_FRAME(gSpriteBank31Frame028, SPRITE_TILES_BANK31 + 0x05480);
const struct sprite_frame gSpriteBank31Frame029 = SPRITE_FRAME(gSpriteBank31Frame029, SPRITE_TILES_BANK31 + 0x05680);
const struct sprite_frame gSpriteBank31Frame030 = SPRITE_FRAME(gSpriteBank31Frame030, SPRITE_TILES_BANK31 + 0x05880);
const struct sprite_frame gSpriteBank31Frame031 = SPRITE_FRAME(gSpriteBank31Frame031, SPRITE_TILES_BANK31 + 0x05a80);
const struct sprite_frame gSpriteBank31Frame032 = SPRITE_FRAME(gSpriteBank31Frame032, SPRITE_TILES_BANK31 + 0x05c80);
const struct sprite_frame gSpriteBank31Frame033 = SPRITE_FRAME(gSpriteBank31Frame033, SPRITE_TILES_BANK31 + 0x05e80);
const struct sprite_frame gSpriteBank31Frame034 = SPRITE_FRAME(gSpriteBank31Frame034, SPRITE_TILES_BANK31 + 0x06080);
const struct sprite_frame_1box gSpriteBank31Frame035 = {
    SPRITE_FRAME(gSpriteBank31Frame035, SPRITE_TILES_BANK31 + 0x06280),
    { { -12, -13, 25, 26 } },
};
const struct sprite_frame gSpriteBank31Frame036 = SPRITE_FRAME(gSpriteBank31Frame036, SPRITE_TILES_BANK31 + 0x06480);
const struct sprite_frame gSpriteBank31Frame037 = SPRITE_FRAME(gSpriteBank31Frame037, SPRITE_TILES_BANK31 + 0x06680);
const struct sprite_frame gSpriteBank31Frame038 = SPRITE_FRAME(gSpriteBank31Frame038, SPRITE_TILES_BANK31 + 0x06880);
const struct sprite_frame gSpriteBank31Frame039 = SPRITE_FRAME(gSpriteBank31Frame039, SPRITE_TILES_BANK31 + 0x06a80);
const struct sprite_frame gSpriteBank31Frame040 = SPRITE_FRAME(gSpriteBank31Frame040, SPRITE_TILES_BANK31 + 0x06c80);
const struct sprite_frame gSpriteBank31Frame041 = SPRITE_FRAME(gSpriteBank31Frame041, SPRITE_TILES_BANK31 + 0x06e80);
const struct sprite_frame gSpriteBank31Frame042 = SPRITE_FRAME(gSpriteBank31Frame042, SPRITE_TILES_BANK31 + 0x07080);
const struct sprite_frame gSpriteBank31Frame043 = SPRITE_FRAME(gSpriteBank31Frame043, SPRITE_TILES_BANK31 + 0x07280);
const struct sprite_frame_1box gSpriteBank31Frame044 = {
    SPRITE_FRAME(gSpriteBank31Frame044, SPRITE_TILES_BANK31 + 0x07480),
    { { -12, -13, 25, 26 } },
};
const struct sprite_frame gSpriteBank31Frame045 = SPRITE_FRAME(gSpriteBank31Frame045, SPRITE_TILES_BANK31 + 0x07680);
const struct sprite_frame gSpriteBank31Frame046 = SPRITE_FRAME(gSpriteBank31Frame046, SPRITE_TILES_BANK31 + 0x07880);
const struct sprite_frame gSpriteBank31Frame047 = SPRITE_FRAME(gSpriteBank31Frame047, SPRITE_TILES_BANK31 + 0x07a80);
const struct sprite_frame gSpriteBank31Frame048 = SPRITE_FRAME(gSpriteBank31Frame048, SPRITE_TILES_BANK31 + 0x07c00);
const struct sprite_frame gSpriteBank31Frame049 = SPRITE_FRAME(gSpriteBank31Frame049, SPRITE_TILES_BANK31 + 0x07d80);
const struct sprite_frame gSpriteBank31Frame050 = SPRITE_FRAME(gSpriteBank31Frame050, SPRITE_TILES_BANK31 + 0x07f00);
const struct sprite_frame gSpriteBank31Frame051 = SPRITE_FRAME(gSpriteBank31Frame051, SPRITE_TILES_BANK31 + 0x08100);
const struct sprite_frame gSpriteBank31Frame052 = SPRITE_FRAME(gSpriteBank31Frame052, SPRITE_TILES_BANK31 + 0x08300);
const struct sprite_frame gSpriteBank31Frame053 = SPRITE_FRAME(gSpriteBank31Frame053, SPRITE_TILES_BANK31 + 0x08500);
const struct sprite_frame_1box gSpriteBank31Frame054 = {
    SPRITE_FRAME(gSpriteBank31Frame054, SPRITE_TILES_BANK31 + 0x08700),
    { { -12, -13, 25, 26 } },
};
const struct sprite_frame_1box gSpriteBank31Frame055 = {
    SPRITE_FRAME(gSpriteBank31Frame055, SPRITE_TILES_BANK31 + 0x08900),
    { { -14, -9, 30, 22 } },
};
const struct sprite_frame_1box gSpriteBank31Frame056 = {
    SPRITE_FRAME(gSpriteBank31Frame056, SPRITE_TILES_BANK31 + 0x08a80),
    { { -14, -8, 30, 21 } },
};
const struct sprite_frame_1box gSpriteBank31Frame057 = {
    SPRITE_FRAME(gSpriteBank31Frame057, SPRITE_TILES_BANK31 + 0x08c00),
    { { -12, -15, 24, 28 } },
};
const struct sprite_frame_1box gSpriteBank31Frame058 = {
    SPRITE_FRAME(gSpriteBank31Frame058, SPRITE_TILES_BANK31 + 0x08e00),
    { { -12, -13, 26, 26 } },
};
const struct sprite_frame gSpriteBank31Frame059 = SPRITE_FRAME(gSpriteBank31Frame059, SPRITE_TILES_BANK31 + 0x09000);
const struct sprite_frame gSpriteBank31Frame060 = SPRITE_FRAME(gSpriteBank31Frame060, SPRITE_TILES_BANK31 + 0x09200);
const struct sprite_frame gSpriteBank31Frame061 = SPRITE_FRAME(gSpriteBank31Frame061, SPRITE_TILES_BANK31 + 0x09400);
const struct sprite_frame gSpriteBank31Frame062 = SPRITE_FRAME(gSpriteBank31Frame062, SPRITE_TILES_BANK31 + 0x09600);
const struct sprite_frame gSpriteBank31Frame063 = SPRITE_FRAME(gSpriteBank31Frame063, SPRITE_TILES_BANK31 + 0x09820);
const struct sprite_frame gSpriteBank31Frame064 = SPRITE_FRAME(gSpriteBank31Frame064, SPRITE_TILES_BANK31 + 0x09a80);
const struct sprite_frame gSpriteBank31Frame065 = SPRITE_FRAME(gSpriteBank31Frame065, SPRITE_TILES_BANK31 + 0x09d00);
const struct sprite_frame gSpriteBank31Frame066 = SPRITE_FRAME(gSpriteBank31Frame066, SPRITE_TILES_BANK31 + 0x09fa0);
const struct sprite_frame gSpriteBank31Frame067 = SPRITE_FRAME(gSpriteBank31Frame067, SPRITE_TILES_BANK31 + 0x0a3a0);
const struct sprite_frame gSpriteBank31Frame068 = SPRITE_FRAME(gSpriteBank31Frame068, SPRITE_TILES_BANK31 + 0x0a5e0);
const struct sprite_frame gSpriteBank31Frame069 = SPRITE_FRAME(gSpriteBank31Frame069, SPRITE_TILES_BANK31 + 0x0a800);
const struct sprite_frame gSpriteBank31Frame070 = SPRITE_FRAME(gSpriteBank31Frame070, SPRITE_TILES_BANK31 + 0x0aa60);
const struct sprite_frame gSpriteBank31Frame071 = SPRITE_FRAME(gSpriteBank31Frame071, SPRITE_TILES_BANK31 + 0x0ac80);
const struct sprite_frame gSpriteBank31Frame072 = SPRITE_FRAME(gSpriteBank31Frame072, SPRITE_TILES_BANK31 + 0x0af00);
const struct sprite_frame gSpriteBank31Frame073 = SPRITE_FRAME(gSpriteBank31Frame073, SPRITE_TILES_BANK31 + 0x0b120);
const struct sprite_frame gSpriteBank31Frame074 = SPRITE_FRAME(gSpriteBank31Frame074, SPRITE_TILES_BANK31 + 0x0b3a0);
const struct sprite_frame gSpriteBank31Frame075 = SPRITE_FRAME(gSpriteBank31Frame075, SPRITE_TILES_BANK31 + 0x07480);
const struct sprite_frame gSpriteBank31Frame076 = SPRITE_FRAME(gSpriteBank31Frame076, SPRITE_TILES_BANK31 + 0x0b5a0);
const struct sprite_frame gSpriteBank31Frame077 = SPRITE_FRAME(gSpriteBank31Frame077, SPRITE_TILES_BANK31 + 0x0b6a0);
const struct sprite_frame gSpriteBank31Frame078 = SPRITE_FRAME(gSpriteBank31Frame078, SPRITE_TILES_BANK31 + 0x0b8c0);
const struct sprite_frame gSpriteBank31Frame079 = SPRITE_FRAME(gSpriteBank31Frame079, SPRITE_TILES_BANK31 + 0x0bb80);
const struct sprite_frame gSpriteBank31Frame080 = SPRITE_FRAME(gSpriteBank31Frame080, SPRITE_TILES_BANK31 + 0x0c040);
const struct sprite_frame gSpriteBank31Frame081 = SPRITE_FRAME(gSpriteBank31Frame081, SPRITE_TILES_BANK31 + 0x0c800);
const struct sprite_frame gSpriteBank31Frame082 = SPRITE_FRAME(gSpriteBank31Frame082, SPRITE_TILES_BANK31 + 0x0d000);
const struct sprite_frame gSpriteBank31Frame083 = SPRITE_FRAME(gSpriteBank31Frame083, SPRITE_TILES_BANK31 + 0x0d800);
const struct sprite_frame gSpriteBank31Frame084 = SPRITE_FRAME(gSpriteBank31Frame084, SPRITE_TILES_BANK31 + 0x0e000);
const struct sprite_frame gSpriteBank31Frame085 = SPRITE_FRAME(gSpriteBank31Frame085, SPRITE_TILES_BANK31 + 0x0e200);
const struct sprite_frame gSpriteBank31Frame086 = SPRITE_FRAME(gSpriteBank31Frame086, SPRITE_TILES_BANK31 + 0x0e400);
const struct sprite_frame gSpriteBank31Frame087 = SPRITE_FRAME(gSpriteBank31Frame087, SPRITE_TILES_BANK31 + 0x0e580);
const struct sprite_frame gSpriteBank31Frame088 = SPRITE_FRAME(gSpriteBank31Frame088, SPRITE_TILES_BANK31 + 0x0e7e0);
const struct sprite_frame gSpriteBank31Frame089 = SPRITE_FRAME(gSpriteBank31Frame089, SPRITE_TILES_BANK31 + 0x0eca0);
const struct sprite_frame gSpriteBank31Frame090 = SPRITE_FRAME(gSpriteBank31Frame090, SPRITE_TILES_BANK31 + 0x0f260);
const struct sprite_frame gSpriteBank31Frame091 = SPRITE_FRAME(gSpriteBank31Frame091, SPRITE_TILES_BANK31 + 0x0fa20);
const struct sprite_frame gSpriteBank31Frame092 = SPRITE_FRAME(gSpriteBank31Frame092, SPRITE_TILES_BANK31 + 0x10220);
const struct sprite_frame gSpriteBank31Frame093 = SPRITE_FRAME(gSpriteBank31Frame093, SPRITE_TILES_BANK31 + 0x10a20);
const struct sprite_frame_1box gSpriteBank31Frame094 = {
    SPRITE_FRAME(gSpriteBank31Frame094, SPRITE_TILES_BANK31 + 0x11220),
    { { -12, -13, 26, 26 } },
};
const struct sprite_frame_1box gSpriteBank31Frame095 = {
    SPRITE_FRAME(gSpriteBank31Frame095, SPRITE_TILES_BANK31 + 0x11420),
    { { -15, -11, 31, 24 } },
};
const struct sprite_frame_1box gSpriteBank31Frame096 = {
    SPRITE_FRAME(gSpriteBank31Frame096, SPRITE_TILES_BANK31 + 0x11620),
    { { -16, -21, 33, 34 } },
};
const struct sprite_frame_1box gSpriteBank31Frame097 = {
    SPRITE_FRAME(gSpriteBank31Frame097, SPRITE_TILES_BANK31 + 0x118e0),
    { { -15, -29, 31, 42 } },
};
const struct sprite_frame_1box gSpriteBank31Frame098 = {
    SPRITE_FRAME(gSpriteBank31Frame098, SPRITE_TILES_BANK31 + 0x11be0),
    { { -12, -31, 25, 44 } },
};
const struct sprite_frame_1box gSpriteBank31Frame099 = {
    SPRITE_FRAME(gSpriteBank31Frame099, SPRITE_TILES_BANK31 + 0x11ee0),
    { { -12, -35, 25, 48 } },
};
const struct sprite_frame_1box gSpriteBank31Frame100 = {
    SPRITE_FRAME(gSpriteBank31Frame100, SPRITE_TILES_BANK31 + 0x122e0),
    { { -13, -39, 26, 52 } },
};
const struct sprite_frame_1box gSpriteBank31Frame101 = {
    SPRITE_FRAME(gSpriteBank31Frame101, SPRITE_TILES_BANK31 + 0x126e0),
    { { -13, -40, 26, 53 } },
};
const struct sprite_frame_1box gSpriteBank31Frame102 = {
    SPRITE_FRAME(gSpriteBank31Frame102, SPRITE_TILES_BANK31 + 0x12ae0),
    { { -12, -13, 26, 26 } },
};
const struct sprite_frame_1box gSpriteBank31Frame103 = {
    SPRITE_FRAME(gSpriteBank31Frame103, SPRITE_TILES_BANK31 + 0x03620),
    { { -12, -13, 25, 26 } },
};
const struct sprite_frame_1box gSpriteBank31Frame104 = {
    SPRITE_FRAME(gSpriteBank31Frame104, SPRITE_TILES_BANK31 + 0x12ce0),
    { { -15, -11, 31, 24 } },
};
const struct sprite_frame_1box gSpriteBank31Frame105 = {
    SPRITE_FRAME(gSpriteBank31Frame105, SPRITE_TILES_BANK31 + 0x12ee0),
    { { -16, -21, 33, 34 } },
};
const struct sprite_frame_1box gSpriteBank31Frame106 = {
    SPRITE_FRAME(gSpriteBank31Frame106, SPRITE_TILES_BANK31 + 0x131a0),
    { { -15, -29, 31, 42 } },
};
const struct sprite_frame_1box gSpriteBank31Frame107 = {
    SPRITE_FRAME(gSpriteBank31Frame107, SPRITE_TILES_BANK31 + 0x134a0),
    { { -12, -31, 25, 44 } },
};
const struct sprite_frame_1box gSpriteBank31Frame108 = {
    SPRITE_FRAME(gSpriteBank31Frame108, SPRITE_TILES_BANK31 + 0x137a0),
    { { -12, -35, 25, 48 } },
};
const struct sprite_frame_1box gSpriteBank31Frame109 = {
    SPRITE_FRAME(gSpriteBank31Frame109, SPRITE_TILES_BANK31 + 0x13ba0),
    { { -13, -39, 26, 52 } },
};
const struct sprite_frame_1box gSpriteBank31Frame110 = {
    SPRITE_FRAME(gSpriteBank31Frame110, SPRITE_TILES_BANK31 + 0x13fa0),
    { { -13, -40, 26, 53 } },
};
const struct sprite_frame_1box gSpriteBank31Frame111 = {
    SPRITE_FRAME(gSpriteBank31Frame111, SPRITE_TILES_BANK31 + 0x143a0),
    { { -12, -13, 25, 26 } },
};

const struct sprite_piece_pos gSpriteBank31Frame000Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame001Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame002Pos[2] = { { -14, -11 }, { -11, 5 } };
const struct sprite_piece_pos gSpriteBank31Frame003Pos[3] = { { -16, -17 }, { 16, -12 }, { 16, 4 } };
const struct sprite_piece_pos gSpriteBank31Frame004Pos[3] = { { -22, -25 }, { -21, 7 }, { 11, 7 } };
const struct sprite_piece_pos gSpriteBank31Frame005Pos[4] = { { -30, -33 }, { -25, -1 }, { 7, -1 }, { 23, -1 } };
const struct sprite_piece_pos gSpriteBank31Frame006Pos[1] = { { -29, -43 } };
const struct sprite_piece_pos gSpriteBank31Frame007Pos[1] = { { -29, -48 } };
const struct sprite_piece_pos gSpriteBank31Frame008Pos[1] = { { -29, -49 } };
const struct sprite_piece_pos gSpriteBank31Frame009Pos[1] = { { -25, -38 } };
const struct sprite_piece_pos gSpriteBank31Frame010Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame011Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame012Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame013Pos[1] = { { -15, -11 } };
const struct sprite_piece_pos gSpriteBank31Frame014Pos[3] = { { -16, -8 }, { 16, -3 }, { -15, 8 } };
const struct sprite_piece_pos gSpriteBank31Frame015Pos[1] = { { -15, -11 } };
const struct sprite_piece_pos gSpriteBank31Frame016Pos[1] = { { -12, -15 } };
const struct sprite_piece_pos gSpriteBank31Frame017Pos[1] = { { -12, -15 } };
const struct sprite_piece_pos gSpriteBank31Frame018Pos[1] = { { -12, -15 } };
const struct sprite_piece_pos gSpriteBank31Frame019Pos[1] = { { -12, -15 } };
const struct sprite_piece_pos gSpriteBank31Frame020Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame021Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame022Pos[3] = { { -15, -11 }, { 17, -8 }, { 17, 8 } };
const struct sprite_piece_pos gSpriteBank31Frame023Pos[4] = { { -16, -8 }, { 16, -7 }, { -15, 8 }, { 17, 8 } };
const struct sprite_piece_pos gSpriteBank31Frame024Pos[3] = { { -15, -11 }, { 17, -8 }, { 17, 8 } };
const struct sprite_piece_pos gSpriteBank31Frame025Pos[1] = { { -12, -15 } };
const struct sprite_piece_pos gSpriteBank31Frame026Pos[1] = { { -12, -15 } };
const struct sprite_piece_pos gSpriteBank31Frame027Pos[1] = { { -12, -15 } };
const struct sprite_piece_pos gSpriteBank31Frame028Pos[1] = { { -12, -15 } };
const struct sprite_piece_pos gSpriteBank31Frame029Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame030Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame031Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame032Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame033Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame034Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame035Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame036Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame037Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame038Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame039Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame040Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame041Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame042Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame043Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame044Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame045Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame046Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame047Pos[2] = { { -12, -10 }, { -12, 6 } };
const struct sprite_piece_pos gSpriteBank31Frame048Pos[2] = { { -12, -8 }, { -12, 8 } };
const struct sprite_piece_pos gSpriteBank31Frame049Pos[2] = { { -12, -10 }, { -12, 6 } };
const struct sprite_piece_pos gSpriteBank31Frame050Pos[1] = { { -12, -14 } };
const struct sprite_piece_pos gSpriteBank31Frame051Pos[1] = { { -10, -14 } };
const struct sprite_piece_pos gSpriteBank31Frame052Pos[1] = { { -10, -14 } };
const struct sprite_piece_pos gSpriteBank31Frame053Pos[1] = { { -12, -14 } };
const struct sprite_piece_pos gSpriteBank31Frame054Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame055Pos[2] = { { -14, -9 }, { -13, 7 } };
const struct sprite_piece_pos gSpriteBank31Frame056Pos[2] = { { -14, -8 }, { -13, 8 } };
const struct sprite_piece_pos gSpriteBank31Frame057Pos[1] = { { -12, -15 } };
const struct sprite_piece_pos gSpriteBank31Frame058Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame059Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame060Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame061Pos[1] = { { -14, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame062Pos[2] = { { -16, -13 }, { 16, -10 } };
const struct sprite_piece_pos gSpriteBank31Frame063Pos[3] = { { -18, -13 }, { 14, -13 }, { 14, 3 } };
const struct sprite_piece_pos gSpriteBank31Frame064Pos[2] = { { -19, -13 }, { 13, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame065Pos[3] = { { -21, -13 }, { 11, -12 }, { 19, -7 } };
const struct sprite_piece_pos gSpriteBank31Frame066Pos[1] = { { -23, -12 } };
const struct sprite_piece_pos gSpriteBank31Frame067Pos[5] = { { -26, -10 }, { 6, -10 }, { 22, -4 }, { -19, 6 }, { 13, 6 } };
const struct sprite_piece_pos gSpriteBank31Frame068Pos[4] = { { -27, -8 }, { 5, -8 }, { 23, -1 }, { -18, 8 } };
const struct sprite_piece_pos gSpriteBank31Frame069Pos[4] = { { -30, -3 }, { 2, -3 }, { -13, 13 }, { 2, 13 } };
const struct sprite_piece_pos gSpriteBank31Frame070Pos[3] = { { -32, 4 }, { 0, 4 }, { 32, 9 } };
const struct sprite_piece_pos gSpriteBank31Frame071Pos[5] = { { -31, 2 }, { 1, 2 }, { 33, 10 }, { -17, 18 }, { -1, 18 } };
const struct sprite_piece_pos gSpriteBank31Frame072Pos[3] = { { -32, 1 }, { 0, 1 }, { 32, 8 } };
const struct sprite_piece_pos gSpriteBank31Frame073Pos[5] = { { -32, 3 }, { 0, 3 }, { 32, 9 }, { -14, 19 }, { 0, 19 } };
const struct sprite_piece_pos gSpriteBank31Frame074Pos[1] = { { -14, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame075Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame076Pos[3] = { { -7, -6 }, { 9, -2 }, { -6, 10 } };
const struct sprite_piece_pos gSpriteBank31Frame077Pos[2] = { { -16, -17 }, { 16, -1 } };
const struct sprite_piece_pos gSpriteBank31Frame078Pos[4] = { { -21, -24 }, { 11, -23 }, { 19, -3 }, { 8, 8 } };
const struct sprite_piece_pos gSpriteBank31Frame079Pos[4] = { { -30, -33 }, { -25, -1 }, { 11, -1 }, { 23, -1 } };
const struct sprite_piece_pos gSpriteBank31Frame080Pos[1] = { { -28, -43 } };
const struct sprite_piece_pos gSpriteBank31Frame081Pos[1] = { { -29, -48 } };
const struct sprite_piece_pos gSpriteBank31Frame082Pos[1] = { { -29, -49 } };
const struct sprite_piece_pos gSpriteBank31Frame083Pos[1] = { { -25, -38 } };
const struct sprite_piece_pos gSpriteBank31Frame084Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame085Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame086Pos[2] = { { -14, -11 }, { -11, 5 } };
const struct sprite_piece_pos gSpriteBank31Frame087Pos[3] = { { -16, -17 }, { 16, -12 }, { 16, 4 } };
const struct sprite_piece_pos gSpriteBank31Frame088Pos[3] = { { -22, -25 }, { -21, 7 }, { 11, 7 } };
const struct sprite_piece_pos gSpriteBank31Frame089Pos[4] = { { -30, -33 }, { -25, -1 }, { 7, -1 }, { 23, -1 } };
const struct sprite_piece_pos gSpriteBank31Frame090Pos[1] = { { -29, -43 } };
const struct sprite_piece_pos gSpriteBank31Frame091Pos[1] = { { -29, -48 } };
const struct sprite_piece_pos gSpriteBank31Frame092Pos[1] = { { -29, -49 } };
const struct sprite_piece_pos gSpriteBank31Frame093Pos[1] = { { -25, -38 } };
const struct sprite_piece_pos gSpriteBank31Frame094Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame095Pos[1] = { { -15, -11 } };
const struct sprite_piece_pos gSpriteBank31Frame096Pos[3] = { { -16, -21 }, { 16, -3 }, { -14, 11 } };
const struct sprite_piece_pos gSpriteBank31Frame097Pos[2] = { { -15, -29 }, { -15, 3 } };
const struct sprite_piece_pos gSpriteBank31Frame098Pos[2] = { { -12, -31 }, { -12, 1 } };
const struct sprite_piece_pos gSpriteBank31Frame099Pos[1] = { { -12, -35 } };
const struct sprite_piece_pos gSpriteBank31Frame100Pos[1] = { { -13, -39 } };
const struct sprite_piece_pos gSpriteBank31Frame101Pos[1] = { { -13, -40 } };
const struct sprite_piece_pos gSpriteBank31Frame102Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame103Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank31Frame104Pos[1] = { { -15, -11 } };
const struct sprite_piece_pos gSpriteBank31Frame105Pos[3] = { { -16, -21 }, { 16, -3 }, { -14, 11 } };
const struct sprite_piece_pos gSpriteBank31Frame106Pos[2] = { { -15, -29 }, { -15, 3 } };
const struct sprite_piece_pos gSpriteBank31Frame107Pos[2] = { { -12, -31 }, { -12, 1 } };
const struct sprite_piece_pos gSpriteBank31Frame108Pos[1] = { { -12, -35 } };
const struct sprite_piece_pos gSpriteBank31Frame109Pos[1] = { { -13, -39 } };
const struct sprite_piece_pos gSpriteBank31Frame110Pos[1] = { { -13, -40 } };
const struct sprite_piece_pos gSpriteBank31Frame111Pos[1] = { { -12, -13 } };

const u8 gSpriteBank31Frame000Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank31Frame001Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame002Pieces[2] = { SPRITE_PIECE(1, 6), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank31Frame003Pieces[3] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank31Frame004Pieces[3] = { SPRITE_PIECE(1, 7), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank31Frame005Pieces[4] = { SPRITE_PIECE(1, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank31Frame006Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank31Frame007Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank31Frame008Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank31Frame009Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank31Frame010Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank31Frame011Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank31Frame012Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame013Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame014Pieces[3] = { SPRITE_PIECE(1, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank31Frame015Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame016Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame017Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame018Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame019Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame020Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank31Frame021Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame022Pieces[3] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank31Frame023Pieces[4] = { SPRITE_PIECE(1, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank31Frame024Pieces[3] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank31Frame025Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame026Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame027Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame028Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame029Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame030Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame031Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame032Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame033Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame034Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame035Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank31Frame036Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame037Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame038Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame039Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame040Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame041Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame042Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame043Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame044Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank31Frame045Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame046Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame047Pieces[2] = { SPRITE_PIECE(1, 6), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank31Frame048Pieces[2] = { SPRITE_PIECE(1, 6), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank31Frame049Pieces[2] = { SPRITE_PIECE(1, 6), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank31Frame050Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame051Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame052Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame053Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame054Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank31Frame055Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank31Frame056Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank31Frame057Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank31Frame058Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank31Frame059Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame060Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame061Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame062Pieces[2] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank31Frame063Pieces[3] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank31Frame064Pieces[2] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank31Frame065Pieces[3] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank31Frame066Pieces[1] = { SPRITE_PIECE(1, 7) };
const u8 gSpriteBank31Frame067Pieces[5] = { SPRITE_PIECE(1, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank31Frame068Pieces[4] = { SPRITE_PIECE(1, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank31Frame069Pieces[4] = { SPRITE_PIECE(1, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank31Frame070Pieces[3] = { SPRITE_PIECE(1, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank31Frame071Pieces[5] = { SPRITE_PIECE(1, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank31Frame072Pieces[3] = { SPRITE_PIECE(1, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank31Frame073Pieces[5] = { SPRITE_PIECE(1, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank31Frame074Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame075Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame076Pieces[3] = { SPRITE_PIECE(1, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank31Frame077Pieces[2] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank31Frame078Pieces[4] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank31Frame079Pieces[4] = { SPRITE_PIECE(1, 7), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank31Frame080Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank31Frame081Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank31Frame082Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank31Frame083Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank31Frame084Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame085Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank31Frame086Pieces[2] = { SPRITE_PIECE(1, 6), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank31Frame087Pieces[3] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank31Frame088Pieces[3] = { SPRITE_PIECE(1, 7), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank31Frame089Pieces[4] = { SPRITE_PIECE(1, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank31Frame090Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank31Frame091Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank31Frame092Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank31Frame093Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank31Frame094Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank31Frame095Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank31Frame096Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank31Frame097Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank31Frame098Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank31Frame099Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank31Frame100Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank31Frame101Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank31Frame102Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank31Frame103Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank31Frame104Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank31Frame105Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank31Frame106Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank31Frame107Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank31Frame108Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank31Frame109Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank31Frame110Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank31Frame111Pieces[1] = { SPRITE_PIECE(2, 2) };

/* ---------------------------------------------------------------------- */
/* Bank 32: 9 animations, 17 frames, tiles in gSpriteBank32Tiles (SPRITE_TILES_BANK32). */

extern const u16 gSpriteBank32Anim00Seq[8];
extern const u16 gSpriteBank32Anim01Seq[8];
extern const u16 gSpriteBank32Anim02Seq[8];
extern const u16 gSpriteBank32Anim03Seq[8];
extern const u16 gSpriteBank32Anim04Seq[8];
extern const u16 gSpriteBank32Anim05Seq[1];
extern const u16 gSpriteBank32Anim06Seq[1];
extern const u16 gSpriteBank32Anim07Seq[1];
extern const u16 gSpriteBank32Anim08Seq[1];
extern const struct sprite_frame_1box gSpriteBank32Frame000;
extern const struct sprite_frame_1box gSpriteBank32Frame001;
extern const struct sprite_frame_1box gSpriteBank32Frame002;
extern const struct sprite_frame_1box gSpriteBank32Frame003;
extern const struct sprite_frame_1box gSpriteBank32Frame004;
extern const struct sprite_frame_1box gSpriteBank32Frame005;
extern const struct sprite_frame_1box gSpriteBank32Frame006;
extern const struct sprite_frame_1box gSpriteBank32Frame007;
extern const struct sprite_frame gSpriteBank32Frame008;
extern const struct sprite_frame gSpriteBank32Frame009;
extern const struct sprite_frame gSpriteBank32Frame010;
extern const struct sprite_frame gSpriteBank32Frame011;
extern const struct sprite_frame gSpriteBank32Frame012;
extern const struct sprite_frame gSpriteBank32Frame013;
extern const struct sprite_frame gSpriteBank32Frame014;
extern const struct sprite_frame gSpriteBank32Frame015;
extern const struct sprite_frame_1box gSpriteBank32Frame016;
extern const struct sprite_piece_pos gSpriteBank32Frame000Pos[2];
extern const struct sprite_piece_pos gSpriteBank32Frame001Pos[2];
extern const struct sprite_piece_pos gSpriteBank32Frame002Pos[2];
extern const struct sprite_piece_pos gSpriteBank32Frame003Pos[2];
extern const struct sprite_piece_pos gSpriteBank32Frame004Pos[2];
extern const struct sprite_piece_pos gSpriteBank32Frame005Pos[2];
extern const struct sprite_piece_pos gSpriteBank32Frame006Pos[2];
extern const struct sprite_piece_pos gSpriteBank32Frame007Pos[2];
extern const struct sprite_piece_pos gSpriteBank32Frame008Pos[2];
extern const struct sprite_piece_pos gSpriteBank32Frame009Pos[2];
extern const struct sprite_piece_pos gSpriteBank32Frame010Pos[2];
extern const struct sprite_piece_pos gSpriteBank32Frame011Pos[2];
extern const struct sprite_piece_pos gSpriteBank32Frame012Pos[2];
extern const struct sprite_piece_pos gSpriteBank32Frame013Pos[2];
extern const struct sprite_piece_pos gSpriteBank32Frame014Pos[2];
extern const struct sprite_piece_pos gSpriteBank32Frame015Pos[2];
extern const struct sprite_piece_pos gSpriteBank32Frame016Pos[3];
extern const u8 gSpriteBank32Frame000Pieces[2];
extern const u8 gSpriteBank32Frame001Pieces[2];
extern const u8 gSpriteBank32Frame002Pieces[2];
extern const u8 gSpriteBank32Frame003Pieces[2];
extern const u8 gSpriteBank32Frame004Pieces[2];
extern const u8 gSpriteBank32Frame005Pieces[2];
extern const u8 gSpriteBank32Frame006Pieces[2];
extern const u8 gSpriteBank32Frame007Pieces[2];
extern const u8 gSpriteBank32Frame008Pieces[2];
extern const u8 gSpriteBank32Frame009Pieces[2];
extern const u8 gSpriteBank32Frame010Pieces[2];
extern const u8 gSpriteBank32Frame011Pieces[2];
extern const u8 gSpriteBank32Frame012Pieces[2];
extern const u8 gSpriteBank32Frame013Pieces[2];
extern const u8 gSpriteBank32Frame014Pieces[2];
extern const u8 gSpriteBank32Frame015Pieces[2];
extern const u8 gSpriteBank32Frame016Pieces[3];

const struct sprite_anim gSpriteBank32Anims[9] = {
    [0] = {
        .seq = gSpriteBank32Anim00Seq,
        .box = { { -12, -11, 25, 23 }, { -12, -11, 25, 23 } },
        .paletteId = 49,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank32Anim00Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [1] = {
        .seq = gSpriteBank32Anim01Seq,
        .box = { { -12, -11, 25, 23 }, { -12, -11, 25, 23 } },
        .paletteId = 50,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank32Anim01Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [2] = {
        .seq = gSpriteBank32Anim02Seq,
        .box = { { -12, -11, 25, 23 }, { -12, -11, 25, 23 } },
        .paletteId = 51,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank32Anim02Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [3] = {
        .seq = gSpriteBank32Anim03Seq,
        .box = { { -12, -11, 25, 23 }, { -12, -11, 25, 23 } },
        .paletteId = 52,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank32Anim03Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [4] = {
        .seq = gSpriteBank32Anim04Seq,
        .box = { { -12, -11, 25, 23 }, { -12, -11, 25, 23 } },
        .paletteId = 53,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank32Anim04Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [5] = {
        .seq = gSpriteBank32Anim05Seq,
        .box = { { -20, -14, 41, 28 }, { -20, -14, 41, 28 } },
        .paletteId = 49,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank32Anim05Seq),
        .flags = 0,
    },
    [6] = {
        .seq = gSpriteBank32Anim06Seq,
        .box = { { -20, -14, 41, 28 }, { -20, -14, 41, 28 } },
        .paletteId = 51,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank32Anim06Seq),
        .flags = 0,
    },
    [7] = {
        .seq = gSpriteBank32Anim07Seq,
        .box = { { -20, -14, 41, 28 }, { -20, -14, 41, 28 } },
        .paletteId = 52,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank32Anim07Seq),
        .flags = 0,
    },
    [8] = {
        .seq = gSpriteBank32Anim08Seq,
        .box = { { -20, -14, 41, 28 }, { -20, -14, 41, 28 } },
        .paletteId = 53,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank32Anim08Seq),
        .flags = 0,
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

const struct sprite_frame *const gSpriteBank32Frames[17] = {
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
    SPRITE_FRAME(gSpriteBank32Frame000, SPRITE_TILES_BANK32 + 0x00000),
    { { -12, -11, 25, 23 } },
};
const struct sprite_frame_1box gSpriteBank32Frame001 = {
    SPRITE_FRAME(gSpriteBank32Frame001, SPRITE_TILES_BANK32 + 0x00140),
    { { -12, -11, 25, 23 } },
};
const struct sprite_frame_1box gSpriteBank32Frame002 = {
    SPRITE_FRAME(gSpriteBank32Frame002, SPRITE_TILES_BANK32 + 0x00280),
    { { -12, -11, 25, 23 } },
};
const struct sprite_frame_1box gSpriteBank32Frame003 = {
    SPRITE_FRAME(gSpriteBank32Frame003, SPRITE_TILES_BANK32 + 0x003c0),
    { { -12, -11, 25, 23 } },
};
const struct sprite_frame_1box gSpriteBank32Frame004 = {
    SPRITE_FRAME(gSpriteBank32Frame004, SPRITE_TILES_BANK32 + 0x00500),
    { { -12, -11, 25, 23 } },
};
const struct sprite_frame_1box gSpriteBank32Frame005 = {
    SPRITE_FRAME(gSpriteBank32Frame005, SPRITE_TILES_BANK32 + 0x00640),
    { { -12, -11, 25, 23 } },
};
const struct sprite_frame_1box gSpriteBank32Frame006 = {
    SPRITE_FRAME(gSpriteBank32Frame006, SPRITE_TILES_BANK32 + 0x00780),
    { { -12, -11, 25, 23 } },
};
const struct sprite_frame_1box gSpriteBank32Frame007 = {
    SPRITE_FRAME(gSpriteBank32Frame007, SPRITE_TILES_BANK32 + 0x008c0),
    { { -12, -11, 25, 23 } },
};
const struct sprite_frame gSpriteBank32Frame008 = SPRITE_FRAME(gSpriteBank32Frame008, SPRITE_TILES_BANK32 + 0x00000);
const struct sprite_frame gSpriteBank32Frame009 = SPRITE_FRAME(gSpriteBank32Frame009, SPRITE_TILES_BANK32 + 0x00140);
const struct sprite_frame gSpriteBank32Frame010 = SPRITE_FRAME(gSpriteBank32Frame010, SPRITE_TILES_BANK32 + 0x00280);
const struct sprite_frame gSpriteBank32Frame011 = SPRITE_FRAME(gSpriteBank32Frame011, SPRITE_TILES_BANK32 + 0x003c0);
const struct sprite_frame gSpriteBank32Frame012 = SPRITE_FRAME(gSpriteBank32Frame012, SPRITE_TILES_BANK32 + 0x00500);
const struct sprite_frame gSpriteBank32Frame013 = SPRITE_FRAME(gSpriteBank32Frame013, SPRITE_TILES_BANK32 + 0x00640);
const struct sprite_frame gSpriteBank32Frame014 = SPRITE_FRAME(gSpriteBank32Frame014, SPRITE_TILES_BANK32 + 0x00780);
const struct sprite_frame gSpriteBank32Frame015 = SPRITE_FRAME(gSpriteBank32Frame015, SPRITE_TILES_BANK32 + 0x008c0);
const struct sprite_frame_1box gSpriteBank32Frame016 = {
    SPRITE_FRAME(gSpriteBank32Frame016, SPRITE_TILES_BANK32 + 0x00a00),
    { { -20, -14, 41, 28 } },
};

const struct sprite_piece_pos gSpriteBank32Frame000Pos[2] = { { -12, -11 }, { -7, 5 } };
const struct sprite_piece_pos gSpriteBank32Frame001Pos[2] = { { -12, -11 }, { -6, 5 } };
const struct sprite_piece_pos gSpriteBank32Frame002Pos[2] = { { -12, -11 }, { -7, 5 } };
const struct sprite_piece_pos gSpriteBank32Frame003Pos[2] = { { -12, -11 }, { -6, 5 } };
const struct sprite_piece_pos gSpriteBank32Frame004Pos[2] = { { -12, -11 }, { -7, 5 } };
const struct sprite_piece_pos gSpriteBank32Frame005Pos[2] = { { -12, -11 }, { -6, 5 } };
const struct sprite_piece_pos gSpriteBank32Frame006Pos[2] = { { -12, -11 }, { -6, 5 } };
const struct sprite_piece_pos gSpriteBank32Frame007Pos[2] = { { -12, -11 }, { -6, 5 } };
const struct sprite_piece_pos gSpriteBank32Frame008Pos[2] = { { -12, -11 }, { -7, 5 } };
const struct sprite_piece_pos gSpriteBank32Frame009Pos[2] = { { -12, -11 }, { -6, 5 } };
const struct sprite_piece_pos gSpriteBank32Frame010Pos[2] = { { -12, -11 }, { -7, 5 } };
const struct sprite_piece_pos gSpriteBank32Frame011Pos[2] = { { -12, -11 }, { -6, 5 } };
const struct sprite_piece_pos gSpriteBank32Frame012Pos[2] = { { -12, -11 }, { -7, 5 } };
const struct sprite_piece_pos gSpriteBank32Frame013Pos[2] = { { -12, -11 }, { -6, 5 } };
const struct sprite_piece_pos gSpriteBank32Frame014Pos[2] = { { -12, -11 }, { -6, 5 } };
const struct sprite_piece_pos gSpriteBank32Frame015Pos[2] = { { -12, -11 }, { -6, 5 } };
const struct sprite_piece_pos gSpriteBank32Frame016Pos[3] = { { -20, -14 }, { 12, -11 }, { 20, -6 } };

const u8 gSpriteBank32Frame000Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank32Frame001Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank32Frame002Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank32Frame003Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank32Frame004Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank32Frame005Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank32Frame006Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank32Frame007Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank32Frame008Pieces[2] = { SPRITE_PIECE(1, 6), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank32Frame009Pieces[2] = { SPRITE_PIECE(1, 6), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank32Frame010Pieces[2] = { SPRITE_PIECE(1, 6), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank32Frame011Pieces[2] = { SPRITE_PIECE(1, 6), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank32Frame012Pieces[2] = { SPRITE_PIECE(1, 6), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank32Frame013Pieces[2] = { SPRITE_PIECE(1, 6), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank32Frame014Pieces[2] = { SPRITE_PIECE(1, 6), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank32Frame015Pieces[2] = { SPRITE_PIECE(1, 6), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank32Frame016Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };

/* ---------------------------------------------------------------------- */
/* Bank 33: 3 animations, 16 frames, tiles in gSpriteBank33Tiles (SPRITE_TILES_BANK33). */

extern const u16 gSpriteBank33Anim00Seq[8];
extern const u16 gSpriteBank33Anim01Seq[8];
extern const u16 gSpriteBank33Anim02Seq[8];
extern const struct sprite_frame_1box gSpriteBank33Frame000;
extern const struct sprite_frame_1box gSpriteBank33Frame001;
extern const struct sprite_frame_1box gSpriteBank33Frame002;
extern const struct sprite_frame_1box gSpriteBank33Frame003;
extern const struct sprite_frame_1box gSpriteBank33Frame004;
extern const struct sprite_frame_1box gSpriteBank33Frame005;
extern const struct sprite_frame_1box gSpriteBank33Frame006;
extern const struct sprite_frame_1box gSpriteBank33Frame007;
extern const struct sprite_frame_1box gSpriteBank33Frame008;
extern const struct sprite_frame_1box gSpriteBank33Frame009;
extern const struct sprite_frame_1box gSpriteBank33Frame010;
extern const struct sprite_frame_1box gSpriteBank33Frame011;
extern const struct sprite_frame_1box gSpriteBank33Frame012;
extern const struct sprite_frame_1box gSpriteBank33Frame013;
extern const struct sprite_frame_1box gSpriteBank33Frame014;
extern const struct sprite_frame_1box gSpriteBank33Frame015;
extern const struct sprite_piece_pos gSpriteBank33Frame000Pos[2];
extern const struct sprite_piece_pos gSpriteBank33Frame001Pos[2];
extern const struct sprite_piece_pos gSpriteBank33Frame002Pos[2];
extern const struct sprite_piece_pos gSpriteBank33Frame003Pos[4];
extern const struct sprite_piece_pos gSpriteBank33Frame004Pos[3];
extern const struct sprite_piece_pos gSpriteBank33Frame005Pos[2];
extern const struct sprite_piece_pos gSpriteBank33Frame006Pos[3];
extern const struct sprite_piece_pos gSpriteBank33Frame007Pos[2];
extern const struct sprite_piece_pos gSpriteBank33Frame008Pos[2];
extern const struct sprite_piece_pos gSpriteBank33Frame009Pos[2];
extern const struct sprite_piece_pos gSpriteBank33Frame010Pos[2];
extern const struct sprite_piece_pos gSpriteBank33Frame011Pos[4];
extern const struct sprite_piece_pos gSpriteBank33Frame012Pos[3];
extern const struct sprite_piece_pos gSpriteBank33Frame013Pos[2];
extern const struct sprite_piece_pos gSpriteBank33Frame014Pos[3];
extern const struct sprite_piece_pos gSpriteBank33Frame015Pos[2];
extern const u8 gSpriteBank33Frame000Pieces[2];
extern const u8 gSpriteBank33Frame001Pieces[2];
extern const u8 gSpriteBank33Frame002Pieces[2];
extern const u8 gSpriteBank33Frame003Pieces[4];
extern const u8 gSpriteBank33Frame004Pieces[3];
extern const u8 gSpriteBank33Frame005Pieces[2];
extern const u8 gSpriteBank33Frame006Pieces[3];
extern const u8 gSpriteBank33Frame007Pieces[2];
extern const u8 gSpriteBank33Frame008Pieces[2];
extern const u8 gSpriteBank33Frame009Pieces[2];
extern const u8 gSpriteBank33Frame010Pieces[2];
extern const u8 gSpriteBank33Frame011Pieces[4];
extern const u8 gSpriteBank33Frame012Pieces[3];
extern const u8 gSpriteBank33Frame013Pieces[2];
extern const u8 gSpriteBank33Frame014Pieces[3];
extern const u8 gSpriteBank33Frame015Pieces[2];

const struct sprite_anim gSpriteBank33Anims[3] = {
    [0] = {
        .seq = gSpriteBank33Anim00Seq,
        .box = { { -14, -22, 28, 45 }, { -15, -22, 29, 45 } },
        .paletteId = 54,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank33Anim00Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [1] = {
        .seq = gSpriteBank33Anim01Seq,
        .box = { { -14, -22, 28, 45 }, { -15, -22, 29, 45 } },
        .paletteId = 55,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank33Anim01Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [2] = {
        .seq = gSpriteBank33Anim02Seq,
        .box = { { -14, -22, 28, 45 }, { -15, -22, 29, 45 } },
        .paletteId = 56,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank33Anim02Seq),
        .flags = SPRITE_ANIM_LOOP,
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

const struct sprite_frame *const gSpriteBank33Frames[16] = {
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
    SPRITE_FRAME(gSpriteBank33Frame000, SPRITE_TILES_BANK33 + 0x00000),
    { { -14, -22, 28, 45 } },
};
const struct sprite_frame_1box gSpriteBank33Frame001 = {
    SPRITE_FRAME(gSpriteBank33Frame001, SPRITE_TILES_BANK33 + 0x00240),
    { { -15, -22, 29, 45 } },
};
const struct sprite_frame_1box gSpriteBank33Frame002 = {
    SPRITE_FRAME(gSpriteBank33Frame002, SPRITE_TILES_BANK33 + 0x00480),
    { { -15, -22, 27, 45 } },
};
const struct sprite_frame_1box gSpriteBank33Frame003 = {
    SPRITE_FRAME(gSpriteBank33Frame003, SPRITE_TILES_BANK33 + 0x006c0),
    { { -12, -22, 21, 45 } },
};
const struct sprite_frame_1box gSpriteBank33Frame004 = {
    SPRITE_FRAME(gSpriteBank33Frame004, SPRITE_TILES_BANK33 + 0x00860),
    { { -8, -22, 13, 44 } },
};
const struct sprite_frame_1box gSpriteBank33Frame005 = {
    SPRITE_FRAME(gSpriteBank33Frame005, SPRITE_TILES_BANK33 + 0x009a0),
    { { -3, -22, 7, 44 } },
};
const struct sprite_frame_1box gSpriteBank33Frame006 = {
    SPRITE_FRAME(gSpriteBank33Frame006, SPRITE_TILES_BANK33 + 0x00a60),
    { { -8, -22, 17, 45 } },
};
const struct sprite_frame_1box gSpriteBank33Frame007 = {
    SPRITE_FRAME(gSpriteBank33Frame007, SPRITE_TILES_BANK33 + 0x00bc0),
    { { -12, -22, 25, 45 } },
};
const struct sprite_frame_1box gSpriteBank33Frame008 = {
    SPRITE_FRAME(gSpriteBank33Frame008, SPRITE_TILES_BANK33 + 0x00e00),
    { { -14, -22, 28, 45 } },
};
const struct sprite_frame_1box gSpriteBank33Frame009 = {
    SPRITE_FRAME(gSpriteBank33Frame009, SPRITE_TILES_BANK33 + 0x01040),
    { { -15, -22, 29, 45 } },
};
const struct sprite_frame_1box gSpriteBank33Frame010 = {
    SPRITE_FRAME(gSpriteBank33Frame010, SPRITE_TILES_BANK33 + 0x01280),
    { { -15, -22, 27, 45 } },
};
const struct sprite_frame_1box gSpriteBank33Frame011 = {
    SPRITE_FRAME(gSpriteBank33Frame011, SPRITE_TILES_BANK33 + 0x014c0),
    { { -12, -22, 21, 45 } },
};
const struct sprite_frame_1box gSpriteBank33Frame012 = {
    SPRITE_FRAME(gSpriteBank33Frame012, SPRITE_TILES_BANK33 + 0x01660),
    { { -8, -22, 13, 44 } },
};
const struct sprite_frame_1box gSpriteBank33Frame013 = {
    SPRITE_FRAME(gSpriteBank33Frame013, SPRITE_TILES_BANK33 + 0x017a0),
    { { -3, -22, 7, 44 } },
};
const struct sprite_frame_1box gSpriteBank33Frame014 = {
    SPRITE_FRAME(gSpriteBank33Frame014, SPRITE_TILES_BANK33 + 0x01860),
    { { -8, -22, 17, 45 } },
};
const struct sprite_frame_1box gSpriteBank33Frame015 = {
    SPRITE_FRAME(gSpriteBank33Frame015, SPRITE_TILES_BANK33 + 0x019c0),
    { { -12, -22, 25, 45 } },
};

const struct sprite_piece_pos gSpriteBank33Frame000Pos[2] = { { -14, -22 }, { -4, 10 } };
const struct sprite_piece_pos gSpriteBank33Frame001Pos[2] = { { -15, -22 }, { -4, 10 } };
const struct sprite_piece_pos gSpriteBank33Frame002Pos[2] = { { -15, -22 }, { -4, 10 } };
const struct sprite_piece_pos gSpriteBank33Frame003Pos[4] = { { -12, -22 }, { 4, -20 }, { 4, -4 }, { -3, 10 } };
const struct sprite_piece_pos gSpriteBank33Frame004Pos[3] = { { -8, -22 }, { 0, -22 }, { -3, 10 } };
const struct sprite_piece_pos gSpriteBank33Frame005Pos[2] = { { -3, -22 }, { -3, 10 } };
const struct sprite_piece_pos gSpriteBank33Frame006Pos[3] = { { -8, -22 }, { 8, -1 }, { -2, 10 } };
const struct sprite_piece_pos gSpriteBank33Frame007Pos[2] = { { -12, -22 }, { -3, 10 } };
const struct sprite_piece_pos gSpriteBank33Frame008Pos[2] = { { -14, -22 }, { -4, 10 } };
const struct sprite_piece_pos gSpriteBank33Frame009Pos[2] = { { -15, -22 }, { -4, 10 } };
const struct sprite_piece_pos gSpriteBank33Frame010Pos[2] = { { -15, -22 }, { -4, 10 } };
const struct sprite_piece_pos gSpriteBank33Frame011Pos[4] = { { -12, -22 }, { 4, -20 }, { 4, -4 }, { -3, 10 } };
const struct sprite_piece_pos gSpriteBank33Frame012Pos[3] = { { -8, -22 }, { 0, -22 }, { -3, 10 } };
const struct sprite_piece_pos gSpriteBank33Frame013Pos[2] = { { -3, -22 }, { -3, 10 } };
const struct sprite_piece_pos gSpriteBank33Frame014Pos[3] = { { -8, -22 }, { 8, -1 }, { -2, 10 } };
const struct sprite_piece_pos gSpriteBank33Frame015Pos[2] = { { -12, -22 }, { -3, 10 } };

const u8 gSpriteBank33Frame000Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank33Frame001Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank33Frame002Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank33Frame003Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank33Frame004Pieces[3] = { SPRITE_PIECE(2, 9), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank33Frame005Pieces[2] = { SPRITE_PIECE(2, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank33Frame006Pieces[3] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank33Frame007Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank33Frame008Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank33Frame009Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank33Frame010Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank33Frame011Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank33Frame012Pieces[3] = { SPRITE_PIECE(2, 9), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank33Frame013Pieces[2] = { SPRITE_PIECE(2, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank33Frame014Pieces[3] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank33Frame015Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8) };

/* ---------------------------------------------------------------------- */
/* Bank 34: 5 animations, 22 frames, tiles in gSpriteBank34Tiles (SPRITE_TILES_BANK34). */

extern const u16 gSpriteBank34Anim00Seq[4];
extern const u16 gSpriteBank34Anim01Seq[4];
extern const u16 gSpriteBank34Anim02Seq[4];
extern const u16 gSpriteBank34Anim03Seq[5];
extern const u16 gSpriteBank34Anim04Seq[5];
extern const struct sprite_frame_1box gSpriteBank34Frame000;
extern const struct sprite_frame_1box gSpriteBank34Frame001;
extern const struct sprite_frame_1box gSpriteBank34Frame002;
extern const struct sprite_frame_1box gSpriteBank34Frame003;
extern const struct sprite_frame_1box gSpriteBank34Frame004;
extern const struct sprite_frame_1box gSpriteBank34Frame005;
extern const struct sprite_frame_1box gSpriteBank34Frame006;
extern const struct sprite_frame_1box gSpriteBank34Frame007;
extern const struct sprite_frame_1box gSpriteBank34Frame008;
extern const struct sprite_frame_1box gSpriteBank34Frame009;
extern const struct sprite_frame_1box gSpriteBank34Frame010;
extern const struct sprite_frame_1box gSpriteBank34Frame011;
extern const struct sprite_frame_1box gSpriteBank34Frame012;
extern const struct sprite_frame_1box gSpriteBank34Frame013;
extern const struct sprite_frame_1box gSpriteBank34Frame014;
extern const struct sprite_frame_1box gSpriteBank34Frame015;
extern const struct sprite_frame_1box gSpriteBank34Frame016;
extern const struct sprite_frame_1box gSpriteBank34Frame017;
extern const struct sprite_frame_1box gSpriteBank34Frame018;
extern const struct sprite_frame_1box gSpriteBank34Frame019;
extern const struct sprite_frame_1box gSpriteBank34Frame020;
extern const struct sprite_frame_1box gSpriteBank34Frame021;
extern const struct sprite_piece_pos gSpriteBank34Frame000Pos[3];
extern const struct sprite_piece_pos gSpriteBank34Frame001Pos[2];
extern const struct sprite_piece_pos gSpriteBank34Frame002Pos[2];
extern const struct sprite_piece_pos gSpriteBank34Frame003Pos[3];
extern const struct sprite_piece_pos gSpriteBank34Frame004Pos[3];
extern const struct sprite_piece_pos gSpriteBank34Frame005Pos[2];
extern const struct sprite_piece_pos gSpriteBank34Frame006Pos[2];
extern const struct sprite_piece_pos gSpriteBank34Frame007Pos[3];
extern const struct sprite_piece_pos gSpriteBank34Frame008Pos[3];
extern const struct sprite_piece_pos gSpriteBank34Frame009Pos[2];
extern const struct sprite_piece_pos gSpriteBank34Frame010Pos[2];
extern const struct sprite_piece_pos gSpriteBank34Frame011Pos[3];
extern const struct sprite_piece_pos gSpriteBank34Frame012Pos[2];
extern const struct sprite_piece_pos gSpriteBank34Frame013Pos[1];
extern const struct sprite_piece_pos gSpriteBank34Frame014Pos[1];
extern const struct sprite_piece_pos gSpriteBank34Frame015Pos[1];
extern const struct sprite_piece_pos gSpriteBank34Frame016Pos[2];
extern const struct sprite_piece_pos gSpriteBank34Frame017Pos[2];
extern const struct sprite_piece_pos gSpriteBank34Frame018Pos[1];
extern const struct sprite_piece_pos gSpriteBank34Frame019Pos[1];
extern const struct sprite_piece_pos gSpriteBank34Frame020Pos[1];
extern const struct sprite_piece_pos gSpriteBank34Frame021Pos[2];
extern const u8 gSpriteBank34Frame000Pieces[3];
extern const u8 gSpriteBank34Frame001Pieces[2];
extern const u8 gSpriteBank34Frame002Pieces[2];
extern const u8 gSpriteBank34Frame003Pieces[3];
extern const u8 gSpriteBank34Frame004Pieces[3];
extern const u8 gSpriteBank34Frame005Pieces[2];
extern const u8 gSpriteBank34Frame006Pieces[2];
extern const u8 gSpriteBank34Frame007Pieces[3];
extern const u8 gSpriteBank34Frame008Pieces[3];
extern const u8 gSpriteBank34Frame009Pieces[2];
extern const u8 gSpriteBank34Frame010Pieces[2];
extern const u8 gSpriteBank34Frame011Pieces[3];
extern const u8 gSpriteBank34Frame012Pieces[2];
extern const u8 gSpriteBank34Frame013Pieces[1];
extern const u8 gSpriteBank34Frame014Pieces[1];
extern const u8 gSpriteBank34Frame015Pieces[1];
extern const u8 gSpriteBank34Frame016Pieces[2];
extern const u8 gSpriteBank34Frame017Pieces[2];
extern const u8 gSpriteBank34Frame018Pieces[1];
extern const u8 gSpriteBank34Frame019Pieces[1];
extern const u8 gSpriteBank34Frame020Pieces[1];
extern const u8 gSpriteBank34Frame021Pieces[2];

const struct sprite_anim gSpriteBank34Anims[5] = {
    [0] = {
        .seq = gSpriteBank34Anim00Seq,
        .box = { { -4, -9, 9, 19 }, { -8, -11, 15, 21 } },
        .paletteId = 57,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank34Anim00Seq),
        .flags = 0,
    },
    [1] = {
        .seq = gSpriteBank34Anim01Seq,
        .box = { { -4, -9, 9, 19 }, { -8, -11, 15, 21 } },
        .paletteId = 57,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank34Anim01Seq),
        .flags = 0,
    },
    [2] = {
        .seq = gSpriteBank34Anim02Seq,
        .box = { { -4, -9, 9, 19 }, { -8, -11, 15, 21 } },
        .paletteId = 57,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank34Anim02Seq),
        .flags = 0,
    },
    [3] = {
        .seq = gSpriteBank34Anim03Seq,
        .box = { { -8, -4, 16, 9 }, { -15, -13, 30, 30 } },
        .paletteId = 57,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank34Anim03Seq),
        .flags = 0,
    },
    [4] = {
        .seq = gSpriteBank34Anim04Seq,
        .box = { { -8, -4, 16, 9 }, { -15, -13, 30, 30 } },
        .paletteId = 57,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank34Anim04Seq),
        .flags = 0,
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

const struct sprite_frame *const gSpriteBank34Frames[22] = {
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
    SPRITE_FRAME(gSpriteBank34Frame000, SPRITE_TILES_BANK34 + 0x00000),
    { { -4, -9, 9, 19 } },
};
const struct sprite_frame_1box gSpriteBank34Frame001 = {
    SPRITE_FRAME(gSpriteBank34Frame001, SPRITE_TILES_BANK34 + 0x00080),
    { { -5, -9, 12, 19 } },
};
const struct sprite_frame_1box gSpriteBank34Frame002 = {
    SPRITE_FRAME(gSpriteBank34Frame002, SPRITE_TILES_BANK34 + 0x00120),
    { { -8, -11, 12, 21 } },
};
const struct sprite_frame_1box gSpriteBank34Frame003 = {
    SPRITE_FRAME(gSpriteBank34Frame003, SPRITE_TILES_BANK34 + 0x001c0),
    { { -8, -11, 10, 21 } },
};
const struct sprite_frame_1box gSpriteBank34Frame004 = {
    SPRITE_FRAME(gSpriteBank34Frame004, SPRITE_TILES_BANK34 + 0x00260),
    { { -4, -9, 9, 19 } },
};
const struct sprite_frame_1box gSpriteBank34Frame005 = {
    SPRITE_FRAME(gSpriteBank34Frame005, SPRITE_TILES_BANK34 + 0x002e0),
    { { -5, -9, 12, 19 } },
};
const struct sprite_frame_1box gSpriteBank34Frame006 = {
    SPRITE_FRAME(gSpriteBank34Frame006, SPRITE_TILES_BANK34 + 0x00380),
    { { -8, -11, 12, 21 } },
};
const struct sprite_frame_1box gSpriteBank34Frame007 = {
    SPRITE_FRAME(gSpriteBank34Frame007, SPRITE_TILES_BANK34 + 0x00420),
    { { -8, -11, 10, 21 } },
};
const struct sprite_frame_1box gSpriteBank34Frame008 = {
    SPRITE_FRAME(gSpriteBank34Frame008, SPRITE_TILES_BANK34 + 0x004c0),
    { { -4, -9, 9, 19 } },
};
const struct sprite_frame_1box gSpriteBank34Frame009 = {
    SPRITE_FRAME(gSpriteBank34Frame009, SPRITE_TILES_BANK34 + 0x00540),
    { { -5, -9, 12, 19 } },
};
const struct sprite_frame_1box gSpriteBank34Frame010 = {
    SPRITE_FRAME(gSpriteBank34Frame010, SPRITE_TILES_BANK34 + 0x005e0),
    { { -8, -11, 12, 21 } },
};
const struct sprite_frame_1box gSpriteBank34Frame011 = {
    SPRITE_FRAME(gSpriteBank34Frame011, SPRITE_TILES_BANK34 + 0x00680),
    { { -8, -11, 10, 21 } },
};
const struct sprite_frame_1box gSpriteBank34Frame012 = {
    SPRITE_FRAME(gSpriteBank34Frame012, SPRITE_TILES_BANK34 + 0x00720),
    { { -8, -4, 16, 9 } },
};
const struct sprite_frame_1box gSpriteBank34Frame013 = {
    SPRITE_FRAME(gSpriteBank34Frame013, SPRITE_TILES_BANK34 + 0x007c0),
    { { -11, 1, 22, 10 } },
};
const struct sprite_frame_1box gSpriteBank34Frame014 = {
    SPRITE_FRAME(gSpriteBank34Frame014, SPRITE_TILES_BANK34 + 0x008c0),
    { { -10, 7, 19, 7 } },
};
const struct sprite_frame_1box gSpriteBank34Frame015 = {
    SPRITE_FRAME(gSpriteBank34Frame015, SPRITE_TILES_BANK34 + 0x009c0),
    { { -8, 10, 19, 8 } },
};
const struct sprite_frame_1box gSpriteBank34Frame016 = {
    SPRITE_FRAME(gSpriteBank34Frame016, SPRITE_TILES_BANK34 + 0x00ac0),
    { { -8, 13, 15, 4 } },
};
const struct sprite_frame_1box gSpriteBank34Frame017 = {
    SPRITE_FRAME(gSpriteBank34Frame017, SPRITE_TILES_BANK34 + 0x00b80),
    { { -8, -4, 16, 9 } },
};
const struct sprite_frame_1box gSpriteBank34Frame018 = {
    SPRITE_FRAME(gSpriteBank34Frame018, SPRITE_TILES_BANK34 + 0x00c20),
    { { -11, 1, 22, 10 } },
};
const struct sprite_frame_1box gSpriteBank34Frame019 = {
    SPRITE_FRAME(gSpriteBank34Frame019, SPRITE_TILES_BANK34 + 0x00d20),
    { { -10, 7, 19, 7 } },
};
const struct sprite_frame_1box gSpriteBank34Frame020 = {
    SPRITE_FRAME(gSpriteBank34Frame020, SPRITE_TILES_BANK34 + 0x00e20),
    { { -8, 10, 19, 8 } },
};
const struct sprite_frame_1box gSpriteBank34Frame021 = {
    SPRITE_FRAME(gSpriteBank34Frame021, SPRITE_TILES_BANK34 + 0x00f20),
    { { -8, 13, 15, 4 } },
};

const struct sprite_piece_pos gSpriteBank34Frame000Pos[3] = { { -4, -9 }, { 4, -8 }, { -2, 7 } };
const struct sprite_piece_pos gSpriteBank34Frame001Pos[2] = { { -5, -9 }, { -3, 7 } };
const struct sprite_piece_pos gSpriteBank34Frame002Pos[2] = { { -8, -11 }, { -4, 5 } };
const struct sprite_piece_pos gSpriteBank34Frame003Pos[3] = { { -8, -11 }, { 0, -8 }, { -4, 5 } };
const struct sprite_piece_pos gSpriteBank34Frame004Pos[3] = { { -4, -9 }, { 4, -8 }, { -2, 7 } };
const struct sprite_piece_pos gSpriteBank34Frame005Pos[2] = { { -5, -9 }, { -3, 7 } };
const struct sprite_piece_pos gSpriteBank34Frame006Pos[2] = { { -8, -11 }, { -4, 5 } };
const struct sprite_piece_pos gSpriteBank34Frame007Pos[3] = { { -8, -11 }, { 0, -8 }, { -4, 5 } };
const struct sprite_piece_pos gSpriteBank34Frame008Pos[3] = { { -4, -9 }, { 4, -8 }, { -2, 7 } };
const struct sprite_piece_pos gSpriteBank34Frame009Pos[2] = { { -5, -9 }, { -3, 7 } };
const struct sprite_piece_pos gSpriteBank34Frame010Pos[2] = { { -8, -11 }, { -4, 5 } };
const struct sprite_piece_pos gSpriteBank34Frame011Pos[3] = { { -8, -11 }, { 0, -8 }, { -4, 5 } };
const struct sprite_piece_pos gSpriteBank34Frame012Pos[2] = { { -9, -13 }, { 7, -8 } };
const struct sprite_piece_pos gSpriteBank34Frame013Pos[1] = { { -15, -13 } };
const struct sprite_piece_pos gSpriteBank34Frame014Pos[1] = { { -10, -5 } };
const struct sprite_piece_pos gSpriteBank34Frame015Pos[1] = { { -13, 1 } };
const struct sprite_piece_pos gSpriteBank34Frame016Pos[2] = { { -14, 6 }, { 2, 4 } };
const struct sprite_piece_pos gSpriteBank34Frame017Pos[2] = { { -9, -13 }, { 7, -8 } };
const struct sprite_piece_pos gSpriteBank34Frame018Pos[1] = { { -15, -13 } };
const struct sprite_piece_pos gSpriteBank34Frame019Pos[1] = { { -10, -5 } };
const struct sprite_piece_pos gSpriteBank34Frame020Pos[1] = { { -13, 1 } };
const struct sprite_piece_pos gSpriteBank34Frame021Pos[2] = { { -14, 6 }, { 2, 4 } };

const u8 gSpriteBank34Frame000Pieces[3] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank34Frame001Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank34Frame002Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank34Frame003Pieces[3] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank34Frame004Pieces[3] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank34Frame005Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank34Frame006Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank34Frame007Pieces[3] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank34Frame008Pieces[3] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank34Frame009Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank34Frame010Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank34Frame011Pieces[3] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank34Frame012Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank34Frame013Pieces[1] = { SPRITE_PIECE(2, 6) };
const u8 gSpriteBank34Frame014Pieces[1] = { SPRITE_PIECE(2, 6) };
const u8 gSpriteBank34Frame015Pieces[1] = { SPRITE_PIECE(2, 6) };
const u8 gSpriteBank34Frame016Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank34Frame017Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank34Frame018Pieces[1] = { SPRITE_PIECE(2, 6) };
const u8 gSpriteBank34Frame019Pieces[1] = { SPRITE_PIECE(2, 6) };
const u8 gSpriteBank34Frame020Pieces[1] = { SPRITE_PIECE(2, 6) };
const u8 gSpriteBank34Frame021Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8) };

/* ---------------------------------------------------------------------- */
/* Bank 35: 2 animations, 28 frames, tiles in gSpriteBank35Tiles (SPRITE_TILES_BANK35). */

extern const u16 gSpriteBank35Anim00Seq[14];
extern const u16 gSpriteBank35Anim01Seq[14];
extern const struct sprite_frame gSpriteBank35Frame000;
extern const struct sprite_frame gSpriteBank35Frame001;
extern const struct sprite_frame gSpriteBank35Frame002;
extern const struct sprite_frame gSpriteBank35Frame003;
extern const struct sprite_frame gSpriteBank35Frame004;
extern const struct sprite_frame gSpriteBank35Frame005;
extern const struct sprite_frame gSpriteBank35Frame006;
extern const struct sprite_frame gSpriteBank35Frame007;
extern const struct sprite_frame gSpriteBank35Frame008;
extern const struct sprite_frame gSpriteBank35Frame009;
extern const struct sprite_frame gSpriteBank35Frame010;
extern const struct sprite_frame gSpriteBank35Frame011;
extern const struct sprite_frame gSpriteBank35Frame012;
extern const struct sprite_frame gSpriteBank35Frame013;
extern const struct sprite_frame gSpriteBank35Frame014;
extern const struct sprite_frame gSpriteBank35Frame015;
extern const struct sprite_frame gSpriteBank35Frame016;
extern const struct sprite_frame gSpriteBank35Frame017;
extern const struct sprite_frame gSpriteBank35Frame018;
extern const struct sprite_frame gSpriteBank35Frame019;
extern const struct sprite_frame gSpriteBank35Frame020;
extern const struct sprite_frame gSpriteBank35Frame021;
extern const struct sprite_frame gSpriteBank35Frame022;
extern const struct sprite_frame gSpriteBank35Frame023;
extern const struct sprite_frame gSpriteBank35Frame024;
extern const struct sprite_frame gSpriteBank35Frame025;
extern const struct sprite_frame gSpriteBank35Frame026;
extern const struct sprite_frame gSpriteBank35Frame027;
extern const struct sprite_piece_pos gSpriteBank35Frame000Pos[4];
extern const struct sprite_piece_pos gSpriteBank35Frame001Pos[4];
extern const struct sprite_piece_pos gSpriteBank35Frame002Pos[4];
extern const struct sprite_piece_pos gSpriteBank35Frame003Pos[4];
extern const struct sprite_piece_pos gSpriteBank35Frame004Pos[3];
extern const struct sprite_piece_pos gSpriteBank35Frame005Pos[3];
extern const struct sprite_piece_pos gSpriteBank35Frame006Pos[4];
extern const struct sprite_piece_pos gSpriteBank35Frame007Pos[4];
extern const struct sprite_piece_pos gSpriteBank35Frame008Pos[4];
extern const struct sprite_piece_pos gSpriteBank35Frame009Pos[4];
extern const struct sprite_piece_pos gSpriteBank35Frame010Pos[4];
extern const struct sprite_piece_pos gSpriteBank35Frame011Pos[3];
extern const struct sprite_piece_pos gSpriteBank35Frame012Pos[3];
extern const struct sprite_piece_pos gSpriteBank35Frame013Pos[4];
extern const struct sprite_piece_pos gSpriteBank35Frame014Pos[1];
extern const struct sprite_piece_pos gSpriteBank35Frame015Pos[1];
extern const struct sprite_piece_pos gSpriteBank35Frame016Pos[1];
extern const struct sprite_piece_pos gSpriteBank35Frame017Pos[1];
extern const struct sprite_piece_pos gSpriteBank35Frame018Pos[1];
extern const struct sprite_piece_pos gSpriteBank35Frame019Pos[1];
extern const struct sprite_piece_pos gSpriteBank35Frame020Pos[1];
extern const struct sprite_piece_pos gSpriteBank35Frame021Pos[1];
extern const struct sprite_piece_pos gSpriteBank35Frame022Pos[1];
extern const struct sprite_piece_pos gSpriteBank35Frame023Pos[1];
extern const struct sprite_piece_pos gSpriteBank35Frame024Pos[1];
extern const struct sprite_piece_pos gSpriteBank35Frame025Pos[1];
extern const struct sprite_piece_pos gSpriteBank35Frame026Pos[1];
extern const struct sprite_piece_pos gSpriteBank35Frame027Pos[1];
extern const u8 gSpriteBank35Frame000Pieces[4];
extern const u8 gSpriteBank35Frame001Pieces[4];
extern const u8 gSpriteBank35Frame002Pieces[4];
extern const u8 gSpriteBank35Frame003Pieces[4];
extern const u8 gSpriteBank35Frame004Pieces[3];
extern const u8 gSpriteBank35Frame005Pieces[3];
extern const u8 gSpriteBank35Frame006Pieces[4];
extern const u8 gSpriteBank35Frame007Pieces[4];
extern const u8 gSpriteBank35Frame008Pieces[4];
extern const u8 gSpriteBank35Frame009Pieces[4];
extern const u8 gSpriteBank35Frame010Pieces[4];
extern const u8 gSpriteBank35Frame011Pieces[3];
extern const u8 gSpriteBank35Frame012Pieces[3];
extern const u8 gSpriteBank35Frame013Pieces[4];
extern const u8 gSpriteBank35Frame014Pieces[1];
extern const u8 gSpriteBank35Frame015Pieces[1];
extern const u8 gSpriteBank35Frame016Pieces[1];
extern const u8 gSpriteBank35Frame017Pieces[1];
extern const u8 gSpriteBank35Frame018Pieces[1];
extern const u8 gSpriteBank35Frame019Pieces[1];
extern const u8 gSpriteBank35Frame020Pieces[1];
extern const u8 gSpriteBank35Frame021Pieces[1];
extern const u8 gSpriteBank35Frame022Pieces[1];
extern const u8 gSpriteBank35Frame023Pieces[1];
extern const u8 gSpriteBank35Frame024Pieces[1];
extern const u8 gSpriteBank35Frame025Pieces[1];
extern const u8 gSpriteBank35Frame026Pieces[1];
extern const u8 gSpriteBank35Frame027Pieces[1];

const struct sprite_anim gSpriteBank35Anims[2] = {
    [0] = {
        .seq = gSpriteBank35Anim00Seq,
        .box = { { -9, -11, 19, 23 }, { -9, -11, 20, 23 } },
        .paletteId = 33,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank35Anim00Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [1] = {
        .seq = gSpriteBank35Anim01Seq,
        .box = { { -6, -7, 12, 15 }, { -6, -7, 12, 15 } },
        .paletteId = 33,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank35Anim01Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank35Anim00Seq[14] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
};
const u16 gSpriteBank35Anim01Seq[14] = {
    14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
};

const struct sprite_frame *const gSpriteBank35Frames[28] = {
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

const struct sprite_frame gSpriteBank35Frame000 = SPRITE_FRAME(gSpriteBank35Frame000, SPRITE_TILES_BANK35 + 0x00000);
const struct sprite_frame gSpriteBank35Frame001 = SPRITE_FRAME(gSpriteBank35Frame001, SPRITE_TILES_BANK35 + 0x00120);
const struct sprite_frame gSpriteBank35Frame002 = SPRITE_FRAME(gSpriteBank35Frame002, SPRITE_TILES_BANK35 + 0x00240);
const struct sprite_frame gSpriteBank35Frame003 = SPRITE_FRAME(gSpriteBank35Frame003, SPRITE_TILES_BANK35 + 0x00360);
const struct sprite_frame gSpriteBank35Frame004 = SPRITE_FRAME(gSpriteBank35Frame004, SPRITE_TILES_BANK35 + 0x00480);
const struct sprite_frame gSpriteBank35Frame005 = SPRITE_FRAME(gSpriteBank35Frame005, SPRITE_TILES_BANK35 + 0x00580);
const struct sprite_frame gSpriteBank35Frame006 = SPRITE_FRAME(gSpriteBank35Frame006, SPRITE_TILES_BANK35 + 0x00680);
const struct sprite_frame gSpriteBank35Frame007 = SPRITE_FRAME(gSpriteBank35Frame007, SPRITE_TILES_BANK35 + 0x007a0);
const struct sprite_frame gSpriteBank35Frame008 = SPRITE_FRAME(gSpriteBank35Frame008, SPRITE_TILES_BANK35 + 0x008c0);
const struct sprite_frame gSpriteBank35Frame009 = SPRITE_FRAME(gSpriteBank35Frame009, SPRITE_TILES_BANK35 + 0x009e0);
const struct sprite_frame gSpriteBank35Frame010 = SPRITE_FRAME(gSpriteBank35Frame010, SPRITE_TILES_BANK35 + 0x00b00);
const struct sprite_frame gSpriteBank35Frame011 = SPRITE_FRAME(gSpriteBank35Frame011, SPRITE_TILES_BANK35 + 0x00c20);
const struct sprite_frame gSpriteBank35Frame012 = SPRITE_FRAME(gSpriteBank35Frame012, SPRITE_TILES_BANK35 + 0x00d20);
const struct sprite_frame gSpriteBank35Frame013 = SPRITE_FRAME(gSpriteBank35Frame013, SPRITE_TILES_BANK35 + 0x00e20);
const struct sprite_frame gSpriteBank35Frame014 = SPRITE_FRAME(gSpriteBank35Frame014, SPRITE_TILES_BANK35 + 0x00f40);
const struct sprite_frame gSpriteBank35Frame015 = SPRITE_FRAME(gSpriteBank35Frame015, SPRITE_TILES_BANK35 + 0x00fc0);
const struct sprite_frame gSpriteBank35Frame016 = SPRITE_FRAME(gSpriteBank35Frame016, SPRITE_TILES_BANK35 + 0x01040);
const struct sprite_frame gSpriteBank35Frame017 = SPRITE_FRAME(gSpriteBank35Frame017, SPRITE_TILES_BANK35 + 0x010c0);
const struct sprite_frame gSpriteBank35Frame018 = SPRITE_FRAME(gSpriteBank35Frame018, SPRITE_TILES_BANK35 + 0x01140);
const struct sprite_frame gSpriteBank35Frame019 = SPRITE_FRAME(gSpriteBank35Frame019, SPRITE_TILES_BANK35 + 0x011c0);
const struct sprite_frame gSpriteBank35Frame020 = SPRITE_FRAME(gSpriteBank35Frame020, SPRITE_TILES_BANK35 + 0x01240);
const struct sprite_frame gSpriteBank35Frame021 = SPRITE_FRAME(gSpriteBank35Frame021, SPRITE_TILES_BANK35 + 0x012c0);
const struct sprite_frame gSpriteBank35Frame022 = SPRITE_FRAME(gSpriteBank35Frame022, SPRITE_TILES_BANK35 + 0x01340);
const struct sprite_frame gSpriteBank35Frame023 = SPRITE_FRAME(gSpriteBank35Frame023, SPRITE_TILES_BANK35 + 0x013c0);
const struct sprite_frame gSpriteBank35Frame024 = SPRITE_FRAME(gSpriteBank35Frame024, SPRITE_TILES_BANK35 + 0x01440);
const struct sprite_frame gSpriteBank35Frame025 = SPRITE_FRAME(gSpriteBank35Frame025, SPRITE_TILES_BANK35 + 0x014c0);
const struct sprite_frame gSpriteBank35Frame026 = SPRITE_FRAME(gSpriteBank35Frame026, SPRITE_TILES_BANK35 + 0x01540);
const struct sprite_frame gSpriteBank35Frame027 = SPRITE_FRAME(gSpriteBank35Frame027, SPRITE_TILES_BANK35 + 0x015c0);

const struct sprite_piece_pos gSpriteBank35Frame000Pos[4] = { { -9, -11 }, { 7, -7 }, { -8, 5 }, { 8, 5 } };
const struct sprite_piece_pos gSpriteBank35Frame001Pos[4] = { { -9, -10 }, { 7, -7 }, { -8, 6 }, { 8, 6 } };
const struct sprite_piece_pos gSpriteBank35Frame002Pos[4] = { { -8, -10 }, { 8, -7 }, { -8, 6 }, { 8, 6 } };
const struct sprite_piece_pos gSpriteBank35Frame003Pos[4] = { { -8, -10 }, { 8, -5 }, { -7, 6 }, { 9, 6 } };
const struct sprite_piece_pos gSpriteBank35Frame004Pos[3] = { { -8, -9 }, { 8, -5 }, { -6, 7 } };
const struct sprite_piece_pos gSpriteBank35Frame005Pos[3] = { { -8, -9 }, { 8, -5 }, { -6, 7 } };
const struct sprite_piece_pos gSpriteBank35Frame006Pos[4] = { { -8, -10 }, { 8, -9 }, { -7, 6 }, { 9, 6 } };
const struct sprite_piece_pos gSpriteBank35Frame007Pos[4] = { { -8, -10 }, { 8, -9 }, { -7, 6 }, { 9, 6 } };
const struct sprite_piece_pos gSpriteBank35Frame008Pos[4] = { { -8, -10 }, { 8, -10 }, { -7, 6 }, { 9, 6 } };
const struct sprite_piece_pos gSpriteBank35Frame009Pos[4] = { { -8, -10 }, { 8, -5 }, { -8, 6 }, { 8, 6 } };
const struct sprite_piece_pos gSpriteBank35Frame010Pos[4] = { { -8, -9 }, { 8, -5 }, { -8, 7 }, { 8, 7 } };
const struct sprite_piece_pos gSpriteBank35Frame011Pos[3] = { { -8, -9 }, { 8, -5 }, { -6, 7 } };
const struct sprite_piece_pos gSpriteBank35Frame012Pos[3] = { { -8, -9 }, { 8, -5 }, { -6, 7 } };
const struct sprite_piece_pos gSpriteBank35Frame013Pos[4] = { { -8, -11 }, { 8, -5 }, { -8, 5 }, { 8, 5 } };
const struct sprite_piece_pos gSpriteBank35Frame014Pos[1] = { { -6, -7 } };
const struct sprite_piece_pos gSpriteBank35Frame015Pos[1] = { { -6, -6 } };
const struct sprite_piece_pos gSpriteBank35Frame016Pos[1] = { { -6, -6 } };
const struct sprite_piece_pos gSpriteBank35Frame017Pos[1] = { { -6, -6 } };
const struct sprite_piece_pos gSpriteBank35Frame018Pos[1] = { { -6, -5 } };
const struct sprite_piece_pos gSpriteBank35Frame019Pos[1] = { { -6, -5 } };
const struct sprite_piece_pos gSpriteBank35Frame020Pos[1] = { { -6, -6 } };
const struct sprite_piece_pos gSpriteBank35Frame021Pos[1] = { { -6, -6 } };
const struct sprite_piece_pos gSpriteBank35Frame022Pos[1] = { { -6, -6 } };
const struct sprite_piece_pos gSpriteBank35Frame023Pos[1] = { { -6, -6 } };
const struct sprite_piece_pos gSpriteBank35Frame024Pos[1] = { { -6, -6 } };
const struct sprite_piece_pos gSpriteBank35Frame025Pos[1] = { { -6, -6 } };
const struct sprite_piece_pos gSpriteBank35Frame026Pos[1] = { { -6, -6 } };
const struct sprite_piece_pos gSpriteBank35Frame027Pos[1] = { { -6, -7 } };

const u8 gSpriteBank35Frame000Pieces[4] = { SPRITE_PIECE(1, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank35Frame001Pieces[4] = { SPRITE_PIECE(1, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank35Frame002Pieces[4] = { SPRITE_PIECE(1, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank35Frame003Pieces[4] = { SPRITE_PIECE(1, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank35Frame004Pieces[3] = { SPRITE_PIECE(1, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank35Frame005Pieces[3] = { SPRITE_PIECE(1, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank35Frame006Pieces[4] = { SPRITE_PIECE(1, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank35Frame007Pieces[4] = { SPRITE_PIECE(1, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank35Frame008Pieces[4] = { SPRITE_PIECE(1, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank35Frame009Pieces[4] = { SPRITE_PIECE(1, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank35Frame010Pieces[4] = { SPRITE_PIECE(1, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank35Frame011Pieces[3] = { SPRITE_PIECE(1, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank35Frame012Pieces[3] = { SPRITE_PIECE(1, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank35Frame013Pieces[4] = { SPRITE_PIECE(1, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank35Frame014Pieces[1] = { SPRITE_PIECE(1, 1) };
const u8 gSpriteBank35Frame015Pieces[1] = { SPRITE_PIECE(1, 1) };
const u8 gSpriteBank35Frame016Pieces[1] = { SPRITE_PIECE(1, 1) };
const u8 gSpriteBank35Frame017Pieces[1] = { SPRITE_PIECE(1, 1) };
const u8 gSpriteBank35Frame018Pieces[1] = { SPRITE_PIECE(1, 1) };
const u8 gSpriteBank35Frame019Pieces[1] = { SPRITE_PIECE(1, 1) };
const u8 gSpriteBank35Frame020Pieces[1] = { SPRITE_PIECE(1, 1) };
const u8 gSpriteBank35Frame021Pieces[1] = { SPRITE_PIECE(1, 1) };
const u8 gSpriteBank35Frame022Pieces[1] = { SPRITE_PIECE(1, 1) };
const u8 gSpriteBank35Frame023Pieces[1] = { SPRITE_PIECE(1, 1) };
const u8 gSpriteBank35Frame024Pieces[1] = { SPRITE_PIECE(1, 1) };
const u8 gSpriteBank35Frame025Pieces[1] = { SPRITE_PIECE(1, 1) };
const u8 gSpriteBank35Frame026Pieces[1] = { SPRITE_PIECE(1, 1) };
const u8 gSpriteBank35Frame027Pieces[1] = { SPRITE_PIECE(1, 1) };

/* ---------------------------------------------------------------------- */
/* Bank 36: 1 animation, 10 frames, tiles in gSpriteBank36Tiles (SPRITE_TILES_BANK36). */

extern const u16 gSpriteBank36Anim00Seq[10];
extern const struct sprite_frame gSpriteBank36Frame000;
extern const struct sprite_frame gSpriteBank36Frame001;
extern const struct sprite_frame gSpriteBank36Frame002;
extern const struct sprite_frame gSpriteBank36Frame003;
extern const struct sprite_frame gSpriteBank36Frame004;
extern const struct sprite_frame gSpriteBank36Frame005;
extern const struct sprite_frame gSpriteBank36Frame006;
extern const struct sprite_frame gSpriteBank36Frame007;
extern const struct sprite_frame gSpriteBank36Frame008;
extern const struct sprite_frame gSpriteBank36Frame009;
extern const struct sprite_piece_pos gSpriteBank36Frame000Pos[4];
extern const struct sprite_piece_pos gSpriteBank36Frame001Pos[4];
extern const struct sprite_piece_pos gSpriteBank36Frame002Pos[4];
extern const struct sprite_piece_pos gSpriteBank36Frame003Pos[4];
extern const struct sprite_piece_pos gSpriteBank36Frame004Pos[3];
extern const struct sprite_piece_pos gSpriteBank36Frame005Pos[2];
extern const struct sprite_piece_pos gSpriteBank36Frame006Pos[4];
extern const struct sprite_piece_pos gSpriteBank36Frame007Pos[2];
extern const struct sprite_piece_pos gSpriteBank36Frame008Pos[2];
extern const struct sprite_piece_pos gSpriteBank36Frame009Pos[3];
extern const u8 gSpriteBank36Frame000Pieces[4];
extern const u8 gSpriteBank36Frame001Pieces[4];
extern const u8 gSpriteBank36Frame002Pieces[4];
extern const u8 gSpriteBank36Frame003Pieces[4];
extern const u8 gSpriteBank36Frame004Pieces[3];
extern const u8 gSpriteBank36Frame005Pieces[2];
extern const u8 gSpriteBank36Frame006Pieces[4];
extern const u8 gSpriteBank36Frame007Pieces[2];
extern const u8 gSpriteBank36Frame008Pieces[2];
extern const u8 gSpriteBank36Frame009Pieces[3];

const struct sprite_anim gSpriteBank36Anims[1] = {
    [0] = {
        .seq = gSpriteBank36Anim00Seq,
        .box = { { -8, -11, 17, 22 }, { -9, -11, 18, 23 } },
        .paletteId = 58,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank36Anim00Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank36Anim00Seq[10] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
};

const struct sprite_frame *const gSpriteBank36Frames[10] = {
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

const struct sprite_frame gSpriteBank36Frame000 = SPRITE_FRAME(gSpriteBank36Frame000, SPRITE_TILES_BANK36 + 0x00000);
const struct sprite_frame gSpriteBank36Frame001 = SPRITE_FRAME(gSpriteBank36Frame001, SPRITE_TILES_BANK36 + 0x00100);
const struct sprite_frame gSpriteBank36Frame002 = SPRITE_FRAME(gSpriteBank36Frame002, SPRITE_TILES_BANK36 + 0x00220);
const struct sprite_frame gSpriteBank36Frame003 = SPRITE_FRAME(gSpriteBank36Frame003, SPRITE_TILES_BANK36 + 0x00320);
const struct sprite_frame gSpriteBank36Frame004 = SPRITE_FRAME(gSpriteBank36Frame004, SPRITE_TILES_BANK36 + 0x00440);
const struct sprite_frame gSpriteBank36Frame005 = SPRITE_FRAME(gSpriteBank36Frame005, SPRITE_TILES_BANK36 + 0x00520);
const struct sprite_frame gSpriteBank36Frame006 = SPRITE_FRAME(gSpriteBank36Frame006, SPRITE_TILES_BANK36 + 0x005e0);
const struct sprite_frame gSpriteBank36Frame007 = SPRITE_FRAME(gSpriteBank36Frame007, SPRITE_TILES_BANK36 + 0x006a0);
const struct sprite_frame gSpriteBank36Frame008 = SPRITE_FRAME(gSpriteBank36Frame008, SPRITE_TILES_BANK36 + 0x00700);
const struct sprite_frame gSpriteBank36Frame009 = SPRITE_FRAME(gSpriteBank36Frame009, SPRITE_TILES_BANK36 + 0x007c0);

const struct sprite_piece_pos gSpriteBank36Frame000Pos[4] = { { -8, -11 }, { 8, -3 }, { -8, 5 }, { 8, 5 } };
const struct sprite_piece_pos gSpriteBank36Frame001Pos[4] = { { -9, -11 }, { 7, -4 }, { -8, 5 }, { 8, 5 } };
const struct sprite_piece_pos gSpriteBank36Frame002Pos[4] = { { -8, -11 }, { 8, -3 }, { -8, 5 }, { 8, 5 } };
const struct sprite_piece_pos gSpriteBank36Frame003Pos[4] = { { -9, -11 }, { 7, -4 }, { -8, 5 }, { 8, 5 } };
const struct sprite_piece_pos gSpriteBank36Frame004Pos[3] = { { -8, -11 }, { 8, -2 }, { -8, 5 } };
const struct sprite_piece_pos gSpriteBank36Frame005Pos[2] = { { -7, -11 }, { -6, 5 } };
const struct sprite_piece_pos gSpriteBank36Frame006Pos[4] = { { -4, -11 }, { 4, -5 }, { -4, 5 }, { 4, 5 } };
const struct sprite_piece_pos gSpriteBank36Frame007Pos[2] = { { -3, -11 }, { -3, 5 } };
const struct sprite_piece_pos gSpriteBank36Frame008Pos[2] = { { -5, -11 }, { -4, 5 } };
const struct sprite_piece_pos gSpriteBank36Frame009Pos[3] = { { -7, -11 }, { 9, 0 }, { -6, 5 } };

const u8 gSpriteBank36Frame000Pieces[4] = { SPRITE_PIECE(1, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank36Frame001Pieces[4] = { SPRITE_PIECE(1, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank36Frame002Pieces[4] = { SPRITE_PIECE(1, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank36Frame003Pieces[4] = { SPRITE_PIECE(1, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank36Frame004Pieces[3] = { SPRITE_PIECE(1, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank36Frame005Pieces[2] = { SPRITE_PIECE(1, 1), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank36Frame006Pieces[4] = { SPRITE_PIECE(1, 8), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank36Frame007Pieces[2] = { SPRITE_PIECE(1, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank36Frame008Pieces[2] = { SPRITE_PIECE(1, 1), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank36Frame009Pieces[3] = { SPRITE_PIECE(1, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };

/* ---------------------------------------------------------------------- */
/* Bank 37: 1 animation, 8 frames, tiles in gSpriteBank37Tiles (SPRITE_TILES_BANK37). */

extern const u16 gSpriteBank37Anim00Seq[8];
extern const struct sprite_frame gSpriteBank37Frame000;
extern const struct sprite_frame gSpriteBank37Frame001;
extern const struct sprite_frame gSpriteBank37Frame002;
extern const struct sprite_frame gSpriteBank37Frame003;
extern const struct sprite_frame gSpriteBank37Frame004;
extern const struct sprite_frame gSpriteBank37Frame005;
extern const struct sprite_frame gSpriteBank37Frame006;
extern const struct sprite_frame gSpriteBank37Frame007;
extern const struct sprite_piece_pos gSpriteBank37Frame000Pos[3];
extern const struct sprite_piece_pos gSpriteBank37Frame001Pos[3];
extern const struct sprite_piece_pos gSpriteBank37Frame002Pos[3];
extern const struct sprite_piece_pos gSpriteBank37Frame003Pos[3];
extern const struct sprite_piece_pos gSpriteBank37Frame004Pos[3];
extern const struct sprite_piece_pos gSpriteBank37Frame005Pos[3];
extern const struct sprite_piece_pos gSpriteBank37Frame006Pos[3];
extern const struct sprite_piece_pos gSpriteBank37Frame007Pos[3];
extern const u8 gSpriteBank37Frame000Pieces[3];
extern const u8 gSpriteBank37Frame001Pieces[3];
extern const u8 gSpriteBank37Frame002Pieces[3];
extern const u8 gSpriteBank37Frame003Pieces[3];
extern const u8 gSpriteBank37Frame004Pieces[3];
extern const u8 gSpriteBank37Frame005Pieces[3];
extern const u8 gSpriteBank37Frame006Pieces[3];
extern const u8 gSpriteBank37Frame007Pieces[3];

const struct sprite_anim gSpriteBank37Anims[1] = {
    [0] = {
        .seq = gSpriteBank37Anim00Seq,
        .box = { { -6, -21, 13, 43 }, { -6, -21, 13, 43 } },
        .paletteId = 117,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank37Anim00Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank37Anim00Seq[8] = {
    0, 1, 2, 3, 4, 5, 6, 7,
};

const struct sprite_frame *const gSpriteBank37Frames[8] = {
    &gSpriteBank37Frame000,
    &gSpriteBank37Frame001,
    &gSpriteBank37Frame002,
    &gSpriteBank37Frame003,
    &gSpriteBank37Frame004,
    &gSpriteBank37Frame005,
    &gSpriteBank37Frame006,
    &gSpriteBank37Frame007,
};

const struct sprite_frame gSpriteBank37Frame000 = SPRITE_FRAME(gSpriteBank37Frame000, SPRITE_TILES_BANK37 + 0x00000);
const struct sprite_frame gSpriteBank37Frame001 = SPRITE_FRAME(gSpriteBank37Frame001, SPRITE_TILES_BANK37 + 0x00140);
const struct sprite_frame gSpriteBank37Frame002 = SPRITE_FRAME(gSpriteBank37Frame002, SPRITE_TILES_BANK37 + 0x00280);
const struct sprite_frame gSpriteBank37Frame003 = SPRITE_FRAME(gSpriteBank37Frame003, SPRITE_TILES_BANK37 + 0x003c0);
const struct sprite_frame gSpriteBank37Frame004 = SPRITE_FRAME(gSpriteBank37Frame004, SPRITE_TILES_BANK37 + 0x00500);
const struct sprite_frame gSpriteBank37Frame005 = SPRITE_FRAME(gSpriteBank37Frame005, SPRITE_TILES_BANK37 + 0x00640);
const struct sprite_frame gSpriteBank37Frame006 = SPRITE_FRAME(gSpriteBank37Frame006, SPRITE_TILES_BANK37 + 0x00780);
const struct sprite_frame gSpriteBank37Frame007 = SPRITE_FRAME(gSpriteBank37Frame007, SPRITE_TILES_BANK37 + 0x008c0);

const struct sprite_piece_pos gSpriteBank37Frame000Pos[3] = { { -6, -21 }, { 2, -20 }, { -3, 11 } };
const struct sprite_piece_pos gSpriteBank37Frame001Pos[3] = { { -6, -20 }, { 2, -19 }, { -3, 12 } };
const struct sprite_piece_pos gSpriteBank37Frame002Pos[3] = { { -6, -21 }, { 2, -20 }, { -3, 11 } };
const struct sprite_piece_pos gSpriteBank37Frame003Pos[3] = { { -6, -21 }, { 2, -20 }, { -3, 11 } };
const struct sprite_piece_pos gSpriteBank37Frame004Pos[3] = { { -5, -21 }, { 3, -19 }, { -3, 11 } };
const struct sprite_piece_pos gSpriteBank37Frame005Pos[3] = { { -5, -21 }, { 3, -19 }, { -3, 11 } };
const struct sprite_piece_pos gSpriteBank37Frame006Pos[3] = { { -5, -21 }, { 3, -19 }, { -3, 11 } };
const struct sprite_piece_pos gSpriteBank37Frame007Pos[3] = { { -5, -21 }, { 3, -18 }, { -3, 11 } };

const u8 gSpriteBank37Frame000Pieces[3] = { SPRITE_PIECE(1, 9), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank37Frame001Pieces[3] = { SPRITE_PIECE(1, 9), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank37Frame002Pieces[3] = { SPRITE_PIECE(1, 9), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank37Frame003Pieces[3] = { SPRITE_PIECE(1, 9), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank37Frame004Pieces[3] = { SPRITE_PIECE(1, 9), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank37Frame005Pieces[3] = { SPRITE_PIECE(1, 9), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank37Frame006Pieces[3] = { SPRITE_PIECE(1, 9), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank37Frame007Pieces[3] = { SPRITE_PIECE(1, 9), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };

/* ---------------------------------------------------------------------- */
/* Bank 38: 4 animations, 60 frames, tiles in gSpriteBank38Tiles (SPRITE_TILES_BANK38). */

extern const u16 gSpriteBank38Anim00Seq[15];
extern const u16 gSpriteBank38Anim01Seq[15];
extern const u16 gSpriteBank38Anim02Seq[15];
extern const u16 gSpriteBank38Anim03Seq[15];
extern const struct sprite_frame gSpriteBank38Frame000;
extern const struct sprite_frame gSpriteBank38Frame001;
extern const struct sprite_frame gSpriteBank38Frame002;
extern const struct sprite_frame gSpriteBank38Frame003;
extern const struct sprite_frame gSpriteBank38Frame004;
extern const struct sprite_frame gSpriteBank38Frame005;
extern const struct sprite_frame gSpriteBank38Frame006;
extern const struct sprite_frame gSpriteBank38Frame007;
extern const struct sprite_frame gSpriteBank38Frame008;
extern const struct sprite_frame gSpriteBank38Frame009;
extern const struct sprite_frame gSpriteBank38Frame010;
extern const struct sprite_frame gSpriteBank38Frame011;
extern const struct sprite_frame gSpriteBank38Frame012;
extern const struct sprite_frame gSpriteBank38Frame013;
extern const struct sprite_frame gSpriteBank38Frame014;
extern const struct sprite_frame gSpriteBank38Frame015;
extern const struct sprite_frame gSpriteBank38Frame016;
extern const struct sprite_frame gSpriteBank38Frame017;
extern const struct sprite_frame gSpriteBank38Frame018;
extern const struct sprite_frame gSpriteBank38Frame019;
extern const struct sprite_frame gSpriteBank38Frame020;
extern const struct sprite_frame gSpriteBank38Frame021;
extern const struct sprite_frame gSpriteBank38Frame022;
extern const struct sprite_frame gSpriteBank38Frame023;
extern const struct sprite_frame gSpriteBank38Frame024;
extern const struct sprite_frame gSpriteBank38Frame025;
extern const struct sprite_frame gSpriteBank38Frame026;
extern const struct sprite_frame gSpriteBank38Frame027;
extern const struct sprite_frame gSpriteBank38Frame028;
extern const struct sprite_frame gSpriteBank38Frame029;
extern const struct sprite_frame gSpriteBank38Frame030;
extern const struct sprite_frame gSpriteBank38Frame031;
extern const struct sprite_frame gSpriteBank38Frame032;
extern const struct sprite_frame gSpriteBank38Frame033;
extern const struct sprite_frame gSpriteBank38Frame034;
extern const struct sprite_frame gSpriteBank38Frame035;
extern const struct sprite_frame gSpriteBank38Frame036;
extern const struct sprite_frame gSpriteBank38Frame037;
extern const struct sprite_frame gSpriteBank38Frame038;
extern const struct sprite_frame gSpriteBank38Frame039;
extern const struct sprite_frame gSpriteBank38Frame040;
extern const struct sprite_frame gSpriteBank38Frame041;
extern const struct sprite_frame gSpriteBank38Frame042;
extern const struct sprite_frame gSpriteBank38Frame043;
extern const struct sprite_frame gSpriteBank38Frame044;
extern const struct sprite_frame gSpriteBank38Frame045;
extern const struct sprite_frame gSpriteBank38Frame046;
extern const struct sprite_frame gSpriteBank38Frame047;
extern const struct sprite_frame gSpriteBank38Frame048;
extern const struct sprite_frame gSpriteBank38Frame049;
extern const struct sprite_frame gSpriteBank38Frame050;
extern const struct sprite_frame gSpriteBank38Frame051;
extern const struct sprite_frame gSpriteBank38Frame052;
extern const struct sprite_frame gSpriteBank38Frame053;
extern const struct sprite_frame gSpriteBank38Frame054;
extern const struct sprite_frame gSpriteBank38Frame055;
extern const struct sprite_frame gSpriteBank38Frame056;
extern const struct sprite_frame gSpriteBank38Frame057;
extern const struct sprite_frame gSpriteBank38Frame058;
extern const struct sprite_frame gSpriteBank38Frame059;
extern const struct sprite_piece_pos gSpriteBank38Frame000Pos[1];
extern const struct sprite_piece_pos gSpriteBank38Frame001Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame002Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame003Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame004Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame005Pos[1];
extern const struct sprite_piece_pos gSpriteBank38Frame006Pos[1];
extern const struct sprite_piece_pos gSpriteBank38Frame007Pos[1];
extern const struct sprite_piece_pos gSpriteBank38Frame008Pos[1];
extern const struct sprite_piece_pos gSpriteBank38Frame009Pos[1];
extern const struct sprite_piece_pos gSpriteBank38Frame010Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame011Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame012Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame013Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame014Pos[1];
extern const struct sprite_piece_pos gSpriteBank38Frame015Pos[1];
extern const struct sprite_piece_pos gSpriteBank38Frame016Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame017Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame018Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame019Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame020Pos[1];
extern const struct sprite_piece_pos gSpriteBank38Frame021Pos[1];
extern const struct sprite_piece_pos gSpriteBank38Frame022Pos[1];
extern const struct sprite_piece_pos gSpriteBank38Frame023Pos[1];
extern const struct sprite_piece_pos gSpriteBank38Frame024Pos[1];
extern const struct sprite_piece_pos gSpriteBank38Frame025Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame026Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame027Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame028Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame029Pos[1];
extern const struct sprite_piece_pos gSpriteBank38Frame030Pos[1];
extern const struct sprite_piece_pos gSpriteBank38Frame031Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame032Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame033Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame034Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame035Pos[1];
extern const struct sprite_piece_pos gSpriteBank38Frame036Pos[1];
extern const struct sprite_piece_pos gSpriteBank38Frame037Pos[1];
extern const struct sprite_piece_pos gSpriteBank38Frame038Pos[1];
extern const struct sprite_piece_pos gSpriteBank38Frame039Pos[1];
extern const struct sprite_piece_pos gSpriteBank38Frame040Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame041Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame042Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame043Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame044Pos[1];
extern const struct sprite_piece_pos gSpriteBank38Frame045Pos[1];
extern const struct sprite_piece_pos gSpriteBank38Frame046Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame047Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame048Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame049Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame050Pos[1];
extern const struct sprite_piece_pos gSpriteBank38Frame051Pos[1];
extern const struct sprite_piece_pos gSpriteBank38Frame052Pos[1];
extern const struct sprite_piece_pos gSpriteBank38Frame053Pos[1];
extern const struct sprite_piece_pos gSpriteBank38Frame054Pos[1];
extern const struct sprite_piece_pos gSpriteBank38Frame055Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame056Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame057Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame058Pos[2];
extern const struct sprite_piece_pos gSpriteBank38Frame059Pos[1];
extern const u8 gSpriteBank38Frame000Pieces[1];
extern const u8 gSpriteBank38Frame001Pieces[2];
extern const u8 gSpriteBank38Frame002Pieces[2];
extern const u8 gSpriteBank38Frame003Pieces[2];
extern const u8 gSpriteBank38Frame004Pieces[2];
extern const u8 gSpriteBank38Frame005Pieces[1];
extern const u8 gSpriteBank38Frame006Pieces[1];
extern const u8 gSpriteBank38Frame007Pieces[1];
extern const u8 gSpriteBank38Frame008Pieces[1];
extern const u8 gSpriteBank38Frame009Pieces[1];
extern const u8 gSpriteBank38Frame010Pieces[2];
extern const u8 gSpriteBank38Frame011Pieces[2];
extern const u8 gSpriteBank38Frame012Pieces[2];
extern const u8 gSpriteBank38Frame013Pieces[2];
extern const u8 gSpriteBank38Frame014Pieces[1];
extern const u8 gSpriteBank38Frame015Pieces[1];
extern const u8 gSpriteBank38Frame016Pieces[2];
extern const u8 gSpriteBank38Frame017Pieces[2];
extern const u8 gSpriteBank38Frame018Pieces[2];
extern const u8 gSpriteBank38Frame019Pieces[2];
extern const u8 gSpriteBank38Frame020Pieces[1];
extern const u8 gSpriteBank38Frame021Pieces[1];
extern const u8 gSpriteBank38Frame022Pieces[1];
extern const u8 gSpriteBank38Frame023Pieces[1];
extern const u8 gSpriteBank38Frame024Pieces[1];
extern const u8 gSpriteBank38Frame025Pieces[2];
extern const u8 gSpriteBank38Frame026Pieces[2];
extern const u8 gSpriteBank38Frame027Pieces[2];
extern const u8 gSpriteBank38Frame028Pieces[2];
extern const u8 gSpriteBank38Frame029Pieces[1];
extern const u8 gSpriteBank38Frame030Pieces[1];
extern const u8 gSpriteBank38Frame031Pieces[2];
extern const u8 gSpriteBank38Frame032Pieces[2];
extern const u8 gSpriteBank38Frame033Pieces[2];
extern const u8 gSpriteBank38Frame034Pieces[2];
extern const u8 gSpriteBank38Frame035Pieces[1];
extern const u8 gSpriteBank38Frame036Pieces[1];
extern const u8 gSpriteBank38Frame037Pieces[1];
extern const u8 gSpriteBank38Frame038Pieces[1];
extern const u8 gSpriteBank38Frame039Pieces[1];
extern const u8 gSpriteBank38Frame040Pieces[2];
extern const u8 gSpriteBank38Frame041Pieces[2];
extern const u8 gSpriteBank38Frame042Pieces[2];
extern const u8 gSpriteBank38Frame043Pieces[2];
extern const u8 gSpriteBank38Frame044Pieces[1];
extern const u8 gSpriteBank38Frame045Pieces[1];
extern const u8 gSpriteBank38Frame046Pieces[2];
extern const u8 gSpriteBank38Frame047Pieces[2];
extern const u8 gSpriteBank38Frame048Pieces[2];
extern const u8 gSpriteBank38Frame049Pieces[2];
extern const u8 gSpriteBank38Frame050Pieces[1];
extern const u8 gSpriteBank38Frame051Pieces[1];
extern const u8 gSpriteBank38Frame052Pieces[1];
extern const u8 gSpriteBank38Frame053Pieces[1];
extern const u8 gSpriteBank38Frame054Pieces[1];
extern const u8 gSpriteBank38Frame055Pieces[2];
extern const u8 gSpriteBank38Frame056Pieces[2];
extern const u8 gSpriteBank38Frame057Pieces[2];
extern const u8 gSpriteBank38Frame058Pieces[2];
extern const u8 gSpriteBank38Frame059Pieces[1];

const struct sprite_anim gSpriteBank38Anims[4] = {
    [0] = {
        .seq = gSpriteBank38Anim00Seq,
        .box = { { -2, -14, 4, 29 }, { -13, -14, 26, 29 } },
        .paletteId = 60,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank38Anim00Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [1] = {
        .seq = gSpriteBank38Anim01Seq,
        .box = { { -2, -14, 4, 29 }, { -13, -14, 26, 29 } },
        .paletteId = 60,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank38Anim01Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [2] = {
        .seq = gSpriteBank38Anim02Seq,
        .box = { { -2, -14, 4, 29 }, { -13, -14, 26, 29 } },
        .paletteId = 60,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank38Anim02Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [3] = {
        .seq = gSpriteBank38Anim03Seq,
        .box = { { -2, -14, 4, 29 }, { -13, -14, 26, 29 } },
        .paletteId = 60,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank38Anim03Seq),
        .flags = SPRITE_ANIM_LOOP,
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

const struct sprite_frame *const gSpriteBank38Frames[60] = {
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

const struct sprite_frame gSpriteBank38Frame000 = SPRITE_FRAME(gSpriteBank38Frame000, SPRITE_TILES_BANK38 + 0x00000);
const struct sprite_frame gSpriteBank38Frame001 = SPRITE_FRAME(gSpriteBank38Frame001, SPRITE_TILES_BANK38 + 0x00080);
const struct sprite_frame gSpriteBank38Frame002 = SPRITE_FRAME(gSpriteBank38Frame002, SPRITE_TILES_BANK38 + 0x00180);
const struct sprite_frame gSpriteBank38Frame003 = SPRITE_FRAME(gSpriteBank38Frame003, SPRITE_TILES_BANK38 + 0x00280);
const struct sprite_frame gSpriteBank38Frame004 = SPRITE_FRAME(gSpriteBank38Frame004, SPRITE_TILES_BANK38 + 0x00400);
const struct sprite_frame gSpriteBank38Frame005 = SPRITE_FRAME(gSpriteBank38Frame005, SPRITE_TILES_BANK38 + 0x00580);
const struct sprite_frame gSpriteBank38Frame006 = SPRITE_FRAME(gSpriteBank38Frame006, SPRITE_TILES_BANK38 + 0x00780);
const struct sprite_frame gSpriteBank38Frame007 = SPRITE_FRAME(gSpriteBank38Frame007, SPRITE_TILES_BANK38 + 0x00980);
const struct sprite_frame gSpriteBank38Frame008 = SPRITE_FRAME(gSpriteBank38Frame008, SPRITE_TILES_BANK38 + 0x00b80);
const struct sprite_frame gSpriteBank38Frame009 = SPRITE_FRAME(gSpriteBank38Frame009, SPRITE_TILES_BANK38 + 0x00d80);
const struct sprite_frame gSpriteBank38Frame010 = SPRITE_FRAME(gSpriteBank38Frame010, SPRITE_TILES_BANK38 + 0x00f80);
const struct sprite_frame gSpriteBank38Frame011 = SPRITE_FRAME(gSpriteBank38Frame011, SPRITE_TILES_BANK38 + 0x01100);
const struct sprite_frame gSpriteBank38Frame012 = SPRITE_FRAME(gSpriteBank38Frame012, SPRITE_TILES_BANK38 + 0x01280);
const struct sprite_frame gSpriteBank38Frame013 = SPRITE_FRAME(gSpriteBank38Frame013, SPRITE_TILES_BANK38 + 0x013c0);
const struct sprite_frame gSpriteBank38Frame014 = SPRITE_FRAME(gSpriteBank38Frame014, SPRITE_TILES_BANK38 + 0x014c0);
const struct sprite_frame gSpriteBank38Frame015 = SPRITE_FRAME(gSpriteBank38Frame015, SPRITE_TILES_BANK38 + 0x01540);
const struct sprite_frame gSpriteBank38Frame016 = SPRITE_FRAME(gSpriteBank38Frame016, SPRITE_TILES_BANK38 + 0x015c0);
const struct sprite_frame gSpriteBank38Frame017 = SPRITE_FRAME(gSpriteBank38Frame017, SPRITE_TILES_BANK38 + 0x016c0);
const struct sprite_frame gSpriteBank38Frame018 = SPRITE_FRAME(gSpriteBank38Frame018, SPRITE_TILES_BANK38 + 0x017c0);
const struct sprite_frame gSpriteBank38Frame019 = SPRITE_FRAME(gSpriteBank38Frame019, SPRITE_TILES_BANK38 + 0x01940);
const struct sprite_frame gSpriteBank38Frame020 = SPRITE_FRAME(gSpriteBank38Frame020, SPRITE_TILES_BANK38 + 0x01ac0);
const struct sprite_frame gSpriteBank38Frame021 = SPRITE_FRAME(gSpriteBank38Frame021, SPRITE_TILES_BANK38 + 0x01cc0);
const struct sprite_frame gSpriteBank38Frame022 = SPRITE_FRAME(gSpriteBank38Frame022, SPRITE_TILES_BANK38 + 0x01ec0);
const struct sprite_frame gSpriteBank38Frame023 = SPRITE_FRAME(gSpriteBank38Frame023, SPRITE_TILES_BANK38 + 0x020c0);
const struct sprite_frame gSpriteBank38Frame024 = SPRITE_FRAME(gSpriteBank38Frame024, SPRITE_TILES_BANK38 + 0x022c0);
const struct sprite_frame gSpriteBank38Frame025 = SPRITE_FRAME(gSpriteBank38Frame025, SPRITE_TILES_BANK38 + 0x024c0);
const struct sprite_frame gSpriteBank38Frame026 = SPRITE_FRAME(gSpriteBank38Frame026, SPRITE_TILES_BANK38 + 0x02640);
const struct sprite_frame gSpriteBank38Frame027 = SPRITE_FRAME(gSpriteBank38Frame027, SPRITE_TILES_BANK38 + 0x027c0);
const struct sprite_frame gSpriteBank38Frame028 = SPRITE_FRAME(gSpriteBank38Frame028, SPRITE_TILES_BANK38 + 0x02900);
const struct sprite_frame gSpriteBank38Frame029 = SPRITE_FRAME(gSpriteBank38Frame029, SPRITE_TILES_BANK38 + 0x02a00);
const struct sprite_frame gSpriteBank38Frame030 = SPRITE_FRAME(gSpriteBank38Frame030, SPRITE_TILES_BANK38 + 0x02a80);
const struct sprite_frame gSpriteBank38Frame031 = SPRITE_FRAME(gSpriteBank38Frame031, SPRITE_TILES_BANK38 + 0x02b00);
const struct sprite_frame gSpriteBank38Frame032 = SPRITE_FRAME(gSpriteBank38Frame032, SPRITE_TILES_BANK38 + 0x02c00);
const struct sprite_frame gSpriteBank38Frame033 = SPRITE_FRAME(gSpriteBank38Frame033, SPRITE_TILES_BANK38 + 0x02d00);
const struct sprite_frame gSpriteBank38Frame034 = SPRITE_FRAME(gSpriteBank38Frame034, SPRITE_TILES_BANK38 + 0x02e80);
const struct sprite_frame gSpriteBank38Frame035 = SPRITE_FRAME(gSpriteBank38Frame035, SPRITE_TILES_BANK38 + 0x03000);
const struct sprite_frame gSpriteBank38Frame036 = SPRITE_FRAME(gSpriteBank38Frame036, SPRITE_TILES_BANK38 + 0x03200);
const struct sprite_frame gSpriteBank38Frame037 = SPRITE_FRAME(gSpriteBank38Frame037, SPRITE_TILES_BANK38 + 0x03400);
const struct sprite_frame gSpriteBank38Frame038 = SPRITE_FRAME(gSpriteBank38Frame038, SPRITE_TILES_BANK38 + 0x03600);
const struct sprite_frame gSpriteBank38Frame039 = SPRITE_FRAME(gSpriteBank38Frame039, SPRITE_TILES_BANK38 + 0x03800);
const struct sprite_frame gSpriteBank38Frame040 = SPRITE_FRAME(gSpriteBank38Frame040, SPRITE_TILES_BANK38 + 0x03a00);
const struct sprite_frame gSpriteBank38Frame041 = SPRITE_FRAME(gSpriteBank38Frame041, SPRITE_TILES_BANK38 + 0x03b80);
const struct sprite_frame gSpriteBank38Frame042 = SPRITE_FRAME(gSpriteBank38Frame042, SPRITE_TILES_BANK38 + 0x03d00);
const struct sprite_frame gSpriteBank38Frame043 = SPRITE_FRAME(gSpriteBank38Frame043, SPRITE_TILES_BANK38 + 0x03e40);
const struct sprite_frame gSpriteBank38Frame044 = SPRITE_FRAME(gSpriteBank38Frame044, SPRITE_TILES_BANK38 + 0x03f40);
const struct sprite_frame gSpriteBank38Frame045 = SPRITE_FRAME(gSpriteBank38Frame045, SPRITE_TILES_BANK38 + 0x03fc0);
const struct sprite_frame gSpriteBank38Frame046 = SPRITE_FRAME(gSpriteBank38Frame046, SPRITE_TILES_BANK38 + 0x04040);
const struct sprite_frame gSpriteBank38Frame047 = SPRITE_FRAME(gSpriteBank38Frame047, SPRITE_TILES_BANK38 + 0x04140);
const struct sprite_frame gSpriteBank38Frame048 = SPRITE_FRAME(gSpriteBank38Frame048, SPRITE_TILES_BANK38 + 0x04240);
const struct sprite_frame gSpriteBank38Frame049 = SPRITE_FRAME(gSpriteBank38Frame049, SPRITE_TILES_BANK38 + 0x043c0);
const struct sprite_frame gSpriteBank38Frame050 = SPRITE_FRAME(gSpriteBank38Frame050, SPRITE_TILES_BANK38 + 0x04540);
const struct sprite_frame gSpriteBank38Frame051 = SPRITE_FRAME(gSpriteBank38Frame051, SPRITE_TILES_BANK38 + 0x04740);
const struct sprite_frame gSpriteBank38Frame052 = SPRITE_FRAME(gSpriteBank38Frame052, SPRITE_TILES_BANK38 + 0x04940);
const struct sprite_frame gSpriteBank38Frame053 = SPRITE_FRAME(gSpriteBank38Frame053, SPRITE_TILES_BANK38 + 0x04b40);
const struct sprite_frame gSpriteBank38Frame054 = SPRITE_FRAME(gSpriteBank38Frame054, SPRITE_TILES_BANK38 + 0x04d40);
const struct sprite_frame gSpriteBank38Frame055 = SPRITE_FRAME(gSpriteBank38Frame055, SPRITE_TILES_BANK38 + 0x04f40);
const struct sprite_frame gSpriteBank38Frame056 = SPRITE_FRAME(gSpriteBank38Frame056, SPRITE_TILES_BANK38 + 0x050c0);
const struct sprite_frame gSpriteBank38Frame057 = SPRITE_FRAME(gSpriteBank38Frame057, SPRITE_TILES_BANK38 + 0x05240);
const struct sprite_frame gSpriteBank38Frame058 = SPRITE_FRAME(gSpriteBank38Frame058, SPRITE_TILES_BANK38 + 0x05380);
const struct sprite_frame gSpriteBank38Frame059 = SPRITE_FRAME(gSpriteBank38Frame059, SPRITE_TILES_BANK38 + 0x05480);

const struct sprite_piece_pos gSpriteBank38Frame000Pos[1] = { { -2, -14 } };
const struct sprite_piece_pos gSpriteBank38Frame001Pos[2] = { { -4, -14 }, { 4, -12 } };
const struct sprite_piece_pos gSpriteBank38Frame002Pos[2] = { { -6, -14 }, { 2, -13 } };
const struct sprite_piece_pos gSpriteBank38Frame003Pos[2] = { { -7, -14 }, { 9, -10 } };
const struct sprite_piece_pos gSpriteBank38Frame004Pos[2] = { { -9, -14 }, { 7, -13 } };
const struct sprite_piece_pos gSpriteBank38Frame005Pos[1] = { { -11, -14 } };
const struct sprite_piece_pos gSpriteBank38Frame006Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank38Frame007Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank38Frame008Pos[1] = { { -13, -13 } };
const struct sprite_piece_pos gSpriteBank38Frame009Pos[1] = { { -13, -13 } };
const struct sprite_piece_pos gSpriteBank38Frame010Pos[2] = { { -12, -13 }, { 4, -14 } };
const struct sprite_piece_pos gSpriteBank38Frame011Pos[2] = { { -10, -13 }, { 6, -14 } };
const struct sprite_piece_pos gSpriteBank38Frame012Pos[2] = { { -9, -14 }, { 7, -11 } };
const struct sprite_piece_pos gSpriteBank38Frame013Pos[2] = { { -7, -13 }, { 1, -14 } };
const struct sprite_piece_pos gSpriteBank38Frame014Pos[1] = { { -4, -14 } };
const struct sprite_piece_pos gSpriteBank38Frame015Pos[1] = { { -2, -14 } };
const struct sprite_piece_pos gSpriteBank38Frame016Pos[2] = { { -4, -14 }, { 4, -12 } };
const struct sprite_piece_pos gSpriteBank38Frame017Pos[2] = { { -6, -14 }, { 2, -13 } };
const struct sprite_piece_pos gSpriteBank38Frame018Pos[2] = { { -7, -14 }, { 9, -10 } };
const struct sprite_piece_pos gSpriteBank38Frame019Pos[2] = { { -9, -14 }, { 7, -13 } };
const struct sprite_piece_pos gSpriteBank38Frame020Pos[1] = { { -11, -14 } };
const struct sprite_piece_pos gSpriteBank38Frame021Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank38Frame022Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank38Frame023Pos[1] = { { -13, -13 } };
const struct sprite_piece_pos gSpriteBank38Frame024Pos[1] = { { -13, -13 } };
const struct sprite_piece_pos gSpriteBank38Frame025Pos[2] = { { -12, -13 }, { 4, -14 } };
const struct sprite_piece_pos gSpriteBank38Frame026Pos[2] = { { -10, -13 }, { 6, -14 } };
const struct sprite_piece_pos gSpriteBank38Frame027Pos[2] = { { -9, -14 }, { 7, -11 } };
const struct sprite_piece_pos gSpriteBank38Frame028Pos[2] = { { -7, -13 }, { 1, -14 } };
const struct sprite_piece_pos gSpriteBank38Frame029Pos[1] = { { -4, -14 } };
const struct sprite_piece_pos gSpriteBank38Frame030Pos[1] = { { -2, -14 } };
const struct sprite_piece_pos gSpriteBank38Frame031Pos[2] = { { -4, -14 }, { 4, -12 } };
const struct sprite_piece_pos gSpriteBank38Frame032Pos[2] = { { -6, -14 }, { 2, -13 } };
const struct sprite_piece_pos gSpriteBank38Frame033Pos[2] = { { -7, -14 }, { 9, -10 } };
const struct sprite_piece_pos gSpriteBank38Frame034Pos[2] = { { -9, -14 }, { 7, -13 } };
const struct sprite_piece_pos gSpriteBank38Frame035Pos[1] = { { -11, -14 } };
const struct sprite_piece_pos gSpriteBank38Frame036Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank38Frame037Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank38Frame038Pos[1] = { { -13, -13 } };
const struct sprite_piece_pos gSpriteBank38Frame039Pos[1] = { { -13, -13 } };
const struct sprite_piece_pos gSpriteBank38Frame040Pos[2] = { { -12, -13 }, { 4, -14 } };
const struct sprite_piece_pos gSpriteBank38Frame041Pos[2] = { { -10, -13 }, { 6, -14 } };
const struct sprite_piece_pos gSpriteBank38Frame042Pos[2] = { { -9, -14 }, { 7, -11 } };
const struct sprite_piece_pos gSpriteBank38Frame043Pos[2] = { { -7, -13 }, { 1, -14 } };
const struct sprite_piece_pos gSpriteBank38Frame044Pos[1] = { { -4, -14 } };
const struct sprite_piece_pos gSpriteBank38Frame045Pos[1] = { { -2, -14 } };
const struct sprite_piece_pos gSpriteBank38Frame046Pos[2] = { { -4, -14 }, { 4, -12 } };
const struct sprite_piece_pos gSpriteBank38Frame047Pos[2] = { { -6, -14 }, { 2, -13 } };
const struct sprite_piece_pos gSpriteBank38Frame048Pos[2] = { { -7, -14 }, { 9, -10 } };
const struct sprite_piece_pos gSpriteBank38Frame049Pos[2] = { { -9, -14 }, { 7, -13 } };
const struct sprite_piece_pos gSpriteBank38Frame050Pos[1] = { { -11, -14 } };
const struct sprite_piece_pos gSpriteBank38Frame051Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank38Frame052Pos[1] = { { -12, -13 } };
const struct sprite_piece_pos gSpriteBank38Frame053Pos[1] = { { -13, -13 } };
const struct sprite_piece_pos gSpriteBank38Frame054Pos[1] = { { -13, -13 } };
const struct sprite_piece_pos gSpriteBank38Frame055Pos[2] = { { -12, -13 }, { 4, -14 } };
const struct sprite_piece_pos gSpriteBank38Frame056Pos[2] = { { -10, -13 }, { 6, -14 } };
const struct sprite_piece_pos gSpriteBank38Frame057Pos[2] = { { -9, -14 }, { 7, -11 } };
const struct sprite_piece_pos gSpriteBank38Frame058Pos[2] = { { -7, -13 }, { 1, -14 } };
const struct sprite_piece_pos gSpriteBank38Frame059Pos[1] = { { -4, -14 } };

const u8 gSpriteBank38Frame000Pieces[1] = { SPRITE_PIECE(1, 9) };
const u8 gSpriteBank38Frame001Pieces[2] = { SPRITE_PIECE(1, 9), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank38Frame002Pieces[2] = { SPRITE_PIECE(1, 9), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank38Frame003Pieces[2] = { SPRITE_PIECE(1, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank38Frame004Pieces[2] = { SPRITE_PIECE(1, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank38Frame005Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank38Frame006Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank38Frame007Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank38Frame008Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank38Frame009Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank38Frame010Pieces[2] = { SPRITE_PIECE(1, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank38Frame011Pieces[2] = { SPRITE_PIECE(1, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank38Frame012Pieces[2] = { SPRITE_PIECE(1, 10), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank38Frame013Pieces[2] = { SPRITE_PIECE(1, 9), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank38Frame014Pieces[1] = { SPRITE_PIECE(1, 9) };
const u8 gSpriteBank38Frame015Pieces[1] = { SPRITE_PIECE(1, 9) };
const u8 gSpriteBank38Frame016Pieces[2] = { SPRITE_PIECE(1, 9), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank38Frame017Pieces[2] = { SPRITE_PIECE(1, 9), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank38Frame018Pieces[2] = { SPRITE_PIECE(1, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank38Frame019Pieces[2] = { SPRITE_PIECE(1, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank38Frame020Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank38Frame021Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank38Frame022Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank38Frame023Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank38Frame024Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank38Frame025Pieces[2] = { SPRITE_PIECE(1, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank38Frame026Pieces[2] = { SPRITE_PIECE(1, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank38Frame027Pieces[2] = { SPRITE_PIECE(1, 10), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank38Frame028Pieces[2] = { SPRITE_PIECE(1, 9), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank38Frame029Pieces[1] = { SPRITE_PIECE(1, 9) };
const u8 gSpriteBank38Frame030Pieces[1] = { SPRITE_PIECE(1, 9) };
const u8 gSpriteBank38Frame031Pieces[2] = { SPRITE_PIECE(1, 9), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank38Frame032Pieces[2] = { SPRITE_PIECE(1, 9), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank38Frame033Pieces[2] = { SPRITE_PIECE(1, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank38Frame034Pieces[2] = { SPRITE_PIECE(1, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank38Frame035Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank38Frame036Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank38Frame037Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank38Frame038Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank38Frame039Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank38Frame040Pieces[2] = { SPRITE_PIECE(1, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank38Frame041Pieces[2] = { SPRITE_PIECE(1, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank38Frame042Pieces[2] = { SPRITE_PIECE(1, 10), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank38Frame043Pieces[2] = { SPRITE_PIECE(1, 9), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank38Frame044Pieces[1] = { SPRITE_PIECE(1, 9) };
const u8 gSpriteBank38Frame045Pieces[1] = { SPRITE_PIECE(1, 9) };
const u8 gSpriteBank38Frame046Pieces[2] = { SPRITE_PIECE(1, 9), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank38Frame047Pieces[2] = { SPRITE_PIECE(1, 9), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank38Frame048Pieces[2] = { SPRITE_PIECE(1, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank38Frame049Pieces[2] = { SPRITE_PIECE(1, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank38Frame050Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank38Frame051Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank38Frame052Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank38Frame053Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank38Frame054Pieces[1] = { SPRITE_PIECE(1, 2) };
const u8 gSpriteBank38Frame055Pieces[2] = { SPRITE_PIECE(1, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank38Frame056Pieces[2] = { SPRITE_PIECE(1, 10), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank38Frame057Pieces[2] = { SPRITE_PIECE(1, 10), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank38Frame058Pieces[2] = { SPRITE_PIECE(1, 9), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank38Frame059Pieces[1] = { SPRITE_PIECE(1, 9) };
