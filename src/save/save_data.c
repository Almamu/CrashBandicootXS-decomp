#include "core.h"
#include "match.h"
#include "audio.h"
#include <agb_eeprom.h>
#include "gba/dma_macros.h"
#include "save.h"
#include "system.h"
#include "globals.h"

/* Reads the save data: `gEepromConfig->maxCount` 8-byte blocks from
 * the EEPROM chip (the SDK's `EEPROMRead`) into a stack
 * buffer sized `len` (always 0x200, `sizeof(struct
 * save_data)`, from every call site), then copies the whole
 * buffer into `self`. One-time-inits the EEPROM chip config
 * (`EEPROMConfigure`) and claims hardware timer 2 for the transfer
 * (`SetEepromTimerIntr`, installing its handler straight into the Timer 2
 * slot of the IRQ table, `gIntrTableTimer2`) on the way in. Returns -1 on any block-read failure
 * (buffer left untouched) or 0 on success.
 *
 * The IME-save/IE-clear/IME-restore snippet (repeated once per exit
 * path) matches the ROM's exact "no extra copy" shape here as plain
 * C (`u16 savedIme = REG_IME; ...`), the same phrasing already proven
 * for `LinkStop` (src/link/link_handshake.c) - the previously
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
        if ((u16)EEPROMRead(i, (u16 *)p) != 0) {
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
        if ((u16)EEPROMWrite1_check(i, (u16 *)p) != 0) {
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

/* Loads the settings record from EEPROM (`ReadSaveData`, retried up to
 * 3 times), muting the music player across the transfer (stop before,
 * resume after, matching `src/audio/audio.cpp`'s established
 * `AudioContext` helpers), then validates the loaded record's two
 * marker bytes and checksum. Returns 4 (EEPROM read failed after
 * retries), 2 (bad `magic` marker), 1 (bad `versionNibble`
 * marker), 3 (checksum mismatch) or 0 (fully valid). */
s32 LoadSaveData(struct save_data *self)
{
    struct audio_context *audio;
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

/* Validates the record's checksum (inline word-sum, same shape as
 * `CheckSaveChecksum` below but not a call to it - the ROM genuinely inlines
 * this one) and, if it fails, repairs the record in place: DMA-fills
 * the whole 0x200 bytes with 0 (raw DMA3 register pokes rather than a
 * `DmaFill16` call - a different, earlier style than
 * `src/save/save_data.c`'s `ResetSaveData` uses for the same
 * "reset to blank" operation), marks every row selected
 * (`EraseSaveSlot`), re-stamps the two marker bytes, clears
 * `flags`/`field_1fb`, and refreshes the checksum (`UpdateSaveChecksum`).
 *
 * Parked as NAKED until the near-miss polish pass
 * (docs/matching/archive/near-miss-polish.md): the ROM computes `&flags` before
 * `&field_1fb` yet still gives `flags` r7 (global-alloc's first pick).
 * With `flags` computed first its live range is one insn longer, so it
 * ranked below `field_1fb` and the two swapped r7/r8. The empty
 * `MATCH_USE(flags)` below emits nothing; it adds one reference
 * to `flags`, which lifts its allocation priority (floor_log2(refs) *
 * refs / live length) above `field_1fb`'s. */
/* An inlined copy of CheckSaveChecksum below. */
static inline u32 checksum_ok(struct save_data *self)
{
    u32 *p = (u32 *)self;
    u32 sum = 0;
    s32 i;
    u32 result;

    for (i = 0x7e; i >= 0; i--) {
        sum += *p++;
    }

    result = 0;
    if (sum == self->checksum) {
        result = 1;
    }
    return result;
}

void ValidateSaveData(struct save_data *self)
{
    s32 i;

    if (!checksum_ok(self)) {
        MATCH_HOLD_REG(u8 *, marker, r6);
        u8 *flags, *f1fb, *version;

        DmaFill16(3, 0, self, 0x200);
        i = 0;
        marker = &self->magic;
        version = &self->versionNibble;
        flags = &self->flags;
        /* No code: one extra use of `flags` for global-alloc's ranking. */
        MATCH_USE(flags);
        f1fb = &self->field_1fb;
        for (; i <= 3; i++) {
            EraseSaveSlot(self, i);
        }
        {
            u8 z = 0;

            *marker = 0x43;
            *version = 0x12;
            *flags = z;
            *f1fb = z;
        }
        UpdateSaveChecksum(self);
    }
}

/* Recomputes this record's additive word-sum checksum over its first
 * 0x1fc bytes (127 words) and compares it against the stored
 * `checksum` field, returning 1 on match. Counterpart to `UpdateSaveChecksum`
 * below, which stores instead of comparing. */
u32 CheckSaveChecksum(struct save_data *self)
{
    u32 *p = (u32 *)self;
    u32 sum = 0;
    s32 i;
    u32 result;

    for (i = 0x7e; i >= 0; i--) {
        sum += *p++;
    }

    result = 0;
    if (sum == self->checksum) {
        result = 1;
    }
    return result;
}

/* Recomputes and stores this record's additive word-sum checksum over
 * its first 0x1fc bytes (127 words) into `checksum`. Every function
 * that mutates `flags`/`slotEmpty` calls this afterward to keep the
 * checksum in sync - see src/save/save_data.c's header
 * comment, which already anticipated this function (it was matched
 * from a later chunk, issue #5, before this one). */
void UpdateSaveChecksum(struct save_data *self)
{
    u32 *p = (u32 *)self;
    u32 sum = 0;
    s32 i;

    for (i = 0x7e; i >= 0; i--) {
        sum += *p++;
    }
    self->checksum = sum;
}

/* `versionNibble`'s high-nibble accessor. */
u32 GetSaveGameId(struct save_data *self)
{
    return self->versionNibble >> 4;
}

/* Saves the settings record to EEPROM (`WriteSaveData`, retried up to 5
 * times), muting the music player across the transfer the same way
 * `LoadSaveData` (src/save/save_data.c) does (checksum
 * refreshed first via `UpdateSaveChecksum`, before the mute). Returns 4
 * (EEPROM write failed after retries) or 0 (success). */
s32 StoreSaveData(struct save_data *self)
{
    struct audio_context *audio;
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
    UpdateSaveChecksum(self);

    if (wasPlaying) {
        StopSong(gAudioContext);
    }

    i = 0;
    do {
        result = WriteSaveData(self, 0x200);
        i++;
    } while (i <= 4 && result != 0);

    if (wasPlaying) {
        PlaySong(gAudioContext, savedSong);
    }

    if (result != 0) {
        return 4;
    }
    return 0;
}

/* Copies row `row`'s 0x70-byte slot out to `dst`, but only if the row
 * hasn't been explicitly selected/edited (`slotEmpty[row] == 0`) -
 * refreshes a caller-side scratch copy with the stored default/synced
 * value while leaving a user-edited row alone. */
void ReadSaveSlot(struct save_data *self, s32 row, void *dst)
{
    if (self->slotEmpty[row] == 0) {
        s32 offset;

        offset = row * 0x70;
        offset = offset + (s32)self;
        MemCopy32(dst, (void *)offset, 0x70);
    }
}

/* Force-writes `src` into row `row`'s 0x70-byte slot, clears the row's
 * `slotEmpty` flag (marking it "not explicitly edited" again), and
 * refreshes the checksum. */
void WriteSaveSlot(struct save_data *self, s32 row, void *src)
{
    s32 offset;

    self->slotEmpty[row] = 0;
    offset = row * 0x70;
    offset = offset + (s32)self;
    MemCopy32((void *)offset, src, 0x70);
    UpdateSaveChecksum(self);
}

/* Marks row `row` as explicitly selected/edited and refreshes the
 * checksum. See `include/settings_sync.h`'s `slotEmpty` field
 * comment, which already anticipated this function (matched from a
 * later chunk, issue #5, before this one). */
void EraseSaveSlot(struct save_data *self, s32 row)
{
    self->slotEmpty[row] = 1;
    UpdateSaveChecksum(self);
}

/* Zeroes the whole 0x200-byte record via a DMA16 fill, marks every row
 * "selected" (EraseSaveSlot), stamps the two fixed marker
 * bytes, clears `flags`/`field_1fb`, then refreshes the checksum. */
void ResetSaveData(struct save_data *self)
{
    s32 i;

    DmaFill16(3, 0, self, sizeof(*self));
    for (i = 0; i <= 3; i++) {
        EraseSaveSlot(self, i);
    }
    self->magic = 0x43;
    self->versionNibble = 0x12;
    self->flags = 0;
    self->field_1fb = 0;
    UpdateSaveChecksum(self);
}

u8 IsSaveSlotEmpty(struct save_data *self, s32 rowIndex)
{
    return self->slotEmpty[rowIndex];
}

u8 TestSaveFlags(struct save_data *self, u8 flags)
{
    MATCH_HOLD_REG(u8, v, r1);
    u8 result;

    v = flags & self->flags;
    result = v;
    if (v != 0) {
        result = 1;
    }
    return result;
}

void ClearSaveFlags(struct save_data *self, u8 flags)
{
    self->flags &= ~flags;
    UpdateSaveChecksum(self);
}
