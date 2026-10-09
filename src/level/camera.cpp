#include "bg_layer.hpp"
#include "sprite_obj.hpp"

extern "C" {
#include "core.h"
#include "math_util.h"
#include "memory.h"
#include "level.h"
#include "player.h"
#include "globals.h"
}

/* GitHub issue #44: the `gCamera` camera-follow block (the
 * "generic 0x18-byte block" docs/matching/archive/issue-37-game-loop-2375c.md
 * saw `PlayRoom`'s tail flush via `SnapCamera`). The global
 * `operator new` & co. that follow it in ROM are in
 * src/system/operator_new.cpp (here until #770).
 *
 * `struct camera` holds a Q8 position (`x`/`y`), a Q8 look-ahead offset
 * (`vx`/`vy`), the followed object (`target`) and a `mode`. Every
 * per-frame update eases the position a quarter of the way toward
 * `target + look-ahead` (`x += (goal - x) / 4`), and both publishers
 * (`SnapCamera`/`UpdateCamera`) hand `(x - (120 << 8), y - (80 << 8))`
 * - the position offset by half the 240x160 screen - to the still-raw
 * `SetLevelScroll` on `gLevelLayers`, which clamps it to `>= 0`,
 * converts Q8 to whole pixels, caps it at that object's own `+0x0`/`+0x4`
 * limits, and stores the result at its `+0x8`/`+0xC`. That is what makes
 * this a camera: the target ends up centered on screen, clamped to
 * the level bounds.
 *
 * - `StepCameraDirectional` (mode 2): `target+0x24` direction bits steer the
 *   look-ahead in 0x100 steps (bit 0/1 = +x/-x up to +0x27FF/-0x2800,
 *   bit 2/3 = -y/+y up to -0x1AAA/+0x1AA9); an axis with neither of its
 *   bits set decays back toward 0 by the same step. `target+0x24` is
 *   gPlayer's `dir` (PLAYER_DIR_*, player.h; the target is gPlayer).
 * - `StepCameraFacing` (mode 1): horizontal look-ahead grows toward -0x1276
 *   or +0x1276 depending on `target+0x28` bit 4 (the mirror flag several
 *   actor-side functions already document at that offset), vertical
 *   look-ahead fixed at -0x1000.
 * - `SnapCamera`: snaps straight to the target (no easing), seeding the
 *   mode-1 look-ahead at its limit (or zero for any other mode), then
 *   publishes. Called from `ResumeRoomAfterPause`'s teardown/refresh pass
 *   (`room.cpp`) and `PlayRoom`'s shared tail (`run_room.cpp`).
 * - `UpdateCamera`: the per-frame update, dispatching on `mode`, then
 *   publishing. Called from `UpdateRoomFrame` (`room_frame.cpp`).
 *
 * Matching notes: both easing functions copy the target's position as
 * one 8-byte `struct vec2` (the pair lands in r2/r3, as in the ROM; two
 * s32 locals get r3/r4 and push `cam` down into r2), and
 * `StepCameraDirectional` turns its goal into the remaining distance
 * (`goal.x -= cam->x`) before each quarter step. The empty
 * `case 3` in `UpdateCamera` has no behavior; it reproduces the ROM's
 * switch decision tree (`cmp #2 / beq`, `bgt`, `cmp #1 / bne`), which
 * a two-case switch compiles to a flat compare chain instead. See
 * docs/matching/archive/issue-44-camera-follow.md.
 *
 * Real bytes formerly the whole of `asm/code_3_2_17_26bf8.s`. */

void StepCameraDirectional(struct camera *cam)
{
    struct vec2 goal = cam->target->Pos();
    u8 dir = cam->target->dir;

    if (dir != 0) {
        if ((dir & PLAYER_DIR_UP) && cam->vy > -0x1AAA)
            cam->vy -= 0x100;
        else if ((dir & PLAYER_DIR_DOWN) && cam->vy <= 0x1AA9)
            cam->vy += 0x100;

        if ((dir & PLAYER_DIR_LEFT) && cam->vx > -0x2800)
            cam->vx -= 0x100;
        else if ((dir & PLAYER_DIR_RIGHT) && cam->vx <= 0x27FF)
            cam->vx += 0x100;

        if (!(dir & PLAYER_DIR_X)) {
            if (cam->vx > 0)
                cam->vx -= 0x100;
            else if (cam->vx < 0)
                cam->vx += 0x100;
        }

        if (!(dir & PLAYER_DIR_Y)) {
            if (cam->vy > 0)
                cam->vy -= 0x100;
            else if (cam->vy < 0)
                cam->vy += 0x100;
        }
    }

    goal.x += cam->vx;
    goal.y += cam->vy;
    goal.x -= cam->x;
    cam->x += goal.x / 4;
    goal.y -= cam->y;
    cam->y += goal.y / 4;
}

void StepCameraFacing(struct camera *cam)
{
    struct vec2 goal = cam->target->Pos();

    if ((cam->target->mirror << 27) < 0) {
        if (cam->vx > -0x1276)
            cam->vx -= 0x100;
    } else {
        if (cam->vx <= 0x1275)
            cam->vx += 0x100;
    }

    cam->vy = -0x1000;
    goal.x += cam->vx;
    goal.y += cam->vy;
    cam->x += (goal.x - cam->x) / 4;
    cam->y += (goal.y - cam->y) / 4;
}

void SnapCamera(struct camera *cam)
{
    Sprite *target = cam->target;

    cam->x = target->x;
    cam->y = target->y;

    if (cam->mode == 1) {
        if ((target->mirror << 27) < 0)
            cam->vx = -0x1276;
        else
            cam->vx = 0x1276;
        cam->vy = -0x1000;
    } else {
        cam->vx = 0;
        cam->vy = 0;
    }

    cam->x += cam->vx;
    cam->y += cam->vy;
    gLevelLayers->SetScroll(cam->x - INT_TO_Q8(120), cam->y - INT_TO_Q8(80));
}

void UpdateCamera(struct camera *cam)
{
    switch (cam->mode) {
    case 1:
        StepCameraFacing(cam);
        break;
    case 2:
        StepCameraDirectional(cam);
        break;
    case 3: // no behavior - needed for the ROM's switch shape (see the file comment)
        break;
    }

    gLevelLayers->SetScroll(cam->x - INT_TO_Q8(120), cam->y - INT_TO_Q8(80));
}
