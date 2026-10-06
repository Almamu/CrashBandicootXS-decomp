#include "core.h"
#include "match.h"
#include "actor_self.h"
#include "actor.h"
#include "bosses.h"

/* Same boss-weapon "self" object family as airship_fireball.c - see that
 * file's header comment and docs/matching/archive/issue-58-0x08030334-actor.md.
 * `gAirship` here is a *separate*, smaller (0x1c-byte) tracker
 * object of the same shape, allocated by `CreateAirship` (left raw in
 * this chunk). */

/* Advances the boss-weapon's screen-space accumulators
 * (`gAirshipX`/`gAirshipY`/`gAirshipZ`) by
 * their per-frame deltas, clamps the `gAirshipVelY` ramp to
 * `0x140`, and - once `gAirshipY` exceeds a threshold - resets
 * the small tracker object at `gAirship` to state 0 (table-
 * index 0) and clears `DISPCNT`'s bit 10 (window/mosaic-family bit not
 * otherwise named yet). */
void AirshipStateFall(void)
{
    s32 total;
    s32 delta;

    gAirshipX += gAirshipVelX;

    total = gAirshipY + gAirshipVelY;
    gAirshipY = total;

    gAirshipZ += gAirshipVelZ;

    delta = gAirshipVelY + 7;
    gAirshipVelY = delta;
    if (delta > 0x140) {
        gAirshipVelY = 0x140;
    }

    if (total > 0xbb80) {
        struct actor_self *self;
        MATCH_HOLD_REG(s32, zero, r5) = 0;

        gAirshipState = zero;
        gAirshipStateTimer = zero;

        self = gAirship;
        self->animIndex = zero;
        {
            MATCH_HOLD_REG(u16, anim, r0) = self->anims[0].duration;
            MATCH_HOLD_REG(u8, zero1, r1) = 0;

            *(u16 *)&self->animTimer = anim;
            *(u8 *)&self->animDone = zero1;
        }

        {
            s32 frame = GetAnimFrameBaseOffset(self);
            MATCH_HOLD_REG(s32, idx, r2) = self->animIndex;
            MATCH_HOLD_REG(u8 *, table, r3) = (u8 *)self->anims;
            MATCH_HOLD_REG(u8 *, entryPtr, r1) = (u8 *)(idx * 0xc);
            MATCH_HOLD_REG(s32, four, r2);
            MATCH_HOLD_REG(s32, val, r1);

            asm("add %0, %0, %1" : "+r" (entryPtr) : "r" (table));
            four = 4;
            val = *(s16 *)(entryPtr + four);

            if (frame >= val) {
                self->animTime = zero;
            }
        }

        REG_DISPCNT &= 0xfbff;
    }
}
