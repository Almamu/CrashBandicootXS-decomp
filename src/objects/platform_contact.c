#include "core.h"
#include "math_util.h"
#include "match.h"
#include "gobj_1a794.h"
#include "objects.h"

/* GitHub issue #25, ROM 0x0801AB34-0x0801AB98: CheckPlatformContact, struct
 * gobj's method-table +0x0C entry - if the player (gPlayer) is
 * active and within 0x7FFF (Q8) on both axes, run the collision test
 * ResolvePlatformCollision. See include/gobj_1a794.h. */

s32 CheckPlatformContact(struct gobj *self)
{
    if (self->type != 6 || self->frame <= 0x12) {
        MATCH_HOLD_REG(struct player *, p, r3) = gPlayer;
        void *arg = (void *)((struct ctrl *)p->ctrl)->state;
        MATCH_HOLD_REG(u32, f, r1) = p->flags.all;
        MATCH_HOLD_REG(u32, top, r0) = f >> 7;

        if (top) {
            MATCH_HOLD_REG(s32, d, r2) = self->x;

            d -= p->x;
            MAKE_ABS(d);
            if (d <= 0x7FFF) {
                d = self->y;
                d -= p->y;
                MAKE_ABS(d);
                if (d <= 0x7FFF)
                    ResolvePlatformCollision(self, arg);
            }
        }
        {
            s32 mask = ~8;

            self->flags = mask & self->flags;
        }
    }
    return 0;
}
