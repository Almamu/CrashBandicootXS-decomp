#include "core.h"

/* Continuation of actor_part28.c (issue #18's chunk) - covers
 * `sub_80151C8` (matched); `sub_8015238`/`sub_80152F0` are parked
 * (NON_MATCHING) below, real bytes in the new asm/code_3_2_17_15238.s.
 * Non-adjacent to actor_part28.c since the parked `sub_8015038` sits
 * raw between them (asm/code_3_2_17_15038.s). Same "self" object
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
/* Trailing byte count isn't a multiple of 4 and this is the last
 * actually-emitted function in the file (the rest is NON_MATCHING-
 * guarded, compiling to nothing in the default build) - without this,
 * `as` pads with its default NOP fill instead of the ROM's zero fill
 * (see docs/matching.md's alignment-padding gotcha). */
asm(".align 2, 0");

#if NON_MATCHING
/* NOT YET BYTE-MATCHING - see docs/matching/issue-18-0x08014f8c-actor.md,
 * "Parked, not matched: sub_8015238" for the full account; compiled
 * only under `make NON_MATCHING=1`, the checked-in assembly
 * (asm/code_3_2_17_15238.s) is used otherwise. Every load/store, branch
 * and call is confirmed correct, in the right order, and in the right
 * registers - the residual gap is purely the two parameter home-copies
 * at function entry (`self` into `r5`, `mode`'s `u8` truncation into
 * `r1`): this compiler always schedules `mode`'s `lsls`/`lsrs`
 * truncation before `self`'s `adds r5,r0,#0` copy, regardless of C
 * statement order, which one is `register`-pinned, or hand-writing the
 * copies as raw `asm volatile` in the desired order (the compiler's own
 * fixed parameter-home-copy prologue pass still ran first and either
 * duplicated the truncation or reintroduced the swap) - the ROM copies
 * `self` first instead. */
extern void *gUnknown_030012C0;
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern s32 sub_803AD84(void *arg0, void *arg1, void *arg2, void *arg3);
extern void sub_8015460(void *selfArg);
extern void sub_8015780(void *selfArg, s32 a, s32 b, s32 c, s32 d);
extern s32 sub_80231C4(void *self);

/* Always sets `self+0x26 = 0xc`. For `mode` `3`/`4`: if `flags` bit
 * `0x200` is set and `sub_80231C4(gUnknown_030012C0)` is true, latches
 * `self+0x29`, fires the mgr trampoline pair with actions `4`/`0x18`,
 * and sets the state/counter/table-index trio (`0x31`/`0x2f`/`0x27`) to
 * `0`/`1`/`0x1b` - otherwise falls back to `sub_8015460`. For every
 * other `mode`: resets via `sub_8015780(self, 0, 0x12, 0, 0)` and clears
 * both state/counter/table-index trios (`0x31`/`0x2f`/`0x27` and
 * `0x32`/`0x30`/`0x28`). */
void sub_8015238(void *selfArg, u8 mode, s32 flags)
{
    u8 *self = selfArg;
    s32 m = mode;

    self[0x26] = 0xc;

    switch (m) {
    case 3:
    case 4:
        break;
    default:
        goto fallback;
    }
    {
        register s32 mask asm("r1") = 0x200;
        register s32 maskCopy asm("r0");
        register s32 result asm("r2") = flags;

        asm volatile("mov %0, %1" : "=r"(maskCopy) : "r"(mask));
        result &= maskCopy;

        if (result != 0 && (u8)sub_80231C4(gUnknown_030012C0) != 0) {
            u8 *mgr;
            u8 *off;

            self[0x29] = 1;

            mgr = *(u8 **)(self + 0xc);
            sub_803AD80(self + *(s16 *)(mgr + 0x20), (void *)4, *(void **)(mgr + 0x24));
            off = *(u8 **)(self + 0xc) + 0x50;
            sub_803AD84(self + *(s16 *)off, *(void **)(self + 0x10), (void *)0x18,
                        *(void **)(off + 4));

            self[0x31] = 0;
            self[0x2f] = 1;
            self[0x27] = 0x1b;
            return;
        }

        sub_8015460(self);
        return;
    }

fallback:
    sub_8015780(self, 0, 0x12, 0, 0);
    self[0x31] = 0;
    self[0x2f] = 1;
    self[0x27] = 0;
    self[0x32] = 0;
    self[0x30] = 1;
    self[0x28] = 0;
}

/* NOT YET BYTE-MATCHING - see docs/matching/issue-18-0x08014f8c-actor.md,
 * "Parked, not matched: sub_80152F0" for the full account; compiled
 * only under `make NON_MATCHING=1`, the checked-in assembly
 * (asm/code_3_2_17_15238.s) is used otherwise. Every load/store, branch
 * and call is confirmed correct and in the right order; the residual
 * gap is a single instruction (`self+0x31`'s store folds a `+6` byte
 * offset from the already-computed `self+0x2b` pointer into the
 * `strb`'s own addressing mode, where the ROM keeps the `adds #6` and
 * the `strb` as two separate instructions) - tried explicit pointer
 * locals, `asm volatile` anti-fold barriers on the pointer itself, and
 * reordering; each either left the fold in place or introduced a new
 * spilled register the ROM doesn't have. */
extern void sub_80122CC(void *self);

/* While `self+0x27`/`self+0x2b` are both clear and `mode` is `3`/`4`:
 * sets the state/counter/table-index trio (`0x31`/`0x2f`/`0x27`) to
 * `0`/`1`/`0x17`. Independently, for `mode <= 2`: resets the same trio
 * to `0`/`1`/`0`. Always tail-calls `sub_80122CC`. */
void sub_80152F0(void *selfArg, u8 mode)
{
    register u8 *self asm("r2") = selfArg;
    register s32 m asm("r4") = mode;
    register u8 *p27 asm("ip") = self + 0x27;

    if (*p27 == 0) {
        u8 *p2b = self + 0x2b;
        u8 old = *p2b;

        if (old == 0) {
            switch (m) {
            case 3:
            case 4:
            {
                u8 *self2 = self;
                u8 val = 0x17;

                p2b += 6;
                *p2b = old; /* self[0x31] */
                asm volatile("" : "+r"(self2));
                self2[0x2f] = 1;
                *p27 = val;
                break;
            }
            }
        }
    }

    if (m <= 2) {
        self[0x31] = 0;
        self[0x2f] = 1;
        self[0x27] = 0;
    }

    sub_80122CC(self);
}
#endif /* NON_MATCHING */
