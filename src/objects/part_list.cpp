#include "part_list.hpp"

extern "C" {
#include <agb_syscall.h>
#include "crates.h"
#include "memory.h"
}

/* The part list's own methods, and InitCrateList (#664, part 7c;
 * include/part_list.hpp). */

/* Draws every part on screen. */
void PartList::Draw()
{
    s32 i;

    for (i = 0; i < visibleCount; i++)
        visible[i]->Draw();
}

/* Removes `part` from `items`: the entries after it move down one place
 * (CpuSet), and the last slot is cleared. */
void PartList::Remove(MovingSprite *part)
{
    s32 i = 0;
    s32 n = capacity;

    if (i >= n)
        return;
    while (items[i] != part)
        if (++i >= n)
            return;
    if (i < capacity) {
        CpuSet(&items[i + 1], &items[i], ((count - i) & CPU_SET_COUNT_MASK) | CPU_SET_32BIT);
        count--;
        items[count] = 0;
    }
}

/* Removes the entry at `index`, as Remove does. */
void PartList::RemoveAt(s32 index)
{
    if (index < capacity) {
        CpuSet(&items[index + 1], &items[index],
               ((count - index) & CPU_SET_COUNT_MASK) | CPU_SET_32BIT);
        count--;
        items[count] = 0;
    }
}

/* Appends `part` if there's room. */
void PartList::Add(MovingSprite *part)
{
    if (count < capacity)
        items[count++] = part;
}

PartList::~PartList()
{
    if (visible)
        delete[] visible;
    if (items)
        delete[] items;
}

PartList::PartList(s32 n)
{
    s32 i;

    count = 0;
    visibleCount = 0;
    capacity = n;
    items = new MovingSprite *[n];
    visible = new MovingSprite *[n];
    for (i = 0; i < capacity; i++)
        items[i] = 0;
}

/* InitCrateList initializes a fixed-slot object pool "manager" struct:
 *  +0x0: s32 activeCount (0)
 *  +0x4: s32 capacity (= count)
 *  +0x8: void **slotArray - `count` pointers, allocated via
 *        OperatorNewArray(count*4), zero-filled
 *  +0xc: u8 *nodeArray - `count` 0x14-byte nodes, allocated via
 *        OperatorNewArray(count*0x14)
 *  +0x10..0x40F: a 256-word (0x400-byte) table, zeroed
 *  +0x410..0x80F: a second 256-word (0x400-byte) table, zeroed
 *  +0x810: void *freeListArray - `count` 8-byte {node, next} pairs,
 *          allocated via OperatorNewArray(count*8)
 *  +0x814: void *freeListHead - set to `freeListArray` itself at the
 *          very end
 *
 * For each node index i (0..count-1): freeListArray[i].node points at
 * nodeArray[i]; nodeArray[i]'s fields 0/4/0xc and byte 0x10 are
 * zeroed; nodeArray[i]'s field 8 is set to point back at
 * freeListArray[i]; and freeListArray[i].next is chained to
 * freeListArray[i+1] (or NULL for the last entry) - building a
 * singly-linked free list of pool nodes through the wrapper array.
 * The two big 256-word tables (+0x10, +0x410) are very likely a pair
 * of spatial-partition/collision grids, consistent with this whole
 * region being part of docs/rom_map.md's still-only-partially-
 * understood AI/collision dispatch system - but that broader purpose
 * isn't needed to confirm every individual load/store here, which are
 * all confirmed correct.
 *
 * Real C (issue #9-#11 NAKED retry; matches under both compilers). The
 * tail is the `PoolResetFreeList` inline (ResetCrateList ends with the same
 * code). Two source details carry the register assignment the old
 * notes blamed on the allocator: the grid clear is a plain indexed
 * `for` loop (gcc reverses it into the ROM's `i = 255 .. 0` countdown
 * with two post-increment pointers), and the free-list loop reads the
 * wrapper array through a `fl = freeList` copy taken inside the
 * `if (i < n)` guard, right before the `do` - that is the ROM's
 * `mov ip, sb` at the loop head. With `m->freeListArray` read directly
 * instead, the loop optimizer hoists `m + 0x810` above the grid clear,
 * which ties up r1 there and pushes the cached count out of r3. */
/* The zeroing stores go through `struct pool_init_node` (crates.h), the
 * untyped view of the node. */
static inline void PoolResetFreeList(struct pool_manager *m)
{
    s32 i;
    s32 n = m->capacity;
    struct pool_link **freeList = &m->freeListArray;
    struct pool_link **freeHead = &m->freeListHead;
    struct pool_link **fl;

    for (i = 0; i < 256; i++) {
        m->gridHead[i] = 0;
        m->gridTail[i] = 0;
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
                link->next = 0;
            else
                link->next = &arr[i + 1];
            i++;
        } while (i < m->capacity);
    }
    *freeHead = *freeList;
}

struct pool_manager *InitCrateList(struct pool_manager *m, s32 count)
{
    m->activeCount = 0;
    m->capacity = count;
    m->slotArray = new struct box_part *[count];
    m->nodeArray = new struct pool_node[m->capacity];
    m->freeListArray = new struct pool_link[m->capacity];
    for (s32 j = 0; j < m->capacity; j++)
        m->slotArray[j] = 0;
    PoolResetFreeList(m);
    return m;
}

/* Same pool-manager struct InitCrateList initializes and crate_list.c
 * operates on - see that file for the full field writeup. Also used by
 * the now-matched `LinkCrateToActiveBucket` (`crate_grid_link.c`). */
/* ResetCrateList is reconstructed (semantics fully understood, and now
 * matched) as a NAKED transcription in its own translation unit,
 * `src/crates/crate_list_reset.c` - not appended here since its real
 * ROM address, 0x08009914, doesn't sit adjacent to this file's own
 * functions (it comes right after `CollidePlayerWithCrates`, `crate_player_collide.c`,
 * and right before `CollideCrateGridPartWithObject`/`crate_list.c`), per
 * docs/workflow.md step 4's "needs its own new .c file" case - the
 * same reason `CollideCrateGridPartWithPlayer` above got its own file
 * (`crate_grid_collide.c`), `CollideCrateGrid` got `crate_grid_collide.c`, the
 * now-matched `LinkCrateToActiveBucket` got `crate_grid_link.c`, and the now-matched
 * `DrawCrateList` got `crate_list_draw.c`. */

/* DrawCrateList is now matched as real C in its own translation unit,
 * `src/crates/crate_list_draw.c`; CollideCrateGrid is a NAKED transcription
 * in `src/crates/crate_grid_collide.c`; CollideCrateGridPartWithPlayer likewise in
 * `src/crates/crate_grid_collide.c` - none appended here since their real
 * ROM addresses don't sit adjacent to this file's own functions (they
 * come right after `LinkCrateToActiveBucket` above and right before `CollidePlayerWithCrates`,
 * `crate_player_collide.c`), per docs/workflow.md step 4's "needs its own new
 * .c file" case. */
