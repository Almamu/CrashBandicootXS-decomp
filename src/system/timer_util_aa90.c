#include "core.h"

/* Part of Nintendo's AgbEeprom SDK library (the ROM carries its
 * "EEPROM_V122" version string at 0x085A9EEC), which spans
 * 0x0803A968-0x0803AD7C: EEPROM type select, the watchdog timer
 * start/stop pair, the DMA3 transfer helper, read, write, compare and
 * write-with-verify. This object is built with **-O1**, not the game's
 * -O2 (see the Makefile's O1_OBJS comment and
 * docs/matching/eeprom-sdk-o1.md): both functions below are the SDK's
 * plain C, byte-identical at -O1 with no register pins or volatile
 * tricks. At -O2 `sub_803AAD4`'s duplicated DMA-wait test gets
 * cross-jumped into the loop (43 halfwords off), which is what kept it
 * NAKED before.
 *
 * Sits at its own real ROM address (0x0803AA90) after `sub_803AA08`
 * (src/system/timer_util.c), so it needs its own translation unit - see
 * docs/workflow.md step 4's "needs its own new .c file" case. */

struct EepromConfig {
    u32 unk0;
    u16 maxCount;
    u16 waitcntBits;
    u8 addrBitCount;
    u8 pad[3];
};

extern u8 gUnknown_03001620;
extern vu16 *gUnknown_03001628;
extern u16 gUnknown_0300162C;
extern struct EepromConfig *gUnknown_03001634;

/* SDK StopEepromTimer: the exact inverse of `sub_803AA08` - stops the
 * claimed timer (CNT_L then CNT_H through the saved register pointer),
 * disables its IRQ, restores IME. Same shape as pokeemerald's
 * agb_flash `StopFlashTimer`. */
void sub_803AA90(void)
{
    REG_IME = 0;
    *gUnknown_03001628++ = 0;
    *gUnknown_03001628-- = 0;
    REG_IE &= ~(INTR_FLAG_TIMER0 << gUnknown_03001620);
    REG_IME = gUnknown_0300162C;
}

/* SDK DMA3Transfer (static in the SDK): saves and clears IME, merges
 * the active `EepromConfig`'s `waitcntBits` into WAITCNT's wait-state-2
 * field, starts a one-shot 16-bit DMA3 transfer of `count` units and
 * waits for it to finish, then restores IME. Used by the EEPROM
 * bit-serial read/write in src/system/eeprom_util.c. Same source as
 * zeldaret/tmc's src/eeprom.c `DMA3Transfer` (EEPROM_V124). The ROM's
 * test-before-loop plus separate in-loop test is gcc's duplicated
 * `while` exit test, which -O1 keeps apart. */
void sub_803AAD4(const void *src, void *dst, u16 count)
{
    u16 ime;

    ime = REG_IME;
    REG_IME = 0;
    REG_WAITCNT = (REG_WAITCNT & 0xF8FF) | gUnknown_03001634->waitcntBits;
    REG_DMA3SAD = (u32)src;
    REG_DMA3DAD = (u32)dst;
    REG_DMA3CNT = count | 0x80000000;
    while (REG_DMA3CNT_H & 0x8000)
        ;
    REG_IME = ime;
}
