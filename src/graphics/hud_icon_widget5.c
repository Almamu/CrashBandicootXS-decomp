#include "core.h"
#include "icon_manager.h"

/* Sits between the parked sub_8028A78 (asm/code_3_2_20_8a78.s) and the
 * still-raw InitHudTextWidget (asm/code_3_2_20_8b7c.s, out of this
 * chunk's scope) - sub_8028AC4 through sub_8028B58, GitHub issue #46.
 * Same `struct icon_manager` as hud_icon_widget.c/hud_icon_widget2.c/
 * hud_icon_widget3.c/hud_icon_widget4.c. */

extern void *sub_803AD7C(void *arg0, void *arg1);
extern s32 sub_8037E54(s32 value, s32 divisor);

/* Divides `value` by the widget's own line height (`field_11c`) - see
 * src/util/word_util.c's sub_8001214, which uses this same field as a
 * divisor for a line-count limit. */
s32 sub_8028AC4(struct icon_manager *self, s32 value)
{
    return sub_8037E54(value, self->field_11c);
}

/* Trivial getter/setter pairs around `struct icon_manager`'s fields -
 * used by callers elsewhere in the still-raw HUD text/icon-widget
 * driver code. */
u32 sub_8028ADC(struct icon_manager *self)
{
    return self->field_12c;
}

void sub_8028AE8(struct icon_manager *self, u32 x, u32 y)
{
    self->posX = x;
    self->posY = y;
}

void sub_8028B04(struct icon_manager *self, u32 y)
{
    self->posX = self->field_118;
    self->posY = y;
}

u32 sub_8028B28(struct icon_manager *self)
{
    return self->field_118;
}

void sub_8028B34(struct icon_manager *self, u32 val)
{
    self->field_118 = val;
}

u32 sub_8028B40(struct icon_manager *self)
{
    return self->posY;
}

u32 sub_8028B4C(struct icon_manager *self)
{
    return self->posX;
}

/* Sets `field_108`, then forwards to `record`'s slot-6 trampoline (see
 * include/icon_manager.h's `struct icon_record`) via `sub_803AD7C`,
 * discarding its result. */
void sub_8028B58(struct icon_manager *self, u32 val)
{
    struct icon_slot *slot;

    self->field_108 = val;
    slot = &self->record->slots[6];
    sub_803AD7C((u8 *)self + slot->offset, slot->ptr);
}
