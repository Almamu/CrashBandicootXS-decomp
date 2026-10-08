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
 * and explosion's constructors their code, part 11h the jetpack ring and
 * the collected wumpa (src/bosses/hovercraft.cpp).
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

/* The polar run's player (gPolarPlayerVtable; src/vehicle/polar_player.cpp,
 * polar_player_states.cpp, polar_player_actions.cpp and
 * polar_player_dispatch.cpp): it rides the polar bear, steered left and
 * right, and jumps; the rest of its state is in the gPolar* globals
 * (vehicle.h), as the ROM has it. ConstructAnimTableState
 * (actor_factory.cpp) makes it gActorList, the actor list's root. The
 * methods the other actors call on gActorList (Hurt, QueueWumpa, ...)
 * keep their C prototypes in vehicle.h for the C files. */
class PolarPlayer : public ActorSelf
{
public:
    PolarPlayer(const struct anim_table_record *rec, s32 z); // ConstructActorPart
    virtual ~PolarPlayer();                                  // 1 DestroyPolarPlayer
    virtual void Update();                                   // 2 UpdatePolarPlayer
    virtual void Draw();                                     // 3 DrawPolarPlayer

    s32 Hurt();             // HurtPolarPlayer
    s32 Shock();            // ShockPolarPlayer
    void AllocTiles();      // AllocPolarPlayerTiles
    void DispenseWumpa();   // DispensePolarWumpa
    s32 IsPauseLocked();    // IsPolarPauseLocked
    void FinishRun();       // FinishPolarRun
    void Catch();           // CatchPolarPlayer
    void QueueWumpa(s32 n); // QueuePolarWumpa
    void GiveLife();        // GivePolarPlayerLife
    void Boost(s32 x);      // BoostPolarPlayer
    void GiveMask();        // GivePolarPlayerMask
    void Launch();          // LaunchPolarPlayer
    void RunState();        // RunPolarPlayerState

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

COMPILE_TIME_ASSERT(vehicle_hpp, sizeof(PolarPlayer) == sizeof(struct actor_self));

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

/* The plane (gJetpackPlaneVtable, src/vehicle/jetpack_plane.cpp): it hops
 * from spawn point to spawn point (Aim, the GetActorSpawn* accessors) and
 * fires cannonballs at the player from its low pose; shot down, it falls
 * out of the sky. */
class JetpackPlane : public HpActor
{
public:
    s32 cooldown;  // 0x58 - frames to the next cannonball
    s32 shotCount; // 0x5C - cannonballs fired since the last long cooldown
    s32 velX;      // 0x60
    s32 velY;      // 0x64
    s32 velZ;      // 0x68 - the hop speed (gJetpackPlaneHopSpeeds)
    s32 accX;      // 0x6C
    s32 accY;      // 0x70
    s32 steps;     // 0x74 - frames left in the hop
    s32 next;      // 0x78 - the next spawn point it aims for
    u8 dying;      // 0x7C

    // CreateJetpackPlane
    JetpackPlane(const struct anim_table_record *rec, s32 x, s32 y, s32 z, struct spawn_arg *arg);
    virtual ~JetpackPlane();         // 1 DestroyJetpackPlane
    virtual void Update();           // 2 UpdateJetpackPlane
    virtual void Damage(s32 amount); // 4 DamageJetpackPlane
    virtual s32 IsUnshootable();     // 5 IsJetpackPlaneUnshootable

    void Aim(s32 target); // AimJetpackPlane
    void RunState();      // RunJetpackPlaneState

    /* The states, indexed by `state` (stateFuncs, gJetpackPlaneStateFuncs). */
    void StateFly();        // JetpackPlaneStateFly
    void StateFollow();     // JetpackPlaneStateFollow
    void StateKnockedOut(); // JetpackPlaneStateKnockedOut
    void StateFall();       // JetpackPlaneStateFall

    typedef void (JetpackPlane::*StateFunc)();
    static const StateFunc stateFuncs[4];
};

COMPILE_TIME_ASSERT(vehicle_hpp, sizeof(JetpackPlane) == 0x80);

/* The bomber (gJetpackBomberVtable, src/vehicle/jetpack_plane.cpp): its
 * record's kind (4-9) picks how it moves around its home point; it
 * explodes on the player. */
class JetpackBomber : public HpActor
{
public:
    s32 homeX;      // 0x58
    s32 homeY;      // 0x5C
    u8 unshootable; // 0x60 - only ever cleared

    // CreateJetpackBomber
    JetpackBomber(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~JetpackBomber();        // 1 DestroyJetpackBomber
    virtual void Update();           // 2 UpdateJetpackBomber
    virtual void Damage(s32 amount); // 4 DamageJetpackBomber
    virtual s32 IsUnshootable();     // 5 IsJetpackBomberUnshootable

    void Home();     // HomeJetpackBomber
    void RunState(); // RunJetpackBomberState

    /* The states, indexed by `state` (stateFuncs, gJetpackBomberStateFuncs). */
    void StateIdle();            // JetpackBomberStateIdle
    void StateHome();            // JetpackBomberStateHome
    void StateBobVertical();     // JetpackBomberStateBobVertical
    void StateSwingHorizontal(); // JetpackBomberStateSwingHorizontal
    void StateCircle();          // JetpackBomberStateCircle
    void StateDrop();            // JetpackBomberStateDrop
    void StateDying();           // JetpackBomberStateDying

    typedef void (JetpackBomber::*StateFunc)();
    static const StateFunc stateFuncs[7];
};

COMPILE_TIME_ASSERT(vehicle_hpp, sizeof(JetpackBomber) == 0x64);

/* The cannonball the planes and the airship fire
 * (gJetpackCannonballVtable, src/vehicle/jetpack_plane.cpp). */
class JetpackCannonball : public HpActor
{
public:
    s32 velX; // 0x58
    s32 velY; // 0x5C

    // CreateJetpackCannonball
    JetpackCannonball(const struct anim_table_record *rec, s32 x, s32 y, s32 z, s32 velX, s32 velY);
    virtual ~JetpackCannonball(); // 1 DestroyJetpackCannonball
    virtual void Update();        // 2 UpdateJetpackCannonball
    virtual s32 IsUnshootable();  // 5 IsJetpackCannonballUnshootable
};

COMPILE_TIME_ASSERT(vehicle_hpp, sizeof(JetpackCannonball) == 0x60);

class JetpackBalloonCrate;

/* The balloon a crate hangs from (gJetpackBalloonVtable,
 * src/vehicle/jetpack_balloon.cpp): the crate moves it (Move) until it is
 * released (Release) and floats away, or is shot and pops. */
class JetpackBalloon : public HpActor
{
public:
    JetpackBalloonCrate *crate; // 0x58 - the crate hanging from it
    u8 dying;                   // 0x5C
    s32 velY;                   // 0x60

    // CreateJetpackBalloon
    JetpackBalloon(const struct anim_table_record *rec, s32 x, s32 y, s32 z,
                   JetpackBalloonCrate *crate);
    virtual ~JetpackBalloon();       // 1 DestroyJetpackBalloon
    virtual void Update();           // 2 UpdateJetpackBalloon
    virtual void Damage(s32 amount); // 4 DamageJetpackBalloon
    virtual s32 IsUnshootable();     // 5 IsJetpackBalloonUnshootable

    void ClearCrate();              // ClearJetpackBalloonCrate
    void Release();                 // ReleaseJetpackBalloon
    void Move(s32 x, s32 y, s32 z); // MoveJetpackBalloon
    void Animate();                 // the animation step (inline, jetpack_balloon.cpp)
    void RunState();                // RunJetpackBalloonState

    /* The states, indexed by `state` (stateFuncs, gJetpackBalloonStateFuncs). */
    void StateAttached();  // JetpackBalloonStateAttached
    void StateFloatAway(); // JetpackBalloonStateFloatAway
    void StatePop();       // JetpackBalloonStatePop

    typedef void (JetpackBalloon::*StateFunc)();
    static const StateFunc stateFuncs[3];
};

COMPILE_TIME_ASSERT(vehicle_hpp, sizeof(JetpackBalloon) == 0x64);

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
    /* Slot 7 is declared for JetpackBalloon::Damage's call (part 11f). */
    virtual void Break(); // 7 BreakJetpackBalloonCrate (jetpack_crates.c)
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

/* A jetpack ring (gJetpackRingVtable; vehicle.h's `struct jetpack_ring` is
 * its C view). Its constructor and slot 5 are in src/bosses/hovercraft.cpp
 * (part 11h), its Update is still C (jetpack_crates.c, part 11g). */
class JetpackRing : public HpActor
{
public:
    u8 cued; // 0x58 - set once UpdateJetpackRing has played its cue

    // CreateJetpackRing
    JetpackRing(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~JetpackRing();      // 1 DestroyJetpackRing
    virtual void Update();       // 2 UpdateJetpackRing (jetpack_crates.c)
    virtual s32 IsUnshootable(); // 5 IsJetpackRingUnshootable
};

COMPILE_TIME_ASSERT(vehicle_hpp, sizeof(JetpackRing) == sizeof(struct jetpack_ring));

/* A collected wumpa of the jetpack levels (gJetpackCollectedWumpaVtable,
 * src/bosses/hovercraft.cpp, part 11h; PolarCollectedWumpa's twin): it
 * flies from where it was collected to the HUD's wumpa counter, and its
 * destructor adds the fruit it carries. */
class JetpackCollectedWumpa : public HpActor
{
public:
    s32 velX;   // 0x58
    s32 velY;   // 0x5C
    s32 reward; // 0x60 - how many fruit the destructor adds

    // CreateJetpackCollectedWumpa
    JetpackCollectedWumpa(const struct anim_table_record *rec, s32 x, s32 y, s32 reward);
    virtual ~JetpackCollectedWumpa(); // 1 DestroyJetpackCollectedWumpa
    virtual void Update();            // 2 UpdateJetpackCollectedWumpa
    virtual void Draw();              // 3 DrawJetpackCollectedWumpa
    virtual s32 IsUnshootable();      // 5 IsJetpackCollectedWumpaUnshootable
};

COMPILE_TIME_ASSERT(vehicle_hpp, sizeof(JetpackCollectedWumpa) == 0x64);

#endif /* GUARD_VEHICLE_HPP */
