#ifndef GUARD_CAMERA_HPP
#define GUARD_CAMERA_HPP

/* The camera (gCamera, globals.h; engine/src/game/level/camera.cpp, #761): a Q8
 * position, a Q8 look-ahead offset, the followed sprite and the mode.
 * PlayRoom builds it with `new Camera` and frees it with `delete
 * gCamera`: with no constructor, destructor or vtable those are the
 * plain OperatorNew(0x18) and OperatorDelete calls (no null test,
 * docs/cplusplus.md). cxx_symbols.txt maps the methods onto their C
 * names. play_room.c once called it `struct gl_scratch`,
 * action_ctrl_event.c `struct follow_state` and level_select.c `struct
 * follow_owner`; it was level.h's `struct camera` until #761.
 *
 * `#pragma interface`: no vtable to emit; it keeps g++ from emitting
 * out-of-line copies of inline methods (docs/cplusplus.md). */
#pragma interface

extern "C" {
#include "core.h"
#include "engine/util/math_util.h"
}

namespace Game
{
namespace Entities
{
class Sprite; // sprite_obj.hpp
} // namespace Entities

namespace Level
{

/* Camera's `mode` (#843, docs/flags/camera.md): how Update eases the
 * look-ahead. `new Camera` leaves it unset; RunRoom sets FACING. */
#define CAMERA_MODE_FACING 1      // StepFacing: ahead of the target's facing (on foot, the hover ride)
#define CAMERA_MODE_DIRECTIONAL 2 // StepDirectional: towards the player's `dir` bits (underwater)
#define CAMERA_MODE_STILL 3       // no easing: the camera stays put (a plain hit kills the player)

/* The camera's framing (#979, docs/screen.md). Update and Snap hand the
 * position minus the screen's centre to the level's scroll, so the
 * target (plus the look-ahead) ends up centred. StepDirectional's
 * look-ahead reaches a sixth of the screen each way (0x2800 x 0x1AAA in
 * Q8 on the GBA), so it scales with the screen; StepFacing's horizontal
 * look-ahead and fixed vertical offset are distances in the level and
 * stay. All Q8. */
#define CAMERA_LOOK_AHEAD_X        (INT_TO_Q8(DISPLAY_WIDTH) / 6)  // StepDirectional, x: 0x2800
#define CAMERA_LOOK_AHEAD_Y        (INT_TO_Q8(DISPLAY_HEIGHT) / 6) // StepDirectional, y: 0x1AAA
#define CAMERA_LOOK_AHEAD_STEP     0x100                           // the look-ahead's step per frame
#define CAMERA_FACING_LOOK_AHEAD_X 0x1276                          // StepFacing: ahead of the target
#define CAMERA_FACING_OFFSET_Y     (-0x1000)                       // StepFacing: above the target

class Camera
{
public:
    s32 x;             // 0x00 - Q8
    s32 y;             // 0x04 - Q8
    screen_dx_q8_t vx; // 0x08 - Q8 look-ahead
    screen_dy_q8_t vy; // 0x0C - Q8 look-ahead
    Game::Entities::Sprite
        *target; // 0x10 - gPlayer, or the level select's camera lead (CameraLead)
    s32 mode;    // 0x14 - CAMERA_MODE_*

    void StepDirectional(); // StepCameraDirectional
    void StepFacing();      // StepCameraFacing
    void Snap();            // SnapCamera
    void Update();          // UpdateCamera
};

LAYOUT_ASSERT(camera_hpp, sizeof(Camera) == 0x18);

} // namespace Level
} // namespace Game

#endif // GUARD_CAMERA_HPP
