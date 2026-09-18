#include "core.h"

/* Same "self" object family as actor_part61.c/actor_part66.c - see
 * docs/matching/issue-63-0x08033ef4-actor.md. */

extern void sub_8034480(void *mgr);
extern void sub_80345B0(void *mgr, s32 idx);

/* Calls `sub_8034480(mgr)` (the OAM/tile-scan update this object's part
 * table drives), then - while the particle slot count (`mgr+0xc`) is
 * still under 0x80 - spawns up to 8 more particles via `sub_80345B0`,
 * incrementing `mgr+0xc` for each one spawned. */
void sub_8034688(void *mgrArg)
{
    u8 *mgr = mgrArg;

    sub_8034480(mgr);

    if (*(s32 *)(mgr + 0xc) <= 0x7f) {
        s32 count = 0x80 - *(s32 *)(mgr + 0xc);

        if (count > 8) {
            count = 8;
        }
        count -= 1;

        if (count != -1) {
            s32 end = -1;

            do {
                register s32 idxR0 asm("r0") = *(s32 *)(mgr + 0xc);
                register s32 idx asm("r1") = idxR0;

                *(s32 *)(mgr + 0xc) = idxR0 + 1;
                sub_80345B0(mgr, idx);
                count -= 1;
            } while (count != end);
        }
    }
}

extern void sub_80006A8(void);
extern void *gUnknown_03001304;
extern void sub_80007AC(void *arg);
extern u16 gUnknown_030007E0[];

/* Busy-waits (yielding a frame via `sub_80006A8`/`sub_8034688` each
 * time) until the input-poll result from `sub_80007AC(gUnknown_03001304)`
 * has either of bits 0/3 set in `gUnknown_030007E0`'s `+2` halfword. */
void sub_80346C8(void *mgrArg)
{
    u8 *mgr = mgrArg;
    s32 result;

    goto check;
body:
    sub_80006A8();
    sub_8034688(mgr);
check:
    sub_80007AC(gUnknown_03001304);
    {
        register u8 *addr asm("r1") = (u8 *)gUnknown_030007E0;
        register s32 nine asm("r0") = 9;
        register s32 flag asm("r1");
        register s32 r asm("r0");

        flag = *(u16 *)(addr + 2);
        r = nine & flag;
        result = r;
    }
    if (result == 0) {
        goto body;
    }
}

extern void sub_8026EB4(void *ptr);
extern void sub_8026ED0(void *self);

/* Releases `self+0x10`/`self+8`'s dynamically-allocated buffers (each,
 * if non-NULL, via `sub_8026EB4`) and, if bit 0 of `flags` is set, also
 * releases `self` itself via `sub_8026ED0`. */
void sub_80346FC(void *selfArg, s32 flags)
{
    u8 *self = selfArg;

    if (*(void **)(self + 0x10) != NULL) {
        sub_8026EB4(*(void **)(self + 0x10));
    }
    if (*(void **)(self + 8) != NULL) {
        sub_8026EB4(*(void **)(self + 8));
    }
    if ((flags & 1) != 0) {
        sub_8026ED0(self);
    }
}

asm(".align 2, 0");
