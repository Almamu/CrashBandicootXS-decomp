#ifndef GUARD_YETI_HPP
#define GUARD_YETI_HPP

/* The yeti that chases the polar player (src/vehicle/polar/yeti*.cpp,
 * #772): an all-static class. The ROM keeps its state in separate
 * globals (IWRAM 0x030014BC-0x030014D7) and loads each one's own address,
 * even for two adjacent bytes in one function; one object's fields would
 * be reached from one base by offset instead. Static data members compile
 * like those globals and static member functions like plain functions, so
 * the class is byte-identical to the C.
 *
 * cxx_symbols.txt maps every member onto its C name: the IWRAM variables
 * get their addresses from sym_iwram.txt under those names, and the ROM
 * tables but stateFuncs (src/data/actor_state_fn_17a840.cpp) are defined
 * in C data files under them. The actor category table
 * (src/data/actor_category_175558.c) names Create, Update, UpdateBg2,
 * Destroy and LoadGraphics by their C names (vehicle.h).
 *
 * `#pragma interface`: no vtable to emit; it keeps g++ from emitting
 * out-of-line copies of inline methods (docs/cplusplus.md). */
#pragma interface

extern "C" {
#include "core.h"
#include "actor_self.h"
}

class ActorSelf;
class AnimPart;

class Yeti
{
public:
    static void Create(s32 level);         // CreateYeti (category slot 2)
    static void Update();                  // UpdateYeti (slot 3)
    static void UpdateBg2();               // UpdateYetiBg2 (slot 4)
    static void Destroy();                 // DestroyYeti (slot 5)
    static void LoadGraphics();            // LoadYetiGraphics (slot 6)
    static void Stop();                    // StopYeti: the polar player's finish
    static u8 IsTouching(ActorSelf *self); // IsTouchingYeti: the polar objects' test

private:
    typedef void (*StateFunc)();

    static void StateChase();                  // YetiStateChase (state 0)
    static void StateCharge();                 // YetiStateCharge (state 1)
    static void StateCaught();                 // YetiStateCaught (state 2)
    static void StateStop();                   // YetiStateStop (state 3, jetpack_spawn.cpp)
    static void UpdatePalette();               // UpdateYetiPalette
    static void BuildBg2Map(u8 *dst, u8 seed); // BuildYetiBg2Map: UNUSED

    /* IWRAM (sym_iwram.txt) */
    static AnimPart *anim;  // gYeti - the 0x1C-byte animation
    static u8 bg2Page;      // gYetiBg2Page
    static u8 bg2PageFlip;  // gYetiBg2PageFlip
    static s32 x;           // gYetiX - Q8
    static s32 position;    // gYetiPosition - Q8
    static s32 distance;    // gYetiDistance - Q8, to the player
    static s32 state;       // gYetiState - indexes stateFuncs
    static s32 paramsIndex; // gYetiParamsIndex - indexes chargeParams

    /* ROM (src/data) */
    static const StateFunc stateFuncs[4];               // gYetiStateFuncs
    static const s32 chargeParams[6][3];                // gYetiChargeParams
    static const struct anim_box box;                   // gYetiBox - IsTouching's
    static const struct anim_box catchBox;              // gYetiCatchBox - Update's
    static const u16 palette[16];                       // gYetiPalette
    static const u8 *const frames[123];                 // gYetiFrames
    static const struct anim_frame_record keyframes[4]; // gYetiKeyframes
};

#endif // GUARD_YETI_HPP
