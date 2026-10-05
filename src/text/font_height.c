#include "core.h"
#include "bitmap_font.h"

/* Sits between FontDrawText/FontMeasureChars (src/text/font_draw_text.c)
 * and FontMeasureText (src/text/font_measure.c) - just
 * FontTextHeight here, GitHub issue #46. Same `struct bitmap_font` as
 * hud_icon_widget.c and the other src/text/font*.c files. */

/* Sums `lineHeight` (line height) once for the first line plus once more
 * per newline in `str` - a "total text block height" helper. */
s32 FontTextHeight(struct bitmap_font *self, u8 *str)
{
    s32 total = self->lineHeight;
    u8 c;

    for (c = *str; c != 0; c = *++str) {
        if (c == '\n') {
            total += self->lineHeight;
        }
    }
    return total;
}
asm(".align 2, 0");
