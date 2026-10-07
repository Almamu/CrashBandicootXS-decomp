#include "crate.hpp"

extern "C" {
#include "globals.h"
}

/* Crate::Draw (#664, include/crate.hpp). */

/* An idle crate (not busy, state 0) restarts its animation's first step
 * (the clamp is ClampFrame's); then the sprite is drawn, and once the
 * animation has ended the touched flag is cleared. */
void Crate::Draw()
{
    u8 s = state;

    if ((s & CRATE_STATE_BUSY) == 0) {
        u8 idle = s & CRATE_STATE_MASK;

        if (idle == 0) {
            animDone = idle;
            ClampFrame(0);
        }
    }
    ((SpriteRenderer *)gSpriteRenderer)->Draw(this);
    if (animDone != 0)
        ClearTouched();
}
