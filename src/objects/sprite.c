#include "core.h"
#include "match.h"
#include "actor.h"
#include "box_part.h"
#include "gobj_1a794.h"
#include "sprite_bank.h"
#include "util.h"
#include "gfx.h"
#include "objects.h"
#include "memory.h"
#include "level.h"
#include "globals.h"
#include "player.h"

/* `screenSpace` selects whether (x, y) are already screen-relative
 * (nonzero - used as-is) or need the camera-relative conversion
 * WorldToScreen applies (zero - the common case). Either way, the
 * resolved {x, y} pair is forwarded to DrawSpritePieces (parked as
 * NON_MATCHING in src/gfx/graphics.c) to build/queue this part's
 * OAM entries. */
void DrawSpriteAt(void *self, void *part, s32 x, s32 y)
{
    s32 pos[2];

    if (((struct box_part *)part)->screenSpace == 0) {
        WorldToScreen(part, x, y, &pos[0], &pos[1]);
    } else {
        pos[0] = x;
        pos[1] = y;
    }
    DrawSpritePieces(self, part, pos);
}

/* `part`'s own leading {x, y} pair (the same Q8 fixed-point position
 * fields struct actor has at 0x00/0x04) becomes the explicit position
 * passed to DrawSpriteAt - confirms `part` embeds a struct-actor-shaped
 * position at its own start. */
void DrawSprite(void *self, void *part)
{
    DrawSpriteAt(self, part, ((struct box_part *)part)->x >> 8, ((struct box_part *)part)->y >> 8);
}

void DestroySpriteRenderer(void *arg0, u32 arg1)
{
    if (arg1 & 1) {
        OperatorDelete(arg0);
    }
}

/* gSpriteRenderer's empty constructor (InitLevelState), next to its
 * destructor DestroySpriteRenderer. */
void InitSpriteRenderer(void)
{
}

/* Initializes/clears the sprite fields also used by
 * DrawSpritePieces/DrawAffineSpritePieces: the animation state (`anim`,
 * `tag`, `frame`, `stepTimer`, `animDone`; `animating` is set), the
 * `mirror`/`slot` byte pair packed into attr1/attr2 (cleared as one
 * halfword), `affine`, `dir`, and `screenSpace` (the
 * screen-vs-camera-relative flag DrawSpriteAt tests); clears `flags`
 * bits 6/7 and `flags2` bit 2. Written through `struct gobj`
 * (gobj_1a794.h), whose `mirror`/`slot` are plain bytes.
 *
 * Register pins throughout match the ROM's own choices for the two
 * bit-clear sequences (constant computed before the byte load, result
 * landing in the constant's own register - the same accumulator
 * pattern documented at length for `struct actor`'s flags field in
 * graphics.c) and for the final `animating` store (the ROM computes that
 * address into a *fresh* register rather than reusing `part`'s, even
 * though `part` is dead afterward - plain C let the allocator reuse
 * it instead). The running `p` pointer (advanced by `+8` then `+0xb`
 * rather than recomputed from `part` each time) and the shared `zero`
 * local (reused across differently-sized stores instead of
 * rematerializing the constant) both mirror the ROM's own address/
 * value reuse - see docs/matching.md, "Matching decompilation". */
void ResetSpriteObj(void *arg0)
{
    MATCH_HOLD_REG(struct gobj *, part, r3) = arg0;
    MATCH_HOLD_REG(s32, result, r0);
    MATCH_HOLD_REG(s32, tmp, r1);

    result = 0x7f;
    tmp = part->flags;
    result &= tmp;
    tmp = -0x41;
    result &= tmp;
    /* A retyped store: as a plain member store the `zero` below is
     * scheduled above it. */
    *(u8 *)&part->flags = result;

    {
        u8 *p = &part->screenSpace;
        s32 zero = 0;
        *p = zero;
        part->anim = (struct anim_table *)zero;
        p += 8; /* &part->tag */
        *p = zero;
        part->frame = zero;
        part->stepTimer = zero;
        *(u16 *)&part->mirror = zero; /* `mirror` and `slot` */
        p += 0xb;                     /* &part->animDone */
        *p = zero;
    }

    {
        MATCH_HOLD_REG(s32, result2, r0);
        MATCH_HOLD_REG(s32, tmp2, r4);

        result2 = -5;
        tmp2 = part->flags2;
        result2 &= tmp2;
        part->flags2 = result2;
    }

    part->dir = 0;
    part->affine = 0;
    {
        MATCH_HOLD_REG(u8 *, p2, r1) = &part->animating;
        *p2 = 1;
    }
}

/* Builds the AABB (via the shared SetAabbPos set-position/SetAabbSize
 * set-size pair) for `part`'s current animation keyframe, whose box is
 * `box[1]` (+0xc), and mirrors it horizontally/vertically around `part`'s
 * own position per the `mirrorX`/`mirrorY` bits. Returned by value (the hidden
 * return pointer is the `dest` the ROM keeps in r8 and hands back in r0).
 * Matches under old_agbcc - see docs/matching/archive/issue-9-naked-retry.md. */
struct aabb GetSpriteBounds(struct box_part *part)
{
    struct aabb box;
    struct hitbox_quad *pb = &(*part->keyframes)[part->frame].box[1];
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
 * box is `box[0]` (+0x4) instead. */
struct aabb GetSpriteHitbox(struct box_part *part)
{
    struct aabb box;
    struct hitbox_quad *pb = &(*part->keyframes)[part->frame].box[0];
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

/* A third AABB-for-keyframe builder (see GetSpriteBounds/GetSpriteHitbox
 * above), this time selecting its `struct hitbox_quad` from the current
 * sprite frame (`GetSpriteFrame(part)`, sprite_bank.h) rather than
 * `part`'s own keyframe table: the upper nibble of the frame's first
 * piece byte (its layout type; only 0-6 handled - anything above 6 and
 * unhandled 1/2/6 fall through to the same default) selects `box[1]`,
 * `box[0]`, or the fixed fallback `gEmptySpriteBox`. The mirror tests
 * read the byte at +0x28 (`mirrorX`/`mirrorY`) through its address. */
void *GetSpriteAttackBox(void *dest, void *pt)
{
    MATCH_HOLD_REG(struct box_part *, part, r6) = pt;
    struct aabb buf_;
    const struct sprite_frame_3box *info;
    const struct hitbox_quad *rec;
    s32 offX, offY;
    s32 w, h;
    s32 x, y;
    u8 type;

    info = GetSpriteFrame((struct gfx_part *)part);
    type = info->frame.pieces[0] >> 4;
    switch (type) {
    case 0:
    case 3:
    case 4:
        rec = &info->box[1];
        break;
    case 1:
    case 2:
    case 6:
        rec = &gEmptySpriteBox;
        break;
    case 5:
        rec = &info->box[0];
        break;
    default:
        rec = &gEmptySpriteBox;
        break;
    }

    x = part->x >> 8;
    offX = rec->offX;
    y = part->y >> 8;
    offY = rec->offY;
    w = rec->w;
    h = rec->h;

    offX = offX + x;
    offY = offY + y;
    SetAabbPos(&buf_, offX, offY);
    SetAabbSize(&buf_, w, h);

    {
        u8 *flagsAddr = (u8 *)part + 0x28;
        MATCH_HOLD_REG(s32, flags, r1);
        MATCH_HOLD_REG(s32, shifted, r0);

        flags = *flagsAddr;
        shifted = flags << 27;
        if (shifted < 0) {
            buf_.x = (part->x >> 8) * 2 - (buf_.x + buf_.w);
        }
        {
            MATCH_HOLD_REG(s32, addr, r3) = (s32)flagsAddr;
            asm("ldrb %1, [%1]\n\tlsl %0, %1, #0x1a" : "=r"(shifted), "+r"(addr));
        }
        if (shifted < 0) {
            buf_.y = (part->y >> 8) * 2 - (buf_.y + buf_.h);
        }
    }

    *(struct aabb *)dest = buf_;
    return dest;
}

/* Same shape as GetSpriteAttackBox above, with a simpler switch: only
 * `box[0]` or the `gEmptySpriteBox` fallback are ever selected
 * (cases 0/2/3/4/6 to `box[0]`; cases 1/5 and the out-of-range
 * default all to the fallback). */
void *GetSpriteBodyBox(void *dest, void *pt)
{
    MATCH_HOLD_REG(struct box_part *, part, r6) = pt;
    struct aabb buf_;
    const struct sprite_frame_3box *info;
    const struct hitbox_quad *rec;
    s32 offX, offY;
    s32 w, h;
    s32 x, y;
    u8 type;

    info = GetSpriteFrame((struct gfx_part *)part);
    type = info->frame.pieces[0] >> 4;
    switch (type) {
    case 0:
    case 2:
    case 3:
    case 4:
    case 6:
        rec = &info->box[0];
        break;
    case 1:
    case 5:
        rec = &gEmptySpriteBox;
        break;
    default:
        rec = &gEmptySpriteBox;
        break;
    }

    x = part->x >> 8;
    offX = rec->offX;
    y = part->y >> 8;
    offY = rec->offY;
    w = rec->w;
    h = rec->h;

    offX = offX + x;
    offY = offY + y;
    SetAabbPos(&buf_, offX, offY);
    SetAabbSize(&buf_, w, h);

    {
        u8 *flagsAddr = (u8 *)part + 0x28;
        MATCH_HOLD_REG(s32, flags, r1);
        MATCH_HOLD_REG(s32, shifted, r0);

        flags = *flagsAddr;
        shifted = flags << 27;
        if (shifted < 0) {
            buf_.x = (part->x >> 8) * 2 - (buf_.x + buf_.w);
        }
        {
            MATCH_HOLD_REG(s32, addr, r3) = (s32)flagsAddr;
            asm("ldrb %1, [%1]\n\tlsl %0, %1, #0x1a" : "=r"(shifted), "+r"(addr));
        }
        if (shifted < 0) {
            buf_.y = (part->y >> 8) * 2 - (buf_.y + buf_.h);
        }
    }

    *(struct aabb *)dest = buf_;
    return dest;
}

/* `part` (a `struct actor`, same layout used throughout this ROM
 * region) collides with the player (`gPlayer`, tested via
 * two `GetSpriteHitbox` AABBs and `AabbOverlaps`) and, if so, plays a sound
 * at the player's position (the `table+0x68` offset/dead-read idiom
 * matches CheckEntityPlayerContact's `_call_via_r4` call exactly, just keyed off
 * `part->kind`) and marks itself
 * "collected" (`gEntityFlags` bitmap, same convention as
 * MarkEntityGone). `part->kind - 0x1b` (0-7) then selects a "kind" to
 * spawn via `SpawnEffectPart` at `part`'s own position - case 1 and any
 * out-of-range value spawn nothing. If something spawned, its
 * `mode` is set to 1 and its `visible` bit cleared.
 *
 * Real C under old_agbcc (issue #9-#11 NAKED retry; the whole file
 * matches under it, so sprite.o is in OLD_AGBCC_OBJS - old_agbcc
 * is also what puts the cached player global in r7). Two details: the
 * flag tests' constant 1 is a variable pinned to r6 and assigned inside
 * the first test (`& (one = 1)`), and the `gone` OR uses it (`orrs r0,
 * r6`) - as a plain constant CSE rematerializes it; and the spawned
 * part's `mode = 1` goes through a `u32` local, which materializes the
 * 1 before the `-4` mask as the ROM does. */

struct collect_part {
    s32 x;
    s32 y;
    u16 id;  // 0x08
    u8 kind; // 0x0A
    u8 unk_0B;
    u8 gone:1; // 0x0C
    u8 unk_0C_1:1;
    u8 visible:1;
    u8 hit:1;
    u8 unk_0C_4:3;
    u8 solid:1;
    u8 unk_0D[0xb];
    u8 *vtable; // 0x18
    u8 unk_1C[0xc];
    u8 mode:2; // 0x28
    u8 unk_28_2:6;
};

#define COLLECT_FLAGS(p) (*((u8 *)(p) + 0xc))

static inline struct collect_part *SpawnPickup(s32 kind, s32 x, s32 y)
{
    return SpawnEffectPart(gEntitySpawner, 0x2b, kind, x, y, 0);
}

s32 CheckSpritePickup(struct collect_part *part)
{
    struct aabb a, b;
    struct player *player;
    struct collect_part *spawned;
    u32 flags = COLLECT_FLAGS(part) << 24;
    MATCH_HOLD_REG(u32, one, r6);

    if (!((flags >> 27) & (one = 1)) && ((flags >> 26) & one)) {
        a = GetSpriteHitbox((struct box_part *)part);
        if (gPlayer->flags.all >> 7) {
            b = GetSpriteHitbox((struct box_part *)gPlayer);
            if (AabbOverlaps(&b, &a)) {
                COLLECT_FLAGS(part) |= 8;
                player = gPlayer;
                {
                    const struct actor_method *m =
                        (const struct actor_method *)&player->vtable->handleEvent;
                    ((void (*)(void *, s32, s32, s32))m->fn)((u8 *)player + m->thisOffset, 0,
                                                             part->kind, 0);
                }
                COLLECT_FLAGS(part) |= one;
                if (part->id != 0xFFFF)
                    do {
                        s32 id = part->id;
                        u8 *base = (u8 *)gEntityFlags;
                        s32 word = id / 32;
                        s32 off = word * 4;
                        u32 *slot = (u32 *)(base + 0x108);

                        slot = (u32 *)((u8 *)slot + off);
                        *slot |= 1 << (id - word * 32);
                    } while (0);

                spawned = NULL;
                switch (part->kind) {
                case 0x1d:
                case 0x1e:
                    spawned = SpawnPickup(1, part->x >> 8, part->y >> 8);
                    break;
                case 0x21:
                    spawned = SpawnPickup(6, part->x >> 8, part->y >> 8);
                    break;
                case 0x1f:
                    spawned = SpawnPickup(5, part->x >> 8, part->y >> 8);
                    break;
                case 0x22:
                    spawned = SpawnPickup(0, part->x >> 8, part->y >> 8);
                    break;
                case 0x20:
                    spawned = SpawnPickup(3, part->x >> 8, part->y >> 8);
                    break;
                case 0x1b:
                    spawned = SpawnPickup(4, part->x >> 8, part->y >> 8);
                    break;
                }
                if (spawned) {
                    u32 m1 = 1;

                    spawned->mode = m1;
                    spawned->visible = 0;
                }
            }
        }
    }
    return 0;
}

/* Same shape as IsEntityNearCamera (graphics.c) - `screenSpace == 1` is a
 * fast "always visible" override; otherwise `flags2` bit 2 gates an
 * on-screen check via `_call_via_r2`, using a 4-word "region" of
 * `{layer 0's x and y in Q8, 240<<8, 160<<8}`
 * (the GBA's screen width/height) and the same
 * `table+N`/`table+N+4` offset/pointer slot pair convention
 * IsEntityNearCamera reads at `table+0x40`, here at `table+0x30` (the
 * part's method-table entry, see PART_METHOD). */
s32 IsSpriteObjOnScreen(struct box_part *part)
{
    MATCH_HOLD_REG(s32, result, r3) = 0;

    if (part->screenSpace == 1) {
        return 1;
    }

    {
        MATCH_HOLD_REG(u32, flags, r1);
        MATCH_HOLD_REG(s32, bit2, r0);

        flags = part->flags2;
        bit2 = (flags >> 2) & 1;
        if (!bit2) {
            s32 buf[4];
            MATCH_HOLD_REG(struct bg_scroll_layer *, cam, r0);
            struct part_method *table;

            cam = gLevelLayers->layer0;
            {
                s32 x = cam->x << 8;
                s32 y = cam->y << 8;

                buf[0] = x;
                buf[1] = y;
            }
            {
                s32 width = 0xf0 << 8;
                s32 height = 0xa0 << 8;

                buf[2] = width;
                buf[3] = height;
            }

            table = PART_METHOD(part, 0x30);
            result = (u8)_call_via_r2((u8 *)part + table->thisOffset, buf, table->fn);
        }
    }
    return result;
}

/* Same `screenSpace`/`flags2` bit-2 fast-path shape as IsSpriteObjOnScreen
 * above, but the real check is an AABB-overlap test: `part`'s own box
 * (via GetSpriteBounds) against `region`'s `{s32 x, y, w, h}`. */
s32 SpriteObjOverlapsRect(struct actor *part, struct aabb *region)
{
    MATCH_HOLD_REG(s32, earlyResult, r3) = 0;

    if (((struct box_part *)part)->screenSpace == 1) {
        return 1;
    }

    {
        MATCH_HOLD_REG(u32, flags, r1);
        MATCH_HOLD_REG(s32, bit2, r0);

        flags = ((struct box_part *)part)->flags2;
        bit2 = (flags >> 2) & 1;
        if (bit2) {
            return earlyResult;
        }
    }

    {
        struct aabb box;
        s32 x1, y1, x2, y2;
        s32 result;

        box = GetSpriteBounds((struct box_part *)part);

        x1 = box.x << 8;
        y1 = box.y << 8;
        x2 = x1 + (box.w << 8);
        y2 = y1 + (box.h << 8);

        result = 0;
        if (x2 > region->x) {
            if (x1 < region->x + region->w) {
                if (y2 > region->y) {
                    if (y1 < region->y + region->h) {
                        result = 1;
                    }
                }
            }
        }
        earlyResult = result;
        return earlyResult;
    }
}

/* Advances `part`'s per-keyframe animation timer by one tick, only
 * while `animating` is set. `timer` counts up each tick against the
 * current keyframe's `duration`; once it reaches it, `timer` resets and
 * `tick` (the step index) advances. When `tick` reaches the keyframe's
 * `steps`, both counters reset and - unless the keyframe's loop flag
 * (bit 1) is set - `animDone` is set.
 *
 * Matches under old_agbcc (see docs/matching/archive/issue-9-naked-retry.md).
 * The ROM keeps `part` in `ip` for the whole function; that falls out
 * naturally from this plain shape under the old compiler. The two
 * details the shape pins down: the second half reads `tick` into a
 * local and then `*keyframes` once into `kf` (reused by the flag test,
 * while `frame` is re-read each time), and `animDone` is stored from a
 * local (`movs r1, #1` before the address, not after). */
void AdvanceSpriteAnim(struct box_part *part)
{
    if (part->animating) {
        s32 timer = part->timer;
        s32 tick;
        struct keyframe *kf;

        if (timer < (*part->keyframes)[part->frame].duration)
            part->timer = timer + 1;
        else {
            part->timer = 0;
            part->tick++;
        }

        tick = part->tick;
        kf = *part->keyframes;
        if (tick >= kf[part->frame].steps) {
            part->tick = 0;
            part->timer = 0;
            if (!(kf[part->frame].flags & 2)) {
                u8 done = TRUE;
                part->animDone = done;
            }
        }
    }
}
