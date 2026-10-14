#ifndef GUARD_OBJ_TILE_ALLOCATOR_HPP
#define GUARD_OBJ_TILE_ALLOCATOR_HPP

/* The OBJ tile allocator (src/engine/gfx/sprite_frame.cpp, #915): a next-fit
 * allocator of OBJ tile VRAM, from `base` to the end of OBJ_VRAM0. It
 * keeps its blocks in a separate pool of `struct vram_tile_block` records
 * (sprite_frame.cpp), and a per-tile index back to them. The sprite frame
 * cache (SpriteFrameCache::LoadTiles), the company logos and the 3D players
 * allocate from it.
 *
 * An all-static class, like the yeti and the airship (docs/cplusplus.md,
 * "The OBJ tile allocator as an all-static class"): its variables are
 * separate IWRAM globals (0x03001320-0x03001343) that no other object
 * reads, and static data members compile like those globals, static
 * member functions like plain functions. The data members are only
 * declared: cxx_symbols.txt (block "#915 ObjTileAllocator") maps each one
 * to its C name (`_Q36Engine3Gfx16ObjTileAllocator.rover` is gVramTileBlockRover,
 * ...), whose address is in sym_iwram.txt, and the functions keep their
 * C names the same way (`Alloc__Q36Engine3Gfx16ObjTileAllocatori` is
 * AllocVramTileBlock).
 *
 * In the namespace Engine::Gfx (#1033, docs/cplusplus.md "Namespaces").
 *
 * `#pragma interface`: no vtable to emit; it keeps g++ from emitting
 * out-of-line copies of inline methods (docs/cplusplus.md). */
#pragma interface

extern "C" {
#include "core.h"
}

struct vram_tile_block;

namespace Engine
{
namespace Gfx
{

class ObjTileAllocator
{
public:
    static void Init(void *base); // InitObjTileFreeList
    static void Free(void *addr); // FreeVramTileBlock
    static void *Alloc(s32 size); // AllocVramTileBlock
    static void Walk();           // WalkVramTileBlocks: UNUSED
    static s32 GetFreeBytes();    // GetFreeVramTileBytes: UNUSED
    static void Destroy();        // FreeObjTileFreeList

private:
    /* IWRAM (sym_iwram.txt). `list` is the sentinel of the blocks'
     * address-ordered ring, `rover` where the next-fit search starts,
     * `spares` the stack of unused records, and `index` the record of
     * each tile (TOTAL_OBJ_TILE_COUNT bytes). */
    static struct vram_tile_block *pool;   // gVramTileBlockPool - the records
    static struct vram_tile_block list;    // gVramTileBlockList
    static struct vram_tile_block *rover;  // gVramTileBlockRover
    static struct vram_tile_block *spares; // gVramTileBlockSpares
    static u8 *index;                      // gVramTileBlockIndex
};

} // namespace Gfx
} // namespace Engine

#endif // GUARD_OBJ_TILE_ALLOCATOR_HPP
