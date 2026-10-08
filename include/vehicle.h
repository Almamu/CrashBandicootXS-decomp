#ifndef GUARD_VEHICLE_H
#define GUARD_VEHICLE_H

/* The vehicle levels (src/vehicle/): the jetpack levels (the player,
 * planes, bombers, balloons, crates and rings), the polar bear levels
 * (the player, crates, pickups, Aku Aku and obstacles) and the yeti
 * chase. Every function they define is declared here, including those
 * that the file layout put in actor or bosses files for ROM order (the
 * teardown functions in actor_anim.cpp, the per-category hooks in actor.cpp,
 * actor_category_frame.cpp and actor_spawn.cpp, the jetpack ring and
 * collected wumpa in hovercraft.cpp, ...), plus their globals and data
 * tables.
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions. Many take a file-local view of their object
 * (`struct jetpack_ring`, `struct actor_hp`, ...), declared here only
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

/* A jetpack ring (CreateJetpackRing, hovercraft.cpp; vtable
 * gJetpackRingVtable): `actor_self` plus hit points and a flag that makes
 * UpdateJetpackRing play its cue only once. The C view of vehicle.hpp's
 * class JetpackRing, same layout. */
struct jetpack_ring {
    struct actor_self base;
    s32 hp;  // 0x54
    u8 cued; // 0x58
};

/* The spawn argument of CreateJetpackPlane and PolarPenguin's
 * constructor (vehicle.hpp). */
struct spawn_arg {
    u8 unk_00[0x10];
    s32 target; // 0x10 - the first hop target index (AimJetpackPlane,
                //        PolarPenguin::Aim)
};

/* actor_anim.h, and the file-local views of the objects (defined in the
 * .c files that use them). */
struct anim_box;
struct anim_table_record;
struct actor_283c;
struct jetpack_spawn_rec;

/* src/actor/actor.cpp */
extern void JetpackReloadPlayerTiles(void *arg0);
extern void PolarReloadPlayerTiles(void *arg0);
extern void JetpackReachCourseEnd(void *arg0);
extern void PolarReachCourseEnd(void *arg0);

/* src/actor/actor_anim.cpp: the C-linkage copies of PolarCrate's inline
 * destructor and of the balloon crate kinds' implicit ones (vehicle.hpp),
 * which the g++-emitted vtables point at (cxx_symbols.txt) */
extern void DestroyPolarCrate(struct actor_self *self, u32 flags);
extern void DestroyJetpackHealthCrate(void *self, u32 flags);
extern void DestroyJetpackTimeCrate(void *self, u32 flags);
extern void DestroyJetpackQuestionCrate(void *self, u32 flags);

/* src/actor/actor_category_frame.cpp */
extern s32 PolarIsTouchingPlayer(void *self);
extern s32 JetpackIsTouchingPlayer(void *self);

/* src/actor/actor_factory.cpp */
extern void CreatePolarCheckpointText(s32 x, s32 y, s32 z);
extern void SpawnPolarCollectedWumpa(s32 x, s32 y, s32 z);
#ifdef __cplusplus
extern class PolarAkuAku *SpawnPolarAkuAku(s32 x, s32 y, s32 z, s32 arg);
#else
extern struct actor_self *SpawnPolarAkuAku(s32 x, s32 y, s32 z, s32 arg);
#endif

/* src/actor/actor_spawn.cpp */
extern s32 JetpackIsPauseLocked(void);
extern s32 PolarIsPauseLocked(void);

/* src/vehicle/jetpack_balloon.cpp: a C-linkage function */
extern void nullsub_30(void);

/* src/vehicle/jetpack_crates.cpp: JetpackBalloonCrate's destructor
 * (vehicle.hpp), for actor_anim.cpp's kinds' destructors */
extern void DestroyJetpackBalloonCrate(void *self, s32 flags);

/* src/vehicle/jetpack_player.cpp: JetpackPlayer's methods (vehicle.hpp)
 * that the C files use, and a C-linkage getter */
extern s32 CountJetpackBomber(void *player);
extern void SetJetpackCheckpoint(void *self);
extern s32 IsJetpackPauseLocked(void *player);
extern u8 IsJetpackPlayerInactive(void);

/* src/vehicle/jetpack_run.cpp: JetpackPlayer's (vehicle.hpp) */
extern void FinishJetpackRun(void *self);
extern void AllocJetpackPlayerTiles(void *self);

/* src/vehicle/jetpack_spawn.cpp: the spawners (C linkage) */
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

/* src/vehicle/polar_aku_aku.cpp, polar_crates.cpp, polar_objects.cpp and
 * polar_pickups.cpp: the C-linkage functions. */
extern s32 GetPolarMaskLevel(void);
extern u8 IsPolarPlayerInactive(void);

/* src/vehicle/polar_player.cpp, polar_player_states.cpp and
 * polar_player_actions.cpp: PolarPlayer's methods (vehicle.hpp) that
 * yeti_update.c (CatchPolarPlayer) and actor.cpp's and actor_spawn.cpp's
 * hooks call by their C names. */
extern void AllocPolarPlayerTiles(void *self);
extern s32 IsPolarPauseLocked(void *player);
extern void FinishPolarRun(void *player);
extern void CatchPolarPlayer(void *self);

/* src/vehicle/yeti.c */
extern void StopYeti(void);
extern void DestroyYeti(void);
extern void CreateYeti(void *arg0);
extern void BuildYetiBg2Map(u8 *dst, u8 seed);
extern void YetiStateCaught(void);

/* src/vehicle/yeti_graphics.c */
#ifdef __cplusplus
extern u8 IsTouchingYeti(class ActorSelf *self);
#else
extern u8 IsTouchingYeti(struct actor_self *self);
#endif
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
/* The polar actors the player keeps: ActorSelfs to the C++ files
 * (actor_self.hpp), as gYeti below. */
#ifdef __cplusplus
extern class PolarAkuAku *gPolarAkuAku;
#else
extern struct actor_self *gPolarAkuAku;
#endif
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
#ifdef __cplusplus
extern class ActorSelf *gRiderlessPolar;
#else
extern struct actor_self *gRiderlessPolar;
#endif
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
