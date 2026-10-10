#ifndef GUARD_BOSSES_H
#define GUARD_BOSSES_H

/* The bosses (src/bosses/): the airship and hovercraft boss fights of
 * the jetpack levels, and the Tiny, Dingodile, Cortex and Mega Mix
 * boss objects. Every function they define is declared here, including
 * those that the file layout put in actor or vehicle files for ROM order
 * (the airship and hovercraft teardown functions in inline_copies_actors.cpp, the
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
 * (gAirshipAttacks, src/data/weapon_kind_17c2d0.cpp). SpawnAirship points
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
 * (Hovercraft::attacks, src/data/singleton_kind_17c460.cpp). SpawnHovercraft
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

/* src/bosses/tiny.cpp and cortex.cpp: methods of TinyCtrl, CortexBossCtrl,
 * CortexTargetCtrl and CortexShotCtrl (include/boss_ctrl.hpp) under their
 * C names (cxx_symbols.txt). SpawnCortexBossGem (cortex.cpp; spawn_gems.cpp
 * calls it) has C linkage. */
extern void SpawnCortexBossGem(u32 a0, u16 a1, u16 a2, u16 a3, s32 kind);

/* src/vehicle/jetpack/jetpack_spawn.cpp */
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

/* The bosses' globals (sym_iwram.txt): the airship's and the
 * hovercraft's variables are Airship's and Hovercraft's static members
 * (include/airship.hpp, include/hovercraft.hpp; #772). */

/* src/data/boss_pictures_167ad4.c: the two boss pictures, a {cols, rows}
 * head, then the frames (docs/data.md "Boss pictures"). Each picture's struct is sized by its generated picture
 * header (build/.../boss_pictures/<addr>.h), so it is only complete in the
 * data file; the code reads the head through BOSS_PICTURE_SIZE, and finds
 * the frames from the palette (gAirshipPalette + 0x204). The airship's
 * tables (gAirshipPalette, gAirshipPicture, gAirshipAttacks,
 * gAirshipBox, gAirshipHitFlashPalettes, gAirshipKeyframes) are
 * Airship's static data members (include/airship.hpp), and the
 * hovercraft's (gHovercraftPalette, gHovercraftPicture, gHovercraftAttacks,
 * gHovercraftBox, gHovercraftKeyframes) Hovercraft's
 * (include/hovercraft.hpp). */
struct boss_picture_size {
    s16 cols;
    s16 rows;
};
struct airship_picture;    /* 4 frames */
struct hovercraft_picture; /* 1 frame */
#define BOSS_PICTURE_SIZE(picture) ((const struct boss_picture_size *)&(picture))

/* src/data/actor_tables_16c2d8.cpp */
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

/* src/iwram/iwram_data.cpp */
extern u16 *gFlashBgPalette;
extern u16 *gFlashObjPalette;

/* src/data/player_pmf_16c250.cpp */
extern const struct entry_set gMegaMixMotionSet;

/* The {a, b} motion record index pairs (src/data/entry_set_16c418.c):
 * CreateDingodile and friends index gDingodileMotionRecords with entries
 * 0-3; 4-7 are gPlatformMoverMotionSet's (objects.h). */
extern const u32 gDingodileMotionEntries[8][2];

#endif /* !GUARD_BOSSES_H */
