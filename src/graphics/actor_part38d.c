#include "core.h"

/* Continuation of actor_part28c.c (issue #18's chunk, the last one) -
 * covers `nullsub_17` through `sub_8015780` (matched) and `sub_80157C4`
 * (parked, NON_MATCHING); non-adjacent to actor_part28c.c since the
 * parked `sub_80156EC` sits raw between them (asm/code_3_2_17_156ec.s).
 * Same "self" object family documented at the top of actor_part18.c/
 * actor_part28.c. */

extern void sub_8015460(void *selfArg);
extern void sub_8012FBC(void *self);
extern void sub_8012D24(void *self);
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern s32 sub_803AD84(void *arg0, void *arg1, void *arg2, void *arg3);

void nullsub_17(void)
{
}

/* While `self+0x29` is clear: tail-calls `sub_8015460` first. Always
 * tail-calls `sub_8012FBC` afterward. */
void sub_8015750(void *selfArg)
{
    register u8 *self asm("r4") = selfArg;

    if (self[0x29] == 0) {
        sub_8015460(self);
    }
    sub_8012FBC(self);
}

void nullsub_18(void)
{
}

/* Trivial tail-call. */
void sub_8015774(void *selfArg)
{
    sub_8012D24(selfArg);
}

/* Fires the mgr trampoline pair with `a`/`b` as the two action
 * arguments, then conditionally latches `self+0x18`/`self+0x1c` from
 * `c`/`d` unless either is the `0x7FFFFFFF` sentinel. */
void sub_8015780(void *selfArg, s32 a, s32 b, s32 c, s32 d)
{
    u8 *self = selfArg;
    u8 *mgr = *(u8 **)(self + 0xc);
    u8 *off;

    sub_803AD80(self + *(s16 *)(mgr + 0x20), (void *)a, *(void **)(mgr + 0x24));
    off = *(u8 **)(self + 0xc) + 0x50;
    sub_803AD84(self + *(s16 *)off, *(void **)(self + 0x10), (void *)b, *(void **)(off + 4));

    if (c != 0x7FFFFFFF) {
        *(s32 *)(self + 0x18) = c;
    }
    if (d != 0x7FFFFFFF) {
        *(s32 *)(self + 0x1c) = d;
    }
}


extern void *gUnknown_030012BC;
extern void *gUnknown_030012D8;
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);
extern void sub_80019A8(void *self, u32 id);
extern u8 sub_800B86C(void *unused, void *partArg, s32 newVal);

/* If the player's `+0x100` flag is set: picks a replacement `mode` for
 * a handful of special values (`0x12` when the player's `+0x60` is
 * nonzero -> `0x25`; `0xd`/`0x18` -> `0x26`, both playing a fixed cue
 * via `sub_80019A8`/`PlaySfx`) and otherwise just re-arms the cue via
 * `sub_80019A8` with the original `mode`. Always tail-calls
 * `sub_800B86C(arg0, arg1, mode)`.
 *
 * Written as NAKED asm, not plain C: every load/store, branch and call
 * was already confirmed correct - the residual gap was register
 * allocation across the 3-way `mode` dispatch (gcc 2.9 wanted `r8` for
 * the running "unused" register the ROM keeps `mode` copies in
 * instead, and merged the `0xd`/`0x18` case pair into two sequential
 * compares sharing a target rather than the ROM's own `cmp/bgt/cmp/beq`
 * triangle) - see docs/matching/issue-18-0x08014f8c-actor.md, "Parked,
 * not matched: sub_80157C4". Transcribed instruction-for-instruction
 * from the ROM disassembly instead, the same escape hatch used for
 * `sub_8001CB8`/`sub_8001DB4` (src/system/link_cable.c). */
NAKED void sub_80157C4(void *arg0, void *arg1, s32 mode)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add r6, r0, #0\n\t"
        "add r7, r1, #0\n\t"
        "add r5, r2, #0\n\t"
        "ldr r0, 20f\n\t"
        "ldr r1, [r0]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "add r0, r1, r2\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 1f\n\t"
        "cmp r5, #0x12\n\t"
        "beq 3f\n\t"
        "cmp r5, #0x12\n\t"
        "bgt 2f\n\t"
        "cmp r5, #0xd\n\t"
        "beq 4f\n\t"
        "b 6f\n\t"
        ".align 2, 0\n"
    "20: .4byte gUnknown_030012D8\n"
    "2:\n\t"
        "cmp r5, #0x18\n\t"
        "beq 4f\n\t"
        "b 6f\n\t"
    "3:\n\t"
        "ldr r0, [r1, #0x60]\n\t"
        "cmp r0, #0\n\t"
        "beq 1f\n\t"
        "mov r5, #0x25\n\t"
        "b 5f\n\t"
    "4:\n\t"
        "mov r5, #0x26\n\t"
    "5:\n\t"
        "ldr r4, 21f\n\t"
        "ldr r0, [r4]\n\t"
        "mov r1, #0x36\n\t"
        "bl sub_80019A8\n\t"
        "ldr r0, [r4]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "mov r1, #0x36\n\t"
        "bl PlaySfx\n\t"
        "b 1f\n\t"
        ".align 2, 0\n"
    "21: .4byte gUnknown_030012BC\n"
    "6:\n\t"
        "ldr r0, 22f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #0x36\n\t"
        "bl sub_80019A8\n\t"
    "1:\n\t"
        "add r0, r6, #0\n\t"
        "add r1, r7, #0\n\t"
        "add r2, r5, #0\n\t"
        "bl sub_800B86C\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
    "22: .4byte gUnknown_030012BC\n"
    );
}
