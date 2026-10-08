#include "player.hpp"

extern "C" {
#include "globals.h"
#include "math_util.h"
}

/* The player's constructor (Player, include/player.hpp; InitPlayer).
 * g++ constructs the ground sprite (InitGroundSprite), stores the vtable
 * pointer and constructs the collision queue (ResetCollisionQueue, which
 * empties it); the body creates Aku Aku (`child`, sprite bank 0xCC), resets
 * the player and sets the spawn's id and position. */
Player::Player(u16 id, u16 px, u16 py, u16 unused)
{
    Sprite *c = Sprite::Create(0, 0, 0, 0);

    child = c;
    c->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + (0xcc << 1));
    {
        /* One 0 for both stores, kept in a register across the calls, as
         * in the ROM (two literals give `id` and the 0 each other's
         * registers). */
        s32 zero = 0;

        c->tag = zero;
        c->ResetFrameTimer();
        c->ResetFrameIndex();
        c->SetAnimDone(0);
        maskTrailIdx = zero;
    }
    Reset();

    this->id = id;
    x = INT_TO_Q8((s32)px);
    y = INT_TO_Q8((s32)py);
}
