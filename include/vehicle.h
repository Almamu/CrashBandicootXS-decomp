#ifndef GUARD_VEHICLE_H
#define GUARD_VEHICLE_H

/* The vehicle levels (src/vehicle/): the jetpack levels (the player,
 * planes, bombers, balloons, crates and rings), the polar bear levels
 * (the player, crates, pickups, Aku Aku and obstacles) and the yeti
 * chase. Every function they define is declared here, including those
 * that the file layout put in actor or bosses files for ROM order (the
 * teardown functions in inline_copies_actors.cpp, the per-category hooks in
 * actor_category_hooks.cpp, actor_category_frame.cpp and
 * actor_category.cpp, the jetpack ring and
 * collected wumpa in jetpack_collected_wumpa.cpp, ...), plus their globals and data
 * tables.
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions. A .c file that needs a different local declaration for codegen
 * keeps it as an asm-label alias with a `codegen:` comment
 * (docs/headers_plan.md). */

#include "core.h"
#include "actor.h"

/* The spawn argument of CreateJetpackPlane and PolarPenguin's
 * constructor (vehicle.hpp): the level spawn record CreateJetpackActor and
 * CreateActor were handed (SpawnJetpackActor, SpawnActor), and the word
 * after it, the next sub_effect_record's `link` (actor_anim.h). */
struct spawn_arg {
    struct actor_spawn spawn; // 0x00 (actor.h)
    s32 target;               // 0x10 - the first hop target index (AimJetpackPlane,
                              //        PolarPenguin::Aim)
};

/* actor_anim.h, and the file-local views of the objects (defined in the
 * .c files that use them). */
struct anim_box;
struct anim_table_record;

/* src/actor/actor_category_hooks.cpp */
extern void JetpackReloadPlayerTiles(void);
extern void PolarReloadPlayerTiles(void);
extern void JetpackReachCourseEnd(void);
extern void PolarReachCourseEnd(void);

/* src/actor/inline_copies_actors.cpp: the C-linkage copies of the balloon crate
 * kinds' implicit destructors (vehicle.hpp), which the g++-emitted
 * vtables point at (cxx_symbols.txt); PolarCrate's is in vehicle.hpp */
extern void DestroyJetpackHealthCrate(void *self, u32 flags);
extern void DestroyJetpackTimeCrate(void *self, u32 flags);
extern void DestroyJetpackQuestionCrate(void *self, u32 flags);

/* src/actor/actor_category_frame.cpp */
extern s32 PolarIsTouchingPlayer(struct ActorSelf *self);
extern s32 JetpackIsTouchingPlayer(struct ActorSelf *self);

/* src/actor/actor_factory.cpp */
extern void CreatePolarCheckpointText(s32 x, s32 y, s32 z);
extern void SpawnPolarCollectedWumpa(s32 x, s32 y, s32 z);
#ifdef __cplusplus
extern class PolarAkuAku *SpawnPolarAkuAku(s32 x, s32 y, s32 z, s32 arg);
#endif

/* src/actor/actor_category.cpp */
extern s32 JetpackIsPauseLocked(void);
extern s32 PolarIsPauseLocked(void);

/* src/vehicle/jetpack/jetpack_crates.cpp: JetpackBalloonCrate's destructor
 * (vehicle.hpp), for inline_copies_actors.cpp's kinds' destructors */
extern void DestroyJetpackBalloonCrate(void *self, s32 flags);

/* src/vehicle/jetpack/jetpack_player.cpp: a C-linkage getter */
extern u8 IsJetpackPlayerInactive(void);

/* src/vehicle/jetpack/jetpack_spawn.cpp: the spawners (C linkage) */
extern struct ActorSelf *SpawnJetpackActor(struct actor_spawn *rec, u8 alt, s32 dz);
extern struct ActorSelf *CreateJetpackActor(u8 kind, s32 x, s32 y, s32 z,
                                            struct actor_spawn *spawn);
extern void CreateJetpackCheckpointText(void);
extern void CreateJetpackExplosion(s32 x, s32 y, s32 z);
extern void SpawnJetpackCollectedWumpa(s32 a, s32 b, s32 c);
extern void *SpawnJetpackBalloon(u8 kind, s32 a, s32 b, s32 c, s32 d);
extern void SpawnJetpackCannonball(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void SpawnJetpackShot(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void CreateJetpackPlayer(struct anim_table_record *table, s32 z);

/* src/vehicle/polar/polar_aku_aku.cpp and polar_player_states.cpp: the
 * C-linkage functions. */
extern s32 GetPolarMaskLevel(void);
extern u8 IsPolarPlayerInactive(void);

/* The yeti's category-table slots (#765), by the C names of their Yeti
 * static members (include/yeti.hpp, cxx_symbols.txt), for the C category
 * table (src/data/actor_category_175558.c). C++ calls Yeti::Create & co. */
#ifndef __cplusplus
extern void CreateYeti(s32 level);
extern void UpdateYeti(void);
extern void UpdateYetiBg2(void);
extern void DestroyYeti(void);
extern void LoadYetiGraphics(void);
#endif

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
#endif
/* The yeti's (gYeti, gYetiX, ...) are Yeti's static data members
 * (include/yeti.hpp). */

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

/* src/iwram/iwram_data.cpp */
extern s32 gPolarPenguinSpeeds[3];
extern s32 gJetpackPlaneHopSpeeds[6];
extern void (*gUnpackNibbleTilesFunc)(u16 *src, s32 lowBlock);

/* The yeti's tables (gYetiChargeParams, gYetiBox, gYetiCatchBox,
 * gYetiPalette, gYetiFrames, gYetiKeyframes, gYetiStateFuncs) are Yeti's
 * static data members (include/yeti.hpp). */

#endif /* !GUARD_VEHICLE_H */
