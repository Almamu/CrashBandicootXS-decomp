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
