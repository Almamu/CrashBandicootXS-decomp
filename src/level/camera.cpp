#include "bg_layer.hpp"
#include "sprite_obj.hpp"
#include "camera.hpp"
#include "player.hpp"

extern "C" {
#include "core.h"
#include "math_util.h"
#include "memory.h"
#include "level.h"
#include "globals.h"
}

/* GitHub issue #44: the `gCamera` camera-follow block (the
 * "generic 0x18-byte block" docs/matching/archive/issue-37-game-loop-2375c.md
 * saw `PlayRoom`'s tail flush via `SnapCamera`), class Camera
 * (include/camera.hpp, #761). The global
 * `operator new` & co. that follow it in ROM are in
 * src/system/operator_new.cpp (here until #770).
 *
 * The camera holds a Q8 position (`x`/`y`), a Q8 look-ahead offset
 * (`vx`/`vy`), the followed object (`target`) and a `mode`. Every
 * per-frame update eases the position a quarter of the way toward
 * `target + look-ahead` (`x += (goal - x) / 4`), and both publishers
 * (`Snap`/`Update`) hand `(x - (120 << 8), y - (80 << 8))`
 * - the position offset by half the 240x160 screen - to the still-raw
 * `SetLevelScroll` on `gLevelLayers`, which clamps it to `>= 0`,
 * converts Q8 to whole pixels, caps it at that object's own `+0x0`/`+0x4`
 * limits, and stores the result at its `+0x8`/`+0xC`. That is what makes
 * this a camera: the target ends up centered on screen, clamped to
 * the level bounds.
 *
 * - `StepDirectional` (StepCameraDirectional, mode 2): `target+0x24`
 *   direction bits steer the look-ahead in 0x100 steps (bit 0/1 = +x/-x up to +0x27FF/-0x2800,
 *   bit 2/3 = -y/+y up to -0x1AAA/+0x1AA9); an axis with neither of its
 *   bits set decays back toward 0 by the same step. `target+0x24` is
 *   gPlayer's `dir` (PLAYER_DIR_*, player.h; the target is gPlayer).
 * - `StepFacing` (StepCameraFacing, mode 1): horizontal look-ahead grows
 *   toward -0x1276 or +0x1276 depending on `target+0x28` bit 4 (the mirror flag several
 *   actor-side functions already document at that offset), vertical
 *   look-ahead fixed at -0x1000.
 * - `Snap` (SnapCamera): snaps straight to the target (no easing), seeding the
 *   mode-1 look-ahead at its limit (or zero for any other mode), then
 *   publishes. Called from `ResumeRoomAfterPause`'s teardown/refresh pass
 *   (`room.cpp`) and `PlayRoom`'s shared tail (`play_room.cpp`).
 * - `Update` (UpdateCamera): the per-frame update, dispatching on `mode`, then
 *   publishing. Called from `UpdateRoomFrame` (`play_room.cpp`).
 *
 * Matching notes: both easing functions copy the target's position as
 * one 8-byte `struct vec2` (the pair lands in r2/r3, as in the ROM; two
 * s32 locals get r3/r4 and push `this` down into r2), and
 * `StepDirectional` turns its goal into the remaining distance
 * (`goal.x -= x`) before each quarter step. The empty
 * `case 3` in `Update` has no behavior; it reproduces the ROM's
 * switch decision tree (`cmp #2 / beq`, `bgt`, `cmp #1 / bne`), which
 * a two-case switch compiles to a flat compare chain instead. See
 * docs/matching/archive/issue-44-camera-follow.md.
 *
 * Real bytes formerly the whole of `asm/code_3_2_17_26bf8.s`. */

void Camera::StepDirectional()
{
    struct vec2 goal = target->Pos();
    u8 dir = target->dir;

    if (dir != 0) {
        if ((dir & PLAYER_DIR_UP) && vy > -0x1AAA)
            vy -= 0x100;
        else if ((dir & PLAYER_DIR_DOWN) && vy <= 0x1AA9)
            vy += 0x100;

        if ((dir & PLAYER_DIR_LEFT) && vx > -0x2800)
            vx -= 0x100;
        else if ((dir & PLAYER_DIR_RIGHT) && vx <= 0x27FF)
            vx += 0x100;

        if (!(dir & PLAYER_DIR_X)) {
            if (vx > 0)
                vx -= 0x100;
            else if (vx < 0)
                vx += 0x100;
        }

        if (!(dir & PLAYER_DIR_Y)) {
            if (vy > 0)
                vy -= 0x100;
            else if (vy < 0)
                vy += 0x100;
        }
    }

    goal.x += vx;
    goal.y += vy;
    goal.x -= x;
    x += goal.x / 4;
    goal.y -= y;
    y += goal.y / 4;
}

void Camera::StepFacing()
{
    struct vec2 goal = target->Pos();

    if ((target->mirror << 27) < 0) {
        if (vx > -0x1276)
            vx -= 0x100;
    } else {
        if (vx <= 0x1275)
            vx += 0x100;
    }

    vy = -0x1000;
    goal.x += vx;
    goal.y += vy;
    x += (goal.x - x) / 4;
    y += (goal.y - y) / 4;
}

void Camera::Snap()
{
    Sprite *followed = target;

    x = followed->x;
    y = followed->y;

    if (mode == 1) {
        if ((followed->mirror << 27) < 0)
            vx = -0x1276;
        else
            vx = 0x1276;
        vy = -0x1000;
    } else {
        vx = 0;
        vy = 0;
    }

    x += vx;
    y += vy;
    gLevelLayers->SetScroll(x - INT_TO_Q8(120), y - INT_TO_Q8(80));
}

void Camera::Update()
{
    switch (mode) {
    case 1:
        StepFacing();
        break;
    case 2:
        StepDirectional();
        break;
    case 3: // no behavior - needed for the ROM's switch shape (see the file comment)
        break;
    }

    gLevelLayers->SetScroll(x - INT_TO_Q8(120), y - INT_TO_Q8(80));
}
