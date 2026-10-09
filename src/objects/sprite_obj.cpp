/* This object has the out-of-line SetFlipX/SetFlipY (sprite_obj.hpp). */
#define SPRITE_FLIP_OUT_OF_LINE
#include "bg_layer.hpp"
#include "sprite_obj.hpp"
#include "player.hpp"
#include "audio.hpp"
#include "level_state.hpp"

extern "C" {
#include "math_util.h"
#include "match.h"
#include "util.h"
#include "gfx.h"
#include "memory.h"
#include "level.h"
#include "globals.h"
#include <agb_syscall.h>
#include "system.h"
#include "player.h"
}

/* Sprite's methods (#664, include/sprite_obj.hpp): the virtual ones, the
 * constructor and destructor, and the accessors of its animation and
 * flags; and the three hitbox edge helpers the terrain probes use. */

/* Whether the current animation's hitbox (box[0], mirrored) overlaps
 * `region`. */
s32 Sprite::HitboxOverlaps(struct aabb *region)
{
    struct aabb box;
    s32 flipX = mirrorFlags.mirrorX;
    s32 flipY = mirrorFlags.mirrorY;
    s32 px = Q8_TO_INT(x);
    s32 py = Q8_TO_INT(y);
    const struct sprite_anim *rec = &bank->anims[tag];
    const struct hitbox_quad *hb = &rec->box[0];
    s32 offX = rec->box[0].offX;
    s32 offY = hb->offY;
    s32 w = hb->w;
    s32 h = hb->h;

    SetAabbPos(&box, offX + px, offY + py);
    SetAabbSize(&box, w, h);
    if (flipX)
        box.x = px * 2 - (box.x + box.w);
    if (flipY)
        box.y = py * 2 - (box.y + box.h);
    return (u8)AabbOverlaps(&box, region);
}

/* The OBJ palette slot of the current animation's palette
 * (GetPaletteSlot). */
s32 Sprite::GetAnimPaletteSlot()
{
    return (u8)gPaletteCache->GetSlot(bank->anims[tag].paletteId);
}

/* UNUSED - no caller anywhere in the ROM. The inverse of
 * OffsetToHitboxEdge: moves the Q8 position `dest` back from the edge of
 * hitbox `rec` that faces direction `kind` (ProbeTerrain's 1 right, 2
 * left, 4 up, 8 down) to the object's origin. */
void OffsetFromHitboxEdge(void *destArg, s32 kind, void *recArg)
{
    struct vec2 *dest = (struct vec2 *)destArg;
    struct hitbox_quad *rec = (struct hitbox_quad *)recArg;

    switch (kind) {
    case 2:
        dest->x += rec->w << 7;
        break;
    case 1:
        dest->x -= rec->w << 7;
        break;
    case 4:
        dest->y -= INT_TO_Q8(rec->offY);
        break;
    case 8:
    case 12:
        dest->y -= INT_TO_Q8(rec->offY + rec->h);
        break;
    }
}

/* Moves the Q8 position `dest` (an object's origin) to the edge of its
 * hitbox `rec` that faces direction `kind`: x +/- w/2, or y + offY (top)
 * / y + offY + h (bottom). ProbeGroundSpriteFloor/ProbeGroundSpriteTerrain
 * use it with 8 for the point under the object's feet. */
void OffsetToHitboxEdge(void *destArg, s32 kind, void *recArg)
{
    struct vec2 *dest = (struct vec2 *)destArg;
    struct hitbox_quad *rec = (struct hitbox_quad *)recArg;

    switch (kind) {
    case 2:
        dest->x -= rec->w << 7;
        break;
    case 1:
        dest->x += rec->w << 7;
        break;
    case 4:
        dest->y += INT_TO_Q8(rec->offY);
        break;
    case 8:
    case 12:
        dest->y += INT_TO_Q8(rec->offY + rec->h);
        break;
    }
}

/* Moves the Q8 position `dest` to the start of the hitbox edge that
 * faces direction `kind`, the point a ProbeTerrain scan along that edge
 * starts from: for 1/2 (right/left) the top end of the side edge (x +/-
 * w/2, y + offY; the scan runs down over `h`), for 4/8 (up/down) the left
 * end of the top or bottom edge (x - w/2; the scan runs right over `w`). */
void OffsetToHitboxEdgeStart(void *destArg, s32 kind, void *recArg)
{
    struct vec2 *dest = (struct vec2 *)destArg;
    struct hitbox_quad *rec = (struct hitbox_quad *)recArg;

    switch (kind) {
    case 2:
        dest->x -= rec->w << 7;
        dest->y += INT_TO_Q8(rec->offY);
        break;
    case 1:
        dest->x += rec->w << 7;
        dest->y += INT_TO_Q8(rec->offY);
        break;
    case 4:
        dest->y += INT_TO_Q8(rec->offY);
        dest->x -= rec->w << 7;
        break;
    case 8:
    case 12:
        dest->y += INT_TO_Q8(rec->offY + rec->h);
        dest->x -= rec->w << 7;
        break;
    }
}

/* Always inside in screen space; otherwise Entity's test. */
s32 Sprite::IsInsideRect(struct aabb *box)
{
    s32 result = 0;

    if (screenSpace == 1)
        result = 1;
    else if ((u8)Entity::IsInsideRect(box))
        result = 1;
    return result;
}

/* Always near in screen space; otherwise Entity's test. */
u8 Sprite::IsNearCamera()
{
    s32 result = 0;

    if (screenSpace == 1)
        result = 1;
    else if (Entity::IsNearCamera())
        result = 1;
    return result;
}

s32 Sprite::ApplyVelocity()
{
    return 1;
}

/* Draws the sprite with the renderer (gSpriteRenderer). */
void Sprite::Draw()
{
    gSpriteRenderer->Draw(this);
}

/* One frame: the animation timer, then the motion (ApplyVelocity), then
 * the contact with the player (CheckPlayerContact). */
void Sprite::Update()
{
    AdvanceAnim();
    ApplyVelocity();
    CheckPlayerContact();
}

/* The current animation's hitbox record, box[0]. */
const struct hitbox_quad *Sprite::GetBounds()
{
    return &bank->anims[tag].box[0];
}

/* The sprite tile pool (the bank table's `tileBase`). */
s32 Sprite::GetTileBase()
{
    return (s32)gSpriteBankSet->table->tileBase;
}

/* The current frame: the animation's `seq` entry for the current step,
 * as an index into the bank's `frames`. When a non-looping animation is
 * done, it holds on the last step. */
const struct sprite_frame *Sprite::GetFrame()
{
    const struct sprite_anim *rec = &bank->anims[tag];

    if (animDone != 0 && !(rec->flags & SPRITE_ANIM_LOOP)) {
        frame = rec->frameCount - 1;
        stepTimer = rec->duration;
    }
    return bank->frames[rec->seq[frame]];
}

/* The OBJ priority: layer 0's BG priority (BGnCNT bits 0-1), one higher
 * when gLevelLayers->raiseObjPriority is set (SetupRoomBlend, blend mode
 * 1). */
s32 Sprite::GetPriority()
{
    if (gLevelLayers->raiseObjPriority == 0)
        return gLevelLayers->layer0->cnt.bits.priority;
    else
        return gLevelLayers->layer0->cnt.bits.priority - 1;
}

/* A new sprite object at the spawn's position. */
Sprite *Sprite::Create(u16 id, u16 x, u16 y, u16)
{
    return new Sprite(id, x, y);
}

s32 Sprite::GetClassId()
{
    return 1;
}

Sprite::~Sprite()
{
}

Sprite::Sprite()
{
    Reset();
}

/* The current frame's anchor point (layout types 0 and 6), or none. */
const struct sprite_point *Sprite::GetFrameAnchor()
{
    const struct sprite_frame *info = GetFrame();
    const struct sprite_point *result;

    switch ((u8)(info->pieces[0] >> 4)) {
    case 0:
        result = &((const struct sprite_frame_3box_anchor *)info)->anchor;
        break;
    case 3:
    case 4:
        result = &gEmptySpritePoint;
        break;
    case 1:
    case 2:
        result = &gEmptySpritePoint;
        break;
    case 5:
        result = &gEmptySpritePoint;
        break;
    case 6:
        result = &((const struct sprite_frame_1box_anchor *)info)->anchor;
        break;
    default:
        result = &gEmptySpritePoint;
        break;
    }
    return result;
}

/* The current frame's third box (layout types 0 and 4), or none. */
const struct hitbox_quad *Sprite::GetFrameThirdBox()
{
    const struct sprite_frame *info = GetFrame();
    const struct hitbox_quad *result;

    switch ((u8)(info->pieces[0] >> 4)) {
    case 0:
        result = &((const struct sprite_frame_3box *)info)->box[2];
        break;
    case 1:
    case 2:
    case 3:
        result = &gEmptySpriteBox;
        break;
    case 4:
        result = &((const struct sprite_frame_3box *)info)->box[2];
        break;
    case 5:
    case 6:
        result = &gEmptySpriteBox;
        break;
    default:
        result = &gEmptySpriteBox;
        break;
    }
    return result;
}

/* The current frame's attack box record: box[1] (types 0, 3, 4), box[0]
 * (type 5), or none. */
const struct hitbox_quad *Sprite::GetFrameAttackBox()
{
    const struct sprite_frame *info = GetFrame();
    const struct hitbox_quad *result;

    switch ((u8)(info->pieces[0] >> 4)) {
    case 0:
    case 3:
    case 4:
        result = &((const struct sprite_frame_3box *)info)->box[1];
        break;
    case 1:
    case 2:
    case 6:
        result = &gEmptySpriteBox;
        break;
    case 5:
        result = &((const struct sprite_frame_1box *)info)->box[0];
        break;
    default:
        result = &gEmptySpriteBox;
        break;
    }
    return result;
}

/* The current frame's body box record: box[0] (types 0, 2, 3, 4, 6), or
 * none. */
const struct hitbox_quad *Sprite::GetFrameBodyBox()
{
    const struct sprite_frame *info = GetFrame();
    const struct hitbox_quad *result;

    switch ((u8)(info->pieces[0] >> 4)) {
    case 0:
        result = &((const struct sprite_frame_1box *)info)->box[0];
        break;
    case 1:
        result = &gEmptySpriteBox;
        break;
    case 2:
    case 3:
    case 4:
        result = &((const struct sprite_frame_1box *)info)->box[0];
        break;
    case 5:
        result = &gEmptySpriteBox;
        break;
    case 6:
        result = &((const struct sprite_frame_1box *)info)->box[0];
        break;
    default:
        result = &gEmptySpriteBox;
        break;
    }
    return result;
}

/* The current animation's record. */
const struct sprite_anim *Sprite::GetAnim()
{
    return &bank->anims[tag];
}

/* Sets the step, clamped to the animation's last one. */
void Sprite::SetFrameIndex(s32 step)
{
    const struct sprite_bank *b = bank; // loaded first, as in the ROM
    u8 n = b->anims[tag].frameCount;

    CLAMP_INDEX(step, n);
    frame = step;
}

u8 Sprite::GetScreenSpace()
{
    return screenSpace;
}

void Sprite::SetScreenSpace(u8 value)
{
    screenSpace = value;
}

s32 Sprite::IsHidden()
{
    return (f.bytes.flags2 >> 2) & 1;
}

void Sprite::ToggleHidden()
{
    f.b.blink = !f.b.blink;
}

s32 Sprite::IsSolid()
{
    return (f.bytes.flags2 >> 3) & 1;
}

void Sprite::ClearSolid()
{
    f.b.solid = 0;
}

void Sprite::SetSolid()
{
    f.b.solid = 1;
}

s32 Sprite::IsVulnerable()
{
    return (f.flags >> 6) & 1;
}

void Sprite::ClearVulnerable()
{
    f.b.vulnerable = 0;
}

void Sprite::SetVulnerable()
{
    f.b.vulnerable = 1;
}

void Sprite::ResetAnimIndex()
{
    tag = 0;
}

s32 Sprite::IsCollisionEnabled()
{
    return f.flags >> 7;
}

void Sprite::DisableCollision()
{
    f.b.collides = 0;
}

void Sprite::EnableCollision()
{
    f.b.collides = 1;
}

u8 Sprite::GetAnimating()
{
    return animating;
}

void Sprite::SetAnimating(u8 value)
{
    animating = value;
}

void Sprite::SetFlipX(u8 value)
{
    mirrorBits.flipX = value;
}

void Sprite::SetFlipY(u8 value)
{
    mirrorBits.flipY = value;
}

void Sprite::SetAnimDone(u8 value)
{
    animDone = value;
}

u8 Sprite::GetAnimPaletteId()
{
    return bank->anims[tag].paletteId;
}

s32 Sprite::GetPalette()
{
    return palette;
}

void Sprite::SetPalette(s32 value)
{
    palette = value;
}

void Sprite::SetBank(void *value)
{
    bank = (const struct sprite_bank *)value;
}

void *Sprite::GetBank()
{
    return (void *)bank;
}

u8 Sprite::IsAnimLooping()
{
    return bank->anims[tag].flags & SPRITE_ANIM_LOOP;
}

/* The rest of Sprite's accessors (#664, include/sprite_obj.hpp; #771:
 * merged from sprite_anim.cpp). UiSprite follows in ui_sprite.cpp. */

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
