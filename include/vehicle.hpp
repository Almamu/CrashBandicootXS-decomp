#ifndef GUARD_VEHICLE_HPP
#define GUARD_VEHICLE_HPP

/* The vehicle levels' 3D actors as C++ (#664, docs/cplusplus.md), on
 * ActorSelf and HpActor (actor_self.hpp). Part 11a declared what the
 * C++ objects needed then: the destructors that src/actor/actor_anim.cpp
 * has (the ROM keeps them there, with a few small methods), and the polar
 * player's state dispatch and table (src/vehicle/polar_player_dispatch.cpp,
 * src/data/actor_pmf_17a6b8.cpp). Part 11b added the polar actors' fields
 * and the constructors src/actor/actor_factory.cpp uses. Their other
 * methods, and the jetpack actors' fields past HpActor's, are still C
 * (vehicle.h, src/vehicle/); cxx_symbols.txt maps the C++ names to the C
 * ones. The constructors are declared with ActorSelf's arguments and the
 * extra ones of their C prototypes (Create*, Init*, named in a comment);
 * where the ROM only has a constructor inlined and its code is still C,
 * its signature is a placeholder until it is converted. Part 11e gives
 * the jetpack player (JetpackPlayer), its shot and the checkpoint banner's
 * and explosion's constructors their code.
 *
 * `#pragma interface`: no vtable is emitted (see ctrl.hpp). */
#pragma interface

#include "actor_self.hpp"

extern "C" {
#include "vehicle.h"
}

/* The polar bear levels (4 vtable slots: ActorSelf's). Part 11b gives
 * them their fields and the constructors CreateActor and the other
 * factories in src/actor/actor_factory.cpp use: inline ones where the ROM
 * expands them there (the crates' and the wumpa's also have an out-of-line
 * C copy, Create*, in src/vehicle/), and declarations of the out-of-line
 * ones, still C. */

class RiderlessPolar : public ActorSelf
{
public:
    RiderlessPolar(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
        : ActorSelf(rec, x, y, z)
    {
    }
    virtual ~RiderlessPolar(); // 1 DestroyRiderlessPolar
};

/* The polar run's player (gPolarPlayerVtable). */
class PolarPlayer : public ActorSelf
{
public:
    PolarPlayer(const struct anim_table_record *rec, s32 z); // ConstructActorPart
    void RunState();                                         // RunPolarPlayerState
    void AllocTiles();                                       // AllocPolarPlayerTiles

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
    PolarCheckpointText(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
        : ActorSelf(rec, x, y, z)
    {
    }
    virtual ~PolarCheckpointText(); // 1 DestroyPolarCheckpointText
    virtual void Update();          // 2 UpdatePolarCheckpointText
};

/* The wumpa fruit flying to the HUD (gPolarCollectedWumpaVtable). */
class PolarCollectedWumpa : public ActorSelf
{
public:
    s32 velX;  // 0x54
    s32 velY;  // 0x58
    s32 count; // 0x5C - how many fruit the destructor hands out

    PolarCollectedWumpa(const struct anim_table_record *rec, s32 x, s32 y,
                        s32 count); // CreatePolarCollectedWumpa
    virtual ~PolarCollectedWumpa(); // 1 DestroyPolarCollectedWumpa
    virtual void Update();          // 2 UpdatePolarCollectedWumpa
    virtual void Draw();            // 3 DrawPolarCollectedWumpa
};

COMPILE_TIME_ASSERT(vehicle_hpp, sizeof(PolarCollectedWumpa) == 0x60);

class PolarWumpa : public ActorSelf
{
public:
    // inline here, and CreatePolarWumpa out of line
    PolarWumpa(const struct anim_table_record *rec, s32 x, s32 y, s32 z) : ActorSelf(rec, x, y, z)
    {
    }
    virtual ~PolarWumpa(); // 1 DestroyPolarWumpa
};

/* The crates' base (gPolarCrateVtable). Its destructor is inline, as
 * every crate kind's expands it; actor_anim.cpp has the out-of-line copy
 * (DestroyPolarCrate). The kinds' constructors are inline, and each also
 * has an out-of-line C copy (CreatePolarTimeCrate, ..., polar_crates.c). */
class PolarCrate : public ActorSelf
{
public:
    PolarCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z); // InitPolarCrate
    virtual ~PolarCrate();                                                // 1 DestroyPolarCrate
};

inline PolarCrate::~PolarCrate()
{
}

class PolarTimeCrate : public PolarCrate
{
public:
    PolarTimeCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
        : PolarCrate(rec, x, y, z)
    {
    }
    virtual ~PolarTimeCrate(); // 1 DestroyPolarTimeCrate
};

class PolarQuestionCrate : public PolarCrate
{
public:
    PolarQuestionCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
        : PolarCrate(rec, x, y, z)
    {
    }
    virtual ~PolarQuestionCrate(); // 1 DestroyPolarQuestionCrate
};

class PolarAkuAkuCrate : public PolarCrate
{
public:
    PolarAkuAkuCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
        : PolarCrate(rec, x, y, z)
    {
    }
    virtual ~PolarAkuAkuCrate(); // 1 DestroyPolarAkuAkuCrate
};

class PolarNitroCrate : public PolarCrate
{
public:
    PolarNitroCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
        : PolarCrate(rec, x, y, z)
    {
    }
    virtual ~PolarNitroCrate(); // 1 DestroyPolarNitroCrate
};

/* Remembers the spawn record that built it, for MarkSpawnCollected
 * (vehicle.h's `struct polar_life_crate`). */
class PolarLifeCrate : public PolarCrate
{
public:
    void *spawn; // 0x54

    PolarLifeCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z, void *spawn)
        : PolarCrate(rec, x, y, z)
    {
        this->spawn = spawn;
    }
    virtual ~PolarLifeCrate(); // 1 DestroyPolarLifeCrate
};

COMPILE_TIME_ASSERT(vehicle_hpp, sizeof(PolarLifeCrate) == sizeof(struct polar_life_crate));

class PolarFourWumpaCrate : public PolarCrate
{
public:
    PolarFourWumpaCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
        : PolarCrate(rec, x, y, z)
    {
    }
    virtual ~PolarFourWumpaCrate(); // 1 DestroyPolarFourWumpaCrate
};

class PolarBasicCrate : public PolarCrate
{
public:
    PolarBasicCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
        : PolarCrate(rec, x, y, z)
    {
    }
    virtual ~PolarBasicCrate(); // 1 DestroyPolarBasicCrate
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

/* Hops from spawn to spawn (AimPolarPenguin). */
class PolarPenguin : public ActorSelf
{
public:
    s32 velX;       // 0x54
    s32 velY;       // 0x58
    s32 velZ;       // 0x5C
    s32 countdown;  // 0x60 - frames until the next retarget
    s32 nextTarget; // 0x64 - the spawn index it aims for next

    // CreatePolarPenguin
    PolarPenguin(const struct anim_table_record *rec, s32 x, s32 y, s32 z, struct spawn_arg *arg);
    virtual ~PolarPenguin(); // 1 DestroyPolarPenguin
};

COMPILE_TIME_ASSERT(vehicle_hpp, sizeof(PolarPenguin) == 0x68);

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
    u8 once; // 0x54 - the boost was given

    // CreatePolarBoostPad
    PolarBoostPad(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarBoostPad(); // 1 DestroyPolarBoostPad
};

COMPILE_TIME_ASSERT(vehicle_hpp, sizeof(PolarBoostPad) == 0x58);

class PolarCheckpointCrate : public ActorSelf
{
public:
    // CreatePolarCheckpointCrate
    PolarCheckpointCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarCheckpointCrate(); // 1 DestroyPolarCheckpointCrate
};

/* The jetpack levels (7 vtable slots: HpActor's; the balloon crates
 * add an 8th). */

/* The jetpack player (gJetpackPlayerVtable; src/vehicle/jetpack_spawn.cpp,
 * jetpack_player.cpp and jetpack_run.cpp): its hit points are HpActor's,
 * shown as a percentage (GetHp); the rest of its state is in the
 * gJetpack* globals (vehicle.h), as the ROM has it. CreateJetpackPlayer
 * makes it gActorList, the actor list's root. */
class JetpackPlayer : public HpActor
{
public:
    JetpackPlayer(const struct anim_table_record *rec, s32 z); // InitJetpackPlayer
    virtual ~JetpackPlayer();                                  // 1 DestroyJetpackPlayer
    virtual void Update();                                     // 2 UpdateJetpackPlayer
    virtual void Draw();                                       // 3 DrawJetpackPlayer
    virtual void Damage(s32 amount);                           // 4 DamageJetpackPlayer
    virtual s32 GetHp();                                       // 6 GetJetpackPlayerHpPercent

    void SteerY();               // SteerJetpackPlayerY
    void SteerX();               // SteerJetpackPlayerX
    void DispenseWumpa();        // DispenseJetpackWumpa
    s32 CountBomber();           // CountJetpackBomber
    void SetCheckpoint();        // SetJetpackCheckpoint
    s32 IsPauseLocked();         // IsJetpackPauseLocked
    void AnimatePalette();       // AnimateJetpackPlayerPalette
    void Heal(s32 delta);        // HealJetpackPlayer
    void QueueWumpa(s32 delta);  // QueueJetpackWumpa
    void RunState();             // RunJetpackPlayerState
    void FinishRun();            // FinishJetpackRun
    void PassRing(s32 x, s32 y); // PassJetpackRing
    void AllocTiles();           // AllocJetpackPlayerTiles

    /* The states, indexed by `state` (stateFuncs, gJetpackPlayerStateFuncs). */
    void StateEnter();     // JetpackPlayerStateEnter
    void StateFly();       // JetpackPlayerStateFly
    void StateRollLeft();  // JetpackPlayerStateRollLeft
    void StateRollRight(); // JetpackPlayerStateRollRight
    void StateFall();      // JetpackPlayerStateFall
    void StateFinish();    // JetpackPlayerStateFinish
    void StateBoost();     // JetpackPlayerStateBoost
    void StateResume();    // JetpackPlayerStateResume

    typedef void (JetpackPlayer::*StateFunc)();
    static const StateFunc stateFuncs[8];
};

COMPILE_TIME_ASSERT(vehicle_hpp, sizeof(JetpackPlayer) == 0x58);
COMPILE_TIME_ASSERT(vehicle_hpp, sizeof(JetpackPlayer) == sizeof(struct actor_hp));

/* The checkpoint banner, drawn at a fixed screen position. */
class JetpackCheckpointText : public HpActor
{
public:
    /* Inline: CreateJetpackCheckpointText expands it. */
    JetpackCheckpointText(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
        : HpActor(rec, x, y, z, 1)
    {
    }
    virtual ~JetpackCheckpointText(); // 1 DestroyJetpackCheckpointText
    virtual void Update();            // 2 UpdateJetpackCheckpointText
    virtual void Draw();              // 3 DrawJetpackCheckpointText
    virtual s32 IsUnshootable();      // 5 IsJetpackCheckpointTextUnshootable
};

class JetpackExplosion : public HpActor
{
public:
    /* Inline: CreateJetpackExplosion expands it. */
    JetpackExplosion(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
        : HpActor(rec, x, y, z, 1)
    {
    }
    virtual ~JetpackExplosion(); // 1 DestroyJetpackExplosion
    virtual void Update();       // 2 UpdateJetpackExplosion
    virtual s32 IsUnshootable(); // 5 IsJetpackExplosionUnshootable
};

/* The jetpack player's shot (src/vehicle/jetpack_shot.cpp): it flies by
 * its speed, and hits the first shootable actor or the airship. */
class JetpackShot : public HpActor
{
public:
    s32 velX; // 0x58
    s32 velY; // 0x5C

    // CreateJetpackShot
    JetpackShot(const struct anim_table_record *rec, s32 x, s32 y, s32 z, s32 velX, s32 velY);
    virtual ~JetpackShot();      // 1 DestroyJetpackShot
    virtual void Update();       // 2 UpdateJetpackShot
    virtual s32 IsUnshootable(); // 5 IsJetpackShotUnshootable
};

COMPILE_TIME_ASSERT(vehicle_hpp, sizeof(JetpackShot) == 0x60);

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
