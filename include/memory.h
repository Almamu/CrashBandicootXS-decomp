#ifndef __MEMORY_H__
#define __MEMORY_H__

struct unk {
    int size;
    int status;
    struct unk* next;
    struct unk* tail;
    struct unk* unk10;
};

#define MEMORY_STATUS_FREE 0
#define MEMORY_STATUS_USED 1
#define MEMORY_STATUS_COLLECT 2

extern struct unk gUnknown_02000000[2];
extern struct unk gUnknown_03001638[2];
extern struct unk* mem_iwram_heap_pointer;
extern struct unk* mem_ewram_heap_pointer;
extern int* gUnknown_030007D4;
extern int iwram_end;

// TODO: THIS SHOULD NOT BE PUBLIC, BUT UNTIL THE WHOLE MEMORY.C CONTENT IS REVERSED
// WE NEED TO LEAVE IT LIKE TI IS, BUT THE INLINED FUNCTION SHOULD BE USED INSTEAD OF THIS
s32 mem_free_bytes(s32 arg0);

#endif /* !__MEMORY_H__ */