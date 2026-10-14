#ifndef GUARD_SAVE_H
#define GUARD_SAVE_H

/* The save subsystem (src/engine/save/): the cartridge save data in EEPROM, the
 * save menu, and the save transfer over the link cable. The save data
 * and the transfer are classes SaveData and SaveTransfer
 * (save_data.hpp), the menu's class is in save_menu.hpp, with their
 * types. C (src/data/menu_tables_16b138.cpp) needs only the tables and the
 * C-linkage functions below.
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions. A .c file that needs a different local declaration
 * for codegen keeps it as an asm-label alias with a `codegen:` comment
 * (docs/headers.md). */

#include "core.h"

/* Set until the first EEPROM access has run AgbEepromInit
 * (src/engine/sys/iwram_data.cpp). */
extern u8 gEepromNeedsInit;

/* The "data from Crash Bandicoot 2/3" messages SaveMenuLinkInput shows
 * when the save received over the link cable is another game's
 * (src/engine/sys/iwram_data.cpp). */
extern const u8 *gCrash2LinkTextPtr;
extern const u8 *gCrash3LinkTextPtr;

/* The save menu's tables (src/data/menu_tables_16b138.cpp): the main
 * options' text ids, the cursor text and the four menu palettes. */
extern const s32 gSaveMenuOptions[5];
extern const u8 gMenuCursorText[];
extern const u16 gSaveMenuPalette0[16];
extern const u16 gSaveMenuPalette1[16];
extern const u16 gSaveMenuPalette2[16];
extern const u16 gSaveMenuPalette3[16];

/* The save data's and the save transfer's methods (C++,
 * include/engine/save/save_data.hpp: classes SaveData and SaveTransfer) have no C
 * caller and no C prototype. */

/* The save menu (C++, include/game/ui/save_menu.hpp: class SaveMenu; the methods,
 * SaveMenu::Open, Run and Close among them, have no C caller and no C
 * prototype). */

/* SaveMenu's `state` (save_menu.hpp; #843, docs/flags/save_menu.md): the
 * screen, which picks SaveMenu::Input's handler and SaveMenu::Draw's
 * routine. SaveMenu::Run starts in the one it is given: UpdateGameFrame
 * passes SAVE_MENU_STATE_LOAD (the title screen's load) or
 * SAVE_MENU_STATE_MAIN (on leaving the level select). */
#define SAVE_MENU_STATE_MAIN 0              // the options: load, load link, save, delete, exit
#define SAVE_MENU_STATE_LOAD 1              // the cartridge's slots, to load one (LoadInput, DrawLoad)
#define SAVE_MENU_STATE_LOAD_LINK 2         // the slots received over the link cable (DrawLoadLink)
#define SAVE_MENU_STATE_LINK_TRANSFER 3     // the link exchange running (LinkInput; DrawMessage)
#define SAVE_MENU_STATE_MESSAGE 4           // a two-line message, A/START back to MAIN (MessageInput)
#define SAVE_MENU_STATE_SAVE 5              // the slots, to save into one (SaveInput, DrawSave)
#define SAVE_MENU_STATE_DELETE 6            // the slots, to delete one (DeleteInput, DrawDelete)
#define SAVE_MENU_STATE_CONFIRM_DELETE 7    // "delete?" (ConfirmDeleteInput, DrawConfirmDelete)
#define SAVE_MENU_STATE_UNK8 8              // nothing enters it; Input and Draw do nothing in it
#define SAVE_MENU_STATE_CONFIRM_OVERWRITE 9 // "overwrite?" (OverwriteInput, DrawOverwrite)
#define SAVE_MENU_STATE_UNK10 10            // nothing enters it either; the last state the switches take

#endif /* GUARD_SAVE_H */
