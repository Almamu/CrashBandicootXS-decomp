#include "crate_list.hpp"
#include "crate.hpp"
#include "player.hpp"
#include "audio.hpp"

extern "C" {
#include "globals.h"
}

/* The time trial's crate conversion (#664, include/crate.hpp). The Aku
 * Aku crate, which the ROM puts right after it, starts crate_stack.cpp. */

/* In a time trial, each crate of the crate list (class id 3) with a
 * time-trial kind becomes that kind, through SolidifyOutlineCrate. */
void ConvertCratesForTimeTrial(void)
{
    s32 i = 0;

    if (i < gCrateList->count) {
        do {
            Entity *e = gCrateList->slots[i];

            if (e->GetClassId() == 3) {
                Crate *crate = (Crate *)e;
                s32 trialKind = crate->trialKind;

                if (trialKind != -1) {
                    crate->solidKind = (u8)trialKind;
                    crate->SolidifyOutline();
                }
            }
            i++;
        } while (i < gCrateList->count);
    }
}
