#include "core.h"
#include "actor_self.h"
#include "actor_anim.h"

/* Branchless absolute value - see actor_part39.c's copy of this macro
 * for the full explanation. */
#define ABS32(x, sign) do { (sign) = (x) >> 0x1f; (x) ^= (sign); (x) -= (sign); } while (0)

/* Same "self" object family as actor_part39.c - see that file's header
 * comment and docs/matching/issue-50-actor-2a69c.md. Non-adjacent to
 * actor_part39.c since the parked `DrawActor`
 * (actor_part44.c) sits raw between them. */

extern s32 sub_8029B2C(void);
extern s32 sub_8029E40(void);

/* Same movement-threshold computation as `InitActorPart`/`UpdateActor`
 * (see this file's header comment), but with no trampoline-fire/frame-
 * update tail - just refreshes `depth`/`sortKey`. */
void sub_802A980(struct actor_self *self)
{
    register s32 value asm("r2") = self->z - (sub_8029B2C() << 8);
    s32 sign;

    ABS32(value, sign);
    self->depth = value;
    value = (value >> 1) & 0x7f80;

    {
        s32 c = self->y;
        s32 cSign;
        s32 b, bSign;

        ABS32(c, cSign);
        b = self->x;
        ABS32(b, bSign);
        c = c + b;
        c >>= 0xb;
        c &= 0x7f;
        value |= c;
    }

    self->sortKey = value;

    if (self->depth > sub_8029E40()) {
        self->sortKey |= 0x8000;
    }
}

/* Trivial getter: the `index` of `self`'s animation record (`+0x30`,
 * InitActorPart's `part`), read as a byte. */
u8 sub_802A9D4(struct actor_self *self)
{
    return *(u8 *)&ACTOR_RECORD(self)->index;
}

/* State/table-index/anim-frame reset, the same idiom already documented
 * for the boss cluster's `sub_8030530`/`sub_8030C98` (see
 * docs/matching/issue-58-0x08030334-actor.md): sets `self+0x28`/
 * `self+0xc` from its own arguments, resets the frame counter
 * (`+0x44`)/accumulator (`+8`), and seeds the anim-frame halfword/byte
 * pair (`+0x10`/`+0x12`) from `self`'s part-table's `kind`th record. The
 * `*(T *)&self->...` stores keep gcc from treating them as struct-member
 * accesses, which changes where the byte zero is built. */
void sub_802A9DC(struct actor_self *self, s32 a, s32 kind)
{
    register s32 zero asm("r4");

    self->state = a;
    zero = 0;
    self->stateTime = zero;
    self->animIndex = kind;
    {
        register struct anim_frame_record *table asm("r3") = self->anims;
        register u16 anim asm("r1") = table[kind].duration;
        register u8 zero2 asm("r2") = 0;

        *(u16 *)&self->animTimer = anim;
        *(u8 *)&self->animDone = zero2;
    }
    self->animTime = zero;
}

/* Trivial getter: `self->z` (the constructor's `d` argument). */
s32 sub_802AA00(struct actor_self *self)
{
    return self->z;
}

/* Trivial getter: `self->y` (the constructor's `c` argument). */
s32 sub_802AA04(struct actor_self *self)
{
    return self->y;
}

/* Trivial getter: `self->x` (the constructor's `b` argument). */
s32 sub_802AA08(struct actor_self *self)
{
    return self->x;
}

asm(".align 2, 0");
