#ifndef GUARD_CAMERA_HPP
#define GUARD_CAMERA_HPP

/* The camera (gCamera, globals.h; src/level/camera.cpp, #761): a Q8
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
}

class Sprite;

/* Camera's `mode` (#843, docs/flags/camera.md): how Update eases the
 * look-ahead. `new Camera` leaves it unset; RunRoom sets FACING. */
#define CAMERA_MODE_FACING 1      // StepFacing: ahead of the target's facing (on foot, the hover ride)
#define CAMERA_MODE_DIRECTIONAL 2 // StepDirectional: towards the player's `dir` bits (underwater)
#define CAMERA_MODE_STILL 3       // no easing: the camera stays put (a plain hit kills the player)

class Camera
{
public:
    s32 x;          // 0x00 - Q8
    s32 y;          // 0x04 - Q8
    s32 vx;         // 0x08 - Q8 look-ahead
    s32 vy;         // 0x0C - Q8 look-ahead
    Sprite *target; // 0x10 - gPlayer, or the level select's camera lead (CameraLead)
    s32 mode;       // 0x14 - CAMERA_MODE_*

    void StepDirectional(); // StepCameraDirectional
    void StepFacing();      // StepCameraFacing
    void Snap();            // SnapCamera
    void Update();          // UpdateCamera
};

COMPILE_TIME_ASSERT(camera_hpp, sizeof(Camera) == 0x18);

#endif // GUARD_CAMERA_HPP
