#include "core.h"

/* Branchless absolute value - see actor_part39.c's copy of this macro
 * for the full explanation. */
#define ABS32(x, sign) do { (sign) = (x) >> 0x1f; (x) ^= (sign); (x) -= (sign); } while (0)

/* Same "self" object family as actor_part39.c - see that file's header
 * comment and docs/matching/issue-50-actor-2a69c.md. Non-adjacent to
 * actor_part39.c since the parked `UpdateAnimatedActorPart`
 * (actor_part44.c) sits raw between them. */

extern s32 sub_8029B2C(void);
extern s32 sub_8029E40(void);

/* Same movement-threshold computation as `InitActorPart`/`sub_802A7B8`
 * (see this file's header comment), but with no trampoline-fire/frame-
 * update tail - just refreshes `self+0x34`/`self+0x14`. */
void sub_802A980(void *selfArg)
{
    u8 *self = selfArg;
    register s32 value asm("r2") = *(s32 *)(self + 0x24) - (sub_8029B2C() << 8);
    s32 sign;

    ABS32(value, sign);
    *(s32 *)(self + 0x34) = value;
    value = (value >> 1) & 0x7f80;

    {
        s32 c = *(s32 *)(self + 0x20);
        s32 cSign;
        s32 b, bSign;

        ABS32(c, cSign);
        b = *(s32 *)(self + 0x1c);
        ABS32(b, bSign);
        c = c + b;
        c >>= 0xb;
        c &= 0x7f;
        value |= c;
    }

    *(s32 *)(self + 0x14) = value;

    if (*(s32 *)(self + 0x34) > sub_8029E40()) {
        *(s32 *)(self + 0x14) |= 0x8000;
    }
}

/* Trivial getter: the first byte of `self`'s part-table pointer
 * (`self+0x30`). */
u8 sub_802A9D4(void *selfArg)
{
    u8 *self = selfArg;

    return **(u8 **)(self + 0x30);
}

/* State/table-index/anim-frame reset, the same idiom already documented
 * for the boss cluster's `sub_8030530`/`sub_8030C98` (see
 * docs/matching/issue-58-0x08030334-actor.md): sets `self+0x28`/
 * `self+0xc` from its own arguments, resets the frame counter
 * (`+0x44`)/accumulator (`+8`), and seeds the anim-frame halfword/byte
 * pair (`+0x10`/`+0x12`) from `self`'s part-table's `kind`th record. */
void sub_802A9DC(void *selfArg, s32 a, s32 kind)
{
    u8 *self = selfArg;
    register s32 zero asm("r4");

    *(s32 *)(self + 0x28) = a;
    zero = 0;
    *(s32 *)(self + 0x44) = zero;
    *(s32 *)(self + 0xc) = kind;
    {
        register u8 *table asm("r3") = *(u8 **)self;
        register u16 anim asm("r1") = *(u16 *)(table + kind * 0xc);
        register u8 zero2 asm("r2") = 0;

        *(u16 *)(self + 0x10) = anim;
        self[0x12] = zero2;
    }
    *(s32 *)(self + 8) = zero;
}

/* Trivial getter: `self+0x24` (the constructor's `d` argument). */
s32 sub_802AA00(void *selfArg)
{
    return *(s32 *)((u8 *)selfArg + 0x24);
}

/* Trivial getter: `self+0x20` (the constructor's `c` argument). */
s32 sub_802AA04(void *selfArg)
{
    return *(s32 *)((u8 *)selfArg + 0x20);
}

/* Trivial getter: `self+0x1c` (the constructor's `b` argument). */
s32 sub_802AA08(void *selfArg)
{
    return *(s32 *)((u8 *)selfArg + 0x1c);
}

asm(".align 2, 0");
