#include "font.hpp"

/* GitHub issue #46: Font's whole-string draw and fixed-count measure
 * (include/font.hpp). Built with old_agbcp, which MeasureChars needs. */

/* Draws a NUL-terminated string: PutChar's per-character logic in a loop,
 * newline and space handled in place, every other character drawn
 * (DrawGlyph, virtual). */
void Font::DrawText(u8 *str)
{
    u8 c;

    for (c = *str; c != 0; c = *++str) {
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
}

/* Sums the advance width of `count` characters starting at `str`
 * (MeasureText's fixed-count sibling): newline contributes nothing,
 * space contributes `spaceWidth`, everything else contributes
 * `glyphRecords[charLookup[c]].width`. */
s32 Font::MeasureChars(u8 *str, s32 count)
{
    s32 total = 0;
    s32 i;

    for (i = 0; i < count; i++) {
        u32 c = str[i];

        switch (c) {
        case ' ':
            total += spaceWidth;
            break;
        case '\n':
            break;
        default:
            total += glyphRecords[charLookup[c]].width;
            break;
        }
    }
    return total;
}
