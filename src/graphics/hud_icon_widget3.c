#include "core.h"
#include "icon_manager.h"

/* Sits between sub_8028890/sub_8028900 (src/graphics/hud_icon_widget_8890.c)
 * and MeasureText (src/graphics/hud_icon_widget_8994.c) - just
 * sub_8028968 here, GitHub issue #46. Same `struct icon_manager` as
 * hud_icon_widget.c/hud_icon_widget2.c/hud_icon_widget4.c/
 * hud_icon_widget5.c. */

/* Sums `field_11c` (line height) once for the first line plus once more
 * per newline in `str` - a "total text block height" helper. */
s32 sub_8028968(struct icon_manager *self, u8 *str)
{
    s32 total = self->field_11c;
    u8 c;

    for (c = *str; c != 0; c = *++str) {
        if (c == '\n') {
            total += self->field_11c;
        }
    }
    return total;
}
asm(".align 2, 0");
