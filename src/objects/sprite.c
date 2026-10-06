#include "core.h"
#include "actor.h"
#include "box_part.h"
#include "util.h"
#include "gfx.h"
#include "objects.h"
#include "memory.h"
#include "level.h"
#include "globals.h"
#include "player.h"

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

/* Builds the AABB (via the shared SetAabbPos set-position/SetAabbSize
 * set-size pair) for `part`'s current animation keyframe, whose box sits
 * at record+0xc, and mirrors it horizontally/vertically around `part`'s
 * own position per the 0x28 mirror bits. Returned by value (the hidden
 * return pointer is the `dest` the ROM keeps in r8 and hands back in r0).
 * Matches under old_agbcc - see docs/matching/archive/issue-9-naked-retry.md. */
struct aabb GetSpriteBounds(struct box_part *part)
{
    struct aabb box;
    u8 *rec = (u8 *)&(*part->keyframes)[part->frame];
    struct hitbox_quad *pb = (struct hitbox_quad *)(rec + 0xc);
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
    struct hitbox_quad *pb = (struct hitbox_quad *)(rec + 4);
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

/* A third AABB-for-keyframe builder (see GetSpriteBounds/GetSpriteHitbox
 * above), this time selecting its 6-byte
 * `{s16 x, s16 y, u8 w, u8 h}` record via a `GetSpriteFrame(part)`-derived
 * "info" struct rather than `part`'s own keyframe table pointer:
 * `info+4` points to a byte whose upper nibble (0-15, but only 0-6
 * handled - anything above 6 and unhandled 1/2/6 fall through to the
 * same default) selects one of `info+0x14`, `info+0xc`, or the fixed
 * fallback table `gEmptySpriteBox`. */
void *GetSpriteAttackBox(void *dest, void *pt)
{
    register void *part asm("r6") = pt;
    struct aabb buf_;
    void *info;
    void *rec;
    s32 offX, offY;
    s32 w, h;
    s32 x, y;
    u8 type;

    info = GetSpriteFrame(part);
    type = *(u8 *)(*(void **)((u8 *)info + 4)) >> 4;
    switch (type) {
    case 0:
    case 3:
    case 4:
        rec = (u8 *)info + 0x14;
        break;
    case 1:
    case 2:
    case 6:
        rec = (void *)&gEmptySpriteBox;
        break;
    case 5:
        rec = (u8 *)info + 0xc;
        break;
    default:
        rec = (void *)&gEmptySpriteBox;
        break;
    }

    x = *(s32 *)part >> 8;
    offX = *(s16 *)((u8 *)rec + 0);
    y = *(s32 *)((u8 *)part + 4) >> 8;
    offY = *(s16 *)((u8 *)rec + 2);
    w = *((u8 *)rec + 4);
    h = *((u8 *)rec + 5);

    offX = offX + x;
    offY = offY + y;
    SetAabbPos(&buf_, offX, offY);
    SetAabbSize(&buf_, w, h);

    {
        u8 *flagsAddr = (u8 *)part + 0x28;
        register s32 flags asm("r1");
        register s32 shifted asm("r0");

        flags = *flagsAddr;
        shifted = flags << 27;
        if (shifted < 0) {
            buf_.x = (*(s32 *)part >> 8) * 2 - (buf_.x + buf_.w);
        }
        {
            register s32 addr asm("r3") = (s32)flagsAddr;
            asm("ldrb %1, [%1]\n\tlsl %0, %1, #0x1a" : "=r" (shifted), "+r" (addr));
        }
        if (shifted < 0) {
            buf_.y = (*(s32 *)((u8 *)part + 4) >> 8) * 2 - (buf_.y + buf_.h);
        }
    }

    *(struct aabb *)dest = buf_;
    return dest;
}

/* Same shape as GetSpriteAttackBox above, with a simpler switch: only
 * `info+0xc` or the `gEmptySpriteBox` fallback are ever selected
 * (cases 0/2/3/4/6 to `info+0xc`; cases 1/5 and the out-of-range
 * default all to the fallback). */
void *GetSpriteBodyBox(void *dest, void *pt)
{
    register void *part asm("r6") = pt;
    struct aabb buf_;
    void *info;
    void *rec;
    s32 offX, offY;
    s32 w, h;
    s32 x, y;
    u8 type;

    info = GetSpriteFrame(part);
    type = *(u8 *)(*(void **)((u8 *)info + 4)) >> 4;
    switch (type) {
    case 0:
    case 2:
    case 3:
    case 4:
    case 6:
        rec = (u8 *)info + 0xc;
        break;
    case 1:
    case 5:
        rec = (void *)&gEmptySpriteBox;
        break;
    default:
        rec = (void *)&gEmptySpriteBox;
        break;
    }

    x = *(s32 *)part >> 8;
    offX = *(s16 *)((u8 *)rec + 0);
    y = *(s32 *)((u8 *)part + 4) >> 8;
    offY = *(s16 *)((u8 *)rec + 2);
    w = *((u8 *)rec + 4);
    h = *((u8 *)rec + 5);

    offX = offX + x;
    offY = offY + y;
    SetAabbPos(&buf_, offX, offY);
    SetAabbSize(&buf_, w, h);

    {
        u8 *flagsAddr = (u8 *)part + 0x28;
        register s32 flags asm("r1");
        register s32 shifted asm("r0");

        flags = *flagsAddr;
        shifted = flags << 27;
        if (shifted < 0) {
            buf_.x = (*(s32 *)part >> 8) * 2 - (buf_.x + buf_.w);
        }
        {
            register s32 addr asm("r3") = (s32)flagsAddr;
            asm("ldrb %1, [%1]\n\tlsl %0, %1, #0x1a" : "=r" (shifted), "+r" (addr));
        }
        if (shifted < 0) {
            buf_.y = (*(s32 *)((u8 *)part + 4) >> 8) * 2 - (buf_.y + buf_.h);
        }
    }

    *(struct aabb *)dest = buf_;
    return dest;
}

extern void _call_via_r4(void *arg0, s32 arg1, s32 arg2, s32 arg3);

/* `part` (a `struct actor`, same layout used throughout this ROM
 * region) collides with the player (`gPlayer`, tested via
 * two `GetSpriteHitbox` AABBs and `AabbOverlaps`) and, if so, plays a sound
 * at the player's position (the `table+0x68` offset/dead-read idiom
 * matches CheckEntityPlayerContact's `_call_via_r4` call exactly, just keyed off
 * `part->field_0A` instead of `self->field_0A`) and marks itself
 * "collected" (`gEntityFlags` bitmap, same convention as
 * MarkEntityGone). `part->field_0A - 0x1b` (0-7) then selects a "kind" to
 * spawn via `SpawnEffectPart` at `part`'s own position - case 1 and any
 * out-of-range value spawn nothing. If something spawned, its
 * `+0x28`/`+0xc` flag bytes get tagged - kept as raw offsets since the
 * spawned object's own type isn't established yet.
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
    u16 id;             // 0x08
    u8 kind;            // 0x0A
    u8 unk_0B;
    u8 gone:1;          // 0x0C
    u8 unk_0C_1:1;
    u8 visible:1;
    u8 hit:1;
    u8 unk_0C_4:3;
    u8 solid:1;
    u8 unk_0D[0xb];
    u8 *vtable;         // 0x18
    u8 unk_1C[0xc];
    u8 mode:2;          // 0x28
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
    register u32 one asm("r6");

    if (!((flags >> 27) & (one = 1)) && ((flags >> 26) & one)) {
        a = GetSpriteHitbox((struct box_part *)part);
        if (gPlayer->flags.all >> 7) {
            b = GetSpriteHitbox((struct box_part *)gPlayer);
            if (AabbOverlaps(&b, &a)) {
                COLLECT_FLAGS(part) |= 8;
                player = gPlayer;
                {
                    const struct actor_method *m = (const struct actor_method *)&player->vtable->handleEvent;
                    ((void (*)(void *, s32, s32, s32))m->fn)((u8 *)player + m->thisOffset, 0, part->kind, 0);
                }
                COLLECT_FLAGS(part) |= one;
                if (part->id != 0xFFFF) do {
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

extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);

/* Same shape as IsEntityNearCamera (graphics.c) - `part+0x25 == 1` is a fast
 * "always visible" override; otherwise `part+0xd` bit 2 gates an
 * on-screen check via `_call_via_r2`, using a 4-word "region" of
 * `{gLevelLayers's sub-object's two Q8 fields, 240<<8, 160<<8}`
 * (the GBA's screen width/height) and the same
 * `table+N`/`table+N+4` offset/pointer slot pair convention
 * IsEntityNearCamera reads at `table+0x40`, here at `table+0x30` (the
 * part's method-table entry, see PART_METHOD). */
s32 IsSpriteObjOnScreen(struct box_part *part)
{
    register s32 result asm("r3") = 0;

    if (*((u8 *)part + 0x25) == 1) {
        return 1;
    }

    {
        register u32 flags asm("r1");
        register s32 bit2 asm("r0");

        flags = *((u8 *)part + 0xd);
        bit2 = (flags >> 2) & 1;
        if (!bit2) {
            s32 buf[4];
            register void *subObj asm("r0");
            struct part_method *table;

            subObj = gLevelLayers->layer0;
            {
                s32 field0 = *(s32 *)subObj << 8;
                s32 field4 = *(s32 *)((u8 *)subObj + 4) << 8;

                buf[0] = field0;
                buf[1] = field4;
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

/* Same `part+0x25`/`part+0xd` bit-2 fast-path shape as IsSpriteObjOnScreen
 * above, but the real check is an AABB-overlap test: `part`'s own box
 * (via GetSpriteBounds) against `region`'s `{s32 x, y, w, h}`. */
s32 SpriteObjOverlapsRect(struct actor *part, struct aabb *region)
{
    register s32 earlyResult asm("r3") = 0;

    if (*((u8 *)part + 0x25) == 1) {
        return 1;
    }

    {
        register u32 flags asm("r1");
        register s32 bit2 asm("r0");

        flags = *((u8 *)part + 0xd);
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
asm(".align 2, 0");

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
