#include "core.h"
#include "gba/io_reg.h"
#include "save.h"
#include "audio.h"
#include "gfx.h"
#include "memory.h"
#include "globals.h"

/* Confirm/cancel handler for the composite pause/options screen: on
 * either flags bit 0 or bit 3, plays the standard "confirm" cue and
 * resets `state`/`cursor` back to their initial values. */
void SaveMenuMessageInput(struct save_menu *self, u32 flags)
{
    if (flags & 1) {
        goto confirm;
    } else if (flags & 8) {
    confirm:
        PlaySfx(gAudioContext, SFX_MENU_SELECT, 0x100);
        self->state = 0;
        self->cursor = 1;
    }
}

/* Restores the saved BG0HOFS/DISPCNT pair (see frame/dispcnt's doc
 * comments in save_menu.h) and flushes the VRAM/OAM commit
 * queues - the counterpart "leaving the screen" step to whatever saved
 * those two fields (still raw, outside this chunk). */
void CommitSaveMenuFrame(struct save_menu *self)
{
    REG_DISPCNT = self->dispcnt;
    REG_BG0HOFS = self->frame >> 3;
    UploadPaletteCache(gPaletteCache);
    CommitOamBuffer(gOamBuffer);
    FlushVramDmaQueue();
}

/* Tears down the "connecting..." SIO-handshake spinner object (see
 * LinkExchangeSaveData, src/save/save_menu_ui.c, for the object this
 * pointer comes from) if one is active, then re-requests the tile
 * cache flush CommitSaveMenuFrame above pairs with. */
void CloseSaveMenu(void)
{
    if (gSaveMenu != NULL) {
        DestroySaveMenu(gSaveMenu, 3);
    }
    gSaveMenu = NULL;
    FreeUnlockedPaletteSlots(gPaletteCache);
}

/* Allocates and constructs a fresh SIO-handshake spinner object (the
 * counterpart to CloseSaveMenu's teardown above), stashing it in the same
 * gSaveMenu global CloseSaveMenu tears down. */
void OpenSaveMenu(void)
{
    struct save_menu **dest;

    FreeUnlockedPaletteSlots(gPaletteCache);
    dest = &gSaveMenu;
    *dest = InitSaveMenu(OperatorNew(0xe4));
}
