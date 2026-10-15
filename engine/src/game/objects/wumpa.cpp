#include "game/objects/pickups.hpp"
#include "game/crash/action_ctrl.hpp"
#include "game/ui/hud.hpp"
#include "engine/audio/audio.hpp"
#include "game/crash/player.hpp"

extern "C" {
#include "engine/util/math_util.h"
#include "engine/util/util.h"
#include "engine/gfx/gfx.h"
#include "globals.h"
}

using Game::Entities::ClampFrame;
using Game::Objects::Wumpa;

/* The wumpa's flight to the HUD, payout start and hop, and its small
 * methods (#664, include/game/objects/pickups.hpp). The stopwatch follows in
 * stopwatch.cpp. The first three were the end
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
    state = WUMPA_STATE_TO_HUD;
    x -= INT_TO_Q8(mode);
    affine = Q8(0.625);
    ClampFrame(this);
    screenSpace = 1;

    WorldToScreen(this, Q8_TO_INT(x), Q8_TO_INT(y), &outX, &outY);

    newX = INT_TO_Q8(outX);
    x = newX;
    velX = -FixedDiv(Offset(newX, INT_TO_Q8(16)), INT_TO_Q8(20));
    newY = INT_TO_Q8(outY);
    y = newY;
    velY = -FixedDiv(Offset(newY, INT_TO_Q8(16)), INT_TO_Q8(20));
    gHud->ShowWumpa();
}

/* The payout: 10 drops, the first on the next frame (Update). */
void Wumpa::StartPayout()
{
    state = WUMPA_STATE_PAYOUT;
    counter = 0xa;
}

/* The hop: `phase` steps a sine, the height 0x30 pixels; hop 1 moves to
 * the left and hop 2 to the right, by gWumpaHopWidths[mode - 1]. */
void Wumpa::UpdateHop()
{
    /* `widths` is needed: inlined, the code is 5 instructions shorter */
    struct three_words widths = gWumpaHopWidths;
    s32 sn;

    sn = gSineTable[phase * 4];
    y = anchor.y - FixedMul(sn, INT_TO_Q8(48));
    sn = gSineTable[phase * 2];
    sn = FixedMul(sn, widths.a[mode - 1]);
    if (mode == PICKUP_MODE_HOP_LEFT)
        x = anchor.x - sn;
    else if (mode == PICKUP_MODE_HOP_RIGHT)
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
    state = WUMPA_STATE_IDLE;
}

Wumpa::Wumpa()
{
    Reset();
}

/* While on the spot, and while the player is in contact: CheckPickup. */
s32 Wumpa::CheckPlayerContact()
{
    if (state == WUMPA_STATE_IDLE && gPlayer->f.flags >> ENTITY_BIT_COLLIDES)
        CheckPickup();
    return 0;
}

/* Puts the wumpa at pixel (x, y), and makes that its home. */
void Wumpa::SetHome(s32 px, s32 py)
{
    SetPixelPos(px, py);
    anchor = Pos();
}

/* Starts hop `mode` from its start; 0xFF starts the payout instead. */
void Wumpa::SetHop(s32 mode)
{
    this->mode = mode;
    phase = 0;
    if (mode == WUMPA_MODE_PAYOUT)
        StartPayout();
}

void Wumpa::SetCounter(u8 value)
{
    counter = value;
}
