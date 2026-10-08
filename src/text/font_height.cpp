#include "font.hpp"

/* GitHub issue #46: Font's TextHeight (include/font.hpp), between
 * font_draw_text.cpp and font.cpp in the ROM. */

/* The height of a block of text: `lineHeight` for the first line plus
 * once more per newline in `str`. */
s32 Font::TextHeight(u8 *str)
{
    s32 total = lineHeight;
    u8 c;

    for (c = *str; c != 0; c = *++str) {
        if (c == '\n') {
            total += lineHeight;
        }
    }
    return total;
}
