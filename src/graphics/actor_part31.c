#include "core.h"

/* Same "self" object family as actor_part28.c - see that file's header
 * comment and docs/matching/issue-62-0x08033804-actor.md. */

/* Looks up `self`'s current state in the stride-8 table
 * `gStaticData_0817C4E0` (`{s16 baseOff; s16 count; s16 subOffset}`,
 * the exact same shape as `sub_802C208`'s `gStaticData_0817A6B8` - see
 * docs/status/actor.md), resolves the trampoline address/function the
 * same way, calls `sub_803AD84`, and finally fires `sub_802A7B8(self)`
 * unless `self` is in state 2 with `self+0x12` set.
 *
 * Transcribed as NAKED asm for the same reason as `sub_802C208`
 * (src/graphics/actor_part19e.c): the ROM keeps `gStaticData_
 * 0817C4E0`'s base address alive in `r7` for the whole function, and
 * this compiler's r7 hazard (see `sub_802C208`'s comment and
 * docs/matching.md's `sub_8007DBC`/`sub_8006600` entries) makes that
 * unreachable from plain C. */
NAKED void sub_8033B44(void *selfArg)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add r4, r0, #0\n\t"
        "ldr r1, 1f\n\t"
        "ldr r0, [r4, #0x28]\n\t"
        "lsl r3, r0, #3\n\t"
        "add r0, r3, r1\n\t"
        "mov r7, #2\n\t"
        "ldrsh r2, [r0, r7]\n\t"
        "add r7, r1, #0\n\t"
        "cmp r2, #0\n\t"
        "ble 2f\n\t"
        "mov r1, #4\n\t"
        "ldrsh r0, [r0, r1]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r0]\n\t"
        "lsl r0, r2, #3\n\t"
        "add r0, r0, r1\n\t"
        "sub r0, #8\n\t"
        "ldr r5, [r0]\n\t"
        "ldr r6, [r0, #4]\n\t"
        "add r3, r6, #0\n\t"
        "b 3f\n\t"
        ".align 2, 0\n"
    "1: .4byte gStaticData_0817C4E0\n"
    "2:\n\t"
        "add r0, r7, #4\n\t"
        "add r0, r3, r0\n\t"
        "ldr r3, [r0]\n\t"
    "3:\n\t"
        "ldr r0, [r4, #0x28]\n\t"
        "lsl r0, r0, #3\n\t"
        "add r0, r0, r7\n\t"
        "mov r7, #0\n\t"
        "ldrsh r1, [r0, r7]\n\t"
        "cmp r2, #0\n\t"
        "ble 4f\n\t"
        "lsl r0, r5, #0x10\n\t"
        "asr r0, r0, #0x10\n\t"
        "add r0, r0, r1\n\t"
        "b 5f\n\t"
    "4:\n\t"
        "add r0, r1, #0\n\t"
    "5:\n\t"
        "add r0, r4, r0\n\t"
        "bl sub_803AD84\n\t"
        "ldr r0, [r4, #0x28]\n\t"
        "mov r1, #1\n\t"
        "cmp r0, #2\n\t"
        "bne 6f\n\t"
        "ldrb r0, [r4, #0x12]\n\t"
        "cmp r0, #0\n\t"
        "beq 6f\n\t"
        "mov r1, #0\n\t"
    "6:\n\t"
        "cmp r1, #0\n\t"
        "beq 7f\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_802A7B8\n\t"
    "7:\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
    );
}
