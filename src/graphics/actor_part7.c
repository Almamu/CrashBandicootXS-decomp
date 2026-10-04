#include "core.h"
#include "actor.h"
#include "actor_self.h"
#include "box_part.h"

extern void *sub_80083B8(void *part);

/* Same keyframe-record lookup as `sub_8008734`/`sub_8008770` above,
 * returning the record's `+0x16` byte (frame count, also read by
 * `sub_80083B8`). */
u8 sub_800878C(struct actor *part)
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

/* Same shape as `sub_800878C` immediately above, returning the
 * record's `+0x15` byte (duration, also read by `sub_80083B8`)
 * instead. */
u8 sub_80087A0(struct actor *part)
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
void sub_80087B4(void *part)
{
    *(s32 *)((u8 *)part + 0x30) = 0;
}

/* `part+0x34` (sub-counter) get/set pair. */
void sub_80087BC(void *part, s32 val)
{
    *(s32 *)((u8 *)part + 0x34) = val;
}

void sub_80087C0(void *part)
{
    *(s32 *)((u8 *)part + 0x34) = 0;
}

/* `part+0x2d` (frame index within the keyframe table) setter. */
void sub_80087C8(void *part, u8 val)
{
    *((u8 *)part + 0x2d) = val;
}

extern void sub_800872C(void *part, u8 val);

/* Sets `part`'s frame index (`+0x2d`), then resets the sub-counter,
 * frame counter, and "done" flag (`sub_80087C0`/`sub_80087B4`/
 * `sub_800872C`). */
void sub_80087D0(void *part, u8 idx)
{
    *((u8 *)part + 0x2d) = idx;
    sub_80087C0(part);
    sub_80087B4(part);
    sub_800872C(part, 0);
}

/* Increments `part`'s frame index (`+0x30`). */
void sub_80087F4(void *part)
{
    *(s32 *)((u8 *)part + 0x30) += 1;
}

/* Increments `part`'s sub-counter (`+0x34`). */
void sub_80087FC(void *part)
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
s32 sub_8008814(struct box_part *part)
{
    return part->tick;
}

/* `timer` (ticks on the current step) getter. */
s32 sub_8008818(struct box_part *part)
{
    return part->timer;
}

/* `part+0x2d` (frame index within the keyframe table) getter. */
u8 sub_800881C(void *part)
{
    return *((u8 *)part + 0x2d);
}

/* `part+0x28` low-2-bit getter - same `(u32 << 30) >> 30` idiom used
 * by `sub_8008408`'s 2-bit field extraction. */
s32 sub_8008824(void *part)
{
    u32 byte = *((u8 *)part + 0x28);
    return (byte << 0x1e) >> 0x1e;
}

/* Sets `part+0x28`'s low 2 bits to `value & 3`. Same accumulator-
 * register pattern (mask/byte/result chain) used throughout this ROM
 * region for AND/OR accessors, plus the same `+r`-on-the-other-
 * operand fix as `sub_8008754`/`sub_80086F4` to stop the mask
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
 * idiom (see `sub_8007B00`'s mirror flags) rather than a plain
 * `(byte >> 4) & 1`. */
s32 sub_8008844(void *part)
{
    u32 byte = *((u8 *)part + 0x28);
    return (byte << 0x1b) >> 0x1f;
}

/* `part+0x28` bit-5 getter, same idiom as `sub_8008844` above. */
s32 sub_8008850(void *part)
{
    u32 byte = *((u8 *)part + 0x28);
    return (byte << 0x1a) >> 0x1f;
}

/* `part+0x38` ("done" flag, also written by `sub_800872C`) getter. */
u8 sub_800885C(void *part)
{
    return *((u8 *)part + 0x38);
}

/* `part+0x28` bit-2 getter, same idiom as `sub_8008844`/`sub_8008850`
 * above. */
s32 sub_8008864(void *part)
{
    u32 byte = *((u8 *)part + 0x28);
    return (byte << 0x1d) >> 0x1f;
}

/* `part+0x29` low-nibble getter, same shape as `sub_8008748`. */
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

extern void *gUnknown_030012CC;
extern void sub_8007634(void *unused, void *part, s32 *posPtr);
extern void sub_80073DC(void *unused, void *part, s32 *posPtr);

/* Resolves `part`'s Q8 position plus a caller-supplied offset into a
 * stack `{x, y}` pair, then dispatches to `sub_8007634` or
 * `sub_80073DC` (both already matched/parked elsewhere in this ROM
 * region) depending on whether `unk_3C` is set. */
void sub_8008890(struct actor *part, s32 arg1, s32 arg2)
{
    s32 pos[2];

    pos[0] = (part->x >> 8) + arg1;
    pos[1] = (part->y >> 8) + arg2;

    if (((struct box_part *)part)->unk_3C != 0) {
        sub_8007634(gUnknown_030012CC, part, pos);
    } else {
        sub_80073DC(gUnknown_030012CC, part, pos);
    }
}

/* Sets `part+0x28`'s top 2 bits to `value << 6`. Needed the parameter
 * typed `s32` (not `u8`) for the same reason as `sub_8008754` - a
 * `u8`-typed parameter's mandatory entry truncation combines with the
 * later `<< 6` into a single, ROM-mismatching shift pair. */
void sub_80088D8(void *part, s32 value)
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
s32 sub_80088E8(void *part)
{
    return *((u8 *)part + 0x28) >> 6;
}

extern u8 gStaticData_087E3CAC[];
extern void sub_8008484(struct actor *self, u32 arg1);

/* Overwrites `part->table` with `gStaticData_087E3CAC`, then tail-
 * calls `sub_8008484` (already matched in `actor_part6.c`) with the
 * same `arg1` - which immediately overwrites `table` again with
 * `gStaticData_087E3BEC` before its own conditional `sub_8026ED0`
 * call. Reproduces the ROM's apparently-redundant double table write
 * as-is. */
void sub_80088F0(struct actor *part, u32 arg1)
{
    part->table = gStaticData_087E3CAC;
    sub_8008484(part, arg1);
}

extern struct actor *sub_80084A4(struct actor *self);

/* Re-initializes `part` via `sub_80084A4` (already matched in
 * `actor_part6.c`, which itself sets `table` to `gStaticData_087E3C44`),
 * then immediately overwrites `table` with `gStaticData_087E3CAC`
 * instead. */
struct actor *sub_8008904(struct actor *part)
{
    sub_80084A4(part);
    part->table = gStaticData_087E3CAC;
    return part;
}

ACTOR_CALL_VIA_ALIASES

typedef s32 (*part_method0_fn)(void *self);
typedef s32 (*part_method1_fn)(void *self, void *arg);
typedef void (*part_method3_fn)(void *self, s32 a, s32 b, s32 c);

struct camera_pos {
    s32 x;
    s32 y;
};

struct viewport {
    u8 unk_00[0x10];
    struct camera_pos *camera; // 0x10
};

extern struct viewport *gLevelLayers;
extern s32 sub_803AD80(void *self, void *arg, void *fn);
extern s32 sub_803AD7C(void *self, void *fn);
extern void sub_803A94C(const void *src, void *dst, u32 cnt);

/* `list` is the part list: each call compacts `items` (dropping parts
 * whose `gone` bit is set) and rebuilds `visible` from scratch. Builds
 * two camera-relative boxes first: a "near" 440x280 region 100/60 px
 * past the camera position and the plain 240x160 screen region at it
 * (Q8).
 *
 * For each part: if it is gone, and its index is still below
 * `capacity`, removes it from `items` via a `sub_803A94C` (the BIOS
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
void sub_800891C(struct part_list *list)
{
    struct { struct part_aabb near; struct part_aabb screen; } f;
    struct camera_pos *cam;
    s32 i;
    s32 zero;
    struct part_aabb *ps;

    {
        s32 w = 440 << 8;
        s32 h = 280 << 8;
        f.near.w = w;
        f.near.h = h;
    }
    cam = gLevelLayers->camera;
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
                sub_803A94C(&items[i + 1], &items[i], ((list->count - i) & 0x1FFFFF) | 0x4000000);
                list->count--;
                list->items[list->count] = NULL;
            }
            if (part != NULL) {
                struct part_method *m = PART_METHOD(part, 0x50);
                sub_803AD80((u8 *)part + m->thisOffset, (void *)3, m->fn);
            }
            i--;
        } else {
            struct part_method *m = PART_METHOD(part, 0x40);

            if ((u8)sub_803AD80((u8 *)part + m->thisOffset, &f.near, m->fn)) {
                struct part_method *m2 = PART_METHOD(part, 0x18);
                struct part_method *m3;

                sub_803AD7C((u8 *)part + m2->thisOffset, m2->fn);
                m3 = PART_METHOD(part, 0x30);
                if ((u8)sub_803AD80((u8 *)part + m3->thisOffset, &f.screen, m3->fn))
                    list->visible[list->visibleCount++] = part;
            }
        }
    }
}

extern void *sub_800014C(void *dst, const void *src, s32 size); /* memcpy (asm/crt0.s) */
extern void sub_8008AD8(struct part_list *list, struct part_aabb box, struct box_part *part);
extern void sub_8008D80(struct part_list *list, struct part_aabb box, struct box_part *part, struct box_part *other);
extern struct box_part *gUnknown_030012D8;

/* Walks `list`'s visible parts. For each: asks its method-table +0x48
 * method for a state and skips it unless that is above 4, and skips it
 * unless its `visible` bit (flags bit 2) is set. Then hands the incoming
 * box to `sub_8008AD8` (when `other` is the player, gUnknown_030012D8)
 * or `sub_8008D80` (otherwise, also passing `other`).
 *
 * The box arrives and is passed on by value; the ROM copies it into one
 * shared temporary with sub_800014C (memcpy) before each call, which is
 * what the explicit call reproduces (a plain struct assignment is
 * copied inline with ldm/stm instead). `unused` is the caller's padding
 * argument. Matches under old_agbcc. */
void sub_8008A40(struct part_list *list, struct part_aabb box, s32 unused, struct box_part *other)
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
        if (other == gUnknown_030012D8) {
            sub_800014C(&tmp, &box, sizeof(tmp));
            sub_8008AD8(list, tmp, part);
        } else {
            sub_800014C(&tmp, &box, sizeof(tmp));
            sub_8008D80(list, tmp, part, other);
        }
    }
}

struct game_state {
    u8 unk_00[0x78];
    s32 mode;           // 0x78
};

extern struct game_state *gLevelState;
extern void *gUnknown_030012BC;
extern s32 sub_8009FF4(struct box_part *part, struct part_aabb *box);
extern struct part_aabb sub_8007B98(struct box_part *part);
extern struct part_aabb sub_8007CF8(struct box_part *part);
extern u8 sub_8001688(struct part_aabb *a, struct part_aabb *b);
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);

/* obj->vtable[0x68](a, b, c) - the part's "hit" method. */
#define CALL_HIT(obj, a, b, c)                                                 \
    if (1) {                                                                   \
        struct part_method *_m = PART_METHOD(obj, 0x68);                       \
        ((part_method3_fn)_m->fn)((u8 *)(obj) + _m->thisOffset, (a), (b), (c)); \
    } else (void)0

/* Resolves a hit between `part` and the player (gUnknown_030012D8)
 * against the incoming box passed by sub_8008A40 (`list` is unused).
 *
 * In mode 3 (gLevelState->mode): if `part` touches the box
 * (`sub_8009FF4`), calls its hit method with the player's kind.
 * Otherwise, for a solid part (flags2 bit 3): builds the player's box
 * (sub_8007B98) and the part's (sub_8007CF8); on overlap pushes the
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
void sub_8008AD8(struct part_list *list, struct part_aabb box, struct box_part *part)
{
    if (gLevelState->mode == 3) {
        if (!sub_8009FF4(part, &box))
            return;
        CALL_HIT(part, 1, gUnknown_030012D8->kind, 0);
    } else if ((part->flags2 >> 3) & 1) {
        struct part_aabb a, b;
        s32 px;

        a = sub_8007B98(gUnknown_030012D8);
        b = sub_8007CF8(part);
        if (!sub_8001688(&a, &b))
            return;
        px = part->x;
        if (px < gUnknown_030012D8->x) {
            gUnknown_030012D8->x = px + ((b.w + a.w) << 7);
            CALL_HIT(gUnknown_030012D8, 0, 0xc, 2);
        } else {
            gUnknown_030012D8->x = px - ((b.w + a.w) << 7);
            CALL_HIT(gUnknown_030012D8, 0, 0xc, 1);
        }
    } else {
        u8 kind;

        switch (sub_8009FF4(part, &box)) {
        case 0:
            break;
        case 1:
        {
            u8 *flags = &gUnknown_030012D8->flags;
            *flags |= 8;
        }
            kind = gUnknown_030012D8->kind;
            if (kind == 1) {
                if (gUnknown_030012D8->unk_64 > 0) {
                    CALL_HIT(part, 1, 1, 0);
                    CALL_HIT(gUnknown_030012D8, 0, 0xd, 0);
                    PlaySfx(gUnknown_030012BC, 0x21, 0x100);
                }
            } else {
                CALL_HIT(part, 1, kind, 0);
            }
            break;
        case 2:
            part->flags |= 8;
            if (gLevelState->mode) {
                CALL_HIT(part, 1, 1, 0);
            }
            CALL_HIT(gUnknown_030012D8, 1, part->kind, 0);
            break;
        }
    }
}

/* sub_8008D80, this function's ROM-adjacent sibling (its collision-hit
 * logic mirror for a non-default "compare viewport"), lives in
 * src/graphics/actor_part7b.c instead of here - its real ROM address
 * isn't adjacent to this file's functions (actor_part10.c's
 * sub_8008C80/sub_8008CEC/sub_8008D30 sit between sub_8008AD8 above and
 * sub_8008D80 in ROM order), so it needs its own translation unit per
 * docs/workflow.md step 4. */
