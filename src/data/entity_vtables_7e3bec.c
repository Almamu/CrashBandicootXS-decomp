#include "core.h"
#include "vtable.h"
#include "text.h"
#include "pickups.h"
#include "enemies.h"
#include "frontend.h"
#include "system.h"
#include "menus.h"
#include "crates.h"
#include "player.h"
#include "actor.h"
#include "bosses.h"
#include "vehicle.h"
#include "gfx.h"
#include "objects.h"
#include "level.h"
#include "globals.h"

/*
 * ROM 0x087E3BEC-0x087E55E4: the virtual tables of the game's C++
 * object classes (docs/rom_map.md's "93 entity vtables") that g++
 * doesn't emit yet, in ROM order. The others are emitted by g++ in their
 * class's key-method object (docs/cplusplus.md, "Emitting the
 * vtables"). Each table is in a section of its own (VTABLE_SECTION) so
 * that ldscript.txt can place it at its ROM address, between the emitted
 * ones. Constructors store one at the object's method-table pointer; the
 * code reads the slots through `struct actor_method` (actor_self.h).
 * Tables of the same class family share their leading slots, the ROM's
 * own inheritance. See docs/data.md.
 */

/* Used by enemy_ctrl.cpp, ctrl.cpp (Ctrl, include/ctrl.hpp), input_ctrl_queue.cpp, action_ctrl.cpp. */
const struct vtable_slot gCtrlVtable[13] VTABLE_SECTION(gCtrlVtable) = {
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

/* Used by level_select.cpp (DestroyCameraLead). */
const struct vtable_slot gCameraLeadVtable[15] VTABLE_SECTION(gCameraLeadVtable) = {
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

/* Used by level_select.cpp (GetCameraLeadOffset, DestroyLaunchPad, ClearLaunchPadVulnerable). */
const struct vtable_slot gLaunchPadVtable[15] VTABLE_SECTION(gLaunchPadVtable) = {
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

/* Used by level_select_widgets.cpp (DestroyLevelSelectEntry). */
const struct vtable_slot gLevelSelectEntryVtable[6] VTABLE_SECTION(gLevelSelectEntryVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(AnimateLevelSelectEntry),
    VTABLE_SLOT(SetLevelSelectEntryLevel),
    VTABLE_SLOT(SetLevelSelectEntryPos),
    VTABLE_SLOT(DrawLevelSelectEntry),
    VTABLE_SLOT(DestroyLevelSelectEntry),
};

/* Used by cutscene_player.c. */
const struct vtable_slot gBgStreamerVtable[2] VTABLE_SECTION(gBgStreamerVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyBgStreamer),
};

/* Used by cutscene_player.c. */
const struct vtable_slot gBgLayerBaseVtable[5] VTABLE_SECTION(gBgLayerBaseVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyBgLayerBase),
    VTABLE_SLOT(ResetBgLayerBase),
    VTABLE_SLOT(ScrollBgLayerBase),
    VTABLE_SLOT(ClampBgLayerScrollStep),
};

/* Used by bg_layer.c (DestroyBgLayer), bg_layer_init.c
 * (InitBgLayer), tile_slot_pool.c (DestroyPooledBgLayer), bg_scroll_layer.h. */
const struct vtable_slot gBgLayerVtable[10] VTABLE_SECTION(gBgLayerVtable) = {
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
const struct vtable_slot gPooledBgLayerVtable[10] VTABLE_SECTION(gPooledBgLayerVtable) = {
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

/* Used by aabb_setup.c, font_glyph.c (FontDrawGlyph). */
const struct vtable_slot gLargeFontVtable[9] VTABLE_SECTION(gLargeFontVtable) = {
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
const struct vtable_slot gSmallFontVtable[9] VTABLE_SECTION(gSmallFontVtable) = {
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
const struct vtable_slot gFontVtable[9] VTABLE_SECTION(gFontVtable) = {
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
 * Used by language_select.cpp (DestroyLogoActor), actor_anim.c (DestroyRiderlessPolar,
 * DestroyPolarCheckpointText, DestroyPolarWumpa, DestroyPolarTimeCrate, DestroyPolarQuestionCrate, DestroyPolarAkuAkuCrate,
 * DestroyPolarNitroCrate, DestroyPolarLifeCrate, DestroyPolarFourWumpaCrate, DestroyPolarBasicCrate, DestroyPolarCrate,
 * DestroyPolarElectricFence, DestroyPolarObstacle, DestroyPolarLauncher, DestroyPolarPenguin, DestroyPolarIcicle,
 * DestroyPolarAkuAku, DestroyPolarGoal, DestroyPolarBoostPad, DestroyPolarCheckpointCrate, DestroyJetpackCheckpointText,
 * DestroyJetpackExplosion, DestroyJetpackShot, DestroyJetpackPlane, DestroyJetpackBomber, DestroyJetpackCannonball,
 * DestroyAirshipFireball, DestroyJetpackBalloon, DestroyJetpackParachuteNitro, DestroyJetpackRocket, DestroyJetpackRing,
 * DestroyHovercraftFireball, DestroyHovercraftCannon, DestroyHovercraftLauncher, DestroyHovercraftSideGun, DestroyHovercraftCannonFlash),
 * jetpack_crates.cpp, hovercraft.cpp (~JetpackCollectedWumpa), polar_player_actions.cpp,
 * polar_pickups.cpp, jetpack_player.cpp, actor.c. */
const struct vtable_slot gActorVtable[4] VTABLE_SECTION(gActorVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyActor),
    VTABLE_SLOT(UpdateActor),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_factory.cpp. */
const struct vtable_slot gRiderlessPolarVtable[4] VTABLE_SECTION(gRiderlessPolarVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyRiderlessPolar),
    VTABLE_SLOT(UpdateActor),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_factory.cpp (CreatePolarCheckpointText). */
const struct vtable_slot gPolarCheckpointTextVtable[4] VTABLE_SECTION(gPolarCheckpointTextVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarCheckpointText),
    VTABLE_SLOT(UpdatePolarCheckpointText),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_player.cpp, polar_player_actions.cpp, actor_factory.cpp
 * (ConstructAnimTableState). */
const struct vtable_slot gPolarPlayerVtable[4] VTABLE_SECTION(gPolarPlayerVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarPlayer),
    VTABLE_SLOT(UpdatePolarPlayer),
    VTABLE_SLOT(DrawPolarPlayer),
};

/* Used by polar_player_actions.cpp, polar_pickups.cpp. */
const struct vtable_slot gPolarCollectedWumpaVtable[4] VTABLE_SECTION(gPolarCollectedWumpaVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarCollectedWumpa),
    VTABLE_SLOT(UpdatePolarCollectedWumpa),
    VTABLE_SLOT(DrawPolarCollectedWumpa),
};

/* Used by polar_player_actions.cpp, polar_pickups.cpp, actor_factory.cpp. */
const struct vtable_slot gPolarWumpaVtable[4] VTABLE_SECTION(gPolarWumpaVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarWumpa),
    VTABLE_SLOT(UpdatePolarWumpa),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_crates.cpp, actor_factory.cpp. */
const struct vtable_slot gPolarTimeCrateVtable[4] VTABLE_SECTION(gPolarTimeCrateVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarTimeCrate),
    VTABLE_SLOT(UpdatePolarTimeCrate),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_crates.cpp, actor_factory.cpp. */
const struct vtable_slot gPolarQuestionCrateVtable[4] VTABLE_SECTION(gPolarQuestionCrateVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarQuestionCrate),
    VTABLE_SLOT(UpdatePolarQuestionCrate),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_crates.cpp, actor_factory.cpp. */
const struct vtable_slot gPolarAkuAkuCrateVtable[4] VTABLE_SECTION(gPolarAkuAkuCrateVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarAkuAkuCrate),
    VTABLE_SLOT(UpdatePolarAkuAkuCrate),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_crates.cpp, actor_factory.cpp. */
const struct vtable_slot gPolarNitroCrateVtable[4] VTABLE_SECTION(gPolarNitroCrateVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarNitroCrate),
    VTABLE_SLOT(UpdatePolarNitroCrate),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_crates.cpp, actor_factory.cpp. */
const struct vtable_slot gPolarLifeCrateVtable[4] VTABLE_SECTION(gPolarLifeCrateVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarLifeCrate),
    VTABLE_SLOT(UpdatePolarLifeCrate),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_crates.cpp, actor_factory.cpp. */
const struct vtable_slot gPolarFourWumpaCrateVtable[4] VTABLE_SECTION(gPolarFourWumpaCrateVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarFourWumpaCrate),
    VTABLE_SLOT(UpdatePolarFourWumpaCrate),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_crates.cpp, actor_factory.cpp. */
const struct vtable_slot gPolarBasicCrateVtable[4] VTABLE_SECTION(gPolarBasicCrateVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarBasicCrate),
    VTABLE_SLOT(UpdatePolarBasicCrate),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_crates.cpp. */
const struct vtable_slot gPolarCrateVtable[4] VTABLE_SECTION(gPolarCrateVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarCrate),
    VTABLE_SLOT(UpdatePolarCrate),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_objects.cpp. */
const struct vtable_slot gPolarElectricFenceVtable[4] VTABLE_SECTION(gPolarElectricFenceVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarElectricFence),
    VTABLE_SLOT(UpdatePolarElectricFence),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_objects.cpp. */
const struct vtable_slot gPolarObstacleVtable[4] VTABLE_SECTION(gPolarObstacleVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarObstacle),
    VTABLE_SLOT(UpdatePolarObstacle),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_objects.cpp. */
const struct vtable_slot gPolarLauncherVtable[4] VTABLE_SECTION(gPolarLauncherVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarLauncher),
    VTABLE_SLOT(UpdatePolarLauncher),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_objects.cpp. */
const struct vtable_slot gPolarPenguinVtable[4] VTABLE_SECTION(gPolarPenguinVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarPenguin),
    VTABLE_SLOT(UpdatePolarPenguin),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_objects.cpp. */
const struct vtable_slot gPolarIcicleVtable[4] VTABLE_SECTION(gPolarIcicleVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarIcicle),
    VTABLE_SLOT(UpdatePolarIcicle),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_aku_aku.cpp. */
const struct vtable_slot gPolarAkuAkuVtable[4] VTABLE_SECTION(gPolarAkuAkuVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarAkuAku),
    VTABLE_SLOT(UpdatePolarAkuAku),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_aku_aku.cpp. */
const struct vtable_slot gPolarGoalVtable[4] VTABLE_SECTION(gPolarGoalVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarGoal),
    VTABLE_SLOT(UpdatePolarGoal),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_aku_aku.cpp. */
const struct vtable_slot gPolarBoostPadVtable[4] VTABLE_SECTION(gPolarBoostPadVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarBoostPad),
    VTABLE_SLOT(UpdatePolarBoostPad),
    VTABLE_SLOT(DrawActor),
};

/* Used by polar_aku_aku.cpp. */
const struct vtable_slot gPolarCheckpointCrateVtable[4] VTABLE_SECTION(gPolarCheckpointCrateVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarCheckpointCrate),
    VTABLE_SLOT(UpdatePolarCheckpointCrate),
    VTABLE_SLOT(DrawActor),
};

/* Used by jetpack_spawn.cpp (CreateJetpackCheckpointText). */
const struct vtable_slot gJetpackCheckpointTextVtable[7] VTABLE_SECTION(gJetpackCheckpointTextVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackCheckpointText),
    VTABLE_SLOT(UpdateJetpackCheckpointText),
    VTABLE_SLOT(DrawJetpackCheckpointText),
    VTABLE_SLOT(DamageActor),
    VTABLE_SLOT(IsJetpackCheckpointTextUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by jetpack_spawn.cpp (CreateJetpackExplosion). */
const struct vtable_slot gJetpackExplosionVtable[7] VTABLE_SECTION(gJetpackExplosionVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackExplosion),
    VTABLE_SLOT(UpdateJetpackExplosion),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageActor),
    VTABLE_SLOT(IsJetpackExplosionUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by jetpack_spawn.cpp (CreateJetpackPlayer), jetpack_player.cpp. */
const struct vtable_slot gJetpackPlayerVtable[7] VTABLE_SECTION(gJetpackPlayerVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackPlayer),
    VTABLE_SLOT(UpdateJetpackPlayer),
    VTABLE_SLOT(DrawJetpackPlayer),
    VTABLE_SLOT(DamageJetpackPlayer),
    VTABLE_SLOT(IsJetpackPlayerUnshootable),
    VTABLE_SLOT(GetJetpackPlayerHpPercent),
};

/* Used by jetpack_shot.cpp. */
const struct vtable_slot gJetpackShotVtable[7] VTABLE_SECTION(gJetpackShotVtable) = {
    VTABLE_SLOT(NULL),       VTABLE_SLOT(DestroyJetpackShot), VTABLE_SLOT(UpdateJetpackShot),
    VTABLE_SLOT(DrawActor),  VTABLE_SLOT(DamageActor),        VTABLE_SLOT(IsJetpackShotUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by jetpack_plane.cpp (JetpackPlane, vehicle.hpp). */
const struct vtable_slot gJetpackPlaneVtable[7] VTABLE_SECTION(gJetpackPlaneVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackPlane),
    VTABLE_SLOT(UpdateJetpackPlane),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackPlane),
    VTABLE_SLOT(IsJetpackPlaneUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by jetpack_plane.cpp (JetpackBomber, vehicle.hpp). */
const struct vtable_slot gJetpackBomberVtable[7] VTABLE_SECTION(gJetpackBomberVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackBomber),
    VTABLE_SLOT(UpdateJetpackBomber),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackBomber),
    VTABLE_SLOT(IsJetpackBomberUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by jetpack_plane.cpp (JetpackCannonball, vehicle.hpp). */
const struct vtable_slot gJetpackCannonballVtable[7] VTABLE_SECTION(gJetpackCannonballVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackCannonball),
    VTABLE_SLOT(UpdateJetpackCannonball),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageActor),
    VTABLE_SLOT(IsJetpackCannonballUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by airship_fireball.c. */
const struct vtable_slot gAirshipFireballVtable[7] VTABLE_SECTION(gAirshipFireballVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyAirshipFireball),
    VTABLE_SLOT(UpdateAirshipFireball),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageAirshipFireball),
    VTABLE_SLOT(IsAirshipFireballUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by jetpack_balloon.cpp (JetpackBalloon, vehicle.hpp). */
const struct vtable_slot gJetpackBalloonVtable[7] VTABLE_SECTION(gJetpackBalloonVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackBalloon),
    VTABLE_SLOT(UpdateJetpackBalloon),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackBalloon),
    VTABLE_SLOT(IsJetpackBalloonUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by jetpack_crates.cpp. */
const struct vtable_slot gJetpackHealthCrateVtable[8] VTABLE_SECTION(gJetpackHealthCrateVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackHealthCrate),
    VTABLE_SLOT(UpdateJetpackHealthCrate),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackHealthCrate),
    VTABLE_SLOT(IsJetpackBalloonCrateUnshootable),
    VTABLE_SLOT(GetActorHp),
    VTABLE_SLOT(BreakJetpackBalloonCrate),
};

/* Used by jetpack_crates.cpp. */
const struct vtable_slot gJetpackTimeCrateVtable[8] VTABLE_SECTION(gJetpackTimeCrateVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackTimeCrate),
    VTABLE_SLOT(UpdateJetpackTimeCrate),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackTimeCrate),
    VTABLE_SLOT(IsJetpackBalloonCrateUnshootable),
    VTABLE_SLOT(GetActorHp),
    VTABLE_SLOT(BreakJetpackBalloonCrate),
};

/* Used by jetpack_crates.cpp. */
const struct vtable_slot gJetpackQuestionCrateVtable[8] VTABLE_SECTION(gJetpackQuestionCrateVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackQuestionCrate),
    VTABLE_SLOT(UpdateJetpackQuestionCrate),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackQuestionCrate),
    VTABLE_SLOT(IsJetpackBalloonCrateUnshootable),
    VTABLE_SLOT(GetActorHp),
    VTABLE_SLOT(BreakJetpackBalloonCrate),
};

/* Used by jetpack_crates.cpp. */
const struct vtable_slot gJetpackBalloonCrateVtable[8] VTABLE_SECTION(gJetpackBalloonCrateVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackBalloonCrate),
    VTABLE_SLOT(UpdateJetpackBalloonCrate),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackBalloonCrate),
    VTABLE_SLOT(IsJetpackBalloonCrateUnshootable),
    VTABLE_SLOT(GetActorHp),
    VTABLE_SLOT(BreakJetpackBalloonCrate),
};

/* Used by jetpack_crates.cpp (CreateJetpackParachuteNitro). */
const struct vtable_slot gJetpackParachuteNitroVtable[7] VTABLE_SECTION(gJetpackParachuteNitroVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackParachuteNitro),
    VTABLE_SLOT(UpdateJetpackParachuteNitro),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackParachuteNitro),
    VTABLE_SLOT(IsJetpackParachuteNitroUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by jetpack_crates.cpp. */
const struct vtable_slot gJetpackRocketVtable[7] VTABLE_SECTION(gJetpackRocketVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackRocket),
    VTABLE_SLOT(UpdateJetpackRocket),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageJetpackRocket),
    VTABLE_SLOT(IsJetpackRocketUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by hovercraft.cpp. */
const struct vtable_slot gJetpackRingVtable[7] VTABLE_SECTION(gJetpackRingVtable) = {
    VTABLE_SLOT(NULL),       VTABLE_SLOT(DestroyJetpackRing), VTABLE_SLOT(UpdateJetpackRing),
    VTABLE_SLOT(DrawActor),  VTABLE_SLOT(DamageActor),        VTABLE_SLOT(IsJetpackRingUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by hovercraft.cpp (JetpackCollectedWumpa's destructor). */
const struct vtable_slot gJetpackCollectedWumpaVtable[7] VTABLE_SECTION(gJetpackCollectedWumpaVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyJetpackCollectedWumpa),
    VTABLE_SLOT(UpdateJetpackCollectedWumpa),
    VTABLE_SLOT(DrawJetpackCollectedWumpa),
    VTABLE_SLOT(DamageActor),
    VTABLE_SLOT(IsJetpackCollectedWumpaUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by hovercraft.cpp. */
const struct vtable_slot gHovercraftFireballVtable[7] VTABLE_SECTION(gHovercraftFireballVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyHovercraftFireball),
    VTABLE_SLOT(UpdateHovercraftFireball),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageHovercraftFireball),
    VTABLE_SLOT(IsHovercraftFireballUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by hovercraft_cannon.cpp. */
const struct vtable_slot gHovercraftCannonVtable[7] VTABLE_SECTION(gHovercraftCannonVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyHovercraftCannon),
    VTABLE_SLOT(UpdateHovercraftCannon),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageHovercraftCannon),
    VTABLE_SLOT(IsHovercraftCannonUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by hovercraft_launcher.cpp. */
const struct vtable_slot gHovercraftLauncherVtable[7] VTABLE_SECTION(gHovercraftLauncherVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyHovercraftLauncher),
    VTABLE_SLOT(UpdateHovercraftLauncher),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageHovercraftLauncher),
    VTABLE_SLOT(IsHovercraftLauncherUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by hovercraft_side_gun.cpp. */
const struct vtable_slot gHovercraftSideGunVtable[7] VTABLE_SECTION(gHovercraftSideGunVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyHovercraftSideGun),
    VTABLE_SLOT(UpdateHovercraftSideGun),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageHovercraftSideGun),
    VTABLE_SLOT(IsHovercraftSideGunUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by hovercraft_cannon_flash.cpp. */
const struct vtable_slot gHovercraftCannonFlashVtable[7] VTABLE_SECTION(gHovercraftCannonFlashVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyHovercraftCannonFlash),
    VTABLE_SLOT(UpdateHovercraftCannonFlash),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(DamageHovercraftCannonFlash),
    VTABLE_SLOT(IsHovercraftCannonFlashUnshootable),
    VTABLE_SLOT(GetActorHp),
};

/* Used by language_select.cpp (DestroyLogoActor), title_screen_init.cpp,
 * title_screen.cpp, company_logos.cpp (LoadUniversalLogoBg). */
const struct vtable_slot gLogoActorVtable[4] VTABLE_SECTION(gLogoActorVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyLogoActor),
    VTABLE_SLOT(UpdateLogoActor),
    VTABLE_SLOT(DrawLogoActor),
};
