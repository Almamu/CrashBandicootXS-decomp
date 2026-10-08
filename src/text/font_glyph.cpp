#include "sprite_obj.hpp"
#include "font.hpp"

extern "C" {
#include "match.h"
#include "gfx.h"
#include "globals.h"
}

/* GitHub issue #46: Font's glyph drawer and character dispatcher, and the
 * two fonts' constructors (include/font.hpp). Built with old_agbcp: under
 * agbcp, DrawGlyph derives its bitfield masks differently. */

/* The value arrives as a parameter so old_agbcp loads the 0x1ff mask
 * from the literal pool, as the ROM does. */
static inline void SetGlyphX(struct oam_attrs *oam, s32 x)
{
    oam->x = x;
}

/* Builds one glyph's draw request in `oam_scratch` from
 * `glyphRecords[charLookup[charByte]]` and the cursor, draws it with
 * AddOamEntry, then advances `posX` by the glyph's width. */
void Font::DrawGlyph(u8 charByte)
{
    struct oam_attrs *oam = (struct oam_attrs *)oam_scratch;
    u8 glyph = charLookup[charByte];

    SetGlyphX(oam, posX);
    {
        u8 *y = (u8 *)&posY;

        oam->y = glyphRecords[glyph].yOffset + *y;
    }
    oam->shape = glyphRecords[glyph].shape;
    oam->tileNum = tileBase + glyph * glyphTileStride;
    gOamBuffer->Add(this);
    posX += glyphRecords[glyph].width;
}

/* gSmallFont: 9-pixel lines, 4-pixel spaces, glyph stride 2, and a
 * charLookup built from the gSmallFontChars font order table (see
 * include/bitmap_font.h). */
SmallFont::SmallFont()
{
    u32 i;
    u32 j;

    lineHeight = 9;
    spaceWidth = 4;
    tiles = gSmallFontTiles;
    glyphTileStride = 2;
    glyphRecords = gSmallFontGlyphs;
    for (i = 0; i <= 0xff; i++) {
        charLookup[i] = 0;
        for (j = 0; j <= 0x4f; j++) {
            if (gSmallFontChars[j] == i) {
                charLookup[i] = j;
                break;
            }
        }
    }
}

/* gLargeFont: 16-pixel lines, 6-pixel spaces, glyph stride 4, OBJ size
 * 1, and a charLookup built from the gLargeFontChars font order table. */
LargeFont::LargeFont()
{
    u32 i;
    u32 j;

    lineHeight = 0x10;
    spaceWidth = 6;
    glyphTileStride = 4;
    glyphRecords = gLargeFontGlyphs;
    tiles = gLargeFontTiles;
    ((struct oam_attrs *)oam_scratch)->size = 1;
    for (i = 0; i <= 0xff; i++) {
        charLookup[i] = 0;
        for (j = 0; j <= 0x4b; j++) {
            if (gLargeFontChars[j] == i) {
                charLookup[i] = j;
                break;
            }
        }
    }
}

/* Draws or handles one character: newline resets `posX` to the left
 * margin and advances `posY` by one line height; space just advances
 * `posX` by `spaceWidth`; anything else is drawn (DrawGlyph, virtual). */
void Font::PutChar(u32 charByte)
{
    u8 c = charByte;

    switch (c) {
    case ' ':
        posX += spaceWidth;
        break;
    case '\n':
        posX = marginX;
        posY += lineHeight;
        break;
    default:
        DrawGlyph(c);
        break;
    }
}
