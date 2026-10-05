#include "core.h"
#include "box_part.h"

struct grid_node {
    struct box_part *data;
    struct grid_node *next;
};

struct pool_manager {
    s32 activeCount;
    s32 capacity;
    void **slotArray;
    void *nodeArray;
    struct grid_node *gridHead[256];
};

struct bg_scroll_layer {
    s32 x;
    s32 y;
};

struct level_layers {
    u8 unk_00[0x10];
    struct bg_scroll_layer *layer0; // 0x10
};

extern struct level_layers *gLevelLayers;
extern struct box_part *gPlayer;
extern s32 _call_via_r2(void *self, void *arg, void *fn);
extern s32 _call_via_r1(void *self, void *fn);
extern void *MemCopy32(void *dst, const void *src, s32 size);
extern void CollideCrateGridPartWithPlayer(struct pool_manager *m, struct part_aabb box, struct box_part *part);
extern void CollideCrateGridPartWithObject(struct pool_manager *m, struct part_aabb box, struct box_part *part, struct box_part *other);

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
 * writeup (`actor_part7.c`) for the full branch-by-branch semantics,
 * identical here.
 *
 * Matches under old_agbcc (see docs/matching/issue-9-naked-retry.md).
 * The size gap the old draft had was the newer compiler plus the box
 * copy: the box arrives by value and is copied into one shared
 * temporary with MemCopy32 (memcpy) before each dispatch, exactly as
 * in CollidePartList. The duplicated per-node test is an inline helper;
 * `heads`/`last` (gridHead / &gridHead[255]) are computed up front, in
 * that order, as the ROM does. Kept in its own translation unit since
 * its ROM address, 0x08009528, sits between `DrawCrateList`
 * (`actor_part11h.c`) and `CollideCrateGridPartWithPlayer` (`actor_part11e.c`). */
static inline void CheckPart(struct pool_manager *m, struct box_part *part, struct part_aabb *screen,
                             struct part_aabb *box, struct part_aabb *tmp, struct box_part *other)
{
    struct part_method *m1 = PART_METHOD(part, 0x30);

    if ((u8)_call_via_r2((u8 *)part + m1->thisOffset, screen, m1->fn)
        && ((part->flags >> 2) & 1)) {
        struct part_method *m2 = PART_METHOD(part, 0x48);

        if (_call_via_r1((u8 *)part + m2->thisOffset, m2->fn) > 4) {
            if (other == gPlayer) {
                MemCopy32(tmp, box, sizeof(*tmp));
                CollideCrateGridPartWithPlayer(m, *tmp, part);
            } else {
                MemCopy32(tmp, box, sizeof(*tmp));
                CollideCrateGridPartWithObject(m, *tmp, part, other);
            }
        }
    }
}

void CollideCrateGrid(struct pool_manager *m, struct part_aabb box, s32 unused, struct box_part *other)
{
    struct part_aabb screen;
    struct part_aabb tmp;
    struct bg_scroll_layer *cam;
    s32 lo;
    s32 i;
    struct grid_node *node;
    struct grid_node **last;
    struct grid_node **heads;

    cam = gLevelLayers->layer0;
    {
        s32 x = cam->x << 8;
        s32 y = cam->y << 8;
        screen.x = x;
        screen.y = y;
    }
    {
        s32 w = 240 << 8;
        s32 h = 160 << 8;
        screen.w = w;
        screen.h = h;
    }
    lo = cam->x;
    lo >>= 8;
    if (lo < 0)
        lo = 0;
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
asm(".align 2, 0");
