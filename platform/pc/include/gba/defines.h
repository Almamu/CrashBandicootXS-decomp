#ifndef GUARD_PC_GBA_DEFINES_H
#define GUARD_PC_GBA_DEFINES_H

/* The GBA's defines, with the memory map on the host's arrays
 * (platform/pc/src/memory.c), as integers like the GBA's
 * (docs/platforms.md, "64-bit hosts"). */
#include "../../../gba/include/gba/defines.h"
#include "./host.h"

/* No IWRAM/EWRAM sections on the host: ordinary data. */
#undef IWRAM_DATA
#undef EWRAM_DATA
#define IWRAM_DATA
#define EWRAM_DATA

/* The ROM's layout of a pointer-holding struct holds only for 4-byte
 * pointers: checked on the GBA, not here (still a typedef, so the `;`
 * after it stays valid). */
#undef LAYOUT_ASSERT
#define LAYOUT_ASSERT(T, X) COMPILE_TIME_ASSERT(T, 1)

/* A NULL pointer the ROM reads through (gba/defines.h) reads the host's
 * zeroed page instead, where the GBA reads its BIOS area. */
#undef NULL_READ
#define NULL_READ(p) ((p) != 0 ? (p) : (__typeof__(p))gHostNullPage)

/* A free of the ROM's own data (gba/defines.h): its writes would fault
 * in the host's read-only data, so it is skipped. The heaps are in
 * gHostEwram and gHostIwram, in that order in one block (memory.S). */
#undef MEM_FREE_LOST
#define MEM_FREE_LOST(p) \
    ((uintptr_t)(p) < (uintptr_t)gHostEwram || (uintptr_t)(p) >= (uintptr_t)gHostIwram + HOST_IWRAM_SIZE)

#undef EWRAM_START
#undef IWRAM_START
#undef EWRAM_SIZE
#undef IWRAM_SIZE
#define EWRAM_START ((uintptr_t)gHostEwram)
#define IWRAM_START ((uintptr_t)gHostIwram)
#define EWRAM_SIZE  HOST_EWRAM_SIZE
#define IWRAM_SIZE  (HOST_IWRAM_SIZE - 0x400)

/* The BIOS's words at the top of IWRAM, at twice their GBA offset like
 * the rest of the host IWRAM (gba/host.h): INTR_VECTOR is a pointer. */
#undef IWRAM_SOUND_INFO_OFFSET
#undef IWRAM_INTR_CHECK_OFFSET
#undef IWRAM_INTR_VECTOR_OFFSET
#define IWRAM_SOUND_INFO_OFFSET  (2 * 0x7FF0)
#define IWRAM_INTR_CHECK_OFFSET  (2 * 0x7FF8)
#define IWRAM_INTR_VECTOR_OFFSET (2 * 0x7FFC)

#undef PLTT
#undef VRAM
#undef OAM
#define PLTT ((uintptr_t)gHostPltt)
#define VRAM ((uintptr_t)gHostVram)
#define OAM  ((uintptr_t)gHostOam)

#endif /* GUARD_PC_GBA_DEFINES_H */
