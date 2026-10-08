#ifndef GUARD_VEHICLE_H
#define GUARD_VEHICLE_H

/* The vehicle levels (src/vehicle/): the jetpack levels (the player,
 * planes, bombers, balloons, crates and rings), the polar bear levels
 * (the player, crates, pickups, Aku Aku and obstacles) and the yeti
 * chase. Every function they define is declared here, including those
 * that the file layout put in actor or bosses files for ROM order (the
 * teardown functions in actor_anim.cpp, the per-category hooks in actor.cpp,
 * actor_category_frame.cpp and actor_spawn.cpp, the jetpack ring and
 * collected wumpa in hovercraft.c, ...), plus their globals and data
 * tables.
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions. Many take a file-local view of their object
 * (`struct jetpack_plane`, `struct actor_hp`, ...), declared here only
 * by tag. A .c file that needs a different local declaration for codegen
 * keeps it as an asm-label alias with a `codegen:` comment
 * (docs/headers_plan.md). */

#include "core.h"
#include "actor.h"

/* `actor_self` plus the hit-point word the jetpack actors keep at +0x54
 * (the player's is refilled by PassJetpackRing, capped at
 * gJetpackPlayerMaxHp). */
struct actor_hp {
    struct actor_self base;
    s32 hp; // 0x54
};

/* A jetpack ring (CreateJetpackRing, hovercraft.c; vtable
 * gJetpackRingVtable): `actor_self` plus hit points and a flag that makes
 * UpdateJetpackRing play its cue only once. */
struct jetpack_ring {
    struct actor_self base;
    s32 hp;  // 0x54
    u8 cued; // 0x58
};

/* A polar life crate (CreatePolarLifeCrate, polar_crates.c; vtable
 * gPolarLifeCrateVtable): `actor_self` plus the spawn record that
 * UpdatePolarLifeCrate hands to MarkSpawnCollected's 15-entry list. */
struct polar_life_crate {
    struct actor_self base;
    void *spawn; // 0x54 - CreatePolarLifeCrate's 6th argument
};

/* The spawn argument of CreateJetpackPlane and CreatePolarPenguin. */
struct spawn_arg {
    u8 unk_00[0x10];
    s32 target; // 0x10 - the first hop target index (AimJetpackPlane,
                //        AimPolarPenguin)
};

/* An actor box (`struct anim_box`, actor_anim.h) copied as three words:
 * this shape, rather than the 6-halfword struct or three separate `s32`
 * copies, gives the ROM's `ldm`/`stm` when a fixed box is copied into an
 * actor's `box` (jetpack_crates.c, polar_objects.c, polar_pickups.c). */
struct vec3_words {
    s32 a, b, c;
};

/* actor_anim.h, and the file-local views of the objects (defined in the
 * .c files that use them). */
struct anim_box;
struct anim_table_record;
struct actor_283c;
struct actor_fa38;
struct actor_once;
struct jetpack_balloon;
struct jetpack_bomber;
struct jetpack_cannonball;
struct jetpack_plane;
struct jetpack_spawn_rec;

/* src/actor/actor.cpp */
extern void JetpackReloadPlayerTiles(void *arg0);
extern void PolarReloadPlayerTiles(void *arg0);
extern void JetpackReachCourseEnd(void *arg0);
extern void PolarReachCourseEnd(void *arg0);

/* src/actor/actor_anim.cpp: the C names of the classes' destructors and
 * small methods (vehicle.hpp), for the vtable data */
extern void DestroyRiderlessPolar(struct actor_self *self, u32 flags);
extern void UpdatePolarCheckpointText(void *self);
extern void DestroyPolarCheckpointText(struct actor_self *self, u32 flags);
extern void DestroyPolarWumpa(struct actor_self *self, u32 flags);
extern void DestroyPolarTimeCrate(struct actor_self *self, u32 flags);
extern void DestroyPolarQuestionCrate(struct actor_self *self, u32 flags);
extern void DestroyPolarAkuAkuCrate(struct actor_self *self, u32 flags);
extern void DestroyPolarNitroCrate(struct actor_self *self, u32 flags);
extern void DestroyPolarLifeCrate(struct actor_self *self, u32 flags);
extern void DestroyPolarBasicCrate(struct actor_self *self, u32 flags);
extern void DestroyPolarCrate(struct actor_self *self, u32 flags);
extern void DestroyPolarElectricFence(struct actor_self *self, u32 flags);
extern void DestroyPolarLauncher(struct actor_self *self, u32 flags);
extern void DestroyPolarPenguin(struct actor_self *self, u32 flags);
extern void DestroyPolarIcicle(struct actor_self *self, u32 flags);
extern void DestroyPolarAkuAku(struct actor_self *self, u32 flags);
extern void DestroyPolarGoal(struct actor_self *self, u32 flags);
extern void DestroyPolarBoostPad(struct actor_self *self, u32 flags);
extern void DestroyPolarCheckpointCrate(struct actor_self *self, u32 flags);
extern void DrawJetpackCheckpointText(void *self);
extern void UpdateJetpackCheckpointText(void *self);
extern s32 IsJetpackCheckpointTextUnshootable(void);
extern void DestroyJetpackCheckpointText(struct actor_self *self, u32 flags);
extern void UpdateJetpackExplosion(void *self);
extern s32 IsJetpackExplosionUnshootable(void);
extern void DestroyJetpackExplosion(struct actor_self *self, u32 flags);
extern s32 IsJetpackPlayerUnshootable(void);
extern void DestroyJetpackShot(struct actor_self *self, u32 flags);
extern void DestroyJetpackPlane(struct actor_self *self, u32 flags);
extern void DestroyJetpackBomber(struct actor_self *self, u32 flags);
extern void DestroyJetpackCannonball(struct actor_self *self, u32 flags);
extern void DestroyJetpackBalloon(struct actor_self *self, u32 flags);
extern void DestroyJetpackHealthCrate(void *self, u32 flags);
extern void DestroyJetpackTimeCrate(void *self, u32 flags);
extern void DestroyJetpackQuestionCrate(void *self, u32 flags);
extern void DestroyJetpackParachuteNitro(struct actor_self *self, u32 flags);
extern void DestroyJetpackRocket(struct actor_self *self, u32 flags);
extern void DestroyJetpackRing(struct actor_self *self, u32 flags);

/* src/actor/actor_category_frame.cpp */
extern s32 PolarIsTouchingPlayer(void *self);
extern s32 JetpackIsTouchingPlayer(void *self);

/* src/actor/actor_factory.cpp */
extern void CreatePolarCheckpointText(s32 x, s32 y, s32 z);
extern void SpawnPolarCollectedWumpa(s32 x, s32 y, s32 z);
#ifdef __cplusplus
extern class ActorSelf *SpawnPolarAkuAku(s32 x, s32 y, s32 z, s32 arg);
#else
extern struct actor_self *SpawnPolarAkuAku(s32 x, s32 y, s32 z, s32 arg);
#endif

/* src/actor/actor_spawn.cpp */
extern s32 JetpackIsPauseLocked(void);
extern s32 PolarIsPauseLocked(void);

/* src/bosses/hovercraft.c */
extern void *CreateJetpackRing(void *self, void *part, s32 b, s32 c, s32 d);
extern s32 IsJetpackRingUnshootable(void *self);
extern void UpdateJetpackCollectedWumpa(void *self);
extern void DrawJetpackCollectedWumpa(void *self);
extern void DestroyJetpackCollectedWumpa(struct actor_283c *self, u32 flags);
extern void *CreateJetpackCollectedWumpa(void *self, void *part, s32 b, s32 c, s32 spawn);
extern s32 IsJetpackCollectedWumpaUnshootable(void *self);

/* src/vehicle/jetpack_balloon.c */
extern void nullsub_30(void);
extern void UpdateJetpackBalloon(struct jetpack_balloon *self);
extern void ClearJetpackBalloonCrate(void *self);
extern void DamageJetpackBalloon(struct jetpack_balloon *self, s32 damage);
extern void ReleaseJetpackBalloon(void *self);
extern void MoveJetpackBalloon(struct actor_self *self, s32 x, s32 y, s32 z);
extern void *CreateJetpackBalloon(void *self, void *part, s32 b, s32 c, s32 d, s32 e);
extern void JetpackBalloonStatePop(struct actor_self *self);
extern void JetpackBalloonStateFloatAway(struct jetpack_balloon *self);
extern void JetpackBalloonStateAttached(void);
extern void RunJetpackBalloonState(struct actor_self *self);
extern u8 IsJetpackBalloonUnshootable(void *self);

/* src/vehicle/jetpack_crates.c */
extern void UpdateJetpackBalloonCrate(void *self);
extern void UpdateJetpackQuestionCrate(void *self);
extern void DamageJetpackQuestionCrate(void *self, s32 delta);
extern void UpdateJetpackHealthCrate(void *self);
extern void UpdateJetpackTimeCrate(void *self);
extern void DamageJetpackTimeCrate(void *self, s32 delta);
extern void *CreateJetpackTimeCrate(void *self, void *part, s32 b, s32 c, s32 d);
extern void DamageJetpackHealthCrate(void *self, s32 delta);
extern void *CreateJetpackHealthCrate(void *self, void *part, s32 b, s32 c, s32 d);
extern void *CreateJetpackQuestionCrate(void *self, void *part, s32 b, s32 c, s32 d, s32 e);
extern void ClearJetpackCrateBalloon(void *self);
extern void BreakJetpackBalloonCrate(void *self);
extern void DamageJetpackBalloonCrate(void *self, s32 delta);
extern void DestroyJetpackBalloonCrate(void *self, s32 flags);
extern void *InitJetpackBalloonCrate(void *self, void *part, s32 b, s32 c, s32 d, u8 kind);
extern void JetpackBalloonCrateStateDestroyed(void *self);
extern void JetpackBalloonCrateStateFall(void *self);
extern void JetpackBalloonCrateStateHang(void *self);
extern void RunJetpackBalloonCrateState(void *self);
extern u8 IsJetpackBalloonCrateUnshootable(void *self);
extern void UpdateJetpackParachuteNitro(void *self);
extern void DamageJetpackParachuteNitro(void *self, s32 delta);
extern void *CreateJetpackParachuteNitro(void *self, void *part, s32 b, s32 c, s32 d);
extern u8 IsJetpackParachuteNitroUnshootable(void *self);
extern void UpdateJetpackRocket(void *self);
extern void LaunchJetpackRocket(void *self);
extern void DamageJetpackRocket(void *self, s32 delta);
extern void *CreateJetpackRocket(void *self, void *part, s32 b, s32 c, s32 d);
extern u8 IsJetpackRocketUnshootable(void *self);
extern void UpdateJetpackRing(void *self);

/* src/vehicle/jetpack_plane.c */
extern void UpdateJetpackPlane(struct actor_fa38 *self);
extern void AimJetpackPlane(struct jetpack_plane *self, s32 target);
extern void DamageJetpackPlane(struct jetpack_plane *self, s32 damage);
extern void *CreateJetpackPlane(struct jetpack_plane *self, void *part, s32 b, s32 c, s32 d,
                                struct spawn_arg *arg);
extern void JetpackPlaneStateFall(struct jetpack_plane *self);
extern void JetpackPlaneStateKnockedOut(struct jetpack_plane *self);
extern void JetpackPlaneStateFollow(struct jetpack_plane *self);
extern void JetpackPlaneStateFly(struct jetpack_plane *self);
extern void RunJetpackPlaneState(struct jetpack_plane *self);
extern u8 IsJetpackPlaneUnshootable(struct jetpack_plane *self);
extern void *CreateJetpackBomber(struct jetpack_bomber *self, u8 *part, s32 b, s32 c, s32 d);
extern void UpdateJetpackBomber(struct jetpack_bomber *self);
extern void HomeJetpackBomber(struct jetpack_bomber *self);
extern void JetpackBomberStateDying(struct jetpack_bomber *self);
extern void JetpackBomberStateDrop(struct jetpack_bomber *self);
extern void JetpackBomberStateCircle(struct jetpack_bomber *self);
extern void JetpackBomberStateSwingHorizontal(struct jetpack_bomber *self);
extern void JetpackBomberStateBobVertical(struct jetpack_bomber *self);
extern void JetpackBomberStateHome(struct jetpack_bomber *self);
extern void JetpackBomberStateIdle(struct jetpack_bomber *self);
extern void DamageJetpackBomber(struct jetpack_bomber *self, s32 damage);
extern void RunJetpackBomberState(struct jetpack_bomber *self);
extern u8 IsJetpackBomberUnshootable(struct jetpack_bomber *self);
extern void UpdateJetpackCannonball(struct jetpack_cannonball *self);
extern void *CreateJetpackCannonball(struct jetpack_cannonball *self, void *part, s32 b, s32 c,
                                     s32 d, s32 velX, s32 velY);
extern s32 IsJetpackCannonballUnshootable(struct jetpack_cannonball *self);

/* src/vehicle/jetpack_player.cpp: JetpackPlayer's methods (vehicle.hpp)
 * that the C files and the vtable data use, and a C-linkage getter */
extern s32 CountJetpackBomber(void *player);
extern s32 GetJetpackPlayerHpPercent(void *self);
extern void SetJetpackCheckpoint(void *self);
extern s32 IsJetpackPauseLocked(void *player);
extern void HealJetpackPlayer(void *self, s32 delta);
extern void QueueJetpackWumpa(void *self, s32 delta);
extern void DestroyJetpackPlayer(void *self, s32 flags);
extern u8 IsJetpackPlayerInactive(void);

/* src/vehicle/jetpack_run.cpp: JetpackPlayer's (vehicle.hpp) */
extern void FinishJetpackRun(void *self);
extern void PassJetpackRing(void *self, s32 x, s32 y);
extern void AllocJetpackPlayerTiles(void *self);

/* src/vehicle/jetpack_shot.cpp: JetpackShot's (vehicle.hpp), for the
 * vtable data */
extern void UpdateJetpackShot(void *self);
extern s32 IsJetpackShotUnshootable(void);

/* src/vehicle/jetpack_spawn.cpp: the spawners (C linkage), and
 * JetpackPlayer's virtual methods (vehicle.hpp), for the vtable data */
extern void YetiStateStop(void);
extern void *SpawnJetpackActor(struct jetpack_spawn_rec *rec, u8 alt, s32 dz);
extern void *CreateJetpackActor(u8 kind, s32 x, s32 y, s32 z, void *spawn);
extern void CreateJetpackCheckpointText(void);
extern void CreateJetpackExplosion(s32 x, s32 y, s32 z);
extern void SpawnJetpackCollectedWumpa(s32 a, s32 b, s32 c);
extern void *SpawnJetpackBalloon(u8 kind, s32 a, s32 b, s32 c, s32 d);
extern void SpawnJetpackCannonball(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void SpawnJetpackShot(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void CreateJetpackPlayer(struct anim_table_record *table, s32 z);
extern void UpdateJetpackPlayer(void *self);
extern void DrawJetpackPlayer(void *self);
extern void DamageJetpackPlayer(void *self, s32 dmg);

/* src/vehicle/polar_aku_aku.c */
extern void ClearPolarAkuAkuMask(void *self);
extern s32 RemovePolarAkuAkuMask(void *self);
extern s32 AddPolarAkuAkuMask(void *self);
extern void *CreatePolarAkuAku(struct actor_self *self, void *part, s32 b, s32 c, s32 d, s32 sixth);
extern void SetPolarMaskLevel(void *arg0, s32 arg1);
extern s32 GetPolarMaskLevel(void);
extern void UpdatePolarGoal(void *self);
extern void *CreatePolarGoal(struct actor_self *self, void *part, s32 b, s32 c, s32 d);
extern void UpdatePolarBoostPad(void *self);
extern void *CreatePolarBoostPad(struct actor_once *self, void *part, s32 posY, s32 c, s32 d);
extern void UpdatePolarCheckpointCrate(void *self);
extern void *CreatePolarCheckpointCrate(struct actor_self *self, void *part, s32 b, s32 c, s32 d);

/* src/vehicle/polar_crates.c */
extern void UpdatePolarAkuAkuCrate(struct actor_self *self);
extern void UpdatePolarTimeCrate(void *self);
extern void DetonatePolarNitroCrate(void *self);
extern void UpdatePolarFourWumpaCrate(void *self);
extern void UpdatePolarBasicCrate(void *self);
extern void *InitPolarCrate(void *self, void *part, s32 b, s32 c, s32 last);
extern void *CreatePolarTimeCrate(void *self, void *part, s32 b, s32 c, s32 last);
extern void *CreatePolarQuestionCrate(void *self, void *part, s32 b, s32 c, s32 last);
extern void *CreatePolarAkuAkuCrate(void *self, void *part, s32 b, s32 c, s32 last);
extern void *CreatePolarNitroCrate(void *self, void *part, s32 b, s32 c, s32 last);
extern void *CreatePolarLifeCrate(void *self, void *part, s32 b, s32 c, s32 last, s32 arg6);
extern void *CreatePolarFourWumpaCrate(void *self, void *part, s32 b, s32 c, s32 last);
extern void *CreatePolarBasicCrate(void *self, void *part, s32 b, s32 c, s32 last);

/* src/vehicle/polar_nitro.c */
extern void DetonateNearbyPolarNitros(struct actor_self *self);

/* src/vehicle/polar_objects.c */
extern void UpdatePolarElectricFence(void *self);
extern void *CreatePolarElectricFence(void *self, void *part, s32 b, s32 c, s32 d);
extern void UpdatePolarObstacle(void *self);
extern void *CreatePolarObstacle(void *self, void *part, s32 b, s32 c, s32 d);
extern void UpdatePolarLauncher(void *self);
extern void *CreatePolarLauncher(void *self, void *part, s32 b, s32 c, s32 d);
extern void UpdatePolarPenguin(void *self);
extern void AimPolarPenguin(void *self, s32 target);
extern void *CreatePolarPenguin(void *self, void *part, s32 b, s32 c, s32 d, struct spawn_arg *e);
extern void UpdatePolarIcicle(void *self);
extern void *CreatePolarIcicle(void *self, u8 *b, s32 c, s32 d, s32 e);
extern void RefreshPolarAkuAku(void *self, s32 retrigger);
extern void UpdatePolarAkuAku(void *self);
extern void MovePolarAkuAku(struct actor_self *self, s32 posX, s32 posY, s32 posZ);

/* src/vehicle/polar_pickups.c */
extern u8 IsPolarPlayerInactive(void);
extern void UpdatePolarCollectedWumpa(void *self);
extern void DrawPolarCollectedWumpa(void *self);
extern void DestroyPolarCollectedWumpa(void *self, u32 arg1);
extern void *CreatePolarCollectedWumpa(void *self, void *part, s32 b, s32 c, s32 spawn);
extern void UpdatePolarWumpa(void *self);
extern void *CreatePolarWumpa(void *self, void *part, s32 b, s32 c, s32 last);
extern void UpdatePolarCrate(void *self);
extern void UpdatePolarQuestionCrate(void *self);
extern void UpdatePolarLifeCrate(void *self);
extern void UpdatePolarNitroCrate(void *self);

/* src/vehicle/polar_player.c */
extern void UpdatePolarPlayer(struct actor_self *self);
extern void DrawPolarPlayer(struct actor_self *self);
extern s32 HurtPolarPlayer(void *self);
extern s32 ShockPolarPlayer(void *self);
extern void AllocPolarPlayerTiles(struct actor_self *self);
extern void PolarPlayerStateMount(struct actor_self *self);
extern void PolarPlayerStateRun(void *self);
extern void PolarPlayerStateJump(struct actor_self *self);
extern void PolarPlayerStateDash(struct actor_self *self);
extern void PolarPlayerStateShocked(void *self);
extern void PolarPlayerStateCaught(struct actor_self *self);

/* src/vehicle/polar_player_actions.c */
extern void PolarPlayerStateLaunched(void *self);
extern void PolarPlayerStateFinish(void *self);
extern void PolarPlayerStateLand(void *self);
extern void FinishPolarRun(void *player);
extern void CatchPolarPlayer(void *self);
extern void QueuePolarWumpa(void *arg0, s32 delta);
extern void GivePolarPlayerLife(void *arg0);
extern void BoostPolarPlayer(void *self, s32 arg1);
extern void GivePolarPlayerMask(void *arg0);
extern void LaunchPolarPlayer(void *self);
extern void DestroyPolarPlayer(void *self, u32 arg1);

/* src/vehicle/polar_player_dispatch.cpp: PolarPlayer::RunState (vehicle.hpp) */
extern void RunPolarPlayerState(struct actor_self *self);

/* src/vehicle/polar_player_states.c */
extern void DispensePolarWumpa(void *self);
extern s32 IsPolarPauseLocked(void *player);
extern void PolarPlayerStateRecover(void *self);
extern void PolarPlayerStateFinishLeap(void *self);
extern void PolarPlayerStateCarriedOff(void *self);
extern void PolarPlayerStateKnockedOff(void *self);
extern void PolarPlayerStateBoost(void *self);

/* src/vehicle/yeti.c */
extern void StopYeti(void);
extern void DestroyYeti(void);
extern void CreateYeti(void *arg0);
extern void BuildYetiBg2Map(u8 *dst, u8 seed);
extern void YetiStateCaught(void);

/* src/vehicle/yeti_graphics.c */
extern u8 IsTouchingYeti(struct actor_self *self);
extern void LoadYetiGraphics(void);

/* src/vehicle/yeti_states.c */
extern void YetiStateChase(void);
extern void YetiStateCharge(void);

/* src/vehicle/yeti_update.c */
extern void UpdateYeti(void);
extern void UpdateYetiPalette(void);
extern void UpdateYetiBg2(void);

/* The vehicles' globals (sym_iwram.txt). */
extern struct anim_table_record *gJetpackAnimTable; /* actor_anim.h */
extern s32 gJetpackBomberCount;
extern s32 gJetpackBomberSfxTimer;
extern u8 gJetpackFadeStarted;
extern s32 gJetpackFlashTimer;
extern u8 gJetpackInputEnabled;
extern u8 gJetpackPauseLocked;
extern u8 gJetpackPlayerHalted;
extern u8 *gJetpackPlayerLastFrame;
extern s32 gJetpackPlayerMaxHp;
extern s32 gJetpackPlayerTileBuffer;
extern void *gJetpackPlayerTiles[2];
extern s32 gJetpackPlayerVelX;
extern s32 gJetpackPlayerVelY;
extern s32 gJetpackQueuedWumpa;
extern s32 gJetpackRingChain;
extern s32 gJetpackRingLastFrame;
extern s32 gJetpackShotCooldown;
extern s32 gJetpackWumpaDispenseTimer;
extern struct actor_self *gPolarAkuAku;
extern s32 gPolarAkuAkuInvincibleTimer;
extern u8 gPolarFadeStarted;
extern s32 gPolarFinishTimer;
extern s32 gPolarInvulnTimer;
extern u8 gPolarPauseLocked;
extern u8 gPolarPlayerHalted;
extern u8 gPolarPlayerInactive;
extern u8 *gPolarPlayerLastFrame;  // the frame last uploaded
extern s32 gPolarPlayerTileBuffer; // which buffer holds the current frame
extern void *gPolarPlayerTiles[2]; // the two VRAM tile buffers
extern s32 gPolarPlayerVelY;
extern s32 gPolarQueuedWumpa;
extern u8 gPolarSteerEnabled;
extern s32 gPolarSteerTime;
extern s32 gPolarWumpaDispenseTimer;
extern struct actor_self *gRiderlessPolar;
/* The yeti's actor: an ActorSelf to the C++ files (actor_self.hpp), the
 * C files see its C view. */
#ifdef __cplusplus
extern class ActorSelf *gYeti;
#else
extern struct actor_self *gYeti;
#endif
extern u8 gYetiBg2Page;
extern u8 gYetiBg2PageFlip;
extern s32 gYetiDistance;
extern s32 gYetiParamsIndex;
extern s32 gYetiPosition;
extern s32 gYetiState;
extern s32 gYetiX;

/* src/data/actor_state_17c3fc.c */
extern const struct actor_pmf gJetpackBalloonCrateStateFuncs[3];
extern const struct actor_pmf gJetpackBalloonStateFuncs[3];

/* src/data/entity_vtables_7e3bec.c */
extern const struct vtable_slot gJetpackBalloonCrateVtable[8];
extern const struct vtable_slot gJetpackBalloonVtable[7];
extern const struct vtable_slot gJetpackBomberVtable[7];
extern const struct vtable_slot gJetpackCannonballVtable[7];
extern const struct vtable_slot gJetpackCheckpointTextVtable[7];
extern const struct vtable_slot gJetpackCollectedWumpaVtable[7];
extern const struct vtable_slot gJetpackExplosionVtable[7];
extern const struct vtable_slot gJetpackHealthCrateVtable[8];
extern const struct vtable_slot gJetpackParachuteNitroVtable[7];
extern const struct vtable_slot gJetpackPlaneVtable[7];
extern const struct vtable_slot gJetpackPlayerVtable[7];
extern const struct vtable_slot gJetpackQuestionCrateVtable[8];
extern const struct vtable_slot gJetpackRingVtable[7];
extern const struct vtable_slot gJetpackRocketVtable[7];
extern const struct vtable_slot gJetpackShotVtable[7];
extern const struct vtable_slot gJetpackTimeCrateVtable[8];
extern const struct vtable_slot gPolarAkuAkuCrateVtable[4];
extern const struct vtable_slot gPolarAkuAkuVtable[4];
extern const struct vtable_slot gPolarBasicCrateVtable[4];
extern const struct vtable_slot gPolarBoostPadVtable[4];
extern const struct vtable_slot gPolarCheckpointCrateVtable[4];
extern const struct vtable_slot gPolarCheckpointTextVtable[4];
extern const struct vtable_slot gPolarCollectedWumpaVtable[4];
extern const struct vtable_slot gPolarCrateVtable[4];
extern const struct vtable_slot gPolarElectricFenceVtable[4];
extern const struct vtable_slot gPolarGoalVtable[4];
extern const struct vtable_slot gPolarIcicleVtable[4];
extern const struct vtable_slot gPolarLauncherVtable[4];
extern const struct vtable_slot gPolarLifeCrateVtable[4];
extern const struct vtable_slot gPolarNitroCrateVtable[4];
extern const struct vtable_slot gPolarPenguinVtable[4];
extern const struct vtable_slot gPolarPlayerVtable[4];
extern const struct vtable_slot gPolarQuestionCrateVtable[4];
extern const struct vtable_slot gPolarTimeCrateVtable[4];
extern const struct vtable_slot gPolarWumpaVtable[4];
extern const struct vtable_slot gRiderlessPolarVtable[4];
extern const struct vtable_slot gPolarObstacleVtable[4];

/* src/data/actor_pmf_17c260.c */
extern const struct actor_pmf gJetpackBomberStateFuncs[7];
extern const struct actor_pmf gJetpackPlaneStateFuncs[4];

/* src/data/palette_strip_17c200.c */
extern const u16 gJetpackFlashPalettes[3][16];

/* src/data/actor_box_17c444.c */
extern const struct anim_box gJetpackRocketBox;

/* src/data/actor_tables_17a728.c */
extern const u16 gPolarAkuAkuPalette1[16];
extern const u16 gPolarAkuAkuPalette2[16];
extern const u16 gPolarAkuAkuPalette3[16];
extern const struct anim_box gPolarElectricFenceLeftPostBox;
extern const struct anim_box gPolarElectricFenceRightPostBox;
extern const struct anim_box gPolarElectricFenceWireBox;
extern const struct anim_box gPolarNitroCrateBox;
extern const u16 gPolarPlayerShockBlinkPalette[16];
extern const u16 gPolarPlayerShockPalette[16];
extern const s32 gYetiChargeParams[6][3];

/* src/data/actor_pmf_17a6b8.cpp: PolarPlayer::stateFuncs (vehicle.hpp), whose
 * C view this is */
extern const struct actor_pmf gPolarPlayerStateFuncs[14];

/* src/iwram/iwram_data.c */
extern s32 gPolarPenguinSpeeds[3];
extern s32 gJetpackPlaneHopSpeeds[6];
extern void (*gUnpackNibbleTilesFunc)(u16 *src, s32 lowBlock);

/* src/data/anim_family_17aa6c.c */
extern const struct anim_box gYetiBox;
extern const struct anim_box gYetiCatchBox;
extern const u16 gYetiPalette[16];

/* src/data/frame_table_17a880.c */
extern const u8 *const gYetiFrames[123];

/* src/data/anim_frames_17a850.c */
extern const struct anim_frame_record gYetiKeyframes[4];

/* src/data/actor_state_fn_17a840.c */
extern void (*const gYetiStateFuncs[4])(void);

#endif /* !GUARD_VEHICLE_H */
