/* The BIOS calls of agb_syscall.h, in C (the GBA's are the SWI wrappers
 * of platform/gba/lib/libagbsyscall). DivMod, the game's inline `svc #6`,
 * is in the pc platform's bios_util.cpp (platform/pc/units/).
 *
 * VBlankIntrWait is a stub: the frame loop that raises VBlank and runs
 * the game's interrupt handlers is phase 2 of #1093. */

#include <math.h>
#include <string.h>

#include "gba/gba.h"
#include <agb_syscall.h>

void CpuSet(const void *src, void *dst, u32 control)
{
    u32 count = control & CPU_SET_COUNT_MASK;
    u32 unit = (control & CPU_SET_32BIT) ? 4 : 2;
    const u8 *s = (const u8 *)src;
    u8 *d = (u8 *)dst;
    u32 i;

    for (i = 0; i < count; i++) {
        memcpy(d, s, unit);
        if (!(control & CPU_SET_SRC_FIXED)) {
            s += unit;
        }
        d += unit;
    }
}

void CpuFastSet(const void *src, void *dst, u32 control)
{
    /* Words, in blocks of 8. */
    u32 count = ((control & CPU_SET_COUNT_MASK) + 7) & ~7u;
    const u8 *s = (const u8 *)src;
    u8 *d = (u8 *)dst;
    u32 i;

    for (i = 0; i < count; i++) {
        memcpy(d, s, 4);
        if (!(control & CPU_SET_SRC_FIXED)) {
            s += 4;
        }
        d += 4;
    }
}

/* The header both decompressors start with: the type, and the size of
 * the data in its top 24 bits. */
static u32 UncompressedSize(const u8 *src)
{
    return (u32)src[1] | ((u32)src[2] << 8) | ((u32)src[3] << 16);
}

void LZ77UnCompVram(const void *src, void *dst)
{
    const u8 *s = (const u8 *)src;
    u8 *d = (u8 *)dst;
    u32 size = UncompressedSize(s);
    u32 done = 0;

    s += 4;
    while (done < size) {
        u8 flags = *s++;
        s32 bit;

        for (bit = 7; bit >= 0 && done < size; bit--) {
            if (flags & (1 << bit)) {
                u32 length = (s[0] >> 4) + 3;
                u32 disp = (((u32)(s[0] & 0xF) << 8) | s[1]) + 1;

                s += 2;
                while (length-- != 0 && done < size) {
                    d[done] = d[done - disp];
                    done++;
                }
            } else {
                d[done++] = *s++;
            }
        }
    }
}

void RLUnCompVram(const void *src, void *dst)
{
    const u8 *s = (const u8 *)src;
    u8 *d = (u8 *)dst;
    u32 size = UncompressedSize(s);
    u32 done = 0;

    s += 4;
    while (done < size) {
        u8 flag = *s++;
        u32 length;

        if (flag & 0x80) {
            u8 value = *s++;

            for (length = (flag & 0x7F) + 3; length != 0 && done < size; length--) {
                d[done++] = value;
            }
        } else {
            for (length = (flag & 0x7F) + 1; length != 0 && done < size; length--) {
                d[done++] = *s++;
            }
        }
    }
}

/* sin and cos of the BIOS's angle (0x10000 a turn; it uses the top 8
 * bits) in 1.14 fixed point. */
static s32 Sin14(u16 angle)
{
    return (s32)lround(sin((angle >> 8) * (2.0 * M_PI / 256.0)) * 0x4000);
}

static s32 Cos14(u16 angle)
{
    return (s32)lround(cos((angle >> 8) * (2.0 * M_PI / 256.0)) * 0x4000);
}

/* BgAffineSet's destination record (the SDK's BgAffineDstData). */
struct bg_affine_dst {
    s16 pa;
    s16 pb;
    s16 pc;
    s16 pd;
    s32 x;
    s32 y;
};

void BgAffineSet(void *src, void *dst, s32 count)
{
    struct bg_affine_src *s = (struct bg_affine_src *)src;
    struct bg_affine_dst *d = (struct bg_affine_dst *)dst;

    for (; count > 0; count--, s++, d++) {
        s32 sn = Sin14(s->angle);
        s32 cs = Cos14(s->angle);
        s32 pa = (s->sx * cs) >> 14;
        s32 pb = -((s->sx * sn) >> 14);
        s32 pc = (s->sy * sn) >> 14;
        s32 pd = (s->sy * cs) >> 14;

        d->pa = (s16)pa;
        d->pb = (s16)pb;
        d->pc = (s16)pc;
        d->pd = (s16)pd;
        d->x = s->texX - (pa * s->scrX + pb * s->scrY);
        d->y = s->texY - (pc * s->scrX + pd * s->scrY);
    }
}

void ObjAffineSet(void *src, void *dst, s32 count, s32 offset)
{
    struct obj_affine_src *s = (struct obj_affine_src *)src;
    u8 *d = (u8 *)dst;

    for (; count > 0; count--, s++) {
        s32 sn = Sin14(s->angle);
        s32 cs = Cos14(s->angle);
        s16 params[4];
        s32 i;

        params[0] = (s16)((s->sx * cs) >> 14);
        params[1] = (s16)-((s->sx * sn) >> 14);
        params[2] = (s16)((s->sy * sn) >> 14);
        params[3] = (s16)((s->sy * cs) >> 14);
        for (i = 0; i < 4; i++) {
            memcpy(d, &params[i], sizeof(s16));
            d += offset;
        }
    }
}

s32 Sqrt(s32 num)
{
    u32 n = (u32)num;
    u32 root = 0;
    u32 bit = 1u << 30;

    while (bit > n) {
        bit >>= 2;
    }
    while (bit != 0) {
        if (n >= root + bit) {
            n -= root + bit;
            root = (root >> 1) + bit;
        } else {
            root >>= 1;
        }
        bit >>= 2;
    }
    return (s32)root;
}

void VBlankIntrWait(void)
{
}
