#include "core.h"
#include "actor.h"
#include "pause_screen_results.h"

extern s32 sub_80060AC(s32 value, void *dest);
extern void *gUnknown_030012BC;
extern s32 sub_803ADB4(s32 arg0, s32 arg1);
extern void sub_8001B30(void *self, u32 value);
extern void sub_8001B50(void *self, u32 value);
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);

#if NON_MATCHING
/* NOT YET BYTE-MATCHING - reconstructed (semantics understood) but off
 * by several register-letter choices in the digit-formatting tail, the
 * same unresolved gcc-2.9 register-allocation class sub_80049CC
 * (src/graphics/settings_menu.c) and src/graphics/settings_menu6.c's
 * parked functions document; parked the same way. One of a matched
 * pair (sub_8005FBC increments the other way): if the current row is
 * an "editable count" row (state 4 or 5) and its count
 * (`field_60`/`field_64` respectively) is non-zero, decrements it,
 * formats a " <NN%>"-shaped scratch string into `buf57`/`buf4f`, and
 * pushes the new percentage through the matching AudioContext setter
 * (`sub_8001B30`/`sub_8001B50` - see src/audio/audio_context.c). Only
 * the state-5/`field_64` branch also plays the standard SFX cue. */
void sub_8005EF4(struct pause_screen_results *self)
{
    s32 state = *((s32 *)self->field_14 + self->field_18 * 2 + 1);
    s32 count;

    if (state == 4) {
        count = self->field_60;
        if (count == 0) {
            return;
        }
        count--;
        self->field_60 = count;
        self->buf57[0] = 0x20;
        self->buf57[1] = 0x3c;
        {
            s32 len = sub_80060AC(count, &self->buf57[2]);
            self->buf57[2 + len] = 0x25;
            self->buf57[3 + len] = 0x3e;
            self->buf57[4 + len] = 0;
        }
        sub_8001B30(gUnknown_030012BC, sub_803ADB4((count << 8) + 1, 0x14));
    } else if (state == 5) {
        count = self->field_64;
        if (count == 0) {
            return;
        }
        count--;
        self->field_64 = count;
        self->buf4f[0] = 0x20;
        self->buf4f[1] = 0x3c;
        {
            s32 len = sub_80060AC(count, &self->buf4f[2]);
            self->buf4f[2 + len] = 0x25;
            self->buf4f[3 + len] = 0x3e;
            self->buf4f[4 + len] = 0;
        }
        sub_8001B50(gUnknown_030012BC, sub_803ADB4((count << 8) + 1, 0x14));
        PlaySfx(gUnknown_030012BC, 0xe, 0x100);
    }
}

/* Counterpart to sub_8005EF4 above: increments (capped at 0x13)
 * instead of decrementing. */
void sub_8005FBC(struct pause_screen_results *self)
{
    s32 state = *((s32 *)self->field_14 + self->field_18 * 2 + 1);
    s32 count;

    if (state == 4) {
        count = self->field_60;
        if (count > 0x13) {
            return;
        }
        count++;
        self->field_60 = count;
        self->buf57[0] = 0x20;
        self->buf57[1] = 0x3c;
        {
            s32 len = sub_80060AC(count, &self->buf57[2]);
            self->buf57[2 + len] = 0x25;
            self->buf57[3 + len] = 0x3e;
            self->buf57[4 + len] = 0;
        }
        sub_8001B30(gUnknown_030012BC, sub_803ADB4((count << 8) + 1, 0x14));
    } else if (state == 5) {
        count = self->field_64;
        if (count > 0x13) {
            return;
        }
        count++;
        self->field_64 = count;
        self->buf4f[0] = 0x20;
        self->buf4f[1] = 0x3c;
        {
            s32 len = sub_80060AC(count, &self->buf4f[2]);
            self->buf4f[2 + len] = 0x25;
            self->buf4f[3 + len] = 0x3e;
            self->buf4f[4 + len] = 0;
        }
        sub_8001B50(gUnknown_030012BC, sub_803ADB4((count << 8) + 1, 0x14));
        PlaySfx(gUnknown_030012BC, 0xe, 0x100);
    }
}
#endif /* NON_MATCHING */
