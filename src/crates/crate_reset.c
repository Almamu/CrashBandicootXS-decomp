#include "core.h"
#include "match.h"
#include "crate.h"
#include "crates.h"
#include "globals.h"
#include "player.h"

/* GitHub issue #13: 0x0800FC70-0x08010A0C, continuing the physics/
 * collision subsystem (see `ResetCrate`'s header comment below and
 * docs/matching/archive/issue-13-graphics-fc70.md). */

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
 * `FindLineCrossingYMajor`/`FindLineCrossingXMajor` (crate.c) already establish.
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
 * `GetTopCrate`/`GetBottomCrate` (docs/matching/archive/naked-sub_8010914-matched.md):
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
s32 FindLineCrossing(s32 pos, s32 countArg, s32 a, s32 b, s32 limit)
{
    MATCH_HOLD_REG(s32, count, r1) = countArg;
    s32 dx, dy, absDx;
    s32 tmp;

    if (count > b) {
        tmp = count;
        count = b;
        b = tmp;
        tmp = pos;
        pos = a;
        a = tmp;
    }

    dx = a - pos;
    dy = b - count;

    if (dx > 0) {
        if (dx > dy) {
            /* X major, increasing */
            s32 twoDy = dy * 2;
            MATCH_HOLD_REG(s32, diff, r6);
            MATCH_HOLD_REG(s32, err, r0);
            s32 n;

            // clang-format off
            asm volatile(
                "lsl r0, %1, #1\n\t"
                "sub %0, %2, r0\n\t"
                : "=r"(diff)
                : "r"(dx), "r"(twoDy)
                : "r0"
            );
            // clang-format on
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
                MATCH_HOLD_REG(s32, retVal, r0);
                asm volatile("add %0, %1, #0" : "=r"(retVal) : "r"(count));
                return retVal;
            }
        } else {
            /* Y major, X advances conditionally, increasing */
            s32 twoDx = dx * 2;
            MATCH_HOLD_REG(s32, diff, r6);
            MATCH_HOLD_REG(s32, err, r0);
            s32 n;

            // clang-format off
            asm volatile(
                "lsl r0, %1, #1\n\t"
                "sub %0, %2, r0\n\t"
                : "=r"(diff)
                : "r"(dy), "r"(twoDx)
                : "r0"
            );
            // clang-format on
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
            MATCH_HOLD_REG(s32, diff, r6);
            MATCH_HOLD_REG(s32, err, r0);
            s32 n;

            // clang-format off
            asm volatile(
                "lsl r0, %1, #1\n\t"
                "sub %0, %2, r0\n\t"
                : "=r"(diff)
                : "r"(absDx), "r"(twoDy)
                : "r0"
            );
            // clang-format on
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
            MATCH_HOLD_REG(s32, diff, r6);
            MATCH_HOLD_REG(s32, err, r0);
            s32 n;

            // clang-format off
            asm volatile(
                "lsl r0, %1, #1\n\t"
                "sub %0, %2, r0\n\t"
                : "=r"(diff)
                : "r"(dy), "r"(twoAbsDx)
                : "r0"
            );
            // clang-format on
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

/* GitHub issue #13: 0x0800FC70-0x08010A0C - continues the physics/
 * collision subsystem `crate_hit.c`/`crate_break.c` started (see
 * docs/matching/archive/issue-12-physics-collision.md and
 * docs/matching/archive/issue-13-graphics-fc70.md), still in the same
 * `asm/code_3_2_17_e560.s` region issue #12 left untouched past its
 * own scope. Recategorized `graphics` -> `game_loop` for the same
 * reason issue #12 recategorized the previous span: this whole
 * neighborhood (~0x0800D000-0x08010D54) is `docs/rom_map.md`'s
 * confirmed shared physics/collision subsystem, not per-entity
 * behavior. `UpdateCrateFall`/`FindLineCrossing` immediately before this
 * function are left untouched raw - see the write-up doc. */

/* Resets `self`'s collision-response bookkeeping: sets `flags` bits
 * 2/6, clears the low 7 bits of `state` while also clearing the global
 * `gPlayer->busy` "hit" latch, then zeroes the fall/per-kind fields
 * (`fallDistance` through `paramB`, and `touched`) and the stack links
 * `above`/`below` (GetCrateAbove/GetCrateBelow in crate.c), and sets
 * `trialKind` to -1 (none). */
void ResetCrate(struct crate *selfArg)
{
    MATCH_HOLD_REG(struct crate *, self, r2) = selfArg;
    u8 v = 4;
    MATCH_HOLD_REG(u8 *, addr, r3);
    u8 zero;

    v |= self->flags;
    v |= 0x40;
    /* Retyped store: through the plain member, gcc builds the `zero`
     * below before this store instead of after it. */
    *(u8 *)&self->flags = v;

    /* Inline-asm-anchored: the ROM computes the 0x7f/0x80 mask
     * immediate *before* the `ldrb` byte load in both of these
     * AND-and-store sequences (`movs r0,#mask; ldrb r4,[r3];
     * ands r0,r4; strb r0,[r3]`), with the loaded byte specifically
     * in r4 and the mask/result in r0 - every plain-C phrasing tried
     * (compound assignment either direction, a named "mask"/"loaded"
     * pair with and without register pins) instead had this compiler
     * either load the byte first or land the AND result in the wrong
     * register. Anchoring the exact instruction sequence here was
     * more reliable than continuing to chase the scheduler. */
    addr = &self->state;
    {
        MATCH_HOLD_REG(u8, result, r0);
        // clang-format off
        asm volatile(
            "mov r0, #0x7f\n"
            "ldrb r4, [%1]\n"
            "and r0, r0, r4\n"
            : "=r"(result) : "l"(addr) : "r4"
        );
        // clang-format on
        zero = 0;
        *addr = result;
    }
    gPlayer->busy = zero;

    // clang-format off
    asm volatile(
        "mov r0, #0x80\n"
        "ldrb r4, [%0]\n"
        "and r0, r0, r4\n"
        "strb r0, [%0]\n"
        :: "l"(addr) : "r0", "r4", "memory"
    );
    // clang-format on

    self->fallDistance = zero;
    self->fallSpeed = zero;
    self->u48.solidKind = zero;
    self->timer = zero;
    self->paramA = zero;
    self->paramB = zero;
    self->touched = zero;
    self->trialKind = -1;
    self->above = (struct crate *)(u32)zero;
    self->below = (struct crate *)(u32)zero;
}
