#include "core.h"

/* HuffUnComp (SWI 0x13) wrapper, GAX2-internal - preserves r0/r1 across
 * the call (via r7/r8, plus dead `sub sp, #8`/stack-store traffic that
 * nothing ever reads back) instead of just falling straight through
 * like this project's other bare SWI wrappers (see
 * src/system/timer_util.c). That unused stack traffic reads like a
 * hand-written asm stub rather than compiler-generated C, so this is
 * transcribed directly as NAKED asm rather than guessed-at C. */
NAKED void sub_80392C4(void)
{
    asm(
        "mov r3, r8\n\t"
        "push {r3}\n\t"
        "sub sp, #8\n\t"
        "str r0, [sp]\n\t"
        "str r1, [sp, #4]\n\t"
        "add r7, r0, #0\n\t"
        "mov r8, r1\n\t"
        "svc #0x13\n\t"
        "add r0, r7, #0\n\t"
        "mov r1, r8\n\t"
        "add sp, #8\n\t"
        "pop {r3}\n\t"
        "mov r8, r3\n\t"
        "bx lr"
    );
}
