#ifndef GUARD_PC_GBA_HOST_H
#define GUARD_PC_GBA_HOST_H

/* The pc platform's host memory (platform/pc/src/memory.c): the arrays
 * its gba/io_reg.h and gba/defines.h point the GBA's memory map at, and
 * the DMA call its gba/dma_macros.h makes (docs/platforms.md). */

/* The sizes are the GBA's, except the two work RAMs: the linker-placed
 * IWRAM globals sit at twice their GBA offset (sym_iwram.txt, laid out by
 * platform/pc/tools/host_link.py), so a struct that holds pointers has the
 * room it takes on a 64-bit host, and the heaps (memory.cpp, EWRAM_SIZE)
 * get twice the room too. */
#define HOST_IO_SIZE    0x400
#define HOST_EWRAM_SIZE 0x80000
#define HOST_IWRAM_SIZE 0x10000
#define HOST_PLTT_SIZE  0x400
#define HOST_VRAM_SIZE  0x18000
#define HOST_OAM_SIZE   0x400
#define HOST_NULL_PAGE_SIZE 0x1000

/* memory.S includes the sizes. */
#ifndef __ASSEMBLER__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned char gHostIo[HOST_IO_SIZE];
extern unsigned char gHostEwram[HOST_EWRAM_SIZE];
extern unsigned char gHostIwram[HOST_IWRAM_SIZE];
extern unsigned char gHostPltt[HOST_PLTT_SIZE];
extern unsigned char gHostVram[HOST_VRAM_SIZE];
extern unsigned char gHostOam[HOST_OAM_SIZE];
/* What a NULL_READ of a NULL pointer reads (gba/defines.h): zeros. */
extern unsigned char gHostNullPage[HOST_NULL_PAGE_SIZE];

/* DmaSet: writes the channel's registers and, for an immediate transfer,
 * does it at once (platform/pc/src/dma.c). */
void HostDmaSet(int channel, uintptr_t src, uintptr_t dest, uint32_t control);

/* REG_KEYINPUT: the keys (active low) of this game frame
 * (platform/pc/src/input.c). */
uint16_t HostReadKeys(void);

#ifdef __cplusplus
}
#endif

#endif /* !__ASSEMBLER__ */

#endif /* GUARD_PC_GBA_HOST_H */
