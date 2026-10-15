#include "game/crates/crate.hpp"
#include "game/objects/platform.hpp"
#include "game/ui/level_select.hpp"
#include "game/enemies/enemy_ctrl.hpp"
#include "game/bosses/boss_ctrl.hpp"
#include "game/objects/pickups.hpp"

extern "C" {
#include "core.h"
#include "game/level/level.h"
#include "globals.h"
}

using Game::Enemies::EnemyCtrl;
using Game::Enemies::PeriodicSpawner;
using Game::Bosses::MegaMixCtrl;
using Game::Bosses::TinyCtrl;
using Game::Bosses::CortexBossCtrl;
using Game::Bosses::DingodileCtrl;
using Game::Crates::Crate;
using Game::Objects::Platform;
using Game::Objects::LaunchPad;
using Game::Objects::Pickups;
using Game::Objects::Wumpa;
using Game::Objects::Stopwatch;

/*
 * ROM 0x0816C6A4-0x0816C814. Linked in ROM order between games/xs/data/data.s
 * sections by games/xs/ldscript.txt - see docs/data.md.
 */

/* The unified 92-slot function-pointer dispatch array (docs/rom_map.md
 * "Major correction: there is no second table"): the spawn function of
 * each entity type of the room data (docs/levels.md, "Entities").
 * EntitySpawner::Create (spawn_markers.cpp) hands it to SetEntitySpawnerTable
 * with a count of ENTITY_COUNT, as EntitySpawner::instance; SpawnEntity calls entry
 * `type` with the entity's id, x, y and param. */
const entity_spawn_fn gEntitySpawnFuncs[ENTITY_COUNT] = {
    /* ENTITY_START_MARKER */ SpawnStartMarker,
    /* ENTITY_PLAYER_POSITION */ SpawnPlayerPosition,
    /* ENTITY_UNDERWATER_START_MARKER */ SpawnUnderwaterStartMarker,
    /* ENTITY_UNDERWATER_PLAYER_POSITION */ SpawnUnderwaterPlayerPosition,
    /* ENTITY_HOVER_START_MARKER */ SpawnHoverStartMarker,
    /* ENTITY_HOVER_PLAYER_POSITION */ (entity_spawn_fn)SpawnHoverPlayerPosition,
    /* ENTITY_WUMPA */ Wumpa::Spawn,
    /* ENTITY_CRYSTAL */ Pickups::SpawnCrystal,
    /* ENTITY_GEM_PATH_GEM */ Pickups::SpawnGemPathGem,
    /* ENTITY_BLUE_GEM */ Pickups::SpawnBlueGem,
    /* ENTITY_RED_GEM */ Pickups::SpawnRedGem,
    /* ENTITY_GREEN_GEM */ Pickups::SpawnGreenGem,
    /* ENTITY_YELLOW_GEM */ Pickups::SpawnYellowGem,
    /* 0x0D */ (entity_spawn_fn)SpawnNoEntity,
    /* 0x0E */ (entity_spawn_fn)SpawnNoEntity,
    /* 0x0F */ (entity_spawn_fn)SpawnNoEntity,
    /* ENTITY_STOPWATCH */ Stopwatch::Spawn,
    /* 0x11 */ (entity_spawn_fn)SpawnNoEntity,
    /* ENTITY_TURBO_RUN_POWER */ Pickups::SpawnPowerTurboRun,
    /* ENTITY_DOUBLE_JUMP_POWER */ Pickups::SpawnPowerDoubleJump,
    /* ENTITY_BODY_SLAM_POWER */ Pickups::SpawnPowerBodySlam,
    /* ENTITY_BASIC_CRATE */ Crate::SpawnBasic,
    /* ENTITY_CHECKPOINT_CRATE */ Crate::SpawnCheckpoint,
    /* ENTITY_AKU_AKU_CRATE */ Crate::SpawnAkuAku,
    /* ENTITY_IRON_SWITCH_CRATE */ Crate::SpawnIronSwitch,
    /* ENTITY_ARROW_CRATE */ Crate::SpawnArrow,
    /* ENTITY_OUTLINE_CRATE */ Crate::SpawnOutline,
    /* ENTITY_NITRO_SWITCH_CRATE */ Crate::SpawnNitroSwitch,
    /* ENTITY_IRON_CRATE */ Crate::SpawnIron,
    /* ENTITY_IRON_ARROW_CRATE */ Crate::SpawnIronArrow,
    /* ENTITY_LIFE_CRATE */ Crate::SpawnLife,
    /* ENTITY_NITRO_CRATE */ Crate::SpawnNitro,
    /* ENTITY_MYSTERY_CRATE */ Crate::SpawnMystery,
    /* ENTITY_BOUNCY_WUMPA_CRATE */ Crate::SpawnBouncyWumpa,
    /* ENTITY_REINFORCED_CRATE */ Crate::SpawnReinforced,
    /* ENTITY_TNT_CRATE */ Crate::SpawnTnt,
    /* ENTITY_SLOT_CRATE */ Crate::SpawnSlot,
    /* ENTITY_TIME_CRATE_1 */ Crate::SpawnTime1,
    /* ENTITY_TIME_CRATE_2 */ Crate::SpawnTime2,
    /* ENTITY_TIME_CRATE_3 */ Crate::SpawnTime3,
    /* ENTITY_LIZARD */ EnemyCtrl::SpawnLizard,
    /* ENTITY_VULTURE */ EnemyCtrl::SpawnVulture,
    /* ENTITY_VENUS_FLYTRAP */ EnemyCtrl::SpawnVenusFlytrap,
    /* ENTITY_PATROLLING_JUNGLE_ENEMY */ EnemyCtrl::SpawnPatrollingJungleEnemy,
    /* ENTITY_BLOWGUN_TRIBESMAN */ EnemyCtrl::SpawnBlowgunTribesman,
    /* ENTITY_PENGUIN */ EnemyCtrl::SpawnPenguin,
    /* ENTITY_SEAL_SPAWNER */ PeriodicSpawner::Spawn,
    /* ENTITY_POLAR_BEAR */ EnemyCtrl::SpawnPolarBear,
    /* ENTITY_PUFFERFISH */ EnemyCtrl::SpawnPufferfish,
    /* ENTITY_SHARK */ EnemyCtrl::SpawnShark,
    /* ENTITY_MORAY_EEL */ EnemyCtrl::SpawnMorayEel,
    /* ENTITY_ELECTRIC_EEL */ EnemyCtrl::SpawnElectricEel,
    /* ENTITY_SQUID */ EnemyCtrl::SpawnSquid,
    /* ENTITY_JELLYFISH */ EnemyCtrl::SpawnJellyfish,
    /* 0x36 */ (entity_spawn_fn)SpawnNoEntity,
    /* ENTITY_LASER_BARRIER */ EnemyCtrl::SpawnLaserBarrier,
    /* ENTITY_STATIONARY_SPACE_ENEMY */ EnemyCtrl::SpawnStationarySpaceEnemy,
    /* ENTITY_PATROLLING_SPACE_ENEMY */ EnemyCtrl::SpawnPatrollingSpaceEnemy,
    /* ENTITY_SAUCER_LAB_ASSISTANT */ EnemyCtrl::SpawnSaucerLabAssistant,
    /* ENTITY_PISTON_CRUSHER */ EnemyCtrl::SpawnPistonCrusher,
    /* 0x3C */ (entity_spawn_fn)SpawnNoEntity,
    /* ENTITY_LAUNCH_PAD */ LaunchPad::SpawnEntity,
    /* 0x3E */ (entity_spawn_fn)SpawnNoEntity,
    /* ENTITY_SAUCER_LAB_ASSISTANT_B */ EnemyCtrl::SpawnSaucerLabAssistant,
    /* ENTITY_FLAMETHROWER_LAB_ASSISTANT */ EnemyCtrl::SpawnFlamethrowerLabAssistant,
    /* ENTITY_HOMING_SEWER_ENEMY */ EnemyCtrl::SpawnHomingSewerEnemy,
    /* ENTITY_PATROLLING_SEWER_ENEMY */ EnemyCtrl::SpawnPatrollingSewerEnemy,
    /* ENTITY_RAT */ EnemyCtrl::SpawnRat,
    /* ENTITY_FROG */ EnemyCtrl::SpawnFrog,
    /* ENTITY_DINGODILE */ DingodileCtrl::Spawn,
    /* 0x46 */ (entity_spawn_fn)SpawnNoEntity,
    /* ENTITY_TINY */ TinyCtrl::Spawn,
    /* ENTITY_CORTEX_BOSS */ CortexBossCtrl::Spawn,
    /* ENTITY_MEGA_MIX */ MegaMixCtrl::Spawn,
    /* ENTITY_TORNADO_SPIN_POWER */ Pickups::SpawnPowerTornadoSpin,
    /* ENTITY_SEA_MINE */ EnemyCtrl::SpawnSeaMine,
    /* ENTITY_SEA_MINE_B */ EnemyCtrl::SpawnSeaMine,
    /* ENTITY_WOODEN_CRUSHER */ EnemyCtrl::SpawnWoodenCrusher,
    /* ENTITY_LARGE_PLATFORM */ Platform::SpawnLarge,
    /* ENTITY_SMALL_PLATFORM */ Platform::SpawnSmall,
    /* ENTITY_MEDIUM_PLATFORM */ Platform::SpawnMedium,
    /* ENTITY_RED_GEM_PLATFORM */ Platform::SpawnGemRed,
    /* ENTITY_YELLOW_GEM_PLATFORM */ Platform::SpawnGemYellow,
    /* ENTITY_GREEN_GEM_PLATFORM */ Platform::SpawnGemGreen,
    /* ENTITY_BLUE_GEM_PLATFORM */ Platform::SpawnGemBlue,
    /* ENTITY_ROOM_EXIT */ SpawnRoomExit,
    /* ENTITY_BONUS_PLATFORM */ Platform::SpawnBonus,
    /* ENTITY_FLIP_PLATFORM */ Platform::SpawnFlip,
    /* ENTITY_ROCK_PLATFORM */ Platform::SpawnRock,
    /* ENTITY_CRATE_GEM_MARKER */ (entity_spawn_fn)SpawnCrateGemMarker,
    /* ENTITY_FLAME */ Pickups::SpawnFlame,
    /* ENTITY_SEAWEED */ Pickups::SpawnSeaweed,
};
