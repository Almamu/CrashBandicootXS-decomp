#include "gax_internal.h"

/* HuffUnComp (SWI 0x13) wrapper, GAX2-internal: keeps `src`/`dst` in
 * r7/r8 across the SWI and hands them back in r0/r1. Shin'en's source
 * (docs/libraries.md, "GAX implementation notes"): register variables
 * pinned to r7 and r8 and an inline `swi`. The ROM saves r8 but not r7,
 * which it clobbers: agbcc drops a register variable pinned to r7 from
 * the push list, so the r7 pin is the original's, bug included.
 *
 * The ROM also stores both arguments to a stack frame nothing reads
 * (`sub sp, #8; str r0, [sp]; str r1, [sp, #4]`). That comes from the
 * asm's `"m"(src)` and `"m"(dst)` inputs, which its text doesn't use.
 * The nearest precedent is GAX_CALL_ARM's `"m"(argp)` (gax_internal.h),
 * which also leaves its argument in a stack slot. The alternatives all
 * miss: `&src`/`&dst` as asm inputs leave one `add r2, sp, #4` too many,
 * `volatile` parameters reload r7/r8 from the stack, and an inline
 * helper or macro taking `&src`/`&dst` folds `*&x` back, losing the
 * stores. The operands are treated as Shin'en's original source (owner
 * decision, #662). */
void GaxHuffUnComp(void *src, void *dst)
{
    register void *savedSrc asm("r7") = src;
    register void *savedDst asm("r8") = dst;

    // clang-format off
    __asm__ volatile("swi 0x13\n\tmov r0, %0\n\tmov r1, %1"
                     : : "r"(savedSrc), "r"(savedDst), "m"(src), "m"(dst)
                     : "r0", "r1");
    // clang-format on
}
