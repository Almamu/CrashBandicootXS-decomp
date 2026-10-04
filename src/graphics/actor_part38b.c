#include "core.h"
#include "action_obj.h"

/* Continuation of actor_part38.c (issue #18's chunk) - covers
 * `sub_80151C8`, `sub_8015238` and `sub_80152F0`. Same "self" object
 * family documented at the top of actor_part18.c/actor_part28.c. */

/* One-shot guard (`self+0x23`): the first time through, picks a value
 * (`0x18`/`0x19`/`0x1a`) from `self+0x22` (a small jump table for
 * `[0,1]`/`2`/`[3,4]`, no-op if `self+0x22 > 4`) into `self+0x28`
 * (latching `self+0x30`/`clearing self+0x32` alongside it), then always
 * sets bit 0 of `part+0xd` and clears `self+0x34`. */
void sub_80151C8(void *selfArg)
{
    register u8 *self asm("r3") = selfArg;
    u8 *p23 = self + 0x23;

    if (*p23 != 0) {
        return;
    }
    *p23 = 1;

    /* Hand-written jump table (rather than a plain `switch`, or a C
     * computed-goto table) to get both the exact table layout and the
     * physical order of the three target blocks (`0x18`, `0x19`,
     * `0x1a`) - this compiler's own jump-table lowering for an
     * equivalent `switch` always matched the case-to-value mapping but
     * picked a different, seemingly source-order-independent block
     * layout every time (tried several case-label scatterings); a
     * computed-goto table hit the same table-order problem and also
     * pulled its `static const` array into a discarded `.data`
     * section this ROM has no room for. Spelling the table out in raw
     * asm, using the same "hand-placed local labels shared across a
     * single literal pool" idea as `sub_8014F8C`'s anti-CSE note in
     * actor_part28.c, sidesteps both problems - `self` is pinned to
     * `r3` for the whole function so this block's hardcoded `r3` use
     * matches whatever the compiler already has it in. */
    {
        register s32 val asm("r2");
        register u8 *selfIn asm("r3") = self;

        asm volatile(
            "add r0, r3, #0\n\t"
            "add r0, r0, #0x22\n\t"
            "ldrb r0, [r0]\n\t"
            "cmp r0, #4\n\t"
            "bhi L_80151c8_skip\n\t"
            "lsl r0, r0, #2\n\t"
            "ldr r1, L_80151c8_tbl\n\t"
            "add r0, r0, r1\n\t"
            "ldr r0, [r0]\n\t"
            "mov pc, r0\n\t"
            ".align 2, 0\n\t"
            "L_80151c8_tbl: .word L_80151c8_tbl2\n\t"
            "L_80151c8_tbl2:\n\t"
            "  .word L_80151c8_18\n\t"
            "  .word L_80151c8_18\n\t"
            "  .word L_80151c8_19\n\t"
            "  .word L_80151c8_1a\n\t"
            "  .word L_80151c8_1a\n\t"
            "L_80151c8_18:\n\t"
            "  mov %0, #0x18\n\t"
            "  b L_80151c8_store\n\t"
            "L_80151c8_19:\n\t"
            "  mov %0, #0x19\n\t"
            "  b L_80151c8_store\n\t"
            "L_80151c8_1a:\n\t"
            "  mov %0, #0x1a\n\t"
            "L_80151c8_store:\n\t"
            "  add r1, r3, #0\n\t"
            "  add r1, r1, #0x32\n\t"
            "  mov r0, #0\n\t"
            "  strb r0, [r1]\n\t"
            "  sub r1, r1, #2\n\t"
            "  mov r0, #1\n\t"
            "  strb r0, [r1]\n\t"
            "  add r0, r3, #0\n\t"
            "  add r0, r0, #0x28\n\t"
            "  strb %0, [r0]\n\t"
            "L_80151c8_skip:\n\t"
            : "=r"(val)
            : "r"(selfIn)
            : "r0", "r1", "cc"
        );
    }

    {
        register u8 *part asm("r1") = *(u8 **)(self + 0x10);
        register s32 one asm("r0") = 1;
        register u8 old asm("r2") = part[0xd];

        one |= old;
        part[0xd] = one;
    }
    {
        register u8 *p34 asm("r1") = self + 0x34;
        register s32 zero asm("r0") = 0;

        *p34 = zero;
    }
}
/* Zero-fill alignment before the next function (see docs/matching.md's
 * alignment-padding gotcha). */
asm(".align 2, 0");

extern void *gLevelState;
extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern s32 _call_via_r3(void *arg0, void *arg1, void *arg2, void *arg3);
extern void sub_8015460(void *selfArg);
extern void sub_8015780(void *selfArg, s32 a, s32 b, s32 c, s32 d);
extern s32 HasTurboRun(void *self);

/* Always sets `self+0x26 = 0xc`. For `mode` `3`/`4`: if `flags` bit
 * `0x200` is set and `HasTurboRun(gLevelState)` is true, latches
 * `self+0x29`, fires the mgr trampoline pair with actions `4`/`0x18`,
 * and sets the state/counter/table-index trio (`0x31`/`0x2f`/`0x27`) to
 * `0`/`1`/`0x1b` - otherwise falls back to `sub_8015460`. For every
 * other `mode`: resets via `sub_8015780(self, 0, 0x12, 0, 0)` and clears
 * both state/counter/table-index trios (`0x31`/`0x2f`/`0x27` and
 * `0x32`/`0x30`/`0x28`).
 *
 * Matched in a later pass (docs/matching/issue-18-0x08014f8c-actor.md,
 * "Later pass: strag2 retry"): `self`/`mode` as real `u8 *`/`u8` parameters fixed the
 * entry home-copy order the old draft got backwards; the `flags` test
 * needs the constant-copy escape below. */
void sub_8015238(struct act *self, u8 mode, s32 flags)
{
    self->unk_26 = 0xc;
    switch (mode) {
    case 3:
    case 4: {
        s32 m = 0x200;
        s32 m2;

        /* The ROM builds 0x200 in r1 and ANDs through a copy in r0, into
         * flags' own r2: the "=r"/"0" escape keeps the copy (m2) apart
         * from m, and the volatile use of m and flags right after the
         * `and` stops combine from sinking it into the test and regmove
         * from retargeting it onto m2. */
        asm("" : "=r"(m2) : "0"(m));
        flags &= m2;
        asm volatile("" : "+r"(flags) : "r"(m));
        if (flags != 0 && (u8)HasTurboRun(gLevelState)) {
            struct act_vtable *mgr;
            struct act_method *off;
            u8 one;
            u8 *p = &self->unk_29;

            one = 1;
            *p = one;
            mgr = self->vt;
            _call_via_r2((u8 *)self + mgr->m20.thisOffset, (void *)4, mgr->m20.fn);
            off = &self->vt->m50;
            _call_via_r3((u8 *)self + off->thisOffset, self->part, (void *)0x18,
                        off->fn);
            {
                u8 idx = 0x1b;

                self->next31 = 0;
                self->flag2F = one;
                self->next27 = idx;
            }
        } else {
            sub_8015460(self);
        }
        break;
    }
    default: {
        u8 zero = 0;

        sub_8015780(self, 0, 0x12, 0, zero);
        self->next31 = zero;
        self->flag2F = 1;
        self->next27 = zero;
        self->next32 = zero;
        self->flag30 = 1;
        self->next28 = zero;
        break;
    }
    }
}

extern void sub_80122CC(void *self);

/* While `self+0x27`/`self+0x2b` are both clear and `mode` is `3`/`4`:
 * sets the state/counter/table-index trio (`0x31`/`0x2f`/`0x27`) to
 * `0`/`1`/`0x17`. Independently, for `mode <= 2`: resets the same trio
 * to `0`/`1`/`0`. Always tail-calls `sub_80122CC`.
 *
 * Matched in a later pass (docs/matching/issue-18-0x08014f8c-actor.md,
 * "Later pass: strag2 retry"): the `0x17`/`0` table indices go through `u8` locals
 * so they're materialized before the stores, which also moves `mode`
 * into `r4` as in the ROM. */
void sub_80152F0(u8 *self, u8 mode)
{
    if (self[0x27] == 0 && self[0x2b] == 0) {
        switch (mode) {
        case 3:
        case 4: {
            u8 idx = 0x17;

            self[0x31] = 0;
            self[0x2f] = 1;
            self[0x27] = idx;
            break;
        }
        }
    }
    if (mode <= 2) {
        u8 idx = 0;

        self[0x31] = idx;
        self[0x2f] = 1;
        self[0x27] = idx;
    }
    sub_80122CC(self);
}
/* Trailing byte count isn't a multiple of 4 - without this, `as` pads
 * with its default NOP fill instead of the ROM's zero fill (see
 * docs/matching.md's alignment-padding gotcha). */
asm(".align 2, 0");
