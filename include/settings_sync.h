#ifndef __SETTINGS_SYNC_H__
#define __SETTINGS_SYNC_H__

/* The 0x200-byte save data, as stored in the cartridge EEPROM
 * (ReadSaveData/WriteSaveData, and LoadSaveData/StoreSaveData with
 * retries and validation): four 0x70-byte save slots
 * (ReadSaveSlot/WriteSaveSlot/EraseSaveSlot; each holds the 0x68-byte
 * progress block of the level state, the level, and the sound and
 * music volumes - see SaveGameToSlot), a per-slot "empty" flag, two
 * marker bytes ('C' and 0x12), a flag byte and an additive word-sum
 * checksum over the first 0x1fc bytes (UpdateSaveChecksum/
 * CheckSaveChecksum). The save menu (gSaveMenu, save_menu.h)
 * holds two copies: the cartridge's (`field_8c`) and the one received
 * over the link cable (`field_90`). Formerly
 * `struct settings_sync_record`. See docs/matching/archive/issue-5-overlay-ui-sync.md.
 * Shared (via this header) between src/save/save_data.c and
 * save_menu_input.c, split apart so the two parked
 * functions between them (SendSaveTransferChunk/ReceiveSaveTransferChunk/PollSaveTransfer,
 * SaveGameToSlot) can stay raw asm without breaking ROM link order. */
struct save_data {
    u8 unused_000[0x1f4];
    u8 slotEmpty[4]; /* 0x1f4 - IsSaveSlotEmpty here; EraseSaveSlot (still raw) sets it */
    u8 magic;       /* 0x1f8 - init'd to 'C' (0x43) by ResetSaveData */
    u8 versionNibble;     /* 0x1f9 - init'd to 0x12; high nibble read by GetSaveGameId (still raw) */
    u8 flags;               /* 0x1fa - bitmask, TestSaveFlags/ClearSaveFlags/SetSaveFlags */
    u8 field_1fb;             /* 0x1fb - zeroed by ResetSaveData, otherwise untouched in this chunk */
    u32 checksum;               /* 0x1fc - UpdateSaveChecksum/CheckSaveChecksum (still raw) */
};
COMPILE_TIME_ASSERT(settings_sync_h, sizeof(struct save_data) == 0x200);

/* A transient SIO send/receive envelope wrapping a save_data
 * copy - allocated per "connecting..." spinner-dialog session
 * (LinkExchangeSaveData, src/save/save_menu_draw.c, parked) and torn down
 * with it. `tmpl`/`cursor` stream a save_data's bytes out to
 * the SIO session's per-player ring buffer (SendSaveTransferChunk); `data`
 * receives the remote side's copy of the same shape from its own ring
 * buffer (ReceiveSaveTransferChunk), with `writePtr` as the fill cursor. See
 * docs/matching/archive/issue-5-overlay-ui-sync.md for the full protocol
 * write-up. */
struct settings_sync_pump {
    u32 remaining;      /* 0x000 - bytes left to send out of `tmpl`, reset to sizeof(data) */
    u32 totalReceived;    /* 0x004 - running total of bytes received into `data` */
    struct save_data *tmpl; /* 0x008 - the record SetSaveTransferRecord copies in */
    u8 *cursor;                          /* 0x00c - read cursor into `tmpl` while draining `remaining` */
    u8 data[sizeof(struct save_data)]; /* 0x010 - the received record's raw bytes */
    u8 *writePtr;                                    /* 0x210 - write cursor into `data` */
    u32 sendDone;                                     /* 0x214 - set 1 once `remaining` fully drains (send complete) */
    u32 receiveDone;                                       /* 0x218 - set 1 once `totalReceived` reaches sizeof(data) (receive complete) */
    u32 settleTimer;                                         /* 0x21c - elapsed-poll counter, PollSaveTransfer */
};
COMPILE_TIME_ASSERT(settings_sync_h, sizeof(struct settings_sync_pump) == 0x220);

#endif /* __SETTINGS_SYNC_H__ */
