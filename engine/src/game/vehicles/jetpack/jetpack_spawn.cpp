#include "game/vehicles/vehicle.hpp"
#include "game/bosses/boss_actors.hpp"
#include "game/bosses/hovercraft.hpp"
#include "game/vehicles/jetpack/airship.hpp"
#include "engine/audio/audio.hpp"
#include "game/level/level_state.hpp"
#include "game/vehicles/polar/yeti.hpp"
#include "engine/actor/collected_spawns.hpp"

extern "C" {
#include "engine/util/math_util.h"
#include "match.h"
#include "engine/sys/system.h"
#include "engine/actor/actor.h"
#include "game/bosses/bosses.h"
#include "engine/gfx/gfx.h"
#include "game/level/level.h"
#include "globals.h"
}

using Engine::Actor::CollectedSpawns;
using Game::Bosses::Hovercraft;
using Game::Bosses::HovercraftCannon;
using Game::Bosses::HovercraftCannonFlash;
using Game::Bosses::HovercraftFireball;
using Game::Bosses::HovercraftLauncher;
using Game::Bosses::HovercraftSideGun;
using Game::Entities::ActorSelf;
using Game::Entities::AnimPart;
using Game::Vehicles::Jetpack::Airship;
using Game::Vehicles::Jetpack::AirshipFireball;
using Game::Vehicles::Jetpack::JetpackBalloon;
using Game::Vehicles::Jetpack::JetpackBalloonCrate;
using Game::Vehicles::Jetpack::JetpackBomber;
using Game::Vehicles::Jetpack::JetpackCannonball;
using Game::Vehicles::Jetpack::JetpackCheckpointText;
using Game::Vehicles::Jetpack::JetpackCollectedWumpa;
using Game::Vehicles::Jetpack::JetpackExplosion;
using Game::Vehicles::Jetpack::JetpackHealthCrate;
using Game::Vehicles::Jetpack::JetpackParachuteNitro;
using Game::Vehicles::Jetpack::JetpackPlane;
using Game::Vehicles::Jetpack::JetpackPlayer;
using Game::Vehicles::Jetpack::JetpackQuestionCrate;
using Game::Vehicles::Jetpack::JetpackRing;
using Game::Vehicles::Jetpack::JetpackRocket;
using Game::Vehicles::Jetpack::JetpackShot;
using Game::Vehicles::Jetpack::JetpackTimeCrate;
using Game::Vehicles::Polar::Yeti;

/* The jetpack levels' spawners (#664 part 11e, engine/include/game/vehicles/vehicle.hpp), ROM
 * 0x0802E0A4-0x0802E740, between yeti.cpp and jetpack_player_update.cpp:
 * the level's spawn dispatcher `CreateJetpackActor` (a 31-case `switch`
 * over the spawn "kind", indexing the per-kind record table
 * `gJetpackAnimTable`) and its helpers: `SpawnJetpackActor` picks a spawn
 * record's kind byte and forwards to it, the run of small spawners
 * (`CreateJetpackCheckpointText`-`SpawnJetpackShot`) each build one object
 * from a fixed record of the same table, and `CreateJetpackPlayer` builds
 * the player. Since #915 they are static members: the first two and
 * CreateJetpackPlayer of JetpackPlayer (SpawnActor, CreateActor, Create),
 * each small spawner of the class it builds (`JetpackShot::Spawn` & co.).
 * It starts with the yeti's `Yeti::StateStop` (YetiStateStop,
 * engine/include/game/vehicles/polar/yeti.hpp), which stays here across the flag boundary with
 * yeti.cpp.
 *
 * Built with old_agbcp (as the C was with old_agbcc). */

/* Once the current animation has played through, switches the yeti to
 * animation sequence 3 (unless it's already on it), restarting its timer
 * from that sequence's first frame. */
void Yeti::StateStop()
{
    AnimPart *self = anim;

    if (self->animIndex != 3 && self->animDone != 0) {
        self->animIndex = 3;
        self->animTimer = self->anims[3].duration;
        self->animDone = 0;
        self->animTime = 0;
    }
}

/* Spawns the object a level spawn record describes: its kind comes from
 * `kind`, `altKind` in the alternate game mode (kind 0x17 there becomes
 * 0x14) or `bonusKind` when `alt` is set. Kind 0x1d only spawns while
 * `IsCrystalSaved` allows it; kinds 0, 0x3e and 0x20-0x25 never do. */
ActorSelf *JetpackPlayer::SpawnActor(struct actor_spawn *rec, u8 alt, s32 dz)
{
    u8 kind = rec->kind;
    s32 x, y, z;

    if (gLevelState->timeTrial != 0) {
        kind = rec->altKind;
        if (kind == JETPACK_KIND_QUESTION_CRATE_LIFE)
            kind = JETPACK_KIND_QUESTION_CRATE_WUMPA_1;
    } else if (alt != 0) {
        kind = rec->bonusKind;
    }
    if (kind == JETPACK_KIND_STOPWATCH && !gLevelState->IsCrystalSaved())
        return 0;
    if (kind == JETPACK_KIND_PLAYER || kind == JETPACK_KIND_UNK62 ||
        (u8)(kind - JETPACK_KIND_PATH_0) <= JETPACK_KIND_PATH_5 - JETPACK_KIND_PATH_0)
        return 0;
    x = INT_TO_Q8(rec->x);
    y = INT_TO_Q8(rec->y);
    z = INT_TO_Q8(rec->z) + dz;
    if ((u8)(kind - JETPACK_KIND_AIRSHIP_0) <= JETPACK_KIND_AIRSHIP_2 - JETPACK_KIND_AIRSHIP_0) {
        Airship::Spawn(kind - JETPACK_KIND_AIRSHIP_0, x, y, z);
    } else if (kind != JETPACK_KIND_HOVERCRAFT) {
        return CreateActor(kind, x, y, z, rec);
    } else {
        Hovercraft::Spawn(0, x, y, z);
    }
    return 0;
}

/* The spawn dispatcher: offsets the position by the kind's record and
 * constructs the kind's object. Kind 23 turns into kind 20's object
 * when `CollectedSpawns::Contains` says so; kind 31 spawns a kind-43 companion first. */
ActorSelf *JetpackPlayer::CreateActor(u8 kind, s32 x, s32 y, s32 z, struct actor_spawn *spawn)
{
    x += gJetpackAnimTable[kind].spawnX;
    y += gJetpackAnimTable[kind].spawnY;
    switch (kind) {
    case JETPACK_KIND_PLANE:
        return new JetpackPlane(&gJetpackAnimTable[kind], x, y, z, (struct spawn_arg *)spawn);
    case JETPACK_KIND_BOMBER_IDLE:
    case JETPACK_KIND_BOMBER_HOME:
    case JETPACK_KIND_BOMBER_BOB_VERTICAL:
    case JETPACK_KIND_BOMBER_SWING_HORIZONTAL:
    case JETPACK_KIND_BOMBER_CIRCLE:
    case JETPACK_KIND_BOMBER_DROP:
        return new JetpackBomber(&gJetpackAnimTable[kind], x, y, z);
    case JETPACK_KIND_HEALTH_CRATE:
        return new JetpackHealthCrate(&gJetpackAnimTable[kind], x, y, z);
    case JETPACK_KIND_QUESTION_CRATE_LIFE:
        if (CollectedSpawns::Contains(spawn))
            return new JetpackQuestionCrate(&gJetpackAnimTable[JETPACK_KIND_QUESTION_CRATE_WUMPA_1],
                                            x, y, z, spawn);
        /* fallthrough */
    case JETPACK_KIND_QUESTION_CRATE_WUMPA_1:
    case JETPACK_KIND_QUESTION_CRATE_WUMPA_3:
    case JETPACK_KIND_QUESTION_CRATE_WUMPA_5:
        return new JetpackQuestionCrate(&gJetpackAnimTable[kind], x, y, z, spawn);
    case JETPACK_KIND_TIME_CRATE_1:
    case JETPACK_KIND_TIME_CRATE_2:
    case JETPACK_KIND_TIME_CRATE_3:
    case JETPACK_KIND_STOPWATCH:
        return new JetpackTimeCrate(&gJetpackAnimTable[kind], x, y, z);
    case JETPACK_KIND_PARACHUTE_NITRO:
        return new JetpackParachuteNitro(&gJetpackAnimTable[kind], x, y, z);
    case JETPACK_KIND_ROCKET:
        return new JetpackRocket(&gJetpackAnimTable[kind], x, y, z);
    case JETPACK_KIND_RING_LEFT:
        new JetpackRing(&gJetpackAnimTable[JETPACK_KIND_RING_RIGHT],
                        x - gJetpackAnimTable[kind].spawnX +
                            gJetpackAnimTable[JETPACK_KIND_RING_RIGHT].spawnX,
                        y, z);
        return new JetpackRing(&gJetpackAnimTable[kind], x, y, z);
    }
    return 0;
}

/* Plays sfx 0x17 and spawns the checkpoint banner (record 46). */
void JetpackCheckpointText::Create()
{
    gAudioContext->PlaySfx(SFX_CHECKPOINT, 0x100);
    new JetpackCheckpointText(&gJetpackAnimTable[JETPACK_KIND_CHECKPOINT_TEXT], 0, 0, 0);
}

/* Plays sfx 4 and spawns an explosion (record 45) at (x, y, z). */
void JetpackExplosion::Create(s32 x, s32 y, s32 z)
{
    gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
    new JetpackExplosion(&gJetpackAnimTable[JETPACK_KIND_EXPLOSION], x, y, z);
}

/* Kind-44 constructor. */
void JetpackCollectedWumpa::Spawn(s32 a, s32 b, s32 c)
{
    new JetpackCollectedWumpa(&gJetpackAnimTable[JETPACK_KIND_COLLECTED_WUMPA], a, b, c);
}

/* A balloon of record `kind` holding crate `d` (the crates' constructors,
 * jetpack_crates.cpp). */
JetpackBalloon *JetpackBalloon::Spawn(u8 kind, s32 a, s32 b, s32 c, JetpackBalloonCrate *crate)
{
    return new JetpackBalloon(&gJetpackAnimTable[kind], a, b, c, crate);
}

/* Kind-14 constructor. */
void HovercraftCannonFlash::Spawn(s32 a, s32 b, s32 c)
{
    new HovercraftCannonFlash(&gJetpackAnimTable[JETPACK_KIND_HOVERCRAFT_CANNON_FLASH], a, b, c);
}

/* Kind-13 constructor. The side gun's `left` is a `bool`, which g++
 * passes as a byte on the stack (`add r2, sp, #4; strb`). */
void HovercraftSideGun::Spawn(s32 a, s32 b, s32 c, bool left)
{
    new HovercraftSideGun(&gJetpackAnimTable[JETPACK_KIND_HOVERCRAFT_SIDE_GUN], a, b, c, left);
}

/* Kind-12 constructor. */
void HovercraftLauncher::Spawn(s32 a, s32 b, s32 c)
{
    new HovercraftLauncher(&gJetpackAnimTable[JETPACK_KIND_HOVERCRAFT_LAUNCHER], a, b, c);
}

/* Kind-11 constructor. */
void HovercraftCannon::Spawn(s32 a, s32 b, s32 c)
{
    new HovercraftCannon(&gJetpackAnimTable[JETPACK_KIND_HOVERCRAFT_CANNON], a, b, c);
}

/* Plays sfx 0x38 and spawns a kind-39 object. */
void HovercraftFireball::Spawn(s32 x, s32 y, s32 z)
{
    gAudioContext->PlaySfx(SFX_FIREBALL_LAUNCH, 0x100);
    new HovercraftFireball(&gJetpackAnimTable[JETPACK_KIND_HOVERCRAFT_FIREBALL], x, y, z);
}

/* Plays sfx 0x38 and spawns an airship fireball (record 38). */
void AirshipFireball::Spawn(s32 x, s32 y, s32 z)
{
    gAudioContext->PlaySfx(SFX_FIREBALL_LAUNCH, 0x100);
    new AirshipFireball(&gJetpackAnimTable[JETPACK_KIND_AIRSHIP_FIREBALL], x, y, z);
}

/* Plays sfx 0x30 and fires a cannonball (record 3) at (d, e). */
void JetpackCannonball::Spawn(s32 a, s32 b, s32 c, s32 d, s32 e)
{
    gAudioContext->PlaySfx(SFX_CANNONBALL_FIRE, 0x100);
    new JetpackCannonball(&gJetpackAnimTable[JETPACK_KIND_CANNONBALL], a, b, c, d, e);
}

/* Spawns the player's shot (record 2; JetpackPlayer::StateFly). */
void JetpackShot::Spawn(s32 x, s32 y, s32 z, s32 velX, s32 velY)
{
    new JetpackShot(&gJetpackAnimTable[JETPACK_KIND_SHOT], x, y, z, velX, velY);
}

/* Installs the level's per-kind table and builds the player from its
 * first record, making it the (self-linked) actor list's root. */
void JetpackPlayer::Create(struct anim_table_record *table, s32 z)
{
    JetpackPlayer *p;

    gJetpackAnimTable = table;
    gActorList = p = new JetpackPlayer(gJetpackAnimTable, z);
    p->prev = p;
    p->next = p;
}
