#include "sprite_obj.hpp"
#include "player.hpp"
#include "level_state.hpp"

extern "C" {
#include "player.h"
#include "constants/events.h"
}

/* MovingSprite's contact with the player (#664, include/sprite_obj.hpp). */

/* IsPlayerInvulnerable, inlined: the player's invulnerability deadline is
 * still ahead of the frame counter. */
static inline u8 PlayerStillInvulnerable(Player *player)
{
    return player->deadline > gRoomFrameCount;
}

/* Slot 14: whether the player touches the sprite, and if so the contact
 * (ResolvePlayerContact). A sprite in contact with the player (the
 * `visible` bit) that isn't solid is tested unless the player is
 * invulnerable without the invincibility mask; a solid one only while the
 * mask makes the player invincible. The attack box is tested first, then
 * the body box. */
void MovingSprite::TouchPlayer()
{
    s32 contact = (f.flags >> 2) & 1;

    if (!contact ||
        (PlayerStillInvulnerable(gPlayer) && gLevelState->maskLevel != MASK_LEVEL_INVINCIBLE) ||
        ((f.bytes.flags2 >> 3) & 1)) {
        s32 solid = (f.bytes.flags2 >> 3) & 1;

        if (!solid)
            return;
        if (gLevelState->maskLevel != MASK_LEVEL_INVINCIBLE)
            return;
    }
    struct aabb box = GetAttackBox();

    if (box.w != 0 && gPlayer->TouchesBox(&box)) {
        ResolvePlayerContact();
        return;
    }
    struct aabb box2 = GetBodyBox();

    /* Kept from the C: a volatile read, so that `w` is read at sp+24
     * rather than through the register holding box2's address (which
     * also gives `this` r5 instead of r4). #662 round 2: a `w` local,
     * nested ifs, reusing `box` and an inline test of the box all keep
     * the register read; the permuter (score 30 of 80) only with an
     * uninitialized pointer. #662 round 3 (RTL): the C++ front end reads
     * `box2.w` as (mem (plus P 8)), with P a fresh copy of `fp + 16`.
     * cse1 ties P to the call's return-slot pseudo in the same block (the
     * one the ROM keeps in r5), so the load goes through it. Declaring
     * box2 in a block, at the top, in an `else`, or reusing `box` doesn't
     * change that, and no -f flag toggle does either. */
    if (*(volatile s32 *)&box2.w != 0 && gPlayer->TouchesBox(&box2))
        ResolvePlayerContact();
}

/* The sprite is touched; the player and the sprite hit each other as the
 * mask says: without a mask the player is hit (with the sprite's kind),
 * with mask level 1 or 2 both are, and an invincible player only hits. */
void MovingSprite::ResolvePlayerContact()
{
    f.b.bit3 = 1;
    switch (gLevelState->maskLevel) {
    case MASK_LEVEL_NONE:
        gPlayer->HandleEvent(0, kind, 0);
        break;
    case MASK_LEVEL_ONE:
    case MASK_LEVEL_TWO:
        gPlayer->HandleEvent(0, kind, 0);
        HandleEvent(1, EVENT_HIT, 0);
        break;
    case MASK_LEVEL_INVINCIBLE:
        HandleEvent(1, EVENT_HIT, 0);
        break;
    }
}
