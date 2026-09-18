#include "core.h"

/* Sits at its own real ROM address (0x0803AA90) between the still-parked
 * `sub_803AA08` (asm/code_3_2_20e_aa08.s) and `sub_803AAD4`
 * (asm/code_3_2_20e_aa90.s), so it needs its own translation unit rather
 * than living alongside them in src/system/timer_util.c - see
 * docs/workflow.md step 4's "needs its own new .c file" case. */

extern u8 gUnknown_03001620;
extern vu16 * volatile gUnknown_03001628;
extern u16 gUnknown_0300162C;

/* The exact inverse of `sub_803AA08` (stops the claimed timer, disables
 * its IRQ, restores IME). The ROM briefly repoints `gUnknown_03001628`
 * itself at the timer's CNT_H half (storing the incremented pointer
 * back to the global) before restoring it - a plain `ptr[1] = 0` here
 * let the compiler dead-store-eliminate that temporary repoint, since
 * nothing reads it back before it's overwritten again. Marking
 * `gUnknown_03001628` `volatile` (src/system/timer_util.c's own
 * declaration of the same global) and routing the increment/decrement
 * through an explicit local `ptr` (rather than
 * `gUnknown_03001628++`/`--` directly, which - being volatile now -
 * would re-read the global from memory on every access instead of
 * keeping it in a register) reproduces the ROM's actual read-once/
 * write-back-twice shape exactly. The final `REG_IE &= ~(8 <<
 * gUnknown_03001620)` needed its own fix: plain C loads the shift's
 * constant operand before its variable operand (confirmed even in
 * isolation - this compiler's `x << y` always materializes `x` first),
 * where the ROM loads the index first; pinning `idx`/`bit` to registers
 * and writing them as their own statements (`idx = ...; bit = 8; bit
 * <<= idx;`) reverses that order to match, and pinning the REG_IE
 * address itself to r2 (assigned before either) gets its literal load
 * to the very top of the sequence too, matching the ROM's load order
 * exactly. */
void sub_803AA90(void)
{
    vu16 *ptr;
    register vu16 *riePtr asm("r2");
    register u8 idx asm("r0");
    register u16 bit asm("r1");

    REG_IME = 0;

    ptr = gUnknown_03001628;
    *ptr = 0;
    ptr++;
    gUnknown_03001628 = ptr;
    *ptr = 0;
    ptr--;
    gUnknown_03001628 = ptr;

    riePtr = (vu16 *)REG_ADDR_IE;
    idx = gUnknown_03001620;
    bit = 8;
    bit <<= idx;
    *riePtr &= ~bit;

    REG_IME = gUnknown_0300162C;
}
