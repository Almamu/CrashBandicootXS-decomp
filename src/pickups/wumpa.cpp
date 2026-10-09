#include "pickups.hpp"
#include "action_ctrl.hpp"
#include "hud.hpp"
#include "audio.hpp"

extern "C" {
#include "math_util.h"
#include "util.h"
#include "gfx.h"
#include "globals.h"
#include "player.h"
}

/* The wumpa's flight to the HUD, payout start and hop, its small methods,
 * the stopwatch, and the action controller's Reset, which the ROM puts
 * after them (#664, include/pickups.hpp). The first three were the end
 * of wumpa_update.cpp; they need cse's skip-blocks, which that object is
 * built without (#662 round 3, see the Makefile). */

/* pos - off. As an inline's parameter, the offset is loaded from the
 * pool again for each axis, as in the ROM, not kept across the call. */
static inline s32 Offset(s32 pos, s32 off)
{
    return pos - off;
}

/* Flies to the HUD's wumpa counter from `mode` pixels to the left
 * (DropWumpa's), as PickUp's state 1 does, and shows the counter. */
void Wumpa::SendToHud()
{
    s32 outX, outY;
    s32 newX, newY;

    gAudioContext->PlaySfx(SFX_WUMPA, 0x100);
    state = 1;
    x -= INT_TO_Q8(mode);
    affine = 0xa0;
    ClampFrame(this);
    screenSpace = 1;

    WorldToScreen(this, Q8_TO_INT(x), Q8_TO_INT(y), &outX, &outY);

    newX = INT_TO_Q8(outX);
    x = newX;
    velX = -FixedDiv(Offset(newX, 0x1000), 0x1400);
    newY = INT_TO_Q8(outY);
    y = newY;
    velY = -FixedDiv(Offset(newY, 0x1000), 0x1400);
    gHud->ShowWumpa();
}

/* The payout: 10 drops, the first on the next frame (Update). */
void Wumpa::StartPayout()
{
    state = 3;
    counter = 0xa;
}

/* The hop: `phase` steps a sine, the height 0x30 pixels; hop 1 moves to
 * the left and hop 2 to the right, by gWumpaHopWidths[mode - 1]. */
void Wumpa::UpdateHop()
{
    struct three_words widths = *(const struct three_words *)gWumpaHopWidths;
    s32 dy;
    s32 sn;

    sn = gSineTable[phase * 4];
    dy = FixedMul(sn, 0x3000);
    y = anchor.y - dy;
    sn = gSineTable[phase * 2];
    sn = FixedMul(sn, widths.a[mode - 1]);
    if (mode == 1)
        x = anchor.x - sn;
    else if (mode == 2)
        x = anchor.x + sn;
    else
        x = anchor.x;
}

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
    anchor = *(struct vec2 *)&x;
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
