#ifndef GUARD_AIRSHIP_HPP
#define GUARD_AIRSHIP_HPP

/* N. Gin's airship, the jetpack levels' boss (#772, docs/cplusplus.md "The
 * airship as an all-static class"): engine/src/vehicle/jetpack/airship*.cpp. A
 * class whose members are all `static`: the ROM loads a separate address
 * for each of its variables (even two adjacent bytes in one function), so
 * they were separate globals, not one object's fields, and static data
 * members compile exactly like them (and static member functions like
 * plain functions).
 *
 * The data members are only declared: cxx_symbols.txt (block "#772
 * Airship") maps each mangled name to its C name
 * (`_Q44Game8Vehicles7Jetpack7Airship.state` to gAirshipState, ...), whose
 * address is in games/xs/sym_iwram.txt; the functions keep their C names the same
 * way (`Update__Q44Game8Vehicles7Jetpack7Airship` is UpdateAirship).
 * The picture's animation is a bare AnimPart (`anim`, gAirship; `new
 * AnimPart` in Create); its fireballs are AirshipFireball
 * (boss_actors.hpp).
 *
 * The ROM tables only it reads are static data members too, defined in
 * their engine/src/.../data/*.cpp files (`const struct airship_attack
 * Airship::attacks[6] = ...`): `_Q44Game8Vehicles7Jetpack7Airship.attacks`
 * is renamed to gAirshipAttacks. The actor category table
 * (engine/src/engine/actor/data/actor_categories.cpp) points at Create, Update,
 * UpdateBg2, Destroy and LoadGraphics.
 *
 * Airship is in the namespace Game::Vehicles::Jetpack (#1033,
 * vehicle.hpp), with its fireball. */

#include "game/entities/actor_self.hpp"

extern "C" {
#include "game/bosses/bosses.h"
}

/* Airship::state (#843, docs/flags/boss_states_vehicle.md): the
 * stateFuncs index, named after its method. */
#define AIRSHIP_STATE_INACTIVE 0  // StateInactive: Create's, until Spawn; StateFall's end
#define AIRSHIP_STATE_APPROACH 1  // StateApproach: Spawn starts the fight
#define AIRSHIP_STATE_FIREBALLS 2 // StateFireballs: close (0x81FF), or the cannon past 0x4300
#define AIRSHIP_STATE_CANNON 3    // StateCannon: the fireballs closer than 0x3200
#define AIRSHIP_STATE_EXPLODE 4   // StateExplode: no hit points left (Damage)
#define AIRSHIP_STATE_FALL 5      // StateFall: the explosion's end (stateTimer 0xAA)

/* Airship::velZ, the Z speed (Q8 px a frame, docs/fixed_point.md):
 * Spawn's for the approach; the fireballs state ramps it by
 * AIRSHIP_SPEED_Z_STEP a frame to about AIRSHIP_FIREBALLS_SPEED_Z, the
 * cannon state up to just past AIRSHIP_CANNON_SPEED_Z; Damage sets
 * AIRSHIP_EXPLODE_SPEED_Z for the explosion, StateExplode
 * AIRSHIP_FALL_SPEED_Z for the fall. */
#define AIRSHIP_APPROACH_SPEED_Z  Q8_FRAC(0, 0x66)
#define AIRSHIP_SPEED_Z_STEP      Q8_FRAC(0, 1)
#define AIRSHIP_FIREBALLS_SPEED_Z Q8_FRAC(0, 0x98)
#define AIRSHIP_CANNON_SPEED_Z    Q8_FRAC(0, 0xb2)
#define AIRSHIP_EXPLODE_SPEED_Z   Q8_FRAC(0, 0xaa)
#define AIRSHIP_FALL_SPEED_Z      Q8_FRAC(0, 0x9d)

/* The fall (StateFall): velY grows by AIRSHIP_FALL_GRAVITY a frame up to
 * AIRSHIP_MAX_FALL_SPEED. */
#define AIRSHIP_FALL_GRAVITY   Q8_FRAC(0, 7)
#define AIRSHIP_MAX_FALL_SPEED Q8(1.25)

/* Steer: within 45 px of the player, the X and Y speeds change by
 * AIRSHIP_STEER_ACCEL_X/Y a frame toward him; near the screen's edges
 * they are pushed back by AIRSHIP_EDGE_PUSH_X/Y; then they are clamped
 * to AIRSHIP_MAX_SPEED_X/Y either way. */
#define AIRSHIP_STEER_ACCEL_X Q8_FRAC(0, 3)
#define AIRSHIP_STEER_ACCEL_Y Q8_FRAC(0, 2)
#define AIRSHIP_EDGE_PUSH_X   Q8_FRAC(0, 6)
#define AIRSHIP_EDGE_PUSH_Y   Q8_FRAC(0, 3)
#define AIRSHIP_MAX_SPEED_X   Q8(1.5)
#define AIRSHIP_MAX_SPEED_Y   INT_TO_Q8(1)

/* Where the fireballs (StateFireballs) and the cannonballs (StateCannon)
 * start, and the cannon aims from: (x - AIRSHIP_GUN_OFFSET_X,
 * y + AIRSHIP_GUN_OFFSET_Y, z - AIRSHIP_GUN_OFFSET_Z), Q8. */
#define AIRSHIP_GUN_OFFSET_X Q8_FRAC(12, 0xdb)
#define AIRSHIP_GUN_OFFSET_Y Q8_FRAC(81, 0x6d)
#define AIRSHIP_GUN_OFFSET_Z Q8_FRAC(0, 10)

namespace Game
{
namespace Vehicles
{
namespace Jetpack
{

class Airship
{
public:
    /* The actor category table's boss slots (type 1, #765). */
    static void Create(s32 lvl); // CreateAirship
    static void Update();        // UpdateAirship
    static void UpdateBg2();     // UpdateAirshipBg2
    static void Destroy();       // DestroyAirship
    static void LoadGraphics();  // LoadAirshipGraphics

    static void Spawn(s32 kind, s32 sx, s32 sy, s32 sz); // SpawnAirship (JetpackPlayer::SpawnActor)
    static void Damage(s32 delta);                       // DamageAirship (JetpackShot)
    static u8 IsTouching(Game::Entities::ActorSelf *self); // IsTouchingAirship (JetpackShot)
    static s32 GetHpPercent();                             // GetAirshipHpPercent (the HUD)

private:
    /* The states, indexed by `state` (stateFuncs). */
    static void StateInactive();  // 0 AirshipStateInactive
    static void StateApproach();  // 1 AirshipStateApproach
    static void StateFireballs(); // 2 AirshipStateFireballs
    static void StateCannon();    // 3 AirshipStateCannon
    static void StateExplode();   // 4 AirshipStateExplode
    static void StateFall();      // 5 AirshipStateFall

    static void Steer();            // SteerAirship
    static void DrawMap(u16 *src);  // DrawAirshipMap
    static void ConvertTiles();     // ConvertAirshipTiles
    static void UpdateFlashColor(); // UpdateAirshipFlashColor
    static void AnimatePalette();   // AnimateAirshipPalette
    static void nullsub_30();       // UNUSED, after Destroy (airship_graphics.cpp)

    /* State `st` and animation `idx`, keeping the current frame unless
     * it is past the new animation's end. */
    static void SetState(s32 st, s32 idx)
    {
        Game::Entities::AnimPart *a;

        state = st;
        stateTimer = 0;
        a = anim;
        a->animIndex = idx;
        a->animTimer = a->anims[idx].duration;
        a->animDone = 0;
        if (a->GetAnimFrameBaseOffset() >= a->anims[a->animIndex].loopThreshold)
            a->animTime = 0;
    }

    /* IWRAM 0x03001520-0x03001580 (games/xs/sym_iwram.txt) */
    static s32 bg2Page;                    // gAirshipBg2Page
    static u8 bg2PageFlip;                 // gAirshipBg2PageFlip: "apply now" latch for UpdateBg2
    static s32 mapCols;                    // gAirshipMapCols
    static s32 mapRows;                    // gAirshipMapRows
    static s32 mapTileBase;                // gAirshipMapTileBase
    static Game::Entities::AnimPart *anim; // gAirship
    static s32 state;                      // gAirshipState
    static u32 stateTimer;                 // gAirshipStateTimer
    static s32 x;                          // gAirshipX
    static s32 y;                          // gAirshipY
    static s32 z;                          // gAirshipZ
    static s32 screenX;                    // gAirshipScreenX
    static s32 screenY;                    // gAirshipScreenY
    static s32 distance;                   // gAirshipDistance
    static s32 velX;                       // gAirshipVelX
    static s32 velY;                       // gAirshipVelY
    static s32 velZ;                       // gAirshipVelZ
    static s32 level;                      // gAirshipLevel: Create's
    static const struct airship_attack *attack; // gAirshipAttack: Spawn's
    static s32 hp;                              // gAirshipHp
    static s32 fireTimer;                       // gAirshipFireTimer
    static s32 volleyCount;                     // gAirshipVolleyCount
    static s32 hitFlashTimer;                   // gAirshipHitFlashTimer
    static s32 checkpointCount;                 // gAirshipCheckpointCount
    /* The picture's row addresses, as the AnimPart's u32 frame offsets
     * (ConvertTiles fills them; a u32 store may alias its heights[]). */
    static uintptr_t mapFrames[]; // gAirshipMapFrames

    /* The ROM tables (engine/src/.../data) */
    static void (*const stateFuncs[6])();          // gAirshipStateFuncs (airship_state_table.cpp)
    static const struct airship_attack attacks[6]; // gAirshipAttacks (airship_attacks.cpp)
    static const u16 hitFlashPalettes[3][16];      // gAirshipHitFlashPalettes
    static const struct anim_box box;              // gAirshipBox
    static const struct anim_frame_record keyframes[2]; // gAirshipKeyframes
    static const u16 palette[256];                      // gAirshipPalette (boss_pictures.cpp)
    static const struct airship_picture picture;        // gAirshipPicture
};

} // namespace Jetpack
} // namespace Vehicles
} // namespace Game

#endif /* !GUARD_AIRSHIP_HPP */
