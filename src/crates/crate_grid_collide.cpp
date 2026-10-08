#include "bg_layer.hpp"
#include "crate_list.hpp"
#include "player.hpp"
#include "audio.hpp"

extern "C" {
#include "math_util.h"
#include "level.h"
#include "globals.h"
#include "player.h"
}

/* CrateList::Collide and CollideWithPlayer (#664, part 7f;
 * include/crate_list.hpp), PartList's Collide and CollideWithPlayer
 * (sprite_anim.cpp) for the grid. An old_agbcp object (OLD_AGBCC_OBJS).
 * The three take the sprites as MovingSprites, as PartList's do (they
 * call HandleEvent, slot 13); the crate list holds crates, which have no
 * slot 13, but nothing calls Collide (below). */

/* Collide's test of one sprite, for the columns and for column 255: a
 * sprite on screen that is in contact with the player (flags bit 2) and
 * of a class above 4 collides with `other`, against `box`. */
static inline void CollidePart(CrateList *list, MovingSprite *part, struct aabb *screen,
                               struct aabb &box, MovingSprite *other)
{
    if ((u8)part->OverlapsRect(screen) && ((part->f.flags >> 2) & 1)) {
        if (part->GetClassId() > 4) {
            if (other == gPlayer)
                list->CollideWithPlayer(box, part);
            else
                list->CollideWithObject(box, part, other);
        }
    }
}

/* PartList::Collide's counterpart: the sprites on screen in the camera's
 * column and the two to its right, then column 255, collide with `other`
 * (the player, or another object) against `box`. A sprite in both is
 * tested twice. UNUSED - no caller anywhere in the ROM (no call in src/,
 * no pointer to 0x08009528 in the ROM): the crates collide with the
 * player through CollidePlayer (crate_player_collide.cpp), and
 * CollideWithPlayer and CollideWithObject have no other caller. */
void CrateList::Collide(struct aabb box, s32 unused, MovingSprite *other)
{
    struct aabb screen;
    BgLayer *cam;
    s32 lo;
    s32 i;
    CrateGridNode *node;
    CrateGridNode **last;
    CrateGridNode **columns;

    cam = gLevelLayers->layer0;
    {
        s32 x = INT_TO_Q8(cam->x);
        s32 y = INT_TO_Q8(cam->y);
        screen.x = x;
        screen.y = y;
    }
    {
        s32 w = INT_TO_Q8(240);
        s32 h = INT_TO_Q8(160);
        screen.w = w;
        screen.h = h;
    }
    lo = cam->x;
    lo >>= 8;
    LIMIT_MIN(lo, 0);
    i = lo + 2;
    columns = heads;
    last = &heads[255];
    do {
        for (node = columns[i]; node != 0; node = node->next)
            CollidePart(this, (MovingSprite *)node->data, &screen, box, other);
        i--;
    } while (i >= lo);
    for (node = *last; node != 0; node = node->next)
        CollidePart(this, (MovingSprite *)node->data, &screen, box, other);
}

/* PartList::CollideWithPlayer's twin (sprite_anim.cpp), the same code:
 * a hit between `part` and the player, against the box Collide passed. */
void CrateList::CollideWithPlayer(struct aabb box, MovingSprite *part)
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
