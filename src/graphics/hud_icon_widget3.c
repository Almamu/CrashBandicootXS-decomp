#include "core.h"
#include "icon_manager.h"

/* Sits between the parked sub_8028890/sub_8028900 (asm/code_3_2_20_8890.s)
 * and the parked MeasureText (asm/code_3_2_20_8994.s) - just
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
