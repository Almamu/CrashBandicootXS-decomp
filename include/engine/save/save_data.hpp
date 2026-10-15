#ifndef GUARD_SAVE_DATA_HPP
#define GUARD_SAVE_DATA_HPP

/* The save data and its link transfer as C++ (#751, docs/cplusplus.md,
 * "The link session, the save data and the save transfer"): class SaveData, the
 * record stored in the cartridge EEPROM (0x200 bytes, the whole 4 Kbit
 * EEPROM, in the ROM), and class SaveTransfer, the
 * envelope that sends one over the link cable and receives the other
 * console's. The save menu holds two SaveDatas (`new SaveData`, the
 * cartridge's and the received one) and makes a SaveTransfer for each
 * link exchange (SaveMenu::LinkExchange); cxx_symbols.txt maps the
 * methods onto their C names.
 *
 *   src/engine/save/save_data.cpp           SaveData
 *   src/engine/save/save_transfer.cpp       SaveTransfer's SendChunk and
 *                                    ReceiveChunk
 *   src/engine/save/save_transfer_state.cpp SaveTransfer's Poll, SetRecord,
 *                                    GetData, Reset
 *
 * Neither has a vtable, a constructor or a destructor: `new` and
 * `delete` of them are plain OperatorNew and OperatorDelete calls, as
 * the ROM has them (no null test before the delete).
 *
 * Both are in the namespace Engine::Save (#1033, docs/cplusplus.md
 * "Namespaces"): `Read__Q36Engine4Save8SaveDatai` & co. The slot's
 * layout, struct save_slot and the GameProgress it holds, is the game's
 * and stays outside.
 *
 * `#pragma interface`: no vtable to emit, and no out-of-line copies of
 * the inline methods. */
#pragma interface

#include "game/level/game_progress.hpp"

extern "C" {
#include "core.h"
#include "constants/save.h"
}

/* One save slot (0x70 bytes in the ROM), as SaveData::ReadSlot/WriteSlot copy it:
 * the level state's packed progress block (GameProgress;
 * PackSaveData/UnpackSaveData), then the current level and the sound
 * and music volumes (SaveMenu::SaveToSlot builds one,
 * SaveMenu::LoadInput restores one). */
struct save_slot {
    Game::Level::GameProgress progress; /* 0x00 */
    u8 level;                           /* 0x68 */
    u16 sfxVolume;                      /* 0x6a */
    u16 musicVolume;                    /* 0x6c */
};
/* 0x68 and 0x70 with the ROM's 25 levels (GameProgress's size). */
COMPILE_TIME_ASSERT(save_data_hpp,
                    offsetof(struct save_slot, level) == sizeof(Game::Level::GameProgress));
COMPILE_TIME_ASSERT(save_data_hpp,
                    sizeof(struct save_slot) == sizeof(Game::Level::GameProgress) + 8);

namespace Engine
{
namespace Save
{

/* The save data, as stored in the cartridge EEPROM (Read/Write, and
 * Load/Store with retries and validation): SAVE_DATA_SIZE bytes, 0x200 in
 * the ROM. Four save slots (0x70 bytes in the ROM; ReadSlot/WriteSlot/
 * EraseSlot; each holds the progress block of the level state, the level,
 * and the sound and music volumes - see SaveMenu::SaveToSlot), then a
 * 12-byte trailer: a per-slot "empty" flag, two marker bytes ('C' and
 * SAVE_VERSION), a flag byte and an additive word-sum checksum over the
 * bytes before it, from SAVE_CHECKSUM_SEED (UpdateChecksum/CheckChecksum).
 * All of it comes from the level data (constants/save.h; docs/levels.md,
 * "The save"): the record is the whole 4 Kbit EEPROM while the four slots
 * fit (the ROM's 25 levels, up to 28), else a record of the slots' size
 * on the 64 Kbit EEPROM (SAVE_EEPROM_TYPE). The version and the seed are
 * 0x12 and 0 with the ROM's levels, so a save made with other levels
 * doesn't load. The save menu (SaveMenu::instance,
 * save_menu.hpp) holds two: the cartridge's (`cartSave`) and the one
 * received over the link cable (`linkSave`). Formerly `struct
 * settings_sync_record`. See
 * docs/matching/archive/issue-5-overlay-ui-sync.md. */
class SaveData
{
public:
    /* 0x000 - read and written through their offsets (ReadSlot,
     * WriteSlot: `row * 0x70 + this`) */
    struct save_slot slots[4];
    /* 0x1c0 with the ROM's 25 levels: the unused rest up to `slotEmpty`,
     * 0x34 bytes in the ROM (tools/levels.py's save_layout sizes the
     * record so that it is 0-0x34 bytes) */
    u8 unused_1c0[SAVE_DATA_SIZE - 0xc - 4 * sizeof(struct save_slot)];
    /* the trailer, the last 12 bytes (the offsets are the ROM's) */
    u8 slotEmpty[4];  // 0x1f4 - IsSlotEmpty; EraseSlot sets it
    u8 magic;         // 0x1f8 - 'C' (0x43) after Reset
    u8 versionNibble; // 0x1f9 - SAVE_VERSION after Reset; GetGameId reads its high nibble
    u8 flags;         // 0x1fa - bitmask, TestFlags/ClearFlags/SetFlags
    u8 field_1fb;     // 0x1fb - zeroed by Reset, otherwise untouched
    u32 checksum;     // 0x1fc - UpdateChecksum/CheckChecksum

    /* The checksum test, which Validate inlines and CheckChecksum is
     * the out-of-line copy of: the additive word sum of the words before
     * `checksum` (127 in the ROM), from SAVE_CHECKSUM_SEED, against
     * `checksum`. */
    u32 ChecksumOk()
    {
        u32 *p = (u32 *)this;
        u32 sum = SAVE_CHECKSUM_SEED;
        s32 i;
        u32 result;

        for (i = SAVE_DATA_SIZE / 4 - 2; i >= 0; i--) {
            sum += *p++;
        }

        result = 0;
        if (sum == checksum) {
            result = 1;
        }
        return result;
    }

    /* The blank record's header: the two markers and both flag bytes
     * clear (Validate, Reset). */
    void StampHeader()
    {
        magic = 0x43;
        versionNibble = SAVE_VERSION;
        flags = 0;
        field_1fb = 0;
    }

    /* src/engine/save/save_data.cpp */
    s32 Read(s32 len);                  // ReadSaveData
    s32 Write(s32 len);                 // WriteSaveData
    s32 Load();                         // LoadSaveData
    void Validate();                    // ValidateSaveData (UNUSED)
    u8 CheckChecksum();                 // CheckSaveChecksum
    void UpdateChecksum();              // UpdateSaveChecksum
    u32 GetGameId();                    // GetSaveGameId
    s32 Store();                        // StoreSaveData
    void ReadSlot(s32 row, void *dst);  // ReadSaveSlot
    void WriteSlot(s32 row, void *src); // WriteSaveSlot
    void EraseSlot(s32 row);            // EraseSaveSlot
    void Reset();                       // ResetSaveData
    u8 IsSlotEmpty(s32 row);            // IsSaveSlotEmpty
    bool TestFlags(u8 mask);            // TestSaveFlags
    void ClearFlags(u8 mask);           // ClearSaveFlags
    void SetFlags(u8 mask);             // SetSaveFlags
};

} // namespace Save
} // namespace Engine

COMPILE_TIME_ASSERT(save_data_hpp, sizeof(Engine::Save::SaveData) == SAVE_DATA_SIZE);
COMPILE_TIME_ASSERT(save_data_hpp, SAVE_DATA_SIZE % 8 == 0);

/* The record the link cable carries (SaveTransfer::data): 0x200 bytes,
 * the ROM's SaveData, which Crash 2 and 3 send too. A receiver (the ROM,
 * Crash 2/3) takes exactly 0x200 bytes, so a larger record (the 64 Kbit
 * save's) doesn't go whole: it goes as a LinkRecord, below
 * (SAVE_LINK_VIEW; docs/levels.md, "The link cable"). */
#define SAVE_LINK_SIZE 0x200

/* Whether the save goes over the link cable as a LinkRecord: when the
 * record isn't the link's 0x200 bytes, the 64 Kbit save's. A constant
 * condition (constants/save.h): 0 in the ROM, whose record goes as it
 * is, and SaveMenu::LinkExchange keeps only one side. */
#define SAVE_LINK_VIEW (SAVE_DATA_SIZE != SAVE_LINK_SIZE)

/* The level words a link slot holds: the ROM's 25. */
#define SAVE_LINK_LEVELS 25

/* The link record's checksum seed: the ROM's SAVE_CHECKSUM_SEED, which
 * Crash 2 and 3 check against. */
#define SAVE_LINK_CHECKSUM_SEED 0

/* A save slot as the link record holds it: the ROM's 0x70-byte
 * save_slot, the progress block with SAVE_LINK_LEVELS level words and
 * no extra flags. */
struct save_link_slot {
    u16 lives:7; // 0x00 - GameProgress's halfword
    u16 maskLevel:2;
    u16 wumpa:7;
    u8 flags;                                    // 0x02 - GameProgress::flags
    union level_record levels[SAVE_LINK_LEVELS]; // 0x04 - the first level words
    u8 level;                                    // 0x68
    u16 sfxVolume;                               // 0x6a
    u16 musicVolume;                             // 0x6c
};
COMPILE_TIME_ASSERT(save_data_hpp, sizeof(struct save_link_slot) == 0x70);

namespace Engine
{
namespace Save
{

/* The 0x200-byte view of a larger record that a 64 Kbit build sends and
 * receives over the link cable (SAVE_LINK_VIEW; SaveMenu::LinkExchange):
 * the ROM's SaveData layout, with the ROM's trailer at 0x1f4 and its
 * checksum (SAVE_LINK_CHECKSUM_SEED), so the ROM and Crash 2/3 read it
 * as an XS record (game id 1) and give the link bonus.
 *
 * Its slots hold the first SAVE_LINK_LEVELS level words of the
 * sender's, but `slotEmpty` marks them all empty: they mean something
 * only with the sender's level data, so the ROM and other builds show
 * four empty slots and load none. The real empty flags are in the ROM
 * record's unused gap, after the sender's SAVE_CHECKSUM_SEED (0 in a
 * record of the ROM, whose gap Reset clears; never 0 in a build of other
 * levels), and only a build of the same level data, so of the same seed,
 * takes the slots back (Unpack). They carry no level past the first
 * SAVE_LINK_LEVELS and no extra progress flags. */
class LinkRecord
{
public:
    struct save_link_slot slots[4]; // 0x000
    /* 0x1c0 - the ROM's unused gap: the sender's SAVE_CHECKSUM_SEED (its
     * level data's), 0 in a ROM record */
    u32 senderSeed;
    u8 senderSlotEmpty[4]; // 0x1c4 - the sender's slotEmpty
    u8 unused_1c8[0x2c];
    /* the ROM's trailer */
    u8 slotEmpty[4];  // 0x1f4 - all 1 (Pack)
    u8 magic;         // 0x1f8 - 'C'
    u8 versionNibble; // 0x1f9 - the sender's SAVE_VERSION (the game id is the high nibble)
    u8 flags;         // 0x1fa - the sender's flags (the Crash 2/3 bonus bits)
    u8 field_1fb;     // 0x1fb
    u32 checksum;     // 0x1fc - from SAVE_LINK_CHECKSUM_SEED

    /* The additive word sum of the words before `checksum`, from
     * SAVE_LINK_CHECKSUM_SEED, as SaveData::UpdateChecksum's. */
    u32 Sum()
    {
        u32 *p = (u32 *)this;
        u32 sum = SAVE_LINK_CHECKSUM_SEED;
        s32 i;

        for (i = SAVE_LINK_SIZE / 4 - 2; i >= 0; i--) {
            sum += *p++;
        }
        return sum;
    }

    /* Makes the view of `save` to send. (The record is cleared a word at
     * a time: DmaFill16's volatile would take a stack slot in
     * SaveMenu::LinkExchange even in the ROM, where this isn't called.) */
    void Pack(SaveData *save)
    {
        u32 *p = (u32 *)this;
        s32 i;
        s32 j;

        for (i = SAVE_LINK_SIZE / 4 - 1; i >= 0; i--) {
            *p++ = 0;
        }
        for (i = 0; i <= 3; i++) {
            struct save_slot *src = &save->slots[i];
            struct save_link_slot *dst = &slots[i];

            dst->lives = src->progress.lives;
            dst->maskLevel = src->progress.maskLevel;
            dst->wumpa = src->progress.wumpa;
            dst->flags = src->progress.flags;
            for (j = 0; j < SAVE_LINK_LEVELS; j++) {
                dst->levels[j] = src->progress.levels[j];
            }
            dst->level = src->level;
            dst->sfxVolume = src->sfxVolume;
            dst->musicVolume = src->musicVolume;
            senderSlotEmpty[i] = save->slotEmpty[i];
            slotEmpty[i] = 1;
        }
        senderSeed = SAVE_CHECKSUM_SEED;
        magic = 0x43;
        versionNibble = SAVE_VERSION;
        flags = save->flags;
        checksum = Sum();
    }

    /* Turns the received view into `save`, or returns false if its
     * checksum fails. `save` gets the sender's version (LinkInput reads
     * the game id from it) and flags, and the sender's slots if it has
     * this build's level data (its version and seed): the levels past
     * SAVE_LINK_LEVELS and the extra progress flags are 0. Then `save`'s
     * own checksum. */
    bool Unpack(SaveData *save)
    {
        s32 i;
        s32 j;

        if (Sum() != checksum) {
            return false;
        }
        save->Reset();
        save->versionNibble = versionNibble;
        save->flags = flags;
        if (versionNibble == SAVE_VERSION && senderSeed == SAVE_CHECKSUM_SEED) {
            for (i = 0; i <= 3; i++) {
                struct save_link_slot *src = &slots[i];
                struct save_slot *dst = &save->slots[i];

                if (senderSlotEmpty[i]) {
                    continue;
                }
                dst->progress.lives = src->lives;
                dst->progress.maskLevel = src->maskLevel;
                dst->progress.wumpa = src->wumpa;
                dst->progress.flags = src->flags;
                for (j = 0; j < SAVE_LINK_LEVELS; j++) {
                    dst->progress.levels[j] = src->levels[j];
                }
                dst->level = src->level;
                dst->sfxVolume = src->sfxVolume;
                dst->musicVolume = src->musicVolume;
                save->slotEmpty[i] = 0;
            }
        }
        save->UpdateChecksum();
        return true;
    }
};

} // namespace Save
} // namespace Engine

COMPILE_TIME_ASSERT(save_data_hpp, sizeof(Engine::Save::LinkRecord) == SAVE_LINK_SIZE);
COMPILE_TIME_ASSERT(save_data_hpp, offsetof(Engine::Save::LinkRecord, slotEmpty) == 0x1f4);
/* A larger record has more levels than a link slot holds. */
COMPILE_TIME_ASSERT(save_data_hpp, !SAVE_LINK_VIEW || LEVEL_COUNT >= SAVE_LINK_LEVELS);

/* The stack the save needs past the ROM's, which main's mem_heap_init
 * reserve adds: Read and Write's record buffer, and SaveMenu::SaveToSlot's
 * two slots below which Store runs. 0 in the ROM. */
#define SAVE_STACK_GROWTH (SAVE_DATA_SIZE - 0x200 + 2 * (sizeof(struct save_slot) - 0x70))

namespace Engine
{
namespace Save
{

/* A transient SIO send/receive envelope wrapping a SaveData copy - made
 * for each "connecting..." spinner-dialog session (SaveMenu::
 * LinkExchange, src/game/ui/save_menu_draw.cpp) and deleted with it.
 * `tmpl`/`cursor` stream a SaveData's bytes out to the link session's
 * outgoing ring (SendChunk); `data` receives the other side's copy of the
 * same shape from that player's incoming ring (ReceiveChunk), with
 * `writePtr` as the fill cursor. See
 * docs/matching/archive/issue-5-overlay-ui-sync.md for the protocol.
 * Formerly `struct settings_sync_pump`. */
class SaveTransfer
{
public:
    /* 0x000 - bytes left to send out of `tmpl`, reset to sizeof(data) */
    u32 remaining;
    u32 totalReceived; // 0x004 - running total of bytes received into `data`
    SaveData *tmpl;    // 0x008 - the record SetRecord copies in
    /* 0x00c - read cursor into `tmpl` while draining `remaining` */
    u8 *cursor;
    u8 data[SAVE_LINK_SIZE]; // 0x010 - the received record's raw bytes
    u8 *writePtr;            // 0x210 - write cursor into `data`
    /* 0x214 - set 1 once `remaining` fully drains (send complete) */
    u32 sendDone;
    /* 0x218 - set 1 once `totalReceived` reaches sizeof(data) (receive complete) */
    u32 receiveDone;
    u32 settleTimer; // 0x21c - elapsed-poll counter, Poll

    /* src/engine/save/save_transfer.cpp */
    void SendChunk();                   // SendSaveTransferChunk
    void ReceiveChunk(s32 playerIndex); // ReceiveSaveTransferChunk

    /* src/engine/save/save_transfer_state.cpp */
    s32 Poll();                       // PollSaveTransfer
    void SetRecord(SaveData *record); // SetSaveTransferRecord
    void *GetData();                  // GetSaveTransferData
    void Reset();                     // ResetSaveTransfer
};

} // namespace Save
} // namespace Engine

COMPILE_TIME_ASSERT(save_data_hpp, sizeof(Engine::Save::SaveTransfer) == 0x220);

#endif /* !GUARD_SAVE_DATA_HPP */
