#include "core.h"

/* BIOS SWI wrappers, thin single-instruction trampolines. Real GBA BIOS
 * call numbers, per the standard swi list. */

NAKED void sub_803A944(void) /* BgAffineSet (SWI 0xE) */
{
    asm("svc #0xe\n\tbx lr");
}

NAKED void sub_803A948(void) /* CpuFastSet (SWI 0xC) */
{
    asm("svc #0xc\n\tbx lr");
}

NAKED void sub_803A94C(void) /* CpuSet (SWI 0xB) */
{
    asm("svc #0xb\n\tbx lr");
}

NAKED void LZ77UnCompWrapper(void) /* LZ77UnCompVram (SWI 0x12) */
{
    asm("svc #0x12\n\tbx lr");
}

NAKED void sub_803A954(void) /* ObjAffineSet (SWI 0xF) */
{
    asm("svc #0xf\n\tbx lr");
}

NAKED void RLUnCompWrapper(void) /* RLUnCompVram (SWI 0x15) */
{
    asm("svc #0x15\n\tbx lr");
}

NAKED void sub_803A95C(void) /* Sqrt (SWI 8) */
{
    asm("svc #8\n\tbx lr");
}

NAKED void sub_0803A960(void) /* VBlankIntrWait (SWI 5), with r2 zeroed first */
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
 * `struct EepromConfig` is the SDK's EEPROMConfig: `maxCount` is the
 * number of 8-byte blocks (0x40 / 0x400), `waitcntBits` the WAITCNT
 * wait-state-2 value and `addrBitCount` the chip's address width (6 or
 * 14). */
struct EepromConfig {
    u32 unk0;
    u16 maxCount;
    u16 waitcntBits;
    u8 addrBitCount;
    u8 pad[3];
};

extern struct EepromConfig gStaticData_085A9EF8; /* 512-byte (4 Kbit) chip */
extern struct EepromConfig gStaticData_085A9F04; /* 8-KB (64 Kbit) chip */
extern struct EepromConfig *gUnknown_03001634;   /* active config */

extern u8 gUnknown_03001620;   /* claimed timer number */
extern u16 gUnknown_03001622;  /* timeout countdown */
extern u8 gUnknown_03001624;   /* timeout flag */
extern vu16 *gUnknown_03001628; /* claimed timer's TMxCNT_L */
extern u16 gUnknown_0300162C;  /* IME saved by sub_803AA08 */

/* SDK EEPROMConfigure / IdentifyEeprom: picks the chip's config by its
 * size code (4 = 4 Kbit, 0x40 = 64 Kbit); anything else falls back to
 * the 512-byte chip and reports failure. At -O1 each branch keeps its
 * own literal pool, which is the ROM's layout. */
s32 sub_803A968(u16 type)
{
    u16 result;

    result = 0;
    if (type == 4) {
        gUnknown_03001634 = &gStaticData_085A9EF8;
    } else if (type == 0x40) {
        gUnknown_03001634 = &gStaticData_085A9F04;
    } else {
        gUnknown_03001634 = &gStaticData_085A9EF8;
        result = 1;
    }
    return result;
}

/* SDK EepromTimerIntr (pokeemerald's FlashTimerIntr): the timer IRQ
 * handler `sub_803A9D0` hands out. Counts the timeout down and sets the
 * timeout flag that `sub_803AC04`'s busy-wait polls. Never called
 * directly, only through the pointer. (Was kept as a raw `.byte` blob
 * before; it is ordinary compiled C.) */
void sub_803A9AC(void)
{
    if (gUnknown_03001622 != 0 && --gUnknown_03001622 == 0)
        gUnknown_03001624 = 1;
}

/* SDK SetEepromTimerIntr (pokeemerald's SetFlashTimerIntr): claims
 * hardware timer `timerNum` (0-3), points `gUnknown_03001628` at its
 * TMxCNT_L and returns the IRQ handler for the caller to install. */
s32 sub_803A9D0(u8 timerNum, void (**intrFunc)(void))
{
    if (timerNum >= 4)
        return 1;
    gUnknown_03001620 = timerNum;
    gUnknown_03001628 = &REG_TMCNT(gUnknown_03001620);
    *intrFunc = sub_803A9AC;
    return 0;
}

/* SDK StartEepromTimer (pokeemerald's StartFlashTimer, with the IF
 * acknowledge before the IE enable): saves and clears IME, stops the
 * timer, acks and enables its IRQ, clears the timeout flag, loads the
 * {countdown, reload, control} triple from `maxTime` and sets IME. The
 * inverse is `sub_803AA90` (src/system/timer_util_aa90.c). */
void sub_803AA08(const u16 *maxTime)
{
    gUnknown_0300162C = REG_IME;
    REG_IME = 0;
    gUnknown_03001628[1] = 0;
    REG_IF = INTR_FLAG_TIMER0 << gUnknown_03001620;
    REG_IE |= INTR_FLAG_TIMER0 << gUnknown_03001620;
    gUnknown_03001624 = 0;
    gUnknown_03001622 = *maxTime++;
    *gUnknown_03001628++ = *maxTime++;
    *gUnknown_03001628-- = *maxTime++;
    REG_IME = 1;
}
