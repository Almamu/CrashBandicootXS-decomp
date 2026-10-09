#include "bg_layer.hpp"
#include "part_list.hpp"
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

/* The part list's per-frame passes (#664, part 7c; include/part_list.hpp):
 * Update, Collide and CollideWithPlayer, then Cull, Clear and
 * CollideClass, and CollideWithObject. An old_agbcp object
 * (OLD_AGBCC_OBJS): CollideClass's flag test takes old_agbcp's registers.
 * (#771: part_list_cull.cpp and part_collide.cpp merged.) */

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
    BgLayer *cam;
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
        if (!part->ClassifyContact(&box))
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

        switch (part->ClassifyContact(&box)) {
        case 0:
            break;
        case 1:
            gPlayer->f.b.bit3 = 1;
            kind = gPlayer->kind;
            if (kind == 1) {
                if (gPlayer->speedY > 0) {
                    part->HandleEvent(1, EVENT_HIT, 0);
                    gPlayer->HandleEvent(0, EVENT_BOUNCE, 0);
                    gAudioContext->PlaySfx(SFX_BOUNCE, 0x100);
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

/* Fills `visible` with the parts that overlap the screen (the 240x160
 * GBA screen, in Q8, at `gLevelLayers->layer0`'s scroll position):
 * UpdatePartList's screen pass without its compaction. */
void PartList::Cull()
{
    s32 i;
    struct aabb screen;
    BgLayer *cam = gLevelLayers->layer0;

    {
        s32 cx = INT_TO_Q8(cam->x);
        s32 cy = INT_TO_Q8(cam->y);
        screen.x = cx;
        screen.y = cy;
    }
    {
        s32 w = INT_TO_Q8(240);
        s32 h = INT_TO_Q8(160);
        screen.w = w;
        screen.h = h;
    }
    visibleCount = 0;
    for (i = 0; i < count; i++) {
        Sprite *part = items[i];

        if ((u8)part->OverlapsRect(&screen))
            visible[visibleCount++] = part;
    }
}

/* Deletes every part and empties the list. */
void PartList::Clear()
{
    s32 i;

    for (i = 0; i < count; i++) {
        delete items[i];
        items[i] = 0;
    }
    count = 0;
    visibleCount = 0;
}

/* CheckPlayerContact on every part on screen of class `classId` that is
 * in contact with the player (flags bit 2). */
void PartList::CollideClass(s32 classId)
{
    s32 i;

    for (i = 0; i < visibleCount; i++) {
        Sprite *part = visible[i];

        if (part->GetClassId() != classId)
            continue;
        s32 inContact = (part->f.flags >> 2) & 1;

        if (!inContact)
            continue;
        part->CheckPlayerContact();
    }
}

/* CollideWithPlayer's counterpart for a list collided with another
 * object (CollidePartList): when `part` touches the box
 * (ClassifySpriteContact), its HandleEvent with `other`'s kind, and
 * `other` is marked touched. */
void PartList::CollideWithObject(struct aabb box, MovingSprite *part, MovingSprite *other)
{
    if (part->ClassifyContact(&box)) {
        part->HandleEvent(1, other->kind, 0);
        other->f.b.bit3 = 1;
    }
}
