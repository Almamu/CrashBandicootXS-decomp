extern "C" {
#include "gba/types.h"
}

/*
 * ROM 0x085A4E70-0x085A6100: the glyph tiles of the two HUD fonts
 * (src/text/font_glyph.cpp). OAM picks their palette at run time, so the
 * PNGs carry a grayscale placeholder one (docs/graphics.md, "Placeholder
 * palettes").
 * Every asset is an LZ77 stream (gbagfx) that starts on a word boundary,
 * hence aligned(4): the 0-3 zero bytes after a stream are the next one's
 * alignment. tools/bin2c.py --lz writes the stream's bytes (graphics.mk).
 * Linked in ROM order between data/data.s sections by ldscript.txt, see
 * docs/data.md.
 */

/* The small HUD font's 4bpp glyph tiles: graphics/intro/00_5a4e70_tiles.png. */
extern const u8 gSmallFontTiles[] __attribute__((aligned(4))) = {
#include "intro/00_5a4e70_tiles.img.bin.lz.inc"
};

/* The large HUD font's 4bpp glyph tiles: graphics/intro/00_5a551c_tiles.png. */
extern const u8 gLargeFontTiles[] __attribute__((aligned(4))) = {
#include "intro/00_5a551c_tiles.img.bin.lz.inc"
};
