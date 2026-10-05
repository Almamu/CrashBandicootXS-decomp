#include "core.h"
#include "audio.h"
#include "settings_sync.h"

extern void MemCopy32(void *dst, void *src, s32 len);
extern void EraseSaveSlot(struct save_data *self, s32 row);
extern void UpdateSaveChecksum(struct save_data *self);

/* Validates the record's checksum (inline word-sum, same shape as
 * `CheckSaveChecksum` below but not a call to it - the ROM genuinely inlines
 * this one) and, if it fails, repairs the record in place: DMA-fills
 * the whole 0x200 bytes with 0 (raw DMA3 register pokes rather than a
 * `DmaFill16` call - a different, earlier style than
 * `src/graphics/settings_menu8.c`'s `ResetSaveData` uses for the same
 * "reset to blank" operation), marks every row selected
 * (`EraseSaveSlot`), re-stamps the two marker bytes, clears
 * `flags`/`field_1fb`, and refreshes the checksum (`UpdateSaveChecksum`).
 *
 * Parked as NAKED until the near-miss polish pass
 * (docs/matching/near-miss-polish.md): the ROM computes `&flags` before
 * `&field_1fb` yet still gives `flags` r7 (global-alloc's first pick).
 * With `flags` computed first its live range is one insn longer, so it
 * ranked below `field_1fb` and the two swapped r7/r8. The empty
 * `asm("" : : "r"(flags))` below emits nothing; it adds one reference
 * to `flags`, which lifts its allocation priority (floor_log2(refs) *
 * refs / live length) above `field_1fb`'s. */
/* An inlined copy of CheckSaveChecksum below. */
static inline u32 checksum_ok(struct save_data *self)
{
    u32 *p = (u32 *)self;
    u32 sum = 0;
    s32 i;
    register u32 result asm("r1");

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
        register u8 *marker asm("r6");
        u8 *flags, *f1fb, *version;

        DmaFill16(3, 0, self, 0x200);
        i = 0;
        marker = &self->magic;
        version = &self->versionNibble;
        flags = &self->flags;
        /* No code: one extra use of `flags` for global-alloc's ranking. */
        asm("" : : "r"(flags));
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
    register u32 result asm("r1");

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
 * checksum in sync - see src/graphics/settings_menu8.c's header
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

extern struct AudioContext *gAudioContext;
extern u32 GetCurrentSong(struct AudioContext *self);
extern void StopSong(struct AudioContext *self);
extern void PlaySong(struct AudioContext *self, u32 id);
extern s32 WriteSaveData(void *self, s32 len);

/* Saves the settings record to EEPROM (`WriteSaveData`, retried up to 5
 * times), muting the music player across the transfer the same way
 * `LoadSaveData` (src/graphics/settings_menu8d.c) does (checksum
 * refreshed first via `UpdateSaveChecksum`, before the mute). Returns 4
 * (EEPROM write failed after retries) or 0 (success). */
s32 StoreSaveData(struct save_data *self)
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
        register s32 offset asm("r1");

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
    register s32 offset asm("r0");

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
/* Trailing byte-padding mismatch fix: GAS's default Thumb code
 * alignment filler is the `mov r8, r8` NOP (0x46c0), but the ROM pads
 * this function's tail with a zero halfword instead (see
 * docs/matching.md's alignment-padding gotcha / the
 * matching_decomp_alignment_fix convention). */
asm(".align 2, 0");
