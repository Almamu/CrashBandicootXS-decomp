#include "core.h"

/* Same singleton system as actor_part28.c - see that file's header
 * comment and docs/matching/issue-62-0x08033804-actor.md. */

extern s32 sub_8033900(void);
extern s32 sub_80338F4(void);
extern s32 sub_80338E8(void);
extern void *sub_80338C4(void);
asm(".set __divsi3, sub_803ADB4");
extern void sub_802E674(s32 x, s32 y, s32 z, s32 dx, s32 dy);
extern void sub_802E504(s32 x, s32 y, s32 z);
extern void *gUnknown_03000884;

/* sub_80339DC: a proximity-triggered effect/hazard detector. Syncs
 * `self`'s position fields to the singleton's current position (plus a
 * fixed offset), and - while the `self+0x64` cooldown slot is zero -
 * measures `self`'s distance to the player; in range, it spawns a pair
 * of effects at `self`'s position and cycles `self+0x68` against a
 * threshold from `sub_80338C4`'s table. Once `self+0x34` passes
 * `0x4B00` it resets `self` to its idle animation state.
 *
 * Matched in a later pass (see docs/matching/issue-62-0x08033804-actor.md,
 * "Later pass: strag2 retry"): both divisions are plain `/` through the ROM's own
 * `__divsi3` (`sub_803ADB4`) - as a libcall they don't clobber memory, so
 * `self+0x24` stays CSE'd in `r6` across them - the divisor is -0x1AA
 * (not -0xAA), and the new cooldown value is stored at one shared
 * `store:` label from all three paths. */
void sub_80339DC(void *selfArg)
{
    u8 *self = selfArg;
    s32 slot;
    s32 next;

    *(s32 *)(self + 0x1c) = sub_8033900() + 0x2000;
    *(s32 *)(self + 0x20) = sub_80338F4() + 0x3000;
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
                u8 *table;
                s32 count;

                sub_802E674(*(s32 *)(self + 0x1c), *(s32 *)(self + 0x20), *(s32 *)(self + 0x24), dx, dy);
                sub_802E504(*(s32 *)(self + 0x1c), *(s32 *)(self + 0x20), *(s32 *)(self + 0x24));

                count = *(s32 *)(self + 0x68) + 1;
                *(s32 *)(self + 0x68) = count;
                table = sub_80338C4();
                if (count == *(s32 *)(table + 0x14)) {
                    *(s32 *)(self + 0x68) = slot;
                    table = sub_80338C4();
                    next = *(s32 *)(table + 0x18);
                } else {
                    table = sub_80338C4();
                    next = *(s32 *)(table + 0x10);
                }
                goto store;
            }
        }
    } else {
        next = slot - 1;
    store:
        *(s32 *)(self + 0x64) = next;
    }

    if (*(s32 *)(self + 0x34) > 0x4B00) {
        *(s32 *)(self + 0x28) = 0;
        *(s32 *)(self + 0x44) = 0;
        *(s32 *)(self + 0xc) = 0;
        {
            u16 anim = *(u16 *)(*(u8 **)self);
            u8 zero;

            /* separate byte zero: the ROM materializes its own movs for it */
            asm("" : "=r"(zero) : "0"(0));
            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero;
        }
        *(s32 *)(self + 8) = 0;
    }
}

asm(".align 2, 0");
