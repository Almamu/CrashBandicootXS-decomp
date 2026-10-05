#include "agb_eeprom_internal.h"

/* The start of Nintendo's AgbEeprom SDK library (the ROM's
 * "EEPROM_V122", 0x0803A968-0x0803AD7C), right after libagbsyscall's
 * BIOS SWI wrappers. The library was compiled at **-O1**: every object
 * of lib/agb_eeprom is on the Makefile's O1_OBJS list. The four
 * functions below are the SDK's plain C (the shapes zeldaret/tmc's
 * src/eeprom.c and pokeemerald's agb_flash.c `FlashTimerIntr`/
 * `SetFlashTimerIntr`/`StartFlashTimer` use) and are byte-identical at
 * -O1 with no pins, volatile globals or barriers. See
 * docs/matching/eeprom-sdk-o1.md. */

/* SDK EEPROMConfigure / IdentifyEeprom: picks the chip's config by its
 * size code (4 = 4 Kbit, 0x40 = 64 Kbit); anything else falls back to
 * the 512-byte chip and reports failure. At -O1 each branch keeps its
 * own literal pool, which is the ROM's layout. */
s32 EEPROMConfigure(u16 type)
{
    u16 result;

    result = 0;
    if (type == 4) {
        gEepromConfig = &gEepromConfig512;
    } else if (type == 0x40) {
        gEepromConfig = &gEepromConfig8k;
    } else {
        gEepromConfig = &gEepromConfig512;
        result = 1;
    }
    return result;
}

/* SDK EepromTimerIntr (pokeemerald's FlashTimerIntr): the timer IRQ
 * handler `SetEepromTimerIntr` hands out. Counts the timeout down and sets the
 * timeout flag that `EEPROMWrite`'s busy-wait polls. Never called
 * directly, only through the pointer. (Was kept as a raw `.byte` blob
 * before; it is ordinary compiled C.) */
void EepromTimerIntr(void)
{
    if (gEepromTimerCount != 0 && --gEepromTimerCount == 0)
        gEepromTimeoutFlag = 1;
}

/* SDK SetEepromTimerIntr (pokeemerald's SetFlashTimerIntr): claims
 * hardware timer `timerNum` (0-3), points `gEepromTimerReg` at its
 * TMxCNT_L and returns the IRQ handler for the caller to install. */
s32 SetEepromTimerIntr(u8 timerNum, void (**intrFunc)(void))
{
    if (timerNum >= 4)
        return 1;
    gEepromTimerNum = timerNum;
    gEepromTimerReg = &REG_TMCNT(gEepromTimerNum);
    *intrFunc = EepromTimerIntr;
    return 0;
}

/* SDK StartEepromTimer (pokeemerald's StartFlashTimer, with the IF
 * acknowledge before the IE enable): saves and clears IME, stops the
 * timer, acks and enables its IRQ, clears the timeout flag, loads the
 * {countdown, reload, control} triple from `maxTime` and sets IME. The
 * inverse is `StopEepromTimer` (eeprom_timer_stop.c). */
void StartEepromTimer(const u16 *maxTime)
{
    gEepromSavedIme = REG_IME;
    REG_IME = 0;
    gEepromTimerReg[1] = 0;
    REG_IF = INTR_FLAG_TIMER0 << gEepromTimerNum;
    REG_IE |= INTR_FLAG_TIMER0 << gEepromTimerNum;
    gEepromTimeoutFlag = 0;
    gEepromTimerCount = *maxTime++;
    *gEepromTimerReg++ = *maxTime++;
    *gEepromTimerReg-- = *maxTime++;
    REG_IME = 1;
}
