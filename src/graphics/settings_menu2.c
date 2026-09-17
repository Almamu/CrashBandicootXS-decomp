#include "core.h"
#include "pause_options_screen.h"

/* A small "load my background" sub-widget - the same field_c/field_d
 * bit-flags-pair idiom as `struct counter_widget`
 * (src/audio/counter_selector_setup.c's sub_80374D0), just at offsets
 * 0x1c/0x1d here - this chunk doesn't include whatever embeds it in a
 * bigger object, so it gets its own minimal type. */
struct bg_widget {
    u32 field_0;
    u8 unused_04[0x1c - 4];
    u8 field_1c;
    u8 field_1d;
};

extern void *sub_801E644(void *buf, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void LoadGraphicsPackage(void *buf, void *asset);
extern s32 sub_801E640(void *buf);
extern u8 gStaticData_0816C484[];

/* Same shape as sub_80374D0 (src/audio/counter_selector_setup.c) - reset
 * two bit-flag bytes, request a BG tile/map graphics package, set BG0's
 * control register from it - plus zeroing `field_0`, which sub_80374D0's
 * counter_widget doesn't have. */
void sub_80047F8(struct bg_widget *self)
{
    u8 buf[0x10];
    u32 zero = 0;
    s32 a;
    register s32 b asm("r1");

    *(u16 *)&self->field_1c = zero;
    a = 0x40;
    a |= self->field_1c;
    a &= -8;
    a |= 1;
    self->field_1c = a;
    b = 1;
    b |= self->field_1d;
    b &= -3;
    b |= 0x10;
    self->field_1d = b;

    sub_801E644(buf, 2, 0x1e, 1, 3);
    LoadGraphicsPackage(buf, gStaticData_0816C484);
    self->field_0 = 0;
    REG_BG0CNT = sub_801E640(buf);
    *(vu32 *)REG_ADDR_BG0HOFS = zero;
}

extern s32 sub_8006920(void *arg0);
extern s32 sub_80068A8(void *arg0);
extern s32 sub_80067E4(void *arg0);
extern s32 sub_800695C(void *arg0);
extern s32 sub_800697C(void *arg0);
extern u8 sub_8002CE8(void *handle, s32 rowIndex);
extern void sub_8002C14(void *handle, s32 rowIndex, void *buf);

/* Refreshes each of the 4 settings rows' aggregate stats from `handle`,
 * skipping any row sub_8002CE8 reports as inactive/hidden. */
void sub_8004860(struct pause_options_screen *self, void *handle)
{
    struct settings_row_stats *row;
    u8 buf[0x70];
    s32 i;

    i = 0;
    row = &self->rowStats[0];
    do {
        if (!sub_8002CE8(handle, i)) {
            sub_8002C14(handle, i, buf);
            row->field_4 = sub_8006920(buf);
            row->field_8 = sub_80068A8(buf);
            row->field_c = sub_80067E4(buf);
            row->field_10 = sub_800695C(buf);
            row->field_0 = sub_800697C(buf);
        }
        row++;
        i++;
    } while (i <= 3);
}

extern s32 sub_8002A08(void *arg0);
extern s32 sub_8002BA4(void *arg0);
extern void sub_8002C84(void *arg0);

void sub_80048BC(struct pause_options_screen *self)
{
    s32 v = sub_8002A08(self->field_8c);
    if ((u32)(v - 1) <= 3) {
        sub_8002C84(self->field_8c);
        sub_8002BA4(self->field_8c);
    }
}

/* Fills `dest` from `src` using the same five-function battery as the
 * loop in sub_8004860 above - `self` (the screen widget) is passed but
 * never used, matching the ROM exactly. */
void sub_80048E0(void *self, struct settings_row_stats *dest, void *src)
{
    dest->field_4 = sub_8006920(src);
    dest->field_8 = sub_80068A8(src);
    dest->field_c = sub_80067E4(src);
    dest->field_10 = sub_800695C(src);
    dest->field_0 = sub_800697C(src);
}
