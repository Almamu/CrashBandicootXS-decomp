#include "core.h"
#include "math_util.h"
#include "match.h"
#include "gobj_1a794.h"
#include "objects.h"

/* GitHub issue #9: 0x08007634-0x0800B3F0, game_loop-labeled chunk that
 * turned out to be part of the `actor` category's "part" object family
 * already tracked in part_list.c-ctrl.cpp (see
 * docs/matching/archive/issue-9-0x08007634-actor.md). `UpdateGroundSprite`/
 * `AnchorGroundSpriteHitbox` sit between part_list.c's raw tail (still-raw
 * CollideGroundSprite/ProbeGroundSpriteTerrain/ProbeGroundSpriteFloor) and the already-matched
 * ground_sprite.c (DrawGroundSprite onward). */

/* Keeps the side of a ground sprite's hitbox it is resting on in place
 * when the hitbox changes: calls `self`'s hitbox method (`m10`, slot 2,
 * GetSpriteObjHitbox's `{s16 xOff, s16 yOff, u8 w, u8 h}` record of the
 * current frame) and, if the record changed since the last call (cached
 * in `self->lastHitbox`, non-NULL), moves `self->y` by the difference
 * between the old and the new record's bottom edge (`yOff + h`) when
 * `self->hitAxes` is 8 (standing on the floor), or their top edge
 * (`yOff`) when it is 4 (against the ceiling). This function
 * (`UpdateGroundSprite`) first runs `UpdateMovingSprite(self)` (return
 * value discarded); its twin `AnchorGroundSpriteHitbox` right below is
 * the same body without that call.
 *
 * Matched. `self` pins to r4 and the trampoline's returned record pins
 * to r3, reproducing most of the ROM's register choices directly.  The
 * one remaining gap was the inner scratch-register allocation reading
 * each record's `+2` halfword/`+5` byte pair: the ROM keeps the record
 * pointer (r3) live across both loads and spends a genuine *fifth*
 * register (r5) purely to hold the immediate `2` offset for the
 * `ldrsh` (since r0-r3 are all already committed to `self`/`target`/the
 * two sum operands), while no C-level phrasing tried ever made this
 * compiler's register allocator introduce that fifth register on its
 * own - it always found a way to reuse one of r0-r3 instead, a
 * *smaller* register footprint than the ROM's own but not the same
 * bytes. Closed by materializing the ROM's own scratch-register
 * sequence directly via `asm volatile`, with `prev`/`rec` passed in
 * through registers already pinned to r1/r3 and the two summed halves
 * pinned to the exact ROM output registers (r2/r1), so the compiler
 * only has to generate the surrounding control flow around a literal
 * transcription of the ROM's own instructions - see
 * docs/matching/archive/issue-9-0x08007634-actor.md.
 *
 * Function order in this file matches ROM address order
 * (`UpdateGroundSprite` < `AnchorGroundSpriteHitbox`) rather than the two twins' logical
 * "base function then its +1-call variant" relationship, since the
 * linker places each object's functions in source order and this one
 * must land first. */
void UpdateGroundSprite(struct gobj *self)
{
    struct gobj_vtable *tbl;
    s16 off;
    void *addr;
    void *fn;
    MATCH_HOLD_REG(void *, rec, r3);
    MATCH_HOLD_REG(void *, prev, r1);
    MATCH_HOLD_REG(s32, delta, r1);

    UpdateMovingSprite((struct actor *)self);

    tbl = self->vtable;
    off = tbl->m10.thisOffset;
    addr = (u8 *)self + off;
    fn = tbl->m10.fn;
    rec = (void *)_call_via_r1(addr, fn);
    prev = self->lastHitbox;

    if (prev == rec)
        goto skip;
    if (prev == NULL)
        goto skip;

    if (self->hitAxes == 8) {
        MATCH_HOLD_REG(s32, prevSum, r2);
        MATCH_HOLD_REG(s32, newSum, r1);

        /* record[5] + record[2] for `prev` (r1) then `rec` (r3), each
         * ldrsh forced onto its own r5-held #2 offset immediate to
         * match the ROM's register footprint exactly. */
        // clang-format off
        asm volatile(
            "movs r5, #2\n"
            "ldrsh r0, [r1, r5]\n"
            "ldrb r1, [r1, #5]\n"
            "add r2, r1, r0\n"
            "movs r1, #2\n"
            "ldrsh r0, [r3, r1]\n"
            "ldrb r5, [r3, #5]\n"
            "add r1, r5, r0\n"
            : "=r"(prevSum), "=r"(newSum)
            : "r"(prev), "r"(rec)
            : "r0", "r5"
        );
        // clang-format on
        if (prevSum == newSum)
            goto skip;
        delta = prevSum - newSum;
    } else if (self->hitAxes == 4) {
        MATCH_HOLD_REG(s32, prevVal, r0);
        MATCH_HOLD_REG(s32, newVal, r1);

        /* Plain record[2] halfwords for `prev` (r1) then `rec` (r3);
         * same r5-held #2 offset idiom for the second load. */
        // clang-format off
        asm volatile(
            "movs r2, #2\n"
            "ldrsh r0, [r1, r2]\n"
            "movs r5, #2\n"
            "ldrsh r1, [r3, r5]\n"
            : "=r"(prevVal), "=r"(newVal)
            : "r"(prev), "r"(rec)
            : "r2", "r5"
        );
        // clang-format on
        if (prevVal == newVal)
            goto skip;
        delta = prevVal - newVal;
    } else {
        goto skip;
    }

    delta = INT_TO_Q8(delta);
    self->y += delta;

skip:
    self->lastHitbox = rec;
}

/* UNUSED - no caller anywhere in the ROM (checked src/, asm/ and the
 * method tables).
 *
 * Same shape as `UpdateGroundSprite` above, minus its leading unconditional
 * `UpdateMovingSprite(self)` call. See `UpdateGroundSprite`'s doc comment for the
 * shared logic and the closed register-allocation gap. */
void AnchorGroundSpriteHitbox(struct gobj *self)
{
    struct gobj_vtable *tbl = self->vtable;
    s16 off = tbl->m10.thisOffset;
    void *addr = (u8 *)self + off;
    void *fn = tbl->m10.fn;
    MATCH_HOLD_REG(void *, rec, r3) = (void *)_call_via_r1(addr, fn);
    MATCH_HOLD_REG(void *, prev, r1) = self->lastHitbox;
    MATCH_HOLD_REG(s32, delta, r1);

    if (prev == rec)
        goto skip;
    if (prev == NULL)
        goto skip;

    if (self->hitAxes == 8) {
        MATCH_HOLD_REG(s32, prevSum, r2);
        MATCH_HOLD_REG(s32, newSum, r1);

        // clang-format off
        asm volatile(
            "movs r5, #2\n"
            "ldrsh r0, [r1, r5]\n"
            "ldrb r1, [r1, #5]\n"
            "add r2, r1, r0\n"
            "movs r1, #2\n"
            "ldrsh r0, [r3, r1]\n"
            "ldrb r5, [r3, #5]\n"
            "add r1, r5, r0\n"
            : "=r"(prevSum), "=r"(newSum)
            : "r"(prev), "r"(rec)
            : "r0", "r5"
        );
        // clang-format on
        if (prevSum == newSum)
            goto skip;
        delta = prevSum - newSum;
    } else if (self->hitAxes == 4) {
        MATCH_HOLD_REG(s32, prevVal, r0);
        MATCH_HOLD_REG(s32, newVal, r1);

        // clang-format off
        asm volatile(
            "movs r2, #2\n"
            "ldrsh r0, [r1, r2]\n"
            "movs r5, #2\n"
            "ldrsh r1, [r3, r5]\n"
            : "=r"(prevVal), "=r"(newVal)
            : "r"(prev), "r"(rec)
            : "r2", "r5"
        );
        // clang-format on
        if (prevVal == newVal)
            goto skip;
        delta = prevVal - newVal;
    } else {
        goto skip;
    }

    delta = INT_TO_Q8(delta);
    self->y += delta;

skip:
    self->lastHitbox = rec;
}
