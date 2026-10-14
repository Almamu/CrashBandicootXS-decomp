#include "actor_self.hpp"
#include "vehicle.hpp"
#include "boss_actors.hpp"
#include "engine/gfx/category_sprite_sheet.hpp"

extern "C" {
#include "engine/util/math_util.h"
#include "engine/actor/actor.h"
#include "engine/gfx/gfx.h"
#include "globals.h"
}

using Engine::Gfx::CategorySpriteSheet;
using Game::Entities::AnimPart;

/* AnimPart's methods (#664 part 11a, include/actor_self.hpp): the frame
 * accessors and SetAnim (SetActorAnim). ROM 0x0803B058-0x0803B0C4, the
 * first of the ROM-tail functions the polar, jetpack and boss actors'
 * destructors follow (inline_copies_actors.cpp; this was the head of
 * actor_anim.cpp until #770). Built with old_agbcp and
 * -fno-implement-inlines, as it was there. */

s32 AnimPart::GetAnimFrameBaseOffset()
{
    return Q8_TO_INT(animTime);
}

/* The current keyframe's `attr`, in the high halfword: an OAM attribute
 * word's flags (DrawActor, DrawJetpackCheckpointText). */
s32 AnimPart::GetAnimFrameAttr()
{
    return anims[animIndex].attr << 16;
}

/* The current frame's graphics: the keyframe's frameIndex plus the
 * animation's frame, indexing frameOffsets (byte offsets into
 * the category sprite sheet). */
u8 *AnimPart::GetAnimFrameData()
{
    s32 base = GetAnimFrameBaseOffset();

    return (u8 *)CategorySpriteSheet::data + frameOffsets[anims[animIndex].frameIndex + base];
}

/* Starts keyframe `idx`: its duration, not done, from the start. */
void AnimPart::SetAnim(s32 idx)
{
    animIndex = idx;
    animTimer = anims[idx].duration;
    animDone = 0;
    animTime = 0;
}
