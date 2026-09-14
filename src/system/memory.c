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

/* ROM 0x0800039C - dead code: reachable from nothing in this file (or
 * any other matched source), but its bytes still sit between
 * mem_collect and mem_free_bytes_for_heap in the ROM and must be
 * reproduced for a byte-exact build. Two near-identical halves, one
 * per heap (IWRAM if MEM_HEAP_IWRAM is set, then EWRAM if
 * MEM_HEAP_EWRAM is set): take &mem_i/ewram_heap_pointer, deref it for
 * the heap struct, then loop `p = p->field_8` until `p->field_8` loops
 * back to the heap struct itself - the same circular-list-walk idiom
 * mem_collect_heap uses, but here the result is never stored anywhere,
 * consistent with this being an optimizer-emitted leftover (e.g. a
 * partially-shared/identical-code-folded copy of a real function body)
 * rather than something reachable from source. Kept as real
 * instructions rather than a raw byte blob so it's inspectable; see
 * docs/decomp_dev.md for how this was found and verified byte-for-byte
 * against the disassembled ROM. */
__asm__(
    ".align 2, 0\n"
    ".thumb_func\n"
    ".type sub_800039C, function\n"
    ".global sub_800039C\n"
    "sub_800039C:\n"
    "add r3, r0, #0\n\t"
    "cmp r3, #0\n\t"
    "bge 1f\n\t"
    "ldr r0, 2f\n\t"
    "ldr r2, [r0, #0]\n\t"
    "ldr r1, [r2, #8]\n\t"
    "b 3f\n\t"
    ".align 2, 0\n"
    "2: .4byte mem_iwram_heap_pointer\n"
    "4: ldr r1, [r1, #8]\n"
    "3: ldr r0, [r1, #8]\n\t"
    "cmp r0, r2\n\t"
    "bne 4b\n"
    "1: mov r0, #0x80\n\t"
    "lsl r0, r0, #23\n\t"
    "and r0, r3\n\t"
    "cmp r0, #0\n\t"
    "beq 5f\n\t"
    "ldr r0, 6f\n\t"
    "ldr r2, [r0, #0]\n\t"
    "ldr r1, [r2, #8]\n\t"
    "b 7f\n\t"
    ".align 2, 0\n"
    "6: .4byte mem_ewram_heap_pointer\n"
    "8: ldr r1, [r1, #8]\n"
    "7: ldr r0, [r1, #8]\n\t"
    "cmp r0, r2\n\t"
    "bne 8b\n"
    "5: bx lr\n"
    ".align 2, 0\n"
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
