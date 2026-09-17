#include "core.h"

/* Register-specific "call through bx" trampolines: each takes the
 * function pointer to invoke already sitting in a specific register -
 * not a normal C argument. Callers rely on that register still holding
 * the right value from whatever expression ran immediately before the
 * call (see irq.c's `sub_8000720`, which calls `sub_803AD78()` right
 * after a `*p != 0` comparison leaves `p`'s value in r0). */
NAKED void sub_803AD78(void)
{
    asm("bx r0\n\tnop");
}

NAKED void sub_803AD7C(void)
{
    asm("bx r1\n\tnop");
}

NAKED void sub_803AD80(void)
{
    asm("bx r2\n\tnop");
}

NAKED void sub_803AD84(void)
{
    asm("bx r3\n\tnop");
}

NAKED void sub_803AD88(void)
{
    asm("bx r4\n\tnop");
}

NAKED void sub_803AD8C(void)
{
    asm("bx r5\n\tnop");
}

NAKED void sub_803AD90(void)
{
    asm("bx r6\n\tnop");
}

/* Continues the same bx-trampoline table for r7-sp; nothing branches
 * directly into these remaining entries so they never got their own
 * labels in the original disassembly. */
NAKED void sub_803AD94(void)
{
    asm(
        "bx r7\n\t"
        "nop\n\t"
        "bx r8\n\t"
        "nop\n\t"
        "bx r9\n\t"
        "nop\n\t"
        "bx sl\n\t"
        "nop\n\t"
        "bx fp\n\t"
        "nop\n\t"
        "bx ip\n\t"
        "nop\n\t"
        "bx sp\n\t"
        "nop"
    );
}

/* Not referenced by anything in this chunk or the neighboring matched
 * code; kept as a plain nullsub like the project's others. Not part of
 * issue #69's listed range (which ends just before this function) but
 * matched alongside it since it's a trivial, contiguous bonus. The
 * trailing `nop` is a real encoded instruction in the ROM (not
 * assembler alignment fill - `asm(".align 2, 0")`'s zero-byte fill
 * doesn't reproduce it), so it's written explicitly. */
NAKED void nullsub_43(void)
{
    asm("bx lr\n\tnop");
}
