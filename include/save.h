#ifndef GUARD_SAVE_H
#define GUARD_SAVE_H

/* The save subsystem (src/save/): the cartridge save data in EEPROM, the
 * save menu, and the save transfer over the link cable. The save data
 * and transfer layouts are in save_data.h, the menu's class in
 * save_menu.hpp (save_menu.h has its slot summaries).
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions. A .c file that needs a different local declaration
 * for codegen keeps it as an asm-label alias with a `codegen:` comment
 * (docs/headers_plan.md). */

#include "core.h"
#include "save_menu.h"
#include "save_data.h"

/* The save menu, between OpenSaveMenu and CloseSaveMenu. Defined in
 * src/iwram/iwram_data.cpp. The C++ files see it as its class, SaveMenu
 * (save_menu.hpp). */
#ifdef __cplusplus
extern class SaveMenu *gSaveMenu;
#else
extern struct save_menu *gSaveMenu;
#endif

/* Set until the first EEPROM access has run AgbEepromInit
 * (src/iwram/iwram_data.cpp). */
extern u8 gEepromNeedsInit;

/* The "data from Crash Bandicoot 2/3" messages SaveMenuLinkInput shows
 * when the save received over the link cable is another game's
 * (src/iwram/iwram_data.cpp). */
extern const u8 *gCrash2LinkTextPtr;
extern const u8 *gCrash3LinkTextPtr;

/* The save menu's tables (src/data/menu_tables_16b138.c): the main
 * options' text ids, the cursor text and the four menu palettes. */
extern const s32 gSaveMenuOptions[5];
extern const u8 gMenuCursorText[];
extern const u16 gSaveMenuPalette0[16];
extern const u16 gSaveMenuPalette1[16];
extern const u16 gSaveMenuPalette2[16];
extern const u16 gSaveMenuPalette3[16];

/* src/save/save_data.cpp */
extern s32 ReadSaveData(void *self, s32 len);
extern s32 WriteSaveData(void *self, s32 len);
extern s32 LoadSaveData(struct save_data *self);
extern void ValidateSaveData(struct save_data *self);
extern u32 CheckSaveChecksum(struct save_data *self);
extern void UpdateSaveChecksum(struct save_data *self);
extern u32 GetSaveGameId(struct save_data *self);
extern s32 StoreSaveData(struct save_data *self);
extern void ReadSaveSlot(struct save_data *self, s32 row, void *dst);
extern void WriteSaveSlot(struct save_data *self, s32 row, void *src);
extern void EraseSaveSlot(struct save_data *self, s32 row);
extern void ResetSaveData(struct save_data *self);
extern u8 IsSaveSlotEmpty(struct save_data *self, s32 row);
extern u8 TestSaveFlags(struct save_data *self, u8 flags);
extern void ClearSaveFlags(struct save_data *self, u8 flags);

/* src/save/save_transfer.cpp */
extern void SetSaveFlags(struct save_data *self, u8 flags);
extern void SendSaveTransferChunk(struct save_transfer *self);
extern void ReceiveSaveTransferChunk(struct save_transfer *self, s32 playerIndex);

/* src/save/save_transfer_poll.cpp */
extern s32 PollSaveTransfer(struct save_transfer *self);

/* The save menu (C++, include/save_menu.hpp: class SaveMenu; the methods
 * have no C caller and no C prototype). Its C-linkage functions, for
 * game_frame.cpp: src/save/save_menu.cpp */
extern void CloseSaveMenu(void);
extern void OpenSaveMenu(void);

/* src/save/save_menu_input.cpp (C linkage): the save transfer's
 * accessors, and the menu's loop */
extern void SetSaveTransferRecord(struct save_transfer *self, struct save_data *tmpl);
extern void *GetSaveTransferData(struct save_transfer *self);
extern void ResetSaveTransfer(struct save_transfer *self);
extern u8 RunSaveMenu(u32 state, u32 cursor);

#endif /* GUARD_SAVE_H */
