#include "player.hpp"
#include "action_ctrl.hpp"
#include "swim_ctrl.hpp"
#include "input_ctrl.hpp"

extern "C" {
#include "globals.h"
}

/* The player's resets (Player, include/player.hpp). */

/* The player's state for a new life: GroundSprite::Reset's fields, the
 * player's own, a fresh invulnerability deadline, and Aku Aku's palette
 * slot. */
void Player::Reset()
{
    f.b.collides = 1;
    f.b.vulnerable = 1;
    f.b.attacks = 1;
    f.b.visible = 0;
    f.b.floorProbe = 1;
    speedX = 0;
    speedY = 0;
    rampX.start = 0;
    rampX.step = 0;
    rampX.target = 0;
    rampY.start = 0;
    rampY.step = 0;
    rampY.target = 0;
    mirrorFlags.mirrorX = 0;
    hitAxes = 8;
    dir = 0;
    mover = 0;
    type = 0;
    lastHitbox = 0;
    bumped = 0;
    carried = 0;
    busy = 0;
    ctrlMode = 0;
    deadline = gRoomFrameCount;
    countdown = 0;
    listCount = 0;
    bounce = 0;
    kind = EVENT_HIT;
    child->palette = child->GetAnimPaletteSlot();
    slippery = 0;
    hanging = 0;
    pushLeft = 0;
    pushRight = 0;
    dead = 0;
    f.b.gone = 0;
    cleared = 0;
}

/* The player's state on entering a room: stopped, standing, facing right,
 * the first animation, and its controller (the room kind's, `ctrlMode`)
 * restarted. */
void Player::ResetForRoom()
{
    speedX = 0;
    speedY = 0;
    hitAxes = 8;
    dir = 0;
    mirrorFlags.mirrorY = 0;
    tag = 0;
    f.b.bit3 = 0;
    f.b.vulnerable = 1;
    {
        /* The switch is on a copy: the ROM runs the compares of `2` and `3`
         * on a copy of the mode in another register (docs/cplusplus.md,
         * "A switch on a copy"). */
        s32 mode = ctrlMode;
        s32 m = mode;

        switch (m) {
        case 0:
            ((ActionCtrl *)mover)->Restart();
            break;
        case 1:
            ((SwimCtrl *)mover)->Restart();
            break;
        case 2:
            break;
        case 3:
            ((InputCtrl *)mover)->Restart();
            break;
        }
    }
}
