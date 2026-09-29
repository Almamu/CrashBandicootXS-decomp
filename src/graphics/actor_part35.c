#include "core.h"

/* Same "self" object family as actor_part28.c - see that file's header
 * comment and docs/matching/issue-62-0x08033804-actor.md. */

extern s32 sub_8033900(void);
extern s32 sub_80338F4(void);
extern s32 sub_80338E8(void);
extern void *sub_80338C4(void);
extern s32 sub_80338D0(void);
asm(".set __divsi3, sub_803ADB4");
extern s32 sub_8000E1C(s32 arg0);
extern void sub_802E170(s32 kind, s32 x, s32 y, s32 z, s32 arg4);
extern void *gUnknown_03000884;

/* sub_8033CF8: `sub_80339DC`'s sibling. Sets `self`'s position fields
 * from the singleton's own position plus a different fixed offset,
 * and - while `self+0x64` (a cooldown slot) is zero - measures `self`'s
 * distance to the player the same way; in range, it picks one of three
 * spawn "kinds" (5/6/8, via `sub_8000E1C(3)`) and calls `sub_802E170`
 * at `self`'s position, then cycles `self+0x68` against a threshold
 * from `sub_80338C4`'s table. Once `self+0x34` passes `0x4B00` and the
 * singleton's own "kind" (`sub_80338D0`) is 3, resets `self` back to
 * its idle animation state.
 *
 * Matched in a later pass with the same shape as `sub_80339DC` (see
 * docs/matching/issue-62-0x08033804-actor.md, "Later pass: strag2 retry"): no
 * register pins at all - the old `r7` blocker came from a wrong
 * source shape, not from a register the allocator couldn't reach. */
void sub_8033CF8(void *selfArg)
{
    u8 *self = selfArg;
    s32 slot;
    s32 next;

    *(s32 *)(self + 0x1c) = sub_8033900() + 0x1E00;
    *(s32 *)(self + 0x20) = sub_80338F4() - 0x3000;
    *(s32 *)(self + 0x24) = sub_80338E8() - 0x100;

    slot = *(s32 *)(self + 0x64);
    if (slot == 0) {
        u8 *player = gUnknown_03000884;
        s32 angle = (*(s32 *)(player + 0x24) - *(s32 *)(self + 0x24)) / -0x1AA;

        if (angle > 0) {
            s32 scale = 0x1000 / angle;
            s32 rawDx = (*(s32 *)(player + 0x1c) - *(s32 *)(self + 0x1c)) * scale;
            s32 dx = rawDx >> 12;
            s32 rawDy = (*(s32 *)(player + 0x20) - *(s32 *)(self + 0x20)) * scale;
            s32 dy = rawDy >> 12;
            s32 signDx = rawDx >> 31;
            s32 absDx = (dx ^ signDx) - signDx;
            s32 signDy = rawDy >> 31;
            s32 absDy = (dy ^ signDy) - signDy;

            if (absDx + absDy <= 0xFFF) {
                s32 kind = (u16)sub_8000E1C(3);
                u8 *table;
                s32 count;

                if (kind == 0) {
                    sub_802E170(5, *(s32 *)(self + 0x1c), *(s32 *)(self + 0x20), *(s32 *)(self + 0x24), slot);
                } else if (kind == 1) {
                    sub_802E170(6, *(s32 *)(self + 0x1c), *(s32 *)(self + 0x20), *(s32 *)(self + 0x24), slot);
                } else {
                    sub_802E170(8, *(s32 *)(self + 0x1c), *(s32 *)(self + 0x20), *(s32 *)(self + 0x24), slot);
                }

                count = *(s32 *)(self + 0x68) + 1;
                *(s32 *)(self + 0x68) = count;
                table = sub_80338C4();
                if (count == *(s32 *)(table + 0x20)) {
                    *(s32 *)(self + 0x68) = 0;
                    table = sub_80338C4();
                    next = *(s32 *)(table + 0x24);
                } else {
                    table = sub_80338C4();
                    next = *(s32 *)(table + 0x1c);
                }
                goto store;
            }
        }
    } else {
        next = slot - 1;
    store:
        *(s32 *)(self + 0x64) = next;
    }

    if (*(s32 *)(self + 0x34) > 0x4B00 && sub_80338D0() == 3) {
        s32 zero32;
        s32 state;

        /* The ROM materializes the 0 and then the 2 before the stores
         * (`movs r2, #0; movs r0, #2`); the "=r"/"0" escapes keep both
         * as registers, and the volatile one stops the 2 from being
         * sunk to its store. */
        asm("" : "=r"(zero32) : "0"(0));
        asm volatile("" : "=r"(state) : "0"(2));
        *(s32 *)(self + 0x28) = zero32;
        *(s32 *)(self + 0x44) = zero32;
        *(s32 *)(self + 0xc) = state;
        {
            u16 anim = *(u16 *)(*(u8 **)self + 0x18);
            u8 zero;

            /* separate byte zero: the ROM materializes its own movs for it */
            asm("" : "=r"(zero) : "0"(0));
            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero;
        }
        *(s32 *)(self + 8) = zero32;
    }
}

asm(".align 2, 0");
