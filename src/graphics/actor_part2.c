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

extern void *sub_8007B98(void *dest, void *part);
extern u8 sub_8001688(void *buf1, void *buf2);
extern void sub_803AD88(void *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void *sub_8025BAC(void *pool, s32 arg1, s32 kind, s32 x, s32 y, s32 arg5);
extern struct actor *gUnknown_030012D8;
extern void *gUnknown_030012E4;
extern void *gUnknown_030012B4;

#if NON_MATCHING
/* `part` (a `struct actor`, same layout used throughout this ROM
 * region) collides with the player (`gUnknown_030012D8`, tested via
 * two `sub_8007B98` AABBs and `sub_8001688`) and, if so, plays a sound
 * at the player's position (the `table+0x68` offset/dead-read idiom
 * matches sub_8007048's `sub_803AD88` call exactly, just keyed off
 * `part->field_0A` instead of `self->field_0A`) and marks itself
 * "collected" (`gUnknown_030012B4` bitmap, same convention as
 * sub_80072D8). `part->field_0A - 0x1b` (0-7) then selects a "kind" to
 * spawn via `sub_8025BAC` at `part`'s own position - case 1 and any
 * out-of-range value spawn nothing. If something spawned, its
 * `+0x28`/`+0xc` flag bytes get tagged - kept as raw offsets since the
 * spawned object's own type isn't established yet.
 *
 * NOT YET BYTE-MATCHING: every instruction's operation, operand, and
 * order matches the ROM exactly except one systematic register choice
 * - the cached address of the `gUnknown_030012D8` global lands in r6
 * here instead of the ROM's r7 (same category of issue as
 * sub_8007B00's `part` above - explicit r7 pins are categorically
 * unsafe in this toolchain, confirmed again here: pinning `pGlobal` to
 * r7 crashes the compiler with an internal error instead of just
 * mis-scheduling). Because this register is read from repeatedly
 * across several basic blocks (the two collision-AABB calls, the
 * sound-position lookup), the single letter mismatch cascades through
 * nearly the entire rest of the function's register numbering, even
 * though every individual instruction's operation is identical.
 * Parked rather than keep chasing this one register - same call as
 * `sub_8007B00`/`sub_8007B98` above. */
void sub_8007DBC(struct actor *part)
{
    struct aabb buf1;
    struct aabb buf2;

    {
        register s32 shifted asm("r1");
        register s32 mask asm("r6");
        register s32 result asm("r0");

        asm volatile(
            "ldrb r0, [%3, #0xc]\n\t"
            "lsl r1, r0, #0x18\n\t"
            "lsr r0, r1, #0x1b\n\t"
            "mov r6, #1\n\t"
            "and r0, r0, r6"
            : "=r" (result), "=r" (shifted), "=r" (mask)
            : "r" (part));
        if (result) {
            return;
        }

        asm("lsr %0, %1, #0x1a\n\tand %0, %0, %2" : "=r" (result) : "r" (shifted), "r" (mask));
        if (!result) {
            return;
        }
    }

    sub_8007B98(&buf1, part);

    {
        struct actor **pGlobal = &gUnknown_030012D8;

        if (!((*pGlobal)->flags >> 7)) {
            return;
        }

        sub_8007B98(&buf2, *pGlobal);

        if (!sub_8001688(&buf2, &buf1)) {
            return;
        }

        {
            register s32 result asm("r0");
            register s32 tmp asm("r3");

            result = 8;
            tmp = part->flags;
            result |= tmp;
            part->flags = result;
        }

        {
            void *table2 = (u8 *)(*pGlobal)->table + 0x68;
            void *addr = (u8 *)(*pGlobal) + *(s16 *)table2;
            u8 field0a = part->field_0A;
            register void *deadRead asm("r4") = *(void *volatile *)((u8 *)table2 + 4);
            (void)deadRead;
            sub_803AD88(addr, 0, field0a, 0);
        }
    }

    {
        register s32 result asm("r0");
        register s32 tmp asm("r6");

        result = 1;
        tmp = part->flags;
        result |= tmp;
        part->flags = result;
    }

    if (part->field_08 != 0xFFFF) {
        register s32 word asm("r0");
        s32 wordOffset;

        asm volatile("add %0, %1, #0\n\tasr %0, %0, #5" : "=r" (word) : "r" ((s32)part->field_08));
        wordOffset = word << 2;
        {
            void *base = gUnknown_030012B4;
            s32 *bitmap = (s32 *)((u8 *)base + 0x108 + wordOffset);
            s32 bit = part->field_08 - (word << 5);
            *bitmap |= 1 << bit;
        }
    }

    {
        void *result = 0;
        u8 subType;

        subType = part->field_0A - 0x1b;
        switch (subType) {
        case 2:
        case 3:
            result = sub_8025BAC(gUnknown_030012E4, 0x2b, 1, part->x >> 8, part->y >> 8, 0);
            break;
        case 6:
            result = sub_8025BAC(gUnknown_030012E4, 0x2b, 6, part->x >> 8, part->y >> 8, 0);
            break;
        case 4:
            result = sub_8025BAC(gUnknown_030012E4, 0x2b, 5, part->x >> 8, part->y >> 8, 0);
            break;
        case 7:
            result = sub_8025BAC(gUnknown_030012E4, 0x2b, 0, part->x >> 8, part->y >> 8, 0);
            break;
        case 5:
            result = sub_8025BAC(gUnknown_030012E4, 0x2b, 3, part->x >> 8, part->y >> 8, 0);
            break;
        case 0:
            result = sub_8025BAC(gUnknown_030012E4, 0x2b, 4, part->x >> 8, part->y >> 8, 0);
            break;
        case 1:
        default:
            break;
        }

        if (result != 0) {
            register s32 r0 asm("r0");
            register s32 tmp asm("r4");

            tmp = 1;
            r0 = -4;
            tmp = *((u8 *)result + 0x28);
            r0 &= tmp;
            r0 |= 1;
            *((u8 *)result + 0x28) = r0;

            r0 = -5;
            tmp = *((u8 *)result + 0xc);
            r0 &= tmp;
            *((u8 *)result + 0xc) = r0;
        }
    }
}
#endif /* NON_MATCHING */
