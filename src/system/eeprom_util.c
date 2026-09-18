#include "core.h"

/* A DMA3 bit-serial GBA EEPROM save-chip read/write cluster, sitting
 * right after the DMA3 transfer helper `sub_803AAD4`
 * (src/system/timer_util_aa90.c) and before the matched verify/retry
 * pair in src/system/eeprom_verify.c - its own file per docs/
 * workflow.md step 4's "needs its own new .c file" case, since it isn't
 * adjacent to any already-matched file's functions. See
 * docs/matching/issue-69-*.md.
 *
 * Protocol confirmed by reading every instruction against the real GBA
 * EEPROM bit-serial protocol: a read sends 2 start bits ("11"), the
 * chip's `addrBitCount` address bits (MSB first), and one more bit
 * (left as uninitialized stack data - the ROM never explicitly sets
 * it, and this reconstruction doesn't either, to match byte-for-byte),
 * then reads back 0x44 (68) halfwords - the first few are the chip's
 * "busy" dummy bits (never unpacked), followed by 64 data bits. A write
 * sends 2 start bits ("10"), the address bits, 64 data bits (MSB first
 * per 16-bit word), and a trailing stop bit ("0"). Only bit 0 of each
 * 16-bit DMA slot matters to the hardware - neither the ROM nor this
 * reconstruction masks the values written into the bit buffer beyond
 * the address/data value itself, relying on the EEPROM interface only
 * latching bit 0 of each transferred halfword. */

/* Same shape as src/system/timer_util.c's own `struct EepromConfig` -
 * redeclared here (not shared via a header) since this file and
 * eeprom_verify.c also need it and C doesn't require cross-TU type
 * identity for this to link correctly. */
struct EepromConfig {
    u32 unk0;
    u16 maxCount;
    u16 waitcntBits;
    u8 addrBitCount;
    u8 pad[3];
};

extern struct EepromConfig *gUnknown_03001634;
extern u8 gUnknown_03001624;
extern u16 gStaticData_085A9F10[];

extern void sub_803AA08(u16 *arg0);
extern void sub_803AA90(void);
extern void sub_803AAD4(const void *src, void *dst, u16 count);

/* Reads one 8-byte (64-bit) EEPROM block at `addr` into `dest[0..3]`.
 * Returns 0x80FF if `addr` is out of range for the currently-selected
 * chip. See the file header comment for the confirmed wire protocol.
 *
 * Written as NAKED asm, not plain C: an earlier pass's plain-C
 * reconstruction (see docs/matching/issue-69-eeprom-timer.md) confirmed
 * every field/register access against the ROM, but the tail's bit-
 * unpack loop never reproduced the ROM's register plan - the ROM keeps
 * the "mask = 1" constant and the outer-loop counter in specific low
 * registers reused from the address-packing phase above, where this
 * compiler's allocator runs out of low registers at that point and
 * spills into r12 instead, plus hoists loop-invariant setup across the
 * two `sub_803AAD4` calls where the ROM doesn't. Since every
 * instruction was already independently confirmed correct, this is a
 * mechanical, byte-verified transcription of the ROM's own instructions
 * (translated from the disassembler's unified syntax to this project's
 * established NAKED plain/divided syntax, local labels renumbered per
 * docs/matching/issue-4-sio-settings-sync.md's convention), not an
 * inferred control-flow guess. */
NAKED s32 sub_803AB54(u16 addr, u16 *dest)
{
    asm(
        "push {r4, r5, r6, lr}\n\t"
        "sub sp, #0x88\n\t"
        "add r5, r1, #0\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r3, r0, #0x10\n\t"
        "ldr r0, 7f\n\t"
        "ldr r0, [r0]\n\t"
        "ldrh r0, [r0, #4]\n\t"
        "cmp r3, r0\n\t"
        "blo 1f\n\t"
        "ldr r0, 8f\n\t"
        "b 6f\n\t"
        ".align 2, 0\n"
    "7: .4byte gUnknown_03001634\n"
    "8: .4byte 0x000080FF\n"
    "1:\n\t"
        "ldr r0, 9f\n\t"
        "add r6, r0, #0\n\t"
        "ldr r0, [r0]\n\t"
        "ldrb r1, [r0, #8]\n\t"
        "lsl r0, r1, #1\n\t"
        "mov r4, sp\n\t"
        "add r2, r0, r4\n\t"
        "add r2, #2\n\t"
        "mov r4, #0\n\t"
        "cmp r4, r1\n\t"
        "bhs 3f\n\t"
    "2:\n\t"
        "strh r3, [r2]\n\t"
        "sub r2, #2\n\t"
        "lsr r3, r3, #1\n\t"
        "add r0, r4, #1\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r4, r0, #0x18\n\t"
        "ldr r0, [r6]\n\t"
        "ldrb r0, [r0, #8]\n\t"
        "cmp r4, r0\n\t"
        "blo 2b\n\t"
    "3:\n\t"
        "mov r0, #1\n\t"
        "strh r0, [r2]\n\t"
        "sub r2, #2\n\t"
        "strh r0, [r2]\n\t"
        "mov r4, #0xd0\n\t"
        "lsl r4, r4, #0x14\n\t"
        "ldr r0, 9f\n\t"
        "ldr r0, [r0]\n\t"
        "ldrb r2, [r0, #8]\n\t"
        "add r2, #3\n\t"
        "mov r0, sp\n\t"
        "add r1, r4, #0\n\t"
        "bl sub_803AAD4\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, sp\n\t"
        "mov r2, #0x44\n\t"
        "bl sub_803AAD4\n\t"
        "add r2, sp, #8\n\t"
        "add r5, #6\n\t"
        "mov r4, #0\n\t"
        "mov r6, #1\n\t"
    "4:\n\t"
        "mov r1, #0\n\t"
        "mov r3, #0\n\t"
    "5:\n\t"
        "lsl r1, r1, #0x11\n\t"
        "ldrh r0, [r2]\n\t"
        "and r0, r6\n\t"
        "lsr r1, r1, #0x10\n\t"
        "orr r1, r0\n\t"
        "add r2, #2\n\t"
        "add r0, r3, #1\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r3, r0, #0x18\n\t"
        "cmp r3, #0xf\n\t"
        "bls 5b\n\t"
        "strh r1, [r5]\n\t"
        "sub r5, #2\n\t"
        "add r0, r4, #1\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r4, r0, #0x18\n\t"
        "cmp r4, #3\n\t"
        "bls 4b\n\t"
        "mov r0, #0\n\t"
    "6:\n\t"
        "add sp, #0x88\n\t"
        "pop {r4, r5, r6}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
    "9: .4byte gUnknown_03001634\n"
    );
}

/* Writes one 8-byte (64-bit) block `src[0..3]` to EEPROM at `addr`,
 * then arms a watchdog timer (`sub_803AA08` with `gStaticData_085A9F10`
 * as the reload/config) and busy-waits on the EEPROM data port's ready
 * bit until either it goes ready or the timer's IRQ handler flags a
 * timeout (`gUnknown_03001624`), stopping the timer either way before
 * returning. Returns 0x80FF if `addr` is out of range, or 0xC001 on a
 * watchdog timeout.
 *
 * Written as NAKED asm: an earlier pass's plain-C reconstruction (see
 * docs/matching/issue-69-eeprom-timer.md) confirmed every field/
 * register access against the ROM, but the busy-wait tail hit the same
 * loop-rotation gap `sub_803AAD4` had - this compiler restructures the
 * `if (cond) break; if (flag) { if (cond) break; result = ...; break;
 * }` shape into extra basic blocks the ROM doesn't have. Since every
 * instruction was already independently confirmed correct, this is a
 * mechanical, byte-verified transcription of the ROM's own instructions
 * - see sub_803AB54's comment above for the syntax-translation/label-
 * renumbering convention used. */
NAKED s32 sub_803AC04(u16 addr, u16 *src)
{
    asm(
        "push {r4, r5, lr}\n\t"
        "sub sp, #0xa4\n\t"
        "add r5, r1, #0\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r4, r0, #0x10\n\t"
        "ldr r0, 9f\n\t"
        "ldr r0, [r0]\n\t"
        "ldrh r0, [r0, #4]\n\t"
        "cmp r4, r0\n\t"
        "blo 1f\n\t"
        "ldr r0, 10f\n\t"
        "b 8f\n\t"
        ".align 2, 0\n"
    "9: .4byte gUnknown_03001634\n"
    "10: .4byte 0x000080FF\n"
    "1:\n\t"
        "ldr r0, 11f\n\t"
        "ldr r0, [r0]\n\t"
        "ldrb r0, [r0, #8]\n\t"
        "lsl r0, r0, #1\n\t"
        "mov r1, sp\n\t"
        "add r3, r0, r1\n\t"
        "add r3, #0x84\n\t"
        "mov r0, #0\n\t"
        "strh r0, [r3]\n\t"
        "sub r3, #2\n\t"
        "mov r1, #0\n\t"
    "2:\n\t"
        "ldrh r2, [r5]\n\t"
        "add r5, #2\n\t"
        "mov r0, #0\n\t"
    "3:\n\t"
        "strh r2, [r3]\n\t"
        "sub r3, #2\n\t"
        "lsr r2, r2, #1\n\t"
        "add r0, #1\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "cmp r0, #0xf\n\t"
        "bls 3b\n\t"
        "add r0, r1, #1\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r1, r0, #0x18\n\t"
        "cmp r1, #3\n\t"
        "bls 2b\n\t"
        "mov r1, #0\n\t"
        "ldr r0, 11f\n\t"
        "add r2, r0, #0\n\t"
        "ldr r0, [r0]\n\t"
        "b 4f\n\t"
        ".align 2, 0\n"
    "11: .4byte gUnknown_03001634\n"
    "5:\n\t"
        "strh r4, [r3]\n\t"
        "sub r3, #2\n\t"
        "lsr r4, r4, #1\n\t"
        "add r0, r1, #1\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r1, r0, #0x18\n\t"
        "ldr r0, [r2]\n\t"
    "4:\n\t"
        "ldrb r0, [r0, #8]\n\t"
        "cmp r1, r0\n\t"
        "blo 5b\n\t"
        "mov r0, #0\n\t"
        "strh r0, [r3]\n\t"
        "sub r3, #2\n\t"
        "mov r0, #1\n\t"
        "strh r0, [r3]\n\t"
        "mov r1, #0xd0\n\t"
        "lsl r1, r1, #0x14\n\t"
        "ldr r0, 12f\n\t"
        "ldr r0, [r0]\n\t"
        "ldrb r2, [r0, #8]\n\t"
        "add r2, #0x43\n\t"
        "mov r0, sp\n\t"
        "bl sub_803AAD4\n\t"
        "ldr r0, 13f\n\t"
        "bl sub_803AA08\n\t"
        "mov r4, #0\n\t"
        "mov r1, #0xd0\n\t"
        "lsl r1, r1, #0x14\n\t"
        "mov r3, #1\n\t"
        "ldr r2, 14f\n\t"
    "6:\n\t"
        "ldrh r0, [r1]\n\t"
        "and r0, r3\n\t"
        "cmp r0, #0\n\t"
        "bne 7f\n\t"
        "ldrb r0, [r2]\n\t"
        "cmp r0, #0\n\t"
        "beq 6b\n\t"
        "ldrh r0, [r1]\n\t"
        "mov r1, #1\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "bne 7f\n\t"
        "ldr r4, 15f\n\t"
    "7:\n\t"
        "bl sub_803AA90\n\t"
        "add r0, r4, #0\n\t"
    "8:\n\t"
        "add sp, #0xa4\n\t"
        "pop {r4, r5}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
    "12: .4byte gUnknown_03001634\n"
    "13: .4byte gStaticData_085A9F10\n"
    "14: .4byte gUnknown_03001624\n"
    "15: .4byte 0x0000C001\n"
    );
}
