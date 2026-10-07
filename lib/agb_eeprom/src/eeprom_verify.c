#include "agb_eeprom_internal.h"

/* EEPROMCompare and EEPROMWrite1_check of Nintendo's AgbEeprom SDK
 * library ("EEPROM_V122", 0x0803A968-0x0803AD7C), after the read/write
 * primitives in eeprom_read_write.c. Built at **-O1** like the rest
 * of the library (Makefile O1_OBJS): both are the SDK's plain C, the
 * shape zeldaret/tmc's src/eeprom.c reconstructed for EEPROM_V124, and
 * byte-identical at -O1. See docs/matching/eeprom-sdk-o1.md. */

/* SDK EEPROMCompare: reads block `addr` back and compares it with
 * `data[0..3]`. Returns 0x80FF if `addr` is out of range, 0x8000 on a
 * mismatch, 0 if equal. The out-of-range case has to be an early
 * `return` (TMC's if/else puts 0x80FF into `result`'s register, 2
 * halfwords off). */
s32 EEPROMCompare(u16 addr, u16 *data)
{
    u16 result;
    u8 i;
    u16 buffer[4];
    u16 *ptr;

    result = 0;
    if (addr >= gEepromConfig->maxCount)
        return 0x80FF;

    EEPROMRead(addr, buffer);
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
s32 EEPROMWrite1_check(u16 addr, u16 *data)
{
    u8 i;
    /* Self-initialized to silence -Wuninitialized (the loop always runs,
     * gcc can't tell): `= 0` adds a store to the SDK code (#577). */
    u16 result = result;

    for (i = 0; i < 3; i++) {
        result = EEPROMWrite(addr, data);
        if (result == 0) {
            result = EEPROMCompare(addr, data);
            if (result == 0)
                break;
        }
    }
    return result;
}
