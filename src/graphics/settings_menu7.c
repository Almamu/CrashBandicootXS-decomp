#include "core.h"
#include "actor.h"
#include "pause_screen_results.h"

extern s32 sub_80060AC(s32 value, void *dest);
extern void *gUnknown_030012BC;
extern void sub_8001B30(void *self, u32 value);
extern void sub_8001B50(void *self, u32 value);
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);

/* sub_803ADB4 is libgcc's `__divsi3`, reached from plain `/`. */
asm(".set __divsi3, sub_803ADB4\n");

/* One entry of the per-row record array at `field_14` (8-byte stride;
 * `type` 4/5 are the two editable-percentage rows). */
struct pause_row {
    void *label;
    s32 type;
};

#define ROW_TYPE(self) (((struct pause_row *)(self)->field_14)[(self)->field_18].type)

/* Writes " <NN%>" into `buf`: the digits of `value` land at `buf + 2`
 * via sub_80060AC, which returns how many it wrote. */
static inline void format_pct(u8 *buf, s32 value)
{
    s32 len;

    buf[0] = ' ';
    buf[1] = '<';
    len = sub_80060AC(value, &buf[2]);
    buf[len + 2] = '%';
    buf[len + 3] = '>';
    buf[len + 4] = '\0';
}

/* One of a matched pair (sub_8005FBC increments the other way): if the
 * current row is an "editable count" row (type 4 or 5) and its count
 * (`field_60`/`field_64` respectively, 0-20 in steps of 5%) is
 * non-zero, decrements it, formats the " <NN%>" scratch string into
 * `buf57`/`buf4f`, and pushes the new level (`(count << 8 | 1) / 20`)
 * through the matching AudioContext setter (`sub_8001B30`/`sub_8001B50`
 * - see src/audio/audio_context.c). Only the type-5/`field_64` branch
 * also plays the standard SFX cue. */
void sub_8005EF4(struct pause_screen_results *self)
{
    s32 count;

    switch (ROW_TYPE(self)) {
    case 4:
        count = self->field_60;
        if (count != 0) {
            count--;
            self->field_60 = count;
            format_pct(self->buf57, count * 5);
            sub_8001B30(gUnknown_030012BC, ((self->field_60 << 8) + 1) / 20);
        }
        break;
    case 5:
        count = self->field_64;
        if (count != 0) {
            count--;
            self->field_64 = count;
            format_pct(self->buf4f, count * 5);
            sub_8001B50(gUnknown_030012BC, ((self->field_64 << 8) + 1) / 20);
            PlaySfx(gUnknown_030012BC, 0xe, 0x100);
        }
        break;
    }
}

/* Counterpart to sub_8005EF4 above: increments (capped at 0x14)
 * instead of decrementing. */
void sub_8005FBC(struct pause_screen_results *self)
{
    s32 count;

    switch (ROW_TYPE(self)) {
    case 4:
        count = self->field_60;
        if (count <= 0x13) {
            count++;
            self->field_60 = count;
            format_pct(self->buf57, count * 5);
            sub_8001B30(gUnknown_030012BC, ((self->field_60 << 8) + 1) / 20);
        }
        break;
    case 5:
        count = self->field_64;
        if (count <= 0x13) {
            count++;
            self->field_64 = count;
            format_pct(self->buf4f, count * 5);
            sub_8001B50(gUnknown_030012BC, ((self->field_64 << 8) + 1) / 20);
            PlaySfx(gUnknown_030012BC, 0xe, 0x100);
        }
        break;
    }
}
