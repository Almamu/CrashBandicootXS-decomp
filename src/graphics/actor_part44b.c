#include "core.h"

/* Same "spawn/pre-attack" singleton family as actor_part39.c - see that
 * file's header comment and docs/matching/issue-56-0x0802f0dc-actor.md. */

#if NON_MATCHING
/* NOT YET BYTE-MATCHING - see docs/matching/issue-56-0x0802f0dc-actor.md,
 * "Parked, not matched: sub_802F748" for the full account; compiled
 * only under `make NON_MATCHING=1`, the checked-in assembly
 * (asm/code_3_2_20_28568_c99c_2f748.s) is used otherwise.
 * A `gStaticData_0817C1C0` stride-8 trampoline-record dispatcher -
 * exactly the same shape as the already-parked `sub_802C208`
 * (src/graphics/actor_part19e.c, issue #52): `{s16 baseOff; s16 count;
 * s16 subOffset}` records, indexed by `self+0x28`'s state; when
 * `count > 0`, indexes a per-instance list pointer at
 * `self+subOffset` and reads its last entry's `{s32 delta; void *fn}`
 * pair, added to `baseOff` for the trampoline address; otherwise falls
 * back to the record's own inline `{..; void *fn}` pair at `+4` with
 * just `baseOff` for the address. Fires
 * `sub_803AD84(self+addr, baseOff, count, fn)`. Every load/store,
 * branch and call confirmed correct; parked on the same register-
 * allocation/instruction-scheduling gap around the two
 * `record = base + state*8` re-derivations documented for
 * `sub_802C208`. */
extern u8 gStaticData_0817C1C0[];
extern s32 sub_803AD84(void *addr, void *arg1, void *tableEntry, void *fn);

void sub_802F748(void *selfArg)
{
    u8 *self = selfArg;
    u8 *base = gStaticData_0817C1C0;
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
}
#endif /* NON_MATCHING */

asm(".align 2, 0");
