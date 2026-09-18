#include "core.h"

/* GitHub issue #14: 0x08010A0C-0x08010D54, continuing the physics/
 * collision subsystem (`game_loop17.c`-`game_loop26.c`, see
 * docs/matching/issue-13-graphics-fc70.md). `sub_8010A00` right before
 * this function is already matched in game_loop26.c; everything here
 * operates on the same `self` type `sub_8010A00`/`sub_800FEB0` do - the
 * viewport's own "collision box" sub-record embedded at
 * `gUnknown_030012D8+0x108` (confirmed by `sub_80106DC` in
 * game_loop23.c, which already calls `sub_8010B6C(gUnknown_030012D8 +
 * 0x108)`). */

extern void *gUnknown_030012D8;

/* Getter for `self+0x48` bits 6-7 - already matched, game_loop26.c. */
extern u32 sub_8010A00(void *selfArg);

/* Decrements `self+0x48`'s bits 6-7 sub-state by one, if it isn't
 * already zero. */
void sub_8010A0C(void *selfArg)
{
    u8 *self = selfArg;
    u8 state = (u8)sub_8010A00(self);

    if (state != 0) {
        u8 newState = (u8)(state - 1);
        *(u32 *)(self + 0x48) = (*(u32 *)(self + 0x48) & 0x3f) | (newState << 6);
    }
}

/* Setter for `self+0x48` bits 6-7. */
void sub_8010A34(void *selfArg, u32 state)
{
    u8 *self = selfArg;
    u8 s = (u8)state;
    *(u32 *)(self + 0x48) = (*(u32 *)(self + 0x48) & 0x3f) | (s << 6);
}

/* Clears `self+0x48` bits 6-7. */
void sub_8010A44(void *selfArg)
{
    u8 *self = selfArg;
    u32 v = *(u32 *)(self + 0x48);
    v &= 0x3f;
    *(u32 *)(self + 0x48) = v;
}

/* Getter for `self+0x48` bits 3-5. */
u32 sub_8010A50(void *selfArg)
{
    u8 *self = selfArg;
    return (*(u32 *)(self + 0x48) & 0x38) >> 3;
}

/* Decrements `self+0x48`'s bits 3-5 sub-state by one, if it isn't
 * already zero - same shape as `sub_8010A0C` for the neighboring
 * bit-field. */
void sub_8010A5C(void *selfArg)
{
    u8 *self = selfArg;
    u8 state = (u8)sub_8010A50(self);

    if (state != 0) {
        u8 newState = (u8)(state - 1);
        *(u32 *)(self + 0x48) = (*(u32 *)(self + 0x48) & 0xc7) | (newState << 3);
    }
}

/* Setter for `self+0x48` bits 3-5. */
void sub_8010A84(void *selfArg, u32 state)
{
    u8 *self = selfArg;
    u8 s = (u8)state;
    *(u32 *)(self + 0x48) = (*(u32 *)(self + 0x48) & 0xc7) | (s << 3);
}

/* Setter for `self+0x48` bits 0-2. */
void sub_8010A94(void *selfArg, u32 state)
{
    u8 *self = selfArg;
    u8 s = (u8)state;
    *(u32 *)(self + 0x48) = (*(u32 *)(self + 0x48) & 0xf8) | s;
}

/* Getter for `self+0x48` bits 0-2. */
u32 sub_8010AA4(void *selfArg)
{
    u8 *self = selfArg;
    return *(u32 *)(self + 0x48) & 7;
}

void sub_8010AAC(void *selfArg, u8 val)
{
    u8 *self = selfArg;
    self[0x4e] = val;
}

u8 sub_8010AB4(void *selfArg)
{
    u8 *self = selfArg;
    return self[0x4e];
}

void sub_8010ABC(void *selfArg, s32 val)
{
    *(s32 *)((u8 *)selfArg + 0x44) = val;
}

s32 sub_8010AC0(void *selfArg)
{
    return *(s32 *)((u8 *)selfArg + 0x44);
}

/* Overwrites `self+0x4d`'s low 7 bits with `val`, preserving bit 7. */
void sub_8010AC4(void *selfArg, u32 val)
{
    u8 *self = selfArg;
    u8 v = (u8)val;
    u8 *p = self + 0x4d;
    u32 mask = 0x80;
    *p = v | (mask & *p);
}

u32 sub_8010AD8(void *selfArg)
{
    /* Register-pinned: the ROM copies `self` into r1 before advancing
     * it, keeping r0 free for the mask constant - a plain `self[0x4d] &
     * 0x7f` lets this compiler reuse r0 as the address register
     * instead, dropping the ROM's own `adds r1, r0, #0` copy. */
    register u8 *p asm("r1") = (u8 *)selfArg + 0x4d;
    register u32 mask asm("r0") = 0x7f;
    register u8 v asm("r1") = *p;
    return mask & v;
}

void sub_8010AE4(void *selfArg, u8 val)
{
    u8 *self = selfArg;
    self[0x4c] = val;
}

/* Sign-extending byte getter. */
s32 sub_8010AEC(void *selfArg)
{
    u8 *self = selfArg;
    return (s8)self[0x4c];
}
/* Trailing byte-padding mismatch fix: the function body isn't a
 * multiple of 4 bytes, and the ROM immediately continues with the
 * unlabeled `sub_8010AF8` right below - see matching_decomp_alignment_fix
 * memory. */
asm(".align 2, 0");

/* The original disassembly never gave this one its own label/symbol -
 * it sits directly after `sub_8010AEC`'s padding, at the address the
 * `bx lr`/alignment arithmetic works out to. Boolean getter for
 * `self+0x4d` bit 7. */
u32 sub_8010AF8(void *selfArg)
{
    register u8 *p asm("r0") = (u8 *)selfArg + 0x4d;
    register u32 mask asm("r1") = 0x80;
    register u8 v asm("r0") = *p;
    mask &= v;
    if (mask != 0) {
        return 1;
    }
    return 0;
}

/* Sets `self+0x4d` bit 7 and the global "hit" latch
 * `gUnknown_030012D8+0x80`. */
void sub_8010B0C(void *selfArg)
{
    u8 *p = (u8 *)selfArg + 0x4d;
    u32 mask = 0x80;
    u8 v = mask | *p;
    u8 *g;
    u32 one;
    *p = v;
    g = (u8 *)gUnknown_030012D8;
    one = 1;
    g = g + 0x80;
    *g = one;
}

/* Clears `self+0x4d` bit 7 and the global "hit" latch
 * `gUnknown_030012D8+0x80`. */
void sub_8010B28(void *selfArg)
{
    u8 *p = (u8 *)selfArg + 0x4d;
    u32 mask = 0x7f;
    u8 v = mask & *p;
    u32 zero = 0;
    *p = v;
    *((u8 *)gUnknown_030012D8 + 0x80) = zero;
}

void sub_8010B44(void *selfArg, u8 val)
{
    u8 *self = selfArg;
    self[0x58] = val;
}

u8 sub_8010B4C(void *selfArg)
{
    u8 *self = selfArg;
    return self[0x51];
}

u8 sub_8010B54(void *selfArg)
{
    u8 *self = selfArg;
    return self[0x50];
}

/* Overwrites the whole `self+0x48` field with a zero-extended byte
 * (unlike `sub_8010A94`, which masks - this one clobbers all bits). */
void sub_8010B5C(void *selfArg, u32 val)
{
    u8 *self = selfArg;
    *(u32 *)(self + 0x48) = (u8)val;
}

void sub_8010B64(void *selfArg, s32 val)
{
    u8 *self = selfArg;
    *(s32 *)(self + 0x54) = val;
}

s32 sub_8010B68(void *selfArg)
{
    u8 *self = selfArg;
    return *(s32 *)(self + 0x54);
}
