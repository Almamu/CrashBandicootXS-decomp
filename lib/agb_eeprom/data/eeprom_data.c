#include "../src/agb_eeprom_internal.h"

/*
 * ROM 0x085A9EEC-0x085A9F70: the AGB SDK EEPROM library's constant data
 * (the library is eeprom_timer.c, eeprom_timer_stop.c and
 * eeprom_read_write.c). Linked in ROM order between data/data.s sections by
 * ldscript.txt - see docs/data.md.
 */

/* The library's identification string. No code reads it: flashing tools
 * and emulators search the ROM for "EEPROM_V" to learn the save type. */
const char gEepromLibraryVersion[] = "EEPROM_V122";

/* EEPROMConfigure (eeprom_timer.c) points gEepromConfig at one of these by
 * the chip type it is given. */
const struct EepromConfig gEepromConfig512 = { 0x200, 0x40, 0x300, 6 };    /* 4 Kbit */
const struct EepromConfig gEepromConfig8k = { 0x2000, 0x400, 0x300, 14 }; /* 64 Kbit */

/* The write timeout EEPROMWrite (eeprom_read_write.c) passes to
 * StartEepromTimer: {countdown, TMxCNT_L reload,
 * TMxCNT_H control}. */
const u16 gEepromMaxTime[3] = { 10, 0xFFBD, 0xC2 };

/* The library's relocatable address constants, function by function:
 * each function's literal-pool words that are symbol addresses (I/O
 * registers and plain numbers left out), without repeats -
 * EEPROMConfigure (3), EepromTimerIntr (2), SetEepromTimerIntr (3),
 * StartEepromTimer (5), StopEepromTimer (3), DMA3Transfer, EEPROMRead
 * (1 each), EEPROMWrite (3), EEPROMCompare (1). No code reads this copy (nothing in the ROM points at
 * it); it is written as symbol references so the variables and code it
 * names can still move. */
const void *const gEepromLibraryAddresses[22] = {
    &gEepromConfig,
    &gEepromConfig512,
    &gEepromConfig8k,
    &gEepromTimerCount,
    &gEepromTimeoutFlag,
    &gEepromTimerNum,
    &gEepromTimerReg,
    (const void *)EepromTimerIntr,
    &gEepromSavedIme,
    &gEepromTimerReg,
    &gEepromTimerNum,
    &gEepromTimeoutFlag,
    &gEepromTimerCount,
    &gEepromTimerReg,
    &gEepromTimerNum,
    &gEepromSavedIme,
    &gEepromConfig,
    &gEepromConfig,
    &gEepromConfig,
    gEepromMaxTime,
    &gEepromTimeoutFlag,
    &gEepromConfig,
};
