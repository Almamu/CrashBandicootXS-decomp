#include "core.h"

/* GitHub issue #13: 0x0800FC70-0x08010A0C, continuing the physics/
 * collision subsystem (see game_loop17.c's header comment and
 * docs/matching/issue-13-graphics-fc70.md). */

/* Full 4-octant Bresenham-line-style line-stepper: treats `(pos,
 * count)` and `(a, b)` as two `(position, value)` pairs, sorts them by
 * `count`/`b` (swapping both coordinates together if `count > b`),
 * then walks from the lower-`count` point toward the higher-`count`
 * one, returning the `pos` (`x`-like) coordinate the moment it steps
 * past `limit`, or -1 if the walk completes `|b - count|` steps
 * without ever reaching it. Which of `pos`/`count` is the "major"
 * (always-advancing) axis and which direction `pos` steps depends on
 * the sign and relative magnitude of `dx = a - pos` vs `dy = b -
 * count`, the classic 4-case Bresenham octant split - each case is a
 * fixed single-octant variant of the same shape
 * `sub_8010784`/`sub_80107C4` (game_loop31.c) already establish.
 *
 * Was NAKED asm, not plain C: this compiler's cross-jump pass used to
 * notice the X-major-increasing case's own early-return (`adds
 * r0,r1,#0; b <exit>`) is byte-identical to the shared early-return
 * the other three cases fold into one physical copy (matching the
 * ROM's own choice there), and folded *all four* into a single shared
 * tail - 4 bytes shorter than the ROM, which keeps the first case's
 * copy separate (its own early-return is never reached from any other
 * case) while still sharing the other three. Closed with the same
 * `goto`-to-a-physically-earlier-label technique already proven for
 * `sub_8010914`/`sub_801095C` (docs/matching/naked-sub_8010914-matched.md):
 * the X-major-increasing case's own return is written as a `goto
 * returnSolo;` whose target is placed immediately after that case's
 * own loop (before case 2's code, matching the ROM's own block
 * order), while the other three cases keep plain `return count;`
 * statements that this compiler's own cross-jump pass still merges
 * into a single shared tail on its own - exactly reproducing the
 * ROM's "one solo copy, one copy shared by three" layout instead of a
 * single 4-way merge. The solo copy is additionally materialized as a
 * literal two-instruction `asm volatile` block (`add r0, <count>,
 * #0`) rather than a plain `return count;`: with matching source
 * structure alone, gcc's crossjump pass still recognized the solo
 * `mov r0,r1;b <exit>` sequence as identical to the shared one purely
 * by instruction content (irrespective of source placement) and
 * folded them anyway. An inline-asm block is a fundamentally
 * different (opaque) RTL node to that pass, so it can never be
 * unified with the plain-C-generated shared copy even when the final
 * bytes coincide.
 *
 * Each case's `diff`/`err` computation additionally needed `diff`/
 * `err` pinned to `r6`/`r0` (the ROM's own fixed register roles for
 * these values in every one of the four cases) plus a small
 * `asm volatile` for the `diff` calculation itself
 * (`lsl r0, <subtrahend>, #1` / `sub <diff>, <minuend*2>, r0`):
 * unconstrained, this compiler computes the doubled subtrahend
 * directly into `diff`'s own pinned register (`r6`) as scratch,
 * whereas the ROM always uses `r0` (the not-yet-live `err`'s own
 * register) as that scratch instead - materializing the exact
 * instruction/register pair as opaque asm reproduces the ROM's
 * choice. `count` is pinned to `r1` throughout (the ROM's own choice,
 * matching its role as both an ordinary accumulator and the eventual
 * return value) - required to avoid an extra copy elsewhere in the
 * function once `err` claims `r0`. None of this pins `r7`: `limit`
 * (alive across all four cases, matching the ROM's own persistent
 * `r7`) is left as a completely unconstrained parameter - with `r0`/
 * `r1`/`r6` already claimed by `err`/`count`/`diff`, this compiler's
 * own allocator has nowhere else to put it and picks `r7` on its own,
 * matching the ROM exactly (see `matching_decomp_register_pinning`
 * memory point 10 / docs/matching.md's extensive r7-must-stay-
 * unpinned notes - pinning `r7` explicitly is a confirmed toolchain
 * bug that silently drops it from the prologue's push/pop list). */
s32 sub_800FDC8(s32 pos, s32 countArg, s32 a, s32 b, s32 limit)
{
    register s32 count asm("r1") = countArg;
    s32 dx, dy, absDx;
    s32 tmp;

    if (count > b) {
        tmp = count; count = b; b = tmp;
        tmp = pos; pos = a; a = tmp;
    }

    dx = a - pos;
    dy = b - count;

    if (dx > 0) {
        if (dx > dy) {
            /* X major, increasing */
            s32 twoDy = dy * 2;
            register s32 diff asm("r6");
            register s32 err asm("r0");
            s32 n;

            asm volatile(
                "lsl r0, %1, #1\n\t"
                "sub %0, %2, r0\n\t"
                : "=r"(diff)
                : "r"(dx), "r"(twoDy)
                : "r0"
            );
            err = twoDy - dx;
            n = dx - 1;
            if (n != -1) {
                do {
                    if (err >= 0) {
                        count++;
                        err += diff;
                    } else {
                        err += twoDy;
                    }
                    if (++pos >= limit) {
                        goto returnSolo;
                    }
                    n--;
                } while (n != -1);
            }
            goto returnNeg1;

returnSolo:
            {
                register s32 retVal asm("r0");
                asm volatile("add %0, %1, #0" : "=r"(retVal) : "r"(count));
                return retVal;
            }
        } else {
            /* Y major, X advances conditionally, increasing */
            s32 twoDx = dx * 2;
            register s32 diff asm("r6");
            register s32 err asm("r0");
            s32 n;

            asm volatile(
                "lsl r0, %1, #1\n\t"
                "sub %0, %2, r0\n\t"
                : "=r"(diff)
                : "r"(dy), "r"(twoDx)
                : "r0"
            );
            err = twoDx - dy;
            n = dy - 1;
            if (n != -1) {
                do {
                    if (err >= 0) {
                        if (++pos >= limit) {
                            return count;
                        }
                        err += diff;
                    } else {
                        err += twoDx;
                    }
                    count++;
                    n--;
                } while (n != -1);
            }
        }
    } else {
        absDx = -dx;
        if (absDx > dy) {
            /* X major, decreasing */
            s32 twoDy = dy * 2;
            register s32 diff asm("r6");
            register s32 err asm("r0");
            s32 n;

            asm volatile(
                "lsl r0, %1, #1\n\t"
                "sub %0, %2, r0\n\t"
                : "=r"(diff)
                : "r"(absDx), "r"(twoDy)
                : "r0"
            );
            err = twoDy - absDx;
            n = absDx - 1;
            if (n != -1) {
                do {
                    if (err >= 0) {
                        count++;
                        err += diff;
                    } else {
                        err += twoDy;
                    }
                    if (--pos >= limit) {
                        return count;
                    }
                    n--;
                } while (n != -1);
            }
        } else {
            /* Y major, X advances conditionally, decreasing */
            s32 twoAbsDx = absDx * 2;
            register s32 diff asm("r6");
            register s32 err asm("r0");
            s32 n;

            asm volatile(
                "lsl r0, %1, #1\n\t"
                "sub %0, %2, r0\n\t"
                : "=r"(diff)
                : "r"(dy), "r"(twoAbsDx)
                : "r0"
            );
            err = twoAbsDx - dy;
            n = dy - 1;
            if (n != -1) {
                do {
                    if (err >= 0) {
                        if (--pos >= limit) {
                            return count;
                        }
                        err += diff;
                    } else {
                        err += twoAbsDx;
                    }
                    count++;
                    n--;
                } while (n != -1);
            }
        }
    }

returnNeg1:
    return -1;
}
