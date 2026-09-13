#include "core.h"
#include "memory.h"

struct dma_queue_entry {
    void *field_00;
    void *field_04;
    u16 field_08;
    u16 field_0A;
};

struct dma_queue {
    struct dma_queue_entry *entries;
    s32 count;
};

struct dma_regs {
    vu32 src;
    vu32 dst;
    vu32 cnt;
};

extern struct dma_queue gUnknown_03001290;
#define DMA3 (*(struct dma_regs *)0x040000D4)
#define QUEUE_COUNT (((volatile struct dma_queue *)&gUnknown_03001290)->count)

void sub_80069E8(void *arg0, u16 *arg1, s32 arg2)
{
    u8 *entry;
    u16 zero;

    if (arg2 <= 0) {
        return;
    }
    zero = 0;
    entry = (u8 *)arg0;
    do {
        *(u16 *)(entry + 0x12) = arg1[0];
        entry += 8;
        *(u16 *)(entry + 0x12) = zero;
        entry += 8;
        *(u16 *)(entry + 0x12) = zero;
        entry += 8;
        *(u16 *)(entry + 0x12) = arg1[1];
        entry += 8;
        arg1 += 2;
        arg2--;
    } while (arg2 != 0);
}

void sub_8006A14(void *arg0, void *arg1, s32 arg2)
{
    if (arg2 == 0) {
        return;
    }
    DMA3.src = arg1;
    DMA3.dst = (u8 *)arg0 + ((*(s32 *)arg0 << 3) + 0xC);
    DMA3.cnt = (arg2 << 1) | 0x84000000;
    (void)DMA3.cnt;
    *(s32 *)arg0 = *(s32 *)arg0 + arg2;
}

void sub_8006A48(void *arg0)
{
    register u8 *self asm("r1");
    register s32 i asm("r2");
    register s32 bit asm("r3");
    register s32 mask asm("r4");
    register u8 loaded asm("r5");
    register s32 result asm("r0");

    self = (u8 *)arg0;
    i = *(s32 *)self;
    if (i > 0x7F) {
        return;
    }
    mask = ~3;
    bit = 2;
    result = (i << 3) + 0xD;
    asm volatile("add %0, %1, %0" : "+r"(self) : "r"(result));
    do {
        asm volatile("add %0, %1, #0" : "=r"(result) : "r"(mask));
        loaded = *self;
        result &= loaded;
        result |= bit;
        *self = result;
        self += 8;
        i++;
    } while (i <= 0x7F);
}

struct sub_8006A78_struct {
    s32 field_00;
    s32 field_04;
    s32 field_08;
};

void sub_8006A78(struct sub_8006A78_struct *arg0)
{
    arg0->field_00 = arg0->field_04;
    arg0->field_08 = 0;
}

void sub_8006A84(struct sub_8006A78_struct *arg0)
{
    arg0->field_04 = arg0->field_00;
    arg0->field_08 = 0;
}

void sub_8006A90(struct sub_8006A78_struct *arg0)
{
    arg0->field_00 = 0;
    arg0->field_08 = 0;
    sub_8006A84(arg0);
    sub_8006A78(arg0);
}

void sub_8006AAC(void *arg0)
{
    DMA3.src = (u8 *)arg0 + 0xC;
    DMA3.dst = (void *)0x07000000;
    DMA3.cnt = 0x84000100;
    (void)DMA3.cnt;
}

void sub_8006AC8(void *arg0, u32 *arg1)
{
    register s32 n1 asm("r2");
    register u16 saved asm("r3");
    register s32 n2 asm("r1");
    register s32 addr2 asm("r0");
    u32 v0;
    u32 v1;

    n1 = *(vs32 *)arg0;
    if (n1 > 0x7F) {
        return;
    }
    n1 = (s32)arg0 + (n1 << 3);
    saved = *(u16 *)(n1 + 0x12);
    v0 = arg1[0];
    v1 = arg1[1];
    *(u32 *)(n1 + 0xC) = v0;
    *(u32 *)(n1 + 0x10) = v1;
    n2 = *(vs32 *)arg0;
    addr2 = (s32)arg0 + (n2 << 3);
    *(u16 *)(addr2 + 0x12) = saved;
    n2++;
    *(s32 *)arg0 = n2;
}

extern void sub_8026ED0(void *arg0);

void sub_8006AF4(void *arg0, u32 arg1)
{
    if (arg1 & 1) {
        sub_8026ED0(arg0);
    }
}

void *sub_8006B0C(void *arg0)
{
    sub_8006A90(arg0);
    return arg0;
}

void FlushVramDmaQueue(void)
{
    struct dma_queue_entry *entry;
    s32 i;
    register u16 raw asm("r1");
    register u32 shifted asm("r0");

    for (i = 0; i < QUEUE_COUNT; i++) {
        entry = &gUnknown_03001290.entries[i];
        if (entry->field_0A == 0x20) {
            DMA3.src = entry->field_04;
            DMA3.dst = entry->field_00;
            raw = entry->field_08;
            shifted = raw >> 2;
            shifted |= 0x84000000;
        } else {
            DMA3.src = entry->field_04;
            DMA3.dst = entry->field_00;
            raw = entry->field_08;
            shifted = raw >> 1;
            shifted |= 0x80000000;
        }
        DMA3.cnt = shifted;
        (void)DMA3.cnt;
    }
    gUnknown_03001290.count = 0;

    while (DMA3.cnt & 0x80000000) {
    }
}

s32 QueueVramDmaTransfer(void *arg0, void *arg1, u16 arg2, u16 arg3)
{
    struct dma_queue_entry *entry;

    if (arg2 == 0) {
        return 0;
    }
    if (gUnknown_03001290.count > 0x2FF) {
        return -1;
    }
    entry = &gUnknown_03001290.entries[gUnknown_03001290.count];
    gUnknown_03001290.count++;
    entry->field_00 = arg1;
    entry->field_04 = arg0;
    entry->field_08 = arg2;
    entry->field_0A = arg3;
    return 0;
}

void FreeVramDmaQueue(void)
{
    if (gUnknown_03001290.entries != NULL) {
        mem_free((u8 *)gUnknown_03001290.entries);
        gUnknown_03001290.entries = NULL;
    }
}
