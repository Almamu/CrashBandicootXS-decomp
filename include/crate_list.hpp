#ifndef GUARD_CRATE_LIST_HPP
#define GUARD_CRATE_LIST_HPP

/* The crate list as C++ (#664, docs/cplusplus.md, part 7f): `gCrateList`,
 * the crates of the room filed in a grid of 256-pixel columns.
 *
 *   CrateList  src/crates/crate_list*.cpp
 *
 * The classes have no C view (crates.h's `struct pool_init_node`, a
 * codegen view of the nodes, is CrateGridNodeInit below since #754).
 * cxx_symbols.txt maps the methods to their C names. CrateList is in the
 * namespace Game::Crates (#1033, crate.hpp); the grid records
 * (CrateGridNode, CrateGridLink, CrateGridNodeInit) stay global.
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

/* The grid's columns are 256 pixels wide: a crate's column is its x's
 * high halfword (`x >> 16` in Q8), the camera's its pixel x `>> 8`. */
#define CRATE_GRID_COLUMN_WIDTH 256

/* How many columns right of the camera's the passes visit (#979): enough
 * for the widest window they test, the activation window (screen.h),
 * whose right edge is DISPLAY_WIDTH + ACTIVE_AREA_MARGIN_X pixels right
 * of the camera, from anywhere in the camera's column: (255 + 340) / 256
 * = 2 on the GBA, 3 for a 426-pixel screen. The ROM's loops count down
 * from `lo + 2` to `lo`, the camera's column; the window's left margin,
 * which can reach into column `lo - 1`, isn't visited: the ROM's own
 * shape, kept. */
#define CRATE_GRID_SPAN \
    ((CRATE_GRID_COLUMN_WIDTH - 1 + DISPLAY_WIDTH + ACTIVE_AREA_MARGIN_X) / CRATE_GRID_COLUMN_WIDTH)

/* One node of the grid (0x14 bytes): a listed sprite, the next node in
 * its column, and the free-list entry it was taken from. A sprite that is
 * always active (flags bit 4) is also filed in column 255 under a second
 * node, and the two nodes' `link`s point at each other. Update and Draw
 * mark the nodes they have handled in the columns, so that the pass over
 * column 255 skips them. */
struct CrateGridNode {
    Game::Crates::Crate *data; // 0x00
    CrateGridNode *next;       // 0x04
    CrateGridLink *wrap;       // 0x08
    CrateGridNode *link;       // 0x0C - the sprite's other node (column 255's, or its column's)
    u8 mark;                   // 0x10 - handled by Update
    u8 mark2;                  // 0x11 - handled by Draw
};

COMPILE_TIME_ASSERT(crate_list_hpp, sizeof(CrateGridNode) == 0x14);

/* An entry of the free list: a node not in use, and the next entry. */
struct CrateGridLink {
    CrateGridNode *node; // 0x00
    CrateGridLink *next; // 0x04
};

COMPILE_TIME_ASSERT(crate_list_hpp, sizeof(CrateGridLink) == 8);

/* codegen: a grid node with untyped pointers, the view ResetGrid (below)
 * zeroes the nodes through (CrateList::InitView): through the real
 * `CrateGridNode *` fields, gcc takes the zeroing stores as possible writes
 * to the list's `nodes` and reloads it (docs/casts/crate_grid.md,
 * docs/headers.md "Codegen exceptions"). Formerly crates.h's
 * `struct pool_init_node`. */
struct CrateGridNodeInit {
    void *data;
    void *next;
    CrateGridLink *wrap;
    void *link;
    u8 mark;
};

ASSERT_VIEW_FIELD(crate_list_hpp, CrateGridNode, CrateGridNodeInit, data);
ASSERT_VIEW_FIELD(crate_list_hpp, CrateGridNode, CrateGridNodeInit, next);
ASSERT_VIEW_FIELD(crate_list_hpp, CrateGridNode, CrateGridNodeInit, wrap);
ASSERT_VIEW_FIELD(crate_list_hpp, CrateGridNode, CrateGridNodeInit, link);
ASSERT_VIEW_FIELD(crate_list_hpp, CrateGridNode, CrateGridNodeInit, mark);

namespace Game
{
namespace Crates
{

/* The crate list (`gCrateList`, play_room.cpp allocates 0x818 bytes for
 * 0xC0 crates): `slots` holds the listed sprites (`count` of them, up to
 * `capacity`); `nodes` are the grid's `capacity` nodes, and `links` the
 * free list over them, `freeHead` its first entry. The grid has 256
 * columns, each a singly linked list from `heads[i]` to `tails[i]`: a
 * sprite is filed in the column of its x (`x >> 16`, 256 pixels wide),
 * and an always-active one in column 255 too. The passes (Update, Draw,
 * CollidePlayer, Collide) visit the camera's column and the
 * CRATE_GRID_SPAN (two) to its right, then column 255. */
class CrateList
{
public:
    s32 count;                 // 0x000
    s32 capacity;              // 0x004
    Crate **slots;             // 0x008
    CrateGridNode *nodes;      // 0x00C
    CrateGridNode *heads[256]; // 0x010
    CrateGridNode *tails[256]; // 0x410
    CrateGridLink *links;      // 0x810
    CrateGridLink *freeHead;   // 0x814

    CrateList(s32 capacity); // InitCrateList (src/crates/crate_list_update.cpp)
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
     * down one place (CpuSet) and clears the last. The search is a plain
     * `while` reading `capacity` in its test. With a local `n = capacity`
     * and early returns instead, Update's copy had `n` and its register
     * copy swapped and needed an asm barrier (#662 round 3). */
    void Detach(Crate *sprite)
    {
        s32 i = 0;

        while (i < capacity && slots[i] != sprite)
            i++;
        if (i < capacity) {
            Unlink(slots[i]);
            CpuSet(&slots[i + 1], &slots[i], ((count - i) & CPU_SET_COUNT_MASK) | CPU_SET_32BIT);
            slots[count - 1] = 0;
            count--;
        }
    }

    /* codegen: the nodes as CrateGridNodeInit, the view ResetGrid zeroes
     * them through (docs/casts/crate_grid.md). The ROM reloads `nodes`
     * after the free-list store and after `mark`'s byte store, but not
     * after the `data`/`next`/`link` stores: those weren't stores of a
     * `CrateGridNode *`, which may alias `nodes`. Each call reads `nodes`
     * again. It comes before ResetGrid: defined after it, the two
     * objects that inline ResetGrid change. */
    CrateGridNodeInit *InitView()
    {
        return (CrateGridNodeInit *)nodes;
    }

    /* Empties the grid and threads the free list through every node: the
     * constructor's tail, and Reset's. Two source details give the ROM's
     * registers: the column clear is a plain indexed loop (gcc reverses it
     * into the ROM's countdown with two post-increment pointers), and the
     * free-list loop reads `links` through a copy (`fl`) taken inside the
     * `if (i < n)`, right before the `do` (the ROM's `mov ip, sb`); read
     * directly, the loop optimizer hoists `this + 0x810` above the clear.
     * The nodes are zeroed through InitView (above). */
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
                InitView()[i].data = 0;
                InitView()[i].next = 0;
                InitView()[i].link = 0;
                InitView()[i].mark = 0;
                InitView()[i].wrap = link = &(arr = *fl)[i];
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
     * `ldrsh`. The shift by 16 isn't a Q16 conversion (no Q16_TO_INT):
     * x is Q8, and its whole pixels divided by CRATE_GRID_COLUMN_WIDTH
     * (256) fold into one shift. */
    static s32 ColumnOf(Crate *sprite)
    {
        return (s16)(sprite->x >> 16);
    }
};

COMPILE_TIME_ASSERT(crate_list_hpp, sizeof(CrateList) == 0x818);

} // namespace Crates
} // namespace Game

/* gCrateList (globals.h) as the class. It was `Crates()`, which inside a
 * Game:: namespace would find the namespace Game::Crates. Still an inline
 * function: calling it, not reading gCrateList, is what keeps the loop
 * tests of the crate walks where the ROM has them (docs/cplusplus.md,
 * part 7f). */
static inline Game::Crates::CrateList *RoomCrates()
{
    return gCrateList;
}

#endif /* !GUARD_CRATE_LIST_HPP */
