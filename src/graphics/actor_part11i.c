#include "core.h"

extern s32 _call_via_r2(void *arg0, void *arg1, void *fn);

/* Resets a pool manager to empty: tears down every active object
 * (`slotArray[0..activeCount)`, firing each one's `table+0x50/0x54`
 * trampoline via `_call_via_r2` with constant arg `3` if non-`NULL`,
 * then clearing the slot), resets `activeCount` to 0, and rebuilds
 * both the grid (`gridHead`/`gridTail` zeroed) and the free list from
 * scratch over `nodeArray` - the exact same free-list-build loop
 * `InitCrateList` (`actor_part11.c`) performs during initialization; see
 * that function's own struct/field writeup, identical here.
 *
 * Real C (issue #9-#11 NAKED retry; matches under both compilers): the
 * teardown loop is plain C, and the tail is `InitCrateList`'s
 * `PoolResetFreeList` inline (see actor_part11.c for the two source
 * details it needs). Kept in its own translation unit (not appended to
 * `actor_part11.c`) since its real ROM address, 0x08009914, sits
 * between `CollidePlayerWithCrates` (`actor_part11d.c`) and `CollideCrateGridPartWithObject`
 * (`actor_part12.c`) in ROM order. */
struct pool_init_link;

struct pool_init_node {
    void *unk_00;
    void *unk_04;
    struct pool_init_link *link;  // 0x08
    void *unk_0C;
    u8 unk_10;
};

struct pool_init_link {
    struct pool_init_node *node;
    struct pool_init_link *next;
};

struct pool_obj {
    u8 unk_00[0x18];
    u8 *vtable;         // 0x18
};

struct pool_init {
    s32 activeCount;
    s32 capacity;
    struct pool_obj **slotArray;
    struct pool_init_node *nodeArray;
    void *gridHead[256];
    void *gridTail[256];
    struct pool_init_link *freeListArray;
    struct pool_init_link *freeListHead;
};

static inline void PoolResetFreeList(struct pool_init *m)
{
    s32 i;
    s32 n = m->capacity;
    struct pool_init_link **freeList = &m->freeListArray;
    struct pool_init_link **freeHead = &m->freeListHead;
    struct pool_init_link **fl;

    for (i = 0; i < 256; i++) {
        m->gridHead[i] = NULL;
        m->gridTail[i] = NULL;
    }
    i = 0;
    if (i < n) {
        fl = freeList;
        do {
            struct pool_init_link *link;
            struct pool_init_link *arr;

            (*fl)[i].node = &m->nodeArray[i];
            m->nodeArray[i].unk_00 = NULL;
            m->nodeArray[i].unk_04 = NULL;
            m->nodeArray[i].unk_0C = NULL;
            m->nodeArray[i].unk_10 = 0;
            m->nodeArray[i].link = link = &(arr = *fl)[i];
            if (i == m->capacity - 1)
                link->next = NULL;
            else
                link->next = &arr[i + 1];
            i++;
        } while (i < m->capacity);
    }
    *freeHead = *freeList;
}

void ResetCrateList(struct pool_init *m)
{
    s32 i;

    for (i = 0; i < m->activeCount; i++) {
        struct pool_obj *obj = m->slotArray[i];

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
