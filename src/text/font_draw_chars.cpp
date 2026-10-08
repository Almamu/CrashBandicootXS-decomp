#include "font.hpp"

/* GitHub issue #46: Font's DrawChars (include/font.hpp), between
 * font_glyph.cpp and font_draw_text.cpp in the ROM. */

/* Draws `count` characters from `str`, one PutChar (virtual) each. */
void Font::DrawChars(u8 *str, s32 count)
{
    if (count > 0) {
        s32 remaining = count;

        do {
            PutChar(*str);
            str++;
            remaining--;
        } while (remaining != 0);
    }
}
