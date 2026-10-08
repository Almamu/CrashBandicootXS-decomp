#ifndef GUARD_AABB_H
#define GUARD_AABB_H

#include "core.h"

/* An axis-aligned box: position and size, in pixels or fixed-point
 * world units depending on the caller. SetAabbPos/SetAabbSize
 * (src/util/aabb_setup.cpp) fill one, AabbOverlaps/AabbOverlapsInclusiveX
 * (src/util/aabb.c) test two for overlap, and the text renderer
 * (DrawWrappedText/DrawWrappedTextInBox, include/text.h) uses one as the
 * text rectangle. GetSpriteHitbox and the other sprite box getters
 * return one, and the collision passes take it by value.
 *
 * The local copies (`part_aabb` in box_part.h, `hop_box`, `gfx_box`,
 * `box`, `ab_box`, `fx_box`, `hit_box` and eight `struct aabb`s with
 * `field_0`..`field_c` or x/y/w/h) were merged here (docs/headers_plan.md,
 * batch 4). */
struct aabb {
    s32 x;
    s32 y;
    s32 w;
    s32 h;
};

/* A box's `w`, read through a volatile: some code re-reads a just-filled
 * box's width (a box with no width is empty) straight from its stack
 * slot rather than through the register already holding the box's
 * address, and the volatile read is what stops gcc's CSE from rewriting
 * the address (dingodile.cpp, tiny_update.cpp). */
#define AABB_VALID(box) (*(vs32 *)&(box).w)

#endif /* GUARD_AABB_H */
