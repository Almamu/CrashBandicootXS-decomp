#ifndef GUARD_AABB_H
#define GUARD_AABB_H

#include "core.h"

/* An axis-aligned box: position and size, in pixels or fixed-point
 * world units depending on the caller. SetAabbPos/SetAabbSize
 * (src/system/inline_copies_misc.cpp) fill one, AabbOverlaps/AabbOverlapsInclusiveX
 * (src/util/aabb.cpp) test two for overlap, and the text renderer
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

/* A position or a vector: x, y, in pixels or fixed-point world units
 * depending on the caller (a sprite's position and previous position,
 * Player's mask trail, a terrain probe's position, a collision
 * candidate's, the pickups' home position, the HUD's and the menus' screen
 * positions in their tables). A copy is one 8-byte struct copy (the ROM's
 * paired `ldr; ldr; str; str`). The copies (gfx_part.h's `gfx_vec`,
 * objects.h's `e08c_pos`, level.h's `probe_pos`, hud.h's `hud_pos`,
 * menus.h's `xy_pair` and `icon_pos`, pickups.h's `orbit_vec`,
 * gobj_1a794.h's `pos2` and the file-local `text_vec`, `lk_point` and
 * `gl_point`) were merged here (#656). */
struct vec2 {
    s32 x;
    s32 y;
};
COMPILE_TIME_ASSERT(aabb_h, sizeof(struct vec2) == 8);

/* A box's fields read through its address, and the box mirrored around
 * a centre on one axis (`x = cx * 2 - (x + w)`). As inlines taking the
 * box's address, a stack box's `&box` reaches the inlined body as the
 * constant `frame + offset` (integrate.c substitutes it for the
 * parameter) instead of a register: each field is read at its own sp
 * offset, and nothing holds the address for cse to reuse. Written as
 * `box.y` instead, a BLKmode local's fields are read through a copy of
 * its address, which cse ties to any register already holding it
 * (#662 round 4; see SetAabb in util.h). */
static inline s32 AabbX(const struct aabb *box)
{
    return box->x;
}

static inline s32 AabbY(const struct aabb *box)
{
    return box->y;
}

static inline s32 AabbW(const struct aabb *box)
{
    return box->w;
}

static inline s32 AabbH(const struct aabb *box)
{
    return box->h;
}

static inline void FlipAabbX(struct aabb *box, s32 cx)
{
    box->x = cx * 2 - (box->x + box->w);
}

static inline void FlipAabbY(struct aabb *box, s32 cy)
{
    box->y = cy * 2 - (box->y + box->h);
}

#endif /* GUARD_AABB_H */
