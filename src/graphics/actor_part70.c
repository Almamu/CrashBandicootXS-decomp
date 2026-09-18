#include "core.h"

/* Same "self" object family as actor_part63.c - see that file's header
 * comment and docs/matching/issue-63-0x08033ef4-actor.md. */

#if NON_MATCHING
/* NOT YET BYTE-MATCHING - see docs/matching/issue-63-0x08033ef4-actor.md,
 * "Parked: sub_8034314" for the full account; compiled only under
 * `make NON_MATCHING=1`, the checked-in assembly
 * (asm/code_3_2_20_28568_c99c_31784_33ef4_34314.s) is used otherwise.
 * Same position-sync/flag/trampoline shape as the parked `sub_8034270`
 * (actor_part62.c), but returns the "should animate" boolean directly
 * instead of calling `sub_802A7B8` itself. Parked on the identical
 * dead-store/dead-branch elimination gap. */
extern s32 sub_80338E8(void);
extern s32 sub_8033900(void);
extern s32 sub_80338F4(void);
extern s32 sub_803AD80(void *arg0, void *arg1, void *fn);

s32 sub_8034314(void *selfArg)
{
    u8 *self = selfArg;
    s32 doAnim;

    *(s32 *)(self + 0x24) = sub_80338E8() - 0x200;
    *(s32 *)(self + 0x1c) = sub_8033900() + 0x2000;
    *(s32 *)(self + 0x20) = sub_80338F4() + 0x3000;
    self[0x58] = 1;

    if (self[0x12] != 0) {
        if (self != NULL) {
            u8 *table = *(u8 **)(self + 0x50);
            u8 *addr = self + *(s16 *)(table + 8);
            void *fn = *(void **)(table + 0xc);

            sub_803AD80(addr, (void *)3, fn);
        }
        doAnim = 0;
    } else {
        doAnim = 1;
    }

    return doAnim;
}
#endif /* NON_MATCHING */

asm(".align 2, 0");
