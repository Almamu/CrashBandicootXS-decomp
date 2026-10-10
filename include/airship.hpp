#ifndef GUARD_AIRSHIP_HPP
#define GUARD_AIRSHIP_HPP

/* N. Gin's airship, the jetpack levels' boss (#772, docs/cplusplus.md "The
 * airship as an all-static class"): src/vehicle/jetpack/airship*.cpp. A
 * class whose members are all `static`: the ROM loads a separate address
 * for each of its variables (even two adjacent bytes in one function), so
 * they were separate globals, not one object's fields, and static data
 * members compile exactly like them (and static member functions like
 * plain functions).
 *
 * The data members are only declared: cxx_symbols.txt (block "#772
 * Airship") maps each mangled name to its C name (`_7Airship.state` to
 * gAirshipState, ...), whose address is in sym_iwram.txt; the functions
 * keep their C names the same way (`Update__7Airship` is UpdateAirship).
 * The picture's animation is a bare AnimPart (`anim`, gAirship; `new
 * AnimPart` in Create); its fireballs are AirshipFireball
 * (boss_actors.hpp).
 *
 * The ROM tables only it reads are static data members too, defined in
 * their src/data/*.cpp files (`const struct airship_attack
 * Airship::attacks[6] = ...`): `_7Airship.attacks` is renamed to
 * gAirshipAttacks. The actor category table
 * (src/data/actor_category_175558.cpp) points at Create, Update,
 * UpdateBg2, Destroy and LoadGraphics. */

#include "actor_self.hpp"

extern "C" {
#include "bosses.h"
}

class Airship
{
public:
    /* The actor category table's boss slots (type 1, #765). */
    static void Create(s32 lvl); // CreateAirship
    static void Update();        // UpdateAirship
    static void UpdateBg2();     // UpdateAirshipBg2
    static void Destroy();       // DestroyAirship
    static void LoadGraphics();  // LoadAirshipGraphics

    static void Spawn(s32 kind, s32 sx, s32 sy, s32 sz); // SpawnAirship (SpawnJetpackActor)
    static void Damage(s32 delta);                       // DamageAirship (JetpackShot)
    static u8 IsTouching(ActorSelf *self);               // IsTouchingAirship (JetpackShot)
    static s32 GetHpPercent();                           // GetAirshipHpPercent (the HUD)

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
        AnimPart *a;

        state = st;
        stateTimer = 0;
        a = anim;
        a->animIndex = idx;
        a->animTimer = a->anims[idx].duration;
        a->animDone = 0;
        if (a->GetAnimFrameBaseOffset() >= a->anims[a->animIndex].loopThreshold)
            a->animTime = 0;
    }

    /* IWRAM 0x03001520-0x03001580 (sym_iwram.txt) */
    static s32 bg2Page;     // gAirshipBg2Page
    static u8 bg2PageFlip;  // gAirshipBg2PageFlip: "apply now" latch for UpdateBg2
    static s32 mapCols;     // gAirshipMapCols
    static s32 mapRows;     // gAirshipMapRows
    static s32 mapTileBase; // gAirshipMapTileBase
    static AnimPart *anim;  // gAirship
    static s32 state;       // gAirshipState
    static s32 stateTimer;  // gAirshipStateTimer
    static s32 x;           // gAirshipX
    static s32 y;           // gAirshipY
    static s32 z;           // gAirshipZ
    static s32 screenX;     // gAirshipScreenX
    static s32 screenY;     // gAirshipScreenY
    static s32 distance;    // gAirshipDistance
    static s32 velX;        // gAirshipVelX
    static s32 velY;        // gAirshipVelY
    static s32 velZ;        // gAirshipVelZ
    static s32 level;       // gAirshipLevel: Create's
    static const struct airship_attack *attack; // gAirshipAttack: Spawn's
    static s32 hp;                              // gAirshipHp
    static s32 fireTimer;                       // gAirshipFireTimer
    static s32 volleyCount;                     // gAirshipVolleyCount
    static s32 hitFlashTimer;                   // gAirshipHitFlashTimer
    static s32 checkpointCount;                 // gAirshipCheckpointCount
    /* The picture's row addresses, as the AnimPart's u32 frame offsets
     * (ConvertTiles fills them; a u32 store may alias its heights[]). */
    static u32 mapFrames[]; // gAirshipMapFrames

    /* The ROM tables (src/data) */
    static void (*const stateFuncs[6])();          // gAirshipStateFuncs (actor_state_17c3fc.cpp)
    static const struct airship_attack attacks[6]; // gAirshipAttacks (weapon_kind_17c2d0.cpp)
    static const u16 hitFlashPalettes[3][16];      // gAirshipHitFlashPalettes
    static const struct anim_box box;              // gAirshipBox
    static const struct anim_frame_record keyframes[2]; // gAirshipKeyframes
    static const u16 palette[256];                      // gAirshipPalette (boss_pictures_167ad4.c)
    static const struct airship_picture picture;        // gAirshipPicture
};

#endif /* !GUARD_AIRSHIP_HPP */
