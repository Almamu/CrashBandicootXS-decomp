#include "graphics.hpp"

/*
 * ROM 0x085A4E70-0x085A6100: the glyph tiles of the two HUD fonts
 * (src/engine/text/font_glyph.cpp), one OBJ per glyph (docs/graphics.md, "OBJ
 * sheets"). OAM picks their palette at run time; the PNGs carry the default
 * one, sprite bank 35's (docs/graphics.md, "Run-time palettes").
 * Each font's tiles are a class of graphics.hpp (SmallFontSheet::tiles,
 * LargeFontSheet::tiles), defined by tools/gfx_manifest.py from
 * graphics/manifest.json (docs/graphics.md, "Graphics classes").
 * Every asset is an LZ77 stream (gbagfx) that starts on a word boundary,
 * hence aligned(4): the 0-3 zero bytes after a stream are the next one's
 * alignment. tools/bin2c.py --lz writes the stream's bytes (graphics.mk).
 * Linked in ROM order between data/data.s sections by ldscript.txt, see
 * docs/data.md.
 */
#include "font_tiles_5a4e70.inc"
