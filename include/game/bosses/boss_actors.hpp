#ifndef GUARD_BOSS_ACTORS_HPP
#define GUARD_BOSS_ACTORS_HPP

/* The 3D bosses' actors as C++ (#664, docs/cplusplus.md): the airship's
 * fireball and the hovercraft's weapons, all HpActors (actor_self.hpp).
 * Part 11a declares their destructors (src/game/vehicles/actor_destructors.cpp). The
 * airship's fireball is complete since part 11i (src/vehicle/jetpack/airship*.cpp,
 * with the airship itself), its two flight states since part 11f
 * (then in jetpack_plane.cpp, at the start of airship_fireball.cpp since
 * #769); the hovercraft's weapons since part
 * 11h (src/game/bosses/hovercraft*.cpp, with the hovercraft itself).
 * They have no C views. cxx_symbols.txt maps the C++ names to the C
 * ones.
 *
 * No `#pragma interface`: g++ emits the vtables in actor_destructors.cpp, where
 * their destructors are (see ctrl.hpp).
 *
 * The airship's fireball is in the namespace Game::Vehicles::Jetpack, with
 * the airship (#1033, vehicle.hpp):
 * `_vt.Q44Game8Vehicles7Jetpack15AirshipFireball`. The hovercraft's
 * weapons are in the namespace Game::Bosses (#1033, boss_ctrl.hpp), with
 * the hovercraft: `_vt.Q34Game6Bosses16HovercraftCannon` & co. */

#include "game/entities/actor_self.hpp"

extern "C" {
#include "game/bosses/bosses.h"
}

namespace Game
{
namespace Vehicles
{
namespace Jetpack
{

/* The airship's fireball (gAirshipFireballVtable,
 * src/game/vehicles/jetpack/airship_fireball.cpp): it flies around the
 * point it was spawned at (StateOrbit), then spirals in on it
 * (StateSpiralIn); the ROM has those two flight states first, right
 * after jetpack_cannonball.cpp. */
/* AirshipFireball's `state` (#843, docs/flags/actor_states_jetpack.md): the
 * stateFuncs index. */
#define AIRSHIP_FIREBALL_STATE_ORBIT 0     // StateOrbit
#define AIRSHIP_FIREBALL_STATE_SPIRAL_IN 1 // StateSpiralIn
#define AIRSHIP_FIREBALL_STATE_EXPLODE 2   // StateExplode: gone when the animation is done

/* The fireball's flight (Q8, docs/fixed_point.md): it starts at
 * AIRSHIP_FIREBALL_LAUNCH_SPEED_Z (the constructor), and its Z speed
 * drops by AIRSHIP_FIREBALL_DRAG_Z a frame down to
 * AIRSHIP_FIREBALL_MIN_SPEED_Z. Its orbit's radius grows by
 * AIRSHIP_FIREBALL_RADIUS_STEP a frame up to AIRSHIP_FIREBALL_ORBIT_RADIUS
 * (StateOrbit), then shrinks by it to 0 (StateSpiralIn); its centre
 * eases toward the player by a 32nd of the gap a frame while it orbits
 * (`gap >> AIRSHIP_FIREBALL_ORBIT_EASE_SHIFT`), a 16th while it spirals
 * in (AIRSHIP_FIREBALL_SPIRAL_EASE_SHIFT): fractions, not formats. */
#define AIRSHIP_FIREBALL_LAUNCH_SPEED_Z    Q8_FRAC(0, 0x95)
#define AIRSHIP_FIREBALL_DRAG_Z            Q8_FRAC(0, 5)
#define AIRSHIP_FIREBALL_MIN_SPEED_Z       Q8_FRAC(0, 0x14)
#define AIRSHIP_FIREBALL_RADIUS_STEP       INT_TO_Q8(1)
#define AIRSHIP_FIREBALL_ORBIT_RADIUS      INT_TO_Q8(42)
#define AIRSHIP_FIREBALL_ORBIT_EASE_SHIFT  5
#define AIRSHIP_FIREBALL_SPIRAL_EASE_SHIFT 4
class AirshipFireball : public Game::Entities::HpActor
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
    virtual u8 IsUnshootable();      // 5 IsAirshipFireballUnshootable

    void RunState(); // RunAirshipFireballState

    /* The states, indexed by `state` (stateFuncs, gAirshipFireballStateFuncs). */
    void StateOrbit();    // AirshipFireballStateOrbit
    void StateSpiralIn(); // AirshipFireballStateSpiralIn
    void StateExplode();  // AirshipFireballStateExplode

    typedef void (AirshipFireball::*StateFunc)();
    static const StateFunc stateFuncs[3];

    static void Spawn(s32 x, s32 y, s32 z); // SpawnAirshipFireball
};

COMPILE_TIME_ASSERT(boss_actors_hpp, sizeof(AirshipFireball) == 0x6C);
COMPILE_TIME_ASSERT(boss_actors_hpp, sizeof(AirshipFireball) == 0x6C);

} // namespace Jetpack
} // namespace Vehicles
} // namespace Game

/* The airship itself is the all-static class Airship (airship.hpp, #772). */

namespace Game
{
namespace Bosses
{

/* The hovercraft's weapons (#664 part 11h, src/game/bosses/hovercraft*.cpp).
 * The hovercraft itself is the all-static class Hovercraft
 * (include/game/bosses/hovercraft.hpp, #772). Its weapons follow it around
 * (Hovercraft::GetX/GetY/GetZ plus an offset) and count down its parts
 * (Hovercraft::LosePart) as they are shot down. */

/* The hovercraft's fireball (gHovercraftFireballVtable, hovercraft_fireball.cpp;
 * the airship fireball's layout): the side guns fire it (HovercraftFireball::Spawn). It sets the
 * orbit fields up as the airship's does, but never reads them: it flies
 * straight on at `velZ`. */
/* HovercraftFireball's `state`: the stateFuncs index. */
#define HOVERCRAFT_FIREBALL_STATE_FLY 0     // StateFly
#define HOVERCRAFT_FIREBALL_STATE_EXPLODE 1 // StateExplode: gone when the animation is done

/* Its Z speed (Q8): the airship fireball's, from
 * HOVERCRAFT_FIREBALL_LAUNCH_SPEED_Z down by HOVERCRAFT_FIREBALL_DRAG_Z a
 * frame to HOVERCRAFT_FIREBALL_MIN_SPEED_Z (StateFly). */
#define HOVERCRAFT_FIREBALL_LAUNCH_SPEED_Z Q8_FRAC(0, 0x95)
#define HOVERCRAFT_FIREBALL_DRAG_Z         Q8_FRAC(0, 5)
#define HOVERCRAFT_FIREBALL_MIN_SPEED_Z    Q8_FRAC(0, 0x14)
class HovercraftFireball : public Game::Entities::HpActor
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
    virtual u8 IsUnshootable();      // 5 IsHovercraftFireballUnshootable

    void RunState(); // RunHovercraftFireballState

    /* The states, indexed by `state` (stateFuncs, gHovercraftFireballStateFuncs). */
    void StateFly();     // HovercraftFireballStateFly
    void StateExplode(); // HovercraftFireballStateExplode

    typedef void (HovercraftFireball::*StateFunc)();
    static const StateFunc stateFuncs[2];

    static void Spawn(s32 x, s32 y, s32 z); // SpawnHovercraftFireball
};

COMPILE_TIME_ASSERT(boss_actors_hpp, sizeof(HovercraftFireball) == 0x6C);

/* The cannon (gHovercraftCannonVtable, hovercraft_cannon.cpp): it waits until the hovercraft is close
 * enough (StateWait), then fires cannonballs at the player in bursts
 * (StateFire), and is destroyed when shot down (StateDestroyed). */
/* HovercraftCannon's `state`: the stateFuncs index. */
#define HOVERCRAFT_CANNON_STATE_WAIT 0      // StateWait
#define HOVERCRAFT_CANNON_STATE_FIRE 1      // StateFire
#define HOVERCRAFT_CANNON_STATE_DESTROYED 2 // StateDestroyed
class HovercraftCannon : public Game::Entities::HpActor
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
    virtual u8 IsUnshootable();      // 5 IsHovercraftCannonUnshootable

    s32 RunState(); // RunHovercraftCannonState

    /* The states, indexed by `state` (stateFuncs, gHovercraftCannonStateFuncs). */
    void StateWait();      // HovercraftCannonStateWait
    void StateFire();      // HovercraftCannonStateFire
    void StateDestroyed(); // HovercraftCannonStateDestroyed

    typedef void (HovercraftCannon::*StateFunc)();
    static const StateFunc stateFuncs[3];

    static void Spawn(s32 x, s32 y, s32 z); // SpawnHovercraftCannon
};

COMPILE_TIME_ASSERT(boss_actors_hpp, sizeof(HovercraftCannon) == 0x70);
COMPILE_TIME_ASSERT(boss_actors_hpp, sizeof(HovercraftCannon) == 0x70);

/* The launcher (gHovercraftLauncherVtable, hovercraft_launcher.cpp; the
 * same layout as the cannon): it launches planes,
 * bombers and balloons (JetpackPlayer::CreateActor kinds 5, 6 and 8) once the
 * hovercraft has lost two parts. */
/* HovercraftLauncher's `state`: the stateFuncs index. */
#define HOVERCRAFT_LAUNCHER_STATE_WAIT 0      // StateWait
#define HOVERCRAFT_LAUNCHER_STATE_LAUNCH 1    // StateLaunch; it can be hit only now
#define HOVERCRAFT_LAUNCHER_STATE_DESTROYED 2 // StateDestroyed
class HovercraftLauncher : public Game::Entities::HpActor
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
    virtual u8 IsUnshootable();      // 5 IsHovercraftLauncherUnshootable

    s32 RunState(); // RunHovercraftLauncherState

    /* The states, indexed by `state` (stateFuncs, gHovercraftLauncherStateFuncs). */
    void StateWait();      // HovercraftLauncherStateWait
    void StateLaunch();    // HovercraftLauncherStateLaunch
    void StateDestroyed(); // HovercraftLauncherStateDestroyed

    typedef void (HovercraftLauncher::*StateFunc)();
    static const StateFunc stateFuncs[3];

    static void Spawn(s32 x, s32 y, s32 z); // SpawnHovercraftLauncher
};

COMPILE_TIME_ASSERT(boss_actors_hpp, sizeof(HovercraftLauncher) == 0x70);
COMPILE_TIME_ASSERT(boss_actors_hpp, sizeof(HovercraftLauncher) == 0x70);

/* A side gun (gHovercraftSideGunVtable, hovercraft_side_gun.cpp): one on
 * each side of the hovercraft (`left`), firing fireballs in bursts. */
/* HovercraftSideGun's `state` (it has no state methods). */
#define HOVERCRAFT_SIDE_GUN_STATE_ALIVE 0     // follows the hovercraft and fires
#define HOVERCRAFT_SIDE_GUN_STATE_DESTROYED 1 // shot down (Damage)
class HovercraftSideGun : public Game::Entities::HpActor
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
    virtual u8 IsUnshootable();      // 5 IsHovercraftSideGunUnshootable

    void RunState(); // RunHovercraftSideGunState (UNUSED)

    static void Spawn(s32 x, s32 y, s32 z, bool left); // SpawnHovercraftSideGun
};

COMPILE_TIME_ASSERT(boss_actors_hpp, sizeof(HovercraftSideGun) == 0x70);

/* The cannon's muzzle flash (gHovercraftCannonFlashVtable,
 * hovercraft_cannon_flash.cpp): it plays its animation once in front of the cannon, then deletes
 * itself. */
class HovercraftCannonFlash : public Game::Entities::HpActor
{
public:
    u8 unshootable; // 0x58 - always set

    // CreateHovercraftCannonFlash
    HovercraftCannonFlash(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~HovercraftCannonFlash(); // 1 DestroyHovercraftCannonFlash
    virtual void Update();            // 2 UpdateHovercraftCannonFlash
    virtual void Damage(s32 amount);  // 4 DamageHovercraftCannonFlash (none)
    virtual u8 IsUnshootable();       // 5 IsHovercraftCannonFlashUnshootable

    s32 RunState(); // RunHovercraftCannonFlashState (UNUSED)

    static void Spawn(s32 x, s32 y, s32 z); // SpawnHovercraftCannonFlash
};

COMPILE_TIME_ASSERT(boss_actors_hpp, sizeof(HovercraftCannonFlash) == 0x5C);
COMPILE_TIME_ASSERT(boss_actors_hpp, sizeof(HovercraftCannonFlash) == 0x5C);

} // namespace Bosses
} // namespace Game

#endif /* GUARD_BOSS_ACTORS_HPP */
