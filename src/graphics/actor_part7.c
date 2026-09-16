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
asm(".align 2, 0");
