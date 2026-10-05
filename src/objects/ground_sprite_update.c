#include "core.h"
#include "gobj_1a794.h"
#include "objects.h"

/* GitHub issue #9: 0x08007634-0x0800B3F0, game_loop-labeled chunk that
 * turned out to be part of the `actor` category's "part" object family
 * already tracked in part_list.c-ctrl.c (see
 * docs/matching/issue-9-0x08007634-actor.md). `UpdateGroundSprite`/
 * `sub_800A590` sit between part_list.c's raw tail (still-raw
 * CollideGroundSprite/ProbeGroundSpriteTerrain/ProbeGroundSpriteFloor) and the already-matched
 * ground_sprite.c (DrawGroundSprite onward). */

/* Looks up `self`'s current "moving platform" record via the
 * virtual method `m10` (the same method-table convention as
 * `DrawPartList`'s `m20` slot in part_list.c) and, if the record
 * pointer changed since the last call (cached in `self->platform`,
 * non-NULL), nudges `self->y` by the delta between the old and new
 * record's position - interpreted as `record[5] + record[2]` (a Q8
 * byte+halfword sum) when `self->unk_68` reads state `8`, or just
 * `record[2]` (a plain halfword) when it reads state `4`. Reads as a
 * "ride along with a moving platform" hookup: when the platform record
 * moves, carry the rider by the same amount. This function
 * (`UpdateGroundSprite`) additionally makes one extra unconditional
 * `UpdateMovingSprite(self)` call first (return value discarded) - its own
 * purpose isn't examined further here, just reproduced. Its twin
 * `sub_800A590` right below is the same body without that extra call.
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
 * docs/matching/issue-9-0x08007634-actor.md.
 *
 * Function order in this file matches ROM address order
 * (`UpdateGroundSprite` < `sub_800A590`) rather than the two twins' logical
 * "base function then its +1-call variant" relationship, since the
 * linker places each object's functions in source order and this one
 * must land first. */
void UpdateGroundSprite(struct gobj *self)
{
    struct gobj_vtable *tbl;
    s16 off;
    void *addr;
    void *fn;
    register void *rec asm("r3");
    register void *prev asm("r1");
    register s32 delta asm("r1");

    UpdateMovingSprite((struct actor *)self);

    tbl = self->vtable;
    off = tbl->m10.thisOffset;
    addr = (u8 *)self + off;
    fn = tbl->m10.fn;
    rec = (void *)_call_via_r1(addr, fn);
    prev = self->platform;

    if (prev == rec) goto skip;
    if (prev == NULL) goto skip;

    if (self->hitAxes == 8) {
        register s32 prevSum asm("r2");
        register s32 newSum asm("r1");

        /* record[5] + record[2] for `prev` (r1) then `rec` (r3), each
         * ldrsh forced onto its own r5-held #2 offset immediate to
         * match the ROM's register footprint exactly. */
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
        if (prevSum == newSum) goto skip;
        delta = prevSum - newSum;
    } else if (self->hitAxes == 4) {
        register s32 prevVal asm("r0");
        register s32 newVal asm("r1");

        /* Plain record[2] halfwords for `prev` (r1) then `rec` (r3);
         * same r5-held #2 offset idiom for the second load. */
        asm volatile(
            "movs r2, #2\n"
            "ldrsh r0, [r1, r2]\n"
            "movs r5, #2\n"
            "ldrsh r1, [r3, r5]\n"
            : "=r"(prevVal), "=r"(newVal)
            : "r"(prev), "r"(rec)
            : "r2", "r5"
        );
        if (prevVal == newVal) goto skip;
        delta = prevVal - newVal;
    } else {
        goto skip;
    }

    delta <<= 8;
    self->y += delta;

skip:
    self->platform = rec;
}

/* Same shape as `UpdateGroundSprite` above, minus its leading unconditional
 * `UpdateMovingSprite(self)` call. See `UpdateGroundSprite`'s doc comment for the
 * shared logic and the closed register-allocation gap. */
void sub_800A590(struct gobj *self)
{
    struct gobj_vtable *tbl = self->vtable;
    s16 off = tbl->m10.thisOffset;
    void *addr = (u8 *)self + off;
    void *fn = tbl->m10.fn;
    register void *rec asm("r3") = (void *)_call_via_r1(addr, fn);
    register void *prev asm("r1") = self->platform;
    register s32 delta asm("r1");

    if (prev == rec) goto skip;
    if (prev == NULL) goto skip;

    if (self->hitAxes == 8) {
        register s32 prevSum asm("r2");
        register s32 newSum asm("r1");

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
        if (prevSum == newSum) goto skip;
        delta = prevSum - newSum;
    } else if (self->hitAxes == 4) {
        register s32 prevVal asm("r0");
        register s32 newVal asm("r1");

        asm volatile(
            "movs r2, #2\n"
            "ldrsh r0, [r1, r2]\n"
            "movs r5, #2\n"
            "ldrsh r1, [r3, r5]\n"
            : "=r"(prevVal), "=r"(newVal)
            : "r"(prev), "r"(rec)
            : "r2", "r5"
        );
        if (prevVal == newVal) goto skip;
        delta = prevVal - newVal;
    } else {
        goto skip;
    }

    delta <<= 8;
    self->y += delta;

skip:
    self->platform = rec;
}
asm(".align 2, 0");
