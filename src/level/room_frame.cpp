#include "player.hpp"
#include "hud.hpp"

extern "C" {
#include "core.h"
#include "actor.h"
#include "level_menu.h"
#include "hud.h"
#include "system.h"
#include "crates.h"
#include "gfx.h"
#include "objects.h"
#include "level.h"
#include "globals.h"
}

/* Built with old_agbcp (old_agbcc as C) - see
 * docs/matching/archive/game-loop-old-agbcc.md. C++ since the #664
 * cleanup: the player's IsOnScreen and Draw are virtual calls. */

struct palette_cache;
struct part_list;
struct oam_shadow_buffer;

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
        gHud->Update();
        DrawPartList(gForegroundList);

        if (gPlayer->IsOnScreen())
            gPlayer->Draw();

        DrawPartList(gCollidableList);
        DrawPartList(gTouchableList);
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
 * blend settings and sets gLevelLayers->raiseObjPriority in mode 1. With
 * no blend effect, the shadow gets a fixed 16/16 alpha pattern. */
void SetupRoomBlend(struct level_ctx *self)
{
    union blend *b = &gBlendRegs.blend;

    b->raw = 0;
    gLevelLayers->raiseObjPriority = 0;
    if (self->blend->effect != 0) {
        if (self->blend->mode == 1)
            gLevelLayers->raiseObjPriority = 1;
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
