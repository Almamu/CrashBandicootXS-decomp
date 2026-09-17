#include "core.h"
#include "gba/io_reg.h"
#include "pause_options_screen.h"

extern void *gUnknown_030012BC;
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);

/* Confirm/cancel handler for the composite pause/options screen: on
 * either flags bit 0 or bit 3, plays the standard "confirm" cue and
 * resets `state`/`field_10` back to their initial values. */
void sub_8004CB4(struct pause_options_screen *self, u32 flags)
{
    if (flags & 1) {
        goto confirm;
    } else if (flags & 8) {
    confirm:
        PlaySfx(gUnknown_030012BC, 0x49, 0x100);
        self->state = 0;
        self->field_10 = 1;
    }
}

extern struct tile_asset_cache *gUnknown_030012B8;
extern struct oam_shadow_buffer *gUnknown_03001300;
extern void sub_8006DC8(struct tile_asset_cache *arg0);
extern void sub_8006AAC(struct oam_shadow_buffer *arg0);
extern void FlushVramDmaQueue(void);

/* Restores the saved BG0HOFS/DISPCNT pair (see field_0/field_1c's doc
 * comments in pause_options_screen.h) and flushes the VRAM/OAM commit
 * queues - the counterpart "leaving the screen" step to whatever saved
 * those two fields (still raw, outside this chunk). */
void sub_8004CE8(struct pause_options_screen *self)
{
    REG_DISPCNT = self->field_1c;
    REG_BG0HOFS = self->field_0 >> 3;
    sub_8006DC8(gUnknown_030012B8);
    sub_8006AAC(gUnknown_03001300);
    FlushVramDmaQueue();
}

extern void *gUnknown_0300080C;
extern void sub_800312C(void *self, u32 flags);
extern void sub_8006EA8(struct tile_asset_cache *self);

/* Tears down the "connecting..." SIO-handshake spinner object (see
 * sub_8003B40, src/graphics/settings_menu3.c, for the object this
 * pointer comes from) if one is active, then re-requests the tile
 * cache flush sub_8004CE8 above pairs with. */
void sub_8004D20(void)
{
    if (gUnknown_0300080C != NULL) {
        sub_800312C(gUnknown_0300080C, 3);
    }
    gUnknown_0300080C = NULL;
    sub_8006EA8(gUnknown_030012B8);
}

extern void *sub_8026EDC(s32 size);
extern void *sub_800306C(void *arg0);

/* Allocates and constructs a fresh SIO-handshake spinner object (the
 * counterpart to sub_8004D20's teardown above), stashing it in the same
 * gUnknown_0300080C global sub_8004D20 tears down. */
void sub_8004D4C(void)
{
    void **dest;

    sub_8006EA8(gUnknown_030012B8);
    dest = &gUnknown_0300080C;
    *dest = sub_800306C(sub_8026EDC(0xe4));
}
