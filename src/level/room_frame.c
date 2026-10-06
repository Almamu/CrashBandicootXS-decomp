#include "core.h"
#include "actor.h"
#include "vtable.h"
#include "level_menu.h"
#include "hud.h"
#include "system.h"
#include "crates.h"
#include "gfx.h"
#include "objects.h"
#include "level.h"
#include "globals.h"
#include "player.h"

/* Built with old_agbcc - see docs/matching/archive/game-loop-old-agbcc.md. */

struct palette_cache;
struct part_list;
struct oam_shadow_buffer;

extern void *_call_via_r1(void *arg0, void *arg1);

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
            struct player *p = gPlayer;
            const struct player_vtable *tbl = p->vtable;
            // clang-format off
            if ((u8)(s32)_call_via_r1((u8 *)p + tbl->isOnScreen.thisOffset,
                                      tbl->isOnScreen.fn) != 0) {
                // clang-format on
                struct player *p2 = gPlayer;
                const struct player_vtable *tbl2 = p2->vtable;
                _call_via_r1((u8 *)p2 + tbl2->draw.thisOffset, tbl2->draw.fn);
            }
        }

        DrawPartList(gCollidableList);
        DrawPartList(gUnknown_030012EC);
        DrawCrateList(gCrateList);
        DrawPartList((struct part_list *)gDecorationList);

        HideUnusedOamEntries(gOamBuffer);
        WaitForVBlank();
        CommitOamBuffer(gOamBuffer);
        CommitLevelScroll(gLevelLayers);
        FlushVramDmaQueue();
    }
}

/* The level's raster/blend settings. */
struct level_blend {
    u8 unk_00[8];
    s32 mode; // 0x08
    u8 unk_0C[4];
    u16 effect; // 0x10 - 0: no blending
    u8 eva;     // 0x12
    u8 evb;     // 0x13
};

struct level_ctx {
    u8 unk_00[0x18];
    struct level_blend *blend; // 0x18
};

/* Rebuilds the gBlendRegs BLDCNT/BLDALPHA shadow from the level's
 * blend settings and sets gLevelLayers's `unk_2B` flag in mode 1. With
 * no blend effect, the shadow gets a fixed 16/16 alpha pattern. */
void SetupRoomBlend(struct level_ctx *self)
{
    union blend *b = &gBlendRegs.blend;

    b->raw = 0;
    gLevelLayers->unk_2B = 0;
    if (self->blend->effect != 0) {
        if (self->blend->mode == 1)
            gLevelLayers->unk_2B = 1;
        b->bits.effect = *(u8 *)&self->blend->effect;
        b->bits.eva = self->blend->eva;
        b->bits.evb = self->blend->evb;
        b->bits.bg3First = 1;
        b->bits.bg0Second = 1;
        b->bits.bg1Second = 1;
        b->bits.bg2Second = 1;
        b->bits.objSecond = 1;
    } else {
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
