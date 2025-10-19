#include "core.h"
#include "memory.h"

static inline void mem_free_bytes_update (s32 flags) {
    s32 result = mem_free_bytes(flags);
    gUnknown_030007D4 = result;
}

static inline void mem_heap_init_section (struct mem_heap_header* first, struct mem_block* block, int length) {
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

s32 mem_heap_init (u32 arg0) {
    u32 start = &gUnknown_03001638;
    u32 end = &iwram_end;
    u32 iwram_size_left = end - start - arg0;

    // zero-out the regions we're going to use
    DmaClear32(3, &gUnknown_03001638, iwram_size_left);
    DmaFill16(3, 0, EWRAM_START, EWRAM_SIZE);

    // and initialize them with some defaults
    mem_iwram_heap_pointer = &gUnknown_03001638;
    mem_heap_init_section (&mem_iwram_heap_pointer->base, &mem_iwram_heap_pointer->mainblock, iwram_size_left);
    mem_ewram_heap_pointer = EWRAM_START;
    mem_heap_init_section (&mem_ewram_heap_pointer->base, &mem_ewram_heap_pointer->mainblock, EWRAM_SIZE);
    mem_free_bytes_update(0xC0000000);
    
    return 0;
}

static inline void mem_collect_join_blocks (
    struct mem_block* into, struct mem_block* from, struct mem_heap* boundary) {
    struct mem_block* temporal;

    into->size += from->size;
    temporal = from->next;
    into->next = temporal;
    temporal->tail = into;

    if (from == boundary->base.nextFreeBlock) {
        boundary->base.nextFreeBlock = into;
    }
}

static inline void mem_collect_heap (struct mem_heap_header* start) {
    struct mem_block* heapblock = &start->header;
    u8* buffer = NULL;
    struct mem_block* current = heapblock->next;

    if (current == heapblock) {
        return;
    }
    
    do {
        struct mem_block* relative;
        struct mem_heap* boundary;
        struct mem_block* block;
        struct mem_block* next = current->next;
        
        if (current->status != MEMORY_STATUS_COLLECT) {
            goto next_iteration;
        }

        buffer = current->buffer;
        
        if (buffer == NULL) {
            goto next_iteration;
        }

        // looks like the ewram and the iwram are treated as contiguos
        if ((u32) buffer >= (u32) mem_iwram_heap_pointer) {
            boundary = mem_iwram_heap_pointer;
        } else {
            boundary = mem_ewram_heap_pointer;
        }

        block = (struct mem_block*) (buffer - sizeof (struct mem_block));
        block->status = MEMORY_STATUS_FREE;
        relative = block->tail;

        if (relative->status == MEMORY_STATUS_FREE) {
            mem_collect_join_blocks (relative, block, boundary);
            block = relative;
        }

        relative = block->next;
        
        if (relative->status == MEMORY_STATUS_FREE) {
            mem_collect_join_blocks (block, relative, boundary);
        }

        next_iteration:
        current = next;
    } while (current != heapblock);
}

void mem_collect(s32 arg0) {
    if (0x80000000 & arg0) {
        mem_collect_heap (&mem_iwram_heap_pointer->base);
    }
    
    if (0x40000000 & arg0) {
        mem_collect_heap (&mem_ewram_heap_pointer->base);
    }
}

// this is ugly AF
__asm__(
    ".align 2, 0\n"
    ".byte 0x03, 0x1C, 0x00, 0x2B\n"
	".byte 0x0A, 0xDA, 0x02, 0x48, 0x02, 0x68, 0x91, 0x68, 0x03, 0xE0, 0x00, 0x00, 0xCC, 0x07, 0x00, 0x03\n"
	".byte 0x89, 0x68, 0x88, 0x68, 0x90, 0x42, 0xFB, 0xD1, 0x80, 0x20, 0xC0, 0x05, 0x18, 0x40, 0x00, 0x28\n"
	".byte 0x0A, 0xD0, 0x02, 0x48, 0x02, 0x68, 0x91, 0x68, 0x03, 0xE0, 0x00, 0x00, 0xD0, 0x07, 0x00, 0x03\n"
	".byte 0x89, 0x68, 0x88, 0x68, 0x90, 0x42, 0xFB, 0xD1, 0x70, 0x47, 0x00, 0x00"
);

static inline u32 mem_free_bytes_for_heap (struct mem_heap* heap) {
    u32 result = 0;
    struct mem_block* current = heap->base.header.next;

    while (current != heap) {
        if (current->status == MEMORY_STATUS_FREE) {
            result += current->size - sizeof (struct mem_block);
        }
        
        current = current->next;
    }
    
    return result;
}

s32 mem_free_bytes(s32 arg0) {
    u32 free_bytes = 0;

    if (0x80000000 & arg0) {
        free_bytes += mem_free_bytes_for_heap (mem_iwram_heap_pointer);
    }
    
    if (0x40000000 & arg0) {
        free_bytes += mem_free_bytes_for_heap (mem_ewram_heap_pointer);
    }
    
    return free_bytes;
}
