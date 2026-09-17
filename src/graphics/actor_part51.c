#include "core.h"

/* Same "self" object family as actor_part39.c/actor_part41.c - see
 * actor_part39.c's header comment and
 * docs/matching/issue-50-actor-2a69c.md. */

#if NON_MATCHING
/* NOT YET BYTE-MATCHING - see docs/matching/issue-50-actor-2a69c.md.
 * A 12-byte little vector block: copies `self+0x38..0x44` into `*out`,
 * integrating a per-axis velocity (`self+0x1c`/`0x20`/`0x24`, each
 * `>>8`'d to a whole-unit delta) into the block's first three `s16`
 * slots along the way; the remaining three `s16` slots pass through
 * unchanged. Semantics are fully understood and every load/store is
 * confirmed correct (this compiler does emit the same `ldm`/`stm`
 * 3-word block-copy idiom the ROM uses at both ends, confirming the
 * 12-byte-vector-of-three-`s16` shape); the residual gap is purely
 * instruction *scheduling* around the three per-axis `>>8` shifts
 * between the two block copies - this compiler's own list scheduler
 * bunches the three loads/shifts together differently from the ROM's
 * own strict load-shift/load-shift/load-shift order no matter how the
 * source statements are grouped or which registers the per-axis values
 * are pinned to. */
void sub_802AA0C(void *outArg, void *selfArg)
{
    struct blob0xc { u32 w0, w1, w2; };

    u8 *self = selfArg;
    struct blob0xc buf = *(struct blob0xc *)(self + 0x38);
    s16 *sbuf = (s16 *)&buf;

    sbuf[0] += *(s32 *)(self + 0x1c) >> 8;
    sbuf[1] += *(s32 *)(self + 0x20) >> 8;
    sbuf[2] += *(s32 *)(self + 0x24) >> 8;

    *(struct blob0xc *)outArg = buf;
}
#endif /* NON_MATCHING */
