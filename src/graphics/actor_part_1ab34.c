#include "core.h"
#include "gobj_1a794.h"

/* GitHub issue #25, ROM 0x0801AB34-0x0801AB98: CheckPlatformContact, struct
 * gobj's method-table +0x0C entry - if the player (gPlayer) is
 * active and within 0x7FFF (Q8) on both axes, run the collision test
 * sub_801AB98. See include/gobj_1a794.h. */

s32 CheckPlatformContact(struct gobj *self)
{
    if (self->type != 6 || self->frame <= 0x12)
    {
        register struct gobj *p asm("r3") = gPlayer;
        void *arg = *(void **)((u8 *)p->mover + 8);
        register u32 f asm("r1") = p->flags;
        register u32 top asm("r0") = f >> 7;

        if (top)
        {
            register s32 d asm("r2") = self->x;

            d -= p->x;
            if (d < 0)
                d = -d;
            if (d <= 0x7FFF)
            {
                d = self->y;
                d -= p->y;
                if (d < 0)
                    d = -d;
                if (d <= 0x7FFF)
                    sub_801AB98(self, arg);
            }
        }
        {
            s32 mask = ~8;

            self->flags = mask & self->flags;
        }
    }
    return 0;
}
