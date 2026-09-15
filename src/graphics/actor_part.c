#include "core.h"
#include "actor.h"

extern void sub_8007174(void *arg0, s32 arg1, s32 arg2, s32 *arg3, s32 *arg4);
extern void sub_80073DC(void *unused, void *part, s32 *posPtr);
extern void sub_8026ED0(void *arg0);

/* `part+0x25` selects whether (x, y) are already screen-relative
 * (nonzero - used as-is) or need the camera-relative conversion
 * sub_8007174 applies (zero - the common case). Either way, the
 * resolved {x, y} pair is forwarded to sub_80073DC (parked as
 * NON_MATCHING in src/graphics/graphics.c) to build/queue this part's
 * OAM entries. */
void sub_8007A48(void *self, void *part, s32 x, s32 y)
{
    s32 pos[2];

    if (*((u8 *)part + 0x25) == 0) {
        sub_8007174(part, x, y, &pos[0], &pos[1]);
    } else {
        pos[0] = x;
        pos[1] = y;
    }
    sub_80073DC(self, part, pos);
}
asm(".align 2, 0");

/* `part`'s own leading {x, y} pair (the same Q8 fixed-point position
 * fields struct actor has at 0x00/0x04) becomes the explicit position
 * passed to sub_8007A48 - confirms `part` embeds a struct-actor-shaped
 * position at its own start. */
void sub_8007A84(void *self, void *part)
{
    sub_8007A48(self, part, *(s32 *)part >> 8, *(s32 *)((u8 *)part + 4) >> 8);
}
asm(".align 2, 0");

void sub_8007A98(void *arg0, u32 arg1)
{
    if (arg1 & 1) {
        sub_8026ED0(arg0);
    }
}
asm(".align 2, 0");

void nullsub_2(void)
{
}
asm(".align 2, 0");

/* Initializes/clears several `part`-object fields also seen used in
 * sub_80073DC/sub_8007634: 0x20/0x30/0x34 (position-interpolation
 * state), 0x28-0x29 (the flags byte pair packed into attr1/attr2),
 * 0x2d (keyframe counter), 0x3c (Q8 "scale" factor), and 0x25 (the
 * screen-vs-camera-relative flag sub_8007A48 tests). `part+0xd` is a
 * second, separate flags byte from `part+0xc`.
 *
 * Register pins throughout match the ROM's own choices for the two
 * bit-clear sequences (constant computed before the byte load, result
 * landing in the constant's own register - the same accumulator
 * pattern documented at length for `struct actor`'s flags field in
 * graphics.c) and for the final `0x2c` store (the ROM computes that
 * address into a *fresh* register rather than reusing `part`'s, even
 * though `part` is dead afterward - plain C let the allocator reuse
 * it instead). The running `p` pointer (advanced by `+8` then `+0xb`
 * rather than recomputed from `part` each time) and the shared `zero`
 * local (reused across differently-sized stores instead of
 * rematerializing the constant) both mirror the ROM's own address/
 * value reuse - see docs/matching.md, "Matching decompilation". */
void sub_8007AB4(void *arg0)
{
    register void *part asm("r3") = arg0;
    register s32 result asm("r0");
    register s32 tmp asm("r1");

    result = 0x7f;
    tmp = *((u8 *)part + 0xc);
    result &= tmp;
    tmp = -0x41;
    result &= tmp;
    *((u8 *)part + 0xc) = result;

    {
        u8 *p = (u8 *)part + 0x25;
        s32 zero = 0;
        *p = zero;
        *(s32 *)((u8 *)part + 0x20) = zero;
        p += 8;
        *p = zero;
        *(s32 *)((u8 *)part + 0x30) = zero;
        *(s32 *)((u8 *)part + 0x34) = zero;
        *(u16 *)((u8 *)part + 0x28) = zero;
        p += 0xb;
        *p = zero;
    }

    {
        register s32 result2 asm("r0");
        register s32 tmp2 asm("r4");

        result2 = -5;
        tmp2 = *((u8 *)part + 0xd);
        result2 &= tmp2;
        *((u8 *)part + 0xd) = result2;
    }

    *((u8 *)part + 0x24) = 0;
    *(u16 *)((u8 *)part + 0x3c) = 0;
    {
        register u8 *p2 asm("r1") = (u8 *)part + 0x2c;
        *p2 = 1;
    }
}
