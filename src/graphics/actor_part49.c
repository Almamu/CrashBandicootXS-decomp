#include "core.h"

/* GitHub issue #9: 0x08007634-0x0800B3F0, game_loop-labeled chunk that
 * turned out to be part of the `actor` category's "part" object family
 * (see docs/matching/issue-9-0x08007634-actor.md). `sub_800B270` sits
 * right after the still-raw `sub_800AFF4`, at the end of that raw span. */

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
 * store) since that's what the ROM actually does. This compiler proves
 * that redundancy and folds the branch's condition down to just
 * `vy == 0` regardless of C-level phrasing (plain `if`, a `volatile`-
 * qualified pointee, an opaque `asm volatile("" : "+r"(gval))` barrier
 * on the loaded value - none stop it), so the whole 8-instruction
 * load/compare/branch/store sequence is instead emitted verbatim via
 * one opaque `asm volatile` block, matching the ROM's exact
 * instructions (and its address-in-`r0`/value-in-`r2` register
 * choice) directly rather than fighting the optimizer's proof.
 *
 * Pinning `vx`/`vy` (the loaded X/Y velocities, needed in `r3`/`r1` to
 * match the ROM through the position-update stores and into the
 * `0x0300129C` block) individually is safe, but pinning *both* at once
 * previously broke the function's own final `return (vx != 0 || vy !=
 * 0)` - the compiler folded it to an unconditional `mov r0, #1`, a
 * genuine gcc-2.9 register-pin miscompile (same class of correctness
 * bug as the confirmed r7-pin hazard elsewhere in this project, just
 * triggered by a different register pair here) rather than a cosmetic
 * mismatch. Fixed by leaving `vy` as a plain, unpinned local - it
 * still lands in `r1` naturally - and pinning only `vx` to `r3`. */
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
        register s32 vx asm("r3") = w[0x60 / 4];
        x = x + vx;
        w[0] = x;
        {
            s32 y = *(vs32 *)&w[1];
            s32 vy = w[0x64 / 4];
            y = y + vy;
            w[1] = y;
            {
                register vs32 *g asm("r0") = (vs32 *)0x0300129c;

                asm volatile(
                    "ldr r2, [%0, #0]\n\t"
                    "cmp r2, #0\n\t"
                    "beq 1f\n\t"
                    "cmp %1, #0\n\t"
                    "bne 1f\n\t"
                    "str %1, [%0, #0]\n\t"
                    "1:\n\t"
                    "str %1, [%0, #0]\n\t"
                    :
                    : "r"(g), "r"(vy)
                    : "r2", "cc", "memory"
                );

                return (vx != 0 || vy != 0);
            }
        }
    }
}
asm(".align 2, 0");
