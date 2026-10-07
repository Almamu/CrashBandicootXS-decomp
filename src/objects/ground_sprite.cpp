#include "sprite_obj.hpp"

extern "C" {
#include "memory.h"
}

/* GroundSprite's methods (#664, include/sprite_obj.hpp): the constructor
 * and destructor and the flag accessors. */

void GroundSprite::Draw()
{
    Sprite::Draw();
}

s32 GroundSprite::GetClassId()
{
    return 6;
}

GroundSprite *GroundSprite::Create(u16 id, u16 x, u16 y, u16)
{
    return new GroundSprite(id, x, y);
}

GroundSprite::~GroundSprite()
{
}

/* Always collides and vulnerable; no speed, ramp, direction, controller,
 * type or last hitbox; standing on the floor (hitAxes 8), probing it. */
void GroundSprite::Reset()
{
    f.b.collides = 1;
    f.b.vulnerable = 1;
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
    mover = 0;
    type = 0;
    lastHitbox = 0;
    f.b.floorProbe = 1;
}

GroundSprite::GroundSprite()
{
    Reset();
}

u8 GroundSprite::IsGrounded()
{
    return (f.bytes.flags2 >> 1) & 1;
}

void GroundSprite::ClearGrounded()
{
    f.b.grounded = 0;
}

void GroundSprite::SetGrounded()
{
    f.b.grounded = 1;
}

u8 GroundSprite::IsFloorProbeEnabled()
{
    return f.bytes.flags2 & 1;
}

void GroundSprite::DisableFloorProbe()
{
    f.b.floorProbe = 0;
}

void GroundSprite::EnableFloorProbe()
{
    f.b.floorProbe = 1;
}

void GroundSprite::ClearFlag5()
{
    f.b.unk_5 = 0;
}

void GroundSprite::SetFlag5()
{
    f.b.unk_5 = 1;
}

u8 GroundSprite::GetFlag5()
{
    return (f.flags >> 5) & 1;
}

Ctrl *GroundSprite::GetMover()
{
    return mover;
}
