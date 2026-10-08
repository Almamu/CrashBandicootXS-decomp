#include "pickups.hpp"
#include "spawners.hpp"
#include "player.hpp"
#include "hud.hpp"

extern "C" {
#include "math_util.h"
#include "match.h"
#include "util.h"
#include "audio.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
}

/* The wumpa's pick-up, update and spawn (#664, include/pickups.hpp). An
 * old_agbcp object (OLD_AGBCC_OBJS). */

/* Collected: plays the wumpa sound and flies off, either to the HUD's
 * wumpa counter (`randomize` 0: state 1, and shows the counter) or to a
 * random point off the screen (state 2; `counter` says which way). The
 * position becomes a screen position, and the step covers the distance
 * to the target in 20 frames. Called by crate_break.cpp and time_trial.cpp
 * with `randomize` 1, by CheckPickup with either. */
void Wumpa::PickUp(u8 randomize)
{
    s32 dx, dy;
    s32 outX, outY;
    s32 newX, newY;

    PlaySfx(gAudioContext, SFX_WUMPA, 0x100);
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
        dx = dy = 0x1000;
        state = 1;
        gHud->ShowWumpa();
    }
    affine = 0xa0;
    ClampFrame(this);
    {
        u8 one = 1; // materialized before the field's address

        screenSpace = one;
    }
    f.b.active = 1;

    WorldToScreen(this, Q8_TO_INT(x), Q8_TO_INT(y), &outX, &outY);

    newX = INT_TO_Q8(outX);
    x = newX;
    velX = -FixedDiv(newX - dx, 0x1400);
    newY = INT_TO_Q8(outY);
    y = newY;
    velY = -FixedDiv(newY - dy, 0x1400);
}

/* State 1 flies to the HUD (while `affine` grows by 4 up to 0x100) and,
 * there, collects the wumpa; state 2 flies off the screen (`affine` down
 * by 4 below 0x40, or up by 12 past 0x1B0) and is gone. State 3 pays out:
 * every 11 frames it drops a wumpa flying to the HUD, 10 in all, staying
 * at the player, then it is gone. On the spot, it bobs or hops. */
void Wumpa::Update()
{
    if (state == 1) {
        Fly();
        if (affine != 0) {
            affine += 4;
            if (Affine(this) > 0x100)
                affine = 0;
        }
        if (Q8_TO_INT(x) <= 0x10 && Q8_TO_INT(y) <= 0x10) {
            PlaySfx(gAudioContext, SFX_HUD_COLLECT, 0x100);
            CollectWumpa(gLevelState);
            MarkGone();
        }
    } else if (state == 2) {
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
    } else if (state == 3) {
        if (++counter > 10) {
            counter = 0;
            s32 px = Q8_TO_INT(x);
            s32 py = Q8_TO_INT(y);

            gEntitySpawner->DropWumpa(px, py, 0, 0, true);
            /* The byte's zero-extension spelled out: `++phase > 9` gives an
             * `and` with a 0xFF hoisted to the top of the function. */
            u32 dropped = phase + 1;

            phase = dropped;
            if ((dropped << 24) >> 24 > 9)
                MarkGone();
        }
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
    } else if (state == 3) {
        /* Both coordinates loaded and offset before the stores, as in the
         * ROM. */
        Player *player = gPlayer;
        s32 px = player->x;
        s32 py = player->y;
        s32 nx = px - 0x400;
        s32 ny = py - 0xe00;

        x = nx;
        y = ny;
    }
    Sprite::Update();
}

/* The wumpa spawner (SpawnWumpa, spawn_pickups.cpp; DropWumpa): a wumpa at
 * pixel (x, y), at home there, in the foreground list if `special` is
 * 0xFFFF and the touchable list otherwise, with animation 1 of the sprite
 * bank at SPRITE_BANK_BASE + 0x1A4.
 *
 * `mode` is a 0 set before the first call: CSE loses it at the list
 * `if`/`else` join, so the ROM's dead `cmp r7, #0xff` (SetHop's payout
 * test) stays. `phase` is a 0 hidden from CSE (MATCH_KEEP, as in the C):
 * the ROM compares it with the frame count (`cmp r6, r0`) and stores it
 * at +0x4B, while `counter` gets a fresh 0; known as a constant, the 0 is
 * shared with `counter`. The tag's 1 is materialized before its address,
 * and `phase`'s 0 between the two (the `one` and `tag` locals). The palette
 * slot goes through an `s32` for the ROM's zero-extension of the `u8`. */
Wumpa *Wumpa::Create(u16 id, u16 x, u16 y, u16 special)
{
    u8 mode = 0;
    u8 phase;
    Wumpa *self = new Wumpa(id, x, y);

    if (special == 0xffff)
        ForegroundList()->Add(self);
    else
        TouchableList()->Add(self);
    self->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + 0xd2 * 2);
    {
        u8 one = 1;
        u8 *tag = &self->tag;

        phase = 0;
        MATCH_KEEP(phase); // see above
        *tag = one;
    }
    self->ResetFrameTimer();
    self->ResetFrameIndex();
    self->SetAnimDone(0);
    {
        s32 frame = 0;
        s32 count = self->bank->anims[self->tag].frameCount;

        if (phase >= count)
            frame = count - 1;
        self->frame = frame;
    }
    self->mirrorBits.flipX = 0;
    self->mirrorBits.flipY = 0;
    self->counter = 0;
    self->mode = mode;
    self->phase = phase;
    if (mode == 0xff)
        self->StartPayout();
    {
        s32 slot = GetPaletteSlot(gPaletteCache, self->bank->anims->paletteId);

        self->palette = slot;
    }
    return self;
}

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

    PlaySfx(gAudioContext, SFX_WUMPA, 0x100);
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
