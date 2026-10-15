#include "game/level/bg_layer.hpp"
#include "game/crates/crate_list.hpp"
#include "game/crash/player.hpp"
#include "engine/audio/audio.hpp"
#include "game/level/level_state.hpp"
#include "game/crates/crate.hpp"

extern "C" {
#include "engine/util/math_util.h"
#include "game/level/level.h"
#include "globals.h"
#include "game/entities/objects.h"
}

using Game::Crash::Player;
using Game::Crates::CrateList;
using Game::Entities::MovingSprite;
using Game::Level::BgLayer;

/* CrateList::Collide and CollideWithPlayer (#664, part 7f;
 * engine/include/game/crates/crate_list.hpp), PartList's Collide and CollideWithPlayer
 * (part_list_collide.cpp) for the grid. An old_agbcp object (OLD_AGBCC_OBJS).
 * The three take the sprites as MovingSprites, as PartList's do (they
 * call HandleEvent, slot 13); the crate list holds crates, which have no
 * slot 13, but nothing calls Collide (below). */

/* Collide's test of one sprite, for the columns and for column 255: a
 * sprite on screen that is in contact with the player (flags bit 2) and
 * of a class above 4 collides with `other`, against `box`. */
static inline void CollidePart(CrateList *list, MovingSprite *part, struct aabb *screen,
                               struct aabb &box, MovingSprite *other)
{
    if ((u8)part->OverlapsRect(screen) && part->IsContactEnabled() && part->GetClassId() > 4) {
        if (other == gPlayer)
            list->CollideWithPlayer(box, part);
        else
            list->CollideWithObject(box, part, other);
    }
}

/* PartList::Collide's counterpart: the sprites on screen in the camera's
 * column and the two to its right, then column 255, collide with `other`
 * (the player, or another object) against `box`. A sprite in both is
 * tested twice. UNUSED - no caller anywhere in the ROM (no call in engine/src/,
 * no pointer to 0x08009528 in the ROM): the crates collide with the
 * player through CollidePlayer (below), and
 * CollideWithPlayer and CollideWithObject have no other caller. */
void CrateList::Collide(struct aabb box, s32 unused, MovingSprite *other)
{
    struct aabb screen;
    BgLayer *cam;
    s32 lo;
    s32 i;
    CrateGridNode *node;
    /* `last` is needed: inlined, the code is 4 instructions shorter */
    CrateGridNode **last;
    /* `columns` is needed: inlined, the instructions are reordered */
    CrateGridNode **columns;

    cam = gLevelLayers->layer0;

    /* `x`, `y` are needed: inlined, the instructions are reordered */
    s32 x = INT_TO_Q8(cam->x);
    s32 y = INT_TO_Q8(cam->y);
    screen.x = x;
    screen.y = y;

    /* `w`, `h` are needed: inlined, the instructions are reordered */
    s32 w = INT_TO_Q8(DISPLAY_WIDTH);
    s32 h = INT_TO_Q8(DISPLAY_HEIGHT);
    screen.w = w;
    screen.h = h;
    lo = cam->x;
    lo >>= 8;
    LIMIT_MIN(lo, 0);
    i = lo + CRATE_GRID_SPAN;
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

/* PartList::CollideWithPlayer's twin (part_list_collide.cpp), the same code:
 * a hit between `part` and the player, against the box Collide passed. */
void CrateList::CollideWithPlayer(struct aabb box, MovingSprite *part)
{
    if (gLevelState->maskLevel == MASK_LEVEL_INVINCIBLE) {
        if (!part->ClassifyContact(&box))
            return;
        part->HandleEvent(1, gPlayer->kind, 0);
    } else if ((part->f.bytes.flags2 >> ENTITY_BIT2_SOLID) & 1) {
        struct aabb a = gPlayer->GetAnimHitbox();
        struct aabb b = part->GetBodyBox();
        s32 px;

        if (!AabbOverlaps(&a, &b))
            return;
        px = part->x;
        if (px < gPlayer->x) {
            gPlayer->x = px + ((b.w + a.w) << 7);
            gPlayer->HandleEvent(0, EVENT_BUMP, PART_DIR_LEFT);
        } else {
            gPlayer->x = px - ((b.w + a.w) << 7);
            gPlayer->HandleEvent(0, EVENT_BUMP, PART_DIR_RIGHT);
        }
    } else {
        u8 kind;

        switch (part->ClassifyContact(&box)) {
        case 0:
            break;
        case 1:
            gPlayer->f.b.bit3 = 1;
            kind = gPlayer->kind;
            if (kind == EVENT_HIT) {
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
            part->f.flags |= ENTITY_FLAG_TOUCHED;
            if (gLevelState->maskLevel)
                part->HandleEvent(1, EVENT_HIT, 0);
            gPlayer->HandleEvent(1, part->kind, 0);
            break;
        }
    }
}

/* The player's pass over the crate grid (#664, part 7f;
 * engine/include/game/crates/crate_list.hpp). */

/* For the crates in the grid buckets from the camera's column to
 * CRATE_GRID_SPAN right of it
 * (as UpdateCrateList's window): a hovering player (PLAYER_CTRL_MODE_INPUT) breaks the
 * ones it touches (BreakCrateTouchedByPlayer); otherwise each collides
 * with the player (CollideCrateWithPlayer), with the player's action (its
 * controller's state; idle underwater, PLAYER_CTRL_MODE_SWIM, or the spin
 * for a player whose `kind` is EVENT_ATTACK_SPIN). Old_agbcc C (see docs/matching/archive/issue-9-naked-retry.md):
 * the camera x is read before the `>> 8`. */
void CrateList::CollidePlayer(s32 unused)
{
    s32 lo = gLevelLayers->layer0->x;
    s32 i;
    Player *p;
    u8 mode;

    lo >>= 8;
    LIMIT_MIN(lo, 0);
    i = lo + CRATE_GRID_SPAN;
    p = gPlayer;
    mode = p->ctrlMode;
    if (mode == PLAYER_CTRL_MODE_INPUT) {
        do {
            CrateGridNode *node;

            for (node = heads[i]; node != 0; node = node->next)
                node->data->BreakIfTouchedByPlayer();
            i--;
        } while (i >= lo);
    } else {
        s32 action = p->mover->state;
        /* `px`, `py` are needed: inlined, the code is 1 instruction longer */
        s32 px = p->x;
        s32 py = p->y;

        if (mode == PLAYER_CTRL_MODE_SWIM) {
            action = ACTION_STATE_IDLE;
            if (p->kind == EVENT_ATTACK_SPIN)
                action = ACTION_STATE_SPIN;
        }
        do {
            CrateGridNode *node;

            for (node = heads[i]; node != 0; node = node->next)
                node->data->CollideWithPlayer(action, px, py);
            i--;
        } while (i >= lo);
    }
}
