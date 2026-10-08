#include "sprite_obj.hpp"

extern "C" {
#include "math_util.h"
#include "level.h"
}

/* GroundSprite's terrain collision (#664, include/sprite_obj.hpp): the
 * floor and wall probes that keep a ground sprite on the terrain. */

/* Slot 1. A sprite that collides runs the terrain probes
 * (ProbeTerrainAxes) and adds the axes they hit to `hitAxes`, then
 * MovingSprite's contact with the player. When that leaves it on the
 * floor (bit 3), it clears flag 5, and unless it is grounded already,
 * confirms the floor with the step probe (ProbeEdgeTerrain); if that
 * doesn't, it sets flag 5 and takes the floor bit back. Returns
 * `hitAxes`. */
s32 GroundSprite::CheckPlayerContact()
{
    u8 *p = &hitAxes;
    u8 val = *p;

    if (f.flags >> 7) {
        val |= ProbeTerrainAxes();
        *p = val;
        MovingSprite::CheckPlayerContact();
        if (*p & 8) {
            s32 grounded;

            f.b.unk_5 = 0;
            grounded = (f.bytes.flags2 >> 1) & 1;
            if (!grounded) {
                if (!(u8)ProbeEdgeTerrain(8, GetBounds())) {
                    f.b.unk_5 = 1;
                    *p &= 7;
                }
            }
        }
    }
    return hitAxes;
}

/* The terrain probes of one frame, for a sprite that is on screen (or
 * near the camera) and collides: clears `hitMask`; with the floor probe
 * enabled, the floor under it (ProbeFloor); a floor it found only by
 * stepping down (ProbeFloor's `outFlag`) is snapped to with
 * ProbeSolidFloorHeight. Then, unless it is on the floor, ProbeTerrain
 * along the hitbox edges it moves towards: sideways (over the hitbox's
 * height less 16, from 8 pixels down), up or down (over its width), and
 * sideways again (over its whole height). Each hit snaps the position
 * and adds the axis to `hitMask` and to the result; a sprite standing on
 * the floor (or probing it while not moving vertically) has 8 in the
 * result too. */
s32 GroundSprite::ProbeTerrainAxes()
{
    s32 origX;
    s32 origY;
    struct vec2 pos;
    u8 unused;
    u8 floorMiss;
    s32 result;
    u8 hit;
    const struct hitbox_quad *quad;
    s32 mode;

    result = 0;
    floorMiss = 0;
    hit = 0;
    if (!(u8)IsNearCamera() || !(f.flags >> 7))
        goto done;
    if (!(dir & PART_DIR_Y_MASK) && (f.bytes.flags2 & 1))
        result = 8;
    hitMask = 0;
    quad = GetBounds();
    if (f.bytes.flags2 & 1)
        hit = ProbeFloor(quad, &floorMiss);
    if (hit && result == 0)
        result = 8;
    if (floorMiss == 1) {
        u8 c;

        origY = y;
        pos = *(struct vec2 *)&Pos();
        OffsetToHitboxEdge(&pos, 8, (void *)quad);
        pos.x = Q8_TO_INT(pos.x);
        pos.y = Q8_TO_INT(pos.y);
        if (mirrorBits.flipX < 0)
            pos.x -= quad->w >> 1;
        else
            pos.x += quad->w >> 1;
        c = ProbeSolidFloorHeight(gLevelLayers, &pos, &origY);
        unused = 0;
        if (c) {
            y = origY & 0xFFFFFF00;
            hit = ProbeFloor(quad, &unused);
            {
                u32 xm = dir & PART_DIR_X_MASK;

                if (xm == 0)
                    goto y_probe;
                if (xm == 2)
                    x += (s32)0xFFFFFF00;
                else
                    x += 0x100;
            }
        } else {
            y += 0x100;
            hit = ProbeFloor(quad, &unused);
        }
    }
    mode = dir & PART_DIR_X_MASK;
    if (mode && !hit) {
        s32 span;

        pos = *(struct vec2 *)&Pos();
        span = quad->h - 0x10;
        origX = x;
        OffsetToHitboxEdgeStart(&pos, mode, (void *)quad);
        pos.x = Q8_TO_INT(pos.x);
        pos.y = Q8_TO_INT(pos.y) + 8;
        if ((u8)ProbeTerrain(gLevelLayers, mode, &pos, span, &origX)) {
            hitMask |= mode;
            result |= mode;
            x = origX;
        }
    }
y_probe:
    mode = dir & PART_DIR_Y_MASK;
    if (mode && !hit) {
        s32 span;

        pos = *(struct vec2 *)&Pos();
        span = quad->w;
        origX = x;
        origY = y;
        OffsetToHitboxEdgeStart(&pos, mode, (void *)quad);
        pos.x = Q8_TO_INT(pos.x);
        pos.y = Q8_TO_INT(pos.y);
        if ((u8)ProbeTerrain(gLevelLayers, mode, &pos, span, &origY)) {
            result |= mode;
            hitMask |= mode;
            y = origY;
        }
    }
    mode = dir & PART_DIR_X_MASK;
    if (mode && !hit) {
        s32 span;

        pos = *(struct vec2 *)&Pos();
        span = quad->h;
        origX = x;
        OffsetToHitboxEdgeStart(&pos, mode, (void *)quad);
        pos.x = Q8_TO_INT(pos.x);
        pos.y = Q8_TO_INT(pos.y);
        if ((u8)ProbeTerrain(gLevelLayers, mode, &pos, span, &origX)) {
            hitMask |= mode;
            result |= mode;
            x = origX;
        }
    }
done:
    return result;
}

/* The floor under the sprite: ProbeFloorHeight under the right end of
 * its hitbox's bottom edge (the left end when mirrored), a pixel higher
 * unless it is grounded. A hit snaps `y` to the floor's pixel (a pixel
 * higher again unless it was grounded) and grounds it. A miss while it
 * wasn't grounded retries a pixel lower, and a hit there grounds it too.
 * Otherwise it isn't grounded any more; one that was sets `*outFlag`
 * (it has stepped off an edge). Returns whether the floor is there. */
u8 GroundSprite::ProbeFloor(const struct hitbox_quad *quad, u8 *outFlag)
{
    s32 origY = y;
    struct vec2 pos;
    u8 hit;
    s32 grounded;

    pos = *(struct vec2 *)&Pos();
    OffsetToHitboxEdge(&pos, 8, (void *)quad);
    pos.x = Q8_TO_INT(pos.x);
    pos.y = Q8_TO_INT(pos.y);
    grounded = (f.bytes.flags2 >> 1) & 1;
    if (!grounded)
        pos.y--;
    if (mirrorBits.flipX < 0)
        pos.x -= quad->w >> 1;
    else
        pos.x += quad->w >> 1;
    hit = ProbeFloorHeight(gLevelLayers, &pos, &origY);
    if (hit) {
        y = origY & 0xFFFFFF00;
        grounded = (f.bytes.flags2 >> 1) & 1;
        if (!grounded)
            y -= 0x100;
        f.b.grounded = 1;
        return hit;
    }
    grounded = (f.bytes.flags2 >> 1) & 1;
    if (!grounded) {
        pos.y++;
        hit = ProbeFloorHeight(gLevelLayers, &pos, &origY);
        if (hit) {
            y = origY & 0xFFFFFF00;
            f.b.grounded = 1;
            return hit;
        }
        grounded = (f.bytes.flags2 >> 1) & 1;
    }
    if (grounded)
        *outFlag = 1;
    f.b.grounded = 0;
    return hit;
}
