/* The host memory the pc platform's headers point the GBA's memory map
 * at (gba/host.h, gba/io_reg.h, gba/defines.h). The linker-placed IWRAM
 * and EWRAM globals (sym_iwram.txt, sym_ewram.txt) are assigned inside
 * gHostIwram and gHostEwram by the link (platform/pc/tools/host_link.py).
 *
 * In assembly so that the regions are in the GBA's address order (EWRAM
 * 0x02, IWRAM 0x03, I/O 0x04, palette 0x05, VRAM 0x06, OAM 0x07), one
 * block, which a C compiler doesn't promise for separate arrays: the
 * allocator tells its two heaps apart by comparing a block's address
 * with the IWRAM heap's (memory.cpp, mem_free: IWRAM is above EWRAM).
 *
 * Each region starts on a 1 MB boundary, as the GBA's start on 16 MB
 * ones: the game takes an OBJ tile's number from the low bits of its
 * VRAM address (company_logos.cpp's `tilesB >> 5` as a 10-bit tileNum),
 * which needs VRAM aligned at least as the GBA's 0x06010000 is. */

#include "gba/host.h"

    .bss

#define REGION(name, bytes) \
    .balign 0x100000;      \
    .globl name;           \
    .type name, @object;   \
    .size name, bytes;     \
name:                      \
    .zero bytes

REGION(gHostEwram, HOST_EWRAM_SIZE)
REGION(gHostIwram, HOST_IWRAM_SIZE)
REGION(gHostIo, HOST_IO_SIZE)
REGION(gHostPltt, HOST_PLTT_SIZE)
REGION(gHostVram, HOST_VRAM_SIZE)
REGION(gHostOam, HOST_OAM_SIZE)

/* Not the GBA's: what a NULL_READ pointer reads when it is NULL. */
    .balign 64
    .globl gHostNullPage
    .type gHostNullPage, @object
    .size gHostNullPage, HOST_NULL_PAGE_SIZE
gHostNullPage:
    .zero HOST_NULL_PAGE_SIZE

/* Not the GBA's either: where a NULL_WRITE pointer writes when it is NULL. */
    .balign 64
    .globl gHostNullSink
    .type gHostNullSink, @object
    .size gHostNullSink, HOST_NULL_PAGE_SIZE
gHostNullSink:
    .zero HOST_NULL_PAGE_SIZE

    .section .note.GNU-stack,"",@progbits
