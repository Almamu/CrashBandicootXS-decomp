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

extern struct threshold_table_entry gStaticData_0816C86C[];
extern void sub_80062A8(s32 arg0, s32 arg1, s32 arg2);
extern s32 sub_803AD80(void *arg0, s32 arg1, void *arg2);
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
extern s32 sub_8026F38(s32 arg0);
extern struct vram_upload_cursor *gUnknown_030012FC;

extern struct icon_manager *gUnknown_030012E0;
extern struct icon_manager *gUnknown_030012DC;

/* Positions two OAM icons flanking a number (drawn via sub_8001214 in
 * between) - centers each icon horizontally from its rendered pixel
 * width (`sub_803AD80`'s return value), at fixed Y coordinates. Written
 * as NAKED asm, not plain C: the last remaining register-letter gap (see
 * docs/matching.md, "Parked, not matched: sub_8006600") was one scratch
 * register (r7) used only to reload a value with no cross-call lifetime
 * - exactly the "plain-C-visible temp, no intervening call" shape that
 * `matching_decomp_register_pinning` memory's technique 10 already
 * confirmed a `register T x asm("r7")` pin never gets included in this
 * toolchain's compiled output for. A transcription of the ROM's own
 * confirmed-correct instructions (this project's other hard-
 * compiler-limitation cases use the same technique - see
 * src/system/link_cable.c/src/audio/gax_swi.c). */
NAKED void sub_8006600(struct sub_8006700_actor *arg0)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sb\n\t"
        "mov r6, r8\n\t"
        "push {r6, r7}\n\t"
        "sub sp, #0x10\n\t"
        "add r4, r0, #0\n\t"
        "ldr r0, 1f\n\t"
        "mov sb, r0\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8006A90\n\t"
        "ldr r0, 2f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8006C28\n\t"
        "ldr r0, [r4, #0x18]\n\t"
        "mov r1, #0\n\t"
        "mov r2, #0\n\t"
        "bl sub_8008890\n\t"
        "ldr r1, 3f\n\t"
        "mov r8, r1\n\t"
        "ldr r0, [r1]\n\t"
        "mov r5, #0x98\n\t"
        "lsl r5, r5, #1\n\t"
        "add r1, r0, r5\n\t"
        "ldr r2, [r1]\n\t"
        "mov r3, #0x10\n\t"
        "ldrsh r1, [r2, r3]\n\t"
        "add r0, r0, r1\n\t"
        "ldr r1, [r4, #0x10]\n\t"
        "ldr r2, [r2, #0x14]\n\t"
        "bl sub_803AD80\n\t"
        "mov r6, #0xf0\n\t"
        "sub r0, r6, r0\n\t"
        "lsr r3, r0, #1\n\t"
        "mov r7, r8\n\t"
        "ldr r0, [r7]\n\t"
        "mov r2, #0x2d\n\t"
        "mov r7, #0x88\n\t"
        "lsl r7, r7, #1\n\t"
        "add r1, r0, r7\n\t"
        "str r3, [r1]\n\t"
        "mov r3, #0x8a\n\t"
        "lsl r3, r3, #1\n\t"
        "add r1, r0, r3\n\t"
        "str r2, [r1]\n\t"
        "add r1, r0, r5\n\t"
        "ldr r2, [r1]\n\t"
        "mov r7, #0x20\n\t"
        "ldrsh r1, [r2, r7]\n\t"
        "add r0, r0, r1\n\t"
        "ldr r1, [r4, #0x10]\n\t"
        "ldr r2, [r2, #0x24]\n\t"
        "bl sub_803AD80\n\t"
        "mov r0, sp\n\t"
        "mov r1, #0x10\n\t"
        "mov r2, #0x6a\n\t"
        "bl sub_803AFE4\n\t"
        "mov r0, sp\n\t"
        "mov r1, #0xd0\n\t"
        "mov r2, #0x35\n\t"
        "bl sub_803AFDC\n\t"
        "ldr r0, [r4, #0x14]\n\t"
        "ldr r4, 4f\n\t"
        "ldr r1, [r4]\n\t"
        "mov r2, sp\n\t"
        "mov r3, #0\n\t"
        "bl sub_8001214\n\t"
        "mov r0, #0x2e\n\t"
        "bl sub_8026F38\n\t"
        "mov r8, r0\n\t"
        "ldr r0, [r4]\n\t"
        "add r1, r0, r5\n\t"
        "ldr r2, [r1]\n\t"
        "mov r3, #0x10\n\t"
        "ldrsh r1, [r2, r3]\n\t"
        "add r0, r0, r1\n\t"
        "ldr r2, [r2, #0x14]\n\t"
        "mov r1, r8\n\t"
        "bl sub_803AD80\n\t"
        "sub r6, r6, r0\n\t"
        "lsr r3, r6, #1\n\t"
        "ldr r0, [r4]\n\t"
        "mov r2, #0x90\n\t"
        "mov r4, #0x88\n\t"
        "lsl r4, r4, #1\n\t"
        "add r1, r0, r4\n\t"
        "str r3, [r1]\n\t"
        "mov r7, #0x8a\n\t"
        "lsl r7, r7, #1\n\t"
        "add r1, r0, r7\n\t"
        "str r2, [r1]\n\t"
        "add r5, r0, r5\n\t"
        "ldr r2, [r5]\n\t"
        "mov r3, #0x20\n\t"
        "ldrsh r1, [r2, r3]\n\t"
        "add r0, r0, r1\n\t"
        "ldr r2, [r2, #0x24]\n\t"
        "mov r1, r8\n\t"
        "bl sub_803AD80\n\t"
        "mov r4, sb\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_8006A48\n\t"
        "add sp, #0x10\n\t"
        "pop {r3, r4}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "1: .4byte gUnknown_03001300\n"
    "2: .4byte gUnknown_030012FC\n"
    "3: .4byte gUnknown_030012E0\n"
    "4: .4byte gUnknown_030012DC\n"
    );
}

extern void sub_80006A8(void *arg0);

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
    struct actor *field18;
    u8 *p;

    field18 = arg0->field_18;
    if (field18 != NULL) {
        p = (u8 *)field18->table + 0x50;
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
