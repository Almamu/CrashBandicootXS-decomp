#ifndef __AGB_EEPROM_INTERNAL_H__
#define __AGB_EEPROM_INTERNAL_H__

#include "gba/gba.h"
#include <agb_eeprom.h>

/* The AgbEeprom library's own state and helpers, shared by its objects
 * and not part of its public interface (<agb_eeprom.h>). The globals
 * keep TMC's (gEEPROMConfig512/gEEPROMConfig8k/gEEPROMConfig) and
 * agb_flash's (timer number, countdown, timer register, saved IME,
 * timeout flag) roles. */

/* The two chip configs EEPROMConfigure picks from
 * (data/eeprom_5a9eec.c). */
extern const struct EepromConfig gEepromConfig512; /* 512-byte (4 Kbit) chip */
extern const struct EepromConfig gEepromConfig8k;  /* 8-KB (64 Kbit) chip */

/* The write timeout EEPROMWrite passes to StartEepromTimer:
 * {countdown, TMxCNT_L reload, TMxCNT_H control}. */
extern const u16 gEepromMaxTime[3];

extern u8 gEepromTimerNum;      /* claimed timer number */
extern u16 gEepromTimerCount;   /* timeout countdown */
extern u8 gEepromTimeoutFlag;   /* timeout flag, set by EepromTimerIntr */
extern vu16 *gEepromTimerReg;   /* claimed timer's TMxCNT_L */
extern u16 gEepromSavedIme;     /* IME saved by StartEepromTimer */

void EepromTimerIntr(void);
void StartEepromTimer(const u16 *maxTime);
void StopEepromTimer(void);
void DMA3Transfer(const void *src, void *dst, u16 count);

#endif /* __AGB_EEPROM_INTERNAL_H__ */
