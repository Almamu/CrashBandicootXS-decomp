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
 * and explosion's constructors their code; part 11c the polar player;
 * part 11h the jetpack ring and the collected wumpa
 * (src/bosses/hovercraft.cpp); part 11d the other polar actors
 * (src/vehicle/polar_crates.cpp, polar_pickups.cpp, polar_objects.cpp,
 * polar_aku_aku.cpp and polar_nitro.cpp), so every polar class is C++;
 * part 11g the balloon crates, the parachute nitro and the rocket
 * (src/vehicle/jetpack_crates.cpp).
 *
 * No `#pragma interface`: g++ emits the vtables, each in its key-method
 * object: most in actor_anim.cpp, where the classes' destructors are (see
 * ctrl.hpp). */

#include "actor_self.hpp"

extern "C" {
#include "vehicle.h"
}

/* The polar bear levels (4 vtable slots: ActorSelf's). The
 * constructors CreateActor and the other factories in
 * src/actor/actor_factory.cpp use are inline where the ROM expands them
 * there (the crates' and the wumpa's also have an out-of-line copy,
 * Create*, in src/vehicle/), the others out of line. */

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
 * other actors call its methods on `static_cast<PolarPlayer *>(gActorList)`,
 * as do the actor zone's C-linkage category hooks (actor.cpp,
 * actor_spawn.cpp); Catch, which yeti_update.c calls, keeps its C
 * prototype in vehicle.h. */
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

COMPILE_TIME_ASSERT(vehicle_hpp, sizeof(PolarPlayer) == sizeof(ActorSelf));

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

/* The wumpa fruit flying to the HUD (gPolarCollectedWumpaVtable;
 * src/vehicle/polar_pickups.cpp): it flies at a fixed speed to the
 * wumpa counter's corner, and its destructor counts in its fruit. */
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

/* A wumpa fruit on the course (gPolarWumpaVtable; polar_pickups.cpp). */
class PolarWumpa : public ActorSelf
{
public:
    /* Inline (CreateActor expands it); polar_pickups.cpp, which defines
     * POLAR_WUMPA_CONSTRUCTOR_OUT_OF_LINE, has the ROM's out-of-line copy
     * (CreatePolarWumpa, no caller). */
    PolarWumpa(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarWumpa(); // 1 DestroyPolarWumpa
    virtual void Update(); // 2 UpdatePolarWumpa
};

#ifndef POLAR_WUMPA_CONSTRUCTOR_OUT_OF_LINE
inline PolarWumpa::PolarWumpa(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
    : ActorSelf(rec, x, y, z)
{
}
#endif

/* The crates' base (gPolarCrateVtable; src/vehicle/polar_crates.cpp and
 * polar_pickups.cpp). Its constructor picks one of 18 looks by the
 * crate's place on the course. A crate breaks (animation 0x12, Break)
 * when the player or the yeti touches it, and Update deletes it once
 * that animation has played. Its destructor is inline, as every crate
 * kind's expands it; actor_anim.cpp has the out-of-line copy
 * (DestroyPolarCrate). */
class PolarCrate : public ActorSelf
{
public:
    PolarCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z); // InitPolarCrate
    virtual ~PolarCrate();                                                // 1 DestroyPolarCrate
    virtual void Update();                                                // 2 UpdatePolarCrate

    /* The broken animation. */
    void Break()
    {
        RestartAnim(0x12);
    }
};

/* src/actor/actor_anim.cpp: the C-linkage copy of PolarCrate's inline
 * destructor, which the g++-emitted vtables point at (cxx_symbols.txt). */
extern "C" void DestroyPolarCrate(PolarCrate *self, u32 flags);

inline PolarCrate::~PolarCrate()
{
}

/* The crate kinds (polar_crates.cpp and polar_pickups.cpp). Their
 * constructors are inline (CreateActor expands them) and also out of line
 * at the end of polar_crates.cpp (CreatePolarTimeCrate, ...), from one
 * source, polar_crate_ctors.hpp. */

/* Freezes the clock for 1-3 seconds (kinds 5-7). */
class PolarTimeCrate : public PolarCrate
{
public:
    PolarTimeCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarTimeCrate(); // 1 DestroyPolarTimeCrate
    virtual void Update();     // 2 UpdatePolarTimeCrate
};

/* 1, 3 or 5 wumpas, or a mask (kinds 0x1C-0x1F). */
class PolarQuestionCrate : public PolarCrate
{
public:
    PolarQuestionCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarQuestionCrate(); // 1 DestroyPolarQuestionCrate
    virtual void Update();         // 2 UpdatePolarQuestionCrate
};

class PolarAkuAkuCrate : public PolarCrate
{
public:
    PolarAkuAkuCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarAkuAkuCrate(); // 1 DestroyPolarAkuAkuCrate
    virtual void Update();       // 2 UpdatePolarAkuAkuCrate
};

/* Hurts the player, and once broken sets off the nitros next to it. */
class PolarNitroCrate : public PolarCrate
{
public:
    PolarNitroCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarNitroCrate(); // 1 DestroyPolarNitroCrate
    virtual void Update();      // 2 UpdatePolarNitroCrate

    void Detonate();       // DetonatePolarNitroCrate (UNUSED)
    void DetonateNearby(); // DetonateNearbyPolarNitros (polar_nitro.cpp)

    /* Broken by an explosion: the state's timer restarts too. */
    void Explode()
    {
        stateTime = 0;
        Break();
    }
};

/* An extra life; remembers the spawn record that built it, for
 * MarkSpawnCollected. */
class PolarLifeCrate : public PolarCrate
{
public:
    void *spawn; // 0x54

    PolarLifeCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z, void *spawn);
    virtual ~PolarLifeCrate(); // 1 DestroyPolarLifeCrate
    virtual void Update();     // 2 UpdatePolarLifeCrate
};

COMPILE_TIME_ASSERT(vehicle_hpp, sizeof(PolarLifeCrate) == 0x58);

class PolarFourWumpaCrate : public PolarCrate
{
public:
    PolarFourWumpaCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarFourWumpaCrate(); // 1 DestroyPolarFourWumpaCrate
    virtual void Update();          // 2 UpdatePolarFourWumpaCrate
};

class PolarBasicCrate : public PolarCrate
{
public:
    PolarBasicCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarBasicCrate(); // 1 DestroyPolarBasicCrate
    virtual void Update();      // 2 UpdatePolarBasicCrate
};

/* polar_crates.cpp defines POLAR_CRATE_CONSTRUCTORS_OUT_OF_LINE and
 * includes the constructors at its end, plain. */
#ifndef POLAR_CRATE_CONSTRUCTORS_OUT_OF_LINE
#define POLAR_CRATE_CTOR inline
#include "polar_crate_ctors.hpp"
#undef POLAR_CRATE_CTOR
#endif

/* The hazards and objects (src/vehicle/polar_objects.cpp and
 * polar_aku_aku.cpp). Their constructors are out of line. */

/* An electric fence (gPolarElectricFenceVtable): shown once near enough,
 * it shocks the player on its wire and hurts it on its posts. */
class PolarElectricFence : public ActorSelf
{
public:
    // CreatePolarElectricFence
    PolarElectricFence(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarElectricFence(); // 1 DestroyPolarElectricFence
    virtual void Update();         // 2 UpdatePolarElectricFence
};

/* A rock in the way (two parts, CreateActor kinds 13 and 14). */
class PolarObstacle : public ActorSelf
{
public:
    // CreatePolarObstacle
    PolarObstacle(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarObstacle(); // 1 DestroyPolarObstacle
    virtual void Update();    // 2 UpdatePolarObstacle
};

/* A ramp that launches the player (state 1 once used). */
class PolarLauncher : public ActorSelf
{
public:
    // CreatePolarLauncher
    PolarLauncher(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarLauncher(); // 1 DestroyPolarLauncher
    virtual void Update();    // 2 UpdatePolarLauncher
};

/* Hops from spawn to spawn (Aim); knocked away (state 1) by the player or
 * the yeti. */
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
    virtual void Update();   // 2 UpdatePolarPenguin

    void Aim(s32 target); // AimPolarPenguin
};

COMPILE_TIME_ASSERT(vehicle_hpp, sizeof(PolarPenguin) == 0x68);

/* An icicle: its look depends on its record and side, and it falls in
 * steps (state 0-2) as it comes near. */
class PolarIcicle : public ActorSelf
{
public:
    // CreatePolarIcicle
    PolarIcicle(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarIcicle(); // 1 DestroyPolarIcicle
    virtual void Update();  // 2 UpdatePolarIcicle
};

/* Aku Aku following the player (gPolarAkuAku; SpawnPolarAkuAku): its look
 * is the mask level (gLevelState->maskLevel), state 1 is invincible
 * (gPolarAkuAkuInvincibleTimer), state 2 a lost mask. */
class PolarAkuAku : public ActorSelf
{
public:
    // CreatePolarAkuAku
    PolarAkuAku(const struct anim_table_record *rec, s32 x, s32 y, s32 z, s32 level);
    virtual ~PolarAkuAku(); // 1 DestroyPolarAkuAku
    virtual void Update();  // 2 UpdatePolarAkuAku

    void Refresh(u8 lost);          // RefreshPolarAkuAku
    void Move(s32 x, s32 y, s32 z); // MovePolarAkuAku
    void ClearMask();               // ClearPolarAkuAkuMask
    s32 RemoveMask();               // RemovePolarAkuAkuMask
    s32 AddMask();                  // AddPolarAkuAkuMask
    void SetMask(s32 level);        // SetPolarMaskLevel
};

/* The finish line (two parts, CreateActor kinds 25 and 26), shown after
 * 5 frames. */
class PolarGoal : public ActorSelf
{
public:
    // CreatePolarGoal
    PolarGoal(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarGoal();  // 1 DestroyPolarGoal
    virtual void Update(); // 2 UpdatePolarGoal
};

/* A boost pad: its look is its side of the course. */
class PolarBoostPad : public ActorSelf
{
public:
    u8 once; // 0x54 - the boost's sound was played

    // CreatePolarBoostPad
    PolarBoostPad(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarBoostPad(); // 1 DestroyPolarBoostPad
    virtual void Update();    // 2 UpdatePolarBoostPad
};

COMPILE_TIME_ASSERT(vehicle_hpp, sizeof(PolarBoostPad) == 0x58);

/* The checkpoint crate: animation 0 whole, 1 opened by the player, 2
 * already the checkpoint, 3 broken by the yeti. */
class PolarCheckpointCrate : public ActorSelf
{
public:
    // CreatePolarCheckpointCrate
    PolarCheckpointCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarCheckpointCrate(); // 1 DestroyPolarCheckpointCrate
    virtual void Update();           // 2 UpdatePolarCheckpointCrate
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
COMPILE_TIME_ASSERT(vehicle_hpp, sizeof(JetpackPlayer) == 0x58);

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

/* The crates hanging from balloons (gJetpackBalloonCrateVtable,
 * src/vehicle/jetpack_crates.cpp): each hangs from a balloon of its own
 * (SpawnJetpackBalloon), swaying around its spawn point, until the
 * balloon is shot (Break: it falls) or the crate is (Damage: it breaks
 * and lets the balloon go). The three kinds built on it pay out when
 * broken or touched; their destructors are g++'s implicit ones
 * (DestroyJetpackHealthCrate, DestroyJetpackTimeCrate and
 * DestroyJetpackQuestionCrate, actor_anim.cpp). */
class JetpackBalloonCrate : public HpActor
{
public:
    JetpackBalloon *balloon; // 0x58 - the balloon it hangs from
    u8 done;                 // 0x5C - broken or touched: no longer shootable
    s32 centerX;             // 0x60 - the point it sways around
    s32 centerY;             // 0x64
    s32 phase;               // 0x68 - random, added to stateTime for the sway
    s32 fallSpeed;           // 0x6C - StateFall's, capped at 0x4C0

    /* InitJetpackBalloonCrate: the constructor out of line, which nothing
     * calls; the kinds expand the inline one below. */
    JetpackBalloonCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z, u8 kind);
    virtual ~JetpackBalloonCrate();  // 1 DestroyJetpackBalloonCrate
    virtual void Update();           // 2 UpdateJetpackBalloonCrate
    virtual void Damage(s32 amount); // 4 DamageJetpackBalloonCrate
    virtual s32 IsUnshootable();     // 5 IsJetpackBalloonCrateUnshootable
    virtual void Break();            // 7 BreakJetpackBalloonCrate

    void ClearBalloon(); // ClearJetpackCrateBalloon
    void RunState();     // RunJetpackBalloonCrateState

    /* The states, indexed by `state` (stateFuncs,
     * gJetpackBalloonCrateStateFuncs). */
    void StateHang();      // JetpackBalloonCrateStateHang
    void StateFall();      // JetpackBalloonCrateStateFall
    void StateDestroyed(); // JetpackBalloonCrateStateDestroyed

    typedef void (JetpackBalloonCrate::*StateFunc)();
    static const StateFunc stateFuncs[3];

protected:
    /* The same constructor, inline (jetpack_crates.cpp): the kinds'
     * constructors expand it. An `int` kind picks this one. */
    JetpackBalloonCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z, s32 kind);

    /* The constructors' body (inline, jetpack_crates.cpp). */
    void Hang(s32 x, s32 y, s32 z, u8 kind);
};

COMPILE_TIME_ASSERT(vehicle_hpp, sizeof(JetpackBalloonCrate) == 0x70);

/* Heals the player (gJetpackHealthCrateVtable). */
class JetpackHealthCrate : public JetpackBalloonCrate
{
public:
    // CreateJetpackHealthCrate
    JetpackHealthCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual void Update();           // 2 UpdateJetpackHealthCrate
    virtual void Damage(s32 amount); // 4 DamageJetpackHealthCrate
};

/* Freezes the level clock, or starts the time trial
 * (gJetpackTimeCrateVtable). */
class JetpackTimeCrate : public JetpackBalloonCrate
{
public:
    // CreateJetpackTimeCrate
    JetpackTimeCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual void Update();           // 2 UpdateJetpackTimeCrate
    virtual void Damage(s32 amount); // 4 DamageJetpackTimeCrate
};

/* Wumpa fruit, or an extra life (gJetpackQuestionCrateVtable). */
class JetpackQuestionCrate : public JetpackBalloonCrate
{
public:
    void *spawn; // 0x70 - the level spawn record, for MarkSpawnCollected

    // CreateJetpackQuestionCrate
    JetpackQuestionCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z, void *spawn);
    virtual void Update();           // 2 UpdateJetpackQuestionCrate
    virtual void Damage(s32 amount); // 4 DamageJetpackQuestionCrate
};

COMPILE_TIME_ASSERT(vehicle_hpp, sizeof(JetpackQuestionCrate) == 0x74);

/* A nitro crate on a parachute (gJetpackParachuteNitroVtable,
 * src/vehicle/jetpack_crates.cpp): it rises to `limitY`, and explodes on
 * the player or when shot. */
class JetpackParachuteNitro : public HpActor
{
public:
    u8 dead;    // 0x58 - exploded: no longer shootable
    s32 limitY; // 0x5C

    // CreateJetpackParachuteNitro
    JetpackParachuteNitro(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~JetpackParachuteNitro(); // 1 DestroyJetpackParachuteNitro
    virtual void Update();            // 2 UpdateJetpackParachuteNitro
    virtual void Damage(s32 amount);  // 4 DamageJetpackParachuteNitro
    virtual s32 IsUnshootable();      // 5 IsJetpackParachuteNitroUnshootable
};

COMPILE_TIME_ASSERT(vehicle_hpp, sizeof(JetpackParachuteNitro) == 0x60);

/* A rocket (gJetpackRocketVtable, src/vehicle/jetpack_crates.cpp): it
 * swings around `originX` while it comes down by `stepY` to `limitY`,
 * then explodes (Launch); it hurts the player once on contact. */
class JetpackRocket : public HpActor
{
public:
    s32 originX;  // 0x58
    s32 limitY;   // 0x5C
    s32 stepY;    // 0x60
    u8 triggered; // 0x64 - exploded or shot: no longer shootable
    u8 hit;       // 0x65 - it has hurt the player

    // CreateJetpackRocket
    JetpackRocket(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~JetpackRocket();        // 1 DestroyJetpackRocket
    virtual void Update();           // 2 UpdateJetpackRocket
    virtual void Damage(s32 amount); // 4 DamageJetpackRocket
    virtual s32 IsUnshootable();     // 5 IsJetpackRocketUnshootable

    void Launch(); // LaunchJetpackRocket
};

COMPILE_TIME_ASSERT(vehicle_hpp, sizeof(JetpackRocket) == 0x68);

/* A jetpack ring (gJetpackRingVtable). Its constructor and slot 5 are in src/bosses/hovercraft.cpp
 * (part 11h), its Update in src/vehicle/jetpack_crates.cpp (part 11g). */
class JetpackRing : public HpActor
{
public:
    u8 cued; // 0x58 - set once UpdateJetpackRing has played its cue

    // CreateJetpackRing
    JetpackRing(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~JetpackRing();      // 1 DestroyJetpackRing
    virtual void Update();       // 2 UpdateJetpackRing
    virtual s32 IsUnshootable(); // 5 IsJetpackRingUnshootable
};

COMPILE_TIME_ASSERT(vehicle_hpp, sizeof(JetpackRing) == 0x5C);

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
