#include "core.h"
#include "memory.h"

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
