#ifndef GUARD_SAVE_MENU_HPP
#define GUARD_SAVE_MENU_HPP

/* The save menu as C++ (#664, docs/cplusplus.md, the final cleanup):
 * class SaveMenu, the object OpenSaveMenu allocates (`new SaveMenu`, 0xE4
 * bytes) into gSaveMenu and CloseSaveMenu deletes. Its code is
 * src/save/save_menu.cpp, save_menu_draw.cpp, save_menu_input.cpp and
 * save_menu_ui.cpp; cxx_symbols.txt maps the methods onto their C names
 * (SaveMenuInput, DrawSaveMenu, ...). The link transfer it drives
 * (struct settings_sync_pump, settings_sync.h) and the save data (struct
 * save_data) stay C structs: their own files (save_data.c,
 * save_transfer*.c) have no C++ trait.
 *
 * SaveMenu has no vtable: its destructor is a plain one, called with
 * `__in_chrg` 3 by `delete gSaveMenu`. `#pragma interface`, as in
 * menus.hpp, keeps g++ from emitting out-of-line copies of the inline
 * helpers. */

#pragma interface

#include "sprite_obj.hpp"
#include "font.hpp"

extern "C" {
#include "core.h"
#include "byte_arg.h"
#include "save.h"
}

/* The save menu: its main options are "load game", "load link game",
 * "save game", "delete game" and "exit" (gSaveMenuOptions). `state`
 * selects the input handler (Input) and the draw routine (Draw): 0 main
 * options, 1 load, 2 load from the link save, 3 link transfer, 4
 * message, 5 save, 6 delete, 7 "delete?", 9 "overwrite?". `cursor` is
 * the option or slot cursor (slots 0-3, 4 = cancel). `cartSave` is the
 * cartridge save, `linkSave` the save received over the link cable.
 * `currentStats`/`rowStats` are the summaries the slot list shows
 * (SummarizeProgress); the slot list reads them as one 5-entry array. */
class SaveMenu
{
public:
    u32 frame; // 0x00 - frame counter (Input); CommitFrame scrolls BG0 by frame >> 3
    s32 flags; // 0x04 - a wrapping 0-0xff frame counter; bit 2 is the highlight blink
    u8 done;   // 0x08 - set to leave RunSaveMenu's loop
    u8 unused_09[3];
    u32 state;        // 0x0C - see above
    s32 cursor;       // 0x10
    u32 messageLine1; // 0x14 - state 4's first text line
    u32 messageLine2; // 0x18 - and its second
    u16 dispcnt;      // 0x1C - the REG_DISPCNT shadow
    u8 unused_1e[2];  //
    u8 gameLoaded;    // 0x20 - a game was loaded; RunSaveMenu returns it
    u8 unused_21[3];  //
    u32 pendingSlot;  // 0x24 - the slot the "delete?"/"overwrite?" prompt acts on
    struct settings_row_stats currentStats; // 0x28
    struct settings_row_stats rowStats[4];  // 0x3C
    struct save_data *cartSave;             // 0x8C - the cartridge's save data
    struct save_data *linkSave;             // 0x90 - the save received over the link cable
    u8 unused_94[0xa8 - 0x94];              //
    UiSprite *rowObjA[5];                   // 0xA8 - the slot list's gem icons
    UiSprite *rowObjB[5];                   // 0xBC - its relic icons
    UiSprite *rowObjC[5];                   // 0xD0 - its crystal icons

    SaveMenu();  // InitSaveMenu
    ~SaveMenu(); // DestroySaveMenu

    /* src/save/save_menu.cpp */
    void MessageInput(u32 keys); // SaveMenuMessageInput
    void CommitFrame();          // CommitSaveMenuFrame

    /* src/save/save_menu_draw.cpp */
    s32 LinkExchange();                            // LinkExchangeSaveData
    void DrawMessageLines(s32 label1, s32 label2); // DrawSaveMenuMessageLines
    void DrawCancel(u8 highlight);                 // DrawSaveMenuCancel
    void DrawYesNoPrompt(s32 value);               // DrawYesNoPrompt
    void DrawSlotStats(s32 label1, s32 label2, s32 rowIdx,
                       struct byte_arg flag);                    // DrawSaveSlotStats
    void DrawSlots(struct save_data *handle, s32 selectedIndex); // DrawSaveSlots
    void InitIcons();                                            // InitSaveMenuIcons

    /* src/save/save_menu_input.cpp */
    void Input(u32 keys);                               // SaveMenuInput
    void MainInput(u32 keys);                           // SaveMenuMainInput
    void MoveCursor(u32 keys);                          // SaveMenuMoveCursor
    void LoadInput(u32 keys, struct save_data *handle); // SaveMenuLoadInput
    void LinkInput();                                   // SaveMenuLinkInput
    void SaveToSlot(s32 rowIndex);                      // SaveGameToSlot
    void OverwriteInput(u32 keys);                      // SaveMenuOverwriteInput
    void SaveInput(u32 keys);                           // SaveMenuSaveInput
    void DeleteInput(u32 keys);                         // SaveMenuDeleteInput
    void ConfirmDeleteInput(u32 keys);                  // SaveMenuConfirmDeleteInput
    void DrawMain();                                    // DrawSaveMenuMain

    /* src/save/save_menu_ui.cpp */
    void LoadBg();                                       // LoadSaveMenuBg
    void RefreshSlotSummaries(struct save_data *handle); // RefreshSaveSlotSummaries
    void LoadData();                                     // LoadSaveMenuData
    void SummarizeProgress(struct settings_row_stats *dest,
                           const struct game_progress *src); // SummarizeProgress
    void DrawEmptySlotLabel(s32 x, s32 y, u8 highlight);     // DrawEmptySlotLabel
    void DrawTitle(s32 labelIndex);                          // DrawSaveMenuTitle
    s32 GetBlinkPalette();                                   // GetSaveMenuBlinkPalette
    void EndLinkTransfer();                                  // EndLinkSaveTransfer
    void BeginLinkTransfer();                                // BeginLinkSaveTransfer
    void DrawConfirmDelete();                                // DrawSaveMenuConfirmDelete
    void DrawDelete();                                       // DrawSaveMenuDelete
    void DrawOverwrite();                                    // DrawSaveMenuOverwrite
    void DrawSave();                                         // DrawSaveMenuSave
    void DrawMessage();                                      // DrawSaveMenuMessage
    void DrawLoadLink();                                     // DrawSaveMenuLoadLink
    void DrawLoad();                                         // DrawSaveMenuLoad
    void Draw();                                             // DrawSaveMenu
    void DeleteSlot(s32 row);                                // DeleteSaveSlot
};

COMPILE_TIME_ASSERT(save_menu_hpp, sizeof(SaveMenu) == 0xE4);

#endif /* !GUARD_SAVE_MENU_HPP */
