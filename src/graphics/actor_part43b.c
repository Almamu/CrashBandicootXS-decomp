#include "core.h"

/* Same "spawn/pre-attack" singleton family as actor_part39.c - see that
 * file's header comment and docs/matching/issue-56-0x0802f0dc-actor.md. */

#if NON_MATCHING
/* NOT YET BYTE-MATCHING - see docs/matching/issue-56-0x0802f0dc-actor.md,
 * "Parked, not matched: sub_802F338" for the full account; compiled
 * only under `make NON_MATCHING=1`, the checked-in assembly
 * (asm/code_3_2_20_28568_c99c_2f338.s) is used otherwise.
 * Computes two `self`-keyframe-driven sizes (byte0*byte1, scaled by
 * 32) via `sub_8028CD4`, storing them into the `gUnknown_03001518`
 * pair, then arms `gUnknown_03001510`/clears `gUnknown_03001514`.
 * Semantics fully understood and every load/store, branch and call is
 * confirmed correct - both keyframe-size sub-blocks are literally
 * identical computations, matching the ROM's own duplication; parked
 * on a residual "materialize the multiply result, then copy it again
 * before shifting" gap (the ROM computes `byte0*byte1` into one
 * register, copies it to a second, *then* shifts - `adds r2,r3,#0;
 * muls r2,r1,r2; adds r0,r2,#0; lsls r0,r0,#5`) that this compiler's
 * dead-store elimination always collapses into a shorter
 * compute-and-shift-in-place sequence, with no C-level formulation
 * (separate locals, a `register`-pinned intermediate, an `asm`
 * barrier) found that reproduces the extra copy without also
 * eliminating it differently. */
extern void *sub_8028CD4(s32 size);
extern void *gUnknown_03001518[2];
extern s32 gUnknown_03001510;
extern s32 gUnknown_03001514;

void sub_802F338(void *selfArg)
{
    u8 *self = selfArg;

    {
        s32 accum = *(s32 *)(self + 8) >> 8;
        s32 idx = *(s32 *)(self + 0xc);
        u8 *table = *(u8 **)self;
        s16 off = *(s16 *)(table + idx * 3 * 4 + 2);
        s32 pos = off + accum;
        u8 **table2 = *(u8 ***)(self + 4);
        u8 *rec = table2[pos];

        gUnknown_03001518[0] = sub_8028CD4((rec[1] * rec[0]) << 5);
    }
    {
        s32 accum = *(s32 *)(self + 8) >> 8;
        s32 idx = *(s32 *)(self + 0xc);
        u8 *table = *(u8 **)self;
        s16 off = *(s16 *)(table + idx * 3 * 4 + 2);
        s32 pos = off + accum;
        u8 **table2 = *(u8 ***)(self + 4);
        u8 *rec = table2[pos];

        gUnknown_03001518[1] = sub_8028CD4((rec[1] * rec[0]) << 5);
    }

    gUnknown_03001510 = 1;
    gUnknown_03001514 = 0;
}
#endif /* NON_MATCHING */

asm(".align 2, 0");
