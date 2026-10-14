#include "vehicle.hpp"
#include "engine/audio/audio.hpp"
#include "level_state.hpp"

extern "C" {
#include "level.h"
#include "globals.h"
}

using Game::Entities::ActorSelf;
using Game::Entities::BoxOverlap;
using Game::Entities::WorldBox;
using Game::Vehicles::Polar::PolarNitroCrate;

/* A broken nitro's blast (#664 part 11d, include/vehicle.hpp), ROM
 * 0x0802C9A8-0x0802CA60, between polar_crate.cpp and polar_crates.cpp.
 * An old_agbcc object (current agbcc schedules the box moves' `asr`s
 * differently).
 *
 * The box test is actor_category_frame.cpp's: actor_self.hpp's
 * BoxOverlap of the two actors' WorldBoxes. */

static inline u8 ActorsOverlap(ActorSelf *a, ActorSelf *b)
{
    return BoxOverlap(WorldBox(a), WorldBox(b));
}

/* UpdatePolarNitroCrate (polar_crate.cpp) calls it 0x14 frames after
 * the nitro broke: every other nitro (record 4) whose box overlaps this
 * one's, and not broken yet, explodes too. */
void PolarNitroCrate::DetonateNearby()
{
    ActorSelf *n = gActorList->next;

    do {
        if (n->record->index == POLAR_KIND_NITRO_CRATE && n != this && ActorsOverlap(this, n) &&
            n->animIndex != 0x12) {
            gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
            gLevelState->AddBrokenCrate();
            static_cast<PolarNitroCrate *>(n)->Explode();
        }
        n = n->next;
    } while (n != gActorList);
}
