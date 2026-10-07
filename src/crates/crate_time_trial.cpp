#include "crate.hpp"

extern "C" {
#include "audio.h"
#include "globals.h"
}

/* The time trial's crate conversion and the Aku Aku crate (#664,
 * include/crate.hpp). */

/* In a time trial, each crate of the crate list (class id 3) with a
 * time-trial kind becomes that kind, through SolidifyOutlineCrate. */
void ConvertCratesForTimeTrial(void)
{
    s32 i = 0;

    if (i < gCrateList->activeCount) {
        do {
            Entity *e = (Entity *)gCrateList->slotArray[i];

            if (e->GetClassId() == 3) {
                Crate *crate = (Crate *)e;
                s32 trialKind = crate->trialKind;

                if (trialKind != -1) {
                    crate->solidKind = (u8)trialKind;
                    crate->SolidifyOutline();
                }
            }
            i++;
        } while (i < gCrateList->activeCount);
    }
}

/* The Aku Aku crate: unless the player is passing through (the
 * `collides` flag clear), it gains a mask (EVENT_MASK_GAIN) with its
 * sound. */
void Crate::OpenAkuAku()
{
    GroundSprite *p = PlayerSprite();

    if (p->f.flags >> 7) {
        p->HandleEvent(0, EVENT_MASK_GAIN, 0);
        PlaySfx(gAudioContext, SFX_AKU_AKU_GAIN, 0x100);
    }
}
