#include "core.h"
#include "actor.h"

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

/* `part+0x30` (frame index) getter. */
s32 sub_8008814(void *part)
{
    return *(s32 *)((u8 *)part + 0x30);
}

/* `part+0x34` (sub-counter) getter. */
s32 sub_8008818(void *part)
{
    return *(s32 *)((u8 *)part + 0x34);
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

/* `part+0x3c` (u16) get/set pair. */
u16 sub_8008888(void *part)
{
    return *(u16 *)((u8 *)part + 0x3c);
}

void sub_800888C(void *part, u16 val)
{
    *(u16 *)((u8 *)part + 0x3c) = val;
}

extern void *gUnknown_030012CC;
extern void sub_8007634(void *unused, void *part, s32 *posPtr);
extern void sub_80073DC(void *unused, void *part, s32 *posPtr);

/* Resolves `part`'s Q8 position plus a caller-supplied offset into a
 * stack `{x, y}` pair, then dispatches to `sub_8007634` or
 * `sub_80073DC` (both already matched/parked elsewhere in this ROM
 * region) depending on whether `part+0x3c` is set. */
void sub_8008890(struct actor *part, s32 arg1, s32 arg2)
{
    s32 pos[2];

    pos[0] = (part->x >> 8) + arg1;
    pos[1] = (part->y >> 8) + arg2;

    if (*(u16 *)((u8 *)part + 0x3c) != 0) {
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
asm(".align 2, 0");
