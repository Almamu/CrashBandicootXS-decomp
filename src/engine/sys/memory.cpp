extern "C" {
#include "core.h"
#include "engine/sys/memory.h"
}

static inline void mem_free_bytes_update(s32 flags)
{
    mem_initial_free_bytes = mem_free_bytes(flags);
}

static inline void mem_heap_init_section(struct mem_heap_header *first, struct mem_block *block,
                                         int length)
{
    first->header.next = block;
    first->header.tail = block;
    first->header.status = MEMORY_STATUS_USED;
    first->header.size = 0;
    first->nextFreeBlock = block;
    block->status = MEMORY_STATUS_FREE;
    block->tail = &first->header;
    block->next = &first->header;
    block->size = length - sizeof(struct mem_heap_header);
}

/* `reservedBytes`: the bytes at the top of IWRAM, below `iwram_end`, that
 * the IWRAM heap leaves out (main passes 0x400). */
s32 mem_heap_init(u32 reservedBytes)
{
    /* `start`, `end` are needed: inlined, the registers differ */
    u32 start = (u32)&mem_iwram_heap;
    u32 end = (u32)&iwram_end;
    u32 iwram_size_left = end - start - reservedBytes;

    // zero-out the regions we're going to use
    DmaClear32(3, &mem_iwram_heap, iwram_size_left);
    DmaFill16(3, 0, EWRAM_START, EWRAM_SIZE);

    // and initialize them with some defaults
    mem_iwram_heap_pointer = &mem_iwram_heap;
    mem_heap_init_section(&mem_iwram_heap_pointer->base, &mem_iwram_heap_pointer->mainblock,
                          iwram_size_left);
    mem_ewram_heap_pointer = (struct mem_heap *)EWRAM_START;
    mem_heap_init_section(&mem_ewram_heap_pointer->base, &mem_ewram_heap_pointer->mainblock,
                          EWRAM_SIZE);
    mem_free_bytes_update(MEM_HEAP_BOTH);

    return 0;
}

static inline void mem_collect_join_blocks(struct mem_block *into, struct mem_block *from,
                                           struct mem_heap *boundary)
{
    struct mem_block *temporal;

    into->size += from->size;
    temporal = from->next;
    into->next = temporal;
    temporal->tail = into;

    if (from == boundary->base.nextFreeBlock) {
        boundary->base.nextFreeBlock = into;
    }
}

static inline void mem_collect_heap(struct mem_heap_header *start)
{
    struct mem_block *heapblock = &start->header;
    u8 *buffer = 0;
    struct mem_block *current = heapblock->next;

    if (current == heapblock) {
        return;
    }

    do {
        struct mem_block *relative;
        struct mem_heap *boundary;
        struct mem_block *block;
        /* `next` is needed: inlined, the code is 2 instructions shorter */
        struct mem_block *next = current->next;

        if (current->status != MEMORY_STATUS_COLLECT) {
            goto next_iteration;
        }

        buffer = current->buffer;

        if (buffer == 0) {
            goto next_iteration;
        }

        // looks like the ewram and the iwram are treated as contiguos
        if ((u32)buffer >= (u32)mem_iwram_heap_pointer) {
            boundary = mem_iwram_heap_pointer;
        } else {
            boundary = mem_ewram_heap_pointer;
        }

        block = (struct mem_block *)(buffer - sizeof(struct mem_block));
        block->status = MEMORY_STATUS_FREE;
        relative = block->tail;

        if (relative->status == MEMORY_STATUS_FREE) {
            mem_collect_join_blocks(relative, block, boundary);
            block = relative;
        }

        relative = block->next;

        if (relative->status == MEMORY_STATUS_FREE) {
            mem_collect_join_blocks(block, relative, boundary);
        }

    next_iteration:
        current = next;
    } while (current != heapblock);
}

void mem_collect(s32 heaps)
{
    if (MEM_HEAP_IWRAM & heaps) {
        mem_collect_heap(&mem_iwram_heap_pointer->base);
    }

    if (MEM_HEAP_EWRAM & heaps) {
        mem_collect_heap(&mem_ewram_heap_pointer->base);
    }
}

/* Returns the heap's last block: the one whose `next` wraps back to the
 * heap's own header. */
static inline struct mem_block *mem_last_block(struct mem_heap *heap)
{
    struct mem_block *current = heap->base.header.next;

    while (current->next != &heap->base.header) {
        current = current->next;
    }

    return current;
}

/* UNUSED - ROM 0x0800039C, between mem_collect and mem_free_bytes: no
 * caller anywhere in the ROM. For each heap selected in `flags` (IWRAM,
 * then EWRAM) it walks to the last block and discards the result. Until
 * #662 round 2 it was a file-scope asm transcription (and before that a
 * `.byte` blob, see docs/decomp_dev.md); the plain C above matches. */
extern "C" void mem_walk_heaps(s32 flags)
{
    if (MEM_HEAP_IWRAM & flags) {
        mem_last_block(mem_iwram_heap_pointer);
    }

    if (MEM_HEAP_EWRAM & flags) {
        mem_last_block(mem_ewram_heap_pointer);
    }
}

static inline u32 mem_free_bytes_for_heap(struct mem_heap *heap)
{
    u32 result = 0;
    struct mem_block *current = heap->base.header.next;

    while (current != &heap->base.header) {
        if (current->status == MEMORY_STATUS_FREE) {
            result += current->size - sizeof(struct mem_block);
        }

        current = current->next;
    }

    return result;
}

s32 mem_free_bytes(s32 heaps)
{
    u32 free_bytes = 0;

    if (MEM_HEAP_IWRAM & heaps) {
        free_bytes += mem_free_bytes_for_heap(mem_iwram_heap_pointer);
    }

    if (MEM_HEAP_EWRAM & heaps) {
        free_bytes += mem_free_bytes_for_heap(mem_ewram_heap_pointer);
    }

    return free_bytes;
}

void *mem_alloc(u32 requestedSize, u32 flags)
{
    struct mem_block *current;
    /* `end` is needed: inlined, the code is 1 instruction shorter */
    struct mem_block *end;
    struct mem_heap *heap;
    u32 alignedSize = requestedSize;
    s32 freeBytesAfterReservation;

    if (MEM_HEAP_IWRAM & flags) {
        heap = mem_iwram_heap_pointer;
    } else {
        heap = mem_ewram_heap_pointer;
    }

    // align value
    alignedSize += 0x13;
    alignedSize &= ~3;

    current = heap->base.nextFreeBlock;
    end = current->tail;

    for (; current->status != MEMORY_STATUS_FREE || (u32)current->size < alignedSize;
         current = current->next) {
        if (current == end) {
            return 0;
        }
    }

    freeBytesAfterReservation = current->size - alignedSize;

    // do not bother updating sizes if the difference in bytes
    // is not enough to fit a decent block of free data
    if ((u32)freeBytesAfterReservation > 0x40U) {
        struct mem_block *newBlockAfter = (struct mem_block *)((u32)current + alignedSize);
        struct mem_block *temp_r1_3;

        newBlockAfter->size = freeBytesAfterReservation;
        newBlockAfter->status = MEMORY_STATUS_FREE;
        newBlockAfter->tail = current;
        temp_r1_3 = current->next;
        newBlockAfter->next = temp_r1_3;
        temp_r1_3->tail = newBlockAfter;
        current->next = newBlockAfter;
        current->size = alignedSize;
    }

    current->status = MEMORY_STATUS_COLLECT;
    heap->base.nextFreeBlock = current->next;

    return current->buffer;
}

void mem_free(void *address)
{
    struct mem_heap *heap;
    struct mem_block *tmp;
    struct mem_block *adjacent;
    struct mem_block *current;

    if (address == 0) {
        return;
    }

    if (address >= mem_iwram_heap_pointer) {
        heap = mem_iwram_heap_pointer;
    } else {
        heap = mem_ewram_heap_pointer;
    }

    // get the memory pointer's block header
    current = (struct mem_block *)((u8 *)address - sizeof(struct mem_block));
    current->status = MEMORY_STATUS_FREE;
    adjacent = current->tail;

    if (adjacent->status == MEMORY_STATUS_FREE) {
        adjacent->size = adjacent->size + current->size;
        tmp = current->next;
        adjacent->next = tmp;
        tmp->tail = adjacent;
        if (current == heap->base.nextFreeBlock) {
            heap->base.nextFreeBlock = adjacent;
        }
        current = adjacent;
    }

    adjacent = current->next;

    if (adjacent->status == MEMORY_STATUS_FREE) {
        current->size = current->size + adjacent->size;
        tmp = adjacent->next;
        current->next = tmp;
        tmp->tail = current;
        if (adjacent == heap->base.nextFreeBlock) {
            heap->base.nextFreeBlock = current;
        }
    }
}

/* Called by AgbMain once MainLoop returns: if the heaps hold less free
 * memory than right after mem_heap_init (mem_initial_free_bytes), some
 * allocations are still live, so collect them. The second
 * mem_free_bytes result is unused. */
void mem_heap_shutdown(void)
{
    /* `currentFreeBytes` is needed: inlined, the instructions differ */
    s32 currentFreeBytes = mem_free_bytes(MEM_HEAP_BOTH);

    if (mem_initial_free_bytes != currentFreeBytes) {
        mem_collect(MEM_HEAP_BOTH);
        mem_free_bytes(MEM_HEAP_BOTH);
    }
}
