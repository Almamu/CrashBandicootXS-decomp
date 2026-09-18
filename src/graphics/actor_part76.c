#include "core.h"

/* Sits right after actor_part60.c's `sub_802DFDC` and before
 * actor_part61.c's `nullsub_27` - the whole contiguous range that used
 * to be `asm/code_3_2_20_28568_c99c_e058.s`. */

/* A parameterized twin of `sub_802DE70`'s (actor_part75.c) 16x16
 * triangular-fill dot-pattern loop, taking the destination buffer
 * (`dst`) and seed byte (`seed`) as real parameters instead of the
 * fixed stack buffer/`0`-or-`0x80` seed constants `sub_802DE70` uses for
 * its own two inline copies of this same loop. No known caller anywhere
 * in the matched portion of this ROM region (`sub_802DE70` always
 * inlines the loop itself rather than calling this) - kept byte-exact
 * regardless, per this project's standing convention for functions
 * without a confirmed call site.
 *
 * Written as NAKED asm for the same register-pressure reasons as
 * `sub_802DE70` - mechanical, byte-verified transcription, not an
 * inferred control-flow guess. */
NAKED void sub_802E058(u8 *dst, u8 seed)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov ip, r0\n\t"
        "lsl r1, r1, #0x18\n\t"
        "lsr r5, r1, #0x18\n\t"
        "mov r4, #0\n\t"
        "mov r7, #0xff\n\t"
        "1:\n\t"
        "mov r3, #0\n\t"
        "lsl r0, r4, #4\n\t"
        "add r6, r4, #1\n\t"
        "mov r1, ip\n\t"
        "add r2, r0, r1\n\t"
        "2:\n\t"
        "sub r0, r3, #3\n\t"
        "cmp r0, #9\n\t"
        "bhi 3f\n\t"
        "cmp r4, #2\n\t"
        "ble 3f\n\t"
        "cmp r4, #0xc\n\t"
        "ble 4f\n\t"
        "3:\n\t"
        "strb r7, [r2]\n\t"
        "b 5f\n\t"
        "4:\n\t"
        "add r1, r5, #0\n\t"
        "add r0, r1, #1\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r5, r0, #0x18\n\t"
        "strb r1, [r2]\n\t"
        "5:\n\t"
        "add r2, #1\n\t"
        "add r3, #1\n\t"
        "cmp r3, #0xf\n\t"
        "ble 2b\n\t"
        "add r4, r6, #0\n\t"
        "cmp r4, #0xf\n\t"
        "ble 1b\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    );
}

asm(".align 2, 0");
