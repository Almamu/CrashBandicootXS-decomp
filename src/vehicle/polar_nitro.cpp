#include "vehicle.hpp"

extern "C" {
#include "audio.h"
#include "level.h"
#include "globals.h"
}

/* A broken nitro's blast (#664 part 11d, include/vehicle.hpp), ROM
 * 0x0802C9A8-0x0802CA60, between polar_pickups.cpp and polar_crates.cpp.
 * An old_agbcc object (current agbcc schedules the box moves' `asr`s
 * differently).
 *
 * The box test is actor_category_frame.cpp's: each actor's box moved to
 * its position in whole units, the two compared on Z, Y and X. BoxOverlap
 * takes the boxes by reference, so g++ binds each returned box to a
 * temporary, and the ROM's two `MemCopy32(box, box, 12)` self-copies are
 * that binding's (docs/cplusplus.md, part 11b). */

static inline u8 BoxOverlap(const struct anim_box &b, const struct anim_box &a)
{
    if (b.z < a.z + a.d && b.z + b.d > a.z && b.y < a.y + a.h && b.y + b.h > a.y &&
        b.x < a.x + a.w && b.x + b.w > a.x)
        goto hit;
    return 0;
hit:
    return 1;
}

static inline struct anim_box WorldBox(ActorSelf *s)
{
    struct anim_box b = s->box;
    s32 dx = s->x >> 8;
    s32 dy = s->y >> 8;
    s32 dz = s->z >> 8;

    b.x += dx;
    b.y += dy;
    b.z += dz;
    return b;
}

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
            PlaySfx(gAudioContext, SFX_EXPLOSION, 0x100);
            AddBrokenCrate(gLevelState);
            static_cast<PolarNitroCrate *>(n)->Explode();
        }
        n = n->next;
    } while (n != gActorList);
}
