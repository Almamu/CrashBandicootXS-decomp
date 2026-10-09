extern "C" {
#include "core.h"
#include "memory.h"
}

/* The game's global `operator delete[]`, `operator new[]`, `operator
 * delete` and `operator new` (in ROM order; OperatorDeleteArray,
 * OperatorNewArray, OperatorDelete and OperatorNew, cxx_symbols.txt):
 * EWRAM allocations through mem_alloc/mem_free, replacing libgcc's
 * (docs/cplusplus.md). ROM 0x08026EB4-0x08026EEC, right after the camera
 * (src/level/camera.cpp, where they were until #770; GitHub issue #44). */

void operator delete[](void *ptr)
{
    mem_free(ptr);
}

void *operator new[](size_t size)
{
    return mem_alloc(size, MEM_HEAP_EWRAM);
}

void operator delete(void *ptr)
{
    mem_free(ptr);
}

void *operator new(size_t size)
{
    return mem_alloc(size, MEM_HEAP_EWRAM);
}
