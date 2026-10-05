#ifndef GUARD_AABB_H
#define GUARD_AABB_H

#include "core.h"

/* An axis-aligned box: position and size, in pixels or fixed-point
 * world units depending on the caller. SetAabbPos/SetAabbSize
 * (src/util/aabb_setup.c) fill one, AabbOverlaps/AabbOverlapsInclusiveX
 * (src/util/aabb.c) test two for overlap, and the text renderer
 * (DrawWrappedText/DrawWrappedTextInBox, include/text.h) uses one as the
 * text rectangle.
 *
 * Several .c files still define their own copy of this struct; they
 * move to this header in their subsystem's header batch
 * (docs/headers_plan.md). */
struct aabb
{
    s32 x;
    s32 y;
    s32 w;
    s32 h;
};

#endif /* GUARD_AABB_H */
