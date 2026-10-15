/* AgbEeprom's API (lib/agb_eeprom/include/agb_eeprom.h), which the pc
 * platform provides in place of the library: the chip as a RAM buffer,
 * erased (0xFF) at start. Saving it to a file is phase 5 of #1093. */

#include <string.h>

#include <agb_eeprom.h>

#define EEPROM_BLOCK_SIZE 8
#define EEPROM_MAX_SIZE   0x2000

static const struct EepromConfig sEepromConfigs[] = {
    { 0x200, 0x40, 0x300, 6 },    /* EEPROM_CHIP_4KBIT */
    { 0x2000, 0x400, 0x300, 14 }, /* EEPROM_CHIP_64KBIT */
};

const struct EepromConfig *gEepromConfig = &sEepromConfigs[0];

static u8 sEeprom[EEPROM_MAX_SIZE];
static u8 sEepromErased;

/* The watchdog timer's handler: nothing to time out on the host. */
static void EepromTimerIntr(void)
{
}

s32 EEPROMConfigure(u16 type)
{
    if (!sEepromErased) {
        memset(sEeprom, 0xFF, sizeof(sEeprom));
        sEepromErased = 1;
    }
    if (type == EEPROM_CHIP_64KBIT) {
        gEepromConfig = &sEepromConfigs[1];
        return 0;
    }
    gEepromConfig = &sEepromConfigs[0];
    return type == EEPROM_CHIP_4KBIT ? 0 : 1;
}

s32 SetEepromTimerIntr(u8 timerNum, void (**intrFunc)(void))
{
    if (timerNum >= 4) {
        return 1;
    }
    *intrFunc = EepromTimerIntr;
    return 0;
}

s32 EEPROMRead(u16 address, u16 *data)
{
    if (address >= gEepromConfig->maxCount) {
        return 0x80FF;
    }
    memcpy(data, &sEeprom[address * EEPROM_BLOCK_SIZE], EEPROM_BLOCK_SIZE);
    return 0;
}

s32 EEPROMWrite(u16 address, u16 *data)
{
    if (address >= gEepromConfig->maxCount) {
        return 0x80FF;
    }
    memcpy(&sEeprom[address * EEPROM_BLOCK_SIZE], data, EEPROM_BLOCK_SIZE);
    return 0;
}

s32 EEPROMCompare(u16 address, u16 *data)
{
    if (address >= gEepromConfig->maxCount) {
        return 0x80FF;
    }
    return memcmp(&sEeprom[address * EEPROM_BLOCK_SIZE], data, EEPROM_BLOCK_SIZE) != 0 ? 0x8000 : 0;
}

s32 EEPROMWrite1_check(u16 address, u16 *data)
{
    s32 result = EEPROMWrite(address, data);

    if (result == 0) {
        result = EEPROMCompare(address, data);
    }
    return result;
}
