extern "C" {
#include "gba/types.h"
}

/*
 * N-Tranced's ROM 0x085293D0-0x0852CD90: its two fonts' glyph tiles, raw
 * (games/n2/graphics/fonts/small.png and large.png, OBJ sheets as XS's
 * fonts', docs/n_tranced.md "Graphics"): its Font uploads a glyph's tiles
 * (glyphTileStride bytes) as it draws it (font.cpp), from these, which
 * the fonts' struct font_desc point at (font_tables.cpp). The small
 * font's 81 glyphs are two tiles each, the large font's 75 four.
 */

extern "C" const u8 gSmallFontTiles[0x1440] = {
#include "fonts/small.img.bin.inc"
};

extern "C" const u8 gLargeFontTiles[0x2580] = {
#include "fonts/large.img.bin.inc"
};
