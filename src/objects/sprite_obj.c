#include "core.h"
#include "math_util.h"
#include "match.h"
#include "actor.h"
#include "vram_pool.h"
#include "gfx_part.h"
#include "box_part.h"
#include "sprite_bank.h"
#include "aabb.h"
#include "util.h"
#include "gfx.h"
#include "objects.h"
#include "memory.h"
#include "level.h"
#include "globals.h"

/* Builds `part`'s AABB (same keyframe-table shape/record layout as
 * GetSpriteHitbox, inlined directly here rather than calling it - this
 * function needs the box on the stack for the final overlap test, not
 * written out through a `dest` pointer) and mirrors it per
 * `mirrorX`/`mirrorY`, then tests it for overlap against `region` via
 * `AabbOverlaps` - the same collision-test function `CheckSpritePickup`
 * uses. */
s32 SpriteHitboxOverlaps(struct actor *self, void *region)
{
    struct box_part *part = (struct box_part *)self;
    struct aabb buf_;
    struct keyframe **tablePtr;
    MATCH_HOLD_REG(struct keyframe *, rec, r0);
    MATCH_HOLD_REG(u8, idx, r3);
    struct hitbox_quad *box;
    s32 offset;
    s32 offX, offY;
    s32 w, h;
    s32 x, y;
    s32 xpos, ypos;
    s32 mirrorX, mirrorY;

    mirrorX = part->mirrorX;
    mirrorY = part->mirrorY;

    xpos = Q8_TO_INT(part->x);
    ypos = Q8_TO_INT(part->y);

    tablePtr = part->keyframes;
    idx = part->frame;
    offset = idx * KEYFRAME_SIZE;
    rec = *tablePtr;
    rec = (struct keyframe *)((u8 *)rec + offset);
    box = &rec->box[0];

    offX = rec->box[0].offX; /* through `rec`, not `box`: the ROM's [rec + 4] */
    offY = box->offY;
    w = box->w;
    h = box->h;

    x = offX + xpos;
    y = offY + ypos;
    SetAabbPos(&buf_, x, y);
    SetAabbSize(&buf_, w, h);

    if (mirrorX) {
        buf_.x = xpos * 2 - (buf_.x + buf_.w);
    }
    if (mirrorY) {
        buf_.y = ypos * 2 - (buf_.y + buf_.h);
    }

    return (u8)AabbOverlaps(&buf_, region);
}

/* Reads `part`'s current keyframe record's `paletteId` as a
 * `GetPaletteSlot` record id, looked up against the global tile-asset
 * cache `gPaletteCache`. */
s32 GetSpriteAnimPaletteSlot(struct actor *self)
{
    struct box_part *part = (struct box_part *)self;
    struct palette_cache *cache = gPaletteCache;
    struct keyframe **tablePtr;
    struct keyframe *table;
    u8 *idxAddr;
    MATCH_HOLD_REG(u8, idx, r4);
    MATCH_HOLD_REG(s32, rec, r1);

    tablePtr = part->keyframes;
    idxAddr = &part->frame;
    table = *tablePtr;
    idx = *idxAddr;
    rec = idx * KEYFRAME_SIZE;
    rec = rec + (s32)table;
    rec = ((struct keyframe *)rec)->paletteId;

    return (u8)GetPaletteSlot(cache, rec);
}

/* UNUSED - no caller anywhere in the ROM (checked src/, asm/ and the
 * data tables). The inverse of `OffsetToHitboxEdge`: moves the Q8
 * position `dest` back from the edge of hitbox `rec` (`struct
 * hitbox_quad`) that faces direction
 * `kind` (ProbeTerrain's 1 right, 2 left, 4 up, 8 down) to the object's
 * origin.
 *
 * Adjusts `dest`'s `{x, y}` per `kind` (`kind-1` is the real switch selector, 0-11;
 * anything else - including the four explicit no-op cases 2/4/5/6/8/9/
 * 10 - does nothing): kind 1/2 add/subtract `rec->w` (Q8 shifted by 7,
 * so half the width) from `dest->x`; kind 4 subtracts `rec->offY`
 * (shifted by 8, full Q8) from `dest->y`; kinds 8/12 do the same but
 * add `rec->h` to `rec->offY` first.
 *
 * Matched in a later session than the original NAKED transcription
 * (see docs/matching.md's "Parked, not matched: OffsetFromHitboxEdge" for the
 * original account and the five techniques that failed against it).
 * The holdout was always the shared kind-8/12 block's register-register
 * `add` (ROM's `adds r1, r2, r1` versus this compiler's always-
 * canonicalized `adds r1, r1, r2`), and an inline-asm anchor for just
 * that one instruction had previously broken the kind-8/12 case-body
 * merging (the compiler stopped recognizing the two switch cases'
 * bodies as identical once one of them contained an opaque asm
 * statement, so it quit merging them into one physical block). The fix
 * that finally worked: route both `case 8` and `case 12` to one shared
 * `goto` target (`kind8_12`) that holds the *entire* load+load+add
 * sequence as a single atomic `asm volatile` block, rather than two
 * switch-case bodies the optimizer has to independently notice are
 * identical - there is exactly one occurrence of this code at the C
 * source level, so there is nothing left for a cross-jump/tail-merging
 * pass to decide about. Every other switch case is likewise routed
 * through a bare `goto` to its own labeled block outside the switch
 * (rather than holding real code inside the switch itself) - this
 * turned out to matter for exact block *ordering* too (see
 * `OffsetToHitboxEdgeStart` below, where a `case` body with real code physically
 * displaced a fallthrough block gcc would otherwise have placed
 * directly after its neighbor). */
void OffsetFromHitboxEdge(void *destArg, s32 kind, void *recArg)
{
    struct gfx_vec *dest = destArg;
    struct hitbox_quad *rec = recArg;
    s32 idx = kind - 1;

    switch (idx) {
    case 1:
        {
            MATCH_HOLD_REG(s32, byteVal, r2) = rec->w;
            MATCH_HOLD_REG(s32, shifted, r1);

            shifted = byteVal << 7;
            dest->x += shifted;
        }
        goto end;
    case 0:
        {
            MATCH_HOLD_REG(s32, byteVal, r2) = rec->w;
            MATCH_HOLD_REG(s32, shifted, r1);

            shifted = byteVal << 7;
            dest->x -= shifted;
        }
        goto end;
    case 2:
        goto end;
    case 3:
        {
            s32 v = rec->offY;
            v = INT_TO_Q8(v);
            dest->y -= v;
        }
        goto end;
    case 4:
        goto end;
    case 5:
        goto end;
    case 6:
        goto end;
    case 7:
        goto kind8_12;
    case 8:
        goto end;
    case 9:
        goto end;
    case 10:
        goto end;
    case 11:
        goto kind8_12;
    default:
        goto end;
    }

kind8_12:
    {
        MATCH_HOLD_REG(struct hitbox_quad *, recReg, r2) = rec;
        MATCH_HOLD_REG(s32, result, r1);

        // clang-format off
        asm volatile(
            "mov r0, #2\n\t"
            "ldrsh r1, [r2, r0]\n\t"
            "ldrb r2, [r2, #5]\n\t"
            "add r1, r2, r1\n\t"
            : "=r"(result), "+r"(recReg)
            :
            : "r0"
        );
        // clang-format on

        result = INT_TO_Q8(result);
        dest->y -= result;
    }
end:
    return;
}

/* Moves the Q8 position `dest` (an object's origin) to the edge of its
 * hitbox `rec` that faces direction `kind` (ProbeTerrain's 1 right, 2
 * left, 4 up, 8 down): x +/- w/2, or y + yOff (top) / y + yOff + h
 * (bottom). ProbeGroundSpriteFloor/ProbeGroundSpriteTerrain use it with 8 for the
 * point under the object's feet.
 *
 * Same shape as `OffsetFromHitboxEdge` above (mirror-image add/subtract
 * directions: kind 1/2 do the opposite sign on `dest->x`, and
 * kinds 4/8/12 add to `dest->y` instead of subtracting).
 *
 * Matched in a later session, same `goto`-unified-block technique as
 * `OffsetFromHitboxEdge` above - see its doc comment for the full account of
 * why the shared kind-8/12 block needs a single atomic `asm volatile`
 * reached via `goto` from both `case 8` and `case 12`, rather than an
 * asm anchor on just the `add` inside two ordinary switch-case
 * bodies. */
void OffsetToHitboxEdge(void *destArg, s32 kind, void *recArg)
{
    struct gfx_vec *dest = destArg;
    struct hitbox_quad *rec = recArg;
    s32 idx = kind - 1;

    switch (idx) {
    case 1:
        {
            MATCH_HOLD_REG(s32, byteVal, r2) = rec->w;
            MATCH_HOLD_REG(s32, shifted, r1);

            shifted = byteVal << 7;
            dest->x -= shifted;
        }
        goto end;
    case 0:
        {
            MATCH_HOLD_REG(s32, byteVal, r2) = rec->w;
            MATCH_HOLD_REG(s32, shifted, r1);

            shifted = byteVal << 7;
            dest->x += shifted;
        }
        goto end;
    case 2:
        goto end;
    case 3:
        {
            s32 v = rec->offY;
            v = INT_TO_Q8(v);
            dest->y += v;
        }
        goto end;
    case 4:
        goto end;
    case 5:
        goto end;
    case 6:
        goto end;
    case 7:
        goto kind8_12;
    case 8:
        goto end;
    case 9:
        goto end;
    case 10:
        goto end;
    case 11:
        goto kind8_12;
    default:
        goto end;
    }

kind8_12:
    {
        MATCH_HOLD_REG(struct hitbox_quad *, recReg, r2) = rec;
        MATCH_HOLD_REG(s32, result, r1);

        // clang-format off
        asm volatile(
            "mov r0, #2\n\t"
            "ldrsh r1, [r2, r0]\n\t"
            "ldrb r2, [r2, #5]\n\t"
            "add r1, r2, r1\n\t"
            : "=r"(result), "+r"(recReg)
            :
            : "r0"
        );
        // clang-format on

        result = INT_TO_Q8(result);
        dest->y += result;
    }
end:
    return;
}

/* Moves the Q8 position `dest` to the start of the hitbox edge that
 * faces direction `kind`, the point a ProbeTerrain scan along that edge
 * starts from: for 1/2 (right/left) the top end of the side edge (x +/-
 * w/2, y + yOff; the scan runs down over `h`), for 4/8 (up/down) the left
 * end of the top or bottom edge (x - w/2; the scan runs right over `w`).
 *
 * A third variant of `OffsetFromHitboxEdge`'s shape: kind 1/2 update
 * `dest->x` (sub/add) AND unconditionally also add `rec->offY` (Q8)
 * to `dest->y`; kinds 4/8/12 add to `dest->y` (same as
 * `OffsetFromHitboxEdge`'s kinds) AND additionally always subtract
 * `rec->w` (Q8, `<<7`) from `dest->x` afterward.
 *
 * Matched in a later session, same `goto`-unified atomic-asm-block
 * technique as `OffsetFromHitboxEdge` above for the shared kind-8/12 gap.
 * `rec` itself also needs its own explicit `MATCH_HOLD_REG(..., r2)`
 * pin (initialized from the incoming parameter right at function
 * entry) - without it, gcc decided `rec` needed to survive in a
 * callee-saved register (`r4`, behind a `push {r4, lr}`/`pop {r4}`
 * the ROM doesn't have) once its live range was forced to span every
 * `goto`-connected block by this restructuring; pinning it to `r2` up
 * front (its own natural incoming register, and the register the ROM
 * itself keeps it in end to end) removed the need for that spill
 * entirely. Also needed every switch case - even the two with real
 * code (`case 0`/`case 1`) - to hold nothing but a bare `goto` to a
 * block declared *outside* the switch, rather than the code directly:
 * an earlier draft that gave `case 0`'s body directly (so it could
 * fall through to the shared field_4 update without an explicit
 * branch, matching the ROM) still had `case 3`'s real code physically
 * appear between the `case 0`/`case 1` blocks and that shared
 * fallthrough target in the compiler's block layout (gcc places a
 * switch's in-place case bodies in switch-declaration order, ahead of
 * any post-switch fallthrough code, regardless of where a `goto`
 * target label textually sits) - forcing a spurious extra branch
 * where the ROM has a genuine fallthrough. Moving every case's real
 * code out to its own `goto` target, laid out in the exact order the
 * ROM's own blocks appear, fixed the layout without changing anything
 * about the kind-8/12 fix itself. */
void OffsetToHitboxEdgeStart(void *destArg, s32 kind, void *recArg)
{
    struct gfx_vec *dest = destArg;
    MATCH_HOLD_REG(struct hitbox_quad *, rec, r2) = recArg;
    MATCH_HOLD_REG(s32, field0, r0);
    s32 idx = kind - 1;
    s32 v;

    switch (idx) {
    case 1:
        goto subCase;
    case 0:
        goto addCase;
    case 2:
        goto end;
    case 3:
        goto kind4Case;
    case 4:
        goto end;
    case 5:
        goto end;
    case 6:
        goto end;
    case 7:
        goto kind8_12;
    case 8:
        goto end;
    case 9:
        goto end;
    case 10:
        goto end;
    case 11:
        goto kind8_12;
    default:
        goto end;
    }

subCase:
    {
        MATCH_HOLD_REG(s32, byteVal, r0) = rec->w;
        MATCH_HOLD_REG(s32, shifted, r1);

        shifted = byteVal << 7;
        field0 = dest->x - shifted;
    }
    goto field4tail;

addCase:
    {
        MATCH_HOLD_REG(s32, byteVal, r0) = rec->w;
        MATCH_HOLD_REG(s32, shifted, r1);

        shifted = byteVal << 7;
        field0 = dest->x + shifted;
    }

field4tail:
    dest->x = field0;
    {
        s32 v2 = rec->offY;
        v2 = INT_TO_Q8(v2);
        dest->y += v2;
    }
    goto end;

kind4Case:
    v = rec->offY;
    goto bigtail;

kind8_12:
    {
        MATCH_HOLD_REG(s32, result, r1);

        // clang-format off
        asm volatile(
            "mov r0, #2\n\t"
            "ldrsh r1, [r2, r0]\n\t"
            "ldrb r0, [r2, #5]\n\t"
            "add r1, r0, r1\n\t"
            : "=r"(result)
            : "r"(rec)
            : "r0"
        );
        // clang-format on
        v = result;
    }

bigtail:
    v = INT_TO_Q8(v);
    dest->y += v;
    {
        MATCH_HOLD_REG(s32, byteVal, r2) = rec->w;
        MATCH_HOLD_REG(s32, shifted, r1);
        MATCH_HOLD_REG(s32, field0b, r0);

        shifted = byteVal << 7;
        field0b = dest->x;
        field0b -= shifted;
        dest->x = field0b;
    }
end:
    return;
}

/* `screenSpace == 1` is the same fast override seen in
 * IsSpriteObjOnScreen/SpriteObjOverlapsRect; otherwise defers to `IsEntityInsideRect` (already
 * matched in graphics.c), forwarding `box` straight through
 * unmodified. */
s32 IsSpriteObjInsideRect(struct actor *part, void *box)
{
    s32 result = 0;
    MATCH_HOLD_REG(u8 *, addr, r2) = &((struct box_part *)part)->screenSpace;
    MATCH_HOLD_REG(u8, byteVal, r2);

    byteVal = *addr;
    if (byteVal == 1) {
        result = 1;
    } else if ((u8)IsEntityInsideRect(part, box)) {
        result = 1;
    }
    return result;
}

/* Same `screenSpace` fast-override shape as `IsSpriteObjInsideRect` above,
 * deferring to `IsEntityNearCamera` (already matched in `graphics.c`)
 * instead - a single-argument sibling, so the address scratch
 * naturally lands in `r1` instead of `r2` (no second call argument to
 * keep out of the way). */
s32 IsSpriteObjNearCamera(struct actor *part)
{
    s32 result = 0;
    MATCH_HOLD_REG(u8 *, addr, r1) = &((struct box_part *)part)->screenSpace;
    MATCH_HOLD_REG(u8, byteVal, r1);

    byteVal = *addr;
    if (byteVal == 1) {
        result = 1;
    } else if (IsEntityNearCamera(part)) {
        result = 1;
    }
    return result;
}

/* Always-true stub. */
s32 ApplySpriteObjVelocity(void)
{
    return 1;
}

/* Tail-calls `DrawSprite` (already matched in `sprite.c`) with
 * the global `gSpriteRenderer` as `self`. */
void DrawSpriteObj(void *part)
{
    DrawSprite(gSpriteRenderer, part);
}

extern void *_call_via_r1(void *arg0, void *arg1);

/* Advances `part`'s animation timer (`AdvanceSpriteAnim`), then calls two
 * of its methods (PART_METHOD, the same convention as
 * `IsEntityNearCamera`/`IsSpriteObjOnScreen`) through `_call_via_r1`:
 * the one at +0x60 first, then the one at +8. */
void UpdateSpriteObj(struct actor *part)
{
    AdvanceSpriteAnim((struct box_part *)part);

    {
        struct part_method *m = PART_METHOD((struct box_part *)part, 0x60);
        s32 offset = m->thisOffset;
        void *addr = (u8 *)part + offset;
        void *ptr = m->fn;

        _call_via_r1(addr, ptr);
    }
    {
        struct part_method *m = PART_METHOD((struct box_part *)part, 8);
        s32 offset = m->thisOffset;
        void *addr = (u8 *)part + offset;
        void *ptr = m->fn;

        _call_via_r1(addr, ptr);
    }
}

/* Returns a pointer to `part`'s current keyframe record's `box[0]` -
 * the same keyframe-table lookup used throughout this ROM region. */
void *GetSpriteObjHitbox(struct actor *part)
{
    MATCH_HOLD_REG(struct keyframe **, tablePtr, r2) = ((struct box_part *)part)->keyframes;
    MATCH_HOLD_REG(u8, idx, r3) = ((struct box_part *)part)->frame;
    s32 offset = idx * KEYFRAME_SIZE;
    struct keyframe *table = *tablePtr;
    struct keyframe *rec = (struct keyframe *)((u8 *)table + offset);
    return &rec->box[0];
}

/* Ignores its `part` argument entirely (the ROM never reads r0 before
 * overwriting it) - already declared with this signature at its
 * `DrawSpritePieces` call site in graphics.c. Returns the sprite bank
 * table's `tileBase`. */
s32 GetSpriteTileBase(void *part)
{
    return (s32)gSpriteBankSet->table->tileBase;
}

/* Looks up `part`'s current keyframe record (same convention as
 * elsewhere in this ROM region). If `animDone` (set by
 * `AdvanceSpriteAnim`) is set and the record's SPRITE_ANIM_LOOP flag is
 * clear, clamps `part`'s step (`frame`) to the last one
 * (`frameCount - 1`) and resets `stepTimer` to the record's `duration`.
 * Either way, then resolves a final pointer: the record's `seq` is a
 * per-step `u16` array, indexed by the (possibly just-clamped) step;
 * that `u16` in turn indexes the bank's `frames`, and the result is
 * that array's pointer at the looked-up index.
 *
 * Matched in a later session than the original NAKED transcription -
 * see docs/matching.md's "Parked, not matched: GetSpriteFrame" for the
 * original account. Unlike `OffsetFromHitboxEdge`/`OffsetToHitboxEdge`/`OffsetToHitboxEdgeStart`'s
 * shared-switch-case gap, this function's resistant
 * `adds r0, r1, r0`-vs-`adds r0, r0, r1` add sits in genuinely
 * straight-line code (no switch, no case merging to protect), so a
 * plain inline-asm anchor on just that one instruction - the same
 * technique that had backfired inside those functions' shared case
 * blocks - works here with no caveats. */
void *GetSpriteFrame(struct gfx_part *part)
{
    MATCH_HOLD_REG(void *, rec, r1) = part->bank;
    MATCH_HOLD_REG(u8 *, idxAddr, r2) = &part->tag;
    MATCH_HOLD_REG(u8, idx, r4) = *idxAddr;
    s32 offset = idx * sizeof(struct sprite_anim);

    rec = (void *)((struct sprite_bank *)rec)->anims;
    rec = (u8 *)rec + offset; /* &bank->anims[part->tag] */

    if (part->animDone != 0) {
        MATCH_HOLD_REG(s32, mask, r0) = SPRITE_ANIM_LOOP;
        MATCH_HOLD_REG(s32, flags, r2) = ((struct sprite_anim *)rec)->flags;
        MATCH_HOLD_REG(s32, test, r0);

        test = mask & flags;
        if (!test) {
            part->frame = ((struct sprite_anim *)rec)->frameCount - 1;
            part->stepTimer = ((struct sprite_anim *)rec)->duration;
        }
    }

    {
        struct sprite_bank *bank = (struct sprite_bank *)part->bank;
        s32 frameIdx = part->frame;
        MATCH_HOLD_REG(void *, recPtr, r1) = (void *)((struct sprite_anim *)rec)->seq;
        MATCH_HOLD_REG(s32, byteOffset, r0) = frameIdx * 2;
        MATCH_HOLD_REG(u16 *, arr, r0);
        MATCH_HOLD_REG(void **, ptrArray, r1);
        u16 idx2;

        /* The ROM's `adds r0, r0, r1` (byteOffset-then-recPtr operand
         * order) versus this compiler's always-canonicalized
         * `adds r0, r1, r0` - see the doc comment above. */
        // clang-format off
        asm volatile(
            "add r0, r0, r1\n\t"
            : "=r"(arr)
            : "0"(byteOffset), "r"(recPtr)
        );
        // clang-format on

        ptrArray = (void **)bank->frames;
        idx2 = *arr;
        return ptrArray[idx2];
    }
}

/* A sprite's OBJ priority: layer 0's BG priority (`gLevelLayers->layer0`'s
 * BGnCNT shadow at +0x34, bits 0-1), minus one when
 * `gLevelLayers->raiseObjPriority` is set (SetupRoomBlend, blend mode 1),
 * which lifts the sprites one priority level above layer 0. */
s32 GetSpriteObjPriority(void)
{
    if (gLevelLayers->raiseObjPriority == 0) {
        struct bg_scroll_layer *layer = gLevelLayers->layer0;
        u8 cnt = *(u8 *)&layer->cnt; /* the low byte: `priority` in bits 0-1 */
        u32 result = ((u32)cnt << 30) >> 30;
        return result;
    } else {
        struct bg_scroll_layer *layer = gLevelLayers->layer0;
        u8 cnt = *(u8 *)&layer->cnt; /* the low byte: `priority` in bits 0-1 */
        u32 result = ((u32)cnt << 30) >> 30;
        return result - 1;
    }
}

/* Allocates a new `struct actor`-shaped object (`OperatorNew`),
 * initializes it via `InitEntity` (already matched in graphics.c -
 * wires up `gEntityVtable` and clears flags), then overwrites
 * its table with `gSpriteObjVtable` instead and clears its
 * part-object fields via `ResetSpriteObj` (already matched in
 * sprite.c). `arg0` becomes `field_08`, `arg1`/`arg2` become the
 * Q8 `x`/`y` position. */
void *CreateSpriteObj(u16 arg0, u16 arg1, u16 arg2, u16 unused)
{
    struct actor *part = OperatorNew(0x40);

    InitEntity(part);
    part->table = (void *)gSpriteObjVtable;
    ResetSpriteObj(part);
    part->id = arg0;
    part->x = INT_TO_Q8((s32)arg1);
    part->y = INT_TO_Q8((s32)arg2);
    return part;
}

/* Always-true stub. */
s32 GetSpriteObjClassId(void)
{
    return 1;
}

/* Same `gEntityVtable`/conditional-`OperatorDelete` shape as
 * `DestroyEntity` (already matched in `graphics.c`). */
void DestroySpriteObj(struct actor *self, u32 arg1)
{
    self->table = (void *)gEntityVtable;
    if (arg1 & 1) {
        OperatorDelete(self);
    }
}

/* Same `InitEntity`/table-swap/`ResetSpriteObj` shape as `CreateSpriteObj`
 * above, but re-initializes an existing `self` instead of allocating
 * a new one. */
struct actor *InitSpriteObj(struct actor *self)
{
    InitEntity(self);
    self->table = (void *)gSpriteObjVtable;
    ResetSpriteObj(self);
    return self;
}

/* Looks up `part`'s keyframe record via `GetSpriteFrame` (already parked
 * as `NON_MATCHING` in `sprite_obj.c`), then picks a pointer off it
 * per its layout type (the upper nibble of its first piece byte,
 * sprite_bank.h): 0 -> the 3-box frame's `anchor`, 6 -> the 1-box
 * frame's `anchor`, anything else (1-5, or above 6) -> the fixed
 * fallback `gEmptySpritePoint`. Needed the case labels scattered
 * out of numeric order (rather than grouped into the obvious
 * contiguous "0 / 1-5 / 6" ranges) to get gcc to emit a real jump
 * table instead of a compare chain - this compiler only builds a
 * jump table when the case-to-block mapping can't be expressed as a
 * few simple range checks, so a source-level shape that *looks*
 * needlessly scattered is what is needed to match the ROM's own
 * jump table here. */
void *GetSpriteFrameAnchor(void *part)
{
    const struct sprite_frame *info = GetSpriteFrame(part);
    u8 type = info->pieces[0] >> 4;
    void *result;

    switch (type) {
    case 0:
        result = (void *)&((const struct sprite_frame_3box_anchor *)info)->anchor;
        break;
    case 3:
    case 4:
        result = (void *)&gEmptySpritePoint;
        break;
    case 1:
    case 2:
        result = (void *)&gEmptySpritePoint;
        break;
    case 5:
        result = (void *)&gEmptySpritePoint;
        break;
    case 6:
        result = (void *)&((const struct sprite_frame_1box_anchor *)info)->anchor;
        break;
    default:
        result = (void *)&gEmptySpritePoint;
        break;
    }
    return result;
}

/* Same `GetSpriteFrame`-derived-record-nibble-switch shape as
 * `GetSpriteFrameAnchor` above, with a different result mapping: 0 and 4
 * select `box[2]`, anything else falls back to
 * `gEmptySpriteBox`. Unlike `GetSpriteFrameAnchor`, no case-scattering
 * trick was needed here - 0 and 4 are already non-adjacent, which is
 * enough on its own to make gcc emit a jump table instead of a
 * compare chain. */
void *GetSpriteFrameThirdBox(void *part)
{
    const struct sprite_frame *info = GetSpriteFrame(part);
    u8 type = info->pieces[0] >> 4;
    void *result;

    switch (type) {
    case 0:
        result = (void *)&((const struct sprite_frame_3box *)info)->box[2];
        break;
    case 1:
    case 2:
    case 3:
        result = (void *)&gEmptySpriteBox;
        break;
    case 4:
        result = (void *)&((const struct sprite_frame_3box *)info)->box[2];
        break;
    case 5:
    case 6:
        result = (void *)&gEmptySpriteBox;
        break;
    default:
        result = (void *)&gEmptySpriteBox;
        break;
    }
    return result;
}

/* Same `GetSpriteFrame`-derived-record-nibble `switch` shape again, with
 * the exact same case-to-block mapping as `GetSpriteAttackBox` (already
 * matched in `sprite.c`) - 0/3/4 select `box[1]`, 5 selects
 * `box[0]`, and 1/2/6/anything-above-6 fall back to
 * `gEmptySpriteBox`. That mapping is non-contiguous on its own,
 * so plain ascending case order was enough for a jump table here too,
 * no scattering needed. */
void *GetSpriteFrameAttackBox(void *part)
{
    const struct sprite_frame *info = GetSpriteFrame(part);
    u8 type = info->pieces[0] >> 4;
    void *result;

    switch (type) {
    case 0:
    case 3:
    case 4:
        result = (void *)&((const struct sprite_frame_3box *)info)->box[1];
        break;
    case 1:
    case 2:
    case 6:
        result = (void *)&gEmptySpriteBox;
        break;
    case 5:
        result = (void *)&((const struct sprite_frame_1box *)info)->box[0];
        break;
    default:
        result = (void *)&gEmptySpriteBox;
        break;
    }
    return result;
}

/* Same `GetSpriteFrame`-derived-record-nibble `switch` shape once more -
 * 0/2/3/4/6 select `box[0]`, 1/5/anything-above-6 fall back to
 * `gEmptySpriteBox`. */
void *GetSpriteFrameBodyBox(void *part)
{
    const struct sprite_frame *info = GetSpriteFrame(part);
    u8 type = info->pieces[0] >> 4;
    void *result;

    switch (type) {
    case 0:
        result = (void *)&((const struct sprite_frame_1box *)info)->box[0];
        break;
    case 1:
        result = (void *)&gEmptySpriteBox;
        break;
    case 2:
    case 3:
    case 4:
        result = (void *)&((const struct sprite_frame_1box *)info)->box[0];
        break;
    case 5:
        result = (void *)&gEmptySpriteBox;
        break;
    case 6:
        result = (void *)&((const struct sprite_frame_1box *)info)->box[0];
        break;
    default:
        result = (void *)&gEmptySpriteBox;
        break;
    }
    return result;
}

/* Same keyframe-record lookup used throughout this ROM region (see
 * `GetSpriteObjHitbox`) - `part`'s `keyframes` dereferenced twice,
 * indexed by its `frame` (the animation index) times KEYFRAME_SIZE. */
void *GetSpriteAnimRecord(struct actor *part)
{
    MATCH_HOLD_REG(struct keyframe **, tablePtr, r2) = ((struct box_part *)part)->keyframes;
    MATCH_HOLD_REG(u8, idx, r3) = ((struct box_part *)part)->frame;
    s32 offset = idx * KEYFRAME_SIZE;
    struct keyframe *table = *tablePtr;
    return (u8 *)table + offset;
}

/* Clamps `frame` to `part`'s current keyframe record's `steps` minus
 * one if it's out of range, then stores the result into `tick` (the
 * step also read/written by `GetSpriteFrame`). Needed explicit register pins on the whole
 * tablePtr/idxAddr/table/idx chain to get the ROM's `r5` (rather than
 * a tighter, naturally-reused register) - `idx` genuinely outlives
 * `table`'s own register here. The final `rec = table + offset` add
 * also hit the resistant "which operand goes first" canonicalization
 * documented at length for `OffsetFromHitboxEdge`/`OffsetToHitboxEdge`/
 * `OffsetToHitboxEdgeStart`/`GetSpriteFrame` - but unlike those (which were inside a
 * `switch` and had to be parked to avoid breaking case-block merging),
 * this function has no such constraint, so a one-instruction inline
 * `asm` anchor for just this add gets a fully byte-exact match. */
void SetSpriteFrameIndex(struct actor *part, s32 frame)
{
    MATCH_HOLD_REG(struct keyframe **, tablePtr, r0) = ((struct box_part *)part)->keyframes;
    MATCH_HOLD_REG(u8 *, idxAddr, r2) = &((struct box_part *)part)->frame;
    MATCH_HOLD_REG(struct keyframe *, table, r1) = *tablePtr;
    MATCH_HOLD_REG(u8, idx, r5) = *idxAddr;
    MATCH_HOLD_REG(s32, offset, r0) = idx * KEYFRAME_SIZE;
    struct keyframe *rec;

    asm("add %0, %0, %1" : "+r"(offset) : "r"(table));
    rec = (struct keyframe *)offset;

    {
        u8 steps = rec->steps;

        CLAMP_INDEX(frame, steps);
        ((struct box_part *)part)->tick = frame;
    }
}

/* `screenSpace` accessor pair - plain byte get/set, no other logic. */
u8 GetSpriteScreenSpace(void *part)
{
    return ((struct box_part *)part)->screenSpace;
}

void SetSpriteScreenSpace(void *part, u8 val)
{
    ((struct box_part *)part)->screenSpace = val;
}

/* `flags2` bit-2 getter. */
s32 IsSpriteHidden(void *part)
{
    return (((struct box_part *)part)->flags2 >> 2) & 1;
}

/* Toggles `flags2` bit 2. Needed the bit-flip (`(byte>>2)^1)&1`)
 * done via genuinely separate `eor`+`and` instructions instead of the
 * single `bic` this compiler normally folds that pattern into -
 * forced via a two-instruction inline `asm` block, whose "one" input
 * also needed marking `+r` (read-write) even though its value never
 * changes, purely to stop the compiler from constant-propagating its
 * value 1 past the asm block and computing the later mask (`-5`) as
 * `1 - 6` off of it instead of the ROM's fresh `movs r1, #5; negs r1,
 * r1`. Also needed the shifted-bit computed before (not after) the
 * mask, matching the ROM's own instruction order. */
void ToggleSpriteHidden(void *part)
{
    MATCH_HOLD_REG(u32, byte, r3) = ((struct box_part *)part)->flags2;
    MATCH_HOLD_REG(u32, shifted, r2) = byte >> 2;
    MATCH_HOLD_REG(u32, one, r1) = 1;
    MATCH_HOLD_REG(u32, bit, r2);
    MATCH_HOLD_REG(u32, shiftedBit, r2);
    MATCH_HOLD_REG(s32, mask, r1);
    MATCH_HOLD_REG(s32, result, r1);

    asm("eor %0, %0, %2\n\tand %0, %0, %2" : "=r"(bit), "+r"(one) : "1"(one), "0"(shifted));
    shiftedBit = bit << 2;

    mask = -5;
    result = mask & byte;
    result |= shiftedBit;
    ((struct box_part *)part)->flags2 = result;
}

/* `flags2` bit-3 (solid) getter - same shape as `IsSpriteHidden` above, one
 * bit over. */
s32 IsPartSolid(void *part)
{
    return (((struct box_part *)part)->flags2 >> 3) & 1;
}

/* Clears `flags2` bit 3. Needed the mask register-pinned to a
 * literal `-9` (computed via `movs r1, #9; negs r1, r1`, same
 * `-(N+1) == ~N` trick as `ToggleSpriteHidden`'s `-5` mask above) instead of
 * `~8`, which this compiler folds directly into a single `mov #0xf7`
 * immediate load. */
void ClearPartSolid(void *part)
{
    MATCH_HOLD_REG(s32, mask, r1) = -9;
    MATCH_HOLD_REG(s32, byte, r2) = ((struct box_part *)part)->flags2;
    MATCH_HOLD_REG(s32, result, r1);

    result = mask & byte;
    ((struct box_part *)part)->flags2 = result;
}

/* Sets `flags2` bit 3. Needed the mask register-pinned and computed
 * before the byte load (matching the ROM's own instruction order) -
 * the natural allocation loads the byte first. Same accumulator-
 * register pattern used for every AND/OR accessor below. */
void SetPartSolid(void *part)
{
    MATCH_HOLD_REG(s32, mask, r1) = 8;
    MATCH_HOLD_REG(s32, byte, r2) = ((struct box_part *)part)->flags2;
    MATCH_HOLD_REG(s32, result, r1);

    result = mask | byte;
    ((struct box_part *)part)->flags2 = result;
}

/* `part->flags` bit-6 getter. */
s32 IsSpriteObjVulnerable(struct actor *part)
{
    return (part->flags >> 6) & 1;
}

/* Clears `part->flags` bit 6. */
void ClearSpriteObjVulnerable(struct actor *part)
{
    MATCH_HOLD_REG(s32, mask, r1) = -0x41;
    MATCH_HOLD_REG(s32, byte, r2) = part->flags;
    MATCH_HOLD_REG(s32, result, r1);

    result = mask & byte;
    part->flags = result;
}

/* Sets `part->flags` bit 6. */
void SetSpriteObjVulnerable(struct actor *part)
{
    MATCH_HOLD_REG(s32, mask, r1) = 0x40;
    MATCH_HOLD_REG(s32, byte, r2) = part->flags;
    MATCH_HOLD_REG(s32, result, r1);

    result = mask | byte;
    part->flags = result;
}

/* Resets `part`'s animation index (`frame`) to 0. */
void ResetSpriteAnimIndex(void *part)
{
    ((struct box_part *)part)->frame = 0;
}

/* `part->flags` bit-7 getter - no mask needed since the shift already
 * leaves only that bit in position 0 of an 8-bit value. */
s32 IsSpriteObjCollisionEnabled(struct actor *part)
{
    return part->flags >> 7;
}

/* Clears `part->flags` bit 7. */
void DisableSpriteObjCollision(struct actor *part)
{
    MATCH_HOLD_REG(s32, mask, r1) = 0x7f;
    MATCH_HOLD_REG(s32, byte, r2) = part->flags;
    MATCH_HOLD_REG(s32, result, r1);

    result = mask & byte;
    part->flags = result;
}

/* Sets `part->flags` bit 7. */
void EnableSpriteObjCollision(struct actor *part)
{
    MATCH_HOLD_REG(s32, mask, r1) = 0x80;
    MATCH_HOLD_REG(s32, byte, r2) = part->flags;
    MATCH_HOLD_REG(s32, result, r1);

    result = mask | byte;
    part->flags = result;
}

/* `animating` get/set pair. */
u8 GetSpriteAnimating(void *part)
{
    return ((struct box_part *)part)->animating;
}

void SetSpriteAnimating(void *part, u8 val)
{
    ((struct box_part *)part)->animating = val;
}

/* Sets `mirrorX` (bit 4 of the byte at +0x28) to `value & 1`. Needed the low-bit extraction
 * done via a two-instruction inline `asm` AND (rather than this
 * compiler's own `& 1`, which produces the same result but as three
 * instructions once the u8 parameter's mandatory entry truncation is
 * folded in) - and, as with `ToggleSpriteHidden`, the "1" input needed
 * marking `+r` to stop the mask constant `-0x11` from being computed
 * relative to that leftover register value instead of freshly. */
void SetSpriteFlipX(void *part, u8 value)
{
    MATCH_HOLD_REG(s32, truncVal, r1) = value;
    MATCH_HOLD_REG(u8 *, addr, r0) = (u8 *)part + 0x28;
    MATCH_HOLD_REG(s32, one, r2) = 1;
    MATCH_HOLD_REG(s32, shiftedBit, r1);
    MATCH_HOLD_REG(s32, mask, r2);
    MATCH_HOLD_REG(s32, byte, r3);
    MATCH_HOLD_REG(s32, result, r2);

    asm("and %0, %0, %1" : "+r"(truncVal), "+r"(one));
    shiftedBit = truncVal << 4;
    mask = -0x11;
    byte = *addr;
    result = mask & byte;
    result |= shiftedBit;
    *addr = result;
}

/* Same shape as `SetSpriteFlipX` immediately above, sets `mirrorY` (bit
 * 5 of +0x28) instead. */
void SetSpriteFlipY(void *part, u8 value)
{
    MATCH_HOLD_REG(s32, truncVal, r1) = value;
    MATCH_HOLD_REG(u8 *, addr, r0) = (u8 *)part + 0x28;
    MATCH_HOLD_REG(s32, one, r2) = 1;
    MATCH_HOLD_REG(s32, shiftedBit, r1);
    MATCH_HOLD_REG(s32, mask, r2);
    MATCH_HOLD_REG(s32, byte, r3);
    MATCH_HOLD_REG(s32, result, r2);

    asm("and %0, %0, %1" : "+r"(truncVal), "+r"(one));
    shiftedBit = truncVal << 5;
    mask = -0x21;
    byte = *addr;
    result = mask & byte;
    result |= shiftedBit;
    *addr = result;
}

/* `animDone` ("done" flag, also read/written by `GetSpriteFrame`)
 * setter. */
void SetSpriteAnimDone(void *part, u8 val)
{
    ((struct box_part *)part)->animDone = val;
}

/* Same keyframe-record lookup used throughout this ROM region (see
 * `GetSpriteObjHitbox`/`GetSpriteAnimRecord`), returning the record's `paletteId`
 * instead of the record pointer itself. The final `rec = table +
 * offset` add hit the same resistant operand-order gap as
 * `SetSpriteFrameIndex` - fixed the same way, with a one-instruction inline
 * `asm` anchor. */
u8 GetSpriteAnimPaletteId(struct actor *part)
{
    MATCH_HOLD_REG(struct keyframe **, tablePtr, r1) = ((struct box_part *)part)->keyframes;
    MATCH_HOLD_REG(u8 *, idxAddr, r0) = &((struct box_part *)part)->frame;
    MATCH_HOLD_REG(struct keyframe *, table, r2) = *tablePtr;
    MATCH_HOLD_REG(u8, idx, r3) = *idxAddr;
    MATCH_HOLD_REG(s32, offset, r1) = idx * KEYFRAME_SIZE;
    struct keyframe *rec;

    asm("add %0, %0, %1" : "+r"(offset) : "r"(table));
    rec = (struct keyframe *)offset;
    return rec->paletteId;
}

/* `palette` (the low nibble of +0x29) getter. */
s32 GetSpritePalette(void *part)
{
    return ((struct box_part *)part)->palette;
}

/* Sets `palette` (the low nibble of the byte at +0x29) to `value & 0xf`. Needed the
 * parameter typed `s32` rather than `u8` - the `& 0xf` mask on a `u8`-
 * typed parameter compiles to a much longer defensive shift-based
 * sequence in this compiler (confirmed in isolation), which the ROM
 * doesn't have. The mask constant also needed the same `+r`-on-the-
 * other-operand fix as `ToggleSpriteHidden`/`SetSpriteFlipX` to stop it being
 * computed relative to the leftover "0xf" register value. */
void SetSpritePalette(void *part, s32 value)
{
    MATCH_HOLD_REG(u8 *, addr, r0) = (u8 *)part + 0x29;
    MATCH_HOLD_REG(s32, value_, r1) = value;
    MATCH_HOLD_REG(s32, fifteen, r2) = 0xf;
    MATCH_HOLD_REG(s32, lowNibble, r1);
    MATCH_HOLD_REG(s32, mask, r2);
    MATCH_HOLD_REG(s32, byte, r3);
    MATCH_HOLD_REG(s32, result, r2);

    asm("and %0, %0, %1" : "+r"(value_), "+r"(fifteen));
    lowNibble = value_;

    mask = -0x10;
    byte = *addr;
    result = mask & byte;
    result |= lowNibble;
    *addr = result;
}

/* `keyframes` (the sprite bank) get/set pair. */
void SetSpriteAnimTable(void *part, void *val)
{
    ((struct box_part *)part)->keyframes = val;
}

void *GetSpriteAnimTable(void *part)
{
    return ((struct box_part *)part)->keyframes;
}

/* Same keyframe-record lookup as `GetSpriteAnimPaletteId` above, testing the
 * record's `flags` bit 1 (looping) and returning it as a plain 0/1 value.
 * Matched after the NAKED transcription this function briefly used
 * (see git history and docs/matching.md's "Parked, not matched:
 * IsSpriteAnimLooping" entry for that account): every instruction here
 * matches the ROM up through the `ands r0, r1` on its own, but the
 * ROM's two trailing byte-truncation instructions (`lsls r0, r0,
 * #0x18; lsrs r0, r0, #0x18`, narrowing the AND result to the `u8`
 * return type) got optimized away by this compiler every time it
 * could prove the AND result (mask is the visible constant 2) already
 * fits in a byte. Closed with a `MATCH_KEEP_VOLATILE(test)`
 * barrier right after the `and`, making `test`'s value opaque to the
 * optimizer so it can no longer prove the automatic `s32`-to-`u8`
 * return-value truncation is redundant - the barrier itself emits no
 * instructions, it just forces the *existing* implicit truncation
 * back in. An explicit asm block emitting the shift pair directly was
 * tried first and also produced byte-exact output up through those
 * two instructions, but always duplicated them (the compiler still
 * inserted its own separate return-value truncation afterward,
 * regardless of whether the asm's output was typed `s32` or `u8`) -
 * the empty-barrier form avoids that by leaving the actual truncation
 * to the compiler's own return-conversion codegen. */
u8 IsSpriteAnimLooping(struct actor *part)
{
    MATCH_HOLD_REG(struct keyframe **, tablePtr, r1) = ((struct box_part *)part)->keyframes;
    MATCH_HOLD_REG(u8 *, idxAddr, r0) = &((struct box_part *)part)->frame;
    MATCH_HOLD_REG(struct keyframe *, table, r2) = *tablePtr;
    MATCH_HOLD_REG(u8, idx, r3) = *idxAddr;
    MATCH_HOLD_REG(s32, offset, r1) = idx * KEYFRAME_SIZE;
    struct keyframe *rec;
    MATCH_HOLD_REG(s32, mask, r0);
    MATCH_HOLD_REG(s32, flags, r1);
    MATCH_HOLD_REG(s32, test, r0);

    asm("add %0, %0, %1" : "+r"(offset) : "r"(table));
    rec = (struct keyframe *)offset;

    mask = 2;
    flags = rec->flags;
    test = mask & flags;
    MATCH_KEEP_VOLATILE(test);
    return test;
}
