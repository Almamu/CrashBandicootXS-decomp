#include "bg_layer.hpp"
#include "crate_list.hpp"
#include "player.hpp"
#include "hud.hpp"
#include "level_state.hpp"

extern "C" {
#include "core.h"
#include "actor.h"
#include "menus.h"
#include "gfx.h"
#include "actor_self.h"
#include "sprite_bank.h"
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
struct oam_shadow_buffer;

/* Runs the DMA3/`UploadPaletteCache`+`ResetLevelLayers` refresh pass over every
 * currently-active dual-array manager, then flushes the VRAM DMA
 * queue - only while `level` is `<= 0x1000` (always, for a level
 * index), otherwise this is a no-op. */
void LevelProgress::UpdateRoomFrame()
{
    gPaletteCache->Upload();
    UpdateCamera(gCamera);
    gLevelLayers->Scroll();
    gPaletteCycles->Tick();

    if (level <= 0x1000) {
        gHud->Update();
        gForegroundList->Draw();

        if (gPlayer->IsOnScreen())
            gPlayer->Draw();

        gCollidableList->Draw();
        gTouchableList->Draw();
        gCrateList->Draw();
        gDecorationList->Draw();

        gOamBuffer->HideUnused();
        WaitForVBlank();
        gOamBuffer->Commit();
        gLevelLayers->CommitScroll();
        FlushVramDmaQueue();
    }
}

/* Rebuilds the gBlendRegs BLDCNT/BLDALPHA shadow from the current room's
 * blend settings (`level_room.param.blend`) and
 * sets gLevelLayers->raiseObjPriority in an underwater room (kind 1). With
 * no blend effect, the shadow gets a fixed 16/16 alpha pattern. The effect
 * is tested as a halfword and stored from its low byte, as in the ROM. */
void LevelProgress::SetupRoomBlend()
{
    union blend *b = &gBlendRegs.blend;

    b->raw = 0;
    gLevelLayers->raiseObjPriority = 0;
    if (cat->param.blend.effect != 0) {
        if (cat->kind == ROOM_KIND_UNDERWATER)
            gLevelLayers->raiseObjPriority = 1;
        b->bits.effect = *(const u8 *)&cat->param.blend.effect;
        b->bits.eva = cat->param.blend.eva;
        b->bits.evb = cat->param.blend.evb;
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
