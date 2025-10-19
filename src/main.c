#include "core.h"
#include "memory.h"

s32 mem_heap_init(u32);
extern void sub_8000518();                                    /* extern */
extern void sub_80005D0();                                    /* extern */
extern u32 irq_setup();                                  /* extern */
extern void sub_8000620();                                    /* extern */
extern s32 sub_8026EEC();                                  /* extern */


s32 AgbMain(void) {
    // setup cart
    REG_WAITCNT = WAITCNT_WS0_N_3 | WAITCNT_WS0_S_1 | WAITCNT_PREFETCH_ENABLE;
    // setup display configuration, also updates REG_ADDR_BLDALPHA to 0
    *(vu32 *) REG_ADDR_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_DARKEN;
    REG_BLDY = 0x10;
    REG_DISPCNT = DISPCNT_MODE_0;

    if (mem_heap_init(0x400) != 0 || irq_setup() != 0) {
        return -1;
    }
    
    sub_8000620();
    
    if (sub_8026EEC() != 0) {
        return -1;
    }
    
    sub_80005D0();
    sub_8000518();
    
    return 0;
}

static inline void mem_free_bytes_update (s32 flags) {
    s32 result = mem_free_bytes(flags);
    gUnknown_030007D4 = result;
}

static inline void mem_heap_init_section (struct mem_heap_header* first, struct mem_heap_block* block, int length) {
    first->next = block;
    first->tail = block;
    first->status = MEMORY_STATUS_USED;
    first->size = 0;
    first->unk10 = block;
    block->status = MEMORY_STATUS_FREE;
    block->tail = first;
    block->next = first;
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
    mem_heap_init_section (mem_iwram_heap_pointer, &mem_iwram_heap_pointer->mainblock, iwram_size_left);
    mem_ewram_heap_pointer = EWRAM_START;
    mem_heap_init_section (mem_ewram_heap_pointer, &mem_ewram_heap_pointer->mainblock, EWRAM_SIZE);
    mem_free_bytes_update(0xC0000000);
    
    return 0;
}
#if NON_MATCHING 1
static inline struct unk* mem_collect_something (struct unk* into, struct unk* from, struct unk* boundary) {
    struct unk* temporal;

    into->size += from->size;
    temporal = from->next;
    into->next = temporal;
    temporal->tail = into;

    if (from == boundary->unk10) {
        boundary->unk10 = into;
    }

    return into;
}
    
static inline void mem_collect_heap (struct unk* start) {
    struct unk* current;

    for (current = start->next; current != start; current = current->next) {
        struct unk* boundary;
        struct unk* block;
        struct unk* relative;
        struct unk** test;
        
        if (current->next->status != MEMORY_STATUS_COLLECT) {
            continue;
        }

        test = &current->unk10;
        
        if (test == NULL) {
            continue;
        }

        // looks like the ewram and the iwram are treated as contiguos
        if ((u32) test >= (u32) mem_iwram_heap_pointer) {
            boundary = mem_iwram_heap_pointer;
        } else {
            boundary = mem_ewram_heap_pointer;
        }

        //TODO: FIND A BETTER WAY OF DESCRIBING THIS INDIRECTION?
        block = test - 0x04;
        block->status = MEMORY_STATUS_FREE;
        relative = block->tail;

        if (relative->status == MEMORY_STATUS_FREE) {
            block = mem_collect_something (relative, block, boundary);
        }

        relative = block->next;
        
        if (relative->status == MEMORY_STATUS_FREE) {
            mem_collect_something (block, relative, boundary);
        }
    }
}

void mem_collect(s32 arg0) {
    if (0x80000000 & arg0) {
        mem_collect_heap (mem_iwram_heap_pointer);
    }
    
    if (0x40000000 & arg0) {
        mem_collect_heap (mem_ewram_heap_pointer);
    }
}
#endif
