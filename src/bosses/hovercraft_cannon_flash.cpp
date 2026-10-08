#include "boss_actors.hpp"

extern "C" {
#include "match.h"
}

/* The cannon's muzzle flash (#664 part 11h, include/boss_actors.hpp): an
 * HpActor the cannon spawns with each shot (SpawnHovercraftCannonFlash).
 * It stays in front of the cannon until its animation has played once,
 * then deletes itself. See docs/matching/archive/issue-63-0x08033ef4-actor.md. */

/* Follows the cannon and deletes itself once the animation is done.
 * Returns whether the flash is still there. Update's body; the ROM also
 * has it out of line, as RunState. */
static inline s32 StepCannonFlash(HovercraftCannonFlash *self)
{
    self->z = GetHovercraftZ() - 0x200;
    self->x = GetHovercraftX() + 0x2000;
    self->y = GetHovercraftY() + 0x3000;
    self->unshootable = 1;

    if (self->animDone != 0) {
        delete self;
        return 0;
    }
    return 1;
}

/* StepCannonFlash, then the common update while the flash is there. */
void HovercraftCannonFlash::Update()
{
    s32 alive = StepCannonFlash(this);

    /* The ROM tests the 0 or 1 again: CSE folds the test of the inline's
     * result on both paths with every spelling tried (a local, a `bool`,
     * empty loops around the call or the test), as it did in the C. */
    MATCH_KEEP_VOLATILE(alive);
    if (alive != 0)
        ActorSelf::Update();
}

/* 1 hit point, state 0. */
HovercraftCannonFlash::HovercraftCannonFlash(const struct anim_table_record *rec, s32 x, s32 y,
                                             s32 z)
    : HpActor(rec, x, y, z, 1)
{
    SetState(0, 0);
    unshootable = 1;
}

/* Update without the common update.
 * UNUSED - no caller anywhere in the ROM (checked every src/ file and
 * every word-aligned Thumb pointer in baserom.gba). */
s32 HovercraftCannonFlash::RunState()
{
    return StepCannonFlash(this);
}

s32 HovercraftCannonFlash::IsUnshootable()
{
    return unshootable;
}
