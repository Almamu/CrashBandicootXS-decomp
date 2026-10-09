#include "pickups.hpp"
#include "player.hpp"
#include "hud.hpp"
#include "audio.hpp"
#include "level_state.hpp"

extern "C" {
#include "math_util.h"
#include "util.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
#include "player.h"
}

/* The extra life (#664, include/pickups.hpp). An old_agbcp object
 * (OLD_AGBCC_OBJS). The wumpa's CheckPickup, which the ROM puts right
 * after it, starts wumpa_update.cpp. */

/* Unless it is early in a hop (`phase` up to 0x16) while the player's
 * `ctrlMode` isn't 3, or it was already touched, or contact is off: on
 * contact with the player's hitbox, touched, and picked up to fly to the
 * HUD. */
void ExtraLife::CheckPickup()
{
    if (mode != 0 && phase <= 0x16) {
        if (gPlayer->ctrlMode != 3)
            return;
    }
    {
        u32 flags = f.flags << 24;
        s32 contact;

        if ((flags >> 27) & 1)
            return;
        contact = (flags >> 26) & 1;
        if (!contact)
            return;
    }
    struct aabb box = GetAnimHitbox();
    Player *player = gPlayer;
    struct aabb playerBox = player->GetAnimHitbox();

    if (AabbOverlaps(&playerBox, &box)) {
        f.b.bit3 = 1;
        PickUp(0);
    }
}

/* Collected: plays the extra life sound and flies off, either to the
 * HUD's lives counter (`randomize` 0: state 1, and shows the counter) or
 * to a random point off the screen (state 2), as Wumpa::PickUp does. */
void ExtraLife::PickUp(u8 randomize)
{
    s32 dx, dy;
    s32 outX, outY;
    s32 newX, newY;

    gAudioContext->PlaySfx(SFX_EXTRA_LIFE, 0x100);
    affine = 0xa0;

    if (randomize) {
        u32 rv = (u16)rand();
        u8 lowbit = rv & 1;

        counter = lowbit;
        if (lowbit) {
            if (rv & 2)
                dx = INT_TO_Q8((rv & 0x3f) + 5);
            else
                dx = INT_TO_Q8(0xeb - (rv & 0x3f));
        } else {
            dx = INT_TO_Q8((rv & 0x7f) + 0x24);
        }
        dy = INT_TO_Q8((rv & 0x1f) + 0x10);
        state = 2;
    } else {
        dx = 0xb400;
        dy = 0xc00;
        state = 1;
        gHud->ShowLives();
    }
    f.b.active = 1;
    {
        u8 one = 1; // materialized before the field's address

        screenSpace = one;
    }

    WorldToScreen(this, Q8_TO_INT(x), Q8_TO_INT(y), &outX, &outY);

    newX = INT_TO_Q8(outX);
    x = newX;
    velX = -FixedDiv(newX - dx, 0x1400);
    newY = INT_TO_Q8(outY);
    y = newY;
    velY = -FixedDiv(newY - dy, 0x1400);
}

/* State 1 flies to the HUD and, there (within 0xB4 by 0xC pixels of the
 * corner), adds a life; state 2 flies off the screen and is gone. On the
 * spot, it bobs or hops. */
void ExtraLife::Update()
{
    u8 s = state;

    if (s == 1) {
        /* `n` is the new x, then the new y: the arrival test computes the
         * x again from the old position and the step, as the ROM does. */
        s32 px = x, vx = velX, n;

        n = px + vx;
        x = n;
        n = y + velY;
        y = n;
        if (Q8_TO_INT(px + vx) <= 0xb4 && Q8_TO_INT(n) <= 0xc) {
            gAudioContext->PlaySfx(SFX_HUD_COLLECT, 0x100);
            gLevelState->AddLife();
            MarkGone();
        }
    } else if (s == 2) {
        s32 fire;

        Fly();
        fire = 0;
        if (counter == 0) {
            s32 t = affine - 4;

            affine = t;
            if (t < 0x40)
                fire = 1;
        } else {
            affine += 0xc;
            if (Affine(this) > 0x1b0)
                fire = 1;
        }
        if (fire)
            MarkGone();
    } else {
        if (mode == 0)
            counter++;
        else if (++phase > 0x1f)
            mode = 0;
    }

    if (state == 0) {
        if (mode == 0) {
            s32 sn = gSineTable[(counter & 0x7f) * 2];

            sn = FixedMul(sn, 0x280);
            y = anchor.y + sn;
        } else {
            UpdateHop();
        }
    }
    Sprite::Update();
}

/* The extra life spawner (DropExtraLife, entity_spawner.cpp): an extra
 * life at pixel (x, y), at home there, in the touchable list. */
ExtraLife *ExtraLife::Create(u16 id, u16 x, u16 y, s32 unused)
{
    ExtraLife *self = new ExtraLife(id, x, y);

    TouchableList()->Add(self);
    ClampFrame(self);
    self->mirrorBits.flipX = 0;
    self->mirrorBits.flipY = 0;
    self->counter = 0;
    self->mode = 0;
    self->phase = 0;
    return self;
}

/* Flies to the HUD's lives counter from `mode` pixels to the left, as
 * PickUp's state 1 does, and shows the counter. */
void ExtraLife::SendToHud()
{
    s32 outX, outY;
    s32 newX, newY;

    gAudioContext->PlaySfx(SFX_EXTRA_LIFE, 0x100);
    state = 1;
    x -= INT_TO_Q8(mode);
    screenSpace = 1;

    WorldToScreen(this, Q8_TO_INT(x), Q8_TO_INT(y), &outX, &outY);

    newX = INT_TO_Q8(outX);
    x = newX;
    velX = -FixedDiv(newX - 0xb400, 0x1400);
    newY = INT_TO_Q8(outY);
    y = newY;
    velY = -FixedDiv(newY - 0xc00, 0x1400);
    gHud->ShowLives();
}

/* The hop: `phase` steps a sine, the height 8 pixels; hop 1 moves to the
 * left and hop 2 to the right, by gExtraLifeHopWidths[mode - 1]. */
void ExtraLife::UpdateHop()
{
    struct three_words widths = *(const struct three_words *)gExtraLifeHopWidths;
    s32 dy;
    s32 sn;

    sn = gSineTable[phase * 4];
    dy = FixedMul(sn, 0x800);
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
void ExtraLife::Draw()
{
    gSpriteRenderer->Draw(this);
    if (animDone != 0)
        f.b.bit3 = 0;
}

s32 ExtraLife::GetClassId()
{
    return 2;
}

ExtraLife::~ExtraLife()
{
}

/* On the spot. */
void ExtraLife::Reset()
{
    state = 0;
}

ExtraLife::ExtraLife()
{
    Reset();
}

/* While on the spot, and while the player is in contact: CheckPickup. */
s32 ExtraLife::CheckPlayerContact()
{
    if (state == 0) {
        if (gPlayer->f.flags >> 7)
            CheckPickup();
    }
    return 0;
}

/* Puts the extra life at pixel (x, y), and makes that its home. */
void ExtraLife::SetHome(s32 px, s32 py)
{
    SetPixelPos(px, py);
    anchor = *(struct vec2 *)&x;
}

/* Starts hop `mode` from its start. */
void ExtraLife::SetHop(u8 mode)
{
    this->mode = mode;
    phase = 0;
}

void ExtraLife::SetCounter(u8 value)
{
    counter = value;
}
