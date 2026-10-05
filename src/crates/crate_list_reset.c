#include "core.h"
#include "crates.h"
#include "box_part.h"

extern s32 _call_via_r2(void *arg0, void *arg1, void *fn);

/* Resets a pool manager to empty: tears down every active object
 * (`slotArray[0..activeCount)`, firing each one's `table+0x50/0x54`
 * trampoline via `_call_via_r2` with constant arg `3` if non-`NULL`,
 * then clearing the slot), resets `activeCount` to 0, and rebuilds
 * both the grid (`gridHead`/`gridTail` zeroed) and the free list from
 * scratch over `nodeArray` - the exact same free-list-build loop
 * `InitCrateList` (`part_list.c`) performs during initialization; see
 * that function's own struct/field writeup, identical here.
 *
 * Real C (issue #9-#11 NAKED retry; matches under both compilers): the
 * teardown loop is plain C, and the tail is `InitCrateList`'s
 * `PoolResetFreeList` inline (see part_list.c for the two source
 * details it needs). Kept in its own translation unit (not appended to
 * `part_list.c`) since its real ROM address, 0x08009914, sits
 * between `CollidePlayerWithCrates` (`crate_player_collide.c`) and `CollideCrateGridPartWithObject`
 * (`crate_list.c`) in ROM order. */
/* codegen: `struct pool_node` (crates.h) with untyped fields. Through
 * the real `struct pool_node *` fields, gcc takes the zeroing stores below
 * as possible writes to `m->nodeArray` and reloads it. */
struct pool_init_node {
    void *data;
    void *next;
    struct pool_link *wrap;
    void *link;
    u8 mark;
};

static inline void PoolResetFreeList(struct pool_manager *m)
{
    s32 i;
    s32 n = m->capacity;
    struct pool_link **freeList = &m->freeListArray;
    struct pool_link **freeHead = &m->freeListHead;
    struct pool_link **fl;

    for (i = 0; i < 256; i++) {
        m->gridHead[i] = NULL;
        m->gridTail[i] = NULL;
    }
    i = 0;
    if (i < n) {
        fl = freeList;
        do {
            struct pool_link *link;
            struct pool_link *arr;

            (*fl)[i].node = &m->nodeArray[i];
            ((struct pool_init_node *)m->nodeArray)[i].data = NULL;
            ((struct pool_init_node *)m->nodeArray)[i].next = NULL;
            ((struct pool_init_node *)m->nodeArray)[i].link = NULL;
            ((struct pool_init_node *)m->nodeArray)[i].mark = 0;
            ((struct pool_init_node *)m->nodeArray)[i].wrap = link = &(arr = *fl)[i];
            if (i == m->capacity - 1)
                link->next = NULL;
            else
                link->next = &arr[i + 1];
            i++;
        } while (i < m->capacity);
    }
    *freeHead = *freeList;
}

void ResetCrateList(struct pool_manager *m)
{
    s32 i;

    for (i = 0; i < m->activeCount; i++) {
        struct box_part *obj = m->slotArray[i];

        if (obj != NULL) {
            u8 *m50 = obj->vtable + 0x50;
            _call_via_r2((u8 *)obj + *(s16 *)m50, (void *)3, *(void **)(m50 + 4));
        }
        m->slotArray[i] = NULL;
    }
    m->activeCount = 0;
    PoolResetFreeList(m);
}
asm(".align 2, 0");
