#ifndef GUARD_VRAM_DMA_QUEUE_HPP
#define GUARD_VRAM_DMA_QUEUE_HPP

/* The VRAM DMA queue (src/engine/sys/vram_dma_queue.cpp, #915): the transfers to
 * VRAM and palette RAM queued during a frame (sprite frames, palette
 * flashes, BG tiles), which the frame loops run with Flush once the
 * frame is drawn, usually right after the VBlank wait.
 *
 * An all-static class, like the OAM queue (docs/cplusplus.md, "The VRAM
 * DMA queue as an all-static class"): its one variable is an IWRAM
 * global that no other object reads, and a static data member compiles
 * like that global, static member functions like plain functions. The
 * data member is only declared: cxx_symbols.txt (block "#915
 * VramDmaQueue") maps it to its C name (`_Q36Engine3Sys12VramDmaQueue.queue` is
 * gVramDmaQueue), whose address is in sym_iwram.txt, and the functions
 * keep their C names the same way (`Flush__Q36Engine3Sys12VramDmaQueue` is
 * FlushVramDmaQueue).
 *
 * In the namespace Engine::Sys (#1033, docs/cplusplus.md "Namespaces").
 *
 * `#pragma interface`: no vtable to emit; it keeps g++ from emitting
 * out-of-line copies of inline methods (docs/cplusplus.md). */
#pragma interface

extern "C" {
#include "core.h"
}

struct dma_queue;

namespace Engine
{
namespace Sys
{

class VramDmaQueue
{
public:
    static void Flush(); // FlushVramDmaQueue: runs the queue, then waits for the last one
    /* QueueVramDmaTransfer: `size` bytes in `unit`-bit units (0x10 or
     * 0x20); 0, or -1 when the queue is full */
    static s32 Add(const void *src, void *dest, u16 size, u16 unit);
    static void Destroy(); // FreeVramDmaQueue
    static s32 Init();     // AllocVramDmaQueue: 0, or -1 when out of EWRAM

private:
    /* IWRAM (sym_iwram.txt): the EWRAM entries and their count, one
     * struct (vram_dma_queue.cpp) */
    static struct dma_queue queue; // gVramDmaQueue
};

} // namespace Sys
} // namespace Engine

#endif // GUARD_VRAM_DMA_QUEUE_HPP
