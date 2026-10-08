#include "spawners.hpp"
#include "level_state.hpp"

extern "C" {
#include "globals.h"
}

/* EntitySpawner::DropExtraLife (#664, include/spawners.hpp). Built with
 * old_agbcp - see docs/matching/archive/game-loop-old-agbcc.md. */

static inline void SetTag(ExtraLife *part, s32 t)
{
    part->tag = t;
}

/* Unless in a time trial (then NULL), an extra life at (x, y), always
 * active, its counter, mode and phase p3, p5 and 0, on animation 0xA of
 * sprite bank 0x8D * 4; `toHud` flies it to the HUD. */
ExtraLife *EntitySpawner::DropExtraLife(u32 x, u32 y, u32 p3, u32 p5, bool toHud)
{
    ExtraLife *part = 0;
    u8 state = gLevelState->timeTrial;

    if (state == 0) {
        part = ExtraLife::Create(0xffff, x, y, 0);
        part->f.flags |= 0x10;
        part->counter = p3;
        part->mode = p5;
        part->phase = state;
        part->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + 0x8d * 4);
        SetTag(part, 0xa);
        part->ResetFrameTimer();
        part->ResetFrameIndex();
        part->SetAnimDone(0);
        part->palette = part->GetAnimPaletteSlot();
        if (toHud)
            part->SendToHud();
    }
    return part;
}
