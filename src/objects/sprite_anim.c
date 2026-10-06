#include "core.h"
#include "match.h"
#include "actor.h"
#include "actor_self.h"
#include "box_part.h"
#include <agb_syscall.h>
#include "util.h"
#include "system.h"
#include "audio.h"
#include "gfx.h"
#include "objects.h"
#include "level.h"
#include "globals.h"
#include "player.h"

/* Same keyframe-record lookup as `GetSpriteAnimPaletteId`/`IsSpriteAnimLooping` above,
 * returning the record's `+0x16` byte (frame count, also read by
 * `GetSpriteFrame`). */
u8 GetSpriteAnimFrameCount(struct actor *part)
{
    MATCH_HOLD_REG(void **, tablePtr, r1) = *(void ***)((u8 *)part + 0x20);
    MATCH_HOLD_REG(u8 *, idxAddr, r0) = (u8 *)part + 0x2d;
    MATCH_HOLD_REG(void *, table, r2) = *tablePtr;
    MATCH_HOLD_REG(u8, idx, r3) = *idxAddr;
    MATCH_HOLD_REG(s32, offset, r1) = idx * 0x1c;
    void *rec;

    asm("add %0, %0, %1" : "+r"(offset) : "r"(table));
    rec = (void *)offset;
    return *((u8 *)rec + 0x16);
}

/* Same shape as `GetSpriteAnimFrameCount` immediately above, returning the
 * record's `+0x15` byte (duration, also read by `GetSpriteFrame`)
 * instead. */
u8 GetSpriteAnimDuration(struct actor *part)
{
    MATCH_HOLD_REG(void **, tablePtr, r1) = *(void ***)((u8 *)part + 0x20);
    MATCH_HOLD_REG(u8 *, idxAddr, r0) = (u8 *)part + 0x2d;
    MATCH_HOLD_REG(void *, table, r2) = *tablePtr;
    MATCH_HOLD_REG(u8, idx, r3) = *idxAddr;
    MATCH_HOLD_REG(s32, offset, r1) = idx * 0x1c;
    void *rec;

    asm("add %0, %0, %1" : "+r"(offset) : "r"(table));
    rec = (void *)offset;
    return *((u8 *)rec + 0x15);
}

/* Resets `part`'s frame index (`+0x30`) to 0. */
void ResetSpriteFrameIndex(void *part)
{
    *(s32 *)((u8 *)part + 0x30) = 0;
}

/* `part+0x34` (sub-counter) get/set pair. */
void SetSpriteFrameTimer(void *part, s32 val)
{
    *(s32 *)((u8 *)part + 0x34) = val;
}

void ResetSpriteFrameTimer(void *part)
{
    *(s32 *)((u8 *)part + 0x34) = 0;
}

/* `part+0x2d` (frame index within the keyframe table) setter. */
void SetSpriteAnimIndex(void *part, u8 val)
{
    *((u8 *)part + 0x2d) = val;
}

/* Sets `part`'s frame index (`+0x2d`), then resets the sub-counter,
 * frame counter, and "done" flag (`ResetSpriteFrameTimer`/`ResetSpriteFrameIndex`/
 * `SetSpriteAnimDone`). */
void SetSpriteAnim(void *part, u8 idx)
{
    *((u8 *)part + 0x2d) = idx;
    ResetSpriteFrameTimer(part);
    ResetSpriteFrameIndex(part);
    SetSpriteAnimDone(part, 0);
}

/* Increments `part`'s frame index (`+0x30`). */
void IncSpriteFrameIndex(void *part)
{
    *(s32 *)((u8 *)part + 0x30) += 1;
}

/* Increments `part`'s sub-counter (`+0x34`). */
void IncSpriteFrameTimer(void *part)
{
    *(s32 *)((u8 *)part + 0x34) += 1;
}

/* `part+0x24` byte get/set pair. */
void SetSpriteMoveAxes(void *part, u8 val)
{
    *((u8 *)part + 0x24) = val;
}

u8 GetSpriteMoveAxes(void *part)
{
    return *((u8 *)part + 0x24);
}

/* `tick` (per-keyframe step counter) getter. */
s32 GetSpriteFrameIndex(struct box_part *part)
{
    return part->tick;
}

/* `timer` (ticks on the current step) getter. */
s32 GetSpriteFrameTimer(struct box_part *part)
{
    return part->timer;
}

/* `part+0x2d` (frame index within the keyframe table) getter. */
u8 GetSpriteAnim(void *part)
{
    return *((u8 *)part + 0x2d);
}

/* `part+0x28` low-2-bit getter - same `(u32 << 30) >> 30` idiom used
 * by `GetSpriteObjPriority`'s 2-bit field extraction. */
s32 GetSpriteGfxMode(void *part)
{
    u32 byte = *((u8 *)part + 0x28);
    return (byte << 0x1e) >> 0x1e;
}

/* Sets `part+0x28`'s low 2 bits to `value & 3`. Same accumulator-
 * register pattern (mask/byte/result chain) used throughout this ROM
 * region for AND/OR accessors, plus the same `+r`-on-the-other-
 * operand fix as `SetSpritePalette`/`SetSpriteFlipX` to stop the mask
 * constant `-4` being computed relative to the leftover `3` register
 * value instead of via a fresh `movs`+`negs`. */
void SetSpriteGfxMode(void *part, s32 value)
{
    MATCH_HOLD_REG(s32, val, r1) = value;
    MATCH_HOLD_REG(u8 *, addr, r0) = (u8 *)part + 0x28;
    MATCH_HOLD_REG(s32, three, r2) = 3;
    MATCH_HOLD_REG(s32, masked, r1);
    MATCH_HOLD_REG(s32, mask, r2);
    MATCH_HOLD_REG(s32, byte, r3);
    MATCH_HOLD_REG(s32, result, r2);

    asm("and %0, %0, %1" : "+r"(val), "+r"(three));
    masked = val;

    mask = -4;
    byte = *addr;
    result = mask & byte;
    result |= masked;
    *addr = result;
}

/* `part+0x28` bit-4 getter, via the `(u32 << N) >> 31` logical-shift
 * idiom (see `GetSpriteBounds`'s mirror flags) rather than a plain
 * `(byte >> 4) & 1`. */
s32 GetSpriteFlipX(void *part)
{
    u32 byte = *((u8 *)part + 0x28);
    return (byte << 0x1b) >> 0x1f;
}

/* `part+0x28` bit-5 getter, same idiom as `GetSpriteFlipX` above. */
s32 GetSpriteFlipY(void *part)
{
    u32 byte = *((u8 *)part + 0x28);
    return (byte << 0x1a) >> 0x1f;
}

/* `part+0x38` ("done" flag, also written by `SetSpriteAnimDone`) getter. */
u8 GetSpriteAnimDone(void *part)
{
    return *((u8 *)part + 0x38);
}

/* `part+0x28` bit-2 getter, same idiom as `GetSpriteFlipX`/`GetSpriteFlipY`
 * above. */
s32 GetSpriteMosaic(void *part)
{
    u32 byte = *((u8 *)part + 0x28);
    return (byte << 0x1d) >> 0x1f;
}

/* `part+0x29` low-nibble getter, same shape as `GetSpritePalette`. */
s32 GetSpriteOamPalette(void *part)
{
    u32 byte = *((u8 *)part + 0x29);
    return (byte << 0x1c) >> 0x1c;
}

/* `part+0x28` bit-3 getter, same idiom as the other single-bit getters
 * above. */
s32 GetSpriteColorMode(void *part)
{
    u32 byte = *((u8 *)part + 0x28);
    return (byte << 0x1c) >> 0x1f;
}

/* `affine` get/set pair. */
u16 GetSpriteAffine(struct box_part *part)
{
    return part->affine;
}

void SetSpriteAffine(struct box_part *part, u16 val)
{
    part->affine = val;
}

/* Resolves `part`'s Q8 position plus a caller-supplied offset into a
 * stack `{x, y}` pair, then dispatches to `DrawAffineSpritePieces` or
 * `DrawSpritePieces` (both already matched/parked elsewhere in this ROM
 * region) depending on whether `unk_3C` is set. */
void DrawSpriteWithOffset(struct actor *part, s32 arg1, s32 arg2)
{
    s32 pos[2];

    pos[0] = (part->x >> 8) + arg1;
    pos[1] = (part->y >> 8) + arg2;

    if (((struct box_part *)part)->affine != 0) {
        DrawAffineSpritePieces(gSpriteRenderer, (struct affine_part *)part, pos);
    } else {
        DrawSpritePieces(gSpriteRenderer, (struct oam_part *)part, pos);
    }
}

/* Sets `part+0x28`'s top 2 bits to `value << 6`. Needed the parameter
 * typed `s32` (not `u8`) for the same reason as `SetSpritePalette` - a
 * `u8`-typed parameter's mandatory entry truncation combines with the
 * later `<< 6` into a single, ROM-mismatching shift pair. */
void SetSpritePriority(void *part, s32 value)
{
    MATCH_HOLD_REG(u8 *, addr, r0) = (u8 *)part + 0x28;
    MATCH_HOLD_REG(s32, shiftedVal, r1) = value << 6;
    MATCH_HOLD_REG(s32, mask, r2) = 0x3f;
    MATCH_HOLD_REG(s32, byte, r3);
    MATCH_HOLD_REG(s32, result, r2);

    byte = *addr;
    result = mask & byte;
    result |= shiftedVal;
    *addr = result;
}

/* `part+0x28` top-2-bit getter - no mask needed since the shift
 * already isolates those bits. */
s32 GetSpritePriority(void *part)
{
    return *((u8 *)part + 0x28) >> 6;
}

/* Overwrites `part->table` with `gUiSpriteObjVtable`, then tail-
 * calls `DestroySpriteObj` (already matched in `sprite_obj.c`) with the
 * same `arg1` - which immediately overwrites `table` again with
 * `gEntityVtable` before its own conditional `OperatorDelete`
 * call. Reproduces the ROM's apparently-redundant double table write
 * as-is. */
void DestroyUiSpriteObj(struct actor *part, u32 arg1)
{
    part->table = (void *)gUiSpriteObjVtable;
    DestroySpriteObj(part, arg1);
}

/* Re-initializes `part` via `InitSpriteObj` (already matched in
 * `sprite_obj.c`, which itself sets `table` to `gSpriteObjVtable`),
 * then immediately overwrites `table` with `gUiSpriteObjVtable`
 * instead. */
struct actor *InitUiSpriteObj(struct actor *part)
{
    InitSpriteObj(part);
    part->table = (void *)gUiSpriteObjVtable;
    return part;
}

typedef s32 (*part_method0_fn)(void *self);
typedef s32 (*part_method1_fn)(void *self, void *arg);
typedef void (*part_method3_fn)(void *self, s32 a, s32 b, s32 c);

extern s32 _call_via_r2(void *self, void *arg, void *fn);
extern s32 _call_via_r1(void *self, void *fn);

/* `list` is the part list: each call compacts `items` (dropping parts
 * whose `gone` bit is set) and rebuilds `visible` from scratch. Builds
 * two camera-relative boxes first: a "near" 440x280 region 100/60 px
 * past the camera position and the plain 240x160 screen region at it
 * (Q8).
 *
 * For each part: if it is gone, and its index is still below
 * `capacity`, removes it from `items` via a `CpuSet` (the BIOS
 * `CpuSet` SWI) block copy shifting every later element down by one,
 * decrementing `count` and clearing the vacated last slot - then (if
 * the slot held a part) calls its method-table +0x50 method with `3`,
 * and re-examines the same index next iteration. Otherwise: if the
 * +0x40 method says it is within the near box, calls its +0x18 method
 * (per-frame update) and, if the +0x30 method says it is on screen,
 * appends it to `visible`.
 *
 * Real C under old_agbcc (issue #9-#11 NAKED retry): the two boxes are
 * members of one frame object (so the screen box's x/y go straight to
 * sp offsets), and the w/h stores go through a `&screen` pointer taken
 * after them - that pointer is the one the loop keeps in r8. */
void UpdatePartList(struct part_list *list)
{
    struct {
        struct aabb near;
        struct aabb screen;
    } f;
    struct bg_scroll_layer *cam;
    s32 i;
    s32 zero;
    struct aabb *ps;

    {
        s32 w = 440 << 8;
        s32 h = 280 << 8;
        f.near.w = w;
        f.near.h = h;
    }
    cam = gLevelLayers->layer0;
    {
        s32 x;
        s32 y;

        x = cam->x << 8;
        zero = 0;
        x -= 100 << 8;
        y = (cam->y << 8) - (60 << 8);
        f.near.x = x;
        f.near.y = y;
    }
    {
        s32 x = cam->x << 8;
        s32 y = cam->y << 8;
        f.screen.x = x;
        f.screen.y = y;
    }
    ps = &f.screen;
    {
        s32 w = 240 << 8;
        s32 h = 160 << 8;
        ps->w = w;
        ps->h = h;
    }
    list->visibleCount = zero;

    for (i = 0; i < list->count; i++) {
        struct box_part **items = list->items;
        struct box_part *part = items[i];

        if (part->flags & 1) {
            if (i < list->capacity) {
                CpuSet(&items[i + 1], &items[i], ((list->count - i) & 0x1FFFFF) | 0x4000000);
                list->count--;
                list->items[list->count] = NULL;
            }
            if (part != NULL) {
                struct part_method *m = PART_METHOD(part, 0x50);
                _call_via_r2((u8 *)part + m->thisOffset, (void *)3, m->fn);
            }
            i--;
        } else {
            struct part_method *m = PART_METHOD(part, 0x40);

            if ((u8)_call_via_r2((u8 *)part + m->thisOffset, &f.near, m->fn)) {
                struct part_method *m2 = PART_METHOD(part, 0x18);
                struct part_method *m3;

                _call_via_r1((u8 *)part + m2->thisOffset, m2->fn);
                m3 = PART_METHOD(part, 0x30);
                if ((u8)_call_via_r2((u8 *)part + m3->thisOffset, &f.screen, m3->fn))
                    list->visible[list->visibleCount++] = part;
            }
        }
    }
}

/* Walks `list`'s visible parts. For each: asks its method-table +0x48
 * method for a state and skips it unless that is above 4, and skips it
 * unless its `visible` bit (flags bit 2) is set. Then hands the incoming
 * box to `CollidePartWithPlayer` (when `other` is the player, gPlayer)
 * or `CollidePartWithObject` (otherwise, also passing `other`).
 *
 * The box arrives and is passed on by value; the ROM copies it into one
 * shared temporary with MemCopy32 (memcpy) before each call, which is
 * what the explicit call reproduces (a plain struct assignment is
 * copied inline with ldm/stm instead). `unused` is the caller's padding
 * argument. Matches under old_agbcc. */
void CollidePartList(struct part_list *list, struct aabb box, s32 unused, struct box_part *other)
{
    s32 i;
    struct aabb tmp;

    for (i = 0; i < list->visibleCount; i++) {
        struct box_part *part = list->visible[i];
        struct part_method *m = PART_METHOD(part, 0x48);

        if (((part_method0_fn)m->fn)((u8 *)part + m->thisOffset) <= 4)
            continue;
        if (!((part->flags >> 2) & 1))
            continue;
        if (other == (struct box_part *)gPlayer) {
            MemCopy32(&tmp, &box, sizeof(tmp));
            CollidePartWithPlayer(list, tmp, part);
        } else {
            MemCopy32(&tmp, &box, sizeof(tmp));
            CollidePartWithObject(list, tmp, part, other);
        }
    }
}

/* obj->vtable[0x68](a, b, c) - the part's "hit" method. */
#define CALL_HIT(obj, a, b, c)                                                 \
    if (1) {                                                                   \
        struct part_method *_m = PART_METHOD(obj, 0x68);                       \
        ((part_method3_fn)_m->fn)((u8 *)(obj) + _m->thisOffset, (a), (b), (c)); \
    } else (void)0

/* Resolves a hit between `part` and the player (gPlayer)
 * against the incoming box passed by CollidePartList (`list` is unused).
 *
 * In mode 3 (gLevelState->maskLevel): if `part` touches the box
 * (`ClassifySpriteContact`), calls its hit method with the player's kind.
 * Otherwise, for a solid part (flags2 bit 3): builds the player's box
 * (GetSpriteHitbox) and the part's (GetSpriteBodyBox); on overlap pushes the
 * player's x out of `part` by the sum of both widths (<<7) on whichever
 * side it is, and calls the player's hit method (0, 0xc, side 2/1).
 * Otherwise, by `ClassifySpriteContact`'s result: 1 marks the player hit; a
 * player of kind 1 with a positive +0x64 counter calls both hit methods
 * and plays SFX 0x21, any other kind calls the part's hit method with
 * that kind. 2 marks the part hit, calls its hit method first when the
 * mode is nonzero, then the player's with the part's kind.
 *
 * Matches under old_agbcc. The box arrives by value (the old "leave one
 * scalar in its incoming stack slot" blocker); the empty `case 0` gives
 * the ROM's `cmp #1; beq; cmp #1; ble; cmp #2` switch; the player's
 * hit-flag update goes through a pointer to keep the ROM's registers. */
void CollidePartWithPlayer(struct part_list *list, struct aabb box, struct box_part *part)
{
    if (gLevelState->maskLevel == 3) {
        if (!ClassifySpriteContact(part, &box))
            return;
        CALL_HIT(part, 1, gPlayer->kind, 0);
    } else if ((part->flags2 >> 3) & 1) {
        struct aabb a, b;
        s32 px;

        a = GetSpriteHitbox((struct box_part *)gPlayer);
        GetSpriteBodyBox(&b, part);
        if (!AabbOverlaps(&a, &b))
            return;
        px = part->x;
        if (px < gPlayer->x) {
            gPlayer->x = px + ((b.w + a.w) << 7);
            CALL_HIT((struct box_part *)gPlayer, 0, 0xc, 2);
        } else {
            gPlayer->x = px - ((b.w + a.w) << 7);
            CALL_HIT((struct box_part *)gPlayer, 0, 0xc, 1);
        }
    } else {
        u8 kind;

        switch (ClassifySpriteContact(part, &box)) {
        case 0:
            break;
        case 1:
            {
                u8 *flags = &gPlayer->flags.all;
                *flags |= 8;
            }
            kind = gPlayer->kind;
            if (kind == 1) {
                if (gPlayer->speedY > 0) {
                    CALL_HIT(part, 1, 1, 0);
                    CALL_HIT((struct box_part *)gPlayer, 0, 0xd, 0);
                    PlaySfx(gAudioContext, 0x21, 0x100);
                }
            } else {
                CALL_HIT(part, 1, kind, 0);
            }
            break;
        case 2:
            part->flags |= 8;
            if (gLevelState->maskLevel) {
                CALL_HIT(part, 1, 1, 0);
            }
            CALL_HIT((struct box_part *)gPlayer, 1, part->kind, 0);
            break;
        }
    }
}

/* CollidePartWithObject, this function's ROM-adjacent sibling (its collision-hit
 * logic mirror for a non-default "compare viewport"), lives in
 * src/objects/part_collide.c instead of here - its real ROM address
 * isn't adjacent to this file's functions (part_list_cull.c's
 * CullPartList/ClearPartList/CollidePartsOfClass sit between CollidePartWithPlayer above and
 * CollidePartWithObject in ROM order), so it needs its own translation unit per
 * docs/workflow.md step 4. */
