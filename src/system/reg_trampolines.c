#include "core.h"

/* libgcc's `_call_via_rN` trampolines (lib1funcs.asm's `call_via`
 * table), which Thumb code uses for indirect calls (`bl _call_via_rN`):
 * each takes the function pointer to invoke already sitting in a
 * specific register - not a normal C argument. C code that calls them
 * by name relies on that register still holding the right value from
 * whatever expression ran immediately before the call (see irq.c's
 * `VBlankHandler`, which calls `_call_via_r0()` right after a
 * `*p != 0` comparison leaves `*p` in r0). */
NAKED void _call_via_r0(void)
{
    asm("bx r0\n\tnop");
}

NAKED void _call_via_r1(void)
{
    asm("bx r1\n\tnop");
}

NAKED void _call_via_r2(void)
{
    asm("bx r2\n\tnop");
}

NAKED void _call_via_r3(void)
{
    asm("bx r3\n\tnop");
}

NAKED void _call_via_r4(void)
{
    asm("bx r4\n\tnop");
}

NAKED void _call_via_r5(void)
{
    asm("bx r5\n\tnop");
}

NAKED void _call_via_r6(void)
{
    asm("bx r6\n\tnop");
}

/* `_call_via_r7`, followed by the r8-sp entries of the same table;
 * nothing branches directly into those, so they never got their own
 * labels in the original disassembly. */
NAKED void _call_via_r7(void)
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

/* `_call_via_lr`, the last entry of lib1funcs.asm's `call_via` table
 * (`bx lr; nop`). No caller in the ROM (Thumb code never calls through
 * lr), so it was once kept as a nullsub. Not part of issue #69's listed
 * range (which ends just before this function) but matched alongside it
 * since it's a trivial, contiguous bonus. The trailing `nop` is a real
 * encoded instruction in the ROM (not
 * assembler alignment fill - `asm(".align 2, 0")`'s zero-byte fill
 * doesn't reproduce it), so it's written explicitly. */
NAKED void _call_via_lr(void)
{
    asm("bx lr\n\tnop");
}
