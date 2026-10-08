#include "vehicle.hpp"
#include "boss_actors.hpp"

extern "C" {
#include "math_util.h"
#include "audio.h"
#include "actor.h"
#include "globals.h"
}

/* 0x08031784-0x08031A6C (#664 part 11f): the end of the airship's code
 * (its HP gauge, its teardown and its idle state, C linkage) and the
 * balloon the jetpack levels' crates hang from, JetpackBalloon
 * (include/vehicle.hpp). See docs/matching/archive/issue-58-0x08030334-actor.md
 * and issue-62-0x08033804-actor.md. */

/* The airship's hit points as a percentage of its attack's (the HUD's
 * gauge, UpdateHudPercentCounters), at least 1 while it has any; -1 while
 * no airship is active (gAirshipState 0). */
s32 GetAirshipHpPercent(void)
{
    s32 hp;
    s32 result;

    if (gAirshipState == 0) {
        return -1;
    }

    hp = gAirshipHp;
    result = hp * 100 / gAirshipAttack->hp;
    if (result == 0 && hp > 0) {
        result = 1;
    }
    return result;
}

/* Frees the airship (CreateAirship's `new AnimPart`, airship.cpp). */
void DestroyAirship(void)
{
    delete gAirship;
}

/* UNUSED - no caller anywhere in the ROM (checked every src/ .c file, the
 * category vtables and every word-aligned Thumb pointer in baserom.gba).
 * Empty; it has no table slot to name it after. */
void nullsub_30(void)
{
}

/* gAirshipStateFuncs[0]: no airship is active (AirshipStateFall goes back
 * to state 0). Empty. */
void AirshipStateInactive(void)
{
}

/* Slot 2: the depth; once it has fallen behind the camera (more than
 * 0x200 past gActorNearClipDepth) it lets go of its crate and is gone,
 * as it is once its pop (state 2) has played through or it has floated
 * away (state 1) past a height; until then its state. */
void JetpackBalloon::Update()
{
    UpdateDepth();
    if (depth < gActorNearClipDepth - 0x200) {
        if (crate != 0) {
            ClearJetpackCrateBalloon(crate);
            crate = 0;
        }
        delete this;
    } else if ((state == 2 && animDone != 0) || (state == 1 && y < -0xE100)) {
        delete this;
    } else {
        RunState();
    }
}

/* UNUSED - no caller anywhere in the ROM (no call, and no pointer to it in
 * baserom.gba). Forgets the crate. */
void JetpackBalloon::ClearCrate()
{
    crate = 0;
}

/* Slot 4: once the hit points run out, it is dying: it breaks its crate
 * off (the crate's slot 7), and pops (state 2, animation 1). */
void JetpackBalloon::Damage(s32 amount)
{
    if ((hp -= amount) > 0) {
        return;
    }
    dying = 1;
    if (crate != 0) {
        crate->Break();
        crate = 0;
    }
    PlaySfx(gAudioContext, SFX_UNKNOWN_2E, 0x100);
    SetState(2, 1);
}

/* Lets go of the crate and floats away (state 1, animation 0). */
void JetpackBalloon::Release()
{
    crate = 0;
    velY = 0;
    SetState(1, 0);
}

/* The animation step: counts the state's frames and plays the
 * animation, looping the sequence back at the keyframe's loopThreshold
 * (ActorSelf::Update's, without the depth). Inline here, and StatePop
 * out of line. */
inline void JetpackBalloon::Animate()
{
    stateTime++;
    animTime += (s16)animTimer;
    animDone = 0;
    if (GetAnimFrameBaseOffset() >= anims[animIndex].loopThreshold) {
        ANIM_REWIND(animTime, anims[animIndex]);
        animDone = 1;
    }
}

/* Moved to (x, y, z) by its crate (UpdateJetpackBalloonCrate's states),
 * then the animation step. */
void JetpackBalloon::Move(s32 x, s32 y, s32 z)
{
    this->x = x;
    this->y = y;
    this->z = z;
    Animate();
}

/* CreateJetpackBalloon: 2 hit points, holding `crate`. */
JetpackBalloon::JetpackBalloon(const struct anim_table_record *rec, s32 x, s32 y, s32 z,
                               JetpackBalloonCrate *crate)
    : HpActor(rec, x, y, z, 2)
{
    this->crate = crate;
    dying = 0;
}

/* gJetpackBalloonStateFuncs[2], entered by Damage: the pop's animation. */
void JetpackBalloon::StatePop()
{
    Animate();
}

/* gJetpackBalloonStateFuncs[1], entered by Release: rises ever faster
 * (`velY` falls by 6 a frame, and is at most -0x12C), then the animation
 * step. */
void JetpackBalloon::StateFloatAway()
{
    y += velY;
    velY -= 6;
    LIMIT_MAX(velY, -0x12C);
    Animate();
}

/* gJetpackBalloonStateFuncs[0]: tied to its crate, which moves it (Move)
 * until Release. Empty. */
void JetpackBalloon::StateAttached()
{
}

/* Update's state dispatch. */
void JetpackBalloon::RunState()
{
    (this->*stateFuncs[state])();
}

/* Slot 5: a popping balloon can't be shot. */
s32 JetpackBalloon::IsUnshootable()
{
    return dying;
}
