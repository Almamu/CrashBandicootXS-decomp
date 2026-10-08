#include "ctrl.hpp"

extern "C" {
#include "match.h"
#include <agb_syscall.h>
#include "system.h"
}

/* Sits right after the permanent hand-written `start`/`init_vector`
 * boot stub in asm/crt0.s (never decompiled - it's the CPU-mode/stack
 * setup and BIOS RegisterRamReset call every GBA ROM needs before
 * `AgbMain` in src/system/main.cpp can run) and before main.cpp itself. */

/* BIOS `Div` (SWI 6) wrapper exposing both the quotient (return value)
 * and the remainder (via `remainderOut`). Written with inline asm
 * (rather than a plain `register`-pinned call) because the ROM saves
 * `remainderOut` across the SWI with a bare `push {r2}`/`pop {r2}`
 * pair - marking r2 as clobbered on a plain call makes the compiler
 * spill it through a callee-saved register (r4) with a normal
 * push/pop-list prologue instead, which is a real but differently-
 * shaped save. */
s32 DivMod(s32 number, s32 denom, s32 *remainderOut)
{
    MATCH_HOLD_REG(s32, quotient, r0) = number;
    MATCH_HOLD_REG(s32, remainder, r1) = denom;
    MATCH_HOLD_REG(s32 *, outPtr, r2) = remainderOut;

    // clang-format off
    asm volatile(
        "push {r2}\n\t"
        "svc #6\n\t"
        "pop {r2}"
        : "+r"(quotient), "+r"(remainder), "+r"(outPtr)
    );
    // clang-format on
    *outPtr = remainder;
    return quotient;
}

/* `CpuSet` (the BIOS SWI wrapper) with swapped src/dst
 * argument order and `byteCount` converted to CpuSet's 32-bit-word
 * count field: masked to the low 23 bits, then divided by 4 (the
 * `<<9`/`>>11` pair nets exactly that), with the 32-bit-transfer flag
 * (`CPU_SET_32BIT`) set. Returns `dst`. This is the ROM's `memcpy`
 * (the GAX code aliases the compiler's `memcpy` calls to it), but only
 * for word-aligned, whole-word copies, so it doesn't take the libc name
 * (which agbcc would also treat as its builtin). */
void *MemCopy32(void *dst, const void *src, u32 byteCount)
{
    CpuSet(src, dst, ((byteCount << 9) >> 11) | CPU_SET_32BIT);
    return dst;
}

/* The controllers' base Update (gCtrlVtable's slot 1, UpdateCtrl): does
 * nothing. It's Ctrl's key method, its first virtual one (include/ctrl.hpp),
 * so g++ emits gCtrlVtable here (ldscript.txt places it); Ctrl's other
 * methods are in src/objects/ctrl.cpp. Built as C++ (agbcp) for that
 * since #664 step 10b; DivMod and MemCopy32 compile the same. */
void Ctrl::Update(MovingSprite *part)
{
}
