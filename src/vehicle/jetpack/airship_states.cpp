#include "boss_actors.hpp"
#include "vehicle.hpp"
#include "audio.hpp"
#include "level_state.hpp"

extern "C" {
#include <libgcc.h>
#include "actor.h"
#include "vehicle.h"
#include "globals.h"
#include "math_util.h"
#include "util.h"
}

/* The airship's states (#664 part 11i, include/boss_actors.hpp):
 * gAirshipStateFuncs[1-5], the attack states, the explosion and the
 * fall (the last two airship_explode.cpp and airship_fall.cpp until
 * #771; state 0 is AirshipStateInactive, airship_graphics.cpp). See
 * docs/matching/archive/issue-58-0x08030334-actor.md.
 *
 * State 1: the airship comes in (gAirshipZ by gAirshipVelZ) until its
 * distance is down to 0x81FF, then stops and enters state 2 (fireballs)
 * with animation 0, and pauses the actor spawns. */
void AirshipStateApproach(void)
{
    gAirshipZ += gAirshipVelZ;

    if (gAirshipDistance <= 0x81FF) {
        /* both addresses first, as the ROM loads them */
        s32 *velX = &gAirshipVelX;
        s32 *velY = &gAirshipVelY;

        *velY = 0;
        *velX = 0;
        SetAirshipState(2, 0);
        PauseActorSpawns();
    }
}

/* State 2: `AirshipStateApproach`'s (above) companion: advances
 * `gAirshipZ` by its per-frame delta the same way, but also
 * ramps `gAirshipVelZ` itself toward a fixed target (`0x98`,
 * +-1/frame). Drives a small phase counter (`gAirshipFireTimer`) that,
 * on its "armed" phase (0), spawns an effect via `SpawnAirshipFireball` centered
 * on a fixed camera offset and advances a per-effect counter
 * (`gAirshipVolleyCount`) through a small weapon-kind table
 * (`gAirshipAttack`)'s thresholds, otherwise just decrements the
 * phase. Always re-runs the position-easing helper `SteerAirship`, and -
 * while `gAirshipDistance` hasn't crossed its (lower) ceiling
 * `0x31FF` - re-arms the phase from the weapon table and enters
 * state 3 (cannon) with animation 0. Always finishes with
 * `UpdateAirshipFlashColor` (the palette bank-1 flash-color select). */

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
        SetAirshipState(3, 0);
    }
    UpdateAirshipFlashColor();
}

/* State 3: `AirshipStateApproach`/`AirshipStateFireballs`'s third sibling: advances
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
 * re-arms the phase from the weapon table (`+4`) and goes back to
 * state 2 (fireballs) with animation 0, then always finishes with `UpdateAirshipFlashColor`.
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
        ActorSelf *pl = gActorList;
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
        SetAirshipState(2, 0);
    }
    UpdateAirshipFlashColor();
}

/* gAirshipStateFuncs[4], the explosion: advances the position
 * accumulators (`gAirshipX`/`gAirshipY`/
 * `gAirshipZ`), clears `gAirshipHitFlashTimer`'s DMA-refresh
 * counter, and derives two base screen coordinates from a fixed
 * keyframe-table box (`gAirshipBox`, `>>8`) offset by the
 * accumulators. Dispatches on `gAirshipStateTimer` (a frame/flags
 * counter, the same one `LoadAirshipGraphics`'s palette fade reads) through
 * five weapon-kind cases (0xa/0x32/0x50/0x6e/0xaa), each clearing one
 * BG palette bank-1 slot then spawning 1-3 sub-projectiles via
 * `RandRange` (a per-axis jitter/randomizer) and `CreateJetpackExplosion` (the
 * actual spawn call, `(x, y, z)`); the 0xaa case instead
 * enters state 5 (falling) with animation 1,
 * plays a sound, and - gated by a lock byte
 * (`gLevelState+0x8c`) and a spawn-budget counter
 * (`gAirshipCheckpointCount`) - spawns a homing/seek effect via
 * `SetJetpackCheckpoint`/`CreateJetpackCheckpointText`.
 *
 * Matching notes: the box table is `const` (so its jitter ranges stay
 * CSE'd in registers across the spawn calls), the palette base pointer
 * is assigned right where the ROM materializes it (declared-and-
 * initialized at the top it gets hoisted into a callee-saved register),
 * the RNG `RandRange` is read back as a `u16` here (the ROM zero-
 * extends its result), and the seek spawn takes `&gActorList`
 * before the last lock check, as the ROM loads that address early. */

/* One sub-projectile, jittered around (x, y) by the box's own +-range. */
#define SPAWN(x, y) CreateJetpackExplosion((x) + RandRange(INT_TO_Q8(gAirshipBox.w)), \
                                (y) + RandRange(INT_TO_Q8(gAirshipBox.h)), \
                                gAirshipZ - 0x100)

void AirshipStateExplode(void)
{
    s32 x, y;
    u16 *pal;

    gAirshipX += gAirshipVelX;
    gAirshipY += gAirshipVelY;
    gAirshipZ += gAirshipVelZ;
    gAirshipHitFlashTimer = 0;
    pal = (u16 *)(BG_PLTT + 0x20);
    x = gAirshipX + INT_TO_Q8(gAirshipBox.x);
    y = gAirshipY + INT_TO_Q8(gAirshipBox.y);

    if (gAirshipStateTimer == 0xa) {
        pal[15] = 0;
        SPAWN(x, y);
    } else if (gAirshipStateTimer == 0x32) {
        pal[1] = 0;
        SPAWN(x, y);
        SPAWN(x, y);
    } else if (gAirshipStateTimer == 0x50) {
        pal[4] = 0;
        SPAWN(x, y);
        SPAWN(x, y);
        SPAWN(x, y);
    } else if (gAirshipStateTimer == 0x6e) {
        pal[8] = 0;
        SPAWN(x, y);
        SPAWN(x, y);
        SPAWN(x, y);
        SPAWN(x, y);
    } else if (gAirshipStateTimer == 0xaa) {
        ResumeActorSpawns();
        SetAirshipState(5, 1);
        gAudioContext->PlaySfx(SFX_UNKNOWN_42, 0x100);
        gAirshipVelZ = 0x9d;
        if (gLevelState->timeTrial == 0 && gAirshipCheckpointCount <= 1) {
            ActorSelf **pl = &gActorList;
            if (gJetpackPlayerInactive == 0) {
                ((JetpackPlayer *)*pl)->SetCheckpoint();
                CreateJetpackCheckpointText();
                gAirshipCheckpointCount++;
            }
        }
    }
}

/* gAirshipStateFuncs[5], after the explosion: the airship falls (its Y
 * speed grows by 7 a frame up to 0x140) until it is past 0xBB80, then
 * goes back to state 0 (inactive) with animation 0 and BG2 off. */
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
        SetAirshipState(0, 0);
        REG_DISPCNT &= 0xfbff;
    }
}
