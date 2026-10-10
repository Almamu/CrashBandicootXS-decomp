#include "audio.hpp"
#include "save_data.hpp"

extern "C" {
#include "core.h"
#include "audio.h"
#include <agb_eeprom.h>
#include "gba/dma_macros.h"
#include "save.h"
#include "system.h"
#include "globals.h"
}

/* Reads the save data: `gEepromConfig->maxCount` 8-byte blocks from
 * the EEPROM chip (the SDK's `EEPROMRead`) into a stack
 * buffer sized `len` (always 0x200, `sizeof(struct
 * save_data)`, from every call site), then copies the whole
 * buffer into `this`. One-time-inits the EEPROM chip config
 * (`EEPROMConfigure`) and claims hardware timer 2 for the transfer
 * (`SetEepromTimerIntr`, installing its handler straight into the Timer 2
 * slot of the IRQ table, `gIntrTableTimer2`) on the way in. Returns -1 on any block-read failure
 * (buffer left untouched) or 0 on success.
 *
 * The IME-save/IE-clear/IME-restore snippet (repeated once per exit
 * path) matches the ROM's exact "no extra copy" shape here as plain
 * C (`u16 savedIme = REG_IME; ...`), the same phrasing already proven
 * for `LinkSession::Stop` (src/link/link_handshake.cpp) - the previously
 * suspected register-pressure gap didn't reproduce with this
 * function's actual field/loop structure. Byte-identical to the ROM,
 * confirmed via a direct `.text`-section `cmp` against
 * `raw_08002868_target.o` (not just objdiff-cli, whose per-symbol
 * instruction diff misreports the trailing literal-pool word at this
 * exact symbol boundary as a size mismatch even though the raw bytes
 * are identical). */
s32 SaveData::Read(s32 len)
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
        REG_IE &= ~INTR_FLAG_TIMER2;
        REG_IME = savedIme;
        REG_IME = 1;
    }

    MemCopy32(this, buf, len);
    return 0;

fail_restore:
    {
        u16 savedIme = REG_IME;
        REG_IME = 0;
        REG_IE &= ~INTR_FLAG_TIMER2;
        REG_IME = savedIme;
        REG_IME = 1;
    }
    return -1;
}

/* EEPROM "save" - counterpart to `SaveData::Read`: same one-time
 * chip-config init and timer-2 claim, but copies `this` into a stack
 * buffer *after* the chip-config check (not before, matching the
 * ROM's own instruction order), then writes it out `maxCount` 8-byte
 * blocks at a time (the SDK's `EEPROMWrite1_check`). Same -1-on-failure/
 * 0-on-success return and IME-save/IE-clear/IME-restore snippet as
 * `SaveData::Read`, byte-identical as plain C for the same reason (see
 * that function's doc comment). */
s32 SaveData::Write(s32 len)
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

    MemCopy32(buf, this, len);

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
        REG_IE &= ~INTR_FLAG_TIMER2;
        REG_IME = savedIme;
        REG_IME = 1;
    }

    return 0;

fail_restore:
    {
        u16 savedIme = REG_IME;
        REG_IME = 0;
        REG_IE &= ~INTR_FLAG_TIMER2;
        REG_IME = savedIme;
        REG_IME = 1;
    }
    return -1;
}

/* Loads the settings record from EEPROM (`SaveData::Read`, retried up to
 * 3 times), muting the music player across the transfer (stop before,
 * resume after, matching `src/audio/audio.cpp`'s established
 * `AudioContext` helpers), then validates the loaded record's two
 * marker bytes and checksum. Returns 4 (EEPROM read failed after
 * retries), 2 (bad `magic` marker), 1 (bad `versionNibble`
 * marker), 3 (checksum mismatch) or 0 (fully valid). */
s32 SaveData::Load()
{
    AudioContext *audio;
    bool wasPlaying;
    u32 savedSong;
    s32 i;
    s32 result;

    audio = gAudioContext;
    wasPlaying = audio->IsPlaying();

    savedSong = audio->GetCurrentSong();
    if (wasPlaying) {
        gAudioContext->StopSong();
    }

    i = 0;
    do {
        result = Read(0x200);
        i++;
    } while (i <= 2 && result != 0);

    if (wasPlaying) {
        gAudioContext->PlaySong(savedSong);
    }

    if (result != 0) {
        return 4;
    }
    if (magic != 0x43) {
        return 2;
    }
    if (versionNibble != 0x12) {
        return 1;
    }
    if ((u8)CheckChecksum() == 0) {
        return 3;
    }
    return 0;
}

/* Validates the record's checksum (the inline ChecksumOk, which
 * CheckChecksum below is the out-of-line copy of) and, if it fails,
 * repairs the record in place, as Reset does: clears the 0x200 bytes
 * (DmaClear16), marks every row selected (`SaveData::EraseSlot`),
 * re-stamps the header (StampHeader: the two marker bytes, `flags` and
 * `field_1fb` cleared) and refreshes the checksum
 * (`SaveData::UpdateChecksum`).
 *
 * The ROM computes the four header addresses before the EraseSlot loop
 * (r6, sb, r7, r8). With the stores written out here gcc computes them
 * after the loop; through the StampHeader inline it hoists them, and
 * DmaClear16 (its `_dest`/`_size` locals) gives the ROM's registers,
 * where DmaFill16 swaps three of them (#662 round 2; the C took the
 * addresses into locals, with a pin and an extra reference for the
 * order). */
void SaveData::Validate()
{
    s32 i;

    if (!ChecksumOk()) {
        DmaClear16(3, this, sizeof(*this));
        for (i = 0; i <= 3; i++)
            EraseSlot(i);
        StampHeader();
        UpdateChecksum();
    }
}

/* Recomputes this record's additive word-sum checksum over its first
 * 0x1fc bytes (127 words) and compares it against the stored
 * `checksum` field, returning 1 on match. Counterpart to `SaveData::UpdateChecksum`
 * below, which stores instead of comparing. */
u32 SaveData::CheckChecksum()
{
    return ChecksumOk();
}

/* Recomputes and stores this record's additive word-sum checksum over
 * its first 0x1fc bytes (127 words) into `checksum`. Every function
 * that mutates `flags`/`slotEmpty` calls this afterward to keep the
 * checksum in sync - see src/save/save_data.cpp's header
 * comment, which already anticipated this function (it was matched
 * from a later chunk, issue #5, before this one). */
void SaveData::UpdateChecksum()
{
    u32 *p = (u32 *)this;
    u32 sum = 0;
    s32 i;

    for (i = 0x7e; i >= 0; i--) {
        sum += *p++;
    }
    checksum = sum;
}

/* `versionNibble`'s high-nibble accessor. */
u32 SaveData::GetGameId()
{
    return versionNibble >> 4;
}

/* Saves the settings record to EEPROM (`SaveData::Write`, retried up to 5
 * times), muting the music player across the transfer the same way
 * `SaveData::Load` (src/save/save_data.cpp) does (checksum
 * refreshed first via `SaveData::UpdateChecksum`, before the mute). Returns 4
 * (EEPROM write failed after retries) or 0 (success). */
s32 SaveData::Store()
{
    AudioContext *audio;
    bool wasPlaying;
    u32 savedSong;
    s32 i;
    s32 result;

    audio = gAudioContext;
    wasPlaying = audio->IsPlaying();

    savedSong = audio->GetCurrentSong();
    UpdateChecksum();

    if (wasPlaying) {
        gAudioContext->StopSong();
    }

    i = 0;
    do {
        result = Write(0x200);
        i++;
    } while (i <= 4 && result != 0);

    if (wasPlaying) {
        gAudioContext->PlaySong(savedSong);
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
void SaveData::ReadSlot(s32 row, void *dst)
{
    if (slotEmpty[row] == 0) {
        s32 offset;

        offset = row * sizeof(struct save_slot);
        offset = offset + (s32)this;
        MemCopy32(dst, (void *)offset, sizeof(struct save_slot));
    }
}

/* Force-writes `src` into row `row`'s 0x70-byte slot, clears the row's
 * `slotEmpty` flag (marking it "not explicitly edited" again), and
 * refreshes the checksum. */
void SaveData::WriteSlot(s32 row, void *src)
{
    s32 offset;

    slotEmpty[row] = 0;
    offset = row * sizeof(struct save_slot);
    offset = offset + (s32)this;
    MemCopy32((void *)offset, src, sizeof(struct save_slot));
    UpdateChecksum();
}

/* Marks row `row` as explicitly selected/edited and refreshes the
 * checksum. See `include/save_data.hpp`'s `slotEmpty` field
 * comment, which already anticipated this function (matched from a
 * later chunk, issue #5, before this one). */
void SaveData::EraseSlot(s32 row)
{
    slotEmpty[row] = 1;
    UpdateChecksum();
}

/* Zeroes the whole 0x200-byte record via a DMA16 fill, marks every row
 * "selected" (SaveData::EraseSlot), stamps the two fixed marker
 * bytes, clears `flags`/`field_1fb`, then refreshes the checksum. */
void SaveData::Reset()
{
    s32 i;

    DmaFill16(3, 0, this, sizeof(*this));
    for (i = 0; i <= 3; i++) {
        EraseSlot(i);
    }
    StampHeader();
    UpdateChecksum();
}

u8 SaveData::IsSlotEmpty(s32 rowIndex)
{
    return slotEmpty[rowIndex];
}

/* Whether any of `mask`'s bits is set in `flags`.
 * UNUSED - no caller anywhere in the ROM (no `bl` to TestSaveFlags and
 * no pointer to it in the data).
 *
 * The `bool` return is what gives the ROM's code, with no workaround
 * (#662 round 9, a sweep of the return, parameter and `flags` types with
 * the local spellings: 14964 variants). The conversion to bool expands
 * as "the value, then 1 if it is nonzero" with the test on the AND
 * itself, and the u8 parameter keeps its extension at the entry. With
 * any integer return type (u8 before, and every other one in the sweep)
 * regmove's optimize_reg_copy_1 moves the test onto the result's copy,
 * which is why rounds 2-8 kept `v` pinned to r1. `(mask & flags) != 0`
 * and `(flags & mask) ? 1 : 0` give the same code; the `flags` field as
 * a u8 bitfield (`u8 flags : 8`, `u32 flags : 8`) does too, an s8 one
 * doesn't. */
bool SaveData::TestFlags(u8 mask)
{
    return flags & mask;
}

void SaveData::ClearFlags(u8 mask)
{
    flags &= ~mask;
    UpdateChecksum();
}

void SaveData::SetFlags(u8 mask)
{
    flags |= mask;
    UpdateChecksum();
}
