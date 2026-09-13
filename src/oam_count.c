#include "core.h"

/* A small per-category threshold table: sub_8006864/sub_8006820/
 * sub_80067EC each count how many of a caller's 20 records fall between
 * two adjacent thresholds here (threshold_08/_0C for one function,
 * threshold_0C/_10 for the next, and just threshold_10 alone for the
 * simplest one) - looks like nested difficulty/category boundaries.
 * sub_8006864/sub_8006820 read through inline asm rather than plain
 * struct field access on purpose: gcc's CSE otherwise shares the
 * "table[i]" address between the two threshold reads even though the
 * ROM recomputes it fresh for each one (see docs/graphics.md, "Matching
 * decompilation"). */
struct threshold_table_entry {
    u8 unused_00[8];
    u32 threshold_08;
    u32 threshold_0C;
    u32 threshold_10;
    u8 unused_14[0x24 - 0x14];
};
COMPILE_TIME_ASSERT(sizeof(struct threshold_table_entry) == 0x24);

extern struct threshold_table_entry gStaticData_0816C86C[];
extern void sub_80062A8(s32 arg0, s32 arg1, s32 arg2);
extern s32 sub_803AD80(void *arg0, s32 arg1, void *arg2);
extern void sub_8026ED0(void *arg0);
extern void sub_80006A8(void *arg0);
extern void sub_8006DC8(void *arg0);
extern void sub_8006AAC(void *arg0);
extern void sub_8006A90(void *arg0);
extern void sub_8006A48(void *arg0);
extern void FlushVramDmaQueue(void);
extern void *gUnknown_030012B8;
extern void *gUnknown_03001300;

extern void sub_8008044(void *arg0);

/* Shared by sub_8006600/sub_8006700/sub_8006714/sub_8006770 below - all
 * four access field_18 (and sub_8006600 also field_10/field_14) at the
 * same offsets on what looks like the same "actor" object. */
struct sub_8006700_actor {
    u8 unused_00[0x10];
    s32 field_10;
    void *field_14;
    void *field_18;
    u32 field_1c;
    u32 field_20;
    u8 field_24;
    u8 unused_25[3];
    u16 field_28;
};

extern void sub_8006C28(void *arg0);
extern void sub_8008890(void *arg0, s32 arg1, s32 arg2);
extern void sub_803AFE4(void *buf, s32 arg1, s32 arg2);
extern void sub_803AFDC(void *buf, s32 arg1, s32 arg2);
extern void sub_8001214(void *arg0, void *arg1, void *buf, s32 arg3);
extern s32 sub_8026F38(s32 arg0);
extern void *gUnknown_030012FC;

/* field_10/field_14 and field_20/field_24 are each an (offset, pointer)
 * pair sub_8006600 feeds straight to sub_803AD80 - two OAM slots one
 * icon spans (a wide sprite needs two entries side by side, most
 * likely). */
struct icon_record {
    u8 unused_00[0x10];
    s16 field_10;
    u8 unused_12[2];
    void *field_14;
    u8 unused_18[8];
    s16 field_20;
    u8 unused_22[2];
    void *field_24;
};

/* An OAM "icon" positioner: screen X/Y for the icon, then a pointer to
 * a small record describing which two OAM slots to draw it into.
 * gUnknown_030012E0/gUnknown_030012DC are two instances of this, used for
 * a left/right icon pair flanking a number in sub_8006600. */
struct icon_manager {
    u8 unused_00[0x110];
    u32 posX;
    u32 posY;
    u8 unused_118[0x130 - 0x118];
    struct icon_record *record;
};

extern struct icon_manager *gUnknown_030012E0;
extern struct icon_manager *gUnknown_030012DC;

#if NON_MATCHING
/* Positions two OAM icons flanking a number (drawn via sub_8001214 in
 * between) - centers each icon horizontally from its rendered pixel
 * width (`sub_803AD80`'s return value), at fixed Y coordinates. NOT YET
 * BYTE-MATCHING: matches the ROM's total instruction/byte count exactly
 * and most register choices, but a handful of low-register (r0-r7)
 * letter picks in the two STORE_TWO_FIELDS blocks still differ from the
 * ROM, and forcing them needs an r7 pin that a fresh test proved is a
 * genuine ABI hazard (see docs/graphics.md, "Parked, not matched:
 * sub_8006600", and matching_decomp_register_pinning memory) - so this
 * is compiled only under NON_MATCHING, with the checked-in matching
 * assembly (asm/code_3_1.s) used otherwise. The two STORE_TWO_FIELDS/
 * GET_RECORD asm blocks anchor address computations gcc would otherwise
 * cache across the sub_803AD80 calls in between, which the ROM does not
 * do. mgrAddrCache/mgr1Base/recOff/g1300Addr are pinned to match the
 * ROM's own register choices for values that must survive those same
 * calls. */
#define SUB_8006600_STORE_TWO_FIELDS(base, halved, yconst) \
    do { \
        register s32 _hv asm("r3") = (halved); \
        register s32 _yv asm("r2") = (yconst); \
        void *_addr; \
        asm volatile("mov %0, #0x88\n\tlsl %0, %0, #1" : "=&r"(_addr)); \
        *(u32 *)((u8 *)(base) + (s32)_addr) = _hv; \
        asm volatile("mov %0, #0x8a\n\tlsl %0, %0, #1" : "+r"(_hv)); \
        *(u32 *)((u8 *)(base) + (s32)_hv) = _yv; \
    } while (0)

#define SUB_8006600_GET_RECORD(base, recOff, out) \
    do { \
        void *_addr; \
        asm volatile("add %0, %1, %2" : "=&r"(_addr) : "r"(base), "r"(recOff)); \
        (out) = *(struct icon_record **)_addr; \
    } while (0)

void sub_8006600(struct sub_8006700_actor *arg0)
{
    register struct sub_8006700_actor *self asm("r4");
    register s32 recOff asm("r5");
    register void *mgrAddrCache asm("r8");
    register void *mgr1Base asm("r0");
    register void **g1300Addr asm("r9");
    void *addr;
    struct icon_record *record;
    u8 buf[0x10];
    u32 width;
    u32 halved;
    s32 charWidth;

    self = arg0;
    g1300Addr = &gUnknown_03001300;
    sub_8006A90(*g1300Addr);
    sub_8006C28(gUnknown_030012FC);
    sub_8008890(self->field_18, 0, 0);

    addr = &gUnknown_030012E0;
    mgrAddrCache = addr;
    mgr1Base = *(void **)addr;
    recOff = 0x98 << 1; /* offsetof(struct icon_manager, record) */
    SUB_8006600_GET_RECORD(mgr1Base, recOff, record);
    width = sub_803AD80((u8 *)mgr1Base + record->field_10,
                         self->field_10, record->field_14);
    halved = (0xF0 - width) >> 1;
    mgr1Base = *(void **)mgrAddrCache;
    SUB_8006600_STORE_TWO_FIELDS(mgr1Base, halved, 0x2D);
    SUB_8006600_GET_RECORD(mgr1Base, recOff, record);
    sub_803AD80((u8 *)mgr1Base + record->field_20,
                self->field_10, record->field_24);

    sub_803AFE4(buf, 0x10, 0x6a);
    sub_803AFDC(buf, 0xd0, 0x35);
    sub_8001214(self->field_14, gUnknown_030012DC, buf, 0);
    charWidth = sub_8026F38(0x2e);
    self = (struct sub_8006700_actor *)&gUnknown_030012DC;

    mgr1Base = *(void **)self;
    SUB_8006600_GET_RECORD(mgr1Base, recOff, record);
    width = sub_803AD80((u8 *)mgr1Base + record->field_10,
                         charWidth, record->field_14);
    halved = (0xF0 - width) >> 1;
    mgr1Base = *(void **)self;
    SUB_8006600_STORE_TWO_FIELDS(mgr1Base, halved, 0x90);
    SUB_8006600_GET_RECORD(mgr1Base, recOff, record);
    sub_803AD80((u8 *)mgr1Base + record->field_20,
                charWidth, record->field_24);

    self = (struct sub_8006700_actor *)g1300Addr;
    mgr1Base = *(void **)self;
    sub_8006A48(mgr1Base);
}
#endif /* NON_MATCHING */

void sub_8006700(struct sub_8006700_actor *arg0)
{
    arg0->field_1c++;
    sub_8008044(arg0->field_18);
}

void sub_8006714(struct sub_8006700_actor *arg0)
{
    sub_80006A8(arg0);
    sub_8006DC8(gUnknown_030012B8);
    sub_8006AAC(gUnknown_03001300);
    FlushVramDmaQueue();
    *(vu16 *)REG_ADDR_BG0HOFS = arg0->field_1c >> 3;
    *(vu16 *)PLTT = 0;
    *(vu32 *)REG_ADDR_BLDCNT = arg0->field_20;
    *(vu16 *)REG_ADDR_BLDY = (u32)(arg0->field_24 << 27) >> 27;
    *(vu16 *)REG_ADDR_DISPCNT = arg0->field_28;
}

void sub_8006770(struct sub_8006700_actor *arg0, u32 arg1)
{
    void *field18;
    u8 *p;

    field18 = arg0->field_18;
    if (field18 != NULL) {
        p = *(u8 **)((u8 *)field18 + 0x18) + 0x50;
        sub_803AD80((u8 *)field18 + *(s16 *)p, 3, *(void **)(p + 4));
    }
    if (arg1 & 1) {
        sub_8026ED0(arg0);
    }
}

void sub_80067A4(void)
{
    sub_80062A8(0x3F, 0x43, 1);
}

void sub_80067B4(void)
{
    sub_80062A8(0x3E, 0x42, 0);
}

void sub_80067C4(void)
{
    sub_80062A8(0x3D, 0x41, 2);
}

void sub_80067D4(void)
{
    sub_80062A8(0x3C, 0x40, 3);
}

u8 sub_80067E4(void *arg0)
{
    return (u32)(*(u8 *)arg0 << 25) >> 25;
}

/* Counts records whose derived value is <= threshold_10 alone (no lower
 * bound) - the simplest of the three sub_8006820/sub_8006864/sub_80067EC
 * threshold checks. */
s32 sub_80067EC(void *arg0)
{
    s32 count;
    u8 *p;
    u8 *bound;
    u8 *base;
    s32 i;
    u16 raw;
    s32 val;

    count = 0;
    base = (u8 *)gStaticData_0816C86C;
    bound = base + 0x10; /* &gStaticData_0816C86C[0].threshold_10 */
    p = (u8 *)arg0;
    i = 0x13;
    do {
        raw = *(u16 *)(p + 4);
        val = raw >> 3;
        if (val != 0) {
            if (val <= *(u32 *)bound) {
                count++;
            }
        }
        bound += 0x24;
        p += 4;
        i--;
    } while (i >= 0);
    return count;
}

/* Counts records whose derived value falls in (threshold_10, threshold_0C]
 * of the matching gStaticData_0816C86C entry - see the comment on
 * struct threshold_table_entry above for why this reads through inline
 * asm instead of entry->threshold_0C/entry->threshold_10. */
s32 sub_8006820(void *arg0)
{
    register u8 *p asm("r3");
    register s32 i asm("r5");
    register s32 count asm("r6");
    register s32 offset asm("r4");
    register s32 val asm("r1");
    register s32 addr asm("r0");
    register u16 raw asm("r0");

    count = 0;
    offset = 0;
    p = (u8 *)arg0;
    i = 0x13;
    do {
        raw = *(u16 *)(p + 4);
        val = raw >> 3;
        if (val != 0) {
            asm volatile("add %0, %1, #0\n\tadd %0, %0, #0xc\n\tadd %0, %2, %0" : "=r"(addr) : "r"(gStaticData_0816C86C), "r"(offset));
            if (val <= *(u32 *)addr) {
                asm volatile("add %0, %1, #0\n\tadd %0, %0, #0x10\n\tadd %0, %2, %0" : "=r"(addr) : "r"(gStaticData_0816C86C), "r"(offset));
                if (val > *(u32 *)addr) {
                    count++;
                }
            }
        }
        offset += 0x24;
        p += 4;
        i--;
    } while (i >= 0);
    return count;
}

/* Same shape as sub_8006820 above, one threshold pair up:
 * (threshold_0C, threshold_08]. */
s32 sub_8006864(void *arg0)
{
    register u8 *p asm("r3");
    register s32 i asm("r5");
    register s32 count asm("r6");
    register s32 offset asm("r4");
    register s32 val asm("r1");
    register s32 addr asm("r0");
    register u16 raw asm("r0");

    count = 0;
    offset = 0;
    p = (u8 *)arg0;
    i = 0x13;
    do {
        raw = *(u16 *)(p + 4);
        val = raw >> 3;
        if (val != 0) {
            asm volatile("add %0, %1, #0\n\tadd %0, %0, #8\n\tadd %0, %2, %0" : "=r"(addr) : "r"(gStaticData_0816C86C), "r"(offset));
            if (val <= *(u32 *)addr) {
                asm volatile("add %0, %1, #0\n\tadd %0, %0, #0xc\n\tadd %0, %2, %0" : "=r"(addr) : "r"(gStaticData_0816C86C), "r"(offset));
                if (val > *(u32 *)addr) {
                    count++;
                }
            }
        }
        offset += 0x24;
        p += 4;
        i--;
    } while (i >= 0);
    return count;
}

extern s32 sub_8006820(void *arg0);
extern s32 sub_80067EC(void *arg0);

s32 sub_80068A8(void *arg0)
{
    s32 total;
    s32 b;
    s32 c;

    total = sub_8006864(arg0);
    b = sub_8006820(arg0);
    c = sub_80067EC(arg0);
    total += b;
    total += c;
    return total;
}

s32 sub_80068CC(void *arg0)
{
    register u8 *p asm("r2");
    register s32 total asm("r4");
    register s32 i asm("r3");
    s32 result;
    u8 flags;
    u8 byte;

    total = 0;
    p = (u8 *)arg0;
    i = 0x13;
    do {
        byte = p[4];
        total += (((u32)byte << 30) >> 31) + (((u32)byte << 29) >> 31);
        p += 4;
        i--;
    } while (i >= 0);
    total += (((u32)*((u8 *)arg0 + 0x64) << 30) >> 31) + (((u32)*((u8 *)arg0 + 0x64) << 29) >> 31);
    flags = *((u8 *)arg0 + 2);
    result = total + (((u32)flags << 31) >> 31);
    result += ((u32)flags << 29) >> 31;
    result += ((u32)flags << 28) >> 31;
    result += ((u32)flags << 30) >> 31;
    return result;
}

s32 sub_8006920(void *arg0)
{
    u8 *p;
    s32 total;
    s32 i;
    u8 byte;

    total = 0;
    p = (u8 *)arg0;
    i = 0x13;
    do {
        byte = p[4];
        total += (((u32)byte << 30) >> 31) + (((u32)byte << 29) >> 31);
        p += 4;
        i--;
    } while (i >= 0);
    byte = *((u8 *)arg0 + 0x64);
    total += (((u32)byte << 30) >> 31) + (((u32)byte << 29) >> 31);
    return total;
}

s32 sub_800695C(void *arg0)
{
    register u8 *p asm("r1");
    register s32 i asm("r2");
    register s32 count asm("r3");
    register u8 byte asm("r4");
    register u32 bit asm("r0");

    count = 0;
    p = (u8 *)arg0;
    i = 0x13;
    do {
        byte = p[4];
        bit = (u32)byte << 31;
        bit >>= 31;
        count += bit;
        p += 4;
        i--;
    } while (i >= 0);
    return count;
}
