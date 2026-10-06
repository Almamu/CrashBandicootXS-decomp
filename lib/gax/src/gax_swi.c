#include "gax_internal.h"
#include "match.h"

/* HuffUnComp (SWI 0x13) wrapper, GAX2-internal - preserves r0/r1 across
 * the call via r7/r8 plus a dead `sub sp, #8`/stack-store of both args
 * that nothing reads back.
 *
 * Note the ROM saves r8 but *not* r7, even though r7 is clobbered: that
 * is exactly agbcc's known bug of dropping an explicitly pinned r7 from
 * the push/pop list (the reason this project never pins r7 by choice).
 * Here the bug is the evidence: the original source pinned `src` to r7
 * and `dst` to r8, and reproducing it needs that same r7 pin. The
 * empty `"m"` asm stands in for whatever forced both args into stack
 * slots (the dead stores), and the final pinned r0/r1 inputs reproduce
 * the ROM's `add r0, r7, #0; mov r1, r8` restore after the SWI. */
void GaxHuffUnComp(void *src, void *dst)
{
    MATCH_HOLD_REG(void *, savedSrc, r7) = src;
    MATCH_HOLD_REG(void *, savedDst, r8) = dst;

    asm volatile("" : : "m"(src), "m"(dst));
    asm volatile("swi 0x13" : : : "r0", "r1");
    {
        MATCH_HOLD_REG(void *, r0, r0) = savedSrc;
        MATCH_HOLD_REG(void *, r1, r1) = savedDst;
        asm volatile("" : : "r"(r0), "r"(r1));
    }
}
