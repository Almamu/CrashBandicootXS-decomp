#include "vehicle.hpp"
#include "boss_actors.hpp"
#include "audio.hpp"

extern "C" {
#include "math_util.h"
#include "actor.h"
#include "globals.h"
}

/* The jetpack plane (#664 part 11f, include/vehicle.hpp), ROM
 * 0x0802FA38-0x0802FF08, between jetpack_shot.cpp and jetpack_bomber.cpp:
 * JetpackPlane (gJetpackPlaneVtable), one of the jetpack levels'
 * HpActors. It hops between the level's spawn points (Aim and the
 * GetActorSpawn* accessors, docs/rom_map.md), fires cannonballs at the
 * player, and has 4 hit points. See
 * docs/matching/archive/issue-56-0x0802f0dc-actor.md,
 * issue-57-0x0802fbf0-actor.md and pmf-dispatch-retry.md. */

/* The jetpack player, the actor list's root. */
static inline HpActor *PlayerActor()
{
    return (HpActor *)gActorList;
}

/* Slot 2: drawn only past a depth; flies by its velocity (Q4) and runs
 * its state. While its low pose (animation 3) plays and the cooldown has
 * run out, it fires a cannonball at the player when the player is close
 * in front (every third shot takes a long cooldown). It and the player
 * damage each other on contact, and it is gone once it has fallen (state
 * 3) past a height; until then ActorSelf's update. */
void JetpackPlane::Update()
{
    if (depth > 0x1B00) {
        visible = 1;
    } else {
        visible = 0;
    }
    x += velX >> 4;
    y += velY >> 4;
    z += velZ >> 4;

    (this->*stateFuncs[state])();

    if (animIndex == 3) {
        s32 cd = cooldown;

        if (cd == 0) {
            ActorSelf *player = gActorList;
            s32 angle = (player->z - (z - 10)) / -0x1AA;

            if (angle > 0 && depth <= 0x8BFF) {
                s32 scale = 0x1000 / angle;
                s32 rawDx = (player->x - x) * scale;
                s32 dx = Q12_TO_INT(rawDx);
                s32 rawDy = (player->y - y) * scale;
                s32 dy = Q12_TO_INT(rawDy);
                s32 signDx = rawDx >> 31;
                s32 absDx = (dx ^ signDx) - signDx;
                s32 signDy = rawDy >> 31;
                s32 absDy = (dy ^ signDy) - signDy;

                if (absDx + absDy <= 0x5FF) {
                    SpawnJetpackCannonball(x, y, z - 10, dx, dy);
                    if (++shotCount == 3) {
                        shotCount = cd;
                        cooldown = 0x3C;
                    } else {
                        cooldown = 0x14;
                    }
                }
            }
        } else {
            cooldown = cd - 1;
        }
    }

    if (dying == 0 && (u8)IsTouchingPlayer(this)) {
        PlayerActor()->Damage(6);
        Damage(4);
    }

    if (state == 3 && y > 0xE100)
        delete this;
    else
        ActorSelf::Update();
}

/* Aims the next hop at spawn point `target`: the hop speed for its kind,
 * the step count from the depth to cover, and the accelerations that
 * land on its x and y after that many steps. A negative target ends the
 * chain: state 1 (following the player) if the speed is low, else a
 * practically endless glide. Then restarts the low (3) or high (0)
 * animation. */
void JetpackPlane::Aim(s32 target)
{
    if (target < 0) {
        if (velZ <= 0x955) {
            SetState(1, 3);
        } else {
            accX = 0;
            accY = 0;
            steps = 0x40000000;
        }
    } else {
        s32 scale;
        s32 scale2;

        velZ = gJetpackPlaneHopSpeeds[GetActorSpawnKindIndex(target)];
        steps = ((GetActorSpawnZ(target) - z) << 8) / velZ >> 4;
        if (steps == 0) {
            steps = 1;
        }
        /* scale2 is assigned inside the X term: the ROM doubles the scale
         * after the GetActorSpawnX call (assigned before, the shift comes
         * first). */
        scale = 0x8000 / steps;
        // clang-format off
        accX = ((((GetActorSpawnX(target) - x) - ((velX * steps) >> 4)) * scale >> 13) *
                (scale2 = scale * 2)) >> 13;
        accY = ((((GetActorSpawnY(target) - y) - ((velY * steps) >> 4)) * scale >> 13) *
                scale2) >> 13;
        // clang-format on
        next = GetActorSpawnNextTarget(target);
    }

    if (velZ <= 0x955) {
        animIndex = 3;
        animTimer = anims[3].duration;
        animDone = 0;
        if (GetAnimFrameBaseOffset() >= anims[animIndex].loopThreshold) {
            animTime = 0;
        }
    } else {
        animIndex = 0;
        animTimer = anims[0].duration;
        animDone = 0;
        if (GetAnimFrameBaseOffset() >= anims[animIndex].loopThreshold) {
            animTime = 0;
        }
    }
}

/* Slot 4: once the hit points run out, it is dying: half its speed (and
 * half its climb), and the knock-out animation (1 or 4, for its pose) in
 * state 2. */
void JetpackPlane::Damage(s32 amount)
{
    s32 idx;

    if ((hp -= amount) > 0) {
        return;
    }
    dying = 1;
    velX /= 2;
    if (velY < 0) {
        velY /= 2;
    }
    if (animIndex == 0) {
        idx = 1;
    } else {
        idx = 4;
    }
    SetState(2, idx);
    gAudioContext->PlaySfx(SFX_JETPACK_PLANE_DOWN, 0x100);
}

/* CreateJetpackPlane: 4 hit points; a spawn whose first target needs a
 * fast hop starts further back, with the fast speed. */
JetpackPlane::JetpackPlane(const struct anim_table_record *rec, s32 x, s32 y, s32 z,
                           struct spawn_arg *arg)
    : HpActor(rec, x, y, z, 4)
{
    cooldown = 0x3c;
    shotCount = 0;
    dying = 0;
    velY = 0;
    velX = 0;
    velZ = 0x955;
    if (arg->target >= 0 && gJetpackPlaneHopSpeeds[GetActorSpawnKindIndex(arg->target)] > 0x955) {
        this->z += -0x8e00;
        velZ = 0xd55;
    }
    Aim(arg->target);
}

/* gJetpackPlaneStateFuncs[3], entered by StateKnockedOut: falls, the
 * speed capped at 0x1400. */
void JetpackPlane::StateFall()
{
    velY += accY;
    LIMIT_MAX(velY, 0x1400);
}

/* gJetpackPlaneStateFuncs[2], entered by Damage with the knock-out
 * animation (1 or 4): when it finishes, the falling animation (2 or 5)
 * in state 3 with a fixed Y acceleration. */
void JetpackPlane::StateKnockedOut()
{
    s32 idx;

    if (animDone == 0) {
        return;
    }
    accY = 0xa0;
    if (animIndex == 1) {
        idx = 2;
    } else {
        idx = 5;
    }
    SetState(3, idx);
}

/* gJetpackPlaneStateFuncs[1], entered by Aim at the end of a slow hop
 * chain: a 16th of the way to the player each frame, so the plane
 * follows it (and fires from pose 3, Update). */
void JetpackPlane::StateFollow()
{
    ActorSelf *player = gActorList;

    velX = (player->x - x) >> 4;
    velY = (player->y - y) >> 4;
}

/* gJetpackPlaneStateFuncs[0]: accelerates, and aims the next hop once
 * the steps run out. */
void JetpackPlane::StateFly()
{
    velX += accX;
    velY += accY;
    if (--steps <= 0) {
        Aim(next);
    }
}

/* Update's state dispatch, without the rest. */
void JetpackPlane::RunState()
{
    (this->*stateFuncs[state])();
}

/* Slot 5: a dying plane can't be shot. */
s32 JetpackPlane::IsUnshootable()
{
    return dying;
}
