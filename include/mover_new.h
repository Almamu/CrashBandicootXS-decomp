#ifndef GUARD_MOVER_NEW_H
#define GUARD_MOVER_NEW_H

/* Constructing a `struct mover` (include/gobj_1a794.h) through
 * sub_801B7D8. Shared by include/gobj_1a794.h (sub_801A878) and
 * src/graphics/actor_part_188d0.c (sub_801961C, which base-constructs its
 * own mover subclass through it). */

struct mover;

struct mover *sub_801B7D8(struct mover *self, s32 distX, s32 distY, u32 dirX, u8 dirY, s32 kind);

/* sub_801B7D8's 5th/6th arguments are passed on the stack, the 5th as a
 * genuine byte (`strb`); both agbcc and old_agbcc widen a stack-passed
 * argument to a word `str`, so the caller writes both slots itself into
 * `args` (the only thing in its frame, i.e. at sp+0/sp+4 - exactly the
 * outgoing-argument area) and calls through a 4-argument view. The
 * stores go through `volatile` so they stay put. */
struct mover_stack_args
{
    u8 dirY;
    u8 unk_1[3];
    s32 kind;
};

typedef struct mover *(*MoverCtor4)(void *mem, s32 distX, s32 distY, u32 dirX);
#define MOVER_NEW(mem, dX, dY, fX) ((MoverCtor4)sub_801B7D8)((mem), (dX), (dY), (fX))

#endif // GUARD_MOVER_NEW_H
