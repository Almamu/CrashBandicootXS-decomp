#include "player.hpp"

extern "C" {
#include "util.h"
#include "objects.h"
#include "math_util.h"
}

/* The player's (Player, include/player.hpp) motion, update, body box test
 * and destructor. */

/* Steps each speed toward its ramp's target by the ramp's step, stopping
 * at the target; sets `dir` from the signs of the speeds; saves the
 * previous position and moves. MovingSprite::ApplyVelocity's code, with
 * the player's own global for its redundant store (gLastPlayerVelY, the
 * word after gLastSpriteVelY). Returns whether the player moves. */
s32 Player::ApplyVelocity()
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
    if (gLastPlayerVelY != 0 && speedY == 0)
        gLastPlayerVelY = speedY;
    gLastPlayerVelY = speedY;
    return speedX != 0 || speedY != 0;
}

u8 Player::HasRampYTarget()
{
    if (rampY.target != 0) {
        return 1;
    } else {
        return 0;
    }
}

void Player::ClearSpeedY()
{
    speedY = 0;
}

/* Clamps the Y speed and its ramp's start and step to 0 or below.
 * UNUSED: nothing calls it. */
void Player::StopFalling()
{
    LIMIT_MAX(speedY, 0);
    LIMIT_MAX(rampY.start, 0);
    LIMIT_MAX(rampY.step, 0);
}

/* Counts the crate-break limiter down, then the ground sprite's update. */
void Player::Update()
{
    if (countdown != 0) {
        countdown -= 1;
    }
    GroundSprite::Update();
}

/* Whether the player's body box (the frame's) overlaps `box`; an empty
 * body box touches nothing. */
u8 Player::TouchesBox(struct aabb *box)
{
    u8 result = 0;
    struct aabb body = GetBodyBox();

    if (body.w > 0) {
        result = AabbOverlaps(&body, box);
    }
    return result;
}

/* Deletes Aku Aku; g++ then destroys the collision queue (flags 2) and
 * the ground sprite. */
Player::~Player()
{
    delete child;
}
