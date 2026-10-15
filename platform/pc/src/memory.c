/* The host memory the pc platform's headers point the GBA's memory map
 * at (gba/host.h, gba/io_reg.h, gba/defines.h). The linker-placed IWRAM
 * and EWRAM globals (sym_iwram.txt, sym_ewram.txt) are assigned inside
 * gHostIwram and gHostEwram by the link (platform/pc/tools/host_link.py). */

#include "gba/host.h"

#define HOST_ALIGNED __attribute__((aligned(64)))

unsigned char gHostIo[HOST_IO_SIZE] HOST_ALIGNED;
unsigned char gHostEwram[HOST_EWRAM_SIZE] HOST_ALIGNED;
unsigned char gHostIwram[HOST_IWRAM_SIZE] HOST_ALIGNED;
unsigned char gHostPltt[HOST_PLTT_SIZE] HOST_ALIGNED;
unsigned char gHostVram[HOST_VRAM_SIZE] HOST_ALIGNED;
unsigned char gHostOam[HOST_OAM_SIZE] HOST_ALIGNED;
