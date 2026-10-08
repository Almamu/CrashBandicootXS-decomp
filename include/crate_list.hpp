#ifndef GUARD_CRATE_LIST_HPP
#define GUARD_CRATE_LIST_HPP

/* The crate list as C++ (#664, docs/cplusplus.md, part 7f): `gCrateList`,
 * the crates of the room filed in a grid of 256-pixel columns.
 *
 *   CrateList  src/objects/part_list.cpp (the constructor, for ROM order),
 *              src/crates/crate_list*.cpp, crate_grid_*.cpp,
 *              crate_player_collide.cpp
 *
 * crates.h's `struct pool_manager`, `struct pool_node` and `struct
 * pool_link` are the C views, for the files that are still C and three of crate_break.cpp's loops; each class checks
 * its size against its view below. The C prototypes (crates.h) keep the C names; cxx_symbols.txt
 * maps the methods to them.
 *
 * `#pragma interface`: no class here has a vtable, so there is none to emit;
 * the pragma keeps g++ from emitting out-of-line copies of the inline
 * methods (docs/cplusplus.md, "Emitting the vtables"). */
#pragma interface

#include "crate.hpp"

extern "C" {
#include "core.h"
#include "aabb.h"
#include "crates.h"
#include "globals.h"
#include "match.h"
#include <agb_syscall.h>
}

struct CrateGridLink;

/* One node of the grid (0x14 bytes): a listed sprite, the next node in
 * its column, and the free-list entry it was taken from. A sprite that is
 * always active (flags bit 4) is also filed in column 255 under a second
 * node, and the two nodes' `link`s point at each other. Update and Draw
 * mark the nodes they have handled in the columns, so that the pass over
 * column 255 skips them. */
struct CrateGridNode {
    Crate *data;         // 0x00
    CrateGridNode *next; // 0x04
    CrateGridLink *wrap; // 0x08
    CrateGridNode *link; // 0x0C - the sprite's other node (column 255's, or its column's)
    u8 mark;             // 0x10 - handled by Update
    u8 mark2;            // 0x11 - handled by Draw
};

COMPILE_TIME_ASSERT(crate_list_hpp, sizeof(CrateGridNode) == sizeof(struct pool_node));

/* An entry of the free list: a node not in use, and the next entry. */
struct CrateGridLink {
    CrateGridNode *node; // 0x00
    CrateGridLink *next; // 0x04
};

COMPILE_TIME_ASSERT(crate_list_hpp, sizeof(CrateGridLink) == sizeof(struct pool_link));

/* The crate list (`gCrateList`, play_room.cpp allocates 0x818 bytes for
 * 0xC0 crates): `slots` holds the listed sprites (`count` of them, up to
 * `capacity`); `nodes` are the grid's `capacity` nodes, and `links` the
 * free list over them, `freeHead` its first entry. The grid has 256
 * columns, each a singly linked list from `heads[i]` to `tails[i]`: a
 * sprite is filed in the column of its x (`x >> 16`, 256 pixels wide),
 * and an always-active one in column 255 too. The passes (Update, Draw,
 * CollidePlayer, Collide) visit the camera's column and the two to its
 * right, then column 255. */
class CrateList
{
public:
    s32 count;                 // 0x000 - pool_manager's activeCount
    s32 capacity;              // 0x004
    Crate **slots;             // 0x008 - slotArray
    CrateGridNode *nodes;      // 0x00C - nodeArray
    CrateGridNode *heads[256]; // 0x010 - gridHead
    CrateGridNode *tails[256]; // 0x410 - gridTail
    CrateGridLink *links;      // 0x810 - freeListArray
    CrateGridLink *freeHead;   // 0x814 - freeListHead

    CrateList(s32 capacity); // InitCrateList (src/objects/part_list.cpp)
    ~CrateList();            // DestroyCrateList
    void Reset();            // ResetCrateList
    void Update();           // UpdateCrateList
    void Draw();             // DrawCrateList
    void Collide(struct aabb box, s32 unused, MovingSprite *other); // CollideCrateGrid
    void CollideWithPlayer(struct aabb box, MovingSprite *part); // CollideCrateGridPartWithPlayer
    void CollideWithObject(struct aabb box, MovingSprite *part,
                           MovingSprite *other); // CollideCrateGridPartWithObject
    void CollidePlayer(s32 unused);              // CollidePlayerWithCrates
    void Remove(Crate *sprite);                  // RemoveCrateFromList
    void RemoveAt(s32 index);                    // RemoveCrateListAt
    void Add(Crate *sprite);                     // AddCrateToList
    CrateGridNode *AddNode(Crate *sprite, s32 column, CrateGridNode *link); // AddCrateGridNode
    void Link(Crate *sprite);                                               // LinkCrateInGrid
    void LinkActive(Crate *sprite); // LinkCrateToActiveBucket
    void Unlink(Crate *sprite);     // UnlinkCrateFromGrid

    /* AddNode's body, which LinkActive inlines: takes a node off the free
     * list for `sprite` and appends it to column `column` (`heads[column]`
     * is set when the column was empty), with `link` its other node. */
    CrateGridNode *Append(Crate *sprite, s32 column, CrateGridNode *link)
    {
        CrateGridLink *entry = freeHead;
        CrateGridNode *node = entry->node;

        freeHead = entry->next;
        entry->next = 0;
        node->data = sprite;
        node->next = 0;
        node->link = link;
        node->mark = 0;
        node->mark2 = 0;
        if (heads[column] == 0)
            heads[column] = node;
        if (tails[column] != 0)
            tails[column]->next = node;
        tails[column] = node;
        return node;
    }

    /* Remove's body, which Update inlines: finds `sprite` in the slots,
     * takes its nodes out of the grid (Unlink), moves the slots after it
     * down one place (CpuSet) and clears the last. */
    void Detach(Crate *sprite)
    {
        s32 i = 0;
        s32 n = capacity;

        /* Emits no code. In Update's two copies, it keeps cse from swapping
         * `n` and its copy: without it, the loaded value is tested before
         * and after the loop and the copy in it, where the ROM has the
         * opposite (Remove is the same either way). Declaring `n` before
         * `i` gives the ROM's registers too, but loads it before the 0. */
        MATCH_BARRIER();
        if (i >= n)
            return;
        while (slots[i] != sprite)
            if (++i >= n)
                return;
        if (i < capacity) {
            Unlink(slots[i]);
            CpuSet(&slots[i + 1], &slots[i], ((count - i) & CPU_SET_COUNT_MASK) | CPU_SET_32BIT);
            slots[count - 1] = 0;
            count--;
        }
    }

    /* Empties the grid and threads the free list through every node: the
     * constructor's tail, and Reset's. Two source details give the ROM's
     * registers: the column clear is a plain indexed loop (gcc reverses it
     * into the ROM's countdown with two post-increment pointers), and the
     * free-list loop reads `links` through a copy (`fl`) taken inside the
     * `if (i < n)`, right before the `do` (the ROM's `mov ip, sb`); read
     * directly, the loop optimizer hoists `this + 0x810` above the clear.
     * The nodes are zeroed through crates.h's `struct pool_init_node`, the
     * untyped view: through `CrateGridNode *` fields, gcc takes the stores
     * as possible writes to `nodes` and reloads it. */
    void ResetGrid()
    {
        s32 i;
        s32 n = capacity;
        CrateGridLink **freeList = &links;
        CrateGridLink **head = &freeHead;
        CrateGridLink **fl;

        for (i = 0; i < 256; i++) {
            heads[i] = 0;
            tails[i] = 0;
        }
        i = 0;
        if (i < n) {
            fl = freeList;
            do {
                CrateGridLink *link;
                CrateGridLink *arr;

                (*fl)[i].node = &nodes[i];
                ((struct pool_init_node *)nodes)[i].data = 0;
                ((struct pool_init_node *)nodes)[i].next = 0;
                ((struct pool_init_node *)nodes)[i].link = 0;
                ((struct pool_init_node *)nodes)[i].mark = 0;
                ((struct pool_init_node *)nodes)[i].wrap =
                    (struct pool_link *)(link = &(arr = *fl)[i]);
                if (i == capacity - 1)
                    link->next = 0;
                else
                    link->next = &arr[i + 1];
                i++;
            } while (i < capacity);
        }
        *head = *freeList;
    }

    /* The column of `sprite`'s x: its high halfword, read with one
     * `ldrsh`. */
    static s32 ColumnOf(Crate *sprite)
    {
        return (s16)(sprite->x >> 16);
    }
};

COMPILE_TIME_ASSERT(crate_list_hpp, sizeof(CrateList) == sizeof(struct pool_manager));

/* gCrateList (globals.h) as the class. */
static inline CrateList *Crates()
{
    return (CrateList *)gCrateList;
}

#endif /* !GUARD_CRATE_LIST_HPP */
