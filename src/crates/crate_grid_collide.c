#include "core.h"
#include "box_part.h"
#include "actor_self.h"
#include "util.h"
#include "system.h"
#include "audio.h"
#include "crates.h"
#include "objects.h"
#include "level.h"
#include "globals.h"
#include "player.h"

extern s32 _call_via_r2(void *self, void *arg, void *fn);
extern s32 _call_via_r1(void *self, void *fn);

/* The spatial-hash-grid-cluster analog of `CollidePartList`: the same
 * "extended screen box" filter shape as `DrawCrateList` (iterating
 * `manager`'s grid buckets from `baseIdx+2` down to 0, then the
 * special "large object" bucket 255), but instead of firing a simple
 * action trampoline on a box hit, additionally tests `part->flags`
 * bit 2 and fires a `table+0x48/0x4c`-driven trampoline via
 * `_call_via_r1`; if that result is greater than 4, reconstructs the
 * caller's original `{boxX, boxY, boxW, boxH}` box (via
 * `MemCopy32`, the same "unavoidable extra `boxH` load" idiom
 * established for `CollidePartList`) and dispatches to `CollideCrateGridPartWithPlayer`
 * (when `compareViewport` is the player, `gPlayer`) or
 * `CollideCrateGridPartWithObject` (otherwise) - the exact same dispatch `CollidePartList`
 * makes to `CollidePartWithPlayer`/`CollidePartWithObject`. See `CollidePartList`'s own
 * writeup (`sprite_anim.c`) for the full branch-by-branch semantics,
 * identical here.
 *
 * Matches under old_agbcc (see docs/matching/archive/issue-9-naked-retry.md).
 * The size gap the old draft had was the newer compiler plus the box
 * copy: the box arrives by value and is copied into one shared
 * temporary with MemCopy32 (memcpy) before each dispatch, exactly as
 * in CollidePartList. The duplicated per-node test is an inline helper;
 * `heads`/`last` (gridHead / &gridHead[255]) are computed up front, in
 * that order, as the ROM does. Its ROM address, 0x08009528, sits
 * between `DrawCrateList` (`crate_list_draw.c`) and
 * `CollideCrateGridPartWithPlayer` (below). */
static inline void CheckPart(struct pool_manager *m, struct box_part *part, struct aabb *screen,
                             struct aabb *box, struct aabb *tmp, struct box_part *other)
{
    struct part_method *m1 = PART_METHOD(part, 0x30);

    if ((u8)_call_via_r2((u8 *)part + m1->thisOffset, screen, m1->fn) && ((part->flags >> 2) & 1)) {
        struct part_method *m2 = PART_METHOD(part, 0x48);

        if (_call_via_r1((u8 *)part + m2->thisOffset, m2->fn) > 4) {
            if (other == (struct box_part *)gPlayer) {
                MemCopy32(tmp, box, sizeof(*tmp));
                CollideCrateGridPartWithPlayer((struct part_list *)m, *tmp, part);
            } else {
                MemCopy32(tmp, box, sizeof(*tmp));
                CollideCrateGridPartWithObject((struct part_list *)m, *tmp, part, other);
            }
        }
    }
}

void CollideCrateGrid(struct pool_manager *m, struct aabb box, s32 unused, struct box_part *other)
{
    struct aabb screen;
    struct aabb tmp;
    struct bg_scroll_layer *cam;
    s32 lo;
    s32 i;
    struct pool_node *node;
    struct pool_node **last;
    struct pool_node **heads;

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
    heads = m->gridHead;
    last = &m->gridHead[255];
    do {
        for (node = heads[i]; node != NULL; node = node->next)
            CheckPart(m, node->data, &screen, &box, &tmp, other);
        i--;
    } while (i >= lo);
    for (node = *last; node != NULL; node = node->next)
        CheckPart(m, node->data, &screen, &box, &tmp, other);
}


/* `CollidePartWithPlayer`'s twin, operating in this spatial-hash-grid cluster:
 * byte-identical collision-hit resolution logic (mode dispatch via
 * `gLevelState`, the AABB push-out via `GetSpriteHitbox`/
 * `GetSpriteBodyBox`/`AabbOverlaps`, and the "hit" method calls) - see
 * `CollidePartWithPlayer`'s own writeup in `sprite_anim.c` for the branch-by-branch
 * semantics, identical here. `list` itself is never read.
 *
 * Matches under old_agbcc with the box passed by value, the same C as
 * CollidePartWithPlayer (see docs/matching/archive/issue-9-naked-retry.md). Its
 * ROM address, 0x080096C0, sits between `CollideCrateGrid` (above) and
 * `CollidePlayerWithCrates` (`crate_player_collide.c`) in ROM order. */
void CollideCrateGridPartWithPlayer(struct part_list *list, struct aabb box, struct box_part *part)
{
    if (gLevelState->maskLevel == MASK_LEVEL_INVINCIBLE) {
        if (!ClassifySpriteContact(part, &box))
            return;
        CALL_HIT(part, 1, gPlayer->kind, 0);
    } else if ((part->flags2 >> 3) & 1) {
        struct aabb a, b;
        s32 px;

        a = GetSpriteHitbox((struct box_part *)gPlayer);
        GetSpriteBodyBox(&b, part);
        if (!AabbOverlaps(&a, &b))
            return;
        px = part->x;
        if (px < gPlayer->x) {
            gPlayer->x = px + ((b.w + a.w) << 7);
            CALL_HIT((struct box_part *)gPlayer, 0, EVENT_BUMP, 2);
        } else {
            gPlayer->x = px - ((b.w + a.w) << 7);
            CALL_HIT((struct box_part *)gPlayer, 0, EVENT_BUMP, 1);
        }
    } else {
        u8 kind;

        switch (ClassifySpriteContact(part, &box)) {
        case 0:
            break;
        case 1:
            {
                u8 *flags = &gPlayer->flags.all;
                *flags |= 8;
            }
            kind = gPlayer->kind;
            if (kind == 1) {
                if (gPlayer->speedY > 0) {
                    CALL_HIT(part, 1, EVENT_HIT, 0);
                    CALL_HIT((struct box_part *)gPlayer, 0, EVENT_BOUNCE, 0);
                    PlaySfx(gAudioContext, SFX_BOUNCE, 0x100);
                }
            } else {
                CALL_HIT(part, 1, kind, 0);
            }
            break;
        case 2:
            part->flags |= PART_FLAG_TOUCHED;
            if (gLevelState->maskLevel) {
                CALL_HIT(part, 1, EVENT_HIT, 0);
            }
            CALL_HIT((struct box_part *)gPlayer, 1, part->kind, 0);
            break;
        }
    }
}
