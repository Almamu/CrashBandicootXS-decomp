#include "sprite_obj.hpp"
#include "player.hpp"

extern "C" {
#include "math_util.h"
#include <agb_syscall.h>
#include "util.h"
#include "system.h"
#include "audio.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
#include "player.h"
}

/* The rest of Sprite's accessors, UiSprite, and the part list's update
 * and collision passes (#664, include/sprite_obj.hpp). An old_agbcp
 * object (OLD_AGBCC_OBJS). */

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
        DrawAffineSpritePieces(gSpriteRenderer, (struct affine_part *)this, pos);
    else
        DrawSpritePieces(gSpriteRenderer, (struct oam_part *)this, pos);
}

void Sprite::SetPriority(s32 value)
{
    mirrorBits.priority = value;
}

/* The OBJ priority of its own mirror byte (the sprite's is layer 0's). */
s32 UiSprite::GetPriority()
{
    return mirrorFlags.priority;
}

UiSprite::~UiSprite()
{
}

UiSprite::UiSprite()
{
}

/* Each frame: compacts `items` (dropping the gone parts, which are
 * deleted) and rebuilds `visible`. A part near the camera (within a
 * 440x280 box 100/60 px before it) is updated, and kept in `visible`
 * when it is on screen (the 240x160 box at the camera). */
void PartList::Update()
{
    struct {
        struct aabb near;
        struct aabb screen;
    } f;
    struct bg_scroll_layer *cam;
    s32 i;
    s32 zero;
    struct aabb *ps;

    {
        s32 w = INT_TO_Q8(440);
        s32 h = INT_TO_Q8(280);
        f.near.w = w;
        f.near.h = h;
    }
    cam = gLevelLayers->layer0;
    {
        s32 cx;
        s32 cy;

        cx = INT_TO_Q8(cam->x);
        zero = 0;
        cx -= INT_TO_Q8(100);
        cy = INT_TO_Q8(cam->y) - INT_TO_Q8(60);
        f.near.x = cx;
        f.near.y = cy;
    }
    {
        s32 cx = INT_TO_Q8(cam->x);
        s32 cy = INT_TO_Q8(cam->y);
        f.screen.x = cx;
        f.screen.y = cy;
    }
    ps = &f.screen;
    {
        s32 w = INT_TO_Q8(240);
        s32 h = INT_TO_Q8(160);
        ps->w = w;
        ps->h = h;
    }
    visibleCount = zero;

    for (i = 0; i < count; i++) {
        Sprite *part = items[i];

        if (part->f.flags & PART_FLAG_GONE) {
            if (i < capacity) {
                CpuSet(&items[i + 1], &items[i],
                       ((count - i) & CPU_SET_COUNT_MASK) | CPU_SET_32BIT);
                count--;
                items[count] = 0;
            }
            delete part;
            i--;
        } else if ((u8)part->IsInsideRect(&f.near)) {
            part->Update();
            if ((u8)part->OverlapsRect(&f.screen))
                visible[visibleCount++] = part;
        }
    }
}

/* Hands each visible part with a class id above 4 that is in contact
 * (`visible`) to CollideWithPlayer (when `other` is the player) or
 * CollideWithObject, with the incoming box. The box is copied into one
 * shared temporary (memcpy) before each call, as in the ROM. `unused` is
 * the caller's padding argument. */
void PartList::Collide(struct aabb box, s32 unused, MovingSprite *other)
{
    s32 i;

    for (i = 0; i < visibleCount; i++) {
        Sprite *s = visible[i];

        if (s->GetClassId() <= 4)
            continue;
        MovingSprite *part = (MovingSprite *)s; // class 5 or 6
        s32 inContact = (part->f.flags >> 2) & 1;

        if (!inContact)
            continue;
        if (other == gPlayer)
            CollideWithPlayer(box, part);
        else
            CollideWithObject(box, part, other);
    }
}

/* A hit between `part` and the player, against the box CollideParts
 * passed. Invincible (mask level 3): when `part` touches the box
 * (ClassifySpriteContact), its HandleEvent with the player's kind. A
 * solid part (flags2 bit 3) pushes the player out of its body box on
 * whichever side the player is, and bumps it. Otherwise by
 * ClassifySpriteContact: 1, the player touched it from above (a player of
 * kind 1 falling onto it bounces, and hits it; any other kind hits it
 * with that kind); 2, the part touched the player: it is marked touched,
 * hit when the mask is on, and hits the player with its kind. */
void PartList::CollideWithPlayer(struct aabb box, MovingSprite *part)
{
    if (gLevelState->maskLevel == MASK_LEVEL_INVINCIBLE) {
        if (!ClassifySpriteContact(part, &box))
            return;
        part->HandleEvent(1, gPlayer->kind, 0);
    } else if ((part->f.bytes.flags2 >> 3) & 1) {
        struct aabb a = gPlayer->GetAnimHitbox();
        struct aabb b = part->GetBodyBox();
        s32 px;

        if (!AabbOverlaps(&a, &b))
            return;
        px = part->x;
        if (px < gPlayer->x) {
            gPlayer->x = px + ((b.w + a.w) << 7);
            gPlayer->HandleEvent(0, EVENT_BUMP, 2);
        } else {
            gPlayer->x = px - ((b.w + a.w) << 7);
            gPlayer->HandleEvent(0, EVENT_BUMP, 1);
        }
    } else {
        u8 kind;

        switch (ClassifySpriteContact(part, &box)) {
        case 0:
            break;
        case 1:
            gPlayer->f.b.bit3 = 1;
            kind = gPlayer->kind;
            if (kind == 1) {
                if (gPlayer->speedY > 0) {
                    part->HandleEvent(1, EVENT_HIT, 0);
                    gPlayer->HandleEvent(0, EVENT_BOUNCE, 0);
                    PlaySfx(gAudioContext, SFX_BOUNCE, 0x100);
                }
            } else {
                part->HandleEvent(1, kind, 0);
            }
            break;
        case 2:
            part->f.flags |= PART_FLAG_TOUCHED;
            if (gLevelState->maskLevel)
                part->HandleEvent(1, EVENT_HIT, 0);
            gPlayer->HandleEvent(1, part->kind, 0);
            break;
        }
    }
}
