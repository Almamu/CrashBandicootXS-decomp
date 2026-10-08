#include "save_menu.hpp"

extern "C" {
#include "core.h"
#include "gba/io_reg.h"
#include "save.h"
#include "audio.h"
#include "gfx.h"
#include "memory.h"
#include "globals.h"
}

/* State 4's input handler (a message): A or START (`keys`: the newly
 * pressed keys) plays the "confirm" cue and goes back to the main
 * options, on "load link game". */
void SaveMenu::MessageInput(u32 keys)
{
    if (keys & A_BUTTON) {
        goto confirm;
    } else if (keys & START_BUTTON) {
    confirm:
        PlaySfx(gAudioContext, SFX_MENU_SELECT, 0x100);
        state = 0;
        cursor = 1;
    }
}

/* Writes the DISPCNT shadow and the BG0 scroll (frame >> 3), and flushes
 * the palette cache, the OAM buffer and the VRAM DMA queue: RunSaveMenu's
 * once-a-frame commit. */
void SaveMenu::CommitFrame()
{
    REG_DISPCNT = dispcnt;
    REG_BG0HOFS = frame >> 3;
    UploadPaletteCache(gPaletteCache);
    CommitOamBuffer(gOamBuffer);
    FlushVramDmaQueue();
}

/* Deletes the save menu (C linkage, for game_frame.cpp) and frees the
 * palette slots it held. */
void CloseSaveMenu(void)
{
    delete gSaveMenu;
    gSaveMenu = 0;
    FreeUnlockedPaletteSlots(gPaletteCache);
}

/* Builds the save menu into gSaveMenu (C linkage, for game_frame.cpp);
 * RunSaveMenu runs it. */
void OpenSaveMenu(void)
{
    SaveMenu **dest;

    FreeUnlockedPaletteSlots(gPaletteCache);
    dest = &gSaveMenu;
    *dest = new SaveMenu;
}
