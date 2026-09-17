#include "core.h"

/* Same singleton system as actor_part28.c - see that file's header
 * comment and docs/matching/issue-62-0x08033804-actor.md. */

#if NON_MATCHING
/* NOT YET BYTE-MATCHING - see docs/matching/issue-62-0x08033804-actor.md,
 * "Parked, not matched: sub_80339DC" for the full account; compiled
 * only under `make NON_MATCHING=1`, the checked-in assembly
 * (asm/code_3_2_20_28568_c99c_31784_339dc.s) is used otherwise.
 * Semantics are fully understood and every load/store, branch and call
 * is confirmed correct (a proximity-triggered effect/hazard detector
 * measuring `self`'s distance to the player after syncing `self`'s own
 * position fields to the singleton's current position); the residual
 * gap is a handful of register-allocation choices this compiler makes
 * differently from the ROM (the ROM keeps `self+0x64`'s cached "slot"
 * value in the `sb`/`r9` high register for the whole function, needing
 * a second high-register relay pair at entry/exit that this compiler
 * doesn't reproduce when only `r8` is otherwise in use, and picks a
 * different scratch register for one 16-bit immediate load early in
 * the function). */
extern s32 sub_8033900(void);
extern s32 sub_80338F4(void);
extern s32 sub_80338E8(void);
extern void *sub_80338C4(void);
extern s32 sub_803ADB4(s32 arg0, s32 arg1);
extern void sub_802E674(s32 x, s32 y, s32 z, s32 dx, s32 dy);
extern void sub_802E504(s32 x, s32 y, s32 z);
extern void *gUnknown_03000884;

void sub_80339DC(void *selfArg)
{
    u8 *self = selfArg;
    s32 slot;

    *(s32 *)(self + 0x1c) = sub_8033900() + 0x2000;
    *(s32 *)(self + 0x20) = sub_80338F4() + 0x3000;
    *(s32 *)(self + 0x24) = sub_80338E8() - 0x100;

    slot = *(s32 *)(self + 0x64);
    if (slot == 0) {
        u8 *player = gUnknown_03000884;
        s32 angle = sub_803ADB4(*(s32 *)(player + 0x24) - *(s32 *)(self + 0x24), -0xAA);

        if (angle > 0) {
            s32 scale = sub_803ADB4(0x1000, angle);
            s32 rawDx = (*(s32 *)(player + 0x1c) - *(s32 *)(self + 0x1c)) * scale;
            s32 rawDy = (*(s32 *)(player + 0x20) - *(s32 *)(self + 0x20)) * scale;
            s32 dx = rawDx >> 12;
            s32 dy = rawDy >> 12;
            s32 signDx = rawDx >> 31;
            s32 signDy = rawDy >> 31;
            s32 absDx = (dx ^ signDx) - signDx;
            s32 absDy = (dy ^ signDy) - signDy;

            if (absDx + absDy <= 0xFFF) {
                u8 *table;

                sub_802E674(*(s32 *)(self + 0x1c), *(s32 *)(self + 0x20), *(s32 *)(self + 0x24), dx, dy);
                sub_802E504(*(s32 *)(self + 0x1c), *(s32 *)(self + 0x20), *(s32 *)(self + 0x24));

                *(s32 *)(self + 0x68) += 1;
                table = sub_80338C4();
                if (*(s32 *)(self + 0x68) == *(s32 *)(table + 0x14)) {
                    *(s32 *)(self + 0x68) = slot;
                    table = sub_80338C4();
                    slot = *(s32 *)(table + 0x18);
                } else {
                    table = sub_80338C4();
                    slot = *(s32 *)(table + 0x10);
                }
                *(s32 *)(self + 0x64) = slot;
            }
        }
    } else {
        *(s32 *)(self + 0x64) = slot - 1;
    }

    if (*(s32 *)(self + 0x34) > 0x4B00) {
        *(s32 *)(self + 0x28) = 0;
        *(s32 *)(self + 0x44) = 0;
        *(s32 *)(self + 0xc) = 0;
        *(u16 *)(self + 0x10) = *(u16 *)(*(u8 **)self);
        self[0x12] = 0;
        *(s32 *)(self + 8) = 0;
    }
}
#endif /* NON_MATCHING */

asm(".align 2, 0");
