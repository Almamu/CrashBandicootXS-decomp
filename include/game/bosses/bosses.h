#ifndef GUARD_BOSSES_H
#define GUARD_BOSSES_H

/* The bosses (src/game/bosses/): the airship and hovercraft boss fights of
 * the jetpack levels, and the Tiny, Dingodile, Cortex and Mega Mix
 * boss objects. Every function they define is declared here, including
 * those that the file layout put in actor or vehicle files for ROM order
 * (the airship and hovercraft teardown functions in actor_destructors.cpp, the
 * hovercraft spawners in jetpack_spawn.cpp, ...), plus their globals and
 * data tables.
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions; the methods of the C++ files (include/game/bosses/boss_ctrl.hpp)
 * take `void *`. Some take a file-local view of their object, declared
 * here only by tag. A .c file that needs a different local declaration
 * for codegen keeps it as an asm-label alias with a `codegen:` comment
 * (docs/headers.md). */

#include "core.h"
#include "engine/actor/actor.h"
#include "game/entities/objects.h"

/* The airship's attack parameters, one per kind and level
 * (gAirshipAttacks, src/game/vehicles/jetpack/data/airship_attacks.cpp). SpawnAirship points
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
 * (Hovercraft::attacks, src/game/bosses/data/hovercraft_attacks.cpp). SpawnHovercraft
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

/* src/game/bosses/tiny.cpp and cortex.cpp: methods of TinyCtrl, CortexBossCtrl,
 * CortexTargetCtrl and CortexShotCtrl (include/game/bosses/boss_ctrl.hpp) under their
 * C names (cxx_symbols.txt). SpawnCortexBossGem (cortex.cpp; spawn_gems.cpp
 * calls it) has C linkage. */
/* Its `kind` (CortexBossGemCtrl::kind; #843, docs/flags/boss_ctrl.md):
 * the gem spawner that hands over, which picks bank 32's gem animation
 * and bank 53's shrink animation. */
#define CORTEX_BOSS_GEM_KIND_RED 0    // SpawnRedGem: animation 3, shrinks with 0xC
#define CORTEX_BOSS_GEM_KIND_GREEN 1  // SpawnGreenGem: animation 2, shrinks with 0xB
#define CORTEX_BOSS_GEM_KIND_YELLOW 2 // SpawnYellowGem: animation 0, shrinks with 0xD
extern void SpawnCortexBossGem(u32 id, u16 x, u16 y, u16 param, s32 kind);

/* The bosses' globals (sym_iwram.txt): the airship's and the
 * hovercraft's variables are Airship's and Hovercraft's static members
 * (include/game/vehicles/jetpack/airship.hpp, include/game/bosses/hovercraft.hpp; #772). */

/* src/game/bosses/data/boss_pictures.cpp: the two boss pictures, a {cols, rows}
 * head, then the frames (docs/data.md "Boss pictures"). Each picture's struct is sized by its generated picture
 * header (build/.../vehicles/jetpack/airship.h, bosses/hovercraft.h), so it is only complete in the
 * data file; the code reads the head through BOSS_PICTURE_SIZE, and finds
 * the frames from the palette (gAirshipPalette + 0x204). The airship's
 * tables (gAirshipPalette, gAirshipPicture, gAirshipAttacks,
 * gAirshipBox, gAirshipHitFlashPalettes, gAirshipKeyframes) are
 * Airship's static data members (include/game/vehicles/jetpack/airship.hpp), and the
 * hovercraft's (gHovercraftPalette, gHovercraftPicture, gHovercraftAttacks,
 * gHovercraftBox, gHovercraftKeyframes) Hovercraft's
 * (include/game/bosses/hovercraft.hpp). */
struct boss_picture_size {
    s16 cols;
    s16 rows;
};
struct airship_picture;    /* 4 frames */
struct hovercraft_picture; /* 1 frame */
#define BOSS_PICTURE_SIZE(picture) ((const struct boss_picture_size *)&(picture))

/* src/game/bosses/data/boss_motion_tables.cpp */
extern const u8 gCortexTargetBlinkStartTimes[3];
extern const u8 gCortexTargetBlinkStopTimes[3];
extern const u8 gCortexTargetChaseSteps[3];
extern const u8 gCortexTargetHopSteps[4];
extern const struct speed_ramp gDingodileMotionRecords[4];
extern const struct speed_ramp gDingodileRocketRiseMotion;
extern const struct speed_ramp gDingodileStalactiteFallMotion[3];
extern const s32 gDingodileStopXLeft[4];
extern const s32 gDingodileStopXLeftHurt[6];
extern const s32 gDingodileStopXRight[4];
extern const s32 gDingodileStopXRightHurt[6];
extern const struct speed_ramp gMegaMixMotionRecords[4];
extern const u8 gTinyHopTargets[77];
extern const u8 gTinyRoundAnchors[3];

/* The bosses' speed-ramp tuning values (`struct speed_ramp`, Q8 px a
 * frame and px a frame per frame, docs/fixed_point.md):
 * - Mega-Mix (gMegaMixMotionRecords, MegaMixCtrl::Update): it runs after
 *   the player at up to MEGA_MIX_RUN_SPEED (record 1), at up to
 *   MEGA_MIX_CATCH_UP_SPEED while off screen (record 2), and, turned back
 *   (X-mirrored) after a player who got behind it, from
 *   MEGA_MIX_RUN_SPEED up to MEGA_MIX_TURN_BACK_SPEED (record 3);
 * - Dingodile (gDingodileMotionRecords, DingodileCtrl::StartMotion): his
 *   walk (entry 1: walk, walk back, retreat) speeds up by
 *   DINGODILE_WALK_ACCEL to DINGODILE_WALK_SPEED; a hit (entry 2) throws
 *   him back at DINGODILE_HIT_RECOIL_SPEED, slowing by
 *   DINGODILE_HIT_RECOIL_DECEL to a stop;
 * - his rocket (gDingodileRocketRiseMotion) rises speeding up by
 *   DINGODILE_ROCKET_RISE_ACCEL, and the stalactite it drops
 *   (gDingodileStalactiteFallMotion[0]) falls speeding up by
 *   DINGODILE_STALACTITE_FALL_ACCEL. UpdateDingodileProjectile reads only
 *   that first record: the third's start and step,
 *   DINGODILE_STALACTITE_UNK_START (up) and DINGODILE_STALACTITE_UNK_STEP,
 *   are never read. */
#define MEGA_MIX_RUN_SPEED              Q8_FRAC(1, 194)
#define MEGA_MIX_CATCH_UP_SPEED         Q8_FRAC(2, 108)
#define MEGA_MIX_TURN_BACK_SPEED        Q8_FRAC(2, 238)
#define DINGODILE_WALK_ACCEL            Q8_FRAC(0, 180)
#define DINGODILE_WALK_SPEED            Q8_FRAC(1, 44)
#define DINGODILE_HIT_RECOIL_SPEED      Q8_FRAC(1, 144)
#define DINGODILE_HIT_RECOIL_DECEL      Q8_FRAC(0, 30)
#define DINGODILE_ROCKET_RISE_ACCEL     Q8_FRAC(0, 7)
#define DINGODILE_STALACTITE_FALL_ACCEL Q8_FRAC(0, 4)
#define DINGODILE_STALACTITE_UNK_START  Q8_FRAC(2, 88)
#define DINGODILE_STALACTITE_UNK_STEP   Q8_FRAC(0, 10)

/* src/engine/sys/iwram_data.cpp */
extern u16 *gFlashBgPalette;
extern u16 *gFlashObjPalette;

/* src/game/crash/data/swim_input_ctrl_state_tables.cpp */
extern const struct entry_set gMegaMixMotionSet;

/* The {a, b} motion record index pairs (src/game/bosses/data/dingodile_motion_entries.cpp):
 * CreateDingodile and friends index gDingodileMotionRecords with entries
 * 0-3; 4-7 are gPlatformMoverMotionSet's (objects.h). */
extern const u32 gDingodileMotionEntries[8][2];

#endif /* !GUARD_BOSSES_H */
