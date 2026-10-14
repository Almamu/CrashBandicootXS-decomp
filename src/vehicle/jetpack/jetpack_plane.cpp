#include "vehicle.hpp"
#include "boss_actors.hpp"
#include "audio.hpp"
#include "actor_spawns.hpp"
#include "actor_category.hpp"

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

/* Slot 2: drawn only past a depth; flies by its velocity (Q4: sixteenths
 * of its Q8 position's unit) and runs its state. While its low pose
 * (animation 3) plays and the cooldown has run out, it fires a cannonball
 * at the player when the player is close in front (every third shot takes
 * a long cooldown). It and the player damage each other on contact, and
 * it is gone once it has fallen (state 3) past a height; until then
 * ActorSelf's update. */
void JetpackPlane::Update()
{
    if (depth > INT_TO_Q8(27)) {
        visible = 1;
    } else {
        visible = 0;
    }
    x += Q4_TO_INT(velX);
    y += Q4_TO_INT(velY);
    z += Q4_TO_INT(velZ);

    (this->*stateFuncs[state])();

    if (animIndex == 3) {
        s32 cd = cooldown;

        if (cd == 0) {
            ActorSelf *player = gActorList;
            s32 angle = (player->z - (z - JETPACK_PLANE_GUN_OFFSET_Z)) / -0x1AA;

            if (angle > 0 && depth <= INT_TO_Q8(140) - 1) {
                s32 scale = 0x1000 / angle;
                s32 rawDx = (player->x - x) * scale;
                s32 dx = Q12_TO_INT(rawDx);
                s32 rawDy = (player->y - y) * scale;
                s32 dy = Q12_TO_INT(rawDy);
                s32 signDx = rawDx >> 31;
                /* `absDx` is needed (inlined, the instructions are reordered); `absDy`
                 * stays beside it */
                s32 absDx = (dx ^ signDx) - signDx;
                s32 signDy = rawDy >> 31;
                s32 absDy = (dy ^ signDy) - signDy;

                if (absDx + absDy <= 0x5FF) {
                    JetpackCannonball::Spawn(x, y, z - JETPACK_PLANE_GUN_OFFSET_Z, dx, dy);
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

    if (dying == 0 && (u8)ActorCategory::IsTouchingPlayer(this)) {
        gActorList->AsHpActor()->Damage(6);
        Damage(4);
    }

    if (state == JETPACK_PLANE_STATE_FALL && y > INT_TO_Q8(225))
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
        if (velZ <= JETPACK_PLANE_SLOW_HOP_SPEED) {
            SetState(JETPACK_PLANE_STATE_FOLLOW, 3);
        } else {
            accX = 0;
            accY = 0;
            steps = 0x40000000;
        }
    } else {
        s32 scale;
        s32 scale2;

        velZ = gJetpackPlaneHopSpeeds[ActorSpawns::GetKindIndex(target)];
        steps = Q4_TO_INT(Q8_DIV(ActorSpawns::GetZ(target) - z, velZ));
        if (steps == 0) {
            steps = 1;
        }
        /* scale2 is assigned inside the X term: the ROM doubles the scale
         * after the ActorSpawns::GetX call (assigned before, the shift comes
         * first). */
        scale = 0x8000 / steps;
        // clang-format off
        accX = ((((ActorSpawns::GetX(target) - x) - Q4_TO_INT(velX * steps)) * scale >> 13) *
                (scale2 = scale * 2)) >> 13;
        accY = ((((ActorSpawns::GetY(target) - y) - Q4_TO_INT(velY * steps)) * scale >> 13) *
                scale2) >> 13;
        // clang-format on
        next = ActorSpawns::GetNextTarget(target);
    }

    if (velZ <= JETPACK_PLANE_SLOW_HOP_SPEED) {
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
    SetState(JETPACK_PLANE_STATE_KNOCKED_OUT, idx);
    gAudioContext->PlaySfx(SFX_JETPACK_PLANE_DOWN, 0x100);
}

/* CreateJetpackPlane: 4 hit points; a spawn whose first target needs a
 * fast hop starts further back, with the fast speed. */
JetpackPlane::JetpackPlane(const struct anim_table_record *rec, s32 x, s32 y, s32 z,
                           struct spawn_arg *spawn)
    : HpActor(rec, x, y, z, 4)
{
    cooldown = 0x3c;
    shotCount = 0;
    dying = 0;
    velY = 0;
    velX = 0;
    velZ = JETPACK_PLANE_SLOW_HOP_SPEED;
    if (spawn->target >= 0 && gJetpackPlaneHopSpeeds[ActorSpawns::GetKindIndex(spawn->target)] >
                                  JETPACK_PLANE_SLOW_HOP_SPEED) {
        this->z += -INT_TO_Q8(142);
        velZ = JETPACK_PLANE_FAST_START_SPEED;
    }
    Aim(spawn->target);
}

/* gJetpackPlaneStateFuncs[3], entered by StateKnockedOut: falls, the
 * speed capped at JETPACK_PLANE_MAX_FALL_SPEED. */
void JetpackPlane::StateFall()
{
    velY += accY;
    LIMIT_MAX(velY, JETPACK_PLANE_MAX_FALL_SPEED);
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
    accY = JETPACK_PLANE_FALL_GRAVITY;
    if (animIndex == 1) {
        idx = 2;
    } else {
        idx = 5;
    }
    SetState(JETPACK_PLANE_STATE_FALL, idx);
}

/* gJetpackPlaneStateFuncs[1], entered by Aim at the end of a slow hop
 * chain: 1/256 of the way to the player each frame (the Q4 speed is the
 * gap >> JETPACK_PLANE_FOLLOW_SHIFT), so the plane follows it (and fires
 * from pose 3, Update). */
void JetpackPlane::StateFollow()
{
    ActorSelf *player = gActorList;

    velX = (player->x - x) >> JETPACK_PLANE_FOLLOW_SHIFT;
    velY = (player->y - y) >> JETPACK_PLANE_FOLLOW_SHIFT;
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
u8 JetpackPlane::IsUnshootable()
{
    return dying;
}
