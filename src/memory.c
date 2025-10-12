#include "core.h"
#include "memory.h"

static s32 inline mem_free_bytes_for_heap (struct unk* heap) {
    s32 result;
    struct unk* current;
    
    result = 0;
    current = heap->next;
    while (current != heap) {
        if (current->status == MEMORY_STATUS_FREE) {
            result = (result - 0x10) + current->size;
        }
        current = current->next;
    }
    
    return result;
}

s32 mem_free_bytes(s32 arg0) {
    s32 result = 0;
    
    if (0x80000000 & arg0) {
        result = mem_free_bytes_for_heap (mem_iwram_heap);
    }
    
    if (0x40000000 & arg0) {
        result += mem_free_bytes_for_heap (mem_ewram_heap);
    }
    
    return result;
}