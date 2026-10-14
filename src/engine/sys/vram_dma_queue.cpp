/* The VRAM DMA queue (VramDmaQueue, vram_dma_queue.hpp): Flush, Add,
 * Destroy, Init (FlushVramDmaQueue, QueueVramDmaTransfer,
 * FreeVramDmaQueue, AllocVramDmaQueue). Split from gfx/graphics.cpp
 * (#767), same flags (old_agbcc). */

#include "engine/sys/vram_dma_queue.hpp"

extern "C" {
#include "core.h"
#include "engine/gfx/gfx.h"
#include "engine/sys/memory.h"
#include "globals.h"
}

using Engine::Sys::VramDmaQueue;

struct dma_queue_entry {
    void *dest;
    const void *src;
    u16 size;
    u16 unit;
};

struct dma_queue {
    struct dma_queue_entry *entries;
    s32 count;
};

/* Allocated capacity of `queue.entries`. */
#define DMA_QUEUE_MAX_ENTRIES 0x300

/* Runs the queued VRAM transfers, then waits for the last one. */
void VramDmaQueue::Flush()
{
    s32 i;

    for (i = 0; i < queue.count; i++) {
        struct dma_queue_entry *entry = &queue.entries[i];

        if (entry->unit == 0x20) {
            DmaCopy32(3, entry->src, entry->dest, entry->size);
        } else {
            DmaCopy16(3, entry->src, entry->dest, entry->size);
        }
    }
    queue.count = 0;

    DmaWait(3);
}

/* Queues a transfer of `size` bytes in units of `unit` bits: 0 when
 * there is nothing to send, -1 when the queue is full. */
s32 VramDmaQueue::Add(const void *src, void *dest, u16 size, u16 unit)
{
    struct dma_queue_entry *entry;

    if (size == 0)
        return 0;
    if (queue.count > DMA_QUEUE_MAX_ENTRIES - 1)
        return -1;
    entry = &queue.entries[queue.count];
    queue.count++;
    entry->dest = dest;
    entry->src = src;
    entry->size = size;
    entry->unit = unit;
    return 0;
}

void VramDmaQueue::Destroy()
{
    if (queue.entries != 0) {
        mem_free(queue.entries);
        queue.entries = 0;
    }
}

s32 VramDmaQueue::Init()
{
    queue.entries = (struct dma_queue_entry *)mem_alloc(
        sizeof(struct dma_queue_entry) * DMA_QUEUE_MAX_ENTRIES, MEM_HEAP_EWRAM);
    if (queue.entries == 0)
        return -1;
    queue.count = 0;
    return 0;
}
