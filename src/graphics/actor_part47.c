#include "core.h"

/* GitHub issue #9: 0x08007634-0x0800B3F0, game_loop-labeled chunk that
 * turned out to be part of the `actor` category's "part" object family
 * already tracked in actor_part11.c-actor_part17.c (see
 * docs/matching/issue-9-0x08007634-actor.md). `sub_800A528`/
 * `sub_800A590` sit between actor_part11.c's raw tail (still-raw
 * sub_800A0FC/sub_800A178/sub_800A420) and the already-matched
 * actor_part14.c (sub_800A5F4 onward). */

extern void *sub_803AD7C(void *arg0, void *fn);
extern void sub_8009FB0(void *self);

#if NON_MATCHING
/* Looks up `self`'s current "moving platform" record via the
 * `self->table+0x10/0x14`-driven trampoline (the same base+offset+
 * fn-pointer convention as `sub_8008DC0`'s `+0x20/0x24` slot in
 * actor_part11.c, just a different table slot) and, if the record
 * pointer changed since the last call (cached at `self+0x1c`, non-NULL),
 * nudges `self->y` (`self+4`) by the delta between the old and new
 * record's position - interpreted as `record[5] + record[2]` (a Q8
 * byte+halfword sum) when `self+0x68` reads state `8`, or just
 * `record[2]` (a plain halfword) when it reads state `4`. Reads as a
 * "ride along with a moving platform" hookup: when the platform record
 * moves, carry the rider by the same amount. `sub_800A528` is the same
 * function with one extra unconditional `sub_8009FB0(self)` call first
 * (return value discarded) - its own purpose isn't examined further
 * here, just reproduced.
 *
 * PARKED, NOT BYTE-MATCHING: every load, store, branch and computed
 * delta is confirmed correct against the ROM (`self`/`rec` register
 * roles, the Q8 shift, both state-8/state-4 sum shapes), and pinning
 * `self` to r4 and the trampoline's returned record to r3 reproduces
 * most of the ROM's register choices exactly. The remaining gap is the
 * inner scratch-register allocation used to read each record's `+2`
 * halfword/`+5` byte pair: the ROM keeps the record pointer (`r3`) live
 * across both loads and needs a *fifth* register (`r5`) purely to hold
 * the immediate `2` offset for the `ldrsh` (since r0-r3 are all already
 * committed to `self`/`target`/the two sum operands), while this
 * compiler's register allocator never introduces that fifth register
 * from any C phrasing tried (separate statements for each load in
 * source order, explicit `register ... asm("r5")` pins on the offset
 * constant, alternate operand orders for the final add) - it always
 * finds a way to reuse one of r0-r3 instead, which is a *smaller*
 * register footprint than the ROM's own (no `push {r4,r5}`/`pop
 * {r4,r5}` needed) but not the same bytes. See
 * docs/matching/issue-9-0x08007634-actor.md. */
void sub_800A590(void *selfArg)
{
    u8 *self = selfArg;
    void *tbl = *(void **)(self + 0x18);
    s16 off = *(s16 *)((u8 *)tbl + 0x10);
    void *addr = self + off;
    void *fn = *(void **)((u8 *)tbl + 0x14);
    register void *rec asm("r3") = sub_803AD7C(addr, fn);
    void *prev = *(void **)(self + 0x1c);
    s32 delta;

    if (prev == rec) goto skip;
    if (prev == NULL) goto skip;

    if (self[0x68] == 8) {
        s32 prevSum = *(u8 *)((u8 *)prev + 5) + *(s16 *)((u8 *)prev + 2);
        s32 newSum = *(u8 *)((u8 *)rec + 5) + *(s16 *)((u8 *)rec + 2);
        if (prevSum == newSum) goto skip;
        delta = prevSum - newSum;
    } else if (self[0x68] == 4) {
        s32 prevVal = *(s16 *)((u8 *)prev + 2);
        s32 newVal = *(s16 *)((u8 *)rec + 2);
        if (prevVal == newVal) goto skip;
        delta = prevVal - newVal;
    } else {
        goto skip;
    }

    delta <<= 8;
    *(s32 *)(self + 4) += delta;

skip:
    *(void **)(self + 0x1c) = rec;
}

/* Same shape as `sub_800A590` above, plus an unconditional
 * `sub_8009FB0(self)` call first. See `sub_800A590`'s doc comment for
 * the shared logic and the parked register-allocation gap. */
void sub_800A528(void *selfArg)
{
    u8 *self = selfArg;
    void *tbl;
    s16 off;
    void *addr;
    void *fn;
    void *rec;
    void *prev;
    s32 delta;

    sub_8009FB0(self);

    tbl = *(void **)(self + 0x18);
    off = *(s16 *)((u8 *)tbl + 0x10);
    addr = self + off;
    fn = *(void **)((u8 *)tbl + 0x14);
    rec = sub_803AD7C(addr, fn);
    prev = *(void **)(self + 0x1c);

    if (prev == rec) goto skip;
    if (prev == NULL) goto skip;

    if (self[0x68] == 8) {
        s32 prevSum = *(u8 *)((u8 *)prev + 5) + *(s16 *)((u8 *)prev + 2);
        s32 newSum = *(u8 *)((u8 *)rec + 5) + *(s16 *)((u8 *)rec + 2);
        if (prevSum == newSum) goto skip;
        delta = prevSum - newSum;
    } else if (self[0x68] == 4) {
        s32 prevVal = *(s16 *)((u8 *)prev + 2);
        s32 newVal = *(s16 *)((u8 *)rec + 2);
        if (prevVal == newVal) goto skip;
        delta = prevVal - newVal;
    } else {
        goto skip;
    }

    delta <<= 8;
    *(s32 *)(self + 4) += delta;

skip:
    *(void **)(self + 0x1c) = rec;
}
#endif /* NON_MATCHING */
asm(".align 2, 0");
