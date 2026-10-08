#include "sprite_obj.hpp"
#include "ctrl.hpp"

extern "C" {
#include "math_util.h"
#include "memory.h"
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
