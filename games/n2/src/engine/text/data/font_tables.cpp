#include "engine/text/font.hpp"

extern "C" {
#include "core.h"
}

using Engine::Text::font_desc;
using Engine::Text::font_glyph;

/*
 * N-Tranced's ROM 0x081DEC60-0x081DF520: its two fonts' tables (N-Tranced's
 * Font, games/n2/src/engine/text/font.cpp, docs/n_tranced.md "Struct
 * layouts"): each font's characters in glyph order (a character's glyph is
 * its position here) and its struct font_desc, which its startup passes
 * to the Font constructor (the small font: gSmallFont; the large one:
 * gLargeFont); the palettes text codes 2 and 3 and the button glyphs
 * (A, B, L, R) draw with; then each font's glyph records. The tiles,
 * gSmallFontTiles and gLargeFontTiles, are still the ROM's (0x085293D0,
 * 0x0852A810). Latin-1, as XS's (hud_font_tables.cpp).
 */

extern "C" const u8 gSmallFontTiles[];
extern "C" const u8 gLargeFontTiles[];
extern "C" const struct font_glyph gSmallFontGlyphs[80];
extern "C" const struct font_glyph gLargeFontGlyphs[80];

/* The small font: 9-pixel lines, 4-pixel spaces, 2 tiles per glyph. */
extern "C" const u8 gSmallFontChars[81] = {
    'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',
    'o',  'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z',  '0',  '1',
    '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  '-',  '+',  '!',  ':',  ',',  '?',
    '.',  '\'', '%',  '<',  '>',  0xE4, 0xE0, 0xE1, 0xE2, 0xEB, 0xE8, 0xE9, 0xEA, 0xEF,
    0xEC, 0xED, 0xEE, 0xF6, 0xF2, 0xF3, 0xF4, 0xFC, 0xF9, 0xFA, 0xFB, 0xF1, 0xDF, 0xE7,
    0xBF, 0xA1, 0xB4, '?',  0x9C, 'A',  'B',  'L',  'R',  '_',  0x00,
};

extern "C" const struct font_desc gSmallFontDesc = {
    9, 4, 2, 0, gSmallFontTiles, gSmallFontChars, 81, gSmallFontGlyphs,
};

/* The large font: 16-pixel lines, 6-pixel spaces, 4 tiles per glyph, OBJ
 * size 1 (16x16). */
extern "C" const u8 gLargeFontChars[76] = {
    'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',  'p',
    'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z',  '0',  '1',  '2',  '3',  '4',  '5',
    '6',  '7',  '8',  '9',  '+',  '-',  ':',  '_',  ',',  '.',  '\'', '!',  '?',  '%',  0xE4, 0xE0,
    0xE1, 0xE2, 0xEB, 0xE8, 0xE9, 0xEA, 0xEF, 0xEC, 0xED, 0xEE, 0xF6, 0xF2, 0xF3, 0xF4, 0xFC, 0xF9,
    0xFA, 0xFB, 0xF1, 0x9C, 0xBF, 0xA1, 0xDF, 0xB4, '?',  '/',  0xE7, 0x00,
};

extern "C" const struct font_desc gLargeFontDesc = {
    16, 6, 4, 1, gLargeFontTiles, gLargeFontChars, 76, gLargeFontGlyphs,
};

/* The glyphs' palette for text code 2. */
extern "C" const u16 gFontCode2Palette[16] = {
    0x0000, 0x9CC6, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* The glyphs' palette for text code 3. */
extern "C" const u16 gFontCode3Palette[16] = {
    0x7C10, 0x9CC6, 0x0FDF, 0x0D04, 0xB7E3, 0x1D82, 0x0864, 0x2E81,
    0x0BD3, 0xA3EE, 0x8BDA, 0x9FF4, 0x886B, 0x94DF, 0x0C9B, 0x0873,
};

/* The glyphs' palette for the button glyphs. */
extern "C" const u16 gFontButtonPalette[16] = {
    0x03E0, 0x18C6, 0x2D6B, 0x18C6, 0x6B5A, 0x2529, 0x0C63, 0x39CE,
    0x56B5, 0x56B5, 0x4210, 0x3DEF, 0x14A5, 0x35AD, 0x294A, 0x1CE7,
};

/* Per glyph its advance, OBJ shape and Y offset. */
extern "C" const struct font_glyph gSmallFontGlyphs[80] = {
    { 7, 2, -8 }, /* a */
    { 7, 2, -8 }, /* b */
    { 7, 2, -8 }, /* c */
    { 7, 2, -8 }, /* d */
    { 7, 2, -8 }, /* e */
    { 7, 2, -8 }, /* f */
    { 7, 2, -8 }, /* g */
    { 7, 2, -8 }, /* h */
    { 4, 2, -8 }, /* i */
    { 7, 2, -8 }, /* j */
    { 7, 2, -8 }, /* k */
    { 7, 2, -8 }, /* l */
    { 7, 2, -8 }, /* m */
    { 7, 2, -8 }, /* n */
    { 7, 2, -8 }, /* o */
    { 7, 2, -8 }, /* p */
    { 7, 2, -8 }, /* q */
    { 7, 2, -8 }, /* r */
    { 7, 2, -8 }, /* s */
    { 7, 2, -8 }, /* t */
    { 7, 2, -8 }, /* u */
    { 7, 2, -8 }, /* v */
    { 7, 2, -8 }, /* w */
    { 7, 2, -8 }, /* x */
    { 7, 2, -8 }, /* y */
    { 7, 2, -8 }, /* z */
    { 7, 2, -8 }, /* 0 */
    { 6, 2, -8 }, /* 1 */
    { 7, 2, -8 }, /* 2 */
    { 7, 2, -8 }, /* 3 */
    { 6, 2, -8 }, /* 4 */
    { 6, 2, -8 }, /* 5 */
    { 6, 2, -8 }, /* 6 */
    { 6, 2, -8 }, /* 7 */
    { 6, 2, -8 }, /* 8 */
    { 6, 2, -8 }, /* 9 */
    { 6, 2, -8 }, /* - */
    { 6, 2, -8 }, /* + */
    { 5, 2, -8 }, /* ! */
    { 3, 2, -8 }, /* : */
    { 3, 2, -6 }, /* , */
    { 6, 2, -8 }, /* ? */
    { 2, 2, -8 }, /* . */
    { 3, 2, -9 }, /* ' */
    { 7, 2, -8 }, /* % */
    { 7, 2, -8 }, /* < */
    { 7, 2, -8 }, /* > */
    { 7, 2, -8 }, /* \344 */
    { 7, 2, -8 }, /* \340 */
    { 7, 2, -8 }, /* \341 */
    { 7, 2, -8 }, /* \342 */
    { 7, 2, -8 }, /* \353 */
    { 7, 2, -8 }, /* \350 */
    { 7, 2, -8 }, /* \351 */
    { 7, 2, -8 }, /* \352 */
    { 6, 2, -8 }, /* \357 */
    { 5, 2, -8 }, /* \354 */
    { 5, 2, -8 }, /* \355 */
    { 5, 2, -8 }, /* \356 */
    { 7, 2, -8 }, /* \366 */
    { 7, 2, -8 }, /* \362 */
    { 7, 2, -8 }, /* \363 */
    { 7, 2, -8 }, /* \364 */
    { 7, 2, -8 }, /* \374 */
    { 7, 2, -8 }, /* \371 */
    { 7, 2, -8 }, /* \372 */
    { 7, 2, -8 }, /* \373 */
    { 7, 2, -8 }, /* \361 */
    { 7, 2, -8 }, /* \337 */
    { 7, 2, -6 }, /* \347 */
    { 7, 2, -8 }, /* \277 */
    { 7, 2, -8 }, /* \241 */
    { 7, 2, -8 }, /* \264 */
    { 7, 2, -8 }, /* ? */
    { 8, 2, -8 }, /* \234 */
    { 7, 2, -8 }, /* A */
    { 7, 2, -8 }, /* B */
    { 15, 1, 0 }, /* L */
    { 15, 1, 0 }, /* R */
    { 6, 2, -8 }, /* _ */
};

/* Per glyph its advance, OBJ shape and Y offset. */
extern "C" const struct font_glyph gLargeFontGlyphs[80] = {
    { 13, 0, 0 },  /* a */
    { 12, 0, 0 },  /* b */
    { 9, 0, 0 },   /* c */
    { 10, 0, 0 },  /* d */
    { 10, 0, 0 },  /* e */
    { 10, 0, 0 },  /* f */
    { 11, 0, 0 },  /* g */
    { 10, 0, 0 },  /* h */
    { 5, 0, 0 },   /* i */
    { 11, 0, 0 },  /* j */
    { 10, 0, 0 },  /* k */
    { 10, 0, 0 },  /* l */
    { 15, 0, 0 },  /* m */
    { 10, 0, 0 },  /* n */
    { 10, 0, 0 },  /* o */
    { 10, 0, 0 },  /* p */
    { 10, 0, 0 },  /* q */
    { 11, 0, 0 },  /* r */
    { 10, 0, 0 },  /* s */
    { 11, 0, 0 },  /* t */
    { 10, 0, 0 },  /* u */
    { 10, 0, 0 },  /* v */
    { 14, 0, 0 },  /* w */
    { 10, 0, 0 },  /* x */
    { 10, 0, 0 },  /* y */
    { 10, 0, 0 },  /* z */
    { 8, 0, 0 },   /* 0 */
    { 8, 0, 0 },   /* 1 */
    { 8, 0, 0 },   /* 2 */
    { 8, 0, 0 },   /* 3 */
    { 8, 0, 0 },   /* 4 */
    { 8, 0, 0 },   /* 5 */
    { 8, 0, 0 },   /* 6 */
    { 8, 0, 0 },   /* 7 */
    { 8, 0, 0 },   /* 8 */
    { 8, 0, 0 },   /* 9 */
    { 8, 0, 0 },   /* + */
    { 7, 0, 0 },   /* - */
    { 5, 0, 0 },   /* : */
    { 14, 0, 1 },  /* _ */
    { 5, 0, 1 },   /* , */
    { 4, 0, 1 },   /* . */
    { 5, 0, -1 },  /* ' */
    { 7, 0, -1 },  /* ! */
    { 8, 0, -1 },  /* ? */
    { 13, 0, -1 }, /* % */
    { 13, 0, -2 }, /* \344 */
    { 13, 0, -2 }, /* \340 */
    { 13, 0, -2 }, /* \341 */
    { 13, 0, -2 }, /* \342 */
    { 10, 0, -2 }, /* \353 */
    { 10, 0, -2 }, /* \350 */
    { 10, 0, -2 }, /* \351 */
    { 10, 0, -2 }, /* \352 */
    { 10, 0, -2 }, /* \357 */
    { 8, 0, -2 },  /* \354 */
    { 8, 0, -2 },  /* \355 */
    { 9, 0, -2 },  /* \356 */
    { 10, 0, -2 }, /* \366 */
    { 10, 0, -2 }, /* \362 */
    { 10, 0, -2 }, /* \363 */
    { 10, 0, -2 }, /* \364 */
    { 10, 0, -2 }, /* \374 */
    { 10, 0, -2 }, /* \371 */
    { 10, 0, -2 }, /* \372 */
    { 10, 0, -2 }, /* \373 */
    { 10, 0, -2 }, /* \361 */
    { 15, 0, -2 }, /* \234 */
    { 8, 0, -1 },  /* \277 */
    { 8, 0, -1 },  /* \241 */
    { 14, 0, 0 },  /* \337 */
    { 10, 0, 0 },  /* \264 */
    { 10, 0, 0 },  /* ? */
    { 11, 0, -1 }, /* / */
    { 8, 0, 1 },   /* \347 */
    { 1818717797, 6845289, 1851880038 },
    { 1936286183, 0, 1953850724 },
    { 6841203, 1634759525, 7106545 },
    { 1818326121, 1869504873, 0 },
    { 1701078382, 1851878514, 29540 },
};
