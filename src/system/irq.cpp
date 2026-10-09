#include "spawners.hpp"

extern "C" {
#include "core.h"
#include "system.h"
#include "audio.h"
#include <gax.h>
#include <agb_syscall.h>
#include "globals.h"
}

/* Points an IRQ's handler at IrqEmptyHandler (its IE bit is left alone). */
void IrqClearHandler(s32 interruptIndex)
{
    gIntrTable[interruptIndex] = IrqEmptyHandler;
}

/* Undoes IrqSetHandler(): reinstalls the handler it replaced (and masks
 * the IRQ in IE if there was none), then forgets the saved one. */
void IrqRestoreHandler(s32 interruptIndex)
{
    irq_handler_t tmp = gIntrTable[interruptIndex] = gPrevIntrTable[interruptIndex];

    if (tmp == 0) {
        u16 previousIMEvalue = REG_IME;
        REG_IME = 0;                      // disable IME
        REG_IE &= ~(1 << interruptIndex); // disable specific interrupt
        REG_IME = previousIMEvalue;       // bring back previous IME status
    }

    gPrevIntrTable[interruptIndex] = IrqEmptyHandler;
}

/* Installs `fn` as an IRQ's handler, saving the old one for
 * IrqRestoreHandler(), and enables the IRQ in IE. */
void IrqSetHandler(s32 interruptIndex, irq_handler_t fn)
{
    gPrevIntrTable[interruptIndex] = gIntrTable[interruptIndex];
    gIntrTable[interruptIndex] = fn;
    REG_IE |= 1 << interruptIndex;
}


void IrqDisable(void)
{
    REG_IME = 0;
}

u32 IrqSetup(void)
{
    u32 *intrbuffer = &IntrMain_Buffer;
    irq_handler_t fn = IrqEmptyHandler;
    irq_handler_t *dst1 = gPrevIntrTable;
    irq_handler_t *dst2 = gIntrTable;
    s32 count;

    for (count = 0xD; count >= 0; count--) {
        *dst1++ = fn;
        *dst2++ = fn;
    }

    INTR_VECTOR = intrbuffer;
    REG_IME = 1;

    return 0;
}

void IrqEmptyHandler(void)
{
}

/* Clears the VBlank callbacks, installs VBlankHandler and enables the
 * VBlank IRQ in DISPSTAT. Called once from AgbMain. */
void EnableVBlankHandler(void)
{
    irq_handler_t fn = VBlankHandler;
    struct vblank_callbacks *base = &gVBlankCallbacks;
    s32 zero = 0;
    s32 *current = &base->funcs[7];

    do {
        *current-- = zero;
    } while ((s32)current >= (s32)&base->funcs[0]);

    IrqSetHandler(INTR_INDEX_VBLANK, fn);

    *(u8 *)REG_ADDR_DISPSTAT |= DISPSTAT_VBLANK_INTR;
}

/* The inverse of EnableVBlankHandler: disables the VBlank IRQ in
 * DISPSTAT and reinstalls the previous VBlank handler. */
void DisableVBlankHandler(void)
{
    u8 tmp = DISPSTAT_VBLANK_INTR;

    *(u8 *)REG_ADDR_DISPSTAT &= ~tmp;

    IrqRestoreHandler(INTR_INDEX_VBLANK);
}

/* Frees the callback slot AddVBlankCallback() returned. */
void RemoveVBlankCallback(s32 index)
{
    gVBlankCallbacks.funcs[index] = 0;
}

/* Puts `fn` in the first free VBlank callback
 * slot and returns the slot, or -1 if all eight are taken. */
s32 AddVBlankCallback(void (*fn)(void))
{
    s32 index = 0;

    while (index <= 7) {
        if (gVBlankCallbacks.funcs[index] == 0) {
            gVBlankCallbacks.funcs[index] = (s32)fn;
            return index;
        }

        index++;
    }

    return -1;
}

/* Waits for the next VBlank (BIOS VBlankIntrWait). With the frame limit
 * on (SetFrameLimit), keeps waiting until gVBlankCounter reaches
 * gFrameLimitTarget, then moves the target on by gFrameLimitInterval. */
void WaitForVBlank(void)
{
    u32 *p1;
    u32 *p2;
    u32 *p3;
    u32 v1;
    u32 v2;

    if (gFrameLimitEnabled != 0) {
        p1 = &gVBlankCounter;
        p2 = &gFrameLimitTarget;
        p3 = &gFrameLimitInterval;
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
 * expected/*.s and src/ file for a bl/.4byte reference). Turns
 * WaitForVBlank's frame limit off. */
void DisableFrameLimit(void)
{
    gFrameLimitEnabled = 0;
}

/* UNUSED - no caller anywhere in the ROM (checked as for
 * DisableFrameLimit). Makes WaitForVBlank return at most once every
 * `interval` VBlanks. */
void SetFrameLimit(u32 interval)
{
    gFrameLimitInterval = interval;
    gFrameLimitTarget = gVBlankCounter + interval;
    gFrameLimitEnabled = 1;
}

extern "C" void _call_via_r0(void);

/* The VBlank IRQ handler: calls GAX_irq while gGaxIrqEnabled is set, calls every VBlank callback (through
 * _call_via_r0, with the pointer left in r0 by the test) and counts the
 * frame in gVBlankCounter. */
void VBlankHandler(void)
{
    s32 *p;
    s32 i;

    if (gGaxIrqEnabled != 0) {
        GAX_irq();
    }
    p = gVBlankCallbacks.funcs;
    i = 7;
    do {
        if (*p != 0) {
            _call_via_r0();
        }
        p++;
        i--;
    } while (i >= 0);
    gVBlankCounter++;
}
