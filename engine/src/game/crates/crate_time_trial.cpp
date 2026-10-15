#include "game/crates/crate_list.hpp"
#include "game/crates/crate.hpp"
#include "game/crash/player.hpp"
#include "engine/audio/audio.hpp"

extern "C" {
#include "globals.h"
}

using Game::Crates::Crate;
using Game::Entities::Entity;

/* The time trial's crate conversion (#664, include/game/crates/crate.hpp). The Aku
 * Aku crate, which the ROM puts right after it, starts crate_fields.cpp. */

/* In a time trial, each crate of the crate list (class id 3) with a
 * time-trial kind becomes that kind, through SolidifyOutlineCrate. */
void Crate::ConvertForTimeTrial()
{
    s32 i = 0;

    if (i < gCrateList->count) {
        do {
            Entity *e = gCrateList->slots[i];

            if (e->GetClassId() == 3) {
                Crate *crate = (Crate *)e;
                s32 trialKind = crate->trialKind;

                if (trialKind != CRATE_TRIAL_KIND_NONE) {
                    crate->solidKind = (u8)trialKind;
                    crate->SolidifyOutline();
                }
            }
            i++;
        } while (i < gCrateList->count);
    }
}
