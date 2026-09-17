#include "core.h"

/* Same palette-cycle cluster as actor_part41.c/actor_part43.c - see
 * docs/matching/issue-50-actor-2a69c.md. */

#if NON_MATCHING
extern u8 gUnknown_03001464;
extern s32 gUnknown_03001470;
extern s32 gUnknown_03001474;
extern s32 gUnknown_03001478;
extern u8 gStaticData_08175760[];
extern s32 QueueVramDmaTransfer(void *arg0, void *arg1, u16 arg2, u16 arg3);

/* NOT YET BYTE-MATCHING - see docs/matching/issue-50-actor-2a69c.md.
 * Per-frame palette-cycle DMA: while `gUnknown_03001464` is set, DMAs one
 * `0x1c0`-byte palette-animation "frame" (`gStaticData_08175760 +
 * cursor*0x1c0`) to BG palette RAM. Every `0x24` calls (the
 * `gUnknown_03001478` counter), advances the cursor (`gUnknown_03001470`)
 * toward `gUnknown_03001474`: increments while still below the bound,
 * decrements once past it, and holds steady exactly at the bound -
 * `sub_802AB34`/`sub_802ABC8` flip which end is "the bound" to make this
 * ping-pong. Semantics are fully understood and every load/store, branch
 * and call is confirmed correct (the DMA setup and refresh-counter reset
 * at the top match the ROM instruction-for-instruction); the residual
 * gap is in the cursor-advance tail: no C phrasing tried (plain
 * if/else-if/else, `goto`-linearized with an explicit `result` copy,
 * cached-address locals, register-pinned address locals) stops this
 * compiler from speculatively computing the decrement (`idx - 1`) ahead
 * of the branch that decides whether it's needed, which shortens the
 * branch-taken path by folding away a redundant unconditional jump the
 * ROM's own build still has - a shorter, still-correct result, but not
 * byte-identical. */
void sub_802AB58(void)
{
    s32 *cursor;

    if (gUnknown_03001464 == 0) {
        return;
    }

    cursor = &gUnknown_03001470;
    QueueVramDmaTransfer(gStaticData_08175760 + *cursor * 0x1c0, (void *)PLTT, 0x1c0, 0x10);

    gUnknown_03001478 += 1;
    if (gUnknown_03001478 <= 0x23) {
        return;
    }
    gUnknown_03001478 = 0;

    {
        s32 bound = gUnknown_03001474;
        s32 idx = *cursor;
        s32 diff = bound - idx;
        s32 result;

        if (diff < 0) {
            result = idx - 1;
        } else if (bound == idx) {
            result = idx;
        } else {
            result = idx + 1;
        }
        *cursor = result;
    }
}
#endif /* NON_MATCHING */
