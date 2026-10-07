#ifndef GUARD_SAVE_H
#define GUARD_SAVE_H

/* The save subsystem (src/save/): the cartridge save data in EEPROM, the
 * save menu, and the save transfer over the link cable. The save data
 * and transfer layouts are in settings_sync.h, the menu's in
 * save_menu.h.
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions. A .c file that needs a different local declaration
 * for codegen keeps it as an asm-label alias with a `codegen:` comment
 * (docs/headers_plan.md). */

#include "core.h"
#include "byte_arg.h"
#include "save_menu.h"
#include "settings_sync.h"

/* The save menu, between OpenSaveMenu and CloseSaveMenu. Defined in
 * src/iwram/iwram_data.c. */
extern struct save_menu *gSaveMenu;

/* Set until the first EEPROM access has run AgbEepromInit
 * (src/iwram/iwram_data.c). */
extern u8 gEepromNeedsInit;

/* The "data from Crash Bandicoot 2/3" messages SaveMenuLinkInput shows
 * when the save received over the link cable is another game's
 * (src/iwram/iwram_data.c). */
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

/* src/save/save_data.c */
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

/* src/save/save_transfer.c */
extern void SetSaveFlags(struct save_data *self, u8 flags);
extern void SendSaveTransferChunk(struct settings_sync_pump *self);
extern void ReceiveSaveTransferChunk(struct settings_sync_pump *self, s32 playerIndex);

/* src/save/save_transfer_poll.c */
extern s32 PollSaveTransfer(struct settings_sync_pump *self);

/* src/save/save_menu.c */
extern void SaveMenuMessageInput(struct save_menu *self, u32 keys);
extern void CommitSaveMenuFrame(struct save_menu *self);
extern void CloseSaveMenu(void);
extern void OpenSaveMenu(void);

/* src/save/save_menu_draw.c */
extern s32 LinkExchangeSaveData(struct save_menu *self);
extern void DrawSaveMenuMessageLines(struct save_menu *self, s32 label1, s32 label2);
extern void DrawSaveMenuCancel(struct save_menu *self, u8 highlight);
extern void DrawYesNoPrompt(struct save_menu *self, s32 value);
extern void DrawSaveSlotStats(struct save_menu *self, s32 label1, s32 label2, s32 rowIdx,
                              struct byte_arg flag);
extern void DrawSaveSlots(struct save_menu *self, void *handle, s32 selectedIndex);
extern void InitSaveMenuIcons(struct save_menu *self);

/* src/save/save_menu_input.c */
extern void SetSaveTransferRecord(struct settings_sync_pump *self, struct save_data *tmpl);
extern void *GetSaveTransferData(struct settings_sync_pump *self);
extern void ResetSaveTransfer(struct settings_sync_pump *self);
extern u8 RunSaveMenu(u32 state, u32 cursor);
extern struct save_menu *InitSaveMenu(struct save_menu *self);
extern void DestroySaveMenu(struct save_menu *self, u32 flags);
extern void SaveMenuInput(struct save_menu *self, u32 keys);
extern void SaveMenuMainInput(struct save_menu *self, u32 keys);
extern void SaveMenuMoveCursor(struct save_menu *self, u32 keys);
extern void SaveMenuLoadInput(struct save_menu *self, u32 keys, void *handle);
extern void SaveMenuLinkInput(struct save_menu *self);
extern void SaveGameToSlot(struct save_menu *self, s32 rowIndex);
extern void SaveMenuOverwriteInput(struct save_menu *self, u32 keys);
extern void SaveMenuSaveInput(struct save_menu *self, u32 keys);
extern void SaveMenuDeleteInput(struct save_menu *self, u32 keys);
extern void SaveMenuConfirmDeleteInput(struct save_menu *self, u32 keys);
extern void DrawSaveMenuMain(struct save_menu *self);

/* src/save/save_menu_ui.c */
extern void LoadSaveMenuBg(struct save_menu *self);
extern void RefreshSaveSlotSummaries(struct save_menu *self, void *handle);
extern void LoadSaveMenuData(struct save_menu *self);
extern void SummarizeProgress(struct save_menu *self, struct settings_row_stats *dest, void *src);
extern void DrawEmptySlotLabel(struct save_menu *self, s32 x, s32 y, u8 highlight);
extern void DrawSaveMenuTitle(struct save_menu *self, s32 labelIndex);
extern s32 GetSaveMenuBlinkPalette(struct save_menu *self);
extern void EndLinkSaveTransfer(struct save_menu *self);
extern void BeginLinkSaveTransfer(struct save_menu *self);
extern void DrawSaveMenuConfirmDelete(struct save_menu *self);
extern void DrawSaveMenuDelete(struct save_menu *self);
extern void DrawSaveMenuOverwrite(struct save_menu *self);
extern void DrawSaveMenuSave(struct save_menu *self);
extern void DrawSaveMenuMessage(struct save_menu *self);
extern void DrawSaveMenuLoadLink(struct save_menu *self);
extern void DrawSaveMenuLoad(struct save_menu *self);
extern void DrawSaveMenu(struct save_menu *self);
extern void DeleteSaveSlot(struct save_menu *self, s32 row);

#endif /* GUARD_SAVE_H */
