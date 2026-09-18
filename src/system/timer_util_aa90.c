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

/* Same `EepromConfig` global src/system/timer_util.c/eeprom_util.c
 * declare (with the real struct type) - only its address is needed
 * here, since it's referenced from inline asm rather than dereferenced
 * in C. */
extern void *gUnknown_03001634;

/* A DMA3-driven block transfer used by the EEPROM bit-serial read/write
 * routines in src/system/eeprom_util.c - saves and clears IME, merges
 * the active `EepromConfig`'s `waitcntBits` into WAITCNT's wait-state-2
 * field, programs DMA3SAD/DAD/CNT for a one-shot 16-bit-unit transfer,
 * busy-waits on DMA3CNT_H's enable bit if it's still set right after
 * the trigger, then restores IME. Sits right after `sub_803AA90` in ROM
 * (0x0803AAD4), so it lives in this file rather than
 * src/system/timer_util.c - see docs/workflow.md step 4's "needs its
 * own new .c file" case (same reason `sub_803AA90` itself is here).
 *
 * Written as NAKED asm: an earlier pass's plain-C reconstruction (see
 * docs/matching/issue-69-eeprom-timer.md) confirmed every instruction's
 * purpose but hit two gaps neither survives rephrasing - the busy-wait
 * tail evaluates `DMA3CNT_H & 0x8000` twice with genuinely different
 * register/literal choices each time (this compiler's loop-rotation
 * always collapses that into one shared top-tested loop instead), and
 * the twice-used `REG_IME` address (read once up top, written once at
 * the very end) gets cached into a scratch register across the whole
 * function where the ROM just re-loads the same literal pool entry
 * twice. Since every instruction was already independently confirmed
 * correct, this is a mechanical, byte-verified transcription of the
 * ROM's own instructions (translated from the disassembler's unified
 * syntax to this project's established NAKED plain/divided syntax,
 * local labels renumbered per
 * docs/matching/issue-4-sio-settings-sync.md's convention), not an
 * inferred control-flow guess. */
NAKED void sub_803AAD4(const void *src, void *dst, u16 count)
{
    asm(
        "push {r4, r5, r6, lr}\n\t"
        "lsl r2, r2, #0x10\n\t"
        "lsr r2, r2, #0x10\n\t"
        "ldr r4, 1f\n\t"
        "ldrh r3, [r4]\n\t"
        "add r6, r3, #0\n\t"
        "mov r3, #0\n\t"
        "strh r3, [r4]\n\t"
        "ldr r5, 2f\n\t"
        "ldrh r4, [r5]\n\t"
        "ldr r3, 3f\n\t"
        "and r4, r3\n\t"
        "ldr r3, 4f\n\t"
        "ldr r3, [r3]\n\t"
        "ldrh r3, [r3, #6]\n\t"
        "orr r4, r3\n\t"
        "strh r4, [r5]\n\t"
        "ldr r3, 5f\n\t"
        "str r0, [r3]\n\t"
        "ldr r0, 6f\n\t"
        "str r1, [r0]\n\t"
        "ldr r1, 7f\n\t"
        "mov r0, #0x80\n\t"
        "lsl r0, r0, #0x18\n\t"
        "orr r2, r0\n\t"
        "str r2, [r1]\n\t"
        "add r1, #2\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #8\n\t"
        "add r0, r2, #0\n\t"
        "ldrh r1, [r1]\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "beq 10f\n\t"
        "ldr r2, 8f\n\t"
        "mov r0, #0x80\n\t"
        "lsl r0, r0, #8\n\t"
        "add r1, r0, #0\n\t"
    "9:\n\t"
        "ldrh r0, [r2]\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "bne 9b\n\t"
    "10:\n\t"
        "ldr r0, 1f\n\t"
        "strh r6, [r0]\n\t"
        "pop {r4, r5, r6}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "1: .4byte 0x04000208\n"
    "2: .4byte 0x04000204\n"
    "3: .4byte 0x0000F8FF\n"
    "4: .4byte gUnknown_03001634\n"
    "5: .4byte 0x040000D4\n"
    "6: .4byte 0x040000D8\n"
    "7: .4byte 0x040000DC\n"
    "8: .4byte 0x040000DE\n"
    );
}
