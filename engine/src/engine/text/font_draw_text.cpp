#include "engine/text/font.hpp"

using Engine::Text::Font;

/* GitHub issue #46: Font's whole-string draw and fixed-count measure
 * (engine/include/engine/text/font.hpp). Built with old_agbcp, which MeasureChars needs. */

#ifndef GAME_N2
/* (N-Tranced's DrawText, which has control codes too, is elsewhere:
 * 0x08028F04) */
/* Draws a NUL-terminated string: PutChar's per-character logic in a loop,
 * newline and space handled in place, every other character drawn
 * (DrawGlyph, virtual). */
void Font::DrawText(const u8 *str)
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
#endif

/* Sums the advance width of `count` characters starting at `str`
 * (MeasureText's fixed-count sibling): newline contributes nothing,
 * space contributes `spaceWidth`, everything else contributes
 * `glyphRecords[charLookup[c]].width`. */
#ifdef GAME_N2
/* N-Tranced's (0x08029298): its text has control codes 1-3, which take
 * no space, and it switches on the byte itself (signed compares, and
 * the glyph lookup reads it again) */
screen_w_t Font::MeasureChars(const u8 *str, s32 count)
{
    screen_w_t total = 0;
    s32 i;

    for (i = 0; i < count; i++) {
        switch (str[i]) {
        case ' ':
            total += spaceWidth;
            break;
        case 1:
        case 2:
        case 3:
        case '\n':
            break;
        default:
            total += glyphRecords[charLookup[str[i]]].width;
            break;
        }
    }
    return total;
}
#else
screen_w_t Font::MeasureChars(const u8 *str, s32 count)
{
    screen_w_t total = 0;
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
#endif
