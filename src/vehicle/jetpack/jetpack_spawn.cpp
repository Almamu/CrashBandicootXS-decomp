#include "vehicle.hpp"
#include "boss_actors.hpp"
#include "hovercraft.hpp"
#include "airship.hpp"
#include "audio.hpp"
#include "level_state.hpp"
#include "yeti.hpp"

extern "C" {
#include "math_util.h"
#include "match.h"
#include "system.h"
#include "actor.h"
#include "bosses.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
}

/* The jetpack levels' spawners (#664 part 11e, include/vehicle.hpp), ROM
 * 0x0802E0A4-0x0802E740, between yeti.cpp and jetpack_player_update.cpp:
 * the level's spawn dispatcher `CreateJetpackActor` (a 31-case `switch`
 * over the spawn "kind", indexing the per-kind record table
 * `gJetpackAnimTable`) and its helpers: `SpawnJetpackActor` picks a spawn
 * record's kind byte and forwards to it, the run of small spawners
 * (`CreateJetpackCheckpointText`-`SpawnJetpackShot`) each build one object
 * from a fixed record of the same table, and `CreateJetpackPlayer` builds
 * the player. It starts with the yeti's `Yeti::StateStop` (YetiStateStop,
 * include/yeti.hpp), which stays here across the flag boundary with
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
ActorSelf *SpawnJetpackActor(struct actor_spawn *rec, u8 alt, s32 dz)
{
    u8 kind = rec->kind;
    s32 x, y, z;

    if (gLevelState->timeTrial != 0) {
        kind = rec->altKind;
        if (kind == 0x17)
            kind = 0x14;
    } else if (alt != 0) {
        kind = rec->bonusKind;
    }
    if (kind == 0x1d && !(u8)gLevelState->IsCrystalSaved())
        return 0;
    if (kind == 0 || kind == 0x3e || (u8)(kind - 0x20) <= 5)
        return 0;
    x = INT_TO_Q8(rec->x);
    y = INT_TO_Q8(rec->y);
    z = INT_TO_Q8(rec->z) + dz;
    if ((u8)(kind - 0x10) <= 2) {
        Airship::Spawn(kind - 0x10, x, y, z);
    } else if (kind != 0xa) {
        return CreateJetpackActor(kind, x, y, z, rec);
    } else {
        Hovercraft::Spawn(0, x, y, z);
    }
    return 0;
}

/* The spawn dispatcher: offsets the position by the kind's record and
 * constructs the kind's object. Kind 23 turns into kind 20's object
 * when `IsSpawnCollected` says so; kind 31 spawns a kind-43 companion first. */
ActorSelf *CreateJetpackActor(u8 kind, s32 x, s32 y, s32 z, struct actor_spawn *spawn)
{
    x += gJetpackAnimTable[kind].spawnX;
    y += gJetpackAnimTable[kind].spawnY;
    switch (kind) {
    case 1:
        return new JetpackPlane(&gJetpackAnimTable[kind], x, y, z, (struct spawn_arg *)spawn);
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        return new JetpackBomber(&gJetpackAnimTable[kind], x, y, z);
    case 19:
        return new JetpackHealthCrate(&gJetpackAnimTable[kind], x, y, z);
    case 23:
        if ((u8)IsSpawnCollected(spawn))
            return new JetpackQuestionCrate(&gJetpackAnimTable[20], x, y, z, spawn);
        /* fallthrough */
    case 20:
    case 21:
    case 22:
        return new JetpackQuestionCrate(&gJetpackAnimTable[kind], x, y, z, spawn);
    case 24:
    case 25:
    case 26:
    case 29:
        return new JetpackTimeCrate(&gJetpackAnimTable[kind], x, y, z);
    case 27:
        return new JetpackParachuteNitro(&gJetpackAnimTable[kind], x, y, z);
    case 28:
        return new JetpackRocket(&gJetpackAnimTable[kind], x, y, z);
    case 31:
        new JetpackRing(&gJetpackAnimTable[43],
                        x - gJetpackAnimTable[kind].spawnX + gJetpackAnimTable[43].spawnX, y, z);
        return new JetpackRing(&gJetpackAnimTable[kind], x, y, z);
    }
    return 0;
}

/* Plays sfx 0x17 and spawns the checkpoint banner (record 46). */
void CreateJetpackCheckpointText(void)
{
    gAudioContext->PlaySfx(SFX_CHECKPOINT, 0x100);
    new JetpackCheckpointText(&gJetpackAnimTable[46], 0, 0, 0);
}

/* Plays sfx 4 and spawns an explosion (record 45) at (x, y, z). */
void CreateJetpackExplosion(s32 x, s32 y, s32 z)
{
    gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
    new JetpackExplosion(&gJetpackAnimTable[45], x, y, z);
}

/* Kind-44 constructor. */
void SpawnJetpackCollectedWumpa(s32 a, s32 b, s32 c)
{
    new JetpackCollectedWumpa(&gJetpackAnimTable[44], a, b, c);
}

/* A balloon of record `kind` holding crate `d` (the crates' constructors,
 * jetpack_crates.cpp). */
void *SpawnJetpackBalloon(u8 kind, s32 a, s32 b, s32 c, s32 d)
{
    return new JetpackBalloon(&gJetpackAnimTable[kind], a, b, c, (JetpackBalloonCrate *)d);
}

/* Kind-14 constructor. */
void SpawnHovercraftCannonFlash(s32 a, s32 b, s32 c)
{
    new HovercraftCannonFlash(&gJetpackAnimTable[14], a, b, c);
}

/* Kind-13 constructor. The side gun's `left` is a `bool`, which g++
 * passes as a byte on the stack (`add r2, sp, #4; strb`). */
void SpawnHovercraftSideGun(s32 a, s32 b, s32 c, bool left)
{
    new HovercraftSideGun(&gJetpackAnimTable[13], a, b, c, left);
}

/* Kind-12 constructor. */
void SpawnHovercraftLauncher(s32 a, s32 b, s32 c)
{
    new HovercraftLauncher(&gJetpackAnimTable[12], a, b, c);
}

/* Kind-11 constructor. */
void SpawnHovercraftCannon(s32 a, s32 b, s32 c)
{
    new HovercraftCannon(&gJetpackAnimTable[11], a, b, c);
}

/* Plays sfx 0x38 and spawns a kind-39 object. */
void SpawnHovercraftFireball(s32 x, s32 y, s32 z)
{
    gAudioContext->PlaySfx(SFX_FIREBALL_LAUNCH, 0x100);
    new HovercraftFireball(&gJetpackAnimTable[39], x, y, z);
}

/* Plays sfx 0x38 and spawns an airship fireball (record 38). */
void SpawnAirshipFireball(s32 x, s32 y, s32 z)
{
    gAudioContext->PlaySfx(SFX_FIREBALL_LAUNCH, 0x100);
    new AirshipFireball(&gJetpackAnimTable[38], x, y, z);
}

/* Plays sfx 0x30 and fires a cannonball (record 3) at (d, e). */
void SpawnJetpackCannonball(s32 a, s32 b, s32 c, s32 d, s32 e)
{
    gAudioContext->PlaySfx(SFX_CANNONBALL_FIRE, 0x100);
    new JetpackCannonball(&gJetpackAnimTable[3], a, b, c, d, e);
}

/* Spawns the player's shot (record 2; JetpackPlayer::StateFly). */
void SpawnJetpackShot(s32 x, s32 y, s32 z, s32 velX, s32 velY)
{
    new JetpackShot(&gJetpackAnimTable[2], x, y, z, velX, velY);
}

/* Installs the level's per-kind table and builds the player from its
 * first record, making it the (self-linked) actor list's root. */
void CreateJetpackPlayer(struct anim_table_record *table, s32 z)
{
    JetpackPlayer *p;

    gJetpackAnimTable = table;
    gActorList = p = new JetpackPlayer(gJetpackAnimTable, z);
    p->prev = p;
    p->next = p;
}
