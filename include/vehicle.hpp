#ifndef GUARD_VEHICLE_HPP
#define GUARD_VEHICLE_HPP

/* The vehicle levels' 3D actors as C++ (#664, docs/cplusplus.md), on
 * ActorSelf and HpActor (actor_self.hpp). Part 11a declares what the
 * C++ objects so far need: the destructors that src/actor/actor_anim.cpp
 * has (the ROM keeps them there, with a few small methods), and the polar
 * player's state dispatch and table (src/vehicle/polar_player_dispatch.cpp,
 * src/data/actor_pmf_17a6b8.cpp). Their other methods, and their fields
 * past ActorSelf's or HpActor's, are still C (vehicle.h, src/vehicle/);
 * cxx_symbols.txt maps the C++ names to the C ones. The constructors are
 * declared with ActorSelf's arguments and the extra ones of their C
 * prototypes (Create*, Init*, named in a comment); where the ROM only has
 * a constructor inlined, its signature is a placeholder until its code is
 * converted.
 *
 * `#pragma interface`: no vtable is emitted (see ctrl.hpp). */
#pragma interface

#include "actor_self.hpp"

extern "C" {
#include "vehicle.h"
}

/* The polar bear levels (4 vtable slots: ActorSelf's). */

class RiderlessPolar : public ActorSelf
{
public:
    // constructor inlined where it is used
    RiderlessPolar(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~RiderlessPolar(); // 1 DestroyRiderlessPolar
};

/* The polar run's player (gPolarPlayerVtable). */
class PolarPlayer : public ActorSelf
{
public:
    PolarPlayer(const struct anim_table_record *rec, s32 z); // ConstructActorPart
    void RunState();                                         // RunPolarPlayerState

    /* The states, indexed by `state` (stateFuncs, gPolarPlayerStateFuncs). */
    void StateMount();      // PolarPlayerStateMount
    void StateRun();        // PolarPlayerStateRun
    void StateDash();       // PolarPlayerStateDash
    void StateBoost();      // PolarPlayerStateBoost
    void StateJump();       // PolarPlayerStateJump
    void StateLaunched();   // PolarPlayerStateLaunched
    void StateKnockedOff(); // PolarPlayerStateKnockedOff
    void StateCaught();     // PolarPlayerStateCaught
    void StateCarriedOff(); // PolarPlayerStateCarriedOff
    void StateLand();       // PolarPlayerStateLand
    void StateFinish();     // PolarPlayerStateFinish
    void StateFinishLeap(); // PolarPlayerStateFinishLeap
    void StateShocked();    // PolarPlayerStateShocked
    void StateRecover();    // PolarPlayerStateRecover

    typedef void (PolarPlayer::*StateFunc)();
    static const StateFunc stateFuncs[14];
};

class PolarCheckpointText : public ActorSelf
{
public:
    // constructor inlined where it is used
    PolarCheckpointText(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarCheckpointText(); // 1 DestroyPolarCheckpointText
    virtual void Update();          // 2 UpdatePolarCheckpointText
};

class PolarWumpa : public ActorSelf
{
public:
    // CreatePolarWumpa
    PolarWumpa(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarWumpa(); // 1 DestroyPolarWumpa
};

class PolarTimeCrate : public ActorSelf
{
public:
    // CreatePolarTimeCrate
    PolarTimeCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarTimeCrate(); // 1 DestroyPolarTimeCrate
};

class PolarQuestionCrate : public ActorSelf
{
public:
    // CreatePolarQuestionCrate
    PolarQuestionCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarQuestionCrate(); // 1 DestroyPolarQuestionCrate
};

class PolarAkuAkuCrate : public ActorSelf
{
public:
    // CreatePolarAkuAkuCrate
    PolarAkuAkuCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarAkuAkuCrate(); // 1 DestroyPolarAkuAkuCrate
};

class PolarNitroCrate : public ActorSelf
{
public:
    // CreatePolarNitroCrate
    PolarNitroCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarNitroCrate(); // 1 DestroyPolarNitroCrate
};

class PolarLifeCrate : public ActorSelf
{
public:
    // CreatePolarLifeCrate
    PolarLifeCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z, void *spawn);
    virtual ~PolarLifeCrate(); // 1 DestroyPolarLifeCrate
};

class PolarFourWumpaCrate : public ActorSelf
{
public:
    // CreatePolarFourWumpaCrate
    PolarFourWumpaCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarFourWumpaCrate(); // 1 DestroyPolarFourWumpaCrate
};

class PolarBasicCrate : public ActorSelf
{
public:
    // CreatePolarBasicCrate
    PolarBasicCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarBasicCrate(); // 1 DestroyPolarBasicCrate
};

class PolarCrate : public ActorSelf
{
public:
    // InitPolarCrate
    PolarCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarCrate(); // 1 DestroyPolarCrate
};

class PolarElectricFence : public ActorSelf
{
public:
    // CreatePolarElectricFence
    PolarElectricFence(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarElectricFence(); // 1 DestroyPolarElectricFence
};

class PolarObstacle : public ActorSelf
{
public:
    // CreatePolarObstacle
    PolarObstacle(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarObstacle(); // 1 DestroyPolarObstacle
};

class PolarLauncher : public ActorSelf
{
public:
    // CreatePolarLauncher
    PolarLauncher(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarLauncher(); // 1 DestroyPolarLauncher
};

class PolarPenguin : public ActorSelf
{
public:
    // CreatePolarPenguin
    PolarPenguin(const struct anim_table_record *rec, s32 x, s32 y, s32 z, struct spawn_arg *arg);
    virtual ~PolarPenguin(); // 1 DestroyPolarPenguin
};

class PolarIcicle : public ActorSelf
{
public:
    // CreatePolarIcicle
    PolarIcicle(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarIcicle(); // 1 DestroyPolarIcicle
};

class PolarAkuAku : public ActorSelf
{
public:
    // CreatePolarAkuAku
    PolarAkuAku(const struct anim_table_record *rec, s32 x, s32 y, s32 z, s32 arg);
    virtual ~PolarAkuAku(); // 1 DestroyPolarAkuAku
};

class PolarGoal : public ActorSelf
{
public:
    // CreatePolarGoal
    PolarGoal(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarGoal(); // 1 DestroyPolarGoal
};

class PolarBoostPad : public ActorSelf
{
public:
    // CreatePolarBoostPad
    PolarBoostPad(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarBoostPad(); // 1 DestroyPolarBoostPad
};

class PolarCheckpointCrate : public ActorSelf
{
public:
    // CreatePolarCheckpointCrate
    PolarCheckpointCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarCheckpointCrate(); // 1 DestroyPolarCheckpointCrate
};

/* The jetpack levels (7 vtable slots: HpActor's; the balloon crates
 * add an 8th). */

/* The checkpoint banner, drawn at a fixed screen position. */
class JetpackCheckpointText : public HpActor
{
public:
    // constructor inlined where it is used
    JetpackCheckpointText(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~JetpackCheckpointText(); // 1 DestroyJetpackCheckpointText
    virtual void Update();            // 2 UpdateJetpackCheckpointText
    virtual void Draw();              // 3 DrawJetpackCheckpointText
    virtual s32 IsUnshootable();      // 5 IsJetpackCheckpointTextUnshootable
};

class JetpackExplosion : public HpActor
{
public:
    // constructor inlined where it is used
    JetpackExplosion(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~JetpackExplosion(); // 1 DestroyJetpackExplosion
    virtual void Update();       // 2 UpdateJetpackExplosion
    virtual s32 IsUnshootable(); // 5 IsJetpackExplosionUnshootable
};

class JetpackShot : public HpActor
{
public:
    // CreateJetpackShot
    JetpackShot(const struct anim_table_record *rec, s32 x, s32 y, s32 z, s32 velX, s32 velY);
    virtual ~JetpackShot(); // 1 DestroyJetpackShot
};

class JetpackPlane : public HpActor
{
public:
    // CreateJetpackPlane
    JetpackPlane(const struct anim_table_record *rec, s32 x, s32 y, s32 z, struct spawn_arg *arg);
    virtual ~JetpackPlane(); // 1 DestroyJetpackPlane
};

class JetpackBomber : public HpActor
{
public:
    // CreateJetpackBomber
    JetpackBomber(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~JetpackBomber(); // 1 DestroyJetpackBomber
};

class JetpackCannonball : public HpActor
{
public:
    // CreateJetpackCannonball
    JetpackCannonball(const struct anim_table_record *rec, s32 x, s32 y, s32 z, s32 velX, s32 velY);
    virtual ~JetpackCannonball(); // 1 DestroyJetpackCannonball
};

class JetpackBalloon : public HpActor
{
public:
    // CreateJetpackBalloon
    JetpackBalloon(const struct anim_table_record *rec, s32 x, s32 y, s32 z, s32 arg);
    virtual ~JetpackBalloon(); // 1 DestroyJetpackBalloon
};

/* The crates hanging from balloons (gJetpackBalloonCrateVtable), and the
 * three kinds built on them, whose destructors are g++'s implicit ones
 * (DestroyJetpackHealthCrate, DestroyJetpackTimeCrate and
 * DestroyJetpackQuestionCrate, actor_anim.cpp). */
class JetpackBalloonCrate : public HpActor
{
public:
    // InitJetpackBalloonCrate
    JetpackBalloonCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z, u8 kind);
    virtual ~JetpackBalloonCrate(); // 1 DestroyJetpackBalloonCrate (jetpack_crates.c)
};

class JetpackHealthCrate : public JetpackBalloonCrate
{
public:
    // CreateJetpackHealthCrate
    JetpackHealthCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
};

class JetpackTimeCrate : public JetpackBalloonCrate
{
public:
    // CreateJetpackTimeCrate
    JetpackTimeCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
};

class JetpackQuestionCrate : public JetpackBalloonCrate
{
public:
    // CreateJetpackQuestionCrate
    JetpackQuestionCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z, s32 arg);
};

class JetpackParachuteNitro : public HpActor
{
public:
    // CreateJetpackParachuteNitro
    JetpackParachuteNitro(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~JetpackParachuteNitro(); // 1 DestroyJetpackParachuteNitro
};

class JetpackRocket : public HpActor
{
public:
    // CreateJetpackRocket
    JetpackRocket(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~JetpackRocket(); // 1 DestroyJetpackRocket
};

class JetpackRing : public HpActor
{
public:
    // CreateJetpackRing
    JetpackRing(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~JetpackRing(); // 1 DestroyJetpackRing
};

#endif /* GUARD_VEHICLE_HPP */
