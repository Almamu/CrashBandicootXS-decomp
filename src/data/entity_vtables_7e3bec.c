#include "core.h"
#include "vtable.h"
#include "text.h"
#include "pickups.h"
#include "enemies.h"

/*
 * ROM 0x087E3BEC-0x087E55E4: the 93 virtual tables of the game's C++
 * object classes (docs/rom_map.md's "93 entity vtables"), in ROM order.
 * Constructors store one at the object's method-table pointer (e.g.
 * `obj->table = gEntityVtable` in graphics.c, `self->vtable` in
 * gobj_1a794.h); the code reads the slots through `struct method` /
 * `struct actor_method`. Tables of the same class family share their
 * leading slots, the ROM's own inheritance. Linked in ROM order between
 * data/data.s sections by ldscript.txt - see docs/data.md.
 */

extern void DrawActor();
extern void DrawEntity();
extern void CtrlHandleEvent();
extern void EffectCtrlHandleEvent();
extern void nullsub_20();
extern void DamageHovercraftCannonFlash();
extern void DamageActor();
extern void UpdateCtrl();
extern void IsEntityNearCamera();
extern void CheckEntityPlayerContact();
extern void UpdateEntity();
extern void GetEntityBounds();
extern void EntityOverlapsRect();
extern void IsEntityOnScreen();
extern void IsEntityInsideRect();
extern void GetEntityClassId();
extern void DestroyEntity();
extern void CheckSpritePickup();
extern void IsSpriteObjOnScreen();
extern void SpriteObjOverlapsRect();
extern void IsSpriteObjInsideRect();
extern void IsSpriteObjNearCamera();
extern void ApplySpriteObjVelocity();
extern void DrawSpriteObj();
extern void UpdateSpriteObj();
extern void GetSpriteObjHitbox();
extern void GetSpriteObjPriority();
extern void GetSpriteObjClassId();
extern void DestroySpriteObj();
extern void GetSpritePriority();
extern void DestroyUiSpriteObj();
extern void CheckPlayerContact();
extern void ApplySpriteVelocity();
extern void GetMovingSpriteClassId();
extern void DestroyMovingSprite();
extern void UpdateMovingSprite();
extern void HitMovingSprite();
extern void CollideMovingSprite();
extern void CollideGroundSprite();
extern void UpdateGroundSprite();
extern void DrawGroundSprite();
extern void GetGroundSpriteClassId();
extern void DestroyGroundSprite();
extern void CollidePlayer();
extern void CollidePlayerWithObjects();
extern void PlayerHandleEvent();
extern void DrawPlayer();
extern void ApplyPlayerVelocity();
extern void UpdatePlayer();
extern void DestroyPlayer();
extern void SetCtrlMode();
extern void SetCtrlTargetMotionY();
extern void StartCtrlTargetMotionY();
extern void StartCtrlTargetMotionYFromSet();
extern void SetCtrlTargetMotionX();
extern void StartCtrlTargetMotionX();
extern void StartCtrlTargetMotionXFromSet();
extern void SetCtrlTargetAnim();
extern void AttachCtrl();
extern void DestroyCtrl();
extern void UpdateEffectCtrl();
extern void DestroyEffectCtrl();
extern void DrawCrate();
extern void UpdateCrate();
extern void IsCrateInsideRect();
extern void GetCrateClassId();
extern void DestroyCrate();
extern void ActionCtrlHandleEvent();
extern void UpdateActionCtrl();
extern void SetActionCtrlMode();
extern void AttachActionCtrl();
extern void ActionCtrlSetTargetAnim();
extern void DestroyActionCtrl();
extern void PlayerCtrlHandleEvent();
extern void UpdatePlayerCtrl();
extern void AttachPlayerCtrl();
extern void DestroyPlayerCtrl();
extern void UpdateInputCtrl();
extern void InputCtrlHandleEvent();
extern void AttachInputCtrl();
extern void DestroyInputCtrl();
extern void BossCtrlHandleEvent();
extern void DestroyBossCtrl();
extern void UpdateMegaMix();
extern void StartMegaMixMotionYFromSet();
extern void StartMegaMixMotionXFromSet();
extern void DestroyMegaMixCtrl();
extern void UpdateTiny();
extern void UpdateStompedHopPad();
extern void DestroyStompedHopPadCtrl();
extern void UpdateOneShotAnimCtrl();
extern void DestroyOneShotAnimCtrl();
extern void UpdateUnusedOneShotAnimCtrl();
extern void DestroyUnusedOneShotAnimCtrl();
extern void DestroyTiny();
extern void UpdateCortexBoss();
extern void UpdateCortexTarget();
extern void UpdateCortexShot();
extern void UpdateCortexBossPlatformMover();
extern void UpdateCortexBossGem();
extern void DestroyCortexBossGemCtrl();
extern void DestroyCortexBossPlatformMover();
extern void DestroyCortexShotCtrl();
extern void DestroyCortexTargetCtrl();
extern void UpdateCortexCannon();
extern void DestroyCortexCannonCtrl();
extern void DestroyCortexBoss();
extern void UpdateDingodile();
extern void UpdateDingodileShield();
extern void UpdateDingodileProjectile();
extern void UpdateDingodileShark();
extern void DestroyDingodileSharkCtrl();
extern void DestroyDingodileProjectileCtrl();
extern void DestroyDingodileShieldCtrl();
extern void DestroyDingodile();
extern void CheckPlatformContact();
extern void UpdatePlatform();
extern void GetPlatformClassId();
extern void DestroyPlatform();
extern void UpdatePlatformMover();
extern void StartPlatformMoverMotionYFromSet();
extern void StartPlatformMoverMotionXFromSet();
extern void DestroyPlatformMover();
extern void UpdateCameraLead();
extern void DestroyCameraLead();
extern void CheckLaunchPadContact();
extern void DestroyLaunchPad();
extern void AnimateLevelSelectEntry();
extern void SetLevelSelectEntryLevel();
extern void SetLevelSelectEntryPos();
extern void DestroyLevelSelectEntry();
extern void DestroyBgStreamer();
extern void DestroyBgLayerBase();
extern void ClampBgLayerScrollStep();
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
extern void ClampPooledBgLayerScrollStep();
extern void ClipPooledBgLayerColumns();
extern void ClipPooledBgLayerRows();
extern void DrawPooledBgLayerRow();
extern void ResetPooledBgLayer();
extern void LoadPooledBgLayerTiles();
extern void DestroyPooledBgLayer();
extern void sub_802710C();
extern void UpdateActor();
extern void DestroyActor();
extern void UpdatePolarPlayer();
extern void DrawPolarPlayer();
extern void DestroyPolarPlayer();
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
extern void UpdatePolarLauncher();
extern void UpdatePolarPenguin();
extern void UpdatePolarIcicle();
extern void UpdatePolarAkuAku();
extern void UpdatePolarGoal();
extern void UpdatePolarBoostPad();
extern void UpdatePolarCheckpointCrate();
extern void UpdateJetpackPlayer();
extern void DrawJetpackPlayer();
extern void DamageJetpackPlayer();
extern void GetJetpackPlayerHpPercent();
extern void DestroyJetpackPlayer();
extern void UpdateJetpackShot();
extern void IsJetpackShotUnshootable();
extern void UpdateJetpackPlane();
extern void DamageJetpackPlane();
extern void IsJetpackPlaneUnshootable();
extern void UpdateJetpackBomber();
extern void DamageJetpackBomber();
extern void IsJetpackBomberUnshootable();
extern void UpdateJetpackCannonball();
extern void IsJetpackCannonballUnshootable();
extern void DamageAirshipFireball();
extern void UpdateAirshipFireball();
extern void IsAirshipFireballUnshootable();
extern void UpdateJetpackBalloon();
extern void DamageJetpackBalloon();
extern void IsJetpackBalloonUnshootable();
extern void UpdateJetpackBalloonCrate();
extern void UpdateJetpackQuestionCrate();
extern void DamageJetpackQuestionCrate();
extern void UpdateJetpackHealthCrate();
extern void UpdateJetpackTimeCrate();
extern void DamageJetpackTimeCrate();
extern void DamageJetpackHealthCrate();
extern void BreakJetpackBalloonCrate();
extern void DamageJetpackBalloonCrate();
extern void DestroyJetpackBalloonCrate();
extern void IsJetpackBalloonCrateUnshootable();
extern void UpdateJetpackParachuteNitro();
extern void DamageJetpackParachuteNitro();
extern void IsJetpackParachuteNitroUnshootable();
extern void UpdateJetpackRocket();
extern void DamageJetpackRocket();
extern void IsJetpackRocketUnshootable();
extern void UpdateJetpackRing();
extern void IsJetpackRingUnshootable();
extern void UpdateJetpackCollectedWumpa();
extern void DrawJetpackCollectedWumpa();
extern void DestroyJetpackCollectedWumpa();
extern void IsJetpackCollectedWumpaUnshootable();
extern void DamageHovercraftFireball();
extern void UpdateHovercraftFireball();
extern void IsHovercraftFireballUnshootable();
extern void DamageHovercraftCannon();
extern void UpdateHovercraftCannon();
extern void IsHovercraftCannonUnshootable();
extern void DamageHovercraftLauncher();
extern void UpdateHovercraftLauncher();
extern void IsHovercraftLauncherUnshootable();
extern void DamageHovercraftSideGun();
extern void UpdateHovercraftSideGun();
extern void IsHovercraftSideGunUnshootable();
extern void UpdateHovercraftCannonFlash();
extern void IsHovercraftCannonFlashUnshootable();
extern void UpdateLogoActor();
extern void DrawLogoActor();
extern void DestroyLogoActor();
extern void DestroyRiderlessPolar();
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
extern void DestroyPolarLauncher();
extern void DestroyPolarPenguin();
extern void DestroyPolarIcicle();
extern void DestroyPolarAkuAku();
extern void DestroyPolarGoal();
extern void DestroyPolarBoostPad();
extern void DestroyPolarCheckpointCrate();
extern void DrawJetpackCheckpointText();
extern void UpdateJetpackCheckpointText();
extern void IsJetpackCheckpointTextUnshootable();
extern void DestroyJetpackCheckpointText();
extern void UpdateJetpackExplosion();
extern void IsJetpackExplosionUnshootable();
extern void DestroyJetpackExplosion();
extern void GetActorHp();
extern void IsJetpackPlayerUnshootable();
extern void DestroyJetpackShot();
extern void DestroyJetpackPlane();
extern void DestroyJetpackBomber();
extern void DestroyJetpackCannonball();
extern void DestroyAirshipFireball();
extern void DestroyJetpackBalloon();
extern void DestroyJetpackHealthCrate();
extern void DestroyJetpackTimeCrate();
extern void DestroyJetpackQuestionCrate();
extern void DestroyJetpackParachuteNitro();
extern void DestroyJetpackRocket();
extern void DestroyJetpackRing();
extern void DestroyHovercraftFireball();
extern void DestroyHovercraftCannon();
extern void DestroyHovercraftLauncher();
extern void DestroyHovercraftSideGun();
extern void DestroyHovercraftCannonFlash();

/* Used by aabb_setup.c, enemy_ctrl.c, wumpa.c,
 * sprite_obj.c (DestroySpriteObj), sprite_anim.c, graphics.c (nullsub_12,
 * ResetEntity, DestroyEntity). */
const struct vtable_slot gEntityVtable[11] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(CheckEntityPlayerContact),
    VTABLE_SLOT(GetEntityBounds),
    VTABLE_SLOT(UpdateEntity),
    VTABLE_SLOT(DrawEntity),
    VTABLE_SLOT(IsEntityOnScreen),
    VTABLE_SLOT(EntityOverlapsRect),
    VTABLE_SLOT(IsEntityNearCamera),
    VTABLE_SLOT(IsEntityInsideRect),
    VTABLE_SLOT(GetEntityClassId),
    VTABLE_SLOT(DestroyEntity),
};

/* Used by sprite_obj.c (GetSpriteObjPriority, DestroySpriteObj), sprite_anim.c. */
const struct vtable_slot gSpriteObjVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(CheckSpritePickup),
    VTABLE_SLOT(GetSpriteObjHitbox),
    VTABLE_SLOT(UpdateSpriteObj),
    VTABLE_SLOT(DrawSpriteObj),
    VTABLE_SLOT(IsSpriteObjOnScreen),
    VTABLE_SLOT(SpriteObjOverlapsRect),
    VTABLE_SLOT(IsSpriteObjNearCamera),
    VTABLE_SLOT(IsSpriteObjInsideRect),
    VTABLE_SLOT(GetSpriteObjClassId),
    VTABLE_SLOT(DestroySpriteObj),
    VTABLE_SLOT(GetSpriteObjPriority),
    VTABLE_SLOT(ApplySpriteObjVelocity),
};

/* Used by sprite_anim.c (DestroyUiSpriteObj). */
const struct vtable_slot gUiSpriteObjVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(CheckSpritePickup),
    VTABLE_SLOT(GetSpriteObjHitbox),
    VTABLE_SLOT(UpdateSpriteObj),
    VTABLE_SLOT(DrawSpriteObj),
    VTABLE_SLOT(IsSpriteObjOnScreen),
    VTABLE_SLOT(SpriteObjOverlapsRect),
    VTABLE_SLOT(IsSpriteObjNearCamera),
    VTABLE_SLOT(IsSpriteObjInsideRect),
    VTABLE_SLOT(GetSpriteObjClassId),
    VTABLE_SLOT(DestroyUiSpriteObj),
    VTABLE_SLOT(GetSpritePriority),
    VTABLE_SLOT(ApplySpriteObjVelocity),
};

/* Used by ground_sprite.c, moving_sprite.c (GetMovingSpriteClassId, DestroyMovingSprite,
 * ResetMovingSprite). */
const struct vtable_slot gMovingSpriteVtable[15] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(CollideMovingSprite),
    VTABLE_SLOT(GetSpriteObjHitbox),
    VTABLE_SLOT(UpdateMovingSprite),
    VTABLE_SLOT(DrawSpriteObj),
    VTABLE_SLOT(IsSpriteObjOnScreen),
    VTABLE_SLOT(SpriteObjOverlapsRect),
    VTABLE_SLOT(IsSpriteObjNearCamera),
    VTABLE_SLOT(IsSpriteObjInsideRect),
    VTABLE_SLOT(GetMovingSpriteClassId),
    VTABLE_SLOT(DestroyMovingSprite),
    VTABLE_SLOT(GetSpriteObjPriority),
    VTABLE_SLOT(ApplySpriteVelocity),
    VTABLE_SLOT(HitMovingSprite),
    VTABLE_SLOT(CheckPlayerContact),
};

/* Used by ground_sprite.c (GetGroundSpriteClassId, DestroyGroundSprite, ResetGroundSprite). */
const struct vtable_slot gGroundSpriteVtable[15] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(CollideGroundSprite),
    VTABLE_SLOT(GetSpriteObjHitbox),
    VTABLE_SLOT(UpdateGroundSprite),
    VTABLE_SLOT(DrawGroundSprite),
    VTABLE_SLOT(IsSpriteObjOnScreen),
    VTABLE_SLOT(SpriteObjOverlapsRect),
    VTABLE_SLOT(IsSpriteObjNearCamera),
    VTABLE_SLOT(IsSpriteObjInsideRect),
    VTABLE_SLOT(GetGroundSpriteClassId),
    VTABLE_SLOT(DestroyGroundSprite),
    VTABLE_SLOT(GetSpriteObjPriority),
    VTABLE_SLOT(ApplySpriteVelocity),
    VTABLE_SLOT(HitMovingSprite),
    VTABLE_SLOT(CheckPlayerContact),
};

/* Used by player_update.c, player_init.c. */
const struct vtable_slot gPlayerVtable[15] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(CollidePlayer),
    VTABLE_SLOT(GetSpriteObjHitbox),
    VTABLE_SLOT(UpdatePlayer),
    VTABLE_SLOT(DrawPlayer),
    VTABLE_SLOT(IsSpriteObjOnScreen),
    VTABLE_SLOT(SpriteObjOverlapsRect),
    VTABLE_SLOT(IsSpriteObjNearCamera),
    VTABLE_SLOT(IsSpriteObjInsideRect),
    VTABLE_SLOT(GetGroundSpriteClassId),
    VTABLE_SLOT(DestroyPlayer),
    VTABLE_SLOT(GetSpriteObjPriority),
    VTABLE_SLOT(ApplyPlayerVelocity),
    VTABLE_SLOT(PlayerHandleEvent),
    VTABLE_SLOT(CollidePlayerWithObjects),
};

/* Used by enemy_ctrl.c, ctrl.c, input_ctrl_queue.c, action_ctrl.c. */
const struct vtable_slot gCtrlVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateCtrl),
    VTABLE_SLOT(CtrlHandleEvent),
    VTABLE_SLOT(AttachCtrl),
    VTABLE_SLOT(SetCtrlMode),
    VTABLE_SLOT(StartCtrlTargetMotionX),
    VTABLE_SLOT(StartCtrlTargetMotionY),
    VTABLE_SLOT(SetCtrlTargetMotionX),
    VTABLE_SLOT(SetCtrlTargetMotionY),
    VTABLE_SLOT(DestroyCtrl),
    VTABLE_SLOT(SetCtrlTargetAnim),
    VTABLE_SLOT(StartCtrlTargetMotionXFromSet),
    VTABLE_SLOT(StartCtrlTargetMotionYFromSet),
};

/* Used by enemy_ctrl_update.c, enemy_ctrl.c. */
const struct vtable_slot gEnemyCtrlVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateEnemyCtrl),
    VTABLE_SLOT(HitEnemy),
    VTABLE_SLOT(AttachEnemyCtrl),
    VTABLE_SLOT(SetCtrlMode),
    VTABLE_SLOT(StartCtrlTargetMotionX),
    VTABLE_SLOT(StartCtrlTargetMotionY),
    VTABLE_SLOT(SetCtrlTargetMotionX),
    VTABLE_SLOT(SetCtrlTargetMotionY),
    VTABLE_SLOT(DestroyEnemyCtrl),
    VTABLE_SLOT(SetCtrlTargetAnim),
    VTABLE_SLOT(StartCtrlTargetMotionXFromSet),
    VTABLE_SLOT(StartCtrlTargetMotionYFromSet),
};

/* Used by enemy_ctrl.c. */
const struct vtable_slot gPeriodicSpawnerVtable[11] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(CheckEntityPlayerContact),
    VTABLE_SLOT(GetEntityBounds),
    VTABLE_SLOT(UpdatePeriodicSpawner),
    VTABLE_SLOT(DrawEntity),
    VTABLE_SLOT(IsEntityOnScreen),
    VTABLE_SLOT(EntityOverlapsRect),
    VTABLE_SLOT(IsEntityNearCamera),
    VTABLE_SLOT(IsEntityInsideRect),
    VTABLE_SLOT(GetEntityClassId),
    VTABLE_SLOT(DestroyPeriodicSpawner),
};

/* Used by enemy_ctrl_update.c, enemy_ctrl.c. */
const struct vtable_slot gKnockedEnemyCtrlVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateKnockedEnemyCtrl),
    VTABLE_SLOT(CtrlHandleEvent),
    VTABLE_SLOT(AttachCtrl),
    VTABLE_SLOT(SetCtrlMode),
    VTABLE_SLOT(StartCtrlTargetMotionX),
    VTABLE_SLOT(StartCtrlTargetMotionY),
    VTABLE_SLOT(SetCtrlTargetMotionX),
    VTABLE_SLOT(SetCtrlTargetMotionY),
    VTABLE_SLOT(DestroyKnockedEnemyCtrl),
    VTABLE_SLOT(SetCtrlTargetAnim),
    VTABLE_SLOT(StartCtrlTargetMotionXFromSet),
    VTABLE_SLOT(StartCtrlTargetMotionYFromSet),
};

/* Used by effect_ctrl.c. */
const struct vtable_slot gEffectCtrlVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateEffectCtrl),
    VTABLE_SLOT(EffectCtrlHandleEvent),
    VTABLE_SLOT(AttachCtrl),
    VTABLE_SLOT(SetCtrlMode),
    VTABLE_SLOT(StartCtrlTargetMotionX),
    VTABLE_SLOT(StartCtrlTargetMotionY),
    VTABLE_SLOT(SetCtrlTargetMotionX),
    VTABLE_SLOT(SetCtrlTargetMotionY),
    VTABLE_SLOT(DestroyEffectCtrl),
    VTABLE_SLOT(SetCtrlTargetAnim),
    VTABLE_SLOT(StartCtrlTargetMotionXFromSet),
    VTABLE_SLOT(StartCtrlTargetMotionYFromSet),
};

/* Used by crate.c (DestroyCrate), crate_create.c (CreateCrate). */
const struct vtable_slot gCrateVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(CheckSpritePickup),
    VTABLE_SLOT(GetSpriteObjHitbox),
    VTABLE_SLOT(UpdateCrate),
    VTABLE_SLOT(DrawCrate),
    VTABLE_SLOT(IsSpriteObjOnScreen),
    VTABLE_SLOT(SpriteObjOverlapsRect),
    VTABLE_SLOT(IsSpriteObjNearCamera),
    VTABLE_SLOT(IsCrateInsideRect),
    VTABLE_SLOT(GetCrateClassId),
    VTABLE_SLOT(DestroyCrate),
    VTABLE_SLOT(GetSpriteObjPriority),
    VTABLE_SLOT(ApplySpriteObjVelocity),
};

/* Used by extra_life.c (UpdateExtraLife). */
const struct vtable_slot gExtraLifeVtable[14] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(CollideExtraLife),
    VTABLE_SLOT(GetSpriteObjHitbox),
    VTABLE_SLOT(UpdateExtraLife),
    VTABLE_SLOT(DrawExtraLife),
    VTABLE_SLOT(IsSpriteObjOnScreen),
    VTABLE_SLOT(SpriteObjOverlapsRect),
    VTABLE_SLOT(IsSpriteObjNearCamera),
    VTABLE_SLOT(IsSpriteObjInsideRect),
    VTABLE_SLOT(GetExtraLifeClassId),
    VTABLE_SLOT(DestroyExtraLife),
    VTABLE_SLOT(GetSpriteObjPriority),
    VTABLE_SLOT(ApplySpriteObjVelocity),
    VTABLE_SLOT(CheckExtraLifePickup),
};

/* Used by wumpa.c (DestroyWumpa, ResetWumpaPickup), wumpa_update.c
 * (UpdateWumpa). */
const struct vtable_slot gWumpaVtable[14] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(CollideWumpa),
    VTABLE_SLOT(GetSpriteObjHitbox),
    VTABLE_SLOT(UpdateWumpa),
    VTABLE_SLOT(DrawWumpa),
    VTABLE_SLOT(IsSpriteObjOnScreen),
    VTABLE_SLOT(SpriteObjOverlapsRect),
    VTABLE_SLOT(IsSpriteObjNearCamera),
    VTABLE_SLOT(IsSpriteObjInsideRect),
    VTABLE_SLOT(GetWumpaClassId),
    VTABLE_SLOT(DestroyWumpa),
    VTABLE_SLOT(GetSpriteObjPriority),
    VTABLE_SLOT(ApplySpriteObjVelocity),
    VTABLE_SLOT(CheckWumpaPickup),
};

/* Used by wumpa.c (UpdateStopwatch, DestroyStopwatch). */
const struct vtable_slot gStopwatchVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(CheckSpritePickup),
    VTABLE_SLOT(GetSpriteObjHitbox),
    VTABLE_SLOT(UpdateStopwatch),
    VTABLE_SLOT(DrawSpriteObj),
    VTABLE_SLOT(IsSpriteObjOnScreen),
    VTABLE_SLOT(SpriteObjOverlapsRect),
    VTABLE_SLOT(IsSpriteObjNearCamera),
    VTABLE_SLOT(IsSpriteObjInsideRect),
    VTABLE_SLOT(GetSpriteObjClassId),
    VTABLE_SLOT(DestroyStopwatch),
    VTABLE_SLOT(GetSpriteObjPriority),
    VTABLE_SLOT(ApplySpriteObjVelocity),
};

/* Used by wumpa.c, action_ctrl.c. */
const struct vtable_slot gActionCtrlVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateActionCtrl),
    VTABLE_SLOT(ActionCtrlHandleEvent),
    VTABLE_SLOT(AttachActionCtrl),
    VTABLE_SLOT(SetActionCtrlMode),
    VTABLE_SLOT(StartCtrlTargetMotionX),
    VTABLE_SLOT(StartCtrlTargetMotionY),
    VTABLE_SLOT(SetCtrlTargetMotionX),
    VTABLE_SLOT(SetCtrlTargetMotionY),
    VTABLE_SLOT(DestroyActionCtrl),
    VTABLE_SLOT(ActionCtrlSetTargetAnim),
    VTABLE_SLOT(StartCtrlTargetMotionXFromSet),
    VTABLE_SLOT(StartCtrlTargetMotionYFromSet),
};

/* Used by swim_ctrl.c (DestroyPlayerCtrl), player_ctrl.h. */
const struct vtable_slot gPlayerCtrlVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdatePlayerCtrl),
    VTABLE_SLOT(PlayerCtrlHandleEvent),
    VTABLE_SLOT(AttachPlayerCtrl),
    VTABLE_SLOT(SetCtrlMode),
    VTABLE_SLOT(StartCtrlTargetMotionX),
    VTABLE_SLOT(StartCtrlTargetMotionY),
    VTABLE_SLOT(SetCtrlTargetMotionX),
    VTABLE_SLOT(SetCtrlTargetMotionY),
    VTABLE_SLOT(DestroyPlayerCtrl),
    VTABLE_SLOT(SetCtrlTargetAnim),
    VTABLE_SLOT(StartCtrlTargetMotionXFromSet),
    VTABLE_SLOT(StartCtrlTargetMotionYFromSet),
};

/* Used by input_ctrl.c (DestroyInputCtrl). */
const struct vtable_slot gInputCtrlVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateInputCtrl),
    VTABLE_SLOT(InputCtrlHandleEvent),
    VTABLE_SLOT(AttachInputCtrl),
    VTABLE_SLOT(SetCtrlMode),
    VTABLE_SLOT(StartCtrlTargetMotionX),
    VTABLE_SLOT(StartCtrlTargetMotionY),
    VTABLE_SLOT(SetCtrlTargetMotionX),
    VTABLE_SLOT(SetCtrlTargetMotionY),
    VTABLE_SLOT(DestroyInputCtrl),
    VTABLE_SLOT(SetCtrlTargetAnim),
    VTABLE_SLOT(StartCtrlTargetMotionXFromSet),
    VTABLE_SLOT(StartCtrlTargetMotionYFromSet),
};

/* Used by input_ctrl_queue.c. The bosses' controller base class: Mega-Mix,
 * Tiny, the Neo Cortex fight's controller, Dingodile and his shield and
 * rocket/stalactite derive from it and keep its event slot
 * (BossCtrlHandleEvent). */
const struct vtable_slot gBossCtrlVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateCtrl),
    VTABLE_SLOT(BossCtrlHandleEvent),
    VTABLE_SLOT(AttachCtrl),
    VTABLE_SLOT(SetCtrlMode),
    VTABLE_SLOT(StartCtrlTargetMotionX),
    VTABLE_SLOT(StartCtrlTargetMotionY),
    VTABLE_SLOT(SetCtrlTargetMotionX),
    VTABLE_SLOT(SetCtrlTargetMotionY),
    VTABLE_SLOT(DestroyBossCtrl),
    VTABLE_SLOT(SetCtrlTargetAnim),
    VTABLE_SLOT(StartCtrlTargetMotionXFromSet),
    VTABLE_SLOT(StartCtrlTargetMotionYFromSet),
};

/* Used by mega_mix.c. */
const struct vtable_slot gMegaMixCtrlVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateMegaMix),
    VTABLE_SLOT(BossCtrlHandleEvent),
    VTABLE_SLOT(AttachCtrl),
    VTABLE_SLOT(SetCtrlMode),
    VTABLE_SLOT(StartCtrlTargetMotionX),
    VTABLE_SLOT(StartCtrlTargetMotionY),
    VTABLE_SLOT(SetCtrlTargetMotionX),
    VTABLE_SLOT(SetCtrlTargetMotionY),
    VTABLE_SLOT(DestroyMegaMixCtrl),
    VTABLE_SLOT(SetCtrlTargetAnim),
    VTABLE_SLOT(StartMegaMixMotionXFromSet),
    VTABLE_SLOT(StartMegaMixMotionYFromSet),
};

/* Used by tiny_hop_pad.c. */
const struct vtable_slot gStompedHopPadVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateStompedHopPad),
    VTABLE_SLOT(CtrlHandleEvent),
    VTABLE_SLOT(AttachCtrl),
    VTABLE_SLOT(SetCtrlMode),
    VTABLE_SLOT(StartCtrlTargetMotionX),
    VTABLE_SLOT(StartCtrlTargetMotionY),
    VTABLE_SLOT(SetCtrlTargetMotionX),
    VTABLE_SLOT(SetCtrlTargetMotionY),
    VTABLE_SLOT(DestroyStompedHopPadCtrl),
    VTABLE_SLOT(SetCtrlTargetAnim),
    VTABLE_SLOT(StartCtrlTargetMotionXFromSet),
    VTABLE_SLOT(StartCtrlTargetMotionYFromSet),
};

/* Used by cortex.c (CreateOneShotAnimCtrl, DestroyOneShotAnimCtrl). */
const struct vtable_slot gOneShotAnimCtrlVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateOneShotAnimCtrl),
    VTABLE_SLOT(CtrlHandleEvent),
    VTABLE_SLOT(AttachCtrl),
    VTABLE_SLOT(SetCtrlMode),
    VTABLE_SLOT(StartCtrlTargetMotionX),
    VTABLE_SLOT(StartCtrlTargetMotionY),
    VTABLE_SLOT(SetCtrlTargetMotionX),
    VTABLE_SLOT(SetCtrlTargetMotionY),
    VTABLE_SLOT(DestroyOneShotAnimCtrl),
    VTABLE_SLOT(SetCtrlTargetAnim),
    VTABLE_SLOT(StartCtrlTargetMotionXFromSet),
    VTABLE_SLOT(StartCtrlTargetMotionYFromSet),
};

/* Used by cortex.c (CreateUnusedOneShotAnimCtrl, DestroyUnusedOneShotAnimCtrl). */
const struct vtable_slot gUnusedOneShotAnimCtrlVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateUnusedOneShotAnimCtrl),
    VTABLE_SLOT(CtrlHandleEvent),
    VTABLE_SLOT(AttachCtrl),
    VTABLE_SLOT(SetCtrlMode),
    VTABLE_SLOT(StartCtrlTargetMotionX),
    VTABLE_SLOT(StartCtrlTargetMotionY),
    VTABLE_SLOT(SetCtrlTargetMotionX),
    VTABLE_SLOT(SetCtrlTargetMotionY),
    VTABLE_SLOT(DestroyUnusedOneShotAnimCtrl),
    VTABLE_SLOT(SetCtrlTargetAnim),
    VTABLE_SLOT(StartCtrlTargetMotionXFromSet),
    VTABLE_SLOT(StartCtrlTargetMotionYFromSet),
};

/* Used by tiny_update.c, cortex.c (DestroyTiny,
 * CreateTiny). */
const struct vtable_slot gTinyVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateTiny),
    VTABLE_SLOT(BossCtrlHandleEvent),
    VTABLE_SLOT(AttachCtrl),
    VTABLE_SLOT(SetCtrlMode),
    VTABLE_SLOT(StartCtrlTargetMotionX),
    VTABLE_SLOT(StartCtrlTargetMotionY),
    VTABLE_SLOT(SetCtrlTargetMotionX),
    VTABLE_SLOT(SetCtrlTargetMotionY),
    VTABLE_SLOT(DestroyTiny),
    VTABLE_SLOT(SetCtrlTargetAnim),
    VTABLE_SLOT(StartCtrlTargetMotionXFromSet),
    VTABLE_SLOT(StartCtrlTargetMotionYFromSet),
};

/* Used by cortex.c (DestroyCortexBossGemCtrl, CreateCortexBossGemCtrl). */
const struct vtable_slot gCortexBossGemVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateCortexBossGem),
    VTABLE_SLOT(CtrlHandleEvent),
    VTABLE_SLOT(AttachCtrl),
    VTABLE_SLOT(SetCtrlMode),
    VTABLE_SLOT(StartCtrlTargetMotionX),
    VTABLE_SLOT(StartCtrlTargetMotionY),
    VTABLE_SLOT(SetCtrlTargetMotionX),
    VTABLE_SLOT(SetCtrlTargetMotionY),
    VTABLE_SLOT(DestroyCortexBossGemCtrl),
    VTABLE_SLOT(SetCtrlTargetAnim),
    VTABLE_SLOT(StartCtrlTargetMotionXFromSet),
    VTABLE_SLOT(StartCtrlTargetMotionYFromSet),
};

/* Used by cortex.c (DestroyCortexBossPlatformMover, CreateCortexBossPlatformMover). */
const struct vtable_slot gCortexBossPlatformMoverVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateCortexBossPlatformMover),
    VTABLE_SLOT(CtrlHandleEvent),
    VTABLE_SLOT(AttachCtrl),
    VTABLE_SLOT(SetCtrlMode),
    VTABLE_SLOT(StartCtrlTargetMotionX),
    VTABLE_SLOT(StartCtrlTargetMotionY),
    VTABLE_SLOT(SetCtrlTargetMotionX),
    VTABLE_SLOT(SetCtrlTargetMotionY),
    VTABLE_SLOT(DestroyCortexBossPlatformMover),
    VTABLE_SLOT(SetCtrlTargetAnim),
    VTABLE_SLOT(StartPlatformMoverMotionXFromSet),
    VTABLE_SLOT(StartPlatformMoverMotionYFromSet),
};

/* Used by cortex.c (DestroyCortexShotCtrl, CreateCortexShotCtrl). */
const struct vtable_slot gCortexShotVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateCortexShot),
    VTABLE_SLOT(CtrlHandleEvent),
    VTABLE_SLOT(AttachCtrl),
    VTABLE_SLOT(SetCtrlMode),
    VTABLE_SLOT(StartCtrlTargetMotionX),
    VTABLE_SLOT(StartCtrlTargetMotionY),
    VTABLE_SLOT(SetCtrlTargetMotionX),
    VTABLE_SLOT(SetCtrlTargetMotionY),
    VTABLE_SLOT(DestroyCortexShotCtrl),
    VTABLE_SLOT(SetCtrlTargetAnim),
    VTABLE_SLOT(StartCtrlTargetMotionXFromSet),
    VTABLE_SLOT(StartCtrlTargetMotionYFromSet),
};

/* Used by dingodile.c (DestroyCortexTargetCtrl). */
const struct vtable_slot gCortexTargetVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateCortexTarget),
    VTABLE_SLOT(CtrlHandleEvent),
    VTABLE_SLOT(AttachCtrl),
    VTABLE_SLOT(SetCtrlMode),
    VTABLE_SLOT(StartCtrlTargetMotionX),
    VTABLE_SLOT(StartCtrlTargetMotionY),
    VTABLE_SLOT(SetCtrlTargetMotionX),
    VTABLE_SLOT(SetCtrlTargetMotionY),
    VTABLE_SLOT(DestroyCortexTargetCtrl),
    VTABLE_SLOT(SetCtrlTargetAnim),
    VTABLE_SLOT(StartCtrlTargetMotionXFromSet),
    VTABLE_SLOT(StartCtrlTargetMotionYFromSet),
};

/* Used by dingodile.c (DestroyCortexCannonCtrl). */
const struct vtable_slot gCortexCannonVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateCortexCannon),
    VTABLE_SLOT(CtrlHandleEvent),
    VTABLE_SLOT(AttachCtrl),
    VTABLE_SLOT(SetCtrlMode),
    VTABLE_SLOT(StartCtrlTargetMotionX),
    VTABLE_SLOT(StartCtrlTargetMotionY),
    VTABLE_SLOT(SetCtrlTargetMotionX),
    VTABLE_SLOT(SetCtrlTargetMotionY),
    VTABLE_SLOT(DestroyCortexCannonCtrl),
    VTABLE_SLOT(SetCtrlTargetAnim),
    VTABLE_SLOT(StartCtrlTargetMotionXFromSet),
    VTABLE_SLOT(StartCtrlTargetMotionYFromSet),
};

/* Used by dingodile.c (DestroyCortexBoss). */
const struct vtable_slot gCortexBossVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateCortexBoss),
    VTABLE_SLOT(BossCtrlHandleEvent),
    VTABLE_SLOT(AttachCtrl),
    VTABLE_SLOT(SetCtrlMode),
    VTABLE_SLOT(StartCtrlTargetMotionX),
    VTABLE_SLOT(StartCtrlTargetMotionY),
    VTABLE_SLOT(SetCtrlTargetMotionX),
    VTABLE_SLOT(SetCtrlTargetMotionY),
    VTABLE_SLOT(DestroyCortexBoss),
    VTABLE_SLOT(SetCtrlTargetAnim),
    VTABLE_SLOT(StartCtrlTargetMotionXFromSet),
    VTABLE_SLOT(StartCtrlTargetMotionYFromSet),
};

/* Used by dingodile.c (UpdateDingodileShark, DestroyDingodileSharkCtrl). */
const struct vtable_slot gDingodileSharkVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateDingodileShark),
    VTABLE_SLOT(HitEnemy),
    VTABLE_SLOT(AttachEnemyCtrl),
    VTABLE_SLOT(SetCtrlMode),
    VTABLE_SLOT(StartCtrlTargetMotionX),
    VTABLE_SLOT(StartCtrlTargetMotionY),
    VTABLE_SLOT(SetCtrlTargetMotionX),
    VTABLE_SLOT(SetCtrlTargetMotionY),
    VTABLE_SLOT(DestroyDingodileSharkCtrl),
    VTABLE_SLOT(SetCtrlTargetAnim),
    VTABLE_SLOT(StartCtrlTargetMotionXFromSet),
    VTABLE_SLOT(StartCtrlTargetMotionYFromSet),
};

/* Used by dingodile.c (SpawnDingodileStalactite, DestroyDingodileProjectileCtrl). */
const struct vtable_slot gDingodileProjectileVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateDingodileProjectile),
    VTABLE_SLOT(BossCtrlHandleEvent),
    VTABLE_SLOT(AttachCtrl),
    VTABLE_SLOT(SetCtrlMode),
    VTABLE_SLOT(StartCtrlTargetMotionX),
    VTABLE_SLOT(StartCtrlTargetMotionY),
    VTABLE_SLOT(SetCtrlTargetMotionX),
    VTABLE_SLOT(SetCtrlTargetMotionY),
    VTABLE_SLOT(DestroyDingodileProjectileCtrl),
    VTABLE_SLOT(SetCtrlTargetAnim),
    VTABLE_SLOT(StartCtrlTargetMotionXFromSet),
    VTABLE_SLOT(StartCtrlTargetMotionYFromSet),
};

/* Used by dingodile.c (DestroyDingodileShieldCtrl), dingodile_create.c,
 * gobj_1a794.h. */
const struct vtable_slot gDingodileShieldVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateDingodileShield),
    VTABLE_SLOT(BossCtrlHandleEvent),
    VTABLE_SLOT(AttachCtrl),
    VTABLE_SLOT(SetCtrlMode),
    VTABLE_SLOT(StartCtrlTargetMotionX),
    VTABLE_SLOT(StartCtrlTargetMotionY),
    VTABLE_SLOT(SetCtrlTargetMotionX),
    VTABLE_SLOT(SetCtrlTargetMotionY),
    VTABLE_SLOT(DestroyDingodileShieldCtrl),
    VTABLE_SLOT(SetCtrlTargetAnim),
    VTABLE_SLOT(StartCtrlTargetMotionXFromSet),
    VTABLE_SLOT(StartCtrlTargetMotionYFromSet),
};

/* Used by dingodile.c, dingodile_create.c (DestroyDingodile),
 * gobj_1a794.h. */
const struct vtable_slot gDingodileVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateDingodile),
    VTABLE_SLOT(BossCtrlHandleEvent),
    VTABLE_SLOT(AttachCtrl),
    VTABLE_SLOT(SetCtrlMode),
    VTABLE_SLOT(StartCtrlTargetMotionX),
    VTABLE_SLOT(StartCtrlTargetMotionY),
    VTABLE_SLOT(SetCtrlTargetMotionX),
    VTABLE_SLOT(SetCtrlTargetMotionY),
    VTABLE_SLOT(DestroyDingodile),
    VTABLE_SLOT(SetCtrlTargetAnim),
    VTABLE_SLOT(StartCtrlTargetMotionXFromSet),
    VTABLE_SLOT(StartCtrlTargetMotionYFromSet),
};

/* Used by platform.c (DestroyPlatform), gobj_1a794.h. */
const struct vtable_slot gPlatformVtable[15] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(CheckPlatformContact),
    VTABLE_SLOT(GetSpriteObjHitbox),
    VTABLE_SLOT(UpdatePlatform),
    VTABLE_SLOT(DrawSpriteObj),
    VTABLE_SLOT(IsSpriteObjOnScreen),
    VTABLE_SLOT(SpriteObjOverlapsRect),
    VTABLE_SLOT(IsSpriteObjNearCamera),
    VTABLE_SLOT(IsSpriteObjInsideRect),
    VTABLE_SLOT(GetPlatformClassId),
    VTABLE_SLOT(DestroyPlatform),
    VTABLE_SLOT(GetSpriteObjPriority),
    VTABLE_SLOT(ApplySpriteVelocity),
    VTABLE_SLOT(HitMovingSprite),
    VTABLE_SLOT(CheckPlayerContact),
};

/* Used by platform.c (DestroyPlatformMover), gobj_1a794.h. */
const struct vtable_slot gPlatformMoverVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdatePlatformMover),
    VTABLE_SLOT(CtrlHandleEvent),
    VTABLE_SLOT(AttachCtrl),
    VTABLE_SLOT(SetCtrlMode),
    VTABLE_SLOT(StartCtrlTargetMotionX),
    VTABLE_SLOT(StartCtrlTargetMotionY),
    VTABLE_SLOT(SetCtrlTargetMotionX),
    VTABLE_SLOT(SetCtrlTargetMotionY),
    VTABLE_SLOT(DestroyPlatformMover),
    VTABLE_SLOT(SetCtrlTargetAnim),
    VTABLE_SLOT(StartPlatformMoverMotionXFromSet),
    VTABLE_SLOT(StartPlatformMoverMotionYFromSet),
};

/* Used by level_select.c (DestroyCameraLead). */
const struct vtable_slot gCameraLeadVtable[15] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(CollideMovingSprite),
    VTABLE_SLOT(GetSpriteObjHitbox),
    VTABLE_SLOT(UpdateCameraLead),
    VTABLE_SLOT(DrawSpriteObj),
    VTABLE_SLOT(IsSpriteObjOnScreen),
    VTABLE_SLOT(SpriteObjOverlapsRect),
    VTABLE_SLOT(IsSpriteObjNearCamera),
    VTABLE_SLOT(IsSpriteObjInsideRect),
    VTABLE_SLOT(GetMovingSpriteClassId),
    VTABLE_SLOT(DestroyCameraLead),
    VTABLE_SLOT(GetSpriteObjPriority),
    VTABLE_SLOT(ApplySpriteVelocity),
    VTABLE_SLOT(HitMovingSprite),
    VTABLE_SLOT(CheckPlayerContact),
};

/* Used by level_select.c (GetCameraLeadOffset, DestroyLaunchPad, sub_801BAC4). */
const struct vtable_slot gLaunchPadVtable[15] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(CollideMovingSprite),
    VTABLE_SLOT(GetSpriteObjHitbox),
    VTABLE_SLOT(UpdateMovingSprite),
    VTABLE_SLOT(DrawSpriteObj),
    VTABLE_SLOT(IsSpriteObjOnScreen),
    VTABLE_SLOT(SpriteObjOverlapsRect),
    VTABLE_SLOT(IsSpriteObjNearCamera),
    VTABLE_SLOT(IsSpriteObjInsideRect),
    VTABLE_SLOT(GetMovingSpriteClassId),
    VTABLE_SLOT(DestroyLaunchPad),
    VTABLE_SLOT(GetSpriteObjPriority),
    VTABLE_SLOT(ApplySpriteVelocity),
    VTABLE_SLOT(HitMovingSprite),
    VTABLE_SLOT(CheckLaunchPadContact),
};

/* Used by level_select_widgets.c (DestroyLevelSelectEntry), level_select_widgets.c,
 * level_select_parts.h. */
const struct vtable_slot gLevelSelectEntryVtable[6] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(AnimateLevelSelectEntry),
    VTABLE_SLOT(SetLevelSelectEntryLevel),
    VTABLE_SLOT(SetLevelSelectEntryPos),
    VTABLE_SLOT(nullsub_20),
    VTABLE_SLOT(DestroyLevelSelectEntry),
};

/* Used by cutscene_player.c. */
const struct vtable_slot gBgStreamerVtable[2] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyBgStreamer),
};

/* Used by cutscene_player.c. */
const struct vtable_slot gBgLayerBaseVtable[5] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyBgLayerBase),
    VTABLE_SLOT(ResetBgLayerBase),
    VTABLE_SLOT(ScrollBgLayerBase),
    VTABLE_SLOT(ClampBgLayerScrollStep),
};

/* Used by bg_layer.c (DestroyBgLayer), bg_layer_init.c
 * (InitBgLayer), tile_slot_pool.c (DestroyPooledBgLayer), bg_scroll_layer.h. */
const struct vtable_slot gBgLayerVtable[10] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyBgLayer),
    VTABLE_SLOT(ResetBgLayer),
    VTABLE_SLOT(ScrollBgLayer),
    VTABLE_SLOT(ClampBgLayerScrollStep),
    VTABLE_SLOT(LoadBgLayerTiles),
    VTABLE_SLOT(DrawBgLayerRow),
    VTABLE_SLOT(DrawBgLayerColumn),
    VTABLE_SLOT(ClipBgLayerColumns),
    VTABLE_SLOT(ClipBgLayerRows),
};

/* Used by bg_layer.c, tile_slot_pool.c (DestroyPooledBgLayer),
 * bg_scroll_layer.h. */
const struct vtable_slot gPooledBgLayerVtable[10] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPooledBgLayer),
    VTABLE_SLOT(ResetPooledBgLayer),
    VTABLE_SLOT(ScrollBgLayer),
    VTABLE_SLOT(ClampPooledBgLayerScrollStep),
    VTABLE_SLOT(LoadPooledBgLayerTiles),
    VTABLE_SLOT(DrawPooledBgLayerRow),
    VTABLE_SLOT(DrawPooledBgLayerColumn),
    VTABLE_SLOT(ClipPooledBgLayerColumns),
    VTABLE_SLOT(ClipPooledBgLayerRows),
};

/* Used by palette_cycle.c (sub_802710C). */
const struct vtable_slot gHudPartVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(CheckSpritePickup),
    VTABLE_SLOT(GetSpriteObjHitbox),
    VTABLE_SLOT(UpdateSpriteObj),
    VTABLE_SLOT(DrawSpriteObj),
    VTABLE_SLOT(IsSpriteObjOnScreen),
    VTABLE_SLOT(SpriteObjOverlapsRect),
    VTABLE_SLOT(IsSpriteObjNearCamera),
    VTABLE_SLOT(IsSpriteObjInsideRect),
    VTABLE_SLOT(GetSpriteObjClassId),
    VTABLE_SLOT(sub_802710C),
    VTABLE_SLOT(GetSpritePriority),
    VTABLE_SLOT(ApplySpriteObjVelocity),
};

/* Used by aabb_setup.c, font_glyph.c (FontDrawGlyph). */
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

/* Used by aabb_setup.c, font_glyph.c (FontDrawGlyph). */
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

/* Used by aabb_setup.c, font.c (DestroyFont),
 * font_glyph.c (FontDrawGlyph), font.c. */
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
 * Used by language_select.c (DestroyLogoActor), actor_anim.c (DestroyRiderlessPolar,
 * DestroyPolarCheckpointText, DestroyPolarWumpa, DestroyPolarTimeCrate, DestroyPolarQuestionCrate, DestroyPolarAkuAkuCrate,
 * DestroyPolarNitroCrate, DestroyPolarLifeCrate, sub_803B25C, DestroyPolarBasicCrate, DestroyPolarCrate,
 * DestroyPolarElectricFence, sub_803B30C, DestroyPolarLauncher, DestroyPolarPenguin, DestroyPolarIcicle,
 * DestroyPolarAkuAku, DestroyPolarGoal, DestroyPolarBoostPad, DestroyPolarCheckpointCrate, DestroyJetpackCheckpointText,
 * DestroyJetpackExplosion, DestroyJetpackShot, DestroyJetpackPlane, DestroyJetpackBomber, DestroyJetpackCannonball,
 * DestroyAirshipFireball, DestroyJetpackBalloon, DestroyJetpackParachuteNitro, DestroyJetpackRocket, DestroyJetpackRing,
 * DestroyHovercraftFireball, DestroyHovercraftCannon, DestroyHovercraftLauncher, DestroyHovercraftSideGun, DestroyHovercraftCannonFlash),
 * jetpack_crates.c, hovercraft.c (DestroyJetpackCollectedWumpa), polar_player_actions.c,
 * polar_pickups.c, jetpack_player.c, actor.c. */
const struct vtable_slot gActorVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyActor),
    VTABLE_SLOT(UpdateActor),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_factory.c. */
const struct vtable_slot gRiderlessPolarVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyRiderlessPolar),
    VTABLE_SLOT(UpdateActor),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_factory.c (CreatePolarCheckpointText). */
const struct vtable_slot gPolarCheckpointTextVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarCheckpointText),
    VTABLE_SLOT(UpdatePolarCheckpointText),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_player.c, polar_player_actions.c, actor_factory.c
 * (ConstructAnimTableState). */
const struct vtable_slot gPolarPlayerVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarPlayer),
    VTABLE_SLOT(UpdatePolarPlayer),
    VTABLE_SLOT(DrawPolarPlayer),
};

/* Used by polar_player_actions.c, polar_pickups.c. */
const struct vtable_slot gPolarCollectedWumpaVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarCollectedWumpa),
    VTABLE_SLOT(UpdatePolarCollectedWumpa),
    VTABLE_SLOT(DrawPolarCollectedWumpa),
};

/* Used by polar_player_actions.c, polar_pickups.c, actor_factory.c. */
const struct vtable_slot gPolarWumpaVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarWumpa),
    VTABLE_SLOT(UpdatePolarWumpa),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_crates.c, actor_factory.c. */
const struct vtable_slot gPolarTimeCrateVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarTimeCrate),
    VTABLE_SLOT(UpdatePolarTimeCrate),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_crates.c, actor_factory.c. */
const struct vtable_slot gPolarQuestionCrateVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarQuestionCrate),
    VTABLE_SLOT(UpdatePolarQuestionCrate),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_crates.c, actor_factory.c. */
const struct vtable_slot gPolarAkuAkuCrateVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarAkuAkuCrate),
    VTABLE_SLOT(UpdatePolarAkuAkuCrate),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_crates.c, actor_factory.c. */
const struct vtable_slot gPolarNitroCrateVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarNitroCrate),
    VTABLE_SLOT(UpdatePolarNitroCrate),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_crates.c, actor_factory.c. */
const struct vtable_slot gPolarLifeCrateVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarLifeCrate),
    VTABLE_SLOT(UpdatePolarLifeCrate),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_crates.c, actor_factory.c. */
const struct vtable_slot gStaticData_087E4F54[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B25C),
    VTABLE_SLOT(sub_802CA6C),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_crates.c, actor_factory.c. */
const struct vtable_slot gPolarBasicCrateVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarBasicCrate),
    VTABLE_SLOT(UpdatePolarBasicCrate),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_crates.c. */
const struct vtable_slot gPolarCrateVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarCrate),
    VTABLE_SLOT(UpdatePolarCrate),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_objects.c. */
const struct vtable_slot gPolarElectricFenceVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarElectricFence),
    VTABLE_SLOT(UpdatePolarElectricFence),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_objects.c. */
const struct vtable_slot gStaticData_087E4FD4[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B30C),
    VTABLE_SLOT(sub_802CE10),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_objects.c. */
const struct vtable_slot gPolarLauncherVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarLauncher),
    VTABLE_SLOT(UpdatePolarLauncher),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_objects.c. */
const struct vtable_slot gPolarPenguinVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarPenguin),
    VTABLE_SLOT(UpdatePolarPenguin),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_objects.c. */
const struct vtable_slot gPolarIcicleVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarIcicle),
    VTABLE_SLOT(UpdatePolarIcicle),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_aku_aku.c. */
const struct vtable_slot gPolarAkuAkuVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarAkuAku),
    VTABLE_SLOT(UpdatePolarAkuAku),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_aku_aku.c. */
const struct vtable_slot gPolarGoalVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarGoal),
    VTABLE_SLOT(UpdatePolarGoal),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_aku_aku.c. */
const struct vtable_slot gPolarBoostPadVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarBoostPad),
    VTABLE_SLOT(UpdatePolarBoostPad),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_aku_aku.c. */
const struct vtable_slot gPolarCheckpointCrateVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarCheckpointCrate),
    VTABLE_SLOT(UpdatePolarCheckpointCrate),
    VTABLE_SLOT(DrawActor),
};

/* Used by jetpack_spawn.c (CreateJetpackCheckpointText). */
const struct vtable_slot gJetpackCheckpointTextVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackCheckpointText),
    VTABLE_SLOT(UpdateJetpackCheckpointText),
    VTABLE_SLOT(DrawJetpackCheckpointText),
    VTABLE_SLOT(DamageActor),
    VTABLE_SLOT(IsJetpackCheckpointTextUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by jetpack_spawn.c (CreateJetpackExplosion). */
const struct vtable_slot gJetpackExplosionVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackExplosion),
    VTABLE_SLOT(UpdateJetpackExplosion),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageActor),
    VTABLE_SLOT(IsJetpackExplosionUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by jetpack_spawn.c (CreateJetpackPlayer), jetpack_player.c. */
const struct vtable_slot gJetpackPlayerVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackPlayer),
    VTABLE_SLOT(UpdateJetpackPlayer),
    VTABLE_SLOT(DrawJetpackPlayer),
    VTABLE_SLOT(DamageJetpackPlayer),
    VTABLE_SLOT(IsJetpackPlayerUnshootable),
    VTABLE_SLOT(GetJetpackPlayerHpPercent),
};

/* Used by jetpack_shot.c. */
const struct vtable_slot gJetpackShotVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackShot),
    VTABLE_SLOT(UpdateJetpackShot),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageActor),
    VTABLE_SLOT(IsJetpackShotUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by jetpack_plane.c (CreateJetpackPlane). */
const struct vtable_slot gJetpackPlaneVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackPlane),
    VTABLE_SLOT(UpdateJetpackPlane),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackPlane),
    VTABLE_SLOT(IsJetpackPlaneUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by jetpack_plane.c (CreateJetpackBomber). */
const struct vtable_slot gJetpackBomberVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackBomber),
    VTABLE_SLOT(UpdateJetpackBomber),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackBomber),
    VTABLE_SLOT(IsJetpackBomberUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by jetpack_plane.c (CreateJetpackCannonball). */
const struct vtable_slot gJetpackCannonballVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackCannonball),
    VTABLE_SLOT(UpdateJetpackCannonball),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageActor),
    VTABLE_SLOT(IsJetpackCannonballUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by airship_fireball.c. */
const struct vtable_slot gAirshipFireballVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyAirshipFireball),
    VTABLE_SLOT(UpdateAirshipFireball),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageAirshipFireball),
    VTABLE_SLOT(IsAirshipFireballUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by jetpack_balloon.c. */
const struct vtable_slot gJetpackBalloonVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackBalloon),
    VTABLE_SLOT(UpdateJetpackBalloon),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackBalloon),
    VTABLE_SLOT(IsJetpackBalloonUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by jetpack_crates.c. */
const struct vtable_slot gJetpackHealthCrateVtable[8] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackHealthCrate),
    VTABLE_SLOT(UpdateJetpackHealthCrate),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackHealthCrate),
    VTABLE_SLOT(IsJetpackBalloonCrateUnshootable),
    VTABLE_SLOT(GetActorHp),
    VTABLE_SLOT(BreakJetpackBalloonCrate),
};

/* Used by jetpack_crates.c. */
const struct vtable_slot gJetpackTimeCrateVtable[8] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackTimeCrate),
    VTABLE_SLOT(UpdateJetpackTimeCrate),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackTimeCrate),
    VTABLE_SLOT(IsJetpackBalloonCrateUnshootable),
    VTABLE_SLOT(GetActorHp),
    VTABLE_SLOT(BreakJetpackBalloonCrate),
};

/* Used by jetpack_crates.c. */
const struct vtable_slot gJetpackQuestionCrateVtable[8] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackQuestionCrate),
    VTABLE_SLOT(UpdateJetpackQuestionCrate),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackQuestionCrate),
    VTABLE_SLOT(IsJetpackBalloonCrateUnshootable),
    VTABLE_SLOT(GetActorHp),
    VTABLE_SLOT(BreakJetpackBalloonCrate),
};

/* Used by jetpack_crates.c. */
const struct vtable_slot gJetpackBalloonCrateVtable[8] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackBalloonCrate),
    VTABLE_SLOT(UpdateJetpackBalloonCrate),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackBalloonCrate),
    VTABLE_SLOT(IsJetpackBalloonCrateUnshootable),
    VTABLE_SLOT(GetActorHp),
    VTABLE_SLOT(BreakJetpackBalloonCrate),
};

/* Used by jetpack_crates.c (CreateJetpackParachuteNitro). */
const struct vtable_slot gJetpackParachuteNitroVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackParachuteNitro),
    VTABLE_SLOT(UpdateJetpackParachuteNitro),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackParachuteNitro),
    VTABLE_SLOT(IsJetpackParachuteNitroUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by jetpack_crates.c. */
const struct vtable_slot gJetpackRocketVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackRocket),
    VTABLE_SLOT(UpdateJetpackRocket),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackRocket),
    VTABLE_SLOT(IsJetpackRocketUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by hovercraft.c. */
const struct vtable_slot gJetpackRingVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackRing),
    VTABLE_SLOT(UpdateJetpackRing),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageActor),
    VTABLE_SLOT(IsJetpackRingUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by hovercraft.c (DestroyJetpackCollectedWumpa). */
const struct vtable_slot gJetpackCollectedWumpaVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackCollectedWumpa),
    VTABLE_SLOT(UpdateJetpackCollectedWumpa),
    VTABLE_SLOT(DrawJetpackCollectedWumpa),
    VTABLE_SLOT(DamageActor),
    VTABLE_SLOT(IsJetpackCollectedWumpaUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by hovercraft.c. */
const struct vtable_slot gHovercraftFireballVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyHovercraftFireball),
    VTABLE_SLOT(UpdateHovercraftFireball),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageHovercraftFireball),
    VTABLE_SLOT(IsHovercraftFireballUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by hovercraft_cannon.c. */
const struct vtable_slot gHovercraftCannonVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyHovercraftCannon),
    VTABLE_SLOT(UpdateHovercraftCannon),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageHovercraftCannon),
    VTABLE_SLOT(IsHovercraftCannonUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by hovercraft_launcher.c. */
const struct vtable_slot gHovercraftLauncherVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyHovercraftLauncher),
    VTABLE_SLOT(UpdateHovercraftLauncher),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageHovercraftLauncher),
    VTABLE_SLOT(IsHovercraftLauncherUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by hovercraft_launcher.c, hovercraft_side_gun.c (CreateHovercraftSideGun and the functions after it). */
const struct vtable_slot gHovercraftSideGunVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyHovercraftSideGun),
    VTABLE_SLOT(UpdateHovercraftSideGun),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageHovercraftSideGun),
    VTABLE_SLOT(IsHovercraftSideGunUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by hovercraft_cannon_flash.c. */
const struct vtable_slot gHovercraftCannonFlashVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyHovercraftCannonFlash),
    VTABLE_SLOT(UpdateHovercraftCannonFlash),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageHovercraftCannonFlash),
    VTABLE_SLOT(IsHovercraftCannonFlashUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by language_select.c (DestroyLogoActor), title_screen_init.c,
 * title_screen.c, company_logos.c (LoadUniversalLogoBg). */
const struct vtable_slot gLogoActorVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyLogoActor),
    VTABLE_SLOT(UpdateLogoActor),
    VTABLE_SLOT(DrawLogoActor),
};
