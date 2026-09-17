#include "core.h"

/* BIOS SWI wrappers, thin single-instruction trampolines. Real GBA BIOS
 * call numbers, per the standard swi list. */

NAKED void sub_803A944(void) /* BgAffineSet (SWI 0xE) */
{
    asm("svc #0xe\n\tbx lr");
}

NAKED void sub_803A948(void) /* CpuFastSet (SWI 0xC) */
{
    asm("svc #0xc\n\tbx lr");
}

NAKED void sub_803A94C(void) /* CpuSet (SWI 0xB) */
{
    asm("svc #0xb\n\tbx lr");
}

NAKED void LZ77UnCompWrapper(void) /* LZ77UnCompVram (SWI 0x12) */
{
    asm("svc #0x12\n\tbx lr");
}

NAKED void sub_803A954(void) /* ObjAffineSet (SWI 0xF) */
{
    asm("svc #0xf\n\tbx lr");
}

NAKED void RLUnCompWrapper(void) /* RLUnCompVram (SWI 0x15) */
{
    asm("svc #0x15\n\tbx lr");
}

NAKED void sub_803A95C(void) /* Sqrt (SWI 8) */
{
    asm("svc #8\n\tbx lr");
}

NAKED void sub_0803A960(void) /* VBlankIntrWait (SWI 5), with r2 zeroed first */
{
    asm("movs r2, #0\n\tsvc #5\n\tbx lr");
}

/* A small (12-byte) config table selected by `sub_803A968` and consumed
 * by the DMA3-driven transfer helper `sub_803AAD4` and the still-raw
 * `sub_803AB54`/`sub_803AC04` (see the `code_3_2_20e_ab54.s` header
 * comment). Every access site is consistent with this being a GBA
 * EEPROM save-chip descriptor: `waitcntBits` only ever ORs into
 * `WAITCNT`'s wait-state-2 field (the field cartridge EEPROM access
 * needs tuned), `addrBitCount` is read as a small byte count (6 or 14 -
 * exactly the real 512-byte/8-KB EEPROM chip address-bit widths), and
 * `sub_803AAD4` DMAs to/from `0x0D000000` (the real EEPROM memory
 * window) elsewhere in this chunk. Kept honest as a `struct` with the
 * two fields this file's own matched code actually touches; the other
 * two are named from context but not yet exercised by any matched
 * function. */
struct EepromConfig {
    u32 unk0;
    u16 maxCount;
    u16 waitcntBits;
    u8 addrBitCount;
    u8 pad[3];
};

extern struct EepromConfig gStaticData_085A9EF8;
extern struct EepromConfig gStaticData_085A9F04;
extern struct EepromConfig *gUnknown_03001634;

/* Picks the EEPROM chip's config table by its "backup type" code (4 or
 * 0x40 - the two real chip sizes); any other type falls back to the
 * 512-byte table but reports failure. */
s32 sub_803A968(u16 type)
{
    s32 result = 0;

    if (type == 4) {
        gUnknown_03001634 = &gStaticData_085A9EF8;
    } else if (type == 0x40) {
        gUnknown_03001634 = &gStaticData_085A9F04;
    } else {
        gUnknown_03001634 = &gStaticData_085A9EF8;
        result = 1;
    }
    return result;
}

/* Raw data, not a function: a small hand-written Thumb code blob
 * (a tiny IRQ handler stub - see `sub_803A9D0` below, which hands its
 * address out to install as a timer's interrupt vector) sitting
 * between `sub_803A968` and `sub_803A9D0` in ROM. Kept as literal bytes
 * rather than reconstructed as a "function", matching how the project
 * treats other referenced-but-not-called data blobs. */
asm(
    "_0803A9AC: .byte 0x06\n"
    "gStaticData_0803A9AD:\n"
    "\t.byte 0x49, 0x08, 0x88\n"
    "\t.byte 0x00, 0x28, 0x08, 0xD0, 0x08, 0x88, 0x01, 0x38, 0x08, 0x80, 0x00, 0x04, 0x00, 0x28, 0x02, 0xD1\n"
    "\t.byte 0x02, 0x49, 0x01, 0x20, 0x08, 0x70, 0x70, 0x47, 0x22, 0x16, 0x00, 0x03, 0x24, 0x16, 0x00, 0x03\n"
);

extern u8 gUnknown_03001620;
extern vu16 *gUnknown_03001628;
extern u8 gStaticData_0803A9AD[];

/* Claims hardware timer `index` (0-3) for this subsystem: records the
 * index, points `gUnknown_03001628` at that timer's TMxCNT_L register,
 * and hands the caller the address of a small hand-written IRQ handler
 * stub (`gStaticData_0803A9AD`, sitting just before this function in
 * ROM) to install as the timer's interrupt vector. */
s32 sub_803A9D0(u8 index, void **out)
{
    if (index > 3) {
        return 1;
    }
    gUnknown_03001620 = index;
    gUnknown_03001628 = (vu16 *)(0x04000100 + gUnknown_03001620 * 4);
    *out = gStaticData_0803A9AD;
    return 0;
}

extern u16 gUnknown_0300162C;
extern u8 gUnknown_03001624;
extern u16 gUnknown_03001622;

#if NON_MATCHING
/* NOT YET BYTE-MATCHING: semantics fully understood (arms the timer
 * claimed by `sub_803A9D0`: saves/clears IME, zeroes the timer's
 * control register, acknowledges and enables its IRQ line in IF/IE,
 * copies `arg0`'s three u16 fields into the module's globals and the
 * timer's reload/control registers, then restores IME) and every
 * field/register access below matches the ROM one-for-one, but this
 * compiler's allocation of the two values that need to survive the
 * whole function (the saved IME register address and the timer
 * register pointer) lands them in the opposite extended registers from
 * the ROM (r8 vs r9 swapped relative to real usage for at least one
 * statement ordering), and a couple of literal-pool loads come out in
 * a different order than the ROM's. Register-pinning both values (see
 * `matching_decomp_register_pinning`) got the extended-register choice
 * right but not the pool ordering; not yet found a source phrasing
 * that reproduces both simultaneously. */
void sub_803AA08(u16 *arg0)
{
    register vu16 *imeAddr asm("r9");
    register vu16 *timerPtr asm("r8");
    u16 savedIme;

    imeAddr = (vu16 *)0x04000208;
    savedIme = *imeAddr;
    gUnknown_0300162C = savedIme;
    *imeAddr = 0;

    timerPtr = gUnknown_03001628;
    timerPtr[1] = 0;

    REG_IF = 8 << gUnknown_03001620;
    REG_IE |= 8 << gUnknown_03001620;

    gUnknown_03001624 = 0;
    gUnknown_03001622 = *arg0;
    arg0++;

    *timerPtr = *arg0;
    arg0++;
    gUnknown_03001628 = timerPtr + 1;
    timerPtr[1] = *arg0;
    gUnknown_03001628 = timerPtr;

    *imeAddr = 1;
}

/* NOT YET BYTE-MATCHING: the exact inverse of `sub_803AA08` (stops the
 * claimed timer, disables its IRQ, restores IME) - semantics and every
 * field/register access confirmed, but the ROM briefly repoints
 * `gUnknown_03001628` itself at the timer's CNT_H half (storing the
 * incremented pointer back to the global) before restoring it, where
 * this compiler folds that round trip into a plain offset store
 * (`ptr[1] = 0` without the intermediate global writes). Behaviorally
 * identical; not byte-identical. */
void sub_803AA90(void)
{
    REG_IME = 0;

    *gUnknown_03001628 = 0;
    gUnknown_03001628++;
    *gUnknown_03001628 = 0;
    gUnknown_03001628--;

    REG_IE &= ~(8 << gUnknown_03001620);

    REG_IME = gUnknown_0300162C;
}

/* NOT YET BYTE-MATCHING: a DMA3-driven block transfer used by the
 * (still-raw) EEPROM bit-serial read/write routines below - saves and
 * clears IME, merges the active `EepromConfig`'s `waitcntBits` into
 * WAITCNT's wait-state-2 field, programs DMA3SAD/DAD/CNT for a one-shot
 * 16-bit-unit transfer, busy-waits on DMA3CNT_H's enable bit if it's
 * still set right after the trigger, then restores IME. Every
 * instruction's purpose is confirmed against the ROM; what resists
 * matching is purely the busy-wait tail's shape - the ROM evaluates
 * `DMA3CNT_H & 0x8000` twice with genuinely different register/literal
 * choices each time (consistent with `if (cond) { do {} while (cond); }`
 * written with two textually-identical checks), but this compiler's
 * loop-rotation collapses any C phrasing of that (plain `while`,
 * `if`+`do-while`, `if`+`while`, explicit `goto`) into a single shared
 * top-tested loop instead. */
void sub_803AAD4(const void *src, void *dst, u16 count)
{
    u16 savedIme;
    u16 waitcnt;

    savedIme = REG_IME;
    REG_IME = 0;

    waitcnt = REG_WAITCNT;
    waitcnt &= 0xF8FF;
    waitcnt |= gUnknown_03001634->waitcntBits;
    REG_WAITCNT = waitcnt;

    REG_DMA3SAD = (u32)src;
    REG_DMA3DAD = (u32)dst;
    REG_DMA3CNT = count | 0x80000000;

    if (REG_DMA3CNT_H & 0x8000) {
        while (REG_DMA3CNT_H & 0x8000) {
        }
    }

    REG_IME = savedIme;
}
#endif
