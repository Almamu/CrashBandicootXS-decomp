#include "core.h"
#include "audio.h"
#include "settings_sync.h"

extern void sub_800014C(void *dst, void *src, s32 len);
extern u8 gUnknown_03000808;
extern s32 sub_803A968(u16 type);
extern void *gUnknown_030009FC;
extern s32 sub_803A9D0(u8 index, void **out);
extern s32 sub_803AB54(u16 index, void *buf);
extern s32 sub_803AD38(u16 index, void *buf);

/* Same shape as src/system/timer_util.c's own `struct EepromConfig`
 * (redeclared here per this project's convention - see
 * src/system/eeprom_util.c's own copy). */
struct EepromConfig {
    u32 unk0;
    u16 maxCount;
    u16 waitcntBits;
    u8 addrBitCount;
    u8 pad[3];
};

extern struct EepromConfig *gUnknown_03001634;

/* EEPROM "load" - reads `gUnknown_03001634->maxCount` 8-byte blocks
 * from the EEPROM chip (`sub_803AB54`, still raw - see
 * src/system/timer_util.c's `EepromConfig` comment) into a stack
 * buffer sized `len` (always 0x200, `sizeof(struct
 * settings_sync_record)`, from every call site), then copies the whole
 * buffer into `self`. One-time-inits the EEPROM chip config
 * (`sub_803A968`) and claims hardware timer 2 for the transfer
 * (`sub_803A9D0`) on the way in. Returns -1 on any block-read failure
 * (buffer left untouched) or 0 on success.
 *
 * The IME-save/IE-clear/IME-restore snippet (repeated once per exit
 * path) matches the ROM's exact "no extra copy" shape here as plain
 * C (`u16 savedIme = REG_IME; ...`), the same phrasing already proven
 * for `sub_8001D30` (src/system/link_cable.c) - the previously
 * suspected register-pressure gap didn't reproduce with this
 * function's actual field/loop structure. Byte-identical to the ROM,
 * confirmed via a direct `.text`-section `cmp` against
 * `raw_08002868_target.o` (not just objdiff-cli, whose per-symbol
 * instruction diff misreports the trailing literal-pool word at this
 * exact symbol boundary as a size mismatch even though the raw bytes
 * are identical). */
s32 sub_8002868(void *self, s32 len)
{
    u8 buf[0x200];
    s32 i;
    u8 *p;

    if (gUnknown_03000808) {
        u16 ret = (u16)sub_803A968(4);
        if (ret != 0) {
            return -1;
        }
        gUnknown_03000808 = 0;
    }

    REG_IME = 0;
    sub_803A9D0(2, &gUnknown_030009FC);

    p = buf;
    i = 0;
    while (i < gUnknown_03001634->maxCount) {
        if ((u16)sub_803AB54(i, p) != 0) {
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

    sub_800014C(self, buf, len);
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

/* EEPROM "save" - counterpart to `sub_8002868`: same one-time
 * chip-config init and timer-2 claim, but copies `self` into a stack
 * buffer *after* the chip-config check (not before, matching the
 * ROM's own instruction order), then writes it out `maxCount` 8-byte
 * blocks at a time (`sub_803AD38`, still raw). Same -1-on-failure/
 * 0-on-success return and IME-save/IE-clear/IME-restore snippet as
 * `sub_8002868`, byte-identical as plain C for the same reason (see
 * that function's doc comment). */
s32 sub_8002938(void *self, s32 len)
{
    u8 buf[0x200];
    s32 i;
    u8 *p;

    if (gUnknown_03000808) {
        u16 ret = (u16)sub_803A968(4);
        if (ret != 0) {
            return -1;
        }
        gUnknown_03000808 = 0;
    }

    sub_800014C(buf, self, len);

    REG_IME = 0;
    sub_803A9D0(2, &gUnknown_030009FC);

    p = buf;
    i = 0;
    while (i < gUnknown_03001634->maxCount) {
        if ((u16)sub_803AD38(i, p) != 0) {
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

extern struct AudioContext *gUnknown_030012BC;
extern u32 sub_8001AB8(struct AudioContext *self);
extern void sub_8001BD4(struct AudioContext *self);
extern void sub_8001B54(struct AudioContext *self, u32 id);
extern s32 sub_8002868(void *self, s32 len);
extern u32 sub_8002B44(struct settings_sync_record *self);

/* Loads the settings record from EEPROM (`sub_8002868`, retried up to
 * 3 times), muting the music player across the transfer (stop before,
 * resume after, matching `src/audio/audio_context.c`'s established
 * `AudioContext` helpers), then validates the loaded record's two
 * marker bytes and checksum. Returns 4 (EEPROM read failed after
 * retries), 2 (bad `field_1f8` marker), 1 (bad `versionNibble`
 * marker), 3 (checksum mismatch) or 0 (fully valid). */
s32 sub_8002A08(struct settings_sync_record *self)
{
    struct AudioContext *audio;
    s32 flag;
    s32 wasPlaying;
    u32 savedSong;
    s32 i;
    s32 result;

    audio = gUnknown_030012BC;
    flag = 0;
    if (audio->state == 1) {
        flag = 1;
    }
    wasPlaying = flag;

    savedSong = sub_8001AB8(audio);
    if (wasPlaying) {
        sub_8001BD4(gUnknown_030012BC);
    }

    i = 0;
    do {
        result = sub_8002868(self, 0x200);
        i++;
    } while (i <= 2 && result != 0);

    if (wasPlaying) {
        sub_8001B54(gUnknown_030012BC, savedSong);
    }

    if (result != 0) {
        return 4;
    }
    if (self->field_1f8 != 0x43) {
        return 2;
    }
    if (self->versionNibble != 0x12) {
        return 1;
    }
    if ((u8)sub_8002B44(self) == 0) {
        return 3;
    }
    return 0;
}
