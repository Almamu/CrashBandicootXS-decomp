#include "core.h"
#include "audio.h"
#include "settings_sync.h"

extern void MemCopy32(void *dst, void *src, s32 len);
extern u8 gEepromNeedsInit;
extern s32 EEPROMConfigure(u16 type);
extern void *gIntrTableTimer2;
extern s32 SetEepromTimerIntr(u8 index, void **out);
extern s32 EEPROMRead(u16 index, void *buf);
extern s32 EEPROMWrite1_check(u16 index, void *buf);

/* Same shape as src/system/timer_util.c's own `struct EepromConfig`
 * (redeclared here per this project's convention - see
 * src/system/eeprom_util.c's own copy). */
struct EepromConfig {
    u32 size;
    u16 maxCount;
    u16 waitcntBits;
    u8 addrBitCount;
    u8 pad[3];
};

extern struct EepromConfig *gEepromConfig;

/* Reads the save data: `gEepromConfig->maxCount` 8-byte blocks from
 * the EEPROM chip (the SDK's `EEPROMRead`) into a stack
 * buffer sized `len` (always 0x200, `sizeof(struct
 * settings_sync_record)`, from every call site), then copies the whole
 * buffer into `self`. One-time-inits the EEPROM chip config
 * (`EEPROMConfigure`) and claims hardware timer 2 for the transfer
 * (`SetEepromTimerIntr`, installing its handler straight into the Timer 2
 * slot of the IRQ table, `gIntrTableTimer2`) on the way in. Returns -1 on any block-read failure
 * (buffer left untouched) or 0 on success.
 *
 * The IME-save/IE-clear/IME-restore snippet (repeated once per exit
 * path) matches the ROM's exact "no extra copy" shape here as plain
 * C (`u16 savedIme = REG_IME; ...`), the same phrasing already proven
 * for `LinkStop` (src/system/link_cable.c) - the previously
 * suspected register-pressure gap didn't reproduce with this
 * function's actual field/loop structure. Byte-identical to the ROM,
 * confirmed via a direct `.text`-section `cmp` against
 * `raw_08002868_target.o` (not just objdiff-cli, whose per-symbol
 * instruction diff misreports the trailing literal-pool word at this
 * exact symbol boundary as a size mismatch even though the raw bytes
 * are identical). */
s32 ReadSaveData(void *self, s32 len)
{
    u8 buf[0x200];
    s32 i;
    u8 *p;

    if (gEepromNeedsInit) {
        u16 ret = (u16)EEPROMConfigure(4);
        if (ret != 0) {
            return -1;
        }
        gEepromNeedsInit = 0;
    }

    REG_IME = 0;
    SetEepromTimerIntr(2, &gIntrTableTimer2);

    p = buf;
    i = 0;
    while (i < gEepromConfig->maxCount) {
        if ((u16)EEPROMRead(i, p) != 0) {
            goto fail_restore;
        }
        p += 8;
        i++;
    }

    {
        u16 savedIme = REG_IME;
        REG_IME = 0;
        REG_IE &= 0xFFDF;
        REG_IME = savedIme;
        REG_IME = 1;
    }

    MemCopy32(self, buf, len);
    return 0;

fail_restore:
    {
        u16 savedIme = REG_IME;
        REG_IME = 0;
        REG_IE &= 0xFFDF;
        REG_IME = savedIme;
        REG_IME = 1;
    }
    return -1;
}

/* EEPROM "save" - counterpart to `ReadSaveData`: same one-time
 * chip-config init and timer-2 claim, but copies `self` into a stack
 * buffer *after* the chip-config check (not before, matching the
 * ROM's own instruction order), then writes it out `maxCount` 8-byte
 * blocks at a time (the SDK's `EEPROMWrite1_check`). Same -1-on-failure/
 * 0-on-success return and IME-save/IE-clear/IME-restore snippet as
 * `ReadSaveData`, byte-identical as plain C for the same reason (see
 * that function's doc comment). */
s32 WriteSaveData(void *self, s32 len)
{
    u8 buf[0x200];
    s32 i;
    u8 *p;

    if (gEepromNeedsInit) {
        u16 ret = (u16)EEPROMConfigure(4);
        if (ret != 0) {
            return -1;
        }
        gEepromNeedsInit = 0;
    }

    MemCopy32(buf, self, len);

    REG_IME = 0;
    SetEepromTimerIntr(2, &gIntrTableTimer2);

    p = buf;
    i = 0;
    while (i < gEepromConfig->maxCount) {
        if ((u16)EEPROMWrite1_check(i, p) != 0) {
            goto fail_restore;
        }
        p += 8;
        i++;
    }

    {
        u16 savedIme = REG_IME;
        REG_IME = 0;
        REG_IE &= 0xFFDF;
        REG_IME = savedIme;
        REG_IME = 1;
    }

    return 0;

fail_restore:
    {
        u16 savedIme = REG_IME;
        REG_IME = 0;
        REG_IE &= 0xFFDF;
        REG_IME = savedIme;
        REG_IME = 1;
    }
    return -1;
}

extern struct AudioContext *gAudioContext;
extern u32 GetCurrentSong(struct AudioContext *self);
extern void StopSong(struct AudioContext *self);
extern void PlaySong(struct AudioContext *self, u32 id);
extern s32 ReadSaveData(void *self, s32 len);
extern u32 CheckSaveChecksum(struct settings_sync_record *self);

/* Loads the settings record from EEPROM (`ReadSaveData`, retried up to
 * 3 times), muting the music player across the transfer (stop before,
 * resume after, matching `src/audio/audio_context.c`'s established
 * `AudioContext` helpers), then validates the loaded record's two
 * marker bytes and checksum. Returns 4 (EEPROM read failed after
 * retries), 2 (bad `magic` marker), 1 (bad `versionNibble`
 * marker), 3 (checksum mismatch) or 0 (fully valid). */
s32 LoadSaveData(struct settings_sync_record *self)
{
    struct AudioContext *audio;
    s32 flag;
    s32 wasPlaying;
    u32 savedSong;
    s32 i;
    s32 result;

    audio = gAudioContext;
    flag = 0;
    if (audio->state == 1) {
        flag = 1;
    }
    wasPlaying = flag;

    savedSong = GetCurrentSong(audio);
    if (wasPlaying) {
        StopSong(gAudioContext);
    }

    i = 0;
    do {
        result = ReadSaveData(self, 0x200);
        i++;
    } while (i <= 2 && result != 0);

    if (wasPlaying) {
        PlaySong(gAudioContext, savedSong);
    }

    if (result != 0) {
        return 4;
    }
    if (self->magic != 0x43) {
        return 2;
    }
    if (self->versionNibble != 0x12) {
        return 1;
    }
    if ((u8)CheckSaveChecksum(self) == 0) {
        return 3;
    }
    return 0;
}
