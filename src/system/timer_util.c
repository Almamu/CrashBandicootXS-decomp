#include "core.h"

/* BIOS SWI wrappers, thin single-instruction trampolines, named after the
 * BIOS calls they make (the libagbsyscall names pokeemerald and the other
 * GBA decomps use). The callers declare whatever prototype they need. */

NAKED void BgAffineSet(void) /* SWI 0xE */
{
    asm("svc #0xe\n\tbx lr");
}

NAKED void CpuFastSet(void) /* SWI 0xC */
{
    asm("svc #0xc\n\tbx lr");
}

NAKED void CpuSet(void) /* SWI 0xB */
{
    asm("svc #0xb\n\tbx lr");
}

NAKED void LZ77UnCompVram(void) /* SWI 0x12 */
{
    asm("svc #0x12\n\tbx lr");
}

NAKED void ObjAffineSet(void) /* SWI 0xF */
{
    asm("svc #0xf\n\tbx lr");
}

NAKED void RLUnCompVram(void) /* SWI 0x15 */
{
    asm("svc #0x15\n\tbx lr");
}

NAKED void Sqrt(void) /* SWI 8 */
{
    asm("svc #8\n\tbx lr");
}

NAKED void VBlankIntrWait(void) /* SWI 5, with r2 zeroed first */
{
    asm("movs r2, #0\n\tsvc #5\n\tbx lr");
}

/* Everything from here on is Nintendo's AgbEeprom SDK library (the ROM's
 * "EEPROM_V122", 0x0803A968-0x0803AD7C), which was compiled at **-O1**:
 * this object is on the Makefile's O1_OBJS list. The four functions
 * below are the SDK's plain C (the shapes zeldaret/tmc's src/eeprom.c
 * and pokeemerald's agb_flash.c `FlashTimerIntr`/`SetFlashTimerIntr`/
 * `StartFlashTimer` use) and are byte-identical at -O1 with no pins,
 * volatile globals or barriers. The BIOS SWI wrappers above are
 * hand-written and come out the same under any flag. See
 * docs/matching/eeprom-sdk-o1.md.
 *
 * `struct EepromConfig` is the SDK's EEPROMConfig: `size` is the chip
 * size in bytes, `maxCount` the number of 8-byte blocks (0x40 / 0x400),
 * `waitcntBits` the WAITCNT wait-state-2 value and `addrBitCount` the
 * chip's address width (6 or 14). The globals keep TMC's
 * (`gEEPROMConfig512`/`gEEPROMConfig8k`/`gEEPROMConfig`) and agb_flash's
 * (timer number, countdown, timer register, saved IME, timeout flag)
 * roles. */
struct EepromConfig {
    u32 size;
    u16 maxCount;
    u16 waitcntBits;
    u8 addrBitCount;
    u8 pad[3];
};

extern struct EepromConfig gEepromConfig512; /* 512-byte (4 Kbit) chip */
extern struct EepromConfig gEepromConfig8k;  /* 8-KB (64 Kbit) chip */
extern struct EepromConfig *gEepromConfig;   /* active config */

extern u8 gEepromTimerNum;      /* claimed timer number */
extern u16 gEepromTimerCount;   /* timeout countdown */
extern u8 gEepromTimeoutFlag;   /* timeout flag */
extern vu16 *gEepromTimerReg;   /* claimed timer's TMxCNT_L */
extern u16 gEepromSavedIme;     /* IME saved by StartEepromTimer */

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
 * inverse is `StopEepromTimer` (src/system/timer_util_aa90.c). */
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
