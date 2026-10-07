#include "platform.hpp"
#include "pickups.hpp"

extern "C" {
#include "crates.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
}

/* GitHub issue #34, UpdateGameFrame-MainLoop cluster (docs/rom_map.md);
 * C++ since #664 part 9 (include/spawners.hpp). Built with old_agbcp -
 * see docs/matching/archive/game-loop-old-agbcc.md. */

/* `t` is an s32: the constant is loaded before the tag's address. */
static inline void SetTag(Platform *part, s32 t)
{
    part->tag = t;
}

/* Starts a time trial: no mask, the clock and the countdown cleared.
 * Outside the category rooms, the bonus platform and the gem platform
 * switch to their time-trial animations (7 and 0xC; the gem platform's
 * palette reloaded), the crates become their time-trial kinds
 * (ConvertCratesForTimeTrial), and every pickup in the touchable list
 * (class 2) is collected if it is on screen, or else gone. */
void StartTimeTrial(struct level_state *self)
{
    Platform *part;
    s32 i;

    SetMaskLevel(self, MASK_LEVEL_NONE);
    self->timeTrial = 1;
    self->minutes = 0;
    self->seconds = 0;
    self->tenths = 0;
    self->frames = 0;
    self->countdown = 0;
    if (self->cat->kind == ROOM_KIND_CATEGORY)
        return;

    part = (Platform *)self->bonusPlatform;
    if (part != 0) {
        SetTag(part, 7);
        part->ResetFrameTimer();
        part->ResetFrameIndex();
        part->SetAnimDone(0);
    }
    part = (Platform *)self->gemPlatform;
    if (part != 0) {
        SetTag(part, 0xc);
        part->ResetFrameTimer();
        part->ResetFrameIndex();
        part->SetAnimDone(0);
        // clang-format off
        LoadPaletteSlot(gPaletteCache, ((Platform *)self->gemPlatform)->palette,
                        ((Platform *)self->gemPlatform)->bank->anims[
                            ((Platform *)self->gemPlatform)->tag].paletteId);
        // clang-format on
    }
    ConvertCratesForTimeTrial();

    i = 0;
    if (i < TouchableList()->count) {
        do {
            Sprite *e = TouchableList()->items[i];
            /* The pickup as a wumpa (class 2): a pointer of its own, a
             * register copy of `e` in the ROM. */
            Wumpa *w = (Wumpa *)e;

            if (e->GetClassId() == 2) {
                if (e->IsOnScreen())
                    w->PickUp(1);
                else
                    w->MarkGone();
            }
        } while (++i < TouchableList()->count);
    }
}
