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
    mem_free_bytes_update(MEM_HEAP_BOTH);
    
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
    if (MEM_HEAP_IWRAM & arg0) {
        mem_collect_heap (&mem_iwram_heap_pointer->base);
    }
    
    if (MEM_HEAP_EWRAM & arg0) {
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

    if (MEM_HEAP_IWRAM & arg0) {
        free_bytes += mem_free_bytes_for_heap (mem_iwram_heap_pointer);
    }
    
    if (MEM_HEAP_EWRAM & arg0) {
        free_bytes += mem_free_bytes_for_heap (mem_ewram_heap_pointer);
    }
    
    return free_bytes;
}

u8* mem_alloc(u32 requestedSize, s32 arg1) {
    struct mem_block* current;
    struct mem_block* end;
    struct mem_heap* heap;
    struct mem_block* result;
    u32 alignedSize = requestedSize;
    s32 freeBytesAfterReservation;

    if (MEM_HEAP_IWRAM & arg1) {
        heap = mem_iwram_heap_pointer;
    } else {
        heap = mem_ewram_heap_pointer;
    }

    // align value
    alignedSize += 0x13;
    alignedSize &= ~3;

    current = heap->base.nextFreeBlock;
    end = current->tail;

    for (; current->status != MEMORY_STATUS_FREE || current->size < alignedSize; current = current->next) {
        if (current == end) {
            return NULL;
        }
    }
    
    freeBytesAfterReservation = current->size - alignedSize;

    // do not bother updating sizes if the difference in bytes
    // is not enough to fit a decent block of free data
    if (freeBytesAfterReservation > 0x40U) {
        struct mem_block* newBlockAfter = (struct mem_block*) ((u32) current + alignedSize);
        struct mem_block* temp_r1_3;
        
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

void mem_free(u8* address) {
    struct mem_heap* heap;
    struct mem_block* tmp;
    struct mem_block* adjacent;
    struct mem_block* current;

    if (address == NULL) {
        return;
    }

    if (address >= (u32) mem_iwram_heap_pointer) {
        heap = mem_iwram_heap_pointer;
    } else {
        heap = mem_ewram_heap_pointer;
    }

    // get the memory pointer's block header
    current = (u8*) address - sizeof(struct mem_block);
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

// TODO: WHAT IS THIS USED FOR? THE RETURN OF mem_free_bytes IS NOT USED FOR ANYTHING?
void sub_8000518() {
    s32 currentFreeBytes = mem_free_bytes(MEM_HEAP_BOTH);
    
    if (gUnknown_030007D4 != currentFreeBytes) {
        mem_collect(MEM_HEAP_BOTH);
        mem_free_bytes(MEM_HEAP_BOTH);
    }
}
