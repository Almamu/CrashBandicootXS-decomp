#include "core.h"

extern s32 sub_803AD80(void *arg0, void *arg1, void *fn);

/* Resets a pool manager to empty: tears down every active object
 * (`slotArray[0..activeCount)`, firing each one's `table+0x50/0x54`
 * trampoline via `sub_803AD80` with constant arg `3` if non-`NULL`,
 * then clearing the slot), resets `activeCount` to 0, and rebuilds
 * both the grid (`gridHead`/`gridTail` zeroed) and the free list from
 * scratch over `nodeArray` - the exact same free-list-build loop
 * `sub_8008F20` (`actor_part11.c`) performs during initialization; see
 * that function's own struct/field writeup, identical here.
 *
 * PARKED AS NAKED: the active-object teardown loop (the first half) is
 * confirmed correct on its own, but the free-list-rebuild loop (the
 * second half) is a byte-for-byte copy of `sub_8008F20`'s own tail and
 * hits the exact same many-register allocation gap documented there -
 * the ROM keeps three persistent high registers (`sb`/`sl`/`r8`) alive
 * across the whole loop, no C-level reconstruction tried reproduces
 * that combination. Hand-transcribed instruction-for-instruction from
 * the ROM disassembly instead, same technique and suffix-less Thumb
 * mnemonics as `sub_8008F20`. Kept in its own translation unit (not
 * appended to `actor_part11.c`) since its real ROM address,
 * 0x08009914, sits between `sub_8009868` (`actor_part11d.c`) and
 * `sub_80099F0` (`actor_part12.c`) in ROM order, far from
 * `actor_part11.c`'s own functions - see docs/workflow.md step 4's
 * "needs its own new .c file" case, same as `sub_80096C0`
 * (`actor_part11e.c`). See docs/matching.md, "Parked, not matched:
 * `sub_8009914`". */
NAKED void sub_8009914(void *manager)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "add r4, r0, #0\n\t"
        "mov r6, #0\n\t"
        "b 3f\n\t"
    "1:\n\t"
        "ldr r0, [r4, #8]\n\t"
        "lsl r5, r6, #2\n\t"
        "add r0, r5, r0\n\t"
        "ldr r2, [r0]\n\t"
        "cmp r2, #0\n\t"
        "beq 2f\n\t"
        "ldr r1, [r2, #0x18]\n\t"
        "add r1, #0x50\n\t"
        "mov r3, #0\n\t"
        "ldrsh r0, [r1, r3]\n\t"
        "add r0, r2, r0\n\t"
        "ldr r2, [r1, #4]\n\t"
        "mov r1, #3\n\t"
        "bl sub_803AD80\n\t"
    "2:\n\t"
        "ldr r0, [r4, #8]\n\t"
        "add r0, r5, r0\n\t"
        "mov r1, #0\n\t"
        "str r1, [r0]\n\t"
        "add r6, #1\n\t"
    "3:\n\t"
        "ldr r0, [r4]\n\t"
        "cmp r6, r0\n\t"
        "blt 1b\n\t"
        "mov r0, #0\n\t"
        "str r0, [r4]\n\t"
        "ldr r3, [r4, #4]\n\t"
        "mov r0, #0x81\n\t"
        "lsl r0, r0, #4\n\t"
        "add r0, r0, r4\n\t"
        "mov sb, r0\n\t"
        "ldr r1, 6f\n\t"
        "add r1, r1, r4\n\t"
        "mov sl, r1\n\t"
        "mov r0, #0\n\t"
        "mov r1, #0x82\n\t"
        "lsl r1, r1, #3\n\t"
        "add r2, r4, r1\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x10\n\t"
        "mov r5, #0xff\n\t"
    "4:\n\t"
        "stm r1!, {r0}\n\t"
        "stm r2!, {r0}\n\t"
        "sub r5, #1\n\t"
        "cmp r5, #0\n\t"
        "bge 4b\n\t"
        "mov r5, #0\n\t"
        "cmp r5, r3\n\t"
        "bge 9f\n\t"
        "mov ip, sb\n\t"
        "mov r7, #0\n\t"
        "mov r2, #8\n\t"
        "mov r8, r2\n\t"
        "mov r6, #0\n\t"
    "5:\n\t"
        "mov r3, ip\n\t"
        "ldr r1, [r3]\n\t"
        "lsl r2, r5, #3\n\t"
        "add r1, r2, r1\n\t"
        "ldr r0, [r4, #0xc]\n\t"
        "add r0, r0, r6\n\t"
        "str r0, [r1]\n\t"
        "ldr r0, [r4, #0xc]\n\t"
        "add r0, r6, r0\n\t"
        "str r7, [r0]\n\t"
        "str r7, [r0, #4]\n\t"
        "str r7, [r0, #0xc]\n\t"
        "strb r7, [r0, #0x10]\n\t"
        "ldr r0, [r4, #0xc]\n\t"
        "add r0, r6, r0\n\t"
        "ldr r3, [r3]\n\t"
        "add r1, r3, r2\n\t"
        "str r1, [r0, #8]\n\t"
        "ldr r0, [r4, #4]\n\t"
        "sub r0, #1\n\t"
        "cmp r5, r0\n\t"
        "bne 7f\n\t"
        "str r7, [r1, #4]\n\t"
        "b 8f\n\t"
        ".align 2, 0\n"
    "6: .4byte 0x00000814\n"
    "7:\n\t"
        "mov r2, r8\n\t"
        "add r0, r3, r2\n\t"
        "str r0, [r1, #4]\n\t"
    "8:\n\t"
        "mov r3, #8\n\t"
        "add r8, r3\n\t"
        "add r6, #0x14\n\t"
        "add r5, #1\n\t"
        "ldr r0, [r4, #4]\n\t"
        "cmp r5, r0\n\t"
        "blt 5b\n\t"
    "9:\n\t"
        "mov r1, sb\n\t"
        "ldr r0, [r1]\n\t"
        "mov r2, sl\n\t"
        "str r0, [r2]\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
    );
}
asm(".align 2, 0");
