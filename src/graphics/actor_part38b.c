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
 * `0x32`/`0x30`/`0x28`).
 *
 * Written as NAKED asm, not plain C: every load/store, branch and call
 * was already confirmed correct and in the right order - the residual
 * gap was purely the two parameter home-copies at function entry
 * (`self` into `r5`, `mode`'s `u8` truncation into `r1`), which gcc 2.9
 * always scheduled in the opposite order from the ROM regardless of
 * source order or register pins - see
 * docs/matching/issue-18-0x08014f8c-actor.md, "Parked, not matched:
 * sub_8015238". Transcribed instruction-for-instruction from the ROM
 * disassembly instead, the same escape hatch used for
 * `sub_8001CB8`/`sub_8001DB4` (src/system/link_cable.c). */
NAKED void sub_8015238(void *selfArg, u8 mode, s32 flags)
{
    asm(
        "push {r4, r5, lr}\n\t"
        "sub sp, #4\n\t"
        "add r5, r0, #0\n\t"
        "lsl r1, r1, #0x18\n\t"
        "lsr r1, r1, #0x18\n\t"
        "add r3, r5, #0\n\t"
        "add r3, #0x26\n\t"
        "mov r0, #0xc\n\t"
        "strb r0, [r3]\n\t"
        "cmp r1, #4\n\t"
        "bgt 1f\n\t"
        "cmp r1, #3\n\t"
        "blt 1f\n\t"
        "mov r1, #0x80\n\t"
        "lsl r1, r1, #2\n\t"
        "add r0, r1, #0\n\t"
        "and r2, r0\n\t"
        "cmp r2, #0\n\t"
        "beq 2f\n\t"
        "ldr r0, 20f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_80231C4\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq 2f\n\t"
        "add r0, r5, #0\n\t"
        "add r0, #0x29\n\t"
        "mov r4, #1\n\t"
        "strb r4, [r0]\n\t"
        "ldr r1, [r5, #0xc]\n\t"
        "mov r2, #0x20\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r5, r0\n\t"
        "ldr r2, [r1, #0x24]\n\t"
        "mov r1, #4\n\t"
        "bl sub_803AD80\n\t"
        "ldr r2, [r5, #0xc]\n\t"
        "add r2, #0x50\n\t"
        "mov r1, #0\n\t"
        "ldrsh r0, [r2, r1]\n\t"
        "add r0, r5, r0\n\t"
        "ldr r1, [r5, #0x10]\n\t"
        "ldr r3, [r2, #4]\n\t"
        "mov r2, #0x18\n\t"
        "bl sub_803AD84\n\t"
        "mov r1, #0x1b\n\t"
        "add r2, r5, #0\n\t"
        "add r2, #0x31\n\t"
        "mov r0, #0\n\t"
        "strb r0, [r2]\n\t"
        "add r0, r5, #0\n\t"
        "add r0, #0x2f\n\t"
        "strb r4, [r0]\n\t"
        "sub r0, #8\n\t"
        "strb r1, [r0]\n\t"
        "b 3f\n\t"
        ".align 2, 0\n"
    "20: .4byte gUnknown_030012C0\n"
    "2:\n\t"
        "add r0, r5, #0\n\t"
        "bl sub_8015460\n\t"
        "b 3f\n\t"
    "1:\n\t"
        "mov r4, #0\n\t"
        "str r4, [sp]\n\t"
        "add r0, r5, #0\n\t"
        "mov r1, #0\n\t"
        "mov r2, #0x12\n\t"
        "mov r3, #0\n\t"
        "bl sub_8015780\n\t"
        "add r0, r5, #0\n\t"
        "add r0, #0x31\n\t"
        "strb r4, [r0]\n\t"
        "sub r0, #2\n\t"
        "mov r1, #1\n\t"
        "strb r1, [r0]\n\t"
        "sub r0, #8\n\t"
        "strb r4, [r0]\n\t"
        "add r0, #0xb\n\t"
        "strb r4, [r0]\n\t"
        "sub r0, #2\n\t"
        "strb r1, [r0]\n\t"
        "sub r0, #8\n\t"
        "strb r4, [r0]\n\t"
    "3:\n\t"
        "add sp, #4\n\t"
        "pop {r4, r5}\n\t"
        "pop {r0}\n\t"
        "bx r0"
    );
}

extern void sub_80122CC(void *self);

/* While `self+0x27`/`self+0x2b` are both clear and `mode` is `3`/`4`:
 * sets the state/counter/table-index trio (`0x31`/`0x2f`/`0x27`) to
 * `0`/`1`/`0x17`. Independently, for `mode <= 2`: resets the same trio
 * to `0`/`1`/`0`. Always tail-calls `sub_80122CC`.
 *
 * Written as NAKED asm, not plain C: every load/store, branch and call
 * was already confirmed correct and in the right order - the residual
 * gap was a single instruction (`self+0x31`'s store folding a `+6`
 * byte offset from the already-computed `self+0x2b` pointer into the
 * `strb`'s own addressing mode, where the ROM keeps the `adds #6` and
 * the `strb` as two separate instructions) - see
 * docs/matching/issue-18-0x08014f8c-actor.md, "Parked, not matched:
 * sub_80152F0". Transcribed instruction-for-instruction from the ROM
 * disassembly instead, the same escape hatch used for
 * `sub_8001CB8`/`sub_8001DB4` (src/system/link_cable.c). */
NAKED void sub_80152F0(void *selfArg, u8 mode)
{
    asm(
        "push {r4, lr}\n\t"
        "add r2, r0, #0\n\t"
        "lsl r1, r1, #0x18\n\t"
        "lsr r4, r1, #0x18\n\t"
        "mov r0, #0x27\n\t"
        "add r0, r0, r2\n\t"
        "mov ip, r0\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne 4f\n\t"
        "add r0, r2, #0\n\t"
        "add r0, #0x2b\n\t"
        "ldrb r3, [r0]\n\t"
        "cmp r3, #0\n\t"
        "bne 4f\n\t"
        "cmp r4, #4\n\t"
        "bgt 4f\n\t"
        "cmp r4, #3\n\t"
        "blt 4f\n\t"
        "mov r1, #0x17\n\t"
        "add r0, #6\n\t"
        "strb r3, [r0]\n\t"
        "add r3, r2, #0\n\t"
        "add r3, #0x2f\n\t"
        "mov r0, #1\n\t"
        "strb r0, [r3]\n\t"
        "mov r0, ip\n\t"
        "strb r1, [r0]\n\t"
    "4:\n\t"
        "cmp r4, #2\n\t"
        "bhi 5f\n\t"
        "mov r1, #0\n\t"
        "add r0, r2, #0\n\t"
        "add r0, #0x31\n\t"
        "strb r1, [r0]\n\t"
        "add r3, r2, #0\n\t"
        "add r3, #0x2f\n\t"
        "mov r0, #1\n\t"
        "strb r0, [r3]\n\t"
        "add r0, r2, #0\n\t"
        "add r0, #0x27\n\t"
        "strb r1, [r0]\n\t"
    "5:\n\t"
        "add r0, r2, #0\n\t"
        "bl sub_80122CC\n\t"
        "pop {r4}\n\t"
        "pop {r0}\n\t"
        "bx r0"
    );
}
/* Trailing byte count isn't a multiple of 4 - without this, `as` pads
 * with its default NOP fill instead of the ROM's zero fill (see
 * docs/matching.md's alignment-padding gotcha). */
asm(".align 2, 0");
