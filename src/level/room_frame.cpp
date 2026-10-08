#include "player.hpp"
#include "hud.hpp"

extern "C" {
#include "core.h"
#include "actor.h"
#include "menus.h"
#include "gfx.h"
#include "actor_self.h"
#include "sprite_bank.h"
#include "level_state.h"
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
 * queue - only while `self->level` is `<= 0x1000` (always, for a level
 * index), otherwise this is a no-op. */
void UpdateRoomFrame(struct level_progress *self)
{
    UploadPaletteCache(gPaletteCache);
    UpdateCamera(gCamera);
    ScrollLevelLayers(gLevelLayers);
    TickPaletteCycles(gPaletteCycles);

    if (self->level <= 0x1000) {
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

/* Rebuilds the gBlendRegs BLDCNT/BLDALPHA shadow from the current room's
 * blend settings (`level_room.param.blend`) and
 * sets gLevelLayers->raiseObjPriority in an underwater room (kind 1). With
 * no blend effect, the shadow gets a fixed 16/16 alpha pattern. The effect
 * is tested as a halfword and stored from its low byte, as in the ROM. */
void SetupRoomBlend(struct level_progress *self)
{
    union blend *b = &gBlendRegs.blend;

    b->raw = 0;
    gLevelLayers->raiseObjPriority = 0;
    if (self->cat->param.blend.effect != 0) {
        if (self->cat->kind == ROOM_KIND_UNDERWATER)
            gLevelLayers->raiseObjPriority = 1;
        b->bits.effect = *(const u8 *)&self->cat->param.blend.effect;
        b->bits.eva = self->cat->param.blend.eva;
        b->bits.evb = self->cat->param.blend.evb;
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
