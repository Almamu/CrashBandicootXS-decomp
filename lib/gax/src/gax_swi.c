#include "gax_internal.h"
#include "match.h"

/* HuffUnComp (SWI 0x13) wrapper, GAX2-internal: keeps `src`/`dst` in
 * r7/r8 across the SWI and hands them back in r0/r1. Shin'en's source
 * (docs/libraries.md, "GAX implementation notes"): register variables
 * pinned to r7 and r8 and an inline `swi`. The ROM saves r8 but not r7,
 * which it clobbers: agbcc drops a register variable pinned to r7 from
 * the push list, so the r7 pin is the original's, bug included.
 *
 * The two MATCH_USE_MEMs are ours: the ROM also stores both arguments to
 * a stack frame nothing reads (`sub sp, #8; str r0, [sp]; str r1,
 * [sp, #4]`), and a memory use of each is what makes gcc do that. What
 * the original had there is unknown. */
void GaxHuffUnComp(void *src, void *dst)
{
    register void *savedSrc asm("r7") = src;
    register void *savedDst asm("r8") = dst;

    MATCH_USE_MEM(src);
    MATCH_USE_MEM(dst);
    // clang-format off
    __asm__ volatile("swi 0x13\n\tmov r0, %0\n\tmov r1, %1"
                     : : "r"(savedSrc), "r"(savedDst) : "r0", "r1");
    // clang-format on
}
