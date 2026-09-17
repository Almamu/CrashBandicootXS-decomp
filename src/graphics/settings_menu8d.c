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
extern u8 *gUnknown_03001634;

#if NON_MATCHING
/* NOT YET BYTE-MATCHING: semantics fully understood. EEPROM "load" -
 * reads `gUnknown_03001634->maxCount` 8-byte blocks from the EEPROM
 * chip (`sub_803AB54`, still raw - see src/system/timer_util.c's
 * `EepromConfig` comment) into a stack buffer sized `len` (always
 * 0x200, `sizeof(struct settings_sync_record)`, from every call site),
 * then copies the whole buffer into `self`. One-time-inits the EEPROM
 * chip config (`sub_803A968`) and claims hardware timer 2 for the
 * transfer (`sub_803A9D0`) on the way in. Returns -1 on any block-read
 * failure (buffer left untouched) or 0 on success. Every load/store,
 * branch and call is semantically confirmed against the ROM, but the
 * ROM's shared IME-save/IE-clear/IME-restore snippet (repeated once
 * per exit path) lands its saved-IME value straight into the register
 * it later restores from (`ldrh r2,[r3]` then, at the end, `strh
 * r2,[r3]`), while this compiler routes the same value through r0
 * first before copying it into the variable's assigned register - an
 * extra `mov` the ROM doesn't have, most likely due to the larger
 * number of live locals in this function (buffer/self/len/p/i) leaving
 * less register headroom than the near-identical snippet that matched
 * cleanly in the much smaller `sub_8001D30` (src/system/link_cable.c).
 * Real bytes stay in `asm/code_3_1_10_3_2868.s`, wrapped
 * `.if NON_MATCHING == 0`. */
s32 sub_8002868(void *self, s32 len)
{
    u8 buffer[0x200];
    u8 *p;
    s32 i;
    u16 savedIme;

    if (gUnknown_03000808 != 0) {
        u16 configResult = sub_803A968(4);
        if (configResult != 0) {
            return -1;
        }
        gUnknown_03000808 = 0;
    }

    REG_IME = 0;
    sub_803A9D0(2, &gUnknown_030009FC);

    p = buffer;
    for (i = 0; i < *(u16 *)(gUnknown_03001634 + 4); i++) {
        u16 blockResult = sub_803AB54((u16)i, p);
        if (blockResult != 0) {
            savedIme = REG_IME;
            REG_IME = 0;
            REG_IE &= ~0x20;
            REG_IME = savedIme;
            REG_IME = 1;
            return -1;
        }
        p += 8;
    }

    savedIme = REG_IME;
    REG_IME = 0;
    REG_IE &= ~0x20;
    REG_IME = savedIme;
    REG_IME = 1;

    sub_800014C(self, buffer, len);
    return 0;
}

/* NOT YET BYTE-MATCHING: semantics fully understood, same gap class as
 * `sub_8002868` above. EEPROM "save" - counterpart to `sub_8002868`:
 * copies `self` into a stack buffer first, then writes it out
 * `maxCount` 8-byte blocks at a time (`sub_803AD38`, still raw). Same
 * one-time chip-config and timer-2 claim, same -1-on-failure/
 * 0-on-success return. Real bytes stay in
 * `asm/code_3_1_10_3_2868.s`, wrapped `.if NON_MATCHING == 0`. */
s32 sub_8002938(void *self, s32 len)
{
    u8 buffer[0x200];
    u8 *p;
    s32 i;
    u16 savedIme;

    if (gUnknown_03000808 != 0) {
        if (sub_803A968(4) != 0) {
            return -1;
        }
        gUnknown_03000808 = 0;
    }

    sub_800014C(buffer, self, len);

    REG_IME = 0;
    sub_803A9D0(2, &gUnknown_030009FC);

    p = buffer;
    for (i = 0; i < *(u16 *)(gUnknown_03001634 + 4); i++) {
        if (sub_803AD38((u16)i, p) != 0) {
            savedIme = REG_IME;
            REG_IME = 0;
            REG_IE &= ~0x20;
            REG_IME = savedIme;
            REG_IME = 1;
            return -1;
        }
        p += 8;
    }

    savedIme = REG_IME;
    REG_IME = 0;
    REG_IE &= ~0x20;
    REG_IME = savedIme;
    REG_IME = 1;

    return 0;
}
#endif /* NON_MATCHING */

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
