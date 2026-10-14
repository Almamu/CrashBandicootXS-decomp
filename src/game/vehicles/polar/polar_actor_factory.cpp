#include "game/vehicles/vehicle.hpp"
#include "game/level/level_state.hpp"
#include "engine/actor/actor_palette_cycle.hpp"
#include "engine/actor/collected_spawns.hpp"

extern "C" {
#include "core.h"
#include "engine/util/math_util.h"
#include "engine/sys/memory.h"
#include "engine/actor/actor_anim.h"
#include "engine/actor/actor.h"
#include "game/bosses/bosses.h"
#include "globals.h"
}

using Engine::Actor::ActorPaletteCycle;
using Engine::Actor::CollectedSpawns;
using Game::Entities::ActorSelf;
using Game::Vehicles::Polar::PolarAkuAku;
using Game::Vehicles::Polar::PolarAkuAkuCrate;
using Game::Vehicles::Polar::PolarBasicCrate;
using Game::Vehicles::Polar::PolarBoostPad;
using Game::Vehicles::Polar::PolarCheckpointCrate;
using Game::Vehicles::Polar::PolarCheckpointText;
using Game::Vehicles::Polar::PolarCollectedWumpa;
using Game::Vehicles::Polar::PolarElectricFence;
using Game::Vehicles::Polar::PolarFourWumpaCrate;
using Game::Vehicles::Polar::PolarGoal;
using Game::Vehicles::Polar::PolarIcicle;
using Game::Vehicles::Polar::PolarLauncher;
using Game::Vehicles::Polar::PolarLifeCrate;
using Game::Vehicles::Polar::PolarNitroCrate;
using Game::Vehicles::Polar::PolarObstacle;
using Game::Vehicles::Polar::PolarPenguin;
using Game::Vehicles::Polar::PolarPlayer;
using Game::Vehicles::Polar::PolarQuestionCrate;
using Game::Vehicles::Polar::PolarTimeCrate;
using Game::Vehicles::Polar::PolarWumpa;
using Game::Vehicles::Polar::RiderlessPolar;

/* 0x0802AC28-0x0802B364 (GitHub issue #51): the polar levels' actor
 * factory, category type 0's hooks.
 *
 * - ConstructAnimTableState (category vtable slot 0, see docs/rom_map.md)
 *   installs the category's animation table (`struct anim_table_record`,
 *   include/engine/actor/actor_anim.h) as gActorAnimTable and builds the player, the
 *   actor list's root: `new PolarPlayer`, whose constructor
 *   (ConstructActorPart) also resets the polar run's globals.
 * - SpawnActor (vtable slot 1) turns a level spawn record into a
 *   CreateActor call, picking the record's alternate kind in a time trial
 *   (with several kinds folded to 1) or its bonus kind when asked to,
 *   and skipping kinds 0/32-34/62.
 * - CreateActor is the per-kind `new`: the crates', the wumpa's and the
 *   riderless polar's constructors are inline (vehicle.hpp), the others'
 *   out of line (src/vehicle/polar/polar_*.cpp); kinds 36-39 only select a
 *   palette-cycle preset (ActorPaletteCycle::Set).
 * - CreatePolarCheckpointText, SpawnPolarCollectedWumpa and
 *   SpawnPolarAkuAku build three fixed records (40, 11, 27).
 *
 * Since #915 they are static members: ConstructAnimTableState, SpawnActor
 * and CreateActor of PolarPlayer (Create, SpawnActor, CreateActor), the
 * other three of the class each builds (PolarCheckpointText::Create,
 * PolarCollectedWumpa::Spawn, PolarAkuAku::Spawn).
 *
 * `new` is AnimPart's operator new (mem_alloc into the IWRAM heap,
 * inline), then the constructor with the arguments evaluated after the
 * allocation: the record's address is computed after mem_alloc returns,
 * as in the ROM. A few constructors take the record's `spawnX` for x
 * where the others take the moved x. */

ActorSelf *PolarPlayer::CreateActor(u8 kind, s32 x, s32 y, s32 z, struct actor_spawn *spawn)
{
    x += gActorAnimTable[kind].spawnX;
    y += gActorAnimTable[kind].spawnY;

    switch (kind) {
    case POLAR_KIND_AKU_AKU_CRATE:
        return new PolarAkuAkuCrate(gActorAnimTable + kind, x, y, z);
    case POLAR_KIND_CHECKPOINT_CRATE:
        return new PolarCheckpointCrate(gActorAnimTable + kind, gActorAnimTable[kind].spawnX, y, z);
    case POLAR_KIND_TIME_CRATE_1:
    case POLAR_KIND_TIME_CRATE_2:
    case POLAR_KIND_TIME_CRATE_3:
        return new PolarTimeCrate(gActorAnimTable + kind, x, y, z);
    case POLAR_KIND_BASIC_CRATE:
        return new PolarBasicCrate(gActorAnimTable + kind, x, y, z);
    case POLAR_KIND_NITRO_CRATE:
        return new PolarNitroCrate(gActorAnimTable + kind, x, y, z);
    case POLAR_KIND_LAUNCHER:
        return new PolarLauncher(gActorAnimTable + kind, x, y, z);
    case POLAR_KIND_BOOST_PAD:
        return new PolarBoostPad(gActorAnimTable + kind, x, y, z);
    case POLAR_KIND_PENGUIN:
        return new PolarPenguin(gActorAnimTable + kind, x, y, z, (struct spawn_arg *)spawn);
    case POLAR_KIND_QUESTION_CRATE_WUMPA_1:
    case POLAR_KIND_QUESTION_CRATE_WUMPA_3:
    case POLAR_KIND_QUESTION_CRATE_WUMPA_5:
    case POLAR_KIND_QUESTION_CRATE_AKU_AKU:
        return new PolarQuestionCrate(gActorAnimTable + kind, x, y, z);
    case POLAR_KIND_QUESTION_CRATE_LIFE:
        if (CollectedSpawns::Contains(spawn))
            return new PolarQuestionCrate(gActorAnimTable + POLAR_KIND_QUESTION_CRATE_WUMPA_1, x, y,
                                          z);
        return new PolarLifeCrate(gActorAnimTable + kind, x, y, z, spawn);
    case POLAR_KIND_LIFE_CRATE:
        /* not merged with the case above: the ROM has its own copy of this body */
        if (CollectedSpawns::Contains(spawn))
            return new PolarQuestionCrate(gActorAnimTable + POLAR_KIND_QUESTION_CRATE_WUMPA_1, x, y,
                                          z);
        return new PolarLifeCrate(gActorAnimTable + kind, x, y, z, spawn);
    case POLAR_KIND_FOUR_WUMPA_CRATE:
        return new PolarFourWumpaCrate(gActorAnimTable + kind, x, y, z);
    case POLAR_KIND_WUMPA:
        return new PolarWumpa(gActorAnimTable + kind, x, y, z);
    case POLAR_KIND_ELECTRIC_FENCE:
        return new PolarElectricFence(gActorAnimTable + kind, x, y, z);
    case POLAR_KIND_ICICLE_A:
    case POLAR_KIND_ICICLE_B:
    case POLAR_KIND_ICICLE_C:
        return new PolarIcicle(gActorAnimTable + kind, x, y, z);
    case POLAR_KIND_GOAL:
        /* The goal's second part (record 26), on its second animation. */
        (new PolarGoal(gActorAnimTable + POLAR_KIND_GOAL_2,
                       gActorAnimTable[POLAR_KIND_GOAL_2].spawnX, y, z))
            ->RestartAnim(1);
        return new PolarGoal(gActorAnimTable + kind, gActorAnimTable[kind].spawnX, y, z);
    case POLAR_KIND_OBSTACLE:
        /* The obstacle's second part (record 14), the same way. */
        (new PolarObstacle(gActorAnimTable + POLAR_KIND_OBSTACLE_2,
                           gActorAnimTable[POLAR_KIND_OBSTACLE_2].spawnX, y, z))
            ->RestartAnim(1);
        return new PolarObstacle(gActorAnimTable + kind, gActorAnimTable[kind].spawnX, y, z);
    case POLAR_KIND_RIDERLESS_POLAR:
        return new RiderlessPolar(gActorAnimTable + kind, x, y, z);
    case POLAR_KIND_PALETTE_CYCLE_0:
    case POLAR_KIND_PALETTE_CYCLE_1:
    case POLAR_KIND_PALETTE_CYCLE_2:
    case POLAR_KIND_PALETTE_CYCLE_3:
        ActorPaletteCycle::Set(kind - POLAR_KIND_PALETTE_CYCLE_0);
        break;
    }
    return 0;
}

void PolarCheckpointText::Create(s32 x, s32 y, s32 z)
{
    new PolarCheckpointText(&gActorAnimTable[POLAR_KIND_CHECKPOINT_TEXT], x, y, z);
}

void PolarCollectedWumpa::Spawn(s32 x, s32 y, s32 count)
{
    new PolarCollectedWumpa(&gActorAnimTable[POLAR_KIND_WUMPA], x, y, count);
}

PolarAkuAku *PolarAkuAku::Spawn(s32 x, s32 y, s32 z, s32 level)
{
    return new PolarAkuAku(&gActorAnimTable[POLAR_KIND_AKU_AKU], x, y, z, level);
}

void PolarPlayer::Create(struct anim_table_record *table, s32 z)
{
    gActorAnimTable = table;
    gActorList = 0;
    gActorList = new PolarPlayer(gActorAnimTable, z);
}

ActorSelf *PolarPlayer::SpawnActor(struct actor_spawn *spawn, u8 useBonus, s32 zOffset)
{
    u8 kind = spawn->kind;

    if (gLevelState->timeTrial != 0) {
        kind = spawn->altKind;
        if (kind == POLAR_KIND_WUMPA)
            return 0;
        if (kind == POLAR_KIND_CHECKPOINT_CRATE || kind == POLAR_KIND_LIFE_CRATE ||
            kind == POLAR_KIND_QUESTION_CRATE_WUMPA_1 ||
            kind == POLAR_KIND_QUESTION_CRATE_WUMPA_3 ||
            kind == POLAR_KIND_QUESTION_CRATE_WUMPA_5 ||
            kind == POLAR_KIND_QUESTION_CRATE_AKU_AKU || kind == POLAR_KIND_QUESTION_CRATE_LIFE)
            kind = POLAR_KIND_BASIC_CRATE;
    } else if (useBonus) {
        kind = spawn->bonusKind;
    }
    if (kind == POLAR_KIND_PLAYER || kind == POLAR_KIND_PATH_SLOW ||
        kind == POLAR_KIND_PATH_MEDIUM || kind == POLAR_KIND_PATH_FAST || kind == POLAR_KIND_UNK62)
        return 0;
    return CreateActor(kind, INT_TO_Q8(spawn->x), INT_TO_Q8(spawn->y),
                       INT_TO_Q8(spawn->z) + zOffset, spawn);
}

/* Mounted (z != 0: the run starts on the bear) or standing beside it. */
PolarPlayer::PolarPlayer(const struct anim_table_record *rec, s32 z)
    : ActorSelf(rec, 0, z != 0 ? 0x2800 : -0x5000, z)
{
    AllocTiles();
    if (this->z != 0)
        SetState(0xD, 0xC);
    else
        RestartAnim(8);
    gPolarPlayerVelY = 0;
    gRiderlessPolar = 0;
    gPolarAkuAku = 0;
    gPolarSteerEnabled = 0;
    gPolarInvulnTimer = 0;
    gPolarSteerTime = 0;
    gPolarFinishTimer = 0;
    gPolarPlayerInactive = 1;
    gPolarFadeStarted = 0;
    gPolarPlayerHalted = 0;
    gPolarQueuedWumpa = 0;
    gPolarWumpaDispenseTimer = 0;
    gPolarPauseLocked = 0;
}
