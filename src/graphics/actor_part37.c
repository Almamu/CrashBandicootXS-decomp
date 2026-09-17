#include "core.h"

/* Same "self" object family as actor_part28.c - see that file's header
 * comment and docs/matching/issue-62-0x08033804-actor.md. */

#if NON_MATCHING
/* NOT YET BYTE-MATCHING - see docs/matching/issue-62-0x08033804-actor.md,
 * "Parked, not matched: sub_8033E80" for the full account; compiled
 * only under `make NON_MATCHING=1`, the checked-in assembly
 * (asm/code_3_2_20_28568_c99c_31784_33e80.s) is used otherwise.
 * `sub_8033B44`'s twin, using the second stride-8 table
 * (`gStaticData_0817C4F8`) instead of `gStaticData_0817C4E0`; otherwise
 * identical, including the conditional `sub_802A7B8(self)` tail.
 * Parked on the same `record = base + state*8` re-derivation
 * register-allocation gap as `sub_8033B44`/`sub_8033C84` - see
 * `actor_part31.c`. */
extern u8 gStaticData_0817C4F8[];
extern s32 sub_803AD84(void *addr, void *arg1, void *tableEntry, void *fn);
extern void sub_802A7B8(void *self);

void sub_8033E80(void *selfArg)
{
    u8 *self = selfArg;
    u8 *base = gStaticData_0817C4F8;
    s32 state = *(s32 *)(self + 0x28);
    u8 *record = base + state * 8;
    s16 count = *(s16 *)(record + 2);
    void *fn;
    s16 baseOff;
    s32 addr;

    if (count > 0) {
        s16 subOffset = *(s16 *)(record + 4);
        u8 *listPtr = *(u8 **)(self + subOffset);
        u8 *entry = listPtr + count * 8 - 8;
        s32 delta = *(s32 *)(entry + 0);

        fn = *(void **)(entry + 4);
        record = base + *(s32 *)(self + 0x28) * 8;
        baseOff = *(s16 *)(record + 0);
        addr = (s16)delta + baseOff;
    } else {
        fn = *(void **)(record + 4);
        record = base + *(s32 *)(self + 0x28) * 8;
        baseOff = *(s16 *)(record + 0);
        addr = baseOff;
    }

    sub_803AD84(self + addr, (void *)(s32)baseOff, (void *)(s32)count, fn);

    if (!(*(s32 *)(self + 0x28) == 2 && self[0x12] != 0)) {
        sub_802A7B8(self);
    }
}
#endif /* NON_MATCHING */

asm(".align 2, 0");
