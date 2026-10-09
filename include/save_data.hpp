#ifndef GUARD_SAVE_DATA_HPP
#define GUARD_SAVE_DATA_HPP

/* The save data and its link transfer as C++ (#751, docs/cplusplus.md,
 * "The link session, the save data and the save transfer"): class SaveData, the 0x200-byte
 * record stored in the cartridge EEPROM, and class SaveTransfer, the
 * envelope that sends one over the link cable and receives the other
 * console's. The save menu holds two SaveDatas (`new SaveData`, the
 * cartridge's and the received one) and makes a SaveTransfer for each
 * link exchange (SaveMenu::LinkExchange); cxx_symbols.txt maps the
 * methods onto their C names.
 *
 *   src/save/save_data.cpp           SaveData (all but SetFlags)
 *   src/save/save_transfer.cpp       SaveData::SetFlags, SaveTransfer's
 *                                    SendChunk and ReceiveChunk
 *   src/save/save_transfer_poll.cpp  SaveTransfer::Poll
 *   src/save/save_menu_input.cpp     SaveTransfer's SetRecord, GetData,
 *                                    Reset
 *
 * Neither has a vtable, a constructor or a destructor: `new` and
 * `delete` of them are plain OperatorNew and OperatorDelete calls, as
 * the ROM has them (no null test before the delete).
 *
 * `#pragma interface`: no vtable to emit, and no out-of-line copies of
 * the inline methods. */
#pragma interface

extern "C" {
#include "core.h"
#include "save_data.h"
}

/* The 0x200-byte save data, as stored in the cartridge EEPROM
 * (ReadSaveData/WriteSaveData, and Load/Store with retries and
 * validation): four 0x70-byte save slots (ReadSlot/WriteSlot/EraseSlot;
 * each holds the 0x68-byte progress block of the level state, the level,
 * and the sound and music volumes - see SaveMenu::SaveToSlot), a
 * per-slot "empty" flag, two marker bytes ('C' and 0x12), a flag byte
 * and an additive word-sum checksum over the first 0x1fc bytes
 * (UpdateChecksum/CheckChecksum). The save menu (gSaveMenu,
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
    u8 unused_1c0[0x34];
    u8 slotEmpty[4];  // 0x1f4 - IsSlotEmpty; EraseSlot sets it
    u8 magic;         // 0x1f8 - 'C' (0x43) after Reset
    u8 versionNibble; // 0x1f9 - 0x12 after Reset; GetGameId reads its high nibble
    u8 flags;         // 0x1fa - bitmask, TestFlags/ClearFlags/SetFlags
    u8 field_1fb;     // 0x1fb - zeroed by Reset, otherwise untouched
    u32 checksum;     // 0x1fc - UpdateChecksum/CheckChecksum

    /* The checksum test, which Validate inlines and CheckChecksum is
     * the out-of-line copy of: the additive word sum of the first 0x1fc
     * bytes (127 words) against `checksum`. */
    u32 ChecksumOk()
    {
        u32 *p = (u32 *)this;
        u32 sum = 0;
        s32 i;
        u32 result;

        for (i = 0x7e; i >= 0; i--) {
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
        versionNibble = 0x12;
        flags = 0;
        field_1fb = 0;
    }

    /* src/save/save_data.cpp */
    s32 Load();                         // LoadSaveData
    void Validate();                    // ValidateSaveData (UNUSED)
    u32 CheckChecksum();                // CheckSaveChecksum
    void UpdateChecksum();              // UpdateSaveChecksum
    u32 GetGameId();                    // GetSaveGameId
    s32 Store();                        // StoreSaveData
    void ReadSlot(s32 row, void *dst);  // ReadSaveSlot
    void WriteSlot(s32 row, void *src); // WriteSaveSlot
    void EraseSlot(s32 row);            // EraseSaveSlot
    void Reset();                       // ResetSaveData
    u8 IsSlotEmpty(s32 row);            // IsSaveSlotEmpty
    u8 TestFlags(u8 mask);              // TestSaveFlags
    void ClearFlags(u8 mask);           // ClearSaveFlags

    /* src/save/save_transfer.cpp */
    void SetFlags(u8 mask); // SetSaveFlags
};

COMPILE_TIME_ASSERT(save_data_hpp, sizeof(SaveData) == 0x200);

/* A transient SIO send/receive envelope wrapping a SaveData copy - made
 * for each "connecting..." spinner-dialog session (SaveMenu::
 * LinkExchange, src/save/save_menu_draw.cpp) and deleted with it.
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
    u8 data[sizeof(SaveData)]; // 0x010 - the received record's raw bytes
    u8 *writePtr;              // 0x210 - write cursor into `data`
    /* 0x214 - set 1 once `remaining` fully drains (send complete) */
    u32 sendDone;
    /* 0x218 - set 1 once `totalReceived` reaches sizeof(data) (receive complete) */
    u32 receiveDone;
    u32 settleTimer; // 0x21c - elapsed-poll counter, Poll

    /* src/save/save_transfer.cpp */
    void SendChunk();                   // SendSaveTransferChunk
    void ReceiveChunk(s32 playerIndex); // ReceiveSaveTransferChunk

    /* src/save/save_transfer_poll.cpp */
    s32 Poll(); // PollSaveTransfer

    /* src/save/save_menu_input.cpp */
    void SetRecord(SaveData *record); // SetSaveTransferRecord
    void *GetData();                  // GetSaveTransferData
    void Reset();                     // ResetSaveTransfer
};

COMPILE_TIME_ASSERT(save_data_hpp, sizeof(SaveTransfer) == 0x220);

#endif /* !GUARD_SAVE_DATA_HPP */
