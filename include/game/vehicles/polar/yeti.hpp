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
 * get their addresses from games/xs/sym_iwram.txt under those names, and the ROM
 * tables are defined in src/.../data/*.cpp as the members (`const
 * struct YetiCharge Yeti::chargeParams[6] = ...`). The actor category table
 * (src/engine/actor/data/actor_categories.cpp) points at Create, Update,
 * UpdateBg2, Destroy and LoadGraphics.
 *
 * Yeti is in the namespace Game::Vehicles::Polar (#1033, vehicle.hpp):
 * `_Q44Game8Vehicles5Polar4Yeti.chargeParams` & co. Its record type
 * YetiCharge, which no mangled name takes, stays global.
 *
 * `#pragma interface`: no vtable to emit; it keeps g++ from emitting
 * out-of-line copies of inline methods (docs/cplusplus.md). */
#pragma interface

extern "C" {
#include "core.h"
#include "engine/actor/actor_anim.h"
}

namespace Game
{
namespace Entities
{
class ActorSelf; // actor_self.hpp
class AnimPart;
} // namespace Entities
} // namespace Game

/* Yeti::state (#843, docs/flags/boss_states_vehicle.md): the stateFuncs
 * index, named after its method. */
#define YETI_STATE_CHASE 0  // StateChase: Create's; StateCharge's end
#define YETI_STATE_CHARGE 1 // StateCharge: picked at random by StateChase (chargeParams)
#define YETI_STATE_CAUGHT 2 // StateCaught: Update, its catch box on the player
#define YETI_STATE_STOP 3   // StateStop: Stop, the run is over (finish, knocked off, shocked)

/* The yeti's BG2 maps (LoadGraphics, BuildBg2Map; tiles, not fixed
 * point): YETI_BG2_MAP_TILES a side (the 128x128 affine BG), the
 * picture's 10x10 tiles numbered in order from YETI_PICTURE_TILE_FIRST
 * to YETI_PICTURE_TILE_LAST on both axes, the blank tile 0xFF around. */
#define YETI_BG2_MAP_TILES      16
#define YETI_PICTURE_TILE_FIRST 3
#define YETI_PICTURE_TILE_LAST  12

/* A record of Yeti::chargeParams (gYetiChargeParams), picked by
 * Yeti::paramsIndex: how far StateCharge moves the yeti each frame, and
 * StateChase's two rolls (out of 0x100) for starting a charge. */
struct YetiCharge {
    s32 step;       // 0x00 - StateCharge adds it to position (Q8)
    s32 fastChance; // 0x04 - the roll while CellAnim::GetSpeed() > 0x24
    s32 slowChance; // 0x08 - the roll otherwise
};

namespace Game
{
namespace Vehicles
{
namespace Polar
{

class Yeti
{
public:
    static void Create(s32 level); // CreateYeti (category slot 2)
    static void Update();          // UpdateYeti (slot 3)
    static void UpdateBg2();       // UpdateYetiBg2 (slot 4)
    static void Destroy();         // DestroyYeti (slot 5)
    static void LoadGraphics();    // LoadYetiGraphics (slot 6)
    static void Stop();            // StopYeti: the polar player's finish
    static u8
    IsTouching(Game::Entities::ActorSelf *self); // IsTouchingYeti: the polar objects' test

private:
    typedef void (*StateFunc)();

    static void StateChase();                  // YetiStateChase (state 0)
    static void StateCharge();                 // YetiStateCharge (state 1)
    static void StateCaught();                 // YetiStateCaught (state 2)
    static void StateStop();                   // YetiStateStop (state 3, jetpack_spawn.cpp)
    static void UpdatePalette();               // UpdateYetiPalette
    static void BuildBg2Map(u8 *dst, u8 seed); // BuildYetiBg2Map: UNUSED

    /* IWRAM (games/xs/sym_iwram.txt) */
    static Game::Entities::AnimPart *anim; // gYeti - the 0x1C-byte animation
    static u8 bg2Page;                     // gYetiBg2Page
    static u8 bg2PageFlip;                 // gYetiBg2PageFlip
    static s32 x;                          // gYetiX - Q8
    static s32 position;                   // gYetiPosition - Q8
    static s32 distance;                   // gYetiDistance - Q8, to the player
    static s32 state;                      // gYetiState - indexes stateFuncs
    static s32 paramsIndex;                // gYetiParamsIndex - indexes chargeParams

    /* ROM (src/.../data) */
    static const StateFunc stateFuncs[4];               // gYetiStateFuncs
    static const struct YetiCharge chargeParams[6];     // gYetiChargeParams
    static const struct anim_box box;                   // gYetiBox - IsTouching's
    static const struct anim_box catchBox;              // gYetiCatchBox - Update's
    static const u16 palette[16];                       // gYetiPalette
    static const u8 *const frames[123];                 // gYetiFrames
    static const struct anim_frame_record keyframes[4]; // gYetiKeyframes
};

} // namespace Polar
} // namespace Vehicles
} // namespace Game

#endif // GUARD_YETI_HPP
