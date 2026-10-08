#ifndef GUARD_BOSS_ACTORS_HPP
#define GUARD_BOSS_ACTORS_HPP

/* The 3D bosses' actors as C++ (#664, docs/cplusplus.md): the airship's
 * fireball and the hovercraft's weapons, all HpActors (actor_self.hpp).
 * Part 11a declares their destructors (src/actor/actor_anim.cpp). The
 * airship's fireball is complete since part 11i (src/bosses/airship*.cpp,
 * with the airship itself), its two flight states since part 11f
 * (src/vehicle/jetpack_plane.cpp); the hovercraft's weapons since part
 * 11h (src/bosses/hovercraft*.cpp, with the hovercraft itself).
 * bosses.h's structs are their C views. cxx_symbols.txt maps the C++
 * names to the C ones.
 *
 * No `#pragma interface`: g++ emits the vtables in actor_anim.cpp, where
 * their destructors are (see ctrl.hpp). */

#include "actor_self.hpp"

extern "C" {
#include "bosses.h"
}

/* The airship's fireball (gAirshipFireballVtable,
 * src/bosses/airship_fireball.cpp): it flies around the
 * point it was spawned at (StateOrbit), then spirals in on it
 * (StateSpiralIn); the ROM has those two flight states in
 * src/vehicle/jetpack_plane.cpp. */
class AirshipFireball : public HpActor
{
public:
    s32 centerX;  // 0x58 - the constructor's `x`
    s32 centerY;  // 0x5C - the constructor's `y`
    s32 velZ;     // 0x60 - Z step, decays by 5 down to 0x14
    s32 radius;   // 0x64
    u8 exploding; // 0x68 - set by StateExplode, IsUnshootable returns it

    // CreateAirshipFireball
    AirshipFireball(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~AirshipFireball();      // 1 DestroyAirshipFireball
    virtual void Update();           // 2 UpdateAirshipFireball
    virtual void Damage(s32 amount); // 4 DamageAirshipFireball
    virtual s32 IsUnshootable();     // 5 IsAirshipFireballUnshootable

    void RunState(); // RunAirshipFireballState

    /* The states, indexed by `state` (stateFuncs, gAirshipFireballStateFuncs). */
    void StateOrbit();    // AirshipFireballStateOrbit
    void StateSpiralIn(); // AirshipFireballStateSpiralIn
    void StateExplode();  // AirshipFireballStateExplode

    typedef void (AirshipFireball::*StateFunc)();
    static const StateFunc stateFuncs[3];
};

COMPILE_TIME_ASSERT(boss_actors_hpp, sizeof(AirshipFireball) == 0x6C);
COMPILE_TIME_ASSERT(boss_actors_hpp, sizeof(AirshipFireball) == 0x6C);

/* The airship itself (src/bosses/airship*.cpp) is no class of its own: a
 * bare AnimPart (gAirship, `new AnimPart` in CreateAirship) for its
 * picture's animation, and globals for the rest (gAirshipState,
 * gAirshipX, ...), stepped by UpdateAirship through the plain function
 * table gAirshipStateFuncs. Its functions keep their C names.
 *
 * SetAirshipState: state `st` and animation `idx`, keeping the current
 * frame unless it is past the new animation's end. */
static inline void SetAirshipState(s32 st, s32 idx)
{
    AnimPart *a;

    gAirshipState = st;
    gAirshipStateTimer = 0;
    a = gAirship;
    a->animIndex = idx;
    a->animTimer = a->anims[idx].duration;
    a->animDone = 0;
    if (a->GetAnimFrameBaseOffset() >= a->anims[a->animIndex].loopThreshold)
        a->animTime = 0;
}

/* The hovercraft's weapons (#664 part 11h, src/bosses/hovercraft*.cpp).
 * The hovercraft itself is no class of its own, like the airship: a bare
 * AnimPart (gHovercraft) and globals, stepped through the plain function
 * table gHovercraftStateFuncs; its functions keep their C names. Its
 * weapons follow it around (GetHovercraftX/Y/Z plus an offset) and count
 * down its parts (LoseHovercraftPart) as they are shot down. */

/* EnterHovercraftState: state `st` and animation `idx`, keeping the
 * current frame unless it is past the new animation's end
 * (SetAirshipState without the timer). SetHovercraftState
 * (hovercraft_parts.cpp) is its out-of-line copy. */
static inline void EnterHovercraftState(s32 st, s32 idx)
{
    AnimPart *a;

    gHovercraftState = st;
    a = gHovercraft;
    a->animIndex = idx;
    a->animTimer = a->anims[idx].duration;
    a->animDone = 0;
    if (a->GetAnimFrameBaseOffset() >= a->anims[a->animIndex].loopThreshold)
        a->animTime = 0;
}

/* The hovercraft's fireball (gHovercraftFireballVtable, hovercraft.cpp;
 * the airship fireball's layout): the side guns fire it (SpawnHovercraftFireball). It sets the
 * orbit fields up as the airship's does, but never reads them: it flies
 * straight on at `velZ`. */
class HovercraftFireball : public HpActor
{
public:
    s32 centerX;  // 0x58 - the constructor's `x`, never read
    s32 centerY;  // 0x5C - the constructor's `y`, never read
    s32 velZ;     // 0x60 - Z step, decays by 5 down to 0x14
    s32 radius;   // 0x64 - never read
    u8 exploding; // 0x68 - set by StateExplode, IsUnshootable returns it

    // CreateHovercraftFireball
    HovercraftFireball(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~HovercraftFireball();   // 1 DestroyHovercraftFireball
    virtual void Update();           // 2 UpdateHovercraftFireball
    virtual void Damage(s32 amount); // 4 DamageHovercraftFireball
    virtual s32 IsUnshootable();     // 5 IsHovercraftFireballUnshootable

    void RunState(); // RunHovercraftFireballState

    /* The states, indexed by `state` (stateFuncs, gHovercraftFireballStateFuncs). */
    void StateFly();     // HovercraftFireballStateFly
    void StateExplode(); // HovercraftFireballStateExplode

    typedef void (HovercraftFireball::*StateFunc)();
    static const StateFunc stateFuncs[2];
};

COMPILE_TIME_ASSERT(boss_actors_hpp, sizeof(HovercraftFireball) == 0x6C);

/* The cannon (gHovercraftCannonVtable, hovercraft_cannon.cpp): it waits until the hovercraft is close
 * enough (StateWait), then fires cannonballs at the player in bursts
 * (StateFire), and is destroyed when shot down (StateDestroyed). */
class HovercraftCannon : public HpActor
{
public:
    s32 spawnX; // 0x58 - the constructor's `x`, never read
    s32 spawnY; // 0x5C - the constructor's `y`, never read
    u8 unk_60[4];
    s32 cooldown; // 0x64 - frames until the next shot
    s32 count;    // 0x68 - shots in this burst
    u8 dead;      // 0x6C - set when shot down, IsUnshootable returns it

    // CreateHovercraftCannon
    HovercraftCannon(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~HovercraftCannon();     // 1 DestroyHovercraftCannon
    virtual void Update();           // 2 UpdateHovercraftCannon
    virtual void Damage(s32 amount); // 4 DamageHovercraftCannon
    virtual s32 IsUnshootable();     // 5 IsHovercraftCannonUnshootable

    s32 RunState(); // RunHovercraftCannonState

    /* The states, indexed by `state` (stateFuncs, gHovercraftCannonStateFuncs). */
    void StateWait();      // HovercraftCannonStateWait
    void StateFire();      // HovercraftCannonStateFire
    void StateDestroyed(); // HovercraftCannonStateDestroyed

    typedef void (HovercraftCannon::*StateFunc)();
    static const StateFunc stateFuncs[3];
};

COMPILE_TIME_ASSERT(boss_actors_hpp, sizeof(HovercraftCannon) == 0x70);
COMPILE_TIME_ASSERT(boss_actors_hpp, sizeof(HovercraftCannon) == 0x70);

/* The launcher (gHovercraftLauncherVtable, hovercraft_launcher.cpp; the
 * same layout as the cannon): it launches planes,
 * bombers and balloons (CreateJetpackActor kinds 5, 6 and 8) once the
 * hovercraft has lost two parts. */
class HovercraftLauncher : public HpActor
{
public:
    s32 spawnX; // 0x58 - the constructor's `x`, never read
    s32 spawnY; // 0x5C - the constructor's `y`, never read
    u8 unk_60[4];
    s32 cooldown; // 0x64 - frames until the next launch
    s32 count;    // 0x68 - launches in this burst
    u8 dead;      // 0x6C - set when shot down, IsUnshootable returns it

    // CreateHovercraftLauncher
    HovercraftLauncher(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~HovercraftLauncher();   // 1 DestroyHovercraftLauncher
    virtual void Update();           // 2 UpdateHovercraftLauncher
    virtual void Damage(s32 amount); // 4 DamageHovercraftLauncher
    virtual s32 IsUnshootable();     // 5 IsHovercraftLauncherUnshootable

    s32 RunState(); // RunHovercraftLauncherState

    /* The states, indexed by `state` (stateFuncs, gHovercraftLauncherStateFuncs). */
    void StateWait();      // HovercraftLauncherStateWait
    void StateLaunch();    // HovercraftLauncherStateLaunch
    void StateDestroyed(); // HovercraftLauncherStateDestroyed

    typedef void (HovercraftLauncher::*StateFunc)();
    static const StateFunc stateFuncs[3];
};

COMPILE_TIME_ASSERT(boss_actors_hpp, sizeof(HovercraftLauncher) == 0x70);
COMPILE_TIME_ASSERT(boss_actors_hpp, sizeof(HovercraftLauncher) == 0x70);

/* A side gun (gHovercraftSideGunVtable, hovercraft_side_gun.cpp): one on
 * each side of the hovercraft (`left`), firing fireballs in bursts. */
class HovercraftSideGun : public HpActor
{
public:
    u8 dead;        // 0x58 - set when shot down, IsUnshootable returns it
    u8 left;        // 0x59 - the constructor's `left`: the side and the animation
    s32 offX;       // 0x5C - added to the hovercraft's position
    s32 offY;       // 0x60
    s32 offZ;       // 0x64
    s32 orbitTimer; // 0x68 - frames until the next fireball
    s32 lap;        // 0x6C - fireballs in this burst

    // CreateHovercraftSideGun
    HovercraftSideGun(const struct anim_table_record *rec, s32 x, s32 y, s32 z, bool left);
    virtual ~HovercraftSideGun();    // 1 DestroyHovercraftSideGun
    virtual void Update();           // 2 UpdateHovercraftSideGun
    virtual void Damage(s32 amount); // 4 DamageHovercraftSideGun
    virtual s32 IsUnshootable();     // 5 IsHovercraftSideGunUnshootable

    void RunState(); // RunHovercraftSideGunState (UNUSED)
};

COMPILE_TIME_ASSERT(boss_actors_hpp, sizeof(HovercraftSideGun) == 0x70);

/* The cannon's muzzle flash (gHovercraftCannonFlashVtable,
 * hovercraft_cannon_flash.cpp): it plays its animation once in front of the cannon, then deletes
 * itself. */
class HovercraftCannonFlash : public HpActor
{
public:
    u8 unshootable; // 0x58 - always set

    // CreateHovercraftCannonFlash
    HovercraftCannonFlash(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~HovercraftCannonFlash(); // 1 DestroyHovercraftCannonFlash
    virtual void Update();            // 2 UpdateHovercraftCannonFlash
    virtual void Damage(s32 amount);  // 4 DamageHovercraftCannonFlash (none)
    virtual s32 IsUnshootable();      // 5 IsHovercraftCannonFlashUnshootable

    s32 RunState(); // RunHovercraftCannonFlashState (UNUSED)
};

COMPILE_TIME_ASSERT(boss_actors_hpp, sizeof(HovercraftCannonFlash) == 0x5C);
COMPILE_TIME_ASSERT(boss_actors_hpp, sizeof(HovercraftCannonFlash) == 0x5C);

#endif /* GUARD_BOSS_ACTORS_HPP */
