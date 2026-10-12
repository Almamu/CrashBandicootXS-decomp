extern "C" {
#include "gba/types.h"
}

/*
 * ROM 0x085A4E70-0x085A6100: the glyph tiles of the two HUD fonts
 * (src/text/font_glyph.cpp), one OBJ per glyph (docs/graphics.md, "OBJ
 * sheets"). OAM picks their palette at run time; the PNGs carry the default
 * one, sprite bank 35's (docs/graphics.md, "Run-time palettes").
 * Every asset is an LZ77 stream (gbagfx) that starts on a word boundary,
 * hence aligned(4): the 0-3 zero bytes after a stream are the next one's
 * alignment. tools/bin2c.py --lz writes the stream's bytes (graphics.mk).
 * Linked in ROM order between data/data.s sections by ldscript.txt, see
 * docs/data.md.
 */

/* The small HUD font's 4bpp glyph tiles: graphics/fonts/small.png. */
extern const u8 gSmallFontTiles[] __attribute__((aligned(4))) = {
#include "fonts/small.img.bin.lz.inc"
};

/* The large HUD font's 4bpp glyph tiles: graphics/fonts/large.png. */
extern const u8 gLargeFontTiles[] __attribute__((aligned(4))) = {
#include "fonts/large.img.bin.lz.inc"
};
