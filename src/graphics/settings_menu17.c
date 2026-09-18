#include "core.h"
#include "actor.h"
#include "icon_manager.h"
#include "pause_screen_results.h"

extern void sub_8008044(struct actor *part);
extern s32 sub_803AE4C(s32 dividend, s32 divisor);
extern void sub_80087C0(struct actor *part);
extern void sub_80087B4(struct actor *part);
extern void sub_800872C(struct actor *part, u8 val);
extern s32 sub_8000E1C(s32 max);

/* A slow reveal/cycle animation over the results screen's icon groups:
 * `field_24` (0-4) selects which group to hide this call (a plain
 * `sub_8008044` per icon, no fade), advancing to the next group every
 * `field_28` (180) calls, wrapping mod 5. Independently, `field_c0`
 * (the row-cursor icon) blinks on its own countdown (`field_c4`):
 * while it's ticking down, just decrement it; once it hits 0, either
 * re-show the icon with a fresh random countdown (0x78-0xef) if it's
 * currently "armed" (`field_38`), or hide it otherwise. */
void sub_8005304(struct pause_screen_results *self)
{
    switch (self->field_24) {
    case 0:
        sub_8008044((struct actor *)self->field_88);
        break;
    case 1: {
        struct settings_icon_actor **p = self->icons8c;
        s32 i;
        for (i = 3; i >= 0; i--) {
            sub_8008044((struct actor *)*p);
            p++;
        }
        break;
    }
    case 2: {
        struct settings_icon_actor **p = self->icons9c;
        s32 i;
        for (i = 4; i >= 0; i--) {
            sub_8008044((struct actor *)*p);
            p++;
        }
        break;
    }
    case 3: {
        struct settings_icon_actor **p = self->iconsB0;
        s32 i;
        for (i = 2; i >= 0; i--) {
            sub_8008044((struct actor *)*p);
            p++;
        }
        break;
    }
    case 4:
        sub_8008044((struct actor *)self->field_bc);
        break;
    }

    self->field_28--;
    if (self->field_28 == 0) {
        self->field_24++;
        self->field_24 = sub_803AE4C(self->field_24, 5);
        self->field_28 = 0xb4;
    }

    {
        s32 *countAddr = &self->field_c4;
        s32 result;

        if (*countAddr != 0) {
            goto decrement;
        }
        {
            struct settings_icon_actor *icon = self->field_c0;

            if (icon->field_38 != 0) {
                icon->frameIndex = 0;
                sub_80087C0((struct actor *)icon);
                sub_80087B4((struct actor *)icon);
                sub_800872C((struct actor *)icon, 0);
                result = (u16)sub_8000E1C(0x78) + 0x78;
                goto store;
            } else {
                sub_8008044((struct actor *)icon);
                goto done;
            }
        }
    decrement:
        result = *countAddr - 1;
    store:
        *countAddr = result;
    done:;
    }
}
/* Trailing byte-padding gotcha (see docs/matching.md/
 * matching_decomp_alignment_fix memory): the ROM pads the gap before
 * the next function (sub_80053F4) with zero bytes (an explicit
 * `.align 2, 0` in the original assembly), but this compiler's own
 * default inter-function padding is a `mov r8, r8` NOP-equivalent
 * instead. */
asm(".align 2, 0");
