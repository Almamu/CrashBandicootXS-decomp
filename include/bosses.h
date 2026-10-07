#ifndef GUARD_BOSSES_H
#define GUARD_BOSSES_H

/* The bosses (src/bosses/): the airship and hovercraft boss fights of
 * the jetpack levels, and the Tiny, Dingodile, Cortex and Mega Mix
 * boss objects. Every function they define is declared here, including
 * those that the file layout put in actor or vehicle files for ROM order
 * (the airship and hovercraft teardown functions in actor_anim.c, the
 * hovercraft spawners in jetpack_spawn.c, ...), plus their globals and
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

/* A spawner object of the hovercraft fight (the cannon, launcher and
 * side gun, hovercraft_parts.c): `actor_self` plus a hit-point word, its
 * spawn cooldown/count and a "dead" flag. */
struct spawner {
    struct actor_self base;
    s32 hp;     // 0x54
    s32 spawnX; // 0x58 - the constructor's `b`/`c` (CreateHovercraftLauncher)
    s32 spawnY; // 0x5C
    u8 unk_60[4];
    s32 cooldown; // 0x64
    s32 count;    // 0x68
    u8 dead;      // 0x6C
};

/* The airship's and the hovercraft's fireballs (CreateAirshipFireball,
 * CreateHovercraftFireball): `actor_self` plus hit points, the orbit
 * that AirshipFireballStateOrbit/AirshipFireballStateSpiralIn
 * (jetpack_plane.c) fly around the constructor's `b`/`c`, and an
 * "exploding" flag. The hovercraft's sets the orbit up but never reads
 * it; it only flies straight on at `velZ`. */
struct actor_orbit {
    struct actor_self base;
    s32 hp;       // 0x54
    s32 centerX;  // 0x58 - the constructor's `b`
    s32 centerY;  // 0x5C - the constructor's `c`
    s32 velZ;     // 0x60 - Z step, decays by 5 down to 0x14
    s32 radius;   // 0x64
    u8 exploding; // 0x68 - set by the StateExplode methods; the
                  //        Is...Unshootable getters return it
};

/* The hovercraft cannon's muzzle flash (CreateHovercraftCannonFlash,
 * vtable gHovercraftCannonFlashVtable): `actor_self` plus hit points and
 * a flag that is always set. */
struct cannon_flash {
    struct actor_self base;
    s32 hp;         // 0x54
    u8 unshootable; // 0x58
};

/* The Mega Mix controller (CreateMegaMixCtrl, vtable gMegaMixCtrlVtable):
 * a boss controller plus a frame stamp and a latch byte. The C view of
 * include/boss_ctrl.hpp's class MegaMixCtrl, same layout. */
struct mega_mix_ctrl {
    struct boss_ctrl base;
    s32 stamp; // 0x1C - reset to -1 by ResetMegaMixCtrl
    u8 latch;  // 0x20
};

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

/* src/actor/actor_anim.c */
extern void DestroyAirshipFireball(struct actor_self *self, u32 flags);
extern void DestroyHovercraftFireball(struct actor_self *self, u32 flags);
extern void DestroyHovercraftCannon(struct actor_self *self, u32 flags);
extern void DestroyHovercraftLauncher(struct actor_self *self, u32 flags);
extern void DestroyHovercraftSideGun(struct actor_self *self, u32 flags);
extern void DestroyHovercraftCannonFlash(struct actor_self *self, u32 flags);

/* src/bosses/airship.c */
extern void SteerAirship(void);
extern void CreateAirship(s32 level);
extern void SpawnAirship(s32 kind, s32 x, s32 y, s32 z);
extern void UpdateAirship(void);
extern void UpdateAirshipBg2(void);

/* src/bosses/airship_damage.c */
extern void DamageAirship(s32 delta);

/* src/bosses/airship_explode.c */
extern void AirshipStateExplode(void);

/* src/bosses/airship_fall.c */
extern void AirshipStateFall(void);

/* src/bosses/airship_fireball.c */
extern void DamageAirshipFireball(void *self, s32 delta);
extern void UpdateAirshipFireball(struct actor_self *self);
extern void *CreateAirshipFireball(void *self, void *part, s32 b, s32 c, s32 d);
extern void AirshipFireballStateExplode(void *self);
extern void RunAirshipFireballState(struct actor_self *self);
extern u8 IsAirshipFireballUnshootable(void *self);

/* src/bosses/airship_graphics.c */
extern void ConvertAirshipTiles(void);
extern void UpdateAirshipFlashColor(void);
extern void AnimateAirshipPalette(void);

/* src/bosses/airship_load_graphics.c */
extern void LoadAirshipGraphics(void);

/* src/bosses/airship_map.c */
extern void DrawAirshipMap(u16 *src);

/* src/bosses/airship_states.c */
extern void AirshipStateApproach(void);
extern void AirshipStateFireballs(void);
extern void AirshipStateCannon(void);

/* src/bosses/airship_touch.c */
extern u8 IsTouchingAirship(void *self);

/* src/bosses/cortex.cpp: the methods of OneShotAnimCtrl (include/ctrl.hpp),
 * UnusedOneShotAnimCtrl, TinyCtrl, CortexBossCtrl, CortexTargetCtrl,
 * CortexShotCtrl, CortexBossGemCtrl (include/boss_ctrl.hpp) and
 * CortexBossPlatformMover (include/platform.hpp) under their C names
 * (cxx_symbols.txt), for the vtables and the C callers (spawn_bosses.c,
 * spawn_gems.c). nullsub_19 and SpawnCortexBossGem have C linkage. */
extern void *CreateOneShotAnimCtrl(void *self);
extern void DestroyOneShotAnimCtrl(void *self, s32 flags);
extern void UpdateUnusedOneShotAnimCtrl(void *self, void *part);
extern void *CreateUnusedOneShotAnimCtrl(void *self);
extern void DestroyUnusedOneShotAnimCtrl(void *self, s32 flags);
extern void nullsub_19(void *self, void *part);
extern void StartTinyHop(void *self, void *part);
extern void DestroyTiny(void *self, s32 flags);
extern void *CreateTiny(void *self);
extern void UpdateCortexBoss(void *self, void *part);
extern void SpawnCortexCannon(void *self, void *part);
extern void SpawnCortexTarget(void *self, void *part);
extern void SpawnCortexBossGem(u32 a0, u16 a1, u16 a2, u16 a3, s32 kind);
extern void UpdateCortexTarget(void *self, void *part);
extern void SetCortexTargetState(void *self, void *part, s32 mode);
extern void FireCortexShot(void *self, void *part, s32 kind);
extern void UpdateCortexShot(void *self, void *part);
extern void UpdateCortexBossPlatformMover(void *self, void *part);
extern void UpdateCortexBossGem(void *self, void *part);
extern void DestroyCortexBossGemCtrl(void *self, s32 flags);
extern void *CreateCortexBossGemCtrl(void *self, s32 kind);
extern void DestroyCortexBossPlatformMover(void *self, s32 flags);
extern void *CreateCortexBossPlatformMover(void *self);
extern void DestroyCortexShotCtrl(void *self, s32 flags);
extern void *CreateCortexShotCtrl(void *self, void *boss);

/* src/bosses/dingodile.cpp and dingodile_create.cpp: the methods of
 * CortexTargetCtrl, CortexCannonCtrl, CortexBossCtrl, DingodileCtrl,
 * DingodileShieldCtrl, DingodileProjectileCtrl and DingodileSharkCtrl
 * (include/boss_ctrl.hpp) under their C names (cxx_symbols.txt), for the
 * vtables and the C callers (spawn_bosses.c). */
extern void SetCortexPlatformsKind(void *self, u8 flag);
extern void SetCortexTargetDest(void *self, void *part, s32 x, s32 y);
extern void DestroyCortexTargetCtrl(void *self, s32 flags);
extern void *CreateCortexTargetCtrl(void *self, void *boss);
extern void SetCortexCannonState(void *self, void *part, s32 next);
extern void UpdateCortexCannon(void *self, void *part);
extern void DestroyCortexCannonCtrl(void *self, s32 flags);
extern void *CreateCortexCannonCtrl(void *self);
extern void SetCortexBossState(void *self, void *part, s32 next);
extern void DestroyCortexBoss(void *self, s32 flags);
extern void *CreateCortexBoss(void *self);
extern s32 GetDingodileHits(void *self);
extern void UpdateDingodile(void *self, void *part);
extern void SetDingodileState(void *self, void *part, s32 next);
extern void SpawnDingodileShieldOrRocket(void *self, s32 mode, u16 x, u16 y, void *owner);
extern void SpawnDingodileShark(void *self, u16 x, u16 y, u8 facing);
extern void UpdateDingodileShield(void *self, void *part);
extern void UpdateDingodileProjectile(void *self, void *part);
extern void SpawnDingodileStalactite(void *self, u16 x, u16 y);
extern void UpdateDingodileShark(void *self, void *part);
extern void *CreateDingodileSharkCtrl(void *self);
extern void DestroyDingodileSharkCtrl(void *self, s32 flags);
extern void DestroyDingodileProjectileCtrl(void *self, s32 flags);
extern void *CreateDingodileProjectileCtrl(void *self);
extern void DestroyDingodileShieldCtrl(void *self, s32 flags);
extern void *CreateDingodileShieldCtrl(void *self);
extern void StartDingodileMotion(void *self, void *part, s32 index);
extern void DestroyDingodile(void *self, s32 flags);
extern void *CreateDingodile(void *self, u32 x, u32 y);
extern void SetDingodileStep(void *self, s32 value);
extern void SetDingodileNextState(void *self, s32 value);

/* src/bosses/hovercraft.c */
extern void DamageHovercraftFireball(void *self, s32 delta);
extern void UpdateHovercraftFireball(void *self);
extern void *CreateHovercraftFireball(void *self, void *part, s32 b, s32 c, s32 d);
extern void HovercraftFireballStateExplode(void *self);
extern void HovercraftFireballStateFly(void *self);
extern void RunHovercraftFireballState(void *self);
extern u8 IsHovercraftFireballUnshootable(void *self);
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

/* src/bosses/hovercraft_cannon.c */
extern void HovercraftCannonStateFire(struct spawner *self);
extern void DamageHovercraftCannon(struct spawner *self, s32 dmg);
extern void UpdateHovercraftCannon(struct actor_self *self);
extern void *CreateHovercraftCannon(void *self, void *part, s32 b, s32 c, s32 d);
extern void HovercraftCannonStateDestroyed(void *self);
extern void HovercraftCannonStateWait(void *self);
extern s32 RunHovercraftCannonState(struct actor_self *self);
extern u8 IsHovercraftCannonUnshootable(void *self);

/* src/bosses/hovercraft_cannon_flash.c */
extern void UpdateHovercraftCannonFlash(void *self);
extern void *CreateHovercraftCannonFlash(void *self, void *part, s32 b, s32 c, s32 d);
extern s32 RunHovercraftCannonFlashState(void *self);
extern u8 IsHovercraftCannonFlashUnshootable(void *self);

/* src/bosses/hovercraft_launcher.c */
extern void HovercraftLauncherStateLaunch(struct spawner *self);
extern void DamageHovercraftLauncher(struct spawner *self, s32 dmg);
extern void UpdateHovercraftLauncher(struct actor_self *self);
extern void *CreateHovercraftLauncher(void *self, void *part, s32 b, s32 c, s32 d);
extern void HovercraftLauncherStateDestroyed(void *self);
extern void HovercraftLauncherStateWait(void *self);
extern s32 RunHovercraftLauncherState(struct actor_self *self);
extern u8 IsHovercraftLauncherUnshootable(void *self);

/* src/bosses/hovercraft_parts.c */
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

/* src/bosses/hovercraft_side_gun.c */
extern void *CreateHovercraftSideGun(void *self, void *part, s32 b, s32 c, s32 d, u8 eByte);
extern void DamageHovercraftSideGun(void *self, s32 dmg);
extern void UpdateHovercraftSideGun(void *self);
extern void RunHovercraftSideGunState(void *self);
extern u8 IsHovercraftSideGunUnshootable(void *self);
extern void DamageHovercraftCannonFlash(void);

/* src/bosses/mega_mix.cpp and mega_mix_update.cpp: MegaMixCtrl's methods
 * (include/boss_ctrl.hpp) under their C names (cxx_symbols.txt), for the
 * vtables and the C callers. */
extern void SetMegaMixMotionYFromSet(void *self, void *part, s32 index);
extern void SetMegaMixMotionXFromSet(void *self, void *part, s32 index);
extern void StartMegaMixMotionYFromSet(void *self, void *part, s32 index);
extern void StartMegaMixMotionXFromSet(void *self, void *part, s32 index);
extern void ResetMegaMixCtrl(void *self);
extern void DestroyMegaMixCtrl(void *self, s32 flags);
extern void *CreateMegaMixCtrl(void *self);
extern void UpdateMegaMix(void *self, void *part);

/* src/bosses/tiny_hop_pad.cpp: StompedHopPadCtrl's methods and
 * OneShotAnimCtrl's Update (include/ctrl.hpp)
 * under their C names (cxx_symbols.txt), for the vtables and the C
 * callers. */
extern void UpdateStompedHopPad(void *obj, void *other);
extern void DestroyStompedHopPadCtrl(void *self, s32 flags);
extern void *CreateStompedHopPadCtrl(void *self);
extern void UpdateOneShotAnimCtrl(void *unused, void *other);

/* src/bosses/tiny_update.cpp: TinyCtrl's methods (include/boss_ctrl.hpp)
 * under their C names (cxx_symbols.txt), for the vtable. */
extern void UpdateTiny(void *self, void *part);
extern void SetTinyState(void *self, void *part, s32 next);
extern s32 PickTinyHopTarget(void *self);
extern void SpawnTinyFallingLeaves(void *self, void *part, s32 n);

/* src/vehicle/jetpack_balloon.c */
extern s32 GetAirshipHpPercent(void);
extern void DestroyAirship(void);
extern void AirshipStateInactive(void);

/* src/vehicle/jetpack_plane.c */
extern void AirshipFireballStateOrbit(struct actor_orbit *self);
extern void AirshipFireballStateSpiralIn(struct actor_orbit *self);

/* src/vehicle/jetpack_spawn.c */
extern void SpawnHovercraftCannonFlash(s32 a, s32 b, s32 c);
extern void SpawnHovercraftSideGun(s32 a, s32 b, s32 c, u8 d);
extern void SpawnHovercraftLauncher(s32 a, s32 b, s32 c);
extern void SpawnHovercraftCannon(s32 a, s32 b, s32 c);
extern void SpawnHovercraftFireball(s32 x, s32 y, s32 z);
extern void SpawnAirshipFireball(s32 x, s32 y, s32 z);

/* The bosses' globals (sym_iwram.txt). */
extern struct actor_self *gAirship;
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
extern struct actor_self *gHovercraft;
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

/* src/data/actor_pmf_17c260.c */
extern const struct actor_pmf gAirshipFireballStateFuncs[3];

/* src/data/entity_vtables_7e3bec.c */
extern const struct vtable_slot gAirshipFireballVtable[7];
extern const struct vtable_slot gCortexBossGemVtable[13];
extern const struct vtable_slot gCortexBossPlatformMoverVtable[13];
extern const struct vtable_slot gCortexBossVtable[13];
extern const struct vtable_slot gCortexCannonVtable[13];
extern const struct vtable_slot gCortexShotVtable[13];
extern const struct vtable_slot gCortexTargetVtable[13];
extern const struct vtable_slot gDingodileProjectileVtable[13];
extern const struct vtable_slot gDingodileSharkVtable[13];
extern const struct vtable_slot gDingodileShieldVtable[13];
extern const struct vtable_slot gDingodileVtable[13];
extern const struct vtable_slot gHovercraftCannonFlashVtable[7];
extern const struct vtable_slot gHovercraftCannonVtable[7];
extern const struct vtable_slot gHovercraftFireballVtable[7];
extern const struct vtable_slot gHovercraftLauncherVtable[7];
extern const struct vtable_slot gHovercraftSideGunVtable[7];
extern const struct vtable_slot gMegaMixCtrlVtable[13];
extern const struct vtable_slot gOneShotAnimCtrlVtable[13];
extern const struct vtable_slot gStompedHopPadVtable[13];
extern const struct vtable_slot gTinyVtable[13];
extern const struct vtable_slot gUnusedOneShotAnimCtrlVtable[13];

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

/* src/data/actor_state_17c4c8.c */
extern const struct actor_pmf gHovercraftCannonStateFuncs[3];
extern const struct actor_pmf gHovercraftLauncherStateFuncs[3];
extern void (*const gHovercraftStateFuncs[6])(void);

/* src/data/actor_pmf_17c450.c */
extern const struct actor_pmf gHovercraftFireballStateFuncs[2];

/* src/data/player_pmf_16c250.c */
extern const struct entry_set gMegaMixMotionSet;

/* The {a, b} motion record index pairs (src/data/entry_set_16c418.c):
 * CreateDingodile and friends index gDingodileMotionRecords with entries
 * 0-3; 4-7 are gPlatformMoverMotionSet's (objects.h). */
extern const u32 gDingodileMotionEntries[8][2];

#endif /* !GUARD_BOSSES_H */
