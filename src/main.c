#include "core.h"
#include "memory.h"

s32 mem_heap_init(s32);
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

s32 mem_heap_init(s32 arg0) {
    u32 start = gUnknown_03001638;
    s32 end = &iwram_end;
    s32 iwram_size_left = end - start - arg0;

    // zero-out the regions we're going to use
    DmaClear32(3, gUnknown_03001638, iwram_size_left);
    DmaFill16(3, 0, EWRAM_START, EWRAM_SIZE);

    // and initialize them with some defaults
    mem_iwram_heap = gUnknown_03001638;
    gUnknown_03001638[0].next = &gUnknown_03001638[1];
    gUnknown_03001638[0].tail = &gUnknown_03001638[1];
    gUnknown_03001638[0].status = MEMORY_STATUS_USED;
    gUnknown_03001638[0].size = 0;
    gUnknown_03001638[0].unk10 = &gUnknown_03001638[1];
    gUnknown_03001638[1].status = MEMORY_STATUS_FREE;
    gUnknown_03001638[1].tail = &gUnknown_03001638[0];
    gUnknown_03001638[1].next = &gUnknown_03001638[0];
    gUnknown_03001638[1].size = iwram_size_left - sizeof(struct unk);
    mem_ewram_heap = EWRAM_START; // gUnknown_02000000
    mem_ewram_heap[0].next = &mem_ewram_heap[1];
    mem_ewram_heap[0].tail = &mem_ewram_heap[1];
    mem_ewram_heap[0].status = MEMORY_STATUS_USED;
    mem_ewram_heap[0].size = 0;
    mem_ewram_heap[0].unk10 = &mem_ewram_heap[1];
    mem_ewram_heap[1].status = MEMORY_STATUS_FREE;
    mem_ewram_heap[1].tail = &mem_ewram_heap[0];
    mem_ewram_heap[1].next = &mem_ewram_heap[0];
    mem_ewram_heap[1].size = EWRAM_SIZE - sizeof(struct unk);
    gUnknown_030007D4 = mem_free_bytes(0xC0000000);
    
    return 0;
}

#if NON_MATCHING
static void inline mem_collect_heap(struct unk* heap) {
    struct unk* var_r3;
    struct unk* temp_r0;
    struct unk* temp_r2;
    struct unk* current;
    struct unk* var_r4;
    
    current = heap->next;
    while (current != heap) {
        if (current->status == MEMORY_STATUS_COLLECT) {
            temp_r0 = current->unk10;
            if (temp_r0 != NULL) {
                temp_r2 = heap;
                if ((u32) temp_r0 >= (u32) temp_r2) {
                    var_r4 = temp_r2;
                } else {
                    var_r4 = heap;
                }
                var_r3 = temp_r0 - 0x10;
                var_r3->status = MEMORY_STATUS_FREE;
                temp_r2 = var_r3->tail;
                if (temp_r2->status == MEMORY_STATUS_FREE) {
                    temp_r2->size = temp_r2->size + var_r3->size;
                    temp_r0 = var_r3->next;
                    temp_r2->next = temp_r0;
                    temp_r0->tail = temp_r2;
                    if (var_r3 == var_r4->unk10) {
                        var_r4->unk10 = temp_r2;
                    }
                    var_r3 = temp_r2;
                }
                temp_r2 = var_r3->next;
                if (temp_r2->status == MEMORY_STATUS_FREE) {
                    var_r3->size = var_r3->size + temp_r2->size;
                    temp_r0 = temp_r2->next;
                    var_r3->next = temp_r0;
                    temp_r0->tail = var_r3;
                    if (temp_r2 == var_r4->unk10) {
                        var_r4->unk10 = var_r3;
                    }
                }
            }
        }
        current = current->next;
    }
}

void mem_collect(s32 arg0) {
    if (0x80000000 & arg0) {
        mem_collect_heap(mem_iwram_heap);
    }
    if (0x40000000 & arg0) {
        mem_collect_heap(mem_ewram_heap);
    }
}
#endif