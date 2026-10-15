/* The pc platform's engine/src/engine/sys/bios_util.cpp (platform/pc/platform.mk,
 * "Units"): the GBA's DivMod is an inline `svc #6` with its operands
 * bound to r0/r1, which no host compiler takes, so the platform builds
 * this copy of the unit in its place. MemCopy32 and Ctrl::Update (Ctrl's
 * key method, so Ctrl's vtable is emitted here) are the original's. */

#include "game/entities/ctrl.hpp"

extern "C" {
#include <agb_syscall.h>
#include "engine/sys/system.h"
}

using Game::Entities::Ctrl;
using Game::Entities::MovingSprite;

/* BIOS `Div` (SWI 6): the quotient, rounded toward zero, and the
 * remainder, with the dividend's sign. */
s32 DivMod(s32 number, s32 denom, s32 *remainderOut)
{
    *remainderOut = number % denom;
    return number / denom;
}

void *MemCopy32(void *dst, const void *src, u32 byteCount)
{
    CpuSet(src, dst, ((byteCount << 9) >> 11) | CPU_SET_32BIT);
    return dst;
}

void Ctrl::Update(MovingSprite *part)
{
}
