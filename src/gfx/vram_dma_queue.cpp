/* The VRAM DMA queue (gVramDmaQueue): FlushVramDmaQueue,
 * QueueVramDmaTransfer, FreeVramDmaQueue, AllocVramDmaQueue. Split from
 * gfx/graphics.cpp (#767), same flags (old_agbcc). */

extern "C" {
#include "core.h"
#include "gfx.h"
#include "memory.h"
#include "globals.h"
}

struct dma_queue_entry {
    void *dest;
    void *src;
    u16 size;
    u16 unit;
};

struct dma_queue {
    struct dma_queue_entry *entries;
    s32 count;
};

/* Allocated capacity of gVramDmaQueue.entries. */
#define DMA_QUEUE_MAX_ENTRIES 0x300

/* Runs the queued VRAM transfers, then waits for the last one. */
void FlushVramDmaQueue(void)
{
    s32 i;

    for (i = 0; i < gVramDmaQueue.count; i++) {
        struct dma_queue_entry *entry = &gVramDmaQueue.entries[i];

        if (entry->unit == 0x20) {
            DMA3.src = (u32)entry->src;
            DMA3.dst = (u32)entry->dest;
            DMA3.cnt = (entry->size >> 2) | ((DMA_ENABLE | DMA_32BIT) << 16);
        } else {
            DMA3.src = (u32)entry->src;
            DMA3.dst = (u32)entry->dest;
            DMA3.cnt = (entry->size >> 1) | ((DMA_ENABLE | DMA_16BIT) << 16);
        }
        (void)DMA3.cnt;
    }
    gVramDmaQueue.count = 0;

    while (DMA3.cnt & (DMA_ENABLE << 16)) {
    }
}

/* Queues a transfer of `size` bytes in units of `unit` bits: 0 when
 * there is nothing to send, -1 when the queue is full. */
s32 QueueVramDmaTransfer(void *src, void *dest, u16 size, u16 unit)
{
    struct dma_queue_entry *entry;

    if (size == 0)
        return 0;
    if (gVramDmaQueue.count > DMA_QUEUE_MAX_ENTRIES - 1)
        return -1;
    entry = &gVramDmaQueue.entries[gVramDmaQueue.count];
    gVramDmaQueue.count++;
    entry->dest = dest;
    entry->src = src;
    entry->size = size;
    entry->unit = unit;
    return 0;
}

void FreeVramDmaQueue(void)
{
    if (gVramDmaQueue.entries != 0) {
        mem_free((u8 *)gVramDmaQueue.entries);
        gVramDmaQueue.entries = 0;
    }
}

s32 AllocVramDmaQueue(void)
{
    gVramDmaQueue.entries = (struct dma_queue_entry *)mem_alloc(
        sizeof(struct dma_queue_entry) * DMA_QUEUE_MAX_ENTRIES, MEM_HEAP_EWRAM);
    if (gVramDmaQueue.entries == 0)
        return -1;
    gVramDmaQueue.count = 0;
    return 0;
}
