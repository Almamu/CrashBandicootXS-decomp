#include "core.h"

/* Same "self" object family as actor_part61.c - see that file's header
 * comment and docs/matching/issue-63-0x08033ef4-actor.md. */

#if NON_MATCHING
/* NOT YET BYTE-MATCHING - see docs/matching/issue-63-0x08033ef4-actor.md,
 * "Parked: sub_8034270" for the full account; compiled only under
 * `make NON_MATCHING=1`, the checked-in assembly
 * (asm/code_3_2_20_28568_c99c_31784_33ef4_34270.s) is used otherwise.
 * Syncs `self`'s position fields from the singleton's own position plus
 * a fixed offset, sets the one-shot flag (`+0x58=1`), and - if
 * `self+0x12` is set and `self` is non-NULL - fires the `self+0x50`
 * event table's slot-3 trampoline; otherwise calls `sub_802A7B8(self)`.
 * Semantics fully understood and every field/call confirmed correct;
 * parked because the ROM computes a "should animate" 0/1 value into a
 * register and then re-checks it against zero before deciding whether
 * to call `sub_802A7B8`, even though the value is a compile-time
 * constant on each path - this compiler's dead-store/dead-branch
 * elimination always collapses that redundant compute-then-recheck
 * step, the same class of gap already documented for `sub_802C2FC`
 * (issue #52) and the `| 0`-with-a-zero-valued-term case in
 * `sub_803B46C` (issue #71). */
extern s32 sub_80338E8(void);
extern s32 sub_8033900(void);
extern s32 sub_80338F4(void);
extern s32 sub_803AD80(void *arg0, void *arg1, void *fn);
extern void sub_802A7B8(void *self);

void sub_8034270(void *selfArg)
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

    if (doAnim != 0) {
        sub_802A7B8(self);
    }
}
#endif /* NON_MATCHING */

asm(".align 2, 0");
