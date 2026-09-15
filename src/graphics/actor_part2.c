#include "core.h"
#include "actor.h"

struct aabb {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_c;
};

extern void sub_803AFE4(void *buf, s32 arg1, s32 arg2);
extern void sub_803AFDC(void *buf, s32 arg1, s32 arg2);
extern void *sub_80083B8(void *part);
extern u8 gStaticData_0816B2F8[];

/* A third AABB-for-keyframe builder (see sub_8007B00/sub_8007B98 in
 * src/graphics/actor_part.c), this time selecting its 6-byte
 * `{s16 x, s16 y, u8 w, u8 h}` record via a `sub_80083B8(part)`-derived
 * "info" struct rather than `part`'s own keyframe table pointer:
 * `info+4` points to a byte whose upper nibble (0-15, but only 0-6
 * handled - anything above 6 and unhandled 1/2/6 fall through to the
 * same default) selects one of `info+0x14`, `info+0xc`, or the fixed
 * fallback table `gStaticData_0816B2F8`. */
void *sub_8007C30(void *dest, void *pt)
{
    register void *part asm("r6") = pt;
    struct aabb buf_;
    void *info;
    void *rec;
    s32 offX, offY;
    s32 w, h;
    s32 x, y;
    u8 type;

    info = sub_80083B8(part);
    type = *(u8 *)(*(void **)((u8 *)info + 4)) >> 4;
    switch (type) {
    case 0:
    case 3:
    case 4:
        rec = (u8 *)info + 0x14;
        break;
    case 1:
    case 2:
    case 6:
        rec = gStaticData_0816B2F8;
        break;
    case 5:
        rec = (u8 *)info + 0xc;
        break;
    default:
        rec = gStaticData_0816B2F8;
        break;
    }

    x = *(s32 *)part >> 8;
    offX = *(s16 *)((u8 *)rec + 0);
    y = *(s32 *)((u8 *)part + 4) >> 8;
    offY = *(s16 *)((u8 *)rec + 2);
    w = *((u8 *)rec + 4);
    h = *((u8 *)rec + 5);

    offX = offX + x;
    offY = offY + y;
    sub_803AFE4(&buf_, offX, offY);
    sub_803AFDC(&buf_, w, h);

    {
        u8 *flagsAddr = (u8 *)part + 0x28;
        register s32 flags asm("r1");
        register s32 shifted asm("r0");

        flags = *flagsAddr;
        shifted = flags << 27;
        if (shifted < 0) {
            buf_.field_0 = (*(s32 *)part >> 8) * 2 - (buf_.field_0 + buf_.field_8);
        }
        {
            register s32 addr asm("r3") = (s32)flagsAddr;
            asm("ldrb %1, [%1]\n\tlsl %0, %1, #0x1a" : "=r" (shifted), "+r" (addr));
        }
        if (shifted < 0) {
            buf_.field_4 = (*(s32 *)((u8 *)part + 4) >> 8) * 2 - (buf_.field_4 + buf_.field_c);
        }
    }

    *(struct aabb *)dest = buf_;
    return dest;
}

/* Same shape as sub_8007C30 above, with a simpler switch: only
 * `info+0xc` or the `gStaticData_0816B2F8` fallback are ever selected
 * (cases 0/2/3/4/6 to `info+0xc`; cases 1/5 and the out-of-range
 * default all to the fallback). */
void *sub_8007CF8(void *dest, void *pt)
{
    register void *part asm("r6") = pt;
    struct aabb buf_;
    void *info;
    void *rec;
    s32 offX, offY;
    s32 w, h;
    s32 x, y;
    u8 type;

    info = sub_80083B8(part);
    type = *(u8 *)(*(void **)((u8 *)info + 4)) >> 4;
    switch (type) {
    case 0:
    case 2:
    case 3:
    case 4:
    case 6:
        rec = (u8 *)info + 0xc;
        break;
    case 1:
    case 5:
        rec = gStaticData_0816B2F8;
        break;
    default:
        rec = gStaticData_0816B2F8;
        break;
    }

    x = *(s32 *)part >> 8;
    offX = *(s16 *)((u8 *)rec + 0);
    y = *(s32 *)((u8 *)part + 4) >> 8;
    offY = *(s16 *)((u8 *)rec + 2);
    w = *((u8 *)rec + 4);
    h = *((u8 *)rec + 5);

    offX = offX + x;
    offY = offY + y;
    sub_803AFE4(&buf_, offX, offY);
    sub_803AFDC(&buf_, w, h);

    {
        u8 *flagsAddr = (u8 *)part + 0x28;
        register s32 flags asm("r1");
        register s32 shifted asm("r0");

        flags = *flagsAddr;
        shifted = flags << 27;
        if (shifted < 0) {
            buf_.field_0 = (*(s32 *)part >> 8) * 2 - (buf_.field_0 + buf_.field_8);
        }
        {
            register s32 addr asm("r3") = (s32)flagsAddr;
            asm("ldrb %1, [%1]\n\tlsl %0, %1, #0x1a" : "=r" (shifted), "+r" (addr));
        }
        if (shifted < 0) {
            buf_.field_4 = (*(s32 *)((u8 *)part + 4) >> 8) * 2 - (buf_.field_4 + buf_.field_c);
        }
    }

    *(struct aabb *)dest = buf_;
    return dest;
}
