#include "core.h"
#include "icon_manager.h"
#include "vram_pool.h"
#include "actor.h"

/* A small per-category threshold table: sub_8006864/sub_8006820/
 * sub_80067EC each count how many of a caller's 20 records fall between
 * two adjacent thresholds here (threshold_08/_0C for one function,
 * threshold_0C/_10 for the next, and just threshold_10 alone for the
 * simplest one) - looks like nested difficulty/category boundaries.
 * sub_8006864/sub_8006820 read through inline asm rather than plain
 * struct field access on purpose: gcc's CSE otherwise shares the
 * "table[i]" address between the two threshold reads even though the
 * ROM recomputes it fresh for each one (see docs/matching.md, "Matching
 * decompilation"). */
struct threshold_table_entry {
    u8 unused_00[8];
    u32 threshold_08;
    u32 threshold_0C;
    u32 threshold_10;
    u8 unused_14[0x24 - 0x14];
};
COMPILE_TIME_ASSERT(sizeof(struct threshold_table_entry) == 0x24);

extern struct threshold_table_entry gLevelTable[];
extern void sub_80062A8(s32 arg0, s32 arg1, s32 arg2);
extern s32 _call_via_r2(void *arg0, s32 arg1, void *arg2);
extern void sub_8026ED0(void *arg0);
extern void sub_8006DC8(struct tile_asset_cache *arg0);
extern void sub_8006AAC(void *arg0);
extern void sub_8006A90(void *arg0);
extern void sub_8006A48(void *arg0);
extern void FlushVramDmaQueue(void);
extern struct tile_asset_cache *gUnknown_030012B8;
extern void *gUnknown_03001300;

extern void sub_8008044(void *arg0);

/* Shared by sub_8006600/sub_8006700/sub_8006714/sub_8006770 below - all
 * four access field_18 (and sub_8006600 also field_10/field_14) at the
 * same offsets on what looks like the same "actor" object. */
struct sub_8006700_actor {
    u8 unused_00[0x10];
    s32 field_10;
    void *field_14;
    struct actor *field_18;
    u32 field_1c;
    u32 field_20;
    u8 field_24;
    u8 unused_25[3];
    u16 field_28;
};

extern void sub_8006C28(struct vram_upload_cursor *arg0);
extern void sub_8008890(void *arg0, s32 arg1, s32 arg2);
extern void sub_803AFE4(void *buf, s32 arg1, s32 arg2);
extern void sub_803AFDC(void *buf, s32 arg1, s32 arg2);
extern s32 sub_8001214(void *arg0, void *arg1, void *buf, s32 arg3);
extern s32 GetUiText(s32 arg0);
extern struct vram_upload_cursor *gUnknown_030012FC;

extern struct icon_manager *gUnknown_030012E0;
extern struct icon_manager *gUnknown_030012DC;

/* Sets an icon manager's draw position. Both coordinates are inline
 * arguments, so gcc evaluates them (and re-reads the manager global)
 * before either store - the ROM's order. */
static inline void set_icon_mgr_pos(struct icon_manager *m, u32 x, u32 y)
{
    m->posX = x;
    m->posY = y;
}

/* Positions two OAM icons flanking a number (drawn via sub_8001214 in
 * between) - centers each icon horizontally from its rendered pixel
 * width (the manager's `record->slots[0]` method, a gcc 2.x virtual call
 * through `_call_via_r2`), at fixed Y coordinates, then
 * draws it with `slots[2]`. Parked as NAKED for a long time over a
 * register-letter gap; closed by computing the centered X into its own
 * local before passing it to the inline setter (passing the expression
 * straight in swapped the X/Y and 0x130/240 registers) - see
 * docs/matching/strag3-naked-retry.md. */
void sub_8006600(struct sub_8006700_actor *arg0)
{
    u8 buf[16];
    s32 w;
    s32 n;
    u32 x;
    struct icon_record *r;

    sub_8006A90(gUnknown_03001300);
    sub_8006C28(gUnknown_030012FC);
    sub_8008890(arg0->field_18, 0, 0);
    r = gUnknown_030012E0->record;
    w = _call_via_r2((u8 *)gUnknown_030012E0 + r->slots[0].offset, arg0->field_10, r->slots[0].ptr);
    x = (u32)(240 - w) >> 1;
    set_icon_mgr_pos(gUnknown_030012E0, x, 0x2d);
    r = gUnknown_030012E0->record;
    _call_via_r2((u8 *)gUnknown_030012E0 + r->slots[2].offset, arg0->field_10, r->slots[2].ptr);
    sub_803AFE4(buf, 0x10, 0x6a);
    sub_803AFDC(buf, 0xd0, 0x35);
    sub_8001214(arg0->field_14, gUnknown_030012DC, buf, 0);
    n = GetUiText(0x2e);
    r = gUnknown_030012DC->record;
    w = _call_via_r2((u8 *)gUnknown_030012DC + r->slots[0].offset, n, r->slots[0].ptr);
    x = (u32)(240 - w) >> 1;
    set_icon_mgr_pos(gUnknown_030012DC, x, 0x90);
    r = gUnknown_030012DC->record;
    _call_via_r2((u8 *)gUnknown_030012DC + r->slots[2].offset, n, r->slots[2].ptr);
    sub_8006A48(gUnknown_03001300);
}

extern void WaitForVBlank(void *arg0);

void sub_8006700(struct sub_8006700_actor *arg0)
{
    arg0->field_1c++;
    sub_8008044(arg0->field_18);
}

void sub_8006714(struct sub_8006700_actor *arg0)
{
    WaitForVBlank(arg0);
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
    struct actor *field18;
    u8 *p;

    field18 = arg0->field_18;
    if (field18 != NULL) {
        p = (u8 *)field18->table + 0x50;
        _call_via_r2((u8 *)field18 + *(s16 *)p, 3, *(void **)(p + 4));
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
    base = (u8 *)gLevelTable;
    bound = base + 0x10; /* &gLevelTable[0].threshold_10 */
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
 * of the matching gLevelTable entry - see the comment on
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
            asm volatile("add %0, %1, #0\n\tadd %0, %0, #0xc\n\tadd %0, %2, %0" : "=r"(addr) : "r"(gLevelTable), "r"(offset));
            if (val <= *(u32 *)addr) {
                asm volatile("add %0, %1, #0\n\tadd %0, %0, #0x10\n\tadd %0, %2, %0" : "=r"(addr) : "r"(gLevelTable), "r"(offset));
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
            asm volatile("add %0, %1, #0\n\tadd %0, %0, #8\n\tadd %0, %2, %0" : "=r"(addr) : "r"(gLevelTable), "r"(offset));
            if (val <= *(u32 *)addr) {
                asm volatile("add %0, %1, #0\n\tadd %0, %0, #0xc\n\tadd %0, %2, %0" : "=r"(addr) : "r"(gLevelTable), "r"(offset));
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
