#include "core.h"
#include "bitmap_font.h"

/* Sits between FontDrawGlyph/InitSmallFont/InitLargeFont/
 * FontPutChar (src/graphics/hud_icon_widget_85c4.c) and FontDrawText/
 * FontMeasureChars (src/graphics/hud_icon_widget_8890.c) - just FontDrawChars
 * here, GitHub issue #46. Same `struct bitmap_font` text/icon-glyph
 * renderer as hud_icon_widget.c/hud_icon_widget3.c/hud_icon_widget4.c/
 * hud_icon_widget5.c. */

extern s32 _call_via_r2(void *arg0, s32 arg1, void *arg2);

/* Draws `count` characters from `str` via `record`'s slot-5 trampoline
 * (`FontPutChar`, parked in asm/code_3_2_20_85c4.s, per the widget's own
 * vtable). */
void FontDrawChars(struct bitmap_font *self, u8 *str, s32 count)
{
    if (count > 0) {
        s32 remaining = count;

        do {
            struct icon_slot *slot = &self->record->slots[5];
            _call_via_r2((u8 *)self + slot->offset, *str, slot->ptr);
            str++;
            remaining--;
        } while (remaining != 0);
    }
}
