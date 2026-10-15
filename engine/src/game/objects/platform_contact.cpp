#include "game/objects/platform.hpp"
#include "game/crash/player.hpp"

using Game::Crash::Player;
using Game::Objects::Platform;

/* Platform::CheckPlayerContact (#664, include/game/objects/platform.hpp), ROM
 * 0x0801AB34-0x0801AB98: CheckPlatformContact, gPlatformVtable's slot 1. */

/* Unless it is a flip platform that has tipped over (PLATFORM_TYPE_FLIP
 * past frame 0x12): if the player collides and is within 0x7FFF (Q8) on both axes,
 * resolves the collision (ResolveCollision); then clears the touched
 * flag. */
s32 Platform::CheckPlayerContact()
{
    if (type != PLATFORM_TYPE_FLIP || frame <= 0x12) {
        Player *p = gPlayer;
        /* `state` is needed: inlined, the instructions differ */
        void *state = (void *)(intptr_t)p->mover->state;
        s32 collides = p->f.flags >> ENTITY_BIT_COLLIDES;

        if (collides) {
            s32 d = x;

            d -= p->x;
            MAKE_ABS(d);
            if (d <= INT_TO_Q8(128) - 1) {
                d = y;
                d -= p->y;
                MAKE_ABS(d);
                if (d <= INT_TO_Q8(128) - 1)
                    ResolveCollision(state);
            }
        }
        ClearTouched();
    }
    return 0;
}
