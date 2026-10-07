#include "platform.hpp"

/* Platform::CheckPlayerContact (#664, include/platform.hpp), ROM
 * 0x0801AB34-0x0801AB98: CheckPlatformContact, gPlatformVtable's slot 1. */

/* Unless it is a Neo Cortex platform that has crumbled (type 6 past frame
 * 0x12): if the player collides and is within 0x7FFF (Q8) on both axes,
 * resolves the collision (ResolveCollision); then clears the touched
 * flag. */
s32 Platform::CheckPlayerContact()
{
    if (type != 6 || frame <= 0x12) {
        struct player *p = gPlayer;
        void *arg = (void *)((Ctrl *)p->ctrl)->state;
        s32 collides = p->flags.all >> 7;

        if (collides) {
            s32 d = x;

            d -= p->x;
            MAKE_ABS(d);
            if (d <= 0x7FFF) {
                d = y;
                d -= p->y;
                MAKE_ABS(d);
                if (d <= 0x7FFF)
                    ResolveCollision(arg);
            }
        }
        ClearTouched();
    }
    return 0;
}
