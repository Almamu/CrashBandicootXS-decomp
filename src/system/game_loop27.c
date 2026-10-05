#include "core.h"
#include "phys_obj.h"

/* GitHub issue #14: 0x08010A0C-0x08010D54, continuing the physics/
 * collision subsystem (`game_loop17.c`-`game_loop26.c`, see
 * docs/matching/issue-13-graphics-fc70.md). `sub_8010A00` right before
 * this function is already matched in game_loop26.c; everything here
 * operates on the same `self` type `sub_8010A00`/`ResetCrate` do - the
 * viewport's own "collision box" sub-record embedded at
 * `gPlayer+0x108` (confirmed by `ResolvePlayerCollisions` in
 * game_loop23.c, which already calls `ResolveCollisionCandidates(gPlayer +
 * 0x108)`). Its fields are `struct crate`'s (include/phys_obj.h). */


/* Getter for `self+0x48` bits 6-7 - already matched, game_loop26.c. */
extern u32 sub_8010A00(void *selfArg);

/* Decrements `self+0x48`'s bits 6-7 sub-state by one, if it isn't
 * already zero. */
void sub_8010A0C(void *selfArg)
{
    struct crate *self = selfArg;
    u8 state = (u8)sub_8010A00(self);

    if (state != 0) {
        u8 newState = (u8)(state - 1);
        self->u48.n = (self->u48.n & 0x3f) | (newState << 6);
    }
}

/* Setter for `self+0x48` bits 6-7. */
void sub_8010A34(void *selfArg, u32 state)
{
    struct crate *self = selfArg;
    u8 s = (u8)state;
    self->u48.n = (self->u48.n & 0x3f) | (s << 6);
}

/* Clears `self+0x48` bits 6-7. */
void sub_8010A44(void *selfArg)
{
    struct crate *self = selfArg;
    u32 v = self->u48.n;
    v &= 0x3f;
    self->u48.n = v;
}

/* Getter for `self+0x48` bits 3-5. */
u32 sub_8010A50(void *selfArg)
{
    struct crate *self = selfArg;
    return ((u32)self->u48.n & 0x38) >> 3;
}

/* Decrements `self+0x48`'s bits 3-5 sub-state by one, if it isn't
 * already zero - same shape as `sub_8010A0C` for the neighboring
 * bit-field. */
void sub_8010A5C(void *selfArg)
{
    struct crate *self = selfArg;
    u8 state = (u8)sub_8010A50(self);

    if (state != 0) {
        u8 newState = (u8)(state - 1);
        self->u48.n = (self->u48.n & 0xc7) | (newState << 3);
    }
}

/* Setter for `self+0x48` bits 3-5. */
void sub_8010A84(void *selfArg, u32 state)
{
    struct crate *self = selfArg;
    u8 s = (u8)state;
    self->u48.n = (self->u48.n & 0xc7) | (s << 3);
}

/* Setter for `self+0x48` bits 0-2. */
void sub_8010A94(void *selfArg, u32 state)
{
    struct crate *self = selfArg;
    u8 s = (u8)state;
    self->u48.n = (self->u48.n & 0xf8) | s;
}

/* Getter for `self+0x48` bits 0-2. */
u32 sub_8010AA4(void *selfArg)
{
    struct crate *self = selfArg;
    return self->u48.n & 7;
}

void SetCrateKind(void *selfArg, u8 val)
{
    struct crate *self = selfArg;
    self->kind = val;
}

u8 GetCrateKind(void *selfArg)
{
    struct crate *self = selfArg;
    return self->kind;
}

void sub_8010ABC(void *selfArg, s32 val)
{
    ((struct crate *)selfArg)->unk_44 = val;
}

s32 sub_8010AC0(void *selfArg)
{
    return ((struct crate *)selfArg)->unk_44;
}

/* Overwrites `self+0x4d`'s low 7 bits with `val`, preserving bit 7. */
void SetCrateState(void *selfArg, u32 val)
{
    struct crate *self = selfArg;
    u8 v = (u8)val;
    u8 *p = &self->state;
    u32 mask = 0x80;
    *p = v | (mask & *p);
}

u32 GetCrateState(void *selfArg)
{
    /* Register-pinned: the ROM copies `self` into r1 before advancing
     * it, keeping r0 free for the mask constant - a plain `self[0x4d] &
     * 0x7f` lets this compiler reuse r0 as the address register
     * instead, dropping the ROM's own `adds r1, r0, #0` copy. */
    register u8 *p asm("r1") = &((struct crate *)selfArg)->state;
    register u32 mask asm("r0") = 0x7f;
    register u8 v asm("r1") = *p;
    return mask & v;
}

void sub_8010AE4(void *selfArg, u8 val)
{
    struct crate *self = selfArg;
    self->unk_4C = val;
}

/* Sign-extending byte getter. */
s32 sub_8010AEC(void *selfArg)
{
    struct crate *self = selfArg;
    return self->unk_4C;
}
/* Trailing byte-padding mismatch fix: the function body isn't a
 * multiple of 4 bytes, and the ROM immediately continues with the
 * unlabeled `IsCrateBusy` right below - see matching_decomp_alignment_fix
 * memory. */
asm(".align 2, 0");

/* The original disassembly never gave this one its own label/symbol -
 * it sits directly after `sub_8010AEC`'s padding, at the address the
 * `bx lr`/alignment arithmetic works out to. Boolean getter for
 * `self+0x4d` bit 7. */
u32 IsCrateBusy(void *selfArg)
{
    register u8 *p asm("r0") = &((struct crate *)selfArg)->state;
    register u32 mask asm("r1") = 0x80;
    register u8 v asm("r0") = *p;
    mask &= v;
    if (mask != 0) {
        return 1;
    }
    return 0;
}

/* Sets `self+0x4d` bit 7 and the global "hit" latch
 * `gPlayer+0x80`. */
void SetCrateBusy(void *selfArg)
{
    u8 *p = &((struct crate *)selfArg)->state;
    u32 mask = 0x80;
    u8 v = mask | *p;
    u8 *g;
    u32 one;
    *p = v;
    g = (u8 *)gPlayer;
    one = 1;
    g = g + 0x80;
    *g = one;
}

/* Clears `self+0x4d` bit 7 and the global "hit" latch
 * `gPlayer+0x80`. */
void ClearCrateBusy(void *selfArg)
{
    u8 *p = &((struct crate *)selfArg)->state;
    u32 mask = 0x7f;
    u8 v = mask & *p;
    u32 zero = 0;
    *p = v;
    *((u8 *)gPlayer + 0x80) = zero;
}

void SetCrateTouched(void *selfArg, u8 val)
{
    struct crate *self = selfArg;
    self->touched = val;
}

u8 sub_8010B4C(void *selfArg)
{
    struct crate *self = selfArg;
    return self->unk_51;
}

u8 sub_8010B54(void *selfArg)
{
    struct crate *self = selfArg;
    return self->unk_50;
}

/* Overwrites the whole `self+0x48` field with a zero-extended byte
 * (unlike `sub_8010A94`, which masks - this one clobbers all bits). */
void sub_8010B5C(void *selfArg, u32 val)
{
    struct crate *self = selfArg;
    self->u48.n = (u8)val;
}

void sub_8010B64(void *selfArg, s32 val)
{
    struct crate *self = selfArg;
    self->unk_54 = val;
}

s32 sub_8010B68(void *selfArg)
{
    struct crate *self = selfArg;
    return self->unk_54;
}
