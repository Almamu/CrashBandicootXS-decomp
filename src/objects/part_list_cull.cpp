#include "bg_layer.hpp"
#include "part_list.hpp"

extern "C" {
#include "math_util.h"
#include "globals.h"
#include "level.h"
}

/* The part list's per-frame passes (#664, part 7c; include/part_list.hpp).
 * An old_agbcp object (OLD_AGBCC_OBJS): CollideClass's flag test takes
 * old_agbcp's registers. */

/* Fills `visible` with the parts that overlap the screen (the 240x160
 * GBA screen, in Q8, at `gLevelLayers->layer0`'s scroll position):
 * UpdatePartList's screen pass without its compaction. */
void PartList::Cull()
{
    s32 i;
    struct aabb screen;
    BgLayer *cam = gLevelLayers->layer0;

    {
        s32 cx = INT_TO_Q8(cam->x);
        s32 cy = INT_TO_Q8(cam->y);
        screen.x = cx;
        screen.y = cy;
    }
    {
        s32 w = INT_TO_Q8(240);
        s32 h = INT_TO_Q8(160);
        screen.w = w;
        screen.h = h;
    }
    visibleCount = 0;
    for (i = 0; i < count; i++) {
        Sprite *part = items[i];

        if ((u8)part->OverlapsRect(&screen))
            visible[visibleCount++] = part;
    }
}

/* Deletes every part and empties the list. */
void PartList::Clear()
{
    s32 i;

    for (i = 0; i < count; i++) {
        delete items[i];
        items[i] = 0;
    }
    count = 0;
    visibleCount = 0;
}

/* CheckPlayerContact on every part on screen of class `classId` that is
 * in contact with the player (flags bit 2). */
void PartList::CollideClass(s32 classId)
{
    s32 i;

    for (i = 0; i < visibleCount; i++) {
        Sprite *part = visible[i];

        if (part->GetClassId() != classId)
            continue;
        s32 inContact = (part->f.flags >> 2) & 1;

        if (!inContact)
            continue;
        part->CheckPlayerContact();
    }
}
