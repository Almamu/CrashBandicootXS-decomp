/* IwramFree/IwramAlloc: the IWRAM heap's free/alloc wrappers over
 * mem_free/mem_alloc. Split from util/aabb.cpp (#767), same flags
 * (old_agbcc). */

extern "C" {
#include "core.h"
#include "memory.h"
#include "util.h"
}

void IwramFree(u8 *address)
{
    mem_free(address);
}

void *IwramAlloc(u32 size)
{
    return mem_alloc(size, MEM_HEAP_IWRAM);
}
