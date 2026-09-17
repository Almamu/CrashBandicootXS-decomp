#include "core.h"

/* GitHub issue #9: 0x08007634-0x0800B3F0, game_loop-labeled chunk that
 * turned out to be part of the `actor` category's "part" object family
 * (see docs/matching/issue-9-0x08007634-actor.md). `sub_800B270` sits
 * right after the still-raw `sub_800AFF4`, at the end of that raw span. */

#if NON_MATCHING
/* Per-frame velocity integrator: moves `self+0x60`/`self+0x64` (current
 * X/Y velocity) toward `self+0x50`/`self+0x5c` (target X/Y velocity) by
 * up to `self+0x4c`/`self+0x58` (X/Y acceleration step) each call,
 * clamping at the target rather than overshooting past it. Derives a
 * `self+0x24` direction-flag byte from the resulting velocity's sign
 * (bit 0 = vx>0, bit 1 = vx<0, bit 3 = vy>0, bit 2 = vy<0), snapshots
 * the pre-move position into `self+0x6c`/`self+0x70`, then applies the
 * velocity to `self+0x0`/`self+0x4` (position). Finally writes the
 * resulting Y velocity into an as-yet-unlabeled global at `0x0300129C`
 * (no symbol name recovered for it anywhere else in this ROM - kept as
 * a raw address rather than guessing one) and returns whether either
 * axis is still moving (`vx != 0 || vy != 0`).
 *
 * The `0x0300129C` write has a genuinely redundant-looking branch: when
 * the global was already non-zero and the new Y velocity is exactly 0,
 * the ROM conditionally stores 0 to it before *unconditionally*
 * overwriting it with the same Y velocity value right after - so the
 * branch has no observable effect (both paths store the same value),
 * but it's reproduced as-is (not simplified to a single unconditional
 * store) since that's what the ROM actually does.
 *
 * PARKED, NOT BYTE-MATCHING: every field offset, branch, and computed
 * value is confirmed correct against the ROM - explicit register pins
 * (`self`/`v`/`target`/each clamp `result` to r2/r1/r3/r0, the flags
 * pointer to r1, the OR-mask to r0, forced fresh reloads via `vs32`
 * casts matching the ROM's own redundant re-reads of `self->x`/
 * `self->y`) reproduce every instruction up to and including the
 * position-update store, matching the ROM one-for-one through that
 * point. The remaining gap is confined to the final 14-instruction
 * `0x0300129C` block: the ROM loads the global's *address* into r0 and
 * its *value* into r2, while this compiler's natural allocation (and
 * every variant tried - a separate `s32 gval` local, `gval`/`g` each
 * pinned to r0 or r2 in both combinations, a `volatile`-qualified
 * register variable) either keeps the opposite assignment (address in
 * r2, value in r0 - functionally identical, same instruction count,
 * same byte total, but the wrong register letters throughout that
 * block) or, when `g` is force-pinned to r0, triggers an unrelated
 * regression elsewhere in the function (an extra `push {r4}`/`pop
 * {r4}` pair appears, as if r0's forced reservation propagates back
 * through the whole function). Parked on this one address/value
 * register-letter swap rather than chase it further - see
 * docs/matching/issue-9-0x08007634-actor.md. */
s32 sub_800B270(void *selfArg)
{
    register s32 *w asm("r2") = (s32 *)selfArg;
    register u8 *flags asm("r1");
    s32 fx, fy;

    {
        register s32 v asm("r1") = w[0x60 / 4];
        register s32 target asm("r3") = w[0x50 / 4];

        if (v >= target) goto case1_ge;
        {
            s32 step = w[0x4c / 4];
            register s32 result asm("r0") = v + step;
            w[0x60 / 4] = result;
            if (result <= target) goto case1_done;
            goto case1_clamp;
        }
    case1_ge:
        if (v <= target) goto case1_done;
        {
            s32 step = w[0x4c / 4];
            register s32 result asm("r0") = v - step;
            w[0x60 / 4] = result;
            if (result >= target) goto case1_done;
        }
    case1_clamp:
        w[0x60 / 4] = target;
    case1_done:
        ;
    }

    {
        register s32 v asm("r1") = w[0x64 / 4];
        register s32 target asm("r3") = w[0x5c / 4];

        if (v >= target) goto case2_ge;
        {
            s32 step = w[0x58 / 4];
            register s32 result asm("r0") = v + step;
            w[0x64 / 4] = result;
            if (result <= target) goto case2_done;
            goto case2_clamp;
        }
    case2_ge:
        if (v <= target) goto case2_done;
        {
            s32 step = w[0x58 / 4];
            register s32 result asm("r0") = v - step;
            w[0x64 / 4] = result;
            if (result >= target) goto case2_done;
        }
    case2_clamp:
        w[0x64 / 4] = target;
    case2_done:
        ;
    }

    flags = (u8 *)w + 0x24;
    *flags = 0;

    fx = w[0x60 / 4];
    if (fx > 0) *flags = 1;
    else if (fx < 0) *flags = 2;

    fy = w[0x64 / 4];
    {
        register s32 mask asm("r0");
        if (fy > 0) {
            mask = 8;
        } else if (fy < 0) {
            mask = 4;
        } else {
            goto skipY;
        }
        mask = mask | *flags;
        *flags = mask;
    }
skipY:

    {
        s32 x0 = w[0];
        s32 y0 = w[1];
        w[0x6c / 4] = x0;
        w[0x70 / 4] = y0;
    }
    {
        s32 x = *(vs32 *)&w[0];
        s32 vx = w[0x60 / 4];
        x = x + vx;
        w[0] = x;
        {
            s32 y = *(vs32 *)&w[1];
            s32 vy = w[0x64 / 4];
            y = y + vy;
            w[1] = y;
            {
                s32 *g = (s32 *)0x0300129c;

                if (*g != 0 && vy == 0) {
                    *g = vy;
                }
                *g = vy;

                return (vx != 0 || vy != 0);
            }
        }
    }
}
#endif /* NON_MATCHING */
asm(".align 2, 0");
