#ifndef __SAVE_MENU_H__
#define __SAVE_MENU_H__

/* A save slot's summary: five totals counted from the slot's 0x70 bytes
 * (SaveData::ReadSlot). SaveMenu keeps the current game's (`currentStats`) and
 * the four slots' (`rowStats`, RefreshSlotSummaries in
 * src/save/save_menu_ui.cpp), and the slot list reads them as one
 * 5-entry array. */
struct settings_row_stats {
    s32 percent;
    s32 gems;
    s32 relics;
    s32 lives;
    s32 crystals;
};
COMPILE_TIME_ASSERT(save_menu_h, sizeof(struct settings_row_stats) == 0x14);

/* The save menu (gSaveMenu) is the C++ class SaveMenu
 * (include/save_menu.hpp); C sees only the tag. */
struct save_menu;

#endif /* __SAVE_MENU_H__ */
