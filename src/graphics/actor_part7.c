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

struct aabb {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_c;
};

extern void *gUnknown_03001308;
extern s32 sub_803AD80(void *arg0, void *arg1, void *fn);
extern void *sub_803AD7C(void *arg0, void *fn);
extern void sub_803A94C(const void *src, void *dst, u32 cnt);

/* `self` is a manager over an array of `part`-like objects (`self+0xc`,
 * length `self+0x4`) that gets filtered/compacted into a second output
 * array (`self+0x10`, length `self+0x8`) each call. Builds two
 * `gUnknown_03001308`-sub-object-centered boxes first: an "extended"
 * 440x280 region 100/60 px past the sub-object's own position
 * (`boxA`), and the plain 240x160 screen region at the sub-object's
 * own position (`boxB`, the GBA's exact visible area in Q8) - reusing
 * the same `gUnknown_03001308+0x10` sub-object convention documented
 * throughout this ROM region (see `sub_8007F78`/`sub_8006FE4`).
 *
 * For each `part` in the array: if `part+0xc` bit 0 is set, and its
 * index is still below `self+0x0`, removes it from the array via a
 * `sub_803A94C` (the GBA BIOS `CpuSet` SWI, confirmed in
 * `docs/rom_map.md`) block-copy shifting every later element down by
 * one slot, decrementing `self+0x4` and clearing the vacated last
 * slot - then (whether or not it was actually removed) calls a
 * `part->table`-driven trampoline at table offset 0x50/0x54 with a
 * constant argument `3` via `sub_803AD80` (same
 * `table+N`/`table+N+4` offset/function-pointer convention as
 * `sub_8006FE4`/`sub_8007F78`/`sub_8008364`), and re-examines the same
 * index next iteration (`i--`) to account for the shift.
 *
 * Otherwise (bit 0 clear): tests the `part` against `boxA` through the
 * table's 0x40/0x44 trampoline; if that passes, fires the table's
 * 0x18/0x1c trampoline (return value discarded) and then tests against
 * `boxB` through the table's 0x30/0x34 trampoline; if THAT also
 * passes, appends `part` to the output array and increments its count.
 *
 * **Build toggle**: default builds (`NON_MATCHING=0`) use the `#else`
 * branch's NAKED transcription of the ROM's own confirmed-correct
 * instructions, byte-exact, same technique as the other functions
 * above. The `#if NON_MATCHING` branch is a since-refined C
 * reconstruction (see docs/matching.md's "Parked, not matched:
 * sub_800891C" for the earlier attempt this supersedes, and its
 * appended follow-up note for this session's findings): pinning `self`
 * to `r5` (its own ROM register, safe - `self` has no r7-hazard, it's
 * just an ordinary call-crossing pointer) raises register pressure
 * enough that gcc's *natural, unforced* allocator reaches for `r7` for
 * the loop counter `i` on its own, closing the `r8`/`r9`-pair mismatch
 * this function was originally parked for down to a single `r8` (for
 * the `boxB` pointer) - matching the ROM's own register *class* usage
 * exactly, without ever touching the unsafe explicit `register s32 i
 * asm("r7")` pin. Splitting the `boxA.field_8`/`field_c` computation
 * into two separate plain locals (computed before either store, rather
 * than sequentially) also fixed that pair's `mov r0`/`mov r1`
 * parallel-materialization to match the ROM exactly. Not yet byte-exact
 * though: several more scratch-register-letter gaps remain in both the
 * box-setup section (e.g. `subObj`'s own register, and which operand
 * becomes the addition's accumulator for `boxA.field_0`/`field_4`) and
 * the loop body (the ROM caches `&arr[i]` in `ip` across the removal
 * call and reuses a stale-cached `count` register inside the loop
 * body, neither of which this reconstruction reproduces - attempting a
 * `count` local mirroring the ROM's cached-register reuse regressed
 * `i` back onto `r8`/`r9`, reverted) - same category of resistant
 * "which anonymous register" gap documented at length for
 * `sub_8007B98`. Kept here despite not being byte-exact since it's a
 * genuine, confirmed step forward on this function's actual documented
 * blocker (the register *class*, not just letter) - the NAKED branch
 * stays the default. */
#if NON_MATCHING
void sub_800891C(void *selfParam)
{
    register void *self asm("r5") = selfParam;
    struct aabb boxA;
    struct aabb boxB;
    s32 i;
    register void *subObj asm("r2");

    {
        s32 t1 = 0xdc << 9;
        s32 t2 = 0x8c << 9;
        boxA.field_8 = t1;
        boxA.field_c = t2;
    }

    subObj = *(void **)((u8 *)gUnknown_03001308 + 0x10);
    boxA.field_0 = (*(s32 *)subObj << 8) + (s32)0xFFFF9C00;
    boxA.field_4 = (*(s32 *)((u8 *)subObj + 4) << 8) + (s32)0xFFFFC400;

    boxB.field_0 = *(s32 *)subObj << 8;
    boxB.field_4 = *(s32 *)((u8 *)subObj + 4) << 8;
    boxB.field_8 = 0xf0 << 8;
    boxB.field_c = 0xa0 << 8;

    *(s32 *)((u8 *)self + 8) = 0;

    for (i = 0; i < *(s32 *)((u8 *)self + 4); i++) {
        void **arr = *(void ***)((u8 *)self + 0xc);
        void **elemAddr = &arr[i];
        void *part = *elemAddr;
        u8 flags = *((u8 *)part + 0xc);

        if (flags & 1) {
            if (i < *(s32 *)self) {
                sub_803A94C(elemAddr + 1, elemAddr, ((*(s32 *)((u8 *)self + 4) - i) & 0x1FFFFF) | 0x4000000);
                *(s32 *)((u8 *)self + 4) -= 1;
                arr[*(s32 *)((u8 *)self + 4)] = 0;
            }
            if (part != 0) {
                u8 *rec = *(u8 **)((u8 *)part + 0x18) + 0x50;
                s16 offset = *(s16 *)rec;
                void *fn = *(void **)(rec + 4);

                sub_803AD80((u8 *)part + offset, (void *)3, fn);
            }
            i--;
        } else {
            u8 *rec1 = *(u8 **)((u8 *)part + 0x18) + 0x40;
            s16 offset1 = *(s16 *)rec1;
            void *fn1 = *(void **)(rec1 + 4);

            if (sub_803AD80((u8 *)part + offset1, &boxA, fn1)) {
                u8 *rec2 = *(u8 **)((u8 *)part + 0x18) + 0x18;
                s16 offset2 = *(s16 *)rec2;
                void *fn2 = *(void **)(rec2 + 4);
                u8 *rec3;
                s16 offset3;
                void *fn3;

                sub_803AD7C((u8 *)part + offset2, fn2);

                rec3 = *(u8 **)((u8 *)part + 0x18) + 0x30;
                offset3 = *(s16 *)rec3;
                fn3 = *(void **)(rec3 + 4);

                if (sub_803AD80((u8 *)part + offset3, &boxB, fn3)) {
                    void **outArr = *(void ***)((u8 *)self + 0x10);
                    s32 outCount = *(s32 *)((u8 *)self + 8);

                    outArr[outCount] = part;
                    *(s32 *)((u8 *)self + 8) = outCount + 1;
                }
            }
        }
    }
}
#else
NAKED void sub_800891C(void *self)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, r8\n\t"
        "push {r7}\n\t"
        "sub sp, #0x20\n\t"
        "add r5, r0, #0\n\t"
        "mov r0, #0xdc\n\t"
        "lsl r0, r0, #9\n\t"
        "mov r1, #0x8c\n\t"
        "lsl r1, r1, #9\n\t"
        "str r0, [sp, #8]\n\t"
        "str r1, [sp, #0xc]\n\t"
        "ldr r0, 4f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r2, [r0, #0x10]\n\t"
        "ldr r1, [r2]\n\t"
        "lsl r1, r1, #8\n\t"
        "mov r3, #0\n\t"
        "ldr r0, 5f\n\t"
        "add r1, r1, r0\n\t"
        "ldr r0, [r2, #4]\n\t"
        "lsl r0, r0, #8\n\t"
        "ldr r4, 6f\n\t"
        "add r0, r0, r4\n\t"
        "str r1, [sp]\n\t"
        "str r0, [sp, #4]\n\t"
        "ldr r1, [r2]\n\t"
        "lsl r1, r1, #8\n\t"
        "ldr r0, [r2, #4]\n\t"
        "lsl r0, r0, #8\n\t"
        "str r1, [sp, #0x10]\n\t"
        "str r0, [sp, #0x14]\n\t"
        "add r0, sp, #0x10\n\t"
        "mov r1, #0xf0\n\t"
        "lsl r1, r1, #8\n\t"
        "mov r2, #0xa0\n\t"
        "lsl r2, r2, #8\n\t"
        "str r1, [r0, #8]\n\t"
        "str r2, [r0, #0xc]\n\t"
        "str r3, [r5, #8]\n\t"
        "mov r7, #0\n\t"
        "ldr r2, [r5, #4]\n\t"
        "mov r8, r0\n\t"
        "cmp r7, r2\n\t"
        "bge 10f\n\t"
    "1:\n\t"
        "ldr r3, [r5, #0xc]\n\t"
        "lsl r1, r7, #2\n\t"
        "add r6, r1, r3\n\t"
        "mov ip, r6\n\t"
        "ldr r4, [r6]\n\t"
        "mov r0, #1\n\t"
        "ldrb r6, [r4, #0xc]\n\t"
        "and r0, r6\n\t"
        "cmp r0, #0\n\t"
        "beq 8f\n\t"
        "ldr r0, [r5]\n\t"
        "cmp r7, r0\n\t"
        "bge 2f\n\t"
        "add r0, r1, #4\n\t"
        "add r0, r3, r0\n\t"
        "sub r2, r2, r7\n\t"
        "ldr r1, 7f\n\t"
        "and r2, r1\n\t"
        "mov r1, #0x80\n\t"
        "lsl r1, r1, #0x13\n\t"
        "orr r2, r1\n\t"
        "mov r1, ip\n\t"
        "bl sub_803A94C\n\t"
        "ldr r0, [r5, #4]\n\t"
        "sub r0, #1\n\t"
        "str r0, [r5, #4]\n\t"
        "ldr r1, [r5, #0xc]\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "mov r1, #0\n\t"
        "str r1, [r0]\n\t"
    "2:\n\t"
        "cmp r4, #0\n\t"
        "beq 3f\n\t"
        "ldr r1, [r4, #0x18]\n\t"
        "add r1, #0x50\n\t"
        "mov r2, #0\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r2, [r1, #4]\n\t"
        "mov r1, #3\n\t"
        "bl sub_803AD80\n\t"
    "3:\n\t"
        "sub r7, #1\n\t"
        "b 9f\n\t"
        ".align 2, 0\n"
    "4: .4byte gUnknown_03001308\n"
    "5: .4byte 0xFFFF9C00\n"
    "6: .4byte 0xFFFFC400\n"
    "7: .4byte 0x001FFFFF\n"
    "8:\n\t"
        "ldr r1, [r4, #0x18]\n\t"
        "add r1, #0x40\n\t"
        "mov r6, #0\n\t"
        "ldrsh r0, [r1, r6]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r2, [r1, #4]\n\t"
        "mov r1, sp\n\t"
        "bl sub_803AD80\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq 9f\n\t"
        "ldr r1, [r4, #0x18]\n\t"
        "mov r2, #0x18\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r1, #0x1c]\n\t"
        "bl sub_803AD7C\n\t"
        "ldr r1, [r4, #0x18]\n\t"
        "mov r6, #0x30\n\t"
        "ldrsh r0, [r1, r6]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r2, [r1, #0x34]\n\t"
        "mov r1, r8\n\t"
        "bl sub_803AD80\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq 9f\n\t"
        "ldr r0, [r5, #8]\n\t"
        "ldr r2, [r5, #0x10]\n\t"
        "lsl r1, r0, #2\n\t"
        "add r1, r1, r2\n\t"
        "str r4, [r1]\n\t"
        "add r0, #1\n\t"
        "str r0, [r5, #8]\n\t"
    "9:\n\t"
        "add r7, #1\n\t"
        "ldr r2, [r5, #4]\n\t"
        "cmp r7, r2\n\t"
        "blt 1b\n\t"
    "10:\n\t"
        "add sp, #0x20\n\t"
        "pop {r3}\n\t"
        "mov r8, r3\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    );
}
#endif /* NON_MATCHING */

extern void *sub_803AD7C(void *arg0, void *fn);
extern void *sub_800014C(void *dest, void *src, s32 size);
extern void sub_8008AD8(void *manager, s32 x, s32 y, s32 w, s32 h, void *part);
extern void sub_8008D80(void *manager, s32 x, s32 y, s32 w, s32 h, void *part, void *arg6);
extern void *gUnknown_030012D8;

/* Iterates `manager`'s array of `part`-like objects (`manager+0x10`
 * base, `manager+8` count) - the same `self+0xc`/`self+0x10` shape as
 * `sub_800891C` above, just at different offsets. For each `part`:
 * fires a `part->table+0x48/0x4c`-driven trampoline via `sub_803AD7C`
 * (same convention throughout this ROM region); skip if the result is
 * `<= 4` (a distance/priority-style broad-phase test). Skip unless
 * `part->flags` bit 2 is set. Then, depending on whether the caller's
 * `compareViewport` argument is the current `gUnknown_030012D8`
 * (confirmed elsewhere to be "very likely the camera/viewport" - see
 * `docs/rom_map.md`) or a *different* one, dispatches the incoming
 * `{boxX, boxY, boxW, boxH}` rectangle (passed in across `r1`-`r3`
 * plus one stack word, following the usual GBA Thumb ABI for more
 * than 3 scalar arguments) to `sub_8008AD8` or `sub_8008D80` (the
 * latter also forwarding `compareViewport` itself as a 6th argument) -
 * reads like "resolve collision against parts near this box, routing
 * differently when testing across a screen/room boundary vs within
 * the active one". `sub_800014C` (used here purely to relocate the
 * incoming box onto a fresh stack slot before unpacking it again for
 * the sub-call - its own signature and behavior, `void *sub_800014C
 * (void *dest, void *src, s32 size)`, confirmed by reading its
 * definition directly in `asm/crt0.s`) turned out to be a plain
 * `memcpy`-style wrapper around the GBA BIOS `CpuSet` SWI
 * (`sub_803A94C`, already identified for `sub_800891C` above) -
 * resolving one of `docs/rom_map.md`'s long-open "packed state
 * round-tripping" mysteries around this function.
 *
 * **Build toggle**: default builds (`NON_MATCHING=0`) use the `#else`
 * branch's NAKED transcription of the ROM's own confirmed-correct
 * instructions, byte-exact, same technique as the other functions
 * above. The `#if NON_MATCHING` branch is a since-refined C
 * reconstruction (see docs/matching.md's "Parked, not matched:
 * sub_8008A40" for the earlier attempt this supersedes, and its
 * appended follow-up note for this session's findings): pinning
 * `manager` to `r5`, the loop counter `i` to `r6`, and `part` to `r4`
 * (all their own ROM registers, all safe - like `sub_800891C`'s `self`
 * above, none of these have any r7-hazard, they're ordinary
 * call-crossing values) raises register pressure enough that gcc's
 * *natural, unforced* allocator reaches for `r7` for `compareViewport`
 * on its own, matching the ROM - without ever touching the unsafe
 * explicit `register void *compareViewport asm("r7")` pin that
 * corrupted `compareViewport` in the earlier attempt. Not yet
 * byte-exact, and the `r8` push/pop pair the earlier attempt was
 * parked over is still present, just for a different, smaller-scoped
 * reason now: pinning the copied-box pointer (`boxp`, from
 * `sub_800014C`'s destination) to `r2` was needed to stop gcc from
 * hoisting its address into `r7` instead of `compareViewport` (the
 * ROM never caches this address at all, recomputing `sp`-relative each
 * time - not reproduced here), but `r2` collides with `sub_8008AD8`/
 * `sub_8008D80`'s own `y` argument register, forcing gcc to shuffle
 * `boxp[1]` through `r8` as a temporary before `r2` gets reused - an
 * unpinned `boxp` (or plain `box[N]` indexing) avoids that shuffle but
 * puts the address-hoist back onto `r7` instead, regressing
 * `compareViewport`; reordering the read earlier via an explicit `by`
 * temporary didn't change gcc's own internal scheduling either (both
 * tried and reverted). Several more scratch-register-letter gaps also
 * remain elsewhere in the loop body (same category of resistant
 * "which anonymous register" gap documented at length for
 * `sub_8007B98`/`sub_800891C` above). Kept here despite not being
 * byte-exact since it's a genuine, confirmed step forward on this
 * function's actual documented blocker (getting `compareViewport`
 * itself onto its correct low register) - the NAKED branch stays the
 * default. */
#if NON_MATCHING
extern void *sub_803AD7C(void *arg0, void *fn);
extern void *sub_800014C(void *dest, void *src, s32 size);
extern void sub_8008AD8(void *manager, s32 x, s32 y, s32 w, s32 h, void *part);
extern void sub_8008D80(void *manager, s32 x, s32 y, s32 w, s32 h, void *part, void *arg6);
extern void *gUnknown_030012D8;

void sub_8008A40(void *managerParam, s32 boxX, s32 boxY, s32 boxW, s32 boxH, s32 unused, void *compareViewport)
{
    register void *manager asm("r5") = managerParam;
    register s32 i asm("r6");
    s32 params[4];
    s32 box[4];

    params[0] = boxX;
    params[1] = boxY;
    params[2] = boxW;
    params[3] = boxH;

    for (i = 0; i < *(s32 *)((u8 *)manager + 8); i++) {
        void **arr = *(void ***)((u8 *)manager + 0x10);
        register void *part asm("r4") = arr[i];
        u8 *rec = *(u8 **)((u8 *)part + 0x18) + 0x48;
        s16 offset = *(s16 *)rec;
        void *fn = *(void **)(rec + 4);
        s32 result = (s32)sub_803AD7C((u8 *)part + offset, fn);

        if (result <= 4) {
            continue;
        }
        if (!((*((u8 *)part + 0xc) >> 2) & 1)) {
            continue;
        }
        if (compareViewport == gUnknown_030012D8) {
            register s32 *boxp asm("r2") = box;
            sub_800014C(boxp, params, 0x10);
            sub_8008AD8(manager, boxp[0], boxp[1], boxp[2], boxp[3], part);
        } else {
            register s32 *boxp asm("r2") = box;
            sub_800014C(boxp, params, 0x10);
            sub_8008D80(manager, boxp[0], boxp[1], boxp[2], boxp[3], part, compareViewport);
        }
    }
}
#else
NAKED void sub_8008A40(void *manager, s32 boxX, s32 boxY, s32 boxW, s32 boxH, s32 unused, void *compareViewport)
{
    asm(
        "sub sp, #0xc\n\t"
        "push {r4, r5, r6, r7, lr}\n\t"
        "sub sp, #0x1c\n\t"
        "add r5, r0, #0\n\t"
        "str r1, [sp, #0x30]\n\t"
        "str r2, [sp, #0x34]\n\t"
        "str r3, [sp, #0x38]\n\t"
        "ldr r7, [sp, #0x44]\n\t"
        "mov r6, #0\n\t"
        "b 5f\n\t"
    "1:\n\t"
        "ldr r1, [r5, #0x10]\n\t"
        "lsl r0, r6, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldr r4, [r0]\n\t"
        "ldr r1, [r4, #0x18]\n\t"
        "add r1, #0x48\n\t"
        "mov r2, #0\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r1, #4]\n\t"
        "bl sub_803AD7C\n\t"
        "cmp r0, #4\n\t"
        "ble 4f\n\t"
        "ldrb r1, [r4, #0xc]\n\t"
        "lsr r0, r1, #2\n\t"
        "mov r1, #1\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "beq 4f\n\t"
        "ldr r0, 2f\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r7, r0\n\t"
        "bne 3f\n\t"
        "add r0, sp, #0xc\n\t"
        "add r1, sp, #0x30\n\t"
        "mov r2, #0x10\n\t"
        "bl sub_800014C\n\t"
        "str r4, [sp, #4]\n\t"
        "ldr r0, [sp, #0x18]\n\t"
        "str r0, [sp]\n\t"
        "ldr r1, [sp, #0xc]\n\t"
        "ldr r2, [sp, #0x10]\n\t"
        "ldr r3, [sp, #0x14]\n\t"
        "add r0, r5, #0\n\t"
        "bl sub_8008AD8\n\t"
        "b 4f\n\t"
        ".align 2, 0\n"
    "2: .4byte gUnknown_030012D8\n"
    "3:\n\t"
        "add r0, sp, #0xc\n\t"
        "add r1, sp, #0x30\n\t"
        "mov r2, #0x10\n\t"
        "bl sub_800014C\n\t"
        "str r4, [sp, #4]\n\t"
        "str r7, [sp, #8]\n\t"
        "ldr r0, [sp, #0x18]\n\t"
        "str r0, [sp]\n\t"
        "ldr r1, [sp, #0xc]\n\t"
        "ldr r2, [sp, #0x10]\n\t"
        "ldr r3, [sp, #0x14]\n\t"
        "add r0, r5, #0\n\t"
        "bl sub_8008D80\n\t"
    "4:\n\t"
        "add r6, #1\n\t"
    "5:\n\t"
        "ldr r0, [r5, #8]\n\t"
        "cmp r6, r0\n\t"
        "blt 1b\n\t"
        "add sp, #0x1c\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r3}\n\t"
        "add sp, #0xc\n\t"
        "bx r3\n\t"
    );
}
#endif /* NON_MATCHING */

extern void *gUnknown_030012C0;
extern void *gUnknown_030012BC;
extern s32 sub_8009FF4(void *part, void *region);
extern void *sub_8007B98(void *dest, void *pt);
extern void *sub_8007CF8(void *dest, void *pt);
extern u8 sub_8001688(void *buf1, void *buf2);
extern void sub_803AD88(void *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);

/* Resolves collision push-out between `part` and the player
 * (`gUnknown_030012D8`, confirmed "very likely the camera/viewport"
 * elsewhere - see `docs/rom_map.md`) against the incoming
 * `{boxX, boxY, boxW, boxH}` rectangle passed by `sub_8008A40` above.
 * `manager` itself is never read - a dead parameter kept for a
 * uniform call signature with `sub_8008D80`'s sibling.
 *
 * If `gUnknown_030012C0`'s mode field (`+0x78`) is 3: tests `part`
 * against the box via `sub_8009FF4` (already matched in
 * `actor_part9.c`); if it misses entirely, returns. Otherwise fires
 * a `part->table+0x68`-driven trampoline via `sub_803AD88` with the
 * player's `+0xa` byte as the third argument.
 *
 * Otherwise, if `part+0xd` bit 3 is set (a "large object" case):
 * builds the player's AABB via `sub_8007B98` (parked as `NON_MATCHING`
 * in `actor_part.c`) and `part`'s secondary AABB via `sub_8007CF8`
 * (already matched), tests them via `sub_8001688`; on a hit, pushes
 * the player's X position away from `part` by the sum of both boxes'
 * widths (`<<7`, i.e. `*128`) in whichever direction `part` is
 * relative to the player, then fires a `player->table+0x68`
 * trampoline (direction encoded in the final argument, `2` or `1`).
 *
 * Otherwise: tests `part` against the box again via `sub_8009FF4`.
 * Result 1: sets the player's `flags` bit 3; if the player's `+0xa`
 * byte is exactly 1 and its `+0x64` counter is positive, fires two
 * trampolines (`part`'s own, then the player's) and plays SFX `0x21`
 * via `gUnknown_030012BC`; otherwise (byte != 1) fires a single
 * `part`-table trampoline with the player's `+0xa` byte as the third
 * argument (the same shared tail the mode-3 branch above also
 * reaches). Result 2: sets `part->flags` bit 3; if the mode field is
 * nonzero, fires a `part`-table trampoline first; either way, then
 * fires a `player`-table trampoline with `part`'s own `+0xa` byte as
 * the third argument.
 *
 * Every `table+0x68/0x6c`-driven trampoline call needed its function-
 * pointer half marked as a "dead read" (loaded into `r4` but never
 * actually passed to `sub_803AD88`, a plain 4-argument function, not
 * itself a trampoline) - the same idiom already confirmed for
 * `sub_8007DBC`/`sub_8009FD4`'s own `sub_803AD88` calls.
 *
 * See the (now removed) NON_MATCHING C draft in git history for the
 * full commented C reconstruction - every branch, field offset, and
 * call argument across this ~150-instruction function was confirmed
 * correct there. Two small structural gaps stopped it from closing:
 * (1) this compiler has no way to express "this scalar parameter is
 * already sitting in the right stack position for the callee I'm
 * about to build a struct pointer into" - the ROM's `box` argument to
 * `sub_8009FF4`/`sub_8007CF8` leaves `boxH` untouched in its own
 * incoming stack slot (an ABI stack-layout coincidence, the same
 * trick `sub_8008A40`'s own box-passing above relies on), while a
 * C-level `struct aabb box; box.field_c = boxH;` necessarily emits a
 * real load-then-store pair to populate a fresh local struct instead;
 * (2) `part` landed in `r6` throughout instead of the ROM's `r5`,
 * needing one extra pushed register as a knock-on effect - see
 * docs/matching.md's "Parked, not matched: sub_8008AD8" for the full
 * account. Written as NAKED asm here instead, same technique as the
 * other functions above. */
NAKED void sub_8008AD8(void *manager, s32 boxX, s32 boxY, s32 boxW, s32 boxH, void *partArg)
{
    asm(
        "sub sp, #0xc\n\t"
        "push {r4, r5, r6, lr}\n\t"
        "sub sp, #0x20\n\t"
        "str r1, [sp, #0x30]\n\t"
        "str r2, [sp, #0x34]\n\t"
        "str r3, [sp, #0x38]\n\t"
        "ldr r5, [sp, #0x40]\n\t"
        "ldr r4, 2f\n\t"
        "ldr r0, [r4]\n\t"
        "ldr r0, [r0, #0x78]\n\t"
        "cmp r0, #3\n\t"
        "bne 4f\n\t"
        "add r0, r5, #0\n\t"
        "add r1, sp, #0x30\n\t"
        "bl sub_8009FF4\n\t"
        "cmp r0, #0\n\t"
        "bne 1f\n\t"
        "b 16f\n\t"
    "1:\n\t"
        "ldr r3, [r5, #0x18]\n\t"
        "add r3, #0x68\n\t"
        "mov r1, #0\n\t"
        "ldrsh r0, [r3, r1]\n\t"
        "add r0, r5, r0\n\t"
        "ldr r1, 3f\n\t"
        "ldr r1, [r1]\n\t"
        "ldrb r2, [r1, #0xa]\n\t"
        "ldr r4, [r3, #4]\n\t"
        "b 13f\n\t"
        ".align 2, 0\n"
    "2: .4byte gUnknown_030012C0\n"
    "3: .4byte gUnknown_030012D8\n"
    "4:\n\t"
        "ldrb r2, [r5, #0xd]\n\t"
        "lsr r0, r2, #3\n\t"
        "mov r1, #1\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "beq 8f\n\t"
        "ldr r6, 6f\n\t"
        "ldr r1, [r6]\n\t"
        "mov r0, sp\n\t"
        "bl sub_8007B98\n\t"
        "add r4, sp, #0x10\n\t"
        "add r0, r4, #0\n\t"
        "add r1, r5, #0\n\t"
        "bl sub_8007CF8\n\t"
        "mov r0, sp\n\t"
        "add r1, r4, #0\n\t"
        "bl sub_8001688\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne 5f\n\t"
        "b 16f\n\t"
    "5:\n\t"
        "ldr r3, [r5]\n\t"
        "ldr r2, [r6]\n\t"
        "ldr r0, [r2]\n\t"
        "cmp r3, r0\n\t"
        "bge 7f\n\t"
        "ldr r0, [r4, #8]\n\t"
        "ldr r1, [sp, #8]\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #7\n\t"
        "add r0, r3, r0\n\t"
        "str r0, [r2]\n\t"
        "ldr r1, [r2, #0x18]\n\t"
        "add r1, #0x68\n\t"
        "mov r3, #0\n\t"
        "ldrsh r0, [r1, r3]\n\t"
        "add r0, r2, r0\n\t"
        "ldr r4, [r1, #4]\n\t"
        "mov r1, #0\n\t"
        "mov r2, #0xc\n\t"
        "mov r3, #2\n\t"
        "bl sub_803AD88\n\t"
        "b 16f\n\t"
        ".align 2, 0\n"
    "6: .4byte gUnknown_030012D8\n"
    "7:\n\t"
        "ldr r0, [r4, #8]\n\t"
        "ldr r1, [sp, #8]\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #7\n\t"
        "sub r0, r3, r0\n\t"
        "str r0, [r2]\n\t"
        "ldr r1, [r2, #0x18]\n\t"
        "add r1, #0x68\n\t"
        "mov r3, #0\n\t"
        "ldrsh r0, [r1, r3]\n\t"
        "add r0, r2, r0\n\t"
        "ldr r4, [r1, #4]\n\t"
        "mov r1, #0\n\t"
        "mov r2, #0xc\n\t"
        "mov r3, #1\n\t"
        "bl sub_803AD88\n\t"
        "b 16f\n\t"
    "8:\n\t"
        "add r0, r5, #0\n\t"
        "add r1, sp, #0x30\n\t"
        "bl sub_8009FF4\n\t"
        "cmp r0, #1\n\t"
        "beq 9f\n\t"
        "cmp r0, #1\n\t"
        "ble 16f\n\t"
        "cmp r0, #2\n\t"
        "beq 14f\n\t"
        "b 16f\n\t"
    "9:\n\t"
        "ldr r6, 10f\n\t"
        "ldr r1, [r6]\n\t"
        "mov r0, #8\n\t"
        "ldrb r2, [r1, #0xc]\n\t"
        "orr r0, r2\n\t"
        "strb r0, [r1, #0xc]\n\t"
        "ldr r0, [r6]\n\t"
        "ldrb r2, [r0, #0xa]\n\t"
        "cmp r2, #1\n\t"
        "bne 12f\n\t"
        "ldr r0, [r0, #0x64]\n\t"
        "cmp r0, #0\n\t"
        "ble 16f\n\t"
        "ldr r1, [r5, #0x18]\n\t"
        "add r1, #0x68\n\t"
        "mov r3, #0\n\t"
        "ldrsh r0, [r1, r3]\n\t"
        "add r0, r5, r0\n\t"
        "ldr r4, [r1, #4]\n\t"
        "mov r1, #1\n\t"
        "mov r2, #1\n\t"
        "mov r3, #0\n\t"
        "bl sub_803AD88\n\t"
        "ldr r0, [r6]\n\t"
        "ldr r1, [r0, #0x18]\n\t"
        "add r1, #0x68\n\t"
        "mov r3, #0\n\t"
        "ldrsh r2, [r1, r3]\n\t"
        "add r0, r0, r2\n\t"
        "ldr r4, [r1, #4]\n\t"
        "mov r1, #0\n\t"
        "mov r2, #0xd\n\t"
        "mov r3, #0\n\t"
        "bl sub_803AD88\n\t"
        "ldr r0, 11f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "mov r1, #0x21\n\t"
        "bl PlaySfx\n\t"
        "b 16f\n\t"
        ".align 2, 0\n"
    "10: .4byte gUnknown_030012D8\n"
    "11: .4byte gUnknown_030012BC\n"
    "12:\n\t"
        "ldr r1, [r5, #0x18]\n\t"
        "add r1, #0x68\n\t"
        "mov r3, #0\n\t"
        "ldrsh r0, [r1, r3]\n\t"
        "add r0, r5, r0\n\t"
        "ldr r4, [r1, #4]\n\t"
    "13:\n\t"
        "mov r1, #1\n\t"
        "mov r3, #0\n\t"
        "bl sub_803AD88\n\t"
        "b 16f\n\t"
    "14:\n\t"
        "mov r0, #8\n\t"
        "ldrb r1, [r5, #0xc]\n\t"
        "orr r0, r1\n\t"
        "strb r0, [r5, #0xc]\n\t"
        "ldr r0, [r4]\n\t"
        "ldr r0, [r0, #0x78]\n\t"
        "cmp r0, #0\n\t"
        "beq 15f\n\t"
        "ldr r1, [r5, #0x18]\n\t"
        "add r1, #0x68\n\t"
        "mov r2, #0\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r5, r0\n\t"
        "ldr r4, [r1, #4]\n\t"
        "mov r1, #1\n\t"
        "mov r2, #1\n\t"
        "mov r3, #0\n\t"
        "bl sub_803AD88\n\t"
    "15:\n\t"
        "ldr r0, 17f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r0, #0x18]\n\t"
        "add r1, #0x68\n\t"
        "mov r3, #0\n\t"
        "ldrsh r2, [r1, r3]\n\t"
        "add r0, r0, r2\n\t"
        "ldrb r2, [r5, #0xa]\n\t"
        "ldr r4, [r1, #4]\n\t"
        "mov r1, #1\n\t"
        "mov r3, #0\n\t"
        "bl sub_803AD88\n\t"
    "16:\n\t"
        "add sp, #0x20\n\t"
        "pop {r4, r5, r6}\n\t"
        "pop {r3}\n\t"
        "add sp, #0xc\n\t"
        "bx r3\n\t"
        ".align 2, 0\n"
    "17: .4byte gUnknown_030012D8\n"
    );
}

/* sub_8008D80, this function's ROM-adjacent sibling (its collision-hit
 * logic mirror for a non-default "compare viewport"), lives in
 * src/graphics/actor_part7b.c instead of here - its real ROM address
 * isn't adjacent to this file's functions (actor_part10.c's
 * sub_8008C80/sub_8008CEC/sub_8008D30 sit between sub_8008AD8 above and
 * sub_8008D80 in ROM order), so it needs its own translation unit per
 * docs/workflow.md step 4. */
