#include "core.h"

/*
 * ROM 0x0816C6A4-0x0816C814. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern void nullsub_21();
extern void nullsub_22();
extern void SpawnStartMarker();
extern void SpawnCrystal();
extern void sub_801EBF0();
extern void SpawnRedGem();
extern void SpawnGreenGem();
extern void SpawnYellowGem();
extern void sub_801EF0C();
extern void SpawnVulture();
extern void SpawnVenusFlytrap();
extern void sub_801F2BC();
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
extern void sub_8020138();
extern void sub_802026C();
extern void SpawnSaucerLabAssistant();
extern void SpawnPistonCrusher();
extern void SpawnFlamethrowerLabAssistant();
extern void sub_8020788();
extern void sub_80208C4();
extern void SpawnRat();
extern void SpawnFrog();
extern void SpawnSeaMine();
extern void SpawnWoodenCrusher();
extern void SpawnRedGemPlatform();
extern void SpawnYellowGemPlatform();
extern void SpawnGreenGemPlatform();
extern void SpawnBlueGemPlatform();
extern void sub_8021280();
extern void SpawnDingodile();
extern void SpawnTiny();
extern void SpawnCortexBoss();
extern void sub_8021668();
extern void SpawnSeaweed();
extern void SpawnFlame();
extern void SpawnRockPlatform();
extern void sub_80218E8();
extern void SpawnBonusPlatform();
extern void SpawnMediumPlatform();
extern void SpawnSmallPlatform();
extern void SpawnLargePlatform();
extern void sub_80219E0();
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
extern void sub_802218C();
extern void sub_80221A4();
extern void sub_80221BC();
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
    sub_80221BC,
    sub_80221A4,
    sub_802218C,
    nullsub_22,
    SpawnWumpa,
    SpawnCrystal,
    sub_801EBF0,
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
    sub_801EF0C,
    SpawnVulture,
    SpawnVenusFlytrap,
    sub_801F2BC,
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
    sub_8020138,
    sub_802026C,
    SpawnSaucerLabAssistant,
    SpawnPistonCrusher,
    nullsub_21,
    sub_80219E0,
    nullsub_21,
    SpawnSaucerLabAssistant,
    SpawnFlamethrowerLabAssistant,
    sub_8020788,
    sub_80208C4,
    SpawnRat,
    SpawnFrog,
    SpawnDingodile,
    nullsub_21,
    SpawnTiny,
    SpawnCortexBoss,
    sub_8021668,
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
    sub_8021280,
    SpawnBonusPlatform,
    sub_80218E8,
    SpawnRockPlatform,
    SpawnCrateGemMarker,
    SpawnFlame,
    SpawnSeaweed,
};
