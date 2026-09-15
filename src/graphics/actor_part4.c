#include "core.h"
#include "actor.h"

struct aabb {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_c;
};

extern void sub_803AFE4(void *buf, s32 arg1, s32 arg2);
extern void sub_803AFDC(void *buf, s32 arg1, s32 arg2);
extern u8 sub_8001688(void *buf1, void *buf2);

/* Builds `part`'s AABB (same keyframe-table shape/record layout as
 * sub_8007B98, inlined directly here rather than calling it - this
 * function needs the box on the stack for the final overlap test, not
 * written out through a `dest` pointer) and mirrors it per
 * `part+0x28` bits 4/5, then tests it for overlap against `region` via
 * `sub_8001688` - the same collision-test function `sub_8007DBC`
 * uses. */
s32 sub_80080C0(struct actor *part, void *region)
{
    struct aabb buf_;
    void **tablePtr;
    register void *rec asm("r0");
    register u8 idx asm("r3");
    void *rec4;
    s32 offset;
    s32 offX, offY;
    s32 w, h;
    s32 x, y;
    s32 xpos, ypos;
    u8 flags;
    s32 mirrorX, mirrorY;

    flags = *((u8 *)part + 0x28);
    mirrorX = ((u32)flags << 27) >> 31;
    mirrorY = ((u32)flags << 26) >> 31;

    xpos = *(s32 *)part >> 8;
    ypos = *(s32 *)((u8 *)part + 4) >> 8;

    tablePtr = *(void ***)((u8 *)part + 0x20);
    part = (struct actor *)((u8 *)part + 0x2d);
    idx = *(u8 *)part;
    offset = idx * 0x1c;
    rec = *tablePtr;
    rec = (u8 *)rec + offset;
    rec4 = (u8 *)rec + 4;

    offX = *(s16 *)((u8 *)rec + 4);
    offY = *(s16 *)((u8 *)rec4 + 2);
    w = *((u8 *)rec4 + 4);
    h = *((u8 *)rec4 + 5);

    x = offX + xpos;
    y = offY + ypos;
    sub_803AFE4(&buf_, x, y);
    sub_803AFDC(&buf_, w, h);

    if (mirrorX) {
        buf_.field_0 = xpos * 2 - (buf_.field_0 + buf_.field_8);
    }
    if (mirrorY) {
        buf_.field_4 = ypos * 2 - (buf_.field_4 + buf_.field_c);
    }

    return (u8)sub_8001688(&buf_, region);
}
asm(".align 2, 0");
