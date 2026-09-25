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
extern vu16 * volatile gUnknown_03001628;
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

/* Arms the timer claimed by `sub_803A9D0`: saves/clears IME, zeroes the
 * timer's control register, acknowledges and enables its IRQ line in
 * IF/IE, copies `arg0`'s three u16 fields into the module's globals and
 * the timer's reload/control registers, then restores IME.
 *
 * Real C, not NAKED asm: the last three register-allocation gaps this
 * function parked on (the `REG_IF = 8 << gUnknown_03001620` shift's
 * operand-evaluation order, the `REG_IE |=` store's result register,
 * and the final restore re-fetching r8 instead of reusing the still-
 * live low-register copy from three instructions earlier) all turned
 * out to be closeable, not fundamental compiler limitations:
 * - The shift-order gap: writing `idx = gUnknown_03001620;` as its own
 *   statement before `*regAddr = bit << idx;` forces the index load
 *   before the constant, matching the ROM (same fix as
 *   `sub_803AA90`'s `REG_IE &=` line); the REG_IE half reuses `bit`'s
 *   register in place for its second, in-place shift instead of a
 *   fresh copy, exactly like the ROM.
 * - `regAddr` walks IF -> IE via `regAddr--` (pointer arithmetic on
 *   the already-loaded address) rather than a second absolute load,
 *   reproducing the ROM's `subs r3, #2`.
 * - The `gUnknown_03001628`-pointer dance needed an
 *   `asm volatile("" : "+r" (addrPtrPin))` barrier right after copying
 *   it into the r8-pinned variable: without it, gcc proves the r8 copy
 *   redundant (the address is still live in a low register the whole
 *   function) and elides the pin entirely, keeping everything in a low
 *   register instead of stashing across the busy IF/IE section the way
 *   the ROM does. The barrier forces the value to actually live in r8,
 *   after which reloading it into a fresh low-register local right
 *   before both trailing stores (and reusing that same local, not the
 *   pin again, for the second store) reproduces the ROM's "one r8
 *   reload, reused for both stores" shape exactly.
 * - `arg0` has to be walked with `arg0++`/`*arg0`, not `arg0[1]`/
 *   `arg0[2]` indexing - the ROM's own `adds r0, #2` between the first
 *   and second halfword reads only appears when the pointer is
 *   actually incremented in C, not when both offsets are computed from
 *   the original base.
 * - Three remaining scratch-register picks (the two `REG_IME` low-
 *   register copies bracketing the busy middle section, and the
 *   `gUnknown_03001628` load itself) needed explicit pins (`r2`/`r3`/
 *   `r3` respectively, each in their own short-lived nested scope) to
 *   land on the exact registers the ROM's compile picked over the
 *   ones this compiler's unforced allocator preferred instead; per
 *   `matching_decomp_register_pinning`, r7 was never pinned - the
 *   ROM's own `r7` scratch copy in the IF computation falls out of
 *   natural allocation once `bit`'s value has to survive to be reused
 *   unshifted in the IE computation below it.
 * Verified instruction-for-instruction against an isolated compile of
 * this file before being folded into the full build - see
 * docs/matching/issue-69-eeprom-timer.md. */
void sub_803AA08(u16 *arg0)
{
    register vu16 *imeAddr asm("r9");
    register vu16 * volatile *addrPtr asm("r3");
    register vu16 * volatile *addrPtrPin asm("r8");
    vu16 * volatile *lowPtr;
    vu16 *ptr;
    register u8 idx asm("r1");
    register u16 bit asm("r2");
    register u16 zero asm("r6");
    vu16 *regAddr;

    gUnknown_0300162C = *(imeAddr = (vu16 *)REG_ADDR_IME);
    zero = 0;
    {
        register vu16 *imeScratch asm("r2");
        imeScratch = imeAddr;
        *imeScratch = zero;
    }

    addrPtr = &gUnknown_03001628;
    addrPtrPin = addrPtr;
    asm volatile("" : "+r" (addrPtrPin));
    ptr = *addrPtr;
    *(vu16 *)((u8 *)ptr + 2) = zero;

    regAddr = (vu16 *)REG_ADDR_IF;
    idx = gUnknown_03001620;
    bit = 8;
    *regAddr = bit << idx;
    regAddr--;

    idx = gUnknown_03001620;
    bit <<= idx;
    *regAddr |= bit;

    gUnknown_03001624 = zero;

    gUnknown_03001622 = arg0[0];
    arg0++;
    *ptr = *arg0;

    {
        vu16 *incPtr = (vu16 *)((u8 *)ptr + 2);
        lowPtr = addrPtrPin;
        *lowPtr = incPtr;
    }
    ((u16 *)ptr)[1] = arg0[1];
    *lowPtr = ptr;

    {
        u16 one;
        register vu16 *imeScratch2 asm("r3");
        one = 1;
        imeScratch2 = imeAddr;
        *imeScratch2 = one;
    }
}
