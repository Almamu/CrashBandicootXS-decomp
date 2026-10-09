#include "vehicle.hpp"
#include "boss_actors.hpp"
#include "audio.hpp"

extern "C" {
#include "math_util.h"
#include "actor.h"
#include "globals.h"
}

/* The jetpack bomber (#664 part 11f, include/vehicle.hpp), ROM
 * 0x0802FF08-0x08030298, between jetpack_plane.cpp and
 * jetpack_cannonball.cpp: JetpackBomber (gJetpackBomberVtable), 2 hit
 * points; its record's kind (4-9) picks its first state, whose movers
 * circle its home point on gSineTable or drift toward the player. See
 * docs/matching/archive/issue-57-0x0802fbf0-actor.md. */

/* The jetpack player, the actor list's root. */
static inline HpActor *PlayerActor()
{
    return (HpActor *)gActorList;
}

/* CreateJetpackBomber: 2 hit points, its home point at (x, y); the
 * record's kind (4-9) picks the first state (0-5). */
JetpackBomber::JetpackBomber(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
    : HpActor(rec, x, y, z, 2)
{
    homeX = x;
    homeY = y;
    unshootable = 0;
    switch ((u8)rec->index) {
    case 4:
        SetState(0, 0);
        break;
    case 5:
        SetState(1, 0);
        break;
    case 6:
        SetState(2, 0);
        break;
    case 7:
        SetState(3, 0);
        break;
    case 8:
        SetState(4, 0);
        break;
    case 9:
        SetState(5, 0);
        break;
    }
}

/* Slot 2: while alive, it counts itself (the player's bomber sound,
 * JetpackPlayer::CountBomber) and on contact damages the player (10) and
 * explodes (state 6). Then its state; once the explosion has played
 * through it is gone, else it sinks a little and ActorSelf's update. */
void JetpackBomber::Update()
{
    if (state != 6) {
        ((JetpackPlayer *)gActorList)->CountBomber();
        if (state != 6 && (u8)IsTouchingPlayer(this)) {
            PlayerActor()->Damage(10);
            palette = 4;
            gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
            SetState(6, 1);
        }
    }

    (this->*stateFuncs[state])();

    if (state == 6 && animDone != 0) {
        delete this;
    } else {
        z += 0x60;
        ActorSelf::Update();
    }
}

/* A 32nd of the way to the player. */
void JetpackBomber::Home()
{
    ActorSelf *player = gActorList;

    x += (player->x - x) >> 5;
    y += (player->y - y) >> 5;
}

/* gJetpackBomberStateFuncs[6]: exploding (Update deletes it once the
 * animation is done). */
void JetpackBomber::StateDying()
{
}

/* gJetpackBomberStateFuncs[5]: comes at the player faster. */
void JetpackBomber::StateDrop()
{
    z -= 0x88;
}

/* gJetpackBomberStateFuncs[4]: circles its home point (radius 60) while
 * far, then homes in on the player. */
void JetpackBomber::StateCircle()
{
    if (depth > 0x35ff) {
        const s16 *sine = gSineTable;
        s32 angle = ((stateTime << 4) >> 4) & 0xff;

        x = homeX + sine[(angle + 0x40) & 0xff] * 60;
        y = homeY + sine[angle] * 60;
    } else {
        Home();
    }
}

/* gJetpackBomberStateFuncs[3]: StateCircle's x alone (radius 80, slower). */
void JetpackBomber::StateSwingHorizontal()
{
    if (depth > 0x35ff) {
        x = homeX + COS_Q8(((stateTime * 10) >> 4) & 0xff) * 80;
    } else {
        Home();
    }
}

/* gJetpackBomberStateFuncs[2]: StateCircle's y alone. */
void JetpackBomber::StateBobVertical()
{
    if (depth > 0x35ff) {
        y = homeY + SIN_Q8((stateTime << 4) >> 4) * 60;
    } else {
        Home();
    }
}

/* gJetpackBomberStateFuncs[1]: homes in on the player once near. */
void JetpackBomber::StateHome()
{
    if (depth <= 0x35ff) {
        Home();
    }
}

/* gJetpackBomberStateFuncs[0], kind 4's: no movement of its own
 * (Update's sinking still applies). */
void JetpackBomber::StateIdle()
{
}

/* Slot 4: explodes (state 6) once the hit points run out. */
void JetpackBomber::Damage(s32 amount)
{
    if (state != 6 && (hp -= amount) <= 0) {
        palette = 4;
        gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
        SetState(6, 1);
    }
}

/* Update's state dispatch, without the rest. */
void JetpackBomber::RunState()
{
    (this->*stateFuncs[state])();
}

s32 JetpackBomber::IsUnshootable()
{
    return unshootable;
}
