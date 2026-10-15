#include "engine/sys/irq.hpp"
#include "engine/sys/frame_limiter.hpp"
#include "game/level/spawners.hpp"
#include "engine/audio/audio.hpp"

extern "C" {
#include "core.h"
#include "engine/sys/system.h"
#include <gax.h>
#include <agb_syscall.h>
#include "globals.h"
}

using Engine::Sys::FrameLimiter;
using Engine::Sys::Irq;

/* Points an IRQ's handler at EmptyHandler (its IE bit is left alone). */
void Irq::ClearHandler(s32 interruptIndex)
{
    table[interruptIndex] = EmptyHandler;
}

/* Undoes SetHandler(): reinstalls the handler it replaced (and masks
 * the IRQ in IE if there was none), then forgets the saved one. */
void Irq::RestoreHandler(s32 interruptIndex)
{
    irq_handler_t tmp = table[interruptIndex] = prevTable[interruptIndex];

    if (tmp == 0) {
        /* `previousIMEvalue` is needed: inlined, the instructions differ */
        u16 previousIMEvalue = REG_IME;
        REG_IME = 0;                      // disable IME
        REG_IE &= ~(1 << interruptIndex); // disable specific interrupt
        REG_IME = previousIMEvalue;       // bring back previous IME status
    }

    prevTable[interruptIndex] = EmptyHandler;
}

/* Installs `fn` as an IRQ's handler, saving the old one for
 * RestoreHandler(), and enables the IRQ in IE. */
void Irq::SetHandler(s32 interruptIndex, irq_handler_t fn)
{
    prevTable[interruptIndex] = table[interruptIndex];
    table[interruptIndex] = fn;
    REG_IE |= 1 << interruptIndex;
}


void Irq::Disable()
{
    REG_IME = 0;
}

u32 Irq::Setup()
{
    /* `intrbuffer` is needed: inlined, the instructions are reordered */
    u32 *intrbuffer = &IntrMain_Buffer;
    irq_handler_t fn = EmptyHandler;
    irq_handler_t *dst1 = prevTable;
    irq_handler_t *dst2 = table;
    s32 count;

    for (count = ARRAY_COUNT(table) - 1; count >= 0; count--) {
        *dst1++ = fn;
        *dst2++ = fn;
    }

    INTR_VECTOR = intrbuffer;
    REG_IME = 1;

    return 0;
}

void Irq::EmptyHandler()
{
}

/* Clears the VBlank callbacks, installs VBlankHandler and enables the
 * VBlank IRQ in DISPSTAT. Called once from AgbMain. */
void Irq::EnableVBlankHandler()
{
    /* `fn` is needed: inlined, the instructions are reordered */
    irq_handler_t fn = VBlankHandler;
    struct vblank_callbacks *base = &vblankCallbacks;
    /* `zero` is needed: inlined, the instructions are reordered */
    irq_handler_t zero = 0;
    irq_handler_t *current = &base->funcs[ARRAY_COUNT(base->funcs) - 1];

    do {
        *current-- = zero;
    } while ((intptr_t)current >= (intptr_t)&base->funcs[0]);

    SetHandler(INTR_INDEX_VBLANK, fn);

    *(u8 *)REG_ADDR_DISPSTAT |= DISPSTAT_VBLANK_INTR;
}

/* The inverse of EnableVBlankHandler: disables the VBlank IRQ in
 * DISPSTAT and reinstalls the previous VBlank handler. */
void Irq::DisableVBlankHandler()
{
    /* `tmp` is needed: inlined, the instructions differ */
    u8 tmp = DISPSTAT_VBLANK_INTR;

    *(u8 *)REG_ADDR_DISPSTAT &= ~tmp;

    RestoreHandler(INTR_INDEX_VBLANK);
}

/* Frees the callback slot AddVBlankCallback() returned. */
void Irq::RemoveVBlankCallback(s32 index)
{
    vblankCallbacks.funcs[index] = 0;
}

/* Puts `fn` in the first free VBlank callback
 * slot and returns the slot, or -1 if all eight are taken. */
s32 Irq::AddVBlankCallback(void (*fn)(void))
{
    s32 index = 0;

    while (index < (s32)ARRAY_COUNT(vblankCallbacks.funcs)) {
        if (vblankCallbacks.funcs[index] == 0) {
            vblankCallbacks.funcs[index] = fn;
            return index;
        }

        index++;
    }

    return -1;
}

/* Waits for the next VBlank (BIOS VBlankIntrWait). With the frame limit
 * on (Set), keeps waiting until gVBlankCounter reaches `target`, then
 * moves the target on by `interval`. */
void FrameLimiter::WaitForVBlank()
{
    u32 *p1;
    u32 *p2;
    /* `p3` is needed: inlined, the code is 1 instruction longer */
    u32 *p3;
    u32 v1;
    u32 v2;

    if (enabled != 0) {
        p1 = &gVBlankCounter;
        p2 = &target;
        p3 = &interval;
        v1 = *p1;
        v2 = *p2;
        while (v1 < v2) {
            VBlankIntrWait();
            v1 = *p1;
            v2 = *p2;
        }
        *p2 = v2 + *p3;
    } else {
        VBlankIntrWait();
    }
}

/* UNUSED - no caller anywhere in the ROM (checked every asm/*.s,
 * games/xs/expected/*.s and src/ file for a bl/.4byte reference). Turns
 * WaitForVBlank's frame limit off. */
void FrameLimiter::Disable()
{
    enabled = 0;
}

/* UNUSED - no caller anywhere in the ROM (checked as for Disable).
 * Makes WaitForVBlank return at most once every `frameCount` VBlanks. */
void FrameLimiter::Set(u32 frameCount)
{
    interval = frameCount;
    target = gVBlankCounter + frameCount;
    enabled = 1;
}

/* The VBlank IRQ handler: calls GAX_irq while gGaxIrqEnabled is set,
 * calls every non-zero VBlank callback and counts the frame in
 * gVBlankCounter. */
void Irq::VBlankHandler()
{
    irq_handler_t *p;
    s32 i;

    if (gGaxIrqEnabled != 0) {
        GAX_irq();
    }
    p = vblankCallbacks.funcs;
    i = 7;
    do {
        if (*p != 0) {
            (*p)();
        }
        p++;
        i--;
    } while (i >= 0);
    gVBlankCounter++;
}
