#include "core.h"

/* A DMA3 bit-serial GBA EEPROM save-chip read/write pair, sitting
 * right after the DMA3 transfer helper `sub_803AAD4`
 * (src/system/timer_util_aa90.c) and before the matched verify/retry
 * pair in src/system/eeprom_verify.c. See
 * docs/matching/issue-69-eeprom-timer.md and
 * docs/matching/eeprom-sdk-o1.md.
 *
 * Both are Nintendo's AgbEeprom SDK library C (the ROM carries its
 * "EEPROM_V122" version string), and this object is built with **-O1**
 * (Makefile O1_OBJS): with the SDK's own source shape - the same one
 * zeldaret/tmc reconstructed for EEPROM_V124's `EEPROMRead`/
 * `EEPROMWrite` - both are byte-identical at -O1 with no pins. At -O2
 * the same C is off in the register plan of the bit-unpack loop and
 * the busy-wait tail, which is what kept them NAKED before.
 *
 * Wire protocol: a read sends 2 start bits ("11"), the chip's
 * `addrBitCount` address bits (MSB first) and one more (uninitialized)
 * bit, then reads back 0x44 (68) halfwords - 4 dummy bits, then 64 data
 * bits. A write sends "10", the address bits, 64 data bits (MSB first
 * per 16-bit word) and a trailing "0". Only bit 0 of each 16-bit DMA
 * slot matters to the hardware. */

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

/* The EEPROM's memory window; reading it returns the chip's ready bit. */
#define EEPROM_PORT ((u16 *)0x0D000000)
#define REG_EEPROM (*(vu16 *)0x0D000000)

/* SDK EEPROMRead: reads one 8-byte (64-bit) block at `address` into
 * `data[0..3]`. Returns 0x80FF if `address` is out of range for the
 * selected chip. */
s32 sub_803AB54(u16 address, u16 *data)
{
    u16 buffer[0x44];
    u16 *ptr;
    u8 t1, t2;
    u16 value;

    if (address >= gUnknown_03001634->maxCount) {
        return 0x80FF;
    } else {
        /* The byte-offset spelling (address bits + 1, then + 1) is what
         * reproduces the ROM's `lsl; add sp; add #2` order;
         * `&buffer[n + 1]` is 1-3 halfwords off. */
        ptr = (u16 *)((u8 *)buffer + ((gUnknown_03001634->addrBitCount << 1) + 1) + 1);
        for (t1 = 0; t1 < gUnknown_03001634->addrBitCount; t1++) {
            *(ptr--) = address;
            address >>= 1;
        }
        /* read request "11" */
        *(ptr--) = 1;
        *ptr = 1;
        sub_803AAD4(buffer, EEPROM_PORT, gUnknown_03001634->addrBitCount + 3);
        sub_803AAD4(EEPROM_PORT, buffer, 0x44);
        /* skip the 4 dummy bits, unpack 4 words MSB first */
        ptr = buffer + 4;
        data += 3;
        for (t1 = 0; t1 < 4; t1++) {
            value = 0;
            for (t2 = 0; t2 < 0x10; t2++) {
                value <<= 1;
                value |= (*ptr++) & 1;
            }
            *(data--) = value;
        }
        return 0;
    }
}

/* SDK EEPROMWrite (V122, timer-watchdog version): writes one 8-byte
 * block `data[0..3]` to EEPROM at `address`, then arms the watchdog
 * timer (`sub_803AA08` = StartEepromTimer with the
 * `gStaticData_085A9F10` timeout table) and busy-waits for the chip's
 * ready bit until either it goes ready or the timer's IRQ handler sets
 * the timeout flag `gUnknown_03001624`, stopping the timer
 * (`sub_803AA90`) either way. Returns 0x80FF if `address` is out of
 * range, 0xC001 on a timeout. */
s32 sub_803AC04(u16 address, u16 *data)
{
    u16 buffer[0x52];
    u16 ret;
    u32 bits;
    u8 i, j;
    u16 *ptr;

    if (address >= gUnknown_03001634->maxCount)
        return 0x80FF;

    /* Same byte-offset spelling as TMC's reconstruction: it gives the
     * ROM's `lsl; add sp; add #0x84` order. */
    ptr = (u16 *)(0x42 + (u32)buffer + (u32)(gUnknown_03001634->addrBitCount * 2) + 0x42);
    /* stop bit */
    *ptr-- = 0;
    for (i = 0; i < 4; i++) {
        bits = *data++;
        for (j = 0; j < 16; j++) {
            *ptr = bits;
            ptr--;
            bits = bits >> 1;
        }
    }
    for (i = 0; i < gUnknown_03001634->addrBitCount; i++) {
        *ptr = address;
        ptr--;
        address = address >> 1;
    }
    /* write request "10" */
    *ptr-- = 0;
    *ptr-- = 1;
    sub_803AAD4(buffer, EEPROM_PORT, gUnknown_03001634->addrBitCount + 0x43);
    sub_803AA08(gStaticData_085A9F10);
    ret = 0;
    while (1) {
        if (REG_EEPROM & 1)
            break;
        if (gUnknown_03001624) {
            if (REG_EEPROM & 1)
                break;
            ret = 0xC001;
            break;
        }
    }
    sub_803AA90();
    return ret;
}
