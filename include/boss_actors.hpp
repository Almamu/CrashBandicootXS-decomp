#ifndef GUARD_BOSS_ACTORS_HPP
#define GUARD_BOSS_ACTORS_HPP

/* The 3D bosses' actors as C++ (#664, docs/cplusplus.md): the airship's
 * fireball and the hovercraft's weapons, all HpActors (actor_self.hpp).
 * Part 11a declares their destructors (src/actor/actor_anim.cpp); their
 * other methods and fields are still C (bosses.h, src/bosses/airship*.c,
 * hovercraft*.c); cxx_symbols.txt maps the C++ names to the C ones.
 *
 * `#pragma interface`: no vtable is emitted (see ctrl.hpp). */
#pragma interface

#include "actor_self.hpp"

class AirshipFireball : public HpActor
{
public:
    // CreateAirshipFireball
    AirshipFireball(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~AirshipFireball(); // 1 DestroyAirshipFireball
};

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
