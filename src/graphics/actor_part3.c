#include "core.h"
#include "actor.h"

extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern void *gUnknown_03001308;

/* Same shape as sub_8006FE4 (graphics.c) - `part+0x25 == 1` is a fast
 * "always visible" override; otherwise `part+0xd` bit 2 gates an
 * on-screen check via `sub_803AD80`, using a 4-word "region" of
 * `{gUnknown_03001308's sub-object's two Q8 fields, 240<<8, 160<<8}`
 * (the GBA's screen width/height) and the same
 * `table+N`/`table+N+4` offset/pointer slot pair convention
 * sub_8006FE4 reads at `table+0x40`, here at `table+0x30`. */
s32 sub_8007F78(struct actor *part)
{
    register s32 result asm("r3") = 0;

    if (*((u8 *)part + 0x25) == 1) {
        return 1;
    }

    {
        register u32 flags asm("r1");
        register s32 bit2 asm("r0");

        flags = *((u8 *)part + 0xd);
        bit2 = (flags >> 2) & 1;
        if (!bit2) {
            s32 buf[4];
            register void *subObj asm("r0");
            void *table;

            subObj = *(void **)((u8 *)gUnknown_03001308 + 0x10);
            {
                s32 field0 = *(s32 *)subObj << 8;
                s32 field4 = *(s32 *)((u8 *)subObj + 4) << 8;

                buf[0] = field0;
                buf[1] = field4;
            }
            {
                s32 width = 0xf0 << 8;
                s32 height = 0xa0 << 8;

                buf[2] = width;
                buf[3] = height;
            }

            table = part->table;
            result = (u8)sub_803AD80((u8 *)part + *(s16 *)((u8 *)table + 0x30), buf, *(void **)((u8 *)table + 0x34));
        }
    }
    return result;
}

struct aabb {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_c;
};

extern void sub_8007B00(void *dest, void *part);

/* Same `part+0x25`/`part+0xd` bit-2 fast-path shape as sub_8007F78
 * above, but the real check is an AABB-overlap test: `part`'s own box
 * (via the parked sub_8007B00) against `region`'s `{s32 x, y, w, h}`
 * (kept raw - `region`'s own type isn't established). */
s32 sub_8007FD8(struct actor *part, void *region)
{
    register s32 earlyResult asm("r3") = 0;

    if (*((u8 *)part + 0x25) == 1) {
        return 1;
    }

    {
        register u32 flags asm("r1");
        register s32 bit2 asm("r0");

        flags = *((u8 *)part + 0xd);
        bit2 = (flags >> 2) & 1;
        if (bit2) {
            return earlyResult;
        }
    }

    {
        struct aabb box;
        s32 x1, y1, x2, y2;
        s32 result;

        sub_8007B00(&box, part);

        x1 = box.field_0 << 8;
        y1 = box.field_4 << 8;
        x2 = x1 + (box.field_8 << 8);
        y2 = y1 + (box.field_c << 8);

        result = 0;
        if (x2 > *(s32 *)region) {
            if (x1 < *(s32 *)region + *(s32 *)((u8 *)region + 8)) {
                if (y2 > *(s32 *)((u8 *)region + 4)) {
                    if (y1 < *(s32 *)((u8 *)region + 4) + *(s32 *)((u8 *)region + 0xc)) {
                        result = 1;
                    }
                }
            }
        }
        earlyResult = result;
        return earlyResult;
    }
}
asm(".align 2, 0");
