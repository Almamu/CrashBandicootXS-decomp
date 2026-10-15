#ifndef GUARD_VEHICLE_HPP
#define GUARD_VEHICLE_HPP

/* The vehicle levels' 3D actors as C++ (#664, docs/cplusplus.md), on
 * ActorSelf and HpActor (actor_self.hpp). Part 11a declared what the
 * C++ objects needed then: the destructors that src/game/vehicles/actor_destructors.cpp
 * has (the ROM keeps them there, with a few small methods), and the polar
 * player's state dispatch and table (src/game/vehicles/polar/polar_player_states.cpp,
 * src/game/vehicles/polar/data/polar_player_state_table.cpp). Part 11b added the polar actors' fields
 * and the constructors src/game/vehicles/polar/polar_actor_factory.cpp uses. Their other
 * methods, and the jetpack actors' fields past HpActor's, are still C
 * (src/game/vehicles/); cxx_symbols.txt maps the C++ names to the C
 * ones. The constructors are declared with ActorSelf's arguments and the
 * extra ones of their C prototypes (Create*, Init*, named in a comment);
 * where the ROM only has a constructor inlined and its code is still C,
 * its signature is a placeholder until it is converted. Part 11e gives
 * the jetpack player (JetpackPlayer), its shot and the checkpoint banner's
 * and explosion's constructors their code; part 11c the polar player;
 * part 11h the jetpack ring and the collected wumpa
 * (src/game/vehicles/jetpack/jetpack_collected_wumpa.cpp); part 11d the other
 * polar actors (src/game/vehicles/polar/polar_crates.cpp, polar_pickups.cpp,
 * polar_crate.cpp, polar_objects.cpp, polar_aku_aku.cpp,
 * polar_course_objects.cpp and polar_nitro.cpp), so every polar class is
 * C++; part 11g the balloon crates, the parachute nitro and the rocket
 * (src/game/vehicles/jetpack/jetpack_crates.cpp, jetpack_rocket.cpp).
 *
 * No `#pragma interface`: g++ emits the vtables, each in its key-method
 * object: most in actor_destructors.cpp, where the classes' destructors are (see
 * ctrl.hpp). */

#include "game/entities/actor_self.hpp"

extern "C" {
#include "core.h"
#include "engine/actor/actor.h"
}

/* Every function the vehicle levels define with C linkage (or a C name,
 * cxx_symbols.txt) is declared here, including those the file layout put
 * in actor or bosses files for ROM order (the teardown functions in
 * actor_destructors.cpp, ...), plus their globals and data tables.
 * The per-category hooks (actor_category_hooks.cpp,
 * actor_category_frame.cpp, actor_category.cpp) are static members of
 * PolarPlayer and JetpackPlayer (#915).
 *
 * The polar levels' classes are in the namespace Game::Vehicles::Polar
 * (#1033, docs/cplusplus.md "Namespaces"), mangled as
 * `Q44Game8Vehicles5Polar`: `Update__Q44Game8Vehicles5Polar11PolarPlayer`,
 * `_vt.Q44Game8Vehicles5Polar11PolarPlayer` (in the section
 * `.gnu.linkonce.d._vt.Q44Game8Vehicles5Polar11PolarPlayer`, which
 * ldscript.txt names) & co. The jetpack levels' classes are in
 * Game::Vehicles::Jetpack the same way (`Q44Game8Vehicles7Jetpack`). The
 * C-linkage functions, the globals, the data tables and `struct
 * spawn_arg` stay global. */

/* src/game/vehicles/actor_destructors.cpp: the C-linkage copies of the balloon crate
 * kinds' implicit destructors (below), which the g++-emitted
 * vtables point at (cxx_symbols.txt); PolarCrate's is below */
extern "C" void DestroyJetpackHealthCrate(void *self, u32 flags);
extern "C" void DestroyJetpackTimeCrate(void *self, u32 flags);
extern "C" void DestroyJetpackQuestionCrate(void *self, u32 flags);

/* src/game/vehicles/jetpack/jetpack_crates.cpp: JetpackBalloonCrate's destructor
 * (below), for actor_destructors.cpp's kinds' destructors */
extern "C" void DestroyJetpackBalloonCrate(void *self, s32 flags);

/* src/game/vehicles/jetpack/jetpack_player.cpp: a C-linkage getter */
extern "C" u8 IsJetpackPlayerInactive(void);

/* src/game/vehicles/polar/polar_aku_aku.cpp and polar_player_states.cpp: the
 * C-linkage functions. */
extern "C" s32 GetPolarMaskLevel(void);
extern "C" u8 IsPolarPlayerInactive(void);

/* The vehicles' globals (sym_iwram.txt). */
extern struct anim_table_record *gJetpackAnimTable; /* actor_anim.h */
extern s32 gJetpackBomberCount;
extern s32 gJetpackBomberSfxTimer;
extern u8 gJetpackFadeStarted;
extern s32 gJetpackFlashTimer;
extern u8 gJetpackInputEnabled;
extern u8 gJetpackPauseLocked;
extern u8 gJetpackPlayerHalted;
extern u8 *gJetpackPlayerLastFrame;
extern s32 gJetpackPlayerMaxHp;
extern s32 gJetpackPlayerTileBuffer;
extern void *gJetpackPlayerTiles[2];
extern s32 gJetpackPlayerVelX;
extern s32 gJetpackPlayerVelY;
extern s32 gJetpackQueuedWumpa;
extern s32 gJetpackRingChain;
extern s32 gJetpackRingLastFrame;
extern s32 gJetpackShotCooldown;
extern s32 gJetpackWumpaDispenseTimer;
/* The polar actors the player keeps. */
namespace Game
{
namespace Vehicles
{
namespace Polar
{
class PolarAkuAku;
} // namespace Polar
} // namespace Vehicles
} // namespace Game
extern Game::Vehicles::Polar::PolarAkuAku *gPolarAkuAku;
extern s32 gPolarAkuAkuInvincibleTimer;
extern u8 gPolarFadeStarted;
extern s32 gPolarFinishTimer;
extern s32 gPolarInvulnTimer;
extern u8 gPolarPauseLocked;
extern u8 gPolarPlayerHalted;
extern u8 gPolarPlayerInactive;
extern u8 *gPolarPlayerLastFrame;  // the frame last uploaded
extern s32 gPolarPlayerTileBuffer; // which buffer holds the current frame
extern void *gPolarPlayerTiles[2]; // the two VRAM tile buffers
extern s32 gPolarPlayerVelY;

/* The polar player's jumps (gPolarPlayerVelY, Q8, docs/fixed_point.md):
 * a jump, and a launcher, start at -POLAR_JUMP_SPEED (7.5 px a frame
 * up); the speed grows by POLAR_GRAVITY a frame up to
 * POLAR_MAX_FALL_SPEED, and releasing A early caps the rise at
 * POLAR_JUMP_CUT_SPEED. The player runs at POLAR_GROUND_Y (40 px). */
#define POLAR_JUMP_SPEED     Q8(7.5)
#define POLAR_GRAVITY        Q8(0.375)
#define POLAR_MAX_FALL_SPEED Q8(7.5)
#define POLAR_JUMP_CUT_SPEED INT_TO_Q8(4)
#define POLAR_GROUND_Y       INT_TO_Q8(40)

/* Off the bear, the player's flights speed up (down) by
 * POLAR_PLAYER_AIR_GRAVITY a frame: dropping onto it (StateMount), the
 * leap over the finish line (StateFinishLeap) and being carried off by
 * the yeti (StateCarriedOff); the last two also move him into the
 * screen by POLAR_LEAP_SPEED_Z a frame. Steering (Update, the D-pad
 * sideways) moves him by POLAR_STEER_START_SPEED a frame for the first
 * 13 frames, POLAR_STEER_SPEED after. */
#define POLAR_PLAYER_AIR_GRAVITY Q8_FRAC(0, 0x2d)
#define POLAR_LEAP_SPEED_Z       Q8_FRAC(0, 0x3c)
#define POLAR_STEER_START_SPEED  Q8_FRAC(2, 0xcd)
#define POLAR_STEER_SPEED        Q8(3.5)

/* The jetpack and polar players' DispenseWumpa: the collected wumpas
 * queued (gJetpackQueuedWumpa, gPolarQueuedWumpa; counts, not fixed
 * point) are paid out one at a time up to VEHICLE_WUMPA_DISPENSE_1_MAX
 * queued, two at a time up to VEHICLE_WUMPA_DISPENSE_2_MAX, four up to
 * VEHICLE_WUMPA_DISPENSE_4_MAX and eight above. */
#define VEHICLE_WUMPA_DISPENSE_1_MAX 9
#define VEHICLE_WUMPA_DISPENSE_2_MAX 0x13
#define VEHICLE_WUMPA_DISPENSE_4_MAX 0x27
extern s32 gPolarQueuedWumpa;
extern u8 gPolarSteerEnabled;
extern s32 gPolarSteerTime;
extern s32 gPolarWumpaDispenseTimer;
extern Game::Entities::ActorSelf *gRiderlessPolar;
/* The yeti's (gYeti, gYetiX, ...) are Yeti's static data members
 * (include/game/vehicles/polar/yeti.hpp). */

/* src/game/vehicles/jetpack/data/jetpack_flash_palettes.cpp */
extern const u16 gJetpackFlashPalettes[3][16];

/* src/game/vehicles/jetpack/data/jetpack_rocket_box.cpp */
extern const struct anim_box gJetpackRocketBox;

/* src/game/vehicles/polar/data/polar_palettes_boxes.cpp */
extern const u16 gPolarAkuAkuPalette1[16];
extern const u16 gPolarAkuAkuPalette2[16];
extern const u16 gPolarAkuAkuPalette3[16];
extern const struct anim_box gPolarElectricFenceLeftPostBox;
extern const struct anim_box gPolarElectricFenceRightPostBox;
extern const struct anim_box gPolarElectricFenceWireBox;
extern const struct anim_box gPolarNitroCrateBox;
extern const u16 gPolarPlayerShockBlinkPalette[16];
extern const u16 gPolarPlayerShockPalette[16];

/* src/engine/sys/iwram_data.cpp */
extern s32 gPolarPenguinSpeeds[3];
extern s32 gJetpackPlaneHopSpeeds[6];
extern void (*gUnpackNibbleTilesFunc)(u16 *src, s32 lowBlock);

/* The yeti's tables (gYetiChargeParams, gYetiBox, gYetiCatchBox,
 * gYetiPalette, gYetiFrames, gYetiKeyframes, gYetiStateFuncs) are Yeti's
 * static data members (include/game/vehicles/polar/yeti.hpp). */

/* The spawn argument of CreateJetpackPlane and PolarPenguin's
 * constructor (below): the level spawn record JetpackPlayer::CreateActor
 * and PolarPlayer::CreateActor were handed (by the two SpawnActors), and
 * the word after it, its next target: a struct actor_spawn_entry
 * (actor.h) of the spawn table. */
struct spawn_arg {
    struct actor_spawn spawn; // 0x00 (actor.h)
    s32 target;               // 0x10 - the first hop target index (AimJetpackPlane,
                              //        PolarPenguin::Aim)
};

namespace Game
{
namespace Vehicles
{
namespace Polar
{

/* The polar bear levels (4 vtable slots: ActorSelf's). The
 * constructors PolarPlayer::CreateActor and the other factories in
 * src/game/vehicles/polar/polar_actor_factory.cpp use are inline where the ROM expands them
 * there (the crates' and the wumpa's also have an out-of-line copy,
 * Create*, in src/game/vehicles/), the others out of line. */

class RiderlessPolar : public Game::Entities::ActorSelf
{
public:
    RiderlessPolar(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
        : Game::Entities::ActorSelf(rec, x, y, z)
    {
    }
    virtual ~RiderlessPolar(); // 1 DestroyRiderlessPolar
};

/* The polar run's player (gPolarPlayerVtable; src/game/vehicles/polar/polar_player.cpp
 * and polar_player_states.cpp): it rides the polar bear, steered left and
 * right, and jumps; the rest of its state is in the gPolar* globals
 * (above), as the ROM has it. Create (ConstructAnimTableState)
 * (polar_actor_factory.cpp) makes it gActorList, the actor list's root. The
 * other actors call its methods on `static_cast<PolarPlayer *>(gActorList)`,
 * as do its static category hooks (IsTouchingPlayer & co., in the actor
 * zone: actor_category_frame.cpp, actor_category_hooks.cpp,
 * actor_category.cpp) and UpdateYeti (yeti_update.cpp). */

/* PolarPlayer's `state` (#843, docs/flags/actor_states_polar.md): the
 * stateFuncs index, named after its method. */
#define POLAR_PLAYER_STATE_MOUNT 0        // StateMount: drops onto the bear
#define POLAR_PLAYER_STATE_RUN 1          // StateRun: riding
#define POLAR_PLAYER_STATE_DASH 2         // StateDash: B held
#define POLAR_PLAYER_STATE_BOOST 3        // StateBoost: on a boost pad (Boost)
#define POLAR_PLAYER_STATE_JUMP 4         // StateJump
#define POLAR_PLAYER_STATE_LAUNCHED 5     // StateLaunched: off a launcher (Launch)
#define POLAR_PLAYER_STATE_KNOCKED_OFF 6  // StateKnockedOff: hurt with no mask (Hurt)
#define POLAR_PLAYER_STATE_CAUGHT 7       // StateCaught: by the yeti (Catch)
#define POLAR_PLAYER_STATE_CARRIED_OFF 8  // StateCarriedOff
#define POLAR_PLAYER_STATE_LAND 9         // StateLand: back on the bear
#define POLAR_PLAYER_STATE_FINISH 10      // StateFinish: the finish countdown ran out
#define POLAR_PLAYER_STATE_FINISH_LEAP 11 // StateFinishLeap
#define POLAR_PLAYER_STATE_SHOCKED 12     // StateShocked: the electric fence (Shock)
#define POLAR_PLAYER_STATE_RECOVER 13     // StateRecover: nothing enters it

class PolarPlayer : public Game::Entities::ActorSelf
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

    /* The polar category's hooks (ActorCategory::vtables' slots 9-12,
     * #915): static, each runs on the player, gActorList. Their code is
     * in the actor zone, for ROM order: IsTouchingPlayer in
     * actor_category_frame.cpp, ReachCourseEnd and ReloadPlayerTiles in
     * actor_category_hooks.cpp, IsPlayerPauseLocked in actor_category.cpp. */
    static s32 IsTouchingPlayer(Game::Entities::ActorSelf *self); // PolarIsTouchingPlayer
    static void ReachCourseEnd();                                 // PolarReachCourseEnd
    static void ReloadPlayerTiles();                              // PolarReloadPlayerTiles
    static s32 IsPlayerPauseLocked();                             // PolarIsPauseLocked

    /* The polar category's factory (polar_actor_factory.cpp, #915): Create and
     * SpawnActor are the category's slots 0 and 1, CreateActor the
     * per-kind `new`. */
    static void Create(struct anim_table_record *table, s32 z); // ConstructAnimTableState
    static Game::Entities::ActorSelf *SpawnActor(struct actor_spawn *spawn, u8 useBonus,
                                                 s32 zOffset); // SpawnActor
    static Game::Entities::ActorSelf *CreateActor(u8 kind, s32 x, s32 y, s32 z,
                                                  struct actor_spawn *spawn); // CreateActor
};

COMPILE_TIME_ASSERT(vehicle_hpp, sizeof(PolarPlayer) == sizeof(Game::Entities::ActorSelf));

} // namespace Polar
} // namespace Vehicles
} // namespace Game

inline Game::Vehicles::Polar::PolarPlayer *Game::Entities::ActorSelf::AsPolarPlayer()
{
    return (Game::Vehicles::Polar::PolarPlayer *)this;
}

namespace Game
{
namespace Vehicles
{
namespace Polar
{

class PolarCheckpointText : public Game::Entities::ActorSelf
{
public:
    PolarCheckpointText(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
        : Game::Entities::ActorSelf(rec, x, y, z)
    {
    }
    virtual ~PolarCheckpointText(); // 1 DestroyPolarCheckpointText
    virtual void Update();          // 2 UpdatePolarCheckpointText

    static void Create(s32 x, s32 y, s32 z); // CreatePolarCheckpointText
};

/* The wumpa fruit flying to the HUD (gPolarCollectedWumpaVtable;
 * src/game/vehicles/polar/polar_pickups.cpp): it flies at a fixed speed to the
 * wumpa counter's corner, and its destructor counts in its fruit. */
class PolarCollectedWumpa : public Game::Entities::ActorSelf
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

    static void Spawn(s32 x, s32 y, s32 count); // SpawnPolarCollectedWumpa
};

LAYOUT_ASSERT(vehicle_hpp, sizeof(PolarCollectedWumpa) == 0x60);

/* A wumpa fruit on the course (gPolarWumpaVtable; polar_pickups.cpp). */
class PolarWumpa : public Game::Entities::ActorSelf
{
public:
    /* Inline (PolarPlayer::CreateActor expands it); polar_pickups.cpp, which defines
     * POLAR_WUMPA_CONSTRUCTOR_OUT_OF_LINE, has the ROM's out-of-line copy
     * (CreatePolarWumpa, no caller). */
    PolarWumpa(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarWumpa(); // 1 DestroyPolarWumpa
    virtual void Update(); // 2 UpdatePolarWumpa
};

#ifndef POLAR_WUMPA_CONSTRUCTOR_OUT_OF_LINE
inline PolarWumpa::PolarWumpa(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
    : Game::Entities::ActorSelf(rec, x, y, z)
{
}
#endif

/* The crates' base (gPolarCrateVtable; src/game/vehicles/polar/polar_crates.cpp and
 * polar_crate.cpp). Its constructor picks one of 18 looks by the
 * crate's place on the course. A crate breaks (animation 0x12, Break)
 * when the player or the yeti touches it, and Update deletes it once
 * that animation has played. Its destructor is inline, as every crate
 * kind's expands it; actor_destructors.cpp has the out-of-line copy
 * (DestroyPolarCrate). */
class PolarCrate : public Game::Entities::ActorSelf
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

} // namespace Polar
} // namespace Vehicles
} // namespace Game

/* src/game/vehicles/actor_destructors.cpp: the C-linkage copy of PolarCrate's inline
 * destructor, which the g++-emitted vtables point at (cxx_symbols.txt). */
extern "C" void DestroyPolarCrate(Game::Vehicles::Polar::PolarCrate *self, u32 flags);

namespace Game
{
namespace Vehicles
{
namespace Polar
{

inline PolarCrate::~PolarCrate()
{
}

/* The crate kinds (polar_crates.cpp and polar_crate.cpp). Their
 * constructors are inline (PolarPlayer::CreateActor expands them) and also out of line
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
 * CollectedSpawns::Add. */
class PolarLifeCrate : public PolarCrate
{
public:
    struct actor_spawn *spawn; // 0x54

    PolarLifeCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z,
                   struct actor_spawn *spawn);
    virtual ~PolarLifeCrate(); // 1 DestroyPolarLifeCrate
    virtual void Update();     // 2 UpdatePolarLifeCrate
};

LAYOUT_ASSERT(vehicle_hpp, sizeof(PolarLifeCrate) == 0x58);

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
#include "game/vehicles/polar/polar_crate_ctors.hpp"
#undef POLAR_CRATE_CTOR
#endif

/* The hazards and objects (src/game/vehicles/polar/polar_objects.cpp,
 * polar_aku_aku.cpp and polar_course_objects.cpp). Their constructors are
 * out of line. */

/* An electric fence (gPolarElectricFenceVtable): shown once near enough,
 * it shocks the player on its wire and hurts it on its posts. */
class PolarElectricFence : public Game::Entities::ActorSelf
{
public:
    // CreatePolarElectricFence
    PolarElectricFence(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarElectricFence(); // 1 DestroyPolarElectricFence
    virtual void Update();         // 2 UpdatePolarElectricFence
};

/* A rock in the way (two parts, PolarPlayer::CreateActor kinds 13 and 14). */
class PolarObstacle : public Game::Entities::ActorSelf
{
public:
    // CreatePolarObstacle
    PolarObstacle(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarObstacle(); // 1 DestroyPolarObstacle
    virtual void Update();    // 2 UpdatePolarObstacle
};

/* A ramp that launches the player (state 1 once used). */
#define POLAR_LAUNCHER_STATE_READY 0   // launches the player, or is set off by the yeti
#define POLAR_LAUNCHER_STATE_EXPLODE 1 // used: deleted when the animation is done
class PolarLauncher : public Game::Entities::ActorSelf
{
public:
    // CreatePolarLauncher
    PolarLauncher(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarLauncher(); // 1 DestroyPolarLauncher
    virtual void Update();    // 2 UpdatePolarLauncher
};

/* Hops from spawn to spawn (Aim); knocked away (state 1) by the player or
 * the yeti. */
#define POLAR_PENGUIN_STATE_FLY 0          // flies from spawn to spawn
#define POLAR_PENGUIN_STATE_KNOCKED_AWAY 1 // flies off, harmless
/* Aim with no target left: the penguin drifts on at this Z speed (Q8, the
 * middle of gPolarPenguinSpeeds). */
#define POLAR_PENGUIN_DRIFT_SPEED_Z Q8_FRAC(0, 0x62)
class PolarPenguin : public Game::Entities::ActorSelf
{
public:
    s32 velX;       // 0x54
    s32 velY;       // 0x58
    s32 velZ;       // 0x5C
    s32 countdown;  // 0x60 - frames until the next retarget
    s32 nextTarget; // 0x64 - the spawn index it aims for next

    // CreatePolarPenguin
    PolarPenguin(const struct anim_table_record *rec, s32 x, s32 y, s32 z, struct spawn_arg *spawn);
    virtual ~PolarPenguin(); // 1 DestroyPolarPenguin
    virtual void Update();   // 2 UpdatePolarPenguin

    void Aim(s32 target); // AimPolarPenguin
};

LAYOUT_ASSERT(vehicle_hpp, sizeof(PolarPenguin) == 0x68);

/* An icicle: its look depends on its record and side, and it falls in
 * steps (state 0-2) as it comes near. Each step is the next animation,
 * and `state` counts them (`state + 1`). */
#define POLAR_ICICLE_STATE_HANGING 0 // until depth 0x5000
#define POLAR_ICICLE_STATE_DROP1 1   // until depth 0x5A00
#define POLAR_ICICLE_STATE_DROP2 2   // until depth 0x6400
#define POLAR_ICICLE_STATE_DROP3 3   // the last step
class PolarIcicle : public Game::Entities::ActorSelf
{
public:
    // CreatePolarIcicle
    PolarIcicle(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarIcicle(); // 1 DestroyPolarIcicle
    virtual void Update();  // 2 UpdatePolarIcicle
};

/* Aku Aku following the player (gPolarAkuAku; Spawn, polar_actor_factory.cpp): its look
 * is the mask level (gLevelState->maskLevel), state 1 is invincible
 * (gPolarAkuAkuInvincibleTimer), state 2 a lost mask. */
#define POLAR_AKU_AKU_STATE_FOLLOW 0     // hovers round behind the player
#define POLAR_AKU_AKU_STATE_INVINCIBLE 1 // sits on the player, in front
#define POLAR_AKU_AKU_STATE_LOSE_MASK 2  // the lost-mask animation, then Refresh
class PolarAkuAku : public Game::Entities::ActorSelf
{
public:
    // CreatePolarAkuAku
    PolarAkuAku(const struct anim_table_record *rec, s32 x, s32 y, s32 z, s32 level);
    virtual ~PolarAkuAku(); // 1 DestroyPolarAkuAku
    virtual void Update();  // 2 UpdatePolarAkuAku

    static PolarAkuAku *Spawn(s32 x, s32 y, s32 z, s32 level); // SpawnPolarAkuAku

    void Refresh(u8 lost);          // RefreshPolarAkuAku
    void Move(s32 x, s32 y, s32 z); // MovePolarAkuAku
    void ClearMask();               // ClearPolarAkuAkuMask
    s32 RemoveMask();               // RemovePolarAkuAkuMask
    s32 AddMask();                  // AddPolarAkuAkuMask
    void SetMask(s32 level);        // SetPolarMaskLevel
};

/* The finish line (two parts, PolarPlayer::CreateActor kinds 25 and 26), shown after
 * 5 frames. */
class PolarGoal : public Game::Entities::ActorSelf
{
public:
    // CreatePolarGoal
    PolarGoal(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarGoal();  // 1 DestroyPolarGoal
    virtual void Update(); // 2 UpdatePolarGoal
};

/* A boost pad: its look is its side of the course. */
class PolarBoostPad : public Game::Entities::ActorSelf
{
public:
    u8 once; // 0x54 - the boost's sound was played

    // CreatePolarBoostPad
    PolarBoostPad(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarBoostPad(); // 1 DestroyPolarBoostPad
    virtual void Update();    // 2 UpdatePolarBoostPad
};

LAYOUT_ASSERT(vehicle_hpp, sizeof(PolarBoostPad) == 0x58);

/* The checkpoint crate: animation 0 whole, 1 opened by the player, 2
 * already the checkpoint, 3 broken by the yeti. */
class PolarCheckpointCrate : public Game::Entities::ActorSelf
{
public:
    // CreatePolarCheckpointCrate
    PolarCheckpointCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~PolarCheckpointCrate(); // 1 DestroyPolarCheckpointCrate
    virtual void Update();           // 2 UpdatePolarCheckpointCrate
};

} // namespace Polar
} // namespace Vehicles
} // namespace Game

namespace Game
{
namespace Vehicles
{
namespace Jetpack
{

/* The jetpack levels (7 vtable slots: HpActor's; the balloon crates
 * add an 8th). */

/* The jetpack player (gJetpackPlayerVtable;
 * src/game/vehicles/jetpack/jetpack_player_update.cpp and jetpack_player.cpp;
 * jetpack_spawn.cpp's JetpackPlayer::Create): its hit points are HpActor's,
 * shown as a percentage (GetHp); the rest of its state is in the
 * gJetpack* globals (above), as the ROM has it. JetpackPlayer::Create
 * makes it gActorList, the actor list's root. */
/* JetpackPlayer's `state` (#843, docs/flags/actor_states_jetpack.md): the
 * stateFuncs index, named after its method. */
#define JETPACK_PLAYER_STATE_ENTER 0      // StateEnter: flies in, the input off
#define JETPACK_PLAYER_STATE_FLY 1        // StateFly: steered by the player
#define JETPACK_PLAYER_STATE_ROLL_LEFT 2  // StateRollLeft: the L barrel roll
#define JETPACK_PLAYER_STATE_ROLL_RIGHT 3 // StateRollRight: the R barrel roll
#define JETPACK_PLAYER_STATE_FALL 4       // StateFall: shot down (Damage)
#define JETPACK_PLAYER_STATE_FINISH 5     // StateFinish: the course's end (FinishRun)
#define JETPACK_PLAYER_STATE_BOOST 6      // StateBoost: through a ring (PassRing)
#define JETPACK_PLAYER_STATE_RESUME 7     // StateResume: from a checkpoint (the constructor)

/* The jetpack player's tuning (Q8, docs/fixed_point.md): shot down
 * (StateFall), gJetpackPlayerVelY grows by JETPACK_PLAYER_FALL_GRAVITY a
 * frame up to JETPACK_PLAYER_MAX_FALL_SPEED; after a barrel roll's
 * burst (StateRollLeft/Right), gJetpackPlayerVelX recovers toward 0 by
 * JETPACK_ROLL_RECOVER_STEP a frame; his shots (StateFly) start
 * JETPACK_PLAYER_SHOT_OFFSET_Z in front of him. */
#define JETPACK_PLAYER_FALL_GRAVITY   Q8_FRAC(0, 9)
#define JETPACK_PLAYER_MAX_FALL_SPEED Q8(1.25)
#define JETPACK_ROLL_RECOVER_STEP     Q8_FRAC(0, 0x2d)
#define JETPACK_PLAYER_SHOT_OFFSET_Z  Q8_FRAC(0, 10)

class JetpackPlayer : public Game::Entities::HpActor
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

    /* The jetpack and hovercraft categories' hooks (ActorCategory::vtables'
     * slots 9-12, #915): static, each runs on the player, gActorList; their
     * code is in the actor zone, next to PolarPlayer's (above). */
    static s32 IsTouchingPlayer(Game::Entities::ActorSelf *self); // JetpackIsTouchingPlayer
    static void ReachCourseEnd();                                 // JetpackReachCourseEnd
    static void ReloadPlayerTiles();                              // JetpackReloadPlayerTiles
    static s32 IsPlayerPauseLocked();                             // JetpackIsPauseLocked

    /* The jetpack and hovercraft categories' factory (jetpack_spawn.cpp,
     * #915): Create and SpawnActor are the categories' slots 0 and 1,
     * CreateActor the per-kind `new`. */
    static void Create(struct anim_table_record *table, s32 z); // CreateJetpackPlayer
    static Game::Entities::ActorSelf *SpawnActor(struct actor_spawn *rec, u8 alt,
                                                 s32 dz); // SpawnJetpackActor
    static Game::Entities::ActorSelf *CreateActor(u8 kind, s32 x, s32 y, s32 z,
                                                  struct actor_spawn *spawn); // CreateJetpackActor
};

LAYOUT_ASSERT(vehicle_hpp, sizeof(JetpackPlayer) == 0x58);
LAYOUT_ASSERT(vehicle_hpp, sizeof(JetpackPlayer) == 0x58);

} // namespace Jetpack
} // namespace Vehicles
} // namespace Game

inline Game::Vehicles::Jetpack::JetpackPlayer *Game::Entities::ActorSelf::AsJetpackPlayer()
{
    return (Game::Vehicles::Jetpack::JetpackPlayer *)this;
}

namespace Game
{
namespace Vehicles
{
namespace Jetpack
{

/* The checkpoint banner, drawn at a fixed screen position. */
class JetpackCheckpointText : public Game::Entities::HpActor
{
public:
    /* Inline: JetpackCheckpointText::Create expands it. */
    JetpackCheckpointText(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
        : Game::Entities::HpActor(rec, x, y, z, 1)
    {
    }
    virtual ~JetpackCheckpointText(); // 1 DestroyJetpackCheckpointText
    virtual void Update();            // 2 UpdateJetpackCheckpointText
    virtual void Draw();              // 3 DrawJetpackCheckpointText
    virtual u8 IsUnshootable();       // 5 IsJetpackCheckpointTextUnshootable

    static void Create(); // CreateJetpackCheckpointText
};

/* An explosion: it moves away (into the screen) by
 * JETPACK_EXPLOSION_SPEED_Z a frame (Q8) while it plays. */
#define JETPACK_EXPLOSION_SPEED_Z Q8_FRAC(0, 170)
class JetpackExplosion : public Game::Entities::HpActor
{
public:
    /* Inline: JetpackExplosion::Create expands it. */
    JetpackExplosion(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
        : Game::Entities::HpActor(rec, x, y, z, 1)
    {
    }
    virtual ~JetpackExplosion(); // 1 DestroyJetpackExplosion
    virtual void Update();       // 2 UpdateJetpackExplosion
    virtual u8 IsUnshootable();  // 5 IsJetpackExplosionUnshootable

    static void Create(s32 x, s32 y, s32 z); // CreateJetpackExplosion
};

/* The jetpack player's shot (src/game/vehicles/jetpack/jetpack_shot.cpp): it flies by
 * its speed, and hits the first shootable actor or the airship. */
class JetpackShot : public Game::Entities::HpActor
{
public:
    s32 velX; // 0x58
    s32 velY; // 0x5C

    // CreateJetpackShot
    JetpackShot(const struct anim_table_record *rec, s32 x, s32 y, s32 z, s32 velX, s32 velY);
    virtual ~JetpackShot();     // 1 DestroyJetpackShot
    virtual void Update();      // 2 UpdateJetpackShot
    virtual u8 IsUnshootable(); // 5 IsJetpackShotUnshootable

    static void Spawn(s32 x, s32 y, s32 z, s32 velX, s32 velY); // SpawnJetpackShot
};

LAYOUT_ASSERT(vehicle_hpp, sizeof(JetpackShot) == 0x60);

/* The plane (gJetpackPlaneVtable, src/game/vehicles/jetpack/jetpack_plane.cpp): it hops
 * from spawn point to spawn point (Aim, the GetActorSpawn* accessors) and
 * fires cannonballs at the player from its low pose; shot down, it falls
 * out of the sky. */
/* JetpackPlane's `state`: the stateFuncs index. */
#define JETPACK_PLANE_STATE_FLY 0         // StateFly: to the next spawn point
#define JETPACK_PLANE_STATE_FOLLOW 1      // StateFollow: eases to the player (Aim, after a slow hop chain)
#define JETPACK_PLANE_STATE_KNOCKED_OUT 2 // StateKnockedOut: shot down (Damage)
#define JETPACK_PLANE_STATE_FALL 3        // StateFall: out of the sky, gone below 0xE100

/* The plane's velocities and accelerations are Q4 of its Q8 position
 * (`x += Q4_TO_INT(velX)`: 4096ths of a pixel). gJetpackPlaneHopSpeeds'
 * hop speeds are twelfths of a pixel a frame (16/12 down to 1/12): a
 * plane flies its hops at JETPACK_PLANE_SLOW_HOP_SPEED (7/12 px a frame)
 * or slower ends its chain following the player (Aim); one whose first
 * hop is faster starts 142 px further back, at
 * JETPACK_PLANE_FAST_START_SPEED (10/12). Shot down, it falls speeding
 * up by JETPACK_PLANE_FALL_GRAVITY (10/256 px a frame per frame) up to
 * JETPACK_PLANE_MAX_FALL_SPEED (1.25 px a frame). Its cannonballs start
 * JETPACK_PLANE_GUN_OFFSET_Z (Q8) in front of it. Following the player
 * (StateFollow), its Q4 speed is the Q8 gap to him `>>
 * JETPACK_PLANE_FOLLOW_SHIFT`: it closes 1/256 of the gap a frame (a
 * gain, not a format). */
#define JETPACK_PLANE_SLOW_HOP_SPEED   (INT_TO_Q4(INT_TO_Q8(7)) / 12)
#define JETPACK_PLANE_FAST_START_SPEED (INT_TO_Q4(INT_TO_Q8(10)) / 12)
#define JETPACK_PLANE_FALL_GRAVITY     INT_TO_Q4(Q8_FRAC(0, 10))
#define JETPACK_PLANE_MAX_FALL_SPEED   INT_TO_Q4(Q8(1.25))
#define JETPACK_PLANE_GUN_OFFSET_Z     Q8_FRAC(0, 10)
#define JETPACK_PLANE_FOLLOW_SHIFT     4
class JetpackPlane : public Game::Entities::HpActor
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
    JetpackPlane(const struct anim_table_record *rec, s32 x, s32 y, s32 z, struct spawn_arg *spawn);
    virtual ~JetpackPlane();         // 1 DestroyJetpackPlane
    virtual void Update();           // 2 UpdateJetpackPlane
    virtual void Damage(s32 amount); // 4 DamageJetpackPlane
    virtual u8 IsUnshootable();      // 5 IsJetpackPlaneUnshootable

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

LAYOUT_ASSERT(vehicle_hpp, sizeof(JetpackPlane) == 0x80);

/* The bomber (gJetpackBomberVtable, src/game/vehicles/jetpack/jetpack_bomber.cpp): its
 * record's kind (4-9) picks how it moves around its home point; it
 * explodes on the player. */
/* JetpackBomber's `state`: the stateFuncs index. The constructor picks
 * 0-5 from the record's kind; 6 is the explosion. */
#define JETPACK_BOMBER_STATE_IDLE 0             // StateIdle
#define JETPACK_BOMBER_STATE_HOME 1             // StateHome
#define JETPACK_BOMBER_STATE_BOB_VERTICAL 2     // StateBobVertical
#define JETPACK_BOMBER_STATE_SWING_HORIZONTAL 3 // StateSwingHorizontal
#define JETPACK_BOMBER_STATE_CIRCLE 4           // StateCircle
#define JETPACK_BOMBER_STATE_DROP 5             // StateDrop
#define JETPACK_BOMBER_STATE_DYING 6            // StateDying: touched or shot down
/* StateDrop: the bomber comes at the player by this much a frame (Q8). */
#define JETPACK_BOMBER_DROP_SPEED_Z Q8_FRAC(0, 0x88)
class JetpackBomber : public Game::Entities::HpActor
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
    virtual u8 IsUnshootable();      // 5 IsJetpackBomberUnshootable

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

LAYOUT_ASSERT(vehicle_hpp, sizeof(JetpackBomber) == 0x64);

/* The cannonball the planes and the airship fire
 * (gJetpackCannonballVtable, src/game/vehicles/jetpack/jetpack_cannonball.cpp). */
class JetpackCannonball : public Game::Entities::HpActor
{
public:
    s32 velX; // 0x58
    s32 velY; // 0x5C

    // CreateJetpackCannonball
    JetpackCannonball(const struct anim_table_record *rec, s32 x, s32 y, s32 z, s32 velX, s32 velY);
    virtual ~JetpackCannonball(); // 1 DestroyJetpackCannonball
    virtual void Update();        // 2 UpdateJetpackCannonball
    virtual u8 IsUnshootable();   // 5 IsJetpackCannonballUnshootable

    static void Spawn(s32 x, s32 y, s32 z, s32 velX, s32 velY); // SpawnJetpackCannonball
};

LAYOUT_ASSERT(vehicle_hpp, sizeof(JetpackCannonball) == 0x60);

class JetpackBalloonCrate;

/* JetpackBalloon's `state`: the stateFuncs index. */
#define JETPACK_BALLOON_STATE_ATTACHED 0   // StateAttached: the crate moves it
#define JETPACK_BALLOON_STATE_FLOAT_AWAY 1 // StateFloatAway: released, gone above -0xE100
#define JETPACK_BALLOON_STATE_POP 2        // StatePop: shot; gone when the animation is done

/* Released (StateFloatAway), the balloon rises speeding up by
 * JETPACK_BALLOON_RISE_ACCEL a frame up to JETPACK_BALLOON_MAX_RISE_SPEED
 * (velY negated: up). A crate hangs JETPACK_BALLOON_HANG_HEIGHT below
 * the balloon it spawns (JetpackBalloonCrate::Hang). Q8. */
#define JETPACK_BALLOON_RISE_ACCEL     Q8_FRAC(0, 6)
#define JETPACK_BALLOON_MAX_RISE_SPEED Q8_FRAC(1, 0x2c)
#define JETPACK_BALLOON_HANG_HEIGHT    Q8_FRAC(61, 0xb6)

/* The balloon a crate hangs from (gJetpackBalloonVtable,
 * src/game/vehicles/jetpack/jetpack_balloon.cpp): the crate moves it (Move) until it is
 * released (Release) and floats away, or is shot and pops. */
class JetpackBalloon : public Game::Entities::HpActor
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
    virtual u8 IsUnshootable();      // 5 IsJetpackBalloonUnshootable

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

    static JetpackBalloon *Spawn(u8 kind, s32 x, s32 y, s32 z,
                                 JetpackBalloonCrate *crate); // SpawnJetpackBalloon
};

LAYOUT_ASSERT(vehicle_hpp, sizeof(JetpackBalloon) == 0x64);

/* The crates hanging from balloons (gJetpackBalloonCrateVtable,
 * src/game/vehicles/jetpack/jetpack_crates.cpp): each hangs from a balloon of its own
 * (JetpackBalloon::Spawn), swaying around its spawn point, until the
 * balloon is shot (Break: it falls) or the crate is (Damage: it breaks
 * and lets the balloon go). The three kinds built on it pay out when
 * broken or touched; their destructors are g++'s implicit ones
 * (DestroyJetpackHealthCrate, DestroyJetpackTimeCrate and
 * DestroyJetpackQuestionCrate, actor_destructors.cpp). */
/* JetpackBalloonCrate's `state` (and its three kinds'): the stateFuncs
 * index. */
#define JETPACK_BALLOON_CRATE_STATE_HANG 0      // StateHang: from its balloon
#define JETPACK_BALLOON_CRATE_STATE_FALL 1      // StateFall: its balloon popped (Break)
#define JETPACK_BALLOON_CRATE_STATE_DESTROYED 2 // StateDestroyed: broken, pays out
class JetpackBalloonCrate : public Game::Entities::HpActor
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
    virtual u8 IsUnshootable();      // 5 IsJetpackBalloonCrateUnshootable
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

LAYOUT_ASSERT(vehicle_hpp, sizeof(JetpackBalloonCrate) == 0x70);

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
    struct actor_spawn *spawn; // 0x70 - the level spawn record, for CollectedSpawns::Add

    // CreateJetpackQuestionCrate
    JetpackQuestionCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z,
                         struct actor_spawn *spawn);
    virtual void Update();           // 2 UpdateJetpackQuestionCrate
    virtual void Damage(s32 amount); // 4 DamageJetpackQuestionCrate
};

LAYOUT_ASSERT(vehicle_hpp, sizeof(JetpackQuestionCrate) == 0x74);

/* A nitro crate on a parachute (gJetpackParachuteNitroVtable,
 * src/game/vehicles/jetpack/jetpack_crates.cpp): it rises to `limitY`, and explodes on
 * the player or when shot. */
class JetpackParachuteNitro : public Game::Entities::HpActor
{
public:
    u8 dead;    // 0x58 - exploded: no longer shootable
    s32 limitY; // 0x5C

    // CreateJetpackParachuteNitro
    JetpackParachuteNitro(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~JetpackParachuteNitro(); // 1 DestroyJetpackParachuteNitro
    virtual void Update();            // 2 UpdateJetpackParachuteNitro
    virtual void Damage(s32 amount);  // 4 DamageJetpackParachuteNitro
    virtual u8 IsUnshootable();       // 5 IsJetpackParachuteNitroUnshootable
};

LAYOUT_ASSERT(vehicle_hpp, sizeof(JetpackParachuteNitro) == 0x60);

/* A rocket (gJetpackRocketVtable, src/game/vehicles/jetpack/jetpack_rocket.cpp): it
 * swings around `originX` while it comes down by `stepY` to `limitY`,
 * then explodes (Launch); it hurts the player once on contact. */
class JetpackRocket : public Game::Entities::HpActor
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
    virtual u8 IsUnshootable();      // 5 IsJetpackRocketUnshootable

    void Launch(); // LaunchJetpackRocket
};

LAYOUT_ASSERT(vehicle_hpp, sizeof(JetpackRocket) == 0x68);

/* A jetpack ring (gJetpackRingVtable). Its constructor and slot 5 are in
 * src/game/vehicles/jetpack/jetpack_collected_wumpa.cpp (part 11h), its Update in
 * jetpack_rocket.cpp (part 11g). */
class JetpackRing : public Game::Entities::HpActor
{
public:
    u8 cued; // 0x58 - set once UpdateJetpackRing has played its cue

    // CreateJetpackRing
    JetpackRing(const struct anim_table_record *rec, s32 x, s32 y, s32 z);
    virtual ~JetpackRing();     // 1 DestroyJetpackRing
    virtual void Update();      // 2 UpdateJetpackRing
    virtual u8 IsUnshootable(); // 5 IsJetpackRingUnshootable
};

LAYOUT_ASSERT(vehicle_hpp, sizeof(JetpackRing) == 0x5C);

/* A collected wumpa of the jetpack levels (gJetpackCollectedWumpaVtable,
 * src/game/vehicles/jetpack/jetpack_collected_wumpa.cpp, part 11h;
 * PolarCollectedWumpa's twin): it
 * flies from where it was collected to the HUD's wumpa counter, and its
 * destructor adds the fruit it carries. */
class JetpackCollectedWumpa : public Game::Entities::HpActor
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
    virtual u8 IsUnshootable();       // 5 IsJetpackCollectedWumpaUnshootable

    static void Spawn(s32 x, s32 y, s32 reward); // SpawnJetpackCollectedWumpa
};

LAYOUT_ASSERT(vehicle_hpp, sizeof(JetpackCollectedWumpa) == 0x64);

} // namespace Jetpack
} // namespace Vehicles
} // namespace Game

#endif /* GUARD_VEHICLE_HPP */
