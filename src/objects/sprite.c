#include "core.h"
#include "actor.h"
#include "box_part.h"

extern void WorldToScreen(void *arg0, s32 arg1, s32 arg2, s32 *arg3, s32 *arg4);
extern void DrawSpritePieces(void *unused, void *part, s32 *posPtr);
extern void OperatorDelete(void *arg0);

/* `part+0x25` selects whether (x, y) are already screen-relative
 * (nonzero - used as-is) or need the camera-relative conversion
 * WorldToScreen applies (zero - the common case). Either way, the
 * resolved {x, y} pair is forwarded to DrawSpritePieces (parked as
 * NON_MATCHING in src/gfx/graphics.c) to build/queue this part's
 * OAM entries. */
void DrawSpriteAt(void *self, void *part, s32 x, s32 y)
{
    s32 pos[2];

    if (*((u8 *)part + 0x25) == 0) {
        WorldToScreen(part, x, y, &pos[0], &pos[1]);
    } else {
        pos[0] = x;
        pos[1] = y;
    }
    DrawSpritePieces(self, part, pos);
}
asm(".align 2, 0");

/* `part`'s own leading {x, y} pair (the same Q8 fixed-point position
 * fields struct actor has at 0x00/0x04) becomes the explicit position
 * passed to DrawSpriteAt - confirms `part` embeds a struct-actor-shaped
 * position at its own start. */
void DrawSprite(void *self, void *part)
{
    DrawSpriteAt(self, part, *(s32 *)part >> 8, *(s32 *)((u8 *)part + 4) >> 8);
}
asm(".align 2, 0");

void DestroySpriteRenderer(void *arg0, u32 arg1)
{
    if (arg1 & 1) {
        OperatorDelete(arg0);
    }
}
asm(".align 2, 0");

void nullsub_2(void)
{
}
asm(".align 2, 0");

/* Initializes/clears several `part`-object fields also seen used in
 * DrawSpritePieces/DrawAffineSpritePieces: 0x20/0x30/0x34 (position-interpolation
 * state), 0x28-0x29 (the flags byte pair packed into attr1/attr2),
 * 0x2d (keyframe counter), 0x3c (Q8 "scale" factor), and 0x25 (the
 * screen-vs-camera-relative flag DrawSpriteAt tests). `part+0xd` is a
 * second, separate flags byte from `part+0xc`.
 *
 * Register pins throughout match the ROM's own choices for the two
 * bit-clear sequences (constant computed before the byte load, result
 * landing in the constant's own register - the same accumulator
 * pattern documented at length for `struct actor`'s flags field in
 * graphics.c) and for the final `0x2c` store (the ROM computes that
 * address into a *fresh* register rather than reusing `part`'s, even
 * though `part` is dead afterward - plain C let the allocator reuse
 * it instead). The running `p` pointer (advanced by `+8` then `+0xb`
 * rather than recomputed from `part` each time) and the shared `zero`
 * local (reused across differently-sized stores instead of
 * rematerializing the constant) both mirror the ROM's own address/
 * value reuse - see docs/matching.md, "Matching decompilation". */
void ResetSpriteObj(void *arg0)
{
    register void *part asm("r3") = arg0;
    register s32 result asm("r0");
    register s32 tmp asm("r1");

    result = 0x7f;
    tmp = *((u8 *)part + 0xc);
    result &= tmp;
    tmp = -0x41;
    result &= tmp;
    *((u8 *)part + 0xc) = result;

    {
        u8 *p = (u8 *)part + 0x25;
        s32 zero = 0;
        *p = zero;
        *(s32 *)((u8 *)part + 0x20) = zero;
        p += 8;
        *p = zero;
        *(s32 *)((u8 *)part + 0x30) = zero;
        *(s32 *)((u8 *)part + 0x34) = zero;
        *(u16 *)((u8 *)part + 0x28) = zero;
        p += 0xb;
        *p = zero;
    }

    {
        register s32 result2 asm("r0");
        register s32 tmp2 asm("r4");

        result2 = -5;
        tmp2 = *((u8 *)part + 0xd);
        result2 &= tmp2;
        *((u8 *)part + 0xd) = result2;
    }

    *((u8 *)part + 0x24) = 0;
    *(u16 *)((u8 *)part + 0x3c) = 0;
    {
        register u8 *p2 asm("r1") = (u8 *)part + 0x2c;
        *p2 = 1;
    }
}

struct aabb {
    s32 x;
    s32 y;
    s32 w;
    s32 h;
};

extern void SetAabbPos(struct aabb *buf, s32 x, s32 y);
extern void SetAabbSize(struct aabb *buf, s32 w, s32 h);

/* Builds the AABB (via the shared SetAabbPos set-position/SetAabbSize
 * set-size pair) for `part`'s current animation keyframe, whose box sits
 * at record+0xc, and mirrors it horizontally/vertically around `part`'s
 * own position per the 0x28 mirror bits. Returned by value (the hidden
 * return pointer is the `dest` the ROM keeps in r8 and hands back in r0).
 * Matches under old_agbcc - see docs/matching/issue-9-naked-retry.md. */
struct aabb GetSpriteBounds(struct box_part *part)
{
    struct aabb box;
    u8 *rec = (u8 *)&(*part->keyframes)[part->frame];
    struct part_box *pb = (struct part_box *)(rec + 0xc);
    s32 px = part->x >> 8;
    s32 offX = pb->offX;
    s32 py = part->y >> 8;
    s32 offY = pb->offY;
    u8 w = pb->w;
    u8 h = pb->h;

    SetAabbPos(&box, offX + px, offY + py);
    SetAabbSize(&box, w, h);
    if (part->mirrorX)
        box.x = (part->x >> 8) * 2 - (box.x + box.w);
    if (part->mirrorY)
        box.y = (part->y >> 8) * 2 - (box.y + box.h);
    return box;
}

/* Same as GetSpriteBounds above for the other keyframe-table layout, whose
 * box sits at record+0x4 instead (the collision box `struct anim_box` in
 * include/gobj_1a794.h). */
struct aabb GetSpriteHitbox(struct box_part *part)
{
    struct aabb box;
    u8 *rec = (u8 *)&(*part->keyframes)[part->frame];
    struct part_box *pb = (struct part_box *)(rec + 4);
    s32 px = part->x >> 8;
    s32 offX = pb->offX;
    s32 py = part->y >> 8;
    s32 offY = pb->offY;
    u8 w = pb->w;
    u8 h = pb->h;

    SetAabbPos(&box, offX + px, offY + py);
    SetAabbSize(&box, w, h);
    if (part->mirrorX)
        box.x = (part->x >> 8) * 2 - (box.x + box.w);
    if (part->mirrorY)
        box.y = (part->y >> 8) * 2 - (box.y + box.h);
    return box;
}
asm(".align 2, 0");
