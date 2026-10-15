/* BIOS helpers (DivMod, MemCopy32), plus Ctrl's empty key method Ctrl::Update. */

#include "game/entities/ctrl.hpp"

extern "C" {
#include <agb_syscall.h>
#include "engine/sys/system.h"
}

using Game::Entities::Ctrl;
using Game::Entities::MovingSprite;

/* Sits right after the permanent hand-written `start`/`init_vector`
 * boot stub in platform/gba/asm/crt0.s (never decompiled - it's the CPU-mode/stack
 * setup and BIOS RegisterRamReset call every GBA ROM needs before
 * `AgbMain` in engine/src/engine/sys/main.cpp can run) and before main.cpp itself. */

#ifndef GAME_N2
/* N-Tranced has no DivMod: its ROM starts this object with MemCopy32
 * (0x08000224). */
/* BIOS `Div` (SWI 6) wrapper exposing both the quotient (return value)
 * and the remainder (via `remainderOut`). The SWI takes the dividend and
 * divisor in r0/r1 and returns the quotient in r0, the remainder in r1
 * and |quotient| in r3: `quotient`/`remainder` are bound to r0/r1 for
 * the asm, and r3 is clobbered. `remainderOut` stays in r2, where it
 * arrives, and the asm text saves it across the SWI itself with a bare
 * `push {r2}`/`pop {r2}`. gcc never saves a register that way (an "r2"
 * clobber gets a copy in callee-saved r4 and a push/pop-list prologue
 * instead), so the pair is in the asm, as the ROM's source had it. r2
 * needs no binding: nothing else wants it.
 *
 * The register variables are the SWI's operand binding, the way an
 * inline SWI wrapper is written for gcc (ARM has no per-register asm
 * constraints), not a nudge of gcc's register choice: like
 * GaxHuffUnComp's (lib/gax/src/gax_swi.c), they and the asm count as the
 * original source (tools/match_idioms.py's ORIGINAL_SOURCE; #662). */
s32 DivMod(s32 number, s32 denom, s32 *remainderOut)
{
    register s32 quotient asm("r0") = number;
    register s32 remainder asm("r1") = denom;

    // clang-format off
    asm volatile("push {r2}\n\tsvc #6\n\tpop {r2}"
                 : "+r"(quotient), "+r"(remainder) : : "r3");
    // clang-format on
    *remainderOut = remainder;
    return quotient;
}
#endif

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
 * nothing. It's Ctrl's key method, its first virtual one (engine/include/game/entities/ctrl.hpp),
 * so g++ emits gCtrlVtable here (games/xs/ldscript.txt places it); Ctrl's other
 * methods are in engine/src/game/entities/ctrl.cpp. Built as C++ (agbcp) for that
 * since #664 step 10b; DivMod and MemCopy32 compile the same. */
void Ctrl::Update(MovingSprite *part)
{
}
