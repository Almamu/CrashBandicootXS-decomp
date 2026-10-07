/* The line steppers are out of line here, at the end of the file
 * (crate_line_step.hpp). */
#define CRATE_LINE_STEP
#include "crate.hpp"
#include "player.hpp"

extern "C" {
#include "memory.h"
#include "objects.h"
#include "globals.h"
#include "player.h"
}

/* The crate's overlap test, stack links, constructor and destructor
 * (#664, include/crate.hpp), with the player's collision pass, and the
 * out-of-line copies of the line steppers (FindLineCrossingYMajor,
 * FindLineCrossingXMajor; no caller), from crate_line_step.hpp. */

/* Whether the crate's box (its hitbox record's size around its position)
 * lies strictly inside `box`. Always true while the crate is falling or
 * has `flags` bit 4 set (BreakCrate), where the base class's test looks
 * at a moving sprite's controller. */
s32 Crate::IsInsideRect(struct aabb *box)
{
    u8 skip = IsAlwaysActive();

    if (fallDistance != 0)
        skip = 1;
    if (!skip) {
        const struct hitbox_quad *q = GetBounds();
        s32 halfW = q->w << 7;
        s32 halfH = q->h << 7;
        s32 left = x - halfW;
        s32 top = y - halfH;
        s32 right = x + halfW;
        s32 bottom = y + halfH;
        u8 inside = 0;

        if (left > box->x && right < box->x + box->w && top > box->y && bottom < box->y + box->h)
            inside = 1;
        skip = inside;
    }
    return skip;
}

/* Resolves the player's collision candidates (`collisionQueue`), then
 * counts `bounce` up unless it is 0. */
void ResolvePlayerCollisions(void)
{
    Player *p = gPlayer;
    u8 *bounce;

    p->collisionQueue.Resolve();
    bounce = &gPlayer->bounce;
    if (*bounce != 0)
        *bounce = *bounce + 1;
}

Crate *Crate::GetBelow()
{
    return below;
}

Crate *Crate::GetAbove()
{
    return above;
}

void Crate::SetBelow(Crate *crate)
{
    below = crate;
}

void Crate::SetAbove(Crate *crate)
{
    above = crate;
}

/* UNUSED as a call: only the vtable has it. */
s32 Crate::GetClassId()
{
    return 3;
}

/* An iron switch crate frees its outline crates' group. */
Crate::~Crate()
{
    if (kind == CRATE_KIND_IRON_SWITCH) {
        struct crate_group *g = group;

        if (PHYS_HAS_GROUP(g)) {
            if (g != NULL)
                delete[] g;
            groupAllocated = 0;
        }
    }
}

Crate::Crate()
{
    groupAllocated = 0;
    Reset();
}

#include "crate_line_step.hpp"
#include "player.hpp"
