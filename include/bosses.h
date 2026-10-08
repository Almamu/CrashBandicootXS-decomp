#ifndef GUARD_BOSSES_H
#define GUARD_BOSSES_H

/* The bosses (src/bosses/): the airship and hovercraft boss fights of
 * the jetpack levels, and the Tiny, Dingodile, Cortex and Mega Mix
 * boss objects. Every function they define is declared here, including
 * those that the file layout put in actor or vehicle files for ROM order
 * (the airship and hovercraft teardown functions in actor_anim.c, the
 * hovercraft spawners in jetpack_spawn.cpp, ...), plus their globals and
 * data tables.
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions; the methods of the C++ files (include/boss_ctrl.hpp)
 * take `void *`. Some take a file-local view of their object, declared
 * here only by tag. A .c file that needs a different local declaration
 * for codegen keeps it as an asm-label alias with a `codegen:` comment
 * (docs/headers_plan.md). */

#include "core.h"
#include "actor.h"
#include "objects.h"
#include "player.h"

/* The airship's attack parameters, one per kind and level
 * (gAirshipAttacks, src/data/weapon_kind_17c2d0.c). SpawnAirship points
 * gAirshipAttack at one. AirshipStateFireballs fires a fireball every
 * `fireballDelay` frames, resting `fireballBurstDelay` frames after each
 * `fireballBurst`-th; AirshipStateCannon does the same with cannonballs
 * (the same delay/burst/burstDelay scheme as `struct spawn_timing`). */
struct airship_attack {
    s32 hp;                 // 0x00
    s32 fireballDelay;      // 0x04
    s32 fireballBurst;      // 0x08
    s32 fireballBurstDelay; // 0x0C - also the first fire timer (SpawnAirship)
    s32 cannonDelay;        // 0x10 - also the first one on entering AirshipStateCannon
    s32 cannonBurst;        // 0x14
    s32 cannonBurstDelay;   // 0x18
};

/* One spawner's timing: after each spawn it waits `delay` frames, except
 * every `burst`-th spawn, which resets its count and waits `burstDelay`
 * instead. The side gun reads the same three words as its orbit
 * `period`, `laps` and `cyclePeriod`. */
struct spawn_timing {
    s32 delay;      // 0x00
    s32 burst;      // 0x04
    s32 burstDelay; // 0x08
};

/* The hovercraft's attack parameters, one per kind and level
 * (gHovercraftAttacks, src/data/singleton_kind_17c460.c). SpawnHovercraft
 * points gHovercraftAttack at one, and GetHovercraftAttack returns it. */
struct hovercraft_attack {
    s32 hp;                        // 0x00 - copied to gHovercraftHp, which nothing reads
    struct spawn_timing timing[3]; // 0x04 - per spawner kind: [0] the side
                                   //        gun, [1] the cannon, [2] the launcher
};

/* actor_anim.h, and the file-local views of the objects (defined in the
 * .c files that use them). */
struct anim_box;
struct entry_set;
struct gobj;

/* src/bosses/airship.cpp */
extern void SteerAirship(void);
extern void CreateAirship(s32 level);
extern void SpawnAirship(s32 kind, s32 x, s32 y, s32 z);
extern void UpdateAirship(void);
extern void UpdateAirshipBg2(void);

/* src/bosses/airship_damage.cpp */
extern void DamageAirship(s32 delta);

/* src/bosses/airship_explode.cpp */
extern void AirshipStateExplode(void);

/* src/bosses/airship_fall.cpp */
extern void AirshipStateFall(void);

/* src/bosses/airship_graphics.cpp */
extern void ConvertAirshipTiles(void);
extern void UpdateAirshipFlashColor(void);
extern void AnimateAirshipPalette(void);

/* src/bosses/airship_load_graphics.cpp */
extern void LoadAirshipGraphics(void);

/* src/bosses/airship_map.cpp */
extern void DrawAirshipMap(u16 *src);

/* src/bosses/airship_states.cpp */
extern void AirshipStateApproach(void);
extern void AirshipStateFireballs(void);
extern void AirshipStateCannon(void);

/* src/bosses/airship_touch.cpp */
extern u8 IsTouchingAirship(void *self);

/* src/bosses/cortex.cpp: methods of TinyCtrl, CortexBossCtrl,
 * CortexTargetCtrl and CortexShotCtrl (include/boss_ctrl.hpp) under their
 * C names (cxx_symbols.txt). nullsub_19 and SpawnCortexBossGem
 * (spawn_gems.cpp calls it) have C linkage. */
extern void nullsub_19(void *self, void *part);
extern void SpawnCortexBossGem(u32 a0, u16 a1, u16 a2, u16 a3, s32 kind);

/* src/bosses/hovercraft.cpp */
extern void UpdateHovercraftHitFlash(void);
extern void RunHovercraftState(void);
extern void HovercraftStateCloseIn(void);
extern void HovercraftStateFallBack(void);
extern void HovercraftStateFall(void);
extern void DrawHovercraftMap(void *tileRow);
extern void CreateHovercraft(s32 level);
extern void SpawnHovercraft(s32 kind, s32 x, s32 y, s32 z);
extern void UpdateHovercraft(void);
extern void UpdateHovercraftBg2(void);
extern void LoadHovercraftGraphics(void);
extern void ConvertHovercraftTiles(void);
extern void DestroyHovercraft(void);
extern void nullsub_34(void);
extern s32 sub_80337FC(void);
extern void nullsub_35(void);

/* src/bosses/hovercraft_parts.cpp */
extern void StartHovercraftHitFlash(void);
extern void SetHovercraftFlashColor(u8 flag);
extern s32 GetHovercraftPartsLeft(void);
extern void LoseHovercraftPart(void);
extern const struct hovercraft_attack *GetHovercraftAttack(void);
extern s32 GetHovercraftState(void);
extern s32 GetHovercraftLevel(void);
extern s32 GetHovercraftZ(void);
extern s32 GetHovercraftY(void);
extern s32 GetHovercraftX(void);
extern void SetHovercraftState(s32 a0, s32 a1);
extern void HovercraftStateInactive(void);
extern void HovercraftStateApproach(void);
extern void HovercraftStateExplodeStub(void);

/* src/vehicle/jetpack_balloon.cpp (C linkage) */
extern s32 GetAirshipHpPercent(void);
extern void DestroyAirship(void);
extern void AirshipStateInactive(void);

/* src/vehicle/jetpack_spawn.cpp */
extern void SpawnHovercraftCannonFlash(s32 a, s32 b, s32 c);
/* `left` is the side gun's `bool` (HovercraftSideGun's constructor); only
 * C++ calls it. */
#ifdef __cplusplus
extern void SpawnHovercraftSideGun(s32 a, s32 b, s32 c, bool left);
#else
extern void SpawnHovercraftSideGun(s32 a, s32 b, s32 c, u8 left);
#endif
extern void SpawnHovercraftLauncher(s32 a, s32 b, s32 c);
extern void SpawnHovercraftCannon(s32 a, s32 b, s32 c);
extern void SpawnHovercraftFireball(s32 x, s32 y, s32 z);
extern void SpawnAirshipFireball(s32 x, s32 y, s32 z);

/* The bosses' globals (sym_iwram.txt). The airship is a bare AnimPart
 * (actor_self.hpp) in C++; the C files only use its first 0x1C bytes. */
#ifdef __cplusplus
extern class AnimPart *gAirship;
#else
extern struct actor_self *gAirship;
#endif
extern const struct airship_attack *gAirshipAttack;
extern s32 gAirshipBg2Page;
extern u8 gAirshipBg2PageFlip;
extern s32 gAirshipCheckpointCount;
extern s32 gAirshipDistance;
extern s32 gAirshipFireTimer;
extern s32 gAirshipHitFlashTimer;
extern s32 gAirshipHp;
extern s32 gAirshipLevel;
extern s32 gAirshipMapCols;
extern u8 *gAirshipMapFrames[];
extern s32 gAirshipMapRows;
extern s32 gAirshipMapTileBase;
extern s32 gAirshipScreenX;
extern s32 gAirshipScreenY;
extern s32 gAirshipState;
extern s32 gAirshipStateTimer;
extern s32 gAirshipVelX;
extern s32 gAirshipVelY;
extern s32 gAirshipVelZ;
extern s32 gAirshipVolleyCount;
extern s32 gAirshipX;
extern s32 gAirshipY;
extern s32 gAirshipZ;
/* The hovercraft is a bare AnimPart too (part 11h), and only C++ uses it. */
#ifdef __cplusplus
extern class AnimPart *gHovercraft;
#else
extern struct actor_self *gHovercraft;
#endif
extern const struct hovercraft_attack *gHovercraftAttack;
extern s32 gHovercraftBg2Page;
extern u8 gHovercraftBg2PageFlip;
extern s32 gHovercraftDistance;
extern s32 gHovercraftFlashColorSaved;
extern u16 gHovercraftFlashSavedColor;
extern s32 gHovercraftFrameCount;
extern u8 gHovercraftGone;
extern u8 gHovercraftHitFlashOn;
extern s16 gHovercraftHitFlashTimer;
extern s32 gHovercraftLevel;
extern s32 gHovercraftMapCols;
extern void *gHovercraftMapFrames[];
extern s32 gHovercraftMapRows;
extern s32 gHovercraftMapTileBase;
extern s32 gHovercraftOrbitRadius;
extern s32 gHovercraftPartsLeft;
extern s32 gHovercraftPhase;
extern s32 gHovercraftScreenX;
extern s32 gHovercraftScreenY;
extern s32 gHovercraftState;
extern s32 gHovercraftVelX;
extern s32 gHovercraftVelY;
extern s32 gHovercraftVelZ;
extern s32 gHovercraftX;
extern s32 gHovercraftY;
extern s32 gHovercraftZ;
/* The twins of gAirshipHp/gAirshipFireTimer/gAirshipVolleyCount (same
 * place in the same IWRAM layout, set the same way by SpawnHovercraft
 * and the state functions), but nothing reads them: the hovercraft's
 * parts keep their own hit points and spawn timers. */
extern s32 gHovercraftHp;
extern s32 gHovercraftFireTimer;
extern s32 gHovercraftVolleyCount;

/* src/data/weapon_kind_17c2d0.c */
extern const struct airship_attack gAirshipAttacks[6];
extern const struct anim_box gAirshipBox;
extern const u16 gAirshipHitFlashPalettes[3][16];
extern const struct anim_frame_record gAirshipKeyframes[2];

/* src/data/boss_pictures_167ad4.c */
extern const u16 gAirshipPalette[256];
extern const u16 gHovercraftPalette[256];
/* The two boss pictures: a {cols, rows} head, then the frames (docs/data.md
 * "Boss pictures"). Each picture's struct is sized by its generated picture
 * header (build/.../boss_pictures/<addr>.h), so it is only complete in the
 * data file; the code reads the head through BOSS_PICTURE_SIZE, and finds
 * the frames from the palette (gAirshipPalette + 0x204). */
struct boss_picture_size {
    s16 cols;
    s16 rows;
};
struct airship_picture;    /* 4 frames */
struct hovercraft_picture; /* 1 frame */
extern const struct airship_picture gAirshipPicture;
extern const struct hovercraft_picture gHovercraftPicture;
#define BOSS_PICTURE_SIZE(picture) ((const struct boss_picture_size *)&(picture))

/* src/data/actor_state_17c3fc.c */
extern void (*const gAirshipStateFuncs[6])(void);

/* src/data/actor_tables_16c2d8.c */
extern const u8 gCortexTargetBlinkStartTimes[3];
extern const u8 gCortexTargetBlinkStopTimes[3];
extern const u8 gCortexTargetChaseSteps[3];
extern const u8 gCortexTargetHopSteps[4];
extern const struct speed_ramp gDingodileMotionRecords[4];
extern const s32 gDingodileRocketRiseMotion[3];
extern const s32 gDingodileStalactiteFallMotion[9];
extern const s32 gDingodileStopXLeft[4];
extern const s32 gDingodileStopXLeftHurt[6];
extern const s32 gDingodileStopXRight[4];
extern const s32 gDingodileStopXRightHurt[6];
extern const struct speed_ramp gMegaMixMotionRecords[4];
extern const u8 gTinyHopTargets[77];
extern const u8 gTinyRoundAnchors[3];

/* src/iwram/iwram_data.c */
extern u16 *gFlashBgPalette;
extern u16 *gFlashObjPalette;

/* src/data/singleton_kind_17c460.c */
extern const struct hovercraft_attack gHovercraftAttacks[2];
extern const struct anim_box gHovercraftBox;
extern const struct anim_frame_record gHovercraftKeyframes[1];

/* src/data/actor_state_17c4c8.cpp, with HovercraftCannon's and
 * HovercraftLauncher's state tables (boss_actors.hpp); the fireball's is
 * in actor_pmf_17c450.cpp */
extern void (*const gHovercraftStateFuncs[6])(void);

/* src/data/player_pmf_16c250.cpp */
extern const struct entry_set gMegaMixMotionSet;

/* The {a, b} motion record index pairs (src/data/entry_set_16c418.c):
 * CreateDingodile and friends index gDingodileMotionRecords with entries
 * 0-3; 4-7 are gPlatformMoverMotionSet's (objects.h). */
extern const u32 gDingodileMotionEntries[8][2];

#endif /* !GUARD_BOSSES_H */
