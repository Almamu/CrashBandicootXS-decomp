#include "save_menu.hpp"
#include "vram_dma_queue.hpp"
#include "audio.hpp"

extern "C" {
#include "core.h"
#include "gba/io_reg.h"
#include "save.h"
#include "gfx.h"
#include "memory.h"
#include "globals.h"
}

using Engine::Sys::VramDmaQueue;

/* State 4's input handler (a message): A or START (`keys`: the newly
 * pressed keys) plays the "confirm" cue and goes back to the main
 * options, on "load link game". */
void SaveMenu::MessageInput(u32 keys)
{
    if (keys & A_BUTTON) {
        goto confirm;
    } else if (keys & START_BUTTON) {
    confirm:
        gAudioContext->PlaySfx(SFX_MENU_SELECT, 0x100);
        state = SAVE_MENU_STATE_MAIN;
        cursor = 1;
    }
}

/* Writes the DISPCNT shadow and the BG0 scroll (frame >> 3), and flushes
 * the palette cache, the OAM buffer and the VRAM DMA queue: Run's
 * once-a-frame commit. */
void SaveMenu::CommitFrame()
{
    REG_DISPCNT = dispcnt.raw;
    REG_BG0HOFS = frame >> 3;
    gPaletteCache->Upload();
    gOamBuffer->Commit();
    VramDmaQueue::Flush();
}

/* Deletes the save menu (for game_frame.cpp) and frees the palette
 * slots it held. */
void SaveMenu::Close()
{
    delete instance;
    instance = 0;
    gPaletteCache->FreeUnlockedSlots();
}

/* Builds the save menu into `instance` (for game_frame.cpp); Run runs
 * it. */
void SaveMenu::Open()
{
    SaveMenu **dest;

    gPaletteCache->FreeUnlockedSlots();
    dest = &instance;
    *dest = new SaveMenu;
}
