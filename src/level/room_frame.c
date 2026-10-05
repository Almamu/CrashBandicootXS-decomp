#include "core.h"
#include "actor.h"
#include "vtable.h"
#include "level_menu.h"

/* Built with old_agbcc - see docs/matching/game-loop-old-agbcc.md. */

struct palette_cache;
struct dual_array_manager;
struct oam_shadow_buffer;

extern void *gPlayer;
extern void *gCamera;
extern void *gPaletteCycles;
extern void *gOamBuffer;
extern u8 *gLevelLayers;
extern void *gHud;
extern void *gCollidableList;
extern void *gUnknown_030012F4;
extern void *gDecorationList;
extern void *gUnknown_030012EC;
extern void *gCrateList;
extern union blend gBlendRegs;
extern struct palette_cache *gPaletteCache;

extern void UploadPaletteCache(struct palette_cache *self);
extern void UpdateCamera(void *self);
extern void ScrollLevelLayers(void *self);
extern void TickPaletteCycles(void *self);
extern void UpdateHud(void *self);
extern void DrawPartList(struct dual_array_manager *manager);
extern void *_call_via_r1(void *arg0, void *arg1);
extern void DrawCrateList(void *managerArg);
extern void HideUnusedOamEntries(struct oam_shadow_buffer *arg0);
extern void WaitForVBlank(void);
extern void CommitOamBuffer(struct oam_shadow_buffer *arg0);
extern void CommitLevelScroll(void *self);
extern void FlushVramDmaQueue(void);

/* Runs the DMA3/`UploadPaletteCache`+`ResetLevelLayers` refresh pass over every
 * currently-active dual-array manager, then flushes the VRAM DMA
 * queue - only while `self->0x0` is still within the "near start of
 * level" range (`<= 0x1000`), otherwise this is a no-op. */
void UpdateRoomFrame(void *self)
{
    UploadPaletteCache(gPaletteCache);
    UpdateCamera(gCamera);
    ScrollLevelLayers(gLevelLayers);
    TickPaletteCycles(gPaletteCycles);

    if (*(s32 *)self <= 0x1000) {
        UpdateHud(gHud);
        DrawPartList(gUnknown_030012F4);

        {
            struct actor *p = (struct actor *)gPlayer;
            struct vtable_slot *tbl = p->table;
            if ((u8)(s32)_call_via_r1((u8 *)p + tbl[5].delta, tbl[5].fn) != 0) {
                struct actor *p2 = (struct actor *)gPlayer;
                struct vtable_slot *tbl2 = p2->table;
                _call_via_r1((u8 *)p2 + tbl2[4].delta, tbl2[4].fn);
            }
        }

        DrawPartList(gCollidableList);
        DrawPartList(gUnknown_030012EC);
        DrawCrateList(gCrateList);
        DrawPartList(gDecorationList);

        HideUnusedOamEntries(gOamBuffer);
        WaitForVBlank();
        CommitOamBuffer(gOamBuffer);
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

/* Rebuilds the gBlendRegs BLDCNT/BLDALPHA shadow from the level's
 * blend settings and sets gLevelLayers's +0x2b flag in mode 1. With
 * no blend effect, the shadow gets a fixed 16/16 alpha pattern. */
void SetupRoomBlend(struct level_ctx *self)
{
    union blend *b = &gBlendRegs;

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
