#include "bg_layer.hpp"
#include "sprite_obj.hpp"
#include "spawners.hpp"
#include "player.hpp"

extern "C" {
#include "math_util.h"
#include "match.h"
#include "util.h"
#include "gfx.h"
#include "memory.h"
#include "level.h"
#include "globals.h"
#include "player.h"
}

/* The sprite renderer and the first of Sprite's methods (#664,
 * include/sprite_obj.hpp). An old_agbcp object (OLD_AGBCC_OBJS). */

/* `screenSpace` says whether (x, y) are already screen coordinates;
 * otherwise WorldToScreen makes them camera-relative. DrawPieces
 * builds and queues the part's OAM entries at the resolved position. */
void SpriteRenderer::DrawAt(Sprite *part, s32 x, s32 y)
{
    s32 pos[2];

    if (part->screenSpace == 0) {
        WorldToScreen(part, x, y, &pos[0], &pos[1]);
    } else {
        pos[0] = x;
        pos[1] = y;
    }
    DrawPieces(part, pos);
}

/* Draws `part` at its own position. */
void SpriteRenderer::Draw(Sprite *part)
{
    DrawAt(part, Q8_TO_INT(part->x), Q8_TO_INT(part->y));
}

SpriteRenderer::~SpriteRenderer()
{
}

SpriteRenderer::SpriteRenderer()
{
}

/* Clears the animation state (`bank`, `tag`, `frame`, `stepTimer`,
 * `animDone`; `animating` is set), the mirror and palette bytes, `affine`,
 * `dir` and `screenSpace`, and the `vulnerable`, `collides` and `blink`
 * flags. */
void Sprite::Reset()
{
    f.b.collides = 0;
    f.b.vulnerable = 0;
    screenSpace = 0;
    anim = 0;
    tag = 0;
    frame = 0;
    stepTimer = 0;
    *(u16 *)&mirror = 0; // the mirror and palette bytes, as one halfword
    animDone = 0;
    f.b.blink = 0;
    dir = 0;
    affine = 0;
    animating = 1;
}

/* The current animation's box[1] at the sprite's position, mirrored
 * around it per the mirror bits. */
struct aabb Sprite::GetAnimBounds()
{
    struct aabb box;
    const struct hitbox_quad *pb = &bank->anims[tag].box[1];
    s32 px = Q8_TO_INT(x);
    s32 offX = pb->offX;
    s32 py = Q8_TO_INT(y);
    s32 offY = pb->offY;
    u8 w = pb->w;
    u8 h = pb->h;

    SetAabbPos(&box, offX + px, offY + py);
    SetAabbSize(&box, w, h);
    if (mirrorBits.flipX < 0)
        box.x = Q8_TO_INT(x) * 2 - (box.x + box.w);
    if (mirrorBits.flipY < 0)
        box.y = Q8_TO_INT(y) * 2 - (box.y + box.h);
    return box;
}

/* The same with the animation's box[0]. */
struct aabb Sprite::GetAnimHitbox()
{
    struct aabb box;
    const struct hitbox_quad *pb = &bank->anims[tag].box[0];
    s32 px = Q8_TO_INT(x);
    s32 offX = pb->offX;
    s32 py = Q8_TO_INT(y);
    s32 offY = pb->offY;
    u8 w = pb->w;
    u8 h = pb->h;

    SetAabbPos(&box, offX + px, offY + py);
    SetAabbSize(&box, w, h);
    if (mirrorBits.flipX < 0)
        box.x = Q8_TO_INT(x) * 2 - (box.x + box.w);
    if (mirrorBits.flipY < 0)
        box.y = Q8_TO_INT(y) * 2 - (box.y + box.h);
    return box;
}

/* The current frame's attack box (GetSpriteFrame; the layout type in the
 * high nibble of its first piece byte picks the record) at the sprite's
 * position, mirrored. */
struct aabb Sprite::GetAttackBox()
{
    struct aabb box;
    const struct sprite_frame_3box *info;
    const struct hitbox_quad *rec;
    s32 offX, offY;
    s32 w, h;
    s32 px, py;

    info = (const struct sprite_frame_3box *)GetFrame();
    switch ((u8)(info->frame.pieces[0] >> 4)) {
    case 0:
    case 3:
    case 4:
        rec = &info->box[1];
        break;
    case 1:
    case 2:
    case 6:
        rec = &gEmptySpriteBox;
        break;
    case 5:
        rec = &info->box[0];
        break;
    default:
        rec = &gEmptySpriteBox;
        break;
    }

    px = Q8_TO_INT(x);
    offX = rec->offX;
    py = Q8_TO_INT(y);
    offY = rec->offY;
    w = rec->w;
    h = rec->h;

    SetAabbPos(&box, offX + px, offY + py);
    SetAabbSize(&box, w, h);
    if (mirrorBits.flipX < 0)
        box.x = Q8_TO_INT(x) * 2 - (box.x + box.w);
    if (mirrorBits.flipY < 0)
        box.y = Q8_TO_INT(y) * 2 - (box.y + box.h);
    return box;
}

/* The current frame's body box, the same way: box[0], or none. */
struct aabb Sprite::GetBodyBox()
{
    struct aabb box;
    const struct sprite_frame_3box *info;
    const struct hitbox_quad *rec;
    s32 offX, offY;
    s32 w, h;
    s32 px, py;

    info = (const struct sprite_frame_3box *)GetFrame();
    switch ((u8)(info->frame.pieces[0] >> 4)) {
    case 0:
    case 2:
    case 3:
    case 4:
    case 6:
        rec = &info->box[0];
        break;
    case 1:
    case 5:
        rec = &gEmptySpriteBox;
        break;
    default:
        rec = &gEmptySpriteBox;
        break;
    }

    px = Q8_TO_INT(x);
    offX = rec->offX;
    py = Q8_TO_INT(y);
    offY = rec->offY;
    w = rec->w;
    h = rec->h;

    SetAabbPos(&box, offX + px, offY + py);
    SetAabbSize(&box, w, h);
    if (mirrorBits.flipX < 0)
        box.x = Q8_TO_INT(x) * 2 - (box.x + box.w);
    if (mirrorBits.flipY < 0)
        box.y = Q8_TO_INT(y) * 2 - (box.y + box.h);
    return box;
}

/* The pickup's collect effect: an effect part (SpawnEffectPart, sprite
 * bank 0x2b) of `kind` at (x, y). */
static inline Sprite *SpawnPickupEffect(s32 kind, s32 x, s32 y)
{
    return gEntitySpawner->SpawnEffectPart(0x2b, kind, x, y, 0);
}

/* A pickup's contact with the player: unless it was already touched,
 * and while it is in contact, its hitbox against the player's. On a
 * touch it tells the player (the player's HandleEvent with its kind),
 * marks itself gone, and spawns the effect of its kind (0x1b-0x22) at its
 * position, semi-transparent and out of contact. */
s32 Sprite::CheckPlayerContact()
{
    Sprite *spawned;
    u32 flags = f.flags << 24;

    /* The 1 that the two tests and the gone flag share is pinned: written
     * as constants, old_agbcp loads a fresh 1 for the flag's OR and gives
     * the tests' register to the gone bit's shift instead, the other way
     * round from the ROM. */
    MATCH_HOLD_REG(u32, one, r6);
    s32 touched = (flags >> 27) & (one = 1);

    if (!touched && ((flags >> 26) & one)) {
        struct aabb a = GetAnimHitbox();

        if (gPlayer->f.flags >> 7) {
            struct aabb b = gPlayer->GetAnimHitbox();

            if (AabbOverlaps(&b, &a)) {
                f.b.bit3 = 1;
                gPlayer->HandleEvent(0, kind, 0);
                f.flags |= one;
                if (id != ENTITY_ID_NONE)
                    ENTITY_SET_GONE_BIT(id);

                spawned = 0;
                switch (kind) {
                case 0x1d:
                case 0x1e:
                    spawned = SpawnPickupEffect(1, Q8_TO_INT(x), Q8_TO_INT(y));
                    break;
                case 0x21:
                    spawned = SpawnPickupEffect(6, Q8_TO_INT(x), Q8_TO_INT(y));
                    break;
                case 0x1f:
                    spawned = SpawnPickupEffect(5, Q8_TO_INT(x), Q8_TO_INT(y));
                    break;
                case 0x22:
                    spawned = SpawnPickupEffect(0, Q8_TO_INT(x), Q8_TO_INT(y));
                    break;
                case 0x20:
                    spawned = SpawnPickupEffect(3, Q8_TO_INT(x), Q8_TO_INT(y));
                    break;
                case 0x1b:
                    spawned = SpawnPickupEffect(4, Q8_TO_INT(x), Q8_TO_INT(y));
                    break;
                }
                if (spawned) {
                    u32 mode = 1; // materialized before the mask, as in the ROM

                    spawned->mirrorBits.gfxMode = mode;
                    spawned->f.b.visible = 0;
                }
            }
        }
    }
    return 0;
}

/* Always on screen in screen space; otherwise unless blinking, whether
 * it overlaps the screen (layer 0's position, 240x160, in Q8). */
u8 Sprite::IsOnScreen()
{
    s32 result = 0;

    if (screenSpace == 1)
        return 1;
    s32 hidden = (f.bytes.flags2 >> 2) & 1;

    if (!hidden) {
        struct aabb screen;
        BgLayer *cam = gLevelLayers->layer0;
        s32 cx = INT_TO_Q8(cam->x);
        s32 cy = INT_TO_Q8(cam->y);
        s32 w, h;

        screen.x = cx;
        screen.y = cy;
        w = INT_TO_Q8(0xf0);
        h = INT_TO_Q8(0xa0);
        screen.w = w;
        screen.h = h;
        result = (u8)OverlapsRect(&screen);
    }
    return result;
}

/* Whether the animation's bounds (GetAnimBounds, in Q8) overlap
 * `region`: always in screen space, never while blinking. */
s32 Sprite::OverlapsRect(struct aabb *region)
{
    s32 result = 0;

    if (screenSpace == 1)
        return 1;
    s32 hidden = (f.bytes.flags2 >> 2) & 1;

    if (!hidden) {
        struct aabb box = GetAnimBounds();
        s32 x1 = INT_TO_Q8(box.x);
        s32 y1 = INT_TO_Q8(box.y);
        s32 x2 = x1 + INT_TO_Q8(box.w);
        s32 y2 = y1 + INT_TO_Q8(box.h);
        s32 in = 0;

        if (x2 > region->x && x1 < region->x + region->w && y2 > region->y &&
            y1 < region->y + region->h)
            in = 1;
        result = in;
    }
    return result;
}

/* One tick of the animation timer while `animating`: `stepTimer` counts
 * up to the animation's `duration`, then the next step; after the last
 * step it starts over, and unless the animation loops, `animDone` is
 * set. */
void Sprite::AdvanceAnim()
{
    if (animating) {
        s32 timer = stepTimer;
        s32 step;
        const struct sprite_anim *anims;

        if (timer < bank->anims[tag].duration)
            stepTimer = timer + 1;
        else {
            stepTimer = 0;
            frame++;
        }

        step = frame;
        anims = bank->anims;
        if (step >= anims[tag].frameCount) {
            frame = 0;
            stepTimer = 0;
            if (!(anims[tag].flags & SPRITE_ANIM_LOOP)) {
                u8 done = TRUE;

                animDone = done;
            }
        }
    }
}
