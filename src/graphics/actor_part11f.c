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

struct camera_pos {
    s32 x;
    s32 y;
};

struct viewport {
    u8 unk_00[0x10];
    struct camera_pos *camera; // 0x10
};

extern struct viewport *gUnknown_03001308;
extern struct box_part *gUnknown_030012D8;
extern s32 sub_803AD80(void *self, void *arg, void *fn);
extern s32 sub_803AD7C(void *self, void *fn);
extern void *sub_800014C(void *dst, const void *src, s32 size);
extern void sub_80096C0(struct pool_manager *m, struct part_aabb box, struct box_part *part);
extern void sub_80099F0(struct pool_manager *m, struct part_aabb box, struct box_part *part, struct box_part *other);

/* The spatial-hash-grid-cluster analog of `sub_8008A40`: the same
 * "extended screen box" filter shape as `sub_800944C` (iterating
 * `manager`'s grid buckets from `baseIdx+2` down to 0, then the
 * special "large object" bucket 255), but instead of firing a simple
 * action trampoline on a box hit, additionally tests `part->flags`
 * bit 2 and fires a `table+0x48/0x4c`-driven trampoline via
 * `sub_803AD7C`; if that result is greater than 4, reconstructs the
 * caller's original `{boxX, boxY, boxW, boxH}` box (via
 * `sub_800014C`, the same "unavoidable extra `boxH` load" idiom
 * established for `sub_8008A40`) and dispatches to `sub_80096C0`
 * (when `compareViewport` is the player, `gUnknown_030012D8`) or
 * `sub_80099F0` (otherwise) - the exact same dispatch `sub_8008A40`
 * makes to `sub_8008AD8`/`sub_8008D80`. See `sub_8008A40`'s own
 * writeup (`actor_part7.c`) for the full branch-by-branch semantics,
 * identical here.
 *
 * Matches under old_agbcc (see docs/matching/issue-9-naked-retry.md).
 * The size gap the old draft had was the newer compiler plus the box
 * copy: the box arrives by value and is copied into one shared
 * temporary with sub_800014C (memcpy) before each dispatch, exactly as
 * in sub_8008A40. The duplicated per-node test is an inline helper;
 * `heads`/`last` (gridHead / &gridHead[255]) are computed up front, in
 * that order, as the ROM does. Kept in its own translation unit since
 * its ROM address, 0x08009528, sits between `sub_800944C`
 * (`actor_part11h.c`) and `sub_80096C0` (`actor_part11e.c`). */
static inline void CheckPart(struct pool_manager *m, struct box_part *part, struct part_aabb *screen,
                             struct part_aabb *box, struct part_aabb *tmp, struct box_part *other)
{
    struct part_method *m1 = PART_METHOD(part, 0x30);

    if ((u8)sub_803AD80((u8 *)part + m1->thisOffset, screen, m1->fn)
        && ((part->flags >> 2) & 1)) {
        struct part_method *m2 = PART_METHOD(part, 0x48);

        if (sub_803AD7C((u8 *)part + m2->thisOffset, m2->fn) > 4) {
            if (other == gUnknown_030012D8) {
                sub_800014C(tmp, box, sizeof(*tmp));
                sub_80096C0(m, *tmp, part);
            } else {
                sub_800014C(tmp, box, sizeof(*tmp));
                sub_80099F0(m, *tmp, part, other);
            }
        }
    }
}

void sub_8009528(struct pool_manager *m, struct part_aabb box, s32 unused, struct box_part *other)
{
    struct part_aabb screen;
    struct part_aabb tmp;
    struct camera_pos *cam;
    s32 lo;
    s32 i;
    struct grid_node *node;
    struct grid_node **last;
    struct grid_node **heads;

    cam = gUnknown_03001308->camera;
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
