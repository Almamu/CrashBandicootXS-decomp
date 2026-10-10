/* The entity base class (Entity, gEntityVtable; include/entity.hpp), with
 * WorldToScreen, WorldPosToScreen and nullsub_12 between its methods. Its
 * key method, CheckPlayerContact, is here, so g++ emits the vtable and, at
 * the end of the file, the out-of-line copies of Entity's inline methods
 * (DestroyEntity last). Split from gfx/graphics.cpp (#767), same flags
 * (old_agbcc). */

#include "bg_layer.hpp"
#include "entity.hpp"
#include "sprite_obj.hpp"
#include "player.hpp"

extern "C" {
#include "math_util.h"
#include "gfx.h"
#include "util.h"
#include "player.h"
#include "level.h"
#include "globals.h"
}

/* Unless always active, whether its IsInsideRect a box around the camera
 * (layer 0's position less 100/60 px, 440x280, in Q8). */
u8 Entity::IsNearCamera()
{
    u8 result = (f.flags >> 4) & 1;

    if (!result) {
        struct aabb near;
        BgLayer *layer;
        s32 w = INT_TO_Q8(440);
        s32 h = INT_TO_Q8(280);

        near.w = w;
        near.h = h;
        layer = gLevelLayers->layer0;
        s32 nx = INT_TO_Q8(layer->x) - INT_TO_Q8(100);
        s32 ny = INT_TO_Q8(layer->y) - INT_TO_Q8(60);

        near.x = nx;
        near.y = ny;
        result = IsInsideRect(&near);
    }
    return result;
}

/* While in contact, its bounds (GetBounds) at its position against the
 * player's (PlayerTouchesBox): on a touch it is marked touched and tells
 * the player (HandleEvent with its kind). */
s32 Entity::CheckPlayerContact()
{
    const struct hitbox_quad *rec = GetBounds();
    struct aabb box;
    s32 bx = Q8_TO_INT(x);
    s32 offX = rec->offX;
    s32 by = Q8_TO_INT(y);
    s32 offY = rec->offY;
    u8 w = rec->w;
    u8 h = rec->h;

    SetAabbPos(&box, bx + offX, by + offY);
    SetAabbSize(&box, w, h);
    s32 inContact = (f.flags >> 2) & 1;

    if (inContact) {
        if (gPlayer->TouchesBox(&box)) {
            f.b.bit3 = 1;
            gPlayer->HandleEvent(0, kind, 0);
        }
    }
    return 0;
}

void Entity::Draw()
{
}

void Entity::Update()
{
    CheckPlayerContact();
}

/* Its size as a box around its position (halfW, halfH, rawW, rawH). */
const struct hitbox_quad *Entity::GetBounds()
{
    return (const struct hitbox_quad *)&halfW;
}

/* `w`/`h` and -w/2, -h/2 (the offsets of a centred box). */
void Entity::SetSize(s32 w, s32 h)
{
    halfW = -w / 2;
    halfH = -h / 2;
    rawW = w;
    rawH = h;
}

s32 Entity::OverlapsRect(struct aabb *)
{
    return 0;
}

u8 Entity::IsOnScreen()
{
    return 0;
}

/* Unless always active, whether its bounds (GetBounds, centred on its
 * position) are inside `box`. */
s32 Entity::IsInsideRect(struct aabb *box)
{
    u8 result = (f.flags >> 4) & 1;

    if (!result) {
        const struct hitbox_quad *rec = GetBounds();
        s32 hw = rec->w << 7;
        s32 hh = rec->h << 7;
        s32 x0 = x - hw;
        s32 y0 = y - hh;
        s32 x1 = x + hw;
        s32 y1 = y + hh;
        s32 inside = 0;

        if (x0 > box->x && x1 < box->x + box->w && y0 > box->y && y1 < box->y + box->h)
            inside = 1;
        result = inside;
    }
    return result;
}

/* (x, y) relative to the camera (layer 0's position, sign-extended from
 * its low 24 bits). `unused` is not read. */
void WorldToScreen(void *unused, s32 x, s32 y, s32 *outX, s32 *outY)
{
    BgLayer *layer = gLevelLayers->layer0;
    s32 dx = (layer->x << 8) >> 8;
    s32 dy = (layer->y << 8) >> 8;

    *outX = x - dx;
    *outY = y - dy;
}

/* A Q8 position (rounded) relative to the camera, in pixels. */
void WorldPosToScreen(s32 *pos, s32 *outX, s32 *outY)
{
    BgLayer *layer;
    s32 x = pos[0];
    s32 y = pos[1];

    if (x & 0x80)
        x += 0x80;
    if (y & 0x80)
        y += 0x80;
    layer = gLevelLayers->layer0;
    s32 cx = INT_TO_Q8(layer->x);
    s32 cy = INT_TO_Q8(layer->y);

    *outX = Q8_TO_INT(x - cx);
    *outY = Q8_TO_INT(y - cy);
}

/* UNUSED - empty, no caller anywhere in the ROM (checked src/, asm/,
 * expected/ and every word-aligned Thumb pointer in baserom.gba). Keeps
 * the nullsub_N name (docs/naming.md): no call, table slot or neighbour
 * shows what it stood for. */
void nullsub_12(void)
{
}

/* The constructor, inline here only: Create inlines it, and the other
 * classes' constructors call it (InitEntity). Its out-of-line copy is the
 * first of the inline methods' at the end of the file. */
inline Entity::Entity()
{
    Reset();
}

/* A new entity at the spawn's position. */
Entity *Entity::Create(u16 id, u16 x, u16 y, u16)
{
    Entity *e = new Entity;

    e->id = id;
    e->x = INT_TO_Q8((s32)x);
    e->y = INT_TO_Q8((s32)y);
    return e;
}

s32 Entity::GetClassId()
{
    return 0;
}

/* Out of contact, no longer touched or always active, the `attacks` bit and
 * gone cleared, contact enabled; a 1x1 size. */
void Entity::Reset()
{
    f.b.attacks = 0;
    f.b.visible = 1;
    f.b.gone = 0;
    f.b.bit3 = 0;
    f.b.active = 0;
    halfW = 0;
    halfH = 0;
    rawW = 1;
    rawH = 1;
}
