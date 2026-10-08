#include "pickups.hpp"
#include "action_ctrl.hpp"

extern "C" {
#include "math_util.h"
#include "globals.h"
#include "player.h"
}

/* The wumpa's small methods, the stopwatch, and the action controller's
 * Reset, which the ROM puts after them (#664, include/pickups.hpp). */

/* Draws the sprite; once a non-looping animation has ended, clears the
 * touched flag. */
void Wumpa::Draw()
{
    gSpriteRenderer->Draw(this);
    if (animDone != 0)
        f.b.bit3 = 0;
}

s32 Wumpa::GetClassId()
{
    return 2;
}

Wumpa::~Wumpa()
{
}

/* Sets the vulnerable flag; the wumpa is on the spot. */
void Wumpa::Reset()
{
    f.b.vulnerable = 1;
    state = 0;
}

Wumpa::Wumpa()
{
    Reset();
}

/* While on the spot, and while the player is in contact: CheckPickup. */
s32 Wumpa::CheckPlayerContact()
{
    if (state == 0) {
        if (gPlayer->f.flags >> 7)
            CheckPickup();
    }
    return 0;
}

/* Puts the wumpa at pixel (x, y), and makes that its home. */
void Wumpa::SetHome(s32 px, s32 py)
{
    SetPixelPos(px, py);
    anchor = *(struct orbit_vec *)&x;
}

/* Starts hop `mode` from its start; 0xFF starts the payout instead. */
void Wumpa::SetHop(s32 mode)
{
    this->mode = mode;
    phase = 0;
    if (mode == 0xff)
        StartPayout();
}

void Wumpa::SetCounter(u8 value)
{
    counter = value;
}

/* Updates the sprite while the player is within 0x180 pixels on both
 * axes; otherwise the stopwatch is gone. */
void Stopwatch::Update()
{
    s32 d = Q8_TO_INT(gPlayer->x);

    d -= Q8_TO_INT(x);
    MAKE_ABS(d);
    if (d > 0x180) {
        MarkGone();
    } else {
        d = Q8_TO_INT(gPlayer->y);
        d -= Q8_TO_INT(y);
        MAKE_ABS(d);
        if (d > 0x180)
            MarkGone();
        else
            Sprite::Update();
    }
}

/* `unused` is the spawn table slot's fourth argument. */
Stopwatch *Stopwatch::Create(u16 id, u16 x, u16 y, u16 unused)
{
    return new Stopwatch(id, x, y);
}

void Stopwatch::Reset()
{
}

Stopwatch::~Stopwatch()
{
}

Stopwatch::Stopwatch()
{
    Reset();
}

/* Clears the controller's state, the player, the motion queue (both
 * entries pending), the spin and bump timers and the flags. InitActionCtrl
 * runs it. */
void ActionCtrl::Reset()
{
    turboRun = 0;
    state = 0;
    bumpedMotionX = 0;
    motionX = 0;
    motionY = 0;
    motionXPending = 1;
    motionYPending = 1;
    unk_14 = 0;
    part = 0;
    spinCooldown = 0;
    unk_2A = 0;
    dpadLockTimer = 0;
    bumpTimer = 0;
    frame = 0;
    frames = 0;
    idleFidget = 0;
    slamBlocked = 0;
}
