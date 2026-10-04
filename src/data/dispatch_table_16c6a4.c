#include "core.h"

/*
 * ROM 0x0816C6A4-0x0816C814. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern void nullsub_21();
extern void nullsub_22();
extern void sub_801E990();
extern void SpawnCrystal();
extern void sub_801EBF0();
extern void SpawnRedGem();
extern void SpawnGreenGem();
extern void SpawnYellowGem();
extern void sub_801EF0C();
extern void sub_801F050();
extern void sub_801F170();
extern void sub_801F2BC();
extern void sub_801F3DC();
extern void sub_801F528();
extern void sub_801F7B8();
extern void sub_801F8DC();
extern void sub_801FA3C();
extern void sub_801FB74();
extern void sub_801FCB4();
extern void sub_801FDEC();
extern void sub_801FEEC();
extern void sub_8020010();
extern void sub_8020138();
extern void sub_802026C();
extern void sub_80203A8();
extern void sub_80204EC();
extern void sub_802062C();
extern void sub_8020788();
extern void sub_80208C4();
extern void sub_80209EC();
extern void sub_8020B0C();
extern void sub_8020C18();
extern void sub_8020D4C();
extern void SpawnRedGemPlatform();
extern void SpawnYellowGemPlatform();
extern void SpawnGreenGemPlatform();
extern void SpawnBlueGemPlatform();
extern void sub_8021280();
extern void sub_8021388();
extern void sub_8021480();
extern void sub_802155C();
extern void sub_8021668();
extern void sub_8021748();
extern void sub_802183C();
extern void sub_80218C4();
extern void sub_80218E8();
extern void sub_802190C();
extern void sub_8021974();
extern void sub_8021998();
extern void sub_80219BC();
extern void sub_80219E0();
extern void sub_8021A00();
extern void SpawnTimeCrate3();
extern void SpawnTimeCrate2();
extern void SpawnTimeCrate1();
extern void sub_8021AB8();
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
extern void sub_802209C();
extern void SpawnWumpa();
extern void sub_802218C();
extern void sub_80221A4();
extern void sub_80221BC();
extern void sub_80221D4();

/* The unified 92-slot function-pointer dispatch array (docs/rom_map.md
 * "Major correction: there is no second table"): the spawn function of
 * each entity type of the room data (docs/levels.md, "Entities").
 * CreateEntitySpawner (graphics_loading_21d80.c) hands it to sub_8025D4C
 * with a count of 0x5c, as gEntitySpawner; SpawnEntity calls entry
 * `type` with the entity's id, x, y and param. */
void (*const gEntitySpawnFuncs[92])() = {
    sub_801E990,
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
    sub_8021AB8,
    SpawnTimeCrate1,
    SpawnTimeCrate2,
    SpawnTimeCrate3,
    sub_801EF0C,
    sub_801F050,
    sub_801F170,
    sub_801F2BC,
    sub_801F3DC,
    sub_801F528,
    sub_8021A00,
    sub_801F7B8,
    sub_801F8DC,
    sub_801FA3C,
    sub_801FB74,
    sub_801FCB4,
    sub_801FDEC,
    sub_801FEEC,
    nullsub_21,
    sub_8020010,
    sub_8020138,
    sub_802026C,
    sub_80203A8,
    sub_80204EC,
    nullsub_21,
    sub_80219E0,
    nullsub_21,
    sub_80203A8,
    sub_802062C,
    sub_8020788,
    sub_80208C4,
    sub_80209EC,
    sub_8020B0C,
    sub_8021388,
    nullsub_21,
    sub_8021480,
    sub_802155C,
    sub_8021668,
    SpawnTornadoSpinPower,
    sub_8020C18,
    sub_8020C18,
    sub_8020D4C,
    sub_80219BC,
    sub_8021998,
    sub_8021974,
    SpawnRedGemPlatform,
    SpawnYellowGemPlatform,
    SpawnGreenGemPlatform,
    SpawnBlueGemPlatform,
    sub_8021280,
    sub_802190C,
    sub_80218E8,
    sub_80218C4,
    sub_802209C,
    sub_802183C,
    sub_8021748,
};
