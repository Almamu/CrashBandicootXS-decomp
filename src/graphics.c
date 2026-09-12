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

extern void sub_8026ED0(void *arg0);

void sub_8006AF4(void *arg0, u32 arg1)
{
    if (arg1 & 1) {
        sub_8026ED0(arg0);
    }
}

extern void sub_8006A90(void *arg0);

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
