#ifndef __MEMORY_H__
#define __MEMORY_H__

struct mem_block {
    int size;               // 0x00
    int status;             // 0x04
    struct mem_block *next; // 0x08
    struct mem_block *tail; // 0x0C
    u8 buffer[0];
};

struct mem_heap_header {
    struct mem_block header;
    struct mem_block *nextFreeBlock; // 0x10
};

struct mem_heap {
    struct mem_heap_header base;
    struct mem_block mainblock;
};

#define MEMORY_STATUS_FREE 0
#define MEMORY_STATUS_USED 1
#define MEMORY_STATUS_COLLECT 2

#define MEM_HEAP_IWRAM 0x80000000
#define MEM_HEAP_EWRAM 0x40000000
#define MEM_HEAP_BOTH (MEM_HEAP_EWRAM | MEM_HEAP_IWRAM)

// ensure some structs don't change size
COMPILE_TIME_ASSERT(memory_h, sizeof(struct mem_block) == 0x10);
COMPILE_TIME_ASSERT(memory_h, sizeof(struct mem_heap_header) == 0x14);

extern struct mem_heap mem_ewram_heap;
extern struct mem_heap mem_iwram_heap;
extern struct mem_heap *mem_iwram_heap_pointer;
extern struct mem_heap *mem_ewram_heap_pointer;
extern s32 mem_initial_free_bytes;
extern int iwram_end;

// TODO: THIS SHOULD NOT BE PUBLIC, BUT UNTIL THE WHOLE MEMORY.C CONTENT IS REVERSED
// WE NEED TO LEAVE IT LIKE TI IS, BUT THE INLINED FUNCTION SHOULD BE USED INSTEAD OF THIS
s32 mem_free_bytes(s32 arg0);
void *mem_alloc(u32 requestedSize, u32 flags);
void mem_free(void *address);

s32 mem_heap_init(u32 arg0);
void mem_collect(s32 arg0);
void mem_heap_shutdown(void);

/* The C++ new/delete operators: EWRAM allocations through mem_alloc/
 * mem_free. src/system/operator_new.cpp defines them as the global
 * `operator new` & co.; these are their C names (cxx_symbols.txt),
 * for the code that calls them by name. */
extern void OperatorDeleteArray(void *ptr);
extern void *OperatorNewArray(u32 size);
extern void OperatorDelete(void *ptr);
extern void *OperatorNew(u32 size);

/* operator new[] of a type with a destructor stores the element count in
 * the word before the array (the block OperatorNewArray returned), and
 * the destructor reads it back to walk the array and frees that block.
 * The game builds these by hand (InitHud, src/hud/hud_init.cpp). */
#define NEW_ARRAY_COUNT(array) (((s32 *)(array))[-1])
#define NEW_ARRAY_BLOCK(array) ((void *)((s32 *)(array) - 1))

#endif /* !__MEMORY_H__ */