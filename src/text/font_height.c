#include "core.h"
#include "bitmap_font.h"

/* Sits between FontDrawText/FontMeasureChars (src/graphics/hud_icon_widget_8890.c)
 * and FontMeasureText (src/graphics/hud_icon_widget_8994.c) - just
 * FontTextHeight here, GitHub issue #46. Same `struct bitmap_font` as
 * hud_icon_widget.c/hud_icon_widget2.c/hud_icon_widget4.c/
 * hud_icon_widget5.c. */

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
