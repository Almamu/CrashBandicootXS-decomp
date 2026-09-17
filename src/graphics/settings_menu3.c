#include "core.h"
#include "pause_options_screen.h"

/* Bit-2 flag test repeated throughout this chunk's functions - matches
 * sub_8004A50's own trivial body: caller-visible "1" (set) vs "2"
 * (clear). */
s32 sub_8004A50(struct pause_options_screen *self)
{
    if ((self->flags >> 2) & 1) {
        return 1;
    }
    return 2;
}

extern void sub_8002798(void *arg0);
extern void *gUnknown_03000804;

void sub_8004A64(void)
{
    void *p = gUnknown_03000804;
    sub_8002798(p);
    *((u8 *)p + 5) = 0;
}

extern void sub_8002C84(void *arg0);

void sub_8004A80(struct pause_options_screen *self)
{
    sub_8002798(gUnknown_03000804);
    *((u8 *)gUnknown_03000804 + 5) = 1;
    sub_8002C84(self->field_90);
}

extern void sub_80049CC(struct pause_options_screen *self, s32 labelIndex);
extern void sub_80041BC(struct pause_options_screen *self, void *handle, s32 arg2);
extern void sub_8003D3C(struct pause_options_screen *self, s32 labelIndex);
extern void sub_8003C90(struct pause_options_screen *self, u8 highlight);
extern void sub_8003BDC(struct pause_options_screen *self, s32 label1, s32 label2);

void sub_8004AA4(struct pause_options_screen *self)
{
    sub_80049CC(self, 0x1d);
    sub_80041BC(self, self->field_8c, self->field_24);
    sub_8003D3C(self, 0x26);
}

void sub_8004ACC(struct pause_options_screen *self)
{
    sub_80049CC(self, 0x1d);
    sub_80041BC(self, self->field_8c, self->field_10);
    sub_8003C90(self, self->field_10 == 4);
}

void sub_8004AFC(struct pause_options_screen *self)
{
    sub_80049CC(self, 0x1e);
    sub_80041BC(self, self->field_8c, self->field_24);
    sub_8003D3C(self, 0x27);
}

void sub_8004B24(struct pause_options_screen *self)
{
    sub_80049CC(self, 0x1e);
    sub_80041BC(self, self->field_8c, self->field_10);
    sub_8003C90(self, self->field_10 == 4);
}

void sub_8004B54(struct pause_options_screen *self)
{
    sub_80049CC(self, 0x1c);
    sub_8003BDC(self, self->field_14, self->field_18);
}

void sub_8004B70(struct pause_options_screen *self)
{
    sub_80049CC(self, 0x1c);
    sub_80041BC(self, self->field_90, self->field_10);
    sub_8003C90(self, self->field_10 == 4);
}

void sub_8004BA0(struct pause_options_screen *self)
{
    sub_80049CC(self, 0x1b);
    sub_80041BC(self, self->field_8c, self->field_10);
    sub_8003C90(self, self->field_10 == 4);
}

extern void sub_8006A90(void *arg0);
extern void sub_8006A48(void *arg0);
extern void sub_8006C28(void *arg0);
extern void sub_8003A60(struct pause_options_screen *self);
extern void *gUnknown_03001300;
extern void *gUnknown_030012FC;

void sub_8004BD0(struct pause_options_screen *self)
{
    sub_8006A90(gUnknown_03001300);
    sub_8006C28(gUnknown_030012FC);
    if ((u32)self->state <= 0xa) {
        switch (self->state) {
        case 0:
            sub_8003A60(self);
            break;
        case 1:
            sub_8004BA0(self);
            break;
        case 2:
            sub_8004B70(self);
            break;
        case 3:
        case 4:
            sub_8004B54(self);
            break;
        case 5:
            sub_8004B24(self);
            break;
        case 6:
            sub_8004ACC(self);
            break;
        case 9:
            sub_8004AFC(self);
            break;
        case 7:
            sub_8004AA4(self);
            break;
        case 8:
            break;
        case 10:
            break;
        default:
            break;
        }
    }
    sub_8006A48(gUnknown_03001300);
}

extern void sub_8002C14(void *handle, s32 rowIndex, void *buf);
extern void sub_8002C6C(void *arg0, s32 arg1);
extern s32 sub_8002BA4(void *arg0);
extern void sub_8002C40(void *arg0, s32 arg1, void *buf);

void sub_8004C7C(struct pause_options_screen *self, s32 arg1)
{
    u8 buf[0x70];

    sub_8002C14(self->field_8c, arg1, buf);
    sub_8002C6C(self->field_8c, arg1);
    if (sub_8002BA4(self->field_8c)) {
        sub_8002C40(self->field_8c, arg1, buf);
    }
}
asm(".align 2, 0");
