#include "core.h"
#include "vtable.h"

/*
 * ROM 0x087E3BEC-0x087E55E4: the 93 virtual tables of the game's C++
 * object classes (docs/rom_map.md's "93 entity vtables"), in ROM order.
 * Constructors store one at the object's method-table pointer (e.g.
 * `obj->table = gStaticData_087E3BEC` in graphics.c, `self->vtable` in
 * gobj_1a794.h); the code reads the slots through `struct method` /
 * `struct actor_method`. Tables of the same class family share their
 * leading slots, the ROM's own inheritance. Linked in ROM order between
 * data/data.s sections by ldscript.txt - see docs/data.md.
 */

extern void DestroyFont();
extern void FontMeasureText();
extern void DrawActor();
extern void FontUploadTiles();
extern void nullsub_11();
extern void nullsub_13();
extern void nullsub_15();
extern void nullsub_20();
extern void nullsub_38();
extern void nullsub_44();
extern void nullsub_9();
extern void sub_8006FE4();
extern void sub_8007048();
extern void sub_80070D4();
extern void sub_80070E8();
extern void sub_800710C();
extern void sub_8007110();
extern void sub_8007114();
extern void sub_800722C();
extern void sub_80073BC();
extern void sub_8007DBC();
extern void sub_8007F78();
extern void sub_8007FD8();
extern void sub_8008304();
extern void sub_8008328();
extern void sub_800834C();
extern void sub_8008350();
extern void sub_8008364();
extern void sub_8008394();
extern void sub_8008408();
extern void sub_8008480();
extern void sub_8008484();
extern void sub_80088E8();
extern void sub_80088F0();
extern void sub_8009CA0();
extern void sub_8009DF4();
extern void sub_8009ECC();
extern void sub_8009F1C();
extern void sub_8009FB0();
extern void sub_8009FD4();
extern void sub_800A050();
extern void sub_800A0FC();
extern void sub_800A528();
extern void sub_800A5F4();
extern void sub_800A600();
extern void sub_800A650();
extern void sub_800A884();
extern void sub_800AB9C();
extern void PlayerHandleEvent();
extern void DrawPlayer();
extern void sub_800B270();
extern void sub_800B360();
extern void DestroyPlayer();
extern void sub_800B698();
extern void sub_800B6A0();
extern void sub_800B6D0();
extern void sub_800B704();
extern void sub_800B734();
extern void sub_800B7B0();
extern void sub_800B838();
extern void sub_800B86C();
extern void sub_800B8A4();
extern void sub_800B8A8();
extern void UpdateEnemyCtrl();
extern void HitEnemy();
extern void AttachEnemyCtrl();
extern void DestroyEnemyCtrl();
extern void UpdatePeriodicSpawner();
extern void DestroyPeriodicSpawner();
extern void UpdateKnockedEnemyCtrl();
extern void DestroyKnockedEnemyCtrl();
extern void sub_800CBF4();
extern void sub_800CCCC();
extern void DrawCrate();
extern void UpdateCrate();
extern void sub_8010674();
extern void GetCrateClassId();
extern void DestroyCrate();
extern void CheckExtraLifePickup();
extern void UpdateExtraLife();
extern void DrawExtraLife();
extern void sub_80112F0();
extern void DestroyExtraLife();
extern void sub_8011330();
extern void CheckWumpaPickup();
extern void UpdateWumpa();
extern void DrawWumpa();
extern void sub_80119D4();
extern void DestroyWumpa();
extern void sub_8011A1C();
extern void sub_8011A8C();
extern void DestroyStopwatch();
extern void sub_8011BD4();
extern void sub_8012420();
extern void sub_8015350();
extern void sub_80155A8();
extern void sub_80157C4();
extern void sub_8015878();
extern void sub_8016128();
extern void sub_8016288();
extern void sub_8017218();
extern void sub_80174D8();
extern void sub_8017650();
extern void sub_80179D4();
extern void sub_80179E8();
extern void sub_80179EC();
extern void sub_8017A70();
extern void sub_8017A78();
extern void sub_8017AB0();
extern void sub_8017F5C();
extern void sub_8017F80();
extern void sub_8017FD4();
extern void UpdateTiny();
extern void sub_80187FC();
extern void sub_8018858();
extern void sub_8018884();
extern void sub_80188E8();
extern void sub_80188FC();
extern void sub_8018960();
extern void DestroyTiny();
extern void sub_8018A30();
extern void sub_8018E4C();
extern void sub_8019324();
extern void sub_8019464();
extern void sub_80194E0();
extern void sub_80195D8();
extern void sub_8019608();
extern void sub_801964C();
extern void sub_80196E4();
extern void sub_8019730();
extern void sub_8019744();
extern void sub_80197C8();
extern void UpdateDingodile();
extern void sub_801A114();
extern void sub_801A2A8();
extern void sub_801A64C();
extern void sub_801A73C();
extern void sub_801A750();
extern void sub_801A780();
extern void DestroyDingodile();
extern void CheckPlatformContact();
extern void UpdatePlatform();
extern void sub_801B2C0();
extern void DestroyPlatform();
extern void UpdatePlatformMover();
extern void sub_801B77C();
extern void sub_801B7A0();
extern void DestroyPlatformMover();
extern void sub_801B8BC();
extern void sub_801B91C();
extern void sub_801BA60();
extern void sub_801BAB0();
extern void sub_801DE30();
extern void sub_801DEA4();
extern void sub_801DF70();
extern void sub_801DF98();
extern void DestroyBgStreamer();
extern void DestroyBgLayerBase();
extern void sub_8024DCC();
extern void ScrollBgLayerBase();
extern void ResetBgLayerBase();
extern void ClipBgLayerColumns();
extern void ClipBgLayerRows();
extern void ScrollBgLayer();
extern void DrawBgLayerColumn();
extern void DrawBgLayerRow();
extern void ResetBgLayer();
extern void LoadBgLayerTiles();
extern void DestroyBgLayer();
extern void DrawPooledBgLayerColumn();
extern void sub_8026250();
extern void ClipPooledBgLayerColumns();
extern void ClipPooledBgLayerRows();
extern void DrawPooledBgLayerRow();
extern void ResetPooledBgLayer();
extern void LoadPooledBgLayerTiles();
extern void DestroyPooledBgLayer();
extern void sub_802710C();
extern void FontDrawGlyph();
extern void FontPutChar();
extern void FontDrawChars();
extern void FontDrawText();
extern void FontMeasureChars();
extern void UpdateActor();
extern void DestroyActor();
extern void sub_802B364();
extern void sub_802B5B4();
extern void sub_802C19C();
extern void UpdatePolarCollectedWumpa();
extern void DrawPolarCollectedWumpa();
extern void DestroyPolarCollectedWumpa();
extern void UpdatePolarWumpa();
extern void UpdatePolarCrate();
extern void UpdatePolarQuestionCrate();
extern void UpdatePolarLifeCrate();
extern void UpdatePolarNitroCrate();
extern void UpdatePolarAkuAkuCrate();
extern void UpdatePolarTimeCrate();
extern void sub_802CA6C();
extern void UpdatePolarBasicCrate();
extern void UpdatePolarElectricFence();
extern void sub_802CE10();
extern void sub_802CE5C();
extern void UpdatePolarPenguin();
extern void UpdatePolarIcicle();
extern void UpdatePolarAkuAku();
extern void sub_802D59C();
extern void sub_802D600();
extern void UpdatePolarCheckpointCrate();
extern void sub_802E84C();
extern void sub_802E9FC();
extern void sub_802EB78();
extern void sub_802F47C();
extern void sub_802F6DC();
extern void UpdateJetpackShot();
extern void sub_802FA34();
extern void UpdateJetpackPlane();
extern void DamageJetpackPlane();
extern void sub_802FF00();
extern void UpdateJetpackBomber();
extern void DamageJetpackBomber();
extern void sub_8030290();
extern void UpdateJetpackCannonball();
extern void sub_8030330();
extern void sub_8030530();
extern void sub_8030574();
extern void sub_80306A4();
extern void UpdateJetpackBalloon();
extern void DamageJetpackBalloon();
extern void sub_8031A64();
extern void UpdateJetpackBalloonCrate();
extern void UpdateJetpackQuestionCrate();
extern void DamageJetpackQuestionCrate();
extern void UpdateJetpackHealthCrate();
extern void UpdateJetpackTimeCrate();
extern void DamageJetpackTimeCrate();
extern void DamageJetpackHealthCrate();
extern void sub_8032140();
extern void DamageJetpackBalloonCrate();
extern void DestroyJetpackBalloonCrate();
extern void sub_8032350();
extern void UpdateJetpackParachuteNitro();
extern void DamageJetpackParachuteNitro();
extern void sub_8032478();
extern void UpdateJetpackRocket();
extern void DamageJetpackRocket();
extern void sub_8032680();
extern void UpdateJetpackRing();
extern void sub_8032714();
extern void UpdateJetpackCollectedWumpa();
extern void DrawJetpackCollectedWumpa();
extern void DestroyJetpackCollectedWumpa();
extern void sub_803290C();
extern void sub_8032910();
extern void sub_8032950();
extern void sub_8032AF0();
extern void DamageHovercraftCannon();
extern void UpdateHovercraftCannon();
extern void sub_8033CF0();
extern void DamageHovercraftLauncher();
extern void UpdateHovercraftLauncher();
extern void sub_8034050();
extern void sub_8034110();
extern void sub_8034188();
extern void sub_8034264();
extern void sub_8034270();
extern void sub_803436C();
extern void UpdateLogoActor();
extern void DrawLogoActor();
extern void DestroyLogoActor();
extern void DestroyLargeFont();
extern void DestroySmallFont();
extern void sub_803B0C4();
extern void UpdatePolarCheckpointText();
extern void DestroyPolarCheckpointText();
extern void DestroyPolarWumpa();
extern void DestroyPolarTimeCrate();
extern void DestroyPolarQuestionCrate();
extern void DestroyPolarAkuAkuCrate();
extern void DestroyPolarNitroCrate();
extern void DestroyPolarLifeCrate();
extern void sub_803B25C();
extern void DestroyPolarBasicCrate();
extern void DestroyPolarCrate();
extern void DestroyPolarElectricFence();
extern void sub_803B30C();
extern void sub_803B338();
extern void DestroyPolarPenguin();
extern void DestroyPolarIcicle();
extern void DestroyPolarAkuAku();
extern void sub_803B3E8();
extern void sub_803B414();
extern void DestroyPolarCheckpointCrate();
extern void DrawJetpackCheckpointText();
extern void UpdateJetpackCheckpointText();
extern void sub_803B54C();
extern void DestroyJetpackCheckpointText();
extern void UpdateJetpackExplosion();
extern void sub_803B5AC();
extern void DestroyJetpackExplosion();
extern void GetActorHp();
extern void sub_803B5E4();
extern void DestroyJetpackShot();
extern void DestroyJetpackPlane();
extern void DestroyJetpackBomber();
extern void DestroyJetpackCannonball();
extern void sub_803B698();
extern void DestroyJetpackBalloon();
extern void DestroyJetpackHealthCrate();
extern void DestroyJetpackTimeCrate();
extern void DestroyJetpackQuestionCrate();
extern void DestroyJetpackParachuteNitro();
extern void DestroyJetpackRocket();
extern void DestroyJetpackRing();
extern void sub_803B7D4();
extern void DestroyHovercraftCannon();
extern void DestroyHovercraftLauncher();
extern void sub_803B858();
extern void sub_803B884();

/* Used by actor_aabb_setup.c, actor_part124.c, actor_part39.c,
 * actor_part6.c (sub_8008484), actor_part7.c, graphics.c (nullsub_12,
 * sub_8007230, sub_80073BC). */
const struct vtable_slot gStaticData_087E3BEC[11] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8007048),
    VTABLE_SLOT(sub_80070E8),
    VTABLE_SLOT(sub_80070D4),
    VTABLE_SLOT(nullsub_11),
    VTABLE_SLOT(sub_8007110),
    VTABLE_SLOT(sub_800710C),
    VTABLE_SLOT(sub_8006FE4),
    VTABLE_SLOT(sub_8007114),
    VTABLE_SLOT(sub_800722C),
    VTABLE_SLOT(sub_80073BC),
};

/* Used by actor_part6.c (sub_8008408, sub_8008484), actor_part7.c. */
const struct vtable_slot gStaticData_087E3C44[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8007DBC),
    VTABLE_SLOT(sub_8008394),
    VTABLE_SLOT(sub_8008364),
    VTABLE_SLOT(sub_8008350),
    VTABLE_SLOT(sub_8007F78),
    VTABLE_SLOT(sub_8007FD8),
    VTABLE_SLOT(sub_8008328),
    VTABLE_SLOT(sub_8008304),
    VTABLE_SLOT(sub_8008480),
    VTABLE_SLOT(sub_8008484),
    VTABLE_SLOT(sub_8008408),
    VTABLE_SLOT(sub_800834C),
};

/* Used by actor_part7.c (sub_80088F0). */
const struct vtable_slot gStaticData_087E3CAC[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8007DBC),
    VTABLE_SLOT(sub_8008394),
    VTABLE_SLOT(sub_8008364),
    VTABLE_SLOT(sub_8008350),
    VTABLE_SLOT(sub_8007F78),
    VTABLE_SLOT(sub_8007FD8),
    VTABLE_SLOT(sub_8008328),
    VTABLE_SLOT(sub_8008304),
    VTABLE_SLOT(sub_8008480),
    VTABLE_SLOT(sub_80088F0),
    VTABLE_SLOT(sub_80088E8),
    VTABLE_SLOT(sub_800834C),
};

/* Used by actor_part14.c, actor_part8.c (sub_8009ECC, sub_8009F1C,
 * sub_8009F50). */
const struct vtable_slot gStaticData_087E3D14[15] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_800A050),
    VTABLE_SLOT(sub_8008394),
    VTABLE_SLOT(sub_8009FB0),
    VTABLE_SLOT(sub_8008350),
    VTABLE_SLOT(sub_8007F78),
    VTABLE_SLOT(sub_8007FD8),
    VTABLE_SLOT(sub_8008328),
    VTABLE_SLOT(sub_8008304),
    VTABLE_SLOT(sub_8009ECC),
    VTABLE_SLOT(sub_8009F1C),
    VTABLE_SLOT(sub_8008408),
    VTABLE_SLOT(sub_8009DF4),
    VTABLE_SLOT(sub_8009FD4),
    VTABLE_SLOT(sub_8009CA0),
};

/* Used by actor_part14.c (sub_800A600, sub_800A650, sub_800A664). */
const struct vtable_slot gStaticData_087E3D8C[15] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_800A0FC),
    VTABLE_SLOT(sub_8008394),
    VTABLE_SLOT(sub_800A528),
    VTABLE_SLOT(sub_800A5F4),
    VTABLE_SLOT(sub_8007F78),
    VTABLE_SLOT(sub_8007FD8),
    VTABLE_SLOT(sub_8008328),
    VTABLE_SLOT(sub_8008304),
    VTABLE_SLOT(sub_800A600),
    VTABLE_SLOT(sub_800A650),
    VTABLE_SLOT(sub_8008408),
    VTABLE_SLOT(sub_8009DF4),
    VTABLE_SLOT(sub_8009FD4),
    VTABLE_SLOT(sub_8009CA0),
};

/* Used by actor_part15.c, actor_part77.c. */
const struct vtable_slot gPlayerVtable[15] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_800A884),
    VTABLE_SLOT(sub_8008394),
    VTABLE_SLOT(sub_800B360),
    VTABLE_SLOT(DrawPlayer),
    VTABLE_SLOT(sub_8007F78),
    VTABLE_SLOT(sub_8007FD8),
    VTABLE_SLOT(sub_8008328),
    VTABLE_SLOT(sub_8008304),
    VTABLE_SLOT(sub_800A600),
    VTABLE_SLOT(DestroyPlayer),
    VTABLE_SLOT(sub_8008408),
    VTABLE_SLOT(sub_800B270),
    VTABLE_SLOT(PlayerHandleEvent),
    VTABLE_SLOT(sub_800AB9C),
};

/* Used by actor_part124.c, actor_part17.c, actor_part27.c, actor_part57.c. */
const struct vtable_slot gStaticData_087E3E7C[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(nullsub_9),
    VTABLE_SLOT(nullsub_13),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_800B8A8),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part112.c, actor_part124.c. */
const struct vtable_slot gEnemyCtrlVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateEnemyCtrl),
    VTABLE_SLOT(HitEnemy),
    VTABLE_SLOT(AttachEnemyCtrl),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(DestroyEnemyCtrl),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part124.c. */
const struct vtable_slot gPeriodicSpawnerVtable[11] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8007048),
    VTABLE_SLOT(sub_80070E8),
    VTABLE_SLOT(UpdatePeriodicSpawner),
    VTABLE_SLOT(nullsub_11),
    VTABLE_SLOT(sub_8007110),
    VTABLE_SLOT(sub_800710C),
    VTABLE_SLOT(sub_8006FE4),
    VTABLE_SLOT(sub_8007114),
    VTABLE_SLOT(sub_800722C),
    VTABLE_SLOT(DestroyPeriodicSpawner),
};

/* Used by actor_part112.c, actor_part117.c, actor_part124.c. */
const struct vtable_slot gKnockedEnemyCtrlVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateKnockedEnemyCtrl),
    VTABLE_SLOT(nullsub_13),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(DestroyKnockedEnemyCtrl),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part123.c. */
const struct vtable_slot gStaticData_087E400C[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_800CBF4),
    VTABLE_SLOT(nullsub_15),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_800CCCC),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by game_loop31.c (DestroyCrate), game_loop36.c (CreateCrate). */
const struct vtable_slot gCrateVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8007DBC),
    VTABLE_SLOT(sub_8008394),
    VTABLE_SLOT(UpdateCrate),
    VTABLE_SLOT(DrawCrate),
    VTABLE_SLOT(sub_8007F78),
    VTABLE_SLOT(sub_8007FD8),
    VTABLE_SLOT(sub_8008328),
    VTABLE_SLOT(sub_8010674),
    VTABLE_SLOT(GetCrateClassId),
    VTABLE_SLOT(DestroyCrate),
    VTABLE_SLOT(sub_8008408),
    VTABLE_SLOT(sub_800834C),
};

/* Used by game_loop52.c, game_loop54.c (UpdateExtraLife). */
const struct vtable_slot gExtraLifeVtable[14] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8011330),
    VTABLE_SLOT(sub_8008394),
    VTABLE_SLOT(UpdateExtraLife),
    VTABLE_SLOT(DrawExtraLife),
    VTABLE_SLOT(sub_8007F78),
    VTABLE_SLOT(sub_8007FD8),
    VTABLE_SLOT(sub_8008328),
    VTABLE_SLOT(sub_8008304),
    VTABLE_SLOT(sub_80112F0),
    VTABLE_SLOT(DestroyExtraLife),
    VTABLE_SLOT(sub_8008408),
    VTABLE_SLOT(sub_800834C),
    VTABLE_SLOT(CheckExtraLifePickup),
};

/* Used by actor_part39.c (DestroyWumpa, sub_80119EC), game_loop53.c
 * (UpdateWumpa). */
const struct vtable_slot gWumpaVtable[14] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8011A1C),
    VTABLE_SLOT(sub_8008394),
    VTABLE_SLOT(UpdateWumpa),
    VTABLE_SLOT(DrawWumpa),
    VTABLE_SLOT(sub_8007F78),
    VTABLE_SLOT(sub_8007FD8),
    VTABLE_SLOT(sub_8008328),
    VTABLE_SLOT(sub_8008304),
    VTABLE_SLOT(sub_80119D4),
    VTABLE_SLOT(DestroyWumpa),
    VTABLE_SLOT(sub_8008408),
    VTABLE_SLOT(sub_800834C),
    VTABLE_SLOT(CheckWumpaPickup),
};

/* Used by actor_part39.c (sub_8011A8C, DestroyStopwatch). */
const struct vtable_slot gStopwatchVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8007DBC),
    VTABLE_SLOT(sub_8008394),
    VTABLE_SLOT(sub_8011A8C),
    VTABLE_SLOT(sub_8008350),
    VTABLE_SLOT(sub_8007F78),
    VTABLE_SLOT(sub_8007FD8),
    VTABLE_SLOT(sub_8008328),
    VTABLE_SLOT(sub_8008304),
    VTABLE_SLOT(sub_8008480),
    VTABLE_SLOT(DestroyStopwatch),
    VTABLE_SLOT(sub_8008408),
    VTABLE_SLOT(sub_800834C),
};

/* Used by actor_part39.c, actor_part57.c. */
const struct vtable_slot gStaticData_087E4224[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8012420),
    VTABLE_SLOT(sub_8011BD4),
    VTABLE_SLOT(sub_80155A8),
    VTABLE_SLOT(sub_8015350),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_8015878),
    VTABLE_SLOT(sub_80157C4),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_16048.c (sub_80174D8), player_ctrl.h. */
const struct vtable_slot gPlayerCtrlVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8016288),
    VTABLE_SLOT(sub_8016128),
    VTABLE_SLOT(sub_8017218),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_80174D8),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_17524.c (sub_80179EC). */
const struct vtable_slot gInputCtrlVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8017650),
    VTABLE_SLOT(sub_80179D4),
    VTABLE_SLOT(sub_80179E8),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_80179EC),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part27.c. */
const struct vtable_slot gStaticData_087E435C[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(nullsub_9),
    VTABLE_SLOT(sub_8017A70),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_8017A78),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part27b.c. */
const struct vtable_slot gStaticData_087E43C4[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8017AB0),
    VTABLE_SLOT(sub_8017A70),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_8017FD4),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_8017F80),
    VTABLE_SLOT(sub_8017F5C),
};

/* Used by actor_part27c.c. */
const struct vtable_slot gStaticData_087E442C[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_80187FC),
    VTABLE_SLOT(nullsub_13),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_8018858),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_188d0.c (sub_80188D0, sub_80188E8). */
const struct vtable_slot gStaticData_087E4494[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8018884),
    VTABLE_SLOT(nullsub_13),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_80188E8),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_188d0.c (sub_8018948, sub_8018960). */
const struct vtable_slot gStaticData_087E44FC[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_80188FC),
    VTABLE_SLOT(nullsub_13),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_8018960),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_18008.c, actor_part_188d0.c (DestroyTiny,
 * CreateTiny). */
const struct vtable_slot gTinyVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateTiny),
    VTABLE_SLOT(sub_8017A70),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(DestroyTiny),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_188d0.c (sub_80195D8, sub_80195EC). */
const struct vtable_slot gStaticData_087E45CC[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_80194E0),
    VTABLE_SLOT(nullsub_13),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_80195D8),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_188d0.c (sub_8019608, sub_801961C). */
const struct vtable_slot gStaticData_087E4634[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8019464),
    VTABLE_SLOT(nullsub_13),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_8019608),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_801B7A0),
    VTABLE_SLOT(sub_801B77C),
};

/* Used by actor_part_188d0.c (sub_801964C, sub_8019660). */
const struct vtable_slot gStaticData_087E469C[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8019324),
    VTABLE_SLOT(nullsub_13),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_801964C),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_1967c.c (sub_80196E4). */
const struct vtable_slot gStaticData_087E4704[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8018E4C),
    VTABLE_SLOT(nullsub_13),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_80196E4),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_1967c.c (sub_8019744). */
const struct vtable_slot gStaticData_087E476C[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8019730),
    VTABLE_SLOT(nullsub_13),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_8019744),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_1967c.c (sub_80197C8). */
const struct vtable_slot gStaticData_087E47D4[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8018A30),
    VTABLE_SLOT(sub_8017A70),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_80197C8),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_1967c.c (sub_801A64C, sub_801A73C). */
const struct vtable_slot gStaticData_087E483C[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_801A64C),
    VTABLE_SLOT(HitEnemy),
    VTABLE_SLOT(AttachEnemyCtrl),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_801A73C),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_1967c.c (sub_801A584, sub_801A750). */
const struct vtable_slot gStaticData_087E48A4[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_801A2A8),
    VTABLE_SLOT(sub_8017A70),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_801A750),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_1967c.c (sub_801A780), actor_part_1a794.c,
 * gobj_1a794.h. */
const struct vtable_slot gStaticData_087E490C[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_801A114),
    VTABLE_SLOT(sub_8017A70),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_801A780),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_1967c.c, actor_part_1a794.c (DestroyDingodile),
 * gobj_1a794.h. */
const struct vtable_slot gDingodileVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateDingodile),
    VTABLE_SLOT(sub_8017A70),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(DestroyDingodile),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_1b208.c (DestroyPlatform), gobj_1a794.h. */
const struct vtable_slot gPlatformVtable[15] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(CheckPlatformContact),
    VTABLE_SLOT(sub_8008394),
    VTABLE_SLOT(UpdatePlatform),
    VTABLE_SLOT(sub_8008350),
    VTABLE_SLOT(sub_8007F78),
    VTABLE_SLOT(sub_8007FD8),
    VTABLE_SLOT(sub_8008328),
    VTABLE_SLOT(sub_8008304),
    VTABLE_SLOT(sub_801B2C0),
    VTABLE_SLOT(DestroyPlatform),
    VTABLE_SLOT(sub_8008408),
    VTABLE_SLOT(sub_8009DF4),
    VTABLE_SLOT(sub_8009FD4),
    VTABLE_SLOT(sub_8009CA0),
};

/* Used by actor_part_1b208.c (DestroyPlatformMover), gobj_1a794.h. */
const struct vtable_slot gPlatformMoverVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdatePlatformMover),
    VTABLE_SLOT(nullsub_13),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(DestroyPlatformMover),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_801B7A0),
    VTABLE_SLOT(sub_801B77C),
};

/* Used by actor_part_1b85c.c (sub_801B91C). */
const struct vtable_slot gStaticData_087E4ABC[15] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_800A050),
    VTABLE_SLOT(sub_8008394),
    VTABLE_SLOT(sub_801B8BC),
    VTABLE_SLOT(sub_8008350),
    VTABLE_SLOT(sub_8007F78),
    VTABLE_SLOT(sub_8007FD8),
    VTABLE_SLOT(sub_8008328),
    VTABLE_SLOT(sub_8008304),
    VTABLE_SLOT(sub_8009ECC),
    VTABLE_SLOT(sub_801B91C),
    VTABLE_SLOT(sub_8008408),
    VTABLE_SLOT(sub_8009DF4),
    VTABLE_SLOT(sub_8009FD4),
    VTABLE_SLOT(sub_8009CA0),
};

/* Used by actor_part_1b85c.c (sub_801B980, sub_801BAB0, sub_801BAC4). */
const struct vtable_slot gStaticData_087E4B34[15] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_800A050),
    VTABLE_SLOT(sub_8008394),
    VTABLE_SLOT(sub_8009FB0),
    VTABLE_SLOT(sub_8008350),
    VTABLE_SLOT(sub_8007F78),
    VTABLE_SLOT(sub_8007FD8),
    VTABLE_SLOT(sub_8008328),
    VTABLE_SLOT(sub_8008304),
    VTABLE_SLOT(sub_8009ECC),
    VTABLE_SLOT(sub_801BAB0),
    VTABLE_SLOT(sub_8008408),
    VTABLE_SLOT(sub_8009DF4),
    VTABLE_SLOT(sub_8009FD4),
    VTABLE_SLOT(sub_801BA60),
};

/* Used by actor_part_1da38.c (sub_801DF98), actor_part_1dfec.c,
 * level_select_parts.h. */
const struct vtable_slot gStaticData_087E4BAC[6] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_801DE30),
    VTABLE_SLOT(sub_801DEA4),
    VTABLE_SLOT(sub_801DF70),
    VTABLE_SLOT(nullsub_20),
    VTABLE_SLOT(sub_801DF98),
};

/* Used by game_loop57.c. */
const struct vtable_slot gBgStreamerVtable[2] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyBgStreamer),
};

/* Used by game_loop57.c. */
const struct vtable_slot gBgLayerBaseVtable[5] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyBgLayerBase),
    VTABLE_SLOT(ResetBgLayerBase),
    VTABLE_SLOT(ScrollBgLayerBase),
    VTABLE_SLOT(sub_8024DCC),
};

/* Used by bg_scroll_layer_25fc8.c (DestroyBgLayer), game_loop15.c
 * (InitBgLayer), tile_slot_pool.c (DestroyPooledBgLayer), bg_scroll_layer.h. */
const struct vtable_slot gBgLayerVtable[10] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyBgLayer),
    VTABLE_SLOT(ResetBgLayer),
    VTABLE_SLOT(ScrollBgLayer),
    VTABLE_SLOT(sub_8024DCC),
    VTABLE_SLOT(LoadBgLayerTiles),
    VTABLE_SLOT(DrawBgLayerRow),
    VTABLE_SLOT(DrawBgLayerColumn),
    VTABLE_SLOT(ClipBgLayerColumns),
    VTABLE_SLOT(ClipBgLayerRows),
};

/* Used by bg_scroll_layer_25fc8.c, tile_slot_pool.c (DestroyPooledBgLayer),
 * bg_scroll_layer.h. */
const struct vtable_slot gPooledBgLayerVtable[10] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPooledBgLayer),
    VTABLE_SLOT(ResetPooledBgLayer),
    VTABLE_SLOT(ScrollBgLayer),
    VTABLE_SLOT(sub_8026250),
    VTABLE_SLOT(LoadPooledBgLayerTiles),
    VTABLE_SLOT(DrawPooledBgLayerRow),
    VTABLE_SLOT(DrawPooledBgLayerColumn),
    VTABLE_SLOT(ClipPooledBgLayerColumns),
    VTABLE_SLOT(ClipPooledBgLayerRows),
};

/* Used by hud_icon_slot.c (sub_802710C). */
const struct vtable_slot gStaticData_087E4CB4[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8007DBC),
    VTABLE_SLOT(sub_8008394),
    VTABLE_SLOT(sub_8008364),
    VTABLE_SLOT(sub_8008350),
    VTABLE_SLOT(sub_8007F78),
    VTABLE_SLOT(sub_8007FD8),
    VTABLE_SLOT(sub_8008328),
    VTABLE_SLOT(sub_8008304),
    VTABLE_SLOT(sub_8008480),
    VTABLE_SLOT(sub_802710C),
    VTABLE_SLOT(sub_80088E8),
    VTABLE_SLOT(sub_800834C),
};

/* Used by actor_aabb_setup.c, hud_icon_widget_85c4.c (FontDrawGlyph). */
const struct vtable_slot gLargeFontVtable[9] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyLargeFont),
    VTABLE_SLOT(FontMeasureText),
    VTABLE_SLOT(FontMeasureChars),
    VTABLE_SLOT(FontDrawText),
    VTABLE_SLOT(FontDrawChars),
    VTABLE_SLOT(FontDrawGlyph),
    VTABLE_SLOT(FontPutChar),
    VTABLE_SLOT(FontUploadTiles),
};

/* Used by actor_aabb_setup.c, hud_icon_widget_85c4.c (FontDrawGlyph). */
const struct vtable_slot gSmallFontVtable[9] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroySmallFont),
    VTABLE_SLOT(FontMeasureText),
    VTABLE_SLOT(FontMeasureChars),
    VTABLE_SLOT(FontDrawText),
    VTABLE_SLOT(FontDrawChars),
    VTABLE_SLOT(FontDrawGlyph),
    VTABLE_SLOT(FontPutChar),
    VTABLE_SLOT(FontUploadTiles),
};

/* Used by actor_aabb_setup.c, hud_icon_widget5.c (DestroyFont),
 * hud_icon_widget_85c4.c (FontDrawGlyph), hud_icon_widget_8a78.c. */
const struct vtable_slot gFontVtable[9] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyFont),
    VTABLE_SLOT(FontMeasureText),
    VTABLE_SLOT(FontMeasureChars),
    VTABLE_SLOT(FontDrawText),
    VTABLE_SLOT(FontDrawChars),
    VTABLE_SLOT(FontDrawGlyph),
    VTABLE_SLOT(FontPutChar),
    VTABLE_SLOT(FontUploadTiles),
};

/* The actor base class (struct actor_self): slot 1 its destructor
 * DestroyActor, slot 2 the per-frame UpdateActor, slot 3 DrawActor.
 * InitActorPart installs it, and every derived destructor puts it back
 * before unlinking the actor.
 *
 * Used by counter_selector.c (DestroyLogoActor), actor_anim.c (sub_803B0C4,
 * DestroyPolarCheckpointText, DestroyPolarWumpa, DestroyPolarTimeCrate, DestroyPolarQuestionCrate, DestroyPolarAkuAkuCrate,
 * DestroyPolarNitroCrate, DestroyPolarLifeCrate, sub_803B25C, DestroyPolarBasicCrate, DestroyPolarCrate,
 * DestroyPolarElectricFence, sub_803B30C, sub_803B338, DestroyPolarPenguin, DestroyPolarIcicle,
 * DestroyPolarAkuAku, sub_803B3E8, sub_803B414, DestroyPolarCheckpointCrate, DestroyJetpackCheckpointText,
 * DestroyJetpackExplosion, DestroyJetpackShot, DestroyJetpackPlane, DestroyJetpackBomber, DestroyJetpackCannonball,
 * sub_803B698, DestroyJetpackBalloon, DestroyJetpackParachuteNitro, DestroyJetpackRocket, DestroyJetpackRing,
 * sub_803B7D4, DestroyHovercraftCannon, DestroyHovercraftLauncher, sub_803B858, sub_803B884),
 * actor_part129.c, actor_part130.c (DestroyJetpackCollectedWumpa), actor_part19.c,
 * actor_part19c.c, actor_part44.c, actor_part50.c, actor_part52.c. */
const struct vtable_slot gActorVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyActor),
    VTABLE_SLOT(UpdateActor),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part_2ac28.c. */
const struct vtable_slot gStaticData_087E4E14[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B0C4),
    VTABLE_SLOT(UpdateActor),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part_2ac28.c (CreatePolarCheckpointText). */
const struct vtable_slot gPolarCheckpointTextVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarCheckpointText),
    VTABLE_SLOT(UpdatePolarCheckpointText),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part127.c, actor_part19.c, actor_part_2ac28.c
 * (ConstructAnimTableState). */
const struct vtable_slot gStaticData_087E4E54[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_802C19C),
    VTABLE_SLOT(sub_802B364),
    VTABLE_SLOT(sub_802B5B4),
};

/* Used by actor_part19.c, actor_part19c.c, actor_part19c2.c. */
const struct vtable_slot gPolarCollectedWumpaVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarCollectedWumpa),
    VTABLE_SLOT(UpdatePolarCollectedWumpa),
    VTABLE_SLOT(DrawPolarCollectedWumpa),
};

/* Used by actor_part19.c, actor_part19g.c, actor_part_2ac28.c. */
const struct vtable_slot gPolarWumpaVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarWumpa),
    VTABLE_SLOT(UpdatePolarWumpa),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part19i.c, actor_part_2ac28.c. */
const struct vtable_slot gPolarTimeCrateVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarTimeCrate),
    VTABLE_SLOT(UpdatePolarTimeCrate),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part19i.c, actor_part_2ac28.c. */
const struct vtable_slot gPolarQuestionCrateVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarQuestionCrate),
    VTABLE_SLOT(UpdatePolarQuestionCrate),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part19i.c, actor_part_2ac28.c. */
const struct vtable_slot gPolarAkuAkuCrateVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarAkuAkuCrate),
    VTABLE_SLOT(UpdatePolarAkuAkuCrate),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part19i.c, actor_part_2ac28.c. */
const struct vtable_slot gPolarNitroCrateVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarNitroCrate),
    VTABLE_SLOT(UpdatePolarNitroCrate),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part19i.c, actor_part_2ac28.c. */
const struct vtable_slot gPolarLifeCrateVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarLifeCrate),
    VTABLE_SLOT(UpdatePolarLifeCrate),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part19i.c, actor_part_2ac28.c. */
const struct vtable_slot gStaticData_087E4F54[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B25C),
    VTABLE_SLOT(sub_802CA6C),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part19i.c, actor_part_2ac28.c. */
const struct vtable_slot gPolarBasicCrateVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarBasicCrate),
    VTABLE_SLOT(UpdatePolarBasicCrate),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part19i.c. */
const struct vtable_slot gPolarCrateVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarCrate),
    VTABLE_SLOT(UpdatePolarCrate),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part126.c. */
const struct vtable_slot gPolarElectricFenceVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarElectricFence),
    VTABLE_SLOT(UpdatePolarElectricFence),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part126.c. */
const struct vtable_slot gStaticData_087E4FD4[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B30C),
    VTABLE_SLOT(sub_802CE10),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part126.c. */
const struct vtable_slot gStaticData_087E4FF4[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B338),
    VTABLE_SLOT(sub_802CE5C),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part126.c. */
const struct vtable_slot gPolarPenguinVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarPenguin),
    VTABLE_SLOT(UpdatePolarPenguin),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part126.c. */
const struct vtable_slot gPolarIcicleVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarIcicle),
    VTABLE_SLOT(UpdatePolarIcicle),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part58.c. */
const struct vtable_slot gPolarAkuAkuVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarAkuAku),
    VTABLE_SLOT(UpdatePolarAkuAku),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part58.c. */
const struct vtable_slot gStaticData_087E5074[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B3E8),
    VTABLE_SLOT(sub_802D59C),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part58.c. */
const struct vtable_slot gStaticData_087E5094[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B414),
    VTABLE_SLOT(sub_802D600),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part58.c. */
const struct vtable_slot gPolarCheckpointCrateVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarCheckpointCrate),
    VTABLE_SLOT(UpdatePolarCheckpointCrate),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part128.c (CreateJetpackCheckpointText). */
const struct vtable_slot gJetpackCheckpointTextVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackCheckpointText),
    VTABLE_SLOT(UpdateJetpackCheckpointText),
    VTABLE_SLOT(DrawJetpackCheckpointText),
    VTABLE_SLOT(nullsub_44),
    VTABLE_SLOT(sub_803B54C),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part128.c (CreateJetpackExplosion). */
const struct vtable_slot gJetpackExplosionVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackExplosion),
    VTABLE_SLOT(UpdateJetpackExplosion),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(nullsub_44),
    VTABLE_SLOT(sub_803B5AC),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part128.c (sub_802E710), actor_part44.c. */
const struct vtable_slot gStaticData_087E5144[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_802F6DC),
    VTABLE_SLOT(sub_802E84C),
    VTABLE_SLOT(sub_802E9FC),
    VTABLE_SLOT(sub_802EB78),
    VTABLE_SLOT(sub_803B5E4),
    VTABLE_SLOT(sub_802F47C),
};

/* Used by actor_part45c.c. */
const struct vtable_slot gJetpackShotVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackShot),
    VTABLE_SLOT(UpdateJetpackShot),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(nullsub_44),
    VTABLE_SLOT(sub_802FA34),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part_2fbf0.c (CreateJetpackPlane). */
const struct vtable_slot gJetpackPlaneVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackPlane),
    VTABLE_SLOT(UpdateJetpackPlane),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackPlane),
    VTABLE_SLOT(sub_802FF00),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part_2fbf0.c (CreateJetpackBomber). */
const struct vtable_slot gJetpackBomberVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackBomber),
    VTABLE_SLOT(UpdateJetpackBomber),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackBomber),
    VTABLE_SLOT(sub_8030290),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part_2fbf0.c (CreateJetpackCannonball). */
const struct vtable_slot gJetpackCannonballVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackCannonball),
    VTABLE_SLOT(UpdateJetpackCannonball),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(nullsub_44),
    VTABLE_SLOT(sub_8030330),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part20d.c. */
const struct vtable_slot gStaticData_087E525C[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B698),
    VTABLE_SLOT(sub_8030574),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(sub_8030530),
    VTABLE_SLOT(sub_80306A4),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part125.c. */
const struct vtable_slot gJetpackBalloonVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackBalloon),
    VTABLE_SLOT(UpdateJetpackBalloon),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackBalloon),
    VTABLE_SLOT(sub_8031A64),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part129.c. */
const struct vtable_slot gJetpackHealthCrateVtable[8] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackHealthCrate),
    VTABLE_SLOT(UpdateJetpackHealthCrate),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackHealthCrate),
    VTABLE_SLOT(sub_8032350),
    VTABLE_SLOT(GetActorHp),
    VTABLE_SLOT(sub_8032140),
};

/* Used by actor_part129.c. */
const struct vtable_slot gJetpackTimeCrateVtable[8] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackTimeCrate),
    VTABLE_SLOT(UpdateJetpackTimeCrate),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackTimeCrate),
    VTABLE_SLOT(sub_8032350),
    VTABLE_SLOT(GetActorHp),
    VTABLE_SLOT(sub_8032140),
};

/* Used by actor_part129.c. */
const struct vtable_slot gJetpackQuestionCrateVtable[8] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackQuestionCrate),
    VTABLE_SLOT(UpdateJetpackQuestionCrate),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackQuestionCrate),
    VTABLE_SLOT(sub_8032350),
    VTABLE_SLOT(GetActorHp),
    VTABLE_SLOT(sub_8032140),
};

/* Used by actor_part129.c. */
const struct vtable_slot gJetpackBalloonCrateVtable[8] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackBalloonCrate),
    VTABLE_SLOT(UpdateJetpackBalloonCrate),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackBalloonCrate),
    VTABLE_SLOT(sub_8032350),
    VTABLE_SLOT(GetActorHp),
    VTABLE_SLOT(sub_8032140),
};

/* Used by actor_part129.c (CreateJetpackParachuteNitro). */
const struct vtable_slot gJetpackParachuteNitroVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackParachuteNitro),
    VTABLE_SLOT(UpdateJetpackParachuteNitro),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackParachuteNitro),
    VTABLE_SLOT(sub_8032478),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part129.c. */
const struct vtable_slot gJetpackRocketVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackRocket),
    VTABLE_SLOT(UpdateJetpackRocket),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackRocket),
    VTABLE_SLOT(sub_8032680),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part130.c. */
const struct vtable_slot gJetpackRingVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackRing),
    VTABLE_SLOT(UpdateJetpackRing),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(nullsub_44),
    VTABLE_SLOT(sub_8032714),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part130.c (DestroyJetpackCollectedWumpa). */
const struct vtable_slot gJetpackCollectedWumpaVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackCollectedWumpa),
    VTABLE_SLOT(UpdateJetpackCollectedWumpa),
    VTABLE_SLOT(DrawJetpackCollectedWumpa),
    VTABLE_SLOT(nullsub_44),
    VTABLE_SLOT(sub_803290C),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part130.c. */
const struct vtable_slot gStaticData_087E54AC[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B7D4),
    VTABLE_SLOT(sub_8032950),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(sub_8032910),
    VTABLE_SLOT(sub_8032AF0),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part32.c. */
const struct vtable_slot gHovercraftCannonVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyHovercraftCannon),
    VTABLE_SLOT(UpdateHovercraftCannon),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageHovercraftCannon),
    VTABLE_SLOT(sub_8033CF0),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part63.c. */
const struct vtable_slot gHovercraftLauncherVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyHovercraftLauncher),
    VTABLE_SLOT(UpdateHovercraftLauncher),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageHovercraftLauncher),
    VTABLE_SLOT(sub_8034050),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part65.c, actor_part66.c (sub_8034058), actor_part67.c. */
const struct vtable_slot gStaticData_087E5554[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B858),
    VTABLE_SLOT(sub_8034188),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(sub_8034110),
    VTABLE_SLOT(sub_8034264),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part69.c. */
const struct vtable_slot gStaticData_087E558C[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B884),
    VTABLE_SLOT(sub_8034270),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(nullsub_38),
    VTABLE_SLOT(sub_803436C),
    VTABLE_SLOT(GetActorHp),
};

/* Used by counter_selector.c (DestroyLogoActor), graphics_loading_35780.c,
 * graphics_loading_35d1c.c, graphics_loading_3686c.c (LoadUniversalLogoBg). */
const struct vtable_slot gLogoActorVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyLogoActor),
    VTABLE_SLOT(UpdateLogoActor),
    VTABLE_SLOT(DrawLogoActor),
};
