#include "core.h"
#include "actor.h"
#include "actor_self.h"
#include "box_part.h"

extern void *GetSpriteFrame(void *part);

/* Same keyframe-record lookup as `GetSpriteAnimPaletteId`/`IsSpriteAnimLooping` above,
 * returning the record's `+0x16` byte (frame count, also read by
 * `GetSpriteFrame`). */
u8 GetSpriteAnimFrameCount(struct actor *part)
{
    register void **tablePtr asm("r1") = *(void ***)((u8 *)part + 0x20);
    register u8 *idxAddr asm("r0") = (u8 *)part + 0x2d;
    register void *table asm("r2") = *tablePtr;
    register u8 idx asm("r3") = *idxAddr;
    register s32 offset asm("r1") = idx * 0x1c;
    void *rec;

    asm("add %0, %0, %1" : "+r" (offset) : "r" (table));
    rec = (void *)offset;
    return *((u8 *)rec + 0x16);
}

/* Same shape as `GetSpriteAnimFrameCount` immediately above, returning the
 * record's `+0x15` byte (duration, also read by `GetSpriteFrame`)
 * instead. */
u8 GetSpriteAnimDuration(struct actor *part)
{
    register void **tablePtr asm("r1") = *(void ***)((u8 *)part + 0x20);
    register u8 *idxAddr asm("r0") = (u8 *)part + 0x2d;
    register void *table asm("r2") = *tablePtr;
    register u8 idx asm("r3") = *idxAddr;
    register s32 offset asm("r1") = idx * 0x1c;
    void *rec;

    asm("add %0, %0, %1" : "+r" (offset) : "r" (table));
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

extern void SetSpriteAnimDone(void *part, u8 val);

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
void sub_8008804(void *part, u8 val)
{
    *((u8 *)part + 0x24) = val;
}

u8 sub_800880C(void *part)
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
s32 sub_8008824(void *part)
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
void sub_8008830(void *part, s32 value)
{
    register s32 val asm("r1") = value;
    register u8 *addr asm("r0") = (u8 *)part + 0x28;
    register s32 three asm("r2") = 3;
    register s32 masked asm("r1");
    register s32 mask asm("r2");
    register s32 byte asm("r3");
    register s32 result asm("r2");

    asm("and %0, %0, %1" : "+r" (val), "+r" (three));
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
s32 sub_8008864(void *part)
{
    u32 byte = *((u8 *)part + 0x28);
    return (byte << 0x1d) >> 0x1f;
}

/* `part+0x29` low-nibble getter, same shape as `GetSpritePalette`. */
s32 sub_8008870(void *part)
{
    u32 byte = *((u8 *)part + 0x29);
    return (byte << 0x1c) >> 0x1c;
}

/* `part+0x28` bit-3 getter, same idiom as the other single-bit getters
 * above. */
s32 sub_800887C(void *part)
{
    u32 byte = *((u8 *)part + 0x28);
    return (byte << 0x1c) >> 0x1f;
}

/* `unk_3C` get/set pair. */
u16 sub_8008888(struct box_part *part)
{
    return part->unk_3C;
}

void sub_800888C(struct box_part *part, u16 val)
{
    part->unk_3C = val;
}

extern void *gSpriteRenderer;
extern void DrawAffineSpritePieces(void *unused, void *part, s32 *posPtr);
extern void DrawSpritePieces(void *unused, void *part, s32 *posPtr);

/* Resolves `part`'s Q8 position plus a caller-supplied offset into a
 * stack `{x, y}` pair, then dispatches to `DrawAffineSpritePieces` or
 * `DrawSpritePieces` (both already matched/parked elsewhere in this ROM
 * region) depending on whether `unk_3C` is set. */
void DrawSpriteWithOffset(struct actor *part, s32 arg1, s32 arg2)
{
    s32 pos[2];

    pos[0] = (part->x >> 8) + arg1;
    pos[1] = (part->y >> 8) + arg2;

    if (((struct box_part *)part)->unk_3C != 0) {
        DrawAffineSpritePieces(gSpriteRenderer, part, pos);
    } else {
        DrawSpritePieces(gSpriteRenderer, part, pos);
    }
}

/* Sets `part+0x28`'s top 2 bits to `value << 6`. Needed the parameter
 * typed `s32` (not `u8`) for the same reason as `SetSpritePalette` - a
 * `u8`-typed parameter's mandatory entry truncation combines with the
 * later `<< 6` into a single, ROM-mismatching shift pair. */
void SetSpritePriority(void *part, s32 value)
{
    register u8 *addr asm("r0") = (u8 *)part + 0x28;
    register s32 shiftedVal asm("r1") = value << 6;
    register s32 mask asm("r2") = 0x3f;
    register s32 byte asm("r3");
    register s32 result asm("r2");

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

extern u8 gUiSpriteObjVtable[];
extern void DestroySpriteObj(struct actor *self, u32 arg1);

/* Overwrites `part->table` with `gUiSpriteObjVtable`, then tail-
 * calls `DestroySpriteObj` (already matched in `actor_part6.c`) with the
 * same `arg1` - which immediately overwrites `table` again with
 * `gEntityVtable` before its own conditional `OperatorDelete`
 * call. Reproduces the ROM's apparently-redundant double table write
 * as-is. */
void DestroyUiSpriteObj(struct actor *part, u32 arg1)
{
    part->table = gUiSpriteObjVtable;
    DestroySpriteObj(part, arg1);
}

extern struct actor *InitSpriteObj(struct actor *self);

/* Re-initializes `part` via `InitSpriteObj` (already matched in
 * `actor_part6.c`, which itself sets `table` to `gSpriteObjVtable`),
 * then immediately overwrites `table` with `gUiSpriteObjVtable`
 * instead. */
struct actor *InitUiSpriteObj(struct actor *part)
{
    InitSpriteObj(part);
    part->table = gUiSpriteObjVtable;
    return part;
}

typedef s32 (*part_method0_fn)(void *self);
typedef s32 (*part_method1_fn)(void *self, void *arg);
typedef void (*part_method3_fn)(void *self, s32 a, s32 b, s32 c);

/* gLevelLayers's view here (level_layers.c's `struct level_layers`):
 * BG layer 0's scroll position (include/bg_scroll_layer.h), which is the
 * camera position in pixels. */
struct bg_scroll_layer {
    s32 x;
    s32 y;
};

struct level_layers {
    u8 unk_00[0x10];
    struct bg_scroll_layer *layer0; // 0x10
};

extern struct level_layers *gLevelLayers;
extern s32 _call_via_r2(void *self, void *arg, void *fn);
extern s32 _call_via_r1(void *self, void *fn);
extern void CpuSet(const void *src, void *dst, u32 cnt);

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
    struct { struct part_aabb near; struct part_aabb screen; } f;
    struct bg_scroll_layer *cam;
    s32 i;
    s32 zero;
    struct part_aabb *ps;

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

extern void *MemCopy32(void *dst, const void *src, s32 size); /* memcpy (asm/crt0.s) */
extern void CollidePartWithPlayer(struct part_list *list, struct part_aabb box, struct box_part *part);
extern void CollidePartWithObject(struct part_list *list, struct part_aabb box, struct box_part *part, struct box_part *other);
extern struct box_part *gPlayer;

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
void CollidePartList(struct part_list *list, struct part_aabb box, s32 unused, struct box_part *other)
{
    s32 i;
    struct part_aabb tmp;

    for (i = 0; i < list->visibleCount; i++) {
        struct box_part *part = list->visible[i];
        struct part_method *m = PART_METHOD(part, 0x48);

        if (((part_method0_fn)m->fn)((u8 *)part + m->thisOffset) <= 4)
            continue;
        if (!((part->flags >> 2) & 1))
            continue;
        if (other == gPlayer) {
            MemCopy32(&tmp, &box, sizeof(tmp));
            CollidePartWithPlayer(list, tmp, part);
        } else {
            MemCopy32(&tmp, &box, sizeof(tmp));
            CollidePartWithObject(list, tmp, part, other);
        }
    }
}

struct game_state {
    u8 unk_00[0x78];
    s32 maskLevel;      // 0x78 - the Aku Aku mask level (0-3)
};

extern struct game_state *gLevelState;
extern void *gAudioContext;
extern s32 sub_8009FF4(struct box_part *part, struct part_aabb *box);
extern struct part_aabb GetSpriteHitbox(struct box_part *part);
extern struct part_aabb sub_8007CF8(struct box_part *part);
extern u8 AabbOverlaps(struct part_aabb *a, struct part_aabb *b);
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);

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
 * (`sub_8009FF4`), calls its hit method with the player's kind.
 * Otherwise, for a solid part (flags2 bit 3): builds the player's box
 * (GetSpriteHitbox) and the part's (sub_8007CF8); on overlap pushes the
 * player's x out of `part` by the sum of both widths (<<7) on whichever
 * side it is, and calls the player's hit method (0, 0xc, side 2/1).
 * Otherwise, by `sub_8009FF4`'s result: 1 marks the player hit; a
 * player of kind 1 with a positive +0x64 counter calls both hit methods
 * and plays SFX 0x21, any other kind calls the part's hit method with
 * that kind. 2 marks the part hit, calls its hit method first when the
 * mode is nonzero, then the player's with the part's kind.
 *
 * Matches under old_agbcc. The box arrives by value (the old "leave one
 * scalar in its incoming stack slot" blocker); the empty `case 0` gives
 * the ROM's `cmp #1; beq; cmp #1; ble; cmp #2` switch; the player's
 * hit-flag update goes through a pointer to keep the ROM's registers. */
void CollidePartWithPlayer(struct part_list *list, struct part_aabb box, struct box_part *part)
{
    if (gLevelState->maskLevel == 3) {
        if (!sub_8009FF4(part, &box))
            return;
        CALL_HIT(part, 1, gPlayer->kind, 0);
    } else if ((part->flags2 >> 3) & 1) {
        struct part_aabb a, b;
        s32 px;

        a = GetSpriteHitbox(gPlayer);
        b = sub_8007CF8(part);
        if (!AabbOverlaps(&a, &b))
            return;
        px = part->x;
        if (px < gPlayer->x) {
            gPlayer->x = px + ((b.w + a.w) << 7);
            CALL_HIT(gPlayer, 0, 0xc, 2);
        } else {
            gPlayer->x = px - ((b.w + a.w) << 7);
            CALL_HIT(gPlayer, 0, 0xc, 1);
        }
    } else {
        u8 kind;

        switch (sub_8009FF4(part, &box)) {
        case 0:
            break;
        case 1:
        {
            u8 *flags = &gPlayer->flags;
            *flags |= 8;
        }
            kind = gPlayer->kind;
            if (kind == 1) {
                if (gPlayer->unk_64 > 0) {
                    CALL_HIT(part, 1, 1, 0);
                    CALL_HIT(gPlayer, 0, 0xd, 0);
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
            CALL_HIT(gPlayer, 1, part->kind, 0);
            break;
        }
    }
}

/* CollidePartWithObject, this function's ROM-adjacent sibling (its collision-hit
 * logic mirror for a non-default "compare viewport"), lives in
 * src/graphics/actor_part7b.c instead of here - its real ROM address
 * isn't adjacent to this file's functions (actor_part10.c's
 * CullPartList/ClearPartList/sub_8008D30 sit between CollidePartWithPlayer above and
 * CollidePartWithObject in ROM order), so it needs its own translation unit per
 * docs/workflow.md step 4. */
