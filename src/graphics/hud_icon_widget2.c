#include "core.h"
#include "icon_manager.h"

/* Sits between sub_80285C4/InitHudIconWidgetA/InitHudIconWidgetB/
 * sub_8028808 (src/graphics/hud_icon_widget_85c4.c) and sub_8028890/
 * sub_8028900 (src/graphics/hud_icon_widget_8890.c) - just sub_8028860
 * here, GitHub issue #46. Same `struct icon_manager` text/icon-glyph
 * renderer as hud_icon_widget.c/hud_icon_widget3.c/hud_icon_widget4.c/
 * hud_icon_widget5.c. */

extern s32 sub_803AD80(void *arg0, s32 arg1, void *arg2);

/* Draws `count` characters from `str` via `record`'s slot-5 trampoline
 * (`sub_8028808`, parked in asm/code_3_2_20_85c4.s, per the widget's own
 * vtable). */
void sub_8028860(struct icon_manager *self, u8 *str, s32 count)
{
    if (count > 0) {
        s32 remaining = count;

        do {
            struct icon_slot *slot = &self->record->slots[5];
            sub_803AD80((u8 *)self + slot->offset, *str, slot->ptr);
            str++;
            remaining--;
        } while (remaining != 0);
    }
}
