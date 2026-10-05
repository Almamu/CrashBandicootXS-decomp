#include "core.h"
#include "actor_self.h"

/* Same boss-weapon "self"/tracker object family as actor_part20.c/
 * actor_part21c.c - see actor_part20.c's header comment and
 * docs/matching/issue-58-0x08030334-actor.md.
 *
 * `AirshipStateApproach`'s (actor_part21c.c) companion: advances
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
extern s32 GetAnimFrameBaseOffset(void *self);
extern void UpdateAirshipFlashColor(void);
extern void SteerAirship(void);
extern s32 SpawnAirshipFireball(s32 x, s32 y, s32 z);

extern s32 gAirshipZ;
extern s32 gAirshipVelZ;
extern s32 gAirshipDistance;
extern s32 gAirshipState;
extern s32 gAirshipStateTimer;
extern struct actor_self *gAirship;
extern s32 gAirshipFireTimer;
extern s32 gAirshipVolleyCount;
extern s32 *gAirshipAttack;
extern s32 gAirshipX;
extern s32 gAirshipY;

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
        if (++gAirshipVolleyCount == gAirshipAttack[2]) {
            gAirshipVolleyCount = 0;
            gAirshipFireTimer = gAirshipAttack[3];
        } else {
            gAirshipFireTimer = gAirshipAttack[1];
        }
    } else {
        gAirshipFireTimer--;
    }
    SteerAirship();
    if (gAirshipDistance <= 0x31FF) {
        gAirshipFireTimer = gAirshipAttack[4];
        gAirshipVolleyCount = 0;
        BossSetState(3, 0);
    }
    UpdateAirshipFlashColor();
}
