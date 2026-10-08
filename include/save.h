#ifndef GUARD_SAVE_H
#define GUARD_SAVE_H

/* The save subsystem (src/save/): the cartridge save data in EEPROM, the
 * save menu, and the save transfer over the link cable. The save data
 * and the transfer are classes SaveData and SaveTransfer
 * (save_data.hpp; save_data.h has the slot layout), the menu's class is
 * in save_menu.hpp (save_menu.h has its slot summaries).
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

/* The save data's and the save transfer's methods (C++,
 * include/save_data.hpp: classes SaveData and SaveTransfer) have no C
 * caller and no C prototype. */

/* The save menu (C++, include/save_menu.hpp: class SaveMenu; the methods
 * have no C caller and no C prototype). Its C-linkage functions, for
 * game_frame.cpp: src/save/save_menu.cpp */
extern void CloseSaveMenu(void);
extern void OpenSaveMenu(void);

/* src/save/save_menu_input.cpp (C linkage): the menu's loop */
extern u8 RunSaveMenu(u32 state, u32 cursor);

#endif /* GUARD_SAVE_H */
