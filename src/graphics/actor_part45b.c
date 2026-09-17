#include "core.h"

/* Same "spawn/pre-attack" singleton family as actor_part39.c - see that
 * file's header comment and docs/matching/issue-56-0x0802f0dc-actor.md. */

#if NON_MATCHING
/* NOT YET BYTE-MATCHING - see docs/matching/issue-56-0x0802f0dc-actor.md,
 * "Parked, not matched: sub_802F97C" for the full account; compiled
 * only under `make NON_MATCHING=1`, the checked-in assembly
 * (asm/code_3_2_20_28568_c99c_2f97c.s) is used otherwise.
 * A physics-step-and-collision-react updater: advances `self`'s
 * position by its `self+0x58`/`self+0x5c` velocity pair (with a fixed
 * gravity-like offset on Y) and a fixed Z step, then reacts to a
 * `sub_802A3AC` collision probe - firing a trampoline on the hit
 * object if any, else checking `sub_8031378` (an AABB overlap test)
 * and a `self+0x34` depth threshold before firing `self`'s own
 * `self+0x50`-table trampoline (index 8) or falling back to
 * `sub_802A7B8`. Every load/store, branch and call confirmed correct,
 * including the ROM's exact duplicate-but-differently-scheduled
 * `self+0x50`-table lookup in both the `sub_8031378`-true arm and the
 * threshold-exceeded fallthrough arm; parked on a residual register
 * choice (`r2` vs `r3`) for the `8` immediate in those two lookups
 * this compiler allocates the opposite way round from the ROM. */
extern void *sub_802A3AC(void *selfArg);
extern u8 sub_8031378(void *selfArg);
extern void sub_803146C(s32 delta);
extern s32 sub_803AD80(void *pos, s32 arg1, void *table);
extern void sub_802A7B8(void *selfArg);

void sub_802F97C(void *selfArg)
{
    u8 *self = selfArg;
    u8 *table;
    s32 off;

    *(s32 *)(self + 0x1c) += *(s32 *)(self + 0x58);
    *(s32 *)(self + 0x20) += -0xc0 + *(s32 *)(self + 0x5c);
    *(s32 *)(self + 0x24) += 0x400;

    {
        u8 *hit = sub_802A3AC(self);

        if (hit != 0) {
            u8 *hitTable = *(u8 **)(hit + 0x50);

            sub_803AD80(hit + *(s16 *)(hitTable + 0x20), 2, *(void **)(hitTable + 0x24));
        } else if (sub_8031378(self)) {
            sub_803146C(2);
            if (self == 0) {
                return;
            }
            table = *(u8 **)(self + 0x50);
            off = *(s16 *)(table + 8);
            goto tail;
        } else if (*(s32 *)(self + 0x34) <= 0x8200) {
            if (self != 0) {
                sub_802A7B8(self);
            }
            return;
        }

        if (self == 0) {
            return;
        }
        table = *(u8 **)(self + 0x50);
        off = *(s16 *)(table + 8);
tail:
        sub_803AD80(self + off, 3, *(void **)(table + 0xc));
    }
}
#endif /* NON_MATCHING */

asm(".align 2, 0");
