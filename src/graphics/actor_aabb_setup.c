#include "core.h"

/* Same `struct aabb` shape as src/graphics/aabb_util.c/actor_part*.c -
 * duplicated here rather than shared, matching this project's existing
 * per-file convention for this struct (see docs/workflow.md/actor_part2.c
 * etc). */
struct aabb {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_c;
};

/* Set-size primitive - already referenced by name from several other
 * files (actor_part.c/actor_part2.c/oam_count.c's sub_8006600) as the
 * shared `sub_803AFE4`(set-position)/`sub_803AFDC`(set-size) pair. */
void sub_803AFDC(struct aabb *dest, s32 w, s32 h)
{
    dest->field_8 = w;
    dest->field_c = h;
}
asm(".align 2, 0");

/* Set-position primitive, see sub_803AFDC above. */
void sub_803AFE4(struct aabb *dest, s32 x, s32 y)
{
    dest->field_0 = x;
    dest->field_4 = y;
}
asm(".align 2, 0");

/* Trivial getter, offset 0x74 of an unknown/unnamed object - not enough
 * context from this call site alone to know the owning struct's shape,
 * so kept as a raw offset per docs/workflow.md's fallback for unclear
 * single-field access. */
s32 sub_803AFEC(void *self)
{
    return *(s32 *)((u8 *)self + 0x74);
}

extern u8 gStaticData_087E4D1C[];
extern u8 gStaticData_087E4D64[];
extern u8 gStaticData_087E4DAC[];
extern void sub_8026ED0(void *self);

/* Both sub_803AFF0/sub_803B024 below are per-type descriptor
 * constructors - the same "set one field of a passed-in struct to a
 * ROM data pointer, then conditionally call sub_8026ED0 based on a bit
 * in the second argument" shape documented at length in docs/rom_map.md
 * for the ~93-entry gStaticData_087E3BEC-family table (these three
 * entries - gStaticData_087E4D1C/4D64/4DAC, each 0x48 bytes - are
 * further members of that same family). Unusually, each writes to
 * `self+0x130` TWICE in a row with two DIFFERENT table pointers, the
 * second immediately clobbering the first - a genuinely dead first
 * store that's really in the ROM (confirmed: the two address
 * computations and both stores are distinct instructions, not a
 * disassembly artifact). The address is recomputed fresh for each
 * store via an inline-asm anchor (matching oam_count.c's established
 * technique for stopping gcc from CSE-ing/dead-store-eliminating a
 * repeated address expression, see docs/matching.md, "Matching
 * decompilation") - plain double `struct` field assignment collapses
 * to a single store no matter how it's phrased, tried first and
 * confirmed to regress before reaching for inline asm. */
void sub_803AFF0(void *self, u32 flags)
{
    void **addr;

    asm volatile("mov r0, #0x98\n\tlsl r0, r0, #1\n\tadd %0, %1, r0" : "=r"(addr) : "r"(self) : "r0");
    *addr = gStaticData_087E4D1C;
    asm volatile("mov r0, #0x98\n\tlsl r0, r0, #1\n\tadd %0, %1, r0" : "=r"(addr) : "r"(self) : "r0");
    *addr = gStaticData_087E4DAC;
    if (flags & 1) {
        sub_8026ED0(self);
    }
}

/* Same shape as sub_803AFF0 above, different first table pointer. */
void sub_803B024(void *self, u32 flags)
{
    void **addr;

    asm volatile("mov r0, #0x98\n\tlsl r0, r0, #1\n\tadd %0, %1, r0" : "=r"(addr) : "r"(self) : "r0");
    *addr = gStaticData_087E4D64;
    asm volatile("mov r0, #0x98\n\tlsl r0, r0, #1\n\tadd %0, %1, r0" : "=r"(addr) : "r"(self) : "r0");
    *addr = gStaticData_087E4DAC;
    if (flags & 1) {
        sub_8026ED0(self);
    }
}
