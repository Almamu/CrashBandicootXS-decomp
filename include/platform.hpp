#ifndef GUARD_PLATFORM_HPP
#define GUARD_PLATFORM_HPP

/* The platforms as C++ (#664, docs/cplusplus.md, part 7d): the classes
 * behind gPlatformVtable and gPlatformMoverVtable (src/objects/platform*.cpp),
 * and the Neo Cortex fight's mover subclass (part 7i, src/bosses/cortex.cpp).
 *
 * No `#pragma interface`: g++ emits the vtables, PlatformMover's in
 * platform_mover.cpp, Platform's in platform_contact.cpp and
 * CortexBossPlatformMover's in cortex.cpp (see ctrl.hpp); cxx_symbols.txt
 * maps the mangled names onto the C names. */

#include "sprite_obj.hpp"
#include "ctrl.hpp"

extern "C" {
#include "gobj_1a794.h"
}

class PlatformMover;

/* Platform::type and PlatformMover::kind (#843, docs/flags/platform.md):
 * what the platform does, from its spawn record or forced by the spawn
 * kind (Platform::Create). */
#define PLATFORM_TYPE_STILL 0         // stands still and carries the player
#define PLATFORM_TYPE_MOVING 1        // moves back and forth over the record's distances
#define PLATFORM_TYPE_WARP_EXIT 2     // EVENT_WARP_EXIT: the level exit
#define PLATFORM_TYPE_WARP_BONUS 3    // EVENT_WARP_BONUS_ROUND, until the bonus round is done
#define PLATFORM_TYPE_WARP_GEM_PATH 4 // EVENT_WARP_GEM_PATH, until the gem path is done
#define PLATFORM_TYPE_FALLING 5       // wobbles once the player lands, falls 60 frames later
#define PLATFORM_TYPE_FLIP 6          // tips over, and comes back 120 frames later
// SpawnRockPlatform's: carried like STILL; its mover holds the animation
// while not `active` and is gone when it ends, but nothing activates it
#define PLATFORM_TYPE_ROCK 7

/* Platform::Create's spawn `kind` (#843, docs/flags/platform.md): the
 * spawner's look, the platform's animation in sprite bank 0x1D4 (its
 * `tag`). The warp kinds force a PLATFORM_TYPE_*; the others keep the
 * record's type. */
#define PLATFORM_KIND_LARGE 0           // SpawnLargePlatform
#define PLATFORM_KIND_SMALL 1           // SpawnSmallPlatform
#define PLATFORM_KIND_MEDIUM 2          // SpawnMediumPlatform
#define PLATFORM_KIND_GEM_YELLOW 3      // SpawnYellowGemPlatform: WARP_GEM_PATH
#define PLATFORM_KIND_EXIT_PAD 4        // SpawnRoomExit's pad: WARP_EXIT
#define PLATFORM_KIND_BONUS 5           // SpawnBonusPlatform: WARP_BONUS
#define PLATFORM_KIND_FLIP 6            // SpawnFlipPlatform: FLIP
#define PLATFORM_KIND_BONUS_DONE 7      // SpawnBonusPlatform once the round is done or in a time trial
#define PLATFORM_KIND_ROCK 8            // SpawnRockPlatform: ROCK
#define PLATFORM_KIND_GEM_BLUE 9        // SpawnBlueGemPlatform: WARP_GEM_PATH
#define PLATFORM_KIND_GEM_GREEN 0xA     // SpawnGreenGemPlatform: WARP_GEM_PATH
#define PLATFORM_KIND_GEM_RED 0xB       // SpawnRedGemPlatform: WARP_GEM_PATH
#define PLATFORM_KIND_GEM_PATH_DONE 0xC // the gem platforms once the gem path is done or in a time trial

/* A level platform (CreatePlatform allocates 0x80 bytes): a moving sprite
 * with a `type` (PLATFORM_TYPE_*), from its spawn record or forced by the
 * spawn kind. Types 1 (a moving platform), 5 (one that falls after the
 * player lands), 6 (the flip platform, which tips over on a timer; the
 * Neo Cortex fight's are these) and 7 (the rock platform) get a
 * PlatformMover; 2-4 are the warp platforms (the level exit, the bonus
 * round, the gem path). */
class Platform : public MovingSprite
{
public:
    s32 type; // 0x78
    u8 unk_7C[4];

    Platform(); // InitPlatform: inline in platform_create.cpp (`new Platform`)
    virtual s32 CheckPlayerContact();                                   // 1 CheckPlatformContact
    virtual void Update();                                              // 3 UpdatePlatform
    virtual s32 GetClassId();                                           // 9 GetPlatformClassId: 4
    virtual ~Platform();                                                // 10 DestroyPlatform
    static Platform *Create(u16 id, u16 x, u16 y, u16 index, s32 kind); // CreatePlatform

    s32 GetExitMirror();                 // GetPlatformExitMirror
    void SetExitMirror(u8 value);        // SetPlatformExitMirror
    void ClearVulnerable();              // ClearPlatformVulnerable: Sprite's, again
    void ResolveCollision(void *unused); // ResolvePlatformCollision

    PlatformMover *Mover()
    {
        return (PlatformMover *)mover;
    }
};

COMPILE_TIME_ASSERT(platform_hpp, sizeof(Platform) == 0x80);

/* A platform's controller (CreatePlatform allocates 0x38 bytes): it moves
 * its platform back and forth over `rangeX`/`rangeY` pixels with the
 * motion records of gPlatformMoverMotionRecords, and carries the player
 * while it is `active` (the player stands on the platform). `kind` is the
 * platform's type (5, 6 and 7 add timed behaviour, see Update). The Neo
 * Cortex fight's platforms have a subclass (CortexBossPlatformMover,
 * below). */
class PlatformMover : public Ctrl
{
public:
    s32 kind;   // 0x10 - the platform type
    s32 timer;  // 0x14 - type 5: the frame the player landed; -1 once it fell
    s32 distX;  // 0x18 - pixels travelled since the last turn; -1: turning
    s32 distY;  // 0x1C
    s32 lastX;  // 0x20 - the platform's last position, in pixels
    s32 lastY;  // 0x24
    s32 rangeX; // 0x28 - the pixels to travel before turning; 0: still
    s32 rangeY; // 0x2C
    u8 dirX;    // 0x30 - nonzero: the motion record's sign as it is
    u8 dirY;    // 0x31
    u8 active;  // 0x32 - the player stands on the platform
    u32 time;   // 0x34 - type 6: the frame its animation runs again

    PlatformMover(s32 distX, s32 distY, bool dirX, bool dirY, s32 kind);   // CreatePlatformMover
    virtual void Update(MovingSprite *part);                               // 1 UpdatePlatformMover
    virtual ~PlatformMover();                                              // 9 DestroyPlatformMover
    virtual void StartTargetMotionXFromSet(MovingSprite *part, s32 index); // 11
    virtual void StartTargetMotionYFromSet(MovingSprite *part, s32 index); // 12

    void MovePlayer(MovingSprite *part); // MovePlayerWithPlatform
    void SetTargetMotionYFromSet(MovingSprite *part, s32 index);
    void SetTargetMotionXFromSet(MovingSprite *part, s32 index);
    void ClearActive();
};

COMPILE_TIME_ASSERT(platform_hpp, sizeof(PlatformMover) == 0x38);

/* The Neo Cortex fight's platforms' controller
 * (gCortexBossPlatformMoverVtable; src/bosses/cortex.cpp): a still type-6
 * PlatformMover whose Update runs the platform's animation to frame 0x1A,
 * or to frame 10 while the target has set the platform's `kind` (its
 * state 5, CortexTargetCtrl::SetPlatformsKind). */
/* Its `state` (Ctrl::state; #843, docs/flags/boss_ctrl.md). */
#define CORTEX_BOSS_PLATFORM_STATE_START 0 // the first frame: the animation to frame 0x1A, then RUN
#define CORTEX_BOSS_PLATFORM_STATE_RUN 1   // runs the animation to frame 0x1A (10 while `kind` is set)
class CortexBossPlatformMover : public PlatformMover
{
public:
    CortexBossPlatformMover(); // CreateCortexBossPlatformMover
    virtual void Update(MovingSprite *part);
    virtual ~CortexBossPlatformMover(); // DestroyCortexBossPlatformMover
};

COMPILE_TIME_ASSERT(platform_hpp, sizeof(CortexBossPlatformMover) == 0x38);

#endif /* !GUARD_PLATFORM_HPP */
