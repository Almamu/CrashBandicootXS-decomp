#include "core.h"

extern u8 gStaticData_0816C86C[];

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
    base = gStaticData_0816C86C;
    bound = base + 0x10;
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
