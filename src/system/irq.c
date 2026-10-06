#include "core.h"
#include "match.h"
#include "system.h"
#include "audio.h"
#include <gax.h>
#include <agb_syscall.h>
#include "globals.h"

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

    if (tmp == NULL) {
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

__asm__(".align 2,0");

/* Clears the VBlank callbacks, installs VBlankHandler and enables the
 * VBlank IRQ in DISPSTAT. Called once from AgbMain. */
void EnableVBlankHandler(void)
{
    irq_handler_t fn = VBlankHandler;
    struct vblank_callbacks *base = &gVBlankCallbacks;
    s32 unknown = 0;
    s32 *current = &base->funcs[7];
    // this does not look right, but matches generated assembly
    u8 tmp;
    MATCH_HOLD_REG(u8 *, value, r1);

    do {
        *current-- = unknown;
    } while ((s32)current >= (s32)&base->funcs[0]);

    IrqSetHandler(INTR_INDEX_VBLANK, fn);

    value = (u8 *)REG_ADDR_DISPSTAT;
    tmp = DISPSTAT_VBLANK_INTR;
    *value = tmp | *value;
}

/* The inverse of EnableVBlankHandler: disables the VBlank IRQ in
 * DISPSTAT and reinstalls the previous VBlank handler. */
void DisableVBlankHandler(void)
{
    MATCH_HOLD_REG(vu8 *, dispstat, r1) = (vu8 *)REG_ADDR_DISPSTAT;
    u8 tmp = DISPSTAT_VBLANK_INTR;

    *dispstat &= ~tmp;

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

extern void _call_via_r0(void);

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

/* The held d-pad bits as a direction 0-8 (0 = none), through
 * gDpadDirectionTable. `input` (gInput at every call site) is unused. */
u8 GetDpadDirection(void *input)
{
    u8 idx = 0;
    if (gKeys.half.held & 0x10)
        idx |= 8;
    if (gKeys.half.held & 0x20)
        idx |= 4;
    if (gKeys.half.held & 0x80)
        idx |= 2;
    if (gKeys.half.held & 0x40)
        idx |= 1;
    return gDpadDirectionTable[idx];
}

/* Reads the raw (active-low) hardware key register, inverts it to
 * active-high, records newly-pressed bits into gKeys' second halfword
 * (`pressed`, read/written through pointer
 * arithmetic off gKeys rather than its own extern: agbcc
 * doesn't know the two globals are adjacent and emits a second,
 * non-matching literal-pool load/store pair otherwise), updates
 * gKeys to the new state, then returns 1 if the low 4 bits
 * (A/B/Select/Start) are all held - a "soft reset" combo check. All
 * four register pins below are plain caller-saved scratch (r0-r3), so
 * none of them carry the r4-r7 save/restore hazard: `addr`/`prevKeys`
 * (r2/r3) match the ROM's choice for the address/reload pair, and
 * `keysR1`/`mask` (r1/r0) match its choice for the closing mask-and-
 * compare (gcc's own unpinned allocator picks a fresh register for the
 * AND result instead of reusing r1 in place, and compares against a
 * fresh immediate instead of reusing r0's already-loaded 0xF). The
 * inline `add %0,%1,#0` anchors a copy of `keys` into a scratch value
 * gcc would otherwise schedule after the `prevKeys` reload instead of
 * before it, despite neither having a data dependency on the other.
 * `input` (gInput at every call site) is unused. */
s32 UpdateKeys(void *input)
{
    u16 keys;
    u16 keysCopy;
    MATCH_HOLD_REG(u16 *, addr, r2);
    MATCH_HOLD_REG(u16, prevKeys, r3);
    MATCH_HOLD_REG(u16, keysR1, r1);
    MATCH_HOLD_REG(s32, mask, r0);

    keys = (u16)~REG_KEYINPUT;
    addr = &gKeys.half.held;
    asm volatile("add %0, %1, #0" : "=r"(keysCopy) : "r"(keys));
    prevKeys = *addr;
    *(u16 *)((u8 *)addr + 2) = keysCopy & ~prevKeys;
    *addr = keys;
    keysR1 = keys;
    mask = 0xF;
    keysR1 &= mask;
    if (mask == keysR1) {
        return 1;
    }
    return 0;
}

/* Clears gKeys (held and newly pressed). */
void ClearKeys(void)
{
    MATCH_HOLD_REG(u16 *, addr, r2);
    MATCH_HOLD_REG(u16, zero, r1);

    addr = &gKeys.half.held;
    zero = 0;
    *addr = zero;
    *(u16 *)((u8 *)addr + 2) = zero;
}

__asm__(".align 2,0");