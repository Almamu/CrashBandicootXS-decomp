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
 * their definitions. Many take a file-local view of their object (`struct
 * gfx_ctrl`, `struct obj_4704`, ...), declared here only by tag. A .c
 * file that needs a different local declaration for codegen keeps it as
 * an asm-label alias with a `codegen:` comment (docs/headers_plan.md). */

#include "core.h"
#include "actor.h"
#include "objects.h"

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

/* The airship's attack parameters, one per kind and level
 * (gAirshipAttacks, src/data/weapon_kind_17c2d0.c): seven words, none
 * named yet. SpawnAirship points gAirshipAttack at one. */
struct weapon_kind {
    s32 unk_00; // 0x00 - the airship's hit points
    s32 unk_04; // 0x04
    s32 unk_08; // 0x08
    s32 unk_0C; // 0x0C - the first fire timer
    s32 unk_10; // 0x10
    s32 unk_14; // 0x14
    s32 unk_18; // 0x18
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
struct singleton_kind {
    s32 unk_00;                    // 0x00
    struct spawn_timing timing[3]; // 0x04 - per spawner kind: [0] the side
                                   //        gun, [1] the cannon, [2] the launcher
};

/* actor_anim.h, and the file-local views of the objects (defined in the
 * .c files that use them). */
struct anim_box;
struct ab_part;
struct ab_self;
struct actor_orbit;
struct dingodile_boss;
struct entry_set;
struct gfx_ctrl;
struct gfx_hit_ctrl;
struct gfx_kind_ctrl;
struct gfx_mover;
struct gfx_offset_ctrl;
struct gfx_pair_ctrl;
struct gfx_part;
struct gfx_squares;
struct gobj;
struct hop_part;
struct obj_4704;
struct obj_476c;
struct obj_483c;
struct obj_48a4;
struct obj_490c;
struct part;
struct seq_obj;
struct tiny_tiger;
struct vobj;

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

/* src/bosses/cortex.c */
extern void *CreateOneShotAnimCtrl(struct gfx_ctrl *self);
extern void DestroyOneShotAnimCtrl(struct gfx_ctrl *self, s32 flags);
extern void UpdateUnusedOneShotAnimCtrl(struct gfx_ctrl *self, struct gfx_part *part);
extern void *CreateUnusedOneShotAnimCtrl(struct gfx_ctrl *self);
extern void DestroyUnusedOneShotAnimCtrl(struct gfx_ctrl *self, s32 flags);
extern void nullsub_19(void *self, void *part);
extern void StartTinyHop(struct gfx_offset_ctrl *self, struct gfx_part *part);
extern void DestroyTiny(struct gfx_squares *self, s32 flags);
extern void *CreateTiny(struct gfx_squares *self);
extern void UpdateCortexBoss(struct gfx_pair_ctrl *self, struct gfx_part *part);
extern void SpawnCortexCannon(struct gfx_pair_ctrl *self, struct gfx_part *part);
extern void SpawnCortexTarget(struct gfx_pair_ctrl *self, struct gfx_part *part);
extern void SpawnCortexBossGem(u32 a0, u16 a1, u16 a2, u16 a3, s32 kind);
extern void UpdateCortexTarget(struct gfx_mover *self, struct gfx_part *part);
extern void SetCortexTargetState(struct gfx_mover *self, struct gfx_part *part, s32 mode);
extern void FireCortexShot(struct gfx_mover *self, struct gfx_part *part, s32 kind);
extern void UpdateCortexShot(struct gfx_hit_ctrl *self, struct gfx_part *part);
extern void UpdateCortexBossPlatformMover(struct gfx_ctrl *self, struct gfx_part *part);
extern void UpdateCortexBossGem(struct gfx_kind_ctrl *self, struct gfx_part *part);
extern void DestroyCortexBossGemCtrl(struct gfx_ctrl *self, s32 flags);
extern void *CreateCortexBossGemCtrl(void *self, s32 kind);
extern void DestroyCortexBossPlatformMover(struct gfx_ctrl *self, s32 flags);
extern void *CreateCortexBossPlatformMover(struct gfx_ctrl *self);
extern void DestroyCortexShotCtrl(struct gfx_ctrl *self, s32 flags);
extern void *CreateCortexShotCtrl(void *self, void *cfg);

/* src/bosses/dingodile.c */
extern void sub_801967C(void *self, u8 flag);
extern void SetCortexTargetDest(struct obj_4704 *self, s32 *origin, s32 x, s32 y);
extern void DestroyCortexTargetCtrl(struct obj_4704 *self, s32 flags);
extern struct obj_4704 *CreateCortexTargetCtrl(struct obj_4704 *self, void *src);
extern void sub_8019718(struct vobj *self, s32 unused, s32 arg);
extern void UpdateCortexCannon(struct obj_476c *self, struct part *other);
extern void DestroyCortexCannonCtrl(struct obj_476c *self, s32 flags);
extern struct obj_476c *CreateCortexCannonCtrl(struct obj_476c *self);
extern void SetCortexBossState(struct obj_476c *self, s32 unused, s32 arg);
extern void DestroyCortexBoss(struct vobj *self, s32 flags);
extern struct vobj *CreateCortexBoss(struct vobj *self);
extern s32 GetDingodileHits(struct dingodile_boss *self);
extern void UpdateDingodile(struct dingodile_boss *self, struct part *other);
extern void SetDingodileState(struct dingodile_boss *self, struct part *other, s32 next);
extern void SpawnDingodileShieldOrRocket(struct dingodile_boss *self, s32 mode, u16 x, u16 y,
                                         struct part *arg);
extern void SpawnDingodileShark(struct dingodile_boss *self, u16 x, u16 y, u8 facing);
extern void UpdateDingodileShield(struct obj_490c *self, struct part *other);
extern void UpdateDingodileProjectile(struct obj_48a4 *self, struct part *other);
extern void SpawnDingodileStalactite(struct obj_48a4 *self, u16 x, u16 y);
extern void UpdateDingodileShark(struct obj_483c *self, struct part *other);
extern struct vobj *CreateDingodileSharkCtrl(void *mem);
extern void DestroyDingodileSharkCtrl(struct vobj *self, s32 flags);
extern void DestroyDingodileProjectileCtrl(struct obj_48a4 *self, s32 flags);
extern struct obj_48a4 *CreateDingodileProjectileCtrl(void *mem);
extern void DestroyDingodileShieldCtrl(struct vobj *self, s32 flags);

/* src/bosses/dingodile_create.c */
extern struct seq_obj *CreateDingodileShieldCtrl(struct seq_obj *self);
extern void StartDingodileMotion(void *self, struct gobj *part, s32 index);
extern void DestroyDingodile(struct seq_obj *self, s32 flags);
extern struct seq_obj *CreateDingodile(struct seq_obj *self, u32 a, u32 b);
extern void SetDingodileStep(struct seq_obj *self, s32 value);
extern void SetDingodileNextState(struct seq_obj *self, s32 value);

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
extern s32 sub_8034314(void *self);
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
extern const struct singleton_kind *GetHovercraftAttack(void);
extern s32 GetHovercraftState(void);
extern s32 GetHovercraftLevel(void);
extern s32 GetHovercraftZ(void);
extern s32 GetHovercraftY(void);
extern s32 GetHovercraftX(void);
extern void SetHovercraftState(s32 a0, s32 a1);
extern void HovercraftStateInactive(void);
extern void HovercraftStateApproach(void);
extern void nullsub_37(void);

/* src/bosses/hovercraft_side_gun.c */
extern void *CreateHovercraftSideGun(void *self, void *part, s32 b, s32 c, s32 d, u8 eByte);
extern void DamageHovercraftSideGun(void *self, s32 dmg);
extern void UpdateHovercraftSideGun(void *self);
extern void sub_80341F8(void *self);
extern u8 IsHovercraftSideGunUnshootable(void *self);
extern void DamageHovercraftCannonFlash(void);

/* src/bosses/mega_mix.c */
extern void SetMegaMixMotionYFromSet(void *self, void *part, s32 index);
extern void SetMegaMixMotionXFromSet(void *self, void *part, s32 index);
extern void StartMegaMixMotionYFromSet(void *self, void *part, s32 index);
extern void StartMegaMixMotionXFromSet(void *self, void *part, s32 index);
extern void ResetMegaMixCtrl(void *self);
extern void DestroyMegaMixCtrl(void *self, s32 flags);
extern void *CreateMegaMixCtrl(void *self);

/* src/bosses/mega_mix_update.c */
extern void UpdateMegaMix(struct ab_self *self, struct ab_part *other);

/* src/bosses/tiny_hop_pad.c */
extern void UpdateStompedHopPad(void *obj, void *other);
extern void DestroyStompedHopPadCtrl(void *self, s32 flags);
extern void *CreateStompedHopPadCtrl(void *self);
extern void UpdateOneShotAnimCtrl(void *unused, void *other);

/* src/bosses/tiny_update.c */
extern void UpdateTiny(struct tiny_tiger *self, struct hop_part *part);
extern void SetTinyState(struct tiny_tiger *self, struct hop_part *part, s32 next);
extern s32 PickTinyHopTarget(struct tiny_tiger *self);
extern void SpawnTinyFallingLeaves(struct tiny_tiger *self, struct hop_part *part, s32 n);

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
extern const struct weapon_kind *gAirshipAttack;
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
extern const struct singleton_kind *gHovercraftAttack;
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
extern s32 gUnknown_030015E0;
extern s32 gUnknown_030015E4;
extern s32 gUnknown_030015E8;

/* src/data/weapon_kind_17c2d0.c */
extern const struct weapon_kind gAirshipAttacks[6];
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
extern const s32 gMegaMixMotionRecords[4][3];
extern const u8 gTinyHopTargets[77];
extern const u8 gTinyRoundAnchors[3];

/* src/iwram/iwram_data.c */
extern void *gFlashBgPalette;
extern void *gFlashObjPalette;

/* src/data/singleton_kind_17c460.c */
extern const struct singleton_kind gHovercraftAttacks[2];
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
