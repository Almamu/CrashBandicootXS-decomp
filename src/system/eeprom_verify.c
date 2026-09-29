#include "core.h"

/* EEPROMCompare and EEPROMWrite1_check of Nintendo's AgbEeprom SDK
 * library ("EEPROM_V122", 0x0803A968-0x0803AD7C), after the read/write
 * primitives in src/system/eeprom_util.c. Built at **-O1** like the rest
 * of the library (Makefile O1_OBJS): both are the SDK's plain C, the
 * shape zeldaret/tmc's src/eeprom.c reconstructed for EEPROM_V124, and
 * byte-identical at -O1. See docs/matching/eeprom-sdk-o1.md. */

/* The SDK's EEPROMConfig; same layout as in src/system/timer_util.c. */
struct EepromConfig {
    u32 unk0;
    u16 maxCount;
    u16 waitcntBits;
    u8 addrBitCount;
    u8 pad[3];
};

extern struct EepromConfig *gUnknown_03001634;

extern s32 sub_803AB54(u16 addr, u16 *dest);
extern s32 sub_803AC04(u16 addr, u16 *src);

/* SDK EEPROMCompare: reads block `addr` back and compares it with
 * `data[0..3]`. Returns 0x80FF if `addr` is out of range, 0x8000 on a
 * mismatch, 0 if equal. The out-of-range case has to be an early
 * `return` (TMC's if/else puts 0x80FF into `result`'s register, 2
 * halfwords off). */
s32 sub_803ACE0(u16 addr, u16 *data)
{
    u16 result;
    u8 i;
    u16 buffer[4];
    u16 *ptr;

    result = 0;
    if (addr >= gUnknown_03001634->maxCount)
        return 0x80FF;

    sub_803AB54(addr, buffer);
    ptr = buffer;
    for (i = 0; i < 4; i++) {
        if (*data++ != *ptr++) {
            result = 0x8000;
            break;
        }
    }
    return result;
}

/* SDK EEPROMWrite1_check: write + compare, up to 3 attempts; returns
 * the last attempt's status (0 on success). */
s32 sub_803AD38(u16 addr, u16 *data)
{
    u8 i;
    u16 result;

    for (i = 0; i < 3; i++) {
        result = sub_803AC04(addr, data);
        if (result == 0) {
            result = sub_803ACE0(addr, data);
            if (result == 0)
                break;
        }
    }
    return result;
}
/* Pads the object to 4 bytes with zeros: the libgcc `_call_via_r0`
 * (src/system/reg_trampolines.c) that follows starts at 0x0803AD78. */
asm(".align 2, 0");
