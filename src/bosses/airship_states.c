#include "core.h"
#include "match.h"
#include "actor_self.h"
#include <libgcc.h>
#include "actor.h"
#include "vehicle.h"
#include "bosses.h"
#include "globals.h"
#include "math_util.h"

/* Same boss-weapon "self"/tracker object family as airship_fireball.c/
 * airship_fall.c/airship_damage.c - see airship_fireball.c's header comment
 * and docs/matching/archive/issue-58-0x08030334-actor.md. */

/* Boss-weapon camera-relative position accumulator: advances
 * `gAirshipZ` by its per-frame delta (`gAirshipVelZ`),
 * and - while `gAirshipDistance` hasn't crossed its ceiling
 * (`0x81FF`) - resets the velocity group (`gAirshipVelY`/
 * `gAirshipVelX`) and fires the state-2/table-index-0 transition
 * on the small tracker object (`gAirship`, anim frame from
 * its own part-table pointer at `+0`), then always calls
 * `PauseActorSpawns`. Same "pin the zero constant so it's loaded before its
 * address" idiom as `AirshipStateFall` (airship_fall.c) - the value is
 * reused across four stores that would otherwise get reordered ahead
 * of the address loads that consume them. The `*(T *)&self->...` stores keep
 * gcc from treating them as struct-member accesses, which would let the
 * scheduler move the `anims[0]` load below the zero constant. */
void AirshipStateApproach(void)
{
    gAirshipZ += gAirshipVelZ;

    if (gAirshipDistance <= 0x81FF) {
        struct actor_self *self;
        s32 *p1558 = &gAirshipVelX;
        s32 *p155C = &gAirshipVelY;
        MATCH_HOLD_REG(s32, zero, r5) = 0;

        *p155C = zero;
        *p1558 = zero;
        {
            MATCH_HOLD_REG(s32, two, r1) = 2;
            gAirshipState = two;
        }
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

            asm("add %0, %0, %1" : "+r"(entryPtr) : "r"(table));
            four = 4;
            val = *(s16 *)(entryPtr + four); /* anims[idx].loopThreshold */

            if (frame >= val) {
                self->animTime = zero;
            }
        }

        PauseActorSpawns();
    }
}

/* Same boss-weapon "self"/tracker object family as airship_fireball.c
 * and the code above - see airship_fireball.c's header comment and
 * docs/matching/archive/issue-58-0x08030334-actor.md.
 *
 * `AirshipStateApproach`'s (above) companion: advances
 * `gAirshipZ` by its per-frame delta the same way, but also
 * ramps `gAirshipVelZ` itself toward a fixed target (`0x98`,
 * +-1/frame). Drives a small phase counter (`gAirshipFireTimer`) that,
 * on its "armed" phase (0), spawns an effect via `SpawnAirshipFireball` centered
 * on a fixed camera offset and advances a per-effect counter
 * (`gAirshipVolleyCount`) through a small weapon-kind table
 * (`gAirshipAttack`)'s thresholds, otherwise just decrements the
 * phase. Always re-runs the position-easing helper `SteerAirship`, and -
 * while `gAirshipDistance` hasn't crossed its (lower) ceiling
 * `0x31FF` - re-arms the phase from the weapon table and fires the
 * state-3/table-index-0 transition on the tracker object
 * (`gAirship`), same shape as `AirshipStateApproach`. Always finishes
 * with `UpdateAirshipFlashColor` (the palette bank-1 flash-color select).
 *
 * The tracker transition is a `static inline` helper (state/table-index
 * as parameters): that is what makes the compiler materialize the
 * state constant right before its own store instead of hoisting its
 * address load ahead of it (the gap that kept this NAKED before). */

static inline void BossSetState(s32 st, s32 idx)
{
    struct actor_self *self;
    gAirshipState = st;
    gAirshipStateTimer = 0;
    self = gAirship;
    self->animIndex = idx;
    self->animTimer = self->anims[idx].duration;
    self->animDone = 0;
    if (GetAnimFrameBaseOffset(self) >= self->anims[self->animIndex].loopThreshold)
        self->animTime = 0;
}

void AirshipStateFireballs(void)
{
    s32 v;
    gAirshipZ += gAirshipVelZ;
    v = gAirshipVelZ;
    if (v <= 0x98)
        gAirshipVelZ = v + 1;
    else
        gAirshipVelZ = v - 1;

    if (gAirshipFireTimer == 0) {
        SpawnAirshipFireball(gAirshipX - 0xCDB, gAirshipY + 0x516D, gAirshipZ - 10);
        if (++gAirshipVolleyCount == gAirshipAttack->fireballBurst) {
            gAirshipVolleyCount = 0;
            gAirshipFireTimer = gAirshipAttack->fireballBurstDelay;
        } else {
            gAirshipFireTimer = gAirshipAttack->fireballDelay;
        }
    } else {
        gAirshipFireTimer--;
    }
    SteerAirship();
    if (gAirshipDistance <= 0x31FF) {
        gAirshipFireTimer = gAirshipAttack->cannonDelay;
        gAirshipVolleyCount = 0;
        BossSetState(3, 0);
    }
    UpdateAirshipFlashColor();
}

/* Same boss-weapon "self"/tracker object family as airship_fireball.c
 * and the code above - see airship_fireball.c's header
 * comment and docs/matching/archive/issue-58-0x08030334-actor.md.
 *
 * `AirshipStateApproach`/`AirshipStateFireballs`'s third sibling: advances
 * `gAirshipZ` by its per-frame delta and ramps
 * `gAirshipVelZ` toward `0xb2` the same +-1/frame way. While the
 * phase counter (`gAirshipFireTimer`) is armed (0), computes the
 * player's (`gActorList`) distance from a target point
 * (`+0x24` axis, offset `+0xa` minus the accumulated position) via
 * `__divsi3`, and - only once that "speed" term is positive -
 * computes a signed Manhattan-style distance in X/Y (`+0x1c`/`+0x20`
 * against `gAirshipX`/`gAirshipY`, scaled by the speed
 * term, `abs`-combined) and, while under a `0x7FF` threshold, spawns an
 * effect via `SpawnJetpackCannonball` (the 5-argument, velocity-carrying sibling of `AirshipStateFireballs`'s
 * `SpawnAirshipFireball`) and advances the same `gAirshipVolleyCount` counter
 * through the weapon table's next threshold slot (`+0x14`/`+0x18`/
 * `+0x10`). Otherwise the phase just decrements. Always re-runs
 * `SteerAirship` and, past a higher position ceiling (`0x4300`),
 * re-arms the phase from the weapon table (`+4`) and fires the
 * state-2/table-index-0 transition on the tracker object, then always
 * finishes with `UpdateAirshipFlashColor`.
 *
 * The distances are written `a - (b - K)`: gcc's `fold` reassociates
 * that into `(a + K) - b`, which is exactly what keeps the ROM from
 * CSE-ing it with the spawn call's own `b - K` arguments (written
 * `(a + K) - b` directly, the compiler shares `b - K` instead). The
 * `/` goes through the ROM's own `__divsi3` and the
 * absolute values are the branchless `asrs`/`eors`/`subs` form. */

void AirshipStateCannon(void)
{
    s32 v;
    s32 phase;

    gAirshipZ += gAirshipVelZ;
    v = gAirshipVelZ;
    if (v <= 0xb2)
        gAirshipVelZ = v + 1;

    phase = gAirshipFireTimer;
    if (phase == 0) {
        struct actor_self *pl = gActorList;
        s32 speed = (pl->z - (gAirshipZ - 10)) / -0x1AA;
        if (speed > 0) {
            s32 dx, dy;
            speed = 0x1000 / speed;
            dx = Q12_MUL(pl->x - (gAirshipX - 0xCDB), speed);
            dy = Q12_MUL(pl->y - (gAirshipY + 0x516D), speed);
            if (ABS_BRANCHLESS(dx) + ABS_BRANCHLESS(dy) <= 0x7FF) {
                SpawnJetpackCannonball(gAirshipX - 0xCDB, gAirshipY + 0x516D, gAirshipZ - 10, dx,
                                       dy);
                if (++gAirshipVolleyCount == gAirshipAttack->cannonBurst) {
                    gAirshipVolleyCount = phase;
                    gAirshipFireTimer = gAirshipAttack->cannonBurstDelay;
                } else {
                    gAirshipFireTimer = gAirshipAttack->cannonDelay;
                }
            }
        }
    } else {
        gAirshipFireTimer = phase - 1;
    }
    SteerAirship();
    if (gAirshipDistance > 0x4300) {
        gAirshipFireTimer = gAirshipAttack->fireballDelay;
        gAirshipVolleyCount = 0;
        BossSetState(2, 0);
    }
    UpdateAirshipFlashColor();
}
