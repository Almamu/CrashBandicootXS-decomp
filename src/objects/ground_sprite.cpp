#include "sprite_obj.hpp"

extern "C" {
#include "math_util.h"
#include "memory.h"
}

/* GroundSprite's update (#664, include/sprite_obj.hpp). */

/* Keeps the side of the hitbox the sprite rests on in place when the
 * hitbox changes (a new animation frame): if the hitbox record
 * (GetBounds) isn't the last one (`lastHitbox`, when there is one), `y`
 * moves by the difference of their bottom edges (offY + h) when the
 * sprite stands on the floor (hitAxes 8), or of their top edges (offY)
 * when it is against the ceiling (4). */
static inline void KeepHitboxAnchored(GroundSprite *self)
{
    const struct hitbox_quad *box = self->GetBounds();
    const struct hitbox_quad *prev = (const struct hitbox_quad *)self->lastHitbox;

    if (prev != box && prev != 0) {
        if (self->hitAxes == 8) {
            s32 prevBottom = prev->offY + prev->h;
            s32 bottom = box->offY + box->h;

            if (prevBottom != bottom)
                self->y += INT_TO_Q8(prevBottom - bottom);
        } else if (self->hitAxes == 4) {
            s32 prevTop = prev->offY;
            s32 top = box->offY;

            if (prevTop != top)
                self->y += INT_TO_Q8(prevTop - top);
        }
    }
    self->lastHitbox = (void *)box;
}

/* MovingSprite's update, then the hitbox anchoring. */
void GroundSprite::Update()
{
    MovingSprite::Update();
    KeepHitboxAnchored(this);
}

/* UNUSED - no caller anywhere in the ROM (checked src/, asm/ and the
 * method tables). The hitbox anchoring alone. */
void GroundSprite::AnchorHitbox()
{
    KeepHitboxAnchored(this);
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
