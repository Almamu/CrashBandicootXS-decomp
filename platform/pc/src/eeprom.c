/* AgbEeprom's API (lib/agb_eeprom/include/agb_eeprom.h), which the pc
 * platform provides in place of the library: the chip as a RAM buffer,
 * erased (0xFF) at start, and with --save FILE kept in a file (phase 5
 * of #1093).
 *
 * The buffer holds the chip's bits in the order the GBA sends them, the
 * layout of mGBA's .sav files: a block's 64 bits, data[3]'s bit 15 first
 * (EEPROMWrite sends data[3] to data[0], each MSB first), 8 to a byte, MSB
 * first. So an 8-byte block is data[0..3]'s bytes in reverse, and a save
 * file moves between the host and mGBA as it is. The game uses the 4 Kbit
 * chip (save_data.cpp): the file is 512 bytes. A longer file (the 8 KiB
 * some emulators write) is read as far as the chip goes.
 *
 * With --save the file is read at the first EEPROMConfigure, if it
 * exists, and written back whole at the VBlank after a write
 * (HostSaveFlush, irq.c) and at the end (main.c). */

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <agb_eeprom.h>

#include "host_platform.h"

#define EEPROM_BLOCK_SIZE 8
#define EEPROM_MAX_SIZE   0x2000

static const struct EepromConfig sEepromConfigs[] = {
    { 0x200, 0x40, 0x300, 6 },    /* EEPROM_CHIP_4KBIT */
    { 0x2000, 0x400, 0x300, 14 }, /* EEPROM_CHIP_64KBIT */
};

const struct EepromConfig *gEepromConfig = &sEepromConfigs[0];

static u8 sEeprom[EEPROM_MAX_SIZE];
static u8 sEepromErased;
static u8 sEepromDirty;

static void LoadSave(void)
{
    FILE *f;

    if (gHostOptions.save == NULL) {
        return;
    }
    f = fopen(gHostOptions.save, "rb");
    if (f == NULL) {
        if (errno != ENOENT) {
            fprintf(stderr, "%s: %s\n", gHostOptions.save, strerror(errno));
        }
        return;
    }
    if (fread(sEeprom, 1, sizeof(sEeprom), f) == 0) {
        memset(sEeprom, 0xFF, sizeof(sEeprom));
    }
    fclose(f);
}

void HostSaveFlush(void)
{
    FILE *f;

    if (!sEepromDirty || gHostOptions.save == NULL) {
        return;
    }
    sEepromDirty = 0;
    f = fopen(gHostOptions.save, "wb");
    if (f == NULL || fwrite(sEeprom, 1, gEepromConfig->size, f) != gEepromConfig->size) {
        fprintf(stderr, "%s: can't write the save\n", gHostOptions.save);
    }
    if (f != NULL) {
        fclose(f);
    }
}

/* The watchdog timer's handler: nothing to time out on the host. */
static void EepromTimerIntr(void)
{
}

s32 EEPROMConfigure(u16 type)
{
    if (!sEepromErased) {
        memset(sEeprom, 0xFF, sizeof(sEeprom));
        sEepromErased = 1;
        LoadSave();
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

/* A block's bytes as data[0..3] holds them: the chip's order reversed. */
static void Unpack(u16 address, u8 *out)
{
    u32 i;

    for (i = 0; i < EEPROM_BLOCK_SIZE; i++) {
        out[i] = sEeprom[address * EEPROM_BLOCK_SIZE + EEPROM_BLOCK_SIZE - 1 - i];
    }
}

s32 EEPROMRead(u16 address, u16 *data)
{
    if (address >= gEepromConfig->maxCount) {
        return 0x80FF;
    }
    Unpack(address, (u8 *)data);
    return 0;
}

s32 EEPROMWrite(u16 address, u16 *data)
{
    const u8 *bytes = (const u8 *)data;
    u32 i;

    if (address >= gEepromConfig->maxCount) {
        return 0x80FF;
    }
    for (i = 0; i < EEPROM_BLOCK_SIZE; i++) {
        sEeprom[address * EEPROM_BLOCK_SIZE + EEPROM_BLOCK_SIZE - 1 - i] = bytes[i];
    }
    sEepromDirty = 1;
    return 0;
}

s32 EEPROMCompare(u16 address, u16 *data)
{
    u8 block[EEPROM_BLOCK_SIZE];

    if (address >= gEepromConfig->maxCount) {
        return 0x80FF;
    }
    Unpack(address, block);
    return memcmp(block, data, EEPROM_BLOCK_SIZE) != 0 ? 0x8000 : 0;
}

s32 EEPROMWrite1_check(u16 address, u16 *data)
{
    s32 result = EEPROMWrite(address, data);

    if (result == 0) {
        result = EEPROMCompare(address, data);
    }
    return result;
}
