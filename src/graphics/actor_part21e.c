#include "core.h"
#include "actor_self.h"

/* Same boss-weapon "self"/tracker object family as actor_part20.c/
 * actor_part21c.c/actor_part21d.c - see actor_part20.c's header
 * comment and docs/matching/issue-58-0x08030334-actor.md.
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
 * `sub_8030E08` and, past a higher position ceiling (`0x4300`),
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
extern s32 gAirshipZ;
extern s32 gAirshipVelZ;
extern s32 gAirshipFireTimer;
extern struct actor_self *gActorList;
extern s32 __divsi3(s32 arg0, s32 arg1);
extern s32 gAirshipX;
extern s32 gAirshipY;
extern s32 SpawnJetpackCannonball(s32 x, s32 y, s32 z, s32 dx, s32 dy);
extern s32 gAirshipVolleyCount;
extern s32 *gAirshipAttack;
extern s32 gAirshipDistance;
extern struct actor_self *gAirship;
extern s32 GetAnimFrameBaseOffset(void *self);
extern s32 gAirshipState;
extern s32 gAirshipStateTimer;
extern void sub_8030E08(void);
extern void UpdateAirshipFlashColor(void);

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

static inline s32 Abs(s32 x)
{
    s32 s = x >> 31;

    return (x ^ s) - s;
}

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
            dx = ((pl->x - (gAirshipX - 0xCDB)) * speed) >> 12;
            dy = ((pl->y - (gAirshipY + 0x516D)) * speed) >> 12;
            if (Abs(dx) + Abs(dy) <= 0x7FF) {
                SpawnJetpackCannonball(gAirshipX - 0xCDB, gAirshipY + 0x516D, gAirshipZ - 10, dx, dy);
                if (++gAirshipVolleyCount == gAirshipAttack[5]) {
                    gAirshipVolleyCount = phase;
                    gAirshipFireTimer = gAirshipAttack[6];
                } else {
                    gAirshipFireTimer = gAirshipAttack[4];
                }
            }
        }
    } else {
        gAirshipFireTimer = phase - 1;
    }
    sub_8030E08();
    if (gAirshipDistance > 0x4300) {
        gAirshipFireTimer = gAirshipAttack[1];
        gAirshipVolleyCount = 0;
        BossSetState(2, 0);
    }
    UpdateAirshipFlashColor();
}
