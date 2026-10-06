#include "core.h"
#include "match.h"
#include "vtable.h"
#include "actor.h"
#include "aabb.h"
#include "crates.h"
#include "objects.h"
#include "memory.h"
#include "globals.h"
#include "player.h"
#include "crate.h"


/* GitHub issue #13: 0x0800FC70-0x08010A0C, continuing the physics/
 * collision subsystem (see crate_reset.c's header comment and
 * docs/matching/archive/issue-13-graphics-fc70.md). `IsCrateInsideRect` prepended
 * ahead of the already-matched `ResolvePlayerCollisions` run below - it's
 * immediately ROM-adjacent (no gap), so it joins this file rather
 * than getting its own per docs/workflow.md's "one file per
 * contiguous ROM region" rule. See
 * docs/matching/archive/issue-13-fc70-second-continuation.md for the
 * register-pinning/toolchain-bug notes this one needed. */

/* AABB-overlap test between `self`'s own table-driven half-width/
 * half-height box (centered on `self`'s own position, via the same
 * `_call_via_r1` table-trampoline convention `CheckEntityPlayerContact`/
 * `UpdateEntity`, graphics.c, already establish - here at the table's
 * own `+0x10`/`+0x14` offset pair) and a caller-supplied `struct aabb
 * *`. Short-circuits true (skipping the real test) when `flags` bit 4
 * is set, or while the crate is falling (`fallDistance`, where the base
 * class's IsSpriteObjInsideRect tests a moving sprite's controller). */
u32 IsCrateInsideRect(struct crate *selfArg, struct aabb *boxArg)
{
    /* self/box pinned to r5/r6: the ROM keeps both live across the
     * whole function (self is dead by the AABB-build block below and
     * gets reused there for `top`; box stays live until the very last
     * compare). result is pinned to r1, matching the ROM's own
     * shared merge point for both the shortcut-true and real-test
     * return paths (`adds r1,r7,#0` / already-r1; `adds r0,r1,#0`) -
     * declaring it (and the function's return type) as `u32` rather
     * than `u8` avoids this compiler's narrow-register-return
     * zero-extend dance (an unconditional `lsls r0,r0,#24; lsrs
     * r0,r0,#24` a `u8`-typed register return always adds, that the
     * ROM never has - callers here still only read the low byte, so
     * the wider C return type changes nothing observable). */
    MATCH_HOLD_REG(struct crate *, self, r5) = selfArg;
    MATCH_HOLD_REG(struct aabb *, box, r6) = boxArg;
    u8 skip = (self->flags >> 4) & 1;
    MATCH_HOLD_REG(u32, result, r1);

    if (self->fallDistance != 0) {
        skip = 1;
    }

    if (!skip) {
        MATCH_HOLD_REG(struct vtable_slot *, table, r1) = (struct vtable_slot *)self->vtable;
        MATCH_HOLD_REG(u8 *, rec, r0) =
            (u8 *)_call_via_r1((u8 *)self + table[2].delta, table[2].fn);
        MATCH_HOLD_REG(s32, left, r4);
        MATCH_HOLD_REG(s32, right, r1);
        MATCH_HOLD_REG(s32, top, r5);
        MATCH_HOLD_REG(s32, bottom, r3);
        u8 success;

        /* Anchored: builds `self`'s AABB (half-extents from the
         * `_call_via_r1` record's `+4`/`+5` raw w/h bytes, shifted by
         * 7 rather than 8) directly into the ROM's own register
         * choices. Plain C here always let this compiler's scheduler
         * hoist the `self->y` load ahead of the still-pending
         * `rec[5] << 7` shift, stealing r3/r0 from each other (the
         * same "which anonymous scratch register" gap already
         * NAKED-parked for `GetSpriteBounds`/`GetSpriteHitbox`,
         * sprite.c) - anchored as one literal block instead,
         * matching `self`'s own register (r5, reused here for `top`
         * once `self` is dead) and `rec`'s (r0, reused for the
         * `self->y` load once `rec` is dead). */
        asm volatile(
            "ldrb r1, [r0, #4]\n\t"
            "lsl r2, r1, #7\n\t"
            "ldrb r0, [r0, #5]\n\t"
            "lsl r3, r0, #7\n\t"
            "ldr r1, [r5]\n\t"
            "sub r4, r1, r2\n\t"
            "ldr r0, [r5, #4]\n\t"
            "sub r5, r0, r3\n\t"
            "add r1, r1, r2\n\t"
            "add r3, r0, r3\n\t"
            : "=r"(left), "=r"(right), "=r"(top), "=r"(bottom)
            : "r"(rec), "r"(self)
            : "r0", "r2", "cc", "memory"
        );

        /* `box->x`/`box->y` are each read once, into r2,
         * and reused for both their own edge compare and the
         * opposite edge's sum (`ble`/`bge` short-circuiting straight
         * past the remaining checks on failure) - the register-pinned
         * locals force that single load/reuse. The sum itself
         * (`box->x + box->w`, `box->y +
         * box->h`) is anchored too: this compiler always
         * computes it in-place into whichever operand's register is
         * written first in the C expression, but the ROM keeps the
         * running edge value (r2) as the *first* source operand while
         * still landing the sum in the freshly-loaded field's own
         * register (r0) - a dest-vs-first-operand split no source
         * reordering here reproduced (the same "which anonymous
         * scratch register" gap as the AABB-build block above).
         * `success` is deliberately left as a plain (non-`register`)
         * local: pinning it to r7 (matching the ROM's own accumulator
         * choice) hits a confirmed toolchain bug where this compiler
         * never adds an inline-asm-clobbered r7 to the function's own
         * push/pop list (see docs/status/game_loop.md's `DropExtraLife`
         * entry for the same bug elsewhere) - left natural, the
         * ordinary if/else control flow below happens to allocate
         * `success` to r7 anyway, and the compiler *does* then track
         * it correctly for save/restore. */
        success = 0;
        {
            MATCH_HOLD_REG(s32, boxX, r2) = box->x;
            if (left > boxX) {
                MATCH_HOLD_REG(s32, boxRight, r0);
                asm volatile(
                    "ldr r0, [r6, #8]\n\t"
                    "add r0, r2, r0\n\t"
                    : "=r"(boxRight)
                    : "r"(box), "r"(boxX)
                    : "cc"
                );
                if (right < boxRight) {
                    MATCH_HOLD_REG(s32, boxY, r2) = box->y;
                    if (top > boxY) {
                        MATCH_HOLD_REG(s32, boxBottom, r0);
                        asm volatile(
                            "ldr r0, [r6, #0xc]\n\t"
                            "add r0, r2, r0\n\t"
                            : "=r"(boxBottom)
                            : "r"(box), "r"(boxY)
                            : "cc"
                        );
                        if (bottom < boxBottom) {
                            success = 1;
                        }
                    }
                }
            }
        }
        result = success;
    } else {
        result = skip;
    }

    return result;
}
/* Trailing byte count isn't a multiple of 4 - without this, `as` pads
 * with its default NOP fill instead of the ROM's zero fill (see
 * docs/matching.md's alignment-padding gotcha). */
asm(".align 2, 0");

/* Refreshes the viewport's own collision box (`ResolveCollisionCandidates` on
 * `gPlayer->collisionQueue`), then increments its `bounce` counter by
 * one as long as it isn't already zero (a saturating-at-zero
 * "recently hit" style counter, never incremented back up from 0). */
void ResolvePlayerCollisions(void)
{
    struct player *p = gPlayer;
    u8 *p2;

    ResolveCollisionCandidates(&p->collisionQueue);
    p2 = &gPlayer->bounce;
    if (*p2 != 0) {
        *p2 = *p2 + 1;
    }
}

/* Neighbor-list "get prev" accessor - reads `self+0x60`, the field
 * `ResetCrate` (crate_reset.c) zeroes on reset. */
struct crate *GetCrateBelow(struct crate *self)
{
    return self->below;
}

/* Neighbor-list "get next" accessor - reads `self+0x5c`. */
struct crate *GetCrateAbove(struct crate *self)
{
    return self->above;
}

/* Neighbor-list "set prev" mutator - writes `self+0x60`. */
void SetCrateBelow(struct crate *self, struct crate *val)
{
    self->below = val;
}

/* Neighbor-list "set next" mutator - writes `self+0x5c`. */
void SetCrateAbove(struct crate *self, struct crate *val)
{
    self->above = val;
}

/* UNUSED - no caller anywhere in the ROM (checked every asm/*.s,
 * expected/*.s and src/*.c file) - trivial constant accessor, always
 * returns 3. */
u32 GetCrateClassId(void)
{
    return 3;
}

/* GitHub issue #13: 0x0800FC70-0x08010A0C, continuing the physics/
 * collision subsystem (see crate_reset.c's header comment and
 * docs/matching/archive/issue-13-graphics-fc70.md). */

/* Sets `self->table`, then - if `self`'s own `+0x4e` state byte is 3 -
 * frees `self+0x48` (a heap pointer, unless it's the sentinel `-1` or
 * already `NULL`) and clears `self+0x59`, before tail-calling
 * `DestroySpriteObj` (already matched, `sprite_obj.c`) - same table-set/
 * tail-call shape as `DestroyWumpa` (`wumpa.c`). */
void DestroyCrate(struct actor *self, u32 arg1)
{
    self->table = (void *)gCrateVtable;

    if (*((u8 *)self + 0x4e) == 3) {
        void *p = *(void **)((u8 *)self + 0x48);
        if ((u32)((u8 *)p + 1) > 1) {
            if (p != NULL) {
                OperatorDeleteArray(p);
            }
            *((u8 *)self + 0x59) = 0;
        }
    }

    DestroySpriteObj(self, arg1);
}

/* Re-initializes `self` via `InitSpriteObj` (already matched,
 * `sprite_obj.c`), sets `self->table`, clears `self+0x59`, then
 * resets `self`'s own collision-response state via `ResetCrate`
 * (`crate_reset.c`). */
struct actor *InitCrate(struct actor *self)
{
    InitSpriteObj(self);
    self->table = (void *)gCrateVtable;
    *((u8 *)self + 0x59) = 0;
    ResetCrate((struct crate *)self);
    return self;
}

/* Bresenham-line-style step algorithm (see `FindLineCrossing`
 * in crate_reset.c for the general 4-octant version this
 * is a fixed single-octant variant of): walks `dy` steps, accumulating
 * `count`; whenever the running error term `err` is non-negative,
 * steps `y` by `yStep` and folds the error by `diff = 2*dx - 2*dy`
 * (returning the *current* `count` immediately, without incrementing
 * it, the moment `y` reaches `bound`), otherwise just folds by
 * `2*dx`. Returns `-1` if the walk completes all `dy` steps without
 * `y` ever reaching `bound`. */
s32 FindLineCrossingYMajor(s32 y, s32 count, s32 dx, s32 dy, s32 yStep, s32 bound)
{
    s32 twoDx = dx * 2;
    s32 diff = twoDx - dy * 2;
    s32 err = twoDx - dy;
    s32 n = dy - 1;

    if (n != -1) {
        do {
            if (err >= 0) {
                y += yStep;
                if (y >= bound) {
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
    return -1;
}

/* Same shape as `FindLineCrossingYMajor`, but stepping `x` (2nd count-position
 * pair swapped relative to that function) instead of `y` - walks `dx`
 * steps this time, checking `y >= bound` each time the error term
 * folds. */
s32 FindLineCrossingXMajor(s32 y, s32 count, s32 dx, s32 dy, s32 yStep, s32 bound)
{
    s32 twoDy = dy * 2;
    s32 diff = twoDy - dx * 2;
    s32 err = twoDy - dx;
    s32 n = dx - 1;

    if (n != -1) {
        do {
            if (err >= 0) {
                count++;
                err += diff;
            } else {
                err += twoDy;
            }
            y += yStep;
            if (y >= bound) {
                return count;
            }
            n--;
        } while (n != -1);
    }
    return -1;
}
