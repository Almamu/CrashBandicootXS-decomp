#include "vehicle.hpp"
#include "audio.hpp"
#include "level_state.hpp"

extern "C" {
#include "math_util.h"
#include "util.h"
#include "actor.h"
#include "level.h"
#include "globals.h"
}

/* 0x08031A6C-0x08032480 (#664 part 11g): the jetpack levels' crates and
 * the parachute nitro (include/vehicle.hpp):
 *
 * - JetpackBalloonCrate (gJetpackBalloonCrateVtable): a crate hanging from
 *   a balloon (JetpackBalloon, jetpack_balloon.cpp), swaying around its
 *   spawn point on gSineTable (StateHang) until the balloon is shot and it
 *   falls (Break, StateFall), or it is shot itself and breaks (Damage).
 *   Its three kinds pay out when broken or touched: JetpackHealthCrate
 *   heals the player, JetpackTimeCrate freezes the level clock (or starts
 *   the time trial), JetpackQuestionCrate gives wumpa fruit or a life.
 * - JetpackParachuteNitro (gJetpackParachuteNitroVtable): rises, and
 *   explodes on the player or when shot.
 *
 * See docs/matching/archive/issue-59-0x08031784-actor.md and
 * issue-60-61-gap-31a6c-part2.md. */

/* The jetpack player, the actor list's root. */
static inline JetpackPlayer *Player()
{
    return (JetpackPlayer *)gActorList;
}

/* Sways around (x, y), hanging from a new balloon of record `kind` above
 * it. */
inline void JetpackBalloonCrate::Hang(s32 x, s32 y, s32 z, u8 kind)
{
    done = 0;
    centerX = x;
    centerY = y;
    phase = (u16)RandRange(0xFF);
    balloon = (JetpackBalloon *)SpawnJetpackBalloon(kind, x, y - 0x3DB6, z, (s32)this);
}

/* The kinds' constructors expand the constructor (InitJetpackBalloonCrate
 * is its out-of-line copy, below). */
inline JetpackBalloonCrate::JetpackBalloonCrate(const struct anim_table_record *rec, s32 x, s32 y,
                                                s32 z, s32 kind)
    : HpActor(rec, x, y, z, 2)
{
    Hang(x, y, z, kind);
}

/* Slot 2: its state, then gone once it has fallen (state 1) past a
 * height or its breaking (state 2) has played through; until then
 * ActorSelf's update. */
void JetpackBalloonCrate::Update()
{
    (this->*stateFuncs[state])();

    if (state == 1 && y > 0xE100) {
        delete this;
    } else if (state == 2 && animDone != 0) {
        delete this;
    } else {
        ActorSelf::Update();
    }
}

/* Slot 2: touched by the player while hanging (animation 0), it breaks
 * (state 2, animation 1) and pays out; then the crate's update. */
void JetpackQuestionCrate::Update()
{
    s32 kind = animIndex;

    if (kind == 0 && (u8)IsTouchingPlayer(this)) {
        SetState(2, 1);
        switch ((u8)record->index) {
        case 0x14:
            gAudioContext->PlaySfx(SFX_CRATE_BREAK, 0x100);
            Player()->QueueWumpa(1);
            break;
        case 0x15:
            gAudioContext->PlaySfx(SFX_CRATE_BREAK, 0x100);
            Player()->QueueWumpa(3);
            break;
        case 0x16:
            gAudioContext->PlaySfx(SFX_CRATE_BREAK, 0x100);
            Player()->QueueWumpa(5);
            break;
        case 0x17:
            gAudioContext->PlaySfx(SFX_EXTRA_LIFE, 0x100);
            MarkSpawnCollected(spawn);
            gLevelState->AddLife();
            break;
        }
        if (balloon != 0) {
            gLevelState->AddBrokenCrate();
            balloon->Release();
            balloon = 0;
        }
        done = 1;
    }

    JetpackBalloonCrate::Update();
}

/* Slot 4: once the hit points run out, it breaks (state 2, animation 1)
 * and pays out. */
void JetpackQuestionCrate::Damage(s32 amount)
{
    if ((hp -= amount) > 0) {
        return;
    }

    SetState(2, 1);
    switch ((u8)record->index) {
    case 0x14:
        gAudioContext->PlaySfx(SFX_CRATE_BREAK, 0x100);
        Player()->QueueWumpa(1);
        break;
    case 0x15:
        gAudioContext->PlaySfx(SFX_CRATE_BREAK, 0x100);
        Player()->QueueWumpa(3);
        break;
    case 0x16:
        gAudioContext->PlaySfx(SFX_CRATE_BREAK, 0x100);
        Player()->QueueWumpa(5);
        break;
    case 0x17:
        gAudioContext->PlaySfx(SFX_EXTRA_LIFE, 0x100);
        MarkSpawnCollected(spawn);
        gLevelState->AddLife();
        break;
    }
    if (balloon != 0) {
        gLevelState->AddBrokenCrate();
        balloon->Release();
        balloon = 0;
    }
    done = 1;
}

/* Slot 2: touched by the player while hanging, it breaks (state 2,
 * animation 1) and heals the player by 0x14; then the crate's update. */
void JetpackHealthCrate::Update()
{
    s32 kind = animIndex;

    if (kind == 0 && (u8)IsTouchingPlayer(this)) {
        SetState(2, 1);
        Player()->Heal(0x14);
        gAudioContext->PlaySfx(SFX_CRATE_BREAK, 0x100);
        if (balloon != 0) {
            gLevelState->AddBrokenCrate();
            balloon->Release();
            balloon = 0;
        }
        done = 1;
    }

    JetpackBalloonCrate::Update();
}

/* Slot 2: touched by the player while hanging, it breaks (state 2,
 * animation 1) and freezes the clock for 1-3 seconds, or (record 0x1D)
 * starts the time trial; then the crate's update. */
void JetpackTimeCrate::Update()
{
    s32 kind = animIndex;

    if (kind == 0 && (u8)IsTouchingPlayer(this)) {
        SetState(2, 1);
        switch ((u8)record->index) {
        case 0x18:
            gAudioContext->PlaySfx(SFX_CRATE_BREAK, 0x100);
            gLevelState->FreezeLevelClock(1);
            break;
        case 0x19:
            gAudioContext->PlaySfx(SFX_CRATE_BREAK, 0x100);
            gLevelState->FreezeLevelClock(2);
            break;
        case 0x1A:
            gAudioContext->PlaySfx(SFX_CRATE_BREAK, 0x100);
            gLevelState->FreezeLevelClock(3);
            break;
        case 0x1D:
            gAudioContext->PlaySfx(SFX_CLOCK, 0x100);
            gLevelState->StartTimeTrial();
            break;
        }
        if (balloon != 0) {
            if ((u8)record->index != 0x1D) {
                gLevelState->AddBrokenCrate();
            }
            balloon->Release();
            balloon = 0;
        }
        done = 1;
    }

    JetpackBalloonCrate::Update();
}

/* Slot 4: once the hit points run out, it breaks (state 2, animation 1)
 * and pays out as when touched. */
void JetpackTimeCrate::Damage(s32 amount)
{
    if ((hp -= amount) > 0) {
        return;
    }

    SetState(2, 1);
    switch ((u8)record->index) {
    case 0x18:
        gAudioContext->PlaySfx(SFX_CRATE_BREAK, 0x100);
        gLevelState->FreezeLevelClock(1);
        break;
    case 0x19:
        gAudioContext->PlaySfx(SFX_CRATE_BREAK, 0x100);
        gLevelState->FreezeLevelClock(2);
        break;
    case 0x1A:
        gAudioContext->PlaySfx(SFX_CRATE_BREAK, 0x100);
        gLevelState->FreezeLevelClock(3);
        break;
    case 0x1D:
        gAudioContext->PlaySfx(SFX_CLOCK, 0x100);
        gLevelState->StartTimeTrial();
        break;
    }
    if (balloon != 0) {
        if ((u8)record->index != 0x1D) {
            gLevelState->AddBrokenCrate();
        }
        balloon->Release();
        balloon = 0;
    }
    done = 1;
}

/* CreateJetpackTimeCrate: the balloon is record 0x28. */
JetpackTimeCrate::JetpackTimeCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
    : JetpackBalloonCrate(rec, x, y, z, 0x28)
{
}

/* Slot 4: once the hit points run out, it breaks (state 2, animation 1)
 * and heals the player by 0x14. */
void JetpackHealthCrate::Damage(s32 amount)
{
    if ((hp -= amount) > 0) {
        return;
    }

    SetState(2, 1);
    Player()->Heal(0x14);
    gAudioContext->PlaySfx(SFX_CRATE_BREAK, 0x100);
    if (balloon != 0) {
        gLevelState->AddBrokenCrate();
        balloon->Release();
        balloon = 0;
    }
    done = 1;
}

/* CreateJetpackHealthCrate: the balloon is record 0x2A. */
JetpackHealthCrate::JetpackHealthCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
    : JetpackBalloonCrate(rec, x, y, z, 0x2A)
{
}

/* CreateJetpackQuestionCrate: the balloon is record 0x29; `spawn` is the
 * level's spawn record, marked collected for the life. */
JetpackQuestionCrate::JetpackQuestionCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z,
                                           struct actor_spawn *spawn)
    : JetpackBalloonCrate(rec, x, y, z, 0x29)
{
    this->spawn = spawn;
}

/* Forgets the balloon (JetpackBalloon::Update, once the balloon is
 * gone). */
void JetpackBalloonCrate::ClearBalloon()
{
    balloon = 0;
}

/* Slot 7 (JetpackBalloon::Damage): its balloon was shot. It falls (state
 * 1, animation 0), and counts as broken. */
void JetpackBalloonCrate::Break()
{
    fallSpeed = 0;
    SetState(1, 0);
    gLevelState->AddBrokenCrate();
    balloon = 0;
}

/* Slot 4: once the hit points run out, it breaks (state 2, animation 1)
 * and lets its balloon go. */
void JetpackBalloonCrate::Damage(s32 amount)
{
    if ((hp -= amount) > 0) {
        return;
    }

    SetState(2, 1);
    if (balloon != 0) {
        gAudioContext->PlaySfx(SFX_CRATE_BREAK, 0x100);
        gLevelState->AddBrokenCrate();
        balloon->Release();
        balloon = 0;
    }
    done = 1;
}

/* Slot 1: ActorSelf's (the unlink); the class's own vtable store is dead
 * before it. */
JetpackBalloonCrate::~JetpackBalloonCrate()
{
}

/* UNUSED - no caller anywhere in the ROM (no call, and no pointer to it
 * in baserom.gba). InitJetpackBalloonCrate, the constructor out of line:
 * the kinds expand the inline one. */
JetpackBalloonCrate::JetpackBalloonCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z,
                                         u8 kind)
    : HpActor(rec, x, y, z, 2)
{
    Hang(x, y, z, kind);
}

/* gJetpackBalloonCrateStateFuncs[2], entered by Damage: Update deletes it
 * once the animation has played through. Empty. */
void JetpackBalloonCrate::StateDestroyed()
{
}

/* gJetpackBalloonCrateStateFuncs[1], entered by Break: falls ever faster
 * (`fallSpeed` grows by 0x12 a frame, up to 0x4C0). */
void JetpackBalloonCrate::StateFall()
{
    s32 pos = y;
    s32 v = fallSpeed;

    y = pos + v;
    v += 0x12;
    fallSpeed = v;
    if (v <= 0x4C0) {
        return;
    }
    fallSpeed = 0x4C0;
}

/* gJetpackBalloonCrateStateFuncs[0]: sways around its centre on
 * gSineTable, and moves its balloon with it. */
void JetpackBalloonCrate::StateHang()
{
    const s16 *trig = gSineTable;
    s32 t = phase + stateTime;
    s32 nx = centerX + trig[((t * 5) >> 4) & 0xFF] * 17;
    s32 ny;

    x = nx;
    ny = centerY + trig[((t * 8) >> 4) & 0xFF] * 30;
    y = ny;

    if (balloon != 0) {
        balloon->Move(nx, ny - 0x3DB6, z);
    }
}

/* UNUSED - no caller anywhere in the ROM (no call, and no pointer to it
 * in baserom.gba). Update's state step on its own. */
void JetpackBalloonCrate::RunState()
{
    (this->*stateFuncs[state])();
}

/* Slot 5: broken or touched. */
s32 JetpackBalloonCrate::IsUnshootable()
{
    return done;
}

/* Slot 2: once its explosion (animation 1) has played through, it is
 * gone. Until then it rises to `limitY`, and on the player it explodes,
 * hurting the player by 0x14; then ActorSelf's update. */
void JetpackParachuteNitro::Update()
{
    if (animIndex == 1) {
        if (animDone == 0) {
            goto tail;
        }
        delete this;
        return;
    }

    if ((u8)IsTouchingPlayer(this)) {
        Player()->Damage(0x14);
        gLevelState->AddBrokenCrate();
        gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
        RestartAnim(1);
        dead = 1;
    }

    if (y < limitY) {
        y += 0x140;
    }

tail:
    ActorSelf::Update();
}

/* Slot 4: once the hit points run out, it explodes (animation 1). */
void JetpackParachuteNitro::Damage(s32 amount)
{
    if ((hp -= amount) > 0) {
        return;
    }

    dead = 1;
    gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
    RestartAnim(1);
    gLevelState->AddBrokenCrate();
}

/* CreateJetpackParachuteNitro: built 0xFA00 below the spawn point, it
 * rises to the spawn's `y`. */
JetpackParachuteNitro::JetpackParachuteNitro(const struct anim_table_record *rec, s32 x, s32 y,
                                             s32 z)
    : HpActor(rec, x, -0xFA00, z, 2)
{
    limitY = y;
    dead = 0;
}

/* Slot 5: exploded. */
s32 JetpackParachuteNitro::IsUnshootable()
{
    return dead;
}
