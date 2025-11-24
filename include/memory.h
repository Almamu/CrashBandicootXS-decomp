#ifndef __MEMORY_H__
#define __MEMORY_H__

struct mem_block {
    int size; // 0x00
    int status; // 0x04
    struct mem_block* next; // 0x08
    struct mem_block* tail; // 0x0C
    u8 buffer[0];
};

struct mem_heap_header {
    struct mem_block header;
    struct mem_block* nextFreeBlock; // 0x10
};

struct mem_heap {
    struct mem_heap_header base;
    struct mem_block mainblock;
};

#define MEMORY_STATUS_FREE 0
#define MEMORY_STATUS_USED 1
#define MEMORY_STATUS_COLLECT 2

// ensure some structs don't change size
COMPILE_TIME_ASSERT(sizeof (struct mem_block) == 0x10);
COMPILE_TIME_ASSERT(sizeof (struct mem_heap_header) == 0x14);

extern struct mem_heap gUnknown_02000000;
extern struct mem_heap gUnknown_03001638;
extern struct mem_heap* mem_iwram_heap_pointer;
extern struct mem_heap* mem_ewram_heap_pointer;
extern int* gUnknown_030007D4;
extern int iwram_end;

// TODO: THIS SHOULD NOT BE PUBLIC, BUT UNTIL THE WHOLE MEMORY.C CONTENT IS REVERSED
// WE NEED TO LEAVE IT LIKE TI IS, BUT THE INLINED FUNCTION SHOULD BE USED INSTEAD OF THIS
s32 mem_free_bytes(s32 arg0);
u8* mem_alloc(u32 requestedSize, s32 arg1);
void mem_free(u8* address);

#endif /* !__MEMORY_H__ */