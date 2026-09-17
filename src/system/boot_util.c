#include "core.h"

/* Sits right after the permanent hand-written `start`/`init_vector`
 * boot stub in asm/crt0.s (never decompiled - it's the CPU-mode/stack
 * setup and BIOS RegisterRamReset call every GBA ROM needs before
 * `AgbMain` in src/system/main.c can run) and before main.c itself. */

/* BIOS `Div` (SWI 6) wrapper exposing both the quotient (return value)
 * and the remainder (via `remainderOut`). Written with inline asm
 * (rather than a plain `register`-pinned call) because the ROM saves
 * `remainderOut` across the SWI with a bare `push {r2}`/`pop {r2}`
 * pair - marking r2 as clobbered on a plain call makes the compiler
 * spill it through a callee-saved register (r4) with a normal
 * push/pop-list prologue instead, which is a real but differently-
 * shaped save. */
s32 sub_8000140(s32 number, s32 denom, s32 *remainderOut)
{
    register s32 quotient asm("r0") = number;
    register s32 remainder asm("r1") = denom;
    register s32 *outPtr asm("r2") = remainderOut;

    asm volatile(
        "push {r2}\n\t"
        "svc #6\n\t"
        "pop {r2}"
        : "+r"(quotient), "+r"(remainder), "+r"(outPtr)
    );
    *outPtr = remainder;
    return quotient;
}

extern void sub_803A94C(const void *src, void *dst, u32 cnt);

/* `sub_803A94C` (the BIOS `CpuSet` wrapper) with swapped src/dst
 * argument order and `byteCount` converted to CpuSet's 32-bit-word
 * count field: masked to the low 23 bits, then divided by 4 (the
 * `<<9`/`>>11` pair nets exactly that), with the 32-bit-transfer flag
 * (`0x04000000`) set. Returns `dst`. */
void *sub_800014C(void *dst, const void *src, u32 byteCount)
{
    sub_803A94C(src, dst, ((byteCount << 9) >> 11) | 0x04000000);
    return dst;
}

void nullsub_9(void)
{
}
asm(".align 2, 0");
