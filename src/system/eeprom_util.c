#include "core.h"

/* A DMA3 bit-serial GBA EEPROM save-chip read/write cluster, sitting
 * right after the DMA3 transfer helper `sub_803AAD4`
 * (asm/code_3_2_20e_aa90.s) and before the matched verify/retry pair in
 * src/system/eeprom_verify.c - its own file per docs/workflow.md step
 * 4's "needs its own new .c file" case, since it isn't adjacent to any
 * already-matched file's functions. See docs/matching/issue-69-*.md.
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

#define EEPROM_PORT ((void *)0x0D000000)

#if NON_MATCHING
/* NOT YET BYTE-MATCHING: reads one 8-byte (64-bit) EEPROM block at
 * `addr` into `dest[0..3]`. Returns 0x80FF if `addr` is out of range
 * for the currently-selected chip. Semantics and every field/register
 * access confirmed against the ROM (see the file header comment for
 * the wire protocol); what resists matching is the tail's bit-unpack
 * loop - the ROM keeps the "mask = 1" constant and the outer-loop
 * counter in specific low registers reused from the address-packing
 * phase above (`r6`/`r4`), where this compiler's allocator runs out of
 * low registers at that point and spills the mask into `r12` instead,
 * plus hoists the read-back/dest-pointer setup earlier than the ROM
 * does (both loop-invariant, so this compiler's scheduler moves them
 * up across the two `sub_803AAD4` calls even though they're plain
 * function calls). */
s32 sub_803AB54(u16 addr, u16 *dest)
{
    u16 buf[68];
    u8 n;
    u8 i;
    u16 v;
    u16 *p;

    if (addr >= gUnknown_03001634->maxCount) {
        return 0x80FF;
    }

    n = gUnknown_03001634->addrBitCount;
    p = &buf[n + 1];
    v = addr;
    i = 0;
    while (i < n) {
        *p = v;
        p--;
        v >>= 1;
        i++;
        n = gUnknown_03001634->addrBitCount;
    }
    *p = 1;
    p--;
    *p = 1;

    sub_803AAD4(buf, EEPROM_PORT, gUnknown_03001634->addrBitCount + 3);
    sub_803AAD4(EEPROM_PORT, buf, 0x44);

    p = &buf[4];
    dest += 3;
    for (i = 0; i < 4; i++) {
        u16 word;
        u8 j;

        word = 0;
        for (j = 0; j <= 0xf; j++) {
            word <<= 1;
            word |= *p & 1;
            p++;
        }
        *dest = word;
        dest--;
    }
    return 0;
}

/* NOT YET BYTE-MATCHING: writes one 8-byte (64-bit) block `src[0..3]`
 * to EEPROM at `addr`, then arms a watchdog timer (`sub_803AA08` with
 * `gStaticData_085A9F10` as the reload/config) and busy-waits on the
 * EEPROM data port's ready bit until either it goes ready or the
 * timer's IRQ handler flags a timeout (`gUnknown_03001624`), stopping
 * the timer either way before returning. Returns 0x80FF if `addr` is
 * out of range, or 0xC001 on a watchdog timeout. Semantics and every
 * field/register access confirmed against the ROM; the busy-wait tail
 * hits the same loop-rotation gap `sub_803AAD4` (src/system/
 * timer_util.c) already has an unresolved entry for - this compiler
 * restructures the `if (cond) break; if (flag) { if (cond) break;
 * result = ...; break; }` shape into extra basic blocks the ROM
 * doesn't have, rather than the ROM's simpler linear flow. */
s32 sub_803AC04(u16 addr, u16 *src)
{
    u16 buf[82];
    u8 n;
    u8 i;
    u16 v;
    u16 *p;
    s32 result;

    if (addr >= gUnknown_03001634->maxCount) {
        return 0x80FF;
    }

    n = gUnknown_03001634->addrBitCount;
    p = &buf[n + 66];
    *p = 0;
    p--;

    i = 0;
    while (i <= 3) {
        u16 word;
        u8 j;

        word = *src;
        src++;
        j = 0;
        while (j <= 0xf) {
            *p = word;
            p--;
            word >>= 1;
            j++;
        }
        i++;
    }

    i = 0;
    v = addr;
    while (i < gUnknown_03001634->addrBitCount) {
        *p = v;
        p--;
        v >>= 1;
        i++;
    }
    *p = 0;
    p--;
    *p = 1;

    sub_803AAD4(buf, EEPROM_PORT, gUnknown_03001634->addrBitCount + 0x43);
    sub_803AA08(gStaticData_085A9F10);

    result = 0;
    while (1) {
        if (*(vu16 *)EEPROM_PORT & 1) {
            break;
        }
        if (gUnknown_03001624 != 0) {
            if (*(vu16 *)EEPROM_PORT & 1) {
                break;
            }
            result = 0xC001;
            break;
        }
    }
    sub_803AA90();
    return result;
}
#endif
