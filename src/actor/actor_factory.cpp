#include "vehicle.hpp"

extern "C" {
#include "core.h"
#include "math_util.h"
#include "memory.h"
#include "actor_anim.h"
#include "actor.h"
#include "bosses.h"
#include "vehicle.h"
#include "level_state.h"
#include "globals.h"
}

/* 0x0802AC28-0x0802B364 (GitHub issue #51): the polar levels' actor
 * factory, category type 0's hooks.
 *
 * - ConstructAnimTableState (category vtable slot 0, see docs/rom_map.md)
 *   installs the category's animation table (`struct anim_table_record`,
 *   include/actor_anim.h) as gActorAnimTable and builds the player, the
 *   actor list's root: `new PolarPlayer`, whose constructor
 *   (ConstructActorPart) also resets the polar run's globals.
 * - SpawnActor (vtable slot 1) turns a level spawn record into a
 *   CreateActor call, picking the record's alternate kind in a time trial
 *   (with several kinds folded to 1) or its bonus kind when asked to,
 *   and skipping kinds 0/32-34/62.
 * - CreateActor is the per-kind `new`: the crates', the wumpa's and the
 *   riderless polar's constructors are inline (vehicle.hpp), the others'
 *   out of line (src/vehicle/polar_*.cpp); kinds 36-39 only select a
 *   palette-cycle preset (SetActorPaletteCycle).
 * - CreatePolarCheckpointText, SpawnPolarCollectedWumpa and
 *   SpawnPolarAkuAku build three fixed records (40, 11, 27).
 *
 * `new` is AnimPart's operator new (mem_alloc into the IWRAM heap,
 * inline), then the constructor with the arguments evaluated after the
 * allocation: the record's address is computed after mem_alloc returns,
 * as in the ROM. A few constructors take the record's `spawnX` for x
 * where the others take the moved x. */

ActorSelf *CreateActor(u8 kind, s32 x, s32 y, s32 z, void *spawn)
{
    x += gActorAnimTable[kind].spawnX;
    y += gActorAnimTable[kind].spawnY;

    switch (kind) {
    case 9:
        return new PolarAkuAkuCrate(gActorAnimTable + kind, x, y, z);
    case 3:
        return new PolarCheckpointCrate(gActorAnimTable + kind, gActorAnimTable[kind].spawnX, y, z);
    case 5:
    case 6:
    case 7:
        return new PolarTimeCrate(gActorAnimTable + kind, x, y, z);
    case 1:
        return new PolarBasicCrate(gActorAnimTable + kind, x, y, z);
    case 4:
        return new PolarNitroCrate(gActorAnimTable + kind, x, y, z);
    case 22:
        return new PolarLauncher(gActorAnimTable + kind, x, y, z);
    case 12:
        return new PolarBoostPad(gActorAnimTable + kind, x, y, z);
    case 24:
        return new PolarPenguin(gActorAnimTable + kind, x, y, z, (struct spawn_arg *)spawn);
    case 28:
    case 29:
    case 30:
    case 31:
        return new PolarQuestionCrate(gActorAnimTable + kind, x, y, z);
    case 35:
        if ((u8)IsSpawnCollected(spawn))
            return new PolarQuestionCrate(gActorAnimTable + 28, x, y, z);
        return new PolarLifeCrate(gActorAnimTable + kind, x, y, z, spawn);
    case 8:
        if ((u8)IsSpawnCollected(spawn))
            return new PolarQuestionCrate(gActorAnimTable + 28, x, y, z);
        return new PolarLifeCrate(gActorAnimTable + kind, x, y, z, spawn);
    case 10:
        return new PolarFourWumpaCrate(gActorAnimTable + kind, x, y, z);
    case 11:
        return new PolarWumpa(gActorAnimTable + kind, x, y, z);
    case 23:
        return new PolarElectricFence(gActorAnimTable + kind, x, y, z);
    case 16:
    case 18:
    case 20:
        return new PolarIcicle(gActorAnimTable + kind, x, y, z);
    case 25:
        /* The goal's second part (record 26), on its second animation. */
        (new PolarGoal(gActorAnimTable + 26, gActorAnimTable[26].spawnX, y, z))->RestartAnim(1);
        return new PolarGoal(gActorAnimTable + kind, gActorAnimTable[kind].spawnX, y, z);
    case 13:
        /* The obstacle's second part (record 14), the same way. */
        (new PolarObstacle(gActorAnimTable + 14, gActorAnimTable[14].spawnX, y, z))->RestartAnim(1);
        return new PolarObstacle(gActorAnimTable + kind, gActorAnimTable[kind].spawnX, y, z);
    case 2:
        return new RiderlessPolar(gActorAnimTable + kind, x, y, z);
    case 36:
    case 37:
    case 38:
    case 39:
        SetActorPaletteCycle(kind - 36);
        break;
    }
    return 0;
}

void CreatePolarCheckpointText(s32 x, s32 y, s32 z)
{
    new PolarCheckpointText(&gActorAnimTable[40], x, y, z);
}

void SpawnPolarCollectedWumpa(s32 x, s32 y, s32 count)
{
    new PolarCollectedWumpa(&gActorAnimTable[11], x, y, count);
}

PolarAkuAku *SpawnPolarAkuAku(s32 x, s32 y, s32 z, s32 arg)
{
    return new PolarAkuAku(&gActorAnimTable[27], x, y, z, arg);
}

void ConstructAnimTableState(struct anim_table_record *table, s32 z)
{
    gActorAnimTable = table;
    gActorList = 0;
    gActorList = new PolarPlayer(gActorAnimTable, z);
}

struct actor_spawn {
    u8 kind;
    u8 altKind;
    u8 bonusKind;
    u8 unk_03;
    s32 x;
    s32 y;
    s32 z;
};

ActorSelf *SpawnActor(struct actor_spawn *spawn, u8 useBonus, s32 zOffset)
{
    u8 kind = spawn->kind;

    if (gLevelState->timeTrial != 0) {
        kind = spawn->altKind;
        if (kind == 11)
            return 0;
        if (kind == 3 || kind == 8 || kind == 28 || kind == 29 || kind == 30 || kind == 31 ||
            kind == 35)
            kind = 1;
    } else if (useBonus) {
        kind = spawn->bonusKind;
    }
    if (kind == 0 || kind == 32 || kind == 33 || kind == 34 || kind == 62)
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
