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

/* EEPROM "save" - counterpart to `sub_8002868`: copies `self` into a
 * stack buffer first, then writes it out `maxCount` 8-byte blocks at a
 * time (`sub_803AD38`, still raw). Same one-time chip-config and
 * timer-2 claim, same -1-on-failure/0-on-success return, same NAKED
 * transcription reason as `sub_8002868` above. */
NAKED s32 sub_8002938(void *self, s32 len)
{
    asm(
        "push {r4, r5, r6, lr}\n\t"
        "ldr r4, 2f\n\t"
        "add sp, r4\n\t"
        "add r4, r0, #0\n\t"
        "add r5, r1, #0\n\t"
        "ldr r6, 3f\n\t"
        "ldrb r0, [r6]\n\t"
        "cmp r0, #0\n\t"
        "beq 1f\n\t"
        "mov r0, #4\n\t"
        "bl sub_803A968\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "cmp r0, #0\n\t"
        "bne 13f\n\t"
        "strb r0, [r6]\n\t"
    "1:\n\t"
        "mov r0, sp\n\t"
        "add r1, r4, #0\n\t"
        "add r2, r5, #0\n\t"
        "bl sub_800014C\n\t"
        "ldr r1, 4f\n\t"
        "mov r0, #0\n\t"
        "strh r0, [r1]\n\t"
        "ldr r1, 5f\n\t"
        "mov r0, #2\n\t"
        "bl sub_803A9D0\n\t"
        "mov r5, sp\n\t"
        "mov r4, #0\n\t"
        "b 7f\n\t"
        ".align 2, 0\n"
    "2: .4byte 0xFFFFFE00\n"
    "3: .4byte gUnknown_03000808\n"
    "4: .4byte 0x04000208\n"
    "5: .4byte gUnknown_030009FC\n"
    "6:\n\t"
        "lsl r0, r4, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "add r1, r5, #0\n\t"
        "bl sub_803AD38\n\t"
        "lsl r0, r0, #0x10\n\t"
        "cmp r0, #0\n\t"
        "bne 12f\n\t"
        "add r5, #8\n\t"
        "add r4, #1\n\t"
    "7:\n\t"
        "ldr r0, 8f\n\t"
        "ldr r0, [r0]\n\t"
        "ldrh r0, [r0, #4]\n\t"
        "cmp r4, r0\n\t"
        "blt 6b\n\t"
        "ldr r3, 9f\n\t"
        "ldrh r2, [r3]\n\t"
        "mov r0, #0\n\t"
        "strh r0, [r3]\n\t"
        "ldr r4, 10f\n\t"
        "ldrh r1, [r4]\n\t"
        "ldr r0, 11f\n\t"
        "and r0, r1\n\t"
        "strh r0, [r4]\n\t"
        "strh r2, [r3]\n\t"
        "mov r0, #1\n\t"
        "strh r0, [r3]\n\t"
        "mov r0, #0\n\t"
        "b 14f\n\t"
        ".align 2, 0\n"
    "8: .4byte gUnknown_03001634\n"
    "9: .4byte 0x04000208\n"
    "10: .4byte 0x04000200\n"
    "11: .4byte 0x0000FFDF\n"
    "12:\n\t"
        "ldr r3, 15f\n\t"
        "ldrh r2, [r3]\n\t"
        "mov r0, #0\n\t"
        "strh r0, [r3]\n\t"
        "ldr r4, 16f\n\t"
        "ldrh r1, [r4]\n\t"
        "ldr r0, 17f\n\t"
        "and r0, r1\n\t"
        "strh r0, [r4]\n\t"
        "strh r2, [r3]\n\t"
        "mov r0, #1\n\t"
        "strh r0, [r3]\n\t"
    "13:\n\t"
        "mov r0, #1\n\t"
        "neg r0, r0\n\t"
    "14:\n\t"
        "mov r3, #0x80\n\t"
        "lsl r3, r3, #2\n\t"
        "add sp, r3\n\t"
        "pop {r4, r5, r6}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
    "15: .4byte 0x04000208\n"
    "16: .4byte 0x04000200\n"
    "17: .4byte 0x0000FFDF\n"
    );
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
