#ifndef __AGB_EEPROM_H__
#define __AGB_EEPROM_H__

#include "gba/types.h"

/* Nintendo's AgbEeprom SDK library, version EEPROM_V122 (the ROM's
 * "EEPROM_V122" string, data/eeprom_5a9eec.c): serial EEPROM save-chip
 * access through DMA3, with a hardware timer as the write watchdog.
 * The library's code is at 0x0803A968-0x0803AD7C and was compiled at
 * -O1 (docs/matching/eeprom-sdk-o1.md). Names and shapes follow
 * zeldaret/tmc's src/eeprom.c (EEPROM_V124) and pokeemerald's agb_flash.
 *
 * Usage, as the game's save code does it: pick the chip with
 * EEPROMConfigure(4) once, install the watchdog handler that
 * SetEepromTimerIntr() returns in the timer's IRQ slot, then read or
 * write 8-byte blocks 0..gEepromConfig->maxCount-1. */

/* The SDK's EEPROMConfig. */
struct EepromConfig {
    u32 size;        /* chip size in bytes */
    u16 maxCount;    /* number of 8-byte blocks */
    u16 waitcntBits; /* WAITCNT wait-state 2 setting */
    u8 addrBitCount; /* address bits in a request (6 or 14) */
    u8 pad[3];
};

/* The active chip's config (EEPROMConfigure). */
extern const struct EepromConfig *gEepromConfig;

/* Selects the chip by size code: 4 = 4 Kbit (512 bytes), 0x40 = 64 Kbit
 * (8 KB). Anything else selects the 512-byte chip and returns 1. */
s32 EEPROMConfigure(u16 type);

/* Claims hardware timer `timerNum` (0-3) as the write watchdog and
 * stores its IRQ handler in `*intrFunc`. Returns 1 if `timerNum` is out
 * of range. */
s32 SetEepromTimerIntr(u8 timerNum, void (**intrFunc)(void));

/* Block access: one 8-byte block (4 halfwords) at `address`. All
 * return 0 on success, 0x80FF for an out-of-range address. */
s32 EEPROMRead(u16 address, u16 *data);
s32 EEPROMWrite(u16 address, u16 *data);
/* 0x8000 if the block differs from `data`. */
s32 EEPROMCompare(u16 address, u16 *data);
/* EEPROMWrite then EEPROMCompare, up to 3 attempts. */
s32 EEPROMWrite1_check(u16 address, u16 *data);

#endif /* __AGB_EEPROM_H__ */
