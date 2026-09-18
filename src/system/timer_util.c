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
 * Written as NAKED asm, not plain C: an earlier pass's plain-C
 * reconstruction (see docs/matching/issue-69-eeprom-timer.md) got the
 * field/register access and literal-pool ordering right but never
 * closed the last few instructions - the `REG_IF = 8 <<
 * gUnknown_03001620` shift's operand-evaluation order, the following
 * `REG_IE |=` store's result register, and the final restore re-using
 * the still-live r8 copy instead of re-fetching it - a cluster of
 * small scratch-register choices this compiler wouldn't reproduce
 * simultaneously no matter how the C was rephrased. Every one of those
 * was already independently confirmed correct at the semantic level in
 * that pass's write-up, so this is a mechanical, byte-verified
 * transcription of the ROM's own instructions (translated from the
 * disassembler's unified syntax to this project's established NAKED
 * plain/divided syntax, local labels renumbered per
 * docs/matching/issue-4-sio-settings-sync.md's convention), not an
 * inferred control-flow guess. */
NAKED void sub_803AA08(u16 *arg0)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sb\n\t"
        "mov r6, r8\n\t"
        "push {r6, r7}\n\t"
        "ldr r2, 1f\n\t"
        "ldr r1, 2f\n\t"
        "mov sb, r1\n\t"
        "ldrh r1, [r1]\n\t"
        "strh r1, [r2]\n\t"
        "mov r6, #0\n\t"
        "mov r2, sb\n\t"
        "strh r6, [r2]\n\t"
        "ldr r3, 3f\n\t"
        "mov r8, r3\n\t"
        "ldr r5, [r3]\n\t"
        "strh r6, [r5, #2]\n\t"
        "ldr r3, 4f\n\t"
        "ldr r4, 5f\n\t"
        "ldrb r1, [r4]\n\t"
        "mov r2, #8\n\t"
        "add r7, r2, #0\n\t"
        "lsl r7, r1\n\t"
        "add r1, r7, #0\n\t"
        "strh r1, [r3]\n\t"
        "sub r3, #2\n\t"
        "ldrb r1, [r4]\n\t"
        "lsl r2, r1\n\t"
        "ldrh r1, [r3]\n\t"
        "orr r1, r2\n\t"
        "strh r1, [r3]\n\t"
        "ldr r1, 6f\n\t"
        "strb r6, [r1]\n\t"
        "ldr r2, 7f\n\t"
        "ldrh r1, [r0]\n\t"
        "strh r1, [r2]\n\t"
        "add r0, #2\n\t"
        "ldrh r1, [r0]\n\t"
        "strh r1, [r5]\n\t"
        "add r1, r5, #2\n\t"
        "mov r2, r8\n\t"
        "str r1, [r2]\n\t"
        "ldrh r0, [r0, #2]\n\t"
        "strh r0, [r5, #2]\n\t"
        "str r5, [r2]\n\t"
        "mov r0, #1\n\t"
        "mov r3, sb\n\t"
        "strh r0, [r3]\n\t"
        "pop {r3, r4}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "1: .4byte gUnknown_0300162C\n"
    "2: .4byte 0x04000208\n"
    "3: .4byte gUnknown_03001628\n"
    "4: .4byte 0x04000202\n"
    "5: .4byte gUnknown_03001620\n"
    "6: .4byte gUnknown_03001624\n"
    "7: .4byte gUnknown_03001622\n"
    );
}
