#include "sprite_obj.hpp"

extern "C" {
#include "math_util.h"
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
