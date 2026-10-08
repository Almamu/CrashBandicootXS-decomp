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
 * ROM 0x087E3BEC-0x087E55E4: the 93 virtual tables of the game's C++
 * object classes (docs/rom_map.md's "93 entity vtables"), in ROM order.
 * Constructors store one at the object's method-table pointer (e.g.
 * `obj->table = gEntityVtable` in graphics.cpp, `self->vtable` in
 * gobj_1a794.h); the code reads the slots through `struct actor_method`
 * (actor_self.h). Tables of the same class family share their
 * leading slots, the ROM's own inheritance. Linked in ROM order between
 * data/data.s sections by ldscript.txt - see docs/data.md.
 */

/* Used by aabb_setup.c, enemy_ctrl.cpp, wumpa.cpp,
 * sprite_obj.cpp (DestroySpriteObj), sprite_anim.cpp, graphics.cpp (nullsub_12,
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

/* Used by sprite_obj.cpp (GetSpriteObjPriority, DestroySpriteObj), sprite_anim.cpp. */
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

/* Used by sprite_anim.cpp (DestroyUiSpriteObj). */
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

/* Used by ground_sprite.cpp, moving_sprite.cpp (GetMovingSpriteClassId, DestroyMovingSprite,
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

/* Used by ground_sprite.cpp (GetGroundSpriteClassId, DestroyGroundSprite, ResetGroundSprite). */
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

/* Used by enemy_ctrl.cpp, ctrl.cpp (Ctrl, include/ctrl.hpp), input_ctrl_queue.cpp, action_ctrl.cpp. */
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

/* Used by enemy_ctrl_update.cpp, enemy_ctrl.cpp. */
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

/* Used by enemy_ctrl.cpp. */
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

/* Used by enemy_ctrl_update.cpp, enemy_ctrl.cpp. */
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

/* Used by effect_ctrl.cpp (EffectCtrl, include/ctrl.hpp). */
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

/* Used by crate.cpp (DestroyCrate), crate_create.cpp (CreateCrate). */
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

/* Used by extra_life.cpp (UpdateExtraLife). */
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

/* Used by wumpa.cpp (DestroyWumpa, ResetWumpaPickup), wumpa_update.cpp
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

/* Used by wumpa.cpp (UpdateStopwatch, DestroyStopwatch). */
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

/* Used by wumpa.cpp, action_ctrl.cpp. */
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

/* Used by swim_ctrl.cpp (DestroyPlayerCtrl). */
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

/* Used by input_ctrl.cpp (DestroyInputCtrl). */
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

/* Used by input_ctrl_queue.cpp (BossCtrl, include/boss_ctrl.hpp). The bosses' controller base class: Mega-Mix,
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

/* Used by mega_mix.cpp and mega_mix_update.cpp (MegaMixCtrl,
 * include/boss_ctrl.hpp). */
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

/* Used by tiny_hop_pad.cpp (StompedHopPadCtrl, include/ctrl.hpp). */
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

/* Used by cortex.cpp (OneShotAnimCtrl, include/ctrl.hpp) and
 * tiny_hop_pad.cpp. */
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

/* Used by cortex.cpp (UnusedOneShotAnimCtrl, include/boss_ctrl.hpp). */
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

/* Used by tiny_update.cpp and cortex.cpp (TinyCtrl, include/boss_ctrl.hpp). */
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

/* Used by cortex.cpp (CortexBossGemCtrl, include/boss_ctrl.hpp). */
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

/* Used by cortex.cpp (CortexBossPlatformMover, include/platform.hpp). */
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

/* Used by cortex.cpp (CortexShotCtrl, include/boss_ctrl.hpp). */
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

/* Used by dingodile.cpp and cortex.cpp (CortexTargetCtrl). */
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

/* Used by dingodile.cpp (DestroyCortexCannonCtrl). */
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

/* Used by dingodile.cpp and cortex.cpp (CortexBossCtrl). */
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

/* Used by dingodile.cpp (UpdateDingodileShark, DestroyDingodileSharkCtrl). */
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

/* Used by dingodile.cpp (SpawnDingodileStalactite, DestroyDingodileProjectileCtrl). */
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

/* Used by dingodile.cpp (DestroyDingodileShieldCtrl), dingodile_create.cpp,
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

/* Used by dingodile.cpp, dingodile_create.cpp (DestroyDingodile),
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

/* Used by platform.cpp (DestroyPlatform, InitPlatform, CreatePlatform). */
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

/* Used by platform.cpp (DestroyPlatformMover, CreatePlatformMover). */
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

/* Used by level_select.cpp (DestroyCameraLead). */
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

/* Used by level_select.cpp (GetCameraLeadOffset, DestroyLaunchPad, ClearLaunchPadVulnerable). */
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

/* Used by level_select_widgets.cpp (DestroyLevelSelectEntry). */
const struct vtable_slot gLevelSelectEntryVtable[6] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(AnimateLevelSelectEntry),
    VTABLE_SLOT(SetLevelSelectEntryLevel),
    VTABLE_SLOT(SetLevelSelectEntryPos),
    VTABLE_SLOT(DrawLevelSelectEntry),
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

/* Used by palette_cycle.cpp (InitHudPart, DestroyHudPart). */
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
    VTABLE_SLOT(DestroyHudPart),
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
 * Used by language_select.cpp (DestroyLogoActor), actor_anim.c (DestroyRiderlessPolar,
 * DestroyPolarCheckpointText, DestroyPolarWumpa, DestroyPolarTimeCrate, DestroyPolarQuestionCrate, DestroyPolarAkuAkuCrate,
 * DestroyPolarNitroCrate, DestroyPolarLifeCrate, DestroyPolarFourWumpaCrate, DestroyPolarBasicCrate, DestroyPolarCrate,
 * DestroyPolarElectricFence, DestroyPolarObstacle, DestroyPolarLauncher, DestroyPolarPenguin, DestroyPolarIcicle,
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
const struct vtable_slot gPolarFourWumpaCrateVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarFourWumpaCrate),
    VTABLE_SLOT(UpdatePolarFourWumpaCrate),
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
const struct vtable_slot gPolarObstacleVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPolarObstacle),
    VTABLE_SLOT(UpdatePolarObstacle),
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
    VTABLE_SLOT(NULL),       VTABLE_SLOT(DestroyJetpackShot), VTABLE_SLOT(UpdateJetpackShot),
    VTABLE_SLOT(DrawActor),  VTABLE_SLOT(DamageActor),        VTABLE_SLOT(IsJetpackShotUnshootable),
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
    VTABLE_SLOT(NULL),       VTABLE_SLOT(DestroyJetpackRing), VTABLE_SLOT(UpdateJetpackRing),
    VTABLE_SLOT(DrawActor),  VTABLE_SLOT(DamageActor),        VTABLE_SLOT(IsJetpackRingUnshootable),
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

/* Used by language_select.cpp (DestroyLogoActor), title_screen_init.cpp,
 * title_screen.cpp, company_logos.cpp (LoadUniversalLogoBg). */
const struct vtable_slot gLogoActorVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyLogoActor),
    VTABLE_SLOT(UpdateLogoActor),
    VTABLE_SLOT(DrawLogoActor),
};
