#include "vehicle.hpp"
#include "audio.hpp"
#include "level_state.hpp"
#include "yeti.hpp"

extern "C" {
#include "math_util.h"
#include "util.h"
#include "actor.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
}

/* The polar course's hazards (#664 part 11d, include/vehicle.hpp), ROM
 * 0x0802CC9C-0x0802D204, between polar_crates.cpp and polar_aku_aku.cpp:
 * the electric fence, the obstacle, the launcher, the penguin and the
 * icicle, each with its constructor. */

/* Switches to animation `idx`, keeping the frame unless it is past the
 * new animation's end (as PolarPlayer::Boost). */
static inline void SwitchAnim(ActorSelf *a, s32 idx)
{
    a->animIndex = idx;
    a->animTimer = a->anims[idx].duration;
    a->animDone = 0;
    if (a->GetAnimFrameBaseOffset() >= a->anims[a->animIndex].loopThreshold)
        a->animTime = 0;
}

/* Shown once past depth 0x15FF. Whole (animation 0), the yeti breaks it
 * on its record's box; the player is shocked on the wire
 * (gPolarElectricFenceWireBox; it breaks only if the shock counts) or
 * breaks it on a post (the left and right post boxes). Once broken, it
 * is deleted when the animation is done. The four hits (SFX_EXPLOSION,
 * animation 1) are written out: through an inline function, PlaySfx's
 * arguments are loaded in the other order. */
void PolarElectricFence::Update()
{
    if (depth > INT_TO_Q8(22) - 1)
        visible = 1;

    if (animIndex == 0) {
        box = record->box_14;
        if (Yeti::IsTouching(this)) {
            gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
            RestartAnim(1);
        }
        box = gPolarElectricFenceWireBox;
        if ((u8)IsTouchingPlayer(this)) {
            if ((u8)static_cast<PolarPlayer *>(gActorList)->Shock()) {
                gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
                RestartAnim(1);
            }
        } else {
            box = gPolarElectricFenceLeftPostBox;
            if ((u8)IsTouchingPlayer(this)) {
                gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
                RestartAnim(1);
            }
            box = gPolarElectricFenceRightPostBox;
            if ((u8)IsTouchingPlayer(this)) {
                gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
                RestartAnim(1);
            }
        }
    } else if (animDone) {
        delete this;
        return;
    }
    ActorSelf::Update();
}

/* Hidden until near enough (Update). */
PolarElectricFence::PolarElectricFence(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
    : ActorSelf(rec, x, y, z)
{
    visible = 0;
}

/* Hurts the player on touch; it never moves or breaks. */
void PolarObstacle::Update()
{
    if ((u8)IsTouchingPlayer(this))
        static_cast<PolarPlayer *>(gActorList)->Hurt();
    ActorSelf::Update();
}

PolarObstacle::PolarObstacle(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
    : ActorSelf(rec, x, y, z)
{
}

/* State 0: launches the player on touch, or is set off by the yeti
 * (state 1, animation 1, SFX_EXPLOSION); state 1: deleted once the
 * animation is done. */
void PolarLauncher::Update()
{
    switch (state) {
    case POLAR_LAUNCHER_STATE_READY:
        if ((u8)IsTouchingPlayer(this)) {
            static_cast<PolarPlayer *>(gActorList)->Launch();
            gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
            SetState(POLAR_LAUNCHER_STATE_EXPLODE, 1);
        } else if (Yeti::IsTouching(this)) {
            gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
            SetState(POLAR_LAUNCHER_STATE_EXPLODE, 1);
        }
        break;
    case POLAR_LAUNCHER_STATE_EXPLODE:
        if (animDone != 0) {
            delete this;
            return;
        }
        break;
    }
    ActorSelf::Update();
}

PolarLauncher::PolarLauncher(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
    : ActorSelf(rec, x, y, z)
{
}

/* Flies by its speed. In flight (state 0) it aims at its next spawn when
 * the countdown runs out, and is knocked away (state 1: to the course's
 * nearer side, up a random bit and on) by the yeti, or by the player if
 * that hurts it. The knock-away is written out twice: as an inline
 * function, PlaySfx's arguments are loaded in the other order. */
void PolarPenguin::Update()
{
    x += velX;
    y += velY;
    z += velZ;

    if (state == POLAR_PENGUIN_STATE_FLY) {
        if (--countdown <= 0)
            Aim(nextTarget);

        if ((u8)IsTouchingPlayer(this)) {
            if ((u8)static_cast<PolarPlayer *>(gActorList)->Hurt()) {
                velX = x > 0 ? 0x600 : -0x600;
                velY = -(s32)RandRange(0x300);
                velZ += INT_TO_Q8(2);
                gAudioContext->PlaySfx(SFX_ENEMY_KNOCKED_AWAY, 0x100);
                SetState(POLAR_PENGUIN_STATE_KNOCKED_AWAY, 0);
            }
        } else if (Yeti::IsTouching(this)) {
            velX = x > 0 ? 0x600 : -0x600;
            velY = -(s32)RandRange(0x300);
            velZ += INT_TO_Q8(2);
            gAudioContext->PlaySfx(SFX_ENEMY_KNOCKED_AWAY, 0x100);
            SetState(POLAR_PENGUIN_STATE_KNOCKED_AWAY, 0);
        }
    }

    ActorSelf::Update();
}

/* Flies to spawn `target` (actor_spawn.cpp's accessors) at its kind's
 * speed in Z (gPolarPenguinSpeeds), arriving in X and Y at the same
 * time; with no target, drifts on slowly for good. */
void PolarPenguin::Aim(s32 target)
{
    if (target < 0) {
        velY = 0;
        velX = 0;
        velZ = POLAR_PENGUIN_DRIFT_SPEED_Z;
        countdown = 0x40000000;
    } else {
        s32 n;
        s32 factor;

        velZ = gPolarPenguinSpeeds[GetActorSpawnKindIndex(target)];
        n = (GetActorSpawnZ(target) - z) / velZ;
        countdown = n;
        if (n == 0)
            countdown = 1;

        factor = 0x1000 / countdown;
        velX = Q12_MUL(factor, GetActorSpawnX(target) - x);
        velY = Q12_MUL(factor, GetActorSpawnY(target) - y);
        nextTarget = GetActorSpawnNextTarget(target);
    }
}

PolarPenguin::PolarPenguin(const struct anim_table_record *rec, s32 x, s32 y, s32 z,
                           struct spawn_arg *spawn)
    : ActorSelf(rec, x, y, z)
{
    Aim(spawn->target);
}

/* Hurts the player on touch, and falls in steps as it comes near: the
 * next animation at depth 0x5000 (state 0), 0x5A00 (state 1) and 0x6400
 * (state 2). */
void PolarIcicle::Update()
{
    if ((u8)IsTouchingPlayer(this))
        static_cast<PolarPlayer *>(gActorList)->Hurt();

    if ((depth > 0x6400 && state == POLAR_ICICLE_STATE_DROP2) ||
        (depth > 0x5a00 && state == POLAR_ICICLE_STATE_DROP1)) {
        SwitchAnim(this, animIndex + 1);
        state = state + 1;
    } else if (depth > 0x5000 && state == POLAR_ICICLE_STATE_HANGING) {
        SwitchAnim(this, animIndex + 1);
        state = state + 1;
    }

    ActorSelf::Update();
}

/* Its look: record POLAR_KIND_ICICLE_A, _B or _C, on the right (x > 0)
 * the next one (the _RIGHT kind), four animations each: `kind * 4 - 0x40`
 * is (kind - POLAR_KIND_ICICLE_A) * 4, as the ROM computes it. */
PolarIcicle::PolarIcicle(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
    : ActorSelf(rec, x, y, z)
{
    s32 kind = rec->index;

    if (x > 0)
        kind += 1;
    RestartAnim(kind * 4 - 0x40);
}
