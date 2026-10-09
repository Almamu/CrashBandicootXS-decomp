#include "sprite_obj.hpp"
#include "player.hpp"
#include "level_state.hpp"
#include "ctrl.hpp"

extern "C" {
#include "player.h"
#include "constants/events.h"
#include "math_util.h"
#include "memory.h"
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

    if (AabbW(&box) != 0 && gPlayer->TouchesBox(&box)) {
        ResolvePlayerContact();
        return;
    }
    struct aabb box2 = GetBodyBox();

    /* The widths through aabb.h's AabbW: `w` is read at sp+24, while
     * box2's address stays in r5 for the call and TouchesBox, as in the
     * ROM. Written `box2.w`, the C++ front end reads it as (mem (plus P
     * 8)), with P a fresh copy of `fp + 16` (expr.c copies a BLKmode
     * local's address, which Thumb's GO_IF_LEGITIMATE_ADDRESS rejects),
     * and cse1 ties P to the return slot's pseudo, so the load went
     * through r5 (a volatile read until #662 round 4). Inlined, AabbW's
     * argument is the constant `fp + 16` itself (integrate.c), and its
     * load folds to sp+24. */
    if (AabbW(&box2) != 0 && gPlayer->TouchesBox(&box2))
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

/* MovingSprite's methods (#664, include/sprite_obj.hpp): the motion, the
 * constructor and destructor, and the update. */

/* Steps each speed towards its ramp's target by the ramp's step, never
 * past it; sets `dir` from the speeds' signs (1 right, 2 left, 8 down, 4
 * up), keeps the position in prevX/prevY and moves by the speeds. The Y
 * speed goes to gLastSpriteVelY (the ROM stores it twice when it is 0 and
 * the old value isn't). Returns whether the sprite moves. */
s32 MovingSprite::ApplyVelocity()
{
    if (speedX < rampX.target) {
        speedX += rampX.step;
        if (speedX > rampX.target)
            speedX = rampX.target;
    } else if (speedX > rampX.target) {
        speedX -= rampX.step;
        if (speedX < rampX.target)
            speedX = rampX.target;
    }
    if (speedY < rampY.target) {
        speedY += rampY.step;
        if (speedY > rampY.target)
            speedY = rampY.target;
    } else if (speedY > rampY.target) {
        speedY -= rampY.step;
        if (speedY < rampY.target)
            speedY = rampY.target;
    }

    dir = 0;
    if (speedX > 0)
        dir = 1;
    else if (speedX < 0)
        dir = 2;
    if (speedY > 0)
        dir |= 8;
    else if (speedY < 0)
        dir |= 4;

    PrevPos() = Pos();
    x += speedX;
    y += speedY;
    if (gLastSpriteVelY != 0 && speedY == 0)
        gLastSpriteVelY = speedY;
    gLastSpriteVelY = speedY;
    return speedX != 0 || speedY != 0;
}

void MovingSprite::SetPrevPos(s32 px, s32 py)
{
    prevX = px;
    prevY = py;
}

struct vec2 MovingSprite::GetPrevPos()
{
    return PrevPos();
}

s32 MovingSprite::GetPrevY()
{
    return Q8_TO_INT(prevY);
}

s32 MovingSprite::GetPrevX()
{
    return Q8_TO_INT(prevX);
}

s32 MovingSprite::GetClassId()
{
    return 5;
}

MovingSprite *MovingSprite::Create(u16 id, u16 x, u16 y, u16)
{
    return new MovingSprite(id, x, y);
}

/* The controller goes with the sprite. */
MovingSprite::~MovingSprite()
{
    delete mover;
}

/* Vulnerable, not solid; no speed, ramp, direction or controller;
 * standing on the floor (hitAxes 8). */
void MovingSprite::Reset()
{
    f.b.vulnerable = 1;
    f.b.solid = 0;
    speedX = 0;
    speedY = 0;
    rampX.start = 0;
    rampX.step = 0;
    rampX.target = 0;
    rampY.start = 0;
    rampY.step = 0;
    rampY.target = 0;
    hitAxes = 8;
    dir = 0;
    probeTries = 0;
    mover = 0;
    unk_40 = 0;
}

MovingSprite::MovingSprite()
{
    Reset();
}

/* Sprite's update, then the controller's. */
void MovingSprite::Update()
{
    Sprite::Update();
    if (mover != 0)
        mover->Update(this);
}
