#include "core.h"
#include "gba/io_reg.h"
#include "save_menu.h"

extern void *gAudioContext;
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);

/* Confirm/cancel handler for the composite pause/options screen: on
 * either flags bit 0 or bit 3, plays the standard "confirm" cue and
 * resets `state`/`field_10` back to their initial values. */
void SaveMenuMessageInput(struct save_menu *self, u32 flags)
{
    if (flags & 1) {
        goto confirm;
    } else if (flags & 8) {
    confirm:
        PlaySfx(gAudioContext, 0x49, 0x100);
        self->state = 0;
        self->field_10 = 1;
    }
}

extern struct palette_cache *gPaletteCache;
extern struct oam_shadow_buffer *gOamBuffer;
extern void UploadPaletteCache(struct palette_cache *arg0);
extern void CommitOamBuffer(struct oam_shadow_buffer *arg0);
extern void FlushVramDmaQueue(void);

/* Restores the saved BG0HOFS/DISPCNT pair (see field_0/field_1c's doc
 * comments in save_menu.h) and flushes the VRAM/OAM commit
 * queues - the counterpart "leaving the screen" step to whatever saved
 * those two fields (still raw, outside this chunk). */
void CommitSaveMenuFrame(struct save_menu *self)
{
    REG_DISPCNT = self->field_1c;
    REG_BG0HOFS = self->field_0 >> 3;
    UploadPaletteCache(gPaletteCache);
    CommitOamBuffer(gOamBuffer);
    FlushVramDmaQueue();
}

extern void *gSaveMenu;
extern void DestroySaveMenu(void *self, u32 flags);
extern void FreeUnlockedPaletteSlots(struct palette_cache *self);

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

extern void *OperatorNew(s32 size);
extern void *InitSaveMenu(void *arg0);

/* Allocates and constructs a fresh SIO-handshake spinner object (the
 * counterpart to CloseSaveMenu's teardown above), stashing it in the same
 * gSaveMenu global CloseSaveMenu tears down. */
void OpenSaveMenu(void)
{
    void **dest;

    FreeUnlockedPaletteSlots(gPaletteCache);
    dest = &gSaveMenu;
    *dest = InitSaveMenu(OperatorNew(0xe4));
}
