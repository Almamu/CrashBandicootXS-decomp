#include "vehicle.hpp"
#include "audio.hpp"

extern "C" {
#include "level.h"
#include "globals.h"
}

/* A broken nitro's blast (#664 part 11d, include/vehicle.hpp), ROM
 * 0x0802C9A8-0x0802CA60, between polar_pickups.cpp and polar_crates.cpp.
 * An old_agbcc object (current agbcc schedules the box moves' `asr`s
 * differently).
 *
 * The box test is actor_category_frame.cpp's: actor_self.hpp's
 * BoxOverlap of the two actors' WorldBoxes. */

static inline u8 ActorsOverlap(ActorSelf *a, ActorSelf *b)
{
    return BoxOverlap(WorldBox(a), WorldBox(b));
}

/* UpdatePolarNitroCrate (polar_pickups.cpp) calls it 0x14 frames after
 * the nitro broke: every other nitro (record 4) whose box overlaps this
 * one's, and not broken yet, explodes too. */
void PolarNitroCrate::DetonateNearby()
{
    ActorSelf *n = gActorList->next;

    do {
        if ((u8)n->record->index == 4 && n != this && ActorsOverlap(this, n) &&
            n->animIndex != 0x12) {
            gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
            AddBrokenCrate(gLevelState);
            static_cast<PolarNitroCrate *>(n)->Explode();
        }
        n = n->next;
    } while (n != gActorList);
}
