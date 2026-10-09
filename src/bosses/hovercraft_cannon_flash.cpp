#include "boss_actors.hpp"
#include "hovercraft.hpp"

/* The cannon's muzzle flash (#664 part 11h, include/boss_actors.hpp): an
 * HpActor the cannon spawns with each shot (SpawnHovercraftCannonFlash).
 * It stays in front of the cannon until its animation has played once,
 * then deletes itself. See docs/matching/archive/issue-63-0x08033ef4-actor.md. */

/* gHovercraftCannonFlashVtable slot 4: none (the cannon's muzzle flash
 * can't be hurt). */
void HovercraftCannonFlash::Damage(s32)
{
}

/* Follows the cannon and deletes itself once the animation is done.
 * Returns whether the flash is still there. Update's body; the ROM also
 * has it out of line, as RunState. A `bool`: Update's test of it is kept
 * (with an `s32`, cse folds it on both paths). */
static inline bool StepCannonFlash(HovercraftCannonFlash *self)
{
    self->z = Hovercraft::GetZ() - 0x200;
    self->x = Hovercraft::GetX() + 0x2000;
    self->y = Hovercraft::GetY() + 0x3000;
    self->unshootable = 1;

    if (self->animDone != 0) {
        delete self;
        return false;
    }
    return true;
}

/* StepCannonFlash, then the common update while the flash is there. */
void HovercraftCannonFlash::Update()
{
    if (StepCannonFlash(this))
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
