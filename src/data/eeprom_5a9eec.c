#include "core.h"

/*
 * ROM 0x085A9EEC-0x085A9F70: the AGB SDK EEPROM library's constant data
 * (the library is src/system/timer_util.c, timer_util_aa90.c and
 * eeprom_util.c). Linked in ROM order between data/data.s sections by
 * ldscript.txt - see docs/data.md.
 */

/* The library's identification string. No code reads it: flashing tools
 * and emulators search the ROM for "EEPROM_V" to learn the save type. */
const char gEepromLibraryVersion[] = "EEPROM_V122";

/* The consumers' `struct EepromConfig` (timer_util.c, eeprom_util.c,
 * eeprom_verify.c, ...). */
struct EepromConfig {
    u32 size;           /* bytes */
    u16 maxCount;       /* number of 8-byte blocks */
    u16 waitcntBits;    /* WAITCNT wait-state 2 setting */
    u8 addrBitCount;    /* address bits in a request */
    u8 pad[3];
};

/* sub_803A968 (timer_util.c) points gUnknown_03001634 at one of these by
 * the chip type it is given. */
const struct EepromConfig gStaticData_085A9EF8 = { 0x200, 0x40, 0x300, 6 };    /* 4 Kbit */
const struct EepromConfig gStaticData_085A9F04 = { 0x2000, 0x400, 0x300, 14 }; /* 64 Kbit */

/* The write timeout sub_803AC04 (eeprom_util.c) passes to
 * sub_803AA08 (StartEepromTimer): {countdown, TMxCNT_L reload,
 * TMxCNT_H control}. */
const u16 gStaticData_085A9F10[3] = { 10, 0xFFBD, 0xC2 };

extern u8 gUnknown_03001620;
extern u8 gUnknown_03001622;
extern u8 gUnknown_03001624;
extern u8 gUnknown_03001628;
extern u8 gUnknown_0300162C;
extern u8 gUnknown_03001634;
extern void sub_803A9AC();

/* The library's relocatable address constants, function by function:
 * each function's literal-pool words that are symbol addresses (I/O
 * registers and plain numbers left out), without repeats -
 * sub_803A968 (3), sub_803A9AC (2), sub_803A9D0 (3), sub_803AA08 (5),
 * sub_803AA90 (3), sub_803AAD4, sub_803AB54 (1 each), sub_803AC04 (3),
 * sub_803ACE0 (1). No code reads this copy (nothing in the ROM points at
 * it); it is written as symbol references so the variables and code it
 * names can still move. */
const void *const gEepromLibraryAddresses[22] = {
    &gUnknown_03001634,
    &gStaticData_085A9EF8,
    &gStaticData_085A9F04,
    &gUnknown_03001622,
    &gUnknown_03001624,
    &gUnknown_03001620,
    &gUnknown_03001628,
    (const void *)sub_803A9AC,
    &gUnknown_0300162C,
    &gUnknown_03001628,
    &gUnknown_03001620,
    &gUnknown_03001624,
    &gUnknown_03001622,
    &gUnknown_03001628,
    &gUnknown_03001620,
    &gUnknown_0300162C,
    &gUnknown_03001634,
    &gUnknown_03001634,
    &gUnknown_03001634,
    gStaticData_085A9F10,
    &gUnknown_03001624,
    &gUnknown_03001634,
};
