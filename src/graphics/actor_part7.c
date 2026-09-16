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

#if NON_MATCHING
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
 * NOT YET BYTE-MATCHING: the overall control flow, all four
 * `table+N`-trampoline call shapes (address adjusted once, then the
 * `s16` offset and function pointer both read relative to it), the
 * `sub_803A94C` block-copy invocation, and the `struct aabb` field
 * values are all confirmed correct - but this compiler puts the loop
 * counter `i` into a high register (`r8`, paired with a second high
 * register `r9` for the `boxB` pointer) instead of the ROM's low
 * register `r7` (with only `r8` used for `boxB`, avoiding a second
 * high-register save/restore entirely). Explicitly pinning `i` to
 * `register s32 i asm("r7")` does not fix this - it reproduces the
 * `r7`-pin corruption pattern documented at length elsewhere in this
 * ROM region (`sub_8007DBC`, `sub_8007FD8`): the pin partially takes
 * for the loop's entry check, then something in the loop body
 * silently reassigns r7 to an unrelated constant (`mov r7, #0x4`)
 * instead of preserving `i`, corrupting the reconstruction outright.
 * Parked with the version that avoids that corruption (natural
 * allocation into r8/r9) rather than risk a silent miscompile for a
 * cosmetically closer register match. A handful of the `table+N`
 * trampoline call sites also have their two reads (`s16` offset,
 * function pointer) in the opposite order from the ROM (fn read before
 * offset read, rather than after) - reordering the two source
 * statements did not change the compiled order. */
void sub_800891C(void *self)
{
    struct aabb boxA;
    struct aabb boxB;
    s32 i;
    void *subObj;

    boxA.field_8 = 0xdc << 9;
    boxA.field_c = 0x8c << 9;

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
        void *part = arr[i];
        u8 flags = *((u8 *)part + 0xc);

        if (flags & 1) {
            if (i < *(s32 *)self) {
                sub_803A94C(&arr[i + 1], &arr[i], ((*(s32 *)((u8 *)self + 4) - i) & 0x1FFFFF) | 0x4000000);
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
#endif /* NON_MATCHING */

#if NON_MATCHING
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
 * NOT YET BYTE-MATCHING: every branch, field offset, and call
 * argument confirmed correct - but `compareViewport` needs to survive
 * the whole loop across calls to `sub_803AD7C`/`sub_800014C`/
 * `sub_8008AD8`/`sub_8008D80`, and this compiler spills it to a high
 * register (`r8`, needing an extra push/pop pair the ROM doesn't
 * have) instead of the ROM's low register `r7`. Explicitly pinning it
 * to `register void *compareViewport asm("r7")` does NOT just fail to
 * help here - it produces a genuine miscompile: the loop counter `i`
 * (an ordinary, unpinned local) independently also gets allocated to
 * `r7`, and since both variables are simultaneously live across the
 * whole loop, the counter's own zero-initialization silently
 * overwrites `compareViewport` before its first use - a concrete,
 * newly-confirmed instance of the general "never pin r7 in this
 * toolchain" hazard already documented at length elsewhere in this
 * ROM region, this time corrupting a value rather than crashing the
 * compiler or dropping a push/pop entry. Parked with the version that
 * avoids the corruption (natural allocation into `r8`) rather than
 * risk a silently-wrong reconstruction for a cosmetically closer
 * register match. */
void sub_8008A40(void *manager, s32 boxX, s32 boxY, s32 boxW, s32 boxH, s32 unused, void *compareViewport)
{
    s32 i;
    s32 params[4];
    s32 box[4];

    params[0] = boxX;
    params[1] = boxY;
    params[2] = boxW;
    params[3] = boxH;

    for (i = 0; i < *(s32 *)((u8 *)manager + 8); i++) {
        void **arr = *(void ***)((u8 *)manager + 0x10);
        void *part = arr[i];
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
            sub_800014C(box, params, 0x10);
            sub_8008AD8(manager, box[0], box[1], box[2], box[3], part);
        } else {
            sub_800014C(box, params, 0x10);
            sub_8008D80(manager, box[0], box[1], box[2], box[3], part, compareViewport);
        }
    }
}
#endif /* NON_MATCHING */
asm(".align 2, 0");
