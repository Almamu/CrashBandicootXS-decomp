#ifndef GUARD_PC_GBA_DMA_MACROS_H
#define GUARD_PC_GBA_DMA_MACROS_H

/* The GBA's DMA macros, with DmaSet, which all the others go through, a
 * call: a 64-bit address doesn't fit the GBA's 32-bit DMA registers, so
 * the transfer is done by the host (HostDmaSet, platform/pc/src/dma.c). */
#include "../../../gba/include/gba/dma_macros.h"
#include "./host.h"

#undef DmaSet
#define DmaSet(dmaNum, src, dest, control) \
    { HostDmaSet(dmaNum, (uintptr_t)(src), (uintptr_t)(dest), (u32)(control)); }

#endif /* GUARD_PC_GBA_DMA_MACROS_H */
