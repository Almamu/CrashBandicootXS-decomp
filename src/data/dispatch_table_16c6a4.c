#include "core.h"

/*
 * ROM 0x0816C6A4-0x0816C814. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern void nullsub_21();
extern void nullsub_22();
extern void SpawnStartMarker();
extern void SpawnCrystal();
extern void SpawnGemPathGem();
extern void SpawnRedGem();
extern void SpawnGreenGem();
extern void SpawnYellowGem();
extern void SpawnLizard();
extern void SpawnVulture();
extern void SpawnVenusFlytrap();
extern void SpawnPatrollingJungleEnemy();
extern void SpawnBlowgunTribesman();
extern void SpawnPenguin();
extern void SpawnPolarBear();
extern void SpawnPufferfish();
extern void SpawnShark();
extern void SpawnMorayEel();
extern void SpawnElectricEel();
extern void SpawnSquid();
extern void SpawnJellyfish();
extern void SpawnLaserBarrier();
extern void SpawnStationarySpaceEnemy();
extern void SpawnPatrollingSpaceEnemy();
extern void SpawnSaucerLabAssistant();
extern void SpawnPistonCrusher();
extern void SpawnFlamethrowerLabAssistant();
extern void SpawnHomingSewerEnemy();
extern void SpawnPatrollingSewerEnemy();
extern void SpawnRat();
extern void SpawnFrog();
extern void SpawnSeaMine();
extern void SpawnWoodenCrusher();
extern void SpawnRedGemPlatform();
extern void SpawnYellowGemPlatform();
extern void SpawnGreenGemPlatform();
extern void SpawnBlueGemPlatform();
extern void SpawnRoomExit();
extern void SpawnDingodile();
extern void SpawnTiny();
extern void SpawnCortexBoss();
extern void SpawnMegaMix();
extern void SpawnSeaweed();
extern void SpawnFlame();
extern void SpawnRockPlatform();
extern void SpawnFlipPlatform();
extern void SpawnBonusPlatform();
extern void SpawnMediumPlatform();
extern void SpawnSmallPlatform();
extern void SpawnLargePlatform();
extern void SpawnLaunchPadEntity();
extern void SpawnSealSpawner();
extern void SpawnTimeCrate3();
extern void SpawnTimeCrate2();
extern void SpawnTimeCrate1();
extern void SpawnSlotCrate();
extern void SpawnTntCrate();
extern void sub_8021B00();
extern void SpawnBouncyWumpaCrate();
extern void SpawnMysteryCrate();
extern void SpawnNitroCrate();
extern void SpawnLifeCrate();
extern void SpawnIronArrowCrate();
extern void SpawnIronCrate();
extern void SpawnNitroSwitchCrate();
extern void SpawnOutlineCrate();
extern void SpawnArrowCrate();
extern void SpawnIronSwitchCrate();
extern void SpawnAkuAkuCrate();
extern void SpawnCheckpointCrate();
extern void SpawnBasicCrate();
extern void SpawnBodySlamPower();
extern void SpawnTornadoSpinPower();
extern void SpawnDoubleJumpPower();
extern void SpawnTurboRunPower();
extern void SpawnStopwatch();
extern void SpawnBlueGem();
extern void SpawnCrateGemMarker();
extern void SpawnWumpa();
extern void SpawnHoverStartMarker();
extern void sub_80221A4();
extern void SpawnUnderwaterStartMarker();
extern void sub_80221D4();

/* The unified 92-slot function-pointer dispatch array (docs/rom_map.md
 * "Major correction: there is no second table"): the spawn function of
 * each entity type of the room data (docs/levels.md, "Entities").
 * CreateEntitySpawner (graphics_loading_21d80.c) hands it to SetEntitySpawnerTable
 * with a count of 0x5c, as gEntitySpawner; SpawnEntity calls entry
 * `type` with the entity's id, x, y and param. */
void (*const gEntitySpawnFuncs[92])() = {
    SpawnStartMarker,
    sub_80221D4,
    SpawnUnderwaterStartMarker,
    sub_80221A4,
    SpawnHoverStartMarker,
    nullsub_22,
    SpawnWumpa,
    SpawnCrystal,
    SpawnGemPathGem,
    SpawnBlueGem,
    SpawnRedGem,
    SpawnGreenGem,
    SpawnYellowGem,
    nullsub_21,
    nullsub_21,
    nullsub_21,
    SpawnStopwatch,
    nullsub_21,
    SpawnTurboRunPower,
    SpawnDoubleJumpPower,
    SpawnBodySlamPower,
    SpawnBasicCrate,
    SpawnCheckpointCrate,
    SpawnAkuAkuCrate,
    SpawnIronSwitchCrate,
    SpawnArrowCrate,
    SpawnOutlineCrate,
    SpawnNitroSwitchCrate,
    SpawnIronCrate,
    SpawnIronArrowCrate,
    SpawnLifeCrate,
    SpawnNitroCrate,
    SpawnMysteryCrate,
    SpawnBouncyWumpaCrate,
    sub_8021B00,
    SpawnTntCrate,
    SpawnSlotCrate,
    SpawnTimeCrate1,
    SpawnTimeCrate2,
    SpawnTimeCrate3,
    SpawnLizard,
    SpawnVulture,
    SpawnVenusFlytrap,
    SpawnPatrollingJungleEnemy,
    SpawnBlowgunTribesman,
    SpawnPenguin,
    SpawnSealSpawner,
    SpawnPolarBear,
    SpawnPufferfish,
    SpawnShark,
    SpawnMorayEel,
    SpawnElectricEel,
    SpawnSquid,
    SpawnJellyfish,
    nullsub_21,
    SpawnLaserBarrier,
    SpawnStationarySpaceEnemy,
    SpawnPatrollingSpaceEnemy,
    SpawnSaucerLabAssistant,
    SpawnPistonCrusher,
    nullsub_21,
    SpawnLaunchPadEntity,
    nullsub_21,
    SpawnSaucerLabAssistant,
    SpawnFlamethrowerLabAssistant,
    SpawnHomingSewerEnemy,
    SpawnPatrollingSewerEnemy,
    SpawnRat,
    SpawnFrog,
    SpawnDingodile,
    nullsub_21,
    SpawnTiny,
    SpawnCortexBoss,
    SpawnMegaMix,
    SpawnTornadoSpinPower,
    SpawnSeaMine,
    SpawnSeaMine,
    SpawnWoodenCrusher,
    SpawnLargePlatform,
    SpawnSmallPlatform,
    SpawnMediumPlatform,
    SpawnRedGemPlatform,
    SpawnYellowGemPlatform,
    SpawnGreenGemPlatform,
    SpawnBlueGemPlatform,
    SpawnRoomExit,
    SpawnBonusPlatform,
    SpawnFlipPlatform,
    SpawnRockPlatform,
    SpawnCrateGemMarker,
    SpawnFlame,
    SpawnSeaweed,
};
