#include "core.h"

/* A small counter/threshold pair on the composite pause/options screen's
 * settings-row sub-widget (the big object sub_8004EC0 constructs,
 * fields 0x88 upward - still raw as a whole; see docs/matching.md's
 * write-up for this chunk). Only the two fields these two functions
 * touch are named, the same minimal-local-type convention
 * src/graphics/settings_menu2.c's `struct bg_widget` uses. */
struct row_counter_widget {
    u8 unused_00[0x18];
    s32 field_18;
    s32 field_1c;
};

extern s32 sub_803AE4C(s32 arg0, s32 arg1);

/* Bumps `field_18` by one, then re-clamps it against `field_1c` via
 * sub_803AE4C (still raw - reads like a generic "wrap/clamp counter"
 * helper, seen throughout this chunk). */
void sub_8006084(struct row_counter_widget *self)
{
    self->field_18 = self->field_18 + 1;
    self->field_18 = sub_803AE4C(self->field_18, self->field_1c);
}

/* Counterpart to sub_8006084 above: decrements `field_18`, wrapping
 * around to `field_1c` first when it's already at zero. */
s32 sub_800609C(struct row_counter_widget *self)
{
    s32 v = self->field_18;
    if (v == 0) {
        v = self->field_1c;
    }
    v -= 1;
    self->field_18 = v;
    return v;
}
