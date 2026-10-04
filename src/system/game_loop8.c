#include "core.h"
#include "actor.h"
#include "vtable.h"
#include "level_menu.h"

/* Built with old_agbcc - see docs/matching/game-loop-old-agbcc.md. */

struct tile_asset_cache;
struct dual_array_manager;
struct oam_shadow_buffer;

extern void *gUnknown_030012D8;
extern void *gUnknown_030012D4;
extern void *gUnknown_030012C8;
extern void *gUnknown_03001300;
extern u8 *gLevelLayers;
extern void *gUnknown_03001318;
extern void *gUnknown_030012F0;
extern void *gUnknown_030012F4;
extern void *gUnknown_030012F8;
extern void *gUnknown_030012EC;
extern void *gUnknown_0300130C;
extern union blend gUnknown_03001280;
extern struct tile_asset_cache *gUnknown_030012B8;

extern void sub_8006DC8(struct tile_asset_cache *self);
extern void sub_8026E6C(void *self);
extern void ScrollLevelLayers(void *self);
extern void sub_8026F54(void *self);
extern void sub_80274EC(void *self);
extern void sub_8008DC0(struct dual_array_manager *manager);
extern void *sub_803AD7C(void *arg0, void *arg1);
extern void sub_800944C(void *managerArg);
extern void sub_8006A48(struct oam_shadow_buffer *arg0);
extern void sub_80006A8(void);
extern void sub_8006AAC(struct oam_shadow_buffer *arg0);
extern void CommitLevelScroll(void *self);
extern void FlushVramDmaQueue(void);

/* Runs the DMA3/`sub_8006DC8`+`ResetLevelLayers` refresh pass over every
 * currently-active dual-array manager, then flushes the VRAM DMA
 * queue - only while `self->0x0` is still within the "near start of
 * level" range (`<= 0x1000`), otherwise this is a no-op. */
void sub_802400C(void *self)
{
    sub_8006DC8(gUnknown_030012B8);
    sub_8026E6C(gUnknown_030012D4);
    ScrollLevelLayers(gLevelLayers);
    sub_8026F54(gUnknown_030012C8);

    if (*(s32 *)self <= 0x1000) {
        sub_80274EC(gUnknown_03001318);
        sub_8008DC0(gUnknown_030012F4);

        {
            struct actor *p = (struct actor *)gUnknown_030012D8;
            struct vtable_slot *tbl = p->table;
            if ((u8)(s32)sub_803AD7C((u8 *)p + tbl[5].delta, tbl[5].fn) != 0) {
                struct actor *p2 = (struct actor *)gUnknown_030012D8;
                struct vtable_slot *tbl2 = p2->table;
                sub_803AD7C((u8 *)p2 + tbl2[4].delta, tbl2[4].fn);
            }
        }

        sub_8008DC0(gUnknown_030012F0);
        sub_8008DC0(gUnknown_030012EC);
        sub_800944C(gUnknown_0300130C);
        sub_8008DC0(gUnknown_030012F8);

        sub_8006A48(gUnknown_03001300);
        sub_80006A8();
        sub_8006AAC(gUnknown_03001300);
        CommitLevelScroll(gLevelLayers);
        FlushVramDmaQueue();
    }
}

/* The level's raster/blend settings. */
struct level_blend
{
    u8 unk_00[8];
    s32 mode;           // 0x08
    u8 unk_0C[4];
    u16 effect;         // 0x10 - 0: no blending
    u8 eva;             // 0x12
    u8 evb;             // 0x13
};

struct level_ctx
{
    u8 unk_00[0x18];
    struct level_blend *blend; // 0x18
};

/* Rebuilds the gUnknown_03001280 BLDCNT/BLDALPHA shadow from the level's
 * blend settings and sets gLevelLayers's +0x2b flag in mode 1. With
 * no blend effect, the shadow gets a fixed 16/16 alpha pattern. */
void sub_80240E4(struct level_ctx *self)
{
    union blend *b = &gUnknown_03001280;

    b->raw = 0;
    gLevelLayers[0x2b] = 0;
    if (self->blend->effect != 0)
    {
        if (self->blend->mode == 1)
            gLevelLayers[0x2b] = 1;
        b->bits.effect = *(u8 *)&self->blend->effect;
        b->bits.eva = self->blend->eva;
        b->bits.evb = self->blend->evb;
        b->bits.bg3First = 1;
        b->bits.bg0Second = 1;
        b->bits.bg1Second = 1;
        b->bits.bg2Second = 1;
        b->bits.objSecond = 1;
    }
    else
    {
        b->bits.effect = 0;
        b->bits.eva = 0x10;
        b->bits.evb = 0x10;
        b->bits.bg3First = 1;
        b->bits.bg0Second = 1;
        b->bits.bg1Second = 1;
        b->bits.bg2Second = 1;
        b->bits.objSecond = 1;
    }
}
