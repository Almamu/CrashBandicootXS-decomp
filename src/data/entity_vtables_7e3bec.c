#include "core.h"
#include "vtable.h"

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

extern void DestroyFont();
extern void FontMeasureText();
extern void DrawActor();
extern void FontUploadTiles();
extern void DrawEntity();
extern void CtrlHandleEvent();
extern void EffectCtrlHandleEvent();
extern void nullsub_20();
extern void DamageHovercraftCannonFlash();
extern void nullsub_44();
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
extern void UpdateEnemyCtrl();
extern void HitEnemy();
extern void AttachEnemyCtrl();
extern void DestroyEnemyCtrl();
extern void UpdatePeriodicSpawner();
extern void DestroyPeriodicSpawner();
extern void UpdateKnockedEnemyCtrl();
extern void DestroyKnockedEnemyCtrl();
extern void UpdateEffectCtrl();
extern void DestroyEffectCtrl();
extern void DrawCrate();
extern void UpdateCrate();
extern void IsCrateInsideRect();
extern void GetCrateClassId();
extern void DestroyCrate();
extern void CheckExtraLifePickup();
extern void UpdateExtraLife();
extern void DrawExtraLife();
extern void GetExtraLifeClassId();
extern void DestroyExtraLife();
extern void CollideExtraLife();
extern void CheckWumpaPickup();
extern void UpdateWumpa();
extern void DrawWumpa();
extern void GetWumpaClassId();
extern void DestroyWumpa();
extern void CollideWumpa();
extern void UpdateStopwatch();
extern void DestroyStopwatch();
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
extern void sub_8017A70();
extern void sub_8017A78();
extern void UpdateChaser();
extern void StartChaserMotionYFromSet();
extern void StartChaserMotionXFromSet();
extern void DestroyChaserCtrl();
extern void UpdateTiny();
extern void UpdateStompedHopPad();
extern void DestroyStompedHopPadCtrl();
extern void UpdateOneShotAnimCtrl();
extern void DestroyOneShotAnimCtrl();
extern void sub_80188FC();
extern void sub_8018960();
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
extern void FontDrawGlyph();
extern void FontPutChar();
extern void FontDrawChars();
extern void FontDrawText();
extern void FontMeasureChars();
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
extern void DestroyLargeFont();
extern void DestroySmallFont();
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

/* Used by actor_aabb_setup.c, actor_part124.c, actor_part39.c,
 * actor_part6.c (DestroySpriteObj), actor_part7.c, graphics.c (nullsub_12,
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

/* Used by actor_part6.c (GetSpriteObjPriority, DestroySpriteObj), actor_part7.c. */
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

/* Used by actor_part7.c (DestroyUiSpriteObj). */
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

/* Used by actor_part14.c, actor_part8.c (GetMovingSpriteClassId, DestroyMovingSprite,
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

/* Used by actor_part14.c (GetGroundSpriteClassId, DestroyGroundSprite, ResetGroundSprite). */
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

/* Used by actor_part15.c, actor_part77.c. */
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

/* Used by actor_part124.c, actor_part17.c, actor_part27.c, actor_part57.c. */
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

/* Used by actor_part112.c, actor_part124.c. */
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

/* Used by actor_part124.c. */
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

/* Used by actor_part112.c, actor_part117.c, actor_part124.c. */
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

/* Used by actor_part123.c. */
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

/* Used by game_loop31.c (DestroyCrate), game_loop36.c (CreateCrate). */
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

/* Used by game_loop52.c, game_loop54.c (UpdateExtraLife). */
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

/* Used by actor_part39.c (DestroyWumpa, ResetWumpaPickup), game_loop53.c
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

/* Used by actor_part39.c (UpdateStopwatch, DestroyStopwatch). */
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

/* Used by actor_part39.c, actor_part57.c. */
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

/* Used by actor_part_16048.c (DestroyPlayerCtrl), player_ctrl.h. */
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

/* Used by actor_part_17524.c (DestroyInputCtrl). */
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

/* Used by actor_part27.c. */
const struct vtable_slot gStaticData_087E435C[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateCtrl),
    VTABLE_SLOT(sub_8017A70),
    VTABLE_SLOT(AttachCtrl),
    VTABLE_SLOT(SetCtrlMode),
    VTABLE_SLOT(StartCtrlTargetMotionX),
    VTABLE_SLOT(StartCtrlTargetMotionY),
    VTABLE_SLOT(SetCtrlTargetMotionX),
    VTABLE_SLOT(SetCtrlTargetMotionY),
    VTABLE_SLOT(sub_8017A78),
    VTABLE_SLOT(SetCtrlTargetAnim),
    VTABLE_SLOT(StartCtrlTargetMotionXFromSet),
    VTABLE_SLOT(StartCtrlTargetMotionYFromSet),
};

/* Used by actor_part27b.c. */
const struct vtable_slot gChaserCtrlVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateChaser),
    VTABLE_SLOT(sub_8017A70),
    VTABLE_SLOT(AttachCtrl),
    VTABLE_SLOT(SetCtrlMode),
    VTABLE_SLOT(StartCtrlTargetMotionX),
    VTABLE_SLOT(StartCtrlTargetMotionY),
    VTABLE_SLOT(SetCtrlTargetMotionX),
    VTABLE_SLOT(SetCtrlTargetMotionY),
    VTABLE_SLOT(DestroyChaserCtrl),
    VTABLE_SLOT(SetCtrlTargetAnim),
    VTABLE_SLOT(StartChaserMotionXFromSet),
    VTABLE_SLOT(StartChaserMotionYFromSet),
};

/* Used by actor_part27c.c. */
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

/* Used by actor_part_188d0.c (CreateOneShotAnimCtrl, DestroyOneShotAnimCtrl). */
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

/* Used by actor_part_188d0.c (sub_8018948, sub_8018960). */
const struct vtable_slot gStaticData_087E44FC[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_80188FC),
    VTABLE_SLOT(CtrlHandleEvent),
    VTABLE_SLOT(AttachCtrl),
    VTABLE_SLOT(SetCtrlMode),
    VTABLE_SLOT(StartCtrlTargetMotionX),
    VTABLE_SLOT(StartCtrlTargetMotionY),
    VTABLE_SLOT(SetCtrlTargetMotionX),
    VTABLE_SLOT(SetCtrlTargetMotionY),
    VTABLE_SLOT(sub_8018960),
    VTABLE_SLOT(SetCtrlTargetAnim),
    VTABLE_SLOT(StartCtrlTargetMotionXFromSet),
    VTABLE_SLOT(StartCtrlTargetMotionYFromSet),
};

/* Used by actor_part_18008.c, actor_part_188d0.c (DestroyTiny,
 * CreateTiny). */
const struct vtable_slot gTinyVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateTiny),
    VTABLE_SLOT(sub_8017A70),
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

/* Used by actor_part_188d0.c (DestroyCortexBossGemCtrl, CreateCortexBossGemCtrl). */
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

/* Used by actor_part_188d0.c (DestroyCortexBossPlatformMover, CreateCortexBossPlatformMover). */
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

/* Used by actor_part_188d0.c (DestroyCortexShotCtrl, CreateCortexShotCtrl). */
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

/* Used by actor_part_1967c.c (DestroyCortexTargetCtrl). */
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

/* Used by actor_part_1967c.c (DestroyCortexCannonCtrl). */
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

/* Used by actor_part_1967c.c (DestroyCortexBoss). */
const struct vtable_slot gCortexBossVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateCortexBoss),
    VTABLE_SLOT(sub_8017A70),
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

/* Used by actor_part_1967c.c (UpdateDingodileShark, DestroyDingodileSharkCtrl). */
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

/* Used by actor_part_1967c.c (SpawnDingodileStalactite, DestroyDingodileProjectileCtrl). */
const struct vtable_slot gDingodileProjectileVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateDingodileProjectile),
    VTABLE_SLOT(sub_8017A70),
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

/* Used by actor_part_1967c.c (DestroyDingodileShieldCtrl), actor_part_1a794.c,
 * gobj_1a794.h. */
const struct vtable_slot gDingodileShieldVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateDingodileShield),
    VTABLE_SLOT(sub_8017A70),
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

/* Used by actor_part_1967c.c, actor_part_1a794.c (DestroyDingodile),
 * gobj_1a794.h. */
const struct vtable_slot gDingodileVtable[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(UpdateDingodile),
    VTABLE_SLOT(sub_8017A70),
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

/* Used by actor_part_1b208.c (DestroyPlatform), gobj_1a794.h. */
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

/* Used by actor_part_1b208.c (DestroyPlatformMover), gobj_1a794.h. */
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

/* Used by actor_part_1b85c.c (DestroyCameraLead). */
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

/* Used by actor_part_1b85c.c (GetCameraLeadOffset, DestroyLaunchPad, sub_801BAC4). */
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

/* Used by actor_part_1da38.c (DestroyLevelSelectEntry), actor_part_1dfec.c,
 * level_select_parts.h. */
const struct vtable_slot gLevelSelectEntryVtable[6] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(AnimateLevelSelectEntry),
    VTABLE_SLOT(SetLevelSelectEntryLevel),
    VTABLE_SLOT(SetLevelSelectEntryPos),
    VTABLE_SLOT(nullsub_20),
    VTABLE_SLOT(DestroyLevelSelectEntry),
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
    VTABLE_SLOT(ClampBgLayerScrollStep),
};

/* Used by bg_scroll_layer_25fc8.c (DestroyBgLayer), game_loop15.c
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

/* Used by bg_scroll_layer_25fc8.c, tile_slot_pool.c (DestroyPooledBgLayer),
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

/* Used by hud_icon_slot.c (sub_802710C). */
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
 * Used by counter_selector.c (DestroyLogoActor), actor_anim.c (DestroyRiderlessPolar,
 * DestroyPolarCheckpointText, DestroyPolarWumpa, DestroyPolarTimeCrate, DestroyPolarQuestionCrate, DestroyPolarAkuAkuCrate,
 * DestroyPolarNitroCrate, DestroyPolarLifeCrate, sub_803B25C, DestroyPolarBasicCrate, DestroyPolarCrate,
 * DestroyPolarElectricFence, sub_803B30C, DestroyPolarLauncher, DestroyPolarPenguin, DestroyPolarIcicle,
 * DestroyPolarAkuAku, DestroyPolarGoal, DestroyPolarBoostPad, DestroyPolarCheckpointCrate, DestroyJetpackCheckpointText,
 * DestroyJetpackExplosion, DestroyJetpackShot, DestroyJetpackPlane, DestroyJetpackBomber, DestroyJetpackCannonball,
 * DestroyAirshipFireball, DestroyJetpackBalloon, DestroyJetpackParachuteNitro, DestroyJetpackRocket, DestroyJetpackRing,
 * DestroyHovercraftFireball, DestroyHovercraftCannon, DestroyHovercraftLauncher, DestroyHovercraftSideGun, DestroyHovercraftCannonFlash),
 * actor_part129.c, actor_part130.c (DestroyJetpackCollectedWumpa), actor_part19.c,
 * actor_part19c.c, actor_part44.c, actor_part50.c, actor_part52.c. */
const struct vtable_slot gActorVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyActor),
    VTABLE_SLOT(UpdateActor),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part_2ac28.c. */
const struct vtable_slot gRiderlessPolarVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyRiderlessPolar),
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
const struct vtable_slot gPolarPlayerVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarPlayer),
    VTABLE_SLOT(UpdatePolarPlayer),
    VTABLE_SLOT(DrawPolarPlayer),
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
const struct vtable_slot gPolarLauncherVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarLauncher),
    VTABLE_SLOT(UpdatePolarLauncher),
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
const struct vtable_slot gPolarGoalVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarGoal),
    VTABLE_SLOT(UpdatePolarGoal),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part58.c. */
const struct vtable_slot gPolarBoostPadVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarBoostPad),
    VTABLE_SLOT(UpdatePolarBoostPad),
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
    VTABLE_SLOT(IsJetpackCheckpointTextUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part128.c (CreateJetpackExplosion). */
const struct vtable_slot gJetpackExplosionVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackExplosion),
    VTABLE_SLOT(UpdateJetpackExplosion),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(nullsub_44),
    VTABLE_SLOT(IsJetpackExplosionUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part128.c (CreateJetpackPlayer), actor_part44.c. */
const struct vtable_slot gJetpackPlayerVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackPlayer),
    VTABLE_SLOT(UpdateJetpackPlayer),
    VTABLE_SLOT(DrawJetpackPlayer),
    VTABLE_SLOT(DamageJetpackPlayer),
    VTABLE_SLOT(IsJetpackPlayerUnshootable),
    VTABLE_SLOT(GetJetpackPlayerHpPercent),
};

/* Used by actor_part45c.c. */
const struct vtable_slot gJetpackShotVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackShot),
    VTABLE_SLOT(UpdateJetpackShot),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(nullsub_44),
    VTABLE_SLOT(IsJetpackShotUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part_2fbf0.c (CreateJetpackPlane). */
const struct vtable_slot gJetpackPlaneVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackPlane),
    VTABLE_SLOT(UpdateJetpackPlane),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackPlane),
    VTABLE_SLOT(IsJetpackPlaneUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part_2fbf0.c (CreateJetpackBomber). */
const struct vtable_slot gJetpackBomberVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackBomber),
    VTABLE_SLOT(UpdateJetpackBomber),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackBomber),
    VTABLE_SLOT(IsJetpackBomberUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part_2fbf0.c (CreateJetpackCannonball). */
const struct vtable_slot gJetpackCannonballVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackCannonball),
    VTABLE_SLOT(UpdateJetpackCannonball),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(nullsub_44),
    VTABLE_SLOT(IsJetpackCannonballUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part20d.c. */
const struct vtable_slot gAirshipFireballVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyAirshipFireball),
    VTABLE_SLOT(UpdateAirshipFireball),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageAirshipFireball),
    VTABLE_SLOT(IsAirshipFireballUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part125.c. */
const struct vtable_slot gJetpackBalloonVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackBalloon),
    VTABLE_SLOT(UpdateJetpackBalloon),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackBalloon),
    VTABLE_SLOT(IsJetpackBalloonUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part129.c. */
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

/* Used by actor_part129.c. */
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

/* Used by actor_part129.c. */
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

/* Used by actor_part129.c. */
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

/* Used by actor_part129.c (CreateJetpackParachuteNitro). */
const struct vtable_slot gJetpackParachuteNitroVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackParachuteNitro),
    VTABLE_SLOT(UpdateJetpackParachuteNitro),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackParachuteNitro),
    VTABLE_SLOT(IsJetpackParachuteNitroUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part129.c. */
const struct vtable_slot gJetpackRocketVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackRocket),
    VTABLE_SLOT(UpdateJetpackRocket),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackRocket),
    VTABLE_SLOT(IsJetpackRocketUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part130.c. */
const struct vtable_slot gJetpackRingVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackRing),
    VTABLE_SLOT(UpdateJetpackRing),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(nullsub_44),
    VTABLE_SLOT(IsJetpackRingUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part130.c (DestroyJetpackCollectedWumpa). */
const struct vtable_slot gJetpackCollectedWumpaVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackCollectedWumpa),
    VTABLE_SLOT(UpdateJetpackCollectedWumpa),
    VTABLE_SLOT(DrawJetpackCollectedWumpa),
    VTABLE_SLOT(nullsub_44),
    VTABLE_SLOT(IsJetpackCollectedWumpaUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part130.c. */
const struct vtable_slot gHovercraftFireballVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyHovercraftFireball),
    VTABLE_SLOT(UpdateHovercraftFireball),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageHovercraftFireball),
    VTABLE_SLOT(IsHovercraftFireballUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part32.c. */
const struct vtable_slot gHovercraftCannonVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyHovercraftCannon),
    VTABLE_SLOT(UpdateHovercraftCannon),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageHovercraftCannon),
    VTABLE_SLOT(IsHovercraftCannonUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part63.c. */
const struct vtable_slot gHovercraftLauncherVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyHovercraftLauncher),
    VTABLE_SLOT(UpdateHovercraftLauncher),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageHovercraftLauncher),
    VTABLE_SLOT(IsHovercraftLauncherUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part65.c, actor_part66.c (CreateHovercraftSideGun), actor_part67.c. */
const struct vtable_slot gHovercraftSideGunVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyHovercraftSideGun),
    VTABLE_SLOT(UpdateHovercraftSideGun),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageHovercraftSideGun),
    VTABLE_SLOT(IsHovercraftSideGunUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by actor_part69.c. */
const struct vtable_slot gHovercraftCannonFlashVtable[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyHovercraftCannonFlash),
    VTABLE_SLOT(UpdateHovercraftCannonFlash),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageHovercraftCannonFlash),
    VTABLE_SLOT(IsHovercraftCannonFlashUnshootable),
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
