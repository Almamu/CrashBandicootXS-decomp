#ifndef GUARD_BOSS_ACTORS_HPP
#define GUARD_BOSS_ACTORS_HPP

/* The 3D bosses' actors as C++ (#664, docs/cplusplus.md): the airship's
 * fireball and the hovercraft's weapons, all HpActors (actor_self.hpp).
 * Part 11a declares their destructors (src/actor/actor_anim.cpp). The
 * airship's fireball is complete since part 11i (src/bosses/airship*.cpp,
 * with the airship itself), its two flight states since part 11f
 * (src/vehicle/jetpack_plane.cpp); the hovercraft's weapons' other methods and fields
 * are still C (bosses.h, src/bosses/hovercraft*.c). cxx_symbols.txt maps
 * the C++ names to the C ones.
 *
 * `#pragma interface`: no vtable is emitted (see ctrl.hpp). */
#pragma interface

#include "actor_self.hpp"

extern "C" {
#include "bosses.h"
}

/* The airship's fireball (gAirshipFireballVtable, src/bosses/airship_fireball.cpp;
 * bosses.h's `struct actor_orbit` is its C view): it flies around the
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
COMPILE_TIME_ASSERT(boss_actors_hpp, sizeof(AirshipFireball) == sizeof(struct actor_orbit));

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

class HovercraftFireball : public HpActor
{
public:
    // CreateHovercraftFireball
    HovercraftFireball(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~HovercraftFireball(); // 1 DestroyHovercraftFireball
};

class HovercraftCannon : public HpActor
{
public:
    // CreateHovercraftCannon
    HovercraftCannon(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~HovercraftCannon(); // 1 DestroyHovercraftCannon
};

class HovercraftLauncher : public HpActor
{
public:
    // CreateHovercraftLauncher
    HovercraftLauncher(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~HovercraftLauncher(); // 1 DestroyHovercraftLauncher
};

class HovercraftSideGun : public HpActor
{
public:
    // CreateHovercraftSideGun
    HovercraftSideGun(const struct anim_table_record *rec, s32 x, s32 y, s32 z, u8 arg);
    virtual ~HovercraftSideGun(); // 1 DestroyHovercraftSideGun
};

class HovercraftCannonFlash : public HpActor
{
public:
    // CreateHovercraftCannonFlash
    HovercraftCannonFlash(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~HovercraftCannonFlash(); // 1 DestroyHovercraftCannonFlash
};

#endif /* GUARD_BOSS_ACTORS_HPP */
