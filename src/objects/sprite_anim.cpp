#include "bg_layer.hpp"
#include "sprite_obj.hpp"
#include "player.hpp"
#include "audio.hpp"
#include "level_state.hpp"

extern "C" {
#include "math_util.h"
#include <agb_syscall.h>
#include "util.h"
#include "system.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
#include "player.h"
}

/* The rest of Sprite's accessors (#664, include/sprite_obj.hpp). An
 * old_agbcp object (OLD_AGBCC_OBJS). UiSprite follows in ui_sprite.cpp. */

/* The current animation's step count. */
u8 Sprite::GetAnimFrameCount()
{
    return bank->anims[tag].frameCount;
}

/* The current animation's ticks per step. */
u8 Sprite::GetAnimDuration()
{
    return bank->anims[tag].duration;
}

void Sprite::ResetFrameIndex()
{
    frame = 0;
}

void Sprite::SetFrameTimer(s32 value)
{
    stepTimer = value;
}

void Sprite::ResetFrameTimer()
{
    stepTimer = 0;
}

void Sprite::SetAnimIndex(u8 value)
{
    tag = value;
}

/* Starts animation `index` from its first step. */
void Sprite::SetAnim(u8 index)
{
    tag = index;
    ResetFrameTimer();
    ResetFrameIndex();
    SetAnimDone(0);
}

void Sprite::IncFrameIndex()
{
    frame += 1;
}

void Sprite::IncFrameTimer()
{
    stepTimer += 1;
}

void Sprite::SetMoveAxes(u8 value)
{
    dir = value;
}

u8 Sprite::GetMoveAxes()
{
    return dir;
}

s32 Sprite::GetFrameIndex()
{
    return frame;
}

s32 Sprite::GetFrameTimer()
{
    return stepTimer;
}

u8 Sprite::GetAnimIndex()
{
    return tag;
}

s32 Sprite::GetGfxMode()
{
    return mirrorFlags.gfxMode;
}

void Sprite::SetGfxMode(s32 value)
{
    mirrorBits.gfxMode = value;
}

s32 Sprite::GetFlipX()
{
    return mirrorFlags.mirrorX;
}

s32 Sprite::GetFlipY()
{
    return mirrorFlags.mirrorY;
}

u8 Sprite::GetAnimDone()
{
    return animDone;
}

s32 Sprite::GetMosaic()
{
    return mirrorFlags.mosaic;
}

s32 Sprite::GetOamPalette()
{
    return palette;
}

s32 Sprite::GetColorMode()
{
    return mirrorFlags.colorMode;
}

u16 Sprite::GetAffine()
{
    return affine;
}

void Sprite::SetAffine(u16 value)
{
    affine = value;
}

/* Draws the sprite at its position plus (dx, dy): the affine pieces when
 * `affine` is set, the plain ones otherwise. */
void Sprite::DrawWithOffset(s32 dx, s32 dy)
{
    s32 pos[2];

    pos[0] = Q8_TO_INT(x) + dx;
    pos[1] = Q8_TO_INT(y) + dy;
    if (affine != 0)
        gSpriteRenderer->DrawAffinePieces(this, pos);
    else
        gSpriteRenderer->DrawPieces(this, pos);
}

void Sprite::SetPriority(s32 value)
{
    mirrorBits.priority = value;
}
