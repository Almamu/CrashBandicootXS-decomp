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

extern void sub_803AFE4(void *buf, s32 arg1, s32 arg2);
extern void sub_803AFDC(void *buf, s32 arg1, s32 arg2);

/* Confirmed by matching sub_8007B00 below (even though that function
 * itself is parked, this shape isn't in doubt - every field read
 * compiled byte-identically to the ROM). */
struct aabb {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_c;
};

#if NON_MATCHING
/* Builds an AABB (via the shared sub_803AFE4/sub_803AFDC primitive -
 * see docs/matching.md) for `part`'s current animation keyframe: the
 * keyframe table pointer at `part+0x20` indexed by the counter at
 * `part+0x2d` (0x1c bytes per record), whose own `+0xc`/`+0xe`/`+0x10`/
 * `+0x11` fields are {s16 xOffset, s16 yOffset, u8 w, u8 h} - the same
 * offset/text-table convention seen elsewhere, just with the AABB
 * dimensions instead of a text pointer. `part+0x28` bits 4/5 mirror
 * the AABB horizontally/vertically around `part`'s own position.
 *
 * NOT YET BYTE-MATCHING, but extremely close: every single
 * instruction's operation and order matches the ROM exactly except
 * one - `part` lands in r6 here instead of the ROM's r7, cascading
 * into a 3- vs 4-register push/pop list. Tried pinning `part` to r7
 * directly (categorically unsafe in this toolchain - see
 * `matching_decomp_register_pinning` memory, point 10: r7 pins never
 * make it into the prologue's push list), pinning `dest` to r8 vs
 * leaving it unpinned (both tried, neither naturally shifts `part`
 * onto r7), and blocking r6 with a dummy pin to force the allocator
 * elsewhere (didn't compile as attempted). Parked rather than continue
 * chasing one specific register choice - same pattern as
 * `sub_80073DC`/`sub_8006600`/`sub_8000EE4`. */
void sub_8007B00(void *dest, void *part)
{
    register struct aabb *pDest asm("r8") = dest;
    struct aabb buf_;
    s32 *buf = (s32 *)&buf_;
    void *table;
    u8 idx;
    void *rec;
    s16 offX, offY;
    u8 w, h;
    s32 x, y;

    {
        void **tablePtr = *(void ***)((u8 *)part + 0x20);
        s32 offset;

        idx = *((u8 *)part + 0x2d);
        offset = idx * 0x1c;
        table = *tablePtr;
        rec = (u8 *)table + offset;
    }
    offX = *(s16 *)((u8 *)rec + 0xc);
    offY = *(s16 *)((u8 *)rec + 0xe);
    w = *((u8 *)rec + 0x10);
    h = *((u8 *)rec + 0x11);

    x = offX + (*(s32 *)part >> 8);
    y = offY + (*(s32 *)((u8 *)part + 4) >> 8);
    sub_803AFE4(buf, x, y);
    sub_803AFDC(buf, w, h);

    if ((*((u8 *)part + 0x28) >> 4) & 1) {
        buf[0] = (*(s32 *)part >> 8) * 2 - (buf[0] + buf[2]);
    }
    if ((*(vu8 *)((u8 *)part + 0x28) >> 5) & 1) {
        buf[1] = (*(s32 *)((u8 *)part + 4) >> 8) * 2 - (buf[1] + buf[3]);
    }

    *pDest = buf_;
}
#endif /* NON_MATCHING */
